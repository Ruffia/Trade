// XTPTabThemeVisualStudio2017.h
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
#if !defined(__XTPTABTHEMEVISUALSTUDIO2017_H__)
#	define __XTPTABTHEMEVISUALSTUDIO2017_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabThemeVisualStudio2017 implements a Visual Studio 2017 tab appearance.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabThemeVisualStudio2017 : public CXTPTabThemeVisualStudio2015
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeVisualStudio2017 object.
	//-----------------------------------------------------------------------
	CXTPTabThemeVisualStudio2017();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the appearance metrics.
	//     Override this member to change the appearance metrics.
	// Remarks:
	//     This method is used to refresh the appearance metrics
	//     such as font, button height, margin size, etc.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
		return xtpTabColorVisualStudio2017;
	}
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPTABTHEMEVISUALSTUDIO2017_H__
//}}AFX_CODEJOCK_PRIVATE
