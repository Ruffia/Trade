// XTPCalendarViewEvent.cpp: implementation of the CXTPCalendarViewEvent class.
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
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPDrawHelpers.h"

#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarResource.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarTheme.h"
#include "Calendar/XTPCalendarCustomProperties.h"
#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CXTPCalendarViewEventSubjectEditor : public CEdit

IMPLEMENT_DYNAMIC(CXTPCalendarViewEvent, CXTPCalendarWMHandler)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarViewEventSubjectEditor, CEdit)
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPCalendarViewEventSubjectEditor::CXTPCalendarViewEventSubjectEditor(CXTPCalendarControl* pOwner)
{
	ASSERT(pOwner);
	m_pOwner = pOwner;
}

CXTPCalendarViewEventSubjectEditor::~CXTPCalendarViewEventSubjectEditor()
{
}

BOOL CXTPCalendarViewEventSubjectEditor::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
												  LRESULT* pResult)
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (message == WM_MOUSEMOVE)
		{
			CPoint pt(lParam);
			MapWindowPoints(m_pOwner, &pt, 1);
			lParam = MAKELPARAM(pt.x, pt.y);
		}

		BOOL bHandled = m_pOwner->OnWndMsg_Children(message, wParam, lParam, pResult);
		if (bHandled)
		{
			return TRUE;
		}
	}
	return CEdit::OnWndMsg(message, wParam, lParam, pResult);
}

UINT CXTPCalendarViewEventSubjectEditor::OnGetDlgCode()
{
	return DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTALLKEYS;
}

//////////////////////////////////////////////////////////////////////////

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarViewEventSubjectEditorColored, CXTPCalendarViewEventSubjectEditor)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPCalendarViewEventSubjectEditorColored::CXTPCalendarViewEventSubjectEditorColored(
	CXTPCalendarControl* pOwner, CXTPCalendarViewEvent* pViewEvent, COLORREF clrBackground)
	: CXTPCalendarViewEventSubjectEditor(pOwner)
{
	ASSERT(pViewEvent);

	m_pViewEvent = pViewEvent;

	if (!pViewEvent)
	{
		return;
	}

	m_grclrBk = clrBackground;
}

CXTPCalendarViewEventSubjectEditorColored::~CXTPCalendarViewEventSubjectEditorColored()
{
}

BOOL CXTPCalendarViewEventSubjectEditorColored::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);

	if (!m_pViewEvent || !m_pOwner)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CRect rcEvent = m_pViewEvent->GetEventRect();

	m_pOwner->ClientToScreen(&rcEvent);
	ScreenToClient(&rcEvent);

	CXTPClientRect rcClient(this);

	XTPDrawHelpers()->GradientFill(pDC, &rcEvent, m_grclrBk, FALSE, &rcClient);

	return TRUE;
}

HBRUSH CXTPCalendarViewEventSubjectEditorColored::CtlColor(CDC* pDC, UINT nCtlColor)
{
	ASSERT(nCtlColor == CTLCOLOR_EDIT);
	UNREFERENCED_PARAMETER(nCtlColor);

	static CBrush s_null_brush;
	if (s_null_brush.m_hObject == NULL)
	{
		s_null_brush.CreateStockObject(NULL_BRUSH);
	}

	pDC->SetBkMode(TRANSPARENT);

	DWORD dwStyle   = GetStyle();
	BOOL bMultiLine = (dwStyle & ES_MULTILINE) != 0;

	return bMultiLine ? (HBRUSH)s_null_brush : (HBRUSH)NULL;
}

void CXTPCalendarViewEventSubjectEditorColored::OnChange()
{
	Invalidate();
}

//////////////////////////////////////////////////////////////////////
CXTPCalendarViewEvent::CXTPCalendarViewEvent(CXTPCalendarEvent* pEvent,
											 CXTPCalendarViewGroup* pViewGroup)
{
	m_rcEvent.SetRectEmpty();
	m_rcEventMax.SetRectEmpty();
	m_rcText.SetRectEmpty();
	m_rcFirstClock.SetRectEmpty();
	m_rcLastClock.SetRectEmpty();
	m_rcStartTime.SetRectEmpty();
	m_rcEndTime.SetRectEmpty();
	m_rcReminderGlyph.SetRectEmpty();
	m_rcRecurrenceGlyph.SetRectEmpty();
	m_rcMeetingGlyph.SetRectEmpty();
	m_rcPrivateGlyph.SetRectEmpty();
	m_rcSubjectEditor.SetRectEmpty();
	m_rcToolTip.SetRectEmpty();

	UNUSED_ALWAYS(pViewGroup);

	m_bShowingToolTip = FALSE;

	ASSERT(pEvent);
	m_ptrEvent = pEvent;
	CMDTARGET_ADDREF(m_ptrEvent);

	m_pMasterView = NULL;
	m_pUIElement  = NULL;

	m_bSelected = FALSE;

	m_nMultiDayEventFlags = xtpCalendarMultiDayUnknown;

	m_nEventPlaceNumber			= 0;
	m_nEventTimelinePlaceNumber = 0;

	m_nTimerID_ShowToolTip = 0;

	m_pSubjectEditor = NULL;

	m_rcText.SetRect(0, 0, 0, 0);
	m_szText.cx			 = 0;
	m_szText.cy			 = 0;
	m_bMultyDayTextAlign = FALSE;

	m_bTextOutOfBorders = TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarViewEvent::~CXTPCalendarViewEvent()
{
	//---------------------------------------------------------------------------
	if (m_pSubjectEditor)
	{
		ASSERT(FALSE);
		delete m_pSubjectEditor;
		m_pSubjectEditor = NULL;
	}

	CMDTARGET_RELEASE(m_ptrEvent);
	CMDTARGET_RELEASE(m_pMasterView);

	XTPMarkupReleaseElement(m_pUIElement);

	ASSERT(m_nTimerID_ShowToolTip == 0);
}

void CXTPCalendarViewEvent::OnBeforeDestroy()
{
	//---------------------------------------------------------------------------

	if (IsEditingSubject())
	{
		XTP_SAFE_CALL3(GetViewGroup_(), GetViewDay_(), GetView_(),
					   EndEditSubject(xtpCalendarEditSubjectCommit, FALSE));
	}
	//---------------------------------------------------------------------------
	if (m_nTimerID_ShowToolTip)
	{
		KillTimer(m_nTimerID_ShowToolTip);
		m_nTimerID_ShowToolTip = 0;
	}

	if (IsShowingToolTip())
	{
		HideToolTip();
	}
}

BOOL CXTPCalendarViewEvent::HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	return HitTestEx(pt, pHitTest);
}

CXTPCalendarControl* CXTPCalendarViewEvent::GetCalendarControl() const
{
	return XTP_SAFE_GET1(GetViewGroup_(), GetCalendarControl(), NULL);
}

void CXTPCalendarViewEvent::AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber)
{
	UNREFERENCED_PARAMETER(pDC);

	m_rcEvent.CopyRect(&rcEventMax);
	m_rcEventMax.CopyRect(&rcEventMax);

	m_rcSubjectEditor.CopyRect(&rcEventMax);
	m_rcToolTip.CopyRect(&rcEventMax);
	m_rcText.CopyRect(&rcEventMax);
	m_rcStartTime.CopyRect(m_rcEvent);
	m_rcStartTime.DeflateRect(0, 0, m_rcStartTime.Width(), m_rcStartTime.Height());
	m_rcEndTime.CopyRect(m_rcStartTime);

	if (NULL != m_pUIElement)
	{
	}

	m_nEventPlaceNumber = nEventPlaceNumber;
}

BOOL CXTPCalendarViewEvent::MarkupHandleMouseEvent(UINT nMessage, UINT nFlags, const CPoint& point)
{
	BOOL bHandled = FALSE;

	if (NULL != m_pUIElement)
	{
		LRESULT lUnused = 0;
		bHandled		= XTPMarkupRelayMessage(m_pUIElement, nMessage, nFlags,
											MAKELPARAM(point.x, point.y), &lUnused);
	}

	return bHandled;
}

BOOL CXTPCalendarViewEvent::OnLButtonDown(UINT nFlags, CPoint point)
{
	return MarkupHandleMouseEvent(WM_LBUTTONDOWN, nFlags, point);
}

BOOL CXTPCalendarViewEvent::OnLButtonUp(UINT nFlags, CPoint point)
{
	return MarkupHandleMouseEvent(WM_LBUTTONUP, nFlags, point);
}

BOOL CXTPCalendarViewEvent::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	return MarkupHandleMouseEvent(WM_LBUTTONDBLCLK, nFlags, point);
}

void CXTPCalendarViewEvent::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCalendarView* pView = XTP_SAFE_GET2(GetViewGroup_(), GetViewDay_(), GetView_(), NULL);

	if (!pView || !GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	MarkupHandleMouseEvent(WM_MOUSEMOVE, nFlags, point);

	XTP_CALENDAR_HITTESTINFO hitInfo;
	BOOL bIsNoDragging = pView->GetDraggingMode() == xtpCalendaDragModeUnknown;
	BOOL bHideTooltip  = TRUE;
	BOOL bForceTooltip = !GetCalendarControl()->m_sCustomFormat4Tooltip.IsEmpty();

	//<< fix for Multiday Events >>
	if (IsMultidayEvent())
	{
		CRect rcEvent(GetEventRect());
		if (rcEvent.PtInRect(point))
			point.x = rcEvent.left + XTP_DPI_X(10);
	}
	//<< fix for Multiday Events >>

	BOOL bHit = HitTest(point, &hitInfo);

	// to check that mouse is not over some topmost element (like Expand/Scroll button)
	if (bHit)
	{
		bHit = XTP_SAFE_GET3(GetViewGroup_(), GetViewDay_(), GetView_(), HitTest(point, &hitInfo),
							 FALSE)
			   && ((hitInfo.uHitCode & xtpCalendarHitTestEvent_Mask) && hitInfo.pViewEvent == this);
	}

	if (bHit)
	{
		int eDragMode = pView->_GetDraggingMode(&hitInfo);
		BOOL bDragEnabled =
			!pView->IsEditOperationDisabledNotify(pView->_DragMod2Operation(eDragMode), this);

		if (bDragEnabled && hitInfo.uHitCode & xtpCalendarHitTestEventResizeHArea)
		{
			GetCalendarControl()->m_mouseMode = xtpCalendarMouseEventPreResizeH;
		}
		else if (bDragEnabled && hitInfo.uHitCode & xtpCalendarHitTestEventResizeVArea)
		{
			GetCalendarControl()->m_mouseMode = xtpCalendarMouseEventPreResizeV;
		}
		else if (bDragEnabled && hitInfo.uHitCode & xtpCalendarHitTestEventDragArea)
		{
			GetCalendarControl()->m_mouseMode = xtpCalendarMouseEventPreDrag;
		}
		else if (hitInfo.uHitCode & xtpCalendarHitTestEventTextArea)
		{
			bHideTooltip = !!(nFlags & MK_LBUTTON);
			BOOL bShow   = GetCalendarControl()->m_bShowTooltipForAllDayEvents || !IsMultidayEvent()
						 || (XTP_SAFE_GET2(pView, GetResources(), GetCount(), 0) > 1)
						 || bForceTooltip;

			if (bShow && bIsNoDragging && !IsEditingSubject() && !pView->IsEditingSubject()
				&& !IsShowingToolTip() && !(nFlags & MK_LBUTTON)
				&& (m_bTextOutOfBorders || bForceTooltip))
			{
				if (m_nTimerID_ShowToolTip == 0)
				{
					UINT uTimeOut_ms	   = GetShowToolTipTimeOut();
					m_nTimerID_ShowToolTip = SetTimer(uTimeOut_ms);
					ASSERT(m_nTimerID_ShowToolTip);
				}
			}
		}
	}
	else // we are not under Event view area
	{
		// kill waiting for mouse under event view area
		if (m_nTimerID_ShowToolTip)
		{
			KillTimer(m_nTimerID_ShowToolTip);
			m_nTimerID_ShowToolTip = 0;
		}
	}

	if (bHideTooltip && IsShowingToolTip())
	{
		HideToolTip();
	}
}

BOOL CXTPCalendarViewEvent::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bHandled = FALSE;

	if (NULL != m_pUIElement)
	{
		bHandled = XTPMarkupRelayMessage(m_pUIElement, message, wParam, lParam, pResult);
	}

	return bHandled;
}

void CXTPCalendarViewEvent::AddMultiDayEvent_Slave(CXTPCalendarViewEvent* pSlaveView,
												   int nMasterEventPlace)
{
	if (!pSlaveView || !pSlaveView->GetViewGroup_() || !GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	pSlaveView->GetViewGroup_()->ExchangeEventPlace(pSlaveView, nMasterEventPlace);
	pSlaveView->SetMultiDayEvent_MasterSlave(FALSE, this);

	if (!pSlaveView->IsVisible())
	{
		return;
	}

	//  ASSERT(m_rcEvent.top == pSlaveView->m_rcEvent.top);
	//  ASSERT(m_rcEventMax.bottom == pSlaveView->m_rcEventMax.bottom);

	m_rcEvent.right	= pSlaveView->m_rcEvent.right;
	m_rcEventMax.right = pSlaveView->m_rcEventMax.right;

	// correct right margins
	m_rcText.right = m_rcEvent.right;
	m_rcLastClock.CopyRect(m_rcText);
	m_rcLastClock.left   = m_rcLastClock.right;
	m_rcLastClock.bottom = m_rcLastClock.top;

	// pSlaveView->SetMultiDayEvent_MasterSlave(FALSE, this);

	int nMDEFlags = pSlaveView->GetMultiDayEventFlags();

	if (nMDEFlags & xtpCalendarMultiDayLast)
	{
		m_nMultiDayEventFlags |= xtpCalendarMultiDayLast;
		COleDateTime dtEE(GetEvent()->GetEndTime());
		if (dtEE == CXTPCalendarUtils::ResetTime(dtEE) || GetEvent()->IsAllDayEvent())
		{
			m_rcLastClock.left = m_rcLastClock.right;
		}
		else
		{
			// correct last clock rect
			CSize szClock		 = XTP_SAFE_GET4(GetViewGroup_(), GetViewDay_(), GetView_(),
											 GetPaintManager(), GetClockSize(), CSize(0));
			m_rcLastClock.left   = m_rcLastClock.right - szClock.cx - XTP_DPI_X(1);
			m_rcLastClock.bottom = m_rcLastClock.top + szClock.cy;

			int delta = (m_rcEvent.Height() - szClock.cy) / 2 - XTP_DPI_Y(1);
			m_rcLastClock.OffsetRect(-delta, delta);

			m_rcText.right -= szClock.cx;
		}
	}
}

BOOL CXTPCalendarViewEvent::IsMultidayEvent() const
{
	if (!m_ptrEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_ptrEvent->IsAllDayEvent())
	{
		return TRUE;
	}

	int nDays = m_ptrEvent->GetEventPeriodDays();
	return nDays > 1;
}

void CXTPCalendarViewEvent::OnPostAdjustLayout()
{
	if (IsMultidayEvent())
	{
		m_rcSubjectEditor.CopyRect(&m_rcEvent);
		m_rcSubjectEditor.left += XTP_DPI_X(1);

		m_rcText.CopyRect(m_rcEvent);
		m_rcText.left += min(m_rcText.Width() - XTP_DPI_X(1),
							 m_rcFirstClock.Width() + XTP_DPI_X(4));
		m_rcText.right -= min(m_rcText.Width() - XTP_DPI_X(1),
							  m_rcLastClock.Width() + XTP_DPI_X(4));

		int nGlyphs = CalcIconsRect(m_rcEvent);
		CRect rcGlyphs(max(m_rcText.CenterPoint().x - (m_szText.cx / 2 + nGlyphs + XTP_DPI_X(3)),
						   m_rcText.left),
					   m_rcText.top, m_rcText.right, m_rcText.bottom);

		if (m_rcText.CenterPoint().x <= m_rcText.left + nGlyphs + m_szText.cx / 2) //
		{
			m_rcText.left = min(m_rcText.left + CalcIconsRect(rcGlyphs), m_rcText.right);

			m_bMultyDayTextAlign = TRUE;
		}
		else
		{
			m_bMultyDayTextAlign = FALSE;
			CalcIconsRect(rcGlyphs);
		}
	}

	m_rcToolTip = m_rcEvent;

	AdjustSubjectEditor();
}

void CXTPCalendarViewEvent::AdjustMultidayEvent(CRect rcEventMax, int nEventPlaceNumber)
{
	if (!XTP_SAFE_GET3(GetViewGroup_(), GetViewDay_(), GetView_(), GetPaintManager(), NULL)
		|| !m_ptrEvent)
	{
		ASSERT(FALSE);
		return;
	}

	int nRowHeight = GetViewGroup_()->GetViewDay_()->GetView_()->GetRowHeight();

	m_nMultiDayEventFlags = 0;

	m_rcEventMax.top	= rcEventMax.top + nRowHeight * nEventPlaceNumber + XTP_DPI_Y(1);
	m_rcEventMax.bottom = m_rcEventMax.top + nRowHeight - XTP_DPI_Y(2);

	m_rcEvent.CopyRect(&m_rcEventMax);
	m_rcEvent.left += XTP_DPI_X(4);
	m_rcEvent.right -= XTP_DPI_X(4);

	if (!IsMultidayEvent())
	{
		m_nMultiDayEventFlags = xtpCalendarMultiDayNoMultiDay;
		return;
	}

	COleDateTime dtDay = GetViewGroup_()->GetViewDay_()->GetDayDate();

	COleDateTime dtStartDay = CXTPCalendarUtils::ResetTime(m_ptrEvent->GetStartTime());
	COleDateTime dtEndDay   = CXTPCalendarUtils::RetriveEventEndDate(m_ptrEvent);

	ASSERT(dtDay == dtStartDay || dtDay == dtEndDay || dtDay > dtStartDay && dtDay < dtEndDay);

	m_rcText.CopyRect(m_rcEvent);
	m_rcFirstClock.CopyRect(m_rcEvent);
	m_rcFirstClock.right  = m_rcFirstClock.left;
	m_rcFirstClock.bottom = m_rcFirstClock.top;
	CSize szClock(GetViewGroup_()->GetViewDay_()->GetView_()->GetPaintManager()->GetClockSize());

	if (dtDay == dtStartDay)
	{
		m_nMultiDayEventFlags |= xtpCalendarMultiDayFirst;
		if (!m_ptrEvent->IsAllDayEvent() && dtDay != m_ptrEvent->GetStartTime())
		{
			m_rcFirstClock.right  = m_rcFirstClock.left + szClock.cx;
			m_rcFirstClock.bottom = m_rcFirstClock.top + szClock.cy;

			int delta = (m_rcEvent.Height() - szClock.cy) / 2 - XTP_DPI_Y(1);
			m_rcFirstClock.OffsetRect(delta, delta);

			m_rcText.left += m_rcFirstClock.right + XTP_DPI_X(4);
		}
	}

	if (dtDay == dtEndDay)
	{
		m_nMultiDayEventFlags |= xtpCalendarMultiDayLast;
	}

	COleDateTimeSpan spDay(1, 0, 0, 0);
	if (dtDay == m_ptrEvent->GetEndTime() - spDay)
	{
		m_nMultiDayEventFlags |= xtpCalendarMultiDayLast;
	}
	else if (dtDay > dtStartDay && dtDay < dtEndDay)
	{
		m_nMultiDayEventFlags |= xtpCalendarMultiDayMiddle;
	}

	// m_rcGlyphs.CopyRect(rcEventMax);
	m_rcStartTime.CopyRect(m_rcEventMax);
	m_rcStartTime.right = m_rcStartTime.left;
	m_rcEndTime.CopyRect(m_rcEventMax);
	m_rcStartTime.left = m_rcStartTime.right;
}

void AFX_CDECL CXTPCalendarViewEvent::_MoveRectY(int nYbasePrev, int nYbaseNew, CRect& rRC)
{
	int nOffset = rRC.top - nYbasePrev;
	rRC.bottom  = rRC.Height() + nYbaseNew + nOffset;
	rRC.top		= nYbaseNew + nOffset;
}

void CXTPCalendarViewEvent::ChangeEventPlace(int nNewPlace)
{
	int nEventHeight = XTP_SAFE_GET3(GetViewGroup_(), GetViewDay_(), GetView_(), GetRowHeight(),
									 XTP_DPI_Y(1));

	int nY0		   = m_rcEvent.top;
	int nEventsTop = m_rcEvent.top - m_nEventPlaceNumber * nEventHeight - XTP_DPI_Y(1);
	int nY1		   = nEventsTop + nNewPlace * nEventHeight + XTP_DPI_Y(1);

	_ChangeEventPlace(nNewPlace, nY0, nY1);
}

void CXTPCalendarViewEvent::_ChangeEventPlace(int nNewPlace, int nYbasePrev, int nYbaseNew)
{
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcFirstClock);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcLastClock);

	_MoveRectY(nYbasePrev, nYbaseNew, m_rcStartTime);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcEndTime);

	_MoveRectY(nYbasePrev, nYbaseNew, m_rcReminderGlyph);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcRecurrenceGlyph);

	_MoveRectY(nYbasePrev, nYbaseNew, m_rcMeetingGlyph);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcPrivateGlyph);

	_MoveRectY(nYbasePrev, nYbaseNew, m_rcEvent);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcEventMax);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcSubjectEditor);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcToolTip);
	_MoveRectY(nYbasePrev, nYbaseNew, m_rcText);

	m_nEventPlaceNumber = nNewPlace;
}

void CXTPCalendarViewEvent::SetMultiDayEvent_MasterSlave(BOOL bMaster,
														 CXTPCalendarViewEvent* pMasterView)
{
	m_nMultiDayEventFlags |= bMaster ? xtpCalendarMultiDayMaster : xtpCalendarMultiDaySlave;

	ASSERT(bMaster && !pMasterView || !bMaster && pMasterView);

	CMDTARGET_RELEASE(m_pMasterView);
	m_pMasterView = pMasterView;
	CMDTARGET_ADDREF(m_pMasterView);
}

UINT CXTPCalendarViewEvent::SetTimer(UINT uTimeOut_ms)
{
	return XTP_SAFE_GET1(GetViewGroup_(), SetTimer(uTimeOut_ms), 0);
}

void CXTPCalendarViewEvent::KillTimer(UINT uTimerID)
{
	XTP_SAFE_CALL1(GetViewGroup_(), KillTimer(uTimerID));
}

BOOL CXTPCalendarViewEvent::OnTimer(UINT_PTR uTimerID)
{
	if (!uTimerID)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (uTimerID == m_nTimerID_ShowToolTip)
	{
		ShowToolTip();
		return TRUE;
	}
	return FALSE;
}

CXTPCalendarViewEvent* CXTPCalendarViewEvent::StartEditSubject()
{
	//- Forward editing process to master event ---------------------------------
	if (m_pMasterView)
	{
		if (!m_pMasterView->IsEditingSubject())
			m_pMasterView->StartEditSubject();

		return m_pMasterView;
	}

	// ensure that previous one is destroyed.
	if (m_pSubjectEditor)
	{
		ASSERT(FALSE);
		delete m_pSubjectEditor;
		m_pSubjectEditor = NULL;
	}

	CXTPCalendarControl* pControl = GetCalendarControl();
	if (!pControl)
	{
		ASSERT(FALSE);
		return NULL;
	}

	if (IsShowingToolTip())
		HideToolTip();

	// let theme to customize subject editor
	if (pControl->GetTheme())
	{
		m_pSubjectEditor = pControl->GetTheme()->StartEditSubject(this);
		if (m_pSubjectEditor)
		{
			//<<>> RELEASE MODE FIX <<
			m_pSubjectEditor->SetFocus();
			CFont* pEditorFont = GetSubjectEditorFont();
			if (pEditorFont)
				m_pSubjectEditor->SetFont(pEditorFont);

			m_pSubjectEditor->SetWindowText(XTP_SAFE_GET1(m_ptrEvent, GetSubject(), CString()));
			m_pSubjectEditor->SetSel(0, 0);
			//<<>> RELEASE MODE FIX >>
			return this;
		}
	}
	else if (pControl->GetPaintManager())
	{
		m_pSubjectEditor = pControl->GetPaintManager()->StartEditSubject(this, m_rcSubjectEditor);
	}
	else
	{
		//- create and show text editor ---------------------------------------------
		m_pSubjectEditor = new CXTPCalendarViewEventSubjectEditor(pControl);
	}

	ASSERT(m_pSubjectEditor);

	if (!m_pSubjectEditor)
		return NULL;

	DWORD dwEditStyles = WS_CHILD | WS_VISIBLE | GetSubjectEditorStyles();

	if (GetEvent()->IsAllDayEvent() || IsMultidayEvent())
	{
		dwEditStyles |= ES_CENTER;
	}

	m_pSubjectEditor->Create(dwEditStyles, m_rcSubjectEditor, pControl, 0);
	m_pSubjectEditor->SetFocus();

	if (pControl->GetPaintManager())
	{
		pControl->GetPaintManager()->SetEditFont(this, m_pSubjectEditor);
	}
	else
	{
		CFont* pEditorFont = GetSubjectEditorFont();
		if (pEditorFont)
			m_pSubjectEditor->SetFont(pEditorFont);
	}

	m_pSubjectEditor->SetWindowText(XTP_SAFE_GET1(m_ptrEvent, GetSubject(), CString()));
	m_pSubjectEditor->SetSel(0, 0);

	return this;
}

CString CXTPCalendarViewEvent::GetEditingSubjectFromEditor() const
{
	if (!m_pSubjectEditor)
	{
		ASSERT(FALSE);
		return _T("");
	}

	CString strSubject;
	m_pSubjectEditor->GetWindowText(strSubject);

	strSubject.Replace(_T(" \r\r\n "), _T(" "));
	strSubject.Replace(_T(" \r\r\n"), _T(" "));
	strSubject.Replace(_T("\r\r\n "), _T(" "));

	strSubject.Replace(_T("\r\n"), _T(" "));
	strSubject.Replace(_T("\n"), _T(" "));
	strSubject.Replace(_T("\r"), _T(" "));

	return strSubject;
}

CString CXTPCalendarViewEvent::EndEditSubject()
{
	if (!m_pSubjectEditor)
	{
		ASSERT(FALSE);
		return _T("");
	}

	CString strSubject = GetEditingSubjectFromEditor();

	//---------------------------------------------------------------------------
	delete m_pSubjectEditor;
	m_pSubjectEditor = NULL;
	//---------------------------------------------------------------------------
	return strSubject;
}

BOOL CXTPCalendarViewEvent::IsShowingToolTip()
{
	return m_bShowingToolTip;
}

void CXTPCalendarViewEvent::ShowToolTip()
{
	if (m_nTimerID_ShowToolTip)
	{
		KillTimer(m_nTimerID_ShowToolTip);
		m_nTimerID_ShowToolTip = 0;
	}
	BOOL bFocused = (CWnd::GetFocus() == (CWnd*)GetCalendarControl());

	if (!bFocused && !GetCalendarControl()->m_bShowTooltipForNotFocused)
		return;

	//<<EventVisible>>
	if (m_ptrEvent != NULL && !m_ptrEvent->IsEventVisible())
		return;
	//<<EventVisible>>

	// create tooltip text
	CString strTip = GetToolText();

	// show tooltip window
	if (!strTip.IsEmpty() && GetCalendarControl())
	{
		GetCalendarControl()->ShowToolTip(strTip, m_rcToolTip, GetSubjectEditorFont());
		m_bShowingToolTip = TRUE;
	}
}

void CXTPCalendarViewEvent::HideToolTip()
{
	if (GetCalendarControl())
		GetCalendarControl()->HideToolTip();

	m_bShowingToolTip = FALSE;
}

CString CXTPCalendarViewEvent::FormatEventTime(COleDateTime dtTime) const
{
	return XTP_SAFE_GET3(GetViewGroup_(), GetViewDay_(), GetView_(), FormatEventTime(dtTime),
						 CString());
}

CString CXTPCalendarViewEvent::GetToolText()
{
	if (!m_ptrEvent)
		return _T("");

	CString sTime, sSubj, sLoc, sBody, strTip;

	if (!IsMultidayEvent())
	{
		COleDateTime dtStart = m_ptrEvent->GetStartTime();
		COleDateTime dtEnd   = m_ptrEvent->GetEndTime();

		DWORD dwTimeFlags = xtpCalendarItemText_EventStartTimeText;
		if (!CXTPCalendarUtils::IsEqual(dtStart, dtEnd))
			dwTimeFlags = dwTimeFlags | xtpCalendarItemText_EventEndTimeText;

		sTime = GetItemText(dwTimeFlags);
	}

	// add Location for Office2007 theme
	CXTPCalendarControl* pControl = GetCalendarControl();
	if (pControl)
	{
		sSubj = m_ptrEvent->GetSubject();
		sLoc  = m_ptrEvent->GetLocation();
		sBody = m_ptrEvent->GetBody();
		int jPos(-1), iPos(-1);
		strTip = pControl->m_sCustomFormat4Tooltip;
		if (!strTip.IsEmpty())
		{
			CString s, t;
			jPos = strTip.Find(_T("<TIME>"));
			if (jPos > -1)
			{
				s	= strTip.Left(jPos + 1);
				iPos = s.ReverseFind(_T('>'));
				if (iPos > -1 && sTime.IsEmpty())
				{
					t = s.Mid(iPos);
					strTip.Replace(t, _T("><"));
				}
				else if (iPos == -1) // first tag case
				{
					iPos = s.ReverseFind(_T('<'));
					if (iPos > -1 && sTime.IsEmpty())
					{
						t = s.Left(iPos - 1);
						strTip.Replace(t, _T(""));
					}
				}
			}
			jPos = strTip.Find(_T("<SUBJ>"));
			if (jPos > -1)
			{
				s	= strTip.Left(jPos + 1);
				iPos = s.ReverseFind(_T('>'));
				if (iPos > -1 && sSubj.IsEmpty())
				{
					t = s.Mid(iPos);
					strTip.Replace(t, _T("><"));
				}
				else if (iPos == -1) // first tag case
				{
					iPos = s.ReverseFind(_T('<'));
					if (iPos > -1 && sSubj.IsEmpty())
					{
						t = s.Left(iPos - 1);
						strTip.Replace(t, _T(""));
					}
				}
			}
			jPos = strTip.Find(_T("<LOC>"));
			if (jPos > -1)
			{
				s	= strTip.Left(jPos + 1);
				iPos = s.ReverseFind(_T('>'));
				if (iPos > -1 && sLoc.IsEmpty())
				{
					t = s.Mid(iPos);
					strTip.Replace(t, _T("><"));
				}
				else if (iPos == -1) // first tag case
				{
					iPos = s.ReverseFind(_T('<'));
					if (iPos > -1 && sLoc.IsEmpty())
					{
						t = s.Left(iPos - 1);
						strTip.Replace(t, _T(""));
					}
				}
			}
			jPos = strTip.Find(_T("<BODY>"));
			if (jPos > -1)
			{
				s	= strTip.Left(jPos + 1);
				iPos = s.ReverseFind(_T('>'));
				if (iPos > -1 && sBody.IsEmpty())
				{
					t = s.Mid(iPos);
					strTip.Replace(t, _T("><"));
				}
				else if (iPos == -1) // first tag case
				{
					iPos = s.ReverseFind(_T('<'));
					if (iPos > -1 && sBody.IsEmpty())
					{
						t = s.Left(iPos - 1);
						strTip.Replace(t, _T(""));
					}
				}
			}
			strTip.Replace(_T("<TIME>"), sTime);
			strTip.Replace(_T("<SUBJ>"), sSubj);
			strTip.Replace(_T("<LOC>"), sLoc);
			strTip.Replace(_T("<BODY>"), sBody);
			strTip.Replace(_T("  "), _T(" "));
			strTip += _T(" ");
			// strTip += _T(". ");
		}
		else
		{
			if (!sTime.IsEmpty())
				sTime += _T(" ");
			strTip = sTime + sSubj;
			if (!sLoc.IsEmpty())
				strTip += _T("\n(") + sLoc + _T(")");
		}

		DWORD dwFlags = pControl->GetAskItemTextFlags();
		if (dwFlags & xtpCalendarItemText_EventToolTipText)
		{
			XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
			::ZeroMemory(&objRequest, sizeof(objRequest));

			objRequest.nItem	  = (int)xtpCalendarItemText_EventToolTipText;
			objRequest.pstrText   = &strTip;
			objRequest.pViewEvent = this;

			pControl->SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0);
		}
	}

	return strTip;
}

AFX_INLINE int CalcIconRect(const CRect& rc, const CSize& szIcon, CPoint& pt, CRect& rcIcon)
{
	rcIcon.left = min(pt.x, rc.right);
	rcIcon.top  = min(pt.y, rc.bottom);

	if (rc.bottom - (pt.y + szIcon.cy) > szIcon.cy)
	{
		pt.Offset(0, szIcon.cy);
	}
	else
	{
		pt.Offset(szIcon.cx, 0);
	}

	rcIcon.right  = rcIcon.left + szIcon.cx;
	rcIcon.bottom = rcIcon.top + szIcon.cy;

	if (rcIcon.right > rc.right || rcIcon.bottom > rc.bottom)
	{
		rcIcon.SetRectEmpty();
		return 0;
	}

	return szIcon.cx;
}

int CXTPCalendarViewEvent::CalcIconsRect(CRect rc)
{
	CSize szIcon(XTP_DPI_X(16), XTP_DPI_Y(11));

	rc.OffsetRect(0, XTP_DPI_Y(2));

	m_rcReminderGlyph.CopyRect(rc);
	m_rcReminderGlyph.right  = m_rcReminderGlyph.left;
	m_rcReminderGlyph.bottom = m_rcReminderGlyph.top + szIcon.cy;

	m_rcPrivateGlyph.CopyRect(rc);
	m_rcPrivateGlyph.right  = m_rcPrivateGlyph.left;
	m_rcPrivateGlyph.bottom = m_rcPrivateGlyph.top + szIcon.cy;

	m_rcRecurrenceGlyph.CopyRect(rc);
	m_rcRecurrenceGlyph.right  = m_rcRecurrenceGlyph.left;
	m_rcRecurrenceGlyph.bottom = m_rcRecurrenceGlyph.top + szIcon.cy;

	m_rcMeetingGlyph.CopyRect(rc);
	m_rcMeetingGlyph.right  = m_rcMeetingGlyph.left;
	m_rcMeetingGlyph.bottom = m_rcMeetingGlyph.top + szIcon.cy;

	CPoint pt(rc.TopLeft());
	int nMinSize = 0;

	CXTPCalendarEvent* pEvent = GetEvent();
	if (pEvent)
	{
		if (pEvent->IsReminder())
		{
			nMinSize = CalcIconRect(rc, szIcon, pt, m_rcReminderGlyph);
		}
		if (pEvent->IsPrivate())
		{
			nMinSize = CalcIconRect(rc, szIcon, pt, m_rcPrivateGlyph);
		}
		if (pEvent->GetRecurrenceState() > xtpCalendarRecurrenceNotRecurring)
		{
			nMinSize = CalcIconRect(rc, szIcon, pt, m_rcRecurrenceGlyph);
		}
		if (pEvent->IsMeeting())
		{
			nMinSize = CalcIconRect(rc, szIcon, pt, m_rcMeetingGlyph);
		}
	}

	return max((pt.x) - rc.left, nMinSize);
}

void CXTPCalendarViewEvent::AdjustSubjectEditor()
{
	if (IsEditingSubject() && m_pSubjectEditor)
	{
		m_pSubjectEditor->MoveWindow(&m_rcSubjectEditor, FALSE);
	}
}

BOOL CXTPCalendarViewEvent::IsSelected() const
{
	CXTPCalendarViewEvents* pSelEvents = XTP_SAFE_GET3(GetViewGroup_(), GetViewDay_(), GetView_(),
													   GetSelectedViewEvents(), NULL);
	if (!pSelEvents)
	{
		return FALSE;
	}

	int nFIndex = pSelEvents->Find(m_ptrEvent);
	return nFIndex >= 0;
}

void CXTPCalendarViewEvent::SetSelected(BOOL bSet)
{
	XTP_SAFE_CALL3(GetViewGroup_(), GetViewDay_(), GetView_(), SelectViewEvent(this, bSet));
}

CString CXTPCalendarViewEvent::GetItemText(DWORD dwItemFlag, LPCTSTR pcszDefaultText) const
{
	if (!GetCalendarControl() || !m_ptrEvent)
	{
		ASSERT(FALSE);
		return _T("");
	}

	DWORD dwFlags = GetCalendarControl()->GetAskItemTextFlags();

	CString strText;

	if (pcszDefaultText)
	{
		strText = pcszDefaultText;
	}
	else
	{
		if (dwItemFlag & xtpCalendarItemText_EventSubject)
		{
			strText = m_ptrEvent->GetSubject();
		}
		else if (dwItemFlag & xtpCalendarItemText_EventLocation)
		{
			strText = m_ptrEvent->GetLocation();
		}
		else if (dwItemFlag & xtpCalendarItemText_EventBody)
		{
			strText = m_ptrEvent->GetBody();
		}
		else if (dwItemFlag
				 & (xtpCalendarItemText_EventStartTimeText | xtpCalendarItemText_EventEndTimeText))
		{
			if (dwItemFlag & xtpCalendarItemText_EventStartTimeText)
				strText = FormatEventTime(m_ptrEvent->GetStartTime());

			if (dwItemFlag & xtpCalendarItemText_EventEndTimeText)
			{
				if (!strText.IsEmpty())
					strText += _T("-");
				strText += FormatEventTime(m_ptrEvent->GetEndTime());
			}
		}
		else if (dwItemFlag & xtpCalendarItemText_EventMarkupTemplate)
		{
			COleVariant oleMTempl;
			BOOL bProp = m_ptrEvent->GetCustomProperties()->GetProperty(
				cszEventCustProp_MarkupTemplate, oleMTempl);
			if (bProp && oleMTempl.vt == VT_BSTR)
			{
				strText = oleMTempl.bstrVal;
			}
		}
		else
		{
			return _T("");
		}
	}

	if ((dwItemFlag & xtpCalendarItemText_EventMarkupTemplate) == 0)
		strText.Left(XTP_CALENDAR_DRAW_EVENTITEMTEXT_MAX);

	if (dwFlags & dwItemFlag)
	{
		XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
		::ZeroMemory(&objRequest, sizeof(objRequest));

		objRequest.nItem	  = (int)dwItemFlag;
		objRequest.pstrText   = &strText;
		objRequest.pViewEvent = (CXTPCalendarViewEvent*)this;

		GetCalendarControl()->SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT,
													 (WPARAM)&objRequest, 0);
	}

	return strText;
}

CString CXTPCalendarViewEvent::GetMarkupText() const
{
	//  /Calendar/Events/Event/Subject
	//  /Calendar/Events/Event/Location
	//  /Calendar/Events/Event/Body
	//  /Calendar/Events/Event/StartTime
	//  /Calendar/Events/Event/EndTime

	// TODO: Add NoCase parameters replacement.

	CString strMarkupTemplate = GetItemText(xtpCalendarItemText_EventMarkupTemplate);
	// strMarkupTemplate = _T("<StackPanel><TextBlock Background='#00ebff' Padding='12, 0, 2,
	// 0'><Bold>%S</Bold></TextBlock><Border Height='1'/><TextBlock
	// Foreground='Green'>[%L]</TextBlock><Border
	// Height='1'/><TextBlock>%B</TextBlock></StackPanel>"); strMarkupTemplate =
	// _T("<StackPanel><TextBlock Padding='12, 0, 2, 0'><Bold>%S</Bold></TextBlock><Border
	// Height='1'/><TextBlock Foreground='Green'>[%L]</TextBlock><Border
	// Height='1'/><TextBlock>%B</TextBlock></StackPanel>");

	strMarkupTemplate.TrimLeft(_T(" \t\n\r"));
	strMarkupTemplate.TrimRight(_T(" \t\n\r"));

	if (strMarkupTemplate.Find(cszCalendarMarkupPrm_Subject) >= 0)
		strMarkupTemplate.Replace(cszCalendarMarkupPrm_Subject, GetItemTextSubject());

	if (strMarkupTemplate.Find(cszCalendarMarkupPrm_Location) >= 0)
		strMarkupTemplate.Replace(cszCalendarMarkupPrm_Location, GetItemTextLocation());

	if (strMarkupTemplate.Find(cszCalendarMarkupPrm_Body) >= 0)
		strMarkupTemplate.Replace(cszCalendarMarkupPrm_Body, GetItemTextBody());

	if (strMarkupTemplate.Find(cszCalendarMarkupPrm_StartTime) >= 0)
		strMarkupTemplate.Replace(cszCalendarMarkupPrm_StartTime, GetItemTextStartTime());

	if (strMarkupTemplate.Find(cszCalendarMarkupPrm_EndTime) >= 0)
		strMarkupTemplate.Replace(cszCalendarMarkupPrm_EndTime, GetItemTextEndTime());

	return strMarkupTemplate;
}

