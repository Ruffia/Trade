// XTPResizeDialog.cpp: implementation of the CXTPResizeDialog class.
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

#include "stdafx.h"

#include "Common/XTPCasting.h"

#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizeDialog.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

#pragma warning(disable : 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeDialog

IMPLEMENT_DYNCREATE(CXTPResizeDialog, CDialog)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPResizeDialog, CDialog)
	//{{AFX_MSG_MAP(CXTPResizeDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeDialog construction/destruction

CXTPResizeDialog::CXTPResizeDialog()
	: CXTPResize(this, 0)
{
	m_nDialogID = 0;
}

CXTPResizeDialog::CXTPResizeDialog(const UINT nID, CWnd* pParent, const UINT nFlags)
	: CDialog(nID, pParent)
	, CXTPResize(this, nFlags)
{
	m_nDialogID = nID;
}

CXTPResizeDialog::CXTPResizeDialog(LPCTSTR lpszTemplateName, CWnd* pParent, const UINT nFlags)
	: CDialog(lpszTemplateName, pParent)
	, CXTPResize(this, nFlags)
{
	m_nDialogID = 0;
}

BOOL CXTPResizeDialog::OnInitDialog()
{
	// Only show the resize grip if the frame is a resizing frame
	DWORD dwStyle = XTPToDWORD(::GetWindowLong(m_hWnd, GWL_STYLE));
	if ((dwStyle & WS_THICKFRAME) == 0)
	{
		SetFlag(xtpResizeNoSizeIcon);
	}

	CDialog::OnInitDialog();
	CXTPResize::Init();

	return TRUE;
}

void CXTPResizeDialog::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void CXTPResizeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CXTPResize::Size();
}

void CXTPResizeDialog::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CDialog::OnDestroy();
}

#if (_MSC_VER > 1200)
/////////////////////////////////////////////////////////////////////////////
// CXTPResizeDHtmlDialog

IMPLEMENT_DYNCREATE(CXTPResizeDHtmlDialog, CDHtmlDialog)

#	include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPResizeDHtmlDialog, CDHtmlDialog)
	//{{AFX_MSG_MAP(CXTPResizeDHtmlDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#	include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeDHtmlDialog construction/destruction

CXTPResizeDHtmlDialog::CXTPResizeDHtmlDialog()
	: CXTPResize(this, 0)
{
	m_nDialogID = 0;
}

CXTPResizeDHtmlDialog::CXTPResizeDHtmlDialog(const UINT nID, UINT nHtmlResID, CWnd* pParent,
											 const UINT nFlags)
	: CDHtmlDialog(nID, nHtmlResID, pParent)
	, CXTPResize(this, nFlags)
{
	m_nDialogID = nID;
}

CXTPResizeDHtmlDialog::CXTPResizeDHtmlDialog(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID,
											 CWnd* pParent, const UINT nFlags)
	: CDHtmlDialog(lpszTemplateName, szHtmlResID, pParent)
	, CXTPResize(this, nFlags)
{
	m_nDialogID = 0;
}

BOOL CXTPResizeDHtmlDialog::OnInitDialog()
{
	// Only show the resize grip if the frame is a resizing frame
	DWORD dwStyle = XTPToDWORD(::GetWindowLong(m_hWnd, GWL_STYLE));
	if ((dwStyle & WS_THICKFRAME) == 0)
	{
		SetFlag(xtpResizeNoSizeIcon);
	}

	CDHtmlDialog::OnInitDialog();
	CXTPResize::Init();

	return TRUE;
}

void CXTPResizeDHtmlDialog::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void CXTPResizeDHtmlDialog::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);
	CXTPResize::Size();
}

void CXTPResizeDHtmlDialog::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CDHtmlDialog::OnDestroy();
}
#endif
