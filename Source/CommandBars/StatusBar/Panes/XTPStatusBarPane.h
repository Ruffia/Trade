// XTPStatusBarPane.h
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
#if !defined(__XTPSTATUSBARPANE_H__)
#	define __XTPSTATUSBARPANE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPStatusBarPane is a CXTPCmdTarget derived class.
//     It represents a single pane of a status bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarPane : public CXTPCmdTarget
{
private:
	DECLARE_DYNAMIC(CXTPStatusBarPane)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarPane object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the text caption that is displayed
	//     in this pane.
	// Parameters:
	//     lpszText - Text caption of the pane.
	// See Also:
	//     GetText
	//-----------------------------------------------------------------------
	void SetText(LPCTSTR lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text that is displayed for the pane in the status
	//     bar customization window.
	// Parameters:
	//     lpszCaption - The caption is the text that is displayed for
	//                   the pane in the status bar customization window.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text that is displayed for the pane in the status
	//     bar customization window.
	// Returns:
	//     Text caption.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value of the pane. This is displayed in the status
	//     bar customization window.
	// Parameters:
	//     lpszValue - The value of the pane can be any string value and
	//                 can be used to store extra data if customization
	//                 is disabled. If customization is enabled, then the
	//                 Value is the text that is displayed for the pane
	//                 in the status bar customization window.
	//-----------------------------------------------------------------------
	void SetValue(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the pane. This is displayed in the status
	//     bar customization window.
	// Returns:
	//     The value of the status bar pane.
	//-----------------------------------------------------------------------
	CString GetValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the text caption of the pane.
	// Returns:
	//     Text caption of pane. This is the text displayed in the pane.
	// See Also:
	//     SetText
	//-----------------------------------------------------------------------
	CString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of this pane.
	// Parameters:
	//     nWidth - new width to be set.
	// See Also:
	//     GetWidth
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the pane.
	// Returns:
	//     Current width of the pane.
	// See Also:
	//     SetWidth
	//-----------------------------------------------------------------------
	virtual int GetWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the style of a status bar's pane.
	//     A pane's style determines how the pane appears.
	// Parameters:
	//     nStyle - Style of the pane whose style is to be set.
	//-----------------------------------------------------------------------
	void SetStyle(UINT nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the style of a status bar's pane.
	// Returns:
	//     Style of the indicator pane.
	//-----------------------------------------------------------------------
	UINT GetStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the ID of a status bar's pane.
	// Returns:
	//     ID of the indicator pane.
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the index of a status bar's pane.
	// Returns:
	//     Index of the indicator pane.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove the pane.
	//-----------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the pane is visible.
	// Returns:
	//     TRUE if the tab is visible, FALSE otherwise.
	// See Also:
	//     SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the visibility of the pane.
	// Parameters:
	//     bVisible - TRUE to show the tab, FALSE to hide the tab.
	//     bRecalcLayout - TRUE to recalculate layout after.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible, BOOL bRecalcLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color of an item text.
	// Parameters:
	//     clrText -  COLORREF object that represents the color of an item
	//                text.
	//-----------------------------------------------------------------------
	void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the disabled color of an item text.
	// Parameters:
	//     clrText -  COLORREF object that represents the disabled color of an item
	//                text.
	//-----------------------------------------------------------------------
	void SetTextGrayColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the color of an item text.
	// Returns:
	//     A COLORREF object that represents the color of an item text.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the disabled color of an item text.
	// Returns:
	//     A COLORREF object that represents the disabled color of an item text.
	//-----------------------------------------------------------------------
	COLORREF GetTextGrayColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color of an item background.
	// Parameters:
	//     clrBackground -  COLORREF object that represents the background color of an item
	//                      text.
	//-----------------------------------------------------------------------
	void SetBackgroundColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the background color of an item.
	// Returns:
	//     The background color of an item.
	//-----------------------------------------------------------------------
	COLORREF GetBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Parameters:
	//     pFntText -  pointer to a CFont object.
	//     pLogfText -  pointer to a LOGFONT structure.
	//-----------------------------------------------------------------------
	void SetTextFont(CFont* pFntText);
	void SetTextFont(PLOGFONT pLogfText); // <combine CXTPStatusBarPane::SetTextFont@CFont*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Returns:
	//     Pointer to CFont object.
	//-----------------------------------------------------------------------
	virtual CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves icon index.
	// Returns:
	//     Icon index.
	// See Also:
	//     SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the icon index of an item.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// See Also:
	//     GetIconIndex
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the alignment of an item text.
	// Parameters:
	//     nAlign -  The alignment style of an item text.
	//-----------------------------------------------------------------------
	void SetTextAlignment(int nAlign);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the alignment of an item text.
	// Returns:
	//     The alignment style of an item text.
	//-----------------------------------------------------------------------
	int GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tooltip for this pane. This is the
	//     text that will be displayed when the mouse pointer is positioned
	//     over the pane.
	// Parameters:
	//     lpszTooltip - Tooltip to set for this tab.
	// See Also:
	//     GetTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tooltip displayed when the mouse
	//     pointer is positioned over the pane.
	// Returns:
	//     The tooltip displayed when the mouse if over the pane.
	// See Also:
	//     SetTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Fits the width of the pane to its text width.
	//-----------------------------------------------------------------------
	void BestFit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the best fit width which fits the width of the pane to
	//     the its text width.
	// Returns:
	//     Best fit width.
	//-----------------------------------------------------------------------
	int GetBestFit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a separator before the pane.
	// Parameters:
	//     bBeginGroup - TRUE to start a new group.
	//-----------------------------------------------------------------------
	void SetBeginGroup(BOOL bBeginGroup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a boolean value indicating whether the pane has a separator
	//     placed before it and thus begins a new group.
	// Returns:
	//     TRUE if the pane begins a group, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetBeginGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the parent status bar object.
	// Returns:
	//     Pointer to the parent status bar object.
	//-----------------------------------------------------------------------
	CXTPStatusBar* GetStatusBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get an image of the pane.
	// Returns:
	//     CXTPImageManagerIcon pointer to be drawn.
	//-----------------------------------------------------------------------
	virtual CXTPImageManagerIcon* GetImage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns margins for pane entry
	// See Also:
	//     SetMargins
	//-----------------------------------------------------------------------
	CRect GetMargins() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the padding placed around the Text of a pane.
	// Returns:
	//     Padding placed around the Text of a pane.
	//-----------------------------------------------------------------------
	CRect GetPadding() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set margins for the pane.
	// Parameters:
	//     nLeft - Left margin.
	//     nTop - Top margin.
	//     nRight - Right margin.
	//     nBottom - Bottom margin.
	// See Also:
	//     GetMargins
	//-----------------------------------------------------------------------
	void SetMargins(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the padding placed around the text of a pane in pixels.
	// Parameters:
	//     nLeft - Padding placed on the left side of the text of a pane.
	//     nTop - Padding placed on the top of the text of a pane.
	//     nRight - Padding placed on the right side of the text of a pane.
	//     nBottom - Padding placed on the bottom of the text of a pane.
	//-----------------------------------------------------------------------
	void SetPadding(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the pane will also act as a button.
	// Parameters:
	//     bButton - TRUE if the pane will act as a button.
	// Remarks:
	//     Button panes will look and act like a button on mouse over
	//     and when clicked.  On mouse over, the button pane will become
	//     "hot" giving a visual que that it is a button and can be
	//     clicked.  When clicked, the button pane will change to a
	//     "pressed" state.
	//-----------------------------------------------------------------------
	void SetButton(BOOL bButton = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the pane will also act as a button.
	// Returns:
	//     TRUE if the pane acts as a button, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a boolean value indicating whether the switch pane is enabled.
	// Returns:
	//     TRUE if enabled, FALSE if disabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the switch pane is enabled or disabled.
	// Parameters:
	//     bEnabled - TRUE to enable the pane, FALSE to disable the pane.
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the pane button is
	//     currently highlighted. This occurs when the mouse cursor is
	//     positioned over the pane button.
	// Returns:
	//     TRUE if the pane button is currently highlighted.
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the pane button is pressed.
	// Returns:
	//     TRUE if the pane button is pressed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables or disables status bar customization.
	// Parameters:
	//     bVisible - TRUE to enable customization, FALSE to disable customization.
	// Remarks:
	//     SetCustomizationVisible will enable a status bar customization
	//     window to appear when the status bar is right-clicked on. The
	//     customization window allows all the panes in the statusbar to
	//     be hidden or displayed (if already hidden).  All panes will be
	//     displayed in the window with the exception of the pane with an
	//     ID equal to 0. The items displayed in the window are the
	//     caption of the pane and the value of the pane. Also indicated
	//     is the visibility of the pane.
	//-----------------------------------------------------------------------
	void SetCustomizationVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets whether customization is enabled.
	// Returns:
	//     TRUE if customization is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetCustomizationVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the pane button state is
	//     checked.
	// Returns:
	//     TRUE if the pane button is checked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsChecked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the checked state of a pane.
	// Parameters:
	//     bChecked - TRUE to set the pane to checked, FALSE to set the pane to unchecked.
	//-----------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the pane has borders
	//     enabled.
	// Returns:
	//     TRUE if the pane has borders enabled, otherwise FALSE.
	// Remarks:
	//     If the pane is a button, the return value is always FALSE
	//     since buttons will handle drawing their own borders.
	// See Also:
	//     IsButton
	//-----------------------------------------------------------------------
	BOOL HasBorders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to show the pane borders.
	// Parameters:
	//     bShowBorders - TRUE to show the pane borders, FALSE to hide them.
	// Remarks:
	//     If the pane is a button, then this has no effect.
	//-----------------------------------------------------------------------
	void ShowBorders(BOOL bShowBorders);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to redraw the pane.
	//-------------------------------------------------------------------------
	void Redraw() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle for the pane.
	// Returns:
	//     A CRect object.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get the stored CXTPMarkupUIElement object.
	// Returns:
	//     A pointer to the CXTPMarkupUIElement object.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Some themes may display status bar panes as dark.
	//     This method turns this effect on/off.
	//-----------------------------------------------------------------------
	void SetDark(BOOL bDark = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if a pane is to be drawn as dark in some themes.
	// Returns:
	//      TRUE if the pane is dark, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDark() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a pane item. Override it to draw a custom pane.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Item rectangle.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process left mouse button events.
	// Parameters:
	//     point - A CPoint that specifies the x- and y- coordinates of
	//             the cursor. These coordinates are always relative to
	//             the upper-left corner of the window.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether
	//     a point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor. These
	//             coordinates are always relative to the upper-left
	//             corner of the window.
	//     pTI   - Pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, then the window control ID is returned.
	//     If the tooltip control was not found, then a value of -1 is returned.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	UINT m_nID;	// IDC of indicator: 0 => normal text area.
	int m_cxText;  // Width of the string area in pixels. On both sides there is a 3 pixel gap and a
				   // 1 pixel border, making a pane 6 pixels wider.
	UINT m_nStyle; // Style flags (SBPS_*).
	CString m_strText;		  // Text in the pane.
	DWORD m_dwHideFlags;	  // TRUE if pane is visible
	COLORREF m_clrText;		  // Text color
	COLORREF m_clrTextGray;   // Disabled text color
	COLORREF m_clrBackground; // Background color
	CString m_strToolTip;	 // Tooltip text for the item

	CRect m_rcPane; // Bounding rectangle for the pane

	HWND m_hWndPane;	   // Child handle
	BOOL m_bAutoDeleteWnd; // TRUE to auto delete child.
	int m_nAlignment;	  // Alignment of the item.

	CXTPStatusBar* m_pStatusBar; // Parent StatusBar object
	int m_nIndex;				 // Index of the item
	CXTPFont m_xtpFontText;		 // Font of an item text
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntText, m_xtpFontText, GetTextFontHandle);
	int m_nIconIndex;						 // Icon index of the pane
	CRect m_rcPadding;						 // Margins of the pane
	CRect m_rcMargins;						 // Margins of the pane
	BOOL m_bButton;							 // TRUE if pane is clickable button
	BOOL m_bPressed;						 // TRUE if button was pressed
	CXTPMarkupUIElement* m_pMarkupUIElement; // Markup Context
	BOOL m_bBeginGroup;						 // TRUE if the pane begins a group
	CString m_strCaption; // The text that is displayed for the pane in the status bar customization
						  // window
	CString m_strValue; // The value of the pane.  This is displayed in the status bar customization
						// window
	int m_nBestFit;		// Width for the pane to have the best fit
	BOOL m_bCustomizable; // True to enable customization, False to disable customization
	BOOL m_bIsDark;		  // True if a pane should be dark in some themes.

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPStatusBarPane);

	afx_msg BSTR OleGetText();
	afx_msg LPFONTDISP OleGetFont();
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg HWND OleGetHandle();
	afx_msg void OleSetHandle(HWND hWnd);
	afx_msg int OleGetHideFlags();
	afx_msg void OleSetHideFlags(int dwHideFlags);
	afx_msg void OleSetVisible(BOOL bVisible);
	afx_msg void OleRedrawPane();
	afx_msg void OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom);
	afx_msg BOOL OleGetDark();
	afx_msg void OleSetDark(BOOL fDark);

//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPStatusBar;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPStatusBarPane::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
}

AFX_INLINE CString CXTPStatusBarPane::GetCaption() const
{
	return m_strCaption;
}

AFX_INLINE void CXTPStatusBarPane::SetValue(LPCTSTR lpszValue)
{
	m_strValue = lpszValue;
}

AFX_INLINE CString CXTPStatusBarPane::GetValue() const
{
	return m_strValue;
}

AFX_INLINE BOOL CXTPStatusBarPane::IsHighlighted() const
{
	return m_pStatusBar->m_pHighlightedPane == this;
}

AFX_INLINE BOOL CXTPStatusBarPane::IsPressed() const
{
	return m_bPressed;
}

AFX_INLINE void CXTPStatusBarPane::SetBeginGroup(BOOL bBeginGroup)
{
	m_bBeginGroup = bBeginGroup;
}

AFX_INLINE BOOL CXTPStatusBarPane::GetBeginGroup() const
{
	return m_bBeginGroup;
}

AFX_INLINE BOOL CXTPStatusBarPane::HasBorders() const
{
	return ((m_nStyle & SBPS_NOBORDERS) == 0);
}

AFX_INLINE void CXTPStatusBarPane::ShowBorders(BOOL bShowBorders)
{
	SetStyle(bShowBorders ? m_nStyle & ~SBPS_NOBORDERS : m_nStyle | SBPS_NOBORDERS);
}

AFX_INLINE CXTPMarkupUIElement* CXTPStatusBarPane::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}

AFX_INLINE void CXTPStatusBarPane::SetDark(BOOL bDark /*= TRUE*/)
{
	m_bIsDark = bDark;
}

AFX_INLINE BOOL CXTPStatusBarPane::IsDark() const
{
	return m_bIsDark;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSTATUSBARPANE_H__)
