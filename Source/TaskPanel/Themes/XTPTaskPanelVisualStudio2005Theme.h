// XTPTaskPanelVisualStudio2005Theme.h : implementation of the CXTPTaskPanelVisualStudio2005Theme
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
#if !defined(__XTPTASKPANELVISUALSTUDIO2005THEME_H__)
#	define __XTPTASKPANELVISUALSTUDIO2005THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelVisualStudio2005Theme represents a Visual Studio 2005 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelVisualStudio2005Theme : public CXTPTaskPanelToolboxTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelVisualStudio2005Theme object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelVisualStudio2005Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group caption background of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the group caption background of.
	//     rc     - Bounding rectangle of the group caption.
	// Returns:
	//     The bounding rectangle of the text within the group caption.
	//-----------------------------------------------------------------------
	CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the client face of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the client face of.
	//     rc     - Rectangle of the group to draw the client face of.
	//-----------------------------------------------------------------------
	void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client face of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to fill the client face of.
	//     rc     - Rectangle of the group to fill the client face of.
	// See Also:
	//     DrawGroupClientFace
	//-----------------------------------------------------------------------
	void FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the background of a specified task panel.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pTaskPanel - Pointer to the task panel to fill.
	//-----------------------------------------------------------------------
	void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

protected:
	virtual CRect DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);
};

//{{AFX_CODEJOCK_PRIVATE
typedef CXTPTaskPanelVisualStudio2005Theme CXTPTaskPanelToolboxWhidbeyTheme;
//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // __XTPTASKPANELVISUALSTUDIO2005THEME_H__
