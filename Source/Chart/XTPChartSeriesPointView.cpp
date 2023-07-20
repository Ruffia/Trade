// XTPChartSeriesPointView.cpp
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

#include "StdAfx.h"

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Utils/XTPChartNumberFormat.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartPalette.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesPointView

IMPLEMENT_DYNAMIC(CXTPChartSeriesPointView, CXTPChartElementView);

CXTPChartSeriesPointView::CXTPChartSeriesPointView(CXTPChartSeriesPoint* pPoint,
												   CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pPoint		 = pPoint;
	m_dInternalValue = m_pPoint->GetValue(0);
}

CXTPChartSeriesPointView::CXTPChartSeriesPointView(CXTPChartSeriesPoint* pPoint,
												   CXTPChartElementView* pParentView,
												   BOOL bAddToParent)
	: CXTPChartElementView(pParentView, bAddToParent)
{
	m_pPoint		 = pPoint;
	m_dInternalValue = m_pPoint->GetValue(0);
}

CXTPChartSeriesView* CXTPChartSeriesPointView::GetSeriesView() const
{
	return STATIC_DOWNCAST(CXTPChartSeriesView, GetParentView()->GetParentView());
}

CXTPChartColor CXTPChartSeriesPointView::PrepareColor(CXTPChartColor color) const
{
	if (!Is3D())
		return color;

	float fAlpha = (1.f - static_cast<float>(GetSeriesView()->GetStyle()->GetTransparency()) / 255);
	return CXTPChartColor(static_cast<BYTE>(fAlpha * static_cast<float>(color.GetA())),
						  color.GetR(), color.GetG(), color.GetB());
}

CXTPChartColor CXTPChartSeriesPointView::GetColor() const
{
	CXTPChartSeriesView* pSeriesView = GetSeriesView();

	if (!m_pPoint->GetColor().IsEmpty())
		return PrepareColor(m_pPoint->GetColor());

	if (pSeriesView->GetStyle()->IsColorEach())
		return PrepareColor(pSeriesView->GetSeries()->GetAppearance()->GetPalette()->GetColor(
			m_pPoint->GetPaletteIndex()));

	return PrepareColor(pSeriesView->m_pSeries->GetColor());
}

CXTPChartColor CXTPChartSeriesPointView::GetColor2() const
{
	CXTPChartSeriesView* pSeriesView = GetSeriesView();

	if (!m_pPoint->GetColor().IsEmpty())
		return PrepareColor(CXTPChartAppearance::GetLightColor(m_pPoint->GetColor()));

	if (pSeriesView->GetStyle()->IsColorEach())
		return PrepareColor(pSeriesView->GetSeries()->GetAppearance()->GetPalette()->GetColor2(
			m_pPoint->GetPaletteIndex()));

	return PrepareColor(pSeriesView->m_pSeries->GetColor2());
}

CXTPChartString CXTPChartSeriesPointView::GetLegendName() const
{
	CXTPChartString strText = m_pPoint->GetLegendText();
	if (!strText.IsEmpty())
		return strText;

	double dValue = m_pPoint->GetValue(
		m_pPoint->GetSeries()->GetStyle()->GetSeriesPointValueIndex());

	return m_pPoint->GetSeries()->GetPointLegendFormat()->FormatPoint(m_pPoint, dValue);
}

CXTPChartDeviceCommand*
	CXTPChartSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	return GetSeriesView()->CreateLegendDeviceCommand(pDC, rcBounds);
}
