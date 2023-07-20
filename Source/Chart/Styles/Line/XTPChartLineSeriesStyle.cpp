// XTPChartLineSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartErrorBar.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "Chart/Appearance/XTPChartLineStyle.h"

#include "Chart/Styles/Point/XTPChartPointSeriesStyle.h"

#include "Chart/Styles/Line/XTPChartLineSeriesStyle.h"
#include "Chart/Styles/Line/XTPChartLineSeriesErrorBar.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesStyle

CXTPChartLineSeriesStyle::CXTPChartLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);

	SetErrorBar(new CXTPChartLineSeriesErrorBar());
}

CXTPChartLineSeriesStyle::~CXTPChartLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);
}

CXTPChartSeriesView* CXTPChartLineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
														  CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartLineSeriesStyle, "LineStyle", 100, OleGetLineStyle,
						SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle)
	INTERFACE_PART(CXTPChartLineSeriesStyle, XTPDIID__DChartLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLineSeriesStyle, XTPDIID__DChartLineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartLineSeriesStyle,
							  "Codejock.ChartLineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartLineSeriesStyle);

LPDISPATCH CXTPChartLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}

#endif
