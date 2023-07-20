// XTPChartGdiCircleDeviceCommand.cpp
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
#include "Chart/Drawing/XTPChartCircleDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiCircleDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidCircleDeviceCommand

CXTPChartGdiSolidCircleDeviceCommand::CXTPChartGdiSolidCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color)
	: CXTPChartCircleDeviceCommand(center, radius)
{
	m_color = color;
}

void CXTPChartGdiSolidCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	RectF rect((REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2),
			   (REAL)(m_radius * 2));

	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GdipFillEllipse(pGdiDC->GetGraphics(), pGpBrush, (REAL)(m_center.X - m_radius),
					(REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiGradientCircleDeviceCommand

CXTPChartGdiGradientCircleDeviceCommand::CXTPChartGdiGradientCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color,
	const CXTPChartColor& color2, XTPChartLinearGradientMode nMode)
	: CXTPChartCircleDeviceCommand(center, radius)
{
	m_color  = color;
	m_color2 = color2;
	m_nMode  = nMode;
}

void CXTPChartGdiGradientCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	RectF rect((REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2),
			   (REAL)(m_radius * 2));

	GpLineGradient* pGpBrush = NULL;
	if (m_nMode == xtpChartLinearGradientModeCenterHorizontal)
	{
		GdipCreateLineBrushFromRect((RectF*)&rect, m_color.GetValue(), m_color.GetValue(),
									LinearGradientModeHorizontal, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = { m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(),
								  m_color.GetValue() };
		REAL positions[]	  = { 0, 0.45f, 0.55f, 1 };
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else if (m_nMode == xtpChartLinearGradientModeCenterVertical)
	{
		GdipCreateLineBrushFromRect((RectF*)&rect, m_color.GetValue(), m_color.GetValue(),
									LinearGradientModeVertical, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = { m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(),
								  m_color.GetValue() };
		REAL positions[]	  = { 0, 0.45f, 0.55f, 1 };
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else
	{
		GdipCreateLineBrushFromRect((RectF*)&rect, m_color.GetValue(), m_color2.GetValue(),
									(LinearGradientMode)m_nMode, WrapModeTileFlipXY, &pGpBrush);
	}

	GdipFillEllipse(pGdiDC->GetGraphics(), pGpBrush, (REAL)(m_center.X - m_radius),
					(REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiHatchCircleDeviceCommand

CXTPChartGdiHatchCircleDeviceCommand::CXTPChartGdiHatchCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color,
	const CXTPChartColor& color2, XTPChartHatchStyle nStyle)
	: CXTPChartCircleDeviceCommand(center, radius)
{
	m_color  = color;
	m_color2 = color2;
	m_nStyle = nStyle;
}

void CXTPChartGdiHatchCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	RectF rect((REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2),
			   (REAL)(m_radius * 2));

	GpHatch* pGpBrush = NULL;
	GdipCreateHatchBrush((GpHatchStyle)m_nStyle, m_color.GetValue(), m_color2.GetValue(),
						 &pGpBrush);

	GdipFillEllipse(pGdiDC->GetGraphics(), pGpBrush, (REAL)(m_center.X - m_radius),
					(REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiBoundedCircleDeviceCommand

CXTPChartGdiBoundedCircleDeviceCommand::CXTPChartGdiBoundedCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color, int thickness)
	: CXTPChartBoundedCircleDeviceCommand(center, radius, color, thickness)
{
}

void CXTPChartGdiBoundedCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	if (m_thickness == 0)
		return;

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_thickness, UnitWorld, &pGpPen);

	GdipDrawEllipse(pGdiDC->GetGraphics(), pGpPen, (REAL)(m_center.X - m_radius),
					(REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeletePen(pGpPen);
}
