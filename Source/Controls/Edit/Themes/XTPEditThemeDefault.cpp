// XTPEditThemeDefault.cpp: implementation of the CXTPEditThemeDefault class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
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

CXTPEditThemeDefault::CXTPEditThemeDefault()
{
	m_nTheme	   = xtpControlThemeDefault;
	m_bUseWinTheme = TRUE;
}

CXTPEditThemeDefault::~CXTPEditThemeDefault()
{
}

void CXTPEditThemeDefault::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditTheme::RefreshMetrics(pEdit);

	m_clrText.SetStandardValue(::GetSysColor(COLOR_BTNTEXT));
	m_clrTextReadOnly.SetStandardValue(::GetSysColor(COLOR_BTNTEXT));
	m_clrBack.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	m_clrBackNonClient.SetStandardValue(::GetSysColor(COLOR_BTNFACE));
	m_clrBorder.SetStandardValue(::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));

	m_xtpBrushBack.DeleteObject();
	m_xtpBrushBack.CreateSolidBrush(m_clrBack);
}

BOOL CXTPEditThemeDefault::DrawNcBordersDefault(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit)
{
	CRect rcIntersect(0, 0, 0, 0);

	// set the buddy (spinner) intersect rect if one exists.

	CWnd* pWndBuddy = pEdit->GetWindow(GW_HWNDNEXT);
	if (pWndBuddy)
	{
		CXTPWindowRect rcSpin(pWndBuddy);
		CXTPWindowRect rcEdit(pEdit);

		if (rcIntersect.IntersectRect(rcSpin, rcEdit))
		{
			rcIntersect.OffsetRect(-rcEdit.TopLeft());
			pDC->ExcludeClipRect(rcIntersect);
		}
		else
		{
			rcIntersect.SetRectEmpty();
		}
	}

	CBrush* pBrush = GetClientBrush(pDC, pEdit);

	if (!pEdit->ShowBorder())
	{
		DrawFrame(pDC, rcWnd, XTP_DPI_X(2), pBrush);
		return TRUE;
	}

	// if windows themes are enabled, use the theme API to draw them.

	if (m_bUseWinTheme && m_winTheme.IsAppThemeActive())
	{
		CRect rcClient(rcWnd.left + XTP_DPI_X(1), rcWnd.top + XTP_DPI_Y(1),
					   rcWnd.right - XTP_DPI_X(1), rcWnd.bottom - XTP_DPI_Y(1));
		pDC->ExcludeClipRect(rcClient);

		if (SUCCEEDED(m_winTheme.DrawThemeBackground(
				*pDC, 0, pEdit->IsWindowEnabled() ? ETS_NORMAL : ETS_DISABLED, &rcWnd, NULL)))
		{
			pDC->SelectClipRgn(NULL);

			if (!rcIntersect.IsRectEmpty())
			{
				pDC->ExcludeClipRect(rcIntersect);
			}

			rcWnd.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			DrawFrame(pDC, rcWnd, XTP_DPI_X(1), pBrush);
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPEditThemeDefault::DrawNcBorders(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit)
{
	if (!DrawNcBordersDefault(pDC, rcWnd, pEdit))
	{
		// draw a default 3D sunken border.

		pDC->DrawEdge(rcWnd, EDGE_SUNKEN, BF_RECT);
	}
}

void CXTPEditThemeDefault::DrawNcScrollBack(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit)
{
	// this will fill the 'square' space in the bottom right of the control when both
	// horizontal and vertical scroll bars are visible.

	if ((pEdit->GetStyle() & (WS_VSCROLL | WS_HSCROLL)) == (WS_VSCROLL | WS_HSCROLL))
	{
		if (pEdit->ShowBorder())
		{
			rcWnd.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));
		}

		rcWnd.SetRect(rcWnd.right - ::GetSystemMetrics(SM_CXVSCROLL),
					  rcWnd.bottom - ::GetSystemMetrics(SM_CYHSCROLL), rcWnd.right, rcWnd.bottom);
		pDC->FillSolidRect(rcWnd, m_clrBackNonClient);
	}
}

CBrush* CXTPEditThemeDefault::CtlColor(CDC* pDC, UINT nCtlColor, CXTPEdit* pEdit)
{
	ASSERT_VALID(pDC);

	UNREFERENCED_PARAMETER(pEdit);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC: // when edit is disabled or read only
		{
			pDC->SetTextColor(m_clrTextReadOnly); // only when edit is read only
			pDC->SetBkColor(m_clrBack);
			return &m_xtpBrushBack;
		}
		case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(m_clrText);
			pDC->SetBkColor(m_clrBack);
			return &m_xtpBrushBack;
		}
		default: return NULL;
	}
}

BOOL CXTPEditThemeDefault::AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignemnt)
{
	if (!CXTPEditTheme::AdjustBuddyRect(pBuddy, rcUpDown, nAlignemnt))
	{
		if (nAlignemnt == UDS_ALIGNRIGHT)
		{
			rcUpDown.DeflateRect(0, XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		}
		else
		{
			rcUpDown.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), 0, XTP_DPI_Y(2));
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPEditThemeDefault::DrawBuddyBorders(CDC* pDC, CRect rc, int nAlignemnt, CXTPEdit* pEdit)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pEdit);

	CBrush* pBrush = GetClientBrush(pDC, pEdit);

	if (m_bFlatStyle)
	{
		return TRUE;
	}

	if (!pEdit->ShowBorder())
	{
		if (pEdit->GetStyle() & WS_BORDER)
		{
			if (nAlignemnt == UDS_ALIGNRIGHT)
			{
				rc.left -= XTP_DPI_X(2);
			}
			else
			{
				rc.right += XTP_DPI_X(2);
			}

			pDC->Draw3dRect(rc, m_clrBorder.clrDark, m_clrBorder.clrLight);
		}

		return TRUE;
	}

	if (m_bUseWinTheme && m_winTheme.IsAppThemeActive())
	{
		pDC->FillRect(rc, pBrush);

		if (nAlignemnt == UDS_ALIGNRIGHT)
		{
			rc.left -= XTP_DPI_X(2);
		}
		else
		{
			rc.right += XTP_DPI_X(2);
		}

		if (SUCCEEDED(m_winTheme.DrawThemeBackground(*pDC, 0, ETS_NORMAL, &rc, NULL)))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPEditThemeDefault::DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignemnt,
											CXTPEdit* pEdit)
{
	ASSERT_VALID(pDC);
	UNREFERENCED_PARAMETER(pBuddy);

	if (!DrawBuddyBorders(pDC, rc, nAlignemnt, pEdit))
	{
		pDC->DrawEdge(rc, EDGE_SUNKEN,
					  UINT((nAlignemnt == UDS_ALIGNRIGHT) ? BF_TOP | BF_BOTTOM | BF_RIGHT
														  : BF_TOP | BF_BOTTOM | BF_LEFT));
	}
}
