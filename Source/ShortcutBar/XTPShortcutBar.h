// XTPShortcutBar.h interface for the CXTPShortcutBar class.
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
#if !defined(__XTPSHORTCUTBAR_H__)
#	define __XTPSHORTCUTBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPShortcutBarAnimation;
class CXTPShortcutBarPopup;
class CXTPImageManagerIcon;

//-----------------------------------------------------------------------
// Summary:
//     XTP_SBN_SELECTION_CHANGED is used to indicate that the selection
//     has changed in the shortcut bar.
// Remarks:
//     XTP_SBN_SELECTION_CHANGED is sent in an XTPWM_SHORTCUTBAR_NOTIFY
//     message to the owner window when the selection has changed.
//
//     The selection changes either when a user clicks on a shortcut bar item
//     or when the items are navigated with the arrow keys.
// Example:
//     See example of XTPWM_SHORTCUTBAR_NOTIFY
// See Also:
//     XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_RCLICK
//-----------------------------------------------------------------------
const UINT XTP_SBN_SELECTION_CHANGING = 1;

//-----------------------------------------------------------------------
// Summary:
//     XTP_SBN_SELECTION_CHANGING is used to indicate that the value
//     for the selection is currently changing.
// Remarks:
//     XTP_SBN_SELECTION_CHANGING is sent in an XTPWM_SHORTCUTBAR_NOTIFY
//     message to the owner window when the selection is in the process of changing.
//
//     The selection changes either when a user clicks on a shortcut bar item
//     or when the items are navigated with the arrow keys.
// See Also:
//     XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_RCLICK
//-----------------------------------------------------------------------
const UINT XTP_SBN_SELECTION_CHANGED = 3;

//-----------------------------------------------------------------------
// Summary:
//     XTP_SBN_RCLICK is used to indicate that the user has pressed the
//     right mouse button on a shortcut bar item.
// Remarks:
//     XTP_SBN_RCLICK is sent in an XTPWM_SHORTCUTBAR_NOTIFY message to the
//     owner window when the user right clicks on a shortcut bar item.
// Example:
//     See example of XTPWM_SHORTCUTBAR_NOTIFY
// See Also:
//     XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_SELECTION_CHANGING
//-----------------------------------------------------------------------
const UINT XTP_SBN_RCLICK = 2;

//-----------------------------------------------------------------------
// Summary:
//     XTP_SBN_MINIMIZEBUTTONCLICKED is used to indicate that the user has
//     pressed the minimize button inside a pane caption.
// Remarks:
//     XTP_SBN_MINIMIZEBUTTONCLICKED is sent in an XTPWM_SHORTCUTBAR_NOTIFY
//     message to the owner window when the user clicks the minimize button.
// Example:
//     See example of XTPWM_SHORTCUTBAR_NOTIFY
// See Also:
//     XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_SELECTION_CHANGING, CXTPShortcutBar::ShowMinimizeButton
//-----------------------------------------------------------------------
const UINT XTP_SBN_MINIMIZEBUTTONCLICKED = 4;

//-----------------------------------------------------------------------
// Summary:
//     The XTPWM_SHORTCUTBAR_NOTIFY message is sent to the CXTPShortcutBar owner window
//     whenever an action occurs within the shortcut bar.
// Parameters:
//     nAction -  wParam value that specifies a shortcut bar value
//                that indicates the user's request.
//     pItem    - lParam value that points to a CXTPShortcutBarItem object
//                that contains information about the specified item.
//                This pointer should <b>never</b> be NULL.
// Remarks:
//     nAction parameter can be one of the following values:
//     * <b>XTP_SBN_SELECTION_CHANGING</b>: Indicates that the selection has
//                                          changed in the shortcut bar.
//     * <b>XTP_SBN_RCLICK</b>: Indicates the user pressed the right mouse button
//                              on the shortcut bar item.
// Returns:
//     TRUE if the application should process this message, otherwise FALSE.
// Example:
//     Here is an example of how an application would process an
//     XTPWM_SHORTCUTBAR_NOTIFY message:
// <code>
//
// BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(XTPWM_SHORTCUTBAR_NOTIFY, OnShortcutBarNotify)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnShortcutBarNotify(WPARAM wParam, LPARAM lParam)
// {
//      switch (wParam)
//      {
//          case XTP_SBN_SELECTION_CHANGING:
//              {
//                  CXTPShortcutBarItem* pItem = (CXTPShortcutBarItem*)lParam;
//                  TRACE(_T("Selection Changing. Item.Caption = %s\n"), pItem->GetCaption());
//
//                  // TODO: You can return -1 to ignore changing
//
//              }
//              return TRUE;
//          case XTP_SBN_RCLICK:
//              {
//                  CXTPShortcutBarItem* pItem =
//                  m_wndShortcutBar.HitTest(XTP_POINT_FROM_LPARAM(lParam)); if (pItem)
//                  {
//                      TRACE(_T("RClick. Item.ID = %i\n"), pItem->GetID());
//
//                      CMenu mnu;
//                      mnu.LoadMenu(IDR_POPUP_MENU);
//
//                      m_wndShortcutBar.ClientToScreen(&point);
//
//                      CXTPCommandBars::TrackPopupMenu(mnu.GetSubMenu(0), 0, point.x, point.y,
//                      AfxGetMainWnd());
//
//                  }
//              }
//              return TRUE;
//
//
//      }
//      return 0;
//  }
// </code>
// See Also:
//     XTP_SBN_SELECTION_CHANGING, XTP_SBN_RCLICK, CXTPShortcutBar
//-----------------------------------------------------------------------
const UINT XTPWM_SHORTCUTBAR_NOTIFY = (WM_USER + 9190 + 1);

class CXTPShortcutBar;
class CXTPImageManager;
class CXTPToolTipContext;

//===========================================================================
// Summary:
//     CXTPShortcutBarItem is a base class that represents
//     an item for a shortcut bar control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarItem : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPShortcutBarItem);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarItem object.
	// Parameters:
	//     pShortcutBar - Pointer to the parent CXTPShortcutBar object.
	//     nID          - Identifier of the item.
	//     pWnd         - Pointer to the client window of the item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem(CXTPShortcutBar* pShortcutBar, int nID, CWnd* pWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the identifier for the item.
	// Parameters:
	//     nID - Identifier to be set.
	//-----------------------------------------------------------------------
	void SetID(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the item.
	// Returns:
	//     The identifier of the item.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the identifier for the icon.
	// Parameters:
	//     nId - Identifier to be set.
	//-----------------------------------------------------------------------
	void SetIconId(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the icon.
	// Returns:
	//     The identifier of the icon.
	//-----------------------------------------------------------------------
	int GetIconId() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the item.
	// Parameters:
	//     strCaption - Caption to be set.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the item.
	// Returns:
	//     The caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the visible state of the item.
	// Parameters:
	//     bVisible - TRUE to set the item to visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the item.
	// Returns:
	//     TRUE if the item is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text for the item.
	// Parameters:
	//     strTooltip - Tooltip text to be set.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR strTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the item.
	// Returns:
	//     The tooltip text of the item.
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the expanded state of the item.
	// Returns:
	//     TRUE if the item is expanded, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is an expand button.
	// Returns:
	//     TRUE if the item is an expand button, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemExpandButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent shortcut bar.
	// Returns:
	//     A pointer to the parent shortcut bar.
	//-----------------------------------------------------------------------
	CXTPShortcutBar* GetShortcutBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the item.
	// Returns:
	//     The bounding rectangle of the item.
	//-----------------------------------------------------------------------
	CRect GetItemRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - 32-bit value to associate with the item.
	//-----------------------------------------------------------------------
	void SetItemData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the application-supplied, 32-bit value associated with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the image of the item.
	// Parameters:
	//     nWidth - Width of the icon to be retrieved
	//              (e.g. pass in a value of 16 to retrieve a 16x16 image).
	// Remarks:
	//     CXTPImageManagerIcon can have multiple image sizes for the same item.
	//     For example, the shortcut bar uses both 24x24 and 16x16 images depending
	//     on if the item is displayed in the extended shortcut list or not.
	// Returns:
	//     A pointer to the image of the item.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage(int nWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the hidden state of the item.
	// Remarks:
	//     An item is hidden when there are more items than can be displayed
	//     in the shortcut bar. As such, items that were hidden can become
	//     visible when the shortcut bar is expanded.
	// Returns:
	//     TRUE if the item is hidden, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHidden() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected state of the item.
	// Remarks:
	//     If CXTPShortcutBar::m_bSingleSelection is set to FALSE, then multiple items
	//     can be selected at a given time. However, only a single item can have focus.
	// Returns:
	//     TRUE if the item is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected state for the item.
	// Parameters:
	//     bSelected - TRUE to set the item to selected, FALSE otherwise.
	// Remarks:
	//     If CXTPShortcutBar::m_bSingleSelection is set to FALSE, then multiple items
	//     can be selected at a given time. However, only a single item can have focus.
	//-----------------------------------------------------------------------
	void SetSelected(BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the child window of the item.
	// Remarks:
	//     The child window of the item is visible in the client part
	//     of the shortcut bar when the item is selected.
	// Returns:
	//     A pointer to the child window of the item.
	//-----------------------------------------------------------------------
	CWnd* GetClientWindow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the child window for the item.
	// Parameters:
	//     pWnd - Pointer to the child window to be set.
	// Remarks:
	//     The child window of the item is visible in the client part
	//     of the shortcut bar when the item is selected.
	//-----------------------------------------------------------------------
	void SetClientWindow(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the enabled state of the item.
	// Returns:
	//     TRUE if the item is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the enabled state for the item.
	// Parameters:
	//     bEnabled - TRUE to set the item to enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

protected:
	CString m_strCaption;			 // Caption of the item.
	CString m_strTooltip;			 // Tooltip of the item.
	int m_nID;						 // Item's identifier.
	CRect m_rcItem;					 // Bounding rectangle of the item.
	BOOL m_bVisible;				 // TRUE if the item is visible.
	BOOL m_bExpanded;				 // TRUE if the item is expanded.
	BOOL m_bHidden;					 // TRUE if the item is hidden.
	HWND m_hwndChild;				 // Child window associated with the item.
	BOOL m_bExpandButton;			 // TRUE if the item is an expand button.
	BOOL m_bNavigateButton;			 // TRUE if the item is a navigate button.
	DWORD_PTR m_dwData;				 // The 32-bit value associated with the item.
	CXTPShortcutBar* m_pShortcutBar; // Parent CXTPShortcutBar class.
	BOOL m_bSelected;				 // TRUE if the item is selected.
	int m_nIconId;					 // Identifier of the item's image.
	BOOL m_bEnabled;				 // TRUE if the item is enabled.

private:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPShortcutBarItem)
public:
	enum
	{
		dispidCaption	= 1L,
		dispidTooltip	= 2L,
		dispidId		 = 3L,
		dispidIconIndex  = 4L,
		dispidVisible	= 5L,
		dispidIconHandle = 6L,
	};
	afx_msg BSTR OleGetCaption();
	afx_msg BSTR OleGetTooltip();

	DECLARE_PROPERTY(IconHandle, int);
	DECLARE_PROPERTY(Handle, OLE_HANDLE);

	static CXTPShortcutBarItem* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/
	friend class CXTPShortcutBar;
};

//-----------------------------------------------------------------------
// Summary:
//     CXTPShortcutBar is a CWnd derived class used to implement
//     an Outlook 2003 shortcut bar control style.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPShortcutBar : public CWnd
{
	//-----------------------------------------------------------------------
	// Summary:
	//     Array of CXTPShortcutBarItem objects.
	//-----------------------------------------------------------------------
	typedef CArray<CXTPShortcutBarItem*, CXTPShortcutBarItem*> CShortcutArray;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBar object.
	//-----------------------------------------------------------------------
	CXTPShortcutBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the CXTPShortcutBar control.
	// Parameters:
	//     dwStyle    - Style of the shortcut bar control.
	//     rect       - Reference to the size and position of the shortcut bar control.
	//     pParentWnd - Pointer to the parent window of the shortcut bar control.
	//     nID        - Identifier of the shortcut bar control.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves state information from the registry or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a NULL-terminated string that specifies the
	//                       name of a section in the initialization file or a key in
	//                       the Windows registry where state information is stored.
	//-----------------------------------------------------------------------
	void LoadState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves state information to the  registry or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a NULL-terminated string that specifies the
	//                       name of a section in the initialization file or a key in
	//                       the Windows registry where state information is stored.
	//-----------------------------------------------------------------------
	void SaveState(LPCTSTR lpszProfileName);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified item.
	// Parameters:
	//     nID  - Identifier of the item to be added.
	//     pWnd - Pointer to the associated child window of the item.
	// Returns:
	//     A pointer to the newly added CXTPShortcutBarItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* AddItem(UINT nID, CWnd* pWnd = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item with a specified identifier.
	// Parameters:
	//     nID - Identifier of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPShortcutBarItem object if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* FindItem(int nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects a specified item.
	// Parameters:
	//     pItem - Pointer to the item to be selected.
	//-----------------------------------------------------------------------
	void SelectItem(CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the expand button.
	// Parameters:
	//     bShow - TRUE to show the expand button, FALSE to hide the expand button.
	//-----------------------------------------------------------------------
	void ShowExpandButton(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the expand button is shown/hidden.
	// Returns:
	//     TRUE if the expand button is shown, FALSE if the expand button is hidden.
	//-----------------------------------------------------------------------
	BOOL IsExpandButtonVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the item with a specified identifier.
	// Parameters:
	//     nID - Identifier of the item to remove.
	//-----------------------------------------------------------------------
	void RemoveItem(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items.
	//-----------------------------------------------------------------------
	void RemoveAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item at a specified index.
	// Parameters:
	//     nIndex - Zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPShortcutBarItem object if successul, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which item, if any, is located at a specified point.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the CXTPShortcutBarItem object at
	//     the specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* HitTest(CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum height for the client area.
	// Parameters:
	//     nMinHeight - Minimum height, in pixels, to be set.
	// Remarks:
	//     The default minimum height for the client area is 200 pixels.
	//-----------------------------------------------------------------------
	void SetMinimumClientHeight(int nMinHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the gripper.
	// Returns:
	//     The bounding rectangle of the gripper.
	//-----------------------------------------------------------------------
	CRect GetGripperRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of items.
	// Returns:
	//     The size of items.
	//-----------------------------------------------------------------------
	CSize GetItemSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size for items.
	// Parameters:
	//     szItem - Size to be set.
	//-----------------------------------------------------------------------
	void SetItemSize(CSize szItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of visible lines in the shortcut bar control.
	// Returns:
	//     The number of visible lines in the shortcut bar control.
	// See Also:
	//     SetExpandedLinesCount
	//-----------------------------------------------------------------------
	int GetExpandedLinesCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of visible lines in the shortcut bar control.
	// Returns:
	//     The height of visible lines in the shortcut bar control.
	// See Also:
	//     SetExpandedLinesHeight
	//-----------------------------------------------------------------------
	int GetExpandedLinesHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of visible lines in the shortcut bar control.
	// Parameters:
	//     nCount - Number to be set.
	// See Also:
	//     GetExpandedLinesCount
	//-----------------------------------------------------------------------
	void SetExpandedLinesCount(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height for visible lines in the shortcut bar control.
	// Parameters:
	//     nHeight - Height to be set.
	// See Also:
	//     SetExpandedLinesCount, GetExpandedLinesCount
	//-----------------------------------------------------------------------
	void SetExpandedLinesHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows resizing the client area.
	// Parameters:
	//     bAllowResize - TRUE to allow resizing the client area,
	//                    FALSE to disallow resizing the client area.
	// See Also:
	//     SetExpandedLinesCount
	//-----------------------------------------------------------------------
	void AllowGripperResize(BOOL bAllowResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the hot item in the shortcut bar control.
	// Returns:
	//     A pointer to the hot item in the shortcut bar control.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetHotItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected item in the shortcut bar control.
	// Returns:
	//     A pointer to the selected item in the shortcut bar control.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetSelectedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables single selection.
	// Parameters:
	//     bSingleSelection - TRUE to enable single selection,
	//                        FALSE to disable single selection.
	// Remarks:
	//     If single selection is enabled, then only one item can be
	//     selected in the shortcut bar control at a given time.
	//     If single selection is disabled, then multiple items can be
	//     selected in the shortcut bar control at a given time.
	// See Also:
	//     IsSingleSelection, SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	void SetSingleSelection(BOOL bSingleSelection = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if single selection is enabled/disabled.
	// Returns:
	//     TRUE if single selection is enabled,
	//     FALSE if single selection is disabled.
	// See Also:
	//     SetSingleSelection, SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	BOOL IsSingleSelection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the active item should be shown on top of controls.
	// Parameters:
	//     bActiveItemOnTop - TRUE to show the active item on
	//                        top of controls, FALSE otherwise.
	// See Also:
	//     IsShowActiveItemOnTop
	//-----------------------------------------------------------------------
	void ShowActiveItemOnTop(BOOL bActiveItemOnTop);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the active item is shown on top of controls.
	// Returns:
	//     TRUE if the active item is shown on top of controls, otherwise FALSE.
	// See Also:
	//     ShowActiveItemOnTop
	//-----------------------------------------------------------------------
	BOOL IsShowActiveItemOnTop() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows resizing the gripper by pixel.
	// Parameters:
	//     bAllowFreeResize - TRUE to allow resizing the gripper by pixel,
	//                        FALSE to disallow resizing the gripper by pixel.
	// See Also:
	//     IsShowActiveItemOnTop
	//-----------------------------------------------------------------------
	void AllowFreeResize(BOOL bAllowFreeResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if resizing the gripper by pixel is allowed/disallowed.
	// Returns:
	//     TRUE if resizing the gripper by pixel is allowed,
	//     FALSE if resizing the gripper by pixel is disallowed.
	// See Also:
	//     ShowActiveItemOnTop
	//-----------------------------------------------------------------------
	BOOL IsAllowFreeResize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow collapse items
	// Parameters:
	//     bAllowCollapse - TRUE to allow collapse items
	//-----------------------------------------------------------------------
	void AllowCollapse(BOOL bAllowCollapse);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the gripper.
	// Parameters:
	//     bShowGripper - TRUE to show the gripper, FALSE to hide the gripper.
	//-----------------------------------------------------------------------
	void ShowGripper(BOOL bShowGripper);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the pressed item in the shortcut bar control.
	// Returns:
	//     A pointer to the pressed item in the shortcut bar control.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetPressedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the client pane is shown/hidden.
	// Returns:
	//     TRUE if the client pane is shown, FALSE if the client pane is hidden.
	//-----------------------------------------------------------------------
	BOOL IsClientPaneVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the client pane.
	// Parameters:
	//     bVisible - TRUE to show the client pane, FALSE to hide the client pane.
	//-----------------------------------------------------------------------
	void SetClientPaneVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image manager for the shortcut bar control.
	// Parameters:
	//     pImageManager - Pointer to the image manager to be set.
	// Remarks:
	//     The image manager is used to hold all of the icons displayed
	//     in the shortcut bar control.
	// Example:
	// <code>
	//     CXTPImageManager* pImageManager = new CXTPImageManager();
	//     pImageManager->SetIcons(IDR_MAINFRAME);
	//     m_wndShortcutBar.SetImageManager(pImageManager);
	// </code>
	// See Also:
	//     GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image manager of the shortcut bar control.
	// Remarks:
	//     The image manager is used to hold all of the icons displayed
	//     in the shortcut bar control.
	// Returns:
	//     A pointer to the image manager of the shortcut bar control.
	// See Also:
	//     SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of items in the shortcut bar control.
	// Returns:
	//     The number of items in the shortcut bar control.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets right-to-left (RTL) mode.
	// Parameters:
	//     bRightToLeft - TRUE to set right-to-left (RTL) reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the shortcut bar client.
	// Returns:
	//     The bounding rectangle of the shortcut bar client.
	//-----------------------------------------------------------------------
	CRect GetClientPaneRect() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the theme for the shortcut bar control.
	// Parameters:
	//     paintTheme - Theme to be set; can be any of the
	//                  values listed in the Remarks section.
	// Remarks:
	//     paintTheme can be one of the following:
	//     * <b>xtpShortcutThemeOffice2000</b>: Enables Office 2000 style theme.
	//     * <b>xtpShortcutThemeOfficeXP</b>: Enables Office XP style theme.
	//     * <b>xtpShortcutThemeOffice2003</b>: Enables Office 2003 style theme.
	//     * <b>xtpTaskPanelThemeShortcutBarOffice2007</b>: Enables Office 2007 style theme.
	// See Also:
	//     GetCurrentTheme, SetCustomTheme, XTPShortcutBarPaintTheme
	//-----------------------------------------------------------------------
	void SetTheme(XTPShortcutBarPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a custom theme for the shortcut bar control.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPShortcutBarPaintManager object.
	// See Also:
	//     GetCurrentTheme, SetTheme, XTPShortcutBarPaintTheme
	//-----------------------------------------------------------------------
	void SetCustomTheme(CXTPShortcutBarPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager.
	// Returns:
	//     A pointer to the paint manager.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the theme of the shortcut bar control.
	// Returns:
	//     The theme of the shortcut bar control; one of the values
	//     defined by the XTPShortcutBarPaintTheme enumeration.
	// See Also:
	//     SetTheme, SetCustomTheme, XTPShortcutBarPaintTheme
	//-----------------------------------------------------------------------
	XTPShortcutBarPaintTheme GetCurrentTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the shortcut bar control.
	// Parameters:
	//     lpRect   - Rectangular area of the control that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-----------------------------------------------------------------------
	void RedrawControl(LPCRECT lpRect = NULL, BOOL bAnimate = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of visible items.
	// Returns:
	//     The number of visible items.
	//-----------------------------------------------------------------------
	int GetVisibleItemsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of collapsed items.
	// Returns:
	//     The number of collapsed items.
	//-----------------------------------------------------------------------
	int GetCollapsedItemsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions items.
	//-----------------------------------------------------------------------
	virtual void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the framework to draw the shortcut bar control
	//     using the specified device context.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     rcClipBox - Rectangular area of the control that is invalid.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcClipBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables animation for the shortcut bar control.
	// Parameters:
	//     bEnable - TRUE to enable animation, FALSE to disable animation.
	//-----------------------------------------------------------------------
	void EnableAnimation(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the shortcut bar control is minimized.
	// Returns:
	//     TRUE if the shortcut bar control is mimimized, otherwise FALSE.
	// See Also:
	//     AllowMinimize
	//-----------------------------------------------------------------------
	BOOL IsShortcutBarMinimized() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows minimizing the shortcut bar control.
	// Parameters:
	//     bAllowMinimize  - TRUE to allow minimizing the shortcut bar control,
	//                       FALSE to disallow minimizing the shortcut bar control.
	//     nMinimizedWidth - Width to be set for the shortcut bar control
	//                       when it is minimized. Set the value of this
	//                       parameter to 0 to use the default width of 32.
	//-----------------------------------------------------------------------
	void AllowMinimize(BOOL bAllowMinimize = TRUE, int nMinimizedWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the minimize button contained in the caption of the pane.
	// Parameters:
	//     bShowMinimizeButton - TRUE to show the minimize button,
	//                           FALSE to hide the minimize button.
	//-----------------------------------------------------------------------
	void ShowMinimizeButton(BOOL bShowMinimizeButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the minimize button contained in the caption of the pane
	//     is shown/hidden.
	// Returns:
	//     TRUE if the minimize button is shown, FALSE if the minimize button is hidden.
	//-----------------------------------------------------------------------
	BOOL IsMinimizeButtonVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if minimizing the shortcut bar control is allowed/disallowed.
	// Returns:
	//     TRUE if minimizing the shortcut bar control is allowed,
	//     FALSE if minimizing the shortcut bar control is disallowed.
	//-----------------------------------------------------------------------
	BOOL IsAllowMinimize() const;

	CXTPShortcutBarItem* GetNavigationPaneItem() const;

	virtual void OnMinimizeButtonClicked();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when reposition is done.
	//-----------------------------------------------------------------------
	virtual void OnRepositionDone();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the expand button is pressed.
	// Parameters:
	//     pExpandButton - Pointer to the expand button.
	//-----------------------------------------------------------------------
	virtual void OnExpandButtonDown(CXTPShortcutBarItem* pExpandButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the navigate button is pressed.
	// Parameters:
	//     pNavigateButton - Pointer to the navigate button.
	//-----------------------------------------------------------------------
	void OnNavigateButtonDown(CXTPShortcutBarItem* pNavigateButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the hot item.
	// Parameters:
	//     pItem - Pointer to the item to be set.
	//-----------------------------------------------------------------------
	void SetHotItem(CXTPShortcutBarItem* pItem);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShortcutBar)
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	void PreSubclassWindow();
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPShortcutBar)
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* /*lpncsp*/);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnMouseLeave();
	afx_msg void OnSysColorChange();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnExpandPopupExecute(WPARAM wparam, LPARAM lParam);
	afx_msg void OnShowMoreButtons();
	afx_msg void OnShowFewerButtons();
	afx_msg void OnUpdateShowMoreButtons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowFewerButtons(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pWnd);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	CXTPShortcutBarItem* CreateNavigationItem();
	CXTPShortcutBarItem* CreateConfigureButton();

protected:
	CXTPShortcutBarPaintManager* m_pPaintManager; // Current paint manager.
	CXTPImageManager* m_pImageManager;			  // Current image manager.
	int m_nExpandedLines;						  // Number of lines currently visible.
	int m_nDesiredExpandedLinesHeight;			  // Desired visible lines height.
	int m_nMinClientHeight;						  // The height of the client area.
	BOOL m_bAllowResize;						  // TRUE to allow resize.
	XTPShortcutBarPaintTheme m_paintTheme;		  // Current theme.
	CShortcutArray m_arrItems;					  // Array of items.
	CSize m_szItem;								  // Size of items.
	CRect m_rcClient;							  // Client's rectangle.
	CRect m_rcGripper;							  // Gripper's rectangle.
	HWND m_hwndClient;							  // Client window handle.
	HCURSOR m_hSizeCursor;		 // Handle to the cursor displayed for the size icon.
	BOOL m_bTrackingSize;		 // TRUE if the control is in tracking mode.
	BOOL m_bShowActiveItemOnTop; // TRUE to show the active item on top.
	BOOL m_bAllowFreeResize;	 // TRUE to allow resize gripper by pixel.
	BOOL m_bAllowCollapse;		 // TRUE to allow collapse items.
	BOOL m_bShowGripper;		 // TRUE to show the gripper.

	CXTPShortcutBarItem* m_pHotItem;	  // Pointer to the hot item.
	CXTPShortcutBarItem* m_pSelectedItem; // Pointer to the selected item.
	CXTPShortcutBarItem* m_pPressedItem;  // Pointer to the pressed item.

	CXTPShortcutBarPopup* m_pActivePopup;

	CXTPShortcutBarItem* m_pNavigationPaneItem;

	BOOL m_bClientPaneVisible; // TRUE if the client pane in the shortcut bar is visible.
	HCURSOR m_hHandCursor;	 // Hand cursor that is displayed when the cursor is positioned over a
							   // shortcut bar item.
	BOOL m_bSingleSelection;   // TRUE to only allow a single selected item.
	CXTPToolTipContext* m_pToolTipContext;  // Tooltip context.
	BOOL m_bPreSubclassWindow;				// True if PreSubclassWindow was called.
	CXTPShortcutBarAnimation* m_pAnimation; // Animation helper.

	int m_bAllowMinimize;
	int m_nMinimizedWidth;
	BOOL m_bMinimized;
	BOOL m_bShowMinimizeButton;

	friend class CXTPShortcutBarItem;
	friend class CShortcutBarCtrl;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CString CXTPShortcutBarItem::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE CString CXTPShortcutBarItem::GetTooltip() const
{
	return m_strTooltip;
}
AFX_INLINE BOOL CXTPShortcutBarItem::IsItemExpandButton() const
{
	return m_bExpandButton;
}
AFX_INLINE BOOL CXTPShortcutBarItem::IsExpanded() const
{
	return m_bExpanded;
}
AFX_INLINE CXTPShortcutBar* CXTPShortcutBarItem::GetShortcutBar() const
{
	return m_pShortcutBar;
}
AFX_INLINE CRect CXTPShortcutBarItem::GetItemRect() const
{
	return m_rcItem;
}
AFX_INLINE void CXTPShortcutBarItem::SetItemData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPShortcutBarItem::GetItemData() const
{
	return m_dwData;
}
AFX_INLINE CWnd* CXTPShortcutBarItem::GetClientWindow() const
{
	return CWnd::FromHandle(m_hwndChild);
}

AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetHotItem() const
{
	return m_pHotItem;
}
AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetSelectedItem() const
{
	return m_pSelectedItem;
}
AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetPressedItem() const
{
	return m_pPressedItem;
}
AFX_INLINE CRect CXTPShortcutBar::GetGripperRect() const
{
	return m_rcGripper;
}
AFX_INLINE CSize CXTPShortcutBar::GetItemSize() const
{
	return m_szItem;
}
AFX_INLINE void CXTPShortcutBar::SetItemSize(CSize szItem)
{
	m_szItem = szItem;
}
AFX_INLINE int CXTPShortcutBar::GetExpandedLinesCount() const
{
	return m_nExpandedLines;
}
AFX_INLINE BOOL CXTPShortcutBar::IsClientPaneVisible() const
{
	return m_bClientPaneVisible;
}
AFX_INLINE void CXTPShortcutBar::SetClientPaneVisible(BOOL bVisible)
{
	m_bClientPaneVisible = bVisible;
	Reposition();
}
AFX_INLINE CXTPShortcutBarPaintManager* CXTPShortcutBar::GetPaintManager() const
{
	return m_pPaintManager;
}
AFX_INLINE void CXTPShortcutBar::SetMinimumClientHeight(int nMinHeight)
{
	m_nMinClientHeight = nMinHeight;
}
AFX_INLINE XTPShortcutBarPaintTheme CXTPShortcutBar::GetCurrentTheme() const
{
	return m_paintTheme;
}
AFX_INLINE BOOL CXTPShortcutBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
										DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
										CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTPShortcutBar::AllowGripperResize(BOOL bAllowResize)
{
	m_bAllowResize = bAllowResize;
}
AFX_INLINE void CXTPShortcutBar::SetSingleSelection(BOOL bSingleSelection)
{
	m_bSingleSelection = bSingleSelection;
	Reposition();
}
AFX_INLINE BOOL CXTPShortcutBar::IsSingleSelection() const
{
	return m_bSingleSelection;
}
AFX_INLINE void CXTPShortcutBar::ShowActiveItemOnTop(BOOL bActiveItemOnTop)
{
	m_bShowActiveItemOnTop = bActiveItemOnTop;
	Reposition();
}
AFX_INLINE BOOL CXTPShortcutBar::IsShowActiveItemOnTop() const
{
	return m_bShowActiveItemOnTop;
}
AFX_INLINE void CXTPShortcutBar::AllowFreeResize(BOOL bAllowFreeResize)
{
	m_bAllowFreeResize = bAllowFreeResize;
	Reposition();
}
AFX_INLINE BOOL CXTPShortcutBar::IsAllowFreeResize() const
{
	return m_bAllowFreeResize;
}
AFX_INLINE void CXTPShortcutBar::AllowCollapse(BOOL bAllowCollapse)
{
	m_bAllowCollapse = bAllowCollapse;
	Reposition();
}
AFX_INLINE void CXTPShortcutBar::ShowGripper(BOOL bShowGripper)
{
	m_bShowGripper = bShowGripper;
	Reposition();
}
AFX_INLINE int CXTPShortcutBar::GetExpandedLinesHeight() const
{
	return m_nDesiredExpandedLinesHeight;
}
AFX_INLINE CRect CXTPShortcutBar::GetClientPaneRect() const
{
	return m_rcClient;
}
AFX_INLINE void CXTPShortcutBar::ShowMinimizeButton(BOOL bShowMinimizeButton)
{
	m_bShowMinimizeButton = bShowMinimizeButton;
	Reposition();
}
AFX_INLINE BOOL CXTPShortcutBar::IsAllowMinimize() const
{
	return m_bAllowMinimize;
}
AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetNavigationPaneItem() const
{
	return m_pNavigationPaneItem;
}
AFX_INLINE BOOL CXTPShortcutBar::IsMinimizeButtonVisible() const
{
	return m_bShowMinimizeButton;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBAR_H__)
