// XTPChart2dFunnelSeriesView.cpp
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

#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesPointViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesLabelBase.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesView.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesPointView.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesStyle.h"
#include "Chart/Styles/Funnel/2D/XTPChart2dFunnelSeriesLabel.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

EXTERN_C CXTPChartDeviceCommand* WINAPI XTP2dFunnelCreateLegendDeviceCommand(
	CXTPChartFunnelSeriesStyleBase* pStyle, CXTPChartDeviceContext* pDC, CRect rcBounds,
	CXTPChartColor color1, CXTPChartColor color2, CXTPChartColor clrBorder)
{
	ASSERT_VALID(pStyle);
	UNREFERENCED_PARAMETER(pDC);

	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	CXTPChartPoints arrPoints;

	arrPoints.Add(CXTPChartPointF(static_cast<float>((rcBounds.left + rcBounds.right) / 2),
								  static_cast<float>(rcBounds.top)));

	arrPoints.Add(
		CXTPChartPointF(static_cast<float>(rcBounds.right), static_cast<float>(rcBounds.bottom)));
	arrPoints.Add(
		CXTPChartPointF(static_cast<float>(rcBounds.left), static_cast<float>(rcBounds.bottom)));

	pCommand->AddChildCommand(
		pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints, clrBorder,
																			   1));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelSeriesView

IMPLEMENT_DYNAMIC(CXTPChartFunnelSeriesView, CXTPChartFunnelSeriesViewBase);

CXTPChartFunnelSeriesView::CXTPChartFunnelSeriesView(CXTPChartSeries* pSeries,
													 CXTPChartDiagramView* pDiagramView)
	: CXTPChartFunnelSeriesViewBase(pSeries, pDiagramView)
{
}

CXTPChartRectF CXTPChartFunnelSeriesView::GetInnerBounds() const
{
	return m_rcInnerBounds;
}

void CXTPChartFunnelSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	rcBounds.DeflateRect(5, 5, 5, 5);

	CXTPChartFunnelSeriesLabel* pLabel = STATIC_DOWNCAST(CXTPChartFunnelSeriesLabel,
														 pStyle->GetLabel());

	if (pLabel->IsVisible() && !pLabel->IsInside())
	{
		CSize sz(0, 0);
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartFunnelSeriesPointView* pPointView =
				STATIC_DOWNCAST(CXTPChartFunnelSeriesPointView, m_pPointsView->GetAt(i));

			CXTPChartString text(pLabel->GetPointLabel(pPointView->GetPoint()));
			CXTPChartTextPainter painter(pDC, text, pLabel);

			sz.cx = max(sz.cx, (INT)painter.GetSize().Width);
			sz.cy = max(sz.cy, (INT)painter.GetSize().Height);
		}

		int nLineLength = pStyle->GetLabel()->GetLineLength();
		if (pLabel->GetPosition() == xtpChartFunnelLabelRight)
			rcBounds.DeflateRect(0, 0, nLineLength + sz.cx, 0);
		if (pLabel->GetPosition() == xtpChartFunnelLabelLeft)
			rcBounds.DeflateRect(nLineLength + sz.cx, 0, 0, 0);
	}

	double dRatio = pStyle->GetHeightToWidthRatio();
	if (dRatio > CXTPMathUtils::m_dEPS)
	{
		int nHeight = rcBounds.Height();
		int nWidth  = int(nHeight / dRatio);

		if (nWidth > rcBounds.Width())
		{
			nWidth  = rcBounds.Width();
			nHeight = int(nWidth * dRatio);
		}

		CPoint ptTopLeft((rcBounds.left + rcBounds.right - nWidth) / 2,
						 (rcBounds.top + rcBounds.bottom - nHeight) / 2);
		rcBounds = CRect(ptTopLeft, CSize(nWidth, nHeight));
	}

	m_rcInnerBounds = rcBounds;

	CXTPChartFunnelSeriesViewBase::CalculatePointLayout(pDC, rcBounds);
}

CXTPChartSeriesPointView* CXTPChartFunnelSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartFunnelSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds, CXTPChartColor color1, CXTPChartColor color2,
	CXTPChartColor clrBorder)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	CXTPChartPoints arrPoints;

	int sz  = rcBounds.Height() / 3;
	int hsz = (rcBounds.Width() - sz) / 2 - 1;

	arrPoints.Add(
		CXTPChartPointF(static_cast<float>(rcBounds.left), static_cast<float>(rcBounds.top)));
	arrPoints.Add(
		CXTPChartPointF(static_cast<float>(rcBounds.right), static_cast<float>(rcBounds.top)));

	arrPoints.Add(CXTPChartPointF(static_cast<float>(rcBounds.right - hsz),
								  static_cast<float>(rcBounds.bottom - sz)));
	arrPoints.Add(CXTPChartPointF(static_cast<float>(rcBounds.right - hsz),
								  static_cast<float>(rcBounds.bottom)));

	arrPoints.Add(CXTPChartPointF(static_cast<float>(rcBounds.left + hsz),
								  static_cast<float>(rcBounds.bottom)));
	arrPoints.Add(CXTPChartPointF(static_cast<float>(rcBounds.left + hsz),
								  static_cast<float>(rcBounds.bottom - sz)));

	pCommand->AddChildCommand(
		pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints, clrBorder,
																			   1));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcBounds);

	CXTPChartColor color1 = m_pSeries->GetColor();
	CXTPChartColor color2 = m_pSeries->GetColor2();

	CXTPChartColor clrBorder;

	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		clrBorder = pStyle->GetBorder()->GetColor();
	else
		clrBorder = m_pSeries->GetColor().GetDarkColor();

	return CreateLegendDeviceCommand(pDC, rcBounds, color1, color2, clrBorder);
}
