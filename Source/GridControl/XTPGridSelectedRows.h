// XTPGridSelectedRows.h: interface for the CXTPGridSelectedRows class.
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
#if !defined(__XTPGRIDSELECTEDROWS_H__)
#	define __XTPGRIDSELECTEDROWS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRow;
class CXTPGridRecord;
class CXTPGridRecordItem;
class CXTPGridControl;
class CXTPGridColumns;

//===========================================================================
// Summary:
//     Encapsulates a collection of CXTPGridRow pointers that represent
//     the selected rows in a Grid control.
// Remarks:
//     Use this class to programmatically manage a collection of
//     CXTPGridRow pointers that represent the selected rows in a
//     Grid control. This class is commonly used to add or remove rows
//     from the collection.
//
//     Typical work flow is using the Add and Remove methods to change the
//     contents of the selection and using the Contains method to check
//     if a specific row is present in the selection.
// Example:
//     The following example demonstrates how to programmatically use
//     the CXTPGridSelectedRows class to select rows in the Grid control.
// <code>
// CXTPGridSelectedRows* pSelRows = pGridControl->GetSelectedRows();
// pSelRows->Add(pRow1);
// pSelRows->Add(pRow2);
// ASSERT(TRUE == pSelRows->Contains(pRow1));
// ASSERT(TRUE == pSelRows->Contains(pRow2));
// pSelRows->Remove(pRow1);
// ASSERT(FALSE == pSelRows->Contains(pRow1));
// pSelRows->Select(pRow1);
// ASSERT(TRUE == pSelRows->Contains(pRow1));
// ASSERT(FALSE == pSelRows->Contains(pRow2));
// </code>
//
// See Also:
//     CXTPGridRow, CXTPGridSelectedRows, CXTPGridControl::GetSelectedRows
//===========================================================================
class _XTP_EXT_CLASS CXTPGridSelectedRows : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPGridSelectedRows collection.
	// Parameters:
	//     pControl - Pointer to the parent Grid control.
	// Remarks:
	//     This collection can only be created in association with
	//     the CXTPGridControl object.
	// Example:
	// <code>
	// // from CXTPGridControl member function
	// CXTPGridSelectedRows* pSelectedRows = new CXTPGridSelectedRows(this);
	// </code>
	// See Also:
	//     CXTPGridSelectedRows overview
	//-----------------------------------------------------------------------
	CXTPGridSelectedRows(CXTPGridControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects a block of rows.
	// Parameters:
	//     nBlockBegin - First row index of the block.
	//     nBlockEnd   - Final row index of the block.
	//     bControlKey - Indicates the control key button state during
	//                   the selection operation.
	// Remarks:
	//     This function uses nBlockBegin and nBlockEnd as the bounds for the
	//     required selection. It enumerates the parent Grid control rows collection
	//     and adds all rows from nBlockEnd to nBlockEnd inclusively to the selection.
	//-----------------------------------------------------------------------
	void SelectBlock(int nBlockBegin, int nBlockEnd, BOOL bControlKey = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears itself, removing all elements from the selection.
	//-----------------------------------------------------------------------
	BOOL Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears itself, removing all elements from the selection.
	// Parameters:
	//     bNotifyOnClear - TRUE to send a clear notification.
	//-----------------------------------------------------------------------
	BOOL Clear(BOOL bNotifyOnClear);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a row to the selection.
	// Parameters:
	//     pRow - Pointer to the row to be added to the selection.
	// Remarks:
	//     This method adds a provided row pointer to the selection.
	//     After adding, the Contains method will return TRUE for all
	//     checking attempts of this row pointer.
	// Example:
	//     See example at CXTPGridSelectedRows overview
	// See Also:
	//     CXTPGridSelectedRows overview, Remove, Select, Clear, Contains
	//-----------------------------------------------------------------------
	BOOL Add(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a block to the selection.
	// Parameters:
	//     nIndexBegin         - First row index of the block to be selected.
	//     nIndexEnd           - Final row index of the block to be selected.
	//     bDeselectIfSelected - TRUE to deselect rows if all rows in the new selection
	//                           [nIndexBegin:nIndexEnd] were selected before.
	//-----------------------------------------------------------------------
	void AddBlock(int nIndexBegin, int nIndexEnd, BOOL bDeselectIfSelected = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a row from the selection.
	// Parameters:
	//     pRow - Pointer to the row to be removed from the selection.
	// Remarks:
	//     This method removes a provided row pointer from the selection.
	//     After removing, the Contains method will return FALSE for all
	//     checking attempts of this row pointer.
	// Example:
	//     See example at CXTPGridSelectedRows overview
	// See Also:
	//     CXTPGridSelectedRows overview, Add, Select, Clear, Contains
	//-----------------------------------------------------------------------
	void Remove(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified row is contained within the selection.
	// Parameters:
	//     pRow - Pointer to the row to check for.
	// Remarks:
	//     Use this method to determine whether the CXTPGridSelectedRows
	//     contains the specified CXTPGridRow pointer.
	// Returns:
	//     TRUE if the row is contained within the selection, otherwise FALSE.
	// Example:
	//     See example at CXTPGridSelectedRows overview
	// See Also:
	//     CXTPGridSelectedRows overview, Add, Remove, Select, Clear
	//-----------------------------------------------------------------------
	BOOL Contains(const CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inverts the selection for a specified row.
	// Parameters:
	//     pRow - Pointer to the row to invert the selection for.
	// Remarks:
	//     This methods checks the specified row for its presence in
	//     the collection and either adds or removes it in the reverse order
	//     depending on the result.
	// Example:
	// <code>
	// CXTPGridSelectedRows* pSelRows = pGridControl->GetSelectedRows();
	// ASSERT(TRUE == pSelRows->Contains(pRow1));
	// pSelRows->Invert(pRow1);
	// ASSERT(FALSE == pSelRows->Contains(pRow1));
	// pSelRows->Invert(pRow1);
	// ASSERT(TRUE == pSelRows->Contains(pRow1));
	// </code>
	// See Also:
	//     Add, Remove, Contains
	//-----------------------------------------------------------------------
	void Invert(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears the initial selection and selects a specified row.
	// Parameters:
	//     pRow - Pointer to the row to select.
	// Remarks:
	//     This method clears the initial selection and selects only the specified row.
	// Example:
	//     See example at CXTPGridSelectedRows overview
	// See Also:
	//     CXTPGridSelectedRows overview, Add, Remove, Contains, Clear
	//-----------------------------------------------------------------------
	BOOL Select(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes block [ib:ie] from the selection.
	// Parameters:
	//     ib         - First row index of the block to be deselected.
	//     ie         - Last row index of the block to be deselected.
	//     bCollapsed - BOOL flag that specifies if rows are deleted or collapsed. If TRUE,
	//                  then the selected rows below block [ib:ie] should be moved upward.
	// See Also:
	//     CXTPGridSelectedRows overview, Add, Select, Clear, Contains, Remove
	//-----------------------------------------------------------------------
	void DeselectBlock(int ib, int ie, BOOL bCollapsed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of selected rows in the collection.
	// Returns:
	//     The number of selected rows in the collection.
	// See Also:
	//     CXTPGridRows overview
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified row by its index.
	// Parameters:
	//     nIndex - Index of the row to retrieve.
	// Returns:
	//     A pointer to the specified row.
	// Remarks:
	//     Recommended to use the GetFirstSelectedRowPosition and
	//     GetNextSelectedRow methods.
	// See Also:
	//     GetFirstSelectedRowPosition, GetNextSelectedRow
	//-----------------------------------------------------------------------
	CXTPGridRow* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the first selected row in the list Grid control.
	// Returns:
	//     The position of the first selected row in the list Grid control
	//     if successful, or NULL if no items are selected.
	// Remarks:
	//     The position of the first selected row in the list Grid control
	//     can be used for iteration or object pointer retrieval.
	// See Also:
	//     GetNextSelectedItem
	//-----------------------------------------------------------------------
	POSITION GetFirstSelectedRowPosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the next selected row in the list Grid control.
	// Parameters:
	//     pos - Reference to a POSITION value returned by a previous call
	//           to GetNextSelectedRow or GetFirstSelectedRowPosition.
	//           This value is updated to the next position by this call.
	// Returns:
	//     A pointer to the next selected row in the list Grid control.
	//-----------------------------------------------------------------------
	CXTPGridRow* GetNextSelectedRow(POSITION& pos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection was changed.
	// Returns:
	//     TRUE if the selection was changed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsChanged() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the selection was changed.
	// Parameters:
	//     bChanged - TRUE if the selection was changed, FALSE otherwise.
	//                By default, this parameter is TRUE.
	//-----------------------------------------------------------------------
	void SetChanged(BOOL bChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Swaps two integers.
	// Parameters:
	//     indexB - Reference to the first integer.
	//     indexE - Reference to the second integer.
	// Remarks:
	//     The swap should only occur if indexB is greater than indexE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SwapIfNeed(int& indexB, int& indexE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if clear notifications are enabled/disabled during
	//     CXTPGridSelectedRows::Select and SelectBlock.
	// Parameters:
	//     bNotifyOnClear - TRUE to enable clear notifications,
	//                      FALSE to disable clear notifications.
	//                      By default, this parameter is TRUE.
	// Remarks:
	//     If clear notifications are enabled (TRUE), then calling
	//     CXTPGridSelectedRows::Select and SelectBlock will send
	//     xtpGridSelectionClear notifications when the selected
	//     rows are internally cleared (i.e. normal behavior).
	//     If clear notifications are disabled (FALSE), then clear notifications
	//     will not be sent during CXTPGridSelectedRows::Select and SelectBlock.
	//-----------------------------------------------------------------------
	void SetNotifyOnClear(BOOL bNotifyOnClear = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if clear notifications are enabled/disabled during
	//     CXTPGridSelectedRows::Select and SelectBlock.
	// Returns:
	//     TRUE if clear notification are enabled,
	//     FALSE if clear notifications are disabled.
	// Remarks:
	//     If clear notifications are enabled (TRUE), then calling
	//     CXTPGridSelectedRows::Select and SelectBlock will send
	//     xtpGridSelectionClear notifications when the selected
	//     rows are internally cleared (i.e. normal behavior).
	//     If clear notifications are disabled (FALSE), then clear notifications
	//     will not be sent during CXTPGridSelectedRows::Select and SelectBlock.
	//-----------------------------------------------------------------------
	BOOL GetNotifyOnClear() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the type of the selected rows (i.e. body, header, or footer).
	// Returns:
	//     The type of the selected rows (i.e. body, header, or footer);
	//     one of the values defined by the XTPGridRowType enumeration.
	//-----------------------------------------------------------------------
	XTPGridRowType GetRowType() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL _NotifySelChanging(XTPGridSelectionChangeType nType, CXTPGridRow* pRow = NULL);
	void _NotifyStateChanged(int nBegin, int nEnd);
	void _InsertBlock(int nIndexInsert, int nIndexBegin, int nIndexEnd);
	void _OnExpanded(int nIndex, int nCount);
	void _OnCollapsed(int nIndex, int nCount);

	struct SELECTED_BLOCK
	{
		int nIndexBegin;
		int nIndexEnd;
	};

	CXTPGridControl* m_pControl; // Pointer to the parent Grid control.
	int m_nRowBlockBegin;		 // Index of the row where rows block begin from.
	int m_nPosSelected;
	BOOL m_bNotifyOnClear; // If TRUE, then calling CXTPGridSelectedRows::Select and SelectBlock
						   // will send xtpGridSelectionClear notifications when the selected rows
						   // are internally cleared (normal behavior). If FALSE, then it will not
						   // send the clear notifications during CXTPGridSelectedRows::Select and
						   // SelectBlock.

	CArray<SELECTED_BLOCK, SELECTED_BLOCK&> m_arrSelectedBlocks;

	BOOL m_bChanged;
	XTPGridRowType m_nRowType; // Only rows of one type can be selected (i.e. body, header, or
							   // footer).
							   //}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridSelectedRows);

	afx_msg void OleAdd(LPDISPATCH lpRow);
	afx_msg void OleDeleteAll();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();

	DECLARE_ENUM_VARIANTLIST(CXTPGridSelectedRows);

	enum
	{
		dispidCount = 1L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPGridControl;
	friend class CXTPGridSection;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE void CXTPGridSelectedRows::SetNotifyOnClear(BOOL bNotifyOnClear)
{
	m_bNotifyOnClear = bNotifyOnClear;
}

AFX_INLINE BOOL CXTPGridSelectedRows::GetNotifyOnClear() const
{
	return m_bNotifyOnClear;
}

AFX_INLINE BOOL CXTPGridSelectedRows::IsChanged() const
{
	return m_bChanged;
}

AFX_INLINE void CXTPGridSelectedRows::SetChanged(BOOL bChanged)
{
	m_bChanged = bChanged;
}

AFX_INLINE XTPGridRowType CXTPGridSelectedRows::GetRowType() const
{
	return m_nRowType;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDSELECTEDROWS_H__)