CXTPMarkupUIElement* CXTPCalendarViewEvent::GetMarkupUIElement()
{
	if (!GetCalendarControl() || !GetCalendarControl()->IsMarkupEnabled())
		return NULL;

	CXTPMarkupContext* pMarkupContext = GetCalendarControl()->GetMarkupContext();
	ASSERT(pMarkupContext);

	CString strMKText = GetMarkupText();

	if (!pMarkupContext || strMKText.IsEmpty())
		return NULL;

	if (!m_pUIElement || strMKText != m_strUIElementText)
	{
		XTPMarkupReleaseElement(m_pUIElement);

		m_pUIElement = XTPMarkupParseText(pMarkupContext, strMKText);

		m_strUIElementText = strMKText;
	}

	return m_pUIElement;
}

CString CXTPCalendarViewEvent::GetItemTextSubject() const
{
	return GetItemText(xtpCalendarItemText_EventSubject);
}

CString CXTPCalendarViewEvent::GetItemTextLocation() const
{
	return GetItemText(xtpCalendarItemText_EventLocation);
}

CString CXTPCalendarViewEvent::GetItemTextBody() const
{
	return GetItemText(xtpCalendarItemText_EventBody);
}

CString CXTPCalendarViewEvent::GetItemTextStartTime() const
{
	return GetItemText(xtpCalendarItemText_EventStartTimeText);
}

CString CXTPCalendarViewEvent::GetItemTextEndTime() const
{
	return GetItemText(xtpCalendarItemText_EventEndTimeText);
}

CXTPCalendarView* CXTPCalendarViewEvent::GetView() const
{
	return XTP_SAFE_GET2(GetViewGroup_(), GetViewDay_(), GetView_(), NULL);
}

//////////////////////////////////////////////////////////////////////////
//  CXTPCalendarViewEvents
//

CXTPCalendarViewEvents::CXTPCalendarViewEvents()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarViewEvents::~CXTPCalendarViewEvents()
{
}

int CXTPCalendarViewEvents::Find(const CXTPCalendarEvent* pEvent) const
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return -1;
	}

	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = GetAt(i);
		CXTPCalendarEvent* pEv			  = pViewEvent ? pViewEvent->GetEvent() : NULL;
		if (pEv && pEvent->IsEqualIDs(pEv) && pEvent->IsEqualStartEnd(pEv))
		{
			return i;
		}
	}

	return -1;
}

void CXTPCalendarViewEvents::Remove(CXTPCalendarEvent* pEvent)
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return;
	}

	int nCount = GetCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		CXTPCalendarViewEvent* pViewEvent = GetAt(i);
		CXTPCalendarEvent* pEv			  = XTP_SAFE_GET1(pViewEvent, GetEvent(), NULL);
		if (pEv && pEvent->IsEqualIDs(pEv) && pEvent->IsEqualStartEnd(pEv))
		{
			RemoveAt(i);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarViewEvent, CXTPCalendarWMHandler)
	DISP_PROPERTY_EX_ID(CXTPCalendarViewEvent, "Event", DISPID_VALUE, OleGetEvent, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPCalendarViewEvent, "Selected", 1, OleGetSelected, OleSetSelected,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCalendarViewEvent, "ViewDay", 2, OleGetViewDay, SetNotSupported,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarViewEvent, "ViewGroup", 3, OleGetViewGroup, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarViewEvent, "IsVisible", 4, IsVisible, VT_BOOL, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarViewEvent, CXTPCalendarWMHandler)
	INTERFACE_PART(CXTPCalendarViewEvent, XTPDIID_CalendarViewEvent, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarViewEvent, XTPDIID_CalendarViewEvent)

LPDISPATCH CXTPCalendarViewEvent::OleGetEvent()
{
	return m_ptrEvent ? m_ptrEvent->GetIDispatch(TRUE) : 0;
}

BOOL CXTPCalendarViewEvent::OleGetSelected()
{
	return IsSelected();
}

void CXTPCalendarViewEvent::OleSetSelected(BOOL bSelected)
{
	SetSelected(bSelected);
}

LPDISPATCH CXTPCalendarViewEvent::OleGetViewDay()
{
	return XTP_SAFE_GET2(GetViewGroup_(), GetViewDay_(), GetIDispatch(TRUE), NULL);
}

LPDISPATCH CXTPCalendarViewEvent::OleGetViewGroup()
{
	return XTP_SAFE_GET1(GetViewGroup_(), GetIDispatch(TRUE), NULL);
}

//////////////////////////////////////////////////////////////////////////
//  CXTPCalendarViewEvents
//

BEGIN_DISPATCH_MAP(CXTPCalendarViewEvents, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarViewEvents, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarViewEvents, "ViewEvent", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarViewEvents, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarViewEvents, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarViewEvents, XTPDIID_CalendarViewEvents, Dispatch)
	// INTERFACE_PART(CXTPCalendarViewEvents, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarViewEvents, XTPDIID_CalendarViewEvents)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarViewEvents)

#endif
