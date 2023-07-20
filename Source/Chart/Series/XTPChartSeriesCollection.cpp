// XTPChartSeriesCollection.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
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
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartTrendline.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesCollection

CXTPChartSeriesCollection::CXTPChartSeriesCollection(CXTPChartContent* pOwner)
{
	m_pOwner = (CXTPChartElement*)pOwner;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeriesCollection::~CXTPChartSeriesCollection()
{
}

CXTPChartSeries* CXTPChartSeriesCollection::Add(CXTPChartSeries* pSeries)
{
	InsertAt(GetCount(), pSeries);

	return pSeries;
}

void CXTPChartSeriesCollection::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Series")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			CXTPChartSeries* pSeries = new CXTPChartSeries();
			Add(pSeries);

			pSeries->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Series")));
		POSITION pos = pEnumerator->GetPosition(XTPToDWORDChecked(m_arrElements.GetSize()));

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSeriesCollection, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "Add", 2, OleAdd, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "AddTrendline", 3, OleAddTrendline, VT_DISPATCH,
					 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "DeleteAt", 6, RemoveAt, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartSeriesCollection, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeriesCollection, XTPDIID_ChartSeriesCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesCollection, XTPDIID_ChartSeriesCollection)

IMPLEMENT_ENUM_VARIANT(CXTPChartSeriesCollection)

long CXTPChartSeriesCollection::OleGetItemCount()
{
	return (long)GetCount();
}

#	pragma warning(push)
#	pragma warning(disable : 4702)

LPDISPATCH CXTPChartSeriesCollection::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartSeries* pSeries = GetAt(nIndex);
		// return XTPGetDispatch(pSeries);
		return pSeries->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#	pragma warning(pop)

LPDISPATCH CXTPChartSeriesCollection::OleAdd(LPCTSTR lpszName)
{
	CXTPChartSeries* pSeries = new CXTPChartSeries();
	Add(pSeries);

	pSeries->SetName(lpszName);

	return XTPGetDispatch(pSeries);
}

LPDISPATCH CXTPChartSeriesCollection::OleAddTrendline(LPCTSTR lpszName)
{
	CXTPChartSeriesTrendline* pSeries = new CXTPChartSeriesTrendline();
	Add(pSeries);

	pSeries->SetName(lpszName);

	return XTPGetDispatch(pSeries);
}

#endif
