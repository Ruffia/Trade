// XTPTabToolBar.h : interface for the CXTPTabToolBar class.
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
#if !defined(__XTPTABTOOLBAR_H__) && defined(_XTP_INCLUDE_TABMANAGER)
#	define __XTPTABTOOLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabToolBar is a CXTPToolBar derived class. It represents a tabbable toolbar.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabToolBar
	: public CXTPToolBar
	, public CXTPTabManager
{
private:
	DECLARE_XTP_COMMANDBAR(CXTPTabToolBar)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabToolBar object.
	//-----------------------------------------------------------------------
	CXTPTabToolBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabToolBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabToolBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new tab to CXTPTabToolBar.
	// Parameters:
	//     nItem - Index to insert category to.
	//     lpszItem - Caption of new tab.
	//     pItems - Control items.
	//     nCount - Control items count.
	//     bLoadIcons - TRUE to load icons.
	//     nIDResource - Toolbar resource identifier.
	// Returns:
	//     CXTPTabManagerItem class of new tab.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* InsertCategory(int nItem, LPCTSTR lpszItem, UINT* pItems, int nCount,
									   BOOL bLoadIcons = TRUE);
	CXTPTabManagerItem* InsertCategory(
		int nItem, LPCTSTR lpszItem, UINT nIDResource,
		BOOL bLoadIcons = TRUE); // <combine
								 // CXTPTabToolBar::InsertCategory@int@LPCTSTR@UINT*@int@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to refresh controls on the selected tab.
	// Parameters:
	//     pItem - Selected item.
	//-----------------------------------------------------------------------
	void UpdateTabs(CXTPTabManagerItem* pItem = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the tab paint manager.
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager (e.g. tab colors,
	//     styles, etc.).
	// Remarks:
	//     This member must be overridden in derived classes.
	// Returns:
	//     Pointer to CXTPTabPaintManager that contains the visual elements
	//     of the tabs.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the minimum available width of the toolbar.
	// Parameters:
	//     nMinWidth - Minimum width of the toolbar.
	//-----------------------------------------------------------------------
	void SetMinimumWidth(int nMinWidth);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update the position of TabManager.
	//-----------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the icon of the tab needs to be drawn.
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Specifies the location of the image.
	//     pItem  - CXTPTabManagerItem object to draw icon on.
	//     bDraw  - TRUE if the icon needs to be drawn (e.g. the icon size changed),
	//              FALSE if the icon does not need to be drawn or redrawn.
	//     szIcon - Size of the tab icon.
	// Remarks:
	//     This member is overridden by its descendants.
	//     This member must be overridden in derived classes.
	// Returns:
	//     TRUE if the icon was successfully drawn, FALSE if the icon was not drawn.
	//-----------------------------------------------------------------------
	BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when the visual properties of the tabs are
	//     changed (e.g. color, mouse-over, and on-click).
	// Remarks:
	//     This member must be overridden in derived classes.
	// Parameters:
	//     lpRect - The rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-------------------------------------------------------------------------
	void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a CXTPTabManagerItem tab. A selected tab
	//     is the currently active tab.
	// Parameters:
	//     pItem - CXTPTabManagerItem tab to select.
	// Remarks:
	//     This member will will select the specified CXTPTabManagerItem.
	//     SetCurSel will set the current CXTPTabManagerItem by its index
	//     within the collection of items.
	//     A tab is selected when it has focus or is clicked.
	// See Also:
	//     CXTPTabManagerItem, GetCurSel, SetCurSel, GetSelectedItem
	//-----------------------------------------------------------------------
	void SetSelectedItem(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before an item click.
	// Parameters:
	//     pItem - Pointer to the item that is about to be clicked.
	// Returns:
	//     TRUE if the process was canceled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnBeforeItemClick(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the borders of the toolbar.
	// Returns:
	//     A CRect object containing the borders of the toolbar.
	//-----------------------------------------------------------------------
	CRect GetBorders();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcClipBox - The rectangular area of the control that is invalid.
	//-----------------------------------------------------------------------
	void DrawCommandBar(CDC* pDC, CRect rcClipBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - Length of the bar.
	//     dwMode - Mode to dock.
	//     nWidth - Width of the bar.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	CSize CalcDockingLayout(int nLength, DWORD dwMode, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - The requested dimension of the control bar, either
	//               horizontal or vertical, depending on dwMode.
	//     dwMode - Mode to dock.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the controls to their original state.
	// Parameters:
	//     bShowWarningMessage - TRUE to show warning message.
	//-----------------------------------------------------------------------
	void Reset(BOOL bShowWarningMessage);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is dropped to the command bar.
	// Parameters:
	//     pDataObject - Pointer to a CXTPControl object.
	//     dropEffect  - DROPEFFECT enumerator.
	//     ptDrop      - Point where the user dropped the control.
	//     ptDrag      - Point where the user started dragging the control.
	//-----------------------------------------------------------------------
	void OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop,
						 CPoint ptDrag);

protected:
	void BeforeCustomizeControlAdd(CXTPControl* pControl);

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	LPDISPATCH OleInsertCategory(int nIndex, LPCTSTR lpszTitle);
	long OleGetItemCount();
	LPDISPATCH OleGetItem(int nIndex);
	void OleRemoveCategory(int nIndex);
	LPDISPATCH OleGetTabPaintManager();
	void OleUpdateTabs();

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTabToolBar);
	DECLARE_INTERFACE_MAP()
	DECLARE_ENUM_VARIANT(CXTPTabToolBar);

	DECLARE_CONNECTION_EX(CXTPTabToolBar)

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTabToolBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTabToolBar)
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

protected:
	CXTPTabPaintManager* m_pTabPaintManager; // Tab PaintManager.
	CRect m_rcTabControl;					 // Bounding rectangle of Tabs.
	int m_nMinWidth;						 // Minimum available width of toolbar.
};

AFX_INLINE CXTPTabPaintManager* CXTPTabToolBar::GetPaintManager() const
{
	return m_pTabPaintManager;
}
AFX_INLINE void CXTPTabToolBar::SetMinimumWidth(int nMinWidth)
{
	m_nMinWidth = nMinWidth;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPTABTOOLBAR_H__)
