// XTPTabClientWnd.h : interface for the CXTPTabClientWnd class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__TABCLIENTWNDEX_H__) && defined(_XTP_INCLUDE_TABMANAGER)
#	define __TABCLIENTWNDEX_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupContext;
class CXTPTearOffFrame;
class CXTPTabClientContext;
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
class CXTPDockingPaneManager;
class CXTPDockingPaneTabbedContainer;
#		endif
#	endif

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_PRETRANSLATEMOUSEMSG message is sent to the owner window to
//     pre-translate mouse messages.
// Parameters:
//     point  - CPoint((DWORD)lParam) - mouse cursor position.
//     wParam - mouse notification.
// Returns:
//     If the application is to process this message, the return value
//     should be TRUE.
// Example:
//     Here is an example of how an application would process the WM_XTP_PRETRANSLATEMOUSEMSG
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam, LPARAM lParam)
// {
//     CPoint point = CPoint((DWORD)lParam);
//
//     CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);
//
//     if (pItem)
//     {
//          if (wParam == WM_RBUTTONDOWN)
//          {
//
//              CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
//              MDIActivate(pFrame);
//
//              m_MTIClientWnd.Refresh();
//
//              CMenu menuPopup;
//              VERIFY(menuPopup.LoadMenu(IDR_WORKSPACE_POPUP));
//
//              m_MTIClientWnd.WorkspaceToScreen(&point);
//              CXTPCommandBars::TrackPopupMenu(menuPopup.GetSubMenu(0), 0, point.x, point.y, this);
//
//              m_MTIClientWnd.Refresh();
//
//              return TRUE;
//          }
//      }
//
//      return FALSE;
// }
// </code>
// See Also:
//     CXTPTabClientWnd
//-----------------------------------------------------------------------
const UINT WM_XTP_PRETRANSLATEMOUSEMSG = (WM_XTP_COMMANDBARS_BASE + 20);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_GETWINDOWTEXT message is sent to the MDI child window to
//     retrieve the text that will be used for the tabbed interface.
// Remarks:
//     If application returns 0, CDocument::GetTitle will be used. You can override
//     CXTPTabClientWnd::GetItemText instead of process the message.
// Returns:
//     Pointer to a null-terminated string that specifies the text to be used.
// Example:
//     Here is an example of how an application would process the WM_XTP_GETWINDOWTEXT
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//     //{{AFX_MSG_MAP(CChildFrame)
//     ON_MESSAGE(WM_XTP_GETWINDOWTEXT, OnGetTabText)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CChildFrame::OnGetTabText(WPARAM /*wParam*/, LPARAM /*lParam*/)
// {
//     return (LPCTSTR)m_strCaption;
// }
// </code>
// See Also:
//     CXTPTabClientWnd, WM_XTP_GETTABICON, WM_XTP_GETWINDOWTOOLTIP
//-----------------------------------------------------------------------
const UINT WM_XTP_GETWINDOWTEXT = (WM_XTP_COMMANDBARS_BASE + 21);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_GETWINDOWTOOLTIP message is sent to the MDI child window to
//     retrieve tooltips for CXTPTabClientWnd's tabs.
// Remarks:
//     You must call CXTPTabClientWnd::EnableToolTips to enable tooltips.
// Returns:
//     Pointer to a null-terminated string that specifies the tooltip to be used. <p/>
//     If the application returns 0, CDocument::GetPathName will be used.
// Example:
//     Here is an example of how an application would process the WM_XTP_GETWINDOWTOOLTIP
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//     //{{AFX_MSG_MAP(CChildFrame)
//     ON_MESSAGE(WM_XTP_GETWINDOWTOOLTIP, OnGetTabTip)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CChildFrame::OnGetTabTip(WPARAM /*wParam*/, LPARAM /*lParam*/)
// {
//     return (LPCTSTR)m_strToolTip;
// }
// </code>
// See Also:
//      CXTPTabClientWnd, WM_XTP_GETTABICON, WM_XTP_GETWINDOWTEXT
//-----------------------------------------------------------------------
const UINT WM_XTP_GETWINDOWTOOLTIP = (WM_XTP_COMMANDBARS_BASE + 23);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_NEWTABITEM message is sent to the owner window when a new tab is created.
// Parameters:
//     pNewItem - (CXTPTabManagerItem*)wParam. Pointer to the tab that was created.
// Example:
//     Here is an example of how an application would process the WM_XTP_NEWTABITEM
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_NEWTABITEM, OnTabbarNewItem)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTabbarNewItem(WPARAM wParam, LPARAM /*lParam*/)
// {
//     CXTPTabManagerItem* pItem = (CXTPTabManagerItem*)wParam;
//     return 0;
// }
// </code>
// See Also:
//     CXTPTabClientWnd
//-----------------------------------------------------------------------
const UINT WM_XTP_NEWTABITEM = (WM_XTP_COMMANDBARS_BASE + 24);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_TABTODOCKPANE message is sent to the owner window when a tab
//     is being docked to Docking Panes.
// Parameters:
//     pNewItem - (CXTPTabManagerItem*)wParam. Pointer to the tab that is being docking.
// Returns:
//     A new docking pane ID to be used for a new pane to be created.
// Example:
//     Here is an example of how an application would process the WM_XTP_NEWTABITEM
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_TABTODOCKPANE, OnTabbarDockPane)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTabbarDockPane(WPARAM wParam, LPARAM /*lParam*/)
// {
//     CXTPTabManagerItem* pItem = (CXTPTabManagerItem*)wParam;
//     return 123;
// }
// </code>
// See Also:
//     CXTPTabClientWnd
//-----------------------------------------------------------------------
const UINT WM_XTP_TABTODOCKPANE = (WM_XTP_COMMANDBARS_BASE + 26);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_PRETRANSLATEMOUSEMSG_EX message is sent to the main window
//     from TearOffFrame to pre-translate mouse messages.
// Parameters:
//     wParam - mouse notification.
//     lParam - pointer to TTHITTESTINFO struct.
// Returns:
//     If the application is to process this message, the return value
//     should be TRUE.
// Example:
//     Here is an example of how an application would process the WM_XTP_PRETRANSLATEMOUSEMSG_EX
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG_EX, OnTabbarMouseMsgEx)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTabbarMouseMsgEx(WPARAM wParam, LPARAM lParam)
// {
//     if (wParam != WM_RBUTTONDOWN)
//       return FALSE;
//
//     TTHITTESTINFO* info = reinterpret_cast<TTHITTESTINFO*>(lParam);
//
//     CWnd* pWnd = CWnd::FromHandle(info->hwnd);
//     CXTPTabClientWnd* pTabClientWnd = DYNAMIC_DOWNCAST(CXTPTabClientWnd, pWnd);
//     CXTPTabManagerItem* pItem = pTabClientWnd->HitTest(info->pt);
//     if (!pItem)
//       return FALSE;
//
//     CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIChildWnd, CWnd::FromHandle(pItem->GetHandle()));
//     if (!pFrame)
//       return FALSE;
//
//     MDIActivate(pFrame);
//     pFrame->GetActiveView()->SetFocus();
//
//     pTabClientWnd->Refresh();
//
//     CMenu menu;
//     VERIFY(XTPResourceManager()->LoadMenu(&menu, IDR_POPUP_TABS_EX));
//     CMenu* pPopup = menu.GetSubMenu(0);
//
//     pTabClientWnd->WorkspaceToScreen(&info->pt);
//
//     CXTPPopupBar* pPopupBar = (CXTPPopupBar*)CXTPPopupBar::CreatePopupBar(GetCommandBars());
//     VERIFY(pPopupBar->LoadMenu(pPopup));
//     VERIFY(pPopupBar->Popup(info->pt.x, info->pt.y, NULL));
//
//     while (pPopupBar->IsTrackingMode())
//     {
//     	 AfxGetApp()->PumpMessage();
//     }
//     pPopupBar->InternalRelease();
//
//     pTabClientWnd->Refresh();
//     return TRUE;
// }
// </code>
// See Also:
//     CXTPTabClientWnd
//-----------------------------------------------------------------------
const UINT WM_XTP_PRETRANSLATEMOUSEMSG_EX = (WM_XTP_COMMANDBARS_BASE + 28);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_SETACTIVEWORKSPACE message is sent to the owner window when
//     workspace changed.
// Parameters:
//     wParam - Active workspace.
// Returns:
//     If the application is to process this message, the return value
//     should be TRUE.
const UINT WM_XTP_SETACTIVEWORKSPACE = (WM_XTP_COMMANDBARS_BASE + 29);

