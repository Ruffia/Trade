// XTPChartRadarSplineAreaSeriesStyle.cpp
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
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
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
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"
#include "Chart/Styles/Radar/XTPChartRadarAreaSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarAreaSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarSplineAreaSeriesStyle.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRadarSplineAreaSeriesStyle, CXTPChartRadarAreaSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRadarSplineAreaSeriesStyle::CXTPChartRadarSplineAreaSeriesStyle()
{
}

CXTPChartRadarSplineAreaSeriesStyle::~CXTPChartRadarSplineAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartRadarSplineAreaSeriesStyle::CreateView(
	CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarSplineAreaSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineAreaSeriesView

CXTPChartRadarSplineAreaSeriesView::CXTPChartRadarSplineAreaSeriesView(
	CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarAreaSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartRadarSplineAreaSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartRadarSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartRadarSplineAreaSeriesStyle* pStyle = (CXTPChartRadarSplineAreaSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();

	if (nCount > 1)
	{
		CXTPChartPoints arrPoints;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartRadarPointSeriesPointView* pPointView = (CXTPChartRadarPointSeriesPointView*)
																 m_pPointsView->GetAt(i);
			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			arrPoints.Add(pointNext);
		}

		CXTPChartPointF ptFirst = arrPoints[0];
		arrPoints.Add(ptFirst);

		pCommand->AddChildCommand(
			pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(),
															  GetActualColor2(), 2));

		if (pStyle->GetBorder()->IsVisible())
		{
			CXTPChartColor clrBorder = GetBorderActualColor();

			CXTPChartDeviceCommand* pSolidSplineDeviceCommand =
				CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(
					arrPoints, clrBorder, pStyle->GetBorder()->GetThickness());
			pCommand->AddChildCommand(pSolidSplineDeviceCommand);
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarSplineAreaSeriesStyle, CXTPChartRadarAreaSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarSplineAreaSeriesStyle, CXTPChartRadarAreaSeriesStyle)
	INTERFACE_PART(CXTPChartRadarSplineAreaSeriesStyle, XTPDIID__DChartRadarSplineAreaSeriesStyle,
				   Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarSplineAreaSeriesStyle,
						XTPDIID__DChartRadarSplineAreaSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRadarSplineAreaSeriesStyle,
							  "Codejock.ChartRadarSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRadarSplineAreaSeriesStyle);

#endif
