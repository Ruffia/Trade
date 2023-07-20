// XTPChartFunnelSeriesStyleBase.h
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
#if !defined(__XTPCHARTFUNNELSERIESSTYLEBASE_H__)
#	define __XTPCHARTFUNNELSERIESSTYLEBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     This class abstracts the basic style of a funnel serie.This class is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//     A funnel chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a funnel chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesStyleBase : public CXTPChartSeriesStyle
{
	DECLARE_DYNAMIC(CXTPChartFunnelSeriesStyleBase)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesStyleBase object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesStyleBase();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFunnelSeriesStyleBase object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFunnelSeriesStyleBase();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets point distance value.
	// Parameters:
	//      nPointDistance - Distance value.
	// Returns:
	//      Current distance value.
	//-----------------------------------------------------------------------
	int GetPointDistance() const;
	void SetPointDistance(int nPointDistance); // <combine
											   // CXTPChartFunnelSeriesStyleBase::GetPointDistance>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets height to width ratio of the funnel.
	// Parameters:
	//      dRatio - Ratio value.
	// Returns:
	//      Current ratio value.
	//-----------------------------------------------------------------------
	double GetHeightToWidthRatio() const;
	void SetHeightToWidthRatio(
		double dRatio); // <combine CXTPChartFunnelSeriesStyleBase::GetHeightToWidthRatio>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets equal height flag.
	// Parameters:
	//      bEqualHeight - TRUE to make funnel points to have equal heights, otherwise FALSE.
	// Returns:
	//      Current equal height flag.
	//-----------------------------------------------------------------------
	void SetEqualHeight(BOOL bEqualHeight);
	BOOL IsEqualHeight() const; // <combine CXTPChartFunnelSeriesStyleBase::SetEqualHeight>

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the background fill style.
	// Returns:
	//     Returns the pointer to CXTPChartFillStyle object, which abstracts
	//     various fill styles.
	//-------------------------------------------------------------------------
	CXTPChartFillStyle* GetFillStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this function to get the border object.
	// Returns:
	//      Border object pointer.
	//-----------------------------------------------------------------------
	CXTPChartBorder* GetBorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to allow style create diagram object
	// Returns:
	//     New diagram object that will contain series.
	//-------------------------------------------------------------------------
	CXTPChartDiagram* CreateDiagram() = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called to create new view associated with current style.
	// Parameters:
	//     pSeries - Series associated with style
	//     pDiagramView - Parent diagram view
	// Returns:
	//     Pointer to new CXTPChartSeriesView object.
	//----------------------------------------------------------------------
	CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
									CXTPChartDiagramView* pDiagramView) = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to ask style if series can be inserted in existing diagram
	// Parameters:
	//     pDiagram - Diagram to check
	// Returns:
	//     TRUE if Series can be inserted to Diagram
	//-------------------------------------------------------------------------
	BOOL IsStyleDiagram(CXTPChartDiagram* pDiagram) const;

	//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_ACTIVEX
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
#	endif
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	double m_dHeightToWidthRatio;
	int m_nPointDistance;
	CXTPChartBorder* m_pBorder;
	CXTPChartFillStyle* m_pFillStyle;
	BOOL m_bEqualHeight;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE int CXTPChartFunnelSeriesStyleBase::GetPointDistance() const
{
	return m_nPointDistance;
}
AFX_INLINE void CXTPChartFunnelSeriesStyleBase::SetPointDistance(int nPointDistance)
{
	m_nPointDistance = nPointDistance;
	OnChartChanged();
}
AFX_INLINE CXTPChartFillStyle* CXTPChartFunnelSeriesStyleBase::GetFillStyle() const
{
	return m_pFillStyle;
}
AFX_INLINE CXTPChartBorder* CXTPChartFunnelSeriesStyleBase::GetBorder() const
{
	return m_pBorder;
}
AFX_INLINE void CXTPChartFunnelSeriesStyleBase::SetHeightToWidthRatio(double dRatio)
{
	m_dHeightToWidthRatio = dRatio;
	OnChartChanged();
}
AFX_INLINE double CXTPChartFunnelSeriesStyleBase::GetHeightToWidthRatio() const
{
	return m_dHeightToWidthRatio;
}
AFX_INLINE void CXTPChartFunnelSeriesStyleBase::SetEqualHeight(BOOL bEqualHeight)
{
	m_bEqualHeight = bEqualHeight;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartFunnelSeriesStyleBase::IsEqualHeight() const
{
	return m_bEqualHeight;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTFUNNELSERIESSTYLEBASE_H__)
