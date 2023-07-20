// XTPProgressThemeNativeWindows10.h
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
#if !defined(__XTPPROGRESSTHEMENATIVEWINDOWS10_H__)
#	define __XTPPROGRESSTHEMENATIVEWINDOWS10_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPProgressBase;
class CXTPPaintManager;

//===========================================================================
// Summary:
//     CXTPProgressThemeNativeWindows10 object.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressThemeNativeWindows10 : public CXTPProgressThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressThemeNativeWindows10 object.
	// Parameters:
	//     pPaintManager - PaintManager of commandbars.
	//-----------------------------------------------------------------------
	CXTPProgressThemeNativeWindows10(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressThemeNativeWindows10 object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressThemeNativeWindows10();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPPROGRESSTHEMENATIVEWINDOWS10_H__)
