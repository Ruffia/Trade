// XTPStatusBarThemeVisualStudio2010.cpp
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
#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2010.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeVisualStudio2010::CXTPStatusBarThemeVisualStudio2010(
	CXTPPaintManager* pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeVisualStudio2010::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect(pBar), m_clrBackground);
}

void CXTPStatusBarThemeVisualStudio2010::RefreshMetrics()
{
	CXTPStatusBarPaintManager::RefreshMetrics();

	m_clrBackground = RGB(41, 57, 85);
	m_clrText		= RGB(255, 255, 255);
}

void CXTPStatusBarThemeVisualStudio2010::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_pPaintManager->m_bThemedStatusBar && m_pPaintManager->m_themeStatusBar->IsAppThemed())
	{
		for (int y = 0; y <= 3; y++)
		{
			for (int x = 0; x <= 3 - y; x++)
			{
				int left = rcClient.right - x * XTP_DPI_X(3) - XTP_DPI_X(4);
				int top  = rcClient.bottom - y * XTP_DPI_Y(3) - XTP_DPI_X(4);

				pDC->SetPixel(left, top, RGB(248, 249, 250));
				pDC->SetPixel(left + 1, top, RGB(198, 206, 221));
				pDC->SetPixel(left, top + 1, RGB(227, 231, 238));
				pDC->SetPixel(left + 1, top + 1, RGB(170, 183, 205));
			}
		}
	}
	else
	{
		CXTPStatusBarPaintManager::DrawStatusBarGripper(pDC, rcClient);
	}
}
