// XTPChartRadarLineSeriesStyle.cpp
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
#include "Common/XTPFramework.h"
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
#include "Chart/XTPChartDiagramView.h"
#include "Chart/Appearance/XTPChartLineStyle.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarLineSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarLineSeriesView.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartRadarLineSeriesStyle, CXTPChartRadarPointSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarLineSeriesStyle

CXTPChartRadarLineSeriesStyle::CXTPChartRadarLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);
}

CXTPChartRadarLineSeriesStyle::~CXTPChartRadarLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);
}

void CXTPChartRadarLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRadarPointSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}

CXTPChartSeriesView* CXTPChartRadarLineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarLineSeriesView(pSeries, pDiagramView);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarLineSeriesStyle, CXTPChartRadarPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRadarLineSeriesStyle, "LineStyle", 100, OleGetLineStyle,
						SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarLineSeriesStyle, CXTPChartRadarPointSeriesStyle)
	INTERFACE_PART(CXTPChartRadarLineSeriesStyle, XTPDIID__DChartRadarLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarLineSeriesStyle, XTPDIID__DChartRadarLineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRadarLineSeriesStyle,
							  "Codejock.ChartRadarLineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRadarLineSeriesStyle);

LPDISPATCH CXTPChartRadarLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}

#endif
