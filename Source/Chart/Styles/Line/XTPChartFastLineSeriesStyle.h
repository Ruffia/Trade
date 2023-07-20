// XTPChartFastLineSeriesStyle.h
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
#if !defined(__XTPCHARTFASTLINESERIESSTYLE_H__)
#	define __XTPCHARTFASTLINESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     This class represents a line series style, which is a kind of
//     CXTPChartPointSeriesStyle.
// Remarks:
//     A line chart or line graph is a type of graph which displays information
//     as a series of data points connected by straight line segments. It is a
//     basic type of chart common in many fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFastLineSeriesStyle : public CXTPChartDiagram2DSeriesStyle
{
	DECLARE_SERIAL(CXTPChartFastLineSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFastLineSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartFastLineSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFastLineSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFastLineSeriesStyle();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the line style of the line series.
	// Returns:
	//     A pointer to a chart line style object.
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetLineStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Enables/disables antialiasing if supported.
	// Parameters:
	//      bEnable - TRUE to enable antialiasing, FALSE to disable.
	// Returns:
	//      TRUE if antialiasing is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetAntialiasing() const;
	void SetAntialiasing(BOOL bEnable = TRUE); // <combine
											   // CXTPChartFastLineSeriesStyle::GetAntialiasing>

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the line series.
	// Parameters:
	//     pSeries - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     Returns a pointer to the associated series view object.
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFastLineSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartFastLineSeriesStyle)
	LPDISPATCH OleGetLineStyle();
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CXTPChartLineStyle* m_pLineStyle; // The line style.
	BOOL m_bAntialiasing;
};

AFX_INLINE BOOL CXTPChartFastLineSeriesStyle::GetAntialiasing() const
{
	return m_bAntialiasing;
}

AFX_INLINE void CXTPChartFastLineSeriesStyle::SetAntialiasing(BOOL bAntialiasing)
{
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}

//===========================================================================
// Summary:
//     This class represents the view of a line series, which is a kind of
//     CXTPChartSeriesView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFastLineSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartLineSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartFastLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the line series point.
	// Parameters:
	//     pDC    - A pointer to the chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     Returns a pointer to the associated series point view object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC,
													CXTPChartSeriesPoint* pPoint,
													CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a line series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the line series.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

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
															  CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//      Creates the view object.
	// Parameters:
	//      pDC - Target device context pointers.
	//-----------------------------------------------------------------------
	void CreatePointsView(CXTPChartDeviceContext* pDC);
};

AFX_INLINE CXTPChartLineStyle* CXTPChartFastLineSeriesStyle::GetLineStyle() const
{
	return m_pLineStyle;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTFASTLINESERIESSTYLE_H__)
