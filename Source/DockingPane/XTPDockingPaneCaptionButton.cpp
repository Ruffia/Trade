// XTPDockingPaneCaptionButton.cpp : implementation of the CXTPDockingPaneCaptionButton class.
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

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPane.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneCaptionButton::CXTPDockingPaneCaptionButton(int nID, CXTPDockingPaneBase* pPane)
{
	ASSERT(NULL != pPane);

	m_nID	  = nID;
	m_pPane	= pPane;
	m_bPressed = m_bSelected = FALSE;
	m_dwState				 = 0;
	m_rcButton.SetRectEmpty();
}

BOOL CXTPDockingPaneCaptionButton::IsVisible() const
{
	return const_cast<CXTPDockingPaneBase*>(m_pPane)->IsCaptionButtonVisible(
		const_cast<CXTPDockingPaneCaptionButton*>(this));
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawPinButton(CDC* pDC, CPoint pt, BOOL bPinned,
														   COLORREF clr /*= COLORREF_NULL*/)
{
	clr = NormalizeButtonColor(pDC, clr);

	CSize half(XTP_DPI_X(8), XTP_DPI_Y(8));
	CRect rc(pt.x - half.cx, pt.y - half.cy, pt.x + half.cx, pt.y + half.cy);
	DrawPinButton(pDC, rc, bPinned, clr);
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawPinButton(CDC* pDC, CRect rc, BOOL bPinned,
														   COLORREF clr /*= COLORREF_NULL*/)
{
	clr = NormalizeButtonColor(pDC, clr);
	XTPPrimitiveDrawer()->DrawSymbol(pDC,
									 bPinned ? xtpPrimitiveSymbolPinLeft
											 : xtpPrimitiveSymbolPinDown,
									 rc, clr);
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(
	CDC* pDC, CPoint pt, BOOL bMaximize, COLORREF clr /*= COLORREF_NULL*/)
{
	clr = NormalizeButtonColor(pDC, clr);

	CSize half(XTP_DPI_X(8), XTP_DPI_Y(8));
	CRect rc(pt.x - half.cx, pt.y - half.cy, pt.x + half.cx, pt.y + half.cy);
	DrawMaximizeRestoreButton(pDC, rc, bMaximize, clr);
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(
	CDC* pDC, CRect rc, BOOL bMaximize, COLORREF clr /*= COLORREF_NULL*/)
{
	clr = NormalizeButtonColor(pDC, clr);
	XTPPrimitiveDrawer()->DrawSymbol(pDC,
									 bMaximize ? xtpPrimitiveSymbolMaximize
											   : xtpPrimitiveSymbolRestore,
									 rc, clr);
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawCloseButton(CDC* pDC, CRect rc,
															 COLORREF clr /*= COLORREF_NULL*/)
{
	clr = NormalizeButtonColor(pDC, clr);
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolClose, rc, clr);
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawMenuButton(CDC* pDC, CRect rc,
															COLORREF clr /*= COLORREF_NULL*/)
{
	clr = NormalizeButtonColor(pDC, clr);
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rc, clr);
}

void CXTPDockingPaneCaptionButton::InvalidateRect()
{
	m_pPane->RedrawPane();
}

BOOL CXTPDockingPaneCaptionButton::IsEnabled() const
{
	CXTPDockingPane* pPane = CXTPDockingPanePaintManager::GetSelectedPane(GetPane());
	if (pPane && ((pPane->GetEnabled() & xtpPaneEnableActions) == 0))
		return FALSE;

	return TRUE;
}

DWORD CXTPDockingPaneCaptionButton::Click(CWnd* pWnd, CPoint pt, BOOL bClient)
{
	if ((::GetCapture() != NULL))
		return FALSE;

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());

	BOOL bClick = FALSE;
	CRect rcBtn(m_rcButton);

	while (CWnd::GetCapture() == pWnd)
	{
		BOOL bNewDown = ::PtInRect(rcBtn, pt) != 0;
		if (bNewDown != m_bPressed)
		{
			m_bPressed = bNewDown;

			InvalidateRect();
		}

		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
			case WM_NCMOUSEMOVE: break;
			case WM_MOUSEMOVE:
				pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
				if (!bClient)
				{
					pWnd->ClientToScreen(&pt);
					CXTPDrawHelpers::ScreenToWindow(pWnd, &pt);
				}
				break;

			case WM_LBUTTONUP: bClick = m_bPressed; goto ExitLoop;

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

	m_bPressed = FALSE;
	CheckForMouseOver(pt);

	InvalidateRect();

	return XTPToDWORDChecked(bClick);
}

void CXTPDockingPaneCaptionButton::SetRect(CRect rc)
{
	ASSERT(m_pPane);
	if (!m_pPane)
		return;

	if (rc != m_rcButton)
	{
		m_rcButton = rc;
		m_pPane->OnCaptionButtonRectChanged(this);
	}
}

BOOL CXTPDockingPaneCaptionButton::CheckForMouseOver(CPoint pt)
{
	if (!PtInRect(pt) && m_bSelected)
	{
		m_bSelected = FALSE;
		InvalidateRect();
	}

	if (!m_bSelected && PtInRect(pt) && IsEnabled())
	{
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_pPane->GetPaneHwnd() };
		_TrackMouseEvent(&tme);

		m_bSelected = TRUE;

		InvalidateRect();
		return TRUE;
	}
	return FALSE;
}

COLORREF AFX_CDECL CXTPDockingPaneCaptionButton::NormalizeButtonColor(CDC* pDC, COLORREF clr)
{
	ASSERT(NULL != pDC);

	if (COLORREF_NULL == clr)
	{
		clr = pDC->GetTextColor();
	}

	return clr;
}
