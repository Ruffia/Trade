// XTPButtonThemeResource.h
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
#if !defined(__XTPBUTTONTHEMERESOURCE_H__)
#	define __XTPBUTTONTHEMERESOURCE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPButtonThemeResource is a class used to perform
//     Office 2007 Theme drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeResource : public CXTPButtonThemeUltraFlat
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPButtonThemeResource object
	//-----------------------------------------------------------------------
	CXTPButtonThemeResource();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device context.
	//     pButton - Pointer to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the check box mark for
	//     the button control.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to the CXTPButton to draw the check box for.
	//-----------------------------------------------------------------------
	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the radio mark for
	//     the button control.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to the CXTPButton to draw the radio mark for.
	//-----------------------------------------------------------------------
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each component's theme.
	// Parameters:
	//     pButton - Pointer to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPButton* pButton);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPBUTTONTHEMERESOURCE_H__)
