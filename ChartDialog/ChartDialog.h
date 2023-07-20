// ChartDialog.h
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

#if !defined(AFX_CHARTDIALOG_H__9FE3F754_EEE7_4622_9FD4_4C00F17A32C4__INCLUDED_)
#	define AFX_CHARTDIALOG_H__9FE3F754_EEE7_4622_9FD4_4C00F17A32C4__INCLUDED_

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	ifndef __AFXWIN_H__
#		error include 'stdafx.h' before including this file for PCH
#	endif

#	include "resource.h" // main symbols

/////////////////////////////////////////////////////////////////////////////
// CChartDialogApp:
// See ChartDialog.cpp for the implementation of this class
//

class CChartDialogApp : public CXTPWinApp
{
public:
	CChartDialogApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartDialogApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Implementation

	//{{AFX_MSG(CChartDialogApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTDIALOG_H__9FE3F754_EEE7_4622_9FD4_4C00F17A32C4__INCLUDED_)
