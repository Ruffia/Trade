// XTPShortcutBarOfficeXPTheme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBAROFFICEXPTHEME_H__)
#	define __XTPSHORTCUTBAROFFICEXPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarOfficeXPTheme is a CXTPShortcutBarPaintManager
//     derived class that represents an Office XP shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOfficeXPTheme : public CXTPShortcutBarPaintManager
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
	//     The size of the gripper (i.e. 8 pixels for Office XP theme).
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

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
#endif // !defined(__XTPSHORTCUTBAROFFICEXPTHEME_H__)
