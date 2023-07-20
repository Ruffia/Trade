// XTPDatePickerOffice2000Theme.cpp: interface for the CXTPDatePickerOffice2000Theme class.
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
#include "Common/XTPDrawHelpers.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/DatePicker/Themes/XTPDatePickerOffice2000Theme.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"
#include "Calendar/DatePicker/XTPDatePickerItemMonth.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPDatePickerThemeOffice2000::CXTPDatePickerThemeOffice2000()
{
	m_nPaintTheme = xtpCalendarThemeOffice2000;
}

CXTPDatePickerThemeOffice2000::~CXTPDatePickerThemeOffice2000()
{
}

void CXTPDatePickerThemeOffice2000::DrawMonthHeader(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
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
	int nVertDiv = 0;

	if (pMonth->m_nColumn == 0)
		rcHeaderBk.left = rcClient.left + XTP_DPI_X(1);

	if (pMonth->m_nColumn == pControl->GetCols() - 1)
	{
		rcHeaderBk.right = rcClient.right;
		nVertDiv		 = 2;
	}
	else
	{
		rcHeaderBk.right += XTP_DPI_X(2);
		nVertDiv = 1;
	}

	pDC->FillSolidRect(rcHeaderBk, GetMonthHeaderBackColor());

	pDC->FillSolidRect(rcHeaderBk.left - XTP_DPI_X(1), rcHeaderBk.bottom - XTP_DPI_Y(1),
					   rcHeaderBk.Width() + XTP_DPI_X(1), XTP_DPI_Y(1), m_clr3DShadow);

	if (nVertDiv == 1)
	{
		CRect rcDiv = rcHeaderBk;
		rcDiv.DeflateRect(0, XTP_DPI_Y(1), XTP_DPI_X(3), XTP_DPI_Y(2));
		rcDiv.left = rcDiv.right - XTP_DPI_X(1);
		pDC->FillSolidRect(rcDiv, m_clr3DShadow);

		rcDiv.left += XTP_DPI_X(1);
		rcDiv.right += XTP_DPI_X(1);
		pDC->FillSolidRect(rcDiv, m_clrControlBack);
	}

	else if (nVertDiv == 2)
	{
		CRect rcDiv = rcHeaderBk;
		rcDiv.left  = rcDiv.right - XTP_DPI_X(1);
		rcDiv.top -= XTP_DPI_Y(1);
		pDC->FillSolidRect(rcDiv, m_clr3DShadow);
	}

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
