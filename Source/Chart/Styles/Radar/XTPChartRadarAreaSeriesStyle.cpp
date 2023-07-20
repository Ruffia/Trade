// XTPChartRadarAreaSeriesStyle.cpp
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
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarAreaSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarAreaSeriesView.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartRadarAreaSeriesStyle, CXTPChartRadarPointSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAreaSeriesStyle

CXTPChartRadarAreaSeriesStyle::CXTPChartRadarAreaSeriesStyle()
{
	m_nTransparency = 135;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartRadarAreaSeriesStyle::~CXTPChartRadarAreaSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);

	SAFE_RELEASE(m_pFillStyle);
}

void CXTPChartRadarAreaSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRadarPointSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

CXTPChartSeriesView* CXTPChartRadarAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarAreaSeriesView(pSeries, pDiagramView);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarAreaSeriesStyle, CXTPChartRadarPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRadarAreaSeriesStyle, "FillStyle", 5, OleGetFillStyle,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartRadarAreaSeriesStyle, "Border", 6, OleGetBorder, SetNotSupported,
						VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarAreaSeriesStyle, CXTPChartRadarPointSeriesStyle)
	INTERFACE_PART(CXTPChartRadarAreaSeriesStyle, XTPDIID__DChartRadarAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarAreaSeriesStyle, XTPDIID__DChartRadarAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRadarAreaSeriesStyle,
							  "Codejock.ChartRadarAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRadarAreaSeriesStyle);

LPDISPATCH CXTPChartRadarAreaSeriesStyle::OleGetFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

LPDISPATCH CXTPChartRadarAreaSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

#endif
