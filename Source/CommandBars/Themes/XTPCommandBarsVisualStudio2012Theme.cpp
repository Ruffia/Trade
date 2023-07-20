// XTPCommandBarsVisualStudio2012Theme.cpp : implementation of the
// CXTPCommandBarsVisualStudio2012Theme class.
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
#include "Common/XTPResource.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPTheme.h"

// Scrollbar
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/Themes/XTPScrollBarThemeVisualStudio2012.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPDockBar.h"
#include "CommandBars/Resource.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2012Theme.h"

#include "CommandBars/XTPCommandBarsColorSet.h"
#include "CommandBars/ColorSets/XTPCommandBarsColorSetVisualStudio2012.h"

// Frame
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeVisualStudio2012.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Base CXTPCommandBarsModernUITheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsVisualStudio2012Theme, Base)

CXTPCommandBarsVisualStudio2012Theme::CXTPCommandBarsVisualStudio2012Theme(XTPPaintTheme paintTheme)
	: Base()
	, m_pColorSet(NULL)
{
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeVisualStudio2012(this);

	SAFE_DELETE(m_pFramePaintManager);

	// Frame and color set theme selection.
	ASSERT(xtpThemeVisualStudio2012Light == paintTheme
		   || xtpThemeVisualStudio2012Dark == paintTheme);

	if (paintTheme == xtpThemeVisualStudio2012Dark)
	{
		SAFE_DELETE(m_pFramePaintManager);
		m_pFramePaintManager = new CXTPFrameThemeVisualStudio2012Dark(this);

		// Scrollbar paint manager
		SAFE_DELETE(m_pScrollBarPaintManager);
		m_pScrollBarPaintManager = new CXTPScrollBarThemeVisualStudio2012();

		m_pColorSet = new CXTPCommandBarsColorSetVisualStudio2012Dark();
	}
	else
	{
		SAFE_DELETE(m_pFramePaintManager);
		m_pFramePaintManager = new CXTPFrameThemeVisualStudio2012Light(this);

		// Scrollbar paint manager
		SAFE_DELETE(m_pScrollBarPaintManager);
		m_pScrollBarPaintManager = new CXTPScrollBarThemeVisualStudio2012();

		m_pColorSet = new CXTPCommandBarsColorSetVisualStudio2012Light();
	}

	ASSERT(NULL != m_pFramePaintManager);
	ASSERT(NULL != m_pColorSet);

	HMODULE hModule = XTPResourceImages()->GetHandle();
	if (NULL == hModule)
	{
		hModule = XTPGetInstanceHandle();
	}

	CXTPResource resource;
	if (resource.LoadResource(hModule, _T("XTP_IDR_VS2012_STYLE"), _T("STYLE")))
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);

		if (hZip)
		{
			static_cast<CXTPFrameThemeVisualStudio2012*>(m_pFramePaintManager)
				->LoadParts(hZip, m_pMarkupContext);
			static_cast<CXTPFrameThemeVisualStudio2012*>(m_pFramePaintManager)
				->RefreshMetrics(XTPResourceImages());

			CloseZip(hZip);
		}
	}

	m_bThickCheckMark = TRUE;
}

CXTPCommandBarsVisualStudio2012Theme::~CXTPCommandBarsVisualStudio2012Theme()
{
	SAFE_DELETE(m_pColorSet);
}

void CXTPCommandBarsVisualStudio2012Theme::RefreshMetrics()
{
	Base::RefreshMetrics();
	m_pFramePaintManager->RefreshMetrics();

	m_pColorSet->RefreshColors(NULL);

	m_arrColor[COLOR_BTNFACE]			= m_pColorSet->m_clrToolBarBackground;
	m_arrColor[XPCOLOR_TOOLBAR_FACE]	= m_pColorSet->m_clrToolBarBackground;
	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER] = m_pColorSet->m_clrToolBarGripper;
	m_arrColor[XPCOLOR_TOOLBAR_TEXT]	= m_pColorSet->m_clrToolBarText;

	m_arrColor[XPCOLOR_MENUBAR_TEXT]   = m_pColorSet->m_clrMenuText;
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT] = m_pColorSet->m_clrMenuTextHighlighted;
	m_arrColor[XPCOLOR_MENUBAR_BORDER] = m_pColorSet->m_clrMenuBorder;
	m_arrColor[XPCOLOR_MENUBAR_FACE]   = m_pColorSet->m_clrMenuBackground;

	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER] = m_pColorSet->m_clrButtonBorderHighlighted;
	m_arrColor[XPCOLOR_HIGHLIGHT]		 = m_pColorSet->m_clrButtonBackgroundHighlighted;

	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] = m_pColorSet->m_clrButtonBorderPushed;
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]		= m_pColorSet->m_clrButtonBackgroundPushed;

	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = m_pColorSet->m_clrButtonBorderChecked;
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED]		 = m_pColorSet->m_clrButtonBackgroundChecked;

	m_arrColor[XPCOLOR_EDITCTRLBORDER] = m_pColorSet->m_csComboBoxNormal.m_clrComboBoxBorder;
	m_arrColor[COLOR_WINDOW]		   = m_pColorSet->m_csComboBoxNormal.m_clrComboBoxBackground;

	m_arrColor[COLOR_APPWORKSPACE] = m_pColorSet->m_clrAppWorkspace;

	m_clrEditTextNormal		 = m_pColorSet->m_clrControlText;
	m_clrEditTextDisabled	= m_pColorSet->m_clrControlTextDisabled;
	m_clrEditTextHighlighted = m_pColorSet->m_clrControlTextHighlighted;

	// for (int i=0; i<XPCOLOR_LAST; i++)
	//{
	//  //m_arrColor[i] = RGB(255,0,0);
	//}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawDashedLine(CDC* pDC, int x, int y1, int y2,
														  COLORREF clr)
{
	CRect rcDot(x, y1, x + XTP_DPI_X(1), y1 + XTP_DPI_Y(1));
	int nStep = XTP_DPI_Y(4);
	while (y2 > rcDot.top)
	{
		pDC->FillSolidRect(rcDot, clr);
		rcDot.top += nStep;
		rcDot.bottom += nStep;
	}
}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
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
			DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ,
						   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			ptDash.x += XTP_DPI_X(2);
			ptDash.y += XTP_DPI_Y(2);
			nDashZ -= XTP_DPI_X(2);
			DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ,
						   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			ptDash.x += XTP_DPI_X(2);
			ptDash.y -= XTP_DPI_Y(2);
			nDashZ += XTP_DPI_X(2);
			DrawDashedLine(pDC, ptDash.x, ptDash.y, nDashZ,
						   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	else
	{
		szGripper = Base::DrawCommandBarGripper(pDC, pBar, bDraw);
	}

	return szGripper;
}

COLORREF CXTPCommandBarsVisualStudio2012Theme::GetControlEditBackColor(CXTPControl* pControl)
{
	COLORREF clrEditColor = CXTPCommandBarsModernUITheme::GetControlEditBackColor(pControl);

	CXTPControlComboBox* pControlCombo = DYNAMIC_DOWNCAST(CXTPControlComboBox, pControl);

	if (pControlCombo)
	{
		BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
		BOOL bSelected = pControlCombo->GetSelected();
		BOOL bEnabled  = pControlCombo->GetEnabled();

		CXTPCommandBarsColorSetComboBox* pColorSet = NULL;

		if (NULL != m_pColorSet)
		{
			if (!bEnabled)
				pColorSet = &m_pColorSet->m_csComboBoxDisabled;
			else if (bDropped)
				pColorSet = &m_pColorSet->m_csComboBoxDropped;
			else if (bSelected)
				pColorSet = &m_pColorSet->m_csComboBoxHighlighted;
			else
				pColorSet = &m_pColorSet->m_csComboBoxNormal;

			clrEditColor = pColorSet->m_clrComboBoxBackground;
		}
	}

	return clrEditColor;
}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawControlComboBox(CDC* pDC,
																CXTPControlComboBox* pControlCombo,
																BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);
	}

	BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
	BOOL bSelected = pControlCombo->GetSelected();
	BOOL bEnabled  = pControlCombo->GetEnabled();

	CXTPCommandBarsColorSetComboBox* pColorSet = NULL;

	if (NULL != m_pColorSet)
	{
		if (!bEnabled)
			pColorSet = &m_pColorSet->m_csComboBoxDisabled;
		else if (bDropped)
			pColorSet = &m_pColorSet->m_csComboBoxDropped;
		else if (bSelected)
			pColorSet = &m_pColorSet->m_csComboBoxHighlighted;
		else
			pColorSet = &m_pColorSet->m_csComboBoxNormal;
	}

	int nThumb = pControlCombo->GetThumbWidth();

	CRect rcComboBox = pControlCombo->GetRect();
	rcComboBox.left += pControlCombo->GetLabelWidth();

	CRect rcDropDown(rcComboBox.right - nThumb, rcComboBox.top + XTP_DPI_Y(1),
					 rcComboBox.right - XTP_DPI_X(1), rcComboBox.bottom - XTP_DPI_Y(1));
	CRect rcText(rcComboBox.left + XTP_DPI_X(3), rcComboBox.top + XTP_DPI_Y(1),
				 rcComboBox.right - rcDropDown.Width(), rcComboBox.bottom - XTP_DPI_Y(1));

	CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw); // Draw label

	if (NULL != pColorSet)
	{
		pDC->FillSolidRect(rcComboBox, pColorSet->m_clrComboBoxBackground);
		pDC->Draw3dRect(rcComboBox, pColorSet->m_clrComboBoxBorder, pColorSet->m_clrComboBoxBorder);

		// Drop-down
		pDC->FillSolidRect(rcDropDown, pColorSet->m_dropDown.m_clrDropDownBackground);

		if (bEnabled)
			CXTPDrawHelpers::DrawLine(pDC, rcDropDown.left, rcDropDown.top, rcDropDown.left,
									  rcDropDown.bottom, pColorSet->m_dropDown.m_clrDropDownBorder);

		DrawComboExpandMark(pDC, rcDropDown, pColorSet->m_dropDown.m_clrDropDownMarker);

		rcDropDown.DeflateRect(1, 1);
	}

	CXTPFontDC font(pDC, GetIconFont());
	pControlCombo->DrawEditText(pDC, rcText);

	return 0;
}

void CXTPCommandBarsVisualStudio2012Theme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
}

void CXTPCommandBarsVisualStudio2012Theme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (xtpBarPopup == pBar->GetPosition())
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);

		CXTPCommandBar* pParentBar = pBar->GetParentCommandBar();
		if (pParentBar && xtpBarPopup != pParentBar->GetPosition())
		{
			FillIntersectRect(pDC, (CXTPPopupBar*)pBar, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		}
	}
	else
	{
		Base::FillCommandBarEntry(pDC, pBar);
	}
}

COLORREF CXTPCommandBarsVisualStudio2012Theme::GetGlyphColor(CXTPControl* pControl)
{
	COLORREF clr = COLORREF_NULL;

	if (!pControl->GetEnabled())
	{
		clr = m_pColorSet->m_clrCheckMarkDisabled;
	}
	else if (pControl->GetSelected() && !pControl->GetPressed())
	{
		clr = m_pColorSet->m_clrGlyphHighlighted;
	}
	else if (pControl->GetSelected() && pControl->GetPressed())
	{
		clr = (pControl->GetType() == xtpControlSplitButtonPopup)
				  ? m_pColorSet->m_clrGlyphHighlighted
				  : m_pColorSet->m_clrGlyphPushed;
	}
	else
	{
		clr = m_pColorSet->m_clrGlyph;
	}

	return clr;
}

void CXTPCommandBarsVisualStudio2012Theme::DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton)
{
	ASSERT_VALID(pButton);

	CRect rc = pButton->GetRect();
	rc.left  = rc.right
			  - (pButton->GetType() == xtpControlSplitButtonPopup ? m_nSplitButtonPopupWidth
																  : rc.Height());
	rc.DeflateRect(XTP_DPI_X(5), XTP_DPI_Y(5), XTP_DPI_X(5), XTP_DPI_Y(5));

	XTPPrimitiveSymbol nSymbol = CXTPDrawHelpers::IsContextRTL(pDC) ? xtpPrimitiveSymbolPlayLeft
																	: xtpPrimitiveSymbolPlayRight;
	XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rc, GetGlyphColor(pButton));
}

void AFX_CDECL CXTPCommandBarsVisualStudio2012Theme::FillRect(CDC* pDC, CRect rc, BOOL bCheckMark,
															  COLORREF clrFore, COLORREF clrBack)
{
	if (bCheckMark)
	{
		rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		pDC->Draw3dRect(rc, clrFore, clrFore);

		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, clrFore, clrFore);

		rc.DeflateRect(1, 1);
		pDC->FillSolidRect(rc, clrBack);
	}
	else
	{
		pDC->Draw3dRect(rc, clrFore, clrFore);
		rc.DeflateRect(1, 1);
		pDC->FillSolidRect(rc, clrBack);
	}
}

BOOL CXTPCommandBarsVisualStudio2012Theme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	if (pButton->GetID() == XTP_ID_CUSTOMIZE_ADDORREMOVE)
	{
		FillRect(pDC, pButton->GetRect(), FALSE, m_pColorSet->m_clrButtonBorderChecked,
				 m_pColorSet->m_clrButtonBackgroundHighlighted);

		return TRUE;
	}

	if (pButton->GetPopuped())
	{
		if (pButton->GetID() == XTP_ID_TOOLBAR_EXPAND)
		{
			FillRect(pDC, pButton->GetRect(), FALSE, m_pColorSet->m_clrButtonBorderPopped,
					 m_pColorSet->m_clrButtonBackgroundPopped);

			return TRUE;
		}
	}

	return FALSE;
}

void CXTPCommandBarsVisualStudio2012Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
														 BOOL bPressed, BOOL bEnabled,
														 BOOL bChecked, BOOL bPopuped,
														 XTPBarType barType,
														 XTPBarPosition barPosition)
{
	if (bPopuped)
	{
		FillRect(pDC, rc, FALSE, m_pColorSet->m_clrMenuBorder, m_pColorSet->m_clrMenuBackground);
	}
	else if (barType == xtpBarTypePopup)
	{
		if (bChecked)
		{
			if (!bEnabled) // disabled
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMarkDisabled,
						 m_pColorSet->m_clrCheckMarkBackgroundDisabled);
			}
			else if (!bSelected && !bPressed) // normal state
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMark,
						 m_pColorSet->m_clrCheckMarkBackground);
			}
			else if (bSelected && !bPressed) // hover
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMarkHighlighted,
						 m_pColorSet->m_clrCheckMarkBackgroundHighlighted);
			}
			else if (bSelected && bPressed) // pressed
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMarkPushed,
						 m_pColorSet->m_clrCheckMarkBackgroundPushed);
			}
		}
		else
		{
			Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType,
								barPosition);
		}
	}
	else
	{
		if (bChecked)
		{
			if (!bSelected && !bPressed) // normal state
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderChecked,
						 m_pColorSet->m_clrButtonBackgroundChecked);
			}
			else if (bSelected && !bPressed) // hover
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderChecked,
						 m_pColorSet->m_clrButtonBackgroundHighlighted);
			}
			else if (bSelected && bPressed) // pressed
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderPushed,
						 m_pColorSet->m_clrButtonBackgroundPushed);
			}
			else
			{
				Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped,
									barType, barPosition);
			}
		}
		else
		{
			if (barType == int(xtpControlSplitButtonPopup))
			{
				FillRect(pDC, rc, FALSE, RGB(255, 0, 0), RGB(255, 0, 0));
			}
			else if (bSelected && !bPressed) // hover
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderHighlighted,
						 m_pColorSet->m_clrButtonBackgroundHighlighted);
			}
			else if (bSelected && bPressed) // pressed
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderPushed,
						 m_pColorSet->m_clrButtonBackgroundPushed);
			}
			else
			{
				Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped,
									barType, barPosition);
			}
		}
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
														 BOOL bPressed, BOOL bEnabled,
														 BOOL bChecked, BOOL bPopuped,
														 XTPBarType barType,
														 XTPBarPosition barPosition, int nID)
{
	CXTPCommandBarsModernUITheme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked,
												bPopuped, barType, barPosition, nID);
}

void CXTPCommandBarsVisualStudio2012Theme::DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl,
															 CPoint pt, BOOL bSelected,
															 BOOL bPopuped, BOOL bEnabled,
															 BOOL bVert)
{
	ASSERT_VALID(pControl);

	UNREFERENCED_PARAMETER(bEnabled);
	UNREFERENCED_PARAMETER(bPopuped);
	UNREFERENCED_PARAMETER(bSelected);

	COLORREF clr = GetGlyphColor(pControl);

	if (pControl->GetPopuped())
	{
		if (pControl->GetType() == xtpControlSplitButtonPopup)
		{
			clr = m_pColorSet->m_clrGlyphPushed;
		}
		else
		{
			clr = m_pColorSet->m_clrGlyph;
		}
	}

	XTPPrimitiveSymbol symb = GetControlSymbol(pControl->GetParent()->GetPosition(), bVert);
	int size				= XTP_COMMANDBAR_TRIANGLE_SIZE / 2;

	XTPPrimitiveDrawer()->DrawSymbol(pDC, symb,
									 CRect(pt.x - size, pt.y - size, pt.x + size, pt.y + size),
									 clr);
}

void CXTPCommandBarsVisualStudio2012Theme::DrawPopupBarGripper(CDC* pDC, int xPos, int yPos, int cx,
															   int cy, BOOL bExpanded)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(xPos);
	UNUSED_ALWAYS(yPos);
	UNUSED_ALWAYS(cx);
	UNUSED_ALWAYS(cy);
	UNUSED_ALWAYS(bExpanded);
}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
																	CXTPControl* pControl,
																	BOOL bDraw)
{
	if (bDraw)
		ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

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
		{
			HorizontalLine(pDC,
						   rcControl.left - XTP_DPI_X(1) + GetPopupBarGripperWidth(pBar)
							   + m_nPopupBarTextPadding,
						   rcControl.top - XTP_DPI_Y(2), rcControl.right + XTP_DPI_X(1),
						   m_pColorSet->m_clrSeparator);
			HorizontalLine(pDC,
						   rcControl.left - XTP_DPI_X(1) + GetPopupBarGripperWidth(pBar)
							   + m_nPopupBarTextPadding,
						   rcControl.top - XTP_DPI_Y(1), rcControl.right + XTP_DPI_X(1),
						   m_pColorSet->m_clrSeparatorHighlight);
		}
		else
		{
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(2), rcRow.top - XTP_DPI_Y(1),
						 rcRow.bottom + XTP_DPI_Y(1), m_pColorSet->m_clrSeparator);
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(1), rcRow.top - XTP_DPI_Y(1),
						 rcRow.bottom + XTP_DPI_Y(1), m_pColorSet->m_clrSeparatorHighlight);
		}
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
		{
			HorizontalLine(pDC, rcRow.left, rcControl.top - XTP_DPI_Y(4), rcRow.right,
						   m_pColorSet->m_clrSeparator);
			HorizontalLine(pDC, rcRow.left, rcControl.top - XTP_DPI_Y(3), rcRow.right,
						   m_pColorSet->m_clrSeparatorHighlight);
		}
		else
		{
			VerticalLine(pDC, rcRow.right + XTP_DPI_X(2), rcRow.top + XTP_DPI_Y(4), rcRow.bottom,
						 m_pColorSet->m_clrSeparator);
			VerticalLine(pDC, rcRow.right + XTP_DPI_X(3), rcRow.top + XTP_DPI_Y(4), rcRow.bottom,
						 m_pColorSet->m_clrSeparatorHighlight);
		}
	}
	else
	{
		if (!pControl->GetWrap())
		{
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(4), rcRow.top + XTP_DPI_Y(2), rcRow.bottom,
						 m_pColorSet->m_clrSeparator);
			VerticalLine(pDC, rcControl.left - XTP_DPI_X(3), rcRow.top + XTP_DPI_Y(2), rcRow.bottom,
						 m_pColorSet->m_clrSeparatorHighlight);
		}
		else
		{
			HorizontalLine(pDC, rcRow.left, rcRow.top - XTP_DPI_Y(4), rcRow.right,
						   m_pColorSet->m_clrSeparator);
			HorizontalLine(pDC, rcRow.left, rcRow.top - XTP_DPI_Y(3), rcRow.right,
						   m_pColorSet->m_clrSeparatorHighlight);
		}
	}
	return 0;
}

void CXTPCommandBarsVisualStudio2012Theme::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton,
														   CPoint pt0, CPoint pt1, CPoint pt2,
														   COLORREF /*clr*/)
{
	ASSERT_VALID(pButton);

	if (pButton->GetPopuped())
	{
		HorizontalLine(pDC, pt2.x - XTP_DPI_X(2), pt0.y - XTP_DPI_Y(3), pt1.x + XTP_DPI_X(1),
					   m_pColorSet->m_clrGlyphPushed);
		Triangle(pDC, pt0, pt1, pt2, m_pColorSet->m_clrGlyphPushed);
	}
	else
	{
		if (pButton->GetSelected())
		{
			HorizontalLine(pDC, pt2.x - XTP_DPI_X(2), pt0.y - XTP_DPI_Y(3), pt1.x + XTP_DPI_X(1),
						   m_pColorSet->m_clrGlyphHighlighted);
			Triangle(pDC, pt0, pt1, pt2, m_pColorSet->m_clrGlyphHighlighted);
		}
		else
		{
			HorizontalLine(pDC, pt2.x - XTP_DPI_X(2), pt0.y - XTP_DPI_Y(3), pt1.x + XTP_DPI_X(1),
						   m_pColorSet->m_clrGlyph);
			Triangle(pDC, pt0, pt1, pt2, m_pColorSet->m_clrGlyph);
		}
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton,
														   COLORREF clr, BOOL bVertical)
{
	CXTPCommandBarsModernUITheme::DrawExpandGlyph(pDC, pButton, clr, bVertical);
}

void CXTPCommandBarsVisualStudio2012Theme::DrawHiddenGlyph(CDC* pDC, CXTPControl* pButton,
														   COLORREF /*clr*/, BOOL bVertical)
{
	ASSERT_VALID(pButton);

	if (pButton->GetPopuped())
	{
		CXTPPaintManager::DrawHiddenGlyph(pDC, pButton, m_pColorSet->m_clrGlyphPushed, bVertical);
	}
	else
	{
		if (pButton->GetSelected())
		{
			CXTPPaintManager::DrawHiddenGlyph(pDC, pButton, m_pColorSet->m_clrGlyphHighlighted,
											  bVertical);
		}
		else
		{
			CXTPPaintManager::DrawHiddenGlyph(pDC, pButton, m_pColorSet->m_clrGlyph, bVertical);
		}
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawCheckMark(CDC* pDC, CRect rcCheck, BOOL bIsRadio,
														 BOOL bEnabled, BOOL bSelected)
{
	rcCheck.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), XTP_DPI_Y(2));

	COLORREF clr = COLORREF_NULL;

	if (!bEnabled)
	{
		clr = m_pColorSet->m_clrCheckMarkDisabled;
	}
	else if (bSelected)
	{
		clr = m_pColorSet->m_clrCheckMarkHighlighted;
	}
	else
	{
		clr = m_pColorSet->m_clrCheckMark;
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

void CXTPCommandBarsVisualStudio2012Theme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton,
																CRect rcButton)
{
	if (pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		Base::DrawSplitButtonFrame(pDC, pButton, rcButton);
		return;
	}

	CPoint pt = CPoint(rcButton.right - m_nSplitButtonDropDownWidth / 2, rcButton.CenterPoint().y);

	BOOL bEnabled  = pButton->GetEnabled();
	BOOL bSelected = pButton->GetSelected();
	BOOL bPopuped  = pButton->GetPopuped();

	if (bEnabled == TRUE_SPLITCOMMAND)
	{
		bEnabled = FALSE;
	}

	if (bEnabled)
	{
		if (bPopuped)
		{
			pDC->FillSolidRect(rcButton, m_pColorSet->m_clrButtonBackgroundPopped);
		}
		else if (pButton->GetSelected() || pButton->GetPressed())
		{
			pDC->FillSolidRect(rcButton, m_pColorSet->m_clrButtonBackgroundHighlighted);

			VerticalLine(pDC, rcButton.right - m_nSplitButtonDropDownWidth, rcButton.top,
						 rcButton.bottom, m_pColorSet->m_clrSeparator);

			if (pButton->GetPressed())
			{
				rcButton.right -= m_nSplitButtonDropDownWidth;

				pDC->FillSolidRect(rcButton, m_pColorSet->m_clrButtonBackgroundPushed);
			}
		}
	}

	DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw,
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
			DrawCheckMark(pDC, rc, FALSE, bEnabled, bSelected);
		}
	}

	return XTP_DPI(CSize(13, 13));
}
