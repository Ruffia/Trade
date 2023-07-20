// XTPMessageBar.h : interface for the CXTPMessageBar class.
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
#if !defined(__XTPMESSAGEBAR_H__)
#	define __XTPMESSAGEBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	define XTP_IDW_MESSAGE_BAR 0xE806

class CXTPCommandBars;
class CXTPMarkupUIElement;
class CXTPMarkupContext;
class CXTPMessageBar;
class CXTPPaintManager;
class CXTPMessageBarPaintManager;

//===========================================================================
// Summary:
//     CXTPMessageBarButton is a standalone class.
//     It represents a single button of a message bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMessageBarButton
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMessageBarButton object.
	//-----------------------------------------------------------------------
	CXTPMessageBarButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMessageBarButton object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPMessageBarButton();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Generates a click event for the button, simulating a click by a user.
	// Parameters:
	//     pBar - Reference to the message bar the button belongs to.
	//     pt - Point in which to simulate a click.
	//-----------------------------------------------------------------------
	void PerformClick(CXTPMessageBar* pBar, CPoint pt);

public:
	CRect m_rcButton;	 // Bounding rectangle for the button
	CString m_strCaption; // Text displayed in the button
	CString m_strToolTip; // Tooltip text displayed for the button
	BOOL m_bPressed;	  // TRUE if the button is currently pressed
	BOOL m_bHot; // TRUE if the button is in a "hot" state, meaning the mouse cursor is hovered over
				 // the button
	UINT m_nID;						   // Id of the button
	CXTPMarkupUIElement* m_pUIElement; // Store pointer to CXTPMarkupUIElement object.
};

//===========================================================================
// Summary:
//     CXTPMessageBar is a CControlBar derived class.
//     It can be used to show some information in the top of a frame.
//===========================================================================
class _XTP_EXT_CLASS CXTPMessageBar : public CControlBar
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMessageBar object.
	//-----------------------------------------------------------------------
	CXTPMessageBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMessageBar object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMessageBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a message bar.
	// Parameters:
	//     pCommandBars - Parent CommandBar control to attach the message bar.
	//     dwStyle - Style of the message bar.
	//     nID - Id of the message bar.
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL Create(CXTPCommandBars* pCommandBars, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
				UINT nID = XTP_IDW_MESSAGE_BAR);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the message displayed in the message bar.
	// Parameters:
	//     lpszMessage - String of text or XML Markup to display in the
	//                   message bar.
	// Remarks:
	//     The message displayed can contain simple text or it can
	//     contain a XML string as in the sample below. Use XML strings
	//     to add items such as images and formatted text.
	//-----------------------------------------------------------------------
	void SetMessage(LPCTSTR lpszMessage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for the text inside the message bar.
	// Parameters:
	//     bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a button to the message bar.
	// Parameters:
	//     nID - ID of the button.
	//     lpszCaption - Text displayed inside the button.
	//     lpszToolTop - Text displayed as a tooltip when the mouse is placed over the button.
	//-----------------------------------------------------------------------
	void AddButton(UINT nID, LPCTSTR lpszCaption, LPCTSTR lpszToolTop = NULL);

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all buttons from the message bar.
	//-------------------------------------------------------------------------
	void RemoveButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Generates a click event for the button, simulating a click by a user.
	// Parameters:
	//     pButton - Reference to the button to click.
	//-----------------------------------------------------------------------
	virtual void Click(CXTPMessageBarButton* pButton);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the message box.
	// Returns:
	//     The bounding rectangle of the message box.
	//-----------------------------------------------------------------------
	CRect GetMessageRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the paint manager.
	// Returns:
	//     A pointer to the paint manager.
	// ----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the paint manager used for drawing a message bar.
	// Returns:
	//     A pointer to the paint manager used for drawing a message bar.
	//-----------------------------------------------------------------------
	CXTPMessageBarPaintManager* GetMessageBarPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Tests a point to determine which message bar button, if any,
	//     is at the specified point.
	// Parameter:
	//     point - POINT structure that contains the point to be hit
	//             test, in client coordinates.
	// Returns:
	//     If the hit test is successful, contains a reference to the
	//     CXTPMessageBarButton at the hit test point, if any, or - 1 otherwise.
	//-----------------------------------------------------------------------
	CXTPMessageBarButton* HitTestButton(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified button.
	// Parameters:
	//     nID - Button's ID.
	// Returns:
	//     The CXTPMessageBarButton object if successful; otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPMessageBarButton* FindButton(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the markup context.
	// Returns:
	//     A pointer to a CXTPMarkupContext object.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the height of the message bar.
	//     If a custom value has been previously set by SetHeight, that value
	//     is returned. Otherwise it will return the paint manager specific value.
	// ----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the custom height of the message bar.
	//     If auto-sizing is enabled, then this value will be used as the minimum
	//     height of the message bar.
	// ----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables auto-sizing of the message bar.
	//     If auto-sizing is enabled, then the height of the message bar will
	//     be adjusted to the content.
	// ----------------------------------------------------------------------
	void SetAutoSize(BOOL bAutoSize);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the message bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void FillMessageBar(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of the content in the message bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     Returns the size of the content.
	//-----------------------------------------------------------------------
	virtual CSize MeasureContent(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of a button.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pButton - Button to measure.
	// Returns:
	//     Returns the size of a button.
	//-----------------------------------------------------------------------
	virtual CSize MeasureButton(CDC* pDC, CXTPMessageBarButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the content of the message bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void DrawContent(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to recalculate the layout of the
	//     message bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void RecalcLayout(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw a button in the message bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pButton - Button to draw.
	//-----------------------------------------------------------------------
	virtual void DrawButton(CDC* pDC, CXTPMessageBarButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw the buttons in the message bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void DrawButtons(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPMessageBar)

	virtual CSize CalcFixedLayout(BOOL, BOOL bHorz);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPMessageBar)

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	//}}AFX_MSG

protected:
	int m_nCustomHeight; // Custom height value. If 0, height value provided be paint manager is
						 // used.
	BOOL m_bAutoSize;	// If TRUE the height of the bar will be adjusted to the content

	CString m_strContent;				 // Content string of the message bar.
	CXTPMarkupUIElement* m_pUIElement;   // Store pointer to CXTPMarkupUIElement object.
	CXTPMarkupContext* m_pMarkupContext; // Store pointer to CXTPMarkupContext object.
	CXTPCommandBars* m_pCommandBars;	 // Store pointer to CXTPCommandBars object.

	CXTPMessageBarButton* m_pHotButton;								   // Current hot button.
	CArray<CXTPMessageBarButton*, CXTPMessageBarButton*> m_arrButtons; // Collection of buttons in
																	   // the message bar.

	CRect m_rcContent; // Bounding rectangle of the content in the message bar.

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	void OnFinalRelease();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMessageBar);
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	afx_msg BOOL OleGetVisible();
	afx_msg void OleSetVisible(BOOL bNewValue);
	afx_msg BSTR OleGetMessage();
	afx_msg void OleSetMessage(LPCTSTR lpszMessage);
	afx_msg void OleAddButton(long Id, LPCTSTR lpszCaption, LPCTSTR lpszToolTip);
	afx_msg void OleAddCloseButton(LPCTSTR lpszToolTip);

	afx_msg int OleGetHeight();
	afx_msg void OleSetHeight(int nHeight);

//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	using CControlBar::Create;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPMarkupContext* CXTPMessageBar::GetMarkupContext() const
{
	return m_pMarkupContext;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPMESSAGEBAR_H__)
