// XTPCommandBarsOffice2013Theme.cpp : implementation of the CXTPCommandBarsOffice2013Theme class.
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

#include "CommandBars/Resource.h"

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPTheme.h"

// Scrollbar
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/Themes/XTPScrollBarThemeOffice2013.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPDockBar.h"
#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2013Theme.h"
#include "CommandBars/Gallery/XTPControlGalleryOffice2013Theme.h"

#ifdef _XTP_INCLUDE_RIBBON
#	include "Markup/Extensions/XTPMarkupStaticExtension.h"
#	include "Ribbon/XTPRibbonPaintManager.h"
#	include "Ribbon/XTPRibbonMarkupTheme.h"
#	include "Ribbon/Themes/XTPRibbonThemeOffice2013.h"
#endif

// Progress
#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/Progress/XTPProgressThemeOffice2013.h"

// Slider
#include "CommandBars/Slider/XTPSliderPaintManager.h"
#include "CommandBars/Slider/XTPSliderThemeOffice2013.h"

// Frame
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeOffice2013.h"

// Message bar
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2013.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOffice2013.h"

#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _XTP_INCLUDE_RIBBON
#endif

//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOffice2013Theme

IMPLEMENT_DYNAMIC(CXTPCommandBarsOffice2013Theme, CXTPCommandBarsModernUITheme)

CXTPCommandBarsOffice2013Theme::CXTPCommandBarsOffice2013Theme()
	: m_bIntersect(FALSE)
{
	// Frame paint manager
	SAFE_DELETE(m_pFramePaintManager);
	m_pFramePaintManager = new CXTPFrameThemeOffice2013(this);

	// Progress paint manager
	SAFE_DELETE(m_pProgressPaintManager);
	m_pProgressPaintManager = new CXTPProgressThemeOffice2013(this);

	// Slider paint manager
	SAFE_DELETE(m_pSliderPaintManager);
	m_pSliderPaintManager = new CXTPSliderThemeOffice2013(this);

	// Message bar paint manager
	SAFE_DELETE(m_pMessageBarPaintManager);
	m_pMessageBarPaintManager = new CXTPMessageBarThemeOffice2013(this);

	// Status bar paint manager
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeOffice2013(this);

	// Ribbon paint manager
	SAFE_DELETE(m_pRibbonPaintManager);
	m_pRibbonPaintManager = new CXTPRibbonOffice2013Theme(this);

	// Gallery paint manager
	SAFE_DELETE(m_pGalleryPaintManager);
	m_pGalleryPaintManager = new CXTPControlGalleryOffice2013Theme(this);

	// Scrollbar paint manager
	SAFE_DELETE(m_pScrollBarPaintManager);
	m_pScrollBarPaintManager = new CXTPScrollBarThemeOffice2013();

	m_strOfficeFont = _T("Segoe UI");
	UpdateFonts();
}

CXTPCommandBarsOffice2013Theme::~CXTPCommandBarsOffice2013Theme()
{
}

void CXTPCommandBarsOffice2013Theme::RefreshStandardColors()
{
	// Menu and toolbar colors.
	m_arrColor[COLOR_WINDOW]					 = m_csComboBox.clrBack;
	m_arrColor[COLOR_BTNFACE]					 = m_csToolbar.clrBack;
	m_arrColor[COLOR_APPWORKSPACE]				 = m_clrWorkspace.clrDark;
	m_arrColor[XPCOLOR_TOOLBAR_FACE]			 = m_csToolbar.clrBack;
	m_arrColor[XPCOLOR_HIGHLIGHT]				 = m_csToolbar.clrBackHighlight;
	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER]		 = m_csToolbar.clrBorderCheckedHighlight;
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]		 = m_csToolbar.clrBackPushed;
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER]  = m_csToolbar.clrBorderPushed;
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED]		 = m_csToolbar.clrBackChecked;
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = m_csToolbar.clrBorderChecked;
	m_arrColor[XPCOLOR_GRAYTEXT]				 = m_csToolbar.clrTextDisabled;
	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]			 = m_csToolbar.clrGripper;
	m_arrColor[XPCOLOR_SEPARATOR]				 = m_csToolbar.clrSeparator.clrDark;
	m_arrColor[XPCOLOR_DISABLED]				 = m_csToolbar.clrDisabledIcon;
	m_arrColor[XPCOLOR_MENUBAR_FACE]			 = m_csMenu.clrMenuBack;
	m_arrColor[XPCOLOR_MENUBAR_BORDER]			 = m_csMenu.clrMenuBorder;
	m_arrColor[XPCOLOR_MENUBAR_TEXT]			 = m_csMenu.clrText;
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]			 = m_csToolbar.clrTextHighlight;
	m_arrColor[XPCOLOR_TOOLBAR_TEXT]			 = m_csToolbar.clrText;
	m_arrColor[XPCOLOR_PUSHED_TEXT]				 = m_csToolbar.clrTextPushed;
	m_arrColor[XPCOLOR_TAB_INACTIVE_BACK]		 = m_csRibbon.clrTabBackInactive;
	m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT]		 = m_csRibbon.clrTabTextInactive;
	m_arrColor[XPCOLOR_CHECKED_TEXT]			 = m_csToolbar.clrTextChecked;
	m_arrColor[XPCOLOR_3DFACE]					 = m_csToolbar.clrBack;
	m_arrColor[XPCOLOR_3DSHADOW]				 = m_csToolbar.clrSeparator.clrDark;
	m_arrColor[XPCOLOR_EDITCTRLBORDER]			 = m_csComboBox.clrBorder;
	m_arrColor[XPCOLOR_FRAME]					 = m_csToolbar.clrEditBorder;
	m_arrColor[XPCOLOR_SPLITTER_FACE]			 = m_csToolbar.clrBack;
	m_arrColor[XPCOLOR_LABEL]					 = m_csToolbar.clrBack;
	m_arrColor[XPCOLOR_STATICFRAME]				 = m_csToolbar.clrEditBorder;
	m_arrColor[XPCOLOR_TOOLBAR_GRAYTEXT]		 = m_csToolbar.clrTextDisabled;
	m_arrColor[XPCOLOR_FLOATBAR_BORDER]			 = m_csToolbar.clrEditBorder;
}

void CXTPCommandBarsOffice2013Theme::RefreshMetrics()
{
	CXTPCommandBarsModernUITheme::RefreshMetrics();

	// load colorset state values.

	m_csMenu.RefreshColors();
	m_csRibbon.RefreshColors();
	m_csToolbar.RefreshColors();
	m_csCheckMark.RefreshColors();
	m_csComboBox.RefreshColors();
	m_csCheckBox.RefreshColors();

	// load remaining colors - Default colors found in OFFICE2013WORD_INI.

	m_clrBack.SetStandardValue(XTPIniColor(_T("CommandBars"), _T("Back"), RGB(255, 255, 255)));
	m_clrWorkspace.clrDark.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("WorkspaceDark"), RGB(240, 240, 240)));
	m_clrWorkspace.clrLight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("WorkspaceLight"), RGB(255, 255, 255)));
	m_clrGlyph.SetStandardValue(XTPIniColor(_T("CommandBars"), _T("Glyph"), RGB(119, 119, 119)));
	m_clrGlyphHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("GlyphHighlight"), RGB(68, 68, 68)));
	m_clrGlyphPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("GlyphPushed"), RGB(68, 68, 68)));
	m_clrGlyphDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("GlyphDisabled"), RGB(177, 177, 177)));
	m_clrEditTextNormal.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("EditText"), RGB(0, 0, 0)));
	m_clrEditTextHighlighted.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("EditTextHighlight"), RGB(0, 0, 0)));
	m_clrEditTextDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("EditTextDisabled"), RGB(152, 152, 152)));
	m_clrMessageBarText.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("Text"), RGB(119, 119, 119)));
	m_clrFloatingGripper.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("GripperFloating"), RGB(255, 255, 255)));
	m_clrFloatingGripperText.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("GripperFloatingText"), RGB(68, 68, 68)));
	m_clrCaptionButtonFill.SetStandardValue(XTPIniColor(_T("CommandBars.Frame"),
														_T("CaptionButtonActiveFillColor"),
														RGB(119, 119, 119)));

	m_clrQuickAccess = XTPIniColor(_T("CommandBars.Frame"), _T("ActiveFontColor"), RGB(38, 38, 38));
	m_clrQuickAccessDisabled	  = XTPIniColor(_T("CommandBars.Frame"), _T("InactiveFontColor"),
											RGB(38, 38, 38));
	m_clrQuickAccessButtonHotBack = XTPIniColor(_T("CommandBars.Frame"),
												_T("CaptionButtonHotBackgroundColor"),
												RGB(255, 255, 255));
	m_clrQuickAccessButtonPressedBack = XTPIniColor(_T("CommandBars.Frame"),
													_T("CaptionButtonPressedBackgroundColor"),
													RGB(255, 255, 255));

	RefreshStandardColors();
}

BOOL CXTPCommandBarsOffice2013Theme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	ASSERT_VALID(pButton);

	if (pButton->GetID() == XTP_ID_TOOLBAR_EXPAND)
	{
		BOOL bFloating = (pButton->GetParent()->GetPosition() == xtpBarFloating);

		if (pButton->GetPopuped() || pButton->GetSelected())
		{
			Rectangle(pDC, pButton->GetRect(),
					  bFloating ? m_csToolbar.clrBorderHighlight : m_csToolbar.clrBackHighlight,
					  m_csToolbar.clrBackHighlight);
		}
		else
		{
			Rectangle(pDC, pButton->GetRect(), m_csToolbar.clrToolbarBack,
					  m_csToolbar.clrToolbarBack);
		}
		return TRUE;
	}
	else if (pButton->GetID() == XTP_ID_CUSTOMIZE_ADDORREMOVE)
	{
		if (pButton->GetPopuped() || pButton->GetSelected())
		{
			Rectangle(pDC, pButton->GetRect(), m_csToolbar.clrBorderCheckedHighlight,
					  m_csToolbar.clrBackHighlight);
		}
		else
		{
			Rectangle(pDC, pButton->GetRect(), m_csMenu.clrBorder, m_csMenu.clrBack);
		}
		return TRUE;
	}
	else if (pButton->IsOnQuickAccess())
	{
		CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pButton->GetParent());
		if (NULL == pRibbon)
			return FALSE;
		if (pRibbon->IsQuickAccessBelowRibbon())
			return FALSE;
		CRect rcButton = pButton->GetRect();
		BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
			 bEnabled = pButton->GetEnabled(), bPopuped = pButton->GetPopuped(),
			 bChecked = pButton->GetChecked();
		if (!bEnabled)
			return TRUE;
		COLORREF clrBack = COLORREF_NULL, clrFrame = COLORREF_NULL;
		if (bSelected)
		{
			if (bPressed)
				clrBack = m_clrQuickAccessButtonPressedBack;
			else
			{
				clrBack = m_clrQuickAccessButtonHotBack;
				if (bChecked)
					clrFrame = m_clrQuickAccessButtonPressedBack;
			}
		}
		else
		{
			if (bPressed || bPopuped)
				clrBack = m_clrQuickAccessButtonPressedBack;
			else if (bChecked)
				clrBack = m_clrQuickAccessButtonHotBack;
		}
		Rectangle(pDC, rcButton, clrFrame, clrBack);
		return TRUE;
	}

	return FALSE;
}

void CXTPCommandBarsOffice2013Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
												   BOOL bPressed, BOOL bEnabled, BOOL bChecked,
												   BOOL bPopuped, XTPBarType barType,
												   XTPBarPosition barPosition, int nID)
{
	CXTPCommandBarsModernUITheme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked,
												bPopuped, barType, barPosition, nID);
}

void CXTPCommandBarsOffice2013Theme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	ASSERT_VALID(pBar);

	CXTPClientRect rcBar(pBar);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		Rectangle(pDC, rcBar, m_csToolbar.clrEditBorder, m_csToolbar.clrBack);
	}
	else if (pBar->GetPosition() == xtpBarPopup)
	{
		Rectangle(pDC, rcBar, m_csMenu.clrMenuBorder, m_csMenu.clrMenuBack);

		CXTPCommandBar* pBarParent = pBar->GetParentCommandBar();

		if (pBarParent && (pBarParent->GetType() == xtpBarTypeMenuBar)
			&& (pBarParent->GetPosition() != xtpBarPopup))
		{
			FillIntersectRect(pDC, (CXTPPopupBar*)pBar, m_csMenu.clrMenuBack);
		}
	}
	else if (IsFlatToolBar(pBar))
	{
		pDC->FillSolidRect(rcBar, m_clrBack);
	}
	else // Docked command bar
	{
		Rectangle(pDC, rcBar, m_csToolbar.clrToolbarBorder, m_csToolbar.clrToolbarBack);
	}
}

void CXTPCommandBarsOffice2013Theme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	CXTPClientRect rcBar(pBar);
	pDC->FillSolidRect(rcBar, m_clrBack);
}

void CXTPCommandBarsOffice2013Theme::DrawCommandBarGroupSeparator(CDC* pDC, CXTPCommandBar* pBar,
																  CXTPControl* pLeftControl,
																  CXTPControl* pRightControl,
																  BOOL bDraw)
{
	UNREFERENCED_PARAMETER(pBar);

	if (bDraw)
	{
		ASSERT(NULL != pLeftControl);
		ASSERT(NULL != pRightControl);

		if (!pLeftControl || !pRightControl)
			return;

		const CRect& rcLeftControl  = pLeftControl->GetRect();
		const CRect& rcRightControl = pRightControl->GetRect();
		const int x					= (rcRightControl.left + rcLeftControl.right) / 2;
		const int topY				= min(rcRightControl.top, rcLeftControl.top);
		const int bottomY			= max(rcRightControl.bottom, rcLeftControl.bottom);
		Line(pDC, x, topY, x, bottomY, m_csToolbar.clrBorderHighlight);
	}
}

CSize CXTPCommandBarsOffice2013Theme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit,
													  BOOL bDraw)
{
	if (bDraw)
		ASSERT_VALID(pDC);
	ASSERT_VALID(pControlEdit);

	CXTPDCSaveState saveState(pDC);

	CSize szResult = CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);

	if (!bDraw)
	{
		return szResult;
	}

	BOOL bSelected = pControlEdit->GetSelected();
	BOOL bEnabled  = pControlEdit->GetEnabled();
	BOOL bPressed  = pControlEdit->GetPressed();

	CRect rc = pControlEdit->GetRect();
	rc.left += pControlEdit->GetLabelWidth();

	COLORREF clrBackground = GetControlEditBackColor(pControlEdit);

	if (!bEnabled)
	{
		rc.DeflateRect(XTP_DPI_X(1), 0, 0, 0);

		pDC->FillSolidRect(rc, clrBackground);

		COLORREF clrBorder = IsKeyboardSelected(bSelected) ? m_csComboBox.clrBorderHighlight
														   : m_csComboBox.clrBorderDisabled;
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
	}
	else
	{
		pDC->FillSolidRect(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1),
						   rc.Width() - XTP_DPI_X(pControlEdit->IsSpinButtonsVisible() ? 18 : 1),
						   rc.Height() - XTP_DPI_Y(1), clrBackground);

		if (bSelected)
		{
			pDC->Draw3dRect(rc, m_csComboBox.clrBorderHighlight, m_csComboBox.clrBorderHighlight);
		}
		else if (bPressed)
		{
			pDC->Draw3dRect(rc, m_csComboBox.clrBorderDropped, m_csComboBox.clrBorderDropped);
		}
		else // if (bPopupBar)
		{
			pDC->Draw3dRect(rc, m_csComboBox.clrBorder, m_csComboBox.clrBorder);
		}
	}

	CRect rcText(pControlEdit->GetRect());
	rcText.DeflateRect(pControlEdit->GetLabelWidth() + XTP_DPI_X(3), XTP_DPI_Y(1), XTP_DPI_X(3),
					   XTP_DPI_Y(1));
	pControlEdit->DrawEditText(pDC, rcText);

	if (pControlEdit->IsSpinButtonsVisible())
	{
		CRect rcSpin(pControlEdit->GetSpinButtonsRect());
		CRect rcTop(rcSpin.left, rcSpin.top, rcSpin.right, rcSpin.CenterPoint().y);
		CRect rcBottom(rcSpin.left, rcTop.bottom, rcSpin.right, rcSpin.bottom);

		if (bEnabled)
		{
			// Draw top spin button.
			COLORREF clrTop = clrBackground;
			if (XTP_EDITSPIN_UP == bPressed)
			{
				clrTop = m_csComboBox.clrDropDownBackDropped;
			}
			else if (XTP_EDITSPIN_UP == bSelected)
			{
				clrTop = m_csComboBox.clrDropDownBackHighlight;
			}
			pDC->FillSolidRect(rcTop, clrTop);

			// Draw bottom spin button.
			COLORREF clrBottom = clrBackground;
			if (XTP_EDITSPIN_DOWN == bPressed)
			{
				clrBottom = m_csComboBox.clrDropDownBackDropped;
			}
			else if (XTP_EDITSPIN_DOWN == bSelected)
			{
				clrBottom = m_csComboBox.clrDropDownBackHighlight;
			}
			pDC->FillSolidRect(rcBottom, clrBottom);

			// Draw spin button borders.
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			pControlEdit->GetEditCtrl()->GetParent()->ScreenToClient(&ptCursor);
			if (rcSpin.PtInRect(ptCursor))
			{
				pDC->FillSolidRect(rcSpin.left, rcSpin.top, XTP_DPI_X(1), rcSpin.Height(),
								   m_csComboBox.clrDropDownBorderDropped);
				pDC->FillSolidRect(rcSpin.left, rcBottom.top, rcSpin.Width(), XTP_DPI_Y(1),
								   m_csComboBox.clrDropDownBorderDropped);
			}
		}
		else
		{
			// Draw disabled spin buttons.
			pDC->FillSolidRect(rcTop, clrBackground);
			pDC->FillSolidRect(rcBottom, clrBackground);
		}

		// Draw top spin button triangle.

		COLORREF clrSpinArrow = bEnabled ? m_csComboBox.clrDropDownMarker
										 : m_csComboBox.clrDropDownMarkerDisabled;

		if (XTP_EDITSPIN_UP == bPressed)
		{
			clrSpinArrow = m_csComboBox.clrDropDownMarkerDropped;
		}
		else if (XTP_EDITSPIN_UP == bSelected)
		{
			clrSpinArrow = m_csComboBox.clrDropDownMarkerHighlight;
		}

		// Draw bottom spin button triangle.
		CRect rcArrow = rcTop;
		rcArrow.OffsetRect(0, 1);
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayUp2, rcArrow, clrSpinArrow);

		if (XTP_EDITSPIN_DOWN == bPressed)
		{
			clrSpinArrow = m_csComboBox.clrDropDownMarkerDropped;
		}
		else if (XTP_EDITSPIN_DOWN == bSelected)
		{
			clrSpinArrow = m_csComboBox.clrDropDownMarkerHighlight;
		}

		rcArrow = rcBottom;
		rcArrow.OffsetRect(0, -1);
		--rcArrow.bottom;
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown2, rcArrow, clrSpinArrow);
	}

	if (pControlEdit->IsBuddyButtonVisible())
	{
		BOOL bIsPressed = pControlEdit->GetPressed();
		CRect rcBuddyButton(pControlEdit->GetBuddyButtonRect());

		DrawRectangle(pDC, rcBuddyButton, bSelected == XTP_EDITBUDDYBUTTON,
					  bIsPressed == XTP_EDITBUDDYBUTTON, bEnabled, FALSE, FALSE, xtpBarTypeNormal,
					  xtpBarTop);

		CXTPImageManager* pImageManager = pControlEdit->GetParent()->GetImageManager();
		if (pImageManager)
		{
			CSize szIcon = pControlEdit->GetIconSize();

			CXTPImageManagerIcon* pIcon =
				pImageManager->GetImage(XTPToUInt(pControlEdit->GetBuddyButtonId()), szIcon.cx);
			if (pIcon)
			{
				szIcon = AdjustDpiIconSize(pIcon, szIcon, pControlEdit, rcBuddyButton.Size());

				CPoint pt = CPoint(rcBuddyButton.CenterPoint().x - szIcon.cx / 2,
								   rcBuddyButton.CenterPoint().y - szIcon.cy / 2);

				pIcon->Draw(pDC, pt,
							(!bEnabled ? xtpImageDisabled
									   : pControlEdit->GetSelected() == XTP_EDITBUDDYBUTTON
											 ? xtpImageHot
											 : xtpImageNormal),
							szIcon);
			}
		}
	}

	return szResult;
}

