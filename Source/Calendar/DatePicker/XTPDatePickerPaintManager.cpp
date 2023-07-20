// XTPDatePickerPaintManager.cpp: implementation of the CXTPDatePickerPaintManager class.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "Calendar/Resource.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"
#include "Calendar/DatePicker/XTPDatePickerItemMonth.h"
#include "Calendar/DatePicker/XTPDatePickerItemDay.h"

#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPDatePickerPaintManager::CXTPDatePickerPaintManager()
{
	m_bEnableTheme = TRUE;

	m_nPaintTheme = xtpCalendarThemeOffice2000;

	m_themeButton = new CXTPWinThemeWrapper();

	m_szDayMargin.cx = XTP_DPI_X(6);
	m_szDayMargin.cy = XTP_DPI_Y(2);
	m_szHdrMargin.cx = 0;
	m_szHdrMargin.cy = XTP_DPI_Y(4);
	m_szDoWMargin.cx = 0;
	m_szDoWMargin.cy = XTP_DPI_Y(5);

	// initialize default fonts
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	SetFontIndirect(&lfIcon);

	// init metrics
	RefreshMetrics();

	// init cursor
	m_hCursorBack = XTPResourceManager()->LoadCursor(XTP_IDC_DATEPICKER_BACKARROW);
	XTPResourceManager()->AssertValid(m_hCursorBack != 0);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPDatePickerPaintManager::~CXTPDatePickerPaintManager()
{
	SAFE_DELETE(m_themeButton);
}

void CXTPDatePickerPaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	// get system params
	m_clr3DLight  = GetSysColor(COLOR_3DHILIGHT);
	m_clr3DText   = GetSysColor(COLOR_BTNTEXT);
	m_clr3DShadow = GetSysColor(COLOR_3DSHADOW);
	m_clr3DFace   = GetSysColor(COLOR_BTNFACE);

	m_clrButtonFace.SetStandardValue(m_clr3DFace);
	m_clrButtonText.SetStandardValue(m_clr3DText);

	// initialize default date picker paint manager settings
	m_clrControlBack.SetStandardValue(GetSysColor(COLOR_WINDOW));

	m_clrDayBack.SetStandardValue(m_clrControlBack);
	m_clrDayText.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
	m_clrNonMonthDayBack.SetStandardValue(m_clrDayBack);

	m_clrSelectedDayBack.SetStandardValue(m_clr3DFace);
	m_clrSelectedDayText.SetStandardValue(m_clr3DText);

	m_clrStaticBorder = GetXtremeColor(XPCOLOR_STATICFRAME);

	m_clrMonthHeaderBack.SetStandardValue(m_clr3DFace);
	m_clrMonthHeaderText.SetStandardValue(m_clrDayText);
	m_clrDaysOfWeekBack.SetStandardValue(m_clrDayBack);
	m_clrDaysOfWeekText.SetStandardValue(m_clrDayText);
	m_clrWeekNumbersBack.SetStandardValue(m_clrDayBack);
	m_clrWeekNumbersText.SetStandardValue(m_clrDayText);

	m_clrListControlBack.SetStandardValue(GetSysColor(COLOR_WINDOW));
	m_clrListControlText.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));

	m_clrHighlightToday.SetStandardValue(RGB(130, 0, 0));

	m_clrNonMonthDayText.SetStandardValue(m_clr3DShadow);
	m_clrMonthBorder.SetStandardValue(m_clr3DShadow);

	m_clrMonthHeaderButton.SetStandardValue(m_clr3DText);

	m_themeButton->CloseThemeData();
}

void CXTPDatePickerPaintManager::SetFontIndirect(LOGFONT* lFont)
{
	// initialize default day item font
	m_xtpFontDay.DeleteObject();
	VERIFY(m_xtpFontDay.CreateFontIndirect(lFont));

	LOGFONT lfBold  = *lFont;
	lfBold.lfWeight = FW_BOLD;
	m_xtpFontDayBold.DeleteObject();
	VERIFY(m_xtpFontDayBold.CreateFontIndirect(&lfBold));

	// initialize default non month day item font
	m_xtpFontNonMonthDay.DeleteObject();
	VERIFY(m_xtpFontNonMonthDay.CreateFontIndirect(lFont));

	// initialize default header font
	m_xtpFontHeader.DeleteObject();
	VERIFY(m_xtpFontHeader.CreateFontIndirect(lFont));

	// initialize default days of week font
	m_xtpFontDaysOfWeek.DeleteObject();
	VERIFY(m_xtpFontDaysOfWeek.CreateFontIndirect(lFont));

	// initialize default popup list control font
	m_xtpFontListControl.DeleteObject();
	VERIFY(m_xtpFontListControl.CreateFontIndirect(lFont));

	// initialize default button font
	m_xtpFontButton.DeleteObject();
	VERIFY(m_xtpFontButton.CreateFontIndirect(lFont));

	// initialize default week numbers font
	m_xtpFontWeekNumbers.DeleteObject();
	lFont->lfHeight -= lFont->lfHeight / 3;
	VERIFY(m_xtpFontWeekNumbers.CreateFontIndirect(lFont));
}

// void CXTPDatePickerPaintManager::SetPaintTheme(XTPCalendarTheme ePaintTheme)
//{
//	m_nPaintTheme = ePaintTheme;
//	RefreshMetrics();
//}

void CXTPDatePickerPaintManager::DrawBackground(CDC* pDC, CRect rcClient)
{
	pDC->FillSolidRect(rcClient, GetControlBackColor());
}

void CXTPDatePickerPaintManager::DrawBorder(CDC* pDC, const CXTPDatePickerControl* pControl,
											CRect& rcClient, BOOL bDraw)
{
	XTPDatePickerBorderStyle borderStyle = pControl->GetBorderStyle();
	BOOL bThemed						 = m_themeButton->IsAppThemed();

	if (bDraw)
	{
		if (borderStyle == xtpDatePickerBorder3D)
		{
			pDC->Draw3dRect(0, 0, rcClient.Width(), rcClient.Height(), m_clr3DFace, m_clr3DText);
			pDC->Draw3dRect(XTP_DPI_X(1), XTP_DPI_Y(1), rcClient.Width() - XTP_DPI_X(2),
							rcClient.Height() - XTP_DPI_Y(2), m_clrControlBack, m_clr3DShadow);
			pDC->Draw3dRect(XTP_DPI_X(2), XTP_DPI_Y(2), rcClient.Width() - XTP_DPI_X(4),
							rcClient.Height() - XTP_DPI_Y(4), m_clr3DFace, m_clr3DFace);
			pDC->Draw3dRect(XTP_DPI_X(3), XTP_DPI_Y(3), rcClient.Width() - XTP_DPI_X(6),
							rcClient.Height() - XTP_DPI_Y(6), m_clr3DShadow, m_clrControlBack);
		}
		if ((borderStyle == xtpDatePickerBorderStatic)
			|| (borderStyle == xtpDatePickerBorderOffice && bThemed))
		{
			pDC->Draw3dRect(0, 0, rcClient.Width(), rcClient.Height(), m_clrStaticBorder,
							m_clrStaticBorder);
		}
		if (borderStyle == xtpDatePickerBorderOffice && !bThemed)
		{
			pDC->Draw3dRect(0, 0, rcClient.Width(), rcClient.Height(), m_clr3DFace, m_clr3DText);
			pDC->Draw3dRect(XTP_DPI_X(1), XTP_DPI_Y(1), rcClient.Width() - XTP_DPI_X(2),
							rcClient.Height() - XTP_DPI_Y(2), m_clrControlBack, m_clr3DShadow);
			pDC->Draw3dRect(XTP_DPI_X(2), XTP_DPI_Y(2), rcClient.Width() - XTP_DPI_X(4),
							rcClient.Height() - XTP_DPI_Y(4), m_clr3DFace, m_clr3DFace);
		}
	}
	else
	{
		if (borderStyle == xtpDatePickerBorder3D)
		{
			rcClient.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4));
		}
		if ((borderStyle == xtpDatePickerBorderStatic)
			|| (borderStyle == xtpDatePickerBorderOffice && bThemed))
		{
			rcClient.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
		}
		if (borderStyle == xtpDatePickerBorderOffice && !bThemed)
		{
			rcClient.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));
		}
	}
}

void CXTPDatePickerPaintManager::SetButtonFontIndirect(LOGFONT* lFont)
{
	m_xtpFontButton.DeleteObject();
	m_xtpFontButton.CreateFontIndirect(lFont);
}

void CXTPDatePickerPaintManager::SetButtonFont(CFont* fontButton)
{
	LOGFONT lFont;
	fontButton->GetLogFont(&lFont);
	SetButtonFontIndirect(&lFont);
}

void CXTPDatePickerPaintManager::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2,
										  COLORREF clrColor)
{
	CXTPPenDC pen(*pDC, clrColor);
	CXTPBrushDC brush(*pDC, clrColor);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}

void CXTPDatePickerPaintManager::DrawScrollTriangle(CDC* pDC, CRect rcSpot, BOOL bLeftDirection,
													COLORREF clrColor, BOOL bYears)
{
	CPoint ptCenter(rcSpot.CenterPoint());

	if (bLeftDirection)
	{
		Triangle(pDC, ptCenter, CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(4)),
				 CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y + XTP_DPI_Y(4)), clrColor);

		if (bYears)
		{
			Triangle(pDC, CPoint(ptCenter.x - XTP_DPI_X(8), ptCenter.y),
					 CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(4)),
					 CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y + XTP_DPI_Y(4)), clrColor);

			Triangle(pDC, CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y),
					 CPoint(ptCenter.x + XTP_DPI_X(8), ptCenter.y - XTP_DPI_Y(4)),
					 CPoint(ptCenter.x + XTP_DPI_X(8), ptCenter.y + XTP_DPI_Y(4)), clrColor);

			// Triangle(pDC,
			//  CPoint(ptCenter.x + 2, ptCenter.y),
			//  CPoint(ptCenter.x + 3, ptCenter.y - 2),
			//  CPoint(ptCenter.x + 3, ptCenter.y + 2),
			//  RGB(255,255,255));
		}
	}
	else
	{
		Triangle(pDC, ptCenter, CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(4)),
				 CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y + XTP_DPI_Y(4)), clrColor);

		if (bYears)
		{
			Triangle(pDC, CPoint(ptCenter.x + XTP_DPI_X(8), ptCenter.y),
					 CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(4)),
					 CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y + XTP_DPI_Y(4)), clrColor);

			Triangle(pDC, CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y),
					 CPoint(ptCenter.x - XTP_DPI_X(8), ptCenter.y - XTP_DPI_Y(4)),
					 CPoint(ptCenter.x - XTP_DPI_X(8), ptCenter.y + XTP_DPI_Y(4)), clrColor);

			// Triangle(pDC,
			//  CPoint(ptCenter.x - 2, ptCenter.y),
			//  CPoint(ptCenter.x - 3, ptCenter.y - 2),
			//  CPoint(ptCenter.x - 3, ptCenter.y + 2),
			//  RGB(255,255,255));
		}
	}
}

void CXTPDatePickerPaintManager::DrawButton(CDC* pDC, const CRect& rcButton,
											const CString& strButton, BOOL bIsDown,
											BOOL bIsHighLight)
{
	UNREFERENCED_PARAMETER(bIsHighLight);

	pDC->SetBkMode(TRANSPARENT);

	CRect rc = rcButton;
	rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	pDC->FillSolidRect(rcButton, m_clrButtonFace);

	if (bIsDown)
	{
		pDC->Draw3dRect(rcButton, m_clr3DText, m_clrButtonFace);
		pDC->Draw3dRect(rc, m_clr3DShadow, m_clr3DLight);
	}
	else
	{
		pDC->Draw3dRect(rcButton, m_clrButtonFace, m_clr3DText);
		pDC->Draw3dRect(rc, m_clr3DLight, m_clr3DShadow);
	}

	if (!strButton.IsEmpty())
	{
		CXTPFontDC autoFont(pDC, &m_xtpFontButton, m_clrButtonText);
		pDC->DrawText(strButton, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

BOOL CXTPDatePickerPaintManager::DrawDay(CDC* pDC, CXTPDatePickerItemDay* pDay)
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

	if (dtDay.GetYear() > pMonth->GetMonth().GetYear())
		bNextMonthItem = TRUE;
	else if (dtDay.GetYear() < pMonth->GetMonth().GetYear())
		bPreviousMonthItem = TRUE;
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
	pMetrics->clrBackground		  = bThisMonthItem ? GetDayBackColor() : GetNonMonthDayBackColor();
	pMetrics->clrForeground		  = bThisMonthItem ? GetDayTextColor() : GetNonMonthDayTextColor();
	pMetrics->SetFont(bThisMonthItem ? GetDayTextFont() : GetNonMonthDayTextFont());

	// call callback function for changing parameters if needed
	pControl->GetDayMetrics(dtDay, pMetrics);

	// change default drawing metrics if day is selected
	if (pControl->IsSelected(dtDay))
	{
		pMetrics->clrBackground = GetSelectedDayBackColor();
		pMetrics->clrForeground = GetSelectedDayTextColor();
	}

	// apply settings to the DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(pMetrics->clrForeground);

	// if (m_bDaysTransparentBackground == FALSE && !pControl->IsSelected(dtDay))
	pDC->FillSolidRect(rcDay, pMetrics->clrBackground); // fill background

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
	BOOL bToday   = FALSE;

	// highlight today item
	if (pControl->GetHighlightToday() && pDay->IsToday(dtDay))
	{
		pDC->Draw3dRect(rcDay, GetHighlightTodayColor(), GetHighlightTodayColor());
		bToday = TRUE;
	}

	if (pControl->IsFocused(dtDay) && pControl->GetSafeHwnd() == ::GetFocus())
	{
		if (!bToday)
		{
			pDC->SetTextColor(0);
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->DrawFocusRect(&rcFocus);
		}
		else
		{
			COLORREF clrFocus = GetHighlightTodayColor();
			clrFocus		  = clrFocus ^ RGB(255, 128, 128);

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

void CXTPDatePickerPaintManager::DrawMonthHeader(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
{
	if (pMonth->m_rcHeader.IsRectEmpty())
		return;

	CXTPDatePickerControl* pControl = pMonth->m_pControl;

	COleDateTime dtMonth = pMonth->GetMonth();
	CRect rcHeader		 = pMonth->m_rcHeader;
	CRect rcLeftScroll   = pMonth->m_rcLeftScroll;
	CRect rcRightScroll  = pMonth->m_rcRightScroll;

	CXTPClientRect rcClient(pControl);
	DrawBorder(pDC, pControl, rcClient, FALSE);

	CRect rcHeaderBk  = pMonth->m_rcMonth;
	rcHeaderBk.bottom = rcHeader.bottom;

	// fill background
	pDC->FillSolidRect(rcHeaderBk, GetMonthHeaderBackColor());

	// draw header text
	pDC->SetBkColor(GetMonthHeaderBackColor());
	pDC->SetTextColor(GetMonthHeaderTextColor());

	CXTPFontDC fnt(pDC, GetHeaderTextFont());

	CString strText;

	if (!pControl->m_strYearMonthFormat.IsEmpty())
	{
		strText = CXTPCalendarUtils::GetDateFormat(dtMonth, pControl->m_strYearMonthFormat);
	}
	else
	{
		strText.Format(_T("%s %d"), (LPCTSTR)pControl->GetMonthName(dtMonth.GetMonth()),
					   dtMonth.GetYear());
	}

	pDC->DrawText(strText, &rcHeader, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	// draw left scroll triangle
	if (!rcLeftScroll.IsRectEmpty())
	{
		// draw left triangle itself
		if (pMonth->GetShowLeftScroll())
			DrawScrollTriangle(pDC, rcLeftScroll, TRUE, m_clrMonthHeaderButton,
							   pControl->m_bYearsTriangle);
	}

	// draw right scroll triangle
	if (!rcRightScroll.IsRectEmpty())
	{
		// draw right triangle itself
		if (pMonth->GetShowRightScroll())
			DrawScrollTriangle(pDC, rcRightScroll, FALSE, m_clrMonthHeaderButton,
							   pControl->m_bYearsTriangle);
	}
}

void CXTPDatePickerPaintManager::DrawDaysOfWeek(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
{
	if (pMonth->m_rcDaysOfWeek.IsRectEmpty())
		return;

	CXTPDatePickerControl* pControl = pMonth->m_pControl;

	pDC->FillSolidRect(pMonth->m_rcDaysOfWeek, GetDaysOfWeekBackColor()); // fill background

	// draw days of week text
	pDC->SetBkColor(GetDaysOfWeekBackColor());
	pDC->SetTextColor(GetDaysOfWeekTextColor());
	int nMaxX = 0;

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
		nMaxX		  = rcItem.right;

		// get item text
		CString strText(pControl->GetDayOfWeekName(pDay->GetDate().GetDayOfWeek()));

		// compute fitting font
		lfFont.lfHeight = CXTPDrawHelpers::ComputeFittingFontHeight(pDC, strText,
																	strText.GetLength(), rcItem,
																	*pFont);

		CXTPFont scaledFont;
		scaledFont.CreateFontIndirect(&lfFont);
		CXTPFontDC fontDC(pDC, GetDaysOfWeekTextFont());

		pDC->DrawText(strText, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}

	// draw bottom line on days of the week
	CRect rcBottomLine(pMonth->m_rcDaysOfWeek);
	rcBottomLine.bottom -= XTP_DPI_Y(1);
	rcBottomLine.top   = rcBottomLine.bottom - XTP_DPI_Y(1);
	rcBottomLine.left  = pMonth->m_rcWeekNumbers.right - XTP_DPI_X(1);
	rcBottomLine.right = nMaxX;
	pDC->FillSolidRect(rcBottomLine, m_clrMonthBorder);
}

void CXTPDatePickerPaintManager::DrawWeekNumbers(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
{
	CXTPDatePickerControl* pControl = pMonth->m_pControl;

	if (!pControl->GetShowWeekNumbers() || pMonth->m_rcWeekNumbers.IsRectEmpty())
		return;

	// fill background
	pDC->FillSolidRect(pMonth->m_rcWeekNumbers, GetWeekNumbersBackColor());

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

CSize CXTPDatePickerPaintManager::CalcDayRect(CDC* pDC)
{
	CFont* fontOld = pDC->SelectObject(&m_xtpFontDay);
	CSize szDay	= pDC->GetTextExtent(_T("30"), 2);
	szDay.cx += m_szDayMargin.cx;
	szDay.cy += m_szDayMargin.cy;
	pDC->SelectObject(fontOld);
	return szDay;
}

CSize CXTPDatePickerPaintManager::CalcMonthHeaderRect(CDC* pDC)
{
	CFont* fontOld = pDC->SelectObject(&m_xtpFontHeader);
	CSize szHeader = pDC->GetTextExtent(_T(" "), 1);
	szHeader.cx += m_szHdrMargin.cx;
	szHeader.cy += m_szHdrMargin.cy;
	pDC->SelectObject(fontOld);
	return szHeader;
}

CSize CXTPDatePickerPaintManager::CalcDayOfWeekRect(CDC* pDC)
{
	CFont* fontOld   = pDC->SelectObject(&m_xtpFontDaysOfWeek);
	CSize szWeekText = pDC->GetTextExtent(_T(" "), 1);
	szWeekText.cx += m_szDoWMargin.cx;
	szWeekText.cy += m_szDoWMargin.cy;
	pDC->SelectObject(fontOld);
	return szWeekText;
}

CSize CXTPDatePickerPaintManager::CalcWeekNumbersRect(CDC* pDC)
{
	CFont* fontOld	 = pDC->SelectObject(&m_xtpFontWeekNumbers);
	CSize szWeekNumber = pDC->GetTextExtent(_T("88"), 2);
	szWeekNumber.cx += XTP_DPI_X(8);
	pDC->SelectObject(fontOld);
	return szWeekNumber;
}

CSize CXTPDatePickerPaintManager::CalcMonthRect(CDC* pDC)
{
	CSize szDay(CalcDayRect(pDC));
	CSize szHeader(CalcMonthHeaderRect(pDC));
	CSize szWeekText(CalcDayOfWeekRect(pDC));
	CSize szWeekNumber(CalcWeekNumbersRect(pDC));

	CSize szMonth(0, 0);
	szMonth.cx = (szDay.cx * 7 + szWeekNumber.cx * 2);
	szMonth.cy = (szDay.cy * 6 + szWeekText.cy + szHeader.cy);

	return szMonth;
}

#ifdef _XTP_ACTIVEX

#	define DISP_PROPERTY_UN_ID(theClass, szExternalName, dispid, vtPropType)                      \
		DISP_PROPERTY_EX_ID(theClass, #szExternalName, dispid, OleGet##szExternalName,             \
							OleSet##szExternalName, vtPropType)

#	define IMPLEMENT_PROPERTY_CLR(theClass, theProperty, theMember)                               \
		COLORREF theClass::OleGet##theProperty()                                                   \
		{                                                                                          \
			return theMember;                                                                      \
		}                                                                                          \
		void theClass::OleSet##theProperty(COLORREF oleColor)                                      \
		{                                                                                          \
			theMember = AxTranslateColor(oleColor);                                                \
		}

//===========================================================================
BEGIN_DISPATCH_MAP(CXTPDatePickerPaintManager, CXTPCmdTarget)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, ControlBackColor, 1, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, HighlightTodayColor, 2, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, SelectedDayBackColor, 3, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, MonthHeaderBackColor, 4, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, MonthHeaderTextColor, 5, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, SelectedDayTextColor, 6, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, MonthBorderColor, 7, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, MonthHeaderButtonColor, 8, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, DayBackColor, 9, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, DayTextColor, 10, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, NonMonthDayBackColor, 11, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, NonMonthDayTextColor, 12, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, DaysOfWeekBackColor, 13, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, DaysOfWeekTextColor, 14, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, WeekNumbersBackColor, 15, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, WeekNumbersTextColor, 16, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, ButtonTextColor, 17, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, ButtonFaceColor, 18, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, ListControlBackColor, 19, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPDatePickerPaintManager, ListControlTextColor, 20, VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPDatePickerPaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPDatePickerPaintManager, XTPDIID_DatePickerPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDatePickerPaintManager, XTPDIID_DatePickerPaintManager)

IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, ControlBackColor, m_clrControlBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, HighlightTodayColor, m_clrHighlightToday)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, SelectedDayBackColor, m_clrSelectedDayBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, MonthHeaderBackColor, m_clrMonthHeaderBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, MonthHeaderTextColor, m_clrMonthHeaderText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, SelectedDayTextColor, m_clrSelectedDayText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, MonthBorderColor, m_clrMonthBorder)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, MonthHeaderButtonColor, m_clrMonthHeaderButton)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, DayBackColor, m_clrDayBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, DayTextColor, m_clrDayText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, NonMonthDayBackColor, m_clrNonMonthDayBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, NonMonthDayTextColor, m_clrNonMonthDayText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, DaysOfWeekBackColor, m_clrDaysOfWeekBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, DaysOfWeekTextColor, m_clrDaysOfWeekText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, WeekNumbersBackColor, m_clrWeekNumbersBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, WeekNumbersTextColor, m_clrWeekNumbersText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, ButtonTextColor, m_clrButtonText)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, ButtonFaceColor, m_clrButtonFace)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, ListControlBackColor, m_clrListControlBack)
IMPLEMENT_PROPERTY_CLR(CXTPDatePickerPaintManager, ListControlTextColor, m_clrListControlText)

#endif
