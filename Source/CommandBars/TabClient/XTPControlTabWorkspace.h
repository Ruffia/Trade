// XTPControlTabWorkspace.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPCONTROLTABWORKSPACE_H__)
#	define __XTPCONTROLTABWORKSPACE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     TabWorkspace control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlTabWorkspace
	: public CXTPControlButton
	, public CXTPTabClientWnd::CWorkspace
{
	DECLARE_XTP_CONTROL(CXTPControlTabWorkspace)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlTabWorkspace object.
	//-----------------------------------------------------------------------
	CXTPControlTabWorkspace();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlTabWorkspace object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	~CXTPControlTabWorkspace();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the visual properties of the tabs are
	//     changed. For example, color, mouse-over, and on-click. This
	//     member must be overridden in derived classes.
	// Parameters:
	//     lpRect - The rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called to get the window handle of the workspace.
	// Returns:
	//     The window handle of the workspace.
	//-----------------------------------------------------------------------
	virtual CWnd* GetWindow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the paint manager.
	// Returns:
	//     A pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager* GetPaintManager() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to recalculate the layout of the tab client.
	//-------------------------------------------------------------------------
	virtual void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current position of the tabs.
	// Returns:
	//     Current XTPTabPosition of the tabs.
	// Remarks:
	//     The position refers to where the tabs are located in relation
	//     to the TabClient. The tabs can be positioned on the top,
	//     bottom, left, or right side of the TabClient.
	//-----------------------------------------------------------------------
	XTPTabPosition GetPosition() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is removed from the TabManager
	//     collection.
	//-------------------------------------------------------------------------
	virtual void OnRemoved();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to set the bounding rectangle of the
	//     tab control.
	// Parameters:
	//     rcControl - Bounding rectangle of the tab control.
	//-----------------------------------------------------------------------
	virtual void SetRect(CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw the tab workspace.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the size of the tab workspace.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     A CSize object containing the size of the tab workspace.
	//-----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether
	//     a point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor. These
	//             coordinates are always relative to the upper-left
	//             corner of the window.
	//     pTI - A pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, then the window control ID is returned.
	//     If the tooltip control was not found, then a value of -1 is returned.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	void OnMouseMove(CPoint point);

protected:
	BOOL m_bForceRecalc; // TRUE to force the tab control to recalculate it's layout.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCONTROLTABWORKSPACE_H__)
