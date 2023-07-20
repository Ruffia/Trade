// XTPRibbonCustomizeQuickAccessPage.h: interface for the CXTPRibbonCustomizeQuickAccessPage class.
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
#if !defined(__XTPRIBBONCUSTOMIZEQUICKACCESSPAGE_H__)
#	define __XTPRIBBONCUSTOMIZEQUICKACCESSPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCustomizeSheet;
class CXTPCommandBars;
class CXTPRibbonBar;
struct XTP_COMMANDBARS_CATEGORYINFO;

//===========================================================================
// Summary:
//     CXTPRibbonCustomizeQuickAccessPage is a CPropertyPage derived class.
//     It represents the Quick Access page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonCustomizeQuickAccessPage : public CXTPPropertyPage
{
	// Construction
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonCustomizeQuickAccessPage object.
	// Parameters:
	//     pSheet       - Pointer to a CXTPCustomizeSheet object.
	//     nIDTemplate  - Identifier of the dialog resource to be used
	//                    as the page in the Customize dialog.
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	//-----------------------------------------------------------------------
	CXTPRibbonCustomizeQuickAccessPage(CXTPCustomizeSheet* pSheet, UINT nIDTemplate = (UINT)-1);
	CXTPRibbonCustomizeQuickAccessPage(
		CXTPCommandBars*
			pCommandBars); // <combine
						   // CXTPRibbonCustomizeQuickAccessPage::CXTPRibbonCustomizeQuickAccessPage@CXTPCustomizeSheet*@UINT>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonCustomizeQuickAccessPage object, handles
	//     cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPRibbonCustomizeQuickAccessPage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the ribbon bar to customize.
	// Returns:
	//     A pointer to the ribbon bar to customize.
	//-----------------------------------------------------------------------
	CXTPRibbonBar* GetRibbonBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a categories branch from a menu resource.
	// Parameters:
	//     nIDResource   - Menu resource from which categories will be built.
	//     bListSubItems - TRUE to add sub-menus to categories.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL AddCategories(UINT nIDResource, BOOL bListSubItems = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a categories branch from Controls.
	// Parameters:
	//     pControls - Pointer to a CXTPControls object
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL AddCategories(CXTPControls* pControls);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new category from a CMenu object.
	// Parameters:
	//     strCategory   - Category to be added.
	//     pMenu         - Pointer to a CMenu object.
	//     bListSubItems - TRUE to add sub-menus to the category.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL AddCategory(LPCTSTR strCategory, CMenu* pMenu, BOOL bListSubItems = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new empty category at a specified index.
	// Parameters:
	//     strCategory - Category to be added.
	//     nIndex      - Index to insert at.
	// Returns:
	//     A pointer to a CXTPControls object.
	//-----------------------------------------------------------------------
	CXTPControls* InsertCategory(LPCTSTR strCategory, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the control list for a specified category.
	// Parameters:
	//     strCategory - Category with the control list to retrieve.
	// Returns:
	//     A pointer to a CXTPControls object.
	//-----------------------------------------------------------------------
	CXTPControls* GetControls(LPCTSTR strCategory);

	//-----------------------------------------------------------------------
	// Summary:
	//     Forces a page to update its internal data as this data is supposed
	//     to be changed from outside the page.
	//-----------------------------------------------------------------------
	virtual void UpdatePage(void);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refills the list of Quick Access controls.
	//-----------------------------------------------------------------------
	void RefreshQuickAccessList();

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPRibbonCustomizeQuickAccessPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPRibbonCustomizeQuickAccessPage)
	afx_msg void OnCheckShowQuickAccessBelow();
	virtual BOOL OnInitDialog();
	afx_msg void OnCategoriesSelectionChanged();
	afx_msg void OnDblclkListQuickAccess();
	afx_msg void OnDblclkListCommands();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonReset();
	afx_msg void OnCommandsSelectionChanged();
	afx_msg void OnQuickAccessSelectionChanged();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

private:
	XTP_COMMANDBARS_CATEGORYINFO* FindCategory(LPCTSTR strCategory) const;
	XTP_COMMANDBARS_CATEGORYINFO* GetCategoryInfo(int nIndex);

public:
#	ifdef _XTP_INCLUDE_CONTROLS
	COLORREF m_clrText;
	CXTPButton m_btnAdd;
	CXTPButton m_btnRemove;
	CXTPButton m_btnReset;
	CXTPComboBox m_lstCategories;
	CXTPButton m_btnShowQuickAccessBelow;
#	else
	CButton m_btnAdd;
	CButton m_btnRemove;
	CButton m_btnReset;
	CComboBox m_lstCategories; // Categories list
	CButton m_btnShowQuickAccessBelow;
#	endif
	CXTPCustomizeCommandsListBox m_lstQuickAccess; // Quick Access list.
	CXTPCustomizeCommandsListBox m_lstCommands;	// Commands list.
	BOOL m_bShowQuickAccessBelow;				   // TRUE to show Quick Access below ribbon.

protected:
	CXTPCustomizeSheet* m_pSheet;				  // Parent Sheet window.
	CXTPCommandBars* m_pCommandBars;			  // Parent CommandBars object.
	CXTPCommandBarsCategoryArray m_arrCategories; // Array of categories.

#	ifdef _XTP_INCLUDE_MARKUP
	CXTPMarkupStatic& m_wndTitleIcon;
	CXTPMarkupStatic& m_wndTitle;
#	else
	CStatic& m_wndTitleIcon;
	CStatic& m_wndTitle;
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONCUSTOMIZEQUICKACCESSPAGE_H__)
