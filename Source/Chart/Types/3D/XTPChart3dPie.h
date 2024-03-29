// XTPChart3dPie.h
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
#if !defined(__XTPCHART3DPIE_H__)
#	define __XTPCHART3DPIE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDeviceCommand;
class CXTPChartEllipse;

//===========================================================================
// Summary:
//     This class abstracts a pie chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChart3dPie : public CXTPChartPieBase
{
	DECLARE_DYNAMIC(CXTPChart3dPie);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dPie object.
	// Parameters:
	//     dStartAngle      - The start angle of the pie.
	//     dFinishAngle     - The finish angle of the pie.
	//     ellipse          - The ellipse associated with the pie.
	//     nDepthPercent    - The depth percentage, in case of 3D chart.
	//     nHolePercent     - The hole percentage.
	//     bStartGap        - TRUE if the point starts with a gap between points.
	//     bEndGap          - TRUE if the point ends with a gap between points.
	//-----------------------------------------------------------------------
	CXTPChart3dPie(double dStartAngle, double dFinishAngle, const CXTPChartEllipse& ellipse,
				   int nDepthPercent, int nHolePercent, BOOL bStartGap = FALSE,
				   BOOL bEndGap = FALSE);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the pie.
	// Parameters:
	//     color      - The first color used for gradient pie.
	//     color2     - The second color used for gradient pie.
	//     basePoint  - Reference to a CXTPChartPointF object.
	//     nThickness - Thickness.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the pie.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreatePieDeviceCommand(const CXTPChartColor& color,
														   const CXTPChartColor& color2,
														   const CXTPChartPointF& basePoint);
	virtual CXTPChartDeviceCommand* CreateBoundedPieDeviceCommand(
		const CXTPChartColor& color, int nThickness,
		const CXTPChartPointF& basePoint); // <combine CXTPChart3dPie::CreatePieDeviceCommand@const
										   // CXTPChartColor&@const CXTPChartColor&@const
										   // CXTPChartPointF&>

private:
	BOOL m_bStartGap;
	BOOL m_bEndGap;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DPIE_H__)
