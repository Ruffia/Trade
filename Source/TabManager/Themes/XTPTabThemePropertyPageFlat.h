// XTPTabThemePropertyPageFlat.h
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
#if !defined(__XTPTABTHEMEPROPERTYPAGEFLAT_H__)
#	define __XTPTABTHEMEPROPERTYPAGEFLAT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CAppearanceSetPropertyPageFlat implements an Office 2000 Flat tab appearance.
// Remarks:
//     To use the Office 2000 Flat appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPageFlat XTPTabAppearanceStyle.
//
//     CAppearanceSetPropertyPageSelected supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>: Default color style.
//     * <b>xtpTabColorVisualStudio2003</b>: Visual Studio color style.
//     * <b>xtpTabColorOffice2003</b>: Office 2003 color style.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CAppearanceSetPropertyPageFlat : public CXTPTabThemeFlat
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CAppearanceSetPropertyPageFlat object.
	// Remarks:
	//     By default, m_bBlurPoints is TRUE to smooth tab corners.
	//-----------------------------------------------------------------------
	CAppearanceSetPropertyPageFlat()
	{
		m_bBlurPoints			 = TRUE;
		m_bVisualStudio2005Style = FALSE;
		m_bDrawNormalTab		 = TRUE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single tab button in the tab client header area.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab button to draw.
	// Remarks:
	//     This method is only used to draw the tab button. This method does not
	//     draw the button's icon or text.
	//
	//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     after the button is drawn to draw the button's icon and text.
	//
	//     If m_bBlurPoints is TRUE, then tab corners will be smoothed.
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bBlurPoints
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default value of the color set for this theme.
	// Returns:
	//     The default value of the color set for this theme.
	// See Also:
	//     GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet()
	{
		return xtpTabColorDefault;
	}

	void BlurPoint(CDC* pDC, int x, int y, COLORREF clr, int nAlpha);
	void BlurTab(CDC* pDC, CRect rcTab, COLORREF clrBackground, COLORREF clrBorder,
				 XTPTabPosition tabPosition);

public:
	BOOL m_bBlurPoints;			   // TRUE to smooth corners of flat tabs.
	BOOL m_bVisualStudio2005Style; // TRUE to draw with Visual Studio 2005 style.
	BOOL m_bDrawNormalTab;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEPROPERTYPAGEFLAT_H__)
