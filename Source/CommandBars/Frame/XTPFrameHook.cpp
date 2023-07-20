// XTPFrameHook.cpp : implementation file
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/FrameShadow/XTPFrameShadow.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPDockBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"

#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonPopups.h"
#	include "Ribbon/XTPRibbonBar.h"
#	include "Ribbon/XTPRibbonPaintManager.h"
#endif

#define XTP_TID_MOUSELEAVE 0xACB1
#define XTP_TID_REFRESHFRAME 0xACD1

#ifndef WM_NCMOUSELEAVE
#	define WM_NCMOUSELEAVE 0x02A2
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CXTPCommandBarsFrameHook::m_bAllowDwm				   = TRUE;
BOOL CXTPCommandBarsFrameHook::m_bAllowBorderMouseTracking = FALSE;

class CXTPCommandBarsFrameHook::CControlCaptionButton : public CXTPControl
{
public:
	CControlCaptionButton(CXTPCommandBarsFrameHook* pFrame)
	{
		m_pFrame = pFrame;
	}

public:
	virtual int GetSelected() const
	{
		return m_pFrame->m_pSelected == (CXTPControl*)this;
	}
	virtual BOOL GetPressed() const
	{
		return m_pFrame->m_pSelected == (CXTPControl*)this && m_pFrame->m_bButtonPressed;
	}

protected:
	CXTPCommandBarsFrameHook* m_pFrame;
};

void CXTPCommandBarsFrameHook::AddCaptionButton(int nId, int nHit)
{
	CXTPControl* pButton = new CControlCaptionButton(this);
	pButton->SetID(nId);
	pButton->SetHelpId(nHit);
	m_pCaptionButtons->Add(pButton);
}

UINT CXTPCommandBarsFrameHook::m_nMsgQueryFrameHook = 0;

#define EVENT_OBJECT_REORDER 0x8004

void CALLBACK CXTPCommandBarsFrameHook::WinEventProc(
	CXTPWinEventHook::HWINEVENTHOOK /*hWinEventHook*/, DWORD event, HWND hWnd, LONG idObject,
	LONG /*idChild*/, DWORD /*dwEventThread*/, DWORD /*dwmsEventTime*/)
{
	if (event == EVENT_OBJECT_REORDER && idObject == OBJID_WINDOW)
	{
		if (!XTPHookManager()->Lookup(hWnd))
			return;

		CXTPCommandBarsFrameHook* pFrame = (CXTPCommandBarsFrameHook*)::SendMessage(
			hWnd, CXTPCommandBarsFrameHook::m_nMsgQueryFrameHook, 0, 0);

		if (pFrame && !pFrame->IsDwmEnabled())
		{
			DWORD dwStyle		= pFrame->GetSiteStyle();
			DWORD dwStyleRemove = (WS_CAPTION | WS_VSCROLL | WS_HSCROLL);

			if (dwStyle & dwStyleRemove)
			{
				pFrame->GetSite()->SetTimer(XTP_TID_REFRESHFRAME, 100, NULL);
			}
		}
	}
}

CXTPCommandBarsFrameHook::CXTPCommandBarsFrameHook()
{
	CXTPPaintManager::GetInstanceAddRef();

	m_bBeingDestructed		 = FALSE;
	m_pCommandBars			 = NULL;
	m_pRibbonBar			 = NULL;
	m_pPaintManager			 = NULL;
	m_bUseGlobalPaintManager = FALSE;

	m_bActive			  = TRUE;
	m_szFrameRegion		  = CSize(0, 0);
	m_hwndSite			  = NULL;
	m_pSelected			  = NULL;
	m_bDelayReclalcLayout = FALSE;
	m_bButtonPressed	  = FALSE;
	m_bThemeFrameEnabled  = FALSE;

	m_bToolBarComposition = FALSE;

	m_pShadow = NULL;

	m_bInUpdateFrame = FALSE;
	m_bLockNCPaint   = FALSE;

	m_pSink = new CXTPNotifySink();

	m_nMsgUpdateSkinState = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_UPDATESTATE"));
	m_nMsgQuerySkinState  = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_QUERYSTATE"));
	if (m_nMsgQueryFrameHook == 0)
		m_nMsgQueryFrameHook = RegisterWindowMessage(_T("WM_GETFRAMEHOOK"));

	m_bDwmEnabled = FALSE;

	m_pCaptionButtons = new CXTPControls();
	AddCaptionButton(SC_CLOSE, HTCLOSE);
	AddCaptionButton(SC_RESTORE, HTMAXBUTTON);
	AddCaptionButton(SC_MAXIMIZE, HTMAXBUTTON);
	AddCaptionButton(SC_MINIMIZE, HTMINBUTTON);

	m_pSink->Advise(
		XTPResourceImages()->GetConnection(), XTP_NC_COMMON_RESOURCEIMAGES_CHANGED,
		CreateNotfySinkClassDelegate(this, &CXTPCommandBarsFrameHook::OnResourceImagesChanged));
}

CXTPCommandBarsFrameHook::~CXTPCommandBarsFrameHook()
{
	m_bBeingDestructed = TRUE;
	CMDTARGET_RELEASE(m_pCaptionButtons);
	DisableFrameTheme();
	m_pSink->Delete();

	if (m_pPaintManager)
	{
		m_pPaintManager->Unsubscribe(this);
		m_pPaintManager->InternalRelease();
	}

	CXTPPaintManager::ReleaseInstance();
}

void CXTPCommandBarsFrameHook::OnResourceImagesChanged(XTP_NOTIFY_CODE Event, WPARAM wParam,
													   LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	ASSERT(Event == XTP_NC_COMMON_RESOURCEIMAGES_CHANGED);
	if (Event == XTP_NC_COMMON_RESOURCEIMAGES_CHANGED)
	{
		if (m_hwndSite && IsWindow(m_hwndSite) && m_bThemeFrameEnabled && !m_bDwmEnabled)
		{
			GetPaintManager()->RefreshMetrics();
			RedrawFrame();
		}
	}
}

void CXTPCommandBarsFrameHook::DisableOffice2007Frame()
{
	DisableFrameTheme();
}

void CXTPCommandBarsFrameHook::DisableFrameTheme()
{
	DisableShadow();

	m_pCommandBars = NULL;
	m_pRibbonBar   = NULL;

	if (m_pPaintManager)
	{
		m_pPaintManager->Unsubscribe(this);
		m_pPaintManager->InternalRelease();
		m_pPaintManager = NULL;
	}

	m_pSelected			  = NULL;
	m_bDelayReclalcLayout = FALSE;
	m_bButtonPressed	  = FALSE;
	m_bThemeFrameEnabled  = FALSE;

	XTPHookManager()->RemoveAll(this);

	if (m_hwndSite && !m_bBeingDestructed)
	{
		if (m_bDwmEnabled)
		{
			CXTPWinDwmWrapper().ExtendFrameIntoClientArea(m_hwndSite, 0, 0, 0, 0);
		}

		SendMessage(m_hwndSite, m_nMsgUpdateSkinState, 0, 0);

		SetWindowRgn(m_hwndSite, (HRGN)NULL, TRUE);
		::SetWindowPos(m_hwndSite, 0, 0, 0, 0, 0,
					   SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	SetSiteWindow(NULL);

	m_bDwmEnabled = FALSE;

	SetWinHook();
}

void CXTPCommandBarsFrameHook::EnableFrameTheme(CWnd* pSite, CXTPPaintManager* pPaintManager)
{
	ASSERT(pSite);
	ASSERT(pPaintManager);

	if (!pSite || !pPaintManager)
		return;

	m_pCommandBars = NULL;
	m_pRibbonBar   = NULL;

	if (m_pPaintManager != pPaintManager)
	{
		if (m_pPaintManager)
		{
			m_pPaintManager->Unsubscribe(this);
			m_pPaintManager->InternalRelease();
			m_pPaintManager = NULL;
		}

		if (XTPPaintManager() != pPaintManager)
		{
			m_pPaintManager = pPaintManager;
			m_pPaintManager->Subscribe(this);
			m_pPaintManager->InternalAddRef();
		}
		else
		{
			m_bUseGlobalPaintManager = TRUE;
		}
	}

	SetSiteWindow(pSite->GetSafeHwnd());

	m_bActive		= TRUE;
	m_szFrameRegion = CSize(0, 0);

	m_pSelected = NULL;

	m_bDelayReclalcLayout = FALSE;
	m_bButtonPressed	  = FALSE;

	m_bThemeFrameEnabled = TRUE;

	m_bDwmEnabled = !m_bAllowDwm || pSite->GetStyle() & WS_CHILD
						? FALSE
						: CXTPWinDwmWrapper().IsCompositionEnabled();

	XTPHookManager()->SetHook(m_hwndSite, this);
	SendMessage(m_hwndSite, m_nMsgUpdateSkinState, 0, 0);

	if (!m_bDwmEnabled)
	{
		RefreshFrameStyle();
	}

	::SetWindowPos(m_hwndSite, 0, 0, 0, 0, 0,
				   SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	SetWinHook();

	EnableShadow();
}

void CXTPCommandBarsFrameHook::EnableFrameTheme(CWnd* pSite, CXTPCommandBars* pCommandBars)
{
	EnableFrameTheme(pSite, pCommandBars->GetPaintManager());
	m_pCommandBars = pCommandBars;
}

void CXTPCommandBarsFrameHook::EnableFrameTheme(CXTPCommandBars* pCommandBars)
{
	BOOL bEnabled = pCommandBars != NULL;
	if (m_bThemeFrameEnabled == bEnabled)
		return;

	m_pCommandBars = pCommandBars;
	m_pRibbonBar   = NULL;

	if (m_pPaintManager)
	{
		m_pPaintManager->Unsubscribe(this);
		m_pPaintManager->InternalRelease();
		m_pPaintManager = NULL;
	}

	m_bUseGlobalPaintManager = FALSE;

	if (NULL != pCommandBars)
	{
#ifdef _XTP_INCLUDE_RIBBON
		m_pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pCommandBars->GetMenuBar());
#endif
		SetSiteWindow(pCommandBars->GetSite()->GetSafeHwnd());
	}

	m_bActive		= TRUE;
	m_szFrameRegion = CSize(0, 0);

	m_pSelected = NULL;

	m_bDelayReclalcLayout = FALSE;
	m_bButtonPressed	  = FALSE;

	m_bThemeFrameEnabled = bEnabled;

	if (NULL != pCommandBars)
	{
		XTPHookManager()->SetHook(m_hwndSite, this);
		SendMessage(m_hwndSite, m_nMsgUpdateSkinState, 0, 0);

		m_bDwmEnabled = !m_bAllowDwm || GetSiteStyle() & WS_CHILD
							? FALSE
							: CXTPWinDwmWrapper().IsCompositionEnabled();

		if (!m_bDwmEnabled)
		{
			RefreshFrameStyle();
		}
		else
		{
			SetWindowRgn(m_hwndSite, NULL, TRUE);
		}

		UpdateFrameRegion();
		::SetWindowPos(m_hwndSite, 0, 0, 0, 0, 0,
					   SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		SetWinHook();

		EnableShadow();

		pCommandBars->SetFrameHook(this);
	}
	else
	{
		DisableFrameTheme();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CXTPCommandBarsFrameHook::EnableOffice2007Frame(CWnd* pSite, CXTPPaintManager* pPaintManager)
{
	EnableFrameTheme(pSite, pPaintManager);
}

void CXTPCommandBarsFrameHook::EnableOffice2007Frame(CWnd* pSite, CXTPCommandBars* pCommandBars)
{
	EnableFrameTheme(pSite, pCommandBars);
}

void CXTPCommandBarsFrameHook::EnableOffice2007Frame(CXTPCommandBars* pCommandBars)
{
	EnableFrameTheme(pCommandBars);
}

/////////////////////////////////////////////////////////////////////////////

DWORD CXTPCommandBarsFrameHook::GetSiteStyle() const
{
	return m_hwndSite ? (DWORD)GetWindowLong(m_hwndSite, GWL_STYLE) : 0;
}

DWORD CXTPCommandBarsFrameHook::GetSiteExStyle() const
{
	return m_hwndSite ? (DWORD)GetWindowLong(m_hwndSite, GWL_EXSTYLE) : 0;
}

void CXTPCommandBarsFrameHook::SetWinHook()
{
	CXTPWinEventHook& hook = CXTPSingleton<CXTPWinEventHook>::Instance();
	if (m_hwndSite)
	{
		hook.SetEventHook(EVENT_OBJECT_REORDER, EVENT_OBJECT_REORDER,
						  &CXTPCommandBarsFrameHook::WinEventProc);
	}
	else
	{
		hook.RemoveEventHook();
	}
}

CXTPPaintManager* CXTPCommandBarsFrameHook::GetPaintManager() const
{
	return m_pCommandBars ? m_pCommandBars->GetPaintManager()
						  : (m_bUseGlobalPaintManager ? XTPPaintManager() : m_pPaintManager);
}

CXTPFramePaintManager* CXTPCommandBarsFrameHook::GetFramePaintManager() const
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	return (NULL != pPaintManager ? pPaintManager->GetFramePaintManager() : NULL);
}

void CXTPCommandBarsFrameHook::UpdateFrameRegion()
{
	if (!IsWindow(m_hwndSite))
		return;

	CXTPWindowRect rc(m_hwndSite);
	UpdateFrameRegion(rc.Size(), TRUE);
}

void CXTPCommandBarsFrameHook::UpdateFrameRegion(CSize szFrameRegion, BOOL bUpdate)
{
	if (m_bInUpdateFrame)
		return;

	m_bInUpdateFrame = TRUE;

	if (m_szFrameRegion != szFrameRegion || bUpdate)
	{
		if (!m_bDwmEnabled)
		{
			if (!IsMDIMaximized())
			{
				if (WS_MAXIMIZE & GetSiteStyle())
				{
					// Maximized window always has a rectangular default frame region
					// which has to be relative to the current monitor.

					CRect rcMonitor = XTPMultiMonitor()->GetWorkArea(m_hwndSite);
					CXTPWindowRect rcWindow(m_hwndSite);

					CRect rcBorder;
					rcBorder.left   = rcMonitor.left - rcWindow.left;
					rcBorder.top	= rcMonitor.top - rcWindow.top;
					rcBorder.right  = rcWindow.right - rcMonitor.right;
					rcBorder.bottom = rcWindow.bottom - rcMonitor.bottom;

					rcWindow.left -= rcMonitor.left;
					rcWindow.right -= rcMonitor.left;
					rcWindow.top -= rcMonitor.top;
					rcWindow.bottom -= rcMonitor.top;

					rcWindow.left += rcBorder.left * 2;
					rcWindow.top += rcBorder.top * 2;

					// For some reason right and bottom borders do not require
					// border adjustments on multi-monitor set.
					// rcWindow.right -= rcBorder.right * 2;
					// rcWindow.bottom -= rcBorder.bottom * 2;

					HRGN hRgn = CreateRectRgn(rcWindow.left, rcWindow.top, rcWindow.right,
											  rcWindow.bottom);
					::SetWindowRgn(m_hwndSite, hRgn, TRUE);
				}
				else
				{
					CRect rc(0, 0, szFrameRegion.cx, szFrameRegion.cy);
					AdjustWindowRectEx(rc, GetSiteStyle(), FALSE, GetSiteExStyle());
					HRGN hRgn = GetFramePaintManager()->CalcFrameRegion(this, szFrameRegion);
					::SetWindowRgn(m_hwndSite, hRgn, TRUE);

					/*
					Note:
					After a successful call to SetWindowRgn, the system owns the region specified by
					the region handle hRgn. The system does not make a copy of the region. Thus, you
					should not make any further function calls with this region handle. In
					particular, do not delete this region handle. The system deletes the region
					handle when it no longer needed.
					*/
				}
			}
			else
			{
				::SetWindowRgn(m_hwndSite, NULL, TRUE);
			}
		}
		else
		{
			UpdateDwmClientArea();
		}

		m_szFrameRegion = szFrameRegion;
	}

	m_bInUpdateFrame = FALSE;
}

void CXTPCommandBarsFrameHook::UpdateDwmClientArea()
{
	int cyTopHeight = 0;

	if (!IsCaptionVisible())
	{
		cyTopHeight += GetCaptionHeight();
	}

#ifdef _XTP_INCLUDE_RIBBON
	if (GetFramePaintManager()->m_bFlatFrame && !IsCaptionVisible() && m_pRibbonBar)
	{
		cyTopHeight += m_pRibbonBar->GetTabsHeight() + XTP_DPI_Y(1);
	}
#endif

	if (m_bToolBarComposition)
	{
		CXTPDockBar* pDockBar = m_pCommandBars->GetDockBar(xtpBarTop);
		cyTopHeight += CXTPWindowRect(pDockBar).Height();
	}

	CXTPWinDwmWrapper().ExtendFrameIntoClientArea(m_hwndSite, 0, cyTopHeight, 0, 0);
}

BOOL CXTPCommandBarsFrameHook::IsMDIMaximized() const
{
	if ((GetSiteStyle() & WS_MAXIMIZE) && (GetSiteExStyle() & WS_EX_MDICHILD))
		return TRUE;

	return FALSE;
}

BOOL CXTPCommandBarsFrameHook::IsCaptionVisible() const
{
	if (!m_pRibbonBar)
		return TRUE;

	DWORD dwStyle = GetSiteStyle();
	if ((dwStyle & (WS_CHILD | WS_MAXIMIZE)) == (WS_CHILD | WS_MAXIMIZE))
		return FALSE;

#ifdef _XTP_INCLUDE_RIBBON
	if (0 == (dwStyle & WS_CHILD) && !m_pRibbonBar->IsVisible()
		&& m_pRibbonBar->GetPosition() == xtpBarTop)
		return TRUE;
#endif

	if ((dwStyle & (WS_CHILD | WS_MINIMIZE)) == (WS_CHILD | WS_MINIMIZE))
		return TRUE;

	return FALSE;
}

BOOL CXTPCommandBarsFrameHook::IsFrameHasStatusBar(int* pnStatusHeight) const
{
	if (!GetFramePaintManager()->m_bFrameStatusBar)
		return FALSE;

	CWnd* pStatusBar = CWnd::FromHandle(::GetDlgItem(m_hwndSite, AFX_IDW_STATUS_BAR));
	if (!pStatusBar)
		return FALSE;

	if ((GetWindowLong(pStatusBar->m_hWnd, GWL_STYLE) & WS_VISIBLE) == 0)
		return FALSE;

	if (!GetPaintManager()->m_bThemedStatusBar)
		return FALSE;

	if (pnStatusHeight)
	{
		*pnStatusHeight = CXTPWindowRect(pStatusBar).Height();
	}

	return TRUE;
}

void CXTPCommandBarsFrameHook::RedrawFrame()
{
	if (!m_bDwmEnabled)
	{
		CWindowDC dc(GetSite());
		GetFramePaintManager()->DrawFrame(&dc, this);
	}
}

void CXTPCommandBarsFrameHook::DrawRibbonFramePart(CDC* pDC)
{
#ifdef _XTP_INCLUDE_RIBBON
	if (m_pRibbonBar)
	{
		m_pRibbonBar->GetRibbonPaintManager()->DrawRibbonFramePart(pDC, m_pRibbonBar);
	}
#else
	UNREFERENCED_PARAMETER(pDC);
#endif
}

void CXTPCommandBarsFrameHook::RedrawRibbonBar()
{
#ifdef _XTP_INCLUDE_RIBBON
	if (m_pRibbonBar)
	{
		m_pRibbonBar->Redraw();
		m_pRibbonBar->UpdateWindow();
	}
#endif
}

void CXTPCommandBarsFrameHook::DelayRecalcFrameLayout()
{
	m_bDelayReclalcLayout = TRUE;
	::PostMessage(m_hwndSite, WM_IDLEUPDATECMDUI, 0, 0);
}

void CXTPCommandBarsFrameHook::RecalcFrameLayout()
{
	m_bDelayReclalcLayout = FALSE;
	m_szFrameRegion		  = CSize(0);
	::SetWindowPos(m_hwndSite, 0, 0, 0, 0, 0,
				   SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	CWnd* pSite = GetSite();

	CFrameWnd* pFrame = pSite->IsFrameWnd() ? (CFrameWnd*)pSite : NULL;
	if (pFrame)
	{
		pFrame->RecalcLayout(FALSE);
	}
	else
	{
		CXTPClientRect rc(pSite);
		pSite->SendMessage(WM_SIZE, 0, MAKELPARAM(rc.Width(), rc.Height()));
	}

#ifdef _XTP_INCLUDE_RIBBON
	if (m_pRibbonBar)
	{
		m_pRibbonBar->Redraw();
	}
#endif
}

AFX_INLINE void SetVisibleAndEnabled(CXTPControl* pControl, BOOL bVisible, BOOL bEnabled)
{
	pControl->SetHideFlag(xtpHideGeneric, !bVisible);
	pControl->SetEnabled(bEnabled);
}

BOOL HasStyle(const DWORD dwStyles, const DWORD dwStyle)
{
	return (dwStyle == (dwStyles & dwStyle));
}

void CXTPCommandBarsFrameHook::RepositionCaptionButtons()
{
	CXTPWindowRect rcWindow(m_hwndSite);
	rcWindow.OffsetRect(-rcWindow.TopLeft());

	DWORD dwStyle   = GetSiteStyle();
	DWORD dwExStyle = GetSiteExStyle();

	BOOL bToolWindow  = HasStyle(dwExStyle, WS_EX_TOOLWINDOW);
	BOOL bMinimized   = HasStyle(dwStyle, WS_MINIMIZE);
	BOOL bMaximized   = HasStyle(dwStyle, WS_MAXIMIZE);
	BOOL bMaximizeBox = HasStyle(dwStyle, WS_MAXIMIZEBOX);
	BOOL bMinimizeBox = HasStyle(dwStyle, WS_MINIMIZEBOX);
	BOOL bCloseBox	= TRUE;
	BOOL bSysMenu	 = HasStyle(dwStyle, WS_SYSMENU);

	BOOL bShowMinMaxButtons = !bToolWindow && bSysMenu && (bMaximizeBox || bMinimizeBox);

	if (bSysMenu && !bToolWindow)
	{
		HMENU hSystemMenu = ::GetSystemMenu(m_hwndSite, FALSE);

		if (NULL != hSystemMenu)
		{
			UINT nMenuState = ::GetMenuState(hSystemMenu, SC_CLOSE, MF_BYCOMMAND);

			bCloseBox = (0 == (nMenuState & (MF_DISABLED | MF_GRAYED)));
		}
	}

	CRect rcCaption = GetFramePaintManager()->CalcFrameCaption(this);

	for (int i = 0; i < m_pCaptionButtons->GetCount(); i++)
	{
		CXTPControl* pControl = m_pCaptionButtons->GetAt(i);

		switch (pControl->GetID())
		{
			case SC_MINIMIZE:
				SetVisibleAndEnabled(pControl, !bMinimized && bShowMinMaxButtons, bMinimizeBox);
				break;
			case SC_MAXIMIZE:
				SetVisibleAndEnabled(pControl, !bMaximized && bShowMinMaxButtons, bMaximizeBox);
				break;
			case SC_RESTORE:
				pControl->SetHelpId(bMinimized ? HTMINBUTTON : HTMAXBUTTON);

				if (bMinimized)
					SetVisibleAndEnabled(pControl, bShowMinMaxButtons, bMinimizeBox);
				else
					SetVisibleAndEnabled(pControl, bMaximized && bShowMinMaxButtons, bMaximizeBox);
				break;
			case SC_CLOSE: SetVisibleAndEnabled(pControl, bSysMenu, bCloseBox); break;
		}

		if (pControl->IsVisible())
		{
			CSize szButton = GetFramePaintManager()->GetCaptionButtonSize(XTPToUInt(
																			  pControl->GetID()),
																		  m_hwndSite);

			CRect rcButton(rcCaption.right - szButton.cx, // Left
						   rcCaption.top,				  // Top
						   rcCaption.right,				  // Right
						   rcCaption.top + szButton.cy	// Bottom
			);
			pControl->SetRect(rcButton);

			rcCaption.right -= rcButton.Width();
		}
	}

	GetFramePaintManager()->RecalcLayout(this);
}

void CXTPCommandBarsFrameHook::ScreenToFrame(LPPOINT lpPoint)
{
	CXTPDrawHelpers::ScreenToWindow(GetSite(), lpPoint);
}

void CXTPCommandBarsFrameHook::OnPaintManagerDestroyed(CXTPPaintManager* pPaintManager)
{
	if (m_pPaintManager == pPaintManager)
	{
		// Must not be released at this point
		m_pPaintManager->Unsubscribe(this);
		m_pPaintManager = NULL;
	}
}

void CXTPCommandBarsFrameHook::OnFrameShadowDisabled(CXTPFrameShadowManager* pManager,
													 HWND hwndOwner, CXTPFrameShadow* pShadow)
{
	ASSERT_VALID(pShadow);

	if (m_pShadow == pShadow)
	{
		m_pShadow = NULL;
		XTPFrameShadowManager()->Unsubscribe(this);
	}

	UNREFERENCED_PARAMETER(pManager);
	UNREFERENCED_PARAMETER(hwndOwner);
}

void CXTPCommandBarsFrameHook::TrackCaptionButton(CXTPControl* pSelected)
{
	CWnd* pSite = GetSite();
	if (!pSite)
		return;

	pSite->SetCapture();
	BOOL bAccept	 = FALSE;
	m_bButtonPressed = TRUE;

	ASSERT(pSelected);
	if (!pSelected)
		return;

	RedrawFrame();

	while (::GetCapture() == m_hwndSite)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
		{
			bAccept = m_bButtonPressed;
			break;
		}
		else if (msg.message == WM_MOUSEMOVE)
		{
			POINT point = msg.pt;
			ScreenToFrame(&point);

			BOOL bButtonPressed = pSelected->GetRect().PtInRect(point);

			if (bButtonPressed != m_bButtonPressed)
			{
				m_bButtonPressed = bButtonPressed;
				RedrawFrame();
			}
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	m_bButtonPressed = NULL;
	m_pSelected		 = NULL;

	ReleaseCapture();
	RedrawFrame();

	if (bAccept)
	{
		pSite->SendMessage(WM_SYSCOMMAND, XTPToWPARAM(pSelected->GetID()));
	}
}

int CXTPCommandBarsFrameHook::GetCaptionHeight() const
{
	CXTPFramePaintManager* pFramePaintManager = GetFramePaintManager();

	int nFrameBorder   = CalcFrameBorder().top;
	int nCaptionHeight = pFramePaintManager->GetFrameCaptionHeight();

#ifdef _XTP_INCLUDE_RIBBON
	if (m_pRibbonBar && !IsCaptionVisible())
	{
		nCaptionHeight = m_pRibbonBar->GetCaptionHeight();
	}
#endif

	return nCaptionHeight + nFrameBorder;
}

#ifndef WM_DWMCOMPOSITIONCHANGED
#	define WM_DWMCOMPOSITIONCHANGED 0x031E
#endif

CXTPControl* CXTPCommandBarsFrameHook::HitTestCaptionButton(CPoint point)
{
	CXTPControl* pCaptionButton = m_pCaptionButtons->HitTest(point);
	if (pCaptionButton && !pCaptionButton->GetEnabled())
		pCaptionButton = NULL;

	return pCaptionButton;
}

void CXTPCommandBarsFrameHook::RefreshFrameStyle()
{
	if (m_bDwmEnabled)
		return;

	DWORD dwStyle		= GetSiteStyle();
	DWORD dwStyleRemove = (WS_DLGFRAME | WS_VSCROLL | WS_HSCROLL);

	if (dwStyle & dwStyleRemove)
	{
		m_bLockNCPaint++;
		SetWindowLong(m_hwndSite, GWL_STYLE, XTPToLong(dwStyle & ~dwStyleRemove));

		RECT rc = CXTPWindowRect(m_hwndSite);
		::SendMessage(m_hwndSite, WM_NCCALCSIZE, FALSE, XTPToLPARAM(&rc));

		SetWindowLong(m_hwndSite, GWL_STYLE, XTPToLong(dwStyle));

		m_bLockNCPaint--;
		RedrawFrame();
	}
}

BOOL CXTPCommandBarsFrameHook::GetAutoHideBar() const
{
	APPBARDATA abd;
	ZeroMemory(&abd, sizeof(APPBARDATA));
	abd.cbSize = sizeof(APPBARDATA);

	return (SHAppBarMessage(ABM_GETSTATE, &abd) & ABS_AUTOHIDE) != 0;
}

CRect CXTPCommandBarsFrameHook::CalcFrameBorder() const
{
	return XTP_SAFE_GET1(GetFramePaintManager(), CalcFrameBorder(this), CRect(0, 0, 0, 0));
}

extern const UINT XTP_WM_UPDATE_MDI_CHILD_THEME;

int CXTPCommandBarsFrameHook::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam,
											LPARAM& lParam, LRESULT& lResult)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (nMessage == XTP_WM_UPDATE_MDI_CHILD_THEME)
	{
		OnUpdateMDIChildrenTheme();
		return TRUE;
	}

	if (!m_bThemeFrameEnabled)
	{
		return FALSE;
	}

	if (nMessage == WM_ACTIVATE && NULL != GetShadow())
	{
		CXTPFramePaintManager* pFramePaintManager = GetFramePaintManager();
		if (NULL != pFramePaintManager)
		{
			BOOL bActivating = static_cast<BOOL>(LOWORD(wParam));

			if (!bActivating && reinterpret_cast<HWND>(lParam) == GetShadow()->m_hWnd)
			{
				// Frame shadow is being activated, e.g. during resizing.
				bActivating = TRUE;
			}

			pFramePaintManager->OnShadowedFrameActiveStateChanged(bActivating, GetSite(),
																  GetShadow());
		}
	}

	if (!m_bDwmEnabled && m_bLockNCPaint
		&& (nMessage == WM_STYLECHANGING || nMessage == WM_STYLECHANGED
			|| nMessage == WM_WINDOWPOSCHANGED || nMessage == WM_WINDOWPOSCHANGING
			|| nMessage == WM_NCPAINT))
	{
		if (nMessage == WM_WINDOWPOSCHANGING)
			((WINDOWPOS*)lParam)->flags &= ~SWP_FRAMECHANGED;

		return TRUE;
	}

	if (nMessage == WM_WINDOWPOSCHANGING)
	{
		WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;

		CSize szFrameRegion(lpwndpos->cx, lpwndpos->cy);

		if (((lpwndpos->flags & SWP_NOSIZE) == 0) && (m_szFrameRegion != szFrameRegion)
			&& ((GetSiteStyle() & WS_CHILD) == 0))
		{
			UpdateFrameRegion(szFrameRegion);
		}

		return FALSE;
	}

	if (nMessage == CXTPCommandBarsFrameHook::m_nMsgQueryFrameHook)
	{
		lResult = (LRESULT)this;
		return TRUE;
	}

	if (nMessage == WM_WINDOWPOSCHANGED)
	{
		WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
		if (lpwndpos->flags & SWP_FRAMECHANGED && !m_bInUpdateFrame)
		{
			UpdateFrameRegion();
		}
		return FALSE;
	}

	if (nMessage == WM_SIZE || nMessage == WM_STYLECHANGED)
	{
		CXTPWindowRect rc(m_hwndSite);

		CSize szFrameRegion(rc.Width(), rc.Height());

		if ((m_szFrameRegion != szFrameRegion) || (nMessage == WM_STYLECHANGED))
		{
			UpdateFrameRegion(szFrameRegion, (nMessage == WM_STYLECHANGED));
			RepositionCaptionButtons();
			RedrawFrame();
		}

		if (nMessage == WM_SIZE && wParam == SIZE_MINIMIZED) // Strange Vista bug with both
															 // WS_MINIMIZE and WS_MAXIMIZE styles
		{
			if (GetSiteStyle() & WS_MAXIMIZE)
			{
				CWnd::ModifyStyle(m_hwndSite, WS_MAXIMIZE, 0, 0);
			}
		}

		return FALSE;
	}

	if (nMessage == WM_NCRBUTTONUP && IsCaptionVisible() && !m_bDwmEnabled && wParam == HTCAPTION)
	{
		if (m_pSelected)
		{
			m_pSelected = NULL;
			RedrawFrame();
		}

		::SendMessage(m_hwndSite, WM_POPUPSYSTEMMENU, XTPToWPARAM(m_hwndSite), lParam);
		return TRUE;
	}

	if (nMessage == WM_NCLBUTTONDOWN && IsCaptionVisible() && !m_bDwmEnabled)
	{
		CPoint point = XTP_POINT_FROM_LPARAM(lParam);
		ScreenToFrame(&point);

		CXTPControl* pCaptionButton = HitTestCaptionButton(point);
		if (pCaptionButton)
		{
			TrackCaptionButton(pCaptionButton);
			return TRUE;
		}
	}

	if (nMessage == WM_NCMOUSEMOVE && IsCaptionVisible() && !m_bDwmEnabled)
	{
		CPoint point = XTP_POINT_FROM_LPARAM(lParam);
		ScreenToFrame(&point);

		CXTPControl* pCaptionButton = HitTestCaptionButton(point);
		if (pCaptionButton != m_pSelected)
		{
			m_pSelected = pCaptionButton;
			RedrawFrame();

			if (m_pSelected)
			{
				::SetTimer(m_hwndSite, XTP_TID_MOUSELEAVE, 50, NULL);
			}
			else
			{
				KillTimer(m_hwndSite, XTP_TID_MOUSELEAVE);
			}
		}
	}

	if (nMessage == WM_TIMER && wParam == XTP_TID_REFRESHFRAME)
	{
		RefreshFrameStyle();
		::KillTimer(m_hwndSite, XTP_TID_REFRESHFRAME);
		return TRUE;
	}

	if (nMessage == WM_TIMER && wParam == XTP_TID_MOUSELEAVE && !m_bDwmEnabled)
	{
		if (::GetCapture() != NULL)
			return TRUE;

		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToFrame(&pt);

		if (!HitTestCaptionButton(pt))
		{
			if (m_pSelected)
			{
				m_pSelected = NULL;
				RedrawFrame();
			}

			KillTimer(m_hwndSite, XTP_TID_MOUSELEAVE);
		}

		return TRUE;
	}

	if (nMessage == WM_NCHITTEST && !IsCaptionVisible() && !m_bDwmEnabled)
	{
		CPoint point = XTP_POINT_FROM_LPARAM(lParam);
		ScreenToFrame(&point);

		lResult = XTPHookManager()->Default();

		if ((GetSiteExStyle() & WS_EX_CLIENTEDGE) && (GetSiteStyle() & WS_SIZEBOX)
			&& !(GetSiteStyle() & WS_MINIMIZE))
		{
			if (point.y == m_szFrameRegion.cy - 1 && lResult == HTBORDER)
			{
				lResult = HTBOTTOM;
			}
		}

		return TRUE;
	}

	if (nMessage == WM_NCHITTEST && !IsCaptionVisible() && m_bDwmEnabled)
	{
		CPoint point = XTP_POINT_FROM_LPARAM(lParam);
		ScreenToFrame(&point);

		lResult = XTPHookManager()->Default();
		if (point.y < GetCaptionHeight())
		{
			CXTPWinDwmWrapper().DefWindowProc(m_hwndSite, nMessage, wParam, lParam, &lResult);
		}
		return TRUE;
	}
	if (nMessage == WM_NCHITTEST && IsCaptionVisible() && !m_bDwmEnabled)
	{
		CPoint point = XTP_POINT_FROM_LPARAM(lParam);
		ScreenToFrame(&point);

		lResult = XTPHookManager()->Default();
		if (lResult == HTCLOSE || lResult == HTMINBUTTON || lResult == HTMAXBUTTON
			|| lResult == HTHELP)
			lResult = HTCAPTION;

		CXTPControl* pCaptionButton = m_pCaptionButtons->HitTest(point);
		if (pCaptionButton)
		{
			lResult = pCaptionButton->GetHelpId();

			if (XTPSystemVersion()->IsWin11OrGreater())
			{
				if (GetSiteStyle() & WS_CHILD && lResult == HTMAXBUTTON)
				{
					lResult = HTCAPTION;
				}
			}

			return TRUE;
		}

		if (lResult == HTNOWHERE && point.y < GetCaptionHeight())
		{
			if ((point.x < XTP_DPI_X(7) + GetSystemMetrics(SM_CXSMICON))
				&& (GetSiteStyle() & WS_SYSMENU))
				lResult = HTSYSMENU;
			else
				lResult = HTCAPTION;
		}

		return TRUE;
	}

	if (nMessage == WM_SETTEXT)
	{
		lResult = XTPHookManager()->Default();

		if (IsCaptionVisible())
		{
			RedrawFrame();
		}
		else
		{
			if (m_pRibbonBar)
				RecalcFrameLayout();
		}

		return TRUE;
	}

	if (nMessage == WM_NCACTIVATE && !m_bDwmEnabled)
	{
		BOOL bActive = (BOOL)wParam;
		CWnd* pSite  = GetSite();

		// stay active if WF_STAYACTIVE bit is on
		if (pSite->m_nFlags & WF_STAYACTIVE)
			bActive = TRUE;

		// but do not stay active if the window is disabled
		if (!pSite->IsWindowEnabled())
			bActive = FALSE;

		DWORD dwStyle = GetSiteStyle();

		if (dwStyle & WS_VISIBLE)
		{
			m_bLockNCPaint = TRUE;

			if (dwStyle & WS_SIZEBOX)
				CWnd::ModifyStyle(m_hwndSite, WS_SIZEBOX, 0, 0);

			if (pSite->IsWindowEnabled())
				lResult = XTPHookManager()->Default(XTPToWPARAM(bActive), 0);
			else
				lResult = XTPHookManager()->Default(wParam, lParam);

			if (dwStyle & WS_SIZEBOX)
				CWnd::ModifyStyle(m_hwndSite, 0, WS_SIZEBOX, 0);

			m_bLockNCPaint = FALSE;

			if (m_bActive != bActive)
			{
				m_bActive = bActive;

				RepositionCaptionButtons();
				RedrawRibbonBar();
			}

			RedrawFrame();
		}
		else
		{
			m_bActive = bActive;
			return FALSE;
		}

		return TRUE;
	}

	if (nMessage == WM_DWMCOMPOSITIONCHANGED)
	{
		BOOL bDwmEnabled = !m_bAllowDwm || GetSiteStyle() & WS_CHILD
							   ? FALSE
							   : CXTPWinDwmWrapper().IsCompositionEnabled();

		if (bDwmEnabled != m_bDwmEnabled)
		{
			m_bDwmEnabled = bDwmEnabled;

			::SetWindowRgn(m_hwndSite, 0, TRUE);

			if (!m_bDwmEnabled)
			{
				RefreshFrameStyle();
			}

			::SetWindowPos(m_hwndSite, 0, 0, 0, 0, 0,
						   SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER
							   | SWP_FRAMECHANGED);
		}
		return FALSE;
	}

	if (nMessage == WM_NCCALCSIZE && !m_bLockNCPaint)
	{
		NCCALCSIZE_PARAMS FAR* lpncsp = (NCCALCSIZE_PARAMS FAR*)lParam;

		CRect rc(lpncsp->rgrc[0]);

		if (m_bDwmEnabled)
		{
			lResult = XTPHookManager()->Default();
		}
		else
		{
			CRect rcBorder = GetFramePaintManager()->CalcFrameBorder(this);

			lpncsp->rgrc[0].left += rcBorder.left;
			lpncsp->rgrc[0].top += rcBorder.top;
			lpncsp->rgrc[0].right -= rcBorder.right;
			lpncsp->rgrc[0].bottom -= rcBorder.bottom;
		}

		DWORD dwStyle = GetSiteStyle();

		if (m_bDwmEnabled)
		{
			if (!IsCaptionVisible())
				lpncsp->rgrc[0].top = rc.top;
		}
		else
		{
			lpncsp->rgrc[0].top += (IsCaptionVisible() ? GetCaptionHeight() - CalcFrameBorder().top
													   : 0);
			if (IsFrameHasStatusBar() && ((dwStyle & WS_MAXIMIZE) == 0))
			{
				int nBorderSize		   = max(rc.bottom - lpncsp->rgrc[0].bottom - XTP_DPI_Y(3), 1);
				lpncsp->rgrc[0].bottom = rc.bottom - nBorderSize;
			}
		}

		if (((dwStyle & (WS_MAXIMIZE | WS_CHILD)) == WS_MAXIMIZE) && GetAutoHideBar())
		{
			lpncsp->rgrc[0].bottom -= 1;
		}

		return TRUE;
	}

	if (nMessage == m_nMsgQuerySkinState)
	{
		lResult = TRUE;
		return TRUE;
	}

	if (nMessage == WM_GETMINMAXINFO && !m_bDwmEnabled)
	{
		lResult = XTPHookManager()->Default();

		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;

		int yMin = CalcFrameBorder().top + GetCaptionHeight();
		int xMin = (int)3 * yMin;

		xMin += GetSystemMetrics(SM_CYSIZE) + 2 * GetSystemMetrics(SM_CXEDGE);

		lpMMI->ptMinTrackSize.x = max(lpMMI->ptMinTrackSize.x, xMin);
		lpMMI->ptMinTrackSize.y = max(lpMMI->ptMinTrackSize.y, yMin);

		return TRUE;
	}

	if (nMessage == WM_SYSCOMMAND && !m_bDwmEnabled && (wParam & 0xFFF0) == SC_MINIMIZE
		&& GetSiteStyle() & WS_CHILD)
	{
		lResult = XTPHookManager()->Default();

		RedrawFrame();

		return TRUE;
	}

	if (nMessage == WM_NCPAINT && !m_bDwmEnabled)
	{
		if (!IsMDIMaximized())
		{
			RepositionCaptionButtons();

			CWindowDC dc(GetSite());
			GetFramePaintManager()->DrawFrame(&dc, this);
		}
		return TRUE;
	}
	if (nMessage == WM_PRINT && !m_bDwmEnabled)
	{
		lResult = XTPHookManager()->Default();

		if (lParam & PRF_NONCLIENT)
		{
			CDC* pDC = CDC::FromHandle((HDC)wParam);
			GetFramePaintManager()->DrawFrame(pDC, this);
		}
	}

	if (nMessage == WM_IDLEUPDATECMDUI)
	{
		if (m_bDelayReclalcLayout)
		{
			RecalcFrameLayout();
		}
	}

	return FALSE;
}

void CXTPCommandBarsFrameHook::OnUpdateMDIChildrenTheme()
{
	// Do nothing by default
}

BOOL CXTPCommandBarsFrameHook::EnableShadow()
{
	if (NULL != GetSite() && 0 == (GetSite()->GetStyle() & WS_CHILD))
	{
		DisableShadow();

		CXTPFramePaintManager* pFramePaintManager = GetFramePaintManager();
		if (NULL != pFramePaintManager)
		{
			m_pShadow = pFramePaintManager->CreateShadow(GetSite());
			if (NULL != m_pShadow)
			{
				XTPFrameShadowManager()->Subscribe(this);
			}
		}
	}

	return NULL != GetShadow();
}

void CXTPCommandBarsFrameHook::DisableShadow()
{
	CWnd* pSite				 = GetSite();
	CXTPFrameShadow* pShadow = GetShadow();
	if (NULL != pSite && XTPFrameShadowManager()->GetShadow(pSite) == pShadow)
	{
		XTPFrameShadowManager()->DisableShadow(pSite);
		m_pShadow = NULL;
	}
	else if (NULL != pShadow)
	{
		ASSERT(pShadow == m_pShadow);
		XTPFrameShadowManager()->DisableShadow(pShadow->GetOwner());
		m_pShadow = NULL;
	}
}

CXTPFrameShadow* CXTPCommandBarsFrameHook::GetShadow() const
{
	if (NULL != m_pShadow)
	{
		ASSERT_VALID(m_pShadow);
	}

	return m_pShadow;
}

void CXTPCommandBarsFrameHook::OnBeforeUpdatePaintManager()
{
	DisableShadow();
}

void CXTPCommandBarsFrameHook::OnUpdatePaintManager(CXTPPaintManager* pPaintManager)
{
	if (m_pPaintManager != pPaintManager)
	{
		if (m_pPaintManager)
		{
			m_pPaintManager->Unsubscribe(this);
			m_pPaintManager->InternalRelease();
			m_pPaintManager = NULL;
		}

		if (XTPPaintManager() != pPaintManager && NULL != pPaintManager)
		{
			m_pPaintManager = pPaintManager;
			m_pPaintManager->Subscribe(this);
			m_pPaintManager->InternalAddRef();
		}
		else
		{
			m_bUseGlobalPaintManager = TRUE;
		}
	}

	EnableShadow();
}
