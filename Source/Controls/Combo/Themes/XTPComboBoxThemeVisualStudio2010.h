// XTPComboBoxThemeVisualStudio2010.h: interface for the CXTPComboBoxThemeVisualStudio2010 class.
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
#ifndef XTPComboBoxThemeVisualStudio2010_h__
#define XTPComboBoxThemeVisualStudio2010_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     Used as the paint manager for a combobox control to render a
//     Visual Studio 2010 appearance.
//===========================================================================
class _XTP_EXT_CLASS CXTPComboBoxThemeVisualStudio2010 : public CXTPComboBoxThemeVisualStudio
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPComboBoxThemeVisualStudio2010();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructor handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPComboBoxThemeVisualStudio2010();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each component's theme.
	// Parameters:
	//     pComboBox - Pointer to a CXTPComboBox object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPComboBox* pComboBox);
};

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // XTPComboBoxThemeVisualStudio2010_h__
