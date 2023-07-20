// XTPChartRadarSplineSeriesStyle.cpp
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
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartLineStyle.h"
#include "Common/Math/XTPMathUtils.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"
#include "Chart/Styles/Radar/XTPChartRadarLineSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarLineSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarSplineSeriesStyle.h"

#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRadarSplineSeriesStyle, CXTPChartRadarLineSeriesStyle,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartRadarSplineSeriesStyle::CXTPChartRadarSplineSeriesStyle()
{
}

CXTPChartRadarSplineSeriesStyle::~CXTPChartRadarSplineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartRadarSplineSeriesStyle::CreateView(CXTPChartSeries* pSeries,
																 CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarSplineSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineSeriesView

CXTPChartRadarSplineSeriesView::CXTPChartRadarSplineSeriesView(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarLineSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartRadarSplineSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartRadarSplineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartRadarSplineSeriesStyle* pStyle = (CXTPChartRadarSplineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();

	CXTPChartPoints points;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartRadarPointSeriesPointView* pPointView = (CXTPChartRadarPointSeriesPointView*)
															 m_pPointsView->GetAt(i);
		if (CXTPMathUtils::IsNan(pPointView->m_dInternalValue))
		{
			if (points.GetSize() > 1)
			{
				pCommand->AddChildCommand(
					pStyle->GetLineStyle()->CreateSplineDeviceCommand(points,
																	  m_pSeries->GetColor()));
			}
			points.RemoveAll();
			continue;
		}

		CXTPChartPointF pointNext = pPointView->GetScreenPoint();
		points.Add(pointNext);
	}

	if (points.GetSize() > 1)
	{
		pCommand->AddChildCommand(
			pStyle->GetLineStyle()->CreateSplineDeviceCommand(points, m_pSeries->GetColor()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarSplineSeriesStyle, CXTPChartRadarLineSeriesStyle)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartRadarSplineSeriesStyle, CXTPChartRadarLineSeriesStyle)
	INTERFACE_PART(CXTPChartRadarSplineSeriesStyle, XTPDIID__DChartRadarSplineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarSplineSeriesStyle, XTPDIID__DChartRadarSplineSeriesStyle)

IMPLEMENT_OLECREATE_EX2_CLSID(CXTPChartRadarSplineSeriesStyle,
							  "Codejock.ChartRadarSplineSeriesStyle." _XTP_AXLIB_VERSION,
							  XTPCLSID_ChartRadarSplineSeriesStyle);

#endif
