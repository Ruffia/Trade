// XTPCommandBarsNativeXPTheme.cpp : implementation of the CXTPCommandBarsNativeXPTheme class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPDockBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlEdit.h"

#include "CommandBars/Themes/XTPCommandBarsOffice2000Theme.h"
#include "CommandBars/Themes/XTPCommandBarsNativeXPTheme.h"

#define COLOR_MENUHILIGHT 29

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SPI_GETFLATMENU
#	define SPI_GETFLATMENU 0x1022
#endif

IMPLEMENT_DYNAMIC(CXTPCommandBarsNativeXPTheme, CXTPCommandBarsOffice2000Theme)

CXTPCommandBarsNativeXPTheme::CXTPCommandBarsNativeXPTheme()
{
	m_iconsInfo.bUseFadedIcons   = TRUE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	m_nPopupBarText = COLOR_MENUTEXT;
	m_bFlatMenus	= TRUE;

	m_bThemedStatusBar		 = TRUE;
	m_bThemedCheckBox		 = TRUE;
	m_bSelectImageInPopupBar = TRUE;
	m_bThickCheckMark		 = TRUE;

	m_themeRebar   = new CXTPWinThemeWrapper();
	m_themeToolbar = new CXTPWinThemeWrapper();
	m_themeCombo   = new CXTPWinThemeWrapper();
	m_themeWindow  = new CXTPWinThemeWrapper();
	m_themeSpin	= new CXTPWinThemeWrapper();
	m_themeMenu	= new CXTPWinThemeWrapper();
}

CXTPCommandBarsNativeXPTheme::~CXTPCommandBarsNativeXPTheme()
{
	SAFE_DELETE(m_themeRebar);
	SAFE_DELETE(m_themeToolbar);
	SAFE_DELETE(m_themeCombo);
	SAFE_DELETE(m_themeWindow);
	SAFE_DELETE(m_themeSpin);
	SAFE_DELETE(m_themeMenu);
}

void CXTPCommandBarsNativeXPTheme::RefreshMetrics()
{
	CXTPCommandBarsOffice2000Theme::RefreshMetrics();

	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
	m_themeToolbar->OpenThemeData(hWnd, L"TOOLBAR");
	m_themeRebar->OpenThemeData(hWnd, L"REBAR");
	m_themeCombo->OpenThemeData(hWnd, L"COMBOBOX");
	m_themeWindow->OpenThemeData(hWnd, L"WINDOW");
	m_themeSpin->OpenThemeData(hWnd, L"SPIN");
	m_themeMenu->OpenThemeData(hWnd, L"MENU");

	m_bFlatMenus = TRUE;
	if (!SystemParametersInfo(SPI_GETFLATMENU, 0, &m_bFlatMenus, 0))
	{
		m_bFlatMenus = FALSE;
	}

	if (!m_themeRebar->IsAppThemed()
		|| FAILED(
			   m_themeRebar->GetThemeColor(RP_BAND, 0, TMT_EDGESHADOWCOLOR, &m_clrEdgeShadowColor)))
		m_clrEdgeShadowColor = GetXtremeColor(COLOR_3DSHADOW);

	if (!m_themeRebar->IsAppThemed()
		|| FAILED(m_themeRebar->GetThemeColor(RP_BAND, 0, TMT_EDGEHIGHLIGHTCOLOR,
											  &m_clrEdgeHighLightColor)))
		m_clrEdgeHighLightColor = GetXtremeColor(COLOR_BTNHILIGHT);

	COLORREF clr;
	if (m_themeToolbar->IsAppThemeActive()
		&& SUCCEEDED(m_themeToolbar->GetThemeColor(0, 0, TMT_TEXTCOLOR, &clr)))
		m_arrColor[COLOR_BTNTEXT].SetStandardValue(clr);

	m_clrSelectedText = m_arrColor[COLOR_BTNTEXT];
	if (m_themeToolbar->IsAppThemeActive()
		&& SUCCEEDED(m_themeToolbar->GetThemeColor(TP_BUTTON, TS_HOT, TMT_TEXTCOLOR, &clr)))
		m_clrSelectedText = clr;

	m_clrPressedText = m_arrColor[COLOR_BTNTEXT];
	if (m_themeToolbar->IsAppThemeActive()
		&& SUCCEEDED(m_themeToolbar->GetThemeColor(TP_BUTTON, TS_PRESSED, TMT_TEXTCOLOR, &clr)))
		m_clrPressedText = clr;

	if (!m_bFlatMenus && m_themeRebar->IsAppThemeActive())
	{
		m_bFlatMenus = m_themeRebar->GetThemeSysBool(TMT_FLATMENUS);
	}

	if (!m_bFlatMenus)
	{
		m_arrColor[COLOR_MENUHILIGHT].SetStandardValue(m_arrColor[COLOR_HIGHLIGHT]);
	}
}

void CXTPCommandBarsNativeXPTheme::DrawControlEntry(CDC* pDC, CXTPControl* pButton)
{
	CXTPCommandBar* pParent = pButton->GetParent();

	if (!IsThemeEnabled() || !pParent)
	{
		CXTPCommandBarsOffice2000Theme::DrawControlEntry(pDC, pButton);
		return;
	}

	if (pButton->GetType() == xtpControlPopup && pParent->GetPosition() != xtpBarPopup
		&& IsFlatToolBar(pParent) && pParent->GetType() == xtpBarTypeMenuBar)
	{
		if (pButton->GetSelected() || pButton->GetPopuped() || pButton->GetPressed())
		{
			pDC->FillSolidRect(pButton->GetRect(), GetXtremeColor(COLOR_HIGHLIGHT));
		}
	}
	else
	{
		CXTPCommandBarsOffice2000Theme::DrawControlEntry(pDC, pButton);
	}
}

COLORREF CXTPCommandBarsNativeXPTheme::GetControlTextColor(CXTPControl* pButton)
{
	CXTPCommandBar* pParent = pButton->GetParent();

	if (!IsThemeEnabled() || !pParent)
		return CXTPCommandBarsOffice2000Theme::GetControlTextColor(pButton);

	if (pButton->GetType() == xtpControlPopup && pParent->GetPosition() != xtpBarPopup
		&& IsFlatToolBar(pParent) && pParent->GetType() == xtpBarTypeMenuBar)
	{
		if (pButton->GetSelected() || pButton->GetPopuped() || pButton->GetPressed())
		{
			return GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		}
	}
	return CXTPCommandBarsOffice2000Theme::GetControlTextColor(pButton);
}

void CXTPCommandBarsNativeXPTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed,
												 BOOL bEnabled, BOOL bChecked, BOOL bPopuped,
												 XTPBarType barType, XTPBarPosition barPosition)
{
	if (!IsThemeEnabled())
	{
		CXTPCommandBarsOffice2000Theme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled,
													  bChecked, bPopuped, barType, barPosition);
		return;
	}

	if (barType == xtpBarTypePopup && !bChecked)
	{
		if (bSelected || bPressed)
		{
			BOOL bDrawn = FALSE;
			if (m_themeMenu->IsAppThemeActive())
			{
				bDrawn = SUCCEEDED(m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(),
																	XTP_MP_POPUPITEM, 2, rc, NULL));
			}

			if (!bDrawn)
			{
				Rectangle(pDC, rc, COLOR_HIGHLIGHT, COLOR_MENUHILIGHT);
			}
		}
		return;
	}

	if (m_bSelectImageInPopupBar && barPosition == xtpBarPopup && barType == xtpBarTypePopup
		&& bChecked && m_themeMenu->IsAppThemeActive())
	{
		m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPCHECKBACKGROUND,
										 bEnabled ? 2 : 1, rc, NULL);
		return;
	}

	if (bChecked == 2 && bEnabled)
		bChecked = FALSE;

	int nState = !bEnabled ? (bChecked ? TS_PRESSED : TS_DISABLED)
						   : bPopuped ? TS_PRESSED
									  : bChecked && !bPressed
											? (bSelected ? TS_HOTCHECKED : TS_CHECKED)
											: bSelected && bPressed || IsKeyboardSelected(bPressed)
												  ? TS_PRESSED
												  : bSelected ? TS_HOT : TS_NORMAL;

	m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, nState, &rc, 0);
}

BOOL CXTPCommandBarsNativeXPTheme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	return CXTPCommandBarsOffice2000Theme::DrawRectangle(pDC, pButton);
}

void CXTPCommandBarsNativeXPTheme::DrawPopupBarRadioMark(CDC* pDC, CRect rc, BOOL bEnabled,
														 COLORREF clr)
{
	ASSERT_VALID(pDC);

	if (m_themeMenu->IsAppThemeActive())
	{
		m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPCHECK, bEnabled ? 3 : 4, rc,
										 NULL);
	}
	else
	{
		CXTPCommandBarsOffice2000Theme::DrawPopupBarRadioMark(pDC, rc, bEnabled, clr);
	}
}

void CXTPCommandBarsNativeXPTheme::DrawPopupBarCheckMark(CDC* pDC, CRect rc, BOOL bEnabled,
														 COLORREF clr)
{
	if (m_themeMenu->IsAppThemeActive())
	{
		if (FAILED(m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPCHECK,
													bEnabled ? 1 : 2, rc, NULL)))
			CXTPCommandBarsOffice2000Theme::DrawPopupBarCheckMark(pDC, rc, bEnabled, clr);
	}
	else
	{
		CXTPCommandBarsOffice2000Theme::DrawPopupBarCheckMark(pDC, rc, bEnabled, clr);
	}
}

int CXTPCommandBarsNativeXPTheme::GetPopupBarGripperWidth(CXTPCommandBar* pBar)
{
	int nWidth = CXTPCommandBarsOffice2000Theme::GetPopupBarGripperWidth(pBar);
	if (m_themeMenu->IsAppThemed())
		nWidth += XTP_DPI_X(3);
	return nWidth;
}

void CXTPCommandBarsNativeXPTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	if (!IsThemeEnabled())
	{
		CXTPCommandBarsOffice2000Theme::FillCommandBarEntry(pDC, pBar);
		return;
	}
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarPopup)
	{
		if (m_themeMenu->IsAppThemeActive())
		{
			if (SUCCEEDED(m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPBORDERS,
														   0, &rc, NULL)))
			{
				rc.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
				m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPBACKGROUND, 0, &rc,
												 NULL);
				return;
			}
		}
		if (m_bFlatMenus)
		{
			Rectangle(pDC, rc, COLOR_3DSHADOW, COLOR_MENU);
		}
		else
		{
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_MENU));
			Draw3dRect(pDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW);
			rc.DeflateRect(1, 1);
			Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		}
	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		if (pBar->IsDialogBar())
		{
			CXTPCommandBarsOffice2000Theme::FillCommandBarEntry(pDC, pBar);
			return;
		}

		if (pBar->GetType() == xtpBarTypePopup)
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_MENU));
		else
		{
			pDC->FillSolidRect(rc, GetXtremeColor(UINT(pBar->GetType() == xtpBarTypePopup
														   ? COLOR_MENU
														   : COLOR_3DFACE)));
			m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rc, NULL);
		}

		m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMELEFT, CS_ACTIVE,
										   CRect(rc.left, rc.top, rc.left + XTP_DPI_X(3),
												 rc.bottom),
										   0);
		m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMERIGHT, CS_ACTIVE,
										   CRect(rc.right - XTP_DPI_X(3), rc.top, rc.right,
												 rc.bottom),
										   0);
		m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMEBOTTOM, CS_ACTIVE,
										   CRect(rc.left, rc.bottom - XTP_DPI_Y(3), rc.right,
												 rc.bottom),
										   0);
	}
	else
	{
		if (pBar->GetPosition() == xtpBarTop)
			FillDockBarRect(pDC, pBar, pBar->GetParent());
		else
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), RP_BAND, 0, &rc, NULL);

		if (!IsFlatToolBar(pBar))
		{
			pDC->Draw3dRect(rc, m_clrEdgeHighLightColor, m_clrEdgeShadowColor);
		}
	}
}

void CXTPCommandBarsNativeXPTheme::FillDockBarRect(CDC* pDC, CWnd* pWnd, CWnd* pParent)
{
	if (IsThemeEnabled())
	{
		CRect rc, rcParent, rcClient, rcClipRect;
		pWnd->GetClientRect(&rcClient);
		rcClipRect = rcClient;

		if (pParent)
		{
			pParent->GetWindowRect(rcClient);
			pWnd->ScreenToClient(rcClient);
		}

		m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, &rcClipRect);
	}
	else
		pDC->FillSolidRect(CXTPClientRect(pWnd), GetXtremeColor(COLOR_3DFACE));
}

void CXTPCommandBarsNativeXPTheme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	if (pBar->GetPosition() == xtpBarTop)
		FillDockBarRect(pDC, pBar, pBar);
	else
		pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));
}

CSize CXTPCommandBarsNativeXPTheme::DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* pBar, BOOL bDraw)
{
	if (!IsThemeEnabled())
	{
		return CXTPCommandBarsOffice2000Theme::DrawDialogBarGripper(pDC, pBar, bDraw);
	}

	CSize sz(XTP_DPI_X(8), max(XTP_DPI_Y(22), m_nTextHeight + XTP_DPI_Y(4)));

	if (pDC && bDraw)
	{
		CRect rc(XTP_DPI_X(2), XTP_DPI_Y(3), XTP_DPI_X(8), sz.cy);
		m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), RP_GRIPPER, 1, &rc, 0);
	}

	return sz;
}

BOOL CXTPCommandBarsNativeXPTheme::IsThemeEnabled() const
{
	return m_themeRebar->IsAppThemed() && m_themeToolbar->IsAppThemed();
}

CSize CXTPCommandBarsNativeXPTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
															CXTPControl* pControl, BOOL bDraw)
{
	if (!IsThemeEnabled() || !bDraw)
	{
		return CXTPCommandBarsOffice2000Theme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);
	}

	CRect rc;
	pBar->GetClientRect(&rc);

	CRect rcControl = pControl->GetRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		if (pBar->GetPosition() == xtpBarPopup && m_themeMenu->IsAppThemeActive()
			&& !pControl->GetWrap())
		{
			m_themeMenu->DrawThemeBackground(
				pDC->GetSafeHdc(), XTP_MP_POPUPSEPARATOR, 0,
				CRect(rcControl.left + GetPopupBarGripperWidth(pControl->GetParent()),
					  rcControl.top - 8, rcControl.right, rcControl.top - 2),
				NULL);
			return 0;
		}
		else
		{
			return CXTPCommandBarsOffice2000Theme::DrawCommandBarSeparator(pDC, pBar, pControl,
																		   bDraw);
		}
	}

	CRect rcRow = pControl->GetRowRect();

	if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
			m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_SEPARATORVERT, 0,
												CRect(rcRow.left, rcControl.top - XTP_DPI_Y(6),
													  rcRow.right, rcControl.top),
												NULL);
		else
			m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_SEPARATOR, 0,
												CRect(rcRow.right, rcRow.top + XTP_DPI_Y(4),
													  rcRow.right + XTP_DPI_X(5), rcRow.bottom),
												NULL);
	}
	else
	{
		if (!pControl->GetWrap())
			m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_SEPARATOR, 0,
												CRect(rcControl.left - XTP_DPI_X(6), rcRow.top,
													  rcControl.left, rcRow.bottom),
												NULL);
		else
			m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_SEPARATORVERT, 0,
												CRect(rcRow.left + XTP_DPI_X(4),
													  rcRow.top - XTP_DPI_Y(5), rcRow.right,
													  rcRow.top),
												NULL);
	}
	return 0;
}

void CXTPCommandBarsNativeXPTheme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy,
													   BOOL /*bExpanded*/)
{
	if (m_themeMenu->IsAppThemeActive())
	{
		CRect rc(CPoint(x, y), CSize(cx, cy));
		m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPGUTTER, 0, &rc, NULL);
	}
}

CSize CXTPCommandBarsNativeXPTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
														  BOOL bDraw)
{
	if (!IsThemeEnabled())
	{
		return CXTPCommandBarsOffice2000Theme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}

	if (pBar->IsDialogBar())
		return DrawDialogBarGripper(pDC, (CXTPDialogBar*)pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetType() == xtpBarTypePopup && pBar->GetPosition() == xtpBarPopup
		&& m_themeMenu->IsAppThemeActive())
	{
		rc.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
		rc.right = rc.left + GetPopupBarGripperWidth(pBar);
		m_themeMenu->DrawThemeBackground(pDC->GetSafeHdc(), XTP_MP_POPUPGUTTER, 0, &rc, NULL);
	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		ASSERT(pDC);
		if (!pDC)
			return CSize(0, 0);
		CXTPFontDC font(pDC, GetSmCaptionFont());
		CSize sz = pDC->GetTextExtent(_T(" "), 1);

		rc.SetRect(0, 0, rc.right, XTP_DPI_Y(3) + max(XTP_DPI_Y(15), sz.cy));

		if (pDC && bDraw)
		{
			m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLCAPTION, CS_ACTIVE, rc,
											   0);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_clrFloatingGripperText);
			pDC->DrawText(pBar->GetTitle(),
						  CRect(XTP_DPI_X(5), XTP_DPI_Y(3),
								rc.right + XTP_DPI_X(3) - 2 * rc.Size().cy, rc.bottom),
						  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
		}
		return CSize(rc.Width(), max(XTP_DPI_Y(15), sz.cy));
	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			rc.top += XTP_DPI_Y(2);
			rc.bottom = rc.top + XTP_DPI_Y(6);
			m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), RP_GRIPPERVERT, 1, &rc, 0);
		}
		return CSize(0, XTP_DPI_Y(8));
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			rc.left += XTP_DPI_X(2);
			rc.right = rc.left + XTP_DPI_X(6);
			m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), RP_GRIPPER, 1, &rc, 0);
		}
		return CSize(XTP_DPI_X(8), 0);
	}
	return 0;
}

void CXTPCommandBarsNativeXPTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl,
													 BOOL bVertical)
{
	if (!IsThemeEnabled())
	{
		CXTPCommandBarsOffice2000Theme::AdjustExcludeRect(rc, pControl, bVertical);
		return;
	}
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);
	if (!pParent)
		return;

	if (pControl->GetType() == xtpControlComboBox)
		return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.DeflateRect(0, 0);
		else
			rc.DeflateRect(0, 0);
	}
	else
		rc.InflateRect(-XTP_DPI_X(3), XTP_DPI_Y(3));
}

void CXTPCommandBarsNativeXPTheme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton,
														CRect rcButton)
{
	if (!IsThemeEnabled())
	{
		CXTPCommandBarsOffice2000Theme::DrawSplitButtonFrame(pDC, pButton, rcButton);
		return;
	}

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		 bPopuped = pButton->GetPopuped();

	int nState = !bEnabled
					 ? TS_DISABLED
					 : bPressed && bSelected
						   ? TS_PRESSED
						   : bChecked ? TS_CHECKED : bPopuped || bSelected ? TS_HOT : TS_NORMAL;

	CRect rc(rcButton.left, rcButton.top, rcButton.right - XTP_DPI_X(12), rcButton.bottom);
	m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_SPLITBUTTON, nState, &rc, 0);

	nState = bPopuped ? TS_PRESSED : nState;
	if (bEnabled == TRUE_SPLITCOMMAND)
		nState = TS_DISABLED;

	CRect rcArrow(rcButton.right - XTP_DPI_X(12), rcButton.top, rcButton.right, rcButton.bottom);
	m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_SPLITBUTTONDROPDOWN, nState, &rcArrow,
										0);
}

void CXTPCommandBarsNativeXPTheme::DrawControlEditSpin(CDC* pDC, CXTPControlEdit* pControlEdit)
{
	if (!IsThemeEnabled())
	{
		CXTPCommandBarsOffice2000Theme::DrawControlEditSpin(pDC, pControlEdit);
		return;
	}

	BOOL bPressed  = pControlEdit->GetPressed();
	BOOL bEnabled  = pControlEdit->GetEnabled();
	BOOL bSelected = pControlEdit->GetSelected();

	CRect rcSpin(pControlEdit->GetSpinButtonsRect());
	CRect rcTop(rcSpin.left, rcSpin.top, rcSpin.right, rcSpin.CenterPoint().y);
	CRect rcBottom(rcSpin.left, rcTop.bottom, rcSpin.right, rcSpin.bottom);

	m_themeSpin->DrawThemeBackground(pDC->GetSafeHdc(), SPNP_UP,
									 !bEnabled ? UPS_DISABLED
											   : bSelected && bPressed == XTP_EDITSPIN_UP
													 ? UPS_PRESSED
													 : bSelected == XTP_EDITSPIN_UP ? UPS_HOT
																					: UPS_NORMAL,
									 rcTop, NULL);

	m_themeSpin->DrawThemeBackground(pDC->GetSafeHdc(), SPNP_DOWN,
									 !bEnabled ? DNS_DISABLED
											   : bSelected && bPressed == XTP_EDITSPIN_DOWN
													 ? DNS_PRESSED
													 : bSelected == XTP_EDITSPIN_DOWN ? DNS_HOT
																					  : DNS_NORMAL,
									 rcBottom, NULL);
}

void CXTPCommandBarsNativeXPTheme::DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled,
														BOOL bSelected)
{
	ASSERT_VALID(pDC);

	if (!m_themeCombo->IsAppThemed())
	{
		CXTPCommandBarsOffice2000Theme::DrawControlEditFrame(pDC, rc, bEnabled, bSelected);
		return;
	}

	int nStateID = !bEnabled ? CBXS_DISABLED : CBXS_NORMAL;

	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	m_themeCombo->GetThemeColor(CP_DROPDOWNBUTTON, nStateID, TMT_BORDERCOLOR, &clrFrame);

	pDC->Draw3dRect(rc, clrFrame, clrFrame);
}

void CXTPCommandBarsNativeXPTheme::DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled,
															 BOOL bSelected, BOOL bDropped)
{
	if (!m_themeCombo->IsAppThemed())
	{
		CXTPCommandBarsOffice2000Theme::DrawControlComboBoxButton(pDC, rcBtn, bEnabled, bSelected,
																  bDropped);
		return;
	}

	int nStateID = !bEnabled ? CBXS_DISABLED
							 : bDropped ? CBXS_PRESSED : bSelected ? CBXS_HOT : CBXS_NORMAL;

	rcBtn.DeflateRect(-XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
	m_themeCombo->DrawThemeBackground(pDC->GetSafeHdc(), CP_DROPDOWNBUTTON, nStateID, &rcBtn, 0);
}

CSize CXTPCommandBarsNativeXPTheme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType,
													   CXTPControl* pButton, CXTPCommandBar* pBar,
													   BOOL bDraw, LPVOID lpParam)
{
	if ((controlType == xtpButtonExpandFloating || controlType == xtpButtonHideFloating)
		&& m_themeToolbar->IsAppThemed())
	{
		return CXTPPaintManager::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw,
													lpParam);
	}

	return CXTPCommandBarsOffice2000Theme::DrawSpecialControl(pDC, controlType, pButton, pBar,
															  bDraw, lpParam);
}

COLORREF CXTPCommandBarsNativeXPTheme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed,
															 BOOL bEnabled, BOOL bChecked,
															 BOOL bPopuped, XTPBarType barType,
															 XTPBarPosition barPosition)
{
	if (barType == xtpBarTypePopup && barPosition == xtpBarPopup && m_themeMenu->IsAppThemed())
	{
		if (XTPSystemVersion()->IsWin11OrGreater())
		{
			return GetXtremeColor(XTPToUIntChecked(
				!bEnabled ? COLOR_GRAYTEXT
						  : bSelected && !bChecked ? COLOR_HIGHLIGHTTEXT : m_nPopupBarText));
		}
		else
		{
			return GetXtremeColor(XTPToUIntChecked(!bEnabled ? COLOR_GRAYTEXT : m_nPopupBarText));
		}
	}

	if (!IsThemeEnabled() || barType == xtpBarTypePopup || barPosition == xtpBarPopup)
		return CXTPCommandBarsOffice2000Theme::GetRectangleTextColor(bSelected, bPressed, bEnabled,
																	 bChecked, bPopuped, barType,
																	 barPosition);

	if (!bEnabled)
		return GetXtremeColor(COLOR_GRAYTEXT);

	if (bSelected && bPressed)
		return m_clrPressedText;

	if (bSelected)
		return m_clrSelectedText;

	return GetXtremeColor(COLOR_BTNTEXT);
}

void CXTPCommandBarsNativeXPTheme::DrawControlMDIButton(CDC* pDC, CXTPControlButton* pButton)
{
	if (m_themeWindow->IsAppThemeActive())
	{
		int nPartID = pButton->GetID() == XTP_ID_MENUBAR_RESTORE
						  ? WP_MDIRESTOREBUTTON
						  : pButton->GetID() == XTP_ID_MENUBAR_MINIMIZE ? WP_MDIMINBUTTON
																		: WP_MDICLOSEBUTTON;

		int nState = !pButton->GetEnabled()
						 ? SBS_DISABLED
						 : pButton->GetPressed() ? SBS_PUSHED
												 : pButton->GetSelected() ? SBS_HOT : SBS_NORMAL;

		if (m_themeWindow->DrawThemeBackground(*pDC, nPartID, nState, pButton->GetRect(), NULL)
			== S_OK)
			return;
	}

	CXTPPaintManager::DrawControlMDIButton(pDC, pButton);
}
