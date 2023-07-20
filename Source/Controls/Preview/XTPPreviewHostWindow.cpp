// XTPPreviewHostWindow.cpp : implementation of the CXTPPreviewHostWindow class.
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
#include "Common/XTPDrawHelpers.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/XTPPreviewHostWindow.h"
#include "Controls/Preview/XTPPreviewCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

IMPLEMENT_DYNAMIC(CXTPPreviewHostWindow, CWnd)

CXTPPreviewHostWindow::CXTPPreviewHostWindow(CXTPPreviewCtrl* pPreviewCtrl)
	: m_pPreviewCtrl(pPreviewCtrl)
	, m_pPreview(NULL)
	, m_strMessage(_T(""))
{
	ASSERT_VALID(pPreviewCtrl);
}

CXTPPreviewHostWindow::~CXTPPreviewHostWindow()
{
	DeletePreview();
}

void CXTPPreviewHostWindow::ShowMessage(LPCTSTR message)
{
	m_strMessage = message;
	Invalidate();
	UpdateWindow();
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPreviewHostWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPPreviewHostWindow::SetPreview(IXTPPreview* preview)
{
	DeletePreview();

	m_strMessage = _T("");
	m_pPreview   = preview;
	Invalidate();

	UpdateWindow();
}

void CXTPPreviewHostWindow::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (NULL == m_pPreview)
	{
		if (!m_strMessage.IsEmpty())
		{
			CXTPDCSaveState dcState(&dc);
			dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
			dc.SetBkMode(TRANSPARENT);

			if (COLORREF_NULL != GetBackgroundColor())
				dc.SetBkColor(GetBackgroundColor());
			if (COLORREF_NULL != GetForegroundColor())
				dc.SetTextColor(GetForegroundColor());

			dc.DrawText(m_strMessage, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		return;
	}

	m_pPreview->Draw(dc, CSize(rcClient.Width(), rcClient.Height()), rcClient);
}

BOOL CXTPPreviewHostWindow::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (NULL != m_pPreview)
	{
		MSG msg;
		msg.hwnd	= m_hWnd;
		msg.lParam  = lParam;
		msg.wParam  = wParam;
		msg.message = message;
		if (m_pPreview->OnHostWndMsg(&msg, pResult))
			return TRUE;
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPPreviewHostWindow::OnDestroy()
{
	CWnd::OnDestroy();
	DeletePreview();
}

BOOL CXTPPreviewHostWindow::OnEraseBkgnd(CDC* pDC)
{
	ASSERT_VALID(pDC);

	if (COLORREF_NULL == GetBackgroundColor())
		return CWnd::OnEraseBkgnd(pDC);

	CXTPClientRect rc(this);
	pDC->FillSolidRect(&rc, GetBackgroundColor());

	return TRUE;
}

void CXTPPreviewHostWindow::DeletePreview()
{
	if (NULL != m_pPreview)
	{
		delete m_pPreview;
		m_pPreview = NULL;
	}
}

COLORREF CXTPPreviewHostWindow::GetBackgroundColor() const
{
	return m_pPreviewCtrl->GetBackgroundColor();
}

COLORREF CXTPPreviewHostWindow::GetForegroundColor() const
{
	return m_pPreviewCtrl->GetForegroundColor();
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
