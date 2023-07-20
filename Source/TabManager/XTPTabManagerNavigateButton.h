// XTPTabManagerNavigateButton.h
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
#if !defined(__XTPTABMANAGERNAVIGATEBUTTON_H__)
#	define __XTPTABMANAGERNAVIGATEBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabManager;
class CXTPTabManagerItem;

//===========================================================================
// Summary:
//     CXTPTabManagerNavigateButton is a class used to represent
//     a tab navigation button.
// Remarks:
//     Navigation buttons are the buttons that appear in the tab header area.
//     The buttons include the left arrow, right arrow, and close buttons.
//     Any combination of these buttons can be shown. You can choose to never
//     display then, always display them, or automatically display them.
//     If you automatically display the buttons, they are only displayed when
//     needed (e.g. when there are too many tabs to be displayed in the
//     tab header, the arrow buttons will appear).
//
//     By default, all these buttons are displayed in the command bars
//     tab workspace. You must add them to the tab control.
//
//     If the xtpTabNavigateButtonAutomatic flag is used, then buttons will
//     only appear when needed (e.g. when there are too many tabs to be
//     displayed in the tab header, the arrow buttons will appear).
// Example:
//     This example code illustrates how to specify when the tab navigation
//     buttons are displayed:
// <code>
// //Finds the left navigation button and specifies that it is always displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonAlways);
// //Finds the right navigation button and specifies that it is never displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);
// //Finds the close navigation button and specifies that it is always displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonClose)->SetFlags(xtpTabNavigateButtonAlways);
//
// //Called to recalculate tab area and reposition components
// Reposition();
// </code>
// See Also:
//     XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManagerNavigateButton
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerNavigateButton object.
	// Parameters:
	//     pManager - CXTPTabManager object to draw the button on.
	//     nID      - ID of the button; can be one of
	//                the values listed in the Remarks section.
	//     dwFlags  - Indicates when to display the button.
	// Remarks:
	//     Standard IDs of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>: Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b>: Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b>: Close tab navigation button.
	// See also:
	//     CXTPTabManager::FindNavigateButton, XTPTabNavigateButtonFlags,
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton(CXTPTabManager* pManager, UINT nID,
								 XTPTabNavigateButtonFlags dwFlags);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerNavigateButton object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabManagerNavigateButton();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the width of the tab header area.
	// Parameters:
	//     nWidth - Reference to the width of the tab header area.
	// Remarks:
	//     If the tabs are horizontal, then the width of the tab header area
	//     is adjusted by subtracting the width of the navigation button from nWidth.
	//
	//     If the tabs are vertical, then the width of the tab header area
	//     is adjusted by subtracting the height of the navigation button from nWidth.
	//
	//     The width will only be adjusted if the xtpTabNavigateButtonAlways flag is set.
	//-----------------------------------------------------------------------
	virtual void AdjustWidth(int& nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies when the navigation button is displayed.
	// Parameters:
	//     dwFlags - Flags that indicate when the navigation button is displayed; must be
	//               one of the values defined by the XTPTabNavigateButtonFlags enumeration.
	// See Also:
	//     XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton, GetFlags
	//-----------------------------------------------------------------------
	void SetFlags(XTPTabNavigateButtonFlags dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines when the navigation button is displayed.
	// Returns:
	//     Flags that indicate when the navigation button is displayed;
	//     one of the values defined by the XTPTabNavigateButtonFlags enumeration.
	// See Also:
	//     XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton, SetFlags
	//-----------------------------------------------------------------------
	XTPTabNavigateButtonFlags GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of the navigation button.
	// Returns:
	//     The size of the navigation button.
	// See Also:
	//     GetRect
	//-----------------------------------------------------------------------
	virtual CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent item of the navigation button.
	// Returns:
	//     A pointer to the parent item of the navigation button.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the navigation button in the tab header area.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the left mouse button.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y- coordinates of the mouse cursor.
	//            These coordinates are always relative to the upper-left
	//            corner of the window.
	// Remarks:
	//     This method performs the common operations of when a button
	//     is clicked (e.g. drawing the "pressed" button and calling
	//     the OnExecute member of the clicked tab navigation button).
	//
	//     When the user clicks on a tab navigation button, this method
	//     will call the OnExecute member of the clicked tab navigation button.
	//     OnExecute will then call CXTPTabManager::OnNavigateButtonClick to
	//     pass in the indentifier of the tab navigation button that was pressed.
	// See Also:
	//     CXTPTabManager::OnNavigateButtonClick
	//-----------------------------------------------------------------------
	virtual void PerformClick(HWND hWnd, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the navigation button is enabled/disabled.
	// Returns:
	//     TRUE if the navigation button is enabled,
	//     FALSE if the navigation button is disabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the pressed state of the navigation button.
	// Returns:
	//     TRUE if the navigation button is pressed, otherwise FALSE.
	// Remarks:
	//     This method is used within both CXTPTabColorSetDefault::FillNavigateButton
	//     and all objects derived from CXTPTabColorSetDefault to determine
	//     how to colorize the navigation button.
	// See Also:
	//     CXTPTabColorSetDefault::FillNavigateButton
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the highlighted state of the navigation button.
	// Remarks:
	//     The navigation button becomes highlighted when
	//     the mouse cursor is positioned over it.
	//
	//     This method is used within both CXTPTabColorSetDefault::FillNavigateButton
	//     and all objects derived from CXTPTabColorSetDefault to determine
	//     how to colorize the navigation button.
	// Returns:
	//     TRUE if the navigation button is highlighted, otherwise FALSE.
	// See Also:
	//     CXTPTabColorSetDefault::FillNavigateButton
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the selected state of the owner tab button.
	// Returns:
	//     TRUE if the owner tab button is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsOwnerSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the highlighted state of the owner tab button.
	// Returns:
	//     TRUE if the owner tab button is highlighted, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsOwnerHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the navigation button is either
	//     a common navigation item or owned by a tab item.
	// Returns:
	//     TRUE if the navigation button is a common navigation item,
	//     FALSE if the navigation button is owned by a tab item.
	//-----------------------------------------------------------------------
	BOOL IsOwnerNull() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this tab group is active/inactive.
	// Returns:
	//     TRUE if this tab group is active, FALSE if this tab group is inactive.
	// Remarks:
	//     This method only applies to tab groups in the MDITabClient
	//     (i.e. the tabs groups that appear in the CommandBars TabWorkspace)
	//     when grouping is enabled.
	//-----------------------------------------------------------------------
	BOOL IsOwnerActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the navigation button.
	// Returns:
	//     TRUE if the navigation button is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the navigation button.
	// Returns:
	//     The bounding rectangle of the navigation button.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bounding rectangle for the navigation button.
	// Parameters:
	//     rcButton - Bounding rectangle to be set.
	//-----------------------------------------------------------------------
	void SetRect(CRect rcButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the navigation button.
	// Returns:
	//     The identifier of the navigation button.
	// Remarks:
	//     See XTPTabNavigateButton for the list of standard identifiers.
	// See Also:
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the position of the navigation button.
	// Parameters:
	//     rcNavigateButtons - Reference to the bounding rectangle of
	//                         the navigation buttons.
	//-----------------------------------------------------------------------
	virtual void Reposition(CRect& rcNavigateButtons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip for the navigation button.
	// Parameters:
	//     lpszTooltip - Tooltip to be set.
	// Remarks:
	//     The tooltip is the text displayed when
	//     the mouse is positioned over the navigation button.
	// See Also:
	//     GetTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip of the navigation button.
	// Remarks:
	//     The tooltip is the text displayed when
	//     the mouse is positioned over the navigation button.
	// Returns:
	//     The tooltip of the navigation button.
	// See Also:
	//     SetTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a symbol in the navigation button
	//     (i.e. either "left", "right" or "x").
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Bounding rectangle of the navigation button.
	//     clr      - Button color.
	//     bEnabled - TRUE if the button is enabled.
	// Remarks:
	//     This method must be overridden in derived classes.
	//-----------------------------------------------------------------------
	virtual void DrawEntry(CDC* pDC, CRect rc, COLORREF clr, BOOL bEnabled) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a navigation button is clicked.
	// Parameters:
	//     bTick - If TRUE, then CXTPTabManager::OnNavigateButtonClick
	//             will be called to pass in the identifier of
	//             the navigation button that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnExecute(BOOL bTick);

protected:
	UINT m_nID;							 // ID of the navigation button.
	XTPTabNavigateButtonFlags m_dwFlags; // Flag that indicates when the navigation button will be
										 // drawn.
	CXTPTabManager* m_pManager; // Pointer to the tab manager the navigation buttons are drawn on.
	CRect m_rcButton;			// Bounding rectangle of the navigation button.

	BOOL m_bEnabled;			 // TRUE if the navigation button is enabled.
	BOOL m_bPressed;			 // TRUE if the navigation button is pressed/clicked.
	CString m_strToolTip;		 // Tooltip for the button.
	CXTPTabManagerItem* m_pItem; // Owner item. NULL if common.

private:
	friend class CXTPTabManager;
	friend class CXTPTabManagerNavigateButtons;
};

//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerNavigateButton

AFX_INLINE void CXTPTabManagerNavigateButton::SetFlags(XTPTabNavigateButtonFlags dwFlags)
{
	m_dwFlags = dwFlags;
}
AFX_INLINE XTPTabNavigateButtonFlags CXTPTabManagerNavigateButton::GetFlags() const
{
	return m_dwFlags;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsEnabled() const
{
	return m_bEnabled;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsPressed() const
{
	return m_bPressed;
}
AFX_INLINE CRect CXTPTabManagerNavigateButton::GetRect() const
{
	return m_rcButton;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsVisible() const
{
	return !m_rcButton.IsRectEmpty();
}
AFX_INLINE UINT CXTPTabManagerNavigateButton::GetID() const
{
	return m_nID;
}
AFX_INLINE void CXTPTabManagerNavigateButton::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strToolTip = lpszTooltip;
}
AFX_INLINE CString CXTPTabManagerNavigateButton::GetTooltip() const
{
	return m_strToolTip;
}
AFX_INLINE CXTPTabManagerItem* CXTPTabManagerNavigateButton::GetItem() const
{
	return m_pItem;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABMANAGERNAVIGATEBUTTON_H__)
