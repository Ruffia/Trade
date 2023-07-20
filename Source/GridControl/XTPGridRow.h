// XTPGridRow.h: interface for the CXTPGridRow class.
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
#if !defined(__XTPGRIDROW_H__)
#	define __XTPGRIDROW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRecord;
class CXTPGridControl;
class CXTPGridTip;
class CXTPGridRecordItem;
class CXTPGridRows;
class CXTPGridColumn;
class CXTPGridRecordMergeItems;
class CXTPGridSection;

struct XTP_GRIDRECORDITEM_DRAWARGS;
struct XTP_GRIDRECORDITEM_METRICS;
struct XTP_GRIDRECORDITEM_CLICKARGS;

class _XTP_EXT_CLASS CXTPGridRowBase
	: public CXTPCmdTarget
	, public CXTPAccessible
{
};

//===========================================================================
// Summary:
//     CXTPGridRow class represents a row of the Grid control window.
// Remarks:
//     Each row item has a corresponding CXTPGridRecord and each CXTPGridRecord
//     could have a list of sub-items associated with it. By clicking a row
//     item, the user can expand and collapse the associated list of sub-items.
//
//     CXTPGridRow handles the behavior of a grid row. It processes user
//     mouse and keyboard events, implements external control member functions,
//     and handles drawing logic for the row.
//
//     A collection of grid row items is combined into CXTPGridRows and
//     can be accessed through the CXTPGridControl::GetRows() method.
// See Also:
//     CXTPGridControl overview, CXTPGridRows, CXTPGridRecord,
//     CXTPGridRecordItem, CXTPGridGroupRow
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRow : public CXTPHeapObjectT<CXTPGridRowBase, CXTPGridRowAllocator>
{
	DECLARE_INTERFACE_MAP()
	DECLARE_DYNAMIC(CXTPGridRow)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRow object.
	//-----------------------------------------------------------------------
	CXTPGridRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRow object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridRow();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Performs initial initialization for the current row.
	// Parameters:
	//     pRow     - Pointer to the row whose settings will be copied.
	//     pControl - Pointer to the parent Grid control.
	//     pSection - Pointer to the parent section.
	//     pRecord  - Pointer to the record associated with the row.
	//-----------------------------------------------------------------------
	virtual void InitRow(CXTPGridRow* pRow);

	virtual void InitRow(CXTPGridControl* pControl, CXTPGridSection* pSection,
						 CXTPGridRecord* pRecord); // <COMBINE
												   // CXTPGridRow::InitRow@CXTPGridRow*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record associated with the current row.
	// Returns:
	//     A pointer to the record associated with the current row.
	// Example:
	//     <code>CXTPGridRecord* pRecord = pRow->GetRecord();</code>
	// See Also:
	//     CXTPGridRecord overview
	//-----------------------------------------------------------------------
	virtual CXTPGridRecord* GetRecord() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the current row on the specified device context.
	// Parameters:
	//     pDC         - Pointer to the device context in which the drawing occurs.
	//     rcRow       - Position of the row in client window coordinates.
	//     rcClip      - Position of the row in client window coordinates.
	//     nLeftOffset - Left offset of the drawing, in pixels (horizontal scroll position).
	//     mergeItems  - Reference to the set of merged grid records.
	//     nColumnFrom - Index of the column to begin drawing.
	//     nColumnTo   - Index of the column to end drawing.
	// Remarks:
	//     This function simply prepares data for drawing and calls the necessary
	//     drawing functions from PaintManager to draw a grid item. As such,
	//     you must provide your own implementation of PaintManager in order to
	//     change the appearance of your grid item.
	// See Also:
	//     CXTPPaintManager
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, CRect rcRow, CRect rcClip, int nLeftOffset,
					  CXTPGridRecordMergeItems& mergeItems, int nColumnFrom, int nColumnTo);

	virtual void DrawItemGrid(CDC* pDC, CXTPGridColumn* pColumn, CRect rcGridItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the height of the current row depending on
	//     the specified device context.
	// Parameters:
	//     pDC    - Pointer to the device context.
	//     nWidth - Row width.
	// Remarks:
	//     Row height depends of many factors: provided device context,
	//     font to print text, visibility of rows items. GetHeight()
	//     accounts for all of these factors to get the real height of row.
	// Returns:
	//     The height of the row in logical units.
	//-----------------------------------------------------------------------
	virtual int GetHeight(CDC* pDC, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current row index in the view.
	// Remarks:
	//     GridControl draws row in some order. It depends of Index value
	//     You use GetIndex() to get current index value for particular row
	// Returns:
	//     The current row index in the view.
	//-----------------------------------------------------------------------
	virtual int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the focused state of the current row.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     the current row is focused in the Grid control or not.
	// Returns:
	//     TRUE if the current row is focused, otherwise FALSE.
	// See Also:
	//     IsSelected
	//-----------------------------------------------------------------------
	virtual BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected state of the current row.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     the current row is selected in the Grid control or not.
	// Returns:
	//     TRUE if current row is selected, otherwise FALSE.
	// See Also:
	//     IsFocused
	//-----------------------------------------------------------------------
	virtual BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the expanded state of the current row.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     the current row is expanded in the Grid control or not.
	// Returns:
	//     TRUE if the current row is expanded, otherwise FALSE.
	// See Also:
	//     SetExpanded
	//-----------------------------------------------------------------------
	virtual BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expanded state for the current row.
	// Parameters:
	//     bExpanded  - TRUE to expand the current row.
	//     bRecursive - TRUE to recursively expand all levels of child rows.
	//                  By default, this parameter is FALSE.
	// Remarks:
	//     Call this member function if you want to expand or collapse the current row.
	// See Also:
	//     IsExpanded
	//-----------------------------------------------------------------------
	virtual void SetExpanded(BOOL bExpanded, BOOL bRecursive = FALSE);
	virtual void _SetExpanded(BOOL bExpanded, BOOL bRecursive,
							  BOOL bNotify); // <combine CXTPGridRow::SetExpanded@BOOL@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expanded state for the current row and its children.
	// Parameters:
	//     bExpanded - TRUE to expand the current row and its children.
	// Remarks:
	//     Call this member function if you want to expand or collapse the current row
	//     and its children.
	//-----------------------------------------------------------------------
	void SetFullExpanded(BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the current row.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     the current row is visible in the Grid control or not.
	// Returns:
	//     TRUE if the current row is visible, otherwise FALSE.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state for the current row.
	// Parameters:
	//     bVisible - TRUE to set the current row to visible,
	//                FALSE to set the current row to hidden.
	// Remarks:
	//     Call this member function if you want to show or hide the current row.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user's ability to collapse/expand rows is locked.
	// Returns:
	//     TRUE if the user's ability to collapse/expand rows is locked.
	//-----------------------------------------------------------------------
	virtual BOOL IsLockExpand() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user's ability to collapse/expand rows should be locked.
	// Parameter:
	//     bLockExpand - TRUE to lock the user's ability to collapse/expand rows.
	//-----------------------------------------------------------------------
	virtual void LockExpand(BOOL bLockExpand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected state for the current row.
	// Remarks:
	//     If multiple selection is enabled, then this row will be added to the selection.
	// Parameters:
	//     bSelected - TRUE to select the row, FALSE to unselect the row.
	//-----------------------------------------------------------------------
	virtual void SetSelected(BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects all child items of the current row.
	//-----------------------------------------------------------------------
	virtual void SelectChilds();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current row is a group row.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     the current row is a group row or not.
	// Returns:
	//     TRUE if the current row is a group row, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsGroupRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which grid item, if any, is at a specified point.
	// Parameters:
	//     ptPoint   - Point to test.
	//     pRectItem - Pointer to a CRect object to receive the item rect.
	//     ppColumn  - Double pointer to a CXTPGridColumn to receive
	//                 the column at the specified point.
	// Remarks:
	//     Call this member function to hit test row contents at a specified point.
	// Returns:
	//     The record item at the specified position, if any,
	//     or NULL otherwise.
	//-----------------------------------------------------------------------
	virtual CXTPGridRecordItem* HitTest(CPoint ptPoint, CRect* pRectItem = NULL,
										CXTPGridColumn** ppColumn = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks. Called by the framework when
	//     the user has clicked the mouse button in the row area.
	// Parameters:
	//     ptClicked - Position of the cursor, in screen coordinates,
	//                 at the time of the mouse click.
	// Remarks:
	//     Call this member function if you want a row to process
	//     a mouse button single click message.
	//     You should provide the method with the coordinates of the
	//     mouse message to proceed.
	// See Also:
	//     OnDblClick, OnMouseMove, OnContextMenu
	//-----------------------------------------------------------------------
	virtual void OnClick(CPoint ptClicked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Returns:
	//     TRUE if the implementation handles the mouse click event and no
	//     further handling is required, otherwise FALSE.
	// See Also:
	//     OnLButtonUp, OnClick
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Returns:
	//     TRUE if the implementation handles the mouse click event and no
	//     further handling is required, otherwise FALSE.
	// See Also:
	//     OnLButtonDown, OnClick
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes double mouse clicks. Called by the framework when
	//     the user has double-clicked the mouse button in the row area.
	// Parameters:
	//     ptClicked - Position of the cursor, in screen coordinates,
	//                 at the time of the mouse click.
	// Remarks:
	//     Call this member function if you want a row to process
	//     a mouse button double click message.
	//     You should provide the method with the coordinates of the
	//     mouse message to proceed.
	// See Also:
	//     OnClick, OnMouseMove, OnContextMenu
	//-----------------------------------------------------------------------
	virtual void OnDblClick(CPoint ptClicked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down.
	//     point  - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want a row to process
	//     a mouse move message. You should provide the method with the
	//     coordinates of the mouse message to proceed and a flag
	//     showing which of virtual keys were down.
	// See Also:
	//     OnClick, OnDblClick, OnContextMenu
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes context menu. Called by the framework when the user
	//     has clicked the right mouse button (right clicked) in the window.
	// Parameters:
	//     ptClick - Position of the cursor, in screen coordinates,
	//               at the time of the mouse click.
	// Remarks:
	//     Call this member function if you want a row to process
	//     a mouse right button down (context menu call) message.
	//     You should provide the method with the coordinates of the
	//     mouse message to proceed.
	// See Also:
	//     OnClick, OnDblClick, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnContextMenu(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows a specified tooltip window at a specified position.
	// Parameters:
	//     ptTip   - Screen position to show the tooltip window at.
	//     pTipWnd - Pointer to the tooltip window to show.
	// Remarks:
	//     Call this member function if you want to show a
	//     grid tooltip window for this row.
	// See Also:
	//     CXTPGridTip overview
	//-----------------------------------------------------------------------
	virtual void ShowToolTip(CPoint ptTip, CXTPGridTip* pTipWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the parent Grid control.
	// Parameters:
	//     pControl - Pointer to the parent Grid control to be set.
	// Remarks:
	//     Call this member function to set a pointer to the
	//     CXTPGridControl which this item belongs to.
	// See Also:
	//     CXTPGridControl overview
	//-----------------------------------------------------------------------
	virtual void SetControl(CXTPGridControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent Grid control.
	// Remarks:
	//     Call this member function to get a pointer to the
	//     CXTPGridControl which this item belongs to.
	// Returns:
	//     A pointer to the parent Grid control object.
	// See Also:
	//     CXTPGridControl overview
	//-----------------------------------------------------------------------
	virtual CXTPGridControl* GetControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent row of the current row.
	// Remarks:
	//     When grid rows are arranged as a tree view, this member
	//     function returns a pointer to the parent CXTPGridRow item.
	// Returns:
	//     A pointer to the parent row of the current row, if any,
	//     or NULL if the current row does not have a parent row.
	// See Also:
	//     HasParent, GetTreeDepth, GetChilds, AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* GetParentRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent rows of the current row.
	// Remarks:
	//     When grid rows are arranged as a tree view, this member
	//     function returns a pointer to the parent CXTPGridRows item.
	// Returns:
	//     A pointer to the parent rows of the current row, if any,
	//     or NULL if the current row does not have parent rows.
	// See Also:
	//     HasParent, GetTreeDepth, GetChilds, AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual CXTPGridRows* GetParentRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the depth of the current row in tree view.
	// Remarks:
	//     When grid rows are arranged as a tree view, this member function
	//     returns the depth level of the row
	//     (i.e. the number of parent elements for the row.)
	// Returns:
	//     The depth of the current row in tree view.
	// See Also:
	//     GetParentRow, HasParent, GetChilds, AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual int GetTreeDepth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified row has a parent row in tree view.
	// Parameters:
	//     pRow - Pointer to the row to check.
	// Remarks:
	//     When grid rows are arranged as a tree view, this member function
	//     calculates whether the specified row has a parent row or not.
	// Returns:
	//     TRUE if the specified row has a parent, otherwise FALSE.
	// See Also:
	//     HasParent, GetParentRow, GetTreeDepth, GetChilds, AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual BOOL HasParent(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the child collection of the current row.
	// Parameters:
	//     bCreate - TRUE to create an empty child collection for the current row
	//               if the current row does not already have a child collection.
	// Remarks:
	//     Call this member function to retrieve access to the current row's
	//     collection of row children.
	// Returns:
	//     A pointer to the child collection of the current row.
	// See Also:
	//     CXTPGridRows overview, GetParentRow, HasParent, GetTreeDepth,
	//     AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual CXTPGridRows* GetChilds(BOOL bCreate = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a child row to the current row.
	// Parameters:
	//     pRow - Pointer to the child row to add.
	// Remarks:
	//     Call this member function if you want to add a CXTPGridRow object
	//     as a child row for the current row in tree view mode.
	// Returns:
	//     A pointer to the newly added child row object.
	// See Also:
	//     GetParentRow, HasParent, GetTreeDepth, GetChilds
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* AddChild(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current row has child rows in tree view.
	// Remarks:
	//     When grid rows are arranged as a tree view, this member function
	//     calculates whether the current row has child rows or not.
	// Returns:
	//     TRUE if the current row has child rows, otherwise FALSE.
	// See Also:
	//     HasParent, GetParentRow, GetTreeDepth, GetChilds, AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual BOOL HasChildren() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the next sibling row of the current row.
	// Remarks:
	//     When grid rows are arranged as a tree view, this member function
	//     returns a pointer to the next CXTPGridRow item.
	// Returns:
	//     A pointer to the next sibling row of the current row, if any,
	//     or NULL if the current row does not have a next sibling row.
	// See Also:
	//     HasParent, GetTreeDepth, GetChilds, AddChild, HasChildren
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* GetNextSiblingRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the coordinates of the collapse/expand button for the current row.
	// Parameters:
	//     rcCollapse - Rectangle in grid window client coordinates.
	// Remarks:
	//     Call this member function to set the coordinates of the
	//     collapse/expand button for the current row. Note that it could be
	//     different depending on the row tree depth.
	// See Also:
	//     GetTreeDepth, GetRect
	//-----------------------------------------------------------------------
	virtual void SetCollapseRect(CRect rcCollapse);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the preview item is visible for the current row.
	// Returns:
	//     TRUE if the preview item is visiblefor the current row, otherwise FALSE.
	// See Also:
	//     CXTPGridRecordItemPreview overview, CXTPGridRecord::GetItemPreview
	//-----------------------------------------------------------------------
	virtual BOOL IsPreviewVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if items are visible.
	// Returns:
	//     TRUE if items are visible.
	// See Also:
	//     IsPreviewVisible
	//-----------------------------------------------------------------------
	virtual BOOL IsItemsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the coordinates of the row rectangle.
	// Returns:
	//     The coordinates of the row rectangle in grid window client coordinates.
	// Remarks:
	//     Call this member function to determine the coordinates of the
	//     region where the current row is drawn on the parent window.
	// See Also:
	//     SetCollapseRect
	//-----------------------------------------------------------------------
	virtual CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the drawing metrics of the current row.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure for calculating metrics.
	//     pMetrics  - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                 structure to fill.
	// See Also:
	//     XTP_GRIDRECORDITEM_DRAWARGS, XTP_GRIDRECORDITEM_METRICS
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								XTP_GRIDRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row depth level when the current row is a group row.
	// Returns:
	//     The row depth level when the current row is a group row.
	//-----------------------------------------------------------------------
	virtual int GetGroupLevel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of a specified item.
	// Parameters:
	//     pItem - Pointer to a CXTPGridRecordItem object.
	// Returns:
	//     The rectangle of the specified item.
	//-----------------------------------------------------------------------
	virtual CRect GetItemRect(CXTPGridRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determine if the current row is the last tree row or is not a part of a tree row.
	// Returns:
	//     TRUE if the current row is the last tree row, FALSE if the current row
	//     is either not the last tree row or not a part of a tree row.
	//-----------------------------------------------------------------------
	virtual BOOL IsLastTreeRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that the current row is visible.
	//     This will cause the Grid control to scroll to this row.
	// Remarks:
	//     This method is also called by CXTPGridControl::SetFocusedRow
	//     to make the newly focused row visible.
	//-----------------------------------------------------------------------
	virtual void EnsureVisible();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the rectangle of an item.
	// Parameters:
	//     rcItem  - Reference to the bounding rectangle of the item cell.
	//     pColumn - Pointer to a CXTPGridColumn object; this column
	//               will be checked to determine if it is a tree column.
	// See Also:
	//     CXTPGridColumn::IsTreeColumn
	//-----------------------------------------------------------------------
	virtual void ShiftTreeIndent(CRect& rcItem, CXTPGridColumn* pColumn) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills an XTP_GRIDRECORDITEM_METRICS structure with drawing metrics.
	// Parameters:
	//     pColumn  - Pointer to the column of the item.
	//     pItem    - Pointer to the item to be drawn.
	//     pMetrics - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                structure to fill.
	// See Also:
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	virtual void FillMetrics(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem,
							 XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether a
	//     point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor. These
	//             coordinates are always relative to the upper-left corner of the window
	//     pTI   - A pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, the window control ID. If
	//     the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determine the type of the current row (i.e. body, header, or footer).
	// Returns:
	//     The type of the current row; one of the values defined by
	//     the XTPGridRowType enumeration.
	//-----------------------------------------------------------------------
	XTPGridRowType GetType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the section that the current row belongs to.
	// Returns:
	//     A pointer to the section that the current row belongs to.
	//-----------------------------------------------------------------------
	CXTPGridSection* GetSection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current row has merged items.
	// Returns:
	//     TRUE if the current row has merged items, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasMergedItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height of the current row calculated during merge cell processing.
	// Remarks:
	//     A merge height of -1 means undefined height.
	//     When calculating the height of any merge cell, the height of all related
	//     merge cells must also be calculated. These calculated values are saved
	//     to prevent repeating the same actions for all related rows.
	//-----------------------------------------------------------------------
	void SetMergeHeight(int nMergeHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the current row calculated during merge cell processing.
	// Remarks:
	//     A merge height of -1 means undefined height.
	//     When calculating the height of any merge cell, the height of all related
	//     merge cells must also be calculated. These calculated values are saved
	//     to prevent repeating the same actions for all related rows.
	//-----------------------------------------------------------------------
	int GetMergeHeight() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of a specified item.
	// Parameters:
	//     pItem     - Pointer to a CXTPGridRecordItem object.
	//     bAsMerged - TRUE if the specified item is merged.
	// Returns:
	//     The rectangle of the item.
	//-----------------------------------------------------------------------
	CRect GetItemRect(CXTPGridRecordItem* pItem, BOOL bAsMerged);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index of the current row in the corresponding rows collection.
	// Parameters:
	//     nNewIndex - Index to be set.
	//-----------------------------------------------------------------------
	virtual void SetIndex(int nNewIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the last child row in a specified child row collection.
	// Parameters:
	//     pChildren - Pointer to a child row collection.
	// Returns:
	//     The index of the last child row in the specified child row collection.
	//-----------------------------------------------------------------------
	int GetLastChildRow(CXTPGridRows* pChildren) const;

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
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPGridControl* m_pControl; // Pointer to the parent Grid control.
	CXTPGridRecord* m_pRecord;   // Pointer to the associated CXTPGridRecord object.
	CXTPGridRow* m_pParentRow;   // Pointer to the parent row in the tree.
	CXTPGridRows* m_pParentRows; // Pointer to the parent rows in the tree.
	CXTPGridRows* m_pChilds;	 // Pointer to collection of children.
	CXTPGridSection* m_pSection; // Pointer to section the row belongs to.

	CRect m_rcRow;		// A rectangle where a row is to be drawn.
	CRect m_rcCollapse; // A rectangle with coordinates of collapse/expand bitmap.

	int m_nIndex;	  // Item index.
	int m_nGroupLevel; // Contains row depth level when row is a group row.
	int m_nRowLevel;   // Row depth level in a tree.

	BOOL m_bVisible;		   // TRUE if this row should be visible, FALSE if row is hidden.
	BOOL m_bExpanded;		   // TRUE if row is expanded in a tree view, FALSE if row is collapsed.
	BOOL m_bLockExpand;		   // TRUE if row cannot be expanded/collapsed by the user.
	BOOL m_bHasSelectedChilds; // TRUE if the row has selected children (on any deep level).

	int m_nPreviewHeight; // Height of the preview item of this row.
	int m_nFreeHeight;	// Height of row in FreeHeight RC mode.
	int m_nMergeHeight;   // Height of row which calculated on merge cells processing. (-1) means
						  // undefined height.
	int m_nChildIndex;	// Index of row inside parent rows.

private:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRow);

	afx_msg LPDISPATCH OleGetRecord();
	afx_msg LPDISPATCH OleGetChilds();
	afx_msg LPDISPATCH OleGetParentRow();
	afx_msg LPDISPATCH OleGetSection();
	afx_msg LPDISPATCH OleGetNextSiblingRow();
	afx_msg void OleGetRect(long* Left, long* Top, long* Right, long* Bottom);
	afx_msg void OleGetItemRect(LPDISPATCH lpDispatch, long* Left, long* Top, long* Right,
								long* Bottom);
	afx_msg void OleGetItemRectAsMerged(LPDISPATCH lpDispatch, long* Left, long* Top, long* Right,
										long* Bottom, VARIANT_BOOL fAsMerged);

public:
	static CXTPGridRow* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/

	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPGridControl;
	friend class CXTPGridSelectedRows;
	friend class CXTPGridRows;
	friend class CXTPGridRecordItem;
	friend class CXTPGridSection;
	friend class CXTPGridPaintManager;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE void CXTPGridRow::SetIndex(int nNewIndex)
{
	m_nIndex = nNewIndex;
}

AFX_INLINE CXTPGridRecord* CXTPGridRow::GetRecord() const
{
	return m_pRecord;
}

AFX_INLINE CXTPGridRow* CXTPGridRow::GetParentRow() const
{
	return m_pParentRow;
}

AFX_INLINE void CXTPGridRow::SetControl(CXTPGridControl* pControl)
{
	m_pControl = pControl;
}

AFX_INLINE CXTPGridControl* CXTPGridRow::GetControl() const
{
	return m_pControl;
}

AFX_INLINE void CXTPGridRow::SetCollapseRect(CRect rcCollapse)
{
	m_rcCollapse = rcCollapse;
}

AFX_INLINE CRect CXTPGridRow::GetRect() const
{
	return m_rcRow;
}

AFX_INLINE int CXTPGridRow::GetGroupLevel() const
{
	return m_nGroupLevel;
}

AFX_INLINE CXTPGridRows* CXTPGridRow::GetParentRows() const
{
	return m_pParentRows;
}

AFX_INLINE CXTPGridSection* CXTPGridRow::GetSection() const
{
	return m_pSection;
}

AFX_INLINE void CXTPGridRow::SetMergeHeight(int nMergeHeight)
{
	m_nMergeHeight = nMergeHeight;
}

AFX_INLINE int CXTPGridRow::GetMergeHeight() const
{
	return m_nMergeHeight;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDROW_H__)
