// XTPGridHeader.h: interface for the CXTPGridHeader class.
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
#if !defined(__XTPGRIDHEADER_H__)
#	define __XTPGRIDHEADER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropExchange;

class CXTPGridColumn;
class CXTPGridColumns;
class CXTPGridControl;
class CXTPGridSubListControl;
class CXTPGridFilterEditControl;
class CXTPGridHeaderDropWnd;
class CXTPGridHeaderDragWnd;
class CXTPGridPaintManager;

//===========================================================================
// Summary:
//     NM_GRIDSCROLL notification data.
//===========================================================================
struct XTP_NM_GRIDSCROLL : public NMHDR
{
	int nSection;  // Index of the section.
	int nPosition; // New scroll position.
};

//===========================================================================
// Summary:
//     NM_GRIDGIVEFEEDBACK notification data.
//===========================================================================
struct XTP_NM_GRIDGIVEFEEDBACK : public NMHDR
{
	DROPEFFECT dropEffect;
	BOOL bDragStarted;
};

//===========================================================================
// Summary:
//     This structure is sent to the main window in a WM_NOTIFY message after
//     a column is resized.
// See Also:
//     CXTPGridHeader::ResizeColumn, CXTPGridHeader::AdjustColumnsWidth,
//     XTP_NM_GRID_COLUMNWIDTHCHANGED.
//===========================================================================
struct XTP_NM_GRIDCOLUMNRESIZE
{
	NMHDR hdr; // Standard structure, containing information about a notification message.
	CXTPGridColumn* pColumn; // Resized column.
	int nPrevWidth;			 // Previous width.
	int nNewWidth;			 // New width.
};

//===========================================================================
// Summary:
//     This structure is sent to the main window in a WM_NOTIFY message after
//     a row is resized.
// See Also:
//     XTP_NM_GRID_ROWHEIGHTCHANGED.
//===========================================================================
struct XTP_NM_GRIDROWRESIZE
{
	NMHDR hdr;		   // Standard structure, containing information about a notification message.
	CXTPGridRow* pRow; // Resized row.
	int nPrevHeight;   // Previous height.
	int nNewHeight;	// New height.
};

//===========================================================================
// Summary:
//     This structure is sent to the main window in a WM_NOTIFY message after
//     columns order or group order is changed.
// See Also:
//     CXTPGridHeader::OnColumnsChanged, XTP_NM_GRID_COLUMNORDERCHANGED,
//     XTP_NM_GRID_GROUPORDERCHANGED.
//===========================================================================
struct XTP_NM_GRIDCOLUMNORDERCHANGED
{
	NMHDR hdr; // Standard structure, containing information about a notification message.
	CXTPGridColumn* pColumn; // Affected column.
	int nReason;			 // Change reason.
};

//===========================================================================
// Summary:
//     Enumeration of OnColumnsChanged event reasons.
// See Also:
//     CXTPGridHeader::OnColumnsChanged
//
// <KEYWORDS >
//===========================================================================
enum XTPGridColumnOrderChangedReason
{
	xtpGridColumnChangeOther	   = 0x0000, // Neither column order nor group order is changed.
	xtpGridColumnOrderChanged	  = 0x0001, // Column order changed.
	xtpGridColumnGroupOrderChanged = 0x0002, // Group order changed.
	xtpGridColumnOrderChangedMask  = 0x000F, // A mask for order change flags.

	xtpGridColumnAdded		  = 0x0010,   // A column is added to the column list.
	xtpGridColumnRemoved	  = 0x0020,   // A column is removed from the column list.
	xtpGridColumnPropExchange = 0x0040,   // The column properties changed during properties
										  // exchange procedure.
	xtpGridColumnCaptionChanged = 0x0080, // The column caption is changed.
	xtpGridColumnWidthChanged   = 0x0100, // The column width is changed.
	xtpGridColumnShown			= 0x0200, // A column visibility property is changed to "visible".
	xtpGridColumnHidden			= 0x0400, // A column visibility property is changed to "hidden".
	xtpGridColumnMoved			= 0x0800, // A column is moved.
	xtpGridColumnAddedToGroupby = 0x1000, // A column is added to the Group By box.
	xtpGridColumnRemovedFromGroupby		 = 0x2000, // A column is removed from the Group By box.
	xtpGridColumnAddedToFieldChooser	 = 0x4000, // A column is added to the field chooser.
	xtpGridColumnRemovedFromFieldChooser = 0x8000, // A column is removed from the field chooser.
	xtpGridColumnChangeMask				 = 0xFFF0, // A mask for column change flags.
};

enum XTPGridColumnDirection
{
	xtpGridColumnDirectionLeft  = -1,
	xtpGridColumnDirectionRight = +1
};

//===========================================================================
// Summary:
//     A grid header is a part of the grid window positioned above
//     columns of text or numbers. It contains a title for each column,
//     and it can be divided into parts. The user can drag the dividers
//     that separate the parts to set the width of each column.
// Remarks:
//     This class is responsible for header drawing on a grid window,
//     processing user events, and changing header properties.
//
//     Functionality of the header includes the following:
//     * Header self-drawing.
//     * Header item custom ordering.
//     * Header item drag and drop, for the reordering of header items.
//     * Sorting grid rows with mouse clicks on a column header.
//     * Filtering grid row items using the associated CXTPGridFilterEditControl.
//     * Choosing header items using the associated field chooser control: CXTPGridSubListControl.
//     * Group By area support containing the order of grouped columns.
//
// See Also:
//     CXTPGridControl, CXTPGridColumn, CXTPGridSubListControl
//===========================================================================
class _XTP_EXT_CLASS CXTPGridHeader
	: public CXTPCmdTarget
	, public CXTPAccessible
{
	DECLARE_INTERFACE_MAP()
	friend class CXTPGridSubListControl;
	friend class CXTPGridControl;
	friend class CXTPGridColumn;
	friend class CXTPGridHeaderDragWnd;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridHeader object.
	// Parameters:
	//     pColumns - Pointer to the associated columns array.
	//     pControl - Pointer to the parent Grid control.
	// Example:
	// <code>
	// // Declare a local CXTPGridHeader object.
	// CXTPGridHeader  myHeader;
	//
	// // Declare a dynamic CXTPGridHeader object.
	// CXTPGridHeader* pmyHeader = new CXTPGridHeader;
	// </code>
	//-----------------------------------------------------------------------
	CXTPGridHeader(CXTPGridControl* pControl, CXTPGridColumns* pColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPGridHeader object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridHeader();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws columns header on a provided device context.
	// Parameters:
	//     pDC         - Pointer to the destination device context.
	//     rcHeader    - Reference to the rectangle in which the header is drawn.
	//     nLeftOffset - Left offset in pixels (Horizontal scroll position).
	// Remarks:
	//     Call this function to draw the header on the provided
	//     device context. Note that it does not contain the Group By area.
	// See Also:
	//     DrawGroupByControl
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, CRect rcHeader, int nLeftOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws Group By area on a provided device context.
	// Parameters:
	//     pDC       - Pointer to the destination device context.
	//     rcGroupBy - Reference to the rectangle of the Group By area.
	// Remarks:
	//     Call this function to draw the Group By area on the provided
	//     device context.
	// See Also:
	//     Draw
	//-----------------------------------------------------------------------
	virtual void DrawGroupByControl(CDC* pDC, CRect& rcGroupBy);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws columns footer on a provided device context.
	// Parameters:
	//     pDC         - Pointer to the destination device context.
	//     rcFooter    - Reference to the rectangle in which the footer is drawn.
	//     nLeftOffset - Left offset in pixels (Horizontal scroll position).
	// Remarks:
	//     Call this function to draw the footer on the provided
	//     device context.
	// See Also:
	//     Draw
	//-----------------------------------------------------------------------
	virtual void DrawFooter(CDC* pDC, CRect& rcFooter, int nLeftOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     ptClick - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want the grid header to process
	//     a mouse left button down message. You should provide the method
	//     with the coordinates of the mouse message to proceed.
	// See Also:
	//     OnLButtonUp, OnContextMenu, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button double clicks.
	// Parameters:
	//     ptDblClick - Point coordinates where the message was fired from.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDblClk(CPoint ptDblClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     nFlags  - Indicates whether various virtual keys are down.
	//     ptClick - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want the grid header to process
	//     a mouse left button up message. You should provide the method
	//     with the coordinates of the mouse message to proceed and a flag
	//     showing which virtual keys were down.
	// See Also:
	//     OnLButtonDown, OnContextMenu, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(UINT nFlags, CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes context menu. Called by the framework when the user
	//     has clicked the right mouse button (right clicked) in the window.
	// Parameters:
	//     ptClick - Position of the cursor, in screen coordinates,
	//               at the time of the mouse click.
	// Remarks:
	//     Call this member function if you want the grid header to process
	//     a mouse right button down (context menu call) message.
	//     You should provide the method with the coordinates of the
	//     mouse message to proceed.
	// See Also:
	//     OnLButtonDown, OnLButtonUp, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnContextMenu(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager object used to draw the Grid control window.
	// Returns:
	//     A pointer to the paint manager object.
	// See Also:
	//     CXTPGridPaintManager overview
	//-----------------------------------------------------------------------
	CXTPGridPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse movements.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down.
	//     point  - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want the grid header to process
	//     a mouse move message. You should provide the method with the
	//     coordinates of the mouse message to proceed and a flag
	//     showing which virtual keys were down.
	// See Also:
	//     OnLButtonDown, OnLButtonUp, OnContextMenu
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes configuration to/from the provided archive stream.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the header is being dragged.
	// Remarks:
	//     When the user begins dragging a row, the grid header enters
	//     a dragging loop and processes each message in a specific way.
	//     This function is used to determine if the grid header is in
	//     the dragging loop or not.
	// Returns:
	//     TRUE if the header is being dragged, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDragHeader() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if column removing is allowed.
	// Parameters:
	//     bAllow - TRUE to allow column removing, FALSE to disallow.
	// Remarks:
	//     Usually the user is allowed to remove a column using mouse drag and drop.
	//     Using this member function, you can change the user's permissions
	//     to force either allowing or disallowing column removing.
	// See Also:
	//     IsAllowColumnRemove, CXTPGridColumn::AllowColumnRemove
	//-----------------------------------------------------------------------
	void AllowColumnRemove(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if column removing is allowed.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to remove the column using drag and drop or not.
	// Returns:
	//     TRUE if column removing is allowed, otherwise FALSE.
	// See Also:
	//     AllowColumnRemove
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnRemove() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if column reordering is allowed.
	// Parameters:
	//     bAllow - TRUE to allow column reordering, FALSE to disallow.
	// Remarks:
	//     Usually the user is allowed to reorder a column using mouse drag and drop.
	//     Using this member function, you can change the user's permissions
	//     to force either allowing or disallowing column reordering.
	// See Also:
	//     IsAllowColumnReorder
	//-----------------------------------------------------------------------
	void AllowColumnReorder(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if column reordering is allowed.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to reorder the column using drag and drop or not.
	// Returns:
	//     TRUE if column reordering is allowed, otherwise FALSE.
	// See Also:
	//     AllowColumnReorder
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnReorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if column resizing is allowed.
	// Parameters:
	//     bAllow - TRUE to allow column resizing, FALSE to disallow.
	// Remarks:
	//     Usually the user is allowed to resize a column using mouse drag and drop.
	//     Using this member function, you can change the user's permissions
	//     to force either allowing or disallowing column resizing.
	// See Also:
	//     IsAllowColumnResize
	//-----------------------------------------------------------------------
	void AllowColumnResize(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if column resizing is allowed.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to resize a column using drag and drop or not.
	// Returns:
	//     TRUE if column resizing is allowed, FALSE to disallow.
	// See Also:
	//     AllowColumnResize
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnResize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables show items in groups mode.
	// Parameters:
	//     bShowInGroups - TRUE to enable show items in groups mode,
	//                     FALSE to disable show items in groups mode.
	// See Also:
	//     IsShowItemsInGroups
	//-----------------------------------------------------------------------
	void ShowItemsInGroups(BOOL bShowInGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if show items in groups mode is enabled/disabled.
	// Returns:
	//     TRUE if show items in groups mode is enabled,
	//     FALSE if show items in groups mode is disabled.
	// See Also:
	//     ShowItemsInGroups
	//-----------------------------------------------------------------------
	BOOL IsShowItemsInGroups() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if column sorting mode is allowed.
	// Parameters:
	//     bAllow - TRUE to allow column sorting mode, FALSE to disallow.
	// Remarks:
	//     Call this member function to specify whether the user is allowed
	//     to sort columns using the mouse.
	// See Also:
	//     IsAllowColumnSort
	//-----------------------------------------------------------------------
	void AllowColumnSort(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if column sorting mode is allowed.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to sort columns using the mouse.
	// Returns:
	//     TRUE when column sorting is allowed, FALSE otherwise.
	// See Also:
	//     AllowColumnSort
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnSort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the next visible column in the array of columns.
	// Parameters:
	//     nIndex     - Position of a column in the array of columns to start from.
	//     nDirection - Direction to move (left or right) in the array of columns
	//                  starting from nIndex; must be one of the values defined by
	//                  the xtpGridColumnDirectionRight enumeration
	// Returns:
	//     A pointer to the next visible column in the array of columns.
	//-----------------------------------------------------------------------
	virtual CXTPGridColumn* GetNextVisibleColumn(int nIndex, int nDirection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which column, if any, is at a specified point.
	// Parameters:
	//     ptPoint - Point to be tested in Grid control client coordinates.
	// Returns:
	//     A pointer to the CXTPGridColumn object at the specified point, if any,
	//     or NULL otherwise.
	// See Also:
	//     CXTPGridControl::HitTest, CXTPGridRow::HitTest
	//-----------------------------------------------------------------------
	virtual CXTPGridColumn* HitTest(CPoint ptPoint) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the header index of where to insert a column nearest
	//     to a specified point.
	// Parameters:
	//     ptPoint - Point coordinates.
	// Returns:
	//     The header index of where to insert a column nearest
	//     to the specified point.
	//-----------------------------------------------------------------------
	virtual int FindHeaderColumn(CPoint ptPoint) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates index of where to insert a column nearest to the provided point.
	// Parameters:
	//     ptPoint      - Point coordinates to test.
	//     bExactSearch - TRUE when search by exact position,
	//                    FALSE when searching average value.
	// Returns:
	//     GroupBy column position.
	//-----------------------------------------------------------------------
	virtual int FindGroupByColumn(CPoint ptPoint, BOOL bExactSearch = FALSE) const;

	// private:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the header column index at a specified point, if any.
	// Parameters:
	//     ptPoint - Point to test in Grid control window client coordinates.
	// Returns:
	//     The header column index at the specified point, if any,
	//     otherwise a value of -1.
	//-----------------------------------------------------------------------
	virtual int HitTestHeaderColumnIndex(CPoint ptPoint) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified point is over the column resize area.
	// Parameters:
	//     ptPoint - Point coordinates to test.
	// Returns:
	//     A pointer to the column for resize at the specified point, if any,
	//     or NULL if the specified point is not over any column resize area.
	//-----------------------------------------------------------------------
	CXTPGridColumn* MouseOverColumnResizeArea(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current scroll information for full columns scroll mode and
	//     the helper information to scroll left or right.
	// Parameters:
	//     rpPrev      - [out] Reference to a CXTPGridColumn pointer
	//                   to receive the previous column (i.e. the next
	//                   column when scrolling left). This parameter
	//                   will be set to NULL if the current scrollbar
	//                   position is on the leftmost column.
	//     rpCurr      - [out] Reference to a CXTPGridColumn pointer
	//                   to receive the current column (i.e. the first
	//                   visible column on the left).
	//     rpNext      - [out] Reference to a CXTPGridColumn pointer
	//                   to receive the next column (i.e. the next
	//                   column when scrolling right). This parameter
	//                   will be set to NULL if the current scrollbar
	//                   position is on the rightmost column.
	//     rnScrollPos - [out] Reference to an integer value to receive
	//                   the current scrollbar position.
	//     rnScrollMax - [out] Reference to an integer value to receive
	//                   the number of scrollbar positions.
	// Returns:
	//      The width of frozen columns, in pixels.
	//-----------------------------------------------------------------------
	int GetFullColScrollInfo(CXTPGridColumn*& rpPrev, CXTPGridColumn*& rpCurr,
							 CXTPGridColumn*& rpNext, int& rnScrollPos, int& rnScrollMax) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the width of columns.
	// Parameters:
	//     nTotalWidth - Total width of the header area, in pixels.
	//     nFirstIndex - Index of the column to begin adjusting from.
	// Remarks:
	//     Call this member function when you want to adjust the width of
	//     columns in reaction to any specific event.
	//     For example, this member function is called by the framework
	//     when changing the Grid control size (WM_SIZE) and when
	//     changing the size of any column by mouse dragging.
	// See Also:
	//     ResizeColumn, GetMaxAvailWidth
	//-----------------------------------------------------------------------
	void AdjustColumnsWidth(int nTotalWidth, int nFirstIndex = 0);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Starts column header dragging mode.
	// Parameters:
	//     pColumn - Pointer to the column to be set as the dragging column.
	//     bHeader - TRUE when starting dragging for column header items,
	//               FALSE when starting dragging for Group By column items.
	// Remarks:
	//     Usually this method is called internally as a reaction to specific
	//     user actions (e.g. when the user begins dragging the header by mouse).
	// See Also:
	//     GetDraggingColumn, CancelMouseMode
	//-----------------------------------------------------------------------
	void StartDragging(CXTPGridColumn* pColumn, BOOL bHeader = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resizes a specified column to a specified width and adjusts the width
	//     of other columns in correspondence with the specified column.
	// Parameters:
	//     pColumn - Pointer to the column to resize.
	//     nWidth  - New column width, in pixels.
	// See Also:
	//     AdjustColumnsWidth, GetMaxAvailWidth
	//-----------------------------------------------------------------------
	void ResizeColumn(CXTPGridColumn* pColumn, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the mouse mode to xtpMouseNothing and performs all
	//     necessary cleanup operations.
	// Remarks:
	//     This member function is usually called when finishing
	//     dragging or resizing columns.
	// See Also:
	//     StartDragging
	//-----------------------------------------------------------------------
	void CancelMouseMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum available width of a specified column, in pixels.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Remarks:
	//     Calculates the maximum available width for a column depending
	//     on the width and modes of other columns.
	// Returns:
	//     The maximum available width of the specified column, in pixels.
	// See Also:
	//     AdjustColumnsWidth, ResizeColumn
	//-----------------------------------------------------------------------
	int GetMaxAvailWidth(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a hot divider between columns.
	// Parameters:
	//     nIndex  - Index of the column to set the hot divider before.
	//     bHeader - TRUE if you want to set the hot divider in the columns header,
	//               FALSE if you want to set the hot divider between Group By area items.
	// Remarks:
	//     When dragging a column, call this member function to show a
	//     hot divider (red arrows by default) between columns to be
	//     used as as a target for dropping the currently dragged column.
	// Returns:
	//     The index of the column that the hot divider was set before.
	//-----------------------------------------------------------------------
	int SetHotDivider(int nIndex, BOOL bHeader = TRUE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the dragging column if the control is in dragging mode.
	// Returns:
	//     A pointer to the dragging column if the control is in
	//     dragging mode, or NULL otherwise.
	// Example:
	// <code>CXTPGridColumn* pColumn = GetDraggingColumn();</code>
	// See Also:
	//      StartDragging, GetResizingColumn
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetDraggingColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the resizing column if the control is in resizing mode.
	// Returns:
	//     A pointer to the resizing column if the control is in
	//     resizing mode, or NULL otherwise.
	// Example:
	// <code>CXTPGridColumn* pColumn = GetResizingColumn();</code>
	// See Also:
	//     StartDragging, GetDraggingColumn
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetResizingColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated field chooser list control.
	// Parameters:
	//     pSubList - Pointer to the field chooser list control.
	// Remarks:
	//     You should call this member function to assign the user's custom
	//     list box control as a Grid control field chooser list control.
	//     After assigning, the framework will automatically process all
	//     windows messages for the control.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// Example:
	// <code>
	// // CXTPGridControl wndGrid;
	// // CXTPGridSubListControl wndSubList;
	// wndGrid.GetColumns()->GetGridHeader()->SetSubListCtrl(&wndSubList);
	// </code>
	// See Also:
	//     CXTPGridSubListControl overview, SetFilterEditCtrl
	//-----------------------------------------------------------------------
	BOOL SetSubListCtrl(CXTPGridSubListControl* pSubList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated field chooser list control.
	// Returns:
	//     A pointer to the associated field chooser list control.
	// See Also:
	//     SetSubListCtrl
	//-----------------------------------------------------------------------
	CXTPGridSubListControl* GetSubListCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated filter edit control.
	// Parameters:
	//     pFilterEdit - Pointer to the filter edit control.
	// Remarks:
	//     You should call this member function to assign the user's custom
	//     edit box control as a Grid control filter edit control.
	//     After assigning, the framework will automatically process all
	//     windows messages for the control.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// Example:
	// <code>
	// CXTPGridControl wndGrid;
	// CXTPGridFilterEditControl wndFilterEdit;
	// wndGrid.GetGridHeader()->SetFilterEditCtrl(&wndFilterEdit);
	// </code>
	// See Also:
	//     CXTPGridFilterEditControl overview, GetFilterEditCtrl, SetSubListCtrl
	//-----------------------------------------------------------------------
	BOOL SetFilterEditCtrl(CXTPGridFilterEditControl* pFilterEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated filter edit control.
	// Returns:
	//     A pointer to the associated filter edit control.
	// See Also:
	//     CXTPGridFilterEditControl overview, SetFilterEditCtrl
	//-----------------------------------------------------------------------
	CXTPGridFilterEditControl* GetFilterEditCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the Group By area, in pixels.
	// Remarks:
	//     Calculates the height of the Group By area based on
	//     the number of items contained inside it.
	// Returns:
	//     The height of the Group By area, in pixels.
	//-----------------------------------------------------------------------
	int GetGroupByHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the width of a specified column to the width of
	//     the widest column item.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	//-----------------------------------------------------------------------
	void BestFit(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified column is the last visible column
	//     in the grid header.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     TRUE if the specified column is the last visible column
	//     in the grid header, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLastVisibleColumn(CXTPGridColumn* pColumn) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified column in the last resizable column
	//     in the grid header.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     TRUE if the specified column is the last resizable column
	//     in the grid header, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLastResizebleColumn(CXTPGridColumn* pColumn) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indent, in pixels, for a single level of tree depth
	//     for the Grid control in tree mode.
	// Returns:
	//     The indent, in pixels, for a single level of tree depth
	//     for the Grid control in tree mode.
	//-----------------------------------------------------------------------
	int GetIndentLevel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates header settings after any changes occur.
	// Parameters:
	//     nReason - Event reason.
	//     pColumn - Affected column.
	// Remarks:
	//     Call this member function if you want to automatically
	//     adjust column widths and redraw the Grid control
	//     depending on current columns settings.
	// See Also:
	//     XTPGridColumnOrderChangedReason
	//-----------------------------------------------------------------------
	virtual void OnColumnsChanged(int nReason, CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent Grid control.
	// Returns:
	//     A pointer to the parent Grid control.
	//-----------------------------------------------------------------------
	CXTPGridControl* GetControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if AutoColumnSizing should be used.
	// Parameters:
	//     bAutoColumnSizing - TRUE to use AutoColumnSizing, FALSE otherwise.
	// Remarks:
	//     If AutoColumnSizing is used (bAutoColumnSizing = TRUE), then all
	//     columns are sized to fill the entire width of the Grid control.
	//     When the Grid control is resized, all columns will be resized
	//     to fit the new size of the Grid control.
	//     If AutoColumnSizing is not used (bAutoColumnSizing = FALSE), then
	//     columns will use either the width that they were assigned when they
	//     were created or the value stored in their column width property.
	//     The width of columns may occupy more or less than the total width of
	//     the Grid control. In the case that the total width of columns
	//     exceeds the width of the Grid control, then those columns will not
	//     be visible. If the Grid control is resized, then the size of
	//     columns will not be affected.
	//-----------------------------------------------------------------------
	void SetAutoColumnSizing(BOOL bAutoColumnSizing);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if AutoColumnSizing is used.
	// Returns:
	//     TRUE if AutoColumnSizing is used, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsAutoColumnSizing() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine m_bForceLayoutAdjustment flag.
	// Returns:
	//     TRUE if the operation is successful, FALSE else.
	//-----------------------------------------------------------------------
	BOOL ForceLayoutAdjustment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total width of the header
	//     (i.e. the total combined width of all columns).
	// Returns:
	//     The total width of the header
	//     (i.e. the total combined width of all columns).
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expanded state of the last column.
	// Parameters:
	//     bLastColumnExpand     - TRUE to expand the last column.
	//     bLastColumnExpandKeep - Not used currently.
	//-----------------------------------------------------------------------
	void SetLastColumnExpand(BOOL bLastColumnExpand, BOOL bLastColumnExpandKeep);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum column width for all columns.
	// Returns:
	//     The maximum column width for all columns.
	//-----------------------------------------------------------------------
	int GetMaxColumnWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the maximum column width for all columns.
	// Parameters:
	//     nMaxColumnWidth - Maximum column width to be set.
	// Remarks:
	//     Call pColumn->SetMaxWidth to set the maximumn column width
	//     for an individual column.
	//-----------------------------------------------------------------------
	void SetMaxColumnWidth(int nMaxColumnWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the handler of the user's horizontal resizing cursor.
	// Returns:
	//     The handler of the user's horizontal resizing cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetResizeCursor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the handler of the user's vertical resizing cursor.
	// Returns:
	//     The handler of the user's vertical resizing cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetResizeVCursor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the handler of the user's Don't Drop cursor.
	// Returns:
	//     The handler of the user's Don't Drop cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetDontDropCursor() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether
	//     a point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor. These
	//             coordinates are always relative to the upper-left
	//             corner of the window.
	//     pTI   - Pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, then the window control ID is returned.
	//     If the tooltip control was not found, then a value of -1 is returned.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	// private:
	//{{AFX_CODEJOCK_PRIVATE
	void TrackColumn(CXTPGridColumn* pColumn, CPoint pt);
	void DestroyDropWnd();
	void SetHotTrackingColumn(CXTPGridColumn* pColumn);
	CXTPGridColumn* GetHotTrackingColumn() const;
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
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPGridColumns* m_pColumns; // Pointer to the associated columns list.
	CXTPGridControl* m_pControl; // Pointer to the parent Grid control.

	int m_nDropIndex; // Item index of the column to drop to.
	int m_nVisIndex;  // Item visual index of the column to drag from.
	// Release 13.1
	CXTPGridHeaderDropWnd* m_pDropWnd; // Column drop window.
	CXTPGridHeaderDragWnd* m_pDragWnd; // Column drag window.

	CXTPGridColumn* m_pDragColumn;		  // Dragging column.
	CXTPGridColumn* m_pResizeColumn;	  // Resizing column.
	CXTPGridColumn* m_pHotTrackingColumn; // Hot column
	BOOL m_bDragHeader;   // Determines whether control is in header dragging mode or group by
						  // dragging mode.
	BOOL m_bDragGroupBox; // Determines whether control is in group box dragging mode.

	CXTPGridSubListControl* m_pSubList;		  // Field chooser list control.
	CXTPGridFilterEditControl* m_pFilterEdit; // Filter edit control

	CRect m_rcGroupBy; // Group By area coordinates.
	CRect m_rcHeader;  // Header coordinates.
	CPoint m_ptMouse;  // Stores mouse coordinates for processing when required (resizing, etc.).

	HCURSOR m_hResizeCursor;   // Handler of the user's horizontal resizing cursor.
	HCURSOR m_hResizeVCursor;  // Handler of the user's vertical resizing cursor.
	HCURSOR m_hDontDropCursor; // Handler of the user's Don't Drop cursor.

	BOOL m_bAllowColumnRemove;  // FALSE if columns removing is not allowed.
	BOOL m_bAllowColumnResize;  // FALSE if columns resizing is not allowed.
	BOOL m_bAllowColumnReorder; // FALSE if columns reordering is not allowed.
	BOOL m_bAllowColumnSort;	// FALSE if columns sorting is not allowed.

	BOOL m_bAutoColumnSizing;  // TRUE if AutoColumnSizing should be used.
	int m_nResizeCaptureRange; // Distance around column divider where column re-sizer is shown.
	int m_nIndentLevel;		   // Width indent at the left of the column.
	BOOL m_bShowItemsInGroups; // TRUE when showing items in groups, FALSE when grouping is not
							   // used.
	int m_nHeaderWidth;		   // The combined width of all the visible column headers.
	int m_nForceLayoutAdjustment;
	// Internal flag to Force Layout Adjustment (and also show scrollbar) if positive

	BOOL m_bLastColumnExpand;	 // Tells whether the last column is expanded or not.
	BOOL m_bLastColumnExpandKeep; // Not used currently.

	int m_nMaxColumnWidth; // Custom maximum column width.

	// private:
	enum GridDraggingMode
	{
		dragNothing		 = 0, // User is dragging nothing.
		dragOutTarget	= 1, // User is dragging an item outside the drop target.
		dragInHeader	 = 2, // User is dragging an item inside header
		dragInGroupBox   = 4, // User is dragging an item inside group box
		dragInTarget	 = 6, // User is dragging an item inside the drop target.
		dragFieldChooser = 8  // User is dragging an item inside field chooser.
	} m_dragMode;			  // Column dragging mode.

public:
	static int s_nMinAutoScrollStep; // Minimal horizontal auto-scrolling step, in pixels.

	static BOOL s_bShowItemsInGroupsPXDefault; // Store default value of m_bShowItemsInGroups member
											   // for serialization (PropExchange).
	static BOOL s_bSendContextMenuForWholeHeaderArea; // If TRUE, then the Grid control sends
													  // ContextMenu event for clicks on whole
													  // header area (not only for columns)
													  // rectangles.

	BOOL m_bHideColumnAfterGroupBoxDrop; // Tell whether to hide the column after group box drop.
};

AFX_INLINE CXTPGridFilterEditControl* CXTPGridHeader::GetFilterEditCtrl() const
{
	return m_pFilterEdit;
}
AFX_INLINE BOOL CXTPGridHeader::IsDragHeader() const
{
	return m_bDragHeader;
}
AFX_INLINE int CXTPGridHeader::GetIndentLevel() const
{
	return m_nIndentLevel;
}
AFX_INLINE void CXTPGridHeader::AllowColumnRemove(BOOL bAllow)
{
	m_bAllowColumnRemove = bAllow;
}
AFX_INLINE void CXTPGridHeader::AllowColumnResize(BOOL bAllow)
{
	m_bAllowColumnResize = bAllow;
}
AFX_INLINE void CXTPGridHeader::AllowColumnReorder(BOOL bAllow)
{
	m_bAllowColumnReorder = bAllow;
}
AFX_INLINE BOOL CXTPGridHeader::IsAllowColumnRemove() const
{
	return m_bAllowColumnRemove;
}
AFX_INLINE BOOL CXTPGridHeader::IsAllowColumnReorder() const
{
	return m_bAllowColumnReorder;
}
AFX_INLINE BOOL CXTPGridHeader::IsAllowColumnResize() const
{
	return m_bAllowColumnResize;
}
AFX_INLINE void CXTPGridHeader::AllowColumnSort(BOOL bAllow)
{
	m_bAllowColumnSort = bAllow;
}
AFX_INLINE BOOL CXTPGridHeader::IsAllowColumnSort() const
{
	return m_bAllowColumnSort;
}
AFX_INLINE BOOL CXTPGridHeader::IsShowItemsInGroups() const
{
	return m_bShowItemsInGroups;
}
AFX_INLINE CXTPGridControl* CXTPGridHeader::GetControl() const
{
	return m_pControl;
}
AFX_INLINE CXTPGridSubListControl* CXTPGridHeader::GetSubListCtrl() const
{
	return m_pSubList;
}
AFX_INLINE int CXTPGridHeader::GetWidth() const
{
	return m_nHeaderWidth;
}
AFX_INLINE BOOL CXTPGridHeader::IsAutoColumnSizing() const
{
	return m_bAutoColumnSizing;
}
AFX_INLINE int CXTPGridHeader::ForceLayoutAdjustment() const
{
	return m_nForceLayoutAdjustment;
}
AFX_INLINE void CXTPGridHeader::SetMaxColumnWidth(int nMaxColumnWidth)
{
	m_nMaxColumnWidth = nMaxColumnWidth;
}
AFX_INLINE int CXTPGridHeader::GetMaxColumnWidth() const
{
	return m_nMaxColumnWidth;
}

AFX_INLINE HCURSOR CXTPGridHeader::GetResizeCursor() const
{
	return m_hResizeCursor;
}

AFX_INLINE HCURSOR CXTPGridHeader::GetResizeVCursor() const
{
	return m_hResizeVCursor;
}

AFX_INLINE HCURSOR CXTPGridHeader::GetDontDropCursor() const
{
	return m_hDontDropCursor;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDHEADER_H__)
