// XTPChart3dFunnelSeriesStyle.h
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
#if !defined(__XTPCHART3DFUNNELSERIESSTYLE_H__)
#	define __XTPCHART3DFUNNELSERIESSTYLE_H__
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
class _XTP_EXT_CLASS CXTPChart3dFunnelSeriesStyle : public CXTPChartFunnelSeriesStyleBase
{
	DECLARE_SERIAL(CXTPChart3dFunnelSeriesStyle);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dFunnelSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChart3dFunnelSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the funnel diagram object.
	// Returns:
	//     A pointer to a CXTPChartDiagram object which points to the newly created
	//     CXTPChart3dFunnelDiagramBase object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagram* CreateDiagram();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the funnel series.
	// Parameters:
	//     pSeries       - A pointer to the chart series object.
	//     pDiagramView  - A pointer to the diagram view object.
	// Returns:
	//     A pointer to the newly created 3D series view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load an event using the
	//     specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store event data to or load
	//     event data from storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets the number of base edges for the funnel. The number of
	//      edges must not be equal to 1 or 2. If 0 is used as edge count, then
	//      a round funnel (with no edges) will be drawn.
	// Parameters:
	//      nEdgeCount - A new number of base edges.
	// Returns:
	//      Current number of base edges.
	//-----------------------------------------------------------------------
	int GetBaseEdgeCount() const;
	void SetBaseEdgeCount(int nEdgeCount); // <combine
										   // CXTPChart3dFunnelSeriesStyle::GetBaseEdgeCount@const>

	//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_ACTIVEX
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChart3dFunnelSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChart3dFunnelSeriesStyle)
#	endif
	//}}AFX_CODEJOCK_PRIVATE

private:
	int m_nBaseEdgeCount;
};

AFX_INLINE int CXTPChart3dFunnelSeriesStyle::GetBaseEdgeCount() const
{
	return m_nBaseEdgeCount;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DFUNNELSERIESSTYLE_H__)
