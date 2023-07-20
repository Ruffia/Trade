// XTPPreviewHostWindow.h : interface for the CXTPPreviewHostWindow class.
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
#if !defined(__XTPPREVIEWHOSTWINDOW_H__)
#	define __XTPPREVIEWHOSTWINDOW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

class CXTPPreviewCtrl;
struct IXTPPreview;

class _XTP_EXT_CLASS CXTPPreviewHostWindow
	: public CWnd
	, public IXTPPreviewHost
{
	DECLARE_DYNAMIC(CXTPPreviewHostWindow)
	friend class CXTPPreviewCtrl;

public:
	explicit CXTPPreviewHostWindow(CXTPPreviewCtrl* pPreviewCtrl);
	virtual ~CXTPPreviewHostWindow();

	void ShowMessage(LPCTSTR message);

private:
	// IXTPPreviewHost implementation
	virtual COLORREF GetBackgroundColor() const;
	virtual COLORREF GetForegroundColor() const;

private:
	void DeletePreview();
	void SetPreview(IXTPPreview* pPreview);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC*);

private:
	CXTPPreviewCtrl* m_pPreviewCtrl;
	IXTPPreview* m_pPreview;
	CString m_strMessage;
};

//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPREVIEWHOSTWINDOW_H__)
