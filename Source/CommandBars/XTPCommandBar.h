// XTPCommandBar.h : interface for the CXTPCommandBar class.
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
#if !defined(__XTPCOMMANDBAR_H__)
#	define __XTPCOMMANDBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockBar;
class CXTPControls;
class CXTPControl;
class CXTPControlPopup;
class CXTPCommandBars;
class CXTPCommandBar;
class CXTPCommandBarAnimation;
class CXTPPaintManager;
class CXTPImageManager;
class CXTPPropExchange;
struct XTP_COMMANDBARS_PROPEXCHANGE_PARAM;
class CXTPToolTipContext;
class CXTPCommandBarAnimation;
class CXTPCommandBarKeyboardTip;
class CXTPCommandBarList;

//{{AFX_CODEJOCK_PRIVATE
#	define XTP_TID_HOVER 112223
#	define XTP_TID_EXPANDED_POPUP 112224
#	define XTP_BOOL_DEFAULT 2
#	define XTP_TID_SHOWKEYBOARDTIPS 0xCCCA
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPCommandBar is a CWnd derived class. It represents the parent
//     class for command bars.
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBar
	: public CWnd
	, public CXTPHookManagerHookAble
	, public CXTPAccessible
	, IXTPPaintManagerEvents
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tracking state.
	// Returns:
	//     TRUE if the command bar is in the tracking mode.
	//-----------------------------------------------------------------------
	int IsTrackingMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the controls collection.
	// Returns:
	//     A CXTPControls pointer.
	//-----------------------------------------------------------------------
	CXTPControls* GetControls() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the controls collection of the command bar.
	// Parameters:
	//     pControls - New controls to be set.
	// See Also:
	//     GetControls
	//-----------------------------------------------------------------------
	void SetControls(CXTPControls* pControls);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the type of the command bar.
	// Returns:
	//     The type of the command bar. It can be one of the following:
	//     * <b>xtpBarTypeMenuBar</b> Indicates the command bar is a menu bar (CXTPMenuBar)
	//     * <b>xtpBarTypeNormal</b> Indicates the command bar is a simple toolbar (CXTPToolBar)
	//     * <b>xtpBarTypePopup</b> Indicates the command bar is a pop-up bar (CXTPPopupBar)
	// See Also:
	//     XTPBarType, GetPosition
	//-----------------------------------------------------------------------
	XTPBarType GetType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change text orientation.
	// PArameters:
	//     nTextOrientation - xtpBarTextHorizontal to draw text horizontally,
	//                        xtpBarTextVertical to draw text vertically.
	//-----------------------------------------------------------------------
	void SetTextOrientation(XTPBarTextOrientation nTextOrientation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the text orientation.
	//-----------------------------------------------------------------------
	XTPBarTextOrientation GetTextOrientation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the position of the command bar.
	// Returns:
	//     The position of the command bar.
	// See Also:
	//     XTPBarPosition, GetType
	//-----------------------------------------------------------------------
	XTPBarPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to recalculate the layout.
	//-----------------------------------------------------------------------
	void DelayLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the command bar.
	//-----------------------------------------------------------------------
	virtual void DelayRedraw();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode        - TRUE to set the tracking mode; otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is popped by the keyboard.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	// See Also:
	//     IsTrackingMode.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to convert menu items to command bar controls.
	// Parameters:
	//     pMenu           - Menu to be converted.
	//     bRemoveControls - TRUE to remove all controls first.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL LoadMenu(CMenu* pMenu, BOOL bRemoveControls = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the control at the specified index.
	// Parameters:
	//     nIndex - An integer index.
	// Returns:
	//     The CXTPControl pointer currently at this index.
	// See also:
	//     GetControls, CXTPControls::GetAt
	//-----------------------------------------------------------------------
	CXTPControl* GetControl(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the count of the controls.
	// Returns:
	//     The count of the controls.
	//-----------------------------------------------------------------------
	int GetControlCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select the specified control.
	// Parameters:
	//     nSelected - An integer index of the item to be selected.
	//     bKeyboard - TRUE if the item was selected using the keyboard.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetSelected(int nSelected, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently selected control.
	// Returns:
	//     Pointer to a CXTPControl object containing the currently
	//     selected control.
	//-----------------------------------------------------------------------
	CXTPControl* GetSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to pop-up the specified control.
	// Parameters:
	//     nPopuped  - An integer index of the item to be popped.
	//     bKeyboard - TRUE if the item is popped by the keyboard.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetPopuped(int nPopuped, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a title for this command bar.
	// Parameters:
	//     lpszTitle - Title to be set.
	//-----------------------------------------------------------------------
	void SetTitle(LPCTSTR lpszTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a title for this command bar.
	// Returns:
	//     The title of the command bar.
	//-----------------------------------------------------------------------
	CString GetTitle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the command bar's object.
	// Returns:
	//     A CXTPCommandBars pointer (can be NULL).
	//-----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's object.
	// Parameters:
	//     pCommandBars - CXTPCommandBars to be set.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the command bar's site.
	// Returns:
	//     Command bar's site (cannot be NULL).
	//-----------------------------------------------------------------------
	CWnd* GetSite() const;

	void SetOwner(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the command bar's site as CFrameWnd.
	// Returns:
	//     The command bar's site (can be NULL).
	//-----------------------------------------------------------------------
	CFrameWnd* GetFrameSite() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's flags.
	// Parameters:
	//     dwFlagsAdd    - Flags to be added.
	//     dwFlagsRemove - Flags to be removed.
	// Remarks:
	//     dwFlagsAdd and dwFlagsRemove can be combination of XTPToolBarFlags and
	//     XTPMenuBarFlags.<p/> Use the bitwise-OR (|) operator to combine the flags.<p/>
	//     * <b>xtpFlagAlignTop</b>:   Allows docking at the top of the client area.
	//     * <b>xtpFlagAlignBottom</b>:   Allows docking at the bottom of the client area.
	//     * <b>xtpFlagAlignLeft</b>:   Allows docking on the left side of the client area.
	//     * <b>xtpFlagAlignRight</b>:   Allows docking on the left side of the client area.
	//     * <b>xtpFlagAlignAny</b>:   Allows docking on any side of the client area.
	//     * <b>xtpFlagFloating</b>:   Allows floating.
	//     * <b>xtpFlagHideWrap</b>:   Allow to hide wrapped controls.
	//     * <b>xtpFlagStretched</b>:   Bar is stretched.
	//     * <b>xtpFlagStretchedShared</b>:   Bar is stretched, but does not fill a whole row.
	//     * <b>xtpFlagSmartLayout</b>:   Smart layout is enabled.
	//     * <b>xtpFlagHideMinimizeBox</b>:  To hide minimize box.
	//     * <b>xtpFlagHideMaximizeBox</b>:  To hide maximize box.
	//     * <b>xtpFlagIgnoreSetMenuMessage</b>:  To ignore MDI menus.
	//     * <b>xtpFlagHideClose</b>:  To hide close button.
	//     * <b>xtpFlagHideMDIButtons</b>:  Specifies to hide all MDI buttons displayed on the
	//                                      Command Bar.
	//     * <b>xtpFlagAddMDISysPopup</b>:  To add system MDI pop-up bar.  Specifies to add
	//                                      system MDI pop-up menu to the Menu Bar.  This will
	//                                      appear as an icon to the far left of the Menu Bar
	//                                      and will display the MDI menu bar options when
	//                                      clicked.
	// See Also:
	//     XTPMenuBarFlags, XTPToolBarFlags
	//-----------------------------------------------------------------------
	void SetFlags(DWORD dwFlagsAdd, DWORD dwFlagsRemove = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the controls from the button array.
	// Parameters:
	//     pButtons - Pointer to an array of command IDs. It cannot be NULL.
	//     nCount   - Number of elements in the array pointed to by pButtons.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also:
	//     LoadToolBar, LoadMenu.
	//-----------------------------------------------------------------------
	BOOL SetButtons(UINT* pButtons, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the toolbar specified by nIDResource.
	// Parameters:
	//     nIDResource - Resource ID of the toolbar to be loaded.
	//     bLoadIcons  - TRUE if the toolbar icons are to be used by CXTPImageManager.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also:
	//     LoadMenu, SetButtons.
	//-----------------------------------------------------------------------
	virtual BOOL LoadToolBar(UINT nIDResource, BOOL bLoadIcons = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the size of the actual bitmapped
	//     images to be added to a toolbar.
	// Parameters:
	//     size - Width and height, in pixels, of the toolbar's icons.
	//-----------------------------------------------------------------------
	void SetIconSize(CSize size);

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
	//     Call this method to get the large icon size of the toolbar.
	// Parameters:
	//     bAutoSize - TRUE to adjust size using current DPI mode.
	// Returns:
	//     CSize value containing the size of large toolbar icons.
	// See Also:
	//     GetIconSize
	//-----------------------------------------------------------------------
	CSize GetLargeIconSize(BOOL bAutoSize) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size of the buttons in the command bar.
	// Parameters:
	//     size - Width and height, in pixels, of the buttons.
	// Remarks:
	//     The button size must always be at least as large as the bitmap
	//     size it encloses. This function must be called only before adding
	//     any bitmaps to the command bar. If the application does not explicitly
	//     set the button size, it is set to the default size.
	//-----------------------------------------------------------------------
	void SetButtonSize(CSize size);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the command bar button.
	// Returns:
	//     The width and height values of the command bar button.
	//-----------------------------------------------------------------------
	virtual CSize GetButtonSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show the caption below control's icon.
	// Parameters:
	//     bTextBelow - TRUE to show the caption below the icon.
	//-----------------------------------------------------------------------
	void ShowTextBelowIcons(BOOL bTextBelow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks whether the caption is below the control's icon.
	// Returns:
	//     TRUE if the caption is below the icon, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsTextBelowIcons() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to pop-up the command bar.
	// Parameters:
	//     pControlPopup - Points to a CXTPControlPopup object.
	//     bSelectFirst  - TRUE to select the first item.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the current focused control.
	// Returns:
	//     Window handle of the focused control.
	//-----------------------------------------------------------------------
	HWND GetTrackFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set focus to the window.
	// Parameters:
	//     hwnd - Window handle of the focused control
	//-----------------------------------------------------------------------
	void SetTrackFocus(HWND hwnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the visibility state of the control.
	// Parameters:
	//     bVisible - TRUE to set control visible.
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a Boolean value indicating if the command bar is visible.
	// Returns:
	//     TRUE if the command bar is visible; FALSE if the command bar is hidden.
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the position of the command bar.
	// Parameters:
	//     barPosition - Position of the command bar.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetPosition(XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the control with the specified
	//     accelerator character.
	// Parameters:
	//     chAccel - Character to find.
	// Returns:
	//     Index of the control with the specified accelerator character.
	//-----------------------------------------------------------------------
	int FindAccel(TCHAR chAccel) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the control with the specified
	//     accelerator character.
	// Parameters:
	//     chAccel     - Character to find.
	//     bSelectOnly - TRUE to select only.
	// Returns:
	//     Index of the control with the specified accelerator character.
	//-----------------------------------------------------------------------
	int FindAccelEx(UINT chAccel, BOOL& bSelectOnly) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the command bars are in customize mode.
	// Returns:
	//     TRUE if the command bars are in customize mode, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCustomizeMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a control is entered into the
	//     command bar.
	// Parameters:
	//     pDataObject - Pointer to a CXTPControl object.
	//     point       - Point to test.
	// Returns:
	//     DROPEFFECT enumerator.
	//-----------------------------------------------------------------------
	virtual DROPEFFECT OnCustomizeDragEnter(CXTPControl* pDataObject, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control leaves the command bar.
	//-----------------------------------------------------------------------
	virtual void OnCustomizeDragLeave();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when control is over the command bar.
	// Parameters:
	//     pDataObject - Pointer to a CXTPControl object.
	//     point       - Point to test.
	// Returns:
	//     DROPEFFECT enumerator.
	//-----------------------------------------------------------------------
	virtual DROPEFFECT OnCustomizeDragOver(CXTPControl* pDataObject, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is dropped to the command bar.
	// Parameters:
	//     pDataObject - Pointer to a CXTPControl object.
	//     dropEffect  - DROPEFFECT enumerator.
	//     ptDrop      - Point where the user dropped the control.
	//     ptDrag      - Point where the user started dragging the control.
	//-----------------------------------------------------------------------
	virtual void OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop,
								 CPoint ptDrag);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the command bar is customizable.
	// Returns:
	//     TRUE if the command bar is customizable, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the command bar is present in Customize dialog.
	// Returns:
	//     TRUE if the command bar is present in Customize dialog, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCustomizeDialogPresent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable customization of the command bar.
	// Parameters:
	//     bEnable - TRUE to enable customization, FALSE to disable customization.
	//-----------------------------------------------------------------------
	void EnableCustomization(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to recalculate the command bar layout.
	//-----------------------------------------------------------------------
	virtual void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove the tracking state of the command bar.
	//-----------------------------------------------------------------------
	virtual void OnTrackLost();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to track the command bar.
	//-----------------------------------------------------------------------
	virtual void OnTrackEnter();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the owner window of the command bar.
	// Returns:
	//     A pointer to a CWnd object.
	//-----------------------------------------------------------------------
	CWnd* GetOwnerSite() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw all parent command bars.
	//-----------------------------------------------------------------------
	void InvalidateParents();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the current behavior modification
	//     flags of the command bar.
	// Returns:
	//     Flags of the command bar that are currently in effect.
	//-----------------------------------------------------------------------
	DWORD GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the identifier of the command bar.
	// Returns:
	//     Identifier of the command bar.
	//-----------------------------------------------------------------------
	UINT GetBarID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the identifier of the command bar.
	// Parameters:
	//     nID - Identifier to set.
	//-----------------------------------------------------------------------
	void SetBarID(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a child control becomes selected.
	// Parameters:
	//     bSelected - TRUE when control becomes selected.
	//     pControl  - Pointer to a CXTPControl object.
	//-----------------------------------------------------------------------
	virtual void OnControlSelected(BOOL bSelected, CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the description context.
	// Parameters:
	//     strDescription - Description string.
	// Returns:
	//     TRUE if successful, otherwise FALSE
	//-----------------------------------------------------------------------
	BOOL SetStatusText(const CString& strDescription);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the command bar.
	// Parameters:
	//     dwData - Contains the new 32-bit value to associate with the command bar.
	// See Also:
	//     GetCommandBarData
	//-----------------------------------------------------------------------
	void SetCommandBarData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied, 32-bit value
	//     associated with the command bar.
	// Returns:
	//     The 32-bit value associated with the command bar.
	// See Also:
	//     SetCommandBarData
	//-----------------------------------------------------------------------
	DWORD_PTR GetCommandBarData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the parent command bar.
	// Returns:
	//     A pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	virtual CXTPCommandBar* GetParentCommandBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the top-level parent command bar.
	// Returns:
	//     A pointer to the top-level CXTPCommandBar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetRootParent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the paint manager.
	// Returns:
	//     A pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image manager.
	// Returns:
	//     A pointer to a CXTPImageManager object.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the floated or pop-up toolbar.
	// Parameters:
	//     nWidth - Width of the floated or pop-up toolbar.
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the floated or pop-up toolbar.
	// Returns:
	//     The width, in pixels, of the floated or pop-up toolbar.
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified paint manager.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPPaintManager object to be set.
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified image manager.
	// Parameters:
	//     pImageManager - Pointer to a CXTPImageManager object to be set.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the command bar layout
	//     is Right-to-Left (RTL).
	// Returns:
	//     TRUE if the command bar layout is RTL, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the command bar is a
	//     CXTPDialogBar.
	// Returns:
	//     TRUE if the command bar is a CXTPDialogBar, otherwise FALSE.
	// See Also:
	//     CXTPToolBar, CXTPDialogBar
	//-----------------------------------------------------------------------
	virtual BOOL IsDialogBar() const;

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
	//     Call this member function to determine if the command bar is a
	//     CXTPPopupBar.
	// Returns:
	//     TRUE if the command bar is a CXTPPopupBar, otherwise FALSE.
	// See Also:
	//     CXTPCommandBar, CXTPPopupBar
	//-----------------------------------------------------------------------
	virtual BOOL IsPopupBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if underlines are visible in the command bar control.
	// Return:
	//     TRUE if underlines are visible in the command bar control, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsKeyboardCuesVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called internally to prevent the command bar from
	//     grabbing edit focus.
	// Parameters:
	//     bGrabFocus - FALSE to prevent the command bar from grabbing edit focus.
	//-----------------------------------------------------------------------
	void SetGrabFocus(BOOL bGrabFocus);

	//-------------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to redraw the command bar.
	// Parameters:
	//     lpRect - The rectangular area of the bar that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-------------------------------------------------------------------------
	virtual void Redraw(LPCRECT lpRect = 0, BOOL bAnimate = TRUE);

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
	//     Call this method to enable animation for the command bar.
	// Parameters:
	//     bEnable - TRUE to enable animation, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableAnimation(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable double buffer for toolbar drawing.
	// Parameters:
	//     bEnable - TRUE to enable double buffer for toolbar drawing.
	// See Also:
	//     EnableAnimation
	//-----------------------------------------------------------------------
	void EnableDoubleBuffer(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if animation is enabled for the specified command bar.
	// Returns:
	//     TRUE if animation is enabled, otherwise FALSE.
	// See Also:
	//     EnableAnimation
	//-----------------------------------------------------------------------
	BOOL IsAnimationEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during
	//     message reflection.
	// Parameters:
	//     hWnd     - Window handle message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam   - Specifies additional message-dependent information.
	//     lParam   - Specifies additional message-dependent information.
	//     lResult  - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	//-----------------------------------------------------------------------
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam,
							  LRESULT& lResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to ensure that a specified control is visible on the toolbar.
	// Parameters:
	//     pControl - Pointer to the CXTPControl object whose visibility must be ensured.
	//-----------------------------------------------------------------------
	virtual void EnsureVisible(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the default button style of the command bar.
	// Returns:
	//     XTPButtonStyle value representing the default button style.
	// See Also:
	//     SetDefaultButtonStyle
	//-----------------------------------------------------------------------
	XTPButtonStyle GetDefaultButtonStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default button style of the command bar.
	// Parameters:
	//     buttonStyle - XTPButtonStyle value representing the default button style.
	// See Also:
	//     GetDefaultButtonStyle
	//-----------------------------------------------------------------------
	void SetDefaultButtonStyle(XTPButtonStyle buttonStyle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to freeze redrawing until UnlockRedraw is called.
	// See Also:
	//     UnlockRedraw
	//-------------------------------------------------------------------------
	void LockRedraw();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to unfreeze redrawing.
	// See Also:
	//     LockRedraw
	//-------------------------------------------------------------------------
	void UnlockRedraw();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the visibility of the command bar's gripper.
	// Parameters:
	//     bShow - TRUE to show the gripper, FALSE to hide the gripper.
	// See Also:
	//     GetShowGripper
	//-----------------------------------------------------------------------
	void SetShowGripper(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the command bar's gripper is visible.
	// Returns:
	//     TRUE if the gripper is visible, otherwise FALSE.
	// See Also:
	//     SetShowGripper
	//-----------------------------------------------------------------------
	BOOL GetShowGripper() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the border margins of the command bar.
	// Returns:
	//     A CRect object containing the border margins of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetBorders();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background of the command bar.
	// Parameters:
	//     pDC - Pointer to the device context to draw.
	// See Also:
	//     DrawCommandBar
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the pop-up gallery can be resized by the user.
	// Returns:
	//     TRUE if the pop-up gallery can be resized by the user, otherwise FALSE.
	// See Also:
	//     XTPGalleryResizable
	//-----------------------------------------------------------------------
	virtual BOOL IsResizable() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if keyboard tips are visible for the command bar.
	// Returns:
	//     TRUE if keyboard tips are visible for the command bar, otherwise FALSE.
	// See Also:
	//     CreateKeyboardTips
	//-----------------------------------------------------------------------
	BOOL IsKeyboardTipsVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to create a keyboard tips collection for the command bar.
	// See Also:
	//     IsKeyboardTipsVisible
	//-------------------------------------------------------------------------
	virtual void CreateKeyboardTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses a key while keyboard tips are visible.
	// Parameters:
	//     chAccel - Virtual key.
	// See Also:
	//     OnKeyboardTip
	//-----------------------------------------------------------------------
	void OnKeyboardTipChar(UINT chAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a keyboard tip is found for the command bar.
	// Parameters:
	//     pTip - Keyboard tip that was found for the commandbar.
	// See Also:
	//     OnKeyboardTipChar
	//-----------------------------------------------------------------------
	virtual void OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to hide keyboard tips.
	// See Also:
	//     IsKeyboardTipsVisible
	//-------------------------------------------------------------------------
	void HideKeyboardTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     Locks the mouse cursor for a specified control.
	//-----------------------------------------------------------------------
	void LockMouseMove(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unlocks the mouse cursor.
	//-----------------------------------------------------------------------
	void UnlockMouseMove();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     TRUE if the key was handled, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either
	//                horizontal or vertical depending on dwMode.
	//      dwMode  - See CControlBar::CalcDynamicLayout for the list of
	//                supported flags.
	// Returns:
	//     The size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process special keys.
	// Parameters:
	//     key - Special keys enumerator. Can be any of the values listed in
	//           the Remarks section.
	// Remarks:
	//     CXTPCommandBar maps key passed as parameter of OnKeyDown to XTPSpecialKey
	//     enumerator and call ProcessSpecialKey.<p/> The key parameter can be one
	//     of the following:
	//         * <b>xtpKeyNext</b>: Indicates that the RIGHT or DOWN key was pressed.
	//                              Next control must be selected.
	//         * <b>xtpKeyPrev</b>: Indicates that the LEFT or UP key was pressed.
	//                              Previous control must be selected.
	//         * <b>xtpKeyBack</b>: Indicates that the LEFT key was pressed for pop-up bar.
	//                              Current sub-menu must be closed.
	//         * <b>xtpKeyPopup</b>: Indicates that the RIGHT key was pressed for pop-up bar.
	//                               Sub-menu must be opened.
	//         * <b>xtpKeyEscape</b>: Indicates that the ESCAPE key was pressed.
	//         * <b>xtpKeyReturn</b>: Indicates that the RETURN key was pressed.
	//         * <b>xtpKeyHome</b>: Indicates that the HOME key was pressed.
	//                              First control must be selected.
	//         * <b>xtpKeyEnd</b>: Indicates that the END key was pressed.
	//                             Last control must be selected.
	// Returns:
	//     TRUE if the key was handled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL ProcessSpecialKey(XTPSpecialKey key);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the controls array is changed.
	//-----------------------------------------------------------------------
	virtual void OnControlsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine mouse hit code.
	// Parameters:
	//     pt - Position to test.
	// Returns:
	//     HTCLIENT if the pointer belongs to the client area, otherwise HTNOWHERE.
	//-----------------------------------------------------------------------
	virtual int OnMouseHitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the control's shortcuts.
	//-----------------------------------------------------------------------
	virtual void UpdateShortcuts();

public:
	BOOL IsDwmEnabled() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the active MDI document template.
	// Remarks:
	//     This method calls CXTPCommandBars::GetActiveDocTemplate.
	// Returns:
	//     Active document template identifier.
	// See Also:
	//     CXTPCommandBars::GetActiveDocTemplate
	//-----------------------------------------------------------------------
	UINT GetActiveDocTemplate() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign self identifiers for the serialization process.
	// Parameters:
	//     nID             - Identifier to assign.
	//     pCommandBarList - List of CommandBars.
	//     pParam          - Address of a XTP_COMMANDBARS_PROPEXCHANGE_PARAM structure.
	//-----------------------------------------------------------------------
	virtual void GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList,
										XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called in the serialization process to restore pop-ups
	//     from a list of command bars.
	// Parameters:
	//     pCommandBarList - List of command bars.
	//-----------------------------------------------------------------------
	virtual void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the parent frame is in help mode.
	//-----------------------------------------------------------------------
	BOOL IsHelpMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a toolbar is removed from the command bars collection.
	//-----------------------------------------------------------------------
	virtual void OnRemoved();

	virtual BOOL IsControlEnabled(const CXTPControl* pControl) const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void OnPaintManagerDestroyed(CXTPPaintManager* pPaintManager);
	virtual void CustomizeFindDropIndex(CXTPControl* pDataObject, const CPoint& point,
										CRect& rcMarker, int& nDropIndex, BOOL& bDropAfter);
	void CustomizeDrawMarker(CDC* pDC);
	void UpdateDocTemplateControls();
	BOOL IsMouseLocked() const;
	CSize GetAutoIconSize(BOOL bLarge) const;
	//}}AFX_CODEJOCK_PRIVATE

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
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPCommandBar)
public:
	virtual void OnFinalRelease();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	void FilterToolTipMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreviewAccel(UINT chAccel);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCommandBar)
	afx_msg void OnUpdateCmdUI();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnPopupRButtonUp(CXTPCommandBar* pCommandBar, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnHelpHitTest(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

public:
	int m_nStateFlags;				// State of the command bars.
	static int s_nHoverDelay;		// Hover delay time
	static int s_nExpandDelay;		// Expand delay time
	static int s_nExpandHoverDelay; // Expand hover delay
	BOOL m_bComboBar;				// TRUE if it is combobox list
	BOOL m_bTrackOnHover;			// TRUE to select item if mouse stay over control

protected:
	BOOL m_bGrabFocus;				 // TRUE to grab current focus.
	int m_nMRUWidth;				 // Recent width.
	int m_nSelected;				 // Selected control index.
	int m_nPopuped;					 // Pop-up'ed control index.
	HWND m_hwndFocus;				 // The handle of the window that currently has focus.
	UINT m_nBarID;					 // Command bar's identifier.
	BOOL m_bTracking;				 // TRUE if the command bar is in tracking mode.
	CWnd* m_pSite;					 // Command bar's site.
	DWORD m_nIdleFlags;				 // Idle flags.
	DWORD m_dwFlags;				 // Command bar's flags.
	DWORD m_dwStyle;				 // Command bar's styles.
	CString m_strTitle;				 // Command bar's caption.
	XTPBarType m_barType;			 // Type of the command bar.
	CXTPControls* m_pControls;		 // A pointer to controls collection.
	XTPBarPosition m_barPosition;	// Position of the command bar.
	CXTPCommandBars* m_pCommandBars; // parent command bars class.
	CWnd* m_pOwner;					 // Owner window.
	CSize m_szIcons;				 // Size of the command bar's icons.
	CSize m_szButtons;				 // Size of the command bar's buttons.
	BOOL m_bTextBelow;				 // TRUE if text is shown below the CommandBar buttons.
	BOOL m_bExecOnRButton;			 // TRUE if the control button is executed on a right-click
	BOOL m_bIgnoreUpdateHandler;	 // TRUE to skip update handler for all child controls
	BOOL m_bRecursePopup;   // TRUE if pop-up bar was opened recursively. See TPM_RECURSE flag
							// description.
	int m_nLockRecurse;		// TRUE if another pop-up bar is opened recursively.
	BOOL m_bKeyboardSelect; // TRUE if last item was selected by keyboard.
	BOOL m_bCloseable;		// TRUE if the CommandBar can be closed and\or Hidden
	BOOL m_bCustomizable;   // TRUE if command bar is customizable.
	CRect m_rcMarker;		// Marker position.
	BOOL m_bVerticalMarker; // TRUE to draw marker vertically.
	DWORD_PTR m_dwData;		// The 32-bit value associated with the command bar.
	UINT* m_pReturnCmd;		// Return command holder.
	BOOL m_bCustomizeDialogPresent;	// TRUE if the command bar is present in Customize dialog.
	BOOL m_bAnimatePopup;			   // TRUE to animate pop-up bar.
	BOOL m_nLockRedraw;				   // TRUE if all drawing routines skipped
	CXTPPaintManager* m_pPaintManager; // Selected paint manager for command bar
	CXTPImageManager* m_pImageManager; // Selected image manager for command bar
	BOOL m_bMultiLine;				   // True if multi line commandbar
	XTPButtonStyle m_buttonStyle;	  // Default button Style
	XTPBarTextOrientation m_nTextOrientation; // Commandbar text orientation.
	CXTPControl* m_pMouseMoveLockControl;

	CXTPToolTipContext* m_pToolTipContext; // Tool tip Context.
	CXTPCommandBarAnimation* m_pAnimation; // Animation context
	CSize m_szButtonSpacing;			   // Spacing used between toolbar buttons
private:
	int m_nClickedControl;

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	DECLARE_XTP_COMMANDBAR(CXTPCommandBar)

	class CCommandBarCmdUI;

	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

#	ifdef _XTP_COMMANDBARS_ACTIVEX

	DECLARE_DISPATCH_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCommandBar);

	afx_msg LPDISPATCH OleControls();
	afx_msg long OleGetType();
	afx_msg LPDISPATCH OleGetCommandBars();
	afx_msg long OleGetPosition();
	afx_msg void OleSetPosition(long);
	afx_msg long OleShowPopup(const VARIANT& Flags, const VARIANT& x, const VARIANT& y);
	afx_msg void OleSetIconSize(long cx, long cy);
	afx_msg int OleGetIconWidth();
	afx_msg int OleGetIconHeight();
	afx_msg void OleEnableDocking(long dwFlags);
	afx_msg void OleSetTearOffPopup(LPCTSTR strCaption, long nID, long nWidth);
	afx_msg LPDISPATCH OleFindControl(const VARIANT& varType, const VARIANT& nId,
									  const VARIANT& bVisible, const VARIANT& bRecursive);
	afx_msg LPDISPATCH OleParent();
	afx_msg BOOL OleBuiltIn();
	afx_msg void OleModifyStyle(long dwRemove, long dwAdd);
	afx_msg void OleGetWindowRect(long* pLeft, long* pTop, long* pRight, long* pBottom);
	afx_msg void OleSetPopupToolBar(BOOL bToolBarType);
	afx_msg LPDISPATCH OleGetControlPopup();
	afx_msg BOOL OleGetCustomizable();
	afx_msg void OleSetCustomizable(BOOL);
	afx_msg void OleRedrawBar();
	afx_msg void OleRecalcLayout();
	afx_msg void OleSetSelectedControl(LPDISPATCH);
	afx_msg LPDISPATCH OleGetSelectedControl();
	afx_msg HWND OleGetHandle();
	afx_msg BOOL OleGetVisible();
	afx_msg void OleSetVisible(BOOL bVisible);

public:
	void OlePixelToUnits(CRect& rc);
	void OleUnitsToPixel(CPoint& pt);
	BOOL m_bAutoMenuLoad;

	void OleFireTrackingModeChanged(int bMode);

	enum
	{
		dispidTitle			  = 1L,
		dispidControls		  = 2L,
		dispidType			  = 3L,
		dispidVisible		  = 4L,
		dispidPosition		  = 5L,
		dispidShowPopup		  = 6L,
		dispidSetIconSize	 = 7L,
		dispidEnableDocking   = 8L,
		dispidSetTearOffPopup = 9L,
		dispidBarID			  = 10L,
		dispidFindControl	 = 11L,
		dispidParent		  = 12L,
		dispidBuiltIn		  = 13L,
		dispidDelete		  = 14L,
		dispidModifyStyle	 = 16L,
		dispidGetWindowRect   = 17L,
		dispidSetFlags		  = 18L
	};

public:
	static CXTPCommandBar* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

#	endif /*_XTP_COMMANDBARS_ACTIVEX*/

	friend class CXTPCommandBars;
	friend class CXTPDockBar;
	friend class CCommandBarCmdUI;
	friend class CXTPControl;
	friend class CXTPControls;
	friend class CXTPControlPopup;
	friend class CXTPToolBar;
	friend class CXTPMouseManager;
	friend class CXTPPopupBar;
	friend class CXTPCustomizeSheet;
	friend class CXTPPaintManager;
	friend class CXTPMiniToolBar;

	//}}AFX_CODEJOCK_PRIVATE
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTPCommandBar::IsTrackingMode() const
{
	return m_bTracking;
}
AFX_INLINE CXTPControls* CXTPCommandBar::GetControls() const
{
	return m_pControls;
}
AFX_INLINE XTPBarType CXTPCommandBar::GetType() const
{
	return m_barType;
}
AFX_INLINE XTPBarPosition CXTPCommandBar::GetPosition() const
{
	return m_barPosition;
}
AFX_INLINE void CXTPCommandBar::DelayLayout()
{
	m_nIdleFlags |= xtpIdleLayout;
}

#	ifndef _XTP_COMMANDBARS_ACTIVEX
AFX_INLINE void CXTPCommandBar::DelayRedraw()
{
	m_nIdleFlags |= xtpIdleRedraw;
}
#	endif

#	ifdef _XTP_COMMANDBARS_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
AFX_INLINE void CXTPCommandBar::DelayRedraw()
{
	Redraw();
}
//}}AFX_CODEJOCK_PRIVATE
#	endif

AFX_INLINE void CXTPCommandBar::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = lpszTitle;
	Redraw();
}
AFX_INLINE CString CXTPCommandBar::GetTitle() const
{
	return m_strTitle;
}
AFX_INLINE void CXTPCommandBar::SetCommandBars(CXTPCommandBars* pCommandBars)
{
	m_pCommandBars = pCommandBars;
}
AFX_INLINE HWND CXTPCommandBar::GetTrackFocus() const
{
	return m_hwndFocus;
}
AFX_INLINE void CXTPCommandBar::SetTrackFocus(HWND hwnd)
{
	m_hwndFocus = hwnd;
}
AFX_INLINE void CXTPCommandBar::SetVisible(BOOL /*bVisible*/)
{
}
AFX_INLINE BOOL CXTPCommandBar::IsVisible() const
{
	return FALSE;
}
AFX_INLINE BOOL CXTPCommandBar::SetPosition(XTPBarPosition /*barPosition*/)
{
	return FALSE;
}
AFX_INLINE CSize CXTPCommandBar::CalcDynamicLayout(int, DWORD /*nMode*/)
{
	return CSize(0, 0);
}
AFX_INLINE CXTPCommandBar* CXTPCommandBar::GetParentCommandBar() const
{
	return NULL;
}
AFX_INLINE void CXTPCommandBar::OnRecalcLayout()
{
}
AFX_INLINE BOOL CXTPCommandBar::IsCustomizable() const
{
	return m_bCustomizable;
}
AFX_INLINE BOOL CXTPCommandBar::IsCustomizeDialogPresent() const
{
	return m_bCustomizeDialogPresent;
}
AFX_INLINE BOOL CXTPCommandBar::IsDialogBar() const
{
	return FALSE;
}
AFX_INLINE void CXTPCommandBar::EnableCustomization(BOOL bEnable)
{
	if (!IsDialogBar())
		m_bCustomizable = bEnable;
}

AFX_INLINE void CXTPCommandBar::SetIconSize(CSize size)
{
	m_szIcons = size;
}
AFX_INLINE void CXTPCommandBar::SetButtonSize(CSize size)
{
	m_szButtons = size;
}

AFX_INLINE DWORD CXTPCommandBar::GetFlags() const
{
	return m_dwFlags;
}

AFX_INLINE UINT CXTPCommandBar::GetBarID() const
{
	return m_nBarID;
}
AFX_INLINE void CXTPCommandBar::SetCommandBarData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPCommandBar::GetCommandBarData() const
{
	return m_dwData;
}
AFX_INLINE void CXTPCommandBar::LockRedraw()
{
	m_nLockRedraw++;
}
AFX_INLINE void CXTPCommandBar::UnlockRedraw()
{
	m_nLockRedraw--;
	if (m_nLockRedraw == 0 && (m_nIdleFlags & xtpIdleRedraw))
		Redraw();
}
AFX_INLINE void CXTPCommandBar::SetGrabFocus(BOOL bGrabFocus)
{
	m_bGrabFocus = bGrabFocus;
}
AFX_INLINE BOOL CXTPCommandBar::PreviewAccel(UINT)
{
	return FALSE;
}
AFX_INLINE BOOL CXTPCommandBar::IsRibbonBar() const
{
	return FALSE;
}
AFX_INLINE BOOL CXTPCommandBar::IsPopupBar() const
{
	return FALSE;
}
AFX_INLINE void CXTPCommandBar::SetBarID(UINT nID)
{
	m_nBarID = nID;
}
AFX_INLINE XTPButtonStyle CXTPCommandBar::GetDefaultButtonStyle() const
{
	return m_buttonStyle;
}

AFX_INLINE void CXTPCommandBar::SetDefaultButtonStyle(XTPButtonStyle buttonStyle)
{
	m_buttonStyle = buttonStyle;
}
AFX_INLINE BOOL CXTPCommandBar::IsResizable() const
{
	return FALSE;
}
AFX_INLINE void CXTPCommandBar::OnPopupRButtonUp(CXTPCommandBar* /*pCommandBar*/, CPoint /*point*/)
{
}
AFX_INLINE CXTPControl* CXTPCommandBar::GetSelected() const
{
	return GetControl(m_nSelected);
}
AFX_INLINE void CXTPCommandBar::SetTextOrientation(XTPBarTextOrientation nTextOrientation)
{
	m_nTextOrientation = nTextOrientation;
	DelayLayout();
}
AFX_INLINE XTPBarTextOrientation CXTPCommandBar::GetTextOrientation() const
{
	return m_nTextOrientation;
}
AFX_INLINE void CXTPCommandBar::SetOwner(CWnd* pWnd)
{
	m_pOwner = pWnd;
}
AFX_INLINE BOOL CXTPCommandBar::IsControlEnabled(const CXTPControl* /*pControl*/) const
{
	return (BOOL)-1;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCOMMANDBAR_H__)
