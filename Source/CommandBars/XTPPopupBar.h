// XTPPopupBar.h : interface for the CXTPPopupBar class.
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
#if !defined(__XTPPOPUPBAR_H__)
#	define __XTPPOPUPBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPControlPopup;
class CXTPCommandBars;

//===========================================================================
// Summary:
//     CXTPPopupBar is a CXTPCommandBar derived class.
//     It represents a submenu of a menu bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupBar : public CXTPCommandBar
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_XTP_COMMANDBAR(CXTPPopupBar)
	//}}AFX_CODEJOCK_PRIVATE

private:
	class CControlExpandButton;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupBar object.
	//-----------------------------------------------------------------------
	CXTPPopupBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a pop-up bar object.
	// Parameters:
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	// Returns:
	//     A pointer to a CXTPPopupBar object.
	//-----------------------------------------------------------------------
	static CXTPPopupBar* AFX_CDECL CreatePopupBar(CXTPCommandBars* pCommandBars);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method adds the tear-off state to the pop-up bar.
	// Parameters:
	//     lpszCaption - Caption of the tear-off toolbar.
	//     nID - Identifier of the toolbar to be created.
	//     nWidth - Width of the toolbar.
	//-----------------------------------------------------------------------
	void SetTearOffPopup(LPCTSTR lpszCaption, UINT nID, int nWidth = 400);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the pop-up bar has the tear-off state.
	// Parameters:
	//     strCaption - Caption of the tear-off toolbar.
	//     nID - Identifier of the toolbar to be created.
	//     nWidth - Width of the toolbar.
	// Returns:
	//     TRUE if the pop-up bar has the tear-off state, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsTearOffPopup(CString& strCaption, UINT& nID, int& nWidth);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the pop-up menu is visible/displayed.
	// Returns:
	//     TRUE if the pop-up menu is visible/displayed,
	//     FALSE if the pop-up menu is currently hidden.
	//-------------------------------------------------------------------------
	virtual BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method opens the pop-up bar.
	// Parameters:
	//     xPos          - Specifies the logical x coordinate of the pop-up bar
	//     yPos          - Specifies the logical y coordinate of the position.
	//     rcExclude     - Excluded area.
	//     pControlPopup - Pointer to a CXTPControlPopup object.
	//     bSelectFirst  - TRUE to select the first item.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Popup(int xPos, int yPos, LPCRECT rcExclude);
	BOOL Popup(CXTPControlPopup* pControlPopup,
			   BOOL bSelectFirst = FALSE); // <combine CXTPPopupBar::Popup@int@int@LPCRECT>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the control's pop-up.
	// Returns:
	//     A pointer to a CXTPControlPopup object.
	//-----------------------------------------------------------------------
	CXTPControlPopup* GetControlPopup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method expands the bar.
	//-----------------------------------------------------------------------
	void ExpandBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to destroy the window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DestroyWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to make the pop-up bar look like a pop-up toolbar.
	// Parameters:
	//     bToolBarType - TRUE to make the pop-up bar as a pop-up toolbar.
	//-----------------------------------------------------------------------
	void SetPopupToolBar(BOOL bToolBarType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the default menu item for the specified pop-up bar.
	// Parameters:
	//     uItem  - Identifier or position of the new default menu item or
	//              -1 for no default item. The meaning of this parameter
	//              depends on the value of fByPos.
	//     fByPos - Value specifying the meaning of uItem. If this parameter
	//              is FALSE, uItem is a menu item identifier. Otherwise,
	//              it is a menu item position.
	//-----------------------------------------------------------------------
	void SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the default menu item on the specified pop-up bar.
	// Parameters:
	//     gmdiFlags - Reserved. Should be NULL.
	//     fByPos    - Value specifying whether to retrieve the menu item's
	//                 identifier or its position. If this parameter is
	//                 FALSE, the identifier is returned. Otherwise, the
	//                 position is returned.
	// Returns:
	//     If the function succeeds, the return value is the identifier
	//     or position of the menu item. If the function fails, the return
	//     value is -1.
	//-----------------------------------------------------------------------
	UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the pop-up bar has a double gripper.
	// Returns:
	//     TRUE if the pop-up bar has a double gripper, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDoubleGripper() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a double gripper for the pop-up bar.
	// Parameters:
	//     bDoubleGripper - TRUE to set a double gripper for the pop-up bar.
	//-----------------------------------------------------------------------
	void SetDoubleGripper(BOOL bDoubleGripper = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the command bar is a CXTPPopupBar.
	// Returns:
	//     TRUE if the command bar is a CXTPPopupBar, otherwise FALSE.
	// See Also:
	//     CXTPCommandBar, CXTPPopupBar
	//-----------------------------------------------------------------------
	virtual BOOL IsPopupBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the pop-up bar is a context menu.
	// Returns:
	//     TRUE if the pop-up bar is a context menu.
	// See Also:
	//     CXTPCommandBarsContextMenus
	//-----------------------------------------------------------------------
	BOOL IsContextMenu() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Translates all messages in the message queue.
	//-------------------------------------------------------------------------
	void PumpMessage();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//     nLength - The requested dimension of the control bar, either
	//               horizontal or vertical, depending on dwMode.
	//     dwMode - See CControlBar::CalcDynamicLayout for a list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the pop-up bar.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to calculate the destination rectangle of the pop-up bar.
	// Parameters:
	//     ptPopup - Point to display the pop-up bar.
	//     sz - Size of the pop-up bar.
	// Returns:
	//     Rectangle where the pop-up bar will be located.
	//-----------------------------------------------------------------------
	virtual CRect CalculatePopupRect(CPoint ptPopup, CSize sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the pop-up direction.
	// Parameters:
	//     dwPopupFlags - Direction of pop-up to open. See XTPPopupDirection enumerator.
	// See Also:
	//     XTPPopupDirection
	//-----------------------------------------------------------------------
	void SetPopupFlags(DWORD dwPopupFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode        - TRUE to set the tracking mode, otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is pop-up'ed by the keyboard.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	// See Also:
	//     IsTrackingMode.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Redraw the pop-up bar.
	//-----------------------------------------------------------------------
	virtual void DelayRedraw()
	{
		Redraw();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select the specified control.
	// Parameters:
	//     nSelected - An integer index of the item to be selected.
	//     bKeyboard - TRUE if the item was selected using the keyboard.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetSelected(int nSelected, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the pop-up bar's position.
	// Parameters:
	//     sz - Size of the pop-up bar.
	//-----------------------------------------------------------------------
	void UpdateLocation(CSize sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Update flags.
	//-----------------------------------------------------------------------
	virtual void UpdateFlags();

	//-----------------------------------------------------------------------
	// Summary:
	//     Update expanding state.
	//-----------------------------------------------------------------------
	void UpdateExpandingState();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcClipBox - The rectangular area of the control that is invalid.
	//-----------------------------------------------------------------------
	virtual void DrawCommandBar(CDC* pDC, CRect rcClipBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the controls array is changed.
	//-----------------------------------------------------------------------
	virtual void OnControlsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to recalculate the command bar layout.
	//-----------------------------------------------------------------------
	virtual void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the parent command bar.
	// Returns:
	//     A pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetParentCommandBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method make a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     TRUE if the key was handled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates rectangle to set the position where the child pop-up bar becomes visible.
	// Parameters:
	//     rc - CRect object specifying the size of the area.
	//     bVertical - TRUE if the control is located vertically in its parent.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, BOOL bVertical);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the toolbar's borders.
	// Parameters:
	//     rcBorders - Borders of the toolbar.
	//-----------------------------------------------------------------------
	void SetBorders(CRect rcBorders);

	//-----------------------------------------------------------------------
	// Summary:
	//     Disables shadow use.
	//-----------------------------------------------------------------------
	void DisableShadow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the control can be resized by the user.
	// Returns:
	//     TRUE if the control can be resized by the user, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsResizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the border margins of the command bar.
	// Returns:
	//     A CRect object containing the border margins.
	//-----------------------------------------------------------------------
	virtual CRect GetBorders();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPPopupBar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnFloatStatus(WPARAM wParam, LPARAM);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Animates pop-up window
	//-------------------------------------------------------------------------
	virtual void Animate();

private:
	void AdjustScrolling(LPSIZE lpSize, BOOL bDown = FALSE, BOOL bInvalidate = FALSE);
	void TrackTearOff();
	void TrackResize();
	void RecalcSizeLayout();
	void SwitchTearOffTracking(BOOL bShow, CPoint point);
	void _MakeSameWidth(int nStart, int nLast, int nWidth);
	BOOL _MouseInResizeGripper(const POINT& pt);

	XTPAnimationType GetAnimationType() const;

public:
	static double m_dMaxWidthDivisor; // Maximum available width of pop-up.

protected:
	CXTPControlPopup* m_pControlPopup; // Control that pop-up'ed the bar.
	CPoint m_ptPopup;				   // Position of the bar.
	CRect m_rcExclude;				   // Excluded area.
	DWORD m_popupFlags;				   // Flags of the bar.
	BOOL m_bDynamicLayout;			   // TRUE if DynamicLayout routine executed.
	BOOL m_bCollapsed;				   // TRUE if pop-up bar is collapsed.
	BOOL m_bExpanded;				   // TRUE if pop-up bar is expanded.
	BOOL m_bExpanding;				   // TRUE if pop-up bar is expanding.

	BOOL m_bTearOffPopup;	 // TRUE if pop-up bar has tear-off option.
	CRect m_rcTearOffGripper; // Tear-off gripper position.
	CRect m_rcResizeGripper;  // Resizable gripper position.
	BOOL m_bResizable;		  // TRUE to allow resize

	BOOL m_bTearOffSelected;	 // TRUE if gripper selected.
	UINT_PTR m_nTearOffTimer;	// Tear-off timer.
	BOOL m_bTearOffTracking;	 // Tear-off tracking mode.
	CSize m_szTearOffBar;		 // Size of the bar.
	CXTPToolBar* m_pTearOffBar;  // Tear-off toolbar pointer.
	CString m_strTearOffCaption; // Caption of the Tear-off toolbar.
	UINT m_nTearOffID;			 // ID of the Tear-off toolbar.
	int m_nTearOffWidth;		 // Width of the Tear-off toolbar.
	BOOL m_bShowShadow;			 // TRUE to show the shadow.
	CRect m_rcBorders;			 // Borders of the bar.
	BOOL m_bDoubleGripper;		 // TRUE if pop-up is state pop-up type.
	BOOL m_bContextMenu;		 // TRUE if pop-up bar is context menu.

	int m_nMaxHeight; // Maximum height the pop-up bar can become.

private:
	XTP_SCROLLINFO m_scrollInfo;

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	DECLARE_DISPATCH_MAP()
#	endif

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	friend class CControlExpandButton;
	friend class CXTPControlPopup;
	friend class CXTPCommandBars;
	friend class CXTPCommandBar;
	friend class CXTPCommandBarsContextMenus;
};

//===========================================================================
// Summary:
//     CXTPPopupToolBar is a CXTPPopupBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupToolBar : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPPopupToolBar)

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupToolBar object.
	//-----------------------------------------------------------------------
	CXTPPopupToolBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a pop-up toolbar object.
	// Parameters:
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	// Returns:
	//     A pointer to a CXTPPopupToolBar object.
	//-----------------------------------------------------------------------
	static CXTPPopupToolBar* AFX_CDECL CreatePopupToolBar(CXTPCommandBars* pCommandBars);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either
	//                horizontal or vertical, depending on dwMode.
	//      dwMode - See CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD dwMode);
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPPopupBar::DisableShadow()
{
	m_bShowShadow = FALSE;
}
AFX_INLINE void CXTPPopupBar::SetBorders(CRect rcBorders)
{
	m_rcBorders = rcBorders;
}
AFX_INLINE BOOL CXTPPopupBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
									 const RECT& rect, CWnd* pParentWnd, UINT nID,
									 CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPPopupBar::IsDoubleGripper() const
{
	return m_bDoubleGripper;
}
AFX_INLINE void CXTPPopupBar::SetDoubleGripper(BOOL bDoubleGripper /*= TRUE*/)
{
	m_bDoubleGripper = bDoubleGripper;
}
AFX_INLINE CXTPControlPopup* CXTPPopupBar::GetControlPopup() const
{
	return m_pControlPopup;
}
AFX_INLINE BOOL CXTPPopupBar::IsPopupBar() const
{
	return TRUE;
}
AFX_INLINE BOOL CXTPPopupBar::IsContextMenu() const
{
	return m_bContextMenu;
}
AFX_INLINE BOOL CXTPPopupBar::IsResizable() const
{
	return m_bResizable;
}
AFX_INLINE void CXTPPopupBar::SetPopupFlags(DWORD dwPopupFlags)
{
	m_popupFlags = dwPopupFlags;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPOPUPBAR_H__)
