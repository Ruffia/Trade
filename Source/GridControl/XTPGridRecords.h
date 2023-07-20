// XTPGridRecords.h: interface for the CXTPGridRecords class.
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
#if !defined(__XTPGRIDRECORDS_H__)
#	define __XTPGRIDRECORDS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRecord;
class CXTPPropExchange;
class CXTPMarkupContext;
class CXTPGridRecordItem;
class CXTPGridRecordItemRange;

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of text search parameters.
// Remarks:
//     Call CXTPGridRecords::FindRecordItem to search for a record item by text.
// Example:
//     <code>m_wndGrid.GetRecords()->FindRecordItem(... , xtpGridTextSearchWholeWord |
//     xtpGridTextSearchBackward);</code>
// See Also:
//     CXTPGridRecords::FindRecordItem
//-----------------------------------------------------------------------
enum XTPGridTextSearchParms
{
	xtpGridTextSearchExactPhrase = 1, // Search exact phrase.
	xtpGridTextSearchMatchCase   = 2, // Match case during search.
	xtpGridTextSearchBackward	= 4, // Search backward.
	xtpGridTextSearchExactStart  = 8, // Search phrase starting in the beginning of the item
									  // caption.
};

//===========================================================================
// Summary:
//     This class represents a grid records collection.
//     It supports an array of CXTPGridRecord pointers.
// Example:
//     See example for the CXTPGridRecords::Add method.
// See Also:
//     CXTPGridRecord
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecords : public CXTPHeapObjectT<CXTPCmdTarget, CXTPGridDataAllocator>
{
	DECLARE_DYNAMIC(CXTPGridRecords)

	void _Init();
	void _swapIndexes(int& A, int& B);
	void _clampIndexes(int& nOrig, const int& nMin, const int& nMax);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPGridRecord pointer array.
	// Parameters:
	//     bArray - Flag for array-based case (if TRUE).
	// Example:
	// <code>
	// // Declare a local CXTPGridRecords object.
	// CXTPGridRecords myList;
	//
	// // Declare a dynamic CXTPGridRecords object.
	// CXTPGridRecords* pTree = new CXTPGridRecords();
	// </code>
	//-----------------------------------------------------------------------
	CXTPGridRecords(BOOL bArray = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPGridRecord pointer array in case it is
	//     used as a member of a CXTPGridRecord object to store children.
	// Parameters:
	//     pOwnerRecord - Pointer to the owner CXTPGridRecord object.
	//-----------------------------------------------------------------------
	CXTPGridRecords(CXTPGridRecord* pOwnerRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRecords object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains an associated owner record pointer.
	// Returns:
	//     An associated owner record pointer, or NULL if no owner record
	//     is associated with the records collection.
	//-----------------------------------------------------------------------
	CXTPGridRecord* GetOwnerRecord() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of record elements in this collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array. Since
	//     indexes are zero-based, the size is 1 greater than the largest index.
	// Returns:
	//     The number of record elements in this collection.
	// Example:
	//     See example for the CXTPGridRecords::Add method.
	// See Also:
	//     CXTPGridRecords overview
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record element at a specified numeric index in this collection.
	// Parameters:
	//     nIndex - Integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Returns:
	//     A pointer to the record element at the specified numeric index.
	// Example:
	//     See example for the CXTPGridRecords::Add method.
	//-----------------------------------------------------------------------
	CXTPGridRecord* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new record element to the end of this collection.
	// Parameters:
	//     pRecord - Pointer to the new record element to be added.
	// Remarks:
	//     Use this method to add a specified CXTPGridRecord pointer
	//     to the end of the CXTPGridRecords collection.
	// Returns:
	//     A pointer to the newly added record element.
	// Example:
	// <code>
	// CXTPGridRecords* pList = new CXTPGridRecords();
	// pList->Add(new CXTPGridRecord());
	// pList->Add(new CXTPGridRecord());
	// CXTPGridRecord* pRecord0 = pList->GetAt(0);
	// CXTPGridRecord* pRecord1 = pList->GetAt(1);
	// ASSERT(2 == pList->GetCount());
	// pList->RemoveAll();
	// ASSERT(0 == pList->GetCount());
	// </code>
	// See Also:
	//     CXTPGridRecords overview, GetAt, RemoveAll, GetCount
	//-----------------------------------------------------------------------
	CXTPGridRecord* Add(CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid records collection.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all the elements from this collection.
	// Remarks:
	//     Removes all the pointers from this array and releases the
	//     instances of all stored CXTPGridRecord objects.
	// Example:
	//     See example for the CXTPGridRecords::Add method.
	// See Also:
	//     CXTPGridRecords overview
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified record from this collection by its index.
	// Parameters:
	//     nIndex - Index of the record to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified record from this collection.
	// Parameters:
	//     pRecord - Pointer to the record to be removed.
	// Returns:
	//     The index of the removed record, or a value of -1 if the
	//     specified record does not exist within this collection.
	//-----------------------------------------------------------------------
	int RemoveRecord(CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified record at a specified index in this collection.
	// Parameters:
	//     nIndex  - Index to insert the specified record at.
	//     pRecord - Record to be inserted.
	// Remarks:
	//     InsertAt inserts an element at a specified index in this array.
	//     In the process, it shifts up (by incrementing the index) the
	//     existing element at this index, and it shifts up all the elements
	//     above it.
	// Example:
	//     See example for the CXTPGridRecords::Add method.
	//-----------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves specified records to a specified index in this collection.
	// Parameters:
	//     nIndex   - Index to move the specified records to.
	//     pRecords - Records to be moved.
	//-----------------------------------------------------------------------
	void Move(int nIndex, CXTPGridRecords* pRecords);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves a specified record to a specified index in this collection.
	// Parameters:
	//     nIndex         - Index to move the specified record to.
	//     pRecord        - Record to be moved.
	//     bUpdateIndexes - Flag to update indexes to 0-N set.
	//-----------------------------------------------------------------------
	void MoveRecord(int nIndex, CXTPGridRecord* pRecord, BOOL bUpdateIndexes = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the markup context.
	// Returns:
	//     A pointer to the markup context.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the markup context.
	// Parameters:
	//     pMarkupContext - Pointer to the markup context to be set.
	//-----------------------------------------------------------------------
	void SetMarkupContext(CXTPMarkupContext* pMarkupContext);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if string comparisons should be case-sensitive.
	// Parameters:
	//     bCaseSensitive - TRUE to set string comparisons to be case-sensitive,
	//                      FALSE otherwise; the default value for this parameter is FALSE.
	// See Also:
	//     IsCaseSensitive
	//-----------------------------------------------------------------------
	void SetCaseSensitive(BOOL bCaseSensitive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if string comparisons are case-sensitive.
	// Returns:
	//     TRUE if string comparisons are case-sensitive, otherwise FALSE.
	// See Also:
	//     SetCaseSensitive
	//-----------------------------------------------------------------------
	BOOL IsCaseSensitive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Helper function to compare two strings.
	// Parameters:
	//     str1 - First string to compare.
	//     str2 - Second string to compare.
	// Returns:
	//     Zero if str1 is identical to str2;
	//     Less than zero if str1 is less than str2;
	//     Greater than zero if str1 is greater than str2.
	// See Also:
	//     SetCaseSensitive
	//-----------------------------------------------------------------------
	virtual int Compare(const CString& str1, const CString& str2) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the size of this collection.
	// Parameters:
	//     nNewSize - New size for this collection.
	//     nGrowBy  - Minimum number of element slots to allocate in the
	//                event that a size increase is necessary.
	//-----------------------------------------------------------------------
	virtual void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a specified record element to a specified index in this collection.
	// Parameters:
	//     nIndex  - Integer index to set the specified record element to;
	//               must be greater than or equal to zero and less than
	//               the value returned by GetCount.
	//     pRecord - Pointer to the CXTPGridRecord object to add.
	//-----------------------------------------------------------------------
	virtual void SetAt(INT_PTR nIndex, CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates record element indices.
	// Parameters:
	//     nStart - Integer index to begin the update from.
	//-----------------------------------------------------------------------
	virtual void UpdateIndexes(int nStart = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a record element by its bookmark.
	// Parameters:
	//     vtBookmark        - Bookmark of the record element to retrieve.
	//     bSearchInChildren - TRUE to search in children too, FALSE otherwise.
	// Returns:
	//     A pointer to the record element if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRecord* FindRecordByBookmark(VARIANT vtBookmark, BOOL bSearchInChildren);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a record item by its text.
	// Parameters:
	//     nStartRecord - Starting record index.
	//     nEndRecord   - Ending record index.
	//     nStartColumn - Starting column index.
	//     nEndColumn   - Ending column index.
	//     nRecord      - Record index to start the search from.
	//     nItem        - Record item index to start the search from.
	//     pcszText     - Search text.
	//     nFlags       - Search parameters.
	// Returns:
	//     A pointer to the record item if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CXTPGridRecordItem* FindRecordItem(int nStartRecord, int nEndRecord, int nStartColumn,
											   int nEndColumn, int nRecord, int nItem,
											   LPCTSTR pcszText, int nFlags);

	void MergeItems(const CXTPGridRecordItemRange& range);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before record dropping is processed.
	// Parameters:
	//     pTargetRecord  - Pointer to the record which is the target for dropping. Set
	//                      to NULL to skip recursion checking. Also can be NULL when
	//                      records are dropped on the top level of the Grid control.
	//     pTargetRecords - Pointer to the records which are the target for dropping.
	//     bChangeParent  - TRUE to change the parent of the current CXTPGridRecords.
	//-----------------------------------------------------------------------
	virtual void PrepareToDropping(CXTPGridRecord* pTargetRecord, CXTPGridRecords* pTargetRecords,
								   BOOL bChangeParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes redundant records.
	//-----------------------------------------------------------------------
	virtual void CleanOfRedundant();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void SetVirtualMode(CXTPGridRecord* pVirtualRecord, int nCount);
	BOOL IsVirtualMode() const;
	virtual void _DoPropExchange(CXTPPropExchange* pPX);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CArray<CXTPGridRecord*, CXTPGridRecord*> m_arrRecords; // Internal storage array for record
														   // items.
	CXTPGridRecord* m_pVirtualRecord;					   // Virtual record.
	int m_nVirtualRecordsCount;							   // Virtual records count.
	CXTPGridControl* m_pControl;						   // Pointer to the parent Grid control.
	//{{AFX_CODEJOCK_PRIVATE
	BOOL m_bArray;
	//}}AFX_CODEJOCK_PRIVATE

	BOOL m_bCaseSensitive; // Indicates whether string comparisons are case-sensitive.

	CXTPGridRecord* m_pOwnerRecord; // Pointer to the owner record object (or NULL).

	CXTPMarkupContext* m_pMarkupContext; // Markup context;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecords);

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg LPDISPATCH OleAdd();

	afx_msg LPDISPATCH OleInsert(int nIndex);
	afx_msg void OleInsertAt(int nIndex, LPDISPATCH pdispRecord);

	afx_msg LPDISPATCH OleFindRecordItem(int nStartRecord, int nEndRecord, int nStartColumn,
										 int nEndColumn, int nRecord, int nItem, LPCTSTR pcszText,
										 int nFlags);

	afx_msg void OleMergeItems(int nRecordFrom, int nRecordTo, int nColumnFrom, int nColumnTo);

	afx_msg void OleDoPropExchange(LPDISPATCH lpPropExchage);

	DECLARE_ENUM_VARIANT(CXTPGridRecords)

	enum
	{
		dispidCount		 = 1L,
		dispidAdd		 = 2L,
		dispidFindColumn = 4L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPGridControl;
	friend class CXTPGridRecord;
	friend class CXTPGridSection;
};

AFX_INLINE BOOL CXTPGridRecords::IsVirtualMode() const
{
	return m_pVirtualRecord != NULL;
}

AFX_INLINE void CXTPGridRecords::SetCaseSensitive(BOOL bCaseSensitive)
{
	m_bCaseSensitive = bCaseSensitive;
}

AFX_INLINE BOOL CXTPGridRecords::IsCaseSensitive() const
{
	return m_bCaseSensitive;
}

AFX_INLINE CXTPGridRecord* CXTPGridRecords::GetOwnerRecord() const
{
	return m_pOwnerRecord;
}

AFX_INLINE CXTPMarkupContext* CXTPGridRecords::GetMarkupContext() const
{
	return m_pMarkupContext;
}

AFX_INLINE void CXTPGridRecords::SetMarkupContext(CXTPMarkupContext* pMarkupContext)
{
	m_pMarkupContext = pMarkupContext;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORDS_H__)
