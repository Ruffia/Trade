// XTPChart2dFunnelSeriesStyle.cpp
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
#include "Common/XTPMathUtils.h"
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Common/Math/XTPMathUtils.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesPointViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesPointView.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesView.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelDiagramSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChartFunnelSeriesPointView, CXTPChartFunnelSeriesPointViewBase);

CXTPChartFunnelSeriesPointView::CXTPChartFunnelSeriesPointView(CXTPChartSeriesPoint* pPoint,
															   CXTPChartElementView* pParentView)
	: CXTPChartFunnelSeriesPointViewBase(pPoint, pParentView)
{
}

CXTPChartFunnelSeriesPointView::CXTPChartFunnelSeriesPointView(CXTPChartSeriesPoint* pPoint,
															   CXTPChartElementView* pParentView,
															   BOOL bAddToParent)
	: CXTPChartFunnelSeriesPointViewBase(pPoint, pParentView, bAddToParent)
{
}

CXTPChartDeviceCommand*
	CXTPChartFunnelSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartColor color1	= GetColor();
	CXTPChartColor color2	= GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pPoint);

	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle,
														 GetSeriesView()->GetStyle());

	CXTPChartPoints arrPoints;

	arrPoints.Add(CXTPChartPointF(m_rc.GetLeft()
									  + static_cast<float>((m_rc.Width - m_rc.Width * m_dFrom) / 2),
								  m_rc.GetTop()));
	arrPoints.Add(CXTPChartPointF(m_rc.GetLeft()
									  + static_cast<float>((m_rc.Width + m_rc.Width * m_dFrom) / 2),
								  m_rc.GetTop()));

	arrPoints.Add(
		CXTPChartPointF(m_rc.GetLeft() + static_cast<float>((m_rc.Width + m_rc.Width * m_dTo) / 2),
						m_rc.GetBottom()));
	arrPoints.Add(
		CXTPChartPointF(m_rc.GetLeft() + static_cast<float>((m_rc.Width - m_rc.Width * m_dTo) / 2),
						m_rc.GetBottom()));

	pCommand->AddChildCommand(
		pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(
				arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));

	return pCommand;
}
