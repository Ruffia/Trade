// XTPChartScatterLineSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"

#include "Chart/Styles/Line/XTPChartLineSeriesStyle.h"
#include "Chart/Styles/Line/XTPChartScatterLineSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartScatterLineSeriesStyle, CXTPChartLineSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesStyle

CXTPChartScatterLineSeriesStyle::CXTPChartScatterLineSeriesStyle()
{
}

CXTPChartScatterLineSeriesStyle::~CXTPChartScatterLineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartScatterLineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartLineSeriesView(pSeries, pDiagramView, FALSE);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartScatterLineSeriesStyle, CXTPChartLineSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartScatterLineSeriesStyle, CXTPChartLineSeriesStyle)
	INTERFACE_PART(CXTPChartScatterLineSeriesStyle, XTPDIID__DChartScatterLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartScatterLineSeriesStyle, XTPDIID__DChartScatterLineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartScatterLineSeriesStyle,
							  "Codejock.ChartScatterLineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartScatterLineSeriesStyle);

#endif
