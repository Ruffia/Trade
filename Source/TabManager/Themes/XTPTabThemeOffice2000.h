// XTPTabThemeOffice2000.h
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
#if !defined(__XTPTABTHEMEOFFICE2000_H__)
#	define __XTPTABTHEMEOFFICE2000_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabThemeOffice2000 implements an Office 2000 tab appearance.
// Remarks:
//     To use the Office 2000 appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPage XTPTabAppearanceStyle.
//
//     CXTPTabThemeOffice2000 supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>: Default color style.
//     * <b>xtpTabColorOffice2003</b>: Office 2003 color style.
//     * <b>xtpTabColorWinNative</b>: Windows XP color style.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabThemeOffice2000 : public CXTPTabPaintManagerTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeOffice2000 object.
	//-----------------------------------------------------------------------
	CXTPTabThemeOffice2000()
	{
		m_rcHeaderMargin.SetRect(2, 3, 6, 0);
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
		return xtpTabColorDefault;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the top, left, right, and bottom margins of the tab client area.
	// Returns:
	//     The top, left, right, and bottom margins of the tab client area.
	// Remarks:
	//     The margins of the tab client area are the space placed around
	//     the tab client area in the tab control.
	// Example:
	// <code>
	// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
	// // place a margin of 5 pixels around the tab buttons in the tab header.
	// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5, 5, 5, 5);
	//
	// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
	// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab
	// client area). m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4, 4, 4, 4);
	//
	// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
	// // place a margin of 2 pixels around the text and icon in the tab buttons.
	// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2, 2, 2, 2);
	//
	// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
	// // place a margin of 10 pixels around the tab client area.
	// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10, 10, 10, 10);
	// </code>
	// See Also:
	//     CXTPTabPaintManagerTheme::GetHeaderMargin
	//-----------------------------------------------------------------------
	virtual CRect GetClientMargin();

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the clip box of the tab header area.
	// Parameters:
	//     pDC         - Pointer to a CDC object.
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The clip box of the tab header area.
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderClipBox(CDC* pDC, CXTPTabManager* pTabManager);
};

class _XTP_EXT_CLASS CXTPTabPaintManager::CAppearanceSetPropertyPage : public CXTPTabThemeOffice2000
{
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEOFFICE2000_H__)
