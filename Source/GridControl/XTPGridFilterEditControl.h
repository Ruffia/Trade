// XTPGridFilterEditControl.h: interface for the CXTPGridFilterEditControl class.
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
#if !defined(__XTPGRIDFILTEREDITCONTROL_H__)
#	define __XTPGRIDFILTEREDITCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The CXTPGridFilterEditControl class provides the functionality of the
//     filter string edit control associated to the main Grid control window.
// Remarks:
//     It has all the functionality of the CEdit control, as well as some
//     specific additions.
//     This control should be instantiated in the user's application. To use
//     a field chooser within an existing dialog box in your application,
//     add a standard list box to your dialog template using the dialog
//     editor and then initialize your CXTPGridSubListControl object inside
//     the dialog OnInitialUpdate handler. See the example below:
// Example:
// <code>
// // CXTPGridFilterEditControl wndFilter;
// wndFilter.SubclassDlgItem(IDC_FILTEREDIT, &pFilterDialog);
// // CXTPGridControl wndGrid;
// wndGrid.GetColumns()->GetGridHeader()->SetFilterEditCtrl(&wndFilter);
// </code>
// See Also:
//     CEdit, CXTPGridHeader::SetFilterEditCtrl
//===========================================================================
class _XTP_EXT_CLASS CXTPGridFilterEditControl : public CEdit
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridFilterEditControl object.
	// Remarks:
	//     You construct a CXTPGridFilterEditControl object in two steps.
	//     First, call the CXTPGridFilterEditControl constructor.
	//     Second, call Create. This initializes the Windows edit box and
	//     attaches it to the CXTPGridFilterEditControl.
	// Example:
	// <code>
	// // Declare a local CXTPGridFilterEditControl object.
	// CXTPGridFilterEditControl myFilterEdit;
	//
	// // Declare a dynamic CXTPGridFilterEditControl object.
	// CXTPGridFilterEditControl* pMyFilterEdit = new CXTPGridFilterEditControl;
	// </code>
	//
	// See Also:
	//    CEdit, SetGridCtrl, CXTPGridHeader::SetFilterEditCtrl
	//-----------------------------------------------------------------------
	CXTPGridFilterEditControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridFilterEditControl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPGridFilterEditControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated Grid control.
	// Parameters:
	//     pGridCtrl - Pointer to the associated Grid control to be set.
	// Remarks:
	//     Usually this method should not be called by the user. Instead
	//     the user should call CXTPGridHeader::SetFilterEditCtrl.
	// Returns:
	//     TRUE if the associated Grid control was set successfully,
	//     otherwise FALSE.
	// See Also:
	//     CXTPGridHeader::SetFilterEditCtrl, GetGridCtrl
	//-----------------------------------------------------------------------
	BOOL SetGridCtrl(CXTPGridControl* pGridCtrl);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridCtrl)
	BOOL SetReportCtrl(CXTPGridControl* pGridCtrl);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated Grid control.
	// Returns:
	//     A pointer to the associated Grid control.
	// See Also:
	//     CXTPGridHeader::SetFilterEditCtrl, SetGridCtrl
	//-----------------------------------------------------------------------
	CXTPGridControl* GetGridCtrl();

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridCtrl)
	CXTPGridControl* GetReportCtrl();
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the edit control text.
	// Parameters:
	//     lpszText - New edit control text to be set.
	//-----------------------------------------------------------------------
	void SetText(const CString& lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit control text.
	// Returns:
	//     The edit control text.
	//-----------------------------------------------------------------------
	CString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the edit hint for the control (i.e. the grayed-out text
	//     displayed in the edit control that provides a helpful
	//     description of what the control is used for).
	// Parameters:
	//     lpszHint - Edit hint to be set.
	// Example:
	//     <code>pEdit->SetHint(_T("Click to find a contact");</code>
	// See Also:
	//     GetHint
	//-----------------------------------------------------------------------
	void SetHint(LPCTSTR lpszHint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit hint of the control (i.e. the grayed-out text
	//     displayed in the edit control that provides a helpful
	//     description of what the control is used for).
	// Returns:
	//     The edit hint of the control.
	// See Also:
	//     SetHint
	//-----------------------------------------------------------------------
	CString GetHint() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPGridFilterEditControl)
	afx_msg BOOL OnChange();
	afx_msg void OnPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPGridControl* m_pGridCtrl; // Pointer to the main Grid control.
	CString m_strHint;			  // Grayed-out text displayed in the edit control to show a helpful
								  // description like "Enter filter text here".
};

AFX_INLINE BOOL CXTPGridFilterEditControl::SetReportCtrl(CXTPGridControl* pGridCtrl)
{
	return SetGridCtrl(pGridCtrl);
}

AFX_INLINE CXTPGridControl* CXTPGridFilterEditControl::GetReportCtrl()
{
	return GetGridCtrl();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDFILTEREDITCONTROL_H__)
