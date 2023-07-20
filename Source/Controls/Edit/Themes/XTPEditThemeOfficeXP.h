// XTPEditThemeOfficeXP.h
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
#if !defined(__XTPEDITTHEMEOFFICEXP_H__)
#	define __XTPEDITTHEMEOFFICEXP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPEditThemeOfficeXP is a class used to perform theme drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPEditThemeOfficeXP : public CXTPEditThemeOffice2000
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPEditThemeOfficeXP object.
	//-----------------------------------------------------------------------
	CXTPEditThemeOfficeXP();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructor, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPEditThemeOfficeXP();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each component's theme.
	// Parameters:
	//     pEdit - Pointer to a CXTPEdit object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPEdit* pEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to draw
	//     non-client borders for the edit control.
	// Parameters:
	//     pDC   - Pointer to a valid device context for the control.
	//     rcWnd - Size of the area to draw borders on.
	//     pEdit - Pointer to a CXTPEdit object.
	//-----------------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rcWnd, CXTPEdit* pEdit);

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
								  CXTPEdit* pEdit);

#	if _MSC_VER > 1200
	using CXTPEditThemeDefault::DrawBuddyBorders;
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to redraw the focus rectangle for the
	//     edit control.
	// Returns:
	//     The implementation must return TRUE to indicate that the focused
	//     frame has to be redrawn, or FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL RedrawFocusedFrame() const;

protected:
	CXTPPaintManagerColor m_clrBorderHilite;   // Highlighted border color.
	CXTPPaintManagerColor m_clrBorderDisabled; // Disabled border color.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // __XTPEDITTHEMEOFFICEXP_H__
