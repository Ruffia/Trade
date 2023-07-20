// XTPDatePickerVisualStudio2017Theme.h
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
#if !defined(_XTPDATEPICKERVISUALSTUDIO2017THEME_H__)
#	define _XTPDATEPICKERVISUALSTUDIO2017THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//---------------------------------------------------------------------------
// Summary:
//     Class CXTPDatePickerThemeVisualStudio2017 is derived from CXTPDatePickerThemeOffice2013
//     for rendering an Visual Studio 2017 style for DatePicker control.
// Example:
// <code>
//      if (m_bThemeVisualStudio2017)
//      {
//          CXTPDatePickerThemeVisualStudio2017* pTheme = new CXTPDatePickerThemeVisualStudio2017;
//          m_wndDatePicker.SetTheme(pTheme);
//      }
//      else
//      {
//          m_wndDatePicker.SetTheme(NULL);
//      }
// </code>
// See Also:
//     CXTPDatePickerThemeVisualStudio2017
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPDatePickerThemeVisualStudio2017 : public CXTPDatePickerThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also: RefreshMetrics()
	//-----------------------------------------------------------------------
	CXTPDatePickerThemeVisualStudio2017();

	// -------------------------------------
	// Summary:
	//     Default paint manager destructor.
	// Remarks:
	//     Handles member item deallocation.
	// -------------------------------------
	virtual ~CXTPDatePickerThemeVisualStudio2017();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(_XTPDATEPICKERVISUALSTUDIO2017THEME_H__)
