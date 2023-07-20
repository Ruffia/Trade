// XTPGridView.h: interface for the CXTPGridPageSetupDialog class.
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
#if !defined(__XTPGRIDPAGESETUPDIALOG_H__)
#	define __XTPGRIDPAGESETUPDIALOG_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//=============================================================================
// Summary:
//     This class is used to store printing options.
// See Also:
//     CXTPPrintOptions, CXTPPrintPageHeaderFooter
//=============================================================================
class _XTP_EXT_CLASS CXTPGridViewPrintOptions : public CXTPPrintOptions
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPGridViewPrintOptions)
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridViewPrintOptions object.
	//-------------------------------------------------------------------------
	CXTPGridViewPrintOptions();

	//-----------------------------------------------------------------------
	// Summary:
	//      Override this member function in a derived class to retrieve
	//      the active locale ID.
	// Returns:
	//      Locale ID. Base implementation returns LOCALE_USER_DEFAULT;
	//-----------------------------------------------------------------------
	virtual LCID GetActiveLCID();

	//-----------------------------------------------------------------------
	// Summary:
	//      Copies members from a specified source object.
	// Parameters:
	//      pSrc - Pointer to a source object to copy data from.
	//-----------------------------------------------------------------------
	virtual void Set(const CXTPGridViewPrintOptions* pSrc);

	BOOL m_bRepeatHeaderRows; // If TRUE, then the header rows will be printed on every page. FALSE
							  // by default.
	BOOL m_bRepeatFooterRows; // If TRUE, then the footer rows will be printed on every page. FALSE
							  // by default.

protected:
//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridViewPrintOptions);
#	endif
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     Customized Grid control page setup dialog.
// Remarks:
//     Use this class when you want to allow users to customize printing
//     options for the Grid control.
//
//     This class enhances the standard MFC page setup dialog, which encapsulates
//     the services provided by the Windows common OLE Page Setup dialog box
//     with additional support for setting and modifying print margins.
//     This class is designed to take the place of the Print Setup dialog box.
//
// See Also:
//     CXTPGridViewPrintOptions overview,
//     CPageSetupDialog overview
//===========================================================================
class _XTP_EXT_CLASS CXTPGridPageSetupDialog : public CPageSetupDialog
{
public:
	//{{AFX_CODEJOCK_PRIVATE
	enum
	{
		IDD = XTP_IDD_GRID_PRINT_PAGE_SETUP
	};
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Class constructor.
	// Parameters:
	//     pOptions   - Pointer to an associated CXTPGridViewPrintOptions
	//                  object for storing user's interaction results.
	//     dwFlags    - One or more flags you can use to customize the settings of the
	//                  dialog box. The values can be combined using the bitwise-OR
	//                  operator. For more details, see CPageSetupDialog::CPageSetupDialog.
	//     pParentWnd - Pointer to the dialog box's parent or owner.
	// Remarks:
	//     Use the DoModal function to display the dialog box.
	// See also:
	//      CPageSetupDialog::CPageSetupDialog()
	//-----------------------------------------------------------------------
	CXTPGridPageSetupDialog(CXTPGridViewPrintOptions* pOptions,
							DWORD dwFlags	= PSD_MARGINS | PSD_INWININIINTLMEASURE,
							CWnd* pParentWnd = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridPageSetupDialog object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridPageSetupDialog();

protected:
	CXTPGridViewPrintOptions* m_pOptions; // Pointer to associated print options object.

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a font name to a human readable string.
	// Parameters:
	//     lfFont - Reference to a font.
	// Returns:
	//     A human readable string describing the provided font.
	//-----------------------------------------------------------------------
	// CString FormatFontName(const LOGFONT& lfFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Dialog-box initialization.
	// Returns:
	//     FALSE because it must explicitly set the input focus to one of the
	//     controls in the dialog box.
	// See also:
	//     CPageSetupDialog::OnInitDialog().
	//-----------------------------------------------------------------------
	virtual BOOL OnInitDialog();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called when the user clicks the OK button (i.e. the button with an ID of IDOK).
	// Remarks:
	//     Saves all values into the m_pOptions structure.
	// See also:
	//     CPageSetupDialog::OnOK().
	//-----------------------------------------------------------------------
	virtual void OnOK();

	//{{AFX_CODEJOCK_PRIVATE
	CEdit m_ctrlHeaderFormat;
	CEdit m_ctrlFooterFormat;

	CButton m_ctrlHeaderFormatBtn;
	CButton m_ctrlFooterFormatBtn;

	afx_msg void OnBnClickedHeaderFormat();
	afx_msg void OnBnClickedFooterFormat();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDPAGESETUPDIALOG_H__)
