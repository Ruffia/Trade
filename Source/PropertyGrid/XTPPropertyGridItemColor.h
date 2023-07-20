// XTPPropertyGridItemColor.h interface for the CXTPPropertyGridItemColor class.
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
#if !defined(__XTPPROPERTYGRIDITEMCOLOR_H__)
#	define __XTPPROPERTYGRIDITEMCOLOR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//-----------------------------------------------------------------------
// Summary:
//     Editor type for CXTPPropertyGridItemColor item.
// See Also:
//     CXTPPropertyGridItemColor, CXTPPropertyGridItemColor::SetEditorStyle
// Example:
//     <code>pItem->SetEditorStyle(xtpPropertyGridItemColorExtendedDialog);</code>
//-----------------------------------------------------------------------
enum XTPPropertyGridItemColorEditor
{
	xtpPropertyGridItemColorPopup,			// Color pop-up editor.
	xtpPropertyGridItemColorStandardDialog, // Standard color dialog editor.
	xtpPropertyGridItemColorExtendedDialog  // Extended color Editor editor.
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemColor is a CXTPPropertyGridItem derived class.
//     It is used to create a COLORREF value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemColor : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemColor object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     clr        - Initial COLORREF value of the item.
	//     pBindColor - If not NULL, then the value of the item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemColor has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same ID in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToColor member.
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
	//     BindToColor
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemColor(LPCTSTR strCaption, COLORREF clr = 0, COLORREF* pBindColor = NULL);
	CXTPPropertyGridItemColor(
		UINT nID, COLORREF clr = 0,
		COLORREF* pBindColor =
			NULL); // <COMBINE
				   // CXTPPropertyGridItemColor::CXTPPropertyGridItemColor@LPCTSTR@COLORREF@COLORREF*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemColor object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemColor();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the COLORREF value for the item.
	// Parameters:
	//     clr - COLORREF value to be set.
	//-----------------------------------------------------------------------
	virtual void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the COLORREF value of the item.
	// Returns:
	//     The COLORREF value of the item.
	//-----------------------------------------------------------------------
	virtual COLORREF GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a specified string to a COLORREF.
	// Parameters:
	//     str - String to be converted (in the format RRR;GGG;BBB).
	// Returns:
	//     The converted COLORREF.
	//-----------------------------------------------------------------------
	static COLORREF AFX_CDECL StringToRGB(LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a specified COLORREF to a string.
	// Parameters:
	//     clr - COLORREF to be coverted.
	// Returns:
	//     The converted string (in the format RRR;GGG;BBB).
	//-----------------------------------------------------------------------
	static CString AFX_CDECL RGBToString(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified COLORREF value.
	// Parameters:
	//     pBindColor - Pointer to a COLORREF value.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToColor member.
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
	virtual void BindToColor(COLORREF* pBindColor);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the editor type for the item.
	// Parameters:
	//     editor - Editor type to be set; can be one of the following:
	//     * xtpPropertyGridItemColorPopup          - Color pop-up editor.
	//     * xtpPropertyGridItemColorStandardDialog - Standard color dialog editor.
	//     * xtpPropertyGridItemColorExtendedDialog - Extended color Editor editor.
	// See Also:
	//     GetEditorStyle, XTPPropertyGridItemColorEditor
	//-----------------------------------------------------------------------
	void SetEditorStyle(XTPPropertyGridItemColorEditor editor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the editor type of the item.
	// Returns:
	//     The editor type of the item; one of the values defined
	//     by the XTPPropertyGridItemColorEditor enumeration.
	// See Also:
	//     SetEditorStyle, XTPPropertyGridItemColorEditor
	//-----------------------------------------------------------------------
	XTPPropertyGridItemColorEditor GetEditorStyle();

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
	//     This method is called when the user presses the in-place button.
	//     Override this method to show an item-specific dialog.
	// Parameters:
	//     pButton - Pointer to the button that was pressed.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display the color picker dialog.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);

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

protected:
	COLORREF m_clrValue;	// Color value of the item.
	COLORREF* m_pBindColor; // Pointer to the variable bound to this item.

	XTPPropertyGridItemColorEditor m_colorEditor; // Editor type of the item.

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemColor)

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemColor)

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPPropertyGridItemColor::GetColor() const
{
	return m_clrValue;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPPROPERTYGRIDITEMCOLOR_H__)
