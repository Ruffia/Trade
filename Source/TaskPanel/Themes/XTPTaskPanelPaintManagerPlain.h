// XTPTaskPanelPaintManagerPlain.h : implementation of the CXTPTaskPanelPaintManagerPlain class.
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
#ifndef XTPTaskPanelPaintManagerPlain_h__
#define XTPTaskPanelPaintManagerPlain_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     Base class theme that all other themes are derived from.
//     Use this class when creating a custom theme.
// See Also:
//     CXTPTaskPanel::SetTheme, CXTPTaskPanel::SetCustomTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelPaintManagerPlain : public CXTPTaskPanelPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelPaintManagerPlain object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManagerPlain();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

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
	BOOL m_bRoundedFrame;  // TRUE to draw a rounded frame around all of the groups in the task
						   // panel. Can be seen in the Native WinXP theme. Default is TRUE.
	BOOL m_bOfficeCaption; // FALSE to draw a rounded rectangle around caption. TRUE to not draw
						   // a rectangle around caption, caption will not appear in a rectangle.
						   // Default is FALSE.
};

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // XTPTaskPanelPaintManagerPlain_h__
