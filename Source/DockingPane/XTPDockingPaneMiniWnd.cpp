// XTPDockingPaneMiniWnd.cpp : implementation of the CXTPDockingPaneMiniWnd class.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#include "TabManager/Includes.h"

#include "DockingPane/Resource.h"
#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPane.h"
#include "DockingPane/XTPDockingPaneSplitterContainer.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPaneContext.h"
#include "DockingPane/XTPDockingPaneManager.h"
#include "DockingPane/XTPDockingPaneLayout.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneAutoHidePanel.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"
#include "DockingPane/XTPDockingPaneIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CXTPDockingPaneMiniWnd::m_bShowPinButton = TRUE;

#define TID_NCLEAVE 2345

#define TID_CHECKACTIVE 1
#define TID_SLIDEIN 2
#define TID_SLIDEOUT 3

#ifndef LWA_ALPHA
#	define LWA_ALPHA 0x00000002
#endif
#ifndef WS_EX_LAYERED
#	define WS_EX_LAYERED 0x00080000
#endif

IMPLEMENT_DYNAMIC(CXTPDockingPaneMiniWnd, CMiniFrameWnd)

AFX_INLINE int ResetStepsCount()
{
	if (CXTPDockingPaneAutoHideWnd::m_nAnimationInterval == 0)
		return 1;

	return max(1, CXTPDockingPaneAutoHideWnd::m_nAnimationDuration
					  / CXTPDockingPaneAutoHideWnd::m_nAnimationInterval);
}

CXTPDockingPaneMiniWnd::CXTPDockingPaneMiniWnd(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeMiniWnd, pLayout)
{
	m_pTopContainer = NULL;

	m_pCaptionButtons = new CXTPDockingPaneCaptionButtons();
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_MAXIMIZE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_RESTORE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_MENU, this));

	m_bSlideOut = m_bExpanded = m_bCollapsed = FALSE;
	m_nExpandedHeight						 = 0;

	m_nSlideStep  = 0;
	m_nStepsCount = ResetStepsCount();

	m_bDelayInvalidate   = FALSE;
	m_nDeactivationCount = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPDockingPaneMiniWnd::Init(CXTPDockingPaneBase* pPane, CRect rc)
{
	m_rcWindow = rc;
	CreateContainer();

	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		CXTPDockingPaneTabbedContainer* pContainer =
			(CXTPDockingPaneTabbedContainer*)GetDockingPaneManager()->OnCreatePane(
				xtpPaneTypeTabbedContainer, m_pLayout);
		pContainer->Init((CXTPDockingPane*)pPane, this);

		pPane = pContainer;
	}

	m_pTopContainer = (CXTPDockingPaneSplitterContainer*)GetDockingPaneManager()->OnCreatePane(
		xtpPaneTypeSplitterContainer, m_pLayout);
	m_pTopContainer->Init(pPane, TRUE, this);

	m_pTopContainer->m_pParentContainer = this;

	m_bDelayInvalidate = TRUE;
	PostMessage(WM_IDLEUPDATECMDUI);
	DelayRecalcLayout();

	OnFocusChanged();
}

CXTPDockingPaneMiniWnd::~CXTPDockingPaneMiniWnd()
{
	DestroyWindow();
}

void CXTPDockingPaneMiniWnd::Copy(CXTPDockingPaneBase* pCloneBase, CXTPPaneToPaneMap* pMap,
								  DWORD /*dwIgnoredOptions*/)
{
	CXTPDockingPaneMiniWnd* pClone = (CXTPDockingPaneMiniWnd*)pCloneBase;

	if (this != pClone)
	{
		ASSERT(pMap);

		m_pTopContainer = (CXTPDockingPaneSplitterContainer*)
							  pClone->m_pTopContainer->Clone(m_pLayout, pMap);
		m_pTopContainer->m_pParentContainer = this;
		m_pTopContainer->m_pDockingSite		= this;

		m_rcWindow = pClone->m_rcWindow;
		if (pClone->m_hWnd)
			::GetWindowRect(pClone->m_hWnd, &m_rcWindow);

		m_bCollapsed	  = ((CXTPDockingPaneMiniWnd*)pClone)->m_bCollapsed;
		m_nExpandedHeight = ((CXTPDockingPaneMiniWnd*)pClone)->m_nExpandedHeight;

		if (m_bCollapsed && pClone->m_hWnd)
		{
			m_rcWindow.bottom = m_rcWindow.top + GetPaintManager()->GetCaptionHeight()
								+ XTP_DPI_Y(8);
		}

		if (!m_pLayout->IsUserLayout())
		{
			CreateContainer();
			m_pTopContainer->OnParentContainerChanged(m_pTopContainer);
			OnChildContainerChanged(m_pTopContainer);
		}
	}
}

