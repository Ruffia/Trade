// XTPTabColorSetOffice2003.h
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
#if !defined(__XTPTABCOLORSETOFFICE2003_H__)
#	define __XTPTABCOLORSETOFFICE2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetOffice2003 implements an Office 2003 tab color set.
// Remarks:
//     To use the Office 2003 color set, SetColor is used to apply
//     the xtpTabColorOffice2003 XTPTabColorStyle.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetOffice2003 : public CXTPTabColorSetDefault
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the tabs.
	//     Override this method to change the colors of the color members.
	// Remarks:
	//     All of the color members are refreshed when this method is called.
	//     A different color set will be used for each luna color if used.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	//
	//     Tab buttons use CXTPTabPaintManagerColorSet::GradientFill to
	//     fill themselves. The gradient will use the color of the tab item
	//     blended with RGB(255, 255, 255) to produce a "light color".
	//     The gradient will start with the "light color" and fill to the
	//     normal color of the tab item.
	// See Also:
	//     FillStateButton
	//-----------------------------------------------------------------------
	virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

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
	// See Also:
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton, Rectangle
	//-----------------------------------------------------------------------
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a rectangle with a border.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rc     - Specifies the rectangle in logical units.
	//     nPen   - Specifies the color used to paint the rectangle.
	//     nBrush - Specifies the color used to fill the rectangle.
	// Remarks:
	//     This method is called by FillNavigateButton to fill the
	//     tab navigation buttons when luna colors are disabled.
	// See Also:
	//     FillNavigateButton
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

protected:
	CXTPPaintManagerColor m_clrNavigateButtonBorder; // Border color of tab navigation buttons
													 // on mouse over.
	BOOL m_bLunaTheme; // TRUE if the current system theme is xtpSystemThemeSilver,
					   // xtpSystemThemeOlive or xtpSystemThemeOlive, otherwise FALSE.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
//{{AFX_CODEJOCK_PRIVATE
#endif // !defined(__XTPTABCOLORSETOFFICE2003_H__)
//}}AFX_CODEJOCK_PRIVATE
