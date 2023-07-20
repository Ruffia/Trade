// XTPRibbonBackstageButton.cpp : implementation file
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
#include "Common/XTPColorManager.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"
#include "Controls/Button/Themes/XTPButtonThemeResource.h"
#include "Controls/Button/Themes/XTPButtonThemeOffice2013.h"
#include "Controls/Button/Themes/XTPButtonThemeVisualStudio2015.h"
#include "Controls/Button/Themes/XTPButtonThemeVisualStudio2017.h"
#include "Controls/Button/Themes/XTPButtonThemeVisualStudio2019.h"
#include "Controls/Button/Themes/XTPButtonThemeVisualStudio2022.h"
#include "Controls/Button/Themes/XTPButtonThemeNativeWindows10.h"

#include "CommandBars/XTPCommandBarsDefines.h"

#include "Ribbon/Backstage/XTPRibbonBackstageButton.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeResource.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeOffice2013.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeVisualStudio2015.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeVisualStudio2017.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeVisualStudio2019.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeVisualStudio2022.h"
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
//  CXTPRibbonBackstageButton

#ifndef _XTP_ACTIVEX_BUTTON
IMPLEMENT_DYNCREATE(CXTPRibbonBackstageButton, CXTPButton)
#else
IMPLEMENT_DYNAMIC(CXTPOleRibbonBackstageButton, CXTPOleButton)
#endif

CXTPRibbonBackstageButton::CXTPRibbonBackstageButton()
	: m_bTabStyle(FALSE)
{
	SetTheme(xtpControlThemeResource);
	m_bDrawKeyboardTip = FALSE;
	m_strKeyboardTip   = _T("");
}

CXTPRibbonBackstageButton::~CXTPRibbonBackstageButton()
{
}

BOOL CXTPRibbonBackstageButton::SetTheme(XTPControlTheme theme)
{
	switch (theme)
	{
		case xtpControlThemeResource:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeResource());
			break;
		case xtpControlThemeOffice2013:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeOffice2013());
			break;
		case xtpControlThemeVisualStudio2015:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeVisualStudio2015());
			break;
		case xtpControlThemeVisualStudio2017:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeVisualStudio2017());
			break;
		case xtpControlThemeVisualStudio2019:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeVisualStudio2019());
			break;
		case xtpControlThemeVisualStudio2022:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeVisualStudio2022());
			break;
		case xtpControlThemeNativeWindows10:
			CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeNativeWindows10());
			break;
		default: CXTPButton::SetTheme(theme); break;
	}

	return TRUE;
}

void CXTPRibbonBackstageButton::SetTabStyle(BOOL bTabStyle)
{
	m_bTabStyle = bTabStyle;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonBackstageButton, CXTPButton)
	//{{AFX_MSG_MAP(CXTPRibbonBackstageSeparator)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_XTP_SETBAKSTAGEBUTTONKEYTIP, OnSetKeyboardTip)
	ON_MESSAGE(WM_XTP_SETBAKSTAGEBUTTONKEYTIPDRAWFLAG, OnSetKeyboardTipDrawFlag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPRibbonBackstageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPButton::OnLButtonDown(nFlags, point);
}

LRESULT CXTPRibbonBackstageButton::OnSetKeyboardTip(WPARAM wParam, LPARAM /*lParam*/)
{
	TCHAR ch		 = (TCHAR)wParam;
	m_strKeyboardTip = _T("");
	m_strKeyboardTip += ch;
	return 0;
}

LRESULT CXTPRibbonBackstageButton::OnSetKeyboardTipDrawFlag(WPARAM wParam, LPARAM /*lParam*/)
{
	m_bDrawKeyboardTip = (BOOL)wParam;
	return 0;
}
// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
