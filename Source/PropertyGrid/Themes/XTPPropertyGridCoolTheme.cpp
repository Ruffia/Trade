// XTPPropertyGridCoolTheme.cpp : implementation of the CXTPPropertyGridCoolTheme class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridToolBar.h"
#include "PropertyGrid/XTPPropertyGrid.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridCoolTheme

CXTPPropertyGridCoolTheme::CXTPPropertyGridCoolTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
}

void CXTPPropertyGridCoolTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
}

void CXTPPropertyGridCoolTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, TRUE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);

	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrLine   = m_pMetrics->m_clrLine;
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);

	if (pItem->IsCategory())
	{
		dc.FillSolidRect(rc, clrLine);

		CFont* pFont = GetItemFont(pItem, FALSE);
		font.SetFont(pFont);
		dc.SetTextColor(GetItemTextColor(pItem, FALSE));

		int nIndent = XTP_DPI_X(XTP_PGI_EXPAND_BORDER) * (pItem->GetIndent() + 1);

		if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(CRect(nIndent - 1, rc.top, rc.right, rc.bottom), m_clrHighlight);
		}

		CRect rcText(nIndent + XTP_DPI_X(3), rc.top, rc.right, rc.bottom - XTP_DPI_Y(1));

		CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(FALSE, FALSE);
		if (pMetrics && pMetrics->m_nImage != -1)
		{
			CXTPImageManagerIcon* pImage =
				m_pGrid->GetImageManager()->GetImage(XTPToUInt(pMetrics->m_nImage), 0);

			if (pImage)
			{
				CSize sz;
				if (pImage->IsVectorIcon())
					sz = CSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
				else
					sz = CSize(pImage->GetWidth(), pImage->GetHeight());

				sz.cx = min(sz.cx, rc.Width());
				sz.cy = min(sz.cy, rc.Height());

				CPoint pt(rcText.left, rcText.CenterPoint().y - sz.cy / 2);
				pImage->Draw(&dc, pt, sz);

				rcText.left += sz.cx + XTP_DPI_X(4);
			}
		}

		if (pItem->GetMarkupCaption())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
									(HFONT)pFont->GetSafeHandle(), dc.GetTextColor());

			XTPMarkupMeasureElement(pItem->GetMarkupCaption(), rcText.Width(), rcText.Height());
			XTPMarkupRenderElement(pItem->GetMarkupCaption(), dc.GetSafeHdc(), rcText);
		}
		else
		{
			dc.DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}

		dc.FillSolidRect((pItem->GetIndent() + (pItem->IsExpanded() ? 1 : 0))
								 * XTP_DPI_X(XTP_PGI_EXPAND_BORDER)
							 - 1,
						 rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1), clrShadow);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcText);

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left
									 + pCategory->GetIndent() * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
								 rc.top, XTP_DPI_X(XTP_PGI_EXPAND_BORDER), rc.Height(), clrLine);
				dc.FillSolidRect(
					rc.left + (pCategory->GetIndent() + 1) * XTP_DPI_X(XTP_PGI_EXPAND_BORDER) - 1,
					rc.top, 1, rc.Height() + 2, clrShadow);
			}
			pCategory = pCategory->GetParentItem();
		}
	}
	else
	{
		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - XTP_DPI_Y(1));

		if (GetItemsBorderHeight() > XTP_DPI_Y(1))
		{
			rcValue.top += GetItemsBorderHeight() / 2;
			rcValue.bottom -= GetItemsBorderHeight() / 2 + GetItemsBorderHeight() % 2;
		}

		DrawItemValue(&dc, pItem, rcValue);

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));
		font.SetFont(GetItemFont(pItem, FALSE));

		BOOL bSelected = (0 != (lpDrawItemStruct->itemState & ODS_SELECTED));
		BOOL bFocused  = bSelected && CWnd::GetFocus()
						&& ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView)
							|| (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(rcCaption, m_clrHighlight);
		}
		else if (bSelected && !m_pGrid->m_bHideSelection)
		{
			dc.FillSolidRect(rcCaption, clrLine);
		}

		CXTPPenDC pen(dc, clrLine);
		if (GetItemsBorderHeight() > XTP_DPI_Y(1))
		{
			int nTop	= GetItemsBorderHeight() / 2;
			int nBottom = GetItemsBorderHeight() / 2 + GetItemsBorderHeight() % 2;
			dc.FillSolidRect(rc.left, rc.top, rc.right, nTop, m_pMetrics->m_clrLine);
			dc.FillSolidRect(rc.left, rc.bottom - nBottom, rc.right, nBottom,
							 m_pMetrics->m_clrLine);
			dc.FillSolidRect(rcCaption.right - nBottom, rc.top, GetItemsBorderHeight(), rc.Height(),
							 m_pMetrics->m_clrLine);
		}
		else
		{
			dc.MoveTo(0, rc.bottom - XTP_DPI_Y(1));
			dc.LineTo(rc.right, rc.bottom - XTP_DPI_Y(1));
			dc.MoveTo(rcCaption.right, rc.top);
			dc.LineTo(rcCaption.right, rc.bottom - XTP_DPI_Y(1));
		}

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left
									 + pCategory->GetIndent() * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
								 rc.top, XTP_DPI_X(XTP_PGI_EXPAND_BORDER), rc.Height(), clrLine);
				dc.FillSolidRect(
					rc.left + (pCategory->GetIndent() + 1) * XTP_DPI_X(XTP_PGI_EXPAND_BORDER) - 1,
					rc.top, 1, rc.Height() + 2, clrShadow);
			}
			pCategory = pCategory->GetParentItem();
		}

		DrawItemCaption(&dc, pItem, rcCaption);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);
	}

	CXTPPropertyGridItem* pNext = (CXTPPropertyGridItem*)pView->GetItemDataPtr(
		XTPToInt(lpDrawItemStruct->itemID + 1));
	if (((ULONG_PTR)pNext == (ULONG_PTR)(-1)))
	{
		dc.FillSolidRect(0, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1), clrShadow);
	}
	else if (pNext && pNext->IsCategory() && pNext->IsVisible())
	{
		dc.FillSolidRect(pNext->GetIndent() * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
						 rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1), clrShadow);
	}

	CXTPPropertyGridItem* pPrev = pItem;
	if (!pPrev)
		return;

	CXTPPropertyGridItem* pCategory = pPrev->GetParentItem();
	while (pCategory != NULL)
	{
		if (pItem->HasChilds() && pItem->IsExpanded())
			break;

		if (pCategory->IsCategory() && pCategory->IsVisible()
			&& pCategory->GetChilds()->GetAt(pCategory->GetChilds()->GetCount() - 1) == pPrev)
		{
			dc.FillSolidRect(pCategory->GetIndent() * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
							 rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1), clrShadow);
		}
		else
			break;
		pPrev	 = pCategory;
		pCategory = pPrev->GetParentItem();
	}

	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		if (!pItem->IsCategory())
		{
			rgn.CreateRectRgnIndirect(&rcCaption);
			dc.SelectClipRgn(&rgn);
		}

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
		rgn.DeleteObject();
	}
}

XTPScrollBarTheme CXTPPropertyGridCoolTheme::GetScrollBarTheme() const
{
	return xtpScrollBarThemeWindowsDefault;
}
