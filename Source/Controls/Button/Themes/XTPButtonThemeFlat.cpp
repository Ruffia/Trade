// XTPButtonThemeFlat.cpp: implementation of the CXTPButtonThemeFlat class.
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
#include "Controls/Button/Themes/XTPButtonThemeFlat.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeFlat

CXTPButtonThemeFlat::CXTPButtonThemeFlat()
{
	m_nTheme	   = xtpControlThemeFlat;
	m_bFlatGlyphs  = TRUE;
	m_nBorderWidth = XTP_DPI_X(4);
}

void CXTPButtonThemeFlat::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
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

	pDC->FillSolidRect(rc, pButton->GetButtonBackColor());

	COLORREF clrBorder = GetXtremeColor(COLOR_WINDOWFRAME);

	if (bDefault || bChecked)
	{
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
		rc.DeflateRect(1, 1);
	}

	pDC->Draw3dRect(rc, clrBorder, clrBorder);

	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
