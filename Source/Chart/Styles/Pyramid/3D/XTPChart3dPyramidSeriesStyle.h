// XTPChart3dPyramidSeriesStyle.h
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
#if !defined(__XTPCHART3DPYRAMIDSERIESSTYLE_H__)
#	define __XTPCHART3DPYRAMIDSERIESSTYLE_H__
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
//     This class abstracts the basic style of a pyramid series. This class is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//     A pyramid chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pyramid chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChart3dPyramidSeriesStyle : public CXTPChartPyramidSeriesStyleBase
{
	DECLARE_SERIAL(CXTPChart3dPyramidSeriesStyle);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dPyramidSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChart3dPyramidSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the pyramid diagram object.
	// Returns:
	//     Returns a pointer to the associated diagram object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagram* CreateDiagram();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the pyramid series.
	// Parameters:
	//     pSeries       - A pointer to the chart series object.
	//     pDiagramView  - A pointer to the diagram view object.
	// Returns:
	//     Returns a pointer to the associated series view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether a chart diagram object passed
	//     is a kind of CXTPChart3dPyramidDiagramBase/CXTPChart3dPyramidDiagramBase.
	// Parameters:
	//     pDiagram - A pointer to a chart diagram, whose type is to be identified.
	// Returns:
	//     TRUE if the chart diagram is a pyramid diagram, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsStyleDiagram(CXTPChartDiagram* pDiagram) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load an event using the
	//     specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store event data to or load event
	//     data from storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets the number of base edges for the pyramid. The number of
	//      edges must not be equal to 1 or 2. If 0 is used as edge count the
	//      round pyramid (with no edges) will be drawn.
	// Parameters:
	//      nEdgeCount - A new number of base edges.
	// Returns:
	//      Current number of base edges.
	//-----------------------------------------------------------------------
	int GetBaseEdgeCount() const;
	void SetBaseEdgeCount(int nEdgeCount); // <combine
										   // CXTPChart3dPyramidSeriesStyle::GetBaseEdgeCount@const>

	//{{AFX_CODEJOCK_PRIVATE

	_XTP_DEPRECATED_IN_FAVOR(Is3dEdgeSmoothingEnabled)
	BOOL IsEdgeSmoothingEnabled() const
	{
		return Is3dEdgeSmoothingEnabled();
	}

	_XTP_DEPRECATED_IN_FAVOR(Enable3dEdgeSmoothing)
	void EnableEdgeSmoothing(BOOL bEnable = TRUE)
	{
		Enable3dEdgeSmoothing(bEnable);
	}

#	ifdef _XTP_ACTIVEX
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChart3dPyramidSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChart3dPyramidSeriesStyle)
#	endif
	//}}AFX_CODEJOCK_PRIVATE

private:
	int m_nBaseEdgeCount;
};

AFX_INLINE int CXTPChart3dPyramidSeriesStyle::GetBaseEdgeCount() const
{
	return m_nBaseEdgeCount;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DPYRAMIDSERIESSTYLE_H__)
