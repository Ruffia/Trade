// XTPFlowGraphConnectionPoint.cpp : implementation of the CXTPFlowGraphConnectionPoint class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphNode.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphPaintManager.h"
#include "FlowGraph/XTPFlowGraphConnectionPoint.h"
#include "FlowGraph/XTPFlowGraphImage.h"
#include "FlowGraph/XTPFlowGraphUndoManager.h"
#include "FlowGraph/XTPFlowGraphConnection.h"
#include "FlowGraph/XTPFlowGraphConnectionPoints.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPFlowGraphConnectionPoint, CXTPCmdTarget,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPFlowGraphConnectionPoint::CXTPFlowGraphConnectionPoint()
{
	m_nStyle   = -1;
	m_clrPoint = (COLORREF)RGB(0, 0, 0);

	m_nId = (int)(INT_PTR)this;

	m_nType = xtpFlowGraphPointInput;

	m_bLocked = FALSE;

	m_nImageIndex = -1;

	m_pNode = NULL;

	m_nMaxConnections = -1;

	m_rcPoint.SetRectEmpty();

	m_nConnectionPointIndex = -1;

	m_bMarkupPoint = FALSE;

	m_bInputOutPair = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphConnectionPoint::~CXTPFlowGraphConnectionPoint()
{
}

void CXTPFlowGraphConnectionPoint::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Name"), m_strName, _T(""));

	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_DWord(pPX, _T("Color"), (DWORD&)m_clrPoint, (COLORREF)RGB(0, 0, 0));
	PX_Int(pPX, _T("Style"), m_nStyle, (int)-1);
	PX_Enum(pPX, _T("Type"), m_nType, xtpFlowGraphPointInput);

	// do not store ID, as it's not used for new connections
	// PX_Int(pPX, _T("Id"), m_nId, 0);

	PX_Bool(pPX, _T("Locked"), m_bLocked, FALSE);

	PX_Int(pPX, _T("MaxConnections"), m_nMaxConnections, -1);

	PX_Int(pPX, _T("ImageIndex"), m_nImageIndex, -1);

	if (pPX->IsLoading())
	{
		CXTPFlowGraphControl* pControl = GetControl();
		if (pControl)
		{
			m_pNode->AddNamedConnectionPoints(m_strName, m_nType, m_strCaption);
		}

		m_pNode->UpdateMarkupPointColor(this);
		m_pNode->UpdateMarkupPointImage(this);
	}
}

CXTPFlowGraphControl* CXTPFlowGraphConnectionPoint::GetControl() const
{
	if (m_pNode)
		return m_pNode->GetControl();
	return NULL;
}

CXTPFlowGraphPage* CXTPFlowGraphConnectionPoint::GetPage() const
{
	if (m_pNode)
		return m_pNode->GetPage();
	return NULL;
}

void CXTPFlowGraphConnectionPoint::Remove()
{
	if (!m_pNode)
		return;

	m_pNode->GetConnectionPoints()->Remove(this);
}

BOOL CXTPFlowGraphConnectionPoint::HitTestConnectionArea(CPoint point) const
{
	CXTPFlowGraphPaintManager* pPaintManager = GetControl()->GetPaintManager();

	return pPaintManager->HitTestConnectionArea(this, point);
}

CXTPFlowGraphImage* CXTPFlowGraphConnectionPoint::GetImage() const
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return NULL;

	return pControl->GetImages()->GetAt(m_nImageIndex);
}

void CXTPFlowGraphConnectionPoint::OnGraphChanged()
{
	if (m_pNode)
		m_pNode->OnGraphChanged();
}

void CXTPFlowGraphConnectionPoint::SetCaption(LPCTSTR lpszCaption)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(
			new CXTPFlowGraphUndoSetNodeCaptionCommand(this, m_strCaption));
	}

	CString oldCaption = m_strCaption;

	m_strCaption = lpszCaption;

	if (m_pNode)
		m_pNode->UpdateMarkupPointCaption(this);

	OnGraphChanged();
}

void CXTPFlowGraphConnectionPoint::SetName(LPCTSTR lpszName)
{
	m_strName = lpszName;

	if (m_pNode)
		m_pNode->UpdateMarkupPointName(this);
}

