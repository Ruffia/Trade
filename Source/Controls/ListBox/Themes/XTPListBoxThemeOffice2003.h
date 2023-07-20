// XTPListBoxThemeOffice2003.h
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
#if !defined(__XTPLISTBOXTHEMEOFFICE2003_H__)
#	define __XTPLISTBOXTHEMEOFFICE2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPListBoxThemeOffice2003 is used by CXTPListBox to draw an
//     Office 2003 style list box.
//===========================================================================
class _XTP_EXT_CLASS CXTPListBoxThemeOffice2003 : public CXTPListBoxThemeOfficeXP
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the CXTPListBox object.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pListBox - Pointer to a valid CXTPListBox object.
	//     lpDIS    - A long pointer to a DRAWITEMSTRUCT structure. The
	//                structure contains information about the item to be
	//                drawn and the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(CDC* pDC, CXTPListBox* pListBox, LPDRAWITEMSTRUCT lpDIS);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to update the colors and metrics used to draw the
	//     CXTPListBox object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPLISTBOXTHEMEOFFICE2003_H__)