void CXTPDockingPaneMiniWnd::RemovePane(CXTPDockingPaneBase* pPane)
{
	ASSERT(pPane == m_pTopContainer);

	pPane->m_pParentContainer = NULL;
	m_pTopContainer			  = NULL;

	if (m_hWnd)
		ShowWindow(SW_HIDE);

	if (m_bCollapsed)
		OnPinButtonClick();
}

void CXTPDockingPaneMiniWnd::InvalidatePane(BOOL /*bSelectionChanged*/)
{
	if (!m_pTopContainer)
		return;

	if (!m_hWnd)
		return;

	CXTPDockingPaneBaseList lstTabbed;

	FindPane(xtpPaneTypeTabbedContainer, &lstTabbed);
	int nCount = (int)lstTabbed.GetCount();

	POSITION pos = lstTabbed.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = lstTabbed.GetNext(pos);
		ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);
		((CXTPDockingPaneTabbedContainer*)pPane)->ShowTitle(nCount != 1);
	}
	CString strTitle = (nCount != 1)
						   ? GetDockingPaneManager()->m_strFloatingFrameCaption
						   : ((CXTPDockingPaneTabbedContainer*)lstTabbed.GetHead())->GetTitle();
	SetWindowText(strTitle);

	if (nCount > 0)
	{
		BOOL bEnabled = FALSE;

		CXTPDockingPaneBaseList lstChilds;

		FindPane(xtpPaneTypeDockingPane, &lstChilds);
		pos = lstChilds.GetHeadPosition();

		while (pos)
		{
			CXTPDockingPaneBase* pPane = lstChilds.GetNext(pos);

			if ((((CXTPDockingPane*)pPane)->GetOptions() & xtpPaneNoCloseable) == 0)
				bEnabled = TRUE;
		}

		if (!bEnabled || !m_bCloseEnabled)
		{
			CMenu* pSysMenu = GetSystemMenu(FALSE);
			pSysMenu->EnableMenuItem(SC_CLOSE,
									 UINT(MF_BYCOMMAND)
										 | UINT(bEnabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED));
			m_bCloseEnabled = bEnabled;
		}
		if ((GetStyle() & WS_CHILD) == 0)
			UpdatePosition();

		if ((GetStyle() & WS_VISIBLE) == 0)
			ShowWindow(SW_SHOWNOACTIVATE);
	}
	SendMessage(WM_NCPAINT);

	OnFocusChanged();
}

void CXTPDockingPaneMiniWnd::UpdatePosition()
{
	CXTPWindowRect rc(this);
	CSize sz = rc.Size();

	if (IsThemed())
	{
		MINMAXINFO mmi;
		ZeroMemory(&mmi, sizeof(mmi));
		mmi.ptMaxTrackSize.x = 32000;
		mmi.ptMaxTrackSize.y = 32000;

		GetMinMaxInfo(&mmi);
		sz.cx = max(min(sz.cx, mmi.ptMaxTrackSize.x), mmi.ptMinTrackSize.x);
		sz.cy = max(min(sz.cy, mmi.ptMaxTrackSize.y), mmi.ptMinTrackSize.y);
	}

	MoveWindow(rc.left, rc.top, sz.cx, sz.cy);
}

void CXTPDockingPaneMiniWnd::CreateContainer()
{
	if (!m_hWnd)
	{
		Create(m_rcWindow);
	}
}

void CXTPDockingPaneMiniWnd::OnChildContainerChanged(CXTPDockingPaneBase* /*pContainer*/)
{
	if (!m_hWnd)
		return;

	if (m_pTopContainer == NULL || m_pTopContainer->IsEmpty())
	{
		DestroyWindow();
	}
	else
	{
		m_bDelayInvalidate = TRUE;
		PostMessage(WM_IDLEUPDATECMDUI);
	}
}

void CXTPDockingPaneMiniWnd::RecalcLayout(BOOL /*bNotify*/)
{
	if (m_pTopContainer == NULL)
		return;

	if (m_bInRecalcLayout)
		return;

	m_bInRecalcLayout = TRUE;

	m_nIdleFlags &= ~(idleLayout | idleNotify);

	if (m_pTopContainer->IsEmpty())
	{
		ShowWindow(SW_HIDE);

		if (m_bCollapsed)
			OnPinButtonClick();
	}
	else
	{
		// ShowWindow(SW_SHOW);

		AFX_SIZEPARENTPARAMS layout;
		layout.bStretch = TRUE;
		GetClientRect(&layout.rect);
		layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess

		m_pTopContainer->OnSizeParent(this, layout.rect, &layout);

		if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		{
			TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
		}

		CWnd::GetWindowRect(&m_pTopContainer->m_rcWindow);
		ScreenToClient(&m_pTopContainer->m_rcWindow);
	}

	GetDockingPaneManager()->SyncPanesState();

	m_bInRecalcLayout = FALSE;
}

