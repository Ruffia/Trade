// XTPCalendarTimeLineView.cpp: implementation of the CXTPCalendarTimeLineView class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarEvents.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarTimeLineViewGroup.h"
#include "Calendar/XTPCalendarTimeLineView.h"
#include "Calendar/XTPCalendarTimeLineViewTimeScale.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPCalendarTimeLineView, CXTPCalendarView)

CXTPCalendarTimeLineView::CXTPCalendarTimeLineView(CXTPCalendarControl* pCalendarControl)
	: CXTPCalendarView(pCalendarControl)
{
	m_pTimeScale	 = new CXTPCalendarTimeLineViewTimeScale(this);
	m_pTimeScaleGrid = new CXTPCalendarTimeLineViewTimeScaleGrid(this);

	m_nHScrollPos = -1;
	m_nVScrollPos = 0;

	m_rcLastPopulate.SetRect(0, 0, 0, 0);

	RefreshMetrics();

	m_bTimeLineSeparateGroup = FALSE;

	m_nTimelineMaxPixelsForEvent		   = XTP_DPI_X(50);
	m_nTimelineMaxPixelsForEvent_ScaleDay  = XTP_DPI_X(200);
	m_nTimelineMaxPixelsForEvent_ScaleWeek = XTP_DPI_X(100);

	m_pGroups = new CXTPCalendarTimeLineViewGroups();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

BOOL CXTPCalendarTimeLineView::HitTestEx(CPoint pt,
										 XTP_CALENDAR_HITTESTINFO_TIMELINE_VIEW* pHitTest) const
{
	UNREFERENCED_PARAMETER(pt);
	UNREFERENCED_PARAMETER(pHitTest);
	return 0;
}

void CXTPCalendarTimeLineView::RefreshMetrics()
{
	CXTPCalendarPaintManager* pPaintManager = GetCalendarControl()->GetPaintManager();

	if (pPaintManager)
	{
		m_clrEventBar			 = pPaintManager->m_clrTLVEventBar;
		m_clrEventBarLine		 = pPaintManager->m_clrTLVEventBarLine;
		m_clrEventBorderLine	 = pPaintManager->m_clrTLVEventBorderLine;
		m_clrTimeScaleBackground = pPaintManager->m_clrTLVTimeScaleBackground;
		m_clrTimeScaleBorder	 = pPaintManager->m_clrTLVTimeScaleBorder;
		m_clrTimeScaleHighlight  = pPaintManager->m_clrTLVTimeScaleHighlight;
		m_clrTimeScaleText		 = pPaintManager->m_clrTLVTimeScaleText;
		m_clrText				 = pPaintManager->m_clrTLVText;
		m_clrBackground			 = pPaintManager->m_clrTLVBack;
		m_clrSelectedText		 = pPaintManager->m_clrTLVSelectedText;
		m_clrSelectedBackground  = pPaintManager->m_clrTLVSelectedBack;
		m_clrWeekendBackground   = pPaintManager->m_clrTLVWeekendBack;
	}
	else
	{
		m_clrEventBar			 = GetXtremeColor(COLOR_ACTIVECAPTION);
		m_clrEventBarLine		 = GetXtremeColor(COLOR_INACTIVECAPTION);
		m_clrEventBorderLine	 = GetXtremeColor(COLOR_INACTIVEBORDER);
		m_clrTimeScaleBackground = GetXtremeColor(COLOR_BTNFACE);
		m_clrTimeScaleBorder	 = GetXtremeColor(COLOR_3DDKSHADOW);
		m_clrTimeScaleHighlight  = GetXtremeColor(COLOR_HIGHLIGHT);
		m_clrTimeScaleText		 = GetXtremeColor(COLOR_WINDOWTEXT);
		m_clrText				 = GetXtremeColor(COLOR_WINDOWTEXT);
		m_clrBackground			 = GetXtremeColor(COLOR_WINDOW);
		m_clrSelectedText		 = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		m_clrSelectedBackground  = GetXtremeColor(COLOR_3DSHADOW);
		m_clrWeekendBackground   = GetXtremeColor(COLOR_WINDOW);
	}
}

CXTPCalendarTimeLineView::~CXTPCalendarTimeLineView()
{
	OnBeforeDestroy();
	delete m_pTimeScaleGrid;
	CMDTARGET_RELEASE(m_pTimeScale);
	CMDTARGET_RELEASE(m_pGroups);
}

void CXTPCalendarTimeLineView::OnBeforeDestroy()
{
	TBase::OnBeforeDestroy();
}

void CXTPCalendarTimeLineView::AdjustLayout(CDC* pDC, const CRect& rcView,
											BOOL bCallPostAdjustLayout)
{
	RefreshMetrics();

	TBase::AdjustLayout(pDC, rcView, bCallPostAdjustLayout);

	if (!m_pTimeScale)
	{
		ASSERT(FALSE);
		return;
	}

	m_pTimeScale->AdjustLayout(rcView);

	CRect rcTimeScaleGrid = rcView;
	rcTimeScaleGrid.top   = GetTimeScale()->GetRect().bottom + 1; // 1 pixel
	m_pTimeScaleGrid->AdjustLayout(rcTimeScaleGrid);

	if (m_rcLastPopulate != rcView)
		Populate();

	CRect rcGroup = rcView;
	rcGroup.top   = GetTimeScale()->GetRect().bottom + XTP_DPI_Y(2);

	int nGroups = m_pGroups->GetCount();
	for (int i = 0; i < nGroups; i++)
	{
		m_pGroups->GetAt(i)->AdjustLayout(pDC, rcGroup);

		rcGroup.top = m_pGroups->GetAt(i)->GetRect().bottom;
	}
}

void CXTPCalendarTimeLineView::Populate()
{
	if (IsEditingSubject())
	{
		EndEditSubject(xtpCalendarEditSubjectCommit);
	}

	COleDateTime m_dtPopulatedStartDay;
	COleDateTime m_dtPopulatedEndDay;

	// This 45 days left and right extra range will prevent event rect from jumps during scroll
	COleDateTimeSpan dtOffset(45.0);

	m_dtPopulatedStartDay		   = CXTPCalendarUtils::ResetTime(GetStartViewDate()) - dtOffset;
	COleDateTimeSpan spTimeperPix  = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();
	COleDateTimeSpan spVisibleTime = (double)GetViewRect().Width() * (double)spTimeperPix;

	m_dtPopulatedEndDay = GetStartViewDate() + spVisibleTime;
	m_dtPopulatedEndDay = CXTPCalendarUtils::ResetTime(m_dtPopulatedEndDay) + dtOffset;

	CXTPCalendarEventsPtr ptrEvents = _GetEvents(m_dtPopulatedStartDay, m_dtPopulatedEndDay);
	if (ptrEvents)
		ptrEvents->Sort(CXTPCalendarEvents::CompareEvents_ForViewByStart);

	m_pGroups->RemoveAll();

	m_pGroups->Add(new CXTPCalendarTimeLineViewGroup(this));

	m_pGroups->GetAt(0)->Populate(ptrEvents);

	_ReSelectSelectEvents();

	m_rcLastPopulate = GetViewRect();
}

void CXTPCalendarTimeLineView::Draw(CDC* pDC)
{
	CRect rcClient = GetViewRect();

	pDC->FillSolidRect(&rcClient, m_clrBackground);

	if (!m_pTimeScale)
	{
		ASSERT(FALSE);
		return;
	}
	m_pTimeScale->Draw(pDC);
	m_pTimeScaleGrid->Draw(pDC);

	int nOffset = m_nVScrollPos;
	int nGroups = m_pGroups->GetCount();
	for (int i = 0; i < nGroups; i++)
	{
		CXTPCalendarTimeLineViewGroup* pGroup = m_pGroups->GetAt(i);
		if (pGroup)
		{
			pGroup->Draw(pDC, nOffset);

			CRect rcLine  = rcClient;
			rcLine.top	= pGroup->GetGroupHeight();
			rcLine.bottom = rcLine.top + XTP_DPI_Y(2);
			if (m_bTimeLineSeparateGroup)
				pDC->FillSolidRect(&rcLine, m_clrEventBar);
		}
	}
}

void CXTPCalendarTimeLineView::Draw2(CDC* pDC)
{
	Draw(pDC);
}

BOOL CXTPCalendarTimeLineView::GetScrollBarInfoV(SCROLLINFO* pSI) const
{
	int nScroll = GetSystemMetrics(SM_CYHSCROLL);
	int h(0);
	if (m_pTimeScale)
		h = m_pTimeScale->GetClientRect().Height();
	int H	 = GetViewRect().Height();
	pSI->nMax = H - nScroll - h;

	int nGroups = m_pGroups->GetCount();
	int iBottom(0);
	for (int i = 0; i < nGroups; i++)
		iBottom = max(iBottom, m_pGroups->GetAt(i)->GetRect().bottom);

	if (iBottom > H)
	{
		pSI->nPage = XTPToUIntChecked(H);
		pSI->nMax  = iBottom;
		pSI->nPos  = m_nVScrollPos;
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPCalendarTimeLineView::GetScrollBarInfoH(SCROLLINFO* pSI, int* pnScrollStep) const
{
	int nScroll = GetSystemMetrics(SM_CXVSCROLL);
	pSI->nMin   = 0;
	pSI->nMax   = GetViewRect().Width() - nScroll;

	int iTimeScaleID = GetTimeScaleID();

	if (iTimeScaleID == xtpTSPID_Month)
		pSI->nPage = 0 < pSI->nMax ? XTPToUIntChecked(pSI->nMax * 0.05) : 0; // approx 8 days
	else if (iTimeScaleID == xtpTSPID_Week)
		pSI->nPage = 0 < pSI->nMax ? XTPToUIntChecked(pSI->nMax * 0.05) : 0;
	else if (iTimeScaleID == xtpTSPID_Day)
		pSI->nPage = 0 < pSI->nMax ? XTPToUIntChecked(pSI->nMax * 0.05) : 0;

	pSI->nPos = m_nHScrollPos;

	if (pnScrollStep)
		*pnScrollStep = 4;

	return TRUE;
}

void CXTPCalendarTimeLineView::ScrollH(int nPos, int nPos_raw)
{
	UNREFERENCED_PARAMETER(nPos_raw);

	COleDateTime dtStart = GetStartViewDate();

	COleDateTimeSpan spTimeperPix = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();

	int nShift = (nPos - m_nHScrollPos) * 4; // si.nPos;

	COleDateTimeSpan spShift = nShift * (double)spTimeperPix;

#ifdef TRACE_EDITING_ON
	CString s_Start		 = dtStart.Format(_T("%d %m %Y %H %M"));
	CString s_TimeperPix = spTimeperPix.Format(_T("%H %M %S"));
	s_TimeperPix.Format(_T("D=%f, H=%f M=%f S=%f"), spTimeperPix.GetTotalDays(),
						spTimeperPix.GetTotalHours(), spTimeperPix.GetTotalMinutes(),
						spTimeperPix.GetTotalSeconds());

	CString s_Shift = spShift.Format(_T("%H %M %S"));
	s_Shift.Format(_T("D=%f, H=%f M=%f S=%f"), spShift.GetTotalDays(), spShift.GetTotalHours(),
				   spShift.GetTotalMinutes(), spShift.GetTotalSeconds());
#endif

	dtStart += spShift;

	SetStartViewDate(dtStart);

	SCROLLINFO si = { 0 };
	GetScrollBarInfoH(&si);

	m_nHScrollPos = nPos;

	GetCalendarControl()->AdjustScrollBar(SB_VERT);
	GetCalendarControl()->RefreshCaptionBar();
}

void CXTPCalendarTimeLineView::ScrollV(int nPos, int nPos_raw)
{
	m_nVScrollPos = nPos;

	UNREFERENCED_PARAMETER(nPos_raw);
}

void CXTPCalendarTimeLineView::OnScrollEnd(int nBar)
{
	if (nBar == SB_HORZ)
	{
		SCROLLINFO si = { 0 };
		GetScrollBarInfoH(&si);

		int nBorder   = (int)si.nPage;
		m_nHScrollPos = max(m_nHScrollPos, si.nMin + nBorder);
		m_nHScrollPos = min(m_nHScrollPos, si.nMax - nBorder - (int)si.nPage);

		GetCalendarControl()->AdjustScrollBar(SB_HORZ);
	}
}

CXTPCalendarEventsPtr CXTPCalendarTimeLineView::_GetEvents(COleDateTime dtStartDay,
														   COleDateTime dtEndDay)
{
	// TODO: change for multiresources.
	CXTPCalendarData* pData			= GetCalendarControl()->GetDataProvider();
	CXTPCalendarEventsPtr ptrEvents = new CXTPCalendarEvents();

	ASSERT(pData);
	if (!pData || !ptrEvents)
		return NULL;

	COleDateTimeSpan spDay(1, 0, 0, 0);

	COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dtStartDay);
	for (; dtDay <= dtEndDay; dtDay += spDay)
	{
		CXTPCalendarEventsPtr ptrDayEvents = pData->RetrieveDayEvents(dtDay);

		int nCount = ptrDayEvents ? ptrDayEvents->GetCount() : 0;
		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarEvent* pEv = ptrDayEvents->GetAt(i);

			if (ptrEvents->FindEvent(pEv) == NULL)
			{
				ptrEvents->Add(pEv, TRUE);
			}
		}
	}

	return ptrEvents;
}

int CXTPCalendarTimeLineView::GetTimeScaleID() const
{
	return XTP_SAFE_GET2(m_pTimeScale, GetTimeScaleParams(), GetTimeScaleParamsID(),
						 xtpTSPID_Unknown);
}

void CXTPCalendarTimeLineView::SetTimeScaleID(int eTimeScaleParamsID)
{
	if (m_pTimeScale)
	{
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateAll);

		m_pTimeScale->SetTimeScaleParams(eTimeScaleParamsID);
	}
}

BOOL CXTPCalendarTimeLineView::HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (pHitTest)
	{
		CPoint point(pt);
		if (m_pTimeScale->HitTestEx(pt, pHitTest))
			pHitTest->uHitCode = xtpCalendarHitTestTimeLineScale;

		GetCalendarControl()->ScreenToClient(&pt);
		COleDateTime dt = GetStartViewDate();
		if (m_pTimeScale)
		{
			COleDateTimeSpan spTimeperPix = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();
			spTimeperPix				  = spTimeperPix * pt.x;
			if (spTimeperPix.m_status == COleDateTimeSpan::valid)
				dt += spTimeperPix;
		}
		pHitTest->dt		 = dt;
		pHitTest->nGroup	 = -1;
		pHitTest->nEvent	 = 0;
		pHitTest->pViewEvent = NULL;

		int nGroups = m_pGroups->GetCount();
		for (int nGroup = 0; nGroup < nGroups; nGroup++)
		{
			CXTPCalendarTimeLineViewGroup* pGroup = m_pGroups->GetAt(nGroup);
			if (pGroup)
			{
				int nCount = pGroup->GetViewEventsCount();
				for (int nEvent = 0; nEvent < nCount; nEvent++)
				{
					CXTPCalendarTimeLineViewEvent* pViewEv = pGroup->GetViewEvent(nEvent);
					if (pViewEv)
					{
						CRect rc = pViewEv->GetLastVisibleRect();
						if (rc.PtInRect(point))
						{
							pHitTest->nGroup	 = nEvent;
							pHitTest->nEvent	 = XTPToInt(pViewEv->GetEvent()->GetEventID());
							pHitTest->pViewEvent = pViewEv;
							return TRUE;
						}
					}
				}
			}
		}

		return TRUE;
	}
	return FALSE;
}

CRect CXTPCalendarTimeLineView::GetTooltipRect(const CPoint& ptHit,
											   const XTP_CALENDAR_HITTESTINFO& hitInfo) const
{
	CRect rcTip(0, 0, 0, 0);
	CPoint pt(ptHit);
	if (hitInfo.pViewEvent)
	{
		rcTip		= hitInfo.pViewEvent->m_rcEvent;
		int w		= rcTip.Width();
		rcTip.left  = pt.x;
		rcTip.right = rcTip.left + w;
	}
	return rcTip;
}

static BOOL m_bWeHavePopup = FALSE;

void CXTPCalendarTimeLineView::ShowCalendarPopup(COleDateTime dt, CPoint point)
{
	GetCalendarControl()->ClientToScreen(&point);
	CPoint pH = m_pTimeScale->m_rcScale.BottomRight();
	GetCalendarControl()->ClientToScreen(&pH);
	// if we follows Outlook spec and only top (title) part of scale call popup

	if (point.y > pH.y - XTP_DPI_Y(31))
	{
		m_bWeHavePopup = FALSE;
		return;
	}

	// if we follows Outlook spec and looking for extra click if Popup visible
	if (m_bWeHavePopup) // cycle by 2
	{
		m_bWeHavePopup = FALSE;
		return;
	}
	m_bWeHavePopup = TRUE;

	COleDateTime rdtDate(dt);
	CXTPDatePickerControl datePicker;

	datePicker.SetPaintTheme(GetPaintManager()->GetPaintTheme());

	CRect rc;
	datePicker.GetMinReqRect(&rc);
	rc.InflateRect(XTP_DPI_X(5), XTP_DPI_Y(5));

	int rWidth  = rc.Width();
	int rHeight = rc.Height();

	CRect rcDesk;

	//<<MULTI-MONITOR case fix>>
	//  GetCalendarControl()->GetDesktopWindow()->GetClientRect(&rcDesk);
	GetCalendarControl()->GetDesktopWindow()->GetWindowRect(&rcDesk);
	GetCalendarControl()->ClientToScreen(&rcDesk);

	if (point.x + rWidth > rcDesk.right)
		point.x -= rWidth;

	// if we follows Outlook spec and keep fixed vert pos!
	point.y = pH.y - XTP_DPI_Y(31);

	rc.left   = point.x;
	rc.top	= point.y;
	rc.right  = rc.left + rWidth;
	rc.bottom = rc.top + rHeight;

	datePicker.SetButtonsVisible(TRUE, FALSE);
	datePicker.SetShowWeekNumbers(FALSE);
	datePicker.SetBorderStyle(xtpDatePickerBorderOffice);

	XTPCalendarTheme ct = GetCalendarControl()->GetPaintTheme();

	datePicker.SetPaintTheme(ct);
	datePicker.GetPaintManager()->EnableTheme(TRUE);

	datePicker.Select(rdtDate);
	datePicker.SetToday(GetStartViewDate());
	datePicker.EnsureVisibleSelection();

	if (datePicker.GoModal(rc, GetCalendarControl()))
	{
		if (datePicker.GetSelRange(dt, rdtDate))
		{
			if (dt == rdtDate)
			{
			}
			else
			{
			}
		}
		COleDateTime dtNow(rdtDate);
		SetStartViewDate(dtNow);

		OnScrollEnd(SB_HORZ);
	}
	else
	{
	}
	CPoint Point;
	::GetCursorPos(&Point);
	CRect rcClient;
	GetCalendarControl()->GetClientRect(&rcClient);
	GetCalendarControl()->ClientToScreen(&rcClient);

	// if we follows Outlook spec and only top (title) part of scale call popup
	if (!rcClient.PtInRect(Point))
		m_bWeHavePopup = FALSE;

	GetCalendarControl()->ClientToScreen(&Point);
	if (Point.y <= pH.y)
		m_bWeHavePopup = FALSE;
}

