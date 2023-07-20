// XTPTaskPanelVisualStudio2017Theme.h : implementation of the CXTPTaskPanelVisualStudio2017Theme
// class.
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
#if !defined(__XTPTASKPANELVISUALSTUDIO2017THEME_H__)
#	define __XTPTASKPANELVISUALSTUDIO2017THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelVisualStudio2017Theme represents a Visutal Studio 2017 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelVisualStudio2017Theme : public CXTPTaskPanelVisualStudio2012Theme
{
protected:
	void Triangle(CDC* pDC, POINT* pPoints, COLORREF clr, BOOL bFilled);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelVisualStudio2017Theme object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelVisualStudio2017Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelVisualStudio2017Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelVisualStudio2017Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // __XTPTASKPANELVISUALSTUDIO2017THEME_H__
