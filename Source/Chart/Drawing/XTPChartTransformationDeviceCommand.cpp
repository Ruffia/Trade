// XTPChartTransformationDeviceCommand.cpp
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
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRotateDeviceCommand

CXTPChartRotateDeviceCommand::CXTPChartRotateDeviceCommand(float fAngle)
{
	m_fAngle		 = fAngle;
	m_ptRotateVector = CXTPPoint3d(0, 0, 1);
}

CXTPChartRotateDeviceCommand::CXTPChartRotateDeviceCommand(float fAngle,
														   const CXTPPoint3d& rotateVector)
{
	m_fAngle		 = fAngle;
	m_ptRotateVector = rotateVector;
}

CXTPChartElement* CXTPChartRotateDeviceCommand::HitTest(CPoint point,
														CXTPChartElement* pParent) const
{
	double angle = m_fAngle * CXTPMathUtils::m_dPI / 180.0;

	float x = static_cast<float>(point.x * cos(angle) - point.y * sin(angle));
	float y = static_cast<float>(point.x * sin(angle) + point.y * cos(angle));

	return CXTPChartDeviceCommand::HitTest(CPoint((int)x, (int)y), pParent);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dRotateDeviceCommand

CXTPChart3dRotateDeviceCommand::CXTPChart3dRotateDeviceCommand(const CXTPChart3dRotation& rotation)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
	, m_rotation(rotation)
{
}

CXTPChartElement* CXTPChart3dRotateDeviceCommand::HitTest(CPoint point,
														  CXTPChartElement* pParent) const
{
	UNREFERENCED_PARAMETER(point);
	UNREFERENCED_PARAMETER(pParent);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTranslateDeviceCommand

CXTPChartTranslateDeviceCommand::CXTPChartTranslateDeviceCommand(double dx, double dy)
{
	m_dx = dx;
	m_dy = dy;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dTranslateDeviceCommand

CXTPChart3dTranslateDeviceCommand::CXTPChart3dTranslateDeviceCommand(double dx, double dy,
																	 double dz)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
{
	m_dx = dx;
	m_dy = dy;
	m_dz = dz;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartClipDeviceCommand

CXTPChartClipDeviceCommand::CXTPChartClipDeviceCommand(CRect rcClip)
{
	m_rcClip = rcClip;
}
