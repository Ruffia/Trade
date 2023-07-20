// XTPChart2dPieSeriesLabel.cpp
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

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

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
#include "Chart/Types/2D/XTPChart2dPie.h"

#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartSeriesPointView.h"

#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Appearance/XTPChartBorder.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesLabelBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesPointViewBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesViewBase.h"

#include "Chart/Styles/Pie/2D/XTPChart2dPieSeriesLabel.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesLabel

IMPLEMENT_SERIAL(CXTPChartPieSeriesLabel, CXTPChartPieSeriesLabelBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieSeriesLabel::CXTPChartPieSeriesLabel()
{
}

CXTPChartPieSeriesLabelsView::CXTPChartPieSeriesLabelsView(CXTPChartElementView* pParentView)
	: CXTPChartPieSeriesLabelsViewBase(pParentView)
{
}

CXTPChartElementView* CXTPChartPieSeriesLabel::CreateView(CXTPChartDeviceContext* pDC,
														  CXTPChartSeriesPointView* pPointView,
														  CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieSeriesLabelView(this, pPointView, pParentView);
}

CXTPChartPointF CXTPChartPieSeriesLabel::CalculateAnchorPointAndAngles(
	CXTPChartPieSeriesPointViewBase* pPointView, int borderThickness, double& lineAngle)
{
	UNREFERENCED_PARAMETER(borderThickness);
	lineAngle = pPointView->GetPie()->GetHalfAngle();

	int nHolePercent = pPointView->GetPie()->GetHolePercent();

	if (nHolePercent > 0 && IsInside())
	{
		double fraction  = nHolePercent / 100.0;
		double width	 = pPointView->GetPie()->GetBounds().Width / 2.0;
		double height	= pPointView->GetPie()->GetBounds().Height / 2.0;
		double minWidth  = width * fraction;
		double minHeight = height * fraction;
		if (minWidth <= 0.0 || minHeight <= 0.0)
			return CXTPChartPointF(0, 0);

		double startAngle  = pPointView->GetPie()->GetStartAngle();
		double halfAngle   = pPointView->GetPie()->GetHalfAngle();
		CXTPPoint3d center = pPointView->GetPie()->CalculateCenter(pPointView->GetBasePoint());
		CXTPChartEllipse maxEllipse(center, width, height);
		CXTPChartEllipse minEllipse(center, minWidth, minHeight);
		CXTPChartPie maxPie(startAngle, halfAngle, maxEllipse, 0, 0);
		CXTPChartPie minPie(startAngle, halfAngle, minEllipse, 0, 0);
		return CXTPChartPointF((maxPie.GetFinishPoint().X + minPie.GetFinishPoint().X) / 2.0f,
							   (maxPie.GetFinishPoint().Y + minPie.GetFinishPoint().Y) / 2.0f);
	}

	CXTPChartRectF realBounds(pPointView->GetPie()->GetBounds());

	CXTPChartEllipse actualEllipse(pPointView->GetPie()->CalculateCenter(
									   pPointView->GetBasePoint()),
								   realBounds.Width / 2, realBounds.Height / 2);
	CXTPChartPie actualPie(pPointView->GetPie()->GetStartAngle(),
						   pPointView->GetPie()->GetHalfAngle(), actualEllipse, 0, 0);

	if (!IsInside())
	{
		return actualPie.GetFinishPoint();
	}

	if (pPointView->m_dValue == 1)
	{
		lineAngle = 0;
		return actualPie.GetCenterPoint();
	}

	return CXTPChartPointF((actualPie.GetCenterPoint().X + actualPie.GetFinishPoint().X) / 2.0f,
						   (actualPie.GetCenterPoint().Y + actualPie.GetFinishPoint().Y) / 2.0f);
}

void CXTPChartPieSeriesLabelsView::ResolveOverlappingColumn(
	CArray<CXTPChartPieSeriesLabelView*, CXTPChartPieSeriesLabelView*>& arrColumn)
{
	if (arrColumn.GetSize() < 2)
		return;

	int i;

	for (i = 0; i < arrColumn.GetSize(); i++)
	{
		double& lineAngle = arrColumn[i]->m_dLineAngle;

		if (lineAngle >= CXTPMathUtils::m_dPI / 2 && lineAngle <= 3 * CXTPMathUtils::m_dPI / 2)
		{
		}
		else
		{
			if (lineAngle > 3 * CXTPMathUtils::m_dPI / 2)
				lineAngle -= CXTPMathUtils::m_dPI * 2;
			lineAngle = -lineAngle;
		}
	}

	// Yeah, bubble sort, but we're not waiting more than 20 labels here.
	for (i = 0; i < arrColumn.GetSize(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (arrColumn[i]->m_dLineAngle < arrColumn[j]->m_dLineAngle)
			{
				CXTPChartPieSeriesLabelView* p = arrColumn[i];
				arrColumn[i]				   = arrColumn[j];
				arrColumn[j]				   = p;
			}
		}
	}

	CXTPChartPieSeriesViewBase* pSeriesView =
		(CXTPChartPieSeriesViewBase*)arrColumn[0]->GetPointView()->GetSeriesView();
	CRect rcBounds = pSeriesView->GetBounds();

	for (i = (int)arrColumn.GetSize() - 2; i >= 0; i--)
	{
		CXTPChartRectF& rcColumnBounds = arrColumn[i]->m_rcBounds;

		if (arrColumn[i]->m_ptStartPoint.Y < rcColumnBounds.GetCenter().Y)
			continue;

		if (rcColumnBounds.GetBottom() + 2 > arrColumn[i + 1]->m_rcBounds.GetTop())
		{
			rcColumnBounds.Y = arrColumn[i + 1]->m_rcBounds.GetTop() - 2 - rcColumnBounds.Height;
		}
	}

	if (arrColumn[0]->m_rcBounds.GetTop() < static_cast<float>(rcBounds.top))
		arrColumn[0]->m_rcBounds.Y = static_cast<float>(rcBounds.top);

	for (i = 1; i < arrColumn.GetSize(); i++)
	{
		CXTPChartRectF& rcColumnBounds = arrColumn[i]->m_rcBounds;

		if (rcColumnBounds.GetTop() < arrColumn[i - 1]->m_rcBounds.GetBottom() + 2)
		{
			rcColumnBounds.Y = arrColumn[i - 1]->m_rcBounds.GetBottom() + 2;
		}
	}
}

void CXTPChartPieSeriesLabelsView::ResolveOverlapping()
{
	if (m_arrChildren.GetSize() == 0)
		return;

	if (((CXTPChartPieSeriesLabelView*)GetAt(0))->GetPosition() != xtpChartPieLabelTwoColumns)
		return;

	CXTPChartPieSeriesViewBase* pSeriesView =
		(CXTPChartPieSeriesViewBase*)((CXTPChartPieSeriesLabelView*)GetAt(0))
			->GetPointView()
			->GetSeriesView();
	CXTPChartRectF rcInnerBounds = pSeriesView->GetInnerBounds();
	int nCenter					 = (int)rcInnerBounds.GetCenter().X;

	CArray<CXTPChartPieSeriesLabelView*, CXTPChartPieSeriesLabelView*> arrColumn;

	int i;
	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelView* pView = (CXTPChartPieSeriesLabelView*)m_arrChildren[i];
		if (pView->m_rcBounds.GetLeft() > static_cast<float>(nCenter))
		{
			arrColumn.Add(pView);
		}
	}

	ResolveOverlappingColumn(arrColumn);

	arrColumn.RemoveAll();

	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelView* pView = (CXTPChartPieSeriesLabelView*)m_arrChildren[i];
		if (!(pView->m_rcBounds.GetLeft() > static_cast<float>(nCenter)))
		{
			arrColumn.Add(pView);
		}
	}

	ResolveOverlappingColumn(arrColumn);
}

CXTPChartPieSeriesLabelView::CXTPChartPieSeriesLabelView(CXTPChartSeriesLabel* pLabel,
														 CXTPChartSeriesPointView* pPointView,
														 CXTPChartElementView* pParentView)
	: CXTPChartPieSeriesLabelViewBase(pLabel, pPointView, pParentView)
{
}

void CXTPChartPieSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	XTPChartPieLabelPosition position = GetPosition();

	if (position
		!= xtpChartPieLabelTwoColumns) // Smart labels implemented for TwoColumns mode only.
		return;

	double lineAngle;

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartRectF realBounds(
		((CXTPChartPieSeriesPointViewBase*)m_pPointView)->GetPie()->GetBounds());

	if (realBounds.Width < 1 || realBounds.Height < 1)
		return;

	CXTPChartPieSeriesLabel* pLabel = DYNAMIC_DOWNCAST(CXTPChartPieSeriesLabel, m_pLabel);
	ASSERT(NULL != pLabel);

	CXTPChartPointF anchorPoint = pLabel->CalculateAnchorPointAndAngles(
		(CXTPChartPieSeriesPointViewBase*)m_pPointView, borderThickness, lineAngle);

	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return;

	if (position == xtpChartPieLabelTwoColumns)
	{
		CXTPChartPieSeriesViewBase* pSeriesView = (CXTPChartPieSeriesViewBase*)
													  m_pPointView->GetSeriesView();
		CXTPChartRectF rcInnerBounds = pSeriesView->GetInnerBounds();

		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartPointF startPoint(anchorPoint);

		CXTPChartPointF finishPoint(
			anchorPoint.X + static_cast<float>(cos(lineAngle) * m_pLabel->GetLineLength()),
			anchorPoint.Y - static_cast<float>(sin(lineAngle) * m_pLabel->GetLineLength()));

		CXTPChartPointF finishPoint2;

		CXTPChartRectF bounds(
			0, 0, static_cast<float>(floor(size.Width + 2 * static_cast<float>(borderThickness))),
			static_cast<float>(floor(size.Height + 2 * static_cast<float>(borderThickness))));

		bounds.Y = (float)(int)(finishPoint.Y - bounds.Height / 2);

		if (lineAngle < 0)
			lineAngle += 2 * CXTPMathUtils::m_dPI;

		if (lineAngle >= CXTPMathUtils::m_dPI / 2 && lineAngle <= 3 * CXTPMathUtils::m_dPI / 2)
		{
			bounds.X = rcInnerBounds.GetLeft()
					   - static_cast<float>(pSeriesView->GetMaxLabelWidth());
			finishPoint2 = CXTPChartPointF(bounds.GetRight(), finishPoint.Y);

			if (finishPoint.X < finishPoint2.X)
				finishPoint = finishPoint2;
		}
		else
		{
			bounds.X = rcInnerBounds.GetRight()
					   + static_cast<float>(pSeriesView->GetMaxLabelWidth()) - bounds.Width;
			finishPoint2 = CXTPChartPointF(bounds.X, finishPoint.Y);

			if (finishPoint.X > finishPoint2.X)
				finishPoint = finishPoint2;
		}
		m_rcBounds	 = bounds;
		m_ptStartPoint = startPoint;
		m_dLineAngle   = lineAngle;
	}
}

