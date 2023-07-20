// XTPCommandBarsOffice2000Theme.cpp : implementation of the CXTPCommandBarsOffice2000Theme class.
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
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2000Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CXTPCommandBarsOffice2000Theme

IMPLEMENT_DYNAMIC(CXTPCommandBarsOffice2000Theme, CXTPPaintManager)

CXTPCommandBarsOffice2000Theme::CXTPCommandBarsOffice2000Theme()
{
	m_iconsInfo.bUseFadedIcons   = FALSE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	m_nPopupBarText = COLOR_BTNTEXT;
}

void CXTPCommandBarsOffice2000Theme::RefreshMetrics()
{
	CXTPPaintManager::RefreshMetrics();

	m_clrFloatingGripper.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrFloatingGripperText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
}

void CXTPCommandBarsOffice2000Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
												   BOOL bPressed, BOOL bEnabled, BOOL bChecked,
												   BOOL bPopuped, XTPBarType barType,
												   XTPBarPosition barPosition)
{
	if (m_bSelectImageInPopupBar && barPosition == xtpBarPopup && barType == xtpBarTypePopup
		&& bChecked)
	{
		DrawShadedRect(pDC, rc);
		Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		return;
	}

	if (barType == xtpBarTypePopup && !bChecked)
	{
		if (bSelected || bPressed)
		{
			Rectangle(pDC, rc, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);
		}
		return;
	}

	if (!bEnabled)
	{
		if (bChecked)
			Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		if (IsKeyboardSelected(bSelected))
			Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bChecked && !bSelected && !bPressed)
	{
		DrawShadedRect(pDC, rc);
		Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (bChecked || bPopuped || IsKeyboardSelected(bPressed) || (bSelected && bPressed))
		Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	else if (bSelected || bPressed)
		Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
}

BOOL CXTPCommandBarsOffice2000Theme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	return CXTPPaintManager::DrawRectangle(pDC, pButton);
}

void CXTPCommandBarsOffice2000Theme::DrawImage(CDC* pDC, CPoint pt, CSize sz,
											   CXTPImageManagerIcon* pImage, BOOL bSelected,
											   BOOL bPressed, BOOL bEnabled, BOOL bChecked,
											   BOOL bPopuped, BOOL)
{
	if (!bEnabled)
	{
		if (!m_iconsInfo.bUseDisabledIcons && !pImage->IsAlpha() && pImage->IsRasterIcon())
		{
			pImage->DrawMono(pDC, pt + XTP_DPI(CPoint(1, 1)), pImage->GetIcon(), sz,
							 GetXtremeColor(COLOR_3DHILIGHT));
			pImage->DrawMono(pDC, pt, pImage->GetIcon(), sz, GetXtremeColor(COLOR_3DSHADOW));
		}
		else
		{
			if (m_iconsInfo.bOfficeStyleDisabledIcons && !pImage->HasDisabledIcon()
				&& pImage->IsRasterIcon())
			{
				pImage->CreateDisabledIcon(m_clrDisabledIcon.clrLight, m_clrDisabledIcon.clrDark);
			}

			pImage->Draw(pDC, pt, xtpImageDisabled, sz);
		}
	}
	else if (bChecked == 2 && !bPressed)
	{
		if (!pImage->IsAlpha() && pImage->IsRasterIcon())
		{
			pImage->DrawMono(pDC, pt, pImage->GetCheckedIcon(), sz, GetXtremeColor(COLOR_3DSHADOW));
		}
		else
		{
			if (m_iconsInfo.bOfficeStyleDisabledIcons && !pImage->HasDisabledIcon()
				&& pImage->IsRasterIcon())
			{
				pImage->CreateDisabledIcon(m_clrDisabledIcon.clrLight, m_clrDisabledIcon.clrDark);
			}

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
		if (m_iconsInfo.bIconsWithShadow && pImage->IsRasterIcon()
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
		pImage->Draw(pDC, CPoint(pt.x + XTP_DPI_X(1), pt.y + XTP_DPI_Y(1)), xtpImagePressed, sz);
	}
}

void CXTPCommandBarsOffice2000Theme::DrawControlEditSpin(CDC* pDC, CXTPControlEdit* pControlEdit)
{
	BOOL bPressed  = pControlEdit->GetPressed();
	BOOL bEnabled  = pControlEdit->GetEnabled();
	BOOL bSelected = pControlEdit->GetSelected();

	CRect rcSpin(pControlEdit->GetSpinButtonsRect());
	CRect rcTop(rcSpin.left + XTP_DPI_X(1), rcSpin.top + XTP_DPI_Y(1), rcSpin.right - XTP_DPI_X(1),
				rcSpin.CenterPoint().y);
	CRect rcBottom(rcSpin.left + XTP_DPI_X(1), rcTop.bottom, rcSpin.right - XTP_DPI_X(1),
				   rcSpin.bottom - XTP_DPI_Y(1));

	if (!bEnabled)
	{
	}
	else if (!bSelected)
	{
		rcSpin.DeflateRect(1, 1);
		Rectangle(pDC, rcSpin, COLOR_WINDOW, COLOR_3DFACE);
	}
	else
	{
		pDC->FillSolidRect(rcSpin, GetXtremeColor(COLOR_3DFACE));

		if (bPressed == XTP_EDITSPIN_UP)
			pDC->Draw3dRect(rcTop, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW));
		else
			pDC->Draw3dRect(rcTop, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));

		if (bPressed == XTP_EDITSPIN_DOWN)
			pDC->Draw3dRect(rcBottom, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW));
		else
			pDC->Draw3dRect(rcBottom, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
	}

	COLORREF clr = GetXtremeColor(UINT(bEnabled ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
	++rcTop.bottom;
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayUp2, rcTop, clr);
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown2, rcBottom, clr);
}

CSize CXTPCommandBarsOffice2000Theme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit,
													  BOOL bDraw)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pControlEdit);

	if (!bDraw)
		return CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);

	BOOL bEnabled  = pControlEdit->GetEnabled();
	BOOL bSelected = pControlEdit->GetSelected();

	CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);

	CRect rc = pControlEdit->GetRect();
	rc.left += pControlEdit->GetLabelWidth();

	COLORREF clrBackground = GetControlEditBackColor(pControlEdit);

	if (bEnabled)
	{
		CRect rcEntry(rc);
		rcEntry.DeflateRect(1, 1);
		pDC->FillSolidRect(rcEntry, clrBackground);
	}

	DrawControlEditFrame(pDC, rc, bEnabled, bSelected);

	CRect rcText(pControlEdit->GetRect());
	rcText.DeflateRect(pControlEdit->GetLabelWidth() + XTP_DPI_X(3), XTP_DPI_Y(1), XTP_DPI_X(3),
					   XTP_DPI_Y(1));
	pControlEdit->DrawEditText(pDC, rcText);

	if (pControlEdit->IsSpinButtonsVisible())
		DrawControlEditSpin(pDC, pControlEdit);

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
			CSize sz = CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), pControlEdit,
														   rc.Size());
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

void CXTPCommandBarsOffice2000Theme::DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled,
														  BOOL bSelected)
{
	if (!bEnabled)
	{
		rc.DeflateRect(1, 1);
		Draw3dRect(pDC, rc, IsKeyboardSelected(bSelected) ? COLOR_3DSHADOW : COLOR_3DHIGHLIGHT,
				   IsKeyboardSelected(bSelected) ? COLOR_3DSHADOW : COLOR_3DHIGHLIGHT);
	}
	else
	{
		if (bSelected)
		{
			Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT);
		}

		rc.DeflateRect(1, 1);
		Draw3dRect(pDC, rc, COLOR_3DFACE, COLOR_3DFACE);
	}
}
void CXTPCommandBarsOffice2000Theme::DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled,
															   BOOL bSelected, BOOL bDropped)
{
	rcBtn.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));

	if (bEnabled)
	{
		Rectangle(pDC, rcBtn, COLOR_3DFACE, COLOR_3DFACE);

		if (bDropped)
		{
			pDC->FillSolidRect(rcBtn.left - XTP_DPI_X(1), rcBtn.top, XTP_DPI_X(1), rcBtn.Height(),
							   GetXtremeColor(COLOR_3DFACE));
			Draw3dRect(pDC, rcBtn, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
			rcBtn.OffsetRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		}
		else if (bSelected)
		{
			pDC->FillSolidRect(rcBtn.left - XTP_DPI_X(1), rcBtn.top, XTP_DPI_X(1), rcBtn.Height(),
							   GetXtremeColor(COLOR_3DFACE));
			Draw3dRect(pDC, rcBtn, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		}
		else
		{
			Draw3dRect(pDC, rcBtn, COLOR_BTNHILIGHT, COLOR_BTNHILIGHT);
		}
	}

	DrawComboExpandMark(pDC, rcBtn, GetXtremeColor(COLOR_BTNTEXT));
}

CSize CXTPCommandBarsOffice2000Theme::DrawControlComboBox(CDC* pDC,
														  CXTPControlComboBox* pControlCombo,
														  BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);
	}

	CXTPFontDC font(pDC, GetIconFont());

	CRect rc	  = pControlCombo->GetRect();
	BOOL bEnabled = pControlCombo->GetEnabled();

	CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);

	rc.left += pControlCombo->GetLabelWidth();

	int nThumb = pControlCombo->GetThumbWidth();
	CRect rcBtn(rc.right - nThumb, rc.top, rc.right, rc.bottom);

	BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
	BOOL bSelected = pControlCombo->GetSelected();

	COLORREF clrBackground = GetControlEditBackColor(pControlCombo);

	if (bEnabled)
	{
		CRect rcEntry(rc);
		rcEntry.DeflateRect(1, 1);
		pDC->FillSolidRect(rcEntry, clrBackground);
	}

	CRect rcText(rc.left + XTP_DPI_X(3), rc.top + XTP_DPI_Y(1),
				 rc.right - rcBtn.Width() + XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(1));
	pControlCombo->DrawEditText(pDC, rcText);

	DrawControlComboBoxButton(pDC, rcBtn, bEnabled, bSelected, bDropped);
	DrawControlEditFrame(pDC, rc, bEnabled, bSelected);

	return 0;
}

CSize CXTPCommandBarsOffice2000Theme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType,
														 CXTPControl* pButton, CXTPCommandBar* pBar,
														 BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandFloating || controlType == xtpButtonHideFloating)
	{
		if (pButton->GetPressed() || pButton->GetSelected()
			|| (controlType == xtpButtonExpandFloating && pButton->GetPopuped()))
			pDC->FillSolidRect(pButton->GetRect(), GetXtremeColor(COLOR_3DFACE));
	}

	return CXTPPaintManager::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);
}

CSize CXTPCommandBarsOffice2000Theme::GetPopupBarImageSize(CXTPCommandBar* pBar)
{
	CSize szIcon = pBar->GetIconSize();
	CSize szImage(max(szIcon.cx + XTP_DPI_X(4), m_nTextHeight),
				  max(szIcon.cy + XTP_DPI_Y(4), m_nTextHeight));
	return szImage;
}

CSize CXTPCommandBarsOffice2000Theme::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton,
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
		 bChecked = pButton->GetChecked(), bPopuped = pButton->GetPopuped();
	if (controlType == xtpControlSplitButtonPopup && bEnabled == TRUE_SPLITDROPDOWN)
		bEnabled = FALSE;

	BOOL bLayoutRTL		= pParent->IsLayoutRTL();
	BOOL bDoubleGripper = pParent->IsPopupBar() ? ((CXTPPopupBar*)pParent)->IsDoubleGripper()
												: FALSE;

	CString strCaption = pButton->GetCaption();
	if (bLayoutRTL)
		UpdateRTLCaption(strCaption);

	CSize szIcon = GetIconSize(pButton);
	CSize szImage(max(szIcon.cx + XTP_DPI_X(4), m_nTextHeight),
				  max(szIcon.cy + XTP_DPI_Y(4), m_nTextHeight));

	if (bDraw)
	{
		CRect rcSelection(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom);
		CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx, rcButton.bottom);
		CRect rcCheck(CPoint(rcImageRect.left + rcImageRect.Width() / 2 - XTP_DPI_X(10),
							 rcImageRect.top + rcImageRect.Height() / 2 - XTP_DPI_Y(10)),
					  XTP_DPI(CSize(20, 20)));

		if (pButton->GetExpanded())
		{
			int nGripperWidth = GetPopupBarGripperWidth(pParent);

			pDC->FillSolidRect(rcButton, GetXtremeColor(COLOR_3DLIGHT));

			if (pButton->GetExpanded() && rcButton.left < nGripperWidth)
			{
				DrawPopupBarGripper(pDC, rcButton.left - XTP_DPI_X(1), rcButton.top,
									nGripperWidth + XTP_DPI_X(1), rcButton.Height(), TRUE);
			}
		}

		COLORREF clrText = GetRectangleTextColor(bSelected, FALSE, bEnabled, FALSE, FALSE,
												 xtpBarTypePopup, xtpBarPopup);
		pDC->SetTextColor(clrText);

		if (m_bSelectImageInPopupBar && bSelected)
			DrawRectangle(pDC, rcSelection, bSelected, FALSE, TRUE, FALSE, FALSE, xtpBarTypePopup,
						  xtpBarPopup);

		if (bChecked)
		{
			DrawRectangle(pDC, rcImageRect, bSelected, FALSE, bEnabled, bChecked, FALSE,
						  xtpBarTypePopup, xtpBarPopup);
			rcSelection.left = rcImageRect.right + XTP_DPI_X(1);
		}

		if (bDoubleGripper)
			rcImageRect.OffsetRect(szImage.cx + XTP_DPI_X(1), 0);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage
										   ? pButton->GetImage(szIcon.cx)
										   : NULL;
		if (pImage)
		{
			szIcon = AdjustDpiIconSize(pImage, szIcon, pButton, rcImageRect.Size());
			if (!m_bSelectImageInPopupBar)
				DrawRectangle(pDC, rcImageRect, bSelected, FALSE, bEnabled,
							  bDoubleGripper ? FALSE : bChecked, FALSE, xtpBarTypeNormal,
							  xtpBarPopup);
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

			rcSelection.left = rcImageRect.right + XTP_DPI_X(1);
		}

		if (bChecked && (bDoubleGripper || !pImage))
		{
			rcCheck.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));

			if (controlType != xtpControlRadioButton)
				DrawPopupBarCheckMark(pDC, rcCheck, bEnabled,
									  GetXtremeColor(
										  UINT(!bEnabled ? COLOR_GRAYTEXT : m_nPopupBarText)));
			else
				DrawPopupBarRadioMark(pDC, rcCheck, bEnabled,
									  GetXtremeColor(
										  UINT(!bEnabled ? COLOR_GRAYTEXT : m_nPopupBarText)));
		}

		pDC->SetBkMode(TRANSPARENT);

		if (IsPopupControlType(controlType))
		{
			CRect rc = rcButton;
			rc.left  = rc.right - rc.Height();
			rc.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));

			if (pButton->GetType() == xtpControlSplitButtonPopup)
			{
				if (!bPopuped || !bSelected || !bEnabled)
				{
					if (bSelected && !m_bSelectImageInPopupBar)
						DrawRectangle(pDC, rcSelection, bSelected, FALSE, TRUE, FALSE, FALSE,
									  xtpBarTypePopup, xtpBarPopup);
					VerticalLine(pDC, rcButton.right - XTP_DPI_X(20), rcButton.top + XTP_DPI_Y(1),
								 rcButton.bottom - XTP_DPI_Y(1),
								 !bEnabled ? clrText
										   : GetXtremeColor(XTPToUIntChecked(
												 bSelected ? COLOR_3DSHADOW : m_nPopupBarText)));
				}
				else
				{
					rcSelection.right -= XTP_DPI_X(20);
					Draw3dRect(pDC,
							   CRect(rcButton.right - XTP_DPI_X(20), rcButton.top, rcButton.right,
									 rcButton.bottom),
							   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
					if (bSelected && !m_bSelectImageInPopupBar)
						DrawRectangle(pDC, rcSelection, bSelected, FALSE, TRUE, FALSE, FALSE,
									  xtpBarTypePopup, xtpBarPopup);
				}

				COLORREF clr = m_bSelectImageInPopupBar || !bPopuped || !bSelected
								   ? clrText
								   : GetXtremeColor(XTPToUIntChecked(m_nPopupBarText));
				XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayRight, rc, clr);
			}
			else
			{
				if (bSelected && !m_bSelectImageInPopupBar)
					DrawRectangle(pDC, rcSelection, bSelected, FALSE, TRUE, FALSE, FALSE,
								  xtpBarTypePopup, xtpBarPopup);

				XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayRight, rc, clrText);
			}
		}
		else if (bSelected && !m_bSelectImageInPopupBar)
			DrawRectangle(pDC, rcSelection, bSelected, FALSE, TRUE, FALSE, FALSE, xtpBarTypePopup,
						  xtpBarPopup);
	}

	return CXTPPaintManager::DrawControlPopupParent(pDC, pButton, bDraw);
}

COLORREF CXTPCommandBarsOffice2000Theme::GetRectangleTextColor(BOOL bSelected, BOOL /*bPressed*/,
															   BOOL bEnabled, BOOL bChecked,
															   BOOL /*bPopuped*/,
															   XTPBarType barType,
															   XTPBarPosition barPosition)
{
	if (barType == xtpBarTypePopup)
	{
		return GetXtremeColor(XTPToUIntChecked(
			!bEnabled ? COLOR_GRAYTEXT
					  : bSelected && !bChecked ? COLOR_HIGHLIGHTTEXT : m_nPopupBarText));
	}

	UINT nColorFace = XTPToUIntChecked(barPosition == xtpBarPopup ? m_nPopupBarText
																  : COLOR_BTNTEXT);
	return GetXtremeColor(!bEnabled ? COLOR_GRAYTEXT : nColorFace);
}

void CXTPCommandBarsOffice2000Theme::DrawShadedRect(CDC* pDC, CRect& rect)
{
	WORD bits[] = { 0x0055, 0x00AA, 0x0055, 0x00AA, 0x0055, 0x00AA, 0x0055, 0x00AA };

	CBitmap bitmap;
	bitmap.CreateBitmap(8, 8, 1, 1, &bits);

	CBrush brush;
	brush.CreatePatternBrush(&bitmap);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	COLORREF crBack = pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
	COLORREF crText = pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);

	brush.DeleteObject();
}

void CXTPCommandBarsOffice2000Theme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton,
														  CRect rcButton)
{
	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		 bPopuped = pButton->GetPopuped();

	if (pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		int nSplitDropDownHeight = GetSplitDropDownHeight();

		BOOL bVert = GetControlTextOrientation(pButton) == xtpBarTextVertical;

		if (bVert)
			DrawRectangle(pDC,
						  CRect(rcButton.left + nSplitDropDownHeight, rcButton.top, rcButton.right,
								rcButton.bottom),
						  bSelected || bPopuped, bPressed, bEnabled, bChecked, FALSE,
						  xtpBarTypeNormal, pButton->GetParent()->GetPosition());
		else
			DrawRectangle(pDC,
						  CRect(rcButton.left, rcButton.top, rcButton.right,
								rcButton.bottom - nSplitDropDownHeight),
						  bSelected || bPopuped, bPressed, bEnabled, bChecked, FALSE,
						  xtpBarTypeNormal, pButton->GetParent()->GetPosition());

		if ((bSelected || bPressed || bPopuped) && bEnabled)
		{
			if (bVert)
				DrawRectangle(pDC,
							  CRect(rcButton.left, rcButton.top,
									rcButton.left + nSplitDropDownHeight, rcButton.bottom),
							  TRUE, FALSE, bEnabled, FALSE, bPopuped, xtpBarTypeNormal,
							  pButton->GetParent()->GetPosition());
			else
				DrawRectangle(pDC,
							  CRect(rcButton.left, rcButton.bottom - nSplitDropDownHeight,
									rcButton.right, rcButton.bottom),
							  TRUE, FALSE, bEnabled, FALSE, bPopuped, xtpBarTypeNormal,
							  pButton->GetParent()->GetPosition());
		}

		return;
	}

	DrawRectangle(pDC,
				  CRect(rcButton.left, rcButton.top, rcButton.right - XTP_DPI_X(12),
						rcButton.bottom),
				  bSelected || bPopuped, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal,
				  pButton->GetParent()->GetPosition());

	CPoint pt = CPoint(rcButton.right - m_nSplitButtonDropDownWidth / 2, rcButton.CenterPoint().y);

	if ((bSelected || bPressed || bPopuped) && bEnabled)
		DrawRectangle(pDC,
					  CRect(rcButton.right - m_nSplitButtonDropDownWidth, rcButton.top,
							rcButton.right, rcButton.bottom),
					  TRUE, FALSE, bEnabled, FALSE, bPopuped, xtpBarTypeNormal,
					  pButton->GetParent()->GetPosition());

	if (bEnabled == TRUE_SPLITCOMMAND)
		bEnabled = FALSE;
	DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
}

