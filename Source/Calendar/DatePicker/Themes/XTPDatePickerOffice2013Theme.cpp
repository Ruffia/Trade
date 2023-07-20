// XTPDatePickerOffice2013Theme.cpp: interface for the CXTPDatePickerOffice2013Theme class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"
#include "Calendar/DatePicker/XTPDatePickerItemMonth.h"
#include "Calendar/DatePicker/XTPDatePickerItemDay.h"
#include "Calendar/DatePicker/Themes/XTPDatePickerOffice2013Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPDatePickerThemeOffice2013::CXTPDatePickerThemeOffice2013()
{
	m_nPaintTheme = xtpCalendarThemeOffice2013;

	// initialize default fonts
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfHeight += 1;
	SetFontIndirect(&lfIcon);

	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfWeight = FW_BOLD;
	m_xtpFontHeader.DeleteObject();
	VERIFY(m_xtpFontHeader.CreateFontIndirect(&lfIcon));

	m_szDayMargin.cx = XTP_DPI_X(12);
	m_szDayMargin.cy = XTP_DPI_Y(8);
	m_szHdrMargin.cx = 0;
	m_szHdrMargin.cy = XTP_DPI_Y(6);
	m_szDoWMargin.cx = 0;
	m_szDoWMargin.cy = XTP_DPI_Y(12);
}

CXTPDatePickerThemeOffice2013::~CXTPDatePickerThemeOffice2013()
{
}

void CXTPDatePickerThemeOffice2013::RefreshMetrics()
{
	CXTPDatePickerPaintManager::RefreshMetrics();

	m_clrBackground = CXTPPaintManagerColorGradient(
		XTPIniColor(_T("Calendar.DatePicker"), _T("BackgroundStart"), RGB(255, 255, 255)),
		XTPIniColor(_T("Calendar.DatePicker"), _T("BackgroundStop"), RGB(245, 245, 245)));

	m_clrDayBack = XTPIniColor(_T("Calendar.DatePicker"), _T("DayBack"), RGB(255, 255, 255));
	m_clrDayText = XTPIniColor(_T("Calendar.DatePicker"), _T("DayText"), RGB(68, 68, 68));
	m_clrNonMonthDayBack	  = XTPIniColor(_T("Calendar.DatePicker"), _T("NonMonthDayBack"),
										RGB(255, 255, 255));
	m_clrNonMonthDayText	  = XTPIniColor(_T("Calendar.DatePicker"), _T("NonMonthDayText"),
										RGB(68, 68, 68));
	m_clrSelectedDayBack	  = XTPIniColor(_T("Calendar.DatePicker"), _T("SelectedDayBack"),
										RGB(205, 230, 247));
	m_clrSelectedDayText	  = XTPIniColor(_T("Calendar.DatePicker"), _T("SelectedDayText"),
										RGB(68, 68, 68));
	m_clrMonthBorder		  = XTPIniColor(_T("Calendar.DatePicker"), _T("MonthBorder"),
									RGB(212, 212, 212));
	m_clrMonthHeaderBack	  = XTPIniColor(_T("Calendar.DatePicker"), _T("MonthHeaderBack"),
										RGB(240, 240, 240));
	m_clrMonthHeaderText	  = XTPIniColor(_T("Calendar.DatePicker"), _T("MonthHeaderText"),
										RGB(68, 68, 68));
	m_clrMonthHeaderButton	= XTPIniColor(_T("Calendar.DatePicker"), _T("MonthHeaderButton"),
											RGB(68, 68, 68));
	m_clrDaysOfWeekBack		  = XTPIniColor(_T("Calendar.DatePicker"), _T("DaysOfWeekBack"),
										RGB(255, 255, 255));
	m_clrDaysOfWeekText		  = XTPIniColor(_T("Calendar.DatePicker"), _T("DaysOfWeekText"),
										RGB(68, 68, 68));
	m_clrWeekNumbersBack	  = XTPIniColor(_T("Calendar.DatePicker"), _T("WeekNumbersBack"),
										RGB(255, 255, 255));
	m_clrWeekNumbersText	  = XTPIniColor(_T("Calendar.DatePicker"), _T("WeekNumbersText"),
										RGB(68, 68, 68));
	m_clrHighlightToday		  = XTPIniColor(_T("Calendar.DatePicker"), _T("HighlightToday"),
										RGB(0, 114, 198));
	m_clrHighlightTodayText   = XTPIniColor(_T("Calendar.DatePicker"), _T("HighlightTodayText"),
											RGB(255, 255, 255));
	m_clrHighlightTodayBorder = XTPIniColor(_T("Calendar.DatePicker"), _T("HighlightTodayBorder"),
											RGB(0, 114, 198));
	m_clrButtonFace = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonFace"), RGB(146, 192, 224));
	m_clrButtonText = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonText"), RGB(68, 68, 68));
	m_clrButtonBorder		   = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonBorder"),
									 RGB(146, 192, 224));
	m_clrButtonPressed		   = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonPressed"),
									 RGB(146, 192, 224));
	m_clrButtonPressedText	 = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonPressedText"),
										 RGB(68, 68, 68));
	m_clrButtonPressedBorder   = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonPressedBorder"),
											 RGB(42, 141, 212));
	m_clrButtonHighlight	   = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonHighlight"),
										 RGB(205, 230, 247));
	m_clrButtonHighlightText   = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonHighlightText"),
											 RGB(68, 68, 68));
	m_clrButtonHighlightBorder = XTPIniColor(_T("Calendar.DatePicker"), _T("ButtonHighlightBorder"),
											 RGB(146, 192, 224));
}

