// XTPChart2dFunnelSeriesLabel.h
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
#if !defined(__XTPCHART2DFUNNELSERIESLABEL_H__)
#	define __XTPCHART2DFUNNELSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartFunnelSeriesPointView;
class CXTPChartFunnelSeriesStyle;

//===========================================================================
// Summary:
//     CXTPChartFunnelSeriesLabel is a kind of CXTPChartSeriesLabel. This class
//     abstracts the label of a funnel series.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesLabel : public CXTPChartFunnelSeriesLabelBase
{
	DECLARE_SERIAL(CXTPChartFunnelSeriesLabel)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesLabel object.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the basic style of the funnel series.
	// Returns:
	//     A pointer to a CXTPChartFunnelSeriesStyle object.
	//-------------------------------------------------------------------------
	CXTPChartFunnelSeriesStyle* GetStyle() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view of the funnel series.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pPointView - The series point view.
	// Returns:
	//     Returns a pointer to the associated label view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartSeriesPointView* pPointView,
											 CXTPChartElementView* pParentView);
};

//-----------------------------------------------------------------------
// Summary:
//      Defines 2D series label view.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPChartFunnelSeriesLabelView : public CXTPChartFunnelSeriesLabelViewBase
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesLabelView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs a CXTPChartFunnelSeriesLabelView object.
	// Parameters:
	//      pLabel - Associated label model pointer.
	//      pPointView - Point view object pointer associated with the label.
	//      pParentView - Parent view pointer.
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesLabelView(CXTPChartSeriesLabel* pLabel,
								   CXTPChartSeriesPointView* pPointView,
								   CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//      Creates a view drawing device command for the device context specified.
	// Parameters:
	//      pDC - Device context pointer for which the drawing command is to be created.
	// Returns:
	//      A new instance of view drawing device command or NULL if no command
	//      is provided.
	//-----------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART2DFUNNELSERIESLABEL_H__)
