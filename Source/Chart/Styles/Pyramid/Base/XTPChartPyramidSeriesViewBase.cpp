// XTPChartPyramidSeriesViewBase.cpp
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
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"

#include "Chart/Styles/Pyramid/XTPChartPyramidSeriesPointView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesViewBase

IMPLEMENT_DYNAMIC(CXTPChartPyramidSeriesViewBase, CXTPChartSeriesView);

CXTPChartPyramidSeriesViewBase::CXTPChartPyramidSeriesViewBase(CXTPChartSeries* pSeries,
															   CXTPChartDiagramView* pDiagramView)
	: CXTPChartSeriesView(pSeries, pDiagramView)
	, m_pDomain(NULL)
	, m_nMaxLabelWidth(0)
{
}

CXTPChartPyramidSeriesViewBase::~CXTPChartPyramidSeriesViewBase()
{
	SAFE_DELETE(m_pDomain);
}

CXTPChartRectF CXTPChartPyramidSeriesViewBase::GetInnerBounds() const
{
	ASSERT(NULL != m_pDomain);
	return m_pDomain->GetInnerBounds();
}

CXTPChartDiagramDomain*
	CXTPChartPyramidSeriesViewBase::CreateDiagramDomain(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_rcBounds = rcBounds;

	CRect rcInnerBounds = rcBounds;

	CXTPChartPyramidSeriesStyleBase* pStyle = (CXTPChartPyramidSeriesStyleBase*)
												  m_pSeries->GetStyle();
	CXTPChartPyramidSeriesLabelBase* pLabel = (CXTPChartPyramidSeriesLabelBase*)pStyle->GetLabel();

	if (pLabel->IsVisible() && !pLabel->IsInside())
	{
		CSize sz(0, 0);
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartPyramidSeriesPointViewBase* pPointView = (CXTPChartPyramidSeriesPointViewBase*)
																  m_pPointsView->GetAt(i);

			CXTPChartString text(pLabel->GetPointLabel(pPointView->GetPoint()));
			CXTPChartTextPainter painter(pDC, text, pLabel);

			sz.cx = max(sz.cx, (INT)painter.GetSize().Width);
			sz.cy = max(sz.cy, (INT)painter.GetSize().Height);
		}

		int nLineLength = pStyle->GetLabel()->GetLineLength();
		int nGap		= 6;
		rcInnerBounds.DeflateRect(nGap + nLineLength + sz.cx, nGap + nLineLength + sz.cy);

		m_nMaxLabelWidth = nGap + sz.cx;
	}
	else
	{
		rcInnerBounds.DeflateRect(10, 10);
	}

	int nWidth = min(rcInnerBounds.Width(), rcInnerBounds.Height());

	if (nWidth < 0)
	{
		nWidth		  = 1;
		rcInnerBounds = CRect(rcBounds.CenterPoint(), CSize(nWidth, nWidth));
	}

	int nHeight = nWidth;

	CXTPChartRectF rcInnerBoundsF(float(rcInnerBounds.left + rcInnerBounds.right - nWidth) / 2,
								  float(rcInnerBounds.top + rcInnerBounds.bottom - nHeight) / 2,
								  (float)nWidth, (float)nHeight);

	return new CXTPChartDiagramDomain(rcInnerBoundsF);
}

void CXTPChartPyramidSeriesViewBase::CalculateLabelLayout(CXTPChartDeviceContext* pDC)
{
	for (int i = 0; i < m_pLabelsView->GetCount(); i++)
	{
		CXTPChartSeriesLabelView* pLabelView = (CXTPChartSeriesLabelView*)m_pLabelsView->GetAt(i);
		pLabelView->CalculateLayout(pDC);
	}
}

void CXTPChartPyramidSeriesViewBase::CalculateValues()
{
	double dSum = 0;
	int i;

	for (i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPyramidSeriesPointViewBase* pPointView = (CXTPChartPyramidSeriesPointViewBase*)
															  m_pPointsView->GetAt(i);

		CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

		double dValue = pPoint->GetValue(0);

		dSum += dValue;
	}

	if (dSum == 0)
		dSum = 1;

	for (i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPyramidSeriesPointViewBase* pPointView = (CXTPChartPyramidSeriesPointViewBase*)
															  m_pPointsView->GetAt(i);

		CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

		double dValue = pPoint->GetValue(0);

		pPointView->m_dValue = dValue >= 0 ? dValue / dSum : 0;
	}
}
