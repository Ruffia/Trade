// XTPChartFunnelSeriesPointViewBase.h
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
#if !defined(__XTPCHARTFUNNELSERIESPOINTVIEWBASE_H__)
#	define __XTPCHARTFUNNELSERIESPOINTVIEWBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesPoint;
class CXTPChartElementView;
class CXTPChartDeviceContext;
class CXTPChartFunnelSeriesView;

//===========================================================================
// Summary:
//     This class abstracts the view of a funnel series point. This class is a kind of
//     CXTPChartSeriesPointView.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesPointViewBase : public CXTPChartSeriesPointView
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesPointViewBase);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesPointView object.
	// Parameters:
	//     pPoint        - Pointer to chart series point.
	//     pParentView   - The pointer to the parent view object.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesPointViewBase(CXTPChartSeriesPoint* pPoint,
									   CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesPointView object.
	// Parameters:
	//     pPoint        - Pointer to chart series point.
	//     pParentView   - The pointer to the parent view object.
	//     bAddToParent  - If TRUE the view will be be added to parent's child view list.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesPointViewBase(CXTPChartSeriesPoint* pPoint,
									   CXTPChartElementView* pParentView, BOOL bAddToParent);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFunnelSeriesPointView object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFunnelSeriesPointViewBase();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to calculate the layout of the funnel series point.
	// Parameters:
	//     nIndex - Point index.
	//     nCount - Number of points.
	//     dFrom - Point start value.
	//     dTo - Value part point value end.
	//-------------------------------------------------------------------------
	virtual void CalculateLayout(int nIndex, int nCount, double dFrom, double dTo);

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains border color value.
	// Returns:
	//      Border color value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetBorderActualColor() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the funnel series point.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the funnel series point.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC) = 0;

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

protected:
	double m_dValue; // The value of the point.

public:
	CXTPChartRectF m_rc;

	double m_dFrom;
	double m_dTo;

protected:
	double m_dWidth;
	double m_dHeight;

	friend class CXTPChartFunnelSeriesViewBase;
	friend class CXTPChartFunnelSeriesView;
	friend class CXTPChart3dFunnelSeriesView;
	friend class CXTPChart3dFunnelSeriesPointView;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTFUNNELSERIESPOINTVIEWBASE_H__)
