// XTPTearOffFrame.cpp : implementation of the CXTPTearOffFrame class.
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

#include "CommandBars/Resource.h"

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#	include "Controls/Edit/XTPEdit.h"
#endif

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
#		include "DockingPane/XTPDockingPaneDefines.h"
#	endif
#endif

#include "TabManager/Includes.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2010.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2012.h"
#include "TabManager/Themes/XTPTabThemeOffice2013.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2015.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2017.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2019.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2022.h"
#include "TabManager/Themes/XTPTabThemeNativeWindows10.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPFrameWnd.h"
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/TabClient/Workspace.h"
#include "CommandBars/TabClient/SingleWorkspace.h"
#include "CommandBars/TabClient/TabClientDropTarget.h"
#include "CommandBars/TabClient/NavigateButtonActiveFiles.h"
#include "CommandBars/TabClient/XTPTabClientDraggingContents.h"
#include "CommandBars/TabClient/XTPTabClientSticker.h"
#include "CommandBars/TabClient/XTPTearOffFrame.h"
#include "CommandBars/TabClient/XTPTabClientContext.h"

#pragma warning(disable : 4510)
#pragma warning(disable : 4610)

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

IMPLEMENT_DYNCREATE(CXTPTearOffFrame, CXTPMDIFrameWnd)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTearOffFrame, CXTPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_XTP_NEWTABITEM, OnTabbarNewItem)
	ON_MESSAGE(WM_XTP_SETACTIVEWORKSPACE, OnSetActiveWorkspace)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPTearOffFrame::CXTPTearOffFrame()
{
	m_bIsFrameDragging = FALSE;
	m_hCursor		   = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorNew	   = XTPResourceManager()->LoadCursor(XTP_IDC_WORKSPACE_NEW);
	ASSERT(NULL != m_hCursorNew);

	m_pTabClientContext = new CXTPTabClientContext();

	pTargetTabPrev = NULL;
}

CXTPTearOffFrame::~CXTPTearOffFrame()
{
	SAFE_DELETE(m_pTabClientContext);
}

HMENU CXTPTearOffFrame::GetWindowMenuPopup(HMENU /*hMenuBar*/)
{
	return NULL;
}

int CXTPTearOffFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetMenu(NULL);

	if (!InitCommandBars())
		return -1;

	if (!GetCommandBars()->SetMenu(_T("Menu Bar"), IDR_XTP_TEAROFF_FRM))
		return -1;

	GetCommandBars()->GetMenuBar()->SetVisible(FALSE);

	if (!m_wndTabClient.Attach(GetCommandBars(), this, TRUE))
		return -1;

	m_wndTabClient.EnableTearOff();

	m_pTabClientContext->SetTabClient(&m_wndTabClient);

	return 0;
}

BOOL CXTPTearOffFrame::DestroyWindow()
{
	if (m_wndTabClient.m_lstNextTab.GetCount() == 0)
	{
		// Last frame
		ASSERT(m_wndTabClient.m_lstPrevTab.GetCount() == 1);

		CXTPTabClientWnd* pPrevTab = (CXTPTabClientWnd*)m_wndTabClient.m_lstPrevTab.GetHead();
		ASSERT(pPrevTab);

		POSITION thisPos = pPrevTab->m_lstNextTab.Find(&m_wndTabClient);
		ASSERT(thisPos);

		pPrevTab->m_lstNextTab.RemoveAt(thisPos);
	}
	else
	{
		{
			// Replace all 'Parent.Next' with 'this.Next'
			ASSERT(m_wndTabClient.m_lstPrevTab.GetCount() == 1);
			CXTPTabClientWnd* pPrevTab = (CXTPTabClientWnd*)m_wndTabClient.m_lstPrevTab.GetHead();
			ASSERT(pPrevTab);
			POSITION thisPos = pPrevTab->m_lstNextTab.Find(&m_wndTabClient);
			pPrevTab->m_lstNextTab.RemoveAt(thisPos);

			POSITION pos = m_wndTabClient.m_lstNextTab.GetHeadPosition();
			while (pos != NULL)
			{
				CXTPTabClientWnd* pTab = (CXTPTabClientWnd*)m_wndTabClient.m_lstNextTab.GetNext(
					pos);
				if (pTab && IsWindow(pTab->GetSafeHwnd()))
				{
					pPrevTab->m_lstNextTab.AddTail(pTab);
				}
			}
		}

		{
			// Replace all 'Child.Prev' with 'this.Prev'
			POSITION pos = m_wndTabClient.m_lstNextTab.GetHeadPosition();
			while (pos != NULL)
			{
				CXTPTabClientWnd* pNextTab = (CXTPTabClientWnd*)m_wndTabClient.m_lstNextTab.GetNext(
					pos);

				POSITION thisPos = pNextTab->m_lstPrevTab.Find(&m_wndTabClient);
				pNextTab->m_lstPrevTab.RemoveAt(thisPos);

				CXTPTabClientWnd* pPrevTab = (CXTPTabClientWnd*)
												 m_wndTabClient.m_lstPrevTab.GetHead();
				pNextTab->m_lstPrevTab.AddTail(pPrevTab);
			}
		}
	}

	return CXTPMDIFrameWnd::DestroyWindow();
}

void CXTPTearOffFrame::Init(CXTPTabClientWnd* pTabClient, CXTPTabManagerItem* pTabItem)
{
	ASSERT(pTabClient);
	ASSERT(pTabItem);

	CRect rc;
	POINT pt;
	GetCursorPos(&pt);
	pTabClient->GetClientRect(&rc);
	MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());

	SetIcon(pTabClient->GetItemIcon(pTabItem), FALSE);

	Update(pTabClient);

	m_pTabClientContext->SetTabClient(GetTabClient());

	ShowWindow(SW_SHOW);
	UpdateWindow();

	m_wndTabClient.m_lstPrevTab.AddTail(pTabClient);

	AfxGetMainWnd()->SendMessage(WM_XTP_TEAROFF_ACTIVATE, (WPARAM)this, 0);
}

void CXTPTearOffFrame::Update(CXTPTabClientWnd* pTabClient)
{
	ASSERT_VALID(pTabClient);

	CXTPTabPaintManager* pTabPaintManager = pTabClient->GetPaintManager();
	if (pTabPaintManager)
	{
		pTabPaintManager->InternalAddRef();
		m_wndTabClient.SetPaintManager(pTabPaintManager);
	}

	m_wndTabClient.m_bEnableGroups = pTabClient->IsGroupingEnabled();

	m_wndTabClient.SetShowTabClientContextStickers(pTabClient->GetShowTabClientContextStickers());
	m_wndTabClient.SetAlphaTabClientContext(pTabClient->GetAlphaTabClientContext());
	m_wndTabClient.SetShowContentsWhileDragging(pTabClient->GetShowContentsWhileDragging());
	m_wndTabClient.EnableFloatingFrameTheme(pTabClient->IsFloatingFrameThemeEnabled());

	m_pTabClientContext->SetShowTabClientContextStickers(
		pTabClient->GetShowTabClientContextStickers());
	m_pTabClientContext->SetAlphaTabClientContext(pTabClient->GetAlphaTabClientContext());
	m_pTabClientContext->SetShowContentsWhileDragging(pTabClient->GetShowContentsWhileDragging());
	m_pTabClientContext->EnableFloatingFrameTheme(pTabClient->IsFloatingFrameThemeEnabled());

	GetCommandBars()->EnableFrameTheme(pTabClient->IsFloatingFrameThemeEnabled()
										   ? pTabClient->GetCommandBars()->IsEnableFrameTheme()
										   : FALSE);
}

CXTPTabClientWnd* CXTPTearOffFrame::GetTabClient() const
{
	return const_cast<CXTPTabClientWnd*>(&m_wndTabClient);
}

LRESULT CXTPTearOffFrame::OnEnterSizeMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	GetWindowRect(&m_rcPrev);

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	return (LRESULT)0;
}

void CXTPTearOffFrame::OnMove(int x, int y)
{
	CXTPMDIFrameWnd::OnMove(x, y);

	CRect rcCurr;
	GetWindowRect(&rcCurr);

	if (m_rcPrev.Width() == rcCurr.Width() && m_rcPrev.Height() == rcCurr.Height())
	{
		m_bIsFrameDragging = TRUE;

		POINT pt = { 0 };
		::GetCursorPos(&pt);
		CXTPTabClientWnd* pTargetTab = m_wndTabClient.FindTargetTabClient(pt);
		if (XTP_SAFE_GET1(pTargetTab, GetParent(), NULL)
			!= CWnd::FromHandle(::GetForegroundWindow()))
			XTP_SAFE_CALL2(pTargetTab, GetParent(), SetForegroundWindow());

		m_pTabClientContext->EnableTabSticker();
		m_pTabClientContext->EnableTabFocusRect();
		m_pTabClientContext->UpdateTabSticker();
		m_pTabClientContext->UpdateTabContextWindow();

		UpdateDragRect();
		m_pTabClientContext->UpdateTabFocusRect(m_rcDragRect);
	}
}

LRESULT CXTPTearOffFrame::OnExitSizeMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	ASSERT(m_pTabClientContext);

	CRect rcCurr;
	GetWindowRect(&rcCurr);

	if (m_rcPrev.Width() == rcCurr.Width() && m_rcPrev.Height() == rcCurr.Height())
	{
		POINT pt;
		::GetCursorPos(&pt);

		BOOL bStickers = m_pTabClientContext->GetShowTabClientContextStickers();
		if (bStickers)
		{
			XTPTabClientStickerPart nSticker =
				m_pTabClientContext->GetLastSelectedTabClientSticker();

			if (nSticker != xtpTabClientStickerNone)
			{
				CXTPTabClientWnd* pTargetTab =
					m_pTabClientContext->GetSticker()->GetTargetTabClientWnd();
				if (pTargetTab)
				{
					int nWorkspace = m_pTabClientContext->GetLastWorkspace();
					if (nWorkspace != -1)
					{
						pTargetTab->GetWorkspace(nWorkspace)
							->OnItemClick(pTargetTab->GetWorkspace(nWorkspace)->GetItem(0));
					}

					ChangeMDIChildFrame(pTargetTab, nWorkspace, nSticker);
				}
			}
		}
		else
		{
			CXTPTabClientWnd* pTargetTab = m_wndTabClient.FindTargetTabClient(pt);
			if (pTargetTab)
			{
				POINT ptWorkspace(pt);
				pTargetTab->ScreenToWorkspace(&ptWorkspace);
				int nWorkspace		= -1;
				int nWorkspaceCount = pTargetTab->GetWorkspaceCount();
				for (int i = 0; i < nWorkspaceCount; i++)
				{
					if (pTargetTab->GetWorkspace(i)->GetHeaderRect().PtInRect(ptWorkspace)
						|| pTargetTab->GetWorkspace(i)->GetClientRect().PtInRect(ptWorkspace))
					{
						nWorkspace = i;
						break;
					}
				}

				XTPTabClientStickerPart type = xtpTabClientStickerNone;
				if (nWorkspace == -1)
				{
					type = xtpTabClientStickerCenter;
				}
				else
				{
					ASSERT(nWorkspace > -1);
					CRect rcHeader = pTargetTab->GetWorkspace(nWorkspace)->GetHeaderRect();
					CRect rcClient = pTargetTab->GetWorkspace(nWorkspace)->GetClientRect();
					pTargetTab->ClientToScreen(&rcHeader);
					pTargetTab->ClientToScreen(&rcClient);
					int delta = 50;

					if (rcHeader.PtInRect(pt))
					{
						type = xtpTabClientStickerCenter;
					}
					else if (rcClient.PtInRect(pt))
					{
						CRect rcLeft(rcClient);
						CRect rcTop(rcClient);
						CRect rcRight(rcClient);
						CRect rcBottom(rcClient);

						rcLeft.right = rcLeft.left + XTP_DPI_X(delta);
						rcTop.bottom = rcTop.top + XTP_DPI_Y(delta);
						rcRight.left = rcRight.right - XTP_DPI_X(delta);
						rcBottom.top = rcBottom.bottom - XTP_DPI_Y(delta);

						if (rcLeft.PtInRect(pt))
							type = xtpTabClientStickerLeft;
						else if (rcTop.PtInRect(pt))
							type = xtpTabClientStickerTop;
						else if (rcRight.PtInRect(pt))
							type = xtpTabClientStickerRight;
						else if (rcBottom.PtInRect(pt))
							type = xtpTabClientStickerBottom;
					}

					if (type != xtpTabClientStickerNone)
					{
						pTargetTab->GetWorkspace(nWorkspace)
							->OnItemClick(pTargetTab->GetWorkspace(nWorkspace)->GetItem(0));
					}
				}

				if (type != xtpTabClientStickerNone)
					ChangeMDIChildFrame(pTargetTab, nWorkspace, type);
			}
		}

		m_pTabClientContext->DisableTabSticker();
		m_pTabClientContext->DisableTabContextWindow();
		m_pTabClientContext->DisableTabFocusRect();
	}

	m_bIsFrameDragging = FALSE;

	::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	return (LRESULT)0;
}

