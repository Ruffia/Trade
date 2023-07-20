// XTPRibbonBackstageView.h: interface for the CXTPRibbonBackstageView class.
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
#if !defined(__XTPRIBBONBACKSTAGEVIEW_H__)
#	define __XTPRIBBONBACKSTAGEVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonControlSystemButton;
class CXTPRibbonBackstageCommand;
class CXTPRibbonBackstageTab;
class CXTPRibbonBackstageMenuSeparator;
class CXTPRibbonBackstagePaintManager;
class CXTPCommandBarScrollBarCtrl;

const UINT XTP_BS_TABCHANGED = 0x1100;

// The backstage style.
enum XTPRibbonBackstageStyle
{
	xtpRibbonBackstageOffice2010Style, // Office 2010 like style.
	xtpRibbonBackstageOffice2013Style, // Office 2013 and Office 2016 like style.
	xtpRibbonBackstageOffice2016Style = xtpRibbonBackstageOffice2013Style
};

// This enum defines what should be done for certain pressed key and the current tip level.
enum XTPRibbonBackstageKeyboardTipResult
{
	xtpNoActionForThisLevel = -3, // Nothing to do for the current level.
	xtpExecuteCommand = -2,  // Pressed key corresponds to a command and tips of first level are
							 // shown.
	xtpHideTips		   = -1, // Hide all tips.
	xtpShowSecondLevel = 2,  // Show tips with two letters with the same first letter.
	xtpSelectTab	   = 3   // Pressed key corresponds to a tab and tips of first level are shown.
};

// Defines the current keyboard tip show level.
enum XTPRibbonBackstageKeyboardTipLevel
{
	xtpNoTips		  = -1, // Tips are not shown.
	xtpUserHoldAltKey = 0,  // User pressed and hold ALT key and previous level was xtpNoTips.
	xtpFirstLevel	 = 1,  // Show tips of first level (commands and tabs).
	xtpSecondLevel	= 2,  // Show tips with two letters with the same first letter (commands and
							// tabs).
	xtpTipsOnTabPane = 3	// Show tips for buttons on the currently active tab.
};

//===========================================================================
// Summary:
//     CXTPRibbonBackstageView is a CDialog derived class used for a Backstage parent frame.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonBackstageView : public CXTPCommandBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonBackstageView);

protected:
	CXTPRibbonBackstageView();
	~CXTPRibbonBackstageView();

public:
	static CXTPRibbonBackstageView* AFX_CDECL CreateBackstageView(CXTPCommandBars* pCommandBars);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the currently active tab.
	// Returns:
	//     The currently active tab.
	//-------------------------------------------------------------------------
	CXTPRibbonBackstageTab* GetActiveTab() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the active tab.
	// Parameters:
	//     pTab - New tab to be set.
	//-------------------------------------------------------------------------
	void SetActiveTab(CXTPRibbonBackstageTab* pTab);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the currently focused control.
	// Returns:
	//     The currently focused control.
	//-------------------------------------------------------------------------
	CXTPControl* GetFocusedControl() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the focused control.
	// Parameters:
	//     pFocusedControl - New control to be set.
	//-------------------------------------------------------------------------
	void SetFocusedControl(CXTPControl* pFocusedControl);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the currently highlighted control.
	// Returns:
	//     The currently highlighted control.
	//-------------------------------------------------------------------------
	CXTPControl* GetHighlightedControl() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Determines if a specified control is a backstage command.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the specified control is a backstage command.
	//-------------------------------------------------------------------------
	BOOL IsCommand(CXTPControl* pControl) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Determines if a specified control is a backstage separator.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the specified control is a backstage separator.
	//-------------------------------------------------------------------------
	BOOL IsMenuSeparator(CXTPControl* pControl) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Determines if a specified control is the active tab of the backstage view.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the specified control is the active tab of the backstage view.
	//-------------------------------------------------------------------------
	BOOL IsActiveTab(CXTPControl* pControl) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Determines if a specified contral is the control with the focus input.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the specified control is the control with the focus input.
	//-------------------------------------------------------------------------
	BOOL IsFocusedControl(CXTPControl* pControl) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Determines if the mouse is hovering over a specified control.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the mouse if hovering over the specified control.
	//-------------------------------------------------------------------------
	BOOL IsHighlightedControl(CXTPControl* pControl) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves Image manager associated with the backstage.
	// Returns:
	//     Pointer to the image manager object that stores the images used in the backstage.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function determines which control, if any, is
	//     at a specified point.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the control at the position specified by 'point',
	//     otherwise NULL.
	//-------------------------------------------------------------------------
	CXTPControl* HitTest(CPoint point) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified control from the array of controls
	//     in backstage view.
	// Parameters:
	//     pControl - Control whose index must be retrieved.
	// Returns:
	//     The index of the specified control if successful, otherwise -1.
	//-------------------------------------------------------------------------
	int IndexOf(CXTPControl* pControl) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Executes the action defined by a specified control.
	// Parameters:
	//     pControl - Pointer to a CXTPControl object.
	//-------------------------------------------------------------------------
	void OnExecute(CXTPControl* pControl);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the pane window of the currently active tab.
	// Returns:
	//     A pointer to the pane window of the currently active tab.
	//-------------------------------------------------------------------------
	CWnd* GetActiveControlPane() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the width of the menu bar of the backstage view.
	// Returns:
	//     The width of the menu bar of the backstage view.
	//-------------------------------------------------------------------------
	int GetMenuWidth() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the theme of the backstage view.
	// Parameters:
	//     theme - New theme to be set.
	//-------------------------------------------------------------------------
	void SetTheme(XTPPaintTheme theme);

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the style of the backstage view.
	// Parameters:
	//     style - New style to be set; must be one of the values defined
	//             by the XTPRibbonBackstageStyle enumeration.
	// See Also:
	//     XTPRibbonBackstageStyle
	//-------------------------------------------------------------------------
	void SetBackstageStyle(XTPRibbonBackstageStyle style = xtpRibbonBackstageOffice2016Style);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the style of the backstage view.
	// Returns:
	//     The style of the backstage view; one of the values defined
	//     by the XTPRibbonBackstageStyle enumeration.
	// See Also:
	//     XTPRibbonBackstageStyle
	//-------------------------------------------------------------------------
	XTPRibbonBackstageStyle GetBackstageStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the color of the rectangle in which the keyboard tip symbol
	//     (or symbols) is (or are) drawn.
	// Returns:
	//     The color of the rectangle in which the keyboard tip symbol
	//     (or symbols) is (or are) drawn.
	//-------------------------------------------------------------------------
	COLORREF GetKeyboardTipsBackColor();

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the font for drawing keyboard tips.
	// Returns:
	//     The font for drawing keyboard tips.
	//-------------------------------------------------------------------------
	CFont* GetKeyboardTipsFont();

	//-------------------------------------------------------------------------
	// Summary:
	//     Defines what should be done for the current keyboard input depending
	//     on the current state of the keyboard tips engine.
	// Parameters:
	//	   keyMessage - Code of the Windows keyboard message.
	//	   altPressed - TRUE if the Alt key was pressed.
	//	   key        - Virtual code of the pressed key.
	//	   repeat     - Count of repeated key presses.
	// Returns:
	//     TRUE if the keyboard input does not require further processing.
	//-------------------------------------------------------------------------
	BOOL ProcessKeyboardInput(UINT keyMessage, BOOL altPressed, UINT key,
							  int repeat); // It defines whether keyboard tip engine should be
										   // activated and what should be done for pressed keys.

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if DPI scaling is enabled for icons.
	// Returns:
	//      TRUE is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsDpiIconsScalingEnabled() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(IsDpiIconsScalingEnabled)
	BOOL IsDPIIconsScallingEnabled() const
	{
		return IsDpiIconsScalingEnabled();
	}
	//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     Overrides the CWnd::GetScrollBarCtrl virtual method.
	//-------------------------------------------------------------------------
	virtual CScrollBar* GetScrollBarCtrl(int nBar);
	virtual CScrollBar*
		GetScrollBarCtrl(int nBar) const; // <combine CXTPRibbonBackstageView::GetScrollBarCtrl@int>

public:
	CXTPRibbonBackstageCommand* AddCommand(UINT nID);
	CXTPRibbonBackstageTab* AddTab(CWnd* pWnd, UINT nID);
	CXTPRibbonBackstageMenuSeparator* AddMenuSeparator(UINT nID = UINT_MAX);

public:
	virtual void OnCancel();

protected:
	virtual void RecalcLayout();

	BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

	void ShowBackstageView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Guarantees that keyboard tips are set for all menu items in the backstage.
	//     It also checks that keyboard tips do not conflict with each other.
	//-----------------------------------------------------------------------
	void CheckKeyboardTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     Guarantees that keyboard tips are set for all buttons in all tab panes
	//     in the backstage view.
	//-----------------------------------------------------------------------
	void CheckTabsKeyboardTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines which control in the backstage corresponds to the
	//     pressed keyboard key (according to the keyboard key) and which action
	//     corresponds to both this key and the current SHOW KEYBOARD TIPS level.
	// Parameters:
	//     key           - Virtual code of the pressed key.
	//     nControlIndex - Receives the index of the control corresponding to the
	//                     pressed key in the array of controls in the backstage.
	// Returns:
	//     An XTPRibbonBackstageKeyboardTipResult enumeration value defining
	//     the action that corresponds to both the pressed key and the
	//     current SHOW KEYBOARD TIPS level.
	// See Also:
	//     XTPRibbonBackstageKeyboardTipResult, XTPRibbonBackstageKeyboardTipLevel
	//-----------------------------------------------------------------------
	XTPRibbonBackstageKeyboardTipResult TranslateKeyboardTip(UINT key, int& nControlIndex);

protected:
	DECLARE_MESSAGE_MAP();

	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam,
							  LRESULT& lResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonBackstageView);

	afx_msg LPDISPATCH OleAddTab(long nId, LPCTSTR lpszCaption, OLE_HANDLE hWnd);
	afx_msg LPDISPATCH OleAddCommand(long nId, LPCTSTR lpszCaption);
	afx_msg LPDISPATCH OleAddMenuSeparator();
	afx_msg int OleGetStyle();
	afx_msg void OleSetStyle(int NewValue);
	afx_msg void OleClose();
	afx_msg void OleSetTheme(long nTheme);
	afx_msg COLORREF OleGetColorForeground() const;
	afx_msg COLORREF OleGetColorBackground() const;

//}}AFX_CODEJOCK_PRIVATE
#	endif
protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Updates scrollbar information.
	//-------------------------------------------------------------------------
	void UpdateScrollInfo();

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the position of the vertical scrollbar.
	// Parameters:
	//     nScrollPos - New scrollbar position to be set.
	//-------------------------------------------------------------------------
	void SetScrollPos(int nScrollPos);
	void RepositionControls(int dy);

public:
	int m_nMenuWidth;

	CXTPFont m_xtpFontCommand;
	CXTPFont m_xtpFontTab;

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntCommand, m_xtpFontCommand,
										  GetCommandFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntTab, m_xtpFontTab, GetTabFontHandle);

	COLORREF m_clrText;
	COLORREF m_clrBack;
	XTPPaintTheme m_themeCurrent;
	CXTPCommandBarScrollBarCtrl m_wndScrollBar;

	CXTPRibbonBackstagePaintManager* m_pPaintManager;

	CXTPRibbonControlSystemButton* m_pSystemButton;
	CXTPRibbonBackstageTab* m_pActiveTab;
	CXTPControl* m_pHighlightedControl;
	CXTPControl* m_pFocusedControl;
	CArray<HWND, HWND> m_arrChildWindows;
	CList<int, int> m_listControlsToBeVisible;

protected:
	int m_nScrollPos;
	int m_nPageSize;
	int m_nMax;
	int m_ScrolLine;
	BOOL m_bIsScrollBarVisible;
	CRect m_rcOldPaneRect;
	XTPRibbonBackstageStyle m_backStageStyle;
	BOOL m_bRestoreQuickAccess;
	CRect m_rcOldSysButtonRect;
	XTPRibbonBackstageKeyboardTipLevel m_nKeyboardTipsLevel; // See
															 // XTPRibbonBackstageKeyboardTipLevel
															 // definition.
	TCHAR m_chSecondLevelCurrentChar; // It is used when keyboard tips with 2 letters are shown
									  // (m_nKeyboardTipsLevel == xtpSecondLevel). It defines the
									  // first character in such tips.
};

AFX_INLINE CXTPImageManager* CXTPRibbonBackstageView::GetImageManager() const
{
	return CXTPCommandBar::GetImageManager();
}

AFX_INLINE int CXTPRibbonBackstageView::GetMenuWidth() const
{
	return m_nMenuWidth;
}

AFX_INLINE XTPRibbonBackstageStyle CXTPRibbonBackstageView::GetBackstageStyle() const
{
	return m_backStageStyle;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONBACKSTAGEVIEW_H__)
