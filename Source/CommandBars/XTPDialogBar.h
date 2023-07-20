// XTPDialogBar.h : interface for the CXTPDialogBar class.
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
#if !defined(__XTPDIALOGBAR_H__)
#	define __XTPDIALOGBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPopupBar;

//===========================================================================
// Summary:
//     CXTPDialogBar is a CXTPToolBar derived class.
//     It represents a dockable container for a child window.
//===========================================================================
class _XTP_EXT_CLASS CXTPDialogBar : public CXTPToolBar
{
	//{{AFX_CODEJOCK_PRIVATE
	class CControlButtonHide;
	class CControlCaptionPopup;
	class CCaptionPopupBar;
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogBar object.
	//-------------------------------------------------------------------------
	CXTPDialogBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDialogBar object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPDialogBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify the child window displayed under the
	//     dialog bar.
	// Parameters:
	//     pChild - Pointer to a CWnd object that is displayed under the
	//              dialog bar.
	//-----------------------------------------------------------------------
	void SetChild(CWnd* pChild);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the text that will be displayed in the
	//     dialog bar's caption pop-up.
	// Parameters:
	//     lpszCaption - Text that is displayed in the dialog bar's caption pop-up.
	// Remarks:
	//     This is the text displayed in the dialog bar's caption pop-up. The
	//     caption pop-up can be used to indicate what is being displayed in the
	//     child window.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the text displayed in the caption pop-up.
	// Returns:
	//     The text displayed in the caption pop-up.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide the caption pop-up control (and close button).
	// Parameters:
	//     bVisible - TRUE to hide the caption pop-up control, FALSE to show it.
	// Remarks:
	//     If bVisible is TRUE, only the commandbar is displayed
	//     (the caption pop-up and close button are hidden).
	//-----------------------------------------------------------------------
	void SetCaptionVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to tell the caption pop-up which CMenu pop-up to display
	//     when the the caption pop-up is dropped down.
	// Parameters:
	//     nMenuResourceID - ID of the CMenu resource that contains the pop-up
	//                       menu that should be displayed when the caption pop-up
	//                       is clicked/displayed.
	// Remarks:
	//     If there is more than one pop-up on the CMenu resource, then only the
	//     first pop-up on the CMenu will be used.
	//-----------------------------------------------------------------------
	void SetCaptionPopup(int nMenuResourceID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the CXTPPopupBar that is
	//     displayed when the caption pop-up control is clicked/dropped down.
	// Returns:
	//     Pointer to the CXTPPopupBar that is displayed when the caption
	//     pop-up control is clicked/dropped down.
	//-----------------------------------------------------------------------
	CXTPPopupBar* GetCaptionPopup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the dialog bar will be re-sizable.
	// Parameters:
	//     bResizable - TRUE if the dialog bar can be resized, FALSE to lock
	//                  the dialog bar.
	// Remarks:
	//     By default, bResizable is TRUE. If FALSE, no splitter is displayed
	//     when the mouse pointer is positioned over the dialog bar borders.
	//     You can use the SetSize member function to change the size of the
	//     dialog bar programmatically.
	//-----------------------------------------------------------------------
	void SetResizable(BOOL bResizable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width and height of the dialog bar child window.
	// Parameters:
	//     sz - The width and height of the dialog bar's child window.
	// Remarks:
	//     The size is only used to size the child window when the dialog
	//     bar is floating, then both the specified width and height are used.
	//     If the bar is docked on the left or right, only the width is used,
	//     the height will depend on how much of the client area the dialog bar
	//     can occupy.  If the bar is docked on the top or bottom, then only
	//     the height is used to size the child window.
	//-----------------------------------------------------------------------
	void SetSize(CSize sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the child pane state.
	// Returns:
	//     TRUE if a child pane is active, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - Length of the bar.
	//     dwMode - Mode to dock.
	//     nWidth - Width of the bar.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	CSize CalcDockingLayout(int nLength, DWORD dwMode, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - The requested dimension of the control bar, either
	//               horizontal or vertical, depending on dwMode.
	//      dwMode - See CControlBar::CalcDynamicLayout for a list of supported flags.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the CommandBar is a CXTPDialogBar.
	// Remarks:
	//     This will always return TRUE.
	// Returns:
	//     Returns TRUE if the CommandBar is a CXTPDialogBar, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDialogBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user resizes the dialog bar.
	// Parameters:
	//     rcWindow - CRect object specifying the size of the window.
	//     nHitTest - Specifies the hit-test area code.
	//-----------------------------------------------------------------------
	void OnResize(CRect rcWindow, int nHitTest);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a dialog bar.
	// Parameters:
	//     nLength - Length of the bar.
	//     dwMode - Mode to dock.
	//     szClient - Client size of dialog bar.
	// Returns:
	//     Size of the dialog bar.
	//-----------------------------------------------------------------------
	CSize CalcSize(int nLength, DWORD dwMode, CSize szClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine mouse hit code.
	// Parameters:
	//     pt - Position to test.
	// Returns:
	//     HTCLIENT if pointer belongs client area, otherwise HTNOWHERE.
	//-----------------------------------------------------------------------
	int OnMouseHitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the client margins of the dialog bar.
	// Returns:
	//     Rectangle containing the top, left, right, and bottom margins of the dialog bar.
	//-----------------------------------------------------------------------
	CRect GetMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to reposition the child window in the dialog bar.
	// Parameters:
	//     rc - CRect object specifying size of the area.
	//-----------------------------------------------------------------------
	void MoveChildWindow(CRect rc);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDialogBar)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CRect m_rcBorders;		   // Borders size.
	BOOL m_bResizable;		   // TRUE if the child window and dialog bar are re-sizable.
	CSize m_szMinTrackRect;	// Minimum tracking rectangle.
	BOOL m_bCaptionVisible;	// TRUE if the caption pop-up control is visible.
	BOOL m_bCaptionPopup;	  // TRUE to use the caption as a pop-up control.
	int m_nCaptionHeight;	  // Caption height.
	CSize m_szDockingClient;   // Docking client rectangle.
	CSize m_szFloatingClient;  // Floating client rectangle.
	HWND m_hwndChild;		   // Child window handle.
	CString m_strCaption;	  // Text that is displayed in the caption pop-up control.
	CXTPPopupBar* m_pPopupBar; // Pointer to caption pop-up bar.
	BOOL m_bActive;			   // TRUE if the dialog bar has an active caption.

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPDialogBar);
	DECLARE_INTERFACE_MAP()

	afx_msg LPDISPATCH OleGetCaptionPopupBar();
	afx_msg void OleSetChildHandle(HWND hWnd);
	afx_msg HWND OleGetChildHandle();
	afx_msg void OleSetSize(int cx, int cy);

//}}AFX_CODEJOCK_PRIVATE
#	endif

	DECLARE_XTP_COMMANDBAR(CXTPDialogBar)

	friend class CXTPToolBar;
	friend class CXTPDockContext;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPDialogBar::IsDialogBar() const
{
	return TRUE;
}
AFX_INLINE CXTPPopupBar* CXTPDialogBar::GetCaptionPopup() const
{
	return m_pPopupBar;
}
AFX_INLINE void CXTPDialogBar::SetResizable(BOOL bResizable)
{
	m_bResizable = bResizable;
}
AFX_INLINE BOOL CXTPDialogBar::IsActive() const
{
	return m_bActive;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPDIALOGBAR_H__)
