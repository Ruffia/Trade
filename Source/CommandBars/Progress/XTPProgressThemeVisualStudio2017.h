// XTPProgressThemeVisualStudio2017.h
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
#if !defined(__XTPPROGRESSTHEMEVISUALSTUDIO2017_H__)
#	define __XTPPROGRESSTHEMEVISUALSTUDIO2017_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPProgressBase;
class CXTPPaintManager;

//===========================================================================
// Summary:
//     CXTPProgressThemeVisualStudio2017 object.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressThemeVisualStudio2017 : public CXTPProgressThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressThemeVisualStudio2017 object.
	// Parameters:
	//     pPaintManager - PaintManager of commandbars.
	//-----------------------------------------------------------------------
	CXTPProgressThemeVisualStudio2017(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressThemeVisualStudio2017 object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressThemeVisualStudio2017();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPPROGRESSTHEMEVISUALSTUDIO2017_H__)
