// XTPPopupThemeOffice2003.h: interface of the CXTPPopupThemeOffice2003 class.
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
#if !defined(__XTPPOPUPTHEMEOFFICE2003_H__)
#	define __XTPPOPUPTHEMEOFFICE2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPPopupThemeOffice2003 is a class used to perform Office 2003
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeOffice2003 : public CXTPPopupThemeOfficeXP
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeOffice2003 object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeOffice2003();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupThemeOffice2003 object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupThemeOffice2003();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the background of a pop-up window.
	// Parameters:
	//     pDC - Pointer to the device context.
	//     rcClient - Size of the area to fill.
	//     pControl - Pop-up control to draw.
	//-----------------------------------------------------------------------
	virtual void DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient);

public:
	CXTPPaintManagerColorGradient m_clrGripper; // Color of the pop-up gripper for the
												// xtpPopupThemeOffice2003 paint theme.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPPOPUPTHEMEOFFICE2003_H__
//}}AFX_CODEJOCK_PRIVATE
