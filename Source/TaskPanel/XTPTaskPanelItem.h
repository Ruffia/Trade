// XTPTaskPanelItem.h interface for the CXTPTaskPanelItem class.
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
#if !defined(__XTPTASKPANELITEM_H__)
#	define __XTPTASKPANELITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTaskPanel;
class CXTPTaskPanelPaintManager;
class CXTPTaskPanelItems;
class CXTPTaskPanelGroup;
class CXTPImageManagerIcon;
class CXTPPropExchange;
class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//     CXTPTaskPanelItem is used in a Task Panel control
//     to represent a single item.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelItem
	: public CXTPCmdTarget
	, public CXTPAccessible
{
	DECLARE_INTERFACE_MAP()
	DECLARE_DYNCREATE(CXTPTaskPanelItem)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelItem object
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelItem object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent Task Panel control.
	// Returns:
	//     A pointer to the parent Task Panel control.
	//-----------------------------------------------------------------------
	CXTPTaskPanel* GetTaskPanel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the identifier for the item.
	// Parameters:
	//     nID - Indentifier to be set.
	//-----------------------------------------------------------------------
	void SetID(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the identifier for the item.
	// Parameters:
	//     nID       - Identifier to be set.
	//     lpCaption - Caption to be set.
	//     lpTooltip - Tooltip to be set.
	//-----------------------------------------------------------------------
	void SetID(UINT nID, LPCTSTR lpCaption, LPCTSTR lpTooltip = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the item.
	// Returns:
	//     The identifier of the item.
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the identifier of the item.
	// Returns:
	//     Identifier of the item.
	//-----------------------------------------------------------------------
	BOOL IsDroppedOff() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the item.
	// Parameters:
	//     lpszCaption - Caption to be set.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text for the item.
	// Parameters:
	//     lpszTooltip - Tooltip text to be set.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the type of the item.
	// Returns:
	//     The type of the item; one of the values defined by
	//     the XTPTaskPanelItemType enumeration.
	//-----------------------------------------------------------------------
	XTPTaskPanelItemType GetType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the type for the item.
	// Parameters:
	//     typeItem - Type to be set; must be one of the values defined by
	//                the the XTPTaskPanelItemType enumeration.
	//-----------------------------------------------------------------------
	void SetType(XTPTaskPanelItemType typeItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the item.
	// Returns:
	//     The caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the item.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	// Returns:
	//     The tooltip text of the item.
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is enabled/disabled.
	// Returns:
	//     TRUE if the item is enabled, FALSE if the item is disabled.
	//-----------------------------------------------------------------------
	BOOL GetEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the item.
	// Parameters:
	//     bEnabled - TRUE to enable the item, FALSE to disable the item.
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager.
	// Returns:
	//     A pointer to the paint manager.
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is hot.
	// Returns:
	//     TRUE if the item is hot, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemHot() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is pressed.
	// Returns:
	//     TRUE if the item is pressed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is focused.
	// Returns:
	//     TRUE if the item is focused, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is being dragged.
	// Returns:
	//     TRUE if the item is being dragged, otherwise FALSE.
	// See Also:
	//     IsItemDragOver
	//-----------------------------------------------------------------------
	BOOL IsItemDragging() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if another item is being dragged over this item.
	// Returns:
	//     TRUE if another item is being dragged over this item, otherwise FALSE.
	// See Also:
	//     IsItemDragging
	//-----------------------------------------------------------------------
	BOOL IsItemDragOver() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is a group.
	// Returns:
	//     TRUE if the item is a group, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the parent Task Panel control.
	// Parameters:
	//     bAnimate - TRUE to animate changes.
	//-----------------------------------------------------------------------
	void RedrawPanel(BOOL bAnimate = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon index for the item.
	// Parameters:
	//     nIndex - Icon index to be set.
	// Remarks:
	//     The icon index refers to the index of the icon within
	//     the image list of the parent Task Panel control.
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the icon index of the item.
	// Remarks:
	//     The icon index refers to the index of the icon within
	//     the image list of the parent Task Panel control.
	// Returns:
	//     The icon index of the item.
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item can be focused.
	// Returns:
	//     TRUE if the item can be focused, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsAcceptFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the image of the item.
	// Parameters:
	//     nWidth - Width of the icon to be retrieved
	//              (e.g. pass in a value of 16 to retrieve a 16x16 image).
	// Remarks:
	//     CXTPImageManagerIcon can have multiple image sizes for the same item.
	// Returns:
	//     A pointer to the image of the item.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage(int nWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Restores a specified COleDataSource object.
	// Parameters:
	//     pDataObject - Pointer to the COleDataSource object to restore.
	// Returns:
	//     A pointer to the item that was stored within
	//     the COleDataSource object.
	// Remarks:
	//     This method will restore the item that was cached using
	//     the PrepareDrag method.
	// See Also:
	//     PrepareDrag, CacheGlobalData, CopyToClipboard,
	//     PasteFromClipboard
	//-----------------------------------------------------------------------
	static CXTPTaskPanelItem* AFX_CDECL CreateFromOleData(COleDataObject* pDataObject);

	//-----------------------------------------------------------------------
	// Summary:
	//     Restores the COleDataSource object contained within a specified CFile object.
	// Parameters:
	//     pDataObject - Pointer to the CFile object containing
	//                   the COleDataSource object to restore.
	// Returns:
	//     A pointer to the item that was stored within
	//     the COleDataSource object.
	// Remarks:
	//     This method will restore the item that was cached using
	//     the PrepareDrag method.
	// See Also:
	//     PrepareDrag, CacheGlobalData, CopyToClipboard,
	//     PasteFromClipboard
	//-----------------------------------------------------------------------
	static CXTPTaskPanelItem* AFX_CDECL CreateFromOleFile(CFile* pDataObject);

	//-----------------------------------------------------------------------
	// Summary:
	//     Stores information to HGLOBAL about the group item being dragged.
	// Returns:
	//     An HGLOBAL object containing information about the group item
	//     being dragged if successful, NULL if an OLE or Archive exception
	//     was thrown while caching the data.
	// See Also:
	//     PrepareDrag, CreateFromOleData, CopyToClipboard, PasteFromClipboard
	//-----------------------------------------------------------------------
	HGLOBAL CacheGlobalData();

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes this item from the array of items.
	//-------------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is shown/hidden.
	// Returns:
	//     TRUE if the item is shown, FALSE if the item is hidden.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the item.
	// Parameters:
	//     bVisible - TRUE to show the item, FALSE to hide the item.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Repositions the groups in the Task Panel control.
	// Parameters:
	//     bRecalcOnly - TRUE to recalculate without repositioning.
	//-------------------------------------------------------------------------
	void RepositionPanel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent collection of items that this item belongs to.
	// Returns:
	//     A pointer to the parent collection of items that this item belongs to.
	// Remarks:
	//     This method allows you to access sibling groups and/or items and
	//     determine the total number of siblings.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems* GetParentItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group that this item belongs to.
	// Returns:
	//     A pointer to the group that this item belongs to.
	//-----------------------------------------------------------------------
	virtual CXTPTaskPanelGroup* GetItemGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of this item within the parent collection of items.
	// Returns:
	//     The index of this item within the parent collection of items.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if dropping is enabled/disabled for this item.
	// Returns:
	//     TRUE if dropping is enabled for this item,
	//     FALSE if dropping is disabled for this item.
	//-----------------------------------------------------------------------
	BOOL IsAllowDrop() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables dropping for this item.
	// Parameters:
	//     bAllowDrop - TRUE to enable dropping for this item,
	//                  FALSE to disable dropping for this item.
	//-----------------------------------------------------------------------
	void AllowDrop(BOOL bAllowDrop);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the drag options for the item.
	// Parameters:
	//     nAllowDrag - Drag options. See the Remarks section
	//                  below for a list of available values.
	// Remarks:
	//     The nAllowDrag parameter can be one or more of the following values:
	//     * <b>xtpTaskItemAllowDragCopyWithinGroup</b>: To only allow the copy operation
	//                                                   within the group.
	//     * <b>xtpTaskItemAllowDragCopyWithinControl</b>: To only allow the copy operation
	//                                                     within the task panel.
	//     * <b>xtpTaskItemAllowDragCopyOutsideControl</b>: To only allow the copy operation
	//                                                      outside the task panel.
	//     * <b>xtpTaskItemAllowDragCopy</b>: To allow the copy operation.
	//     * <b>xtpTaskItemAllowDragMoveWithinGroup</b>: To only allow the move operation
	//                                                   within the group.
	//     * <b>xtpTaskItemAllowDragMoveWithinControl</b>: To only allow the move operation
	//                                                     within the task panel.
	//     * <b>xtpTaskItemAllowDragMoveOutsideControl</b>: To only allow the move operation
	//                                                      outside the task panel.
	//     * <b>xtpTaskItemAllowDragMove</b>: To allow the move operation.
	//     * <b>xtpTaskItemAllowDragAll</b>: To allow all drag operations.
	// See Also:
	//     IsAllowDrag, XTPTaskPanelItemAllowDrag
	//-----------------------------------------------------------------------
	void AllowDrag(long nAllowDrag = xtpTaskItemAllowDragDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this item can be dragged.
	// Returns:
	//     TRUE if this item can be dragged, otherwise FALSE.
	// See Also:
	//     AllowDrag, XTPTaskPanelItemAllowDrag
	//-----------------------------------------------------------------------
	long IsAllowDrag() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item font
	//     (i.e. the font used to display task panel item text).
	// Returns:
	//     A pointer to the item font.
	//-----------------------------------------------------------------------
	CFont* GetItemFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item font
	//     (i.e. the font used to display task panel item text).
	// Parameters:
	//     pLogFont - Pointer to the item font to be set.
	// Remarks:
	//     The item font can be a custom font or the standard item font.
	//-----------------------------------------------------------------------
	void SetItemFont(LOGFONT* pLogFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bounding rectangle for the caption text of the item.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     rc      - Bounding rectangle to be set.
	//     nFormat - Format of the caption text
	//               (i.e. DT_LEFT, DT_CENTER, DT_RIGHT, DT_VCENTER).
	// Remarks:
	//     This method is called by XTPTaskPanelPaintManager::DrawItemCaption
	//     to set the bounding rectangle for the caption text of the item.
	// See Also:
	//     CXTPTaskPanelPaintManager::DrawItemCaption
	//-----------------------------------------------------------------------
	void SetTextRect(CDC* pDC, CRect rc, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the caption text of the item.
	// Parameters:
	//     nFormat - Format of the caption text
	//               (i.e. DT_LEFT, DT_CENTER, DT_RIGHT, DT_VCENTER).
	// Returns:
	//     The bounding rectangle of the caption text of the item.
	//-----------------------------------------------------------------------
	CRect GetTextRect(UINT* pFormat = 0) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the "tooltip zone".
	// Returns:
	//     The base class implementation returns CRect(0, 0, 0, 0).
	//
	//     The CXTPTaskPanelGroupItem implementation returns
	//     the rectangle of the "tooltip zone".
	// Remarks:
	//     The "tooltip zone" is the area where the tooltip will be displayed
	//     when the mouse is positioned over the group item.
	//
	//     The member is overridden in CXTPTaskPanelGroupItem.
	// See Also:
	//     CXTPTaskPanelGroupItem::GetHitTestRect
	//-----------------------------------------------------------------------
	virtual CRect GetHitTestRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the "drawing zone".
	// Returns:
	//     The rectangle of the "drawing zone".
	// See Also:
	//     CXTPTaskPanelGroupItem::GetHitTestRect
	//-----------------------------------------------------------------------
	virtual CRect GetInvalidateRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the application-supplied, 32-bit value associated with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - 32-bit value to associate with the item.
	//-----------------------------------------------------------------------
	void SetItemData(DWORD_PTR dwData);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon padding of the item.
	// Returns:
	//     A reference to the icon padding of the item.
	//-----------------------------------------------------------------------
	CRect& GetIconPadding();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Markup element that renders the caption of the item.
	// Returns:
	//     A pointer to the Markup element that renders the caption of the item.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to calculate boundary rectangle to fit caption
	// Parameters:
	//     pDC     - Pointer to valid device context
	//     rc      - Boundary rectangle
	//     nFormat - Format flags to draw
	// Returns:
	//     Size of caption
	//-----------------------------------------------------------------------
	virtual CSize GetCaptionTextSize(CDC* pDC, CRect rc, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw caption text
	// Parameters:
	//     pDC     - Pointer to valid device context
	//     rc      - Boundary rectangle to draw
	//     nFormat - Format flags to draw
	//-----------------------------------------------------------------------
	virtual void DrawCaptionText(CDC* pDC, CRect rc, UINT nFormat);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from/to an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-------------------------------------------------------------------------
	// Summary:
	//     Copies this item to the clipboard.
	// See Also:
	//     PasteFromClipboard, CreateFromOleData, CacheGlobalData,
	//     PasteFromClipboard, PrepareDrag
	//-------------------------------------------------------------------------
	void CopyToClipboard();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the last item placed in the clipboard.
	// Returns:
	//     A pointer to the last item placed in the clipboard.
	// See Also:
	//     CopyToClipboard, CreateFromOleData, CacheGlobalData,
	//     CopyToClipboard, PrepareDrag
	//-----------------------------------------------------------------------
	static CXTPTaskPanelItem* AFX_CDECL PasteFromClipboard();

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that the item is visible in the Task Panel.
	// Remarks:
	//     This method will scroll the task panel and the group that
	//     the item is within until the item is visible.
	//-----------------------------------------------------------------------
	void EnsureVisible();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is removed from
	//     the parent items collection.
	//-----------------------------------------------------------------------
	virtual void OnRemoved();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is added to
	//     the parent items collection.
	//-----------------------------------------------------------------------
	virtual void OnInserted();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility Support
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleDescription(VARIANT varChild, BSTR* pszDescription);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual CCmdTarget* GetAccessible();
	virtual HRESULT AccessibleSelect(long flagsSelect, VARIANT varChild);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPTaskPanel* m_pPanel; // Parent Task Panel.
	CString m_strCaption;	// Caption of the item.
	CString m_strTooltip;	// Tooltip of the item.
	UINT m_nID;				 // Identifier of the item.

	XTPTaskPanelItemType m_typeItem; // Type of the item.

	CXTPTaskPanelItems* m_pItems; // Collection of child items.

	int m_nIconIndex;  // Icon index.
	BOOL m_bEnabled;   // TRUE if the item is enabled.
	BOOL m_bAllowDrop; // TRUE if the item can be dragged.
	long m_nAllowDrag; // TRUE if the item can be dropped.

	CRect m_rcText;		   // Size and location of the caption text of the item.
	UINT m_nTextFormat;	// Format of the caption text of the item.
	BOOL m_bVisible;	   // TRUE if the item is visible.
	CRect m_rcIconPadding; // Padding/spacing placed around the item icon.

	DWORD_PTR m_dwData; // The 32-bit value associated with the item.

	CXTPFont m_xtpFontItem; // Font of the item.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntItem, m_xtpFontItem, GetItemFontHandle);

	CXTPMarkupUIElement* m_pMarkupUIElement; // Markup element that renders caption.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTaskPanelItem)

	enum
	{
		dispidCaption   = 1L,
		dispidTooltip   = 2L,
		dispidEnabled   = 3L,
		dispidType		= 4L,
		dispidId		= 5L,
		dispidIconIndex = 6L,
	};
	afx_msg BSTR OleGetCaption();
	afx_msg BSTR OleGetTooltip();
	afx_msg LPDISPATCH OleGetGroup();
	afx_msg LPDISPATCH OleGetParentItems();
	int OleGetIndex();
	COleVariant m_oleTag;
	void OleSetIconPadding(long nLeft, long nTop, long nRight, long nBottom);
	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);
	void SetID_(UINT nID);

public:
	static CXTPTaskPanelItem* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/

	friend class CXTPTaskPanelItems;
	friend class CXTPTaskPanelPaintManager;
};

AFX_INLINE CXTPTaskPanel* CXTPTaskPanelItem::GetTaskPanel() const
{
	ASSERT(m_pPanel != NULL);
	return m_pPanel;
}
AFX_INLINE CString CXTPTaskPanelItem::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE CString CXTPTaskPanelItem::GetTooltip() const
{
	return m_strTooltip;
}
AFX_INLINE CXTPTaskPanelItems* CXTPTaskPanelItem::GetParentItems() const
{
	return m_pItems;
}
AFX_INLINE CRect& CXTPTaskPanelItem::GetIconPadding()
{
	return m_rcIconPadding;
}
AFX_INLINE void CXTPTaskPanelItem::OnRemoved()
{
}
AFX_INLINE DWORD_PTR CXTPTaskPanelItem::GetItemData() const
{
	return m_dwData;
}
AFX_INLINE void CXTPTaskPanelItem::SetItemData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}
AFX_INLINE CXTPMarkupUIElement* CXTPTaskPanelItem::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}
AFX_INLINE BOOL CXTPTaskPanelItem::IsDroppedOff() const
{
	return m_pPanel == NULL ? TRUE : FALSE;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTASKPANELITEM_H__)
