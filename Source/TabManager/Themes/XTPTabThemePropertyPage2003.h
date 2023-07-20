// XTPTabThemePropertyPage2003.h
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
#if !defined(__XTPTABTHEMEPROPERTYPAGE2003_H__)
#	define __XTPTABTHEMEPROPERTYPAGE2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//-------------------------------------------------------------------------
// Summary:
//     CAppearanceSetPropertyPage2003 implements an Office 2003 tab appearance.
// Remarks:
//     To use the Office 2003 appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPage2003 XTPTabAppearanceStyle.
//
//     CAppearanceSetPropertyPage2003 supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>: Default color style.
//     * <b>xtpTabColorOffice2003</b>: Office 2003 color style.
//     * <b>xtpTabColorVisualStudio2005</b>: Visual Studio 2005 color style.
//
//     By default, m_bDoubleHighlightedBorder is FALSE.
//     If m_bDoubleHighlightedBorder is TRUE, then a double border is
//     drawn on mouse over when hot tracking is enabled.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CAppearanceSetPropertyPage2003 : public CXTPTabPaintManagerTheme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CAppearanceSetPropertyPage2003 object.
	//-------------------------------------------------------------------------
	CAppearanceSetPropertyPage2003()
	{
		m_rcHeaderMargin.SetRect(2, 3, 6, 0);
		m_bDoubleHighlightedBorder = FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the top, left, right, and bottom margins of the tab header.
	// Returns:
	//     The top, left, right, and bottom margins of the tab header.
	// Remarks:
	//     The margins of the tab header are the space placed around
	//     the tab buttons inside the tab header.
	//
	//     The margins of the tab header are not placed around each
	//     tab button. Instead, they are placed around all tab buttons.
	// See Also:
	//     CXTPTabPaintManager::GetHeaderRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderMargin()
	{
		return CRect(m_rcHeaderMargin.left + m_nButtonHeight, m_rcHeaderMargin.top,
					 m_rcHeaderMargin.right, m_rcHeaderMargin.bottom);
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the appearance metrics.
	//     Override this member to change the appearance metrics.
	// Remarks:
	//     This method is used to refresh the appearance metrics
	//     such as font, button height, margin size, etc.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	//     If m_bDoubleHighlightedBorder is TRUE, then a double border is
	//     drawn on mouse over when hot tracking is enabled.
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bDoubleHighlightedBorder
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
		return xtpTabColorOffice2003;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the bounding rectangle of a specified drawing item.
	// Parameters:
	//     pItem - Pointer to the drawing item with
	//             the bounding rectangle to retrieve.
	// Returns:
	//     The bounding rectangle of the specified drawing item.
	// See Also:
	//     CXTPTabManagerItem::GetRect
	//-----------------------------------------------------------------------
	virtual CRect GetButtonDrawRect(const CXTPTabManagerItem* pItem);

public:
	BOOL m_bDoubleHighlightedBorder; // TRUE to draw a double border on mouse over when hot tracking
									 // is enabled.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEPROPERTYPAGE2003_H__)
