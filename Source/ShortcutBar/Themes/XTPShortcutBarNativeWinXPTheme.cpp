// XTPShortcutBarNativeWinXPTheme.cpp : implementation of the CXTPShortcutBarNativeWinXPTheme class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "ShortcutBar/XTPShortcutBarPaintManager.h"
#include "ShortcutBar/XTPShortcutBar.h"
#include "ShortcutBar/Themes/XTPShortcutBarOffice2003Theme.h"
#include "ShortcutBar/Themes/XTPShortcutBarNativeWinXPTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarNativeWinXPTheme

CXTPShortcutBarNativeWinXPTheme::CXTPShortcutBarNativeWinXPTheme()
{
	m_bLunaColorDisabled = TRUE;

	m_themeToolbar = new CXTPWinThemeWrapper();
	m_themeRebar   = new CXTPWinThemeWrapper();
}

CXTPShortcutBarNativeWinXPTheme::~CXTPShortcutBarNativeWinXPTheme()
{
	SAFE_DELETE(m_themeToolbar);
	SAFE_DELETE(m_themeRebar);
}

void CXTPShortcutBarNativeWinXPTheme::RefreshMetrics()
{
	CXTPShortcutBarOffice2003Theme::RefreshMetrics();

	m_themeToolbar->OpenThemeData(NULL, L"TOOLBAR");
	m_themeRebar->OpenThemeData(NULL, L"REBAR");
}

void CXTPShortcutBarNativeWinXPTheme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	if (!m_themeRebar->IsAppThemeActive())
	{
		CXTPShortcutBarOffice2003Theme::FillShortcutBar(pDC, pShortcutBar);
		return;
	}

	CXTPClientRect rc(pShortcutBar);

	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rc, NULL);
}

void CXTPShortcutBarNativeWinXPTheme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	if (!m_themeToolbar->IsAppThemeActive())
	{
		CXTPShortcutBarOffice2003Theme::FillShortcutItemEntry(pDC, pItem);
		return;
	}

	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc					  = pItem->GetItemRect();

	if (pItem->IsSelected()
		&& (pShortcutBar->GetHotItem() != pItem
			|| (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_CHECKED, &rc, 0);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
	}
	else if (pItem->IsSelected()
			 || (pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_PRESSED, &rc, 0);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL)
			 || pShortcutBar->GetPressedItem() == pItem)
	{
		m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_HOT, &rc, 0);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
	}
}
