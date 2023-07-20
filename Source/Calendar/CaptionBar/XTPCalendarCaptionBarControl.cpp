// XTPCalendarCaptionBarControl.cpp : implementation file
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

#include "Calendar/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarThemePart.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarTheme.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarControl.h"
#include "Calendar/XTPCalendarControlView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarCaptionBarControl, CWnd)

CXTPCalendarCaptionBarControl::CXTPCalendarCaptionBarControl()
{
	RegisterWindowClass();

	SetBorders(0, 0, 0, 0);

	m_pTheme = NULL;
	SetTheme(new CXTPCalendarCaptionBarTheme());
}

CXTPCalendarCaptionBarControl::~CXTPCalendarCaptionBarControl()
{
	CMDTARGET_RELEASE(m_pTheme);
}

BOOL CXTPCalendarCaptionBarControl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, XTPCALENDARCAPTIONBAR_CLASSNAME,
											  /*CS_DBLCLKS |*/ CS_HREDRAW | CS_VREDRAW);
}

BOOL CXTPCalendarCaptionBarControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
										   UINT nID)
{
	if (!CWnd::Create(XTPCALENDARCAPTIONBAR_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	AdjustLayout(NULL);

	if (m_pTheme)
	{
		CDC* pDC = GetDC();
		if (pDC)
		{
			m_pTheme->m_dScale = pDC->GetDeviceCaps(LOGPIXELSY) / 96.0;
			ReleaseDC(pDC);
		}
	}
	return TRUE;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarCaptionBarControl, CWnd)
	//{{AFX_MSG_MAP(CXTPCalendarCaptionBarControl)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPCalendarCaptionBarControl::SetButtonText(int iButtonNumber, CString sButtonText)
{
	CXTPCalendarCaptionBarTheme* pBarTheme = GetTheme();

	if (!pBarTheme)
		return;

	BOOL bShow = !sButtonText.IsEmpty();

	switch (iButtonNumber)
	{
		case 0: /* DayView */
		{
			if (pBarTheme->m_pButtonViewDay)
			{
				pBarTheme->m_pButtonViewDay->SetText(sButtonText);
				pBarTheme->m_pButtonViewDay->SetVisible(bShow);
			}
		}
		break;
		case 1: /* WeekView */
		{
			if (pBarTheme->m_pButtonViewWeek)
			{
				pBarTheme->m_pButtonViewWeek->SetText(sButtonText);
				pBarTheme->m_pButtonViewWeek->SetVisible(bShow);
			}
		}
		break;
		case 2: /* MonthView */
		{
			if (pBarTheme->m_pButtonViewMonth)
			{
				pBarTheme->m_pButtonViewMonth->SetText(sButtonText);
				pBarTheme->m_pButtonViewMonth->SetVisible(bShow);
			}
		}
		break;
		case 3: /* TimeLine */
		{
			if (pBarTheme->m_pShowTimeline)
			{
				pBarTheme->m_pShowTimeline->SetText(sButtonText);
				pBarTheme->m_pShowTimeline->SetVisible(bShow);
			}
		}
		break;
		case 4: /* WorkWeek */
		{
			if (pBarTheme->m_pShowWorkWeek)
			{
				pBarTheme->m_pShowWorkWeek->SetText(sButtonText);
				pBarTheme->m_pShowWorkWeek->SetVisible(bShow);
			}
		}
		break;
		case 5: /* FullWeek */
		{
			if (pBarTheme->m_pShowFullWeek)
			{
				pBarTheme->m_pShowFullWeek->SetText(sButtonText);
				pBarTheme->m_pShowFullWeek->SetVisible(bShow);
			}
		}
		break;
		case 6: /* Multi-Columns mode */
		{
			if (pBarTheme->m_pShowMcWeek)
			{
				pBarTheme->m_pShowMcWeek->SetText(sButtonText);
				pBarTheme->m_pShowMcWeek->SetVisible(bShow);
			}
		}
		break;
	}
}

void CXTPCalendarCaptionBarControl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	UNREFERENCED_PARAMETER(bCalcValidRects);

	if (!GetTheme())
		return;

	RECT* pClient = &lpncsp->rgrc[0];

	pClient->left += m_Borders.left;
	pClient->top += m_Borders.top;

	pClient->right  = max(pClient->left, pClient->right - m_Borders.right);
	pClient->bottom = max(pClient->top, pClient->bottom - m_Borders.bottom);
}

void CXTPCalendarCaptionBarControl::OnNcPaint()
{
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	CWindowDC dc(this);

	if (GetTheme())
		GetTheme()->DrawBorders(&dc, rc, m_Borders);
}

void CXTPCalendarCaptionBarControl::UpdateBorders()
{
	if (::IsWindow(m_hWnd))
	{
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
					   SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

		Invalidate(NULL);
	}
}

void CXTPCalendarCaptionBarControl::SetBorders(int nLeft, int nTop, int nRight, int nBottom)
{
	m_Borders.SetRect(nLeft, nTop, nRight, nBottom);
	UpdateBorders();
}

CRect CXTPCalendarCaptionBarControl::CalcMinRect(BOOL bExactSize)
{
	if (!m_pTheme)
		return CRect(0, 0, 0, 0);

	CRect rcMin = m_pTheme->CalcMinRect(NULL, bExactSize);
	if (rcMin.Height() > 0)
	{
		CRect rcBorders = GetBorders();

		rcMin.left += rcBorders.left + rcBorders.right;
		rcMin.bottom += rcBorders.top + rcBorders.bottom;
	}

	return rcMin;
}

void CXTPCalendarCaptionBarControl::SetTheme(CXTPCalendarCaptionBarTheme* pTheme)
{
	ASSERT(pTheme);
	if (pTheme)
	{
		CMDTARGET_RELEASE(m_pTheme);
		m_pTheme = pTheme;
		m_pTheme->SetOwner(this);
		m_pTheme->RefreshMetrics();
	}
}

void CXTPCalendarCaptionBarControl::AdjustLayout(CDC* pDC, const CRect* prcWnd)
{
	CXTPClientRect rcClient(this);

	if (prcWnd)
		((CRect&)rcClient) = *prcWnd;

	if (m_pTheme)
		m_pTheme->AdjustLayout(pDC, rcClient);
}

void CXTPCalendarCaptionBarControl::OnPaint()
{
	CXTPClientRect rc(this);

	if (rc.IsRectEmpty())
		return;

	CPaintDC dc(this); // device context for painting

	CXTPBufferDC memDC(dc.GetSafeHdc(), rc);

	if (m_pTheme)
		m_pTheme->Draw(&memDC);
}

BOOL CXTPCalendarCaptionBarControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPCalendarCaptionBarControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!GetSafeHwnd())
		return;

	CRect rcRect(0, 0, cx, cy);
	if (m_pTheme)
		m_pTheme->AdjustLayout(NULL, rcRect);
}

void CXTPCalendarCaptionBarControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pTheme)
		m_pTheme->OnLButtonDown(nFlags, point);

	if (GetCapture() != this)
		SetCapture();
}

void CXTPCalendarCaptionBarControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pTheme)
		m_pTheme->OnLButtonUp(nFlags, point);

	if (GetCapture() == this)
		ReleaseCapture();
}

void CXTPCalendarCaptionBarControl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pTheme)
		m_pTheme->OnMouseMove(nFlags, point);
}

void CXTPCalendarCaptionBarControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pTheme)
		m_pTheme->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPCalendarCaptionBarControl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pTheme)
		m_pTheme->OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPCalendarCaptionBarControl::OnTimer(UINT_PTR uTimerID)
{
	if (m_pTheme)
		m_pTheme->OnTimer(uTimerID);
}

void CXTPCalendarCaptionBarControl::OnDestroy()
{
	if (m_pTheme)
		m_pTheme->OnBeforeDestroy();
}

void CXTPCalendarCaptionBarControl::OnKillFocus(CWnd* pNewWnd)
{
	if (m_pTheme)
		m_pTheme->OnKillFocus(pNewWnd);
}

void CXTPCalendarCaptionBarControl::EnableTimeline(BOOL bEnable)
{
	if (m_pTheme)
		m_pTheme->m_bTimelineEnable = bEnable;
}

BOOL CXTPCalendarCaptionBarControl::IsTimelineEnable()
{
	if (m_pTheme)
		return m_pTheme->m_bTimelineEnable;
	else
		return FALSE;
}

void CXTPCalendarCaptionBarControl::EnableMulticolumns(BOOL bEnable)
{
	if (m_pTheme)
		m_pTheme->m_bMulticolumnsEnable = bEnable;
}

BOOL CXTPCalendarCaptionBarControl::IsMulticolumnsEnable()
{
	if (m_pTheme)
		return m_pTheme->m_bMulticolumnsEnable;
	else
		return FALSE;
}

BOOL CXTPCalendarCaptionBarControl::IsUseOneLine()
{
	if (GetTheme())
		return GetTheme()->m_bIsOneLine;
	else
		return FALSE;
}

void CXTPCalendarCaptionBarControl::UseOneLine(BOOL bSet)
{
	if (GetTheme())
	{
		GetTheme()->m_bIsOneLine = bSet;

		GetTheme()->RefreshMetrics();

		if (::IsWindow(*this))
		{
			CXTPCalendarControlView* pView = DYNAMIC_DOWNCAST(CXTPCalendarControlView, GetParent());
			if (pView)
				pView->_ResizeControls();
		}
	}
}

CXTPCalendarCaptionBarTheme* CXTPCalendarCaptionBarControl::GetTheme() const
{
	return m_pTheme;
}
