// XTPTaskPanelShortcutBarOffice2003Theme.h : implementation of the
// CXTPTaskPanelShortcutBarOffice2003Theme class.
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
#ifndef XTPTaskPanelShortcutBarOffice2003Theme_h__
#define XTPTaskPanelShortcutBarOffice2003Theme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelShortcutBarOffice2003Theme represents an Office 2003 ShortcutBar style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelShortcutBarOffice2003Theme
	: public CXTPTaskPanelListViewOffice2003Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelShortcutBarOffice2003Theme object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelShortcutBarOffice2003Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

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
};

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // XTPTaskPanelShortcutBarOffice2003Theme_h__
