// XTPCalendarMonthViewEvent.cpp: implementation of the CXTPCalendarMonthViewEvent class.
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
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarMonthViewEvent.h"
#include "Calendar/XTPCalendarMonthViewDay.h"
#include "Calendar/XTPCalendarMonthView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarMonthViewEvent, CXTPCalendarViewEvent)

CXTPCalendarMonthViewEvent::CXTPCalendarMonthViewEvent(CXTPCalendarEvent* pEvent,
													   CXTPCalendarMonthViewGroup* pViewGroup)
	: TBase(pEvent, pViewGroup)
{
	ASSERT(pEvent);
}

CXTPCalendarMonthViewEvent::~CXTPCalendarMonthViewEvent()
{
}

void CXTPCalendarMonthViewEvent::Draw(CDC* pDC)
{
	if (m_nMultiDayEventFlags & xtpCalendarMultiDaySlave)
	{
		return; // was drawn in the Master
	}

	ASSERT((m_nMultiDayEventFlags & xtpCalendarMultiDayMaster)
		   || m_nMultiDayEventFlags == xtpCalendarMultiDayNoMultiDay);

	//-----------------------------------------------------------------------
	CXTPCalendarPaintManager::CMonthViewEventPart* pMEPart =
		XTP_SAFE_GET2(GetView(), GetPaintManager(), GetMonthViewEventPart(), NULL);

	ASSERT(pMEPart);
	XTP_SAFE_CALL1(pMEPart, OnDrawEvent(pDC, this));
}

void CXTPCalendarMonthViewEvent::AdjustLayout(CDC* pDC, const CRect& rcEventMax,
											  int nEventPlaceNumber)
{
	if (!GetCalendarControl() || !GetEvent() || !pDC || !GetView())
	{
		ASSERT(FALSE);
		return;
	}

	TBase::AdjustLayout(pDC, rcEventMax, nEventPlaceNumber);

	m_nMultiDayEventFlags = 0;

	//---------------------------------------------------
	CString strLoc		 = GetItemTextLocation();
	CString strEventText = GetItemTextSubject();
	if (strLoc.GetLength() > 0)
	{
		strEventText += _T(" (") + strLoc + _T(")");
	}

	m_szText = XTP_SAFE_GET3(GetView(), GetPaintManager(), GetDayViewEventPart(),
							 GetTextExtent(pDC, strEventText), CSize(0));
	//---------------------------------------------------

	if (IsMultidayEvent())
	{
		// calc icons rect
		AdjustMultidayEvent(rcEventMax, nEventPlaceNumber);
	}
	else
	{
		m_nMultiDayEventFlags = xtpCalendarMultiDayNoMultiDay;

		int nEventHeight = XTP_SAFE_GET1(GetView(), GetRowHeight(), 0);

		m_rcEvent.top	= rcEventMax.top + nEventPlaceNumber * nEventHeight + XTP_DPI_Y(1);
		m_rcEvent.bottom = m_rcEvent.top + nEventHeight - XTP_DPI_Y(2);

		m_rcEvent.left += XTP_DPI_X(4) + XTP_DPI_X(2);
		m_rcEvent.right -= XTP_DPI_X(5) + XTP_DPI_X(2);

		m_rcEventMax.top	= m_rcEvent.top;
		m_rcEventMax.bottom = m_rcEvent.bottom;

		// adjust subject area
		m_rcText.CopyRect(m_rcEvent);

		// set times rects
		int nTimeCellWidth = 0;

		if (IsTimeAsClock())
		{
			nTimeCellWidth = XTP_DPI_X(2)
							 + XTP_SAFE_GET2(GetView(), GetPaintManager(), GetClockSize().cx, 0);
		}
		else
		{
			CString strTime = XTP_SAFE_GET1(GetView(), GetItemTextEventTimeMax(), CString());
			nTimeCellWidth  = XTP_DPI_X(2)
							 + XTP_SAFE_GET3(GetView(), GetPaintManager(), GetMonthViewEventPart(),
											 GetTextExtent(pDC, strTime).cx, 0);
		}

		int nECFormat = XTP_SAFE_GET1(((CXTPCalendarMonthView*)GetView()), GetEventCaptionFormat(),
									  0);

		BOOL bStartTime	= nECFormat == xtpCalendarCaptionFormatStartSubject;
		BOOL bStartEndTime = nECFormat == xtpCalendarCaptionFormatStartEndSubject;
		if (bStartTime || bStartEndTime)
		{
			m_rcStartTime.CopyRect(m_rcEvent);
			m_rcStartTime.right = min(m_rcStartTime.left + nTimeCellWidth, m_rcEvent.right);
			m_rcText.left		= min(m_rcStartTime.right + XTP_DPI_X(1), m_rcEvent.right);
		}

		if (bStartEndTime)
		{
			m_rcEndTime.CopyRect(m_rcStartTime);
			m_rcEndTime.OffsetRect(nTimeCellWidth + XTP_DPI_X(1), 0);
			m_rcEndTime.left  = min(m_rcEndTime.left, m_rcEvent.right);
			m_rcEndTime.right = min(m_rcEndTime.right, m_rcEvent.right);
			m_rcText.left	 = min(m_rcEndTime.right + XTP_DPI_X(1), m_rcEvent.right);
		}

		if (bStartTime || bStartEndTime)
		{
			m_rcText.left = min(m_rcText.left + XTP_DPI_X(5), m_rcEvent.right);
		}

		m_rcSubjectEditor.CopyRect(m_rcEvent);
		m_rcSubjectEditor.right = m_rcEventMax.right;
		m_rcSubjectEditor.top += XTP_DPI_Y(1);
		m_rcSubjectEditor.bottom -= XTP_DPI_Y(1);
	}
	ASSERT(m_nMultiDayEventFlags);
}

