// XTPTabManagerItem.h
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
#if !defined(__XTPTABMANAGERITEM_H__)
#	define __XTPTABMANAGERITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupUIElement;
class CXTPImageManagerIcon;
class CXTPTabManagerNavigateButtons;
class CXTPTabManager;

//===========================================================================
// Summary:
//     CXTPTabManagerItem is the base class for the tabs that appear in
//     the command bars tab workspace, docking panes, and tab control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManagerItem : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPTabManagerItem)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerItem object.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerItem object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabManagerItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text caption for this tab.
	// Parameters:
	//     lpszCaption - Text caption to be set.
	// Remarks:
	//     The text caption is the text displayed in the tab button.
	// See Also:
	//     GetCaption
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption of this tab.
	// Remarks:
	//     The text caption is the text displayed in the tab button.
	// Returns:
	//     The text caption of this tab.
	// See Also:
	//     SetCaption
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip for this tab.
	// Parameters:
	//     lpszTooltip - Tooltip to be set.
	// Remarks:
	//     The tooltip is the text displayed when
	//     the mouse is positioned over the tab button.
	// See Also:
	//     GetTooltip, CXTPTabManager::GetItemTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip of this tab.
	// Remarks:
	//     The tooltip is the text displayed when
	//     the mouse is positioned over the tab button.
	// Returns:
	//     The tooltip of this tab.
	// See Also:
	//     SetTooltip, CXTPTabManager::GetItemTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the color of the tab button.
	// Returns:
	//     The color of the tab button.
	// See Also:
	//     SetColor, CXTPTabManager::GetItemColor
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the color for the tab button.
	// Parameters:
	//     clr - Color to be set.
	// Remarks:
	//     This method calls CXTPTabManager::RedrawControl after the color is set.
	// See Also:
	//     GetColor, CXTPTabManager::GetItemColor
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab button.
	// Returns:
	//     The bounding rectangle of the tab button.
	// See Also:
	//     SetRect
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width of the tab button.
	// Returns:
	//     The width of the tab button.
	// Remarks:
	//     The width of the tab button may change depending on
	//     the layout and position of the tabs.
	// See Also:
	//     XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	int GetButtonLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width of the content of the tab button.
	// Returns:
	//     The width of the content of the tab button.
	// Remarks:
	//     The width of the content of the tab button may change depending on
	//     the layout and position of the tabs.
	// See Also:
	//     XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	int GetContentLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of this tab within the collection of tabs.
	// Returns:
	//     The index of this tab within the collection of tabs.
	// See Also:
	//     CXTPTabManager::GetItem, CXTPTabManager
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the image displayed in the tab button.
	// Returns:
	//     The index of the image displayed in the tab button.
	// Remarks:
	//     This is the index of the image within either
	//     a CXTPImageManager or CImageList object.
	//
	//     CXTPTabManager::ShowIcons is used to show/hide icons.
	// See Also:
	//     SetImageIndex, CXTPTabControl::InsertItem, CXTPTabControl::GetImageManager,
	//     CXTPTabControl::SetImageList, CXTPTabManager::ShowIcons
	//-----------------------------------------------------------------------
	int GetImageIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index for the image displayed in the tab button.
	// Parameters:
	//     nImage - Index to be set.
	// Remarks:
	//     This is the index of the image within either
	//     a CXTPImageManager or CImageList object.
	//
	//     CXTPTabManager::ShowIcons is used to show/hide icons.
	// See Also:
	//     GetImageIndex, CXTPTabControl::InsertItem, CXTPTabControl::GetImageManager,
	//     CXTPTabControl::SetImageList, CXTPTabManager::ShowIcons
	//-----------------------------------------------------------------------
	void SetImageIndex(int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the application-supplied, 32-bit value associated with
	//     the tab item.
	// Remarks:
	//     This method returns the "extra" information, such as pointers
	//     and/or integers, that was associated with the tab item using
	//     the SetData function.
	// Note:
	//     Do not use GetData when working with docking pane tabs.
	//     With docking panes, GetData returns a pointer to a CXTPDockingPane.
	//     Use CXTPDockingPane::GetPaneData when working with docking panes.
	// Returns:
	//     The 32-bit value associated with the tab item.
	// See Also:
	//     SetData, CXTPDockingPane::SetPaneData, CXTPDockingPane::GetPaneData
	//-----------------------------------------------------------------------
	DWORD_PTR GetData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the 32-bit value associated with the tab item.
	// Parameters:
	//     dwData - Value to be set.
	// Remarks:
	//     This method allows for "extra" information, such as pointers
	//     and/or integers to be associated with the tab item.
	// Note:
	//     Do not use SetData when working with docking pane tabs.
	//     With docking panes, dwData stores a pointer to a CXTPDockingPane.
	//     Use CXTPDockingPane::SetPaneData when working with docking panes.
	// See Also:
	//     SetData, CXTPDockingPane::SetPaneData
	//-----------------------------------------------------------------------
	void SetData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the handle for the object displayed in the tab client area
	//     when this tab is selected.
	// Parameters:
	//     hWnd - Handle of the object to be set.
	// See Also:
	//     GetHandle
	//-----------------------------------------------------------------------
	void SetHandle(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the handle of the object displayed in the tab client area
	//     when this tab is selected.
	// Returns:
	//     The handle of the object displayed in the tab client area
	//     when this tab is selected.
	// See Also:
	//     SetHandle
	//-----------------------------------------------------------------------
	HWND GetHandle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bounding rectangle for this tab.
	// Parameters:
	//     rcItem - Bounding rectangle to be set.
	// See Also:
	//     GetRect
	//-----------------------------------------------------------------------
	void SetRect(CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this tab is shown/hidden.
	// Returns:
	//     TRUE if this tab is shown, FALSE if this tab is hidden.
	// Remarks:
	//     A tab may not be visible due to the tab layout.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides this tab.
	// Parameters:
	//     bVisible - TRUE to show this tab, FALSE to hide this tab.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this tab is enabled/disabled.
	// Returns:
	//     TRUE if this tab is enabled, FALSE if this tab is disabled.
	// See Also:
	//     SetEnabled
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables this tab.
	// Parameters:
	//     nEnabled - TRUE to enable this tab, FALSE to disable this tab.
	// Remarks:
	//     If a tab is disabled, then it will appear "grayed out"
	//     and will be inaccessible to the user.
	// See Also:
	//     IsEnabled
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row of this item.
	// Returns:
	//     The row of this item.
	//-----------------------------------------------------------------------
	int GetItemRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this item has shrinked.
	// Returns:
	//     TRUE if this item has shrinked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemShrinked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of navigate buttons.
	// Returns:
	//     A pointer to the collection of navigate buttons.
	// See Also:
	//     CXTPTabManagerNavigateButtons, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButtons* GetNavigateButtons();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the selected state of this tab
	//     (i.e. if this tab is clicked).
	// Returns:
	//     TRUE if this tab is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the focused state of this tab.
	// Returns:
	//     TRUE if this tab has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the highlighted state of this tab.
	// Remarks:
	//     A tab is highlighted when CXTPTabPaintManager::m_bHotTracking
	//     is TRUE and either the mouse pointer is positioned over
	//     the tab button or the tab is being dragged (i.e. when reordering tabs).
	// Returns:
	//     TRUE if this tab is highlighted, otherwise FALSE.
	// See Also:
	//     IsPressed, IsFocused
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the pressed state of this tab.
	// Returns:
	//     TRUE if this tab is pressed, otherwise FALSE.
	// See Also:
	//     IsHighlighted
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes this tab.
	//-----------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects this tab.
	// Remarks:
	//     This method calls OnItemClick.
	//
	//     A tab can also be selected by clicking on it.
	//-----------------------------------------------------------------------
	void Select();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the position for this item.
	// Parameters:
	//     nIndex - Position to be set.
	//-----------------------------------------------------------------------
	void Move(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this tab is closable.
	// Returns:
	//     TRUE if this tab can be closed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsClosable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if this tab should be closable.
	// Parameters:
	//     bClosable - TRUE to allow this tab to be closed,
	//                 FALSE to disallow this tab from being closed.
	//-----------------------------------------------------------------------
	void SetClosable(BOOL bClosable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Markup element used to render the caption of this tab.
	// Returns:
	//     A pointer to the Markup element used to render the caption of this tab.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this tab is new window.
	// Returns:
	//     TRUE if this tab is new window, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsNewWindowTab();

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if this tab is new window.
	//-----------------------------------------------------------------------
	void SetNewWindowTab();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab manager.
	// Remarks:
	//     The tab manager is used to manage all CXTPTabManagerItem objects.
	// Returns:
	//     A pointer to the tab manager.
	//-----------------------------------------------------------------------
	CXTPTabManager* GetTabManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the icon of a tab.
	// Parameters:
	//     pDC    - Pointer to device context to draw.
	//     rcIcon - Bounding rectangle of the icon.
	//     pImage - Pointer to the image associated with the tab.
	// See Also:
	//     CXTPImageManagerIcon
	//-----------------------------------------------------------------------
	void DrawImage(CDC* pDC, CRect rcIcon, CXTPImageManagerIcon* pImage);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the position of the tab manager.
	//-----------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is removed from
	//     the tab manager collection.
	//-----------------------------------------------------------------------
	virtual void OnRemoved();

private:
	BOOL DrawRotatedImage(CDC* pDC, CRect rcItem, CXTPImageManagerIcon* pImage);

protected:
	int m_nButtonLength;		   // Width of the tab button.
	int m_nContentLength;		   // Width of the content of the tab button.
	int m_nImage;				   // Image index; this is the index of the image within
								   // a CXTPImageManger or CImageList object.
	int m_nIndex;				   // Index of this tab within the collection of tabs.
	BOOL m_bEnabled;			   // TRUE if this tab is enabled.
	BOOL m_bVisible;			   // TRUE if this tab is visible.
	HWND m_hWnd;				   // CWnd pointer to the window associated with this tab.
	HICON m_hIcon;				   // Icon associated with this tab.
	DWORD_PTR m_dwData;			   // Developer specific data.
	CRect m_rcItem;				   // Rectangle of this tab.
	CString m_strCaption;		   // User specified label for this tab.
	CString m_strToolTip;		   // Tooltip text for this tab.
	COLORREF m_clrItem;			   // Color of the tab button.
	CXTPTabManager* m_pTabManager; // Pointer to the parent item.
	int m_nItemRow;				   // Row of this item.
	BOOL m_bClosable;			   // TRUE if this item can be closed.
	BOOL m_bNewWindowTab;		   // TRUE if this item is new window tab.

	CXTPTabManagerNavigateButtons m_arrNavigateButtons; // Array of navigate buttons.
	CXTPMarkupUIElement* m_pMarkupUIElement;			// Markup element pointer.

private:
	BOOL m_bFound;

	friend class CXTPTabManager;
	friend class CXTPTabPaintManager;
	friend class CXTPTabWorkspace;
	friend class CXTPTabClientWnd;
	friend class CXTPGroupClientWnd;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR lpszNewValue);
	afx_msg long OleGetIndex();
	afx_msg long OleGetTag();
	afx_msg void OleSetTag(long nNewValue);
	afx_msg BOOL OleGetSelected();
	afx_msg void OleSetSelected(BOOL bNewValue);
	afx_msg long OleGetImage();
	afx_msg void OleSetImage(long nNewValue);
	afx_msg void OleSetHandle(long hWnd);
	afx_msg void OleSetVisible(BOOL bVisible);
	afx_msg void OleGetRect(long* Left, long* Top, long* Right, long* Botton);
	afx_msg void OleEnsureVisible();
	DECLARE_DISPATCH_MAP();
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTabManagerItem)
	COleVariant m_oleTag;
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerItem

AFX_INLINE CXTPTabManager* CXTPTabManagerItem::GetTabManager() const
{
	return m_pTabManager;
}
AFX_INLINE CRect CXTPTabManagerItem::GetRect() const
{
	return m_rcItem;
}
AFX_INLINE int CXTPTabManagerItem::GetIndex() const
{
	return m_nIndex;
}
AFX_INLINE DWORD_PTR CXTPTabManagerItem::GetData() const
{
	return m_dwData;
}
AFX_INLINE void CXTPTabManagerItem::SetData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}
AFX_INLINE int CXTPTabManagerItem::GetImageIndex() const
{
	return m_nImage;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsClosable() const
{
	return m_bClosable;
}
AFX_INLINE int CXTPTabManagerItem::GetButtonLength() const
{
	return m_nButtonLength;
}
AFX_INLINE int CXTPTabManagerItem::GetContentLength() const
{
	return m_nContentLength;
}
AFX_INLINE void CXTPTabManagerItem::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strToolTip = lpszTooltip;
}
AFX_INLINE int CXTPTabManagerItem::GetItemRow() const
{
	return m_nItemRow;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsItemShrinked() const
{
	return m_nButtonLength < m_nContentLength;
}
AFX_INLINE CXTPTabManagerNavigateButtons* CXTPTabManagerItem::GetNavigateButtons()
{
	return &m_arrNavigateButtons;
}
AFX_INLINE void CXTPTabManagerItem::SetNewWindowTab()
{
	m_bNewWindowTab = TRUE;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsNewWindowTab()
{
	return m_bNewWindowTab;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABMANAGERITEM_H__)
