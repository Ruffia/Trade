// XTPDatePickerResourceTheme.cpp: interface for the CXTPDatePickerResourceTheme class.
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
#include "Common/XTPResourceImage.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/DatePicker/Themes/XTPDatePickerResourceTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPDatePickerThemeResource::CXTPDatePickerThemeResource()
{
	m_nPaintTheme = xtpCalendarThemeResource;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPDatePickerThemeResource::~CXTPDatePickerThemeResource()
{
}

void CXTPDatePickerThemeResource::RefreshMetrics()
{
	CXTPDatePickerPaintManager::RefreshMetrics();

	m_clrMonthBorder.SetStandardValue(GetResourceColor(_T("MonthBorder"), RGB(154, 198, 255)));
	m_clrMonthHeaderBack.SetStandardValue(
		GetResourceColor(_T("MonthHeaderBack"), RGB(191, 219, 255)));
	m_clrMonthHeaderText.SetStandardValue(GetResourceColor(_T("MonthHeaderText"), RGB(0, 0, 0)));
	m_clrMonthHeaderButton.SetStandardValue(
		GetResourceColor(_T("MonthHeaderButton"), RGB(111, 157, 217)));

	m_clrControlBack.SetStandardValue(GetResourceColor(_T("ControlBack"), RGB(255, 255, 255)));

	m_clrHighlightToday.SetStandardValue(GetResourceColor(_T("HighlightToday"), RGB(187, 85, 3)));

	m_clrDayBack.SetStandardValue(m_clrControlBack);
	m_clrDayText.SetStandardValue(GetResourceColor(_T("DayText"), RGB(0, 0, 0)));

	m_clrSelectedDayBack.SetStandardValue(
		GetResourceColor(_T("SelectedDayBack"), RGB(251, 200, 79)));
	m_clrSelectedDayText.SetStandardValue(GetResourceColor(_T("SelectedDayText"), m_clrDayText));

	m_clrNonMonthDayBack.SetStandardValue(m_clrDayBack);
	m_clrNonMonthDayText.SetStandardValue(GetResourceColor(_T("NonMonthDayText"), m_clr3DShadow));

	m_clrDaysOfWeekBack.SetStandardValue(m_clrDayBack);
	m_clrDaysOfWeekText.SetStandardValue(m_clrDayText);

	m_clrWeekNumbersBack.SetStandardValue(m_clrDayBack);
	m_clrWeekNumbersText.SetStandardValue(m_clrDayText);

	m_clrButtonText.SetStandardValue(RGB(0, 0, 0));
}

void CXTPDatePickerThemeResource::DrawButton(CDC* pDC, const CRect& rcButton,
											 const CString& strButton, BOOL bIsDown,
											 BOOL bIsHighLight)
{
	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));
	if (!pImage)
	{
		CXTPDatePickerPaintManager::DrawButton(pDC, rcButton, strButton, bIsDown, bIsHighLight);
		return;
	}

	pImage->DrawImage(pDC, rcButton, pImage->GetSource(bIsDown ? 2 : bIsHighLight ? 1 : 0, 5),
					  CRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(4)), 0xFF00FF);

	pDC->SetBkMode(TRANSPARENT);

	if (!strButton.IsEmpty())
	{
		CXTPFontDC autoFont(pDC, &m_xtpFontButton, m_clrButtonText);

		CRect rcText(rcButton);
		rcText.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

		pDC->DrawText(strButton, rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

COLORREF CXTPDatePickerThemeResource::GetResourceColor(LPCTSTR pcszColorName, COLORREF clrDefault)
{
	if (this == NULL)
		return clrDefault;

	COLORREF clrColor = COLORREF_NULL;

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		clrColor = pImages->GetImageColor(_T("DatePicker"), pcszColorName);
	}

	if (clrColor == COLORREF_NULL)
	{
		clrColor = clrDefault;
	}

	return clrColor;
}
