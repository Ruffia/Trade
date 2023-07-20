// XTPControlGalleryVisualStudio2022Theme.h : interface for the
// CXTPControlGalleryVisualStudio2022Theme class.
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
#if !defined(__XTPCONTROLGALLERYVISUALSTUDIO2022THEME_H__)
#	define __XTPCONTROLGALLERYVISUALSTUDIO2022THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The CXTPControlGalleryResourceTheme class is used to enable a
//     Gallery Visual Studio 2022 style theme for Command Bars.
// See Also:
//     CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryVisualStudio2022Theme
	: public CXTPControlGalleryOffice2013Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryVisualStudio2022Theme object.
	// Parameters:
	//     pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPControlGalleryVisualStudio2022Theme(CXTPPaintManager* pPaintManager);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCONTROLGALLERYVISUALSTUDIO2022THEME_H__)
