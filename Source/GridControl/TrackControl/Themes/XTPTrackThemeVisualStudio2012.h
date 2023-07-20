// XTPTrackThemeVisualStudio2012.h: interface for the CXTPTrackThemeOffice2013 class.
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
#if !defined(__XTPTRACKTHEMEVISUALSTUDIO2012_H__)
#	define __XTPTRACKTHEMEVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2012Light is a CXTPTrackThemeOffice2013 derived
//     class that represents a Visual Studio 2012 Light style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2012Light : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2012Light();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the track control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     track control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2012Dark is a CXTPTrackThemeOffice2013 derived
//     class that represents a Visual Studio 2012 Dark style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2012Dark : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2012Dark();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the track control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     track control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2012Blue is a CXTPTrackThemeOffice2013 derived
//     class that represents a Visual Studio 2012 Blue style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2012Blue : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2012Blue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the track control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     track control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPTRACKTHEMEVISUALSTUDIO2012_H__)
