// XTPChartPieDeviceCommand.cpp
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

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/Types/XTPChartPie.h"
#include "Chart/Drawing/XTPChartPieDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDeviceCommandImpl

CXTPChartPieDeviceCommandImpl::~CXTPChartPieDeviceCommandImpl()
{
}

CXTPChartPieDeviceCommandImpl::CXTPChartPieDeviceCommandImpl(const CXTPPoint3d& center,
															 double dMajorSemiAxis,
															 double dMinorSemiAxis,
															 double dStartAngle, double dSweepAngle,
															 double depth, double nHolePercent)
{
	m_ptCenter			  = center;
	m_dMajorSemiAxis	  = dMajorSemiAxis;
	m_dMinorSemiAxis	  = dMinorSemiAxis;
	m_dStartAngle		  = dStartAngle;
	m_dSweepAngle		  = dSweepAngle;
	m_dDepth			  = depth;
	m_dHolePercent		  = nHolePercent / 100.0;
	m_dFacetSize		  = 0;
	m_dInnerMajorSemiAxis = 0;
	m_dInnerMinorSemiAxis = 0;
	m_dCorrectedDepth	 = 0;
	m_dInnerRadius		  = 0;
	m_bDoughnut			  = nHolePercent > CXTPMathUtils::m_dEPS;

	if (m_bDoughnut)
	{
		double doughnutThickness = dMinorSemiAxis * (1.0 - m_dHolePercent);
		m_dInnerMajorSemiAxis	= dMajorSemiAxis - doughnutThickness;
		m_dInnerMinorSemiAxis	= dMinorSemiAxis - doughnutThickness;
	}

	if (0 < depth)
	{
		m_dRadius  = dMajorSemiAxis;
		float diff = float(m_dRadius - m_dInnerMajorSemiAxis);
		if (diff < 0.0f)
		{
			m_dRadius -= diff;
			m_dFacetSize += diff;
		}

		m_dCorrectedDepth = depth - m_dFacetSize * 2;

		if (m_bDoughnut)
		{
			m_dInnerRadius = m_dInnerMajorSemiAxis;
			if (m_dInnerRadius > m_dRadius)
			{
				m_dInnerRadius = m_dRadius;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartPieDeviceCommand

CXTPChartPieDeviceCommand::CXTPChartPieDeviceCommand(const CXTPPoint3d& ptCenter,
													 double dMajorSemiAxis, double dMinorSemiAxis,
													 double dStartAngle, double dSweepAngle,
													 double dDepth, double nHolePercent)
	: CXTPChartPieDeviceCommandImpl(ptCenter, dMajorSemiAxis, dMinorSemiAxis, dStartAngle,
									dSweepAngle, dDepth, nHolePercent)
{
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPieDeviceCommand

CXTPChart3dPieDeviceCommand::CXTPChart3dPieDeviceCommand(
	XTPChart3dAntialiasingPolicy nAntialiasingPolicy, const CXTPPoint3d& ptCenter,
	double dMajorSemiAxis, double dMinorSemiAxis, double dStartAngle, double dSweepAngle,
	double dDepth, double nHolePercent)
	: CXTPChart3dDeviceCommand(nAntialiasingPolicy)
	, CXTPChartPieDeviceCommandImpl(ptCenter, dMajorSemiAxis, dMinorSemiAxis, dStartAngle,
									dSweepAngle, dDepth, nHolePercent)
{
}
