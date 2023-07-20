// XTPButtonThemeVisualStudio2012.cpp: implementation of the CXTPButtonThemeVisualStudio2012 class.
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
#include "Controls/Button/Themes/XTPButtonThemeOffice2013.h"
#include "Controls/Button/Themes/XTPButtonThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeVisualStudio2012

CXTPButtonThemeVisualStudio2012::CXTPButtonThemeVisualStudio2012(BOOL bLight)
	: m_bLight(bLight)
{
	if (m_bLight)
		m_nTheme = xtpControlThemeVisualStudio2012Light;
	else
		m_nTheme = xtpControlThemeVisualStudio2012Dark;

	m_nBorderWidth		= XTP_DPI_X(2);
	m_bOffsetHiliteText = FALSE;
}

void CXTPButtonThemeVisualStudio2012::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::RefreshMetrics(pButton);

	if (m_bLight)
	{
		m_crBack.SetStandardValue(RGB(238, 238, 242)); // RGB value for nomral background
		m_crBackHilite.SetStandardValue(
			RGB(201, 222, 245)); // RGB value for highlighted background color.
		m_crBackPushed.SetStandardValue(RGB(0, 122, 204)); // RGB value for pushed background color.
		m_crBorderHilite.SetStandardValue(RGB(0, 122, 204));
		m_crBorderShadow.SetStandardValue(RGB(204, 206, 219));
		m_crBorder3DHilite.SetStandardValue(RGB(0, 122, 204));
		m_crBorder3DShadow.SetStandardValue(RGB(204, 206, 219));
		m_crBorderPushed.SetStandardValue(RGB(0, 122, 204)); // RGB value for border pushed color.
		m_crText.SetStandardValue(RGB(30, 30, 30));			 // RGB value for normal text
		m_crTextCheck.SetStandardValue(RGB(30, 30, 30));	 // RGB value for normal text
		m_crTextHilite.SetStandardValue(RGB(30, 30, 30));	// RGB value for highlighted text color
		m_crTextPushed.SetStandardValue(RGB(255, 255, 255)); // RGB value for pushed text color.
		m_crTextDisabled.SetStandardValue(RGB(162, 164, 165)); // RGB value for disabled text color.
		m_crCheckMark.SetStandardValue(RGB(113, 113, 113));
	}
	else
	{
		m_crBack.SetStandardValue(RGB(45, 45, 48)); // RGB value for nomral background
		m_crBackHilite.SetStandardValue(
			RGB(31, 31, 32)); // RGB value for highlighted background color.
		m_crBackPushed.SetStandardValue(RGB(0, 122, 204)); // RGB value for pushed background color.
		m_crBorderHilite.SetStandardValue(RGB(0, 122, 204));
		m_crBorderShadow.SetStandardValue(RGB(67, 67, 70));
		m_crBorder3DHilite.SetStandardValue(RGB(0, 122, 204));
		m_crBorder3DShadow.SetStandardValue(RGB(67, 67, 70));
		m_crBorderPushed.SetStandardValue(RGB(0, 122, 204)); // RGB value for border pushed color.
		m_crText.SetStandardValue(RGB(241, 241, 241));		 // RGB value for normal text
		m_crTextCheck.SetStandardValue(RGB(241, 241, 241));  // RGB value for normal text
		m_crTextHilite.SetStandardValue(RGB(241, 241, 241)); // RGB value for highlighted text color
		m_crTextPushed.SetStandardValue(RGB(255, 255, 255)); // RGB value for pushed text color.
		m_crTextDisabled.SetStandardValue(RGB(101, 101, 101)); // RGB value for disabled text color.
		m_crCheckMark.SetStandardValue(RGB(153, 153, 153));
	}
}

void CXTPButtonThemeVisualStudio2012::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bChecked  = pButton->GetChecked();
	BOOL bDefault  = pButton->GetButtonStyle() == BS_DEFPUSHBUTTON;

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	COLORREF clrBack;
	COLORREF clrBorder;

	if (!bEnabled)
	{
		clrBack   = m_crBack;
		clrBorder = m_crBorderShadow;
	}
	else if (bSelected && bPressed)
	{
		clrBack   = m_crBackPushed;
		clrBorder = m_crBorderPushed;
	}
	else if (bSelected || bPressed && m_bHiglightButtons)
	{
		if (bDefault)
		{
			clrBack   = XTPColorManager()->LightColor(m_crBackPushed, m_crBackHilite, 50);
			clrBorder = XTPColorManager()->LightColor(m_crBorderPushed, m_crBorderHilite, 50);
		}
		else
		{
			clrBack   = m_crBackHilite;
			clrBorder = m_crBorderHilite;
		}
	}
	else
	{
		if (bDefault)
		{
			clrBack   = m_crBackHilite;
			clrBorder = m_crBorderHilite;
		}
		else
		{
			clrBack   = m_crBack;
			clrBorder = m_crBorderShadow;
		}
	}

	CRect rc(pButton->GetButtonRect());
	pDC->FillSolidRect(rc, clrBack);
	pDC->Draw3dRect(rc, clrBorder, clrBorder);
}

COLORREF CXTPButtonThemeVisualStudio2012::GetTextColor(CXTPButton* pButton)
{
	if (!pButton->IsWindowEnabled())
		return m_crTextDisabled;

	COLORREF textColor = pButton->GetButtonTextColor();
	if (textColor != COLORREF_NULL)
		return textColor;

	BYTE bStyle = pButton->GetButtonStyle();
	if (bStyle == BS_RADIOBUTTON || bStyle == BS_AUTORADIOBUTTON || bStyle == BS_CHECKBOX
		|| bStyle == BS_AUTOCHECKBOX || bStyle == BS_3STATE || bStyle == BS_AUTO3STATE)
	{
		return m_crTextCheck;
	}

	if (pButton->IsPushed())
		return m_crTextPushed;

	return m_crText;
}

void CXTPButtonThemeVisualStudio2012::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::DrawCheckBoxMark(pDC, pButton);
}

void CXTPButtonThemeVisualStudio2012::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::DrawRadioButtonMark(pDC, pButton);
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
