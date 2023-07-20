// XTPChartRadarPointSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesLabel.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartRadarPointSeriesStyle, CXTPChartRadarDiagramSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesStyle

CXTPChartRadarPointSeriesStyle::CXTPChartRadarPointSeriesStyle()
{
	m_pMarker = new CXTPChartMarker(this);

	SetLabel(new CXTPChartRadarPointSeriesLabel());
}

CXTPChartRadarPointSeriesStyle::~CXTPChartRadarPointSeriesStyle()
{
	SAFE_RELEASE(m_pMarker);
}

void CXTPChartRadarPointSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRadarDiagramSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection secMarker(pPX->GetSection(_T("Marker")));
	m_pMarker->DoPropExchange(&secMarker);
}

CXTPChartSeriesView* CXTPChartRadarPointSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarPointSeriesView(pSeries, pDiagramView);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarPointSeriesStyle, CXTPChartSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRadarPointSeriesStyle, "Marker", 4, OleGetMarker, SetNotSupported,
						VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarPointSeriesStyle, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartRadarPointSeriesStyle, XTPDIID__DChartRadarPointSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarPointSeriesStyle, XTPDIID__DChartRadarPointSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRadarPointSeriesStyle,
							  "Codejock.ChartRadarPointSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRadarPointSeriesStyle);

LPDISPATCH CXTPChartRadarPointSeriesStyle::OleGetMarker()
{
	return XTPGetDispatch(m_pMarker);
}

#endif
