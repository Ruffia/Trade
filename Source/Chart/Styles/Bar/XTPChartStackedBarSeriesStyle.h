// XTPChartStackedBarSeriesStyle.h
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
#if !defined(__XTPCHARTSTACKEDBARSERIESSTYLE_H__)
#	define __XTPCHARTSTACKEDBARSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents a bar series style for the chart, which is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//    A bar chart or bar graph is a chart with rectangular bars with lengths
//    proportional to the values that they represent. Bar charts are used for
//    comparing two or more values that were taken over time or on different
//    conditions, usually on small data sets.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStackedBarSeriesStyle : public CXTPChartBarSeriesStyle
{
	DECLARE_SERIAL(CXTPChartStackedBarSeriesStyle)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStackedBarSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartStackedBarSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartStackedBarSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartStackedBarSeriesStyle();

public:
	void SetStackHeight(double dLevel = 100);
	double GetStackHeight() const;

	void SetStackGroup(int nStackGroup);
	int GetStackGroup() const;

public:
public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the bar series.
	// Parameters:
	//     pSeries      - A pointer to the chart series object to which this object
	//                    is associated with.
	//     pDiagramView - A pointer to the chart diagram view object to which this object
	//                    is associated with.
	// Returns:
	//     A pointer to CXTPChartSeriesView. Refers to the newly created
	//     CXTPChartStackedBarSeriesView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to correct the axis side margins.
	// Parameters:
	//     pAxis        - A pointer to the chart axis object, to which this object
	//                    is associated with.
	//     nMinValue    - A double specifying the minimum value.
	//     nMaxValue    - A double specifying the maximum value.
	//     nCorrection  - A reference to a double object, which specifies the correction
	//                    applied.
	//-------------------------------------------------------------------------
	virtual void CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue, double nMaxValue,
										double& nCorrection);

protected:
	double m_dStackHeight;
	int m_nStackGroup;

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartStackedBarSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartStackedBarSeriesStyle)

	double OleGetStackHeight();
	void OleSetStackHeight(double dStackHeight);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     This class represents the view of a bar series, which is a kind of
//     CXTPChartSeriesView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStackedBarSeriesView : public CXTPChartBarSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStackedBarSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartStackedBarSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);
	virtual ~CXTPChartStackedBarSeriesView();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the bar series point.
	// Parameters:
	//     pDC    - A pointer to the chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartSeriesPointView, which refers to the newly created
	//     CXTPChartStackedBarSeriesPointView object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC,
													CXTPChartSeriesPoint* pPoint,
													CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the layout of the bar series.
	// Parameters:
	//     pDC    - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	void UpdateRange(CXTPChartDeviceContext* pDC);

protected:
	friend class CXTPChartStackedBarSeriesPointView;
};

//===========================================================================
// Summary:
//     This class represents the view of a bar series point, which is a kind of
//     CXTPChartPointSeriesPointView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStackedBarSeriesPointView : public CXTPChartBarSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStackedBarSeriesPointView object.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	//-----------------------------------------------------------------------
	CXTPChartStackedBarSeriesPointView(CXTPChartSeriesPoint* pPoint,
									   CXTPChartElementView* pParentView);

public:
	CXTPChartRectF GetScreenRect() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a bar series point.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles the
	//     rendering of the bar series point.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:
protected:
	double m_dValueFrom;
	double m_dValueTo;
	friend class CXTPChartStackedBarSeriesView;
};

AFX_INLINE void CXTPChartStackedBarSeriesStyle::SetStackHeight(double dStackHeight)
{
	m_dStackHeight = dStackHeight;
	OnChartChanged();
}
AFX_INLINE double CXTPChartStackedBarSeriesStyle::GetStackHeight() const
{
	return m_dStackHeight;
}

AFX_INLINE void CXTPChartStackedBarSeriesStyle::SetStackGroup(int nStackGroup)
{
	m_nStackGroup = nStackGroup;
	OnChartChanged();
}
AFX_INLINE int CXTPChartStackedBarSeriesStyle::GetStackGroup() const
{
	return m_nStackGroup;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTSTACKEDBARSERIESSTYLE_H__)
