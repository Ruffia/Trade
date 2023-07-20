// XTPTaskPanelGroup.h interface for the CXTPTaskPanelGroup class.
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
#if !defined(__XTPTASKPANELGROUP_H__)
#	define __XTPTASKPANELGROUP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTaskPanel;
class CXTPTaskPanelItems;
class CXTPTaskPanelItem;
class CXTPTaskPanelPaintManager;
class CXTPTaskPanelGroupItem;
class CXTPTaskPanelGroupItems;

//===========================================================================
// Summary:
//     CXTPTaskPanelGroup is used in a Task Panel control to represent a group.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroup : public CXTPTaskPanelItem
{
	DECLARE_SERIAL(CXTPTaskPanelGroup)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelGroup object.
	// Parameters:
	//     pPanel - Pointer to the parent Task Panel.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup(CXTPTaskPanel* pPanel = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelGroup object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelGroup();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of group items.
	// Returns:
	//     A pointer to the collection of group items.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems* GetItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group item at a specified index in
	//     the collection of group items.
	// Parameters:
	//     nIndex - Zero-based index of the group item to retrieve.
	// Returns:
	//     A pointer to the group item at the specified index in
	//     the collection of group items.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group item with a specified identifier.
	// Parameters:
	//     nID - Identifier of the item to retrieve.
	// Returns:
	//     A pointer to the group item with the specified identifier if successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* FindItem(int nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a link item to the group.
	// Parameters:
	//     nID    - Identifier of the link item to be added.
	//     nImage - Image index in the Task Panel image list.
	// Returns:
	//     A pointer to the newly added link item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddLinkItem(UINT nID, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a link item to the group.
	// Parameters:
	//     nID       - Identifier of the link item to be added.
	//     lpCaption - Caption of the item.
	//     lpTooltip - Tooltip of the item.
	//     nImage    - Image index in the Task Panel image list.
	// Returns:
	//     A pointer to the newly added link item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddLinkItem(UINT nID, LPCTSTR lpCaption, LPCTSTR lpTooltip = NULL,
										int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a text item to the group.
	// Parameters:
	//     strText - Text of the item.
	// Returns:
	//     A pointer to the newly added text item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddTextItem(LPCTSTR strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a control item to the group.
	// Parameters:
	//     hWnd - Window handle of the item.
	// Returns:
	//     A pointer to the newly added control item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddControlItem(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which group item, if any, is at a specified point.
	// Parameters:
	//     pt     - Point to be tested.
	//     lpRect - Pointer to a CRect object to receive the bounding rectangle of the item.
	//              Can be NULL if the bounding rectangle does not need to be retrieved.
	// Returns:
	//     A pointer to the group item at the specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* HitTest(CPoint pt, CRect* lpRect = 0) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the group is repositioned.
	// Parameters:
	//     rc          - New position of the group.
	//     bRecalcOnly - TRUE to recalculate without repositioning.
	// Returns:
	//     The target location of the group.
	// Remarks:
	//     Groups must be repositioned during animation. This method will
	//     retrieve the target location of the group.
	// Remarks:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, OnAnimate
	//-----------------------------------------------------------------------
	CRect OnReposition(CRect rc, BOOL bRecalcOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an animation step is executed.
	// Parameters:
	//     nStep - Animation step that was executed.
	// Remarks:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, OnReposition
	//-----------------------------------------------------------------------
	void OnAnimate(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the group.
	// Parameters:
	//     pDC - Pointer to the client device context.
	// Remarks:
	//     OnPaint calls CXTPTaskPanelPaintManager::DrawGroupCaption and
	//     CXTPTaskPanelPaintManager::DrawGroupClient.
	//-----------------------------------------------------------------------
	void OnPaint(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the caption.
	// Returns:
	//     The rectangle of the caption.
	//-----------------------------------------------------------------------
	CRect GetCaptionRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the client.
	// Returns:
	//     The rectangle of the client.
	//-----------------------------------------------------------------------
	CRect GetClientRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the target rectangle of the client.
	// Returns:
	//     The target rectangle of the client.
	//-----------------------------------------------------------------------
	CRect GetTargetClientRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the target rectangle of the caption.
	// Returns:
	//     The target rectangle of the caption.
	//-----------------------------------------------------------------------
	CRect GetTargetCaptionRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the group should be special.
	// Parameters:
	//     bSpecial - TRUE to set the group to special, FALSE otherwise.
	// Remarks:
	//     If a group is special, then it will be displayed in a different
	//     color than normal groups to draw the user's attention to that group.
	//     Setting a group to special is an excellent way to show the user
	//     the "default" or important items.
	//-----------------------------------------------------------------------
	void SetSpecialGroup(BOOL bSpecial = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group is special.
	// Returns:
	//     TRUE if the group is special, otherwise FALSE.
	// Remarks:
	//     If a group is special, then it will be displayed in a different
	//     color than normal groups to draw the user's attention to that group.
	//     Setting a group to special is an excellent way to show the user
	//     the "default" or important items.
	//-----------------------------------------------------------------------
	BOOL IsSpecialGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expandable state for the group.
	// Parameters:
	//     bExpandable - TRUE to set the group to expandable, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the expandable state of the group.
	// Returns:
	//     TRUE if the group is expandable, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpandable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the caption of the group.
	// Parameters:
	//     bShow - TRUE to show the caption of the group,
	//             FALSE to hide the caption of the group.
	//-----------------------------------------------------------------------
	void ShowCaption(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the caption of the group is shown/hidden.
	// Returns:
	//     TRUE if the caption of the group is shown,
	//     FALSE if the caption of the group is hidden.
	// See Also:
	//     ShowCaption
	//-----------------------------------------------------------------------
	BOOL IsCaptionVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group is expanded/collapsed.
	// Returns:
	//     TRUE if the group is expanded, FALSE if the group is collapsed.
	// See Also:
	//     SetExpanded, CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, IsExpanding
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group is expanding.
	// Returns:
	//     TRUE is group is expanding, otherwise FALSE.
	// See Also:
	//     SetExpanded, CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, IsExpanded
	//-----------------------------------------------------------------------
	BOOL IsExpanding() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the client height of the expanded group.
	// Returns:
	//     The client height of the expanded group.
	//-----------------------------------------------------------------------
	int GetExpandedClientHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands/collapses the group.
	// Parameters:
	//     bExpanded - TRUE to expand the group, FALSE to collapse the group.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, IsExpanding, IsExpanded
	//-----------------------------------------------------------------------
	void SetExpanded(BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group position is "dirty" (i.e. moving during animation).
	// Returns:
	//     TRUE if the group position is "dirty", otherwise FALSE.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, OnAnimate, m_rcGroupTarget, m_rcGroupCurrent
	//-----------------------------------------------------------------------
	BOOL IsDirty() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the outer margins of the group.
	// Returns:
	//     A reference to the outer margins of the group.
	//-----------------------------------------------------------------------
	CRect& GetOuterMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the inner margins of the group.
	// Returns:
	//     A reference to the inner margins of the group.
	//-----------------------------------------------------------------------
	CRect& GetInnerMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum client height of the group.
	// Returns:
	//     The minimum client height of the group.
	//-----------------------------------------------------------------------
	int GetMinimumClientHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum client height for the group.
	// Parameters:
	//     nMinClientHeight - Minimum client height to be set.
	//-----------------------------------------------------------------------
	void SetMinimumClientHeight(int nMinClientHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group can accept focus.
	// Returns:
	//     TRUE if the group can accept focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsAcceptFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the client bitmap for the group.
	// Parameters:
	//     nID                  - Bitmap resource identifier.
	//     clrTransparent       - Color in the bitmap to make transparent.
	//     nBackgroundAlignment - Alignment of the bitmap.
	// Remarks:
	//     The bitmap will always be in the lower right corner
	//     of the group and will not move.
	//     The bitmap is drawn in the group by OnFillClient.
	// See Also:
	//     OnFillClient
	//-----------------------------------------------------------------------
	void SetClientBitmap(UINT nID, COLORREF clrTransparent,
						 int nBackgroundAlignment = DT_RIGHT | DT_BOTTOM);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the Task Panel group client face is drawn.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - CRect object specifying the size of the area.
	// Remarks:
	//     This method is called by CXTPTaskPanelPaintManager::DrawGroupClientFace
	//     to fill the group client face.
	//
	//     This method determines if a bitmap should be drawn in the lower right
	//     corner of the group and will draw the bitmap if a bitmap was added.
	//     A bitmap can be added with SetClientBitmap.
	// See Also:
	//     SetClientBitmap
	//-----------------------------------------------------------------------
	virtual void OnFillClient(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item layout of the group.
	// Returns:
	//     The item layout of the group; one of the values
	//     defined by the XTPTaskPanelItemLayout enumeration.
	// Remarks:
	//     Item layouts include text only, image only, image and text, and
	//     image with text below the image.
	//-----------------------------------------------------------------------
	XTPTaskPanelItemLayout GetItemLayout() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item layout for the group.
	// Parameters:
	//     itemLayout - Item layout to be set; must be one of the values
	//                  defined by the XTPTaskPanelItemLayout enumeration.
	// Remarks:
	//     Item layouts include text only, image only, image and text, and
	//     image with text below the image.
	//
	//     This method only applies to this group.
	//-----------------------------------------------------------------------
	void SetItemLayout(XTPTaskPanelItemLayout itemLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects a specified group item.
	// Parameters:
	//     pItem - Pointer to the group item to be selected.
	// Remarks:
	//     Selected items will appear either "pressed" or "highlighted"
	//     depending on the selected theme.
	//
	//     Multiple items can have focus.
	//     See CXTPTaskPanel::SetSelectItemOnFocus for details.
	// See Also:
	//     XTPTaskPanelGroupItem::IsItemSelected, XTPTaskPanelGroupItem::SetItemSelected,
	//     CXTPTaskPanel::SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	void SetSelectedItem(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item currently being dragged over the group
	//     is a child item of the group.
	// Returns:
	//     TRUE if the item is a child item of the group, otherwise FALSE.
	// Remarks:
	//     This method is used in the paint manager to determine if an inverted
	//     rectangle must be drawn for the item. An inverted rectangle is only
	//     drawn when the theme of the Task Panel control is Visual Studio 2003.
	// See Also:
	//     XTPTaskPanelPaintTheme, CXTPTaskPanelToolboxTheme::DrawGroupCaption
	//-----------------------------------------------------------------------
	BOOL IsChildItemDragOver() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of items in the group.
	// Returns:
	//     The number of items in the group.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified group scroll button is enabled/disabled.
	// Parameters:
	//     bScrollButtonUp - TRUE to check if the group Up scroll button is enabled,
	//                       FALSE to check if the group Down scroll button is enabled.
	// Returns:
	//     TRUE if the specified group scroll button is enabled,
	//     FALSE if the specified group scroll button is disabled.
	// Remarks:
	//     CXTPTaskPanel::UpdateScrollButtons uses this method to
	//     update the state of the scroll buttons.
	//-----------------------------------------------------------------------
	BOOL IsScrollButtonEnabled(BOOL bScrollButtonUp) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scroll offset of the first visible item in the group.
	// Returns:
	//     The scroll offset of the first visible item in the group.
	// Remarks:
	//     The scroll offset is the height, in pixels, that has been scrolled.
	// See Also:
	//     Scroll, GetOffsetItem, GetItemCount, GetItemsInRow
	//-----------------------------------------------------------------------
	int GetScrollOffsetPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the index for the first visible item in the group.
	// Parameters:
	//     nScrollOffset        - Index to be set.
	//     bUpdateScrollButtons - TRUE to update scroll buttons, otherwise FALSE.
	//-----------------------------------------------------------------------
	void SetOffsetItem(int nScrollOffset, BOOL bUpdateScrollButtons = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified item is visible in the group.
	// Parameters:
	//     pItem      - Pointer to the group item to test.
	//     bAllowPart - TRUE to allow the item to be partially visible, FALSE otherwise.
	// Returns:
	//     TRUE if the specified item is visible in the group, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemVisible(CXTPTaskPanelGroupItem* pItem, BOOL bAllowPart) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the first visible item in the group.
	// Returns:
	//     The index of the first visible item in the group.
	// See Also:
	//     GetScrollOffsetPos, Scroll, GetItemCount, GetItemsInRow
	//-----------------------------------------------------------------------
	int GetOffsetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Scrolls to the item at a specified scroll offset in the group when
	//     the group has scroll buttons.
	// Parameters:
	//     nDelta - Offset of the item to be scrolled to.
	// Remarks:
	//     The scroll offset begins at zero and remains at zero until
	//     the view of the group is too small to display all items.
	//
	//     If the group is scrolled, then the offset will increment by one
	//     for each item that is scrolled from the top unless there is more
	//     than one item in a row. If there is more than one item in a row,
	//     then the offset will increment by GetItemsInRow instead.
	//
	//     GetItemCount and GetOffsetItem can be used to determine
	//     which item to scroll to.
	//
	//     Setting nDelta to 0 will scroll to the top of the group.
	// See Also:
	//     GetScrollOffsetPos, GetOffsetItem, GetItemCount, GetItemsInRow
	//-----------------------------------------------------------------------
	void Scroll(int nDelta);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a specified group item is visible in the Task Panel.
	// Parameters:
	//     pItem                - Pointer to the group item to ensure the visibility of.
	//     bUpdateScrollButtons - TRUE to update scroll buttons, otherwise FALSE.
	// Remarks:
	//     This method will scroll the task panel and the group that
	//     the item is within until the item is visible.
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTaskPanelGroupItem* pItem, BOOL bUpdateScrollButtons = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of group items per row in the group.
	// Returns:
	//     The number of group items per row in the group.
	// See Also:
	//     If the Task Panel behaviour is set to xtpTaskPanelBehaviourExplorer or
	//     xtpTaskPanelBehaviourList, then the number of group items per row
	//     in the group will always be one unless the Task Panel layout
	//     is set to xtpTaskItemLayoutImages.
	//
	//     If the Task Panel behavior is set to xtpTaskPanelBehaviourToolbox,
	//     then there can be multiple group items per row in the group.
	// See Also:
	//     XTPTaskPanelBehaviour, XTPTaskPanelItemLayout, Scroll
	//-----------------------------------------------------------------------
	int GetItemsInRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from/to an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the "tooltip zone".
	// Returns:
	//     The rectangle of the "tooltip zone".
	// Remarks:
	//     The "tooltip zone" is the area where the tooltip will be displayed
	//     when the mouse is positioned over the group caption.
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
	//     Gets the height of the group caption.
	// Returns:
	//     The height of the group caption.
	//-----------------------------------------------------------------------
	int GetCaptionHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of group item icons.
	// Returns:
	//     The size of group item icons.
	// See Also:
	//     SetItemIconSize
	//-----------------------------------------------------------------------
	CSize GetItemIconSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size for group item icons.
	// Parameters:
	//     szItemIcon - Size to be set.
	// See Also:
	//     GetItemIconSize
	//-----------------------------------------------------------------------
	void SetIconSize(CSize szItemIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the next/previous visible item in this group
	//     from a specified index.
	// Parameters:
	//     nIndex     - Index to begin search.
	//     nDirection - Direction to search in (+1 for next, -1 for previous).
	// Returns:
	//     A pointer to the next/previous visible item in this group
	//     from the specified index if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* GetNextVisibleItem(int nIndex, int nDirection = +1) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the height, in pixels, of the available area that
	//     can be used to draw the active group if the behaviour of
	//     the Task Panel is not set to Windows Explorer.
	// Returns:
	//     The height, in pixels, of the available area that
	//     can be used to draw the active group.
	// See Also:
	//     OnCalcClientHeight
	//-----------------------------------------------------------------------
	int CalcInsideHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to calculate the group client area
	//     required to display all items.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Bounding rectangle of the Task Panel group.
	// See Also:
	//     CalcInsideHeight
	//-----------------------------------------------------------------------
	virtual void OnCalcClientHeight(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the group is removed from
	//     the parent groups collection.
	//-----------------------------------------------------------------------
	void OnRemoved();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility Support
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual HRESULT GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
	virtual HRESULT AccessibleDoDefaultAction(VARIANT varChild);
	//}}AFX_CODEJOCK_PRIVATE

private:
	void RepositionAutoHeightControls(int nHeightOffset);
	void RepositionScrollOffset();

protected:
	CRect m_rcGroupTarget;  // Target rectangle of the group during animation.
	CRect m_rcGroupCurrent; // Current rectangle of the group during animation.

	CRect m_rcMarginsOuter; // Outer margins.
	CRect m_rcMarginsInner; // Inner margins.

	int m_nCaptionHeight; // Height of the caption.

	BOOL m_bSpecial; // TRUE if the group is special.

	BOOL m_bExpandable;			 // TRUE if the group is expandable.
	BOOL m_bExpanded;			 // TRUE if the group is expanded.
	BOOL m_bExpanding;			 // TRUE if the group is expanding.
	int m_nExpandedClientHeight; // Expanded client height.
	int m_nMinClientHeight;		 // Minimum client height.
	BOOL m_bCaptionVisible;		 // TRUE if the caption is visible.

	int m_nScrollOffset; // Scroll offset of the group. Only applies if the task panel behavior is
						 // set to xtpTaskPanelBehaviourList or xtpTaskPanelBehaviourToolbox.
	int m_nItemsInRow; // Number of group items in a row when using the xtpTaskPanelBehaviourToolbox
					   // XTPTaskPanelBehaviour. For all other behaviors, it will only be 1 unless
					   // the layout is set to to xtpTaskPanelBehaviourExplorer or
					   // xtpTaskPanelBehaviourList.
	CSize m_szItemIcon; // Size of group item icons.

	CXTPTaskPanelGroupItems* m_pItems;   // Collection of items.
	XTPTaskPanelItemLayout m_itemLayout; // Currently set layout for the group.

	int m_nBackgroundImage; // Index of the background image for the group within the image manager.
	int m_nBackgroundAlignemnt; // Alignment of the background image for the group.

private:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTaskPanelGroup)

	enum
	{

		dispidItems			 = 10L,
		dispidSpecial		 = 11L,
		dispidCaptionVisible = 12L,
		dispidExpandable	 = 13L,
		dispidExpanded		 = 14L,
	};

	afx_msg LPDISPATCH OleGetItems();
	afx_msg void OleSetIconSize(long cx, long cy);
	afx_msg void OleSetBackgroundImage(int ImageIndex, int Alignment);

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPTaskPanel;
};

AFX_INLINE CXTPTaskPanelItems* CXTPTaskPanelGroup::GetItems() const
{
	ASSERT(m_pItems);
	return (CXTPTaskPanelItems*)m_pItems;
}
AFX_INLINE int CXTPTaskPanelGroup::GetExpandedClientHeight() const
{
	return m_nExpandedClientHeight;
}
AFX_INLINE CRect& CXTPTaskPanelGroup::GetOuterMargins()
{
	return m_rcMarginsOuter;
}
AFX_INLINE CRect& CXTPTaskPanelGroup::GetInnerMargins()
{
	return m_rcMarginsInner;
}
AFX_INLINE int CXTPTaskPanelGroup::GetItemsInRow() const
{
	return m_nItemsInRow;
}
AFX_INLINE int CXTPTaskPanelGroup::GetCaptionHeight() const
{
	return m_nCaptionHeight;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTASKPANELGROUP_H__)
