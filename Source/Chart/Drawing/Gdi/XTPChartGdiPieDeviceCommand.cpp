// XTPChartGdiPieDeviceCommand.cpp
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
#include "Chart/Types/XTPChartPie.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"
#include "Chart/Drawing/XTPChartPieDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiPieDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiPieDeviceCommand

CXTPChartGdiPieDeviceCommand::CXTPChartGdiPieDeviceCommand(const CXTPPoint3d& center,
														   double dMajorSemiAxis,
														   double dMinorSemiAxis,
														   double dStartAngle, double dSweepAngle,
														   double depth, double nHolePercent)
	: CXTPChartPieDeviceCommand(center, dMajorSemiAxis, dMinorSemiAxis, dStartAngle, dSweepAngle,
								depth, nHolePercent)
{
}

void CXTPChartGdiPieDeviceCommand::CalculateStartFinishPoints(
	const CXTPPoint3d& center, double majorSemiaxis, double minorSemiaxis, double dStartAngle,
	double dSweepAngle, CXTPChartPointF& startPoint, CXTPChartPointF& finishPoint) const
{
	CXTPChartEllipse ellipse(center, majorSemiaxis, minorSemiaxis);
	startPoint = (CXTPChartPointF)ellipse.CalcEllipsePoint(
		-CXTPMathUtils::Degree2Radian(dStartAngle));
	finishPoint = (CXTPChartPointF)ellipse.CalcEllipsePoint(
		-CXTPMathUtils::Degree2Radian(dStartAngle + dSweepAngle));
}

CXTPChartElement* CXTPChartGdiPieDeviceCommand::HitTest(CPoint point,
														CXTPChartElement* pParent) const
{
	GpPath* pGpPath = CreatePieGraphicsPath(m_ptCenter, m_dMajorSemiAxis, m_dMinorSemiAxis,
											m_dHolePercent, m_dStartAngle, m_dSweepAngle);

	BOOL bResult = FALSE;

	GdipIsVisiblePathPointI(pGpPath, point.x, point.y, NULL, &bResult);

	GdipDeletePath(pGpPath);

	return bResult ? pParent : NULL;
}

GpPath* CXTPChartGdiPieDeviceCommand::CreatePieGraphicsPath(const CXTPPoint3d& center,
															double majorSemiaxis,
															double minorSemiaxis,
															double holePercent, double dStartAngle,
															double dSweepAngle) const
{
	GpPath* pGpPath = NULL;
	GdipCreatePath(FillModeAlternate, &pGpPath);

	CRect rect(int(center.X - majorSemiaxis), int(center.Y - minorSemiaxis),
			   int(center.X + majorSemiaxis), int(center.Y + minorSemiaxis));

	BOOL bShouldAddLines	  = CXTPMathUtils::Compare(dSweepAngle, -360.0) != 0;
	float dInnerMajorSemiaxis = (float)(majorSemiaxis * holePercent);
	float dInnerMinorSemiaxis = (float)(minorSemiaxis * holePercent);
	if (dInnerMajorSemiaxis >= 1.0f && dInnerMinorSemiaxis >= 1.0f)
	{
		CRect innerRect(int(center.X - dInnerMajorSemiaxis), int(center.Y - dInnerMinorSemiaxis),
						int(center.X + dInnerMajorSemiaxis), int(center.Y + dInnerMinorSemiaxis));

		CXTPChartPointF startPoint, finishPoint, innerStartPoint, innerFinishPoint;

		if (bShouldAddLines)
		{
			CalculateStartFinishPoints(center, majorSemiaxis, minorSemiaxis, dStartAngle,
									   dSweepAngle, startPoint, finishPoint);
			CalculateStartFinishPoints(center, dInnerMajorSemiaxis, dInnerMinorSemiaxis,
									   dStartAngle, dSweepAngle, innerStartPoint, innerFinishPoint);
		}

		GdipAddPathArc(pGpPath, (REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(),
					   (REAL)rect.Height(), (REAL)dStartAngle, (REAL)dSweepAngle);

		if (bShouldAddLines)
		{
			GdipAddPathLine(pGpPath, finishPoint.X, finishPoint.Y, innerFinishPoint.X,
							innerFinishPoint.Y);
		}
		GdipAddPathArc(pGpPath, (REAL)innerRect.left, (REAL)innerRect.top, (REAL)innerRect.Width(),
					   (REAL)innerRect.Height(), (REAL)(dStartAngle + dSweepAngle),
					   (REAL)(-dSweepAngle));

		if (bShouldAddLines)
			GdipAddPathLine(pGpPath, innerStartPoint.X, innerStartPoint.Y, startPoint.X,
							startPoint.Y);
	}
	else if (bShouldAddLines)
	{
		GdipAddPathPie(pGpPath, (REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(),
					   (REAL)rect.Height(), (REAL)dStartAngle, (REAL)dSweepAngle);
	}
	else
	{
		GdipAddPathEllipse(pGpPath, (REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(),
						   (REAL)rect.Height());
	}

	return pGpPath;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiGradientPieDeviceCommand

CXTPChartGdiGradientPieDeviceCommand::CXTPChartGdiGradientPieDeviceCommand(
	const CXTPPoint3d& center, double dMajorSemiAxis, double dMinorSemiAxis, double dStartAngle,
	double dSweepAngle, double depth, double holePercent, const CXTPChartRectF& gradientBounds,
	const CXTPChartColor& color, const CXTPChartColor& color2)
	: CXTPChartGdiPieDeviceCommand(center, dMajorSemiAxis, dMinorSemiAxis, dStartAngle, dSweepAngle,
								   depth, holePercent)
{
	m_rcGradientBounds = gradientBounds;
	m_color			   = color;
	m_color2		   = color2;
}

GpBrush* CXTPChartGdiGradientPieDeviceCommand::CreateBrush()
{
	GpLineGradient* pGpBrush = NULL;

	PointF ptGradient[2];
	ptGradient[0] = PointF((REAL)m_rcGradientBounds.X, (REAL)m_ptCenter.Y);
	ptGradient[1] = PointF((REAL)(m_rcGradientBounds.X + m_rcGradientBounds.Width),
						   (REAL)m_ptCenter.Y);

	GdipCreateLineBrush(&ptGradient[0], &ptGradient[1], m_color.GetValue(), m_color2.GetValue(),
						WrapModeTile, &pGpBrush);

	return pGpBrush;
}

void CXTPChartGdiGradientPieDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpBrush* pGpBrush = CreateBrush();

	GpPath* pGpPath = CreatePieGraphicsPath(m_ptCenter, m_dMajorSemiAxis, m_dMinorSemiAxis,
											m_dHolePercent, m_dStartAngle, m_dSweepAngle);

	GdipFillPath(pGdiDC->GetGraphics(), pGpBrush, pGpPath);

	GdipDeletePath(pGpPath);

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiBoundedPieDeviceCommand

CXTPChartGdiBoundedPieDeviceCommand::CXTPChartGdiBoundedPieDeviceCommand(
	const CXTPPoint3d& center, double dMajorSemiAxis, double dMinorSemiAxis, double dStartAngle,
	double dSweepAngle, double depth, int holePercent, const CXTPChartColor& color, int nThickness)
	: CXTPChartGdiPieDeviceCommand(center, dMajorSemiAxis, dMinorSemiAxis, dStartAngle, dSweepAngle,
								   depth, holePercent)
{
	m_color		 = color;
	m_nThickness = nThickness;
}

void CXTPChartGdiBoundedPieDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);

	GpPath* pGpPath = CreatePieGraphicsPath(m_ptCenter, m_dMajorSemiAxis, m_dMinorSemiAxis,
											m_dHolePercent, m_dStartAngle, m_dSweepAngle);

	GdipDrawPath(pGdiDC->GetGraphics(), pGpPen, pGpPath);

	GdipDeletePath(pGpPath);

	GdipDeletePen(pGpPen);
}
