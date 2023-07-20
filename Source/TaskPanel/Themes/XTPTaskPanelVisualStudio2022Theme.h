// XTPTaskPanelVisualStudio2022Theme.h : implementation of the CXTPTaskPanelVisualStudio2022Theme
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
#if !defined(__XTPTASKPANELVISUALSTUDIO2022THEME_H__)
#	define __XTPTASKPANELVISUALSTUDIO2022THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelVisualStudio2022Theme represents a Visutal Studio 2022 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelVisualStudio2022Theme : public CXTPTaskPanelVisualStudio2012Theme
{
protected:
	void Triangle(CDC* pDC, POINT* pPoints, COLORREF clr, BOOL bFilled);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelVisualStudio2022Theme object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelVisualStudio2022Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelVisualStudio2022Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelVisualStudio2022Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // __XTPTASKPANELVISUALSTUDIO2022THEME_H__
