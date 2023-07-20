// XTPTabClientContext.cpp : implementation of the CXTPTabClientContext class.
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
#include "Common/XTPImageManager.h"
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

#include "TabManager/Includes.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2010.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2012.h"
#include "TabManager/Themes/XTPTabThemeOffice2013.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2015.h"
#include "TabManager/Themes/XTPTabThemeNativeWindows10.h"

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
#		include "DockingPane/Includes.h"
#		include "DockingPane/XTPDockingPaneManager.h"
#	endif
#endif

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

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

#ifndef LWA_ALPHA
#	define LWA_ALPHA 0x00000002
#endif

#ifndef WS_EX_LAYERED
#	define WS_EX_LAYERED 0x00080000
#endif

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//*******************************************************************
//
//		CXTPAlphaWnd implementation
//
//*******************************************************************

CXTPAlphaWnd::CXTPAlphaWnd()
{
}

CXTPAlphaWnd::~CXTPAlphaWnd()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPAlphaWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPAlphaWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPAlphaWnd message handlers

BOOL CXTPAlphaWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPAlphaWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
}

//////////////////////////////////////////////////////////////////////////
// CXTPTabClientContext

CXTPTabClientContext::CXTPTabClientContext()
{
	m_bAlphaTabClientContext		= FALSE;
	m_bShowTabClientContextStickers = FALSE;
	m_bShowContentsWhileDragging	= FALSE;
	m_bThemedFloatingFrames			= FALSE;

	m_pTabClientWnd = NULL;

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	m_pDockSticker   = new CXTPDockPaneContextSticker();
	m_pDockAlphaWnd  = new CXTPDockPaneAlphaWnd();
	m_pDockFocusRect = new CXTPDockFocusRect();
#	else
	m_pDockSticker   = NULL;
	m_pDockAlphaWnd  = NULL;
	m_pDockFocusRect = NULL;
#	endif
#endif
}

CXTPTabClientContext::~CXTPTabClientContext()
{
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	SAFE_DELETE(m_pDockSticker);
	SAFE_DELETE(m_pDockAlphaWnd);
	SAFE_DELETE(m_pDockFocusRect);
#	else
	m_pDockSticker   = NULL;
	m_pDockAlphaWnd  = NULL;
	m_pDockFocusRect = NULL;
#	endif
#endif
}

void CXTPTabClientContext::SetTabClient(CXTPTabClientWnd* pTabClientWnd)
{
	m_pTabClientWnd = pTabClientWnd;

	m_tabSticker.InitContext(this);
	m_tabAlphaWnd.InitContext(this);
	m_tabFocusRect.InitContext(this);
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	m_pDockSticker->InitContext(this);
	m_pDockAlphaWnd->InitContext(this);
	m_pDockFocusRect->InitContext(this);
#	endif
#endif
	m_draggingContents.InitContext(this);
}

CXTPTabClientWnd* CXTPTabClientContext::GetTabClient() const
{
	return m_pTabClientWnd;
}

CXTPTabClientWnd* CXTPTabClientContext::FindTargetTabClient(POINT pt) const
{
	return XTP_SAFE_GET1(GetTabClient(), FindTargetTabClient(pt), NULL);
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
CXTPDockingPaneTabbedContainer* CXTPTabClientContext::FindTargetDockPane(POINT pt) const
{
	return XTP_SAFE_GET1(GetTabClient(), FindTargetDockPane(pt), NULL);
}
#	endif
#endif

//*******************************************************************
//
//		CXTPTabSticker implementation
//
//*******************************************************************

CXTPTabClientContext::CXTPTabClientContextSticker::CXTPTabClientContextSticker()
{
	m_pContext			  = NULL;
	m_pLastTarget		  = NULL;
	m_sticker			  = NULL;
	m_lastWorkspace		  = -1;
	m_lastSelectedSticker = xtpTabClientStickerNone;

	m_hCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorNew = XTPResourceManager()->LoadCursor(XTP_IDC_WORKSPACE_NEW);
	ASSERT(NULL != m_hCursorNew);
}

CXTPTabClientContext::CXTPTabClientContextSticker::~CXTPTabClientContextSticker()
{
}

void CXTPTabClientContext::CXTPTabClientContextSticker::InitContext(CXTPTabClientContext* pContext)
{
	m_pContext = pContext;
}

BOOL CXTPTabClientContext::CXTPTabClientContextSticker::Enable()
{
	ASSERT(m_pContext->GetTabClient());

	POINT pt;
	::GetCursorPos(&pt);

	CXTPTabClientWnd* pTarget = m_pContext->FindTargetTabClient(pt);

	if (pTarget)
	{
		CXTPTabClientWnd* pSource		= XTP_SAFE_GET1(m_pContext, GetTabClient(), NULL);
		CXTPTearOffFrame* pTearOffFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame,
														   XTP_SAFE_GET1(pSource, GetParentFrame(),
																		 NULL));
		if (pTearOffFrame && pTearOffFrame->IsFrameBeingDragged() && pSource == pTarget)
			return FALSE;
	}

	int nWorkspace = -1;
	if (pTarget)
	{
		pTarget->ScreenToWorkspace(&pt);

		for (int i = 0; i < pTarget->GetWorkspaceCount(); i++)
			if (pTarget->GetWorkspace(i)->GetControlRect().PtInRect(pt))
				nWorkspace = i;
	}

	BOOL bAdd = FALSE, bRemove = FALSE;

	if (pTarget && !m_pLastTarget)
	{
		bAdd = TRUE;
	}
	else if (pTarget && m_pLastTarget)
	{
		if (pTarget == m_pLastTarget)
		{
			if (m_lastWorkspace != nWorkspace)
			{
				bRemove = TRUE;
				bAdd	= TRUE;
			}
		}
		else
		{
			bRemove = TRUE;
			bAdd	= TRUE;
		}
	}
	else if (!pTarget && m_pLastTarget)
	{
		bRemove = TRUE;
		bAdd	= FALSE;
	}
	else if (!pTarget && !m_pLastTarget)
	{
		bRemove = FALSE;
		bAdd	= FALSE;
	}

	m_pLastTarget   = pTarget;
	m_lastWorkspace = nWorkspace;

	if (m_sticker && bRemove)
	{
		Disable();
	}
	if (!m_sticker && bAdd)
	{
		m_sticker = new CXTPTabClientSticker(pTarget, nWorkspace);
		m_sticker->Create();
	}

	return TRUE;
}

BOOL CXTPTabClientContext::CXTPTabClientContextSticker::Update()
{
	Enable();

	POINT pt;
	::GetCursorPos(&pt);

	XTPTabClientStickerPart selectedSticker = XTP_SAFE_GET1(m_sticker, HitTest(pt),
															xtpTabClientStickerNone);
	if (selectedSticker != m_lastSelectedSticker)
	{
		SetCursor(selectedSticker == xtpTabClientStickerNone ? m_hCursor : m_hCursorNew);

		XTP_SAFE_CALL1(m_sticker, RedrawWindow());
		m_lastSelectedSticker = selectedSticker;
	}

	return TRUE;
}

void CXTPTabClientContext::CXTPTabClientContextSticker::Disable()
{
	XTP_SAFE_CALL1(m_sticker, DestroyWindow());
	SAFE_DELETE(m_sticker);

	m_pLastTarget   = NULL;
	m_lastWorkspace = -1;
}

int CXTPTabClientContext::CXTPTabClientContextSticker::GetLastWorkspace() const
{
	return m_lastWorkspace;
}

CXTPTabClientWnd* CXTPTabClientContext::CXTPTabClientContextSticker::GetLastTarget() const
{
	return m_pLastTarget;
}

XTPTabClientStickerPart
	CXTPTabClientContext::CXTPTabClientContextSticker::GetLastSelectedSticker() const
{
	return m_lastSelectedSticker;
}

CXTPTabClientSticker* CXTPTabClientContext::CXTPTabClientContextSticker::GetSticker() const
{
	return m_sticker;
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
//*******************************************************************
//
//		CXTPDockPaneContextSticker implementation
//
//*******************************************************************

CXTPTabClientContext::CXTPDockPaneContextSticker::CXTPDockPaneContextSticker()
{
	m_pContext			  = NULL;
	m_pLastTarget		  = NULL;
	m_sticker			  = NULL;
	m_lastSelectedSticker = xtpPaneStickerNone;

	m_hCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorNew = XTPResourceManager()->LoadCursor(XTP_IDC_WORKSPACE_NEW);
	ASSERT(NULL != m_hCursorNew);
}

CXTPTabClientContext::CXTPDockPaneContextSticker::~CXTPDockPaneContextSticker()
{
}

void CXTPTabClientContext::CXTPDockPaneContextSticker::InitContext(CXTPTabClientContext* pContext)
{
	m_pContext = pContext;
}

BOOL CXTPTabClientContext::CXTPDockPaneContextSticker::Enable()
{
	ASSERT(m_pContext->GetTabClient());

	POINT pt;
	::GetCursorPos(&pt);

	CXTPDockingPaneTabbedContainer* pTarget = m_pContext->FindTargetDockPane(pt);

	BOOL bAdd = FALSE, bRemove = FALSE;
	if (pTarget && !m_pLastTarget)
	{
		bAdd = TRUE;
	}
	else if (pTarget && m_pLastTarget)
	{
		if (pTarget == m_pLastTarget)
		{
			bRemove = FALSE;
			bAdd	= FALSE;
		}
		else
		{
			bRemove = TRUE;
			bAdd	= TRUE;
		}
	}
	else if (!pTarget && m_pLastTarget)
	{
		bRemove = TRUE;
		bAdd	= FALSE;
	}
	else if (!pTarget && !m_pLastTarget)
	{
		bRemove = FALSE;
		bAdd	= FALSE;
	}

	m_pLastTarget = pTarget;

	if (m_sticker && bRemove)
	{
		Disable();
	}
	if (!m_sticker && bAdd)
	{
		m_sticker = new CXTPDockPaneSticker(pTarget);
		m_sticker->Create();
	}

	return TRUE;
}

BOOL CXTPTabClientContext::CXTPDockPaneContextSticker::Update()
{
	Enable();

	POINT pt;
	::GetCursorPos(&pt);

	XTPDockingPaneStickerType selectedSticker = XTP_SAFE_GET1(m_sticker, HitTest(pt),
															  xtpPaneStickerNone);
	if (selectedSticker != m_lastSelectedSticker)
	{
		SetCursor(selectedSticker == xtpPaneStickerNone ? m_hCursor : m_hCursorNew);

		XTP_SAFE_CALL1(m_sticker, RedrawWindow());
		m_lastSelectedSticker = selectedSticker;
	}

	return TRUE;
}

void CXTPTabClientContext::CXTPDockPaneContextSticker::Disable()
{
	XTP_SAFE_CALL1(m_sticker, DestroyWindow());
	SAFE_DELETE(m_sticker);

	m_pLastTarget = NULL;
}

CXTPDockingPaneTabbedContainer*
	CXTPTabClientContext::CXTPDockPaneContextSticker::GetLastTarget() const
{
	return m_pLastTarget;
}

XTPDockingPaneStickerType
	CXTPTabClientContext::CXTPDockPaneContextSticker::GetLastSelectedSticker() const
{
	return m_lastSelectedSticker;
}
#	endif
#endif

//*******************************************************************
//
//		CXTPAlphaWndBase implementation
//
//*******************************************************************

CXTPTabClientContext::CXTPAlphaWndBase::CXTPAlphaWndBase()
{
	m_pContext						= NULL;
	m_pfnSetLayeredWindowAttributes = NULL;
	HMODULE hLib					= GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		m_pfnSetLayeredWindowAttributes = (PVOID)::GetProcAddress(hLib,
																  "SetLayeredWindowAttributes");
	}
}

CXTPTabClientContext::CXTPAlphaWndBase::~CXTPAlphaWndBase()
{
}

void CXTPTabClientContext::CXTPAlphaWndBase::InitContext(CXTPTabClientContext* pContext)
{
	m_pContext = pContext;
}

BOOL CXTPTabClientContext::CXTPAlphaWndBase::Enable()
{
	if (!m_pfnSetLayeredWindowAttributes)
		return FALSE;

	if (m_wndAlpha.GetSafeHwnd())
		return TRUE;

	BOOL res = m_wndAlpha.CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
								   AfxRegisterWndClass(NULL,
													   AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
								   0, WS_POPUP, CRect(0, 0, 0, 0), NULL, 0);

	((PFNSETLAYEREDWINDOWATTRIBUTES)m_pfnSetLayeredWindowAttributes)(m_wndAlpha.m_hWnd, 0, 100,
																	 LWA_ALPHA);

	return res;
}

void CXTPTabClientContext::CXTPAlphaWndBase::Disable()
{
	if (!m_pfnSetLayeredWindowAttributes)
		return;

	m_wndAlpha.DestroyWindow();
}

//*******************************************************************
//
//		CXTPAlphaTabClientWnd implementation
//
//*******************************************************************

BOOL CXTPTabClientContext::CXTPTabClientAlphaWnd::Update()
{
	if (!m_pfnSetLayeredWindowAttributes)
		return FALSE;

	XTPTabClientStickerPart type = m_pContext->GetLastSelectedTabClientSticker();
	if (type == xtpTabClientStickerNone)
	{
		Disable();
	}
	else
	{
		CXTPTabClientWnd* pTarget = m_pContext->GetLastTargetTabClient();
		if (!pTarget)
			return FALSE;

		VERIFY(Enable());

		CRect rc;
		if (pTarget->GetWorkspaceCount() == 0)
			pTarget->GetClientRect(&rc);
		else
			rc = pTarget->GetWorkspace(m_pContext->GetLastWorkspace())->GetControlRect();

		pTarget->ClientToScreen(&rc);

		switch (type)
		{
			case xtpTabClientStickerLeft: rc.DeflateRect(0, 0, rc.Width() / 2, 0); break;
			case xtpTabClientStickerRight: rc.DeflateRect(rc.Width() / 2, 0, 0, 0); break;
			case xtpTabClientStickerTop: rc.DeflateRect(0, 0, 0, rc.Height() / 2); break;
			case xtpTabClientStickerBottom: rc.DeflateRect(0, rc.Height() / 2, 0, 0); break;
			case xtpTabClientStickerCenter: break;
			case xtpTabClientStickerNone:
			default: break;
		}

		m_wndAlpha.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(),
								SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}

	return TRUE;
}
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
//*******************************************************************
//
//		CXTPAlphaDockPaneWnd implementation
//
//*******************************************************************

BOOL CXTPTabClientContext::CXTPDockPaneAlphaWnd::Update()
{
	if (!m_pfnSetLayeredWindowAttributes)
		return FALSE;

	XTPDockingPaneStickerType type = m_pContext->GetLastSelectedDockSticker();
	if (type == xtpPaneStickerNone)
	{
		Disable();
	}
	else
	{
		CXTPDockingPaneTabbedContainer* pTarget = m_pContext->GetLastTargetDockPane();
		if (!pTarget)
			return FALSE;

		VERIFY(Enable());

		CRect rc;
		pTarget->GetWindowRect(&rc);

		switch (type)
		{
			case xtpPaneStickerLeft: rc.DeflateRect(0, 0, rc.Width() / 2, 0); break;
			case xtpPaneStickerRight: rc.DeflateRect(rc.Width() / 2, 0, 0, 0); break;
			case xtpPaneStickerTop: rc.DeflateRect(0, 0, 0, rc.Height() / 2); break;
			case xtpPaneStickerBottom: rc.DeflateRect(0, rc.Height() / 2, 0, 0); break;
			case xtpPaneStickerCenter: break;
			case xtpPaneStickerNone:
			default: break;
		}

		m_wndAlpha.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(),
								SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}

	return TRUE;
}
#	endif
#endif

//*******************************************************************
//
//		CXTPDraggingContents implementation
//
//*******************************************************************

CXTPTabClientContext::CXTPDraggingContents::CXTPDraggingContents()
{
	m_pContext = NULL;
	m_pWndDrag = NULL;
}

CXTPTabClientContext::CXTPDraggingContents::~CXTPDraggingContents()
{
}

void CXTPTabClientContext::CXTPDraggingContents::InitContext(CXTPTabClientContext* pContext)
{
	m_pContext = pContext;
}

BOOL CXTPTabClientContext::CXTPDraggingContents::Enable()
{
	BOOL res = FALSE;

	if (m_pWndDrag)
	{
		res = TRUE;
	}
	else
	{
		m_pWndDrag = new CXTPTabClientDraggingContents(m_pContext->GetTabClient());
		res		   = m_pWndDrag->Create();
	}

	return res;
}

void CXTPTabClientContext::CXTPDraggingContents::Update()
{
	if (m_pWndDrag)
	{
		POINT pt;
		::GetCursorPos(&pt);
		m_pWndDrag->MoveWindow(pt.x, pt.y, m_pWndDrag->GetWidth(), m_pWndDrag->GetHeight());
	}
}

void CXTPTabClientContext::CXTPDraggingContents::Disable()
{
	XTP_SAFE_CALL1(m_pWndDrag, DestroyWindow());
	SAFE_DELETE(m_pWndDrag);
}

//*******************************************************************
//
//		CXTPFocusRectBase implementation
//
//*******************************************************************

CXTPTabClientContext::CXTPFocusRectBase::CXTPFocusRectBase()
{
	m_pContext  = NULL;
	m_pDC		= NULL;
	m_focusRect = FALSE;
}

CXTPTabClientContext::CXTPFocusRectBase::~CXTPFocusRectBase()
{
}

void CXTPTabClientContext::CXTPFocusRectBase::InitContext(CXTPTabClientContext* pContext)
{
	m_pContext = pContext;
}

void CXTPTabClientContext::CXTPFocusRectBase::Disable()
{
	if (!m_pContext)
		return;

	if (m_focusRect)
	{
		CancelLoop();
		m_focusRect = FALSE;
	}
}

void CXTPTabClientContext::CXTPFocusRectBase::InitLoop(CWnd* pWnd)
{
	if (CXTPDrawHelpers::ProcessPendingPaintMessages())
	{
		// initialize state
		m_rectLast.SetRectEmpty();
		m_sizeLast.cx = m_sizeLast.cy = 0;
		m_rcGroup.SetRectEmpty();

		// lock window update while dragging
		ASSERT(m_pDC == NULL);
		if (pWnd->LockWindowUpdate())
			m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		else
			m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE);
		ASSERT(m_pDC != NULL);
	}
}

void CXTPTabClientContext::CXTPFocusRectBase::CancelLoop()
{
	DrawFocusRect(TRUE); // gets rid of focus rect

	CWnd* pWnd = CWnd::GetDesktopWindow();
	pWnd->UnlockWindowUpdate();
	if (m_pDC != NULL)
	{
		pWnd->ReleaseDC(m_pDC);
		m_pDC = NULL;
	}
}

void CXTPTabClientContext::CXTPFocusRectBase::DrawFocusRect(BOOL bRemoveRect)
{
	// ASSERT(m_pDC != NULL);
	if (!m_pDC)
		return;

	// determine new rect and size
	CBrush* pDitherBrush = CDC::GetHalftoneBrush();
	CRect rect			 = m_rcGroup;

	CSize size(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));

	if (bRemoveRect)
		size.cx = size.cy = 0;

	// draw it and remember last size
	m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast, pDitherBrush, pDitherBrush);

	m_rectLast = rect;
	m_sizeLast = size;
}

//*******************************************************************
//
//		CXTPTabFocusRect implementation
//
//*******************************************************************

void CXTPTabClientContext::CXTPTabFocusRect::Enable()
{
	if (!XTP_SAFE_GET1(m_pContext, GetTabClient(), NULL))
		return;

	if (!m_focusRect)
	{
		POINT pt;
		::GetCursorPos(&pt);

		CXTPTabClientWnd* pTarget = m_pContext->FindTargetTabClient(pt);
		if (!pTarget)
			return;

		InitLoop(pTarget);
		m_focusRect = TRUE;
	}
}

