// XTPChart2dPieDiagram.cpp
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
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
#include "Common/Math/XTPMathUtils.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/Types/XTPChartPie.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesPointView.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesLabelBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesPointViewBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesViewBase.h"

#include "Chart/Styles/Pie/2D/XTPChart2dPieDiagram.h"
#include "Chart/Styles/Pie/2D/XTPChart2dPieDiagramView.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDiagram

IMPLEMENT_SERIAL(CXTPChartPieDiagram, CXTPChart2dDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieDiagram::CXTPChartPieDiagram()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPChartDiagramView* CXTPChartPieDiagram::CreateView(CXTPChartDeviceContext* pDC,
													  CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieDiagramView(this, pParent);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPieDiagram, CXTPChart2dDiagram)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPieDiagram, CXTPChart2dDiagram)
	INTERFACE_PART(CXTPChartPieDiagram, XTPDIID__DChartPieDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieDiagram, XTPDIID__DChartPieDiagram)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartPieDiagram, "Codejock.ChartPieDiagram." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartPieDiagram);

#endif
