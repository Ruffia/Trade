// XTPChartDrawingObjectFactory.cpp
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
#include "GraphicLibrary/OpenGL/GL.h"
#include "GraphicLibrary/OpenGL/GLU.h"
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
#include "Common/Math/XTPMatrix.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/Drawing/XTPChartCircleDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPieDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartTextDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"

#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"
#include "Chart/Drawing/Gdi/XTPChartGdiPolygonDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiLineDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiTransformationDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiRectangleDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiCircleDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiTextDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiPieDeviceCommand.h"

#include "Chart/Drawing/OpenGL/XTPChartOpenGLDeviceContext.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLCameraDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLShadeModelDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLLineDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLTransformationDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLBackgroundColorDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLTransformationDeviceCommand.h"
#include "Chart/Styles/Pie/3D/Commands/OpenGL/XTPChartOpenGLPieDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPChartDrawingObjectFactory

CXTPChartDrawingObjectFactory::CXTPChartDrawingObjectFactory()
{
}

CXTPChartDrawingObjectFactory::~CXTPChartDrawingObjectFactory()
{
}

CXTPChartDeviceContext* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dDeviceContext(
	CXTPChartContainer* pContainer, HDC hDC, CXTPChartDeviceContext* p3dDC /*= NULL*/)
{
	return new CXTPChartGdiDeviceContext(pContainer, hDC, p3dDC);
}

CXTPChartDeviceContext* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dDeviceContext(
	CXTPChartContainer* pContainer, HDC hDC, const RECT& rcBounds)
{
	return new CXTPChartOpenGLDeviceContext(pContainer, hDC, rcBounds);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(CXTPChartElement* pElement)
{
	return new CXTPChartHitTestElementCommand(pElement);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
	CXTPChartElement* pElement, const CRect& rcBounds)
{
	return new CXTPChartHitTestElementCommand(pElement, rcBounds);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(
	CXTPChartElement* pElement, const CXTPChartRectF& rcBounds)
{
	return new CXTPChartHitTestElementCommand(pElement, rcBounds);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dBoundedCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiBoundedCircleDeviceCommand(center, radius, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dSolidCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color)
{
	return new CXTPChartGdiSolidCircleDeviceCommand(center, radius, color);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dHatchCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color,
	const CXTPChartColor& color2, XTPChartHatchStyle nStyle)
{
	return new CXTPChartGdiHatchCircleDeviceCommand(center, radius, color, color2, nStyle);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color,
	const CXTPChartColor& color2, XTPChartLinearGradientMode nMode)
{
	return new CXTPChartGdiGradientCircleDeviceCommand(center, radius, color, color2, nMode);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(
	const CXTPPoint2f& p1, const CXTPPoint2f& p2, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiSolidLineDeviceCommand(p1, p2, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dSolidLineDeviceCommand(
	const CXTPPoint3d& p1, const CXTPPoint3d& p2, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartOpenGLSolidLineDeviceCommand(p1, p2, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dDashedLineDeviceCommand(
	const CXTPPoint2f& p1, const CXTPPoint2f& p2, const CXTPChartColor& color, int thickness,
	XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& dashArray)
{
	return new CXTPChartGdiDashedLineDeviceCommand(p1, p2, color, thickness, nDashStyle, dashArray);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dSolidPolylineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiSolidPolylineDeviceCommand(p, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiSolidSplineDeviceCommand(p, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dDashedSplineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness,
	XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& dashArray)
{
	return new CXTPChartGdiDashedSplineDeviceCommand(p, color, thickness, nDashStyle, dashArray);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dDashedPolylineDeviceCommand(
	const CXTPChartPoints& p, const CXTPChartColor& color, int thickness,
	XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& dashArray)
{
	return new CXTPChartGdiDashedPolylineDeviceCommand(p, color, thickness, nDashStyle, dashArray);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dSolidSplinePolygonDeviceCommand(
		const CXTPChartPoints& points, const CXTPChartColor& color, BOOL bTwoSides /*= FALSE*/)
{
	return new CXTPChartGdiSolidSplinePolygonDeviceCommand(points, color, bTwoSides);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dBoundedPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiBoundedPolygonDeviceCommand(points, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dSolidPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color)
{
	return new CXTPChartGdiSolidPolygonDeviceCommand(points, color);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dHatchPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2,
	XTPChartHatchStyle nStyle)
{
	return new CXTPChartGdiHatchPolygonDeviceCommand(points, color, color2, nStyle);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
		const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2,
		XTPChartLinearGradientMode nMode)
{
	return new CXTPChartGdiGradientPolygonDeviceCommand(points, color, color2, nMode);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dPolygonAntialiasingDeviceCommand(
		BOOL bAntiAlias /*= TRUE*/)
{
	return new CXTPChartGdiPolygonAntialiasingDeviceCommand(bAntiAlias);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create3dShadeModelDeviceCommand(BOOL bAntiAlias /*= TRUE*/)
{
	return new CXTPChartOpenGLShadeModelDeviceCommand(bAntiAlias);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dInnerBorderDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiInnerBorderDeviceCommand(rect, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dBoundedRectangleDeviceCommand(
		const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness)
{
	return new CXTPChartGdiBoundedRectangleDeviceCommand(rect, color, thickness);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dSolidRectangleDeviceCommand(const CXTPChartRectF& rect,
																	   const CXTPChartColor& color)
{
	return new CXTPChartGdiSolidRectangleDeviceCommand(rect, color);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
		const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2,
		XTPChartLinearGradientMode nMode)
{
	return new CXTPChartGdiGradientRectangleDeviceCommand(rect, color, color2, nMode);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dHatchRectangleDeviceCommand(const CXTPChartRectF& rect,
																	   const CXTPChartColor& color,
																	   const CXTPChartColor& color2,
																	   XTPChartHatchStyle nStyle)
{
	return new CXTPChartGdiHatchRectangleDeviceCommand(rect, color, color2, nStyle);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dTextAntialiasingDeviceCommand(BOOL bAntialias /*= TRUE*/)
{
	return new CXTPChartGdiTextAntialiasingDeviceCommand(bAntialias);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dBoundedTextDeviceCommand(
	const CXTPChartString& strText, CXTPChartFont* pFont, const CXTPChartColor& color,
	const CXTPChartRectF& rectangle)
{
	return new CXTPChartGdiBoundedTextDeviceCommand(strText, pFont, color, rectangle);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dMarkupElementDeviceCommand(
	CXTPMarkupUIElement* pMarkupUIElement, CXTPChartFont* pFont, const CXTPChartColor& color,
	const CXTPChartRectF& rectangle)
{
	return new CXTPChartGdiMarkupElementDeviceCommand(pMarkupUIElement, pFont, color, rectangle);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dSaveStateDeviceCommand()
{
	return new CXTPChartGdiSaveStateDeviceCommand();
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dSaveMatrixDeviceCommand()
{
	return new CXTPChartOpenGLSaveMatrixDeviceCommand();
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dMultiplyMatrixDeviceCommand(
	const CXTPMatrix<double>& matrix, BOOL bPushMatrix)
{
	return new CXTPChartOpenGLMultiplyMatrixDeviceCommand(matrix, bPushMatrix);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dLoadIdentityDeviceCommand()
{
	return new CXTPChartOpenGLLoadIdentityDeviceCommand();
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dClipDeviceCommand(CRect rcClip)
{
	return new CXTPChartGdiClipDeviceCommand(rcClip);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dClipRegionDeviceCommand(const CXTPChartPoints& points)
{
	return new CXTPChartGdiClipRegionDeviceCommand(points);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dRotateDeviceCommand(float fAngle)
{
	return new CXTPChartGdiRotateDeviceCommand(fAngle);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dRotateDeviceCommand(
	float fAngle, const CXTPPoint3d& rotateVector)
{
	return new CXTPChartGdiRotateDeviceCommand(fAngle, rotateVector);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create3dRotateDeviceCommand(const CXTPChart3dRotation& rotation)
{
	return new CXTPChartOpenGLRotateDeviceCommand(rotation);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dTranslateDeviceCommand(double dx, double dy)
{
	return new CXTPChartGdiTranslateDeviceCommand(dx, dy);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create3dTranslateDeviceCommand(double dx, double dy, double dz)
{
	return new CXTPChartOpenGLTranslateDeviceCommand(dx, dy, dz);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dGradientPieDeviceCommand(
	const CXTPPoint3d& center, double majorSemiAxis, double minorSemiAxis, double startAngle,
	double sweepAngle, double depth, double holePercent, const CXTPChartRectF& gradientBounds,
	const CXTPChartColor& color, const CXTPChartColor& color2)
{
	return new CXTPChartGdiGradientPieDeviceCommand(center, majorSemiAxis, minorSemiAxis,
													startAngle, sweepAngle, depth, holePercent,
													gradientBounds, color, color2);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dGradientPieDeviceCommand(
	const CXTPPoint3d& center, double radius, double startAngle, double sweepAngle, double depth,
	double holePercent, const CXTPChartRectF& gradientBounds, const CXTPChartColor& color,
	const CXTPChartColor& color2, BOOL bStartGap /*= FALSE*/, BOOL bEndGap /*= FALSE*/)
{
	return new CXTPChartOpenGLPieDeviceCommand(center, radius, startAngle, sweepAngle, depth,
											   holePercent, gradientBounds, color, color2,
											   bStartGap, bEndGap);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create2dBoundedPieDeviceCommand(
	const CXTPPoint3d& center, double majorSemiAxis, double minorSemiAxis, double startAngle,
	double sweepAngle, double depth, int holePercent, const CXTPChartColor& color, int nThickness)
{
	return new CXTPChartGdiBoundedPieDeviceCommand(center, majorSemiAxis, minorSemiAxis, startAngle,
												   sweepAngle, depth, holePercent, color,
												   nThickness);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create2dFillBackgroundDeviceCommand(const CXTPChartRectF& rect,
																	   const CXTPChartColor& color)
{
	return Create2dSolidRectangleDeviceCommand(rect, color);
}

CXTPChartDeviceCommand* AFX_CDECL
	CXTPChartDrawingObjectFactory::Create3dFillBackgroundDeviceCommand(const CXTPChartColor& color)
{
	return new CXTPChartOpenGLBackgroundColorDeviceCommand(color);
}

CXTPChartDeviceCommand* AFX_CDECL CXTPChartDrawingObjectFactory::Create3dCameraDeviceCommand(
	const CXTPPoint3f& ptPosition, const CXTPPoint3f& ptFocal, const CXTPPoint3f& ptUpVector)
{
	return new CXTPChartOpenGLCameraDeviceCommand(ptPosition, ptFocal, ptUpVector);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart2dDrawingObjectFactory

CXTPChart2dDrawingObjectFactory::CXTPChart2dDrawingObjectFactory()
{
}

CXTPChartDeviceContext* CXTPChart2dDrawingObjectFactory::CreateDeviceContext(
	CXTPChartContainer* pContainer, HDC hDC, const RECT& rcBounds)
{
	UNREFERENCED_PARAMETER(rcBounds);
	return Create2dDeviceContext(pContainer, hDC);
}

CXTPChartDeviceCommand* CXTPChart2dDrawingObjectFactory::CreatePolygonAntialiasingDeviceCommand(
	BOOL bAntiAlias /*= TRUE*/)
{
	return CreatePolygonAntialiasingDeviceCommand(bAntiAlias);
}

CXTPChartDeviceCommand* CXTPChart2dDrawingObjectFactory::CreateGradientPieDeviceCommand(
	const CXTPPoint3d& center, double majorSemiAxis, double minorSemiAxis, double startAngle,
	double sweepAngle, double depth, double holePercent, const CXTPChartRectF& gradientBounds,
	const CXTPChartColor& color, const CXTPChartColor& color2)
{
	return Create2dGradientPieDeviceCommand(center, majorSemiAxis, minorSemiAxis, startAngle,
											sweepAngle, depth, holePercent, gradientBounds, color,
											color2);
}

CXTPChartDeviceCommand* CXTPChart2dDrawingObjectFactory::CreateFillBackgroundDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color)
{
	return Create2dFillBackgroundDeviceCommand(rect, color);
}

CXTPChartDeviceCommand* CXTPChart2dDrawingObjectFactory::CreateSolidLineDeviceCommand(
	const CXTPPoint3d& p1, const CXTPPoint3d& p2, const CXTPChartColor& color, int thickness)
{
	return Create2dSolidLineDeviceCommand(p1, p2, color, thickness);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dDrawingObjectFactory

CXTPChart3dDrawingObjectFactory::CXTPChart3dDrawingObjectFactory()
{
}

CXTPChartDeviceContext* CXTPChart3dDrawingObjectFactory::CreateDeviceContext(
	CXTPChartContainer* pContainer, HDC hDC, const RECT& rcBounds)
{
	return Create3dDeviceContext(pContainer, hDC, rcBounds);
}

CXTPChartDeviceCommand* CXTPChart3dDrawingObjectFactory::CreatePolygonAntialiasingDeviceCommand(
	BOOL bAntiAlias /*= TRUE*/)
{
	return Create3dShadeModelDeviceCommand(bAntiAlias);
}

CXTPChartDeviceCommand* CXTPChart3dDrawingObjectFactory::CreateGradientPieDeviceCommand(
	const CXTPPoint3d& center, double majorSemiAxis, double minorSemiAxis, double startAngle,
	double sweepAngle, double depth, double holePercent, const CXTPChartRectF& gradientBounds,
	const CXTPChartColor& color, const CXTPChartColor& color2)
{
	UNREFERENCED_PARAMETER(minorSemiAxis);

	// In case of 3D pie both semiaxes have to be equal
	ASSERT(majorSemiAxis == minorSemiAxis);

	return Create3dGradientPieDeviceCommand(center, majorSemiAxis, startAngle, sweepAngle, depth,
											holePercent, gradientBounds, color, color2);
}

CXTPChartDeviceCommand* CXTPChart3dDrawingObjectFactory::CreateFillBackgroundDeviceCommand(
	const CXTPChartRectF& rect, const CXTPChartColor& color)
{
	UNREFERENCED_PARAMETER(rect);
	return Create3dFillBackgroundDeviceCommand(color);
}

CXTPChartDeviceCommand* CXTPChart3dDrawingObjectFactory::CreateSolidLineDeviceCommand(
	const CXTPPoint3d& p1, const CXTPPoint3d& p2, const CXTPChartColor& color, int thickness)
{
	return Create3dSolidLineDeviceCommand(p1, p2, color, thickness);
}
