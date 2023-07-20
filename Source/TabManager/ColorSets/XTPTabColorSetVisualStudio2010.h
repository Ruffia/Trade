// XTPTabColorSetVisualStudio2010.h
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
#if !defined(__XTPTABCOLORSETVISUALSTUDIO2010_H__)
#	define __XTPTABCOLORSETVISUALSTUDIO2010_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetVisualStudio2010 implements a Visual Studio 2010 tab color set.
// Remarks:
//     To use the Visual Studio 2010 color set, SetColor is used to apply
//     the xtpTabColorVisualStudio2010 XTPTabColorStyle.
//
//     The following appearances (XTPTabAppearanceStyle) support CColorSetVisualStudio2010:
//     * <b>xtpTabAppearancePropertyPage2003</b>: Office 2003 appearance.
//     * <b>xtpTabAppearanceStateButtons</b>: State Button appearance.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetVisualStudio2010 : public CXTPTabColorSetVisualStudio2008
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabColorSetVisualStudio2010 object.
	//-----------------------------------------------------------------------
	CXTPTabColorSetVisualStudio2010(BOOL bPaneColor = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the tabs.
	//     Override this method to change the colors of the color members.
	// Remarks:
	//     All of the color members are refreshed when this method is called.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

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
	//     m_bGradientButton
	//-----------------------------------------------------------------------
	COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

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
	void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text color for a specified tab.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab with the text color to be set.
	//-----------------------------------------------------------------------
	void SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem);

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

protected:
	BOOL m_bGradientButton; // TRUE if luna colors are enabled. This will be TRUE when using
							// Windows XP and the current system theme is xtpSystemThemeSilver,
							// xtpSystemThemeOlive or xtpSystemThemeOlive, otherwise FALSE.
	BOOL m_bPaneColor;

	CXTPBrush m_xtpBrushHeader;
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushHeader, m_xtpBrushHeader,
										  GetHeaderBrushHandle);
};

#	define CColorSetVisualStudio2010 CXTPTabColorSetVisualStudio2010

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCOLORSETVISUALSTUDIO2010_H__)
