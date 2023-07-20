// XTPSyntaxEditFindReplaceDlg.h : header file
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
#if !defined(__XTPSYNTAXEDITFINDREPLACEDLG_H__)
#	define __XTPSYNTAXEDITFINDREPLACEDLG_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditView;

//===========================================================================
// Summary:
//     This class implements the "Find and Replace" and "Find" dialogs
//     for a SyntaxEdit control.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditFindReplaceDlg : public CDialog
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditFindReplaceDlg)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	// Parameters:
	//      pParentWnd - Pointer to a parent window.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditFindReplaceDlg(CWnd* pParentWnd = NULL);

	int m_nSearchDirection;		  // Search Direction: 0-up, 1-down.
	BOOL m_bMatchWholeWord;		  // Match Whole Words option.
	BOOL m_bMatchCase;			  // Match Case option.
	CString m_csFindText;		  // Text to find.
	CString m_csReplaceText;	  // Text to replace.
	int m_nSearchHistoryCountMax; // Maximum number of search strings.

	//{{AFX_CODEJOCK_PRIVATE
	enum
	{
		IDD = XTP_IDD_EDIT_SEARCH_REPLACE
	};

	CButton m_btnFindNext;
	CButton m_btnRadioUp;
	CButton m_btnRadioDown;
	CButton m_btnReplace;
	CButton m_btnReplaceAll;
	CComboBox m_wndFindCombo;
	CComboBox m_wndReplaceCombo;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows either a "Find and Replace" or "Find" dialog. If a window
	//     has not been created, then this method will load a dialog template
	//     from resources and create a modeless dialog.
	// Parameters:
	//     pEditCtrl   - Pointer to a CXTPSyntaxEditControl object.
	//     bReplaceDlg - TRUE to create a "Find and Replace" dialog,
	//                   FALSE to create a "Find" dialog.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     ShowWindow
	//-----------------------------------------------------------------------
	BOOL ShowDialog(CXTPSyntaxEditCtrl* pEditCtrl, BOOL bReplaceDlg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the attached edit control.
	// Returns:
	//     A pointer to the attached edit control.
	// See Also:
	//     ShowDialog
	//-----------------------------------------------------------------------
	CXTPSyntaxEditCtrl* GetEdirControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the type of dialog that was created.
	// Returns:
	//     TRUE if a "Find and Replace" dialog was created,
	//     FALSE if a "Find" dialog was created.
	// See Also:
	//     ShowDialog
	//-----------------------------------------------------------------------
	BOOL IsReplaceDialog() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum allowed number of search strings for
	//     find and replace comboboxes.
	// Returns:
	//     The maximum allowed number of search strings for
	//     find and replace comboboxes.
	// See Also:
	//     SetSearchHistoryCountMax, LoadHistory, SaveHistory
	//-----------------------------------------------------------------------
	int GetSearchHistoryCountMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the maximum allowed number of search strings for
	//     find and replace comboboxes.
	// Parameters:
	//     nMaxCount - Maximum allowed number of search strings to be set for
	//                 find and replace comboboxes.
	// See Also:
	//     GetSearchHistoryCountMax, LoadHistory, SaveHistory
	//-----------------------------------------------------------------------
	void SetSearchHistoryCountMax(int nMaxCount);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Moves or adds a string to the top of history items (i.e. item 0).
	// Parameters:
	//      csText    - Reference to the string to move or add.
	//      arHistory - Reference to the history array.
	//      wndCombo  - Reference to the history combobox.
	//-----------------------------------------------------------------------
	void UpdateHistoryCombo(const CString& csText, CStringArray& arHistory);
	void UpdateHistoryCombo(
		const CString& csText, CStringArray& arHistory,
		CComboBox& wndCombo); //<COMBINE
							  // CXTPSyntaxEditFindReplaceDlg::UpdateHistoryCombo@const
							  // CString&@CStringArray&>
	void UpdateHistoryCombo(
		const CString& csText,
		CComboBox& wndCombo); //<COMBINE
							  // CXTPSyntaxEditFindReplaceDlg::UpdateHistoryCombo@const
							  // CString&@CStringArray&>

	//-----------------------------------------------------------------------
	// Summary:
	//     Initializes a combobox from an array of strings.
	// Parameters:
	//     arHistory - Reference to an array of strings.
	//     wndCombo  - Reference to a CComboBox object.
	// See Also:
	//     LoadHistory
	//-----------------------------------------------------------------------
	void InitHistoryCombo(CStringArray& arHistory, CComboBox& wndCombo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads from application profile history for find and replace
	//     comboboxes and state for other dialog controls.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     SaveHistory
	//-----------------------------------------------------------------------
	BOOL LoadHistory();

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves to application profile history for find and replace
	//     comboboxes and state for other dialog controls.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     LoadHistory
	//-----------------------------------------------------------------------
	BOOL SaveHistory();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the control state (i.e. enable/disable) for the
	//     "Find", "Replace", and "Replace All" buttons.
	//-----------------------------------------------------------------------
	void EnableControls();

	//{{AFX_CODEJOCK_PRIVATE
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnFinalRelease();

	afx_msg void OnEditChangeComboFind();
	afx_msg void OnSelendOkComboFind();
	afx_msg void OnChkMatchWholeWord();
	afx_msg void OnChkMatchCase();
	afx_msg void OnRadioUp();
	afx_msg void OnRadioDown();
	afx_msg void OnBtnFindNext();
	afx_msg void OnBtnReplace();
	afx_msg void OnBtnReplaceAll();
	afx_msg void OnEditChangeComboReplace();
	afx_msg void OnSelendOkComboReplace();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bReplaceDlg;				 // Dialog type.
	CPoint m_ptWndPos;				 // Window position.
	CXTPSyntaxEditCtrl* m_pEditCtrl; // Pointer to the attached view.
};

//////////////////////////////////////////////////////////////////////////
AFX_INLINE CXTPSyntaxEditCtrl* CXTPSyntaxEditFindReplaceDlg::GetEdirControl() const
{
	return m_pEditCtrl;
}
AFX_INLINE BOOL CXTPSyntaxEditFindReplaceDlg::IsReplaceDialog() const
{
	return m_bReplaceDlg;
}
AFX_INLINE int CXTPSyntaxEditFindReplaceDlg::GetSearchHistoryCountMax() const
{
	return m_nSearchHistoryCountMax;
}
AFX_INLINE void CXTPSyntaxEditFindReplaceDlg::SetSearchHistoryCountMax(int nMaxCount)
{
	m_nSearchHistoryCountMax = nMaxCount;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITFINDREPLACEDLG_H__)
