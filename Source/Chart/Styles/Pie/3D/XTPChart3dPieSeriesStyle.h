// XTPChart3dPieSeriesStyle.h
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
#if !defined(__XTPCHART3DPIESERIESSTYLE_H__)
#	define __XTPCHART3DPIESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDiagramView;
class CXTPChartSeries;

//===========================================================================
// Summary:
//     This class abstracts the pie series style. This class is a kind of
//     CXTPChartPieSeriesStyleBase.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChart3dPieSeriesStyle : public CXTPChartPieSeriesStyleBase
{
	DECLARE_SERIAL(CXTPChart3dPieSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dPieSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChart3dPieSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the pie diagram object.
	// Returns:
	//     A pointer to a CXTPChartDiagram object which points to the newly created
	//     CXTPChart3dPieDiagramBase object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagram* CreateDiagram();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the pie series.
	// Parameters:
	//     pSeries       - A pointer to the chart series  object.
	//     pDiagramView  - A pointer to the diagram view object.
	// Returns:
	//     Returns a pointer to the associated series view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries,
											CXTPChartDiagramView* pDiagramView);

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains rotation parameters for 3D chart.
	// Returns:
	//      Rotation parameters for 3D chart.
	// See also:
	//      Set3dRotation
	//-----------------------------------------------------------------------
	virtual CXTPChart3dRotation Get3dRotation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains depth value.
	// Returns:
	//      Depth value.
	// See also:
	//      SetDepth
	//-----------------------------------------------------------------------
	virtual int GetDepth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets depth value.
	// Parameters:
	//      dDepth - New depth value.
	// See also:
	//      GetDepth
	//-----------------------------------------------------------------------
	void SetDepth(int dDepth);

	//-----------------------------------------------------------------------
	// Summary:
	//      Checks if the chart has to be drawn as a torus instead of a pie.
	// Returns:
	//      TRUE if the chart is to be drawn as a torus, FALSE otherwise.
	// See also:
	//      SetTorus
	//-----------------------------------------------------------------------
	BOOL IsTorus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets torus appearance of the chart.
	// Parameters:
	//      bTorus - If TRUE, then the chart will be drawn as a torus.
	//               If FALSE, then the chart will be drawn as a pie.
	// See also:
	//      IsTorus
	//-----------------------------------------------------------------------
	void SetTorus(BOOL bTorus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load an event using the
	//     specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store event data to or load event
	//     data from storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChart3dPieSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChart3dPieSeriesStyle);
	//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	int OleGetDepth();
	void OleSetDepth(int dDepth);
	BOOL OleGetIsTorus();
	void OleSetTorus(BOOL bTorus);

private:
	int m_nDepth;
	BOOL m_bTorus;
};

AFX_INLINE int CXTPChart3dPieSeriesStyle::GetDepth() const
{
	return m_nDepth;
}

AFX_INLINE void CXTPChart3dPieSeriesStyle::SetDepth(int nDepth)
{
	m_nDepth = nDepth;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChart3dPieSeriesStyle::IsTorus() const
{
	return m_bTorus;
}

AFX_INLINE void CXTPChart3dPieSeriesStyle::SetTorus(BOOL bTorus)
{
	m_bTorus = bTorus;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DPIESERIESSTYLE_H__)
