// XTPGridSection.h: interface for the CXTPGridSection class.
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
#if !defined(__XTPGRIDSECTION_H__)
#	define __XTPGRIDSECTION_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridControl;
class CXTPGridRecords;
class CXTPGridRecord;
class CXTPGridRows;
class CXTPGridRow;
class CXTPGridSelectedRows;
class CXTPGridBorder;

//-----------------------------------------------------------------------
// Summary:
//     Section height algorithm.
//-----------------------------------------------------------------------
enum XTPGridSectionHeightMode
{
	xtpGridSectionHeightModeFixed,	// Equal to 'px' in HTML.
	xtpGridSectionHeightModeRelative, // Equal to '%' in HTML.
	xtpGridSectionHeightModeAuto,	 // Equal to 'auto' in HTML.
	xtpGridSectionHeightModeExpand,   // Equal to '*' in HTML.

	xtpGridSectionHeightModeAutoExpand, // Minimum auto size.
	xtpGridSectionHeightModeAutoShrink, // Maximum auto size.
};

//-----------------------------------------------------------------------
// Summary:
//     Represents a grid section.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPGridSection : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a grid section.
	//-----------------------------------------------------------------------
	CXTPGridSection(CXTPGridControl* pControl, XTPGridRowType rowType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructs a grid section.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridSection();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the border of this grid section.
	// Returns:
	//     The border of this grid section.
	//-----------------------------------------------------------------------
	CXTPGridBorder* GetBorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the user is allowed to move the selection to rows.
	// Parameters:
	//     bAllowAccess - TRUE to allow the user to move the selection to rows,
	//                    FALSE otherwise.
	// See Also:
	//     IsAllowAccess
	//-----------------------------------------------------------------------
	void AllowAccess(BOOL bAllowAccess);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user is allowed to move the selection to rows.
	// Returns:
	//     TRUE if the user is allowed to move the selection to rows,
	//     otherwise FALSE.
	// See Also:
	//     AllowAccess.
	//-----------------------------------------------------------------------
	BOOL IsAllowAccess() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the text in all CXTPGridRecordItem(s) can be edited.
	// Parameters:
	//     bAllowEdit - TRUE to allow the text in CXTPGridRecordItem(s)
	//                  to be edited, FALSE otherwise.
	// Remarks:
	//     If bAllowEdit is set to TRUE, then an edit box will be added that
	//     allows for CXTPGridRecordItem(s) to be editable.
	//     If bAllowEdit is set to FALSE, then the edit box that allows for
	//     CXTPGridRecordItem(s) to be editable will be removed.
	// See Also:
	//     IsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void AllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the text in all CXTPGridRecordItem(s) can be edited.
	// Returns:
	//     TRUE if the text in all CXTPGridRecordItem(s) can be edited,
	//     otherwise FALSE.
	// See Also:
	//     AllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables row grouping.
	// Parameters:
	//     bAllowGroup - TRUE to enable row grouping,
	//                   FALSE to disable row grouping.
	// See Also:
	//     IsAllowGroup
	//-----------------------------------------------------------------------
	void AllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if row grouping is enabled/disabled.
	// Returns:
	//     TRUE if row grouping is enabled, otherwise FALSE.
	// See Also:
	//     AllowGroup
	//-----------------------------------------------------------------------
	BOOL IsAllowGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables row sorting.
	// Parameters:
	//     bAllowSort - TRUE to enable row sorting,
	//                  FALSE to disable row sorting.
	// See Also:
	//     IsAllowSort
	//-----------------------------------------------------------------------
	void AllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if row sorting is enabled/disabled.
	// Returns:
	//     TRUE if row sorting is enabled, otherwise FALSE.
	// See Also:
	//     AllowSort
	//-----------------------------------------------------------------------
	BOOL IsAllowSort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables row filtering.
	// Parameters:
	//     bAllowFilter - TRUE to enable row filtering,
	//                    FALSE to disable row filtering.
	// See Also:
	//     IsAllowFilter
	//-----------------------------------------------------------------------
	void AllowFilter(BOOL bAllowFilter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if row filtering is enabled/disabled.
	// Returns:
	//     TRUE if row filtering is enabled, otherwise FALSE.
	// See Also:
	//     AllowFilter
	//-----------------------------------------------------------------------
	BOOL IsAllowFilter() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables rows selection.
	// Parameters:
	//     bEnable - TRUE to enable rows selection,
	//               FALSE to disable rows selection.
	// See Also:
	//     IsSelectionEnabled
	//-----------------------------------------------------------------------
	void SelectionEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if rows selection is enabled/disabled.
	// Returns:
	//     TRUE if rows selection is enabled, otherwise FALSE.
	// See Also:
	//     SelectionEnable
	//-----------------------------------------------------------------------
	BOOL IsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state of the section.
	// Parameters:
	//     bShow - TRUE to set the section to visible,
	//             FALSE to set the section to hidden.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the visible state of the section.
	// Returns:
	//     TRUE if the section is visible, otherwise FALSE.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current height mode.
	// Returns:
	//     The current height mode; one of the values defined
	//     by the XTPGridSectionHeightMode enumeration.
	// See Also:
	//     SetHeightMode
	//-----------------------------------------------------------------------
	XTPGridSectionHeightMode GetHeightMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current height mode.
	// Parameters:
	//     heightMode - New height mode; must be one of the values defined
	//                  by the XTPGridSectionHeightMode enumeration.
	// See Also:
	//     GetHeightMode
	//-----------------------------------------------------------------------
	void SetHeightMode(XTPGridSectionHeightMode heightMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the suggested height.
	// Returns:
	//     The suggested height.
	// See Also:
	//     SetHeightMode
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the suggested height.
	// Parameters:
	//     nHeight - New suggested height.
	// See Also:
	//     GetHeight
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the divider position for this section.
	// Returns:
	//     The divider position for this section; one of the values defined
	//     by the XTPGridSectionDividerPosition enumeration.
	// See Also:
	//     SetDividerPosition
	//-----------------------------------------------------------------------
	XTPGridSectionDividerPosition GetDividerPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider position for this section.
	// Parameters:
	//     dividerPosition - New divider position; must be one of the values defined
	//                       by the XTPGridSectionDividerPosition enumeration.
	// See Also:
	//     GetDividerPosition
	//-----------------------------------------------------------------------
	void SetDividerPosition(XTPGridSectionDividerPosition dividerPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the divider style for this section.
	// Returns:
	//     The divider style for this section; one of the values defined
	//     by the XTPGridSectionDividerStyle enumeration.
	// See Also:
	//     SetDividerStyle
	//-----------------------------------------------------------------------
	XTPGridSectionDividerStyle GetDividerStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider style for this section.
	// Parameters:
	//     dividerStyle - New divider style; must be one of the values defined
	//                    by the XTPGridSectionDividerStyle enumeration.
	// See Also:
	//     GetDividerStyle
	//-----------------------------------------------------------------------
	void SetDividerStyle(XTPGridSectionDividerStyle dividerStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the divider color for this section.
	// Returns:
	//     The divider color for this section.
	// See Also:
	//     SetDividerColor
	//-----------------------------------------------------------------------
	CXTPPaintManagerColor GetDividerColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider color for this section.
	// Parameters:
	//     clrDivider - New divider color.
	// See Also:
	//     GetDividerColor
	//-----------------------------------------------------------------------
	void SetDividerColor(CXTPPaintManagerColor clrDivider);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of this section.
	// Returns:
	//     The rectangle of this section.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the divider rectangle of this section.
	// Returns:
	//     The divider rectangle of this section.
	//-----------------------------------------------------------------------
	CRect GetDividerRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of data records.
	// Returns:
	//     A pointer to the collection of data records.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid data records. You may then perform standard operations
	//     on the collection (e.g. adding, removing, etc.).
	// See Also:
	//     CXTPGridRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPGridRecords* GetRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of grid rows.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of grid rows, representing the current control view.
	//     Note that the rows collection could be rebuilt automatically
	//     by executing the Populate method.
	// Returns:
	//     A pointer to the collection of grid rows.
	// Example:
	//     See example for the CXTPGridControl::BeginUpdate method.
	// See Also:
	//     CXTPGridRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPGridRows* GetRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the type of the selected rows (i.e. body, header, or footer).
	// Returns:
	//     The type of the selected rows (i.e. body, header, or footer);
	//     one of the values defined by the XTPGridRowType enumeration.
	//-----------------------------------------------------------------------
	XTPGridRowType GetRowType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a record to the records collection.
	// Parameters:
	//     pRecord - Pointer to a record to be added.
	// Remarks:
	//     Call this member function if you want to add a record to the
	//     Grid control's internal storage.
	// Example:
	// <code>
	// // this function adds 2 empty records to a Grid control
	// void Add2Empties(CXTPGridControl* pGridCtrl)
	// {
	//     pGridCtrl->AddRecord(new CXTPGridRecord());
	//     pGridCtrl->AddRecord(new CXTPGridRecord());
	// }
	// </code>
	// Returns:
	//     Pointer to the recently added record object.
	// See Also:
	//     CXTPGridRecord overview, GetRecords
	//-----------------------------------------------------------------------
	virtual CXTPGridRecord* AddRecord(CXTPGridRecord* pRecord);

	virtual void AddRecordEx(CXTPGridRecord* pRecord, CXTPGridRecord* pParentRecord = NULL,
							 int nRowChildIndex = -1, int nRecordChildIndex = -1);

	virtual void UpdateRecord(CXTPGridRecord* pRecord, BOOL bUpdateChildren);

	virtual BOOL RemoveRecordEx(CXTPGridRecord* pRecord, BOOL bAdjustLayout = TRUE,
								BOOL bRemoveFromParent = TRUE);

	virtual BOOL RemoveRowEx(CXTPGridRow* pRow, BOOL bAdjustLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes all rows and records from the section.
	//-----------------------------------------------------------------------
	virtual void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Populates the section with rows.
	// Parameters:
	//     bUpdate - TRUE to update the parent Grid control.
	//-----------------------------------------------------------------------
	virtual void Populate(BOOL bUpdate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Re-sorts the rows for this section.
	//-----------------------------------------------------------------------
	virtual void ReSortRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the indexes for all rows.
	//-----------------------------------------------------------------------
	virtual void RefreshIndexes();
	virtual void RefreshIndexes(int nIndexStart); // <combine CXTPGridSection::RefreshIndexes>

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all children of a specified row.
	// Parameters:
	//     nIndex           - Index to insert the expanded children at; used in recursion.
	//     pRow             - Pointer to a row to expand.
	//     bUpdateSelection - TRUE to update the rows selection. This is used to prevent
	//                        multiple invocation of CXTPGridSelectedRows::_OnExpanded
	//                        in recursion through InsertRow.
	// Returns:
	//     The number of expanded children.
	//-----------------------------------------------------------------------
	virtual int _DoExpand(int nIndex, CXTPGridRow* pRow, BOOL bUpdateSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all children of a specified row.
	// Parameters:
	//     nIndex - Index to insert the expanded children at; used in recursion.
	//     pRow   - Pointer to a row to expand.
	// Returns:
	//     The number of expanded children.
	//-----------------------------------------------------------------------
	virtual int _DoExpand(int nIndex, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all children of the specified row.
	// Parameters:
	//     pRow - Pointer to a row to collapse.
	//-----------------------------------------------------------------------
	virtual int _DoCollapse(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws this section on a specified device context.
	// Parameters:
	//     pDC - Pointer to a device context to draw this section on.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if virtual mode is enabled/disabled for this section.
	// Returns:
	//     TRUE if virtual mode is enabled for this section,
	//     FALSE if virtual mode is disabled for this section.
	//-----------------------------------------------------------------------
	virtual BOOL IsVirtualMode() const;

	virtual int InsertRow(int nIndex, CXTPGridRow* pRow);

	virtual int GetPageRowCount(CDC* pDC) const;

	virtual int GetTotalHeight(CDC* pDC) const;

	virtual int GetRowsHeight(CDC* pDC) const;

	virtual BOOL GetRowAt(CDC* pDC, int nWidth, int nOffset, int& nIndex, int& nTop);

	virtual int GetRowOffset(CDC* pDC, int nWidth, int nIndex);

	virtual BOOL EnsureVisible(CDC* pDC, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the markup context.
	// Parameters:
	//     pMarkupContext - Pointer to the markup context to be set.
	//-----------------------------------------------------------------------
	void SetMarkupContext(CXTPMarkupContext* pMarkupContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of records after using m_strFilterText.
	// Returns:
	//     The number of records after using m_strFilterText.
	//-----------------------------------------------------------------------
	int GetPopulatedRecordsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of this section.
	// Returns:
	//     The index of this section.
	//-----------------------------------------------------------------------
	int GetIndex() const;

protected:
	BOOL EnsureVisibleSmooth(CDC* pDC, CXTPGridRow* pRow);
	int EnsureVisibleCalc(CDC* pDC, CXTPGridRow* pRow);
	BOOL EnsureVisibleBlock(CDC* pDC, CXTPGridRow* pRow);
	BOOL EnsureVisibleIcon(CDC* pDC, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Builds a rows tree based on the provided data record.
	// Parameters:
	//     pTree      - Pointer to the rows tree to add items into.
	//     pParentRow - Pointer to the parent tree row.
	//     pRecords   - Pointer to the records collection to transfer to rows.
	// Remarks:
	//     This method recursively calls itself when building nested branches of rows.
	//-----------------------------------------------------------------------
	void BuildTree(CXTPGridRows* pTree, CXTPGridRow* pParentRow, CXTPGridRecords* pRecords);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts a rows tree collection according to the sort order for columns.
	// Parameters:
	//     pTree - Rows tree collection to sort.
	//-----------------------------------------------------------------------
	virtual void SortTree(CXTPGridRows* pTree);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts a rows collection according to the sort order for columns.
	// Parameters:
	//     pRows - Rows collection to sort.
	//-----------------------------------------------------------------------
	virtual void SortRows(CXTPGridRows* pRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of selected rows.
	// Remarks:
	//     Use this member function to retrieve access to the collection
	//     of currently selected grid rows.
	// Returns:
	//     A pointer to the collection of selected rows.
	// See Also:
	//     CXTPGridSelectedRows overview.
	//-----------------------------------------------------------------------
	CXTPGridSelectedRows* GetSelectedRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently focused row in the Grid control's view.
	// Returns:
	//     A pointer to the currently focused row if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRow* GetFocusedRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current topmost row.
	// Returns:
	//     The index of the current topmost row.
	//-----------------------------------------------------------------------
	virtual int GetScrollIndexV();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current topmost row.
	// Parameters:
	//     nIndex - Index of the row to be set as the current topmost row.
	//-----------------------------------------------------------------------
	virtual void SetScrollIndexV(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the vertical scroll offset.
	// Returns:
	//     The vertical scroll offset.
	//-----------------------------------------------------------------------
	virtual int GetScrollOffsetV() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the vertical scroll offset.
	// Parameters:
	//     nScrollOffsetV - Vertical scroll offset to be set.
	//-----------------------------------------------------------------------
	virtual void SetScrollOffsetV(int nScrollOffsetV);

	XTPGridScrollMode GetScrollModeV() const;
	XTPGridScrollMode GetScrollModeH() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all rows.
	// Remarks:
	//     The CollapseAll method collapses all CXTPGridRow objects,
	//     including child rows, that are in the Grid control.
	// See Also:
	//     ExpandAll, CXTPGridRow::SetExpanded
	//-----------------------------------------------------------------------
	void CollapseAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all rows.
	// Parameters:
	//     bRecursive - TRUE to recursively expand all levels of child rows,
	//                  FALSE to only expand one level of child rows.
	// Remarks:
	//     The ExpandAll method expands all CXTPGridRow objects,
	//     including child rows, that are in the Grid control.
	// See Also:
	//     CollapseAll, CXTPGridRow::SetExpanded
	//-----------------------------------------------------------------------
	void ExpandAll(BOOL bRecursive);

protected:
	void OnDeletedRefreshIndexes();

protected:
	CXTPMarkupContext* m_pMarkupContext; // Markup context.
	CXTPGridBorder* m_pBorder;			 // Border.

	CXTPGridControl* m_pControl; // Parent control.
	CXTPGridRecords* m_pRecords; // Records container.
	CXTPGridRows* m_pRows;		 // Rows container.
	CXTPGridRows* m_pTree;		 // Tree rows container.

	int m_nPopulatedRecordsCount; // Number of records after using m_strFilterText.

	//////////////////////////////////////////////////////////////////////////
	// Divider
	//////////////////////////////////////////////////////////////////////////

	CRect m_rcSection;								 // The area occupied by the section.
	CRect m_rcDivider;								 // Divider rectangle.
	CXTPPaintManagerColor m_clrDivider;				 // Divider color.
	XTPGridSectionDividerPosition m_dividerPosition; // Divider position.
	XTPGridSectionDividerStyle m_dividerStyle;		 // Divider style.

	XTPGridSectionHeightMode m_heightMode; // Height mode algorithm.
	int m_nHeight;						   // Suggested height (Fixed/Relative).

	BOOL m_bAllowAccess;	 // TRUE if rows can be selected.
	BOOL m_bAllowEdit;		 // TRUE if rows can be edited.
	BOOL m_bAllowGroup;		 // TRUE if rows can be grouped.
	BOOL m_bAllowSort;		 // TRUE if rows can be sorted.
	BOOL m_bAllowFilter;	 // TRUE if rows can be filtered.
	BOOL m_bSelectionEnable; // TRUE if selection enabled.
	BOOL m_bVisible;		 // TRUE if section is visible.

	//////////////////////////////////////////////////////////////////////////
	// Scrolling
	//////////////////////////////////////////////////////////////////////////
	int m_nScrollIndexV; // Current top row in the visible area.
						 // IMPORTANT: With smooth scrolling this member may
						 // be -1 when the scrolling offset is changed and
						 // the new top row index is not yet recalculated.
						 // GetTopRowIndex() always returns the correct value.

	int m_nScrollOffsetV; // Vertical scroll offset.

	XTPGridRowType m_rowType;

	CUIntArray m_UaSorted; // Sorted array.

private:
#	ifdef _XTP_ACTIVEX
	DECLARE_OLETYPELIB_EX(CXTPGridSection);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

#	endif // _XTP_ACTIVEX

	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPGridControl;
	friend class CXTPGridSections;
	friend class CXTPGridGroupRow;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE void CXTPGridSection::AllowAccess(BOOL bAllowAccess)
{
	m_bAllowAccess = bAllowAccess;
}

AFX_INLINE BOOL CXTPGridSection::IsAllowAccess() const
{
	return m_bAllowAccess;
}

AFX_INLINE void CXTPGridSection::AllowEdit(BOOL bAllowEdit)
{
	m_bAllowEdit = bAllowEdit;
}

AFX_INLINE BOOL CXTPGridSection::IsAllowEdit() const
{
	return m_bAllowEdit;
}

AFX_INLINE void CXTPGridSection::AllowGroup(BOOL bAllowGroup)
{
	m_bAllowGroup = bAllowGroup;
}

AFX_INLINE BOOL CXTPGridSection::IsAllowGroup() const
{
	return m_bAllowGroup;
}

AFX_INLINE void CXTPGridSection::AllowSort(BOOL bAllowSort)
{
	m_bAllowSort = bAllowSort;
}

AFX_INLINE BOOL CXTPGridSection::IsAllowSort() const
{
	return m_bAllowSort;
}

AFX_INLINE void CXTPGridSection::AllowFilter(BOOL bAllowFilter)
{
	m_bAllowFilter = bAllowFilter;
}

AFX_INLINE BOOL CXTPGridSection::IsAllowFilter() const
{
	return m_bAllowFilter;
}

AFX_INLINE void CXTPGridSection::SelectionEnable(BOOL bEnable)
{
	m_bSelectionEnable = bEnable;
}

AFX_INLINE BOOL CXTPGridSection::IsSelectionEnabled() const
{
	return m_bSelectionEnable;
}

AFX_INLINE void CXTPGridSection::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

AFX_INLINE BOOL CXTPGridSection::IsVisible() const
{
	return m_bVisible;
}

AFX_INLINE CXTPGridRecords* CXTPGridSection::GetRecords() const
{
	return m_pRecords;
}

AFX_INLINE CXTPGridRows* CXTPGridSection::GetRows() const
{
	return m_pRows;
}

AFX_INLINE XTPGridSectionHeightMode CXTPGridSection::GetHeightMode() const
{
	return m_heightMode;
}

AFX_INLINE void CXTPGridSection::SetHeightMode(XTPGridSectionHeightMode heightMode)
{
	m_heightMode = heightMode;
}

AFX_INLINE int CXTPGridSection::GetHeight() const
{
	return m_nHeight;
}

AFX_INLINE void CXTPGridSection::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}

AFX_INLINE XTPGridSectionDividerPosition CXTPGridSection::GetDividerPosition() const
{
	return m_dividerPosition;
}

AFX_INLINE void CXTPGridSection::SetDividerPosition(XTPGridSectionDividerPosition dividerPosition)
{
	m_dividerPosition = dividerPosition;
}

AFX_INLINE XTPGridSectionDividerStyle CXTPGridSection::GetDividerStyle() const
{
	return m_dividerStyle;
}

AFX_INLINE void CXTPGridSection::SetDividerStyle(XTPGridSectionDividerStyle dividerStyle)
{
	m_dividerStyle = dividerStyle;
}

AFX_INLINE CXTPPaintManagerColor CXTPGridSection::GetDividerColor() const
{
	return m_clrDivider;
}

AFX_INLINE void CXTPGridSection::SetDividerColor(CXTPPaintManagerColor clrDivider)
{
	m_clrDivider = clrDivider;
}

AFX_INLINE CRect CXTPGridSection::GetRect() const
{
	return m_rcSection;
}

AFX_INLINE CRect CXTPGridSection::GetDividerRect() const
{
	return m_rcDivider;
}

AFX_INLINE int CXTPGridSection::GetPopulatedRecordsCount() const
{
	return m_nPopulatedRecordsCount;
}

AFX_INLINE XTPGridScrollMode CXTPGridSection::GetScrollModeH() const
{
	XTPGridScrollMode scrollMode = xtpGridScrollModeNone;

	if (NULL != m_pControl)
	{
		scrollMode = m_pControl->GetScrollMode(xtpGridOrientationHorizontal);
	}

	return scrollMode;
}

AFX_INLINE XTPGridScrollMode CXTPGridSection::GetScrollModeV() const
{
	XTPGridScrollMode scrollMode = xtpGridScrollModeNone;

	if (NULL != m_pControl)
	{
		scrollMode = m_pControl->GetScrollMode(xtpGridOrientationVertical);
	}

	return scrollMode;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDSECTION_H__)
