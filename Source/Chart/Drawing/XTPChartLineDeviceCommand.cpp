// XTPChartLineDeviceCommand.cpp
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

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineDeviceCommand

CXTPChartLineDeviceCommand::CXTPChartLineDeviceCommand(const CXTPPoint2f& p1, const CXTPPoint2f& p2,
													   const CXTPChartColor& color, int thickness)
	: m_p1(p1)
	, m_p2(p2)
	, m_color(color)
	, m_nThickness(thickness)
{
}

CXTPChartElement* CXTPChartLineDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	CXTPChartElement* pHitElement = NULL;

	if (CXTPMathUtils::HitTestLineSegment(m_p1.X, m_p1.Y, m_p2.X, m_p2.Y, point.x, point.y,
										  m_nThickness))
	{
		pHitElement = pParent;
	}

	return pHitElement;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPolylineDeviceCommand

CXTPChartPolylineDeviceCommand::CXTPChartPolylineDeviceCommand(const CXTPChartPoints& p,
															   int thickness)
{
	m_p.Copy(p);
	m_nThickness = thickness;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineDeviceCommand

CXTPChartSplineDeviceCommand::CXTPChartSplineDeviceCommand(const CXTPChartPoints& p, int thickness)
{
	m_p.Copy(p);
	m_nThickness = thickness;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dLineDeviceCommand

CXTPChart3dLineDeviceCommand::CXTPChart3dLineDeviceCommand(const CXTPPoint3d& p1,
														   const CXTPPoint3d& p2,
														   const CXTPChartColor& color,
														   int thickness)
	: m_p1(p1)
	, m_p2(p2)
	, m_color(color)
	, m_nThickness(thickness)
	, m_bProjected(FALSE)
{
	Set3D();
}

CXTPChartElement* CXTPChart3dLineDeviceCommand::HitTest(CPoint point,
														CXTPChartElement* pParent) const
{
	CXTPChartElement* pHitElement = NULL;

	if (m_bProjected
		&& CXTPMathUtils::HitTestLineSegment(m_pt2d1.x, m_pt2d1.y, m_pt2d2.x, m_pt2d2.y, point.x,
											 point.y, m_nThickness))
	{
		pHitElement = pParent;
	}

	return pHitElement;
}

void CXTPChart3dLineDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext, pDC);
	ASSERT(NULL != p3DDC);

	if (NULL != p3DDC)
	{
		m_bProjected = p3DDC->Project(m_p1, m_pt2d1) && p3DDC->Project(m_p2, m_pt2d2);
	}
}
