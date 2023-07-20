// XTPChartPointSeriesErrorBar.h
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
#if !defined(__XTPCHARTPOINTSERIESERRORBAR_H__)
#	define __XTPCHARTPOINTSERIESERRORBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;

class _XTP_EXT_CLASS CXTPChartPointSeriesErrorBar : public CXTPChartErrorBar
{
	DECLARE_SERIAL(CXTPChartPointSeriesErrorBar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesErrorBar object.
	//-----------------------------------------------------------------------
	CXTPChartPointSeriesErrorBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPointSeriesErrorBar object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPointSeriesErrorBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function creates the view of the bar series label.
	// Parameters:
	//     pDC -        The device context.
	//     pPointView - A pointer to the series point view object.
	// Returns:
	//     Returns a pointer to the associated label view object.
	//-----------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartSeriesPointView* pPointView,
											 CXTPChartElementView* pParentView);

	void DoPropExchange(CXTPPropExchange* pPX);

public:
#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPointSeriesErrorBar);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     This class abstracts the view of the label of a bar series. It is a kind of point
//     series label view.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPointSeriesErrorBarView : public CXTPChartErrorBarView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesErrorBarView object.
	// Parameters:
	//     pLabel     - Pointer to chart series label.
	//     pPointView - Pointer to chart series point view.
	//-----------------------------------------------------------------------
	CXTPChartPointSeriesErrorBarView(CXTPChartErrorBar* pErrorBar,
									 CXTPChartSeriesPointView* pPointView,
									 CXTPChartElementView* pParentView);

	void CalculateLayout(CXTPChartDeviceContext* pDC);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTPOINTSERIESERRORBAR_H__)
