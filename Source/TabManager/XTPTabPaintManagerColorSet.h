// XTPTabPaintManagerColorSet.h
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
#if !defined(__XTPTABPAINTMANAGERCOLORSET_H__)
#	define __XTPTABPAINTMANAGERCOLORSET_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabManagerItem;
class CXTPTabManager;
class CXTPTabManagerNavigateButton;
class CXTPTabPaintManager;

//===========================================================================
// Summary:
//     CXTPTabPaintManagerColorSet represents the colors for all of the
//     components of the tab manager (e.g. tab button, tab button border, etc.).
// Remarks:
//     CXTPTabPaintManagerColorSet colorizes the different parts that make up
//     a tab, tab header, tab client, and tab manager control.
//
//     Colors should be updated by using the RefreshMetrics method for each
//     CXTPTabPaintManagerColorSet object.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabPaintManagerColorSet
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for Excel tab button colorization.
	// Remarks:
	//     Members of COLORSET_TAB_EXCEL are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabAppearanceExcel2003.
	//
	//     Both the m_csExcelSelected and m_csExcelNormal color sets are
	//     used when the xtpTabAppearanceExcel2003 appearance is applied.
	//
	//     By default, the "selected" excel style tab will display
	//     clrLeftInnerBorder and clrTopInnerBorder in addition to
	//     the outer borders to indicate that the tab is selected.
	//
	//     The following color sets customize members of COLORSET_TAB_EXCEL
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b>: Office 2003 Color Set.
	//     * <b>CXTPTabColorSetDefault</b>: Default Color Set.
	// See Also:
	//     m_csExcelSelected, m_csExcelNormal
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_EXCEL
	{
		CXTPPaintManagerColor clrLeftInnerBorder; // Color of the left inner border of the tab
												  // button.
		CXTPPaintManagerColor clrTopInnerBorder; // Color of the top inner border of the tab button.
		CXTPPaintManagerColor clrRightInnerBorder; // Color of the right inner border of the tab
												   // button.
		CXTPPaintManagerColor clrLeftOuterBorder;  // Color of the left outer border of the tab
												   // button.
		CXTPPaintManagerColor clrRightOuterBorder; // Color of the right outer border of the tab
												   // button.
		CXTPPaintManagerColor clrTopOuterBorder; // Color of the top outer border of the tab button.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for Office 2000 tab button colorization.
	// Remarks:
	//     The tab buttons and the tab client area cast a shadow. The shadow consists
	//     of both a normal and a dark color. One or both of these colors can be used
	//     depending on the selected XTPTabAppearanceStyle.
	//
	//     Members of COLORSET_TAB_PROPERTYPAGE are only used when the
	//     XTPTabAppearanceStyle is set to either xtpTabAppearancePropertyPage,
	//     xtpTabAppearancePropertyPageFlat, xtpTabAppearancePropertyPageSelected,
	//     or xtpTabAppearancePropertyPage2003.
	//
	//     The following color sets customize members of COLORSET_TAB_PROPERTYPAGE
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b>: Office 2003 Color Set.
	//     * <b>CColorSetWinNative</b>: Windows XP Color Set.
	//     * <b>CXTPTabColorSetDefault</b>:  Default Color Set.
	// See Also:
	//     m_csPropertyPage
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_PROPERTYPAGE
	{
		CXTPPaintManagerColor clrHighlight;  // Color of tab button on mouse over when hot tracking
											 // is enabled.
		CXTPPaintManagerColor clrShadow;	 // Color of the tab button's shadow.
		CXTPPaintManagerColor clrDarkShadow; // Color of the tab button's dark shadow.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for Office 2003 tab button colorization.
	// Remarks:
	//     Members of COLORSET_TAB_PROPERTYPAGE2003 are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabAppearancePropertyPage2003.
	//
	//     The following color sets customize members of COLORSET_TAB_PROPERTYPAGE2003
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b>: Office 2003 Color Set.
	//     * <b>CColorSetVisualStudio2005</b>: Visual Studio 2005 Color Set.
	//     * <b>CXTPTabColorSetDefault</b>: Default Color Set.
	// See Also:
	//     m_csPropertyPage2003
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_PROPERTYPAGE2003
	{
		CXTPPaintManagerColor clrDarkShadow;		  // Color of the shadow of a tab button.
		CXTPPaintManagerColor clrFrameBorder;		  // Color of the XTPTabClientFrame.
		CXTPPaintManagerColor clrBorderLeftHighlight; // Color of the left border of a tab button.
		CXTPPaintManagerColor clrBorderTopHighlight;  // Color of the top border of a tab button.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for "State Button" tab button colorization.
	// Remarks:
	//     Members of COLORSET_TAB_STATEBUTTON are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabAppearanceStateButtons.
	//
	//     The following color sets customize members of COLORSET_TAB_STATEBUTTON
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b>: Office 2003 Color Set.
	//     * <b>CXTPTabColorSetDefault</b>: Default Color Set.
	// See Also:
	//     m_csStateButton
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_STATEBUTTON
	{
		CXTPPaintManagerColorGradient clrFace;   // Color of the currently selected tab button.
		CXTPPaintManagerColorGradient clrBorder; // Border color of the currently selected tab
												 // button.
	};

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManagerColorSet object.
	//-------------------------------------------------------------------------
	CXTPTabPaintManagerColorSet();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManagerColorSet object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabPaintManagerColorSet();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the tabs.
	//     Override this method to change the colors of the color members.
	// Remarks:
	//     All of the color members are refreshed when this method is called.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text color for the tab.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab to set the text color of.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the header of the tab client of a specified tab manager.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Bounding rectangle of the tab header.
	//     pTabManager - Pointer to the tab manager with the header to fill.
	// Remarks:
	//     The tab client header is the bounding rectangle containing the tab buttons.
	//     The bounding rectangle can be retrieved by using the
	//     CXTPTabPaintManagerTheme::GetHeaderRect and
	//     CXTPTabManager::GetHeaderRect methods.
	// See Also:
	//     CXTPTabPaintManagerTheme::GetHeaderRect, CXTPTabManager::GetHeaderRect
	//-----------------------------------------------------------------------
	virtual void FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the tab client area of a specified tab manager.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Bounding rectangle of the tab client area.
	//     pTabManager - Pointer to the tab manager with the tab client area to fill.
	// Remarks:
	//     The tab client bounding rectangle can be retrieved by using the
	//     CXTPTabPaintManagerTheme::GetClientRect and
	//     CXTPTabManager::GetClientRect members.
	// Returns:
	//     The fill color value.
	//-----------------------------------------------------------------------
	virtual COLORREF FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains client background color.
	// Parameters:
	//     pTabManager - Pointer to the tab manager with the tab client area to fill.
	// Returns:
	//     Client area background color value.
	//-----------------------------------------------------------------------
	virtual COLORREF GetClientColor(CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab button when XTPTabAppearanceStyle
	//     is xtpTabAppearanceStateButtons.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Pointer to the tab button to fill.
	// Remarks:
	//     This method is only used to fill buttons when XTPTabAppearanceStyle
	//     is xtpTabAppearanceStateButtons. Otherwise, FillPropertyButton is used.
	// See Also:
	//     FillPropertyButton
	//-----------------------------------------------------------------------
	virtual void FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Pointer to the tab button to fill.
	// Returns:
	//     The color of the specified tab button.
	// Remarks:
	//     This method is used to fill all tab buttons except when
	//     XTPTabAppearanceStyle is set to xtpTabAppearanceStateButtons.
	//     xtpTabAppearanceStateButtons requires the FillStateButton member.
	// See Also:
	//     FillStateButton
	//-----------------------------------------------------------------------
	virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a clipping region for the specified tab client area.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Bounding rectangle of the tab client area. This will
	//                   be the clipping region for the device context.
	//     pTabManager - Pointer to the tab manager.
	//-----------------------------------------------------------------------
	virtual void SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab navigation button.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to the tab navigation button to fill.
	//     rc      - Reference to the bounding rectangle of the tab navigation button.
	// Remarks:
	//     This method is used to fill the tab navigation buttons in the header of
	//     the tab client and is overridden by XTPTabColorStyle and
	//     CXTPTabPaintManagerColorSet to perform actions such as painting
	//     the highlighted, pressed, and normal versions of tab navigation buttons.
	//
	//     This method must be overridden.
	// See Also:
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the color of a specified tab.
	// Parameters:
	//     pItem - Pointer to the tab with the color to retrieve.
	// Returns:
	//     The color of the tab.
	// Remarks:
	//     This method is used within many of
	//     CXTPTabPaintManagerColorSet's "fill" methods.
	// See Also:
	//     CXTPTabManager::GetItemColor, XTPTabColorStyle,
	//     SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemColor(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs a gradient fill of a rectangle with a color that
	//     smoothly fades from top to bottom.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Rectangle to fill.
	//     clrLight - COLORREF to start the gradient from.
	//     clrDark  - COLORREF to end the gradient with.
	//     position - Position of the tab buttons; one of the values
	//                defined by the XTPTabPosition enumeration.
	// Remarks:
	//     The color smoothly fades from clrLight at the top to clrDark at the bottom.
	//
	//     If clrLight is XTP_TABMANAGER_COLOR_SHADED, then the rectangle will
	//     be filled with a special "shaded" fill type and clrDark will
	//     be ignored. See the description of XTP_TABMANAGER_COLOR_SHADED for
	//     more information on this fill type.
	// See Also:
	//     XTP_TABMANAGER_COLOR_SHADED
	//-----------------------------------------------------------------------
	void GradientFill(CDC* pDC, CRect rc, COLORREF clrLight, COLORREF clrDark,
					  XTPTabPosition position);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Windows XP themes are enabled and if
	//     a Windows XP theme is currently used.
	// Returns:
	//     Always returns FALSE in the base class implementation.
	// Remarks:
	//     This method is overridden in CXTPTabPaintManager::CColorSetWinNative.
	// See Also:
	//     CXTPTabPaintManager::CColorSetWinNative::IsAppThemed
	//-----------------------------------------------------------------------
	virtual BOOL IsAppThemed() const;

public:
	CXTPPaintManagerColorGradient m_clrHeaderFace;   // Color set of the tab header background.
	CXTPPaintManagerColorGradient m_clrAutoHideFace; // Color set of the auto-hide panel background.
													 // This is the tab header for docking pane tabs
													 // when the pane is hidden.
	CXTPPaintManagerColor m_clrFrameBorder; // Color of the border placed around the tab buttons and
											// the tab client.
	CXTPPaintManagerColor m_clrControlFace; // Color of the control foreground. This is the area
											// that the tab header and tab client are drawn on.
	CXTPPaintManagerColor m_clrPaneFace;	// Color of the control pane foreground.
	CXTPPaintManagerColor m_clrBorderHighlight; // Color of the highlight border.
	CXTPPaintManagerColor m_clrTabClientBack;   // Background color for the tab client area.
	CXTPPaintManagerColor m_clrTabClientBorder; // Border color for the tab client area.

	CXTPPaintManagerColor m_clrButtonNormal;  // Color of normal tabs. Normal tabs are tabs that are
											  // not selected or disabled.
	CXTPPaintManagerColor m_clrButtonPressed; // Tab pressed button color.
	CXTPPaintManagerColor m_clrButtonHighlighted;	  // Tab button color on mouse over when hot
													   // tracking is enabled.
	CXTPPaintManagerColor m_clrButtonHighlightedClose; // Tab close button color on mouse over when
													   // hot tracking is enabled.

	CXTPPaintManagerColor m_clrButtonBorderHighlighted; // Tab button border color on mouse over
														// when hot tracking is enabled.
	CXTPPaintManagerColor m_clrButtonBorderTabContext1; // Tab button border color with tab context.
	CXTPPaintManagerColor m_clrButtonBorderTabContext2; // Tab button border color with tab context.
	CXTPPaintManagerColor m_clrButtonBorderKeyTips;		// Tab button border color when ALT button
														// is pressed

	CXTPPaintManagerColor m_clrButtonSelected;			  // Color of the currently selected tab.
	CXTPPaintManagerColor m_clrButtonSelectedPressed;	 // Color of the currently selected tab
														  // when pressed.
	CXTPPaintManagerColor m_clrButtonSelectedHighlighted; // Color of the currently selected tab
														  // when highlighted.
	CXTPPaintManagerColor m_clrButtonBorder;			  // Tab button border (edge) color.
	CXTPPaintManagerColor m_clrButtonBorderHot; // Tab button highlight or pressed border (edge)
												// color.

	CXTPPaintManagerColor m_clrTextNormal;	// Color of text in a normal tab. Normal tabs are tabs
											  // that are not selected or disabled.
	CXTPPaintManagerColor m_clrTextPressed;   // Color of text in a "pressed" tab button. The
											  // "pressed" tab button is the tab with focus
											  // (i.e. the tab becomes active when it is clicked).
	CXTPPaintManagerColor m_clrTextHighlight; // Color of text on mouse over when hot tracking is
											  // enabled.
	CXTPPaintManagerColor m_clrTextHighlightClose; // Color of close button text on mouse over when
												   // hot tracking is enabled.
	CXTPPaintManagerColor m_clrTextSelected;	   // Color of text in a "selected" tab button. The
												   // "selected" tab button is the tab with focus
	// (i.e. the tab becomes active when it is clicked).
	CXTPPaintManagerColor m_clrTextSelectedPressed; // Color of text in a "pressed" tab button. The
													// "pressed" tab button is the tab with focus
													// (i.e. The tab becomes active when it is
													// clicked).
	CXTPPaintManagerColor m_clrTextSelectedHighlighted; // Color of text for tabs in all non-active
														// groups of tabs in MDITabClient.
	CXTPPaintManagerColor m_clrTextDisabled; // Color of text when a tab is disabled. A disabled
											 // tab cannot receive focus.

	CXTPPaintManagerColor m_clrActiveFilesGlyph;			// Color of navigation button.
	CXTPPaintManagerColor m_clrActiveFilesGlyphHighlighted; // Color of highlighted navigation
															// button.
	CXTPPaintManagerColor m_clrActiveFilesGlyphPressed;		// Color of selected navigation button.
	CXTPPaintManagerColor m_clrActiveFilesBack;			   // Color of highlighted navigation button
														   // background.
	CXTPPaintManagerColor m_clrActiveFilesBackHighlighted; // Color of navigation button background.
	CXTPPaintManagerColor m_clrActiveFilesBackPressed;	 // Color of selected navigation button
														   // background.

	COLORSET_TAB_STATEBUTTON m_csStateButton;			// Color set of state button style tabs.
	COLORSET_TAB_PROPERTYPAGE m_csPropertyPage;			// Color set of visual studio style tabs.
	COLORSET_TAB_PROPERTYPAGE2003 m_csPropertyPage2003; // Color set of Office 2003 style tabs.
	COLORSET_TAB_EXCEL m_csExcelSelected; // Color set of selected (focused) Excel style tabs.
	COLORSET_TAB_EXCEL m_csExcelNormal;   // Color set of normal Excel style tabs.
	CXTPPaintManagerColor m_clrFrameHighlightBorder;

	//{{AFX_CODEJOCK_PRIVATE
	__declspec(property(get = GetTextSelected,
						put = SetTextSelected)) CXTPPaintManagerColor& m_clrSelectedText;

	_XTP_DEPRECATED_IN_FAVOR(m_clrTextSelected)
	CXTPPaintManagerColor& GetTextSelected();

	_XTP_DEPRECATED_IN_FAVOR(m_clrTextSelected)
	const CXTPPaintManagerColor& GetTextSelected() const;

	_XTP_DEPRECATED_IN_FAVOR(m_clrTextSelected)
	CXTPPaintManagerColor& SetTextSelected(const CXTPPaintManagerColor& clr);

	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPTabPaintManager* m_pPaintManager; // Self paint manager pointer.

	friend class CXTPTabPaintManager;
};

AFX_INLINE void CXTPTabPaintManagerColorSet::SelectClipRgn(CDC* /*pDC*/, CRect /*rcClient*/,
														   CXTPTabManager* /*pTabManager*/)
{
}

AFX_INLINE CXTPPaintManagerColor& CXTPTabPaintManagerColorSet::GetTextSelected()
{
	return m_clrTextSelected;
}

AFX_INLINE const CXTPPaintManagerColor& CXTPTabPaintManagerColorSet::GetTextSelected() const
{
	return m_clrTextSelected;
}

AFX_INLINE CXTPPaintManagerColor&
	CXTPTabPaintManagerColorSet::SetTextSelected(const CXTPPaintManagerColor& clr)
{
	m_clrTextSelected = clr;
	return m_clrTextSelected;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABPAINTMANAGERCOLORSET_H__)
