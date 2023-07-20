// XTPStatusBarThemeOffice2003.cpp
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
#include "Common/XTPWinThemeWrapper.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeOffice2003::CXTPStatusBarThemeOffice2003(CXTPCommandBarsOffice2003Theme* pTheme)
	: CXTPStatusBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPStatusBarThemeOffice2003::RefreshMetrics()
{
	CXTPStatusBarPaintManager::RefreshMetrics();

	m_clrBackground = XTPColorManager()->LightColor(::GetXtremeColor(COLOR_3DFACE),
													::GetXtremeColor(COLOR_WINDOW), 70);

	switch (m_pTheme->GetCurrentSystemTheme())
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeOlive:
		case xtpSystemThemeAero: m_clrBackground = RGB(211, 211, 211); break;
		case xtpSystemThemeSilver: m_clrBackground = RGB(236, 234, 218); break;
		case xtpSystemThemeUnknown:
		case xtpSystemThemeAuto:
		case xtpSystemThemeDefault:
		default: break;
	}
}

void CXTPStatusBarThemeOffice2003::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_pTheme->m_bThemedStatusBar && m_pTheme->m_themeStatusBar->IsAppThemeActive())
	{
		CRect rcGripper(rcClient.right - XTP_DPI_X(18), rcClient.top, rcClient.right,
						rcClient.bottom);
		m_pTheme->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), SP_GRIPPER, 0,
														&rcGripper, 0);
	}
	else
	{
		//	96  DPI	- 1 white, 3 black lines
		//	120 DPI	- 2 white, 3 black lines
		//	144 DPI - 2 white, 4 black lines

		int nDPI			 = XTPDpiHelper()->GetDPIX();
		int nBlackLinesCount = (nDPI < 144) ? 3 : 4;
		int nWhiteLinesCount = ((nDPI < 120) ? 1 : 2);
		int x				 = rcClient.Width() - 2;
		int y				 = rcClient.Height() - 2;
		int nGripperStep	 = nBlackLinesCount + nWhiteLinesCount + 1;

		for (int i = 0; i < 3; i++)
		{
			CXTPPenDC penBorder(*pDC, m_clrGripperDark);
			int j = 1;
			for (; j <= nBlackLinesCount; j++)
			{
				pDC->MoveTo(x - j - i * nGripperStep, y);
				pDC->LineTo(x, y - j - i * nGripperStep);
			}

			CXTPPenDC penBorderWhite(*pDC, m_clrGripperLite);
			for (; j < nGripperStep; j++)
			{
				pDC->MoveTo(x - j - i * nGripperStep, y);
				pDC->LineTo(x, y - j - i * nGripperStep);
			}
		}
	}
}

void CXTPStatusBarThemeOffice2003::DrawStatusBarPaneBorder(CDC* pDC, CRect rc,
														   CXTPStatusBarPane* pPane,
														   BOOL bGripperPane)
{
	if (m_pTheme->m_bThemedStatusBar && m_pTheme->m_themeStatusBar->IsAppThemed())
	{
		CXTPStatusBarPaintManager::DrawStatusBarPaneBorder(pDC, rc, pPane, bGripperPane);
	}
	else
	{
		pDC->Draw3dRect(rc, m_clrBackground, m_clrBackground);
	}
}
