// XTPTabClientWnd.cpp : implementation of the CXTPTabClientWnd class.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "CommandBars/Resource.h"
#include "Common/Resource.h"

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/XTPMaskEditT.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#	include "Controls/Edit/XTPEdit.h"
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

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
#		include "DockingPane/Includes.h"
#		include "DockingPane/XTPDockingPaneManager.h"
#	endif
#endif

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlExt.h"
#include "CommandBars/XTPMouseManager.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPFrameWnd.h"
#include "CommandBars/XTPCommandBarsIIDs.h"
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/TabClient/Workspace.h"
#include "CommandBars/TabClient/SingleWorkspace.h"
#include "CommandBars/TabClient/TabClientDropTarget.h"
#include "CommandBars/TabClient/NavigateButtonActiveFiles.h"
#include "CommandBars/TabClient/XTPTabClientDraggingContents.h"
#include "CommandBars/TabClient/XTPTabClientSticker.h"
#include "CommandBars/TabClient/XTPTabClientContext.h"
#include "CommandBars/TabClient/XTPTearOffFrame.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#pragma warning(disable : 4510)
#pragma warning(disable : 4610)

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef OIC_WINLOGO
#	define OIC_WINLOGO 32517
#endif

#define XTP_TID_REFRESHFRAME 0xACD1

/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWnd

IMPLEMENT_DYNAMIC(CXTPTabClientWnd, CWnd)

CXTPTabClientWnd::CXTPTabClientWnd()
{
	m_pParentFrame  = NULL;
	m_bAllowReorder = TRUE;

	m_pPaintManager					 = new CXTPTabPaintManager();
	m_pPaintManager->m_bBoldSelected = TRUE;

	m_themeCommandBars = xtpThemeNone;
	m_bAutoTheme	   = TRUE;

	m_bLockUpdate	 = FALSE;
	m_bLockReposition = FALSE;
	m_bForceToRecalc  = FALSE;

	m_pActiveWorkspace = NULL;
	m_bHorizSplitting  = TRUE;

	m_bRefreshed = FALSE;

	m_bThemedBackColor		= TRUE;
	m_bUpdateContents		= FALSE;
	m_bRightToLeft			= FALSE;
	m_bAutoUpdateTabCaption = TRUE;
	m_pMarkupContext		= NULL;

	m_bUseSplitterTracker		= TRUE;
	m_bIgnoreFlickersOnActivate = FALSE;

	m_bShowNewWindowTab	= FALSE;
	m_nNewWindowTabCommand = ID_FILE_NEW;

	m_pCommandBars = NULL;

	m_nSplitterSize = 5;

	m_hCursorHoriz  = XTPResourceManager()->LoadCursor(XTP_IDC_HSPLITBAR);
	m_hCursorVert   = XTPResourceManager()->LoadCursor(XTP_IDC_VSPLITBAR);
	m_hCursorNew	= XTPResourceManager()->LoadCursor(XTP_IDC_WORKSPACE_NEW);
	m_hCursorDelete = XTPResourceManager()->LoadCursor(XTP_IDC_WORKSPACE_DELETE);

	m_bShowWorkspace = TRUE;
	m_pTabWorkspace  = NULL;
	m_bEnableGroups  = FALSE;
	m_bUserWorkspace = FALSE;

	m_nMsgUpdateSkinState = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_UPDATESTATE"));
	m_nMsgQuerySkinState  = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_QUERYSTATE"));

	m_dwFlags = 0;

	m_pDropTarget				   = new CTabClientDropTarget();
	m_pDropTarget->m_pTabClientWnd = this;

	m_pToolTipContext = new CXTPToolTipContext;

	m_bDelayLock = FALSE;

	m_newTabPosition		= xtpWorkspaceNewTabRightMost;
	m_afterCloseTabPosition = xtpWorkspaceActivateTopmost;

	m_bEnableTearOff = FALSE;

	m_pTabClientContext = new CXTPTabClientContext();
	m_pTabClientContext->SetTabClient(this);

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	m_paneManager = NULL;
#	endif
#endif

#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
	EnableConnections();
#endif
}

CXTPTabClientWnd::~CXTPTabClientWnd()
{
	SAFE_DELETE(m_pTabClientContext);

	CMDTARGET_RELEASE(m_pPaintManager);

	if (m_pTabWorkspace != NULL)
	{
		m_pTabWorkspace->m_pTabClientWnd = NULL;
		m_pTabWorkspace->DeleteAllItems();
	}
	else
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			delete GetWorkspace(i);
		}
	}

	SAFE_DELETE(m_pDropTarget);

	CMDTARGET_RELEASE(m_pToolTipContext);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	ReleaseConnections();
#endif

	XTPMarkupReleaseContext(m_pMarkupContext);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTabClientWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPTabClientWnd)
	ON_MESSAGE(WM_MDIACTIVATE, OnMDIActivate)
	ON_MESSAGE(WM_MDINEXT, OnMDINext)
	ON_MESSAGE(WM_MDICREATE, OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY, OnMDIDestroy)
	ON_WM_SIZE()
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()

	ON_COMMAND_RANGE(XTP_ID_WORKSPACE_MOVEPREVIOUS, XTP_ID_WORKSPACE_NEWVERTICAL,
					 OnWorkspaceCommand)
	ON_UPDATE_COMMAND_UI_RANGE(XTP_ID_WORKSPACE_MOVEPREVIOUS, XTP_ID_WORKSPACE_NEWVERTICAL,
							   OnUpdateWorkspaceCommand)
	ON_COMMAND_RANGE(XTP_ID_WORKSPACE_DETACH, XTP_ID_WORKSPACE_ATTACH_ALL, OnWorkspaceCommand)
	ON_UPDATE_COMMAND_UI_RANGE(XTP_ID_WORKSPACE_DETACH, XTP_ID_WORKSPACE_ATTACH_ALL,
							   OnUpdateWorkspaceCommand)

	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPTabClientWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
}

void CXTPTabClientWnd::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
}

void CXTPTabClientWnd::EnableMarkup(BOOL bEnableMarkup)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnableMarkup)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd, TRUE);
	}
}

BOOL CXTPTabClientWnd::IsLayoutRTL() const
{
	if (IsAttached() && (GetParentFrame()->GetExStyle() & WS_EX_LAYOUTRTL))
		return TRUE;

	return m_bRightToLeft;
}

#ifndef WS_EX_NOINHERITLAYOUT
#	define WS_EX_NOINHERITLAYOUT 0x00100000L
#endif

void CXTPTabClientWnd::SetLayoutRTL(BOOL bRightToLeft)
{
	if (!XTPSystemVersion()->IsLayoutRTLSupported())
		return;

	if (m_bUserWorkspace)
		return;

	m_bRightToLeft = bRightToLeft;

	if (!m_bEnableGroups && GetWorkspaceCount() == 1)
	{
		((CSingleWorkspace*)GetWorkspace(0))
			->ModifyStyleEx(DWORD(m_bRightToLeft ? 0 : WS_EX_LAYOUTRTL),
							DWORD(m_bRightToLeft ? WS_EX_LAYOUTRTL : 0));
	}

	if (m_bEnableGroups)
	{
		ModifyStyleEx(DWORD(m_bRightToLeft ? 0 : WS_EX_LAYOUTRTL),
					  DWORD(m_bRightToLeft ? WS_EX_LAYOUTRTL | WS_EX_NOINHERITLAYOUT : 0));
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	if (IsAttached())
		m_pParentFrame->DelayRecalcLayout();
}

CXTPTabManagerItem* CXTPTabClientWnd::FindItem(const HWND hWnd) const
{
	// loop through all tab items
	for (int nIndex = 0; nIndex < GetWorkspaceCount(); nIndex++)
	{
		CXTPTabManagerItem* pItem = GetWorkspace(nIndex)->FindItem(hWnd);

		if (pItem)
			return pItem;
	}

	return NULL;
}

BOOL CXTPTabClientWnd::OnBeforeItemClick(CXTPTabManagerItem* pItem)
{
	UNREFERENCED_PARAMETER(pItem);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	FireBeforeItemClick(pItem ? pItem->GetIDispatch(FALSE) : NULL);
#endif

	return TRUE;
}

CWnd* CXTPTabClientWnd::MDIGetActive()
{
#ifdef _XTP_COMMANDBARS_ACTIVEX
	HWND hWnd = (HWND)SendMessage(WM_MDIGETACTIVE);
	return CWnd::FromHandle(hWnd);
#endif
#ifndef _XTP_COMMANDBARS_ACTIVEX
	return (IsAttached() ? GetParentFrame()->MDIGetActive() : NULL);
#endif
}

void CXTPTabClientWnd::MDIActivate(CWnd* pWnd)
{
	if (!pWnd || !::IsWindow(pWnd->GetSafeHwnd()))
		return;

	SendMessage(WM_MDIACTIVATE, (WPARAM)pWnd->m_hWnd, 0);

	if (pWnd->GetStyle() & WS_MINIMIZE)
	{
		pWnd->SendMessage(WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0L);
	}
}

CString CXTPTabClientWnd::GetItemTooltip(const CXTPTabManagerItem* pItem) const
{
	CString strTooltip = pItem->m_strToolTip;
	if (!strTooltip.IsEmpty())
		return strTooltip;

	CMDIChildWnd* pChild = DYNAMIC_DOWNCAST(CMDIChildWnd, CWnd::FromHandle(pItem->GetHandle()));
	if (pChild)
	{
		strTooltip = (LPCTSTR)pChild->SendMessage(WM_XTP_GETWINDOWTOOLTIP, (WPARAM)pItem);
		if (!strTooltip.IsEmpty())
			return strTooltip;

		if (pChild->GetActiveDocument())
			strTooltip = pChild->GetActiveDocument()->GetPathName();

		if (!strTooltip.IsEmpty())
			return strTooltip;
	}
	return pItem->GetCaption();
}

CString CXTPTabClientWnd::GetItemText(const CWnd* pChildWnd) const
{
	ASSERT(pChildWnd != NULL);
	if (!pChildWnd)
		return _T("");

#ifndef _XTP_COMMANDBARS_ACTIVEX
	ASSERT(pChildWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

	CString sWindowText = (LPCTSTR)((CWnd*)pChildWnd)->SendMessage(WM_XTP_GETWINDOWTEXT);

	if (sWindowText.IsEmpty())
	{
		CDocument* pDoc = ((CMDIChildWnd*)pChildWnd)->GetActiveDocument();
		if (pDoc != NULL)
			sWindowText = pDoc->GetTitle();

		if (sWindowText.IsEmpty())
			CXTPDrawHelpers::GetWindowCaption(pChildWnd->GetSafeHwnd(), sWindowText);
	}
#else
	CString sWindowText;
	CXTPDrawHelpers::GetWindowCaption(pChildWnd->GetSafeHwnd(), sWindowText);
#endif

	return sWindowText;
}

COLORREF CXTPTabClientWnd::GetItemColor(const CXTPTabManagerItem* pItem) const
{
#ifndef _XTP_COMMANDBARS_ACTIVEX
	COLORREF clr = (COLORREF)::SendMessage(pItem->GetHandle(), WM_XTP_GETTABCOLOR, 0, 0);

	if (clr != 0)
	{
		return clr;
	}
#else
	pItem;
#endif

	return COLORREF_NULL;
}

HICON CXTPTabClientWnd::GetItemIcon(const CXTPTabManagerItem* pItem) const
{
	HWND hWnd = pItem->GetHandle();
	HICON hIcon;

	hIcon = (HICON)::SendMessage(hWnd, WM_XTP_GETTABICON, 0, 0);

	if (!hIcon)
		hIcon = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);
	if (!hIcon)
		hIcon = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
	if (!hIcon)
		hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(hWnd, GCLP_HICONSM);
	if (!hIcon)
		hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(hWnd, GCLP_HICON);

	return hIcon ? hIcon : AfxGetApp()->LoadOEMIcon(OIC_WINLOGO);
}

CXTPTabClientWnd::CWorkspace* CXTPTabClientWnd::CreateWorkspace()
{
	if (m_pTabWorkspace != NULL)
		return m_pTabWorkspace;

	if (m_bUserWorkspace)
		return NULL;

	if (m_bEnableGroups)
		return new CWorkspace();

	return new CSingleWorkspace();
}

CXTPTabClientWnd::CWorkspace* CXTPTabClientWnd::AddWorkspace(int nIndex)
{
	if (m_pDropTarget)
		m_pDropTarget->Revoke();

	CWorkspace* pWorkspace = CreateWorkspace();
	if (!pWorkspace)
		return NULL;

	if (m_bEnableGroups)
	{
		if (m_pDropTarget)
			m_pDropTarget->Register(this);
	}
	else if (!m_bUserWorkspace)
	{
		VERIFY(((CSingleWorkspace*)pWorkspace)
				   ->Create(AfxRegisterWndClass(CS_DBLCLKS,
												AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
							_T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE,
							CXTPEmptyRect(), m_pParentFrame, 0));
		if (m_pDropTarget)
			m_pDropTarget->Register((CSingleWorkspace*)pWorkspace);

		if (IsLayoutRTL())
		{
			((CSingleWorkspace*)pWorkspace)->ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		}
	}

	if (nIndex == -1)
		nIndex = GetWorkspaceCount();
	m_arrWorkspace.InsertAt(nIndex, pWorkspace);

	pWorkspace->m_pTabClientWnd = this;

	pWorkspace->SetActive(FALSE);
	pWorkspace->SetItemMetrics(CSize(XTP_DPI_X(70), XTP_DPI_Y(18)), 0, CSize(XTP_DPI_X(220), 0));
	pWorkspace->GetNavigateButtons()->InsertAt(2, new CNavigateButtonActiveFiles(pWorkspace, this));

	UpdateFlags(pWorkspace);

	m_bForceToRecalc = TRUE;

	return pWorkspace;
}

BOOL CXTPTabClientWnd::Attach(CXTPMDIFrameWnd* pParentFrame, BOOL bEnableGroups /*= FALSE*/)
{
	return Attach(pParentFrame->GetCommandBars(), pParentFrame, bEnableGroups);
}

BOOL CXTPTabClientWnd::Attach(CXTPCommandBars* pCommandBars, CMDIFrameWnd* pParentFrame,
							  BOOL bEnableGroups)
{
	ASSERT(GetWorkspaceCount() == 0);
	ASSERT(pParentFrame != NULL);
	if (!pParentFrame)
		return FALSE;
	ASSERT(::IsWindow(pParentFrame->GetSafeHwnd()));

	m_bEnableGroups = bEnableGroups;

	m_pCommandBars = NULL;

	// check if already attached
	if (IsAttached())
	{
		TRACE(_T("CXTPTabClientWnd::Attach: window has already been attached. Call Detach() ")
			  _T("function before !\n"));
		return FALSE;
	}

	// make sure the specified window is/derived from CMDIFrameWnd class
	if (!pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		TRACE(_T("CXTPTabClientWnd::Attach: specified frame window is not of CMDIFrameWnd class ")
			  _T("(or derived)!\n"));
		return FALSE;
	}

	// try to sublass MDIClient window
	if (!SubclassWindow(pParentFrame->m_hWndMDIClient))
	{
		TRACE(_T("CXTPTabClientWnd::Attach: failed to subclass MDI Client window\n"));
		return FALSE;
	}

	m_pCommandBars = pCommandBars;

	SendMessage(m_nMsgUpdateSkinState);

	// save the pointer to parent MDIFrame
	m_pParentFrame = pParentFrame;

	CheckCommandBarsTheme();
	GetPaintManager()->RefreshMetrics();

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER);

	// update the size and position of the tab control and MDIClient window
	Reposition();

	if (bEnableGroups)
	{
		::SetClassLong(m_hWnd, GCL_STYLE,
					   XTPToLong(::GetClassLong(m_hWnd, GCL_STYLE) | CS_DBLCLKS));
	}

	// populate tab control with MDIChild windows if any exist at the moment
	UpdateContents();

	return TRUE;
}

void CXTPTabClientWnd::EnableToolTips(XTPTabToolTipBehaviour behaviour /*= xtpTabToolTipAlways*/)
{
	m_pPaintManager->EnableToolTips(behaviour);
}

void CXTPTabClientWnd::ShowWorkspace(BOOL bShow)
{
	if (m_bShowWorkspace == bShow)
		return;

	if (!m_bEnableGroups && !m_bUserWorkspace)
	{
		if (GetWorkspaceCount() == 1 && ((CSingleWorkspace*)GetWorkspace(0))->m_hWnd)
		{
			((CSingleWorkspace*)GetWorkspace(0))->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
		}

		m_bShowWorkspace = bShow;
		UpdateContents();
		Reposition();
	}
}

BOOL CXTPTabClientWnd::Detach()
{
	if (IsTearOffEnabled())
	{
		CXTPTearOffFrame* pParentTearOffFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame,
																 GetParentFrame());
		if (!pParentTearOffFrame)
		{
			CloseTearOffFrame(this);
		}
	}

	// check if any attached
	if (!IsAttached())
	{
		TRACE(_T("CXTPTabClientWnd::Attach: there is nothing to detach! Window hasn't been ")
			  _T("attached!\n"));
		return FALSE;
	}

	if (m_pDropTarget)
		m_pDropTarget->Revoke();

	if (m_pTabWorkspace)
	{
		m_pTabWorkspace->m_pTabClientWnd = NULL;
		m_pTabWorkspace->DeleteAllItems();
	}
	else
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			delete GetWorkspace(i);
		}
	}

	m_arrWorkspace.RemoveAll();

	m_pActiveWorkspace = NULL;

	HWND hWnd = m_hWnd;
	// unsubclass MDIClient window
	UnsubclassWindow();

	// update the size and position of the MDIClient window
	if (::IsWindow(m_pParentFrame->GetSafeHwnd()))
		m_pParentFrame->RecalcLayout();

	m_pParentFrame = NULL;
	m_pCommandBars = NULL;

	::SendMessage(hWnd, m_nMsgUpdateSkinState, 0, 0);

	return TRUE;
}

INT_PTR CXTPTabClientWnd::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (XTPMouseManager()->IsTrackedLock())
		return -1;

	if (!m_bShowWorkspace)
		return -1;

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		if (GetWorkspace(i)->GetControlRect().PtInRect(point))
		{
			return GetWorkspace(i)->PerformToolHitTest(m_hWnd, point, pTI);
		}
	}
	return -1;
}

BOOL CXTPTabClientWnd::NeedsRefresh(XTPPaintTheme themeCommandBars)
{
	// if the theme has changed refresh.
	if (m_themeCommandBars != themeCommandBars)
	{
		return TRUE;
	}

	// if a new INI settings file was loaded refresh.
	if (m_strIniFileName != XTPResourceImages()->m_strIniFileName)
	{
		return TRUE;
	}

	return FALSE;
}

void CXTPTabClientWnd::CheckCommandBarsTheme()
{
	if (!m_bAutoTheme)
		return;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return;

	XTPPaintTheme themeCommandBars = pCommandBars->GetPaintManager()->GetCurrentTheme();
	if (!NeedsRefresh(themeCommandBars))
		return;

	// initialize attributes common to most themes.
	m_pPaintManager->m_bStaticFrame   = FALSE;
	m_pPaintManager->m_clientFrame	= xtpTabFrameBorder;
	m_pPaintManager->m_bOneNoteColors = FALSE;
	m_pPaintManager->m_bHotTracking   = FALSE;
	m_pPaintManager->m_rcButtonMargin.SetRect(0, 0, 0, 0);
	m_pPaintManager->m_bClearTypeTextQuality =
		pCommandBars->GetPaintManager()->m_bClearTypeTextQuality;
	m_pPaintManager->m_bBoldSelected = FALSE;
	m_pPaintManager->m_rcClientMargin.SetRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2),
											  XTP_DPI_Y(2));

	SetShowTabClientContextStickers(FALSE);
	SetAlphaTabClientContext(FALSE);
	SetShowContentsWhileDragging(FALSE);
	EnableFloatingFrameTheme(FALSE);

	// set theme specific attributes.
	switch (themeCommandBars)
	{
			// Office Themes:
			//-----------------------------------

		case xtpThemeOffice2000:
		{
			m_pPaintManager->m_bBoldSelected = TRUE;
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPageSelected);
			m_pPaintManager->SetColor(xtpTabColorDefault);
		}
		break;

		case xtpThemeOfficeXP:
		{
			m_pPaintManager->SetAppearance(xtpTabAppearanceVisualStudio2003);
			m_pPaintManager->m_bStaticFrame = TRUE;
			m_pPaintManager->m_clientFrame  = xtpTabFrameSingleLine;
		}
		break;

		case xtpThemeOffice2003:
		{
			m_pPaintManager->m_bBoldSelected	  = TRUE;
			m_pPaintManager->m_bDisableLunaColors = FALSE;
			m_pPaintManager->m_bOneNoteColors	 = TRUE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
			m_pPaintManager->SetColor(xtpTabColorOffice2003);
		}
		break;

		case xtpThemeOffice2013:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcClientMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(12, 4, 12, 4);

			m_pPaintManager->SetAppearanceSet(new CXTPTabThemeOffice2013());
			m_pPaintManager->SetColor(xtpTabColorOffice2013);

			if (IsGroupingEnabled() || IsTearOffEnabled())
			{
				SetShowTabClientContextStickers(TRUE);
				SetAlphaTabClientContext(TRUE);
				SetShowContentsWhileDragging(TRUE);
				EnableFloatingFrameTheme(TRUE);
			}
		}
		break;

		// Visual Studio Themes:
		//-----------------------------------
		case xtpThemeVisualStudio6:
		{
			m_pPaintManager->m_bBoldSelected = TRUE;
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
			m_pPaintManager->SetColor(xtpTabColorDefault);
		}
		break;

		case xtpThemeVisualStudio2005:
		{
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2005);
			m_pPaintManager->m_bStaticFrame		  = TRUE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
		}
		break;

		case xtpThemeVisualStudio2008:
		{
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2008);
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameBorder;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
		}
		break;

		case xtpThemeVisualStudio2010:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameBorder;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcClientMargin.SetRect(0, XTP_DPI_Y(2), 0, XTP_DPI_Y(2));
			m_pPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(2), 0, XTP_DPI_Y(1));

			CXTPTabThemeVisualStudio2010* pTheme = new CXTPTabThemeVisualStudio2010();

			pTheme->m_rcHeaderMargin.left = XTP_DPI_X(1);
			pTheme->m_rcHeaderMargin.top  = 0;

			m_pPaintManager->SetAppearanceSet(pTheme);
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2010);
		}
		break;

		case xtpThemeVisualStudio2012:
		case xtpThemeVisualStudio2012Light:
		case xtpThemeVisualStudio2012Dark:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcClientMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(0, 0, 0, 0);

			CXTPTabThemeVisualStudio2012* pTheme = new CXTPTabThemeVisualStudio2012();
			m_pPaintManager->SetAppearanceSet(pTheme);
			m_pPaintManager->SetColor((themeCommandBars == xtpThemeVisualStudio2012Dark)
										  ? xtpTabColorVisualStudio2012Dark
										  : xtpTabColorVisualStudio2012);
		}
		break;

		case xtpThemeVisualStudio2015:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcClientMargin.SetRect(0, XTP_DPI_Y(1), 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(1), XTP_DPI_X(1), 0);
			m_pPaintManager->m_rcButtonTextPadding.SetRect(XTP_DPI_X(3), XTP_DPI_Y(2), XTP_DPI_X(3),
														   0);
			m_pPaintManager->m_szTabCloseButton = CSize(XTP_DPI_X(15), XTP_DPI_Y(15));
			m_pPaintManager->SetAppearanceSet(new CXTPTabThemeVisualStudio2015());
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2015);

			if (IsGroupingEnabled() || IsTearOffEnabled())
			{
				SetShowTabClientContextStickers(TRUE);
				SetAlphaTabClientContext(TRUE);
				SetShowContentsWhileDragging(TRUE);
				EnableFloatingFrameTheme(TRUE);
			}
		}
		break;

		case xtpThemeVisualStudio2017:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcClientMargin.SetRect(0, XTP_DPI_Y(1), 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(1), XTP_DPI_X(1), 0);
			m_pPaintManager->m_rcButtonTextPadding.SetRect(XTP_DPI_X(3), XTP_DPI_Y(2), XTP_DPI_X(3),
														   0);
			m_pPaintManager->m_szTabCloseButton = CSize(XTP_DPI_X(15), XTP_DPI_Y(15));
			m_pPaintManager->SetAppearanceSet(new CXTPTabThemeVisualStudio2017());
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2017);

			if (IsGroupingEnabled() || IsTearOffEnabled())
			{
				SetShowTabClientContextStickers(TRUE);
				SetAlphaTabClientContext(TRUE);
				SetShowContentsWhileDragging(TRUE);
				EnableFloatingFrameTheme(TRUE);
			}
		}
		break;

		case xtpThemeVisualStudio2019:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcClientMargin.SetRect(0, XTP_DPI_Y(1), 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(1), XTP_DPI_X(1), 0);
			m_pPaintManager->m_rcButtonTextPadding.SetRect(XTP_DPI_X(3), XTP_DPI_Y(2), XTP_DPI_X(3),
														   0);
			m_pPaintManager->m_szTabCloseButton = CSize(XTP_DPI_X(15), XTP_DPI_Y(15));
			m_pPaintManager->SetAppearanceSet(new CXTPTabThemeVisualStudio2019());
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2019);

			if (IsGroupingEnabled() || IsTearOffEnabled())
			{
				SetShowTabClientContextStickers(TRUE);
				SetAlphaTabClientContext(TRUE);
				SetShowContentsWhileDragging(TRUE);
				EnableFloatingFrameTheme(TRUE);
			}
		}
		break;

		case xtpThemeVisualStudio2022:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcClientMargin.SetRect(0, XTP_DPI_Y(1), 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(1), XTP_DPI_X(1), 0);
			m_pPaintManager->m_rcButtonTextPadding.SetRect(XTP_DPI_X(3), XTP_DPI_Y(2), XTP_DPI_X(3),
														   0);
			m_pPaintManager->m_szTabCloseButton = CSize(XTP_DPI_X(15), XTP_DPI_Y(15));
			m_pPaintManager->m_bBoldSelected	= TRUE;
			m_pPaintManager->SetAppearanceSet(new CXTPTabThemeVisualStudio2022());
			m_pPaintManager->SetColor(xtpTabColorVisualStudio2022);

			if (IsGroupingEnabled() || IsTearOffEnabled())
			{
				SetShowTabClientContextStickers(TRUE);
				SetAlphaTabClientContext(TRUE);
				SetShowContentsWhileDragging(TRUE);
				EnableFloatingFrameTheme(TRUE);
			}
		}
		break;

		case xtpThemeNativeWindows10:
		{
			m_pPaintManager->m_bStaticFrame		  = FALSE;
			m_pPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->m_bShowIcons		  = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcClientMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->m_rcButtonMargin.SetRect(0, 0, 0, 0);
			m_pPaintManager->SetAppearanceSet(new CXTPTabThemeNativeWindows10());
			m_pPaintManager->SetColor(xtpTabColorNativeWindows10);

			if (IsGroupingEnabled() || IsTearOffEnabled())
			{
				SetShowTabClientContextStickers(TRUE);
				SetAlphaTabClientContext(TRUE);
				SetShowContentsWhileDragging(TRUE);
				EnableFloatingFrameTheme(TRUE);
			}
		}
		break;

			// Other Themes:
			//-----------------------------------

		case xtpThemeNativeWinXP:
		{
			m_pPaintManager->m_bBoldSelected = TRUE;
			m_pPaintManager->m_bHotTracking  = TRUE;
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
			m_pPaintManager->SetColor(xtpTabColorWinNative);
		}
		break;

		case xtpThemeResource:
		{
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
			m_pPaintManager->SetColor(xtpTabColorResource);
			m_pPaintManager->m_bDisableLunaColors = FALSE;
			m_pPaintManager->m_bOneNoteColors	 = TRUE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
		}
		break;

		case xtpThemeRibbon:
		{
			m_pPaintManager->m_bDisableLunaColors = FALSE;
			m_pPaintManager->m_bOneNoteColors	 = FALSE;
			m_pPaintManager->m_bHotTracking		  = TRUE;
			m_pPaintManager->m_rcClientMargin.SetRect(XTP_DPI_X(3), XTP_DPI_Y(2), XTP_DPI_X(3),
													  XTP_DPI_Y(4));

			if (pCommandBars->GetMenuBar()
				&& pCommandBars->GetMenuBar()->GetType() == xtpBarTypeRibbon)
			{
				m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPageAccess2007);
				m_pPaintManager->m_rcButtonMargin.SetRect(XTP_DPI_X(3), 0, XTP_DPI_X(3), 0);
			}
			else
			{
				m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2007);
				m_pPaintManager->m_rcButtonMargin.SetRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2),
														  XTP_DPI_Y(2));
			}
		}
		break;

		case xtpThemeNone:
		case xtpThemeOffice2007System:
		case xtpThemeCustom:
		default:
		{
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
			m_pPaintManager->SetColor(xtpTabColorDefault);
		}
		break;
	}

	m_pPaintManager->RefreshMetrics();
	m_themeCommandBars = themeCommandBars;
	m_strIniFileName   = XTPResourceImages()->m_strIniFileName;

	Refresh();

	if (IsTearOffEnabled() && this == GetTabClientHead())
	{
		CArray<CXTPTabClientWnd*, CXTPTabClientWnd*> tabs;
		GetTabClients(tabs);

		for (int i = 0; i < tabs.GetSize(); i++)
		{
			if (this != tabs.GetAt(i))
			{
				tabs.GetAt(i)->Refresh(TRUE);

				CXTPTearOffFrame* pTearOffFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame,
																   tabs.GetAt(i)->GetParentFrame());
				ASSERT(pTearOffFrame);
				XTP_SAFE_CALL1(pTearOffFrame, Update(this));
				XTP_SAFE_CALL1(pTearOffFrame, UpdateMDIChildrenTheme());
			}
		}
	}
}

