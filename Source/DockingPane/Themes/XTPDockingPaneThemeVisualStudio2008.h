// XTPDockingPaneThemeVisualStudio2008.h
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
#if !defined(__XTPDOCKINGPANETHEMEVISUALSTUDIO2008_H__)
#	define __XTPDOCKINGPANETHEMEVISUALSTUDIO2008_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//=============================================================================
// Summary:
//     CXTPDockingPaneVisualStudio2008Theme is a CXTPDockingPaneVisualStudio2005Theme
//     derived class that represents a Visual Studio 2008 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio2008); to set
//     this theme.
//=============================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2008Theme
	: public CXTPDockingPaneVisualStudio2005Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2008Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2008Theme();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of manager.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPDOCKINGPANETHEMEVISUALSTUDIO2008_H__)
