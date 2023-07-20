// XTPChart2dPieSeriesStyle.cpp
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
#include "Common/XTPXMLHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
#include "Common/PropExchange/XTPPropExchangeRegistry.h"
#include "Common/PropExchange/XTPPropExchangeIniFile.h"
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
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartAppearance.h"

#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesLabelBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesPointViewBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesViewBase.h"

#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesStyle.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieDiagram.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesLabel.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesStyle

IMPLEMENT_SERIAL(CXTPChartPieSeriesStyle, CXTPChartPieSeriesStyleBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieSeriesStyle::CXTPChartPieSeriesStyle()
{
	SetLabel(new CXTPChartPieSeriesLabel());
}

CXTPChartDiagram* CXTPChartPieSeriesStyle::CreateDiagram()
{
	return new CXTPChartPieDiagram();
}

CXTPChartSeriesView* CXTPChartPieSeriesStyle::CreateView(CXTPChartSeries* pSeries,
														 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartPieSeriesView(pSeries, pDiagramView);
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPChartPieSeriesStyle, CXTPChartPieSeriesStyleBase)
	INTERFACE_PART(CXTPChartPieSeriesStyle, XTPDIID__DChartPieSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieSeriesStyle, XTPDIID__DChartPieSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartPieSeriesStyle,
							  "Codejock.ChartPieSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartPieSeriesStyle);

#endif
