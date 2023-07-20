// XTPChartRadarPointSeriesView.cpp
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
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartDiagramView.h"

#include "Common/Math/XTPMathUtils.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "Chart/Styles/Radar/XTPChartRadarPointSeriesStyle.h"
#include "Chart/Styles/Radar/XTPChartRadarPointSeriesView.h"
#include "Chart/Styles/Point/XTPChartMarker.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesView

CXTPChartRadarPointSeriesView::CXTPChartRadarPointSeriesView(CXTPChartSeries* pSeries,
															 CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarDiagramSeriesView(pSeries, pDiagramView)
{
}
CXTPChartSeriesPointView* CXTPChartRadarPointSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartRadarPointSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartRadarPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarPointSeriesStyle,
															 m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));
	int nSize = __min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(
		pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(),
												 m_pSeries->GetColor2(),
												 m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesPointView

CXTPChartRadarPointSeriesPointView::CXTPChartRadarPointSeriesPointView(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{
}

CXTPChartPointF CXTPChartRadarPointSeriesPointView::GetScreenPoint() const
{
	CXTPChartRadarPointSeriesView* pView = (CXTPChartRadarPointSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_dInternalValue);
}

CXTPChartDeviceCommand*
	CXTPChartRadarPointSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color	   = GetColor();
	CXTPChartColor color2	  = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();

	if (CXTPMathUtils::IsNan(m_dInternalValue))
		return NULL;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartPointF point = GetScreenPoint();

	pCommand->AddChildCommand(((CXTPChartRadarPointSeriesStyle*)GetSeriesView()->GetStyle())
								  ->GetMarker()
								  ->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarPointSeriesPointView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartRadarPointSeriesStyle* pStyle =
		STATIC_DOWNCAST(CXTPChartRadarPointSeriesStyle, GetSeriesView()->GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(static_cast<int>(rcBounds.CenterPoint().x),
							 static_cast<int>(rcBounds.CenterPoint().y));
	int nSize = __min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize,
																	   GetColor(), GetColor2(),
																	   GetColor().GetDarkColor()));

	return pCommand;
}
