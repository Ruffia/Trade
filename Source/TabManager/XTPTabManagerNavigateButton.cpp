// XTPTabManagerNavigateButton.cpp
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

#include "StdAfx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerNavigateButton

CXTPTabManagerNavigateButton::CXTPTabManagerNavigateButton(CXTPTabManager* pManager, UINT nID,
														   XTPTabNavigateButtonFlags dwFlags)
{
	m_pManager = pManager;
	m_nID	  = nID;
	m_dwFlags  = dwFlags;

	m_rcButton.SetRectEmpty();

	m_bEnabled = TRUE;
	m_bPressed = FALSE;
	m_pItem	= NULL;
}

CXTPTabManagerNavigateButton::~CXTPTabManagerNavigateButton()
{
	if (m_pManager->m_pHighlightedNavigateButton == this)
		m_pManager->m_pHighlightedNavigateButton = NULL;
}

CSize CXTPTabManagerNavigateButton::GetSize() const
{
	return m_pManager->GetPaintManager()->m_szNavigateButton;
}

void CXTPTabManagerNavigateButton::AdjustWidth(int& nWidth)
{
	if (m_pManager->IsNavigateButtonVisible(this))
	{
		if ((m_pManager->GetPosition() == xtpTabPositionTop)
			|| (m_pManager->GetPosition() == xtpTabPositionBottom))
		{
			nWidth -= GetSize().cx;
		}
		else
		{
			nWidth -= GetSize().cy;
		}
	}
}

void CXTPTabManagerNavigateButton::SetRect(CRect rcButton)
{
	m_rcButton = rcButton;
}

void CXTPTabManagerNavigateButton::Reposition(CRect& rcNavigateButtons)
{
	if (m_pManager->IsNavigateButtonVisible(this))
	{
		CSize szButton = GetSize();

		if (m_pManager->IsHorizontalPosition())
		{
			m_rcButton.SetRect(rcNavigateButtons.right - szButton.cx,
							   rcNavigateButtons.CenterPoint().y + szButton.cy / 2 - szButton.cy,
							   rcNavigateButtons.right,
							   rcNavigateButtons.CenterPoint().y + szButton.cy / 2);
			rcNavigateButtons.right -= szButton.cx;
		}
		else
		{
			m_rcButton.SetRect(rcNavigateButtons.CenterPoint().x - szButton.cx / 2,
							   rcNavigateButtons.bottom - szButton.cy,
							   rcNavigateButtons.CenterPoint().x - szButton.cx / 2 + szButton.cx,
							   rcNavigateButtons.bottom);
			rcNavigateButtons.bottom -= szButton.cy;
		}
	}
	else
		m_rcButton.SetRectEmpty();
}

void CXTPTabManagerNavigateButton::Draw(CDC* pDC)
{
	if (!m_rcButton.IsRectEmpty())
	{
		CRect rc(m_rcButton);
		m_pManager->GetPaintManager()->DrawNavigateButton(pDC, this, rc);
	}
}

void CXTPTabManagerNavigateButton::PerformClick(HWND hWnd, CPoint pt)
{
	if ((::GetCapture() != NULL) || !m_bEnabled)
		return;

	::SetCapture(hWnd);

	BOOL bClick = FALSE;

	DWORD dwStart = GetTickCount();
	for (;;)
	{
		if (m_bEnabled && GetTickCount() - dwStart > 20)
		{
			dwStart = GetTickCount();
			OnExecute(TRUE);
		}

		BOOL bPressed = m_rcButton.PtInRect(pt);

		if (bPressed != m_bPressed)
		{
			m_bPressed = bPressed;
			m_pManager->RedrawControl(m_rcButton, TRUE);
		}
		MSG msg;

		if (!::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			continue;

		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (::GetCapture() != hWnd)
		{
			DispatchMessage(&msg);
			goto ExitLoop;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
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
	m_pManager->PerformMouseMove(hWnd, pt);
	m_pManager->RedrawControl(NULL, FALSE);

	if (bClick)
	{
		OnExecute(FALSE);
	}
}

void CXTPTabManagerNavigateButton::OnExecute(BOOL bTick)
{
	if (!bTick)
	{
		m_pManager->OnNavigateButtonClick(this);
	}
}

BOOL CXTPTabManagerNavigateButton::IsHighlighted() const
{
	return (m_pManager->m_pHighlightedNavigateButton == this);
}

BOOL CXTPTabManagerNavigateButton::IsOwnerHighlighted() const
{
	return (m_pManager->m_pHighlighted == m_pItem);
}

BOOL CXTPTabManagerNavigateButton::IsOwnerSelected() const
{
	return (m_pManager->m_pSelected == m_pItem);
}

BOOL CXTPTabManagerNavigateButton::IsOwnerNull() const
{
	return (m_pItem == NULL);
}

BOOL CXTPTabManagerNavigateButton::IsOwnerActive() const
{
	return m_pManager->IsActive();
}
