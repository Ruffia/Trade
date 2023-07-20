// XTPMiniToolBar.h : interface for the CXTPMiniToolBar class.
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
#if !defined(__XTPMINITOOLBAR_H__)
#	define __XTPMINITOOLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPMiniToolBar window

//===========================================================================
// Summary:
//     Represents a MiniToolbar object.
// Remarks:
//     MiniToolBar is an Office 2007 style context toolbar which has many
//     uses. For example, you might want to display a MiniToolbar presenting some
//     of the more commonly used commands that a user would probably want to use
//     in a given situation like selecting text. For example, this can save the
//     user a lot of time by making font related tasks available without fumbling
//     through tabs or menus. So, to effectively use a MiniToolbar, you would
//     place all commands that the user would most probably want to use when
//     performing a given action.
//===========================================================================
class _XTP_EXT_CLASS CXTPMiniToolBar : public CXTPPopupToolBar
{
	DECLARE_XTP_COMMANDBAR(CXTPMiniToolBar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Call this method to create a CXTPMiniToolBar class.
	// Parameters:
	//      pCommandBars - Pointer to owner CXTPCommandBars.
	// Returns:
	//      Pointer to the newly created CXTPMiniToolBar object.
	//-----------------------------------------------------------------------
	static CXTPMiniToolBar* AFX_CDECL CreateMiniToolBar(CXTPCommandBars* pCommandBars);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMiniToolBar object.
	//-------------------------------------------------------------------------
	CXTPMiniToolBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMiniToolBar object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPMiniToolBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show the minibar in a specific position.
	// Parameters:
	//     nFlags - Reserved, currently not used.
	//     xPos - Specifies the horizontal position (in screen coordinates)
	//            of the minibar.
	//     yPos - Specifies the vertical position (in screen coordinates)
	//            of the top of the minibar.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL TrackMiniBar(UINT nFlags, int xPos, int yPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to display a floating pop-up menu at the specified location.
	// Parameters:
	//     pPopupMenu - Pointer to a CXTPPopupBar object.
	//     nFlags - Reserved, currently not used.
	//     xPos - Specifies the horizontal position (in screen coordinates)
	//            of the pop-up menu.
	//     yPos - Specifies the vertical position (in screen coordinates)
	//            of the top of the menu on the screen.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL TrackPopupMenu(CXTPPopupBar* pPopupMenu, UINT nFlags, int xPos, int yPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     TRUE if the key was handled, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable keyboard interaction with custom controls.
	// Parameters:
	//     bInteract - TRUE to enable keyboard interaction with custom controls.
	// Remarks:
	//     xtpControlCustom is disabled by default.
	//     The MiniToolbar closes when keyboard events occur within a custom control.
	//-----------------------------------------------------------------------
	void SetCustomControlKeyboardInteraction(BOOL bInteract = TRUE);

	BOOL IsCustomControlKeyboardInteraction() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Translates all messages in the message queue.
	//-------------------------------------------------------------------------
	virtual void PumpMessage();

	//-------------------------------------------------------------------------
	// Summary:
	//     Updates the opacity of the mini toolbar and pop-up menu.
	//-------------------------------------------------------------------------
	virtual void UpdateOpacity();

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the mouse cursor is hovered over the mini toolbar.
	// Returns:
	//     TRUE if the mouse cursor is hovered over the mini toolbar, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL CursorInWindow() const;

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPMiniToolBar)

	virtual void Animate();
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam,
							  LRESULT& lResult);

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPMiniToolBar)
	//}}AFX_MSG

	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMiniToolBar);
	DECLARE_INTERFACE_MAP()

	long OlePopupMiniToolBar(const VARIANT& varFlags, const VARIANT& x, const VARIANT& y);
	long OlePopupContextMenu(LPDISPATCH lpDispatch, const VARIANT& varFlags, const VARIANT& x,
							 const VARIANT& y);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//      UpdateLayeredWindow declaration.
	// Parameters:
	//      LPFNUPDATELAYEREDWINDOW - Struct that contains the position, size, shape,
	//                                content, and translucency of a layered window.
	// Remarks:
	//      Required for transparent Windows, but they are not present in the VC6 headers.
	//      They are only present on Win2k or later.
	// Returns:
	//      TRUE if successful, otherwise FALSE.
	//-------------------------------------------------------------------------
	typedef BOOL(WINAPI* LPFNUPDATELAYEREDWINDOW)(HWND hwnd, HDC hdcDst, POINT* pptDst, SIZE* psize,
												  HDC hdcSrc, POINT* pptSrc, COLORREF crKey,
												  BLENDFUNCTION* pblend, DWORD dwFlags);

	//-------------------------------------------------------------------------
	// Summary:
	//      SetLayeredWindowAttributes declaration.
	// Parameters:
	//      PFNSETLAYEREDWINDOWATTRIBUTES - Struct that contains the opacity and
	//                                      transparency color key of a layered window.
	// Remarks:
	//      Required for transparent Windows, but they are not present in the VC6 headers.
	//      They are only present on Win2k or later.
	// Returns:
	//      TRUE if successful, otherwise FALSE.
	//-------------------------------------------------------------------------
	typedef BOOL(WINAPI* PFNSETLAYEREDWINDOWATTRIBUTES)(HWND hwnd, COLORREF crKey, BYTE bAlpha,
														DWORD dwFlags);

	PFNSETLAYEREDWINDOWATTRIBUTES m_pfnSetLayeredWindowAttributes; // Point to Transparency proc in
																   // USER32.dll module.
	LPFNUPDATELAYEREDWINDOW m_pfnUpdateLayeredWindow; // Point to UpdateLayeredWindow proc in
													  // USER32.dll module.

	BOOL m_bActivated;			  // TRUE if the mini toolbar is activated.
	int m_nOpacity;				  // Opacity level.
	BOOL m_bTracking;			  // TRUE if the command bar is in tracking mode.
	CXTPPopupBar* m_pContextMenu; // Context menu that is displayed in customization mode when a
								  // user right-clicks on a control.
	BOOL m_bCustomControlKeyboardInteraction;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPMINITOOLBAR_H__)
