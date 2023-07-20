// XTPTabThemeVisio.h
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
#if !defined(__XTPTABTHEMEVISIO_H__)
#	define __XTPTABTHEMEVISIO_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CAppearanceSetVisio implements a Visio tab appearance.
// Remarks:
//     To use the Visio appearance, SetAppearance is used to apply
//     the xtpTabAppearanceVisio XTPTabAppearanceStyle.
//
//     CAppearanceSetVisio supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>: Default color style.
//     * <b>xtpTabColorOffice2003</b>: Office 2003 color style.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CAppearanceSetVisio : public CXTPTabPaintManagerTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the tab client rectangle.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to adjust the tab client rectangle.
	//     rcClient    - [out] Reference to a CRect object to receive
	//                   the new tab client rectangle with margins included.
	// Remarks:
	//     Both CXTPTabPaintManager::AdjustClientRect and
	//     CXTPTabPaintManager::RepositionTabControl are called when the
	//     window is resized. When CXTPTabPaintManager::DrawTabControl
	//     is called, the changes will be used when drawing the tab control.
	//
	//     This method will adjust the tab client rectangle based on
	//     the currently set XTPTabClientFrame.
	//
	//     CXTPTabPaintManager::AdjustClientRect calls this method if
	//     there is one or more tabs in the tab manager.
	// See Also:
	//     CXTPTabPaintManager::AdjustClientRect
	//-----------------------------------------------------------------------
	void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the tab buttons of the tab control to their correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab buttons are on.
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Bounding rectangle of the tab client area.
	// Remarks:
	//     Both CXTPTabPaintManager::AdjustClientRect and
	//     CXTPTabPaintManager::RepositionTabControl are called when the
	//     window is resized. When CXTPTabPaintManager::DrawTabControl
	//     is called, the changes will be used when drawing the tab control.
	//
	//     This method is called during CXTPTabPaintManager::RepositionTabControl.
	//-----------------------------------------------------------------------
	void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab header area.
	// Parameters:
	//     rcControl   - Bounding rectangle of the tab header.
	//                   See CXTPTabManager::GetControlRect.
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The bounding rectangle of the tab header area.
	// See Also:
	//     GetClientRect, CXTPTabManager::GetControlRect,
	//     CXTPTabManager::CXTPTabPaintManagerTheme::GetHeaderMargin,
	//     CXTPTabManager::CXTPTabPaintManagerTheme::GetClientMargin
	//-----------------------------------------------------------------------
	CRect GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab client area.
	// Parameters:
	//     rcControl   - Bounding rectangle of the tab control.
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The bounding rectangle of the tab client area.
	// See Also:
	//     GetClientRect, CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	CRect GetClientRect(CRect rcControl, CXTPTabManager* pTabManager);

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
	void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

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
	//     This method is called by CXTPTabPaintManager::DrawTabControl
	//     when the tab control must be drawn.
	//-----------------------------------------------------------------------
	void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEVISIO_H__)
