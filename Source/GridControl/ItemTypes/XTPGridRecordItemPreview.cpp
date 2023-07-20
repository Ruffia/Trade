// XTPGridRecordItemPreview.cpp : implementation of the CXTPGridRecordItemPreview class.
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
#include "Common/XTPMarkupRender.h"
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
#include "GridControl/ItemTypes/XTPGridRecordItemPreview.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPGridRecordItemPreview, CXTPGridRecordItem,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemPreview::CXTPGridRecordItemPreview(LPCTSTR szPreviewText)
	: CXTPGridRecordItem()
{
	SetPreviewText(szPreviewText);
}

void CXTPGridRecordItemPreview::SetPreviewText(LPCTSTR strPreviewText)
{
	m_strPreviewText = strPreviewText;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	CXTPMarkupContext* pMarkupContext = m_pRecord ? m_pRecord->GetMarkupContext() : NULL;

	if (pMarkupContext)
	{
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, m_strPreviewText);
	}
}

void CXTPGridRecordItemPreview::SetCaption(LPCTSTR strCaption)
{
	SetPreviewText(strCaption);
}

CString CXTPGridRecordItemPreview::GetCaption(CXTPGridColumn* pColumn)
{
	UNREFERENCED_PARAMETER(pColumn);
	return GetPreviewText();
}

void CXTPGridRecordItemPreview::GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
											   XTP_GRIDRECORDITEM_METRICS* pItemMetrics)
{
	ASSERT(NULL != pDrawArgs);
	ASSERT(NULL != pItemMetrics);

	pItemMetrics->clrForeground = pDrawArgs->pControl->GetPaintManager()->m_clrPreviewText;

	if (m_clrText != XTP_GRID_COLOR_DEFAULT)
		pItemMetrics->clrForeground = m_clrText;

	pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_xtpFontPreview;
}

void CXTPGridRecordItemPreview::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	UNREFERENCED_PARAMETER(pItemArgs);
	m_strPreviewText = szText;
}

void CXTPGridRecordItemPreview::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT_VALID(pPX);

	CXTPGridRecordItem::DoPropExchange(pPX);

	PX_String(pPX, _T("PreviewText"), m_strPreviewText);

	if (pPX->IsLoading())
	{
		XTPMarkupReleaseElement(m_pMarkupUIElement);

		CXTPMarkupContext* pMarkupContext = m_pRecord ? m_pRecord->GetMarkupContext() : NULL;
		if (pMarkupContext)
		{
			m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, m_strPreviewText);
		}
	}
}

int CXTPGridRecordItemPreview::GetPreviewHeight(CDC* pDC, CXTPGridRow* pRow, int nWidth)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pRow);

	int nHeight							 = 0;
	XTP_GRIDRECORDITEM_METRICS* pMetrics = new XTP_GRIDRECORDITEM_METRICS;
	pMetrics->strText					 = GetPreviewText();
	pRow->FillMetrics(NULL, this, pMetrics);

	CString strPreviewText = pMetrics->strText;

	if (strPreviewText.IsEmpty())
	{
		pMetrics->InternalRelease();
		return 0;
	}

	CXTPGridControl* pControl = pRow->GetControl();

	int nIndentWidth = pControl->GetHeaderIndent();

	CRect& rcIndent = pControl->GetPaintManager()->m_rcPreviewIndent;

	CRect rcPreviewItem(nIndentWidth + rcIndent.left, 0, nWidth - rcIndent.right, 0);

	if (m_pMarkupUIElement)
	{
		// Calculate Markup item height
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement),
								(HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);
		CSize szMarkup = XTPMarkupMeasureElement(m_pMarkupUIElement, rcPreviewItem.Width(),
												 INT_MAX);
		nHeight		   = szMarkup.cy + rcIndent.top + rcIndent.bottom;
	}
	else
	{
		// Calculate height of a usual text
		CXTPFontDC font(pDC, pMetrics->pFont);

		int nMaxPreviewLines = pControl->GetPaintManager()->GetPreviewLinesCount(pDC, rcPreviewItem,
																				 strPreviewText);
		int nFontHeight		 = pDC->GetTextExtent(_T(" "), 1).cy;

		nHeight = nFontHeight * nMaxPreviewLines + rcIndent.top + rcIndent.bottom;
	}

	// Cleanup
	pMetrics->InternalRelease();
	return nHeight;
}

void CXTPGridRecordItemPreview::GetCaptionRect(XTP_GRIDRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(NULL != pDrawArgs);
	ASSERT(NULL != pDrawArgs->pControl);

	if (NULL == pDrawArgs->pControl)
		return;

	CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;
	rcItem.DeflateRect(rcIndent.left - XTP_DPI_X(2), -XTP_DPI_Y(1), rcIndent.right,
					   -rcIndent.bottom);
}

void CXTPGridRecordItemPreview::OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
											  XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	ASSERT(NULL != pDrawArgs);
	ASSERT(NULL != pDrawArgs->pControl);

	if (NULL == pDrawArgs->pControl)
		return;

	if (!pDrawArgs->pControl->GetPaintManager()->OnDrawAction(pDrawArgs))
		return;

	CString strText = pMetrics->strText;

	// draw item text
	if (!strText.IsEmpty())
	{
		CRect rcItem(pDrawArgs->rcItem);

		CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;

		rcItem.DeflateRect(rcIndent.left, rcIndent.top, rcIndent.right, rcIndent.bottom);

		if (m_pMarkupUIElement)
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement),
									(HFONT)pMetrics->pFont->GetSafeHandle(),
									pMetrics->clrForeground);

			XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width(), INT_MAX);

			XTPMarkupRenderElement(m_pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
		}
		else
		{
			CXTPFontDC font(pDrawArgs->pDC, pMetrics->pFont);
			pDrawArgs->pDC->DrawText(strText, rcItem, DT_WORDBREAK | DT_LEFT | DT_NOPREFIX);
		}
	}
}

void CXTPGridRecordItemPreview::OnBeginEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	if (m_pControl && m_pControl->IsPreviewAllowEdit())
	{
		CXTPGridRecordItem::OnBeginEdit(pItemArgs);
	}
}
