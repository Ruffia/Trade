// XTPDockingPaneContext.cpp : implementation of the CXTPDockingPaneContext class.
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

#include "StdAfx.h"

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#include "TabManager/Includes.h"

#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
#		include "CommandBars/XTPCommandBarsDefines.h"
#		include "CommandBars/XTPTabClientWnd.h"
#	endif
#endif

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPaneContext.h"
#include "DockingPane/XTPDockingPane.h"
#include "DockingPane/XTPDockingPaneManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPaneSplitterContainer.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPaneSidePanel.h"
#include "DockingPane/XTPDockingPanePaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFX_CDECL CXTPDockingPaneContext::AdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left)
					   ? (pt.x - rect.left - XTP_DPI_X(6))
					   : (pt.x > rect.right) ? (pt.x - rect.right + XTP_DPI_X(6)) : 0;
	int nYOffset = (pt.y < rect.top)
					   ? (pt.y - rect.top - XTP_DPI_Y(6))
					   : (pt.y > rect.bottom) ? (pt.y - rect.bottom + XTP_DPI_Y(6)) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}

void AFX_CDECL CXTPDockingPaneContext::AdjustCursor(CPoint& pt)
{
	CRect rc = XTPMultiMonitor()->GetWorkArea(pt);
	if (pt.x < rc.left)
		pt.x = rc.left;
	if (pt.x > rc.right)
		pt.x = rc.right;
	if (pt.y < rc.top)
		pt.y = rc.top;
	if (pt.y > rc.bottom)
		pt.y = rc.bottom;
}

typedef BOOL(WINAPI* PFNSETLAYEREDWINDOWATTRIBUTES)(HWND hwnd, COLORREF crKey, BYTE bAlpha,
													DWORD dwFlags);

#ifndef LWA_ALPHA
#	define LWA_ALPHA 0x00000002
#endif

#ifndef WS_EX_LAYERED
#	define WS_EX_LAYERED 0x00080000
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextAlphaWnd

IMPLEMENT_DYNCREATE(CXTPDockingPaneContextAlphaWnd, CWnd)

CXTPDockingPaneContextAlphaWnd::CXTPDockingPaneContextAlphaWnd()
{
}

CXTPDockingPaneContextAlphaWnd::~CXTPDockingPaneContextAlphaWnd()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPDockingPaneContextAlphaWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPDockingPaneContextAlphaWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextAlphaWnd message handlers

BOOL CXTPDockingPaneContextAlphaWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPDockingPaneContextAlphaWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContext

CXTPDockingPaneContext::CXTPDockingPaneContext()
{
	m_bUseAlphaContext	= FALSE;
	m_bUseDockingStickers = FALSE;
	m_bDragKeyboard		  = FALSE;

	m_pStickerPane	 = NULL;
	m_pLastStickerPane = NULL;
	m_pManager		   = NULL;
	m_pPane			   = NULL;
	m_pContainer	   = NULL;
	m_bAttachLast	  = FALSE;
	m_bSideDock		   = FALSE;
	m_bAttach		   = FALSE;
	m_bFloatable	   = TRUE;
	m_bDockable		   = TRUE;
	m_bDitherLast	  = FALSE;
	m_containDirection = xtpPaneDockUndefined;
	m_pDC			   = NULL;

	m_pfnSetLayeredWindowAttributes = NULL;

	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		m_pfnSetLayeredWindowAttributes = (PVOID)::GetProcAddress(hLib,
																  "SetLayeredWindowAttributes");
	}

	m_bResetDC = FALSE;
}

CXTPDockingPaneContext::~CXTPDockingPaneContext()
{
}

void CXTPDockingPaneContext::Drag(CXTPDockingPaneBase* pPane, CPoint point)
{
	CRect rcPane = pPane->GetPaneWindowRect();

	CWnd* pSite = pPane->GetDockingSite();
	if (pSite
		&& (pPane->GetType() == xtpPaneTypeDockingPane
			|| pPane->GetType() == xtpPaneTypeTabbedContainer)
		&& !pSite->IsKindOf(RUNTIME_CLASS(CXTPDockingPaneMiniWnd)))
	{
		CXTPDockingPane* pActivePane = pPane->GetType() == xtpPaneTypeTabbedContainer
										   ? ((CXTPDockingPaneTabbedContainer*)pPane)->GetSelected()
										   : (CXTPDockingPane*)pPane;

		if (pActivePane)
		{
			CXTPDockingPaneTabbedContainer* pHolder = (CXTPDockingPaneTabbedContainer*)
														  m_pManager->_GetHolder(pActivePane, TRUE);
			pSite = pHolder ? pHolder->GetDockingSite() : NULL;

			if (pSite && pSite->IsKindOf(RUNTIME_CLASS(CXTPDockingPaneMiniWnd)))
			{
				CRect rcFloating = ((CXTPDockingPaneMiniWnd*)pSite)->GetPaneWindowRect();

				rcPane.right  = rcPane.left + rcFloating.Width();
				rcPane.bottom = rcPane.top + rcFloating.Height();
			}
			else if (!pHolder)
			{
				rcPane.right  = rcPane.left + pActivePane->m_szDocking.cx;
				rcPane.bottom = rcPane.top + pActivePane->m_szDocking.cy;
			}
		}
	}

	Drag(pPane, point, rcPane);
}

void CXTPDockingPaneContext::Drag(CXTPDockingPaneBase* pPane, CPoint pt, CRect rect)
{
	m_bDragKeyboard = (pt == CPoint(-1, -1));

	if (m_bDragKeyboard)
	{
		pt.x = rect.CenterPoint().x;
		pt.y = rect.top + XTP_DPI_Y(15);
		SetCursorPos(pt.x, pt.y);
	}

	m_ptSticky = m_ptLast = pt;

	AdjustRectangle(rect, pt);

	m_pPane				  = pPane;
	m_rectDragFrameScreen = m_rectDragFrame = rect;
	m_pDC									= NULL;
	m_pContainer							= NULL;
	m_bAttach = m_bAttachLast = FALSE;
	m_bFloatable			  = TRUE;
	m_bDockable				  = TRUE;
	m_bSideDock				  = FALSE;

	m_bUseAlphaContext = m_pManager->IsAlphaDockingContext()
						 && (m_pfnSetLayeredWindowAttributes != NULL);
	m_bUseDockingStickers = m_pManager->IsShowDockingContextStickers() && m_bUseAlphaContext;

	CXTPDockingPaneBaseList lst;
	pPane->FindPane(xtpPaneTypeDockingPane, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pListPane = (CXTPDockingPane*)lst.GetNext(pos);

		if (m_pManager->NotifyAction(xtpPaneActionFloating, pListPane))
			m_bFloatable = FALSE;

		if (pListPane->GetOptions() & xtpPaneNoFloatable)
			m_bFloatable = FALSE;

		if (pListPane->GetOptions() & xtpPaneNoDockable)
			m_bDockable = FALSE;
	}

	if (m_pManager->m_bShowSizeCursorWhileDragging)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
	}

	InitLoop();

	Track();
}

void CXTPDockingPaneContext::ProcessPendingPaintMessages()
{
	if (!m_bUseAlphaContext)
	{
		MSG msg;
		if (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			ReleaseDesktopDC();

			CXTPDrawHelpers::ProcessPendingPaintMessages();

			AcquireDesktopDC();
		}
	}
}

void CXTPDockingPaneContext::InitLoop()
{
	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bDitherLast				  = FALSE;
	m_rectStickerPane.SetRectEmpty();
	m_pStickerPane	 = NULL;
	m_pLastStickerPane = NULL;
	m_bResetDC		   = FALSE;

	if (!m_bUseAlphaContext)
	{
		CXTPDrawHelpers::ProcessPendingPaintMessages();

		// lock window update while dragging
		ASSERT(m_pDC == NULL);
		AcquireDesktopDC();
	}
}
void CXTPDockingPaneContext::CancelLoop()
{
	ReleaseCapture();

	if (m_bUseAlphaContext)
	{
		m_wndContext.DestroyWindow();
		m_wndAttachedTab.DestroyWindow();

		DestroyDockingStickers();

		POSITION pos = m_rgnStickers.GetStartPosition();
		while (pos)
		{
			UINT nKey;
			CRgn* pRgn;
			m_rgnStickers.GetNextAssoc(pos, nKey, pRgn);
			delete pRgn;
		}
		m_rgnStickers.RemoveAll();
	}
	else
	{
		DrawFocusRect(TRUE); // gets rid of focus rect
		ReleaseDesktopDC();
	}
}

void CXTPDockingPaneContext::_CreateRectangleRgn(CRgn& rgnResult, CRect rc)
{
	CRgn rgnOutside, rgnInside;
	rgnOutside.CreateRectRgnIndirect(&rc);
	CRect rect = rc;
	rect.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4));
	rect.IntersectRect(rect, rc);
	rgnInside.CreateRectRgnIndirect(rect);
	rgnResult.CreateRectRgn(0, 0, 0, 0);
	rgnResult.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);
	rgnOutside.DeleteObject();
	rgnInside.DeleteObject();
}

void CXTPDockingPaneContext::_CreateRgn(CRgn& rgnResult, CRect rc, BOOL bTabbedRgn, BOOL bRemove)
{
	if (bRemove)
	{
		rgnResult.CreateRectRgn(0, 0, 0, 0);
		return;
	}

	if (bTabbedRgn)
	{
		CSize szTab(min(XTP_DPI_X(50), rc.Width() - XTP_DPI_X(5)),
					min(XTP_DPI_Y(20), rc.Height() / 2));
		CRect rcIntersect, rcTop(rc.left, rc.top, rc.right, rc.bottom - szTab.cy),
			rcBottom(rc.left + XTP_DPI_X(5), rc.bottom - szTab.cy - XTP_DPI_Y(4),
					 rc.left + XTP_DPI_X(5) + szTab.cx, rc.bottom);

		CRgn rgnTop, rgnBottom, rgnIntersect;

		rcIntersect.IntersectRect(rcTop, rcBottom);
		rcIntersect.DeflateRect(XTP_DPI_X(4), 0);

		_CreateRectangleRgn(rgnTop, rcTop);
		_CreateRectangleRgn(rgnBottom, rcBottom);
		_CreateRectangleRgn(rgnIntersect, rcIntersect);

		rgnResult.CreateRectRgn(0, 0, 0, 0);
		rgnResult.CombineRgn(&rgnBottom, &rgnTop, RGN_OR);
		rgnResult.CombineRgn(&rgnResult, &rgnIntersect, RGN_XOR);

		rgnTop.DeleteObject();
		rgnBottom.DeleteObject();
		rgnIntersect.DeleteObject();
	}
	else
	{
		_CreateRectangleRgn(rgnResult, rc);
	}
}

void CXTPDockingPaneContext::DrawFocusRect(BOOL bRemoveRect)
{
	if (m_pManager->m_bShowContentsWhileDragging && m_pContainer == NULL && m_bFloatable
		&& !m_bAttach && !m_rectDragFrame.IsRectEmpty())
	{
		CRect rect = m_rectDragFrame;

		m_wndAttachedTab.DestroyWindow();
		m_wndContext.DestroyWindow();

		if (!m_bUseAlphaContext && !m_bResetDC)
		{
			ASSERT(m_pDC != NULL);
			// determine new rect and size

			rect.SetRectEmpty();

			// first, determine the update region and select it
			CRgn rgnNew, rgnLast, rgnUpdate;

			_CreateRgn(rgnNew, rect, m_bAttach, TRUE);
			_CreateRgn(rgnLast, m_rectLast, m_bAttachLast);

			rgnUpdate.CreateRectRgn(0, 0, 0, 0);
			rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);

			// draw into the update/new region
			m_pDC->SelectClipRgn(&rgnUpdate);
			m_pDC->GetClipBox(&rect);
			CBrush* pBrushOld = m_pDC->SelectObject(CDC::GetHalftoneBrush());
			m_pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
			// cleanup DC

			if (pBrushOld != NULL)
				m_pDC->SelectObject(pBrushOld);

			m_pDC->SelectClipRgn(NULL);
			rgnUpdate.DeleteObject();
			rgnNew.DeleteObject();
			rgnLast.DeleteObject();

			ReleaseDesktopDC();

			m_bResetDC = TRUE;
		}

		if (m_pPane->GetType() == xtpPaneTypeSplitterContainer)
		{
			if (m_rectDragFrame != m_rectLast)
			{
				m_pPane->GetDockingSite()->MoveWindow(m_rectDragFrame);
			}
		}
		else if (!bRemoveRect)
		{
			XTPDockingPaneAction actionNotify = xtpPaneActionFloated;

			CXTPDockingPaneBaseList lst;
			m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);

			if (m_rectDragFrame.top < 0)
				m_rectDragFrame.OffsetRect(0, -m_rectDragFrame.top);

			CXTPDockingPaneMiniWnd* pMiniWnd = m_pManager->FloatPane(m_pPane, m_rectDragFrame);

			m_pManager->RecalcFrameLayout(m_pPane);
			SAFE_CALLPTR(pMiniWnd, RecalcLayout());

			m_pPane = pMiniWnd->GetTopPane();

			POSITION pos = lst.GetHeadPosition();
			while (pos)
			{
				CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);
				m_pManager->NotifyAction(actionNotify, pPane);
			}

			XTPGetThread()->PumpMessage();
			XTPGetThread()->OnIdle(0);
		}

		m_rectLast	= rect;
		m_bAttachLast = FALSE;

		return;
	}

	if (m_bResetDC)
	{
		ASSERT(m_bUseAlphaContext == FALSE);

		// lock window update while dragging
		ReleaseDesktopDC();
		CXTPDrawHelpers::ProcessPendingPaintMessages();
		AcquireDesktopDC();

		m_bResetDC = FALSE;

		m_sizeLast.cx = m_sizeLast.cy = 0;
		m_bDitherLast				  = FALSE;
		m_rectLast.SetRectEmpty();
	}

	if (m_bUseAlphaContext)
	{
		ASSERT(bRemoveRect == FALSE);

		CRect rect = (m_pContainer == 0) ? m_rectDragFrame : m_rectContainer;

		m_rectLast = rect;

		if (!m_bFloatable && m_pContainer == 0)
		{
			rect.SetRectEmpty();
		}

		if (m_bAttach != m_bAttachLast)
		{
			m_bAttachLast = m_bAttach;
			if (m_bAttach)
				CreateContextWindow(&m_wndAttachedTab);
			else
				m_wndAttachedTab.DestroyWindow();
		}

		if (m_bAttach)
		{
			CSize szTab(min(XTP_DPI_X(50), rect.Width() - XTP_DPI_X(5)),
						min(XTP_DPI_Y(20), rect.Height() / 2));
			CRect rcTab;

			if (m_pManager->GetPaintManager()->GetTabPaintManager()->GetPosition()
					== xtpTabPositionTop
				&& m_pContainer && m_pContainer->GetType() == xtpPaneTypeTabbedContainer)
			{
				if (((CXTPDockingPaneTabbedContainer*)m_pContainer)->IsTitleVisible())
				{
					rect.top += m_pManager->GetPaintManager()->GetCaptionHeight() + XTP_DPI_Y(3);
				}

				rect.top += szTab.cy;
				rcTab = CRect(rect.left + XTP_DPI_X(5), rect.top - szTab.cy,
							  rect.left + XTP_DPI_X(5) + szTab.cx, rect.top);
			}
			else
			{
				rect.bottom -= szTab.cy;
				rcTab = CRect(rect.left + XTP_DPI_X(5), rect.bottom,
							  rect.left + XTP_DPI_X(5) + szTab.cx, rect.bottom + szTab.cy);
			}

			m_wndAttachedTab.SetWindowPos(0, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(),
										  SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
		}

		if (m_wndContext.GetSafeHwnd() == 0 || CXTPWindowRect(&m_wndContext).Size() != rect.Size())
		{
			m_wndContext.DestroyWindow();
			CreateContextWindow(&m_wndContext);
		}

		m_wndContext.SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(),
								  SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}
	else
	{
		ASSERT(m_pDC != NULL);
		// determine new rect and size
		CRect rect = (m_pContainer == 0) ? m_rectDragFrame : m_rectContainer;

		if (!m_bFloatable && m_pContainer == 0)
			rect.SetRectEmpty();

		// first, determine the update region and select it
		CRgn rgnNew, rgnLast, rgnUpdate;

		_CreateRgn(rgnNew, rect, m_bAttach, bRemoveRect);
		_CreateRgn(rgnLast, m_rectLast, m_bAttachLast);

		rgnUpdate.CreateRectRgn(0, 0, 0, 0);
		rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);

		m_rectLast	= rect;
		m_bAttachLast = m_bAttach;

		// draw into the update/new region
		m_pDC->SelectClipRgn(&rgnUpdate);
		m_pDC->GetClipBox(&rect);
		CBrush* pBrushOld = m_pDC->SelectObject(CDC::GetHalftoneBrush());
		m_pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
		// cleanup DC

		if (pBrushOld != NULL)
			m_pDC->SelectObject(pBrushOld);

		m_pDC->SelectClipRgn(NULL);

		rgnNew.DeleteObject();
		rgnLast.DeleteObject();
		rgnUpdate.DeleteObject();
	}
}

BOOL CXTPDockingPaneContext::CanSideDock(CXTPDockingPaneBase* pPane, CRect rcFrame)
{
	CRect rc = pPane->GetPaneWindowRect();
	rc.DeflateRect(m_pManager->GetSideDockingMargin());

	int dSize = 10;

	if (rc.Height() == 0 || rc.Width() == 0 || rcFrame.Height() == 0 || rcFrame.Width() == 0)
		return FALSE;

	if (!CRect().IntersectRect(rc, rcFrame))
		return FALSE;

	m_rectContainer.SetRectEmpty();

	if (abs(rcFrame.top - rc.top) < dSize)
	{
		m_containDirection = xtpPaneDockTop;
		m_rectContainer	= CRect(rcFrame.left, rc.top, rcFrame.right, rc.top + rcFrame.Height());

		dSize = abs(rcFrame.top - rc.top);
	}

	if (abs(rc.bottom - rcFrame.bottom) < dSize)
	{
		m_containDirection = xtpPaneDockBottom;
		m_rectContainer	= CRect(rcFrame.left, rc.bottom - rcFrame.Height(), rcFrame.right,
								   rc.bottom);

		dSize = abs(rc.bottom - rcFrame.bottom);
	}

	if (abs(rcFrame.left - rc.left) < dSize)
	{
		m_containDirection = xtpPaneDockLeft;
		m_rectContainer	= CRect(rc.left, rcFrame.top, rc.left + rcFrame.Width(), rcFrame.bottom);

		dSize = abs(rcFrame.left - rc.left);
	}

	if (abs(rc.right - rcFrame.right) < dSize)
	{
		m_containDirection = xtpPaneDockRight;
		m_rectContainer = CRect(rc.right - rcFrame.Width(), rcFrame.top, rc.right, rcFrame.bottom);
	}

	if (!m_rectContainer.IsRectEmpty())
	{
		if (!IsAllowDockingTo(pPane, m_containDirection))
			return FALSE;

		m_pContainer = pPane;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPDockingPaneContext::CanDock(CRect rc, CPoint pt, CXTPDockingPaneBase* pPane, BOOL bInside)
{
	double dSizeX = XTP_DPI_X(20);
	double dSizeY = XTP_DPI_Y(20);

	double dRatio = 1.0;

	if (bInside && !m_bFloatable)
	{
		if (rc.Width() > 1 && rc.Height() > 1)
			dRatio = (double)rc.Height() / (double)rc.Width();
		dSizeX = dSizeY = 32000.0;
	}

	if (rc.Height() == 0 || rc.Width() == 0)
		return FALSE;

	if (bInside && !rc.PtInRect(pt))
		return FALSE;

	if (!bInside
		&& !CRect(rc.left - XTP_DPI_X(20), rc.top - XTP_DPI_Y(20), rc.right + XTP_DPI_X(20),
				  rc.bottom + XTP_DPI_Y(20))
				.PtInRect(pt))
		return FALSE;

	int nInside = bInside ? 1 : -1;
	BOOL bFound = FALSE;

	if ((double)abs(pt.y - rc.top) < dSizeY && nInside * (pt.y - rc.top) >= 0)
	{
		m_containDirection = xtpPaneDockTop;
		dSizeY			   = (double)abs(pt.y - rc.top);
		bFound			   = TRUE;
	}

	if ((double)abs(rc.bottom - pt.y) < dSizeY && nInside * (rc.bottom - pt.y) >= 0)
	{
		m_containDirection = xtpPaneDockBottom;
		dSizeY			   = (double)abs(rc.bottom - pt.y);
		bFound			   = TRUE;
	}

	if ((double)abs(pt.x - rc.left) * dRatio < dSizeX && nInside * (pt.x - rc.left) >= 0)
	{
		m_containDirection = xtpPaneDockLeft;
		dSizeX			   = (double)abs(pt.x - rc.left) * dRatio;
		bFound			   = TRUE;
	}

	if ((double)abs(rc.right - pt.x) * dRatio < dSizeX && nInside * (rc.right - pt.x) >= 0)
	{
		m_containDirection = xtpPaneDockRight;
		bFound			   = TRUE;
	}

	if (bFound)
	{
		if (!IsAllowDockingTo(pPane, m_containDirection))
			return FALSE;

		m_rectContainer = m_pManager->_CalculateResultDockingRect(m_pPane, m_containDirection,
																  pPane);
		m_pContainer	= pPane;
	}
	return bFound;
}

BOOL CXTPDockingPaneContext::IsAllowDockingTo(CXTPDockingPaneBase* pPane,
											  XTPDockingPaneDirection direction)
{
	CXTPDockingPaneBaseList lst;
	m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* p = (CXTPDockingPane*)lst.GetNext(pos);

		if (m_pManager->NotifyAction(xtpPaneActionDocking, p, pPane, direction))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CXTPDockingPaneContext::IsAllowAttachTo(CXTPDockingPaneBase* pPane)
{
	CXTPDockingPaneBaseList lst;
	m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* p = (CXTPDockingPane*)lst.GetNext(pos);

		if (m_pManager->NotifyAction(xtpPaneActionAttaching, p, pPane))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CXTPDockingPaneContext::IsBehind(CXTPDockingPaneBase* pPaneBase,
									  CXTPDockingPaneBase* pPaneTest)
{
	CWnd* pFrameBase = pPaneBase->GetDockingSite();
	if (!pFrameBase || !pFrameBase->IsKindOf(RUNTIME_CLASS(CXTPDockingPaneMiniWnd)))
		return TRUE;

	CWnd* pFrameTest = pPaneTest->GetDockingSite();
	if (!pFrameTest || !pFrameTest->IsKindOf(RUNTIME_CLASS(CXTPDockingPaneMiniWnd)))
		return FALSE;

	HWND hWndNext = ::GetWindow(pFrameBase->GetSafeHwnd(), GW_HWNDNEXT);

	while (hWndNext)
	{
		if (hWndNext == pFrameTest->GetSafeHwnd())
			return FALSE;

		hWndNext = ::GetWindow(hWndNext, GW_HWNDNEXT);
	}

	return TRUE;
}

void CXTPDockingPaneContext::FindContainer(CPoint pt)
{
	CXTPDockingPaneInfoList* pList = &m_pManager->GetPaneList();

	CXTPDockingPaneBase* pFloatingPane = NULL;

	POSITION pos = pList->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pNextPane = pList->GetNext(pos);

		CXTPDockingPaneBase* pPane = pNextPane->GetContainer();
		if (pPane == NULL)
			continue;

		ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);

		if (pPane->GetContainer() == 0
			|| pPane->GetContainer()->GetType() != xtpPaneTypeSplitterContainer)
			continue;

		if (!(::GetWindowLong(((CXTPDockingPaneTabbedContainer*)pPane)->GetSafeHwnd(), GWL_STYLE)
			  & WS_VISIBLE))
			continue;

		if (m_pPane->ContainPane(pPane))
			continue;

		CRect rcClient = pPane->GetPaneWindowRect();

		if (rcClient.PtInRect(pt) && pNextPane->IsFloating())
		{
			if (pFloatingPane == NULL || IsBehind(pFloatingPane, pPane))
			{
				pFloatingPane = pPane;
			}
		}
	}

	pos = pList->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pNextPane = pList->GetNext(pos);
		if (pNextPane->GetOptions() & xtpPaneNoDockable)
			continue;

		CXTPDockingPaneBase* pPane = pNextPane->GetContainer();
		if (pPane == NULL)
			continue;

		if (pFloatingPane != NULL && pFloatingPane != pPane)
			continue;

		ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);

		if (pPane->GetContainer() == 0
			|| (pPane->GetContainer()->GetType() != xtpPaneTypeSplitterContainer
				&& pPane->GetContainer()->GetType() != xtpPaneTypeSidePanel))
			continue;

		if (!(::GetWindowLong(((CXTPDockingPaneTabbedContainer*)pPane)->GetSafeHwnd(), GWL_STYLE)
			  & WS_VISIBLE))
			continue;

		CRect rcClient = m_rectContainer = pPane->GetPaneWindowRect();

		if (m_pPane->ContainPane(pPane))
			continue;

		if (((CXTPDockingPaneTabbedContainer*)pPane)->CanAttach(rcClient, pt))
		{
			if (IsAllowAttachTo(pPane))
			{
				m_pContainer   = pPane;
				m_bAttach	  = TRUE;
				m_pStickerPane = 0;
				m_rectStickerPane.SetRectEmpty();
				return;
			}
			else
			{
				rcClient = m_rectContainer;
			}
		}
		if (pPane->GetContainer()->GetType() == xtpPaneTypeSidePanel)
			continue;

		if (m_bUseDockingStickers)
		{
			if (rcClient.PtInRect(pt))
			{
				if (m_pStickerPane == NULL || IsBehind(m_pStickerPane, pPane))
				{
					m_rectStickerPane = m_rectContainer;
					m_pStickerPane	= pPane;
				}
			}
		}
		else
		{
			if (CanDock(rcClient, pt, pPane))
				return;
		}
	}

	if (pFloatingPane && m_bUseDockingStickers && m_pStickerPane == NULL
		&& m_rectStickerPane.IsRectEmpty())
	{
		CXTPDockingPaneBase* pPane = m_pManager->GetTopPane();
		if (pPane->GetPaneWindowRect().PtInRect(pt))
		{
			m_rectStickerPane = pPane->GetPaneWindowRect();
		}
	}

	if (pFloatingPane)
		return;

	if (m_pManager->IsSideDockingEnabled())
	{
		if (CanSideDock(m_pManager->GetClientPane(), m_rectDragFrame))
		{
			m_bSideDock = TRUE;
			return;
		}
	}

	if (m_bUseDockingStickers)
	{
		if (m_pStickerPane)
		{
			return;
		}

		if (m_rectStickerPane.IsRectEmpty())
		{
			CXTPDockingPaneBase* pPane = m_pManager->GetClientPane();
			if (pPane->GetPaneWindowRect().PtInRect(pt))
			{
				m_rectStickerPane = pPane->GetPaneWindowRect();
				m_pStickerPane	= pPane;
				return;
			}

			pPane = m_pManager->GetTopPane();
			if (pPane->GetPaneWindowRect().PtInRect(pt))
			{
				m_rectStickerPane = pPane->GetPaneWindowRect();
			}
		}
	}
	else
	{
		CXTPDockingPaneBase* pPane;

		pPane			= m_pManager->GetClientPane();
		m_rectContainer = pPane->GetPaneWindowRect();
		if (CanDock(pPane->GetPaneWindowRect(), pt, pPane))
			return;

		pPane			= m_pManager->GetTopPane();
		m_rectContainer = pPane->GetPaneWindowRect();
		if (CanDock(pPane->GetPaneWindowRect(), pt, pPane, FALSE))
			return;
	}
}

