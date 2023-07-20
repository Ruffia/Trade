// XTPTaskPanel.h interface for the CXTPTaskPanel class.
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
#if !defined(__XTPTASKPANEL_H__)
#	define __XTPTASKPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTaskPanelGroups;
class CXTPTaskPanelGroup;
class CXTPTaskPanelPaintManager;
class CXTPTaskPanelItems;
class CXTPTaskPanelItem;
class CXTPTaskPanelGroupItem;
class CXTPImageManager;
class CXTPToolTipContext;
class CXTPTaskPanelAnimation;
class CXTPMarkupContext;
class CXTPScrollBar;

//===========================================================================
// Summary:
//     CXTPTaskPanelScrollButton is a standalone class that represents
//     a task panel scroll button.
// Remarks:
//     The task panel uses a scroll button in the group caption to
//     expand and retract the group if the group is expandable.
//
//     The task panel also uses scroll buttons in task panel group objects
//     when there are more items in the group than can be viewed. Only
//     necessary buttons are displayed. If the items in the group cannot
//     be scrolled anymore, then that button is removed. Scroll buttons are
//     only visible when using either the xtpTaskPanelBehaviourList or
//     xtpTaskPanelBehaviourToolbox task panel behaviors. The group scroll
//     buttons are stored in m_pScrollButton.
// See Also:
//     m_pScrollButton
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelScrollButton
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelScrollButton object.
	// Parameters:
	//     bScrollUp - TRUE to construct a "scroll up" button,
	//                 FALSE to construct a "scroll down" button.
	//-----------------------------------------------------------------------
	CXTPTaskPanelScrollButton(BOOL bScrollUp);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this scroll button is visible.
	// Returns:
	//     TRUE if this scroll button is visible,
	//     FALSE if this scroll button is not visible.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

public:
	BOOL bScrollUp; // If TRUE, then the button is a "scroll up" button.
					// If FALSE, then the button is a "scroll down" button.
	BOOL bEnabled;  // TRUE if the scroll button is enabled, FALSE if it is disabled.
	BOOL bPressed;  // TRUE if the scroll button is currently pressed, FALSE otherwise.
	CRect rcButton; // Bounding rectangle of the scroll button.
					// Will be empty if the button is not visible.
	BOOL bHot; // TRUE if the scroll button is "hot", FALSE otherwise.
			   // A scroll button is hot when the mouse cursor is placed over the button.

	CXTPTaskPanelGroup* pGroupScroll;  // Pointer to the group that uses this
									   // group item scroll button.
	CXTPTaskPanelGroup* pGroupCaption; // Pointer to the group that uses this
									   // group caption scroll button.
};

//===========================================================================
// Summary:
//     CXTPTaskPanel is used to implement an Explorer and Office-like Task Panel control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanel
	: public CXTPScrollBarContainer<CWnd>
	, public CXTPAccessible
{
	DECLARE_DYNAMIC(CXTPTaskPanel)
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

private:
	class CPanelDropTarget;

public:
	//=======================================================================
	// Summary:
	//     CRepositionContext is a helper class used to
	//     lock/unlock redraw for the Task Panel.
	//=======================================================================
	class _XTP_EXT_CLASS CRepositionContext
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Constructs a CRepositionContext object.
		// Parameters:
		//     pTaskPanel - Pointer to the parent Task Panel.
		//-------------------------------------------------------------------
		CRepositionContext(CXTPTaskPanel* pTaskPanel);

		//-------------------------------------------------------------------
		// Summary:
		//     Destroys a CRepositionContext object, handles cleanup and deallocation.
		//-------------------------------------------------------------------
		~CRepositionContext();

	protected:
		CXTPTaskPanel* m_pTaskPanel; // Parent Task Panel.
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanel object.
	//-----------------------------------------------------------------------
	CXTPTaskPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanel object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a Task Panel control.
	// Parameters:
	//     dwStyle    - Style of the Task Panel control.
	//     rect       - Reference to a RECT structure specifying
	//                  the size and position of the Task Panel control.
	//     pParentWnd - Pointer to the parent window of the Task Panel control.
	//     nID        - Identifier of the Task Panel control.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of groups of this Task Panel control.
	// Returns:
	//     A pointer to the collection of groups of this Task Panel control.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems* GetGroups() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a group to the collection of groups.
	// Parameters:
	//     nID    - Identifier of the group to be added.
	//     nImage - Image index of the group to be added.
	// Returns:
	//     A pointer to the newly added group.
	// See Also:
	//     SetGroupImageList
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* AddGroup(UINT nID, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group with a specified identifier.
	// Parameters:
	//     nID - Identifier of the group to be retrieved.
	// Returns:
	//     A pointer to the group with the specified identifier if successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* FindGroup(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group item with a specified identifier.
	// Parameters:
	//     nID - Identifier of the group item to be retrieved.
	// Returns:
	//     A pointer to the group item with the specified identifier if successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* FindItem(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group at a specified index in the collection of groups.
	// Parameters:
	//     nIndex - Zero-based index of the group to retrieve.
	// Returns:
	//     A pointer to the group at the specified index.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of groups in the collection of groups.
	// Returns:
	//     The number of groups in the collection of groups.
	//-----------------------------------------------------------------------
	int GetGroupCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the scroll offset.
	// Returns:
	//     The scroll offset.
	// Remarks:
	//     This method gets the scroll offset of the scrollbar displayed to scroll
	//     through the entire task panel. To get the scroll offset of
	//     the scrollbar of a task panel group, use CXTPTaskPanelGroup::GetOffsetItem
	//     and CXTPTaskPanelGroup::GetScrollOffsetPos.
	// See Also:
	//     CXTPTaskPanelGroup::GetOffsetItem, CXTPTaskPanelGroup::GetScrollOffsetPos
	//-----------------------------------------------------------------------
	int GetScrollOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the client area.
	// Returns:
	//     The height of the client area.
	//-----------------------------------------------------------------------
	int GetClientHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions the groups in the Task Panel control.
	// Parameters:
	//     bRecalcOnly - TRUE to recalculate without repositioning.
	//-----------------------------------------------------------------------
	void Reposition(BOOL bRecalcOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the "hot" item for the Task Panel control.
	// Parameters:
	//     pItem - Item to be set.
	//-----------------------------------------------------------------------
	void SetHotItem(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the "hot" item of the Task Panel control.
	// Returns:
	//     A pointer to the "hot" item of the Task Panel control.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetHotItem() const;

	// -----------------------------------------------------------------------
	// Summary:
	//     Sets the focused item for the Task Panel control.
	// Parameters:
	//     pItem          - Pointer to the item to be set.
	//     bDrawFocusRect - TRUE to draw a focus rectangle.
	//     nSetFocus      - TRUE to set focus to the item.
	// -----------------------------------------------------------------------
	virtual void SetFocusedItem(CXTPTaskPanelItem* pItem, BOOL bDrawFocusRect = FALSE,
								BOOL bSetFocus = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a specified item is visible in the Task Panel.
	// Parameters:
	//     pItem - Pointer to the item to ensure the visibility of.
	// Remarks:
	//     This method will scroll the task panel and the group that
	//     the item is within until the item is visible.
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the focused item of the Task Panel control.
	// Returns:
	//     A pointer to the focused item of the Task Panel control.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetFocusedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a focus rectangle is drawn around an item when
	//     the Up and Down arrows are used to navigate through items in
	//     the Task Panel control.
	// Returns:
	//     TRUE if a focus rectangle is drawn, otherwise FALSE.
	// See Also:
	//     DrawFocusRect
	//-----------------------------------------------------------------------
	BOOL IsDrawFocusRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which group, if any, is located at a specified point.
	// Parameters:
	//     point     - Point to be tested.
	//     pbCaption - Pointer to a BOOL value to receive if the caption of the group
	//                 is located at the specified point;
	//                 TRUE if the caption of the group is located at
	//                 the specified point, otherwise FALSE.
	// Returns:
	//     A pointer to the group at the specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* HitTestGroup(CPoint pt, BOOL* pbCaption = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which item, if any, is located at a specified point.
	// Parameters:
	//     pt           - Point to be tested.
	//     bClientAvail - BOOL value to receive if the client area of the item
	//                    is located at the specified point;
	//                    TRUE if the client area of the item is located at
	//                    the specified point, otherwise FALSE.
	// Returns:
	//     A pointer to the item at the specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* HitTestItem(CPoint pt, BOOL bClientAvail = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the outer margins for the groups of the Task Panel control.
	// Parameters:
	//     nLeft   - Left margin.
	//     nTop    - Top margin.
	//     nRight  - Right margin.
	//     nBottom - Bottom margin.
	//     nMiddle - Margin between groups.
	// Remarks:
	//     This method sets the default margins for all groups.
	// See Also:
	//     GetMargins, CXTPTaskPanelPaintManager::GetGroupSpacing,
	//     CXTPTaskPanelPaintManager::GetGroupOuterMargins,
	//     CXTPTaskPanelPaintManager::GetGroupInnerMargins,
	//     CXTPTaskPanelPaintManager::GetControlMargins
	//-----------------------------------------------------------------------
	void SetMargins(long nLeft, long nTop, long nRight, long nBottom, long nMiddle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image list for the Task Panel control.
	// Parameters:
	//     pImageList - Pointer to the image list to be set;
	//                  can be NULL to update the size of item icons.
	//     szItemIcon - Size of item icons to be set.
	// See Also:
	//     SetGroupImageList, GetImageManager
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList, CSize szItemIcon = CSize(16, 16));

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image list for the groups of the Task Panel control.
	// Parameters:
	//     pImageList  - Pointer to the image list to be set;
	//                   can be NULL to update the size of group icons.
	//     szGroupIcon - Size of group icons to be set.
	// See Also:
	//     SetImageList, GetImageManager
	//-----------------------------------------------------------------------
	void SetGroupImageList(CImageList* pImageList, CSize szGroupIcon = CSize(32, 32));

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image manager associated with the Task Panel control.
	// Returns:
	//     A pointer to the image manager associated with the Task Panel control.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the image manager associated with the Task Panel control.
	// Parameters:
	//     pImageManager - Pointer to the image manager to be set.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expandable state for groups.
	// Parameters:
	//     bExpandable - TRUE to set groups to expandable, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the expandable state of groups.
	// Returns:
	//     TRUE if groups are expandable, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpandable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the animation type for the Task Panel control.
	// Parameters:
	//     panelAnimation - Animation type to be set.
	//                      Can be any of the values listed in the Remarks section.
	// Remarks:
	//     The panelAnimation parameter can be one of the following:
	//     * <b>xtpTaskPanelAnimationYes</b>: Enables animation (default value).
	//     * <b>xtpTaskPanelAnimationNo</b>: Disables animation.
	//     * <b>xtpTaskPanelAnimationSystem</b>: Use system settings to determine animation.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::GetAnimation,
	//     CXTPTaskPanelGroup::IsDirty, CXTPTaskPanelGroup::OnAnimate, XTPTaskPanelAnimation
	//-----------------------------------------------------------------------
	void SetAnimation(XTPTaskPanelAnimation panelAnimation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the animation type of the Task Panel control.
	// Returns:
	//     The animation type of the Task Panel control; one of the values
	//     defined by the XTPTaskPanelAnimation enumeration.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanelGroup::IsDirty, CXTPTaskPanelGroup::OnAnimate, XTPTaskPanelAnimation
	//-----------------------------------------------------------------------
	XTPTaskPanelAnimation GetAnimation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets right-to-left (RTL) mode.
	// Parameters:
	//     bRightToLeft - TRUE to set right-to-left (RTL) reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Locks/unlocks redrawing controls.
	// Parameters:
	//     bLockRedraw - TRUE to lock redrawing controls,
	//                   FALSE to unlock redrawing controls.
	// Remarks:
	//     The bLockRedraw parameter should be set to TRUE when you want
	//     to add many items, but do not want CXTPTaskPanel::Reposition and
	//     CXTPTaskPanel::ReDraw to be called each time an item is added.
	//-----------------------------------------------------------------------
	void SetLockRedraw(BOOL bLockRedraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the next/previous item in the tab order from a specified item.
	// Parameters:
	//     pItem         - Pointer to the item to search from.
	//     nDirection    - Direction to search in (+1 for next, -1 for previous).
	//     bTab          - TRUE to use tabulation, FALSE otherwise.
	//     bSkipRowItems - TRUE to skip row items (i.e. get the next/previous item
	//                     from the next/previous row), FALSE otherwise.
	// Returns:
	//     A pointer to the next/previous item in the tab order from the specified item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetNextItem(CXTPTaskPanelItem* pItem, int nDirection, BOOL bTab,
								   BOOL bSkipRowItems = TRUE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks on an item.
	// Parameters:
	//     pItem - Pointer to the item that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnClick(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group position is "dirty" (i.e. moving during animation).
	// Returns:
	//     TRUE if the group position is "dirty", otherwise FALSE.
	// See Also:
	//     AnimateGroups, SetAnimation, GetAnimation, CXTPTaskPanelGroup::OnAnimate,
	//     CXTPTaskPanelGroup::m_rcGroupTarget, CXTPTaskPanelGroup::m_rcGroupCurrent,
	//     CXTPTaskPanelGroup::IsExpanding, CXTPTaskPanelGroup::IsExpanded,
	//     CXTPTaskPanelGroup::SetExpanded
	//-----------------------------------------------------------------------
	BOOL IsDirty() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the scrollbar of the entire Task Panel control
	//     is scrolled (i.e. not the scrollbar of a Task Panel group).
	// Parameters:
	//     nCurPos - Position of the scrollbar to scroll to.
	//-----------------------------------------------------------------------
	void OnScrollChanged(int nCurPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to animate the group(s) when animation is enabled.
	// Parameters:
	//     bCheckDirty - TRUE if the group(s) are dirty
	//                   (i.e. have not reached their target location),
	//                   FALSE if the group(s) are not dirty
	//                   (i.e. have reached their target location).
	// Returns:
	//     TRUE if the groups are dirty (i.e. have not reached their target location),
	//     FALSE if the groups are not dirty (i.e. have reached their target location).
	// Remarks:
	//     This method is called when the expand/collapse button is pressed within
	//     the group caption causing the group to expand/contract.
	// See Also:
	//     SetAnimation, GetAnimation, CXTPTaskPanelGroup::OnAnimate,
	//     CXTPTaskPanelGroup::m_rcGroupTarget, CXTPTaskPanelGroup::m_rcGroupCurrent,
	//     CXTPTaskPanelGroup::IsExpanding, CXTPTaskPanelGroup::IsExpanded,
	//     CXTPTaskPanelGroup::SetExpanded
	//-----------------------------------------------------------------------
	BOOL AnimateGroups(BOOL bCheckDirty = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of item icons in the Task Panel control.
	// Remarks:
	//     By default, the size of item icons is 16x16.
	//
	//     To change the size of item icons, use SetIconSize.
	// Returns:
	//     The size of item icons in the Task Panel control.
	//-----------------------------------------------------------------------
	CSize GetItemIconSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of group icons in the Task Panel control.
	// Remarks:
	//     By default, the size of group icons is 32x32.
	//
	//     To change the size of group icons, use SetGroupIconSize.
	// Returns:
	//     The size of group icons in the Task Panel control.
	//-----------------------------------------------------------------------
	CSize GetGroupIconSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size for item icons in the Task Panel control.
	// Parameters:
	//     szIcon - Item icon size to be set.
	//-----------------------------------------------------------------------
	void SetIconSize(CSize szIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size for group icons in the Task Panel control.
	// Parameters:
	//     szIcon - Group icon size to be set.
	//-----------------------------------------------------------------------
	void SetGroupIconSize(CSize szIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables item navigation by using the Tab key.
	// Parameters:
	//     bTabItems - TRUE to enable item navigation by using the Tab key,
	//                 FALSE to disable item navigation by using the Tab key.
	//-----------------------------------------------------------------------
	void NavigateItems(BOOL bTabItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables animation for the Task Panel control.
	// Parameters:
	//     bEnable - TRUE to enable animation, FALSE to disable animation.
	//-----------------------------------------------------------------------
	void EnableAnimation(BOOL bEnable = TRUE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the theme for the Task Panel control.
	// Parameters:
	//     paintTheme - Theme to be set.
	//                  Can be any of the values listed in the Remarks section.
	// Remarks:
	//     The paintTheme parameter can be one of the following:
	//     * <b>xtpTaskPanelThemeOffice2000</b>: Enables an Office 2000 style theme.
	//     * <b>xtpTaskPanelThemeOffice2003</b>: Enables an Office 2003 style theme.
	//     * <b>xtpTaskPanelThemeNativeWinXP</b>: Enables an Explorer style theme.
	//     * <b>xtpTaskPanelThemeOffice2000Plain</b>: Enables a Classic Office 2000 style theme.
	//     * <b>xtpTaskPanelThemeOfficeXPPlain</b>: Enables a Classic Office XP style theme.
	//     * <b>xtpTaskPanelThemeOffice2003Plain</b>: Enables a Classic Office 2003 style theme.
	//     * <b>xtpTaskPanelThemeNativeWinXPPlain</b>: Enables a Classic Explorer style theme.
	//     * <b>xtpTaskPanelThemeVisualStudio2003</b>: Enables a VS 2003 style Toolbox theme.
	//     * <b>xtpTaskPanelThemeVisualStudio2005</b>: Enables a VS 2005 style Toolbox theme.
	//     * <b>xtpTaskPanelThemeListView</b>: Enables Standard a List View theme.
	//     * <b>xtpTaskPanelThemeListViewOfficeXP</b>: Enables an Office XP List View theme.
	//     * <b>xtpTaskPanelThemeListViewOffice2003</b>: Enables an Office 2003 List View theme.
	//     * <b>xtpTaskPanelThemeShortcutBarOffice2003</b>: Enables a ShortcutBar Office 2003 theme.
	//     * <b>xtpTaskPanelThemeResource</b>: Enables a ShortcutBar Office 2007/2010 theme.
	//     * <b>xtpTaskPanelThemeVisualStudio2010</b>: Enables a VS 2010 style Toolbox theme.
	//     * <b>xtpTaskPanelThemeVisualStudio2012Light</b>: Enables a VS 2012 Light style Toolbox
	//     theme.
	//     * <b>xtpTaskPanelThemeVisualStudio2012Dark</b>: Enables a VS 2012 Dark style Toolbox
	//     theme.
	//     * <b>xtpTaskPanelThemeCustom</b>: Enables a user-defined Custom theme.
	// See Also:
	//     CXTPTaskPanel::GetCurrentTheme
	//-----------------------------------------------------------------------
	void SetTheme(XTPTaskPanelPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a custom theme for the Task Panel control.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPTaskPanelPaintManager object.
	//-----------------------------------------------------------------------
	void SetCustomTheme(CXTPTaskPanelPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the paint manager.
	// Returns:
	//     A pointer to the paint manager.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the theme of the Task Panel control.
	// Returns:
	//     The theme; one of the values
	//     defined by the XTPTaskPanelPaintTheme enumeration.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//-----------------------------------------------------------------------
	XTPTaskPanelPaintTheme GetCurrentTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Expands/collapses a specified group.
	// Parameters:
	//     pGroup    - Pointer to the group to be expanded/collapsed.
	//     bExpanded - TRUE to expand the group, FALSE to collapse the group.
	// See Also:
	//     CXTPTaskPanelGroup::SetExpandable, CXTPTaskPanelGroup::IsExpandable
	//-----------------------------------------------------------------------
	virtual void ExpandGroup(CXTPTaskPanelGroup* pGroup, BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified group is expanded/collapsed.
	// Parameters:
	//     pGroup - Pointer to the group to be tested.
	// Returns:
	//     TRUE if the specified group is expanded,
	//     FALSE if the specified group is collapsed.
	//-----------------------------------------------------------------------
	BOOL IsGroupExpanded(const CXTPTaskPanelGroup* pGroup) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the hot tracking style.
	// Parameters:
	//     hotTrackStyle - Hot tracking style to be set; must be one of the values
	//                     defined by the XTPTaskPanelHotTrackStyle enumeration.
	//-----------------------------------------------------------------------
	void SetHotTrackStyle(XTPTaskPanelHotTrackStyle hotTrackStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the hot tracking style.
	// Returns:
	//     The hot tracking style; one of the values
	//     defined by the XTPTaskPanelHotTrackStyle enumeration.
	//-----------------------------------------------------------------------
	XTPTaskPanelHotTrackStyle GetHotTrackStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item layout.
	// Returns:
	//     The item layout; one of the values
	//     defined by the XTPTaskPanelItemLayout enumeration.
	//-----------------------------------------------------------------------
	XTPTaskPanelItemLayout GetItemLayout() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item layout.
	// Parameters:
	//     itemLayout - Item layout to be set; must be one of the values
	//                  defined by the XTPTaskPanelItemLayout enumeration.
	// Remarks:
	//     Item layouts include text only, image only, image and text, and
	//     image with text below the image.
	//
	//     This method applies to all groups unless the item layout for
	//     each group is set individually.
	// See Also:
	//     CXTPTaskPanelGroup::SetItemLayout
	//-----------------------------------------------------------------------
	void SetItemLayout(XTPTaskPanelItemLayout itemLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the behaviour for the Task Panel control.
	// Parameters:
	//     panelBehaviour - Behaviour to be set; must be one of the values
	//                      defined by the XTPTaskPanelBehaviour enumeration.
	// Remarks:
	//     The Task Panel can behave like a ToolBox, List View, or Explorer.
	//-----------------------------------------------------------------------
	void SetBehaviour(XTPTaskPanelBehaviour panelBehaviour);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the behaviour of the Task Panel control.
	// Returns:
	//     The behaviour of the Task Panel control; one of the values
	//     defined by the XTPTaskPanelBehaviour enumeration.
	// Remarks:
	//     The Task Panel can behave like a ToolBox, List View, or Explorer.
	//-----------------------------------------------------------------------
	XTPTaskPanelBehaviour GetBehaviour() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the active group.
	// Returns:
	//     A pointer to the active group.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* GetActiveGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if items should receive focus when they are clicked.
	// Parameters:
	//     bSelect - TRUE to set items to receive focus when they are clicked,
	//               FALSE otherwise.
	// Remarks:
	//     If bSelect is TRUE, then an item will receive focus when
	//     it is clicked and the previously focused item will lose focus.
	//
	//     If bSelect is FALSE, then an item will not receive focus when
	//     it is clicked. bSelect should be set to FALSE to either
	//     use toggle buttons or to allow multiple items to have focus.
	// See Also:
	//     SetFocusedItem, GetFocusedItem
	//-----------------------------------------------------------------------
	void SetSelectItemOnFocus(BOOL bSelect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables single selection.
	// Parameters:
	//     bSingleSelection - TRUE to enable single selection,
	//                        FALSE to disable single selection.
	// Remarks:
	//     If single selection is enabled, then only one item can be
	//     selected in the Task Panel control at a given time.
	//     If single selection is disabled, then multiple items can be
	//     selected in the Task Panel control at a given time.
	// See Also:
	//     IsSingleSelection, SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	void SetSingleSelection(BOOL bSingleSelection = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if single selection is enabled/disabled.
	// Returns:
	//     TRUE if single selection is enabled,
	//     FALSE if single selection is disabled.
	// See Also:
	//     SetSingleSelection, SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	BOOL IsSingleSelection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a focus rectangle around an item/group item when the Up and Down arrows
	//     are used to navigate the items in the Task Panel control.
	// Parameters:
	//     bAccept - TRUE to draw the focus rectangle, FALSE otherwise.
	// See Also:
	//     IsDrawFocusRect
	//-----------------------------------------------------------------------
	void DrawFocusRect(BOOL bAccept);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the outer margins of groups in the Task Panel control.
	// Returns:
	//     The outer margins of groups in the Task Panel control.
	//-----------------------------------------------------------------------
	CRect GetMargins() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the clipboard format.
	// Returns:
	//     The clipboard format.
	//-----------------------------------------------------------------------
	static CLIPFORMAT AFX_CDECL GetClipboardFormat();

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the drag options for the Task Panel.
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
	//     AllowDrop, XTPTaskPanelItemAllowDrag
	//-----------------------------------------------------------------------
	void AllowDrag(long nAllowDrag = xtpTaskItemAllowDragDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the drop options for the Task Panel.
	// Parameters:
	//     bAllowDrop - TRUE to only allow items to be dropped inside the task panel,
	//                  FALSE to only allow items to be dropped outside the task panel.
	// See Also:
	//     AllowDrag
	//-----------------------------------------------------------------------
	void AllowDrop(BOOL bAllowDrop);

	//-----------------------------------------------------------------------
	// Summary:
	//     Renames the caption of a specified item.
	// Parameters:
	//     pItem - Pointer to the item with the caption to be renamed.
	// Remarks:
	//     When this method is called, an edit box will appear on
	//     the caption allowing the user to type in a new name.
	//
	//     OnEndLabelEdit will be called when the user has finished editing
	//     the caption. This will cause NotifyOwner to send a message to
	//     the parent that the caption has finished being edited.
	// See Also:
	//     OnEndLabelEdit
	//-----------------------------------------------------------------------
	void RenameItem(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified group scroll button.
	// Parameters:
	//     bScrollUp - TRUE to retrieve the Up scroll button,
	//                 FALSE to retrieve the Down scroll button.
	// Returns:
	//     A pointer to the specified group scroll button.
	// See Also:
	//     CXTPTaskPanelScrollButton, m_pScrollButton
	//-----------------------------------------------------------------------
	CXTPTaskPanelScrollButton* GetScrollButton(BOOL bScrollUp) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which group scroll button, if any,
	//     is located at a specified point.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the group scroll button at the specified point, if any,
	//     otherwise a value of 0.
	//-----------------------------------------------------------------------
	CXTPTaskPanelScrollButton* HitTestScrollButton(CPoint point) const;

	// ----------------------------------------------------------------------
	// Summary:
	//     The framework calls this method to draw the Task Panel control
	//     using a specified device context.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     rcClipBox - Rectangular area of the control that is invalid.
	// Remarks:
	//     OnDraw calls CXTPTaskPanelGroup::OnPaint for each group in
	//     the Task Panel. OnPaint also calls CXTPTaskPanelPaintManager::FillTaskPanel
	//     and CXTPTaskPanelPaintManager::DrawScrollButton.
	// ----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcClipBox);

	//-------------------------------------------------------------------------
	// Summary:
	//     The framework calls this method to redraw the Task Panel control.
	// Parameters:
	//     lpRect   - Rectangular area of the control that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-------------------------------------------------------------------------
	void Redraw(LPCRECT lpRect = NULL, BOOL bAnimate = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the parent window that an event has occurred in the Task Panel control.
	// Parameters:
	//     wParam - Specifies which event has occurred.
	//     lParam - Additional message-specific information.
	// Remarks:
	//     wParam parameter can be one of the following values:
	//          * <b>XTP_TPN_CLICK</b> Indicates the user clicked a TaskPanel item.
	//          * <b>XTP_TPN_RCLICK</b> Indicates the user pressed the right mouse button on a
	//          TaskPanel item.
	//          * <b>XTP_TPN_STARTDRAG</b> Indicates the user started to dragging an item.
	//                                          You can return XTP_ACTION_CANCEL to cancel the drag.
	//          * <b>XTP_TPN_ENDLABELEDIT</b> Indicates the user starts to rename an item.
	//          * <b>XTP_TPN_ITEMDROP</b> Indicates the user dropped item in task panel.
	//          * <b>XTP_TPN_GROUPEXPANDING</b> Indicates the group is about to be expanded.
	//                                          <i>lParam</i> contains XTP_TPNGROUPEXPANDING
	//                                          pointer. You can return XTP_ACTION_CANCEL to ignore
	//                                          expanding.
	//          * <b>XTP_TPN_GROUPEXPANDED</b> Indicates the user has expanded a group.
	//     <p/>
	//     The following method in the task panel use NotifyOwner to send
	//     messages:
	//          * <b>CXTPTaskPanelGroup::OnAnimate</b>
	//          * <b>CXTPTaskPanel::ExpandGroup</b> lParam contains XTP_TPNGROUPEXPANDING pointer.
	//                                              You can return XTP_ACTION_CANCEL to ignore
	//                                              expanding.
	//          * <b>CXTPTaskPanel::OnDrop</b>  lParam contains item dropped.
	//          * <b>CXTPTaskPanel::OnStartItemDrag</b> lParam contains item dragged.
	//          * <b>CXTPTaskPanel::OnRButtonDown</b> lParam contains item right-clicked.
	//          * <b>CXTPTaskPanel::OnClick</b> lParam contains item clicked.
	//          * <b>CXTPTaskPanel::OnEndLabelEdit</b> lParam contains item who's caption is being
	//          renamed.
	// See Also:
	//     XTPWM_TASKPANEL_NOTIFY
	//-----------------------------------------------------------------------
	virtual LRESULT NotifyOwner(WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables multi-column groups.
	// Parameters:
	//     bMultiColumn - TRUE to enable multi-column groups,
	//                    FALSE to disable multi-column groups.
	// See Also:
	//     IsMultiColumn, SetColumnWidth
	//-----------------------------------------------------------------------
	void SetMultiColumn(BOOL bMultiColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if multi-column groups is enabled/disabled.
	// Returns:
	//     TRUE if multi-column groups is enabled,
	//     FALSE if multi-column groups is disabled.
	// See Also:
	//     SetMultiColumn, SetColumnWidth
	//-----------------------------------------------------------------------
	BOOL IsMultiColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the width, in pixels, for columns in a multi-column Task Panel control.
	// Parameters:
	//     nColumnWidth - Width, in pixels, to be set.
	// See Also:
	//     SetMultiColumn, GetColumnWidth
	//-----------------------------------------------------------------------
	void SetColumnWidth(int nColumnWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width, in pixels, of columns in a multi-column Task Panel control.
	// Returns:
	//     The width, in pixels, of columns in a multi-column Task Panel control.
	// See Also:
	//     SetMultiColumn, SetColumnWidth
	//-----------------------------------------------------------------------
	int GetColumnWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum client height of all groups.
	// Returns:
	//     The minimum client height of all groups.
	//-----------------------------------------------------------------------
	int GetMinimumGroupClientHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum client height for all groups.
	// Parameters:
	//     nMinClientHeight - Minimum client height to be set.
	//-----------------------------------------------------------------------
	void SetMinimumGroupClientHeight(int nMinClientHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Enables/disables markup for tab captions.
	// Parameters:
	//     bEnable - TRUE to enable markup for tab captions,
	//               FALSE to disable markup for tab captions.
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the markup context.
	// Returns:
	//     A pointer to the markup context.
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the behaviour of the Task Panel control is Explorer behavior.
	// Returns:
	//     TRUE if the behaviour of the Task Panel control is Explorer behavior,
	//     otherwise FALSE.
	// See Also:
	//     SetBehaviour, GetBehaviour, XTPTaskPanelBehaviour
	//-----------------------------------------------------------------------
	BOOL IsExplorerBehaviour() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a group item is starting to be dragged.
	// Parameters:
	//     pItem - Pointer to the group item starting to be dragged.
	// Remarks:
	//     This method is called by OnMouseMove if a group item is dragged.
	// See Also:
	//     OnStartGroupDrag, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnStartItemDrag(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a group is starting to be dragged.
	// Parameters:
	//     pItem - Pointer to the group starting to be dragged.
	// Remarks:
	//     This method is called by OnMouseMove if a group is dragged.
	// See Also:
	//     OnStartItemDrag, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnStartGroupDrag(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the caption of an item is finished
	//     being edited/renamed.
	// Parameters:
	//     pItem - Pointer to the item being edited.
	//     str   - New caption of the item.
	// Remarks:
	//     This method is called after RenameItem.
	// See Also:
	//     RenameItem
	//-----------------------------------------------------------------------
	virtual void OnEndLabelEdit(CXTPTaskPanelItem* pItem, LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is dragged over the Task Panel control.
	// Parameters:
	//     pDataObject - Pointer to the COleDataObject being dragged over the drop target.
	//     dwKeyState  - State of keys on the keyboard (including modifier keys).
	//                   This is a combination of any number of the following: MK_CONTROL,
	//                   MK_SHIFT, MK_ALT, MK_LBUTTON, MK_MBUTTON, and MK_RBUTTON.
	//     point       - Current mouse position relative to the Task Panel control.
	// Returns:
	//     A value from the DROPEFFECT enumerated type which indicates the type
	//     of drop that would occur if the user dropped the object at this position.
	//     The type of drop often depends on the current key state as indicated
	//     by dwKeyState. A standard mapping of key states to DROPEFFECT values is:
	//     * <b>DROPEFFECT_NONE</b>: The data object cannot be dropped in this window.
	//     * <b>DROPEFFECT_COPY</b> for <b>MK_CONTROL</b>: Creates a copy of
	//                                                     the dropped object.
	//     * <b>DROPEFFECT_MOVE</b> for <b>MK_ALT</b>: Creates a copy of the dropped
	//                                                 object and deletes the original
	//                                                 object. This is typically the
	//                                                 default drop effect when the
	//                                                 view can accept the data object.
	//-----------------------------------------------------------------------
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is dropped into the Task Panel control.
	// Parameters:
	//     pDataObject - Pointer to the COleDataObject dropped into the drop target.
	//     dropEffect  - Drop effect requested by the user.
	//                   Can be any of the values listed in the Remarks section.
	//     point       - Current mouse position relative to the Task Panel control.
	// Remarks:
	//     The <i>dropEffect</i> parameter can be one of the following values:
	//     * <b>DROPEFFECT_COPY</b>: Creates a copy of the data object being dropped.
	//     * <b>DROPEFFECT_MOVE</b>: Moves the data object to the current mouse location.
	// Returns:
	//     TRUE if the drop was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	//-------------------------------------------------------------------------
	// Summary:
	//     Repositions the group scroll buttons.
	// Remarks:
	//     This method is called by Reposition and AnimateGroups when
	//     the groups in the task panel must be repositioned
	//     (i.e. during animation while the group is expanding or collapsing).
	//-------------------------------------------------------------------------
	void RepositionScrollButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     The member is called when a group scroll button is clicked.
	// Parameters:
	//     pScrollButton - Pointer to the group scroll button that was clicked.
	// Remarks:
	//     This method is called by OnLButtonDown when a group scroll button is clicked.
	// See Also:
	//     CXTPTaskPanelScrollButton, m_pScrollButton
	//-----------------------------------------------------------------------
	void OnClickScrollButton(CXTPTaskPanelScrollButton* pScrollButton);

	//-------------------------------------------------------------------------
	// Summary:
	//     Updates the enabled/disabled state of the group scroll buttons.
	// Remarks:
	//     This method is called by RepositionScrollButtons,
	//     CXTPTaskPanelGroup::Scroll, and CXTPTaskPanelGroup::EnsureVisible
	//     to update the state of the group scroll buttons.
	//-------------------------------------------------------------------------
	void UpdateScrollButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the mouse cursor is positioned over a group scroll button.
	// Parameters:
	//     point - Mouse cursor position.
	// Remarks:
	//     If the mouse cursor is positioned over a group scroll button
	//     and that group scroll button is enabled, then the appearance of
	//     the button will be changed to "hot". When the mouse cursor leaves
	//     the button, the appearance of the button will be returned to normal.
	//
	//     This method is called by OnMouseMove and OnMouseLeave to update
	//     the appearance of the group scroll buttons.
	//-----------------------------------------------------------------------
	void CheckScrollButtonMouseOver(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is removed from the Task Panel.
	// Parameters:
	//     pItem - Pointer to the item that was removed.
	// Remarks:
	//     This method is called by CXTPTaskPanelItems::Clear
	//     and CXTPTaskPanelItems::RemoveAt.
	// See Also:
	//     CXTPTaskPanelItems::Clear, CXTPTaskPanelItems::RemoveAt
	//-----------------------------------------------------------------------
	virtual void OnItemRemoved(CXTPTaskPanelItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     Updates the scrollbar of the Task Panel.
	// Remarks:
	//     This method updates the scrollbar displayed to scroll
	//     through the entire task panel. To update the scrollbar of
	//     a task panel group, use UpdateScrollButtons.
	// See Also:
	//     UpdateScrollButtons
	//-------------------------------------------------------------------------
	virtual void UpdateScrollBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the client height is changed.
	//-------------------------------------------------------------------------
	virtual void OnClientHeightChanged();

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the next/previous visible group from a specified index.
	// Parameters:
	//     nIndex     - Index to begin search.
	//     nDirection - Direction to search in (+1 for next, -1 for previous).
	// Returns:
	//     A pointer to the next/previous group from the specified index
	//     if successful, otherwise a value of -1.
	//-------------------------------------------------------------------------
	CXTPTaskPanelGroup* GetNextVisibleGroup(int nIndex, int nDirection = +1) const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility support.
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual CCmdTarget* GetAccessible();
	CXTPTaskPanelItem* GetAccessibleItem(int nIndex);
	int GetAccessibleIndex(CXTPTaskPanelItem* pItem);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTaskPanel)
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTaskPanel)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseLeave();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL GetMessageExt(MSG& msg);
	void EnsureVisibleRect(const CRect& rc);
	void GrabFocus();
	HWND GetNextControl(BOOL bForward) const;
	void CheckOldFocus(CWnd* pOldWnd);
	CXTPTaskPanelGroupItem* FindControlItem(HWND hWnd);
	HWND GetRootChild(HWND hWndTaskPanel, HWND hWnd);
	void DrawBorders(CPaintDC& paintDC);

protected:
	CXTPTaskPanelGroups* m_pGroups;		  // Collection of groups.
	CXTPTaskPanelAnimation* m_pAnimation; // Animation context

	CXTPTaskPanelPaintManager* m_pPaintManager; // Current Paint Manager.
	XTPTaskPanelPaintTheme m_paintTheme;		// Current theme.
	BOOL m_bPreSubclassWindow;					// TRUE when initialized from PreSubclassWindow.
	BOOL m_nLockRedraw;							// TRUE when control is locked.

	int m_nClientHeight;	// Group client height. This is the area displayed under
							// the group caption.
	int m_nOldClientHeight; // Old group client height.

	CRect m_rcMargins;   // Outer margins of groups.
	int m_nGroupSpacing; // Amount of space placed between groups.

	BOOL m_bScrollVisible; // TRUE if the scroll bar is visible. This refers to
						   // the scroll bar of the entire task panel.

	CXTPTaskPanelItem* m_pItemHot;	 // Currently hot item.
	CXTPTaskPanelItem* m_pItemFocused; // Currently focused item.
	CXTPTaskPanelItem* m_pItemPressed; // Currently pressed item.

	CXTPTaskPanelItem* m_pItemDragging; // Pointer to item currently being dragged.
	CXTPTaskPanelItem* m_pItemDragOver; // Pointer to item under mouse cursor with dragged icon.
										// This is the item that will appear directly under the
										// dragged item if the item were dropped.

	BOOL m_bAnimation;					// TRUE if animation enabled.
	XTPTaskPanelAnimation m_eAnimation; // Current animation state.

	HCURSOR m_hHandCursor; // Hand cursor.

	UINT_PTR m_nAnimateTimer; // Timer identifier for animation.
	int m_nAnimationStep;	 // Current step of animation.

	CXTPImageManager* m_pImageManager; // Image manager of the Task Panel. This is
									   // the collection of images that are used for
									   // Task Panel group and group item icons.

	BOOL m_bExpandable; // TRUE if groups are expandable.

	BOOL m_bAcceptFocus;   // TRUE if control accept focus.
	BOOL m_bGrabbingFocus; // TRUE if control move focus to itself.
	BOOL m_bDrawFocusRect; // TRUE to draw focused rectangle drawn around the item when the Up and
						   // Down arrows are used to navigate the items in the Task Panel.
	CSize m_szItemIcon;	// Size of group item icons.
	CSize m_szGroupIcon;   // Size of icons displayed in group captions.
	BOOL m_bSelectItemOnFocus; // TRUE if items receive focus when they are clicked.

	CPoint m_ptPressed; // Point on Task Panel that was clicked.

	long m_nAllowDrag; // TRUE to allow Task Panel group items to be dragged.
	BOOL m_bAllowDrop; // TRUE to allow Task Panel group items to be dropped inside the Task Panel,
					   // FALSE to only allow Task Panel group items to be dropped outside the Task
					   // Panel.

	XTPTaskPanelItemLayout m_itemLayout; // Currently set item layout.
	BOOL m_bMultiColumn; // TRUE is multiple columns are used to display items in a group.
	int m_nColumnWidth;  // Width of task panel columns when m_bMultiColumn is TRUE.
	XTPTaskPanelHotTrackStyle m_hotTrackStyle; // Currently set hot tracking style.
	XTPTaskPanelBehaviour m_panelBehaviour;	// Currently set Task Panel behavior.

	CXTPTaskPanelGroup* m_pActiveGroup; // Pointer to the currently active group.
	CPanelDropTarget* m_pDropTarget;	// Internal drag'n'drop helper.
	static CLIPFORMAT m_cfItem;			// Clipboard format.

	BOOL m_bTimerGroupHover; // TRUE if the timer for the Hover event is currently set. The timer
							 // for the Hover event is set when the mouse is left in the same
							 // position for at least 500 milliseconds.

	CXTPTaskPanelScrollButton*
		m_pScrollButton[2]; // Scroll buttons of the task panel. The task panel uses these scroll
							// buttons in CXTPTaskPanelGroup objects when there are more items in
							// the group than can be viewed. Only the necessary buttons are
							// displayed. If the items in the group cannot be scrolled anymore, then
							// that button is removed. Scroll buttons are only visible when using
							// the xtpTaskPanelBehaviourList and xtpTaskPanelBehaviourToolbox task
							// panel behaviors.
	BOOL m_bSingleSelection;			   // TRUE to use focused item as selected.
	CXTPToolTipContext* m_pToolTipContext; // Tooltip context.
	BOOL m_bInUpdateScrollBar;   // TRUE if the CXTPTaskPanel::UpdateScrollBar() method is currently
								 // in use (Scrollbar is currently being updated).
	int m_nGroupMinClientHeight; // Group minimum client height.
	long m_nOLEDropMode;		 // TRUE if ActiveX OLEDropMode used.
	BOOL m_bTabItems;			 // TRUE to tab all items in group.
	CXTPMarkupContext* m_pMarkupContext; // Markup context of the Task Panel.

	COLORREF m_clrBorder[3];

private:
	friend class CXTPTaskPanelGroup;
	friend class CXTPTaskPanelItem;
	friend class CTaskPanelCtrl;
	friend class CXTPTaskPanelPaintManager;
	friend class CPanelDropTarget;
	friend class CXTPTaskPanelGroupItem;
	friend class CXTPTaskPanelEditItem;
	friend class CXTPTaskPanelItems;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPTaskPanelItems* CXTPTaskPanel::GetGroups() const
{
	return (CXTPTaskPanelItems*)m_pGroups;
}
AFX_INLINE CXTPTaskPanelPaintManager* CXTPTaskPanel::GetPaintManager() const
{
	return m_pPaintManager;
}
AFX_INLINE CXTPTaskPanelItem* CXTPTaskPanel::GetHotItem() const
{
	return m_pItemHot;
}
AFX_INLINE XTPTaskPanelPaintTheme CXTPTaskPanel::GetCurrentTheme() const
{
	return m_paintTheme;
}
AFX_INLINE void CXTPTaskPanel::SetExpandable(BOOL bExpandable)
{
	m_bExpandable = bExpandable;
}
AFX_INLINE XTPTaskPanelAnimation CXTPTaskPanel::GetAnimation() const
{
	return m_eAnimation;
}
AFX_INLINE BOOL CXTPTaskPanel::IsExpandable() const
{
	return m_bExpandable;
}
AFX_INLINE CSize CXTPTaskPanel::GetItemIconSize() const
{
	return m_szItemIcon;
}
AFX_INLINE CSize CXTPTaskPanel::GetGroupIconSize() const
{
	return m_szGroupIcon;
}
AFX_INLINE BOOL CXTPTaskPanel::IsExplorerBehaviour() const
{
	return m_panelBehaviour == xtpTaskPanelBehaviourExplorer;
}
AFX_INLINE CXTPTaskPanelGroup* CXTPTaskPanel::GetActiveGroup() const
{
	return m_pActiveGroup;
}
AFX_INLINE void CXTPTaskPanel::SetSelectItemOnFocus(BOOL bSelect)
{
	m_bSelectItemOnFocus = bSelect;
}
AFX_INLINE void CXTPTaskPanel::DrawFocusRect(BOOL bAccept)
{
	m_bAcceptFocus = bAccept;
}
AFX_INLINE CRect CXTPTaskPanel::GetMargins() const
{
	return m_rcMargins;
}
AFX_INLINE void CXTPTaskPanel::SetIconSize(CSize szIcon)
{
	m_szItemIcon = szIcon;
	Reposition();
}
AFX_INLINE void CXTPTaskPanel::SetGroupIconSize(CSize szIcon)
{
	m_szGroupIcon = szIcon;
	Reposition();
}
AFX_INLINE CLIPFORMAT AFX_CDECL CXTPTaskPanel::GetClipboardFormat()
{
	return m_cfItem;
}
AFX_INLINE void CXTPTaskPanel::AllowDrop(BOOL bAllowDrop)
{
	m_bAllowDrop = bAllowDrop;
}
AFX_INLINE CXTPTaskPanelScrollButton* CXTPTaskPanel::GetScrollButton(BOOL bScrollUp) const
{
	return bScrollUp ? m_pScrollButton[TRUE] : m_pScrollButton[FALSE];
}
AFX_INLINE BOOL CXTPTaskPanel::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
									  const RECT& rect, CWnd* pParentWnd, UINT nID,
									  CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTPTaskPanel::SetSingleSelection(BOOL bSingleSelection)
{
	m_bSingleSelection = bSingleSelection;
	Reposition();
}
AFX_INLINE BOOL CXTPTaskPanel::IsSingleSelection() const
{
	return m_bSingleSelection;
}
AFX_INLINE void CXTPTaskPanel::SetMultiColumn(BOOL bMultiColumn)
{
	m_bMultiColumn = bMultiColumn;
	Reposition();
}
AFX_INLINE BOOL CXTPTaskPanel::IsMultiColumn() const
{
	return m_bMultiColumn;
}
AFX_INLINE void CXTPTaskPanel::SetColumnWidth(int nColumnWidth)
{
	m_nColumnWidth = nColumnWidth;
	Reposition();
}
AFX_INLINE int CXTPTaskPanel::GetColumnWidth() const
{
	return m_nColumnWidth;
}
AFX_INLINE int CXTPTaskPanel::GetMinimumGroupClientHeight() const
{
	return m_nGroupMinClientHeight;
}

AFX_INLINE void CXTPTaskPanel::SetMinimumGroupClientHeight(int nMinClientHeight)
{
	m_nGroupMinClientHeight = nMinClientHeight;
}
AFX_INLINE void CXTPTaskPanel::NavigateItems(BOOL bTabItems)
{
	m_bTabItems = bTabItems;
}
AFX_INLINE CXTPMarkupContext* CXTPTaskPanel::GetMarkupContext() const
{
	return m_pMarkupContext;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTASKPANEL_H__)
