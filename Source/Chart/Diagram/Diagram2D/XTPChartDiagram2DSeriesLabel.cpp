// XTPChartDiagram2DSeriesLabel.cpp
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
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Utils/XTPChartTextPainter.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DView.h"
#include "Chart/Diagram/Diagram2D/XTPChartDiagram2DPane.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DSeriesLabel
IMPLEMENT_DYNAMIC(CXTPChartDiagram2DSeriesLabel, CXTPChartSeriesLabel)

CXTPChartDiagram2DSeriesLabel::CXTPChartDiagram2DSeriesLabel()
{
}

CXTPChartDiagram2DSeriesLabel::~CXTPChartDiagram2DSeriesLabel()
{
}

void CXTPChartDiagram2DSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DSeriesLabelView

CXTPChartDiagram2DSeriesLabelView::CXTPChartDiagram2DSeriesLabelView(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{
}

CRect CXTPChartDiagram2DSeriesLabelView::GetPaneRect() const
{
	CXTPChartSeriesView* pSeriseView = m_pPointView->GetSeriesView();

	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)pSeriseView->GetDiagramView();

	return pDiagramView->GetPaneView()->GetBounds();
}

CXTPChartPointF CXTPChartDiagram2DSeriesLabelView::GetAnchorPoint() const
{
	return CXTPChartPointF(0, 0);
}

double CXTPChartDiagram2DSeriesLabelView::GetAnchorAngle() const
{
	return 0;
}

void CXTPChartDiagram2DSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CXTPChartPointF ptAnchorPoint = GetAnchorPoint();

	CalculateLayout(pDC, ptAnchorPoint, GetAnchorAngle(), text);
}

void CXTPChartDiagram2DSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC,
														const CXTPChartPointF& ptAnchorPoint,
														double dAnchorAngle,
														const CXTPChartString& text)
{
	double dLineAngle = CXTPMathUtils::Degree2Radian(dAnchorAngle);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptAnchorPoint.X, (int)ptAnchorPoint.Y)))
		return;

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartPointF ptStartPoint(CXTPMathUtils::Round(ptAnchorPoint.X),
								 CXTPMathUtils::Round(ptAnchorPoint.Y));
	CXTPChartPointF ptFinishPoint(
		CXTPMathUtils::Round(ptAnchorPoint.X + (cos(dLineAngle) * m_pLabel->GetLineLength())),
		CXTPMathUtils::Round(ptAnchorPoint.Y - (sin(dLineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF rcInnerBounds;
	CXTPChartRectF rcBounds =
		CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
			ptFinishPoint, dLineAngle, szText, nBorderThickness, rcInnerBounds);

	CXTPChartSeriesView* pSeriseView	 = m_pPointView->GetSeriesView();
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)pSeriseView->GetDiagramView();
	pDiagramView->CheckLabelBounds(rcBounds);
}

void CXTPChartDiagram2DSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC,
														CXTPChartPointF ptCenterPoint)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CalculateLayout(pDC, ptCenterPoint, text);
}

void CXTPChartDiagram2DSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC,
														CXTPChartPointF ptCenterPoint,
														const CXTPChartString& text)
{
	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptCenterPoint.X, (int)ptCenterPoint.Y)))
		return;

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartRectF rcBounds(0, 0, szText.Width + static_cast<float>(2 * nBorderThickness),
							szText.Height + static_cast<float>(2 * nBorderThickness));
	rcBounds.Offset(ptCenterPoint.X - rcBounds.Width / 2, ptCenterPoint.Y - rcBounds.Height / 2);
	rcBounds.Round();

	CXTPChartSeriesView* pSeriseView	 = m_pPointView->GetSeriesView();
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)pSeriseView->GetDiagramView();
	pDiagramView->CheckLabelBounds(rcBounds);
}

CXTPChartDeviceCommand*
	CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CXTPChartPointF ptAnchorPoint = GetAnchorPoint();

	return CreateDeviceCommand(pDC, ptAnchorPoint, GetAnchorAngle(), text);
}

CXTPChartDeviceCommand* CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(
	CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle,
	const CXTPChartString& text)
{
	if (text.IsEmpty() || text == _T(" "))
		return NULL;

	double dLineAngle = CXTPMathUtils::Degree2Radian(dAnchorAngle);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptAnchorPoint.X, (int)ptAnchorPoint.Y)))
		return NULL;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pLabel);

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartColor clrTextColor = GetActualTextColor();

	CXTPChartPointF ptStartPoint(CXTPMathUtils::Round(ptAnchorPoint.X),
								 CXTPMathUtils::Round(ptAnchorPoint.Y));
	CXTPChartPointF ptFinishPoint(
		CXTPMathUtils::Round(ptAnchorPoint.X + (cos(dLineAngle) * m_pLabel->GetLineLength())),
		CXTPMathUtils::Round(ptAnchorPoint.Y - (sin(dLineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF rcInnerBounds;
	CXTPChartRectF rcBounds =
		CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
			ptFinishPoint, dLineAngle, szText, nBorderThickness, rcInnerBounds);

	CXTPChartPointF ptLabelPoint = rcInnerBounds.GetLocation();

	CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
	pCommand->AddChildCommand(
		m_pLabel->GetFillStyle()->CreateDeviceCommand(rcBounds, clrBackColor, clrBackColor));

	painter.SetLocation(ptLabelPoint);

	pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

	if (m_pLabel->GetShowLines())
	{
		CXTPChartColor clrConnectorColor = GetActualConnectorColor();

		CXTPChartSeriesLabelLineConnectorPainter linePainter(ptStartPoint, ptFinishPoint, rcBounds);
		pCommand->AddChildCommand(linePainter.CreateDeviceCommand(pDC, clrConnectorColor));
	}

	if (nBorderThickness)
	{
		CXTPChartColor clrBorderColor = GetActualBorderColor();

		pCommand->AddChildCommand(
			m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(rcBounds, clrBorderColor));
	}

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(
	CXTPChartDeviceContext* pDC, CXTPChartPointF ptCenterPoint)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	return CreateDeviceCommand(pDC, ptCenterPoint, text);
}

CXTPChartDeviceCommand* CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(
	CXTPChartDeviceContext* pDC, CXTPChartPointF ptCenterPoint, const CXTPChartString& text)
{
	if (text.IsEmpty() || text == _T(" "))
		return NULL;

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptCenterPoint.X, (int)ptCenterPoint.Y)))
		return NULL;

	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pLabel);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartColor clrTextColor = GetActualTextColor();

	CXTPChartRectF rcBounds(0, 0, szText.Width + static_cast<float>(2 * nBorderThickness),
							szText.Height + static_cast<float>(2 * nBorderThickness));
	rcBounds.Offset(ptCenterPoint.X - rcBounds.Width / 2, ptCenterPoint.Y - rcBounds.Height / 2);
	rcBounds.Round();

	CXTPChartRectF rcInnerBounds = rcBounds;
	rcInnerBounds.Inflate((float)-nBorderThickness, (float)-nBorderThickness);

	CXTPChartPointF ptLabelPoint = rcInnerBounds.GetLocation();

	CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
	pCommand->AddChildCommand(
		m_pLabel->GetFillStyle()->CreateDeviceCommand(rcBounds, clrBackColor, clrBackColor));

	painter.SetLocation(ptLabelPoint);

	pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

	if (nBorderThickness)
	{
		CXTPChartColor clrBorderColor = GetActualBorderColor();

		pCommand->AddChildCommand(
			m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(rcBounds, clrBorderColor));
	}

	return pCommand;
}
