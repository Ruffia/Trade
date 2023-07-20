// XTPRibbonBackstageViewThemeNativeWindows10.h: interface for the
// CXTPRibbonBackstageViewThemeNativeWindows10 class.
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
#if !defined(__XTPRIBBONBACKSTAGEVIEWTHEMENATIVEWINDOWS10_H__)
#	define __XTPRIBBONBACKSTAGEVIEWTHEMENATIVEWINDOWS10_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPRibbonBackstageViewThemeNativeWindows10
	: public CXTPRibbonBackstageViewThemeOffice2013
{
public:
	CXTPRibbonBackstageViewThemeNativeWindows10();
	~CXTPRibbonBackstageViewThemeNativeWindows10();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the backstage button back normal color (open, close, options).
	// Parameters:
	//     clr - Color value.
	//-----------------------------------------------------------------------
	void SetButtonBackNormalColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the backstage button back selected color (open, close, options).
	// Parameters:
	//     clr - Color value.
	//-----------------------------------------------------------------------
	void SetButtonBackSelectedColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the backstage button back highlighted color (open, close, options).
	// Parameters:
	//     clr - Color value.
	//-----------------------------------------------------------------------
	void SetButtonBackHighlightedColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the backstage button text normal color (open, close, options).
	// Parameters:
	//     clr - Color value.
	//-----------------------------------------------------------------------
	void SetButtonTextNormalColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the backstage button text selected color (open, close, options).
	// Parameters:
	//     clr - Color value.
	//-----------------------------------------------------------------------
	void SetButtonTextSelectedColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the backstage button text highlighted color (open, close, options).
	// Parameters:
	//     clr - Color value.
	//-----------------------------------------------------------------------
	void SetButtonTextHighlightedColor(COLORREF clr);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONBACKSTAGEVIEWTHEMENATIVEWINDOWS10_H__)
