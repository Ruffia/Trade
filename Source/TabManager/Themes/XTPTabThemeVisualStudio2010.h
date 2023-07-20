// XTPTabThemeVisualStudio2010.h
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
#if !defined(__XTPTABTHEMEVISUALSTUDIO2010_H__)
#	define __XTPTABTHEMEVISUALSTUDIO2010_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabThemeVisualStudio2010 implements a Visual Studio 2010 tab appearance.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabThemeVisualStudio2010 : public CAppearanceSetPropertyPageFlat
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeVisualStudio2010 object.
	// Remarks:
	//     By default, m_bBlurPoints is TRUE to smooth tab corners.
	//-----------------------------------------------------------------------
	CXTPTabThemeVisualStudio2010()
	{
		m_bDrawNormalTab = FALSE;
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
		return xtpTabColorVisualStudio2010;
	}

public:
};

#	define CAppearanceSetVisualStudio2010 CXTPTabThemeVisualStudio2010 // Old name.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEVISUALSTUDIO2010_H__)
