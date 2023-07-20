// XTPChartPanel.cpp
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

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartTitle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPanel

IMPLEMENT_DYNAMIC(CXTPChartPanel, CXTPChartElement);

CXTPChartPanel::CXTPChartPanel()
{
	m_pTitles = new CXTPChartTitleCollection(this);
}

CXTPChartPanel::~CXTPChartPanel()
{
	SAFE_RELEASE(m_pTitles);
}

void CXTPChartPanel::Remove()
{
	if (m_pOwner)
	{
		CXTPChartElementCollection* pCollection = STATIC_DOWNCAST(CXTPChartElementCollection,
																  m_pOwner);
		pCollection->Remove(this);
	}
}

void CXTPChartPanel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection secTitles(pPX->GetSection(_T("Titles")));
	m_pTitles->DoPropExchange(&secTitles);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTitleCollection

CXTPChartPanelCollection::CXTPChartPanelCollection(CXTPChartContent* pOwner)
{
	m_pOwner = (CXTPChartElement*)pOwner;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartPanelCollection::~CXTPChartPanelCollection()
{
}

CXTPChartPanel* CXTPChartPanelCollection::Add(CXTPChartPanel* pPanel)
{
	InsertAt(GetCount(), pPanel);
	return pPanel;
}

void CXTPChartPanelCollection::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Panel")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPChartPanel* pPanel = NULL;

			CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
			PX_Object(&sec, pPanel, RUNTIME_CLASS(CXTPChartPanel));

			if (!pPanel)
				AfxThrowArchiveException(CArchiveException::badClass);

			Add(pPanel);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Panel")));
		POSITION pos = pEnumerator->GetPosition(XTPToDWORDChecked(m_arrElements.GetSize()));

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
			PX_Object(&sec, m_arrElements[i], RUNTIME_CLASS(CXTPChartPanel));
		}
	}
}

#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
//
BEGIN_DISPATCH_MAP(CXTPChartPanelCollection, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartPanelCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartPanelCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPChartPanelCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartPanelCollection, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartPanelCollection, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPChartPanelCollection, "DeleteAt", 6, RemoveAt, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPanelCollection, CXTPChartElement)
	INTERFACE_PART(CXTPChartPanelCollection, XTPDIID_ChartDiagramCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPanelCollection, XTPDIID_ChartDiagramCollection)

IMPLEMENT_ENUM_VARIANT(CXTPChartPanelCollection)

long CXTPChartPanelCollection::OleGetItemCount()
{
	return (long)GetCount();
}

#	pragma warning(push)
#	pragma warning(disable : 4702)

LPDISPATCH CXTPChartPanelCollection::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartPanel* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#	pragma warning(pop)

void CXTPChartPanelCollection::OleAdd(LPDISPATCH lpDisp)
{
	CXTPChartPanel* pPanel =
		XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPChartPanel, CXTPChartPanel::FromIDispatchSafe(lpDisp));
	if (NULL == pPanel)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	pPanel->InternalAddRef();

	Add(pPanel);
}

#endif
