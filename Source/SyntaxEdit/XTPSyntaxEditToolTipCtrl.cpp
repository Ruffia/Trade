// XTPSyntaxEditToolTipCtrl.cpp
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

#include "stdafx.h"

// common includes
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"

#include "SyntaxEdit/XTPSyntaxEditToolTipCtrl.h"

// syntax editor includes
#include "SyntaxEdit/XTPSyntaxEditDefines.h"
#include "SyntaxEdit/XTPSyntaxEditStruct.h"
#include "SyntaxEdit/XTPSyntaxEditCtrl.h"
#include "SyntaxEdit/XTPSyntaxEditPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT XTP_TIP_TIMER_ID = 1001;

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditToolTipCtrl

CXTPSyntaxEditToolTipCtrl::CXTPSyntaxEditToolTipCtrl()
	: m_nDelayTime(5000)
	, m_pParentWnd(NULL)
{
	RegisterWindowClass();
}

CXTPSyntaxEditToolTipCtrl::~CXTPSyntaxEditToolTipCtrl()
{
	DestroyWindow();
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPSyntaxEditToolTipCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTPSyntaxEditToolTipCtrl)
	ON_WM_NCHITTEST_EX()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPSyntaxEditToolTipCtrl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	WNDCLASS wndcls;
	if (hInstance == NULL)
		hInstance = XTPGetInstanceHandle();

	if (!(::GetClassInfo(hInstance, XTP_EDIT_CLASSNAME_TOOLTIP, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style	   = CS_VREDRAW | CS_HREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance					  = hInstance;
		wndcls.hIcon						  = NULL;
		wndcls.hCursor						  = ::LoadCursor(0, IDC_ARROW);
		wndcls.hbrBackground				  = (HBRUSH)(COLOR_INFOBK + 1);
		wndcls.lpszMenuName					  = NULL;
		wndcls.lpszClassName				  = XTP_EDIT_CLASSNAME_TOOLTIP;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditToolTipCtrl message handlers

LRESULT CXTPSyntaxEditToolTipCtrl::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	return (LRESULT)HTTRANSPARENT;
}

BOOL CXTPSyntaxEditToolTipCtrl::Create(CXTPSyntaxEditCtrl* pParentWnd)
{
	ASSERT(pParentWnd);
	m_pParentWnd = pParentWnd;

	// Already created?
	if (::IsWindow(m_hWnd))
		return TRUE;

	// first, create a tool tip window
	if (!CWnd::CreateEx(WS_EX_TOOLWINDOW, XTP_EDIT_CLASSNAME_TOOLTIP, NULL,
						WS_POPUP | WS_BORDER | WS_CLIPSIBLINGS, CXTPEmptyRect(), NULL, 0))
	{
		TRACE0("Failed to create tooltip window.\n");
		return FALSE;
	}

	return TRUE;
}

void CXTPSyntaxEditToolTipCtrl::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	dc.SetBkColor(::GetSysColor(COLOR_INFOBK));
	dc.SetTextColor(::GetSysColor(COLOR_INFOTEXT));

	CXTPFontDC font(&dc, m_pParentWnd ? m_pParentWnd->GetPaintManager()->GetFontToolTip() : NULL);
	CRect rcText(rc);
	rcText.left += XTP_DPI_X(3);
	rcText.top += XTP_DPI_Y(3);

	dc.DrawText(m_strToolTipText, rcText, DT_WORDBREAK | DT_NOPREFIX | DT_EXPANDTABS);
}

void CXTPSyntaxEditToolTipCtrl::ReCalcToolTipRect()
{
	CWindowDC dc(NULL);

	CXTPFontDC font(&dc, m_pParentWnd ? m_pParentWnd->GetPaintManager()->GetFontToolTip() : NULL);

	CRect rcDesktop = XTPMultiMonitor()->GetWorkArea();
	CRect rc(0, 0, (rcDesktop.right - m_rcHover.left) - XTP_DPI_X(10), XTP_DPI_Y(20));
	dc.DrawText(m_strToolTipText, rc, DT_CALCRECT | DT_WORDBREAK | DT_EXPANDTABS);

	m_rcHover.right  = m_rcHover.left + rc.Width() + XTP_DPI_X(8);
	m_rcHover.bottom = m_rcHover.top + rc.Height() + XTP_DPI_Y(8);

	MoveWindow(&m_rcHover);
}

void CXTPSyntaxEditToolTipCtrl::Hide()
{
	if (!m_hWnd)
		return;

	// stop existing timer.
	KillTimer(XTP_TIP_TIMER_ID);

	if (::IsWindowVisible(m_hWnd))
	{
		ShowWindow(SW_HIDE);
	}
}

void CXTPSyntaxEditToolTipCtrl::Activate(const CString& strText)
{
	BOOL bVisible = ::IsWindowVisible(m_hWnd);

	// stop existing timer.
	KillTimer(XTP_TIP_TIMER_ID);

	// hide the window if the tip text has changed.
	if (bVisible && (m_strToolTipText != strText))
	{
		ShowWindow(SW_HIDE);
	}

	// set the tip text.
	m_strToolTipText = strText;

	if (!bVisible)
	{
		ReCalcToolTipRect();
		ShowWindow(SW_SHOWNA);
		Invalidate();
	}

	// set timer.
	SetTimer(XTP_TIP_TIMER_ID, XTPToUIntChecked(m_nDelayTime), NULL);
}

void CXTPSyntaxEditToolTipCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == XTP_TIP_TIMER_ID)
	{
		Hide();
		return;
	}

	CWnd::OnTimer(nIDEvent);
}
