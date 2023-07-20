// XTPFrameWnd.cpp : implementation for the CXTPFrameWnd and CXTPMDIFrameWnd classes.
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
#include "Common/XTPIntel80Helpers.h"
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
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/XTPFrameWnd.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/TabClient/Workspace.h"
#include "CommandBars/TabClient/SingleWorkspace.h"
#include "CommandBars/TabClient/TabClientDropTarget.h"
#include "CommandBars/TabClient/NavigateButtonActiveFiles.h"
#include "CommandBars/TabClient/XTPTabClientDraggingContents.h"
#include "CommandBars/TabClient/XTPTabClientSticker.h"
#include "CommandBars/TabClient/XTPTearOffFrame.h"
#include "CommandBars/TabClient/XTPTabClientContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CXTPFrameWnd

IMPLEMENT_DYNCREATE(CXTPFrameWnd, CFrameWnd)

CXTPFrameWnd::CXTPFrameWnd()
{
}

// CXTPMDIFrameWnd

IMPLEMENT_DYNCREATE(CXTPMDIFrameWnd, CMDIFrameWnd)

CXTPMDIFrameWnd::CXTPMDIFrameWnd()
{
}

extern const UINT XTP_WM_UPDATE_MDI_CHILD_THEME = RegisterWindowMessage(
	_T("XTP_WM_UPDATE_MDI_CHILD_THEME"));

void CXTPMDIFrameWnd::UpdateMDIChildrenTheme()
{
	if (NULL != GetSafeHwnd())
	{
		CWnd* pWnd = GetActiveFrame();
		if (pWnd != this)
		{
			while (NULL != pWnd)
			{
				pWnd->SendMessage(XTP_WM_UPDATE_MDI_CHILD_THEME);
				pWnd = pWnd->GetNextWindow();
			}
		}
	}
}

BOOL CXTPMDIFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID >= XTP_ID_WORKSPACE_MOVEPREVIOUS && nID <= XTP_ID_WORKSPACE_NEWVERTICAL
		|| nID >= XTP_ID_WORKSPACE_DETACH && nID <= XTP_ID_WORKSPACE_ATTACH_ALL)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(m_hWndMDIClient);
		if (pWnd && pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// then pump through normal frame
	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// CXTPMDIFrameWndEx

IMPLEMENT_DYNAMIC(CXTPMDIFrameWndEx, CXTPMDIFrameWnd)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPMDIFrameWndEx, CXTPMDIFrameWnd)
	//{{AFX_MSG_MAP(CXTPMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_MESSAGE(WM_XTP_TEAROFF_ACTIVATE, OnTearoffActivate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

	/////////////////////////////////////////////////////////////////////////////
	// CXTPMDIFrameWndEx diagnostics

#ifdef _DEBUG
void CXTPMDIFrameWndEx::AssertValid() const
{
	CXTPMDIFrameWnd::AssertValid();
}

void CXTPMDIFrameWndEx::Dump(CDumpContext& dc) const
{
	CXTPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

CXTPMDIFrameWndEx::CXTPMDIFrameWndEx()
	: m_pPrevActiveFrame(NULL)
	, m_pLastActiveFrame(NULL)
	, m_nIDResource(0)
{
}

CXTPMDIFrameWndEx::~CXTPMDIFrameWndEx()
{
}

int CXTPMDIFrameWndEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	XTP_SAFE_CALL1(GetTabClientWnd(), SetDockingPaneManager(GetDockingPaneManager()));
#	endif
#endif

	XTP_SAFE_CALL1(GetTabClientWnd(), EnableTearOff());

	return 0;
}

void CXTPMDIFrameWndEx::OnDestroy()
{
	CXTPMDIFrameWnd::OnDestroy();
}

BOOL CXTPMDIFrameWndEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CArray<CXTPTabClientWnd*, CXTPTabClientWnd*> tabClients;
	XTP_SAFE_CALL1(GetTabClientWnd(), GetTabClients(tabClients));

	for (int i = 0; i < tabClients.GetSize(); i++)
	{
		for (int j = 0; j < tabClients.GetAt(i)->GetItemCount(); j++)
		{
			CXTPTabManagerItem* pItem = tabClients.GetAt(i)->GetItem(j);
			if (pItem->GetHandle() == reinterpret_cast<HANDLE>(wParam))
			{
				pItem->Select();
				tabClients.GetAt(i)->GetParent()->SetForegroundWindow();
				return TRUE;
			}
		}
	}

	if (m_pLastActiveFrame && ::IsWindow(m_pLastActiveFrame->m_hWnd))
	{
		CMDIChildWnd* pActiveChild = m_pLastActiveFrame->MDIGetActive();
		if (pActiveChild != NULL
			&& AfxCallWndProc(pActiveChild, pActiveChild->m_hWnd, WM_COMMAND, wParam, lParam) != 0)
			return TRUE; // handled by child
	}

	return CXTPMDIFrameWnd::OnCommand(wParam, lParam);
}

BOOL CXTPMDIFrameWndEx::OnCmdMsg(UINT nID, int nCode, void* pExtra,
								 AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_pLastActiveFrame)
		return m_pLastActiveFrame->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	return CXTPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CXTPMDIFrameWndEx::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		m_pPrevActiveFrame = m_pLastActiveFrame;
		m_pLastActiveFrame = NULL;

		OnUpdateFrameMenu(NULL);
	}

	CXTPMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

int CXTPMDIFrameWndEx::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);

	if (m_pLastActiveFrame && nHitTest == HTCLIENT && message == WM_LBUTTONDOWN)
	{
		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
		{
			for (int i = 0; i < pCommandBars->GetCount(); i++)
			{
				CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
				if (pToolBar)
				{
					CXTPWindowRect rc(pToolBar);
					if (rc.PtInRect(pt))
					{
						return MA_NOACTIVATE;
					}
				}
			}
		}
	}

	return CXTPMDIFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CXTPMDIFrameWndEx::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	UNREFERENCED_PARAMETER(dwThreadID);

	if (bActive)
		XTP_SAFE_CALL1(GetTabClientWnd(), SetForegroundTearOffFrames());
}

LRESULT CXTPMDIFrameWndEx::OnTearoffActivate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	CXTPTearOffFrame* pTearOffFrame = (CXTPTearOffFrame*)wParam;
	if (pTearOffFrame)
	{
		m_pPrevActiveFrame = m_pLastActiveFrame;
		m_pLastActiveFrame = pTearOffFrame;

		return UpdateMDIMenu(pTearOffFrame->MDIGetActive());
	}

	return 0;
}

LRESULT CXTPMDIFrameWndEx::UpdateMDIMenu(CWnd* pWnd)
{
	CMDIChildWnd* pMDIChildWnd = DYNAMIC_DOWNCAST(CMDIChildWnd, pWnd);
	if (!pMDIChildWnd)
		return 0;

	CDocument* pDocument = pMDIChildWnd->GetActiveDocument();
	if (!pDocument)
		return 0;

	CDocTemplate* pDocTemplate = pDocument->GetDocTemplate();
	if (!pDocTemplate)
		return 0;

	CMultiDocTemplate* pMultiDocTemplate = DYNAMIC_DOWNCAST(CMultiDocTemplate, pDocTemplate);
	if (!pMultiDocTemplate)
		return 0;

	_XTP_USES_PROTECTED_ACCESS_ARGS(CXTPMDIFrameWndEx, CDocTemplate,
									(UINT nIDResource, CRuntimeClass * pDocClass,
									 CRuntimeClass * pFrameClass, CRuntimeClass * pViewClass),
									(nIDResource, pDocClass, pFrameClass, pViewClass), UINT,
									m_nIDResource);
	m_nIDResource = _XTP_PROTECTED_ACCESS(CDocTemplate, pDocTemplate, m_nIDResource);

	return ::SendMessage(m_hWndMDIClient, WM_MDISETMENU, (WPARAM)pMultiDocTemplate->m_hMenuShared,
						 NULL);
}

BOOL CXTPMDIFrameWndEx::PreTranslateMessage(MSG* pMsg)
{
	if (m_pLastActiveFrame && ::IsWindow(m_pLastActiveFrame->m_hWnd))
	{
		if (m_pLastActiveFrame->PreTranslateMessage(pMsg))
			return TRUE;

		if (m_hAccelTable != NULL && ::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;

		return FALSE;
	}

	return CXTPMDIFrameWnd::PreTranslateMessage(pMsg);
}