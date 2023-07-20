// XTPPreviewNavigatorWindow.cpp : implementation of the CXTPPreviewNavigatorWindow class.
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
#include "Controls/Preview/XTPPreviewNavigatorWindow.h"
#include "Controls/Preview/XTPPreviewCtrl.h"
#include "Controls/Resource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

IMPLEMENT_DYNAMIC(CXTPPreviewNavigatorWindow, CWnd)

CXTPPreviewNavigatorWindow::CXTPPreviewNavigatorWindow(CXTPPreviewCtrl* pPreviewCtrl)
	: m_pPreviewCtrl(pPreviewCtrl)
{
	ASSERT_VALID(pPreviewCtrl);
}

CXTPPreviewNavigatorWindow::~CXTPPreviewNavigatorWindow()
{
}

BOOL CXTPPreviewNavigatorWindow::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(NULL != pMsg);

	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
			case VK_LEFT:
			{
				if (GetKeyState(VK_CONTROL) & 0x800)
				{
					m_FirstBtn.SetFocus();
					OnFirstBtn();
				}
				else
				{
					m_PrevBtn.SetFocus();
					OnPrevBtn();
				}

				return -1;
			}
			case VK_RIGHT:
			{
				if (GetKeyState(VK_CONTROL) & 0x800)
				{
					m_LastBtn.SetFocus();
					OnLastBtn();
				}
				else
				{
					m_NextBtn.SetFocus();
					OnNextBtn();
				}

				return -1;
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPreviewNavigatorWindow, CWnd)
	ON_WM_CREATE()
	ON_COMMAND(XTP_IDC_NAVIGATOR_FIRST_BTN, OnFirstBtn)
	ON_COMMAND(XTP_IDC_NAVIGATOR_LAST_BTN, OnLastBtn)
	ON_COMMAND(XTP_IDC_NAVIGATOR_PREV_BTN, OnPrevBtn)
	ON_COMMAND(XTP_IDC_NAVIGATOR_NEXT_BTN, OnNextBtn)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

int CXTPPreviewNavigatorWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_FirstBitmap.LoadBitmap(XTP_IDR_FIRST_BITMAP))
		return -1;

	BITMAP bm;
	m_FirstBitmap.GetBitmap(&bm);
	int buttonShiftY = bm.bmHeight / 4;
	int buttonH		 = bm.bmHeight + XTP_DPI_Y(2);
	int w			 = bm.bmWidth + XTP_DPI_X(4);
	if (!m_FirstBtn.Create(NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP | WS_TABSTOP,
						   CRect(XTP_DPI_X(5), buttonShiftY, XTP_DPI_X(5) + w,
								 buttonShiftY + buttonH),
						   this, XTP_IDC_NAVIGATOR_FIRST_BTN))
	{
		return -1;
	}

	m_FirstBtn.SetBitmap(m_FirstBitmap);

	if (!m_PreviousBitmap.LoadBitmap(XTP_IDR_PREVIOUS_BITMAP))
		return -1;

	if (!m_PrevBtn.Create(NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP | WS_TABSTOP,
						  CRect(XTP_DPI_X(2 * 5) + w, buttonShiftY, XTP_DPI_X(2 * 5) + 2 * w,
								buttonShiftY + buttonH),
						  this, XTP_IDC_NAVIGATOR_PREV_BTN))
	{
		return -1;
	}

	m_PrevBtn.SetBitmap(m_PreviousBitmap);

	if (!m_NextBitmap.LoadBitmap(XTP_IDR_NEXT_BITMAP))
		return -1;

	if (!m_NextBtn.Create(NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP | WS_TABSTOP,
						  CRect(XTP_DPI_X(3 * 5) + 2 * w, buttonShiftY, XTP_DPI_X(3 * 5) + 3 * w,
								buttonShiftY + buttonH),
						  this, XTP_IDC_NAVIGATOR_NEXT_BTN))
	{
		return -1;
	}

	m_NextBtn.SetBitmap(m_NextBitmap);

	if (!m_LastBitmap.LoadBitmap(XTP_IDR_LAST_BITMAP))
		return -1;

	if (!m_LastBtn.Create(NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP | WS_TABSTOP,
						  CRect(XTP_DPI_X(4 * 5) + 3 * w, buttonShiftY, XTP_DPI_X(4 * 5) + 4 * w,
								buttonShiftY + buttonH),
						  this, XTP_IDC_NAVIGATOR_LAST_BTN))
	{
		return -1;
	}

	m_LastBtn.SetBitmap(m_LastBitmap);

	return 0;
}

void CXTPPreviewNavigatorWindow::OnFirstBtn()
{
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_FIRST_BTN, BN_CLICKED),
							 XTPToLPARAM(m_FirstBtn.m_hWnd));
}

void CXTPPreviewNavigatorWindow::OnLastBtn()
{
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_LAST_BTN, BN_CLICKED),
							 XTPToLPARAM(m_LastBtn.m_hWnd));
}

void CXTPPreviewNavigatorWindow::OnPrevBtn()
{
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_PREV_BTN, BN_CLICKED),
							 XTPToLPARAM(m_PrevBtn.m_hWnd));
}

void CXTPPreviewNavigatorWindow::OnNextBtn()
{
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_NEXT_BTN, BN_CLICKED),
							 XTPToLPARAM(m_NextBtn.m_hWnd));
}

void CXTPPreviewNavigatorWindow::PlaceButtons()
{
	CRect rcClient, firstRec, prevRec, nextRec, lastRec;

	GetClientRect(&rcClient);
	m_FirstBtn.GetClientRect(&firstRec);
	m_PrevBtn.GetClientRect(&prevRec);
	m_NextBtn.GetClientRect(&nextRec);
	m_LastBtn.GetClientRect(&lastRec);

	const int gap = XTP_DPI_X(5);
	int w		  = rcClient.Width();
	int midY	  = (rcClient.top + rcClient.bottom) / 2;
	int firstW	= firstRec.Width();
	int prevW	 = prevRec.Width();
	int nextW	 = nextRec.Width();
	int lastW	 = lastRec.Width();

	int left = w / 2 - (firstW + prevW + nextW + lastW) / 2 - gap * 3 / 2;
	firstRec.SetRect(left, midY - firstRec.Height() / 2, left + firstW,
					 midY + firstRec.Height() / 2);

	left = firstRec.right + gap;
	prevRec.SetRect(left, midY - prevRec.Height() / 2, left + prevW, midY + prevRec.Height() / 2);

	left = prevRec.right + gap;
	nextRec.SetRect(left, midY - nextRec.Height() / 2, left + nextW, midY + nextRec.Height() / 2);

	left = nextRec.right + gap;
	lastRec.SetRect(left, midY - lastRec.Height() / 2, left + lastW, midY + lastRec.Height() / 2);

	HDWP dwp = BeginDeferWindowPos(4);
	dwp		 = DeferWindowPos(dwp, m_FirstBtn, NULL, firstRec.left, firstRec.top, firstRec.Width(),
						  firstRec.Height(), SWP_NOZORDER);
	dwp		 = DeferWindowPos(dwp, m_PrevBtn, NULL, prevRec.left, prevRec.top, prevRec.Width(),
						  prevRec.Height(), SWP_NOZORDER);
	dwp		 = DeferWindowPos(dwp, m_NextBtn, NULL, nextRec.left, nextRec.top, nextRec.Width(),
						  nextRec.Height(), SWP_NOZORDER);
	dwp		 = DeferWindowPos(dwp, m_LastBtn, NULL, lastRec.left, lastRec.top, lastRec.Width(),
						  lastRec.Height(), SWP_NOZORDER);
	EndDeferWindowPos(dwp);
}

void CXTPPreviewNavigatorWindow::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);

	if ((lpwndpos->cx <= 0) && (lpwndpos->cy <= 0))
		return;

	PlaceButtons();
}

void CXTPPreviewNavigatorWindow::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);

	if ((lpwndpos->cx <= 0) && (lpwndpos->cy <= 0))
		return;

	PlaceButtons();
}

BOOL CXTPPreviewNavigatorWindow::OnEraseBkgnd(CDC* pDC)
{
	ASSERT_VALID(pDC);

	if (COLORREF_NULL == m_pPreviewCtrl->GetBackgroundColor())
		return CWnd::OnEraseBkgnd(pDC);

	CXTPClientRect rc(this);
	pDC->FillSolidRect(&rc, m_pPreviewCtrl->GetBackgroundColor());

	return TRUE;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
