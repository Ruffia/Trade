// XTPPropertyGridItemNumber.h interface for the CXTPPropertyGridItemNumber class.
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
#if !defined(__XTPPROPERTYGRIDITEMNUMBER_H__)
#	define __XTPPROPERTYGRIDITEMNUMBER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPPropertyGridItemNumber is a CXTPPropertyGridItem derived class.
//     It is used to create an integer value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemNumber : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemNumber object
	// Parameters:
	//     strCaption  - Caption of the item.
	//     nID         - Identifier of the item.
	//     nValue      - Initial long value of the item.
	//     pBindNumber - If not NULL, then the value of the item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemNumber has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same ID in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToNumber member.
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
	//     BindToNumber
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemNumber(LPCTSTR strCaption, long nValue = 0, long* pBindNumber = NULL);
	CXTPPropertyGridItemNumber(
		UINT nID, long nValue = 0,
		long* pBindNumber =
			NULL); // <COMBINE
				   // CXTPPropertyGridItemNumber::CXTPPropertyGridItemNumber@LPCTSTR@long@long*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemNumber object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemNumber();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the long value for the item.
	// Parameters:
	//     nValue - Long value to be set.
	//-----------------------------------------------------------------------
	virtual void SetNumber(long nValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the long value of the item.
	// Returns:
	//     The long value of the item.
	//-----------------------------------------------------------------------
	virtual long GetNumber() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified long value.
	// Parameters:
	//     pBindNumber - Pointer to a long value.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToNumber member.
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
	virtual void BindToNumber(long* pBindNumber);

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

protected:
	long m_nValue;		 // Value of the item.
	long* m_pBindNumber; // Pointer to the variable bound to this item.

private:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#	endif
	DECLARE_DYNAMIC(CXTPPropertyGridItemNumber)
};

//===========================================================================
// Summary:
//     CXTPPropertyGridItemDouble is a CXTPPropertyGridItem derived class.
//     It is used to create a double value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemDouble : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemDouble object.
	// Parameters:
	//     strCaption  - Caption of the item.
	//     nID         - Identifier of the item.
	//     fValue      - Initial double value of the item.
	//     strFormat   - The number of digits after the decimal point.
	//                   For example, "%0.5f" would display
	//                   five digits after the decimal point.
	//     pBindNumber - If not NULL, then the value of this item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemDouble has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same id in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToDouble member.
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
	//     BindToDouble
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemDouble(LPCTSTR strCaption, double fValue = 0, LPCTSTR strFormat = NULL,
							   double* pBindNumber = NULL);
	CXTPPropertyGridItemDouble(
		UINT nID, double fValue = 0, LPCTSTR strFormat = NULL,
		double* pBindNumber =
			NULL); // <COMBINE
				   // CXTPPropertyGridItemDouble::CXTPPropertyGridItemDouble@LPCTSTR@double@LPCTSTR@double*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemDouble object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemDouble();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the double value for the item.
	// Parameters:
	//     fValue - Double value to be set.
	//-----------------------------------------------------------------------
	virtual void SetDouble(double fValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the double value of the item.
	// Returns:
	//     The double value of the item.
	//-----------------------------------------------------------------------
	virtual double GetDouble() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the item to a specified double value.
	// Parameters:
	//     pBindDouble - Pointer to a double value.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToDouble member.
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
	virtual void BindToDouble(double* pBindDouble);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies which type of symbol should be used to divide
	//     the whole and decimal parts of numbers.
	// Parameters:
	//     bUseSystemDecimalSymbol - TRUE to use the system defined decimal symbol,
	//                               FALSE to always use a decimal point ".".
	// Remarks:
	//     Setting bUseSystemDecimalSymbol to FALSE causes the decimal point "."
	//     to always be used to separate the whole and decimal parts of numbers.
	//
	//     Setting bUseSystemDecimalSymbol to TRUE causes the system defined symbol
	//     to be used to separate the whole and decimal parts of numbers.
	//     For example, if bUseSystemDecimalSymbol is set to TRUE, then a comma ","
	//     will be used in Europe to separate the whole and decimal parts of numbers.
	//-----------------------------------------------------------------------
	void UseSystemDecimalSymbol(BOOL bUseSystemDecimalSymbol);

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

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a specified string to a double.
	// Parameters:
	//     lpszValue - String to be converted.
	// Returns:
	//     The converted double.
	// See Also:
	//     DoubleToString
	//-----------------------------------------------------------------------
	double StringToDouble(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a specified double to a string.
	// Parameters:
	//     dValue - Double to be converted.
	// Returns:
	//     The converted string.
	// See Also:
	//     StringToDouble
	//-----------------------------------------------------------------------
	CString DoubleToString(double dValue);

protected:
	double m_fValue;				// Double value of the item.
	double* m_pBindDouble;			// Pointer to the variable bound to this item.
	BOOL m_bUseSystemDecimalSymbol; // TRUE to use the system defined decimal symbol,
									// FALSE to always use a decimal point ".".

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemDouble)

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

//////////////////////////////////////////////////////////////////////

AFX_INLINE long CXTPPropertyGridItemNumber::GetNumber() const
{
	return m_nValue;
}

//////////////////////////////////////////////////////////////////////

AFX_INLINE double CXTPPropertyGridItemDouble::GetDouble() const
{
	return m_fValue;
}
AFX_INLINE void CXTPPropertyGridItemDouble::UseSystemDecimalSymbol(BOOL bUseSystemDecimalSymbol)
{
	m_bUseSystemDecimalSymbol = bUseSystemDecimalSymbol;
	m_strValue				  = DoubleToString(m_fValue);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDITEMNUMBER_H__)
