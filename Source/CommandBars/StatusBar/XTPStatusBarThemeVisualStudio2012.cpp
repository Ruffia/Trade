// XTPStatusBarThemeVisualStudio2012.cpp
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

#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeVisualStudio2012::CXTPStatusBarThemeVisualStudio2012(
	CXTPPaintManager* pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeVisualStudio2012::RefreshMetrics()
{
	CXTPStatusBarPaintManager::RefreshMetrics();

	m_clrBackground  = XTPIniColor(_T("StatusBar"), _T("BackgroundColor"), RGB(1, 120, 208));
	m_clrTextGray	= XTPIniColor(_T("StatusBar"), _T("TextGrayColor"), m_clrBackground);
	m_clrGripperLite = XTPIniColor(_T("StatusBar"), _T("GripperLite"), RGB(84, 191, 255));
	m_clrGripperDark = XTPIniColor(_T("StatusBar"), _T("GripperDark"), RGB(0, 91, 158));
}

void CXTPStatusBarThemeVisualStudio2012::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	if ((NULL != pDC) && (NULL != pBar))
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), m_clrBackground);
	}
}

void CXTPStatusBarThemeVisualStudio2012::DrawStatusBarPaneBorder(CDC* pDC, CRect rc,
																 CXTPStatusBarPane* pPane,
																 BOOL bGripperPane)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	UNUSED_ALWAYS(pPane);
	UNUSED_ALWAYS(bGripperPane);
}

void CXTPStatusBarThemeVisualStudio2012::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	CPoint pt(rcClient.BottomRight());
	pt.x -= XTP_DPI_X(6);
	pt.y -= XTP_DPI_Y(6);

	COLORREF clr = m_clrGripperDark;
	int diffX = XTP_DPI_X(1), diffY = XTP_DPI_Y(1);
	CRect rcDot; // CRect(a, b, a + diffX, b + diffY);

	for (int i = 1; i >= 0; --i)
	{
		for (int a = 0, c = 4; a < 4; ++a, --c)
		{
			rcDot.left  = pt.x - a * XTP_DPI_X(2);
			rcDot.right = rcDot.left + diffX;
			for (int b = 0; b < c; ++b)
			{
				rcDot.top	= pt.y - b * XTP_DPI_Y(2);
				rcDot.bottom = rcDot.top + diffY;
				pDC->FillSolidRect(rcDot, clr);
			}
		}
		clr = m_clrGripperLite;
		pt.x += diffX;
		pt.y += diffY;
	}
}
