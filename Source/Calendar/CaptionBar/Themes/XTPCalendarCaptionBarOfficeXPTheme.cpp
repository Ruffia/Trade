// XTPCalendarCaptionBarOfficeXPTheme.cpp : implementation of the
// CXTPCalendarCaptionBarOffice2000Theme class.
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
#include "Common/XTPCasting.h"
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
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarThemePart.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarTheme.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarPaintManager.h"

#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOffice2000Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOfficeXPTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarOfficeXPTheme

CXTPCalendarCaptionBarOfficeXPTheme::CXTPCalendarCaptionBarOfficeXPTheme(
	CXTPCalendarCaptionBarTheme* pTheme /*=NULL*/)
	: CXTPCalendarCaptionBarOffice2000Theme(pTheme)
{
	m_nPaintTheme = xtpCalendarThemeOfficeXP;
}

CXTPCalendarCaptionBarOfficeXPTheme::~CXTPCalendarCaptionBarOfficeXPTheme()
{
}

void CXTPCalendarCaptionBarOfficeXPTheme::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager::RefreshMetrics();

	m_pTheme->m_nScrollDateButtonsMargin = XTP_DPI_X(20);

	m_clrSwitchViewBarBk.SetStandardValue(GetSysColor(COLOR_BTNFACE));
	m_clrScrollDateBarBk.SetStandardValue(RGB(130, 130, 130));
	m_clrBorders[0].SetStandardValue(RGB(130, 130, 130));
	m_clrBorders[1].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[2].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[3].SetStandardValue(m_clrBorders[0]);
	m_clrTextColor.SetStandardValue(RGB(255, 255, 255));

	LOGFONT lfBaseFont;
	::ZeroMemory(&lfBaseFont, sizeof(lfBaseFont));
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfBaseFont));
	m_fntTextFont.SetStandardValue(&lfBaseFont);

	int nCount = m_pTheme->m_arMembers.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarCaptionBarThemePart* pPart = m_pTheme->m_arMembers.GetAt(i);
		ASSERT(pPart);
		if (pPart)
			pPart->RefreshMetrics();
	}
}

void CXTPCalendarCaptionBarOfficeXPTheme::PartDraw(CXTPRadioButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CSize szRadio(XTP_DPI_X(13), XTP_DPI_Y(13));
	CRect rcRadio = pBarPart->m_rcRect;

	rcRadio.right = rcRadio.left + szRadio.cx;

	UINT uFlags = UINT(DFCS_BUTTONRADIO | DFCS_FLAT
					   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
					   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

	::DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, uFlags);

	// drawText
	CXTPFontDC autoFont(pDC, m_fntTextFont, pBarPart->GetTextColor());
	pDC->SetBkMode(TRANSPARENT);

	CRect rcText = pBarPart->m_rcRect;
	rcText.left  = rcRadio.right + XTP_DPI_X(9);

	pDC->DrawText(pBarPart->GetText(), rcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

	rcText.left -= XTP_DPI_X(1);
	rcText.right += XTP_DPI_X(1);

	if (pBarPart->m_nState & xtpBtnStatePressed)
		pDC->DrawFocusRect(rcText);
}

void CXTPCalendarCaptionBarOfficeXPTheme::PartDraw(CXTPCheckboxButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CSize szRadio(XTP_DPI_X(13), XTP_DPI_Y(13));
	CRect rcRadio = pBarPart->m_rcRect;

	rcRadio.right = rcRadio.left + szRadio.cx;

	UINT uFlags = UINT(DFCS_BUTTONCHECK | DFCS_FLAT
					   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
					   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

	::DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, uFlags);

	// drawText
	CXTPFontDC autoFont(pDC, m_fntTextFont, pBarPart->GetTextColor());
	pDC->SetBkMode(TRANSPARENT);

	CRect rcText = pBarPart->m_rcRect;
	rcText.left  = rcRadio.right + XTP_DPI_X(9);

	pDC->DrawText(pBarPart->GetText(), rcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

	rcText.left -= XTP_DPI_X(1);
	rcText.right += XTP_DPI_X(1);

	if (pBarPart->m_nState & xtpBtnStatePressed)
		pDC->DrawFocusRect(rcText);
}