//-----------------------------------------------------------------------
// Summary:
//     Workspace buttons enumerator. Used to indicate which tab navigation
//     buttons to display in the TabWorkspace.
// Example:
//     <code>m_MTIClientWnd.SetFlags(xtpWorkspaceHideClose);</code>
// See Also: CXTPTabClientWnd::SetFlags
//
// <KEYWORDS xtpWorkspaceHideArrows, xtpWorkspaceHideClose, xtpWorkspaceHideAll>
//-----------------------------------------------------------------------
enum XTPWorkspaceButtons
{
	xtpWorkspaceHideArrows			 = 1,	 // To hide arrow buttons.
	xtpWorkspaceHideClose			 = 2,	 // To hide close button.
	xtpWorkspaceHideAll				 = 3,	 // To hide arrow and close buttons.
	xtpWorkspaceShowActiveFiles		 = 4,	 // To show active file button.
	xtpWorkspaceHideArrowsAlways	 = 1 + 8, // To hide arrow buttons always
	xtpWorkspaceShowCloseTab		 = 16,	// To show close button for all tabs.
	xtpWorkspaceShowCloseSelectedTab = 32,	// To show close button for selected tabs.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPWorkspaceNewTabPosition enumerator. Used to determine where new tabs will appear.
// Example:
//     <code>m_MTIClientWnd.SetNewTabPosition(xtpWorkspaceNewTabLeftMost);</code>
// See Also:
//     CXTPTabClientWnd::SetNewTabPosition
//
// <KEYWORDS xtpWorkspaceNewTabRightMost, xtpWorkspaceNewTabLeftMost,
// xtpWorkspaceNewTabNextToActive>
//-----------------------------------------------------------------------
enum XTPWorkspaceNewTabPosition
{
	xtpWorkspaceNewTabRightMost,   // New tab will be created after all tabs.
	xtpWorkspaceNewTabLeftMost,	// New tab will be created before all tabs.
	xtpWorkspaceNewTabNextToActive // New tab will be created after active tab.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPWorkspaceActivateTab enumerator. Used to determine which tab will
//     become active after the currently active tab is closed.
// See Also:
//     CXTPTabClientWnd.SetAfterCloseActiveTab
//
// <KEYWORDS xtpWorkspaceActivateNextToClosed, xtpWorkspaceActivateTopmost>
//-----------------------------------------------------------------------
enum XTPWorkspaceActivateTab
{
	xtpWorkspaceActivateNextToClosed, // Activate tab next to the closed tab.
	xtpWorkspaceActivateTopmost		  // Activate the left-most tab.
};

//{{AFX_CODEJOCK_PRIVATE
class CXTPTabPaintManager;
class CXTPCommandBars;
class CXTPMDIFrameWnd;
class CXTPToolTipContext;
//}}AFX_CODEJOCK_PRIVATE

#	define XTP_TABCLIENT_ACTION_CANCEL 0
#	define XTP_TABCLIENT_ACTION_POPUP 1
#	define XTP_TABCLIENT_ACTION_ATTACH_CENTER 2
#	define XTP_TABCLIENT_ACTION_ATTACH_LEFT 3
#	define XTP_TABCLIENT_ACTION_ATTACH_RIGHT 4
#	define XTP_TABCLIENT_ACTION_ATTACH_TOP 5
#	define XTP_TABCLIENT_ACTION_ATTACH_BOTTOM 6
#	define XTP_TABCLIENT_ACTION_DETACH 7
#	define XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER 8
#	define XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_LEFT 9
#	define XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_RIGHT 10
#	define XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_TOP 11
#	define XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_BOTTOM 12
#	define XTP_TABCLIENT_NO_ACTION 14
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
#			define XTP_TABCLIENT_ACTION_DOCK_CENTER 15
#			define XTP_TABCLIENT_ACTION_DOCK_LEFT 16
#			define XTP_TABCLIENT_ACTION_DOCK_RIGHT 17
#			define XTP_TABCLIENT_ACTION_DOCK_TOP 18
#			define XTP_TABCLIENT_ACTION_DOCK_BOTTOM 19
#		endif
#	endif

//===========================================================================
// Summary:
//     CXTPTabClientWnd is a CWnd derived class. It represents a tabbed
//     workspace.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabClientWnd
	: public CWnd
	, public CXTPTabManagerAtom
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPTabClientWnd)

	class CTabClientDropTarget;
	class CNavigateButtonActiveFiles;
	//}}AFX_CODEJOCK_PRIVATE

public:
	class CWorkspace;
	class CSingleWorkspace;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabClientWnd object.
	//-----------------------------------------------------------------------
	CXTPTabClientWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabClientWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabClientWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Attaches the tabbed workspace.
	// Parameters:
	//     pParentFrame - Pointer to a CMDIFrameWnd object.
	//     bEnableGroups - TRUE to enable groups.
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Attach(CXTPMDIFrameWnd* pParentFrame, BOOL bEnableGroups = FALSE);
	BOOL Attach(CXTPCommandBars* pCommandBars, CMDIFrameWnd* pParentFrame,
				BOOL bEnableGroups); // <CXTPTabClientWnd::Attach@CXTPMDIFrameWnd*@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Detaches the tabbed workspace.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Detach();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a tab workspace is attached.
	// Returns:
	//     TRUE if attached, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsAttached() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the parent frame of the control.
	// Returns:
	//     Pointer to a CMDIFrameWnd object.
	//-----------------------------------------------------------------------
	CMDIFrameWnd* GetParentFrame() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if right-to-left (RTL) mode is set.
	// Returns:
	//     TRUE if text is displayed using right-to-left (RTL)
	//     reading-order properties.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if right-to-left (RTL) mode should be set.
	// Parameters:
	//     bRightToLeft - TRUE to display text using right-to-left (RTL)
	//                    reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set where new tab must appear.
	// Parameters:
	//     tabPosition - Position of new tab regarding other tabs.
	//                   It can be one of the following:
	//                      * <b>xtpWorkspaceNewTabRightMost</b>:  New tab will be created after all
	//                                                             tabs.
	//                      * <b>xtpWorkspaceNewTabLeftMost</b>:  New tab will be created before all
	//                                                            tabs.
	//                      * <b>xtpWorkspaceNewTabNextToActive</b>:  New tab will be created after
	//                                                                active tab.
	//     activeTab - Enumerator that determines which tab will be activated after closing another
	//                 one.
	//                 It can be one of the following:
	//                      * <b>xtpWorkspaceActivateNextToClosed</b>:  Activate tab next to the
	//                                                                  closed tab.
	//                      * <b>xtpWorkspaceActivateTopmost</b>:  Activate the top-most tab.
	// See Also:
	//     XTPWorkspaceNewTabPosition, GetNewTabPositon, XTPWorkspaceActivateTab
	//-----------------------------------------------------------------------
	void SetNewTabPosition(XTPWorkspaceNewTabPosition tabPosition);
	void SetAfterCloseActiveTab(
		XTPWorkspaceActivateTab
			activeTab); // <combine CXTPTabClientWnd::SetNewTabPosition@XTPWorkspaceNewTabPosition>

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine where the new tab must appear.
	// Returns:
	//     XTPWorkspaceNewTabPosition enumerator that shows where the new tab must appear.
	// See Also:
	//     XTPWorkspaceNewTabPosition, SetNewTabPosition
	//-----------------------------------------------------------------------
	XTPWorkspaceNewTabPosition GetNewTabPositon() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves Position state.
	//-----------------------------------------------------------------------
	void SaveState();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads previous docking state.
	//-----------------------------------------------------------------------
	void LoadState();

