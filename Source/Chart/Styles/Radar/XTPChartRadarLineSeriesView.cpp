// XTPChartRadarLineSeriesView.cpp
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
#include "Common/Math/XTPMathUtils.h"
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
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartLineStyle.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarLineSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"
#include "Chart/Styles/Radar/XTPChartRadarLineSeriesView.h"
#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarLineSeriesView

CXTPChartRadarLineSeriesView::CXTPChartRadarLineSeriesView(CXTPChartSeries* pSeries,
														   CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarDiagramSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartRadarLineSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand*
	CXTPChartRadarLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartRadarLineSeriesStyle* pStyle = (CXTPChartRadarLineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointF pointPrev;
		BOOL bValidPrev = FALSE;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartRadarPointSeriesPointView* pPointView = (CXTPChartRadarPointSeriesPointView*)
																 m_pPointsView->GetAt(i);

			if (CXTPMathUtils::IsNan(pPointView->m_dInternalValue))
			{
				bValidPrev = FALSE;
				continue;
			}

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (bValidPrev)
			{
				pCommand->AddChildCommand(
					pStyle->GetLineStyle()->CreateDeviceCommand(pointPrev, pointNext,
																m_pSeries->GetColor()));
			}

			pointPrev  = pointNext;
			bValidPrev = TRUE;
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarLineSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartRadarPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarPointSeriesStyle,
															 m_pSeries->GetStyle());
	CXTPChartRadarLineSeriesStyle* pRadarLineSeriesStyle =
		STATIC_DOWNCAST(CXTPChartRadarLineSeriesStyle, m_pSeries->GetStyle());

	if (pRadarLineSeriesStyle != 0)
	{
		CXTPChartLineStyle* pLineStyle = pRadarLineSeriesStyle->GetLineStyle();
		m_pSeries->GetLegendItem()->SetLineThickness(pLineStyle->GetThickness());
		m_pSeries->GetLegendItem()->SetDashStyle(pLineStyle->GetDashStyle());
	}

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));

	// Set different style on legend lines.
	// Set custom dash style on legend is not supported, because the length of legend is small.
	if (m_pSeries->GetLegendItem()->GetDashStyle() == xtpChartDashStyleSolid
		|| m_pSeries->GetLegendItem()->GetDashStyle() == xtpChartDashStyleEmpty
		|| m_pSeries->GetLegendItem()->GetDashStyle() == xtpChartDashStyleCustom)
	{
		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPPoint3d(rcBounds.left, ptCenter.Y), CXTPPoint3d(rcBounds.right, ptCenter.Y),
			m_pSeries->GetColor(), m_pSeries->GetLegendItem()->GetActualLineThickness()));
	}
	else
	{
		CXTPChartLineDashArray dashArray;
		pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dDashedLineDeviceCommand(
			CXTPPoint3d(rcBounds.left, ptCenter.Y), CXTPPoint3d(rcBounds.right, ptCenter.Y),
			m_pSeries->GetColor(), m_pSeries->GetLegendItem()->GetActualLineThickness(),
			m_pSeries->GetLegendItem()->GetDashStyle(), dashArray));
	}

	int nSize = __min(rcBounds.Width(), rcBounds.Height());
	pCommand->AddChildCommand(
		pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(),
												 m_pSeries->GetColor2(),
												 m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}
