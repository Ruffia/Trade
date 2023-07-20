// XTPButtonThemeOfficeXP.cpp: implementation of the CXTPButtonThemeOfficeXP class.
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
#include "Controls/Button/Themes/XTPButtonThemeOfficeXP.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeOfficeXP

CXTPButtonThemeOfficeXP::CXTPButtonThemeOfficeXP()
{
	m_nTheme			= xtpControlThemeOfficeXP;
	m_nBorderWidth		= XTP_DPI_X(2);
	m_bOffsetHiliteText = FALSE;
}

void CXTPButtonThemeOfficeXP::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::RefreshMetrics(pButton);

	m_crBack.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_crBackPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_crBackHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorderHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_crText.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_TEXT));
	m_crTextCheck.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_TEXT));
	m_crTextHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_crTextPushed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_crBackChecked.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
}

void CXTPButtonThemeOfficeXP::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot	 = pButton->IsHighlighted();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (IsVisualThemeUsed(pButton))
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (bHot || bPressed || bChecked)
	{
		pDC->FillSolidRect(rc, bPressed ? m_crBackPushed
										: bChecked ? (bHot ? m_crBackPushed : m_crBackChecked)
												   : m_crBackHilite);

		pDC->Draw3dRect(rc, m_crBorderHilite, m_crBorderHilite);
	}
	else
	{
		pDC->FillSolidRect(rc, m_crBack);

		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
	}
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
