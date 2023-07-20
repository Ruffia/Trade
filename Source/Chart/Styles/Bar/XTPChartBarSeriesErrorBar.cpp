// XTPChartBarSeriesErrorBar.cpp
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
#include "Chart/Styles/Bar/XTPChartBarSeriesErrorBar.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesErrorBar
IMPLEMENT_SERIAL(CXTPChartBarSeriesErrorBar, CXTPChartErrorBar,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartBarSeriesErrorBar::CXTPChartBarSeriesErrorBar()
{
}

CXTPChartBarSeriesErrorBar::~CXTPChartBarSeriesErrorBar()
{
}

void CXTPChartBarSeriesErrorBar::DoPropExchange(CXTPPropExchange* pPX)
{
	UNREFERENCED_PARAMETER(pPX);
}

CXTPChartElementView* CXTPChartBarSeriesErrorBar::CreateView(CXTPChartDeviceContext* pDC,
															 CXTPChartSeriesPointView* pPointView,
															 CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBarSeriesErrorBarView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesErrorBarView

CXTPChartBarSeriesErrorBarView::CXTPChartBarSeriesErrorBarView(CXTPChartErrorBar* pErrorBar,
															   CXTPChartSeriesPointView* pPointView,
															   CXTPChartElementView* pParentView)
	: CXTPChartErrorBarView(pErrorBar, pPointView, pParentView)
{
}

void CXTPChartBarSeriesErrorBarView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartBarSeriesErrorBar, CXTPChartErrorBar)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartBarSeriesErrorBar, CXTPChartErrorBar)
	INTERFACE_PART(CXTPChartBarSeriesErrorBar, XTPDIID_ChartSeriesErrorBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBarSeriesErrorBar, XTPDIID_ChartSeriesErrorBar)

#endif
