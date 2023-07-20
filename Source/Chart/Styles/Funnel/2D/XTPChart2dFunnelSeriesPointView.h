// XTPChart2dFunnelSeriesPointView.h
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
#if !defined(__XTPCHART2DFUNNELSERIESPOINTVIEW_H__)
#	define __XTPCHART2DFUNNELSERIESPOINTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesPoint;
class CXTPChartElementView;
class CXTPChartDeviceContext;

//===========================================================================
// Summary:
//     This class abstracts the view of a funnel series point. This class is a kind of
//     CXTPChartSeriesPointView.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesPointView : public CXTPChartFunnelSeriesPointViewBase
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesPointView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesPointView object.
	// Parameters:
	//     pPoint        - Pointer to a chart series point.
	//     bAddToParent  - If TRUE the view will be be added to parent's child view list.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);
	CXTPChartFunnelSeriesPointView(
		CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView,
		BOOL bAddToParent); // <combine
							// CXTPChartFunnelSeriesPointView::CXTPChartFunnelSeriesPointView@CXTPChartSeriesPoint*@CXTPChartElementView*>

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the funnel series point.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the funnel series point.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART2DFUNNELSERIESPOINTVIEW_H__)
