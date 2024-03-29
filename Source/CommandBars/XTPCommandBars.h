// XTPCommandBars.h : interface for the CXTPCommandBars class.
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
#if !defined(__XTPCOMMANDBARS_H__)
#	define __XTPCOMMANDBARS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockBar;
class CXTPToolBar;
class CXTPControl;
class CXTPPopupBar;
class CXTPDockState;
class CXTPCommandBar;
class CXTPCustomizeDropSource;
class CXTPCommandBars;
class CXTPCommandBarList;
class CXTPImageManager;
class CXTPPaintManager;
class CXTPControls;
class CXTPMenuBar;
class CXTPReBar;
class CXTPToolTipContext;
class CXTPShortcutManager;
class CXTPKeyboardManager;
class CXTPStatusBar;
class CXTPMouseManager;
class CXTPControlActions;
class CXTPControlAction;
class CXTPPropExchange;
class CXTPCommandBarsFrameHook;
class CXTPRecentFileList;
class CXTPCommandBarsOptions;
class CXTPCommandBarsContextMenus;
class CXTPCommandBarKeyboardTip;

//-----------------------------------------------------------------------
// Summary:
//     XTP_COMMANDBARS_PROPEXCHANGE_PARAM is a
//     structure used to store the CommandBar's save settings. This is
//     a set of State options that will be used to change how and what
//     will be saved and loaded when loading and saving XML files.
// Remarks:
//     When saving your XML string to a file, be sure to use the .XML
//     file extension.  If you open up your .XML file in your favorite
//     web browser it will automatically format the XML string into the
//     correct format.
// See Also:
//     CXTPCommandBars::DoPropExchange, CXTPPropExchangeXMLNode
// Example:
// <code>
// XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
// param.bSerializeControls = TRUE;
// // Serialize to XML
// CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
// CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
// GetCommandBars()->DoPropExchange(&pxCommandBars, &param);
// px.SaveToFile(m_strIniFileName);
// </code>
//-----------------------------------------------------------------------
struct XTP_COMMANDBARS_PROPEXCHANGE_PARAM
{
	//{{AFX_CODEJOCK_PRIVATE
	//-------------------------------------------------------------------------
	// Summary:
	//     Creates a XTP_COMMANDBARS_PROPEXCHANGE_PARAM object which is a
	//     structure used to store the CommandBar's save settings.  This is
	//     a set of State options that will be used to change how and what
	//     will be saved and loaded when loading and saving XML files.
	//     The CXTPPropExchangeXMLNode class is used to create the XML file.
	// Remarks:
	//     When saving your XML string to a file, be sure to use the .XML
	//     file extension.  If you open up your .XML file in your favorite
	//     web browser it will automatically format the XML string into the
	//     correct format.
	// See Also: CXTPCommandBars::DoPropExchange, CXTPPropExchangeXMLNode
	//-------------------------------------------------------------------------
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM()
	{
		pCommandBars			   = NULL;
		bSerializeControls		   = FALSE;
		bSaveOnlyCustomized		   = TRUE;
		bLoadSilent				   = FALSE;
		bSaveOriginalControls	  = TRUE;
		bSerializeLayout		   = TRUE;
		bSerializeOptions		   = FALSE;
		bSerializeImages		   = FALSE;
		bSerializeDesignerControls = FALSE;
		bSerializeActions		   = FALSE;
	}
	//}}AFX_CODEJOCK_PRIVATE

	BOOL bSerializeControls;  // If TRUE, the controls in the CommandBars will be saved.  If FALSE,
							  // no controls will be saved, regardless of the other settings.  The
							  // default is TRUE.
	BOOL bSaveOnlyCustomized; // If TRUE, only CommandBars that have been customized will be saved.
							  // The default is TRUE.
	BOOL bLoadSilent; // If TRUE, no message boxes will be displayed when the CommandBar layout is
					  // loaded. i.e.  The message box that asks for conformation before the
					  // CommandBar layout is reset.
	BOOL bSaveOriginalControls; // If TRUE, the original state of the CommandBars is saved along
								// with the customized state. For example, if a button on a toolbar
								// is moved, then both the original and modified states will be
								// saved. The Default is TRUE.
	BOOL bSerializeImages;  // If TRUE, the images stored in the ImageManager will be saved.  The
							// default is FALSE.
	BOOL bSerializeOptions; // If TRUE, the setting on the Keyboard and Options pages of the
							// Customize dialog will be saved.  This includes shortcut keys and
							// CommandBar options like large icons and full menus. The default is
							// FALSE.
	BOOL bSerializeLayout;  // If TRUE, the layout of the CommandBars will be saved.  The Layout
							// includes information such as bar position, location, and size.  The
							// Default is TRUE.
	BOOL bSerializeDesignerControls; // In the CommandBar Designer you can export a XCB file to a
									 // XML file and Load it in your project. If
									 // SerializeDesignerControls is TRUE, then the DesignerControls
									 // property will be filled with Control items from the
									 // CommandBar Designer's controls pane.  This allows you to
									 // load the XML file instead of loading the XCB file using the
									 // LoadDesignerBars method.  NOTE: DesignerControls are the
									 // controls displayed in the customization dialog. The
									 // CXTPPropExchangeXMLNode class is used to create the XML
									 // file. The default is FALSE.
	BOOL bSerializeActions;			 // TRUE to save/restore actions for commandbars
	CXTPCommandBars* pCommandBars;   // Pointer to the CommandBars object that the
									 // XTP_COMMANDBARS_PROPEXCHANGE_PARAM setting apply to.
};

//===========================================================================
// Summary:
//     CXTPCommandBars is a standalone class. It allows developers to manipulate
//     toolbars and pop-up bars.
// See Also:
//     CXTPCommandBar, CXTPToolBar, CXTPCommandBarsOptions
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBars
	: public CXTPCmdTarget
	, private IXTPApplicationEvents
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBars object.
	//-----------------------------------------------------------------------
	CXTPCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBars object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBars();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to create a menu bar.
	// Parameters:
	//     lpszName - Title of the menu bar.
	//     nID - Menu identifier to be loaded.
	// Returns:
	//     A CXTPMenuBar object if successful; otherwise NULL.
	// See Also:
	//     Add
	//-----------------------------------------------------------------------
	CXTPMenuBar* SetMenu(LPCTSTR lpszName, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new toolbar.
	// Parameters:
	//     lpszName - Title of the toolbar.
	//     nBarPosition - Position of toolbar to be created.
	//     pToolbarClass - Runtime class of toolbar to create
	// Remarks:
	//     CommandBars will create a new toolbar with pToolbarClass runtime class
	//     if it specified. If not, then m_pToolBarClass will be used.<p/>
	//     Call SetToolBarClass to set the default runtime class for toolbars.
	// Returns:
	//     A CXTPToolBar object if successful; otherwise NULL.
	// See Also:
	//     SetMenu, SetToolBarClass
	//-----------------------------------------------------------------------
	CXTPToolBar* Add(LPCTSTR lpszName, XTPBarPosition nBarPosition,
					 CRuntimeClass* pToolbarClass = NULL);
	CXTPToolBar* Add(
		CXTPToolBar* pToolBar, LPCTSTR strName,
		XTPBarPosition nBarPosition); // <combine
									  // CXTPCommandBars::Add@LPCTSTR@XTPBarPosition@CRuntimeClass*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the active menu bar.
	// Returns:
	//     A CXTPMenuBar pointer associated with the menu bar.
	//-----------------------------------------------------------------------
	virtual CXTPMenuBar* GetMenuBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the toolbar with the specified identifier.
	// Parameters:
	//     nID - An integer identifier.
	// Returns:
	//     The CXTPToolBar pointer.
	//-----------------------------------------------------------------------
	CXTPToolBar* GetToolBar(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the site of the command bars.
	// Returns:
	//     A CFrameWnd pointer representing the site of the command bars.
	//-----------------------------------------------------------------------
	CWnd* GetSite() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the dock bar at the specified position.
	// Parameters:
	//     barPosition - Position of the dockbar.
	// Returns:
	//     A CXTPDockBar pointer.
	//-----------------------------------------------------------------------
	CXTPDockBar* GetDockBar(XTPBarPosition barPosition) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the command bars.
	//-----------------------------------------------------------------------
	void RedrawCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to invalidate the command bars.
	//-----------------------------------------------------------------------
	void InvalidateCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to recalculate site layout.
	// Parameters:
	//     bDelay - TRUE to delay dock site layout until remaining framework
	//              items have been initialized completely.
	//-----------------------------------------------------------------------
	virtual void RecalcFrameLayout(BOOL bDelay = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified control.
	// Parameters:
	//     type - Type of the control to find.
	//     nId - Control's identifier.
	//     bVisible - Control's visibility.
	//     bRecursive - To find in the nested command bars.
	// Returns:
	//     The CXTPControl object if successful; otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPControl* FindControl(XTPControlType type, UINT nId, BOOL bVisible, BOOL bRecursive) const;

	// -----------------------------------------------------------------------------
	// Summary:
	//     Call this member to display a floating pop-up menu at the
	//     specified location.
	// Parameters:
	//     pMenu -         Menu to be popped up.
	//     nFlags -        Reserved, currently not used.
	//     xPos -          Specifies the horizontal position in screen coordinates
	//                     of the pop-up menu.
	//     yPos -          Specifies the vertical position in screen coordinates of
	//                     the top of the menu on the screen.
	//     pWnd -          Identifies the window that owns the pop-up menu. This
	//                     window receives all WM_COMMAND messages from the menu.
	//     rcExclude -     Reserved, currently not used.
	//     pWndOwner -     Pointer to a CWnd object.
	//     pCommandBars -  Pointer to a CXTPCommandBars object.
	//     pPopupBar -     Pointer to a CXTPPopupBar object.
	//     nBarID -        Identifier CommandBar to track.
	//     lptpm -         Pointer to a TPMPARAMS structure that specifies an area of the
	//                     screen the menu should not overlap. This parameter can be NULL.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	// Example:
	//     <code>
	//         void CCustomThemesView::OnRButtonDown(UINT /*nFlags*/, CPoint point)
	// {
	//     ClientToScreen(&point);
	//
	//     CMenu menu;
	//     VERIFY(XTPResourceManager()->LoadMenu(&menu, IDR_MAINFRAME));
	//
	//     // get a pointer to the application window.
	//     CXTPMDIFrameWnd* pMainWnd = DYNAMIC_DOWNCAST(
	//         CXTPMDIFrameWnd, AfxGetMainWnd());
	//
	//     if (!pMainWnd)
	//         return;
	//
	//     // get a pointer to the CXTPCommandBars object.
	//     CXTPCommandBars* pCommandBars = pMainWnd-\>GetCommandBars();
	//
	//     if (!pCommandBars)
	//         return;
	//
	//     // display context menu.
	//     CXTPCommandBars::TrackPopupMenu(&menu, TPM_RIGHTBUTTON,
	//         point.x, point.y, pMainWnd, 0, 0, pCommandBars);
	// }
	//     </code>
	// -----------------------------------------------------------------------------
	static BOOL AFX_CDECL TrackPopupMenu(CMenu* pMenu, UINT nFlags, int xPos, int yPos, CWnd* pWnd,
										 LPCRECT rcExclude = NULL, CWnd* pWndOwner = NULL,
										 CXTPCommandBars* pCommandBars = NULL);
	static BOOL AFX_CDECL TrackPopupMenu(
		CXTPPopupBar* pPopupBar, UINT nFlags, int xPos, int yPos, CWnd* pWnd,
		LPCRECT rcExclude = NULL,
		CWnd* pWndOwner =
			NULL); //<COMBINE
				   // CXTPCommandBars::TrackPopupMenu@CMenu*@UINT@int@int@CWnd*@LPCRECT@CWnd*@CXTPCommandBars*>
	BOOL TrackPopupMenu(
		UINT nBarID, UINT nFlags, int xPos, int yPos,
		LPCRECT rcExclude =
			NULL); //<COMBINE
				   // CXTPCommandBars::TrackPopupMenu@CMenu*@UINT@int@int@CWnd*@LPCRECT@CWnd*@CXTPCommandBars*>
	BOOL TrackPopupMenuEx(
		CMenu* pMenu, UINT nFlags, int xPos, int yPos, CWnd* pWndOwner = NULL,
		LPTPMPARAMS lptpm =
			NULL); //<COMBINE
				   // CXTPCommandBars::TrackPopupMenu@CMenu*@UINT@int@int@CWnd*@LPCRECT@CWnd*@CXTPCommandBars*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves list of available context menus.
	// Returns:
	//     Command Bars context menus.
	//-----------------------------------------------------------------------
	CXTPCommandBarsContextMenus* GetContextMenus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the options of the command bars.
	// Returns:
	//     Command Bars options.
	//-----------------------------------------------------------------------
	CXTPCommandBarsOptions* GetCommandBarsOptions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName  - Points to a null-terminated string that
	//                        specifies the name of a section in the
	//                        initialization file or a key in the Windows
	//                        registry where state information is stored.
	//     bOnlyCustomized  - TRUE to save only the state for customized
	//                        command bar items.
	// See Also:
	//     LoadBarState, DoPropExchange
	//-----------------------------------------------------------------------
	void SaveBarState(LPCTSTR lpszProfileName, BOOL bOnlyCustomized = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where
	//                       state information is stored.
	//     bSilent         - TRUE to disable user notifications when command bars are
	//                       restored to their original state.
	// See Also:
	//     SaveBarState, DoPropExchange
	//-----------------------------------------------------------------------
	void LoadBarState(LPCTSTR lpszProfileName, BOOL bSilent = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the command bars options.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where
	//                       state information is stored.
	// See Also:
	//     LoadOptions, DoPropExchange
	//-----------------------------------------------------------------------
	void SaveOptions(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to restore the options of the command bars.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where
	//                       state information is stored.
	// See Also:
	//     SaveOptions, DoPropExchange
	//-----------------------------------------------------------------------
	void LoadOptions(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to either read command bars from or write
	//     command bars to an archive.
	// Parameters:
	//     ar                 - A CArchive object to serialize to or from.
	//     bSerializeControls - TRUE to store controls information.
	//     bOnlyCustomized    - TRUE to store each command bar only if it was
	//                          customized by the user.
	//     bSilent            - TRUE to disable user notifications when command
	//                          bars are restored to their original state.
	// See Also:
	//     DoPropExchange
	//-----------------------------------------------------------------------
	void SerializeBarState(CArchive& ar, BOOL bSerializeControls = FALSE,
						   BOOL bOnlyCustomized = TRUE, BOOL bSilent = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ShortcutManager class pointer.
	// Returns:
	//     Pointer to the current shortcut manager.
	// See Also:
	//     SetShortcutManager
	//-----------------------------------------------------------------------
	CXTPShortcutManager* GetShortcutManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set a custom shortcut manager class.
	// Parameters:
	//     pShortcutManager - New ShortcutManager to be set.
	// See Also:
	//     GetShortcutManager
	//-----------------------------------------------------------------------
	void SetShortcutManager(CXTPShortcutManager* pShortcutManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pParam - Exchange options.
	// Example:
	// <code>
	// XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
	// param.bSerializeControls = TRUE;
	// // Serialize to XML
	// CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
	// CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
	// GetCommandBars()->DoPropExchange(&pxCommandBars, &param);
	// px.SaveToFile(m_strIniFileName);
	// </code>
	// See Also:
	//     CXTPPropExchange, XTP_COMMANDBARS_PROPEXCHANGE_PARAM
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load designer command bars from
	//     an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     nSchemaVersion - Schema version number that indicates the version of the serialized
	//     CommandBars data, if it is known. The default value _XTP_SCHEMA_CURRENT assumes that the
	//     CommandBars data have been created using the current version and hence is fully
	//     compatible. If the schema version number does not match the actual schema version number
	//     used for generating the CommandBars data then the behavior is undefined and it may result
	//     in crashes and memory access issues.
	// See Also:
	//     CXTPPropExchange, CXTPPropExchange::SetSchema
	//-----------------------------------------------------------------------
	void LoadDesignerBars(CArchive& ar, UINT nSchemaVersion = _XTP_SCHEMA_CURRENT);
	void LoadDesignerBars(
		CXTPPropExchange* pPX); // <combine
								// CXTPCommandBars::LoadDesignerBars@CArchive&@UINT>

	//# Intelligent menus routines

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds the specified commands to the list of
	//     menu items to hide until activated by clicking on the chevron.
	// Parameters:
	//     pCommands - An array of command IDs, or menu items, to hide.
	//     nCount    - Size of the array passed in.
	// Remarks:
	//     The command will not be displayed in pop-up menus until the user
	//     clicks on the "expand menu" chevron or after a short delay when
	//     bShowFullAfterDelay is TRUE.  Note:  bAlwaysShowFullMenus must set
	//     to FALSE to hide the hidden commands
	//-----------------------------------------------------------------------
	void HideCommands(const UINT* pCommands, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds the specified command to the list of
	//     menu items to hide until activated by clicking on the chevron.
	// Parameters:
	//     nCommand - The command ID of a menu item to hide.
	//-----------------------------------------------------------------------
	void HideCommand(UINT nCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve if the command is hidden
	//     until activated by clicking on the chevron.  I.e. Infrequently
	//     used commands.
	// Parameters:
	//     nCommand - Command id to check.
	// Remarks:
	//     The command will not be displayed in pop-up menus until the user
	//     clicks on the "expand menu" chevron or after a short delay when
	//     bShowFullAfterDelay is TRUE.  Note:  bAlwaysShowFullMenus must set
	//     to FALSE to hide the hidden commands
	// Returns:
	//     TRUE if the command item is hidden; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCommandHidden(UINT nCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve if the control is hidden and of type
	//     CXTPControlPopup when bAutoHideUnusedPopups is TRUE.
	// Parameters:
	//     pControl - Control pointer to check.
	// Remarks:
	//     If bAutoHideUnusedPopups is FALSE, then this member performs the same
	//     action as IsCommandHidden. Override this member to provide a custom
	//     intelligent menu scheme.
	// Returns:
	//     TRUE if the control item is hidden; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsControlHidden(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the command activated by the user.
	// Parameters:
	//     nCommand - Command ID to set.
	//-----------------------------------------------------------------------
	void SetCommandUsed(UINT nCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to restore the state of the hidden
	//     commands.
	//-----------------------------------------------------------------------
	void ResetUsageData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the active MDI document template.
	// Returns:
	//     Active document template identifier.
	//-----------------------------------------------------------------------
	virtual UINT GetActiveDocTemplate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the application's command bars into and out of
	//     print-preview mode.
	// Parameters:
	//     bPreview - Specifies whether or not to place the application in print-preview mode.
	//                Set to TRUE to place in print preview, FALSE to cancel preview mode.
	//-----------------------------------------------------------------------
	void OnSetPreviewMode(BOOL bPreview);

	//-------------------------------------------------------------------------
	// Summary:
	//     Sends WM_IDLEUPDATECMDUI message to all tool bars.
	//-------------------------------------------------------------------------
	void UpdateCommandBars() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets/sets an all-caps flag value that causes command bars to draw all
	//     titles uppercase if the flag is set.
	// Parameters:
	//     bAllCaps - TRUE to make all command bars title uppercase.
	// Returns:
	//     Current all-caps flag value.
	//-----------------------------------------------------------------------
	BOOL IsAllCaps() const;
	void SetAllCaps(BOOL bAllCaps); // <combine CXTPCommandBars::IsAllCaps>

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

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to process frame messages.
	//     CXTPFrameWndBase derived classes call this method automatically.
	// Parameters:
	//     pMsg - Points to an MSG object.
	// Returns:
	//     TRUE if the message was handled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL PreTranslateFrameMessage(MSG* pMsg);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to process frame messages.
	//     CXTPFrameWndBase derived classes call this method automatically.
	// Parameters:
	//     message - Specifies the message to be sent.
	//     wParam  - Specifies additional message-dependent information.
	//     lParam  - Specifies additional message-dependent information.
	//     pResult - The return value of WindowProc. Depends on the message;
	//               may be NULL.
	// Returns:
	//     TRUE if the message was handled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnFrameWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method shows the context menu for toolbars.
	// Parameters:
	//     pToolBar - Pointer to a CXTPToolBar object.
	//     point - Position of the menu to show.
	//-----------------------------------------------------------------------
	virtual void ContextMenu(CXTPToolBar* pToolBar, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates a context menu for toolbars.
	// Returns:
	//     A CXTPPopupBar object containing a list of toolbars.
	//-----------------------------------------------------------------------
	virtual CXTPPopupBar* GetToolbarsPopup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create an expand bar.
	// Parameters:
	//     pToolBar - Pointer to a CXTPToolBar object.
	//     pExpandBar - Pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	virtual void GetHiddenControls(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create customized controls.
	// Parameters:
	//     pToolBar   - Pointer to a CXTPToolBar object.
	//     pExpandBar - Pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	virtual void GetAddOrRemovePopup(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the site window.
	// Parameters:
	//     pFrame - Points to a CWnd object.
	//-----------------------------------------------------------------------
	void SetSite(CWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the dock bar at the specified point.
	// Parameters:
	//     pt - Point to test.
	//     pMainDock - CXTPDockBar class currently docked in.
	// Returns:
	//     A pointer to a CXTPDockBar object.
	//-----------------------------------------------------------------------
	CXTPDockBar* CanDock(CPoint pt, CXTPDockBar* pMainDock = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member creates command bars.
	// Returns:
	//     A pointer to a CXTPCommandBars object.
	//-----------------------------------------------------------------------
	static CXTPCommandBars* AFX_CDECL CreateCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to create dock bars.
	//-----------------------------------------------------------------------
	void EnableDocking();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the toolbar count.
	// Returns:
	//     Number of the toolbars.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a toolbar with a specified index.
	// Parameters:
	//     nIndex - Index of the toolbar.
	// Returns:
	//     A pointer to a CXTPToolBar object.
	//-----------------------------------------------------------------------
	CXTPToolBar* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to activate a toolbar.
	// Parameters:
	//     pCommandBar - Toolbar to activate.
	//     bNext - TRUE to set focus to the next toolbar.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetToolBarFocus(CXTPToolBar* pCommandBar, BOOL bNext = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if command bars are in customize mode.
	// Returns:
	//     TRUE if command bars are in customize mode, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCustomizeMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set command bars to customize mode.
	// Parameters:
	//     bMode - TRUE to set command bars to customize mode.
	//-----------------------------------------------------------------------
	void SetCustomizeMode(BOOL bMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if command bars are in quick customize mode.
	// Returns:
	//     TRUE if command bars are in quick customize mode, otherwise FALSE.
	// See Also:
	//     SetQuickCustomizeMode
	//-----------------------------------------------------------------------
	BOOL IsQuickCustomizeMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set command bars to quick customize mode.
	// Parameters:
	//     bMode - TRUE to set command bars to quick customize mode.
	// See Also:
	//     IsQuickCustomizeMode
	//-----------------------------------------------------------------------
	void SetQuickCustomizeMode(BOOL bMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the CXTPCustomizeDropSource helper
	//     class for customization.
	// Returns:
	//     A pointer to a CXTPCustomizeDropSource object.
	//-----------------------------------------------------------------------
	CXTPCustomizeDropSource* GetDropSource() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the customized control changes.
	// Parameters:
	//     pControl - Pointer to a CXTPControl object.
	//-----------------------------------------------------------------------
	void SetDragControl(CCmdTarget* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the current customized control.
	// Returns:
	//     A pointer to a CXTPControl object.
	//-----------------------------------------------------------------------
	CCmdTarget* GetDragControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve if the customize is avail.
	// Returns:
	//     TRUE if customize is avail; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCustomizeAvail() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable/disable customization.
	// Parameters:
	//     bCustomizeAvail - TRUE to enable customization; FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableCustomization(BOOL bCustomizeAvail);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove the toolbar.
	// Parameters:
	//     pToolBar - Points to a CXTPToolBar object to remove.
	//-----------------------------------------------------------------------
	void Remove(CXTPToolBar* pToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all toolbars.
	//-----------------------------------------------------------------------
	void RemoveAll();

	// -----------------------------------------------------------------
	// Summary:
	//     Call this member to dock a toolbar to the specified dockbar.
	// Parameters:
	//     pBar        - Pointer to a CXTPToolBar object.
	//     lpRect      - Position to dock.
	//     pDockBar    - Dock bar needed to be docked.
	//     barPosition - Position to dock.
	// Returns:
	//     TRUE if the toolbar was successfully docked, FALSE if the bar
	//     could not be docked.
	// -----------------------------------------------------------------
	BOOL DockCommandBar(CXTPToolBar* pBar, LPRECT lpRect, CXTPDockBar* pDockBar);
	BOOL DockCommandBar(
		CXTPToolBar* pBar,
		XTPBarPosition
			barPosition); // <combine
						  // CXTPCommandBars::DockCommandBar@CXTPToolBar*@LPRECT@CXTPDockBar*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified paint manager.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified paint manager.
	// Parameters:
	//     paintTheme - Visual theme to be set.
	//-----------------------------------------------------------------------
	void SetTheme(XTPPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified image manager.
	// Parameters:
	//     pImageManager - Pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the paint manager of the command
	//     bars.
	// Returns:
	//     A pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the image manager of the command
	//     bars.
	// Returns:
	//     A pointer to a CXTPImageManager object.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show/hide the toolbar.
	// Parameters:
	//     nIndex - Index of the toolbar to show or hide.
	//-----------------------------------------------------------------------
	void ToggleVisible(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the customization file path.
	// Parameters:
	//     lpszFile - Path to customization file information.
	// Remarks:
	//     CommandBars can't store to registry customization information greater than 16kb,
	//     so it creates a file with user customization information.
	//     If a file is not specified, then the current directory will be used
	//     to save customization.
	// Example:
	// <code>
	// if (GetAppDataFolder(strAppDataFolder))
	// {
	//     CreateDirectory(strAppDataFolder + _T("\\") + AfxGetApp()->m_pszAppName, NULL);
	//
	//      // Set customization path as "\\Documents and Settings\\user\\Application
	//      Data\\CustomThemes\\CommandBars"
	//      GetCommandBars()->SetCustomizationDataFileName(strAppDataFolder + _T("\\") +
	//      AfxGetApp()->m_pszAppName + _T("\\") + lpszProfileName);
	// }
	// </code>
	//-----------------------------------------------------------------------
	void SetCustomizationDataFileName(LPCTSTR lpszFile);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the default choice for the Reset Customized Toolbar message.
	// Parameters:
	//     bEnable - TRUE to enable the default choice, FALSE to disable the default choice.
	//     bDefaultYes - If TRUE, then the default choice will be "Yes". Otherwise "No".
	//-----------------------------------------------------------------------
	void EnableDefaultResetCustomizedToolbarChoice(BOOL bEnable, BOOL bDefaultYes);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method finds the rebar of the frame.
	// Returns:
	//     CXTPReBar pointer if the rebar was found; otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPReBar* GetFrameReBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default toolbar class.
	// Parameters:
	//     pToolBarClass - Runtime class of toolbars to be added.
	// Example:
	// <code>
	// class CSystemToolBar : public CXTPToolBar
	// {
	//     DECALRE_DYNCREATE(CSystemToolBar)
	//
	//     ....
	//
	// }
	//
	// pCommandBars->SetToolBarClass(RUNTIME_CLASS(CSystemToolBar));
	//
	// CXTPToolBar* pToolBar = pCommandBars->Add("New ToolBar", xtpBarTop);
	// ASSERT_KINDOF(CSystemToolBar, pToolBar);
	// </code>
	// See Also:
	//     SetMenuBarClass, SetPopupBarClass
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetToolBarClass(CRuntimeClass* pToolBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default dock bar class.
	// Parameters:
	//     pDockBarClass - Runtime class of dock bars to be added.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetDockBarClass(CRuntimeClass* pDockBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default menu bar class.
	// Parameters:
	//     pMenuBarClass - Runtime class of menu bars to be added.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetMenuBarClass(CRuntimeClass* pMenuBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default pop-up bar class.
	// Parameters:
	//     pPopupBarClass - Runtime class of pop-up bars to be added.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetPopupBarClass(CRuntimeClass* pPopupBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default pop-up toolbar class.
	// Parameters:
	//     pPopupToolBarClass - Runtime class of pop-up toolbars to be added.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetPopupToolBarClass(CRuntimeClass* pPopupToolBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the index of the specified toolbar.
	// Parameters:
	//     pToolBar - Pointer to a CXTPToolBar object.
	// Returns:
	//     The index of the matching item.
	//-----------------------------------------------------------------------
	int FindIndex(CXTPToolBar* pToolBar) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to close all pop-up windows.
	//-----------------------------------------------------------------------
	void ClosePopups() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a reference to the status bar object.
	// Returns:
	//     Pointer to the CXTPStatusBar object.
	//-----------------------------------------------------------------------
	CXTPStatusBar* GetStatusBar() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to float a CXTPToolBar.
	// Parameters:
	//     pBar - Pointer to a valid CXTPToolBar object.
	// Returns:
	//     TRUE if the CXTPToolBar was successfully floated, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL FloatCommandBar(CXTPToolBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store state information about the
	//     frame window's control bars in a CXTPDockState object.
	// Parameters:
	//     state - Contains the current state of the frame window's control
	//             bars upon return.
	//-----------------------------------------------------------------------
	void GetDockState(CXTPDockState& state);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to apply state information stored in a
	//     CXTPDockState object to the frame window's control bars.
	// Parameters:
	//     state - Apply the stored state to the frame window's control bars.
	//-----------------------------------------------------------------------
	void SetDockState(CXTPDockState& state);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a toolbar is added.
	// Parameters:
	//     pToolBar - Pointer to the toolbar.
	//-----------------------------------------------------------------------
	virtual void OnToolBarAdded(CXTPToolBar* pToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a toolbar is removed.
	// Parameters:
	//     pToolBar - Pointer to the toolbar.
	//-----------------------------------------------------------------------
	virtual void OnToolBarRemoved(CXTPToolBar* pToolBar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to add a custom toolbar.
	// Parameters:
	//     lpcstrCaption - Title of the toolbar.
	//     nID - Toolbar ID.
	//     bTearOffBar - TRUE if toolbar is tear-off.
	// Returns:
	//     A CXTPToolBar object if successful; otherwise NULL.
	// See Also:
	//     Add
	//-----------------------------------------------------------------------
	virtual CXTPToolBar* AddCustomBar(LPCTSTR lpcstrCaption, UINT nID, BOOL bTearOffBar = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the tracking mode of a command bar changes.
	// Parameters:
	//     pCommandBar - Pointer to the command bar whose mode has changed.
	//     bMode - The new mode that the commandbar changed to.
	//-----------------------------------------------------------------------
	virtual void OnTrackingModeChanged(CXTPCommandBar* pCommandBar, int bMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the CXTPMouseManager pointer for command bars.
	// Returns:
	//     Pointer to a CXTPMouseManager object.
	// See Also:
	//     GetKeyboardManager
	//-----------------------------------------------------------------------
	CXTPMouseManager* GetMouseManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the GetKeyboardManager pointer for command bars.
	// Returns:
	//     Pointer to a GetKeyboardManager object.
	// See Also:
	//     CXTPMouseManager
	//-----------------------------------------------------------------------
	CXTPKeyboardManager* GetKeyboardManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user press Alt + char.
	// Parameters:
	//     chAccel - Character that was pressed.
	// Returns:
	//     TRUE if command bars processed this character, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnFrameAccel(UINT chAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the Help identifier of a control under the mouse cursor.
	// Returns:
	//     Identifier of the control under the mouse cursor.
	// See Also:
	//     CXTPControl::SetHelpId
	//-----------------------------------------------------------------------
	int GetHelpTrackingId() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to show the keyboard cues of controls.
	// Parameters:
	//     bShow - TRUE to show keyboard cues.
	//-----------------------------------------------------------------------
	void ShowKeyboardCues(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the actions list for command bars.
	//-----------------------------------------------------------------------
	CXTPControlActions* GetActions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to let command bars automatically add actions for new commands.
	//-----------------------------------------------------------------------
	void EnableActions(BOOL bEnableActions = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the EnableActions method was called.
	// Returns:
	//     TRUE if the EnableActions method was called, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsActionsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create a new action for a specified ID.
	// Parameters:
	//     nId - Identifier of the action to create.
	//-----------------------------------------------------------------------
	CXTPControlAction* CreateAction(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find an action for a specified ID.
	// Parameters:
	//     nId - Identifier of the action to find.
	//-----------------------------------------------------------------------
	CXTPControlAction* FindAction(int nId) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable Office 2007 frame.
	// Parameters:
	//     bEnable - TRUE to enable Office 2007 frame,
	//               FALSE to disable Office 2007 frame.
	// See Also:
	//     IsFrameThemeEnabled, EnableToolbarComposition
	//-----------------------------------------------------------------------
	void EnableFrameTheme(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if Office 2007 frame is enabled.
	// Returns:
	//     TRUE if Office 2007 frame is enabled, otherwise FALSE.
	// See Also:
	//     IsFrameThemeEnabled, EnableToolbarComposition
	//-----------------------------------------------------------------------
	BOOL IsEnableFrameTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable Office 2007 frame and toolbar composition.
	// Parameters:
	//     bEnable - TRUE to enable Office 2007 frame and toolbar composition,
	//               FALSE to disable Office 2007 frame and toolbar composition.
	// See Also:
	//     IsFrameThemeEnabled, EnableFrameTheme
	//-----------------------------------------------------------------------
	void EnableToolbarComposition(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets/sets the CXTPCommandBarsFrameHook hook window used to skin the frame.
	// See Also:
	//     EnableFrameTheme
	//-----------------------------------------------------------------------
	CXTPCommandBarsFrameHook* GetFrameHook() const;
	void SetFrameHook(CXTPCommandBarsFrameHook* pFrameHook); // <combine
															 // CXTPCommandBars::GetFrameHook@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Vista Glass Effect is enabled for Ribbon Bar.
	// Returns:
	//     TRUE if Vista Glass Effect is enabled for Ribbon Bar, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDwmEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set a custom font height for command bars.
	// Parameters:
	//     nFontHeight - Font height to set.
	//-----------------------------------------------------------------------
	void SetFontHeight(int nFontHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the font height used for Ribbon Bar.
	// Returns:
	//     The font height used for Ribbon Bar.
	//-----------------------------------------------------------------------
	int GetFontHeight() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// Implementation of Command Bars
	virtual BOOL SaveCommandBarList(CXTPPropExchange* pPX, CXTPCommandBarList* pCommandBarList);
	virtual BOOL LoadCommandBarList(CXTPPropExchange* pPX, CXTPCommandBarList* pCommandBarList);
	virtual void GenerateCommandBarList(CXTPCommandBarList* pCommandBarList,
										XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);
	virtual void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList, BOOL bSilent = FALSE);
	void SerializeCommandBars(CArchive& ar);
	void SaveDockBarsState(LPCTSTR lpszProfileName);
	void LoadDockBarsState(LPCTSTR lpszProfileName);
	void SerializeDockBarsState(CXTPPropExchange* pPX);
	void _GetAddOrRemovePopup(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar);
	void _LoadControlsPart(CFile& file, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);
	BOOL GetControlsFileName(CString& strFileName, LPCTSTR lpszProfileName);
	CDocTemplate* FindDocTemplate(CMDIChildWnd* pChild);
	void DelayRedrawCommandBars();
	int GetNextVisible(long nIndex, int nDirection) const;
	CXTPControl* FindAccel(CXTPCommandBar* pCommandBar, UINT chAccel, BOOL& bSelectOnly) const;
	CXTPControl* _GetNextControl(CXTPControls*& pControls, int nIndex) const;
	void IdleRecalcLayout();
	void SetLayoutRTL(CWnd* pWnd, BOOL bRTLLayout);
	CString GetIsolatedFileName(const CString& strPrifileName);

	//}}AFX_CODEJOCK_PRIVATE

private:
	virtual void OnAmbientPropertyChanged(CXTPApplication* pApplication,
										  XTPApplicationAmbientProperty nProperty);

public:
	CXTPControls* m_pDesignerControls; // Designer controls if used.
	BOOL m_bIgnoreShiftMenuKey;		   // if set menus will ignore Shift+Alt key combinations
	BOOL m_bDesignerMode;			   // TRUE if command bars in designer mode.

	//===========================================================================
	// Summary:
	//     Keyboard tips information.
	//===========================================================================
	struct KEYBOARDTIPS
	{
		CArray<CXTPCommandBarKeyboardTip*, CXTPCommandBarKeyboardTip*> arr; // Keyboard tips
																			// collection
		CXTPCommandBar* pBar; // Owner of keyboard tips
		int nLevel;			  // Keyboard tip level (for RibbonBar)
		int nKey;			  // Keys was pressed already.
	} m_keyboardTips;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to show keyboard tips for the commandbar.
	// Parameters:
	//     pCommandBar - Owner of keyboard tips.
	//     nLevel - Keyboard tip level (for RibbonBar).
	// See Also:
	//     HideKeyboardTips
	//-----------------------------------------------------------------------
	void ShowKeyboardTips(CXTPCommandBar* pCommandBar, int nLevel = 1);

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all keyboard tips.
	// See Also:
	//     ShowKeyboardTips
	//-------------------------------------------------------------------------
	void HideKeyboardTips();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to create a keyboard tips collection for command bar.
	// Parameters:
	//     pCommandBar - Owner of keyboard tips.
	// See Also:
	//     IsKeyboardTipsVisible
	//-------------------------------------------------------------------------
	virtual void CreateKeyboardTips(CXTPCommandBar* pCommandBar);

protected:
	CXTPCustomizeDropSource* m_pDropSource; // Customize helper class.

	CXTPPaintManager* m_pPaintManager; // Paint manager.
	CXTPImageManager* m_pImageManager; // Image manager

	CCmdTarget* m_pDragSelected; // Customize control.
	int m_nDefaultMenuID;		 // Default menu resource.

	BOOL m_bCustomizeMode;		// Customize mode.
	BOOL m_bCustomizeAvail;		// TRUE if customize available.
	BOOL m_bEnableActions;		// True if actions was enabled
	BOOL m_bQuickCustomizeMode; // TRUE if command bars in quick customization mode
	BOOL m_bDefaultResetCustomizedToolbarChoiceEnabled; // TRUE if default choice is enabled.
	BOOL m_bDefaultResetCustomizedToolbarChoice;		// TRUE if default choice is Yes.

	CString m_strControlsFileName; // File name to be used to store user customization.

	CXTPCommandBarsOptions* m_pOptions; // Command bars' options

	CXTPCommandBarsContextMenus* m_pContextMenus; // Context menu collection

	CArray<CXTPToolBar*, CXTPToolBar*> m_arrBars; // Toolbars list.
	CXTPDockBar* m_pDocks[4];					  // DockBars list
	CWnd* m_pFrame;								  // Parent frame of command bars
	CXTPToolTipContext* m_pToolTipContext;		  // Tooltip Context.

	mutable CXTPMouseManager* m_pMouseManager;		 // Mouse manager of command bars
	mutable CXTPKeyboardManager* m_pKeyboardManager; // Keyboard manager of command bars

	static CRuntimeClass* m_pToolBarClass;		// Default toolbar class
	static CRuntimeClass* m_pMenuBarClass;		// Default menubar class
	static CRuntimeClass* m_pPopupBarClass;		// Default pop-up bar class
	static CRuntimeClass* m_pPopupToolBarClass; // Default pop-up bar class
	static CRuntimeClass* m_pDockBarClass;		// Default dockbar class
	BOOL m_bKeyboardCuesVisible;				// TRUE if menu underlines is currently visible
	CXTPShortcutManager* m_pShortcutManager;	// Shortcut Manager of command bars.

	int m_nIDHelpTracking; // Selected control.

	CXTPControlActions* m_pActions; // Actions collection

	CXTPCommandBarsFrameHook* m_pFrameHook; // Frame hook pointer for enabled frame theme.
	BOOL m_bInternalFrameHook;				// TRUE if frame hook object is owned internally.

#	ifdef _XTP_COMMANDBARS_ACTIVEX
public:
	CXTPRecentFileList* m_pRecentFileList;
#	endif

private:
	BOOL m_bRecalcLayout;
	BOOL m_bUseKeyboardCues;
	BOOL m_bAllCaps;
	BOOL m_bRightToLeft;
	BOOL m_bPreviewMode;
	BOOL m_bBeingDestructed; // TRUE if class instance is being destructructed

	static const UINT m_nRedrawMsg;

private:
	DECLARE_DYNCREATE(CXTPCommandBars)

	friend class CXTPCommandBar;
	friend class CXTPToolBar;
	friend class CXTPDockContext;
	friend class CXTPDockState;
	friend class CXTPDockBar;
	friend class CXTPControlToolbars;
	friend class CXTPControl;
	friend class CCommandBarsCtrl;
	friend class CXTPCustomizeToolbarsPage;
	friend class CXTPPopupBar;
	friend class CXTPPopupToolBar;
	friend class CXTPMenuBar;
	friend class CXTPCommandBarsOptions;
	friend class CCommandBarsSite;
	friend class CXTPDialogEx;
#	if (_MSC_VER > 1200)
	friend class CXTPDHtmlDialogEx;
#	endif
	friend class CXTPRibbonBar;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPCommandBars::IsAllCaps() const
{
	return m_bAllCaps;
}

AFX_INLINE void CXTPCommandBars::SetAllCaps(BOOL bAllCaps)
{
	m_bAllCaps = bAllCaps;
}

AFX_INLINE CWnd* CXTPCommandBars::GetSite() const
{
	return m_pFrame;
}

AFX_INLINE CXTPDockBar* CXTPCommandBars::GetDockBar(XTPBarPosition barPosition) const
{
	ASSERT(barPosition >= 0 && barPosition < 4);
	return barPosition >= 0 && barPosition < 4 ? m_pDocks[barPosition] : NULL;
}

AFX_INLINE int CXTPCommandBars::GetCount() const
{
	return (int)m_arrBars.GetSize();
}

AFX_INLINE CXTPCustomizeDropSource* CXTPCommandBars::GetDropSource() const
{
	return m_pDropSource;
}

AFX_INLINE BOOL CXTPCommandBars::IsCustomizeAvail() const
{
	return m_bCustomizeAvail;
}

AFX_INLINE void CXTPCommandBars::EnableCustomization(BOOL bCustomizeAvail)
{
	m_bCustomizeAvail = bCustomizeAvail;
}

AFX_INLINE CXTPCommandBarsOptions* CXTPCommandBars::GetCommandBarsOptions() const
{
	return m_pOptions;
}

AFX_INLINE void CXTPCommandBars::SetCustomizationDataFileName(LPCTSTR lpszFile)
{
	m_strControlsFileName = lpszFile;
}

AFX_INLINE void CXTPCommandBars::EnableDefaultResetCustomizedToolbarChoice(BOOL bEnable,
																		   BOOL bDefaultYes)
{
	m_bDefaultResetCustomizedToolbarChoiceEnabled = bEnable;
	m_bDefaultResetCustomizedToolbarChoice		  = bDefaultYes;
}

AFX_INLINE void CXTPCommandBars::SetToolBarClass(CRuntimeClass* pToolBarClass)
{
	m_pToolBarClass = pToolBarClass;
}

AFX_INLINE void CXTPCommandBars::SetMenuBarClass(CRuntimeClass* pMenuBarClass)
{
	m_pMenuBarClass = pMenuBarClass;
}

AFX_INLINE void CXTPCommandBars::SetDockBarClass(CRuntimeClass* pDockBarClass)
{
	m_pDockBarClass = pDockBarClass;
}

AFX_INLINE void CXTPCommandBars::SetPopupBarClass(CRuntimeClass* pPopupBarClass)
{
	m_pPopupBarClass = pPopupBarClass;
}

AFX_INLINE void CXTPCommandBars::SetPopupToolBarClass(CRuntimeClass* pPopupToolBarClass)
{
	m_pPopupToolBarClass = pPopupToolBarClass;
}

AFX_INLINE int CXTPCommandBars::GetHelpTrackingId() const
{
	return m_nIDHelpTracking;
}

AFX_INLINE CXTPControlActions* CXTPCommandBars::GetActions() const
{
	return m_pActions;
}

AFX_INLINE BOOL CXTPCommandBars::IsActionsEnabled() const
{
	return this == NULL ? FALSE : m_bEnableActions;
}

AFX_INLINE CXTPCommandBarsContextMenus* CXTPCommandBars::GetContextMenus() const
{
	return m_pContextMenus;
}

AFX_INLINE CXTPCommandBarsFrameHook* CXTPCommandBars::GetFrameHook() const
{
	return m_pFrameHook;
}

AFX_INLINE void CXTPCommandBars::SetFrameHook(CXTPCommandBarsFrameHook* pFrameHook)
{
	m_pFrameHook = pFrameHook;
	if (NULL == m_pFrameHook)
		m_bInternalFrameHook = FALSE;
}

AFX_INLINE BOOL CXTPCommandBars::IsQuickCustomizeMode() const
{
	return m_bQuickCustomizeMode;
}

AFX_INLINE BOOL CXTPCommandBars::IsCustomizeMode() const
{
	return m_bCustomizeMode;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCOMMANDBARS_H__)
