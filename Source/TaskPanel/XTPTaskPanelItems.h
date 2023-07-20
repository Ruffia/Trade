// XTPTaskPanelItems.h interface for the CXTPTaskPanelItems class.
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
#if !defined(__XTPTASKPANELITEMS_H__)
#	define __XTPTASKPANELITEMS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTaskPanelGroup;
class CXTPTaskPanelItem;
class CXTPTaskPanel;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPTaskPanelItems is used in a Task Panel control
//     to represent a collection of items.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelItems : public CXTPCmdTarget
{
	DECLARE_DYNCREATE(CXTPTaskPanelItems)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelItems object.
	// Parameters:
	//     pPanel - Pointer to the parent Task Panel.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems(CXTPTaskPanel* pPanel = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelItems object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelItems();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds an item to this collection.
	// Parameters:
	//     pItem - Pointer to the item to add.
	//     nID   - Identifier of the item.
	// Returns:
	//     A pointer to the newly added item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* Add(CXTPTaskPanelItem* pItem, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds an item to this collection.
	// Parameters:
	//     pItem     - Pointer to the item to add.
	//     nID       - Identifier of the item.
	//     lpCaption - Caption of the item.
	//     lpTooltip - Tooltip of the item.
	// Returns:
	//     A pointer to the newly added item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* Add(CXTPTaskPanelItem* pItem, UINT nID, LPCTSTR lpCaption,
						   LPCTSTR lpTooltip = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts an item at a specified index in this collection.
	// Parameters:
	//     nIndex - Index to insert the item at.
	//     pItem  - Pointer to the item to insert.
	// Remarks:
	//     To move an item, use the Move method.
	// Returns:
	//     A pointer to the newly inserted item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* InsertAt(int nIndex, CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item at a specified index in this collection.
	// Parameters:
	//     nIndex - Zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to the item at the specified index in this collection.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items from this collection.
	// Parameters:
	//     bReposition - TRUE to reposition items after the removal
	//                   is complete, FALSE otherwise.
	// See Also:
	//     CXTPTaskPanel::Reposition
	//-----------------------------------------------------------------------
	void Clear(BOOL bReposition = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item with a specified identifier.
	// Parameters:
	//     nID - Identifier of the item to be retrieved.
	// Returns:
	//     A pointer to the item with the specified identifier if successful,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* Find(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified item from this collection.
	// Parameters:
	//     pItem - Pointer to the item to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the item at a specified index from this collection.
	// Parameters:
	//     nIndex - Zero-based index of the item to be removed.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified item in this collection.
	// Parameters:
	//     pItem - Pointer to the item with the index to be retrieved.
	// Returns:
	//     The index of the specified item in this collection.
	//-----------------------------------------------------------------------
	int GetIndex(const CXTPTaskPanelItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of items in this collection.
	// Returns:
	//     The number of items in this collection.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the owner.
	// Returns:
	//     A pointer to the owner.
	//-----------------------------------------------------------------------
	CCmdTarget* GetOwner() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Moved a specified item to a specified index in this collection.
	// Parameters:
	//     pItem  - Pointer to the item to be moved.
	//     nIndex - Index to move the specified item to.
	// Remarks:
	//     Items are stored in a zero based array and are displayed in
	//     the order that they are added to the array. This method allows
	//     for the order of items in the array to be changed.
	//-----------------------------------------------------------------------
	void Move(CXTPTaskPanelItem* pItem, int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from or/an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default runtime class of items in this collection.
	// Returns:
	//     A pointer to the default runtime class of items in this collection.
	//-----------------------------------------------------------------------
	virtual CRuntimeClass* GetDefaultItemClass() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the next/previous visible item in this collection
	//     from a specified index.
	// Parameters:
	//     nIndex     - Index to begin search.
	//     nDirection - Direction to search in (+1 for next, -1 for previous).
	// Returns:
	//     The index of the next/previous visible item in this collection
	//     from the specified index if successful, otherwise a value of -1.
	// See Also:
	//     GetFirstVisibleItem, GetLastVisibleItem
	//-----------------------------------------------------------------------
	int GetNextVisibleIndex(int nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the first visible item in this collection.
	// Returns:
	//     A pointer to the first visible item in this collection.
	// See Also:
	//     GetLastVisibleItem, GetNextVisibleIndex
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetFirstVisibleItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the last visible item in this collection.
	// Returns:
	//     A pointer to the last visible item in this collection.
	// See Also:
	//     GetFirstVisibleItem, GetNextVisibleIndex
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetLastVisibleItem() const;

protected:
	CXTPTaskPanel* m_pPanel;								   // Parent Task Panel control.
	CArray<CXTPTaskPanelItem*, CXTPTaskPanelItem*> m_arrItems; // Array of items.
	CCmdTarget* m_pOwner; // Owner of items. Can be Task Panel or Group.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleItem(long nIndex);

	afx_msg void OleClear();
	afx_msg void OleRemove(long nIndex);
	afx_msg LPDISPATCH OleFind(long nID);
	afx_msg void OleMove(LPDISPATCH lpItem, long nIndex);
	afx_msg LPDISPATCH OleGetItem(long nIndex);

	DECLARE_ENUM_VARIANT(CXTPTaskPanelItems)

	DECLARE_OLETYPELIB_EX(CXTPTaskPanelItems)

	enum
	{
		dispidCount  = 1L,
		dispidRemove = 2L,
		dispidClear  = 3L,
		dispidAdd	= 4L,
		dispidFind   = 5L,
	};

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPTaskPanel;
	friend class CXTPTaskPanelGroup;
};

//===========================================================================
// Summary:
//     CXTPTaskPanelGroupItems represents a collection of items in a group.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroupItems : public CXTPTaskPanelItems
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPTaskPanelGroupItems object.
	// Parameters:
	//     pPanel - Pointer to the parent Task Panel.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItems(CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default runtime class of items in this collection.
	// Returns:
	//     A pointer to the default runtime class of items in this collection.
	//-----------------------------------------------------------------------
	virtual CRuntimeClass* GetDefaultItemClass() const;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTaskPanelGroupItems)

	afx_msg LPDISPATCH OleAdd(long nID, LPCTSTR lpCaption, long nType,
							  const VARIANT& varImageIndex);
	afx_msg LPDISPATCH OleInsert(long nIndex, long nID, LPCTSTR lpCaption, long nType,
								 const VARIANT& varImageIndex);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     CXTPTaskPanelGroups represents a collection of groups in a Task Panel control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroups : public CXTPTaskPanelItems
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelGroups object.
	// Parameters:
	//     pPanel - Pointer to the parent Task Panel.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroups(CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default runtime class of items in this collection.
	// Returns:
	//     A pointer to the default runtime class of items in this collection.
	//-----------------------------------------------------------------------
	virtual CRuntimeClass* GetDefaultItemClass() const;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTaskPanelGroups)

	afx_msg LPDISPATCH OleAdd(long nID, LPCTSTR lpCaption);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPTaskPanelItems::GetCount() const
{
	return (int)m_arrItems.GetSize();
}
AFX_INLINE CXTPTaskPanelItem* CXTPTaskPanelItems::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrItems.GetSize() ? m_arrItems.GetAt(nIndex) : NULL;
}
AFX_INLINE CCmdTarget* CXTPTaskPanelItems::GetOwner() const
{
	return m_pOwner;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTASKPANELITEMS_H__)
