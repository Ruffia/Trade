// XTPPropertyGridToolBar.cpp : implementation of the CXTPPropertyGridToolBar class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridToolBar.h"
#include "PropertyGrid/XTPPropertyGrid.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridToolBar

CXTPPropertyGridToolBar::CXTPPropertyGridToolBar()
{
	m_cxLeftBorder = m_cxRightBorder = m_cyBottomBorder = m_cyTopBorder = 0;
}

CXTPPropertyGridToolBar::~CXTPPropertyGridToolBar()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPropertyGridToolBar, CToolBar)
	//{{AFX_MSG_MAP(CToolBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPPropertyGridToolBar::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* lpLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult			   = CDRF_DODEFAULT;

	switch (lpLVCD->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
			CWnd* pGrid   = GetParent();
			HBRUSH hBrush = (HBRUSH)pGrid->GetParent()->SendMessage(WM_CTLCOLORSTATIC,
																	(WPARAM)lpLVCD->nmcd.hdc,
																	(LPARAM)pGrid->m_hWnd);

			::FillRect(lpLVCD->nmcd.hdc, &lpLVCD->nmcd.rc,
					   hBrush ? hBrush : GetSysColorBrush(COLOR_3DFACE));
			break;
	}
}

BOOL CXTPPropertyGridToolBar::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGridToolBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* /*lpncsp*/)
{
}

void CXTPPropertyGridToolBar::OnPaint()
{
	CPaintDC dc(this);

	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc, rc);
	memDC.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	CToolBar::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
}

void CXTPPropertyGridToolBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}

BOOL CXTPPropertyGridToolBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPPropertyGrid* pGrid = DYNAMIC_DOWNCAST(CXTPPropertyGrid, GetParent());
	if (pGrid && pGrid->GetToolTipContext())
	{
		pGrid->GetToolTipContext()->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
