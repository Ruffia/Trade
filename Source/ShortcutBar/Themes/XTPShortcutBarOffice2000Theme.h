// XTPShortcutBarOffice2000Theme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBAROFFICE2000THEME_H__)
#	define __XTPSHORTCUTBAROFFICE2000THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarOffice2000Theme is a CXTPShortcutBarPaintManager
//     derived class that represents an Office 2000 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOffice2000Theme : public CXTPShortcutBarPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client area of the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//-----------------------------------------------------------------------
	void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the frame for the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	// Remarks:
	//     The frame is the border drawn around the entire shortcut bar.
	//-----------------------------------------------------------------------
	void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified item for the shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the gripper for the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//     bDraw        - TRUE to draw the gripper, FALSE to only
	//                    retrieve the size of the gripper.
	// Returns:
	//     The size of the gripper (i.e. 5 pixels for Office 2000 theme).
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the rectangle to be used to draw the shortcut bar items.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rc     - Specifies the rectangle in logical units.
	//     nPen   - Specifies the color used to paint the rectangle.
	//     nBrush - Specifies the color used to fill the rectangle.
	// Remarks:
	//     Rectangle is used to draw the rectangle of the shortcut bar items
	//     in the Office XP theme. This will draw the normal, pressed, and
	//     hot versions of the items.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the "gripper" lines in the gripper.
	// Parameters:
	//     pDC  - Pointer to the client device context.
	//     x0   - Specifies the logical x- coordinate of the start position of the line.
	//     y0   - Specifies the logical y- coordinate of the start position of the line.
	//     x1   - Specifies the logical x- coordinate of the end position of the line.
	//     y1   - Specifies the logical y- coordinate of the end position of the line.
	//     nPen - Specifies the color used to paint the line.
	//-----------------------------------------------------------------------
	void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the image of a shortcut bar item.
	// Parameters:
	//     pDC       - Pointer to the client device context.
	//     pt        - Location at which to draw the image
	//                 within the specified device context.
	//     sz        - Size of the image.
	//     pImage    - Pointer to a CXTPImageManagerIcon object.
	//     bSelected - TRUE if the shortcut bar item is selected/has focus
	//                 (i.e. when the user clicks on the item).
	//     bPressed  - TRUE if the shortcut bar item is currently pressed
	//                 (i.e. when the user is clicking on the item).
	//     bChecked  - TRUE if the shortcut bar item is checked
	//                 (i.e. toggle buttons).
	//     bEnabled  - TRUE to draw the item as enabled,
	//                 FALSE to draw the item as disabled.
	// Remarks:
	//     The DrawShortcutItem method uses this method to draw shortcut bar images.
	//-----------------------------------------------------------------------
	void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected,
				   BOOL bPressed, BOOL bChecked, BOOL bEnabled);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBAROFFICE2000THEME_H__)
