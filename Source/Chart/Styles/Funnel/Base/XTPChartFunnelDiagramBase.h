// XTPChartFunnelDiagramBase.h
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
#if !defined(__XTPCHARTFUNNELDIAGRAMBASE_H__)
#	define __XTPCHARTFUNNELDIAGRAMBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPChartFunnelDiagramBaseImpl
{
protected:
	virtual ~CXTPChartFunnelDiagramBaseImpl()
	{
	}

	virtual void CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView);
};

//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class abstracts the funnel diagram. This class is a kind of
//     CXTPChartDiagram.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
template<class BaseDiagram>
class CXTPChartFunnelDiagramBase
	: public BaseDiagram
	, private CXTPChartFunnelDiagramBaseImpl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelDiagramBase object.
	//-----------------------------------------------------------------------
	CXTPChartFunnelDiagramBase();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the funnel diagram.
	// Parameters:
	//     pDC           - A pointer to the chart device context.
	//     pParent       - A pointer to the parent view.
	// Returns:
	//     A pointer to a CXTPChartDiagramView object which points to the newly created
	//     CXTPChartFunnelDiagramView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartElementView* pParent) = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the funnel series layout.
	// Parameters:
	//     pDC           - A pointer to the chart device context.
	//     pView         - A pointer to the diagram view.
	//-------------------------------------------------------------------------
	virtual void CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView);
};

template<class BaseDiagram>
AFX_INLINE CXTPChartFunnelDiagramBase<BaseDiagram>::CXTPChartFunnelDiagramBase()
{
}

template<class BaseDiagram>
AFX_INLINE void CXTPChartFunnelDiagramBase<BaseDiagram>::CalculateSeriesLayout(
	CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView)
{
	CXTPChartFunnelDiagramBaseImpl::CalculateSeriesLayout(pDC, pView);
}

typedef CXTPChartFunnelDiagramBase<CXTPChart2dDiagram> CXTPChart2dFunnelDiagramBase;
typedef CXTPChartFunnelDiagramBase<CXTPChart3dDiagram> CXTPChart3dFunnelDiagramBase;

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTFUNNELDIAGRAMBASE_H__)
