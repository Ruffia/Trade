// XTPGridRecord.h: interface for the CXTPGridRecord class.
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
#if !defined(__XTPGRIDRECORD_H__)
#	define __XTPGRIDRECORD_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRecordItem;
class CXTPGridRecordItemPreview;
class CXTPGridColumn;
class CXTPGridRecords;
class CXTPPropExchange;
class CXTPMarkupContext;

struct XTP_GRIDRECORDITEM_DRAWARGS;
struct XTP_GRIDRECORDITEM_METRICS;

//===========================================================================
// Summary:
//     CXTPGridRecord represents a collection of CXTPGridRecordItem objects
//     or objects of classes that are children of CXTPGridRecordItem.
//     A collection of such items is one record in a GridControl list.
//     You create an object of CXTPGridRecord by simply calling its constructor.
//     After calling its constructor, you can add items to the record.
// Example:
// <code>
// // add new record to the record list
// CXTPGridRecords* pList = new CXTPGridRecords();
// pList->Add(new CXTPGridRecord());
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecord : public CXTPHeapObjectT<CXTPCmdTarget, CXTPGridDataAllocator>
{
	friend class CXTPGridRecords;
	friend class CXTPGridRow;
	friend class CXTPGridControl;
	friend class CXTPGridRecordItem;
	friend class CXTPGridPaintManager;
	DECLARE_SERIAL(CXTPGridRecord)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecord object.
	//-----------------------------------------------------------------------
	CXTPGridRecord();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridRecord object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridRecord();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of items inside the record.
	// Returns:
	//     The number of items inside the record.
	// See Also:
	//     GetItem
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a record item by its index/column.
	// Parameters:
	//     nIndex  - Zero-based index of the record item in the collection.
	//     pColumn - Pointer to the column of the record item.
	// Remarks:
	//     You use this member function to get a record item from the collection by
	//     its index. If the provided index is less than 0 or greater than the
	//     value returned by GetItemCount, then GetItem will return NULL.
	// Returns:
	//     A pointer to the record item if successful, otherwise NULL.
	// See Also:
	//     GetItemCount
	//-----------------------------------------------------------------------
	CXTPGridRecordItem* GetItem(int nIndex) const;
	CXTPGridRecordItem*
		GetItem(CXTPGridColumn* pColumn) const; // <COMBINE CXTPGridRecord::GetItem@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated preview item of the record.
	// Remarks:
	//     Preview items are stored separately from all other items. A record
	//     can have only one preview item. If a GridRecord does not have
	//     a preview item, then GetItemPreview() will return NULL.
	// Returns:
	//     The associated preview item if successful, otherwise NULL.
	// See Also:
	//     SetPreviewItem
	//-----------------------------------------------------------------------
	CXTPGridRecordItemPreview* GetItemPreview() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated preview item for the record.
	// Parameters:
	//     pItemPreview - Pointer to the preview item to set.
	// Remarks:
	//     Preview items are stored separately from all other items. A record
	//     can have only one preview item.
	// See Also:
	//     GetItemPreview
	//-----------------------------------------------------------------------
	void SetPreviewItem(CXTPGridRecordItemPreview* pItemPreview);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the visible state of the record.
	// Returns:
	//     TRUE if the record is visible, FALSE if the record is hidden.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state for the record.
	// Parameters:
	//     bVisible - TRUE to set the record to visible,
	//                FALSE to set the record to hidden.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the locked state of the record.
	// Returns:
	//     TRUE if the record is locked, FALSE if the record is unlocked.
	// See Also:
	//     SetLocked
	//-----------------------------------------------------------------------
	BOOL IsLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the locked state for the record.
	// Parameters:
	//     bLocked - TRUE to set the record to locked,
	//               FALSE to set the record to unlocked.
	// Remarks:
	//     You should set the record to locked when the record
	//     should not be sorted on hide.
	// See Also:
	//     IsLocked
	//-----------------------------------------------------------------------
	void SetLocked(BOOL bLocked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the editable state of the record.
	// Returns:
	//     TRUE if the record is editable, otherwise FALSE.
	// See Also:
	//     SetEditable
	//-----------------------------------------------------------------------
	BOOL IsEditable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the editable state for the record.
	// Parameters:
	//     bEditable - TRUE to set the record to editable, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetEditable(BOOL bEditable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the list of child items of the record.
	// Remarks:
	//     A grid record can have a list of child items.
	//     This method is used to get a pointer to that list.
	// Returns:
	//     A pointer to the list of child items of the record.
	//-----------------------------------------------------------------------
	CXTPGridRecords* GetChilds();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the list of parent items of the record.
	// Returns:
	//     A pointer to the list of parent items of the record.
	//-----------------------------------------------------------------------
	CXTPGridRecords* GetRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the record has child records.
	// Returns:
	//     TRUE if the record has child records, otherwise FALSE.
	// See Also:
	//     GetChilds
	//-----------------------------------------------------------------------
	BOOL HasChildren() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items from the record.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of a specified record item object.
	// Parameters:
	//     pItem - Pointer to the record item whose index must be retrieved.
	// Returns:
	//     The zero-based index of the specified record item if successful,
	//     otherwise a value of -1.
	//-----------------------------------------------------------------------
	int IndexOf(const CXTPGridRecordItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates item metrics based on specified draw arguments.
	//     Can be overridden by descendants.
	// Parameters:
	//     pDrawArgs    - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                    structure used to calculate item metrics.
	//     pItemMetrics - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                    structure to fill with values.
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								XTP_GRIDRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates DataSet in VirtualMode without involving _RecordsetPtr.
	// Remarks:
	//     Can be overridden by descendants.
	// Parameters:
	//     row   - Row.
	//     index - Index.
	//     sText - Value as a string.
	// Returns:
	//     TRUE if successful.
	//-----------------------------------------------------------------------
	virtual BOOL UpdateRecordField(int row, int index, CString sText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the expanded state of the record.
	// Remarks:
	//     If the expanded state is set to TRUE, then the control will
	//     attempt to draw the record with all of its children.
	// Returns:
	//     TRUE if the record is expanded, otherwise FALSE.
	// See Also:
	//     SetExpanded
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expanded state for the record.
	// Parameters:
	//     bExpanded - TRUE to set the record state to expanded.
	// Remarks:
	//     If the expanded state is set to TRUE, then the control will
	//     attempt to draw the record with all of its children.
	// See Also:
	//     IsExpanded
	//-----------------------------------------------------------------------
	virtual void SetExpanded(BOOL bExpanded);

	BOOL IsSelected() const;

	void SetSelected(BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the record is filtered.
	// Returns:
	//     TRUE if the record is filtered, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsFiltered() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the record index in the array.
	// Returns:
	//     The record index in the array.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a record item to the internal records item collection.
	// Parameters:
	//     pItem - Pointer to the record item to add.
	// Returns:
	//     A pointer to the newly added record item.
	//-----------------------------------------------------------------------
	CXTPGridRecordItem* AddItem(CXTPGridRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record with record
	//     items using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the record.
	//-----------------------------------------------------------------------
	void Delete();

	//-----------------------------------------------------------------------
	// Summary:
	//     Increments the reference counter of a record and all of its children.
	//-----------------------------------------------------------------------
	void TreeAddRef();

	//-----------------------------------------------------------------------
	// Summary:
	//     Decrements the reference counter of a record and all of its children.
	//-----------------------------------------------------------------------
	void TreeRelease();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bookmark for the data source record.
	// Parameters:
	//     vtBookmark - Bookmark to be set.
	//-----------------------------------------------------------------------
	void SetBookmark(VARIANT vtBookmark);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bookmark to the data source record.
	// Returns:
	//     The bookmark to the data source record.
	//-----------------------------------------------------------------------
	VARIANT GetBookmark() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the markup context.
	// Returns:
	//     A pointer to the markup context.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent record.
	// Returns:
	//     A pointer to the parent record.
	//-----------------------------------------------------------------------
	CXTPGridRecord* GetParentRecord() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of rows in FreeHeight RC mode.
	// Returns:
	//     The height of rows in FreeHeight RC mode.
	//-----------------------------------------------------------------------
	int GetFreeHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height for rows in FreeHeight RC mode.
	// Parameters:
	//     nFreeHeight - Height to be set.
	//-----------------------------------------------------------------------
	void SetFreeHeight(int nFreeHeight);

protected:
	void SetGridControl(CXTPGridControl* pGridControl);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridControl)
	void SetReportControl(CXTPGridControl* pGridControl);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CArray<CXTPGridRecordItem*, CXTPGridRecordItem*> m_arrItems; // An internal storage for
																 // record items.

	CXTPGridRecordItemPreview* m_pPreviewItem; // Preview item for record.

	CXTPGridControl* m_pControl;  // Pointer to the parent Grid control.
	CXTPGridRecords* m_pRecords;  // Parent records collection.
	CXTPGridRecords* m_pChildren; // A collection of children records.

	BOOL m_bExpanded; // TRUE if the record is expanded, FALSE otherwise.
	BOOL m_bEditable; // TRUE if the record is editable, FALSE otherwise.
	BOOL m_bVisible;  // TRUE if the record is visible, FALSE otherwise.
	BOOL m_bLocked;   // TRUE if the record is locked, FALSE otherwise.
	BOOL m_bSelected; // TRUE if the record is selected, FALSE otherwise.

	int m_nFreeHeight;	// Height of a visual row in FreeHeight RC mode.
	int m_nIndex;		  // Index of the record within the collection records.
	VARIANT m_vtBookmark; // Bookmark to data source record for data binding.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecord);
	DECLARE_OLECREATE_EX(CXTPGridRecord);

	int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg LPDISPATCH OleAdd(const VARIANT& lpValue);
	afx_msg LPDISPATCH OleGetChilds();
	afx_msg LPDISPATCH OleGetParentRecord();
	afx_msg LPDISPATCH OleGetRecords();
	afx_msg void OleAddEx(LPDISPATCH lpDisp);

	DECLARE_ENUM_VARIANT(CXTPGridRecord)

	afx_msg BSTR OleGetPreviewText();
	afx_msg void OleSetPreviewText(LPCTSTR lpszText);
	afx_msg LPDISPATCH OleGetPreviewItem();
	afx_msg int OleGetFreeHeight();
	afx_msg void OleSetFreeHeight(int nFreeHeight);

	enum
	{
		dispidCount		 = 1L,
		dispidAdd		 = 2L,
		dispidFindColumn = 4L,
	};

	COleVariant m_oleTag;

public:
	static CXTPGridRecord* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/
};

AFX_INLINE int CXTPGridRecord::GetItemCount() const
{
	return (int)m_arrItems.GetSize();
}

AFX_INLINE CXTPGridRecordItem* CXTPGridRecord::GetItem(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetItemCount() ? m_arrItems.GetAt(nIndex) : NULL;
}

AFX_INLINE BOOL CXTPGridRecord::IsVisible() const
{
	return m_bVisible;
}

AFX_INLINE void CXTPGridRecord::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

AFX_INLINE BOOL CXTPGridRecord::IsEditable() const
{
	return m_bEditable;
}

AFX_INLINE BOOL CXTPGridRecord::IsExpanded() const
{
	return m_bExpanded;
}

AFX_INLINE BOOL CXTPGridRecord::IsLocked() const
{
	return m_bLocked;
}

AFX_INLINE void CXTPGridRecord::SetLocked(BOOL bLocked)
{
	m_bLocked = bLocked;
}

AFX_INLINE BOOL CXTPGridRecord::IsSelected() const
{
	return m_bSelected;
}

AFX_INLINE void CXTPGridRecord::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
}

AFX_INLINE int CXTPGridRecord::GetFreeHeight()
{
	return m_nFreeHeight;
}

AFX_INLINE void CXTPGridRecord::SetFreeHeight(int nFreeHeight)
{
	m_nFreeHeight = nFreeHeight;
}

AFX_INLINE void CXTPGridRecord::GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
											   XTP_GRIDRECORDITEM_METRICS* pItemMetrics)
{
	UNREFERENCED_PARAMETER(pDrawArgs);
	UNREFERENCED_PARAMETER(pItemMetrics);
}

AFX_INLINE BOOL CXTPGridRecord::UpdateRecordField(int /*row*/, int /*index*/, CString /*sText*/)
{
	return TRUE;
}
AFX_INLINE CXTPGridRecords* CXTPGridRecord::GetRecords() const
{
	return m_pRecords;
}
AFX_INLINE void CXTPGridRecord::SetBookmark(VARIANT vtBookmark)
{
	m_vtBookmark = vtBookmark;
}
AFX_INLINE VARIANT CXTPGridRecord::GetBookmark() const
{
	return m_vtBookmark;
}

AFX_INLINE void CXTPGridRecord::SetReportControl(CXTPGridControl* pGridControl)
{
	SetGridControl(pGridControl);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORD_H__)
