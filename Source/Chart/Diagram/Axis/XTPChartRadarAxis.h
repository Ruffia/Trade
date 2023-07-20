// XTPChartRadarAxis.h
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
#if !defined(__XTPCHARTRADARAXIS_H__)
#	define __XTPCHARTRADARAXIS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartRadarDiagram;

//===========================================================================
// Summary:
//     CXTPChartRadarAxisX is a derived class from the CXTPChartAxis class
//     and represents the X axis of a chart.
// Remarks:
//     It includes the features specific to the X axis of a chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisX : public CXTPChartAxis
{
	DECLARE_DYNAMIC(CXTPChartRadarAxisX)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisX object.
	// Parameters:
	//     pDiagram     - Pointer to a CXTPChartRadarDiagram object.
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisX(CXTPChartRadarDiagram* pDiagram);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method overrides the base class member and determines if
	//     the X axis is vertical.
	// Returns:
	//     Returns TRUE if the X axis is vertical, FALSE if the X axis is not vertical.
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method overrides the base class member and determines if
	//     the X axis is value type.
	// Returns:
	//     Returns TRUE if the X axis is value type, FALSE if the X axis is not value type.
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method overrides the base class member and returns the
	//     side margin value of the chart's X axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart's X axis.
	//     nMaxValue - The maximum value of the chart's X axis.
	// Returns:
	//     Returns a double value that represents the margin.
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the direction of the axis.
	// Returns:
	//     A BOOL value. TRUE if the direction of the axis is clockwise,
	//     FALSE if the direction of the axis is counterclockwise.
	// Remarks:
	//     TRUE/clockwise is the default value for the direction of the axis.
	// See Also:
	//     SetDirectionClockwise
	//-------------------------------------------------------------------------
	BOOL IsDirectionClockwise() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function sets the direction of the axis.
	// Returns:
	//     bDirectionClockwise - A new value for the axis direction.
	//                           TRUE to set the direction of the axis to clockwise,
	//                           FALSE to set the direction of the axis to counterclockwise.
	// Remarks:
	//     TRUE/clockwise is the default value for the direction of the axis.
	// See Also:
	//     IsDirectionClockwise
	//-------------------------------------------------------------------------
	void SetDirectionClockwise(BOOL bDirectionClockwise);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView) of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns a pointer to the newly created CXTPChartAxisView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC,
										  CXTPChartElementView* pParentView);

	BOOL m_bDirectionClockwise; // The direction of the axis. Default is TRUE (clockwise).
};

//===========================================================================
// Summary:
//     CXTPChartRadarAxisY is a derived class from the CXTPChartAxis class
//     and represents the Y axis of a chart.
// Remarks:
//     It includes the features specific to the Y axis of a chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisY : public CXTPChartAxis
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisY object.
	// Parameters:
	//     pDiagram     - Pointer to a CXTPChartRadarDiagram object.
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisY(CXTPChartRadarDiagram* pDiagram);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method overrides the base class member and determines if
	//     the Y axis is vertical.
	// Returns:
	//     Returns TRUE if the Y axis is vertical, FALSE if the Y axis is not vertical.
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method overrides the base class member and determines if
	//     the Y axis is value type.
	// Returns:
	//     Returns TRUE if the Y axis is value type, FALSE if the Y axis is not value type.
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method overrides the base class member and returns the
	//     side margin value of the chart's Y axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart's Y axis.
	//     nMaxValue - The maximum value of the chart's Y axis.
	// Returns:
	//     Returns a double value that represents the margin.
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView) of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns a pointer to the newly created CXTPChartAxisView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC,
										  CXTPChartElementView* pParentView);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTRADARAXIS_H__)
