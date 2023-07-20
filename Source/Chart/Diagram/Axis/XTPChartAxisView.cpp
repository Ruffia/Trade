// XTPChartAxisView.cpp
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
#include "Common/Math/XTPMathUtils.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartContentView.h"
#include "Chart/XTPChartLegendView.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartAxisRange.h"
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
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"

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
static const int MIN_SPACE_BETWEEN_AXIS_AND_LABEL = 5;
//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisView

CXTPChartAxisView::CXTPChartAxisView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_dGridSpacing = 1;
	m_pAxis		   = pAxis;
}

CXTPChartAxisView::~CXTPChartAxisView()
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DAxisView

CXTPChartDiagram2DAxisView::CXTPChartDiagram2DAxisView(CXTPChartAxis* pAxis,
													   CXTPChartElementView* pParentView)
	: CXTPChartAxisView(pAxis, pParentView)
{
	ASSERT(m_pContainer);

	m_nSize = 0;

	m_ptOldPosition = CPoint(0, 0);
	m_rcBounds.SetRectEmpty();
}

CXTPChartDiagram2DAxisView::~CXTPChartDiagram2DAxisView()
{
}

double CXTPChartAxisView::ValueToAxis(double dValue) const
{
	if (!m_pAxis->IsLogarithmic())
		return dValue;

	if (fabs(dValue) < CXTPMathUtils::m_dEPS)
		return 0;

	if (dValue < 0)
		return 0;

	return log(dValue) / log(m_pAxis->GetLogarithmicBase());
}

double CXTPChartAxisView::AxisToValue(double dValue) const
{
	if (!m_pAxis->IsLogarithmic())
		return dValue;

	return pow(m_pAxis->GetLogarithmicBase(), dValue);
}

double CXTPChartAxisView::GetRangeMinValue() const
{
	return m_pAxis->GetRange()->GetMinValue();
}
double CXTPChartAxisView::GetRangeMaxValue() const
{
	return m_pAxis->GetRange()->GetMaxValue();
}
double CXTPChartAxisView::GetViewRangeMinValue() const
{
	return max(m_pAxis->GetRange()->GetViewMinValue(), GetRangeMinValue());
}
double CXTPChartAxisView::GetViewRangeMaxValue() const
{
	return min(m_pAxis->GetRange()->GetViewMaxValue(), GetRangeMaxValue());
}

double CXTPChartAxisView::GetAxisRangeMinValue() const
{
	return ValueToAxis(GetViewRangeMinValue());
}

double CXTPChartAxisView::GetAxisRangeMaxValue() const
{
	return ValueToAxis(GetViewRangeMaxValue());
}

CXTPChartDeviceCommand* CXTPChartDiagram2DAxisView::CreateInterlacedDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcPane)
{
	UNREFERENCED_PARAMETER(pDC);

	if (!m_pAxis->IsInterlaced())
		return NULL;

	if (m_arrTicks.GetSize() < 1)
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	BOOL bVertical = IsVertical();

	BOOL bReversed = m_pAxis->IsReversed();

	int nLeft = bVertical ? (bReversed ? m_rcBounds.top : m_rcBounds.bottom)
						  : (bReversed ? m_rcBounds.right : m_rcBounds.left);

	int i = 0;
	double dMark;

	double dGridSpacing = GetGridSpacing();

	int nCount = int((m_arrTicks[0].m_dValue - GetRangeMinValue()) / dGridSpacing);

	if ((nCount & 1) == 1)
	{
		dMark = m_arrTicks[0].m_dValue;
		nLeft = (int)ValueToPoint(dMark);

		i = 1;
	}

	while (i <= m_arrTicks.GetSize())
	{
		int nRight;

		if (i >= m_arrTicks.GetSize())
		{
			nRight = bVertical ? (!bReversed ? m_rcBounds.top : m_rcBounds.bottom)
							   : (!bReversed ? m_rcBounds.right : m_rcBounds.left);
		}
		else
		{
			dMark  = m_arrTicks[i].m_dValue;
			nRight = (int)ValueToPoint(dMark);
		}

		i++;

		CXTPChartRectF rc = bVertical ? CXTPChartRectF(static_cast<float>(rcPane.left),
													   static_cast<float>(nRight),
													   static_cast<float>(rcPane.Width()),
													   static_cast<float>(nLeft - nRight))
									  : CXTPChartRectF(static_cast<float>(nLeft),
													   static_cast<float>(rcPane.top),
													   static_cast<float>(nRight - nLeft),
													   static_cast<float>(rcPane.Height()));

		if (rc.Width < 0)
			rc.X += rc.Width, rc.Width = -rc.Width;
		if (rc.Height < 0)
			rc.Y += rc.Height, rc.Height = -rc.Height;

		pCommands->AddChildCommand(m_pAxis->GetInterlacedFillStyle()->CreateDeviceCommand(
			rc, m_pAxis->GetActualInterlacedColor(), m_pAxis->GetActualInterlacedColor2()));

		if (i >= m_arrTicks.GetSize())
			break;

		dMark = m_arrTicks[i].m_dValue;
		nLeft = (int)ValueToPoint(dMark);

		i++;
	}

	return pCommands;
}

CXTPChartDeviceCommand*
	CXTPChartDiagram2DAxisView::CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcPane)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartAxisStrips* pStrips = m_pAxis->GetStrips();
	if (pStrips->GetCount() == 0)
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();
	BOOL bVertical					  = IsVertical();

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

		int nLeft  = (int)ValueToPoint(dMarkLeft);
		int nRight = (int)ValueToPoint(dMarkRight);

		CXTPChartRectF rc;
		if (bVertical)
		{
			rc = CXTPChartRectF(static_cast<float>(rcPane.left), static_cast<float>(nRight),
								static_cast<float>(rcPane.Width()),
								static_cast<float>(nLeft - nRight));
		}
		else
		{
			rc = CXTPChartRectF(static_cast<float>(nLeft), static_cast<float>(rcPane.top),
								static_cast<float>(nRight - nLeft),
								static_cast<float>(rcPane.Height()));
		}

		rc.Normalize();

		pCommands->AddChildCommand(
			pStrip->GetFillStyle()->CreateDeviceCommand(rc, pStrip->GetActualColor(),
														pStrip->GetActualColor2()));
	}

	return pCommands;
}

CXTPChartDeviceCommand* CXTPChartDiagram2DAxisView::CreateConstantLinesDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcPane, BOOL bBehind)
{
	CXTPChartAxisConstantLines* pConstantLines = m_pAxis->GetConstantLines();
	if (pConstantLines->GetCount() == 0)
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();
	BOOL bVertical					  = IsVertical();

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

		int nLeft = (int)ValueToPoint(dMark);

		if (bVertical)
		{
			pCommands->AddChildCommand(pConstantLine->GetLineStyle()->CreateDeviceCommand(
				CXTPPoint3d(rcPane.left, nLeft), CXTPPoint3d(rcPane.right, nLeft),
				pConstantLine->GetActualColor()));
		}
		else
		{
			pCommands->AddChildCommand(pConstantLine->GetLineStyle()->CreateDeviceCommand(
				CXTPPoint3d(nLeft, rcPane.top), CXTPPoint3d(nLeft, rcPane.bottom),
				pConstantLine->GetActualColor()));
		}

		CXTPChartString strText = pConstantLine->GetText();

		if (bVertical)
		{
			CXTPChartTextPainter painter(pDC, strText, pConstantLine);
			int x = pConstantLine->GetAlignment() == xtpChartAlignNear
						? rcPane.left + 2
						: pConstantLine->GetAlignment() == xtpChartAlignFar
							  ? rcPane.right - 2 - (int)painter.GetSize().Width
							  : (rcPane.left + rcPane.right - (int)painter.GetSize().Width) / 2;

			painter.SetLocation(CXTPPoint2i(x, pConstantLine->IsTextBelow()
												   ? nLeft + 2
												   : nLeft - (int)painter.GetSize().Height));
			pCommands->AddChildCommand(
				painter.CreateDeviceCommand(pDC, pConstantLine->GetActualTextColor()));
		}
		else
		{
			CXTPChartRotatedTextPainterNearLine painter(pDC, strText, pConstantLine, CPoint(0, 0),
														TRUE ? xtpChartTextNearRight
															 : xtpChartTextNearTop,
														90);

			int y = pConstantLine->GetAlignment() == xtpChartAlignNear
						? rcPane.bottom - (int)painter.GetSize().Width / 2
						: pConstantLine->GetAlignment() == xtpChartAlignFar
							  ? rcPane.top + (int)painter.GetSize().Width / 2
							  : (rcPane.top + rcPane.bottom) / 2;

			painter.SetBasePoint(CPoint(pConstantLine->IsTextBelow()
											? nLeft + 2 - (int)painter.GetSize().Height
											: nLeft + 2,
										y));

			pCommands->AddChildCommand(
				painter.CreateDeviceCommand(pDC, pConstantLine->GetActualTextColor()));
		}
	}

	return pCommands;
}

CXTPChartDeviceCommand* CXTPChartDiagram2DAxisView::CreateGridLinesDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcPane)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartAxisGridLines* pGridLines = m_pAxis->GetGridLines();

	if (!pGridLines->IsVisible())
		return NULL;

	CXTPChartDeviceCommand* pCommands = new CXTPChartDeviceCommand();

	CXTPChartColor clrGridLines		 = pGridLines->GetColor();
	CXTPChartColor clrMinorGridLines = pGridLines->GetMinorColor();
	BOOL bMinorVisible				 = pGridLines->IsMinorVisible();

	BOOL bVertical = IsVertical();

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		int nLeft = (int)ValueToPoint(m_arrTicks[i].m_dValue);

		if (bVertical)
		{
			pCommands->AddChildCommand(pGridLines->GetLineStyle()->CreateDeviceCommand(
				CXTPPoint3d(rcPane.left, nLeft), CXTPPoint3d(rcPane.right, nLeft), clrGridLines));
		}
		else
		{
			pCommands->AddChildCommand(pGridLines->GetLineStyle()->CreateDeviceCommand(
				CXTPPoint3d(nLeft, rcPane.top), CXTPPoint3d(nLeft, rcPane.bottom), clrGridLines));
		}
	}

	if (bMinorVisible)
	{
		for (int i = 0; i < m_arrMinorTicks.GetSize(); i++)
		{
			int nLeft = (int)ValueToPoint(m_arrMinorTicks[i]);

			if (bVertical)
			{
				pCommands->AddChildCommand(pGridLines->GetMinorLineStyle()->CreateDeviceCommand(
					CXTPPoint3d(rcPane.left, nLeft), CXTPPoint3d(rcPane.right, nLeft),
					clrMinorGridLines));
			}
			else
			{
				pCommands->AddChildCommand(pGridLines->GetMinorLineStyle()->CreateDeviceCommand(
					CXTPPoint3d(nLeft, rcPane.top), CXTPPoint3d(nLeft, rcPane.bottom),
					clrMinorGridLines));
			}
		}
	}

	return pCommands;
}

double CXTPChartDiagram2DAxisView::GetScale() const
{
	double dMinValue = GetAxisRangeMinValue();
	double dMaxValue = GetAxisRangeMaxValue();

	if (IsVertical())
	{
		return (m_rcBounds.Height()) / (dMaxValue - dMinValue);
	}
	else
	{
		return (m_rcBounds.Width()) / (dMaxValue - dMinValue);
	}
}

double CXTPChartDiagram2DAxisView::DistanceToPoint(double x) const
{
	double dMinValue = GetAxisRangeMinValue();
	double dMaxValue = GetAxisRangeMaxValue();

	if (IsVertical())
	{
		return x / (dMaxValue - dMinValue) * (m_rcBounds.Height());
	}
	else
	{
		return x / (dMaxValue - dMinValue) * (m_rcBounds.Width());
	}
}

double CXTPChartDiagram2DAxisView::PointToValue(int nPoint) const
{
	double dMinValue = GetAxisRangeMinValue();
	double dMaxValue = GetAxisRangeMaxValue();

	BOOL bRevered = m_pAxis->IsReversed();

	double dValue;

	if (bRevered)
	{
		if (IsVertical())
		{
			dValue = dMinValue
					 + (double)(nPoint - m_rcBounds.top) * (dMaxValue - dMinValue)
						   / m_rcBounds.Height();
		}
		else
		{
			dValue = dMinValue
					 + (double)(m_rcBounds.right - nPoint) * (dMaxValue - dMinValue)
						   / m_rcBounds.Width();
		}
	}
	else
	{
		if (IsVertical())
		{
			dValue = dMinValue
					 + (double)(m_rcBounds.bottom - nPoint) * (dMaxValue - dMinValue)
						   / m_rcBounds.Height();
		}
		else
		{
			dValue = dMinValue
					 + (double)(nPoint - m_rcBounds.left) * (dMaxValue - dMinValue)
						   / m_rcBounds.Width();
		}
	}

	return AxisToValue(dValue);
}

double CXTPChartDiagram2DAxisView::ValueToPoint(double x) const
{
	double dMinValue = GetAxisRangeMinValue();
	double dMaxValue = GetAxisRangeMaxValue();

	x = ValueToAxis(x);

	BOOL bRevered = m_pAxis->IsReversed();

	double dValue;

	if (bRevered)
	{
		if (IsVertical())
		{
			dValue = m_rcBounds.top
					 + (x - dMinValue) / (dMaxValue - dMinValue) * (m_rcBounds.Height());
		}
		else
		{
			dValue = m_rcBounds.right
					 - (x - dMinValue) / (dMaxValue - dMinValue) * (m_rcBounds.Width());
		}
	}
	else
	{
		if (IsVertical())
		{
			dValue = m_rcBounds.bottom
					 - (x - dMinValue) / (dMaxValue - dMinValue) * (m_rcBounds.Height());
		}
		else
		{
			dValue = m_rcBounds.left
					 + (x - dMinValue) / (dMaxValue - dMinValue) * (m_rcBounds.Width());
		}
	}

	return CXTPMathUtils::Round(dValue);
}

CXTPChartDeviceCommand*
	CXTPChartDiagram2DAxisView::CreateTickMarksDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDeviceCommand* pCommands =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pAxis, m_rcBounds);

	CXTPChartAxisTickMarks* pTickMarks = m_pAxis->GetTickMarks();

	if (!pTickMarks->IsVisible())
		return pCommands;

	int nScrollBarSize = IsScollBarVisible() ? SCROLLBARSIZE : 0;

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();
	int nLength			   = pTickMarks->GetLength();
	int nThickness		   = pTickMarks->GetThickness();
	BOOL bMinorVisible	 = pTickMarks->IsMinorVisible();
	BOOL bVertical		   = IsVertical();
	BOOL bCross			   = pTickMarks->IsCrossAxis();
	int nAxisThickness	 = m_pAxis->GetThickness() - 1 + nScrollBarSize;
	int nExtraLength	   = bCross ? nLength + nAxisThickness : 0;

	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		int nLeft = (int)ValueToPoint(m_arrTicks[i].m_dValue);

		if (bVertical)
		{
			if (m_pAxis->GetAlignment() == xtpChartAxisNear)
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
						CXTPPoint3d(m_rcBounds.right - nAxisThickness - nLength, nLeft),
						CXTPPoint3d(m_rcBounds.right - nAxisThickness + nExtraLength, nLeft),
						clrAxis, nThickness));
			else
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
						CXTPPoint3d(m_rcBounds.left + nAxisThickness - nExtraLength, nLeft),
						CXTPPoint3d(m_rcBounds.left + nAxisThickness + nLength, nLeft), clrAxis,
						nThickness));
		}
		else
		{
			if (m_pAxis->GetAlignment() == xtpChartAxisNear)
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
						CXTPPoint3d(nLeft, m_rcBounds.top + nAxisThickness - nExtraLength),
						CXTPPoint3d(nLeft, m_rcBounds.top + nAxisThickness + nLength), clrAxis,
						nThickness));
			else
				pCommands->AddChildCommand(
					CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
						CXTPPoint3d(nLeft, m_rcBounds.bottom - nAxisThickness - nLength),
						CXTPPoint3d(nLeft, m_rcBounds.bottom - nAxisThickness + nExtraLength),
						clrAxis, nThickness));
		}
	}

	if (bMinorVisible)
	{
		nLength				= pTickMarks->GetMinorLength();
		int nExtra			= bCross ? nLength + nAxisThickness : 0;
		int nMinorThickness = pTickMarks->GetMinorThickness();

		for (int i = 0; i < m_arrMinorTicks.GetSize(); i++)
		{
			int nLeft = (int)ValueToPoint(m_arrMinorTicks[i]);

			if (bVertical)
			{
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
			else
			{
				if (m_pAxis->GetAlignment() == xtpChartAxisNear)
					pCommands->AddChildCommand(
						CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
							CXTPPoint3d(nLeft, m_rcBounds.top + nAxisThickness - nExtra),
							CXTPPoint3d(nLeft, m_rcBounds.top + nAxisThickness + nLength), clrAxis,
							nMinorThickness));
				else
					pCommands->AddChildCommand(
						CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
							CXTPPoint3d(nLeft, m_rcBounds.bottom - nAxisThickness - nLength),
							CXTPPoint3d(nLeft, m_rcBounds.bottom - nAxisThickness + nExtra),
							clrAxis, nMinorThickness));
			}
		}
	}

	return pCommands;
}

static BOOL DoLineSegmentsIntersect(float ax1, float ay1, float ax2, float ay2, float bx1,
									float by1, float bx2, float by2)
{
	float s1x = ax2 - ax1;
	float s1y = ay2 - ay1;
	float s2x = bx2 - bx1;
	float s2y = by2 - by1;
	float s   = (-s1y * (ax1 - bx1) + s1x * (ay1 - by1)) / (-s2x * s1y + s1x * s2y);
	float t   = (s2x * (ay1 - by1) - s2y * (ax1 - bx1)) / (-s2x * s1y + s1x * s2y);
	return (0 <= s && s <= 1 && 0 <= t && t <= 1);
}

