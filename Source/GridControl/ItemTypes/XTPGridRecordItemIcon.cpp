// XTPGridRecordItemIcon.cpp : implementation of the CXTPGridRecordItemIcon class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/ItemTypes/XTPGridRecordItemIcon.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPGridRecordItemIcon, CXTPGridRecordItem,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemIcon::CXTPGridRecordItemIcon()
	: CXTPGridRecordItem()
{
	SetAlignment(xtpColumnIconTop | xtpColumnIconCenter | xtpColumnTextWordBreak
				 | xtpColumnTextCenter);
}

int CXTPGridRecordItemIcon::Draw(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs)
{
	if (pDrawArgs == NULL)
		return CXTPGridRecordItem::Draw(pDrawArgs);

	CXTPGridControl* pControl = DYNAMIC_DOWNCAST(CXTPGridControl, pDrawArgs->pControl);
	if (pControl && pControl->IsIconView())
	{
		pDrawArgs->rcItem = pDrawArgs->pRow->GetRect();
		pDrawArgs->rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	}

	return CXTPGridRecordItem::Draw(pDrawArgs);
}

void CXTPGridRecordItemIcon::OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
										   XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	if (pDrawArgs == NULL)
		return;

	CXTPGridControl* pControl = DYNAMIC_DOWNCAST(CXTPGridControl, pDrawArgs->pControl);
	if (pControl && pControl->IsIconView())
	{
		// Adjust the rect for the text.
		pDrawArgs->rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		pDrawArgs->rcItem.top += pControl->m_iIconHeight;

		CXTPGridPaintManager* pPaintManager = pControl->GetPaintManager();

		const UINT c_uiDrawFlags = UINT(pDrawArgs->nTextAlign | DT_CENTER | DT_NOPREFIX
										| DT_WORDBREAK | DT_EDITCONTROL | DT_END_ELLIPSIS
										| DT_WORD_ELLIPSIS);

		// Get rect for text
		CRect rcItem(pDrawArgs->rcItem);
		rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0);
		rcItem = pPaintManager->CalculateMaxTextRect(pDrawArgs->pDC, pMetrics->strText, rcItem,
													 TRUE, TRUE, c_uiDrawFlags);

		int nWidth = min(rcItem.Width() + XTP_DPI_X(4), pDrawArgs->rcItem.Width());

		// Center the rect.
		int nLeft = (pDrawArgs->rcItem.left + pDrawArgs->rcItem.right - nWidth) / 2;

		rcItem.left  = nLeft;
		rcItem.right = rcItem.left + nWidth;

		if (rcItem.bottom >= pDrawArgs->rcItem.bottom)
			rcItem.bottom -= XTP_DPI_Y(2);

		pDrawArgs->rcItem.left  = rcItem.left;
		pDrawArgs->rcItem.right = rcItem.right;

		// Draw the selection
		if (pDrawArgs->pRow->IsSelected() && (pDrawArgs->pDC && !pDrawArgs->pDC->IsPrinting()))
		{
			COLORREF clrBackground = 0;

			if (pControl->HasFocus())
			{
				pMetrics->clrForeground = pPaintManager->m_clrHighlightText;
				pDrawArgs->pDC->SetTextColor(pMetrics->clrForeground);
				clrBackground = pPaintManager->m_clrHighlight;
			}
			else if (!pPaintManager->m_bHideSelection)
			{
				pMetrics->clrForeground = pPaintManager->m_clrSelectedRowText;
				pDrawArgs->pDC->SetTextColor(pMetrics->clrForeground);
				clrBackground = pPaintManager->m_clrSelectedRow;
			}

			// fill select rect
			pDrawArgs->pDC->FillSolidRect(rcItem, clrBackground);
		}

		if (pDrawArgs->pRow->IsFocused() && pControl->HasFocus() && pControl->IsRowFocusVisible())
		{
			rcItem.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pPaintManager->DrawFocusedRow(pDrawArgs->pDC, rcItem);
		}

		pDrawArgs->nTextAlign |= c_uiDrawFlags;
	}

	CXTPGridRecordItem::OnDrawCaption(pDrawArgs, pMetrics);
}

void CXTPGridRecordItemIcon::GetCaptionRect(XTP_GRIDRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	if (pDrawArgs == NULL)
		return;

	CXTPGridControl* pControl = DYNAMIC_DOWNCAST(CXTPGridControl, pDrawArgs->pControl);
	if (pControl && pControl->IsIconView())
	{
		// Adjust the rect for the text.
		pDrawArgs->rcItem.CopyRect(pDrawArgs->pRow->GetRect());
		pDrawArgs->rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		pDrawArgs->rcItem.top += pControl->m_iIconHeight;

		// The left coming in here is to the right of the icon, we need to get back to the
		// beginning.
		pDrawArgs->rcItem.left = pDrawArgs->pRow->GetRect().left + XTP_DPI_X(5);
		pDrawArgs->rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0);

		rcItem = pDrawArgs->rcItem;
		return;
	}
	else
	{
		CXTPGridRecordItem::GetCaptionRect(pDrawArgs, rcItem);
	}
}

void CXTPGridRecordItemIcon::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	if (pItemArgs == NULL)
		return;

	CXTPGridRecordItem::OnEditChanged(pItemArgs, szText);

	int r				 = pItemArgs->pRow->GetIndex();
	int i				 = pItemArgs->pItem->GetIndex();
	CXTPGridRecord* pRec = pItemArgs->pRow->GetRecord();
	if (pRec)
	{
		pRec->UpdateRecordField(r, i, szText);
	}
}
