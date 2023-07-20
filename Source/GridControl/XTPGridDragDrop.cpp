// XTPGridDragDrop.cpp : implementation of the CXTPGridHeaderDropWnd class.
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
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridDragDrop.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPGridHeaderDragWnd

CXTPGridHeaderDragWnd::CXTPGridHeaderDragWnd()
{
	m_pHeader		= NULL;
	m_pPaintManager = NULL;
	m_pColumn		= NULL;
}

CXTPGridHeaderDragWnd::~CXTPGridHeaderDragWnd()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridHeaderDragWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPGridHeaderDragWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPGridHeaderDragWnd message handlers

BOOL CXTPGridHeaderDragWnd::Create(CRect rect, CXTPGridHeader* pHeader,
								   CXTPGridPaintManager* pPaintManager, CXTPGridColumn* pColumn)
{
	m_pHeader		= pHeader;
	m_pPaintManager = pPaintManager;
	m_pColumn		= pColumn;
	if (rect.Height() == 0)
		rect.bottom = rect.top + pPaintManager->GetHeaderHeight();

	DWORD dwStyle   = WS_POPUP | WS_DISABLED;
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	BOOL bCreated = CreateEx(dwExStyle,
							 AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
							 NULL, dwStyle, rect.left, rect.top, rect.Width(), rect.Height(), NULL,
							 NULL, NULL);

	if (bCreated && m_pHeader && m_pHeader->GetControl()
		&& m_pHeader->GetControl()->m_bHScrollBarVisible)
	{
		SetTimer(1, 100, NULL);
	}
	return bCreated;
}

void CXTPGridHeaderDragWnd::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc, rc);
	OnDraw(&memDC, rc);
}

BOOL CXTPGridHeaderDragWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPGridHeaderDragWnd::OnDraw(CDC* pDC, CRect rcClient)
{
	if (!m_pPaintManager)
		return;

	// draw background
	m_pPaintManager->FillHeaderControl(pDC, rcClient);
	// draw column header
	m_pPaintManager->DrawColumn(pDC, m_pColumn, m_pHeader, rcClient, TRUE);
}

void CXTPGridHeaderDragWnd::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;
}

void CXTPGridHeaderDragWnd::OnTimer(UINT_PTR nIDEvent)
{
	UNREFERENCED_PARAMETER(nIDEvent);

	if (!m_pHeader || !m_pHeader->GetControl())
	{
		return;
	}
	CXTPGridControl* pControl = m_pHeader->GetControl();

	ASSERT(pControl->m_bHScrollBarVisible);

	CRect rcHeaderScreen = m_pHeader->m_rcHeader;
	pControl->ClientToScreen(&rcHeaderScreen);

	CRect rcControlScreen(0, 0, 0, 0);
	CRect rcDragScreen(0, 0, 0, 0);

	pControl->GetWindowRect(&rcControlScreen);
	GetWindowRect(&rcDragScreen);

	CPoint ptMouseScreen(0, 0);
	if (!GetCursorPos(&ptMouseScreen) || ptMouseScreen.y < rcHeaderScreen.top
		|| ptMouseScreen.y > rcHeaderScreen.bottom)
	{
		return;
	}

	BOOL bScrolled  = FALSE;
	BOOL bLayoutRTL = (0 != (pControl->GetExStyle() & WS_EX_LAYOUTRTL));

	int nDiffRight = bLayoutRTL ? rcControlScreen.left - rcDragScreen.left
								: rcDragScreen.right - rcControlScreen.right;

	//-----------------------------------------------------------------------
	CXTPGridColumn *pPrev = NULL, *pCurr = NULL, *pNext = NULL;
	int nScrollPos = 0, nScrollMax = 0;
	BOOL bFullColumnScrolling = pControl->IsFullColumnScrolling();
	if (bFullColumnScrolling)
		m_pHeader->GetFullColScrollInfo(pPrev, pCurr, pNext, nScrollPos, nScrollMax);
	//-----------------------------------------------------------------------

	if (nDiffRight > 0)
	{
		int nStep			  = max(min(7, nDiffRight / 10), CXTPGridHeader::s_nMinAutoScrollStep);
		UINT uTimer			  = XTPToUInt(bFullColumnScrolling ? max(30, 800 - nDiffRight * 10)
													   : max(10, 100 - nDiffRight));
		SCROLLINFO scrollInfo = { 0 };

		BOOL bRes = pControl->GetScrollInfo(SB_HORZ, &scrollInfo);
		if (bRes)
		{
			if (!bFullColumnScrolling && scrollInfo.nPos < scrollInfo.nMax
				|| bFullColumnScrolling && nScrollPos < nScrollMax)
			{
				if (bFullColumnScrolling)
					pControl->SendMessage(WM_HSCROLL, SB_LINERIGHT);
				else
					pControl->SetScrollOffsetH(scrollInfo.nPos + nStep);

				KillTimer(1);
				SetTimer(1, uTimer, NULL);
				bScrolled = TRUE;
			}
		}
	}
	else
	{
		int nLeftBorderX = bLayoutRTL ? rcControlScreen.right : rcControlScreen.left;

		int nLastFrozenColumnIndex = -1;
		int nFreezCols			   = pControl->GetFreezeColumnsCount(nLastFrozenColumnIndex);
		if (nFreezCols && pControl->GetColumns())
		{
			CXTPGridColumn* pLastFCol = pControl->GetColumns()->GetAt(nLastFrozenColumnIndex);
			if (pLastFCol)
			{
				CRect rcLastFCol = pLastFCol->GetRect();
				pControl->ClientToScreen(&rcLastFCol);

				int nLastFCol_middleX = rcLastFCol.left + rcLastFCol.Width() / 2;
				if (rcDragScreen.left < rcLastFCol.right && ptMouseScreen.x > nLastFCol_middleX)
				{
					nLeftBorderX = rcLastFCol.right;
				}
			}
		}

		int nDiffLeft = bLayoutRTL ? rcDragScreen.right - nLeftBorderX
								   : nLeftBorderX - rcDragScreen.left;

		if (nDiffLeft > 0)
		{
			int nStep   = max(min(7, nDiffLeft / 10), CXTPGridHeader::s_nMinAutoScrollStep);
			UINT uTimer = XTPToUInt(bFullColumnScrolling ? max(30, 800 - nDiffLeft * 10)
														 : max(10, 100 - nDiffLeft));
			SCROLLINFO scrollInfo = { 0 };

			BOOL bRes = pControl->GetScrollInfo(SB_HORZ, &scrollInfo);
			if (bRes)
			{
				if (scrollInfo.nPos > scrollInfo.nMin)
				{
					if (bFullColumnScrolling)
						pControl->SendMessage(WM_HSCROLL, SB_LINELEFT);
					else
						pControl->SetScrollOffsetH(scrollInfo.nPos - nStep);

					SetTimer(1, uTimer, NULL);
					bScrolled = TRUE;
				}
			}
		}
	}

	//-------------------------------------------------------------------------
	if (bScrolled)
	{
		CPoint ptMouse = ptMouseScreen;
		pControl->ScreenToClient(&ptMouse);

		pControl->RedrawControl();
		pControl->UpdateWindow();

		m_pHeader->OnMouseMove(0, ptMouse);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPGridHeaderDropWnd

CXTPGridHeaderDropWnd::CXTPGridHeaderDropWnd(COLORREF crColor)
	: m_clr(crColor)
{
	m_nHeight = 0;
}

CXTPGridHeaderDropWnd::~CXTPGridHeaderDropWnd()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridHeaderDropWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPGridHeaderDropWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPGridHeaderDropWnd message handlers

BOOL CXTPGridHeaderDropWnd::Create(CWnd* /*pParentWnd*/, int nHeight)
{
	m_nHeight = nHeight + XTP_DPI_Y(20);

	DWORD dwStyle   = WS_POPUP | WS_DISABLED;
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	BOOL bResult = CreateEx(dwExStyle,
							AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
							NULL, dwStyle, 0, 0, XTP_DPI_X(12), m_nHeight, NULL, NULL, NULL);

	CRgn rgn1, rgn2;
	POINT ptArrow[7];

	ptArrow[0].x = XTP_DPI_X(8);
	ptArrow[0].y = 0;
	ptArrow[1].x = XTP_DPI_X(8);
	ptArrow[1].y = XTP_DPI_Y(4);
	ptArrow[2].x = XTP_DPI_X(11);
	ptArrow[2].y = XTP_DPI_Y(4);
	ptArrow[3].x = XTP_DPI_X(6);
	ptArrow[3].y = XTP_DPI_Y(9);
	ptArrow[4].x = XTP_DPI_X(1);
	ptArrow[4].y = XTP_DPI_Y(4);
	ptArrow[5].x = XTP_DPI_X(4);
	ptArrow[5].y = XTP_DPI_Y(4);
	ptArrow[6].x = XTP_DPI_X(4);
	ptArrow[6].y = 0;
	rgn1.CreatePolygonRgn(ptArrow, 7, ALTERNATE);

	ptArrow[0].x = XTP_DPI_X(4);
	ptArrow[0].y = m_nHeight;
	ptArrow[1].x = XTP_DPI_X(4);
	ptArrow[1].y = m_nHeight - XTP_DPI_Y(4);
	ptArrow[2].x = 0;
	ptArrow[2].y = m_nHeight - XTP_DPI_Y(4);
	ptArrow[3].x = XTP_DPI_X(6);
	ptArrow[3].y = m_nHeight - XTP_DPI_Y(10);
	ptArrow[4].x = XTP_DPI_X(12);
	ptArrow[4].y = m_nHeight - XTP_DPI_Y(4);
	ptArrow[5].x = XTP_DPI_X(8);
	ptArrow[5].y = m_nHeight - XTP_DPI_Y(4);
	ptArrow[6].x = XTP_DPI_X(8);
	ptArrow[6].y = m_nHeight;
	rgn2.CreatePolygonRgn(ptArrow, 7, ALTERNATE);

	m_rgn.CreateRectRgn(0, 0, XTP_DPI_X(12), nHeight);
	m_rgn.CombineRgn(&rgn1, &rgn2, RGN_OR);
	SetWindowRgn(m_rgn, FALSE);

	rgn1.DeleteObject();
	rgn2.DeleteObject();

	return bResult;
}

void CXTPGridHeaderDropWnd::PostNcDestroy()
{
	m_rgn.DeleteObject();

	CWnd::PostNcDestroy();
	delete this;
}

BOOL CXTPGridHeaderDropWnd::OnEraseBkgnd(CDC* pDC)
{
	pDC->FillSolidRect(0, 0, XTP_DPI_X(12), m_nHeight, m_clr);
	return TRUE;
}

void CXTPGridHeaderDropWnd::SetWindowPos(int x, int y)
{
	CWnd::SetWindowPos(0, x - XTP_DPI_X(6), y - (m_nHeight / 2), 0, 0,
					   SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER
						   | SWP_NOOWNERZORDER);
}
