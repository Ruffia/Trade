// XTPChartAxisGridLines.h
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
#if !defined(__XTPCHARTAXISGRIDLINES_H__)
#	define __XTPCHARTAXISGRIDLINES_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartAxis;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     CXTPChartAxisGridLines is a kind of CXTPChartElement,
//     This class represents the grid lines parallel to an axis.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisGridLines : public CXTPChartElement
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisGridLines object.
	// Parameters:
	//     pAxis - Pointer to a chart axis object.
	//-----------------------------------------------------------------------
	CXTPChartAxisGridLines(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisGridLines object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisGridLines();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the grid lines.
	// Returns:
	//     A BOOL value. TRUE if the grid lines are visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility of the grid lines.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the grid lines to visible,
	//                FALSE to hide the grid lines.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the minor grid lines.
	// Returns:
	//     A BOOL value. TRUE if the minor grid lines are visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsMinorVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility of the minor grid lines.
	// Parameters:
	//     bMinorVisible - A BOOL value. TRUE to set the minor grid lines to visible,
	//                     FALSE to hide the minor grid lines.
	//-----------------------------------------------------------------------
	void SetMinorVisible(BOOL bMinorVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the color of the grid lines.
	// Returns:
	//     A CXTPChartColor object representing the ARGB color value selected.
	//-----------------------------------------------------------------------
	CXTPChartColor GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the color of the minor grid lines.
	// Returns:
	//     A CXTPChartColor object representing the ARGB color value selected.
	//-----------------------------------------------------------------------
	CXTPChartColor GetMinorColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the grid line style used.
	// Returns:
	//     A pointer to the CXTPChartLineStyle object representing the grid line style
	//     selected.
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetLineStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the minor grid line style used.
	// Returns:
	//     A pointer to the CXTPChartLineStyle object representing the minor grid line
	//     style selected.
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetMinorLineStyle() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the axis object to which the grid lines
	//     are associated.
	// Returns:
	//     A pointer to the CXTPChartAxis object representing the chart axis.
	//-----------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisGridLines);
	LPDISPATCH OleGetLineStyle();
	LPDISPATCH OleGetMinorLineStyle();
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bVisible;				// TRUE if the grid lines are visible, FALSE if not.
	BOOL m_bMinorVisible;			// TRUE if the minor grid lines are visible, FALSE if not.
	CXTPChartColor m_clrColor;		// ARGB value of the grid lines color.
	CXTPChartColor m_clrMinorColor; // ARGB value of the minor grid lines color.

	CXTPChartLineStyle* m_pLineStyle;	  // The grid line style.
	CXTPChartLineStyle* m_pMinorLineStyle; // The minor grid line style.
};

AFX_INLINE CXTPChartAxis* CXTPChartAxisGridLines::GetAxis() const
{
	return (CXTPChartAxis*)m_pOwner;
}
AFX_INLINE BOOL CXTPChartAxisGridLines::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE BOOL CXTPChartAxisGridLines::IsMinorVisible() const
{
	return m_bMinorVisible;
}
AFX_INLINE void CXTPChartAxisGridLines::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE CXTPChartLineStyle* CXTPChartAxisGridLines::GetLineStyle() const
{
	return m_pLineStyle;
}
AFX_INLINE CXTPChartLineStyle* CXTPChartAxisGridLines::GetMinorLineStyle() const
{
	return m_pMinorLineStyle;
}
AFX_INLINE void CXTPChartAxisGridLines::SetMinorVisible(BOOL bMinorVisible)
{
	m_bMinorVisible = bMinorVisible;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISGRIDLINES_H__)