	//-----------------------------------------------------------------------
	// Summary:
	//     Scan through all MDIChild windows and update the corresponding
	//     tab items if any changes happened (e.g. window text or active MDIChild).
	//-----------------------------------------------------------------------
	virtual void UpdateContents();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create a new CWorkspace class.
	//     You can override it for a custom Workskspace.
	//-----------------------------------------------------------------------
	virtual CWorkspace* CreateWorkspace();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the flags of the tab workspace.
	// Parameters:
	//     dwButtons - Buttons to hide. Can be any of the values listed in the
	//                 remarks section.
	// Remarks:
	//     dwButtons can be one of the following:
	//     * <b>xtpWorkspaceHideArrows</b>:  Hides the arrow buttons.
	//     * <b>xtpWorkspaceHideClose</b>:  Hides the close button.
	//     * <b>xtpWorkspaceHideAll</b>:  Hides the arrow and close buttons.
	// See also:
	//     GetFlags, XTPWorkspaceButtons
	//-----------------------------------------------------------------------
	void SetFlags(DWORD dwButtons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine which tab navigation buttons are
	//     currently displayed in the tab workspace.
	// Returns:
	//     Retrieves the tab navigation flags of the tab workspace.
	// See Also:
	//     SetFlags, XTPWorkspaceButtons
	//-----------------------------------------------------------------------
	DWORD GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable/disable tooltips show.
	// Parameters:
	//     behaviour - Tooltips behaviour to be set. See remarks section for available flags.
	// Remarks:
	//     <i>behaviour</i> parameter can be one of the following:
	//     * <b>xtpTabToolTipNever</b>:  Show tooltips for tabs always.
	//     * <b>xtpTabToolTipAlways</b>:  Doesn't show tooltips for tabs.
	//     * <b> xtpTabToolTipShrinkedOnly</b>:  Show tooltips only if tab was shrinked
	//                                           (see xtpTabLayoutSizeToFit layout).
	//-----------------------------------------------------------------------
	void EnableToolTips(XTPTabToolTipBehaviour behaviour = xtpTabToolTipAlways);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the paint manager.
	// Returns:
	//     A pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified paint manager.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPTabPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether auto themes are used for the
	//     TabWorkspace.
	// Parameters:
	//     bAutoTheme - TRUE to use auto themes.
	// Remarks:
	//     By default, AutoTheme = TRUE. When AutoTheme = TRUE, the theme
	//     of the TabWorkspace will change when the theme of the CommandBars
	//     control is changed. Set AutoTheme = FALSE to keep the currently
	//     set theme even if the CommandBars theme is changed.
	//-----------------------------------------------------------------------
	void SetAutoTheme(BOOL bAutoTheme = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if auto themes are used to
	//     theme the TabWorkspace.
	// Returns:
	//     TRUE if auto themes are used to theme the TabWorkspace, FALSE
	//     if auto themes are not used.
	//-----------------------------------------------------------------------
	BOOL GetAutoTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the item that corresponds to the
	//     specified window.
	// Parameters:
	//     hWnd - Handle of the window.
	// Returns:
	//     A pointer to a CXTPTabManagerItem object.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* FindItem(const HWND hWnd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine which tab, if any, is at the
	//     specified screen position.
	// Parameters:
	//     pt - Point to be tested.
	// Returns:
	//     The zero-based index of the tab, or returns -1 if no tab is
	//     at the specified position.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* HitTest(CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the current number of workspaces.
	// Returns:
	//     An integer value that represents the current number of workspaces.
	// Remarks:
	//     If tab groups are enabled, then there will be more than
	//     one workspace. If tab groups are disabled, then there is only 1
	//     workspace with an index of 0.
	// See Also:
	//     GetWorkspace
	//-----------------------------------------------------------------------
	int GetWorkspaceCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the workspace at the specified index.
	// Parameters:
	//     nIndex - An integer index starting at 0.
	// Returns:
	//     A pointer to the workspace currently at this index.
	// Remarks:
	//     If tab groups are enabled, then there will be more than
	//     one workspace. If tab groups are disabled, then there is only 1
	//     workspace with an index of 0.
	// See Also:
	//     GetWorkspaceCount
	//-----------------------------------------------------------------------
	CWorkspace* GetWorkspace(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the workspace command mode.
	// Parameters:
	//     nID    - Specifies the identifier of the workspace.
	//     ppItem - MDI active item.
	// Remarks:
	//     Call this member function if you want to determine whether or
	//     not workspace command is enabled.
	// Returns:
	//     TRUE when workspace command is enabled, FALSE when it is disabled.
	//-----------------------------------------------------------------------
	BOOL IsWorkspaceCommandEnabled(UINT nID, CXTPTabManagerItem** ppItem = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to execute special workspace commands.
	// Parameters:
	//     nID - Specifies the identifier of the workspace. Can be any of
	//           the values listed in the Remarks section.
	// Remarks:
	//     nID can be one of the following:
	//     * <b>XTP_ID_WORKSPACE_NEWVERTICAL</b>:  Creates new vertical group.
	//     * <b>XTP_ID_WORKSPACE_NEWHORIZONTAL</b>:  Creates new horizontal group.
	//     * <b>XTP_ID_WORKSPACE_MOVEPREVIOUS</b>:  Moves item to previous group.
	//     * <b>XTP_ID_WORKSPACE_MOVENEXT</b>:  Moves item to next group.
	//-----------------------------------------------------------------------
	void OnWorkspaceCommand(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to refresh all the items.
	// Parameters:
	//     bRecalcLayout - TRUE to recalculate the layout of the frame.
	//-----------------------------------------------------------------------
	void Refresh(BOOL bRecalcLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the workspace coordinates of a given point on the
	//     display to screen coordinates.
	// Parameters:
	//     lpPoint - Pointer to a POINT structure or CPoint object that
	//               contains the client coordinates to be converted.
	//-----------------------------------------------------------------------
	void WorkspaceToScreen(LPPOINT lpPoint) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the screen coordinates of a given point or rectangle on the
	//     display to client coordinates.
	// Parameters:
	//     lpPoint - Pointer to a CPoint object or POINT structure that
	//               contains the screen coordinates to be converted.
	//-----------------------------------------------------------------------
	void ScreenToWorkspace(LPPOINT lpPoint) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to show/hide the workspace.
	// Parameters:
	//     bShow - TRUE to show the workspace, FALSE to hide it.
	//-----------------------------------------------------------------------
	void ShowWorkspace(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the custom workspace control.
	// Parameters:
	//     pWorkspace - Custom workspace control.
	//-----------------------------------------------------------------------
	void SetTabWorkspace(CWorkspace* pWorkspace);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current reordering allowance.
	// Returns:
	//     TRUE when reordering is allowed, FALSE when it is disallowed.
	//-----------------------------------------------------------------------
	BOOL IsAllowReorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to allow or disallow reordering.
	// Parameters:
	//     bAllowReorder - TRUE for allowing reordering,
	//                     FALSE for disallowing reordering.
	//-----------------------------------------------------------------------
	void SetAllowReorder(BOOL bAllowReorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the background is filled.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Rectangle area to be filled.
	//-----------------------------------------------------------------------
	virtual void OnFillBackground(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the client is drawn.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Rectangle area to be drawn.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to use trackers.
	// Parameters:
	//     bSplitterTracker - TRUE to use trackers.
	// Remarks:
	//     This member function will display the contents for child windows
	//     while the splitter is resized if bSplitterTracker is set to FALSE.
	// See Also:
	//     IsSplitterTrackerUsed
	//-----------------------------------------------------------------------
	void UseSplitterTracker(BOOL bSplitterTracker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the splitter tracker is used.
	// Returns:
	//     TRUE if the splitter tracker is used, otherwise FALSE.
	// See Also:
	//     UseSplitterTracker
	//-----------------------------------------------------------------------
	BOOL IsSplitterTrackerUsed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets/sets the command bars object.
	// Parameters:
	//     pCommandBars - Pointer to the command bars object to be set.
	// Returns:
	//     A pointer to the command bars object (can be NULL).
	//-----------------------------------------------------------------------
	virtual CXTPCommandBars* GetCommandBars() const;
	void SetCommandBars(CXTPCommandBars* pCommandBars); // <combine
														// CXTPTabClientWnd::GetCommandBars@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to tile child windows in the workspace
	//     (only if EnableGroups is enabled).
	// Parameters:
	//     bHorizontal - TRUE to tile horizontally.
	//-----------------------------------------------------------------------
	void MDITile(BOOL bHorizontal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns an item by its index.
	// Parameters:
	//     nIndex - Zero-based index of the item in the collection.
	// Remarks:
	//     You use this member function to get an item from collection by
	//     item index. If the given index is less than 0 or greater than
	//     the value returned by GetItemCount(), GetItem() returns NULL.
	// Returns:
	//     Pointer to the found item, if any, or NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the number of items.
	// Returns:
	//     The number of items.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to reposition items.
	//-----------------------------------------------------------------------
	virtual void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to enable/disable Markup
	//     functionality for the control.
	// Parameters:
	//     bEnableMarkup - A BOOL. Specifies whether Markup functionality
	//                     should be enabled (TRUE) or disabled (FALSE).
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add an Explorer 7 style new window tab button.
	// Parameters:
	//     bShowNewWindowTab - TRUE to show new window tab button.
	//     nCommand - Command to execute.
	//-----------------------------------------------------------------------
	void ShowNewWindowTab(BOOL bShowNewWindowTab = TRUE, int nCommand = ID_FILE_NEW);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified index.
	// Parameters:
	//     pWorkspace - Pointer to a CXTPTabManger object.
	// Returns:
	//     The index of the matching item.
	//-----------------------------------------------------------------------
	int FindIndex(CXTPTabManager* pWorkspace) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     If m_bAutoTheme is FALSE, then it checks to see if the theme
	//     set for the TabWorkspace is different from the theme set for
	//     the CommandBars. If the themes are different, then the TabWorkSpace
	//     theme is set to the theme specified in m_themeCommandBars.
	//-----------------------------------------------------------------------
	virtual void CheckCommandBarsTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the current active MDI child window.
	// Returns:
	//     A pointer to the active MDI child window.
	//-----------------------------------------------------------------------
	virtual CWnd* MDIGetActive();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to activate an MDI child window
	//     independently of the MDI frame window.
	// Parameters:
	//     pWnd - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void MDIActivate(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the text of a specific tab.
	// Parameters:
	//     pChildWnd - A CWnd pointer that represents the child to be
	//                 displayed when the window is activated.
	// Returns:
	//     The text of a particular tab, or NULL if an error occurs.
	//-----------------------------------------------------------------------
	virtual CString GetItemText(const CWnd* pChildWnd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the RGB value for the referenced
	//     item text, or (COLORREF)-1 if the color was not set.
	// Parameters:
	//     pItem - A pointer to a CXTPTabManagerItem.
	// Returns:
	//     The RGB value for the referenced item text, or (COLORREF)-1
	//     if the color was not set.
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemColor(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the icon of a specific tab.
	// Parameters:
	//     pItem - A pointer to a CXTPTabManagerItem.
	// Returns:
	//     The icon of a particular tab, or NULL if an error occurs.
	//-----------------------------------------------------------------------
	virtual HICON GetItemIcon(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the tooltip associated with the specified item.
	// Parameters:
	//     pItem - Pointer to a CXTPTabManagerItem object.
	// Returns:
	//     Associated tooltip.
	//-----------------------------------------------------------------------
	virtual CString GetItemTooltip(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a workspace.
	// Parameters:
	//     nIndex - Index of the workspace.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual CWorkspace* AddWorkspace(int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds a new tab item for the specified window.
	// Parameters:
	//     pChildWnd - A CWnd pointer that represents the child to be
	//                 displayed when the window is activated.
	// Returns:
	//     A pointer to a CXTPTabManagerItem object.
	//-----------------------------------------------------------------------
	virtual CXTPTabManagerItem* AddItem(CWnd* pChildWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets a workspace active.
	// Parameters:
	//     pWorkspace - Pointer to a CWorkspace object.
	//-----------------------------------------------------------------------
	void SetActiveWorkspace(CWorkspace* pWorkspace);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method shows the context menu for toolbars.
	// Parameters:
	//     pt - Position of the menu to show.
	//-----------------------------------------------------------------------
	void ContextMenu(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to update the status of the
	//     TabWorkspace navigation buttons.
	// Parameters:
	//     pWorkspace - Pointer to a CWorkspace object.
	// SeeAlso:
	//     SetFlags, GetFlags
	//-----------------------------------------------------------------------
	virtual void UpdateFlags(CWorkspace* pWorkspace);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the selected item in the control.
	// Returns:
	//     A pointer to a CXTPTabManagerItem object.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetSelectedItem() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void DoWorkspaceCommand(CXTPTabManagerItem* pItem, CWorkspace* pFocusWorkspace, int nAction,
							CPoint pt = CPoint(), CXTPTabClientWnd* pTargetTab = NULL
#	ifndef _XTP_COMMANDBARS_ACTIVEX
							,
#		ifdef _XTP_INCLUDE_DOCKINGPANE
							CXTPDockingPaneTabbedContainer* pTargetPane = NULL
#		else
							LPVOID pReserved = NULL
#		endif
#	endif
	);
	void TrackSplitter(int nWorkspace, CPoint point);
	void RepositionWorkspaces(CRect rc, CRect rcAvail, CWorkspace* pWorkspaceFirst,
							  CWorkspace* pWorkspaceSecond);
	void NormalizeWorkspaceSize();
	void ActivateNextItem(CXTPTabManagerItem* pItem);
	void ActivateNextTopmostItem(CXTPTabManagerItem* pItem);
	virtual BOOL OnBeforeItemClick(CXTPTabManagerItem* pItem);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTabClientWnd)
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTabClientWnd)

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	afx_msg LRESULT OnMDIActivate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDICreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDINext(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdateWorkspaceCommand(CCmdUI* pCmdUI);
	afx_msg BOOL OnNcActivate(BOOL bActive);

public:
	afx_msg void OnIdleUpdateCmdUI();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

public:
	BOOL m_bDelayLock;		// TRUE to lock update with small delay.
	BOOL m_bLockUpdate;		// TRUE to lock update tabs.
	BOOL m_bForceToRecalc;  // TRUE to recalculate in next idle event.
	BOOL m_bLockReposition; // TRUE to lock reposition.

protected:
	CArray<CWorkspace*, CWorkspace*> m_arrWorkspace; // Array of workspaces.
	CWorkspace* m_pTabWorkspace;					 // Custom workspace.
	BOOL m_bUserWorkspace;							 // TRUE if custom workspace used.

	CMDIFrameWnd* m_pParentFrame; // Pointer to the corresponding parent MDIFrame window.
	BOOL m_bRefreshed;			  // TRUE if Refresh currently executed.

	CXTPTabPaintManager* m_pPaintManager; // Current paint manager.
	XTPPaintTheme m_themeCommandBars;	 // Theme set for the TabWorkspace (Can be different from
										  // CommandBars if m_bAutoTheme is FALSE)
	BOOL m_bAutoTheme;					  // TRUE to auto theme the TabWorkspace.

	CWorkspace* m_pActiveWorkspace; // Active workspace.
	BOOL m_bHorizSplitting;			// TRUE if groups divided horizontally.

	HCURSOR m_hCursorHoriz;  // Horizontal split cursor handle.
	HCURSOR m_hCursorVert;   // Vertical split cursor handle.
	HCURSOR m_hCursorNew;	// New item cursor handle.
	HCURSOR m_hCursorDelete; // Remove item cursor handle.

	BOOL m_bThemedBackColor; // TRUE if a themed backcolor will be used. If TRUE, then the
							 // backcolor of the MDIClient is calculated from the current theme, if
							 // FALSE, then the standard BackColor property will be used.

	BOOL m_bEnableGroups; // TRUE if TabWorkspace groups are enabled.

	DWORD m_dwFlags;	   // Specifies which tab navigation buttons are displayed.
	BOOL m_bAllowReorder;  // TRUE to allow the user to reorder tabs.
	BOOL m_bShowWorkspace; // TRUE to show the TabWorkspace, FALSE to hide the TabWorkspace.

	BOOL m_bUpdateContents;				   // TRUE if UpdateContents executed.
	CXTPToolTipContext* m_pToolTipContext; // Tooltip Context.
	BOOL m_bUseSplitterTracker; // If TRUE, splitter trackers are used. When resizing a workspace,
								// an outline of the workspace is drawn as the splitter is dragged.
								// If FALSE, then the workspace will be resized in "real-time."
	int m_nSplitterSize;		// Splitter size.
	XTPWorkspaceNewTabPosition m_newTabPosition;	 // New tab position.
	XTPWorkspaceActivateTab m_afterCloseTabPosition; // Tab to make active after the currently
													 // active tab is closed.
	BOOL m_bAutoUpdateTabCaption; // TRUE if the tab caption is to be updated automatically when
								  // changed, FALSE if it must manually be updated.
	CXTPMarkupContext* m_pMarkupContext; // Markup Context.
	BOOL m_bShowNewWindowTab;
	int m_nNewWindowTabCommand;

	CXTPCommandBars* m_pCommandBars;

public:
	void EnableTearOff();
	BOOL IsTearOffEnabled() const;
	BOOL IsGroupingEnabled() const;

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
public:
	void SetDockingPaneManager(CXTPDockingPaneManager* pPaneManager);
	CXTPDockingPaneManager* GetDockingPaneManager();

protected:
	CXTPDockingPaneManager* m_paneManager;
	CXTPDockingPaneTabbedContainer* FindTargetDockPane(POINT pt);
#		else
	//{{AFX_CODEJOCK_PRIVATE
	// Required for padding
	LPVOID m_paneManager;
	//}}AFX_CODEJOCK_PRIVATE
#		endif
#	endif

private:
	BOOL m_bEnableTearOff; // TRUE if tear-off enabled
	CPtrList m_lstNextTab;
	CPtrList m_lstPrevTab;
	CXTPTabClientWnd* GetTabClientHead();
	CXTPTabClientWnd* FindTargetTabClient(POINT pt);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
	void FindTargetTabClients(CXTPTabClientWnd* pTabClient,
							  CArray<CXTPTabClientWnd*, CXTPTabClientWnd*>* collection, POINT pt);

	void EnumTabClients(CXTPTabClientWnd* pTabClient,
						CArray<CXTPTabClientWnd*, CXTPTabClientWnd*>* collection);
	void ChangeMDIChildFrame(CWnd* pTarget);
	void CloseTearOffFrame(CXTPTabClientWnd* pTarget);
	void ChangeItemWorkspace(CXTPTabClientWnd* pTarget, int nOldWorkspace, int nSticker);

	CXTPTabClientContext* m_pTabClientContext;

public:
	void SetShowTabClientContextStickers(BOOL value);
	void SetAlphaTabClientContext(BOOL value);
	void SetShowContentsWhileDragging(BOOL value);
	void EnableFloatingFrameTheme(BOOL value);

	BOOL GetAlphaTabClientContext() const;
	BOOL GetShowTabClientContextStickers() const;
	BOOL GetShowContentsWhileDragging() const;
	BOOL IsFloatingFrameThemeEnabled() const;

	void SetForegroundTearOffFrames();
	void CloseAllTearOffFrames();
	void GetTabClients(CArray<CXTPTabClientWnd*, CXTPTabClientWnd*>& collection);
	HWND GetLastSelecedItem();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	CRect m_rcGroup;
	CTabClientDropTarget* m_pDropTarget;
	CWorkspace* m_pFocusWorkspace;
	int m_nFocusedAction;
	BOOL m_bIgnoreFlickersOnActivate;
	BOOL m_bRightToLeft;
	UINT m_nMsgUpdateSkinState;
	UINT m_nMsgQuerySkinState;
	CString m_strIniFileName;
	BOOL NeedsRefresh(XTPPaintTheme theme);
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	DECLARE_OLETYPELIB_EX(CXTPTabClientWnd);
	DECLARE_CONNECTION_EX(CXTPTabClientWnd)

	DECLARE_ENUM_VARIANT(CXTPTabClientWnd)

	afx_msg LPDISPATCH OleGetItem(int nItem);
	afx_msg LPDISPATCH OleGetSelected();
	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleHitTest(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	afx_msg void OleRefresh();
	afx_msg BOOL OleIsWorkspaceCommandEnabled(long nID);
	afx_msg void OleSetEnableMarkup(BOOL bEnableMarkup);
	afx_msg BOOL OleGetEnableMarkup();
	afx_msg LPDISPATCH OleGetToolTipContext();
	afx_msg void OleSetHorizGroupSplitting(BOOL bHorizSplitting);
	afx_msg BOOL OleGetHorizGroupSplitting();
	afx_msg long OlegGetGroupCount();
	afx_msg double OleGetGroupSplitter(int nGroup);
	afx_msg void OleSetGroupSplitter(int nGroup, double dSplitterPos);
	afx_msg OLE_HANDLE OleGetHwnd();

	afx_msg LPDISPATCH OleFindItem(OLE_HANDLE hWnd);
	afx_msg int OleFindGroup(LPDISPATCH Item);
	afx_msg void OleCreateGroup(LPDISPATCH Item, BOOL Horiz);
	afx_msg void OleMoveToGroup(LPDISPATCH Item, int nGroup);

	void FireRClick(LPDISPATCH lpDispatch)
	{
		FireEvent(1, EVENT_PARAM(VTS_DISPATCH), lpDispatch);
	}
	void FireNewItem(LPDISPATCH lpDispatch)
	{
		FireEvent(2, EVENT_PARAM(VTS_DISPATCH), lpDispatch);
	}
	void FireSelectedChanged(LPDISPATCH Item)
	{
		FireEvent(3, EVENT_PARAM(VTS_DISPATCH), Item);
	}
	void FireBeforeItemClick(LPDISPATCH Item)
	{
		FireEvent(4, EVENT_PARAM(VTS_DISPATCH), Item);
	}

	enum
	{
		dispidFlags = 1L,
	};

	LPDISPATCH OleGetPaintManager();
	void OleEnableGroups();

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CWorkspace;
	friend class CSingleWorkspace;
	friend class CTabClientDropTarget;
	friend class CNavigateButtonActiveFiles;
	friend class CXTPControlTabWorkspace;
	friend class CXTPTearOffFrame;
	friend class CXTPTabClientSticker;
	friend class CXTPDockPaneContextSticker;
	friend class CXTPTabClientContext;
	friend class CXTPTabClientDraggingContents;
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	friend class CXTPDockingPaneContext;
#		endif
#	endif
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPTabClientWnd::IsAttached() const
{
	return (m_pParentFrame != NULL ? TRUE : FALSE);
}

AFX_INLINE CMDIFrameWnd* CXTPTabClientWnd::GetParentFrame() const
{
	return m_pParentFrame;
}
AFX_INLINE DWORD CXTPTabClientWnd::GetFlags() const
{
	return m_dwFlags;
}
AFX_INLINE CXTPToolTipContext* CXTPTabClientWnd::GetToolTipContext() const
{
	return m_pToolTipContext;
}
AFX_INLINE void CXTPTabClientWnd::UseSplitterTracker(BOOL bSplitterTracker)
{
	m_bUseSplitterTracker = bSplitterTracker;
}
AFX_INLINE BOOL CXTPTabClientWnd::IsSplitterTrackerUsed() const
{
	return m_bUseSplitterTracker;
}
AFX_INLINE void CXTPTabClientWnd::SetNewTabPosition(XTPWorkspaceNewTabPosition tabPosition)
{
	m_newTabPosition = tabPosition;
}
AFX_INLINE XTPWorkspaceNewTabPosition CXTPTabClientWnd::GetNewTabPositon() const
{
	return m_newTabPosition;
}
AFX_INLINE void CXTPTabClientWnd::SetAfterCloseActiveTab(XTPWorkspaceActivateTab activeTab)
{
	m_afterCloseTabPosition = activeTab;
}
AFX_INLINE void CXTPTabClientWnd::EnableTearOff()
{
	m_bEnableTearOff = TRUE;
}
AFX_INLINE BOOL CXTPTabClientWnd::IsTearOffEnabled() const
{
	return m_bEnableTearOff;
}
AFX_INLINE BOOL CXTPTabClientWnd::IsGroupingEnabled() const
{
	return m_bEnableGroups;
}
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
AFX_INLINE void CXTPTabClientWnd::SetDockingPaneManager(CXTPDockingPaneManager* pPaneManager)
{
	m_paneManager = pPaneManager;
}
AFX_INLINE CXTPDockingPaneManager* CXTPTabClientWnd::GetDockingPaneManager()
{
	return m_paneManager;
}
#		endif
#	endif
#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPTABCLIENTWNDEX_H__)
