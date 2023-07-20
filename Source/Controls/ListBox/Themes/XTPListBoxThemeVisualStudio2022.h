// XTPListBoxThemeVisualStudio2022.h
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
#if !defined(__XTPLISTBOXTHEMEVISUALSTUDIO2022_H__)
#	define __XTPLISTBOXTHEMEVISUALSTUDIO2022_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPListBoxThemeVisualStudio2022 is used by CXTPListBox to draw a
//     Visual Studio 2022 style list box.
//===========================================================================
class _XTP_EXT_CLASS CXTPListBoxThemeVisualStudio2022 : public CXTPListBoxThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPListBoxThemeVisualStudio2022();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPLISTBOXTHEMEVISUALSTUDIO2022_H__)
