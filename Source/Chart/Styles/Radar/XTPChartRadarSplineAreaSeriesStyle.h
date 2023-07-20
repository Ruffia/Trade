// XTPChartRadarSplineAreaSeriesStyle.h
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
#if !defined(__XTPCHARTRADARSPLINEAREASERIESSTYLE_H__)
#	define __XTPCHARTRADARSPLINEAREASERIESSTYLE_H__
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
//     This class represents a spline area series style, which is a kind of
//     CXTPChartRadarAreaSeriesStyle.
// Remarks:
//     Spline area series displays graphically the quantitative data in a chart.
//     It displays a series as a set of points connected by a spline with all
//     the area filled in below the line.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarSplineAreaSeriesStyle : public CXTPChartRadarAreaSeriesStyle
{
	DECLARE_SERIAL(CXTPChartRadarSplineAreaSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarSplineAreaSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartRadarSplineAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarSplineAreaSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRadarSplineAreaSeriesStyle();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to create a spline area series view object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Returns:
	//     Returns a pointer to the associated series view object.
	// Remarks:
	//     CXTPChartRadarSplineAreaSeriesView class abstracts the view of a spine area
	//     series.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRadarSplineAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartRadarSplineAreaSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
};

//===========================================================================
// Summary:
//     CXTPChartRadarSplineAreaSeriesView is a kind of CXTPChartSeriesView. This class
//     represents the view of the area series of a chart.
// Remarks:
//     Spline area series displays graphically the quantitative data in a chart.
//     It displays a series as a set of points connected by a spline with all
//     the area filled in below the line.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarSplineAreaSeriesView : public CXTPChartRadarAreaSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarSplineAreaSeriesView object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartRadarSplineAreaSeriesView(CXTPChartSeries* pSeries,
									   CXTPChartDiagramView* pDiagramView);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create a spline area series point view object.
	// Parameters:
	//     pDC      - Pointer to a chart device context object.
	//     pPoint   - Pointer to a chart series object.
	// Returns:
	//     Returns a pointer to the series point view object.
	// Remarks:
	//     CXTPChartSeriesPointView object abstracts the view of a point in a series.
	//-------------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC,
													CXTPChartSeriesPoint* pPoint,
													CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a spline area series in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     A CXTPChartDeviceCommand object. This polymorphic object handles
	//     the rendering of an element in the chart. Here it handles the drawing
	//     of the spline area series of the chart.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTRADARSPLINEAREASERIESSTYLE_H__)
