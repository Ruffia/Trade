// XTPChartPolygonDeviceCommand.cpp
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
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPolygonDeviceCommand

CXTPChartPolygonDeviceCommand::CXTPChartPolygonDeviceCommand(const CXTPChartPoints& points)
{
	m_points.Copy(points);

	if (points.GetSize() != 0)
	{
		float xMax = points[0].X, xMin = points[0].X;
		float yMax = points[0].Y, yMin = points[0].Y;

		for (int i = 1; i < points.GetSize(); i++)
		{
			xMax = max(xMax, points[i].X);
			yMax = max(yMax, points[i].Y);
			xMin = min(xMin, points[i].X);
			yMin = min(yMin, points[i].Y);
		}

		m_bounds.X		= xMin;
		m_bounds.Y		= yMin;
		m_bounds.Width  = xMax - xMin;
		m_bounds.Height = yMax - yMin;
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSolidSplinePolygonDeviceCommand

CXTPChartSolidSplinePolygonDeviceCommand::CXTPChartSolidSplinePolygonDeviceCommand(
	const CXTPChartColor& color, BOOL bTwoSides)
	: m_color(color)
	, m_bTwoSides(bTwoSides)
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedPolygonDeviceCommand

CXTPChartBoundedPolygonDeviceCommand::CXTPChartBoundedPolygonDeviceCommand(
	const CXTPChartPoints& points, const CXTPChartColor& color, int thickness)
{
	m_points.Copy(points);
	m_color		= color;
	m_thickness = thickness;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPolygonAntialiasingDeviceCommand

CXTPChartPolygonAntialiasingDeviceCommand::CXTPChartPolygonAntialiasingDeviceCommand(BOOL bAntiAlias)
{
	m_bAntiAlias = bAntiAlias;
}