void CXTPFlowGraphConnectionPoint::OnRemoved()
{
	if (m_bMarkupPoint)
	{
		m_pNode->RemoveMarkupTableRow(this);
	}

	int i;

	for (i = (int)m_arrInputConnections.GetSize() - 1; i >= 0; i--)
	{
		m_arrInputConnections[i]->Remove();
	}

	for (i = (int)m_arrOutputConnections.GetSize() - 1; i >= 0; i--)
	{
		m_arrOutputConnections[i]->Remove();
	}

	// Removed Connections should update them
	ASSERT(m_arrInputConnections.GetSize() == 0);
	ASSERT(m_arrOutputConnections.GetSize() == 0);

	CXTPFlowGraphElement::OnRemoved();
}

void CXTPFlowGraphConnectionPoint::SetColor(COLORREF clr)
{
	m_clrPoint = clr;

	m_pNode->UpdateMarkupPointColor(this);
}

void CXTPFlowGraphConnectionPoint::SetImageIndex(int nImageIndex)
{
	m_nImageIndex = nImageIndex;

	m_pNode->UpdateMarkupPointImage(this);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphConnectionPoint, CXTPFlowGraphElement)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "Caption", DISPID_CAPTION, OleGetCaption,
						OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "Tooltip", 2, OleGetTooltip, OleSetTooltip,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "Color", 3, OleGetColor, OleSetColor,
						VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "Style", 4, OleGetStyle, OleSetStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPFlowGraphConnectionPoint, "Id", 5, m_nId, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "Type", 6, OleGetType, OleSetType, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "Node", 7, OleGetNode, SetNotSupported,
						VT_DISPATCH)

	DISP_PROPERTY_ID(CXTPFlowGraphConnectionPoint, "Locked", 8, m_bLocked, VT_BOOL)
	DISP_PROPERTY_ID(CXTPFlowGraphConnectionPoint, "MaxConnections", 9, m_nMaxConnections, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "ConnectionsCount", 10, GetConnectionsCount,
						SetNotSupported, VT_I4)

	DISP_PROPERTY_ID(CXTPFlowGraphConnectionPoint, "ImageIndex", 14, m_nImageIndex, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "InputConnectionsCount", 15,
						GetInputConnectionsCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnectionPoint, "OutputConnectionsCount", 16,
						GetOutputConnectionsCount, SetNotSupported, VT_I4)

	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoint, "InputConnection", 17, OleGetInputConnection,
					 VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoint, "OutputConnection", 18, OleGetOutputConnection,
					 VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPFlowGraphNode, "Remove", 19, Remove, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphConnectionPoint, CXTPFlowGraphElement)
	INTERFACE_PART(CXTPFlowGraphConnectionPoint, XTPDIID_FlowGraphConnectionPoint, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphConnectionPoint, XTPDIID_FlowGraphConnectionPoint)

void CXTPFlowGraphConnectionPoint::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(lpszCaption);
}

BSTR CXTPFlowGraphConnectionPoint::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

void CXTPFlowGraphConnectionPoint::OleSetName(LPCTSTR lpszName)
{
	SetName(lpszName);
}

BSTR CXTPFlowGraphConnectionPoint::OleGetName()
{
	return m_strName.AllocSysString();
}

void CXTPFlowGraphConnectionPoint::OleSetTooltip(LPCTSTR lpszTooltip)
{
	SetTooltip(lpszTooltip);
}

BSTR CXTPFlowGraphConnectionPoint::OleGetTooltip()
{
	return m_strTooltip.AllocSysString();
}

void CXTPFlowGraphConnectionPoint::OleSetColor(OLE_COLOR clr)
{
	SetColor(AxTranslateColor(clr));
}

OLE_COLOR CXTPFlowGraphConnectionPoint::OleGetColor()
{
	return m_clrPoint;
}

void CXTPFlowGraphConnectionPoint::OleSetStyle(int nStyle)
{
	SetStyle(nStyle);
}

int CXTPFlowGraphConnectionPoint::OleGetStyle()
{
	return GetStyle();
}

void CXTPFlowGraphConnectionPoint::OleSetType(int nType)
{
	SetType((XTPFlowGraphConnectionPointType)nType);
}

int CXTPFlowGraphConnectionPoint::OleGetType()
{
	return GetType();
}

LPDISPATCH CXTPFlowGraphConnectionPoint::OleGetNode()
{
	return XTPGetDispatch(GetNode());
}

LPDISPATCH CXTPFlowGraphConnectionPoint::OleGetInputConnection(int nIndex)
{
	return XTPGetDispatch(GetInputConnectionAt(nIndex));
}

LPDISPATCH CXTPFlowGraphConnectionPoint::OleGetOutputConnection(int nIndex)
{
	return XTPGetDispatch(GetOutputConnectionAt(nIndex));
}

#endif
