// XTPChartAxisRange.h
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
#if !defined(__XTPCHARTAXISRANGE_H__)
#	define __XTPCHARTAXISRANGE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartAxis;
class CXTPChartAxisView;

//===========================================================================
// Summary
//    This class represents the value range of the axis.
// Remarks:
//    This class is associated with a chart axis class.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisRange : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartAxisRange)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisRange object.
	// Parameters:
	//     pAxis - Pointer to the associated chart axis object.
	//-----------------------------------------------------------------------
	CXTPChartAxisRange(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisLabel object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisRange();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function updates the axis, with respect to the series values.
	// Remarks:
	//     Call this function whenever there is a change in series values.
	//-------------------------------------------------------------------------
	void UpdateRange(CXTPChartAxisView* pView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the maximum value of the axis.
	// Returns:
	//     A double value which denotes the maximum value.
	//-------------------------------------------------------------------------
	double GetMaxValue() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the minimum value of the axis.
	// Returns:
	//     A double value which denotes the minimum value.
	//-------------------------------------------------------------------------
	double GetMinValue() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the maximum value of the axis.
	// Parameters:
	//     dMaxValue - A double value which denotes the maximum value.
	//-------------------------------------------------------------------------
	void SetMaxValue(double dMaxValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the minimum value of the axis.
	// Parameters:
	//     dMinValue - A double value which denotes the minimum value.
	//-------------------------------------------------------------------------
	void SetMinValue(double dMinValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to manually set the min and max values for the range.
	// Parameters:
	//     bAutoRange - TRUE to allow manually set range, FALSE to calculate automatically.
	//-------------------------------------------------------------------------
	void SetAutoRange(BOOL bAutoRange);

	BOOL IsAutoRange() const;

	//////////////////////////////////////////////////////////////////////////
	// View

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the maximum value of axis.
	// Returns:
	//     A double value which denotes the maximum value.
	//-------------------------------------------------------------------------
	double GetViewMaxValue() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the minimum value of axis.
	// Returns:
	//     A double value which denotes the minimum value.
	//-------------------------------------------------------------------------
	double GetViewMinValue() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the maximum value of axis.
	// Parameters:
	//     dMaxValue - A double value which denotes the maximum value.
	//-------------------------------------------------------------------------
	void SetViewMaxValue(double dMaxValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the minimum value of axis.
	// Parameters:
	//     dMinValue - A double value which denotes the minimum value.
	//-------------------------------------------------------------------------
	void SetViewMinValue(double dMinValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to manually set the min and max values for the range.
	// Parameters:
	//     bAutoRange - TRUE to allow manually set range, FALSE to calculate automatically.
	//-------------------------------------------------------------------------
	void SetViewAutoRange(BOOL bAutoRange);

	BOOL IsViewAutoRange() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to enable or disable the side margins.
	// Parameters:
	//     bSideMargins - A BOOL value. TRUE to enable the side margins,
	//                    FALSE to disable the side margins.
	//-------------------------------------------------------------------------
	void SetSideMargins(BOOL bSideMargins);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the side margins are enabled or disabled.
	// Returns:
	//     A BOOL value. TRUE if the side margins are enabled, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL GetSideMargins() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to enable or disable the zero level on the axis.
	// Parameters:
	//     bShowZeroLevel - A BOOL value. TRUE to enable the zero level,
	//                      FALSE to disable the zero level.
	//-------------------------------------------------------------------------
	void SetShowZeroLevel(BOOL bShowZeroLevel);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the zero level is enabled or disabled.
	// Returns:
	//     A BOOL value. TRUE if the zero level is enabled, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL GetShowZeroLevel() const;

public:
	double GetZoomLimit() const;
	void SetZoomLimit(double dZoomLimit);

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function updates range based on the various parameters affecting
	//     the axis range (e.g. size margins, showing zero level, etc.).
	//-------------------------------------------------------------------------
	void Correct(CXTPChartAxisView* pView);

protected:
	CXTPChartAxis* m_pAxis; // Pointer to chart axis object.

	double m_nMinValue;	// The minimum value of the range.
	double m_nMaxValue;	// The maximum value of the range.
	BOOL m_bSideMargins;   // TRUE if the side margins are present and FALSE if not.
	BOOL m_bShowZeroLevel; // TRUE if the zero level is shown on the axis and FALSE if not.

	BOOL m_bAutoRange;

	double m_nViewMinValue;
	double m_nViewMaxValue;
	BOOL m_bViewAutoRange;

	double m_dZoomLimit;

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisRange);

	afx_msg BOOL OleGetShowZeroLevel();
	afx_msg void OleSetShowZeroLevel(BOOL bSet);
	afx_msg BOOL OleGetSideMargins();
	afx_msg void OleSetSideMargins(BOOL bSet);

	afx_msg BOOL OleGetAutoRange();
	afx_msg void OleSetAutoRange(BOOL bSet);
	afx_msg double OleGetMinValue();
	afx_msg void OleSetMinValue(double bSet);
	afx_msg double OleGetMaxValue();
	afx_msg void OleSetMaxValue(double bSet);

	//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPChartAxis;
};

AFX_INLINE double CXTPChartAxisRange::GetMaxValue() const
{
	return m_nMaxValue;
}
AFX_INLINE double CXTPChartAxisRange::GetMinValue() const
{
	return m_nMinValue;
}
AFX_INLINE BOOL CXTPChartAxisRange::GetSideMargins() const
{
	return m_bSideMargins;
}
AFX_INLINE BOOL CXTPChartAxisRange::GetShowZeroLevel() const
{
	return m_bShowZeroLevel;
}
AFX_INLINE BOOL CXTPChartAxisRange::IsAutoRange() const
{
	return m_bAutoRange;
}
AFX_INLINE double CXTPChartAxisRange::GetViewMaxValue() const
{
	return m_nViewMaxValue;
}
AFX_INLINE double CXTPChartAxisRange::GetViewMinValue() const
{
	return m_nViewMinValue;
}
AFX_INLINE BOOL CXTPChartAxisRange::IsViewAutoRange() const
{
	return m_bViewAutoRange;
}
AFX_INLINE double CXTPChartAxisRange::GetZoomLimit() const
{
	return m_dZoomLimit;
}
AFX_INLINE void CXTPChartAxisRange::SetZoomLimit(double dZoomLimit)
{
	m_dZoomLimit = dZoomLimit;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISRANGE_H__)
