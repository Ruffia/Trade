// XTPButton.h : interface for the CXTPButton class.
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
#if !defined(__XTPBUTTON_H__)
#	define __XTPBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPImageManager;
class CXTPImageManagerIcon;
class CXTPMarkupContext;
class CXTPMarkupUIElement;
class CXTPButtonTheme;
class CXTPImageManagerIconHandle;
class CXTPToolTipContext;

//-----------------------------------------------------------------------
// Summary:
//     Image and text relationship settings for a push button control.
// Remarks:
//     The CXTPButton::SetTextImageRelation method uses flags from the
//     XTPButtonTextImageRelation enumeration to set the text and image
//     relationship inside the button control.
//-----------------------------------------------------------------------
enum XTPButtonTextImageRelation
{
	xtpButtonOverlay,		  // Image is displayed over top of the text.
	xtpButtonImageAboveText,  // Image is displayed above the text.
	xtpButtonTextAboveImage,  // Text is displayed above the image.
	xtpButtonImageBeforeText, // Image is displayed before/to the left of the text.
	xtpButtonTextBeforeImage  // Text is displayed before/to the left of the image.
};

//-----------------------------------------------------------------------
// Summary:
//     PushButton styles.
// Remarks:
//     The CXTPButton::SetPushButtonStyle method uses flags from the
//     XTPPushButtonStyle enumeration to set the style of the button.
//-----------------------------------------------------------------------
enum XTPPushButtonStyle
{
	xtpButtonNormal,   // Normal push button.
	xtpButtonDropDown, // Dropdown button. Button will have an arrow indicating it has a dropdown.
					   // The dropdown will appear under the button.
	xtpButtonDropDownRight, // Dropdown button. Button will have an arrow indicating it has a
							// dropdown. The dropdown will appear to the right of the button.
	xtpButtonSplitDropDown, // Split button. Button will be split into a "button" part and "arrow"
							// part. The "arrow" part will have an arrow indicating it has a
							// dropdown. The dropdown will appear under the button when the
							// "arrow" part is clicked.
	xtpButtonDropDownNoGlyph
};

//-----------------------------------------------------------------------
// Summary:
//     Gets/sets the border style of the control.
// Remarks:
//     There are 3 different border styles for the control which are
//     single line with caption, complete border with caption, and no
//     border or caption. CXTPButton::SetBorderStyle method stores a
//     value from the XTPGroupBoxBorderStyle enumeration to specify
//     the border style of the control.
//-----------------------------------------------------------------------
enum XTPGroupBoxBorderStyle
{
	xtpGroupBoxBorder,	 // Complete frame with caption.
	xtpGroupBoxSingleLine, // Single line with caption.
	xtpGroupBoxNone		   // No frame or caption.
};

#	define BST_INCLICK 0x0010
#	define BST_CAPTURED 0x0020
#	define BST_MOUSE 0x0040

#	ifndef CXTPButtonBase
#		define CXTPButtonBase CButton
#	endif

//===========================================================================
// Summary:
//     Push Button control.
//===========================================================================
class _XTP_EXT_CLASS CXTPButton : public CXTPButtonBase
{
#	ifndef _XTP_ACTIVEX_BUTTON
	DECLARE_DYNAMIC(CXTPButton)
#	else
	DECLARE_DYNAMIC(CXTPOleButton)
#	endif

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPButton object.
	//-----------------------------------------------------------------------
	CXTPButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPButton object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPButton();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the button styles for this object. This function returns
	//     only the BS_ style values, not any of the other window styles.
	// Returns:
	//     The BS_ style for this button.
	//-----------------------------------------------------------------------
	BYTE GetButtonStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the the button is
	//     currently being pushed.
	// Returns:
	//     TRUE if the button is currently being pushed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsPushed();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the the button is
	//     currently being highlighted (mouse over).
	// Returns:
	//     TRUE if the button is currently being highlighted (mouse over),
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsHighlighted();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the checked state has
	//     been defined for the button control.
	// Returns:
	//     TRUE if the checked state is defined for the button control,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetChecked();

	// ------------------------------------------------------------------------
	// Summary:
	//     Sets the highlight state for the button control.
	// Parameters:
	//     bChecked -  Specifies whether the button is to be checked. TRUE will
	//                 check the button, FALSE will uncheck it.
	// Remarks:
	//     This member function sets the highlighting state of the button
	//     control. This is the same as its CButton counterpart, except the
	//     m_bChecked flag is set. This is done because MFC does not recognize
	//     the button as being checked unless it is a radio or check box.
	// ------------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the flat style appearance of the button control.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves over it,
	//     at which point it appears three-dimensional.
	// Returns:
	//     TRUE if the flat style is used, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetFlatStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the flat style appearance of the button control.
	// Parameters:
	//     bFlatStyle - TRUE to use the flat style, otherwise FALSE.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves over it,
	//     at which point it appears three-dimensional.
	//-----------------------------------------------------------------------
	void SetFlatStyle(BOOL bFlatStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the control is drawn using visual styles/SkinFramework,
	//     if supported.
	// Remarks:
	//     This method enables Windows XP visual styles for the control.
	//     This Control will draw using visual styles if both the control
	//     and the operating system support it.
	//     Visual styles and SkinFramework skins are specifications for
	//     the appearance of controls. GetUseVisualStyle determines whether
	//     to use the currently set Windows XP visual style. If using
	//     SkinFramework, then it determines whether to use the currently set
	//     style of the SkinFramework.
	// Returns:
	//     TRUE if the currently set Windows XP visual style will be used to
	//     theme the control. If using SkinFramework, then the currently set
	//     style of the SkinFramework will be used to skin the control.
	//     FALSE if the currently set appearance/style in the OS will be used.
	//-----------------------------------------------------------------------
	BOOL GetUseVisualStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables Windows XP visual styles for the control.
	// Remarks:
	//     This method enables Windows XP visual styles for the control.
	//     This Control will draw using visual styles if both the control
	//     and the operating system support it.
	//     Visual styles and SkinFramework skins are specifications for
	//     the appearance of controls. GetUseVisualStyle determines whether
	//     to use the currently set Windows XP visual style. If using
	//     SkinFramework, then it determines whether to use the currently set
	//     style of the SkinFramework.
	// Parameters:
	//     bUseVisualStyle - TRUE if the currently set Windows XP visual style
	//                       will be used to theme the control. If using
	//                       SkinFramework, then the currently set style of the
	//                       SkinFramework will be used to skin the control.
	//                       FALSE if the currently set appearance/style in the
	//                       OS will be used.
	//-----------------------------------------------------------------------
	void SetUseVisualStyle(BOOL bUseVisualStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the gap between the
	//     button's image and border.
	// Returns:
	//     The amount, in pixels, of the gap between the button's
	//     image and border.
	//-----------------------------------------------------------------------
	long GetBorderGap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the gap between the
	//     button's image and border.
	// Parameters:
	//     nBorderGap - The amount, in pixels, of the gap between the button's
	//                  image and border.
	//-----------------------------------------------------------------------
	void SetBorderGap(int nBorderGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function retrieves a flag from the XTPPushButtonStyle enumeration
	//     that specifies the current style of the button.
	// Returns:
	//     The button styles for this CXTPButton object.
	//-----------------------------------------------------------------------
	XTPPushButtonStyle GetPushButtonStyle() const;

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function will set the display style for the button.
	// Parameters:
	//     nPushButtonStyle -  Specifies the button style. The button style can be defined
	//                         as one or more of the styles listed in the Remarks section.
	// Remarks:
	//     The Button can be set to any value from the XTPPushButtonStyle enumeration.
	// See Also:
	//     GetPushButtonStyle
	// ----------------------------------------------------------------------------
	void SetPushButtonStyle(XTPPushButtonStyle nPushButtonStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment of the text on the button control.
	// Returns:
	//     Alignment of the text on the button control.
	//-----------------------------------------------------------------------
	virtual int GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment of the text on the button control.
	// Parameters:
	//     dwAlignment - Alignment of the text on the button control.
	//-----------------------------------------------------------------------
	void SetTextAlignment(DWORD dwAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment of the image on the button control.
	// Returns:
	//     Alignment of the image on the button control.
	//-----------------------------------------------------------------------
	virtual int GetImageAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment of the image on the button control.
	// Parameters:
	//     dwAlignment - Alignment of the image on the button control.
	//-----------------------------------------------------------------------
	void SetImageAlignment(DWORD dwAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text and image relation in the button.
	// Returns:
	//     Flag from the XTPButtonTextImageRelation enumeration that
	//     specifies the text and image relation in the button.
	//-----------------------------------------------------------------------
	XTPButtonTextImageRelation GetTextImageRelation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text and image relation in the button.
	// Parameters:
	//     relation - Flag from the XTPButtonTextImageRelation enumeration that
	//                specifies the text and image relation in the button.
	//-----------------------------------------------------------------------
	void SetTextImageRelation(XTPButtonTextImageRelation relation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the gap between the
	//     button's text and image.
	// Returns:
	//     The amount, in pixels, of the gap between the button's
	//     text and image.
	//-----------------------------------------------------------------------
	long GetImageGap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the gap between the
	//     button's text and image.
	// Parameters:
	//     nImageGap - Amount, in pixels, of the gap between the button's
	//                 text and image.
	//-----------------------------------------------------------------------
	void SetImageGap(int nImageGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the size of the image
	//     displayed for the button control.
	// Returns:
	//     A CSize object.
	//-----------------------------------------------------------------------
	virtual CSize GetImageSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if DPI scaling of images is enabled.
	// 	   If DPI image scaling has ever been changed for the object using
	//     SetDpiImageScaling then the returned value will be the latest
	// 	   value set via SetDpiImageScaling, otherwise it'll return
	// 	   the global DPI bitmap scaling flag obtained via CXTPDpi::IsDpiBitmapScalingEnabled.
	// Returns:
	//     TRUE if DPI scaling of images is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsDpiImageScalingEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable/disable DPI scaling of images. Overrides
	// 	   the default value used from the CXTPDpi::IsDpiBitmapScalingEnabled call.
	// Parameters:
	//     bEnable - TRUE to enable DPI scaling of images, FALSE to disable.
	//-----------------------------------------------------------------------
	virtual void SetDpiImageScaling(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the image
	//     manager for the button control.
	// Returns:
	//     A pointer to a CXTPImageManagerIcon object.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetIcon() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the image of the control.
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     rc            - Size of the image.
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the border style of the control.
	// Returns:
	//     Flag from the XTPGroupBoxBorderStyle enumeration that
	//     specifies the border style of the control.
	//-----------------------------------------------------------------------
	XTPGroupBoxBorderStyle GetBorderStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the border style of the control.
	// Parameters:
	//     style - Flag from the XTPGroupBoxBorderStyle enumeration that
	//             specifies the border style of the control.
	//-----------------------------------------------------------------------
	void SetBorderStyle(XTPGroupBoxBorderStyle style);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets whether a focus rectangle is drawn around the contents of a
	//     button when the button has focus.
	// Returns:
	//     TRUE if the focus rectangle is shown, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetShowFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets whether a focus rectangle is drawn around the contents of a
	//     button when the button has focus.
	// Parameters:
	//     bShowFocus - TRUE to show the focus rectangle, FALSE to hide it.
	//-----------------------------------------------------------------------
	void SetShowFocus(BOOL bShowFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     eTheme         - New visual theme. Can be any of the values listed in the
	//                      Remarks section.
	//     pPaintManager  - A CXTPButtonTheme pointer to the theme object.
	//                      If pPaintManager is NULL, then themes are disabled and the
	//                      paint manager is used.
	// Remarks:
	//     eTheme can be one of the following:
	//     * <b>xtpControlThemeDefault</b>: Use default theme.
	//     * <b>xtpControlThemeFlat</b>: Flat appearance style.
	//     * <b>xtpControlThemeUltraFlat</b>: Ultra flat appearance style.
	//     * <b>xtpControlThemeOffice2000</b>: Office 2000 appearance style.
	//     * <b>xtpControlThemeOfficeXP</b>: Office XP appearance style.
	//     * <b>xtpControlThemeOffice2003</b>: Office 2003 appearance style.
	//     * <b>xtpControlThemeResource</b>: Office 2007 appearance style.
	//     * <b>xtpControlThemeVisualStudio2012Light</b>: VS 2012 Light style theme.
	//     * <b>xtpControlThemeVisualStudio2012Dark</b>: VS 2012 Dark style theme.

	// Returns:
	//     The version that accepts a XTPControlTheme style returns TRUE if successful,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetTheme(XTPControlTheme eTheme);
	virtual void SetTheme(CXTPButtonTheme* pPaintManager); // <combine
														   // CXTPButton::SetTheme@XTPControlTheme>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the currently selected theme.
	// Returns:
	//     A pointer to a CXTPButtonTheme object representing the currently selected theme.
	//-----------------------------------------------------------------------
	CXTPButtonTheme* GetTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the button.
	// Parameters:
	//     bUpdateWindow - TRUE to notify the parent window.
	//-----------------------------------------------------------------------
	void RedrawButton(BOOL bUpdateWindow = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the buttons have a drop
	//     down arrow and if it is drop down style.
	// Returns:
	//     TRUE if the button uses drop down style, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDropDownStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine the size and location
	//     of the button.
	// Returns:
	//     The size and location of the button, or an empty rect if no button is defined.
	//-----------------------------------------------------------------------
	virtual CRect GetButtonRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the control used as the buddy control.
	// Parameters:
	//     pWndBuddy - Handle to the control to use as a buddy.
	//     bMoveButton - TRUE to move the button, FALSE to move the buddy.
	//-----------------------------------------------------------------------
	void SetBuddy(CWnd* pWndBuddy, BOOL bMoveButton = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Associates tooltip text with the button.
	// Parameters:
	//     lpszTooltip - The tooltip text to display when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the button's tooltip.
	// Returns:
	//     The tooltip text for the button.
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the tooltip context.
	// Returns:
	//     A pointer to the tooltip context.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the normal and hot state icons for
	//     the push button.
	// Parameters:
	//     size           - CSize object that represents the size of the icon.
	//     hIcon          - Handle to the normal icon.
	//     pIcon          - Handle to a CXTPImageManagerIcon icon.
	//     hIconHandle    - Handle to the normal icon.
	//     hIconHot       - Handle to the hot icon.
	//     hIconHotHandle - Handle to the hot icon.
	//     nID            - Resource ID for the normal icon.
	//     nHotID         - Resource ID for the pressed icon.
	//     lpszID         - Resource string ID for the normal icon.
	//     lpszHotID      - Resource string ID for the hot icon.
	//     bRedraw        - Specifies whether the button is to be redrawn. A
	//                      nonzero value redraws the button. A zero value does
	//                      not redraw the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetIcon(CSize size, HICON hIcon, HICON hIconHot = NULL, BOOL bRedraw = TRUE);
	virtual BOOL SetIcon(CSize size, UINT nID, UINT nHotID = 0,
						 BOOL bRedraw = TRUE); //<COMBINE
											   // CXTPButton::SetIcon@CSize@HICON@HICON@BOOL>
	virtual BOOL SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszHotID = NULL,
						 BOOL bRedraw = TRUE); //<COMBINE
											   // CXTPButton::SetIcon@CSize@HICON@HICON@BOOL>
	BOOL SetIcon(CSize size, CXTPImageManagerIcon* pIcon,
				 BOOL bRedraw = TRUE); //<COMBINE CXTPButton::SetIcon@CSize@HICON@HICON@BOOL>
	virtual BOOL SetIcon(CSize size, const CXTPImageManagerIconHandle& hIconHandle,
						 const CXTPImageManagerIconHandle& hIconHotHandle,
						 BOOL bRedraw = TRUE); //<COMBINE
											   // CXTPButton::SetIcon@CSize@HICON@HICON@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the bitmap associated with the
	//     button.
	// Parameters:
	//     size    - CSize object that represents the size of the bitmap.
	//     nID     - Resource ID for the bitmap.
	//     bRedraw - Specifies whether the button is to be redrawn. A
	//               nonzero value redraws the button. A zero value does
	//               not redraw the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetBitmap(CSize size, UINT nID, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the button's text minus the '&'.
	// Parameters:
	//     bRemoveAmpersand - TRUE to remove the '&' used for mnemonics.
	// Returns:
	//     A CString object that represents the button's text.
	//-----------------------------------------------------------------------
	virtual CString GetButtonText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the size of the check box glyph.
	// Parameters:
	//     bCheckBox - TRUE if the check box is checked, otherwise FALSE.
	// Returns:
	//     The size of the check box glyph.
	//-----------------------------------------------------------------------
	CSize GetGlyphSize(BOOL bCheckBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is a push button or a drop down button.
	// Returns:
	//     TRUE if the button is a push button, FALSE if it is a drop down button.
	//-----------------------------------------------------------------------
	BOOL IsPushButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows the use of a XAML snippet to be used for the text of
	//     panes in the control.
	// Parameters:
	//     bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the button's text color.
	// Returns:
	//     The button's text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetButtonTextColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the button's back color.
	// Returns:
	//     The button's back color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetButtonBackColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when the text has changed.
	//-----------------------------------------------------------------------
	virtual void InternalTextChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the markup context.
	// Returns:
	//     A pointer to a CXTPMarkupContext object.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the markup element that renders the
	//     caption of the control.
	// Returns:
	//     Pointer to a CXTPMarkupUIElement element.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Occurs when the drop down button is dropped down.
	//-----------------------------------------------------------------------
	virtual void OnDropDown();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs all of the drawing logic of the
	//     calendar control window.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Manually drops down a drop down button.
	//-----------------------------------------------------------------------
	void DoDropDown();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh theme colors and redraw the control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
	//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPButton)
	virtual void DoDraw(CDC* pDC);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPButton)
	// Window
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	// Focus
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// Mouse
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	//
	afx_msg LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnInvalidate();
	afx_msg LRESULT OnCaptureChanged(WPARAM, LPARAM);
	afx_msg LRESULT OnDefaultAndInvalidate(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateUIState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetDlgCode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	//}}AFX_CODEJOCK_PRIVATE

public:
	BOOL m_bPushed;			   // TRUE if button was pressed.
	BOOL m_bHot;			   // TRUE if mouse under button.
	CXTPButtonTheme* m_pTheme; // Pointer to the current theme object.
	BOOL m_bChecked;		   // TRUE if checked.

	BOOL m_bFlatStyle;		// TRUE to see the flat style.
	BOOL m_bUseVisualStyle; // TRUE to use visual styles.

	long m_nImageAlignment; // Image Alignment.
	long m_nBorderGap;		// Gap between the button edge and the image.

	long m_nPushButtonStyle; // The style of the button.

	BOOL m_bEnableMarkup; // TRUE to enable markup.

	long m_nImageGap;			   // Gap between the button text and the image.
	long m_nTextImageRelation;	 // Image and text orientation.
	CXTPImageManagerIcon* m_pIcon; // Multiple state icon manager converts icon to different states.
	CSize m_szVectorIcon;		   // Size of displayed image if vector icon is used.
	long m_nBorderStyle;		   // Border style used.
	BOOL m_bShowFocus;			   // TRUE to show the focus rectangle.

	HWND m_hWndBuddy;					   // Handle to the buddy control.
	CXTPToolTipContext* m_pToolTipContext; // TooltipContext.

	CString m_strTooltip; // Tooltip of the item.

protected:
	CXTPMarkupContext* m_pMarkupContext; // Markup context.
	CXTPMarkupUIElement* m_pUIElement;   // Markup element pointer.
	BOOL m_bImageDPIScaling;			 // TRUE if DPI scaling of images enabled.

private:
	BOOL m_bImageDPIScalingModified;
};

AFX_INLINE BOOL CXTPButton::IsPushed()
{
	return m_bPushed;
}
AFX_INLINE BOOL CXTPButton::GetFlatStyle() const
{
	return m_bFlatStyle;
}
AFX_INLINE BOOL CXTPButton::GetUseVisualStyle() const
{
	return m_bUseVisualStyle;
}
AFX_INLINE long CXTPButton::GetBorderGap() const
{
	return m_nBorderGap;
}
AFX_INLINE XTPPushButtonStyle CXTPButton::GetPushButtonStyle() const
{
	return static_cast<XTPPushButtonStyle>(m_nPushButtonStyle);
}
AFX_INLINE int CXTPButton::GetImageAlignment() const
{
	return m_nImageAlignment;
}
AFX_INLINE XTPButtonTextImageRelation CXTPButton::GetTextImageRelation() const
{
	return static_cast<XTPButtonTextImageRelation>(m_nTextImageRelation);
}
AFX_INLINE long CXTPButton::GetImageGap() const
{
	return m_nImageGap;
}
AFX_INLINE void CXTPButton::SetImageGap(int nImageGap)
{
	m_nImageGap = nImageGap;
}
AFX_INLINE CXTPImageManagerIcon* CXTPButton::GetIcon() const
{
	return m_pIcon;
}
AFX_INLINE XTPGroupBoxBorderStyle CXTPButton::GetBorderStyle() const
{
	return static_cast<XTPGroupBoxBorderStyle>(m_nBorderStyle);
}
AFX_INLINE void CXTPButton::SetBorderStyle(XTPGroupBoxBorderStyle style)
{
	m_nBorderStyle = style;
}
AFX_INLINE BOOL CXTPButton::GetShowFocus() const
{
	return m_bShowFocus;
}
AFX_INLINE void CXTPButton::SetShowFocus(BOOL bShowFocus)
{
	m_bShowFocus = bShowFocus;
}
AFX_INLINE CXTPMarkupContext* CXTPButton::GetMarkupContext() const
{
	return m_pMarkupContext;
}
AFX_INLINE CXTPMarkupUIElement* CXTPButton::GetMarkupUIElement()
{
	return m_pUIElement;
}
AFX_INLINE void CXTPButton::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strTooltip = lpszTooltip;
}
AFX_INLINE CString CXTPButton::GetTooltip() const
{
	return m_strTooltip;
}
AFX_INLINE CXTPToolTipContext* CXTPButton::GetToolTipContext() const
{
	return m_pToolTipContext;
}
AFX_INLINE CXTPButtonTheme* CXTPButton::GetTheme()
{
	return m_pTheme;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPBUTTON_H__)