void CXTPCommandBarsOffice2000Theme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	if (pBar->GetPosition() == xtpBarFloating || pBar->GetPosition() == xtpBarPopup)
	{
		Draw3dRect(pDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW);
		rc.DeflateRect(1, 1);
		Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
	}
	else if (!IsFlatToolBar(pBar))
	{
		Draw3dRect(pDC, rc, COLOR_BTNHILIGHT, COLOR_3DSHADOW);
	}
}

CSize CXTPCommandBarsOffice2000Theme::DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* /*pBar*/,
														   BOOL bDraw)
{
	CSize sz(XTP_DPI_X(6), max(XTP_DPI_Y(22), m_nTextHeight + XTP_DPI_Y(4)));

	if (pDC && bDraw)
	{
		Draw3dRect(pDC, CRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_Y(6), sz.cy), COLOR_BTNHILIGHT,
				   COLOR_3DSHADOW);
	}

	return sz;
}

CSize CXTPCommandBarsOffice2000Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
															BOOL bDraw)
{
	if (pBar->IsDialogBar())
		return DrawDialogBarGripper(pDC, (CXTPDialogBar*)pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		return CXTPPaintManager::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
			Draw3dRect(pDC,
					   CRect(XTP_DPI_X(3), XTP_DPI_Y(3), rc.right - XTP_DPI_X(3), XTP_DPI_Y(6)),
					   COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		return CSize(0, XTP_DPI_Y(8));
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
			Draw3dRect(pDC,
					   CRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(6), rc.bottom - XTP_DPI_Y(3)),
					   COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		return CSize(XTP_DPI_X(6), 0);
	}
	return 0;
}

CSize CXTPCommandBarsOffice2000Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
															  CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		if (pBar->GetType() == xtpBarTypePopup)
			return XTP_DPI(CSize(8, 8));

		return XTP_DPI(CSize(6, 6));
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRow		= pControl->GetRowRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		if (!pControl->GetWrap())
			Draw3dRect(pDC,
					   CRect(rcControl.left + XTP_DPI_X(11), rcControl.top - XTP_DPI_Y(5),
							 rcControl.right - XTP_DPI_X(11), rcControl.top - XTP_DPI_Y(3)),
					   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		else
			Draw3dRect(pDC,
					   CRect(rcControl.left - XTP_DPI_X(5), rcRow.top + XTP_DPI_Y(3),
							 rcControl.left - XTP_DPI_X(3), rcRow.bottom - XTP_DPI_Y(3)),
					   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
			Draw3dRect(pDC,
					   CRect(rcRow.left, rcControl.top - XTP_DPI_Y(4), rcRow.right,
							 rcControl.top - XTP_DPI_Y(2)),
					   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		else
			Draw3dRect(pDC,
					   CRect(rcRow.right + XTP_DPI_X(2), rcRow.top + XTP_DPI_Y(4),
							 rcRow.right + XTP_DPI_X(4), rcRow.bottom),
					   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	else
	{
		if (!pControl->GetWrap())
			Draw3dRect(pDC,
					   CRect(rcControl.left - XTP_DPI_X(4), rcRow.top,
							 rcControl.left - XTP_DPI_X(2), rcRow.bottom),
					   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
		else
			Draw3dRect(pDC,
					   CRect(rcRow.left + XTP_DPI_X(4), rcRow.top - XTP_DPI_Y(4), rcRow.right,
							 rcRow.top - XTP_DPI_Y(2)),
					   COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
	return 0;
}

void CXTPCommandBarsOffice2000Theme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl,
													   BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);
	if (!pParent)
		return;

	if (pControl->GetType() == xtpControlComboBox)
		return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.InflateRect(0, XTP_DPI_Y(1));
		else
			rc.InflateRect(XTP_DPI_X(1), 0);
	}
	else
	{
		rc.DeflateRect(XTP_DPI_X(1), 0);
	}
}

CRect CXTPCommandBarsOffice2000Theme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetType() == xtpBarTypePopup)
		return XTP_DPI(CRect(3, 3, 3, 3));
	else
		return CXTPPaintManager::GetCommandBarBorders(pBar);
}

CSize CXTPCommandBarsOffice2000Theme::DrawListBoxControl(CDC* pDC, CXTPControl* pButton,
														 CRect rcButton, BOOL bSelected, BOOL bDraw,
														 CXTPCommandBars* pCommandBars)
{
	CSize szIcon = pCommandBars && pCommandBars->GetCommandBarsOptions()->szPopupIcons != CSize(0)
					   ? pCommandBars->GetCommandBarsOptions()->szPopupIcons
					   : GetAutoIconSize(FALSE);
	CSize szImage(max(szIcon.cx + XTP_DPI_X(4), m_nTextHeight),
				  max(szIcon.cy + XTP_DPI_Y(4), m_nTextHeight));

	if (bDraw)
	{
		if (pButton == NULL)
		{
			pDC->FillSolidRect(rcButton, GetXtremeColor(COLOR_3DFACE));
			return 0;
		}
		pDC->FillSolidRect(rcButton, GetXtremeColor(COLOR_3DFACE));

		CRect rcSelection(rcButton.left + XTP_DPI_X(1), rcButton.top, rcButton.right - XTP_DPI_X(1),
						  rcButton.bottom);
		XTPControlType controlType = pButton->GetType();
		XTPButtonStyle buttonStyle = pButton->GetStyle();

		BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup)
						  || (buttonStyle == xtpButtonIcon)
						  || (buttonStyle == xtpButtonIconAndCaption);

		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage
										   ? pButton->GetImage(szIcon.cx)
										   : NULL;
		if (pImage)
		{
			CRect rcImageRect(rcButton.left, rcButton.top, rcButton.left + szImage.cx,
							  rcButton.bottom);
			szIcon = AdjustDpiIconSize(pImage, szIcon, pButton, rcImageRect.Size());
			DrawRectangle(pDC, rcImageRect, bSelected, FALSE, TRUE, FALSE, FALSE, xtpBarTypeNormal,
						  xtpBarPopup);
			CPoint ptImage = CPoint(rcImageRect.left + rcImageRect.Width() / 2 - szIcon.cx / 2,
									rcImageRect.top + rcImageRect.Height() / 2 - szIcon.cy / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, FALSE, TRUE, FALSE, FALSE, FALSE);
			rcSelection.left = rcImageRect.right + XTP_DPI_X(1);
		}
		if (bSelected)
			Rectangle(pDC, rcSelection, COLOR_HIGHLIGHT, COLOR_HIGHLIGHT);

		COLORREF clrText = GetXtremeColor(UINT(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT));

		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);

		CString strText = pButton->GetCaption();
		StripMnemonics(strText);

		if (pCommandBars && pCommandBars->IsLayoutRTL())
			UpdateRTLCaption(strText);

		CRect rcText(rcButton.left + szImage.cx + XTP_DPI_X(4), rcButton.top, rcButton.right,
					 rcButton.bottom);
		CXTPFontDC fnt(pDC, &m_xtpFontRegular);
		pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

		if (controlType == xtpControlPopup || controlType == xtpControlButtonPopup
			|| controlType == xtpControlSplitButtonPopup)
		{
			CRect rc = rcButton;
			rc.left  = rc.right - rc.Height();
			rc.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayRight, rc, clrText);
		}
		if (controlType == xtpControlSplitButtonPopup)
		{
			Line(pDC, rcButton.right - XTP_DPI_X(20), rcButton.top + XTP_DPI_Y(1),
				 rcButton.right - XTP_DPI_X(20), rcButton.bottom - XTP_DPI_Y(1),
				 bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT);
		}

		if (controlType == xtpControlComboBox || controlType == xtpControlEdit)
		{
			rcText.SetRectEmpty();
			pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_CALCRECT);
			int nThumb = pDC->GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(2);

			CRect rcCombo(rcButton.left + szImage.cx + XTP_DPI_X(4) + rcText.Width() + XTP_DPI_X(5),
						  rcButton.top, rcButton.right, rcButton.bottom);
			CRect rcThumb(rcButton.right - nThumb, rcButton.top, rcButton.right, rcButton.bottom);

			Rectangle(pDC, rcCombo, bSelected ? COLOR_HIGHLIGHT : COLOR_3DFACE, COLOR_WINDOW);

			if (controlType == xtpControlComboBox)
			{
				if (!bSelected)
					pDC->FillSolidRect(rcThumb.left + XTP_DPI_X(2), rcThumb.top + XTP_DPI_Y(2),
									   rcThumb.Width() - XTP_DPI_X(4),
									   rcThumb.Height() - XTP_DPI_Y(4),
									   GetXtremeColor(COLOR_3DFACE));
				else
					Rectangle(pDC, rcThumb, COLOR_HIGHLIGHT, COLOR_3DFACE);

				rcThumb.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
				DrawComboExpandMark(pDC, rcThumb, GetXtremeColor(COLOR_BTNTEXT));
			}
		}
	}
	return szImage;
}

CSize CXTPCommandBarsOffice2000Theme::DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected,
														 BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc(rcGripper.left, rcGripper.top, rcGripper.right, rcGripper.bottom - XTP_DPI_Y(1));
		pDC->FillSolidRect(rc, GetXtremeColor(
								   UINT(bSelected ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION)));
	}

	return CSize(0, XTP_DPI_Y(8));
}

void CXTPCommandBarsOffice2000Theme::DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags)
{
	int c = rcGripper.CenterPoint().x;

	if (nFlags == 2)
	{
		for (int x = c - 8; x < c + 8; x += 4)
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

		x -= 4;
		pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), XTP_DPI_X(1) + y, x + XTP_DPI_X(3),
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
