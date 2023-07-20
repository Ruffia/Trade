// XTPMouseManager.cpp : implementation of the CXTPMouseManager class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPMouseManager.h"
#include "CommandBars/XTPCommandBar.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThreadLocal<CXTPMouseManager> CXTPMouseManager::_xtpMouseThreadState;

#define TID_MOUSELEAVE 113341

//---------------------------------------------------------------------------
// CTrackArray
//---------------------------------------------------------------------------
int CXTPMouseManager::CTrackArray::Find(const CXTPCommandBar* pCommandBar) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (pCommandBar == GetAt(i))
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------
// CTrustedArray
//---------------------------------------------------------------------------
int CXTPMouseManager::CTrustedArray::Find(const HWND hWnd) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (hWnd == GetAt(i))
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------
// CXTPMouseManager
//---------------------------------------------------------------------------
CXTPMouseManager::CXTPMouseManager()
{
	m_hHookMouse					  = 0;
	m_nLock							  = 0;
	m_hwndLeave						  = 0;
	m_bForceExpanded				  = FALSE;
	m_pSelected						  = 0;
	m_bIgnoreLButtonUp				  = FALSE;
	m_bSkipTrackedMouseMoveDuplicates = FALSE;

#ifdef _AFXDLL
	m_pModuleState = 0;
#endif
	SetupHook(TRUE);
}

CXTPMouseManager::~CXTPMouseManager()
{
	SetupHook(FALSE);
}

void CXTPMouseManager::CTrustedArray::Remove(HWND hWnd)
{
	int i = Find(hWnd);
	if (i != -1)
		RemoveAt(i);
}

void CXTPMouseManager::DeliverMessage(CXTPCommandBar* pCapture, WPARAM wParam, POINT pt)
{
	if (!pCapture->GetSafeHwnd())
		return;

	switch (wParam)
	{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			if (pCapture->m_nLockRecurse <= 0)
			{
				pCapture->ScreenToClient(&pt);
				pCapture->OnMouseMove(0, pt);
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN: SendTrackLost(); break;

		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			if (!IsMouseLocked())
				SendTrackLost();
			break;
	}
}

CXTPCommandBar* CXTPMouseManager::HitTest(POINT point) const
{
	HWND hWndMouse				  = ::WindowFromPoint(point);
	const CTrackArray& arrTracked = m_arrTracked;
	int i;

	for (i = (int)arrTracked.GetSize() - 1; i >= 0; i--)
	{
		CXTPCommandBar* pCommandBar = arrTracked[i];

		if (!pCommandBar->GetSafeHwnd())
			continue;

		int nHtCode = pCommandBar->OnMouseHitTest(point);

		if ((nHtCode != HTERROR)
			&& ((nHtCode == HTCLIENT) || ::IsChild(pCommandBar->GetSafeHwnd(), hWndMouse)))
		{
			return pCommandBar;
		}
	}

	return NULL;
}

BOOL CXTPMouseManager::PreviewTackLost(CXTPCommandBar* pContextMenu, MSG* msg) const
{
	if (!(msg->message == WM_LBUTTONDOWN || msg->message == WM_RBUTTONDOWN
		  || msg->message == WM_MBUTTONDOWN || msg->message == WM_NCLBUTTONDOWN
		  || msg->message == WM_NCRBUTTONDOWN))
	{
		return FALSE;
	}

	CPoint point;
	GetCursorPos(&point);

	CXTPCommandBar* pCommandBar = HitTest(point);
	if (pCommandBar)
	{
		if (pCommandBar->m_nLockRecurse == 0 || pContextMenu->m_nLockRecurse != 0)
			return FALSE;
	}

	HWND hWndMouse = ::WindowFromPoint(point);

	if (m_arrTracked.GetSize() > 0)
	{
		const CTrustedArray& arrTrusted = m_arrTrusted;
		for (int i = 0; i < arrTrusted.GetSize(); i++)
		{
			HWND hWnd = arrTrusted[i];
			if (IsWindow(hWnd) && IsWindowVisible(hWnd) && CXTPWindowRect(hWnd).PtInRect(point))
			{
				return FALSE;
			}
		}

		if (hWndMouse)
		{
			if (GetClassLong(hWndMouse, GCL_STYLE) & CS_IME)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CXTPMouseManager::IsRelated(HWND hWndParent, HWND hWnd) const
{
	hWnd = ::GetParent(hWnd);

	while (hWnd)
	{
		if (hWnd == hWndParent)
			return TRUE;

		hWnd = ::GetParent(hWnd);
	}

	return FALSE;
}

BOOL CXTPMouseManager::PreTranslateMouseEvents(WPARAM wParam, POINT point)
{
	HWND hWndMouse			= ::WindowFromPoint(point);
	CTrackArray& arrTracked = m_arrTracked;
	int i;

	for (i = (int)arrTracked.GetSize() - 1; i >= 0; i--)
	{
		CXTPCommandBar* pCommandBar = arrTracked[i];

		if (!pCommandBar->GetSafeHwnd())
			continue;

		int nHtCode = pCommandBar->OnMouseHitTest(point);

		if ((nHtCode != HTERROR)
			&& ((nHtCode == HTCLIENT) || ::IsChild(pCommandBar->GetSafeHwnd(), hWndMouse)
				|| IsRelated(pCommandBar->GetSafeHwnd(), hWndMouse)))
		{
			if (pCommandBar->m_nLockRecurse > 0 && wParam != WM_MOUSEMOVE
				&& wParam != WM_NCMOUSEMOVE && !m_bIgnoreLButtonUp)
			{
				SendTrackLostRecurse();
				m_ptTrackedPos = CPoint();
			}
			return FALSE;
		}
	}

	if (arrTracked.GetSize() > 0)
	{
		CTrustedArray& arrTrusted = m_arrTrusted;
		for (i = 0; i < arrTrusted.GetSize(); i++)
		{
			HWND hWnd = arrTrusted[i];
			if (IsWindow(hWnd) && IsWindowVisible(hWnd) && CXTPWindowRect(hWnd).PtInRect(point))
			{
				return FALSE;
			}
		}
	}

	if (hWndMouse && arrTracked.GetSize() > 0)
	{
		if (GetClassLong(hWndMouse, GCL_STYLE) & CS_IME)
			return FALSE;
	}

	for (i = 0; i < arrTracked.GetSize(); i++)
	{
		DeliverMessage(arrTracked.GetAt(i), wParam, point);
	}

	return FALSE;
}

LRESULT CALLBACK CXTPMouseManager::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CXTPMouseManager* pMouseManager = XTPMouseManager();

	if (nCode == HC_ACTION)
	{
		if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
		{
			// Reset m_bIgnoreLButtonUp due to a new click event.
			pMouseManager->m_bIgnoreLButtonUp = FALSE;
		}

		if ((wParam == WM_LBUTTONUP || wParam == WM_NCLBUTTONUP)
			&& pMouseManager->m_bIgnoreLButtonUp)
		{
			// Eat LBUTTONUP message.
			pMouseManager->m_bIgnoreLButtonUp = FALSE;
			return TRUE;
		}

		PMOUSEHOOKSTRUCT pHook = reinterpret_cast<PMOUSEHOOKSTRUCT>(lParam);
		ASSERT(NULL != pHook);

		// It may be necessary to eliminate all duplicated tracked mouse movements.
		if (pMouseManager->m_bSkipTrackedMouseMoveDuplicates)
		{
			if (pMouseManager->m_arrTracked.GetSize() != 0
				&& (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE)
				&& pMouseManager->m_ptTrackedPos == pHook->pt)
			{
				return TRUE;
			}

			pMouseManager->m_ptTrackedPos = pHook->pt;
		}

		// Pass mouse event to the trackers.
		if (0 < pMouseManager->m_arrTracked.GetSize())
		{
			SAFE_MANAGE_STATE(pMouseManager->m_pModuleState);
			if (pMouseManager->PreTranslateMouseEvents(wParam, pHook->pt))
			{
				return TRUE;
			}
		}
	}

	return CallNextHookEx(pMouseManager->m_hHookMouse, nCode, wParam, lParam);
}

void CXTPMouseManager::SetupHook(BOOL bHook)
{
	if (bHook && m_hHookMouse == 0)
	{
		m_hHookMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId());
	}
	if (!bHook && m_hHookMouse)
	{
		UnhookWindowsHookEx(m_hHookMouse);
		m_hHookMouse = 0;
	}
}

void CXTPMouseManager::SetTrack(CXTPCommandBar* pTrack, BOOL /*bCaptureMouse*/)
{
#ifdef _AFXDLL
	m_pModuleState = AfxGetModuleState();
#endif

	if (m_arrTracked.GetSize() == 0 && m_pSelected && m_pSelected != pTrack)
		m_pSelected->OnTrackEnter();

	if (m_arrTracked.Find(pTrack) == -1)
		m_arrTracked.Add(pTrack);
}

void CXTPMouseManager::RemoveTrack(CXTPCommandBar* pTrack)
{
	int nIndex = m_arrTracked.Find(pTrack);
	if (nIndex != -1)
	{
		m_arrTracked.RemoveAt(nIndex);
	}

	if (m_arrTracked.GetSize() == 0)
	{
		RefreshCursor();
	}

	m_bIgnoreLButtonUp = FALSE;
}

void AFX_CDECL CXTPMouseManager::RefreshCursor()
{
	XTPMouseManager()->m_ptTrackedPos = CPoint(-1, -1);

	CPoint pt;
	GetCursorPos(&pt);
	SetCursorPos(pt.x, pt.y);
}

void CXTPMouseManager::SendTrackLost()
{
	while (m_arrTracked.GetSize() > 0)
	{
		m_arrTracked[0]->OnTrackLost();
	}
}

BOOL CXTPMouseManager::IsTopParentActive(HWND hWnd)
{
	return CXTPDrawHelpers::IsTopParentActive(hWnd);
}

void CALLBACK CXTPMouseManager::TrackMouseTimerProc(HWND hWnd, UINT /*uMsg*/, UINT_PTR idEvent,
													DWORD /*dwTime*/)
{
	RECT rect;
	POINT pt;

	if (!IsWindow(hWnd))
	{
		KillTimer(hWnd, idEvent);
		XTPMouseManager()->m_hwndLeave = 0;
		return;
	}
	GetWindowRect(hWnd, &rect);
	::GetCursorPos(&pt);

	BOOL bTopParentActive = GetParent(hWnd) == 0 || (GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP)
							|| IsTopParentActive(hWnd);
	if (!::PtInRect(&rect, pt) || !bTopParentActive)
	{
		KillTimer(hWnd, idEvent);
		XTPMouseManager()->m_hwndLeave = 0;

		::PostMessage(hWnd, WM_MOUSELEAVE, 0, 0);
	}
}

void CXTPMouseManager::TrackMouseLeave(HWND hwnd)
{
	if (m_hwndLeave == hwnd)
		return;
	if (m_hwndLeave && hwnd != m_hwndLeave && IsWindow(m_hwndLeave))
		SendMessage(m_hwndLeave, WM_MOUSELEAVE, 0, 0);
	m_hwndLeave = hwnd;
	::SetTimer(hwnd, TID_MOUSELEAVE, 50, TrackMouseTimerProc);
}

void CXTPMouseManager::AddTrustedWindow(HWND hWnd)
{
	m_arrTrusted.Add(hWnd);
}

void CXTPMouseManager::RemoveTrustedWindow(HWND hWnd)
{
	m_arrTrusted.Remove(hWnd);
}

void CXTPMouseManager::LockTrackRecurse(BOOL bLockTrack)
{
	for (int i = 0; i < (int)m_arrTracked.GetSize(); i++)
	{
		m_arrTracked[i]->m_nLockRecurse += bLockTrack ? +1 : -1;
	}
}

CXTPCommandBar* CXTPMouseManager::GetTopTracked() const
{
	if (m_arrTracked.GetSize() == NULL)
		return NULL;

	return m_arrTracked.GetAt(m_arrTracked.GetSize() - 1);
}

BOOL CXTPMouseManager::IsTrackedLock(const CXTPCommandBar* pItem /*= NULL*/)
{
	if (m_arrTracked.GetSize() == NULL)
		return FALSE;

	if (pItem == NULL)
		return TRUE;

	if (pItem->m_nLockRecurse > 0)
		return TRUE;

	return m_arrTracked.Find(pItem) == -1;
}

BOOL CXTPMouseManager::IsTrackedLockRecurse() const
{
	if (m_arrTracked.GetSize() == 0)
		return FALSE;

	return m_arrTracked[0]->m_nLockRecurse > 0;
}

void CXTPMouseManager::SendTrackLostRecurse()
{
	if (m_arrTracked.GetSize() == 0)
		return;

	BOOL bLocked = m_arrTracked[0]->m_nLockRecurse > 0;

	if (bLocked)
		for (int i = 0; i < (int)m_arrTracked.GetSize(); i++)
		{
			if (m_arrTracked[i]->m_nLockRecurse == 0)
				m_arrTracked[i]->OnTrackLost();
		}
}