int CXTPTabClientWnd::GetWorkspaceCount() const
{
	return (int)m_arrWorkspace.GetSize();
}

CXTPTabClientWnd::CWorkspace* CXTPTabClientWnd::GetWorkspace(int nIndex) const
{
	return m_arrWorkspace[nIndex];
}

void CXTPTabClientWnd::OnIdleUpdateCmdUI()
{
	if (m_bDelayLock)
	{
		m_bDelayLock  = FALSE;
		m_bLockUpdate = FALSE;
	}

	UpdateContents();
}

void CXTPTabClientWnd::CWorkspace::AddNewWindowItem()
{
	if (!m_pNewWindowItem && m_pTabClientWnd->m_bShowNewWindowTab)
	{
		m_pNewWindowItem = CXTPTabManager::AddItem(-1);
		m_pNewWindowItem->SetClosable(FALSE);
		m_pNewWindowItem->GetNavigateButtons()->RemoveAll();
		m_pNewWindowItem->SetNewWindowTab();
	}
}

CXTPTabManagerItem* CXTPTabClientWnd::AddItem(CWnd* pChildWnd)
{
#ifdef _DEBUG
	if (m_bEnableGroups)
	{
		ASSERT((pChildWnd->GetStyle() & WS_MAXIMIZE) == 0);
	}
#endif

	if (GetWorkspaceCount() == 0)
		AddWorkspace();

	if (GetWorkspaceCount() == 0)
		return NULL;

	CWorkspace* pActiveWorkspace = m_pActiveWorkspace ? m_pActiveWorkspace
													  : m_arrWorkspace[GetWorkspaceCount() - 1];

	return pActiveWorkspace->AddItem(pChildWnd);
}

void CXTPTabClientWnd::SetActiveWorkspace(CWorkspace* pWorkspace)
{
	if (m_pActiveWorkspace != pWorkspace)
	{
		if (m_pActiveWorkspace)
			m_pActiveWorkspace->SetActive(FALSE);

		m_pActiveWorkspace = pWorkspace;
		m_pActiveWorkspace->SetActive(TRUE);

		GetParentFrame()->SendMessage(WM_XTP_SETACTIVEWORKSPACE, (WPARAM)pWorkspace);
	}
}

void CXTPTabClientWnd::WorkspaceToScreen(LPPOINT lpPoint) const
{
	if (m_bEnableGroups)
	{
		ClientToScreen(lpPoint);
	}
	else if (GetWorkspaceCount() != 0)
	{
		CWorkspace* pWorkspace = GetWorkspace(0);
		pWorkspace->GetWindow()->ClientToScreen(lpPoint);
	}
}
void CXTPTabClientWnd::ScreenToWorkspace(LPPOINT lpPoint) const
{
	if (m_bEnableGroups)
	{
		ScreenToClient(lpPoint);
	}
	else if (GetWorkspaceCount() != 0)
	{
		CWorkspace* pWorkspace = GetWorkspace(0);
		pWorkspace->GetWindow()->ScreenToClient(lpPoint);
	}
}

void CXTPTabClientWnd::Refresh(BOOL bRecalcLayout)
{
	if (!IsAttached())
		return;

	if (m_bRefreshed)
		return;

	m_bRefreshed = TRUE;

	UpdateContents();

	if (GetWorkspaceCount() != 0)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToWorkspace(&pt);

		if (m_bEnableGroups)
		{
			OnMouseMove(0, pt);
		}
		else if (!m_bUserWorkspace)
		{
			((CSingleWorkspace*)GetWorkspace(0))->OnMouseMove(0, pt);
		}
	}

	Invalidate(FALSE);

	if (bRecalcLayout)
	{
		if (m_bEnableGroups)
			SetWindowPos(NULL, 0, 0, 0, 0,
						 SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
							 | SWP_FRAMECHANGED);

		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
			pCommandBars->RecalcFrameLayout();
	}

	m_bRefreshed = FALSE;
}

void CXTPTabClientWnd::UpdateContents()
{
	if (m_hWnd == 0)
		return;

	if (!m_bShowWorkspace)
		return;

	if (m_bLockUpdate)
		return;

	// check MDI windows
	CFrameWnd* pFrameWnd = GetParentFrame();
	if (pFrameWnd == NULL)
		return;

	if (m_bUpdateContents)
		return;

	m_bUpdateContents = TRUE;

	CheckCommandBarsTheme();

	m_bLockReposition = TRUE;

	// get pointer to currently active MDIChild
	CWnd* pActiveChildWnd = MDIGetActive();

	CXTPTabManagerItem *pActiveItem = NULL, *pNewItem = NULL;

	int i;
	for (i = 0; i < GetWorkspaceCount(); i++)
	{
		for (int nIndex = 0; nIndex < GetWorkspace(i)->GetItemCount(); nIndex++)
			GetWorkspace(i)->GetItem(nIndex)->m_bFound = FALSE;
	}

	// start enumerating from currently active MDIChild
	CWnd* pChildWnd = GetWindow(GW_CHILD);

	// enumerate all child windows
	while (pChildWnd)
	{
		// see if can find
		CXTPTabManagerItem* pFoundItem = FindItem(pChildWnd->GetSafeHwnd());

		if (pFoundItem != NULL)
		{
			if ((pChildWnd->GetStyle() & WS_VISIBLE) == WS_VISIBLE)
			{
				// update text if necessary
				if (m_bAutoUpdateTabCaption)
					pFoundItem->SetCaption(GetItemText(pChildWnd));
			}
			else
			{
				pFoundItem->Remove();
				pFoundItem = NULL;
			}
		}
		else
		{
			if ((pChildWnd->GetStyle() & WS_VISIBLE) == WS_VISIBLE)
			{
				// add item
				pNewItem = pFoundItem = AddItem(pChildWnd);
				if (pNewItem)
				{
					pFoundItem->m_hWnd = pChildWnd->GetSafeHwnd();

#ifdef _XTP_COMMANDBARS_ACTIVEX
					FireNewItem(pNewItem->GetIDispatch(FALSE));
#endif
#ifndef _XTP_COMMANDBARS_ACTIVEX
					pFrameWnd->SendMessage(WM_XTP_NEWTABITEM, (WPARAM)pNewItem);
#endif
					pNewItem->GetTabManager()->SetSelectedItem(pNewItem);
				}
			}
		}
		if (pFoundItem)
		{
			pFoundItem->m_bFound = TRUE;

			if (pChildWnd == pActiveChildWnd)
				pActiveItem = pFoundItem;
		}

		// get next MDIChild
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}

	for (i = GetWorkspaceCount() - 1; i >= 0; i--)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		for (int nIndex = pWorkspace->GetItemCount() - 1; nIndex >= 0; nIndex--)
		{
			CXTPTabManagerItem* pItem = pWorkspace->GetItem(nIndex);
			if (!pItem->m_bFound && pItem != pWorkspace->m_pNewWindowItem)
			{
				pItem->Remove();
			}
		}

		if (pWorkspace->GetItemCount() - (pWorkspace->m_pNewWindowItem ? 1 : 0) == 0)
		{
			if (pWorkspace == m_pActiveWorkspace)
				m_pActiveWorkspace = NULL;

			CWorkspace* pWorkcpaceGrow = i == 0 ? (GetWorkspaceCount() > 1 ? GetWorkspace(i + 1)
																		   : NULL)
												: GetWorkspace(i - 1);
			if (pWorkcpaceGrow)
			{
				pWorkcpaceGrow->m_dHeight += pWorkspace->m_dHeight + m_nSplitterSize;
			}

			if (pWorkspace != m_pTabWorkspace)
				delete pWorkspace;
			m_arrWorkspace.RemoveAt(i);

			m_bForceToRecalc = TRUE;

			if (m_bEnableGroups)
			{
				Invalidate(FALSE);
			}
		}
	}

	// set the active item
	if (pActiveItem != NULL)
	{
		if (pActiveItem->GetTabManager()->GetSelectedItem() != pActiveItem)
		{
			pActiveItem->GetTabManager()->SetSelectedItem(pActiveItem);
		}

		SetActiveWorkspace((CWorkspace*)pActiveItem->GetTabManager());
	}

	m_bLockReposition = FALSE;

	if (m_bForceToRecalc)
	{
		// update the size and position of the tab control and MDIClient window
		if (IsAttached() && ::IsWindow(GetParentFrame()->GetSafeHwnd()))
		{
			Reposition();
		}

		if (pNewItem)
			pNewItem->GetTabManager()->EnsureVisible(pNewItem);

		m_bForceToRecalc = FALSE;
	}

	m_bUpdateContents = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWnd message handlers

