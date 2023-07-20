// XTPFlowGraphConnection.cpp : implementation of the CXTPFlowGraphConnection class.
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPSystemHelpers.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphPaintManager.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphConnection.h"
#include "FlowGraph/XTPFlowGraphConnections.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphNode.h"
#include "FlowGraph/XTPFlowGraphNodes.h"
#include "FlowGraph/XTPFlowGraphConnectionPoint.h"
#include "FlowGraph/XTPFlowGraphConnectionPoints.h"
#include "FlowGraph/XTPFlowGraphUndoManager.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPFlowGraphConnection, CXTPFlowGraphElement,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPFlowGraphConnection::CXTPFlowGraphConnection()
{
	m_pPage = NULL;

	m_pInputPoint  = NULL;
	m_pOutputPoint = NULL;

	m_nInputId = m_nOutputId = 0;

	m_nInputPointIndex  = -1;
	m_nOutputPointIndex = -1;

	m_clrConnection = (COLORREF)-1;
	m_nStyle		= -1;

	m_ptInputPoint  = CPoint(-1, -1);
	m_ptOutputPoint = CPoint(-1, -1);

	m_ptControlPoint = CPoint(-1, -1);

	m_rcBoundingRect.SetRectEmpty();

	m_pPath = NULL;

	m_pNextVisibleConnection = NULL;

	m_nInputPointConnectionIndex  = -1;
	m_nOutputPointConnectionIndex = -1;
	m_nConnectionIndex			  = -1;

	m_bPathReversed = FALSE;

	m_nWidth  = 2;
	m_bDashed = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphConnection::~CXTPFlowGraphConnection()
{
	CMDTARGET_RELEASE(m_pInputPoint);
	CMDTARGET_RELEASE(m_pOutputPoint);

	SAFE_DELETE(m_pPath);
}

CXTPFlowGraphControl* CXTPFlowGraphConnection::GetControl() const
{
	if (!m_pPage)
		return NULL;
	return m_pPage->GetControl();
}

int CXTPFlowGraphConnection::GetConnectionPointIndex(CXTPFlowGraphConnectionPoint* pPoint) const
{
	int iIndex = -1;

	int i = 0, c = 0;

	for (i = 0, c = pPoint->GetNode()->GetConnectionPoints()->GetCount(); i < c; i++)
	{
		if (pPoint->GetNode()->GetConnectionPoints()->GetAt(i) == pPoint)
		{
			iIndex = i;

			break;
		}
	}

	return iIndex;
}

void CXTPFlowGraphConnection::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_String(pPX, _T("Tag"), m_strTag, _T(""));

	PX_DWord(pPX, _T("Color"), (DWORD&)m_clrConnection, (COLORREF)-1);
	PX_Int(pPX, _T("Style"), m_nStyle, (int)-1);
	PX_Int(pPX, _T("Width"), m_nWidth, 2);
	PX_Bool(pPX, _T("Dashed"), m_bDashed, FALSE);

	PX_Point(pPX, _T("ControlPoint"), m_ptControlPoint, CPoint(-1, -1));

	if (pPX->IsStoring())
	{
		m_nInputId = m_pInputPoint ? m_pInputPoint->GetNode()->GetID() : 0;
		PX_Int(pPX, _T("InputNode"), m_nInputId, 0);

		m_nOutputId = m_pOutputPoint ? m_pOutputPoint->GetNode()->GetID() : 0;
		PX_Int(pPX, _T("OutputNode"), m_nOutputId, 0);

		m_nInputPointIndex  = GetConnectionPointIndex(m_pInputPoint);
		m_nOutputPointIndex = GetConnectionPointIndex(m_pOutputPoint);

		ASSERT(m_nInputPointIndex != -1);
		ASSERT(m_nOutputPointIndex != -1);

		PX_Int(pPX, _T("InputPointIndex"), m_nInputPointIndex, -1);
		PX_Int(pPX, _T("OutputPointIndex"), m_nOutputPointIndex, -1);
	}
	else
	{
		PX_Int(pPX, _T("InputNode"), m_nInputId, 0);
		PX_Int(pPX, _T("OutputNode"), m_nOutputId, 0);

		PX_Int(pPX, _T("InputPointIndex"), m_nInputPointIndex, -1);
		PX_Int(pPX, _T("OutputPointIndex"), m_nOutputPointIndex, -1);
	}
}

void CXTPFlowGraphConnection::SetControlPoint(CPoint pt)
{
	if (m_ptControlPoint == pt)
		return;

	GetControl()->GetUndoManager()->AddUndoCommand(
		new CXTPFlowGraphUndoSetConnectionPositionCommand(this, m_ptControlPoint));

	m_ptControlPoint = pt;
	OnGraphChanged();
}

