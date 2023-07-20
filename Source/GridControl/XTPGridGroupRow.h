// XTPGridGroupRow.h: interface for the CXTPGridGroupItem class.
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
#if !defined(__XTPGRIDGROUPROW_H__)
#	define __XTPGRIDGROUPROW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupContext;
class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//     This class represents a 'group row' of the Grid control view.
//     A 'group row' is a row which has a special text caption
//     instead of an associated record item.
// Remarks:
//     CXTPGridGroupRow has a slightly different reaction on mouse events
//     and another draw logic.
//     All other behavior of the group row is similar to its parent.
// See Also:
//     CXTPGridRow overview, CXTPGridControl overview
//===========================================================================
class _XTP_EXT_CLASS CXTPGridGroupRow : public CXTPGridRow
{
	DECLARE_DYNAMIC(CXTPGridGroupRow);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridGroupRow object.
	//-----------------------------------------------------------------------
	CXTPGridGroupRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridGroupRow object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridGroupRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption.
	// Returns:
	//     The text caption.
	// See Also:
	//     CXTPGridGroupRow::CXTPGridGroupRow
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the optional native group caption value.
	// Returns:
	//     The optional native group caption value.
	// See Also:
	//     CXTPGridGroupRow::CXTPGridGroupRow
	//-----------------------------------------------------------------------
	virtual COleVariant GetCaptionValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text.
	// Returns:
	//     The tooltip text.
	//-----------------------------------------------------------------------
	virtual CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text caption and, optionally, the group caption value.
	// Parameters:
	//     lpszCaption - Caption to be set.
	//     vtValue     - Optional group caption value.
	// Remarks:
	//     When a column that the GridRecordItem belongs to has been grouped,
	//     the caption of the column is displayed as the group caption.
	//     If you specify a caption with CXTPGridGroupRow::SetCaption for
	//     the GridRecordItem, then the m_strGroupText text will be
	//     displayed instead of the caption of the column. You can use
	//     CXTPGridGroupRow::SetCaption to "sub group" your GridRecordItems
	//     when the column that they belong to has been grouped. The
	//     GridRecordItems will be "sub grouped" by the value in
	//     m_strGroupText when the column that they belong to is grouped.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);
	void SetCaption(LPCTSTR lpszCaption,
					COleVariant vtValue); // <combine CXTPGridGroupRow::SetCaption@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//      Causes formula computed value to be updated.
	//-----------------------------------------------------------------------
	void UpdateValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group row formula.
	// Returns:
	//     The group row formula.
	//-----------------------------------------------------------------------
	virtual CString GetFormula() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group row formula.
	// Parameters:
	//     sFormula - New group row formula to be set.
	//-----------------------------------------------------------------------
	virtual void SetFormula(LPCTSTR sFormula);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group row caption format string.
	// Returns:
	//     The group row caption format string.
	//-----------------------------------------------------------------------
	virtual CString GetFormatString() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group row caption format string.
	// Parameters:
	//     strFormat - New group row caption format string to be set.
	//-----------------------------------------------------------------------
	virtual void SetFormatString(LPCTSTR strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group row on the specified device context.
	// Parameters:
	//     pDC         - Pointer to the device context in which the drawing occurs.
	//     rcRow       - Position of the row in client window coordinates.
	//     rcClip      - Position of the row in client window coordinates.
	//     nLeftOffset - Left offset of the drawing, in pixels (horizontal scroll position).
	//     mergeItems  - Reference to the set of merged grid records.
	//     nColumnFrom - Index of the column to begin drawing.
	//     nColumnTo   - Index of the column to end drawing.
	// See Also:
	//     CXTPGridRow::Draw
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, CRect rcRow, CRect rcClip, int nLeftOffset,
					  CXTPGridRecordMergeItems& mergeItems, int nColumnFrom, int nColumnTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     ptClicked - Point coordinates where the message was fired from.
	// See Also:
	//     CXTPGridRow::OnClick
	//-----------------------------------------------------------------------
	void OnClick(CPoint ptClicked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes double mouse clicks.
	// Parameters:
	//     ptClicked - Point coordinates where the message was fired from.
	// Remarks:
	//     The group row adds a collapse/expand function in the event
	//     that the double-click occurs on a row.
	// See Also:
	//     CXTPGridRow::OnDblClick
	//-----------------------------------------------------------------------
	void OnDblClick(CPoint ptClicked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this row is a group row.
	// Returns:
	//     TRUE since this row is a group row.
	// See Also:
	//     CXTPGridRow::IsGroupRow
	//-----------------------------------------------------------------------
	BOOL IsGroupRow() const;

	//{{AFX_CODEJOCK_PRIVATE
	BOOL CalculateByChilds(CXTPGridRow* pPassedRow, int col_start, int col_end,
						   double& dPassedValue);
	//}}AFX_CODEJOCK_PRIVATE
	CXTPMarkupUIElement* m_pMarkupUIElement; // Store markup object.

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the Markup element.
	// See Also:
	//     CXTPGridControl::EnableMarkup
	//-----------------------------------------------------------------------
	virtual void ResetMarkupUIElement();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a pointer to the parent Grid control.
	// Parameters:
	//     pControl - Pointer to the parent Grid control.
	// See Also:
	//     CXTPGridControl overview
	//-----------------------------------------------------------------------
	virtual void SetControl(CXTPGridControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a pointer to the LPCURRENCYFMT
	//     (i.e. the information that defines the format of a currency string).
	// Parameters:
	//     fmt - Pointer to the LPCURRENCYFMT.
	//-----------------------------------------------------------------------
	void SetCurrencyFormat(LPCURRENCYFMT fmt);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether
	//     a point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor. These
	//             coordinates are always relative to the upper-left
	//             corner of the window.
	//     pTI   - Pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, then the window control ID is returned.
	//     If the tooltip control was not found, then a value of -1 is returned.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a value into a formatted currency string using m_lpCurrencyFmt.
	// Parameters:
	//     value - Value to be converted.
	// Returns:
	//     The formatted currency string.
	//-----------------------------------------------------------------------
	CString GetCurrencyString(double value);

protected:
	CString m_strGroupText;		   // Group text label.
	CString m_strGroupLabel;	   // Group text label - used as formula prefix.
	CString m_strFormula;		   // The formula.
	CString m_strFormat;		   // The format string.
	COleVariant m_vtCaptionValue;  // Native caption value.
	LPCURRENCYFMT m_lpCurrencyFmt; // Contains information that defines the format of a currency
								   // string.

	friend class CXTPGridPaintManager;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridGroupRow);
	BSTR OleGetCaption();
	void OleSetCaption(LPCTSTR pcszCaption);

	BSTR OleGetFormat();
	void OleSetFormat(LPCTSTR pcszFormat);

	BSTR OleGetFormula();
	void OleSetFormula(LPCTSTR pcszFormula);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE BOOL CXTPGridGroupRow::IsGroupRow() const
{
	return TRUE;
}

AFX_INLINE CString CXTPGridGroupRow::GetTooltip() const
{
	return GetCaption();
}

AFX_INLINE CString CXTPGridGroupRow::GetFormula() const
{
	return m_strFormula;
}

AFX_INLINE void CXTPGridGroupRow::SetFormula(LPCTSTR sFormula)
{
	m_strFormula = sFormula;
}

AFX_INLINE CString CXTPGridGroupRow::GetFormatString() const
{
	return m_strFormat;
}

AFX_INLINE void CXTPGridGroupRow::SetFormatString(LPCTSTR strFormat)
{
	m_strFormat = strFormat;
}

AFX_INLINE void CXTPGridGroupRow::SetControl(CXTPGridControl* pControl)
{
	CXTPGridRow::SetControl(pControl);
	ResetMarkupUIElement();
}

AFX_INLINE void CXTPGridGroupRow::SetCurrencyFormat(LPCURRENCYFMT fmt)
{
	m_lpCurrencyFmt = fmt;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDGROUPROW_H__)
