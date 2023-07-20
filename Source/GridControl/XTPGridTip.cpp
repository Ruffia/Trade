// XTPGridTip.cpp : implementation of the CXTPGridTip class.
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
#include "Common/XTPToolTipContext.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridTip.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPGridTip

CXTPGridTip::CXTPGridTip()
{
	m_pParentWnd   = NULL;
	m_pGridControl = NULL;

	m_pItem		= NULL;
	m_nRowIndex = -1;
}

CXTPGridTip::~CXTPGridTip()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridTip, CWnd)
	//{{AFX_MSG_MAP(CXTPGridTip)
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST_EX()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPGridTip message handlers

LRESULT CXTPGridTip::OnNcHitTest(CPoint /*point*/)
{
	return (UINT)HTTRANSPARENT;
}

BOOL CXTPGridTip::Create(CWnd* pParentWnd)
{
	ASSERT(pParentWnd);

	// Already created ?
	if (m_hWnd)
		return TRUE;

	m_pParentWnd = pParentWnd;
	return CWnd::CreateEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
						  AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL,
						  WS_POPUP, CXTPEmptyRect(), m_pParentWnd, NULL);
}

void CXTPGridTip::Activate(BOOL bActive, BOOL bMultiline)
{
	m_bMultiline = bMultiline;

	if (bActive)
	{
		CXTPWindowRect rc(this);
		RecalcTooltipRect(rc);
		Invalidate(FALSE);
		SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(),
					 SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
	}
	else
	{
		SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
					 SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE
						 | SWP_NOSIZE);
	}
}

BOOL CXTPGridTip::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPGridTip::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	dc.SetBkMode(TRANSPARENT);

	BOOL bToolipDrawn = FALSE;
	CWnd* pToolTip	= m_pGridControl->GetToolTipContext()->CreateToolTip(m_pParentWnd);
	if (NULL != pToolTip)
	{
		pToolTip->SetWindowPos(&CWnd::wndTop, 0, 0, rc.Width(), rc.Height(),
							   SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER
								   | SWP_NOREDRAW | SWP_NOMOVE | SWP_NOCOPYBITS);
		bToolipDrawn =
			(TRUE
			 == pToolTip->SendMessage(WM_PRINTCLIENT, reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
									  PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN | PRF_OWNED));
		pToolTip->DestroyWindow();
		delete pToolTip;
	}

	if (!bToolipDrawn)
	{
		m_pGridControl->GetToolTipContext()->DrawBackground(&dc, rc);
	}

	if ((m_pParentWnd->GetExStyle() & WS_EX_RTLREADING)
		|| (m_pParentWnd->GetExStyle() & WS_EX_LAYOUTRTL))
	{
		dc.SetTextAlign(TA_RTLREADING);
	}

	CXTPFontDC font(&dc, &m_xtpFontToolTip);
	CRect rcText(rc);
	if (m_bMultiline)
	{
		rcText.left += XTP_DPI_X(2);
		rcText.right--;
	}
	else
	{
		rcText.left += XTP_DPI_X(3);
		rcText.top -= XTP_DPI_Y(2);
	}

	dc.DrawText(m_strTooltipText, rcText,
				UINT(m_bMultiline ? DT_WORDBREAK : DT_SINGLELINE) | DT_VCENTER | DT_NOPREFIX);
}

BOOL CXTPGridTip::IsMultilineForce() const
{
	if (m_strTooltipText.IsEmpty())
		return FALSE;

	return (m_strTooltipText.Find(_T('\n'), 0) != -1);
}

void CXTPGridTip::RecalcTooltipRect(CRect& rc)
{
	if (!m_bMultiline)
	{
		CString strText = m_strTooltipText;
		CWindowDC dc(this);
		CXTPFontDC font(&dc, &m_xtpFontToolTip);
		rc.right = rc.left + dc.GetTextExtent(strText).cx + XTP_DPI_X(6);
	}

	if ((m_pParentWnd->GetExStyle() & WS_EX_RTLREADING)
		|| (m_pParentWnd->GetExStyle() & WS_EX_LAYOUTRTL))
	{
		rc.OffsetRect(-rc.right + m_rcHover.right + XTP_DPI_X(1), 0);
	}

	CRect rcWork = XTPMultiMonitor()->GetWorkArea();
	if (rc.right > rcWork.right)
		rc.OffsetRect(rcWork.right - rc.right, 0);
	if (rc.left < rcWork.left)
		rc.OffsetRect(rcWork.left - rc.left, 0);
	if (m_bMultiline)
	{
		if (rc.bottom > rcWork.bottom)
			rc.OffsetRect(0, rcWork.bottom - rc.bottom);
	}
}

void CXTPGridTip::SetFont(CFont* pFont)
{
	ASSERT_VALID(pFont);

	m_xtpFontToolTip.DeleteObject();

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_xtpFontToolTip.CreateFontIndirect(&lf);
}
