// XTPRibbonThemeOffice2007System.h: interface for the CXTPRibbonThemeOffice2007System class.
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
#if !defined(__XTPRIBBONTHEMEOFFICE2007SYSTEM_H__)
#	define __XTPRIBBONTHEMEOFFICE2007SYSTEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPRibbonThemeOffice2007System is a CXTPRibbonMarkupTheme derived class
//     used to represent an Office 2007 ribbon theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonThemeOffice2007System : public CXTPRibbonMarkupTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonThemeOffice2007System object.
	// Parameters:
	//     pPaintManager - A CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonThemeOffice2007System(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonThemeOffice2007System object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonThemeOffice2007System();

public:
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
	//     This method is called to draw ribbon Quick Access controls.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pRibbonBar - Pointer to the parent ribbon bar.
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessControls(CDC* pDC, CXTPRibbonBar* pRibbonBar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the ribbon caption height.
	// Parameters:
	//     pRibbonBar - Pointer to the parent ribbon bar.
	// Returns:
	//     The ribbon caption height.
	//-----------------------------------------------------------------------
	virtual int GetRibbonCaptionHeight(const CXTPRibbonBar* pRibbonBar) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the Quick Access customize button.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pControl - Pointer to the Quick Access customize button.
	// See Also:
	//     DrawRibbonQuickAccessMoreButton
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads markup parts.
	// Parameters:
	//     bReload - Re-loads previously loaded parts if TRUE.
	//-----------------------------------------------------------------------
	virtual void LoadParts(BOOL bReload = FALSE);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(LoadParts)
	BOOL LoadResource();
	//}}AFX_CODEJOCK_PRIVATE
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONTHEMEOFFICE2007SYSTEM_H__)
