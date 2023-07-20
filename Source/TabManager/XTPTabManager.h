// XTPTabManager.h: interface for the CXTPTabManager class.
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
#if !defined(__XTPTABMANAGER_H__)
#	define __XTPTABMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabManagerNavigateButton;
class CXTPTabManagerNavigateButtons;
class CXTPTabManagerItem;
class CXTPTabManagerAtom;
class CXTPTabManager;
class CXTPTabPaintManagerColorSet;
class CXTPTabPaintManagerTheme;

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_GETTABCOLOR message is sent to an MDI child window to
//     retrieve the color that will be used for a tabbed interface with
//     enabled OneNote colors.
// Remarks:
//     You can override CXTPTabClientWnd::GetItemColor instead of
//     processing this message.
// Returns:
//     The color that will be used for a tabbed interface with
//     enabled OneNote colors.
// Example:
//     Here is an example of how an application would process
//     the WM_XTP_GETTABCOLOR message:
// <code>
// BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//     //{{AFX_MSG_MAP(CChildFrame)
//     ON_MESSAGE(WM_XTP_GETTABCOLOR, OnGetTabColor)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CChildFrame::OnGetTabColor(WPARAM /*wParam*/, LPARAM /*lParam*/)
// {
//     return CXTPTabPaintManager::GetOneNoteColor(xtpTabColorOrange);
// }
// </code>
// See Also:
//     CXTPTabPaintManager::GetOneNoteColor, CXTPTabClientWnd, WM_XTP_GETWINDOWTEXT,
//     WM_XTP_GETTABICON, WM_XTP_GETWINDOWTOOLTIP
//-----------------------------------------------------------------------
const UINT WM_XTP_GETTABCOLOR = (WM_USER + 9400 + 1);

class CXTPTabPaintManager;
class CXTPImageManagerIcon;
class CXTPMarkupContext;
class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//     CXTPTabManager is the base class for docking pane tabs,
//     tab control tabs, and MDI client tabs.
// See Also:
//     CXTPTabClientWnd, CXTPDockingPaneTabbedContainer, CXTPTabControl
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManager : public CXTPTabManagerAtom
{
protected:
	class CNavigateButtonArrow;
	class CNavigateButtonArrowLeft;
	class CNavigateButtonArrowRight;
	class CNavigateButtonClose;
	class CNavigateButtonTabClose;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Helper structure that contains the first and last item indices
	//     of rows of the tab manager.
	//-------------------------------------------------------------------------
	struct ROW_ITEMS
	{
		int nFirstItem; // First item index of row.
		int nLastItem;  // Last item index of row.
	};

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Helper class used to collect the indices of rows for a multi-row tab manager.
	//-------------------------------------------------------------------------
	class CRowIndexer
	{
	public:
		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CRowIndexer object.
		// Parameters:
		//     pManager - Pointer to the owner tab manager.
		//-------------------------------------------------------------------------
		CRowIndexer(CXTPTabManager* pManager);

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CRowIndexer object, handles cleanup and deallocation.
		//-------------------------------------------------------------------------
		~CRowIndexer();

	public:
		//-------------------------------------------------------------------------
		// Summary:
		//     Creates the array of indices for the multi-row tab manager.
		// Parameters:
		//     nRowCount - Total number of rows in the tab manager.
		// Returns:
		//     A pointer to the array of indices of the multi-row tab manager.
		//-------------------------------------------------------------------------
		ROW_ITEMS* CreateIndexer(int nRowCount);

		//-------------------------------------------------------------------------
		// Summary:
		//     Gets the array of indices of the multi-row tab manager.
		// Returns:
		//     A pointer to the array of indices of the multi-row tab manager.
		//-------------------------------------------------------------------------
		ROW_ITEMS* GetRowItems() const;

	private:
		ROW_ITEMS* m_pRowItems;
		int m_nRowCount;
		CXTPTabManager* m_pManager;

		friend class CXTPTabManager;
	};

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManager object.
	//-------------------------------------------------------------------------
	CXTPTabManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManager object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a tab item by its index.
	// Parameters:
	//     nIndex - Zero-based index of the tab item to retrieve.
	// Returns:
	//     A pointer to the tab item if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes a tab item by its index.
	// Parameters:
	//     nItem - Zero-based index of the tab item to delete.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	BOOL DeleteItem(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes all tab items from the tab manager.
	//-----------------------------------------------------------------------
	void DeleteAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of tab items in the tab manager.
	// Returns:
	//     The number of tab items in the tab manager.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption text of a specified tab item.
	// Parameters:
	//     pItem - Pointer to the tab item with the caption text to retrieve.
	// Returns:
	//     The caption text of the specified tab item.
	//-----------------------------------------------------------------------
	virtual CString GetItemCaption(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon of a specified tab item.
	// Parameters:
	//     pItem - Pointer to the tab item with the icon to retrieve.
	// Returns:
	//     The icon of the specified tab item.
	//-----------------------------------------------------------------------
	virtual HICON GetItemIcon(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tab button color of a specified tab item.
	// Parameters:
	//     pItem - Pointer to the tab item with the tab button color to retrieve.
	// Returns:
	//     The tab button color of the specified tab item.
	// See Also:
	//     CXTPTabManagerItem::SetColor, CXTPTabManagerItem::GetColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemColor(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tooltip of a specified tab item.
	// Parameters:
	//     pItem - Pointer to the tab item with the tooltip to retrieve.
	// Remarks:
	//     The tooltip is the text displayed when
	//     the mouse is positioned over the tab button.
	// Returns:
	//     The tooltip of the specified tab item.
	// See Also:
	//     CXTPTabManagerItem::SetTooltip, CXTPTabManagerItem::GetTooltip
	//-----------------------------------------------------------------------
	virtual CString GetItemTooltip(const CXTPTabManagerItem* pItem) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when tab items are changed
	//     (i.e. after a tab item is added, deleted, or reordered).
	// Remarks:
	//     This method will recalculate the index for all tab items
	//     and then call Reposition.
	// See Also:
	//     CXTPTabManager::AddItem, CXTPTabManager::ReOrder,
	//     CXTPTabManager::DeleteItem, CXTPTabManager::DeleteAllItems
	//-------------------------------------------------------------------------
	virtual void OnItemsChanged();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index for the currently selected tab item within
	//     the collection of tab items.
	// Parameters:
	//     nIndex - Index to be set.
	// Remarks:
	//     The currently selected tab item is the currently active tab item.
	//     A tab item is selected when it has focus or is clicked.
	//
	//     This method calls GetItem to retrieve a pointer to the tab item at
	//     the specified index and then SetSelectedItem to select that tab item.
	// See Also:
	//     CXTPTabManagerItem, GetCurSel, SetSelectedItem, GetSelectedItem
	//-----------------------------------------------------------------------
	void SetCurSel(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the currently selected tab item within
	//     the collection of tab items.
	// Remarks:
	//     The currently selected tab item is the currently active tab item.
	//     A tab item is selected when it has focus or is clicked.
	//
	//     The currently selected tab item can be retrieved by using
	//     GetSelectedItem.
	// Returns:
	//     The index of the currently selected tab item.
	// See Also:
	//     CXTPTabManagerItem, SetSelectedItem, SetCurSel, GetSelectedItem
	//-----------------------------------------------------------------------
	int GetCurSel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently selected tab item.
	// Parameters:
	//     pItem - Pointer to the tab item to be selected.
	// Remarks:
	//     The currently selected tab item is the currently active tab item.
	//     A tab item is selected when it has focus or is clicked.
	//
	//     The currently selected tab item can be set by its index within
	//     the collection of tab items by using SetCurSel.
	// See Also:
	//     CXTPTabManagerItem, GetCurSel, SetCurSel, GetSelectedItem
	//-----------------------------------------------------------------------
	virtual void SetSelectedItem(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the focused tab item.
	// Parameters:
	//     pItem - Pointer to the tab item to be focused.
	// See Also:
	//     SetSelectedItem
	//-----------------------------------------------------------------------
	virtual void SetFocusedItem(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently selected tab item.
	// Remarks:
	//     The currently selected tab item is the currently active tab item.
	//     A tab item is selected when it has focus or is clicked.
	// Returns:
	//     A pointer to the currently selected tab item.
	// See Also:
	//     CXTPTabManagerItem, SetSelectedItem, SetCurSel, GetCurSel
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetSelectedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which tab is at a specified point, if any.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the tab at the specified point, if any,
	//     otherwise NULL.
	// Remarks:
	//     This method can be used to determine if the user clicked on a tab.
	//
	//     In order for a pointer to the tab at the specified point to
	//     be returned, the tab must be both visible and enabled. If the tab
	//     is not both visible and enabled, then NULL will be returned.
	// See Also:
	//     PerformClick, PerformMouseMove
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* HitTest(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which navigation button is at a specified point, if any.
	// Parameters:
	//     point       - Point to be tested.
	//     pnIndex     - Pointer to an integer to receive the index of
	//                   the navigation button at the specified point, if any.
	//     bHeaderOnly - TRUE to only test buttons located in the header,
	//                   FALSE to test all buttons.
	// Returns:
	//     A pointer to the navigation button at the specified point, if any,
	//     otherwise NULL.
	// See Also:
	//     HitTest
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* HitTestNavigateButton(CPoint point, BOOL bHeaderOnly,
														int* pnIndex = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum, maximum, and normal lengths of tab buttons.
	// Parameters:
	//     szNormal - Pointer to a CSize object to receive the fixed length used
	//                for all tabs when the layout is xtpTabLayoutFixed.
	//     szMin    - Pointer to a CSize object to receive the minimum length used
	//                for tabs when the layout is not xtpTabLayoutFixed.
	//     szMax    - Pointer to a CSize object to receive the maximum length used
	//                for tabs when the layout is not xtpTabLayoutFixed.
	// Remarks:
	//     The minimum, maximum, and normal lengths of tab buttons are used
	//     when calculating the lengths of tabs for each XTPTabLayoutStyle.
	//
	//     szNormal only applies when the layout is xtpTabLayoutFixed. When
	//     the layout is xtpTabLayoutFixed, every tab will have a fixed length of
	//     szNormal. Otherwise, szMin and szMax represent the lower and upper
	//     bounds for the length of individual tabs respectively.
	// See Also:
	//     XTPTabLayoutStyle, SetItemMetrics, SetLayoutStyle
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(CSize* szNormal, CSize* szMin = NULL, CSize* szMax = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum, maximum, and normal lengths for tab buttons.
	// Parameters:
	//     szNormal - Fixed length to be used for all tabs when
	//                the layout is xtpTabLayoutFixed.
	//     szMin    - Minimum length to be used for tabs when
	//                the layout is not xtpTabLayoutFixed.
	//     szMax    - Maximum length to be used for tabs when
	//                the layout is not xtpTabLayoutFixed.
	// Remarks:
	//     The minimum, maximum, and normal lengths of tab buttons are used
	//     when calculating the lengths of tabs for each XTPTabLayoutStyle.
	//
	//     szNormal only applies when the layout is xtpTabLayoutFixed. When
	//     the layout is xtpTabLayoutFixed, every tab will have a fixed length of
	//     szNormal. Otherwise, szMin and szMax represent the lower and upper
	//     bounds for the length of individual tabs respectively.
	// See Also:
	//     XTPTabLayoutStyle, GetItemMetrics, SetLayoutStyle
	//-----------------------------------------------------------------------
	void SetItemMetrics(CSize szNormal, CSize szMin = CSize(0, 0), CSize szMax = CSize(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the position of a specified tab item.
	// Parameters:
	//     pItem  - Pointer to the tab item whose position must be set.
	//     nIndex - Position to be set.
	//-----------------------------------------------------------------------
	void MoveItem(CXTPTabManagerItem* pItem, int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show close button for each tab
	// Parameters:
	//     bCloseItemButton - XTPTabNavigateButtonFlags that specify if close button for each tab is
	//     visible
	//-----------------------------------------------------------------------
	void ShowCloseItemButton(
		XTPTabNavigateButtonFlags bCloseItemButton = xtpTabNavigateButtonAlways);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total width of all of the tab buttons.
	// Returns:
	//     The total width of all of the tab buttons.
	// Remarks:
	//     The width may change depending on both
	//     the tab layout and the position of the tabs.
	// See Also:
	//     XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	virtual int GetItemsLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length of the bounding rectangle of
	//     the tab client header area.
	// Parameters:
	//     rc - Bounding rectangle of tab client header area.
	// Returns:
	//     If tabs are horizontal, then the width of
	//     the tab client header area is returned.
	//     If tabs are vertical, then the height of
	//     the tab client header area is returned.
	// Remarks:
	//     The tab client header area is the area that
	//     has all of the tab buttons.
	// See Also:
	//     GetAppearanceSet, CXTPTabPaintManagerTheme::GetHeaderRect
	//-----------------------------------------------------------------------
	int GetRectLength(CRect rc) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified tab navigation button (i.e. either the
	//     left arrow, right arrow, or close button in the tab header area).
	// Parameters:
	//     nID - Identifier of the button to retrieve.
	//           Can be one of the values listed in the Remarks section.
	// Remarks:
	//     Standard IDs of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>: Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b>: Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b>: Close tab navigation button.
	// Returns:
	//     A pointer to the specified tab navigation button.
	// See Also:
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* FindNavigateButton(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the tabs are positioned horizontally.
	// Returns:
	//     TRUE if the tabs are positioned horizontally (i.e. top or bottom),
	//     FALSE if the tabs are positioned vertically (i.e. left or right).
	// See Also:
	//     GetPosition, XTPTabPosition
	//-----------------------------------------------------------------------
	BOOL IsHorizontalPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scroll offset of the tabs in the tab header.
	// Returns:
	//     The scroll offset of the tabs in the tab header.
	// Remarks:
	//     The scroll offset is how much the tabs have been scrolled by
	//     pressing the left and right tab navigation buttons.
	//
	//     The scroll offset starts at zero and remains at zero until
	//     there are enough tabs to scroll. If the tabs are scrolled, then
	//     the offset will be a negative number indicating how much
	//     the tabs have been scrolled to the right.
	//
	//     NOTE: GetHeaderOffset and SetHeaderOffset are used when the
	//           left and right tab navigate buttons are pressed to scroll
	//           the tabs by 20 pixels to the left or right.
	//
	//           EnsureVisible uses GetHeaderOffset and SetHeaderOffset to
	//           ensure that a tab button is visible.
	// Example:
	//     This sample illustrates how to programmatically set the scroll offset
	//     of the tabs in the tab header of the CXTPTabClientWnd:
	// <code>
	// //Scroll the tabs in the tab header to the right by 100 pixels.
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset())
	// - 100);
	//
	// //Scroll the tabs in the tab header to the left by 100 pixels.//
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset())
	// + 100);
	// </code>
	// See Also:
	//     SetHeaderOffset, CXTPTabManagerNavigateButton, EnsureVisible
	//-----------------------------------------------------------------------
	int GetHeaderOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the scroll offset for the tabs in the tab header.
	// Parameters:
	//     nOffset - Scroll offset to be set. The value of this parameter
	//               must be \<= 0. GetHeaderOffset should be used to get
	//               the current scroll offset and then add to or subtract
	//               from the current scroll offset to move left or right.
	//               If the value of this parameter is set to 0, then the
	//               tabs will be reset to there starting position.
	// Remarks:
	//     The scroll offset is how much the tabs have been scrolled by
	//     pressing the left and right tab navigation buttons.
	//
	//     The scroll offset starts at zero and remains at zero until
	//     there are enough tabs to scroll. If the tabs are scrolled, then
	//     the offset will be a negative number indicating how much
	//     the tabs have been scrolled to the right.
	//
	//     NOTE: GetHeaderOffset and SetHeaderOffset are used when the
	//           left and right tab navigate buttons are pressed to scroll
	//           the tabs by 20 pixels to the left or right.
	//
	//           EnsureVisible uses GetHeaderOffset and SetHeaderOffset to
	//           ensure that a tab button is visible.
	// Example:
	//     This sample illustrates how to programmatically set the scroll offset
	//     of the tabs in the tab header of the CXTPTabClientWnd:
	// <code>
	// //Scroll the tabs in the tab header to the right by 100 pixels.
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset())
	// - 100);
	//
	// //Scroll the tabs in the tab header to the left by 100 pixels.//
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset())
	// + 100);
	// </code>
	// See Also:
	//     GetHeaderOffset, CXTPTabManagerNavigateButton, EnsureVisible
	//-----------------------------------------------------------------------
	void SetHeaderOffset(int nOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a specified tab is visible.
	// Parameters:
	//     pItem - Pointer to the tab to ensure the visibility of.
	// See Also:
	//     SetHeaderOffset, GetHeaderOffset
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTabManagerItem* pItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab paint manager.
	// Remarks:
	//     The tab paint manager is used to customize the appearance of
	//     both CXTPTabManagerItem objects and the tab manager
	//     (i.e. tab colors, styles, etc.).
	//
	//     This method must be overridden in derived classes.
	// Returns:
	//     A pointer to the tab paint manager.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab paint manager.
	// Parameters:
	//     pPaintManager - Pointer to the tab paint manager to be set.
	// Remarks:
	//     The tab paint manager is used to customize the appearance of
	//     both CXTPTabManagerItem objects and the tab manager
	//     (i.e. tab colors, styles, etc.).
	//-----------------------------------------------------------------------
	virtual void SetPaintManager(CXTPTabPaintManager* pPaintManager);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the visual properties of the tabs
	//     are changed (e.g. color, mouse-over, on-click).
	// Parameters:
	//     lpRect   - Rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	// Remarks:
	//     This method must be overridden in derived classes.
	//-------------------------------------------------------------------------
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if tabs are allowed to be reordered.
	// Returns:
	//     TRUE if tabs are allowed to be reordered,
	//     FALSE if tabs are not allowed to be reordered.
	// See Also:
	//     SetAllowReorder
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowReorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if tabs should be allowed to be reordered.
	// Parameters:
	//     bAllowReorder - TRUE to allow tabs to be reordered,
	//                     FALSE to disallow tabs from being reordered.
	// See Also:
	//     IsAllowReorder
	//-----------------------------------------------------------------------
	virtual void SetAllowReorder(BOOL bAllowReorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a static frame is used.
	// Returns:
	//     TRUE if a static frame is used, otherwise FALSE.
	// Remarks:
	//     If a static frame is used, then it will be drawn around
	//     the entire tab manager control
	//     (i.e. around both the tab header and the tab client).
	// See Also:
	//     CXTPTabPaintManager::m_bStaticFrame
	//-----------------------------------------------------------------------
	virtual BOOL IsDrawStaticFrame() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the icon of a tab must
	//     be redrawn (e.g. on mouse-over).
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Point that specifies the location of the image.
	//     pItem  - Pointer to a CXTPTabManagerItem object to draw the icon on.
	//     bDraw  - TRUE if the icon must be drawn/redrawn
	//              (i.e. the icon size changed), FALSE otherwise.
	//     szIcon - Reference to the size of the tab icon.
	// Remarks:
	//     This method is overridden by its descendants.
	//
	//     This method must be overridden in derived classes.
	// Returns:
	//     TRUE if the icon was drawn successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw,
						  CSize& szIcon) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables Windows XP themed back color.
	// Parameters:
	//     hWnd    - Window handle of the target object or dialog.
	//     bEnable - TRUE to enable Windows XP themed back color,
	//               FALSE to disable Windows XP themed back color.
	// Remarks:
	//     This method should only be used when the tab color
	//     is set to xtpTabColorWinNative.
	// Example:
	//     This sample code illustrates how to apply a
	//     Windows XP themed back color to a tab control:
	// <code>
	// m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorWinNative);
	// XTPTabColorStyle color = m_wndTabControl.GetPaintManager()->GetColor();
	// m_wndTabControl.EnableTabThemeTexture(m_wndTabControl.GetSafeHwnd(), color ==
	// xtpTabColorWinNative ? TRUE : FALSE); RedrawWindow();
	// </code>
	// See Also:
	//     XTPTabColorStyle, GetColor, SetColor
	//-----------------------------------------------------------------------
	void EnableTabThemeTexture(HWND hWnd, BOOL bEnable = TRUE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Applies one of the "built-in" color sets for the tabs.
	// Parameters:
	//     tabColor - "Built-in" color set to be applied.
	//                Can be one of the values listed in the Remarks section.
	// Remarks:
	//     Use SetColor to apply a "built-in" color set.
	//     Use SetColorSet to apply a custom color set.
	//
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	//
	//     tabColor can be one of the following:
	//     * <b>xtpTabColorDefault</b>: Tabs will use the default color
	//                                  style for the currently set Appearance.
	//     * <b>xtpTabColorVisualStudio2003</b>: Tabs will use the Visual Studio color
	//                                           style for the currently set Appearance.
	//     * <b>xtpTabColorOffice2003</b>: Tabs will use the Office 2003 color
	//                                     style for the currently set Appearance.
	//     * <b>xtpTabColorWinNative</b>: Tabs will use the Windows XP color
	//                                    style for the currently set Appearance.
	//     * <b>xtpTabColorVisualStudio2005</b>: Tabs will use the Visual Studio 2005 color
	//                                           style for the currently set Appearance.
	// Returns:
	//     A pointer to the newly applied "built-in" color set.
	// See Also:
	//     XTPTabColorStyle, SetColorSet, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* SetColor(XTPTabColorStyle tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a custom color set for the tabs.
	// Parameters:
	//     pColorSet - Pointer to the custom color set to be applied.
	// Returns:
	//     A pointer to the newly applied custom color set.
	// Remarks:
	//     Use SetColor to apply a "built-in" color set.
	//     Use SetColorSet to apply a custom color set.
	//
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* SetColorSet(CXTPTabPaintManagerColorSet* pColorSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies one of the "built-in" appearance sets for the tabs.
	// Parameters:
	//     tabAppearance - "Built-in" appearance set to be applied; must be one of
	//                     the values defined by the XTPTabAppearanceStyle enumeration.
	// Returns:
	//     A pointer to the newly applied "built-in" appearance set.
	// Remarks:
	//     Use SetAppearance to apply a "built-in" appearance.
	//     Use SetAppearanceSet to apply a custom appearance.
	//
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	// See Also:
	//     XTPTabAppearanceStyle, SetAppearanceSet, GetAppearance, GetAppearanceSet,
	//     SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* SetAppearance(XTPTabAppearanceStyle tabAppearance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a custom appearance set for the tabs.
	// Parameters:
	//     pAppearanceSet - Pointer to the custom appearance set to be applied.
	// Remarks:
	//     Use SetAppearance to apply a "built-in" appearance.
	//     Use SetAppearanceSet to apply a custom appearance.
	//
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	//
	//     SetColorSet is used to set custom colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it for the tabs.
	// Returns:
	//     A pointer to the newly applied custom appearance set.
	// See Also:
	//     XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//     SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* SetAppearanceSet(CXTPTabPaintManagerTheme* pAppearanceSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied custom appearance set.
	// Returns:
	//     A pointer to the currently applied custom appearance set.
	// Remarks:
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	//
	//     SetColorSet is used to set custom colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it for the tabs.
	// See Also:
	//     GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* GetAppearanceSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied custom color set.
	// Returns:
	//     A pointer to the currently applied custom color set.
	// Remarks:
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, GetColor, SetColorSet, SetColor
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* GetColorSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied "built-in appearance set.
	// Returns:
	//     The currently applied "built-in" appearance set; one of
	//     the values defined by the XTPTabAppearanceStyle enumeration.
	// Remarks:
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	// See Also:
	//     XTPTabAppearanceStyle, GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	XTPTabAppearanceStyle GetAppearance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied "built-in" color set.
	// Returns:
	//     The currently applied "built-in" color set.
	// Remarks:
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	XTPTabColorStyle GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab position.
	// Parameters:
	//     tabPosition - Tab position to be set.
	//                   Can be one of the values listed in the Remarks section.
	// Remarks:
	//     The tab position refers to where the tab buttons are located.
	//     The tab buttons can be positioned on either the top, bottom,
	//     left, or right side of the tab client area.
	//
	//     tabPosition can be one of the following:
	//     * <b>xtpTabPositionTop</b>: Tabs will be drawn on the top.
	//     * <b>xtpTabPositionLeft</b>: Tabs will be drawn on the left.
	//     * <b>xtpTabPositionBottom</b>: Tabs will be drawn on the bottom.
	//     * <b>xtpTabPositionRight</b>: Tabs will be drawn on the right.
	// See Also:
	//     XTPTabPosition, GetPosition
	//-----------------------------------------------------------------------
	void SetPosition(XTPTabPosition tabPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab layout style.
	// Parameters:
	//     tabLayout - Tab layout style to be set.
	//                 Can be one of the values listed in the Remarks section.
	// Remarks:
	//     The tab layout style refers to how the tab buttons are sized
	//     within the tab client header. The tab buttons can be in either
	//     a compressed, fixed, auto-size, or size-to-fit layout.
	//
	//     tabLayout can be one of the following:
	//     * <b>xtpTabLayoutAutoSize</b>: Tabs will be automatically sized based on
	//                                    the caption and image size. With this flag set,
	//                                    tabs will appear in their normal size.
	//     * <b>xtpTabLayoutSizeToFit</b>: Tabs are sized to fit within the tab panel.
	//                                     All tabs will be compressed and forced to fit
	//                                     into the tab panel.
	//     * <b>xtpTabLayoutFixed</b>: All tabs will be set to a fixed size within
	//                                 the tab panel.
	//     * <b>xtpTabLayoutCompressed</b>: Tabs will be compressed within the tab panel.
	//                                      This will compress the size of the tabs, but
	//                                      all tabs will not be forced into the tab panel.
	//     * <b>xtpTabLayoutMultiRow</b>: Causes a tab control to display multiple
	//                                    rows of tabs.
	// See Also:
	//     XTPTabLayoutStyle, GetLayout
	//-----------------------------------------------------------------------
	void SetLayoutStyle(XTPTabLayoutStyle tabLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab position.
	// Returns:
	//     The tab position; one of the values defined by
	//     the XTPTabPosition enumeration.
	// Remarks:
	//     The tab position refers to where the tab buttons are located.
	//     The tab buttons can be positioned on either the top, bottom,
	//     left, or right side of the tab client area.
	// See Also:
	//     XTPTabPosition, SetPosition
	//-----------------------------------------------------------------------
	virtual XTPTabPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab layout style.
	// Returns:
	//     The tab layout style; one of the values defined by
	//     the XTPTabLayoutStyle enumeration.
	// Remarks:
	//     The tab layout style refers to how the tab buttons are sized
	//     within the tab client header. The tab buttons can be in either
	//     a compressed, fixed, auto-size, or size-to-fit layout.
	// See Also:
	//     XTPTabLayoutStyle, SetLayoutStyle
	//-----------------------------------------------------------------------
	XTPTabLayoutStyle GetLayout() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides tab icons.
	// Parameters:
	//     bShowIcons - TRUE to show tab icons, FALSE to hide tab icons.
	//-----------------------------------------------------------------------
	void ShowIcons(BOOL bShowIcons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if this tab group should be active/inactive.
	// Parameters:
	//     bActive - TRUE to set this tab group to active,
	//               FALSE to set this tab group to inactive.
	// Remarks:
	//     This method only applies to tab groups in the MDITabClient
	//     (i.e. the tabs groups that appear in the CommandBars TabWorkspace)
	//     when grouping is enabled.
	//
	//     When grouping is enabled, each tab group has its own CXTPTabManager.
	//
	//     SetActive is used to activate/set focus to the tab group.
	//
	//     Only one tab group can be active at a given time.
	// See Also:
	//     IsActive, CXTPTabClientWnd::Attach
	//-----------------------------------------------------------------------
	void SetActive(BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this tab group is active/inactive.
	// Returns:
	//     TRUE if this tab group is active, FALSE if this tab group is inactive.
	// Remarks:
	//     This method only applies to tab groups in the MDITabClient
	//     (i.e. the tabs groups that appear in the CommandBars TabWorkspace)
	//     when grouping is enabled.
	//
	//     When grouping is enabled, each tab group has its own CXTPTabManager.
	//
	//     SetActive is used to activate/set focus to the tab group.
	//
	//     Only one tab group can be active at a given time.
	// See Also:
	//     SetActive, CXTPTabClientWnd::Attach
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab manager control area.
	// Returns:
	//     The bounding rectangle of the tab manager control area.
	// See Also:
	//     GetHeaderRect, GetClientRect
	//-----------------------------------------------------------------------
	CRect GetControlRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab client area.
	// Returns:
	//     The bounding rectangle of the tab client area.
	// See Also:
	//     GetHeaderRect, GetControlRect
	//-----------------------------------------------------------------------
	CRect GetClientRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab client header area.
	// Returns:
	//     The bounding rectangle of the tab client header area.
	// See Also:
	//     GetClientRect, GetControlRect
	//-----------------------------------------------------------------------
	CRect GetHeaderRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows of tabs in the tab control.
	// Returns:
	//     The number of rows of tabs in the tab control.
	// Remarks:
	//     Only tab controls which have the xtpTabLayoutMultiRow layout
	//     can have multiple rows of tabs.
	// See Also:
	//     SetLayoutStyle, CXTPTabManagerItem::GetItemRow
	//-----------------------------------------------------------------------
	int GetRowCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses either
	//     the left or right navigation buttons.
	// Parameters:
	//     bRight - TRUE to scroll right, FALSE to scroll left.
	//-----------------------------------------------------------------------
	virtual void OnScrollHeader(BOOL bRight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of navigation buttons.
	// Returns:
	//     A pointer to the collection of navigation buttons.
	// See Also:
	//     CXTPTabManagerNavigateButtons, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButtons* GetNavigateButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the next/previous focusable tab item from
	//     a specified index.
	// Parameters:
	//     nIndex     - Index to begin search.
	//     nDirection - Direction to search in (+1 for next, -1 for previous).
	// Returns:
	//     A pointer to the next/previous focusable tab item from
	//     the specified index.
	// Remarks:
	//     A tab item is focusable if it is both visible and enabled.
	// See Also:
	//     CXTPTabManagerItem
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* FindNextFocusable(int nIndex, int nDirection) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the mouse cursor moves.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y- coordinates of the mouse cursor.
	//            These coordinates are always relative to the upper-left
	//            corner of the window.
	// Remarks:
	//     When the mouse cursor is moved across a tab while hot tracking is enabled,
	//     PerformMouseMove will highlight and un-highlight the tabs and then call
	//     CXTPTabManagerNavigateButton::PerformMouseMove for each tab navigation button.
	//
	//     When the mouse cursor is moved across a tab navigation button,
	//     CXTPTabManagerNavigateButton::PerformMouseMove will be called to perform
	//     actions related to the mouse movement over the tab navigation buttons.
	//     This will highlight and un-highlight the tab navigation buttons.
	// See Also:
	//     CXTPTabManagerNavigateButton::PerformMouseMove
	//-----------------------------------------------------------------------
	void PerformMouseMove(HWND hWnd, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the left mouse button.
	// Parameters:
	//     hWnd         - Handle to the CWnd object beneath the mouse cursor.
	//     pt           - Specifies the x- and y- coordinates of the mouse cursor.
	//                    These coordinates are always relative to the upper-left
	//                    corner of the window.
	//     bNoItemClick - TRUE to only test tab navigation buttons
	//                    to determine if they were clicked,
	//                    FALSE to test both tabs and tab navigation buttons
	//                    to determine if they were clicked.
	// Remarks:
	//     When the user clicks on a tab navigation button, this method will
	//     call the CXTPTabManagerNavigateButton::PerformClick member
	//     of the tab navigation button that was clicked.
	//
	//     When the user clicks on a tab, OnItemClick will be called to
	//     select the tab.
	//
	//     If IsAllowReorder is TRUE, then ReOrder will be called before
	//     OnItemClick. ReOrder will check if the tab is being moved and, if
	//     it is being moved, keep the tab selected so OnItemClick is not
	//     called. If the tab is not being moved, then OnItemClick will be called.
	// Returns:
	//    If bNoItemClick is TRUE, then this method will return either
	//    TRUE if a tab navigation button was clicked or
	//    FALSE if a tab navigation button was not clicked.
	//
	//    If bNoItemClick is FALSE, then this method will return either
	//    TRUE if a tab or tab navigation button was clicked or
	//    FALSE if a tab or tab navigation button was not clicked.
	// See Also:
	//     OnItemClick, ReOrder, CXTPTabManager::OnNavigateButtonClick
	//-----------------------------------------------------------------------
	BOOL PerformClick(HWND hWnd, CPoint pt, BOOL bNoItemClick = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which tab is beneath the mouse cursor, if any, and
	//     stores the tooltip information of the tab within a TOOLINFO structure.
	// Parameters:
	//     hWnd  - Handle to the window that contains the tab.
	//     point - Specifies the x- and y- coordinates of the mouse cursor.
	//     pTI   - Pointer to a TOOLINFO structure to receive the tooltip information
	//             of the tab beneath the mouse cursor, if any.
	// Returns:
	//     The identifier of the tab beneath the mouse cursor, if any,
	//     if a tooltip was found, otherwise a value of -1.
	// Remarks:
	//     This member is called in the CXTPTabControl::OnToolHitTest,
	//     CXTPDockingPaneTabbedContainer::OnToolHitTest, and
	//     CXTPTabClientWnd::OnToolHitTest members.
	//-----------------------------------------------------------------------
	virtual INT_PTR PerformToolHitTest(HWND hWnd, CPoint point, TOOLINFO* pTI) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process a WM_KEYDOWN event for
	//     the tab manager.
	// Parameters:
	//     hWnd  - Handle to the window that contains the tab.
	//     nChar - Specifies the virtual-key code of the given key.
	// Returns:
	//     TRUE if the key was processed successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL PerformKeyDown(HWND hWnd, UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the parent background under the tab control.
	// Parameters:
	//     pDC - Pointer to valid device context.
	//     rc  - Bounding rectangle.
	// Returns:
	//     TRUE if the rectangle was filled.
	//-----------------------------------------------------------------------
	virtual BOOL DrawParentBackground(CDC* pDC, CRect rc);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total number of tab navigation buttons within
	//     the collection of tab navigation buttons.
	// Returns:
	//     The total number of tab navigation buttons within
	//     the collection of tab navigation buttons.
	// See Also:
	//     GetNavigateButton, CXTPTabManagerNavigateButton, OnNavigateButtonClick
	//-----------------------------------------------------------------------
	int GetNavigateButtonCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tab navigation button at a specified index within
	//     the collection of tab navigation buttons.
	// Parameters:
	//     nIndex - Zero-based index of the tab navigation button to retrieve.
	// Returns:
	//     A pointer to the tab navigation button at the specified index within
	//     the collection of tab navigation buttons.
	// See Also:
	//     GetNavigateButtonCount, CXTPTabManagerNavigateButton, OnNavigateButtonClick
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* GetNavigateButton(int nIndex) const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Enables/disables markup for the tab captions.
	// Parameters:
	//     bEnable - TRUE to enable markup for the tab captions,
	//               FALSE to disable markup for the tab captions.
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the markup context.
	// Returns:
	//     A pointer to the markup context.
	//-------------------------------------------------------------------------
	virtual CXTPMarkupContext* GetMarkupContext() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified tab navigation button is automatically displayed.
	// Parameters:
	//     pButton - Pointer to the tab navigation button to check.
	// Returns:
	//     TRUE if the specified tab navigation button is automatically displayed,
	//     otherwise FALSE.
	// See Also:
	//     XTPTabNavigateButtonFlags
	//-----------------------------------------------------------------------
	virtual BOOL IsNavigateButtonAutomaticVisible(CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified tab navigation button is visible.
	// Parameters:
	//     pButton - Pointer to the tab navigation button to check.
	// Returns:
	//     TRUE if the specified tab navigation button is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsNavigateButtonVisible(CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a tab button is clicked
	//     to select the tab item that was clicked.
	// Parameters:
	//     pItem - Pointer to the tab item that was clicked.
	// See Also:
	//     PerformClick
	//-----------------------------------------------------------------------
	virtual void OnItemClick(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before a tab item is clicked.
	// Parameters:
	//     pItem - Pointer to the tab item that is about
	//             to be clicked.
	// Returns:
	//     TRUE to cancel the process.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeItemClick(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a tab item is clicked and dragged
	//     within the tab header.
	// Parameters:
	//     hWnd  - Handle to the CWnd object beneath the mouse cursor.
	//     pt    - Specifies the x- and y- coordinates of the mouse cursor.
	//             These coordinates are always relative to the upper-left
	//             corner of the window.
	//     pItem - Pointer to the tab item that is about
	//             to be clicked and dragged.
	// Remarks:
	//     This method will reorder the selected tab item to
	//     the location that it is dragged to.
	// See Also:
	//     PerformClick
	//-----------------------------------------------------------------------
	virtual void ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by ReOrder when two tab items are switched.
	// Parameters:
	//     pItem1 - Pointer to the tab item dragged by the user.
	//     pItem2 - Pointer to the other tab item that was switched.
	//-----------------------------------------------------------------------
	virtual void OnSwitchItem(CXTPTabManagerItem* pItem1, CXTPTabManagerItem* pItem2);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by ReOrder when a tab item's position is changed.
	// Parameters:
	//     pItem     - Pointer to the tab item dragged by the user.
	//     nOldIndex - Old item position.
	//     nNewIndex - New item position.
	//-----------------------------------------------------------------------
	virtual void OnItemOrderChanged(CXTPTabManagerItem* pItem, int nOldIndex, int nNewIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a tab navigation button is clicked.
	// Parameters:
	//     pButton - Pointer to the tab navigation button that was clicked.
	//     nID     - Button identifier.
	//               Can be one of the values listed in the Remarks section.
	// Remarks:
	//     Standard identifiers of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>: Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b>: Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b>: Close tab navigation button.
	//
	//     This method is overridden in CXTPTabClientWnd::CWorkspace and CXTPTabControl
	//     to capture when the tab navigation close "x" button
	//     is pressed and deletes the tab.
	//
	//     This member is called after CXTPTabManagerNavigateButton::PerformClick.
	// See Also:
	//     CXTPTabManagerNavigateButton::PerformClick, XTPTabNavigateButton
	//-----------------------------------------------------------------------
	virtual void OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton);
	virtual void OnNavigateButtonClick(
		UINT nID); // <COMBINE CXTPTabManager::OnNavigateButtonClick@CXTPTabManagerNavigateButton*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the MouseMove event has been disabled.
	// Returns:
	//     Always returns FALSE in the base class implementation.
	// Remarks:
	//     This method is overridden in CXTPTabClientWnd::CWorkspace.
	//
	//     The MouseMove event is disabled when a command bar pop-up
	//     becomes active/visible.
	// See Also:
	//     CXTPTabClientWnd::CWorkspace::IsMouseLocked
	//-----------------------------------------------------------------------
	virtual BOOL IsMouseLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified tab item to the collection of tab items.
	// Parameters:
	//     nItem - Position in the collection of tab items to add
	//             the specified tab item. If the value of
	//             this parameter is less than 0 or greater than
	//             the number of tab items in the collection,
	//             then the specified tab item will be added to
	//             the end of the collection.
	//     pItem - Pointer to the tab item to add, or NULL to
	//             create and add a default tab item.
	// Returns:
	//     A pointer to the newly added tab item.
	// Remarks:
	//     This method is overridden in derived classes to provide
	//     additional functionality and then this base class method is called.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* AddItem(int nItem, CXTPTabManagerItem* pItem = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     The implementation should return TRUE if
	//     the header has focus or FALSE otherwise.
	// Returns:
	//     TRUE if the header has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HeaderHasFocus() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Activates and tracks a specified tab item on mouse click.
	// Parameters:
	//     hWnd  - Handle to the CWnd object beneath the mouse cursor.
	//     pt    - Specifies the x- and y- coordinates of the mouse cursor.
	//             These coordinates are always relative to the upper-left
	//             corner of the window.
	//     pItem - Pointer to the tab item to activate and track.
	//-----------------------------------------------------------------------
	void TrackClick(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the handle of a tab item is changed.
	// Parameters:
	//     pItem   - Pointer to the tab item whose handle was changed.
	//     hwndOld - Previous handle of the tab item.
	//     hwndNew - New handle of the tab item.
	//-----------------------------------------------------------------------
	virtual void OnItemHandleChanged(CXTPTabManagerItem* pItem, HWND hwndOld, HWND hwndNew);

protected:
	CXTPTabManagerItem* m_pSelected;	// Tab currently selected (i.e. when the tab is clicked).
	CXTPTabManagerItem* m_pHighlighted; // Tab currently highlighted
										// (i.e. When the mouse cursor is over the tab button).
	CXTPTabManagerItem* m_pPressed;		// Pointer to the currently pressed tab.

	CXTPTabManagerNavigateButton* m_pHighlightedNavigateButton; // Pointer to highlighted navigate
																// button.

	int m_nHeaderOffset;  // Scroll offset, this is how much the tabs in the tab header have been
						  // scrolled using the left and right tab navigation buttons.
						  // See: GetHeaderOffset, SetHeaderOffset.
	BOOL m_bAllowReorder; // TRUE to allow tabs to be reordered.
	BOOL m_bActive;		  // TRUE if the MDITabClient tab group is active. This only applies to
						  // a TabWorkspace when grouping is enabled.

	XTPTabNavigateButtonFlags m_bCloseItemButton; // TRUE to show the close button for tabs.

	CRect m_rcHeaderRect; // Bounding rectangle of the tab header area.
	CRect m_rcControl;	// Bounding rectangle of the tab manager control. This is the
						  // bounding rectangle for both the tab header and tab client.
	CRect m_rcClient;	 // Bounding rectangle of the tab client area.

	CArray<CXTPTabManagerItem*, CXTPTabManagerItem*> m_arrItems; // Collection of tabs for this tab
																 // manager.
	CXTPTabManagerNavigateButtons m_arrNavigateButtons; // Collection of tab navigation buttons for
														// this tab manager.

	CRowIndexer* m_pRowIndexer;			 // Row indexer.
	int m_nScrollDelta;					 // Scroll header delta.
	CXTPMarkupContext* m_pMarkupContext; // Markup context of the tab manager.

private:
	friend class CXTPTabManagerItem;
	friend class CXTPTabPaintManager;
	friend class CXTPTabManagerNavigateButton;
	friend class CXTPTabClientWnd;
};

//////////////////////////////////////////////////////////////////////////
// CXTPTabManager

AFX_INLINE int CXTPTabManager::GetItemCount() const
{
	return (int)m_arrItems.GetSize();
}
AFX_INLINE CXTPTabManagerItem* CXTPTabManager::GetItem(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetItemCount() ? m_arrItems[nIndex] : NULL;
}
AFX_INLINE int CXTPTabManager::GetNavigateButtonCount() const
{
	return (int)m_arrNavigateButtons.GetSize();
}
AFX_INLINE CXTPTabManagerNavigateButton* CXTPTabManager::GetNavigateButton(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetNavigateButtonCount() ? m_arrNavigateButtons[nIndex] : NULL;
}
AFX_INLINE CXTPTabManagerItem* CXTPTabManager::GetSelectedItem() const
{
	return m_pSelected;
}
AFX_INLINE BOOL CXTPTabManager::IsHorizontalPosition() const
{
	return (GetPosition() == xtpTabPositionBottom || GetPosition() == xtpTabPositionTop);
}
AFX_INLINE int CXTPTabManager::GetRectLength(CRect rc) const
{
	if (IsHorizontalPosition())
		return rc.Width();
	return rc.Height();
}
AFX_INLINE int CXTPTabManager::GetHeaderOffset() const
{
	return m_nHeaderOffset;
}
AFX_INLINE void CXTPTabManager::OnNavigateButtonClick(UINT)
{
}
AFX_INLINE void CXTPTabManager::OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton)
{
	OnNavigateButtonClick(pButton->GetID());
}
AFX_INLINE BOOL CXTPTabManager::IsAllowReorder() const
{
	return m_bAllowReorder;
}
AFX_INLINE void CXTPTabManager::SetAllowReorder(BOOL bAllowReorder)
{
	m_bAllowReorder = bAllowReorder;
}
AFX_INLINE void CXTPTabManager::SetPaintManager(CXTPTabPaintManager* /*pPaintManager*/)
{
}
AFX_INLINE BOOL CXTPTabManager::IsActive() const
{
	return m_bActive;
}
AFX_INLINE CRect CXTPTabManager::GetControlRect() const
{
	return m_rcControl;
}
AFX_INLINE CRect CXTPTabManager::GetClientRect() const
{
	return m_rcClient;
}
AFX_INLINE CRect CXTPTabManager::GetHeaderRect() const
{
	return m_rcHeaderRect;
}
AFX_INLINE BOOL CXTPTabManager::IsMouseLocked() const
{
	return FALSE;
}
AFX_INLINE int CXTPTabManager::GetRowCount() const
{
	return m_pRowIndexer->m_nRowCount;
}
AFX_INLINE BOOL CXTPTabManager::HeaderHasFocus() const
{
	return FALSE;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsFocused() const
{
	return IsSelected() && m_pTabManager->HeaderHasFocus();
}
AFX_INLINE CXTPMarkupUIElement* CXTPTabManagerItem::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}
AFX_INLINE CXTPTabManager::ROW_ITEMS* CXTPTabManager::CRowIndexer::GetRowItems() const
{
	return m_pRowItems;
}
AFX_INLINE CXTPTabManagerNavigateButtons* CXTPTabManager::GetNavigateButtons()
{
	return &m_arrNavigateButtons;
}
AFX_INLINE void CXTPTabManager::OnItemHandleChanged(CXTPTabManagerItem*, HWND, HWND)
{
}
AFX_INLINE BOOL CXTPTabManager::DrawParentBackground(CDC* /*pDC*/, CRect /*rc*/)
{
	return FALSE;
}
AFX_INLINE void CXTPTabManager::ShowCloseItemButton(XTPTabNavigateButtonFlags bCloseItemButton)
{
	m_bCloseItemButton = bCloseItemButton;
}
AFX_INLINE CXTPMarkupContext* CXTPTabManager::GetMarkupContext() const
{
	return m_pMarkupContext;
}
AFX_INLINE void CXTPTabManager::OnSwitchItem(CXTPTabManagerItem* /*pItem1*/,
											 CXTPTabManagerItem* /*pItem2*/)
{
}
AFX_INLINE void CXTPTabManager::OnItemOrderChanged(CXTPTabManagerItem* /*pItem*/, int /*nOldIndex*/,
												   int /*nNewIndex*/)
{
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABMANAGER_H__)
