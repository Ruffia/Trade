// XTPSyntaxEditColorSampleText.h : header file
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
#if !defined(__XTPSYNTAXEDITPROPERTIESSAMPLETEXT_H__)
#	define __XTPSYNTAXEDITPROPERTIESSAMPLETEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     This class, derived from CStatic, is used to display a sample of colored text.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorSampleText : public CStatic
{
public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// ----------------------------------------------------------------------
	CXTPSyntaxEditColorSampleText();

	// ----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	// ----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorSampleText();

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the text color.
	// Parameters:
	//     crText - Text color to be set.
	// ----------------------------------------------------------------------
	void SetTextColor(COLORREF crText);

	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the text color.
	// Returns:
	//     The text color.
	// ----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the background color.
	// Parameters:
	//     crBack - Background color to be set.
	// ----------------------------------------------------------------------
	void SetBackColor(COLORREF crBack);

	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the background color.
	// Returns:
	//     The background color.
	// ----------------------------------------------------------------------
	COLORREF GetBackColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the border color.
	// Parameters:
	//     crBorder - Border color to be set.
	// ----------------------------------------------------------------------
	void SetBorderColor(COLORREF crBorder);

	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the border color.
	// Returns:
	//     The border color.
	// ----------------------------------------------------------------------
	COLORREF GetBorderColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Redraws the control.
	// ----------------------------------------------------------------------
	void Refresh();

	//{{AFX_VIRTUAL(CXTPSyntaxEditColorSampleText)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPSyntaxEditColorSampleText)
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_CODEJOCK_PRIVATE
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CXTPPaintManagerColor m_crBack;
	CXTPPaintManagerColor m_crText;
	CXTPPaintManagerColor m_crBorder;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITPROPERTIESSAMPLETEXT_H__)
