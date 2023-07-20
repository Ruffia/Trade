// XTPPreviewNavigatorWindow.h : interface for the CXTPPreviewNavigatorWindow class.
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
#if !defined(__XTPPREVIEWNAVIGATORWINDOW_H__)
#	define __XTPPREVIEWNAVIGATORWINDOW_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

class CXTPPreviewCtrl;

class _XTP_EXT_CLASS CXTPPreviewNavigatorWindow : public CWnd
{
	DECLARE_DYNAMIC(CXTPPreviewNavigatorWindow)

public:
	explicit CXTPPreviewNavigatorWindow(CXTPPreviewCtrl* pPreviewCtrl);
	virtual ~CXTPPreviewNavigatorWindow();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFirstBtn();
	afx_msg void OnLastBtn();
	afx_msg void OnPrevBtn();
	afx_msg void OnNextBtn();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC*);

private:
	void PlaceButtons();

private:
	CButton m_FirstBtn;
	CButton m_LastBtn;
	CButton m_PrevBtn;
	CButton m_NextBtn;

	CBitmap m_FirstBitmap;
	CBitmap m_PreviousBitmap;
	CBitmap m_NextBitmap;
	CBitmap m_LastBitmap;

	CXTPPreviewCtrl* m_pPreviewCtrl;
};

//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPREVIEWNAVIGATORWINDOW_H__)
