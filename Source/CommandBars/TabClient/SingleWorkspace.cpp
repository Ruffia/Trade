// SingleWorkspace.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "StdAfx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPColorManager.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/TabClient/Workspace.h"
#include "CommandBars/TabClient/SingleWorkspace.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CSingleWorkspace

CXTPTabClientWnd::CSingleWorkspace::~CSingleWorkspace()
{
	DestroyWindow();
}

CWnd* CXTPTabClientWnd::CSingleWorkspace::GetWindow() const
{
	return (CWnd*)this;
}

INT_PTR CXTPTabClientWnd::CSingleWorkspace::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	return PerformToolHitTest(m_hWnd, point, pTI);
}

BOOL CXTPTabClientWnd::CSingleWorkspace::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
												  LRESULT* pResult)
{
	if (m_pTabClientWnd)
	{
		m_pTabClientWnd->m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CXTPTabClientWnd::CSingleWorkspace::PreTranslateMessage(MSG* pMsg)
{
#ifndef _XTP_COMMANDBARS_ACTIVEX
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK
		|| pMsg->message == WM_RBUTTONDBLCLK)
	{
		if (GetParentFrame()
			&& GetParentFrame()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG, (WPARAM)pMsg->message,
											 pMsg->lParam))
			return TRUE;
	}

#endif

	return CWnd::PreTranslateMessage(pMsg);
}

void CXTPTabClientWnd::CSingleWorkspace::RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
{
	if (!m_pTabClientWnd->m_bLockReposition)
		InvalidateRect(lpRect, FALSE);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
// Custom message handlers definition is required
BEGIN_MESSAGE_MAP(CXTPTabClientWnd::CSingleWorkspace, CWnd)
	{ WM_LBUTTONDBLCLK,
	  0,
	  0,
	  0,
	  AfxSig_vwp,
	  (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))
		  & CXTPTabClientWnd::CSingleWorkspace::OnLButtonDblClk },
		{ WM_PAINT,
		  0,
		  0,
		  0,
		  AfxSig_vv,
		  (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))
			  & CXTPTabClientWnd::CSingleWorkspace::OnPaint },
		{ WM_PRINTCLIENT,
		  0,
		  0,
		  0,
		  AfxSig_lwl,
		  (AFX_PMSG)(AFX_PMSGW)(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))
			  & CXTPTabClientWnd::CSingleWorkspace::OnPrintClient },
		{ WM_ERASEBKGND,
		  0,
		  0,
		  0,
		  AfxSig_bD,
		  (AFX_PMSG)(AFX_PMSGW)(BOOL(AFX_MSG_CALL CWnd::*)(CDC*))
			  & CXTPTabClientWnd::CSingleWorkspace::OnEraseBkgnd },
		{ WM_LBUTTONDOWN,
		  0,
		  0,
		  0,
		  AfxSig_vwp,
		  (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))
			  & CXTPTabClientWnd::CSingleWorkspace::OnLButtonDown },
		{ WM_MOUSEMOVE,
		  0,
		  0,
		  0,
		  AfxSig_vwp,
		  (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))
			  & CXTPTabClientWnd::CSingleWorkspace::OnMouseMove },
		{ WM_MOUSELEAVE,
		  0,
		  0,
		  0,
		  AfxSig_vv,
		  (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))
			  & CXTPTabClientWnd::CSingleWorkspace::OnMouseLeave },
		{ WM_RBUTTONDOWN,
		  0,
		  0,
		  0,
		  AfxSig_vwp,
		  (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))
			  & CXTPTabClientWnd::CSingleWorkspace::OnRButtonDown },
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPTabClientWnd::CSingleWorkspace::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	UNREFERENCED_PARAMETER(point);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	CXTPTabManagerItem* pItem = HitTest(point);
	m_pTabClientWnd->FireRClick(pItem ? pItem->GetIDispatch(FALSE) : NULL);
#endif
}

void CXTPTabClientWnd::CSingleWorkspace::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint);

	if (m_pTabClientWnd)
	{
		CRect rectClient;
		m_pTabClientWnd->GetWindowRect(rectClient);
		ScreenToClient(&rectClient);
		dcPaint.ExcludeClipRect(rectClient);
		dc.ExcludeClipRect(rectClient);
	}
	GetPaintManager()->DrawTabControl(this, &dc, rc);
}

LRESULT CXTPTabClientWnd::CSingleWorkspace::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		if (m_pTabClientWnd)
		{
			CRect rectClient;
			m_pTabClientWnd->GetWindowRect(rectClient);
			ScreenToClient(&rectClient);
			pDC->ExcludeClipRect(rectClient);
		}
		CXTPClientRect rc(this);
		GetPaintManager()->DrawTabControl(this, pDC, rc);
	}
	return TRUE;
}

BOOL CXTPTabClientWnd::CSingleWorkspace::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPTabClientWnd::CSingleWorkspace::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	m_pTabClientWnd->m_bLockUpdate = TRUE;
#ifndef _XTP_COMMANDBARS_ACTIVEX
	m_pTabClientWnd->m_bDelayLock = TRUE;
#endif

	PerformClick(m_hWnd, point);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	m_pTabClientWnd->m_bLockUpdate = FALSE;
#endif
}

void CXTPTabClientWnd::CSingleWorkspace::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

void CXTPTabClientWnd::CSingleWorkspace::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	PerformMouseMove(m_hWnd, point);
}

void CXTPTabClientWnd::CSingleWorkspace::OnMouseLeave()
{
	PerformMouseMove(m_hWnd, CPoint(-1, -1));
}
