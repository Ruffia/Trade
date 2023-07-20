// XTPChartCircleDeviceCommand.cpp
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
#include "Chart/Drawing/XTPChartCircleDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartCircleDeviceCommand

CXTPChartCircleDeviceCommand::CXTPChartCircleDeviceCommand(const CXTPChartPointF& center,
														   double radius)
{
	m_center = center;
	m_radius = radius;
}

CXTPChartElement* CXTPChartCircleDeviceCommand::HitTest(CPoint point,
														CXTPChartElement* pParent) const
{
	if (point.x < m_center.X - m_radius || point.x > m_center.X + m_radius)
		return NULL;

	if (point.y < m_center.Y - m_radius || point.y > m_center.Y + m_radius)
		return NULL;

	if ((point.x - static_cast<double>(m_center.X)) * (point.x - static_cast<double>(m_center.X))
			+ (point.y - static_cast<double>(m_center.Y))
				  * (point.y - static_cast<double>(m_center.Y))
		<= m_radius * m_radius)
		return pParent;

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedCircleDeviceCommand

CXTPChartBoundedCircleDeviceCommand::CXTPChartBoundedCircleDeviceCommand(
	const CXTPChartPointF& center, double radius, const CXTPChartColor& color, int thickness)
{
	m_center	= center;
	m_radius	= radius;
	m_color		= color;
	m_thickness = thickness;
}
