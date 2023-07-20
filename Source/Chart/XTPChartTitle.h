// XTPChartTitle.h
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
#if !defined(__XTPCHARTTITLE_H__)
#	define __XTPCHARTTITLE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartFont;
class CXTPChartElementView;
class CXTPChartContent;
class CXTPPropExchange;

enum XTPChartDocking
{
	xtpChartDockTop,
	xtpChartDockBottom,
	xtpChartDockLeft,
	xtpChartDockRight
};

//===========================================================================
// Summary:
//     This class represents a chart title.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTitle : public CXTPChartTextElement
{
	DECLARE_DYNAMIC(CXTPChartTitle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTitle object.
	//-----------------------------------------------------------------------
	CXTPChartTitle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartTitle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartTitle();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the visibility of the title.
	// Parameters:
	//     bVisible    - A BOOL value. TRUE to set the title to visible,
	//                   FALSE to hide the title.
	//-------------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the title is visible.
	// Returns:
	//     A BOOL value. TRUE if the title is visible, FALSE otherwise.
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text of the title.
	// Parameters:
	//     lpszTitle    - New text of the title.
	// See Also:
	//     GetText
	//-------------------------------------------------------------------------
	void SetText(LPCTSTR lpszTitle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the text of the title.
	// Returns:
	//     Text of the title.
	// See Also:
	//     SetText
	//-------------------------------------------------------------------------
	CXTPChartString GetText() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set the custom text color of the title.
	// Parameters:
	//     clrTextColor - New text color of the title. It can be CXTPChartColor::Empty
	//                    to use the default appearance color.
	// See Also:
	//     GetTextColor
	//-------------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the custom text color of the title.
	// Returns:
	//     The text color of the title. If default appearance color is used, then
	//     CXTPChartColor::Empty will be returned.
	// See Also:
	//     SetTextColor
	//-------------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the actual text color of the title.
	// Returns:
	//     The actual text color of the title.
	// See Also:
	//     GetTextColor
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set the font of the title.
	// Parameters:
	//     pFont - New font to be set.
	// See Also:
	//     GetFont
	//-------------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the font for the title.
	// Returns:
	//     Pointer to a CXTPChartFont object that holds font information.
	// See Also:
	//     SetFont
	//-------------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set the docking position of the title.
	// Parameters:
	//     nDocking - docking position of the title. It can be one of the following:
	//                xtpChartDockTop, xtpChartDockBottom, xtpChartDockLeft, xtpChartDockRight.
	// See Also:
	//     GetDocking
	//-------------------------------------------------------------------------
	void SetDocking(XTPChartDocking nDocking);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the docking position of the title.
	// Returns:
	//     Docking position of title. It can be one of the following:
	//     xtpChartDockTop, xtpChartDockBottom, xtpChartDockLeft, xtpChartDockRight.
	// See Also:
	//     SetDocking
	//-------------------------------------------------------------------------
	XTPChartDocking GetDocking() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set the indent between 2 titles.
	// Parameters:
	//     nIndent - New indent to be set.
	// See Also:
	//     GetIndent
	//-------------------------------------------------------------------------
	void SetIndent(int nIndent);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the indent between 2 titles.
	// Returns:
	//     Indent between 2 titles.
	// See Also:
	//     GetIndent
	//-------------------------------------------------------------------------
	int GetIndent() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set the text alignment of the title.
	// Parameters:
	//     nAlignment - New text alignment to be set. It can be one of the following:
	//                  xtpChartAlignNear, xtpChartAlignCenter, xtpChartAlignFar.
	// See Also:
	//     GetAlignment
	//-------------------------------------------------------------------------
	void SetAlignment(XTPChartStringAlignment nAlignment);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get the text alignment of the title.
	// Parameters:
	//     Text alignment of the title. It can be one of the following:
	//     xtpChartAlignNear, xtpChartAlignCenter, xtpChartAlignFar.
	// See Also:
	//     SetAlignment
	//-------------------------------------------------------------------------
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

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method calculates text angle according to the current docking position.
	// See Also:
	//     GetDocking
	//-------------------------------------------------------------------------
	int GetTextAngle() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of a title object
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store property data to or load property
	//     data from storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	//{{AFX_CODEJOCK_PRIVATE

	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartElementView* pParentView);
	CPoint CalcOrigin(CRect bounds, CRect startBounds, CRect rect);
	XTPChartNearTextPosition GetNearTextPosition();
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartTitle);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	LPDISPATCH OleGetFont();

	OLE_COLOR OleGetTextColor();
	void OleSetTextColor(OLE_COLOR clr);

	//}}AFX_CODEJOCK_PRIVATE

#	endif

protected:
	friend class CXTPChartTitleCollection;

	CXTPChartString m_strText;			  // Text of the title.
	CXTPChartColor m_clrTextColor;		  // Text color
	BOOL m_bVisible;					  // TRUE if title is visible
	CXTPChartFont* m_pFont;				  // Font of the title
	BOOL m_bAntialiasing;				  // Antialiasing flag
	XTPChartDocking m_nDocking;			  // Current docking position of the title
	int m_nIndent;						  // Indent between 2 titles
	int m_nInnerIndent;					  // Inner indent
	XTPChartStringAlignment m_nAlignment; // Text alignment
};

//===========================================================================
// Summary:
//     CXTPChartTitleCollection represents a collection of chart titles.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTitleCollection : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTitleCollection object.
	// Parameters:
	//     pOwner - Owner object.
	//-----------------------------------------------------------------------
	CXTPChartTitleCollection(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartTitleCollection object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartTitleCollection();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new title.
	// Parameters:
	//     pTitle - New title to add.
	//-----------------------------------------------------------------------
	CXTPChartTitle* Add(CXTPChartTitle* pTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a title by its index.
	// Parameters:
	//     nIndex - Index of the title to get.
	//-----------------------------------------------------------------------
	CXTPChartTitle* GetAt(int nIndex) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of a title object
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store property data to or load property
	//     data from storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//{{AFX_CODEJOCK_PRIVATE
	void CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);
	void CalculateView(CXTPChartDeviceContext* pDC, CRect& rcChart,
					   CXTPChartElementView* pParentView);
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartTitleCollection);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	DECLARE_ENUM_VARIANT(CXTPChartTitleCollection)

	afx_msg LPDISPATCH OleAdd(LPCTSTR lpszTitle);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	friend class CXTPChartContent;
};

AFX_INLINE CXTPChartTitle* CXTPChartTitleCollection::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrElements.GetSize()
			   ? (CXTPChartTitle*)m_arrElements.GetAt(nIndex)
			   : NULL;
}

AFX_INLINE CXTPChartString CXTPChartTitle::GetText() const
{
	return m_strText;
}
AFX_INLINE CXTPChartFont* CXTPChartTitle::GetFont() const
{
	return m_pFont;
}
AFX_INLINE XTPChartDocking CXTPChartTitle::GetDocking() const
{
	return m_nDocking;
}
AFX_INLINE int CXTPChartTitle::GetIndent() const
{
	return m_nIndent;
}
AFX_INLINE XTPChartStringAlignment CXTPChartTitle::GetAlignment() const
{
	return m_nAlignment;
}

AFX_INLINE void CXTPChartTitle::SetAntialiasing(BOOL bAntialiasing)
{
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartTitle::GetAntialiasing() const
{
	return m_bAntialiasing;
}
AFX_INLINE BOOL CXTPChartTitle::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE void CXTPChartTitle::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTTITLE_H__)
