// XTPShortcutBarOffice2013Theme.h interface for the CXTPShortcutBarOffice2013Theme class.
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
#if !defined(__XTPSHORTCUTBAROFFICE2013THEME_H__)
#	define __XTPSHORTCUTBAROFFICE2013THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarOffice2013Theme is a CXTPShortcutBarResourceTheme
//     derived class that represents an Office 2013 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOffice2013Theme : public CXTPShortcutBarResourceTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarOffice2013Theme object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarOffice2013Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarOffice2013Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarOffice2013Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the shortcut bar paint manager.
	// Remarks:
	//     This method refreshes all of the visual metrics of the shortcut bar
	//     (pane caption color, pane font, pane caption height, etc.).
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	virtual int DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the expand button.
	// Parameters:
	//     pDC - Pointer to the client device context.
	//     rc  - Client rectangle of the expand button.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC* pDC, CRect rc);

	virtual void DrawPaneCaptionMinimizeButton(CDC* pDC, CXTPShortcutBarPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the gripper for the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//     bDraw        - TRUE to draw the gripper, FALSE to only
	//                    retrieve the size of the gripper.
	// Returns:
	//     The size of the gripper (i.e. 7 pixels for Office 2013 theme).
	//-----------------------------------------------------------------------
	virtual int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

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
	//     Fills a specified navigation item for the minimized shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	virtual void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	virtual void FillPopupBack(CDC* pDC, CRect rc, CSize szGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client area of the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//-----------------------------------------------------------------------
	virtual void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

protected:
	CXTPPaintManagerColor m_clrBack;
	CXTPPaintManagerColor m_clrBackHilite;
	CXTPPaintManagerColor m_clrBackPushed;
	CXTPPaintManagerColor m_clrBackPushedDark;
	CXTPPaintManagerColor m_clrBackSelected;
	CXTPPaintManagerColor m_clrGlyph;		  // Normal glyph color.
	CXTPPaintManagerColor m_clrGlyphFill;	 // Expanded glyph fill color.
	CXTPPaintManagerColor m_clrGlyphExpanded; // Expanded glyph border color.
	CXTPPaintManagerColor m_clrGlyphPushed;   // Pushed glyph color.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBAROFFICE2013THEME_H__)
