// XTPGridRecordItemText.h: interface for the CXTPGridRecordItemText class.
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
#if !defined(__XTPGRIDRECORDITEMTEXT_H__)
#	define __XTPGRIDRECORDITEMTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     This class represents a text-only cell and inherits basic functionality
//     from the CXTPGridRecordItem class.
// Remarks:
//     You create a text record item simply by calling a constructor with one
//     parameter - its text value.
// See Also:
//     CXTPGridRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemText : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPGridRecordItemText)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record text item.
	// Parameters:
	//     szText - Text item value to set.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemText(LPCTSTR szText = _T(""));

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
	//     szText - New text item value.
	//-----------------------------------------------------------------------
	void SetValue(LPCTSTR szText);

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
	//     The text item value.
	//-----------------------------------------------------------------------
	CString GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CString m_strText; // Item text value.
};

AFX_INLINE CString CXTPGridRecordItemText::GetValue()
{
	if (IsChildOfMerge())
	{
		if (GetMergeItem()->IsKindOf(RUNTIME_CLASS(CXTPGridRecordItemText)))
			return ((CXTPGridRecordItemText*)GetMergeItem())->m_strText;
		else
			return GetMergeItem()->GetCaption();
	}

	return m_strText;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORDITEMTEXT_H__)