void CXTPTabClientContext::CXTPTabFocusRect::Update(CRect rcGroup)
{
	if (!XTP_SAFE_GET1(m_pContext, GetTabClient(), NULL))
		return;

	if (m_focusRect)
	{
		POINT pt;
		::GetCursorPos(&pt);

		CXTPTabClientWnd* pTarget = m_pContext->FindTargetTabClient(pt);
		if (!pTarget)
		{
			Disable();
			return;
		}

		if (pTarget != m_pDC->GetWindow())
		{
			Disable();
			Enable();
		}

		if (m_rcGroup != rcGroup)
		{
			m_rcGroup = rcGroup;
			DrawFocusRect();
		}

		if (rcGroup.IsRectNull())
		{
			DrawFocusRect(TRUE);
		}
	}
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
//*******************************************************************
//
//		CXTPDockFocusRect implementation
//
//*******************************************************************

void CXTPTabClientContext::CXTPDockFocusRect::Enable()
{
	if (!m_focusRect)
	{
		POINT pt;
		::GetCursorPos(&pt);

		CXTPDockingPaneTabbedContainer* pTarget = m_pContext->FindTargetDockPane(pt);
		if (!pTarget)
			return;

		InitLoop(pTarget);
		m_focusRect = TRUE;
	}
}

void CXTPTabClientContext::CXTPDockFocusRect::Update(CRect rcGroup)
{
	if (m_focusRect)
	{
		POINT pt;
		::GetCursorPos(&pt);

		CXTPDockingPaneTabbedContainer* pTarget = m_pContext->FindTargetDockPane(pt);
		if (!pTarget)
		{
			Disable();
			return;
		}

		if (pTarget != m_pDC->GetWindow())
		{
			Disable();
			Enable();
		}

		if (m_rcGroup != rcGroup)
		{
			m_rcGroup = rcGroup;
			DrawFocusRect();
		}

		if (rcGroup.IsRectNull())
		{
			DrawFocusRect(TRUE);
		}
	}
}
#	endif
#endif

//*******************************************************************
//
//		CXTPTabClientContext implementation
//
//*******************************************************************

BOOL CXTPTabClientContext::EnableTabSticker()
{
	return m_bShowTabClientContextStickers ? m_tabSticker.Enable() : FALSE;
}

BOOL CXTPTabClientContext::UpdateTabSticker()
{
	return m_bShowTabClientContextStickers ? m_tabSticker.Update() : FALSE;
}

void CXTPTabClientContext::DisableTabSticker()
{
	if (m_bShowTabClientContextStickers)
		m_tabSticker.Disable();
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
BOOL CXTPTabClientContext::EnableDockSticker()
{
	return m_bShowTabClientContextStickers ? m_pDockSticker->Enable() : FALSE;
}

BOOL CXTPTabClientContext::UpdateDockSticker()
{
	return m_bShowTabClientContextStickers ? m_pDockSticker->Update() : FALSE;
}

void CXTPTabClientContext::DisableDockSticker()
{
	if (m_bShowTabClientContextStickers)
		m_pDockSticker->Disable();
}
#	endif
#endif

BOOL CXTPTabClientContext::EnableTabContextWindow()
{
	return m_bAlphaTabClientContext ? m_tabAlphaWnd.Enable() : FALSE;
}

BOOL CXTPTabClientContext::UpdateTabContextWindow()
{
	return m_bAlphaTabClientContext ? m_tabAlphaWnd.Update() : FALSE;
}

void CXTPTabClientContext::DisableTabContextWindow()
{
	if (m_bAlphaTabClientContext)
		m_tabAlphaWnd.Disable();
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
BOOL CXTPTabClientContext::EnableDockContextWindow()
{
	return m_bAlphaTabClientContext ? m_pDockAlphaWnd->Enable() : FALSE;
}

BOOL CXTPTabClientContext::UpdateDockContextWindow()
{
	return m_bAlphaTabClientContext ? m_pDockAlphaWnd->Update() : FALSE;
}

void CXTPTabClientContext::DisableDockContextWindow()
{
	if (m_bAlphaTabClientContext)
		m_pDockAlphaWnd->Disable();
}
#	endif
#endif

void CXTPTabClientContext::EnableDraggingContents()
{
	if (m_bShowContentsWhileDragging)
		m_draggingContents.Enable();
}

void CXTPTabClientContext::DisableDraggingContents()
{
	if (m_bShowContentsWhileDragging)
		m_draggingContents.Disable();
}

void CXTPTabClientContext::UpdateDraggingContents()
{
	if (m_bShowContentsWhileDragging)
		m_draggingContents.Update();
}

void CXTPTabClientContext::EnableTabFocusRect()
{
	if (m_bAlphaTabClientContext || m_bShowTabClientContextStickers)
		return;

	m_tabFocusRect.Enable();
}

void CXTPTabClientContext::DisableTabFocusRect()
{
	if (m_bAlphaTabClientContext || m_bShowTabClientContextStickers)
		return;

	m_tabFocusRect.Disable();
}

void CXTPTabClientContext::UpdateTabFocusRect(CRect rcGroup)
{
	if (m_bAlphaTabClientContext || m_bShowTabClientContextStickers)
		return;

	m_tabFocusRect.Update(rcGroup);
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
void CXTPTabClientContext::EnableDockFocusRect()
{
	if (m_bAlphaTabClientContext || m_bShowTabClientContextStickers)
		return;

	m_pDockFocusRect->Enable();
}

void CXTPTabClientContext::DisableDockFocusRect()
{
	if (m_bAlphaTabClientContext || m_bShowTabClientContextStickers)
		return;

	m_pDockFocusRect->Disable();
}

void CXTPTabClientContext::UpdateDockFocusRect(CRect rcGroup)
{
	if (m_bAlphaTabClientContext || m_bShowTabClientContextStickers)
		return;

	m_pDockFocusRect->Update(rcGroup);
}
#	endif
#endif
