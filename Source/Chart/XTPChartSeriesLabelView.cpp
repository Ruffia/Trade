// XTPChartSeriesLabelView.cpp
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
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"

#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Appearance/XTPChartBorder.h"
#include "Appearance/XTPChartAppearance.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesLabelView

IMPLEMENT_DYNAMIC(CXTPChartSeriesLabelView, CXTPChartElementView);

CXTPChartSeriesLabelView::CXTPChartSeriesLabelView(CXTPChartSeriesLabel* pLabel,
												   CXTPChartSeriesPointView* pPointView,
												   CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pLabel	 = pLabel;
	m_pPointView = pPointView;
}

void CXTPChartSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

CXTPChartColor CXTPChartSeriesLabelView::GetActualTextColor() const
{
	if (!m_pLabel->GetTextColor().IsEmpty())
		return m_pLabel->GetTextColor();

	CXTPChartSeriesStyle* pStyle = (CXTPChartSeriesStyle*)m_pLabel->GetOwner();

	CXTPChartColor clrTextColor = pStyle->GetStyleAppearance()->GetLabelAppearance()->TextColor;
	if (!clrTextColor.IsEmpty())
		return clrTextColor;

	return m_pPointView->GetColor().GetDarkColor();
}

CXTPChartColor CXTPChartSeriesLabelView::GetActualBorderColor() const
{
	CXTPChartColor clrBorderColor = m_pLabel->GetBorder()->GetColor();

	if (clrBorderColor.IsEmpty())
	{
		CXTPChartSeriesStyle* pStyle = (CXTPChartSeriesStyle*)m_pLabel->GetOwner();

		clrBorderColor = pStyle->GetStyleAppearance()->GetLabelAppearance()->BorderColor;

		if (clrBorderColor.IsEmpty())
			clrBorderColor = m_pPointView->GetColor().GetDarkColor();
	}

	int nTransparency = m_pLabel->GetTransparency();

	if (nTransparency != 255)
	{
		return CXTPChartColor((BYTE)nTransparency, clrBorderColor.GetR(), clrBorderColor.GetG(),
							  clrBorderColor.GetB());
	}

	return clrBorderColor;
}

CXTPChartColor CXTPChartSeriesLabelView::GetActualConnectorColor() const
{
	CXTPChartColor clrLineColor = m_pLabel->GetLineColor();

	if (clrLineColor.IsEmpty())
	{
		CXTPChartSeriesStyle* pStyle = (CXTPChartSeriesStyle*)m_pLabel->GetOwner();

		clrLineColor = pStyle->GetStyleAppearance()->GetLabelAppearance()->ConnectorColor;

		if (clrLineColor.IsEmpty())
			clrLineColor = m_pPointView->GetColor().GetDarkColor();
	}

	int nTransparency = m_pLabel->GetTransparency();

	if (nTransparency != 255)
	{
		return CXTPChartColor((BYTE)nTransparency, clrLineColor.GetR(), clrLineColor.GetG(),
							  clrLineColor.GetB());
	}

	return clrLineColor;
}

CXTPChartSeriesPointView* CXTPChartSeriesLabelView::GetPointView() const
{
	return m_pPointView;
}
