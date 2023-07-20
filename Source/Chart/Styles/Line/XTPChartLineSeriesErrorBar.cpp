// XTPChartLineSeriesErrorBar.cpp
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
#include "Common/XTPFramework.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"

#include "Chart/XTPChartErrorBar.h"
#include "Chart/Styles/Line/XTPChartLineSeriesErrorBar.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesErrorBar
IMPLEMENT_SERIAL(CXTPChartLineSeriesErrorBar, CXTPChartErrorBar,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartLineSeriesErrorBar::CXTPChartLineSeriesErrorBar()
{
}

CXTPChartLineSeriesErrorBar::~CXTPChartLineSeriesErrorBar()
{
}

void CXTPChartLineSeriesErrorBar::DoPropExchange(CXTPPropExchange* pPX)
{
	UNREFERENCED_PARAMETER(pPX);
}

CXTPChartElementView* CXTPChartLineSeriesErrorBar::CreateView(CXTPChartDeviceContext* pDC,
															  CXTPChartSeriesPointView* pPointView,
															  CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartLineSeriesErrorBarView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesErrorBarView

CXTPChartLineSeriesErrorBarView::CXTPChartLineSeriesErrorBarView(
	CXTPChartErrorBar* pErrorBar, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartErrorBarView(pErrorBar, pPointView, pParentView)
{
}

void CXTPChartLineSeriesErrorBarView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartLineSeriesErrorBar, CXTPChartErrorBar)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartLineSeriesErrorBar, CXTPChartErrorBar)
	INTERFACE_PART(CXTPChartLineSeriesErrorBar, XTPDIID_ChartSeriesErrorBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLineSeriesErrorBar, XTPDIID_ChartSeriesErrorBar)

#endif
