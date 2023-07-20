// XTPPropertyGridItem.h interface for the CXTPPropertyGridItem class.
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
#if !defined(__XTPPROPERTYGRIDITEM_H__)
#	define __XTPPROPERTYGRIDITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// class forwards.

class CXTPPropertyGridView;
class CXTPPropertyGridItem;
class CXTPPropertyGridItemMetrics;
class CXTPImageManagerIcon;
class CXTPPropertyGridInplaceButtons;
class CXTPPropertyGridInplaceSlider;
class CXTPPropertyGridInplaceSpinButton;
class CXTPPropertyGridInplaceControls;
class CXTPMarkupUIElement;
class CXTPPropertyGridInplaceEdit;
class CXTPPropertyGridInplaceList;
class CXTPPropertyGridInplaceButton;

//-----------------------------------------------------------------------
// Summary:
//     This constant is used to determine the "gutter" width of the
//     property grid. This is the area on the left side of the
//     grid that the expand buttons are drawn in.
// Remarks:
//     This is useful when drawing owner drawn controls such as buttons
//     and drop-down menus.
// Example:
//     The following example illustrates using XTP_PGI_EXPAND_BORDER:
// <code>
// void CDelphiGridPage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
// {
//     // code snip ...
//
//     CRect rcText(rc);
//     rcText.left = max(1, pItem->GetIndent()) * XTP_DPI_X(XTP_PGI_EXPAND_BORDER) + 3;
//     rcText.right = rcCaption.right - 1;
//     rcText.bottom -= 1;
//     dc.DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER);
// }
// </code>
//-----------------------------------------------------------------------
extern _XTP_EXT_CLASS const int XTP_PGI_EXPAND_BORDER;
const TCHAR XTP_PG_ITEMVALUE_TO_REPLACE_IN_MARKUP[] = _T("%value%");

//-----------------------------------------------------------------------
// Summary:
//     Flags that indicate which styles to set for the grid item.
// Example:
//     <code>pItem->SetFlags(xtpPropertyGridItemHasEdit | xtpPropertyGridItemHasComboButton);</code>
// See Also:
//     CXTPPropertyGridItem::SetFlags, OnInplaceButtonDown
//
// <KEYWORDS xtpPropertyGridItemHasEdit, xtpPropertyGridItemHasExpandButton,
// xtpPropertyGridItemHasComboButton>
//-----------------------------------------------------------------------
enum XTPPropertyGridItemFlags
{
	xtpPropertyGridItemHasEdit		   = 1, // Item has an edit control.
	xtpPropertyGridItemHasExpandButton = 2, // Item has an expand button.
	xtpPropertyGridItemHasComboButton  = 4  // Item has a combo button.
};

//{{AFX_CODEJOCK_PRIVATE
_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridItemHasEdit)
const XTPPropertyGridItemFlags xtpGridItemHasEdit = xtpPropertyGridItemHasEdit;

_XTP_DEPRECATED_IN_FAVOR(xtpGridItemHasExpandButton)
const XTPPropertyGridItemFlags xtpGridItemHasExpandButton = xtpPropertyGridItemHasExpandButton;

_XTP_DEPRECATED_IN_FAVOR(xtpGridItemHasComboButton)
const XTPPropertyGridItemFlags xtpGridItemHasComboButton = xtpPropertyGridItemHasComboButton;
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPPropertyGridItems is a CList derived class. It represents the
//     items collection with some internal functionality.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItems : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves an item by its identifier/caption.
	// Parameters:
	//     nID        - Indentifier of the item to retrieve.
	//     strCaption - Caption of the item to retrieve.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object if found, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* FindItem(UINT nID) const;
	CXTPPropertyGridItem*
		FindItem(LPCTSTR strCaption) const; // <COMBINE CXTPPropertyGridItems::FindItem@UINT@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified item within the collection of items.
	// Parameters:
	//     pItem - Pointer to the CXTPPropertyGridItem object
	//             whose index must be retrieved.
	// Returns:
	//     The index of the specified item within the collection of items
	//     if found, otherwise a value of -1.
	//-----------------------------------------------------------------------
	int Find(CXTPPropertyGridItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears all items from the collection of items.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of items within the collection of items.
	// Returns:
	//     The number of items within the collection of items.
	//-----------------------------------------------------------------------
	long GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if any items have been added to the property grid.
	// Returns:
	//     TRUE if there are no items in the property grid,
	//     FALSE if there is at least one item in the property grid.
	//-----------------------------------------------------------------------
	BOOL IsEmpty() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item at a specified index within the collection of items.
	// Parameters:
	//     nIndex - Index of the item to retrieve.
	// Returns:
	//     A pointer to the CXTPPropertyGridItem object at the specified index
	//     if successful, otherwise a value of 0.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the item at a specified index within the collection of items.
	// Parameters:
	//     nIndex - Index of the item to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts the collection of items in alphabetical order.
	// Remarks:
	//     This method uses the Visual C++ run-time library (MSVCRT)
	//     implementation of the quick-sort function, qsort, for sorting
	//     stored CXTPPropertyGridItem objects.
	//-----------------------------------------------------------------------
	void Sort();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItems object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds an item/collection of items to the end (tail) of the
	//     property grid's collection of items.
	// Parameters:
	//     pItem  - Pointer to an item to add.
	//     pItems - Pointer to a collection of items to add.
	// Remarks:
	//     Property grid's collection of items can be empty prior to calling this method.
	//-----------------------------------------------------------------------
	void AddTail(CXTPPropertyGridItem* pItem);
	void AddTail(
		CXTPPropertyGridItems* pItems); //<combine
										// CXTPPropertyGridItems::AddTail@CXTPPropertyGridItem*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified item at a specified index within the
	//     property grid's collection of items.
	// Parameters:
	//     nIndex - Index to insert the item at.
	//     pItem  - Pointer to the item to insert.
	//-----------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPPropertyGridItem* pItem);

private:
	static int AFX_CDECL CompareFunc(const CXTPPropertyGridItem** ppItem1,
									 const CXTPPropertyGridItem** ppItem2);

private:
	CArray<CXTPPropertyGridItem*, CXTPPropertyGridItem*> m_arrItems;
	// CXTPPropertyGridView* m_pGrid;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItems)
	DECLARE_ENUM_VARIANT(CXTPPropertyGridItems)

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleItem(long nIndex);
	afx_msg LPDISPATCH OleGetItem(long nIndex);

	afx_msg void OleClear();
	afx_msg void OleRemove(long nIndex);

	enum
	{
		dispidCount  = 1L,
		dispidRemove = 2L,
		dispidClear  = 3L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPPropertyGridItem;
	friend class CXTPPropertyGridView;
	friend class CXTPPropertyGrid;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemConstraint is a CXTPCmdTarget derived class.
//     It represents a single item constraint.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemConstraint : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemConstraint object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemConstraint();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of this constraint within the collection of constraints.
	// Returns:
	//     The index of this constrant within the collection of constraints.
	//-----------------------------------------------------------------------
	virtual int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the image of this constraint so that it can be used
	//     outside of Codejock components.
	// Returns:
	//     A pointer to a CXTPImageManagerIcon object containing
	//     the image of this constraint.
	//-----------------------------------------------------------------------
	virtual CXTPImageManagerIcon* GetImage() const;

public:
	CString m_strConstraint; // Caption text of this constraint. This is the
							 // text displayed for this constraint.
	DWORD_PTR m_dwData;		 // The 32-bit value associated with the item.
	int m_nImage;			 // Image index.
	BOOL m_bEnabled;		 // TRUE if this constraint is enabled.
protected:
	int m_nIndex;				   // Index of this constraint.
	CXTPPropertyGridItem* m_pItem; // Parent item.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemConstraint);

//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPPropertyGridItemConstraints;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemConstraints is a CXTPCmdTarget derived class.
//     It represents the item constraints collection.
// Remarks:
//     Each PropertyGridItem has its own collection of constraints.
//     The CXTPPropertyGridItem::GetConstraints method allows
//     for access to the constraints of a single item.
//     If a combo button is added to the property grid item, then all
//     the constraints in the item's constraints collection will be displayed
//     in the drop-down list. The AddConstraint method is used to add
//     constraints to the collection.
// See Also:
//     CXTPPropertyGridItem::GetConstraints, CXTPPropertyGridItem::SetConstraintEdit,
//     AddConstraint
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemConstraints : public CXTPCmdTarget
{
protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemConstraints object.
	// Parameters:
	//     pItem - Pointer to the parent CXTPPropertyGridItem object.
	//-------------------------------------------------------------------------
	CXTPPropertyGridItemConstraints(CXTPPropertyGridItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemConstraints object,
	//     handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPPropertyGridItemConstraints();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified constraint to the collection of constraints.
	// Parameters:
	//     str    - Constraint to add.
	//     dwData - Data associated with the constraint.
	//     nImage - Image of the constraint.
	// Returns:
	//     A pointer to the newly added constraint.
	// See Also:
	//     CXTPPropertyGridItem::OnConstraintsChanged
	//-----------------------------------------------------------------------
	virtual CXTPPropertyGridItemConstraint* AddConstraint(LPCTSTR str, DWORD_PTR dwData = 0,
														  int nImage = -1);

	//-------------------------------------------------------------------------
	// Summary:
	//     Gets the number of constraints in the collection.
	// Returns:
	//     The number of constraints in the collection.
	//-------------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if any constraints have been added to the collection.
	// Returns:
	//     TRUE if there are no constraints in the collection,
	//     FALSE if there is at least one constraint in the collection.
	//-----------------------------------------------------------------------
	BOOL IsEmpty() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all constraints from the collection.
	// See Also:
	//     RemoveAt
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the constraint at a specified index within the collection.
	// Parameters:
	//     nIndex - Index of the constraint to remove.
	// See Also:
	//     RemoveAll
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current constraint.
	// Parameters:
	//     nIndex - Index of the constraint to be set.
	//-----------------------------------------------------------------------
	void SetCurrent(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the current constraint.
	// Returns:
	//     The index of the current constraint.
	//-----------------------------------------------------------------------
	int GetCurrent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified constraint.
	// Parameters:
	//     str    - Constraint with the index to retrieve.
	//     dwData - Data associated with the constraint with the index to retrieve.
	// Returns:
	//     The zero-based index of the constraint if successful, otherwise a value of -1.
	//-----------------------------------------------------------------------
	int FindConstraint(LPCTSTR str) const;
	int FindConstraint(DWORD_PTR dwData)
		const; // <COMBINE CXTPPropertyGridItemConstraints::FindConstraint@LPCTSTR@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption of the constraint at a specified index
	//     within the collection.
	// Parameters:
	//     nIndex - Index of the constraint with the caption to retrieve.
	// Remarks:
	//     This method only retrieves the caption of the constaint at
	//     a specified index. To retrieve the entire constraint at
	//     a specified index, use the GetConstraintAt method.
	// Returns:
	//     The caption of the constraint at the specified index
	//     if successful, otherwise "".
	// See Also:
	//     GetConstraintAt
	//-----------------------------------------------------------------------
	CString GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the constraint at a specified index within the collection.
	// Parameters:
	//     nIndex - Index of the constraint to retrieve.
	// Remarks:
	//     This method retrieves the entire constraint at a specified index.
	//     To retrieve only the caption of the constaint at a specified index,
	//     use the GetAt method.
	// Returns:
	//     A pointer to the constraint at the specified index.
	// See Also:
	//     GetAt
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemConstraint* GetConstraintAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts constraints alphabetically.
	//-----------------------------------------------------------------------
	virtual void Sort();

private:
	static int AFX_CDECL CompareFunc(const CXTPPropertyGridItemConstraint** ppConstraint1,
									 const CXTPPropertyGridItemConstraint** ppConstraint2);

private:
	CArray<CXTPPropertyGridItemConstraint*, CXTPPropertyGridItemConstraint*> m_arrConstraints;
	int m_nCurrent;
	CXTPPropertyGridItem* m_pItem;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemConstraints)

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleItem(long nIndex);
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg void OleClear();
	afx_msg void OleRemove(long nIndex);
	afx_msg void OleAdd(LPCTSTR strItem, const VARIANT* Data);

	DECLARE_ENUM_VARIANT(CXTPPropertyGridItemConstraints)

	enum
	{
		dispidCount  = 1L,
		dispidRemove = 2L,
		dispidAdd	= 3L,
		dispidClear  = 4L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPPropertyGridItem;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItem is the base class representing
//     an item of the Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItem
	: public CXTPCmdTarget
	, public CXTPAccessible
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItem object
	// Parameters:
	//     lpszCaption   - Caption of the item.
	//     nID           - Identifier of the item.
	//     strValue      - Initial value of the item.
	//     pBindString   - If not NULL, then the value of this item
	//                     is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItem has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same ID in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToString member.
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
	//     BindToString
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem(LPCTSTR lpszCaption, LPCTSTR strValue = NULL, CString* pBindString = NULL);
	CXTPPropertyGridItem(
		UINT nID, LPCTSTR strValue = NULL,
		CString* pBindString =
			NULL); // <COMBINE CXTPPropertyGridItem::CXTPPropertyGridItem@LPCTSTR@LPCTSTR@CString*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItem object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption text for the item.
	// Parameters:
	//     lpszCaption - Caption text to be set.
	// See Also:
	//     SetPrompt
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a string from a STRINGTABLE resource to set the
	//     caption and description for the item.
	// Parameters:
	//     lpszPrompt - String from a STRINGTABLE resource that contains the
	//                  caption and description of the item. The
	//                  caption and description must be separated by "\\n"
	//                  if both are included. The description
	//                  can be omitted in order to only set the caption.
	// Remarks:
	//     Resource strings are in the format "Caption\\nDescription".
	// See Also:
	//     SetCaption, SetDescription
	//-----------------------------------------------------------------------
	void SetPrompt(LPCTSTR lpszPrompt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the description text for the item.
	// Parameters:
	//     lpszDescription - Description text to be set.
	// Remarks:
	//     The description text is displayed in the help panel.
	// See Also:
	//     SetPrompt
	//-----------------------------------------------------------------------
	void SetDescription(LPCTSTR lpszDescription);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the markup text for the item.
	// Parameters:
	//     lpstrMarkupTemplate - Markup template to display when markup is on.
	//                           Use the XTP_PG_ITEMVALUE_TO_REPLACE_IN_MARKUP string
	//                           for the item's plain text value substitution.
	//-----------------------------------------------------------------------
	void SetMarkupTemplate(LPCTSTR lpstrMarkupTemplate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text for the item.
	// Parameters:
	//     lpstrTooltip - Tooltip text to be set.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpstrTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the item.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	// Returns:
	//     The tooltip text of the item.
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the item state should be set to read-only or to read/write.
	// Parameters:
	//     bReadOnly - TRUE to set the item state to read-only,
	//                 FALSE to set the item state to read/write.
	//-----------------------------------------------------------------------
	virtual void SetReadOnly(BOOL bReadOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the identifier for the item.
	// Parameters:
	//     nID - Identifier to be set.
	//-----------------------------------------------------------------------
	void SetID(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a specific flag for the item.
	// Parameters:
	//     nFlags - Flag to set; can be any of the values listed in the Remarks section.
	// Remarks:
	//     nFlags value can be one or more of the following:
	//      * <b>xtpPropertyGridItemHasEdit</b>: Item has an edit control.
	//      * <b>xtpPropertyGridItemHasExpandButton</b>: Item has an expand button.
	//      * <b>xtpPropertyGridItemHasComboButton</b>: Item has a combo button.
	// See Also:
	//     XTPPropertyGridItemFlags, OnInplaceButtonDown
	//-----------------------------------------------------------------------
	void SetFlags(UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption text of the item.
	// Returns:
	//     The caption text of the item.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the description text of the item.
	// Remarks:
	//     The description text is displayed in the help panel.
	// Returns:
	//     The description text of the item.
	//-----------------------------------------------------------------------
	CString GetDescription() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the markup text of the item.
	// Returns:
	//     The markup text of the item.
	//-----------------------------------------------------------------------
	virtual CString GetMarkupTemplate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item state is set to read-only or to read/write.
	// Returns:
	//     TRUE if the item state is set to read-only,
	//     FALSE if the item state is set to read/write.
	//-----------------------------------------------------------------------
	virtual BOOL GetReadOnly() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if items in the property grid are allowed to be edited by the user.
	// Returns:
	//     TRUE if items in the property grid are allowed to be edited by the user,
	//     FALSE if items in the property grid are not allowed to be edited by the user.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the item.
	// Returns:
	//     The identifier of the item.
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the item within the property grid list.
	// Returns:
	//     The index of the item within the property grid list.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item.
	// Returns:
	//     The value of the item.
	//-----------------------------------------------------------------------
	CString GetValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default value of the item.
	// Returns:
	//     The default value of the item.
	//-----------------------------------------------------------------------
	CString GetDefaultValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the flags of the item.
	// Remarks:
	//     The value returned by this method should be a member
	//     of XTPPropertyGridItemFlags.
	// Returns:
	//     The flags of the item.
	// See Also:
	//     XTPPropertyGridItemFlags
	//-----------------------------------------------------------------------
	UINT GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is a category.
	// Returns:
	//     TRUE if the item is a category, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsCategory() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a child item.
	// Parameters:
	//     pItem - Pointer to the CXTPPropertyGridItem object to add as a child.
	// Returns:
	//     A pointer to the newly added CXTPPropertyGridItem child object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* AddChildItem(CXTPPropertyGridItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the item to another category.
	// Parameters:
	//     pParent - Pointer to the category to move the item to.
	//     nIndex  - Index to insert the item at.
	//-----------------------------------------------------------------------
	void Move(CXTPPropertyGridItem* pParent, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a child item.
	// Parameters:
	//     pItem  - Pointer to the CXTPPropertyGridItem object to insert as a child.
	//     nIndex - Index to insert the item at.
	// Returns:
	//     A pointer to the newly inserted CXTPPropertyGridItem child object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* InsertChildItem(int nIndex, CXTPPropertyGridItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item has children.
	// Returns:
	//     TRUE if the item has children, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasChilds() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item has visible children.
	// Returns:
	//     TRUE if the item has visible children, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasVisibleChilds() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item's list of child items.
	// Returns:
	//     A pointer to the item's list of child items.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItems* GetChilds() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item has a specified item as a parent.
	// Parameters:
	//     pParent - Pointer to a CXTPPropertyGridItem object.
	// Returns:
	//     TRUE if the item has the specified item as a parent, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasParent(CXTPPropertyGridItem* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item's list of constraints.
	// Returns:
	//     A pointer to the item's list of constraints.
	// Example:
	// <code> CXTPPropertyGridItemConstraints* pList = pItem->GetConstraints(); </code>
	// See Also:
	//     CXTPPropertyGridItemConstraints::AddConstraint, OnConstraintsChanged
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemConstraints* GetConstraints() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables constraint editing mode.
	// Parameters:
	//     bConstraintEdit - TRUE to enable constraint editing mode,
	//                       FALSE to disable constraint editing mode.
	// Remarks:
	//     If constraint editing mode is enabled, then only values from
	//     the constraint list can be input to the edit.
	//-----------------------------------------------------------------------
	void SetConstraintEdit(BOOL bConstraintEdit = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables auto-complete for the constraint list.
	// Parameters:
	//     bAutoComplete - TRUE to enable auto-complete,
	//                     FALSE to disable auto-complete.
	//-----------------------------------------------------------------------
	void SetAutoComplete(BOOL bAutoComplete = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if constraint editing mode is enabled/disabled.
	// Remarks:
	//     If constraint editing mode is enabled, then only values from
	//     the constraint list can be input to the edit.
	// Returns:
	//     TRUE if constraint editing mode is enabled,
	//     FALSE if constraint editing mode is disabled.
	//-----------------------------------------------------------------------
	BOOL GetConstraintEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands the item.
	//-----------------------------------------------------------------------
	void Expand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses the item.
	//-----------------------------------------------------------------------
	void Collapse();

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the item should be expandable by the user.
	// Parameters:
	//     bExpandable - TRUE to set the item to be expandable by the user, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is expandable by the user.
	// Returns:
	//     TRUE if the item is expandable by the user, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpandable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects the item.
	//-----------------------------------------------------------------------
	void Select();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the item.
	// Returns:
	//     The bounding rectangle of the item.
	//-----------------------------------------------------------------------
	CRect GetItemRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the item value's text.
	//     Override this method to change it.
	// Returns:
	//     The bounding rectangle of the item value's text.
	//-----------------------------------------------------------------------
	virtual CRect GetValueRect();

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
	//     Hides/shows the item.
	// Parameters:
	//     bHidden - TRUE to hide the item, FALSE to show the item.
	//-----------------------------------------------------------------------
	void SetHidden(BOOL bHidden);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is hidden/shown.
	// Returns:
	//     TRUE if the item is hidden, FALSE if the item is shown.
	//-----------------------------------------------------------------------
	BOOL IsHidden() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the item is changed.
	//     Override this method if needed.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged(CString strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the markup text of the item is changed.
	//     Override this method if needed.
	//-----------------------------------------------------------------------
	virtual void OnMarkupChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent window.
	// Returns:
	//     A pointer to the parent window.
	//-----------------------------------------------------------------------
	CXTPPropertyGridView* GetGrid() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visible state of the item.
	// Returns:
	//     TRUE if the item is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the expanded state of the item.
	// Returns:
	//     TRUE if the item is expanded, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent item of this item, if any.
	// Returns:
	//     A pointer to the parent item of this item, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetParentItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indent of the item.
	// Returns:
	//     The indent of the item.
	//-----------------------------------------------------------------------
	int GetIndent() const;

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
	//     This method is called when a constraint in an in-place list is drawn.
	//     Override this method if needed.
	// Parameters:
	//     pDC         - Pointer to the device context to be used
	//                   for rendering an image of the item.
	//     pConstraint - Pointer to the constraint to be drawn.
	//     rc          - Bounding rectangle of the constraint.
	//     bSelected   - TRUE if the item is selected.
	//-----------------------------------------------------------------------
	virtual void OnDrawItemConstraint(CDC* pDC, CXTPPropertyGridItemConstraint* pConstraint,
									  CRect rc, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get size of constraint in in-place list.
	//     Override this function if needed.
	// Parameters:
	//     pDC         - Reference to the device context to be used for rendering
	//                   an image of the item.
	//     pConstraint - Constraint to be drawn
	//-----------------------------------------------------------------------
	virtual CSize OnMergeItemConstraint(CDC* pDC, CXTPPropertyGridItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method and fill in a specified MEASUREITEMSTRUCT structure
	//     to inform Windows of the list-box dimensions.
	// Parameters:
	//     lpMeasureItemStruct - Long pointer to a MEASUREITEMSTRUCT structure.
	//-----------------------------------------------------------------------
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the selected stae of the item.
	// Returns:
	//     TRUE if the item is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified CString object.
	// Parameters:
	//     pBindString - Pointer to a CString object.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToString member.
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
	void BindToString(CString* pBindString);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes this item.
	//-----------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a specified mask to a CXTPPropertyGridItem data field
	//     to create a restrictive field for text entry.
	// Parameters:
	//     strMask    - Mask of the item.
	//     strLiteral - Defines the prompt area of the mask where text entry
	//                  is allowed. This value should match the areas that
	//                  you have already defined as mask fields. By default,
	//                  the prompt character is '_'.
	//
	//                  Phone No: (___) ___-____
	//
	//
	//     chPrompt   - (Optional) This value is the prompt character used
	//                  in strLiteral to indicate a place holder. By default,
	//                  this parameter is the underscore '_' character.
	// Remarks:
	//     You can use any of these characters for defining your edit mask:
	//     <TABLE>
	//          <b>Mask Character</b>  <b>Description</b>
	//          ---------------------  ------------------------
	//          0                      Numeric (0-9)
	//          9                      Numeric (0-9) or space (' ')
	//          #                      Numeric (0-9) or space (' ') or ('+') or ('-')
	//          L                      Alpha (a-Z)
	//          ?                      Alpha (a-Z) or space (' ')
	//          A                      Alpha numeric (0-9 and a-Z)
	//          a                      Alpha numeric (0-9 and a-Z) or space (' ')
	//          &                      All print character only
	//          H                      Hex digit (0-9 and A-F)
	//          X                      Hex digit (0-9 and A-F) and space (' ')
	//          >                      Forces characters to upper case (A-Z)
	//          <                      Forces characters to lower case (a-z)
	//     </TABLE>
	//
	//      Any of these characters can be combined with additional
	//      characters to create a restrictive field for text entry.
	//      For example, if you wanted to display a prompt for a
	//      telephone number and wanted to restrict only numeric
	//      values, but wanted to display the area code inside of
	//      parentheses '(' and ')' then you could define your
	//      mask as:
	//
	//      Phone No: (000) 000-0000
	//
	// See Also:
	//      GetMaskedText, SetMaskedText
	//-----------------------------------------------------------------------
	void SetMask(LPCTSTR strMask, LPCTSTR strLiteral, TCHAR chPrompt = _T('_'));

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a password mask for the item.
	// Parameters:
	//     chMask - Character to be used as the password mask; by default,
	//              this parameter is the asterisk (*) character.
	// Remarks:
	//     The password mask is the character used to replace the characters
	//     of the item's text when displayed.
	//     The actual text value of the item can be retrieved by using
	//     the GetMaskedText method.
	// See Also:
	//     GetMaskedText
	//-----------------------------------------------------------------------
	void SetPasswordMask(TCHAR chMask = _T('*'));

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the actual text value of the item without mask characters.
	// Remarks:
	//     Mask characters are added by methods such as SetMask.
	// Returns:
	//     The actual text value of the item without mask characters.
	// See Also:
	//     SetMaskedText, SetMask
	//-----------------------------------------------------------------------
	CString GetMaskedText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the value of the item was changed by the user.
	// Remarks:
	//     Call CXTPPropertyGrid::HighlightChangedItems to highlight
	//     all items with changed values.
	// Returns:
	//     TRUE if the value of the item was changed by the user.
	// See Also:
	//     CXTPPropertyGrid::HighlightChangedItems, ResetValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual BOOL IsValueChanged() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the value of the item to its default value.
	// See Also:
	//     SetDefaultValue, IsValueChanged
	//-----------------------------------------------------------------------
	virtual void ResetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the default value for the item.
	// Parameters:
	//     lpszDefaultValue - Value to be set.
	// See Also:
	//     ResetValue, IsValueChanged
	//-----------------------------------------------------------------------
	void SetDefaultValue(LPCTSTR lpszDefaultValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the actual text value for the item without mask characters.
	// Parameters:
	//     lpszMaskedText - Text string without mask.
	// Remarks:
	//     Mask characters are added by methods such as SetMask.
	//     If a mask is used, then the mask will be applied
	//     to the text string passed through lpszMaskedText.
	// See Also:
	//     GetMaskedText, SetMask
	//-----------------------------------------------------------------------
	void SetMaskedText(LPCTSTR lpszMaskedText);

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
	//     Sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - 32-bit value to associate with the item.
	//-----------------------------------------------------------------------
	void SetItemData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the application-supplied, 32-bit value associated with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the in-place edit control of the item.
	// Returns:
	//     A reference to the in-place edit control of the item.
	//-----------------------------------------------------------------------
	virtual CXTPPropertyGridInplaceEdit& GetInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets focus to the in-place control of the item.
	//-----------------------------------------------------------------------
	virtual void SetFocusToInplaceControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified in-place button is visible.
	// Parameters:
	//     pButton - Pointer to a CXTPPropertyGridInplaceButton object.
	// Returns:
	//     TRUE if the specified in-place button is visible, otherwise FALSE.
	// See Also:
	//     CXTPPropertyGrid::SetShowInplaceButtonsAlways
	//-----------------------------------------------------------------------
	virtual BOOL IsInplaceButtonVisible(CXTPPropertyGridInplaceButton* pButton) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of in-place buttons of the item.
	// Returns:
	//     A pointer to the collection of in-place buttons of the item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButtons* GetInplaceButtons() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of in-place controls of the item.
	// Returns:
	//     A pointer to the collection of in-place controls of the item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControls* GetInplaceControls() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a standard combo button.
	//-----------------------------------------------------------------------
	void AddComboButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a standard expand button.
	//-----------------------------------------------------------------------
	void AddExpandButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a slider control.
	// Returns:
	//     A pointer to the newly added CXTPPropertyGridInplaceSlider object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceSlider* AddSliderControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a spin button control.
	// Returns:
	//     A pointer to the newly added CXTPPropertyGridInplaceSpinButton object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceSpinButton* AddSpinButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the metrics of the value part.
	// Returns:
	//     A pointer to a CXTPPropertyGridItemMetrics object containing
	//     the metrics (color, style) of the value part.
	// See Also:
	//     CXTPPropertyGridItemMetrics, GetCaptionMetrics
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemMetrics* GetValueMetrics() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the metrics of the caption part.
	// Returns:
	//     A pointer to a CXTPPropertyGridItemMetrics object containing
	//     the metrics (color, style) of the caption part.
	// See Also:
	//     CXTPPropertyGridItemMetrics, GetValueMetrics
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemMetrics* GetCaptionMetrics() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the metrics of the value/caption part.
	// Parameters:
	//     bValueMetrics - TRUE to get the metrics of the value part,
	//                     FALSE to get the metrics of the caption part.
	//     bCreate       - TRUE to call the GetValueMetrics/GetCaptionMetrics method
	//                     to get the metrics of the value/caption part.
	// Returns:
	//     A pointer to a CXTPPropertyGridItemMetrics object containing
	//     the metrics (color, style) of the value/caption part.
	// See Also:
	//     CXTPPropertyGridItemMetrics, GetValueMetrics
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemMetrics* GetMetrics(BOOL bValueMetrics, BOOL bCreate = TRUE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the variable height of the item.
	// Returns:
	//     The height of the item, or a value of -1 if default values are used.
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the variable height of the item.
	// Parameters:
	//     nHeight - Height to be set, or a value of -1 to use default values.
	// Remarks:
	//     Call CXTPPropertyGrid::SetVariableItemsHeight to use variable height items.
	// See Also:
	//     CXTPPropertyGrid::SetVariableItemsHeight, SetMultiLinesCount
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is multi-line (i.e. more than one line).
	// Returns:
	//     TRUE if the item is multi-line, otherwise FALSE.
	// See Also:
	//     SetMultiLinesCount
	//-----------------------------------------------------------------------
	BOOL IsMultiLine() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of lines of the item.
	// Returns:
	//     The number of lines of the item.
	//-----------------------------------------------------------------------
	int GetMultiLinesCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of lines for the item.
	// Parameters:
	//     nLinesCount - Number of lines to be set.
	// Remarks:
	//     Call CXTPPropertyGrid::SetVariableItemsHeight to use variable height items.
	//-----------------------------------------------------------------------
	void SetMultiLinesCount(int nLinesCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that this item is visible.
	//-----------------------------------------------------------------------
	void EnsureVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the user begins editing the item.
	// Returns:
	//     TRUE to allow the user to edit the item, FALSE to disallow.
	//-----------------------------------------------------------------------
	virtual BOOL OnRequestEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the user applies a new value to the item.
	// Parameters:
	//     strValue - Value to be set.
	// Returns:
	//     TRUE to allow the new item value, FALSE to disallow.
	//-----------------------------------------------------------------------
	virtual BOOL OnAfterEdit(CString& strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called if the user cancels editing the item.
	//-----------------------------------------------------------------------
	virtual void OnCancelEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the edit style for the item.
	// Parameters:
	//     dwEditStyle - Edit style to be set.
	// Example:
	//     <code>pItem->SetEditStyle(ES_MULTILINE | ES_AUTOVSCROLL);</code>
	//-----------------------------------------------------------------------
	void SetEditStyle(DWORD dwEditStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of items in the dropdown list.
	// Parameters:
	//     nDropDownItemCount - Number of items to be set.
	//-----------------------------------------------------------------------
	void SetDropDownItemCount(int nDropDownItemCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of items in the dropdown list.
	// Returns:
	//     The number of items in the dropdown list.
	// See Also:
	//     SetDropDownItemCount
	//-----------------------------------------------------------------------
	int GetDropDownItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to check the value from the in-place edit control.
	//-----------------------------------------------------------------------
	virtual void OnValidateEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPMarkupUIElement object that renders
	//     the caption of the item.
	// Returns:
	//     A pointer to the CXTPMarkupUIElement object that renders
	//     the caption of the item.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPMarkupUIElement object that renders
	//     the description of the item in the Help pane.
	// Returns:
	//     A pointer to the CXTPMarkupUIElement object that renders
	//     the description of the item in the Help pane.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupDescription() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPMarkupUIElement object that renders
	//     the value of the item in the property grid.
	// Returns:
	//     A pointer to the CXTPMarkupUIElement object that renders
	//     the value of the item in the property grid.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit style of the item.
	//     Override this method if you need a custom style.
	// Remarks:
	//     By default, this method returns WS_CHILD | ES_AUTOHSCROLL.
	// Returns:
	//     The edit style of the item.
	//-----------------------------------------------------------------------
	virtual DWORD GetEditStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Redraws the item.
	//-------------------------------------------------------------------------
	void RedrawItem();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the visible state of the item.
	// Parameters:
	//     bVisible - TRUE to set the item to visible.
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the index of the item is changed.
	//-----------------------------------------------------------------------
	virtual void OnIndexChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is selected.
	//-----------------------------------------------------------------------
	virtual void OnSelect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is de-selected.
	//-----------------------------------------------------------------------
	virtual void OnDeselect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a key is pressed.
	// Parameters:
	//     nChar -  Specifies the virtual-key code of the given key.
	// Returns:
	//     TRUE if the key was processed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnChar(UINT nChar);

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
	//     This method is called when the user presses the
	//     right mouse button on the item.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnRButtonDown(UINT nFlags, CPoint point);

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
	//     This method is called when the user releases the
	//     left mouse button on the item.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual-key code of the given key.
	// Returns:
	//     TRUE if the key was processed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is added to the parent.
	//-----------------------------------------------------------------------
	virtual void OnAddChildItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override this method to show an item-specific dialog.
	// Parameters:
	//     pButton - Pointer to the button that was pressed.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the in-place list control of the item.
	// Returns:
	//     A reference to the in-place list control of the item.
	//-----------------------------------------------------------------------
	virtual CXTPPropertyGridInplaceList& GetInplaceList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text for the in-place edit control.
	// Parameters:
	//     str - Reference to the text to be set.
	//-----------------------------------------------------------------------
	virtual void SetEditText(const CString& str);

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

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when constraints are added to the item
	//     (i.e. when CXTPPropertyGridItemConstraints::AddConstraint is called).
	// See Also:
	//     CXTPPropertyGridItemConstraints::AddConstraint, GetConstraints
	//-----------------------------------------------------------------------
	virtual void OnConstraintsChanged();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the caption of the item is changed.
	//-------------------------------------------------------------------------
	virtual void OnCaptionChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the property grid to set focus to
	//     an in-place child window.
	// Parameters:
	//     bForward - TRUE if the user moves focus forward (with Tab),
	//                FALSE if the user moves focus backword (with Shift+Tab).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnInpaceControlFocus(BOOL bForward);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text of the in-place edit control.
	//     It is sometimes useful to override the default behavior of this method.
	//-----------------------------------------------------------------------
	virtual CString GetInPlacedEditValue()
	{
		return m_strValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the item's value is changed.
	// See Also:
	//     CXTPPropertyGridItem::OnValueChanged
	//-----------------------------------------------------------------------
	void NotifyValueChanged();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Selects the next constraint in the list of constraints.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SelectNextConstraint();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified point is within the value rectangle.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     TRUE if the specified point is within the value rectangle, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL PtInValueRect(CPoint point);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility Support
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleDescription(VARIANT varChild, BSTR* pszDescription);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual CCmdTarget* GetAccessible();
	virtual HRESULT AccessibleSelect(long flagsSelect, VARIANT varChild);
	virtual HRESULT GetAccessibleValue(VARIANT varChild, BSTR* pszValue);
	DECLARE_INTERFACE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	static int AFX_CDECL NextNumber(LPCTSTR& str);

private:
	void Init();
	void Refresh(BOOL bInvalidate = TRUE);
	void Clear();
	CRect GetInplaceWindowsRect();
	void SetIndent(int nIndent);
	void RefreshHeight();

protected:
	BOOL m_bConstraintEdit; // TRUE to constraint edit.

	int m_nIndex;					  // Index of the item.
	int m_nIndent;					  // Indent of the item.
	UINT m_nID;						  // Identifier of the item.
	UINT m_nFlags;					  // Item's flags.
	BOOL m_bReadOnly;				  // TRUE if the item state is set to read-only,
									  // FALSE if the item state is set to read/write.
	BOOL m_bVisible;				  // Visibility of the item.
	BOOL m_bCategory;				  // TRUE if the item is a category.
	BOOL m_bExpandable;				  // TRUE if the item can be expanded.
	BOOL m_bExpanded;				  // TRUE if the item is expanded.
	CString m_strValue;				  // Value of the item.
	CString m_strDefaultValue;		  // Default value of the item.
	CString m_strCaption;			  // Caption of the item.
	CString m_strDescription;		  // Description of the item.
	CString m_strTooltip;			  // Tooltip text.
	CXTPPropertyGridItem* m_pParent;  // Parent item.
	CXTPPropertyGridView* m_pGrid;	// Parent grid class.
	CXTPPropertyGridItems* m_pChilds; // Child items.
	CXTPPropertyGridItemConstraints* m_pConstraints; // Item's constraints.
	BOOL m_bAutoComplete;							 // TRUE to enable auto-complete for the item.

	mutable CXTPPropertyGridItemMetrics* m_pValueMetrics;   // Value part metrics.
	mutable CXTPPropertyGridItemMetrics* m_pCaptionMetrics; // Caption part metrics.

	CXTPPropertyGridInplaceButtons* m_pInplaceButtons;   // In-place buttons array.
	CXTPPropertyGridInplaceControls* m_pInplaceControls; // In-place controls array.

	int m_nDropDownItemCount; // Maximum drop down items.

	CString* m_pBindString; // Bound string.
	CString m_strFormat;	// Format of the double value.

	CString m_strMask;	// String to use as edit mask.
	CString m_strLiteral; // String to use as literal. This is the same as the edit mask, but all
						  // mask characters are replaced with m_chPrompt.
	CString m_strMarkupTemplate; // String that represents item's display when markup is on.
								 // Use XTP_PG_ITEMVALUE_TO_REPLACE_IN_MARKUP for item text value
								 // substitution.
	TCHAR m_chPrompt; // Character used as a space holder for a character. This is used in
					  // m_strLiteral.
	BOOL m_bUseMask;  // TRUE to use an edit mask to display item data.

	BOOL m_bPassword; // TRUE to use a password mask. If TRUE, then each character will be replaced
					  // with an asterisk (*).
	DWORD_PTR m_dwData; // The 32-bit value associated with the item.
	BOOL m_bHidden;		// TRUE if the item is hidden, FALSE if the item is shown.

	int m_nHeight;							   // Item's height; -1 to use the default.
	int m_nLinesCount;						   // Number of lines in a multi-line item.
	DWORD m_dwEditStyle;					   // Edit style.
	CXTPMarkupUIElement* m_pMarkupCaption;	 // Markup UIElement for item's caption.
	CXTPMarkupUIElement* m_pMarkupDescription; // Markup UIElement for item's description.
	CXTPMarkupUIElement* m_pMarkupValue;	   // Markup UIElement for item's value.

	friend class CXTPPropertyGridItems;
	friend class CXTPPropertyGridView;
	friend class CXTPPropertyGridInplaceEdit;
	friend class CXTPPropertyGridInplaceButton;

	DECLARE_DYNAMIC(CXTPPropertyGridItem)

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

protected:
	DISPID m_dispidBinded;
	COleDispatchDriver m_dispDriverBinded;
	COleVariant m_oleTag;

	DECLARE_DISPATCH_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItem)

	afx_msg LPDISPATCH OleInsertChildItem(int nIndex, long nType, LPCTSTR strTitle,
										  const VARIANT* varValue);
	afx_msg LPDISPATCH OleAddChildItem(long nType, LPCTSTR strTitle, const VARIANT* varValue);
	afx_msg LPDISPATCH OleAddChildItemBinded(long nType, LPDISPATCH lpHandler,
											 LPCTSTR lpzsProperty);
	afx_msg LPDISPATCH OleInsertChildItemBinded(long nIndex, long nType, LPDISPATCH lpHandler,
												LPCTSTR lpzsProperty);
	afx_msg BOOL OleGetExpanded();
	afx_msg void OleSetExpanded(BOOL bExpanded);
	afx_msg BOOL OleGetSelected();
	afx_msg BSTR OleGetDescription();
	afx_msg void OleSetSelected(BOOL bSelected);
	afx_msg LPDISPATCH OleGetParent();
	afx_msg LPDISPATCH OleGetChilds();
	afx_msg LPDISPATCH OleGetConstraints();
	afx_msg long OleGetType();
	afx_msg BSTR OleGetMaskedText();

	afx_msg virtual const VARIANT OleGetValue();
	afx_msg virtual void OleSetValue(const VARIANT* varValue);
	afx_msg void OleSetMask(LPCTSTR strMask, LPCTSTR strLiteral, const VARIANT* pPrompt);
	afx_msg LPDISPATCH OleAddInplaceButton(UINT Id);
	afx_msg LPDISPATCH OleAddSliderControl();
	afx_msg LPDISPATCH OleAddSpinButton();
	afx_msg LPDISPATCH OleGetButtons();
	afx_msg int OleGetHeight();
	afx_msg int OleGetMultiLinesCount();
	afx_msg void OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom);
	afx_msg void OleStartEdit();
	virtual void BindDispatch();
	afx_msg LPDISPATCH OleGetValueMetrics();
	afx_msg LPDISPATCH OleGetCaptionMetrics();
	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR lpszCaption);

	afx_msg BSTR OleGetMarkupTemplate();
	afx_msg void OleSetMarkupTemplate(LPCTSTR lpszMarkupTemplate);

	long m_nType;
	enum
	{
		dispidValue				 = 0L,
		dispidExpanded			 = 1L,
		dispidReadOnly			 = 2L,
		dispidSelected			 = 3L,
		dispidCaption			 = 4L,
		dispidDescription		 = 5L,
		dispidTag				 = 6L,
		dispidFlags				 = 7L,
		dispidConstraintEdit	 = 8L,
		dispidFormat			 = 9L,
		dispidConstraints		 = 97L,
		dispidChilds			 = 98L,
		dispidParent			 = 99L,
		dispidAddChildItem		 = 100L,
		dispidType				 = 101L,
		dispidAddChildItemBinded = 102,
		dispidMarkupTemplate	 = 121
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPPropertyGridItemConstraints;
	friend class CXTPPropertyGrid;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemCategory is a CXTPPropertyGridItem derived class.
//     Use it to add a sub-category to a property grid control.
// Note:
//     The parent item for the category must also be a category.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemCategory : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemCategory object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	// Remarks:
	//     Class CXTPPropertyGridItemCategory has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same ID in such form "Caption\\nDescription".
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemCategory(LPCTSTR strCaption);
	CXTPPropertyGridItemCategory(
		UINT nID); // <COMBINE CXTPPropertyGridItemCategory::CXTPPropertyGridItemCategory@LPCTSTR>
};

//////////////////////////////////////////////////////////////////////
AFX_INLINE long CXTPPropertyGridItems::GetCount() const
{
	return (long)m_arrItems.GetSize();
}
AFX_INLINE BOOL CXTPPropertyGridItems::IsEmpty() const
{
	return GetCount() == 0;
}
AFX_INLINE void CXTPPropertyGridItems::AddTail(CXTPPropertyGridItem* pItem)
{
	m_arrItems.Add(pItem);
}
AFX_INLINE void CXTPPropertyGridItems::InsertAt(int nIndex, CXTPPropertyGridItem* pItem)
{
	m_arrItems.InsertAt(nIndex, pItem);
}
AFX_INLINE BOOL CXTPPropertyGridItem::HasChilds() const
{
	return !m_pChilds->IsEmpty();
}
AFX_INLINE void CXTPPropertyGridItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);
	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;
		OnCaptionChanged();
	}
}
AFX_INLINE CString CXTPPropertyGridItem::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE CString CXTPPropertyGridItem::GetDescription() const
{
	return m_strDescription;
}
AFX_INLINE CString CXTPPropertyGridItem::GetMarkupTemplate() const
{
	return m_strMarkupTemplate;
}
AFX_INLINE BOOL CXTPPropertyGridItem::GetReadOnly() const
{
	return m_bReadOnly;
}
AFX_INLINE void CXTPPropertyGridItem::SetID(UINT nID)
{
	m_nID = nID;
}
AFX_INLINE UINT CXTPPropertyGridItem::GetID() const
{
	return m_nID;
}
AFX_INLINE CString CXTPPropertyGridItem::GetValue() const
{
	return m_strValue;
}
AFX_INLINE CString CXTPPropertyGridItem::GetDefaultValue() const
{
	return m_strDefaultValue;
}
AFX_INLINE BOOL CXTPPropertyGridItem::IsCategory() const
{
	return m_bCategory;
}
AFX_INLINE CXTPPropertyGridView* CXTPPropertyGridItem::GetGrid() const
{
	return m_pGrid;
}
AFX_INLINE BOOL CXTPPropertyGridItem::OnDrawItemValue(CDC&, CRect)
{
	return FALSE;
}
AFX_INLINE BOOL CXTPPropertyGridItem::IsVisible() const
{
	return m_bVisible;
}
AFX_INLINE BOOL CXTPPropertyGridItem::IsExpanded() const
{
	return m_bExpanded;
}
AFX_INLINE void CXTPPropertyGridItem::SetExpandable(BOOL bExpandable)
{
	m_bExpandable = bExpandable;
}
AFX_INLINE BOOL CXTPPropertyGridItem::IsExpandable() const
{
	return m_bExpandable;
}
AFX_INLINE CXTPPropertyGridItem* CXTPPropertyGridItem::GetParentItem() const
{
	return m_pParent;
}
AFX_INLINE CXTPPropertyGridItems* CXTPPropertyGridItem::GetChilds() const
{
	return m_pChilds;
}
AFX_INLINE int CXTPPropertyGridItem::GetIndent() const
{
	return m_nIndent;
}
AFX_INLINE void CXTPPropertyGridItem::SetConstraintEdit(BOOL bConstraintEdit)
{
	m_bConstraintEdit = bConstraintEdit;
}
AFX_INLINE BOOL CXTPPropertyGridItem::GetConstraintEdit() const
{
	return m_bConstraintEdit;
}
AFX_INLINE void CXTPPropertyGridItem::SetPasswordMask(TCHAR chMask)
{
	m_bPassword = TRUE;
	m_chPrompt  = chMask;
}
AFX_INLINE void CXTPPropertyGridItem::OnRButtonDown(UINT, CPoint)
{
}
AFX_INLINE void CXTPPropertyGridItem::OnConstraintsChanged()
{
}
AFX_INLINE void CXTPPropertyGridItem::SetItemData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPPropertyGridItem::GetItemData() const
{
	return m_dwData;
}
AFX_INLINE void CXTPPropertyGridItem::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}
AFX_INLINE void CXTPPropertyGridItem::OnIndexChanged()
{
}
AFX_INLINE CXTPPropertyGridItemConstraints* CXTPPropertyGridItem::GetConstraints() const
{
	return m_pConstraints;
}
AFX_INLINE int CXTPPropertyGridItemConstraints::GetCount() const
{
	return (int)m_arrConstraints.GetSize();
}
AFX_INLINE BOOL CXTPPropertyGridItemConstraints::IsEmpty() const
{
	return GetCount() == 0;
}
AFX_INLINE void CXTPPropertyGridItemConstraints::SetCurrent(int nIndex)
{
	m_nCurrent = nIndex;
}
AFX_INLINE int CXTPPropertyGridItemConstraints::GetCurrent() const
{
	return m_nCurrent;
}
AFX_INLINE int CXTPPropertyGridItemConstraint::GetIndex() const
{
	return m_nIndex;
}

AFX_INLINE int CXTPPropertyGridItem::GetHeight() const
{
	return m_nHeight;
}
AFX_INLINE BOOL CXTPPropertyGridItem::IsMultiLine() const
{
	return m_nLinesCount > 1;
}
AFX_INLINE int CXTPPropertyGridItem::GetMultiLinesCount() const
{
	return m_nLinesCount;
}
AFX_INLINE void CXTPPropertyGridItem::SetEditStyle(DWORD dwEditStyle)
{
	m_dwEditStyle = dwEditStyle;
}
AFX_INLINE BOOL CXTPPropertyGridItem::OnKeyDown(UINT /*nChar*/)
{
	return FALSE;
}
AFX_INLINE void CXTPPropertyGridItem::SetDropDownItemCount(int nDropDownItemCount)
{
	m_nDropDownItemCount = nDropDownItemCount;
}
AFX_INLINE int CXTPPropertyGridItem::GetDropDownItemCount() const
{
	return m_nDropDownItemCount;
}
AFX_INLINE void CXTPPropertyGridItem::SetAutoComplete(BOOL bAutoComplete)
{
	m_bAutoComplete = bAutoComplete;
}
AFX_INLINE void CXTPPropertyGridItem::SetTooltip(LPCTSTR lpstrTooltip)
{
	m_strTooltip = lpstrTooltip;
}
AFX_INLINE CString CXTPPropertyGridItem::GetTooltip() const
{
	return m_strTooltip;
}
AFX_INLINE CXTPMarkupUIElement* CXTPPropertyGridItem::GetMarkupCaption() const
{
	return m_pMarkupCaption;
}
AFX_INLINE CXTPMarkupUIElement* CXTPPropertyGridItem::GetMarkupDescription() const
{
	return m_pMarkupDescription;
}
AFX_INLINE CXTPMarkupUIElement* CXTPPropertyGridItem::GetMarkupValue() const
{
	return m_pMarkupValue;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDITEM_H__)
