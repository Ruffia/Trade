// XTPChartLegendView.h
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
#if !defined(__XTPCHARTLEGENDVIEW_H__)
#	define __XTPCHARTLEGENDVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartFont;
class CXTPChartElementView;
class CXTPChartDeviceContext;
class CXTPChartLegend;
class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartBorder;
class CXTPChartFillStyle;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class abstracts the view of a legend and its child items in
//     a bounding rectangle context.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartLegendView : public CXTPChartElementView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartLegendView object.
	// Parameters:
	//     pLegend     - Pointer to a CXTPChartLegend object.
	//     pParentView - Pointer to a CXTPChartElementView object.
	//-----------------------------------------------------------------------
	CXTPChartLegendView(CXTPChartLegend* pLegend, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartLegendView object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartLegendView();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of a legend in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the legend.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function calculates the legend location in the chart based on the state
	//     of the CXTPChartLegend object and adjusts the chart layout accordingly.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcChart - The chart bounding rectangle.
	//-------------------------------------------------------------------------
	void CalculateView(CXTPChartDeviceContext* pDC, CRect& rcChart);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function calculates the legend's width and height based
	//     on the number of legend items present.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     The return value is a CSize object representing the height and width
	//     of the legend.
	//-------------------------------------------------------------------------
	CSize CalcSize(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to add a new legend item.
	// Parameters:
	//     pItem     - Pointer to a CXTPChartLegendItemView object.
	//     minimum   - minimum of the point collection.
	//     maximum   - maximum of the point collection.
	//-------------------------------------------------------------------------
	void AddItem(CXTPChartLegendItemView* pItem, double minimum = 0, double maximum = 0);

protected:
	CXTPChartLegend* m_pLegend; // Pointer to the legend object.
	CRect m_rcBounds;			// The bounding rectangle of the legend.

	struct CHARTLEGEND_ITEM
	{
		CXTPChartLegendItemView* pView;
		double min;
		double max;
		CXTPChartString str;
	};

	CArray<CHARTLEGEND_ITEM, CHARTLEGEND_ITEM> m_arrItems; // The list of legend items.

	static int CompareFuncAsc(const CHARTLEGEND_ITEM* pItem1, const CHARTLEGEND_ITEM* pItem2);
	static int CompareFuncDesc(const CHARTLEGEND_ITEM* pItem1, const CHARTLEGEND_ITEM* pItem2);
	static int CompareFuncMinToMax(const CHARTLEGEND_ITEM* pItem1, const CHARTLEGEND_ITEM* pItem2);
	static int CompareFuncMaxToMin(const CHARTLEGEND_ITEM* pItem1, const CHARTLEGEND_ITEM* pItem2);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTLEGENDVIEW_H__)
