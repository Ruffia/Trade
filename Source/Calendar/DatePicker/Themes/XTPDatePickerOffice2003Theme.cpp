// XTPDatePickerOffice2003Theme.cpp: interface for the CXTPDatePickerOffice2003Theme class.
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
#include "Common/XTPWinThemeWrapper.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"
#include "Calendar/DatePicker/Themes/XTPDatePickerOffice2003Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPDatePickerThemeOffice2003::CXTPDatePickerThemeOffice2003()
{
	m_nPaintTheme = xtpCalendarThemeOffice2003;
}

CXTPDatePickerThemeOffice2003::~CXTPDatePickerThemeOffice2003()
{
}

void CXTPDatePickerThemeOffice2003::RefreshMetrics()
{
	CXTPDatePickerPaintManager::RefreshMetrics();

	m_clrSelectedDayBack.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrSelectedDayText.SetStandardValue(m_clrDayText);
	m_clrHighlightToday.SetStandardValue(GetSysColor(COLOR_HIGHLIGHT));

	if (m_bEnableTheme && !XTPColorManager()->IsLunaColorsDisabled())
	{
		switch (XTPColorManager()->GetCurrentSystemTheme())
		{
			case xtpSystemThemeBlue:
			case xtpSystemThemeRoyale:
			case xtpSystemThemeAero:
				m_clrSelectedDayBack.SetStandardValue(RGB(0xFB, 0xE6, 0x94));
				m_clrHighlightToday.SetStandardValue(RGB(0xBB, 0x55, 0x03));
				m_clrMonthHeaderBack.SetStandardValue(RGB(0x9E, 0xBE, 0xF5));
				break;
			case xtpSystemThemeOlive:
				m_clrSelectedDayBack.SetStandardValue(RGB(251, 230, 148));
				m_clrHighlightToday.SetStandardValue(RGB(187, 85, 3));
				m_clrMonthHeaderBack.SetStandardValue(RGB(217, 217, 167));
				break;
			case xtpSystemThemeSilver:
				m_clrSelectedDayBack.SetStandardValue(RGB(251, 230, 148));
				m_clrHighlightToday.SetStandardValue(RGB(187, 85, 3));
				m_clrMonthHeaderBack.SetStandardValue(RGB(215, 215, 229));
				break;
			case xtpSystemThemeAuto:
			case xtpSystemThemeDefault:
			case xtpSystemThemeUnknown:
				// do nothing
				break;
		}

		m_themeButton->OpenThemeData(NULL, L"BUTTON");
	}
}

void CXTPDatePickerThemeOffice2003::DrawButton(CDC* pDC, const CRect& rcButton,
											   const CString& strButton, BOOL bIsDown,
											   BOOL bIsHighLight)
{
	if (m_themeButton->IsAppThemeActive())
	{
		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON,
										   bIsDown ? PBS_PRESSED
												   : bIsHighLight ? PBS_HOT : PBS_NORMAL,
										   &rcButton, NULL);

		CRect rc = rcButton;
		rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

		CXTPFontDC autoFont(pDC, &m_xtpFontButton, m_clrButtonText);
		pDC->DrawText(strButton, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	else
	{
		CXTPDatePickerPaintManager::DrawButton(pDC, rcButton, strButton, bIsDown, bIsHighLight);
	}
}
