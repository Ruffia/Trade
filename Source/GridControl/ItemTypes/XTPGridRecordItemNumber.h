// XTPGridRecordItemNumber.h: interface for the CXTPGridRecordItemNumber class.
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
#if !defined(__XTPGRIDRECORDITEMNUMBER_H__)
#	define __XTPGRIDRECORDITEMNUMBER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     This class represents a numeric cell and inherits basic functionality
//     from the CXTPGridRecordItem class.
// Remarks:
//     You create a number record item simply by calling its constructor with one
//     parameter - its numeric value. There is an additional constructor with a
//     second parameter - its format string. Format string allows you to determine
//     exactly in which format the numeric value will be represented on the screen.
//     It could be useful for displaying money values, etc.
// See Also:
//     CXTPGridRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemNumber : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPGridRecordItemNumber)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record numeric item with default formatting.
	// Parameters:
	//     dValue - Numeric item value to be set.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemNumber(double dValue = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record numeric item with specified formatting.
	// Parameters:
	//     dValue    - Numeric item value to be set.
	//     strFormat - Format string for creating the caption value of the item.
	// Remarks:
	//     If the format string is set, then the value of the item will
	//     be formatted according to this format string before it is drawn. This is
	//     convenient in many cases (e.g. when drawing dates). The format string is C like
	//     style; see the sprintf() C function or the CString.Format() member function.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemNumber(double dValue, LPCTSTR strFormat);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the text caption of the current item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the current item.
	// Remarks:
	//     Overwrites parents' member function.
	// Returns:
	//     The text caption of the current item.
	//-----------------------------------------------------------------------
	virtual CString GetCaption(CXTPGridColumn* pColumn);

	using CXTPGridRecordItem::GetCaption;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item.
	// Returns:
	//     The numeric item value.
	//-----------------------------------------------------------------------
	double GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value for the item.
	// Parameters:
	//     dValue - New numeric item value.
	//-----------------------------------------------------------------------
	void SetValue(double dValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the in-place edit control is changed.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares the current record item with a provided record item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the current record item.
	//     pItem   - Pointer to the record item to compare with the current record item.
	// Remarks:
	//     Overwrites parents' member function.
	// Returns:
	//     Zero if the current record item is equal to pItem;
	//     Less than zero if the current record item is less than pItem;
	//     Greater than zero if the current record item is greater than pItem.
	//-----------------------------------------------------------------------
	virtual int Compare(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the group caption native value.
	// Parameters:
	//     pColumn - Pointer to a CXTPGridColumn object.
	// Returns:
	//     The group caption native value.
	//-----------------------------------------------------------------------
	virtual COleVariant GetGroupCaptionValue(CXTPGridColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares the group caption of the current record item with
	//     the group caption of a provided group row.
	// Parameters:
	//     pColumn   - Pointer to the corresponding column of the current record item.
	//     pGroupRow - Pointer to the group row with the group caption to
	//                 compare with the group caption of the current record item.
	// Returns:
	//     Zero if the group caption of the current record item is
	//     identical to the group caption of pGroupRow;
	//     Less than zero if the group caption of the current record item is
	//     less than the group caption of pGroupRow;
	//     Greater than zero if the group caption of the current record item is
	//     greater than the group caption of pGroupRow.
	//-----------------------------------------------------------------------
	virtual int CompareGroupCaption(CXTPGridColumn* pColumn, CXTPGridGroupRow* pGroupRow);

	using CXTPGridRecordItem::CompareGroupCaption;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

private:
	static int CompareValues(double v1, double v2);

protected:
	double m_dValue; // Cell value.
};

//===========================================================================
// Summary:
//     This class represents a numeric percent cell and inherits basic functionality
//     from the CXTPGridRecordItemNumber class.
// See Also:
//     CXTPGridRecordItemNumber
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemPercentNumber : public CXTPGridRecordItemNumber
{
	DECLARE_SERIAL(CXTPGridRecordItemPercentNumber)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecordItemPercentNumber object using
	//     specified percentage and color values.
	// Parameters:
	//     dValue                  - Percentage value.
	//     clr                     - Color value.
	//     bPercentCompleteDisplay - TRUE to show as a colored rectangle inside.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemPercentNumber(double dValue = 0, COLORREF clr = RGB(224, 224, 224),
									BOOL bPercentCompleteDisplay = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     pMetrics  - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                 structure containing item metrics.
	//-----------------------------------------------------------------------
	void OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
					   XTP_GRIDRECORDITEM_METRICS* pMetrics);

public:
	COLORREF m_clr;					// Color.
	BOOL m_bPercentCompleteDisplay; // Flag.
};

AFX_INLINE void CXTPGridRecordItemNumber::SetValue(double dValue)
{
	m_dValue = dValue;
}

AFX_INLINE double CXTPGridRecordItemNumber::GetValue()
{
	return m_dValue;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDRECORDITEMNUMBER_H__)
