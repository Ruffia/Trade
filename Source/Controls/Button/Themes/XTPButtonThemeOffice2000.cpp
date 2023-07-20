// XTPButtonThemeOffice2000.cpp: implementation of the CXTPButtonThemeOffice2000 class.
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
#include "Controls/Button/Themes/XTPButtonThemeOffice2000.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeOffice2000

CXTPButtonThemeOffice2000::CXTPButtonThemeOffice2000()
{
	m_nTheme		   = xtpControlThemeOffice2000;
	m_nBorderWidth	 = XTP_DPI_X(2);
	m_bHiglightButtons = FALSE;
}

void CXTPButtonThemeOffice2000::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
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

	pDC->FillSolidRect(rc, pButton->GetButtonBackColor());

	BOOL bSelected = bPressed || bChecked;

	if (::IsWindow(pButton->m_hWndBuddy))
	{
		rc.left += 1;
	}

	pDC->Draw3dRect(rc, bSelected ? m_crBorderShadow : m_crBorder3DHilite,
					bSelected ? m_crBorder3DHilite : m_crBorderShadow);
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
