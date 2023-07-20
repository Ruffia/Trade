// XTPCommandBarsVisualStudio2019Theme.cpp : implementation of the
// CXTPCommandBarsVisualStudio2019Theme class.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPTheme.h"

// Scrollbar
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/Themes/XTPScrollBarThemeVisualStudio2012.h"
#include "Common/ScrollBar/Themes/XTPScrollBarThemeVisualStudio2019.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2013Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2019Theme.h"
#include "CommandBars/Gallery/XTPControlGalleryOffice2013Theme.h"
#include "CommandBars/Gallery/XTPControlGalleryVisualStudio2019Theme.h"
#include "CommandBars/Resource.h"

#ifdef _XTP_INCLUDE_RIBBON
#	include "Markup/Extensions/XTPMarkupStaticExtension.h"
#	include "Ribbon/XTPRibbonPaintManager.h"
#	include "Ribbon/XTPRibbonMarkupTheme.h"
#	include "Ribbon/Themes/XTPRibbonThemeOffice2013.h"
#	include "Ribbon/Themes/XTPRibbonThemeVisualStudio2019.h"
#endif

// Progress
#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/Progress/XTPProgressThemeOffice2013.h"
#include "CommandBars/Progress/XTPProgressThemeVisualStudio2019.h"

// Slider
#include "CommandBars/Slider/XTPSliderPaintManager.h"
#include "CommandBars/Slider/XTPSliderThemeOffice2013.h"
#include "CommandBars/Slider/XTPSliderThemeVisualStudio2019.h"

// Frame
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeOffice2013.h"
#include "CommandBars/Frame/XTPFrameThemeVisualStudio2019.h"

// Message bar
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2013.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeVisualStudio2019.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOffice2013.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2019.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _XTP_INCLUDE_RIBBON
#endif

//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsVisualStudio2019Theme

IMPLEMENT_DYNAMIC(CXTPCommandBarsVisualStudio2019Theme, CXTPCommandBarsOffice2013Theme)

CXTPCommandBarsVisualStudio2019Theme::CXTPCommandBarsVisualStudio2019Theme()
{
	// Frame paint manager
	SAFE_DELETE(m_pFramePaintManager);
	m_pFramePaintManager = new CXTPFrameThemeVisualStudio2019(this);

	// Progress paint manager
	SAFE_DELETE(m_pProgressPaintManager);
	m_pProgressPaintManager = new CXTPProgressThemeVisualStudio2019(this);

	// Slider paint manager
	SAFE_DELETE(m_pSliderPaintManager);
	m_pSliderPaintManager = new CXTPSliderThemeVisualStudio2019(this);

	// Message bar paint manager
	SAFE_DELETE(m_pMessageBarPaintManager);
	m_pMessageBarPaintManager = new CXTPMessageBarThemeVisualStudio2019(this);

	// Status bar paint manager
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeVisualStudio2019(this);

	// Ribbon paint manager
	SAFE_DELETE(m_pRibbonPaintManager);
	m_pRibbonPaintManager = new CXTPRibbonThemeVisualStudio2019(this);

	// Gallery paint manager
	SAFE_DELETE(m_pGalleryPaintManager);
	m_pGalleryPaintManager = new CXTPControlGalleryVisualStudio2019Theme(this);

	// Scrollbar paint manager
	SAFE_DELETE(m_pScrollBarPaintManager);
	m_pScrollBarPaintManager = new CXTPScrollBarThemeVisualStudio2019();

	m_strOfficeFont = _T("Segoe UI");
	UpdateFonts();
}

CXTPCommandBarsVisualStudio2019Theme::~CXTPCommandBarsVisualStudio2019Theme()
{
}

void CXTPCommandBarsVisualStudio2019Theme::SetStateColor(COLORREF clrState /*=COLORREF_NULL*/)
{
	CXTPFrameThemeVisualStudio2019* pFramePaintManager =
		static_cast<CXTPFrameThemeVisualStudio2019*>(m_pFramePaintManager);
	pFramePaintManager->SetStateColor(clrState);

	CXTPStatusBarThemeVisualStudio2019* pStatusBarPaintManager =
		static_cast<CXTPStatusBarThemeVisualStudio2019*>(m_pStatusBarPaintManager);
	pStatusBarPaintManager->SetStateColor(clrState);
}

COLORREF CXTPCommandBarsVisualStudio2019Theme::GetStateColor() const
{
	const CXTPFrameThemeVisualStudio2019* pFramePaintManager =
		static_cast<const CXTPFrameThemeVisualStudio2019*>(m_pFramePaintManager);
	COLORREF clr1 = pFramePaintManager->GetStateColor();

	const CXTPStatusBarThemeVisualStudio2019* pStatusBarPaintManager =
		static_cast<const CXTPStatusBarThemeVisualStudio2019*>(m_pStatusBarPaintManager);
	COLORREF clr2 = pStatusBarPaintManager->GetStateColor();

	return clr1 == clr2 ? clr1 : COLORREF_NULL;
}

void CXTPCommandBarsVisualStudio2019Theme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton,
																CRect rcButton)
{
	ASSERT_VALID(pButton);

	if (pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		CXTPCommandBarsModernUITheme::DrawSplitButtonFrame(pDC, pButton, rcButton);
	}
	else
	{
		BOOL bEnabled  = pButton->GetEnabled();
		BOOL bSelected = pButton->GetSelected();
		BOOL bPopuped  = pButton->GetPopuped();
		BOOL bPressed  = pButton->GetPressed();
		BOOL bChecked  = pButton->GetChecked();

		if (bEnabled == TRUE_SPLITCOMMAND)
		{
			bEnabled = FALSE;
		}

		DrawRectangle(pDC, rcButton, bSelected, bPressed, bEnabled, bChecked, bPopuped,
					  pButton->GetParent()->GetType(), pButton->GetParent()->GetPosition());

		if (bEnabled && bSelected && !bPopuped && !bPressed && bChecked)
		{
			COLORREF clrLine = m_csToolbar.clrBorderHighlight;

			if (m_csToolbar.clrBackHighlight == m_csToolbar.clrBorderHighlight)
				clrLine = m_csToolbar.clrBack;

			if (bChecked)
				clrLine = m_csToolbar.clrBorderCheckedHighlight;

			VerticalLine(pDC, rcButton.right - m_nSplitButtonDropDownWidth - 1, rcButton.top,
						 rcButton.bottom, clrLine);
		}

		CPoint pt = CPoint(rcButton.right - m_nSplitButtonDropDownWidth / 2,
						   rcButton.CenterPoint().y);
		DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
	}
}

void CXTPCommandBarsVisualStudio2019Theme::DrawCheckRect(CDC* pDC, CRect rc, COLORREF clrFore,
														 COLORREF clrBack)
{
	ASSERT_VALID(pDC);

	rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
	pDC->Draw3dRect(rc, clrFore, clrFore);

	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, clrFore, clrFore);

	rc.DeflateRect(1, 1);
	pDC->FillSolidRect(rc, clrBack);
}

void CXTPCommandBarsVisualStudio2019Theme::DrawGalleryItemBack(CDC* pDC, CRect rcItem,
															   BOOL bEnabled, BOOL bSelected,
															   BOOL bPressed, BOOL bChecked,
															   XTPBarType barType)
{
	if (barType == xtpBarTypePopup)
	{
		if (!bEnabled) // disabled
		{
			Rectangle(pDC, rcItem, m_csComboBox.clrBackDisabled, m_csComboBox.clrBackDisabled);
			return;
		}
		else if (!bSelected && !bPressed) // normal state
		{
			Rectangle(pDC, rcItem, m_csComboBox.clrBack, m_csComboBox.clrBack);
			return;
		}
		else if (bSelected && !bPressed) // hover
		{
			Rectangle(pDC, rcItem, m_csToolbar.clrBorderHighlight, m_csToolbar.clrBackHighlight);
			return;
		}
		else if (bSelected && bPressed) // pressed
		{
			Rectangle(pDC, rcItem, m_csToolbar.clrBorderPushed, m_csToolbar.clrBackPushed);
			return;
		}
	}

	DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, barType,
				  xtpBarPopup);
}

COLORREF CXTPCommandBarsVisualStudio2019Theme::GetControlTextColor(CXTPControl* pButton)
{
	ASSERT_VALID(pButton);

	int nID = pButton->GetID();

	if (nID == XTP_ID_TOOLBAR_EXPAND || nID == XTP_ID_CUSTOMIZE_ADDORREMOVE)
	{
		if (pButton->GetPopuped() || pButton->GetSelected())
		{
			return m_csToolbar.clrTextHighlight;
		}
	}
	else if (nID == XTP_ID_TOOLBAR_HIDE)
	{
		if (pButton->GetPressed())
		{
			return m_csToolbar.clrTextPushed;
		}
	}

	return CXTPCommandBarsOffice2013Theme::GetControlTextColor(pButton);
}

void CXTPCommandBarsVisualStudio2019Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
														 BOOL bPressed, BOOL bEnabled,
														 BOOL bChecked, BOOL bPopuped,
														 XTPBarType barType,
														 XTPBarPosition barPosition, int nID)
{
	// for customize add / remove buttons draw special check box.

	if (bChecked && nID == 0)
	{
		rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

		if (!bEnabled) // disabled
		{
			DrawCheckRect(pDC, rc, m_csCheckMark.clrCheckDisabled, m_csCheckMark.clrBackDisabled);
			return;
		}
		else if (!bSelected && !bPressed) // normal state
		{
			DrawCheckRect(pDC, rc, m_csCheckMark.clrCheck, m_csCheckMark.clrBack);
			return;
		}
		else if (bSelected && !bPressed) // hover
		{
			DrawCheckRect(pDC, rc, m_csCheckMark.clrCheckHighlight, m_csCheckMark.clrBackHighlight);
			return;
		}
		else if (bSelected && bPressed) // pressed
		{
			DrawCheckRect(pDC, rc, m_csCheckMark.clrCheckPushed, m_csCheckMark.clrBackPushed);
			return;
		}
	}
	else
	{
		DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType,
					  barPosition);
	}
}

void CXTPCommandBarsVisualStudio2019Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
														 BOOL bPressed, BOOL bEnabled,
														 BOOL bChecked, BOOL bPopuped,
														 XTPBarType barType,
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
			Rectangle(pDC, rc, pColorSet->clrBorderPushed, pColorSet->clrBackPushed);
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

BOOL CXTPCommandBarsVisualStudio2019Theme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	return CXTPCommandBarsOffice2013Theme::DrawRectangle(pDC, pButton);
}

BOOL AFX_CDECL XTPSetVS2019StateColor(CXTPCommandBars* pCommandBars,
									  COLORREF clrState /*=COLORREF_NULL*/)
{
	if (!pCommandBars)
		return FALSE;

	CXTPCommandBarsVisualStudio2019Theme* pPaintManager =
		DYNAMIC_DOWNCAST(CXTPCommandBarsVisualStudio2019Theme, pCommandBars->GetPaintManager());

	if (!pPaintManager)
		return FALSE;

	pPaintManager->SetStateColor(clrState);
	pCommandBars->RedrawCommandBars();

	return TRUE;
}

CSize CXTPCommandBarsVisualStudio2019Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
																  BOOL bDraw)
{
	CSize szGripper(0, 0);

	CRect rcBar;
	pBar->GetClientRect(&rcBar);

	if (xtpBarTop == pBar->GetPosition() || xtpBarBottom == pBar->GetPosition())
	{
		szGripper = XTP_DPI(CSize(20, 0));

		if (bDraw)
		{
			CPoint ptDash(rcBar.left + XTP_DPI_X(10), rcBar.top + XTP_DPI_Y(5));
			int nDashZ = rcBar.bottom - XTP_DPI_X(3);
			DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER),
						   TRUE);

			ptDash.x += XTP_DPI_X(2);
			ptDash.y += XTP_DPI_Y(2);
			nDashZ -= XTP_DPI_X(2);
			DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER),
						   TRUE);

			ptDash.x += XTP_DPI_X(2);
			ptDash.y -= XTP_DPI_Y(2);
			nDashZ += XTP_DPI_X(2);
			DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER),
						   TRUE);
		}
	}
	else
	{
		szGripper = CXTPCommandBarsModernUITheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}

	return szGripper;
}

CSize CXTPCommandBarsVisualStudio2019Theme::DrawControlComboBox(CDC* pDC,
																CXTPControlComboBox* pControlCombo,
																BOOL bDraw)
{
	// call base class to draw label.
	CSize szResult = CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw); // Draw label

	if (!bDraw)
	{
		return szResult;
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
		clrComboBoxBorder	 = m_csComboBox.clrBorderDropped;
		clrDropDownBackground = m_csComboBox.clrDropDownBackDropped;
		clrDropDownBorder	 = m_csComboBox.clrDropDownBorderDropped;
		clrDropDownMarker	 = m_csComboBox.clrDropDownMarkerDropped;
	}
	else if (bSelected)
	{
		clrComboBoxBackground = m_csComboBox.clrBackHighlight;
		clrComboBoxBorder	 = m_csComboBox.clrBorderHighlight;
		clrDropDownBackground = m_csComboBox.clrDropDownBackHighlight;
		clrDropDownBorder	 = m_csComboBox.clrDropDownBorderHighlight;
		clrDropDownMarker	 = m_csComboBox.clrDropDownMarkerHighlight;
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
