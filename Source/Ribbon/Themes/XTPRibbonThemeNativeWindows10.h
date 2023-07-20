// XTPRibbonThemeNativeWindows10.h: interface for the CXTPRibbonThemeNativeWindows10 class.
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
#if !defined(__XTPRIBBONNATIVEWINDOWS10THEME_H__)
#	define __XTPRIBBONNATIVEWINDOWS10THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPRibbonThemeNativeWindows10 is a CXTPRibbonOffice2013Theme derived class
//     used to represent a Native Windows 10 ribbon theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonThemeNativeWindows10 : public CXTPRibbonOffice2013Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonThemeNativeWindows10 object.
	// Parameters:
	//     pPaintManager - A CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonThemeNativeWindows10(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonThemeNativeWindows10 object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonThemeNativeWindows10();

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called to refresh the visual metrics of the manager.
	// Remarks:
	//      Refreshes all of the colors in the ribbon bar. Many of the colors
	//      used are set with the GROUP_COLOR structure.
	// See Also:
	//     GROUP_COLOR
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the background of a CXTPRibbonGroup.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pActiveTab - Pointer to the tab that is currently active.
	//     rcGroups   - Bounding rectangle of the group.
	// Remarks:
	//     Performs a gradient fill using the GROUP_COLOR::clrBackground for
	//     the main group background and GROUP_COLOR::clrCaption to fill the
	//     group caption rectangle.
	// See Also:
	//     CXTPRibbonGroup::GetRect, GROUP_COLOR
	//-----------------------------------------------------------------------
	void FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single tab of the ribbon bar.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Ribbon tab to draw.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon context headers.
	// Parameters:
	//     pDC             - Pointer to a valid device context.
	//     pContextHeaders - Collection of context headers.
	//     pRibbonBar      - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameContextHeaders(CDC* pDC, CXTPRibbonBar* pRibbonBar,
											   CXTPRibbonTabContextHeaders* pContextHeaders);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONNATIVEWINDOWS10THEME_H__)
