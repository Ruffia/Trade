// XTPTabColorSetVisualStudio2022.h
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
#if !defined(__XTPTABCOLORSETVISUALSTUDIO2022_H__)
#	define __XTPTABCOLORSETVISUALSTUDIO2022_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabColorSetVisualStudio2022 implements a Visual Studio 2022 tab color set.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetVisualStudio2022 : public CXTPTabColorSetVisualStudio2019
{
public:
	CXTPTabColorSetVisualStudio2022();

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the tabs.
	//     Override this method to change the colors of the color members.
	// Remarks:
	//     All of the color members are refreshed when this method is called.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCOLORSETVISUALSTUDIO2022_H__)
