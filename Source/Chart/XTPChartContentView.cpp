// XTPChartContentView.cpp
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
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartContent.h"
#include "Chart/XTPChartContentView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartTitle.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/XTPChartLegend.h"
#include "Chart/XTPChartLegendView.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartBorder.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartContentView

CXTPChartContentView::CXTPChartContentView(CXTPChartContainer* pContainer,
										   CXTPChartContent* pContent)
	: CXTPChartElementView(pContainer)
{
	m_pContent = pContent;
	m_rcBounds.SetRectEmpty();

	m_pLegendView  = NULL;
	m_pTitlesView  = NULL;
	m_pDiagramView = NULL;
}

CXTPChartDeviceCommand* CXTPChartContentView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pContent, m_rcBounds);

	pCommand->AddChildCommand(pDC->GetDrawingObjectFactory()->CreateFillBackgroundDeviceCommand(
		m_rcBounds, m_pContent->GetActualBackgroundColor()));

	CXTPChartDeviceCommand* pDrawingType = pCommand->AddChildCommand(new CXTPChartDeviceCommand());

	if (m_pContent->GetBorder()->IsVisible())
	{
		pDrawingType->AddChildCommand(m_pContent->GetBorder()->CreateInnerBorderDeviceCommand(
			m_rcBounds, m_pContent->GetActualBorderColor()));
	}

	if (m_pDiagramView)
	{
		pDrawingType->AddChildCommand(m_pDiagramView->CreateDeviceCommand(pDC));
	}

	if (m_pLegendView)
	{
		pDrawingType->AddChildCommand(m_pLegendView->CreateDeviceCommand(pDC));
	}

	if (m_pTitlesView)
	{
		pDrawingType->AddChildCommand(m_pTitlesView->CreateDeviceCommand(pDC));
	}

	return pCommand;
}

void CXTPChartContentView::CreateView(CXTPChartDeviceContext* pDC)
{
	m_pTitlesView = new CXTPChartElementView(this);

	m_pLegendView = m_pContent->GetLegend()->CreateView(pDC, this);

	m_pDiagramView = new CXTPChartElementView(this);

	for (int nPanel = 0; nPanel < m_pContent->GetPanels()->GetCount(); nPanel++)
	{
		CXTPChartPanel* pPanel = m_pContent->GetPanels()->GetAt(nPanel);

		CXTPChartDiagram* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram, pPanel);
		if (pDiagram == NULL)
			continue;

		CXTPChartDiagramView* pDiagramView = pDiagram->CreateView(pDC, m_pDiagramView);

		if (pDiagramView)
		{
			pDiagramView->CreateView(pDC);

			for (int i = 0; i < m_pContent->GetSeries()->GetCount(); i++)
			{
				CXTPChartSeries* pSeries = m_pContent->GetSeries()->GetAt(i);
				if (!pSeries->IsVisible())
					continue;

				if (pSeries->GetDiagram() != pDiagram)
					continue;

				CXTPChartSeriesView* pSeriesView = pSeries->GetStyle()->CreateView(pSeries,
																				   pDiagramView);

				if (pSeriesView)
				{
					pSeriesView->CreatePointsView(pDC);

					if (m_pLegendView)
					{
						pSeriesView->AddToLegend(m_pLegendView);
					}
				}
			}

			pDiagramView->UpdateRange(pDC);
		}
	}

	m_pContent->GetTitles()->CreateView(pDC, m_pTitlesView);
}

CXTPChartDiagramView* CXTPChartContentView::HitTestDiagramView(CPoint pt) const
{
	for (int i = 0; i < m_pDiagramView->GetCount(); i++)
	{
		CXTPChartDiagramView* pDiagramView = (CXTPChartDiagramView*)m_pDiagramView->GetAt(i);

		if (pDiagramView->GetBounds().PtInRect(pt))
			return pDiagramView;
	}
	return NULL;
}

void CXTPChartContentView::CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_rcBounds = rcBounds;

	if (m_pContent->GetBorder()->IsVisible())
		rcBounds.DeflateRect(m_pContent->GetBorder()->GetThickness(),
							 m_pContent->GetBorder()->GetThickness());

	m_pContent->GetTitles()->CalculateView(pDC, rcBounds, m_pTitlesView);

	rcBounds.DeflateRect(10, 10, 10, 10);

	if (m_pLegendView)
	{
		m_pLegendView->CalculateView(pDC, rcBounds);
	}

	int nCount = m_pDiagramView->GetCount();
	if (nCount == 0)
		return;

	int nGap = m_pContent->GetPanelDistance();

	int nSize = m_pContent->GetPanelDirection() == xtpChartPanelHorizontal
					? (rcBounds.Width() - nGap * (nCount - 1)) / nCount
					: (rcBounds.Height() - nGap * (nCount - 1)) / nCount;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartDiagramView* pDiagramView = (CXTPChartDiagramView*)m_pDiagramView->GetAt(i);

		CRect rcDiagram = m_pContent->GetPanelDirection() == xtpChartPanelHorizontal
							  ? CRect(rcBounds.left + i * (nSize + nGap), rcBounds.top,
									  rcBounds.left + i * (nSize + nGap) + nSize, rcBounds.bottom)
							  : CRect(rcBounds.left, rcBounds.top + i * (nSize + nGap),
									  rcBounds.right, rcBounds.top + i * (nSize + nGap) + nSize);

		pDiagramView->CalculateView(pDC, rcDiagram);
	}
}

CXTPChartLegendView* CXTPChartContentView::GetLegendView() const
{
	return m_pLegendView;
}
