// XTPChartGanttSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Styles/Bar/XTPChartBarSeriesStyle.h"
#include "Chart/Styles/Bar/XTPChartBarSeriesView.h"

#include "Chart/Styles/Range/XTPChartRangeBarSeriesStyle.h"
#include "Chart/Styles/Range/XTPChartRangeBarSeriesView.h"
#include "Chart/Styles/Range/XTPChartGanttSeriesStyle.h"
#include "Chart/Styles/Range/XTPChartRangeBarSeriesLabel.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartGanttSeriesStyle

IMPLEMENT_SERIAL(CXTPChartGanttSeriesStyle, CXTPChartRangeBarSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartGanttSeriesStyle::CXTPChartGanttSeriesStyle()
{
	SetLabel(new CXTPChartRangeBarSeriesLabel());
}

CXTPChartGanttSeriesStyle::~CXTPChartGanttSeriesStyle()
{
}

void CXTPChartGanttSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRangeBarSeriesStyle::DoPropExchange(pPX);
}

CXTPChartSeriesView* CXTPChartGanttSeriesStyle::CreateView(CXTPChartSeries* pSeries,
														   CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartGanttSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGanttSeriesView

CXTPChartGanttSeriesView::CXTPChartGanttSeriesView(CXTPChartSeries* pSeries,
												   CXTPChartDiagramView* pDiagramView)
	: CXTPChartRangeBarSeriesView(pSeries, pDiagramView)
{
}

CXTPChartSeriesPointView* CXTPChartGanttSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartGanttSeriesPointView(pPoint, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGanttSeriesPointView

CXTPChartGanttSeriesPointView::CXTPChartGanttSeriesPointView(CXTPChartSeriesPoint* pPoint,
															 CXTPChartElementView* pParentView)
	: CXTPChartRangeBarSeriesPointView(pPoint, pParentView)
{
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartGanttSeriesStyle, CXTPChartRangeBarSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartGanttSeriesStyle, CXTPChartRangeBarSeriesStyle)
	INTERFACE_PART(CXTPChartGanttSeriesStyle, XTPDIID__DChartGanttSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartGanttSeriesStyle, XTPDIID__DChartGanttSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartGanttSeriesStyle,
							  "Codejock.ChartGanttSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartGanttSeriesStyle);

#endif