CXTPChartDeviceCommand*
	CXTPChartPieSeriesLabelView::CreateConnectorDeviceCommand(CXTPChartDeviceContext* pDC)
{
	XTPChartPieLabelPosition position = GetPosition();

	if (position == xtpChartPieLabelInside || position == xtpChartPieLabelRadial)
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartRectF realBounds(
		((CXTPChartPieSeriesPointViewBase*)m_pPointView)->GetPie()->GetBounds());

	if (realBounds.Width < 1 || realBounds.Height < 1)
		return pCommand;

	CXTPChartPieSeriesLabel* pLabel = DYNAMIC_DOWNCAST(CXTPChartPieSeriesLabel, m_pLabel);
	ASSERT(NULL != pLabel);

	double lineAngle;
	CXTPChartPointF anchorPoint = pLabel->CalculateAnchorPointAndAngles(
		(CXTPChartPieSeriesPointViewBase*)m_pPointView, borderThickness, lineAngle);

	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return pCommand;

	CXTPChartColor clrTextColor = GetActualTextColor();

	if (position == xtpChartPieLabelOutside)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartPointF startPoint(anchorPoint);
		CXTPChartPointF finishPoint(anchorPoint.X
										+ (float)(cos(lineAngle) * m_pLabel->GetLineLength()),
									anchorPoint.Y
										- (float)(sin(lineAngle) * m_pLabel->GetLineLength()));

		CXTPChartRectF innerBounds;
		CXTPChartRectF bounds =
			CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
				finishPoint, lineAngle, size, borderThickness, innerBounds);
		bounds.Round();
		innerBounds.Round();

		if (m_pLabel->GetShowLines())
		{
			CXTPChartColor clrConnectorColor = GetActualConnectorColor();

			CXTPChartSeriesLabelLineConnectorPainter linePainter(startPoint, finishPoint, bounds);
			pCommand->AddChildCommand(linePainter.CreateDeviceCommand(pDC, clrConnectorColor));
		}
	}
	else if (position == xtpChartPieLabelTwoColumns)
	{
		CXTPChartRectF bounds = m_rcBounds;

		if (m_pLabel->GetShowLines())
		{
			CXTPChartPointF startPoint(anchorPoint);
			CXTPChartPointF finishPoint;
			CXTPChartPointF breakPoint;

			breakPoint.Y = finishPoint.Y = bounds.GetCenter().Y;

			double sn = sin(lineAngle);

			if (m_rcBounds.GetLeft() >= anchorPoint.X)
			{
				breakPoint.X = finishPoint.X = bounds.GetLeft();

				if (CXTPMathUtils::Compare(sn, 0, 0.1) != 0)
				{
					double lineLength = (startPoint.Y - finishPoint.Y) / sn;

					breakPoint.X = startPoint.X + (float)(cos(lineAngle) * lineLength);

					if (breakPoint.X > finishPoint.X || breakPoint.X < startPoint.X)
						breakPoint.X = finishPoint.X;
				}
			}
			else
			{
				breakPoint.X = finishPoint.X = bounds.GetRight();

				if (CXTPMathUtils::Compare(sn, 0, 0.1) != 0)
				{
					double lineLength = (startPoint.Y - finishPoint.Y) / sn;

					breakPoint.X = startPoint.X + (float)(cos(lineAngle) * lineLength);

					if (breakPoint.X < finishPoint.X || breakPoint.X > startPoint.X)
						breakPoint.X = finishPoint.X;
				}
			}

			CXTPChartColor clrConnectorColor = GetActualConnectorColor();

			CXTPChartDeviceCommand* pContaier =
				CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand();
			pContaier->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
					startPoint, breakPoint, clrConnectorColor, 1));
			pContaier->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
					breakPoint, finishPoint, clrConnectorColor, 1));

			pCommand->AddChildCommand(pContaier);
		}
	}

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPieSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
		m_pLabel);

	double lineAngle;

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartRectF realBounds(
		((CXTPChartPieSeriesPointViewBase*)m_pPointView)->GetPie()->GetBounds());

	if (realBounds.Width < 1 || realBounds.Height < 1)
		return pCommand;

	CXTPChartPieSeriesLabel* pLabel = DYNAMIC_DOWNCAST(CXTPChartPieSeriesLabel, m_pLabel);
	ASSERT(NULL != pLabel);

	CXTPChartPointF anchorPoint = pLabel->CalculateAnchorPointAndAngles(
		(CXTPChartPieSeriesPointViewBase*)m_pPointView, borderThickness, lineAngle);

	XTPChartPieLabelPosition position = GetPosition();

	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return pCommand;

	CXTPChartColor clrTextColor = GetActualTextColor();

	if (position == xtpChartPieLabelInside)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartRectF bounds(0, 0, size.Width + static_cast<float>(2 * borderThickness),
							  size.Height + static_cast<float>(2 * borderThickness));
		bounds.Offset(anchorPoint.X - bounds.Width / 2, anchorPoint.Y - bounds.Height / 2);
		bounds.Round();

		CXTPChartRectF innerBounds = bounds;
		innerBounds.Inflate((float)-borderThickness, (float)-borderThickness);

		CXTPChartPointF labelPoint = innerBounds.GetLocation();

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pCommand->AddChildCommand(
			m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		painter.SetLocation(labelPoint);

		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pCommand->AddChildCommand(
				m_pLabel->GetBorder()->CreateDeviceCommand(bounds, clrBorderColor));
		}
	}
	else if (position == xtpChartPieLabelRadial)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartRectF bounds(0, 0, size.Width + static_cast<float>(2 * borderThickness),
							  size.Height + static_cast<float>(2 * borderThickness));
		bounds.Offset(-bounds.Width / 2, -bounds.Height / 2);
		bounds.Round();

		CXTPChartRectF innerBounds = bounds;
		innerBounds.Inflate((float)-borderThickness, (float)-borderThickness);

		float fAngle = (float)(-(int)CXTPMathUtils::Radian2Degree(lineAngle) + 90);
		fAngle		 = (float)CXTPMathUtils::NormalizeDegree(fAngle);

		fAngle = fAngle <= 180 ? fAngle - 90 : fAngle + 90;

		CXTPChartDeviceCommand* pStateGraphicsCommand = pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dSaveStateDeviceCommand());
		pStateGraphicsCommand = pStateGraphicsCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dTranslateDeviceCommand(anchorPoint.X,
																		  anchorPoint.Y));
		pStateGraphicsCommand = pStateGraphicsCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dRotateDeviceCommand(fAngle));
		pStateGraphicsCommand = pStateGraphicsCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand(TRUE));

		CXTPChartPointF labelPoint = innerBounds.GetLocation();

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pStateGraphicsCommand->AddChildCommand(
			m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		painter.SetLocation(labelPoint);

		pStateGraphicsCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pStateGraphicsCommand->AddChildCommand(
				m_pLabel->GetBorder()->CreateDeviceCommand(bounds, clrBorderColor));
		}
	}

	else if (position == xtpChartPieLabelOutside)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartPointF startPoint(anchorPoint);
		CXTPChartPointF finishPoint(anchorPoint.X
										+ (float)(cos(lineAngle) * m_pLabel->GetLineLength()),
									anchorPoint.Y
										- (float)(sin(lineAngle) * m_pLabel->GetLineLength()));

		CXTPChartRectF innerBounds;
		CXTPChartRectF bounds =
			CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
				finishPoint, lineAngle, size, borderThickness, innerBounds);
		bounds.Round();
		innerBounds.Round();

		CXTPChartPointF labelPoint = innerBounds.GetLocation();

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pCommand->AddChildCommand(
			m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		painter.SetLocation(labelPoint);
		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pCommand->AddChildCommand(
				m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(bounds, clrBorderColor));
		}
	}
	else if (position == xtpChartPieLabelTwoColumns)
	{
		CXTPChartRectF bounds = m_rcBounds;

		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		painter.SetLocation(bounds.GetLocation());

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pCommand->AddChildCommand(
			m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pCommand->AddChildCommand(
				m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(bounds, clrBorderColor));
		}
	}

	return pCommand;
}

CXTPChartDeviceCommand*
	CXTPChartPieSeriesLabelsView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	int i;
	CXTPChartDeviceCommand* pDrawingType = new CXTPChartDeviceCommand();

	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelViewBase* pChildView = static_cast<CXTPChartPieSeriesLabelViewBase*>(
			m_arrChildren[i]);
		pDrawingType->AddChildCommand(pChildView->CreateConnectorDeviceCommand(pDC));
	}

	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelViewBase* pChildView = static_cast<CXTPChartPieSeriesLabelViewBase*>(
			m_arrChildren[i]);
		pDrawingType->AddChildCommand(pChildView->CreateDeviceCommand(pDC));
	}

	return pDrawingType;
}
