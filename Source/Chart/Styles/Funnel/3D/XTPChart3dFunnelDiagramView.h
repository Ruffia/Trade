// XTPChart3dFunnelDiagramView.h
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
#if !defined(__XTPCHART3DFUNNELDIAGRAMVIEW_H__)
#	define __XTPCHART3DFUNNELDIAGRAMVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDiagram;
class CXTPChartElementView;

//===========================================================================
// Summary:
//     This class abstracts the view of a funnel diagram. This class is a kind of
//     CXTPChartDiagramView.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChart3dFunnelDiagramView : public CXTPChart3dDiagramView
{
	DECLARE_DYNAMIC(CXTPChart3dFunnelDiagramView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dFunnelDiagramView object.
	// Parameters:
	//     pDiagram - A pointer to the funnel diagram object.
	//     pParent  - A pointer to the parent view.
	//-----------------------------------------------------------------------
	CXTPChart3dFunnelDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DFUNNELDIAGRAMVIEW_H__)
