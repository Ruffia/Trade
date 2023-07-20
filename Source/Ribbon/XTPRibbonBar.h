// XTPRibbonBar.h: interface for the CXTPRibbonBar class.
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
#if !defined(__XTPRIBBONBAR_H__)
#	define __XTPRIBBONBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonQuickAccessControls;
class CXTPRibbonTab;
class CXTPRibbonControlTab;
class CXTPTabPaintManager;
class CXTPRibbonBar;
class CXTPRibbonGroup;
class CXTPCommandBarsFrameHook;
class CXTPRibbonTabContextHeaders;
class CXTPRibbonGroups;
class CXTPRibbonPaintManager;
class CXTPRibbonControlSystemButton;

//---------------------------------------------------------------------------
// Summary:
//     The WM_XTP_RIBBONMINIMIZE message is sent to the CXTPCommandBars site
//     when the user minimizes the ribbon bar.
//---------------------------------------------------------------------------
#	define WM_XTP_RIBBONMINIMIZE (WM_XTP_COMMANDBARS_BASE + 25)

#	define WM_XTP_CUSTOMIZATION_RIBBONRESET (WM_XTP_COMMANDBARS_BASE + 27)

//===========================================================================
// Summary:
//     CXTPRibbonBar is a CXTPMenuBar derived class.
//     It represents a Ribbon control from Office 2007.
// Example:
//     The following code sample demonstrates how to create a CXTPRibbonBar:
// <code>
// CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pCommandBars->Add(_T("The Ribbon"), xtpBarTop,
// RUNTIME_CLASS(CXTPRibbonBar));
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonBar
	: public CXTPMenuBar
	, public CXTPRibbonScrollableBar
{
private:
	class CControlQuickAccessMorePopup;
	class CControlQuickAccessCommand;

private:
	DECLARE_XTP_COMMANDBAR(CXTPRibbonBar)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonBar object.
	//-----------------------------------------------------------------------
	CXTPRibbonBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new tab to the ribbon bar.
	// Parameters:
	//     lpszCaption - Caption of the tab to be added.
	//     nID         - Identifier of the tab to be added.
	// Returns:
	//     A pointer to a new CXTPRibbonTab object.
	// See Also:
	//     InsertTab
	//-----------------------------------------------------------------------
	CXTPRibbonTab* AddTab(LPCTSTR lpszCaption);
	CXTPRibbonTab* AddTab(int nID); // <combine CXTPRibbonBar::AddTab@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to insert a new tab to the ribbon bar at a specified position.
	// Parameters:
	//     nItem       - Position to insert the new tab.
	//     nID         - Identifier of the tab to be added.
	//     lpszCaption - Caption of the tab to be added.
	//     pTab        - Pointer to a CXTPRibbonTab object.
	// Returns:
	//     A pointer to a new CXTPRibbonTab object.
	// See Also:
	//     AddTab
	//-----------------------------------------------------------------------
	CXTPRibbonTab* InsertTab(int nItem, int nID);
	CXTPRibbonTab* InsertTab(int nItem, LPCTSTR lpszCaption,
							 int nID = 0); // <combine CXTPRibbonBar::InsertTab@int@int>
	CXTPRibbonTab* InsertTab(int nItem,
							 CXTPRibbonTab* pTab); // <combine CXTPRibbonBar::InsertTab@int@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the currently selected tab.
	// Returns:
	//     A pointer to the currently selected CXTPRibbonTab object.
	// See Also:
	//     SetCurSel
	//-----------------------------------------------------------------------
	CXTPRibbonTab* GetSelectedTab() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the tab at a specified position.
	// Parameters:
	//     nIndex - Index of the tab to retrieve.
	// Returns:
	//     A pointer to the CXTPRibbonTab object at the specified position.
	// See Also:
	//     GetTabCount
	//-----------------------------------------------------------------------
	CXTPRibbonTab* GetTab(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find a tab with a specified identifier.
	// Parameters:
	//     nId - Identifier of the tab to be found.
	// Returns:
	//     A pointer to the CXTPRibbonTab object with the specified identifier.
	// See Also:
	//     FindGroup
	//-----------------------------------------------------------------------
	CXTPRibbonTab* FindTab(int nId) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find a group with a specified identifier.
	// Parameters:
	//     nId - Identifier of the group to be found.
	// Returns:
	//     A pointer to the CXTPRibbonGroup object with the specified identifier.
	// See Also:
	//     FindTab
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* FindGroup(int nId) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of tabs in the ribbon bar.
	// Returns:
	//     The number of tabs in the ribbon bar.
	// See Also:
	//     GetTab
	//-----------------------------------------------------------------------
	int GetTabCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the ribbon paint manager.
	// Returns:
	//     A pointer to a CXTPRibbonPaintManager object.
	// See Also:
	//     AddTab
	//-----------------------------------------------------------------------
	CXTPRibbonPaintManager* GetRibbonPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the currently selected tab.
	// Parameters:
	//     nIndex - Index of the tab to be set.
	// See Also:
	//     GetSelectedTab
	//-----------------------------------------------------------------------
	void SetCurSel(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the command bar button.
	// Returns:
	//     The width and height values of the command bar button.
	//-----------------------------------------------------------------------
	CSize GetButtonSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the currently selected tab.
	// Returns:
	//     The index of the currently selected tab.
	// See Also:
	//     SetCurSel
	//-----------------------------------------------------------------------
	int GetCurSel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the tab paint manager.
	// Remarks:
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager (i.e. tab colors,
	//     styles, etc.). This member must be overridden in derived classes.
	// Returns:
	//     A pointer to the CXTPTabPaintManager that contains the
	//     visual elements of the tabs.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager* GetTabPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the highlighted group.
	// Returns:
	//     A pointer to the CXTPRibbonGroup object under the mouse cursor.
	// See Also:
	//     CXTPRibbonGroup
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* GetHighlightedGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the Ribbon state is active/inactive.
	// Returns:
	//     TRUE if the Ribbon state is active, FALSE if the Ribbon state is inactive.
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the groups part of the ribbon bar is visible.
	// Returns:
	//     TRUE if the groups part of the ribbon bar is visible.
	// See Also:
	//     SetGroupsVisible
	//-----------------------------------------------------------------------
	BOOL IsGroupsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the tabs part of the ribbon bar is visible.
	// Returns:
	//     TRUE if the tabs part of the ribbon bar is visible.
	// See Also:
	//     SetTabsVisible
	//-----------------------------------------------------------------------
	BOOL IsTabsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the ribbon bar is visible.
	// Returns:
	//     TRUE if the ribbon bar is visible.
	//-----------------------------------------------------------------------
	BOOL IsRibbonBarVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to show/hide the ribbon.
	// Parameters:
	//     bVisible - TRUE to show the ribbon, FALSE to hide.
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide the groups part of the ribbon bar.
	// Parameters:
	//     bVisible - TRUE to show the groups part of the ribbon bar.
	// See Also:
	//     IsGroupsVisible
	//-----------------------------------------------------------------------
	void SetGroupsVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide the tabs part of the ribbon bar.
	// Parameters:
	//     bVisible - TRUE to show the tabs part of the ribbon bar.
	// See Also:
	//     IsTabsVisible
	//-----------------------------------------------------------------------
	void SetTabsVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the CXTPRibbonControlTab object that represents
	//     ribbon tabs.
	// Returns:
	//     A pointer to the CXTPRibbonControlTab object that represents ribbon tabs.
	//-----------------------------------------------------------------------
	CXTPRibbonControlTab* GetControlTab() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the minimum width/height before the ribbon bar disappears.
	// Parameters:
	//     nMinVisibleWidth  - New minimum width.
	//     nMinVisibleHeight - New minimum height.
	// Remarks:
	//     Default value is 250 pixels.
	// See Also:
	//     GetMinimumVisibleWidth
	//-----------------------------------------------------------------------
	void SetMinimumVisibleWidth(int nMinVisibleWidth);
	void SetMinimumVisibleSize(int nMinVisibleWidth,
							   int nMinVisibleHeight); // <combine
													   // CXTPRibbonBar::SetMinimumVisibleWidth@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the minimum width before the ribbon bar disappears.
	// Returns:
	//     The minimum width before the ribbon bar disappears.
	// See Also:
	//     SetMinimumVisibleWidth
	//-----------------------------------------------------------------------
	int GetMinimumVisibleWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the collection of Quick Access controls.
	// Returns:
	//     A pointer to the CXTPRibbonQuickAccessControls object that represents
	//     the collection of Quick Access controls.
	//-----------------------------------------------------------------------
	CXTPRibbonQuickAccessControls* GetQuickAccessControls() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add the Ribbon top-left system button.
	// Parameters:
	//     nID - Identifier of the system button.
	// See Also:
	//     GetSystemButton
	//-----------------------------------------------------------------------
	CXTPControlPopup* AddSystemButton(int nID = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the Ribbon top-left system button.
	// Returns:
	//     The Ribbon top-left system button.
	// See Also:
	//     AddSystemButton
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemButton* GetSystemButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the command bar is a
	//     CXTPRibbonBar.
	// Returns:
	//     TRUE if the command bar is a CXTPRibbonBar, otherwise FALSE.
	// See Also:
	//     CXTPCommandBar, CXTPRibbonBar
	//-----------------------------------------------------------------------
	virtual BOOL IsRibbonBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all tabs.
	// See Also:
	//     RemoveTab
	//-----------------------------------------------------------------------
	void RemoveAllTabs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a single tab.
	// Parameters:
	//     nIndex - Index of the tab to remove.
	// See Also:
	//     RemoveTab
	//-----------------------------------------------------------------------
	void RemoveTab(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to make sure that a control is visible on the ribbon bar.
	// Parameters:
	//     pControl - Pointer to the CXTPControl child whose visibility must be checked.
	//-----------------------------------------------------------------------
	virtual void EnsureVisible(CXTPControl* pControl);

	BOOL OnFrameMouseWheel(BOOL bForward);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable Office 2007 frame.
	// Parameters:
	//     bEnable - TRUE to enable Office 2007 frame, FALSE to disable.
	// See Also:
	//     IsFrameThemeEnabled
	//-----------------------------------------------------------------------
	void EnableFrameTheme(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Office 2007 frame is enabled/disabled.
	// Returns:
	//     TRUE if enabled, FALSE if disabled.
	// See Also:
	//     EnableFrameTheme
	//-----------------------------------------------------------------------
	BOOL IsFrameThemeEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show Quick Access controls below/above the ribbon bar.
	// Parameters:
	//     bBelow - TRUE to show Quick Access controls below the ribbon bar,
	//              FALSE to show Quick Access controls above the ribbon bar.
	// See Also:
	//     IsQuickAccessBelowRibbon
	//-----------------------------------------------------------------------
	void ShowQuickAccessBelowRibbon(BOOL bBelow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Quick Access controls are shown below/above the ribbon bar.
	// Returns:
	//     TRUE if Quick Access controls are shown below the ribbon bar,
	//     FALSE if Quick Access controls are shown above the ribbon bar.
	// See Also:
	//     ShowQuickAccessBelowRibbon
	//-----------------------------------------------------------------------
	BOOL IsQuickAccessBelowRibbon() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide Quick Access.
	// Parameters:
	//     bShow - TRUE to show Quick Access, FALSE to hide.
	// See Also:
	//     IsQuickAccessVisible, ShowQuickAccessBelowRibbon
	//-----------------------------------------------------------------------
	void ShowQuickAccess(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Quick Access is visible.
	// Returns:
	//     TRUE if Quick Access is visible, otherwise FALSE.
	// See Also:
	//     ShowQuickAccess
	//-----------------------------------------------------------------------
	BOOL IsQuickAccessVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow for duplicate controls for Quick Access.
	// Parameters:
	//     bAllow - TRUE to allow for duplicate controls for Quick Access.
	// See Also:
	//     IsAllowQuickAccessDuplicates, ShowQuickAccessBelowRibbon
	//-----------------------------------------------------------------------
	void AllowQuickAccessDuplicates(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow for customization of Quick Access.
	// Parameters:
	//     bAllow - TRUE to allow for customization of Quick Access.
	// See Also:
	//     IsAllowQuickAccessDuplicates, ShowQuickAccessBelowRibbon
	//-----------------------------------------------------------------------
	void AllowQuickAccessCustomization(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if duplicate controls are allowed for Quick Access.
	// Returns:
	//     TRUE if duplicate controls are allowed for Quick Acess.
	// See Also:
	//     AllowQuickAccessDuplicates
	//-----------------------------------------------------------------------
	BOOL IsAllowQuickAccessDuplicates() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to select the next/previous tab.
	// Parameters:
	//     bNext - TRUE to select next, FALSE to select previous.
	//-----------------------------------------------------------------------
	void SelectNextTab(BOOL bNext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to minimize the ribbon bar.
	// Parameters:
	//     bMinimized - TRUE to minimize the ribbon bar.
	//-----------------------------------------------------------------------
	virtual void SetRibbonMinimized(BOOL bMinimized);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the ribon bar is minimized.
	// Returns:
	//     TRUE if the ribbon bar is minimized.
	//-----------------------------------------------------------------------
	BOOL IsRibbonMinimized() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow/disallow the minimize feature of the ribbon bar.
	// Parameters:
	//     bAllow - TRUE to allow the minimize feature of the ribbon bar,
	//              FALSE to disallow.
	//-----------------------------------------------------------------------
	void AllowMinimize(BOOL bAllow);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     When switching tabs, RebuildControls first deletes all of the controls
	//     from the old tab groups. Then controls are added to the tab groups
	//     of the tab to be selected. Finally, the new tab is selected.
	// Parameters:
	//     pSelected - New tab to be selected.
	// See Also:
	//     CXTPRibbonControlTab::SetSelectedItem, CXTPRibbonTab::GetGroups
	//-----------------------------------------------------------------------
	void RebuildControls(CXTPRibbonTab* pSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the ribbon tabs.
	// Returns:
	//     A CRect object containing the bounding rectangle of the
	//     ribbon tabs.
	// See Also:
	//     CXTPRibbonThemeOffice2007Theme::FillRibbonBar
	//-----------------------------------------------------------------------
	CRect GetTabControlRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the ribbon caption.
	// Returns:
	//     A CRect object containing the bounding rectangle of the
	//     ribbon caption.
	// See Also:
	//     GetCaptionTextRect, GetQuickAccessRect, GetTabControlRect
	//-----------------------------------------------------------------------
	CRect GetCaptionRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the ribbon caption text.
	// Returns:
	//     A CRect object containing the bounding rectangle of the
	//     ribbon caption text.
	// See Also:
	//     GetCaptionRect, GetQuickAccessRect, GetTabControlRect
	//-----------------------------------------------------------------------
	CRect GetCaptionTextRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the ribbon Quick Access controls.
	// Returns:
	//     A CRect object containing the bounding rectangle of the
	//     ribbon Quick Access Controls.
	// See Also:
	//     GetCaptionRect, GetCaptionTextRect, GetTabControlRect
	//-----------------------------------------------------------------------
	CRect GetQuickAccessRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines which ribbon group, if any, is at
	//     a specified position.
	// Parameters:
	//     point - Point to test.
	// Returns:
	//     A pointer to the CXTPRibbonGroup at the specified position, if any,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* HitTestGroup(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tabs area height.
	// Returns:
	//     The tabs area height.
	// See Also:
	//     GetGroupsHeight
	//-----------------------------------------------------------------------
	virtual int GetTabsHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the Quick Access area height.
	// Returns:
	//     The Quick Access area height.
	// See Also:
	//     GetGroupsHeight, GetTabsHeight
	//-----------------------------------------------------------------------
	int GetQuickAccessHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the Quick Access area height.
	// Returns:
	//     The Quick Access area height.
	//-----------------------------------------------------------------------
	int CalcQuickAccessHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group area height.
	// Returns:
	//     The group area height.
	// See Also:
	//     GetTabsHeight
	//-----------------------------------------------------------------------
	virtual int GetGroupsHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group client area height.
	// Returns:
	//     The group client area height.
	// See Also:
	//     GetGroupsHeight, GetTabsHeight
	//-----------------------------------------------------------------------
	virtual int CalcClientHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ribbon groups bounding rectangle.
	// Returns:
	//     A CRect object containing the groups bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetGroupsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the ribbon groups height using the GetLargeIconSize method
	//     and the height of the captions.
	// Returns:
	//     The ribbon groups height.
	// See Also:
	//     GetGroupsHeight
	//-----------------------------------------------------------------------
	virtual int CalcGroupsHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height of the ribbon groups.
	// Parameters:
	//     nHeight - New height to be set.
	// See Also:
	//     GetTabsHeight
	//-----------------------------------------------------------------------
	void SetGroupsHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption height of the ribbon bar, in pixels.
	// Returns:
	//     The caption height of the ribbon bar, in pixels.
	// See Also:
	//     CalcGroupsHeight
	//-----------------------------------------------------------------------
	int GetCaptionHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the collection of context headers.
	// Returns:
	//     A pointer to the CXTPRinnobTabContextHeaders object that represents
	//     the collection of context headers.
	// See Also:
	//     CXTPRibbonTabContextHeader
	//-----------------------------------------------------------------------
	CXTPRibbonTabContextHeaders* GetContextHeaders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a control belongs to Quick Access controls.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the control is located in the Quick Access area.
	//-----------------------------------------------------------------------
	BOOL IsQuickAccessControl(CXTPControl* pControl) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a control can be added to Quick Access controls.
	// Parameters:
	//     pControl - Control to check.
	// Returns:
	//     TRUE if the control can be added to the Quick Access area.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowQuickAccessControl(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcClipBox - The rectangular area of the control that is invalid.
	//-----------------------------------------------------------------------
	virtual void DrawCommandBar(CDC* pDC, CRect rcClipBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the CXTPCommandBarsFrameHook object used to skin the frame.
	// Returns:
	//     A pointer to the CXTPCommandBarsFrameHook object used to skin the frame.
	// See Also:
	//     EnableFrameTheme
	//-----------------------------------------------------------------------
	CXTPCommandBarsFrameHook* GetFrameHook() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Vista Glass effect is enabled for the ribbon bar.
	// Returns:
	//     TRUE if Vista Glass effect is enabled for the ribbon bar.
	//-----------------------------------------------------------------------
	BOOL IsDwmEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font height for the ribbon bar.
	// Parameters:
	//     nFontHeight - New font height to set.
	//-----------------------------------------------------------------------
	void SetFontHeight(int nFontHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the font height of the ribbon bar.
	// Returns:
	//     The font height of the ribbon bar.
	//-----------------------------------------------------------------------
	int GetFontHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the caption part of the ribbon is visible.
	// Returns:
	//     TRUE if the caption part of the ribbon is visible.
	//-----------------------------------------------------------------------
	BOOL IsCaptionVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide the caption if the frame theme is not enabled.
	// Parameters:
	//     bShowCaption - TRUE to show the caption if the frame theme is not enabled,
	//                    FALSE to hide the caption if the frame theme is not enabled.
	//-----------------------------------------------------------------------
	void ShowCaptionAlways(BOOL bShowCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the caption is shown/hidden if the frame theme is not enabled.
	// Returns:
	//     TRUE if the caption is shown if the frame theme is not enabled,
	//     FALSE if the caption is hidden if the frame theme is not enabled.
	//-----------------------------------------------------------------------
	BOOL GetShowCaptionAlways() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of the toolbar icon.
	// Returns:
	//     The size of the toolbar icon.
	// See Also:
	//     GetLargeIconSize
	//-----------------------------------------------------------------------
	virtual CSize GetIconSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the Quick Access button.
	// Returns:
	//     A pointer to the Quick Access button.
	//-----------------------------------------------------------------------
	CXTPControl* GetControlQuickAccess() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if keyboard cues are visible.
	// Returns:
	//     TRUE if keyboard cues are visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsKeyboardCuesVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the ribbon/Quick Access toolbar.
	// Parameters:
	//     bShowWarningMessage - TRUE to show a warning message box to the user.
	//-----------------------------------------------------------------------
	void Reset(BOOL bShowWarningMessage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if backstage view is visible.
	// Returns:
	//     TRUE if backstage view is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsBackstageViewVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the icon ID that will be used if the user
	//     adds a new control to Quick Access without an icon.
	// Parameters:
	//     nIconId - Icon to be associated with the new Quick Access control.
	//-----------------------------------------------------------------------
	void SetQuickAccessEmptyIconId(int nIconId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow the backstage view to process keyboard
	//     input if it is active.
	// Parameters:
	//	   message - Code of the Window message (like WM_KEYDOWN).
	//	   wParam  - WPARAM of Window message.
	//	   lParam  - LPARAM of Window message.
	//-----------------------------------------------------------------------
	BOOL PassKeyboardInputToBackstageView(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when the current tab is about to be changed.
	// Parameters:
	//     pTab - New tab to be selected.
	// Returns:
	//     TRUE to cancel tab changing, FALSE to allow.
	//-----------------------------------------------------------------------
	virtual BOOL OnTabChanging(CXTPRibbonTab* pTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method called when the current tab is changed.
	// Parameters:
	//     pTab - New selected tab.
	//-----------------------------------------------------------------------
	virtual void OnTabChanged(CXTPRibbonTab* pTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the ribbon groups height using the GetLargeIconSize method
	//     and the height of the captions.
	// Parameters:
	//     nClientHeight - Client height for which the ribbon groups height
	//                     is to be computed.
	// Returns:
	//     The ribbon groups height.
	// See Also:
	//     GetGroupsHeight
	//-----------------------------------------------------------------------
	virtual int CalcGroupsHeight(int nClientHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a new pop-up toolbar object for a provided tab.
	// Parameters:
	//     pTab - Pointer to the tab for which a new pop-up toolbar must be created.
	// Returns:
	//     A pointer to the newly allocated pop-up toolbar object. The caller
	//     is responsible for releasing resources.
	//-----------------------------------------------------------------------
	virtual CXTPRibbonTabPopupToolBar* CreateTabPopupToolBar(CXTPRibbonTab* pTab);

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines whether a control is enabled.
	// Parameters:
	//      pControl - Pointer to the control to check.
	// Returns:
	//      TRUE if the control is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsControlEnabled(const CXTPControl* pControl) const;

	//{{AFX_CODEJOCK_PRIVATE
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign self identifiers for the serialization process.
	// Parameters:
	//     nID             - Identifier to assign.
	//     pCommandBarList - List of command bars.
	//     pParam          - Address of an XTP_COMMANDBARS_PROPEXCHANGE_PARAM structure.
	//-----------------------------------------------------------------------
	void GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList,
								XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called in the serialization process to restore pop-ups
	//     from a list of command bars.
	// Parameters:
	//     pCommandBarList - List of command bars.
	//-----------------------------------------------------------------------
	void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList);

	void DoPropExchange(CXTPPropExchange* pPX);
	void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);
	virtual CXTPPopupBar* CreateContextMenu(CXTPControl* pSelectedControl);
	virtual CXTPPopupBar* CreateMoreQuickAccessContextMenu();
	virtual BOOL ShouldSerializeBar();
	virtual void MergeToolBar(CXTPCommandBar* pCommandBar, BOOL bSilent);
	virtual void MergeToolBar(CXTPCommandBar* pCommandBar, BOOL bSilent,
							  int nDefaultConfirmationChoice);
	int HitTestCaption(CPoint point) const;
	void RepositionCaptionButtons();
	void RepositionContextHeaders();
	void AddCaptionButton(int nId, BOOL bAdd, BOOL bEnabled, CRect& rcCaption);
	BOOL ShrinkContextHeaders(int nLeft, int nRight);
	void ShowContextMenu(CPoint point, CXTPControl* pSelectedControl);
	virtual void CreateKeyboardTips();
	virtual void OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip);
	virtual void OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop,
								 CPoint ptDrag);
	void RefreshSysButtons();
	virtual BOOL ProcessSpecialKey(XTPSpecialKey key);

protected:
public:
	virtual CSize CalcDockingLayout(int nLength, DWORD dwMode, int nWidth = 0);
	void Reposition(int cx, int cy);
	void RepositionGroups(CDC* pDC, CRect rcGroups);
	virtual BOOL PreviewAccel(UINT chAccel);

	BOOL IsCaptionBarInBackstageMode() const;
	void SetCaptionBarBackstageMode(BOOL bMode);
	int GetBackstageModeMenuWidth() const;
	void SetBackstageModeMenuWidth(int nWidth);
	int GetBackstageSysButtonTop() const;
	void SetBackstageSysButtonTop(int nTop);
	int GetBackstageSysButtonBottom() const;
	void SetBackstageSysButtonBottom(int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode        - TRUE to set the tracking mode, FALSE otherwise.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is pop-up'ed by the keyboard.
	// Returns:
	//     TRUE if the method was successful.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);
	int OnHookMessage(HWND /*hWnd*/, UINT nMessage, WPARAM& wParam, LPARAM& lParam,
					  LRESULT& /*lResult*/);
	void OnRemoved();
	void OnGroupsScroll(BOOL bScrollLeft);
	int GetRibbonTopBorder() const;
	void CustomizeFindDropIndex(CXTPControl* pDataObject, const CPoint& point, CRect& rcMarker,
								int& nDropIndex, BOOL& bDropAfter);

	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_VIRTUAL(CXTPRibbonBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPRibbonBar)
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnPopupRButtonUp(CXTPCommandBar* pCommandBar, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomizePlaceQuickAccess(UINT nCommand);
	afx_msg LRESULT OnCustomizeCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg LPDISPATCH OleInsertTab(int nIndex, LPCTSTR lpszTitle);
	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg LPDISPATCH OleAddSystemButton();
	afx_msg LPDISPATCH OleGetSystemButton();
	afx_msg void OleSetSelectedTab(LPDISPATCH lpTab);
	afx_msg LPDISPATCH OleGetSelectedTab();
	afx_msg void OleEnableFrameTheme();

	afx_msg LPDISPATCH OleGetRibbonPaintManager();
	afx_msg LPDISPATCH OleGetTabPaintManager();
	afx_msg LPDISPATCH OleGetQuickAccessControls();
	afx_msg LPDISPATCH OleFindGroup(int nID);
	afx_msg LPDISPATCH OleFindTab(int nID);
	virtual void OleEnableCustomization();
	afx_msg LPDISPATCH OleGetControlQuickAccess();
	afx_msg LPDISPATCH OleGetControlSystemButton();
	afx_msg void OleSetRibbonMinimized(BOOL bNewValue);
	afx_msg long OleGetGroupsHeight();
	afx_msg void OleSetGroupsHeight(long nHeight);
	afx_msg long OleGetClientHeight();
	afx_msg void OleSetClientHeight(long nHeight);

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonBar);
	DECLARE_INTERFACE_MAP()
	DECLARE_ENUM_VARIANT(CXTPRibbonBar);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CRect m_rcTabControl;  // Bounding rectangle of tabs.
	CRect m_rcGroups;	  // Groups bounding rectangle.
	CRect m_rcCaption;	 // Caption bounding rectangle.
	CRect m_rcCaptionText; // Caption text bounding rectangle.
	CRect m_rcHeader;	  // Header bounding rectangle.
	CRect m_rcQuickAccess; // Quick Access bounding rectangle.

	BOOL m_bRibbonBarVisible;			// TRUE if the ribbon bar is visible.
	BOOL m_bGroupsVisible;				// TRUE if groups is visible.
	BOOL m_bTabsVisible;				// TRUE if tabs are visible.
	BOOL m_bShowQuickAccessBelow;		// TRUE to show Quick Access controls below the ribbon bar.
	BOOL m_bShowQuickAccess;			// TRUE to show Quick Access.
	BOOL m_bAllowQuickAccessDuplicates; // TRUE to allow Quick Access duplicates.
	BOOL m_bAllowQuickAccessCustomization; // TRUE to allow Quick Access customization.
	BOOL m_bGroupReducedChanged;		   // Some group appear in resize handler.
	int m_nQuickAccessHeight;			   // Quick Access height.
	BOOL m_bAllowMinimize;				   // TRUE to allow minimize ribbon.

	CXTPRibbonQuickAccessControls* m_pQuickAccessControls; // Quick Access controls (not used now).
	CXTPRibbonControlTab* m_pControlTab;				   // Control tab pointer.
	CXTPControl* m_pControlQuickAccess;					   // Quick Access menu control.
	CXTPControl* m_pControlQuickAccessMore;				   // Quick Access more control.
	CXTPRibbonControlSystemButton* m_pControlSystemButton; // System button control.
	BOOL m_bMinimizeOnDblClick; // TRUE to allow the ribbon to be minimized/maximized when double
								// clicked.
	int m_nMinVisibleWidth;		// Minimum width before ribbon bar disappears.
	int m_nMinVisibleHeight;	// Minimum height before ribbon bar disappears.

	CXTPRibbonTabContextHeaders* m_pContextHeaders; // Context header collection.
	int m_nCustomGroupsHeight;						// Custom groups height.
	int m_nCustomClientHeight;						// Custom client height.

	BOOL m_bMinimized;		   // TRUE if the Ribbon is currently minimized.
	BOOL m_bShowCaptionAlways; // TRUE to show caption even if EnableFrameTheme was not called.
	CString m_strCaptionText;  // Caption text.

	const int m_nQuickAccessHeightDiff;
	int m_nQuickAccessEmptyIconId;

	BOOL m_bIsCaptionBarInBackstageMode; // Is backstage active.
	int m_nBackstageMenuWidth;	// The width of the panel with commands and tabs in the backstage.
	int m_nBackstageSysButtonTop; // The top of the sysbutton on the backstage.
	int m_nBackstageSysButtonBottom; // The buttom of the sysbutton on the backstage.

private:
	friend class CXTPRibbonControlTab;
	friend class CXTPRibbonBarControlQuickAccessPopup;
	friend class CControlQuickAccessMorePopup;
	friend class CXTPRibbonGroup;
	friend class CXTPRibbonControls;
	friend class CXTPRibbonGroups;
	friend class CXTPRibbonTabPopupToolBar;
};

AFX_INLINE CRect CXTPRibbonBar::GetTabControlRect() const
{
	return m_rcTabControl;
}

AFX_INLINE CXTPRibbonGroup* CXTPRibbonBar::GetHighlightedGroup() const
{
	return m_pHighlightedGroup;
}

AFX_INLINE CXTPRibbonGroup* CXTPRibbonScrollableBar::GetHighlightedGroup() const
{
	return m_pHighlightedGroup;
}

AFX_INLINE CXTPRibbonControlTab* CXTPRibbonBar::GetControlTab() const
{
	return m_pControlTab;
}

AFX_INLINE void CXTPRibbonBar::SetMinimumVisibleWidth(int nMinVisibleWidth)
{
	m_nMinVisibleWidth = nMinVisibleWidth;
}

AFX_INLINE void CXTPRibbonBar::SetMinimumVisibleSize(int nMinVisibleWidth, int nMinVisibleHeight)
{
	m_nMinVisibleWidth  = nMinVisibleWidth;
	m_nMinVisibleHeight = nMinVisibleHeight;
}

AFX_INLINE int CXTPRibbonBar::GetMinimumVisibleWidth() const
{
	return m_nMinVisibleWidth;
}

AFX_INLINE CRect CXTPRibbonBar::GetCaptionRect() const
{
	return m_rcCaption;
}

AFX_INLINE CRect CXTPRibbonBar::GetCaptionTextRect() const
{
	return m_rcCaptionText;
}

AFX_INLINE CRect CXTPRibbonBar::GetQuickAccessRect() const
{
	return m_rcQuickAccess;
}

AFX_INLINE CXTPRibbonQuickAccessControls* CXTPRibbonBar::GetQuickAccessControls() const
{
	return m_pQuickAccessControls;
}

AFX_INLINE CXTPRibbonControlSystemButton* CXTPRibbonBar::GetSystemButton() const
{
	return m_pControlSystemButton;
}

AFX_INLINE BOOL CXTPRibbonBar::IsRibbonBarVisible() const
{
	return m_bVisible && m_bRibbonBarVisible;
}

AFX_INLINE CXTPRibbonTabContextHeaders* CXTPRibbonBar::GetContextHeaders() const
{
	return m_pContextHeaders;
}

AFX_INLINE BOOL CXTPRibbonBar::IsRibbonBar() const
{
	return TRUE;
}

AFX_INLINE void CXTPRibbonBar::ShowQuickAccess(BOOL bShow)
{
	m_bShowQuickAccess = bShow;
	OnRecalcLayout();
}

AFX_INLINE BOOL CXTPRibbonBar::IsQuickAccessVisible() const
{
	return m_bShowQuickAccess;
}

AFX_INLINE void CXTPRibbonBar::AllowQuickAccessDuplicates(BOOL bAllow)
{
	m_bAllowQuickAccessDuplicates = bAllow;
}

AFX_INLINE void CXTPRibbonBar::AllowQuickAccessCustomization(BOOL bAllow)
{
	m_bAllowQuickAccessCustomization = bAllow;
}

AFX_INLINE BOOL CXTPRibbonBar::IsAllowQuickAccessDuplicates() const
{
	return m_bAllowQuickAccessDuplicates;
}

AFX_INLINE void CXTPRibbonBar::SetGroupsHeight(int nHeight)
{
	m_nCustomGroupsHeight = nHeight;
}

AFX_INLINE void CXTPRibbonBar::AllowMinimize(BOOL bAllow)
{
	m_bAllowMinimize = bAllow;
}

AFX_INLINE CRect CXTPRibbonBar::GetGroupsRect() const
{
	return m_rcGroups;
}

AFX_INLINE void CXTPRibbonBar::ShowCaptionAlways(BOOL bShowCaption)
{
	m_bShowCaptionAlways = bShowCaption;
}

AFX_INLINE BOOL CXTPRibbonBar::GetShowCaptionAlways() const
{
	return m_bShowCaptionAlways;
}

AFX_INLINE CXTPControl* CXTPRibbonBar::GetControlQuickAccess() const
{
	return m_pControlQuickAccess;
}

AFX_INLINE void CXTPRibbonBar::SetQuickAccessEmptyIconId(int nIconId)
{
	m_nQuickAccessEmptyIconId = nIconId;
}

AFX_INLINE BOOL CXTPRibbonBar::IsCaptionBarInBackstageMode() const
{
	return m_bIsCaptionBarInBackstageMode;
}

AFX_INLINE void CXTPRibbonBar::SetCaptionBarBackstageMode(BOOL bMode)
{
	m_bIsCaptionBarInBackstageMode = bMode;
}

AFX_INLINE int CXTPRibbonBar::GetBackstageModeMenuWidth() const
{
	return m_nBackstageMenuWidth;
}

AFX_INLINE void CXTPRibbonBar::SetBackstageModeMenuWidth(int nWidth)
{
	m_nBackstageMenuWidth = nWidth;
}

AFX_INLINE int CXTPRibbonBar::GetBackstageSysButtonTop() const
{
	return m_nBackstageSysButtonTop;
}

AFX_INLINE void CXTPRibbonBar::SetBackstageSysButtonTop(int nTop)
{
	m_nBackstageSysButtonTop = nTop;
}

AFX_INLINE int CXTPRibbonBar::GetBackstageSysButtonBottom() const
{
	return m_nBackstageSysButtonBottom;
}

AFX_INLINE void CXTPRibbonBar::SetBackstageSysButtonBottom(int nBottom)
{
	m_nBackstageSysButtonBottom = nBottom;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONBAR_H__)
