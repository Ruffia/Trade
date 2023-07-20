// XTPGridHyperlink.h: interface for the CXTPGridHyperlink class.
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
#if !defined(__XTPGRIDRECORDHYPERLINK_H__)
#	define __XTPGRIDRECORDHYPERLINK_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropExchange;

//===========================================================================
// Summary:
//      This class stores the anchor styles.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridHyperlinkStyle : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the object and initializes the color and text decoration.
	// Parameters:
	//     color          - Color.
	//     textDecoration - Text decoration.
	//-----------------------------------------------------------------------
	CXTPGridHyperlinkStyle(COLORREF color, XTPGridTextDecoration textDecoration);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the color of the anchor text.
	// Returns:
	//     The color of the anchor text.
	//-----------------------------------------------------------------------
	COLORREF GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the color for the anchor text.
	// Parameters:
	//     color - New color to be set.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF color);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text decoration of the anchor text.
	// Returns:
	//     The text decoration of the anchor text.
	//-----------------------------------------------------------------------
	XTPGridTextDecoration GetTextDecoration() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text decoration for the anchor text.
	// Parameters:
	//     textDecoration - New text decoration to be set.
	//-----------------------------------------------------------------------
	void SetTextDecoration(XTPGridTextDecoration textDecoration);

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPGridHyperlinkStyle);

#		define DECLARE_PROPERTY_UN(theProperty, theType)                                          \
			afx_msg theType OleGet##theProperty();                                                 \
			afx_msg void OleSet##theProperty(theType);

	DECLARE_PROPERTY_UN(Color, COLORREF);
	DECLARE_PROPERTY_UN(TextDecoration, int);

//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	COLORREF m_color;						// Color.
	XTPGridTextDecoration m_textDecoration; // Text decoration.
};

//===========================================================================
// Summary:
//     This class represents hyperlinks. It stores the start position
//     and length of a hyperlink item's text as well as the rectangle used
//     for drawing the hyperlink and identifying clicks on the hyperlink.
//     To create an instance of this class, you simply call its constructor
//     with the start position and length of a hyperlink item's text.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridHyperlink
	: public CXTPHeapObjectT<CXTPCmdTarget, CXTPGridDataAllocator>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPGridHyperlink)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default CXTPGridHyperlink constructor.
	// Parameters:
	//     nHyperTextBegin - Start position of the hyperlink item text
	//                       in the parent item caption.
	//     nHyperTextLen   - Length of the hyperlink item text.
	//-----------------------------------------------------------------------
	CXTPGridHyperlink(int nHyperTextBegin = 0, int nHyperTextLen = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridHyperlink object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridHyperlink();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid hyperlink
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	CRect m_rcHyperSpot;   // Hyperlink draw place.
	int m_nHyperTextBegin; // Start position of the hyperlink item text in the parent item caption.
	int m_nHyperTextLen;   // The length of the hyperlink item text.

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridHyperlink);
	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//      This typedef is used to declare a base class for CXTPGridHyperlinks.
//===========================================================================
typedef CXTPArrayT<CXTPGridHyperlink*, CXTPGridHyperlink*, LPDISPATCH> CXTPGridHyperlinks_base;

//===========================================================================
// Summary:
//      This class represents an array of pointers to CXTPGridHyperlink objects.
//      InternalRelease is called for each item in the array destructor.
//      Serialization for array items is provided (see the DoPropExchange method).
//===========================================================================
class _XTP_EXT_CLASS CXTPGridHyperlinks
	: public CXTPHeapObjectT<CXTPGridHyperlinks_base, CXTPGridDataAllocator>

{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPGridHyperlinks)
	typedef CXTPArrayT<CXTPGridHyperlink*, CXTPGridHyperlink*, LPDISPATCH> TBase;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default CXTPGridHyperlink constructor.
	//-----------------------------------------------------------------------
	CXTPGridHyperlinks();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridHyperlinks object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridHyperlinks();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid hyperlinks collection
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the hyperlink at a specified index from the collection.
	// Parameters:
	//     nIndex - Zero-based index of the hyperlink to remove.
	//      nCount - The number of elements to remove.
	// See Also:
	//     CXTPGridHyperlink, CXTPArrayT
	//-----------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);
	virtual void RemoveAt(int nIndex, int nCount); // <combine CXTPGridHyperlinks@RemoveAt@int>

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all hyperlinks from the collection.
	// See Also:
	//     RemoveAt, CXTPGridHyperlink, CXTPArrayT
	//-------------------------------------------------------------------------
	virtual void RemoveAll();

	//-------------------------------------------------------------------------
	// Summary:
	//     Copies all members from a specified CXTPGridHyperlinks object
	//     to this CXTPGridHyperlinks object.
	// Parameters:
	//     pSrc - Pointer to a CXTPGridHyperlinks object to copy from.
	//-------------------------------------------------------------------------
	virtual void CopyFrom(CXTPGridHyperlinks* pSrc);

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridHyperlinks);
	DECLARE_ENUM_VARIANT(CXTPGridHyperlinks)

	virtual LPDISPATCH OleGetItem(long nIndex);
	afx_msg LPDISPATCH OleAddHyperlink(long nHyperTextBegin, long nHyperTextLen);
	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

/////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDRECORDHYPERLINK_H__)
