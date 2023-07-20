// XTPCommandBarsOfficeXPTheme.cpp : implementation of the CXTPCommandBarsOfficeXPTheme class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPCommandBars.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOfficeXP.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCommandBarsOfficeXPTheme, CXTPPaintManager)

CXTPCommandBarsOfficeXPTheme::CXTPCommandBarsOfficeXPTheme()
{
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeOfficeXP(this);

	m_bFlatMenuBar = TRUE;

	m_iconsInfo.bUseFadedIcons			  = TRUE;
	m_iconsInfo.bIconsWithShadow		  = TRUE;
	m_iconsInfo.bUseDisabledIcons		  = TRUE;
	m_iconsInfo.bOfficeStyleDisabledIcons = TRUE;

	m_bEmbossedDisabledText = FALSE;

	m_pShadowManager->SetShadowOptions(xtpShadowOfficeAlpha + xtpShadowShowPopupControl);

	m_nPopupBarTextPadding = XTP_DPI_X(8);
}

void CXTPCommandBarsOfficeXPTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed,
												 BOOL bEnabled, BOOL bChecked, BOOL bPopuped,
												 XTPBarType barType, XTPBarPosition barPosition,
												 int nID)
{
	UNREFERENCED_PARAMETER(nID);
	DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
}

void CXTPCommandBarsOfficeXPTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed,
												 BOOL bEnabled, BOOL bChecked, BOOL bPopuped,
												 XTPBarType barType, XTPBarPosition barPosition)
{
	UNREFERENCED_PARAMETER(barType);

	if (!bEnabled)
	{
		if (IsKeyboardSelected(bSelected))
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER,
					  barPosition != xtpBarPopup
						  ? (bChecked ? XPCOLOR_HIGHLIGHT_PUSHED : XPCOLOR_HIGHLIGHT)
						  : XPCOLOR_MENUBAR_FACE);
		}
		else if (bChecked)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_DISABLED_BORDER,
					  barPosition != xtpBarPopup ? XPCOLOR_HIGHLIGHT_CHECKED : -1);
		}
	}
	else
	{
		if (bPopuped)
			Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_TOOLBAR_FACE);
		else if (bChecked == 2 && !bSelected && !bPressed)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_3DFACE);
		else if (bChecked && !bSelected && !bPressed)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED);
		else if (bChecked && bSelected && !bPressed)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed))
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
		else if (bSelected || bPressed)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
	}
}

BOOL CXTPCommandBarsOfficeXPTheme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	return CXTPPaintManager::DrawRectangle(pDC, pButton);
}

void CXTPCommandBarsOfficeXPTheme::DrawImage(CDC* pDC, CPoint pt, CSize sz,
											 CXTPImageManagerIcon* pImage, BOOL bSelected,
											 BOOL bPressed, BOOL bEnabled, BOOL bChecked,
											 BOOL bPopuped, BOOL)
{
	if (!bEnabled)
	{
		if (pImage->IsRasterIcon())
		{
			if (!m_iconsInfo.bUseDisabledIcons && !pImage->IsAlpha() && pImage->IsRasterIcon())
			{
				pImage->DrawMono(pDC, pt, pImage->GetIcon(), sz, GetXtremeColor(XPCOLOR_DISABLED));
			}
			else
			{
				if (m_iconsInfo.bOfficeStyleDisabledIcons && !pImage->HasDisabledIcon()
					&& pImage->IsRasterIcon())
				{
					pImage->CreateDisabledIcon(m_clrDisabledIcon.clrLight,
											   m_clrDisabledIcon.clrDark);
				}

				pImage->Draw(pDC, pt, xtpImageDisabled, sz);
			}
		}
		else
		{
			pImage->Draw(pDC, pt, xtpImageDisabled, sz);
		}
	}
	else if (bChecked == 2 && !bPressed)
	{
		if (pImage->IsRasterIcon())
		{
			if (!pImage->IsAlpha())
			{
				pImage->DrawMono(pDC, pt, pImage->GetIcon(), sz, GetXtremeColor(XPCOLOR_DISABLED));
			}
			else
			{
				if (m_iconsInfo.bOfficeStyleDisabledIcons && !pImage->HasDisabledIcon())
				{
					pImage->CreateDisabledIcon(m_clrDisabledIcon.clrLight,
											   m_clrDisabledIcon.clrDark);
				}

				pImage->Draw(pDC, pt, xtpImageDisabled, sz);
			}
		}
		else
		{
			pImage->Draw(pDC, pt, xtpImageDisabled, sz);
		}
	}
	else if (bPopuped || bChecked)
	{
		pImage->Draw(pDC, pt, bChecked ? xtpImageChecked : xtpImageNormal, sz);
	}
	else if (!bSelected && !bPressed)
	{
		pImage->Draw(pDC, pt, xtpImageNormal, sz);
	}
	else if ((bSelected && !bPressed) || (!bSelected && bPressed == TRUE))
	{
		if (pImage->IsRasterIcon() && m_iconsInfo.bIconsWithShadow
			&& !pImage->GetShadowIcon().IsEmpty())
		{
			pImage->Draw(pDC, CPoint(pt.x + XTP_DPI_X(1), pt.y + XTP_DPI_Y(1)),
						 pImage->GetShadowIcon(), sz);
			pImage->Draw(pDC, CPoint(pt.x - XTP_DPI_X(1), pt.y - XTP_DPI_Y(1)), xtpImageHot, sz);
		}
		else
		{
			pImage->Draw(pDC, pt, xtpImageHot, sz);
		}
	}
	else if ((bSelected && bPressed) || (IsKeyboardSelected(bPressed)))
	{
		pImage->Draw(pDC, pt, xtpImagePressed, sz);
	}
}

CSize CXTPCommandBarsOfficeXPTheme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit,
													BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);
	}

	BOOL bSelected = pControlEdit->GetSelected(), bEnabled = pControlEdit->GetEnabled();

	CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);

	CRect rc = pControlEdit->GetRect();
	rc.left += pControlEdit->GetLabelWidth();

	COLORREF clrBackground = GetControlEditBackColor(pControlEdit);

	if (!bEnabled)
	{
		rc.DeflateRect(XTP_DPI_X(1), 0, 0, 0);

		pDC->FillSolidRect(rc, clrBackground);

		COLORREF clrBorder = GetXtremeColor(
			UINT(IsKeyboardSelected(bSelected) ? XPCOLOR_HIGHLIGHT_BORDER : XPCOLOR_DISABLED));
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
	}
	else
	{
		pDC->FillSolidRect(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1),
						   rc.Width() - XTP_DPI_X(pControlEdit->IsSpinButtonsVisible() ? 18 : 2),
						   rc.Height() - XTP_DPI_Y(2), clrBackground);

		if (bSelected)
		{
			Draw3dRect(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_BORDER);
		}
		else // if (bPopupBar)
		{
			Draw3dRect(pDC, rc, XPCOLOR_EDITCTRLBORDER, XPCOLOR_EDITCTRLBORDER);
		}
	}

	CRect rcText(pControlEdit->GetRect());
	rcText.DeflateRect(pControlEdit->GetLabelWidth() + XTP_DPI_X(3), XTP_DPI_Y(1), XTP_DPI_X(3),
					   XTP_DPI_Y(1));
	pControlEdit->DrawEditText(pDC, rcText);

	if (pControlEdit->IsSpinButtonsVisible())
	{
		BOOL bPressed = pControlEdit->GetPressed();
		CRect rcSpin(pControlEdit->GetSpinButtonsRect());
		CRect rcTop(rcSpin.left, rcSpin.top, rcSpin.right, rcSpin.CenterPoint().y);
		CRect rcBottom(rcSpin.left, rcTop.bottom, rcSpin.right, rcSpin.bottom);

		if (!bEnabled)
		{
		}
		else if (!bSelected)
		{
			pDC->Draw3dRect(rcSpin, clrBackground, clrBackground);
		}
		else
		{
			pDC->FillSolidRect(rcSpin, GetXtremeColor(XPCOLOR_HIGHLIGHT));

			if (bPressed == XTP_EDITSPIN_UP)
				pDC->FillSolidRect(rcTop, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
			else if (bPressed == XTP_EDITSPIN_DOWN)
				pDC->FillSolidRect(rcBottom, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));

			pDC->FillSolidRect(rcSpin.left, rcSpin.top, XTP_DPI_X(1), rcSpin.Height(),
							   GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
			pDC->FillSolidRect(rcSpin.left, rcBottom.top, rcSpin.Width(), XTP_DPI_Y(1),
							   GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
		}

		// Draw spin button triangles.
		COLORREF crArrow = bEnabled ? m_clrEditTextNormal : m_clrEditTextDisabled;
		CRect rcArrow	= rcTop;
		rcArrow.OffsetRect(0, 1);
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayUp2, rcArrow, crArrow);

		rcArrow = rcBottom;
		rcArrow.OffsetRect(0, -1);
		--rcArrow.bottom;
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown2, rcArrow, crArrow);
	}

	if (pControlEdit->IsBuddyButtonVisible())
	{
		BOOL bPressed = pControlEdit->GetPressed();
		CRect rcBuddyButton(pControlEdit->GetBuddyButtonRect());

		DrawRectangle(pDC, rcBuddyButton, bSelected == XTP_EDITBUDDYBUTTON,
					  bPressed == XTP_EDITBUDDYBUTTON, bEnabled, FALSE, FALSE, xtpBarTypeNormal,
					  xtpBarTop);

		CXTPImageManager* pImageManager = pControlEdit->GetParent()->GetImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(
			XTPToUInt(pControlEdit->GetBuddyButtonId()));

		if (pIcon)
		{
			CSize sz = AdjustDpiIconSize(pIcon, pIcon->GetExtent(), pControlEdit, rc.Size());
			pIcon->Draw(pDC,
						CPoint((rcBuddyButton.left + rcBuddyButton.right - sz.cx) / 2,
							   (rcBuddyButton.top + rcBuddyButton.bottom - sz.cy) / 2),
						(!bEnabled
							 ? xtpImageDisabled
							 : pControlEdit->GetSelected() == XTP_EDITBUDDYBUTTON ? xtpImageHot
																				  : xtpImageNormal),
						sz);
		}
	}

	return 0;
}

CSize CXTPCommandBarsOfficeXPTheme::DrawControlComboBox(CDC* pDC,
														CXTPControlComboBox* pControlCombo,
														BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);
	}

	BOOL bPopupBar = pControlCombo->GetParent()->GetType() == xtpBarTypePopup;

	CXTPFontDC font(pDC, GetIconFont());

	CRect rc	   = pControlCombo->GetRect();
	BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
	BOOL bSelected = pControlCombo->GetSelected();
	BOOL bEnabled  = pControlCombo->GetEnabled();

	CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);

	rc.left += pControlCombo->GetLabelWidth();

	int nThumb = pControlCombo->GetThumbWidth();
	CRect rcBtn(rc.right - nThumb, rc.top, rc.right, rc.bottom);
	CRect rcText(rc.left + XTP_DPI_X(3), rc.top + XTP_DPI_Y(1), rc.right - rcBtn.Width(),
				 rc.bottom - XTP_DPI_Y(1));

	COLORREF clrBackground = GetControlEditBackColor(pControlCombo);

	if (!bEnabled)
	{
		rc.DeflateRect(XTP_DPI_X(1), 0, 0, 0);

		pDC->FillSolidRect(rc, clrBackground);

		COLORREF clrBorder = GetXtremeColor(
			UINT(IsKeyboardSelected(bSelected) ? XPCOLOR_HIGHLIGHT_BORDER : XPCOLOR_DISABLED));
		pDC->Draw3dRect(rc, clrBorder, clrBorder);

		rcBtn.DeflateRect(1, 1);
	}
	else
	{
		CRect rcEntry(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1),
					  rc.right - nThumb + XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(1));
		pDC->FillSolidRect(rcEntry, clrBackground);

		if (bSelected)
		{
			pDC->FillSolidRect(rc, clrBackground);
			pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
							GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));

			DrawRectangle(pDC, rcBtn, TRUE, bDropped, TRUE, FALSE, FALSE, xtpBarTypeNormal,
						  pControlCombo->GetParent()->GetPosition());

			rcBtn.DeflateRect(1, 1);
		}
		else
		{
			pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_EDITCTRLBORDER),
							GetXtremeColor(XPCOLOR_EDITCTRLBORDER));
			rcBtn.DeflateRect(1, 1);

			if (bPopupBar)
			{
				Draw3dRect(pDC, rc, XPCOLOR_EDITCTRLBORDER, XPCOLOR_EDITCTRLBORDER);
				Rectangle(pDC, rcBtn, COLOR_WINDOW, XPCOLOR_EDITCTRLBORDER);
			}
			else
			{
				pDC->Draw3dRect(rcBtn, clrBackground, clrBackground);
			}
		}
	}

	pControlCombo->DrawEditText(pDC, rcText);

	DrawComboExpandMark(pDC, rcBtn,
						GetXtremeColor(UINT(!pControlCombo->GetEnabled()
												? XPCOLOR_DISABLED
												: bDropped ? XPCOLOR_PUSHED_TEXT
														   : bSelected ? XPCOLOR_HIGHLIGHT_TEXT
																	   : XPCOLOR_TOOLBAR_TEXT)));

	return 0;
}

void CXTPCommandBarsOfficeXPTheme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy,
													   BOOL bExpanded)
{
	pDC->FillSolidRect(x, y, cx, cy,
					   GetXtremeColor(
						   UINT(bExpanded ? XPCOLOR_MENUBAR_EXPANDED : XPCOLOR_TOOLBAR_FACE)));
}

void CXTPCommandBarsOfficeXPTheme::DrawSplitButtonPopup(CDC* pDC, CXTPControl* pButton)
{
	CRect rcButton = pButton->GetRect();
	BOOL bSelected = pButton->GetSelected(), bPopuped = pButton->GetPopuped(),
		 bEnabled = pButton->GetEnabled();

	if (!bPopuped || !bSelected || !bEnabled)
		VerticalLine(pDC, rcButton.right - m_nSplitButtonPopupWidth, rcButton.top + XTP_DPI_Y(1),
					 rcButton.bottom - XTP_DPI_Y(1),
					 !bEnabled ? GetControlTextColor(pButton)
							   : bSelected ? GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER)
										   : GetXtremeColor(COLOR_3DSHADOW));
	else
		Rectangle(pDC,
				  CRect(rcButton.right - m_nSplitButtonPopupWidth, rcButton.top, rcButton.right,
						rcButton.bottom),
				  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
}

void CXTPCommandBarsOfficeXPTheme::DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton)
{
	ASSERT_VALID(pButton);

	CRect rc = pButton->GetRect();
	rc.left  = rc.right
			  - (pButton->GetType() == xtpControlSplitButtonPopup ? m_nSplitButtonPopupWidth
																  : rc.Height());
	rc.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));

	BOOL bSelected = pButton->GetSelected(), bPopuped = pButton->GetPopuped(),
		 bEnabled = pButton->GetEnabled();
	COLORREF clr  = (pButton->GetType() == xtpControlSplitButtonPopup && bEnabled && bPopuped
							 && bSelected
						 ? GetXtremeColor(XPCOLOR_PUSHED_TEXT)
						 : GetControlTextColor(pButton));

	XTPPrimitiveSymbol nSymbol = CXTPDrawHelpers::IsContextRTL(pDC) ? xtpPrimitiveSymbolPlayLeft
																	: xtpPrimitiveSymbolPlayRight;
	XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rc, clr);
}

void CXTPCommandBarsOfficeXPTheme::FillPopupLabelEntry(CDC* pDC, CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_LABEL));
}