void CXTPCommandBarsOffice2013Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
												   BOOL bPressed, BOOL bEnabled, BOOL bChecked,
												   BOOL bPopuped, XTPBarType barType,
												   XTPBarPosition barPosition)
{
	UNREFERENCED_PARAMETER(barPosition);

	CXTPColorSetStateButton* pColorSet = GetButtonColorSet(barType);

	if (!bEnabled)
	{
		if (bChecked)
		{
			if (!IsKeyboardSelected(bSelected))
				Rectangle(pDC, rc, pColorSet->clrBackDisabled, pColorSet->clrBackDisabled);
		}
		else if (IsKeyboardSelected(bSelected))
		{
			CRect rcFill(rc);
			rcFill.DeflateRect(2, 2);

			pDC->Draw3dRect(rc, pColorSet->clrBorderDisabled, pColorSet->clrBorderDisabled);
			pDC->FillSolidRect(rcFill, pColorSet->clrBackDisabled);
		}
	}

	else if (bPopuped)
	{
		if (barType == xtpBarTypeMenuBar)
		{
			Rectangle(pDC, rc, m_csMenu.clrMenuBorder, m_csMenu.clrMenuBack);
		}
		else
		{
			Rectangle(pDC, rc, pColorSet->clrBorderChecked, pColorSet->clrBackChecked);
		}
	}

	else
	{
		if (bChecked && !bSelected && !bPressed)
		{
			Rectangle(pDC, rc, pColorSet->clrBorderChecked,
					  (bChecked == 2) ? pColorSet->clrBack : pColorSet->clrBackChecked);
		}
		else if (bChecked && bSelected && !bPressed)
		{
			Rectangle(pDC, rc, pColorSet->clrBorderCheckedHighlight,
					  pColorSet->clrBackCheckedHighlight);
		}
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed))
		{
			Rectangle(pDC, rc, pColorSet->clrBorderPushed, pColorSet->clrBackPushed);
		}
		else if (bSelected || bPressed)
		{
			Rectangle(pDC, rc, pColorSet->clrBorderHighlight, pColorSet->clrBackHighlight);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Workspace

void CXTPCommandBarsOffice2013Theme::FillWorkspace(CDC* pDC, CRect rc, CRect rcExclude)
{
	UNUSED_ALWAYS(rcExclude);
	XTPDrawHelpers()->GradientFill(pDC, rc, m_clrWorkspace, FALSE);
}

void DrawDot(CDC* pDC, CPoint pt)
{
	pt.Offset(-2, -2); // start at top left.

	pDC->SetPixel(pt, RGB(195, 195, 195));
	pt.Offset(0, 1);
	pDC->SetPixel(pt, RGB(135, 135, 135));
	pt.Offset(0, 1);
	pDC->SetPixel(pt, RGB(195, 195, 195));
	pt.Offset(1, -2);

	pDC->SetPixel(pt, RGB(135, 135, 135));
	pt.Offset(0, 1);
	pDC->SetPixel(pt, RGB(119, 119, 119));
	pt.Offset(0, 1);
	pDC->SetPixel(pt, RGB(135, 135, 135));
	pt.Offset(1, -2);

	pDC->SetPixel(pt, RGB(195, 195, 195));
	pt.Offset(0, 1);
	pDC->SetPixel(pt, RGB(135, 135, 135));
	pt.Offset(0, 1);
	pDC->SetPixel(pt, RGB(195, 195, 195));
}

void DrawGripper(CDC* pDC, CRect rcGripper, BOOL bHorz)
{
	CPoint pt = rcGripper.CenterPoint();

	if (bHorz)
	{
		pt.Offset(1, 0);

		pt.Offset(0, -6);
		DrawDot(pDC, pt);

		pt.Offset(0, 6);
		DrawDot(pDC, pt);

		pt.Offset(0, 6);
		DrawDot(pDC, pt);
	}
	else
	{
		pt.Offset(0, 1);

		pt.Offset(-6, 0);
		DrawDot(pDC, pt);

		pt.Offset(6, 0);
		DrawDot(pDC, pt);

		pt.Offset(6, 0);
		DrawDot(pDC, pt);
	}
}

void CXTPCommandBarsOffice2013Theme::DrawDashedLine(CDC* pDC, int x, int y, int z, COLORREF clr,
													BOOL bHorzBar)
{
	CRect rcDot(x, y, x + XTP_DPI_X(1), y + XTP_DPI_Y(1));
	if (bHorzBar)
	{
		int nStep = XTP_DPI_Y(4);
		while (z > rcDot.top)
		{
			pDC->FillSolidRect(rcDot, clr);
			rcDot.top += nStep;
			rcDot.bottom += nStep;
		}
	}
	else
	{
		int nStep = XTP_DPI_X(4);
		while (z >= rcDot.left)
		{
			pDC->FillSolidRect(rcDot, clr);
			rcDot.left += nStep;
			rcDot.right += nStep;
		}
	}
}

CSize CXTPCommandBarsOffice2013Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
															BOOL bDraw)
{
	if (bDraw)
		ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

	CSize szGripper(0, 0);

	if (bDraw ? pBar->GetShowGripper() : TRUE)
	{
		CRect rcBar;
		pBar->GetClientRect(&rcBar);

		XTPBarPosition barPos = pBar->GetPosition();

		if (barPos == xtpBarRight || barPos == xtpBarLeft || barPos == xtpBarTop
			|| barPos == xtpBarBottom)
		{
			szGripper = CXTPCommandBarsModernUITheme::DrawCommandBarGripper(pDC, pBar, FALSE);

			if (bDraw && !pBar->IsDialogBar())
			{
				if (barPos == xtpBarRight || barPos == xtpBarLeft)
				{
					CPoint ptDash(rcBar.left + XTP_DPI_X(3), rcBar.top + XTP_DPI_Y(3));
					int nDashZ = rcBar.right - XTP_DPI_X(3);
					DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, m_csToolbar.clrGripper, FALSE);

					ptDash.x += XTP_DPI_X(2);
					ptDash.y += XTP_DPI_Y(2);
					nDashZ -= XTP_DPI_X(1);
					DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, m_csToolbar.clrGripper, FALSE);

					ptDash.x -= XTP_DPI_X(2);
					ptDash.y += XTP_DPI_Y(2);
					nDashZ += XTP_DPI_X(1);
					DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, m_csToolbar.clrGripper, FALSE);
				}
				else if (barPos == xtpBarTop || barPos == xtpBarBottom)
				{
					CPoint ptDash(rcBar.left + XTP_DPI_X(3), rcBar.top + XTP_DPI_Y(5));
					int nDashZ = rcBar.bottom - XTP_DPI_Y(3);
					DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, m_csToolbar.clrGripper, TRUE);

					ptDash.x += XTP_DPI_X(2);
					ptDash.y += XTP_DPI_Y(2);
					nDashZ -= XTP_DPI_X(1);
					DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, m_csToolbar.clrGripper, TRUE);

					ptDash.x += XTP_DPI_X(2);
					ptDash.y -= XTP_DPI_Y(2);
					nDashZ += XTP_DPI_X(1);
					DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, m_csToolbar.clrGripper, TRUE);
				}
			}
		}
		else if (barPos == xtpBarFloating)
		{
			CXTPFontDC font(pDC, GetSmCaptionFont());
			CSize sz = pDC->GetTextExtent(_T(" "), 1);

			rcBar.SetRect(XTP_DPI_X(3), XTP_DPI_Y(3), rcBar.right - XTP_DPI_X(3),
						  XTP_DPI_Y(3) + max(XTP_DPI_Y(15), sz.cy));
			if (pDC && bDraw)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->FillSolidRect(rcBar, m_clrFloatingGripper);
				pDC->SetTextColor(m_clrFloatingGripperText);
				pDC->DrawText(pBar->GetTitle(),
							  CRect(XTP_DPI_X(5), XTP_DPI_Y(3), rcBar.right - 2 * rcBar.Size().cy,
									rcBar.bottom),
							  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			}

			szGripper = rcBar.Size();

			if (bDraw && !pBar->IsDialogBar() && pBar->GetType() == xtpBarTypePopup)
			{
				CRect rcBorders = pBar->GetBorders();
				int nTop		= rcBorders.top + szGripper.cy + XTP_DPI_Y(1);
				DrawPopupBarGripper(pDC, rcBorders.left - XTP_DPI_X(1), nTop,
									GetPopupBarGripperWidth(pBar),
									rcBar.bottom - nTop - rcBorders.bottom);
			}
		}
		else
		{
			szGripper = CXTPCommandBarsModernUITheme::DrawCommandBarGripper(pDC, pBar, bDraw);
		}
	}

	return szGripper;
}

COLORREF CXTPCommandBarsOffice2013Theme::GetControlEditBackColor(CXTPControl* pControl)
{
	BOOL bDropped  = FALSE;
	BOOL bSelected = FALSE;
	BOOL bEnabled  = FALSE;

	CXTPControlEdit* pEdit = DYNAMIC_DOWNCAST(CXTPControlEdit, pControl);
	if (pEdit)
	{
		bSelected = pEdit->GetSelected();
		bEnabled  = pEdit->GetEnabled();
	}

	CXTPControlComboBox* pCombo = DYNAMIC_DOWNCAST(CXTPControlComboBox, pControl);
	if (pCombo)
	{
		bDropped  = pCombo->GetDroppedState() != 0;
		bSelected = pCombo->GetSelected();
		bEnabled  = pCombo->GetEnabled();
	}

	if (!bEnabled)
	{
		return m_csComboBox.clrBackDisabled;
	}
	else if (bDropped)
	{
		return m_csComboBox.clrBackDropped;
	}
	else if (bSelected)
	{
		return m_csComboBox.clrBackHighlight;
	}
	else if (pEdit || pCombo)
	{
		return m_csComboBox.clrBack;
	}

	return CXTPCommandBarsModernUITheme::GetControlEditBackColor(pControl);
}

