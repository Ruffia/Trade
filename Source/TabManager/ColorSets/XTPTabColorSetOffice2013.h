// XTPTabColorSetOffice2013.h
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
#if !defined(__XTPTABCOLORSETOFFICE2013_H__)
#	define __XTPTABCOLORSETOFFICE2013_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetOffice2013 implements an Office 2013 tab color set.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetOffice2013 : public CXTPTabColorSetVisualStudio2012
{
public:
	CXTPTabColorSetOffice2013();

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
	//     Sets the text color for a specified tab.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab with the text color to be set.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem);

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
	//     Obtains client background color.
	// Parameters:
	//     pTabManager - Pointer to the tab manager with the tab client area to fill.
	// Returns:
	//     Client area background color value.
	//-----------------------------------------------------------------------
	virtual COLORREF GetClientColor(CXTPTabManager* pTabManager);

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
	//
	//     This method must be overridden.
	// See Also:
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

protected:
	void FillNavigateButtonEx(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc,
							  CXTPPaintManagerColor clrBack = RGB(255, 0, 0),
							  CXTPPaintManagerColor clrText = RGB(0, 0, 255));

public:
	CXTPPaintManagerColor m_clrButtonSeparator;
	CXTPPaintManagerColor m_clrNonActiveButtonSeparator;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCOLORSETOFFICE2013_H__)