BOOL CXTPDockingPaneMiniWnd::Create(CRect rc)
{
	DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_BORDER | MFS_BLOCKSYSMENU
					| WS_THICKFRAME
					| (GetDockingPaneManager()->m_bSyncActiveFloatingFrames ? MFS_SYNCACTIVE : 0);

	DWORD dwExStyle = 0;

	if (GetDockingPaneManager()->IsLayoutRTL())
	{
		dwExStyle = WS_EX_LAYOUTRTL;
	}

	CXTPDockingPaneContext::EnsureVisible(rc);

	CWnd* pParentWnd = GetDockingPaneManager()->GetSite();

#ifndef _XTP_ACTIVEX
	if (pParentWnd->GetStyle() & WS_CHILD)
		pParentWnd = AfxGetMainWnd();
#endif

	if (!CMiniFrameWnd::CreateEx(dwExStyle, _T("XTPDockingPaneMiniWnd"), _T(""), dwStyle, rc,
								 pParentWnd))
	{
		return FALSE;
	}
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
	CString strHide;
	if (strHide.LoadString(AFX_IDS_HIDE))
	{
		pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
		pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, SC_CLOSE, strHide);
	}
	m_bCloseEnabled = TRUE;

	if (IsThemed())
	{
		ModifyStyle(WS_BORDER | WS_THICKFRAME, 0, SWP_FRAMECHANGED);
	}

	SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(),
				 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

	UpdateWindowOpacity();

	return TRUE;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPDockingPaneMiniWnd, CMiniFrameWnd)
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_NCHITTEST_EX()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_GETMINMAXINFO()
	ON_WM_SETFOCUS()
	ON_WM_NCACTIVATE()
	ON_WM_SIZING()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPDockingPaneMiniWnd::OnSizing(UINT nSide, LPRECT lpRect)
{
	CMiniFrameWnd::OnSizing(nSide, lpRect);

	GetDockingPaneManager()->GetDockingContext()->OnSizingFloatingFrame(this, nSide, lpRect);
}

void CXTPDockingPaneMiniWnd::UpdateWindowOpacity()
{
	if (!GetSafeHwnd())
		return;

	CXTPDockingPaneManager::PFNSETLAYEREDWINDOWATTRIBUTES pfnSetLayeredWindowAttributes =
		GetDockingPaneManager()->m_pfnSetLayeredWindowAttributes;

	if (!pfnSetLayeredWindowAttributes)
		return;

	int nFloatingFramesOpacity = GetDockingPaneManager()->m_nFloatingFramesOpacity;
	if (nFloatingFramesOpacity == 255)
	{
		ModifyStyleEx(WS_EX_LAYERED, 0);
		return;
	}

	if (!m_bActive)
	{
		ModifyStyleEx(0, WS_EX_LAYERED);
		pfnSetLayeredWindowAttributes(m_hWnd, 0x00, (BYTE)nFloatingFramesOpacity, LWA_ALPHA);
	}
	else
	{
		pfnSetLayeredWindowAttributes(m_hWnd, 0x00, (BYTE)255, LWA_ALPHA);
	}
}

void CXTPDockingPaneMiniWnd::OnFocusChanged()
{
	if (GetSafeHwnd())
	{
		CWnd* pFocus = GetFocus();
		BOOL bActive = (pFocus->GetSafeHwnd()
						&& (pFocus == this || IsChild(pFocus)
							|| (pFocus->GetOwner()->GetSafeHwnd() && IsChild(pFocus->GetOwner()))));

		if (bActive != m_bActive)
		{
			m_bActive = bActive;

			if (IsThemed())
			{
				SendMessage(WM_NCPAINT);
			}
			UpdateWindowOpacity();
		}
	}
}

void CXTPDockingPaneMiniWnd::OnIdleUpdateCmdUI()
{
	if (!GetSafeHwnd())
		return;

	if (m_bDelayInvalidate)
	{
		m_bDelayInvalidate = FALSE;
		InvalidatePane(FALSE);
	}

	CMiniFrameWnd::OnIdleUpdateCmdUI();
}

void CXTPDockingPaneMiniWnd::OnClose()
{
	if (m_pTopContainer)
	{
		m_pCaptionButtons->CheckForMouseOver(CPoint(-1, -1));

		CXTPDockingPaneManager* pManager = GetDockingPaneManager();

		CXTPDockingPaneBaseList lst;
		FindPane(xtpPaneTypeDockingPane, &lst);

		POSITION pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);

			if ((pPane->GetOptions() & xtpPaneNoCloseable) != 0)
				continue;

			pPane->InternalAddRef();

			if (!pManager->NotifyAction(xtpPaneActionClosing, pPane))
			{
				pPane->Close();
				pManager->NotifyAction(xtpPaneActionClosed, pPane);
			}

			pPane->InternalRelease();
		}
	}
}

void CXTPDockingPaneMiniWnd::OnSetFocus(CWnd* pOldWnd)
{
	CMiniFrameWnd::OnSetFocus(pOldWnd);

	if (m_pTopContainer == 0)
		return;

	if (!m_pLayout || !m_pLayout->IsValid())
		return;

	CXTPDockingPaneBaseList lst;
	FindPane(xtpPaneTypeTabbedContainer, &lst);

	if (lst.GetCount() == 1)
	{
		CXTPDockingPane* pPane = ((CXTPDockingPaneTabbedContainer*)lst.GetHead())->GetSelected();
		if (pPane)
		{
			pPane->SetFocus();
		}
	}

	GetDockingPaneManager()->UpdatePanes();
}

void CXTPDockingPaneMiniWnd::PostNcDestroy()
{
	// prevent auto deleting
}

void CXTPDockingPaneMiniWnd::FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList) const
{
	if (GetType() == type)
	{
		pList->AddTail((CXTPDockingPaneBase*)this);
	}
	else if (m_pTopContainer)
	{
		m_pTopContainer->FindPane(type, pList);
	}
}

void CXTPDockingPaneMiniWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CXTPDockingPaneBaseList lst;
	FindPane(xtpPaneTypeTabbedContainer, &lst);

	CXTPDockingPane* pSelectedPane = 0;
	if (lst.GetCount() == 1)
	{
		pSelectedPane = ((CXTPDockingPaneTabbedContainer*)lst.GetHead())->GetSelected();
	}
	else
	{
		POSITION pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)
															 lst.GetNext(pos);
			if (pContainer->IsActive())
			{
				pSelectedPane = pContainer->GetSelected();
				break;
			}
		}
	}

	if (pSelectedPane)
		pSelectedPane->SetFocus();
	XTP_DOCKINGPANE_CLICK menu;
	menu.pPane = pSelectedPane;
	menu.pt	= point;
	menu.rcExclude.SetRectEmpty();
	menu.pContainer = this;

	if (GetDockingPaneManager()->NotifyOwner(XTP_DPN_CONTEXTMENU, (LPARAM)&menu))
		return;

	CMiniFrameWnd::OnContextMenu(pWnd, point);
}

void CXTPDockingPaneMiniWnd::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	if (IsThemed() && (nHitTest == HTCAPTION))
	{
		OnContextMenu(this, point);
		return;
	}

	CMiniFrameWnd::OnNcRButtonUp(nHitTest, point);
}

void CXTPDockingPaneMiniWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint /*point*/)
{
	if (nHitTest == HTCAPTION)
	{
		if (m_bCollapsed)
			OnPinButtonClick();
		else
			GetDockingPaneManager()->ToggleDocking(m_pTopContainer);
	}
}

void CXTPDockingPaneMiniWnd::OnDestroy()
{
	::GetWindowRect(m_hWnd, &m_rcWindow);
	CWnd::OnDestroy();
}

BOOL CXTPDockingPaneMiniWnd::IsThemed() const
{
	return GetDockingPaneManager()->IsFloatingFrameThemeEnabled();
}

//////////////////////////////////////////////////////////////////////////
// Themed methods.

LRESULT CXTPDockingPaneMiniWnd::OnNcHitTest(CPoint point)
{
	LRESULT nHitTest = (LRESULT)CMiniFrameWnd::OnNcHitTest(point);

	if (!IsThemed())
		return nHitTest;

	CXTPWindowRect rcWindow(this);
	CRect rcBorders = rcWindow;
	rcBorders.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));

	if (rcWindow.PtInRect(point) && !rcBorders.PtInRect(point))
	{
		int ht = 0;

		if (point.y < rcBorders.top)
			ht = (HTTOP - HTSIZEFIRST + 1);
		else if (point.y >= rcBorders.bottom)
			ht = (HTBOTTOM - HTSIZEFIRST + 1);

		if (point.x < rcBorders.left)
			ht += (HTLEFT - HTSIZEFIRST + 1);
		else if (point.x >= rcBorders.right)
			ht += (HTRIGHT - HTSIZEFIRST + 1);

		return (LRESULT)(ht + HTSIZEFIRST - 1);
	}

	if (nHitTest == HTNOWHERE)
	{
		int nPoint = point.y - rcWindow.top;
		if (nPoint < GetPaintManager()->GetCaptionHeight() + XTP_DPI_Y(4))
			return HTCAPTION;
	}

	return nHitTest;
}

