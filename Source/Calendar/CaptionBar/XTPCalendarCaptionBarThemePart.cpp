// XTPCalendarCaptionBarThemePart.cpp : implementation file
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
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarThemePart.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarTheme.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarCaptionBarThemePart, CXTPCalendarWMHandler)

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarThemePart

CXTPCalendarCaptionBarThemePart::CXTPCalendarCaptionBarThemePart(
	CXTPCalendarCaptionBarTheme* pTheme, int nPartID)
{
	ASSERT(pTheme);
	m_pTheme   = pTheme;
	m_nPartID  = nPartID;
	m_nState   = xtpBtnStateNormal;
	m_bVisible = TRUE;

	if (m_pTheme)
		m_pTheme->m_arMembers.Add(this, TRUE);
}

CXTPCalendarCaptionBarThemePart::~CXTPCalendarCaptionBarThemePart()
{
}

void CXTPCalendarCaptionBarThemePart::AdjustLayout(CDC* /*pDC*/, const CRect& rcRect)
{
	m_rcRect = rcRect;
}

COLORREF CXTPCalendarCaptionBarThemePart::GetTextColor()
{
	if (m_clrTextColor.IsDefaultValue() && m_pTheme && m_pTheme->m_pPaintManager)
		return (COLORREF)m_pTheme->m_pPaintManager->m_clrTextColor;

	return (COLORREF)m_clrTextColor;
}

void CXTPCalendarCaptionBarThemePart::Draw(CDC* /*pDC*/){};

BOOL CXTPCalendarCaptionBarThemePart::HitTest(const CPoint& pt) const
{
	return IsVisible() && m_rcRect.PtInRect(pt);
};

void CXTPCalendarCaptionBarThemePart::Redraw(BOOL bUpdateNow)
{
	if (m_pTheme)
		m_pTheme->Redraw(bUpdateNow);
}

void CXTPCalendarCaptionBarThemePart::OnMouseMove(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nStatePrev = m_nState;

	if (HitTest(point))
		m_nState = m_nState | xtpBtnStateMouseOver;
	else
		m_nState = m_nState & (~(xtpBtnStateMouseOver));

	if (nStatePrev != m_nState)
		Redraw();
}

BOOL CXTPCalendarCaptionBarThemePart::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nStatePrev = m_nState;

	BOOL bHitTest = HitTest(point);
	if (bHitTest)
	{
		m_nState = m_nState | xtpBtnStatePressed | xtpBtnStateFocused;
	}
	else
		m_nState = m_nState & (~(xtpBtnStatePressed | xtpBtnStateFocused));

	if (nStatePrev != m_nState)
		Redraw();

	return FALSE; // to allow receive this notification for all other parts (to clear state)
}

BOOL CXTPCalendarCaptionBarThemePart::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nStatePrev = m_nState;
	BOOL bHitTest  = HitTest(point);

	if (bHitTest && (m_nState & (xtpBtnStatePressed | xtpBtnStateFocused)))
	{
		m_nState = m_nState & (~xtpBtnStatePressed);

		if (m_pTheme && IsVisible())
			m_pTheme->OnPartClick(m_nPartID);
	}

	m_nState = m_nState & (~xtpBtnStatePressed);

	if (nStatePrev != m_nState)
		Redraw();

	return FALSE; // to allow receive this notification for all other parts (to clear state)
}

XTPCalendarTheme CXTPCalendarCaptionBarThemePart::GetPaintTheme() const
{
	return m_pTheme ? m_pTheme->GetPaintTheme() : xtpCalendarThemeUnknown;
}

CXTPCalendarCaptionBarPaintManager* CXTPCalendarCaptionBarThemePart::GetPaintManager() const
{
	if (m_pTheme)
	{
		return m_pTheme->m_pPaintManager;
	}
	return NULL;
}

//------------------------------------------------------------------

void CXTPSwitchViewButtonPart::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartRefresh(this);
	}
}

void CXTPSwitchViewButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartAdjustLayout(this, pDC, rcRect);
	}
}

void CXTPSwitchViewButtonPart::Draw(CDC* pDC)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDraw(this, pDC);
	}
}

//------------------------------------------------------------------

LPCTSTR CXTPScrollDateButtonPart::_GetImageName()
{
	if (m_nPartID == xtpCalendarCaptionBar_ScrollNextDate)
		return _T("CALENDARCAPTIONBARNEXTDATEBUTTON");

	return _T("CALENDARCAPTIONBARPREVDATEBUTTON");
}

void CXTPScrollDateButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartAdjustLayout(this, pDC, rcRect);
	}
}

void CXTPScrollDateButtonPart::Draw(CDC* pDC)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDraw(this, pDC);
	}
}

void CXTPScrollDateButtonPart::DrawScrollTriangle(CDC* pDC, CRect rcRect, BOOL bLeftDirection,
												  COLORREF clrColor)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDrawScrollTriangle(this, pDC, rcRect, bLeftDirection, clrColor);
	}
}

BOOL CXTPScrollDateButtonPart::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPCalendarCaptionBarThemePart::OnLButtonDown(nFlags, point);

	if (GetTheme() && GetTheme()->GetOwnerWnd() && HitTest(point)
		&& CWnd::GetFocus() != GetTheme()->GetOwnerWnd())
	{
		GetTheme()->GetOwnerWnd()->SetFocus();
	}

	return FALSE;
}

void CXTPScrollDateButtonPart::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nFlags);

	if (!IsVisible())
		return;

	if ((m_nState & xtpBtnStateFocused) && (m_nState & xtpBtnStatePressed) == 0
		&& nChar == VK_SPACE)
	{
		m_nState |= xtpBtnStatePressed;

		Redraw();
	}
}

void CXTPScrollDateButtonPart::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nFlags);

	if (!IsVisible())
		return;

	int nMask = xtpBtnStateFocused | xtpBtnStatePressed;

	if ((m_nState & nMask) == nMask && nChar == VK_SPACE && GetTheme())
	{
		GetTheme()->OnPartClick(GetPartID());
	}

	if (nChar == VK_SPACE)
	{
		m_nState = m_nState & (~xtpBtnStatePressed);

		Redraw();
	}
}

//------------------------------------------------------------------

void CXTPRadioButtonPart::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartRefresh(this);
	}
}

void CXTPRadioButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartAdjustLayout(this, pDC, rcRect);
	}
}

void CXTPRadioButtonPart::Draw(CDC* pDC)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDraw(this, pDC);
	}
}

//------------------------------------------------------------------

void CXTPCheckboxButtonPart::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartRefresh(this);
	}
}

void CXTPCheckboxButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartAdjustLayout(this, pDC, rcRect);
	}
}

void CXTPCheckboxButtonPart::Draw(CDC* pDC)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDraw(this, pDC);
	}
}

//------------------------------------------------------------------

void CXTPDateLabelPart::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartRefresh(this);
	}
}

void CXTPDateLabelPart::Draw(CDC* pDC)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDraw(this, pDC);
	}
}

CString CXTPDateLabelPart::_FormatDate(COleDateTime dtDay1, COleDateTime dtDay2, BOOL bShowMonthDay)
{
	// LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day
	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_ILDATE);

	CString strFormat1 = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE, 256);
	CString strFormat2 = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE, 256);

	strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T('g'));
	strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T("ddd"));
	strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T("dddd"));

	strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T('g'));
	strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T("ddd"));
	strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T("dddd"));

	if (!bShowMonthDay)
	{
		strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T('d'));
		strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T('d'));
	}
	CString strCaption;

	if (!CXTPCalendarUtils::IsEqual(dtDay1, dtDay2))
	{
		// CString strFormat2(strFormat1);

		if (dtDay1.GetMonth() == dtDay2.GetMonth())
		{
			if (nDateOrdering == 1)
				strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T('M'));
			else
				strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T('M'));
		}

		if (dtDay1.GetYear() == dtDay2.GetYear())
		{
			if (nDateOrdering == 2)
				strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T('y'));
			else
				strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T('y'));
		}

		if (!strFormat1.IsEmpty())
			strCaption = CXTPCalendarUtils::GetDateFormat(dtDay1, strFormat1);

		if (!strFormat1.IsEmpty() && !strFormat2.IsEmpty())
			strCaption += _T(" - ");

		if (!strFormat2.IsEmpty())
			strCaption += CXTPCalendarUtils::GetDateFormat(dtDay2, strFormat2);
	}
	else
	{
		strCaption = CXTPCalendarUtils::GetDateFormat(dtDay1, strFormat1);
	}

	UserAction_FormatCaptionBarLabel(strCaption, dtDay1);
	return strCaption;
}

void CXTPDateLabelPart::UserAction_FormatCaptionBarLabel(CString& strCaption, COleDateTime dtDay1)
{
	DWORD dwFlags = XTP_SAFE_GET2(GetTheme(), GetCalendarCtrl(), GetAskItemTextFlags(), 0);
	CXTPCalendarView* pActiveView = XTP_SAFE_GET2(GetTheme(), GetCalendarCtrl(), GetActiveView(),
												  NULL);
	if (pActiveView == NULL)
		return;

	XTPCalendarViewType activeViewType = pActiveView->GetViewType();

	XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
	::ZeroMemory(&objRequest, sizeof(objRequest));
	objRequest.pstrText = &strCaption;
	objRequest.nWeekNum = CXTPCalendarUtils::GetWeekOfYear(dtDay1);

	if ((dwFlags & xtpCalendarItemText_CaptionBarDayViewDateLabel)
		&& activeViewType == xtpCalendarDayView)
		objRequest.nItem = (int)xtpCalendarItemText_CaptionBarDayViewDateLabel;

	if ((dwFlags & xtpCalendarItemText_CaptionBarWeekViewDateLabel)
		&& (activeViewType == xtpCalendarWeekView || activeViewType == xtpCalendarFullWeekView
			|| activeViewType == xtpCalendarWorkWeekView))
		objRequest.nItem = (int)xtpCalendarItemText_CaptionBarWeekViewDateLabel;

	if ((dwFlags & xtpCalendarItemText_CaptionBarMonthViewDateLabel)
		&& activeViewType == xtpCalendarMonthView)
		objRequest.nItem = (int)xtpCalendarItemText_CaptionBarMonthViewDateLabel;

	if (objRequest.nItem != 0)
		XTP_SAFE_CALL2(GetTheme(), GetCalendarCtrl(),
					   SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0));
}

//------------------------------------------------------------------

void CXTPExpandButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartAdjustLayout(this, pDC, rcRect);
	}
}

void CXTPExpandButtonPart::Draw(CDC* pDC)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDraw(this, pDC);
	}
}

void CXTPExpandButtonPart::DrawExpandTriangle(CDC* pDC, CRect rcRect, BOOL bDownDirection,
											  COLORREF clrColor)
{
	CXTPCalendarCaptionBarPaintManager* pPaintManager = GetPaintManager();
	ASSERT(pPaintManager);

	if (pPaintManager)
	{
		pPaintManager->PartDrawExpandTriangle(this, pDC, rcRect, bDownDirection, clrColor);
	}
}
