// XTPCalendarWeekViewDay.cpp: implementation of the CXTPCalendarWeekViewDay class.
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

#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarWeekViewEvent.h"
#include "Calendar/XTPCalendarWeekViewDay.h"
#include "Calendar/XTPCalendarWeekView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

CXTPCalendarWeekViewGroup::CXTPCalendarWeekViewGroup(CXTPCalendarWeekViewDay* pViewDay)
	: TBase(pViewDay)
{
}
CXTPCalendarWeekViewGroup::~CXTPCalendarWeekViewGroup()
{
}

void CXTPCalendarWeekViewGroup::FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	ASSERT(pHitTest && GetViewDay());
	if (pHitTest && GetViewDay())
	{
		GetViewDay()->FillHitTestEx(pHitTest);
		pHitTest->pViewGroup = (CXTPCalendarViewGroup*)this;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPCalendarWeekViewDay, CXTPCalendarViewDay)

CXTPCalendarWeekViewDay::CXTPCalendarWeekViewDay(CXTPCalendarWeekView* pWeekView)
	: TBase(pWeekView)
{
}

CXTPCalendarWeekViewDay::~CXTPCalendarWeekViewDay()
{
}

void CXTPCalendarWeekViewDay::FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pInfo) const
{
	ASSERT(pInfo);
	if (!pInfo)
	{
		return;
	}
	pInfo->dt		= m_dtDate;
	pInfo->pViewDay = (CXTPCalendarViewDay*)this;
	pInfo->uHitCode = xtpCalendarHitTestEvent_Mask;
}

BOOL CXTPCalendarWeekViewDay::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_Layout.m_rcExpandSign.PtInRect(pt))
	{
		if (XTP_SAFE_GET4(GetView(), GetTheme(), GetWeekViewPart(), GetDayPart(),
						  HitTestExpandDayButton(this, &pt), 0))
		{
			FillHitTestEx(pHitTest);
			pHitTest->uHitCode = xtpCalendarHitTestDayExpandButton;
			return TRUE;
		}
	}

	return TBase::HitTestEx(pt, pHitTest);
}

void CXTPCalendarWeekViewDay::AdjustLayout(CDC* pDC, const CRect& rcDay)
{
	if (!GetView() || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	int nHeaderHeight = GetView()->GetDayHeaderHeight();

	m_Layout.m_rcDay.CopyRect(&rcDay);
	m_Layout.m_rcDayHeader.CopyRect(&rcDay);
	m_Layout.m_rcDayHeader.bottom = m_Layout.m_rcDayHeader.top + nHeaderHeight;
	m_Layout.m_rcDayHeader.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), 0);

	CRect rcDayEvents = rcDay;
	rcDayEvents.DeflateRect(XTP_DPI_X(1), nHeaderHeight, XTP_DPI_X(1), XTP_DPI_Y(1));

	//-------------------------------------
	int nGroups = GetViewGroupsCount();
	ASSERT(nGroups == 1);

	CXTPCalendarViewGroup* pViewGroup = nGroups ? GetViewGroup_(0) : NULL;
	ASSERT(pViewGroup);

	if (pViewGroup)
	{
		pViewGroup->AdjustLayout(pDC, rcDayEvents);
	}
}
void CXTPCalendarWeekViewDay::AdjustLayout2(CDC* pDC, const CRect& rcDay)
{
	TBase::AdjustLayout(pDC, rcDay);

	XTP_SAFE_CALL4(GetView(), GetTheme(), GetWeekViewPart(), GetDayPart(),
				   AdjustLayout(this, pDC, rcDay));
}

void CXTPCalendarWeekViewDay::Draw(CDC* pDC)
{
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
		CXTPCalendarWeekView* pView = GetView();
		if (pView)
		{
			CXTPCalendarPaintManager* pPaintManager = pView->GetPaintManager();
			if (pPaintManager)
			{
				pPaintManager->DrawExpandSign(pDC, FALSE, GetExpandSignRect());
			}
		}
	}
}

CRect CXTPCalendarWeekViewDay::GetDayEventsRect() const
{
	int nColHeaderHeight = XTP_SAFE_GET1(GetView(), GetRowHeight(), 0);
	CRect rcDayEvents	= m_Layout.m_rcDay;
	// rcDayEvents.bottom -= min(2, rcDayEvents.Height());
	rcDayEvents.top += min(nColHeaderHeight + 0, rcDayEvents.Height());
	int nBotSpace = XTP_SAFE_GET5(GetView(), GetCalendarControl(), GetTheme(), GetWeekViewPart(),
								  GetDayPart(), GetExpandButtonHeight(), 0);

	rcDayEvents.bottom -= min(nBotSpace + XTP_DPI_Y(2), rcDayEvents.Height());

	return rcDayEvents;
}

BOOL CXTPCalendarWeekViewDay::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!GetView() || !GetCalendarControl())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (GetView()->GetTheme())
	{
		if (XTP_SAFE_GET4(GetView(), GetTheme(), GetWeekViewPart(), GetDayPart(),
						  OnLButtonDown(this, nFlags, point), FALSE))
		{
			return TRUE;
		}
	}
	else if (m_Layout.m_rcExpandSign.PtInRect(point))
	{
		if (UserAction_OnExpandDay(xtpCalendarExpandDayButton_WeekView))
			return TRUE;

		XTP_SAFE_CALL1(GetCalendarControl(), QueueDayViewSwitch(GetDayDate()));
		return TRUE;
	}

	return TBase::OnLButtonDown(nFlags, point);
}

void CXTPCalendarWeekViewDay::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetView() && GetView()->GetTheme() && GetView()->GetTheme()->GetWeekViewPart()
		&& GetView()->GetTheme()->GetWeekViewPart()->GetDayPart())
		GetView()->GetTheme()->GetWeekViewPart()->GetDayPart()->OnMouseMove(this, nFlags, point);

	TBase::OnMouseMove(nFlags, point);
}

CString CXTPCalendarWeekViewDay::GetCaption() const
{
	if (!GetView() || !GetCalendarControl())
	{
		ASSERT(FALSE);
		return _T("");
	}

	CString strHeaderFormat = GetView()->GetDayHeaderFormat();

	if (strHeaderFormat == _T("MMMM d"))
		strHeaderFormat = _T("dddd MMMM dd");
	else if (strHeaderFormat == _T("d MMMM"))
		strHeaderFormat = _T("dddd dd MMMM");

	SYSTEMTIME st;
	COleDateTime dtDay = GetDayDate();
	CXTPCalendarUtils::GetAsSystemTime(dtDay, st);
	CString strDate = CXTPCalendarUtils::GetDateFormat(&st, strHeaderFormat);

	//-------------------------------------------------------------
	DWORD dwFlags = GetCalendarControl()->GetAskItemTextFlags();

	if (dwFlags & xtpCalendarItemText_WeekViewDayHeader)
	{
		XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
		::ZeroMemory(&objRequest, sizeof(objRequest));

		objRequest.nItem	= (int)xtpCalendarItemText_WeekViewDayHeader;
		objRequest.pstrText = &strDate;
		objRequest.pViewDay = (CXTPCalendarViewDay*)this;

		GetCalendarControl()->SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT,
													 (WPARAM)&objRequest, 0);
	}
	//-------------------------------------------------------------

	return strDate;
}
