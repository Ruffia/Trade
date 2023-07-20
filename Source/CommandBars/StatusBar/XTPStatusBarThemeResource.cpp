// XTPStatusBarThemeResource.cpp
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
#include "Common/XTPResourceImage.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"
#include "CommandBars/Themes/XTPCommandBarsResourceTheme.h"

#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPane.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeResource::CXTPStatusBarThemeResource(CXTPResourceTheme* pTheme)
	: CXTPStatusBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPStatusBarThemeResource::RefreshMetrics()
{
	CXTPStatusBarPaintManager::RefreshMetrics();

	m_clrText	 = XTPIniColor(_T("StatusBar"), _T("StatusBarText"), ::GetSysColor(COLOR_BTNTEXT));
	m_clrTextGray = XTPIniColor(_T("StatusBar"), _T("StatusBarTextGray"),
								::GetSysColor(COLOR_GRAYTEXT));
	m_clrGripperLite = XTPIniColor(_T("StatusBar"), _T("GripperLite"), RGB(255, 255, 255));
	m_clrGripperDark = XTPIniColor(_T("StatusBar"), _T("GripperDark"),
								   ::GetXtremeColor(XPCOLOR_SEPARATOR));
}

void CXTPStatusBarThemeResource::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	if (!XTPResourceImages()->IsValid())
		return;

	const CXTPClientRect rc(pBar);

	// Fill light background.
	CXTPResourceImage* pLightImage = XTPResourceImages()->LoadFile(_T("STATUSBARLIGHT"));
	ASSERT(NULL != pLightImage);
	if (NULL != pLightImage)
	{
		const CRect rcLightImage(0, 0, pLightImage->GetWidth(), pLightImage->GetHeight());
		pLightImage->DrawImage(pDC, rc, rcLightImage);
	}

	// Fill dark panes.
	CXTPResourceImage* pDarkImage = NULL;
	CRect rcDarkImage;
	const int nPaneCount = pBar->GetPaneCount();
	for (int i = 0; i < nPaneCount; ++i)
	{
		CXTPStatusBarPane* pPane = pBar->GetPane(i);
		ASSERT(NULL != pPane);
		if (pPane->IsDark() && pPane->IsVisible())
		{
			if (NULL == pDarkImage)
			{
				pDarkImage = XTPResourceImages()->LoadFile(_T("STATUSBARDARK"));
				rcDarkImage.SetRect(0, 0, pDarkImage->GetWidth(), pDarkImage->GetHeight());
			}

			ASSERT(NULL != pDarkImage);
			if (NULL != pDarkImage)
			{
				CRect rcPane = pPane->GetRect();

				BOOL bLastVisible = TRUE;
				for (int nIndex = i + 1; nIndex < nPaneCount; ++nIndex)
				{
					CXTPStatusBarPane* pNextPane = pBar->GetPane(nIndex);
					ASSERT(NULL != pNextPane);
					if (pNextPane->IsVisible())
					{
						bLastVisible = FALSE;
						break;
					}
				}

				if (bLastVisible)
				{
					rcPane.right = rc.right;
				}

				rcPane.top	= rc.top;
				rcPane.bottom = rc.bottom;
				pDarkImage->DrawImage(pDC, rcPane, rcDarkImage);
			}
		}
	}
}

void CXTPStatusBarThemeResource::DrawStatusBarPaneBorder(CDC* pDC, CRect rc,
														 CXTPStatusBarPane* /*pPane*/,
														 BOOL bGripperPane)
{
	if (!bGripperPane)
	{
		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1),
						   rc.Height() - XTP_DPI_Y(2), m_clrGripperDark);
		pDC->FillSolidRect(rc.right, rc.top, XTP_DPI_X(1), rc.Height() - XTP_DPI_Y(2),
						   m_clrGripperLite);
	}
}

void CXTPStatusBarThemeResource::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("STATUSBARGRIPPER"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CSize szMargin(XTP_DPI_X(1), XTP_DPI_Y(4));
	CRect rcSrc(0, 0, pImage->GetWidth(), pImage->GetHeight());
	CRect rcGripper(rcClient.right - XTP_DPI_X(rcSrc.Width()) - szMargin.cx,
					rcClient.bottom - XTP_DPI_Y(rcSrc.Height()) - szMargin.cy,
					rcClient.right - szMargin.cx, rcClient.bottom - szMargin.cy);
	pImage->DrawImage(pDC, rcGripper, rcSrc, CRect(0, 0, 0, 0), RGB(0xFF, 0, 0xFF));
}
