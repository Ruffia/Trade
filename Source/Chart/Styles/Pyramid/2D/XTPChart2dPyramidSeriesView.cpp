// XTPChart2dPyramidSeriesView.cpp
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
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesPointViewBase.h"

#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidSeriesView.h"
#include "Chart/Styles/Pyramid/2D/XTPChart2dPyramidSeriesPointView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

EXTERN_C CXTPChartDeviceCommand* WINAPI XTP2dPyramidCreateLegendDeviceCommand(
	CXTPChartPyramidSeriesStyleBase* pStyle, CXTPChartDeviceContext* pDC, CRect rcBounds,
	CXTPChartColor color1, CXTPChartColor color2, CXTPChartColor clrBorder)
{
	ASSERT_VALID(pStyle);
	UNREFERENCED_PARAMETER(pDC);

	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();

	CXTPChartPoints arrPoints;

	arrPoints.Add(CXTPChartPointF(static_cast<int>(rcBounds.left + rcBounds.right) / 2,
								  static_cast<int>(rcBounds.top)));

	arrPoints.Add(
		CXTPChartPointF(static_cast<int>(rcBounds.right), static_cast<int>(rcBounds.bottom)));
	arrPoints.Add(
		CXTPChartPointF(static_cast<int>(rcBounds.left), static_cast<int>(rcBounds.bottom)));

	pCommand->AddChildCommand(
		pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(arrPoints, clrBorder,
																			   1));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesView

IMPLEMENT_DYNAMIC(CXTPChartPyramidSeriesView, CXTPChartPyramidSeriesViewBase);

CXTPChartPyramidSeriesView::CXTPChartPyramidSeriesView(CXTPChartSeries* pSeries,
													   CXTPChartDiagramView* pDiagramView)
	: CXTPChartPyramidSeriesViewBase(pSeries, pDiagramView)
{
}

CXTPChartRectF CXTPChartPyramidSeriesView::GetInnerBounds() const
{
	return m_rcInnerBounds;
}

void CXTPChartPyramidSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartPyramidSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyleBase,
															  GetStyle());

	rcBounds.DeflateRect(5, 5, 5, 5);

	CXTPChartPyramidSeriesLabelBase* pLabel = (CXTPChartPyramidSeriesLabelBase*)pStyle->GetLabel();

	if (pLabel->IsVisible() && !pLabel->IsInside())
	{
		CSize sz(0, 0);
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartPyramidSeriesPointViewBase* pPointView = (CXTPChartPyramidSeriesPointViewBase*)
																  m_pPointsView->GetAt(i);

			CXTPChartString text(pLabel->GetPointLabel(pPointView->GetPoint()));
			CXTPChartTextPainter painter(pDC, text, pLabel);

			sz.cx = max(sz.cx, (INT)painter.GetSize().Width);
			sz.cy = max(sz.cy, (INT)painter.GetSize().Height);
		}

		int nLineLength = pStyle->GetLabel()->GetLineLength();
		if (pLabel->GetPosition() == xtpChartPyramidLabelRight)
			rcBounds.DeflateRect(0, 0, nLineLength + sz.cx, 0);
		if (pLabel->GetPosition() == xtpChartPyramidLabelLeft)
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

	CalculateValues();

	int nCount = m_pPointsView->GetCount();

	double dFrom = 0;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartPyramidSeriesPointViewBase* pPointView = (CXTPChartPyramidSeriesPointViewBase*)
															  m_pPointsView->GetAt(i);

		pPointView->CalculateLayout(i, nCount, dFrom, dFrom + pPointView->m_dValue);
		dFrom += pPointView->m_dValue;
	}
}

CXTPChartSeriesPointView* CXTPChartPyramidSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPyramidSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds, CXTPChartColor color1, CXTPChartColor color2,
	CXTPChartColor clrBorder)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartPyramidSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyleBase,
															  GetStyle());
	return XTP2dPyramidCreateLegendDeviceCommand(pStyle, pDC, rcBounds, color1, color2, clrBorder);
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1 = m_pSeries->GetColor();
	CXTPChartColor color2 = m_pSeries->GetColor2();

	CXTPChartColor clrBorder;

	CXTPChartPyramidSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyleBase,
															  GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		clrBorder = pStyle->GetBorder()->GetColor();
	else
		clrBorder = m_pSeries->GetColor().GetDarkColor();

	return CreateLegendDeviceCommand(pDC, rcBounds, color1, color2, clrBorder);
}
