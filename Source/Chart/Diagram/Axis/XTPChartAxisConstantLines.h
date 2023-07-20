// XTPChartAxisConstantLines.h
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
#if !defined(__XTPCHARTAXISCONSTANTLINES_H__)
#	define __XTPCHARTAXISCONSTANTLINES_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartAxis;
class CXTPChartLineStyle;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     CXTPChartAxisConstantLine is a hybrid object derived from CXTPChartTextElement
//     and CXTPChartLegendItemView. This class represents a constant line parallel
//     to an axis. Normally this class is owned by an axis object (CXTPChartAxis).
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisConstantLine
	: public CXTPChartTextElement
	, public CXTPChartLegendItemView
{
	DECLARE_DYNCREATE(CXTPChartAxisConstantLine)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisConstantLine object.
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLine();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisConstantLine object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisConstantLine();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility of the axis constant line.
	// Parameters:
	//     bVisible - BOOL value. TRUE to set the axis constant line to visible,
	//                FALSE to hide the axis constant line.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the axis constant line.
	// Return:
	//     A BOOL value. TRUE if the axis constant line is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility of the constant line legend item.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the constant line legend item to visible,
	//                FALSE to hide the constant line legend item.
	//-----------------------------------------------------------------------
	void SetLegendVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the constant line legend item.
	// Returns:
	//     BOOL value. TRUE if the constant line legend item is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsLegendVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text of the constant line legend item.
	// Parameters:
	//     lpszTitle - The string value for the text. Its type is CXTPChartString.
	//-----------------------------------------------------------------------
	void SetText(const CXTPChartString& lpszTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the text of the constant line legend item.
	// Returns:
	//     The string value of the text. Its type is CXTPChartString.
	//-----------------------------------------------------------------------
	CXTPChartString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text color of the constant line legend item.
	// Parameters:
	//     clrTextColor - The CXTPChartColor value for the text color.
	//-----------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the text color of the constant line legend item.
	// Returns:
	//     The CXTPChartColor value of the text color.
	//-----------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the actual text color of the constant line legend item.
	// Returns:
	//     The CXTPChartColor value for the actual text color.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the font of the constant line legend item text.
	// Returns:
	//     The CXTPChartFont value of the font used to render the text.
	//-----------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the font of the constant line legend item text.
	// Parameters:
	//     pFont - The CXTPChartFont value for the font to be used to render the text.
	//-----------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text alignment of the constant line legend item text.
	// Parameters:
	//     nAlignment - The enumerated value for the text alignment, the common alignments are
	//                  near, far and center.
	//-----------------------------------------------------------------------
	void SetAlignment(XTPChartStringAlignment nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the alignment of the constant line legend item text.
	// Returns:
	//     The XTPChartStringAlignment enumeration corresponding to the chosen alignment.
	//-----------------------------------------------------------------------
	XTPChartStringAlignment GetAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set anti-aliasing ON or OFF. Anti-aliasing is
	//     the smoothing of sharp edges of text and drawing.
	// Parameters:
	//     bAntialiasing - A BOOL value. TRUE to set anti-aliasing to ON,
	//                     FALSE to set anti-aliasing to OFF.
	//-------------------------------------------------------------------------
	void SetAntialiasing(BOOL bAntialiasing);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check if anti-aliasing is ON or OFF. Anti-aliasing is
	//     the smoothing of sharp edges of text and drawing.
	// Returns:
	//     A BOOL value. TRUE if anti-aliasing is ON, FALSE if anti-aliasing is OFF.
	//-------------------------------------------------------------------------
	BOOL GetAntialiasing() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend name.
	// Returns:
	//     The string value of the legend name. Its type is CXTPChartString.
	//-----------------------------------------------------------------------
	CXTPChartString GetLegendName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the legend name.
	// Parameters:
	//     strName - The string value for the legend name. Its type is CXTPChartString.
	//-----------------------------------------------------------------------
	void SetLegendName(const CXTPChartString& strName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the constant line style.
	// Returns:
	//     The pointer to an object of type CXTPChartLineStyle.
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetLineStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the value of the constant line in the axis.
	// Returns:
	//     A string/double denoting the axis value.
	//-----------------------------------------------------------------------
	CXTPChartString GetAxisValue() const;
	double GetAxisValueInternal() const; // <combine CXTPChartAxisConstantLine::GetAxisValue>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the value of the constant line in the axis.
	// Parameters:
	//     lpszValue - A string denoting the axis value.
	//     dValue    - A double denoting the axis value.
	//-----------------------------------------------------------------------
	void SetAxisValue(LPCTSTR lpszValue);
	void SetAxisValue(double dValue); // <combine CXTPChartAxisConstantLine::SetAxisValue@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the value of the actual color of the constant line.
	// Returns:
	//     A CXTPChartColor object denoting the ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;

	BOOL IsShowBehind() const;
	void SetShowBehind(BOOL bShowBehind);

	void ShowTextBelow(BOOL bShowTextBelow);
	BOOL IsTextBelow() const;

	CXTPChartColor GetColor() const;
	void SetColor(const CXTPChartColor& color);

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates a CXTPChartDeviceCommand object. This object
	//     represents the rendering of the legend item.
	// Parameters:
	//     pDC       - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds  - The bounding rectangle.
	// Returns:
	//     Returns a CXTPChartDeviceCommand object, this polymorphic object handles
	//     the rendering of an element in the chart. Here it handles the drawing
	//     of the legend item.
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC,
															  CRect rcBounds);

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisConstantLine);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	VARIANT OleGetAxisValue();
	void OleSetAxisValue(const VARIANT& var);

	BOOL OleGetLegendVisible();
	void OleSetLegendVisible(BOOL bValue);
	BOOL OleGetVisible();
	void OleSetVisible(BOOL bValue);
	BOOL OleGetShowTextBelow();
	void OleSetShowTextBelow(BOOL bValue);
	BOOL OleGetAlignment();
	void OleSetAlignment(int nValue);
	LPDISPATCH OleGetLineStyle();
	LPDISPATCH OleGetFont();

	OLE_COLOR OleGetColor();
	OLE_COLOR OleGetTextColor();
	void OleSetColor(OLE_COLOR clr);
	void OleSetTextColor(OLE_COLOR clr);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bLegendVisible; // TRUE if the legend is visible, FALSE if invisible.
	BOOL m_bVisible;	   // TRUE if the constant line is visible, FALSE if invisible.

	CXTPChartString m_strText;		 // The text of the constant line legend item.
	CXTPChartString m_strLegendName; // The legend name.
	CXTPChartString m_strAxisValue;  // The string value of constant line parallel to an axis.
	double m_dAxisValue;

	CXTPChartColor m_clrTextColor; // The text color of the constant line legend item.
	CXTPChartColor m_clrColor;	 // The color of the constant line.

	XTPChartStringAlignment m_nAlignment; // The text alignment of the constant line legend item.
	BOOL m_bAntialiasing;   // TRUE if anti-aliasing is used, FALSE if not using antialiasing.
	CXTPChartFont* m_pFont; // The font used for the text of the constant line legend item.
	BOOL m_bShowTextBelow;  // TRUE if the text is below, FALSE if not.
	BOOL m_bShowBehind;		// TRUE if the text is behind, FALSE if not.

	CXTPChartLineStyle* m_pLineStyle; // The chart line style object.

	friend class CXTPChartAxisConstantLines;
};

//===========================================================================
// Summary:
//     This class encapsulates a collection of CXTPChartAxisConstantLine objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisConstantLines : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisConstantLines object.
	// Parameters:
	//     pAxis - The pointer to a CXTPChartAxis object.
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLines(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisConstantLines object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisConstantLines();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function adds a new constant line to the collection of lines.
	// Parameters:
	//     pConstantLine - The pointer to a CXTPChartAxisConstantLine object.
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLine* Add(CXTPChartAxisConstantLine* pConstantLine);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the constant line object at a particular
	//     index in the collection.
	// Parameters:
	//     nIndex - The zero-based index of the item in the collection.
	// Returns:
	//     The pointer to the CXTPChartAxisConstantLine object at the index selected.
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLine* GetAt(int nIndex) const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartAxisConstantLines);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	DECLARE_ENUM_VARIANT(CXTPChartAxisConstantLines)

	afx_msg LPDISPATCH OleAdd(LPCWSTR lpszTitle);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
};

