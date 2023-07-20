// XTPChartBarSeriesErrorBar.h
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
#if !defined(__XTPCHARTBARSERIESERRORBAR_H__)
#	define __XTPCHARTBARSERIESERRORBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;

class _XTP_EXT_CLASS CXTPChartBarSeriesErrorBar : public CXTPChartErrorBar
{
	DECLARE_SERIAL(CXTPChartBarSeriesErrorBar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesErrorBar object.
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesErrorBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBarSeriesErrorBar object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBarSeriesErrorBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function creates the view of the bar series label.
	// Parameters:
	//     pDC -        The device context.
	//     pPointView - A pointer to the series point view object.
	//     pParentView - The pointer to the parent view object.
	// Returns:
	//     A pointer to a CXTPChartElementView object, which is polymorphic to
	//     CXTPChartBarSeriesLabelView.
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
	DECLARE_OLETYPELIB_EX(CXTPChartBarSeriesErrorBar);
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     This class abstracts the view of the label of a bar series. It is a kind of point
//     series label view.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBarSeriesErrorBarView : public CXTPChartErrorBarView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesErrorBarView object.
	// Parameters:
	//     pLabel     - Pointer chart series label.
	//     pPointView - Pointer to chart series point view.
	//     pParentView - The pointer to the parent view object.
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesErrorBarView(CXTPChartErrorBar* pErrorBar,
								   CXTPChartSeriesPointView* pPointView,
								   CXTPChartElementView* pParentView);

	// virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext *pDC);
	void CalculateLayout(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the anchor point of the error bar.
	//-----------------------------------------------------------------------
	// virtual CXTPChartPointF GetAnchorPoint(XTPChartErrorBarPosition position) const;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTBARSERIESERRORBAR_H__)
