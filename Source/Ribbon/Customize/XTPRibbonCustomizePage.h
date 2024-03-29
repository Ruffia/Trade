// XTPRibbonCustomizePage.h: interface for the CXTPRibbonCustomizePage class.
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
#if !defined(__XTPRIBBONCUSTOMIZEPAGE_H__)
#	define __XTPRIBBONCUSTOMIZEPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCustomizeSheet;
class CXTPCommandBars;
class CXTPRibbonBar;
class CXTPCoreTreeItem;
class CXTPCoreTreeItems;

//===========================================================================
// Summary:
//     CXTPRibbonCustomizePage is a CPropertyPage derived class.
//     It represents the Quick Access page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonCustomizePage : public CXTPPropertyPage
{
	// Construction
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonCustomizePage object.
	// Parameters:
	//     pSheet       - Pointer to a CXTPCustomizeSheet object.
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	//-----------------------------------------------------------------------
	CXTPRibbonCustomizePage(CXTPCustomizeSheet* pSheet);
	CXTPRibbonCustomizePage(
		CXTPCommandBars*
			pCommandBars); // <combine
						   // CXTPRibbonCustomizePage::CXTPRibbonCustomizePage@CXTPCustomizeSheet*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeCommandsPage object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonCustomizePage();

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

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refills the list of Quick Access controls.
	//-----------------------------------------------------------------------
	void RefreshRibbonList();

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPRibbonCustomizePage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPRibbonCustomizePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCategoriesSelectionChanged();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonReset();
	afx_msg void OnCommandsSelectionChanged();
	afx_msg void OnRibbonSelectionChanged();
	afx_msg void OnRibbonTreeCheckChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/);
	afx_msg void OnRibbonSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCommandsSelChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnButtonNewTab();
	afx_msg void OnButtonNewGroup();
	afx_msg void OnButtonRename();
	afx_msg void OnImportDropDown();

	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();

	//}}AFX_MSG

private:
	XTP_COMMANDBARS_CATEGORYINFO* FindCategory(LPCTSTR strCategory) const;
	XTP_COMMANDBARS_CATEGORYINFO* GetCategoryInfo(int nIndex);

	CXTPCoreTreeItem* FindItem(CCmdTarget* pItemData, CXTPCoreTreeItems* pItems = NULL);

public:
	CXTPComboBox m_cmbCategories;				// Categories list.
	CXTPComboBox m_cmbTabGroup;					// Categories list.
	CXTPRibbonCustomizeTreeCtrl m_treeRibbon;   // Ribbon list.
	CXTPRibbonCustomizeTreeCtrl m_treeCommands; // Ribbon list.

protected:
	CXTPCustomizeSheet* m_pSheet;				  // Parent Sheet window.
	CXTPCommandBars* m_pCommandBars;			  // Parent CommandBars object.
	CXTPCommandBarsCategoryArray m_arrCategories; // Array of categories.
	friend class CXTPRibbonCustomizeTreeCtrl;

	CXTPButton m_btnAdd;
	CXTPButton m_btnRemove;

	CXTPButton m_btnNewTab;
	CXTPButton m_btnNewGroup;
	CXTPButton m_btnRename;
	CXTPButton m_btnReset;

	CXTPButton m_btnImport;

	CXTPButton m_btnUp;
	CXTPButton m_btnDown;

#	ifdef _XTP_INCLUDE_CONTROLS
	COLORREF m_clrText;
#	endif

#	ifdef _XTP_INCLUDE_MARKUP
	CXTPMarkupStatic& m_wndTitleIcon;
	CXTPMarkupStatic& m_wndTitle;
#	else
	CStatic& m_wndTitleIcon;
	CStatic& m_wndTitle;
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONCUSTOMIZEPAGE_H__)
