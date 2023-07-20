// XTPComboBox.h interface for the CXTPComboBox class.
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
#if !defined(__XTPCOMBOBOX_H__)
#	define __XTPCOMBOBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPComboBoxTheme;
class CXTPComboBoxEditCtrl;

//===========================================================================
// Summary:
//     The CXTPComboBox class provides the functionality of a themed Windows combobox.
//===========================================================================
class _XTP_EXT_CLASS CXTPComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CXTPComboBox)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPComboBox object.
	//-----------------------------------------------------------------------
	CXTPComboBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPComboBox object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPComboBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed
	//              in the Remarks section.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// Remarks:
	//     nTheme can be one of the theme IDs specified by XTPControlTheme.
	// See Also:
	//     XTPControlTheme
	//-----------------------------------------------------------------------
	BOOL SetTheme(XTPControlTheme nTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the theme for the combobox
	//     control using the theme specified by pTheme.
	// Parameters:
	//     pTheme - Pointer to a valid CXTPComboBox theme object.
	//-----------------------------------------------------------------------
	virtual void SetTheme(CXTPComboBoxTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to the current
	//     theme used by the combobox.
	// Returns:
	//     A CXTPComboBoxTheme pointer, or NULL if no theme is set.
	//-----------------------------------------------------------------------
	CXTPComboBoxTheme* GetTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to the current
	//     theme used by the combobox.
	// Returns:
	//     A CXTPComboBoxTheme pointer, or NULL if no theme is set.
	//-----------------------------------------------------------------------
	const CXTPComboBoxTheme* GetTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the button.
	// Parameters:
	//     bUpdateWindow - TRUE to notify the parent window.
	//-----------------------------------------------------------------------
	void RedrawComboBox(BOOL bUpdateWindow = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the flat style appearance of the combobox.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves
	//     over it, at which point it appears three-dimensional.
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
	//     The control will appear flat until the mouse pointer moves
	//     over it, at which point it appears three-dimensional.
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
	//     This member function enables/disables auto-completion.
	// Parameters:
	//     bEnable - TRUE to enable auto-completion, FALSE to disable auto-completion.
	//               By default, this parameter is TRUE.
	//-----------------------------------------------------------------------
	void EnableAutoCompletion(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the background color
	//     for the CXTPComboBox control.
	// Parameters:
	//     crBack - An RGB color value representing the background color
	//              for the CXTPComboBox control.
	// See Also:
	//     SetTextColor, GetBackColor, GetTextColor
	//-----------------------------------------------------------------------
	virtual void SetBackColor(COLORREF crBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the background color
	//     of the CXTPComboBox control.
	// Returns:
	//     An RGB color value representing the background color
	//     of the CXTPComboBox control.
	// See Also:
	//     SetTextColor, SetBackColor, GetTextColor
	//-----------------------------------------------------------------------
	COLORREF GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text color
	//     for the CXTPComboBox control.
	// Parameters:
	//     crText - An RGB value representing the text color
	//              for the CXTPComboBox control.
	// See Also:
	//     SetBackColor, GetBackColor, GetTextColor
	//-----------------------------------------------------------------------
	virtual void SetTextColor(COLORREF crText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the text color
	//     of the CXTPComboBox control.
	// Returns:
	//     An RGB color value representing the text color
	//     of the CXTPComboBox control.
	// See Also:
	//     SetTextColor, SetBackColor, GetBackColor
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to update the color, text, and other
	//     visual elements of the control.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the combobox has
	//     input focus.
	// Returns:
	//     TRUE if the combobox has input focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the combobox state
	//     is highilghted, meaning that the mouse cursor is currently
	//     hovering over the control.
	// Returns:
	//     TRUE if the combobox state is highlighted, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a handle to the client
	//     background brush.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     A HBRUSH handle representing the client background brush.
	//-----------------------------------------------------------------------
	HBRUSH GetClientBrush(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the combobox
	//     in client coordinates.
	// Returns:
	//     A CRect object representing the client size for the combobox.
	//-----------------------------------------------------------------------
	CRect GetComboBoxRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member fuction is called to set the highlighted state for
	//     the combobox.
	// Parameters:
	//     bHot - TRUE to set the combobox state to highlighted.
	//-----------------------------------------------------------------------
	void SetHighlighted(BOOL bHot);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to redraw the focused frame for the combobox.
	//-----------------------------------------------------------------------
	void RedrawFocusedFrame();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to determine if the combobox needs to be redrawn.
	// Returns:
	//     TRUE if the combobox needs to be redrawn.
	//-----------------------------------------------------------------------
	BOOL NeedRedraw();

	//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPComboBox)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	virtual void Init();

	//{{AFX_MSG(CXTPComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSysColorChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor); // reflect message
	afx_msg BOOL OnEditUpdate();
	afx_msg BOOL OnCloseUp();
	afx_msg BOOL OnDropDown();
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bPreSubclassInit; // TRUE if the control has been subclassed.
	BOOL m_bUseVisualStyle;  // TRUE to use Windows themes.
	BOOL m_bFlatStyle; // TRUE to draw the control using flat styles. This is mostly deprecated; use
					   // SetTheme() instead.
	BOOL m_bHighlighted; // TRUE when the combobox is highlighted.
	BOOL m_bFocused;	 // TRUE when the combobox has input focus.
	BOOL m_bAutoComp;	// TRUE when the combobox is using the auto-complete feature.
	BOOL m_bDisableAC;   // TRUE to disable auto-complete temporarily.

	CXTPComboBoxTheme* m_pTheme;	  // Pointer to the current theme object.
	CXTPComboBoxEditCtrl* m_pWndEdit; // Pointer to the edit-control for the combobox.

private:
	friend class CXTPComboBoxEditCtrl;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPComboBox::GetUseVisualStyle() const
{
	return m_bUseVisualStyle;
}
AFX_INLINE BOOL CXTPComboBox::IsFocused() const
{
	return m_bFocused;
}
AFX_INLINE BOOL CXTPComboBox::IsHighlighted() const
{
	return m_bHighlighted;
}
AFX_INLINE BOOL CXTPComboBox::GetFlatStyle() const
{
	return m_bFlatStyle;
}
AFX_INLINE void CXTPComboBox::SetFlatStyle(BOOL bFlatStyle /* = TRUE*/)
{
	m_bFlatStyle = bFlatStyle;
	if (m_hWnd)
		Invalidate(FALSE);
}
AFX_INLINE void CXTPComboBox::EnableAutoCompletion(BOOL bEnable /*=TRUE*/)
{
	m_bAutoComp = bEnable;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPCOMBOBOX_H__)
