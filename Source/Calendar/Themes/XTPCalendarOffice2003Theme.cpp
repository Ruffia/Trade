// XTPCalendarOffice2003Theme.cpp : implementation of the CXTPCalendarOffice2000Theme class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarDayViewEvent.h"
#include "Calendar/XTPCalendarDayViewDay.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarPaintManager.h"

#include "Calendar/Themes/XTPCalendarOffice2000Theme.h"
#include "Calendar/Themes/XTPCalendarOffice2003Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarOffice2003Theme

CXTPCalendarOffice2003Theme::CXTPCalendarOffice2003Theme()
{
	m_CurrSystemTheme = xtpSystemThemeUnknown;
	m_nPaintTheme	 = xtpCalendarThemeOffice2003;
}

CXTPCalendarOffice2003Theme::~CXTPCalendarOffice2003Theme()
{
}

void CXTPCalendarOffice2003Theme::RefreshMetrics()
{
	CXTPCalendarOffice2000Theme::RefreshMetrics();

	m_CurrSystemTheme = xtpSystemThemeUnknown;

	if (m_bEnableTheme && !XTPColorManager()->IsLunaColorsDisabled())
	{
		m_CurrSystemTheme = XTPColorManager()->GetCurrentSystemTheme();

		if (m_CurrSystemTheme != xtpSystemThemeUnknown)
		{
			m_grclrToday.SetStandardValue(m_clrButtonFace, RGB(250, 203, 91));
			m_clrUnderLineHdr = RGB(187, 85, 3);
		}
	}
}

void CXTPCalendarOffice2003Theme::PartRefresh(CWeekViewPart* pViewPart)
{
	ASSERT(pViewPart);
	if (!pViewPart)
		return;

	CXTPCalendarPaintManager::PartRefresh(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 213));
}

void CXTPCalendarOffice2003Theme::PartRefresh(CDayViewWorkCellPart* pViewPart)
{
	ASSERT(pViewPart);
	if (!pViewPart)
		return;

	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 213));
	pViewPart->m_clrShadow.SetStandardValue(RGB(243, 228, 177));
	pViewPart->m_clrHour.SetStandardValue(RGB(234, 208, 152));
}

void CXTPCalendarOffice2003Theme::PartRefresh(CDayViewNonworkCellPart* pViewPart)
{
	ASSERT(pViewPart);
	if (!pViewPart)
		return;

	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 244, 188));
	pViewPart->m_clrBackgroundSlave.SetStandardValue(RGB(255, 244, 188));
	pViewPart->m_clrShadow.SetStandardValue(RGB(243, 228, 177));
	pViewPart->m_clrHour.SetStandardValue(RGB(234, 208, 152));
}

void CXTPCalendarOffice2003Theme::PartRefresh(CMonthViewGridPart* pViewPart)
{
	ASSERT(pViewPart);
	if (!pViewPart)
		return;

	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 213));
	pViewPart->m_clrBackground2.SetStandardValue(RGB(255, 244, 188));
}

void CXTPCalendarOffice2003Theme::PartDraw(CDayViewCellPart* pViewPart, CDC* pDC,
										   CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
										   const XTP_CALENDAR_DAYVIEWCELL_PARAMS& cellParams)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewGroup);

	if (!pDC || !pDC->GetSafeHdc() || !pViewPart || !pViewGroup)
		return;

	int iLeftMargin	= XTP_DPI_X(7);
	COLORREF clrBorder = m_clrWindowText;

	COleDateTime dtTime = cellParams.dtBeginTime;

	CRect rcBusyStatus(rc);
	rcBusyStatus.right = rcBusyStatus.left + (iLeftMargin - XTP_DPI_X(1));

	pDC->FillSolidRect(&rcBusyStatus, m_clrWindow);
	int nDayBusyStatus = pViewGroup->GetBusyStatus(dtTime);
	if (nDayBusyStatus != xtpCalendarBusyStatusUnknown)
		DrawBusyStatus(pDC, rcBusyStatus, nDayBusyStatus);

	if (XTP_SAFE_GET2(pViewGroup, GetViewDay(), GetDayIndex(), 0) != 0
		|| pViewGroup->GetGroupIndex() != 0)
		pDC->FillSolidRect(rc.left, rc.top, XTP_DPI_X(1), rc.Height(), clrBorder);

	int nDayDeviderX = 0; // pViewGroup->GetGroupIndex() ? -2 : 0;

	pDC->FillSolidRect(rc.left + (iLeftMargin - XTP_DPI_X(1)) + nDayDeviderX, rc.top, XTP_DPI_X(1),
					   rc.Height(), clrBorder);

	rc.left += iLeftMargin + nDayDeviderX;
	pDC->FillSolidRect(rc, cellParams.clrBackgroundColor);

	if (dtTime.GetMinute() == 0 && dtTime.GetHour() != 0)
		pDC->FillSolidRect(rc.left, rc.top - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrHour);

	pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
					   pViewPart->m_clrShadow);
}

void CXTPCalendarOffice2003Theme::PartDrawDayHeader(CWeekViewPart* pViewPart, CDC* pDC, CRect rcDay,
													int nHeaderHeight, CString strHeader,
													BOOL bIsCurrent, BOOL bIsSelected)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (!pDC || !pDC->GetSafeHdc() || !pViewPart)
		return;

	CRect rcHeader(rcDay);
	rcHeader.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(1),
						 rcHeader.Height() - nHeaderHeight - XTP_DPI_Y(1));
	CRect rcText(rcHeader);
	rcText.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(0), XTP_DPI_X(3), XTP_DPI_Y(1));

	CPen outerPen;
	outerPen.CreatePen(PS_SOLID, 1, pViewPart->m_clrHeaderBottomLine);
	CPen* pOldPen = pDC->SelectObject(&outerPen);

	// draw header top line
	DrawHorizontalLine(pDC, rcDay.left + XTP_DPI_X(1), rcDay.top, rcDay.Width() + XTP_DPI_X(1));

	pDC->SelectObject(pOldPen);
	outerPen.DeleteObject();

	// draw header contents
	CRect rcHeader2(rcHeader);
	rcHeader2.bottom -= XTP_DPI_Y(1);

	if (bIsCurrent && bIsSelected)
	{
		DrawHeader(pDC, rcHeader2, FALSE, bIsCurrent);
		CRect rcSel  = CRect(rcHeader2);
		CSize szText = pViewPart->GetTextExtent(pDC, strHeader);
		rcSel.left   = rcSel.right - (szText.cx + XTP_DPI_X(7));
		rcSel.bottom -= XTP_DPI_Y(1);
		DrawHeader(pDC, rcSel, bIsSelected, FALSE);
	}
	else
	{
		DrawHeader(pDC, rcHeader2, bIsSelected, bIsCurrent);
	}

	// draw header bottom line
	if (!bIsCurrent)
	{
		CRect rcBottomLine(rcHeader);
		rcBottomLine.top = --rcBottomLine.bottom - XTP_DPI_Y(1);
		if (!bIsSelected)
			rcBottomLine.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);
		pDC->FillSolidRect(rcBottomLine, pViewPart->m_clrHeaderBottomLine);
	}

	// draw header text
	pDC->SetBkMode(TRANSPARENT);
	pViewPart->SetTextColor(bIsSelected ? pViewPart->m_clrTextHighLightColor
										: pViewPart->m_clrTextNormalColor);
	pViewPart->DrawText(pDC, strHeader, rcText, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
}

void CXTPCalendarOffice2003Theme::PartDrawDayDate(CMonthViewEventPart* pViewPart, CDC* pDC,
												  CRect rc, BOOL bToday, BOOL bSelected,
												  BOOL bFirstMonthDay, CString strText)
{
	UNREFERENCED_PARAMETER(bFirstMonthDay);

	ASSERT(pDC);
	ASSERT(pViewPart);

	if (!pDC || !pDC->GetSafeHdc() || !pViewPart)
		return;

	rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	BOOL bSysTheme = GetCurrentSystemTheme() != xtpSystemThemeUnknown;

	if (bSelected && bToday)
	{
		CRect rcSel   = CRect(rc);
		CRect rcUnSel = CRect(rc);
		CSize szText  = pViewPart->GetTextExtent(pDC, strText);
		rcSel.left	= rcSel.right - (szText.cx + XTP_DPI_X(7));
		rcUnSel.right = rcUnSel.left + rcUnSel.Width() - (szText.cx + XTP_DPI_X(7));

		if (bSysTheme)
			DrawHeader(pDC, rcUnSel, FALSE, bToday);
		else
			DrawHeader(pDC, rcUnSel, FALSE, bToday, pViewPart->m_grclrClassicSelDay,
					   pViewPart->m_clrClassicUnderline);

		DrawHeader(pDC, rcSel, bSelected, FALSE);
	}
	else
	{
		if (bSelected || bToday)
		{
			if (bSysTheme)
				DrawHeader(pDC, rc, bSelected, bToday);
			else
				DrawHeader(pDC, rc, bSelected, bToday, pViewPart->m_grclrClassicSelDay,
						   pViewPart->m_clrClassicUnderline);
		}
	}

	UINT dtTextFormat = DT_VCENTER | DT_SINGLELINE;

	dtTextFormat |= DT_RIGHT;
	rc.DeflateRect(0, 0, XTP_DPI_X(5), 0);

	COLORREF clrOldTextColor = pDC->GetTextColor();

	if (bSelected)
	{
		pDC->SetTextColor(pViewPart->m_clrTextHighLightColor);
	}

	CXTPFontDC fnt(pDC, &pViewPart->GetTextFont());
	pDC->DrawText(strText, rc, dtTextFormat);
	pViewPart->m_clrTextColor.SetDefaultValue();

	pDC->SetTextColor(clrOldTextColor);
}