COleDateTime CXTPCalendarTimeLineView::GetStartViewDate() const
{
	ASSERT(m_pTimeScale);
	return m_pTimeScale ? m_pTimeScale->GetStartViewDate() : COleDateTime::GetCurrentTime();
}

void CXTPCalendarTimeLineView::SetStartViewDate(COleDateTime dt)
{
	ASSERT(m_pTimeScale);
	if (m_pTimeScale)
		m_pTimeScale->SetStartViewDate(dt);

	Populate();
}

COleDateTime CXTPCalendarTimeLineView::GetViewDayDate(int nIndex) const
{
	UNREFERENCED_PARAMETER(nIndex);
	return CXTPCalendarUtils::ResetTime(GetStartViewDate());
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarTimeLineView, CXTPCalendarView)

	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "MaxPixelsForEvent", 1, OleGetMaxPixelsForEvent,
						OleSetMaxPixelsForEvent, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "MaxPixelsForEvent_ScaleDay", 2,
						OleGetMaxPixelsForEvent_ScaleDay, OleSetMaxPixelsForEvent_ScaleDay, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "MaxPixelsForEvent_ScaleWeek", 3,
						OleGetMaxPixelsForEvent_ScaleWeek, OleSetMaxPixelsForEvent_ScaleWeek, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "SeparateGroup", 4, OleGetTimeLineSeparateGroup,
						OleSetTimeLineSeparateGroup, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "EventBarColor", 5, OleGetTimeLineEventBarColor,
						OleSetTimeLineEventBarColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "EventBarLineColor", 6,
						OleGetTimeLineEventBarLineColor, OleSetTimeLineEventBarLineColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "ScaleBackgroundColor", 7,
						OleGetTimeLineScaleBackgroundColor, OleSetTimeLineScaleBackgroundColor,
						VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "ScaleBorderColor", 8,
						OleGetTimeLineScaleBorderColor, OleSetTimeLineScaleBorderColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "ScaleHighlightColor", 9,
						OleGetTimeLineScaleHighlightColor, OleSetTimeLineScaleHighlightColor,
						VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "ScaleTextColor", 10,
						OleGetTimeLineScaleTextColor, OleSetTimeLineScaleTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "SelectedBackgroundColor", 11,
						OleGetTimeLineSelectedBackgroundColor,
						OleSetTimeLineSelectedBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "SelectedTextColor", 12,
						OleGetTimeLineSelectedTextColor, OleSetTimeLineSelectedTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "BackgroundColor", 13,
						OleGetTimeLineBackgroundColor, OleSetTimeLineBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "TextColor", 14, OleGetTimeLineTextColor,
						OleSetTimeLineTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "TimeScaleMinTime", 15, OleGetTimeScaleMinTime,
						OleSetTimeScaleMinTime, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "TimeScaleMaxTime", 16, OleGetTimeScaleMaxTime,
						OleSetTimeScaleMaxTime, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "TimeScaleID", 17, GetTimeScaleID, SetTimeScaleID,
						VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "EventBorderLineColor", 18,
						OleGetTimeLineEventBorderLineColor, OleSetTimeLineEventBorderLineColor,
						VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeLineView, "EventWeekendBackgroundColor", 19,
						OleGetTimeLineWeekendBackgroundColor, OleSetTimeLineWeekendBackgroundColor,
						VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarTimeLineView, CXTPCalendarView)
	INTERFACE_PART(CXTPCalendarTimeLineView, XTPDIID_CalendarTimeLineView, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarTimeLineView, XTPDIID_CalendarTimeLineView)

BOOL CXTPCalendarTimeLineView::OleGetTimeLineSeparateGroup()
{
	return m_bTimeLineSeparateGroup;
}

void CXTPCalendarTimeLineView::OleSetTimeLineSeparateGroup(BOOL bSet)
{
	m_bTimeLineSeparateGroup = bSet;
}

long CXTPCalendarTimeLineView::OleGetMaxPixelsForEvent()
{
	return m_nTimelineMaxPixelsForEvent;
}

void CXTPCalendarTimeLineView::OleSetMaxPixelsForEvent(long nPixels)
{
	m_nTimelineMaxPixelsForEvent = nPixels;
}

long CXTPCalendarTimeLineView::OleGetMaxPixelsForEvent_ScaleDay()
{
	return m_nTimelineMaxPixelsForEvent_ScaleDay;
}

void CXTPCalendarTimeLineView::OleSetMaxPixelsForEvent_ScaleDay(long nPixels)
{
	m_nTimelineMaxPixelsForEvent_ScaleDay = nPixels;
}

long CXTPCalendarTimeLineView::OleGetMaxPixelsForEvent_ScaleWeek()
{
	return m_nTimelineMaxPixelsForEvent_ScaleWeek;
}

void CXTPCalendarTimeLineView::OleSetMaxPixelsForEvent_ScaleWeek(long nPixels)
{
	m_nTimelineMaxPixelsForEvent_ScaleWeek = nPixels;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineEventBarColor()
{
	return (OLE_COLOR)m_clrEventBar;
}

void CXTPCalendarTimeLineView::OleSetTimeLineEventBarColor(OLE_COLOR clrOleColor)
{
	m_clrEventBar = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineEventBarLineColor()
{
	return (OLE_COLOR)m_clrEventBarLine;
}

void CXTPCalendarTimeLineView::OleSetTimeLineEventBarLineColor(OLE_COLOR clrOleColor)
{
	m_clrEventBarLine = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineEventBorderLineColor()
{
	return (OLE_COLOR)m_clrEventBorderLine;
}

void CXTPCalendarTimeLineView::OleSetTimeLineEventBorderLineColor(OLE_COLOR clrOleColor)
{
	m_clrEventBorderLine = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineScaleBackgroundColor()
{
	return (OLE_COLOR)m_clrTimeScaleBackground;
}

void CXTPCalendarTimeLineView::OleSetTimeLineScaleBackgroundColor(OLE_COLOR clrOleColor)
{
	m_clrTimeScaleBackground = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineScaleBorderColor()
{
	return (OLE_COLOR)m_clrTimeScaleBorder;
}

void CXTPCalendarTimeLineView::OleSetTimeLineScaleBorderColor(OLE_COLOR clrOleColor)
{
	m_clrTimeScaleBorder = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineScaleHighlightColor()
{
	return (OLE_COLOR)m_clrTimeScaleHighlight;
}

void CXTPCalendarTimeLineView::OleSetTimeLineScaleHighlightColor(OLE_COLOR clrOleColor)
{
	m_clrTimeScaleHighlight = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineScaleTextColor()
{
	return (OLE_COLOR)m_clrTimeScaleText;
}

void CXTPCalendarTimeLineView::OleSetTimeLineScaleTextColor(OLE_COLOR clrOleColor)
{
	m_clrTimeScaleText = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineSelectedBackgroundColor()
{
	return (OLE_COLOR)m_clrSelectedBackground;
}

void CXTPCalendarTimeLineView::OleSetTimeLineSelectedBackgroundColor(OLE_COLOR clrOleColor)
{
	m_clrSelectedBackground = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineWeekendBackgroundColor()
{
	return (OLE_COLOR)m_clrWeekendBackground;
}

void CXTPCalendarTimeLineView::OleSetTimeLineWeekendBackgroundColor(OLE_COLOR clrOleColor)
{
	m_clrWeekendBackground = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineSelectedTextColor()
{
	return (OLE_COLOR)m_clrSelectedText;
}

void CXTPCalendarTimeLineView::OleSetTimeLineSelectedTextColor(OLE_COLOR clrOleColor)
{
	m_clrSelectedText = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineBackgroundColor()
{
	return m_clrBackground;
}

void CXTPCalendarTimeLineView::OleSetTimeLineBackgroundColor(OLE_COLOR clrOleColor)
{
	m_clrBackground = (COLORREF)clrOleColor;
}

OLE_COLOR CXTPCalendarTimeLineView::OleGetTimeLineTextColor()
{
	return (OLE_COLOR)m_clrText;
}

void CXTPCalendarTimeLineView::OleSetTimeLineTextColor(OLE_COLOR clrOleColor)
{
	m_clrText = (COLORREF)clrOleColor;
}

DATE CXTPCalendarTimeLineView::OleGetTimeScaleMinTime()
{
	return GetStartViewDate();
}

void CXTPCalendarTimeLineView::OleSetTimeScaleMinTime(DATE dtNewScaleMinTime)
{
	SetStartViewDate(dtNewScaleMinTime);
}

DATE CXTPCalendarTimeLineView::OleGetTimeScaleMaxTime()
{
	COleDateTime dtS = GetStartViewDate();
	if (m_pTimeScale)
	{
		COleDateTimeSpan spTimeperPix = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();
		spTimeperPix				  = spTimeperPix * m_pTimeScale->m_rcScale.Width();
		if (spTimeperPix.m_status == COleDateTimeSpan::valid)
			dtS += spTimeperPix;
	}
	return dtS;
}

void CXTPCalendarTimeLineView::OleSetTimeScaleMaxTime(DATE dtNewScaleMaxTime)
{
	//  SetScaleMaxTime(dtNewScaleMaxTime);
}

#endif