void CXTPDockingPaneMiniWnd::RedrawPane()
{
	if (!IsThemed())
		return;

	if (GetSafeHwnd())
		::InvalidateRect(GetSafeHwnd(), 0, FALSE);

	SendMessage(WM_NCPAINT);
}

void CXTPDockingPaneMiniWnd::OnNcPaint()
{
	if (IsThemed())
	{
		CWindowDC dc(this);

		CRect rcClient;
		GetClientRect(rcClient);
		CRect rcWindow;
		GetWindowRect(rcWindow);
		ScreenToClient(rcWindow);

		rcClient.OffsetRect(-rcWindow.left, -rcWindow.top);
		dc.ExcludeClipRect(rcClient);

		CXTPDockingPanePaintManager* pPaintManager = GetPaintManager();

		CXTPWindowRect rc(this);
		rc.OffsetRect(-rc.TopLeft());
		pPaintManager->DrawFloatingFrame(&dc, this, rc);
	}
	else
	{
		Default();
	}
}

BOOL CXTPDockingPaneMiniWnd::OnNcActivate(BOOL bActive)
{
	if (IsThemed())
	{
		return TRUE;
	}

	return CMiniFrameWnd::OnNcActivate(bActive);
}

void CXTPDockingPaneMiniWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	if (IsThemed())
	{
		int nSize = 3;

		lpncsp->rgrc[0].left += XTP_DPI_X(nSize);
		lpncsp->rgrc[0].top += XTP_DPI_Y(nSize);
		lpncsp->rgrc[0].right -= XTP_DPI_X(nSize);
		lpncsp->rgrc[0].bottom -= XTP_DPI_Y(nSize);

		int nTitleFont = GetPaintManager()->GetCaptionHeight();
		lpncsp->rgrc[0].top += nTitleFont + XTP_DPI_Y(2);
		return;
	}

	CMiniFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CXTPDockingPaneMiniWnd::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CMiniFrameWnd::OnNcMouseMove(nHitTest, point);

	if (IsThemed())
	{
		CXTPDrawHelpers::ScreenToWindow(this, &point);

		if (m_pCaptionButtons->CheckForMouseOver(point))
		{
			SetTimer(TID_NCLEAVE, 100, NULL);
		}

		Expand();
	}
}

void CXTPDockingPaneMiniWnd::Collapse(BOOL bDelay)
{
	m_nSlideStep = m_nStepsCount = ResetStepsCount();

	CWnd::GetWindowRect(&m_rcWindow);

	m_nExpandedHeight = m_rcWindow.Height();

	m_bCollapsed = TRUE;
	m_bExpanded  = TRUE;

	if (!bDelay)
	{
		if (!OnAction(xtpPaneActionCollapsing))
		{
			m_nSlideStep = 0;
			DoSlideStep(FALSE);
			KillTimer(TID_SLIDEOUT);
			KillTimer(TID_CHECKACTIVE);
			OnAction(xtpPaneActionCollapsed);
			return;
		}
	}

	if (m_bSlideOut)
	{
		m_bSlideOut = FALSE;
		KillTimer(TID_SLIDEOUT);
		OnAction(xtpPaneActionCollapsed);
	}

	m_nDeactivationCount = 6;
	SetTimer(TID_CHECKACTIVE, 100, NULL);
}

void CXTPDockingPaneMiniWnd::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		OnClose();
		return;
	}
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		OnPinButtonClick();
		return;
	}
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_MENU)
	{
	}
}

BOOL CXTPDockingPaneMiniWnd::OnCaptionButtonDown(CXTPDockingPaneCaptionButton* pButton)
{
	BOOL bHandled = FALSE;
	CXTPDockingPane* pSelectedPane;

	switch (pButton->GetID())
	{
		case XTP_IDS_DOCKINGPANE_MENU:
			pSelectedPane = GetSelectedPane();
			if (pSelectedPane)
			{
				m_bActive = TRUE;

				CXTPDockingPaneManager* pManager = GetDockingPaneManager();
				XTP_DOCKINGPANE_CLICK menu;

				menu.pContainer = this;
				menu.rcExclude  = pButton->GetRect();
				CXTPWindowRect rcWindow(this);

				if (GetExStyle() & WS_EX_LAYOUTRTL)
				{
					menu.rcExclude.OffsetRect(-menu.rcExclude.left + rcWindow.right
												  - menu.rcExclude.right,
											  rcWindow.top);
					menu.pt = CPoint(menu.rcExclude.right, menu.rcExclude.bottom);
				}
				else
				{
					menu.rcExclude.OffsetRect(rcWindow.left, rcWindow.top);
					menu.pt = CPoint(menu.rcExclude.left, menu.rcExclude.bottom);
				}

				menu.pPane			= pSelectedPane;
				pButton->m_bPressed = TRUE;
				SendMessage(WM_NCPAINT);

				pManager->NotifyOwner(XTP_DPN_CONTEXTMENU, (LPARAM)&menu);

				pButton->m_bPressed = FALSE;
				if (m_hWnd)
					SendMessage(WM_NCPAINT);

				bHandled = TRUE;
			}
			break;

		case XTP_IDS_DOCKINGPANE_MAXIMIZE:
			ShowWindow(SW_MAXIMIZE);
			bHandled = TRUE;
			break;

		case XTP_IDS_DOCKINGPANE_RESTORE:
			ShowWindow(SW_RESTORE);
			bHandled = TRUE;
			break;
	}
	return bHandled;
}

BOOL CXTPDockingPaneMiniWnd::OnAction(XTPDockingPaneAction action)
{
	if (m_pTopContainer)
	{
		return m_pTopContainer->OnAction(action);
	}

	return FALSE;
}

void CXTPDockingPaneMiniWnd::OnPinButtonClick()
{
	if (!m_hWnd)
		return;

	BOOL bPinning = m_bCollapsed;

	if (OnAction(bPinning ? xtpPaneActionPinning : xtpPaneActionUnpinning))
		return;

	if (!m_bCollapsed)
	{
		Collapse(TRUE);
	}
	else
	{
		if (m_nStepsCount != m_nSlideStep)
		{
			CXTPWindowRect rcWindow(this);
			SetWindowPos(0, 0, 0, rcWindow.Width(), m_nExpandedHeight, SWP_NOZORDER | SWP_NOMOVE);
		}
		m_bCollapsed = FALSE;
		m_bExpanded  = FALSE;

		KillTimer(TID_CHECKACTIVE);
		KillTimer(TID_SLIDEOUT);
	}
	SendMessage(WM_NCPAINT);

	OnAction(bPinning ? xtpPaneActionPinned : xtpPaneActionUnpinned);
}

void CXTPDockingPaneMiniWnd::DoSlideStep(BOOL bActivate)
{
	int nMinHeight = GetPaintManager()->GetCaptionHeight() + XTP_DPI_Y(8);

	CXTPWindowRect rc(this);
	rc.bottom = rc.top + max(nMinHeight, m_nSlideStep * m_nExpandedHeight / m_nStepsCount);

	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(),
				 UINT(!bActivate ? SWP_NOZORDER | SWP_NOACTIVATE : SWP_NOACTIVATE));
	Invalidate(FALSE);
}

CXTPDockingPane* CXTPDockingPaneMiniWnd::GetSelectedPane() const
{
	if (!m_pTopContainer)
		return 0;

	CXTPDockingPaneBaseList lst;
	FindPane(xtpPaneTypeTabbedContainer, &lst);
	int nCount = (int)lst.GetCount();
	if (nCount != 1)
		return 0;

	return ((CXTPDockingPaneTabbedContainer*)lst.GetHead())->GetSelected();
}

BOOL CXTPDockingPaneMiniWnd::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton)
{
	BOOL bVisible = TRUE;
	WINDOWPLACEMENT wndpl;
	CXTPDockingPane* pSelectedPane;

	switch (pButton->GetID())
	{
		case XTP_IDS_DOCKINGPANE_CLOSE: bVisible = m_bCloseEnabled; break;

		case XTP_IDS_DOCKINGPANE_AUTOHIDE:
			pButton->SetState(DWORD(m_bCollapsed ? xtpPanePinPushed : 0));
			bVisible = m_bShowPinButton;
			break;

		case XTP_IDS_DOCKINGPANE_MENU:
			pSelectedPane = GetSelectedPane();
			bVisible	  = pSelectedPane && (pSelectedPane->GetOptions() & xtpPaneHasMenuButton);
			break;

		case XTP_IDS_DOCKINGPANE_MAXIMIZE:
			bVisible = (GetDockingPaneManager()->IsFloatingCaptionMaximizeButtonsVisible()
						&& GetWindowPlacement(&wndpl) && SW_SHOWNORMAL == wndpl.showCmd);
			break;

		case XTP_IDS_DOCKINGPANE_RESTORE:
			bVisible = (GetDockingPaneManager()->IsFloatingCaptionMaximizeButtonsVisible()
						&& GetWindowPlacement(&wndpl) && SW_SHOWMAXIMIZED == wndpl.showCmd);
			break;
	}

	return bVisible;
}

void CXTPDockingPaneMiniWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (IsThemed())
	{
		CPoint ptClick(point);
		CXTPDrawHelpers::ScreenToWindow(this, &ptClick);

		for (int i = 0; i < m_pCaptionButtons->GetSize(); i++)
		{
			CXTPDockingPaneCaptionButton* pButton = m_pCaptionButtons->GetAt(i);
			if (pButton->PtInRect(ptClick))
			{
				if (!pButton->IsEnabled())
					return;

				if (OnCaptionButtonDown(pButton))
					return;

				if (pButton->Click(this, ptClick, FALSE))
				{
					OnCaptionButtonClick(pButton);
				}
				return;
			}
		}
	}

	if (nHitTest == HTCAPTION)
	{
		// special activation for floating toolbars
		ActivateTopParent();
		BringWindowToTop();

		CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();
		pContext->Drag(m_pTopContainer, point);

		return;
	}

	CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CXTPDockingPaneMiniWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_NCLEAVE)
	{
		CPoint pt;
		::GetCursorPos(&pt);

		CXTPWindowRect rcCaption(this);
		rcCaption.bottom = rcCaption.top + GetPaintManager()->GetCaptionHeight() + 4;

		if (!rcCaption.PtInRect(pt))
		{
			m_pCaptionButtons->CheckForMouseOver(CPoint(-1, -1));

			KillTimer(TID_NCLEAVE);
		}
	}

	if (nIDEvent == TID_SLIDEOUT && m_bSlideOut)
	{
		m_nSlideStep--;

		if (m_nSlideStep > -1)
			DoSlideStep();
		else
		{
			m_bSlideOut = FALSE;
			m_bExpanded = FALSE;
			KillTimer(TID_SLIDEOUT);
			OnAction(xtpPaneActionCollapsed);
		}
	}

	if (nIDEvent == TID_CHECKACTIVE)
	{
		CPoint pt;
		GetCursorPos(&pt);

		CWnd* pFocus = GetFocus();
		BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus)));

		if (!bActive && !m_bSlideOut && !CXTPWindowRect(this).PtInRect(pt)
			&& ::GetCapture() == NULL)
		{
			if (--m_nDeactivationCount <= 0)
			{
				if (OnAction(xtpPaneActionCollapsing))
				{
					m_nDeactivationCount = 6;
					return;
				}

				if (m_nSlideStep == m_nStepsCount)
				{
					m_nExpandedHeight = CXTPWindowRect(this).Height();
				}

				m_bSlideOut = TRUE;
				SetTimer(TID_SLIDEOUT, UINT(CXTPDockingPaneAutoHideWnd::m_nAnimationInterval),
						 NULL);

				KillTimer(TID_CHECKACTIVE);
			}
		}
	}

	CMiniFrameWnd::OnTimer(nIDEvent);
}

void CXTPDockingPaneMiniWnd::OnThemedChanged()
{
	if (!m_hWnd)
		return;

	ModifyStyle(DWORD(IsThemed() ? WS_BORDER | WS_THICKFRAME : 0),
				DWORD(IsThemed() ? 0 : WS_BORDER | WS_THICKFRAME), SWP_FRAMECHANGED);

	if (!IsThemed() && m_bCollapsed)
	{
		OnPinButtonClick();
	}

	SetWindowPos(0, 0, 0, 0, 0,
				 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	if (m_bCollapsed && IsThemed())
	{
		DoSlideStep();
	}
}

void CXTPDockingPaneMiniWnd::Expand()
{
	if (m_bCollapsed && m_nSlideStep <= 0)
	{
		if (m_bSlideOut)
		{
			m_bSlideOut = FALSE;
			m_bExpanded = FALSE;
			KillTimer(TID_SLIDEOUT);
			OnAction(xtpPaneActionCollapsed);
		}

		if (OnAction(xtpPaneActionExpanding))
			return;

		m_bExpanded  = TRUE;
		m_nSlideStep = m_nStepsCount = ResetStepsCount();
		DoSlideStep(TRUE);

		m_nDeactivationCount = 8;
		SetTimer(TID_CHECKACTIVE, 100, NULL);

		OnAction(xtpPaneActionExpanded);
	}
}

void CXTPDockingPaneMiniWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CMiniFrameWnd::OnGetMinMaxInfo(lpMMI);

	if (IsThemed())
	{
		lpMMI->ptMinTrackSize.x = 46;
	}

	GetMinMaxInfo(lpMMI);
}

void CXTPDockingPaneMiniWnd::GetMinMaxInfo(LPMINMAXINFO lpMMI) const
{
	if (m_pTopContainer && ((!m_bCollapsed || m_bExpanded) && !m_bSlideOut))
	{
		CXTPWindowRect rcWindow(this);
		CXTPClientRect rcClient(this);
		CSize szBorder = rcWindow.Size() - rcClient.Size();

		MINMAXINFO mInfo;
		m_pTopContainer->GetMinMaxInfo(&mInfo);

		lpMMI->ptMinTrackSize.x = max(lpMMI->ptMinTrackSize.x,
									  mInfo.ptMinTrackSize.x + szBorder.cx);
		lpMMI->ptMinTrackSize.y = max(lpMMI->ptMinTrackSize.y,
									  mInfo.ptMinTrackSize.y + szBorder.cy);

		lpMMI->ptMaxTrackSize.x = min(lpMMI->ptMaxTrackSize.x,
									  mInfo.ptMaxTrackSize.x + szBorder.cx);
		lpMMI->ptMaxTrackSize.y = min(lpMMI->ptMaxTrackSize.y,
									  mInfo.ptMaxTrackSize.y + szBorder.cy);
	}
}

BOOL CXTPDockingPaneMiniWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (pManager)
	{
		pManager->GetToolTipContext()->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

INT_PTR CXTPDockingPaneMiniWnd::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT(this);
	ASSERT(::IsWindow(m_hWnd));

	if (!IsThemed())
		return -1;

	CXTPWindowRect rcWindow(this);
	CPoint ptTopLeft(rcWindow.TopLeft());
	ScreenToClient(&ptTopLeft);

	if (GetExStyle() & WS_EX_LAYOUTRTL)
		ptTopLeft.x -= rcWindow.Width();

	for (int i = 0; i < m_pCaptionButtons->GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = m_pCaptionButtons->GetAt(i);
		CRect rc							  = pButton->GetRect();
		rc.OffsetRect(ptTopLeft);

		if (rc.PtInRect(point))
		{
			INT_PTR nHit = (INT_PTR)pButton->GetID();

			CString strTip;
			XTPResourceManager()->LoadString(&strTip, (UINT)nHit);

			if (strTip.GetLength() == 0)
				return -1;

			CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, rc, nHit, strTip);
			return nHit;
		}
	}

	return -1;
}

void CXTPDockingPaneMiniWnd::DeletePane()
{
	InternalRelease();
}

void CXTPDockingPaneMiniWnd::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPDockingPaneMiniWnd, CMiniFrameWnd)
	DISP_FUNCTION_ID(CXTPDockingPaneMiniWnd, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPDockingPaneMiniWnd, "Pane", DISPID_VALUE, OleGetPane, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPDockingPaneMiniWnd, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneMiniWnd, "ParentContainer", 20, OleGetContainer,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneMiniWnd, "Type", 21, OleGetType, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneMiniWnd, "hWnd", DISPID_HWND, OleGetHwnd, SetNotSupported,
						VT_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneMiniWnd, "IsEmpty", 22, OleIsEmpty, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneMiniWnd, "Position", 23, OleGetPosition, SetNotSupported,
						VT_I4)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDockingPaneMiniWnd, XTPDIID_IPaneContainer)

BEGIN_INTERFACE_MAP(CXTPDockingPaneMiniWnd, CMiniFrameWnd)
	INTERFACE_PART(CXTPDockingPaneMiniWnd, XTPDIID_IPaneContainer, Dispatch)
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE_PART(CXTPDockingPaneMiniWnd)
END_INTERFACE_MAP()

IMPLEMENT_ENUM_VARIANT(CXTPDockingPaneMiniWnd);
XTP_IMPLEMENT_CMDTARGETPROVIDER_INTERFACE(CXTPDockingPaneMiniWnd)

BOOL CXTPDockingPaneMiniWnd::OleIsEmpty()
{
	return m_pTopContainer ? m_pTopContainer->IsEmpty() : TRUE;
}

LPDISPATCH CXTPDockingPaneMiniWnd::OleGetContainer()
{
	return m_pParentContainer ? m_pParentContainer->OleGetDispatch(TRUE) : NULL;
}

int CXTPDockingPaneMiniWnd::OleGetPosition()
{
	return GetDockingPaneManager()->GetPaneDirection(this);
}

int CXTPDockingPaneMiniWnd::OleGetType()
{
	return m_type;
}

HWND CXTPDockingPaneMiniWnd::OleGetHwnd()
{
	return m_hWnd;
}

int CXTPDockingPaneMiniWnd::OleGetItemCount()
{
	return 1;
}

LPDISPATCH CXTPDockingPaneMiniWnd::OleGetPane(int nIndex)
{
	return OleGetItem(nIndex - 1);
}

LPDISPATCH CXTPDockingPaneMiniWnd::OleGetItem(int nIndex)
{
	if (nIndex != 0 || !m_pTopContainer)
		return NULL;

	return m_pTopContainer->OleGetDispatch(TRUE);
}

LPDISPATCH CXTPDockingPaneMiniWnd::OleGetDispatch(BOOL bAddRef)
{
	return GetIDispatch(bAddRef);
}

#endif
