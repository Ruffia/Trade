// XTPChartBubbleSeriesStyle.h
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
#if !defined(__XTPCHARTBUBBLESERIESSTYLE_H__)
#	define __XTPCHARTBUBBLESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents a bubble series style, which is a kind of
//     CXTPChartPointSeriesStyle.
// Remarks:
//     A bubble chart is a type of chart where each plotted entity is defined
//     in terms of three distinct numeric parameters. Bubble charts can facilitate
//     the understanding of the social, economical, medical, and other scientific
//     relationships.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesStyle : public CXTPChartPointSeriesStyle
{
	DECLARE_SERIAL(CXTPChartBubbleSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBubbleSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBubbleSeriesStyle();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets the miminum bubble size value.
	// Parameters:
	//      dMinSize - The miminum bubble size value.
	// Returns:
	//      The current minimum bubble size value.
	//-----------------------------------------------------------------------
	double GetMinSize() const;
	void SetMinSize(double dMinSize); // <combine CXTPChartBubbleSeriesStyle::GetMinSize>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets the maximum bubble size value.
	// Parameters:
	//      dMinSize - The maximum bubble size value.
	// Returns:
	//      The current maximum bubble size value.
	//-----------------------------------------------------------------------
	double GetMaxSize() const;
	void SetMaxSize(double dMinSize); // <combine CXTPChartBubbleSeriesStyle::GetMaxSize>

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the index of a point element for which an internal value
	//      can be otained.
	// Returns:
	//      The index of a point element for which an internal value
	//      can be otained.
	//-----------------------------------------------------------------------
	virtual int GetSeriesPointValueIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the bubble series.
	// Parameters:
	//     pSeries      - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     Returns a pointer to the associated series view object.
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to correct the axis side margins.
	// Parameters:
	//     pAxis        - A pointer to the chart axis object to which this object
	//                    is associated with.
	//     nMinValue    - A double specifying the minimum value.
	//     nMaxValue    - A double specifying the maximum value.
	//     nCorrection  - A reference to a double object which specifies the correction
	//                    applied.
	// Returns:
	//     A pointer to CXTPChartSeriesView. Refers to a newly created
	//     CXTPChartStackedBarSeriesView object.
	//-------------------------------------------------------------------------
	void CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue, double nMaxValue,
								double& nCorrection);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartBubbleSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartBubbleSeriesStyle)

	double OleGetMinSize();
	void OleSetMinSize(double dMinSize);

	double OleGetMaxSize();
	void OleSetMaxSize(double dMinSize);

//}}AFX_CODEJOCK_PRIVATE
#	endif

	using CXTPChartPointSeriesStyle::m_nTransparency;

	double m_dMinSize;
	double m_dMaxSize;
};

//===========================================================================
// Summary:
//     This class represents the view of a bubble series, which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//     A bubble chart is a type of chart where each plotted entity is defined
//     in terms of three distinct numeric parameters. Bubble charts can facilitate
//     the understanding of the social, economical, medical, and other scientific
//     relationships.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesView : public CXTPChartPointSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the bubble series point.
	// Parameters:
	//     pDC      - A pointer to chart device context.
	//     pPoint   - A pointer to the chart series point object.
	// Returns:
	//     Returns a pointer to the associated series point view object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC,
													CXTPChartSeriesPoint* pPoint,
													CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a bubble series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the bubble series.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Creates the view object.
	// Parameters:
	//      pDC - Target device context pointers.
	//-----------------------------------------------------------------------
	void CreatePointsView(CXTPChartDeviceContext* pDC);

	double m_dMinValue;
	double m_dMaxValue;

	friend class CXTPChartBubbleSeriesPointView;
};

//===========================================================================
// Summary:
//     This class represents the view of a bubble series point, which is a kind of
//     CXTPChartPointSeriesPointView.
// Remarks:
//     A bubble chart is a type of chart where each plotted entity is defined
//     in terms of three distinct numeric parameters. Bubble charts can facilitate
//     the understanding of the social, economical, medical, and other scientific
//     relationships.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesPointView : public CXTPChartPointSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesPointView object.
	// Parameters:
	//     pPoint       - A pointer to the chart series point object.
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a bubble series point.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartPolygonAntialiasingDeviceCommand object. This
	//     object handles the rendering of an element in the chart. Here
	//     it handles the drawing of the bubble series point.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual color.
	// Returns:
	//     A CXTPChartColor object representing an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the second actual color.
	// Returns:
	//     A CXTPChartColor object representing an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor2() const;
};

AFX_INLINE double CXTPChartBubbleSeriesStyle::GetMinSize() const
{
	return m_dMinSize;
}
AFX_INLINE void CXTPChartBubbleSeriesStyle::SetMinSize(double dMinSize)
{
	m_dMinSize = dMinSize;
	OnChartChanged();
}
AFX_INLINE double CXTPChartBubbleSeriesStyle::GetMaxSize() const
{
	return m_dMaxSize;
}
AFX_INLINE void CXTPChartBubbleSeriesStyle::SetMaxSize(double dMaxSize)
{
	m_dMaxSize = dMaxSize;
	OnChartChanged();
}
AFX_INLINE int CXTPChartBubbleSeriesStyle::GetSeriesPointValueIndex() const
{
	return 1;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTBUBBLESERIESSTYLE_H__)
