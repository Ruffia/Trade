// XTPShortcutBarOffice2013Theme.cpp : implementation of the CXTPShortcutBarOffice2013Theme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"

#include "ShortcutBar/XTPShortcutBarPaintManager.h"
#include "ShortcutBar/XTPShortcutBar.h"
#include "ShortcutBar/XTPShortcutBarPane.h"
#include "ShortcutBar/Themes/XTPShortcutBarOffice2003Theme.h"
#include "ShortcutBar/Themes/XTPShortcutBarResourceTheme.h"
#include "ShortcutBar/Themes/XTPShortcutBarOffice2013Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOffice2013Theme

CXTPShortcutBarOffice2013Theme::CXTPShortcutBarOffice2013Theme()
{
	EnableOfficeFont();
}

CXTPShortcutBarOffice2013Theme::~CXTPShortcutBarOffice2013Theme()
{
}

void CXTPShortcutBarOffice2013Theme::RefreshMetrics()
{
	CXTPShortcutBarResourceTheme::RefreshMetrics();

	CreateFonts();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_xtpFontPaneItemCaption);
	m_nPaneItemHeight = max(XTP_DPI_Y(19), dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(4));

	m_clrNormalText.SetStandardValue(XTPIniColor(_T("ShortcutBar"), _T("Text"), RGB(68, 68, 68)));
	m_clrHotText.SetStandardValue(XTPIniColor(_T("ShortcutBar"), _T("TextHot"), RGB(68, 68, 68)));
	m_clrPushedText.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("TextPushed"), RGB(68, 68, 68)));
	m_clrSelectedText.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("TextSelected"), RGB(68, 68, 68)));
	m_clrDisabledText.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("TextDisabled"), RGB(177, 177, 177)));
	m_clrCaptionText.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("TextCaption"), RGB(68, 68, 68)));
	m_clrItemCaptionText.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("TextItemCaption"), RGB(68, 68, 68)));
	m_clrBack.SetStandardValue(XTPIniColor(_T("ShortcutBar"), _T("Back"), RGB(255, 255, 255)));
	m_clrBackHilite.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BackHighlight"), RGB(213, 225, 242)));
	m_clrBackPushed.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BackPushed"), RGB(163, 189, 227)));
	m_clrBackPushedDark.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BackPushedDark"), RGB(43, 87, 154)));
	m_clrBackSelected.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BackSelected"), RGB(194, 213, 242)));
	m_grcCaption.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BackCaption"), RGB(255, 255, 255)));
	m_clrPopupBackground.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BackPopup"), RGB(255, 255, 255)));
	m_clrPopupFrame.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("BorderPopup"), RGB(171, 171, 171)));
	m_clrGlyph.SetStandardValue(XTPIniColor(_T("ShortcutBar"), _T("Glyph"), RGB(131, 134, 136)));
	m_clrGlyphFill.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("GlyphFill"), RGB(255, 255, 255)));
	m_clrGlyphExpanded.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("GlyphExpanded"), RGB(68, 68, 68)));
	m_clrGlyphPushed.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("GlyphPushed"), RGB(255, 255, 255)));
	m_grcShortcutBarGripper.SetStandardValue(
		XTPIniColor(_T("ShortcutBar"), _T("Gripper"), RGB(212, 212, 212)));
	m_clrAppSplitterFace = XTPIniColor(_T("ShortcutBar"), _T("SplitterFace"), RGB(255, 255, 255));
	m_bFlatStyle		 = XTPIniInt(_T("ShortcutBar"), _T("FlatStyle"), TRUE);

	m_clrShortcutItemShadow.SetStandardValue(m_clrBack);
	m_clrFlatBackground = m_clrBack;
	m_clrFlatTextColor  = m_clrNormalText;

	m_bShowBorder				= !m_bFlatStyle;
	m_bShowTopLeftCaptionShadow = !m_bFlatStyle;
	m_bBoldItemCaptionFont		= !m_bFlatStyle;

	m_bNavigationButton = TRUE;
}

void CXTPShortcutBarOffice2013Theme::DrawPaneCaptionMinimizeButton(CDC* pDC,
																   CXTPShortcutBarPane* pPane)
{
	CRect rc = pPane->GetMinimizeButtonRect();
	if (!rc.IsRectEmpty())
	{
		COLORREF clrArrow = m_clrGlyphExpanded;

		if (pPane->IsMinimizeButtonPressed())
		{
			pDC->FillSolidRect(rc, m_clrBackPushedDark);
			clrArrow = m_clrGlyphPushed;
		}
		else if (pPane->IsMinimizeButtonHighlighted())
		{
			pDC->FillSolidRect(rc, m_clrBackPushed);
		}

		if (pPane->GetShortcutBar()->IsShortcutBarMinimized())
		{
			XTPDrawHelpers()->DrawExpandArrow(pDC, rc, FALSE, clrArrow);
		}
		else
		{
			XTPDrawHelpers()->DrawExpandArrow(pDC, rc, TRUE, clrArrow);
		}
	}
}

int CXTPShortcutBarOffice2013Theme::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem,
														BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_xtpFontPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();

		if (pItem->IsHighlighted())
		{
			pDC->FillSolidRect(rc, m_clrBackHilite);
		}

		CRect rcText(rc);
		rcText.DeflateRect(m_rcItemCaptionPadding);

		if (pItem->IsExpandable())
		{
			if (pItem->IsExpanded())
			{
				CPoint pt(rc.left + XTP_DPI_X(7), rc.top + XTP_DPI_Y(7));
				XTPDrawHelpers()->DrawExpandTriangle(pDC, pt, TRUE, m_clrGlyphExpanded);
			}
			else
			{
				CPoint pt(rc.left + XTP_DPI_X(8), rc.top + XTP_DPI_Y(5));
				XTPDrawHelpers()->DrawExpandTriangle(pDC, pt, FALSE, m_clrGlyph, m_clrGlyphFill);
			}

			rcText.left = rc.left + XTP_DPI_X(20);
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clrItemCaptionText);
		pDC->DrawText(pItem->GetCaption(), rcText,
					  XTPToUInt(DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS
								| (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0)));
	}

	return m_nPaneItemHeight;
}

void CXTPShortcutBarOffice2013Theme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);
	pDC->FillSolidRect(rc, m_clrBack);
}

void CXTPShortcutBarOffice2013Theme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc					  = pItem->GetItemRect();
	CXTPClientRect rcShortcutBar(pShortcutBar);

	if (pItem->IsSelected() && pShortcutBar->GetHotItem() == pItem)
	{
		pDC->FillSolidRect(rc, m_clrBackHilite);
		pDC->Draw3dRect(rc, m_clrBackPushed, m_clrBackPushed);
		pDC->SetTextColor(m_clrSelectedText);
	}
	else if (pShortcutBar->GetPressedItem() == pItem)
	{
		pDC->FillSolidRect(rc, m_clrBackPushed);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if (pShortcutBar->GetHotItem() == pItem)
	{
		pDC->FillSolidRect(rc, m_clrBackHilite);
		pDC->SetTextColor(m_clrHotText);
	}
	else if (pItem->IsSelected())
	{
		pDC->FillSolidRect(rc, m_clrBackSelected);
		pDC->SetTextColor(m_clrSelectedText);
	}
}

void CXTPShortcutBarOffice2013Theme::FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CRect rc = pItem->GetItemRect();

	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();

	if (pShortcutBar->GetPressedItem() == pItem)
	{
		pDC->FillSolidRect(rc, m_clrBackPushed);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if (pShortcutBar->GetHotItem() == pItem)
	{
		pDC->FillSolidRect(rc, m_clrBackHilite);
		pDC->SetTextColor(m_clrHotText);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrBack);
		pDC->SetTextColor(m_clrNormalText);
	}
}

void CXTPShortcutBarOffice2013Theme::DrawExpandButton(CDC* pDC, CRect rc)
{
	CRect rcArrow = rc;
	rcArrow.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow, pDC->GetTextColor());
}

int CXTPShortcutBarOffice2013Theme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();

		int x  = rc.left + XTP_DPI_X(4);
		int y  = rc.top + (rc.Height() / 2);
		int cx = rc.Width() - XTP_DPI_X(8);
		int cy = 1;

		pDC->FillSolidRect(x, y, cx, cy, m_grcShortcutBarGripper);
	}

	return XTP_DPI_Y(7);
}

void CXTPShortcutBarOffice2013Theme::FillPopupBack(CDC* pDC, CRect rc, CSize szGap)
{
	UNREFERENCED_PARAMETER(szGap);
	pDC->FillSolidRect(rc, m_clrPopupBackground);
	pDC->Draw3dRect(rc, m_clrPopupFrame, m_clrPopupFrame);
}
