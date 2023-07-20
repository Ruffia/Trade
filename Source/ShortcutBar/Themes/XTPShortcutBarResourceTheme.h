// XTPShortcutBarResourceTheme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBARRESOURCETHEME_H__)
#	define __XTPSHORTCUTBARRESOURCETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarResourceTheme is a CXTPShortcutBarOffice2003Theme
//     derived class that represents the Office 2007, 2010 and 2012 shortcut bar themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarResourceTheme : public CXTPShortcutBarOffice2003Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarResourceTheme object.
	//-------------------------------------------------------------------------
	CXTPShortcutBarResourceTheme();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the shortcut bar paint manager.
	// Remarks:
	//     This method refreshes all of the visual metrics of the shortcut bar
	//     (pane caption color, pane font, pane caption height, etc.).
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the gripper for the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//     bDraw        - TRUE to draw the gripper, FALSE to only
	//                    retrieve the size of the gripper.
	// Returns:
	//     The size of the gripper (i.e. 7 pixels for flat style, 9 pixels otherwise).
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
	//     Fills the client area of the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//-----------------------------------------------------------------------
	void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

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
	virtual int DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the expand button.
	// Parameters:
	//     pDC - Pointer to the client device context.
	//     rc  - Client rectangle of the expand button.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified navigation item for the minimized shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	virtual void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	virtual void DrawPaneCaptionMinimizeButton(CDC* pDC, CXTPShortcutBarPane* pPane);

public:
	BOOL m_bFlatStyle;
};

typedef CXTPShortcutBarResourceTheme CXTPShortcutBarOffice2007Theme;
typedef CXTPShortcutBarResourceTheme CXTPShortcutBarOffice2010Theme;

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBARRESOURCETHEME_H__)
