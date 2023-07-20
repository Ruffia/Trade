// XTPChartAxisLabel.h
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
#if !defined(__XTPCHARTAXISLABEL_H__)
#	define __XTPCHARTAXISLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartAxis;
class CXTPChartNumberFormat;

//===========================================================================
// Summary:
//     CXTPChartAxisLabel is a kind of CXTPChartTextElement.
//     This class represents the text label on an axis.
// Remarks:
//     This class is associated with a chart axis class.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisLabel : public CXTPChartTextElement
{
	DECLARE_DYNAMIC(CXTPChartAxisLabel)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisLabel object.
	// Parameters:
	//     pAxis - Pointer to the associated chart axis object.
	//-----------------------------------------------------------------------
	CXTPChartAxisLabel(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisLabel object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisLabel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text color of the axis label.
	// Parameters:
	//     clrTextColor - A reference to a chart color object representing an ARGB value.
	//-----------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the apparent text color of the axis label.
	// Returns:
	//     A chart color object representing an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the real text color of the axis label.
	// Returns:
	//     A chart color object representing an ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the font of the axis label text.
	// Parameters:
	//     pFont - The pointer to a chart font object.
	//-----------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the font of the axis label.
	// Returns:
	//     A pointer to the chart font object.
	//-----------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the angle of the text label.
	// Returns:
	//     An integer value representing the angle.
	//-----------------------------------------------------------------------
	int GetAngle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the angle of the text label.
	// Parameters:
	//     nAgle - An integer value representing the angle.
	//-----------------------------------------------------------------------
	void SetAngle(int nAgle);

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
	//     Call this function to set the visibility of the text label.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the text label to visible,
	//                FALSE to hide the text label.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the visibility of the text label.
	// Returns:
	//      A BOOL value. TRUE if the text label is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get a pointer to CXTPChartNumberFormat which
	//     specifies how to convert a number to a string.
	// Returns:
	//     Pointer to a CXTPChartNumberFormat class.
	// See Also:
	//     CXTPChartNumberFormat
	//-------------------------------------------------------------------------
	CXTPChartNumberFormat* GetFormat() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Obsolete method. Use GetFormat()->SetPattern(lpszFormat) instead.
	// Parameters:
	//     lpszFormat - New pattern to be set.
	// See Also:
	//     CXTPChartNumberFormat
	//-------------------------------------------------------------------------
	void SetFormat(LPCTSTR lpszFormat);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the pointer to the axis object to which the
	//     label is associated.
	// Returns:
	//     A pointer to the axis object.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisLabel);

	afx_msg LPDISPATCH OleGetFont();
	afx_msg OLE_COLOR OleGetTextColor();
	afx_msg void OleSetTextColor(OLE_COLOR clr);
	afx_msg LPDISPATCH OleGetFormat();

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CXTPChartColor m_clrTextColor; // The text color.
	CXTPChartFont* m_pFont;		   // The font used to render the text.
	BOOL m_bVisible;			   // TRUE if the label is visible, FALSE if not.
	BOOL m_bAntialiasing;		   // TRUE if antialiasing enabled, FALSE if not.
	int m_nAngle;				   // The angle of the label.

	CXTPChartNumberFormat* m_pFormat;
};

AFX_INLINE CXTPChartAxis* CXTPChartAxisLabel::GetAxis() const
{
	return (CXTPChartAxis*)m_pOwner;
}

AFX_INLINE CXTPChartFont* CXTPChartAxisLabel::GetFont() const
{
	return m_pFont;
}

AFX_INLINE int CXTPChartAxisLabel::GetAngle() const
{
	return m_nAngle;
}
AFX_INLINE void CXTPChartAxisLabel::SetAngle(int nAgle)
{
	m_nAngle = nAgle;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisLabel::SetAntialiasing(BOOL bAntialiasing)
{
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisLabel::GetAntialiasing() const
{
	return m_bAntialiasing;
}
AFX_INLINE BOOL CXTPChartAxisLabel::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE void CXTPChartAxisLabel::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE CXTPChartNumberFormat* CXTPChartAxisLabel::GetFormat() const
{
	return m_pFormat;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTAXISLABEL_H__)