CSize CXTPCommandBarsOfficeXPTheme::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton,
														   BOOL bDraw)
{
	CXTPFontDC font(pDC, GetControlFont(pButton));

	CXTPCommandBar* pParent = pButton->GetParent();

	CRect rcButton			   = pButton->GetRect();
	XTPControlType controlType = pButton->GetType();
	XTPButtonStyle buttonStyle = pButton->GetStyle();

	BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup)
					  || (buttonStyle != xtpButtonCaption);

	BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(),
		 bChecked = pButton->GetChecked();

	if (controlType == xtpControlSplitButtonPopup && bEnabled == TRUE_SPLITDROPDOWN)
		bEnabled = FALSE;

	BOOL bLayoutRTL		= pParent->IsLayoutRTL();
	BOOL bDoubleGripper = pParent->IsPopupBar() ? ((CXTPPopupBar*)pParent)->IsDoubleGripper()
												: FALSE;

	CString strCaption = pButton->GetCaption();
	if (bLayoutRTL)
		UpdateRTLCaption(strCaption);

	CSize szIcon = GetIconSize(pButton);
	CSize szCheck(szIcon.cx + XTP_DPI_X(4), szIcon.cy + XTP_DPI_Y(4));
	CSize szImage(max(szIcon.cx + XTP_DPI_X(4), m_nTextHeight + XTP_DPI_Y(4)),
				  max(szIcon.cy + XTP_DPI_Y(6), m_nTextHeight));

	int nGripperWidth = GetPopupBarGripperWidth(pParent);

	if (bDraw)
	{
		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage
										   ? pButton->GetImage(szIcon.cx)
										   : NULL;

		if (pButton->GetExpanded() && rcButton.left < nGripperWidth)
		{
			DrawPopupBarGripper(pDC, rcButton.left - XTP_DPI_X(1), rcButton.top, nGripperWidth,
								rcButton.Height(), TRUE);
		}

		if (pButton->GetType() == xtpControlLabel)
		{
			if (!m_bOffsetPopupLabelText && !pImage)
				nGripperWidth = 0;

			if (!pParent->GetShowGripper())
				nGripperWidth = 0;
			int nLabelGripper = rcButton.left > nGripperWidth || !m_bOffsetPopupLabelText
									? 0
									: nGripperWidth;

			FillPopupLabelEntry(pDC, CRect(rcButton.left + nLabelGripper, rcButton.top,
										   rcButton.right, rcButton.bottom));
		}
		else
		{
			DrawRectangle(pDC, rcButton, bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup,
						  xtpBarPopup);
		}

		if (pButton->GetType() == xtpControlSplitButtonPopup)
		{
			DrawSplitButtonPopup(pDC, pButton);
		}

		CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
		if (!pParent->GetShowGripper())
			rcImageRect.OffsetRect(m_nPopupBarTextPadding / 2, 0);

		CRect rcCheck(CPoint(rcImageRect.left + rcImageRect.Width() / 2 - szCheck.cx / 2,
							 rcImageRect.top + rcImageRect.Height() / 2 - szCheck.cy / 2),
					  szCheck);

		if (bDoubleGripper)
			rcImageRect.OffsetRect(szImage.cx, 0);

		if (bChecked)
		{
			DrawRectangle(pDC, rcCheck, bSelected, FALSE, bEnabled, bChecked, FALSE,
						  xtpBarTypePopup, xtpBarPopup, pButton->GetID());
		}

		if (pImage)
		{
			szIcon		   = AdjustDpiIconSize(pImage, szIcon, pButton, rcImageRect.Size());
			CPoint ptImage = CPoint(rcImageRect.left + rcImageRect.Width() / 2 - szIcon.cx / 2,
									rcImageRect.top + rcImageRect.Height() / 2 - szIcon.cy / 2);

			BOOL bRTL = XTPDrawHelpers()->IsContextRTL(pDC);
			if (bRTL && m_bEnableAnimation)
			{
				if (pImage->IsVectorIcon())
				{
					XTPDrawHelpers()->SetContextRTL(pDC, !bRTL);

					CXTPClientRect rcParent(pButton->GetParent());
					ptImage.x = rcParent.Width() - (ptImage.x + szIcon.cx);
				}
			}

			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, bEnabled, bChecked, FALSE,
					  FALSE);

			XTPDrawHelpers()->SetContextRTL(pDC, XTPToDWORD(bRTL));
		}

		if (bChecked && (bDoubleGripper || !pImage))
		{
			rcCheck.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));
			DrawCheckMark(pDC, rcCheck, (controlType == xtpControlRadioButton), bEnabled,
						  bSelected);
		}

		COLORREF clrText = GetControlTextColor(pButton);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);

		if (IsPopupControlType(controlType))
		{
			DrawControlPopupGlyph(pDC, pButton);
		}
	}

	return CXTPPaintManager::DrawControlPopupParent(pDC, pButton, bDraw);
}

void CXTPCommandBarsOfficeXPTheme::DrawCheckMark(CDC* pDC, CRect rcCheck, BOOL bIsRadio,
												 BOOL bEnabled, BOOL /*bSelected*/)
{
	COLORREF clr = !bEnabled ? GetXtremeColor(XPCOLOR_DISABLED) : 0;

	if (bEnabled
		&& !XTPColorManager()->LongColor(0, GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED), 1, 50))
	{
		clr = RGB(255, 255, 255);
	}

	if (bIsRadio)
		DrawPopupBarRadioMark(pDC, rcCheck, bEnabled, clr);
	else
		DrawPopupBarCheckMark(pDC, rcCheck, bEnabled, clr);
}

COLORREF CXTPCommandBarsOfficeXPTheme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed,
															 BOOL bEnabled, BOOL bChecked,
															 BOOL bPopuped, XTPBarType barType,
															 XTPBarPosition barPosition)
{
	UINT nColorFace = UINT(barPosition == xtpBarPopup ? XPCOLOR_MENUBAR_TEXT
													  : XPCOLOR_TOOLBAR_TEXT);

	if (barType == xtpBarTypePopup)
	{
		return GetXtremeColor(
			UINT(!bEnabled ? XPCOLOR_MENUBAR_GRAYTEXT
						   : bSelected ? XPCOLOR_HIGHLIGHT_TEXT : XPCOLOR_MENUBAR_TEXT));
	}

	COLORREF clrText = GetXtremeColor(UINT(
		!bEnabled
			? XPCOLOR_TOOLBAR_GRAYTEXT
			: bPopuped ? XPCOLOR_TOOLBAR_TEXT
					   : bChecked && !bSelected && !bPressed
							 ? XPCOLOR_CHECKED_TEXT
							 : IsKeyboardSelected(bPressed) || (bSelected && (bPressed || bChecked))
								   ? XPCOLOR_PUSHED_TEXT
								   : bSelected || bPressed ? XPCOLOR_HIGHLIGHT_TEXT : nColorFace));

	return clrText;
}

void CXTPCommandBarsOfficeXPTheme::FillIntersectRect(CDC* pDC, CXTPPopupBar* pPopupBar,
													 COLORREF clr)
{
	if (pPopupBar && pPopupBar->GetControlPopup())
	{
		CXTPCommandBar* pParentBar = pPopupBar->GetControlPopup()->GetParent();
		if (!pParentBar || pParentBar->GetType() == xtpBarTypePopup)
			return;

		CRect rcParent;
		pParentBar->GetWindowRect(rcParent);
		CRect rcExclude = pPopupBar->GetControlPopup()->GetRect();
		pParentBar->ClientToScreen(&rcExclude);

		CRect rcWindow;
		pPopupBar->GetWindowRect(&rcWindow);

		CRect rcIntersect;

		if (rcIntersect.IntersectRect(rcWindow, rcExclude))
		{
			pPopupBar->ScreenToClient(rcIntersect);

			if (rcIntersect.Width() > XTP_DPI_X(1))
				rcIntersect.DeflateRect(XTP_DPI_X(1), 0);
			if (rcIntersect.Height() > XTP_DPI_Y(1))
				rcIntersect.DeflateRect(0, XTP_DPI_Y(1));

			pDC->FillSolidRect(rcIntersect, clr);
		}
	}
}

void CXTPCommandBarsOfficeXPTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		COLORREF clrPen   = GetXtremeColor(XPCOLOR_FLOATBAR_BORDER);
		COLORREF clrBrush = GetXtremeColor(
			UINT(pBar->GetType() == xtpBarTypePopup
					 ? XPCOLOR_MENUBAR_FACE
					 : IsFlatToolBar(pBar) ? COLOR_3DFACE : XPCOLOR_TOOLBAR_FACE));

		pDC->Draw3dRect(rc, clrPen, clrPen);
		rc.DeflateRect(1, 1);

		pDC->FillSolidRect(rc, clrBrush);
		pDC->Draw3dRect(rc, clrPen, clrPen);

		pDC->SetPixel(XTP_DPI_X(2), XTP_DPI_Y(2), clrPen);
		pDC->SetPixel(rc.right - XTP_DPI_X(2), XTP_DPI_Y(2), clrPen);
		pDC->SetPixel(XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clrPen);
		pDC->SetPixel(rc.right - XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clrPen);
	}
	else if (pBar->GetPosition() == xtpBarPopup)
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);

		FillIntersectRect(pDC, (CXTPPopupBar*)pBar, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	}
	else if (IsFlatToolBar(pBar))
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);

		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
		pDC->Draw3dRect(rc, clr3DFace, clr3DFace);

		pDC->SetPixel(XTP_DPI_X(1), XTP_DPI_Y(1), clr3DFace);
		pDC->SetPixel(rc.right - XTP_DPI_X(2), XTP_DPI_Y(1), clr3DFace);
		pDC->SetPixel(XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(2), clr3DFace);
		pDC->SetPixel(rc.right - XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clr3DFace);
	}
}

CSize CXTPCommandBarsOfficeXPTheme::DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* /*pBar*/,
														 BOOL bDraw)
{
	CSize sz(XTP_DPI_X(6), max(XTP_DPI_Y(22), m_nTextHeight + XTP_DPI_Y(4)));

	if (pDC && bDraw)
	{
		CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		for (int y = XTP_DPI_Y(6); y < sz.cy - XTP_DPI_Y(2); y += XTP_DPI_Y(2))
			Line(pDC, CPoint(XTP_DPI_X(3), y), CPoint(XTP_DPI_X(6), y));
	}

	return sz;
}

CSize CXTPCommandBarsOfficeXPTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
														  BOOL bDraw)
{
	if (pBar->IsDialogBar())
		return DrawDialogBarGripper(pDC, (CXTPDialogBar*)pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarPopup && pBar->GetType() == xtpBarTypePopup)
	{
		CRect rcBorders = pBar->GetBorders();
		DrawPopupBarGripper(pDC, rcBorders.left - XTP_DPI_X(1), rcBorders.top,
							GetPopupBarGripperWidth(pBar),
							rc.bottom - rcBorders.top - rcBorders.bottom);
	}
	if (pBar->GetPosition() == xtpBarFloating)
	{
		CSize sz = CXTPPaintManager::DrawCommandBarGripper(pDC, pBar, bDraw);

		if (pBar->GetType() == xtpBarTypePopup && pBar->GetShowGripper())
		{
			CRect rcBorders = pBar->GetBorders();
			int nTop		= rcBorders.top + sz.cy + XTP_DPI_Y(1);
			DrawPopupBarGripper(pDC, rcBorders.left - XTP_DPI_X(1), nTop,
								GetPopupBarGripperWidth(pBar), rc.bottom - nTop - rcBorders.bottom);
		}

		return sz;
	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
			for (int x = XTP_DPI_X(5); x < rc.Width() - XTP_DPI_X(5); x += XTP_DPI_X(2))
				Line(pDC, CPoint(x, XTP_DPI_Y(3)), CPoint(x, XTP_DPI_Y(6)));
		}
		return CSize(0, XTP_DPI_Y(8));
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
			for (int y = XTP_DPI_Y(5); y < rc.Height() - XTP_DPI_Y(5); y += XTP_DPI_Y(2))
				Line(pDC, CPoint(XTP_DPI_X(3), y), CPoint(XTP_DPI_X(6), y));
		}
		return CSize(XTP_DPI_X(8), 0);
	}
	return 0;
}

CSize CXTPCommandBarsOfficeXPTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
															CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		if (pBar->GetType() == xtpBarTypePopup)
			return XTP_DPI(CSize(3, 3));

		return XTP_DPI(CSize(6, 6));
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRow		= pControl->GetRowRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		if (!pControl->GetWrap())
			HorizontalLine(pDC,
						   rcControl.left - XTP_DPI_X(1) + GetPopupBarGripperWidth(pBar)
							   + m_nPopupBarTextPadding,
						   rcControl.top - XTP_DPI_Y(2), rcControl.right + XTP_DPI_X(1),
						   GetXtremeColor(XPCOLOR_SEPARATOR));
		else
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(2), rcRow.top - XTP_DPI_Y(1),
						 rcRow.bottom + XTP_DPI_Y(1), GetXtremeColor(XPCOLOR_SEPARATOR));
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
			HorizontalLine(pDC, rcRow.left, rcControl.top - XTP_DPI_Y(4), rcRow.right,
						   GetXtremeColor(XPCOLOR_SEPARATOR));
		else
			VerticalLine(pDC, rcRow.right + XTP_DPI_X(2), rcRow.top + XTP_DPI_Y(4), rcRow.bottom,
						 GetXtremeColor(XPCOLOR_SEPARATOR));
	}
	else
	{
		if (!pControl->GetWrap())
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(4), rcRow.top, rcRow.bottom,
						 GetXtremeColor(XPCOLOR_SEPARATOR));
		else
			HorizontalLine(pDC, rcRow.left, rcRow.top - XTP_DPI_Y(4), rcRow.right,
						   GetXtremeColor(XPCOLOR_SEPARATOR));
	}
	return 0;
}

void CXTPCommandBarsOfficeXPTheme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl,
													 BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);
	if (!pParent)
		return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (pControl->GetType() == xtpControlComboBox)
			return;

		if (pParent->GetFlags() & xtpFlagSmartLayout && pParent->GetPosition() == xtpBarPopup)
		{
			rc.DeflateRect(0, XTP_DPI_Y(1));
		}
		else if (bVertical)
			rc.DeflateRect(XTP_DPI_X(1), 0);
		else
			rc.DeflateRect(0, XTP_DPI_Y(1));
	}
	else
		rc.InflateRect(XTP_DPI_X(1), 0);
}

CSize CXTPCommandBarsOfficeXPTheme::DrawListBoxControl(CDC* pDC, CXTPControl* pButton,
													   CRect rcButton, BOOL bSelected, BOOL bDraw,
													   CXTPCommandBars* pCommandBars)
{
	CSize szIcon = pCommandBars && pCommandBars->GetCommandBarsOptions()->szPopupIcons != CSize(0)
					   ? pCommandBars->GetCommandBarsOptions()->szPopupIcons
					   : GetAutoIconSize(FALSE);
	CSize szImage(max(szIcon.cx + XTP_DPI_X(4), m_nTextHeight + XTP_DPI_X(4)),
				  max(szIcon.cy + XTP_DPI_Y(6), m_nTextHeight));

	if (bDraw)
	{
		pDC->FillSolidRect(rcButton, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		DrawPopupBarGripper(pDC, 0, rcButton.top, szImage.cx + XTP_DPI_X(1), rcButton.Height());

		if (pButton == NULL)
		{
			return 0;
		}

		XTPControlType controlType = pButton->GetType();
		XTPButtonStyle buttonStyle = pButton->GetStyle();

		if (bSelected)
			DrawRectangle(pDC, rcButton, bSelected, FALSE, TRUE, FALSE, FALSE, xtpBarTypePopup,
						  xtpBarPopup);

		BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup)
						  || (buttonStyle == xtpButtonIcon)
						  || (buttonStyle == xtpButtonIconAndCaption);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage
										   ? pButton->GetImage(szIcon.cx)
										   : NULL;
		if (pImage)
		{
			szIcon		   = AdjustDpiIconSize(pImage, szIcon, pButton, szImage);
			CPoint ptImage = CPoint(rcButton.left + szImage.cx / 2 - szIcon.cx / 2,
									rcButton.top + rcButton.Height() / 2 - szIcon.cy / 2);

			BOOL bRTL = XTPDrawHelpers()->IsContextRTL(pDC);
			if (bRTL)
			{
				if (pImage->IsVectorIcon())
				{
					XTPDrawHelpers()->SetContextRTL(pDC, !bRTL);
					ptImage.x = rcButton.right - (ptImage.x + szIcon.cx);
				}
			}

			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);

			XTPDrawHelpers()->SetContextRTL(pDC, XTPToDWORD(bRTL));
		}

		COLORREF clrText = GetRectangleTextColor(bSelected, FALSE, TRUE, FALSE, FALSE,
												 xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);

		CString strText = pButton->GetCaption();
		StripMnemonics(strText);

		if (pCommandBars && pCommandBars->IsLayoutRTL())
			UpdateRTLCaption(strText);

		CRect rcLabelText(rcButton.left + szImage.cx + XTP_DPI_X(8), rcButton.top, rcButton.right,
						  rcButton.bottom);

		if (controlType == xtpControlPopup || controlType == xtpControlButtonPopup
			|| controlType == xtpControlSplitButtonPopup)
		{
			CRect rc		  = rcButton;
			rc.left			  = rc.right - rc.Height();
			rcLabelText.right = rc.left;
			rc.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayRight, rc, clrText);
		}
		if (controlType == xtpControlSplitButtonPopup)
		{
			Line(pDC, rcButton.right - XTP_DPI_X(20), rcButton.top + XTP_DPI_Y(1),
				 rcButton.right - XTP_DPI_X(20), rcButton.bottom - XTP_DPI_Y(1),
				 bSelected ? XPCOLOR_HIGHLIGHT_BORDER : COLOR_3DSHADOW);
			rcLabelText.right = rcButton.right - XTP_DPI_X(20);
		}
		if (controlType == xtpControlComboBox || controlType == xtpControlEdit)
		{
			CRect rcText(0, 0, 0, 0);
			pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_CALCRECT);
			int nThumb = pDC->GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(2);

			CRect rcCombo(rcButton.left + szImage.cx + XTP_DPI_X(8) + rcText.Width() + XTP_DPI_X(5),
						  rcButton.top, rcButton.right, rcButton.bottom);
			CRect rcThumb(rcButton.right - nThumb, rcButton.top, rcButton.right, rcButton.bottom);

			Rectangle(pDC, rcCombo, bSelected ? XPCOLOR_HIGHLIGHT_BORDER : COLOR_3DFACE,
					  COLOR_WINDOW);
			if (controlType == xtpControlComboBox)
			{
				if (!bSelected)
					pDC->FillSolidRect(rcThumb.left + XTP_DPI_X(2), rcThumb.top + XTP_DPI_Y(2),
									   rcThumb.Width() - XTP_DPI_X(4),
									   rcThumb.Height() - XTP_DPI_Y(4),
									   GetXtremeColor(COLOR_3DFACE));
				else
					DrawRectangle(pDC, rcThumb, bSelected, FALSE, TRUE, FALSE, FALSE,
								  xtpBarTypePopup, xtpBarPopup);

				rcThumb.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
				DrawComboExpandMark(pDC, rcThumb,
									GetXtremeColor(UINT(bSelected ? XPCOLOR_HIGHLIGHT_TEXT
																  : XPCOLOR_TOOLBAR_TEXT)));
			}
		}

		CXTPFontDC fnt(pDC, &m_xtpFontRegular);
		pDC->DrawText(strText, &rcLabelText,
					  DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
	}
	return szImage;
}

CSize CXTPCommandBarsOfficeXPTheme::DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected,
													   BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc(XTP_DPI_X(1), XTP_DPI_Y(1), rcGripper.right + XTP_DPI_X(1),
				 rcGripper.Height() + XTP_DPI_Y(1));
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		rc.DeflateRect(1, 1);

		int nLeft  = rc.CenterPoint().x - min(XTP_DPI_X(16), rcGripper.Width() / 2 - XTP_DPI_X(10));
		int nRight = rc.CenterPoint().x + min(XTP_DPI_X(16), rcGripper.Width() / 2 - XTP_DPI_X(10));

		if (bSelected)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
			if (nLeft < nRight)
			{
				Line(pDC, nLeft, XTP_DPI_Y(4), nRight, XTP_DPI_Y(4), XPCOLOR_HIGHLIGHT_TEXT);
				Line(pDC, nLeft, XTP_DPI_Y(6), nRight, XTP_DPI_Y(6), XPCOLOR_HIGHLIGHT_TEXT);
			}
		}
		else
		{
			pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

			if (nLeft < nRight)
			{
				Line(pDC, nLeft, XTP_DPI_Y(3), nRight, XTP_DPI_Y(3), XPCOLOR_TOOLBAR_GRIPPER);
				Line(pDC, nLeft, XTP_DPI_Y(5), nRight, XTP_DPI_Y(5), XPCOLOR_TOOLBAR_GRIPPER);
				Line(pDC, nLeft, XTP_DPI_Y(7), nRight, XTP_DPI_Y(7), XPCOLOR_TOOLBAR_GRIPPER);
			}
		}
	}

	return CSize(0, XTP_DPI_Y(9));
}

void CXTPCommandBarsOfficeXPTheme::DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags)
{
	pDC->FillSolidRect(rcGripper, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

	int c = rcGripper.CenterPoint().x;

	if (nFlags == 2)
	{
		for (int x = c - XTP_DPI_X(8); x < c + XTP_DPI_X(8); x += XTP_DPI_X(4))
		{
			pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), XTP_DPI_Y(4) + rcGripper.top,
									 x + XTP_DPI_X(3), XTP_DPI_Y(6) + rcGripper.top),
							   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(CRect(x, XTP_DPI_Y(3) + rcGripper.top, x + XTP_DPI_X(2),
									 XTP_DPI_Y(5) + rcGripper.top),
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	else
	{
		int x = rcGripper.right - XTP_DPI_X(4);
		int y = rcGripper.bottom - XTP_DPI_Y(4);

		pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), XTP_DPI_Y(1) + y, x + XTP_DPI_X(3),
								 XTP_DPI_Y(3) + y),
						   GetXtremeColor(COLOR_WINDOW));
		pDC->FillSolidRect(CRect(x, 0 + y, x + XTP_DPI_X(2), XTP_DPI_Y(2) + y),
						   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

		x -= XTP_DPI_X(4);
		pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), XTP_DPI_Y(1) + y, x + XTP_DPI_X(3),
								 XTP_DPI_Y(3) + y),
						   GetXtremeColor(COLOR_WINDOW));
		pDC->FillSolidRect(CRect(x, 0 + y, x + XTP_DPI_X(2), XTP_DPI_Y(2) + y),
						   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

		x += XTP_DPI_X(4);
		y -= XTP_DPI_Y(4);
		pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), XTP_DPI_Y(1) + y, x + XTP_DPI_X(3),
								 XTP_DPI_Y(3) + y),
						   GetXtremeColor(COLOR_WINDOW));
		pDC->FillSolidRect(CRect(x, 0 + y, x + XTP_DPI_X(2), XTP_DPI_Y(2) + y),
						   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
	}
}

CSize CXTPCommandBarsOfficeXPTheme::DrawControlCheckBoxMark(CDC* pDC, CXTPControl* pButton,
															CRect rc, BOOL bDraw)
{
	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked();
	CRect rcButton(rc);
	if (!bDraw)
		rcButton = CXTPEmptyRect();
	return DrawControlCheckBoxMark(pDC, rcButton, bDraw, bSelected, bPressed, bChecked, bEnabled);
}

CSize CXTPCommandBarsOfficeXPTheme::DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw,
															BOOL bSelected, BOOL bPressed,
															BOOL bChecked, BOOL bEnabled)
{
	if (m_bThemedCheckBox && m_themeButton->IsAppThemed())
		return CXTPPaintManager::DrawControlCheckBoxMark(pDC, rc, bDraw, bSelected, bPressed,
														 bChecked, bEnabled);

	if (bDraw)
	{
		if (!bEnabled)
		{
			Draw3dRect(pDC, rc, XPCOLOR_GRAYTEXT, XPCOLOR_GRAYTEXT);
		}
		else
		{
			Rectangle(pDC, rc, bSelected || bPressed ? XPCOLOR_HIGHLIGHT_BORDER : XPCOLOR_3DSHADOW,
					  bSelected && bPressed
						  ? XPCOLOR_HIGHLIGHT_PUSHED
						  : bPressed || bSelected ? XPCOLOR_HIGHLIGHT : COLOR_WINDOW);
		}

		if (bChecked)
		{
			CPoint pt = rc.CenterPoint();
			CXTPPenDC pen(pDC->GetSafeHdc(),
						  GetXtremeColor(
							  UINT(!bEnabled || bChecked == 2 ? XPCOLOR_GRAYTEXT : COLOR_BTNTEXT)));

			int n1x = XTP_DPI_X(1);
			int n3x = XTP_DPI_X(3);
			int n4x = XTP_DPI_X(4);
			int n1y = XTP_DPI_Y(1);
			int n4y = XTP_DPI_Y(4);

			if (CXTPDrawHelpers::IsContextRTL(pDC)) // Revert Check Box
			{
				for (int i = 0; i < 3; i++)
				{
					pDC->MoveTo(pt.x + n3x, pt.y - n1y + i);
					pDC->LineTo(pt.x + n1x, pt.y + n1y + i);
					pDC->LineTo(pt.x - n4x, pt.y - n4y + i);
				}
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					pDC->MoveTo(pt.x - n3x, pt.y - n1y + i);
					pDC->LineTo(pt.x - n1x, pt.y + n1y + i);
					pDC->LineTo(pt.x + n4x, pt.y - n4y + i);
				}
			}
		}
	}

	return XTP_DPI(CSize(13, 13));
}

CSize CXTPCommandBarsOfficeXPTheme::DrawControlRadioButtonMark(CDC* pDC, CRect rc, BOOL bDraw,
															   BOOL bSelected, BOOL bPressed,
															   BOOL bChecked, BOOL bEnabled)
{
	if (m_bThemedCheckBox && m_themeButton->IsAppThemed())
		return CXTPPaintManager::DrawControlRadioButtonMark(pDC, rc, bDraw, bSelected, bPressed,
															bChecked, bEnabled);

	if (bDraw)
	{
		if (!bEnabled)
		{
			AlphaEllipse(pDC, rc, GetXtremeColor(XPCOLOR_GRAYTEXT), GetXtremeColor(XPCOLOR_3DFACE));
		}
		else
		{
			AlphaEllipse(pDC, rc,
						 GetXtremeColor(UINT(bSelected || bPressed ? XPCOLOR_HIGHLIGHT_BORDER
																   : XPCOLOR_3DSHADOW)),
						 GetXtremeColor(
							 UINT(bSelected && bPressed
									  ? XPCOLOR_HIGHLIGHT_PUSHED
									  : bPressed || bSelected ? XPCOLOR_HIGHLIGHT : COLOR_WINDOW)));
		}

		if (bChecked)
		{
			COLORREF clrFace = GetXtremeColor(
				UINT(!bEnabled || bChecked == 2 ? XPCOLOR_GRAYTEXT : COLOR_BTNTEXT));
			AlphaEllipse(pDC,
						 CRect(rc.left + XTP_DPI_X(4), rc.top + XTP_DPI_Y(4),
							   rc.left + XTP_DPI_X(9), rc.top + XTP_DPI_Y(9)),
						 clrFace, clrFace);
		}
	}

	return XTP_DPI(CSize(13, 13));
}

void CXTPCommandBarsOfficeXPTheme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton,
														CRect rcButton)
{
	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		 bPopuped = pButton->GetPopuped();

	if (pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		int nSplitDropDownHeight = GetSplitDropDownHeight();

		BOOL bCommandPressed = ((bSelected && bPressed) && !bPopuped && bEnabled);
		BOOL bVert			 = GetControlTextOrientation(pButton) == xtpBarTextVertical;

		DrawRectangle(pDC, CRect(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom),
					  bSelected, bPressed && !bCommandPressed, bEnabled, bChecked, bPopuped,
					  pButton->GetParent()->GetType(), pButton->GetParent()->GetPosition());

		if ((bSelected || bPressed) && !bPopuped && bEnabled)
		{
			if (bVert)
				pDC->FillSolidRect(rcButton.left + nSplitDropDownHeight, rcButton.top, XTP_DPI_X(1),
								   rcButton.Height(), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
			else
				pDC->FillSolidRect(rcButton.left,
								   rcButton.bottom - nSplitDropDownHeight - XTP_DPI_Y(1),
								   rcButton.Width(), XTP_DPI_Y(1),
								   GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
		}

		if (bCommandPressed)
		{
			if (bVert)
				DrawRectangle(pDC,
							  CRect(rcButton.left + nSplitDropDownHeight, rcButton.top,
									rcButton.right, rcButton.bottom),
							  bSelected, bPressed, bEnabled, bChecked, bPopuped,
							  pButton->GetParent()->GetType(), pButton->GetParent()->GetPosition());
			else
				DrawRectangle(pDC,
							  CRect(rcButton.left, rcButton.top, rcButton.right,
									rcButton.bottom - nSplitDropDownHeight),
							  bSelected, bPressed, bEnabled, bChecked, bPopuped,
							  pButton->GetParent()->GetType(), pButton->GetParent()->GetPosition());
		}
		return;
	}

	DrawRectangle(pDC,
				  CRect(rcButton.left, rcButton.top,
						rcButton.right - (bPopuped ? 0 : m_nSplitButtonDropDownWidth),
						rcButton.bottom),
				  bSelected, bPressed, bEnabled, bChecked, bPopuped,
				  pButton->GetParent()->GetType(), pButton->GetParent()->GetPosition());

	if ((bSelected || bPressed) && !bPopuped && bEnabled)
		DrawRectangle(pDC,
					  CRect(rcButton.right - m_nSplitButtonDropDownWidth - XTP_DPI_X(1),
							rcButton.top, rcButton.right, rcButton.bottom),
					  TRUE, FALSE, TRUE, FALSE, FALSE, pButton->GetParent()->GetType(),
					  pButton->GetParent()->GetPosition());

	CPoint pt = CPoint(rcButton.right - m_nSplitButtonDropDownWidth / 2, rcButton.CenterPoint().y);
	if (bEnabled == TRUE_SPLITCOMMAND)
		bEnabled = FALSE;

	DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
}
