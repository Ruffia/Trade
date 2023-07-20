// XTPPropertySheet.h : interface for the CXTPPropertySheet class.
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
#if !defined(__XTPPROPERTYSHEET_H__)
#	define __XTPPROPERTYSHEET_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDialogEx;
class CXTPPropertyPageNavigator;
class CXTPPropertyPage;

//-----------------------------------------------------------------------
// Summary:
//     XTPPropertyPageBorder is an enumeration used to set the client frame style
//     drawn around the client of tabs.
// Example:
//     <code>wndSheet.SetPageBorderStyle(xtpPageBorderFrame);</code>
// See Also:
//     CXTPPropertySheet
//
// <KEYWORDS xtpPageBorderNone, xtpPageBorderBottomLine, xtpPageBorderFrame>
//-----------------------------------------------------------------------
enum XTPPropertyPageBorder
{
	xtpPageBorderNone,
	xtpPageBorderBottomLine,
	xtpPageBorderFrame
};

//===========================================================================
// Summary:
//     CXTPPropertySheet is a CXTPDialogEx derived class. It represents a property
//     sheet, otherwise known as a tab dialog box. A property sheet consists of a
//     CXTPPropertySheet object and one or more CXTPPropertyPage objects.
// See Also:
//     CXTPPropertySheet::DoModal, CXTPPropertyPage, CXTPPropertyPageNavigator
//===========================================================================

class _XTP_EXT_CLASS CXTPPropertySheet : public CXTPDialogEx
{
	DECLARE_DYNAMIC(CXTPPropertySheet)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertySheet object.
	// Parameters:
	//     nIDCaption - ID of the caption to be used for the property sheet.
	//     pParentWnd - Pointer to the parent window of the property sheet. If NULL, the
	//                  parent window will be the main window of the application.
	//     iSelectPage - The index of the page that will initially be on top. Default is
	//                   the first page added to the sheet.
	//     pszCaption - Pointer to a string containing the caption to be used for the
	//                  property sheet. Cannot be NULL.
	// Remarks:
	//     To display the property sheet, call DoModal or Create.
	//     The string contained in the first parameter will be placed in the caption bar
	//     for the property sheet.
	//-----------------------------------------------------------------------
	CXTPPropertySheet();
	CXTPPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL,
					  UINT iSelectPage = 0); // <combine CXTPPropertySheet::CXTPPropertySheet>
	CXTPPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL,
					  UINT iSelectPage = 0); // <combine CXTPPropertySheet::CXTPPropertySheet>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertySheet object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPPropertySheet();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Displays a modeless property sheet.
	// Parameters:
	//     pParentWnd - Pointer to the parent window. If NULL, the parent is the desktop.
	//     dwStyle - Window styles for property sheet.
	//     dwExStyle  - Extended window styles for property sheet.
	// Remarks:
	//     The default style, expressed by passing -1 as dwStyle, is actually
	//     WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXT_HELP | WS_VISIBLE.
	//
	//     To display a modal property sheet, call DoModal instead.
	// See Also:
	//     CXTPPropertySheet::DoModal
	//-----------------------------------------------------------------------
	BOOL Create(CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0);

	using CXTPDialogEx::Create;
