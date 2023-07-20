// XTPMsgPreviewDialog.h : interface for the CXTPMsgPreviewDialog class.
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

#if !defined(__XTPMSGPREVIEWDIALOG_H__)
#	define __XTPMSGPREVIEWDIALOG_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPMsgPreviewDialog : public CDialog
{
	DECLARE_DYNAMIC(CXTPMsgPreviewDialog)

public:
	CXTPMsgPreviewDialog(); // standard constructor
	virtual ~CXTPMsgPreviewDialog();

	BOOL Create(CWnd* pParent);
	void SetSubject(LPCTSTR lpSubject);
	void SetTo(LPCTSTR lpTo);
	void SetFrom(LPCTSTR lpFrom);
	void SetAttachment(LPCTSTR lpAttachment);
	void SetDate(LPCTSTR lpDate);
	void SetCC(LPCTSTR lpCC);
	void SetBCC(LPCTSTR lpBCC);
	void SetBody(LPCTSTR lpBody);

#	if _MSC_VER > 1200
	using CDialog::Create;
	using CWnd::Create;
#	endif

protected:
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpWndPos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpWndPos);

private:
	void Layout();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPMSGPREVIEWDIALOG_H__)

//}}AFX_CODEJOCK_PRIVATE
