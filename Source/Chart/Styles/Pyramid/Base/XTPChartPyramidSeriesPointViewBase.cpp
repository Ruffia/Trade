// XTPChartPyramidSeriesPointViewBase.cpp
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"

#include "Chart/Appearance/XTPChartBorder.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesPointViewBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidDiagramSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChartPyramidSeriesPointViewBase, CXTPChartSeriesPointView);

CXTPChartPyramidSeriesPointViewBase::CXTPChartPyramidSeriesPointViewBase(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
	, m_dValue(0)
{
}

CXTPChartPyramidSeriesPointViewBase::CXTPChartPyramidSeriesPointViewBase(
	CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView, BOOL bAddToParent)
	: CXTPChartSeriesPointView(pPoint, pParentView, bAddToParent)
	, m_dValue(0)
{
}

CXTPChartPyramidSeriesPointViewBase::~CXTPChartPyramidSeriesPointViewBase()
{
}

CXTPChartColor CXTPChartPyramidSeriesPointViewBase::GetBorderActualColor() const
{
	CXTPChartPyramidSeriesStyleBase* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyleBase,
															  GetSeriesView()->GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = GetColor().GetDarkColor();
	return clr;
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesPointViewBase::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1	= GetColor();
	CXTPChartColor color2	= GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	return ((CXTPChartPyramidSeriesViewBase*)GetSeriesView())
		->CreateLegendDeviceCommand(pDC, rcBounds, color1, color2, clrBorder);
}
