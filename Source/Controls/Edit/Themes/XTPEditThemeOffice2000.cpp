// XTPEditThemeOffice2000.cpp: implementation of the CXTPEditThemeOffice2000 class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2000.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditThemeOffice2000::CXTPEditThemeOffice2000()
{
	m_nTheme	   = xtpControlThemeOffice2000;
	m_bUseWinTheme = FALSE;
}

CXTPEditThemeOffice2000::~CXTPEditThemeOffice2000()
{
}

void CXTPEditThemeOffice2000::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditThemeDefault::RefreshMetrics(pEdit);

	m_clrBorder.SetStandardValue(GetXtremeColor(COLOR_BTNHIGHLIGHT),
								 GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPEditThemeOffice2000::DrawNcBorders(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit)
{
	if (!CXTPEditThemeDefault::DrawNcBordersDefault(pDC, rcWnd, pEdit))
	{
		// draw the outer border.
		pDC->Draw3dRect(rcWnd, m_clrBorder.clrDark, m_clrBorder.clrLight);

		// draw the inner border.
		rcWnd.DeflateRect(1, 1);
		pDC->Draw3dRect(rcWnd, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNFACE));
	}
}

void CXTPEditThemeOffice2000::DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignemnt,
											   CXTPEdit* pEdit)
{
	ASSERT_VALID(pDC);
	UNREFERENCED_PARAMETER(pBuddy);

	if (!CXTPEditThemeDefault::DrawBuddyBorders(pDC, rc, nAlignemnt, pEdit))
	{
		CBrush* pBrush = GetClientBrush(pDC, pEdit);

		pDC->FillRect(rc, pBrush);

		if (nAlignemnt == UDS_ALIGNRIGHT)
		{
			rc.left -= XTP_DPI_X(2);
		}
		else
		{
			rc.right += XTP_DPI_X(2);
		}

		pDC->Draw3dRect(rc, m_clrBorder.clrDark, m_clrBorder.clrLight);
		rc.DeflateRect(0, XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNFACE));
	}
}

BOOL CXTPEditThemeOffice2000::RedrawFocusedFrame() const
{
	return TRUE;
}
