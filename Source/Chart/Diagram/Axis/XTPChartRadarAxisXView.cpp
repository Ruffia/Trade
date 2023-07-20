// XTPChartRadarAxisXView.cpp
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
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxis.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisView.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisXView.h"
#include "Chart/Diagram/Axis/XTPChartAxisLabel.h"
#include "Chart/Diagram/Axis/XTPChartAxisGridLines.h"
#include "Chart/Diagram/Axis/XTPChartAxisTickMarks.h"
#include "Chart/Diagram/Axis/XTPChartAxisConstantLines.h"
#include "Chart/Diagram/Axis/XTPChartAxisStrips.h"
#include "Chart/Diagram/Axis/XTPChartAxisCustomLabels.h"
#include "Chart/Diagram/Axis/XTPChartScaleTypeMap.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartCircleDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartPieDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"
#include "Chart/Appearance/XTPChartLineStyle.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SCROLLBARSIZE 10

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAxisXView

CXTPChartRadarAxisXView::CXTPChartRadarAxisXView(CXTPChartAxis* pAxis,
												 CXTPChartElementView* pParentView)
	: CXTPChartRadarAxisView(pAxis, pParentView)
{
	m_dGridSpacing = 1;

	ASSERT(m_pContainer);

	m_nSize = 0;

	m_rcBounds.SetRectEmpty();

	m_ptCenter = CXTPPoint2i(0, 0);
	m_nRadius  = 0;

	m_dMinValue = 0;
	m_dMaxValue = 0;
}

