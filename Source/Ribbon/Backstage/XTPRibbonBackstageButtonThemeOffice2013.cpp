// XTPRibbonBackstageButtonThemeOffice2013.cpp : implementation file
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"
#include "CommandBars/XTPCommandBar.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"
#include "Controls/Button/Themes/XTPButtonThemeOffice2013.h"

#include "Ribbon/Backstage/XTPRibbonBackstageView.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButton.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparator.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageButtonThemeOffice2013

CXTPRibbonBackstageButtonThemeOffice2013::CXTPRibbonBackstageButtonThemeOffice2013()
{
	m_bOffsetHiliteText = FALSE;
}

void CXTPRibbonBackstageButtonThemeOffice2013::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
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

	CXTPButtonThemeOffice2013::DrawButtonBackground(pDC, pButton);
}

void CXTPRibbonBackstageButtonThemeOffice2013::DrawButton(CDC* pDC, CXTPButton* pButton)
{
	ASSERT(pDC);
	ASSERT(pButton);
	if ((pDC == NULL) || (pButton == NULL))
		return;
	CXTPButtonThemeOffice2013::DrawButton(pDC, pButton);
#ifndef _XTP_COMMANDBARS_ACTIVEX
	CXTPRibbonBackstageButton* pBackstageButton = DYNAMIC_DOWNCAST(CXTPRibbonBackstageButton,
																   pButton);
	if (!pBackstageButton)
		return;
	if (!pBackstageButton->m_bDrawKeyboardTip)
		return;

	CWnd* pWnd = pBackstageButton->GetParent();
	CXTPRibbonBackstageView* pBsView;
	while (pWnd != NULL)
	{
		if (pWnd->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageView)))
			break;
		pWnd = pWnd->GetParent();
	}
	if (pWnd == NULL)
		return;
	pBsView = DYNAMIC_DOWNCAST(CXTPRibbonBackstageView, pWnd);

	CRect rec;
	pButton->GetClientRect(&rec);
	CXTPFontDC font(pDC, pBsView->GetKeyboardTipsFont());
	CSize sz = pDC->GetTextExtent(pBackstageButton->m_strKeyboardTip);
	sz = CSize(max(sz.cx + XTP_DPI_X(6), XTP_DPI_X(16)), max(sz.cy + XTP_DPI_Y(2), XTP_DPI_Y(15)));
	rec.right  = rec.left + sz.cx;
	rec.bottom = rec.top + sz.cy;

	pDC->FillSolidRect(rec, pBsView->GetKeyboardTipsBackColor());
	pDC->Draw3dRect(rec, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetXtremeColor(
		UINT(pBackstageButton->IsWindowEnabled() ? COLOR_INFOTEXT : COLOR_GRAYTEXT)));
	pDC->DrawText(pBackstageButton->m_strKeyboardTip, rec, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
#endif
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
