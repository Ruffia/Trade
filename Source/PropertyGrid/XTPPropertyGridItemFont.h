// XTPPropertyGridItemFont.h interface for the CXTPPropertyGridItemFont class.
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
#if !defined(__XTPPROPERTYGRIDITEMFONT_H__)
#	define __XTPPROPERTYGRIDITEMFONT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPPropertyGridItemFont is a CXTPPropertyGridItem derived class.
//     It is used to create a Font value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemFont : public CXTPPropertyGridItem
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemFont object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     font       - Reference to the initial LOGFONT value of the item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemFont(LPCTSTR strCaption, LOGFONT& font);
	CXTPPropertyGridItemFont(
		UINT nID,
		LOGFONT& font); // <COMBINE
						// CXTPPropertyGridItemFont::CXTPPropertyGridItemFont@LPCTSTR@LOGFONT&>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemFont object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemFont();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the LOGFONT value for the item.
	// Parameters:
	//     font - Reference to the LOGFONT value to be set.
	//-----------------------------------------------------------------------
	virtual void SetFont(LOGFONT& font);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the LOGFONT value of the item.
	// Parameters:
	//     lf - Pointer to the LOGFONT value to
	//          receive the LOGFONT value of the item.
	//-----------------------------------------------------------------------
	virtual void GetFont(LOGFONT* lf);

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
	virtual COLORREF GetColor();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override this method to show an item-specific dialog.
	// Parameters:
	//     pButton - Pointer to the button that was pressed.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display the font picker dialog.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);

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
	//     Converts a specified LOGFONT to a string that contains
	//     the name and pt of the font.
	// Parameters:
	//     lfValue - LOGFONT to be converted.
	// Returns:
	//     The converted string that contains the name and pt of the font.
	//-----------------------------------------------------------------------
	virtual CString FontToString(const LOGFONT& lfValue);

private:
	static UINT_PTR CALLBACK FontDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	LOGFONT m_lfValue;   // LOGFONT value of the item.
	COLORREF m_clrValue; // Font color.

protected:
	DECLARE_DYNAMIC(CXTPPropertyGridItemFont)

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemFont)

	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();
	BSTR OleGetFaceName();
	void OleSetFaceName(LPCTSTR lpszFaceName);
	int OleGetSize();
	void OleSetSize(int nHeigth);
	int OleGetWeight();
	void OleSetWeight(int nWeight);
	BOOL OleGetBold();
	void OleSetBold(BOOL bBold);
	BOOL OleGetItalic();
	void OleSetItalic(BOOL bItalic);
	BOOL OleGetUnderline();
	void OleSetUnderline(BOOL bUnderline);
	BOOL OleGetStrikeOut();
	void OleSetStrikeOut(BOOL bStrikeOut);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDITEMFONT_H__)