CXTPChartRadarAxisXView::~CXTPChartRadarAxisXView()
{
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisXView::CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	if (!m_pAxis->IsInterlaced())
		return NULL;

	if (m_arrTicks.GetSize() < 2)
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	CXTPChartColor color1 = m_pAxis->GetActualInterlacedColor();
	CXTPChartColor color2 = m_pAxis->GetActualInterlacedColor2();

	for (int i = 0; i + 1 < m_arrTicks.GetSize(); i += 2)
	{
		double dFrom = ValueToAngle(m_arrTicks[i].m_dValue);
		double dTo   = ValueToAngle(m_arrTicks[i + 1].m_dValue);

		if (dTo > dFrom)
			dTo -= 2 * CXTPMathUtils::m_dPI;

		if (IsPolygonDiagramStyle())
		{
			CXTPChartPoints pts;
			pts.Add(m_ptCenter);
			pts.Add(CXTPChartPointF(m_ptCenter.X + cos(dFrom) * m_nRadius,
									m_ptCenter.Y - sin(dFrom) * m_nRadius));
			pts.Add(CXTPChartPointF(m_ptCenter.X + cos(dTo) * m_nRadius,
									m_ptCenter.Y - sin(dTo) * m_nRadius));

			pCommands->AddChildCommand(
				m_pAxis->GetInterlacedFillStyle()->CreateDeviceCommand(pts, color1, color2));
		}
		else
		{
			CXTPChartDeviceCommand* pCommand =
				CXTPChartDrawingObjectFactory::Create2dGradientPieDeviceCommand(
					CXTPPoint3d(m_ptCenter), m_nRadius, m_nRadius,
					-CXTPMathUtils::Radian2Degree(dFrom), CXTPMathUtils::Radian2Degree(dFrom - dTo),
					0, 0, m_rcBounds, color1, color2);
			pCommands->AddChildCommand(pCommand);
		}
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisXView::CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartAxisStrips* pStrips = m_pAxis->GetStrips();
	if (pStrips->GetCount() == 0)
		return NULL;

	if (IsPolygonDiagramStyle()) // TODO
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	for (int i = 0; i < pStrips->GetCount(); i++)
	{
		CXTPChartAxisStrip* pStrip = pStrips->GetAt(i);
		if (!pStrip->IsVisible())
			continue;

		double dMarkLeft = !pStrip->GetAxisMinValue().IsEmpty()
							   ? m_pAxis->GetScaleTypeMap()->ValueToInternal(
									 pStrip->GetAxisMinValue())
							   : pStrip->GetAxisMinValueInternal();

		double dMarkRight = !pStrip->GetAxisMaxValue().IsEmpty()
								? m_pAxis->GetScaleTypeMap()->ValueToInternal(
									  pStrip->GetAxisMaxValue())
								: pStrip->GetAxisMaxValueInternal();

		double dFrom = ValueToAngle(dMarkLeft);
		double dTo   = ValueToAngle(dMarkRight);

		if (dTo > dFrom)
			dTo -= 2 * CXTPMathUtils::m_dPI;

		CXTPChartDeviceCommand* pCommand =
			CXTPChartDrawingObjectFactory::Create2dGradientPieDeviceCommand(
				CXTPPoint3d(m_ptCenter), m_nRadius, m_nRadius, -CXTPMathUtils::Radian2Degree(dFrom),
				-CXTPMathUtils::Radian2Degree(dTo - dFrom), 0, 0, m_rcBounds,
				pStrip->GetActualColor(), pStrip->GetActualColor2());
		pCommands->AddChildCommand(pCommand);
	}

	return pCommands;
}

CXTPChartDeviceCommand* CXTPChartRadarAxisXView::CreateConstantLinesDeviceCommand(
	CXTPChartDeviceContext* pDC, BOOL bBehind)
{
	CXTPChartAxisConstantLines* pConstantLines = m_pAxis->GetConstantLines();
	if (pConstantLines->GetCount() == 0)
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	for (int i = 0; i < pConstantLines->GetCount(); i++)
	{
		CXTPChartAxisConstantLine* pConstantLine = pConstantLines->GetAt(i);
		if (!pConstantLine->IsVisible())
			continue;

		if (pConstantLine->IsShowBehind() != bBehind)
			continue;

		double dMark = !pConstantLine->GetAxisValue().IsEmpty()
						   ? m_pAxis->GetScaleTypeMap()->ValueToInternal(
								 pConstantLine->GetAxisValue())
						   : pConstantLine->GetAxisValueInternal();

		double lineAngle = ValueToAngle(dMark);

		CXTPPoint3d ptEnd(m_ptCenter.X + cos(lineAngle) * m_nRadius,
						  m_ptCenter.Y - sin(lineAngle) * m_nRadius);

		pCommands->AddChildCommand(
			pConstantLine->GetLineStyle()->CreateDeviceCommand(CXTPPoint3d(m_ptCenter), ptEnd,
															   pConstantLine->GetActualColor()));

		CXTPChartString strText = pConstantLine->GetText();

		CPoint ptText(int(m_ptCenter.X + cos(lineAngle) * (m_nRadius - 5)),
					  int(m_ptCenter.Y - sin(lineAngle) * (m_nRadius - 5)));

		CXTPChartRotatedTextPainterNearLine painter(pDC, strText, pConstantLine, ptText,
													xtpChartTextNearTop,
													-(float)CXTPMathUtils::Radian2Degree(
														lineAngle));
		pCommands->AddChildCommand(
			painter.CreateDeviceCommand(pDC, pConstantLine->GetActualTextColor()));
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisXView::CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartAxisGridLines* pGridLines = m_pAxis->GetGridLines();

	if (!pGridLines->IsVisible())
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	CXTPChartColor clrGridLines		 = pGridLines->GetColor();
	CXTPChartColor clrMinorGridLines = pGridLines->GetMinorColor();
	BOOL bMinorVisible				 = pGridLines->IsMinorVisible();

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		double lineAngle = ValueToAngle(m_arrTicks[i].m_dValue);

		pCommands->AddChildCommand(pGridLines->GetLineStyle()->CreateDeviceCommand(
			CXTPPoint3d(m_ptCenter.X, m_ptCenter.Y),
			CXTPPoint3d(m_ptCenter.X + cos(lineAngle) * m_nRadius,
						m_ptCenter.Y - sin(lineAngle) * m_nRadius),
			clrGridLines));
	}

	if (bMinorVisible && !IsPolygonDiagramStyle())
	{
		for (int i = 0; i < m_arrMinorTicks.GetSize(); i++)
		{
			double lineAngle = ValueToAngle(m_arrMinorTicks[i]);

			pCommands->AddChildCommand(pGridLines->GetMinorLineStyle()->CreateDeviceCommand(
				CXTPPoint3d(m_ptCenter.X, m_ptCenter.Y),
				CXTPPoint3d(m_ptCenter.X + cos(lineAngle) * m_nRadius,
							m_ptCenter.Y - sin(lineAngle) * m_nRadius),
				clrMinorGridLines));
		}
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisXView::CreateTickMarksDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDeviceCommand* pCommands =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pAxis, m_rcBounds);

	CXTPChartAxisTickMarks* pTickMarks = m_pAxis->GetTickMarks();

	if (!pTickMarks->IsVisible())
		return pCommands;

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();
	int nLength			   = pTickMarks->GetLength();
	int nThickness		   = pTickMarks->GetThickness();
	BOOL bMinorVisible	 = pTickMarks->IsMinorVisible();
	BOOL bCross			   = pTickMarks->IsCrossAxis();
	int nAxisThickness	 = m_pAxis->GetThickness() - 1;
	int nExtraLength	   = bCross ? nLength + nAxisThickness : 0;

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		double lineAngle = ValueToAngle(m_arrTicks[i].m_dValue);

		double cs = cos(lineAngle);
		double sn = sin(lineAngle);

		CXTPChartPointF startPoint(m_ptCenter.X + (m_nRadius - nExtraLength) * cs,
								   m_ptCenter.Y - (m_nRadius - nExtraLength) * sn);
		CXTPChartPointF finishPoint(m_ptCenter.X + (m_nRadius + nAxisThickness + nLength) * cs,
									m_ptCenter.Y - (m_nRadius + nAxisThickness + nLength) * sn);

		pCommands->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(startPoint, finishPoint,
																		  clrAxis, nThickness));
	}

	if (bMinorVisible && !IsPolygonDiagramStyle())
	{
		nLength				= pTickMarks->GetMinorLength();
		int nExtra			= bCross ? nLength + nAxisThickness : 0;
		int nMinorThickness = pTickMarks->GetMinorThickness();

		for (int i = 0; i < m_arrMinorTicks.GetSize(); i++)
		{
			double lineAngle = ValueToAngle(m_arrMinorTicks[i]);

			double cs = cos(lineAngle);
			double sn = sin(lineAngle);

			CXTPChartPointF startPoint(m_ptCenter.X + (m_nRadius - nExtra) * cs,
									   m_ptCenter.Y - (m_nRadius - nExtra) * sn);
			CXTPChartPointF finishPoint(m_ptCenter.X + (m_nRadius + nAxisThickness + nLength) * cs,
										m_ptCenter.Y - (m_nRadius + nAxisThickness + nLength) * sn);

			pCommands->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(startPoint,
																			  finishPoint, clrAxis,
																			  nMinorThickness));
		}
	}

	return pCommands;
}

double CXTPChartRadarAxisXView::ValueToAngle(double nValue) const
{
	double delta = (nValue - m_dMinValue) * 360 / (m_dMaxValue - m_dMinValue);

	CXTPChartRadarDiagram* pDiagram  = (CXTPChartRadarDiagram*)GetAxis()->GetDiagram();
	CXTPChartRadarAxisX* pRadarAxisX = DYNAMIC_DOWNCAST(CXTPChartRadarAxisX, GetAxis());

	delta = CXTPMathUtils::NormalizeDegree(
		90 - pDiagram->GetStartAngle() - delta * (pRadarAxisX->IsDirectionClockwise() ? 1 : -1));

	return CXTPMathUtils::Degree2Radian(delta);
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisXView::CreateLabelsDeviceCommand(CXTPChartDeviceContext* pDC)
{
	if (!m_pAxis->GetLabel()->IsVisible())
		return NULL;

	CXTPChartDeviceCommand* pLabelsCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pAxis->GetLabel(),
																	 m_rcBounds);
	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	CXTPChartColor clrTextColor = m_pAxis->GetLabel()->GetActualTextColor();

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		CXTPChartString text = m_arrTicks[i].m_strLabel;

		CXTPChartTextPainter painter(pDC, text, m_pAxis->GetLabel());
		painter.SetLocation(m_arrTicks[i].m_ptLocation);

		pLabelsCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));
	}

	return pLabelsCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarAxisXView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pAxisCommand =
		CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand(TRUE);

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	int nThickness = m_pAxis->GetThickness();

	if (m_pAxis->IsVisible())
	{
		if (nThickness > 0)
		{
			if (IsPolygonDiagramStyle())
			{
				CXTPChartLineStyle lineStyle(NULL);
				lineStyle.SetThickness(nThickness);

				pAxisCommand->AddChildCommand(
					GetDiagramView()->CreatePolygonLineDeviceCommand(pDC, m_nRadius, clrAxis,
																	 &lineStyle));
			}
			else
			{
				pAxisCommand->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dBoundedCircleDeviceCommand(
						m_ptCenter, m_nRadius, clrAxis, nThickness));
			}
		}

		pAxisCommand->AddChildCommand(CreateTickMarksDeviceCommand(pDC));
		pAxisCommand->AddChildCommand(CreateLabelsDeviceCommand(pDC));
	}

	return pAxisCommand;
}

void CXTPChartRadarAxisXView::SetBounds(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	m_rcBounds = rcBounds;

	m_ptCenter.X = rcBounds.CenterPoint().x;
	m_ptCenter.Y = rcBounds.CenterPoint().y;
	m_nRadius	= rcBounds.Width() / 2;
}

void CXTPChartRadarAxisXView::CreateTickMarks(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	m_arrTicks.RemoveAll();
	m_arrMinorTicks.RemoveAll();

	double dAxisMinValue = m_dMinValue;
	double dAxisMaxValue = m_dMaxValue;
	double dGridSpacing  = GetGridSpacing();
	int nOffset			 = m_pAxis->GetThickness()
				  + (m_pAxis->GetTickMarks()->IsVisible() ? m_pAxis->GetTickMarks()->GetLength()
														  : 0);

	if (m_pAxis->GetCustomLabels()->GetCount() > 0)
	{
		CXTPChartAxisCustomLabels* pCustomLabels = m_pAxis->GetCustomLabels();
		int nCount								 = pCustomLabels->GetCount();

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartAxisCustomLabel* pLabel = pCustomLabels->GetAt(i);

			CXTPChartRadarAxisXViewTick tick;
			tick.m_dValue = !pLabel->GetAxisValue().IsEmpty()
								? m_pAxis->GetScaleTypeMap()->ValueToInternal(
									  pLabel->GetAxisValue())
								: pLabel->GetAxisValueInternal();

			if (tick.m_dValue >= dAxisMinValue && tick.m_dValue <= dAxisMaxValue)
			{
				tick.m_strLabel = pLabel->GetText();

				CXTPChartTextPainter painter(pDC, tick.m_strLabel, m_pAxis->GetLabel());

				tick.m_szLabel = painter.GetSize();

				tick.m_szBounds = painter.GetRoundedBounds().Size();

				double lineAngle = ValueToAngle(tick.m_dValue);

				CXTPChartSizeF size = painter.GetSize();

				CXTPChartPointF startPoint(m_ptCenter.X + m_nRadius * cos(lineAngle),
										   m_ptCenter.Y - m_nRadius * sin(lineAngle));
				CXTPChartPointF finishPoint(startPoint.X + (float)(cos(lineAngle) * nOffset),
											startPoint.Y - (float)(sin(lineAngle) * nOffset));

				CXTPChartRectF innerBounds;
				CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
					finishPoint, lineAngle, size, 0, innerBounds);
				innerBounds.Round();

				tick.m_ptLocation = innerBounds.GetLocation();

				m_arrTicks.Add(tick);
			}
		}
	}
	else
	{
		double dMark = m_dMinValue;

		while (dMark < dAxisMaxValue - CXTPMathUtils::m_dEPS)
		{
			CXTPChartRadarAxisXViewTick tick;
			tick.m_dValue = AxisToValue(dMark);

			tick.m_strLabel = m_pAxis->GetScaleTypeMap()->InternalToValue(
				m_pAxis->GetLabel()->GetFormat(), tick.m_dValue);

			CXTPChartTextPainter painter(pDC, tick.m_strLabel, m_pAxis->GetLabel());

			tick.m_szLabel = painter.GetSize();

			tick.m_szBounds = painter.GetRoundedBounds().Size();

			double lineAngle = ValueToAngle(tick.m_dValue);

			CXTPChartSizeF size = painter.GetSize();

			CXTPChartPointF startPoint(m_ptCenter.X + m_nRadius * cos(lineAngle),
									   m_ptCenter.Y - m_nRadius * sin(lineAngle));
			CXTPChartPointF finishPoint(startPoint.X + (float)(cos(lineAngle) * nOffset),
										startPoint.Y - (float)(sin(lineAngle) * nOffset));

			CXTPChartRectF innerBounds;
			CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(
				finishPoint, lineAngle, size, 0, innerBounds);
			innerBounds.Round();

			tick.m_ptLocation = innerBounds.GetLocation();

			m_arrTicks.Add(tick);

			dMark += dGridSpacing;
		}

		int nMinorCount = m_pAxis->GetMinorCount();

		if (m_arrTicks.GetSize() > 0 && nMinorCount > 0)
		{
			double cur, prev;

			for (int i = 0; i <= m_arrTicks.GetSize(); i++)
			{
				if (m_pAxis->IsLogarithmic())
				{
					cur = i == m_arrTicks.GetSize()
							  ? m_arrTicks[i - 1].m_dValue * m_pAxis->GetLogarithmicBase()
							  : m_arrTicks[i].m_dValue;

					prev = i == 0 ? m_arrTicks[0].m_dValue / m_pAxis->GetLogarithmicBase()
								  : m_arrTicks[i - 1].m_dValue;
				}
				else
				{
					cur = i == m_arrTicks.GetSize() ? m_arrTicks[i - 1].m_dValue + dGridSpacing
													: m_arrTicks[i].m_dValue;
					prev = i == 0 ? cur - dGridSpacing : m_arrTicks[i - 1].m_dValue;
				}

				for (int j = 0; j < nMinorCount; j++)
				{
					double dValue = prev + (cur - prev) * (j + 1) / (nMinorCount + 1);

					if (dValue >= dAxisMinValue && dValue <= dAxisMaxValue)
					{
						m_arrMinorTicks.Add(dValue);
					}
				}
			}
		}
	}

	if (GetAxis()->IsVisible() && GetAxis()->GetLabel()->IsVisible())
	{
		CXTPChartRadarDiagramView* pDiagramView = GetDiagramView();

		for (int i = 0; i < m_arrTicks.GetSize(); i++)
		{
			CXTPChartRectF rc(m_arrTicks[i].m_ptLocation, m_arrTicks[i].m_szLabel);
			pDiagramView->CheckLabelBounds(rc);
		}
	}
}

void CXTPChartRadarAxisXView::CalcSize(CXTPChartDeviceContext* pDC, CRect rcDiagram)
{
	m_ptCenter.X = rcDiagram.CenterPoint().x;
	m_ptCenter.Y = rcDiagram.CenterPoint().y;
	m_nRadius	= rcDiagram.Width() / 2;

	CXTPChartAxis* pAxis = GetAxis();

	double dScreenLength = 2.0 * m_nRadius * CXTPMathUtils::m_dPI;

	if (dScreenLength < 1)
	{
		m_nSize = 0;
		return;
	}

	if (pAxis->GetGridSpacingAuto())
	{
		double dRangeDelta = GetAxisRangeMaxValue() - GetAxisRangeMinValue();

		m_dGridSpacing = CalculateGridSpacing(dRangeDelta, dScreenLength, 50);

		if (pAxis->IsLogarithmic())
			m_dGridSpacing = max(1, m_dGridSpacing);
	}
	else
	{
		m_dGridSpacing = pAxis->GetGridSpacing();
	}

	m_dMinValue = int(GetRangeMinValue() / m_dGridSpacing) * m_dGridSpacing;
	if (m_dMinValue > GetRangeMinValue() + CXTPMathUtils::m_dEPS)
		m_dMinValue -= m_dGridSpacing;

	m_dMaxValue = int(GetRangeMaxValue() / m_dGridSpacing) * m_dGridSpacing;
	if (m_dMaxValue < GetRangeMaxValue() - CXTPMathUtils::m_dEPS)
		m_dMaxValue += m_dGridSpacing;

	m_dMaxValue += m_dGridSpacing;

	pAxis->m_dGridSpacing = m_dGridSpacing;

	CreateTickMarks(pDC);

	if (!pAxis->IsVisible())
	{
		m_nSize = 0;
		return;
	}

	m_nSize = pAxis->GetThickness();

	if (pAxis->GetTickMarks()->IsVisible())
	{
		m_nSize += pAxis->GetTickMarks()->GetLength();
	}

	if (pAxis->GetLabel()->IsVisible())
	{
		int nLabelSize = 0;

		for (int i = 0; i < m_arrTicks.GetSize(); i++)
		{
			CSize szBounds = m_arrTicks[i].m_szBounds;

			nLabelSize = max(nLabelSize, szBounds.cx);
		}

		m_nSize += nLabelSize;
	}
}

void CXTPChartRadarAxisXView::CreateView(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}
