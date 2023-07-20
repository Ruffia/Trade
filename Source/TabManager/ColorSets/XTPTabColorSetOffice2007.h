// XTPTabColorSetOffice2007.h
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
#if !defined(__XTPTABCOLORSETOFFICE2007_H__)
#	define __XTPTABCOLORSETOFFICE2007_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetOffice2007 implements an Office 2007 tab color set.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetOffice2007 : public CXTPTabColorSetOffice2003
{
public:
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
	//     If IsAppThemed is FALSE, then
	//     CXTPTabColorSetDefault::FillNavigationButton is used.
	// See Also:
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

protected:
	CXTPPaintManagerColor m_clrButtonText; // Text color.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
//{{AFX_CODEJOCK_PRIVATE
#endif // !defined(__XTPTABCOLORSETOFFICE2007_H__)
//}}AFX_CODEJOCK_PRIVATE