CSize CXTPCommandBarsOffice2013Theme::DrawControlComboBox(CDC* pDC,
														  CXTPControlComboBox* pControlCombo,
														  BOOL bDraw)
{
	// call base class to draw label.
	CSize szResult = CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw); // Draw label

	if (!bDraw)
	{
		return szResult;
	}

	BOOL bIsOnAuicAccessAboveRibbon = FALSE;
	if (pControlCombo->IsOnQuickAccess())
	{
		CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControlCombo->GetParent());
		if (NULL != pRibbon)
		{
			if (!pRibbon->IsQuickAccessBelowRibbon())
				bIsOnAuicAccessAboveRibbon = TRUE;
		}
	}

	BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
	BOOL bSelected = pControlCombo->GetSelected();
	BOOL bEnabled  = pControlCombo->GetEnabled();

	COLORREF clrComboBoxBackground = m_csComboBox.clrBack;
	COLORREF clrComboBoxBorder	 = m_csComboBox.clrBorder;
	COLORREF clrDropDownBackground = m_csComboBox.clrDropDownBack;
	COLORREF clrDropDownBorder	 = m_csComboBox.clrDropDownBorder;
	COLORREF clrDropDownMarker	 = m_csComboBox.clrDropDownMarker;

	if (!bEnabled)
	{
		clrComboBoxBackground = m_csComboBox.clrBackDisabled;
		clrComboBoxBorder	 = m_csComboBox.clrBorderDisabled;
		clrDropDownBackground = m_csComboBox.clrDropDownBackDisabled;
		clrDropDownBorder	 = m_csComboBox.clrDropDownBorderDisabled;
		clrDropDownMarker	 = m_csComboBox.clrDropDownMarkerDisabled;
	}
	else if (bDropped)
	{
		clrComboBoxBackground = m_csComboBox.clrBackDropped;
		if (bIsOnAuicAccessAboveRibbon)
		{
			clrComboBoxBorder	 = m_clrQuickAccessButtonPressedBack;
			clrDropDownBackground = m_clrQuickAccessButtonPressedBack;
			clrDropDownBorder	 = m_clrQuickAccessButtonPressedBack;
			clrDropDownMarker	 = m_clrQuickAccess;
		}
		else
		{
			clrComboBoxBorder	 = m_csComboBox.clrBorderDropped;
			clrDropDownBackground = m_csComboBox.clrDropDownBackDropped;
			clrDropDownBorder	 = m_csComboBox.clrDropDownBorderDropped;
			clrDropDownMarker	 = m_csComboBox.clrDropDownMarkerDropped;
		}
	}
	else if (bSelected)
	{
		clrComboBoxBackground = m_csComboBox.clrBackHighlight;
		if (bIsOnAuicAccessAboveRibbon)
		{
			clrComboBoxBorder	 = m_clrQuickAccessButtonHotBack;
			clrDropDownBackground = m_clrQuickAccessButtonHotBack;
			clrDropDownBorder	 = m_clrQuickAccessButtonHotBack;
			clrDropDownMarker	 = m_clrQuickAccess;
		}
		else
		{
			clrComboBoxBorder	 = m_csComboBox.clrBorderHighlight;
			clrDropDownBackground = m_csComboBox.clrDropDownBackHighlight;
			clrDropDownBorder	 = m_csComboBox.clrDropDownBorderHighlight;
			clrDropDownMarker	 = m_csComboBox.clrDropDownMarkerHighlight;
		}
	}

	// draw combo box background and border.
	CRect rcComboBox = pControlCombo->GetRect();
	rcComboBox.left += pControlCombo->GetLabelWidth();
	Rectangle(pDC, rcComboBox, clrComboBoxBorder, clrComboBoxBackground);

	// draw drop down background.
	CRect rcDropDown(rcComboBox.right - pControlCombo->GetThumbWidth(), rcComboBox.top + 1,
					 rcComboBox.right - 1, rcComboBox.bottom - 1);
	pDC->FillSolidRect(rcDropDown, clrDropDownBackground);

	if (bEnabled)
		CXTPDrawHelpers::DrawLine(pDC, rcDropDown.left, rcDropDown.top, rcDropDown.left,
								  rcDropDown.bottom, clrDropDownBorder);

	CXTPFontDC font(pDC, GetIconFont());
	CRect rcText(rcComboBox.left + 3, rcComboBox.top + 1, rcComboBox.right - rcDropDown.Width(),
				 rcComboBox.bottom - 1);
	pControlCombo->DrawEditText(pDC, rcText);

	rcDropDown.OffsetRect(1, 0);
	DrawComboExpandMark(pDC, rcDropDown, clrDropDownMarker);

	return szResult;
}

COLORREF CXTPCommandBarsOffice2013Theme::GetGlyphColor(CXTPControl* pControl)
{
	BOOL bEnabled  = pControl->GetEnabled();
	BOOL bSelected = pControl->GetSelected();
	BOOL bPopuped  = pControl->GetPopuped();
	BOOL bPressed  = pControl->GetPressed();

	if (pControl->IsOnQuickAccess())
	{
		CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControl->GetParent());
		if (NULL != pRibbon)
		{
			if (!pRibbon->IsQuickAccessBelowRibbon())
			{
				if (bEnabled)
					return m_clrQuickAccess;
				else
					return m_clrQuickAccessDisabled;
			}
		}
	}

	if ((pControl->GetFlags() & xtpFlagRibbonTabBackground) && !bPopuped && !bSelected)
	{
		return m_csRibbon.clrTabTextInactive;
	}

	XTPBarType barType = pControl->GetParent()->GetType();

	BOOL bHilite = (pControl->GetID() == XTP_ID_TOOLBAR_EXPAND
					|| pControl->GetID() == XTP_ID_CUSTOMIZE_ADDORREMOVE);

	if (!bEnabled)
		return m_clrGlyphDisabled;

	if (barType == xtpBarTypePopup)
		return bSelected ? m_clrGlyphHighlight : m_clrGlyph;

	if (IsKeyboardSelected(bPressed) || bPressed || bPopuped)
		return bHilite ? m_clrGlyphHighlight : m_clrGlyphPushed;

	if (bSelected)
		return m_clrGlyphHighlight;

	return m_clrGlyph;
}

COLORREF CXTPCommandBarsOffice2013Theme::GetSeparatorColor(CXTPControl* pControl, BOOL bDark)
{
	XTPBarType barType = pControl->GetParent()->GetType();

	if (barType == xtpBarTypeMenuBar || barType == xtpBarTypePopup)
	{
		CXTPColorSetStateToolbar* pColorSet = DYNAMIC_DOWNCAST(CXTPColorSetStateToolbar,
															   GetButtonColorSet(barType));
		if (pColorSet)
		{
			return bDark ? pColorSet->clrSeparator.clrDark : pColorSet->clrSeparator.clrLight;
		}
		CXTPColorSetStateMenu* pColorSetM = DYNAMIC_DOWNCAST(CXTPColorSetStateMenu,
															 GetButtonColorSet(barType));
		if (pColorSetM)
		{
			return bDark ? pColorSetM->clrSeparator.clrDark : pColorSetM->clrSeparator.clrLight;
		}
	}

	if (barType == xtpBarTypeNormal || barType == xtpBarTypeRibbon)
	{
		CXTPColorSetStateToolbar* pColorSet = DYNAMIC_DOWNCAST(CXTPColorSetStateToolbar,
															   GetButtonColorSet(xtpBarTypeNormal));
		if (pColorSet)
		{
			return bDark ? pColorSet->clrSeparator.clrDark : pColorSet->clrSeparator.clrLight;
		}
	}

	return COLORREF_ERROR;
}

void CXTPCommandBarsOffice2013Theme::DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pButton);

	CRect rcArrow = pButton->GetRect();
	rcArrow.left  = rcArrow.right
				   - ((pButton->GetType() == xtpControlButtonPopup
					   || pButton->GetType() == xtpControlSplitButtonPopup)
						  ? m_nSplitButtonPopupWidth
						  : rcArrow.Height());
	rcArrow.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));

	XTPPrimitiveSymbol nSymbol = CXTPDrawHelpers::IsContextRTL(pDC) ? xtpPrimitiveSymbolPlayLeft
																	: xtpPrimitiveSymbolPlayRight;
	XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rcArrow, GetGlyphColor(pButton));
}

void CXTPCommandBarsOffice2013Theme::DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt,
													   BOOL bSelected, BOOL bPopuped, BOOL bEnabled,
													   BOOL bVert)
{
	UNREFERENCED_PARAMETER(bEnabled);
	UNREFERENCED_PARAMETER(bPopuped);
	UNREFERENCED_PARAMETER(bSelected);

	CRect rcArrow(pt.x, pt.y, pt.x, pt.y);
	int nSize = XTP_COMMANDBAR_TRIANGLE_SIZE / 2;
	rcArrow.InflateRect(nSize, nSize, nSize, nSize);
	XTPPrimitiveSymbol symb = GetControlSymbol(pControl->GetParent()->GetPosition(), bVert);

	XTPPrimitiveDrawer()->DrawSymbol(pDC, symb, rcArrow, GetGlyphColor(pControl));
}

void CXTPCommandBarsOffice2013Theme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy,
														 BOOL bExpanded)
{
	UNREFERENCED_PARAMETER(bExpanded);
	pDC->FillSolidRect(x, y, cx, cy, m_csMenu.clrGripper);
}

CSize CXTPCommandBarsOffice2013Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
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

	COLORREF clrDark  = GetSeparatorColor(pControl, TRUE);
	COLORREF clrLight = GetSeparatorColor(pControl, FALSE);

	if (pBar->GetType() == xtpBarTypePopup)
	{
		if (!pControl->GetWrap())
		{
			HorizontalLine(pDC,
						   rcControl.left - XTP_DPI_X(1) + GetPopupBarGripperWidth(pBar)
							   + m_nPopupBarTextPadding,
						   rcControl.top - XTP_DPI_Y(2), rcControl.right + XTP_DPI_X(1), clrDark);
			HorizontalLine(pDC,
						   rcControl.left - XTP_DPI_X(1) + GetPopupBarGripperWidth(pBar)
							   + m_nPopupBarTextPadding,
						   rcControl.top - XTP_DPI_Y(1), rcControl.right + XTP_DPI_X(1), clrLight);
		}
		else
		{
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(2), rcRow.top - XTP_DPI_Y(1),
						 rcRow.bottom + XTP_DPI_Y(1), clrDark);
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(1), rcRow.top - XTP_DPI_Y(1),
						 rcRow.bottom + XTP_DPI_Y(1), clrLight);
		}
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
		{
			HorizontalLine(pDC, rcRow.left, rcControl.top - XTP_DPI_Y(4), rcRow.right, clrDark);
			HorizontalLine(pDC, rcRow.left, rcControl.top - XTP_DPI_Y(3), rcRow.right, clrLight);
		}
		else
		{
			VerticalLine(pDC, rcRow.right + XTP_DPI_X(2), rcRow.top + XTP_DPI_Y(4), rcRow.bottom,
						 clrDark);
			VerticalLine(pDC, rcRow.right + XTP_DPI_X(3), rcRow.top + XTP_DPI_Y(4), rcRow.bottom,
						 clrLight);
		}
	}
	else
	{
		if (!pControl->GetWrap())
		{
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(4), rcRow.top + XTP_DPI_Y(2), rcRow.bottom,
						 clrDark);
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(3), rcRow.top + XTP_DPI_Y(2), rcRow.bottom,
						 clrLight);
		}
		else
		{
			HorizontalLine(pDC, rcRow.left, rcRow.top - XTP_DPI_Y(4), rcRow.right, clrDark);
			HorizontalLine(pDC, rcRow.left, rcRow.top - XTP_DPI_Y(3), rcRow.right, clrLight);
		}
	}
	return 0;
}

void CXTPCommandBarsOffice2013Theme::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, CPoint pt0,
													 CPoint pt1, CPoint pt2, COLORREF /*clr*/)
{
	COLORREF clrGlyph = GetGlyphColor(pButton);
	HorizontalLine(pDC, pt2.x - XTP_DPI_X(2), pt0.y - XTP_DPI_Y(3), pt1.x + XTP_DPI_X(1), clrGlyph);
	Triangle(pDC, pt0, pt1, pt2, clrGlyph);
}

void CXTPCommandBarsOffice2013Theme::DrawHiddenGlyph(CDC* pDC, CXTPControl* pButton,
													 COLORREF /*clr*/, BOOL bVertical)
{
	COLORREF clrGlyph = GetGlyphColor(pButton);
	CXTPPaintManager::DrawHiddenGlyph(pDC, pButton, clrGlyph, bVertical);
}

void CXTPCommandBarsOffice2013Theme::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, COLORREF clr,
													 BOOL bVertical)
{
	CXTPCommandBarsModernUITheme::DrawExpandGlyph(pDC, pButton, clr, bVertical);
}

void CXTPCommandBarsOffice2013Theme::DrawCheckMark(CDC* pDC, CRect rcCheck, BOOL bIsRadio,
												   BOOL bEnabled, BOOL bSelected)
{
	COLORREF clr = COLORREF_NULL;

	if (!bEnabled)
	{
		clr = m_csCheckMark.clrCheckDisabled;
	}
	else if (bSelected)
	{
		clr = m_csCheckMark.clrCheckHighlight;
	}
	else
	{
		clr = m_csCheckMark.clrCheck;
	}

	if (bIsRadio)
	{
		DrawPopupBarRadioMark(pDC, rcCheck, bEnabled, clr);
	}
	else
	{
		DrawPopupBarCheckMark(pDC, rcCheck, bEnabled, clr);
	}
}

void CXTPCommandBarsOffice2013Theme::GetCheckMarkColors(COLORREF& clrMark, COLORREF& clrBack,
														COLORREF& clrBorder, BOOL bSelected,
														BOOL bPressed, BOOL bEnabled)
{
	if (!bEnabled)
	{
		clrMark   = m_csCheckBox.clrMarkDisabled;
		clrBack   = m_csCheckBox.clrBackDisabled;
		clrBorder = m_csCheckBox.clrBorderDisabled;
	}
	else if (bSelected && bPressed)
	{
		clrMark   = m_csCheckBox.clrMarkPushed;
		clrBack   = m_csCheckBox.clrBackPushed;
		clrBorder = m_csCheckBox.clrBorderPushed;
	}
	else if (bSelected || bPressed)
	{
		clrMark   = m_csCheckBox.clrMarkHighlight;
		clrBack   = m_csCheckBox.clrBackHighlight;
		clrBorder = m_csCheckBox.clrBorderHighlight;
	}
	else
	{
		clrMark   = m_csCheckBox.clrMark;
		clrBack   = m_csCheckBox.clrBack;
		clrBorder = m_csCheckBox.clrBorder;
	}
}
CSize CXTPCommandBarsOffice2013Theme::DrawControlCheckBoxMark(CDC* pDC, CXTPControl* pButton,
															  CRect rc, BOOL bDraw)
{
	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked();
	CRect rcButton(rc);
	if (m_bThemedCheckBox && m_themeButton->IsAppThemed())
	{
		return CXTPPaintManager::DrawControlCheckBoxMark(pDC, rc, bDraw, bSelected, bPressed,
														 bChecked, bEnabled);
	}
	if (!bDraw)
		return CSize(XTP_DPI_X(13), XTP_DPI_Y(13));
	BOOL bOnQuickAccessAboveRibbon = FALSE;
	if (pButton->IsOnQuickAccess())
	{
		CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pButton->GetParent());
		if (NULL != pRibbon)
		{
			if (!pRibbon->IsQuickAccessBelowRibbon())
				bOnQuickAccessAboveRibbon = TRUE;
		}
	}
	COLORREF clrMark, clrBack, clrBorder;
	GetCheckMarkColors(clrMark, clrBack, clrBorder, bSelected, bPressed, bEnabled);
	if ((bOnQuickAccessAboveRibbon)
		&& (m_themeCurrent != xtpThemeVisualStudio2015 || m_themeCurrent != xtpThemeVisualStudio2017
			|| m_themeCurrent != xtpThemeVisualStudio2019
			|| m_themeCurrent != xtpThemeVisualStudio2022))
	{
		if (bSelected && bPressed)
		{
			clrMark   = m_clrQuickAccess;
			clrBack   = m_clrQuickAccessButtonPressedBack;
			clrBorder = m_clrQuickAccessButtonPressedBack;
		}
		else if (bSelected || bPressed)
		{
			clrMark   = m_clrQuickAccess;
			clrBack   = m_clrQuickAccessButtonHotBack;
			clrBorder = m_clrQuickAccessButtonPressedBack;
		}
	}
	Rectangle(pDC, rc, clrBorder, clrBack);

	if (bChecked)
	{
		DrawCheckMark(pDC, rc, FALSE, bEnabled, bSelected);
	}

	return CSize(XTP_DPI_X(13), XTP_DPI_Y(13));
}

CSize CXTPCommandBarsOffice2013Theme::DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw,
															  BOOL bSelected, BOOL bPressed,
															  BOOL bChecked, BOOL bEnabled)
{
	if (m_bThemedCheckBox && m_themeButton->IsAppThemed())
	{
		return CXTPPaintManager::DrawControlCheckBoxMark(pDC, rc, bDraw, bSelected, bPressed,
														 bChecked, bEnabled);
	}

	if (bDraw)
	{
		COLORREF clrMark, clrBack, clrBorder;
		GetCheckMarkColors(clrMark, clrBack, clrBorder, bSelected, bPressed, bEnabled);

		Rectangle(pDC, rc, clrBorder, clrBack);

		if (bChecked)
		{
			CPoint pt = rc.CenterPoint();
			CXTPPenDC pen(pDC->GetSafeHdc(), clrMark);

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

	return CSize(XTP_DPI_X(13), XTP_DPI_Y(13));
}

CSize CXTPCommandBarsOffice2013Theme::DrawControlRadioButtonMark(CDC* pDC, CRect rc, BOOL bDraw,
																 BOOL bSelected, BOOL bPressed,
																 BOOL bChecked, BOOL bEnabled)
{
	if (m_bThemedCheckBox && m_themeButton->IsAppThemed())
	{
		return CXTPPaintManager::DrawControlRadioButtonMark(pDC, rc, bDraw, bSelected, bPressed,
															bChecked, bEnabled);
	}

	if (bDraw)
	{
		COLORREF clrMark, clrBack, clrBorder;
		GetCheckMarkColors(clrMark, clrBack, clrBorder, bSelected, bPressed, bEnabled);

		AlphaEllipse(pDC, rc, clrBorder, clrBack);

		if (bChecked)
		{
			AlphaEllipse(pDC,
						 CRect(rc.left + XTP_DPI_X(4), rc.top + XTP_DPI_Y(4),
							   rc.left + XTP_DPI_X(9), rc.top + XTP_DPI_Y(9)),
						 clrMark, clrMark);
		}
	}

	return CSize(XTP_DPI_X(13), XTP_DPI_Y(13));
}

void CXTPCommandBarsOffice2013Theme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton,
														  CRect rcButton)
{
	if (!pButton || !pDC || rcButton.IsRectEmpty())
		return;

	CXTPCommandBar* pCmdBar = pButton->GetParent();
	if (!pCmdBar)
		return;

	BOOL bEnabled  = pButton->GetEnabled();
	BOOL bSelected = pButton->GetSelected();
	BOOL bPopuped  = pButton->GetPopuped();
	BOOL bPressed  = pButton->GetPressed();
	BOOL bChecked  = pButton->GetChecked();

	BOOL bOnQuickAccessAboveRibbon = FALSE;
	if (pButton->IsOnQuickAccess())
	{
		CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pButton->GetParent());
		if (NULL != pRibbon)
		{
			if (!pRibbon->IsQuickAccessBelowRibbon())
				bOnQuickAccessAboveRibbon = TRUE;
		}
	}

	CRect rcPartA(rcButton);
	CRect rcPartB(rcButton);

	if (pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		int cy = GetSplitDropDownHeight();

		if (GetControlTextOrientation(pButton) != xtpBarTextVertical)
		{
			rcPartA.bottom = rcPartA.bottom - cy;
			rcPartB.top	= rcPartA.bottom;
		}
		else
		{
			rcPartA.right = rcPartA.right - cy;
			rcPartB.left  = rcPartA.right;
		}
	}
	else
	{
		if (bEnabled == TRUE_SPLITCOMMAND)
		{
			bEnabled = FALSE;
		}

		int cx		  = m_nSplitButtonDropDownWidth;
		rcPartA.right = rcPartA.right - cx;
		rcPartB.left  = rcPartA.right - 1;
	}

	CXTPColorSetStateButton* pColorSet = GetButtonColorSet(xtpBarTypeNormal);

	if (!bEnabled)
	{
		DrawRectangle(pDC, pButton);
	}
	else if (bPopuped)
	{
		if (bOnQuickAccessAboveRibbon)
			pDC->FillSolidRect(rcButton, m_clrQuickAccessButtonHotBack);
		else
			pDC->FillSolidRect(rcButton, pColorSet->clrBackChecked);
	}
	else if (bSelected)
	{
		CPoint pt;
		::GetCursorPos(&pt);
		pCmdBar->ScreenToClient(&pt);

		if (rcPartB.PtInRect(pt))
		{
			if (bOnQuickAccessAboveRibbon)
				pDC->FillSolidRect(rcButton, m_clrQuickAccessButtonHotBack);
			else
				pDC->FillSolidRect(rcPartB, pColorSet->clrBackHighlight);
			if (bChecked)
			{
				if (bOnQuickAccessAboveRibbon)
					pDC->FillSolidRect(rcButton, m_clrQuickAccessButtonHotBack);
				else
					pDC->FillSolidRect(rcButton, pColorSet->clrBackChecked);
			}
		}
		else
		{
			if (bOnQuickAccessAboveRibbon)
				pDC->FillSolidRect(rcPartA, bPressed ? m_clrQuickAccessButtonPressedBack
													 : m_clrQuickAccessButtonHotBack);
			else
				pDC->FillSolidRect(rcPartA, bPressed ? pColorSet->clrBackPushed
													 : pColorSet->clrBackHighlight);
			if (bChecked)
			{
				if (bOnQuickAccessAboveRibbon)
					pDC->FillSolidRect(rcButton, m_clrQuickAccessButtonHotBack);
				else
					pDC->FillSolidRect(rcButton, pColorSet->clrBackChecked);
			}
		}
		if (bOnQuickAccessAboveRibbon)
			pDC->Draw3dRect(rcButton, m_clrQuickAccessButtonHotBack, m_clrQuickAccessButtonHotBack);
		else
			pDC->Draw3dRect(rcButton, pColorSet->clrBorderHighlight, pColorSet->clrBorderHighlight);
	}
	else if (bChecked)
	{
		if (bOnQuickAccessAboveRibbon)
			Rectangle(pDC, rcButton, m_clrQuickAccessButtonHotBack,
					  (bChecked == 2) ? pColorSet->clrBack.operator COLORREF()
									  : m_clrQuickAccessButtonHotBack);
		else
			Rectangle(pDC, rcButton, pColorSet->clrBorderChecked,
					  (bChecked == 2) ? pColorSet->clrBack : pColorSet->clrBackChecked);
	}

	if (pButton->GetStyle() != xtpButtonIconAndCaptionBelow)
	{
		CPoint pt(rcButton.right - m_nSplitButtonDropDownWidth / 2, rcButton.CenterPoint().y);
		DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
	}
}

CXTPColorSetStateButton* CXTPCommandBarsOffice2013Theme::GetButtonColorSet(
	XTPBarType barType, BOOL bTextColor /*=FALSE*/)
{
	CXTPColorSetStateButton* pColorSet = NULL;

	switch (barType)
	{
		case xtpBarTypePopup: // CXTPPopupBar
			pColorSet = &m_csMenu;
			break;

		case xtpBarTypeRibbon: // CXTPRibbonBar
			pColorSet = &m_csRibbon;
			break;

		case xtpBarTypeNormal: // CXTPToolBar
			pColorSet = &m_csToolbar;
			break;

		case xtpBarTypeMenuBar: // CXTPMenuBar
			pColorSet = bTextColor ? (CXTPColorSetStateButton*)&m_csMenu
								   : (CXTPColorSetStateButton*)&m_csToolbar;
			break;
	}

	return pColorSet;
}

COLORREF CXTPCommandBarsOffice2013Theme::GetControlTextColor(CXTPControl* pButton)
{
	if (pButton->IsOnQuickAccess())
	{
		CXTPRibbonBar* pRibbon = DYNAMIC_DOWNCAST(CXTPRibbonBar, pButton->GetParent());
		if (NULL != pRibbon)
		{
			if (!pRibbon->IsQuickAccessBelowRibbon())
			{
				if (pButton->GetEnabled())
					return m_clrQuickAccess;
				else
					return m_clrQuickAccessDisabled;
			}
		}
	}
	BOOL isOnTabBar = (pButton->GetFlags() & xtpFlagRibbonTabBackground)
					  && (!pButton->GetSelected());
	if (!isOnTabBar || pButton->GetPopuped())
		return CXTPPaintManager::GetControlTextColor(pButton);
	return m_csRibbon.clrTabTextInactive;
}

COLORREF CXTPCommandBarsOffice2013Theme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed,
															   BOOL bEnabled, BOOL bChecked,
															   BOOL bPopuped, XTPBarType barType,
															   XTPBarPosition barPosition)
{
	// CXTPPopupBar.
	if (barType == xtpBarTypePopup)
	{
		return !bEnabled ? m_csMenu.clrTextDisabled
						 : (bSelected ? m_csMenu.clrTextHighlight : m_csMenu.clrText);
	}

	CXTPColorSetStateButton* pColorSet = GetButtonColorSet(barType, TRUE);

	if (!bEnabled)
		return pColorSet->clrTextDisabled;

	if (bPopuped)
	{
		return pColorSet->clrTextPushed;
	}

	if (bChecked && !bSelected && !bPressed)
		return pColorSet->clrTextChecked;

	if (IsKeyboardSelected(bPressed) || (bChecked && bPressed))
		return pColorSet->clrTextPushed;

	if (bSelected || bPressed)
		return pColorSet->clrTextHighlight;

	return (barPosition == xtpBarPopup) ? m_csMenu.clrText : m_csToolbar.clrText;
}

void CXTPCommandBarsOffice2013Theme::DrawControlMDIButton(CDC* pDC, CXTPControlButton* pButton)
{
	BOOL isOnTabBar = (pButton->GetFlags() & xtpFlagRibbonTabBackground)
					  && (!pButton->GetSelected());
	CXTPDrawHelpers::DrawFontText(pDC, _T("Marlett"), 80, pButton->GetParameter(),
								  pButton->GetParameter().GetLength(), pButton->GetRect(),
								  DT_SINGLELINE | DT_CENTER | DT_VCENTER,
								  isOnTabBar ? m_csRibbon.clrTabTextInactive : m_csMenu.clrText);
}

BOOL CXTPCommandBarsOffice2013Theme::SetOfficeBackImage(CXTPCommandBars* pCommandBars,
														XTPBackImage nBackImage /*=XTPNone*/)
{
	CXTPRibbonOffice2013Theme* pRibbonTheme = DYNAMIC_DOWNCAST(CXTPRibbonOffice2013Theme,
															   m_pRibbonPaintManager);

	if (pRibbonTheme && pCommandBars)
	{
		pRibbonTheme->SetBackImage(nBackImage);
		pCommandBars->RedrawCommandBars();
		return TRUE;
	}

	return FALSE;
}

BOOL AFX_CDECL XTPSetOfficeSetBackImage(CXTPCommandBars* pCommandBars,
										XTPBackImage nBackImage /*=XTPNone*/)
{
	if (!pCommandBars)
		return FALSE;

	CXTPCommandBarsOffice2013Theme* pPaintManager =
		DYNAMIC_DOWNCAST(CXTPCommandBarsOffice2013Theme, pCommandBars->GetPaintManager());

	if (!pPaintManager)
		return FALSE;

	return pPaintManager->SetOfficeBackImage(pCommandBars, nBackImage);
}

void CXTPCommandBarsOffice2013Theme::DrawFrameBorder(CDC* pDC, CRect rc, int szBorderThickness,
													 BOOL bSelected, BOOL bPressed, BOOL bEnabled,
													 BOOL bChecked, BOOL bPopuped,
													 XTPBarType barType, XTPBarPosition barPosition)
{
	UNREFERENCED_PARAMETER(barPosition);

	CXTPColorSetStateButton* pColorSet = GetButtonColorSet(barType);

	if (!bEnabled)
	{
		if (bChecked)
		{
			if (!IsKeyboardSelected(bSelected))
				Draw3dRect(pDC, rc, szBorderThickness, pColorSet->clrBackDisabled,
						   pColorSet->clrBackDisabled);
		}
		else if (IsKeyboardSelected(bSelected))
		{
			CRect rcFill(rc);
			rcFill.DeflateRect(2, 2);

			pDC->Draw3dRect(rc, pColorSet->clrBorderDisabled, pColorSet->clrBorderDisabled);
			pDC->FillSolidRect(rcFill, pColorSet->clrBackDisabled);
		}
	}

	else if (bPopuped)
	{
		if (barType == xtpBarTypeMenuBar)
		{
			Draw3dRect(pDC, rc, szBorderThickness, m_csMenu.clrMenuBorder, m_csMenu.clrMenuBack);
		}
		else
		{
			Draw3dRect(pDC, rc, szBorderThickness, pColorSet->clrBorderChecked,
					   pColorSet->clrBackChecked);
		}
	}

	else
	{
		if (bChecked && !bSelected && !bPressed)
		{
			Draw3dRect(pDC, rc, szBorderThickness, pColorSet->clrBorderChecked,
					   (bChecked == 2) ? pColorSet->clrBack : pColorSet->clrBackChecked);
		}
		else if (bChecked && bSelected && !bPressed)
		{
			Draw3dRect(pDC, rc, szBorderThickness, pColorSet->clrBorderCheckedHighlight,
					   pColorSet->clrBackCheckedHighlight);
		}
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed))
		{
			Draw3dRect(pDC, rc, szBorderThickness, pColorSet->clrBorderPushed,
					   pColorSet->clrBackPushed);
		}
		else if (bSelected || bPressed)
		{
			Draw3dRect(pDC, rc, szBorderThickness, pColorSet->clrBorderHighlight,
					   pColorSet->clrBackHighlight);
		}
	}
}

void CXTPCommandBarsOffice2013Theme::DrawGalleryItemBack(CDC* pDC, CRect rcItem, BOOL bEnabled,
														 BOOL bSelected, BOOL bPressed,
														 BOOL bChecked, XTPBarType barType)
{
	int borderThickness = XTP_DPI_X(4);
	DrawFrameBorder(pDC, rcItem, borderThickness, bSelected, bPressed, bEnabled, bChecked, FALSE,
					barType, xtpBarPopup);
}
