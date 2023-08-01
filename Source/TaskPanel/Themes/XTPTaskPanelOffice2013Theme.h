// XTPTaskPanelOffice2013Theme.cpp : implementation of the CXTPTaskPanelOffice2013Theme class.
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
#if !defined(__XTPTASKPANELOFFICE2013THEME_H__)
#	define __XTPTASKPANELOFFICE2013THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelOffice2013Theme represents an Office 2013 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelOffice2013Theme : public CXTPTaskPanelVisualStudio2012Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelOffice2013Theme object
	//-----------------------------------------------------------------------
	CXTPTaskPanelOffice2013Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelOffice2013Theme object,
	//     handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelOffice2013Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the background of a specified task panel.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pTaskPanel - Pointer to the task panel to fill.
	//-----------------------------------------------------------------------
	virtual void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // __XTPTASKPANELOFFICE2013THEME_H__