void CXTPTearOffFrame::UpdateDragRect()
{
	if (!m_wndTabClient)
		return;

	CRect rcDragRect(m_rcDragRect);
	m_rcDragRect.SetRectEmpty();

	POINT pos;
	::GetCursorPos(&pos);

	CXTPTabClientWnd* pTargetTab = m_wndTabClient.FindTargetTabClient(pos);
	if (pTargetTab)
	{
		POINT ptWorkspace(pos);
		pTargetTab->ScreenToWorkspace(&ptWorkspace);
		int nWorkspace		= -1;
		int nWorkspaceCount = pTargetTab->GetWorkspaceCount();
		for (int i = 0; i < nWorkspaceCount; i++)
		{
			if (pTargetTab->GetWorkspace(i)->GetHeaderRect().PtInRect(ptWorkspace)
				|| pTargetTab->GetWorkspace(i)->GetClientRect().PtInRect(ptWorkspace))
			{
				nWorkspace = i;
				break;
			}
		}

		if (nWorkspace == -1)
		{
			pTargetTab->GetWindowRect(&m_rcDragRect);
			pTargetTab->ScreenToClient(&m_rcDragRect);
		}
		else
		{
			ASSERT(nWorkspace > -1);
			CRect rcHeader = pTargetTab->GetWorkspace(nWorkspace)->GetHeaderRect();
			CRect rcClient = pTargetTab->GetWorkspace(nWorkspace)->GetClientRect();
			pTargetTab->ClientToScreen(&rcHeader);
			pTargetTab->ClientToScreen(&rcClient);
			int delta = 50;

			if (rcHeader.PtInRect(pos))
			{
				m_rcDragRect = pTargetTab->GetWorkspace(nWorkspace)->GetControlRect();
			}
			else if (rcClient.PtInRect(pos))
			{
				CRect rcLeft(rcClient);
				CRect rcTop(rcClient);
				CRect rcRight(rcClient);
				CRect rcBottom(rcClient);

				rcLeft.right = rcLeft.left + XTP_DPI_X(delta);
				rcTop.bottom = rcTop.top + XTP_DPI_Y(delta);
				rcRight.left = rcRight.right - XTP_DPI_X(delta);
				rcBottom.top = rcBottom.bottom - XTP_DPI_Y(delta);

				if (rcLeft.PtInRect(pos))
				{
					m_rcDragRect	   = pTargetTab->GetWorkspace(nWorkspace)->GetControlRect();
					m_rcDragRect.right = m_rcDragRect.CenterPoint().x;
				}
				else if (rcTop.PtInRect(pos))
				{
					m_rcDragRect		= pTargetTab->GetWorkspace(nWorkspace)->GetControlRect();
					m_rcDragRect.bottom = m_rcDragRect.CenterPoint().y;
				}
				else if (rcRight.PtInRect(pos))
				{
					m_rcDragRect	  = pTargetTab->GetWorkspace(nWorkspace)->GetControlRect();
					m_rcDragRect.left = m_rcDragRect.CenterPoint().x;
				}
				else if (rcBottom.PtInRect(pos))
				{
					m_rcDragRect	 = pTargetTab->GetWorkspace(nWorkspace)->GetControlRect();
					m_rcDragRect.top = m_rcDragRect.CenterPoint().y;
				}
			}
		}
	}

	if (pTargetTab != pTargetTabPrev || !rcDragRect.EqualRect(m_rcDragRect))
	{
		m_pTabClientContext->DisableTabFocusRect();

		UINT uFlags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN;
		XTP_SAFE_CALL1(pTargetTabPrev, RedrawWindow(NULL, NULL, uFlags));
	}

	pTargetTabPrev = pTargetTab;
}

void CXTPTearOffFrame::ChangeMDIChildFrame(CXTPTabClientWnd* pTarget, int nOldWorkspace,
										   int nSticker)
{
	if (!pTarget)
		return;

	CXTPTabClientWnd::CWorkspace* pNewWorkspace = NULL;

	CMDIChildWnd* pMDIChildWnd = DYNAMIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
	while (pMDIChildWnd)
	{
		pMDIChildWnd->SetParent(CWnd::FromHandle(pTarget->GetParentFrame()->m_hWndMDIClient));

		CMDIChildWnd* pDummyChild = new CMDIChildWnd();
		if (pDummyChild->Create(NULL, _T("Dummy"), WS_CHILD | WS_VISIBLE, CFrameWnd::rectDefault,
								GetTabClient()->GetParentFrame(), NULL))
		{
			::SendMessage(GetTabClient()->GetParentFrame()->m_hWndMDIClient, WM_MDIACTIVATE,
						  (WPARAM)pDummyChild->GetSafeHwnd(), 0);
			::SendMessage(pDummyChild->m_hWnd, WM_CLOSE, 0, 0);
		}
		else
		{
			ASSERT(FALSE);
			delete pDummyChild;
		}

		pNewWorkspace = ChangeItemWorkspace(pTarget, nOldWorkspace, nSticker, pNewWorkspace);

		pMDIChildWnd = DYNAMIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
	}

	if (!MDIGetActive())
		PostMessage(WM_CLOSE);
}

CXTPTabClientWnd::CWorkspace* CXTPTearOffFrame::ChangeItemWorkspace(
	CXTPTabClientWnd* pTarget, int nOldWorkspace, int nSticker,
	CXTPTabClientWnd::CWorkspace* pExistingWorkspace)
{
	if (!pTarget || nOldWorkspace == -1 || nSticker == xtpTabClientStickerNone
		|| nSticker == xtpTabClientStickerCenter)
		return NULL;

	pTarget->UpdateContents();

	CXTPTabManagerItem* pItem = pTarget->GetSelectedItem();
	if (pItem)
	{
		pItem->InternalAddRef();
		pItem->Remove();

		if (!pExistingWorkspace)
		{
			int nIndex = (nSticker == xtpTabClientStickerLeft || nSticker == xtpTabClientStickerTop)
							 ? nOldWorkspace
							 : nOldWorkspace + 1;

			pExistingWorkspace = pTarget->AddWorkspace(nIndex);
			pExistingWorkspace->AddNewWindowItem();

			pTarget->m_bHorizSplitting = (nSticker == xtpTabClientStickerTop
										  || nSticker == xtpTabClientStickerBottom)
											 ? TRUE
											 : FALSE;
		}

		pExistingWorkspace->CXTPTabManager::AddItem(0, pItem);
	}

	return pExistingWorkspace;
}

BOOL CXTPTearOffFrame::IsFrameBeingDragged() const
{
	return m_bIsFrameDragging;
}

void CXTPTearOffFrame::OnClose()
{
	CMDIChildWnd* pMDIChildWnd = DYNAMIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
	while (pMDIChildWnd)
	{
		pMDIChildWnd->SendMessage(WM_CLOSE, 0, 0);

		if (MDIGetActive() == pMDIChildWnd)
			return;

		pMDIChildWnd = DYNAMIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
	}

	CFrameWnd::OnClose();
}

LRESULT CXTPTearOffFrame::OnTabbarMouseMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN
		|| wParam == WM_LBUTTONDBLCLK || wParam == WM_RBUTTONDBLCLK)
	{
		TTHITTESTINFO info = { 0 };
		info.hwnd		   = GetTabClient()->m_hWnd;
		info.pt			   = CPoint(lParam);

		return AfxGetMainWnd()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG_EX, wParam, (LPARAM)&info);
	}

	return FALSE;
}

void CXTPTearOffFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		AfxGetMainWnd()->SendMessage(WM_XTP_TEAROFF_ACTIVATE, (WPARAM)this, 0);
	}
}

LRESULT CXTPTearOffFrame::OnTabbarNewItem(WPARAM wParam, LPARAM /*lParam*/)
{
	CXTPTabManagerItem* pItem = (CXTPTabManagerItem*)wParam;
	if (pItem)
	{
		CXTPMDIFrameWndEx* pMDIFrameWndEx = DYNAMIC_DOWNCAST(CXTPMDIFrameWndEx, AfxGetMainWnd());
		if (pMDIFrameWndEx)
		{
			return pMDIFrameWndEx->UpdateMDIMenu(CWnd::FromHandle(pItem->GetHandle()));
		}
	}

	return 0;
}

LRESULT CXTPTearOffFrame::OnSetActiveWorkspace(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CXTPTabManagerItem* pItem = m_wndTabClient.GetSelectedItem();
	if (pItem)
	{
		CXTPMDIFrameWndEx* pMDIFrameWndEx = DYNAMIC_DOWNCAST(CXTPMDIFrameWndEx, AfxGetMainWnd());
		if (pMDIFrameWndEx)
		{
			return pMDIFrameWndEx->UpdateMDIMenu(CWnd::FromHandle(pItem->GetHandle()));
		}
	}

	return 0;
}