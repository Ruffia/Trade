// X TPChartFunnelSeriesView.h
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
#if !defined(__XTPCHARTFUNNELSERIESVIEWBASE_H__)
#	define __XTPCHARTFUNNELSERIESVIEWBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;
class CXTPChartDiagramDomain;

//===========================================================================
// Summary:
//     This class abstracts the view of a funnel series.This class is a kind of
//     CXTPChartSeriesView.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesViewBase : public CXTPChartSeriesView
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesViewBase);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesViewBase object.
	// Parameters:
	//     pSeries       - Pointer to funnel series object.
	//     pDiagramView  - Pointer to the funnel diagram view.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesViewBase(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFunnelSeriesViewBase object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFunnelSeriesViewBase();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the funnel series point layout.
	// Parameters:
	//     pDC      - Pointer to the chart device context.
	//     rcBounds - The rectangular boundary.
	//-------------------------------------------------------------------------
	virtual void CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the funnel series label layout.
	// Parameters:
	//     pDC      - Pointer to the chart device context.
	//     rcBounds - The rectangular boundary.
	//-------------------------------------------------------------------------
	void CalculateLabelLayout(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the pie diagram domain object associated
	//     with this object.
	// Parameters:
	//     rcBounds - The rectangular boundary of the diagram.
	// Returns:
	//     A pointer to the chart diagram domain object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramDomain* CreateDiagramDomain(CXTPChartDeviceContext* pDC,
														CRect rcBounds);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the inner bounds of the diagram.
	// Returns:
	//     A chart rect object denoting the rectangular boundary.
	//-------------------------------------------------------------------------
	virtual CXTPChartRectF GetInnerBounds() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the pie diagram domain object associated with
	//     this object.
	// Returns:
	//     A pointer to the chart diagram domain object.
	//-------------------------------------------------------------------------
	CXTPChartDiagramDomain* GetDomain() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains maximum allowed label width.
	// Returns:
	//      Maximum allowed label width.
	//-----------------------------------------------------------------------
	int GetMaxLabelWidth() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a label view drawing command object.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds - The rectangular boundary of the legend.
	//     color1 - First gradient color.
	//     color2 - Second gradient color.
	///    clrBorder - Border color.
	// Returns:
	//     Returns a new label view drawing command object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC,
															  CRect rcBounds, CXTPChartColor color1,
															  CXTPChartColor color2,
															  CXTPChartColor clrBorder) = 0;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to create the funnel series point view object.
	// Parameters:
	//     pDC    - Pointer to chart device context.
	//     pPoint - Pointer to chart series point.
	// Returns:
	//     A pointer to the chart series point view.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC,
															CXTPChartSeriesPoint* pPoint,
															CXTPChartElementView* pParentView) = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the funnel series.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds - The rectangular boundary of the legend.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the funnel series.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC) = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a label view drawing command object.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds - The rectangular boundary of the legend.
	// Returns:
	//     Returns a new label view drawing command object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC,
															  CRect rcBounds) = 0;

	//{{AFX_CODEJOCK_PRIVATE
	void CalculateValues();
	//{{AFX_CODEJOCK_PRIVATE

protected:
	CXTPChartDiagramDomain* m_pDomain; // Pointer to the diagram domain.
	int m_nMaxLabelWidth;
};

AFX_INLINE CXTPChartDiagramDomain* CXTPChartFunnelSeriesViewBase::GetDomain() const
{
	return m_pDomain;
}

AFX_INLINE int CXTPChartFunnelSeriesViewBase::GetMaxLabelWidth() const
{
	return m_nMaxLabelWidth;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTFUNNELSERIESVIEWBASE_H__)
