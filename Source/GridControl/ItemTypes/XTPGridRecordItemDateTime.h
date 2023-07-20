// XTPGridRecordItemDateTime.h: interface for the CXTPGridRecordItemDateTime class.
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
#if !defined(__XTPGRIDRECORDITEMDATETIME_H__)
#	define __XTPGRIDRECORDITEMDATETIME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     Represents a time cell and inherits basic functionality
//     from the CXTPGridRecordItem class.
// Remarks:
//     You create a CXTPGridRecordItemDateTime record item simply by calling
//     its constructor with one parameter - its initial date/time item value.
// See Also:
//     CXTPGridRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemDateTime : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPGridRecordItemDateTime)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record date/time item.
	// Parameters:
	//     odtValue - Date/time item value to set.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemDateTime(COleDateTime odtValue = (DATE)0);

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
	//     This method is called when the value of the in-place edit control is changed.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item.
	// Returns:
	//     A date-time item value.
	//-----------------------------------------------------------------------
	COleDateTime GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value of the item.
	// Parameters:
	//     odtValue - New date-time item value.
	//-----------------------------------------------------------------------
	void SetValue(COleDateTime odtValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

private:
	static int CompareValues(const COleDateTime& dt1, const COleDateTime& dt2);

protected:
	COleDateTime m_odtValue; // Cell value.
};

AFX_INLINE COleDateTime CXTPGridRecordItemDateTime::GetValue()
{
	return m_odtValue;
}

AFX_INLINE void CXTPGridRecordItemDateTime::SetValue(COleDateTime odtValue)
{
	m_odtValue = odtValue;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDRECORDITEMDATETIME_H__)
