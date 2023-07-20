// XTPMessageBarThemeVisualStudio2017.h
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
#if !defined(__XTPMESSAGEBARTHEMEVISUALSTUDIO2017_H__)
#	define __XTPMESSAGEBARTHEMEVISUALSTUDIO2017_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPaintManager;
class CXTPMessageBar;
class CXTPMessageBarButton;

//===========================================================================
// Summary:
//     The CXTPMessageBarThemeVisualStudio2017 class is used by the message bar
//     to draw a Visual Studio 2017 style message bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMessageBarThemeVisualStudio2017 : public CXTPMessageBarThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Message bar constructor.
	// Parameters:
	//     pPaintManager - Pointer to the owner paint manager.
	//-----------------------------------------------------------------------
	CXTPMessageBarThemeVisualStudio2017(CXTPPaintManager* pPaintManager);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPMESSAGEBARTHEMEVISUALSTUDIO2017_H__)
