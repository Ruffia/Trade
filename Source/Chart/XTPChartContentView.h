// XTPChartContentView.h
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
#if !defined(__XTPCHARTCONTENTVIEW_H__)
#	define __XTPCHARTCONTENTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDeviceContext;
class CXTPChartDiagram;
class CXTPChartTitleCollection;
class CXTPChartElementView;
class CXTPChartSeriesCollection;
class CXTPChartLegend;
class CXTPChartPanelCollection;
class CXTPChartSeries;
class CXTPChartAppearance;
class CXTPChartLegendView;
class CXTPPropExchange;
class CXTPMarkupContext;
class CXTPChartContentView;
class CXTPChartDiagramView;
class CXTPChartBorder;
class CXTPChartDeviceCommand;

//===========================================================================
// Summary:
//     This class represents the view of the chart content, which is a kind of
//     CXTPChartElementView.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartContentView : public CXTPChartElementView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartContentView object.
	// Parameters:
	//     pContainer - Pointer to a chart container object.
	//     pContent - Pointer to a chart content object.
	//-----------------------------------------------------------------------
	CXTPChartContentView(CXTPChartContainer* pContainer, CXTPChartContent* pContent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create various child view objects which
	//     constitute the view of the chart content.
	// Parameters:
	//     pDC - Pointer to a chart device context.
	//-----------------------------------------------------------------------
	void CreateView(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the view/layout of the chart content
	//     with respect to a bounding rectangle.
	// Parameters:
	//     pDC      - Pointer to a chart device context.
	//     rcBounds - The bounding rectangle.
	//-----------------------------------------------------------------------
	void CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend view object associated with this
	//     object.
	// Returns:
	//     Pointer to a chart legend view object.
	//-----------------------------------------------------------------------
	CXTPChartLegendView* GetLegendView() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine the current DiagramView under point.
	// Parameters:
	//     pt - Position to hit test.
	// Returns:
	//     Pointer to a CXTPChartDiagramView view object.
	//-----------------------------------------------------------------------
	CXTPChartDiagramView* HitTestDiagramView(CPoint pt) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the device command for this object.
	// Parameters:
	//     pDC - The device context.
	// Returns:
	//     A pointer to a chart device context object.
	//-----------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

protected:
	CRect m_rcBounds;			  // The bounds.
	CXTPChartContent* m_pContent; // The chart content.

public:
	CXTPChartLegendView* m_pLegendView;   // The legend view.
	CXTPChartElementView* m_pTitlesView;  // The titles view.
	CXTPChartElementView* m_pDiagramView; // The diagram view.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTCONTENTVIEW_H__)
