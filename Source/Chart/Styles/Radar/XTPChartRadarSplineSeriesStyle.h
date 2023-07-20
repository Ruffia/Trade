// XTPChartRadarSplineSeriesStyle.h
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
#if !defined(__XTPCHARTRADARSPLINESERIESSTYLE_H__)
#	define __XTPCHARTRADARSPLINESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     This class represents the spline series style in the chart. Objects
//     of this class are a kind of point series style of a chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarSplineSeriesStyle : public CXTPChartRadarLineSeriesStyle
{
	DECLARE_SERIAL(CXTPChartRadarSplineSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarSplineSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartRadarSplineSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarSplineSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRadarSplineSeriesStyle();

public:
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the spline series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     Returns a pointer to the associated series view object.
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

protected:
#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRadarSplineSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartRadarSplineSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
};

//===========================================================================
// Summary:
//     This class represents the view of a spline series, which is a kind of
//     CXTPChartSeriesView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarSplineSeriesView : public CXTPChartRadarLineSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarSplineSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartRadarSplineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the spline series point.
	// Parameters:
	//     pDC    - A pointer to the chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     Returns a pointer to the series point view object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC,
													CXTPChartSeriesPoint* pPoint,
													CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a spline series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the spline series.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTRADARSPLINESERIESSTYLE_H__)
