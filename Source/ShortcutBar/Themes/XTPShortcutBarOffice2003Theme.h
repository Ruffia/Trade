// XTPShortcutBarOffice2003Theme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBAROFFICE2003THEME_H__)
#	define __XTPSHORTCUTBAROFFICE2003THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarOffice2003Theme is a CXTPShortcutBarPaintManager
//     derived class that represents an Office 2003 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOffice2003Theme : public CXTPShortcutBarPaintManager
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarOffice2003Theme object.
	//-------------------------------------------------------------------------
	CXTPShortcutBarOffice2003Theme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client area of the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//-----------------------------------------------------------------------
	virtual void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

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
	//     The size of the gripper (i.e. 7 pixels for Office 2003 theme).
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption for a specified shortcut bar pane.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pPane - Pointer to the CXTPShortcutBarPane object to be drawn.
	//     bDraw - TRUE to draw the shortcut bar pane caption, FALSE to only
	//             retrieve the size of the shortcut bar pane caption.
	// Remarks:
	//     Panes of the shortcut bar are displayed in the shortcut bar's
	//     client area. Each pane consists of a caption and one or more
	//     sub-panes. Each sub-pane has its own caption and is drawn by
	//     the DrawPaneItemCaption method.
	//
	//     The caption of the shortcut bar pane is the bar on the top the pane.
	//     The caption of the shortcut bar pane is set when the
	//     pane is created by the CXTPShortcutBarPane::Create method.
	//
	//     The caption of the shortcut bar pane can be retrieved by using
	//     the CXTPShortcutBarPane::GetCaption method.
	//
	//     Shortcut bar panes have sub-items which also have captions that
	//     are drawn by the DrawPaneItemCaption method.
	// Returns:
	//     The size of the shortcut pane caption (i.e. 24 pixels).
	//-----------------------------------------------------------------------
	int DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption for a specified shortcut bar pane item.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarPaneItem object to be drawn.
	//     bDraw - TRUE to draw the shortcut bar pane item caption, FALSE to only
	//             retrieve the size of the shortcut bar pane item caption.
	// Remarks:
	//     Panes of the shortcut bar are displayed in the shortcut bar's
	//     client area. Each pane consists of a caption and one or more
	//     sub-panes. Each sub-pane has its own caption and is drawn by
	//     the DrawPaneItemCaption method.
	//
	//     The caption of the shortcut bar pane item is set when the
	//     pane item is created by the CXTPShortcutBarPane::AddItem method.
	//
	//     The sub-items of a shortcut bar pane can be retrieved by using
	//     the CXTPShortcutBarPane::GetItem method.
	// Returns:
	//     The size of the shortcut pane item caption.
	//-----------------------------------------------------------------------
	int DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the shortcut bar paint manager.
	// Remarks:
	//     This method refreshes all of the visual metrics of the shortcut bar
	//     (pane caption color, pane font, pane caption height, etc.).
	//
	//     If luna colors are enabled, then the shortcut bar will be colored
	//     to match the current system theme if the XTPCurrentSystemTheme is
	//     xtpSystemThemeBlue, xtpSystemThemeSilver, or xtpSystemThemeOlive.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified navigation item for the minimized shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

public:
	CXTPPaintManagerColorGradient m_grcCaption;		   // Caption color.
	CXTPPaintManagerColorGradient m_grcItemCaption;	// Items color.
	CXTPPaintManagerColorGradient m_grcItemHotCaption; // Items color.
	CXTPPaintManagerColor m_clrItemCaptionShadow;	  // Color of sub-caption shadow.
	CXTPPaintManagerColor m_clrItemCaptionTop;		   // Color under sub-caption rectangle.
	CXTPPaintManagerColor m_clrCaptionText;			   // Color of items text.
	CXTPPaintManagerColor m_clrItemCaptionText;		   // Color of sub-caption text.

	BOOL m_bShowTopLeftCaptionShadow; // TRUE to draw rectangle around items.
	BOOL m_bLunaColorDisabled;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBAROFFICE2003THEME_H__)
