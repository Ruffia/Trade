// XTPTabColorSetVisualStudio2012.h
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
#if !defined(__XTPTABCOLORSETVISUALSTUDIO2012_H__)
#	define __XTPTABCOLORSETVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetVisualStudio2012 implements a Visual Studio 2012 tab color set.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetVisualStudio2012 : public CXTPTabPaintManagerColorSet
{
public:
	CXTPTabColorSetVisualStudio2012();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the tabs.
	//     Override this method to change the colors of the color members.
	// Remarks:
	//     All of the color members are refreshed when this method is called.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtains client background color.
	// Parameters:
	//     pTabManager - Pointer to the tab manager with the tab client area to fill.
	// Returns:
	//     Client area background color value.
	//-----------------------------------------------------------------------
	virtual COLORREF GetClientColor(CXTPTabManager* pTabManager);

protected:
	CXTPPaintManagerColor m_clrNonActiveButtonNormal;
	CXTPPaintManagerColor m_clrNonActiveButtonPressed;
	CXTPPaintManagerColor m_clrNonActiveButtonHighlighted;
	CXTPPaintManagerColor m_clrNonActiveButtonHighlightedClose;
	CXTPPaintManagerColor m_clrNonActiveButtonSelected;
	CXTPPaintManagerColor m_clrNonActiveButtonSelectedPressed;
	CXTPPaintManagerColor m_clrNonActiveButtonSelectedHighlighted;
	CXTPPaintManagerColor m_clrNonActiveButtonBorderHot;
	CXTPPaintManagerColor m_clrNonActiveText;
	CXTPPaintManagerColor m_clrNonActiveTextPressed;
	CXTPPaintManagerColor m_clrNonActiveTextHighlighted;
	CXTPPaintManagerColor m_clrNonActiveTextHighlightedClose;
	CXTPPaintManagerColor m_clrNonActiveTextSelected;
	CXTPPaintManagerColor m_clrNonActiveTextSelectedPressed;
	CXTPPaintManagerColor m_clrNonActiveTextSelectedHighlighted;
	CXTPPaintManagerColor m_clrNonActiveActiveFilesGlyph;
	CXTPPaintManagerColor m_clrNonActiveActiveFilesGlyphPressed;
	CXTPPaintManagerColor m_clrNonActiveActiveFilesGlyphHighlighted;
	CXTPPaintManagerColor m_clrNonActiveActiveFilesBack;
	CXTPPaintManagerColor m_clrNonActiveActiveFilesBackPressed;
	CXTPPaintManagerColor m_clrNonActiveActiveFilesBackHighlighted;
	CXTPPaintManagerColor m_clrNonActiveFrameBorder;
	CXTPPaintManagerColor m_clrNonActiveButtonBorder;
	CXTPPaintManagerColor m_clrNonActiveAutoHideFace;
	CXTPPaintManagerColor m_clrNonActiveHeaderFace;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCOLORSETVISUALSTUDIO2012_H__)