void CXTPDatePickerThemeOffice2013::DrawBackground(CDC* pDC, CRect rcClient)
{
	if (m_clrBackground.clrDark != m_clrBackground.clrLight)
	{
		XTPDrawHelpers()->GradientFill(pDC, &rcClient, m_clrBackground, FALSE);
	}
	else
	{
		pDC->FillSolidRect(rcClient, m_clrBackground.clrDark);
	}
}

void CXTPDatePickerThemeOffice2013::DrawWeekNumbers(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
{
	CXTPDatePickerControl* pControl = pMonth->m_pControl;

	if (!pControl->GetShowWeekNumbers() || pMonth->m_rcWeekNumbers.IsRectEmpty())
		return;

	// fill background
	//	pDC->FillSolidRect(pMonth->m_rcWeekNumbers, GetWeekNumbersBackColor());

	// draw week numbers
	pDC->SetBkColor(GetWeekNumbersBackColor());
	pDC->SetTextColor(GetWeekNumbersTextColor());
	CXTPFontDC fnt(pDC, GetWeekNumbersTextFont());

	// draw right line on week days
	CRect rcLine(pMonth->m_rcWeekNumbers);

	for (int nWeek = 0; nWeek < XTP_MAX_WEEKS; nWeek++)
	{
		// calculate item rectangle (get week middle day)
		CXTPDatePickerItemDay* pDayFirst = pMonth->GetDay(XTPToInt(nWeek * XTP_WEEK_DAYS));
		CXTPDatePickerItemDay* pDayLast = pMonth->GetDay(XTPToInt((nWeek + 1) * XTP_WEEK_DAYS - 1));

		if (pDayFirst && (pDayFirst->IsVisible() || nWeek < 4) && pDayLast)
		{
			CRect rcItem(pDayFirst->GetRect());
			rcItem.left  = pMonth->m_rcWeekNumbers.left;
			rcItem.right = pMonth->m_rcWeekNumbers.right - XTP_DPI_X(3);
			// get item text
			CString strText;
			int nWeekNumber = ((pDayLast->GetDate()
								- COleDateTimeSpan(pControl->GetFirstWeekOfYearDays() - 1))
								   .GetDayOfYear()
							   - 1)
								  / 7
							  + 1;
			strText.Format(_T("%d"), nWeekNumber);
			pDC->DrawText(strText, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);

			rcLine.bottom = rcItem.bottom;
		}
	}

	pDC->FillSolidRect(rcLine.right - XTP_DPI_X(1), rcLine.top - XTP_DPI_Y(1), XTP_DPI_X(1),
					   rcLine.Height(), m_clrMonthBorder);
}

void CXTPDatePickerThemeOffice2013::DrawButton(CDC* pDC, const CRect& rcButton,
											   const CString& strButton, BOOL bIsDown,
											   BOOL bIsHighLight)
{
	CRect rc(rcButton);

	pDC->SetBkMode(TRANSPARENT);

	COLORREF clrText   = m_clrButtonText;
	COLORREF clrButton = m_clrButtonFace;
	COLORREF clrBorder = m_clrButtonBorder;

	if (bIsDown)
	{
		clrText   = m_clrButtonPressedText;
		clrButton = m_clrButtonPressed;
		clrBorder = m_clrButtonPressedBorder;
	}
	else if (bIsHighLight)
	{
		clrText   = m_clrButtonHighlightText;
		clrButton = m_clrButtonHighlight;
		clrBorder = m_clrButtonHighlightBorder;
	}

	pDC->FillSolidRect(rc, clrButton);
	pDC->Draw3dRect(rc, clrBorder, clrBorder);

	if (!strButton.IsEmpty())
	{
		CXTPFontDC autoFont(pDC, &m_xtpFontButton, clrText);
		pDC->DrawText(strButton, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

BOOL CXTPDatePickerThemeOffice2013::DrawDay(CDC* pDC, CXTPDatePickerItemDay* pDay)
{
	if (!pDC || !pDay)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// check this day item
	BOOL bThisMonthItem		= FALSE;
	BOOL bNextMonthItem		= FALSE;
	BOOL bPreviousMonthItem = FALSE;

	CXTPDatePickerControl* pControl = pDay->GetDatePickerControl();
	CXTPDatePickerItemMonth* pMonth = pDay->GetMonth();

	COleDateTime dtDay = pDay->GetDate();
	CRect rcDay		   = pDay->GetRect();

	// TRACE2("cx: %d, cy: %d\n", rcDay.Width(), rcDay.Height());

	if (dtDay.GetYear() > pMonth->GetMonth().GetYear())
	{
		bNextMonthItem = TRUE;
	}
	else if (dtDay.GetYear() < pMonth->GetMonth().GetYear())
	{
		bPreviousMonthItem = TRUE;
	}
	else // years the same
	{
		if (dtDay.GetMonth() > pMonth->GetMonth().GetMonth())
			bNextMonthItem = TRUE;
		else if (dtDay.GetMonth() < pMonth->GetMonth().GetMonth())
			bPreviousMonthItem = TRUE;
		else // months the same
			bThisMonthItem = TRUE;
	}

	if (bPreviousMonthItem && !pMonth->GetShowDaysBefore())
		return FALSE;

	if (bNextMonthItem && !pMonth->GetShowDaysAfter())
		return FALSE;

	// if selected day isn't in range
	COleDateTime pMinRange, pMaxRange;
	DWORD dwRes = pControl->GetRange(&pMinRange, &pMaxRange);
	if (dwRes & GDTR_MIN && pMinRange > dtDay)
		return FALSE;
	if (dwRes & GDTR_MAX && pMaxRange < dtDay)
		return FALSE;

	// set default drawing metrics depending on item month position
	XTP_DAYITEM_METRICS* pMetrics = new XTP_DAYITEM_METRICS;

	// call callback function for changing parameters if needed
	pMetrics->clrBackground = bThisMonthItem ? GetDayBackColor() : GetNonMonthDayBackColor();
	pMetrics->clrForeground = bThisMonthItem ? GetDayTextColor() : GetNonMonthDayTextColor();
	pMetrics->SetFont(bThisMonthItem ? GetDayTextFont() : GetNonMonthDayTextFont());
	COLORREF clrBackOrig = pMetrics->clrBackground;
	pControl->GetDayMetrics(dtDay, pMetrics);

	BOOL bToday  = FALSE;
	CFont* pFont = pMetrics->GetFont(); // Normal

	// Previous / Next month day
	if (bNextMonthItem || bPreviousMonthItem)
	{
		pMetrics->clrBackground = GetNonMonthDayBackColor();
		pMetrics->clrForeground = GetNonMonthDayTextColor();
		pFont					= GetNonMonthDayTextFont();
	}

	// Hilighted
	else if (pControl->GetHighlightToday() && pDay->IsToday(dtDay))
	{
		bToday					= TRUE;
		pMetrics->clrBackground = GetHighlightTodayBackColor();
		pMetrics->clrForeground = GetHighlightTodayTextColor();
		pFont					= GetDayTextFont();
		pDC->FillSolidRect(rcDay, pMetrics->clrBackground);
		pDC->Draw3dRect(rcDay, GetHighlightTodayBorderColor(), GetHighlightTodayBorderColor());
	}

	// Selected
	else if (pControl->IsSelected(dtDay))
	{
		pMetrics->clrBackground = GetSelectedDayBackColor();
		pMetrics->clrForeground = GetSelectedDayTextColor();
		pFont					= GetDayTextFont();
		pDC->FillSolidRect(rcDay, pMetrics->clrBackground);
	}

	// fill background if used custom color from GetDayMetrics
	else if (clrBackOrig != pMetrics->clrBackground)
	{
		pDC->FillSolidRect(rcDay, pMetrics->clrBackground);
	}

	pMetrics->SetFont(pFont);
	if (!pMetrics->GetFont() || !pMetrics->GetFont()->GetSafeHandle())
	{
		pMetrics->SetFont(bThisMonthItem ? GetDayTextFont() : GetNonMonthDayTextFont());
	}

	// apply settings to the DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(pMetrics->clrForeground);
	pDC->SetBkColor(pMetrics->clrBackground);

	// make a text for drawing
	CString strText;
	strText.Format(_T("%i"), dtDay.GetDay());

	// draw item content
#ifdef _XTP_ACTIVEX
	CPictureHolder* pPictureHolder = pMetrics->GetPictureHolder();
	if ((pPictureHolder != NULL) && (pPictureHolder->GetType() != PICTYPE_NONE)
		&& (pPictureHolder->GetType() != PICTYPE_UNINITIALIZED))
	{
		// draw item picture
		pPictureHolder->Render(pDC, rcDay, rcDay);
	}
	else
#endif
	{
		// draw item text
		CXTPFontDC fntdc(pDC, pMetrics->GetFont());
		pDC->DrawText(strText, &rcDay, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP);
	}

	CRect rcFocus = rcDay;

	if (pControl->IsFocused(dtDay) && pControl->GetSafeHwnd() == ::GetFocus())
	{
		if (!bToday)
		{
			pDC->DrawFocusRect(&rcFocus);
		}
		else
		{
			COLORREF clrFocus = GetHighlightTodayColor();
			clrFocus		  = clrFocus ^ m_clr3DShadow;

			rcFocus.DeflateRect(0, 0, XTP_DPI_X(1), XTP_DPI_Y(1));

			CXTPPenDC autoPen(pDC->GetSafeHdc(), PS_DOT, 1, clrFocus);
			pDC->MoveTo(rcFocus.TopLeft());
			pDC->LineTo(rcFocus.right, rcFocus.top);
			pDC->LineTo(rcFocus.right, rcFocus.bottom);
			pDC->LineTo(rcFocus.left, rcFocus.bottom);
			pDC->LineTo(rcFocus.left, rcFocus.top);
		}
	}

	pMetrics->InternalRelease();

	return TRUE;
}

void CXTPDatePickerThemeOffice2013::DrawDaysOfWeek(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
{
	if (pMonth->m_rcDaysOfWeek.IsRectEmpty())
		return;

	CXTPDatePickerControl* pControl = pMonth->m_pControl;

	// draw days of week text
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetDaysOfWeekTextColor());

	CFont* pFont = GetDaysOfWeekTextFont();
	LOGFONT lfFont;
	pFont->GetLogFont(&lfFont);

	for (int nDayDelta = 0; nDayDelta < XTP_WEEK_DAYS; nDayDelta++)
	{
		// calc item rect
		CXTPDatePickerItemDay* pDay = pMonth->GetDay(nDayDelta);
		CRect rcItem(pDay->GetRect());
		rcItem.top	= pMonth->m_rcDaysOfWeek.top;
		rcItem.bottom = pMonth->m_rcDaysOfWeek.bottom - XTP_DPI_Y(2);

		// get item text
		CString strText(pControl->GetDayOfWeekName(pDay->GetDate().GetDayOfWeek()));
		strText.MakeUpper();

		// compute fitting font
		lfFont.lfHeight = CXTPDrawHelpers::ComputeFittingFontHeight(pDC, strText,
																	strText.GetLength(), rcItem,
																	*pFont);

		CXTPFont scaledFont;
		scaledFont.CreateFontIndirect(&lfFont);
		CXTPFontDC fontDC(pDC, GetDaysOfWeekTextFont());

		pDC->DrawText(strText, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
}

void CXTPDatePickerThemeOffice2013::DrawBorder(CDC* pDC, const CXTPDatePickerControl* pControl,
											   CRect& rcClient, BOOL bDraw)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pControl);
	UNUSED_ALWAYS(rcClient);
	UNUSED_ALWAYS(bDraw);

	// do nothing
}
