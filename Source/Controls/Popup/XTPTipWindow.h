// XTPTipWindow.h interface for the CXTPTipWindow class.
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
#if !defined(__XTPTIPWINDOW_H__)
#	define __XTPTIPWINDOW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

const DWORD TWS_XTP_THICKBORDER = 0x0001; //<ALIAS CXTPTipWindow::ShowTipWindow@const
										  // CPoint&@DWORD@UINT@UINT@BOOL>
const DWORD TWS_XTP_DROPSHADOW = 0x0002;  //<ALIAS CXTPTipWindow::ShowTipWindow@const
										  // CPoint&@DWORD@UINT@UINT@BOOL>
const DWORD TWS_XTP_ALPHASHADOW = 0x0004; //<ALIAS CXTPTipWindow::ShowTipWindow@const
										  // CPoint&@DWORD@UINT@UINT@BOOL>

//===========================================================================
// Summary:
//     CXTPTipWindow is a CWnd derived class. It is used to create a CXTPTipWindow
//     object. CXTPTipWindow objects are tooltip style pop-up windows that can
//     display useful information to the end user. Possible options include
//     shadows, borders, and background color settings.
//===========================================================================
class _XTP_EXT_CLASS CXTPTipWindow : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTipWindow object.
	//-----------------------------------------------------------------------
	CXTPTipWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTipWindow object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTipWindow();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create the tooltip window.
	// Parameters:
	//     pParentWnd - Pointer to the parent of the tooltip window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to offset the tip window by the distance
	//     specified by pointOffset.
	// Parameters:
	//     pointOffset - A CPoint object that represents the XY offset.
	//-----------------------------------------------------------------------
	void SetTipOffsetPos(CPoint pointOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to hide the tip window.
	//-----------------------------------------------------------------------
	void HideTipWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text tab stop positions for the
	//     tooltip window.
	// Parameters:
	//     pnTabStopPositions - Pointer to an integer array containing the tab
	//                          stop positions (could be a static array).
	//     nTabPositions      - Number of array elements.
	//-----------------------------------------------------------------------
	void SetTextTabStopPositions(LPINT pnTabStopPositions, int nTabPositions);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text for the title and description
	//     values that are displayed in the tip window.
	// Parameters:
	//     lpszTitle   - A NULL-terminated string that is to be displayed as the
	//                   title for the tip window.
	//     lpszDescrip - A NULL-terminated string that is to be displayed as the
	//                   description text for the tip window.
	//     bRedraw     - If TRUE, then the tip window will be redrawn.
	//-----------------------------------------------------------------------
	void SetTipText(LPCTSTR lpszTitle, LPCTSTR lpszDescrip, BOOL bRedraw = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text and background colors for
	//     the tip window display.
	// Parameters:
	//     crBackColor - An RGB color value to be used for the background of the tip window.
	//     crTextColor - An RGB color value to be used for the text of the tip window.
	//     bRedraw     - If TRUE, then the tip window will be redrawn.
	//-----------------------------------------------------------------------
	void SetTipColors(COLORREF crBackColor, COLORREF crTextColor, BOOL bRedraw = FALSE);

	// ------------------------------------------------------------------------------------
	// Summary:
	//     This member function will display a tip window to display a title
	//     and description text.
	// Parameters:
	//     point -               The initial x- and y- coordinates of where the tip window
	//                           is to be displayed.
	//     rectCtrl -            Reference to a CRect object that represents the size of
	//                           the control window where the tip is displayed.
	//     pointMousePos -       Reference to a CPoint object that represents the XY position
	//                           of the mouse cursor.
	//     dwTipStyle -          Initial style for the tip window. See the Remarks section
	//                           for the styles that can be used with the tip window.
	//     nDelay -              Specifies the delay value in milliseconds of when the tip
	//                           window should show.
	//     nElapse -             Specifies the time-out value in milliseconds of when the
	//                           tip window should close.
	//     bCenterHorz -         Set this parameter to 'true' to center the window
	//                           horizontally on the point passed in.
	//     bShowTipAtCursorPos - TRUE to show the tip at the cursor position.
	// Returns:
	//     TRUE if the tip window was successfully created, otherwise FALSE.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>TWS_XTP_THICKBORDER</b>: This style will cause the tip window to
	//                                   display a thick border around its edges.
	//     * <b>TWS_XTP_DROPSHADOW</b>: This style will cause the tip window to
	//                                  cast a shadow on the parent window.
	//     * <b>TWS_XTP_ALPHASHADOW</b>: Used with TWS_XTP_DROPSHADOW to display
	//                                   an alpha style shadow.
	//                                   By default, mono-shadowing is enabled.
	// ------------------------------------------------------------------------------------
	virtual BOOL ShowTipWindow(const CPoint& point, DWORD dwTipStyle = TWS_XTP_DROPSHADOW,
							   UINT nDelay = 0, UINT nElapse = 5000, BOOL bCenterHorz = FALSE);
	virtual BOOL ShowTipWindow(
		const CRect& rectCtrl, const CPoint& pointMousePos, DWORD dwTipStyle = TWS_XTP_DROPSHADOW,
		UINT nDelay = 0, UINT nElapse = 5000, BOOL bCenterHorz = FALSE,
		BOOL bShowTipAtCursorPos = FALSE); //<combine
										   // CXTPTipWindow::ShowTipWindow@const
										   // CPoint&@DWORD@UINT@UINT@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text margins for the tip window.
	// Parameters:
	//     size - Initial size for the horizontal and vertical text margins.
	//-----------------------------------------------------------------------
	void SetMargins(CSize size);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text line spacing for the tip window.
	// Parameters:
	//     nLineSpace - Initial size for the text line spacing.
	//-----------------------------------------------------------------------
	void SetLineSpace(int nLineSpace);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will draw a shadow rect to the device context
	//     specified by 'pDC'.
	// Parameters:
	//     pDC  - Device context of the window to cast the shadow on.
	//     rect - Reference to a CRect object that represents the initial
	//            size of the shadow.
	//-----------------------------------------------------------------------
	void DrawShadowRect(CDC* pDC, const CRect& rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the sizes for the tip window.
	// Parameters:
	//     point       - The x- and y- coordinates of where the tip window
	//                   is to be displayed.
	//     bCenterHorz - TRUE to horizontally center the window position.
	//-----------------------------------------------------------------------
	virtual void InitializeSize(const CPoint& point, BOOL bCenterHorz = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Forwards a message to the window at a specified cursor position.
	// Parameters:
	//     uiMsg  - Specifies the message to be sent.
	//     nFlags - Indicates whether various virtual keys are down. See Remarks
	//              section for a list of values.
	//     point  - The x- and y- coordinates of the window to send the message to.
	// Remarks:
	//     The <i>nFlags</i> parameter can be any combination of the following
	//     values:<p/>
	//     * <b>MK_CONTROL</b>: Set if the CTRL key is down.
	//     * <b>MK_LBUTTON</b>: Set if the left mouse button is down.
	//     * <b>MK_MBUTTON</b>: Set if the middle mouse button is down.
	//     * <b>MK_RBUTTON</b>: Set if the right mouse button is down.
	//     * <b>MK_SHIFT</b>:   Set if the SHIFT key is down.
	//     This member function is called by the CXTPTipWindow object to hide
	//     the tip window and forward the message specified by <i>uiMsg</i>
	//     to the window at the coordinates specified by <i>point</i>.
	//-----------------------------------------------------------------------
	void ForwardMessage(UINT uiMsg, UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates style to show system shadow.
	//-----------------------------------------------------------------------
	void CreateShadow();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTipWindow)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTipWindow)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	COLORREF AlphaPixel(COLORREF crPixel, int i);
	int CheckValue(int iValue);

protected:
	BOOL m_bSystemShadow;
	int m_nLineSpace;	// Amount in pixels to space the title and description text.
	int m_nTabPositions; // Specifies the number of tab-stop positions in the array pointed to by
						 // <i>m_pnTabStopPositions</i>.
	UINT m_nElapseTimerEventID; // Elapse timer event ID.
	UINT m_nDelayTimerEventID;  // Delay timer event ID.
	BOOL m_bDelayTimerRunning;  // TRUE if the delay timer is running.
	BOOL m_bCenterHorz;			// TRUE if the window is to be centered horizontally.
	LPINT m_pnTabStopPositions; // Pointer to an array of integers containing the tab-stop positions
								// in logical units. The tab stops must be sorted in increasing
								// order; the smallest x-value should be the first item in the
								// array. Back tabs are not allowed.
	DWORD m_dwTipStyle;			// Initial style for the tip window. See ShowTipWindow(..).
	COLORREF m_crBackColor;		// Background color for the tip window.
	COLORREF m_crTextColor;		// Text color for the tip window.
	CRect m_rcWindow;			// Initial size of the tip window.
	CRect m_rcScreenWindow;		// Initial size of the desktop.
	CRect m_rcShadow;			// Initial size of the tip window shadow.
	CRect m_rectTipArea;		// Size of the client area for the tip window.
	CPoint m_pointTipOffsetPos; // Specifies the offset to move the top window in relation to the
								// mouse cursor.
	CPoint m_ptMousePos;  // Initial x- and y- coordinates of the cursor when when tip window was
						  // created.
	CWnd* m_pParentWnd;   // Parent window.
	CSize m_sizeMargin;   // Amount in pixels of the tip and side margins of where the tip text is
						  // displayed.
	CSize m_sizeTitle;	// Title text size.
	CString m_strTitle;   // Title text for the tip window.
	CString m_strDescrip; // Description text for the tip window.
	CStringArray m_arStrings; // Stores strings if the tip is multiline.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPTipWindow::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
									  const RECT& rect, CWnd* pParentWnd, UINT nID,
									  CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTPTipWindow::SetTipColors(COLORREF crBackColor, COLORREF crTextColor,
											BOOL bRedraw)
{
	m_crBackColor = crBackColor;
	m_crTextColor = crTextColor;
	if (bRedraw)
	{
		InvalidateRect(NULL);
	}
}
AFX_INLINE void CXTPTipWindow::SetMargins(CSize size)
{
	m_sizeMargin.cx = __max(0, size.cx);
	m_sizeMargin.cy = __max(0, size.cy); // EM_BM
}
AFX_INLINE void CXTPTipWindow::SetLineSpace(int nLineSpace)
{
	m_nLineSpace = __max(0, nLineSpace);
}
AFX_INLINE void CXTPTipWindow::SetTipOffsetPos(CPoint pointOffset)
{
	m_pointTipOffsetPos = pointOffset;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPTIPWINDOW_H__)
