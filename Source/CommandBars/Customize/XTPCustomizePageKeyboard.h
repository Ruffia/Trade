// XTPCustomizePageKeyboard.h : interface for the CXTPCustomizeKeyboardPage class.
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
#if !defined(__XTPCUSTOMIZEKEYBOARDPAGE_H__) && defined(_XTP_INCLUDE_CONTROLS)
#	define __XTPCUSTOMIZEKEYBOARDPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCustomizeSheet;
class CXTPShortcutManagerAccelTable;

//===========================================================================
// Summary:
//     CXTPCustomizeKeyboardPage is a CPropertyPage derived class.
//     It represents the Keyboard page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeKeyboardPage : public CXTPPropertyPage
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeKeyboardPage object
	// Parameters:
	//     pSheet - Points to a CXTPCustomizeSheet object that this page
	//     belongs to.
	//-----------------------------------------------------------------------
	CXTPCustomizeKeyboardPage(CXTPCustomizeSheet* pSheet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeKeyboardPage object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	~CXTPCustomizeKeyboardPage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Finds the category.
	// Parameters:
	//     strCategory - Category needed to find.
	//-----------------------------------------------------------------------
	XTP_COMMANDBARS_CATEGORYINFO* FindCategory(LPCTSTR strCategory) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the category by its index.
	// Parameters:
	//     nIndex - Index of category
	//-----------------------------------------------------------------------
	XTP_COMMANDBARS_CATEGORYINFO* GetCategoryInfo(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method adds a new empty category in the given index.
	// Parameters:
	//     strCategory - Category to be added.
	//     nIndex - Index to insert.
	// Returns:
	//     A pointer to a CXTPControls object
	//-----------------------------------------------------------------------
	CXTPControls* InsertCategory(LPCTSTR strCategory, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new category from a CMenu object.
	// Parameters:
	//     strCategory - Category to be added.
	//     pMenu - Points to a CMenu object
	//     bListSubItems - TRUE to add sub menus to categories.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategory(LPCTSTR strCategory, CMenu* pMenu, BOOL bListSubItems = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds categories branch from Control or resource ID.
	// Parameters:
	//     pControls - Points to a CXTPControls object
	//     nIDResource - Menu resource from where categories will be built.
	//     bListSubItems - TRUE to add sub menus to categories.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategories(CXTPControls* pControls);
	BOOL AddCategories(
		UINT nIDResource,
		BOOL bListSubItems = FALSE); //<combine
									 // CXTPCustomizeKeyboardPage::AddCategories@CXTPControls*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the page to reload the list
	//     items.
	//-----------------------------------------------------------------------
	void ReloadList();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the page to enable the
	//     assignment buttons.
	//-----------------------------------------------------------------------
	void EnableControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves frame accelerators.
	//-----------------------------------------------------------------------
	CXTPShortcutManagerAccelTable* GetFrameAccelerator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates frame accelerators.
	// Parameters:
	//     lpAccel - Accelerator table to set.
	//     nSize - Number of items.
	//-----------------------------------------------------------------------
	void UpdateAcellTable(LPACCEL lpAccel, int nSize);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPCustomizeKeyboardPage)
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCustomizeKeyboardPage)
	afx_msg void OnSelchangeCategory();
	afx_msg void OnSelchangeCommands();
	afx_msg void OnSelchangeCurKeys();
	afx_msg void OnChangeShortcutKey();
	afx_msg void OnAssign();
	afx_msg void OnResetAll();
	afx_msg void OnRemove();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

public:
	enum
	{
		IDD = XTP_IDD_PAGE_KEYBOARD // Property page identifier
	};

#	ifdef _XTP_INCLUDE_CONTROLS
	COLORREF m_clrText;
	CXTPButton m_btnAssign;
	CXTPButton m_btnRemove;
	CXTPButton m_btnResetAll;
	CXTPListBox m_lboxKeys;
	CXTPListBox m_lboxCommands;
	CXTPComboBox m_comboCategory;
#	else
	CButton m_btnAssign;
	CButton m_btnRemove;
	CButton m_btnResetAll;
	CListBox m_lboxKeys;	   // Keys list box
	CListBox m_lboxCommands;   // Commands list box
	CComboBox m_comboCategory; // Categories combobox
#	endif
	CString m_strDesc;								   // Description of selected control
	int m_iCategory;								   // Selected category
	CXTPShortcutManager::CKeyAssign m_editShortcutKey; // Shortcut edit control

protected:
	CXTPCommandBarsCategoryArray m_arrCategories; // Array of categories
	CXTPCustomizeSheet* m_pSheet;				  // Parent Customize dialog
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCUSTOMIZEKEYBOARDPAGE_H__)
