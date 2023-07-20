// XTPCalendarMonthViewDay.cpp: implementation of the CXTPCalendarMonthViewDay class.
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

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
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

IMPLEMENT_DYNAMIC(CXTPCalendarMonthViewDay, CXTPCalendarViewDay)
//////////////////////////////////////////////////////////////////////////
CXTPCalendarMonthViewGroup::CXTPCalendarMonthViewGroup(CXTPCalendarMonthViewDay* pViewDay)
	: TBase(pViewDay)
{
}

CXTPCalendarMonthViewGroup::~CXTPCalendarMonthViewGroup()
{
}

void CXTPCalendarMonthViewGroup::FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	ASSERT(pHitTest && GetViewDay());
	if (pHitTest && GetViewDay())
	{
		GetViewDay()->FillHitTestEx(pHitTest);
		pHitTest->pViewGroup = (CXTPCalendarViewGroup*)this;
	}
}

CRect CXTPCalendarMonthViewGroup::GetTooltipRect(const CPoint& ptHit,
												 const XTP_CALENDAR_HITTESTINFO& hitInfo) const
{
	UNREFERENCED_PARAMETER(ptHit);
	UNREFERENCED_PARAMETER(hitInfo);

	CRect rcTip(m_Layout.m_rcGroup);
	rcTip.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), 0, 0);
	int nColHeaderHeight = XTP_SAFE_GET2(GetViewDay(), GetView(), GetRowHeight(), 0);
	rcTip.top			 = rcTip.bottom - nColHeaderHeight;

	return rcTip;
}

//////////////////////////////////////////////////////////////////////////

CXTPCalendarMonthViewDay::CXTPCalendarMonthViewDay(const CXTPCalendarMonthView* pMonthView,
												   int nWeekIndex, int nWeekDayIndex)
	: TBase(pMonthView)
{
	ASSERT(pMonthView);

	m_nWeekIndex	= nWeekIndex;
	m_nWeekDayIndex = nWeekDayIndex;
}

CXTPCalendarMonthViewDay::~CXTPCalendarMonthViewDay()
{
}

void CXTPCalendarMonthViewDay::SetSelected()
{
	CXTPCalendarViewDay::SetSelected();
	if (GetCalendarControl()->GetThemeVersion() == xtpCalendarVersion2010)
		GetCalendarControl()->GetMonthView()->SetMDayViewLightColorBaseDate(GetDayDate());
}

void CXTPCalendarMonthViewDay::AdjustLayout(CDC* pDC, const CRect& rcDay)
{
	TBase::AdjustLayout(pDC, rcDay);

	m_Layout.m_rcDayHeader.CopyRect(CalcDayDateRect());

	CRect rcDayEvents = GetDayEventsRect();

	int nGroups = GetViewGroupsCount();
	ASSERT(nGroups == 1);

	CXTPCalendarViewGroup* pViewGroup = nGroups ? GetViewGroup_(0) : NULL;
	ASSERT(pViewGroup);

	if (pViewGroup)
	{
		pViewGroup->AdjustLayout(pDC, rcDayEvents);
	}
}

void CXTPCalendarMonthViewDay::AdjustLayout2(CDC* pDC, const CRect& rcDay)
{
	TBase::AdjustLayout(pDC, rcDay);

	GetView()->GetTheme()->GetMonthViewPart()->GetDayPart()->AdjustLayout(this, pDC, rcDay);
}

void CXTPCalendarMonthViewDay::Draw(CDC* pDC)
{
	if (!GetView() || !GetView()->GetPaintManager())
	{
		ASSERT(FALSE);
		return;
	}
	//-- Draw day date // '25' or  'July 25'

	BOOL bLong		= (m_nWeekIndex == 0 && m_nWeekDayIndex == 0);
	CString strDate = GetView()->_FormatDayDate(m_dtDate, bLong);

	BOOL bToday = XTP_SAFE_GET1(GetCalendarControl(), IsToday(m_dtDate), FALSE);

	//-------------------------------------------------------------
	DWORD dwFlags = XTP_SAFE_GET2(GetView(), GetCalendarControl(), GetAskItemTextFlags(), 0);

	if (dwFlags & xtpCalendarItemText_MonthViewDayHeader)
	{
		XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
		::ZeroMemory(&objRequest, sizeof(objRequest));

		objRequest.nItem	= (int)xtpCalendarItemText_MonthViewDayHeader;
		objRequest.pstrText = &strDate;
		objRequest.pViewDay = this;

		XTP_SAFE_CALL2(GetView(), GetCalendarControl(),
					   SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0));
	}
	//-------------------------------------------------------------

	XTP_SAFE_CALL3(GetView(), GetPaintManager(), GetMonthViewEventPart(),
				   OnDrawDayDate(pDC, m_Layout.m_rcDayHeader, bToday, IsSelected(),
								 m_dtDate.GetDay() == 1, strDate));

	//-- Draw Events (Group) -----------------
	int nGroups = GetViewGroupsCount();
	ASSERT(nGroups == 1);

	CXTPCalendarViewGroup* pViewGroup = nGroups ? GetViewGroup_(0) : NULL;
	ASSERT(pViewGroup);

	if (pViewGroup)
	{
		pViewGroup->Draw(pDC);
	}

	if (ContainsInvisibleEventsUpdate())
	{
		GetView()->GetPaintManager()->DrawExpandSign(pDC, FALSE, GetExpandSignRect());
	}
}

void CXTPCalendarMonthViewDay::FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pInfo) const
{
	ASSERT(pInfo);
	if (!pInfo)
	{
		return;
	}

	pInfo->dt					= m_dtDate;
	pInfo->dayPOS.nWeekIndex	= m_nWeekIndex;
	pInfo->dayPOS.nWeekDayIndex = m_nWeekDayIndex;
	pInfo->nDay					= m_nWeekIndex * 7 + m_nWeekDayIndex;
	pInfo->nEvent				= -1;
	pInfo->pViewDay				= (CXTPCalendarViewDay*)this;
	pInfo->pViewEvent			= NULL;

	pInfo->uHitCode = xtpCalendarHitTestUnknown;
}

BOOL CXTPCalendarMonthViewDay::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_Layout.m_rcExpandSign.PtInRect(pt))
	{
		if (XTP_SAFE_GET4(GetView(), GetTheme(), GetMonthViewPart(), GetDayPart(),
						  HitTestExpandDayButton(this, &pt), 0))
		{
			FillHitTestEx(pHitTest);
			pHitTest->uHitCode = xtpCalendarHitTestDayExpandButton;
			return TRUE;
		}
	}

	return TBase::HitTestEx(pt, pHitTest);
}

CRect CXTPCalendarMonthViewDay::CalcDayDateRect() const
{
	CRect rcDayDate = m_Layout.m_rcDay;
	rcDayDate.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), 0, 0);
	int nColHeaderHeight = XTP_SAFE_GET1(GetView(), GetRowHeight(), 0);
	rcDayDate.bottom	 = rcDayDate.top + min(nColHeaderHeight, rcDayDate.Height());

	return rcDayDate;
}

CRect CXTPCalendarMonthViewDay::GetDayEventsRect() const
{
	int nColHeaderHeight = m_Layout.m_rcDayHeader.Height();

	CRect rcDayEvents = m_Layout.m_rcDay;
	//  rcDayEvents.bottom -= min(2, rcDayEvents.Height());
	rcDayEvents.top += min(nColHeaderHeight, rcDayEvents.Height());

	int nBotSpace = XTP_SAFE_GET5(GetView(), GetCalendarControl(), GetTheme(), GetMonthViewPart(),
								  GetDayPart(), GetExpandButtonHeight(), 0);

	rcDayEvents.bottom -= min(nBotSpace + XTP_DPI_Y(2), rcDayEvents.Height());

	return rcDayEvents;
}

BOOL CXTPCalendarMonthViewDay::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!GetView() || !GetCalendarControl())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (GetView()->GetTheme())
	{
		if (XTP_SAFE_GET4(GetView(), GetTheme(), GetMonthViewPart(), GetDayPart(),
						  OnLButtonDown(this, nFlags, point), FALSE))
		{
			return TRUE;
		}
	}
	else if (m_Layout.m_rcExpandSign.PtInRect(point))
	{
		if (UserAction_OnExpandDay(xtpCalendarExpandDayButton_MonthView))
			return TRUE;

		XTP_SAFE_CALL1(GetCalendarControl(), QueueDayViewSwitch(GetDayDate()));
		return TRUE;
	}

	return TBase::OnLButtonDown(nFlags, point);
}

void CXTPCalendarMonthViewDay::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetView() && GetView()->GetTheme() && GetView()->GetTheme()->GetMonthViewPart()
		&& GetView()->GetTheme()->GetMonthViewPart()->GetDayPart())
		GetView()->GetTheme()->GetMonthViewPart()->GetDayPart()->OnMouseMove(this, nFlags, point);

	TBase::OnMouseMove(nFlags, point);
}
