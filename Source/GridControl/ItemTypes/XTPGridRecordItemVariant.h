// XTPGridRecordItemVariant.h: interface for the CXTPGridRecordItemVariant class.
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
#if !defined(__XTPGRIDRECORDITEMVARIANT_H__)
#	define __XTPGRIDRECORDITEMVARIANT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     Represents a variant item and inherits basic functionality
//     from the CXTPGridRecordItem class.
// Remarks:
//     You can create a bitmap record item simply by calling a constructor with
//     one parameter - a value of VARIANT type.
// See Also:
//     CXTPGridRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemVariant : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPGridRecordItemVariant)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record variant item.
	// Parameters:
	//     lpValue - Pointer to the VARIANT item value to set.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemVariant(const VARIANT& lpValue = COleVariant((long)0));

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
	CString GetCaption(CXTPGridColumn* pColumn);

	using CXTPGridRecordItem::GetCaption;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value for the item.
	// Parameters:
	//     var - Reference to the new COleVarient item value.
	//-----------------------------------------------------------------------
	void SetValue(const COleVariant& var);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value for the item.
	// Parameters:
	//     var - Reference to the new VARIANT item value.
	//-----------------------------------------------------------------------
	void SetValue(const VARIANT& var);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value for the item.
	// Parameters:
	//     var - Pointer to the new VARIANT item value.
	//-----------------------------------------------------------------------
	void SetValue(const VARIANT* var);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item.
	// Returns:
	//     The VARIANT item value.
	//-----------------------------------------------------------------------
	COleVariant GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares the current item with a provided item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the current item.
	//     pItem   - Pointer to the item to compare with the current item.
	// Remarks:
	//     Overwrites parents' member function.
	// Returns:
	//     Zero if the current item is equal to pItem;
	//     Less than zero if the current item is less than pItem;
	//     Greater than zero if the current item is greater than pItem.
	//-----------------------------------------------------------------------
	int Compare(CXTPGridColumn* pCol, CXTPGridRecordItem* pItem);

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
	//     This method is called when the value of the selected contraint of the
	//     in-place list is changed by the user.
	// Parameters:
	//     pItemArgs   - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                   structure containing item arguments.
	//     pConstraint - Pointer to the selected constraint.
	//-----------------------------------------------------------------------
	void OnConstraintChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs,
							 CXTPGridRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the value associated with the selected constraint.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	// Returns:
	//     The value associated with the selected constraint.
	//-----------------------------------------------------------------------
	DWORD GetSelectedConstraintData(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the value of the item is changed by the user.
	// Parameters:
	//     pItemArgs  - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                  structure containing item arguments.
	//     lpNewValue - New value to be set.
	// Returns:
	//     FALSE to cancel the edit operation.
	//-----------------------------------------------------------------------
	virtual BOOL OnValueChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPVARIANT lpNewValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the value of the in-place editor is
	//     changed by the user.
	// Parameters:
	//     pItemArgs   - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                   structure containing item arguments.
	//     rstrNewText - New string to be set in the editor.
	// Returns:
	//     FALSE to cancel the edit operation.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs, CString& rstrNewText);

protected:
#	ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
#	endif

private:
	static int CompareValues(const COleVariant& vt1, const COleVariant& vt2, BOOL bCaseSensitive);

public:
	COleVariant m_oleValue;   // Item value.
	static int m_nSortLocale; // Sort locale.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDRECORDITEMVARIANT_H__)
