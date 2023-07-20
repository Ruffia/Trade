// XTPPopupThemeOffice2003.cpp: implementation of the CXTPPopupThemeOffice2003 class.
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "Controls/Defines.h"
#include "Controls/Popup/XTPPopupControl.h"
#include "Controls/Popup/XTPPopupPaintManager.h"
#include "Controls/Popup/Themes/XTPPopupThemeOffice2000.h"
#include "Controls/Popup/Themes/XTPPopupThemeOfficeXP.h"
#include "Controls/Popup/Themes/XTPPopupThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOffice2003

CXTPPopupThemeOffice2003::CXTPPopupThemeOffice2003()
{
}

CXTPPopupThemeOffice2003::~CXTPPopupThemeOffice2003()
{
}

void CXTPPopupThemeOffice2003::RefreshMetrics()
{
	CXTPPopupThemeOfficeXP::RefreshMetrics();

	m_clrBackground.SetStandardValue(
		XTPColorManager()->grcDockBar.clrDark,
		XTPColorManager()->grcDockBar.clrLight); //.SetStandardValue(RGB(214,
												 // 231, 252), RGB(168,
												 // 198, 238));
	m_clrGripper.SetStandardValue(XTPColorManager()->grcShortcutBarGripper);
	m_clrFrame.SetStandardValue(GetXtremeColor(XPCOLOR_FRAME));

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
			case xtpSystemThemeBlue:
			case xtpSystemThemeRoyale:
			case xtpSystemThemeAero:
				m_clrButtonSelected = RGB(255, 238, 194);
				m_clrButtonPressed  = RGB(254, 128, 62);
				m_clrButtonPressedBorder.SetStandardValue(RGB(0, 0, 128));
				m_clrButtonSelectedBorder.SetStandardValue(RGB(0, 0, 128));
				break;

			case xtpSystemThemeOlive:
				m_clrButtonSelected = RGB(255, 238, 194);
				m_clrButtonPressed  = RGB(254, 128, 62);
				m_clrButtonPressedBorder.SetStandardValue(RGB(63, 93, 56));
				m_clrButtonSelectedBorder.SetStandardValue(RGB(63, 93, 56));
				break;

			case xtpSystemThemeSilver:
				m_clrButtonSelected = RGB(255, 238, 194);
				m_clrButtonPressed  = RGB(254, 128, 62);
				m_clrButtonPressedBorder.SetStandardValue(RGB(75, 75, 111));
				m_clrButtonSelectedBorder.SetStandardValue(RGB(75, 75, 111));
				break;
		}
	}
}

void CXTPPopupThemeOffice2003::DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient)
{
	if (pControl->GetBackgroundBitmap() > 0)
	{
		CXTPPopupPaintManager::DrawBackground(pDC, pControl, rcClient);
		return;
	}

	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, FALSE);
	pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	rcClient.DeflateRect(1, 1);

	CRect rcGripper(rcClient);
	rcGripper.bottom = rcGripper.top + XTP_DPI_Y(7);
	XTPDrawHelpers()->GradientFill(pDC, rcGripper, m_clrGripper, FALSE);

	int nRight = max(XTP_DPI_X(2), (rcGripper.Width() - XTP_DPI_X(4) * 9) / 2);
	for (int i = 0; i < 9; i++)
	{
		pDC->FillSolidRect(nRight, rcGripper.top + XTP_DPI_Y(1), XTP_DPI_X(2), XTP_DPI_Y(2),
						   RGB(40, 50, 71));
		pDC->FillSolidRect(nRight + XTP_DPI_X(1), rcGripper.top + XTP_DPI_Y(2), XTP_DPI_X(2),
						   XTP_DPI_Y(2), RGB(249, 249, 251));
		pDC->FillSolidRect(nRight + XTP_DPI_X(1), rcGripper.top + XTP_DPI_Y(2), XTP_DPI_X(1),
						   XTP_DPI_Y(1), RGB(97, 116, 152));

		nRight += XTP_DPI_X(4);
	}
}
