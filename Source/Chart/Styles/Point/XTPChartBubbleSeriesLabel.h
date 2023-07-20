// XTPChartBubbleSeriesLabel.h
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
#if !defined(__XTPCHARTBUBBLESERIESLABEL_H__)
#	define __XTPCHARTBUBBLESERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     CXTPChartBubbleSeriesLabel is a kind of CXTPChartSeriesLabel. This class
//     abstracts the label of a bubble series.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesLabel : public CXTPChartSeriesLabel
{
	DECLARE_SERIAL(CXTPChartBubbleSeriesLabel);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesLabel object.
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBubbleSeriesLabel object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBubbleSeriesLabel();

public:
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartBubbleSeriesLabelView) of the
	//     bubble series.
	// Parameters:
	//     pDC        - The chart device context object pointer.
	//     pPointView - The seris point view.
	// Returns:
	//     Returns a pointer to the associated label view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartSeriesPointView* pPointView,
											 CXTPChartElementView* pParentView);

protected:
};

//===========================================================================
// Summary:
//     CXTPChartBubbleSeriesLabelView is a kind of CXTPChartSeriesLabelView. This class
//     abstracts the view of a bubble series label.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesLabelView : public CXTPChartDiagram2DSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesLabelView object.
	// Parameters:
	//     pLabel     - A pointer to the chart series label object.
	//     pPointView - A pointer to the chart series point view object.
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesLabelView(CXTPChartSeriesLabel* pLabel,
								   CXTPChartSeriesPointView* pPointView,
								   CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a bubble series label.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the bubble series label.
	//-------------------------------------------------------------------------
	using CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand;

	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	void CalculateLayout(CXTPChartDeviceContext* pDC);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTBUBBLESERIESLABEL_H__)
