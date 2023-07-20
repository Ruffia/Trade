// XTPListCtrlView.cpp : implementation of the CXTPListView and CXTPListCtrl class.
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
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Header/XTPHeaderCtrl.h"
#include "Controls/List/XTPListBase.h"
#include "Controls/List/XTPListCtrlView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPListView
/////////////////////////////////////////////////////////////////////////////
CXTPListView::CXTPListView()
{
	ImplAttach(&GetListCtrl());
}

CXTPListView::~CXTPListView()
{
}

IMPLEMENT_DYNCREATE(CXTPListView, CListView)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPListView, CListView)
	//{{AFX_MSG_MAP(CXTPListView)
	ON_LISTCTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPListCtrl
/////////////////////////////////////////////////////////////////////////////

CXTPListCtrl::CXTPListCtrl()
{
	ImplAttach(this);
}

CXTPListCtrl::~CXTPListCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTPListCtrl, CListCtrl)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXTPListCtrl)
	ON_LISTCTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////

static BOOL OnScrollableListCtrlScroll(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult,
									   CWnd* pEmbeddedControl)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(pResult);

	if (NULL == pEmbeddedControl || NULL == lParam)
		return FALSE;

	CScrollBar* pScrollBar = DYNAMIC_DOWNCAST(CScrollBar,
											  CWnd::FromHandle(reinterpret_cast<HWND>(lParam)));
	if (NULL == pScrollBar)
		return FALSE;

	ASSERT(NULL != pResult);
	ASSERT(WM_VSCROLL == message || WM_HSCROLL == message);

	int nBar = (WM_VSCROLL == message ? SB_VERT : SB_HORZ);

	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);
	si.fMask	  = SIF_ALL;
	pEmbeddedControl->GetScrollInfo(nBar, &si);

	int nPos	= pScrollBar->GetScrollPos();
	int nSBCode = LOWORD(wParam);
	switch (nSBCode)
	{
		case SB_TOP: si.nPos = si.nMin; break;
		case SB_BOTTOM: si.nPos = si.nMax; break;
		case SB_LINEUP: si.nPos -= 1; break;
		case SB_LINEDOWN: si.nPos += 1; break;
		case SB_PAGEUP: si.nPos -= si.nPage; break;
		case SB_PAGEDOWN: si.nPos += si.nPage; break;
		case SB_ENDSCROLL: return TRUE;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
		{
			CRect rcClient;
			pEmbeddedControl->GetClientRect(&rcClient);

			SIZE size = { 0, 0 };
			if (SB_VERT == nBar)
			{
				int cy  = (si.nPage == 0
							   ? static_cast<int>(rcClient.bottom)
							   : static_cast<int>(rcClient.bottom * (si.nMax + 1) / si.nPage));
				LONG dy = HIWORD(wParam) - si.nPos;
				size.cy = cy * dy / (si.nMax + 1);
			}
			else
			{
				int cx  = (si.nPage == 0
							   ? static_cast<int>(rcClient.right)
							   : static_cast<int>(rcClient.right * (si.nMax + 1) / si.nPage));
				LONG dx = HIWORD(wParam) - si.nPos;
				size.cx = cx * dx / (si.nMax + 1);
			}

			CXTPListCtrl* pListControl = DYNAMIC_DOWNCAST(CXTPListCtrl, pEmbeddedControl);
			if (NULL != pListControl)
				pListControl->Scroll(size);

			pEmbeddedControl->GetScrollInfo(nBar, &si);
			pScrollBar->SetScrollPos(si.nPos);

			return TRUE;
		}
	}

	pScrollBar->SetScrollPos(nPos);
	pEmbeddedControl->SendMessage(message, XTPToWPARAM(MAKELONG(nSBCode, nPos)), 0);
	return TRUE;
}

BOOL AFXAPI XTPOnScrollableListCtrlWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
										  LRESULT* pResult, CWnd* pEmbeddedControl)
{
	if (WM_VSCROLL == message || WM_HSCROLL == message)
		return OnScrollableListCtrlScroll(message, wParam, lParam, pResult, pEmbeddedControl);

	return FALSE;
}
