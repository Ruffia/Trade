// XTPPropertyGridView.h interface for the CXTPPropertyGridView class.
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
#if !defined(__XTPPROPERTYGRIDVIEW_H__)
#	define __XTPPROPERTYGRIDVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropertyGridView;
class CXTPPropertyGridPaintManager;
class CXTPPropertyGrid;
class CXTPImageManager;
//{{AFX_CODEJOCK_PRIVATE
class CXTPScrollablePropertyGridListBox;
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPPropertyGridView is a CListBox derived class.
//     It is an internal class used by the property grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridView
	: public CListBox
	, public CXTPAccessible
{
	struct WNDRECT;
	DECLARE_DYNAMIC(CXTPPropertyGridView)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridView object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridView object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridView();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the divider.
	// Returns:
	//     The position of the divider.
	//-----------------------------------------------------------------------
	int GetDividerPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the position for the divider.
	// Parameters:
	//     nDivider - Position for the divider.
	//-----------------------------------------------------------------------
	void SetDividerPos(int nDivider);

	//-----------------------------------------------------------------------
	// Summary:
	//     Disables auto-calculation for the position of the divider.
	//-----------------------------------------------------------------------
	void LockDivider();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which item, if any, is located at a specified point.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object at
	//     the specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* ItemFromPoint(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the items in the list.
	//-----------------------------------------------------------------------
	void Refresh();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items from the list.
	//-----------------------------------------------------------------------
	void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item at a specified index in the list.
	// Parameters:
	//     nIndex - Zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to the item at the specified index in the list.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new category to the property grid control.
	// Parameters:
	//     strCaption - Name of the category to be added.
	//     pCategory  - Allows for custom classses to be categories. If it is NULL, then
	//                  the property grid creates a default CXTPPropertyGridItem as a
	//                  category. If it is not NULL, then the specified item will be used.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object of the newly added category.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* AddCategory(LPCTSTR strCaption, CXTPPropertyGridItem* pCategory = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a new category to the property grid control.
	// Parameters:
	//     strCaption - Name of the category to be inserted.
	//     nIndex     - Index of the category to be inserted.
	//     pCategory  - Allows for custom classses to be categories. If it is NULL, then
	//                  the property grid creates a default CXTPPropertyGridItem as a
	//                  category. If it is not NULL, then the specified item will be used.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object of the newly inserted category.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* InsertCategory(int nIndex, LPCTSTR strCaption,
										 CXTPPropertyGridItem* pCategory = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager associated with the property grid
	//     (i.e. the paint manager used to draw the property grid control window).
	// Returns:
	//     A pointer to the paint manager associated with the property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGridPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image manager associated with the property grid
	//     (i.e. the image manager that stores images in the property grid).
	// Returns:
	//     A pointer to the image manager associated with the property grid.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a specified message to the owner window.
	// Parameters:
	//     wParam - Specifies additional message-dependent information.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     The result of the message processing; this value depends on the message sent.
	//-----------------------------------------------------------------------
	LRESULT SendNotifyMessage(WPARAM wParam = 0, LPARAM lParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent property grid.
	// Returns:
	//     A pointer to the parent property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGrid* GetPropertyGrid() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves focus to a specified in-place button.
	// Parameters:
	//     pButton - Pointer to a CXTPPropertyGridInplaceButton object.
	//-----------------------------------------------------------------------
	void FocusInplaceButton(CXTPPropertyGridInplaceButton* pButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Shows the tooltip of the item located at the mouse position.
	// Parameters:
	//     pt - Mouse position.
	//-----------------------------------------------------------------------
	virtual void ShowToolTip(CPoint pt);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPPropertyGridView)
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT)
	{
		return 0;
	}
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGridView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelectionChanged();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCntr, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility support.
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual CCmdTarget* GetAccessible();

	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()
	//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPPropertyGridItem* GetSelectedItem();
	void SetPropertySort(XTPPropertyGridSortOrder sort, BOOL bRrefresh = FALSE,
						 BOOL bSetRedraw = TRUE);
	int InsertItem(CXTPPropertyGridItem* pItem, int nIndex);
	int _DoExpand(CXTPPropertyGridItem* pItem, int nIndex);
	void _DoCollapse(CXTPPropertyGridItem* pItem);
	void _RefreshIndexes();
	void SwitchExpandState(int nItem);
	int HitTest(CPoint point) const;

	void _ShowToolTip(CRect rcBound, CRect rcText, CXTPPropertyGridItem* pItem, BOOL bValuePart);
	CSize _GetTextExtent(const CString& str, CXTPPropertyGridItem* pItem, BOOL bValuePart);
	void RelayToolTipEvent(UINT message);
	LRESULT OnScrollControl();
	CPoint ViewToGrid(CPoint point);

protected:
	CXTPPropertyGrid* m_pGrid;				  // Parent CXTPPropertyGrid class.
	BOOL m_bTracking;						  // TRUE if the grid in tracking mode.
	CRect m_rcToolTip;						  // Position of the tooltip.
	double m_dDivider;						  // Divider position.
	BOOL m_bAutoDivider;					  // TRUE to auto-calculate the position of the divider.
	HCURSOR m_hCursor;						  // Handle of the cursor.
	CString m_strTipText;					  // Tooltip text.
	XTPPropertyGridSortOrder m_properetySort; // Current sort order.
	CXTPPropertyGridItem* m_pSelected;		  // Current selected item.
	CXTPPropertyGridItems* m_pCategories;	 // Collection of the categories.
	CXTPPropertyGridToolTip m_wndTip;		  // Tooltip control.
	int m_nLockUpdate;						  // TRUE to lock update.
	UINT m_nItemHeight;						  // Default height of the item.
	BOOL m_bVariableSplitterPos; // TRUE to allow the user to resize the splitter with the mouse.

	CXTPPropertyGridInplaceButton* m_pFocusedButton; // Currently focused button.
	CXTPPropertyGridInplaceButton* m_pHotButton;	 // Currently highlighted button.

private:
	friend class CXTPPropertyGridItem;
	friend class CXTPPropertyGridItems;
	friend class CXTPPropertyGrid;
	friend class CXTPPropertyGridInplaceEdit;
	friend class CPropertyGridCtrl;
	friend class CXTPPropertyGridToolTip;
	friend class CXTPPropertyGridInplaceButton;
	friend class CXTPPropertyGridInplaceButtons;
	friend class CXTPScrollablePropertyGridListBox;
};

AFX_INLINE CXTPPropertyGrid* CXTPPropertyGridView::GetPropertyGrid() const
{
	return m_pGrid;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDVIEW_H__)
