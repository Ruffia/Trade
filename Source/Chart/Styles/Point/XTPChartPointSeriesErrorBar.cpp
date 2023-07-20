// XTPChartPointSeriesErrorBar.cpp
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
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartErrorBar.h"
#include "Chart/Styles/Point/XTPChartPointSeriesErrorBar.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesErrorBar
IMPLEMENT_SERIAL(CXTPChartPointSeriesErrorBar, CXTPChartErrorBar,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPointSeriesErrorBar::CXTPChartPointSeriesErrorBar()
{
}

CXTPChartPointSeriesErrorBar::~CXTPChartPointSeriesErrorBar()
{
}

void CXTPChartPointSeriesErrorBar::DoPropExchange(CXTPPropExchange* pPX)
{
	UNREFERENCED_PARAMETER(pPX);
}

CXTPChartElementView* CXTPChartPointSeriesErrorBar::CreateView(CXTPChartDeviceContext* pDC,
															   CXTPChartSeriesPointView* pPointView,
															   CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesErrorBarView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesErrorBarView

CXTPChartPointSeriesErrorBarView::CXTPChartPointSeriesErrorBarView(
	CXTPChartErrorBar* pErrorBar, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartErrorBarView(pErrorBar, pPointView, pParentView)
{
}

void CXTPChartPointSeriesErrorBarView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPointSeriesErrorBar, CXTPChartErrorBar)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPointSeriesErrorBar, CXTPChartErrorBar)
	INTERFACE_PART(CXTPChartPointSeriesErrorBar, XTPDIID_ChartSeriesErrorBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPointSeriesErrorBar, XTPDIID_ChartSeriesErrorBar)

#endif