// handler for WM_MDIACTIVATE message. Will select corresponding
// tab control item
LRESULT CXTPTabClientWnd::OnMDIActivate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = 0;

	if (m_bEnableGroups || m_bIgnoreFlickersOnActivate || !m_bShowWorkspace)
	{
		lResult = Default();
	}
	else
	{
		SetRedraw(FALSE);

		lResult = Default();

		SetRedraw(TRUE);
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	UpdateContents();
	return lResult;
}

// handler for WM_MDICREATE message. Will add new item into the
// tab control
LRESULT CXTPTabClientWnd::OnMDICreate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();

	if (lResult && m_bEnableGroups)
	{
		CWnd* pWnd = CWnd::FromHandle((HWND)lResult);
		if (pWnd)
		{
			pWnd->ModifyStyle(WS_BORDER | WS_DLGFRAME | WS_THICKFRAME | WS_MAXIMIZEBOX
								  | WS_MINIMIZEBOX,
							  0);
			pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
			pWnd->MoveWindow(0, 0, 0, 0);
		}
	}

	UpdateContents();
	return lResult;
}

void CXTPTabClientWnd::ActivateNextItem(CXTPTabManagerItem* pItem)
{
	if (!pItem)
		return;

	CWorkspace* pWorkspace			  = (CWorkspace*)pItem->GetTabManager();
	CXTPTabManagerItem* pItemActivate = pItem->GetTabManager()->GetItem(pItem->GetIndex() + 1);

	if (!pItemActivate || pItemActivate == pWorkspace->m_pNewWindowItem)
	{
		pItemActivate = pItem->GetTabManager()->GetItem(pItem->GetIndex() - 1);
	}

	if (!pItemActivate)
		return;

	HWND hWndActivate = pItemActivate->GetHandle();
	if (!hWndActivate)
		return;

	::BringWindowToTop(hWndActivate);
}

void CXTPTabClientWnd::ActivateNextTopmostItem(CXTPTabManagerItem* pItem)
{
	if (!pItem)
		return;

	CXTPTabManagerItem* pItemActivate = NULL;
	CWorkspace* pWorkspace			  = (CWorkspace*)pItem->GetTabManager();

	CWnd* pChildWnd = GetWindow(GW_CHILD); // start enumerating from currently active MDIChild

	do // enumerate all child windows
	{
		pChildWnd	 = pChildWnd->GetWindow(GW_HWNDNEXT); // get next MDIChild
		pItemActivate = pWorkspace->FindItem(pChildWnd->GetSafeHwnd());
	} while (pChildWnd && pItemActivate == NULL);

	if (!pItemActivate)
		return;

	HWND hWndActivate = pItemActivate->GetHandle();
	if (!hWndActivate)
		return;

	::BringWindowToTop(hWndActivate);
}

// handler for WM_MDIDESTROY message. Will remove the correspondent item
// from the tab control
LRESULT CXTPTabClientWnd::OnMDIDestroy(WPARAM wParam, LPARAM /*lParam*/)
{
	CWnd* pActiveWnd = MDIGetActive();

	if (pActiveWnd && pActiveWnd->GetSafeHwnd() == (HWND)wParam)
	{
		if (m_afterCloseTabPosition == xtpWorkspaceActivateNextToClosed)
			ActivateNextItem(FindItem((HWND)wParam));
		else if (m_afterCloseTabPosition == xtpWorkspaceActivateTopmost)
			ActivateNextTopmostItem(FindItem((HWND)wParam));
	}

	LRESULT lResult = Default();
	UpdateContents();

	CXTPTearOffFrame* pParentFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame, GetParentFrame());
	if (pParentFrame && !MDIGetActive())
		pParentFrame->PostMessage(WM_CLOSE);

	return lResult;
}

LRESULT CXTPTabClientWnd::OnMDINext(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	UpdateContents();
	return lResult;
}

BOOL CXTPTabClientWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT && m_bEnableGroups)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		for (int nIndex = 0; nIndex < GetWorkspaceCount(); nIndex++)
		{
			CWorkspace* pWorkspace = GetWorkspace(nIndex);
			if (pWorkspace->m_rcSplitter.PtInRect(point))
			{
				SetCursor(!m_bHorizSplitting ? m_hCursorHoriz : m_hCursorVert);
				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPTabClientWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (GetWorkspaceCount() == 0 || !m_bShowWorkspace || m_bUserWorkspace)
	{
		CWnd::CalcWindowRect(lpClientRect, nAdjustType);
		return;
	}

	if (!m_bEnableGroups)
	{
		CSingleWorkspace* pWorkspace = (CSingleWorkspace*)GetWorkspace(0);

		// now do the laying out
		HDWP dwh = BeginDeferWindowPos(2);

		// move tab window
		if (::IsWindow(pWorkspace->m_hWnd) && (pWorkspace->GetStyle() & WS_VISIBLE) == WS_VISIBLE)
		{
			{
				// move tab control
				dwh = ::DeferWindowPos(dwh, pWorkspace->m_hWnd, NULL, lpClientRect->left,
									   lpClientRect->top, lpClientRect->right - lpClientRect->left,
									   lpClientRect->bottom - lpClientRect->top, SWP_NOZORDER);

				CClientDC dc(pWorkspace);

				CRect rectTab(lpClientRect);
				rectTab.OffsetRect(-rectTab.TopLeft());

				GetPaintManager()->RepositionTabControl(pWorkspace, &dc, rectTab);

				CRect rectTabAdjust(lpClientRect);
				GetPaintManager()->AdjustClientRect(pWorkspace, rectTabAdjust);
				*lpClientRect = rectTabAdjust;

				pWorkspace->Invalidate(FALSE);
			}

			if (lpClientRect->bottom < lpClientRect->top
				|| lpClientRect->right < lpClientRect->left)
				::memset(lpClientRect, 0, sizeof(RECT));

			::ShowWindow(pWorkspace->m_hWnd, SW_SHOWNA);
		}

#ifndef _XTP_COMMANDBARS_ACTIVEX
		// move MDIClient window
		dwh = ::DeferWindowPos(dwh, m_hWnd, NULL, lpClientRect->left, lpClientRect->top,
							   lpClientRect->right - lpClientRect->left,
							   lpClientRect->bottom - lpClientRect->top, SWP_NOZORDER);
#endif

		EndDeferWindowPos(dwh);
	}
	else
	{
		CClientDC dc(this);

		CRect rcBorders(2, 2, -2, -2); // Should not be DPI scaled
		m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(NULL, rcBorders,
															   xtpTabWorkspacePartWidth);

		CRect rcClient(0, 0, lpClientRect->right - lpClientRect->left + rcBorders.Width(),
					   lpClientRect->bottom - lpClientRect->top + rcBorders.Height());

		int nIndex;
		double dTotalHeight = 0;
		int nWorkspaceCount = GetWorkspaceCount();

		for (nIndex = 0; nIndex < nWorkspaceCount; nIndex++)
		{
			dTotalHeight += GetWorkspace(nIndex)->m_dHeight;
		}
		int nTotalHeight = (m_bHorizSplitting ? rcClient.Height() : rcClient.Width())
						   - m_nSplitterSize * (nWorkspaceCount - 1);
		double dHeight = 0;
		if ((int)dTotalHeight == 0)
			dTotalHeight = 1.0;

		CRect rcWorkspace(rcClient);

		for (nIndex = 0; nIndex < nWorkspaceCount; nIndex++)
		{
			CWorkspace* pWorkspace = GetWorkspace(nIndex);

			CRect rc(rcClient);
			if (nIndex != nWorkspaceCount - 1)
			{
				dHeight = dHeight += pWorkspace->m_dHeight;

				int nHeight = int(dHeight * nTotalHeight / dTotalHeight) + m_nSplitterSize * nIndex;

				if (m_bHorizSplitting)
				{
					pWorkspace->m_rcSplitter.SetRect(rc.left, rcWorkspace.top + nHeight, rc.right,
													 rcWorkspace.top + nHeight + m_nSplitterSize);

					rc.bottom	= rcWorkspace.top + nHeight;
					rcClient.top = rc.bottom + m_nSplitterSize;
				}
				else
				{
					pWorkspace->m_rcSplitter.SetRect(rcWorkspace.left + nHeight, rc.top,
													 rcWorkspace.left + nHeight + m_nSplitterSize,
													 rc.bottom);

					rc.right	  = rcWorkspace.left + nHeight;
					rcClient.left = rc.right + m_nSplitterSize;
				}
			}
			else
			{
				pWorkspace->m_rcSplitter.SetRectEmpty();
			}

			GetPaintManager()->RepositionTabControl(pWorkspace, &dc, rc);
			rc = pWorkspace->GetClientRect();

			for (int i = 0; i < pWorkspace->GetItemCount(); i++)
			{
				HWND hWndChild = pWorkspace->GetItem(i)->m_hWnd;
				::MoveWindow(hWndChild, rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
			}
		}

#ifdef _XTP_COMMANDBARS_ACTIVEX
		CXTPBufferDC dcMem(dc, rcWorkspace);
		OnDraw(&dcMem, rcWorkspace);
#else
		Invalidate(FALSE);
		// UpdateWindow();
#endif
	}

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

// update the size of the tab control and the MDIClient window
void CXTPTabClientWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

BOOL CXTPTabClientWnd::OnEraseBkgnd(CDC*)
{
	if (!m_bThemedBackColor && !m_bEnableGroups)
	{
		return (BOOL)Default();
	}
	return TRUE;
}

LRESULT CXTPTabClientWnd::OnPrintClient(WPARAM wParam, LPARAM)
{
	if (!m_bThemedBackColor && !m_bEnableGroups)
	{
		return (LRESULT)Default();
	}

	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		CXTPClientRect rc(this);
		OnDraw(pDC, rc);
	}
	return TRUE;
}

void CXTPTabClientWnd::OnFillBackground(CDC* pDC, CRect rc)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
	{
		if (!m_bThemedBackColor)
		{
			DefWindowProc(WM_PAINT, (WPARAM)pDC->m_hDC, 0);
		}
		else
		{
			m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(pDC, rc,
																   xtpTabWorkspacePartBack);
		}
	}
	else
	{
		pDC->FillSolidRect(rc, GetSysColor(COLOR_APPWORKSPACE));
	}
}

void CXTPTabClientWnd::OnDraw(CDC* pDC, CRect rc)
{
	if (GetWorkspaceCount() == 0 || !m_bEnableGroups)
	{
		OnFillBackground(pDC, rc);
	}
	else
	{
		CRect rcClipBox;
		pDC->GetClipBox(&rcClipBox);

		for (int i = GetWorkspaceCount() - 1; i >= 0; i--)
		{
			CWorkspace* pWorkspace = GetWorkspace(i);

			if (CRect().IntersectRect(rcClipBox, pWorkspace->m_rcControl))
			{
				GetPaintManager()->DrawTabControl(GetWorkspace(i), pDC, pWorkspace->m_rcControl);
			}

			if (!pWorkspace->m_rcSplitter.IsRectEmpty())
			{
				m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(
					pDC, pWorkspace->m_rcSplitter,
					m_bHorizSplitting ? xtpTabWorkspacePartHSplitter
									  : xtpTabWorkspacePartVSplitter);
			}
		}
	}
}

void CXTPTabClientWnd::OnPaint()
{
	if (!m_bThemedBackColor && !m_bEnableGroups)
	{
		Default();
		return;
	}
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint);

	OnDraw(&dc, rc);
}

void CXTPTabClientWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	if (m_bEnableGroups)
	{
		m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(NULL, &lpncsp->rgrc[0],
															   xtpTabWorkspacePartWidth);
	}

	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CXTPTabClientWnd::OnNcActivate(BOOL bActive)
{
	if (m_bEnableGroups)
		return TRUE;

	return CWnd::OnNcActivate(bActive);
}

void CXTPTabClientWnd::OnNcPaint()
{
	CWnd::OnNcPaint();

	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	CWindowDC dc(this);
	m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(&dc, rc, xtpTabWorkspacePartBorder);
}

void CXTPTabClientWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	CXTPTabManagerItem* pItem = HitTest(point);
	FireRClick(pItem ? pItem->GetIDispatch(FALSE) : NULL);
#endif
}

void CXTPTabClientWnd::RepositionWorkspaces(CRect rc, CRect rcAvail, CWorkspace* pWorkspaceFirst,
											CWorkspace* pWorkspaceSecond)
{
	double dTotal = pWorkspaceFirst->m_dHeight + pWorkspaceSecond->m_dHeight + m_nSplitterSize;

	if (!m_bHorizSplitting)
	{
		double dDelta			   = double(rc.left - rcAvail.left) / rcAvail.Width();
		pWorkspaceFirst->m_dHeight = GetExStyle() & WS_EX_LAYOUTRTL
										 ? dTotal - dDelta * dTotal - m_nSplitterSize
										 : dDelta * dTotal;
	}
	else
	{
		double dDelta			   = double(rc.top - rcAvail.top) / rcAvail.Height();
		pWorkspaceFirst->m_dHeight = dDelta * dTotal;
	}

	pWorkspaceSecond->m_dHeight = dTotal - pWorkspaceFirst->m_dHeight - m_nSplitterSize;
}

void CXTPTabClientWnd::NormalizeWorkspaceSize()
{
	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		pWorkspace->m_dHeight  = !m_bHorizSplitting ? pWorkspace->GetControlRect().Width()
												   : pWorkspace->GetControlRect().Height();
	}
}

void CXTPTabClientWnd::TrackSplitter(int nWorkspace, CPoint point)
{
	ASSERT(nWorkspace >= 0 && nWorkspace < GetWorkspaceCount() - 1);

	if (nWorkspace < 0 || nWorkspace >= GetWorkspaceCount() - 1)
		return;

	NormalizeWorkspaceSize();

	CXTPSplitterTracker tracker;

	CWorkspace* pWorkspaceFirst  = m_arrWorkspace[nWorkspace];
	CWorkspace* pWorkspaceSecond = m_arrWorkspace[nWorkspace + 1];

	CRect rcFirst  = pWorkspaceFirst->GetControlRect();
	CRect rcSecond = pWorkspaceSecond->GetControlRect();

	CRect rcAvail;
	rcAvail.UnionRect(rcFirst, rcSecond);

	CRect rc = pWorkspaceFirst->m_rcSplitter;

	ClientToScreen(&rc);
	ClientToScreen(&rcAvail);
	ClientToScreen(&point);

	if (m_bUseSplitterTracker)
	{
		if (tracker.Track(this, rcAvail, rc, point, !m_bHorizSplitting))
		{
			RepositionWorkspaces(rc, rcAvail, pWorkspaceFirst, pWorkspaceSecond);
			if (IsAttached())
				GetParentFrame()->RecalcLayout();
		}
	}
	else
	{
		CPoint ptOffset = !m_bHorizSplitting ? CPoint(rc.left - point.x, 0)
											 : CPoint(0, rc.top - point.y);

		SetCapture();

		while (CWnd::GetCapture() == this)
		{
			MSG msg;

			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(msg.lParam);
				ClientToScreen(&point);
				point += ptOffset;

				point.x = max(min(point.x, rcAvail.right), rcAvail.left);
				point.y = max(min(point.y, rcAvail.bottom), rcAvail.top);

				if (!m_bHorizSplitting)
				{
					if (rc.left == point.x)
						continue;
					rc.OffsetRect(point.x - rc.left, 0);
				}
				else
				{
					if (rc.top == point.y)
						continue;
					rc.OffsetRect(0, point.y - rc.top);
				}

				RepositionWorkspaces(rc, rcAvail, pWorkspaceFirst, pWorkspaceSecond);

				if (IsAttached())
					GetParentFrame()->RecalcLayout();
			}
			else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
				break;
			else if (msg.message == WM_LBUTTONUP)
				break;
			else
				::DispatchMessage(&msg);
		}

		if (CWnd::GetCapture() == this)
			ReleaseCapture();
	}
}

void CXTPTabClientWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bEnableGroups)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	m_bLockUpdate = TRUE;

#ifndef _XTP_COMMANDBARS_ACTIVEX
	m_bDelayLock = TRUE;
#endif

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		if (pWorkspace->m_rcSplitter.PtInRect(point) && i < GetWorkspaceCount() - 1)
		{
			TrackSplitter(i, point);
			break;
		}

		if (pWorkspace->GetControlRect().PtInRect(point))
		{
			if (!pWorkspace->IsActive())
			{
				CXTPTabManagerItem* pSelectedItem = pWorkspace->GetSelectedItem();
				if (pSelectedItem)
					pWorkspace->OnItemClick(pSelectedItem);
			}

			pWorkspace->PerformClick(m_hWnd, point);
			break;
		}
	}

#ifdef _XTP_COMMANDBARS_ACTIVEX
	m_bLockUpdate = FALSE;
#endif
}

int CXTPTabClientWnd::FindIndex(CXTPTabManager* pWorkspace) const
{
	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		if (GetWorkspace(i) == pWorkspace)
			return i;
	}
	return -1;
}

CXTPTabManagerItem* CXTPTabClientWnd::HitTest(CPoint pt) const
{
	if (!m_bShowWorkspace)
		return NULL;

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CXTPTabManagerItem* pItem = GetWorkspace(i)->HitTest(pt);
		if (pItem)
			return pItem;
	}
	return NULL;
}

CXTPCommandBars* CXTPTabClientWnd::GetCommandBars() const
{
	return m_pCommandBars;
}

void CXTPTabClientWnd::SetCommandBars(CXTPCommandBars* pCommandBars)
{
	m_pCommandBars = pCommandBars;
}

void CXTPTabClientWnd::ContextMenu(CPoint pt)
{
	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

	ClientToScreen(&pt);

	pPopupBar->GetControls()->Add(new CXTPControlWorkspaceActions, 0);

	CXTPControl* pControl = pPopupBar->GetControls()->Add(xtpControlButton,
														  XTP_ID_WORKSPACE_CANCEL);
	pControl->SetBeginGroup(TRUE);
	pControl->SetFlags(xtpFlagManualUpdate);

	CXTPCommandBars::TrackPopupMenu(pPopupBar, 0, pt.x, pt.y, GetParentFrame());

	pPopupBar->InternalRelease();
}

void CXTPTabClientWnd::OnWorkspaceCommand(UINT nID)
{
	CXTPTabManagerItem* pItem = NULL;

	if (!IsWorkspaceCommandEnabled(nID, &pItem))
		return;

	ASSERT(pItem);
	if (!pItem)
		return;

	switch (nID)
	{
		case XTP_ID_WORKSPACE_NEWVERTICAL:
			DoWorkspaceCommand(pItem, (CWorkspace*)pItem->GetTabManager(),
							   XTP_TABCLIENT_ACTION_ATTACH_RIGHT);
			break;

		case XTP_ID_WORKSPACE_NEWHORIZONTAL:
			DoWorkspaceCommand(pItem, (CWorkspace*)pItem->GetTabManager(),
							   XTP_TABCLIENT_ACTION_ATTACH_BOTTOM);
			break;

		case XTP_ID_WORKSPACE_MOVEPREVIOUS:
		{
			int nIndex = FindIndex(pItem->GetTabManager());
			DoWorkspaceCommand(pItem, GetWorkspace(nIndex - 1), XTP_TABCLIENT_ACTION_ATTACH_CENTER);
		}
		break;
		case XTP_ID_WORKSPACE_MOVENEXT:
		{
			int nIndex = FindIndex(pItem->GetTabManager());
			DoWorkspaceCommand(pItem, GetWorkspace(nIndex + 1), XTP_TABCLIENT_ACTION_ATTACH_CENTER);
		}
		break;

		case XTP_ID_WORKSPACE_DETACH:
			DoWorkspaceCommand(pItem, 0, XTP_TABCLIENT_ACTION_DETACH);
			break;

			// case XTP_ID_WORKSPACE_DETACH_ALL:
			// case XTP_ID_WORKSPACE_ATTACH:
			// case XTP_ID_WORKSPACE_ATTACH_ALL:
	}
}

void CXTPTabClientWnd::OnUpdateWorkspaceCommand(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsWorkspaceCommandEnabled(pCmdUI->m_nID));
}

BOOL CXTPTabClientWnd::IsWorkspaceCommandEnabled(UINT nID, CXTPTabManagerItem** ppItem)
{
	if (!m_bEnableGroups)
		return FALSE;

	CWnd* pWnd = MDIGetActive();
	if (!pWnd)
		return FALSE;

	CXTPTabManagerItem* pItem = FindItem(pWnd->GetSafeHwnd());
	if (!pItem)
		return FALSE;

	if (ppItem)
		*ppItem = pItem;

	switch (nID)
	{
		case XTP_ID_WORKSPACE_NEWVERTICAL:
			return (GetWorkspaceCount() == 1 || !m_bHorizSplitting)
				   && pItem->GetTabManager()->GetItemCount() > 1;

		case XTP_ID_WORKSPACE_NEWHORIZONTAL:
			return (GetWorkspaceCount() == 1 || m_bHorizSplitting)
				   && pItem->GetTabManager()->GetItemCount() > 1;

		case XTP_ID_WORKSPACE_MOVEPREVIOUS: return FindIndex(pItem->GetTabManager()) != 0;

		case XTP_ID_WORKSPACE_MOVENEXT:
			return FindIndex(pItem->GetTabManager()) != GetWorkspaceCount() - 1;

		case XTP_ID_WORKSPACE_DETACH:
			return IsTearOffEnabled() && pItem->GetTabManager()->GetItemCount() > 0;

			// case XTP_ID_WORKSPACE_DETACH_ALL:
			// case XTP_ID_WORKSPACE_ATTACH:
			// case XTP_ID_WORKSPACE_ATTACH_ALL:
	}
	return TRUE;
}

void CXTPTabClientWnd::MDITile(BOOL bHorizontal)
{
	if (!m_bEnableGroups || m_bUserWorkspace)
		return;

	if (GetItemCount() < 2)
		return;

	m_bLockUpdate = m_bLockReposition = TRUE;

	CArray<CXTPTabManagerItem*, CXTPTabManagerItem*> arrItems;

	while (GetWorkspaceCount() > 0)
	{
		CWorkspace* pWorkspace = GetWorkspace(0);

		for (int j = 0; j < pWorkspace->GetItemCount(); j++)
		{
			CXTPTabManagerItem* pItem = pWorkspace->GetItem(j);
			arrItems.Add(pItem);
			pItem->InternalAddRef();
		}

		m_arrWorkspace.RemoveAt(0);
		delete pWorkspace;
	}

	for (int i = 0; i < (int)arrItems.GetSize(); i++)
	{
		CXTPTabManagerItem* pItem = arrItems[i];
		COLORREF clr			  = pItem->m_clrItem;

		CWorkspace* pWorkspace = AddWorkspace(i);

		pWorkspace->CXTPTabManager::AddItem(-1, pItem);

		pItem->m_clrItem = clr;
		pWorkspace->SetSelectedItem(pItem);

		pWorkspace->m_dHeight = XTP_DPI_Y(100);
	}

	m_pActiveWorkspace = NULL;
	m_bHorizSplitting  = bHorizontal;

	m_bLockUpdate = m_bLockReposition = FALSE;
	m_bForceToRecalc				  = TRUE;
	UpdateContents();

	NormalizeWorkspaceSize();
}

void CXTPTabClientWnd::DoWorkspaceCommand(CXTPTabManagerItem* pItem, CWorkspace* pFocusWorkspace,
										  int nAction, CPoint pt,
										  CXTPTabClientWnd* pTargetTab /*= NULL*/
#ifndef _XTP_COMMANDBARS_ACTIVEX
										  ,
#	ifdef _XTP_INCLUDE_DOCKINGPANE
										  CXTPDockingPaneTabbedContainer* pTargetPane /*= NULL*/
#	else
										  LPVOID pReserved /*= NULL*/
#	endif
#endif
)
{
	m_bLockUpdate = m_bLockReposition = TRUE;
	CWorkspace* pOldWorkspace		  = NULL;

	switch (nAction)
	{
		case XTP_TABCLIENT_ACTION_ATTACH_CENTER:
		{
			ASSERT(pFocusWorkspace);
			if (!pFocusWorkspace)
				return;

			if (pFocusWorkspace != pItem->GetTabManager())
			{
				pOldWorkspace = (CWorkspace*)pItem->GetTabManager();
				pItem->InternalAddRef();
				pItem->Remove();

				COLORREF clr = pItem->m_clrItem;

				int nInsert = pFocusWorkspace->GetItemCount()
							  + (pFocusWorkspace->m_pNewWindowItem ? -1 : 0);
				pFocusWorkspace->CXTPTabManager::AddItem(nInsert, pItem);

				pItem->m_clrItem = clr;
			}

			pFocusWorkspace->SetSelectedItem(pItem);

			break;
		}
		case XTP_TABCLIENT_ACTION_ATTACH_LEFT:
		case XTP_TABCLIENT_ACTION_ATTACH_RIGHT:
		case XTP_TABCLIENT_ACTION_ATTACH_TOP:
		case XTP_TABCLIENT_ACTION_ATTACH_BOTTOM:
		{
			NormalizeWorkspaceSize();

			ASSERT(pFocusWorkspace);
			if (!pFocusWorkspace)
				return;

			pOldWorkspace = (CWorkspace*)pItem->GetTabManager();

			pItem->InternalAddRef();
			pItem->Remove();

			COLORREF clr = pItem->m_clrItem;

			int nIndex = 0;
			if (nAction == XTP_TABCLIENT_ACTION_ATTACH_RIGHT)
				nIndex = FindIndex(pFocusWorkspace) + 1;
			if (nAction == XTP_TABCLIENT_ACTION_ATTACH_LEFT)
				nIndex = FindIndex(pFocusWorkspace);
			if (nAction == XTP_TABCLIENT_ACTION_ATTACH_BOTTOM)
				nIndex = FindIndex(pFocusWorkspace) + 1;
			if (nAction == XTP_TABCLIENT_ACTION_ATTACH_TOP)
				nIndex = FindIndex(pFocusWorkspace);

			CWorkspace* pWorkspace = AddWorkspace(nIndex);

			pWorkspace->AddNewWindowItem();

			int nInsert = pWorkspace->GetItemCount() + (pWorkspace->m_pNewWindowItem ? -1 : 0);
			pWorkspace->CXTPTabManager::AddItem(nInsert, pItem);

			pItem->m_clrItem = clr;
			pWorkspace->SetSelectedItem(pItem);

			pWorkspace->m_dHeight	  = (pFocusWorkspace->m_dHeight - m_nSplitterSize) / 2;
			pFocusWorkspace->m_dHeight = pFocusWorkspace->m_dHeight - pWorkspace->m_dHeight
										 - m_nSplitterSize;

			m_bHorizSplitting = (nAction == XTP_TABCLIENT_ACTION_ATTACH_BOTTOM
								 || nAction == XTP_TABCLIENT_ACTION_ATTACH_TOP);
			break;
		}
		case XTP_TABCLIENT_ACTION_DETACH:
		{
			if (!IsTearOffEnabled())
				break;

			CMDIChildWnd* pMDIChildWnd = DYNAMIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
			if (NULL == pMDIChildWnd)
				break;

			CCreateContext context;
			context.m_pCurrentFrame = GetParentFrame();
			context.m_pCurrentDoc   = pMDIChildWnd->GetActiveDocument();
			context.m_pLastView		= pMDIChildWnd->GetActiveView();

			CXTPTearOffFrame* pTearOffFrm = new CXTPTearOffFrame();
			if (!pTearOffFrm->LoadFrame(IDR_XTP_TEAROFF_FRM,
										FWS_ADDTOTITLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN
											| WS_CLIPSIBLINGS,
										NULL, &context))
			{
				ASSERT(FALSE);
				delete pTearOffFrm;
				return;
			}

			pTearOffFrm->Init(this, pItem);

			m_lstNextTab.AddTail(pTearOffFrm->GetTabClient());

			ChangeMDIChildFrame(pTearOffFrm);

			if (!MDIGetActive())
			{
				CXTPTearOffFrame* pTearOffFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame, GetParent());
				XTP_SAFE_CALL1(pTearOffFrame, PostMessage(WM_CLOSE));
			}

			break;
		}
		case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_LEFT:
		case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_TOP:
		case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_RIGHT:
		case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_BOTTOM:
		case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER:
		{
			if (!IsTearOffEnabled())
				break;
			ASSERT(pTargetTab);

			POINT ptWorkspace(pt);
			pTargetTab->ScreenToWorkspace(&ptWorkspace);
			int nWorkspace = -1;
			for (int i = 0; i < pTargetTab->GetWorkspaceCount(); i++)
			{
				if (pTargetTab->GetWorkspace(i)->GetHeaderRect().PtInRect(ptWorkspace)
					|| pTargetTab->GetWorkspace(i)->GetClientRect().PtInRect(ptWorkspace))
				{
					pTargetTab->GetWorkspace(i)->OnItemClick(
						pTargetTab->GetWorkspace(i)->GetItem(0));
					nWorkspace = i;
					break;
				}
			}

			ChangeMDIChildFrame(pTargetTab->GetParent());

			XTPTabClientStickerPart sticker = xtpTabClientStickerNone;
			if (nAction == XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_LEFT)
				sticker = xtpTabClientStickerLeft;
			else if (nAction == XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_TOP)
				sticker = xtpTabClientStickerTop;
			else if (nAction == XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_RIGHT)
				sticker = xtpTabClientStickerRight;
			else if (nAction == XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_BOTTOM)
				sticker = xtpTabClientStickerBottom;
			else if (nAction == XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER)
				sticker = xtpTabClientStickerCenter;

			ChangeItemWorkspace(pTargetTab, nWorkspace, sticker);

			if (!MDIGetActive())
			{
				CXTPTearOffFrame* pTearOffFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame, GetParent());
				XTP_SAFE_CALL1(pTearOffFrame, PostMessage(WM_CLOSE));
			}

			break;
		}
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
		case XTP_TABCLIENT_ACTION_DOCK_CENTER:
		case XTP_TABCLIENT_ACTION_DOCK_LEFT:
		case XTP_TABCLIENT_ACTION_DOCK_RIGHT:
		case XTP_TABCLIENT_ACTION_DOCK_TOP:
		case XTP_TABCLIENT_ACTION_DOCK_BOTTOM:
		{
			ASSERT(!pFocusWorkspace);
			ASSERT(!pTargetTab);
			ASSERT(pTargetPane);
			ASSERT(pItem);

			CXTPDockingPaneManager* pDockingPaneManager = GetDockingPaneManager();
			if (pDockingPaneManager)
			{
				UINT paneID = XTPToUInt(
					GetParentFrame()->SendMessage(WM_XTP_TABTODOCKPANE, (WPARAM)pItem));
				ASSERT(paneID != 0);

				CXTPDockingPane* pPane = pDockingPaneManager->CreatePane(
					paneID, pTargetPane->GetPaneWindowRect(), xtpPaneDockUndefined);

				if (nAction == XTP_TABCLIENT_ACTION_DOCK_CENTER)
					m_paneManager->AttachPane(pPane, pTargetPane);
				else if (nAction == XTP_TABCLIENT_ACTION_DOCK_LEFT)
					pDockingPaneManager->DockPane(pPane, xtpPaneDockLeft, pTargetPane);
				else if (nAction == XTP_TABCLIENT_ACTION_DOCK_RIGHT)
					pDockingPaneManager->DockPane(pPane, xtpPaneDockRight, pTargetPane);
				else if (nAction == XTP_TABCLIENT_ACTION_DOCK_TOP)
					pDockingPaneManager->DockPane(pPane, xtpPaneDockTop, pTargetPane);
				else if (nAction == XTP_TABCLIENT_ACTION_DOCK_BOTTOM)
					pDockingPaneManager->DockPane(pPane, xtpPaneDockBottom, pTargetPane);

				pPane->SetTitle(GetSelectedItem()->GetCaption());
				pPane->Select();

				CMDIChildWnd* pFrame = DYNAMIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
				if (pFrame)
				{
					CDocument* pDocument = ((CMDIChildWnd*)pFrame)->GetActiveDocument();
					if (pDocument)
					{
						CXTPMultiDocTemplate* pTemplate = (CXTPMultiDocTemplate*)
															  pDocument->GetDocTemplate();
						if (pTemplate)
						{
							pPane->AttachView(this, pTemplate->m_pViewClass, pDocument);

							XTP_SAFE_CALL1(pFrame, DestroyWindow());
						}
					}
				}
			}

			break;
		}
#	else
			UNREFERENCED_PARAMETER(pReserved);
#	endif
#endif
	}

	if (pOldWorkspace && pOldWorkspace->GetItemCount() > 1)
	{
		HWND hWndChild = ::GetWindow(m_hWnd, GW_CHILD);
		while (hWndChild)
		{
			CXTPTabManagerItem* pItemOld = FindItem(hWndChild);
			if (pItemOld && pItemOld->GetTabManager() == pOldWorkspace)
			{
				pOldWorkspace->SetSelectedItem(pItemOld);
				break;
			}
			hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
		}
	}

	m_bLockUpdate = m_bLockReposition = FALSE;
	m_bForceToRecalc				  = TRUE;
	UpdateContents();
}

void CXTPTabClientWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bEnableGroups)
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			GetWorkspace(i)->PerformMouseMove(m_hWnd, point);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CXTPTabClientWnd::OnMouseLeave()
{
	if (m_bEnableGroups)
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			GetWorkspace(i)->PerformMouseMove(m_hWnd, CPoint(-1, -1));
		}
	}
}

void CXTPTabClientWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

void CXTPTabClientWnd::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	GetPaintManager()->RefreshMetrics();

	Refresh();
}

BOOL CXTPTabClientWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_bEnableGroups)
	{
#ifndef _XTP_COMMANDBARS_ACTIVEX
		if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN
			|| pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK
			|| pMsg->message == WM_RBUTTONDBLCLK)
		{
			if (pMsg->hwnd != m_hWnd)
				return CWnd::PreTranslateMessage(pMsg);

			if (IsAttached()
				&& GetParentFrame()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG, (WPARAM)pMsg->message,
												 pMsg->lParam))
				return TRUE;
		}
#endif
	}

	return CWnd::PreTranslateMessage(pMsg);
}

CXTPTabPaintManager* CXTPTabClientWnd::GetPaintManager() const
{
	return m_pPaintManager;
}

void CXTPTabClientWnd::SetPaintManager(CXTPTabPaintManager* pPaintManager)
{
	m_pPaintManager->InternalRelease();
	m_pPaintManager = pPaintManager;
	m_pPaintManager->RefreshMetrics();

	m_themeCommandBars = xtpThemeCustom;
	m_bAutoTheme	   = FALSE;

	Refresh();
}

void CXTPTabClientWnd::SetAutoTheme(BOOL bAutoTheme /*= FALSE*/)
{
	m_bAutoTheme = bAutoTheme;
}

BOOL CXTPTabClientWnd::GetAutoTheme() const
{
	return m_bAutoTheme;
}

void CXTPTabClientWnd::UpdateFlags(CWorkspace* pWorkspace)
{
	pWorkspace->FindNavigateButton(xtpTabNavigateButtonClose)
		->SetFlags(m_dwFlags & xtpWorkspaceHideClose ? xtpTabNavigateButtonNone
													 : xtpTabNavigateButtonAutomatic);

	pWorkspace->FindNavigateButton(xtpTabNavigateButtonActiveFiles)
		->SetFlags(m_dwFlags & xtpWorkspaceShowActiveFiles ? xtpTabNavigateButtonAlways
														   : xtpTabNavigateButtonNone);

	pWorkspace->FindNavigateButton(xtpTabNavigateButtonLeft)
		->SetFlags((m_dwFlags & xtpWorkspaceHideArrowsAlways) == xtpWorkspaceHideArrowsAlways
					   ? xtpTabNavigateButtonNone
					   : (m_dwFlags & xtpWorkspaceHideArrows ? xtpTabNavigateButtonAutomatic
															 : xtpTabNavigateButtonAlways));
	pWorkspace->FindNavigateButton(xtpTabNavigateButtonRight)
		->SetFlags((m_dwFlags & xtpWorkspaceHideArrowsAlways) == xtpWorkspaceHideArrowsAlways
					   ? xtpTabNavigateButtonNone
					   : (m_dwFlags & xtpWorkspaceHideArrows ? xtpTabNavigateButtonAutomatic
															 : xtpTabNavigateButtonAlways));

	if (m_dwFlags & xtpWorkspaceShowCloseTab || m_dwFlags & xtpWorkspaceShowCloseSelectedTab)
	{
		pWorkspace->ShowCloseItemButton(m_dwFlags & xtpWorkspaceShowCloseSelectedTab
											? xtpTabNavigateButtonAutomatic
											: xtpTabNavigateButtonAlways);
	}
}

void CXTPTabClientWnd::SetFlags(DWORD dwFlags)
{
	m_dwFlags = dwFlags;

	for (int i = 0; i < GetWorkspaceCount(); i++)
		UpdateFlags(GetWorkspace(i));

	Reposition();
}

void CXTPTabClientWnd::Reposition()
{
	m_bForceToRecalc = FALSE;

	if (m_pTabWorkspace)
	{
		m_pTabWorkspace->OnRecalcLayout();
	}
	else if (!m_bUserWorkspace)
	{
		if (IsAttached())
			GetParentFrame()->RecalcLayout();
	}
}

BOOL CXTPTabClientWnd::IsAllowReorder() const
{
	return m_bAllowReorder;
}

void CXTPTabClientWnd::SetAllowReorder(BOOL bAllowReorder)
{
	m_bAllowReorder = bAllowReorder;
}

CXTPTabManagerItem* CXTPTabClientWnd::GetSelectedItem() const
{
	if (m_pActiveWorkspace)
		return m_pActiveWorkspace->GetSelectedItem();
	return NULL;
}

CXTPTabManagerItem* CXTPTabClientWnd::GetItem(int nIndex) const
{
	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		int nCount			   = pWorkspace->GetItemCount();

		if (nIndex < nCount)
			return pWorkspace->GetItem(nIndex);

		nIndex -= nCount;
	}
	return NULL;
}

int CXTPTabClientWnd::GetItemCount() const
{
	int nCount = 0;

	for (int i = 0; i < GetWorkspaceCount(); i++)
		nCount += GetWorkspace(i)->GetItemCount();

	return nCount;
}

LRESULT CXTPTabClientWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_bEnableGroups && IsAttached())
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	if (message == m_nMsgQuerySkinState && m_bEnableGroups && IsAttached())
	{
		return TRUE;
	}

	if (!m_bEnableGroups && GetCommandBars())
	{
		if (message == WM_MDIICONARRANGE || message == WM_MDICASCADE || message == WM_MDITILE)
		{
			CArray<CXTPCommandBarsFrameHook*, CXTPCommandBarsFrameHook*> arrChildren;

			HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD);
			while (hWnd)
			{
				if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
				{
					CXTPCommandBarsFrameHook* pFrame = (CXTPCommandBarsFrameHook*)::SendMessage(
						hWnd, CXTPCommandBarsFrameHook::m_nMsgQueryFrameHook, 0, 0);

					if (pFrame)
					{
						RECT rc = CXTPWindowRect(hWnd);
						pFrame->m_bLockNCPaint += 1;
						::SendMessage(hWnd, WM_NCCALCSIZE, FALSE, (LPARAM)&rc);
						pFrame->m_bLockNCPaint -= 1;

						::SetTimer(hWnd, XTP_TID_REFRESHFRAME, 100, NULL);

						arrChildren.Add(pFrame);
					}
				}
				hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
			}

			LRESULT lResult = CWnd::WindowProc(message, wParam, lParam);

			for (int i = 0; i < (int)arrChildren.GetSize(); i++)
			{
				CXTPCommandBarsFrameHook* pFrame = arrChildren[i];
				pFrame->RedrawFrame();
			}

			return lResult;
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CXTPTabClientWnd::SetTabWorkspace(CWorkspace* pWorkspace)
{
	ASSERT(!m_bEnableGroups);

	m_pTabWorkspace  = pWorkspace;
	m_bUserWorkspace = pWorkspace != NULL;

	if (pWorkspace)
		pWorkspace->m_pTabClientWnd = this;
}

void CXTPTabClientWnd::ShowNewWindowTab(BOOL bShowNewWindowTab, int nCommand)
{
	m_bShowNewWindowTab	= bShowNewWindowTab;
	m_nNewWindowTabCommand = nCommand;
}

void CXTPTabClientWnd::SaveState()
{
	AfxGetApp()->WriteProfileInt(_T("TabDocking"), _T("Position"),
								 (int)m_pPaintManager->GetPosition());
}

void CXTPTabClientWnd::LoadState()
{
	UINT nValue = AfxGetApp()->GetProfileInt(_T("TabDocking"), _T("Position"), xtpTabPositionTop);
	if (xtpTabPositionTop == nValue || xtpTabPositionLeft == nValue
		|| xtpTabPositionBottom == nValue || xtpTabPositionRight == nValue)
	{
		m_pPaintManager->SetPosition(static_cast<XTPTabPosition>(nValue));
		m_bForceToRecalc = TRUE;
	}
}

CXTPTabClientWnd* CXTPTabClientWnd::GetTabClientHead()
{
	if (m_lstPrevTab.GetCount() == 0)
		return this;

	CXTPTabClientWnd* pPrev = (CXTPTabClientWnd*)m_lstPrevTab.GetHead();

	return pPrev->GetTabClientHead();
}

BOOL CXTPTabClientWnd::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwProcessId = 0;
	if (GetWindowThreadProcessId(hwnd, &dwProcessId) && dwProcessId == GetCurrentProcessId())
	{
		CArray<CWnd*, CWnd*>* arrWin = reinterpret_cast<CArray<CWnd*, CWnd*>*>(lParam);

		CWnd* pWnd = CWnd::FromHandle(hwnd);
		if (pWnd == AfxGetMainWnd())
			arrWin->Add(pWnd);

		CXTPMDIFrameWnd* pTarget = DYNAMIC_DOWNCAST(CXTPMDIFrameWnd, pWnd);
		if (pTarget)
			arrWin->Add(pWnd);
	}

	return TRUE;
}

CXTPTabClientWnd* CXTPTabClientWnd::FindTargetTabClient(POINT pt)
{
	// get list of tabs in point
	CArray<CXTPTabClientWnd*, CXTPTabClientWnd*> tabs;
	FindTargetTabClients(GetTabClientHead(), &tabs, pt);

	if (tabs.GetSize() == 0)
		return NULL;

	// get list of Z-oredr windows
	CArray<CWnd*, CWnd*> windows;
	EnumWindows(&EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));

	// attempt to found target window in point
	CWnd* targetWin = NULL;
	for (int i = 0; i < windows.GetSize(); i++)
	{
		// skip if TearOff is dragged
		CXTPTearOffFrame* pTearOff = DYNAMIC_DOWNCAST(CXTPTearOffFrame, windows.GetAt(i));
		if (pTearOff && pTearOff->IsFrameBeingDragged())
			continue;

		CXTPWindowRect rc(windows.GetAt(i));
		if (rc.PtInRect(pt))
		{
			targetWin = windows.GetAt(i);
			break;
		}
	}

	if (!targetWin)
		return NULL;

	// check target tab parent window and target window
	for (int t = 0; t < tabs.GetSize(); t++)
	{
		if (tabs.GetAt(t)->GetParent() == targetWin)
			return tabs.GetAt(t);
	}

	return NULL;
}

void CXTPTabClientWnd::FindTargetTabClients(CXTPTabClientWnd* pTabClient,
											CArray<CXTPTabClientWnd*, CXTPTabClientWnd*>* collection,
											POINT pt)
{
	ASSERT(pTabClient);

	POINT ptWorkspace(pt);
	pTabClient->ScreenToWorkspace(&ptWorkspace);
	if (pTabClient->GetWorkspaceCount() == 0)
	{
		CRect rc;
		pTabClient->GetClientRect(&rc);
		if (rc.PtInRect(ptWorkspace))
			collection->Add(pTabClient);
	}
	else
	{
		for (int i = 0; i < pTabClient->GetWorkspaceCount(); i++)
		{
			if (pTabClient->GetWorkspace(i)->GetHeaderRect().PtInRect(ptWorkspace))
				collection->Add(pTabClient);
			if (pTabClient->GetWorkspace(i)->GetClientRect().PtInRect(ptWorkspace))
				collection->Add(pTabClient);
		}
	}

	POSITION pos = pTabClient->m_lstNextTab.GetHeadPosition();
	while (pos != NULL)
	{
		CXTPTabClientWnd* pNext = (CXTPTabClientWnd*)pTabClient->m_lstNextTab.GetNext(pos);

		FindTargetTabClients(pNext, collection, pt);
	}
}

void CXTPTabClientWnd::GetTabClients(CArray<CXTPTabClientWnd*, CXTPTabClientWnd*>& collection)
{
	EnumTabClients(GetTabClientHead(), &collection);
}

void CXTPTabClientWnd::EnumTabClients(CXTPTabClientWnd* pTabClient,
									  CArray<CXTPTabClientWnd*, CXTPTabClientWnd*>* collection)
{
	collection->Add(pTabClient);

	POSITION pos = pTabClient->m_lstNextTab.GetHeadPosition();
	while (pos != NULL)
	{
		CXTPTabClientWnd* pNext = (CXTPTabClientWnd*)pTabClient->m_lstNextTab.GetNext(pos);

		EnumTabClients(pNext, collection);
	}
}

void CXTPTabClientWnd::ChangeMDIChildFrame(CWnd* pTarget)
{
	if (!pTarget)
		return;

	CMDIChildWnd* pMDIChildWnd = STATIC_DOWNCAST(CMDIChildWnd, MDIGetActive());
	CMDIFrameWnd* pMDIFrame	= STATIC_DOWNCAST(CMDIFrameWnd, pTarget);

	pMDIChildWnd->SetParent(CWnd::FromHandle(pMDIFrame->m_hWndMDIClient));

	CMDIChildWnd* pDummyChild = new CMDIChildWnd();
	if (!pDummyChild->Create(NULL, _T("Dummy"), WS_CHILD | WS_VISIBLE, CFrameWnd::rectDefault,
							 GetParentFrame(), NULL))
	{
		ASSERT(FALSE);
		delete pDummyChild;
		return;
	}

	::SendMessage(GetParentFrame()->m_hWndMDIClient, WM_MDIACTIVATE,
				  (WPARAM)pDummyChild->GetSafeHwnd(), 0);
	::SendMessage(pDummyChild->m_hWnd, WM_CLOSE, 0, 0);
}

void CXTPTabClientWnd::CloseTearOffFrame(CXTPTabClientWnd* pTarget)
{
	if (!pTarget)
		return;

	POSITION pos = pTarget->m_lstNextTab.GetHeadPosition();
	while (pos != NULL)
	{
		CXTPTabClientWnd* pNextTarget = (CXTPTabClientWnd*)pTarget->m_lstNextTab.GetNext(pos);
		XTP_SAFE_CALL1(pNextTarget, CloseTearOffFrame(pNextTarget));
	}

	CXTPTearOffFrame* pParentFrame = DYNAMIC_DOWNCAST(CXTPTearOffFrame, pTarget->GetParentFrame());
	XTP_SAFE_CALL1(pParentFrame, SendMessage(WM_CLOSE));
}

void CXTPTabClientWnd::ChangeItemWorkspace(CXTPTabClientWnd* pTarget, int nOldWorkspace,
										   int nSticker)
{
	if (!pTarget || nOldWorkspace == -1 || nSticker == xtpTabClientStickerNone
		|| nSticker == xtpTabClientStickerCenter)
		return;

	pTarget->UpdateContents();

	CXTPTabClientWnd::CWorkspace* pOldWorkspace = pTarget->GetWorkspace(nOldWorkspace);

	CXTPTabManagerItem* pItem = pTarget->GetSelectedItem();

	pItem->InternalAddRef();
	pItem->Remove();

	COLORREF clr = pItem->m_clrItem;

	int nIndex = (nSticker == xtpTabClientStickerLeft || nSticker == xtpTabClientStickerTop)
					 ? nOldWorkspace
					 : nOldWorkspace + 1;

	CXTPTabClientWnd::CWorkspace* pNewWorkspace = pTarget->AddWorkspace(nIndex);
	ASSERT(pNewWorkspace);

	pNewWorkspace->AddNewWindowItem();

	int nInsert = pNewWorkspace->GetItemCount() + (pNewWorkspace->m_pNewWindowItem ? -1 : 0);
	pNewWorkspace->CXTPTabManager::AddItem(nInsert, pItem);

	pItem->m_clrItem = clr;
	pNewWorkspace->SetSelectedItem(pItem);

	pNewWorkspace->m_dHeight = (pOldWorkspace->m_dHeight - pTarget->m_nSplitterSize) / 2;
	pOldWorkspace->m_dHeight = pOldWorkspace->m_dHeight - pNewWorkspace->m_dHeight
							   - pTarget->m_nSplitterSize;

	pTarget->m_bHorizSplitting = (nSticker == xtpTabClientStickerTop
								  || nSticker == xtpTabClientStickerBottom)
									 ? TRUE
									 : FALSE;
}

HWND CXTPTabClientWnd::GetLastSelecedItem()
{
	CWnd* pWnd = CWnd::FromHandle(::GetForegroundWindow());

	CArray<CXTPTabClientWnd*, CXTPTabClientWnd*> tabClients;
	GetTabClients(tabClients);

	for (int i = 0; i < tabClients.GetSize(); i++)
	{
		if (pWnd == tabClients.GetAt(i)->GetParent())
		{
			return XTP_SAFE_GET2(tabClients.GetAt(i), GetSelectedItem(), GetHandle(), NULL);
		}
	}

	return NULL;
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
CXTPDockingPaneTabbedContainer* CXTPTabClientWnd::FindTargetDockPane(POINT pt)
{
	CXTPDockingPaneManager* pDockingPaneManager = GetDockingPaneManager();
	if (pDockingPaneManager)
	{
		ASSERT(pDockingPaneManager->GetSite() == GetParentFrame());

		CXTPDockingPaneBaseList& list = pDockingPaneManager->GetPaneStack();
		{
			POSITION p = list.GetHeadPosition();
			while (p)
			{
				CXTPDockingPaneBase* pPane = list.GetNext(p);
				if (pPane && pPane->GetDockingSite()->GetSafeHwnd()
					&& pPane->GetType() == xtpPaneTypeTabbedContainer
					&& pPane->GetPaneWindowRect().PtInRect(pt))
				{
					CXTPDockingPaneTabbedContainer* pPaneTabbedContainer =
						(CXTPDockingPaneTabbedContainer*)pPane;
					if (pPaneTabbedContainer && ::IsWindow(pPaneTabbedContainer->GetSafeHwnd()))
						return pPaneTabbedContainer;
				}
			}
		}
	}

	return NULL;
}
#	endif
#endif

void CXTPTabClientWnd::SetShowTabClientContextStickers(BOOL value)
{
	ASSERT(m_pTabClientContext);

	m_pTabClientContext->SetShowTabClientContextStickers(value);
}

void CXTPTabClientWnd::SetAlphaTabClientContext(BOOL value)
{
	ASSERT(m_pTabClientContext);

	m_pTabClientContext->SetAlphaTabClientContext(value);
}

void CXTPTabClientWnd::SetShowContentsWhileDragging(BOOL value)
{
	ASSERT(m_pTabClientContext);

	m_pTabClientContext->SetShowContentsWhileDragging(value);
}

void CXTPTabClientWnd::EnableFloatingFrameTheme(BOOL value)
{
	ASSERT(m_pTabClientContext);

	m_pTabClientContext->EnableFloatingFrameTheme(value);
}

BOOL CXTPTabClientWnd::GetAlphaTabClientContext() const
{
	ASSERT(m_pTabClientContext);

	return m_pTabClientContext->GetAlphaTabClientContext();
}

BOOL CXTPTabClientWnd::GetShowTabClientContextStickers() const
{
	ASSERT(m_pTabClientContext);

	return m_pTabClientContext->GetShowTabClientContextStickers();
}

BOOL CXTPTabClientWnd::GetShowContentsWhileDragging() const
{
	ASSERT(m_pTabClientContext);

	return m_pTabClientContext->GetShowContentsWhileDragging();
}

BOOL CXTPTabClientWnd::IsFloatingFrameThemeEnabled() const
{
	ASSERT(m_pTabClientContext);

	return m_pTabClientContext->IsFloatingFrameThemeEnabled();
}

void CXTPTabClientWnd::SetForegroundTearOffFrames()
{
	if (!IsTearOffEnabled())
		return;

	if (this != GetTabClientHead())
		return;

	CArray<CXTPTabClientWnd*, CXTPTabClientWnd*> tabClients;
	GetTabClients(tabClients);

	ASSERT(tabClients.GetSize());

	for (int i = 0; i < tabClients.GetSize(); i++)
	{
		if (this != tabClients.GetAt(i))
		{
			CXTPTearOffFrame* pTearOffFrame =
				DYNAMIC_DOWNCAST(CXTPTearOffFrame, tabClients.GetAt(i)->GetParentFrame());
			XTP_SAFE_CALL1(pTearOffFrame, SetForegroundWindow());
		}
	}

	XTP_SAFE_CALL1(tabClients.GetAt(0)->GetParentFrame(), SetForegroundWindow());
}

void CXTPTabClientWnd::CloseAllTearOffFrames()
{
	if (!IsTearOffEnabled())
		return;

	if (this != GetTabClientHead())
		return;

	CArray<CXTPTabClientWnd*, CXTPTabClientWnd*> tabClients;
	GetTabClients(tabClients);

	ASSERT(tabClients.GetSize());

	for (int i = 0; i < tabClients.GetSize(); i++)
	{
		if (this != tabClients.GetAt(i))
		{
			CXTPTearOffFrame* pTearOffFrame =
				DYNAMIC_DOWNCAST(CXTPTearOffFrame, tabClients.GetAt(i)->GetParentFrame());
			XTP_SAFE_CALL1(pTearOffFrame, PostMessage(WM_CLOSE));
		}
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTabClientWnd, CWnd)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "Flags", dispidFlags, GetFlags, SetFlags, VT_I4)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "PaintManager", 2, OleGetPaintManager, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "AllowReorder", 3, IsAllowReorder, SetAllowReorder,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "AutoTheme", 4, GetAutoTheme, SetAutoTheme, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "EnableGroups", 5, OleEnableGroups, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "Refresh", 6, OleRefresh, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPTabClientWnd, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "ItemCount", 7, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "HitTest", 9, OleHitTest, VT_DISPATCH,
					 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "Selected", 10, OleGetSelected, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPTabClientWnd, "DoWorkspaceCommand", 11, OnWorkspaceCommand, VT_EMPTY,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "IsWorkspaceCommandEnabled", 12,
					 OleIsWorkspaceCommandEnabled, VT_BOOL, VTS_I4)

	DISP_PROPERTY_ID(CXTPTabClientWnd, "ThemedBackColor", 13, m_bThemedBackColor, VT_BOOL)

	DISP_FUNCTION_ID(CXTPTabClientWnd, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "ToolTipContext", 14, OleGetToolTipContext,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPTabClientWnd, "UseSplitterTracker", 15, m_bUseSplitterTracker, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "HorizGroupSplitting", 19, OleGetHorizGroupSplitting,
						OleSetHorizGroupSplitting, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "GroupCount", 20, OlegGetGroupCount, SetNotSupported,
						VT_I4)

	DISP_FUNCTION_ID(CXTPTabClientWnd, "FindItem", 21, OleFindItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "FindGroup", 22, OleFindGroup, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "CreateGroup", 23, OleCreateGroup, VT_EMPTY,
					 VTS_DISPATCH VTS_BOOL)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "MoveToGroup", 24, OleMoveToGroup, VT_EMPTY,
					 VTS_DISPATCH VTS_I4)

	DISP_PROPERTY_ID(CXTPTabClientWnd, "NewTabPosition", 25, m_newTabPosition, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "RightToLeft", 26, IsLayoutRTL, SetLayoutRTL, VT_BOOL)
	DISP_PROPERTY_PARAM_ID(CXTPTabClientWnd, "GroupSplitter", 27, OleGetGroupSplitter,
						   OleSetGroupSplitter, VT_R8, VTS_I4)
	DISP_PROPERTY_ID(CXTPTabClientWnd, "IgnoreFlickersOnActivate", 28, m_bIgnoreFlickersOnActivate,
					 VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "hWnd", DISPID_HWND, OleGetHwnd, SetNotSupported,
						VT_HANDLE)

	DISP_PROPERTY_ID(CXTPTabClientWnd, "AutoUpdateTabCaption", 29, m_bAutoUpdateTabCaption, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabClientWnd, "EnableMarkup", 30, OleGetEnableMarkup,
						OleSetEnableMarkup, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTabClientWnd, "ShowNewWindowTab", 31, ShowNewWindowTab, VT_EMPTY,
					 VTS_BOOL VTS_I4)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPTabClientWnd, CWnd)
	INTERFACE_PART(CXTPTabClientWnd, XTPDIID_ITabWorkspace, Dispatch)
	// INTERFACE_PART(CXTPTabClientWnd, IID_IEnumVARIANT, EnumVARIANT)
	INTERFACE_PART(CXTPTabClientWnd, IID_IConnectionPointContainer, ConnPtContainer)
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE_PART(CXTPTabClientWnd)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTabClientWnd, XTPDIID_ITabWorkspace)
XTP_IMPLEMENT_CMDTARGETPROVIDER_INTERFACE(CXTPTabClientWnd)

IMPLEMENT_CONNECTION_EX(CXTPTabClientWnd, XTPDIID_ITabWorkspaceEvents)

IMPLEMENT_ENUM_VARIANT(CXTPTabClientWnd)

OLE_HANDLE CXTPTabClientWnd::OleGetHwnd()
{
	return (OLE_HANDLE) reinterpret_cast<LONG_PTR>(m_hWnd);
}

LPDISPATCH CXTPTabClientWnd::OleGetPaintManager()
{
	CXTPTabPaintManager* pPaintManager = GetPaintManager();
	pPaintManager->AddObserver(this);

	return pPaintManager->GetIDispatch(TRUE);
}

LPDISPATCH CXTPTabClientWnd::OleFindItem(OLE_HANDLE hWnd)
{
	CXTPTabManagerItem* pItem = FindItem(reinterpret_cast<HWND>((LONG_PTR)hWnd));
	return pItem ? pItem->GetIDispatch(TRUE) : 0;
}

int CXTPTabClientWnd::OleFindGroup(LPDISPATCH Item)
{
	CXTPTabManagerItem* pItem = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPTabManagerItem,
															 CXTPTabManagerItem::FromIDispatchSafe(
																 Item));
	if (!pItem)
	{
		return -1;
	}

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		if (GetWorkspace(i) == (CWorkspace*)pItem->GetTabManager())
		{
			return i;
		}
	}

	return -1;
}

void CXTPTabClientWnd::OleCreateGroup(LPDISPATCH Item, BOOL Horiz)
{
	CXTPTabManagerItem* pItem = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPTabManagerItem,
															 CXTPTabManagerItem::FromIDispatchSafe(
																 Item));
	if (pItem)
	{
		DoWorkspaceCommand(pItem, GetWorkspace(GetWorkspaceCount() - 1),
						   Horiz ? XTP_TABCLIENT_ACTION_ATTACH_RIGHT
								 : XTP_TABCLIENT_ACTION_ATTACH_BOTTOM);
	}
}

void CXTPTabClientWnd::OleMoveToGroup(LPDISPATCH Item, int nGroup)
{
	CXTPTabManagerItem* pItem = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPTabManagerItem,
															 CXTPTabManagerItem::FromIDispatchSafe(
																 Item));
	if (!pItem)
		return;

	if (nGroup < 0 || nGroup >= GetWorkspaceCount())
		return;

	CWorkspace* pDest = GetWorkspace(nGroup);
	if (!pDest)
		return;

	DoWorkspaceCommand(pItem, pDest, XTP_TABCLIENT_ACTION_ATTACH_CENTER);
}

double CXTPTabClientWnd::OleGetGroupSplitter(int nGroup)
{
	CWorkspace* pDest = GetWorkspace(nGroup);
	if (!pDest)
		return 0;

	return pDest->m_dHeight;
}

void CXTPTabClientWnd::OleSetGroupSplitter(int nGroup, double dSplitterPos)
{
	CWorkspace* pDest = GetWorkspace(nGroup);
	if (!pDest)
		return;

	pDest->m_dHeight = dSplitterPos;
	Reposition();
}

void CXTPTabClientWnd::OleEnableGroups()
{
	if (!m_bEnableGroups && m_bShowWorkspace && GetWorkspaceCount() == 0)
	{
		m_bEnableGroups = TRUE;
	}
	SendMessage(m_nMsgUpdateSkinState);
	Refresh();
}

LPDISPATCH CXTPTabClientWnd::OleGetSelected()
{
	CXTPTabManagerItem* pItem = GetSelectedItem();

	return pItem ? pItem->GetIDispatch(TRUE) : FALSE;
}

LPDISPATCH CXTPTabClientWnd::OleGetItem(int nItem)
{
	CXTPTabManagerItem* pItem = GetItem(nItem);

	return pItem ? pItem->GetIDispatch(TRUE) : FALSE;
}

long CXTPTabClientWnd::OleGetItemCount()
{
	return GetItemCount();
}
void CXTPTabClientWnd::OleRefresh()
{
	Refresh(FALSE);
}

LPDISPATCH CXTPTabClientWnd::OleGetToolTipContext()
{
	return GetToolTipContext()->GetIDispatch(TRUE);
}

void OleUnitsToPixel(CWnd* pSite, CPoint& pt);

LPDISPATCH CXTPTabClientWnd::OleHitTest(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
{
	CPoint pt(CPoint(x, y));

	if (IsAttached())
		OleUnitsToPixel(GetParentFrame(), pt);

	CXTPTabManagerItem* pItem = HitTest(pt);
	return pItem ? pItem->GetIDispatch(TRUE) : NULL;
}

BOOL CXTPTabClientWnd::OleGetHorizGroupSplitting()
{
	return m_bHorizSplitting;
}

void CXTPTabClientWnd::OleSetHorizGroupSplitting(BOOL bHorizSplitting)
{
	m_bHorizSplitting = bHorizSplitting;
}

long CXTPTabClientWnd::OlegGetGroupCount()
{
	return GetWorkspaceCount();
}

BOOL CXTPTabClientWnd::OleIsWorkspaceCommandEnabled(long nID)
{
	return IsWorkspaceCommandEnabled(nID);
}

void CXTPTabClientWnd::OleSetEnableMarkup(BOOL bEnableMarkup)
{
	EnableMarkup(bEnableMarkup);
}

BOOL CXTPTabClientWnd::OleGetEnableMarkup()
{
	return m_pMarkupContext != NULL;
}

#endif
