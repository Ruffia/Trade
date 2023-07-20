// XTPShortcutBarVisualStudio2019Theme.h
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
#if !defined(__XTPSHORTCUTBARVISUALSTUDIO2019THEME_H__)
#	define __XTPSHORTCUTBARVISUALSTUDIO2019THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarVisualStudio2019Theme is a CXTPShortcutBarOffice2013Theme
//     derived class that represents a Visual Studio 2019 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarVisualStudio2019Theme : public CXTPShortcutBarOffice2013Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarVisualStudio2019Theme object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarVisualStudio2019Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarVisualStudio2019Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarVisualStudio2019Theme();

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

	virtual void DrawPaneCaptionMinimizeButton(CDC* pDC, CXTPShortcutBarPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified item for the shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	virtual void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified navigation item for the minimized shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	virtual void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	void DrawHiliteBorder(CDC* pDC, CRect rc);

protected:
	BOOL m_bHiliteBorders;						   // TRUE to draw borders around highlighted items.
	CXTPPaintManagerColor m_clrBorderHilite;	   // Highlighted border color.
	CXTPPaintManagerColor m_clrBackHiliteSelected; // Selected highlighted background color.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBARVISUALSTUDIO2019THEME_H__)
