// XTPRibbonThemeVisualStudio2022.h: interface for the CXTPRibbonThemeVisualStudio2022 class.
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
#if !defined(__XTPRIBBONVISUALSTUDIO2022THEME_H__)
#	define __XTPRIBBONVISUALSTUDIO2022THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPRibbonThemeVisualStudio2022 is a CXTPRibbonOffice2013Theme derived class
//     used to represent a Visual Studio 2022 ribbon theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonThemeVisualStudio2022 : public CXTPRibbonOffice2013Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonThemeVisualStudio2022 object.
	// Parameters:
	//     pPaintManager - A CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonThemeVisualStudio2022(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonThemeVisualStudio2022 object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonThemeVisualStudio2022();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONVISUALSTUDIO2022THEME_H__)
