// XTPTabPaintManager.h: interface for the CXTPTabPaintManager class.
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
#if !defined(__XTPTABPAINTMANAGER_H__)
#	define __XTPTABPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabManager;
class CXTPTabManagerAtom;
class CXTPTabManagerItem;
class CXTPTabManagerNavigateButton;
class CXTPWinThemeWrapper;
class CXTPTabPaintManager;
class CXTPTabPaintManagerColorSet;
class CXTPTabPaintManagerTheme;

//===========================================================================
// Summary:
//     CXTPTabPaintManager is used to store the color and appearance information
//     of the tabs in the tab manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabPaintManager : public CXTPCmdTarget
{
public:
	class CAppearanceSetPropertyPage;
	class CAppearanceSetFlat;
	class CColorSetDefault;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager(BOOL bPanelManager = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Applies one of the "built-in" color sets for the tabs.
	// Parameters:
	//     tabColor - "Built-in" color set to be applied; must be one of
	//                the values defined by the XTPTabColorStyle enumeration.
	// Returns:
	//     A pointer to the newly applied "built-in" color set.
	// Remarks:
	//     Use SetColor to apply a "built-in" color set.
	//     Use SetColorSet to apply a custom color set.
	//
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, SetColorSet, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManagerColorSet* SetColor(XTPTabColorStyle tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a custom color set for the tabs.
	// Parameters:
	//     pColorSet - Pointer to the custom color set to be applied.
	// Returns:
	//     A pointer to the newly applied custom color set.
	// Remarks:
	//     Use SetColor to apply a "built-in" color set.
	//     Use SetColorSet to apply a custom color set.
	//
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManagerColorSet* SetColorSet(CXTPTabPaintManagerColorSet* pColorSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies one of the "built-in" appearance sets for the tabs.
	// Parameters:
	//     tabAppearance - "Built-in" appearance set to be applied; must be one of
	//                     the values defined by the XTPTabAppearanceStyle enumeration.
	// Returns:
	//     A pointer to the newly applied "built-in" appearance set.
	// Remarks:
	//     Use SetAppearance to apply a "built-in" appearance.
	//     Use SetAppearanceSet to apply a custom appearance.
	//
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	// See Also:
	//     XTPTabAppearanceStyle, SetAppearanceSet, GetAppearance, GetAppearanceSet,
	//     SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManagerTheme* SetAppearance(XTPTabAppearanceStyle tabAppearance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a custom appearance set for the tabs.
	// Parameters:
	//     pAppearanceSet - Pointer to the custom appearance set to be applied.
	// Remarks:
	//     Use SetAppearance to apply a "built-in" appearance.
	//     Use SetAppearanceSet to apply a custom appearance.
	//
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	//
	//     SetColorSet is used to set custom colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it for the tabs.
	// Returns:
	//     A pointer to the newly applied custom appearance set.
	// See Also:
	//     XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//     SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManagerTheme* SetAppearanceSet(CXTPTabPaintManagerTheme* pAppearanceSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied custom appearance set.
	// Returns:
	//     A pointer to the currently applied custom appearance set.
	// Remarks:
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	//
	//     SetColorSet is used to set custom colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it for the tabs.
	// See Also:
	//     GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* GetAppearanceSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied custom color set.
	// Returns:
	//     A pointer to the currently applied custom color set.
	// Remarks:
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, GetColor, SetColorSet, SetColor
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* GetColorSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied "built-in appearance set.
	// Returns:
	//     The currently applied "built-in" appearance set; one of
	//     the values defined by the XTPTabAppearanceStyle enumeration.
	// Remarks:
	//     An appearance set specifies how tabs will look. This is
	//     the actual physical structure of the tab buttons. For example,
	//     tab buttons with an Excel appearance set will have a half-hexagon shape.
	// See Also:
	//     XTPTabAppearanceStyle, GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	XTPTabAppearanceStyle GetAppearance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently applied "built-in" color set.
	// Returns:
	//     The currently applied "built-in" color set.
	// Remarks:
	//     Color sets are used to colorize the tabs. For example, applying
	//     the xtpTabColorWinNative color set will give the tabs a Windows XP colorization.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	XTPTabColorStyle GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if luna colors are disabled/enabled.
	// Returns:
	//     TRUE if luna colors are disabled, FALSE is luna colors are enabled.
	// Remarks:
	//     Luna colors are the Windows XP colors (Silver, Blue, Olive).
	//     CXTPDrawHelpers::GetCurrentSystemTheme can be called to get
	//     the currently used system theme.
	// See Also:
	//     DisableLunaColors, XTPCurrentSystemTheme,
	//     CXTPDrawHelpers::GetCurrentSystemTheme
	//-----------------------------------------------------------------------
	BOOL IsLunaColorsDisabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Disables/enables luna colors.
	// Parameters:
	//     bDisableLunaColors - TRUE to disable luna colors,
	//                          FALSE to enable luna colors.
	// Remarks:
	//     Luna colors are the Windows XP colors (Silver, Blue, Olive).
	//     CXTPDrawHelpers::GetCurrentSystemTheme can be called to get
	//     the currently used system theme.
	// See Also:
	//     IsLunaColorsDisabled, XTPCurrentSystemTheme,
	//     CXTPDrawHelpers::GetCurrentSystemTheme
	//-----------------------------------------------------------------------
	void DisableLunaColors(BOOL bDisableLunaColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the RGB value of a specified OneNote color.
	// Parameters:
	//     tabColor - XTPTabOneNoteColor flag indicating which OneNote color to retrieve
	//                the RGB value of. See remarks section for available flags.
	// Remarks:
	//     The tab manager calls this member function to retrieve the RGB
	//     color for the value specified by <i>tabColor</i>. <i>tabColor</i>
	//     can be one of the following values:
	//     * <b>xtpTabColorBlue</b>:    Blue tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorYellow</b>:  Yellow tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorGreen</b>:   Green tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorRed</b>:     Red tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorPurple</b>:  Purple tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorCyan</b>:    Cyan tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorOrange</b>:  Orange tab color used when OneNote
	//                                  colors are enabled.
	//     * <b>xtpTabColorMagenta</b>: Magenta tab color used when OneNote
	//                                  colors are enabled.
	// Returns:
	//     The RGB value of the specified OneNote color.
	// Example:
	//     See WM_XTP_GETTABCOLOR for an example.
	// See Also:
	//     XTPTabOneNoteColor
	//-----------------------------------------------------------------------
	static COLORREF AFX_CDECL GetOneNoteColor(XTPTabOneNoteColor tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables OneNote colorization for the tabs.
	// Parameters:
	//     bOneNoteColors - TRUE to enable OneNote colorization,
	//                      FALSE to disable OneNote colorization.
	// Remarks:
	//     OneNote colors are similar to the tab colorization for
	//     tabs in Microsoft OneNote.
	//-----------------------------------------------------------------------
	void SetOneNoteColors(BOOL bOneNoteColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab position.
	// Parameters:
	//     tabPosition - Tab position to be set; must be one of the
	//                   values defined by the XTPTabPosition enumeration.
	// Remarks:
	//     The tab position refers to where the tab buttons are located.
	//     The tab buttons can be positioned on either the top, bottom,
	//     left, or right side of the tab client area.
	// See Also:
	//     XTPTabPosition, GetPosition
	//-----------------------------------------------------------------------
	void SetPosition(XTPTabPosition tabPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab layout style.
	// Parameters:
	//     tabLayout - Tab layout style to be set; must be one of the
	//                 values defined by the XTPTabLayoutStyle enumeration.
	// Remarks:
	//     The tab layout style refers to how the tab buttons are sized
	//     within the tab client header. The tab buttons can be in either
	//     a compressed, fixed, auto-size, or size-to-fit layout.
	// See Also:
	//     XTPTabPosition, GetLayout
	//-----------------------------------------------------------------------
	void SetLayout(XTPTabLayoutStyle tabLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab position.
	// Returns:
	//     The tab position; one of the values defined by
	//     the XTPTabPosition enumeration.
	// Remarks:
	//     The tab position refers to where the tab buttons are located.
	//     The tab buttons can be positioned on either the top, bottom,
	//     left, or right side of the tab client area.
	// See Also:
	//     XTPTabPosition, SetPosition
	//-----------------------------------------------------------------------
	XTPTabPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab layout style.
	// Returns:
	//     The tab layout style; one of the values defined by
	//     the XTPTabLayoutStyle enumeration.
	// Remarks:
	//     The tab layout style refers to how the tab buttons are sized
	//     within the tab client header. The tab buttons can be in either
	//     a compressed, fixed, auto-size, or size-to-fit layout.
	// See Also:
	//     XTPTabLayoutStyle, SetLayoutStyle
	//-----------------------------------------------------------------------
	XTPTabLayoutStyle GetLayout() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font for the tabs.
	// Parameters:
	//     pLogFont         - Font to be set.
	//     bUseStandardFont - The value of this parameter should only be TRUE when
	//                        this method is called internally. Otherwise the value
	//                        of this parameter should always be FALSE to indicate
	//                        that pLogFont should be used as the font for the tabs.
	// Remarks:
	//     When bUseStandardFont is TRUE, the standard system "icon" font
	//     will be used (i.e. Tahoma 8pt).
	//-----------------------------------------------------------------------
	void SetFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     This method is called during various Draw and Paint functions.
	//
	//     This method will call CXTPTabPaintManagerTheme::DrawTabControl
	//     for the currently set appearance. CXTPTabPaintManagerTheme::DrawTabControl
	//     will then call CXTPTabPaintManager::DrawTabControlEx.
	//-----------------------------------------------------------------------
	virtual void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the tab client rectangle.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to adjust the tab client rectangle.
	//     rcClient    - [out] Reference to a CRect object to receive
	//                   the new tab client rectangle with margins included.
	// Remarks:
	//     Both AdjustClientRect and RepositionTabControl are called
	//     when the window is resized. When DrawTabControl is called,
	//     the changes will be used when drawing the tab control.
	//
	//     This method will adjust the tab client rectangle of
	//     the entire tab manager if IsDrawStaticFrame is TRUE
	//     in order to make room to draw the static frame.
	//
	//     CXTPTabPaintManagerTheme::AdjustClientRect will be called if
	//     there is one or more tabs in the tab manager.
	// See Also:
	//     CXTPTabPaintManagerTheme::AdjustClientRect
	//-----------------------------------------------------------------------
	virtual void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the tab buttons of the tab control to their correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab buttons are on.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Client rectangle of the tab control.
	// Remarks:
	//     Both AdjustClientRect and RepositionTabControl are called
	//     when the window is resized. When DrawTabControl is called,
	//     the changes will be used when drawing the tab control.
	//
	//     CXTPTabPaintManagerTheme::RepositionTabControl will be called
	//     for the currently set appearance.
	//-----------------------------------------------------------------------
	virtual void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions the tab navigation buttons.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab navigation buttons are on.
	//     rcClient    - Client rectangle of the tab control.
	// Returns:
	//     The bounding rectangle of the tab header (including header margins).
	// Remarks:
	//     CXTPTabPaintManager::RepositionTabControlEx calls this method
	//     to reposition the tab navigation buttons.
	//-----------------------------------------------------------------------
	CRect RepositionNavigateButtons(CXTPTabManager* pTabManager, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab navigation button.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to the tab navigation button to fill.
	//     rc      - Reference to the bounding rectangle of the tab navigation button.
	// Remarks:
	//     This method calls m_pColorSet->FillNavigateButton.
	//
	//     The XTPTabColorStyle CXTPTabPaintManagerColorSet classes override
	//     this method to perform actions such as painting
	//     the highlighted, pressed, and normal versions of tab navigation buttons.
	// See Also:
	//     CXTPTabPaintManagerColorSet::FillNavigateButton,
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a specified tab navigation button.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to the tab navigation button to draw.
	//     rc      - Reference to the bounding rectangle of the tab navigation button.
	// See Also:
	//     CXTPTabPaintManagerColorSet::FillNavigateButton,
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void DrawNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the text and icon of a specified tab button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pItem  - Pointer to the tab button to draw the text and icon of.
	//     rcItem - Bounding rectangle of the tab.
	//     bDraw  - TRUE to draw the tab button's text and icon,
	//              FALSE to only calculate the button width (no drawing is performed).
	// Returns:
	//     If bDraw is TRUE, then a value of 0 is returned.
	//     If bDraw is FALSE, then the button width including both
	//     the left and right button margins is returned.
	// Remarks:
	//     This method is only used to draw the text and icon of a tab button.
	//     This method does not draw the tab button.
	//
	//     CXTPTabPaintManagerTheme::DrawSingleButton calls
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText after
	//     the button is drawn to draw the button's icon and text.
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManagerTheme::DrawSingleButton
	//-----------------------------------------------------------------------
	virtual int DrawSingleButtonIconAndText(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem,
											BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a specified tab is visible.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab is on.
	//     pItem       - Pointer to the tab to ensure the visibility of.
	// See Also:
	//     CXTPTabManager::SetHeaderOffset, CXTPTabManager::GetHeaderOffset
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTabManager* pTabManager, CXTPTabManagerItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the main tab manager.
	// Remarks:
	//     This method checks if m_bUseStandardFont is TRUE.
	//     If m_bUseStandardFont is TRUE, then this method updates
	//     the font with the standard system "icon" font (i.e. Tahoma 8pt).
	//
	//     This method calls RefreshMetrics for the currently used
	//     CXTPTabPaintManagerColorSet and CXTPTabPaintManagerTheme.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a control to the list of controls that receives a notification
	//     when a property of the paint manager is changed.
	// Parameters:
	//     pObserver - Pointer to the control to be added to the list of controls.
	// Remarks:
	//     This method is not used in the MFC version.
	//     This method is only used in the ActiveX version and does nothing in MFC.
	//-----------------------------------------------------------------------
	void AddObserver(CXTPTabManagerAtom* pObserver);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the behaviour for tooltips.
	// Parameters:
	//     behaviour - Tooltips behaviour to be set.
	//                 See the Remarks section for available flags.
	// Remarks:
	//     The <i>behaviour</i> parameter can be one of the following:
	//     * <b>xtpTabToolTipNever</b>: Never show tooltips for tabs.
	//     * <b>xtpTabToolTipAlways</b>: Always show tooltips for tabs.
	//     * <b>xtpTabToolTipShrinkedOnly</b>: Only show tooltips if the tab was shrinked
	//                                         (see xtpTabLayoutSizeToFit layout).
	//-----------------------------------------------------------------------
	void EnableToolTips(XTPTabToolTipBehaviour behaviour = xtpTabToolTipAlways);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the font used to display caption text in the tab buttons.
	// Parameters:
	//     bVertical - TRUE if XTPTabPosition is vertical,
	//                 FALSE if XTPTabPosition is horizontal.
	// Returns:
	//     The font used to display caption text in the tab buttons.
	// See Also:
	//     GetBoldFont
	//-----------------------------------------------------------------------
	CFont* GetFont(BOOL bVertical = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the font used to display bold caption text in the tab buttons.
	// Parameters:
	//     bVertical - TRUE if XTPTabPosition is vertical,
	//                 FALSE if XTPTabPosition is horizontal.
	// Returns:
	//     The font used to display bold caption text in the tab buttons.
	// Remarks:
	//     Bold font is used to display caption text when a tab
	//     is selected and m_bBoldSelected = TRUE.
	//
	//     If m_bBoldNormal = TRUE, then tabs will use bold font
	//     even when they are not selected.
	// See Also:
	//     m_bBoldNormal, m_bBoldSelected, GetFont
	//-----------------------------------------------------------------------
	CFont* GetBoldFont(BOOL bVertical = FALSE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the tab button size when
	//     XTPTabLayoutStyle is set to xtpTabLayoutSizeToFit.
	// Parameters:
	//     pTabManager - Pointer to the tab manager.
	//     nWidth      - Total width that all tab buttons must fit within.
	// Remarks:
	//     When calculating nWidth, the width of both the left and right
	//     rcHeaderMargin and the width of the tab navigation button
	//     should be subtracted from the width of the tab client.
	// See Also:
	//     XTPTabLayoutStyle, SetLayout, GetLayout
	//-----------------------------------------------------------------------
	void SizeToFit(CXTPTabManager* pTabManager, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     DrawTabControl calls CXTPTabPaintManagerTheme::DrawTabControl
	//     which will then call DrawTabControlEx if it has not been overridden.
	// See Also:
	//     CXTPTabPaintManagerTheme::DrawTabControl
	//-----------------------------------------------------------------------
	void DrawTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the tab buttons of the tab control to their correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab buttons are on.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Client rectangle of the tab control.
	// Remarks:
	//     Both AdjustClientRect and RepositionTabControl are called
	//     when the window is resized. When DrawTabControl is called,
	//     the changes will be used when drawing the tab control.
	//
	//     RepositionTabControl calls CXTPTabPaintManagerTheme::RepositionTabControl
	//     which will then call RepositionTabControlEx if it has not been overridden.
	// See Also:
	//     CXTPTabPaintManagerTheme::RepositionTabControl
	//-----------------------------------------------------------------------
	void RepositionTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the tab buttons of the multi-row tab control to their correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab buttons are on.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Client rectangle of the tab control.
	//-----------------------------------------------------------------------
	void RepositionTabControlMultiRow(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when a property of the tab paint manager is changed.
	//-------------------------------------------------------------------------
	virtual void OnPropertyChanged();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total button length of a tab button including
	//     the left and right button margins.
	// Parameters:
	//     nValue - Width of the tab button.
	//     nMin   - Minimum width of the tab button.
	//     nMax   - Maximum width of the tab button.
	// Returns:
	//     The total button length of a tab button including
	//     the left and right button margins.
	// See Also:
	//     CXTPTabManager::GetItemMetrics
	//-----------------------------------------------------------------------
	int _GetButtonLength(int nValue, int nMin, int nMax);

	// -----------------------------------------------------------------------
	// Summary:
	//     Draws an ellipse "..." in the middle of a specified string.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pManager - Pointer to the tab manager to draw.
	//     strItem  - Reference to the string item to draw the ellipse in.
	//     rcItem   - Bounding rectangle of the tab button. This is the rectangle
	//                (in logical coordinates) in which the text is to be formatted.
	//     uFormat  - See nFormat parameter of CDC\:\:DrawText.
	// -----------------------------------------------------------------------
	void DrawTextPathEllipsis(CDC* pDC, CXTPTabManager* pManager, const CString& strItem,
							  CRect rcItem, UINT uFormat);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	int GetPrefixTextExtent(CDC* pDC, CXTPTabManager* pManager, const CString& str, CRect rcItem,
							UINT uFormat);
	void StripMnemonics(CString& strClear);
	void DrawRowItems(CXTPTabManager* pTabManager, CDC* pDC, const CRect& rcClient, int nItemRow);
	void CreateMultiRowIndexer(CXTPTabManager* pTabManager, CDC* pDC, int nWidth);
	virtual void DrawFocusRect(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem);
	BOOL _CreateMultiRowIndexerBestFit(CXTPTabManager* pTabManager, int nWidth, int nRow,
									   int nTotalLength);
	BOOL _CreateMultiRowIndexerPlain(CXTPTabManager* pTabManager, int nWidth, int nRow,
									 int nTotalLength);

	int _ReduceNumberOfRowsForPlainIndexer(CXTPTabManager* pTabManager, int nRowCount, int nWidth,
										   int nTotalLength); // returns nRowCount or a lesser value
															  // when plain indexer can produce
															  // extra rows

	static int AFX_CDECL _SizeToFitCompare(const void* arg1, const void* arg2);

	//}}AFX_CODEJOCK_PRIVATE

public:
	BOOL m_bHotTracking;  // If TRUE, then tab hot tracking will be enabled.
	BOOL m_bShowIcons;	// If TRUE, then tab icons will be drawn if icons were added.
	BOOL m_bBoldSelected; // If TRUE, then the text of selected tabs will be displayed in bold font.
	BOOL m_bBoldNormal;   // If TRUE, then bold font will be used for all tabs regardless of
						  // whether they are selected or non-selected.
	BOOL m_bDisableLunaColors;		 // If FALSE, then tabs will not use luna colors if
									 // the current theme uses luna colors.
	BOOL m_bOneNoteColors;			 // If TRUE, then tabs will use OneNote colorization.
	XTPTabClientFrame m_clientFrame; // Frame style of the tab client area.
	BOOL m_bStaticFrame; // If TRUE, then a static frame will be drawn around the entire tab area.
	CRect m_rcClientMargin;		 // Margin around the tab client area.
	CRect m_rcControlMargin;	 // Margin around the entire tab control
								 // (i.e. tab client header and tab client area).
	CRect m_rcButtonMargin;		 // Margin around the text in tab button.
	CRect m_rcButtonTextPadding; // Margin around the text in tab button.
	BOOL m_bInvertGradient;		 // If TRUE, then the colors passed into
								 // CXTPTabPaintManagerColorSet::GradientFill will be swapped before
								 // the rectangle is filled with the gradient.
	BOOL m_bFillBackground;		 // If TRUE, then the tab client space is filled using
								 // CXTPTabPaintManager.CXTPTabPaintManagerColorSet.m_clrControlFace
	// color. This is only FALSE when drawing docking pane panel auto-hide
	// tabs because there is no client area to draw.
	CSize m_szIcon;				  // Size of the icon displayed in the tab button.
	BOOL m_bDrawTextPathEllipsis; // If TRUE, when the size of a tab button is too small to display
								  // the caption, the middle of the caption will be replaced with
								  // an ellipse "...". If FALSE, then the end of the caption will be
								  // replaced with the ellipse.
	BOOL m_bDrawTextEndEllipsis;  // If TRUE, when the size of a tab button is too small to display
								  // the caption, the end of the caption will be replaced with
								  // an ellipse "...". If FALSE, then the end of the caption will be
								  // replaced with the ellipse.
	BOOL m_bDrawTextNoPrefix; // If FALSE, then an ampersand '&' in the tab button caption will be
							  // ignored and a double ampersand '&&' will display a single ampersand
							  // '&'. If TRUE, then the caption will be displayed unmodified.
	UINT m_nDrawTextFormat;   // Text format for tabs (default - DT_LEFT | DT_VCENTER).
	BOOL m_bDrawTextHidePrefix; // If FALSE, then an ampersand '&' in the tab button caption will be
								// ignored and a double ampersand '&&' will display a single
								// ampersand '&'. If TRUE, then the caption will be hidden.
	BOOL m_bMultiRowFixedSelection; // FALSE to move row with selected item close to client pane.
	BOOL m_bMultiRowJustified;  // TRUE to stretch each row of tabs to fill the entire width of the
								// control.
	BOOL m_bDrawFocusRect;		// TRUE to draw focus rect for focused item.
	int m_bSelectOnDragOver;	// 1 - to activate tab when user drag files/text under it.
								// 2 - to activate with small delay.
	int m_nButtonExtraLength;   // Additional padding of tabs.
	BOOL m_bSelectOnButtonDown; // If TRUE, then tab selected on mouse button down.
	BOOL m_bClearTypeTextQuality;	  // TRUE to enable ClearType text for the font.
	BOOL m_bRotateImageOnVerticalDraw; // TRUE to rotate image when tab located left or right.
	BOOL m_bClipHeader;				   // TRUE to select clip rectangle before draw tabs.
	CSize m_szNavigateButton;		   // Navigate buttons size.
	CSize m_szTabCloseButton;		   // Tab close button size.
	BOOL m_bShowTabs;				   // TRUE to show tabs.
	BOOL m_bMultiRowTabsOptimization;  // TRUE if the tab manager will arrange tabs
									   // according to their widths.
									   // TRUE by default. Otherwise plain ordering.

	int m_nMinTabWidth;						// Minimum tabs width.
	int m_nMaxTabWidth;						// Maximum tabs width.
	int m_nFixedTabWidth;					// Tabs width for fixed layout.
	XTPTabToolTipBehaviour m_toolBehaviour; // Tootips behaviour.
	BOOL m_bVerticalTextTopToBottom;		// Draw vertical text Top-to-Bottom or Bottom-to-Top.
	BOOL m_bPanelManager; // TRUE indicates that this is a panel manager, FALSE for tab manager.

	static DWORD m_nSelectOnDragOverDelay; // Delay before Select-on-Drag-Over (300 by default).

protected:
	CXTPTabPaintManagerTheme* m_pAppearanceSet; // Currently set CXTPTabPaintManagerTheme.
	CXTPTabPaintManagerColorSet* m_pColorSet;   // Currently used CXTPTabPaintManagerColorSet.
	CArray<CXTPTabManagerAtom*, CXTPTabManagerAtom*> m_arrObservers; // List of controls that
																	 // receive a notification
																	 // when a property of the
																	 // paint manager has changed.
	XTPTabAppearanceStyle m_tabAppearance; // Currently set appearance style.
	XTPTabColorStyle m_tabColor;		   // Currently used color style.
	XTPTabPosition m_tabPosition;  // Currently set tab button position. The position refers to
								   // where the tab buttons are located. The tab buttons can be
								   // positioned on the top, bottom, left, or right side of the
								   // tab client area.
	XTPTabLayoutStyle m_tabLayout; // Currently set tab button layout. The layout refers to how the
								   // tab buttons are sized within the tab client header.

	CXTPFont m_xtpFontNormal;		  // Font used to display text in tab buttons.
	CXTPFont m_xtpFontBold;			  // Font used to display bold text in the tab buttons
									  // (i.e. when a tab is selected and m_bBoldSelected = TRUE).
	CXTPFont m_xtpFontVerticalNormal; // Font used to display tab button caption when
									  // the tab position is vertical.
	CXTPFont m_xtpFontVerticalBold;   // Font used to display bold text in the tab buttons when
									  // the tab position is vertical
									  // (i.e. when a tab is selected and m_bBoldSelected = TRUE).

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntNormal, m_xtpFontNormal, GetNormalFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntBold, m_xtpFontBold, GetBoldFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntVerticalNormal, m_xtpFontVerticalNormal,
										  GetVerticalNormalFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntVerticalBold, m_xtpFontVerticalBold,
										  GetVerticalBoldFontHandle);

	BOOL m_bUseStandardFont; // TRUE to use standard system "icon" font (i.e. Tahoma 8pt).

private:
	int m_nItemColor;

	friend class CXTPTabPaintManagerTheme;
	friend class CXTPTabPaintManagerColorSet;
	friend class CXTPTabManager;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
public:
	afx_msg BOOL OleIsColorSupported(long nColor);
	afx_msg LPDISPATCH OleGetClientMargin();
	afx_msg LPDISPATCH OleGetControlMargin();
	afx_msg LPDISPATCH OleGetHeaderMargin();
	afx_msg LPDISPATCH OleGetButtonMargin();
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg LPFONTDISP OleGetFont();
	afx_msg LPDISPATCH OleGetColorSet();
	afx_msg void OleSetIconSize(long cx, long cy);
	afx_msg void OleRefreshMetrics();
	afx_msg void OnClearTypeTextQualityChanged();
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTabPaintManager);

	friend class CXTPTabPaintManagerMargin;
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CFont* CXTPTabPaintManager::GetFont(BOOL bVertical)
{
	return bVertical ? &m_xtpFontVerticalNormal : &m_xtpFontNormal;
}

AFX_INLINE CFont* CXTPTabPaintManager::GetBoldFont(BOOL bVertical)
{
	return bVertical ? &m_xtpFontVerticalBold : &m_xtpFontBold;
}

AFX_INLINE CXTPTabPaintManagerTheme* CXTPTabPaintManager::GetAppearanceSet() const
{
	return m_pAppearanceSet;
}

AFX_INLINE CXTPTabPaintManagerColorSet* CXTPTabPaintManager::GetColorSet() const
{
	return m_pColorSet;
}

AFX_INLINE void CXTPTabPaintManager::EnableToolTips(XTPTabToolTipBehaviour behaviour)
{
	m_toolBehaviour = behaviour;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABPAINTMANAGER_H__)
