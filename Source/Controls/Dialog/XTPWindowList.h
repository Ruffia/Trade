// XTPWindowList.h : header file
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
#if !defined(_XTPWINDOWLIST_H__)
#	define _XTPWINDOWLIST_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPWindowList dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPWindowList : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CXTPWindowList)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPWindowList object.
	// Parameters:
	//     pMDIFrameWnd - Pointer to the application's main window.
	//-----------------------------------------------------------------------
	CXTPWindowList(CMDIFrameWnd* pMDIFrameWnd);
	CXTPWindowList(HWND hWndMDIClient); // <combine CXTPWindowList::CXTPWindowList@CMDIFrameWnd*>

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the dialog's icon.
	// Parameters:
	//     hIcon - Icon handle to set.
	//-----------------------------------------------------------------------
	void SetWindowIcon(HICON hIcon)
	{
		m_hIcon = hIcon;
	}

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a MDI child to the window list.
	// Parameters:
	//     hwndMDIChildWnd - Pointer to a MDI child window.
	//-----------------------------------------------------------------------
	virtual void InsertMDIChild(HWND hwndMDIChildWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a MDI child window's text.
	// Parameters:
	//     hWnd - Handle to a MDI child window.
	// Returns:
	//     A CString object representing the window's text.
	//-----------------------------------------------------------------------
	virtual CString GetChildWndText(HWND hWnd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to toggle the restore/minimize state for
	//     selected MDI children.
	//-----------------------------------------------------------------------
	void RestoreAndMinimize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable/disable button controls for
	//     the window list dialog depending on selection.
	//-----------------------------------------------------------------------
	virtual void SetEnableButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to populate the window list with
	//     MDI children.
	// Parameters:
	//     hWnd - Handle to a MDI client window.
	//-----------------------------------------------------------------------
	void FillMDIClientWindowList(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to populate the window list with
	//     currently open MDI children.
	//-----------------------------------------------------------------------
	virtual void FillMDIWindowList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to select the active MDI child window.
	//-----------------------------------------------------------------------
	virtual void SelectMDIActive();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPWindowList)
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPWindowList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLBox();
	afx_msg void OnBtnActivate();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnClosewindows();
	afx_msg void OnBtnTilehorz();
	afx_msg void OnBtnTilevert();
	afx_msg void OnBtnCascade();
	afx_msg void OnBtnMinimize();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPWindowList)
	CButton m_btnMinimize;
	CButton m_btnCascade;
	CButton m_btnOk;
	CButton m_btnTileVert;
	CButton m_btnTileHorz;
	CButton m_btnSave;
	CButton m_btnClose;
	CButton m_btnActivate;
	//}}AFX_DATA
	//}}AFX_CODEJOCK_PRIVATE

	HWND m_hWndMDIClient;		  // Handle to a MDI client area.
	HICON m_hIcon;				  // Resize dialog's handle.
	CMDIFrameWnd* m_pMDIFrameWnd; // Pointer to application's MDI frame.
	CXTPListBox m_lboxWindows;	// Flicker free list box to display MDI children.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPWINDOWLIST_H__)
