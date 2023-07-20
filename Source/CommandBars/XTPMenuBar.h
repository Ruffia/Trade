// XTPMenuBar.h : interface for the CXTPMenuBar class.
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
#if !defined(__XTPMENUBAR_H__)
#	define __XTPMENUBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	define XTP_IDR_MENUBAR 1 // Menu Bar

class CXTPCommandBars;

//===========================================================================
// Summary:
//     CXTPMenuBarMDIMenuInfo structure used to describe the
//     parameters of application MDI menus.
//===========================================================================
class _XTP_EXT_CLASS CXTPMenuBarMDIMenuInfo : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMenuBarMDIMenuInfo object.
	// Parameters:
	//     pCommandBars - Parent commandbars.
	//     nIDResource - Template identifier.
	//-----------------------------------------------------------------------
	CXTPMenuBarMDIMenuInfo(CXTPCommandBars* pCommandBars, UINT nIDResource);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMenuBarMDIMenuInfo object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPMenuBarMDIMenuInfo();

public:
	CXTPControls* m_pControls; // Menu controls.
	CString m_strTitle;		   // Title of MDI menu.
	CString m_strDescription;  // Description of MDI menu.
	HICON m_hIcon;			   // Menu Icon.
	UINT m_nIDResource;		   // Template identifier.
	BOOL m_bChanged;		   // TRUE if menu was changed.

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMenuBarMDIMenuInfo);
	LPDISPATCH OleGetControls();
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

class CXTPMenuBar;

//===========================================================================
// Summary:
//     CXTPMenuBarMDIMenus represents a collection of MDI menus for CXTPMenuBar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMenuBarMDIMenus : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMenuBarMDIMenus object.
	// Parameters:
	//     pMenuBar - Parent CXTPMenuBar.
	//-----------------------------------------------------------------------
	CXTPMenuBarMDIMenus(CXTPMenuBar* pMenuBar);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMenuBarMDIMenus object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPMenuBarMDIMenus();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the number of MDI menus in the collection.
	// Returns:
	//     The number of MDI menus in the collection.
	//-------------------------------------------------------------------------
	int GetCount() const;

	//{{AFX_CODEJOCK_PRIVATE
	void SetAt(UINT nIDResource, CXTPMenuBarMDIMenuInfo* pInfo);
	CXTPMenuBarMDIMenuInfo* Lookup(UINT nIDResource) const;
	void Remove(UINT nIDResource);
	void RemoveAll();
	POSITION GetStartPosition() const;
	void GetNextMenu(POSITION& pos, CXTPMenuBarMDIMenuInfo*& pInfo) const;
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()

	DECLARE_OLETYPELIB_EX(CXTPMenuBarMDIMenus);
	LPDISPATCH OleGetItem(UINT nID);
	LPDISPATCH OleAdd(UINT nID, LPCTSTR lpszTitle, LPCTSTR lpszDescription);
	DECLARE_ENUM_VARIANTLIST(CXTPMenuBarMDIMenus);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CMap<UINT, UINT, CXTPMenuBarMDIMenuInfo*, CXTPMenuBarMDIMenuInfo*> m_mapMenus; // Menu map.
	CXTPMenuBar* m_pMenuBar; // Parent Menu Bar.
};

//===========================================================================
// Summary:
//     CXTPMenuBar is a CXTPToolBar derived class.
//     It represents a menu bar of a frame window.
//===========================================================================
class _XTP_EXT_CLASS CXTPMenuBar : public CXTPToolBar
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMenuBar object.
	//-----------------------------------------------------------------------
	CXTPMenuBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMenuBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPMenuBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a menu from the resource.
	// Parameters:
	//     nIDResource - Menu identifier to load.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL LoadMenuBar(UINT nIDResource);

	//------------------------------------------------------------------------
	// Summary:
	//     Destroy all MDI menus.
	//------------------------------------------------------------------------
	void FreeMDIMenus();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will refresh the menus.
	//-----------------------------------------------------------------------
	void RefreshMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the identifier of the currently visible menu.
	// Returns:
	//     The identifier of the currently visible menu.
	//-----------------------------------------------------------------------
	UINT GetCurrentMenuResource() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the identifier of the default menu.
	// Returns:
	//     The identifier of the default menu.
	//-----------------------------------------------------------------------
	UINT GetDefaultMenuResource() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the collection of application MDI menus.
	// Returns:
	//     A pointer to the collection of application MDI menus.
	//-----------------------------------------------------------------------
	CXTPMenuBarMDIMenus* GetMDIMenus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to switch to an MDI menu from the MDI menus collection.
	// Parameters:
	//     nIDMenu - Menu identifier to switch.
	//     pMenu - Menu to switch.
	// See Also:
	//     SwitchActiveMenu
	//-----------------------------------------------------------------------
	virtual void SwitchMDIMenu(UINT nIDMenu, CMenu* pMenu = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to manually load an MDI menu in application initialization.
	// Parameters:
	//     nIDResource - Template identifier
	// Returns:
	//     A pointer to a newly created menu information object or a pointer to
	//     the existing menu information object if it has been previously added.
	//-----------------------------------------------------------------------
	CXTPMenuBarMDIMenuInfo* AddMDIMenu(UINT nIDResource);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to switch the menu to the active document menu.
	// See Also:
	//     GetActiveDocTemplate, SwitchMDIMenu
	//-------------------------------------------------------------------------
	void SwitchActiveMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to reset the controls to their original states.
	// Parameters:
	//     bShowWarningMessage - TRUE to show warning message.
	//-----------------------------------------------------------------------
	virtual void Reset(BOOL bShowWarningMessage);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during
	//     message reflection.
	// Parameters:
	//     hWnd     - Window handle message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam   - Specifies additional message-dependent information.
	//     lParam   - Specifies additional message-dependent information.
	//     lResult  - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	//-----------------------------------------------------------------------
	int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to create hooks for the main window.
	//-------------------------------------------------------------------------
	void SetupHook();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to create the menu for the main window.
	//-------------------------------------------------------------------------
	void SetupMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the handle to the active MDI child window.
	// Parameters:
	//     bMaximized -  If this parameter is not NULL, it is a pointer to a value
	//                   that indicates the maximized state of the MDI child window.
	// Returns:
	//     The handle to the active MDI child window.
	//-----------------------------------------------------------------------
	HWND GetActiveMdiChildWnd(BOOL* bMaximized = NULL);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to update the active MDI window hooks.
	//-------------------------------------------------------------------------
	void SyncActiveMdiChild();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to update the system buttons of the menu bar.
	//-------------------------------------------------------------------------
	virtual void RefreshSysButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to switch the current MDI menu.
	// Parameters:
	//     hMenu - Active menu to select.
	//-----------------------------------------------------------------------
	void SwitchMDIMenu(HMENU hMenu);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to assign self-identifiers for the serialization process.
	// Parameters:
	//     nID             - Identifier to assign.
	//     pCommandBarList - List of CommandBars.
	//     pParam          - Address of an XTP_COMMANDBARS_PROPEXCHANGE_PARAM structure.
	//-----------------------------------------------------------------------
	void GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList,
								XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method during the serialization process to restore pop-ups from
	//     a list of command bars.
	// Parameters:
	//     pCommandBarList - List of command bars.
	//-----------------------------------------------------------------------
	void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the toolbar has changed and must be saved.
	// Returns:
	//     TRUE if the toolbar has changed and must be saved, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ShouldSerializeBar();

	virtual BOOL OnFrameMouseWheel(BOOL bForward);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPMenuBar)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPMenuBar);
	afx_msg LPDISPATCH OleGetMDIMenus();
	afx_msg void OleSwitchMDIMenu(int nId);
//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	void AddSysButton(CXTPControl* pControl, int nId, LPCTSTR lpszParameter, int nBefore = -1);
	BOOL IsMDIMenusChanged();
	BOOL IsOleDocumentActive(HWND hWndActiveChild) const;

protected:
	BOOL m_bMdiApp;			// TRUE if parent frame is CMDIFrameWnd derived.
	HWND m_hWndActiveChild; // Active MDI window handle.
	HMENU m_hMenuDefault;   // Default menu handle.

	UINT m_nIDResource;				  // Resource of default menu.
	UINT m_nIDResourceCurrent;		  // Currently active menu.
	CXTPMenuBarMDIMenus* m_pMDIMenus; // MDI menus.

private:
	BOOL m_bDirtyMenu;

private:
	DECLARE_XTP_COMMANDBAR(CXTPMenuBar)

	class CControlMDIButton;
	class CControlMDISysMenuPopup;

	friend class CXTPCommandBars;
	friend class CControlMDIButton;
	friend class CCommandBarsSite;
	friend class CCommandBarsCtrl;
};

AFX_INLINE UINT CXTPMenuBar::GetCurrentMenuResource() const
{
	return m_nIDResourceCurrent;
}
AFX_INLINE UINT CXTPMenuBar::GetDefaultMenuResource() const
{
	return m_nIDResource;
}
AFX_INLINE CXTPMenuBarMDIMenus* CXTPMenuBar::GetMDIMenus() const
{
	return m_pMDIMenus;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPMENUBAR_H__)
