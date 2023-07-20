// XTPTaskPanelToolboxTheme.cpp : implementation of the CXTPTaskPanelToolboxTheme class.
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
#ifndef XTPTaskPanelToolboxTheme_h__
#define XTPTaskPanelToolboxTheme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelToolboxTheme represents a Toolbox style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelToolboxTheme : public CXTPTaskPanelPaintManagerPlain
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelToolboxTheme object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelToolboxTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the caption of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the caption of.
	//     bDraw  - TRUE to draw the caption of the specified group,
	//              FALSE to retrieve the height of the caption.
	// Returns:
	//     The height of the caption.
	//-----------------------------------------------------------------------
	int DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw);

protected:
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
	virtual CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);
};

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // XTPTaskPanelToolboxTheme_h__