AFX_INLINE CXTPChartAxisConstantLine* CXTPChartAxisConstantLines::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrElements.GetSize()
			   ? (CXTPChartAxisConstantLine*)m_arrElements.GetAt(nIndex)
			   : NULL;
}

AFX_INLINE CXTPChartString CXTPChartAxisConstantLine::GetText() const
{
	return m_strText;
}
AFX_INLINE CXTPChartFont* CXTPChartAxisConstantLine::GetFont() const
{
	return m_pFont;
}
AFX_INLINE XTPChartStringAlignment CXTPChartAxisConstantLine::GetAlignment() const
{
	return m_nAlignment;
}

AFX_INLINE void CXTPChartAxisConstantLine::SetAntialiasing(BOOL bAntialiasing)
{
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::GetAntialiasing() const
{
	return m_bAntialiasing;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE CXTPChartLineStyle* CXTPChartAxisConstantLine::GetLineStyle() const
{
	return m_pLineStyle;
}
AFX_INLINE CXTPChartString CXTPChartAxisConstantLine::GetAxisValue() const
{
	return m_strAxisValue;
}
AFX_INLINE double CXTPChartAxisConstantLine::GetAxisValueInternal() const
{
	return m_dAxisValue;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetAxisValue(LPCTSTR lpszValue)
{
	m_strAxisValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisConstantLine::SetAxisValue(double dValue)
{
	m_dAxisValue = dValue;
	m_strAxisValue.Empty();
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsLegendVisible() const
{
	return m_bLegendVisible;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetLegendVisible(BOOL bLegendVisible)
{
	m_bLegendVisible = bLegendVisible;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisConstantLine::ShowTextBelow(BOOL bShowTextBelow)
{
	m_bShowTextBelow = bShowTextBelow;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsTextBelow() const
{
	return m_bShowTextBelow;
}
AFX_INLINE CXTPChartColor CXTPChartAxisConstantLine::GetColor() const
{
	return m_clrColor;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetColor(const CXTPChartColor& color)
{
	m_clrColor = color;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisConstantLine::SetLegendName(const CXTPChartString& strName)
{
	m_strLegendName = strName;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsShowBehind() const
{
	return m_bShowBehind;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetShowBehind(BOOL bShowBehind)
{
	m_bShowBehind = bShowBehind;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISCONSTANTLINES_H__)
