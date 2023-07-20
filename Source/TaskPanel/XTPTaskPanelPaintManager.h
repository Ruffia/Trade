// XTPTaskPanelPaintManager.h interface for the CXTPTaskPanelPaintManager class.
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
#if !defined(__XTPTASKPANELPAINTMANAGER_H__)
#	define __XTPTASKPANELPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTaskPanel;
class CXTPTaskPanelGroupItem;
class CXTPTaskPanelScrollButton;
class CXTPWinThemeWrapper;

#	define XTP_TASKPANEL_HIGHLIGHT_OFFICE2003 2

//-----------------------------------------------------------------------
// Summary:
//     Structure used to store task panel group colorization information for
//     various parts of the group.
// Example:
// <code>
//      m_wndTaskPanel.GetPaintManager()->GetGroupColors(FALSE)->clrClientLink = RGB(0, 0, 255);
// </code>
// See Also:
//     CXTPTaskPanelPaintManager::GetGroupColors, CXTPTaskPanelPaintManager::m_groupNormal,
//     CXTPTaskPanelPaintManager::m_groupSpecial
//-----------------------------------------------------------------------
struct XTP_TASKPANEL_GROUPCOLORS
{
	CXTPPaintManagerColorGradient clrHead;   // Color gradient used for the background color of
											 // group headers.
	CXTPPaintManagerColorGradient clrClient; // Background color of the client area of the group.
											 // This is the group area under the group header.
	CXTPPaintManagerColor clrClientBorder;   // Color of the client group border.
											 // This is the group area under the group header.
	double dHeadGradientFactor; // Value used to calculate the gradient of the group header.
								// This is used to increase or decrease the color
								// as the backcolor of the header is filled in.
	CXTPPaintManagerColor clrHeadTextNormal; // Color of normal text in the group header.
	CXTPPaintManagerColor clrHeadTextHot;	// Color of "hot" text in the group header.
											 // This is the color of text on mouse over.
	CXTPPaintManagerColor clrClientText; // Color of text within the client or "body" of the group.
										 // This is the area under the group header.
	CXTPPaintManagerColor clrClientLink; // Color of links within the client or "body" of the group.
										 // This is the area under the group header.
	CXTPPaintManagerColor clrClientLinkHot; // Color of "hot" links within the client or "body" of
											// the group.
											// This is the color of link text on mouse over.
	CXTPPaintManagerColor clrClientLinkSelected; // Color of links within the client or "body" of
												 // the group.
												 // This is the area under the group header.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to indicate which type of gripper is drawn
//     on task panel group headers. This can be used when creating custom
//     themes or you can override the predefined themes to change
//     the grippers if drawn in the group headers.
// Remarks:
//     Depending on the theme and XTPTaskPanelGripper selection, the gripper
//     can appear "hot" on mouse over and display a different gripper
//     than when the group is expanded and closed.
// Example:
// <code>
//      m_wndTaskPanel.GetPaintManager()->m_eGripper = xtpTaskPanelGripperTriangle;
// </code>
// See Also:
//     CXTPTaskPanelPaintManager::m_eGripper
//
// <KEYWORDS xtpTaskPanelGripperNone, xtpTaskPanelGripperBitmap, xtpTaskPanelGripperPlain,
// xtpTaskPanelGripperTriangle>
//-----------------------------------------------------------------------
enum XTPTaskPanelGripper
{
	xtpTaskPanelGripperNone,   // Do not draw a gripper in task panel group headers.
	xtpTaskPanelGripperBitmap, // Display a bitmap in task panel group headers
							   // (i.e. arrows in WinXP theme).
	xtpTaskPanelGripperPlain,  // Display the default "classic" gripper in task panel group headers.
	xtpTaskPanelGripperTriangle // Display a triangle gripper in task panel group headers.
};

//===========================================================================
// Summary:
//     CXTPTaskPanelPaintManager is a standalone class used
//     by CXTPTaskPanel as a paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelPaintManager
{
public:
	static const CRect rectDefault; // System selected default margins. All parameters of
									// this CRECT are set to CW_USEDEFAULT, which tells the system
									// to select the default position for the object (i.e. the
									// upper-left corner ignoring the y parameter). This CRECT is
									// used when calculating the margins of the task panel,
									// group items margin, group outer margin, and group inner
									// margin. If the margins have not been modified, then this
									// CRECT is used to set the margins.

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelPaintManager object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelPaintManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the background of a specified task panel.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pTaskPanel - Pointer to the task panel to fill.
	//-----------------------------------------------------------------------
	virtual void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw.
	//-----------------------------------------------------------------------
	virtual void DrawGroupClient(CDC* pDC, CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the caption of.
	//     bDraw  - TRUE to draw the caption of the specified group,
	//              FALSE to retrieve the height of the caption.
	// Returns:
	//     The height of the caption.
	//-----------------------------------------------------------------------
	virtual int DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a specified group item.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the group item to draw.
	//     rc    - Rectangle of the group item to draw.
	//     bDraw - TRUE to draw the group item,
	//             FALSE to retrieve the height of the group item.
	// Returns:
	//     The height of the group item.
	//-----------------------------------------------------------------------
	virtual CRect DrawGroupItem(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the paint manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified group color set.
	// Parameters:
	//     bSpecial - TRUE to retrieve the special group color set,
	//                FALSE to retrieve the normal group color set.
	//     pGroup   - Pointer to the group with the color set to be retrieved.
	// Remarks:
	//     If a group is special, then it will be displayed in a different color
	//     than other groups in order to draw the user's attention to that group.
	//     This is an excellent way to show the user "default" or important items.
	// Returns:
	//     A pointer to the specified group color set.
	// See Also:
	//     CXTPTaskPanelGroup::IsSpecialGroup, CXTPTaskPanelGroup::SetSpecialGroup
	//-----------------------------------------------------------------------
	XTP_TASKPANEL_GROUPCOLORS* GetGroupColors(BOOL bSpecial);
	virtual XTP_TASKPANEL_GROUPCOLORS* GetGroupColors(
		CXTPTaskPanelGroup* pGroup); // <combine CXTPTaskPanelPaintManager::GetGroupColors@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a rectangle around a group item when it is
	//     selected, pressed or highlighted.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the group item.
	//     rc    - Rectangle to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the outer margins of a specified group item.
	// Parameters:
	//     pItem - Pointer to the group item with the outer margins to retrieve.
	// Returns:
	//     The outer margins of the specified group item.
	// See Also:
	//     GetItemInnerMargins, GetGroupOuterMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetItemOuterMargins(const CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the inner margins of a specified group item.
	// Parameters:
	//     pItem - Pointer to the group item with the inner margins to retrieve.
	// Returns:
	//     The inner margins of the specified group item.
	// See Also:
	//     GetItemOuterMargins, GetGroupOuterMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetItemInnerMargins(const CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the outer margins of a specified group.
	// Parameters:
	//     pGroup - Pointer to the group with the outer margins to retrieve.
	// Returns:
	//     The outer margins of the specified group.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetGroupOuterMargins(const CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon padding of a specified item.
	// Parameters:
	//     pItem - Pointer to the item with the icon padding to retrieve.
	// Returns:
	//     The icon padding of the specified item.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetItemIconPadding(const CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the inner margins of a specified group.
	// Parameters:
	//     pGroup - Pointer to the group with the inner margins to retrieve.
	// Returns:
	//     The inner margins of the specified group.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupOuterMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetGroupInnerMargins(const CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default margins used for all groups within
	//     a specified Task Panel control.
	// Parameters:
	//     pPanel - Pointer to a Task Panel control.
	// Returns:
	//     The default margins used for all groups within
	//     the specified Task Panel control.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupOuterMargins,
	//     GetGroupInnerMargins
	//-----------------------------------------------------------------------
	virtual CRect GetControlMargins(const CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the spacing used between groups within
	//     a specified Task Panel control.
	// Parameters:
	//     pPanel - Pointer to a Task Panel control.
	// Returns:
	//     The spacing used between groups within
	//     the specified Task Panel control.
	//-----------------------------------------------------------------------
	virtual int GetGroupSpacing(const CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a group scroll button.
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     pScrollButton - Pointer to the group scroll button to draw.
	// Remarks:
	//     DrawScrollButton only draws group scroll buttons
	//     (i.e. DrawScrollButton does not draw group caption scroll buttons).
	//     DrawCaptionGripper draws group caption scroll buttons.
	// See Also:
	//     DrawCaptionGripper
	//-----------------------------------------------------------------------
	virtual void DrawScrollButton(CDC* pDC, CXTPTaskPanelScrollButton* pScrollButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text format for captions/items.
	// Parameters:
	//     nTextFormat - Text format to be set.
	//     bCaption    - TRUE to set the text format for captions,
	//                   FALSE to set the text format for items.
	// Example:
	//     <code>m_wndTaskPanel.GetPaintManager()->SetTextFormat(DT_SINGLELINE | DT_END_ELLIPSIS |
	//     DT_VCENTER | DT_NOPREFIX);</code>
	//-----------------------------------------------------------------------
	void SetTextFormat(int nTextFormat, BOOL bCaption = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text font for items.
	// Parameters:
	//     pLogFont              - Pointer to the text font to be set.
	//     bUseStandardItemsFont - TRUE to use the standard text font,
	//                             FALSE to use the text font specified by pLogFont.
	//-----------------------------------------------------------------------
	void SetItemsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardItemsFont = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text font for captions.
	// Parameters:
	//     pLogFont                - Pointer to the text font to be set.
	//     bUseStandardCaptionFont - TRUE to use the standard text font,
	//                               FALSE to use the text font specified by pLogFont.
	//-----------------------------------------------------------------------
	void SetCaptionFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardCaptionFont = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the text font of a specified item.
	// Parameters:
	//     pItem - Pointer to the item with the text font to retrieve.
	// Returns:
	//     A pointer to the text font of the specified item.
	//-----------------------------------------------------------------------
	virtual CFont* GetItemFont(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client face of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to fill the client face of.
	//     rc     - Rectangle of the group to fill the client face of.
	// See Also:
	//     DrawGroupClientFace
	//-----------------------------------------------------------------------
	virtual void FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the icon of a specified group item.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the group item to draw the icon of.
	//     rc    - Rectangle of the group item to draw the icon of.
	//     bDraw - TRUE to draw the icon of the group item,
	//             FALSE to retrieve the bounding rectangle of the group item.
	// Returns:
	//     If bDraw is TRUE and the drawing is successful, then a value of 0 is returned.
	//
	//     If bDraw if FALSE, then the bounding rectangle of the group item is returned
	//     including the inner margins of the item (i.e. the left and right margins
	//     of the item will be the left and right margins of the item in addition to
	//     the left and right inner margins of the item).
	// See Also:
	//     DrawGroupItem, DrawGroupItemImageWithTextBelow, DrawGroupItemImage,
	//     GetItemInnerMargins
	//-----------------------------------------------------------------------
	CRect DrawGroupItemImageWithText(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the icon of a specified group item.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the group item to draw the icon of.
	//     rc    - Rectangle of the group item to draw the icon of.
	//     bDraw - TRUE to draw the icon of the group item,
	//             FALSE to retrieve the bounding rectangle of the group item.
	// Returns:
	//     If bDraw is TRUE and the drawing is successful, then a value of 0 is returned.
	//
	//     If bDraw if FALSE, then the bounding rectangle of the group item is returned
	//     including the inner margins of the item (i.e. the left and right margins
	//     of the item will be the left and right margins of the item in addition to
	//     the left and right inner margins of the item).
	// See Also:
	//     DrawGroupItem, DrawGroupItemImageWithText, DrawGroupItemImage,
	//     GetItemInnerMargins
	//-----------------------------------------------------------------------
	CRect DrawGroupItemImageWithTextBelow(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc,
										  BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the icon of a specified group item.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the group item to draw the icon of.
	//     rc    - Rectangle of the group item to draw the icon of.
	//     bDraw - TRUE to draw the icon of the group item,
	//             FALSE to retrieve the bounding rectangle of the group item.
	// Returns:
	//     If bDraw is TRUE and the drawing is successful, then a value of 0 is returned.
	//
	//     If bDraw if FALSE, then the bounding rectangle of the group item is returned
	//     including the inner margins of the item (i.e. the left and right margins
	//     of the item will be the left and right margins of the item in addition to
	//     the left and right inner margins of the item).
	// See Also:
	//     DrawGroupItem, DrawGroupItemImageWithText, DrawGroupItemImageWithTextBelow,
	//     GetItemInnerMargins
	//-----------------------------------------------------------------------
	CRect DrawGroupItemImage(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the client face of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the client face of.
	//     rc     - Rectangle of the group to draw the client face of.
	//-----------------------------------------------------------------------
	virtual void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the items of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the items of.
	//     rc     - Rectangle of the group to draw the client face of.
	//-----------------------------------------------------------------------
	virtual void DrawGroupClientItems(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption gripper of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the caption gripper of.
	//     rc     - Rectangle of the group to draw the caption gripper of.
	// Returns:
	//     The rectangle of the caption after the drawing is complete.
	// Remarks:
	//     The caption gripper is the button that allows the group
	//     to be expanded and collapsed.
	// See Also:
	//     DrawCaptionGripperBitmap, DrawCaptionGripperSigns
	//-----------------------------------------------------------------------
	virtual CRect DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the width of the caption gripper of a specified group.
	// Parameters:
	//     pGroup - Pointer to the group with the caption gripper
	//              to retrieve the width of.
	//     rc     - Rectangle of the group with the caption gripper
	//              to retrieve the width of.
	// Returns:
	//     The width of the caption gripper of the specified group.
	// Remarks:
	//     The caption gripper is the button that allows the group to be
	//     expanded and collapsed.
	//-----------------------------------------------------------------------
	virtual int GetCaptionGripperWidth(CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the text caption of a specified item.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pItem   - Pointer to the item to draw the text caption of.
	//     rcText  - Rectangle of the item to draw the text caption of.
	//     nFormat - Specifies the method of formatting the text.
	//               See the Remarks section below for a list of available values.
	// Remarks:
	//     The nFormat parameter can be one or more of the following values:
	//        * <b>DT_BOTTOM</b>:               Justifies the text to the bottom of the
	//                                          rectangle. This value is used only with
	//                                          the DT_SINGLELINE value.
	//        * <b>DT_CALCRECT</b>:             Determines the width and height of the
	//                                          rectangle.
	//        * <b>DT_CENTER</b>:               Centers text horizontally in the
	//        * <b>DT_EDITCONTROL</b>:          rectangle. Duplicates the text-displaying
	//                                          characteristics of a multi-line edit
	//                                          control. Specifically, the average character
	//                                          width is calculated in the same manner as
	//                                          for an edit control, and the function does
	//                                          not display a partially visible last line.
	//        * <b>DT_END_ELLIPSIS</b>:         For displayed text, if the end of a string
	//                                          does not fit in the rectangle, it is
	//                                          truncated and ellipses are added. If a
	//                                          word that is not at the end of the string
	//                                          goes beyond the limits of the rectangle,
	//                                          it is truncated without ellipses. The string
	//                                          is not modified unless the DT_MODIFYSTRING
	//                                          flag is specified.
	//        * <b>DT_EXPANDTABS</b>:           Expands tab characters. The default
	//                                          number of characters per tab is eight. The
	//                                          DT_WORD_ELLIPSIS, DT_PATH_ELLIPSIS, and
	//                                          DT_END_ELLIPSIS values cannot be used with
	//                                          the DT_EXPANDTABS value.
	//        * <b>DT_EXTERNALLEADING</b>:      Includes the font external leading in-line
	//                                          height. Normally, external leading is not
	//                                          included in the height of a line of text.
	//        * <b>DT_HIDEPREFIX</b>:           Ignores the ampersand (&) prefix character
	//                                          in the text. The letter that follows will
	//                                          not be underlined, but other mnemonic-prefix
	//                                          characters are still processed.
	//        * <b>DT_INTERNAL</b>:             Uses the system font to calculate
	//                                          text metrics.
	//        * <b>DT_LEFT</b>:                 Aligns text to the left.
	//        * <b>DT_MODIFYSTRING</b>:         Modifies the specified string to match the
	//                                          displayed text. This value has no effect
	//                                          unless DT_END_ELLIPSIS or DT_PATH_ELLIPSIS
	//                                          is specified.
	//        * <b>DT_NOCLIP</b>:               Draws without clipping.
	//        * <b>DT_NOFULLWIDTHCHARBREAK</b>: Prevents a line break at a DBCS (i.e. a
	//                                          double-wide character string), so that the
	//                                          line breaking rule is equivalent to SBCS
	//                                          strings. For example, this can be used in
	//                                          Korean windows, for more readability of icon
	//                                          labels. This value has no effect unless
	//                                          DT_WORDBREAK is specified.
	//        * <b>DT_NOPREFIX</b>:             Turns off processing of prefix characters.
	//        * <b>DT_PATH_ELLIPSIS</b>:        For displayed text, replaces characters
	//                                          in the middle of the string with ellipses
	//                                          so that the result fits in the specified
	//                                          rectangle. If the string contains backslash
	//                                          (\\) characters, DT_PATH_ELLIPSIS preserves
	//                                          as much as possible of the text after the
	//                                          last backslash. The string is not modified
	//                                          unless the DT_MODIFYSTRING flag is specified.
	//        * <b>DT_PREFIXONLY</b>:           Draws only an underline at the position of
	//                                          the character following the ampersand (&)
	//                                          prefix character. Does not draw any other
	//                                          characters in the string.
	//        * <b>DT_RIGHT</b>:                Aligns text to the right.
	//        * <b>DT_RTLREADING</b>:           Layout in right-to-left reading order for
	//                                          bi-directional text when the font selected
	//                                          into the hdc is a Hebrew or Arabic font.
	//                                          The default reading order for all text is
	//                                          left-to-right.
	//        * <b>DT_SINGLELINE</b>:           Displays text on a single line only.
	//                                          Carriage returns and line feeds do not
	//                                          break the line.
	//        * <b>DT_TABSTOP</b>:              Sets tab stops. Bits 158 (high-order byte
	//                                          of the low-order word) of the uFormat
	//                                          parameter specify the number of characters
	//                                          for each tab. The default number of
	//                                          characters per tab is eight. The DT_CALCRECT,
	//                                          DT_EXTERNALLEADING, DT_INTERNAL, DT_NOCLIP,
	//                                          and DT_NOPREFIX values cannot be used with
	//                                          the DT_TABSTOP value.
	//        * <b>DT_TOP</b>:                  Justifies the text to the top of the
	//                                          rectangle.
	//        * <b>DT_VCENTER</b>:              Centers text vertically. This value is used
	//                                          only with the DT_SINGLELINE value.
	//        * <b>DT_WORDBREAK</b>:            Breaks words. Lines are automatically broken
	//                                          between words if a word would extend past
	//                                          the edge of the rectangle specified by the
	//                                          lpRect parameter. A carriage return-line
	//                                          feed sequence also breaks the line. If this
	//                                          is not specified, then output is on one line.
	//        * <b>DT_WORD_ELLIPSIS</b>:        Truncates any word that does not fit in the
	//                                          rectangle and adds ellipses.
	//     <p/>
	//     DrawGroupCaption uses this method to draw the group caption with
	//     nFormat = DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX
	//
	//     DrawGroupItemImageWithText and DrawGroupItemImageWithTextBelow both use
	//     this method to draw the group item caption.
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the icon of a specified item.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pItem  - Pointer to the item to draw the icon of.
	//     ptIcon - Top left corner to start drawing the icon.
	//     pImage - Pointer to the icon to draw.
	//     szIcon - Size of the icon to draw.
	//-----------------------------------------------------------------------
	virtual void DrawItemImage(CDC* pDC, CXTPTaskPanelItem* pItem, CPoint ptIcon,
							   CXTPImageManagerIcon* pImage, CSize szIcon);

	// Helper functions
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Blends one color to another color by a specified amount.
	// Parameters:
	//     clrFrom - Color to start from.
	//     clrTo   - Color to blend to.
	//     nAlpha  - Amount of transparency.
	// Returns:
	//     The blended color.
	// Remarks:
	//     This function takes a blended color intensity which is
	//     (clrFrom * nAlpha + clrTo * (255 - nAlpha)) / 255
	//-----------------------------------------------------------------------
	COLORREF BlendColor(COLORREF clrFrom, COLORREF clrTo, int nAlpha);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption gripper bitmap.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pGroup    - Pointer to the group.
	//     bExpanded - TRUE to draw the bitmap when the group is expanded.
	//     bHot      - TRUE to draw the bitmap when the group is "hot"
	//                 (i.e. when the mouse cursor is positioned over the gripper).
	//     rc        - CRect object specifying the size of the area.
	// Returns:
	//     The bounding rectangle of the group caption excluding
	//     the width of the caption gripper bitmap.
	// See Also:
	//     CXTPTaskPanelGroup::SetSpecialGroup, CXTPTaskPanelGroup::IsSpecialGroup
	//-----------------------------------------------------------------------
	CRect DrawCaptionGripperBitmap(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bExpanded, BOOL bHot,
								   CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the ">>" and "<<" signs of the caption gripper when
	//     the theme of the Task Panel is set to Office 2000.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pt        - CPoint object specifying the x- and y- coordinates.
	//     clr       - COLORREF specifying the RGB color value.
	//     bExpanded - TRUE to draw the expanded gripper ">>",
	//                 FALSE to draw the retracted gripper "<<".
	//-----------------------------------------------------------------------
	void DrawCaptionGripperSigns(CDC* pDC, CPoint pt, COLORREF clr, BOOL bExpanded);

	//-------------------------------------------------------------------------
	// Summary:
	//     Loads the gripper bitmap when the theme of the Task Panel
	//     is set to either Windows XP or Classic Windows XP.
	// Remarks:
	//     If the current operating system is Windows XP and the current theme
	//     is set to either Luna Blue, Luna Olive or Luna Silver, then
	//     the gripper bitmap will be loaded from the operating system.
	//     For all other operating systems and themes, CreateGripperIcon will be
	//     used to create an emulated version of the Windows XP gripper.
	// See Also:
	//     CreateGripperIcon
	//-------------------------------------------------------------------------
	void CreateGripperBitmaps();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the gripper bitmap icons when the theme of the Task Panel
	//     is set to either Windows XP or Classic Windows XP and either
	//     the operating system is not Windows XP or the operating system
	//     is Windows XP and Luna themes are used.
	// Parameters:
	//     hIcon          - Reference to the icon handle.
	//     nIDResource    - Icon resource identifier.
	//     clrGripperBack - Background color of the gripper bitmap.
	// See Also:
	//     CreateGripperBitmaps
	//-----------------------------------------------------------------------
	void CreateGripperIcon(HICON& hIcon, UINT nIDResource, COLORREF clrGripperBack);

	//-------------------------------------------------------------------------
	// Summary:
	//     Frees the memory used by the group caption bitmaps.
	//-------------------------------------------------------------------------
	void FreeBitmaps();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is only used when drawing hot, pressed, and selected
	//     group items and scroll buttons when m_bOfficeHighlight is TRUE.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - CRect object specifying the size of the area.
	//     clrPen   - COLORREF specifying the pen RGB color value.
	//     clrBrush - COLORREF specifying the brush RGB color value.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, COLORREF clrPen, COLORREF clrBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the highlight offset of the text of a specified group item.
	// Parameters:
	//     pItem - Pointer to the group item with the text
	//             to retrieve the highlight offset of.
	// Returns:
	//     If the group item is selected, then the highlight offset is (1, 1).
	//     If the group item is not selected, then the highlight offset is (0, 0).
	//-----------------------------------------------------------------------
	virtual CPoint GetHighlightOffset(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the "drag arrow" of a specified item.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pItem  - Pointer to the item to draw the drag arrow of.
	//     rc     - CRect object specifying the size of the area.
	//     bAfter - TRUE to draw the drag arrow after the item,
	//              FALSE to draw the arrow before the item.
	// Remarks:
	//     The "drag arrow" is the line that is placed above or below an item
	//     when it is dragged. The drag arrow is drawn for all themes except
	//     xtpTaskPanelThemeVisualStudio2003. The drag arrow will be drawn
	//     above the item if dragging up or below the item if dragging down.
	// See Also:
	//     CXTPTaskPanelToolboxTheme::DrawGroupCaption
	//-----------------------------------------------------------------------
	virtual void DrawDragArrow(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rc, BOOL bAfter = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a rectangle around the focused item.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawFocusRect(CDC* pDC, CRect rc);

private:
	void PremultplyAlpha(HBITMAP& bmp);
	void ConvertToHSL(COLORREF clr, double& h, double& s, double& l);
	double ConvertHueToRGB(double temp1, double temp2, double temp3);
	COLORREF ConvertFromHSL(double h, double s, double l);
	void Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);
	void DrawScrollArrow(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, BOOL bEnabled);
	CRect MergeRect(CRect rcSpecified, CRect rcDefault);

public:
	BOOL m_bEmulateEplorerTheme;	// TRUE to use Explorer theme under WinXP and
									// Office 2000 under older OS.
	XTPTaskPanelGripper m_eGripper; // Gripper of the manager.

	BOOL m_bBoldCaption; // TRUE to display caption text in bold font,
						 // FALSE to display caption text in normal type.

	CRect m_rcGroupOuterMargins;	  // Outer margins of the group.
	CRect m_rcGroupInnerMargins;	  // Inner margins of the group.
	CRect m_rcItemOuterMargins;		  // Outer margins of the group items.
	CRect m_rcItemInnerMargins;		  // Inner margins of the group items.
	CRect m_rcControlMargins;		  // Default margins for all groups.
	int m_nGroupSpacing;			  // Amount of space placed between groups in the task panel.
	CRect m_rcImageLayoutIconPadding; // Padding around images for xtpTaskItemLayoutImages layout.
	CRect m_rcItemIconPadding;		  // Padding around images for items.
	CRect m_rcGroupIconPadding;		  // Padding around images for groups.

	BOOL m_bOfficeHighlight; // TRUE to highlight group items and scroll buttons using the Office XP
							 // style (XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_CHECKED,
							 // XPCOLOR_HIGHLIGHT_PUSHED).
	BOOL m_bInvertDragRect; // Only used when the theme is set to xtpTaskPanelThemeVisualStudio2003.
	BOOL m_bCaptionScrollButton;  // TRUE if the group caption scroll button is visible,
								  // FALSE if the group caption scroll button is hidden.
	BOOL m_bEmbossedDisabledText; // TRUE to draw disabled text embossed.

protected:
	BOOL m_bUseBitmapGrippers; // TRUE to use alpha grippers.
	HICON m_hGripperNormal;	// Icon of normal group header gripper
	HICON m_hGripperSpecial;   // Icon of special group header gripper.

	HBITMAP m_bmpGrippers[8]; // Bitmaps of alpha grippers.

	CXTPFont m_xtpFontCaption;	 // Caption font.
	CXTPFont m_xtpFontIcon;		   // Normal items font.
	CXTPFont m_xtpFontIconBold;	// Bold items font.
	CXTPFont m_xtpFontIconHot;	 // "Hot" items font.
	CXTPFont m_xtpFontIconHotBold; // Bold items font.

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntCaption, m_xtpFontCaption,
										  GetCaptionFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntIcon, m_xtpFontIcon, GetIconFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntIconBold, m_xtpFontIconBold,
										  GetIconBoldFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntIconHot, m_xtpFontIconHot,
										  GetIconHotFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntIconHotBold, m_xtpFontIconHotBold,
										  GetIconHotBoldFontHandle);

	CXTPPaintManagerColorGradient m_clrBackground; // Background color of the Task Panel.

	XTP_TASKPANEL_GROUPCOLORS m_groupNormal;  // Normal colors set.
	XTP_TASKPANEL_GROUPCOLORS m_groupSpecial; // Special colors set.

	CXTPPaintManagerColor m_clrHighlightHot; // Color of items when they are "hot." This is the
											 // color of the item on mouseover if hot tracking is
											 // enabled.
	CXTPPaintManagerColor m_clrHighlightSelected; // Color of items when they are selected. This is
												  // when the item has focus or is clicked on.
	CXTPPaintManagerColor m_clrHighlightPressed;  // Color of items when they are pressed. This is
												  // when the item is clicked on.
	CXTPPaintManagerColorGradient m_clrHighlightBorder; // Border color of items when they are
														// "hot." This is the border color of the
														// item on mouse over if hot tracking is
														// enabled.

	BOOL m_bOfficeBorder;		// TRUE to use Office borders.
	BOOL m_bLeftRoundedCorner;  // TRUE to round left corner of the group caption.
	BOOL m_bRightRoundedCorner; // TRUE to round right corner of the group caption.

	int m_nCaptionHeight;			// Caption height, in pixels.
	BOOL m_bUseStandardCaptionFont; // TRUE to use system font for group caption text.
	BOOL m_bUseStandardItemsFont;   // TRUE to use system font for item caption text.

	int m_nItemTextFormat; // Format flags of item text. See
						   // CXTPTaskPanelPaintManager::DrawItemCaption for a list of format flags.
	int m_nCaptionTextFormat; // Format flags of caption text. See
							  // CXTPTaskPanelPaintManager::DrawItemCaption for a list of format
							  // flags.

	friend class CXTPTaskPanelColorSet;
};

AFX_INLINE XTP_TASKPANEL_GROUPCOLORS* CXTPTaskPanelPaintManager::GetGroupColors(BOOL bSpecial)
{
	return bSpecial ? &m_groupSpecial : &m_groupNormal;
}

AFX_INLINE void CXTPTaskPanelPaintManager::SetTextFormat(int nTextFormat, BOOL bCaption /*= FALSE*/)
{
	if (bCaption)
		m_nCaptionTextFormat = nTextFormat;
	else
		m_nItemTextFormat = nTextFormat;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTASKPANELPAINTMANAGER_H__)
