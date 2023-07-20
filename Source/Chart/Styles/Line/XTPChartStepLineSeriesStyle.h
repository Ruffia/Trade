// XTPChartStepLineSeriesStyle.h
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
#if !defined(__XTPCHARTSTEPLINESERIESSTYLE_H__)
#	define __XTPCHARTSTEPLINESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartStepLineStyle;

//===========================================================================
// Summary:
//     This class represents a StepLine series style, which is a kind of
//     CXTPChartLineSeriesStyle.
// Remarks:
//     A Step Line chart or StepLine graph is a type of graph, which displays information
//     as a series of data points connected by straight Step Line segments. It is a
//     basic type of chart common in many fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStepLineSeriesStyle : public CXTPChartLineSeriesStyle
{
	DECLARE_SERIAL(CXTPChartStepLineSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStepLineSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartStepLineSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartStepLineSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartStepLineSeriesStyle();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets inverted step flag.
	// Parameters:
	//      bInvertedStep - TRUE to invert step, otherwise FALSE.
	// Returns:
	//      Current inverted step flag value.
	//-----------------------------------------------------------------------
	BOOL GetInvertedStep() const;
	void SetInvertedStep(BOOL bInvertedStep); // <combine
											  // CXTPChartStepLineSeriesStyle::GetInvertedStep>

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the StepLine series.
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
	DECLARE_OLETYPELIB_EX(CXTPChartStepLineSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartStepLineSeriesStyle)
	LPDISPATCH OleGetStepLineStyle();
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bInvertedStep;
};

//===========================================================================
// Summary:
//     This class represents the view of a StepLine series, which is a kind of
//     CXTPChartSeriesView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStepLineSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStepLineSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartStepLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a view of the StepLine series point.
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
	//     represents the rendering of a StepLine series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the StepLine series.
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
};

//===========================================================================
// Summary:
//     This class represents the view of a step line series point, which is a kind of
//     CXTPChartSeriesPointView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStepLineSeriesPointView : public CXTPChartPointSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs a point view object.
	// Parameters:
	//      pPoint - Associated point object pointer.
	//      pParentView - Parent view object pointer.
	//-----------------------------------------------------------------------
	CXTPChartStepLineSeriesPointView(CXTPChartSeriesPoint* pPoint,
									 CXTPChartElementView* pParentView);

public:
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
};

AFX_INLINE BOOL CXTPChartStepLineSeriesStyle::GetInvertedStep() const
{
	return m_bInvertedStep;
}
AFX_INLINE void CXTPChartStepLineSeriesStyle::SetInvertedStep(BOOL bInvertedStep)
{
	m_bInvertedStep = bInvertedStep;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTSTEPLINESERIESSTYLE_H__)
