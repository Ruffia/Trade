// XTPTabCtrlButtons.cpp : implementation of the CXTPTabCtrlButtons class.
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
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Tab/XTPTabCtrlButtons.h"
#include "Controls/Tab/XTPTabBaseTheme.h"
#include "Controls/Tab/XTPTabBase.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DB_LEFT 0x0002
#define DB_RIGHT 0x0004
#define DB_CROSS 0x0008

#define DB_ENABLED 0x0000
#define DB_DISABLED 0x0100
#define DB_PRESSED 0x1000
#define DB_OVER 0x4000

#define DB_ISENABLED(Style) (!((Style)&DB_DISABLED))
#define DB_ISPRESSED(Style) (((Style) & (DB_PRESSED)) != 0)
#define DB_ISOVER(Style) (((Style)&DB_OVER) == DB_OVER)

void CXTPTabCtrlButton::SetEnable(BOOL bEnable)
{
	if (!bEnable)
		m_wStyle |= DB_DISABLED;
	else
		m_wStyle &= ~DB_DISABLED;
}

void CXTPTabCtrlButton::Draw(CDC* pDC, COLORREF clrButton)
{
	m_pButtons->m_pTabCtrlBase->GetTheme()->DrawButton(pDC, this, clrButton);
}

DWORD CXTPTabCtrlButton::Click(CXTPTabCtrlButtons* pWnd, CPoint pt, BOOL bRepeat)
{
	if ((::GetCapture() != NULL) || !DB_ISENABLED(m_wStyle))
	{
		return FALSE;
	}

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());

	BOOL bDown = FALSE, bClick = FALSE;
	CRect rcBtn(m_Rect);

	UINT_PTR nTimer = (bRepeat == FALSE) ? 0 : SetTimer(pWnd->GetSafeHwnd(), 1, 150, NULL);

	for (;;)
	{
		if (bRepeat && DB_ISENABLED(m_wStyle))
		{
			ASSERT(m_pTabCtrl);
			if (!m_pTabCtrl)
				return FALSE;

			int nCount = m_pTabCtrl->GetItemCount();

			CRect rc;
			m_pTabCtrl->GetItemRect(-1, rc);
			TCHITTESTINFO hi = { { XTP_DPI_X(5), rc.CenterPoint().y } };
			int nFirst		 = m_pTabCtrl->HitTest(&hi);

			if (m_wStyle & DB_LEFT)
			{
				if (nFirst > 0)
					m_pTabCtrl->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nFirst - 1),
											0);
			}
			else if (nCount > 0)
			{
				m_pTabCtrl->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nFirst + 1), 0);
			}

			m_pTabCtrl->Invalidate(FALSE);
			pWnd->Invalidate(FALSE);
		}

		BOOL bNewDown = ::PtInRect(rcBtn, pt) != 0;
		if (bNewDown != bDown)
		{
			bDown = bNewDown;
			if (bDown)
				m_wStyle = m_wStyle | DB_PRESSED;
			else
				m_wStyle &= ~DB_PRESSED;

			pWnd->InvalidateRect(m_Rect, FALSE);
		}
		MSG msg;

		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
		{
			DispatchMessage(&msg);
			goto ExitLoop;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
				break;

			case WM_LBUTTONUP: bClick = bDown; goto ExitLoop;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_CANCELMODE:
			case WM_RBUTTONDOWN: goto ExitLoop;

			default: DispatchMessage(&msg); break;
		}
	}
ExitLoop:
	ReleaseCapture();

	if (nTimer)
		KillTimer(*pWnd, nTimer);

	m_wStyle &= ~DB_PRESSED;

	CheckForMouseOver(pWnd, pt);
	pWnd->InvalidateRect(m_Rect, FALSE);

	return XTPToDWORDChecked(bClick);
}

void CXTPTabCtrlButton::CheckForMouseOver(CWnd* pWnd, CPoint pt)
{
	if (!PtInRect(pt) && DB_ISOVER(m_wStyle))
	{
		m_wStyle &= ~DB_OVER;
		pWnd->InvalidateRect(m_Rect, FALSE);
	}

	if (!DB_ISOVER(m_wStyle) && PtInRect(pt) && DB_ISENABLED(m_wStyle))
	{
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, *pWnd, 0 };
		_TrackMouseEvent(&tme);
		m_wStyle |= DB_OVER;

		pWnd->InvalidateRect(m_Rect, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabCtrlButtons

CXTPTabCtrlButtons::CXTPTabCtrlButtons()
	: m_btnClose(DB_CROSS)
	, m_btnLeft(DB_LEFT)
	, m_btnRight(DB_RIGHT)
{
	m_dwFlags		 = xtpNavBtnArrows;
	m_bShow			 = FALSE;
	m_hwndSubclassed = 0;
	m_hwndPrimary	= 0;
	m_pTabCtrlBase   = 0;
	m_pTabCtrl		 = 0;

	m_btnRight.m_pButtons = m_btnClose.m_pButtons = m_btnLeft.m_pButtons = this;
}

CXTPTabCtrlButtons::~CXTPTabCtrlButtons()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTabCtrlButtons, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPTabCtrlButtons::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(m_pTabCtrl);
	CXTPClientRect rcClient(this);
	CXTPBufferDC dc(dcPaint, rc);

	int nCount = m_pTabCtrl->GetItemCount();
	if (!m_bShow || nCount == 0)
	{
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
		return;
	}

	COLORREF clrText = m_pTabCtrlBase->GetTheme()->FillButtons(&dc, this);

	CRect rcItem;
	m_pTabCtrl->GetItemRect(-1, &rcItem);

	int nTop   = (rcItem.Height() - XTP_DPI_Y(13)) / 2;
	int nRight = rcClient.right - XTP_DPI_X(5);

	if (m_dwFlags & xtpNavBtnClose)
	{
		m_btnClose.SetRect(CRect(nRight - XTP_DPI_X(14), nTop, nRight, nTop + XTP_DPI_Y(15)));
		m_btnClose.Draw(&dc, clrText);
		nRight -= XTP_DPI_X(14);
	}
	else
		m_btnClose.SetRect(CXTPEmptyRect());

	if (m_dwFlags & xtpNavBtnArrows)
	{
		ASSERT(m_pTabCtrl);
		if (!m_pTabCtrl)
			return;

		BOOL bEnableRight = FALSE, bEnableLeft = FALSE;
		if (nCount > 0)
		{
			CRect rcLastItem;
			m_pTabCtrl->GetItemRect(nCount - 1, &rcLastItem);
			bEnableRight = rcLastItem.right > rc.Width() - GetWidth();

			m_pTabCtrl->GetItemRect(0, &rcLastItem);
			bEnableLeft = rcLastItem.left < 0;
		}

		m_btnRight.SetRect(CRect(nRight - XTP_DPI_X(14), nTop, nRight, nTop + XTP_DPI_Y(15)));
		m_btnRight.SetEnable(bEnableRight);
		m_btnRight.Draw(&dc, clrText);

		m_btnLeft.SetRect(
			CRect(nRight - XTP_DPI_X(28), nTop, nRight - XTP_DPI_X(14), nTop + XTP_DPI_Y(15)));
		m_btnLeft.SetEnable(bEnableLeft);
		m_btnLeft.Draw(&dc, clrText);
	}
	else
	{
		m_btnRight.SetRect(CXTPEmptyRect());
		m_btnLeft.SetRect(CXTPEmptyRect());
	}
}
BOOL CXTPTabCtrlButtons::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

int CXTPTabCtrlButtons::GetWidth()
{
	return XTP_DPI_X(10) + (m_dwFlags & xtpNavBtnClose ? XTP_DPI_X(14) : 0)
		   + (m_dwFlags & xtpNavBtnArrows ? XTP_DPI_X(28) : 0);
}

BOOL CXTPTabCtrlButtons::Create(CXTPTabBase* pCtrlBase)
{
	m_pTabCtrlBase = pCtrlBase;

	m_pTabCtrl = m_btnRight.m_pTabCtrl = m_btnClose.m_pTabCtrl = m_btnLeft.m_pTabCtrl =
		pCtrlBase->GetTabCtrlImpl();

	return TRUE;
}

CRect CXTPTabCtrlButtons::AdjustRect()
{
	CRect rcItem;
	m_pTabCtrl->GetItemRect(-1, &rcItem);

	CXTPClientRect rc(m_pTabCtrl);
	rc.left   = rc.right - GetWidth();
	rc.bottom = rcItem.bottom + XTP_DPI_Y(2);

	rc.top = rcItem.top + ((m_pTabCtrl->GetStyle() & TCS_BOTTOM) != 0 ? XTP_DPI_Y(-2) : 0);

	return rc;
}

void CXTPTabCtrlButtons::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShow)
	{
		if (m_btnClose.PtInRect(point) && m_btnClose.Click(this, point))
		{
			m_pTabCtrl->SendMessage(WM_COMMAND, XTP_IDC_TAB_CLOSE);
		}
		else if (m_btnLeft.PtInRect(point))
		{
			m_btnLeft.Click(this, point, TRUE);
		}
		else if (m_btnRight.PtInRect(point))
		{
			m_btnRight.Click(this, point, TRUE);
		}
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

BOOL CXTPTabCtrlButtons::ShowButtons()
{
	return m_bShow;
}
void CXTPTabCtrlButtons::ShowButtons(BOOL bShow)
{
	if (m_bShow != bShow)
	{
		m_bShow = bShow;

		if (m_hWnd && !m_bShow)
			ShowWindow(SW_HIDE);
	}
	if (m_bShow && m_hWnd)
	{
		Refresh();
	}
}

void CXTPTabCtrlButtons::Refresh()
{
	if (!m_hWnd)
		return;

	if (m_bShow)
	{
		CRect rc = AdjustRect();
		SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(),
					 UINT(m_pTabCtrl->GetItemCount() > 0 ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		OnMouseMove(0, point);
	}
	Invalidate(FALSE);
}

void CXTPTabCtrlButtons::OnMouseMove(UINT, CPoint point)
{
	if (m_bShow)
	{
		m_btnClose.CheckForMouseOver(this, point);
		m_btnLeft.CheckForMouseOver(this, point);
		m_btnRight.CheckForMouseOver(this, point);
	}
}

void CXTPTabCtrlButtons::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPTabCtrlButtons::SubclassTabButtons(CWnd* pWnd)
{
	if (pWnd && pWnd->GetSafeHwnd() && m_hwndSubclassed != pWnd->GetSafeHwnd())
	{
		UnSubclassTabButtons();
		SubclassWindow(*pWnd);

		m_hwndSubclassed = pWnd->GetSafeHwnd();
	}
	else
	{
		if (!m_hWnd)
		{
			VERIFY(CWnd::Create(AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
								0, WS_CHILD | WS_VISIBLE, CXTPEmptyRect(), m_pTabCtrl, 0));
			m_hwndPrimary = m_hWnd;
		}
	}
}
void CXTPTabCtrlButtons::UnSubclassTabButtons()
{
	if (m_hWnd)
	{
		HWND hWnd = UnsubclassWindow();
		if (hWnd == m_hwndPrimary)
		{
			::DestroyWindow(hWnd);
		}
	}
	m_hwndSubclassed = NULL;
}

void CXTPTabCtrlButtons::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	static BOOL bOnWindowPosChanged = FALSE;

	if (m_bShow && !bOnWindowPosChanged)
	{
		if (m_pTabCtrl->GetItemCount() == 0)
			return;

		bOnWindowPosChanged = TRUE;
		CRect rc			= AdjustRect();
		SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		bOnWindowPosChanged = FALSE;
	}
}

void CXTPTabCtrlButtons::SetButtonStyle(DWORD dwFlags)
{
	m_dwFlags = dwFlags;
}
