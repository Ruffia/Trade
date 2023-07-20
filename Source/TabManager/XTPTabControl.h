// XTPTabControl.h: interface for the CXTPTabControl class.
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
#if !defined(__XTPTABCONTROL_H__)
#	define __XTPTABCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabPaintManager;
class CXTPImageManager;
class CXTPToolTipContext;

//===========================================================================
// Summary:
//     CXTPTabControl is used to draw an enhanced tab control with
//     Office 2003, Visual Studio, or Win XP tabs.
// See Also:
//     Create, CXTPTabManager::SetColor, CXTPTabManager::SetAppearance
//===========================================================================
class _XTP_EXT_CLASS CXTPTabControl
	: public CWnd
	, public CXTPTabManager
{
private:
	class CTabControlDropTarget;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabControl object.
	//-----------------------------------------------------------------------
	CXTPTabControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabControl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a tab at a specified index.
	// Parameters:
	//     nItem     - Index of the new tab.
	//     lpszItem  - Caption of the new tab.
	//     hwndChild - Handle of the object to be displayed in the tab
	//                 (e.g. a dialog page).
	//     nImage    - Identifier of the image resource to display in
	//                 the tab if icons are displayed.
	//     pAddItem  - Pointer to the item to insert, or NULL to
	//                 create and add a default item.
	// Returns:
	//     A pointer to the newly inserted CXTPTabManagerItem object.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* InsertItem(int nItem, LPCTSTR lpszItem, HWND hwndChild = NULL,
								   int nImage = -1, CXTPTabManagerItem* pAddItem = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the tab control.
	// Parameters:
	//     dwStyle    - Style of the tab control.
	//     rect       - Reference to the size of the tab control.
	//     pParentWnd - Pointer to the parent of the tab control.
	//     nID        - Identifier of the tab control.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     CXTPTabControl::CXTPTabControl, GetPaintManager, CXTPTabManager::SetPosition,
	//     CXTPTabManager::SetLayout, GetImageManager
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

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
	virtual CXTPTabPaintManager* GetPaintManager() const;

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
	//     Recalculates the layout of the tab manager and then repositions itself.
	// Remarks:
	//     This method must be overridden in derived classes.
	//-------------------------------------------------------------------------
	virtual void Reposition();

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
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects a specified tab.
	// Parameters:
	//     pItem - Pointer to the tab to be selected.
	//-----------------------------------------------------------------------
	virtual void SetSelectedItem(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a tab button is clicked
	//     to select the tab item that was clicked.
	// Parameters:
	//     pItem - Pointer to the tab item that was clicked.
	// See Also:
	//     PerformClick
	//-----------------------------------------------------------------------
	void OnItemClick(CXTPTabManagerItem* pItem);

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
						  CSize& szIcon) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icons of the tabs.
	// Parameters:
	//     pImageList - Pointer to an image list containing the icons of the tabs.
	// Remarks:
	//     It is recommended to use both the GetImageManager and
	//     CXTPImageManager::SetIcons methods to add icons.
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image manager containing the icons of the tabs.
	// Returns:
	//     A pointer to the image manager containing the icons of the tabs.
	// Example:
	//     <code>m_wmdTabControl.GetImageManager()->SetIcons(IDR_MAINFRAME);</code>
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image manager to contain the icons of the tabs.
	// Parameters:
	//     pImageManager - Pointer to the image manager to be set.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the behaviour for tooltips.
	// Parameters:
	//     behaviour - Tooltips behaviour to be set.
	//                 See the Remarks section for available flags.
	// Remarks:
	//     The <i>behaviour</i> parameter can be one of the following:
	//     * <b>xtpTabToolTipNever</b>: Never show tooltips for tabs.
	//     * <b>xtpTabToolTipAlways</b>: Always show tooltips for tabs.
	//     * <b>xtpTabToolTipShrinkedOnly</b>: Only show tooltips if the tab was shrinked
	//                                         (see xtpTabLayoutSizeToFit layout).
	//-----------------------------------------------------------------------
	void EnableToolTips(XTPTabToolTipBehaviour behaviour = xtpTabToolTipAlways);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method will either calculate the window rectangle of a tab control
	//     when provided with a display rectangle or vice versa.
	// Parameters:
	//     bLarger - Indicates which operation to perform.
	//               If the value of this parameter is TRUE, then lpRect specifies
	//               a display rectangle and receives the corresponding window rectangle.
	//               If the value of this parameter is FALSE, then lpRect specifies
	//               a window rectangle and receives the corresponding display rectangle.
	//     lpRect  - Pointer to a RECT structure that contains the given rectangle
	//               and receives the calculated rectangle.
	//-----------------------------------------------------------------------
	void AdjustRect(BOOL bLarger, LPRECT lpRect) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Locks/unlocks redrawing controls.
	// Parameters:
	//     bLockRedraw - TRUE to lock redrawing controls,
	//                   FALSE to unlock redrawing controls.
	// Remarks:
	//     The bLockRedraw parameter should be set to TRUE when you want
	//     to add many items, but do not want CXTPTabControl::Reposition
	//     to be called each time an item is added.
	//-----------------------------------------------------------------------
	void SetLockRedraw(BOOL bLockRedraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the focused item.
	// Parameters:
	//     pItem - Pointer to the item to set.
	// See Also:
	//     SetSelectedItem
	//-----------------------------------------------------------------------
	virtual void SetFocusedItem(CXTPTabManagerItem* pItem);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a tab navigation button is clicked.
	// Parameters:
	//     pButton - Pointer to the tab navigation button that was clicked.
	// Remarks:
	//     This method is overridden in CXTPTabClientWnd::CWorkspace and CXTPTabControl
	//     to capture when the tab navigation close "x" button
	//     is pressed and deletes the tab.
	//
	//     This member is called after CXTPTabManagerNavigateButton::PerformClick.
	// See Also:
	//     CXTPTabManagerNavigateButton::PerformClick, XTPTabNavigateButton
	//-----------------------------------------------------------------------
	void OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton);

	using CXTPTabManager::OnNavigateButtonClick;

	//-----------------------------------------------------------------------
	// Summary:
	//     The implementation should return TRUE if
	//     the header has focus or FALSE otherwise.
	// Returns:
	//     TRUE if the header has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HeaderHasFocus() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTabControl)
	virtual void PreSubclassWindow();
	BOOL DrawParentBackground(CDC* pDC, CRect rc);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTabControl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_CODEJOCK_PRIVATE

private:
	HWND m_hwndClient;

	CXTPTabPaintManager* m_pPaintManager;
	CXTPImageManager* m_pImageManager;
	CTabControlDropTarget* m_pDropTarget;
	CXTPToolTipContext* m_pToolTipContext;
	int m_nLockRedraw;

	friend class CTabControlDropTarget;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPTabPaintManager* CXTPTabControl::GetPaintManager() const
{
	return m_pPaintManager;
}

AFX_INLINE CXTPImageManager* CXTPTabControl::GetImageManager() const
{
	return m_pImageManager;
}

AFX_INLINE BOOL CXTPTabControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
									   const RECT& rect, CWnd* pParentWnd, UINT nID,
									   CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

AFX_INLINE CXTPToolTipContext* CXTPTabControl::GetToolTipContext() const
{
	return m_pToolTipContext;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCONTROL_H__)
