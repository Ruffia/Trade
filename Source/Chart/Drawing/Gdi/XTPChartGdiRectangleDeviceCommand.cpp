// XTPChartGdiRectangleDeviceCommand.cpp
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
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiRectangleDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiBoundedRectangleDeviceCommand

CXTPChartGdiBoundedRectangleDeviceCommand::CXTPChartGdiBoundedRectangleDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
	, m_thickness(thickness)
{
}

void CXTPChartGdiBoundedRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_thickness, UnitWorld, &pGpPen);
	GdipDrawRectangle(pGdiDC->GetGraphics(), pGpPen, m_rect.X, m_rect.Y, m_rect.Width,
					  m_rect.Height);
	GdipDeletePen(pGpPen);
}

void CXTPChartGdiBoundedRectangleDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipGetSmoothingMode(pGdiDC->GetGraphics(), (SmoothingMode*)&m_OldSmoothingMode);
	GdipGetPixelOffsetMode(pGdiDC->GetGraphics(), (PixelOffsetMode*)&m_OldPixelOffsetMode);
	GdipGetInterpolationMode(pGdiDC->GetGraphics(), (InterpolationMode*)&m_OldInterpolationMode);

	GdipSetSmoothingMode(pGdiDC->GetGraphics(), SmoothingModeNone);
	GdipSetPixelOffsetMode(pGdiDC->GetGraphics(), PixelOffsetModeDefault);
	GdipSetInterpolationMode(pGdiDC->GetGraphics(), InterpolationModeBilinear);
}

void CXTPChartGdiBoundedRectangleDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSetSmoothingMode(pGdiDC->GetGraphics(), (SmoothingMode)m_OldSmoothingMode);
	GdipSetPixelOffsetMode(pGdiDC->GetGraphics(), (PixelOffsetMode)m_OldPixelOffsetMode);
	GdipSetInterpolationMode(pGdiDC->GetGraphics(), (InterpolationMode)m_OldInterpolationMode);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiInnerBorderDeviceCommand

CXTPChartGdiInnerBorderDeviceCommand::CXTPChartGdiInnerBorderDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness)
	: CXTPChartGdiBoundedRectangleDeviceCommand(rect, color, thickness)
{
	m_rect.X += static_cast<float>(thickness) / 2.0f - 0.5f;
	m_rect.Y += static_cast<float>(thickness) / 2.0f - 0.5f;

	m_rect.Width -= static_cast<float>(thickness) - 0.5f;
	m_rect.Height -= static_cast<float>(thickness) - 0.5f;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidRectangleDeviceCommand

CXTPChartGdiSolidRectangleDeviceCommand::CXTPChartGdiSolidRectangleDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
{
}

void CXTPChartGdiSolidRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);
	GdipFillRectangle(pGdiDC->GetGraphics(), pGpBrush, m_rect.X, m_rect.Y, m_rect.Width,
					  m_rect.Height);
	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiGradientRectangleDeviceCommand

CXTPChartGdiGradientRectangleDeviceCommand::CXTPChartGdiGradientRectangleDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2,
	XTPChartLinearGradientMode nMode)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
	, m_color2(color2)
	, m_nMode(nMode)
{
}

void CXTPChartGdiGradientRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpLineGradient* pGpBrush = NULL;
	if (m_nMode == xtpChartLinearGradientModeCenterHorizontal)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_rect, m_color.GetValue(), m_color.GetValue(),
									LinearGradientModeHorizontal, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = { m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(),
								  m_color.GetValue() };
		REAL positions[]	  = { 0, 0.45f, 0.55f, 1 };
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else if (m_nMode == xtpChartLinearGradientModeCenterVertical)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_rect, m_color.GetValue(), m_color.GetValue(),
									LinearGradientModeVertical, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = { m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(),
								  m_color.GetValue() };
		REAL positions[]	  = { 0, 0.45f, 0.55f, 1 };
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else
	{
		GdipCreateLineBrushFromRect((RectF*)&m_rect, m_color.GetValue(), m_color2.GetValue(),
									(LinearGradientMode)m_nMode, WrapModeTileFlipXY, &pGpBrush);
	}

	GdipFillRectangle(pGdiDC->GetGraphics(), pGpBrush, m_rect.X, m_rect.Y, m_rect.Width,
					  m_rect.Height);

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiHatchRectangleDeviceCommand

CXTPChartGdiHatchRectangleDeviceCommand::CXTPChartGdiHatchRectangleDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2,
	XTPChartHatchStyle nStyle)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
	, m_color2(color2)
	, m_nStyle(nStyle)
{
}

void CXTPChartGdiHatchRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpHatch* pGpBrush = NULL;
	GdipCreateHatchBrush((GpHatchStyle)m_nStyle, m_color.GetValue(), m_color2.GetValue(),
						 &pGpBrush);
	GdipFillRectangle(pGdiDC->GetGraphics(), pGpBrush, m_rect.X, m_rect.Y, m_rect.Width,
					  m_rect.Height);
	GdipDeleteBrush(pGpBrush);
}
