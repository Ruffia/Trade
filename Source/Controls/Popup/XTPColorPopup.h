// XTPColorPopup.h : interface for the CXTPColorPopup class.
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
#if !defined(__XTPCOLORPOPUP_H__)
#	define __XTPCOLORPOPUP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPShadowManager;

//===========================================================================
// Summary:
//     CXTPColorPopup is a CXTPColorSelectorCtrl derived class. It is used
//     to create a CXTPColorPopup window that will allow a user to select
//     colors and add custom colors to a color list.
//===========================================================================
class _XTP_EXT_CLASS CXTPColorPopup : public CXTPColorSelectorCtrl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPColorPopup object.
	// Parameters:
	//     bAutoDelete - TRUE if the color picker window is to be self deleting.
	//     callerParam - Caller parameter; it will be reported back as an LPARAM
	//                   of color pop-up notification messages.
	//-----------------------------------------------------------------------
	CXTPColorPopup(BOOL bAutoDelete = FALSE, LPARAM callerParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPColorPopup object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPColorPopup();

public:
	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(AddRecentColor)
	static void AFX_CDECL AddUserColor(COLORREF clrColor)
	{
		AddRecentColor(clrColor);
	}

	_XTP_DEPRECATED_IN_FAVOR(ResetRecentColors)
	static void AFX_CDECL ResetUserColors()
	{
		ResetRecentColors();
	}
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function handles the creation of the color pop-up window.
	// Parameters:
	//     rect -        A reference to a <i>CRect</i> object that represents the
	//                   size of the color pop-up window.
	//     pParentWnd -  Pointer to the parent window for the color pop-up.
	//     dwPopup -     Style for the pop-up window. See the Remarks section for
	//                   a complete list of available styles.
	//     clrColor -    An <i>RGB</i> value that represents the currently selected
	//                   color for the pop-up window.
	//     clrDefault -  Specifies the default color for the color pop-up.
	//                   If the current style includes CPS_NOFILL, then this
	//                   parameter is ignored.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPS_XTP_NOFILL</b>: The color picker will display a No Fill button
	//                              rather than the default Automatic Color button.
	//     * <b>CPS_XTP_EXTENDED</b>: The color picker will display 40 extended
	//                                colors rather than the default 16 colors.
	//     * <b>CPS_XTP_MORECOLORS</b>: The color picker will display a More Colors
	//                                  button which will display a CXTPColorDialog.
	//     * <b>CPS_XTP_SHOW3DSELECTION</b>: Displays the color selection box with
	//                                       a 3D raised border in CXTPColorDialog.
	//     * <b>CPS_XTP_SHOWHEXVALUE</b>: Displays the hex equivalent of
	//                                    the selected color.
	//     * <b>CPS_XTP_SHOWEYEDROPPER</b>: Displays the eye dropper box to allow
	//                                      for selecting any color on the screen.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     CXTPColorPicker::ModifyCPStyle, CXTPColorSelectorCtrl::Create
	//-----------------------------------------------------------------------
	virtual BOOL Create(CRect& rect, CWnd* pParentWnd, DWORD dwPopup, COLORREF clrColor,
						COLORREF clrDefault = CLR_DEFAULT);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds a window that picker notifications are sent to.
	// Parameters:
	//     hwndListener -  A handle to the listener window.
	//                     Messages will be sent to it.
	//-----------------------------------------------------------------------
	void AddListener(HWND hwndListener);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables pop-up window shadow.
	// Parameters:
	//     bDisplayShadow -  Flag that tells if the shadow is to be displayed.
	//                       TRUE to enable the shadow, FALSE to disable.
	// Remarks:
	//     This member function enables/disables the drawing of pop-up shadows.
	//     Call this function after instantiating the color pop-up as a part
	//     of an object's initialization. Do not call it after pop-up creation
	//     because doing so may cause unpredictable results.
	//-----------------------------------------------------------------------
	void DisplayShadow(BOOL bDisplayShadow = TRUE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will finish the selection process for the color
	//     box or button in the color pop-up window.
	// Parameters:
	//     nCurSel - Current index of the selected color box or button
	//               in the color pop-up window.
	//-----------------------------------------------------------------------
	virtual void EndSelection(int nCurSel);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPColorPopup)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPColorPopup)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	BOOL m_bAutoDelete;	// TRUE if the pop-up window is to be self deleting.
	BOOL m_bDisplayShadow; // TRUE if the pop-up window will render its shadow.
	CRect m_rcExclude;	 // Area to exclude from shadow display.
	BOOL m_bEndSelection;
	const LPARAM m_callerParam; // Caller parameter that will be reported back as an LPARAM of all
								// notification messages.
	CXTPShadowManager* m_pShadowManager; // Shadow manager to draw pop-up shadow
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPColorPopup::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
									   const RECT& rect, CWnd* pParentWnd, UINT nID,
									   CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTPColorPopup::DisplayShadow(BOOL bDisplayShadow /* = TRUE*/)
{
	m_bDisplayShadow = bDisplayShadow;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCOLORPOPUP_H__)