void CXTPFlowGraphConnection::RestoreConnection()
{
	ASSERT(m_pInputPoint == NULL && m_pOutputPoint == NULL);
	ASSERT(m_pPage != NULL);

	ASSERT(m_nInputPointIndex != -1);
	ASSERT(m_nOutputPointIndex != -1);

	CXTPFlowGraphNode* pNodeIn  = NULL;
	CXTPFlowGraphNode* pNodeOut = NULL;

	for (int i = 0; i < m_pPage->GetNodes()->GetCount(); i++)
	{
		CXTPFlowGraphNode* pNode = m_pPage->GetNodes()->GetAt(i);

		if (pNode->GetID() == m_nInputId) // this is input node
			pNodeIn = pNode;
		else if (pNode->GetID() == m_nOutputId) // output node
			pNodeOut = pNode;

		if (pNodeIn && pNodeOut)
			break;
	}

	ASSERT(pNodeOut && pNodeIn);

	if (pNodeIn && pNodeOut)
	{
		ASSERT(m_nInputPointIndex < pNodeIn->GetConnectionPoints()->GetCount());

		if (m_nInputPointIndex < pNodeIn->GetConnectionPoints()->GetCount())
		{
			SetInputPoint(pNodeIn->GetConnectionPoints()->GetAt(m_nInputPointIndex));
		}

		ASSERT(m_nOutputPointIndex < pNodeOut->GetConnectionPoints()->GetCount());

		if (m_nOutputPointIndex < pNodeOut->GetConnectionPoints()->GetCount())
		{
			SetOutputPoint(pNodeOut->GetConnectionPoints()->GetAt(m_nOutputPointIndex));
		}
	}
}

void CXTPFlowGraphConnection::RecalcLayout(CXTPFlowGraphDrawContext* pDC)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return;

	pControl->GetPaintManager()->RecalcConnectionLayout(pDC, this);

	Gdiplus::Rect rcBounds;

	if (m_pPath) // caused exception!
		m_pPath->GetBounds(&rcBounds);

	m_rcBoundingRect.SetRect(rcBounds.GetLeft(), rcBounds.GetTop(), rcBounds.GetRight(),
							 rcBounds.GetBottom());
}

CRect CXTPFlowGraphConnection::GetBoundingRect()
{
	return m_rcBoundingRect;
}

BOOL CXTPFlowGraphConnection::PtInRect(POINT pt)
{
	if (!m_pPath)
		return FALSE;

	double x = pt.x;
	double y = pt.y;
	m_pPage->ScreenToPage(x, y);

#ifdef _DEBUG
#	pragma push_macro("new")
#	undef new
#endif
	Gdiplus::Pen* pPen = new Gdiplus::Pen(0xFF000000, (float)(m_nWidth + 1));
#ifdef _DEBUG
#	pragma pop_macro("new")
#endif

	BOOL res = m_pPath->IsOutlineVisible((float)x, (float)y, pPen);

	delete pPen;

	return res;
}

void CXTPFlowGraphConnection::Remove()
{
	if (!m_pPage)
		return;

	m_pPage->GetConnections()->Remove(this);
}

void CXTPFlowGraphConnection::OnRemoved()
{
	SAFE_DELETE(m_pPath);

	SetInputPoint(NULL);
	SetOutputPoint(NULL);

	CXTPFlowGraphElement::OnRemoved();
}

void CXTPFlowGraphConnection::SetInputPoint(CXTPFlowGraphConnectionPoint* pPoint)
{
	if (m_pInputPoint == pPoint)
		return;

	if (m_pInputPoint)
	{
		CArray<CXTPFlowGraphConnection*, CXTPFlowGraphConnection*>& arrInputConnections =
			m_pInputPoint->m_arrInputConnections;

		int nCount = (int)arrInputConnections.GetSize();

		ASSERT(m_nInputPointConnectionIndex != -1);
		ASSERT(arrInputConnections[m_nInputPointConnectionIndex] == this);

		if (m_nInputPointConnectionIndex != nCount - 1)
		{
			arrInputConnections[m_nInputPointConnectionIndex] = arrInputConnections[nCount - 1];

			arrInputConnections[m_nInputPointConnectionIndex]->m_nInputPointConnectionIndex =
				m_nInputPointConnectionIndex;
		}

		arrInputConnections.SetSize(nCount - 1);

		m_nInputPointConnectionIndex = -1;
		CMDTARGET_RELEASE(m_pInputPoint);
	}

	m_pInputPoint = pPoint;

	if (pPoint)
	{
		m_nInputPointConnectionIndex = (int)pPoint->m_arrInputConnections.Add(this);

		CMDTARGET_ADDREF(pPoint);
	}

	OnGraphChanged();
}

void CXTPFlowGraphConnection::SetOutputPoint(CXTPFlowGraphConnectionPoint* pPoint)
{
	if (m_pOutputPoint == pPoint)
		return;

	if (m_pOutputPoint)
	{
		CArray<CXTPFlowGraphConnection*, CXTPFlowGraphConnection*>& arrOutputConnections =
			m_pOutputPoint->m_arrOutputConnections;

		int nCount = (int)arrOutputConnections.GetSize();

		ASSERT(m_nOutputPointConnectionIndex != -1);
		ASSERT(arrOutputConnections[m_nOutputPointConnectionIndex] == this);

		if (m_nOutputPointConnectionIndex != nCount - 1)
		{
			arrOutputConnections[m_nOutputPointConnectionIndex] = arrOutputConnections[nCount - 1];

			arrOutputConnections[m_nOutputPointConnectionIndex]->m_nOutputPointConnectionIndex =
				m_nOutputPointConnectionIndex;
		}

		arrOutputConnections.SetSize(nCount - 1);

		m_nOutputPointConnectionIndex = -1;
		CMDTARGET_RELEASE(m_pOutputPoint);
	}

	m_pOutputPoint = pPoint;

	if (pPoint)
	{
		m_nOutputPointConnectionIndex = (int)pPoint->m_arrOutputConnections.Add(this);

		CMDTARGET_ADDREF(pPoint);
	}

	OnGraphChanged();
}

CXTPFlowGraphNode* CXTPFlowGraphConnection::GetInputNode() const
{
	if (m_pInputPoint)
		return m_pInputPoint->GetNode();
	return NULL;
}

CXTPFlowGraphNode* CXTPFlowGraphConnection::GetOutputNode() const
{
	if (m_pOutputPoint)
		return m_pOutputPoint->GetNode();
	return NULL;
}

void CXTPFlowGraphConnection::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	OnGraphChanged();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphConnection, CXTPFlowGraphElement)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Caption", DISPID_CAPTION, OleGetCaption,
						OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Tooltip", 2, OleGetTooltip, OleSetTooltip,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Tag", 8, OleGetTag, OleSetTag, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Color", 3, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Style", 4, OleGetStyle, OleSetStyle, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "OutputPoint", 5, OleGetOutputPoint,
						OleSetOutputPoint, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "InputPoint", 6, OleGetInputPoint,
						OleSetInputPoint, VT_DISPATCH)

	DISP_FUNCTION_ID(CXTPFlowGraphConnection, "Remove", 7, Remove, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Width", 9, OleGetWidth, OleSetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphConnection, "Dashed", 10, OleGetDashed, OleSetDashed, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphConnection, CXTPFlowGraphElement)
	INTERFACE_PART(CXTPFlowGraphConnection, XTPDIID_FlowGraphConnection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphConnection, XTPDIID_FlowGraphConnection)

void CXTPFlowGraphConnection::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(lpszCaption);
}

BSTR CXTPFlowGraphConnection::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

void CXTPFlowGraphConnection::OleSetTooltip(LPCTSTR lpszTooltip)
{
	SetTooltip(lpszTooltip);
}

BSTR CXTPFlowGraphConnection::OleGetTooltip()
{
	return m_strTooltip.AllocSysString();
}

void CXTPFlowGraphConnection::OleSetTag(LPCTSTR lpszTag)
{
	SetTag(lpszTag);
}

BSTR CXTPFlowGraphConnection::OleGetTag()
{
	return m_strTag.AllocSysString();
}

void CXTPFlowGraphConnection::OleSetColor(OLE_COLOR clr)
{
	SetColor(AxTranslateColor(clr));
}

OLE_COLOR CXTPFlowGraphConnection::OleGetColor()
{
	return m_clrConnection;
}

void CXTPFlowGraphConnection::OleSetStyle(int nStyle)
{
	SetStyle(nStyle);
}

int CXTPFlowGraphConnection::OleGetStyle()
{
	return GetStyle();
}

LPDISPATCH CXTPFlowGraphConnection::OleGetOutputPoint()
{
	return XTPGetDispatch(GetOutputPoint());
}

void CXTPFlowGraphConnection::OleSetOutputPoint(LPDISPATCH lpDisp)
{
	CXTPFlowGraphConnectionPoint* pConnectionPoint = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPFlowGraphConnectionPoint, CXTPFlowGraphConnectionPoint::FromIDispatchSafe(lpDisp));
	if (NULL == pConnectionPoint)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	SetOutputPoint(pConnectionPoint);
}

LPDISPATCH CXTPFlowGraphConnection::OleGetInputPoint()
{
	return XTPGetDispatch(GetInputPoint());
}

void CXTPFlowGraphConnection::OleSetInputPoint(LPDISPATCH lpDisp)
{
	CXTPFlowGraphConnectionPoint* pConnectionPoint = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPFlowGraphConnectionPoint, CXTPFlowGraphConnectionPoint::FromIDispatchSafe(lpDisp));
	if (NULL == pConnectionPoint)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	SetInputPoint(pConnectionPoint);
}

void CXTPFlowGraphConnection::OleSetWidth(int nWidth)
{
	SetWidth(nWidth);
}

int CXTPFlowGraphConnection::OleGetWidth()
{
	return GetWidth();
}

void CXTPFlowGraphConnection::OleSetDashed(BOOL bDashed)
{
	SetDashed(bDashed);
}

BOOL CXTPFlowGraphConnection::OleGetDashed()
{
	return GetDashed();
}

#endif
