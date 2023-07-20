// XTPChartSeriesView.cpp
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
#include "Chart/XTPChartLegendView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartErrorBar.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartDiagramView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesView

IMPLEMENT_DYNAMIC(CXTPChartSeriesView, CXTPChartElementView);

CXTPChartSeriesView::CXTPChartSeriesView(CXTPChartSeries* pSeries,
										 CXTPChartDiagramView* pDiagramView)
	: CXTPChartElementView(pDiagramView->GetSeriesView())
	, m_pSeries(pSeries)
	, m_pDiagramView(pDiagramView)
	, m_pPointsView(NULL)
	, m_pLabelsView(NULL)
	, m_pErrorBarsView(NULL)
{
}

CXTPChartSeriesPointView* CXTPChartSeriesView::CreateSeriesPointView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pPoint);
	UNREFERENCED_PARAMETER(pParentView);

	return NULL;
}

void CXTPChartSeriesView::CreatePointsView(CXTPChartDeviceContext* pDC)
{
	CXTPChartSeriesPointCollection* pPoints = m_pSeries->GetPoints();

	m_pPointsView	= new CXTPChartElementView(this);
	m_pLabelsView	= m_pDiagramView->GetLabelsView();
	m_pErrorBarsView = new CXTPChartElementView(this);

	for (int i = 0; i < pPoints->GetCount(); i++)
	{
		CXTPChartSeriesPoint* pPoint = pPoints->GetAt(i);

		CXTPChartSeriesPointView* pPointView = CreateSeriesPointView(pDC, pPoint, m_pPointsView);

		// Error bar

		CXTPChartErrorBar* pErrorBar = m_pSeries->GetStyle()->GetErrorBar();

		if (NULL != pPointView && NULL != pErrorBar && pErrorBar->IsVisible())
		{
			pErrorBar->CreateView(pDC, pPointView, m_pErrorBarsView);
		}

		// Label

		CXTPChartSeriesLabel* pLabel = m_pSeries->GetStyle()->GetLabel();

		if ((NULL != pPointView) && (NULL != pLabel) && pLabel->IsVisible())
		{
			pLabel->CreateView(pDC, pPointView, m_pLabelsView);
		}
	}
}

CXTPChartSeriesStyle* CXTPChartSeriesView::GetStyle() const
{
	return m_pSeries->GetStyle();
}

CXTPChartDiagram* CXTPChartSeriesView::GetDiagram() const
{
	return m_pDiagramView->GetDiagram();
}

void CXTPChartSeriesView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartSeriesView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

CXTPChartString CXTPChartSeriesView::GetLegendName() const
{
	CXTPChartString strName = m_pSeries->GetLegendText();
	if (strName.IsEmpty())
		strName = m_pSeries->GetName();

	return strName;
}

CXTPChartDeviceCommand* CXTPChartSeriesView::CreateLegendDeviceCommand(
	CXTPChartDeviceContext* /*pDC*/, CRect /*rcBounds*/)
{
	return NULL;
}

void CXTPChartSeriesView::AddToLegend(CXTPChartLegendView* pView)
{
	if (!m_pSeries->IsLegendVisible())
		return;

	double minValue = m_pSeries->GetPoints()->GetMinimum(0);
	double maxValue = m_pSeries->GetPoints()->GetMaximum(0);

	if (m_pSeries->GetStyle()->IsColorEach())
	{
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartSeriesPointView* pPointView = (CXTPChartSeriesPointView*)m_pPointsView->GetAt(
				i);

			pView->AddItem(pPointView, minValue, maxValue);
		}
	}
	else
	{
		pView->AddItem(this, minValue, maxValue);
	}
}
