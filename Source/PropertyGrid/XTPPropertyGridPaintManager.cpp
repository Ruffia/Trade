// XTPPropertyGridPaintManager.cpp : implementation of the CXTPPropertyGridPaintManager class.
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridInplaceButton.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridToolBar.h"
#include "PropertyGrid/XTPPropertyGrid.h"
#include "PropertyGrid/XTPPropertyGridView.h"
#include "PropertyGrid/XTPPropertyGridItemBool.h"
#include "PropertyGrid/XTPPropertyGridIIDs.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#	include "Controls/Button/XTPButton.h"
#	include "Controls/Button/XTPButtonTheme.h"

#	include "Controls/Combo/XTPComboBox.h"
#	include "Controls/Combo/XTPComboBoxTheme.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPPropertyGridItemMetrics::CXTPPropertyGridItemMetrics(CXTPPropertyGridPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	m_nImage		  = -1;
	m_uDrawTextFormat = DT_VCENTER | DT_SINGLELINE;
	m_nMaxLength	  = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}
void CXTPPropertyGridItemMetrics::SetDefaultValues()
{
	m_clrHelpBack.SetDefaultValue();
	m_clrHelpFore.SetDefaultValue();
	m_clrLine.SetDefaultValue();
	m_clrBack.SetDefaultValue();
	m_clrFore.SetDefaultValue();
	m_clrReadOnlyFore.SetDefaultValue();
	m_clrCategoryFore.SetDefaultValue();
	m_clrVerbFace.SetDefaultValue();
}

COLORREF CXTPPropertyGridPaintManager::GetItemTextColor(CXTPPropertyGridItem* pItem,
														BOOL bValuePart)
{
	ASSERT(pItem);
	if (!pItem)
		return m_pMetrics->m_clrFore;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(bValuePart, FALSE);
	if (pMetrics && !pMetrics->m_clrFore.IsDefaultValue())
		return pMetrics->m_clrFore;

	return pItem->IsCategory()
			   ? m_pMetrics->m_clrCategoryFore
			   : pItem->GetReadOnly() ? m_pMetrics->m_clrReadOnlyFore : m_pMetrics->m_clrFore;
}

COLORREF CXTPPropertyGridPaintManager::GetItemBackColor(CXTPPropertyGridItem* pItem,
														BOOL bValuePart)
{
	ASSERT(pItem);
	if (!pItem)
		return m_pMetrics->m_clrBack;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(bValuePart, FALSE);
	if (pMetrics && !pMetrics->m_clrBack.IsDefaultValue())
		return pMetrics->m_clrBack;

	return m_pMetrics->m_clrBack;
}

CFont* CXTPPropertyGridPaintManager::GetItemFont(CXTPPropertyGridItem* pItem, BOOL bValuePart)
{
	ASSERT(pItem);
	if (!pItem)
		return &m_pMetrics->m_xtpFontNormal;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(bValuePart, FALSE);
	if (pMetrics && pMetrics->m_xtpFontNormal.GetSafeHandle())
		return &pMetrics->m_xtpFontNormal;

	if (pItem->IsCategory())
		return &m_pMetrics->m_xtpFontBold;

	if (bValuePart && m_pGrid->IsHighlightChangedItems() && pItem->IsValueChanged())
		return &m_pMetrics->m_xtpFontBold;

	return &m_pMetrics->m_xtpFontNormal;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPPropertyGridPaintManager::CXTPPropertyGridPaintManager(CXTPPropertyGrid* pGrid)
	: m_pGrid(pGrid)
{
	m_nVerbsHeight	= XTP_DPI_Y(25);
	m_nHelpHeight	 = XTP_DPI_Y(58);
	m_nSplitterHeight = XTP_DPI_Y(3);
	m_bThinSplitter   = FALSE;
	m_buttonsStyle	= xtpPropertyGridButtonsDefault;

#ifdef _XTP_INCLUDE_CONTROLS
	m_bUseThemedControls = FALSE;
	m_pButtonCheck		 = NULL;
	m_pCombo			 = NULL;
	m_pButton			 = NULL;
#endif

	m_themeTree   = new CXTPWinThemeWrapper();
	m_themeButton = new CXTPWinThemeWrapper();
	m_themeCombo  = new CXTPWinThemeWrapper();

	m_pMetrics = new CXTPPropertyGridItemMetrics(this);

	m_bTransparent = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPPropertyGridPaintManager::~CXTPPropertyGridPaintManager()
{
	CMDTARGET_RELEASE(m_pMetrics);

	SAFE_DELETE(m_themeTree);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeCombo);

#ifdef _XTP_INCLUDE_CONTROLS
	SAFE_DELETE(m_pButtonCheck);
	SAFE_DELETE(m_pCombo);
	SAFE_DELETE(m_pButton);
#endif
}

void CXTPPropertyGridPaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	m_clrHighlight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrNonHighlightBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrNonHighlightText.SetStandardValue(::GetSysColor(COLOR_WINDOWTEXT));

	m_clrFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
	m_pMetrics->m_clrHelpFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_pMetrics->m_clrLine.SetStandardValue(GetSysColor(COLOR_SCROLLBAR));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));
	m_pMetrics->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_pMetrics->m_clrFore.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_pMetrics->m_clrReadOnlyFore.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	HWND hWnd = AfxGetMainWnd() ? AfxGetMainWnd()->GetSafeHwnd() : 0;
	m_themeTree->OpenThemeData(hWnd, L"TREEVIEW");
	m_themeButton->OpenThemeData(hWnd, L"BUTTON");
	m_themeCombo->OpenThemeData(hWnd, L"COMBOBOX");

	m_pMetrics->m_clrVerbFace.SetStandardValue(XTPSystemVersion()->IsWinVistaOrGreater()
													   && m_themeButton->IsAppThemed()
												   ? RGB(0, 102, 204)
												   : GetXtremeColor(COLOR_ACTIVECAPTION));

	m_nItemsBorderHeight = XTP_DPI_Y(1); // grid line between items no need DPI scaling

#ifdef _XTP_INCLUDE_CONTROLS
	// themed controls

	if (m_bUseThemedControls && !m_pButtonCheck && m_pGrid->GetSafeHwnd())
	{
		m_pButtonCheck = new CXTPButton();
		m_pButtonCheck->Create(NULL, WS_CHILD | BS_AUTOCHECKBOX, CRect(0, 0, 0, 0), m_pGrid, 1);
		m_pButtonCheck->ShowWindow(SW_HIDE);

		m_pCombo = new CXTPComboBox();
		m_pCombo->Create(WS_CHILD, CRect(0, 0, 0, 0), m_pGrid, 2);
		m_pCombo->ShowWindow(SW_HIDE);

		m_pButton = new CXTPButton();
		m_pButton->Create(NULL, WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), m_pGrid, 3);
		m_pButton->SetFlatStyle();
	}
#endif
}

XTPScrollBarTheme CXTPPropertyGridPaintManager::GetScrollBarTheme() const
{
	return xtpScrollBarThemeWindowsDefault;
}

CRect CXTPPropertyGridPaintManager::GetSplitterRect(CRect rc, BOOL bIsHelp)
{
	if (bIsHelp)
	{
		return CRect(CPoint(rc.left, rc.bottom - m_nSplitterHeight - m_nHelpHeight),
					 CSize(rc.Width(), m_nSplitterHeight));
	}
	else
	{
		if (m_pGrid->IsHelpVisible())
		{
			rc.bottom -= m_nHelpHeight + m_nSplitterHeight;
		}

		return CRect(CPoint(rc.left, rc.bottom - m_nSplitterHeight - m_nVerbsHeight),
					 CSize(rc.Width(), m_nSplitterHeight));
	}
}

void CXTPPropertyGridPaintManager::DrawSplitter(CDC* pDC, BOOL bIsHelp)
{
	if (!m_bThinSplitter)
		return;

	CRect rc(GetSplitterRect(CXTPClientRect(m_pGrid), bIsHelp));

	if (m_pGrid->GetBorderStyle() == xtpPropertyGridBorderNone
		|| m_pGrid->GetBorderStyle() == xtpPropertyGridBorderStaticEdge)
	{
		CRect rcTop(rc);
		rcTop.bottom = rcTop.top + 1;
		pDC->FillSolidRect(rcTop, m_clrSplitter.IsDefaultValue()
									  ? GetXtremeColor(COLOR_3DFACE)
									  : m_clrSplitter.operator COLORREF());

		if (m_pGrid->GetBorderStyle() == xtpPropertyGridBorderStaticEdge)
		{
			CRect rcLeft(rc);
			rcLeft.right = rcLeft.left + 1;
			pDC->FillSolidRect(rcLeft, m_clrShadow);

			CRect rcRight(rc);
			rcRight.left = rcRight.right - 1;
			pDC->FillSolidRect(rcRight, m_clrShadow);
		}
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrSplitter.IsDefaultValue() ? GetXtremeColor(COLOR_3DFACE)
															  : m_clrSplitter.operator COLORREF());
	}
}

void CXTPPropertyGridPaintManager::FillPropertyGrid(CDC* pDC)
{
	CXTPClientRect rc(m_pGrid);

	HBRUSH hBrush = NULL;

	if (m_bTransparent || m_clrFace == GetXtremeColor(COLOR_3DFACE))
	{
		hBrush = (HBRUSH)m_pGrid->GetParent()->SendMessage(WM_CTLCOLORSTATIC,
														   (WPARAM)pDC->GetSafeHdc(),
														   (LPARAM)m_pGrid->m_hWnd);
	}
	if (hBrush)
	{
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrFace);
	}

	pDC->SetBkMode(TRANSPARENT);

	if (m_pGrid->IsHelpVisible())
	{
		CXTPPropertyGridItem* pItem = m_pGrid->GetSelectedItem();

		CRect rcHelp(rc);
		rcHelp.top = rc.bottom - m_nHelpHeight;

		if (!hBrush || !m_bTransparent)
			pDC->FillSolidRect(rcHelp, m_pMetrics->m_clrHelpBack);
		DrawPropertyGridBorder(pDC, rcHelp, xtpPropertyGridHelpWindow);

		CXTPFontDC fontCaption(pDC, &m_pMetrics->m_xtpFontBold);

		if (pItem)
		{
			pDC->SetTextColor(m_pMetrics->m_clrHelpFore);

			pDC->SetBkMode(TRANSPARENT);
			CRect rcCaption(rcHelp);
			rcCaption.DeflateRect(XTP_DPI_X(3) + XTP_DPI_X(3), XTP_DPI_Y(3));
			CXTPEmptyRect rcCaptionHeight;

			if (pItem->GetMarkupCaption())
			{
				XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
										(HFONT)m_pMetrics->m_xtpFontBold, pDC->GetTextColor());

				CSize sz = XTPMarkupMeasureElement(pItem->GetMarkupCaption(), INT_MAX, INT_MAX);
				XTPMarkupRenderElement(pItem->GetMarkupCaption(), pDC->GetSafeHdc(),
									   CRect(rcCaption.TopLeft(), sz));
				rcCaptionHeight.right  = sz.cx;
				rcCaptionHeight.bottom = sz.cy;
			}
			else
			{
				pDC->DrawText(pItem->GetCaption(), rcCaptionHeight,
							  DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT);
				pDC->DrawText(pItem->GetCaption(), rcCaption, DT_SINGLELINE | DT_NOPREFIX);
			}

			CString strDesc = pItem->GetDescription();
			if (!strDesc.IsEmpty())
			{
				CXTPFontDC fontDescription(pDC, &m_pMetrics->m_xtpFontNormal);
				CRect rcDesc(rcHelp);
				rcDesc.DeflateRect(XTP_DPI_X(3) + XTP_DPI_X(3),
								   XTP_DPI_Y(3) + XTP_DPI_Y(3) + rcCaptionHeight.Height(),
								   XTP_DPI_X(3), XTP_DPI_Y(3));

				if (pItem->GetMarkupDescription())
				{
					XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupDescription()),
											(HFONT)m_pMetrics->m_xtpFontNormal,
											pDC->GetTextColor());

					CSize sz = XTPMarkupMeasureElement(pItem->GetMarkupDescription(),
													   rcDesc.Width(), rcDesc.Height());
					XTPMarkupRenderElement(pItem->GetMarkupDescription(), pDC->GetSafeHdc(),
										   rcDesc);
				}
				else
				{
					pDC->DrawText(strDesc, rcDesc, DT_WORDBREAK | DT_NOPREFIX);
				}
			}
		}
		DrawSplitter(pDC, TRUE);

		rc.bottom -= m_nHelpHeight + m_nSplitterHeight;
	}

	if (m_pGrid->IsVerbsVisible())
	{
		CRect rcVerbs(rc);
		rcVerbs.top = rc.bottom - m_nVerbsHeight;

		if (!hBrush || !m_bTransparent)
			pDC->FillSolidRect(rcVerbs, m_pMetrics->m_clrHelpBack);
		DrawPropertyGridBorder(pDC, rcVerbs, xtpPropertyGridVerbWindow);
		DrawSplitter(pDC, FALSE);

		HitTestVerbs(pDC, rcVerbs, CPoint(0));
	}
}

int CXTPPropertyGridPaintManager::HitTestVerbs(CDC* pDC, CRect rcVerbs, CPoint pt)
{
#define DRAWTEXTPART(strCaption)                                                                   \
	CRect rcText(                                                                                  \
		CRect(x, y, min(x + nTextWidth, rcVerbs.right), min(y + nTextHeight, rcVerbs.bottom)));    \
	if (bDraw)                                                                                     \
	{                                                                                              \
		pVerb->SetPart(rcText);                                                                    \
		pDC->SetTextColor(m_pMetrics->m_clrVerbFace);                                              \
		pDC->DrawText(strCaption, rcText, DT_SINGLELINE | DT_NOPREFIX);                            \
		if (pVerb->IsFocused())                                                                    \
		{                                                                                          \
			pDC->SetTextColor(m_pMetrics->m_clrHelpBack);                                          \
			pDC->SetBkColor(0);                                                                    \
			rcText.bottom++;                                                                       \
			pDC->DrawFocusRect(rcText);                                                            \
		}                                                                                          \
	}                                                                                              \
	else if (rcText.PtInRect(pt))                                                                  \
		return i;

	BOOL bDraw = (pt == CPoint(0));

	CXTPFontDC font(pDC, &m_pMetrics->m_xtpFontUnderline);

	rcVerbs.DeflateRect(XTP_DPI_X(6), XTP_DPI_Y(4));

	int nCount = m_pGrid->GetVerbs()->GetCount();

	int x = rcVerbs.left, y = rcVerbs.top, nWidth = rcVerbs.Width();
	int nTextHeight = pDC->GetTextExtent(_T("XXX"), 3).cy;

	for (int i = 0; i < nCount; i++)
	{
		CXTPPropertyGridVerb* pVerb = m_pGrid->GetVerbs()->GetAt(i);
		CString strCaption			= pVerb->GetCaption();

		for (;;)
		{
			if (strCaption.IsEmpty())
				break;

			int nTextWidth = pDC->GetTextExtent(strCaption).cx;

			if (x + nTextWidth < nWidth)
			{
				DRAWTEXTPART(strCaption);
				x += nTextWidth;
				break;
			}
			else
			{
				int nStartIndex = (strCaption.GetLength() - 1) * (nWidth - x) / nTextWidth;
				int nIndex		= nStartIndex;

				if ((nStartIndex < 1) && (x == rcVerbs.left))
				{
					i = nCount - 1;
					break;
				}

				for (; nIndex >= 0; nIndex--)
				{
					if (strCaption[nIndex] == _T(' ') || strCaption[nIndex] == _T(',')
						|| strCaption[nIndex] == _T('-'))
						break;
				}

				if (nIndex > 0 || (x == rcVerbs.left))
				{
					if (nIndex <= 0)
						nIndex = nStartIndex;

					CString strLeft  = strCaption.Left(nIndex + 1);
					CString strRight = strCaption.Mid(nIndex + 1);

					nTextWidth = pDC->GetTextExtent(strLeft).cx;
					DRAWTEXTPART(strLeft);

					x = rcVerbs.left;
					y += nTextHeight;
					strCaption = strRight;
				}
				else
				{
					y += nTextHeight;
					x = rcVerbs.left;
				}
			}
			if (y >= rcVerbs.bottom)
				break;
		}

		if ((y >= rcVerbs.bottom) || (i == nCount - 1))
			break;

		if (bDraw)
		{
			font.SetFont(&m_pMetrics->m_xtpFontNormal);
			pDC->SetTextColor(m_pMetrics->m_clrHelpFore);
			pDC->DrawText(_T(","), 1,
						  CRect(x, y, x + XTP_DPI_X(8), min(y + nTextHeight, rcVerbs.bottom)),
						  DT_SINGLELINE | DT_NOPREFIX);
			font.SetFont(&m_pMetrics->m_xtpFontUnderline);
		}
		x += XTP_DPI_X(8);
	}
	return -1;
}

void CXTPPropertyGridPaintManager::OnNcCalcSize(RECT& rc)
{
	int nSizeX = 0;
	int nSizeY = 0;

	switch (m_pGrid->GetBorderStyle())
	{
		case xtpPropertyGridBorderClientEdge:
			nSizeX = XTP_DPI_X(1) * 2;
			nSizeY = XTP_DPI_Y(1) * 2;
			break;
		case xtpPropertyGridBorderStaticEdge:
		case xtpPropertyGridBorderFlat:
			nSizeX = XTP_DPI_X(1);
			nSizeY = XTP_DPI_Y(1);
			break;
	}

	rc.left += nSizeX;
	rc.top += nSizeY;
	rc.right -= nSizeX;
	rc.bottom -= nSizeY;
}

void CXTPPropertyGridPaintManager::DrawPropertyGridBorder(CDC* pDC, RECT& rc,
														  XTPPropertyGridWindow nWindow)
{
	if (!pDC)
	{
		return;
	}

	switch (m_pGrid->GetBorderStyle())
	{
		case xtpPropertyGridBorderStaticEdge:
		{
			COLORREF clrBorder[4];

			switch (nWindow)
			{
				case xtpPropertyGridGridWindow:
					clrBorder[0] = m_clrShadow;											  // left
					clrBorder[1] = m_clrShadow;											  // top
					clrBorder[2] = m_clrShadow;											  // right
					clrBorder[3] = m_bThinSplitter ? m_pMetrics->m_clrBack : m_clrShadow; // bottom
					break;

				case xtpPropertyGridHelpWindow:
					clrBorder[0] = m_clrShadow; // left
					clrBorder[1] = m_bThinSplitter ? m_pMetrics->m_clrHelpBack : m_clrShadow; // top
					clrBorder[2] = m_clrShadow; // right
					clrBorder[3] = m_clrShadow; // bottom
					break;

				case xtpPropertyGridVerbWindow:
				default:
					clrBorder[0] = m_clrShadow; // left
					clrBorder[1] = m_bThinSplitter ? m_pMetrics->m_clrHelpBack : m_clrShadow; // top
					clrBorder[2] = m_clrShadow; // right
					clrBorder[3] = m_bThinSplitter ? m_pMetrics->m_clrHelpBack
												   : m_clrShadow; // bottom
					break;
			}

			CRect rcTop(rc);
			rcTop.bottom = rcTop.top + XTP_DPI_Y(1);
			pDC->FillSolidRect(rcTop, clrBorder[1]);

			CRect rcBottom(rc);
			rcBottom.top = rcBottom.bottom - XTP_DPI_Y(1);
			pDC->FillSolidRect(rcBottom, clrBorder[3]);

			CRect rcLeft(rc);
			rcLeft.right = rcLeft.left + XTP_DPI_X(1);
			pDC->FillSolidRect(rcLeft, clrBorder[0]);

			CRect rcRight(rc);
			rcRight.left = rcRight.right - XTP_DPI_X(1);
			pDC->FillSolidRect(rcRight, clrBorder[2]);
		}
		break;

		case xtpPropertyGridBorderFlat:
		{
			COLORREF clrFrame = GetXtremeColor(COLOR_WINDOWFRAME);

			for (int i = XTP_DPI_X(1) - 1; i >= 0; --i)
			{
				pDC->Draw3dRect(rc.left + i, rc.top + i, rc.right - rc.left - i * 2,
								rc.bottom - rc.top - i * 2, clrFrame, clrFrame);
			}
		}
		break;

		case xtpPropertyGridBorderClientEdge:
		{
			COLORREF clrTL, clrBR;
			int nHalfThickness = XTP_DPI_X(1);
			for (int i = XTP_DPI_X(1) * 2 - 1; i >= 0; --i)
			{
				clrTL = GetXtremeColor(
					UINT((i < nHalfThickness) ? COLOR_3DSHADOW : COLOR_3DDKSHADOW));
				clrBR = GetXtremeColor(
					UINT((i < nHalfThickness) ? COLOR_3DHIGHLIGHT : COLOR_3DFACE));

				pDC->Draw3dRect(rc.left + i, rc.top + i, rc.right - rc.left - i * 2,
								rc.bottom - rc.top - i * 2, clrTL, clrBR);
			}
		}
		break;
	}
}

void CXTPPropertyGridPaintManager::FillPropertyGridView(CDC* pDC)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	CXTPClientRect rc(pView);
	pDC->FillSolidRect(rc, m_pMetrics->m_clrBack);
}

void CXTPPropertyGridPaintManager::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	if (pView->GetStyle() & LBS_OWNERDRAWVARIABLE)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpMeasureItemStruct->itemData;
		pItem->MeasureItem(lpMeasureItemStruct);
	}
}

void CXTPPropertyGridPaintManager::DrawCategoryCaptionBackground(CDC* pDC,
																 CXTPPropertyGridItem* /*pItem*/,
																 CRect rc)
{
	pDC->FillSolidRect(rc, m_pMetrics->m_clrLine);
}

void CXTPPropertyGridPaintManager::DrawInplaceListItem(CDC* pDC,
													   CXTPPropertyGridItemConstraint* pConstraint,
													   CRect rc, BOOL bSelected)
{
	pDC->SetBkMode(TRANSPARENT);

	if (bSelected)
	{
		pDC->SetTextColor(m_clrHighlightText);
		pDC->FillSolidRect(rc, m_clrHighlight);
	}
	else
	{
		pDC->SetTextColor(m_clrNonHighlightText);
		pDC->FillSolidRect(rc, m_clrNonHighlightBack);
	}

	if (!pConstraint->m_bEnabled)
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));

	CRect rcText(rc.left + XTP_DPI_X(2), rc.top, rc.right - XTP_DPI_X(2), rc.bottom);

	CXTPImageManagerIcon* pImage = pConstraint->GetImage();

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
		pImage->Draw(pDC, pt, sz);

		rcText.left += sz.cx + XTP_DPI_X(4);
	}

	// Draw the text.
	pDC->DrawText(pConstraint->m_strConstraint, rcText,
				  DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
}

void CXTPPropertyGridPaintManager::AdjustItemValueRect(CXTPPropertyGridItem* pItem, CRect& rcValue)
{
	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(TRUE, FALSE);
	if (pMetrics && pMetrics->m_nImage != -1)
	{
		CXTPImageManagerIcon* pImage =
			m_pGrid->GetImageManager()->GetImage(XTPToUInt(pMetrics->m_nImage), 0);

		if (pImage)
		{
			int nWidth = pImage->GetWidth();
			rcValue.left += nWidth + XTP_DPI_X(4);
		}
	}

	if (GetItemsBorderHeight() > XTP_DPI_Y(1))
		rcValue.left += GetItemsBorderHeight() / 2;
}

void CXTPPropertyGridPaintManager::AdjustItemCaptionRect(CXTPPropertyGridItem* pItem,
														 CRect& rcCaption)
{
	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(FALSE, FALSE);
	if (pMetrics && pMetrics->m_nImage != -1)
	{
		CXTPImageManagerIcon* pImage =
			m_pGrid->GetImageManager()->GetImage(XTPToUInt(pMetrics->m_nImage), 0);

		if (pImage)
		{
			int nWidth = pImage->GetWidth();
			rcCaption.left += nWidth + XTP_DPI_X(4);
		}
	}
}

void CXTPPropertyGridPaintManager::DrawItemValue(CDC* pDC, CXTPPropertyGridItem* pItem,
												 CRect rcValue)
{
	pDC->SetTextColor(GetItemTextColor(pItem, TRUE));
	pDC->FillSolidRect(rcValue, GetItemBackColor(pItem, TRUE));

	rcValue.left += XTP_DPI_X(4);
	if (GetItemsBorderHeight() > XTP_DPI_Y(1))
		rcValue.left += GetItemsBorderHeight() / 2;

	for (int i = 0; i < pItem->GetInplaceButtons()->GetCount(); i++)
	{
		CXTPPropertyGridInplaceButton* pButton = pItem->GetInplaceButtons()->GetAt(i);
		if (pButton && pItem->IsInplaceButtonVisible(pButton))
			if ((pButton->GetAlignment() & DT_RIGHT) == 0)
			{
				rcValue.left += pButton->GetWidth();
			}
			else
			{
				rcValue.right -= pButton->GetWidth();
			}
	}

	if (pItem->GetMarkupValue())
	{
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupValue()),
								(HFONT)GetItemFont(pItem, TRUE)->GetSafeHandle(),
								pDC->GetTextColor());

		CSize sz = XTPMarkupMeasureElement(pItem->GetMarkupValue(), rcValue.Width(),
										   rcValue.Height());

		sz.cy = max(sz.cy, rcValue.Height()); // for correct vertical alignment

		XTPMarkupRenderElement(pItem->GetMarkupValue(), pDC->GetSafeHdc(),
							   CRect(rcValue.TopLeft(), sz));
	}
	else
	{
		if (pItem->OnDrawItemValue(*pDC, rcValue))
			return;

		CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(TRUE, FALSE);
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

				sz.cx = min(sz.cx, rcValue.Width());
				sz.cy = min(sz.cy, rcValue.Height());

				CPoint pt(rcValue.left, rcValue.CenterPoint().y - sz.cy / 2);
				pImage->Draw(pDC, pt, sz);

				rcValue.left += sz.cx + XTP_DPI_X(4);
			}
		}
		rcValue.DeflateRect(0, 1);

		UINT nFormat = pMetrics ? pMetrics->m_uDrawTextFormat : DT_VCENTER | DT_SINGLELINE;

		if (pItem->IsMultiLine())
			nFormat = (nFormat | DT_WORDBREAK) & ~DT_SINGLELINE;

		pDC->DrawText(pItem->GetViewValue(), rcValue, DT_NOPREFIX | nFormat);
	}
}

void CXTPPropertyGridPaintManager::DrawItemCaption(CDC* pDC, CXTPPropertyGridItem* pItem,
												   CRect rcCaption)
{
	CRect rcText(rcCaption);
	rcText.left = (pItem->GetIndent() + (pItem->IsCategory() ? 1 : 0))
					  * XTP_DPI_X(XTP_PGI_EXPAND_BORDER)
				  + XTP_DPI_X(3);

	rcText.right = rcCaption.right - XTP_DPI_X(1);
	rcText.bottom -= XTP_DPI_Y(1);

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

			sz.cx = min(sz.cx, rcText.Width());
			sz.cy = min(sz.cy, rcText.Height());

			CPoint pt(rcText.left, rcText.CenterPoint().y - sz.cy / 2);
			pImage->Draw(pDC, pt, sz);

			rcText.left += sz.cx + XTP_DPI_X(4);
		}
	}

	if (pItem->GetMarkupCaption())
	{
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
								(HFONT)GetItemFont(pItem, FALSE)->GetSafeHandle(),
								pDC->GetTextColor());

		XTPMarkupMeasureElement(pItem->GetMarkupCaption(), INT_MAX, INT_MAX);
		XTPMarkupRenderElement(pItem->GetMarkupCaption(), pDC->GetSafeHdc(), rcText);
	}
	else
	{
		UINT nFormat = pMetrics ? pMetrics->m_uDrawTextFormat : DT_VCENTER | DT_SINGLELINE;

		pDC->DrawText(pItem->GetCaption(), rcText, DT_NOPREFIX | nFormat);
	}
}

void CXTPPropertyGridPaintManager::DrawInplaceButtons(CDC* pDC, PDRAWITEMSTRUCT lpDrawItemStruct,
													  CXTPPropertyGridItem* pItem, CRect rcValue)
{
	if (!pItem->IsAllowEdit())
		return;

	BOOL bSelected =
		(0
		 != (lpDrawItemStruct ? (lpDrawItemStruct->itemState & (ODS_SELECTED | ODS_FOCUS)) : TRUE));
	BOOL bShowAlways = m_pGrid->GetShowInplaceButtonsAlways();

	int nRight = rcValue.right;
	int nLeft  = rcValue.left + XTP_DPI_X(2);

	for (int i = pItem->GetInplaceButtons()->GetCount() - 1; i >= 0; i--)
	{
		CXTPPropertyGridInplaceButton* pButton = pItem->GetInplaceButtons()->GetAt(i);

		if (!(bSelected || bShowAlways || pButton->GetShowAlways()))
			continue;

		int nWidth = pButton->GetWidth();

		if (pButton->GetAlignment() & DT_RIGHT)
		{
			CRect rcButton(nRight - nWidth, rcValue.top, nRight, rcValue.bottom);
			pButton->OnDraw(pDC, rcButton);
			nRight -= nWidth;
		}
		else
		{
			CRect rcButton(nLeft, rcValue.top, nLeft + nWidth, rcValue.bottom);
			pButton->OnDraw(pDC, rcButton);
			nLeft += nWidth;
		}
	}
}

void CXTPPropertyGridPaintManager::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, TRUE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);

	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	if (pItem->IsCategory())
	{
		DrawCategoryCaptionBackground(&dc, pItem, rc);

		CFont* pFont = GetItemFont(pItem, FALSE);
		font.SetFont(pFont);

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));
		CRect rcCanvas(((pItem->GetIndent() + 1) * XTP_DPI_X(XTP_PGI_EXPAND_BORDER)) + XTP_DPI_X(3),
					   rc.top, rc.right, rc.bottom - XTP_DPI_Y(1));

		if (GetItemsBorderHeight() > XTP_DPI_Y(1))
		{
			rcCanvas.top += GetItemsBorderHeight() / 2;
			rcCanvas.bottom -= GetItemsBorderHeight() / 2 + GetItemsBorderHeight() % 2;
		}

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

				CPoint pt(rcCanvas.left, rcCanvas.CenterPoint().y - sz.cy / 2);
				pImage->Draw(&dc, pt, sz);

				rcCanvas.left += sz.cx + XTP_DPI_X(4);
			}
		}

		CRect rcText = rcCanvas;
		for (int i = 0; i < pItem->GetInplaceButtons()->GetCount(); i++)
		{
			CXTPPropertyGridInplaceButton* pButton = pItem->GetInplaceButtons()->GetAt(i);
			if (pButton && pItem->IsInplaceButtonVisible(pButton))
				if ((pButton->GetAlignment() & DT_RIGHT) == 0)
				{
					rcText.left += pButton->GetWidth();
				}
				else
				{
					rcText.right -= pButton->GetWidth();
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
			dc.DrawText(pItem->GetCaption(), rcText,
						DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_LEFT);
		}

		if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			CXTPEmptyRect rcFocus;
			if (pItem->GetMarkupCaption())
			{
				CSize sz	  = XTPMarkupMeasureElement(pItem->GetMarkupCaption(), rcCanvas.Width(),
													rcCanvas.Height());
				rcFocus.right = rcFocus.left + sz.cx;
			}
			else
			{
				dc.DrawText(pItem->GetCaption(), rcFocus,
							DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_CALCRECT);
			}

			rcFocus.SetRect(rcText.left - XTP_DPI_X(3), rcText.top + XTP_DPI_Y(1),
							rcText.left + rcFocus.Width() + XTP_DPI_X(3),
							rcText.bottom - XTP_DPI_Y(1));

			dc.SetTextColor(0);
			dc.DrawFocusRect(rcFocus);
		}

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcCanvas);
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
			dc.FillSolidRect(rcCaption, m_pMetrics->m_clrLine);
		}

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left
									 + pCategory->GetIndent() * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
								 rc.top, XTP_DPI_X(XTP_PGI_EXPAND_BORDER), rc.Height(),
								 m_pMetrics->m_clrLine);
			}

			pCategory = pCategory->GetParentItem();
		}

		CXTPPenDC pen(dc, m_pMetrics->m_clrLine);

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
			dc.MoveTo(0, rc.bottom - 1);
			dc.LineTo(rc.right, rc.bottom - 1);
			dc.MoveTo(rcCaption.right, rc.top);
			dc.LineTo(rcCaption.right, rc.bottom - 1);
		}

		DrawItemCaption(&dc, pItem, rcCaption);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);
	}

	CXTPPropertyGridItem* pNext = pView->GetItem(XTPToInt(lpDrawItemStruct->itemID + 1));
	if (pNext && pNext->IsCategory() && pNext->IsVisible())
	{
		dc.FillSolidRect(pNext->GetIndent() * XTP_DPI_X(XTP_PGI_EXPAND_BORDER), rc.bottom - 1,
						 rc.Width(), 1,
						 m_clrCategorySeparator.IsDefaultValue() ? m_clrFace
																 : m_clrCategorySeparator);
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

void CXTPPropertyGridPaintManager::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem,
													CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcSign(CPoint(XTP_DPI_X(XTP_PGI_EXPAND_BORDER) / 2 - XTP_DPI_X(5),
						rcCaption.CenterPoint().y - XTP_DPI_Y(4)),
				 CSize(XTP_DPI_X(9), XTP_DPI_Y(9)));

	if (pItem->GetIndent() > 0)
		rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1))
							  * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
						  0);

	if (m_buttonsStyle == xtpPropertyGridButtonsThemed && m_themeTree->IsAppThemeActive())
	{
		m_themeTree->DrawThemeBackground(dc, TVP_GLYPH,
										 pItem->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcSign,
										 0);
	}
	else
	{
		CPoint pt = rcSign.CenterPoint();

		if (!pItem->IsCategory())
			dc.FillSolidRect(rcSign, m_pMetrics->m_clrBack);
		dc.Draw3dRect(rcSign, m_pMetrics->m_clrFore, m_pMetrics->m_clrFore);

		XTPPrimitiveSymbol nSymbol = (pItem->IsExpanded() ? xtpPrimitiveSymbolMinus
														  : xtpPrimitiveSymbolPlus);
		XTPPrimitiveDrawer()->DrawSymbol(&dc, nSymbol, rcSign, m_pMetrics->m_clrFore);
	}
}

#ifdef _XTP_INCLUDE_CONTROLS
void CXTPPropertyGridPaintManager::SetControlsTheme(int eTheme)
{
	ASSERT(m_bUseThemedControls);

	if (m_bUseThemedControls && m_pButtonCheck)
	{
		m_pButtonCheck->SetTheme((XTPControlTheme)eTheme);
		m_pCombo->SetTheme((XTPControlTheme)eTheme);
		m_pButton->SetTheme((XTPControlTheme)eTheme);
	}
}

void CXTPPropertyGridPaintManager::DrawCheckMarkThemed(CDC* pDC, CRect rc, BOOL bEnabled,
													   BOOL bChecked)
{
	m_pButtonCheck->MoveWindow(rc);
	m_pButtonCheck->SetChecked(bChecked);
	m_pButtonCheck->EnableWindow(bEnabled);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, rc.Width(), rc.Height(), pDC, rc.left, rc.top, SRCCOPY);

	m_pButtonCheck->GetTheme()->DrawCheckBoxMark(&memDC, m_pButtonCheck);

	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CXTPPropertyGridPaintManager::DrawRadioMarkThemed(CDC* pDC, CRect rc, BOOL bEnabled,
													   BOOL bChecked)
{
	m_pButtonCheck->MoveWindow(rc);
	m_pButtonCheck->SetChecked(bChecked);
	m_pButtonCheck->EnableWindow(bEnabled);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, rc.Width(), rc.Height(), pDC, rc.left, rc.top, SRCCOPY);

	m_pButtonCheck->GetTheme()->DrawRadioButtonMark(&memDC, m_pButtonCheck);

	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CXTPPropertyGridPaintManager::FillInplaceButtonThemed(CDC* pDC,
														   CXTPPropertyGridInplaceButton* pButton)
{
	CRect rc(pButton->GetRect());

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, rc.Width(), rc.Height(), pDC, rc.left, rc.top, SRCCOPY);

	if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		m_pButton->MoveWindow(rc);
		m_pButton->EnableWindow(pButton->GetEnabled());
		m_pButton->m_bHot = (pButton->IsHot());
		m_pButton->SetState(pButton->IsPressed());
		// m_pButton->SetWindowText(pButton->GetCaption());

		m_pButton->GetTheme()->DrawButton(&memDC, m_pButton);
	}
	else
	{
		// m_pCombo->MoveWindow(rc);

		m_pCombo->EnableWindow(pButton->GetEnabled());
		m_pCombo->SetHighlighted(pButton->IsHot());

		m_pCombo->GetTheme()->DrawButton(&memDC, m_pCombo, CRect(0, 0, rc.Width(), rc.Height()));
		m_pCombo->GetTheme()->DrawButtonTriangle(&memDC, m_pCombo,
												 CRect(0, 0, rc.Width(), rc.Height()));
	}

	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	memDC.DeleteDC();
}
#endif

