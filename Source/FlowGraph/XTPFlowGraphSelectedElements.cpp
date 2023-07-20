// XTPFlowGraphSelectedElements.cpp : implementation of the CXTPFlowGraphSelectedElements class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPSystemHelpers.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphSelectedElements.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphMessages.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFlowGraphSelectedElements::CXTPFlowGraphSelectedElements(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphSelectedElements::~CXTPFlowGraphSelectedElements()
{
	Clear();
}

void CXTPFlowGraphSelectedElements::Clear()
{
	if (m_arrSelectedElements.GetSize() == 0)
		return;

	BOOL bRecalcLayout =
		TRUE; // always recalc layout
			  //(m_pPage->GetControl()->GetPaintManager()->m_bRecalcConnectionLayoutWhenNeeded &&
			  //! m_pPage->GetControl()->GetPaintManager()->m_bAlwaysDrawConnectionPoints);

	for (int i = 0; i < m_arrSelectedElements.GetSize(); i++)
	{
		m_arrSelectedElements[i]->m_bSelected = FALSE;
		m_arrSelectedElements[i]->InternalRelease();

		if (bRecalcLayout)
			m_arrSelectedElements[i]->m_bDirty = TRUE;
	}
	m_arrSelectedElements.RemoveAll();

	if (bRecalcLayout)
		m_pPage->GetControl()->Reposition();

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

void CXTPFlowGraphSelectedElements::SetSelection(CXTPFlowGraphElement* pElement)
{
	if (m_arrSelectedElements.GetSize() == 1 && m_arrSelectedElements[0] == pElement)
		return;

	Clear();

	m_arrSelectedElements.Add(pElement);
	pElement->InternalAddRef();
	pElement->m_bSelected = TRUE;

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

void CXTPFlowGraphSelectedElements::Remove(CXTPFlowGraphElement* pElement)
{
	if (!pElement)
		return;

	if (!pElement->IsSelected())
		return;

	for (int i = 0; i < m_arrSelectedElements.GetSize(); i++)
	{
		if (m_arrSelectedElements[i] == pElement)
		{
			RemoveAt(i);
			return;
		}
	}
}

void CXTPFlowGraphSelectedElements::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrSelectedElements.GetSize())
		return;

	CXTPFlowGraphElement* pElement = m_arrSelectedElements[nIndex];

	m_arrSelectedElements.RemoveAt(nIndex);

	pElement->m_bSelected = FALSE;
	pElement->InternalRelease();

	BOOL bRecalcLayout = TRUE; // always recalc layout

	if (bRecalcLayout)
	{
		pElement->m_bDirty = TRUE;

		m_pPage->GetControl()->Reposition();
	}

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

void CXTPFlowGraphSelectedElements::AddSelection(CXTPFlowGraphElement* pElement)
{
	if (!pElement)
		return;

	if (pElement->IsSelected())
		return;

	BOOL bRecalcLayout =
		TRUE; // always recalc layout
			  //(m_pPage->GetControl()->GetPaintManager()->m_bRecalcConnectionLayoutWhenNeeded &&
			  //! m_pPage->GetControl()->GetPaintManager()->m_bAlwaysDrawConnectionPoints);

	m_arrSelectedElements.Add(pElement);
	pElement->InternalAddRef();
	pElement->m_bSelected = TRUE;

	if (bRecalcLayout)
	{
		pElement->m_bDirty = TRUE;

		m_pPage->GetControl()->Reposition();
	}

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphSelectedElements, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "Page", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "_NewEnum", DISPID_NEWENUM, OleNewEnum,
					 VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "AddSelection", 2, OleAddSelection, VT_EMPTY,
					 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "RemoveAll", 3, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "Remove", 5, OleRemove, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphSelectedElements, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphSelectedElements, XTPDIID_FlowGraphSelectedElements, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphSelectedElements, XTPDIID_FlowGraphSelectedElements)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphSelectedElements)

LPDISPATCH CXTPFlowGraphSelectedElements::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphSelectedElements::OleGetItemCount()
{
	return GetCount();
}

void CXTPFlowGraphSelectedElements::OleAddSelection(LPDISPATCH lpDisp)
{
	CXTPFlowGraphElement* pElement = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPFlowGraphElement, CXTPFlowGraphElement::FromIDispatchSafe(lpDisp));
	if (NULL == pElement)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	AddSelection(pElement);
}

void CXTPFlowGraphSelectedElements::OleRemove(LPDISPATCH lpDisp)
{
	CXTPFlowGraphElement* pElement = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPFlowGraphElement, CXTPFlowGraphElement::FromIDispatchSafe(lpDisp));
	if (NULL == pElement)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	Remove(pElement);
}

#endif
