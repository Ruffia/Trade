// XTPTabColorSetVisualStudio2008.h
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
#if !defined(__XTPTABCOLORSETVISUALSTUDIO2008_H__)
#	define __XTPTABCOLORSETVISUALSTUDIO2008_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetVisualStudio2008 implements a Visual Studio 2008 tab color set.
// Remarks:
//     To use the Visual Studio 2008 color set, SetColor is used to apply
//     the xtpTabColorVisualStudio2008 XTPTabColorStyle.
//
//     The following appearances (XTPTabAppearanceStyle) support CColorSetVisualStudio2008:
//     * <b>xtpTabAppearancePropertyPage2003</b>: Office 2003 appearance.
//     * <b>xtpTabAppearanceStateButtons</b>: State Button appearance.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetVisualStudio2008 : public CXTPTabColorSetOffice2003
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabColorSetVisualStudio2008 object.
	//-----------------------------------------------------------------------
	CXTPTabColorSetVisualStudio2008();

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

protected:
	BOOL m_bGradientButton; // TRUE if luna colors are enabled. This will be TRUE when using
							// Windows XP and the current system theme is xtpSystemThemeSilver,
							// xtpSystemThemeOlive or xtpSystemThemeOlive, otherwise FALSE.
};

#	define CColorSetVisualStudio2008 CXTPTabColorSetVisualStudio2008

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCOLORSETVISUALSTUDIO2008_H__)
