// XTPChart3dPie.cpp
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

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartPieDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Types/XTPChartPie.h"
#include "Chart/Types/3D/XTPChart3dPie.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPie

IMPLEMENT_DYNAMIC(CXTPChart3dPie, CXTPChartPieBase);

CXTPChart3dPie::CXTPChart3dPie(double dStartAngle, double dFinishAngle,
							   const CXTPChartEllipse& ellipse, int nDepthPercent, int nHolePercent,
							   BOOL bStartGap /*= FALSE*/, BOOL bEndGap /*= FALSE*/)
	: CXTPChartPieBase(dStartAngle, dFinishAngle, ellipse, nDepthPercent, nHolePercent)
	, m_bStartGap(bStartGap)
	, m_bEndGap(bEndGap)
{
	// In case of 3D pie the ellipse must have equal semiaxes
	ASSERT(ellipse.m_dMajorSemiaxis == ellipse.m_dMinorSemiaxis);
}

CXTPChartDeviceCommand* CXTPChart3dPie::CreatePieDeviceCommand(const CXTPChartColor& color,
															   const CXTPChartColor& color2,
															   const CXTPChartPointF& basePoint)
{
	CXTPPoint3d center = CalculateCenter(basePoint);

	CXTPChartRectF bounds(m_anchorBounds);
	bounds.Offset(basePoint);

	return CXTPChartDrawingObjectFactory::Create3dGradientPieDeviceCommand(
		center, m_ellipse.m_dMajorSemiaxis, -CXTPMathUtils::Radian2Degree(m_dStartAngle),
		-CXTPMathUtils::Radian2Degree(m_dSweepAngle), m_depth, m_nHolePercent, bounds, color2,
		color, m_bStartGap, m_bEndGap);
}

CXTPChartDeviceCommand* CXTPChart3dPie::CreateBoundedPieDeviceCommand(
	const CXTPChartColor& color, int nThickness, const CXTPChartPointF& basePoint)
{
	// Not supported in 3D.

	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(nThickness);
	UNREFERENCED_PARAMETER(basePoint);

	return NULL;
}
