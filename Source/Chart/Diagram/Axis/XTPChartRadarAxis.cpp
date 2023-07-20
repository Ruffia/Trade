// XTPChartRadarAxis.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Chart/XTPChartDefines.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/Diagram/RadarDiagram/XTPChartRadarDiagram.h"

#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"
#include "Chart/Diagram/Axis/XTPChartAxisRange.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxis.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisView.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisXView.h"
#include "Chart/Diagram/Axis/XTPChartRadarAxisYView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAxisX

IMPLEMENT_DYNAMIC(CXTPChartRadarAxisX, CXTPChartAxis)

CXTPChartRadarAxisX::CXTPChartRadarAxisX(CXTPChartRadarDiagram* pDiagram)
	: CXTPChartAxis(pDiagram)
{
	m_nMinorCount = 4;

	m_bInterlaced		  = TRUE;
	m_bDirectionClockwise = TRUE;

	m_pRange->SetSideMargins(FALSE);
	m_pRange->SetShowZeroLevel(FALSE);
}

BOOL CXTPChartRadarAxisX::IsVertical() const
{
	return FALSE;
}

BOOL CXTPChartRadarAxisX::IsValuesAxis() const
{
	return FALSE;
}

double CXTPChartRadarAxisX::CalcSideMarginsValue(double nMinValue, double nMaxValue) const
{
	if (m_nScaleType == xtpChartScaleNumerical)
		return (nMaxValue - nMinValue) / 15.0;
	return 0.5;
}

BOOL CXTPChartRadarAxisX::IsDirectionClockwise() const
{
	return m_bDirectionClockwise;
}

void CXTPChartRadarAxisX::SetDirectionClockwise(BOOL bDirectionClockwise)
{
	m_bDirectionClockwise = bDirectionClockwise;
	OnChartChanged();
}

CXTPChartAxisView* CXTPChartRadarAxisX::CreateView(CXTPChartDeviceContext* pDC,
												   CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRadarAxisXView* pView = new CXTPChartRadarAxisXView(this, pParentView);

	return pView;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAxisY

CXTPChartRadarAxisY::CXTPChartRadarAxisY(CXTPChartRadarDiagram* pDiagram)
	: CXTPChartAxis(pDiagram)
{
	m_nMinorCount = 2;

	m_pRange->SetShowZeroLevel(TRUE);
}

BOOL CXTPChartRadarAxisY::IsVertical() const
{
	return TRUE;
}

BOOL CXTPChartRadarAxisY::IsValuesAxis() const
{
	return TRUE;
}

double CXTPChartRadarAxisY::CalcSideMarginsValue(double nMinValue, double nMaxValue) const
{
	if (m_nScaleType != xtpChartScaleNumerical)
		return 0.5;

	double delta;
	if (nMinValue > 0 && nMaxValue > 0)
		delta = nMaxValue;
	else if (nMinValue < 0 && nMaxValue < 0)
		delta = -nMinValue;
	else
		delta = nMaxValue - nMinValue;

	return delta / 10.0;
}

CXTPChartAxisView* CXTPChartRadarAxisY::CreateView(CXTPChartDeviceContext* pDC,
												   CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRadarAxisYView* pView = new CXTPChartRadarAxisYView(this, pParentView);

	return pView;
}
