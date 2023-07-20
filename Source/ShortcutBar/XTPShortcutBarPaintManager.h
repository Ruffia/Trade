// XTPShortcutBarPaintManager.h interface for the CXTPShortcutBarPaintManager class.
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
#if !defined(__XTPSHORTCUTBARPAINTMANAGER_H__)
#	define __XTPSHORTCUTBARPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPWinThemeWrapper;

//---------------------------------------------------------------------------
// Summary:
//     XTPShortcutBarPaintTheme is an enumeration used by CXTPShortcutBar to
//     set the theme of the control.
// Example:
// <code>
//     m_wndShortcutBar.SetTheme(xtpShortcutThemeOffice2003);
// </code>
// See Also:
//     CXTPShortcutBar::GetCurrentTheme, CXTPShortcutBar::SetTheme, CXTPShortcutBar::SetCustomTheme
//
// <KEYWORDS xtpShortcutThemeOffice2000, xtpShortcutThemeOfficeXP, xtpShortcutThemeOffice2003>
//---------------------------------------------------------------------------
enum XTPShortcutBarPaintTheme
{
	xtpShortcutThemeResource		 = 3,						   // Office 2007/2010 style theme.
	xtpShortcutThemeOffice2000		 = 0,						   // Office 2000 style theme.
	xtpShortcutThemeOfficeXP		 = 1,						   // Office XP style theme.
	xtpShortcutThemeOffice2003		 = 2,						   // Office 2003 style theme.
	xtpShortcutThemeOffice2007		 = 3,						   // Office 2007 style theme.
	xtpShortcutThemeOffice2010		 = 3,						   // Office 2010 style theme.
	xtpShortcutThemeNativeWinXP		 = 4,						   // Native Windows XP theme.
	xtpShortcutThemeOffice2013		 = 5,						   // Office 2013 style theme.
	xtpShortcutThemeOffice2016		 = xtpShortcutThemeOffice2013, // Office 2016 style theme.
	xtpShortcutThemeVisualStudio2015 = 6,  // Visual Studio 2015 style theme.
	xtpShortcutThemeNativeWindows10  = 7,  // Native Windows 10 theme.
	xtpShortcutThemeVisualStudio2017 = 8,  // Visual Studio 2017 style theme.
	xtpShortcutThemeVisualStudio2019 = 9,  // Visual Studio 2019 style theme.
	xtpShortcutThemeVisualStudio2022 = 10, // Visual Studio 2022 style theme.
};

class CXTPShortcutBar;
class CXTPShortcutBarItem;
class CXTPShortcutBarPane;
class CXTPShortcutBarPaneItem;
class CXTPImageManagerIcon;

//===========================================================================
// Summary:
//     CXTPShortcutBarPaintManager is a standalone class used
//     as a paint manager for a shortcut bar control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarPaintManager : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPaintManager object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarPaintManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarPaintManager();

public:
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
	//     Draws a specified item for the shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawShortcutItem(CDC* pDC, CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client area of the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//-----------------------------------------------------------------------
	virtual void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the frame for the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	// Remarks:
	//     The frame is the border drawn around the entire shortcut bar.
	//-----------------------------------------------------------------------
	virtual void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified item for the shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	virtual void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the gripper for the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//     bDraw        - TRUE to draw the gripper, FALSE to only
	//                    retrieve the size of the gripper.
	// Returns:
	//     The size of the gripper.
	//-----------------------------------------------------------------------
	virtual int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw) = 0;

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
	//     Draws a specified navigation item for the minimized shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawNavigationItem(CDC* pDC, CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified navigation item for the minimized shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	virtual void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

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
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage,
						   BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled) = 0;

public:
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
	//     in the Office XP theme.
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
	//     Determines if the caption text of a specified expanded shortcut bar item
	//     has been truncated.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to check.
	// Remarks:
	//     The caption text of an expanded shortcut bar item is truncated if
	//     the shortcut bar item is not wide enough to display the entirety of
	//     the caption text.
	// Returns:
	//     TRUE if the specified shortcut bar item is both expanded and
	//     its caption text has been truncated,
	//     FALSE if the specified shortcut bar item is either not expanded or
	//     its caption text has not been truncated.
	//-----------------------------------------------------------------------
	BOOL IsShortcutItemTruncated(CDC* pDC, CXTPShortcutBarItem* pItem);

	virtual void FillPopupBack(CDC* pDC, CRect rc, CSize szGap);

	virtual void CreateFonts();

	virtual void EnableOfficeFont(BOOL bEnable = TRUE);

public:
	CXTPFont m_xtpFontText;			   // Text font.
	CXTPFont m_xtpFontPaneCaption;	 // Caption font.
	CXTPFont m_xtpFontPaneItemCaption; // Sub-caption font.
	CXTPFont m_xtpFontVerticalText;	// Navigation button text font.

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntText, m_xtpFontText, GetTextFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntPaneCaption, m_xtpFontPaneCaption,
										  GetPaneCaptionFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntPaneItemCaption, m_xtpFontPaneItemCaption,
										  GetPaneItemCaptionFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntVerticalText, m_xtpFontVerticalText,
										  GetVerticalTextFontHandle);

	int m_nPaneItemHeight;				 // Height of sub-pane caption.
	CXTPImageManagerIcon* m_pExpandIcon; // Pointer to expand icon.
	CXTPImageManager* m_pImageManager;
	BOOL m_bNavigationButton; // TRUE if navigation button small

	int m_nNormalIconWidth;		  // Width of the icons.
	int m_nExpandedIconWidth;	 // Width of the expanded icons.
	BOOL m_bBoldItemCaptionFont;  // TRUE to draw item with bold font.
	BOOL m_bClearTypeTextQuality; // TRUE to use ClearType font.
	BOOL m_bUseStandardFont;	  // TRUE to use standard font.

	CRect m_rcCaptionPadding;	 // Padding of pane caption.
	CRect m_rcItemCaptionPadding; // Padding of item caption.
	CString m_strOfficeFont;	  // TRUE to use Office Tahoma font.
	BOOL m_bShowBorder;

	BOOL m_bDrawTextNoPrefix;

public:
	CXTPPaintManagerColor m_clrPushedText;   // Color of caption text when an CXTPShortcutBarItem is
											 // pressed.
	CXTPPaintManagerColor m_clrSelectedText; // Color of caption text when an CXTPShortcutBarItem is
											 // selected (this happens after the item is clicked).
	CXTPPaintManagerColor m_clrHotText; // Color of caption text when the mouse cursor is positioned
										// over an CXTPShortcutBarItem.
	CXTPPaintManagerColor m_clrNormalText;   // Color of caption text when an CXTPShortcutBarItem is
											 // displayed normally.
	CXTPPaintManagerColor m_clrDisabledText; // Color of caption text when an CXTPShortcutBarItem is
											 // disabled.

	CXTPPaintManagerColorGradient m_grcSelected; // Color gradient of currently selected item.
	CXTPPaintManagerColorGradient m_grcHot; // Color gradient of "hot" items.  An item is hot when
											// the mouse cursor is positioned over the item.
	CXTPPaintManagerColorGradient m_grcPushed; // Color gradient of item when clicked on. This color
											   // is also used to color the currently selected item
											   // on mouse over (i.e. when the selected item is
											   // "hot").
	CXTPPaintManagerColorGradient m_grcShortcutItem; // Color gradient of shortcut bar items.
	CXTPPaintManagerColor m_clrShortcutItemShadow;   // Color of line border that goes around each
													 // item.
	CXTPPaintManagerColorGradient m_grcShortcutBarGripper; // Color gradient used to draw the
														   // shortcut bar gripper.

	CXTPPaintManagerColor m_clrPopupFrame;
	CXTPPaintManagerColor m_clrPopupBackground;

	COLORREF m_clrFlatBackground;
	COLORREF m_clrFlatTextColor;
	COLORREF m_clrAppSplitterFace;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPShortcutBarPaintManager);
	DECLARE_INTERFACE_MAP()
	afx_msg void OleRefreshMetrics();
	afx_msg LPFONTDISP OleGetFont();
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg OLE_COLOR OleGetFlatBackgroundColor();
	afx_msg OLE_COLOR OleGetFlatTextColor();
	afx_msg OLE_COLOR OleGetSplitterColor();
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBARPAINTMANAGER_H__)
