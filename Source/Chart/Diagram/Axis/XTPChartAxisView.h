// XTPChartAxisView.h
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
#if !defined(__XTPCHARTAXISVIEW_H__)
#	define __XTPCHARTAXISVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDiagram2DAxisView;
class CXTPChartAxisRange;
class CXTPChartAxis;
class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents a tick mark in the axis.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisViewTick
{
public:
	double m_dValue;			// The value of the tick mark.
	CXTPChartString m_strLabel; // The tick mark label.
	CSize m_szBounds;			// The boundary.
	CXTPChartSizeF m_szLabel;   // The size of the label.
};

class _XTP_EXT_CLASS CXTPChartAxisView : public CXTPChartElementView
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisView object.
	// Parameters:
	//     pAxis     - Pointer to a CXTPChartAxis object.
	//     pParentView - Pointer to a CXTPChartElementView object.
	//-----------------------------------------------------------------------
	CXTPChartAxisView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView);
	~CXTPChartAxisView();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Returns the associated axis.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;

public:
	virtual void CreateView(CXTPChartDeviceContext* pDC)				= 0;
	virtual void CalcSize(CXTPChartDeviceContext* pDC, CRect rcDiagram) = 0;
	virtual void SetBounds(CXTPChartDeviceContext* pDC, CRect rcBounds) = 0;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the spacing of the grid lines.
	// Returns:
	//     A double value denoting the grid seperation.
	//-------------------------------------------------------------------------
	double GetGridSpacing() const;

public:
	double GetRangeMinValue() const;
	double GetRangeMaxValue() const;

	double GetViewRangeMinValue() const;
	double GetViewRangeMaxValue() const;

	double GetAxisRangeMinValue() const;
	double GetAxisRangeMaxValue() const;

	double ValueToAxis(double dValue) const;
	double AxisToValue(double dValue) const;

public:
	void AddLegendItems();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function calculates the spacing of the grid lines.
	// Parameters:
	//     nAxisRangeDelta    - The difference between the min and max value of the range.
	//     nScreenDelta       - The length of the diagram. If width is greater than height,
	//                          then nscreenDelta is width and vice versa.
	//     nGridSpacingFactor - The grid spacing factor.
	// Returns:
	//     A double value denoting the grid separation.
	//-------------------------------------------------------------------------
	double CalculateGridSpacing(double nAxisRangeDelta, double nScreenDelta,
								double nGridSpacingFactor);

public:
	CArray<CXTPChartSeriesView*, CXTPChartSeriesView*> m_arrSeries;
	CXTPChartAxis* m_pAxis; // The chart axis.
	double m_dGridSpacing;  // The grid spacing.
};

//===========================================================================
// Summary:
//     This class abstracts the view of a chart axis and its child items in
//     a bounding rectangle context.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram2DAxisView : public CXTPChartAxisView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram2DAxisView object.
	// Parameters:
	//     pAxis     - Pointer to a CXTPChartAxis object.
	//     pParentView - Pointer to a CXTPChartElementView object.
	//-----------------------------------------------------------------------
	CXTPChartDiagram2DAxisView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDiagram2DAxisView object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPChartDiagram2DAxisView();

public:
	CRect GetBounds() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Override this function to create a CXTPChartDeviceCommand object. This object
	//     represents the rendering of an axis in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of an axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the grid lines in a chart axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the grid lines in a chart axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC,
																 CRect rcPane);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the interlaced strips in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the interlaced strips in the chart.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC,
																  CRect rcPane);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the tick marks in the axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the tick marks in the axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateTickMarksDeviceCommand(CXTPChartDeviceContext* pDC);

	virtual CXTPChartDeviceCommand* CreateScrollBarDeviceCommand(CXTPChartDeviceContext* pDC);

	void PerformMouseWheel(short zDelta, CPoint pt);
	void PerformPaneDragging(int dx, int dy);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the labels in the axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the labels in the axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLabelsDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the axis title.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the axis title.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateTitleDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the constant lines perpendicular to an axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the constant lines perpendicular to an axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateConstantLinesDeviceCommand(CXTPChartDeviceContext* pDC,
																	 CRect rcPane, BOOL bBehind);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the strips parallel to an axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the strips parallel to an axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC,
															  CRect rcPane);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the chart axis view.
	// Parameters:
	//     pDC     - The chart device context object.
	// Remarks:
	//     This function creates the view of an axis and its constituent elements
	//     (e.g. tick marks, labels, axis title, etc.).
	//-------------------------------------------------------------------------
	void CreateView(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function calculates the size of an axis, including its constituent elements
	//     (e.g. tick marks, labels, axis title, etc.).
	// Parameters:
	//     pDC       - The chart device context object.
	//     rcDiagram - The rectangle of the diagram.
	//-------------------------------------------------------------------------
	void CalcSize(CXTPChartDeviceContext* pDC, CRect rcDiagram);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function sets the bounds for the axis.
	// Parameters:
	//     pDC       - The chart device context object.
	//     rcBounds  - The bounding rectangle.
	//-------------------------------------------------------------------------
	void SetBounds(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get a point on the axis from a value within the range.
	// Parameters:
	//     nValue - A point within the axis range.
	//-------------------------------------------------------------------------
	double ValueToPoint(double nValue) const;

	double PointToValue(int nPoint) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the distance of a point from the axis origin.
	// Parameters:
	//     nDistance - A point within the axis range.
	//-------------------------------------------------------------------------
	double DistanceToPoint(double nDistance) const;

	double GetScale() const;

	BOOL IsScollBarVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the size of the axis.
	// Returns:
	//     The last computed axis size value. If the axis size value has not been
	//     computed yet, a value of 0 will be returned instead.
	//-------------------------------------------------------------------------
	int GetSize() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function checks if the axis is vertical.
	// Returns:
	//     A BOOL value. TRUE if the axis is vertical, FALSE if the axis is horizontal.
	//-------------------------------------------------------------------------
	BOOL IsVertical() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the axis tick marks.
	// Parameters:
	//     pDC -       The device context.
	//     rcDiagram - The bounding rectangle of the diagram.
	//-------------------------------------------------------------------------
	void CreateTickMarks(CXTPChartDeviceContext* pDC, CRect rcDiagram);

public:
	CArray<CXTPChartAxisViewTick, CXTPChartAxisViewTick&> m_arrTicks; // The collection of tick
																	  // marks.
	CArray<double, double&> m_arrMinorTicks;

protected:
	CRect m_rcBounds; // The diagram boundary.
	int m_nSize;	  // The axis size;

	CPoint m_ptOldPosition;

	CXTPChartRectF m_rcScrollBar;
	CXTPChartRectF m_rcThumb;
};

AFX_INLINE CXTPChartAxis* CXTPChartAxisView::GetAxis() const
{
	return m_pAxis;
}
AFX_INLINE int CXTPChartDiagram2DAxisView::GetSize() const
{
	return m_nSize;
}
AFX_INLINE CRect CXTPChartDiagram2DAxisView::GetBounds() const
{
	return m_rcBounds;
}

AFX_INLINE double CXTPChartAxisView::GetGridSpacing() const
{
	return m_dGridSpacing;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISVIEW_H__)
