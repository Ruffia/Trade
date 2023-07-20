// XTPFlowGraphPages.cpp : implementation of the CXTPFlowGraphPages class.
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

#include "stdafx.h"

#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"

#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphPages.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphUndoManager.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFlowGraphPages::CXTPFlowGraphPages(CXTPFlowGraphControl* pControl)
{
	m_pControl = pControl;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphPages::~CXTPFlowGraphPages()
{
	RemoveAll();
}

void CXTPFlowGraphPages::RemoveAll()
{
	if (m_arrPages.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrPages.GetSize(); i++)
	{
		m_arrPages[i]->OnRemoved();
		m_arrPages[i]->m_pControl = NULL;
		m_arrPages[i]->InternalRelease();
	}
	m_arrPages.RemoveAll();

	m_pControl->GetUndoManager()->Clear();
	m_pControl->OnGraphChanged();
}

CXTPFlowGraphPage* CXTPFlowGraphPages::AddPage(CXTPFlowGraphPage* pPage)
{
	m_arrPages.Add(pPage);
	pPage->m_pControl = m_pControl;

	return pPage;
}

CXTPFlowGraphPage* CXTPFlowGraphPages::FindPage(int nId) const
{
	for (int i = 0; i < m_arrPages.GetSize(); i++)
	{
		if (m_arrPages[i]->GetID() == nId)
			return m_arrPages[i];
	}
	return NULL;
}

void CXTPFlowGraphPages::Remove(CXTPFlowGraphPage* pPage)
{
	for (int i = 0; i < m_arrPages.GetSize(); i++)
	{
		if (m_arrPages[i] == pPage)
		{
			RemoveAt(i);
			return;
		}
	}
}

void CXTPFlowGraphPages::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrPages.GetSize())
		return;

	CXTPFlowGraphPage* pPage = m_arrPages[nIndex];

	m_arrPages.RemoveAt(nIndex);

	pPage->OnRemoved();
	pPage->m_pControl = NULL;
	pPage->InternalRelease();

	m_pControl->GetUndoManager()->Clear();
	m_pControl->OnGraphChanged();
}

void CXTPFlowGraphPages::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Page")));

	if (pPX->IsStoring())
	{
		int nCount   = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphPage* pPage = GetAt(i);
			ASSERT(pPage);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pPage, RUNTIME_CLASS(CXTPFlowGraphPage));

			BOOL bActive = m_pControl->GetActivePage() == pPage;
			PX_Bool(&sec, _T("Active"), bActive, FALSE);
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		CXTPFlowGraphPage* pActivePage = NULL;

		while (pos)
		{
			CXTPFlowGraphPage* pPage = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));

			if (!sec->ExchangeObjectInstance((CObject*&)pPage, RUNTIME_CLASS(CXTPFlowGraphPage)))
				AfxThrowArchiveException(CArchiveException::badClass);

			pPage->m_pControl = m_pControl;
			pPage->DoPropExchange(&sec);

			m_arrPages.Add(pPage);

			BOOL bActive = FALSE;
			PX_Bool(&sec, _T("Active"), bActive, FALSE);

			if (bActive)
				pActivePage = pPage;
		}

		m_pControl->SetActivePage(pActivePage);
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphPages, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "Page", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphPages, "AddPage", 2, OleAddPage, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphPages, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "FindPage", 5, OleFindPage, VT_DISPATCH, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphPages, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphPages, XTPDIID_FlowGraphPages, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphPages, XTPDIID_FlowGraphPages)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphPages)

LPDISPATCH CXTPFlowGraphPages::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphPages::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphPages::OleAddPage()
{
	return XTPGetDispatch(AddPage(new CXTPFlowGraphPage()));
}

LPDISPATCH CXTPFlowGraphPages::OleFindPage(int nId)
{
	return XTPGetDispatch(FindPage(nId));
}

#endif
