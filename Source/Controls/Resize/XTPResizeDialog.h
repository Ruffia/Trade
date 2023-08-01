// XTPResizeDialog.h: interface for the CXTPResizeDialog class.
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
#if !defined(__XTPRESIZEDIALOG_H__)
#	define __XTPRESIZEDIALOG_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPResizeDialog is a multiple inheritance class derived from CDialog
//     and CXTPResize. CXTPResizeDialog is used to create a resizable CDialog
//     type object that allows its dialog items to be resized or moved dynamically.
//===========================================================================
class _XTP_EXT_CLASS CXTPResizeDialog
	: public CDialog
	, public CXTPResize
{
	DECLARE_DYNCREATE(CXTPResizeDialog)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizeDialog object.
	// Parameters:
	//     nID              - Contains the ID number of a dialog box template resource.
	//     lpszTemplateName - Contains a NULL-terminated string that is the name of a
	//                        dialog-box template resource.
	//     pParent          - Pointer to the parent or owner window object of type
	//                        CWnd, which the dialog object belongs to. If it is NULL,
	//                        then the dialog object's parent window is set to the main
	//                        application window.
	//     nFlags           - Flags that are to be passed to CXTPResize that specify the
	//                        attributes of the resizing property page. They can be one
	//                        or more of the values listed in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>xtpResizeNoSizeIcon</b>: Do not add size icon.
	//     * <b>xtpResizeNoHorizontal</b>: No horizontal resizing.
	//     * <b>xtpResizeNoVertical</b>: No vertical resizing.
	//     * <b>xtpResizeNoMinsize</b>: Do not require a minimum size.
	//     * <b>xtpResizeNoClipChildren</b>: Do not set clip children style.
	//     * <b>xtpResizeNoTransparentGroup</b>: Do not set transparent style
	//                                           for group boxes.
	//-----------------------------------------------------------------------
	CXTPResizeDialog();
	CXTPResizeDialog(const UINT nID, CWnd* pParent = 0,
					 const UINT nFlags = 0); // <combine CXTPResizeDialog::CXTPResizeDialog>
	CXTPResizeDialog(LPCTSTR lpszTemplateName, CWnd* pParent = 0,
					 const UINT nFlags = 0); // <combine CXTPResizeDialog::CXTPResizeDialog>

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPResizeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	UINT m_nDialogID; // Contains the ID number of a dialog box template resource.
};

#	if (_MSC_VER > 1200)
//===========================================================================
// Summary:
//     CXTPResizeDHtmlDialog is a multiple inheritance class derived from CDHtmlDialog
//     and CXTPResize. CXTPResizeDHtmlDialog is used to create a resizable CDHtmlDialog
//     type object that allows its dialog items to be resized or moved dynamically.
//===========================================================================
class _XTP_EXT_CLASS CXTPResizeDHtmlDialog
	: public CDHtmlDialog
	, public CXTPResize
{
	DECLARE_DYNCREATE(CXTPResizeDHtmlDialog)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizeDialog object.
	// Parameters:
	//     nID              - Contains the ID number of a dialog box template resource.
	//     lpszTemplateName - Contains a NULL-terminated string that is the name of a
	//                        dialog-box template resource.
	//     nHtmlResID       - Contains the ID number of an HTML resource.
	//     szHtmlResID      - The NULL-terminated string that is the name of an HTML resource.
	//     pParent          - Pointer to the parent or owner window object of type
	//                        CWnd, which the dialog object belongs to. If it is NULL,
	//                        then the dialog object's parent window is set to the main
	//                        application window.
	//     nFlags           - Flags that are to be passed to CXTPResize that specify the
	//                        attributes of the resizing property page. They can be one
	//                        or more of the values listed in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>xtpResizeNoSizeIcon</b>: Do not add size icon.
	//     * <b>xtpResizeNoHorizontal</b>: No horizontal resizing.
	//     * <b>xtpResizeNoVertical</b>: No vertical resizing.
	//     * <b>xtpResizeNoMinsize</b>: Do not require a minimum size.
	//     * <b>xtpResizeNoClipChildren</b>: Do not set clip children style.
	//     * <b>xtpResizeNoTransparentGroup</b>: Do not set transparent style
	//                                           for group boxes.
	//-----------------------------------------------------------------------
	CXTPResizeDHtmlDialog();
	CXTPResizeDHtmlDialog(const UINT nID, UINT nHtmlResID = 0, CWnd* pParent = 0,
						  const UINT nFlags = 0); // <combine
												  // CXTPResizeDHtmlDialog::CXTPResizeDHtmlDialog>
	CXTPResizeDHtmlDialog(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd* pParent = 0,
						  const UINT nFlags = 0); // <combine
												  // CXTPResizeDHtmlDialog::CXTPResizeDHtmlDialog>

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPResizeDHtmlDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	UINT m_nDialogID; // Contains the ID number of a dialog box template resource
};
#	endif

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRESIZEDIALOG_H__)