// XTPChartRadarAxisYView.cpp
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
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagramView.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisView.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisYView.h"
#include "Chart/Diagram/Axis/XTPChartAxisLabel.h"
#include "Chart/Diagram/Axis/XTPChartAxisGridLines.h"
#include "Chart/Diagram/Axis/XTPChartAxisTickMarks.h"
#include "Chart/Diagram/Axis/XTPChartAxisTitle.h"
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
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"

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
// CXTPChartRadarAxisYView

CXTPChartRadarAxisYView::CXTPChartRadarAxisYView(CXTPChartAxis* pAxis,
												 CXTPChartElementView* pParentView)
	: CXTPChartRadarAxisView(pAxis, pParentView)
{
	m_dGridSpacing = 1;

	ASSERT(m_pContainer);

	m_nSize = 0;

	m_rcPaneBounds.SetRectEmpty();

	m_rcBounds.SetRectEmpty();

	m_ptCenter = CXTPPoint2i(0, 0);
	m_nRadius  = 0;
}

CXTPChartRadarAxisYView::~CXTPChartRadarAxisYView()
{
}

BOOL CXTPChartRadarAxisView::IsPolygonDiagramStyle() const
{
	CXTPChartRadarDiagram* pDiagram = (CXTPChartRadarDiagram*)m_pAxis->GetDiagram();

	return pDiagram->GetStyle() == xtpChartRadarDiagramPolygon;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisYView::CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	if (!m_pAxis->IsInterlaced())
		return NULL;

	if (m_arrTicks.GetSize() < 1)
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	CXTPChartColor color1 = m_pAxis->GetActualInterlacedColor();
	CXTPChartColor color2 = m_pAxis->GetActualInterlacedColor2();

	for (int i = 0; i < m_arrTicks.GetSize(); i += 2)
	{
		double dRFrom = ValueToRadius(m_arrTicks[i].m_dValue);
		double dRTo   = i + 1 == m_arrTicks.GetSize() ? m_nRadius
													: ValueToRadius(m_arrTicks[i + 1].m_dValue);

		if (IsPolygonDiagramStyle())
		{
			CXTPChartRadarDiagramView* pDiagramView = GetDiagramView();

			pCommands->AddChildCommand(
				pDiagramView->CreatePolygonStripDeviceCommand(pDC, dRFrom, dRTo, color1, color2,
															  m_pAxis->GetInterlacedFillStyle()));
		}
		else
		{
			CXTPChartDeviceCommand* pCommand =
				CXTPChartDrawingObjectFactory::Create2dGradientPieDeviceCommand(
					CXTPPoint3d(m_ptCenter), dRTo, dRTo, 0, 360, 0, dRFrom / dRTo * 100,
					m_rcPaneBounds, color1, color2);
			pCommands->AddChildCommand(pCommand);
		}
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisYView::CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	CXTPChartAxisStrips* pStrips = m_pAxis->GetStrips();
	if (pStrips->GetCount() == 0)
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

		double dRFrom = ValueToRadius(dMarkLeft);
		double dRTo   = ValueToRadius(dMarkRight);

		if (IsPolygonDiagramStyle())
		{
			pCommands->AddChildCommand(GetDiagramView()->CreatePolygonStripDeviceCommand(
				pDC, dRFrom, dRTo, pStrip->GetActualColor(), pStrip->GetActualColor2(),
				pStrip->GetFillStyle()));
		}
		else
		{
			CXTPChartDeviceCommand* pCommand =
				CXTPChartDrawingObjectFactory::Create2dGradientPieDeviceCommand(
					CXTPPoint3d(m_ptCenter), dRTo, dRTo, 0, 360, 0, dRFrom / dRTo * 100,
					m_rcPaneBounds, pStrip->GetActualColor(), pStrip->GetActualColor2());
			pCommands->AddChildCommand(pCommand);
		}
	}

	return pCommands;
}

CXTPChartDeviceCommand* CXTPChartRadarAxisYView::CreateConstantLinesDeviceCommand(
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

		double dRad = ValueToRadius(dMark);

		if (IsPolygonDiagramStyle())
			pCommands->AddChildCommand(GetDiagramView()->CreatePolygonLineDeviceCommand(
				pDC, dRad, pConstantLine->GetActualColor(), pConstantLine->GetLineStyle()));
		else
			pCommands->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedCircleDeviceCommand(
					m_ptCenter, dRad, pConstantLine->GetActualColor(),
					pConstantLine->GetLineStyle()->GetThickness()));
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisYView::CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC)
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
		double dRadius = ValueToRadius(m_arrTicks[i].m_dValue);

		if (IsPolygonDiagramStyle())
			pCommands->AddChildCommand(
				GetDiagramView()->CreatePolygonLineDeviceCommand(pDC, dRadius, clrGridLines,
																 pGridLines->GetLineStyle()));
		else
			pCommands->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dBoundedCircleDeviceCommand(
					m_ptCenter, dRadius, clrGridLines, pGridLines->GetLineStyle()->GetThickness()));
	}

	if (bMinorVisible)
	{
		for (int i = 0; i < m_arrMinorTicks.GetSize(); i++)
		{
			double dRadius = ValueToRadius(m_arrMinorTicks[i]);

			if (IsPolygonDiagramStyle())
				pCommands->AddChildCommand(GetDiagramView()->CreatePolygonLineDeviceCommand(
					pDC, dRadius, clrMinorGridLines, pGridLines->GetMinorLineStyle()));
			else
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dBoundedCircleDeviceCommand(
						m_ptCenter, dRadius, clrMinorGridLines,
						pGridLines->GetMinorLineStyle()->GetThickness()));
		}
	}

	return pCommands;
}

double CXTPChartRadarAxisYView::ValueToRadius(double x) const
{
	double dMinValue = GetAxisRangeMinValue();
	double dMaxValue = GetAxisRangeMaxValue();

	x = ValueToAxis(x);

	BOOL bRevered = m_pAxis->IsReversed();

	double dValue;

	if (bRevered)
	{
		dValue = m_nRadius - (x - dMinValue) / (dMaxValue - dMinValue) * m_nRadius;
	}
	else
	{
		dValue = (x - dMinValue) / (dMaxValue - dMinValue) * m_nRadius;
	}

	return dValue;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisYView::CreateTickMarksDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDeviceCommand* pCommands =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pAxis, m_rcBounds);

	CXTPChartAxisTickMarks* pTickMarks = m_pAxis->GetTickMarks();

	if (!pTickMarks->IsVisible())
		return pCommands;

	int nScrollBarSize = 0;

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();
	int nLength			   = pTickMarks->GetLength();
	int nThickness		   = pTickMarks->GetThickness();
	BOOL bMinorVisible	 = pTickMarks->IsMinorVisible();
	BOOL bCross			   = pTickMarks->IsCrossAxis();
	int nAxisThickness	 = m_pAxis->GetThickness() - 1 + nScrollBarSize;
	int nExtraLength	   = bCross ? nLength + nAxisThickness : 0;

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		int nLeft = m_ptCenter.Y - (int)ValueToRadius(m_arrTicks[i].m_dValue);

		if (m_pAxis->GetAlignment() == xtpChartAxisNear)
			pCommands->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
					CXTPPoint3d(m_rcBounds.right - nAxisThickness - nLength, nLeft),
					CXTPPoint3d(m_rcBounds.right - nAxisThickness + nExtraLength, nLeft), clrAxis,
					nThickness));
		else
			pCommands->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
					CXTPPoint3d(m_rcBounds.left + nAxisThickness - nExtraLength, nLeft),
					CXTPPoint3d(m_rcBounds.left + nAxisThickness + nLength, nLeft), clrAxis,
					nThickness));
	}

	if (bMinorVisible)
	{
		nLength				= pTickMarks->GetMinorLength();
		int nExtra			= bCross ? nLength + nAxisThickness : 0;
		int nMinorThickness = pTickMarks->GetMinorThickness();

		for (int i = 0; i < m_arrMinorTicks.GetSize(); i++)
		{
			int nLeft = m_ptCenter.Y - (int)ValueToRadius(m_arrMinorTicks[i]);

			if (m_pAxis->GetAlignment() == xtpChartAxisNear)
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
						CXTPPoint3d(m_rcBounds.right - nAxisThickness - nLength, nLeft),
						CXTPPoint3d(m_rcBounds.right - nAxisThickness + nExtra, nLeft), clrAxis,
						nMinorThickness));
			else
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
						CXTPPoint3d(m_rcBounds.left + nAxisThickness - nExtra, nLeft),
						CXTPPoint3d(m_rcBounds.left + nAxisThickness + nLength, nLeft), clrAxis,
						nMinorThickness));
		}
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisYView::CreateLabelsDeviceCommand(CXTPChartDeviceContext* pDC)
{
	if (!m_pAxis->GetLabel()->IsVisible())
		return NULL;

	CXTPChartDeviceCommand* pLabelsCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pAxis->GetLabel(),
																	 m_rcBounds);
	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	int nAngle = m_pAxis->GetLabel()->GetAngle();
	BOOL bNear = m_pAxis->GetAlignment() == xtpChartAxisNear;

	int nOffset = m_pAxis->GetThickness()
				  + (m_pAxis->GetTickMarks()->IsVisible() ? m_pAxis->GetTickMarks()->GetLength()
														  : 0);

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		int nLeft		  = m_ptCenter.Y - (int)ValueToRadius(m_arrTicks[i].m_dValue);
		CXTPChartString s = m_arrTicks[i].m_strLabel;

		CXTPChartRotatedTextPainterNearLine painter(
			pDC, s, m_pAxis->GetLabel(),
			CPoint(bNear ? m_rcBounds.right - nOffset : m_rcBounds.left + nOffset, nLeft),
			bNear ? xtpChartTextNearLeft : xtpChartTextNearRight, (float)nAngle);
		pLabelsCommand->AddChildCommand(
			painter.CreateDeviceCommand(pDC, m_pAxis->GetLabel()->GetActualTextColor()));
	}

	return pLabelsCommand;
}

CXTPChartDeviceCommand*
	CXTPChartRadarAxisYView::CreateTitleDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartAxisTitle* pTitle = GetAxis()->GetTitle();
	if (!pTitle->IsVisible())
		return NULL;

	CXTPChartString strText = pTitle->GetText();

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(pTitle, m_rcBounds);

	if (m_pAxis->GetAlignment() == xtpChartAxisNear)
	{
		CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
													CPoint(m_rcBounds.left,
														   m_rcBounds.CenterPoint().y),
													xtpChartTextNearRight, 270);
		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor()));
	}
	else
	{
		CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
													CPoint(m_rcBounds.right,
														   m_rcBounds.CenterPoint().y),
													xtpChartTextNearLeft, 90);
		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor()));
	}

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarAxisYView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pAxisCommand = new CXTPChartDeviceCommand();

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	int nScrollBarSize = 0;
	int nThickness	 = m_pAxis->GetThickness();

	if (m_pAxis->IsVisible())
	{
		float fPosition = m_pAxis->GetAlignment() == xtpChartAxisNear
							  ? static_cast<float>(m_rcBounds.right)
									- static_cast<float>(nScrollBarSize)
									- static_cast<float>(nThickness) / 2.0f + 0.5f
							  : static_cast<float>(m_rcBounds.left)
									+ static_cast<float>(nScrollBarSize)
									+ static_cast<float>(nThickness) / 2.0f - 0.5f;

		pAxisCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
			CXTPPoint3d(fPosition, (float)m_rcBounds.top),
			CXTPPoint3d(fPosition, static_cast<float>(m_rcBounds.bottom) + 0.5f), clrAxis,
			m_pAxis->GetThickness()));

		pAxisCommand->AddChildCommand(CreateTickMarksDeviceCommand(pDC));
		pAxisCommand->AddChildCommand(CreateLabelsDeviceCommand(pDC));
		pAxisCommand->AddChildCommand(CreateTitleDeviceCommand(pDC));
	}

	return pAxisCommand;
}

void CXTPChartRadarAxisYView::SetBounds(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);

	m_rcPaneBounds = rcBounds;

	CPoint ptCenter = rcBounds.CenterPoint();

	m_rcBounds = CRect(ptCenter.x - m_nSize, rcBounds.top, ptCenter.x, ptCenter.y);

	m_ptCenter.X = rcBounds.CenterPoint().x;
	m_ptCenter.Y = rcBounds.CenterPoint().y;
	m_nRadius	= rcBounds.Width() / 2;
}

void CXTPChartRadarAxisYView::CreateTickMarks(CXTPChartDeviceContext* pDC, CRect rcDiagram)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcDiagram);

	m_arrTicks.RemoveAll();
	m_arrMinorTicks.RemoveAll();

	double dViewMinValue = GetViewRangeMinValue();
	double dViewMaxValue = GetViewRangeMaxValue();

	double dAxisMinValue = GetAxisRangeMinValue();
	double dAxisMaxValue = GetAxisRangeMaxValue();
	double dGridSpacing  = GetGridSpacing();

	BOOL bNear = m_pAxis->GetAlignment() == xtpChartAxisNear;
	int nAngle = m_pAxis->GetLabel()->GetAngle();

	if (m_pAxis->GetCustomLabels()->GetCount() > 0)
	{
		CXTPChartAxisCustomLabels* pCustomLabels = m_pAxis->GetCustomLabels();
		int nCount								 = pCustomLabels->GetCount();

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartAxisCustomLabel* pLabel = pCustomLabels->GetAt(i);

			CXTPChartRadarAxisYViewTick tick;
			tick.m_dValue = !pLabel->GetAxisValue().IsEmpty()
								? m_pAxis->GetScaleTypeMap()->ValueToInternal(
									  pLabel->GetAxisValue())
								: pLabel->GetAxisValueInternal();

			if (tick.m_dValue >= dViewMinValue && tick.m_dValue <= dViewMaxValue)
			{
				tick.m_strLabel = pLabel->GetText();

				CXTPChartRotatedTextPainterNearLine painter(
					pDC, tick.m_strLabel, m_pAxis->GetLabel(), CPoint(0, 0),
					(bNear ? xtpChartTextNearLeft : xtpChartTextNearRight), (float)nAngle);

				tick.m_szLabel = painter.GetSize();

				tick.m_szBounds = painter.GetRoundedBounds().Size();

				m_arrTicks.Add(tick);
			}
		}
	}
	else
	{
		double dMark = ((int)(dAxisMinValue / dGridSpacing)) * dGridSpacing;
		if (dMark < dAxisMinValue)
			dMark += dGridSpacing;
		while (dMark < dAxisMaxValue + CXTPMathUtils::m_dEPS)
		{
			CXTPChartRadarAxisYViewTick tick;
			tick.m_dValue = AxisToValue(dMark);

			tick.m_strLabel = m_pAxis->GetScaleTypeMap()->InternalToValue(
				m_pAxis->GetLabel()->GetFormat(), tick.m_dValue);

			CXTPChartRotatedTextPainterNearLine painter(
				pDC, tick.m_strLabel, m_pAxis->GetLabel(), CPoint(0, 0),
				(bNear ? xtpChartTextNearLeft : xtpChartTextNearRight), (float)nAngle);

			tick.m_szLabel = painter.GetSize();

			tick.m_szBounds = painter.GetRoundedBounds().Size();

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

					if (dValue >= dViewMinValue && dValue <= dViewMaxValue)
					{
						m_arrMinorTicks.Add(dValue);
					}
				}
			}
		}
	}
}

void CXTPChartRadarAxisYView::CalcSize(CXTPChartDeviceContext* pDC, CRect rcDiagram)
{
	CXTPChartAxis* pAxis = GetAxis();

	double dScreenLength = rcDiagram.Height() / 2;

	if (dScreenLength < 1)
	{
		m_nSize = 0;
		return;
	}

	if (pAxis->GetGridSpacingAuto())
	{
		double dRangeDelta = GetAxisRangeMaxValue() - GetAxisRangeMinValue();

		m_dGridSpacing = CalculateGridSpacing(dRangeDelta, dScreenLength, 30);

		if (pAxis->IsLogarithmic())
			m_dGridSpacing = max(1, m_dGridSpacing);
	}
	else
	{
		m_dGridSpacing = pAxis->GetGridSpacing();
	}

	pAxis->m_dGridSpacing = m_dGridSpacing;

	CreateTickMarks(pDC, rcDiagram);

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

void CXTPChartRadarAxisYView::CreateView(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}