void CXTPCalendarMonthViewEvent::AdjustLayout2(CDC* pDC, const CRect& rcEventMax,
											   int nEventPlaceNumber)
{
	TBase::AdjustLayout(pDC, rcEventMax, nEventPlaceNumber);

	m_nMultiDayEventFlags = 0;

	CXTPCalendarView* pView   = GetView();
	CXTPCalendarEvent* pEvent = GetEvent();

	if (!pEvent || !pDC || !pView || !XTP_SAFE_GET1(pView, GetTheme(), NULL))
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTheme* pTheme = pView->GetTheme();

	// all-day event
	if (IsMultidayEvent())
	{
		// to calculate multiday-flags
		AdjustMultidayEvent(rcEventMax, nEventPlaceNumber);

		pTheme->GetMonthViewPart()->GetDayPart()->GetMultiDayEventPart()->AdjustLayout(
			this, pDC, rcEventMax, nEventPlaceNumber);
		return;
	}

	m_nMultiDayEventFlags = xtpCalendarMultiDayNoMultiDay;

	pTheme->GetMonthViewPart()->GetDayPart()->GetSingleDayEventPart()->AdjustLayout(
		this, pDC, rcEventMax, nEventPlaceNumber);
}

BOOL CXTPCalendarMonthViewEvent::IsVisible() const
{
	//<<EventVisible>>
	if (GetEvent() && !GetEvent()->IsEventVisible())
		return FALSE;
	//<<EventVisible>>

	if (!GetViewGroup() || !GetViewGroup()->GetViewDay())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CRect rcDayEvents = GetViewGroup()->GetViewDay()->GetDayEventsRect();
	BOOL bVisible	 = m_rcEvent.bottom <= rcDayEvents.bottom;
	return bVisible;
}

BOOL CXTPCalendarMonthViewEvent::HitTestEx(CPoint point, XTP_CALENDAR_HITTESTINFO* pInfo) const
{
	if (!GetView())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (!m_rcEventMax.PtInRect(point) || !IsVisible())
	{
		return FALSE;
	}
	//-----------------------------------------
	XTP_CALENDAR_HITTESTINFO tmpInfo;
	// tmpInfo.pt = point;
	tmpInfo.dt					 = GetViewGroup()->GetViewDay()->GetDayDate();
	tmpInfo.dayPOS.nWeekIndex	= GetViewGroup()->GetViewDay()->m_nWeekIndex;
	tmpInfo.dayPOS.nWeekDayIndex = GetViewGroup()->GetViewDay()->m_nWeekDayIndex;
	tmpInfo.pViewDay			 = GetViewGroup()->GetViewDay();
	tmpInfo.pViewGroup			 = GetViewGroup();
	tmpInfo.pViewEvent			 = (CXTPCalendarViewEvent*)this;
	tmpInfo.uHitCode			 = xtpCalendarHitTestUnknown;
	//-----------------------------------------
	BOOL bIsFirst = (m_nMultiDayEventFlags & xtpCalendarMultiDayFirst);
	BOOL bIsLast  = (m_nMultiDayEventFlags & xtpCalendarMultiDayLast);
	// BOOL bIsMiddle = (m_nMultiDayEventFlags & xtpCalendarMultiDayMiddle);

	CRect rcBegin = m_rcEventMax;
	rcBegin.right = min(m_rcEvent.left + XTP_DPI_X(5), m_rcEvent.right);
	if (!bIsFirst)
	{
		rcBegin		  = m_rcEvent;
		rcBegin.right = rcBegin.left;
	}

	CRect rcEnd = m_rcEventMax;
	rcEnd.left  = max(m_rcEvent.right - XTP_DPI_X(5), m_rcEvent.left);
	if (!bIsLast)
	{
		rcEnd	  = m_rcEvent;
		rcEnd.left = rcEnd.right;
	}

	CRect rcTime   = m_rcEvent;
	int nTimeWidth = ((CXTPCalendarMonthView*)GetView())->GetEventTimeWidth();
	rcTime.left	= min(rcBegin.right, m_rcEvent.right);
	rcTime.right   = min(rcTime.left + max(nTimeWidth - XTP_DPI_X(4), 0), rcTime.right);
	BOOL bTimeUsed = nTimeWidth && !(m_nMultiDayEventFlags & xtpCalendarMultiDayFMLmask);

	CRect rcDrag = m_rcEventMax;
	rcDrag.right = rcDrag.left + XTP_DPI_X(1);

	CRect rcText = m_rcEvent;
	rcText.left  = bTimeUsed ? rcTime.right : rcBegin.right;
	rcText.right = max(rcEnd.left, rcText.left);

	// Must be one region without holes between Begin, Time, Text, End parts
	ASSERT(rcBegin.right == rcTime.left && bTimeUsed
			   ? rcTime.right == rcText.left
			   : rcBegin.right == rcText.left && rcText.right == rcEnd.left);

	// // for DEBUG
	// TRACE(_T("*** Begin(%d, %d) [%s]Time(%d, %d) Text(%d, %d) End(%d, %d) \n"), rcBegin.left,
	// rcBegin.right,
	//  bTimeUsed ? _T("+") :_T("-"), rcTime.left, rcTime.right,
	//  rcText.left, rcText.right, rcEnd.left, rcEnd.right);

	if (bIsFirst && rcBegin.PtInRect(point))
	{
		tmpInfo.uHitCode = xtpCalendarHitTestEventResizeHArea | xtpCalendarHitTestEventResizeBegin;
		*pInfo			 = tmpInfo;
		return TRUE;
	}

	if (bIsLast && rcEnd.PtInRect(point))
	{
		tmpInfo.uHitCode = xtpCalendarHitTestEventResizeHArea | xtpCalendarHitTestEventResizeEnd;
		*pInfo			 = tmpInfo;
		return TRUE;
	}

	if (bTimeUsed && rcTime.PtInRect(point))
	{
		tmpInfo.uHitCode = xtpCalendarHitTestEventDragArea;
		*pInfo			 = tmpInfo;
		return TRUE;
	}

	// to repeat outlook logic, bat this is strange (looks like bug)
	// if (bIsMiddle && rcDrag.PtInRect(point))
	//{
	//  tmpInfo.uHitCode = xtpCalendarHitTestEventDragArea;
	// *pInfo = tmpInfo;
	//  return TRUE;
	//}

	if (rcText.PtInRect(point))
	{
		tmpInfo.uHitCode = xtpCalendarHitTestEventTextArea;
		*pInfo			 = tmpInfo;
		return TRUE;
	}

	//-----------------------------------------
	return FALSE;
}

CFont* CXTPCalendarMonthViewEvent::GetSubjectEditorFont() const
{
	CXTPCalendarPaintManager::CMonthViewEventPart* pMEPart =
		XTP_SAFE_GET2(GetView(), GetPaintManager(), GetMonthViewEventPart(), NULL);

	ASSERT(pMEPart);
	if (!pMEPart)
	{
		return NULL;
	}
	CFont* pFont = &pMEPart->GetTextFont();
	return pFont;
}

BOOL CXTPCalendarMonthViewEvent::IsShowEndTime() const
{
	return XTP_SAFE_GET1(GetCalendarControl(), MonthView_IsShowEndDate(), FALSE);
}

BOOL CXTPCalendarMonthViewEvent::IsTimeAsClock() const
{
	return XTP_SAFE_GET1(GetCalendarControl(), MonthView_IsShowTimeAsClocks(), FALSE);
}

void CXTPCalendarMonthViewEvent::OnPostAdjustLayout()
{
	TBase::OnPostAdjustLayout();

	int nMDFmask	   = xtpCalendarMultiDayNoMultiDay | xtpCalendarMultiDayMaster;
	BOOL bOutOfBorders = m_rcText.Width() < m_szText.cx && (m_nMultiDayEventFlags & nMDFmask) != 0;

	SetTextOutOfBorders(bOutOfBorders);
}