#	if (1200 < _MSC_VER)
	using CWnd::Create;
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to display a modal property sheet.
	// Returns:
	//     IDOK or IDCANCEL if the function was successful, otherwise 0 or -1.
	// Remarks:
	//     The return value corresponds to the ID of the control that closed the
	//     property sheet. After this function returns, the windows corresponding
	//     to the property sheet and all the pages will have been destroyed.
	//
	//     To display a modeless property sheet, call Create instead.
	// See Also:
	//     CXTPPropertySheet::Create
	//-----------------------------------------------------------------------
	INT_PTR DoModal();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a page to the property sheet.
	// Parameters:
	//     pPage  - Pointer to the page to be added to the property sheet. Cannot be NULL.
	//     nIndex - Index to insert the page.
	// Remarks:
	//     This member function adds the supplied page with the rightmost tab in
	//     the property sheet. Add pages to the property sheet in the left-to-right
	//     order you want them to appear.
	//
	//     AddPage adds the CPropertyPage object to the CPropertySheet object's list
	//     of pages but does not actually create the window for the page. The framework
	//     postpones creation of the window for the page until the user selects that page.
	// See Also:
	//     CXTPPropertySheet::GetPageCount
	//-----------------------------------------------------------------------
	void AddPage(CXTPPropertyPage* pPage);
	void InsertPage(int nIndex,
					CXTPPropertyPage* pPage); // <combine
											  // CXTPPropertySheet::AddPage@CXTPPropertyPage*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function removes a page from the property sheet and destroys
	//     the associated window.
	// Parameters:
	//     pPage - Pointer to the page to be removed from the property sheet. Cannot be NULL.
	//     nPage - Index of the page to be removed. Must be between 0 and one less than
	//             the number of pages in the property sheet, inclusive.
	// See Also:
	//     CXTPPropertySheet::AddPage
	//-----------------------------------------------------------------------
	void RemovePage(int nPage);
	void RemovePage(CXTPPropertyPage* pPage); // <combine CXTPPropertySheet::RemovePage@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the number of pages currently in
	//     the property sheet.
	// Returns:
	//     The number of pages in the property sheet.
	// See Also:
	//     CXTPPropertySheet::AddPage, CXTPPropertySheet::GetPage
	//-----------------------------------------------------------------------
	int GetPageCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to a specified page in
	//     the property sheet.
	// Parameters:
	//     nPage - Index of the desired page, starting at 0. Must be between 0 and
	//             one less than the number of pages in the property sheet, inclusive.
	// Returns:
	//     The pointer to the page corresponding to the nPage parameter.
	// See Also:
	//     CXTPPropertySheet::AddPage, CXTPPropertySheet::GetPageCount
	//-----------------------------------------------------------------------
	CXTPPropertyPage* GetPage(int nPage) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the index number of a specified page in
	//     the property sheet.
	// Parameters:
	//     pPage - Pointer to the page with the index to be found. Cannot be NULL.
	// Returns:
	//     The index number of the page corresponding to the pPage parameter.
	// Remarks:
	//     This method can be used to get the page index number for the
	//     SetActivePage or GetPage methods.
	// See Also:
	//     CXTPPropertySheet::GetActivePage
	//-----------------------------------------------------------------------
	int GetPageIndex(CXTPPropertyPage* pPage) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to the property sheet
	//     window's active page.
	// Returns:
	//     The pointer to the property sheet window's active page.
	// Remarks:
	//     This method can be used to perform some action on the active page.
	// See Also:
	//     CXTPPropertySheet::GetPage
	//-----------------------------------------------------------------------
	CXTPPropertyPage* GetActivePage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the index number of the property sheet
	//     window's active page.
	// Returns:
	//     The index number of the property sheet window's active page.
	// Remarks:
	//     The return value for this method can be used as the parameter for GetPage.
	// See Also:
	//     CXTPPropertySheet::GetActivePage
	//-----------------------------------------------------------------------
	int GetActiveIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the active page.
	// Parameters:
	//     pPage - Pointer to the page to set in the property sheet. It cannot be NULL.
	//     nPage - Index of the page to set. It must be between 0 and one less than
	//             the number of pages in the property sheet, inclusive.
	// Returns:
	//     Nonzero if the property sheet is activated successfully, otherwise 0.
	// Remarks:
	//     This method should be used if a user's action on one page should cause
	//     another page to become the active page.
	// See Also:
	//     CXTPPropertySheet::GetActivePage
	//-----------------------------------------------------------------------
	virtual BOOL SetActivePage(int nPage);
	virtual BOOL SetActivePage(CXTPPropertyPage* pPage); // <combine
														 // CXTPPropertySheet::SetActivePage@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the property sheet's caption
	//     (the text displayed in the title bar of a frame window).
	// Parameters:
	//     lpszText - Pointer to the text to be used as the caption in the title bar
	//                of the property sheet.
	// See Also:
	//     CXTPPropertySheet::GetPage
	//-----------------------------------------------------------------------
	void SetTitle(LPCTSTR lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the CRect bounding rectangle for all property pages.
	// Returns:
	//     The CRect bounding rectangle for all property pages.
	//-----------------------------------------------------------------------
	CRect GetPageRect() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the currently used navigator object.
	// Returns:
	//     A CXTPPropertyPageNavigator class pointer.
	// See Also:
	//     CXTPPropertySheet::SetNavigator
	//-----------------------------------------------------------------------
	CXTPPropertyPageNavigator* GetNavigator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the navigator object that will be used to switch property pages.
	// Parameters:
	//     pNavigator - A CXTPPropertyPageNavigator class pointer that will be set
	//                  as the new navigator object
	// Remarks:
	//     CXTPPropertyPageNavigator class used to specify how the user will navigate
	//     and switch pages. Default navigator classes are:
	//     * <b>CXTPPropertyPageTabNavigator</b>: Represents TabControl navigator.
	//     * <b>CXTPPropertyPageListNavigator</b>: Represents List navigator.
	//     * <b>CXTPPropertyPageTreeNavigator</b>: Represents Tree navigator.
	// Example:
	// <code>
	//     CXTPPropertyPageListNavigator* pList = new CXTPPropertyPageListNavigator();
	//     pList->SetListStyle(xtpListBoxOffice2007);
	//     ps.SetNavigator(pList);
	// </code>
	// See Also:
	//     CXTPPropertySheet::GetNavigator
	//-----------------------------------------------------------------------
	void SetNavigator(CXTPPropertyPageNavigator* pNavigator);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new button to the property sheet window.
	// Parameters:
	//     nIDCaption - ID of the caption to be used for the property sheet button.
	//     nID        - Identifier to be used for the property sheet button.
	// Returns:
	//     Nonzero if the property sheet is activated successfully, otherwise 0.
	// See Also:
	//     CXTPPropertySheet::RemoveButtons
	//-----------------------------------------------------------------------
	virtual BOOL AddButton(UINT nIDCaption, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes all buttons from the property sheet.
	// See Also:
	//     CXTPPropertySheet::AddButton
	//-----------------------------------------------------------------------
	void RemoveButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a border style around a client page rectangle.
	// Parameters:
	//     nBorder - Border style to be set.
	// See Also:
	//     XTPPropertyPageBorder, CXTPPropertySheet::GetPageBorderStyle
	//-----------------------------------------------------------------------
	void SetPageBorderStyle(XTPPropertyPageBorder nBorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the border style around a client page rectangle.
	// Returns:
	//     An XTPPropertyPageBorder border style.
	// See Also:
	//     XTPPropertyPageBorder, CXTPPropertySheet::SetPageBorderStyle
	//-----------------------------------------------------------------------
	XTPPropertyPageBorder GetPageBorderStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow the user to resize the property sheet.
	// Parameters:
	//     bResizable - TRUE to allow the user to resize the property sheet.
	//                  By default, this parameter is TRUE.
	//-----------------------------------------------------------------------
	void SetResizable(BOOL bResizable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom size for property pages.
	// Parameters:
	//     szPage - New property page size to be set.
	//-----------------------------------------------------------------------
	void SetPageSize(CSize szPage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a minimun size for property pages.
	// Parameters:
	//     szPage - New property page minimun size to be set.
	//-----------------------------------------------------------------------
	void SetPageSizeMin(CSize szPage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a maximum size for property pages.
	// Parameters:
	//     szPage - New property page maximum size to be set.
	//-----------------------------------------------------------------------
	void SetPageSizeMax(CSize szPage);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the window placement to the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the
	//                  Windows registry where placement information is stored.
	// Returns:
	//     TRUE if successful.
	//-----------------------------------------------------------------------
	BOOL SavePlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the
	//                  Windows registry where placement information is stored.
	// Returns:
	//     TRUE if position was changed.
	//-----------------------------------------------------------------------
	BOOL LoadPlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry. This version is the same as LoadPlacement but there
	//     is no need for calling SavePlacement when the window is destroyed.
	//     This will be called automatically.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the
	//                  Windows registry where placement information is stored.
	// Returns:
	//     TRUE if position was changed.
	//-----------------------------------------------------------------------
	BOOL AutoLoadPlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Forces all pages to update themselves as the data that they rely on
	//     is supposed to be changed from outside.
	//-----------------------------------------------------------------------
	void UpdatePages();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed
	//              in the Remarks section.
	// Remarks:
	//     nTheme can be one of the theme IDs specified by XTPControlTheme.
	// See Also:
	//     XTPControlTheme
	//-----------------------------------------------------------------------
	void SetTheme(XTPControlTheme theme);

	XTPControlTheme GetTheme();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create all property sheet buttons.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CreateButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create a property page window.
	// Parameters:
	//     pPage - Page to be created.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CreatePage(CXTPPropertyPage* pPage);

	//{{AFX_CODEJOCK_PRIVATE
public:
	CFont* GetFont();
	void EndDialog(int nEndID);
	virtual void RecalcLayout(LPSIZE pPageSize = NULL, BOOL bMoveWindow = FALSE);
	BOOL IsWizard() const;
	HWND GetNavigatorHwnd() const;

protected:
	void CommonConstruct(CWnd* pParentWnd, UINT iSelectPage);

protected:
	virtual BOOL CreateClient();
	void ResizeParentToFit(CSize szClient, BOOL bMoveWindow);
	BOOL IsPositionRelativeToWindow();
	virtual void MoveButtons(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient);
	void CreateFont(CXTPPropertyPage* pActivePage);

	virtual CXTPButton* CreateButton();

private:
	void PageInfoChanged(CXTPPropertyPage* pPage, BOOL bModified);
	BOOL OnButtonClick(UINT nButton);
	LRESULT SendPageNotify(CXTPPropertyPage* pPage, int code, LPARAM lParam = 0);
	TCHAR ToUpper(TCHAR vkTCHAR);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_VIRTUAL(CXTPPropertySheet)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertySheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* pMMI);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CString m_strCaption;					 // Caption of the dialog.
	LPDLGTEMPLATE m_lpDlgTemplate;			 // Dialog template.
	CArray<CButton*, CButton*> m_arrButtons; // Array of buttons of property sheet.
	CXTPPropertyPageNavigator* m_pNavigator; // Currently used navigator.
	CXTPFont m_xtpFontNormal;				 // Font used to draw buttons and navigator.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntNormal, m_xtpFontNormal, GetNormalFontHandle);
	CArray<CXTPPropertyPage*, CXTPPropertyPage*> m_arrPages; // Array of propety pages.
	CXTPPropertyPage* m_pActivePage;						 // Pointer to active property page.
	CRect m_rcPage;											 // Bounding rectangle of active page.
	CRect m_rcMinPage;			// Minimum bounding rectangle of active page.
	CRect m_rcMaxPage;			// Maximum bounding rectangle of active page.
	BOOL m_bAllowApply;			// TRUE to allow enable apply button.
	BOOL m_bModeless;			// TRUE if property sheet was created as modeless window.
	BOOL m_bResizable;			// TRUE if dialog can be resized.
	CSize m_szWindow;			// Last window size of property sheet.
	CSize m_szButton;			// Button size.
	CXTPScrollBar m_scSizeIcon; // Resize glyph.
	XTPPropertyPageBorder m_nPageBorderStyle; // Client border style.

	CString m_strSection;

	CXTPBrush m_brBack;

public:
	PROPSHEETHEADER m_psh; // The Windows PROPSHEETHEADER structure. Provides access to basic
						   // property sheet parameters.
};

AFX_INLINE int CXTPPropertySheet::GetPageCount() const
{
	return (int)m_arrPages.GetSize();
}
AFX_INLINE CXTPPropertyPageNavigator* CXTPPropertySheet::GetNavigator() const
{
	return m_pNavigator;
}
AFX_INLINE CFont* CXTPPropertySheet::GetFont()
{
	return &m_xtpFontNormal;
}
AFX_INLINE CRect CXTPPropertySheet::GetPageRect() const
{
	return m_rcPage;
}
AFX_INLINE XTPPropertyPageBorder CXTPPropertySheet::GetPageBorderStyle() const
{
	return m_nPageBorderStyle;
}
AFX_INLINE INT_PTR CXTPPropertySheet::DoModal()
{
	return CXTPDialogEx::DoModal();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPPROPERTYSHEET_H__)
