// XTPGridColumn.h: interface for the CXTPGridColumn class.
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
#if !defined(__XTPGRIDCOLUMN_H__)
#	define __XTPGRIDCOLUMN_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridGroups;
class CXTPGridRecordItemEditOptions;
class CXTPPropExchange;
class CXTPMarkupContext;
class CXTPMarkupUIElement;

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the best fit mode.
//-----------------------------------------------------------------------
enum XTPGridColumnBestFitMode
{
	xtpColumnBestFitModeVisibleData = 0, // Fit by visible column data.
	xtpColumnBestFitModeAllData		= 1, // Fit by all column data.
};

class _XTP_EXT_CLASS CXTPGridColumnDisplaySettings : public CXTPCmdTarget
{
public:
	CXTPGridColumnDisplaySettings();

	BOOL IsShowIcon() const;
	void SetShowIcon(BOOL bShowIcon);

	BOOL IsShowText() const;
	void SetShowText(BOOL bShowText);

protected:
	BOOL m_bShowIcon; // If TRUE, then the icon will be shown.
	BOOL m_bShowText; // If TRUE, then the text will be shown.
};

class _XTP_EXT_CLASS CXTPGridColumnDisplayOptions : public CXTPCmdTarget
{
public:
	CXTPGridColumnDisplayOptions();
	virtual ~CXTPGridColumnDisplayOptions();

public:
	CXTPGridColumnDisplaySettings* Column() const;
	CXTPGridColumnDisplaySettings* FieldChooser() const;
	CXTPGridColumnDisplaySettings* GroupBy() const;

protected:
	CXTPGridColumnDisplaySettings* m_pColumn;
	CXTPGridColumnDisplaySettings* m_pFieldChooser;
	CXTPGridColumnDisplaySettings* m_pGroupBy;
};

//===========================================================================
// Summary:
//     Represents a grid column item with its properties and operations.
// Remarks:
//     You create a CXTPGridColumn object by calling a constructor and
//     providing all necessary parameters. Many of these parameters have default
//     values. Column has 3 properties that greatly influence its behavior:
//
//     * bAutoSize: TRUE allows auto-resizing of the width of columns when
//                  GridControl's client area is resized by the user.
//                  If bAutoSize is set to FALSE, then the column keeps its
//                  initial size.
//
//     * bSortable: TRUE allows a column to be sortable. If bSortable is
//                  set to FALSE, then you will not be able to sort records
//                  by their column.
//
//     * bVisible:  Determines if columns will draw by control or not.
//                  For example, if the column is dragged into FieldChooser,
//                  then the column still exists in the columns list,
//                  but it becomes invisible.
// See Also:
//     CXTPGridControl, CXTPGridColumns
//===========================================================================
class _XTP_EXT_CLASS CXTPGridColumn : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPGridColumn);

	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPGridControl;
	friend class CXTPGridColumns;
	friend class CXTPGridHeader;
	friend class CXTPGridRows;
	friend class CXTPGridRow;
	friend class CXTPGridColumnOrder;
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridColumn object.
	// Parameters:
	//     strName          - Column name.
	//     nItemIndex       - Index of the corresponding record item.
	//     nWidth           - Initial width of the created column.
	//     nMinWidth        - Minimal width of the column.
	//     nIconID          - Number of the corresponding icon in the image list.
	//     bAutoSize        - Is column resizable flag.
	//     bSortable        - Is column sortable flag.
	//     bVisible         - Is column visible flag.
	//     strInternalName  - Internal column name (to serialize column options).
	//     strDisplayName   - The display name (caption) of the column.
	// Remarks:
	//     There are 3 parameters that greatly influence a column's behavior:
	//     * <b>bAutoSize</b>: TRUE allows auto-resizing of the width of columns when
	//                         GridControl's client area is resized by the user.
	//                         If bAutoSize is set to FALSE, then the column keeps its
	//                         initial size.
	//     * <b>bSortable</b>: TRUE allows a column to be sortable. If bSortable is
	//                         set to FALSE, then you will not be able to sort records
	//                         by their column.
	//     * <b>bVisible</b>:  Determines if columns will draw by control or not.
	//                         For example, if the column is dragged into FieldChooser,
	//                         then the column still exists in the columns list,
	//                         but it becomes invisible.
	// Example:
	// <code>
	// #define COLUMN_ICON 1
	// #define COLUMN_MAIL_ICON 1
	//
	// CXTPGridControl wndGrid;
	// wndGrid.AddColumn(new CXTPGridColumn(COLUMN_ICON, _T("Message Class"), 18, FALSE,
	// COLUMN_MAIL_ICON));
	// </code>
	//-----------------------------------------------------------------------
	CXTPGridColumn(int nItemIndex, LPCTSTR strName, int nWidth, BOOL bAutoSize = TRUE,
				   int nIconID = XTP_GRID_NOICON, BOOL bSortable = TRUE, BOOL bVisible = TRUE);
	CXTPGridColumn(int nItemIndex, LPCTSTR strDisplayName, LPCTSTR strInternalName, int nWidth,
				   BOOL bAutoSize = TRUE, int nIconID = XTP_GRID_NOICON, BOOL bSortable = TRUE,
				   BOOL bVisible = TRUE);
	// <COMBINE CXTPGridColumn::CXTPGridColumn@int@LPCTSTR@int@BOOL@int@BOOL@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridColumn object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column header is in dragging mode.
	// Remarks:
	//     Grid columns may be moved (change order of columns, delete columns
	//     from header, move column to FieldChooser) using drag and drop.
	//     When a column is moved, it is in dragging mode. You use IsDragging()
	//     to determine if the columns are in dragging mode.
	// Returns:
	//     TRUE if the column header is in dragging mode, otherwise FALSE.
	// See Also:
	//     IsAllowDragging
	//-----------------------------------------------------------------------
	BOOL IsDragging() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the client rectangle where the column was drawn last time.
	// Remarks:
	//     Each time when the column is drawing, its rectangle is recalculated
	//     to fit in size after GridControl resizing, column resizing or
	//     moving, etc. You use GetWidth() to get the latest rectangle width.
	// Returns:
	//     The client rectangle where the column was drawn last time.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the column width.
	// Remarks:
	//     Each time when the column is drawing, its rectangle is recalculated
	//     to fit in size after GridControl resizing, column resizing or
	//     moving, etc. You use GetWidth() to get the latest rectangle width.
	// Returns:
	//     The column width.
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the caption width (i.e. the caption text length).
	// Parameters:
	//     pDC - Pointer to the device context to calculate the width at.
	// Remarks:
	//     This function calculates the caption width in logical units using
	//     the given device context, current caption font, and string of text.
	// Returns:
	//     The caption width, in pixels.
	//-----------------------------------------------------------------------
	int GetCaptionWidth(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the column width.
	// Parameters:
	//     nNewWidth - New column width, in pixels.
	// Remarks:
	//     This member function provides you with the ability to change
	//     column width manually. Used to set the
	//     initial width and/or width for columns in which AutoSize = FALSE.
	// Returns:
	//     The previous column width, in pixels.
	//-----------------------------------------------------------------------
	int SetWidth(int nNewWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the column indentation.
	// Returns:
	//     The column indentation, in pixels.
	//-----------------------------------------------------------------------
	int GetIndent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the associated record item.
	// Returns:
	//     The index of the associated record item.
	//-----------------------------------------------------------------------
	int GetItemIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index of the associated record item.
	// Parameters:
	//     nItemIndex - New index to be set.
	//-----------------------------------------------------------------------
	void SetItemIndex(int nItemIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the column.
	// Returns:
	//     The index of the column.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the visible index of the column.
	// Returns:
	//     The visible index of the column.
	//-----------------------------------------------------------------------
	int GetVisibleIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the old visible index of the column.
	// Returns:
	//     The old visible index of the column.
	//-----------------------------------------------------------------------
	int GetOldVisibleIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ID of the associated column header icon.
	// Remarks:
	//     Columns can have an image that will be drawn on it. You use GetIconID()
	//     to get the current image ID which passes to the column ID of the column.
	//     The ID must match one of the IDs in the imagelist.
	// Returns:
	//     The ID of the associated column header icon.
	//-----------------------------------------------------------------------
	int GetIconID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the ID for the associated column header icon.
	// Parameters:
	//     nIconId - ID to be set.
	//-----------------------------------------------------------------------
	void SetIconID(int nIconId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption of the column header.
	// Remarks:
	//     You use GetCaption() to get the current column caption.
	// Returns:
	//     The text caption of the column header.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text caption for the column header.
	// Parameters:
	//     strCaption - Text caption to be set.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text caption for the column footer.
	// Parameters:
	//     strFooter - Text caption to be set.
	// Remarks
	//     Call CXTPGridControl::ShowFooter to show the footer area.
	// See Also:
	//     GetFooterText, CXTPGridControl::ShowFooter
	//-----------------------------------------------------------------------
	void SetFooterText(LPCTSTR strFooter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption of the column footer.
	// Remarks:
	//     Use SetFooterText() to set the text caption for the column footer.
	// Returns:
	//     The text caption of the column footer.
	//-----------------------------------------------------------------------
	CString GetFooterText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font for the column footer.
	// Parameters:
	//     pFont - Pointer to a CFont object; may be NULL.
	// Remarks:
	//     CXTPGridPaintManager::m_fontCaption is used as the default
	//     footer font. If pFont is NULL, then the default font will be used.
	// See Also:
	//     GetFooterFont
	//-----------------------------------------------------------------------
	void SetFooterFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the font of the column footer.
	// Remarks:
	//     CXTPGridPaintManager::m_fontCaption is used as the default
	//     footer font.
	// Returns:
	//     The font of the column footer.
	// See Also:
	//     SetFooterFont
	//-----------------------------------------------------------------------
	CFont* GetFooterFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/change is the right column footer divider
	//     visible.
	// Parameters:
	//     bSet - If TRUE divider is visible, FALSE - invisible.
	// Remarks:
	//     By default divider is visible.
	// See Also:
	//     GetDrawFooterDivider
	//-----------------------------------------------------------------------
	void SetDrawFooterDivider(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns is right column footer divider visible.
	// Remarks:
	//     By default divider is visible.
	// Returns:
	//     Is column footer divider visible.
	// See Also:
	//     SetDrawFooterDivider
	//-----------------------------------------------------------------------
	BOOL GetDrawFooterDivider() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/change is the right column header divider
	//     visible.
	// Parameters:
	//     bSet - If TRUE divider is visible, FALSE - invisible.
	// Remarks:
	//     By default divider is visible.
	// See Also:
	//     GetDrawHeaderDivider
	//-----------------------------------------------------------------------
	void SetDrawHeaderDivider(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns is right column header divider visible.
	// Remarks:
	//     By default divider is visible.
	// Returns:
	//     Is column footer divider visible.
	// See Also:
	//     SetDrawHeaderDivider
	//-----------------------------------------------------------------------
	BOOL GetDrawHeaderDivider() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the column header.
	// Returns:
	//     The tooltip text of the column header.
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltop text for the column header.
	// Parameters:
	//     lpszTooltip - Tooltip text to be set.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the column header.
	// Returns:
	//     TRUE if the column header is visible, otherwise FALSE.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state for the column header.
	// Parameters:
	//     bVisible - TRUE to set the column header to visible,
	//                FALSE to set the column header to invisible.
	// Remarks:
	//     A column header will become invisible if it is moved to FieldChooser.
	//     To change the visibility of a column header, use SetVisible(BOOL).
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the highlighted state of the group.
	// Returns:
	//     TRUE if the group is highlighted, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHotTracking() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the filterable state for the column header.
	// Parameters:
	//     bFilterable - TRUE to set the column header to filterable,
	//                   FALSE to set the column header to not filterable.
	// Returns:
	//     The previous filterable state of the column header.
	// See Also:
	//     IsFiltrable
	//-----------------------------------------------------------------------
	BOOL SetFilterable(BOOL bFilterable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the filterable state of the column header.
	// Returns:
	//     TRUE if the column header is filterable, otherwise FALSE.
	// See Also:
	//     SetFilterable
	//-----------------------------------------------------------------------
	BOOL IsFilterable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the resizable state of the column.
	// Returns:
	//     TRUE if the column is resizable, otherwise FALSE.
	// See Also:
	//     EnableResize
	//-----------------------------------------------------------------------
	BOOL IsResizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the resizable state of the column.
	// Parameters:
	//     bIsResizable - TRUE to set the column to resizable, FALSE otherwise.
	// See Also:
	//     IsResizable
	//-----------------------------------------------------------------------
	void EnableResize(BOOL bIsResizable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimal width for the column.
	// Parameters:
	//     nMinWidth - Minimal width, in pixels, to be set.
	// Remarks:
	//     After you set the minimal width for the column, the column
	//     cannot be resized to less than the minimal width.
	// Returns:
	//     The previous minimal width of the column, in pixels.
	// See Also:
	//     GetMinWidth
	//-----------------------------------------------------------------------
	int SetMinWidth(int nMinWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimal width of the column.
	// Returns:
	//     The minimal width of the column, in pixels.
	// See Also:
	//     SetMinWidth
	//-----------------------------------------------------------------------
	int GetMinWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the maximal width for the column.
	// Parameters:
	//     nMaxWidth - Maximal width, in pixels, to be set.
	// Remarks:
	//     After you set the maximal width for the column, the column
	//     cannot be resized to more than the maximal width.
	// Returns:
	//     The previous maximal width of the column, in pixels.
	// See Also:
	//     GetMaxWidth
	//-----------------------------------------------------------------------
	int SetMaxWidth(int nMaxWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximal width of the column.
	// Returns:
	//     The maximal width of the column, in pixels.
	// See Also:
	//     SetMaxWidth
	//-----------------------------------------------------------------------
	int GetMaxWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the sortable state of the column.
	// Returns:
	//     TRUE if the column is sortable, otherwise FALSE.
	// See Also:
	//     IsSorted, IsSortedIncreasing, IsSortedDecreasing
	//-----------------------------------------------------------------------
	BOOL IsSortable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the groupable state of the column.
	// Remarks:
	//     Not all columns allow grouping. This method can be used to determine
	//     if the column can be grouped (i.e. added to the "Group By" box).
	// Returns:
	//     TRUE if the column is groupable, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsGroupable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column's expand collapse right icon (+/-) is enabled/disabled.
	// Returns:
	//     TRUE if the column's expand collapse right icon (+/-) is enabled,
	//     FALSE the column's expand collapse right icon (+/-) is disabled.
	// See also:
	//     SetPlusMinus(...)
	//-----------------------------------------------------------------------
	BOOL IsPlusMinus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the expanded state of the column.
	// Returns:
	//     TRUE if the column is expanded, otherwise FALSE.
	// See also:
	//     SetExpanded(...)
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the next visual column.
	// Returns:
	//     The zero-based index of the next visual column.
	// See also:
	//     SetNextVisualBlock(...)
	//-----------------------------------------------------------------------
	int GetNextVisualBlock() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sortable when grouped.
	// Returns:
	//     TRUE if the column is sortable when grouped, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsAutoSortWhenGrouped() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column can be dragged.
	// Returns:
	//     TRUE if the column can be dragged, otherwise FALSE.
	// See Also:
	//     IsDragging
	//-----------------------------------------------------------------------
	BOOL IsAllowDragging() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sorted in increasing order.
	// Returns:
	//     TRUE if the column is sorted in increasing order, otherwise FALSE.
	// See Also:
	//     IsSorted, IsSortable, IsSortedDecreasing
	//-----------------------------------------------------------------------
	BOOL IsSortedIncreasing() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sorted in decreasing order.
	// Returns:
	//     TRUE if the column is sorted in decreasing order, otherwise FALSE.
	// See Also:
	//     IsSorted, IsSortable, IsSortedIncreasing
	//-----------------------------------------------------------------------
	BOOL IsSortedDecreasing() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the column should be sorted in increasing/decreasing order.
	// Parameters:
	//     bSortIncreasing - TRUE to sort the column in increasing order,
	//                       FALSE to sort the column in decreasing order.
	// See Also:
	//     IsSorted, IsSortable, IsSortedIncreasing, IsSortedDecreasing
	//-----------------------------------------------------------------------
	void SetSortIncreasing(BOOL bSortIncreasing);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sorted.
	// Returns:
	//     TRUE if the column is sorted, otherwise FALSE.
	// See Also:
	//     IsSortable, IsSortedIncreasing, IsSortedDecreasing
	//-----------------------------------------------------------------------
	BOOL IsSorted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the column should contain a tree.
	// Parameters:
	//     bIsTreeColumn - TRUE to set the column to contain tree items,
	//                     FALSE otherwise.
	// See Also:
	//     IsTreeColumn
	//-----------------------------------------------------------------------
	void SetTreeColumn(BOOL bIsTreeColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column contains a tree.
	// Returns:
	//     TRUE if the column contains tree items, otherwise FALSE.
	// See Also:
	//     SetTreeColumn
	//-----------------------------------------------------------------------
	BOOL IsTreeColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes column data to/from the provided archive stream.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment value for the column.
	// Parameters:
	//     nAlignment - Alignment value to be set; the alignment value should be
	//                  a combination of DT_* bits used by the DrawText and
	//                  DrawTextEx functions from the Windows SDK.
	// Returns:
	//     The previous alignment value of the column.
	//-----------------------------------------------------------------------
	int SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment value of the column.
	// Returns:
	//     The alignment value of the column; the alignment value should be
	//     a combination of DT_* bits used by the DrawText and
	//     DrawTextEx functions from the Windows SDK.
	//-----------------------------------------------------------------------
	int GetAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment value for the column header.
	// Parameters:
	//     nAlignment - Alignment value to be set; the alignment value should be
	//                  a combination of DT_* bits used by the DrawText and
	//                  DrawTextEx functions from the Windows SDK.
	//-----------------------------------------------------------------------
	void SetHeaderAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment value of the column header.
	// Returns:
	//     The alignment value of the column header; the alignment value should be
	//     a combination of DT_* bits used by the DrawText and
	//     DrawTextEx functions from the Windows SDK.
	//-----------------------------------------------------------------------
	int GetHeaderAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment value for the column footer.
	// Parameters:
	//     nAlignment - Alignment value to be set; the alignment value should be
	//                  a combination of DT_* bits used by the DrawText and
	//                  DrawTextEx functions from the Windows SDK.
	//-----------------------------------------------------------------------
	void SetFooterAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment value of the column footer.
	// Returns:
	//     The alignment value of the column footer; the alignment value should be
	//     a combination of DT_* bits used by the DrawText and
	//     DrawTextEx functions from the Windows SDK.
	//-----------------------------------------------------------------------
	int GetFooterAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks if the column header should have the sort triangle drawn.
	// Returns:
	//     TRUE if the sort triangle should be drawn for this column,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL HasSortTriangle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets if the column is automatically sized.
	// Returns:
	//     TRUE if the column is automatically sized, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsAutoSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the column is automatically sized.
	// Parameters:
	//     bSet - TRUE to automatically size the column, FALSE otherwise.
	// Remarks:
	//     Missing MFC function (presented in ActiveX version).
	//-----------------------------------------------------------------------
	void SetAutoSize(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the column should be sorted when the user clicks
	//     on the column's header.
	// Parameters:
	//     bSortable - TRUE to allow records to be sorted by the column
	//                 by clicking on the column's header,
	//                 FALSE to disallow records from being sorted by the column
	//                 by clicking on the column's header.
	// Remarks:
	//     If bSortable is TRUE, then the sort order will toggle between
	//     ascending and descending order each time the column header is clicked.
	//     If bSortable is FALSE, then the column may still be sorted, but
	//     not by clicking the column header. In order to sort the column when
	//     bSortable is FALSE, the column can be added to Grid control's
	//     sort order. This will keep the column sorted at all times.
	// See Also:
	//     SetGroupable
	//-----------------------------------------------------------------------
	void SetSortable(BOOL bSortable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the column should be sortable when grouped.
	// Parameters:
	//     bAutoSortWhenGrouped - TRUE to allow the column to
	//                            be sorted when grouped,
	//                            FALSE to disallow the column from
	//                            being sorted when grouped.
	// Remarks:
	//     TRUE by default. If FALSE, then records will be grouped without
	//     sorting by the column (sort by other columns will still be applied).
	//-----------------------------------------------------------------------
	void SetAutoSortWhenGrouped(BOOL bAutoSortWhenGrouped);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the groupable state of the column.
	// Parameters:
	//     bGroupable - TRUE to set the column to groupable, FALSE otherwise.
	// Remarks:
	//     If a column is groupable, then it can be added to the "Group By" box.
	//     In the event that a column is added to the "Group By" box, the
	//     contents of the Grid control will be grouped by the column.
	//     If a column is not groupable, then it cannot be added to the "Group By" box.
	// See Also:
	//     SetSortable
	//-----------------------------------------------------------------------
	void SetGroupable(BOOL bGroupable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the column's expand collapse right icon (+/-).
	// Parameters:
	//     bPlusMinus - TRUE to enable, FALSE to disable.
	// See also:
	//     IsPlusMinus(...)
	//-----------------------------------------------------------------------
	void SetPlusMinus(BOOL bPlusMinus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expanded state of the column.
	// Parameters:
	//     bExpanded - TRUE to expand the column, FALSE otherwise.
	// See also:
	//     IsExpanded(...)
	//-----------------------------------------------------------------------
	void SetExpanded(BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index of the next visual column.
	// Parameters:
	//     nNextVisualBlock - Zero-based index of the column.
	// See also:
	//     GetNextVisualBlock(...)
	//-----------------------------------------------------------------------
	void SetNextVisualBlock(int nNextVisualBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if columns can be dragged with the mouse.
	// Parameters:
	//     bAllowDrag - TRUE to allow columns to be dragged with the mouse,
	//                  FALSE to disallow columns from being dragged with the mouse.
	// See Also:
	//     SetSortable, SetGroupable
	//-----------------------------------------------------------------------
	void SetAllowDrag(BOOL bAllowDrag);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the array of column objects.
	// Returns:
	//     A pointer to the array of column objects.
	//-----------------------------------------------------------------------
	CXTPGridColumns* GetColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent Grid control.
	// Returns:
	//     A pointer to the parent Grid control.
	// See Also:
	//     GetColumns
	//-----------------------------------------------------------------------
	CXTPGridControl* GetControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column header will appear in the field chooser
	//     when the column is hidden or in the "Group By Box."
	// Remarks:
	//     When a column is hidden or added to the "Group By Box", it will be
	//     automatically added to the FieldChooser unless the ShowInFieldChooser
	//     property is set to FALSE.
	//     If you would like to have a hidden column that the user cannot access
	//     and you enable the FieldChooser, then you will need to set the
	//     ShowInFieldChooser property to FALSE. By default, this property is TRUE.
	// Returns:
	//     TRUE if the column header will appear in the field chooser when the
	//     column is hidden or in the "Group By Box", otherwise FALSE.
	// See Also:
	//     SetShowInFieldChooser
	//-----------------------------------------------------------------------
	BOOL IsShowInFieldChooser() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the column header will appear in the field chooser
	//     when the column is hidden or in the "Group By Box."
	// Parameters:
	//     bShow - TRUE to show the column header in the field chooser when the
	//             column is hidden or in the "Group By Box", FALSE otherwise.
	// Remarks:
	//     When a column is hidden or added to the "Group By Box", it will be
	//     automatically added to the FieldChooser unless the ShowInFieldChooser
	//     property is set to FALSE.
	//     If you would like to have a hidden column that the user cannot access
	//     and you enable the FieldChooser, then you will need to set the
	//     ShowInFieldChooser property to FALSE. By default, this property is TRUE.
	// See Also:
	//     IsShowInFieldChooser
	//-----------------------------------------------------------------------
	void SetShowInFieldChooser(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit options of the column.
	// Returns:
	//     A pointer to a CXTPGridRecordItemEditOptions object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemEditOptions* GetEditOptions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the display options of the column.
	// Returns:
	//     A pointer to a CXTPGridColumnDisplayOptions object.
	//-----------------------------------------------------------------------
	CXTPGridColumnDisplayOptions* GetDisplayOptions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the editable state of items in the column.
	// Remarks:
	//     If the items in the column are editable, then an edit box will appear when
	//     an item is clicked/double-clicked that allows for that item to be edited.
	//     This can be overridden for individual items by calling the
	//     CXTPGridRecordItem::SetEditable method.
	// Returns:
	//     TRUE if the items in the column are editable, otherwise FALSE.
	// See Also:
	//     SetEditable
	//-----------------------------------------------------------------------
	BOOL IsEditable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the editable state of items in the column.
	// Parameters:
	//     bEditable - TRUE to set the items in the column to editable,
	//                 FALSE otherwise.
	// Remarks:
	//     If the items in the column are editable, then an edit box will appear when
	//     an item is clicked/double-clicked that allows for that item to be edited.
	//     This can be overridden for individual items by calling the
	//     CXTPGridRecordItem::SetEditable method.
	// See Also:
	//     IsEditable
	//-----------------------------------------------------------------------
	void SetEditable(BOOL bEditable = TRUE);

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
	//     CXTPGridHeader::AllowColumnRemove
	//-----------------------------------------------------------------------
	void AllowRemove(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if column removing is allowed.
	// Returns:
	//     TRUE if column removing is allowed, otherwise FALSE.
	// See Also:
	//     AllowRemove
	//-----------------------------------------------------------------------
	BOOL IsAllowRemove() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the column print width.
	// Parameters:
	//     nTotalWidth - Total width, in pixels.
	// Remarks:
	//     Returns the current column width in logical units for print mode.
	// Returns:
	//     The column print width.
	//-----------------------------------------------------------------------
	int GetPrintWidth(int nTotalWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the internal column name used in serialization to uniquely
	//     identify a column rather than a column index.
	// Returns:
	//     The internal column name.
	//-----------------------------------------------------------------------
	CString GetInternalName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the internal column name used in serialization to uniquely
	//     identify a column rather than a column index.
	// Parameters:
	//     pcszName - Internal column name to be set.
	//-----------------------------------------------------------------------
	void SetInternalName(LPCTSTR pcszName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the vertical grid will be shown for header rows.
	// Returns:
	//     TRUE if the vertical grid will be shown for header rows, FALSE otherwise.
	// See Also:
	//     SetDrawHeaderRowsVGrid
	//-----------------------------------------------------------------------
	BOOL GetDrawHeaderRowsVGrid() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the vertical grid will be shown for footer rows.
	// Returns:
	//     TRUE if the vertical grid will be shown for footer rows, FALSE otherwise.
	// See Also:
	//     SetDrawFooterRowsVGrid
	//-----------------------------------------------------------------------
	BOOL GetDrawFooterRowsVGrid() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the vertical grid will be shown for header rows.
	// Parameters:
	//     bDrawVGrid - TRUE to show the vertical grid for header rows.
	// See Also:
	//     GetDrawHeaderRowsVGrid
	//-----------------------------------------------------------------------
	void SetDrawHeaderRowsVGrid(BOOL bDrawVGrid = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the vertical grid will be shown for footer rows.
	// Parameters:
	//     bDrawVGrid - TRUE to show the vertical grid for footer rows.
	// See Also:
	//     GetDrawFooterRowsVGrid
	//-----------------------------------------------------------------------
	void SetDrawFooterRowsVGrid(BOOL bDrawVGrid = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that this column is visible.
	//     This will cause the Grid control to scroll to this column.
	//-----------------------------------------------------------------------
	virtual void EnsureVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the column best fit mode.
	// Returns:
	//     The column best fit mode; one of the values defined
	//     by the XTPGridColumnBestFitMode enumeration.
	// See Also:
	//     SetBestFitMode
	//-----------------------------------------------------------------------
	int GetBestFitMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the column best fit mode.
	// Parameters:
	//     nMode - Column best fit mode to be set; must be one of the values defined
	//             by the XTPGridColumnBestFitMode enumeration.
	// See Also:
	//     GetBestFitMode
	//-----------------------------------------------------------------------
	void SetBestFitMode(int nMode);

	// private:
	//{{AFX_CODEJOCK_PRIVATE
	int GetNormAlignment(int nAlignment) const;
	int GetBestFitWidth();
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Markup UI element object.
	// Returns:
	//     A pointer to the Markup UI element object.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Resets the Markup UI element object.
	// See Also:
	//      GetMarkupUIElement, CXTPGridControl::EnableMarkup
	//-----------------------------------------------------------------------
	virtual void ResetMarkupUIElement();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the fixed state for the column.
	// Parameters:
	//     bFixed - TRUE to fix the column, FALSE to unfix it.
	//-----------------------------------------------------------------------
	void SetFixed(BOOL bFixed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the fixed state of the column.
	// Returns:
	//     TRUE if the column is fixed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsFixed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the frozen state of the column.
	// Returns:
	//     TRUE if the column is frozen, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsFrozen() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the frozen state for the column.
	// Parameters:
	//     bFreeze - TRUE to freeze the column, FALSE to unfreeze it.
	//-----------------------------------------------------------------------
	void SetFrozen(BOOL bFrozen);

	BOOL IsAutoNumbering() const;

	void SetAutoNumbering(BOOL bAutoNumbering);

	int GetAutoNumberingBase() const;

	void SetAutoNumberingBase(int nAutoNumberingBase);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a specified mask to a CXTPGridColumn data field
	//     to create a restrictive field for text entry.
	// Parameters:
	//     strMask    - Mask of the item.
	//     strLiteral - Defines the prompt area of the mask where text entry
	//                  is allowed. This value should match the areas that
	//                  you have already defined as mask fields. By default,
	//                  the prompt character is '_'.
	//
	//                  Phone No: (___) ___-____
	//
	//
	//     chPrompt   - (Optional) This value is the prompt character used
	//                  in strLiteral to indicate a place holder. By default,
	//                  this parameter is the underscore '_' character.
	// Remarks:
	//     You can use any of these characters for defining your edit mask:
	//     <TABLE>
	//          <b>Mask Character</b>  <b>Description</b>
	//          ---------------------  ------------------------
	//          0                      Numeric (0-9)
	//          9                      Numeric (0-9) or space (' ')
	//          #                      Numeric (0-9) or space (' ') or ('+') or ('-')
	//          L                      Alpha (a-Z)
	//          ?                      Alpha (a-Z) or space (' ')
	//          A                      Alpha numeric (0-9 and a-Z)
	//          a                      Alpha numeric (0-9 and a-Z) or space (' ')
	//          &                      All print character only
	//          H                      Hex digit (0-9 and A-F)
	//          X                      Hex digit (0-9 and A-F) and space (' ')
	//          >                      Forces characters to upper case (A-Z)
	//          <                      Forces characters to lower case (a-z)
	//     </TABLE>
	//
	//      Any of these characters can be combined with additional
	//      characters to create a restrictive field for text entry.
	//      For example, if you wanted to display a prompt for a
	//      telephone number and wanted to restrict only numeric
	//      values, but wanted to display the area code inside of
	//      parentheses '(' and ')' then you could define your
	//      mask as:
	//
	//      Phone No: (000) 000-0000
	//
	//-----------------------------------------------------------------------
	void SetMask(LPCTSTR strMask, LPCTSTR strLiteral, TCHAR chPrompt = _T('_'));

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets string to use as edit mask.
	// Returns:
	//     String to use as edit mask.
	// See Also:
	//      SetMask
	//-----------------------------------------------------------------------
	CString GetMask() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets string to use as literal.
	// Returns:
	//     String to use as literal.
	// See Also:
	//      SetMask
	//-----------------------------------------------------------------------
	CString GetLiteral() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets character used as a space holder for a character.
	// Returns:
	//     Character used as a space holder for a character.
	// See Also:
	//      SetMask
	//-----------------------------------------------------------------------
	TCHAR GetPrompt() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the specified mask to a data field.
	// Returns:
	//     TRUE if the mask is used, otherwise FALSE.
	// See Also:
	//      SetMask
	//-----------------------------------------------------------------------
	BOOL IsUseMask() const;

private:
	void _initData(int nItemIndex, LPCTSTR strDisplayName, LPCTSTR strInternalName, int nWidth,
				   BOOL bAutoSize, int nIconID, BOOL bSortable, BOOL bVisible);

protected:
	CString m_strInternalName; // Internal column name (to serialize column options).
	CString m_strName;		   // Column name.
	CString m_strTooltip;	  // Column tooltip.
	int m_nItemIndex;		   // Index of the associated record item.

	CString m_strFooterText; // Column Footer Text.

	CXTPFont m_xtpFontFooter; // Column Footer font. If Font handle is NULL, then the default font
							  // is used.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontFooter, m_xtpFontFooter,
										  GetFooterFontHandle);

	BOOL m_bDrawFooterDivider; // If TRUE, then draw footer column right divider.
	BOOL m_bDrawHeaderDivider; // If TRUE, then draw header column right divider.

	BOOL m_bAutoNumbering;	// Enables auto-numbering.
	int m_nAutoNumberingBase; // Base index for auto-numbering.

	BOOL m_bSortIncreasing;		 // Stores column sort direction (used when m_bSortable is TRUE).
	BOOL m_bSortable;			 // TRUE if the column could be sortable by.
	BOOL m_bGroupable;			 // TRUE if the column could be grouped by.
	BOOL m_bAutoSortWhenGrouped; // Column will be sortable when grouped by, otherwise it will
								 // grouped without sorting by it (sort by  other columns wiil be
								 // applied).
	BOOL m_bFixed;				 // TRUE if the column is fixed.
	BOOL m_bFrozen;				 // TRUE if the column is frozen (i.e. not scrollable).
	BOOL m_bFilterable;			 // TRUE if filtering by this column is allowed.

	BOOL m_bPlusMinus;
	// Determines if the column have Expand / Collapse right icon (+/-).
	// See IsPlusMinus() and SetPlusMinus().

	BOOL m_bExpanded;
	// State of PlusMinus column.

	int m_nNextVisualBlock;
	// Number of next visual columns to show or hide
	// see SetNextVisualBlock(...) and GetNextVisualBlock().

	BOOL m_bAllowDrag;   // Allow/disallow dragging of the column.
	BOOL m_bAllowRemove; // TRUE to allow column remove.

	CXTPGridColumns* m_pColumns; // Pointer to the parent columns collection.

	int m_nIconID;			// Associated icon ID.
	BOOL m_bVisible;		// Visible state.
	int m_nOldVisibleIndex; // Visible index before Hide state.
	// Release 13.1

	BOOL m_bIsResizable; // Indicates if the column allows for width resizing.
	CRect m_rcColumn;	// Saves rectangle where column was drawn last time.
	CRect m_rcGroupBy;   // Saves rectangle where column was drawn in group box.
	int m_nMinWidth;	 // Minimal column width. It is not allowed to do it less then minimal.
	int m_nMaxWidth; // Maximal column width (if set as positive). It is not allowed to do it more
					 // then maximal.
	int m_nMaxItemWidth; // Contains maximal width of the corresponding item drawn in the column.
	int m_nAlignment;	// Alignment mode for column: DT_LEFT || DT_RIGHT || DT_CENTER.

	int m_nHeaderAlignment; // Alignment mode for column text: DT_LEFT || DT_RIGHT || DT_CENTER.
	int m_nFooterAlignment; // Alignment mode for column footer text: DT_LEFT || DT_RIGHT ||
							// DT_CENTER.

	int m_nColumnAutoWidth;   // The column automatic width
	int m_nColumnStaticWidth; // The user defined width.
	BOOL m_bAutoSize;		  // TRUE if column auto-sized.

	BOOL m_bShowInFieldChooser; // Indicates if the column is shown in the field chooser when the
								// column is hidden or in the GroupBy Box.
	BOOL m_bEditable; // Indicates whether an edit box should be added to the items in the column.
	BOOL m_bDrawHeaderRowsVGrid; // Indicates whether to draw vertical grid for headers rows.
	BOOL m_bDrawFooterRowsVGrid; // Indicates whether to draw vertical grid for footers rows.
	int m_nBestFitMode;			 // Best fit mode. See enum XTPGridColumnBestFitMode.

	CXTPGridRecordItemEditOptions* m_pEditOptions;   // Edit options of the column.
	CXTPGridColumnDisplayOptions* m_pDisplayOptions; // Display options of the column.
	CXTPMarkupUIElement* m_pMarkupUIElement;		 // Markup Object.

	CString m_strMask;	// String to use as edit mask.
	CString m_strLiteral; // String to use as literal. This is the same as the edit mask, but all
						  // mask characters are replaced with m_chPrompt.
	TCHAR m_chPrompt;	 // Character used as a space holder for a character. This is used in
						  // m_strLiteral.
	BOOL m_bUseMask;	  // TRUE to use an edit mask to display item data.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridColumn);

	void OleMove(long nIndex);
	BSTR OleGetFooterText();
	BSTR OleGetCaption();

	void OleBestFit();

	LPFONTDISP OleGetFooterFont();
	void OleSetFooterFont(LPFONTDISP pFontDisp);

	afx_msg void OleSetMask(LPCTSTR strMask, LPCTSTR strLiteral, LPCTSTR chPrompt);

public:
	static CXTPGridColumn* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

	LPDISPATCH OleGetEditOptions();
	COleVariant m_oleTag;

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/
};

AFX_INLINE CString CXTPGridColumn::GetCaption() const
{
	return m_strName;
}

AFX_INLINE int CXTPGridColumn::GetIconID() const
{
	return m_nIconID;
}

AFX_INLINE void CXTPGridColumn::SetIconID(int nIconId)
{
	m_nIconID = nIconId;
}

AFX_INLINE BOOL CXTPGridColumn::IsFilterable() const
{
	return m_bFilterable;
}

AFX_INLINE BOOL CXTPGridColumn::IsResizable() const
{
	return m_bIsResizable;
}

AFX_INLINE void CXTPGridColumn::EnableResize(BOOL bIsResizable)
{
	m_bIsResizable = bIsResizable;
}

AFX_INLINE BOOL CXTPGridColumn::IsPlusMinus() const
{
	return m_bPlusMinus;
}

AFX_INLINE BOOL CXTPGridColumn::IsExpanded() const
{
	return m_bExpanded;
}

AFX_INLINE int CXTPGridColumn::GetNextVisualBlock() const
{
	return m_nNextVisualBlock;
}

AFX_INLINE BOOL CXTPGridColumn::IsAutoSortWhenGrouped() const
{
	return m_bAutoSortWhenGrouped;
}

AFX_INLINE BOOL CXTPGridColumn::IsAllowDragging() const
{
	return m_bAllowDrag;
}

AFX_INLINE void CXTPGridColumn::SetAutoSortWhenGrouped(BOOL bAutoSortWhenGrouped)
{
	m_bAutoSortWhenGrouped = bAutoSortWhenGrouped;
}

AFX_INLINE void CXTPGridColumn::SetSortIncreasing(BOOL bSortIncreasing)
{
	m_bSortIncreasing = bSortIncreasing;
}

AFX_INLINE CXTPGridRecordItemEditOptions* CXTPGridColumn::GetEditOptions() const
{
	return m_pEditOptions;
}

AFX_INLINE CXTPGridColumnDisplayOptions* CXTPGridColumn::GetDisplayOptions() const
{
	return m_pDisplayOptions;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPGridColumn::SetAutoSize(BOOL bSet)
{
	m_bAutoSize = bSet;
}

AFX_INLINE BOOL CXTPGridColumn::IsAutoSize() const
{
	return m_bAutoSize;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPGridColumn::SetFixed(BOOL bFixed)
{
	m_bFixed = bFixed;
}

AFX_INLINE BOOL CXTPGridColumn::IsFixed() const
{
	return m_bFixed;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPGridColumn::IsFrozen() const
{
	return m_bFrozen;
}

AFX_INLINE void CXTPGridColumn::SetFrozen(BOOL bFrozen)
{
	m_bFrozen = bFrozen;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPGridColumn::IsAutoNumbering() const
{
	return m_bAutoNumbering;
}

AFX_INLINE void CXTPGridColumn::SetAutoNumbering(BOOL bAutoNumbering)
{
	m_bAutoNumbering = bAutoNumbering;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTPGridColumn::GetAutoNumberingBase() const
{
	return m_nAutoNumberingBase;
}

AFX_INLINE void CXTPGridColumn::SetAutoNumberingBase(int nAutoNumberingBase)
{
	m_nAutoNumberingBase = nAutoNumberingBase;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPGridColumn::SetGroupable(BOOL bGroupable)
{
	m_bGroupable = bGroupable;
}

AFX_INLINE BOOL CXTPGridColumn::IsGroupable() const
{
	return m_bGroupable;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPGridColumn::SetSortable(BOOL bSortable)
{
	m_bSortable = bSortable;
}

AFX_INLINE BOOL CXTPGridColumn::IsSortable() const
{
	return m_bSortable;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPGridColumn::IsEditable() const
{
	return m_bEditable;
}

AFX_INLINE void CXTPGridColumn::SetEditable(BOOL bEditable)
{
	m_bEditable = bEditable;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPGridColumn::AllowRemove(BOOL bAllow)
{
	m_bAllowRemove = bAllow;
}

AFX_INLINE BOOL CXTPGridColumn::IsAllowRemove() const
{
	return m_bAllowRemove;
}

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CString CXTPGridColumn::GetTooltip() const
{
	return m_strTooltip;
}

AFX_INLINE void CXTPGridColumn::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strTooltip = lpszTooltip;
}

AFX_INLINE CString CXTPGridColumn::GetFooterText() const
{
	return m_strFooterText;
}

AFX_INLINE void CXTPGridColumn::SetHeaderAlignment(int nAlignment)
{
	m_nHeaderAlignment = nAlignment;
}

AFX_INLINE BOOL CXTPGridColumn::GetDrawFooterDivider() const
{
	return m_bDrawFooterDivider;
}

AFX_INLINE BOOL CXTPGridColumn::GetDrawHeaderDivider() const
{
	return m_bDrawHeaderDivider;
}

AFX_INLINE CString CXTPGridColumn::GetInternalName() const
{
	return m_strInternalName;
}

AFX_INLINE void CXTPGridColumn::SetInternalName(LPCTSTR pcszName)
{
	m_strInternalName = pcszName;
}

AFX_INLINE BOOL CXTPGridColumn::GetDrawHeaderRowsVGrid() const
{
	return m_bDrawHeaderRowsVGrid;
}

AFX_INLINE void CXTPGridColumn::SetDrawHeaderRowsVGrid(BOOL bDrawVGrid)
{
	m_bDrawHeaderRowsVGrid = bDrawVGrid;
}

AFX_INLINE BOOL CXTPGridColumn::GetDrawFooterRowsVGrid() const
{
	return m_bDrawFooterRowsVGrid;
}

AFX_INLINE void CXTPGridColumn::SetDrawFooterRowsVGrid(BOOL bDrawVGrid)
{
	m_bDrawFooterRowsVGrid = bDrawVGrid;
}

AFX_INLINE int CXTPGridColumn::GetBestFitMode() const
{
	return m_nBestFitMode;
}

AFX_INLINE void CXTPGridColumn::SetBestFitMode(int nMode)
{
	m_nBestFitMode = nMode;
}

AFX_INLINE CXTPMarkupUIElement* CXTPGridColumn::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}

AFX_INLINE CString CXTPGridColumn::GetMask() const
{
	return m_strMask;
}

AFX_INLINE CString CXTPGridColumn::GetLiteral() const
{
	return m_strLiteral;
}

AFX_INLINE TCHAR CXTPGridColumn::GetPrompt() const
{
	return m_chPrompt;
}

AFX_INLINE BOOL CXTPGridColumn::IsUseMask() const
{
	return m_bUseMask;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDCOLUMN_H__)
