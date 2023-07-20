// XTPControlComboBox.h : interface for the CXTPControlComboBox class.
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
#if !defined(__XTPCONTOLCOMBOBOX_H__)
#	define __XTPCONTOLCOMBOBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	define WM_XTP_SHELLAUTOCOMPLETESTART (WM_XTP_COMMANDBARS_BASE + 30)

class CXTPControlComboBox;
class CXTPControlComboBoxAutoCompleteWnd;

//{{AFX_CODEJOCK_PRIVATE

// CXTPControlComboBoxAutoCompleteWnd implementation.
// used internally in CXTPControlComboBox and CXTPControlEdit controls

class _XTP_EXT_CLASS CXTPControlComboBoxAutoCompleteWnd : public CXTPHookManagerHookAble
{
public:
	CXTPControlComboBoxAutoCompleteWnd();
	~CXTPControlComboBoxAutoCompleteWnd();

public:
	HRESULT ShellAutoComplete(HWND hEdit, DWORD dwFlags);
	void CloseWindow();
	void SetupMessageHook(BOOL bSetup);
	BOOL IsDialogCode(UINT nChar, LPARAM lParam);

private:
	static CXTPControlComboBoxAutoCompleteWnd* m_pWndMonitor;
	static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam,
							  LRESULT& lResult);
	void SetAutoCompeteHandle(HWND);

public:
	HWND m_hWndAutoComplete;

private:
	static HHOOK m_hHookMessage;
	HWND m_hWndEdit;
};

//}}AFX_CODEJOCK_PRIVATE

///===========================================================================
// Summary:
//     CXTPControlComboBoxPopupBar is a CXTPPopupBar derived class.
//     It represents the base class for combo pop-ups.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlComboBoxPopupBar : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPControlComboBoxPopupBar)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlComboBoxPopupBar object.
	//-----------------------------------------------------------------------
	CXTPControlComboBoxPopupBar();

public:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRUAL(CXTPControlComboBoxPopupBar)
	virtual int GetCurSel() const
	{
		return CB_ERR;
	}
	virtual int FindString(int /*nStartAfter*/, LPCTSTR /*lpszItem*/) const
	{
		return CB_ERR;
	}
	virtual int FindStringExact(int /*nIndexStart*/, LPCTSTR /*lpsz*/) const
	{
		return CB_ERR;
	}
	virtual int SetTopIndex(int /*nIndex*/)
	{
		return CB_ERR;
	}
	virtual void SetCurSel(int /*nIndex*/)
	{
	}
	virtual void GetText(int /*nIndex*/, CString& /*rString*/) const
	{
	}
	virtual BOOL ProcessHookKeyDown(CXTPControlComboBox* pComboBox, UINT nChar, LPARAM lParam);
	BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	virtual int GetCount() const
	{
		return CB_ERR;
	}

	//}}AFX_VIRUAL
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPControlComboBoxList is a CXTPControlComboBoxPopupBar derived class.
//     It represents a list box of CXTPControlComboBox control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlComboBoxList : public CXTPControlComboBoxPopupBar
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlButton object.
	//-----------------------------------------------------------------------
	CXTPControlComboBoxList();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates list box.
	//-----------------------------------------------------------------------
	virtual void CreateListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves list box  window.
	//-----------------------------------------------------------------------
	CListBox* GetListBoxCtrl() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during
	//     message reflection.
	// Parameters:
	//     hWnd     - Window handle that the message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam   - Specifies additional message-dependent information.
	//     lParam   - Specifies additional message-dependent information.
	//     lResult  - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	//-----------------------------------------------------------------------
	int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called, then the pop-up bar becomes visible.
	// Parameters:
	//     pControlPopup - Pointer to a CXTPControlPopup object.
	//     bSelectFirst  - TRUE to select the first item.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode - TRUE to set the tracking mode, otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is pop-up'ed by the keyboard.
	// See Also:
	//     IsTrackingMode.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard = FALSE);

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
	//     The framework calls this member function when a non-system key
	//     is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	//     lParam   - Specifies additional message-dependent information.
	// Returns:
	//     TRUE if key handled, otherwise FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the customize mode of the command bars.
	// Returns:
	//     TRUE if command bars are in customized mode, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizable() const
	{
		return FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRUAL(CXTPControlComboBoxList)
	virtual int GetCurSel() const
	{
		return GetListBoxCtrl()->GetCurSel();
	}
	virtual int FindString(int nStartAfter, LPCTSTR lpszItem) const
	{
		return GetListBoxCtrl()->FindString(nStartAfter, lpszItem);
	}
	virtual int FindStringExact(int nIndexStart, LPCTSTR lpsz) const
	{
		return GetListBoxCtrl()->FindStringExact(nIndexStart, lpsz);
	}
	virtual int SetTopIndex(int nIndex)
	{
		return GetListBoxCtrl()->SetTopIndex(nIndex);
	}
	virtual void SetCurSel(int nIndex)
	{
		GetListBoxCtrl()->SetCurSel(nIndex);
	}
	virtual void GetText(int nIndex, CString& rString) const
	{
		GetListBoxCtrl()->GetText(nIndex, rString);
	}
	//}}AFX_VIRUAL
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPControlComboBoxList)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process key down events.
	// Parameters:
	//     pComboBox - Owner combobox pointer.
	//     nChar - Specifies the virtual key code of the given key.
	//     lParam - Keystroke-message information.
	// Returns:
	//     TRUE if the message was processed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ProcessHookKeyDown(CXTPControlComboBox* pComboBox, UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the combobox.
	// Parameters:
	//     lpDrawItemStruct - A long pointer to a DRAWITEMSTRUCT structure.
	//                        The structure contains information about the item
	//                        to be drawn and the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method and fill in the MEASUREITEMSTRUCT structure
	//     to inform Windows of the list-box dimensions.
	// Parameters:
	//     lpMeasureItemStruct - Specifies a long pointer to a MEASUREITEMSTRUCT
	//                           structure.
	//-----------------------------------------------------------------------
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

private:
	DECLARE_XTP_COMMANDBAR(CXTPControlComboBoxList)
	friend class CXTPControlComboBox;

	int m_nListIconId; // Icon identifier
};

//////////////////////////////////////////////////////////////////////////

//===========================================================================
// Summary:
//     Inplace Edit control of the combo.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlComboBoxEditCtrl : public CXTPCommandBarEditCtrl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves parent CXTPControlComboBox object.
	// Returns:
	//     Pointer to parent CXTPControlComboBox.
	//-----------------------------------------------------------------------
	CXTPControlComboBox* GetControlComboBox() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the char format of edit control.
	//-------------------------------------------------------------------------
	void UpdateCharFormat();

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CXTPControlComboBoxEditCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditChanged();
	afx_msg LRESULT OnEnableDisable(WPARAM, LPARAM);
	afx_msg LRESULT OnWindowFromPoint(WPARAM, LPARAM);
	afx_msg void OnShellAutoCompleteStart();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPControlComboBox* m_pControl; // Parent Combobox.

private:
	friend class CXTPControlComboBox;
};

//-----------------------------------------------------------------------
// Summary:
//     CXTPControlComboBox is a CXTPControl derived class.
//     It represents a combobox control.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPControlComboBox : public CXTPControlPopup
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlComboBox object.
	// Parameters:
	//     pCommandBars - Pointer to parent CommandBars class.
	//-----------------------------------------------------------------------
	CXTPControlComboBox(CXTPCommandBars* pCommandBars = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlComboBox object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPControlComboBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the style of edit control.
	// Returns:
	//     The style of edit control.
	// See Also:
	//     SetEditStyle
	//-----------------------------------------------------------------------
	DWORD GetEditStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the style of edit control.
	// Parameters:
	//     dwStyle - The style to be set.
	// See Also:
	//     GetEditStyle
	//-----------------------------------------------------------------------
	void SetEditStyle(DWORD dwStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert/delete an edit box in the combobox
	//     when the control has focus.
	// Parameters:
	//     bSet - TRUE to set the combobox to have an edit control.
	// Remarks:
	//     If bSet is TRUE, an edit control will be used to display the text
	//     when the combobox control is clicked. This allows the user to
	//     edit or copy the text.
	// See Also:
	//     GetDropDownListStyle
	//-----------------------------------------------------------------------
	void SetDropDownListStyle(BOOL bSet = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the combobox has an edit control.
	// Returns:
	//     TRUE if the combobox has an edit control, otherwise FALSE.
	// See Also:
	//     SetDropDownListStyle
	//-----------------------------------------------------------------------
	BOOL GetDropDownListStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the dropdown list.
	// Parameters:
	//     nWidth - The width of the dropdown list.
	//-----------------------------------------------------------------------
	void SetDropDownWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the count of items in the dropdown list.
	// Parameters:
	//     nDropDownItemCount - The count of items in the dropdown list.
	//-----------------------------------------------------------------------
	void SetDropDownItemCount(int nDropDownItemCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the dropdown list width.
	// Returns:
	//     Width of the dropdown list.
	//-----------------------------------------------------------------------
	int GetDropDownWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a string to a list box.
	// Parameters:
	//     lpsz - The string that is to be added.
	// Returns:
	//     The zero-based index of the string in the list box. The return value is LB_ERR
	//     if an error occurs.
	//-----------------------------------------------------------------------
	int AddString(LPCTSTR lpsz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the number of items in a list box.
	// Returns:
	//     The number of items in the list box, or LB_ERR if an error occurs.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a string from the list box of a combobox control.
	// Parameters:
	//     nIndex - Contains the zero-based index of the list-box string to be copied.
	//     str - A reference to a CString.
	//-----------------------------------------------------------------------
	void GetLBText(int nIndex, CString& str) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all items from the list box of a combobox control.
	//-----------------------------------------------------------------------
	void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the first list-box string (in a combobox) that
	//     matches the string specified in str.
	// Parameters:
	//     nIndexStart - Specifies the zero-based index of the item before the first item
	//                   to be searched.
	//     lpsz - The string to search for.
	// Returns:
	//     The zero-based index of the matching item, or CB_ERR if the search was
	//     unsuccessful.
	// See Also:
	//     FindString
	//-----------------------------------------------------------------------
	int FindStringExact(int nIndexStart, LPCTSTR lpsz) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert a string into the list box of a
	//     combobox control.
	// Parameters:
	//     nIndex - Contains the zero-based index to the position in the list box that
	//              will receive the string.
	//     lpsz - The string that is to be inserted.
	// Returns:
	//     The zero-based index of a newly inserted item, or CB_ERR if insertion was
	//     unsuccessful.
	//-----------------------------------------------------------------------
	int InsertString(int nIndex, LPCTSTR lpsz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently selected item's text
	// Returns:
	//     The text that is currently selected.
	//-----------------------------------------------------------------------
	virtual CString GetListBoxText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the edit control of the combobox control.
	// Returns:
	//     A pointer to the CEdit control.
	//-----------------------------------------------------------------------
	CXTPControlComboBoxEditCtrl* GetEditCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a string in the list box of a combobox.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the string to select.
	//-----------------------------------------------------------------------
	void SetCurSel(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine which item in the combobox is selected.
	// Returns:
	//     The zero-based index of the currently selected item in the list box of a combobox,
	//     or CB_ERR if no item is selected.
	//-----------------------------------------------------------------------
	int GetCurSel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call the GetDroppedState member function to determine whether the list box
	//     of a drop-down combobox is visible (dropped down).
	// Returns:
	//     Nonzero if the list box is visible; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL GetDroppedState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the edit text.
	// Returns:
	//     The Edit control text.
	//-----------------------------------------------------------------------
	CString GetEditText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to modify the style of a list box.
	// Parameters:
	//     dwRemove - Styles to remove.
	//     dwAdd - Styles to add.
	//-----------------------------------------------------------------------
	void ModifyListBoxStyle(DWORD dwRemove, DWORD dwAdd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the edit control text.
	// Parameters:
	//     lpszText - New text of the edit control.
	// See Also:
	//     FindStringExact, FindString
	//-----------------------------------------------------------------------
	void SetEditText(const CString& lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the grayed-out text displayed in the edit control
	//     which provides a helpful description of what the control is used for.
	// Parameters:
	//     lpszEditHint - Edit hint to be set.
	// Example:
	//     <code>pCombo->SetEditHint(_T("Click to find a contact");</code>
	// See Also:
	//     GetEditHint
	//-----------------------------------------------------------------------
	void SetEditHint(LPCTSTR lpszEditHint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the grayed-out text displayed in the edit control
	//     which provides a helpful description of what the control is used for.
	// Returns:
	//     Edit hint of the control
	// See Also:
	//     SetEditHint
	//-----------------------------------------------------------------------
	CString GetEditHint() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the default char format of rich edit text.
	//-----------------------------------------------------------------------
	virtual CHARFORMAT2 GetDefaultCharFormat();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables or disables shell auto completion.
	// Parameters:
	//     dwFlags - Flags that will be passed to SHAutoComplete function.
	// Remarks:
	//     Flags can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>SHACF_FILESYSTEM</b>: This includes the File System as well as the rest of
	//                                the shell (Desktop\My Computer\Control Panel\).
	//     * <b>SHACF_URLALL</b>:  Include the URL's in the users History and Recently Used
	//                             lists. Equivalent to SHACF_URLHISTORY | SHACF_URLMRU.
	//     * <b>HACF_URLHISTORY</b>: URLs in the User's History.
	//     * <b>SHACF_URLMRU</b>: URLs in the User's Recently Used list.
	//     * <b>SHACF_FILESYS_ONLY</b>: Include only the file system. Do not include
	//                                  virtual folders such as Desktop or Control Panel.
	// ---------------------------------------------------------------------------
	void EnableShellAutoComplete(DWORD dwFlags = SHACF_FILESYSTEM | SHACF_URLALL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables or disables auto completion.
	// Parameters:
	//     bAutoComplete - TRUE to enable auto completion, otherwise FALSE.
	//-----------------------------------------------------------------------
	void EnableAutoComplete(BOOL bAutoComplete = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method finds the first string in a list box that contains the
	//     specified prefix, without changing the list-box selection.
	// Parameters:
	//     nStartAfter - Contains the zero-based index of the item before the
	//                   first item to be searched. When the search reaches the
	//                   bottom of the list box, it continues from the top of
	//                   the list box back to the item specified by nStartAfter.
	//                   If nStartAfter is -1, the entire list box is searched
	//                   from the beginning.
	//     lpszItem    - Points to the null-terminated string that contains the
	//                   prefix to search for. The search is case independent,
	//                   so this string may contain any combination of uppercase
	//                   and lowercase letters.
	// Returns:
	//     The zero-based index of the matching item, or LB_ERR if the search
	//     was unsuccessful.
	// See Also:
	//     FindStringExact
	//-----------------------------------------------------------------------
	int FindString(int nStartAfter, LPCTSTR lpszItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied, 32-bit value associated
	//     with the specified combobox item.
	// Parameters:
	//     nIndex - Contains the zero-based index of an item in the combobox's list box.
	// Returns:
	//     The 32-bit value associated with the item, or CB_ERR if an error occurs.
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the specified item in
	//     a combobox.
	// Parameters:
	//     nIndex     - Contains a zero-based index of the item to set.
	//     dwItemData - Contains the new value to associate with the item.
	// Returns:
	//     CB_ERR if an error occurs.
	//-----------------------------------------------------------------------
	int SetItemData(int nIndex, DWORD_PTR dwItemData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to delete a string.
	// Parameters:
	//     nIndex - Contains a zero-based index of the item to delete.
	//-----------------------------------------------------------------------
	void DeleteItem(long nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control's enabled state is changed.
	//-----------------------------------------------------------------------
	void OnEnabledChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to compare controls.
	// Parameters:
	//     pOther - The control to compare with.
	// Returns:
	//     TRUE if the controls are identical, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Compare(CXTPControl* pOther);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the width of a label.
	// Returns:
	//     Width of the label.
	// See Also:
	//     SetLabelWidth, SetStyle, GetStyle
	//-----------------------------------------------------------------------
	int GetLabelWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the width of a label.
	// Parameters:
	//     nLabelWidth - Width of the label to be set.
	// See Also:
	//     GetLabelWidth, SetStyle, GetStyle
	//-----------------------------------------------------------------------
	void SetLabelWidth(int nLabelWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the caption of the control is visible.
	// Returns:
	//     TRUE if the caption of the control is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the icon is visible for a combobox control.
	// Returns:
	//     TRUE if the icon is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsImageVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the width of a thumb button.
	// Returns:
	//     Width of the thumb button.
	// See Also:
	//     SetThumbWidth
	//-----------------------------------------------------------------------
	int GetThumbWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the width of a thumb button.
	// Parameters:
	//     nThumbWidth - Width of the thumb button to be set.
	// See Also:
	//     GetThumbWidth
	//-----------------------------------------------------------------------
	void SetThumbWidth(int nThumbWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if control has focus.
	//-----------------------------------------------------------------------
	BOOL HasFocus() const;

	//----------------------------------------------------------------------
	// Summary:
	//     This method draws the text of control if the style is CBS_DROPDOWNLIST.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcText - Rectangle to draw.
	//----------------------------------------------------------------------
	virtual void DrawEditText(CDC* pDC, CRect rcText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves list box window.
	//-----------------------------------------------------------------------
	CListBox* GetListBoxCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns child pop-up bar.
	//-----------------------------------------------------------------------
	CXTPControlComboBoxPopupBar* GetComboBoxPopupBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the edit icon's identifier.
	// Parameters:
	//     nId - Icon's identifier to be set.
	//-----------------------------------------------------------------------
	void SetEditIconId(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the edit icon's identifier.
	// Returns:
	//     Edit icon's identifier.
	//-----------------------------------------------------------------------
	int GetEditIconId() const;
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the combo list icon's identifier.
	// Parameters:
	//     nId - Icon's identifier to be set.
	//-----------------------------------------------------------------------
	void SetListIconId(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the combo list icon's identifier.
	// Returns:
	//     Combo list icon's identifier.
	//-----------------------------------------------------------------------
	int GetListIconId() const;

	//{{AFX_CODEJOCK_PRIVATE

	// deprecated
	virtual CString GetText() const
	{
		return GetListBoxText();
	}
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set focus to the control.
	// Parameters:
	//     bFocused - TRUE to set focus to the control.
	//-----------------------------------------------------------------------
	virtual void SetFocused(BOOL bFocused);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the focused state of the control.
	// Returns:
	//     TRUE if the control has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when edit control's text is changed
	//-----------------------------------------------------------------------
	virtual void OnEditChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the selected string is changed.
	//-----------------------------------------------------------------------
	virtual void OnSelChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control is executed.
	//-----------------------------------------------------------------------
	virtual void OnExecute();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to hide the control.
	// Parameters:
	//     dwFlags - Reasons to hide.
	// See Also:
	//     XTPControlHideFlags
	//-----------------------------------------------------------------------
	virtual void SetHideFlags(DWORD dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an action property is changed
	// Parameters:
	//     nProperty - Property of the action.
	// See Also:
	//     OnActionChanging
	//-----------------------------------------------------------------------
	virtual void OnActionChanged(int nProperty);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an action property is about to be changed.
	// Parameters:
	//     nProperty - Property of the action
	// See Also:
	//     OnActionChanged
	//-----------------------------------------------------------------------
	virtual void OnActionChanging(int nProperty);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the maximum text length that the user may enter
	//     into an edit control.
	// Parameters:
	//     nTextLimit - Maximum text length user may enter.
	// See Also:
	//     GetTextLimit
	//-----------------------------------------------------------------------
	void SetTextLimit(int nTextLimit);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets the maximum text length that the user may enter
	//     into an edit control.
	// Returns:
	//     Maximum text length user may enter.
	// See Also:
	//     SetTextLimit
	//-----------------------------------------------------------------------
	int GetTextLimit() const;

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to check if a control can accept focus.
	// See Also:
	//     SetFocused
	//----------------------------------------------------------------------
	virtual BOOL IsFocusable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates an edit control. Override it to use inherited edit control.
	//-----------------------------------------------------------------------
	virtual CXTPControlComboBoxEditCtrl* CreateEditControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called after the mouse hovers over the control.
	//-----------------------------------------------------------------------
	void OnMouseHover();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the edit control gets focus.
	// Parameters:
	//     pOldWnd - Pointer to a CWnd object.
	//-----------------------------------------------------------------------
	virtual void OnSetFocus(CWnd* pOldWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the edit control loses focus.
	//-----------------------------------------------------------------------
	virtual void OnKillFocus();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the real rect of an edit control of a combobox.
	// Parameters:
	//     rcControl - Rectangle of combobox area.
	//-----------------------------------------------------------------------
	virtual void DeflateEditRect(CRect& rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnSetSelected(int bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the bounding rectangle of the control.
	// Parameters:
	//     rcControl - Bounding rectangle of the control.
	//-----------------------------------------------------------------------
	void SetRect(CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user activates a control using its underline.
	//----------------------------------------------------------------------
	virtual void OnUnderlineActivate();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Pointer to a source CXTPControl object.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     TRUE if the key was handled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign a parent command bar object.
	// Parameters:
	//     pParent - Pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	void SetParent(CXTPCommandBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command
	//     bar size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar. See Remarks section for a list of
	//              values.
	// Remarks:
	//     The following predefined flags are used to determine the height and
	//     width of the dynamic command bar. Use the bitwise-OR (|) operator to
	//     combine the flags.<p/>
	//
	//     * <b>LM_STRETCH</b>: Indicates whether the command bar should be
	//                          stretched to the size of the frame. Set if the bar is
	//                          not a docking bar (not available for docking). Not set
	//                          when the bar is docked or floating (available for
	//                          docking). If set, LM_STRETCH returns dimensions based
	//                          on the LM_HORZ state. LM_STRETCH works similarly to
	//                          the the bStretch parameter used in CalcFixedLayout;
	//                          see that member function for more information about
	//                          the relationship between stretching and orientation.
	//     * <b>LM_HORZ</b>: Indicates that the bar is horizontally or
	//                       vertically oriented. Set if the bar is horizontally
	//                       oriented, and if it is vertically oriented, it is not
	//                       set. LM_HORZ works similarly to the the bHorz
	//                       parameter used in CalcFixedLayout; see that member
	//                       function for more information about the relationship
	//                       between stretching and orientation.
	//     * <b>LM_MRUWIDTH</b>: Most Recently Used Dynamic Width. Uses the
	//                           remembered most recently used width.
	//     * <b>LM_HORZDOCK</b>: Horizontal Docked Dimensions. Returns the
	//                           dynamic size with the largest width.
	//     * <b>LM_VERTDOCK</b>: Vertical Docked Dimensions. Returns the dynamic
	//                           size with the largest height.
	//     * <b>LM_COMMIT</b>: Resets LM_MRUWIDTH to the current width of the
	//                         floating command bar.
	//
	//     The framework calls this member function to calculate the dimensions
	//     of a dynamic command bar.<p/>
	//
	//     Override this member function to provide your own layout in classes
	//     you derive from CXTPControl. XTP classes derived from CXTPControl,
	//     such as CXTPControlComboBox, override this member function to provide
	//     their own implementation.
	// See Also:
	//     CXTPControl, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to pop-up the control.
	// Parameters:
	//     bPopup - TRUE to set pop-up.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnSetPopup(BOOL bPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member checks if the user can resize the control.
	// Returns:
	//     TRUE if resize is available.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeResizeAllow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member returns the minimum width that the combobox
	//     can be sized to by the user while in customization mode.
	// Returns:
	//     Width of label + Width of Dropdown button + 5
	//-----------------------------------------------------------------------
	virtual int GetCustomizeMinWidth() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when a control is removed from the
	//     parent controls collection.
	//-------------------------------------------------------------------------
	virtual void OnRemoved();

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	//----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//{{AFX_CODEJOCK_PRIVATE
protected:
public:
	void UpdatePopupSelection();

protected:
	BOOL IsValidList() const;
	void _SetEditText(const CString& lpszText);
	CString _GetEditText() const;
	virtual BOOL OnHookMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnThemeChanged();
	void ShowHideEditControl();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlComboBox);

	afx_msg void OleSetDropDownWidth(long nWidth);
	afx_msg void OleSetDropDownListStyle(BOOL bSet);
	afx_msg void OleAddItem(LPCTSTR strText, const VARIANT* varIndex);
	afx_msg BSTR OleList(long nIndex);
	afx_msg void OleClear();
	afx_msg void OleRemoveItem(long nIndex);

	afx_msg BSTR OleGetText();
	afx_msg void OleSetText(LPCTSTR str);
	afx_msg long OleGetListIndex();
	afx_msg void OleSetListIndex(long nIndex);
	afx_msg long OleFindItem(LPCTSTR strText);
	afx_msg int OleGetCount();
	afx_msg HWND OleGetEditHandle();

	afx_msg long OleGetItemData(int nIndex);
	afx_msg void OleSetItemData(int nIndex, long nNumber);
	afx_msg BSTR OleGetEditHint();
	afx_msg DWORD OleGetShellAutoComplete();
	afx_msg void OleSetShellAutoComplete(DWORD dwShellAutoCompleteFlags);
	afx_msg BOOL OleGetThemedItems();
	afx_msg void OleSetThemedItems(BOOL bThemedItems);

	afx_msg void OleSetEditIconId(int nId);
	afx_msg int OleGetEditIconId();
	afx_msg void OleSetListIconId(int nId);
	afx_msg int OleGetListIconId();

	enum
	{
		dispidStyle				= 50L,
		dispidAddItem			= 51L,
		dispidDropDownWidth		= 52L,
		dispidDropDownListStyle = 53L,
		dispidWidth				= 54L,
		dispidList				= 55L,
		dispidClear				= 56L,
		dispidListCount			= 57L,
		dispidRemoveItem		= 58L,
		dispidText				= 59L,
		dispidListIndex			= 60L,
		dispidFindItem			= 61L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif
	DECLARE_XTP_CONTROL(CXTPControlComboBox)

protected:
	CXTPControlComboBoxEditCtrl* m_pEdit; // Child edit control.
	BOOL m_bDropDown;					  // TRUE if the combo is dropdown.
	XTPButtonStyle m_comboStyle;		  // Style of the combobox.
	int m_nLastSel;			 // Last user selected index (used during display of list box).
	CString m_strLastText;   // Last Text before user select focus and change it.
	BOOL m_bDelayDestroy;	// TRUE if need to recreate control.
	BOOL m_bDelayReposition; // Need to reposition control.
	int m_nLabelWidth;		 // Width of the label.
	int m_nThumbWidth;		 // Width of the thumb area.
	CString m_strEditHint; // Grayed-out text displayed in the edit control that displayed a helpful
						   // description.
	BOOL m_bAutoComplete;  // TRUE if Auto Complete is enabled.
	BOOL m_bIgnoreAutoComplete;		  // TRUE to disable auto complete till next key event.
	DWORD m_dwShellAutoCompleteFlags; // Shell auto complete flags.
	BOOL m_bFocused;				  // TRUE if control is focused.
	int m_nEditIconId;				  // Edit Icon identifier.
	BOOL m_bSelEndOk;				  // TRUE if user selects a list item.
	int m_nDropDownItemCount;		  // Maximum drop down items.

	mutable CString m_strEditText; // Edit text.
	mutable BOOL m_bEditChanged;   // TRUE if Edit Text is changed.
	int m_nTextLimit; // The maximum number of characters that can be entered into an edit control.

	CXTPControlComboBoxAutoCompleteWnd* m_pAutoCompleteWnd; // Auto Complete hook window.
	DWORD m_dwEditStyle;									// Edit style.

private:
	int m_nCurSel;
	BOOL m_bIgnoreSelection;

	friend class CXTPControlComboBoxList;
	friend class CXTPControlComboBoxEditCtrl;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPControlComboBox::SetDropDownWidth(int nWidth)
{
	m_pCommandBar->SetWidth(nWidth);
}
AFX_INLINE int CXTPControlComboBox::AddString(LPCTSTR lpsz)
{
	return GetListBoxCtrl()->AddString(lpsz);
}
AFX_INLINE int CXTPControlComboBox::GetCount() const
{
	return GetListBoxCtrl()->GetCount();
}
AFX_INLINE void CXTPControlComboBox::GetLBText(int nIndex, CString& str) const
{
	GetComboBoxPopupBar()->GetText(nIndex, str);
}
AFX_INLINE void CXTPControlComboBox::ResetContent()
{
	GetListBoxCtrl()->ResetContent();
}
AFX_INLINE int CXTPControlComboBox::FindStringExact(int nIndexStart, LPCTSTR lpsz) const
{
	return GetComboBoxPopupBar()->FindStringExact(nIndexStart, lpsz);
}
AFX_INLINE int CXTPControlComboBox::InsertString(int nIndex, LPCTSTR lpsz)
{
	return GetListBoxCtrl()->InsertString(nIndex, lpsz);
}
AFX_INLINE CXTPControlComboBoxEditCtrl* CXTPControlComboBox::GetEditCtrl() const
{
	return m_pEdit;
}
AFX_INLINE int CXTPControlComboBox::FindString(int nStartAfter, LPCTSTR lpszItem) const
{
	return GetComboBoxPopupBar()->FindString(nStartAfter, lpszItem);
}
AFX_INLINE DWORD_PTR CXTPControlComboBox::GetItemData(int nIndex) const
{
	return (DWORD_PTR)GetListBoxCtrl()->GetItemData(nIndex);
}
AFX_INLINE int CXTPControlComboBox::SetItemData(int nIndex, DWORD_PTR dwItemData)
{
	return GetListBoxCtrl()->SetItemData(nIndex, dwItemData);
}
AFX_INLINE BOOL CXTPControlComboBox::IsCustomizeResizeAllow() const
{
	return TRUE;
}
AFX_INLINE CXTPControlComboBox* CXTPControlComboBoxEditCtrl::GetControlComboBox() const
{
	return m_pControl;
}
AFX_INLINE int CXTPControlComboBox::GetLabelWidth() const
{
	return m_nLabelWidth;
}
AFX_INLINE int CXTPControlComboBox::GetEditIconId() const
{
	return m_nEditIconId;
}
AFX_INLINE int CXTPControlComboBox::GetThumbWidth() const
{
	return m_nThumbWidth;
}
AFX_INLINE void CXTPControlComboBox::OnThemeChanged()
{
	m_bDelayReposition = TRUE;
}
AFX_INLINE void CXTPControlComboBox::SetDropDownItemCount(int nDropDownItemCount)
{
	m_nDropDownItemCount = nDropDownItemCount;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCONTOLCOMBOBOX_H__)
