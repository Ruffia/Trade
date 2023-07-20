// XTPChart3dFunnelSeriesPointView.h
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
#if !defined(__XTPCHART3DPYPAMIDSERIESPOINTVIEW_H__)
#	define __XTPCHART3DFUNNELSERIESPOINTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

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
class _XTP_EXT_CLASS CXTPChart3dFunnelSeriesPointView
	: public CXTPChartFunnelSeriesPointViewBase
	, private CXTPChart3dFunnelDeviceCommandEvents
{
	DECLARE_DYNAMIC(CXTPChart3dFunnelSeriesPointView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dFunnelSeriesPointView object.
	// Parameters:
	//     pPoint        - Pointer to chart series point.
	//     pParentView   - Parent element view pointer.
	//-----------------------------------------------------------------------
	CXTPChart3dFunnelSeriesPointView(CXTPChartSeriesPoint* pPoint,
									 CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles view destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPChart3dFunnelSeriesPointView();

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
	//      Obtains the shape center connector point.
	// Returns:
	//      Shape center connector point.
	// See also:
	//      GetOuterCenterConnectorPoint
	//-----------------------------------------------------------------------
	const CXTPPoint3d& GetCenterShapeConnectorPoint() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the outer shape center connector point.
	// Returns:
	//      Outer shape center connector point.
	// See also:
	//      GetCenterShapeConnectorPoint
	//-----------------------------------------------------------------------
	const CXTPPoint3d& GetOuterCenterConnectorPoint() const;

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

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Notifies about updated boundaries of the command result.
	// Parameters:
	//      pCommand - Sender command pointer.
	//      rcBounds - Updated bounray rectangle.
	//-----------------------------------------------------------------------
	virtual void OnUpdateBounds(CXTPChartDeviceCommand* pCommand, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets called when funnel connector points are computed.
	// Parameters:
	//      pCommand - The sender command.
	//      ptShapeCenter - The coordinates of the physical shape center.
	//      ptOuterCenter - The coordinates of the most outer horizontal shape center point.
	//-----------------------------------------------------------------------
	virtual void OnSetConnectorPoints(CXTPChartDeviceCommand* pCommand, CXTPPoint3d ptShapeCenter,
									  CXTPPoint3d ptOuterCenter);

private:
	CXTPChartFunnelSeriesPointViewBase* m_p2dSeriesPointView; // Used for drawing 2D legend items
	CXTPPoint3d m_ptShapeCenter;
	CXTPPoint3d m_ptOuterCenter;
	CXTPSubscriptionList<CXTPChartDeviceCommand, CXTPChartDeviceCommandEvents>*
		m_pCommandSubscriptionList;

	// double m_dWidth;
	// double m_dHeight;
	// double m_dFrom;
	// double m_dTo;
};

AFX_INLINE const CXTPPoint3d& CXTPChart3dFunnelSeriesPointView::GetCenterShapeConnectorPoint() const
{
	return m_ptShapeCenter;
}

AFX_INLINE const CXTPPoint3d& CXTPChart3dFunnelSeriesPointView::GetOuterCenterConnectorPoint() const
{
	return m_ptOuterCenter;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DFUNNELSERIESPOINTVIEW_H__)
