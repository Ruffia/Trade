// XTPTabThemeVisualStudio2003.h
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
#if !defined(__XTPTABTHEMEVISUALSTUDIO2003_H__)
#	define __XTPTABTHEMEVISUALSTUDIO2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabThemeVisualStudio2003 implements a Visual Studio 2003 tab appearance.
// Remarks:
//     To use the Visual Studio appearance, SetAppearance is used to apply
//     the xtpTabAppearanceVisualStudio2003 XTPTabAppearanceStyle.
//
//     CXTPTabThemeVisualStudio2003 supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>: Default color style.
//     * <b>xtpTabColorVisualStudio2003</b>: Visual Studio color style.
//     * <b>xtpTabColorOffice2003</b>: Office 2003 color style.
//     * <b>xtpTabColorWinNative</b>: Windows XP color style.
//     * <b>xtpTabColorVisualStudio2005</b>: Visual Studio 2005 color style.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabThemeVisualStudio2003 : public CXTPTabPaintManagerTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeVisualStudio2003 object.
	//-----------------------------------------------------------------------
	CXTPTabThemeVisualStudio2003()
	{
		m_rcHeaderMargin.SetRect(4, 2, 4, 0);
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
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills both the tab header and the tab client area and then
	//     draws the tab client frame.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to fill.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	// Returns:
	//     The bounding rectangle of the tab header.
	// Remarks:
	//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client space
	//     is filled using CXTPTabPaintManagerColorSet::m_clrControlFace.
	//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
	//     docking pane panel auto-hide tabs because there is no client area to draw.
	// See Also:
	//     CXTPTabPaintManagerTheme::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground,
	//     XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
	//-----------------------------------------------------------------------
	virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

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
		return xtpTabColorVisualStudio2003;
	}
};

#	define CAppearanceSetVisualStudio CXTPTabThemeVisualStudio2003

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEVISUALSTUDIO2003_H__)