void CXTPDockingPaneContext::EnsureVisible(CRect& rectDragFrame)
{
	CRect rcWork = XTPMultiMonitor()->GetWorkArea(rectDragFrame);
	int nGapX	= XTP_DPI_X(10);
	int nGapY	= XTP_DPI_Y(10);

	if (rcWork.bottom - rectDragFrame.top < nGapY)
	{
		rectDragFrame.OffsetRect(0, rcWork.bottom - rectDragFrame.top - nGapY);
	}
	if (rectDragFrame.bottom - rcWork.top < nGapY)
	{
		rectDragFrame.OffsetRect(0, rcWork.top - rectDragFrame.bottom + nGapY);
	}
	if (rcWork.right - rectDragFrame.left < nGapX)
	{
		rectDragFrame.OffsetRect(rcWork.right - rectDragFrame.left - nGapX, 0);
	}
	if (rectDragFrame.right - rcWork.left < nGapX)
	{
		rectDragFrame.OffsetRect(rcWork.left - rectDragFrame.right + nGapX, 0);
	}
}

void CXTPDockingPaneContext::UpdateStickyFrame(CRect& rectDragFrame, CWnd* pHost)
{
	int nGap = m_pManager->m_nStickyGap;
	CXTPWindowRect rcWork(pHost);

	if (rectDragFrame.bottom < rcWork.top - nGap || rectDragFrame.top > rcWork.bottom + nGap)
		return;

	if (rectDragFrame.right < rcWork.left - nGap || rectDragFrame.left > rcWork.right + nGap)
		return;

	if (abs(rcWork.bottom - rectDragFrame.top) < nGap)
	{
		rectDragFrame.OffsetRect(0, rcWork.bottom - rectDragFrame.top);
	}
	if (abs(rectDragFrame.bottom - rcWork.top) < nGap)
	{
		rectDragFrame.OffsetRect(0, rcWork.top - rectDragFrame.bottom);
	}
	if (abs(rcWork.left - rectDragFrame.right) < nGap)
	{
		rectDragFrame.OffsetRect(rcWork.left - rectDragFrame.right, 0);
	}
	if (abs(rectDragFrame.left - rcWork.right) < nGap)
	{
		rectDragFrame.OffsetRect(rcWork.right - rectDragFrame.left, 0);
	}
	if (abs(rectDragFrame.left - rcWork.left) < nGap)
	{
		rectDragFrame.OffsetRect(rcWork.left - rectDragFrame.left, 0);
	}
	if (abs(rectDragFrame.right - rcWork.right) < nGap)
	{
		rectDragFrame.OffsetRect(rcWork.right - rectDragFrame.right, 0);
	}
	if (abs(rectDragFrame.bottom - rcWork.bottom) < nGap)
	{
		rectDragFrame.OffsetRect(0, rcWork.bottom - rectDragFrame.bottom);
	}
	if (abs(rectDragFrame.top - rcWork.top) < nGap)
	{
		rectDragFrame.OffsetRect(0, rcWork.top - rectDragFrame.top);
	}
}

void CXTPDockingPaneContext::UpdateStickyFrame(CRect& rectDragFrame)
{
	CRect rcWork = XTPMultiMonitor()->GetWorkArea(rectDragFrame);
	int nGap	 = m_pManager->m_nStickyGap;

	if (abs(rcWork.top - rectDragFrame.top) < nGap)
	{
		rectDragFrame.OffsetRect(0, rcWork.top - rectDragFrame.top);
	}
	if (abs(rectDragFrame.bottom - rcWork.bottom) < nGap)
	{
		rectDragFrame.OffsetRect(0, rcWork.bottom - rectDragFrame.bottom);
	}
	if (abs(rcWork.right - rectDragFrame.right) < nGap)
	{
		rectDragFrame.OffsetRect(rcWork.right - rectDragFrame.right, 0);
	}
	if (abs(rectDragFrame.left - rcWork.left) < nGap)
	{
		rectDragFrame.OffsetRect(rcWork.left - rectDragFrame.left, 0);
	}

	if ((m_pManager->GetSite()->GetStyle() & (WS_MAXIMIZE | WS_MINIMIZE)) == 0)
		UpdateStickyFrame(rectDragFrame, m_pManager->GetSite());

	CXTPDockingPaneBaseList* pList = &m_pManager->GetPaneStack();
	POSITION pos				   = pList->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pNextPane = pList->GetNext(pos);
		if (pNextPane->GetType() != xtpPaneTypeMiniWnd)
			continue;

		CXTPDockingPaneMiniWnd* pWnd = (CXTPDockingPaneMiniWnd*)pNextPane;
		if (pWnd->GetSafeHwnd() && pWnd->IsWindowVisible() && m_pPane->GetDockingSite() != pWnd)
		{
			UpdateStickyFrame(rectDragFrame, pWnd);
		}
	}
}

void CXTPDockingPaneContext::UpdateSizingStickyFrame(UINT nSide, CRect& rectDragFrame, CWnd* pHost)
{
	int nGap = m_pManager->m_nStickyGap;
	CXTPWindowRect rcWork(pHost);

	if (rectDragFrame.bottom < rcWork.top - nGap || rectDragFrame.top > rcWork.bottom + nGap)
		return;

	if (rectDragFrame.right < rcWork.left - nGap || rectDragFrame.left > rcWork.right + nGap)
		return;

	if (abs(rectDragFrame.top - rcWork.bottom) < nGap
		&& (nSide == WMSZ_TOP || nSide == WMSZ_TOPLEFT || nSide == WMSZ_TOPRIGHT))
	{
		rectDragFrame.top = rcWork.bottom;
	}
	if (abs(rectDragFrame.bottom - rcWork.top) < nGap
		&& (nSide == WMSZ_BOTTOM || nSide == WMSZ_BOTTOMLEFT || nSide == WMSZ_BOTTOMRIGHT))
	{
		rectDragFrame.bottom = rcWork.top;
	}
	if (abs(rectDragFrame.left - rcWork.left) < nGap
		&& (nSide == WMSZ_LEFT || nSide == WMSZ_TOPLEFT || nSide == WMSZ_BOTTOMLEFT))
	{
		rectDragFrame.left = rcWork.left;
	}
	if (abs(rectDragFrame.left - rcWork.right) < nGap
		&& (nSide == WMSZ_LEFT || nSide == WMSZ_TOPLEFT || nSide == WMSZ_BOTTOMLEFT))
	{
		rectDragFrame.left = rcWork.right;
	}
	if (abs(rectDragFrame.right - rcWork.left) < nGap
		&& (nSide == WMSZ_RIGHT || nSide == WMSZ_TOPRIGHT || nSide == WMSZ_BOTTOMRIGHT))
	{
		rectDragFrame.right = rcWork.left;
	}
	if (abs(rectDragFrame.right - rcWork.right) < nGap
		&& (nSide == WMSZ_RIGHT || nSide == WMSZ_TOPRIGHT || nSide == WMSZ_BOTTOMRIGHT))
	{
		rectDragFrame.right = rcWork.right;
	}
}

void CXTPDockingPaneContext::OnSizingFloatingFrame(CXTPDockingPaneMiniWnd* pMiniWnd, UINT nSide,
												   LPRECT lpRect)
{
	if (!m_pManager->IsStickyFloatingFrames())
		return;

	CRect rectDragFrame(lpRect);

	if ((m_pManager->GetSite()->GetStyle() & (WS_MAXIMIZE | WS_MINIMIZE)) == 0)
		UpdateSizingStickyFrame(nSide, rectDragFrame, m_pManager->GetSite());

	CXTPDockingPaneBaseList* pList = &m_pManager->GetPaneStack();
	POSITION pos				   = pList->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pNextPane = pList->GetNext(pos);
		if (pNextPane->GetType() != xtpPaneTypeMiniWnd)
			continue;

		CXTPDockingPaneMiniWnd* pWnd = (CXTPDockingPaneMiniWnd*)pNextPane;
		if (pWnd->GetSafeHwnd() && pWnd->IsWindowVisible() && pMiniWnd != pWnd)
		{
			UpdateSizingStickyFrame(nSide, rectDragFrame, pWnd);
		}
	}

	*lpRect = rectDragFrame;
}

void CXTPDockingPaneContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	m_rectDragFrameScreen.OffsetRect(ptOffset);

	if (CRect().IntersectRect(m_rectDragFrameScreen,
							  XTPMultiMonitor()->GetWorkArea(m_rectDragFrame)))
	{
		m_rectDragFrame = m_rectDragFrameScreen;
	}

	EnsureVisible(m_rectDragFrame);

	if (!m_rectDragFrame.IsRectEmpty() && m_pManager->m_bStickyFloatingFrames)
	{
		UpdateStickyFrame(m_rectDragFrame);
	}

	m_pContainer = 0;
	m_bAttach	= FALSE;
	m_rectStickerPane.SetRectEmpty();
	m_pStickerPane = NULL;
	m_bSideDock	= FALSE;

	if (GetKeyState(VK_CONTROL) >= 0 && m_bDockable)
	{
		FindContainer(pt);
	}

	m_ptLast = pt;

	if (m_bUseDockingStickers)
	{
		UpdateDockingStickers();

		BOOL bFound  = FALSE;
		POSITION pos = m_lstStickers.GetTailPosition();
		while (pos)
		{
			CXTPDockingPaneContextStickerWnd* pSticker = m_lstStickers.GetPrev(pos);
			XTPDockingPaneStickerType selectedSticker  = xtpPaneStickerNone;

			if (!bFound)
			{
				XTPDockingPaneStickerType ht = pSticker->HitTest(pt);
				if (ht != xtpPaneStickerNone)
				{
					if ((pSticker->m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient)
					{
						m_pContainer	= m_pStickerPane;
						m_rectContainer = m_rectStickerPane;
					}
					else
					{
						m_pContainer	= m_pManager->GetTopPane();
						m_rectContainer = m_pContainer->GetPaneWindowRect();
					}

					switch (ht)
					{
						case xtpPaneStickerTop:
							m_rectContainer.bottom = m_rectContainer.CenterPoint().y;
							m_containDirection	 = xtpPaneDockTop;
							break;
						case xtpPaneStickerBottom:
							m_rectContainer.top = m_rectContainer.CenterPoint().y;
							m_containDirection  = xtpPaneDockBottom;
							break;
						case xtpPaneStickerLeft:
							m_rectContainer.right = m_rectContainer.CenterPoint().x;
							m_containDirection	= xtpPaneDockLeft;
							break;
						case xtpPaneStickerRight:
							m_rectContainer.left = m_rectContainer.CenterPoint().x;
							m_containDirection   = xtpPaneDockRight;
							break;
						case xtpPaneStickerCenter: m_bAttach = TRUE; break;
					}

					BOOL bAllow = TRUE;

					if ((ht != xtpPaneStickerCenter)
						&& !IsAllowDockingTo(m_pContainer, m_containDirection))
					{
						m_pContainer = NULL;
						bAllow		 = FALSE;
					}

					if ((ht == xtpPaneStickerCenter) && !IsAllowAttachTo(m_pContainer))
					{
						m_pContainer = NULL;
						m_bAttach	= FALSE;
						bAllow		 = FALSE;
					}

					if (bAllow)
					{
						if (ht != xtpPaneStickerCenter)
							m_rectContainer = m_pManager->_CalculateResultDockingRect(
								m_pPane, m_containDirection, m_pContainer);

						selectedSticker = ht;
					}

					bFound = TRUE;
				}
			}
			if (pSticker->m_selectedSticker != selectedSticker)
			{
				pSticker->m_selectedSticker = selectedSticker;
				pSticker->Invalidate(FALSE);
			}
		}
	}

	DrawFocusRect();
}

void CXTPDockingPaneContext::Track()
{
#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
	CXTPTabClientWnd* pTarget = NULL;
#	endif
#endif

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return;

	HWND hwndCapture = m_pManager->GetSite()->GetSafeHwnd();
	ASSERT(hwndCapture);
	if (!hwndCapture)
		return;
	// set capture to the window which received this message
	::SetCapture(hwndCapture);
	ASSERT(hwndCapture == ::GetCapture());

	BOOL bAccept = FALSE;
	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == hwndCapture)
	{
		ProcessPendingPaintMessages();

		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
		{
#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
			if (XTP_SAFE_GET2(m_pManager, GetTabClient(), IsTearOffEnabled(), NULL))
			{
				CWnd* pTargetWnd = CWnd::WindowFromPoint(msg.pt);
				CXTPDockingPaneContextAlphaWnd* pAlphaWnd =
					DYNAMIC_DOWNCAST(CXTPDockingPaneContextAlphaWnd, pTargetWnd);
				if (pAlphaWnd)
				{
					pTarget = XTP_SAFE_GET2(m_pManager, GetTabClient(), FindTargetTabClient(msg.pt),
											NULL);
				}
				else
				{
					pTarget			   = DYNAMIC_DOWNCAST(CXTPTabClientWnd, pTargetWnd);
					CView* pTargetView = DYNAMIC_DOWNCAST(CView, pTargetWnd);
					if (pTargetView)
					{
						CWnd* pWnd = XTP_SAFE_GET2(pTargetView, GetParent(), GetParent(), NULL);
						pTarget	= DYNAMIC_DOWNCAST(CXTPTabClientWnd, pWnd);
					}
				}
			}
#	endif
#endif
			bAccept = TRUE;
			break;
		}
		else if (msg.message == WM_MOUSEMOVE)
		{
			CSize sz = m_ptSticky - CPoint(msg.pt);
			if (abs(sz.cx) > XTP_DPI_X(4) || abs(sz.cy) > XTP_DPI_Y(4))
			{
				Move(msg.pt);
				m_ptSticky = CPoint(0, 0);
			}
		}
		else if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			if (m_bDragKeyboard && msg.message == WM_KEYDOWN)
			{
				if (msg.wParam == VK_RETURN)
				{
					bAccept = TRUE;
					break;
				}
				if (msg.wParam >= VK_LEFT && msg.wParam <= VK_DOWN)
				{
					CPoint pt(m_ptLast);
					if (msg.wParam == VK_LEFT)
						pt.x -= XTP_DPI_X(10);
					else if (msg.wParam == VK_RIGHT)
						pt.x += XTP_DPI_X(10);
					else if (msg.wParam == VK_UP)
						pt.y -= XTP_DPI_Y(10);
					else if (msg.wParam == VK_DOWN)
						pt.y += XTP_DPI_Y(10);

					AdjustCursor(pt);
					SetCursorPos(pt.x, pt.y);
					Move(pt);
					m_ptSticky = CPoint(0, 0);
				}
			}
			if (msg.wParam == VK_CONTROL)
			{
				Move(m_ptLast);
				m_ptSticky = CPoint(0, 0);
			}
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
		{
			DispatchMessage(&msg);
		}
	}

	CancelLoop();

	if (bAccept && m_ptSticky == CPoint(0, 0))
	{
		XTPDockingPaneAction actionNotify = xtpPaneActionFloated;

		CXTPDockingPaneBaseList lst;
		m_pPane->FindPane(xtpPaneTypeDockingPane, &lst);

		if (m_bAttach)
		{
#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
			BOOL bTearOffEnabled = XTP_SAFE_GET2(m_pManager, GetTabClient(), IsTearOffEnabled(),
												 NULL);
			if (bTearOffEnabled && m_pContainer->GetType() == xtpPaneTypeClient)
			{
				m_pManager->AttachDockingPaneMiniWndToTabClientWnd(
					(CXTPDockingPaneMiniWnd*)m_pPane->GetContainer(), pTarget);
			}
			else
			{
#	endif
#endif
				ASSERT(m_pContainer->GetType() == xtpPaneTypeTabbedContainer);
				m_pManager->AttachPane(m_pPane, m_pContainer);
				actionNotify = xtpPaneActionAttached;
#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
			}
#	endif
#endif
		}
		else if (m_bSideDock && m_pContainer && m_pContainer == m_pManager->GetClientPane())
		{
			CRect rectSide(m_rectContainer);
			rectSide.OffsetRect(-m_pContainer->GetPaneWindowRect().TopLeft());

			if (m_pPane->GetContainer()
				&& m_pPane->GetContainer()->GetType() == xtpPaneTypeSidePanel)
				((CXTPDockingPaneSidePanel*)m_pPane->GetContainer())
					->MovePanel(m_containDirection, rectSide);
			else
				m_pManager->DockSidePane(m_pPane, m_containDirection, rectSide);

			actionNotify = xtpPaneActionDocked;
		}
		else if (m_pContainer)
		{
			m_pManager->DockPane(m_pPane, m_pManager->GetRTLDirection(m_containDirection),
								 m_pContainer);
			m_pManager->EnsureVisible(m_pContainer);
			actionNotify = xtpPaneActionDocked;
		}
		else if (m_bFloatable)
		{
			CRect rcWork = XTPMultiMonitor()->GetWorkArea(m_rectDragFrame);

			if (m_rectDragFrame.top < rcWork.top)
				m_rectDragFrame.OffsetRect(0, rcWork.top - m_rectDragFrame.top);

			if (m_pPane->GetType() == xtpPaneTypeSplitterContainer)
			{
				m_pPane->GetDockingSite()->MoveWindow(m_rectDragFrame);
#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
				if (!m_pStickerPane)
					m_pManager->AttachDockingPaneMiniWndToTabClientWnd(
						DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, m_pPane->GetDockingSite()),
						pTarget);
#	endif
#endif
			}
			else
				m_pManager->FloatPane(m_pPane, m_rectDragFrame);
		}
		else
		{
			bAccept = FALSE;
		}

		if (bAccept)
		{
			POSITION pos = lst.GetHeadPosition();
			while (pos)
			{
				CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);
				m_pManager->NotifyAction(actionNotify, pPane, m_pContainer, m_containDirection);
			}
		}
	}
}

void CXTPDockingPaneContext::DestroyDockingStickers()
{
	while (!m_lstStickers.IsEmpty())
	{
		CWnd* pWnd = m_lstStickers.RemoveHead();
		pWnd->DestroyWindow();
		delete pWnd;
	}
}

void CXTPDockingPaneContext::IncludeRgnPart(CRgn* pRgn, int x1, int y, int x2)
{
	if (x1 < x2)
	{
		CRgn rgnExclude;
		rgnExclude.CreateRectRgn(x1, y, x2, y + 1);
		pRgn->CombineRgn(pRgn, &rgnExclude, RGN_OR);
		rgnExclude.DeleteObject();
	}
}

void CXTPDockingPaneContext::RegionFromBitmap(CRgn* pRgn, CDC* pDC, CRect rc)
{
	CSize sz = rc.Size();

	pRgn->CreateRectRgn(0, 0, 0, 0);

	for (int y = 0; y < sz.cy; y++)
	{
		int nStart = 0, x = 0;
		BOOL bTransparent = TRUE;

		while (x < sz.cx)
		{
			BOOL bTransparentPixel = pDC->GetPixel(x, y) == 0;

			if (bTransparent && !bTransparentPixel)
			{
				nStart		 = x;
				bTransparent = FALSE;
			}
			else if (!bTransparent && bTransparentPixel)
			{
				IncludeRgnPart(pRgn, nStart, y, x);
				bTransparent = TRUE;
			}
			x++;
		}
		if (!bTransparent)
		{
			IncludeRgnPart(pRgn, nStart, y, x);
		}
	}
}

CXTPDockingPaneContextStickerWnd*
	CXTPDockingPaneContext::CreateNewSticker(CRect rc, XTPDockingPaneStickerType typeSticker)
{
	if (typeSticker == xtpPaneStickerNone || typeSticker == xtpPaneStickerClient)
		return NULL;

	CXTPDockingPaneContextStickerWnd* pWnd = new CXTPDockingPaneContextStickerWnd(this);
	m_lstStickers.AddTail(pWnd);

	pWnd->CreateEx(WS_EX_TOOLWINDOW,
				   AfxRegisterWndClass(NULL, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 0,
				   WS_POPUP, CRect(0, 0, 0, 0), m_pManager->GetSite(), 0);

	pWnd->m_typeSticker = typeSticker;
	pWnd->Init();

	CRgn* pRgn = NULL;
	if (!m_rgnStickers.Lookup(typeSticker, pRgn))
	{
		pRgn = new CRgn;
		m_rgnStickers.SetAt(typeSticker, pRgn);
	}
	ASSERT(pRgn != NULL);
	if (!pRgn)
		return NULL;

	if (!pRgn->GetSafeHandle())
	{
		CClientDC dcClient(pWnd);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(&dcClient, rc.Width(), rc.Height());

		if (bmp.GetSafeHandle())
		{
			CXTPCompatibleDC dc(&dcClient, &bmp);
			dc.FillSolidRect(rc, RGB(0, 0, 0));
			pWnd->OnDraw(&dc);

			RegionFromBitmap(pRgn, &dc, rc);
		}
	}

	HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
	::CombineRgn(hRgn, (HRGN)pRgn->GetSafeHandle(), NULL, RGN_COPY);

	pWnd->SetWindowRgn(hRgn, FALSE);
	pWnd->SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(),
					   SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	return pWnd;
}

CSize CXTPDockingPaneContext::GetStickerSize(XTPDockingPaneStickerType type) const
{
	int cx, cy, sz;

	cx = XTP_DPI_X(40);
	cy = XTP_DPI_Y(40);
	sz = XTP_DPI_X(120);

	switch (type)
	{
		case xtpPaneStickerTop: return CSize(cy, cx);
		case xtpPaneStickerLeft: return CSize(cx, cy);
		case xtpPaneStickerBottom: return CSize(cy, cx);
		case xtpPaneStickerRight: return CSize(cx, cy);
	}

	return CSize(sz, sz);
}

XTPDockingContextStickerStyle CXTPDockingPaneContext::GetStickerStyle() const
{
	return m_pManager->GetDockingContextStickerStyle();
}

void CXTPDockingPaneContext::UpdateDockingStickers()
{
	if (m_rectStickerPane.IsRectEmpty())
	{
		m_pLastStickerPane = NULL;
		DestroyDockingStickers();
		return;
	}

	CSize sz(GetStickerSize(xtpPaneStickerClient));
	CRect rc(m_rectStickerPane.CenterPoint(), sz);
	rc.OffsetRect(-sz.cx / 2, -sz.cy / 2);

	if (m_pLastStickerPane != m_pStickerPane || m_lstStickers.IsEmpty())
	{
		DestroyDockingStickers();

		if (m_pStickerPane)
		{
			UINT allowStickers = xtpPaneStickerNone;
			if (IsAllowDockingTo(m_pStickerPane, xtpPaneDockLeft))
				allowStickers = allowStickers + xtpPaneStickerLeft;
			if (IsAllowDockingTo(m_pStickerPane, xtpPaneDockRight))
				allowStickers = allowStickers + xtpPaneStickerRight;
			if (IsAllowDockingTo(m_pStickerPane, xtpPaneDockBottom))
				allowStickers = allowStickers + xtpPaneStickerBottom;
			if (IsAllowDockingTo(m_pStickerPane, xtpPaneDockTop))
				allowStickers = allowStickers + xtpPaneStickerTop;

			CreateNewSticker(rc,
							 (m_pStickerPane == m_pManager->GetClientPane()
							  || !IsAllowAttachTo(m_pStickerPane))
#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
									 && !IsAllowAttachToTabClient(m_pStickerPane)
#	endif
#endif
								 ? XTPDockingPaneStickerType(xtpPaneStickerClient + allowStickers)
								 : XTPDockingPaneStickerType(xtpPaneStickerClient + allowStickers
															 + xtpPaneStickerCenter));
		}

		CXTPDockingPaneBase* pTopPane = m_pManager->GetTopPane();
		CRect rcWindow				  = pTopPane->GetPaneWindowRect();

		if (IsAllowDockingTo(pTopPane, xtpPaneDockTop))
		{
			CreateNewSticker(CRect(CPoint(rcWindow.CenterPoint().x
											  - GetStickerSize(xtpPaneStickerTop).cx / 2,
										  rcWindow.top + XTP_DPI_Y(16)),
								   GetStickerSize(xtpPaneStickerTop)),
							 xtpPaneStickerTop);
		}
		if (IsAllowDockingTo(pTopPane, xtpPaneDockLeft))
		{
			CreateNewSticker(CRect(CPoint(rcWindow.left + XTP_DPI_X(16),
										  rcWindow.CenterPoint().y
											  - GetStickerSize(xtpPaneStickerLeft).cy / 2),
								   GetStickerSize(xtpPaneStickerLeft)),
							 xtpPaneStickerLeft);
		}
		if (IsAllowDockingTo(pTopPane, xtpPaneDockBottom))
		{
			CreateNewSticker(CRect(CPoint(rcWindow.CenterPoint().x
											  - GetStickerSize(xtpPaneStickerBottom).cx / 2,
										  rcWindow.bottom - XTP_DPI_Y(16)
											  - GetStickerSize(xtpPaneStickerBottom).cy),
								   GetStickerSize(xtpPaneStickerBottom)),
							 xtpPaneStickerBottom);
		}
		if (IsAllowDockingTo(pTopPane, xtpPaneDockRight))
		{
			CreateNewSticker(CRect(CPoint(rcWindow.right - GetStickerSize(xtpPaneStickerRight).cx
											  - XTP_DPI_X(16),
										  rcWindow.CenterPoint().y
											  - GetStickerSize(xtpPaneStickerRight).cy / 2),
								   GetStickerSize(xtpPaneStickerRight)),
							 xtpPaneStickerRight);
		}

		m_pLastStickerPane = m_pStickerPane;
	}

	ASSERT(m_lstStickers.GetCount() < 6);
}

void CXTPDockingPaneContext::CreateContextWindow(CXTPDockingPaneContextAlphaWnd* pWnd)
{
	ASSERT(m_bUseAlphaContext);

	if (pWnd->GetSafeHwnd())
		return;

	pWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
				   AfxRegisterWndClass(NULL, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 0,
				   WS_POPUP, CRect(0, 0, 0, 0), m_pManager->GetSite(), 0);

	if (m_pfnSetLayeredWindowAttributes)
	{
		((PFNSETLAYEREDWINDOWATTRIBUTES)m_pfnSetLayeredWindowAttributes)(pWnd->m_hWnd, 0, 100,
																		 LWA_ALPHA);
	}
}

void CXTPDockingPaneContext::AcquireDesktopDC()
{
	ReleaseDesktopDC();

	CWnd* pWnd  = CWnd::GetDesktopWindow();
	UINT nFlags = UINT(pWnd->LockWindowUpdate() ? DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE
												: DCX_WINDOW | DCX_CACHE);
	m_pDC		= pWnd->GetDCEx(NULL, nFlags);

	ASSERT(NULL != m_pDC);
}

void CXTPDockingPaneContext::ReleaseDesktopDC()
{
	if (NULL != m_pDC)
	{
		CWnd* pWnd = CWnd::GetDesktopWindow();
		pWnd->UnlockWindowUpdate();
		pWnd->ReleaseDC(m_pDC);
		m_pDC = NULL;
	}
}

#ifndef _XTP_ACTIVEX
#	ifdef _XTP_INCLUDE_COMMANDBARS
BOOL CXTPDockingPaneContext::IsAllowAttachToTabClient(CXTPDockingPaneBase* pPane)
{
	if (!pPane)
		return FALSE;

	if (!m_pManager->GetTabClient())
		return FALSE;

	if (!m_pManager->GetTabClient()->IsTearOffEnabled())
		return FALSE;

	if (m_pManager->GetTabClient()->GetWorkspaceCount() > 1)
		return FALSE;

	if (pPane->GetType() != xtpPaneTypeClient)
		return FALSE;

	if (m_pManager->GetSite() != m_pManager->GetTabClient()->GetParentFrame())
		return FALSE;

	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, m_pManager->GetActivePane()->GetChild());
	if (!pFrame)
		return FALSE;

	return XTP_SAFE_GET2(m_pManager, GetTabClient(), FindTargetTabClient(m_ptLast), NULL) ? TRUE
																						  : FALSE;
}
#	endif
#endif
