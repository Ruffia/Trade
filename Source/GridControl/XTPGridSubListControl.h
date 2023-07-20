// XTPGridSubListControl.h: interface for the CXTPGridSubListControl class.
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
#if !defined(__XTPGRIDSUBLISTCONTROL_H__)
#	define __XTPGRIDSUBLISTCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridHeaderDragWnd;
class CXTPGridControl;
class CXTPGridColumn;
//===========================================================================
// Summary:
//     Field chooser list box control.
//     This control allows the user to move column names from an associated
//     CXTPGridControl object to and from its header.
// Remarks:
//     To use a field chooser in an existing dialog box of your application,
//     add a standard list box to your dialog template using the dialog
//     editor and then initialize your CXTPGridSubListControl object
//     inside the OnInitialUpdate dialog handler. See example below:
// Example:
// <code>
//  // CXTPGridSubListControl m_wndSubList;
//  wndSubList.SubclassDlgItem(IDC_COLUMNLIST, &pFieldChooserDialog);
//  // CXTPGridControl wndGrid;
//  wndGrid.GetGridHeader()->SetSubListCtrl(&wndSubList);
// </code>
//
// See Also:
//     CDragListBox, CXTPGridControl, CXTPGridHeader,
//     CXTPGridHeaderDragWnd, GridControl sample application
//===========================================================================
class _XTP_EXT_CLASS CXTPGridSubListControl : public CListBox
{
	DECLARE_DYNAMIC(CXTPGridSubListControl)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridSubListControl object.
	// Remarks:
	//     You construct a CXTPGridSubListControl object in two steps.
	//     First, call the constructor CXTPGridSubListControl.
	//     Second, call CListBox::Create. This initializes the Windows list box
	//     and attaches it to the CXTPGridSubListControl.
	// Example:
	// <code>
	// // Declare a local CXTPGridSubListControl object.
	// CXTPGridSubListControl mySubList;
	//
	// // Declare a dynamic CXTPGridSubListControl object.
	// CXTPGridSubListControl* pMySubList = new CXTPGridSubListControl;
	// </code>
	//
	// See Also:
	//     CListBox, CXTPGridSubListControl::SetGridCtrl,
	//     CXTPGridHeader::SetSubListCtrl
	//-----------------------------------------------------------------------
	CXTPGridSubListControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridSubListControl object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPGridSubListControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated Grid control.
	// Parameters:
	//     pGridCtrl - Pointer to the associated Grid control to be set.
	// Remarks:
	//     Usually this method should not be called by the user. Instead
	//     the user should call CXTPGridHeader::SetSubListCtrl.
	// Returns:
	//     TRUE if the associated Grid control was set successfully,
	//     otherwise FALSE.
	// See Also:
	//     CXTPGridHeader::SetSubListCtrl, GetGridCtrl
	//-----------------------------------------------------------------------
	BOOL SetGridCtrl(CXTPGridControl* pGridCtrl);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridCtrl)
	BOOL SetReportCtrl(CXTPGridControl* pGridCtrl);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated Grid control.
	// Returns:
	//     A pointer to the associated Grid control.
	// See Also:
	//     CXTPGridHeader::SetSubListCtrl, SetGridCtrl
	//-----------------------------------------------------------------------
	CXTPGridControl* GetGridCtrl();

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridCtrl)
	CXTPGridControl* GetReportCtrl();
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Updates columns list data in the field chooser.
	// Remarks:
	//     Reloads the column list from the associated CXTPGridControl object,
	//     deletes all visible column items from the internal list,
	//     and adds the rest of the invisible items.
	// Example:
	//     <code>wndSubList.UpdateList();</code>
	// Returns:
	//     TRUE if the update was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL UpdateList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Not used currently.
	// Parameters:
	//     iSubItem - Sub-item number for including.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL Include(int iSubItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Not used currently.
	// Parameters:
	//     iSubItem - Sub-item number for disabling.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL Disable(int iSubItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the list should be sorted in ascending order.
	// Parameters:
	//     bSort - TRUE to sort the list in ascending order.
	//-----------------------------------------------------------------------
	void SortAscending(BOOL bSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption of an item by its index.
	// Parameters:
	//     nIndex - Index of the item with the caption to retrieve.
	// Returns:
	//     The caption of the item.
	//-----------------------------------------------------------------------
	CString GetItemCaption(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the column object of an item by its index.
	// Parameters:
	//     nIndex - Index of the item with the column object to retrieve.
	// Returns:
	//     A pointer to the CXTPGridColumn object of the item.
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetItemColumn(int nIndex);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of an item by its data.
	// Parameters:
	//     nItemData - Data of the item with the index to retrieve.
	// Returns:
	//     The index of the item if successful, otherwise a value of -1.
	//-----------------------------------------------------------------------
	int FindItemData(int nItemData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when a drag operation starts.
	// Parameters:
	//     nItem - Item being dragged.
	// Remarks:
	//     Override this function if you want to control what happens when
	//     a drag operation begins. This implementation captures the mouse,
	//     creates a CXTPGridHeaderDragWnd and stays in drag mode until
	//     the user clicks the left or right mouse button or presses ESC,
	//     at which time the drag operation is canceled.
	// Returns:
	//     Nonzero if dragging is allowed, otherwise 0.
	// See Also:
	//     CDragListBox::BeginDrag
	//-----------------------------------------------------------------------
	BOOL BeginDrag(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when an item is being dragged within the
	//     CDragListBox object.
	// Parameters:
	//     pt - CPoint object that contains the x- and y- screen coordinates
	//          of the cursor.
	// Remarks:
	//     This implementation allows dropping items only on the control itself
	//     and associated Grid control header.
	//     Override this function if you want to provide additional functionality.
	// Returns:
	//     The resource ID of the cursor to be displayed.
	//     The following values are possible:
	//     * <b>DL_COPYCURSOR</b>: Indicates that the item will be copied.
	//     * <b>DL_MOVECURSOR</b>: Indicates that the item will be moved.
	//     * <b>DL_STOPCURSOR</b>: Indicates that the current drop target
	//                             is not acceptable.
	// See Also:
	//     CDragListBox::Dragging
	//-----------------------------------------------------------------------
	UINT Dragging(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when a drag operation has been canceled.
	// Remarks:
	//     Destroys the dragging CXTPGridHeaderDragWnd window and resets
	//     the associated Grid control mouse mode.
	// See Also:
	//     CDragListBox::CancelDrag
	//-----------------------------------------------------------------------
	void CancelDrag();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when an item is dropped within a
	//     CDragListBox object.
	// Parameters:
	//     pt - CPoint object that contains the coordinates of the drop site.
	// Remarks:
	//     Destroys the dragging CXTPGridHeaderDragWnd window and returns
	//     control to the caller.
	// See Also:
	//     CDragListBox::Dropped
	//-----------------------------------------------------------------------
	void Dropped(CPoint pt);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	void ParentNotify_SelChangd();
	//{{AFX_VIRTUAL(CXTPGridSubListControl)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();
	virtual void MeasureItem(LPMEASUREITEMSTRUCT)
	{
	}
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridSubListControl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPGridControl* m_pGridCtrl; // Pointer to the parent Grid control.

	CXTPGridHeaderDragWnd* m_pDragWnd; // Pointer to the associated dragging window.
	CRect m_rcDragWnd;				   // Coordinates of the associated dragging window.
	CRect m_rcDropTarget1;			   // First drop target - the list window itself.
	CRect m_rcDropTarget2;			   // Second drop target - the grid header.

	int m_nDropIndex;	  // Drop item index.
	BOOL m_bSortAscending; // TRUE to sort items ascending in the list.
};

AFX_INLINE BOOL CXTPGridSubListControl::SetReportCtrl(CXTPGridControl* pGridCtrl)
{
	return SetGridCtrl(pGridCtrl);
}

AFX_INLINE CXTPGridControl* CXTPGridSubListControl::GetReportCtrl()
{
	return GetGridCtrl();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDSUBLISTCONTROL_H__)
