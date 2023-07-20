// XTPRibbonBackstageButtonThemeResource.cpp : implementation file
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPHookManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"
#include "CommandBars/XTPCommandBar.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"
#include "Controls/Button/Themes/XTPButtonThemeResource.h"

#include "Ribbon/Backstage/XTPRibbonBackstageView.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButton.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparator.h"
#include "Ribbon/Backstage/XTPRibbonBackstageButtonThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageButtonThemeResource

CXTPRibbonBackstageButtonThemeResource::CXTPRibbonBackstageButtonThemeResource()
{
	m_bOffsetHiliteText = FALSE;
}

void CXTPRibbonBackstageButtonThemeResource::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	CXTPRibbonBackstageButton* pBackstageButton = (CXTPRibbonBackstageButton*)pButton;

	BOOL bPressed  = pBackstageButton->IsPushed();
	BOOL bSelected = pBackstageButton->IsHighlighted();
	BYTE bStyle	= pBackstageButton->GetButtonStyle();
	BOOL bEnabled  = pBackstageButton->IsWindowEnabled();
	BOOL bChecked  = pBackstageButton->GetChecked();
	BOOL bDefault  = bStyle == BS_DEFPUSHBUTTON;
	CRect rc(pButton->GetButtonRect());

	if (pBackstageButton->m_bTabStyle)
	{
		CWnd* pWndParent = pButton->GetParent();

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

					CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(
						_T("BACKSTAGEPANESEPARATOR"));
					if (pImage)
					{
						pImage->DrawImage(pDC,
										  CRect(rcSeparatorScreen.right - XTP_DPI_X(1),
												rcSeparatorScreen.top, rcSeparatorScreen.right,
												rcSeparatorScreen.bottom),
										  pImage->GetSource(0, 1), CRect(0, 30, 0, 30));
					}
				}
			}
		}
	}

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemeActive())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (pBackstageButton->m_bShowShadow && bEnabled)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTONSHADOW"));

		if (pImage)
		{
			pImage->DrawImage(pDC, CRect(rc.left, rc.bottom - XTP_DPI_Y(2), rc.right, rc.bottom),
							  pImage->GetSource(), CRect(3, 0, 3, 0), 0xFF00FF);
			rc.bottom -= XTP_DPI_Y(2);
		}
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTON"));
	if (!pImage)
		pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));

	if (!pImage)
	{
		CXTPButtonTheme::DrawButtonBackground(pDC, pButton);
		return;
	}

	int nState = bDefault ? 4 : 0;

	if (!bEnabled)
	{
		nState = 3;
	}
	else if (bChecked && !bSelected && !bPressed)
		nState = 2;
	else if (bChecked && bSelected && !bPressed)
		nState = 1;
	else if ((bSelected && bPressed) || pButton->IsPushed())
		nState = 2;
	else if (bSelected || bPressed)
		nState = 1;

	if (nState != -1)
	{
		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 5), CRect(4, 4, 4, 4), 0xFF00FF);
	}

	if (bChecked && pBackstageButton->m_bTabStyle)
	{
		pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTONGLYPH"));
		if (pImage)
		{
			CRect rcSrc(pImage->GetSource());
			CRect rcDst(XTP_DPI(rcSrc));
			int y = rc.CenterPoint().y;
			pImage->DrawImage(pDC,
							  CRect(rc.right - rcDst.Width(), y - rcDst.Height() / 2, rc.right,
									y - rcDst.Height() / 2 + rcDst.Height()),
							  rcSrc);
		}
	}
}

void CXTPRibbonBackstageButtonThemeResource::DrawButton(CDC* pDC, CXTPButton* pButton)
{
	ASSERT(pDC);
	ASSERT(pButton);
	if ((pDC == NULL) || (pButton == NULL))
		return;
	CXTPButtonThemeResource::DrawButton(pDC, pButton);
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
