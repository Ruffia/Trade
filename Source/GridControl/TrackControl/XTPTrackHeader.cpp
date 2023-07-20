// XTPTrackHeader.cpp : implementation of the CXTPTrackHeader class.
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
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridPaintManager.h"

#include "GridControl/TrackControl/XTPTrackHeader.h"
#include "GridControl/TrackControl/XTPTrackControl.h"
#include "GridControl/TrackControl/XTPTrackPaintManager.h"
#include "GridControl/TrackControl/XTPTrackUndoManager.h"
#include "GridControl/TrackControl/XTPTrackBlock.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTrackHeader

CXTPTrackHeader::CXTPTrackHeader(CXTPGridControl* pControl, CXTPGridColumns* pColumns)
	: CXTPGridHeader(pControl, pColumns)
{
}

CXTPTrackHeader::~CXTPTrackHeader()
{
}

void CXTPTrackHeader::Draw(CDC* pDC, CRect rcHeader, int nLeftOffset)
{
	CXTPGridHeader::Draw(pDC, rcHeader, nLeftOffset);

	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	pTrackControl->GetTrackPaintManager()->DrawTrackHeader(pDC);
}

void CXTPTrackHeader::OnMoveScrollBar(CPoint pt, BOOL bResize)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	int fromPosition = pControl->TrackToPosition(m_ptStartDrag.x);
	int toPosition   = pControl->TrackToPosition(pt.x);

	int delta = toPosition - fromPosition;
	if (pControl->IsLayoutRTL())
		delta = -delta;

	int nWorkAreaMin = m_nOldWorkAreaMin + (bResize != 2 ? delta : 0);
	int nWorkAreaMax = m_nOldWorkAreaMax + (bResize != 1 ? delta : 0);

	if (nWorkAreaMin < pControl->GetTimeLineMin())
	{
		if (bResize != 1)
			nWorkAreaMax = pControl->GetTimeLineMin() + nWorkAreaMax - nWorkAreaMin;
		nWorkAreaMin = pControl->GetTimeLineMin();
	}

	if (nWorkAreaMax > pControl->GetTimeLineMax())
	{
		if (bResize != 2)
			nWorkAreaMin = pControl->GetTimeLineMax() - (nWorkAreaMax - nWorkAreaMin);
		nWorkAreaMax = pControl->GetTimeLineMax();
	}

	if (nWorkAreaMax < nWorkAreaMin)
		if (bResize == 1)
			nWorkAreaMin = nWorkAreaMax;
		else
			nWorkAreaMax = nWorkAreaMin;

	pControl->SetWorkArea(nWorkAreaMin, nWorkAreaMax);
	pControl->RedrawControl();
}

void CXTPTrackHeader::StartDragScrollBar(BOOL bResize)
{
	// set capture to the window which received this message
	m_pControl->SetCapture();
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag	 = pt;
	m_nOldWorkAreaMin = pControl->GetWorkAreaMin();
	m_nOldWorkAreaMax = pControl->GetWorkAreaMax();

	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorResize);
	::SetCursor(((CXTPTrackControl*)pControl)->m_hResizeCursor);

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pControl)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			OnMoveScrollBar(pt, bResize);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);
	}

	ReleaseCapture();
	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorNothing);
}

void CXTPTrackHeader::OnMoveSlider(CPoint pt, BOOL bResize)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea   = pTrackControl->GetElementRect(xtpGridElementRectHeaderArea);

	CXTPGridColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	int nLeftOffset				 = pTrackColumn->GetRect().left;
	int nRightOffset			 = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset,
					   rcTimeLineArea.top + XTP_DPI_Y(10));

	int fromPosition = MulDiv(m_ptStartDrag.x,
							  (pControl->GetTimeLineMax() - pControl->GetTimeLineMin()),
							  rcSliderArea.Width());
	int toPosition   = MulDiv(pt.x, (pControl->GetTimeLineMax() - pControl->GetTimeLineMin()),
							  rcSliderArea.Width());

	int delta = toPosition - fromPosition;
	if (pControl->IsLayoutRTL())
		delta = -delta;

	int nViewPortMin = m_nOldViewPortMin + (bResize != 2 ? delta : 0);
	int nViewPortMax = m_nOldViewPortMax + (bResize != 1 ? delta : 0);

	if (nViewPortMin < pControl->GetTimeLineMin())
	{
		if (bResize != 1)
			nViewPortMax = pControl->GetTimeLineMin() + nViewPortMax - nViewPortMin;
		nViewPortMin = pControl->GetTimeLineMin();
	}

	if (nViewPortMax > pControl->GetTimeLineMax())
	{
		if (bResize != 2)
			nViewPortMin = pControl->GetTimeLineMax() - (nViewPortMax - nViewPortMin);
		nViewPortMax = pControl->GetTimeLineMax();
	}

	if (nViewPortMax <= nViewPortMin)
	{
		if (bResize == 1)
			nViewPortMin = nViewPortMax - 1;
		else
			nViewPortMax = nViewPortMin + 1;
	}

	pControl->SetViewPort(nViewPortMin, nViewPortMax);
}

void CXTPTrackHeader::StartDragSlider(BOOL bResize)
{
	// set capture to the window which received this message
	m_pControl->SetCapture();
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag	 = pt;
	m_nOldViewPortMin = pControl->GetViewPortMin();
	m_nOldViewPortMax = pControl->GetViewPortMax();

	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorResize);
	::SetCursor(((CXTPTrackControl*)pControl)->m_hResizeCursor);

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pControl)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			OnMoveSlider(pt, bResize);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);
	}

	ReleaseCapture();
	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorNothing);
}

void CXTPTrackHeader::OnMoveMarker(CPoint pt, int nMarkerIndex)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	int fromPosition = pControl->TrackToPosition(m_ptStartDrag.x);
	int toPosition   = pControl->TrackToPosition(pt.x);

	int delta = toPosition - fromPosition;
	if (pControl->IsLayoutRTL())
		delta = -delta;

	if (nMarkerIndex == -1)
	{
		int nPosition = max(pControl->GetTimeLineMin(),
							min(pControl->GetTimeLineMax(), m_nOldMarkerPos + delta));
		pControl->SetTimeLinePosition(nPosition);

		pControl->SendMessageToParent(XTP_NM_TRACK_TIMELINECHANGED);
	}
	else
	{
		CXTPTrackMarker* pMarker = pControl->GetMarkers()->GetAt(nMarkerIndex);

		int nPosition = max(pControl->GetTimeLineMin(),
							min(pControl->GetTimeLineMax(), m_nOldMarkerPos + delta));
		pMarker->SetPosition(nPosition);

		pControl->SendMessageToParent(XTP_NM_TRACK_MARKERCHANGED, 0, pMarker);
	}

	pControl->RedrawControl();
}

void CXTPTrackHeader::StartDragMarker(int nMarkerIndex)
{
	// set capture to the window which received this message
	m_pControl->SetCapture();
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	pControl->GetUndoManager()->StartGroup();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag   = pt;
	m_nOldMarkerPos = nMarkerIndex == -1
						  ? pControl->GetTimeLinePosition()
						  : pControl->GetMarkers()->GetAt(nMarkerIndex)->GetPosition();

	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorDrag);
	::SetCursor(((CXTPTrackControl*)pControl)->m_hMoveCursor);

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pControl)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			OnMoveMarker(pt, nMarkerIndex);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);
	}

	pControl->GetUndoManager()->EndGroup();

	ReleaseCapture();
	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorNothing);
}

void CXTPTrackHeader::OnLButtonDown(CPoint point)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea   = pTrackControl->GetElementRect(xtpGridElementRectHeaderArea);

	CXTPGridColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	if (!pTrackColumn || !pTrackColumn->IsVisible())
	{
		CXTPGridHeader::OnLButtonDown(point);
		return;
	}

	int nLeftOffset  = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset,
					   rcTimeLineArea.top + XTP_DPI_Y(10));
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset,
					   rcHeaderArea.bottom - XTP_DPI_Y(3));

	int nMarker = pTrackControl->GetMarkers()->HitTest(point);
	if (nMarker != -1)
	{
		StartDragMarker(nMarker);
		return;
	}

	if (pTrackControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMin());
		int nWorkAreaMax = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMax());

		CRect rcWorkAreaScrollBar(max(nLeftOffset, nWorkAreaMin), rcColumnArea.top,
								  min(nRightOffset, nWorkAreaMax), rcColumnArea.bottom);

		rcWorkAreaScrollBar.InflateRect(XTP_DPI_X(7), 0);

		if (rcWorkAreaScrollBar.PtInRect(point))
		{
			int nResize = 0;
			if (point.x < rcWorkAreaScrollBar.left + XTP_DPI_X(7))
				nResize = 1;
			else if (point.x > rcWorkAreaScrollBar.right - XTP_DPI_X(7))
				nResize = 2;

			StartDragScrollBar(nResize);

			return;
		}
	}

	rcSliderArea.DeflateRect(XTP_DPI_X(7), 0);

	int nZoomAreaMin = rcSliderArea.left
					   + MulDiv(pTrackControl->GetViewPortMin() - pTrackControl->GetTimeLineMin(),
								rcSliderArea.Width(),
								pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());
	int nZoomAreaMax = rcSliderArea.left
					   + MulDiv(pTrackControl->GetViewPortMax() - pTrackControl->GetTimeLineMin(),
								rcSliderArea.Width(),
								pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());

	CRect rcZoomSlider(nZoomAreaMin, rcSliderArea.top, nZoomAreaMax, rcSliderArea.bottom);
	rcZoomSlider.InflateRect(XTP_DPI_X(7), 0);

	if (rcZoomSlider.PtInRect(point))
	{
		int nResize = 0;
		if (point.x < rcZoomSlider.left + XTP_DPI_X(7))
			nResize = 1;
		else if (point.x > rcZoomSlider.right - XTP_DPI_X(7))
			nResize = 2;

		StartDragSlider(nResize);
		return;
	}

	CRect rcRullerArea(rcTimeArea.left, rcTimeArea.top, rcTimeArea.right, rcColumnArea.top);
	if (rcRullerArea.PtInRect(point))
	{
		pTrackControl->SetTimeLinePosition(pTrackControl->TrackToPosition(point.x));

		StartDragMarker(-1);
		return;
	}

	if (rcColumnArea.PtInRect(point))
		return;

	CXTPGridHeader::OnLButtonDown(point);
}

void CXTPTrackHeader::OnMouseMove(UINT nFlags, CPoint point)
{
	if (::GetCapture() != NULL)
	{
		CXTPGridHeader::OnMouseMove(nFlags, point);
		return;
	}
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CXTPGridColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	if (!pTrackColumn || !pTrackColumn->IsVisible())
	{
		CXTPGridHeader::OnMouseMove(nFlags, point);
		return;
	}
	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea   = pTrackControl->GetElementRect(xtpGridElementRectHeaderArea);

	int nLeftOffset  = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset,
					   rcTimeLineArea.top + XTP_DPI_Y(10));
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset,
					   rcHeaderArea.bottom - XTP_DPI_Y(3));

	int nMarker = pTrackControl->GetMarkers()->HitTest(point);
	if (nMarker != -1)
	{
		((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorDrag);
		::SetCursor(((CXTPTrackControl*)GetControl())->m_hMoveCursor);
		return;
	}

	if (pTrackControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMin());
		int nWorkAreaMax = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMax());

		CRect rcWorkAreaScrollBar(max(nLeftOffset, nWorkAreaMin), rcColumnArea.top,
								  min(nRightOffset, nWorkAreaMax), rcColumnArea.bottom);

		rcWorkAreaScrollBar.InflateRect(XTP_DPI_X(7), 0);

		if (rcWorkAreaScrollBar.PtInRect(point))
		{
			((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorResize);
			::SetCursor(((CXTPTrackControl*)GetControl())->m_hResizeCursor);
			return;
		}
	}

	int nZoomAreaMin = rcSliderArea.left
					   + MulDiv(pTrackControl->GetViewPortMin() - pTrackControl->GetTimeLineMin(),
								rcSliderArea.Width(),
								pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());
	int nZoomAreaMax = rcSliderArea.left
					   + MulDiv(pTrackControl->GetViewPortMax() - pTrackControl->GetTimeLineMin(),
								rcSliderArea.Width(),
								pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());

	CRect rcZoomSlider(nZoomAreaMin, rcSliderArea.top, nZoomAreaMax, rcSliderArea.bottom);
	rcZoomSlider.InflateRect(XTP_DPI_X(7), 0);

	if (rcZoomSlider.PtInRect(point))
	{
		((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorResize);
		::SetCursor(((CXTPTrackControl*)GetControl())->m_hResizeCursor);
		return;
	}

	((CXTPTrackControl*)GetControl())->SetMouseCursorMode(xtpTrackCursorNothing);

	CXTPGridHeader::OnMouseMove(nFlags, point);
}
