// XTPCalendarCaptionBarVisualStudio2019Theme.h
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
#if !defined(__XTPCALENDARCAPTIONBARVISUALSTUDIO2019THEME_H__)
#	define __XTPCALENDARCAPTIONBARVISUALSTUDIO2019THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPCalendarCaptionBarVisualStudio2019Theme is a CXTPCalendarCaptionBarOffice2013Theme
//     derived class that represents the Office 2013 white calendar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarCaptionBarVisualStudio2019Theme
	: public CXTPCalendarCaptionBarOffice2013Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also: RefreshMetrics()
	//-----------------------------------------------------------------------
	CXTPCalendarCaptionBarVisualStudio2019Theme(CXTPCalendarCaptionBarTheme* pTheme = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarCaptionBarVisualStudio2019Theme();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCALENDARCAPTIONBARVISUALSTUDIO2019THEME_H__)
