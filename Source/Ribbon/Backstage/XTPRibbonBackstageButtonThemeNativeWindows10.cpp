// XTPRibbonBackstageButtonThemeNativeWindows10.cpp : implementation file
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"
#include "Controls/Button/Themes/XTPButtonThemeOffice2013.h"
#include "Controls/Button/Themes/XTPButtonThemeNativeWindows10.h"

#include "Ribbon/Backstage/XTPRibbonBackstageButton.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparator.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageButtonThemeNativeWindows10

CXTPRibbonBackstageButtonThemeNativeWindows10::CXTPRibbonBackstageButtonThemeNativeWindows10()
{
	m_bOffsetHiliteText = FALSE;
}

void CXTPRibbonBackstageButtonThemeNativeWindows10::DrawButtonBackground(CDC* pDC,
																		 CXTPButton* pButton)
{
	CXTPRibbonBackstageButton* pBackstageButton = (CXTPRibbonBackstageButton*)pButton;

	if (pBackstageButton->m_bTabStyle)
	{
		CWnd* pWndParent	  = pButton->GetParent();
		COLORREF clrSeparator = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageSeparator"),
											RGB(225, 225, 225));
		CXTPWindowRect rcButtonScreen(pButton);

		for (CWnd* pChildWnd = pWndParent->GetWindow(GW_CHILD); pChildWnd != NULL;
			 pChildWnd		 = pChildWnd->GetWindow(GW_HWNDNEXT))
		{
#ifdef _XTP_ACTIVEX_BUTTON
			if (pChildWnd->IsKindOf(RUNTIME_CLASS(CBackstageSeparatorCtrl)))
#else
			if (pChildWnd->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageSeparator)))
#endif
			{
				CXTPWindowRect rcSeparatorScreen(pChildWnd);

				CRect rcIntersect;
				if (rcIntersect.IntersectRect(rcButtonScreen, rcSeparatorScreen))
				{
					pButton->ScreenToClient(rcSeparatorScreen);

					CRect rc(rcSeparatorScreen.right - XTP_DPI_X(1), rcSeparatorScreen.top,
							 rcSeparatorScreen.right, rcSeparatorScreen.bottom);
					pDC->FillSolidRect(rc, clrSeparator);
				}
			}
		}
	}

	CXTPButtonThemeNativeWindows10::DrawButtonBackground(pDC, pButton);
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
