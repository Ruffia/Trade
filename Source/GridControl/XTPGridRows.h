// XTPGridRows.h: interface for the CXTPGridRows class.
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
#if !defined(__XTPGRIDROWS_H__)
#	define __XTPGRIDROWS_H__
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
//     This enumeration defines the selection change action.
// See Also:
//     XTP_NM_GRID_SELCHANGING, XTP_NM_SELECTION_CHANGING
//===========================================================================
enum XTPGridSelectionChangeType
{
	xtpGridSelectionAdd,	// Row will be added to the selected rows collection.
	xtpGridSelectionRemove, // Row will be removed from the selected rows collection.
	xtpGridSelectionClear   // The selected rows collection will be cleared.
};

//===========================================================================
// Summary:
//     This struct defines data for an XTP_NM_GRID_SELCHANGING notification.
// See Also:
//     XTP_NM_GRID_SELCHANGING, XTP_NM_SELECTION_CHANGING
//===========================================================================
struct XTP_NM_SELECTION_CHANGING
{
	NMHDR hdr;		   // Standard structure, containing information
					   // about a notification message.
	CXTPGridRow* pRow; // Pointer to the row associated with the
					   // notification. It is NULL for an xtpGridSelectionClear action.
	XTPGridSelectionChangeType nType; // Selection change action.
};

//===========================================================================
// Summary:
//     This structure contains information for use in processing an
//     XTP_NM_GRID_STATECHANGED message.
//===========================================================================
struct XTP_NM_GRIDSTATECHANGED
{
	NMHDR hdr;  // Standard structure, containing information.
	int nBegin; // Zero-based index of the first item in the range of items.
	int nEnd;   // Zero-based index of the last item in the range of items.
};

class _XTP_EXT_CLASS CXTPGridScreenRows : public CArray<CXTPGridRow*, CXTPGridRow*>
{
public:
	CXTPGridScreenRows();
	~CXTPGridScreenRows();

public:
	CXTPGridRow* HitTest(CPoint pt) const;
	void Clear();
};

class _XTP_EXT_CLASS CXTPGridRowsArray : public CArray<CXTPGridRow*, CXTPGridRow*>
{
	friend class CXTPGridSection;

public:
	void ReserveSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
};

//===========================================================================
// Summary:
//     This class represents a rows collection class.
//     It supports an array of CXTPGridRow pointers.
// Example:
//     See CXTPGridRows::Add for an example of how to work with this class.
// See Also:
//     CXTPGridRow, CXTPGridSelectedRows
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRows : public CXTPHeapObjectT<CXTPCmdTarget, CXTPGridDataAllocator>
{
	void _swapIndexes(int& A, int& B);
	void _clampIndexes(int& nOrig, const int& nMin, const int& nMax);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPGridRow pointer array.
	// Example:
	// <code>
	// // Declare a local CXTPGridRows object.
	// CXTPGridRows myList;
	//
	// // Declare a dynamic CXTPGridRows object.
	// CXTPGridRows* pTree = new CXTPGridRows();
	// </code>
	//-----------------------------------------------------------------------
	CXTPGridRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRows object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this row collection contains a specified row.
	// Parameters:
	//     pRow - Pointer to a CXTPGridRow.
	// Returns:
	//     TRUE if this row collection contains the specified row.
	//-----------------------------------------------------------------------
	virtual BOOL Contains(const CXTPGridRow* pRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all the elements from this collection.
	// Parameters:
	//     bResetRow - TRUE to remove the visible flag of child rows.
	// Remarks:
	//     Removes all the pointers from this array and releases the instances
	//     of all stored CXTPGridRow objects.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	// See Also:
	//     CXTPGridRows overview
	//-----------------------------------------------------------------------
	virtual void Clear(BOOL bResetRow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of row elements in this collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Since indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	// Returns:
	//     The number of row elements in this collection.
	// See Also:
	//     CXTPGridRows overview
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the row element at a specified numeric index in this collection.
	// Parameters:
	//     nIndex - Integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Returns:
	//     A pointer to the row element at the specified numeric index.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently displayed rows of this collection.
	// Returns:
	//     A pointer to a CXTPGridScreenRows object containing the
	//     currently displayed rows of this collection.
	//-----------------------------------------------------------------------
	virtual CXTPGridScreenRows* GetScreenRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the currently focused row.
	// Returns:
	//     The index of the currently focused row.
	//-----------------------------------------------------------------------
	virtual int GetFocusedRowIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a row in this collection currently has focus.
	// Returns:
	//     TRUE if a row in this collection currently has focus.
	//-----------------------------------------------------------------------
	virtual BOOL HasFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index of the currently focused row.
	// Returns:
	//     nIndex - Index of the currently focused row to be set.
	//-----------------------------------------------------------------------
	virtual void SetFocusedRowIndex(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently focused row in the Grid control's view.
	// Returns:
	//     A pointer to the currently focused row if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* GetFocusedRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new row element to the end of this collection.
	// Parameters:
	//     pRow - Pointer to the new row element to be added.
	// Remarks:
	//     Use this method to add the specified CXTPGridRow pointer
	//     to the end of the CXTPGridRows collection.
	// Returns:
	//     The index of the newly added row element.
	// Example:
	// <code>
	// CXTPGridRows* pTree = new CXTPGridRows();
	// pTree->Add(new CXTPGridRow(pControl, pRecord1));
	// pTree->Add(new CXTPGridRow(pControl, pRecord2));
	// CXTPGridRow* pRow0 = pTree->GetAt(0);
	// CXTPGridRow* pRow1 = pTree->GetAt(1);
	// ASSERT(pRow0 == pTree->GetPrev(pRow1));
	// ASSERT(pRow1 == pTree->GetNext(pRow0));
	// pTree->RemoveAt(0);
	// ASSERT(1 == pTree->GetCount());
	// pTree->InsertAt(0, pRow0);
	// ASSERT(2 == pTree->GetCount());
	// pTree->Clear();
	// ASSERT(0 == pTree->GetCount());
	// </code>
	// See Also:
	//     CXTPGridRows overview, GetAt, InsertAt, Clear, GetNext, GetPrev, GetCount
	//-----------------------------------------------------------------------
	virtual int Add(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified row from this collection by its index.
	// Parameters:
	//     nIndex - Integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Remarks:
	//     In the process, this method shifts down all elements above the
	//     removed element. This method also decrements the upper bound
	//     of the array, but does not free memory. This class also releases
	//     the instance of the removed element.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	//-----------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified row from this collection.
	// Parameters:
	//     pRow - Pointer to the row to be removed.
	// Returns:
	//     The index of the removed row, or a value of -1 if the
	//     specified row does not exist within this collection.
	//-----------------------------------------------------------------------
	virtual int RemoveRow(CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified row at a specified index in this collection.
	// Parameters:
	//     nIndex - Integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	//     pRow   - Row to be inserted.
	// Remarks:
	//     InsertAt inserts an element at a specified index in this array.
	//     In the process, it shifts up (by incrementing the index) the
	//     existing element at this index, and it shifts up all the elements
	//     above it.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	//-----------------------------------------------------------------------
	virtual void InsertAt(int nIndex, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the next row in the list.
	// Parameters:
	//     pRow            - Pointer to the current row in the list.
	//     bSkipGroupFocus - TRUE to skip all groups.
	// Returns:
	//     A pointer to the next row in the list.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	// See Also:
	//     GetPrev, CXTPGridControl::SkipGroupsFocus
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* GetNext(CXTPGridRow* pRow, BOOL bSkipGroupFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the previous row in the list.
	// Parameters:
	//     pRow            - Pointer to the current row in the list.
	//     bSkipGroupFocus - TRUE to skip all groups.
	// Returns:
	//     A pointer to the previous row in the list.
	// Example:
	//     See example for the CXTPGridRows::Add method.
	// See Also:
	//     GetNext, CXTPGridControl::SkipGroupsFocus
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* GetPrev(CXTPGridRow* pRow, BOOL bSkipGroupFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set virtual mode with nCount rows.
	// Parameters:
	//     pRow   - Virtual row
	//     nCount - Count of virtual rows
	//-----------------------------------------------------------------------
	virtual void SetVirtualMode(CXTPGridRow* pRow, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the row associated with a specified record.
	// Parameters:
	//     pRecord - Pointer to the CXTPGridRecord object to find.
	// Remarks:
	//     This function will not search through row children.
	// Returns:
	//     A pointer to the row associated with the specified record if successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* Find(CXTPGridRecord* pRecord);
	virtual CXTPGridRow* Find(CXTPGridRecord* pRecord,
							  BOOL bRecursive); // <combine
												// CXTPGridRows::Find@CXTPGridRecord*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the row associated with a specified record.
	// Parameters:
	//     pRecord - Pointer to the CXTPGridRecord object to find.
	// Remarks:
	//     This function will search through row children.
	// Returns:
	//     A pointer to the row associated with the specified record if successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* FindInTree(CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Searches for a position to insert a specified row at.
	// Parameters:
	//     pRow          - Pointer to the CXTPGridRow object to be inserted.
	//     bInsertAfter  - Reference to a BOOL value to be set to TRUE by this method
	//                     if the specified row must be inserted after the found row.
	// Remarks:
	//     This method will search through child rows.
	// Returns:
	//     A pointer to the found CXTPGridRow object if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRow* FindInsertionPos(CXTPGridRow* pRow, BOOL& bInsertAfter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the child indices of all rows in the collection.
	// Parameters:
	//     bRunInChildren - TRUE to recalculate the child indices of row children.
	//-----------------------------------------------------------------------
	virtual void RefreshChildIndices(BOOL bRunInChildren = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the merge height for all rows
	//     (i.e. resets CXTPGridRow::m_nMergeHeight to -1 for all rows).
	//-----------------------------------------------------------------------
	void ClearMergeHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts rows in the collection.
	// Parameters:
	//     pCompareFunc - Pointer to a T_CompareFunc function
	//                    to be used to compare rows.
	// Remarks:
	//     This method uses the Visual C++ run-time library (MSVCRT) implementation
	//     of the quick-sort function, qsort, for sorting stored CXTPGridRow objects.
	//     Sort() internally uses the CompareRows method for comparing two rows.
	// See Also:
	//     CompareRows
	//-----------------------------------------------------------------------
	virtual void SortEx(XTPGridRowsCompareFunc pCompareFunc);
	virtual void Sort(); // <COMBINE CXTPGridRows::SortEx@XTPGridRowsCompareFunc>

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two rows using groups and sort orders of the Grid control.
	// Parameters:
	//     pRow1 - First row for comparison.
	//     pRow2 - Second row for comparison.
	// Remarks:
	//     This function is called directly by the Sort method.
	//
	//     The default implementation returns the result of the comparison
	//     of *pRow1 and *pRow2.
	//
	//     This implementation uses CXTPGridRecordItem::Compare to compare
	//     two corresponding items from row records. First it compares the
	//     record items by group order, then by sort order. It returns the
	//     first comparison result that differs by equality. If no differences
	//     are found, then it returns 0.
	//
	// Returns:
	//     Zero if pRow1 is equal to pRow2;
	//     Less than zero if pRow1 is less than pRow2;
	//     Greater than zero if pRow1 is greater than pRow2.
	// See Also:
	//     Sort, CXTPGridRecordItem::Compare
	//-----------------------------------------------------------------------
	static int AFX_CDECL CompareRows(const CXTPGridRow** pRow1, const CXTPGridRow** pRow2);
	static int AFX_CDECL
		CompareRows2(const CXTPGridRow** pRow1,
					 const CXTPGridRow** pRow2); //<COMBINE CXTPGridRows::CompareRows@const
												 // CXTPGridRow**@const CXTPGridRow**>

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds a record item by rows.
	// Parameters:
	//     nStartIndex  - Starting index.
	//     nEndIndex    - Ending index.
	//     nStartColumn - Starting column.
	//     nEndColumn   - Ending column.
	//     nItem        - Record item index to start the search from.
	//     nRecordIndex - Record index to start the search from.
	//     pcszText     - Search text.
	//     nFlags       - Search flags.
	// Remarks:
	//     The various values for nFlags are:
	//     * xtpGridTextSearchExactPhrase - Search exact phrase.
	//     * xtpGridTextSearchMatchCase   - Match case during search.
	//     * xtpGridTextSearchBackward    - Search backwards.
	//     * xtpGridTextSearchExactStart  - Search phrase where the start of the phrase
	//                                        matches exactly. For those familiar with grep,
	//                                        this is just like using the "^" to state that
	//                                        the text must start with the specified search.
	// Returns:
	//     A pointer to the found record item if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRecordItem* FindRecordItemByRows(int nStartIndex, int nEndIndex,
													 int nStartColumn, int nEndColumn,
													 int nRecordIndex, int nItem, LPCTSTR pcszText,
													 int nFlags);

	//{{AFX_CODEJOCK_PRIVATE
	virtual void ReserveSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
	virtual void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
	virtual void SetAt(INT_PTR nIndex, CXTPGridRow* pRow);
	//}}AFX_CODEJOCK_PRIVATE
protected:
	//{{AFX_CODEJOCK_PRIVATE

	CXTPGridRowsArray m_arrRows; // Internal storage for CXTPGridRow objects.
	//}}AFX_CODEJOCK_PRIVATE

	CXTPGridRow* m_pVirtualRow;		   // Virtual row.
	CXTPGridScreenRows* m_pScreenRows; // Rows currently presented on screen.
	int m_nVirtualRowsCount;		   // Count of virtual rows.

	int m_nFocusedRow; // Current focused row index.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRows);

	afx_msg LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();
	DECLARE_ENUM_VARIANT(CXTPGridRows)

	afx_msg LPDISPATCH OleFindRow(LPDISPATCH pRecordDisp);
	afx_msg LPDISPATCH OleFindRowInTree(LPDISPATCH pRecordDisp);
	afx_msg LPDISPATCH OleFindRecordItemByRows(int nStartIndex, int nEndIndex, int nStartColumn,
											   int nEndColumn, int nRecord, int nItem,
											   LPCTSTR pcszText, int nFlags);
	enum
	{
		dispidCount = 1L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPGridControl;
	friend class CXTPTrackControl;
	friend class CXTPGridNavigator;
	friend class CXTPGridSection;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDROWS_H__)
