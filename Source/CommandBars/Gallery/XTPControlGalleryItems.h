// XTPControlGalleryItems.h
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
#if !defined(__XTPCONTROLGALLERYITEMS_H__)
#	define __XTPCONTROLGALLERYITEMS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPControlGalleryItems is a CXTPCmdTarget derived class. It represents
//     a collection of items for gallery control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryItems : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPControlGalleryItems)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryItems object.
	// Parameters:
	//     pAction - Action for gallery control to link with.
	// Remarks:
	//     Protected constructor, use CreateItems to create items for gallery.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItems(CXTPControlAction* pAction, CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlGalleryItems object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPControlGalleryItems();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates new items collection for gallery control.
	// Parameters:
	//     pCommandBars - Parent Commandbars.
	//     nId - Identifier of the items
	//-----------------------------------------------------------------------
	static CXTPControlGalleryItems* AFX_CDECL CreateItems(CXTPCommandBars* pCommandBars, int nId);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items in the collection.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove an item.
	// Parameters:
	//     nIndex - Index of the item to be removed.
	//-----------------------------------------------------------------------
	void Remove(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     nId   - Identifier of the item.
	//     nImage - Image of the item.
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(int nId, int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     nId   - Identifier of the item.
	//     nImage - Image of the item.
	//     lpszCaption - Caption of the item to be added.
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(LPCTSTR lpszCaption, int nId, int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     pItem - Item to be added.
	//     nId   - Identifier of the item.
	//     nImage - Image of the item.
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(CXTPControlGalleryItem* pItem, int nId = -1, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     nImage - Image of the item.
	//     lpszCaption - Caption of the item to be added.
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(LPCTSTR lpszCaption, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert a new item.
	// Parameters:
	//     nIndex - Index to insert new item.
	//     pItem - Item to be added.
	//     nId   - Identifier of the item.
	//     nImage - Image of the item.
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* InsertItem(int nIndex, CXTPControlGalleryItem* pItem, int nId = -1,
									   int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new label item.
	// Parameters:
	//     nId   - Identifier of the item.
	//     lpszCaption - Caption of the item to be added.
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddLabel(int nId);
	CXTPControlGalleryItem* AddLabel(LPCTSTR lpszCaption); //<combine
														   // CXTPControlGalleryItems::AddLabel@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the image manager for the items.
	// Parameters:
	//     pImageManager - New image manager to be set.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the image manager for the items.
	// Returns:
	//     The image manager for the items.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the default size of the items.
	// Returns:
	//     The default size of the items.
	// See Also:
	//     SetItemSize
	//-----------------------------------------------------------------------
	CSize GetItemSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the size of the items.
	// Parameters:
	//     szItem - New size to be set.
	// See Also:
	//     GetItemSize
	//-----------------------------------------------------------------------
	void SetItemSize(CSize szItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the count of the items.
	// Returns:
	//     The count of the items in the collection.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the item at the specified index.
	// Parameters:
	//     nIndex - An integer index.
	// Returns:
	//     The CXTPControlGalleryItem pointer currently at this index.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable/disable clipping items if selection was changed
	// Parameters:
	//     bClipItems - TRUE to enable clipping, FALSE to disable clipping.
	// Remarks:
	//     By default clipping is enabled.
	//-----------------------------------------------------------------------
	void ClipItems(BOOL bClipItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called if items are added to or removed from the collection.
	//-----------------------------------------------------------------------
	virtual void OnItemsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pGallery  - Pointer to the parent gallery of the item.
	//     pItem     - Pointer to the item to be drawn.
	//     rcItem    - Bounding rectangle of the item.
	//     bEnabled  - TRUE to draw the item enabled, FALSE to draw the item disabled.
	//     bSelected - TRUE to draw the item selected.
	//     bPressed  - TRUE to draw the item pressed.
	//     bChecked  - TRUE to draw the item checked.
	//-----------------------------------------------------------------------
	virtual BOOL PreDrawItem(CDC* pDC, CXTPControlGallery* pGallery, CXTPControlGalleryItem* pItem,
							 CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed,
							 BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the actions list for command bars.
	// Returns:
	//     A pointer to the actions list for command bars.
	//-----------------------------------------------------------------------
	CXTPControlAction* GetAction() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows the use of a XAML snippet to be used for the text of items.
	// Parameters:
	//     bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the markup context.
	// Returns:
	//     A pointer to a CXTPMarkupContext object.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

private:
	void UpdateIndexes(int nStart /*= 0*/);

protected:
	CSize m_szItem;														 // Size of the items
	CXTPImageManager* m_pImageManager;									 // Image manager of items
	BOOL m_bClipItems;													 // TRUE to clip items
	CXTPControlAction* m_pAction;										 // Action of the items
	CArray<CXTPControlGalleryItem*, CXTPControlGalleryItem*> m_arrItems; // Array of items
	CXTPMarkupContext* m_pMarkupContext;
	CXTPCommandBars* m_pCommandBars;

private:
	friend class CXTPControlGalleryItem;
	friend class CXTPControlGallery;
	friend class CXTPControlGalleryAction;

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlGalleryItems);
	DECLARE_ENUM_VARIANT(CXTPControlGalleryItems);
	DECLARE_CONNECTION_EX(CXTPControlGalleryItems)

	afx_msg LPDISPATCH OleAddItem(int nId, LPCTSTR lpszCaption);
	afx_msg LPDISPATCH OleAddLabel(LPCTSTR lpszCaption);
	afx_msg LPDISPATCH OleAddSeparator();
	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg BOOL OleGetEnableMarkup();
	afx_msg void OleSetEnableMarkup(BOOL bEnableMarkup);
	afx_msg LPDISPATCH OleGetMarkupContext();

	DECLARE_PROPERTY(Icons, LPDISPATCH);
	DECLARE_PROPERTY(ItemWidth, int);
	DECLARE_PROPERTY(ItemHeight, int);

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE CXTPImageManager* CXTPControlGalleryItems::GetImageManager() const
{
	return m_pImageManager;
}
AFX_INLINE CSize CXTPControlGalleryItems::GetItemSize() const
{
	return m_szItem;
}
AFX_INLINE void CXTPControlGalleryItems::ClipItems(BOOL bClipItems)
{
	m_bClipItems = bClipItems;
}
AFX_INLINE CXTPControlAction* CXTPControlGalleryItems::GetAction() const
{
	return m_pAction;
}
AFX_INLINE CXTPMarkupContext* CXTPControlGalleryItems::GetMarkupContext() const
{
	return m_pMarkupContext;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPCONTROLGALLERYITEMS_H__
//}}AFX_CODEJOCK_PRIVATE
