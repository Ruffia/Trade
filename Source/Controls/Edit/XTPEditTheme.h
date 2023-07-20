// XTPEditTheme.h: interface for the CXTPEditTheme class.
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
#if !defined(__XTPEDITTHEME_H__)
#	define __XTPEDITTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPWinThemeWrapper;
class CXTPEdit;

//===========================================================================
// Summary:
//     CXTPEditTheme is the base class for all edit themes used to perform
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPEditTheme : public CXTPControlTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPEditThemeDefault object.
	//-----------------------------------------------------------------------
	CXTPEditTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructor, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPEditTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each component's theme.
	// Parameters:
	//     pEdit - Pointer to a CXTPEdit object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPEdit* pEdit) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to draw
	//     non-client borders for the edit control.
	// Parameters:
	//     pDC   - Pointer to a valid device context for the control.
	//     rcWnd - Size of the area to draw borders on.
	//     pEdit - Pointer to a CXTPEdit object.
	//-----------------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the non-client background for
	//     the combobox scrollbar area.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     rcWnd - Size in window coordinates for the combobox.
	//     pEdit - Pointer to the edit control to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawNcScrollBack(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the edit control to fill a rectangle using the brush
	//     specifed by pBrush.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     x      - Left side of the rectangle to draw.
	//     y      - Top side of the rectangle to draw.
	//     cx     - Width of the rectangle to draw.
	//     cy     - Height of the rectangle to draw.
	//     pBrush - Pointer to a CBrush object representing the background
	//              fill color.
	//-----------------------------------------------------------------------
	virtual void FillRect(CDC* pDC, int x, int y, int cx, int cy, CBrush* pBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the edit control to draw a frame rectangle using the
	//     brush specified by pBrush.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rc     - CRect object representing the size of rectangle to draw.
	//     nSize  - Width in pixels of the rectangle borders.
	//     pBrush - Pointer to a CBrush object representing the border
	//              fill color.
	//-----------------------------------------------------------------------
	virtual void DrawFrame(CDC* pDC, CRect rc, int nSize, CBrush* pBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to the client
	//     background brush.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     A CBrush pointer representing the client background brush.
	//-----------------------------------------------------------------------
	virtual CBrush* GetClientBrush(CDC* pDC, CXTPEdit* pEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to handle setting the text and background
	//     colors for the edit control.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     nCtlColor - Specifies type of control (either CTLCOLOR_STATIC
	//                 or CTLCOLOR_EDIT).
	//     pEdit     - Pointer to the edit control to to be drawn.
	//-----------------------------------------------------------------------
	virtual CBrush* CtlColor(CDC* pDC, UINT nCtlColor, CXTPEdit* pEdit) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to adjust the size of the area to draw
	//     the buddy control borders on.
	// Parameters:
	//     pBuddy     - Pointer to the buddy control associated with the edit.
	//     rcUpDown   - Size of the buddy border area to be drawn.
	//     nAlignment - Side of the edit box that the buddy is attached to
	//                  (either UDS_ALIGNRIGHT or UDS_ALIGNLEFT).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the borders for the associated
	//     buddy window.
	// Parameters:
	//     pBuddy     - Pointer to the buddy control associated with the edit.
	//     pDC        - Pointer to a valid device context.
	//     rc         - Size of the buddy border area to be drawn.
	//     nAlignment - Side of the edit box that the buddy is attached to
	//                  (either UDS_ALIGNRIGHT or UDS_ALIGNLEFT).
	//     pEdit      - Pointer to the edit control for the buddy window.
	//-----------------------------------------------------------------------
	virtual void DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignment,
								  CXTPEdit* pEdit) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to redraw the focus rectangle for the
	//     edit control.
	// Returns:
	//     The implementation must return TRUE to indicate that the focused
	//     frame has to be redrawn, or FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL RedrawFocusedFrame() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the m_bFlatStyle
	//     flag has be set.
	// Returns:
	//     TRUE if the flag has been set, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsFlatStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set a value for m_bFlatStyle.
	// Parameters:
	//     bFlatStyle - TRUE to set the flat style flag.
	//-----------------------------------------------------------------------
	void SetFlatStyle(BOOL bFlatStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the style for the theme.
	// Returns:
	//     A XTPControlTheme enumerator index value representing the
	//     theme style.
	//-----------------------------------------------------------------------
	XTPControlTheme GetThemeID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable Windows themes for the
	//     edit control.
	// Parameters:
	//     bUseWinTheme - TRUE to use Windows themes.
	//-----------------------------------------------------------------------
	void UseWinTheme(BOOL bUseWinTheme);

	CXTPBrush m_xtpBrushBack; // Background brush color.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brBack, m_xtpBrushBack, GetBackBrushHandle);

	CXTPPaintManagerColor m_clrBack;		   // Background color.
	CXTPPaintManagerColor m_clrText;		   // Text color.
	CXTPPaintManagerColor m_clrTextReadOnly;   // Read-only text color.
	CXTPPaintManagerColor m_clrBackNonClient;  // Non-client area background color.
	CXTPPaintManagerColorGradient m_clrBorder; // Dark and light border colors.

protected:
	BOOL m_bFlatStyle;				// TRUE when drawing flat borders.
	BOOL m_bUseWinTheme;			// TRUE to use windows themes.
	XTPControlTheme m_nTheme;		// Specifies the Theme ID used by the paint manager.
	CXTPWinThemeWrapper m_winTheme; // Windows theme wrapper.

private:
	friend class CXTPEdit;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE XTPControlTheme CXTPEditTheme::GetThemeID() const
{
	return m_nTheme;
}
AFX_INLINE void CXTPEditTheme::UseWinTheme(BOOL bUseWinTheme)
{
	m_bUseWinTheme = bUseWinTheme;
}
AFX_INLINE BOOL CXTPEditTheme::IsFlatStyle() const
{
	return m_bFlatStyle;
}
AFX_INLINE void CXTPEditTheme::SetFlatStyle(BOOL bFlatStyle)
{
	m_bFlatStyle = bFlatStyle;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // __XTPEDITTHEME_H__
