// XTPChart2dFunnelSeriesStyle.h
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
#if !defined(__XTPCHART2DFUNNELSERIESSTYLE_H__)
#	define __XTPCHART2DFUNNELSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     This class abstracts the basic style of a funnel series. This class is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesStyle : public CXTPChartFunnelSeriesStyleBase
{
	DECLARE_SERIAL(CXTPChartFunnelSeriesStyle)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesStyleBase object.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesStyle();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the diagram for this series.
	// Returns:
	//     A pointer to a CXTPChartDiagram object which points to the newly created
	//     CXTPChartDiagram2D object.
	//-----------------------------------------------------------------------
	CXTPChartDiagram* CreateDiagram();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the funnel series.
	// Parameters:
	//     pSeries       - A pointer to a chart series object.
	//     pDiagramView  - A pointer to the diagram view object.
	// Returns:
	//     A pointer to the newly created 2D series view object.
	//-------------------------------------------------------------------------
	CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the chart diagram object passed
	//     is a kind of CXTPChart3dFunnelDiagramBase/CXTPChart3dFunnelDiagramBase.
	// Parameters:
	//     pDiagram - A pointer to a chart diagram, whose type is to be identified.
	// Returns:
	//     TRUE if the chart diagram is a funnel diagram, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsStyleDiagram(CXTPChartDiagram* pDiagram) const;

	//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_ACTIVEX
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFunnelSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartFunnelSeriesStyle)
#	endif
	//}}AFX_CODEJOCK_PRIVATE
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART2DFUNNELSERIESSTYLE_H__)
