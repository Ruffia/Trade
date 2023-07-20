// XTPChartLegendView.cpp
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
#include "Chart/XTPChartLegend.h"
#include "Chart/XTPChartLegendView.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartBorder.h"
#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Utils/XTPChartTextPainter.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartLegendView

CXTPChartLegendView::CXTPChartLegendView(CXTPChartLegend* pLegend,
										 CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pLegend = pLegend;
	m_rcBounds.SetRectEmpty();
}

CXTPChartLegendView::~CXTPChartLegendView()
{
}

CXTPChartDeviceCommand* CXTPChartLegendView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	int nCount = (int)m_arrItems.GetSize();
	if (nCount == 0)
		return 0;

	CXTPChartDeviceCommand* pDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dHitTestElementCommand(m_pLegend, m_rcBounds);

	int nBorderSize = m_pLegend->GetBorder()->IsVisible() ? m_pLegend->GetBorder()->GetThickness()
														  : 0;
	nBorderSize += 2;

	pDeviceCommand->AddChildCommand(m_pLegend->GetFillStyle()->CreateDeviceCommand(
		m_rcBounds, m_pLegend->GetActualBackgroundColor(), m_pLegend->GetActualBackgroundColor()));

	if (m_pLegend->GetBorder()->IsVisible())
	{
		pDeviceCommand->AddChildCommand(m_pLegend->GetBorder()->CreateInnerBorderDeviceCommand(
			m_rcBounds, m_pLegend->GetActualBorderColor()));
	}

	BOOL bVertical   = m_pLegend->GetDirection() == xtpChartLegendTopToBottom;
	int nColumnCount = m_pLegend->GetColumnCount();

	if (nColumnCount == 0)
		nColumnCount = bVertical ? 1 : nCount;

	if (nColumnCount > nCount)
		nColumnCount = nCount;

	int nRowCount = (nCount + nColumnCount - 1) / nColumnCount;

	int nLeft = m_rcBounds.left + nBorderSize;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		int nColumnWidth = 0;

		int nTop = m_rcBounds.top + nBorderSize;

		for (int nRow = 0; nRow < nRowCount; nRow++)
		{
			int i = bVertical ? nColumn * nRowCount + nRow : nRow * nColumnCount + nColumn;
			if (i >= nCount)
				continue;

			CXTPChartLegendItemView* pItem = m_arrItems[i].pView;

			CXTPChartString strName = pItem->GetLegendName();

			if (strName.GetLength() == 0)
				continue;

			CXTPChartTextPainter painter(pDC, strName, m_pLegend);

			CXTPChartSizeF szText = painter.GetSize();
			CXTPChartSizeF sz	 = szText;

			if (m_pLegend->IsMarkerVisible())
			{
				if (sz.Height < static_cast<float>(m_pLegend->GetMarkerSize().cy))
					sz.Height = static_cast<float>(m_pLegend->GetMarkerSize().cy);
			}

			int nTextLeft = nLeft;

			if (m_pLegend->IsMarkerVisible())
			{
				CSize szMarker = m_pLegend->GetMarkerSize();

				CRect rcMarker(CPoint(nLeft, nTop + (int)sz.Height / 2 - szMarker.cy / 2),
							   szMarker);

				pDeviceCommand->AddChildCommand(pItem->CreateLegendDeviceCommand(pDC, rcMarker));

				nTextLeft += szMarker.cx + 2;
				sz.Width += static_cast<float>(szMarker.cx + 2);
			}

			painter.SetLocation(CXTPPoint2f(
				static_cast<float>(nTextLeft),
				static_cast<float>(static_cast<float>(nTop) + sz.Height / 2 - szText.Height / 2)));
			pDeviceCommand->AddChildCommand(
				painter.CreateDeviceCommand(pDC, m_pLegend->GetActualTextColor()));

			nColumnWidth = max(nColumnWidth, (int)sz.Width + 2);

			nTop += (int)sz.Height + 2;
		}
		nLeft += nColumnWidth;
	}

	return pDeviceCommand;
}

CSize CXTPChartLegendView::CalcSize(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	int nHeight = 0;
	int nWidth  = 0;

	BOOL bVertical   = m_pLegend->GetDirection() == xtpChartLegendTopToBottom;
	int nColumnCount = m_pLegend->GetColumnCount();
	int nBorderSize  = m_pLegend->GetBorder()->IsVisible() ? m_pLegend->GetBorder()->GetThickness()
														  : 0;
	nBorderSize += 2;

	int nCount = (int)m_arrItems.GetSize();
	if (nCount == 0)
		return CSize(0, 0);

	if (nColumnCount == 0)
		nColumnCount = bVertical ? 1 : nCount;

	if (nColumnCount > nCount)
		nColumnCount = nCount;

	int nRowCount = (nCount + nColumnCount - 1) / nColumnCount;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		int nColumnWidth  = 0;
		int nColumnHeight = 0;

		for (int nRow = 0; nRow < nRowCount; nRow++)
		{
			int i = bVertical ? nColumn * nRowCount + nRow : nRow * nColumnCount + nColumn;
			if (i >= nCount)
				continue;

			CXTPChartLegendItemView* pItem = m_arrItems[i].pView;

			CXTPChartString strName = pItem->GetLegendName();

			if (strName.GetLength() == 0)
				continue;

			CXTPChartTextPainter painter(pDC, strName, m_pLegend);
			CXTPChartSizeF sz = painter.GetSize();

			if (m_pLegend->IsMarkerVisible())
			{
				if (sz.Height < static_cast<float>(m_pLegend->GetMarkerSize().cy))
					sz.Height = static_cast<float>(m_pLegend->GetMarkerSize().cy);

				sz.Width += static_cast<float>(m_pLegend->GetMarkerSize().cx + 2);
			}

			nColumnWidth = max(nColumnWidth, (int)sz.Width + 2);

			if (nRow)
				nColumnHeight += 2;
			nColumnHeight += (int)sz.Height;
		}

		nWidth += nColumnWidth;
		nHeight = max(nHeight, nColumnHeight);
	}

	return CSize(nWidth + 2 * nBorderSize, nHeight + 2 * nBorderSize);
}

void CXTPChartLegendView::CalculateView(CXTPChartDeviceContext* pDC, CRect& rcChart)
{
	CSize sz = CalcSize(pDC);
	CPoint pt;

	int nHorizontalAlignment = m_pLegend->GetHorizontalAlignment();
	int nVerticalAlignment   = m_pLegend->GetVerticalAlignment();

	BOOL bInside = nHorizontalAlignment != xtpChartLegendNearOutside
				   && nHorizontalAlignment != xtpChartLegendFarOutside
				   && nVerticalAlignment != xtpChartLegendNearOutside
				   && nVerticalAlignment != xtpChartLegendFarOutside;

	if (nHorizontalAlignment == xtpChartLegendNear
		|| nHorizontalAlignment == xtpChartLegendNearOutside)
	{
		pt.x = rcChart.left + (bInside ? 10 : 0);

		if (nHorizontalAlignment == xtpChartLegendNearOutside)
			rcChart.left += sz.cx + 10;
	}
	else if (nHorizontalAlignment == xtpChartLegendCenter)
	{
		pt.x = (rcChart.left + rcChart.right - sz.cx) / 2;
	}
	else
	{
		pt.x = rcChart.right - sz.cx - (bInside ? 10 : 0);

		if (nHorizontalAlignment == xtpChartLegendFarOutside)
			rcChart.right -= sz.cx + 10;
	}

	if (nVerticalAlignment == xtpChartLegendNear || nVerticalAlignment == xtpChartLegendNearOutside)
	{
		pt.y = rcChart.top + (bInside ? 10 : 0);

		if (nVerticalAlignment == xtpChartLegendNearOutside)
			rcChart.top += sz.cy + 10;
	}
	else if (nVerticalAlignment == xtpChartLegendCenter)
	{
		pt.y = (rcChart.top + rcChart.bottom - sz.cy) / 2;
	}
	else
	{
		pt.y = rcChart.bottom - sz.cy - (bInside ? 10 : 0);

		if (nVerticalAlignment == xtpChartLegendFarOutside)
			rcChart.bottom -= sz.cy + 10;
	}

	pt += m_pLegend->GetOffset();

	m_rcBounds = CRect(pt, sz);
}

void CXTPChartLegendView::AddItem(CXTPChartLegendItemView* pItem, double minimum, double maximum)
{
	CHARTLEGEND_ITEM legendItem;
	legendItem.pView = pItem;
	legendItem.min   = minimum;
	legendItem.max   = maximum;
	legendItem.str   = pItem->GetLegendName();

	m_arrItems.Add(legendItem);

	typedef int(AFX_CDECL * LEGENDITEMVIEWCOMPAREFUNC)(const void*, const void*);

	switch (m_pLegend->GetSortDirection())
	{
		case xtpChartLegendMinToMax:
			qsort(m_arrItems.GetData(), static_cast<size_t>(m_arrItems.GetSize()),
				  sizeof(CHARTLEGEND_ITEM), (LEGENDITEMVIEWCOMPAREFUNC)CompareFuncMinToMax);
			break;

		case xtpChartLegendMaxToMin:
			qsort(m_arrItems.GetData(), static_cast<size_t>(m_arrItems.GetSize()),
				  sizeof(CHARTLEGEND_ITEM), (LEGENDITEMVIEWCOMPAREFUNC)CompareFuncMaxToMin);
			break;

		case xtpChartLegendAsc:
			qsort(m_arrItems.GetData(), static_cast<size_t>(m_arrItems.GetSize()),
				  sizeof(CHARTLEGEND_ITEM), (LEGENDITEMVIEWCOMPAREFUNC)CompareFuncAsc);
			break;

		case xtpChartLegendDesc:
			qsort(m_arrItems.GetData(), static_cast<size_t>(m_arrItems.GetSize()),
				  sizeof(CHARTLEGEND_ITEM), (LEGENDITEMVIEWCOMPAREFUNC)CompareFuncDesc);
			break;

		case xtpChartLegendNone:
		default: break;
	}
}

int CXTPChartLegendView::CompareFuncAsc(const CHARTLEGEND_ITEM* pItem1,
										const CHARTLEGEND_ITEM* pItem2)
{
	CString str1((*pItem1).pView->GetLegendName());
	CString str2((*pItem2).pView->GetLegendName());

	return str1.CompareNoCase(str2);
}

int CXTPChartLegendView::CompareFuncDesc(const CHARTLEGEND_ITEM* pItem1,
										 const CHARTLEGEND_ITEM* pItem2)
{
	CString str1((*pItem1).pView->GetLegendName());
	CString str2((*pItem2).pView->GetLegendName());

	return str1.CompareNoCase(str2) < 0;
}

int CXTPChartLegendView::CompareFuncMinToMax(const CHARTLEGEND_ITEM* pItem1,
											 const CHARTLEGEND_ITEM* pItem2)
{
	if ((*pItem1).min < (*pItem2).min)
		return -1;
	if ((*pItem1).min > (*pItem2).min)
		return 1;

	return 0;
}

int CXTPChartLegendView::CompareFuncMaxToMin(const CHARTLEGEND_ITEM* pItem1,
											 const CHARTLEGEND_ITEM* pItem2)
{
	if ((*pItem1).max < (*pItem2).max)
		return 1;
	if ((*pItem1).max > (*pItem2).max)
		return -1;

	return 0;
}