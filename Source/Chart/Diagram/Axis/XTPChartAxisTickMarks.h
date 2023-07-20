// XTPChartAxisTickMarks.h
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
#if !defined(__XTPCHARTAXISTICKMARKS_H__)
#	define __XTPCHARTAXISTICKMARKS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartAxis;

//===========================================================================
// Summary:
//     This class represents the tick marks in an axis. It's a kind of chart element.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisTickMarks : public CXTPChartElement
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisTickMarks object.
	// Parameters:
	//     pAxis - A pointer to the chart axis object to which these tick marks
	//             are associated with.
	//-----------------------------------------------------------------------
	CXTPChartAxisTickMarks(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisTickMarks object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisTickMarks();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns a pointer to the CXTPChartAxis object associated with
	//     the axis tick mark object.
	// Returns:
	//     Returns a pointer to the associated chart axis object.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility for the axis tick marks.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the tick marks to visible,
	//                FALSE to hide the tick marks.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the axis tick marks.
	// Returns:
	//     A BOOL value. TRUE if the tick marks are visible, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility for the axis minor tick marks.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the minor tick marks to visible,
	//                FALSE to hide the minor tick marks.
	//-----------------------------------------------------------------------
	void SetMinorVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the axis minor tick marks.
	// Returns:
	//     A BOOL value. TRUE if the minor tick marks are visible, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL IsMinorVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the thickness of the tick marks.
	// Parameters:
	//     nThickness - An integer value denoting the thickness of the tick marks.
	//-----------------------------------------------------------------------
	void SetThickness(int nThickness);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the thickness of the tick marks.
	// Returns:
	//     An integer value denoting the thickness of the tick marks.
	//-------------------------------------------------------------------------
	int GetThickness() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the thickness of the minor tick marks.
	// Parameters:
	//     nThickness - An integer value denoting the thickness of the minor tick marks.
	//-----------------------------------------------------------------------
	void SetMinorThickness(int nThickness);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the thickness of the minor tick marks.
	// Returns:
	//     An integer value denoting the thickness of the minor tick marks.
	//-------------------------------------------------------------------------
	int GetMinorThickness() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the length of the tick marks.
	// Parameters:
	//     nLength - An integer value denoting the length of the tick marks.
	//-----------------------------------------------------------------------
	void SetLength(int nLength);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the length of the tick marks.
	// Returns:
	//     An integer value denoting the length of the tick marks.
	//-------------------------------------------------------------------------
	int GetLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the length of the minor tick marks.
	// Parameters:
	//     nLength - An integer value denoting the length of the minor tick marks.
	//-----------------------------------------------------------------------
	void SetMinorLength(int nLength);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the length of the minor tick marks.
	// Returns:
	//     An integer value denoting the length of the minor tick marks.
	//-------------------------------------------------------------------------
	int GetMinorLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the tick marks to cross the axis.
	// Parameters:
	//     bCrossAxis - A BOOL value. TRUE to make the tick marks cross the axis,
	//                  FALSE to make the tick marks normal.
	//-----------------------------------------------------------------------
	void SetCrossAxis(BOOL bCrossAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the tick marks cross the axis.
	// Returns:
	//     A BOOL value. TRUE if the tick marks are crossing the axis, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL IsCrossAxis() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisTickMarks);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bVisible;	   // TRUE if the tick marks are visible, FALSE if not.
	BOOL m_bMinorVisible;  // TRUE if the minor tick marks are visible, FALSE if not.
	int m_nThickness;	  // The thickness of the tick marks.
	int m_nMinorThickness; // The thickness of the minor tick marks.
	int m_nLength;		   // The length of the tick marks.
	int m_nMinorLength;	// The length of the minor tick marks.
	BOOL m_bCrossAxis;	 // TRUE if the tick marks are crossing the axis, FALSE if not.
};

AFX_INLINE CXTPChartAxis* CXTPChartAxisTickMarks::GetAxis() const
{
	return (CXTPChartAxis*)m_pOwner;
}
AFX_INLINE BOOL CXTPChartAxisTickMarks::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE BOOL CXTPChartAxisTickMarks::IsMinorVisible() const
{
	return m_bMinorVisible;
}
AFX_INLINE int CXTPChartAxisTickMarks::GetThickness() const
{
	return m_nThickness;
}
AFX_INLINE int CXTPChartAxisTickMarks::GetMinorThickness() const
{
	return m_nMinorThickness;
}
AFX_INLINE int CXTPChartAxisTickMarks::GetLength() const
{
	return m_nLength;
}
AFX_INLINE int CXTPChartAxisTickMarks::GetMinorLength() const
{
	return m_nMinorLength;
}
AFX_INLINE BOOL CXTPChartAxisTickMarks::IsCrossAxis() const
{
	return m_bCrossAxis;
}
AFX_INLINE void CXTPChartAxisTickMarks::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisTickMarks::SetMinorVisible(BOOL bVisible)
{
	m_bMinorVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisTickMarks::SetThickness(int nThickness)
{
	m_nThickness = nThickness;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisTickMarks::SetMinorThickness(int nThickness)
{
	m_nMinorThickness = nThickness;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisTickMarks::SetLength(int nLength)
{
	m_nLength = nLength;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisTickMarks::SetMinorLength(int nLength)
{
	m_nMinorLength = nLength;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisTickMarks::SetCrossAxis(BOOL bCrossAxis)
{
	m_bCrossAxis = bCrossAxis;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISTICKMARKS_H__)
