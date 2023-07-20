// XTPEditThemeOfficeXP.cpp: implementation of the CXTPEditThemeOfficeXP class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2000.h"
#include "Controls/Edit/Themes/XTPEditThemeOfficeXP.h"
#include "Controls/Edit/XTPEdit.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditThemeOfficeXP::CXTPEditThemeOfficeXP()
{
	m_bFlatStyle = TRUE;
	m_nTheme	 = xtpControlThemeOfficeXP;
}

CXTPEditThemeOfficeXP::~CXTPEditThemeOfficeXP()
{
}

void CXTPEditThemeOfficeXP::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditThemeDefault::RefreshMetrics(pEdit);

	m_clrBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrBorderDisabled.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrBorderHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
}

void CXTPEditThemeOfficeXP::DrawNcBorders(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit)
{
	if (!CXTPEditThemeDefault::DrawNcBordersDefault(pDC, rcWnd, pEdit))
	{
		CBrush* pBrush = GetClientBrush(pDC, pEdit);

		// draw the outer border.

		COLORREF clrBorder = 0;
		if (pEdit->IsWindowEnabled())
		{
			if (pEdit->IsHighlighted() || pEdit->IsFocused())
			{
				clrBorder = m_clrBorderHilite;
			}
			else
			{
				clrBorder = m_clrBorder;
			}
		}
		else
		{
			clrBorder = m_clrBorderDisabled;
		}

		pDC->Draw3dRect(rcWnd, clrBorder, clrBorder);

		// draw the inner border the same color as the client background brush.

		rcWnd.DeflateRect(1, 1);
		DrawFrame(pDC, rcWnd, 1, pBrush);
	}
}

BOOL CXTPEditThemeOfficeXP::AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignemnt)
{
	return CXTPEditTheme::AdjustBuddyRect(pBuddy, rcUpDown, nAlignemnt);
}

void CXTPEditThemeOfficeXP::DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignemnt,
											 CXTPEdit* pEdit)
{
	UNREFERENCED_PARAMETER(pBuddy);

	if (!CXTPEditThemeDefault::DrawBuddyBorders(pDC, rc, nAlignemnt, pEdit))
	{
	}
}

BOOL CXTPEditThemeOfficeXP::RedrawFocusedFrame() const
{
	return FALSE;
}