void CXTPPropertyGridPaintManager::FillInplaceButton(CDC* pDC,
													 CXTPPropertyGridInplaceButton* pButton)
{
	CRect rect(pButton->GetRect());
	COLORREF clrText = GetXtremeColor(UINT(pButton->GetEnabled() ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

	if (pButton->IsHyperlink())
	{
		clrText = !pButton->GetEnabled() ? GetXtremeColor(COLOR_GRAYTEXT)
										 : m_pMetrics->m_clrVerbFace.operator COLORREF();
	}
	else if (m_buttonsStyle == xtpPropertyGridButtonsThemed && m_themeButton->IsAppThemeActive()
			 && m_themeCombo->IsAppThemeActive())
	{
#ifdef _XTP_INCLUDE_CONTROLS
		if (m_bUseThemedControls)
		{
			FillInplaceButtonThemed(pDC, pButton);

			clrText = m_pButton->GetTheme()->m_crText;
		}
		else
		{
#endif
			pDC->FillSolidRect(rect, m_pMetrics->m_clrBack);

			if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
			{
				m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON,
												   !pButton->GetEnabled()
													   ? PBS_DISABLED
													   : pButton->IsPressed() && pButton->IsHot()
															 ? PBS_PRESSED
															 : pButton->IsHot()
																   ? PBS_HOT
																   : pButton->IsFocused()
																		 ? PBS_DEFAULTED
																		 : PBS_NORMAL,
												   rect, 0);
			}
			else
			{
				m_themeCombo->DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON,
												  !pButton->GetEnabled()
													  ? CBXS_DISABLED
													  : pButton->IsPressed() && pButton->IsHot()
															? CBXS_PRESSED
															: pButton->IsHot() ? CBXS_HOT
																			   : CBXS_NORMAL,
												  rect, 0);
			}
#ifdef _XTP_INCLUDE_CONTROLS
		}
#endif
	}
	else
	{
		if (m_buttonsStyle == xtpPropertyGridButtonsOfficeXP)
		{
			pDC->FillSolidRect(rect,
							   GetXtremeColor(UINT(pButton->IsPressed() && pButton->IsHot()
													   ? XPCOLOR_HIGHLIGHT_PUSHED
													   : pButton->IsHot() ? XPCOLOR_HIGHLIGHT
																		  : XPCOLOR_TOOLBAR_FACE)));
			COLORREF clrBorder = GetXtremeColor(
				UINT(pButton->IsPressed() && pButton->IsHot()
						 ? XPCOLOR_HIGHLIGHT_PUSHED_BORDER
						 : pButton->IsHot() ? XPCOLOR_HIGHLIGHT_BORDER : COLOR_3DSHADOW));
			pDC->Draw3dRect(rect, clrBorder, clrBorder);
		}
		else
		{
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

			if (pButton->IsPressed() && pButton->IsHot())
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DSHADOW),
								GetXtremeColor(COLOR_3DSHADOW));
			else
			{
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE),
								GetXtremeColor(COLOR_3DDKSHADOW));
				rect.DeflateRect(1, 1);
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
			}
		}

		if (pButton->GetID() == XTP_ID_PROPERTYGRID_COMBOBUTTON)
		{
			CRect rcArrow = rect;
			rcArrow.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow, clrText);
		}
	}

	BOOL bCustom = FALSE;

	if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		CXTPImageManagerIcon* pIcon = pButton->GetImage();
		if (pIcon)
		{
			CSize sz;
			if (pIcon->IsVectorIcon())
				sz = CSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
			else
				sz = CSize(pIcon->GetWidth(), pIcon->GetHeight());

			sz.cx = min(sz.cx, rect.Width());
			sz.cy = min(sz.cy, rect.Height());

			CPoint pt((rect.left + rect.right - sz.cx) / 2, (rect.top + rect.bottom - sz.cy) / 2);
			pIcon->Draw(pDC, pt, sz);
			bCustom = TRUE;
		}
		else if (!pButton->GetCaption().IsEmpty())
		{
			CXTPFontDC font(pDC, pButton->IsHyperlink()
									 ? &m_pMetrics->m_xtpFontUnderline
									 : (m_pGrid->IsHighlightChangedButtonItems()
											? GetItemFont(pButton->GetItem(), TRUE)
											: &m_pMetrics->m_xtpFontNormal));

			pDC->SetTextColor(clrText);
			pDC->DrawText(pButton->GetCaption(), rect,
						  DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
			bCustom = TRUE;
		}
	}

	if (!bCustom && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)
	{
		CRect rcEllipsis = rect;
		rcEllipsis.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEllipsis, rcEllipsis, clrText);
	}

	if (pButton->IsFocused())
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		rect.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));
		pDC->DrawFocusRect(rect);
	}
}

void CXTPPropertyGridPaintManager::DrawCheckMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked)
{
	if (m_buttonsStyle == xtpPropertyGridButtonsThemed && m_themeButton->IsAppThemeActive())
	{
#ifdef _XTP_INCLUDE_CONTROLS
		if (m_bUseThemedControls)
		{
			DrawCheckMarkThemed(pDC, rc, bEnabled, bChecked);
		}
		else
		{
#endif
			m_themeButton->DrawThemeBackground(
				pDC->GetSafeHdc(), BP_CHECKBOX,
				!bEnabled ? (bChecked ? CBS_CHECKEDDISABLED : CBS_UNCHECKEDDISABLED)
						  : (bChecked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL),
				rc, 0);
#ifdef _XTP_INCLUDE_CONTROLS
		}
#endif
	}
	else
	{
		pDC->DrawFrameControl(rc, DFC_BUTTON,
							  UINT(DFCS_BUTTONCHECK | (!bEnabled ? DFCS_INACTIVE : 0)
								   | (bChecked ? DFCS_CHECKED : 0)));
	}
}

void CXTPPropertyGridPaintManager::DrawRadioMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked)
{
	if (m_buttonsStyle == xtpPropertyGridButtonsThemed && m_themeButton->IsAppThemeActive())
	{
#ifdef _XTP_INCLUDE_CONTROLS
		if (m_bUseThemedControls)
		{
			DrawRadioMarkThemed(pDC, rc, bEnabled, bChecked);
		}
		else
		{
#endif
			m_themeButton->DrawThemeBackground(
				pDC->GetSafeHdc(), BP_RADIOBUTTON,
				!bEnabled ? (bChecked ? RBS_CHECKEDDISABLED : RBS_UNCHECKEDDISABLED)
						  : (bChecked ? RBS_CHECKEDNORMAL : RBS_UNCHECKEDNORMAL),
				rc, 0);
#ifdef _XTP_INCLUDE_CONTROLS
		}
#endif
	}
	else
	{
		pDC->DrawFrameControl(rc, DFC_BUTTON,
							  UINT(DFCS_BUTTONRADIO | (!bEnabled ? DFCS_INACTIVE : 0)
								   | (bChecked ? DFCS_CHECKED : 0)));
	}
}

void CXTPPropertyGridPaintManager::DrawCheckBox(CDC* pDC, CXTPPropertyGridItemBool* pItem)
{
	CRect rcValue = pItem->GetValueRect();
	CRect rcSample(rcValue.left + XTP_DPI_X(2), rcValue.CenterPoint().y - XTP_DPI_Y(6),
				   rcValue.left + XTP_DPI_X(2) + XTP_DPI_X(6) + XTP_DPI_X(7),
				   rcValue.CenterPoint().y + XTP_DPI_Y(7));

	DrawCheckMark(pDC, rcSample, !pItem->GetReadOnly(), pItem->GetBool());
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPPropertyGridPaintManager, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "HelpBackColor", 1, OleGetHelpBackColor,
						OleSetHelpBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "HelpForeColor", 2, OleGetHelpForeColor,
						OleSetHelpForeColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "ViewBackColor", 3, OleGetViewBackColor,
						OleSetViewBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "ViewForeColor", 4, OleGetViewForeColor,
						OleSetViewForeColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "LineColor", 5, OleGetLineColor,
						OleSetLineColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "ViewCategoryForeColor", 6,
						OleGetViewCategoryForeColor, OleSetViewCategoryForeColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "ViewReadOnlyForeColor", 7,
						OleGetViewReadOnlyForeColor, OleSetViewReadOnlyForeColor, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "BackgroundColor", 8, OleGetBackgroundColor,
						OleSetBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "ShadowColor", 9, OleGetShadowColor,
						OleSetShadowColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "HighlightBackColor", 10,
						OleGetHighlightBackColor, OleSetHighlightBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "HighlightForeColor", 11,
						OleGetHighlightForeColor, OleSetHighlightForeColor, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "HyperlinkColor", 12, OleGetHyperlinkColor,
						OleSetHyperlinkColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridPaintManager, "ItemsBorderHeight", 13,
						OleGetItemsBorderHeight, OleSetItemsBorderHeight, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridPaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridPaintManager, XTPDIID_PropertyGridPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridPaintManager, XTPDIID_PropertyGridPaintManager)

COLORREF CXTPPropertyGridPaintManager::OleGetHelpBackColor()
{
	return GetItemMetrics()->m_clrHelpBack;
}

void CXTPPropertyGridPaintManager::OleSetHelpBackColor(COLORREF clr)
{
	GetItemMetrics()->m_clrHelpBack = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetHelpForeColor()
{
	return GetItemMetrics()->m_clrHelpFore;
}

void CXTPPropertyGridPaintManager::OleSetHelpForeColor(COLORREF clr)
{
	GetItemMetrics()->m_clrHelpFore = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetViewBackColor()
{
	return GetItemMetrics()->m_clrBack;
}

void CXTPPropertyGridPaintManager::OleSetViewBackColor(COLORREF clr)
{
	GetItemMetrics()->m_clrBack = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetViewForeColor()
{
	return GetItemMetrics()->m_clrFore;
}

void CXTPPropertyGridPaintManager::OleSetViewForeColor(COLORREF clr)
{
	GetItemMetrics()->m_clrFore = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetLineColor()
{
	return GetItemMetrics()->m_clrLine;
}

void CXTPPropertyGridPaintManager::OleSetLineColor(COLORREF clr)
{
	GetItemMetrics()->m_clrLine = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetViewCategoryForeColor()
{
	return GetItemMetrics()->m_clrCategoryFore;
}

void CXTPPropertyGridPaintManager::OleSetViewCategoryForeColor(COLORREF clr)
{
	GetItemMetrics()->m_clrCategoryFore = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetViewReadOnlyForeColor()
{
	return GetItemMetrics()->m_clrReadOnlyFore;
}

void CXTPPropertyGridPaintManager::OleSetViewReadOnlyForeColor(COLORREF clr)
{
	GetItemMetrics()->m_clrReadOnlyFore = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetBackgroundColor()
{
	return m_clrFace;
}

void CXTPPropertyGridPaintManager::OleSetBackgroundColor(COLORREF clr)
{
	m_clrFace = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetShadowColor()
{
	return m_clrShadow;
}

void CXTPPropertyGridPaintManager::OleSetShadowColor(COLORREF clr)
{
	m_clrShadow = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetHighlightBackColor()
{
	return m_clrHighlight;
}

void CXTPPropertyGridPaintManager::OleSetHighlightBackColor(COLORREF clr)
{
	m_clrHighlight = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetHighlightForeColor()
{
	return m_clrHighlightText;
}

void CXTPPropertyGridPaintManager::OleSetHighlightForeColor(COLORREF clr)
{
	m_clrHighlightText = AxTranslateColor(clr);
}

COLORREF CXTPPropertyGridPaintManager::OleGetHyperlinkColor()
{
	return GetItemMetrics()->m_clrVerbFace;
}

void CXTPPropertyGridPaintManager::OleSetHyperlinkColor(COLORREF clr)
{
	GetItemMetrics()->m_clrVerbFace = AxTranslateColor(clr);
}

void CXTPPropertyGridPaintManager::OleSetItemsBorderHeight(long value)
{
	SetItemsBorderHeight((int)value);
}

long CXTPPropertyGridPaintManager::OleGetItemsBorderHeight()
{
	return (long)GetItemsBorderHeight();
}

///////////////////////////////////////////////////////////////////////////////
/// CXTPPropertyGridItemMetrics

BEGIN_DISPATCH_MAP(CXTPPropertyGridItemMetrics, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemMetrics, "ForeColor", 1, OleGetForeColor,
						OleSetForeColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemMetrics, "BackColor", 2, OleGetBackColor,
						OleSetBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemMetrics, "Font", DISPID_FONT, OleGetFont, OleSetFont,
						VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPPropertyGridItemMetrics, "IconIndex", 3, m_nImage, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItemMetrics, "DrawTextFormat", 4, m_uDrawTextFormat, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItemMetrics, "MaxLength", 5, m_nMaxLength, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridItemMetrics, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridItemMetrics, XTPDIID_IPropertyGridItemMetrics, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemMetrics, XTPDIID_IPropertyGridItemMetrics)

COLORREF CXTPPropertyGridItemMetrics::OleGetForeColor()
{
	return m_clrFore;
}

void CXTPPropertyGridItemMetrics::OleSetForeColor(COLORREF clr)
{
	m_clrFore.SetCustomValue(AxTranslateColor(clr));
}

COLORREF CXTPPropertyGridItemMetrics::OleGetBackColor()
{
	return m_clrBack;
}

void CXTPPropertyGridItemMetrics::OleSetBackColor(COLORREF clr)
{
	m_clrBack.SetCustomValue(AxTranslateColor(clr));
}

void CXTPPropertyGridItemMetrics::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_xtpFontNormal.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_xtpFontNormal.CreateFontIndirect(&lf);
	}
}

LPFONTDISP CXTPPropertyGridItemMetrics::OleGetFont()
{
	return AxCreateOleFont(m_xtpFontNormal.GetSafeHandle()
							   ? &m_xtpFontNormal
							   : &m_pPaintManager->GetItemMetrics()->m_xtpFontNormal,
						   this, (LPFNFONTCHANGED)&CXTPPropertyGridItemMetrics::OleSetFont);
}

#endif
