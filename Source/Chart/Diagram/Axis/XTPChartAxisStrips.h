// XTPChartAxisStrips.h
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
#if !defined(__XTPCHARTAXISSTRIPS_H__)
#	define __XTPCHARTAXISSTRIPS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartAxis;
class CXTPChartFillStyle;
//===========================================================================
// Summary:
//     This is a hybrid object of CXTPChartElement and CXTPChartLegendItemView.
//     This class represents a strip parallel to an axis.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisStrip
	: public CXTPChartElement
	, public CXTPChartLegendItemView
{
	DECLARE_DYNCREATE(CXTPChartAxisStrip)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisStrip object.
	//-----------------------------------------------------------------------
	CXTPChartAxisStrip();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisStrip object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisStrip();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the visibility of the axis strip.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the axis strip to visible,
	//                FALSE to hide the axis strip.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the axis strip.
	// Returns:
	//     A BOOL value. TRUE if the axis strip is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the visibility of the legend item.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the legend item to visible,
	//                FALSE to hide the legend item.
	//-----------------------------------------------------------------------
	void SetLegendVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the legend item.
	// Returns:
	//     A BOOL value. TRUE if the legend item is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsLegendVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the name for the axis strip.
	// Parameters:
	//     lpszName - A string value denoting the name of the axis strip.
	//-----------------------------------------------------------------------
	void SetName(const CXTPChartString& lpszName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the name of the axis strip.
	// Returns:
	//     A string value denoting the name of the axis strip.
	//-----------------------------------------------------------------------
	CXTPChartString GetName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the legend item name for the axis strip.
	// Parameters:
	//     lpszName - A string value denoting the legend item name of the axis strip.
	//-----------------------------------------------------------------------
	void SetLegendName(const CXTPChartString& lpszName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function get the legend item name of the axis strip.
	// Returns:
	//     A string value denoting the legend item name of the axis strip.
	//-----------------------------------------------------------------------
	CXTPChartString GetLegendName() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the fill style of the axis strip.
	// Returns:
	//     Returns a pointer to a CXTPChartFillStyle object which represents the
	//     fill style of the axis strip.
	//-------------------------------------------------------------------------
	CXTPChartFillStyle* GetFillStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the minimum value of the strip in the axis.
	// Returns:
	//     Returns a string/double describing the minimum value of the strip in the axis.
	//-------------------------------------------------------------------------
	CXTPChartString GetAxisMinValue() const;
	double GetAxisMinValueInternal() const; // <combine CXTPChartAxisStrip::GetAxisMinValue>

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the minimum value for the strip in the axis.
	// Parameters:
	//     lpszValue - A string describing the minimum value of the strip in the axis.
	//     dValue    - A double describing the minimum value of the strip in the axis.
	//-------------------------------------------------------------------------
	void SetAxisMinValue(LPCTSTR lpszValue);
	void SetAxisMinValue(double dValue); // <combine CXTPChartAxisStrip::SetAxisMinValue@LPCTSTR>

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the maximum value of the strip in the axis.
	// Returns:
	//     Returns a string describing the maximum value of the strip in the axis.
	//-------------------------------------------------------------------------
	CXTPChartString GetAxisMaxValue() const;
	double GetAxisMaxValueInternal() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the maximum value for the strip in the axis.
	// Parameters:
	//     lpszValue - A string describing the maximum value of the strip in the axis.
	//-------------------------------------------------------------------------
	void SetAxisMaxValue(LPCTSTR lpszValue);
	void SetAxisMaxValue(double dValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual color of the axis strip.
	// Returns:
	//     A chart color object representing an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;

	CXTPChartColor GetActualColor2() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the apparent color of the axis strip.
	// Returns:
	//     A CXTPChartColor object representing the ARGB color value selected.
	//-----------------------------------------------------------------------
	CXTPChartColor GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the color of the axis strip.
	// Parameters:
	//     color - A chart color object representing an ARGB value.
	//-----------------------------------------------------------------------
	void SetColor(const CXTPChartColor& color);

	CXTPChartColor GetColor2() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the color of the axis strip.
	// Parameters:
	//     color - A chart color object representing an ARGB value.
	//-----------------------------------------------------------------------
	void SetColor2(const CXTPChartColor& color);

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of an axis strip in the chart.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds - The bounding rectangle of the axis strip.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object. This object handles
	//     the rendering of an element in the chart. Here it handles
	//     the drawing of the axis strip.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC,
															  CRect rcBounds);

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisStrip);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	VARIANT OleGetAxisMinValue();
	void OleSetAxisMinValue(const VARIANT& var);
	VARIANT OleGetAxisMaxValue();
	void OleSetAxisMaxValue(const VARIANT& var);

	BOOL OleGetLegendVisible();
	void OleSetLegendVisible(BOOL bValue);
	BOOL OleGetVisible();
	void OleSetVisible(BOOL bValue);
	LPDISPATCH OleGetFillStyle();

	OLE_COLOR OleGetColor();
	OLE_COLOR OleGetColor2();
	void OleSetColor(OLE_COLOR clr);
	void OleSetColor2(OLE_COLOR clr);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bLegendVisible; // TRUE if the legend item is visible, FALSE if not visible.
	BOOL m_bVisible;	   // TRUE if the strip is visible, FALSE if not visible.

	CXTPChartString m_strName;		 // The name of the strip.
	CXTPChartString m_strLegendName; // The name of the legend item.

	CXTPChartString m_strAxisMinValue; // The minimum value of the strip in the axis.
	CXTPChartString m_strAxisMaxValue; // The maximum value of the strip in the axis.

	double m_dAxisMinValue; // The minimum value of the strip in the axis.
	double m_dAxisMaxValue; // The maximum value of the strip in the axis.

	CXTPChartColor m_clrColor;  // The color of the strip.
	CXTPChartColor m_clrColor2; // The color of the strip.

	CXTPChartFillStyle* m_pFillStyle; // The fill style used for the background painting.

	friend class CXTPChartAxisStrips;
};

//===========================================================================
// Summary:
//     This class encapsulates a collection of chart axis strip objects.
//     CXTPChartAxisStrips class is designed as a kind of CXTPChartElement.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisStrips : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisStrips object.
	// Parameters:
	//     pAxis - The pointer to the chart axis object to which the strips are associated.
	//-----------------------------------------------------------------------
	CXTPChartAxisStrips(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisStrip object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisStrips();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a new strip to the collection.
	// Parameters:
	//     pStrip - A pointer to an axis strip object.
	//-----------------------------------------------------------------------
	CXTPChartAxisStrip* Add(CXTPChartAxisStrip* pStrip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get a strip at a particular index in the strips collection.
	// Parameters:
	//     nIndex - The zero-based index of the item of choice.
	// Returns:
	//     A pointer to the strip object in the selected index.
	//-----------------------------------------------------------------------
	CXTPChartAxisStrip* GetAt(int nIndex) const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartAxisStrips);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	DECLARE_ENUM_VARIANT(CXTPChartAxisStrips)

	afx_msg LPDISPATCH OleAdd(LPCWSTR lpszTitle);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
};

AFX_INLINE CXTPChartAxisStrip* CXTPChartAxisStrips::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrElements.GetSize()
			   ? (CXTPChartAxisStrip*)m_arrElements.GetAt(nIndex)
			   : NULL;
}

AFX_INLINE CXTPChartString CXTPChartAxisStrip::GetName() const
{
	return m_strName;
}
AFX_INLINE void CXTPChartAxisStrip::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisStrip::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE CXTPChartFillStyle* CXTPChartAxisStrip::GetFillStyle() const
{
	return m_pFillStyle;
}
AFX_INLINE CXTPChartString CXTPChartAxisStrip::GetAxisMinValue() const
{
	return m_strAxisMinValue;
}
AFX_INLINE double CXTPChartAxisStrip::GetAxisMinValueInternal() const
{
	return m_dAxisMinValue;
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMinValue(LPCTSTR lpszValue)
{
	m_strAxisMinValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMinValue(double dValue)
{
	m_dAxisMinValue = dValue;
	m_strAxisMinValue.Empty();
	OnChartChanged();
}
AFX_INLINE CXTPChartString CXTPChartAxisStrip::GetAxisMaxValue() const
{
	return m_strAxisMaxValue;
}
AFX_INLINE double CXTPChartAxisStrip::GetAxisMaxValueInternal() const
{
	return m_dAxisMaxValue;
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMaxValue(LPCTSTR lpszValue)
{
	m_strAxisMaxValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMaxValue(double dValue)
{
	m_dAxisMaxValue = dValue;
	m_strAxisMaxValue.Empty();
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisStrip::IsLegendVisible() const
{
	return m_bLegendVisible;
}
AFX_INLINE void CXTPChartAxisStrip::SetLegendVisible(BOOL bLegendVisible)
{
	m_bLegendVisible = bLegendVisible;
	OnChartChanged();
}
AFX_INLINE CXTPChartColor CXTPChartAxisStrip::GetColor() const
{
	return m_clrColor;
}
AFX_INLINE CXTPChartColor CXTPChartAxisStrip::GetColor2() const
{
	return m_clrColor2;
}
AFX_INLINE void CXTPChartAxisStrip::SetLegendName(const CXTPChartString& lpszName)
{
	m_strLegendName = lpszName;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISStripS_H__)
