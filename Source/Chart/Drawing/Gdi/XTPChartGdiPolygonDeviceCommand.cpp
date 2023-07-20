// XTPChartGdiPolygonDeviceCommand.cpp
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
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiPolygonDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiPolygonDeviceCommand

CXTPChartGdiPolygonDeviceCommand::CXTPChartGdiPolygonDeviceCommand(const CXTPChartPoints& points)
	: CXTPChartPolygonDeviceCommand(points)
{
}

CXTPChartElement* CXTPChartGdiPolygonDeviceCommand::HitTest(CPoint point,
															CXTPChartElement* pParent) const
{
	if (point.x < static_cast<LONG>(m_bounds.GetLeft())
		|| point.x > static_cast<LONG>(m_bounds.GetRight()))
		return NULL;

	if (point.y < static_cast<int>(m_bounds.GetTop())
		|| point.y > static_cast<int>(m_bounds.GetBottom()))
		return NULL;

	GpPath* pGpPath;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipAddPathPolygon(pGpPath, (PointF*)m_points.GetData(), (int)m_points.GetSize());

	BOOL bResult = FALSE;
	GdipIsVisiblePathPointI(pGpPath, point.x, point.y, 0, &bResult);

	GdipDeletePath(pGpPath);

	if (bResult)
		return pParent;

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidPolygonDeviceCommand

CXTPChartGdiSolidPolygonDeviceCommand::CXTPChartGdiSolidPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color)
	: CXTPChartGdiPolygonDeviceCommand(points)
{
	m_color = color;
}

CXTPChartGdiSolidPolygonDeviceCommand::~CXTPChartGdiSolidPolygonDeviceCommand()
{
}

void CXTPChartGdiSolidPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GdipFillPolygon(pGdiDC->GetGraphics(), pGpBrush, (PointF*)m_points.GetData(),
					(int)m_points.GetSize(), FillModeAlternate);

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiGradientPolygonDeviceCommand

CXTPChartGdiGradientPolygonDeviceCommand::CXTPChartGdiGradientPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2,
	XTPChartLinearGradientMode nMode)
	: CXTPChartGdiPolygonDeviceCommand(points)
{
	m_color  = color;
	m_color2 = color2;
	m_nMode  = nMode;
}

void CXTPChartGdiGradientPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpLineGradient* pGpBrush = NULL;

	if (m_nMode == xtpChartLinearGradientModeCenterHorizontal)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_bounds, m_color.GetValue(), m_color.GetValue(),
									LinearGradientModeHorizontal, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = { m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(),
								  m_color.GetValue() };
		REAL positions[]	  = { 0, 0.45f, 0.55f, 1 };
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else if (m_nMode == xtpChartLinearGradientModeCenterVertical)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_bounds, m_color.GetValue(), m_color.GetValue(),
									LinearGradientModeVertical, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = { m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(),
								  m_color.GetValue() };
		REAL positions[]	  = { 0, 0.45f, 0.55f, 1 };
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else
	{
		GdipCreateLineBrushFromRect((RectF*)&m_bounds, m_color.GetValue(), m_color2.GetValue(),
									(LinearGradientMode)m_nMode, WrapModeTileFlipXY, &pGpBrush);
	}

	GdipFillPolygon(pGdiDC->GetGraphics(), pGpBrush, (PointF*)m_points.GetData(),
					(int)m_points.GetSize(), FillModeAlternate);
	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiHatchPolygonDeviceCommand

CXTPChartGdiHatchPolygonDeviceCommand::CXTPChartGdiHatchPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2,
	XTPChartHatchStyle nStyle)
	: CXTPChartGdiPolygonDeviceCommand(points)
{
	m_color  = color;
	m_color2 = color2;
	m_nStyle = nStyle;
}

void CXTPChartGdiHatchPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpHatch* pGpBrush = NULL;
	GdipCreateHatchBrush((GpHatchStyle)m_nStyle, m_color.GetValue(), m_color2.GetValue(),
						 &pGpBrush);
	GdipFillPolygon(pGdiDC->GetGraphics(), pGpBrush, (PointF*)m_points.GetData(),
					(int)m_points.GetSize(), FillModeAlternate);
	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidSplinePolygonDeviceCommand

CXTPChartGdiSolidSplinePolygonDeviceCommand::CXTPChartGdiSolidSplinePolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, BOOL bTwoSides)
	: CXTPChartSolidSplinePolygonDeviceCommand(color, bTwoSides)
{
	GpPath* pGpPath = 0;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipStartPathFigure(pGpPath);

	int nCount = (int)points.GetSize();

	if (bTwoSides == 2)
	{
		GdipAddPathCurve(pGpPath, (PointF*)points.GetData(), nCount);
	}
	else if (!bTwoSides)
	{
		GdipAddPathLine(pGpPath, points[0].X, points[0].Y, points[1].X, points[1].Y);
		GdipAddPathCurve(pGpPath, (PointF*)points.GetData() + 1, nCount - 2);
		GdipAddPathLine(pGpPath, points[nCount - 2].X, points[nCount - 2].Y, points[nCount - 1].X,
						points[nCount - 1].Y);
	}
	else
	{
		GdipAddPathCurve(pGpPath, (PointF*)points.GetData(), nCount / 2);
		GdipAddPathLine(pGpPath, points[nCount / 2].X, points[nCount / 2].Y,
						points[nCount / 2 + 1].X, points[nCount / 2 + 1].Y);

		GdipAddPathCurve(pGpPath, (PointF*)points.GetData() + nCount / 2, nCount / 2);

		GdipAddPathLine(pGpPath, points[nCount - 1].X, points[nCount - 1].Y, points[0].X,
						points[0].Y);
	}

	GdipClosePathFigures(pGpPath);

	m_pGpPath = pGpPath;
}

CXTPChartGdiSolidSplinePolygonDeviceCommand::~CXTPChartGdiSolidSplinePolygonDeviceCommand()
{
	GdipDeletePath(m_pGpPath);
}

CXTPChartElement* CXTPChartGdiSolidSplinePolygonDeviceCommand::HitTest(
	CPoint point, CXTPChartElement* pParent) const
{
	BOOL bResult = FALSE;
	GdipIsVisiblePathPointI(m_pGpPath, point.x, point.y, 0, &bResult);

	if (bResult)
		return pParent;

	return NULL;
}

void CXTPChartGdiSolidSplinePolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GdipFillPath(pGdiDC->GetGraphics(), pGpBrush, m_pGpPath);

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiBoundedPolygonDeviceCommand

CXTPChartGdiBoundedPolygonDeviceCommand::CXTPChartGdiBoundedPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, int thickness)
	: CXTPChartBoundedPolygonDeviceCommand(points, color, thickness)
{
}

void CXTPChartGdiBoundedPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_thickness, UnitWorld, &pGpPen);
	GdipDrawPolygon(pGdiDC->GetGraphics(), pGpPen, (PointF*)m_points.GetData(),
					(int)m_points.GetSize());
	GdipDeletePen(pGpPen);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiPolygonAntialiasingDeviceCommand

CXTPChartGdiPolygonAntialiasingDeviceCommand::CXTPChartGdiPolygonAntialiasingDeviceCommand(
	BOOL bAntiAlias)
	: CXTPChartPolygonAntialiasingDeviceCommand(bAntiAlias)
{
}

CXTPChartGdiPolygonAntialiasingDeviceCommand::~CXTPChartGdiPolygonAntialiasingDeviceCommand()
{
}

void CXTPChartGdiPolygonAntialiasingDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipGetSmoothingMode(pGdiDC->GetGraphics(), (SmoothingMode*)&m_bOldAntiAlias);
	GdipSetSmoothingMode(pGdiDC->GetGraphics(),
						 m_bAntiAlias ? SmoothingModeHighQuality : SmoothingModeDefault);
}

void CXTPChartGdiPolygonAntialiasingDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSetSmoothingMode(pGdiDC->GetGraphics(), (SmoothingMode)m_bOldAntiAlias);
}
