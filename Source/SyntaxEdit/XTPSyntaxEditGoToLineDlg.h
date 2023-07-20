// XTPSyntaxEditGoToLineDlg.h : header file
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
#if !defined(__XTPSYNTAXEDITGOTOLINEDLG_H__)
#	define __XTPSYNTAXEDITGOTOLINEDLG_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//      This class implements the "GoToLine" dialog for a SyntaxEdit control.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditGoToLineDlg : public CDialog
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	// Parameters:
	//      pParent - Pointer to a parent window.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditGoToLineDlg(CWnd* pParent = NULL);

	//{{AFX_CODEJOCK_PRIVATE
	enum
	{
		IDD = XTP_IDD_EDIT_GOTOLINE
	};
	CEdit m_wndEditLineNo;
	int m_iLineNo;
	CString m_csLineNo;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows the "GoToLine" dialog. If a window has not been
	//     created, then this method will load a dialog template
	//     from resources and create a modeless dialog.
	// Parameters:
	//     pEditCtrl   - Pointer to a CXTPSyntaxEditCtrl object.
	//     bSelectLine - Select Line option.
	//     bHideOnFind - Hide On Find option.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     ShowWindow
	//-----------------------------------------------------------------------
	BOOL ShowDialog(CXTPSyntaxEditCtrl* pEditCtrl, BOOL bSelectLine = FALSE,
					BOOL bHideOnFind = FALSE);

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
													 //}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Loads window position from the application profile.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL LoadPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves window position to the application profile.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SavePos();

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnChangeEditLineNo();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGoTo();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

	int m_iMaxLineNo;				 // Maximum line number.
	BOOL m_bHideOnFind;				 // Hide On Find option.
	BOOL m_bSelectLine;				 // Select Line option.
	CPoint m_ptWndPos;				 // Window position.
	CXTPSyntaxEditCtrl* m_pEditCtrl; // Pointer to the attached edit control.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITGOTOLINEDLG_H__)
