// XTPChartRadarAreaSeriesView.cpp
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "Chart/Styles/Point/XTPChartPointSeriesLabel.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarAreaSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"
#include "Chart/Styles/Radar/XTPChartRadarAreaSeriesView.h"
#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAreaSeriesView

CXTPChartRadarAreaSeriesView::CXTPChartRadarAreaSeriesView(CXTPChartSeries* pSeries,
														   CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarDiagramSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartRadarAreaSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartColor CXTPChartRadarAreaSeriesView::GetActualColor() const
{
	CXTPChartColor clr = m_pSeries->GetColor();
	return CXTPChartColor((BYTE)((CXTPChartRadarAreaSeriesStyle*)GetStyle())->GetTransparency(),
						  clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartRadarAreaSeriesView::GetActualColor2() const
{
	CXTPChartColor clr = m_pSeries->GetColor2();
	return CXTPChartColor((BYTE)((CXTPChartRadarAreaSeriesStyle*)GetStyle())->GetTransparency(),
						  clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartRadarAreaSeriesView::GetBorderActualColor() const
{
	CXTPChartRadarAreaSeriesStyle* pStyle = (CXTPChartRadarAreaSeriesStyle*)GetStyle();

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = m_pSeries->GetColor().GetDarkColor();
	return clr;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand());

	CXTPChartRadarAreaSeriesStyle* pStyle = (CXTPChartRadarAreaSeriesStyle*)GetStyle();

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

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints,
																			  GetActualColor(),
																			  GetActualColor2()));

		if (pStyle->GetBorder()->IsVisible())
		{
			CXTPChartColor clrBorder = GetBorderActualColor();

			pCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidPolylineDeviceCommand(
					arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarAreaSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	CXTPChartRadarAreaSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarAreaSeriesStyle,
															m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.top + 4));

	CXTPChartPoints arrPoints;
	arrPoints.Add(
		CXTPChartPointF(static_cast<int>(rcBounds.left), static_cast<int>(rcBounds.bottom + 1)));
	arrPoints.Add(CXTPChartPointF(ptCenter.X, float(rcBounds.top + 4)));
	arrPoints.Add(
		CXTPChartPointF(static_cast<int>(rcBounds.right), static_cast<int>(rcBounds.bottom + 1)));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints,
																		  GetActualColor(),
																		  GetActualColor2()));

	CXTPChartColor clrBorder = GetBorderActualColor();
	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(arrPoints[0], arrPoints[1],
																	  clrBorder, 1));
	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(arrPoints[1], arrPoints[2],
																	  clrBorder, 1));

	int nSize = __min(rcBounds.Width(), rcBounds.Height());
	pCommand->AddChildCommand(
		pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(),
												 m_pSeries->GetColor2(),
												 m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}
