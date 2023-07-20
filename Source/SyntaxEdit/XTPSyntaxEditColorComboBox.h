// XTPSyntaxEditColorComboBox.h : header file
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITCOLORCOMBOBOX_H__)
#	define __XTPSYNTAXEDITCOLORCOMBOBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//      This class allows for colors to be edited by using a combobox control.
//      It displays the color name and draws a rectangle for each item using the item color.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CXTPSyntaxEditColorComboBox)

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Standard object constructor.
	// ----------------------------------------------------------------------
	CXTPSyntaxEditColorComboBox();

	// ----------------------------------------------------------------------
	// Summary:
	//     Standard object destructor.
	// ----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorComboBox();

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the selected color.
	// Returns:
	//     The selected color, or COLORREF_NULL if no color is selected.
	// ----------------------------------------------------------------------
	COLORREF GetSelColor();

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the selected color.
	// Parameters:
	//     crColor - Color to be set.
	// Returns:
	//     The index of the newly selected color.
	// ----------------------------------------------------------------------
	int SetSelColor(COLORREF crColor);

	// ----------------------------------------------------------------------
	// Summary:
	//     Removes a specified color.
	// Parameters:
	//     crColor - Color to be removed.
	// Returns:
	//     The index of the deleted color if successful, otherwise CB_ERR.
	// ----------------------------------------------------------------------
	int DeleteColor(COLORREF crColor);

	// ----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a specified color.
	// Parameters:
	//     crColor - Color to retrieve the index of.
	// Returns:
	//     The index of the specified color if successful, otherwise CB_ERR.
	// ----------------------------------------------------------------------
	int FindColor(COLORREF crColor);

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the user color value and user color name.
	// Parameters:
	//     crColor      - User color value to be set.
	//     lpszUserText - User color name to be set or NULL to use the default.
	// Returns:
	//     The index of the user color value if successful, otherwise CB_ERR.
	// ----------------------------------------------------------------------
	int SetUserColor(COLORREF crColor, LPCTSTR lpszUserText = NULL);

	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the user color value.
	// Returns:
	//     The user color value.
	// ----------------------------------------------------------------------
	COLORREF GetUserColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the auto color value and auto color name.
	// Parameters:
	//     crColor      - Auto color value to be set.
	//     lpszAutoText - Auto color name to be set or NULL to use the default.
	// Returns:
	//     The index of the auto color value if successful, otherwise CB_ERR.
	// ----------------------------------------------------------------------
	int SetAutoColor(COLORREF crColor, LPCTSTR lpszAutoText = NULL);

	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the auto color value.
	// Returns:
	//     The auto color value.
	// ----------------------------------------------------------------------
	COLORREF GetAutoColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Selects the user color.
	// Returns:
	//     The index of the selected user color if successful, otherwise CB_ERR.
	// ----------------------------------------------------------------------
	int SelectUserColor();

	// ----------------------------------------------------------------------
	// Summary:
	//     Adds the default set of colors.
	// Returns:
	//     true if successful, otherwise false.
	// ----------------------------------------------------------------------
	virtual bool Init();

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCIS);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	// ----------------------------------------------------------------------
	// Summary:
	//     Adds a new color.
	// Parameters:
	//     crColor - Color to be added.
	//     nID     - Resource ID of the color name string.
	// Returns:
	//     The index of the newly added color if successful, otherwise CB_ERR.
	// ----------------------------------------------------------------------
	virtual int AddColor(COLORREF crColor, UINT nID);

	// ----------------------------------------------------------------------
	// Summary:
	//     Sends a WM_COMMAND message to the owner window.
	// Remarks:
	//     Default implementation sends the following notifications:
	//     CBN_SELENDCANCEL, CBN_SELENDOK
	// Parameters:
	//     nCode - Notification code.
	// ----------------------------------------------------------------------
	virtual void NotifyOwner(UINT nCode);

	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the currently selected listbox item
	//     (works for dropped state only).
	// Returns:
	//     The index of the currently selected listbox item if successful, otherwise LB_ERR.
	// ----------------------------------------------------------------------
	virtual int GetLBCurSel() const;

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCloseUp();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

private:
	int m_iPrevSel;
	bool m_bPreInit;
	COLORREF m_crAuto;
	COLORREF m_crUser;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPSyntaxEditColorComboBox::GetUserColor() const
{
	return m_crUser;
}
AFX_INLINE COLORREF CXTPSyntaxEditColorComboBox::GetAutoColor() const
{
	return m_crAuto;
}

//===========================================================================
// Summary:
//     Exchanges the currently selected color for a CXTPSyntaxEditColorComboBox control.
// Parameters:
//     pDX   - Pointer to a CDataExchange object.
//     nIDC  - Control ID.
//     value - Reference to the color value.
//===========================================================================
_XTP_EXT_CLASS void AFXAPI DDX_CBSyntaxColor(CDataExchange* pDX, int nIDC, COLORREF& value);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITCOLORCOMBOBOX_H__)
