// XTPMsgPreviewDialog.cpp : implementation of the CXTPMsgPreviewDialog class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Preview/Providers/MSG/XTPMsgPreviewDialog.h"

#include "Controls/Resource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#define XTP_DLG_ITEM_RIGHT_SHFT XTP_DPI_X(7)
#define XTP_DLG_ITEM_MIN_SIZE XTP_DPI_Y(10)

// CXTPMsgPreviewDialog dialog

IMPLEMENT_DYNAMIC(CXTPMsgPreviewDialog, CDialog)

CXTPMsgPreviewDialog::CXTPMsgPreviewDialog()
	: CDialog(XTP_IDD_MSG_PREVIEW_DLG, NULL)
{
}

CXTPMsgPreviewDialog::~CXTPMsgPreviewDialog()
{
}

BOOL CXTPMsgPreviewDialog::Create(CWnd* pParent)
{
	return CDialog::Create(XTP_IDD_MSG_PREVIEW_DLG, pParent);
}

void CXTPMsgPreviewDialog::SetSubject(LPCTSTR lpSubject)
{
	SetDlgItemText(XTP_IDC_MSG_SUBJECT_EDIT, lpSubject);
}

void CXTPMsgPreviewDialog::SetTo(LPCTSTR lpTo)
{
	SetDlgItemText(XTP_IDC_MSG_TO_EDIT, lpTo);
}

void CXTPMsgPreviewDialog::SetFrom(LPCTSTR lpFrom)
{
	SetDlgItemText(XTP_IDC_MSG_FROM_EDIT, lpFrom);
}

void CXTPMsgPreviewDialog::SetAttachment(LPCTSTR lpAttachment)
{
	SetDlgItemText(XTP_IDC_MSG_ATTACHMENT_EDIT, lpAttachment);
}

void CXTPMsgPreviewDialog::SetDate(LPCTSTR lpDate)
{
	SetDlgItemText(XTP_IDC_MSG_DATE_EDIT, lpDate);
}

void CXTPMsgPreviewDialog::SetCC(LPCTSTR lpCC)
{
	SetDlgItemText(XTP_IDC_MSG_CC_EDIT, lpCC);
}

void CXTPMsgPreviewDialog::SetBCC(LPCTSTR lpBCC)
{
	SetDlgItemText(XTP_IDC_MSG_BCC_EDIT, lpBCC);
}

void CXTPMsgPreviewDialog::SetBody(LPCTSTR lpBody)
{
	SetDlgItemText(XTP_IDC_MSG_BODY_RICHEDIT, lpBody);
}

void CXTPMsgPreviewDialog::OnOK()
{
	// Do nothing.
}

void CXTPMsgPreviewDialog::OnCancel()
{
	// Do nothing.
}

static CRect GetDlgItemRect(RECT pos, int w)
{
	CRect rc;

	int minY = pos.bottom - pos.top;
	rc.top   = pos.top;
	rc.left  = pos.left;

	int cx = w - pos.left - XTP_DLG_ITEM_RIGHT_SHFT;
	int cy = pos.bottom - pos.top;

	if (cx < XTP_DLG_ITEM_MIN_SIZE)
		cx = XTP_DLG_ITEM_MIN_SIZE;
	if (cy < XTP_DLG_ITEM_MIN_SIZE)
		cy = minY;

	rc.right  = rc.left + cx;
	rc.bottom = rc.top + cy;

	return rc;
}

void CXTPMsgPreviewDialog::Layout()
{
	const DWORD flags = SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER;

	CXTPClientRect rcClient(this);
	int width  = rcClient.Width();
	int height = rcClient.Height();

	HDWP dwp = BeginDeferWindowPos(8);

	CWnd* pFrom = GetDlgItem(XTP_IDC_MSG_FROM_EDIT);
	WINDOWPLACEMENT pl;
	pl.length = sizeof(pl);
	pFrom->GetWindowPlacement(&pl);
	CRect rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp		  = DeferWindowPos(dwp, pFrom->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						   rec.Height(), flags);

	CWnd* pTo = GetDlgItem(XTP_IDC_MSG_TO_EDIT);
	pTo->GetWindowPlacement(&pl);
	rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp = DeferWindowPos(dwp, pTo->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						 rec.Height(), flags);

	CWnd* pDate = GetDlgItem(XTP_IDC_MSG_DATE_EDIT);
	pDate->GetWindowPlacement(&pl);
	rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp = DeferWindowPos(dwp, pDate->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						 rec.Height(), flags);

	CWnd* pSubject = GetDlgItem(XTP_IDC_MSG_SUBJECT_EDIT);
	pSubject->GetWindowPlacement(&pl);
	rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp = DeferWindowPos(dwp, pSubject->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						 rec.Height(), flags);

	CWnd* pCC = GetDlgItem(XTP_IDC_MSG_CC_EDIT);
	pCC->GetWindowPlacement(&pl);
	rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp = DeferWindowPos(dwp, pCC->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						 rec.Height(), flags);

	CWnd* pBCC = GetDlgItem(XTP_IDC_MSG_BCC_EDIT);
	pBCC->GetWindowPlacement(&pl);
	rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp = DeferWindowPos(dwp, pBCC->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						 rec.Height(), flags);

	CWnd* pAttach = GetDlgItem(XTP_IDC_MSG_ATTACHMENT_EDIT);
	pAttach->GetWindowPlacement(&pl);
	rec = GetDlgItemRect(pl.rcNormalPosition, width);
	dwp = DeferWindowPos(dwp, pAttach->GetSafeHwnd(), NULL, rec.left, rec.top, rec.Width(),
						 rec.Height(), flags);

	CWnd* pBody = GetDlgItem(XTP_IDC_MSG_BODY_RICHEDIT);
	pBody->GetWindowPlacement(&pl);

	int t  = pl.rcNormalPosition.top;
	int x  = pl.rcNormalPosition.left;
	int cx = width - pl.rcNormalPosition.left - XTP_DLG_ITEM_RIGHT_SHFT;
	int cy = height - pl.rcNormalPosition.top - XTP_DLG_ITEM_RIGHT_SHFT;
	if (cx < XTP_DLG_ITEM_MIN_SIZE)
		cx = XTP_DLG_ITEM_MIN_SIZE;
	if (cy < XTP_DLG_ITEM_MIN_SIZE)
		cy = XTP_DLG_ITEM_MIN_SIZE;

	dwp = DeferWindowPos(dwp, pBody->GetSafeHwnd(), NULL, x, t, cx, cy, flags);

	EndDeferWindowPos(dwp);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPMsgPreviewDialog, CDialog)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPMsgPreviewDialog::OnWindowPosChanging(WINDOWPOS* lpWndPos)
{
	CDialog::OnWindowPosChanging(lpWndPos);
	Layout();
}

void CXTPMsgPreviewDialog::OnWindowPosChanged(WINDOWPOS* lpWndPos)
{
	CDialog::OnWindowPosChanged(lpWndPos);
	Layout();
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
