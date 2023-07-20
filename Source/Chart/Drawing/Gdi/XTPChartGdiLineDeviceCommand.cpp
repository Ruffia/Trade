// XTPChartGdiLineDeviceCommand.cpp
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
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiLineDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidLineDeviceCommand

CXTPChartGdiSolidLineDeviceCommand::CXTPChartGdiSolidLineDeviceCommand(const CXTPPoint2f& p1,
																	   const CXTPPoint2f& p2,
																	   const CXTPChartColor& color,
																	   int thickness)
	: CXTPChartLineDeviceCommand(p1, p2, color, thickness)
{
}

void CXTPChartGdiSolidLineDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);
	GdipDrawLine(pGdiDC->GetGraphics(), pGpPen, (REAL)m_p1.X, (REAL)m_p1.Y, (REAL)m_p2.X,
				 (REAL)m_p2.Y);
	GdipDeletePen(pGpPen);
}

void CXTPChartGdiSolidLineDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipGetPixelOffsetMode(pGdiDC->GetGraphics(), (PixelOffsetMode*)&m_OldPixelOffsetMode);
	GdipGetInterpolationMode(pGdiDC->GetGraphics(), (InterpolationMode*)&m_OldInterpolationMode);

	GdipSetPixelOffsetMode(pGdiDC->GetGraphics(), PixelOffsetModeDefault);
	GdipSetInterpolationMode(pGdiDC->GetGraphics(), InterpolationModeBilinear);
}

void CXTPChartGdiSolidLineDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSetPixelOffsetMode(pGdiDC->GetGraphics(), (PixelOffsetMode)m_OldPixelOffsetMode);
	GdipSetInterpolationMode(pGdiDC->GetGraphics(), (InterpolationMode)m_OldInterpolationMode);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartDashedLineGraphicsCommand

static DashStyle GetDashStyle(XTPChartDashStyle dashStyle)
{
	DashStyle gdipDashStyle = DashStyleSolid;

	switch (dashStyle)
	{
		case xtpChartDashStyleSolid: gdipDashStyle = DashStyleSolid; break;
		case xtpChartDashStyleDash: gdipDashStyle = DashStyleDash; break;
		case xtpChartDashStyleDot: gdipDashStyle = DashStyleDot; break;
		case xtpChartDashStyleDashDot: gdipDashStyle = DashStyleDashDot; break;
		case xtpChartDashStyleDashDotDot: gdipDashStyle = DashStyleDashDotDot; break;
		case xtpChartDashStyleCustom: gdipDashStyle = DashStyleCustom; break;
	}

	return gdipDashStyle;
}

CXTPChartGdiDashedLineDeviceCommand::CXTPChartGdiDashedLineDeviceCommand(
	const CXTPPoint2f& p1, const CXTPPoint2f& p2, const CXTPChartColor& color, int thickness,
	XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& arrDashArray)
	: CXTPChartLineDeviceCommand(p1, p2, color, thickness)
{
	m_nDashStyle = nDashStyle;
	m_arrDashArray.Copy(arrDashArray);
};

void CXTPChartGdiDashedLineDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);

	if (m_nDashStyle == xtpChartDashStyleCustom)
	{
		GdipSetPenDashStyle(pGpPen, DashStyleCustom);
		GdipSetPenDashArray(pGpPen, (REAL*)m_arrDashArray.GetData(), (int)m_arrDashArray.GetSize());
	}
	else
	{
		GdipSetPenDashStyle(pGpPen, GetDashStyle(m_nDashStyle));
	}

	GdipDrawLine(pGdiDC->GetGraphics(), pGpPen, (REAL)m_p1.X, (REAL)m_p1.Y, (REAL)m_p2.X,
				 (REAL)m_p2.Y);

	GdipDeletePen(pGpPen);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidPolylineDeviceCommand

CXTPChartGdiSolidPolylineDeviceCommand::CXTPChartGdiSolidPolylineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness)
	: CXTPChartPolylineDeviceCommand(p, thickness)
{
	m_color = color;
}

void CXTPChartGdiSolidPolylineDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);

	GdipDrawLines(pGdiDC->GetGraphics(), pGpPen, (PointF*)m_p.GetData(), (int)m_p.GetSize());

	GdipDeletePen(pGpPen);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiDashedPolylineDeviceCommand

CXTPChartGdiDashedPolylineDeviceCommand::CXTPChartGdiDashedPolylineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness,
	XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& arrDashArray)
	: CXTPChartPolylineDeviceCommand(p, thickness)
{
	m_color		 = color;
	m_nDashStyle = nDashStyle;
	m_arrDashArray.Copy(arrDashArray);
};

void CXTPChartGdiDashedPolylineDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);
	GdipSetPenDashStyle(pGpPen, GetDashStyle(m_nDashStyle));

	if (m_nDashStyle == xtpChartDashStyleCustom)
	{
		GdipSetPenDashArray(pGpPen, (REAL*)m_arrDashArray.GetData(), (int)m_arrDashArray.GetSize());
	}

	GdipDrawLines(pGdiDC->GetGraphics(), pGpPen, (PointF*)m_p.GetData(), (int)m_p.GetSize());

	GdipDeletePen(pGpPen);
}

CXTPChartElement* CXTPChartGdiDashedPolylineDeviceCommand::HitTest(CPoint point,
																   CXTPChartElement* pParent) const
{
	ASSERT(m_p.GetSize() > 1);

	for (int i = 0; i < m_p.GetSize() - 1; i++)
	{
		CXTPChartPointF p1 = m_p.GetAt(i);
		CXTPChartPointF p2 = m_p.GetAt(i + 1);

		if (CXTPMathUtils::HitTestLineSegment(p1.X, p1.Y, p2.X, p2.Y, point.x, point.y,
											  m_nThickness))
		{
			return pParent;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSplineDeviceCommand

CXTPChartGdiSplineDeviceCommand::CXTPChartGdiSplineDeviceCommand(const CXTPChartPoints& p,
																 int thickness)
	: CXTPChartSplineDeviceCommand(p, thickness)
{
}

CXTPChartElement* CXTPChartGdiSplineDeviceCommand::HitTest(CPoint point,
														   CXTPChartElement* pParent) const
{
	GpPath* pGpPath;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipAddPathCurve(pGpPath, (PointF*)m_p.GetData(), (int)m_p.GetSize());

	GpPen* pGpPen = NULL;
	GdipCreatePen1(CXTPChartColor::Black, (float)m_nThickness, UnitWorld, &pGpPen);

	BOOL bResult = FALSE;
	GdipIsOutlineVisiblePathPointI(pGpPath, point.x, point.y, pGpPen, 0, &bResult);

	GdipDeletePen(pGpPen);

	GdipDeletePath(pGpPath);

	if (bResult)
		return pParent;

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSolidSplineDeviceCommand

CXTPChartGdiSolidSplineDeviceCommand::CXTPChartGdiSolidSplineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness)
	: CXTPChartSplineDeviceCommand(p, thickness)
{
	m_color = color;
}

void CXTPChartGdiSolidSplineDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (float)m_nThickness, UnitWorld, &pGpPen);

	GdipDrawCurve(pGdiDC->GetGraphics(), pGpPen, (PointF*)m_p.GetData(), (int)m_p.GetSize());

	GdipDeletePen(pGpPen);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiDashedSplineDeviceCommand

CXTPChartGdiDashedSplineDeviceCommand::CXTPChartGdiDashedSplineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness,
	XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& arrDashArray)
	: CXTPChartSplineDeviceCommand(p, thickness)
{
	m_color		 = color;
	m_nDashStyle = nDashStyle;
	m_arrDashArray.Copy(arrDashArray);
};

void CXTPChartGdiDashedSplineDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);

	GdipSetPenDashStyle(pGpPen, GetDashStyle(m_nDashStyle));

	if (m_nDashStyle == xtpChartDashStyleCustom)
	{
		GdipSetPenDashArray(pGpPen, (REAL*)m_arrDashArray.GetData(), (int)m_arrDashArray.GetSize());
	}

	GdipDrawCurve(pGdiDC->GetGraphics(), pGpPen, (PointF*)m_p.GetData(), (int)m_p.GetSize());

	GdipDeletePen(pGpPen);
}
