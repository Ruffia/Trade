// XTPChartRadarAxisXView.h
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
#if !defined(__XTPCHARTRADARAXISXVIEW_H__)
#	define __XTPCHARTRADARAXISXVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartRadarAxisXView;
class CXTPChartAxisRange;
class CXTPChartAxis;
class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents a tick mark in the axis.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisXViewTick
{
public:
	double m_dValue;			// The value of the tick mark.
	CXTPChartString m_strLabel; // The tick mark label.
	CSize m_szBounds;			// The boundary.
	CXTPChartSizeF m_szLabel;   // The size of the label.
	CXTPChartPointF m_ptLocation;
};
//===========================================================================
// Summary:
//     This class abstracts the view of a chart axis and its child items in
//     a bounding rectangle context.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisXView : public CXTPChartRadarAxisView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarAxisXView object.
	// Parameters:
	//     pAxis     - Pointer to a CXTPChartAxis object.
	//     pParentView - Pointer to a CXTPChartElementView object.
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisXView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarAxisXView object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPChartRadarAxisXView();

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
	//     represents the rendering of the grid lines in the chart axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to a CXTPChartDeviceCommand object. This polymorphic object
	//     handles the rendering of an element in the chart. Here it handles the drawing
	//     of the grid lines in an axis.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC);

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
	virtual CXTPChartDeviceCommand* CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC);

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
																	 BOOL bBehind);

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
	virtual CXTPChartDeviceCommand* CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create a chart axis view.
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

	double ValueToAngle(double nValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the size of the axis.
	// Returns:
	//     The last computed axis size value. If the axis size value has not been
	//     computed yet, a value of 0 will be returned instead.
	//-------------------------------------------------------------------------
	int GetSize() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the axis tick marks.
	// Parameters:
	//     pDC -       The device context.
	//-------------------------------------------------------------------------
	void CreateTickMarks(CXTPChartDeviceContext* pDC);

public:
	CArray<CXTPChartRadarAxisXViewTick, CXTPChartRadarAxisXViewTick&> m_arrTicks; // The collection
																				  // of tick marks.

	CArray<double, double&> m_arrMinorTicks;

protected:
	CRect m_rcBounds; // The diagram boundary.
	int m_nSize;	  // The axis size;

	double m_dMinValue;
	double m_dMaxValue;

	CXTPPoint2i m_ptCenter;
	int m_nRadius;
};

AFX_INLINE int CXTPChartRadarAxisXView::GetSize() const
{
	return m_nSize;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTRADARAXISXVIEW_H__)
