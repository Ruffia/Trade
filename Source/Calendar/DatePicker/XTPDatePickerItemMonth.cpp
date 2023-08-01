// XTPDatePickerItemMonth.cpp: implementation of the CXTPDatePickerItemMonth class.
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

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/DatePicker/XTPDatePickerItemMonth.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/DatePicker/XTPDatePickerItemDay.h"
#include "Calendar/DatePicker/XTPDatePickerDaysCollection.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPDatePickerItemMonth

CXTPDatePickerItemMonth::CXTPDatePickerItemMonth(CXTPDatePickerControl* pControl,
												 COleDateTime dtMonth, int nRow, int nCol)
	: m_pControl(pControl)
	, m_nRow(nRow)
	, m_nColumn(nCol)
	, m_dtMonth(dtMonth)
{
	m_rcHeader.SetRectEmpty();
	m_rcDaysOfWeek.SetRectEmpty();
	m_rcWeekNumbers.SetRectEmpty();
	m_rcDaysArea.SetRectEmpty();
	m_rcLeftScroll.SetRectEmpty();
	m_rcRightScroll.SetRectEmpty();

	m_bShowDaysBefore  = FALSE;
	m_bShowDaysAfter   = FALSE;
	m_bShowLeftScroll  = FALSE;
	m_bShowRightScroll = FALSE;

	PopulateDays();
}

CXTPDatePickerItemMonth::~CXTPDatePickerItemMonth()
{
	ClearDays();
}

void CXTPDatePickerItemMonth::PopulateDays()
{
	if (m_dtMonth.GetYear() <= 100 && m_dtMonth.GetMonth() <= 2)
		m_dtMonth = COleDateTime(100, 2, 1, 0, 0, 0);

	if (m_dtMonth.GetStatus() != COleDateTime::valid)
	{
		ASSERT(FALSE);
		m_dtMonth = CXTPCalendarUtils::ResetTime(COleDateTime::GetCurrentTime());
	}
	// if (m_dtMonth < COleDateTime(1601,4,1,0,0,0))
	//  m_dtMonth = COleDateTime(1601,4,1,0,0,0);

	// init days from 1st to last
	COleDateTime dtDay(m_dtMonth.GetYear(), m_dtMonth.GetMonth(), 1, 0, 0, 0);
	COleDateTimeSpan spDay(1, 0, 0, 0);

	// adjust first day of the week
	int nOleFirstDayOfWeek = m_pControl->GetFirstDayOfWeek();

	int iDayOfWeek = dtDay.GetDayOfWeek();
	if (iDayOfWeek != -1)
	{
		while (dtDay.GetDayOfWeek() != nOleFirstDayOfWeek && dtDay.GetDayOfWeek() != -1)
		{
			dtDay -= spDay;
		}
	}
	else
	{
		COleDateTime dtFutureDay(dtDay);
		dtFutureDay.SetDate(dtFutureDay.GetYear() + 2000, dtFutureDay.GetMonth(),
							dtFutureDay.GetDay());
		iDayOfWeek = dtFutureDay.GetDayOfWeek();

		while (iDayOfWeek != nOleFirstDayOfWeek && iDayOfWeek != -1)
		{
			dtFutureDay -= spDay;
			dtDay -= spDay;
			iDayOfWeek = dtFutureDay.GetDayOfWeek();
		}
	}

	// populate all grid days
	for (int nWeek = 0; nWeek < XTP_MAX_WEEKS; nWeek++)
	{
		for (int nDay = 0; nDay < XTP_WEEK_DAYS; nDay++)
		{
			// create and add new day item
			CXTPDatePickerItemDay* pDay = new CXTPDatePickerItemDay(m_pControl, this, dtDay);
			m_arrDays.Add(pDay);
			// move next day
			dtDay += spDay;
		}
	}
}

void CXTPDatePickerItemMonth::AutoAdjustLayout(CRect rcClient)
{
	m_rcMonth = rcClient;
	// calc header rect
	m_rcHeader		  = m_rcMonth;
	m_rcHeader.bottom = m_rcHeader.top + XTP_DPI_Y(18); // TODO: change to font height

	// calc days of week rect
	m_rcDaysOfWeek		  = m_rcMonth;
	m_rcDaysOfWeek.top	= m_rcHeader.bottom;
	m_rcDaysOfWeek.bottom = m_rcDaysOfWeek.top + XTP_DPI_Y(18); // TODO: change to font height

	// draw week numbers
	m_rcWeekNumbers		  = m_rcMonth;
	m_rcWeekNumbers.top   = m_rcDaysOfWeek.bottom;
	m_rcWeekNumbers.right = m_rcWeekNumbers.left + XTP_DPI_X(18); // TODO: change to font width

	// calc triangles rect
	m_rcLeftScroll   = m_rcHeader;
	m_rcRightScroll  = m_rcHeader;
	int nScrollWidth = min(m_rcWeekNumbers.Width(), m_rcHeader.Width() / 8);
	m_rcHeader.left += nScrollWidth;
	m_rcHeader.right -= nScrollWidth;
	m_rcLeftScroll.right = m_rcHeader.left;
	m_rcRightScroll.left = m_rcHeader.right;

	// calc days display params
	m_rcDaysArea	  = m_rcMonth;
	m_rcDaysArea.top  = m_rcDaysOfWeek.bottom;
	m_rcDaysArea.left = m_rcWeekNumbers.right;
	m_rcDaysArea.right -= m_rcWeekNumbers.Width();

	// calc simple day rect sizes
	CSize szDay(XTPToIntChecked(m_rcDaysArea.Width() / XTP_WEEK_DAYS),
				XTPToIntChecked(m_rcDaysArea.Height() / XTP_MAX_WEEKS));

	// calc rects for all days
	CRect rcDay;
	int nIndex = 0;
	for (int nWeek = 0; nWeek < XTP_MAX_WEEKS; nWeek++)
	{
		for (int nDay = 0; nDay < XTP_WEEK_DAYS; nDay++)
		{
			CXTPDatePickerItemDay* pDay = GetDay(nIndex);
			nIndex++;

			rcDay.left   = m_rcDaysArea.left + nDay * szDay.cx + XTP_DPI_X(1);
			rcDay.top	= m_rcDaysArea.top + nWeek * szDay.cy;
			rcDay.right  = rcDay.left + szDay.cx;
			rcDay.bottom = rcDay.top + szDay.cy;
			pDay->SetRect(rcDay);
		}
	}
}

void CXTPDatePickerItemMonth::ByFontAdjustLayout(CRect rcClient)
{
	CWindowDC dc(CWnd::GetDesktopWindow());
	CSize szDay(m_pControl->m_pPaintManager->CalcDayRect(&dc));
	CSize szHeader(m_pControl->m_pPaintManager->CalcMonthHeaderRect(&dc));
	CSize szWeekText(m_pControl->m_pPaintManager->CalcDayOfWeekRect(&dc));
	CSize szWeekNumber(m_pControl->m_pPaintManager->CalcWeekNumbersRect(&dc));

	m_rcMonth = rcClient;
	// calc header rect
	m_rcHeader		  = m_rcMonth;
	m_rcHeader.bottom = m_rcHeader.top + szHeader.cy;
	// m_rcHeader.right = m_rcHeader.left + szHeader.cx;

	// calc days of week rect
	m_rcDaysOfWeek	 = m_rcMonth;
	m_rcDaysOfWeek.top = m_rcHeader.bottom;
	// m_rcDaysOfWeek.bottom = m_rcDaysOfWeek.top + 18; // TODO: change to font height
	m_rcDaysOfWeek.bottom = m_rcDaysOfWeek.top + szWeekText.cy;

	// draw week numbers
	m_rcWeekNumbers		= m_rcMonth;
	m_rcWeekNumbers.top = m_rcDaysOfWeek.bottom;
	// m_rcWeekNumbers.right = m_rcWeekNumbers.left + 18; // TODO: change to font width
	m_rcWeekNumbers.right = m_rcWeekNumbers.left + szWeekNumber.cx;

	// calc triangles rect
	m_rcLeftScroll		 = m_rcHeader;
	m_rcRightScroll		 = m_rcHeader;
	m_rcLeftScroll.right = m_rcHeader.left + szWeekNumber.cx + XTP_DPI_X(3);
	m_rcRightScroll.left = m_rcHeader.right - szWeekNumber.cx - XTP_DPI_X(3);

	// calc days display params
	m_rcDaysArea	  = m_rcMonth;
	m_rcDaysArea.top  = m_rcDaysOfWeek.bottom;
	m_rcDaysArea.left = m_rcWeekNumbers.right;
	m_rcDaysArea.right -= m_rcWeekNumbers.Width();

	CRect rcDay;
	int nIndex = 0;
	for (int nWeek = 0; nWeek < XTP_MAX_WEEKS; nWeek++)
	{
		for (int nDay = 0; nDay < XTP_WEEK_DAYS; nDay++)
		{
			CXTPDatePickerItemDay* pDay = GetDay(nIndex);
			nIndex++;

			rcDay.left   = m_rcDaysArea.left + nDay * szDay.cx + XTP_DPI_X(1);
			rcDay.top	= m_rcDaysArea.top + nWeek * szDay.cy;
			rcDay.right  = rcDay.left + szDay.cx;
			rcDay.bottom = rcDay.top + szDay.cy;
			pDay->SetRect(rcDay);
		}
	}
}

void CXTPDatePickerItemMonth::AdjustLayout(CRect rcClient, BOOL bIsAuto)
{
	if (bIsAuto)
		AutoAdjustLayout(rcClient);
	else
		ByFontAdjustLayout(rcClient);
}

CXTPDatePickerItemDay* CXTPDatePickerItemMonth::GetDay(int nIndex) const
{
	if (nIndex < GetDayCount())
		return m_arrDays[nIndex];
	return NULL;
}

int CXTPDatePickerItemMonth::GetDayCount() const
{
	return (int)m_arrDays.GetSize();
}

void CXTPDatePickerItemMonth::Draw(CDC* pDC)
{
	// draw header
	CXTPDatePickerPaintManager* pPaintManager = m_pControl->GetPaintManager();
	pPaintManager->DrawMonthHeader(pDC, this);

	// draw days of week
	pPaintManager->DrawDaysOfWeek(pDC, this);

	//  draw all days
	int nDaysCount = GetDayCount();
	for (int nIndex = 0; nIndex < nDaysCount; nIndex++)
	{
		CXTPDatePickerItemDay* pDay = GetDay(nIndex);
		pDay->Draw(pDC);
	}

	// draw week numbers
	pPaintManager->DrawWeekNumbers(pDC, this);
}

void CXTPDatePickerItemMonth::ClearDays()
{
	// cleanup old month array
	int nOldDayCount = GetDayCount();
	for (int nDay = 0; nDay < nOldDayCount; nDay++)
	{
		CXTPDatePickerItemDay* pDay = GetDay(nDay);
		pDay->InternalRelease();
	}
	m_arrDays.RemoveAll();
}

void CXTPDatePickerItemMonth::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_pControl)
		return;

	BOOL bMultiselect = !!(nFlags & MK_CONTROL) || m_pControl->IsMultiSelectionMode();

	// check left scroll item
	if (m_bShowLeftScroll && !m_rcLeftScroll.IsRectEmpty() && m_rcLeftScroll.PtInRect(point))
	{
		m_pControl->m_mouseMode = CXTPDatePickerControl::mouseScrollingLeft;
		m_pControl->m_nTimerID  = XTPToInt(
			 m_pControl->SetTimer(XTP_DATEPICKER_TIMERID, XTP_DATEPICKER_TIMER_INTERVAL, NULL));
		if (m_pControl->m_bYearsTriangle)
		{
			if (point.x < m_rcLeftScroll.left + XTP_DPI_X(9))
			{
				if (m_dtMonth.GetYear() >= 100
					|| (m_dtMonth.GetYear() == 100 && m_dtMonth.GetMonth() >= 3))
					m_pControl->ScrollLeft(m_pControl->GetMonthDelta());
			}
			else if (point.x > m_rcLeftScroll.right - XTP_DPI_X(9))
			{
				if (m_dtMonth.GetYear() >= 101
					|| (m_dtMonth.GetYear() == 101 && m_dtMonth.GetMonth() >= 3))
					m_pControl->ScrollLeft(12 * m_pControl->GetMonthDelta());
			}
		}
		else
		{
			m_pControl->ScrollLeft(m_pControl->GetMonthDelta());
		}
		return;
	}
	// check right scroll item
	if (m_bShowRightScroll && !m_rcRightScroll.IsRectEmpty() && m_rcRightScroll.PtInRect(point))
	{
		m_pControl->m_mouseMode = CXTPDatePickerControl::mouseScrollingRight;
		m_pControl->m_nTimerID  = XTPToInt(
			 m_pControl->SetTimer(XTP_DATEPICKER_TIMERID, XTP_DATEPICKER_TIMER_INTERVAL, NULL));

		if (m_pControl->m_bYearsTriangle)
		{
			if (point.x < m_rcRightScroll.left + XTP_DPI_X(9))
				m_pControl->ScrollRight(12 * m_pControl->GetMonthDelta());
			else if (point.x > m_rcLeftScroll.right - XTP_DPI_X(9))
				m_pControl->ScrollRight(m_pControl->GetMonthDelta());
		}
		else
		{
			m_pControl->ScrollRight(m_pControl->GetMonthDelta());
		}
		return;
	}
	// check header click
	if (!m_rcHeader.IsRectEmpty()
		&& CRect(m_rcHeader.left + XTP_DPI_X(20), m_rcHeader.top, m_rcHeader.right - XTP_DPI_X(20),
				 m_rcHeader.bottom)
			   .PtInRect(point))
	// m_rcHeader.PtInRect(point))
	{
		// call month popup list
		m_pControl->ShowListHeader(m_rcHeader, m_dtMonth);
		return;
	}
	// check week numbers click
	m_pControl->m_bSelectWeek = !m_rcWeekNumbers.IsRectEmpty() && m_rcWeekNumbers.PtInRect(point);
	CPoint ptFirstWeekDay(point);
	ptFirstWeekDay.x = m_rcWeekNumbers.right + XTP_DPI_X(3);

	// check day item
	CXTPDatePickerItemDay* pDay = HitTest(m_pControl->m_bSelectWeek ? ptFirstWeekDay : point,
										  FALSE);
	if (pDay)
	{
		m_pControl->ClearFocus();

		BOOL bAllowNoncontinuousSelection = m_pControl->IsAllowNoncontinuousSelection();

		COleDateTime dtDay(pDay->GetDate());

		// if selected day isn't in range
		COleDateTime pMinRange, pMaxRange;
		DWORD dwRes = m_pControl->GetRange(&pMinRange, &pMaxRange);
		if (dwRes & GDTR_MIN && pMinRange > dtDay)
			return;
		if (dwRes & GDTR_MAX && pMaxRange < dtDay)
			return;

		if (m_pControl->GetShowNonMonthDays() == FALSE && !pDay->IsVisible())
			return;

		BOOL bSelected = m_pControl->IsSelected(dtDay);
		if (m_pControl->m_bSelectWeek)
		{
			// week is selected only when all week days are selected
			for (int i = 0; i < 7; i++)
			{
				COleDateTime dtDay_i = dtDay + COleDateTimeSpan(i, 0, 0, 0);
				bSelected &= m_pControl->IsSelected(dtDay_i);
			}
		}
		if (bSelected && bMultiselect && bAllowNoncontinuousSelection)
			m_pControl->m_mouseMode = CXTPDatePickerControl::mouseDeselecting;
		else
			m_pControl->m_mouseMode = CXTPDatePickerControl::mouseSelecting;

		if (!bMultiselect || !bAllowNoncontinuousSelection)
			m_pControl->m_pSelectedDays->Clear();

		if (nFlags & MK_SHIFT)
		{
			if (m_pControl->m_dtFirstClicked.GetStatus() == COleDateTime::valid)
			{
				m_pControl->m_dtLastClicked = dtDay;
			}
			else if (m_pControl->m_dtLastClicked.GetStatus() == COleDateTime::valid)
			{
				m_pControl->m_dtFirstClicked = m_pControl->m_dtLastClicked;
				m_pControl->m_dtLastClicked  = dtDay;
			}
			else if (m_pControl->GetSelRange(m_pControl->m_dtFirstClicked,
											 m_pControl->m_dtLastClicked))
			{
				if (m_pControl->m_dtFirstClicked < dtDay)
					m_pControl->m_dtLastClicked = dtDay;
				else
					m_pControl->m_dtFirstClicked = dtDay;
			}
			else
			{
				m_pControl->m_dtFirstClicked = dtDay;
				m_pControl->m_dtLastClicked  = dtDay;
			}
		}
		else
		{
			m_pControl->m_dtFirstClicked = dtDay;
			m_pControl->m_dtLastClicked  = m_pControl->m_bSelectWeek
											  ? dtDay + COleDateTimeSpan(6, 0, 0, 0)
											  : dtDay;
		}

		m_pControl->_RedrawControl(FALSE);
	}
}

void CXTPDatePickerItemMonth::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
}

void CXTPDatePickerItemMonth::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	if ((m_pControl->m_mouseMode == CXTPDatePickerControl::mouseDeselecting
		 || m_pControl->m_mouseMode == CXTPDatePickerControl::mouseSelecting)
		&& (m_rcDaysArea.PtInRect(point) || m_rcWeekNumbers.PtInRect(point)))
	{
		// check day item
		CPoint ptWeekBegin(m_rcWeekNumbers.right + XTP_DPI_X(3), point.y);
		CXTPDatePickerItemDay* pDay = HitTest(m_pControl->m_bSelectWeek ? ptWeekBegin : point,
											  FALSE);
		if (pDay)
		{
			// save old clicked dates
			COleDateTime dtOldFirstClicked = m_pControl->m_dtFirstClicked;
			COleDateTime dtOldLastClicked  = m_pControl->m_dtLastClicked;

			// calculate new clicked dates
			COleDateTime dtDay(pDay->GetDate());
			if (m_pControl->m_bSelectWeek)
			{
				COleDateTime dtWeekEnd(dtDay + COleDateTimeSpan(6, 0, 0, 0));
				if (dtWeekEnd >= m_pControl->m_dtFirstClicked)
				{
					if (dtDay.GetDayOfWeek() != m_pControl->m_dtFirstClicked.GetDayOfWeek())
						m_pControl->m_dtFirstClicked -= COleDateTimeSpan(6, 0, 0, 0);
					m_pControl->m_dtLastClicked = dtWeekEnd;
				}
				if (dtDay < m_pControl->m_dtFirstClicked)
				{
					if (dtDay.GetDayOfWeek() == m_pControl->m_dtFirstClicked.GetDayOfWeek())
						m_pControl->m_dtFirstClicked += COleDateTimeSpan(6, 0, 0, 0);
					m_pControl->m_dtLastClicked = dtDay;
				}
			}
			else
			{
				m_pControl->m_dtLastClicked = dtDay;
			}

			// update control if needed
			if (((dtOldLastClicked - m_pControl->m_dtLastClicked).GetDays() != 0)
				|| ((dtOldFirstClicked - m_pControl->m_dtFirstClicked).GetDays() != 0))
			{
				m_pControl->RedrawControl();
			}
		}
	}
}

CXTPDatePickerItemDay* CXTPDatePickerItemMonth::HitTest(CPoint point, BOOL bCheckVisible) const
{
	// enumerate month array
	int nDayCount = GetDayCount();
	for (int nDay = 0; nDay < nDayCount; nDay++)
	{
		CXTPDatePickerItemDay* pDay = GetDay(nDay);
		if (pDay && pDay->GetRect().PtInRect(point) && (pDay->IsVisible() || !bCheckVisible))
		{
			return pDay;
		}
	}
	return NULL;
}

BOOL CXTPDatePickerItemMonth::OnSetCursor(CPoint point)
{
	if (!m_rcWeekNumbers.IsRectEmpty() && m_rcWeekNumbers.PtInRect(point) && m_pControl
		&& m_pControl->GetPaintManager())
	{
		// week selection cursor
		SetCursor(m_pControl->GetPaintManager()->GetCursorBack());
		return TRUE;
	}
	return FALSE;
}