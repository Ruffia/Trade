// XTPTaskPanelListViewOffice2003Theme.h : implementation of the
// CXTPTaskPanelListViewOffice2003Theme class.
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
#ifndef XTPTaskPanelListViewOffice2003Theme_h__
#define XTPTaskPanelListViewOffice2003Theme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelListViewOffice2003Theme represents a List View Office 2003 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelListViewOffice2003Theme
	: public CXTPTaskPanelListViewOfficeXPTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelListViewOffice2003Theme object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelListViewOffice2003Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	CXTPPaintManagerColorGradient m_grcHot;	// Color gradient used to colorize "hot"
											   // items for this theme.
	CXTPPaintManagerColorGradient m_grcPushed; // Color gradient used to colorize "pushed"
											   // items for this theme.

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background of a group caption.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group with the group caption to fill.
	//     rc     - Bounding rectangle of the group caption.
	// Returns:
	//     The bounding rectangle of the group caption.
	// Remarks:
	//     This method will draw the normal, hot, and pressed backgrounds for
	//     the group caption. It will then fill the group caption with
	//     the Office 2003 color gradient.
	//-----------------------------------------------------------------------
	CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);
};

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // XTPTaskPanelListViewOffice2003Theme_h__
