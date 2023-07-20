// XTPGridThemeVisualStudio2012.h: interface for the CXTPGridThemeOffice2013 class.
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
#if !defined(__XTPGRIDTHEMEVISUALSTUDIO2012_H__)
#	define __XTPGRIDTHEMEVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridThemeVisualStudio2012Light is a CXTPGridThemeOffice2013 derived
//     class that represents a Visual Studio 2012 Light style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeVisualStudio2012Light : public CXTPGridThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeVisualStudio2012Light();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the Grid control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Grid control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPGridThemeVisualStudio2012Dark is a CXTPGridThemeOffice2013 derived
//     class that represents a Visual Studio 2012 Dark style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeVisualStudio2012Dark : public CXTPGridThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeVisualStudio2012Dark();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the Grid control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Grid control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPGridThemeVisualStudio2012Blue is a CXTPGridThemeOffice2013 derived
//     class that represents a Visual Studio 2012 Blue style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeVisualStudio2012Blue : public CXTPGridThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeVisualStudio2012Blue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the Grid control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Grid control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDTHEMEVISUALSTUDIO2012_H__)
