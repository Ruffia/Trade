// XTPChartSeriesLabel.h
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
#if !defined(__XTPCHARTSERIESLABEL_H__)
#	define __XTPCHARTSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartSeriesStyle;
class CXTPChartElementView;
class CXTPChartDeviceContext;
class CXTPChartSeriesPointView;
class CXTPChartDeviceCommand;
class CXTPChartBorder;
class CXTPChartFillStyle;
class CXTPChartSeriesPoint;
class CXTPChartNumberFormat;

//===========================================================================
// Summary:
//     This class abstracts the label of a series. It is a kind of chart
//     text element.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSeriesLabel : public CXTPChartTextElement
{
	DECLARE_SERIAL(CXTPChartSeriesLabel);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeriesLabel object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeriesLabel object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesLabel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text color of the label.
	// Parameters:
	//     clrTextColor - A 32 bit color value specifying the color of choice.
	//-----------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the apparent text color of the label.
	// Returns:
	//     A 32 bit color value specifying the color.
	//-----------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the font of the text label.
	// Returns:
	//     A CXTPChartFont object describing the font used.
	//-----------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the font of the text label.
	// Parameters:
	//     pFont -  A pointer to a CXTPChartFont object describing the font of choice.
	//-----------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the lines visibility.
	// Parameters:
	//     bShowLines -  A BOOL value. TRUE to set the lines to visible,
	//                   FALSE to hide the lines.
	//-----------------------------------------------------------------------
	void SetShowLines(BOOL bShowLines);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the lines are visible.
	// Returns:
	//     A BOOL value. TRUE if the lines are visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetShowLines() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the line length.
	// Parameters:
	//     nLineLength - An integer value specifying the line length.
	//-----------------------------------------------------------------------
	void SetLineLength(int nLineLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the line length.
	// Returns:
	//     An integer value specifying the line length.
	//-----------------------------------------------------------------------
	int GetLineLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the label visibility.
	// Parameters:
	//     bVisible - A BOOL value. TRUE to set the label to visible,
	//                FALSE to hide the label.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the label is visible.
	// Returns:
	//     A BOOL value. TRUE if the label is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

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

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get a pointer to CXTPChartNumberFormat that specifies
	//     how to convert numbers to strings.
	// Returns:
	//     Pointer to CXTPChartNumberFormat class.
	// See Also:
	//     CXTPChartNumberFormat
	//-------------------------------------------------------------------------
	CXTPChartNumberFormat* GetFormat() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Obsolete method. Use GetFormat()->SetPattern(lpszFormat) instead.
	// Parameters:
	//     lpszFormat - New Pattern to be set.
	// See Also:
	//     CXTPChartNumberFormat
	//-------------------------------------------------------------------------
	void SetFormat(LPCTSTR lpszFormat);

	virtual CXTPChartString GetPointLabel(CXTPChartSeriesPoint* pPoint, int nValueIndex = -1) const;

	CXTPChartBorder* GetBorder() const;
	CXTPChartFillStyle* GetFillStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text color of the label.
	// Parameters:
	//     clrTextColor - A 32 bit color value specifying the color of choice.
	//-----------------------------------------------------------------------
	void SetBackColor(const CXTPChartColor& clrBackColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the apparent text color of the label.
	// Returns:
	//     A 32 bit color value specifying the color.
	//-----------------------------------------------------------------------
	CXTPChartColor GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text color of the label.
	// Parameters:
	//     clrTextColor - A 32 bit color value specifying the color of choice.
	//-----------------------------------------------------------------------
	void SetLineColor(const CXTPChartColor& clrBackColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the apparent text color of the label.
	// Returns:
	//     A 32 bit color value specifying the color.
	//-----------------------------------------------------------------------
	CXTPChartColor GetLineColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the transparency of the filled area with
	//     respect to the background.
	// Returns:
	//     Returns an integer whose values are from 0 to 255.
	// Remarks:
	//     A value of 0 means fully transparent and a value of 255 means fully opaque.
	//-------------------------------------------------------------------------
	int GetTransparency() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the transparency of the filled area with
	//     respect to the background.
	// Parameters:
	//     nTransparency - an integer whose values are from 0 to 255.
	// Remarks:
	//     A value of 0 means fully transparent and a value of 255 means fully opaque.
	//-------------------------------------------------------------------------
	void SetTransparency(int nTransparency);

public:
	CXTPChartColor GetActualBackColor() const;

	CXTPChartColor GetActualConnectorColor() const;

public:
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartSeriesPointView* pPointView,
											 CXTPChartElementView* pParentView);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartSeriesLabel);
	afx_msg void OleChartChanged();
	afx_msg LPDISPATCH OleGetFormat();
	afx_msg LPDISPATCH OleGetFont();
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CXTPChartColor m_clrTextColor; // The text color.
	CXTPChartColor m_clrBackColor; // The background color.
	CXTPChartColor m_clrLineColor; // The line color.
	BOOL m_bVisible;			   // TRUE if the label is visible and FALSE if not.
	CXTPChartFont* m_pFont;		   // The font of the label.
	BOOL m_bAntialiasing;		   // TRUE if antialiasing enabled, FALSE if not.
	BOOL m_bShowLines;			   // TRUE if the lines are visible, FALSE if not.
	int m_nLineLength;			   // The line length.
	CXTPChartBorder* m_pBorder;	// Chart border object.
	CXTPChartFillStyle* m_pFillStyle;
	CXTPChartNumberFormat* m_pFormat;
	int m_nTransparency; // The transparency values range from 0 to 255.

	LPDISPATCH OleGetBorder();
	LPDISPATCH OleGetFillStyle();
	OLE_COLOR OleGetTextColor();
	void OleSetTextColor(OLE_COLOR clr);
	OLE_COLOR OleGetBackColor();
	void OleSetBackColor(OLE_COLOR clr);
	OLE_COLOR OleGetLineColor();
	void OleSetLineColor(OLE_COLOR clr);

protected:
	friend class CXTPChartSeriesStyle;
};

class _XTP_EXT_CLASS CXTPChartSeriesLabelView : public CXTPChartElementView
{
	DECLARE_DYNAMIC(CXTPChartSeriesLabelView);

public:
	CXTPChartSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
							 CXTPChartElementView* pParentView);

public:
	virtual void CalculateLayout(CXTPChartDeviceContext* pDC);

	CXTPChartColor GetActualTextColor() const;
	CXTPChartColor GetActualBorderColor() const;
	CXTPChartColor GetActualConnectorColor() const;
	CXTPChartSeriesPointView* GetPointView() const;
	CXTPChartSeriesLabel* GetLabel() const;

protected:
	CXTPChartSeriesLabel* m_pLabel;
	CXTPChartSeriesPointView* m_pPointView;
};

AFX_INLINE CXTPChartSeriesLabel* CXTPChartSeriesLabelView::GetLabel() const
{
	return m_pLabel;
}

class _XTP_EXT_CLASS CXTPChartSeriesLabelConnectorPainterBase
{
public:
	CXTPChartSeriesLabelConnectorPainterBase(CXTPChartPointF startPoint,
											 CXTPChartPointF finishPoint, CXTPChartRectF bounds);

public:
	static CXTPChartRectF AFX_CDECL CalcBorderBoundsForTangentDrawing(
		const CXTPPoint3d& point, double angle, const CXTPChartSizeF& textSize, int borderThickness,
		CXTPChartRectF& innerBounds);

protected:
	CXTPChartPointF m_startPoint;
	CXTPChartPointF m_finishPoint;
	CXTPChartRectF m_bounds;
};

class _XTP_EXT_CLASS CXTPChartSeriesLabelLineConnectorPainter
	: public CXTPChartSeriesLabelConnectorPainterBase
{
public:
	CXTPChartSeriesLabelLineConnectorPainter(CXTPChartPointF startPoint,
											 CXTPChartPointF finishPoint, CXTPChartRectF bounds);

	virtual ~CXTPChartSeriesLabelLineConnectorPainter()
	{
	}

public:
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC,
														const CXTPChartColor& color);
};

AFX_INLINE CXTPChartElementView* CXTPChartSeriesLabel::CreateView(
	CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pPointView);
	UNREFERENCED_PARAMETER(pParentView);
	return NULL;
}

AFX_INLINE CXTPChartFont* CXTPChartSeriesLabel::GetFont() const
{
	return m_pFont;
}

AFX_INLINE CXTPChartColor CXTPChartSeriesLabel::GetTextColor() const
{
	return m_clrTextColor;
}

AFX_INLINE void CXTPChartSeriesLabel::SetShowLines(BOOL bShowLines)
{
	m_bShowLines = bShowLines;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartSeriesLabel::GetShowLines() const
{
	return m_bShowLines;
}

AFX_INLINE int CXTPChartSeriesLabel::GetLineLength() const
{
	return m_nLineLength;
}

AFX_INLINE void CXTPChartSeriesLabel::SetLineLength(int nLineLength)
{
	m_nLineLength = nLineLength;
	OnChartChanged();
}

AFX_INLINE void CXTPChartSeriesLabel::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartSeriesLabel::IsVisible() const
{
	return m_bVisible;
}

AFX_INLINE void CXTPChartSeriesLabel::SetAntialiasing(BOOL bAntialiasing)
{
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartSeriesLabel::GetAntialiasing() const
{
	return m_bAntialiasing;
}

AFX_INLINE CXTPChartNumberFormat* CXTPChartSeriesLabel::GetFormat() const
{
	return m_pFormat;
}

AFX_INLINE void CXTPChartSeriesLabel::SetTextColor(const CXTPChartColor& clrTextColor)
{
	m_clrTextColor = clrTextColor;
	OnChartChanged();
}

AFX_INLINE void CXTPChartSeriesLabel::SetLineColor(const CXTPChartColor& clrLineColor)
{
	m_clrLineColor = clrLineColor;
	OnChartChanged();
}

AFX_INLINE void CXTPChartSeriesLabel::SetBackColor(const CXTPChartColor& clrBackColor)
{
	m_clrBackColor = clrBackColor;
	OnChartChanged();
}

AFX_INLINE CXTPChartColor CXTPChartSeriesLabel::GetBackColor() const
{
	return m_clrBackColor;
}

AFX_INLINE CXTPChartColor CXTPChartSeriesLabel::GetLineColor() const
{
	return m_clrLineColor;
}

AFX_INLINE CXTPChartBorder* CXTPChartSeriesLabel::GetBorder() const
{
	return m_pBorder;
}

AFX_INLINE CXTPChartFillStyle* CXTPChartSeriesLabel::GetFillStyle() const
{
	return m_pFillStyle;
}

AFX_INLINE int CXTPChartSeriesLabel::GetTransparency() const
{
	return m_nTransparency;
}

AFX_INLINE void CXTPChartSeriesLabel::SetTransparency(int nTransparency)
{
	m_nTransparency = nTransparency;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTSERIESLABEL_H__)
