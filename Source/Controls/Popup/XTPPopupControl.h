// XTPPopupControl.h: interface for the CXTPPopupControl class.
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
#if !defined(_XTPPOPUPCONTROL_H__)
#	define _XTPPOPUPCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPopupItem;
class CXTPPopupPaintManager;
class CXTPImageManager;
class CXTPImageManagerIcon;
class CXTPMarkupContext;

//---------------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the pop-up window theme in use.
// Example:
//     <code>m_wndPopupControl.SetTheme(xtpPopupThemeOffice2000);</code>
// See Also:
//     CXTPPopupControl::SetTheme, CXTPPopupControl::GetTheme
//
// <KEYWORDS xtpPopupThemeOffice2000, xtpPopupThemeOfficeXP, xtpPopupThemeOffice2003,
// xtpPopupThemeMSN, xtpPopupThemeCustom, xtpPopupThemeOffice2013, xtpPopupThemeOffice2016>
//---------------------------------------------------------------------------
enum XTPPopupPaintTheme
{
	xtpPopupThemeOffice2000,						  // Office 2000 pop-up theme.
	xtpPopupThemeOfficeXP,							  // Office XP pop-up theme.
	xtpPopupThemeOffice2003,						  // Office 2003 pop-up theme.
	xtpPopupThemeMSN,								  // MSN Messenger pop-up theme.
	xtpPopupThemeResource,							  // A resource based theme.
	xtpPopupThemeCustom,							  // Custom pop-up theme.
	xtpPopupThemeOffice2013,						  // Office 2013 pop-up theme.
	xtpPopupThemeOffice2016 = xtpPopupThemeOffice2013 // Office 2016 pop-up theme.
};

//{{AFX_CODEJOCK_PRIVATE
#	define xtpPopupThemeOffice2007 xtpPopupThemeResource
//}}AFX_CODEJOCK_PRIVATE

//---------------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the location where the pop-up control is displayed.
//---------------------------------------------------------------------------
enum XTPPopupLocation
{
	xtpPopupLocationNearTaskBar, // Displays in the bottom right of the screen right above the
								 // taskbar.
	xtpPopupLocationBottomRight, // Displays in the bottom right of the screen.
	xtpPopupLocationCenter		 // Displays in the center of the screen.
};
//---------------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the pop-up window animation in use.
// Example:
//     <code>m_wndPopupControl.SetPopupAnimation(xtpPopupAnimationFade);</code>
// See Also:
//     CXTPPopupControl::SetPopupAnimation, CXTPPopupControl::GetPopupAnimation
//
// <KEYWORDS xtpPopupAnimationNone, xtpPopupAnimationFade, xtpPopupAnimationSlide,
// xtpPopupAnimationUnfold>
//---------------------------------------------------------------------------
enum XTPPopupAnimation
{
	xtpPopupAnimationNone,  // Animation is not used.
	xtpPopupAnimationFade,  // Fade method animation.
	xtpPopupAnimationSlide, // Slide method animation.
	xtpPopupAnimationUnfold // Unfold method animation.
};

//---------------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the current pop-up window state.
// Example:
// <code>
// if (pControl->GetPopupState() == xtpPopupStateClosed)
// {
//     EnableItems(TRUE);
// }
// </code>
// See Also:
//     CXTPPopupControl::GetPopupState, CXTPPopupControl::SetPopupState
//
// <KEYWORDS xtpPopupStateClosed, xtpPopupStateExpanding, xtpPopupStateShow,
// xtpPopupStateCollapsing>
//---------------------------------------------------------------------------
enum XTPPopupState
{
	xtpPopupStateClosed,	// The pop-up window is closed.
	xtpPopupStateExpanding, // The pop-up window is expanding.
	xtpPopupStateShow,		// The pop-up window is shown.
	xtpPopupStateCollapsing // The pop-up window is collapsing.
};

//---------------------------------------------------------------------------
// Summary:
//     The XTPWM_POPUPCONTROL_NOTIFY message is sent to the CXTPPopupControl
//     owner window whenever an action occurs within the CXTPPopupControl.
// Parameters:
//     nAction -  Value of wParam specifies a pop-up control value that
//                indicates the user's request. Can be any of the values
//                listed in the Remarks section.
// Remarks:
//     nAction parameter can be one of the following:
//     * <b>XTP_PCN_ITEMCLICK</b>: Indicates that the user clicked on the
//                                 pop-up control item.
//     * <b>XTP_PCN_STATECHANGED</b>: Indicates that the state of the
//                                    control changed.
// Returns:
//     If the application is to process this message, then the return value should be TRUE.
//     Otherwise, the return value is FALSE.
// Example:
//     Here is an example of how an application would process the XTPWM_POPUPCONTROL_NOTIFY
//     message:
// <code>
//
// BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(XTPWM_POPUPCONTROL_NOTIFY, OnPopUpNotify)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnPopUpNotify(WPARAM wParam, LPARAM lParam)
// {
//      if (wParam == XTP_PCN_ITEMCLICK)
//      {
//          CXTPPopupItem* pItem = (CXTPPopupItem*)lParam;
//          ASSERT(pItem);
//
//          m_wndPopupControl.Close();
//
//          if (pItem->GetID() == ID_GOTO_SITE)
//          {
//              ::ShellExecute(NULL, _T("open"), _T("http://www.codejock.com/"), NULL, NULL,
//              SW_SHOW);
//          }
//      }
//      else if (wParam == XTP_PCN_STATECHANGED)
//      {
//          CXTPPopupControl* pControl = (CXTPPopupControl*)lParam;
//          ASSERT(pControl);
//
//          if (pControl->GetPopupState() == xtpPopupStateClosed)
//          {
//              EnableItems(TRUE);
//          }
//      }
//      return TRUE;
//  }
// </code>
// See Also:
//     XTP_PCN_ITEMCLICK, XTP_PCN_STATECHANGED, CXTPPopupControl
//---------------------------------------------------------------------------
const UINT XTPWM_POPUPCONTROL_NOTIFY = (WM_XTP_CONTROLS_BASE + 37);

//---------------------------------------------------------------------------
// Summary:
//     XTP_PCN_ITEMCLICK is used to indicate that the user clicked on a
//     pop-up control item.
// Remarks:
//     XTP_PCN_ITEMCLICK is sent in the XTPWM_POPUPCONTROL_NOTIFY message to
//     the owner window when the user has clicked on a pop-up control item.
// Example:
//   See example of XTPWM_POPUPCONTROL_NOTIFY.
// See Also:
//     XTPWM_POPUPCONTROL_NOTIFY, XTP_PCN_STATECHANGED
//---------------------------------------------------------------------------
const UINT XTP_PCN_ITEMCLICK = 1;

//---------------------------------------------------------------------------
// Summary:
//     XTP_PCN_STATECHANGED is used to indicate that the state of the
//     pop-up control has changed.
// Remarks:
//     XTP_PCN_STATECHANGED is sent in the XTPWM_POPUPCONTROL_NOTIFY message
//     to the owner window when the state of the pop-up control has changed.
//     It can be one of the following states:
//         * <b>xtpPopupStateClosed</b>: Indicates that the pop-up window is closed.
//         * <b>xtpPopupStateExpanding</b>: Indicates that the pop-up window is expanding.
//         * <b>xtpPopupStateShow</b>: Indicates that the pop-up window is shown.
//         * <b>xtpPopupStateCollapsing</b>: Indicates that the pop-up window is collapsing.
// Example:
//   See example of XTPWM_POPUPCONTROL_NOTIFY.
// See Also:
//     XTPWM_POPUPCONTROL_NOTIFY, XTP_PCN_ITEMCLICK
//---------------------------------------------------------------------------
const UINT XTP_PCN_STATECHANGED = 2;

//---------------------------------------------------------------------------
// Summary:
//     XTP_PCN_POSCHANGED is used to indicate that the user moved the
//     pop-up control with the mouse.
// See Also:
//     XTPWM_POPUPCONTROL_NOTIFY
//---------------------------------------------------------------------------
const UINT XTP_PCN_POSCHANGED = 3;

//===========================================================================
// Summary:
//     CXTPPopupControl is a class for control of a pop-up window.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupControl : public CWnd
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Pop-up state descriptor.
	// See Also:
	//     CXTPPopupControl::m_stateTarget, CXTPPopupControl::m_stateCurrent
	//-----------------------------------------------------------------------
	struct POPUPSTATE
	{
		CRect rcPopup;	 // Pop-up position.
		int nTransparency; // Pop-up transparency.
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupControl object.
	// Parameters:
	//     bAutoDelete - If TRUE, then the object will be deleted automatically
	//                   upon final release.
	//-----------------------------------------------------------------------
	CXTPPopupControl(BOOL bAutoDelete = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupControl object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the position of a pop-up window.
	// Parameters:
	//     ptPopup - A CPoint object containing an XY location.
	//-----------------------------------------------------------------------
	void SetPopupPos(CPoint ptPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the position of a pop-up window.
	// Returns:
	//     A CPoint object containing an XY location.
	//-----------------------------------------------------------------------
	CPoint GetPopupPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the size of a pop-up window.
	// Parameters:
	//     szPopup - A CSize object containing an XY size.
	//-----------------------------------------------------------------------
	void SetPopupSize(CSize szPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of a pop-up window.
	// Returns:
	//     A CSize object containing an XY size.
	//-----------------------------------------------------------------------
	CSize GetPopupSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create and expand a pop-up window.
	// Parameters:
	//     pParent - Pointer to a CWnd object that represents the parent
	//               window.
	//     nID     - ID for the pop-up window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Show(CWnd* pParent, int nID = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show a modal pop-up control.
	// Parameters:
	//     pParent - Pointer to a CWnd object that represents the parent
	//               window.
	//     nID     - ID for pop-up window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ShowModal(CWnd* pParent, int nID = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to collapse a pop-up window.
	//-----------------------------------------------------------------------
	void Hide();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to destroy a pop-up window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Close();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the transparency value of a
	//     pop-up window.
	// Parameters:
	//     bTransp - Value of transparency (0...255).
	//               If bTransp = 0, then the pop-up window is fully transparent,
	//               if bTransp = 255, then the pop-up window is fully opaque.
	//-----------------------------------------------------------------------
	void SetTransparency(BYTE bTransp);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the transparency value of a
	//     pop-up window.
	// Returns:
	//     Transparency value of a pop-up window.
	//-----------------------------------------------------------------------
	BYTE GetTransparency() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the show delay value for a
	//     pop-up window.
	// Parameters:
	//     dwShowDelay - Show delay value in milliseconds.
	//-----------------------------------------------------------------------
	void SetShowDelay(DWORD dwShowDelay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the show delay value for a
	//     pop-up window.
	// Returns:
	//     Show delay value in milliseconds.
	//-----------------------------------------------------------------------
	DWORD GetShowDelay() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the animate delay value for a
	//     pop-up window.
	// Parameters:
	//     dwAnimateDelay - Animate delay value in milliseconds.
	//-----------------------------------------------------------------------
	void SetAnimateDelay(DWORD dwAnimateDelay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the animate delay value for a
	//     pop-up window.
	// Returns:
	//     Animate delay value in milliseconds.
	//-----------------------------------------------------------------------
	DWORD GetAnimateDelay() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the pop-up animation method.
	// Parameters:
	//     popupAnimation - Method to use. See XTPPopupAnimation enum.
	//-----------------------------------------------------------------------
	void SetPopupAnimation(XTPPopupAnimation popupAnimation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the default pop-up animation method.
	//-----------------------------------------------------------------------
	void SetPopupAnimation();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the pop-up animation method.
	// Returns:
	//     Method in use. See XTPPopupAnimation enum.
	//-----------------------------------------------------------------------
	XTPPopupAnimation GetPopupAnimation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the current state of a
	//     pop-up window.
	// Returns:
	//     Current state. See XTPPopupState enum.
	//-----------------------------------------------------------------------
	XTPPopupState GetPopupState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the paint theme or paint manager
	//     used for drawing a pop-up window.
	// Parameters:
	//     theme - Paint theme to use. See XTPPopupPaintTheme enum.
	//     pPaintManager - Pointer to the paint manager object to use.
	//-----------------------------------------------------------------------
	void SetTheme(XTPPopupPaintTheme theme);
	void SetTheme(
		CXTPPopupPaintManager* pPaintManager); // <combine
											   // CXTPPopupControl::SetTheme@XTPPopupPaintTheme>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the paint manager object
	//     used for drawing a pop-up window.
	// Returns:
	//     Pointer to the paint manager object.
	//-----------------------------------------------------------------------
	CXTPPopupPaintManager* GetPaintManager() const;
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the paint theme that the
	//     pop-up window is currently using.
	// Returns:
	//     The paint theme that the pop-up window is currently using.
	// See Also:
	//     SetTheme, XTPPopupPaintTheme
	//-----------------------------------------------------------------------
	XTPPopupPaintTheme GetTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to redraw all controlled items of a
	//     pop-up window.
	//-----------------------------------------------------------------------
	void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to install a new controlled item in
	//     a pop-up window.
	// Parameters:
	//     pItem - Pointer to a CXTPPopupItem object which will be installed
	//             in a pop-up window.
	// Returns:
	//     Pointer to the installed CXTPPopupItem object.
	//-----------------------------------------------------------------------
	CXTPPopupItem* AddItem(CXTPPopupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all controlled items from
	//     a pop-up window.
	//-----------------------------------------------------------------------
	void RemoveAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove a controlled item from
	//     a pop-up window.
	// Parameters:
	//     pItem  - Pointer to the CXTPPopupItem object which will be removed.
	//     nIndex - Index of the CXTPPopupItem object to remove from the control.
	//-----------------------------------------------------------------------
	void RemoveItem(CXTPPopupItem* pItem);
	void RemoveItem(int nIndex); // <combine CXTPPopupControl::RemoveItem@CXTPPopupItem*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the count of controlled items in
	//     a pop-up window.
	// Returns:
	//     Count of controlled items.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the controlled item at a
	//     specified index.
	// Parameters:
	//     nIndex - Index of the controlled item.
	// Returns:
	//     Pointer to a CXTPPopupItem object.
	//-----------------------------------------------------------------------
	CXTPPopupItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to test a point for controlled items
	//     of a pop-up window.
	// Parameters:
	//     pt - A CPoint object containing an XY location to test.
	// Returns:
	//     Pointer to a CXTPPopupItem object if the test is successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPopupItem* HitTest(CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the user can click and drag
	//     the pop-up window when it is displayed.
	// Parameters:
	//     bAllowMove - TRUE to allow the user to click and drag the pop-up
	//                  window anywhere on the screen. FALSE to prevent the
	//                  pop-up window from being moved by the user.
	//-----------------------------------------------------------------------
	void AllowMove(BOOL bAllowMove);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the bitmap index for the background.
	// Parameters:
	//     nBackgroundBitmap - The bitmap index for the background.
	//-----------------------------------------------------------------------
	void SetBackgroundBitmap(int nBackgroundBitmap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bitmap index for the background.
	// Returns:
	//     The bitmap index for the background.
	// See Also:
	//     SetBackgroundBitmap
	//-----------------------------------------------------------------------
	int GetBackgroundBitmap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow the pop-up control to delete itself
	//     automatically upon final release.
	// Parameters:
	//     bAutoDelete - If TRUE, then the object will be deleted automatically
	//                   upon final release.
	//-----------------------------------------------------------------------
	void SetAutoDelete(BOOL bAutoDelete);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the default pop-up location.
	// Parameters:
	//     nPopupLocation - Default Popup Location (Near TaskBar, BottomLeft or Center).
	// See Also:
	//     SetPopupPos
	//-----------------------------------------------------------------------
	void SetDefaultLocation(XTPPopupLocation nPopupLocation);

	BOOL GetSplashScreenMode() const;
	void SetSplashScreenMode(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the ID assigned to the pop-up window when it was created.
	// Returns:
	//     An integer value representing the pop-up window ID.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if right-to-left (RTL) mode is set.
	// Returns:
	//     TRUE if text is displayed using right-to-left (RTL)
	//     reading-order properties.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if right-to-left (RTL) mode should be set.
	// Parameters:
	//     bRightToLeft - TRUE to display text using right-to-left (RTL)
	//                    reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set a new image manager.
	// Parameters:
	//     pImageManager - A pointer to the CXTPImageManager object to be set.
	// Example:
	// <code>
	//     CXTPImageManager* pImageManager = new CXTPImageManager();
	//     pImageManager->SetIcons(IDR_MAINFRAME);
	//     m_wndPopupControl.SetImageManager(pImageManager);
	// </code>
	// See Also:
	//     GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the image manager of the pop-up
	//     control.
	// Returns:
	//     A pointer to the image manager of the pop-up control.
	// Remarks:
	//     The image manager is used to hold all of the icons displayed in the
	//     pop-up control.
	// See Also:
	//     SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for the pop-up control.
	// Parameters:
	//     bEnableMarkup - TRUE to enable markup.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the markup context associated with the pop-up control.
	// Returns:
	//     A pointer to a CXTPMarkupContext object.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//--------------------------------------------------------------------
	// Summary:
	//     Gets/sets DPI bitmap scaling flag. If set, all raster icons and bitmaps
	//     used in the control will be stretched automatically if DPI scaling
	//     factor for control's window isn't equal to 100%. The default value is
	//     derived from CXTPDpi::IsDpiBitmapScalingEnabled. Once changed the new value
	//     will be used.
	// Parameters:
	//     bEnable - TRUE to enable bitmaps scaling, FALSE to disable.
	// Returns:
	//     The current value of the DPI bitmap scaling flag for the control.
	//--------------------------------------------------------------------
	BOOL IsDpiBitmapScalingEnabled() const;
	void EnableDpiBitmapScaling(
		BOOL bEnable = TRUE); // <combine CXTPPopupControl::IsDpiBitmapScalingEnabled@const>

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a pop-up window.
	// Parameters:
	//     pParentWnd - Pointer to the parent window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks on a controlled item.
	// Parameters:
	//     pItem - A pointer to the item that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnClick(CXTPPopupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start the expanding animation.
	// Parameters:
	//     bUpdateCurrent - TRUE to update the current state.
	//-----------------------------------------------------------------------
	virtual void OnExpanding(BOOL bUpdateCurrent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start the collapsing animation.
	//-----------------------------------------------------------------------
	virtual void OnCollapsing();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to show the control.
	//-----------------------------------------------------------------------
	virtual void OnShow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to notify the parent about an action that
	//     occurred.
	// Parameters:
	//     wParam - The action that occurred.
	//     lParam - Extended parameter.
	//-----------------------------------------------------------------------
	virtual void Notify(WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method animates the pop-up control.
	// Parameters:
	//     nStep - Current step of the animation.
	//-----------------------------------------------------------------------
	void Animate(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the position in the animation according to the current
	//     animation step.
	// Parameters:
	//     bInit - Initial step of the animation.
	//-----------------------------------------------------------------------
	void UpdateState(BOOL bInit = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the current state of the control
	//     changes.
	// Parameters:
	//     popupState - Current state of the control.
	//-----------------------------------------------------------------------
	void SetPopupState(XTPPopupState popupState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the current control's transparency.
	// Parameters:
	//     bAlpha - Current alpha level of the window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetLayeredWindowAttributes(int bAlpha);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loop of control moving.
	//-----------------------------------------------------------------------
	void TrackMove();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CXTPPopupControl)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void UpdateBitmapRegion();
	HRGN BitmapToRegion(CXTPImageManagerIcon* pIcon);
	void SetRegionAlphaLayer(CXTPImageManagerIcon* pIcon);

protected:
	CXTPPopupPaintManager* m_pPaintManager;			   // Pointer to CXTPPopupPaintManager object.
	CArray<CXTPPopupItem*, CXTPPopupItem*> m_arrItems; // Array for controlled items.
	CXTPPopupItem* m_pSelected;						   // Pointer to selected item.
	CXTPPopupItem* m_pPressed;						   // Pointer to pressed item.
	XTPPopupAnimation m_popupAnimation;				   // Current animation method.
	XTPPopupState m_popupState;						   // Current pop-up state.
	CSize m_szPopup;								   // Current XY location of a pop-up window.
	CPoint m_ptPopup;								   // Current XY size of a pop-up window.
	int m_nID;										   // Control ID for the pop-up window.
	int m_nTransparency;			 // Current value transparency of a pop-up window.
	int m_nCurrentTransparency;		 // Current transparency of a pop-up window.
	UINT m_uShowDelay;				 // Value of show delay of a pop-up window.
	UINT m_uAnimationDelay;			 // Value of animation delay of a pop-up window.
	UINT m_nAnimationInterval;		 // Value of animation interval of a pop-up window.
	int m_nStep;					 // Value of step animation of a pop-up window.
	XTPPopupPaintTheme m_paintTheme; // Currently used theme.
	BOOL m_bAllowMove; // If TRUE, then the user can click and drag the pop-up window anywhere on
					   // the screen. If FALSE, then the pop-up window cannot be moved by the user.

	POPUPSTATE m_stateTarget;		   // Target descriptor.
	POPUPSTATE m_stateCurrent;		   // Current descriptor.
	CXTPImageManager* m_pImageManager; // Images of pop-up items.
	BOOL m_bRightToLeft;			   // Right-To-Left layout.

	CXTPMarkupContext* m_pMarkupContext; // Associated Markup Context.

	int m_nBackgroundBitmap;		   // Image index used as the background of the pop-up control.
	BOOL m_bAutoDelete;				   // TRUE to delete the pop-up after it will be closed.
	XTPPopupLocation m_nPopupLocation; // Default locations for the pop-up control to display.
	HCURSOR m_hHandCursor;			   // The system hand cursor for hyperlink items.

	BOOL m_bSplashScreenMode; // TRUE to ignore mouse movement.

private:
	typedef BOOL(WINAPI* PFNSETLAYEREDWINDOWATTRIBUTES)(HWND hwnd, COLORREF crKey, BYTE bAlpha,
														DWORD dwFlags);
	typedef BOOL(WINAPI* LPFNUPDATELAYEREDWINDOW)(HWND hwnd, HDC hdcDst, POINT* pptDst, SIZE* psize,
												  HDC hdcSrc, POINT* pptSrc, COLORREF crKey,
												  BLENDFUNCTION* pblend, DWORD dwFlags);

	PFNSETLAYEREDWINDOWATTRIBUTES m_pfnSetLayeredWindowAttributes; // Point to Transparency proc in
																   // USER32.dll module.
	LPFNUPDATELAYEREDWINDOW m_pfnUpdateLayeredWindow;
	BOOL m_bCapture; // Capture flag of mouse event.
	BOOL m_bLayered;
	BOOL m_bDpiBitmapScaling;
	BOOL m_bDpiBitmapScalingModified;

	friend class CXTPPopupItem;
	friend class CPopupControlCtrl;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPPopupControl::GetSplashScreenMode() const
{
	return m_bSplashScreenMode;
}
AFX_INLINE void CXTPPopupControl::SetSplashScreenMode(BOOL bSet)
{
	m_bSplashScreenMode = bSet;
}
AFX_INLINE CXTPPopupPaintManager* CXTPPopupControl::GetPaintManager() const
{
	return m_pPaintManager;
}
AFX_INLINE void CXTPPopupControl::SetPopupSize(CSize szPopup)
{
	m_szPopup = szPopup;
}
AFX_INLINE CSize CXTPPopupControl::GetPopupSize() const
{
	return m_szPopup;
}
AFX_INLINE void CXTPPopupControl::SetPopupPos(CPoint ptPopup)
{
	m_ptPopup = ptPopup;
}
AFX_INLINE void CXTPPopupControl::SetShowDelay(DWORD dwShowDelay)
{
	m_uShowDelay = dwShowDelay;
}
AFX_INLINE DWORD CXTPPopupControl::GetShowDelay() const
{
	return m_uShowDelay;
}
AFX_INLINE void CXTPPopupControl::SetAnimateDelay(DWORD dwAnimateDelay)
{
	m_uAnimationDelay = dwAnimateDelay;
}
AFX_INLINE DWORD CXTPPopupControl::GetAnimateDelay() const
{
	return m_uAnimationDelay;
}
AFX_INLINE BYTE CXTPPopupControl::GetTransparency() const
{
	return (BYTE)m_nTransparency;
}
AFX_INLINE void CXTPPopupControl::SetTransparency(BYTE nTransparency)
{
	m_nTransparency = nTransparency;
}
AFX_INLINE void CXTPPopupControl::SetAutoDelete(BOOL bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
}
AFX_INLINE XTPPopupAnimation CXTPPopupControl::GetPopupAnimation() const
{
	return m_popupAnimation;
}
AFX_INLINE void CXTPPopupControl::SetPopupAnimation(XTPPopupAnimation popupAnimation)
{
	m_popupAnimation = popupAnimation;
}
AFX_INLINE XTPPopupPaintTheme CXTPPopupControl::GetTheme() const
{
	return m_paintTheme;
}
AFX_INLINE void CXTPPopupControl::AllowMove(BOOL bAllowMove)
{
	m_bAllowMove = bAllowMove;
}
AFX_INLINE BOOL CXTPPopupControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
										 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
										 UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPPopupControl::IsLayoutRTL() const
{
	return m_bRightToLeft;
}
AFX_INLINE void CXTPPopupControl::SetBackgroundBitmap(int nBackgroundBitmap)
{
	m_nBackgroundBitmap = nBackgroundBitmap;
	UpdateBitmapRegion();
}
AFX_INLINE int CXTPPopupControl::GetBackgroundBitmap() const
{
	return m_nBackgroundBitmap;
}
AFX_INLINE CXTPMarkupContext* CXTPPopupControl::GetMarkupContext() const
{
	return m_pMarkupContext;
}
AFX_INLINE void CXTPPopupControl::SetDefaultLocation(XTPPopupLocation nPopupLocation)
{
	m_nPopupLocation = nPopupLocation;
}
AFX_INLINE int CXTPPopupControl::GetID() const
{
	return m_nID;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPPOPUPCONTROL_H__)