static BOOL DoRectanglesIntersect(float r1[4][2], float r2[4][2])
{
	/*
	0:(0,1)      A      1:(0,1)
		+----------------+
		| D              | B
		+----------------+
	3:(0,1)      C      2:(0,1)
	*/

	return
		// r1:B x r2:A
		DoLineSegmentsIntersect(r1[1][0], r1[1][1], r1[2][0], r1[2][1], r2[0][0], r2[0][1],
								r2[1][0], r2[1][1])
		||
		// r1:B x r2:C
		DoLineSegmentsIntersect(r1[1][0], r1[1][1], r1[2][0], r1[2][1], r2[2][0], r2[2][1],
								r2[3][0], r2[3][1])
		||
		// r1:D x r2:A
		DoLineSegmentsIntersect(r1[0][0], r1[0][1], r1[3][0], r1[3][1], r2[0][0], r2[0][1],
								r2[1][0], r2[1][1])
		||
		// r1:D x r2:C
		DoLineSegmentsIntersect(r1[0][0], r1[0][1], r1[3][0], r1[3][1], r2[2][0], r2[2][1],
								r2[3][0], r2[3][1])

		||
		// r2:B x r1:A
		DoLineSegmentsIntersect(r2[1][0], r2[1][1], r2[2][0], r2[2][1], r1[0][0], r1[0][1],
								r1[1][0], r1[1][1])
		||
		// r2:B x r1:C
		DoLineSegmentsIntersect(r2[1][0], r2[1][1], r2[2][0], r2[2][1], r1[2][0], r1[2][1],
								r1[3][0], r1[3][1])
		||
		// r2:D x r1:A
		DoLineSegmentsIntersect(r2[0][0], r2[0][1], r2[3][0], r2[3][1], r1[0][0], r1[0][1],
								r1[1][0], r1[1][1])
		||
		// r2:D x r1:C
		DoLineSegmentsIntersect(r2[0][0], r2[0][1], r2[3][0], r2[3][1], r1[2][0], r1[2][1],
								r1[3][0], r1[3][1]);
}

CXTPChartDeviceCommand*
	CXTPChartDiagram2DAxisView::CreateLabelsDeviceCommand(CXTPChartDeviceContext* pDC)
{
	if (!m_pAxis->GetLabel()->IsVisible())
		return NULL;

	CXTPChartDeviceCommand* pLabelsCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pAxis->GetLabel(),
																	 m_rcBounds);
	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	BOOL bVertical = IsVertical();
	int nAngle	 = m_pAxis->GetLabel()->GetAngle();
	BOOL bNear	 = m_pAxis->GetAlignment() == xtpChartAxisNear;

	int nOffset = m_pAxis->GetThickness()
				  + (m_pAxis->GetTickMarks()->IsVisible()
						 ? (m_pAxis->GetTickMarks()->GetLength() > 0
								? m_pAxis->GetTickMarks()->GetLength()
								: MIN_SPACE_BETWEEN_AXIS_AND_LABEL)
						 : 0)
				  + (IsScollBarVisible() ? SCROLLBARSIZE : 0);

	float prevLabelPoints[4][2];
	for (int i = 0; i < m_arrTicks.GetSize(); i++)
	{
		int nLeft		  = (int)ValueToPoint(m_arrTicks[i].m_dValue);
		CXTPChartString s = m_arrTicks[i].m_strLabel;

		CPoint ptBase;
		XTPChartNearTextPosition position;
		if (bVertical)
		{
			ptBase.x = bNear ? m_rcBounds.right - nOffset : m_rcBounds.left + nOffset;
			ptBase.y = nLeft;
			position = bNear ? xtpChartTextNearLeft : xtpChartTextNearRight;
		}
		else
		{
			ptBase.x = nLeft;
			ptBase.y = bNear ? m_rcBounds.top + nOffset : m_rcBounds.bottom - nOffset;
			position = bNear ? xtpChartTextNearBottom : xtpChartTextNearTop;
		}

		CXTPChartRotatedTextPainterNearLine painter(pDC, s, m_pAxis->GetLabel(), ptBase, position,
													(float)nAngle);

		CRect rcText = painter.GetInitialTextRect();
		rcText.DeflateRect(2, 2, 2, 2);

		float labelPoints[4][2];
		painter.CalculatePoints(painter.CalculateRotation(), rcText, (float)nAngle, labelPoints);

		BOOL bOverlaps = (0 < i ? DoRectanglesIntersect(labelPoints, prevLabelPoints) : FALSE);
		if (!bOverlaps)
		{
			pLabelsCommand->AddChildCommand(
				painter.CreateDeviceCommand(pDC, m_pAxis->GetLabel()->GetActualTextColor()));
			memcpy(prevLabelPoints, labelPoints, sizeof(prevLabelPoints));
		}
	}

	return pLabelsCommand;
}

void CXTPChartDiagram2DAxisView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPChartContainer* pContainer = m_pContainer;
	ASSERT(pContainer);
	UNUSED(pContainer);

	if (static_cast<LONG>(m_rcScrollBar.GetLeft()) > point.x
		|| static_cast<LONG>(m_rcScrollBar.GetRight()) < point.x)
		return;
	if (static_cast<int>(m_rcScrollBar.GetTop()) > point.y
		|| static_cast<int>(m_rcScrollBar.GetBottom()) < point.y)
		return;

	m_pContainer->SetCapture(this);
	m_ptOldPosition = point;

	if (IsVertical())
	{
		double top	= m_rcThumb.GetTop();
		double bottom = m_rcThumb.GetBottom();

		if (point.y < top || point.y > bottom)
		{
			double dy = !m_pAxis->IsReversed()
							? (static_cast<int>(m_rcScrollBar.GetBottom()) - point.y)
							: (point.y - static_cast<int>(m_rcScrollBar.GetTop()));

			double pos = dy * (ValueToAxis(GetRangeMaxValue()) - ValueToAxis(GetRangeMinValue()))
							 / m_rcScrollBar.Height
						 + ValueToAxis(GetRangeMinValue());

			double delta = -(GetAxisRangeMinValue()
							 - (pos - (GetAxisRangeMaxValue() - GetAxisRangeMinValue()) / 2));

			if (GetAxisRangeMaxValue() + delta > ValueToAxis(GetRangeMaxValue()))
				delta = ValueToAxis(GetRangeMaxValue()) - GetAxisRangeMaxValue();

			if (GetAxisRangeMinValue() + delta < ValueToAxis(GetRangeMinValue()))
				delta = ValueToAxis(GetRangeMinValue()) - GetAxisRangeMinValue();

			m_pAxis->GetRange()->SetViewMaxValue(AxisToValue(GetAxisRangeMaxValue() + delta));
			m_pAxis->GetRange()->SetViewMinValue(AxisToValue(GetAxisRangeMinValue() + delta));
		}
	}
	else
	{
		double left  = m_rcThumb.GetLeft();
		double right = m_rcThumb.GetRight();

		if (point.x < left || point.x > right)
		{
			double dx = !m_pAxis->IsReversed()
							? (point.x - static_cast<LONG>(m_rcScrollBar.GetLeft()))
							: (static_cast<LONG>(m_rcScrollBar.GetRight()) - point.x);

			double pos = dx * (ValueToAxis(GetRangeMaxValue()) - ValueToAxis(GetRangeMinValue()))
							 / m_rcScrollBar.Width
						 + ValueToAxis(GetRangeMinValue());

			double delta = -(GetAxisRangeMinValue()
							 - (pos - (GetAxisRangeMaxValue() - GetAxisRangeMinValue()) / 2));

			if (GetAxisRangeMaxValue() + delta > ValueToAxis(GetRangeMaxValue()))
				delta = ValueToAxis(GetRangeMaxValue()) - GetAxisRangeMaxValue();

			if (GetAxisRangeMinValue() + delta < ValueToAxis(GetRangeMinValue()))
				delta = ValueToAxis(GetRangeMinValue()) - GetAxisRangeMinValue();

			m_pAxis->GetRange()->SetViewMaxValue(AxisToValue(GetAxisRangeMaxValue() + delta));
			m_pAxis->GetRange()->SetViewMinValue(AxisToValue(GetAxisRangeMinValue() + delta));
		}
	}
}

void CXTPChartDiagram2DAxisView::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	int dx			= point.x - m_ptOldPosition.x;
	int dy			= point.y - m_ptOldPosition.y;
	m_ptOldPosition = point;

	CXTPChartContainer* pContainer = m_pContainer;
	ASSERT(pContainer);
	UNUSED(pContainer);

	m_pAxis->GetRange()->SetViewAutoRange(FALSE);

	double dMinValue = ValueToAxis(GetRangeMinValue());
	double dMaxValue = ValueToAxis(GetRangeMaxValue());
	double dScale	= 0;

	if (IsVertical())
	{
		dScale = (m_rcBounds.Height()) / (dMaxValue - dMinValue);
	}
	else
	{
		dScale = (m_rcBounds.Width()) / (dMaxValue - dMinValue);
	}

	double delta = 1.0 / dScale;
	if (IsVertical())
		delta *= -dy;
	else
		delta *= dx;

	if (m_pAxis->IsReversed())
		delta *= -1;

	if (GetAxisRangeMaxValue() + delta > dMaxValue)
		delta = dMaxValue - GetAxisRangeMaxValue();

	if (GetAxisRangeMinValue() + delta < dMinValue)
		delta = dMinValue - GetAxisRangeMinValue();

	m_pAxis->GetRange()->SetViewMaxValue(AxisToValue(GetAxisRangeMaxValue() + delta));
	m_pAxis->GetRange()->SetViewMinValue(AxisToValue(GetAxisRangeMinValue() + delta));
}

void CXTPChartDiagram2DAxisView::PerformPaneDragging(int dx, int dy)
{
	CXTPChartContainer* pContainer = m_pContainer;
	ASSERT(pContainer);
	UNUSED(pContainer);

	m_pAxis->GetRange()->SetViewAutoRange(FALSE);

	double delta = 1.0 / GetScale();
	if (IsVertical())
		delta *= dy;
	else
		delta *= -dx;

	if (m_pAxis->IsReversed())
		delta *= -1;

	double dMinValue = ValueToAxis(GetRangeMinValue());
	double dMaxValue = ValueToAxis(GetRangeMaxValue());

	if (GetAxisRangeMaxValue() + delta > dMaxValue)
		delta = dMaxValue - GetAxisRangeMaxValue();

	if (GetAxisRangeMinValue() + delta < dMinValue)
		delta = dMinValue - GetAxisRangeMinValue();

	m_pAxis->GetRange()->SetViewMaxValue(AxisToValue(GetAxisRangeMaxValue() + delta));
	m_pAxis->GetRange()->SetViewMinValue(AxisToValue(GetAxisRangeMinValue() + delta));
}

void CXTPChartDiagram2DAxisView::PerformMouseWheel(short zDelta, CPoint /*pt*/)
{
	CXTPChartContainer* pContainer = m_pContainer;
	ASSERT(pContainer);
	UNUSED(pContainer);

	if (!m_pAxis->IsAllowZoom())
		return;

	CXTPChartAxisRange* pRange = m_pAxis->GetRange();

	pRange->SetViewAutoRange(FALSE);

	double dRange = (GetAxisRangeMaxValue() - GetAxisRangeMinValue());
	double delta  = dRange / 10;

	if (zDelta >= 0)
		delta = -delta;

	if (dRange - 2 * delta < pRange->GetZoomLimit())
	{
		delta = (dRange - pRange->GetZoomLimit()) / 2;
	}

	pRange->SetViewMaxValue(min(GetRangeMaxValue(), AxisToValue(GetAxisRangeMaxValue() - delta)));
	pRange->SetViewMinValue(max(GetRangeMinValue(), AxisToValue(GetAxisRangeMinValue() + delta)));
}

CXTPChartDeviceCommand*
	CXTPChartDiagram2DAxisView::CreateScrollBarDeviceCommand(CXTPChartDeviceContext* /*pDC*/)
{
	m_rcScrollBar = CXTPChartRectF(0, 0, 0, 0);

	if (!IsScollBarVisible())
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CRect rcBounds = m_rcBounds;
	int nSize	  = SCROLLBARSIZE;

	if (IsVertical())
	{
		if (m_pAxis->GetAlignment() == xtpChartAxisNear)
		{
			rcBounds.left = rcBounds.right - nSize;
		}
		else
		{
			rcBounds.right = rcBounds.left + nSize;
		}
	}
	else
	{
		if (m_pAxis->GetAlignment() == xtpChartAxisNear)
		{
			rcBounds.bottom = rcBounds.top + nSize;
		}
		else
		{
			rcBounds.top = rcBounds.bottom - nSize;
		}
	}

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dSolidRectangleDeviceCommand(rcBounds,
																		   CXTPChartColor::White));

	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dBoundedRectangleDeviceCommand(rcBounds, clrAxis, 1));

	CXTPChartRectF rcScrollBar(rcBounds);

	rcScrollBar.DeflateRect(1.5f, 1.5f, 1.5f, 1.5f);

	double dRangeMaxValue = ValueToAxis(GetRangeMaxValue());
	double dRangeMinValue = ValueToAxis(GetRangeMinValue());

	double minValue = (GetAxisRangeMinValue() - dRangeMinValue) / (dRangeMaxValue - dRangeMinValue);

	double maxValue = (GetAxisRangeMaxValue() - dRangeMinValue) / (dRangeMaxValue - dRangeMinValue);

	const int MIN_BUTTON_SIZE = 15;

	CXTPChartRectF rcThumb(rcScrollBar);
	if (IsVertical())
	{
		double top, bottom;
		if (!m_pAxis->IsReversed())
		{
			bottom = rcThumb.GetBottom() - rcThumb.Height * minValue;
			top	= rcThumb.GetBottom() - rcThumb.Height * maxValue;
		}
		else
		{
			top	= rcThumb.GetTop() + rcThumb.Height * minValue;
			bottom = rcThumb.GetTop() + rcThumb.Height * maxValue;
		}

		if (bottom - top < MIN_BUTTON_SIZE)
		{
			bottom = top + MIN_BUTTON_SIZE;
			if (bottom > rcScrollBar.GetBottom())
			{
				bottom = rcScrollBar.GetBottom();
				top	= bottom - MIN_BUTTON_SIZE;
			}
			if (top < rcScrollBar.GetTop())
				top = rcScrollBar.GetTop();
		}

		rcThumb.Y	  = (float)top;
		rcThumb.Height = (float)(bottom - top);
	}
	else
	{
		double left, right;
		if (!m_pAxis->IsReversed())
		{
			left  = rcThumb.GetLeft() + rcThumb.Width * minValue;
			right = rcThumb.GetLeft() + rcThumb.Width * maxValue;
		}
		else
		{
			right = rcThumb.GetRight() - rcThumb.Width * minValue;
			left  = rcThumb.GetRight() - rcThumb.Width * maxValue;
		}

		if (right - left < MIN_BUTTON_SIZE)
		{
			right = left + MIN_BUTTON_SIZE;
			if (right > rcScrollBar.GetRight())
			{
				right = rcScrollBar.GetRight();
				left  = right - MIN_BUTTON_SIZE;
			}
			if (left < rcScrollBar.GetLeft())
				left = rcScrollBar.GetLeft();
		}

		rcThumb.X	 = (float)left;
		rcThumb.Width = (float)(right - left);
	}

	m_rcScrollBar = rcScrollBar;
	m_rcThumb	 = rcThumb;

	pCommand->AddChildCommand(
		CXTPChartDrawingObjectFactory::Create2dSolidRectangleDeviceCommand(rcThumb, clrAxis));

	if (IsVertical())
	{
		if (rcThumb.Height > 10)
		{
			CXTPChartPointF pt = rcThumb.GetCenter();

			pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
				CXTPPoint3d(pt.X - 1, pt.Y), CXTPPoint3d(pt.X + 1, pt.Y), CXTPChartColor::White,
				1));
			pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
				CXTPPoint3d(pt.X - 1, pt.Y - 2), CXTPPoint3d(pt.X + 1, pt.Y - 2),
				CXTPChartColor::White, 1));
			pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
				CXTPPoint3d(pt.X - 1, pt.Y + 2), CXTPPoint3d(pt.X + 1, pt.Y + 2),
				CXTPChartColor::White, 1));
		}
	}
	else
	{
		if (rcThumb.Width > 10)
		{
			CXTPChartPointF pt = rcThumb.GetCenter();

			pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
				CXTPPoint3d(pt.X, pt.Y - 1), CXTPPoint3d(pt.X, pt.Y + 1), CXTPChartColor::White,
				1));
			pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
				CXTPPoint3d(pt.X - 2, pt.Y - 1), CXTPPoint3d(pt.X - 2, pt.Y + 1),
				CXTPChartColor::White, 1));
			pCommand->AddChildCommand(CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
				CXTPPoint3d(pt.X + 2, pt.Y - 1), CXTPPoint3d(pt.X + 2, pt.Y + 1),
				CXTPChartColor::White, 1));
		}
	}

	return pCommand;
}

CXTPChartDeviceCommand*
	CXTPChartDiagram2DAxisView::CreateTitleDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartAxisTitle* pTitle = GetAxis()->GetTitle();
	if (!pTitle->IsVisible())
		return NULL;

	CXTPChartString strText = pTitle->GetText();

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(pTitle, m_rcBounds);

	CXTPChartDeviceCommand* pChildCommand = NULL;

	if (IsVertical())
	{
		if (m_pAxis->GetAlignment() == xtpChartAxisNear)
		{
			if (pTitle->GetAlignment() == xtpChartAlignCenter)
			{
				CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
															CPoint(m_rcBounds.left,
																   m_rcBounds.CenterPoint().y),
															xtpChartTextNearRight, 270);

				pChildCommand = painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor());
			}
			else
			{
				CPoint basePoint = CPoint(m_rcBounds.left,
										  pTitle->GetAlignment() == xtpChartAlignFar
											  ? m_rcBounds.bottom
											  : m_rcBounds.top);

				CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
															basePoint,
															pTitle->GetAlignment()
																	== xtpChartAlignNear
																? xtpChartTextNearBottom
																: xtpChartTextNearTop,
															270);

				// we use Width because CXTPChartTextPainterBase doesn't take into account rotation
				// angle
				basePoint.x += LONG(painter.GetSize().Height / 2);

				painter.SetBasePoint(basePoint);

				pChildCommand = painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor());
			}
		}
		else
		{
			CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
														CPoint(m_rcBounds.right,
															   m_rcBounds.CenterPoint().y),
														xtpChartTextNearLeft, 90);

			pChildCommand = painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor());
		}
	}
	else
	{
		if (m_pAxis->GetAlignment() == xtpChartAxisNear)
		{
			if (pTitle->GetAlignment() == xtpChartAlignCenter)
			{
				CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
															CPoint(m_rcBounds.CenterPoint().x,
																   m_rcBounds.bottom + 3),
															xtpChartTextNearTop, 0);

				pChildCommand = painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor());
			}
			else
			{
				CXTPChartTextPainter painter(pDC, pTitle->GetText(), pTitle);
				painter.SetLocation(CXTPChartPointF(
					pTitle->GetAlignment() == xtpChartAlignNear
						? static_cast<int>(m_rcBounds.left)
						: static_cast<int>(m_rcBounds.right
										   - static_cast<LONG>(painter.GetSize().Width)),
					static_cast<int>(m_rcBounds.bottom + 3
									 - static_cast<LONG>(painter.GetSize().Height))));

				pChildCommand = painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor());
			}
		}
		else
		{
			CXTPChartRotatedTextPainterNearLine painter(pDC, pTitle->GetText(), pTitle,
														CPoint(m_rcBounds.CenterPoint().x,
															   m_rcBounds.top),
														xtpChartTextNearBottom, 0);

			pChildCommand = painter.CreateDeviceCommand(pDC, pTitle->GetActualTextColor());
		}
	}

	ASSERT(pChildCommand);

	if (pChildCommand)
	{
		pCommand->AddChildCommand(pChildCommand);
	}

	return pCommand;
}

BOOL CXTPChartDiagram2DAxisView::IsScollBarVisible() const
{
	if (m_pAxis->GetRange()->IsViewAutoRange())
		return FALSE;

	if (fabs(GetViewRangeMinValue() - GetRangeMinValue()) < CXTPMathUtils::m_dEPS
		&& fabs(GetViewRangeMaxValue() - GetRangeMaxValue()) < CXTPMathUtils::m_dEPS)
		return FALSE;

	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, m_pAxis->GetDiagram());
	if (!pDiagram)
		return FALSE;

	return pDiagram->IsAllowScroll();
}

CXTPChartDeviceCommand* CXTPChartDiagram2DAxisView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pAxisCommand = new CXTPChartDeviceCommand();

	CXTPChartColor clrAxis = m_pAxis->GetActualColor();

	int nScrollBarSize = IsScollBarVisible() ? SCROLLBARSIZE : 0;
	int nThickness	 = m_pAxis->GetThickness();

	if (m_pAxis->IsVisible())
	{
		if (IsVertical())
		{
			float fPosition = m_pAxis->GetAlignment() == xtpChartAxisNear
								  ? static_cast<float>(m_rcBounds.right)
										- static_cast<float>(nScrollBarSize)
										- static_cast<float>(nThickness) / 2.0f + 0.5f
								  : static_cast<float>(m_rcBounds.left)
										+ static_cast<float>(nScrollBarSize)
										+ static_cast<float>(nThickness) / 2.0f - 0.5f;

			pAxisCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
					CXTPPoint3d(fPosition, static_cast<float>(m_rcBounds.top)),
					CXTPPoint3d(fPosition, static_cast<float>(m_rcBounds.bottom) + 0.5f), clrAxis,
					m_pAxis->GetThickness()));
		}
		else
		{
			float fPosition = m_pAxis->GetAlignment() == xtpChartAxisNear
								  ? static_cast<float>(m_rcBounds.top)
										+ static_cast<float>(nScrollBarSize)
										+ static_cast<float>(nThickness) / 2.0f - 0.5f
								  : static_cast<float>(m_rcBounds.bottom)
										- static_cast<float>(nThickness) / 2.0f + 0.5f
										- static_cast<float>(nScrollBarSize);

			pAxisCommand->AddChildCommand(
				CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
					CXTPPoint3d(m_rcBounds.left, fPosition),
					CXTPPoint3d(static_cast<float>(m_rcBounds.right) + 0.5f, fPosition), clrAxis,
					nThickness));
		}

		pAxisCommand->AddChildCommand(CreateTickMarksDeviceCommand(pDC));
		pAxisCommand->AddChildCommand(CreateLabelsDeviceCommand(pDC));
		pAxisCommand->AddChildCommand(CreateTitleDeviceCommand(pDC));

		pAxisCommand->AddChildCommand(CreateScrollBarDeviceCommand(pDC));
	}

	return pAxisCommand;
}

double CXTPChartAxisView::CalculateGridSpacing(double nAxisRangeDelta, double nScreenDelta,
											   double nGridSpacingFactor)
{
	if (m_pAxis->GetScaleType() == xtpChartScaleQualitative)
		return 1.0;

	double multipliers[] = { 1, 2, 3, 5 };

	if (nScreenDelta <= 0)
		return 1;

	double deltaCoef = nGridSpacingFactor * nAxisRangeDelta / nScreenDelta;

	if (deltaCoef < CXTPMathUtils::m_dEPS)
		return 1;

	if (deltaCoef > 1.0)
	{
		for (double factor = 1;; factor *= 10)
		{
			for (int i = 0; i < 4; i++)
			{
				double result = multipliers[i] * factor;
				if (deltaCoef <= result)
					return result;
			}
		}
	}
	else
	{
		double result = 1;

		if (m_pAxis->GetScaleType() == xtpChartScaleDateTime)
			return result;

		for (double factor = 0.1;; factor /= 10)
		{
			for (int i = 4 - 1; i >= 0; i--)
			{
				double newResult = multipliers[i] * factor;
				if (deltaCoef > newResult)
					return result;
				result = newResult;
			}
		}
	}
}

void CXTPChartDiagram2DAxisView::SetBounds(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	m_rcBounds = rcBounds;
}

void CXTPChartDiagram2DAxisView::CreateTickMarks(CXTPChartDeviceContext* pDC, CRect rcDiagram)
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

	BOOL bVertical = IsVertical();
	BOOL bNear	 = m_pAxis->GetAlignment() == xtpChartAxisNear;
	int nAngle	 = m_pAxis->GetLabel()->GetAngle();

	if (m_pAxis->GetCustomLabels()->GetCount() > 0)
	{
		CXTPChartAxisCustomLabels* pCustomLabels = m_pAxis->GetCustomLabels();
		int nCount								 = pCustomLabels->GetCount();

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartAxisCustomLabel* pLabel = pCustomLabels->GetAt(i);

			CXTPChartAxisViewTick tick;
			tick.m_dValue = !pLabel->GetAxisValue().IsEmpty()
								? m_pAxis->GetScaleTypeMap()->ValueToInternal(
									  pLabel->GetAxisValue())
								: pLabel->GetAxisValueInternal();

			if (tick.m_dValue >= dViewMinValue && tick.m_dValue <= dViewMaxValue)
			{
				tick.m_strLabel = pLabel->GetText();

				CXTPChartRotatedTextPainterNearLine painter(
					pDC, tick.m_strLabel, m_pAxis->GetLabel(), CPoint(0, 0),
					bVertical ? (bNear ? xtpChartTextNearLeft : xtpChartTextNearRight)
							  : (bNear ? xtpChartTextNearBottom : xtpChartTextNearTop),
					(float)nAngle);

				tick.m_szLabel = painter.GetSize();

				tick.m_szBounds = painter.GetRoundedBounds().Size();

				m_arrTicks.Add(tick);
			}
		}
	}
	else
	{
		if (dAxisMinValue < -CXTPMathUtils::m_dEPS)
			dAxisMinValue += -CXTPMathUtils::m_dEPS;

		double dMark = floor(dAxisMinValue / dGridSpacing) * dGridSpacing;

		if (dMark < dAxisMinValue)
			dMark += dGridSpacing;

		while (dMark < dAxisMaxValue + CXTPMathUtils::m_dEPS)
		{
			CXTPChartAxisViewTick tick;
			tick.m_dValue = AxisToValue(dMark);

			tick.m_strLabel = m_pAxis->GetScaleTypeMap()->InternalToValue(
				m_pAxis->GetLabel()->GetFormat(), tick.m_dValue);

			CXTPChartRotatedTextPainterNearLine painter(
				pDC, tick.m_strLabel, m_pAxis->GetLabel(), CPoint(0, 0),
				bVertical ? (bNear ? xtpChartTextNearLeft : xtpChartTextNearRight)
						  : (bNear ? xtpChartTextNearBottom : xtpChartTextNearTop),
				(float)nAngle);

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

void CXTPChartDiagram2DAxisView::CalcSize(CXTPChartDeviceContext* pDC, CRect rcDiagram)
{
	CXTPChartAxis* pAxis = GetAxis();

	double dScreenLength = IsVertical() ? rcDiagram.Height() : rcDiagram.Width();

	if (dScreenLength < 1)
	{
		m_nSize = 0;
		return;
	}

	if (pAxis->GetGridSpacingAuto())
	{
		double dRangeDelta = GetAxisRangeMaxValue() - GetAxisRangeMinValue();

		m_dGridSpacing = CalculateGridSpacing(dRangeDelta, dScreenLength, IsVertical() ? 30 : 50);

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

	BOOL bVertical = IsVertical();

	m_nSize = pAxis->GetThickness();

	if (pAxis->GetTickMarks()->IsVisible())
	{
		m_nSize += (pAxis->GetTickMarks()->GetLength() > 0) ? pAxis->GetTickMarks()->GetLength()
															: MIN_SPACE_BETWEEN_AXIS_AND_LABEL;
	}

	if (IsScollBarVisible())
	{
		m_nSize += SCROLLBARSIZE;
	}

	if (pAxis->GetLabel()->IsVisible())
	{
		int nLabelSize = 0;

		for (int i = 0; i < m_arrTicks.GetSize(); i++)
		{
			CSize szBounds = m_arrTicks[i].m_szBounds;

			nLabelSize = max(nLabelSize, bVertical ? szBounds.cx : szBounds.cy);
		}

		m_nSize += nLabelSize;
	}

	if (pAxis->GetTitle()->IsVisible())
	{
		CXTPChartAxisTitle* pTitle = GetAxis()->GetTitle();

		CXTPChartString strText = pTitle->GetText();

		BOOL bNear = m_pAxis->GetAlignment() == xtpChartAxisNear;

		CXTPChartRotatedTextPainterNearLine painter(
			pDC, pTitle->GetText(), pTitle, CPoint(0, 0),
			IsVertical() ? (bNear ? xtpChartTextNearRight : xtpChartTextNearLeft)
						 : (bNear ? xtpChartTextNearTop : xtpChartTextNearBottom),
			(float)(IsVertical() ? (bNear ? 270 : 90) : 0));

		CSize szTitle = painter.GetRoundedBounds().Size();

		m_nSize += bVertical ? szTitle.cx : szTitle.cy;
	}
}

BOOL CXTPChartDiagram2DAxisView::IsVertical() const
{
	return m_pAxis->IsVertical();
}

void CXTPChartDiagram2DAxisView::CreateView(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartAxisView::AddLegendItems()
{
	CXTPChartElementView* pParentView = m_pParentView;
	while (pParentView->GetParentView() != NULL)
		pParentView = pParentView->GetParentView();

	CXTPChartContentView* pContentView = (CXTPChartContentView*)pParentView;

	if (pContentView->GetLegendView())
	{
		CXTPChartAxisConstantLines* pConstantLines = m_pAxis->GetConstantLines();

		int i;

		for (i = 0; i < pConstantLines->GetCount(); i++)
		{
			CXTPChartAxisConstantLine* pConstantLine = pConstantLines->GetAt(i);
			if (!pConstantLine->IsVisible() || !pConstantLine->IsLegendVisible())
				continue;

			pContentView->GetLegendView()->AddItem(pConstantLine);
		}

		CXTPChartAxisStrips* pStrips = m_pAxis->GetStrips();

		for (i = 0; i < pStrips->GetCount(); i++)
		{
			CXTPChartAxisStrip* pStrip = pStrips->GetAt(i);
			if (!pStrip->IsVisible() || !pStrip->IsLegendVisible())
				continue;

			pContentView->GetLegendView()->AddItem(pStrip);
		}
	}
}
