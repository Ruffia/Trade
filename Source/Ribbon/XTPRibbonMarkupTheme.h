// XTPRibbonMarkupTheme.h: interface for the CXTPRibbonMarkupTheme class.
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
#if !defined(__XTPRIBBONMARKUPTHEME_H__)
#	define __XTPRIBBONMARKUPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupContext;
class CXTPRibbonTabContextHeader;

enum XTPRibbonThemePart
{
	xtpRibbonFrameCaptionActive,
	xtpRibbonFrameCaptionInactive,
	xtpRibbonFrameSystemButtonNormal,
	xtpRibbonFrameSystemButtonPopuped,
	xtpRibbonFrameSystemButtonSelected,
	xtpRibbonGroupControlEntry,
	xtpRibbonGroupControlEntryLeft,
	xtpRibbonGroupControlEntryLeftRight,
	xtpRibbonGroupControlEntryRight,
	xtpRibbonGroupRect,
	xtpRibbonGroupRectBlue,
	xtpRibbonGroupRectCyan,
	xtpRibbonGroupRectGreen,
	xtpRibbonGroupRectOrange,
	xtpRibbonGroupRectPurple,
	xtpRibbonGroupRectRed,
	xtpRibbonGroupRectYellow,
	xtpRibbonScrollButtonLeft,
	xtpRibbonScrollButtonRight,
	xtpRibbonTabHighlighted,
	xtpRibbonTabSelected,
	xtpRibbonTabSelectedFocused,
	xtpRibbonTabSelectedHighlighted,
	xtpRibbonContextTabBlue,
	xtpRibbonContextTabCyan,
	xtpRibbonContextTabGreen,
	xtpRibbonContextTabOrange,
	xtpRibbonContextTabPurple,
	xtpRibbonContextTabRed,
	xtpRibbonContextTabYellow,
	xtpRibbonTabSelectedBlue,
	xtpRibbonTabSelectedFocusedBlue,
	xtpRibbonTabSelectedHighlightedBlue,
	xtpRibbonTabSelectedCyan,
	xtpRibbonTabSelectedFocusedCyan,
	xtpRibbonTabSelectedHighlightedCyan,
	xtpRibbonTabSelectedGreen,
	xtpRibbonTabSelectedFocusedGreen,
	xtpRibbonTabSelectedHighlightedGreen,
	xtpRibbonTabSelectedOrange,
	xtpRibbonTabSelectedFocusedOrange,
	xtpRibbonTabSelectedHighlightedOrange,
	xtpRibbonTabSelectedPurple,
	xtpRibbonTabSelectedFocusedPurple,
	xtpRibbonTabSelectedHighlightedPurple,
	xtpRibbonTabSelectedRed,
	xtpRibbonTabSelectedFocusedRed,
	xtpRibbonTabSelectedHighlightedRed,
	xtpRibbonTabSelectedYellow,
	xtpRibbonTabSelectedFocusedYellow,
	xtpRibbonTabSelectedHighlightedYellow,
	xtpRibbonFrameCaptionBackstageLeft,
	xtpRibbonFrameCaptionBackstageRight,
	xtpRibbonThemePartCount
};

class _XTP_EXT_CLASS CXTPRibbonMarkupThemePartGroup : public CXTPMarkupThemePart
{
public:
	void SetCaption(LPCTSTR pszCaption);

	void SetTextColor(COLORREF clrText);

	void SetFontSize(int nFontSize);

	void SetFontFamily(LPCTSTR pszFontFamily);
};

class _XTP_EXT_CLASS CXTPRibbonMarkupThemePartSystemMenu : public CXTPMarkupThemePart
{
public:
	void SetBackgroundColor(COLORREF clrBackground);

	void SetForegroundColor(COLORREF clrForeground);

	void SetCaption(LPCTSTR pszCaption);

	void SetFontSize(int nFontSize);

	void SetFontFamily(LPCTSTR pszFontFamily);
};

//===========================================================================
// Summary:
//     CXTPRibbonMarkupTheme class.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonMarkupTheme : public CXTPRibbonPaintManager
{
	DECLARE_DYNAMIC(CXTPRibbonMarkupTheme)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonMarkupTheme object.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonMarkupTheme(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonMarkupTheme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonMarkupTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts from the specified ZIP source.
	// Parameters:
	//     hZip           - ZIP source to load markup parts from.
	//     pMarkupContext - Associated markup context.
	//     bReload        - Causes markup parts to be re-loaded if TRUE.
	//-----------------------------------------------------------------------
	void LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts.
	// Parameters:
	//     bReload - Re-loads previously loaded parts if TRUE.
	//-----------------------------------------------------------------------
	virtual void LoadParts(BOOL bReload = FALSE) = 0;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the background of a ribbon bar control.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     pRibbonBar - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	void FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called to refresh the visual metrics of the manager.
	// Remarks:
	//      Refreshes all of the colors in the ribbon bar. Many of the colors
	//      used are set with the GROUP_COLOR structure.
	// See Also:
	//     GROUP_COLOR
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the background of a CXTPRibbonGroup.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pActiveTab - Pointer to the tab that is currently active.
	//     rcGroups   - Bounding rectangle of the group.
	// Remarks:
	//     Performs a gradient fill using the GROUP_COLOR::clrBackground for
	//     the main group background and GROUP_COLOR::clrCaption to fill the
	//     group caption rectangle.
	// See Also:
	//     CXTPRibbonGroup::GetRect, GROUP_COLOR
	//-----------------------------------------------------------------------
	void FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the specified CXTPRibbonGroup.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the CXTPRibbonGroup to be drawn.
	// Remarks:
	//     Draws the entire group including background, caption, and text.
	//     This will not draw the option button or the group pop-up.
	// See Also:
	//     GROUP_COLOR, FillRibbonBar, FillGroupRect, RefreshMetrics
	//-----------------------------------------------------------------------
	void DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Renders a Markup string in the Ribbon.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Bounding rectangle for the Markup.
	//     lpszText - Markup string to render.
	//-----------------------------------------------------------------------
	void RenderMarkup(CDC* pDC, CRect rc, LPCTSTR lpszText);
	void RenderMarkup(CDC* pDC, CRect rc,
					  XTPRibbonThemePart
						  part); // <combine CXTPRibbonMarkupTheme::RenderMarkup@CDC*@CRect@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group option button.
	// Parameters:
	//     pDC                 - Pointer to a valid device context.
	//     pControlGroupOption - Pointer to a CXTPControl object.
	//     bDraw               - TRUE to draw option button,
	//                           FALSE to retrieve the size of the control.
	// Remarks:
	//     The option button is typically used to launch a dialog associated with the group.
	//     It is drawn on the right side of the group caption.
	// Returns:
	//     If bDraw is FALSE, then the size of the option button control
	//     will be returned.
	// See Also:
	//     CXTPRibbonGroup::ShowOptionButton, CXTPRibbonGroup::GetControlGroupOption
	//-----------------------------------------------------------------------
	CSize DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the ribbon group option size.
	// Returns:
	//     The ribbon group option size.
	//-----------------------------------------------------------------------
	CSize GetControlGroupOptionSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group pop-up button.
	// Parameters:
	//     pDC                - Pointer to a valid device context
	//     pControlGroupPopup - Pointer to a CXTPControlPopup object.
	//     bDraw              - TRUE to draw the group pop-up button,
	//                          FALSE to retrieve the size of the control.
	// Remarks:
	//     A single group button will be displayed if there is not enough room
	//     to display the items in the group. A group pop-up is displayed
	//     when the button is clicked. The pop-up contains all the items in
	//     the group. When a group button is clicked, a CXTPControlPopup
	//     is displayed that contains the items of the group.
	// Returns:
	//     If bDraw is FALSE, then the size of the group pop-up button control
	//     will be returned.
	// See Also:
	//     CXTPRibbonGroup::GetControlGroupPopup
	//-----------------------------------------------------------------------
	CSize DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up toolbar of a reduced group.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     pGroup      - Parent group of pop-up'ed toolbar.
	//     pCommandBar - Pop-up toolbar to draw.
	//-----------------------------------------------------------------------
	void FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup, CXTPCommandBar* pCommandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Quick Access customize button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - Pointer to the Quick Access customize button.
	// See Also:
	//     DrawRibbonQuickAccessMoreButton
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Quick Access more button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - Pointer to the Quick Access more button.
	// See Also:
	//     DrawRibbonQuickAccessButton
	//-----------------------------------------------------------------------
	void DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up bar of the more button.
	// Parameters:
	//      pDC  - Pointer to a valid device context.
	//      pBar - Toolbar to fill.
	//-----------------------------------------------------------------------
	void FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single tab of the ribbon bar.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Ribbon tab to draw.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up bar of the system button.
	// Parameters:
	//      pDC  - Pointer to a valid device context.
	//      pBar - Toolbar to fill.
	//-----------------------------------------------------------------------
	void FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a pop-up bar button.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pop-up bar button to draw.
	//-----------------------------------------------------------------------
	void DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scroll group buttons.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     pControl    - Scroll button to draw.
	//     bScrollLeft - TRUE to draw the left button, FALSE to draw the right.
	//-----------------------------------------------------------------------
	void DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl, BOOL bScrollLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the top-left system button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - System button control.
	//     rc       - Bounding rectangle of the control.
	//-----------------------------------------------------------------------
	void DrawRibbonFrameSystemButton(CDC* pDC, CXTPRibbonControlSystemButton* pControl, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon group when the group is
	//     minimized and only a button is displayed. When the button is
	//     pressed, the group is displayed as a pop-up.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Button to draw.
	//-----------------------------------------------------------------------
	BOOL DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon group pop-up toolbar.
	// Parameters:
	//     pDC          - Pointer to a valid device context.
	//     pRibbonGroup - Group to draw.
	//-----------------------------------------------------------------------
	void DrawRibbonGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pRibbonGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon caption if EnableFrameTheme
	//     was not called.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//	   pRibbonBar - Pointer to the ribbon bar to draw the caption for.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Ribbon caption.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//	   pRibbonBar - Pointer to the ribbon bar to draw the caption for.
	//     bActive    - TRUE if the ribbon frame is active.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon Quick Access controls.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pRibbonBar - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessControls(CDC* pDC, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon context headers.
	// Parameters:
	//     pDC             - Pointer to a valid device context.
	//     pContextHeaders - Collection of context headers.
	//     pRibbonBar      - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameContextHeaders(CDC* pDC, CXTPRibbonBar* pRibbonBar,
											   CXTPRibbonTabContextHeaders* pContextHeaders);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member fills in the tab header and tab client area. It
	//     will then draw tab client frame.
	// Parameters:
	//     pRibbonBar  - Pointer to the ribbon bar to draw the tab control for.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab control.
	//-----------------------------------------------------------------------
	virtual void FillRibbonTabControl(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rcControl);

protected:
	virtual CXTPMarkupUIElement* GetTabContextHeader(CXTPRibbonTabContextHeader* pHeader);

protected:
	COLORREF m_clrControlGroupOption;
	COLORREF m_clrGroupCaption;
	COLORREF m_clrQuickAccess;
	COLORREF m_clrQuickAccessDisabled;
	COLORREF m_clrQuickAccessBelow;
	COLORREF m_clrQuickAccessButtonHotBack;
	COLORREF m_clrQuickAccessButtonPressedBack;
	COLORREF m_clrBackstageSysButtonBack2013Normal; // The color of "back" button in backstage if
													// the button is not selected.
	COLORREF m_clrBackstageSysButtonBack2013Highlighted; // The color of "back" button in backstage
														 // if the button is selected.

	CString m_strHighlightedPrefix; // The prefix of the file name for the backstage "back" button
									// image used for the selected button.
	CString m_strNormalPrefix; // The prefix of the file name for the backstage "back" button image
							   // used for the not selected button.

	BOOL m_bLoaded;

	CXTPMarkupContext* m_pMarkupContext; // Store pointer to a CXTPMarkupContext object.

	CXTPMarkupThemePart m_parts[xtpRibbonThemePartCount];

	CXTPMarkupThemePart m_systemPopupBarEntry;
	CXTPMarkupThemePart m_systemPopupBarButtonNormal;
	CXTPMarkupThemePart m_systemPopupBarButtonSelected;

	CXTPRibbonMarkupThemePartSystemMenu* m_pSystemMenuNormal;
	CXTPRibbonMarkupThemePartSystemMenu* m_pSystemMenuSelected;
	CXTPRibbonMarkupThemePartSystemMenu* m_pSystemMenuPopuped;

	CXTPRibbonMarkupThemePartGroup* m_pGroupNormal;
	CXTPRibbonMarkupThemePartGroup* m_pGroupHighlighted;

	CXTPRibbonMarkupThemePartGroup* m_pGroupPopupNormal;
	CXTPRibbonMarkupThemePartGroup* m_pGroupPopupPopuped;
	CXTPRibbonMarkupThemePartGroup* m_pGroupPopupSelected;

	CXTPRibbonMarkupThemePartGroup* m_pGroupPopupToolBar;

	CXTPMarkupThemePart* m_pBackstageBackButton;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONMARKUPTHEME_H__)
