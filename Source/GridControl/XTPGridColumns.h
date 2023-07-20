// XTPGridColumns.h: interface for the CXTPGridColumns class.
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
#if !defined(__XTPGRIDCOLUMNS_H__)
#	define __XTPGRIDCOLUMNS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridColumn;
class CXTPGridHeader;
class CXTPGridControl;
class CXTPGridColumnOrder;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class is a collection of grid column objects. Each
//     grid column object can be referred to by its zero-based index.
// Remarks:
//     This class provides main operations to manipulate a list of columns:
//     * Adding/removing columns
//     * Showing columns header
//     * Working with field chooser
//     * Working with "group by" field
//     * (Re)Storing column settings
//     To create a CXTPGridColumns object simply call its constructor with
//     one parameter - the pointer to the parent Grid control object.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridColumns : public CXTPCmdTarget
{
	friend class CXTPGridColumn;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridColumns object.
	// Parameters:
	//     pControl - Pointer to the parent Grid control object.
	//-----------------------------------------------------------------------
	CXTPGridColumns(CXTPGridControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridColumns object.
	// Remarks:
	//     Handles cleanup and deallocation, including all of the objects
	//     presented in the list by calling the Clear() member function.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridColumns();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the total number of column items in the collection.
	// Returns:
	//     The total number of column items in the collection.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears all column collection items.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of visible columns in the collection.
	// Remarks:
	//     Unlike the GetCount() member function, this function only returns
	//     the count of columns whose visibility property is set to TRUE.
	// Returns:
	//     The number of visible columns in the collection.
	//-----------------------------------------------------------------------
	int GetVisibleColumnsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves all visible columns in the collection and places
	//     them within a referenced CXTPGridColumns array object.
	// Parameters:
	//     arrColumns - Reference to a CXTPGridColumns array object.
	// Remarks:
	//     The referenced CXTPGridColumns array object will be filled
	//     with all visible columns in the collection.
	//-----------------------------------------------------------------------
	void GetVisibleColumns(CXTPGridColumns& arrColumns) const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Retrieves the column item at a specified index in the collection.
	// Parameters:
	//     nIndex - Index of the column to retrieve; must be greater than or equal
	//              to zero and less than the value returned by GetCount().
	// Returns:
	//     A pointer to the column item at the specified index if successful, or
	//     a NULL value if the specified index is less than zero or greater
	//     than the value returned by GetCount().
	// ---------------------------------------------------------------------
	CXTPGridColumn* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the closest visible column to a specified index in the
	//     collection that appears at or before the specified index.
	// Parameters:
	//     nIndex - Index of a column in the collection; must be greater than or equal
	//              to zero and less than the value returned by GetCount().
	// Returns:
	//     A pointer to the closest visible column to the specified index in the
	//     collection that appears at or before the specified index, or a NULL value
	//     if either a visible column is not found or the specified index is less
	//     than zero or greater than the value returned by GetCount().
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetVisibleAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the column at a specified left offset.
	// Parameters:
	//     nOffset - Left offset, in pixels, of the column to retrieve the index of.
	//     bFrozen - TRUE to count frozen columns, FALSE to not count frozen columns.
	// Returns:
	//     The index of the column at the specified left offset.
	//-----------------------------------------------------------------------
	int GetAtOffset(int nOffset, BOOL bFrozen) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the left offset, in pixels, of the column at a specified index.
	// Parameters:
	//     nIndex - Index of the column to retrieve the left offset of
	//              (i.e. this value should include frozen and non-frozen columns).
	// Returns:
	//     The left offset, in pixels, of the column at the specified index.
	//-----------------------------------------------------------------------
	int GetOffset(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indentation column of this collection.
	// Returns:
	//     A pointer to the indentation column of this collection.
	// Remarks:
	//     Indentation columns are used to represent the ends of column collections.
	//     An indentation column is usually the first column in the list.
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetIndentColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds a specified column by its item index/internal name.
	// Parameters:
	//     nItemIndex       - Item index of the column to find.
	//     strInternalName  - Internal name of the column to find.
	// Remarks:
	//     Searches the collection sequentially to find the first column
	//     matching the specified column's item index/internal name.
	// Returns:
	//     A pointer to the found column if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridColumn* Find(int nItemIndex) const;
	CXTPGridColumn* Find(const CString& strInternalName) const; //<COMBINE
																// CXTPGridColumns::Find@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified column to the end of the collection.
	// Parameters:
	//     pColumn - Pointer to the column to be added.
	// Remarks:
	//     Adds a new column to the end of a collection, increasing the collection by 1.
	//-----------------------------------------------------------------------
	void Add(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified column from the collection.
	// Parameters:
	//     pColumn - Pointer to the column to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears the sort order for all columns.
	//-----------------------------------------------------------------------
	void ResetSortOrder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated grid header object.
	// Returns:
	//     A pointer to the associated grid header object.
	//-----------------------------------------------------------------------
	CXTPGridHeader* GetGridHeader() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridHeader)
	CXTPGridHeader* GetReportHeader() const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions a specified column in the collection.
	// Parameters:
	//     nNewIndex - New index of the column.
	//     nOldIndex - Old index of the column.
	// Returns:
	//     The new index of the column if successful, otherwise a value of -1.
	//-----------------------------------------------------------------------
	int ChangeColumnOrder(int nNewIndex, int nOldIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a column.
	// Parameters:
	//     pColumn - Pointer to the column whose index must be retrieved.
	// Remarks:
	//     Searches for the first occurrence of the specified column,
	//     beginning with the first index in the collection.
	// Returns:
	//     The index of the first occurrence of the specified column if successful,
	//     otherwise a value of -1.
	//-----------------------------------------------------------------------
	int IndexOf(const CXTPGridColumn* pColumn) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified column to the sort columns array according to
	//     the columns order of the group columns array.
	// Parameters:
	//     pColumn - Pointer to the column to be inserted.
	// Returns:
	//     The index of the inserted column within the sort columns array.
	//-----------------------------------------------------------------------
	void InsertSortColumn(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a specified column as the sorted column and
	//     resets the previous sort order.
	// Parameters:
	//     pColumn     - Pointer to the column to be set.
	//     bIncreasing - TRUE if the column should be sorted in increasing order,
	//                   FALSE if the column should be sorted in decreasing order.
	//-----------------------------------------------------------------------
	void SetSortColumn(CXTPGridColumn* pColumn, BOOL bIncreasing);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the columns groups order array.
	// Returns:
	//     A pointer to the columns groups order array.
	//-----------------------------------------------------------------------
	CXTPGridColumnOrder* GetGroupsOrder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the columns sort order array.
	// Returns:
	//     A pointer to the columns sort order array.
	//-----------------------------------------------------------------------
	CXTPGridColumnOrder* GetSortOrder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes configuration to/from the provided archive stream.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the first visible column in the collection.
	// Returns:
	//     A pointer to the first visible column in the collection.
	// See Also:
	//     GetLastVisibleColumn
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetFirstVisibleColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the last visible column in the collection.
	// Returns:
	//     A pointer to the last visible column in the collection.
	// See Also:
	//     GetFirstVisibleColumn
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetLastVisibleColumn() const;

protected:
	CXTPGridColumnOrder* m_pGroupsOrder; // Container for the columns order in auto-grouping mode.
	CXTPGridColumnOrder* m_pSortOrder;   // Container for the columns sort order.
	CArray<CXTPGridColumn*, CXTPGridColumn*> m_arrColumns; // Storage for column items.
	CXTPGridControl* m_pControl;   // Pointer to the associated Grid control object.
	CXTPGridColumn* m_pTreeColumn; // Column containing tree view images.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridColumns);

	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleAdd(long nId, LPCTSTR strCaption, int nWidth, BOOL bAutoSize);
	afx_msg LPDISPATCH OleAddEx(long nId, LPCTSTR strCaption, LPCTSTR strInternalName, int nWidth,
								BOOL bAutoSize);
	afx_msg LPDISPATCH OleFindColumn(long nId);

	DECLARE_ENUM_VARIANT(CXTPGridColumns)

	enum
	{
		dispidCount		 = 1L,
		dispidAdd		 = 2L,
		dispidAddEx		 = 3L,
		dispidFindColumn = 4L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPGridColumns::GetCount() const
{
	return (int)m_arrColumns.GetSize();
}

AFX_INLINE CXTPGridColumn* CXTPGridColumns::GetAt(int nIndex) const
{
	return (nIndex >= 0 && nIndex < GetCount()) ? m_arrColumns.GetAt(nIndex) : NULL;
}

AFX_INLINE CXTPGridColumnOrder* CXTPGridColumns::GetGroupsOrder() const
{
	return m_pGroupsOrder;
}

AFX_INLINE CXTPGridColumnOrder* CXTPGridColumns::GetSortOrder() const
{
	return m_pSortOrder;
}

AFX_INLINE CXTPGridColumn* CXTPGridColumns::GetIndentColumn() const
{
	return (GetCount() > 0) ? m_arrColumns.GetAt(0) : NULL;
}

//===========================================================================
// Summary:
//     Represents the column order, used for sorting and grouping.
// Remarks:
//     This class is a collection of sorted columns objects, each of which
//     can be referred to by its zero-based index.
//     It provides the following main operations to manipulate a list of columns:
//     adding/removing columns, get at a given position, clear collection,
//     get the index of an object by it pointer.
//     You create a CXTPGridColumnOrder object simply by calling its constructor
//     with one parameter pointer to the columns collection.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridColumnOrder : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridColumnOrder object.
	// Parameters:
	//     pColumns - Parent columns collection.
	//-----------------------------------------------------------------------
	CXTPGridColumnOrder(CXTPGridColumns* pColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the column item at a specified index in the collection.
	// Parameters:
	//     nIndex - Index of the column to retrieve; must be greater than or equal
	//              to zero and less than the value returned by GetCount().
	// Returns:
	//     A pointer to the column item at the specified index if successful, or
	//     a NULL value if the specified index is less than zero or greater
	//     than the value returned by GetCount().
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified column at a specified index.
	// Parameters:
	//     nIndex  - Index to insert the column at.
	//     pColumn - Pointer to the column to be inserted.
	// Returns:
	//     The index of the inserted column if successful, otherwise a value of -1.
	//-----------------------------------------------------------------------
	int InsertAt(int nIndex, CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the column at a specified index from the collection.
	// Parameters:
	//     nIndex - Index of the column to be removed.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified column from the collection.
	// Parameters:
	//     pColumn - Pointer to the column to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified column to the end of the collection.
	// Parameters:
	//     pColumn         - Pointer to the column to be added.
	//     bSortIncreasing - TRUE if the column should be sorted in increasing order,
	//                       FALSE if the column should be sorted in decreasing order.
	// Remarks:
	//     Adds a new column to the end of a collection, increasing the collection by 1.
	// Returns:
	//     The index of the newly added column.
	//-----------------------------------------------------------------------
	int Add(CXTPGridColumn* pColumn);
	int Add(CXTPGridColumn* pColumn,
			BOOL bSortIncreasing); // <COMBINE CXTPGridColumnOrder::Add@CXTPGridColumn*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a column.
	// Parameters:
	//     pColumn - Pointer to the column whose index must be retrieved.
	// Remarks:
	//     Searches for the first occurrence of the specified column,
	//     beginning with the first index in the collection.
	// Returns:
	//     The index of the first occurrence of the specified column if successful,
	//     otherwise a value of -1.
	//-----------------------------------------------------------------------
	int IndexOf(const CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears all stored order information.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of columns in the collection.
	// Returns:
	//     The number of columns in the collection.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes column order settings to/from the provided archive stream.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CXTPGridColumns* m_pColumns; // Parent columns array.

	CArray<CXTPGridColumn*, CXTPGridColumn*> m_arrColumns; // Storage for column items with the
														   // corresponding order.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridColumnOrder);

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg void OleAdd(LPDISPATCH lpColumn);
	afx_msg int OleIndexOf(LPDISPATCH lpColumn);

	DECLARE_ENUM_VARIANT(CXTPGridColumnOrder)

	enum
	{
		dispidCount = 1L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE CXTPGridHeader* CXTPGridColumns::GetReportHeader() const
{
	return GetGridHeader();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPGRIDCOLUMNS_H__)
