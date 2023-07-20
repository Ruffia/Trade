// XTPButtonThemeUltraFlat.cpp: implementation of the CXTPButtonThemeUltraFlat class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeUltraFlat

CXTPButtonThemeUltraFlat::CXTPButtonThemeUltraFlat()
{
	m_nTheme		   = xtpControlThemeUltraFlat;
	m_bFlatGlyphs	  = TRUE;
	m_nBorderWidth	 = XTP_DPI_X(3);
	m_bHiglightButtons = TRUE;
}

void CXTPButtonThemeUltraFlat::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonTheme::RefreshMetrics(pButton);

	COLORREF clrWindow  = GetXtremeColor(COLOR_WINDOW);
	COLORREF clrToolBar = GetXtremeColor(XPCOLOR_TOOLBAR_FACE);
	COLORREF clrFace	= pButton->GetButtonBackColor();

	m_crBack.SetStandardValue(clrFace);

	COLORREF clrPushed = GetXtremeColor(COLOR_3DSHADOW);
	clrPushed		   = XTPColorManager()->LightColor(clrPushed, clrFace, 0x32);
	m_crBackPushed.SetStandardValue(clrPushed);

	COLORREF clrHighlight = clrFace;
	clrHighlight		  = XTPColorManager()->LightColor(clrHighlight, clrWindow, 0x1E);
	m_crBackHilite.SetStandardValue(clrHighlight);

	COLORREF clrChecked = RGB(GetRValue(clrWindow) * 40 / 100 + GetRValue(clrHighlight) * 10 / 100
								  + GetRValue(clrToolBar) * 50 / 100,
							  GetGValue(clrWindow) * 40 / 100 + GetGValue(clrHighlight) * 10 / 100
								  + GetGValue(clrToolBar) * 50 / 100,
							  GetBValue(clrWindow) * 40 / 100 + GetBValue(clrHighlight) * 10 / 100
								  + GetBValue(clrToolBar) * 50 / 100);

	m_crBackChecked.SetStandardValue(clrChecked);

	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorderHilite.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	COLORREF clrText = pButton->GetButtonTextColor();
	m_crText.SetStandardValue(clrText);
	m_crTextCheck.SetStandardValue(clrText);
	m_crTextHilite.SetStandardValue(clrText);
	m_crTextPushed.SetStandardValue(clrText);

	m_crCheckMark.SetStandardValue(COLORREF(-1));
}

void CXTPButtonThemeUltraFlat::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot	 = pButton->IsHighlighted();
	BYTE bStyle   = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (::IsWindow(pButton->m_hWndBuddy))
	{
		bDefault = FALSE;
	}

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (IsVisualThemeUsed(pButton))
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (bDefault || bChecked)
	{
		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
		rc.DeflateRect(1, 1);
	}

	COLORREF clrBorder;
	COLORREF clrBackground;

	if (bPressed)
	{
		clrBorder	 = m_crBorderShadow;
		clrBackground = m_crBackPushed;
	}
	else if (bHot)
	{
		clrBorder	 = m_crBorderHilite;
		clrBackground = m_crBackHilite;
	}
	else if (bChecked)
	{
		clrBorder	 = m_crBorderHilite;
		clrBackground = m_crBackChecked;
	}
	else
	{
		clrBorder	 = m_crBorderShadow;
		clrBackground = pButton->GetButtonBackColor();
	}

	pDC->FillSolidRect(rc, clrBackground);
	pDC->Draw3dRect(rc, clrBorder, clrBorder);
}

void CXTPButtonThemeUltraFlat::DrawGroupBox(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawGroupBox(pDC, pButton);
		return;
	}

	CRect rcGroupBox;
	CalcRect(pDC, pButton, &rcGroupBox, CBR_GROUPFRAME);

	CRect rcGrouText;
	CalcRect(pDC, pButton, &rcGrouText, CBR_GROUPTEXT);

	if (pButton->GetTextAlignment() & BS_RIGHT)
		rcGrouText.OffsetRect(+m_cxEdge, 0);
	else
		rcGrouText.OffsetRect(-m_cxEdge, 0);

	pDC->ExcludeClipRect(&rcGrouText);

	if (pButton->GetBorderStyle() == xtpGroupBoxSingleLine)
	{
		pDC->FillSolidRect(rcGroupBox.left, rcGroupBox.top, rcGroupBox.Width(), XTP_DPI_Y(1),
						   m_crBorderShadow);
	}
	else
	{
		pDC->Draw3dRect(&rcGroupBox, m_crBorderShadow, m_crBorderShadow);
	}

	pDC->SelectClipRgn(NULL);
}

void CXTPButtonThemeUltraFlat::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bChecked  = pButton->GetChecked();

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_CHECKBOX);

	COLORREF crBack = m_crBack;
	COLORREF crBorder;
	if (bEnabled)
	{
		if (!m_bHiglightButtons)
		{
			crBack = bSelected && bPressed ? m_crBackPushed : m_crBack;
		}
		else if (bSelected && bPressed)
		{
			crBack = m_crBackPushed;
		}
		else if (bPressed || bSelected)
		{
			crBack = m_crBackHilite;
		}
		else
		{
			crBack = m_crBack;
		}

		crBorder = (bSelected || bPressed ? m_crBorderHilite : m_crBorderShadow);
	}
	else
	{
		crBorder = GetXtremeColor(XPCOLOR_GRAYTEXT);
		crBack   = GetXtremeColor(XPCOLOR_3DFACE);
	}

	pDC->FillSolidRect(rc, crBack);
	pDC->Draw3dRect(rc, crBorder, crBorder);

	if (bChecked)
	{
		CXTPPaintManagerColor clrCheck;

		if (m_crCheckMark == (COLORREF)-1)
			clrCheck = GetXtremeColor(!bEnabled || bChecked == 2 ? UINT(XPCOLOR_GRAYTEXT)
																 : UINT(COLOR_BTNTEXT));
		else
			clrCheck = m_crCheckMark;

		XTPPrimitiveDrawer()->DrawSymbol(pDC,
										 CXTPDrawHelpers::IsContextRTL(pDC)
											 ? xtpPrimitiveSymbolCheckMarkRTL
											 : xtpPrimitiveSymbolCheckMark,
										 rc, clrCheck);
	}
}

void CXTPButtonThemeUltraFlat::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bChecked  = pButton->GetChecked();

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_RADIOBOX);

	COLORREF crBorder;
	COLORREF crFace;
	if (bEnabled)
	{
		if (!m_bHiglightButtons)
		{
			crBorder = m_crBorderShadow;
			crFace   = (bSelected && bPressed ? m_crBackPushed : m_crBack);
		}
		else
		{
			crBorder = (bSelected || bPressed ? m_crBorderHilite : m_crBorderShadow);
			if (bSelected && bPressed)
			{
				crFace = m_crBackPushed;
			}
			else if (bPressed || bSelected)
			{
				crFace = m_crBackHilite;
			}
			else
			{
				crFace = m_crBack;
			}
		}
	}
	else
	{
		crBorder = GetXtremeColor(XPCOLOR_GRAYTEXT);
		crFace   = GetXtremeColor(XPCOLOR_3DFACE);
	}

	AlphaEllipse(pDC, rc, crBorder, crFace);

	if (bChecked)
	{
		CXTPPaintManagerColor clrCheck;

		if (m_crCheckMark == (COLORREF)-1)
			clrCheck = GetXtremeColor(!bEnabled || bChecked == 2 ? UINT(XPCOLOR_GRAYTEXT)
																 : UINT(COLOR_BTNTEXT));
		else
			clrCheck = m_crCheckMark;

		AlphaEllipse(pDC,
					 CRect(rc.left + XTP_DPI_X(4), rc.top + XTP_DPI_Y(4), rc.left + XTP_DPI_X(9),
						   rc.top + XTP_DPI_Y(9)),
					 clrCheck, clrCheck);
	}
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
