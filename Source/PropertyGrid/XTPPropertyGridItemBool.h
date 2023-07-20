// XTPPropertyGridItemBool.h interface for the CXTPPropertyGridItemBool class.
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
#if !defined(__XTPPROPERTYGRIDITEMBOOL_H__)
#	define __XTPPROPERTYGRIDITEMBOOL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPPropertyGridItemBool is a CXTPPropertyGridItem derived class.
//     It is used to create a BOOL value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemBool : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemBool object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     bValue     - Initial BOOL value of the item.
	//     pBindBool  - If not NULL, then the value of the item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemBool has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same ID in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToBool member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties. Bound variables allow the property grid to
	//     store data in variables. When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value. The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE: If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	// See Also:
	//     BindToBool
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemBool(LPCTSTR strCaption, BOOL bValue = FALSE, BOOL* pBindBool = NULL);
	CXTPPropertyGridItemBool(
		UINT nID, BOOL bValue = FALSE,
		BOOL* pBindBool =
			NULL); // <COMBINE
				   // CXTPPropertyGridItemBool::CXTPPropertyGridItemBool@LPCTSTR@BOOL@BOOL*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemBool object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemBool();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the BOOL value for the item.
	// Parameters:
	//     bValue - BOOL value to be set.
	//-----------------------------------------------------------------------
	virtual void SetBool(BOOL bValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the BOOL value of the item.
	// Returns:
	//     The BOOL value of the item
	//     (i.e. TRUE if the item's value is TRUE, FALSE if the item's value is FALSE).
	//-----------------------------------------------------------------------
	virtual BOOL GetBool() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified BOOL value.
	// Parameters:
	//     pBindBool - Pointer to a BOOL value.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToBool member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties. Bound variables allow the property grid to
	//     store data in variables. When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value. The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE: If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	//-----------------------------------------------------------------------
	virtual void BindToBool(BOOL* pBindBool);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the values for the TRUE and FALSE labels in the drop list.
	// Parameters:
	//     strTrueText  - NULL-terminated string that represents the TRUE label.
	//     strFalseText - NULL-terminated string that represents the FALSE label.
	// Remarks:
	//     By default, the values for the TRUE and FALSE labels in the drop list
	//     are set to "True" and "False" respectively.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetTrueFalseText(LPCTSTR strTrueText, LPCTSTR strFalseText);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the item should be drawn as a check box frame.
	// Parameters:
	//     bCheckBoxStyle - TRUE to draw the item as a check box frame, FALSE otherwise.
	// See Also:
	//     IsCheckBoxStyle
	//-----------------------------------------------------------------------
	void SetCheckBoxStyle(BOOL bCheckBoxStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is drawn as a check box frame.
	// Returns:
	//     TRUE if the item is drawn as a check box frame, otherwise FALSE.
	// See Also:
	//     SetCheckBoxStyle
	//-----------------------------------------------------------------------
	BOOL IsCheckBoxStyle() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value of the item.
	//     Override this method to add new functionality. You should call
	//     the base class version of this function from your override.
	// Parameters:
	//     strValue - Value to be set.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Searches the drop list for labels with the value specified by strValueText
	//     and updates them with the value specified by strNewText.
	// Parameters:
	//     strValueText - NULL-terminated string that represents the current value of
	//                    labels that must be updated.
	//     strNewText   - NULL-terminated string that represents the new value for
	//                    labels that must be updated.
	// Remarks:
	//     If the value specified by strValueText is also the current value of the
	//     property grid item, then the value of that item will be updated as well.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetValueText(CString& strValueText, LPCTSTR strNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first checked to see if it
	//     is bound to a variable. If it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//     OnBeforeInsert is called when an item is inserted,
	//     when a category is inserted, when a category or item is expanded,
	//     and when the sort property has changed.
	//-----------------------------------------------------------------------
	virtual void OnBeforeInsert();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is drawn.
	//     Override this method if needed.
	// Parameters:
	//     dc      - Reference to the device context to be used
	//               for rendering an image of the item.
	//     rcValue - Bounding rectangle of the item.
	// Returns:
	//     TRUE if the item is self-drawn.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item that the user is currently viewing.
	// Remarks:
	//     The return value of this method includes the formatting used for
	//     the value of the item. For example, if a password mask is shown,
	//     then the return value of this method would be a CString filled
	//     with asterisks (*). Otherwise, if a password mask is not shown,
	//     the return value would be the same as the GetValue method.
	// Returns:
	//     The value of the item that the user is currently viewing.
	//-----------------------------------------------------------------------
	virtual CString GetViewValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the
	//     left mouse button on the item.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y- coordinates of the cursor.
	// Returns:
	//     TRUE if the message was processed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses a key.
	// Parameters:
	//     nChar - Specifies the virtual-key code of the given key.
	// Remarks:
	//     This method checks if the item has a checkbox style, is not read-only,
	//     and if the user pressed the Spacebar key. If these three criteria
	//     are met, then the checkbox will be be checked/un-checked.
	// Returns:
	//     TRUE if the item has a checkbox style, is not read-only,
	//     and if the user pressed the Spacebar key, otherwise FALSE.
	// See Also:
	//     PtInCheckBoxRect
	//-----------------------------------------------------------------------
	virtual BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user double-clicks the
	//     left mouse button on the item.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified point is inside the checkbox
	//     displayed if the item has a checkbox style.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     TRUE if the specified point is inside the checkbox, otherwise FALSE.
	// See Also:
	//     OnKeyDown
	//-----------------------------------------------------------------------
	BOOL PtInCheckBoxRect(CPoint point);

private:
	void _Init(BOOL bValue);

protected:
	BOOL m_bValue;			// Value of the item.
	BOOL* m_pBindBool;		// Pointer to the variable bound to this item.
	CString m_strTrueText;  // Text of the TRUE label.
	CString m_strFalseText; // Text of the FALSE label.
	BOOL m_bCheckBoxStyle;  // Checkbox style.

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemBool)

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemDouble)

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemEnum is a CXTPPropertyGridItem derived class.
//     It is used to create an enum value item in a Property Grid control.
// Remarks:
//     An enum item is used to display a list of constraints. This is
//     very similar to a normal constraint list. The difference is that
//     an enum constraint list will return the value of the constraint when
//     selected, while a normal constraint will return the text of the constraint.
//
// Example:
//     This code illustrates how to add an item of type enum to your grid:
// <code>
// //Adds a category to the grid called "Software"
// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Software"));
//
// //Adds a CXTPPropertyGridItemFlags item with a caption of "Current OS" and an initial value of 2
// //This will cause the constraint with a value of 2 to be selected
// CXTPPropertyGridItemEnum* pItem = (CXTPPropertyGridItemEnum*)(pStandard->AddChildItem(new
// CXTPPropertyGridItemEnum(_T("Current OS"), 2)));
//
// //Adds some constraints along with a value
// pItem->GetConstraints()->AddConstraint(_T("Windows 98"), 1);
// pItem->GetConstraints()->AddConstraint(_T("Windows 2000"), 2);
// pItem->GetConstraints()->AddConstraint(_T("Windows XP"), 3);
//
// //GetEnum will return the Data value of the constraint selected
// TRACE(_T("Enum Item Value= %d\n"), pItem->GetEnum());
//
// //GetValue will return the String of the constraint selected
// TRACE(_T("Enum Item String= %s\n"), pItem->GetValue());
// </code>
//
// See Also:
//     CXTPPropertyGridItemEnum::SetEnum, CXTPPropertyGridItemEnum::GetEnum,
//     CXTPPropertyGridItem::GetValue
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemEnum : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemEnum object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     nValue     - Initial value of the item;
	//                  this should be the value of a constraint.
	//     pBindEnum  - If not NULL, then the value of this item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemEnum has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same ID in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToEnum member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties. Bound variables allow the property grid to
	//     store data in variables. When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value. The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE: If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	// See Also:
	//     BindToEnum
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemEnum(LPCTSTR strCaption, int nValue = 0, int* pBindEnum = NULL);
	CXTPPropertyGridItemEnum(
		UINT nID, int nValue = 0,
		int* pBindEnum =
			NULL); // <COMBINE CXTPPropertyGridItemEnum::CXTPPropertyGridItemEnum@LPCTSTR@int@int*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemEnum object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemEnum();

	//-----------------------------------------------------------------------
	// Summary:
	//     The first method sets the value for the item.
	//     The second method is called when user clicks an item in the constraint list.
	// Parameters:
	//     nValue      - Value to be set;
	//                   this should be the value of a constraint.
	//     pConstraint - Pointer to the currently selected constraint.
	// See Also:
	//     CXTPPropertyGridItemEnum, GetEnum
	//-----------------------------------------------------------------------
	virtual void SetEnum(int nValue);
	virtual void SetEnum(
		CXTPPropertyGridItemConstraint* pConstraint); // <COMBINE
													  // CXTPPropertyGridItemEnum::SetEnum@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item.
	// Returns:
	//     The value of the item
	//     (i.e. the value of the selected constraint (not the text)).
	// See Also:
	//     CXTPPropertyGridItemEnum, SetEnum
	//-----------------------------------------------------------------------
	virtual int GetEnum() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified integer value.
	// Parameters:
	//     pBindEnum - Pointer to an integer value.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToEnum member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties. Bound variables allow the property grid to
	//     store data in variables. When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value. The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE: If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	//-----------------------------------------------------------------------
	virtual void BindToEnum(int* pBindEnum);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value of the item.
	//     Override this method to add new functionality. You should call
	//     the base class version of this function from your override.
	// Parameters:
	//     strValue - Value to be set.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first checked to see if it
	//     is bound to a variable. If it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//     OnBeforeInsert is called when an item is inserted,
	//     when a category is inserted, when a category or item is expanded,
	//     and when the sort property has changed.
	//-------------------------------------------------------------------------
	virtual void OnBeforeInsert();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when constraints are added to the item
	//     (i.e. when CXTPPropertyGridItemConstraints::AddConstraint is called).
	// See Also:
	//     CXTPPropertyGridItemConstraints::AddConstraint
	//-------------------------------------------------------------------------
	virtual void OnConstraintsChanged();

private:
	void _Init(int nValue);

protected:
	int m_nValue;	 // Value of the item. This will contain the value of the
					  // selected constraint (not the text).
	int* m_pBindEnum; // Pointer to the variable bound to this item.

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemEnum)

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPPropertyGridItemBool::GetBool() const
{
	return m_bValue;
}
AFX_INLINE int CXTPPropertyGridItemEnum::GetEnum() const
{
	return m_nValue;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDITEMBOOL_H__)
