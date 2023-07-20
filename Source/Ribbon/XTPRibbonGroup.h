// XTPRibbonGroup.h: interface for the CXTPRibbonGroup class.
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
#if !defined(__XTPRIBBONGROUP_H__)
#	define __XTPRIBBONGROUP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonGroups;
class CXTPRibbonBar;
class CXTPRibbonTab;

class CXTPControl;
class CXTPControlPopup;

enum XTPRibbonGroupReductionLevels
{
	xtpRibbonGroupReductionDisabled   = 1,   // Do not reduce.
	xtpRibbonGroupFullCaptionDontHide = 2,   // Do not hide captions and do not allow hiding at all
											 // when reduced.
	xtpRibbonGroupReduceCaptionDontHide = 3, // Hide captions, but do not allow hiding at all when
											 // reduced.
	xtpRibbonGroupFullReduction = 4,		 // Hide captions and allow hiding when reduced.
};

//===========================================================================
// Summary:
//     Represents a single group that is added to a CXTPRibbonTab.
// Remarks:
//     Each CXTPRibbonTab can contain one or more CXTPRibbonGroup objects
//     that are used to hold CXTPControl objects. The caption of
//     a CXTPRibbonGroup is a title bar that visually "groups" the CXTPControl
//     objects that have been added to the RibbonGroup Items Collection
//     (CXTPRibbonGroup::GetAt).
//
//     Groups are added to the CXTPRibbonTab Groups Collection using the
//     CXTPRibbonGroups::AddGroup and CXTPRibbonGroups::InsertAt methods. Items
//     are added to a group using the CXTPRibbonGroup::Add method.
// See Also:
//     CXTPRibbonGroup::GetAt
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonGroup : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPRibbonGroup)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonGroup object.
	//-----------------------------------------------------------------------
	CXTPRibbonGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonGroup object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonGroup();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text to display in the group title bar.
	// Parameters:
	//     lpszCaption - Text to display in the group title bar.
	// Remarks:
	//     The caption is the text string that is displayed in the group's
	//     title bar. The caption of the group is initially set when it is
	//     added or inserted.
	// See Also:
	//     GetCaption, CXTPRibbonGroups::AddGroup, CXTPRibbonGroups::InsertAt
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text displayed in the group title bar.
	// Remarks:
	//     The caption is the text string that is displayed in the group's
	//     title bar. The caption of the group is initially set when it is
	//     added or inserted.
	// Returns:
	//     The text displayed in the group title bar.
	// See Also:
	//     SetCaption, CXTPRibbonGroups::AddGroup, CXTPRibbonGroups::InsertAt
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the parent command bar.
	// Returns:
	//     A pointer to the parent command bar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetParent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bounding rectangle for the group.
	// Parameters:
	//     rc - CRect object containing the bounding rectangle for the group.
	// Remarks:
	//     The rectangle is the size of the entire group, including the
	//     group caption and group pop-up if present.
	// See Also:
	//     GetRect
	//-----------------------------------------------------------------------
	virtual void SetRect(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the group.
	// Remarks:
	//     The rectangle is the size of the entire group, including the
	//     group caption and group pop-up if present.
	// Returns:
	//     A CRect containing the bounding rectangle of the group.
	// See Also:
	//     SetRect
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the group caption.
	// Remarks:
	//     The rectangle is the size of the group caption.
	// Returns:
	//     A CRect containing the bounding rectangle of the group caption.
	// See Also:
	//     SetRect
	//-----------------------------------------------------------------------
	CRect GetCaptionRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonBar object that the CXTPRibbonGroup belongs to.
	// Returns:
	//     A pointer to the CXTPRibbonBar object that the CXTPRibbonGroup belongs to.
	//     This is the ribbon bar that the group is displayed in.
	// See Also:
	//     CXTPRibbonBar
	//-----------------------------------------------------------------------
	CXTPRibbonBar* GetRibbonBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonTab object that the CXTPRibbonGroup belongs to.
	// Returns:
	//     A pointer to the CXTPRibbonTab object that the CXTPRibbonGroup belongs to.
	//     This is the tab that the group is displayed in.
	// See Also:
	//     CXTPRibbonTab
	//-----------------------------------------------------------------------
	CXTPRibbonTab* GetParentTab() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets whether the group is currently highlighted.
	// Remarks:
	//     A group becomes highlighted when the mouse cursor is positioned
	//     over the group or any items in the group.
	// Returns:
	//     TRUE if the group is currently highlighted, otherwise FALSE.
	// See Also:
	//     CXTPRibbonBar::GetHighlightedGroup
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified CXTPControl object in the CXTPRibbonGroup.
	// Parameters:
	//     pControl - Control whose index must be retrieved.
	// Remarks:
	//     This member function is used to get the zero-based index of items
	//     in the group. If the item is not found, it returns a value of -1.
	// Returns:
	//     The index of the specified control if successful, otherwise -1.
	//-----------------------------------------------------------------------
	int IndexOf(CXTPControl* pControl) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total number of CXTPControl objects that have been added
	//     to the CXTPRibbonGroup.
	// Returns:
	//     The total number of CXTPControl objects that have been added
	//     to the CXTPRibbonGroup.
	// Remarks:
	//     A CXTPRibbonGroup is used to hold one or more CXTPControl objects.
	//     The total number of items can be found by using GetCount.
	// See Also:
	//     CXTPRibbonGroup
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a CXTPControl to the ribbon group.
	// Parameters:
	//     controlType   - Type of control to add to the ribbon group.
	//     nId           - Identifier of the CXTPControl to be added.
	//     lpszParameter - The caption of the CXTPControl to be added.
	//     nBefore       - Index of an existing control that the new control
	//                     will be inserted before.
	//     bTemporary    - Boolean value indicating whether the control is
	//                     temporary. Temporary controls will not be saved to the
	//                     system registry when the application is closed, and
	//                     they will not be restored when the application is opened.
	//     pControl      - Pointer to a CXTPControl to add to the ribbon group.
	// Remarks:
	//     The Add method allows CXTPControl objects to be added to the ribbon group.
	// Returns:
	//     A pointer to the CXTPControl object that was added to the ribbon group.
	// See Also:
	//     CXTPControl, GetAt, GetCount, RemoveAt, RemoveAll
	//-----------------------------------------------------------------------
	CXTPControl* Add(XTPControlType controlType, int nId, LPCTSTR lpszParameter = NULL,
					 int nBefore = -1, BOOL bTemporary = FALSE);
	CXTPControl*
		Add(CXTPControl* pControl, int nId, LPCTSTR lpszParameter = NULL, int nBefore = -1,
			BOOL bTemporary = FALSE); // <combine
									  // CXTPRibbonGroup::Add@XTPControlType@int@LPCTSTR@int@BOOL>
	CXTPControl* AddClone(CXTPControl* pControl, int nBefore, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a CXTPControl to the ribbon group.
	// Parameters:
	//     pControl - Control to add.
	//     nBefore  - Index of an existing control that the new control
	//                will be inserted before.
	// Returns:
	//     A pointer to the CXTPControl object that was added to the ribbon group.
	// See Also:
	//     Add
	//-----------------------------------------------------------------------
	CXTPControl* InsertAt(CXTPControl* pControl, int nBefore = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPControl object that has been added to the CXTPRibbonGroup
	//     at the specified index.
	// Parameters:
	//     nIndex - Index of the item to retrieve within the group's
	//              collection of controls.
	// Remarks:
	//     GetAt retrieves the CXTPControl from the m_arrControls items collection
	//     that contains all of the CXTPControl objects that have been added
	//     to the CXTPRibbonGroup.
	// Returns:
	//     A pointer to the CXTPControl object at the specified index.
	// See Also:
	//     m_arrControls, Add, RemoveAt, RemoveAll
	//-----------------------------------------------------------------------
	CXTPControl* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the item at a specified index from the group's
	//     collection of items.
	// Parameters:
	//     nIndex - Index of the item to remove from the group's
	//              collection of items.
	// Remarks:
	//     Removes an item by its index from the group's collection of items.
	// See Also:
	//     m_arrControls, Add, GetAt, RemoveAll
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified item from the group's collection of items.
	// Parameters:
	//     pControl - Item to remove from the group's collection of items.
	// See Also:
	//     RemoveAt, Add, GetAt, RemoveAll
	//-----------------------------------------------------------------------
	void Remove(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items from the group.
	// See Also:
	//     m_arrControls, Add, GetAt, RemoveAt
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the CXTPRibbonGroup is currently displayed as a
	//     group button.
	// Remarks:
	//     A single group button will be displayed if there is not enough
	//     room to display the items in the group. A group pop-up is
	//     displayed when the button is clicked. The pop-up contains all
	//     the items in the group. When a group button is clicked, a
	//     CXTPControlPopup is displayed that contains the items of the group.
	//
	//     The m_nIconId specifies the ID of the image to use for the group
	//     button that is displayed when there is not enough room to display
	//     the items in the group. No image will be displayed for the group
	//     button if m_nIconId is not set with SetIconId.
	//
	// Returns:
	//     TRUE if the group is displayed as a single group button,
	//     FALSE if the group is displayed normally.
	// See Also:
	//     SetIconId, GetIconId, GetControlGroupPopup
	//-----------------------------------------------------------------------
	BOOL IsReduced() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the group icon's identifier. This is the ID of
	//     the image to use for the group button that is displayed when there
	//     is not enough room to display the items in the group.
	// Parameters:
	//     nId - Group icon identifier to be set.
	// Remarks:
	//     The m_nIconId specifies the ID of the image to use for the group button
	//     that is displayed when there is not enough room to display the items
	//     in the group. When a group button is clicked, a CXTPControlPopup
	//     is displayed that contains the items of the group. No image
	//     will be displayed for the group button if m_nIconId is not set.
	// See Also:
	//     IsReduced, GetIconId, GetControlGroupPopup
	//-----------------------------------------------------------------------
	void SetIconId(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the group icon's identifier. This is the ID of
	//     the image to use for the group button that is displayed when there
	//     is not enough room to display the items in the group.
	// Remarks:
	//     The m_nIconId specifies the ID of the image to use for the group button
	//     that is displayed when there is not enough room to display the items
	//     in the group. When a group button is clicked, a CXTPControlPopup
	//     is displayed that contains the items of the group. No image
	//     will be displayed for the group button if m_nIconId is not set.
	// Returns:
	//     The group icon's identifier.
	// See Also:
	//     IsReduced, SetIconId, GetControlGroupPopup
	//-----------------------------------------------------------------------
	int GetIconId() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether to display an option button in the group's caption.
	// Parameters:
	//     bShowOptionButton - TRUE to show an option button in the group's caption,
	//                         FALSE to hide the option button if one was visible.
	// Remarks:
	//     The option button is typically used to launch a dialog associated
	//     with the group. The Group ID (GetID) is used to identify which
	//     groups' option button was clicked.
	// See Also:
	//     m_bShowOptionButton, GetID, CXTPRibbonPaintManager::DrawControlGroupOption
	//-----------------------------------------------------------------------
	void ShowOptionButton(BOOL bShowOptionButton = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the option button is visible.
	// Returns:
	//     TRUE if the option button is visible.
	// See Also:
	//     ShowOptionButton
	//-----------------------------------------------------------------------
	BOOL IsOptionButtonVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group is visible.
	// Returns:
	//     TRUE if the group is visible.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide the group.
	// Parameters:
	//     bVisible - TRUE to show the group, FALSE to hide.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group's own index within the parent group collection.
	// Returns:
	//     The group's own index within the parent group collection.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies a parameter from one group to another.
	// Parameters:
	//     pGroup - Group with the parameter to be copied.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPRibbonGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the type of the control.
	// Parameters:
	//     pControl - Pointer to a CXTPControl object.
	//     type     - New control type; one of the values defined by
	//                the XTPControlType enumeration.
	// Returns:
	//     A pointer to a CXTPControl object.
	//-----------------------------------------------------------------------
	CXTPControl* SetControlType(CXTPControl* pControl, XTPControlType type);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified control.
	// Parameters:
	//     type       - Type of the control to find.
	//     nId        - Control's identifier.
	//     bVisible   - TRUE if the control is visible.
	//     bRecursive - TRUE to find the control in the nested command bars.
	// Returns:
	//     A pointer to the CXTPControl object if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPControl* FindControl(int nId) const;

	// <combine CXTPRibbonGroup::FindControl@int@const>
	CXTPControl* FindControl(XTPControlType type, int nId, BOOL bVisible, BOOL bRecursive) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group's ID.
	// Remarks:
	//     The ID is used to identify the group.
	// Returns:
	//     The group's ID.
	// See Also:
	//     GetControlGroupPopup, GetControlGroupOption
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group's ID.
	// Parameters:
	//     nId - New ID to be set.
	// See Also:
	//     GetID
	//-----------------------------------------------------------------------
	void SetID(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if controls grouping is used.
	// Remarks:
	//     This is for controls grouping within a ribbon group.
	//     When you set begin group to TRUE, it starts a new group of controls.
	//     SetControlsGrouping will group controls like in MS Word within the
	//     ribbon group. If you pass FALSE, it groups them like in a regular
	//     toolbar (divider line).
	// Returns:
	//     TRUE if controls grouping is used, otherwise FALSE.
	// See Also:
	//     SetControlsGrouping
	//-----------------------------------------------------------------------
	BOOL IsControlsGrouping() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if controls grouping should be used.
	// Parameters:
	//     bControlsGrouping - TRUE to use controls grouping.
	// Remarks:
	//     This is for controls grouping within a ribbon group.
	//     When you set begin group to TRUE, it starts a new group of controls.
	//     SetControlsGrouping will group controls like in MS Word within the
	//     ribbon group. If you pass FALSE, it groups them like in a regular
	//     toolbar (divider line).
	// See Also:
	//     IsControlsGrouping
	//-----------------------------------------------------------------------
	void SetControlsGrouping(BOOL bControlsGrouping = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if controls should be automatically centered in the group.
	// Parameters:
	//     bControlsCentering - TRUE to automatically center controls in the group.
	//-----------------------------------------------------------------------
	void SetControlsCentering(BOOL bControlsCentering = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if controls are automatically centered in the group.
	// Returns:
	//     TRUE if controls are automatically centered in the group, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsControlsCentering() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group and all of the controls in the group.
	// Parameters:
	//     pDC - Pointer to the device context in which to draw the group.
	//     rcClipBox - The rectangular area of the window that is invalid.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, CRect rcClipBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from/to an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the ribbon group reduction level.
	// Parameters:
	//     nAllowReduceLevel - Ribbon group reduction level to be set;
	//                         must be one of the values defined by the
	//                         XTPRibbonGroupReductionLevels enumeration.
	// See also:
	//     XTPRibbonGroupReductionLevels
	//-----------------------------------------------------------------------
	void AllowReduce(int nAllowReduceLevel = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ribbon group reduction level.
	// Returns:
	//     The ribbon group reduction level; one of the values defined by the
	//     XTPRibbonGroupReductionLevels enumeration.
	// See also:
	//     XTPRibbonGroupReductionLevels
	//-----------------------------------------------------------------------
	int GetAllowReduceLevel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to specify whether the group will automatically
	//     align caption/edit parts.
	// Parameters:
	//     bArrangeEditCaption - TRUE to automatically align caption/edit parts.
	//-----------------------------------------------------------------------
	void SetArrangeEditCaption(BOOL bArrangeEditCaption);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the CXTPControl that represents the group's OptionButton
	//     (used to set tooltip and status bar text).
	// Remarks:
	//     GetControlGroupOption is only used to set the tooltip text and
	//     description text for the OptionButton. No other properties/methods
	//     of the CXTPControl are used.
	// Returns:
	//     A pointer to the CXTPControl that represents the group's OptionButton.
	// See Also:
	//     CXTPRibbonPaintManager::DrawControlGroupOption
	//-----------------------------------------------------------------------
	CXTPControl* GetControlGroupOption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the CXTPControlPopup used to display controls that
	//     are not currently visible in the group.
	// Remarks:
	//     A single group button will be displayed if there is not enough
	//     room to display the items in the group. A group pop-up is displayed
	//     when the button is clicked. The pop-up contains all the items
	//     in the group. When a group button is clicked, a CXTPControlPopup
	//     is displayed that contains the items of the group.
	// Returns:
	//     A pointer to the CXTPControlPopup used to display controls that
	//     are not currently visible in the group.
	// See Also:
	//     IsReduced, CXTPRibbonPaintManager::DrawControlGroupPopup
	//-----------------------------------------------------------------------
	CXTPControlPopup* GetControlGroupPopup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the toolbar specified by nIDResource.
	// Parameters:
	//     nIDResource - Resource ID of the toolbar to be loaded.
	//     bLoadIcons  - TRUE to load icons from application resources.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadToolBar(UINT nIDResource, BOOL bLoadIcons = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the controls from the button array.
	// Parameters:
	//     pButtons - Pointer to an array of command IDs. It cannot be NULL.
	//     nCount   - Number of elements in the array pointed to by pButtons.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	// See Also:
	//     LoadToolBar
	//-----------------------------------------------------------------------
	BOOL SetButtons(UINT* pButtons, int nCount);

	CXTPRibbonGroups* GetGroups() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width and height of the group.
	// Parameters:
	//     pDC - Pointer to the device context that the group is drawn on.
	// Remarks:
	//     The total width and height is determined based on how many
	//     controls are added to the group. Regardless of how few
	//     controls are added, the minimum width of a group is 47 pixels
	//     and the minimum height is 85 pixels.
	// Returns:
	//     A CSize object containing the width and height of the group.
	// See Also:
	//     GetCount
	//-----------------------------------------------------------------------
	// virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called automatically to reposition all controls in the group.
	//     (e.g. when the position/size of the ribbon bar changes).
	// Parameters:
	//     pDC - Pointer to the device context in which to draw the group.
	//-----------------------------------------------------------------------
	virtual void RepositionControls(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the group is added.
	// See Also:
	//     OnGroupRemoved
	//-----------------------------------------------------------------------
	virtual void OnGroupAdded();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the group is removed.
	// See Also:
	//     OnGroupAdded
	//-----------------------------------------------------------------------
	virtual void OnGroupRemoved();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the minimum width of the ribbon group.
	// Parameters:
	//     pDC - Pointer to the device context used to draw the ribbon group.
	// Returns:
	//     The minimum width of the ribbon group.
	// See Also:
	//     Draw
	//-----------------------------------------------------------------------
	virtual int GetMinimumWidth(CDC* pDC);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the size of the ribbon group is calculated.
	// Parameters:
	//     pDC - Pointer to the device context used to draw the ribbon group.
	// See Also:
	//     OnAfterCalcSize
	//-----------------------------------------------------------------------
	virtual void OnBeforeCalcSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called after the size of the ribbon group is calculated.
	// See Also:
	//     OnBeforeCalcSize
	//-----------------------------------------------------------------------
	virtual void OnAfterCalcSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the width of the ribbon group.
	// Parameters:
	//     pDC - Pointer to the device context used to draw the ribbon group.
	// Returns:
	//     The width of the ribbon group.
	// See Also:
	//     OnBeforeCalcSize, OnAfterCalcSize, GetMinimumWidth
	//-----------------------------------------------------------------------
	virtual int OnCalcDynamicSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to reduce the size of the ribbon group.
	// Parameters:
	//     nLevel      - Reduce level; must be one of the values defined
	//                   by the XTPRibbonGroupReductionLevels enumeration.
	//     nWidthAvail - Total pixels to reduce the size of the ribbon group by.
	// Returns:
	//     TRUE if the size of the ribbon group was reduced.
	// See also:
	//     XTPRibbonGroupReductionLevels
	//-----------------------------------------------------------------------
	virtual BOOL OnReduceSize(int nLevel, int nWidthAvail);

	//-----------------------------------------------------------------------
	// Summary:
	///    This method is called to extend the size of the ribbon group.
	// Parameters:
	//     nWidthAvail - Total pixels to extend the size of the ribbon group by.
	// Returns:
	//     TRUE if the size of the ribbon group was extended.
	//-----------------------------------------------------------------------
	virtual BOOL OnExtendSize(int nWidthAvail);

	//{{AFX_CODEJOCK_PRIVATE
protected:
	virtual void OnAdjustBorders(int nWidth, CRect rcBorders);
	void CenterColumn(int nFirstItem, int nLastItem, int nGroupHeight);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
	int _CalcSpecialDynamicSize(CDC* pDC);
	int _WrapSpecialDynamicSize(int nWidth, BOOL bIncludeTail);
	int _GetSizeSpecialDynamicSize();
	BOOL _FindBestWrapSpecialDynamicSize();
	void ArrangeEditCaption();
	//}}AFX_CODEJOCK_PRIVATE

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	afx_msg long OleGetItemCount();
	afx_msg int OleGetIndex();
	afx_msg LPDISPATCH OleGetItem(int nIndex);

	afx_msg LPDISPATCH OleAdd(long controlType, int nId, LPCTSTR strCaption,
							  const VARIANT& varBefore, const VARIANT& varTemporary);
	afx_msg LPDISPATCH OleGetControlGroupPopup();
	afx_msg LPDISPATCH OleGetControlGroupOption();
	afx_msg LPDISPATCH OleGetRibbonBar();
	afx_msg LPDISPATCH OleGetGroups();
	DECLARE_PROPERTY(Visible, BOOL);
	afx_msg void OleAddControl(LPDISPATCH Control, const VARIANT& varBefore);

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonGroup);
	DECLARE_INTERFACE_MAP()
	DECLARE_ENUM_VARIANT(CXTPRibbonGroup);
	void OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bControlsCentering;						  // TRUE to center controls inside groups.
	BOOL m_bControlsGrouping;						  // TRUE to group controls.
	CString m_strCaption;							  // Caption of the group.
	int m_nIndex;									  // Index of the group.
	CXTPRibbonGroups* m_pGroups;					  // Parent groups collection.
	CRect m_rcGroup;								  // Bounding rectangle of the group.
	CArray<CXTPControl*, CXTPControl*> m_arrControls; // Collection of group's controls.
	CXTPCommandBar* m_pParent;						  // Parent ribbon bar.
	CXTPRibbonBar* m_pRibbonBar;					  // Parent ribbon bar.
	int m_nId;										  // Identifier of the group.
	int m_nIconId;									  // Icon index of the group.

	CXTPControl* m_pControlGroupOption;		// Group option control.
	CXTPControlPopup* m_pControlGroupPopup; // Group popup control.
	BOOL m_bReduced;						// TRUE if group was reduced.
	BOOL m_bShowOptionButton;				// TRUE to show option button.
	BOOL m_bVisible;						// TRUE if group is visible.
	int m_nAllowReduce;						// Ribbon group reduction level.

	int m_nRowCount; // Total rows number of items.

	BOOL m_bAutoArrangeEditCaption; // Specifies whether the group will automatically align
									// caption/edit parts.

private:
	struct LAYOUTINFO;
	struct CONTROLINFO;
	LAYOUTINFO* m_pLayoutInfo;

private:
	friend class CXTPRibbonGroups;
	friend class CXTPRibbonBar;
	friend class CControlGroupPopup;
	friend class CXTPRibbonGroupPopupToolBar;
	friend class CXTPRibbonGroupControlPopup;
};

//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPRibbonGroupOption : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CXTPRibbonGroupOption)

public:
	CXTPRibbonGroupOption();
	void Draw(CDC* pDC);
};

class _XTP_EXT_CLASS CXTPRibbonGroupControlPopup : public CXTPControlPopup
{
	DECLARE_XTP_CONTROL(CXTPRibbonGroupControlPopup)

public:
	CXTPRibbonGroupControlPopup(CXTPRibbonGroup* pGroup = 0);

	virtual CSize GetSize(CDC* pDC);
	void Draw(CDC* pDC);
	BOOL IsTransparent() const;

	CXTPRibbonBar* GetRibbonBar() const;

	virtual void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);
	virtual void GenerateCommandBarList(DWORD& /*nID*/, CXTPCommandBarList* /*pCommandBarList*/,
										XTP_COMMANDBARS_PROPEXCHANGE_PARAM* /*pParam*/);
	virtual void OnLButtonUp(CPoint /*point*/);
	virtual BOOL IsVisible(DWORD dwSkipFlags = 0) const;
	virtual BOOL OnSetPopup(BOOL bPopup);

protected:
	CXTPRibbonGroup* m_pPopupGroup;
};

//}}AFX_CODEJOCK_PRIVATE

AFX_INLINE CString CXTPRibbonGroup::GetCaption() const
{
	return m_strCaption;
}

AFX_INLINE CXTPCommandBar* CXTPRibbonGroup::GetParent() const
{
	return m_pParent;
}

AFX_INLINE CRect CXTPRibbonGroup::GetRect() const
{
	return m_rcGroup;
}

AFX_INLINE CXTPRibbonBar* CXTPRibbonGroup::GetRibbonBar() const
{
	return (CXTPRibbonBar*)m_pRibbonBar;
}

AFX_INLINE int CXTPRibbonGroup::GetCount() const
{
	return (int)m_arrControls.GetSize();
}

AFX_INLINE CXTPControl* CXTPRibbonGroup::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetCount() ? m_arrControls.GetAt(nIndex) : NULL;
}

AFX_INLINE int CXTPRibbonGroup::GetID() const
{
	return m_nId;
}

AFX_INLINE CXTPControl* CXTPRibbonGroup::GetControlGroupOption() const
{
	return m_pControlGroupOption;
}

AFX_INLINE CXTPControlPopup* CXTPRibbonGroup::GetControlGroupPopup() const
{
	return m_pControlGroupPopup;
}

AFX_INLINE BOOL CXTPRibbonGroup::IsReduced() const
{
	return m_bReduced;
}

AFX_INLINE void CXTPRibbonGroup::SetIconId(int nId)
{
	m_nIconId = nId;
}

AFX_INLINE int CXTPRibbonGroup::GetIconId() const
{
	return m_nIconId <= 0 ? m_nId : m_nIconId;
}

AFX_INLINE void CXTPRibbonGroup::ShowOptionButton(BOOL bShowOptionButton)
{
	m_bShowOptionButton = bShowOptionButton;
}

AFX_INLINE int CXTPRibbonGroup::GetIndex() const
{
	return m_nIndex;
}

AFX_INLINE BOOL CXTPRibbonGroup::IsControlsGrouping() const
{
	return m_bControlsGrouping;
}

AFX_INLINE void CXTPRibbonGroup::SetControlsGrouping(BOOL bControlsGrouping)
{
	m_bControlsGrouping = bControlsGrouping;
}

AFX_INLINE void CXTPRibbonGroup::SetControlsCentering(BOOL bControlsCentering)
{
	m_bControlsCentering = bControlsCentering;
}

AFX_INLINE BOOL CXTPRibbonGroup::IsControlsCentering() const
{
	return m_bControlsCentering;
}

AFX_INLINE void CXTPRibbonGroup::AllowReduce(int nAllowReduce)
{
	m_nAllowReduce = nAllowReduce;
}

AFX_INLINE int CXTPRibbonGroup::GetAllowReduceLevel() const
{
	return m_nAllowReduce;
}

AFX_INLINE void CXTPRibbonGroup::SetArrangeEditCaption(BOOL bArrangeEditCaption)
{
	m_bAutoArrangeEditCaption = bArrangeEditCaption;
}

AFX_INLINE CXTPRibbonGroups* CXTPRibbonGroup::GetGroups() const
{
	return m_pGroups;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONGROUP_H__)
