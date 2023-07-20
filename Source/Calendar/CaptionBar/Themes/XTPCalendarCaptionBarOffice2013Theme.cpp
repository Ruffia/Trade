// XTPCalendarCaptionBarOffice2013Theme.cpp : implementation of the
// CXTPCalendarCaptionBarOffice2003Theme class.
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
#include "Common/XTPResourceImage.h"
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
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOffice2003Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOffice2013Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarOffice2013Theme

CXTPCalendarCaptionBarOffice2013Theme::CXTPCalendarCaptionBarOffice2013Theme(
	CXTPCalendarCaptionBarTheme* pTheme /*=NULL*/)
	: CXTPCalendarCaptionBarOffice2003Theme(pTheme)
{
	m_nPaintTheme = xtpCalendarThemeOffice2013;
}

CXTPCalendarCaptionBarOffice2013Theme::~CXTPCalendarCaptionBarOffice2013Theme()
{
}

void CXTPCalendarCaptionBarOffice2013Theme::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager::RefreshMetrics();

	m_pTheme->m_nScrollDateButtonsMargin = XTP_DPI_X(
		XTPIniInt(_T("Calendar.CaptionBar"), _T("ScrollDateButtonsMargin"), 20));

	m_clrScrollDateBarBk.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ScrollDateBarBack"), RGB(255, 255, 255)));
	m_clrSwitchViewBarBk.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("SwitchViewBarBack"), RGB(255, 255, 255)));
	m_clrTextColor.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("TextColor"), RGB(38, 38, 38)));
	m_clrBorders[0].SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("BorderLeft"), RGB(212, 212, 212)));
	m_clrBorders[1].SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("BorderTop"), RGB(212, 212, 212)));
	m_clrBorders[2].SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("BorderRight"), RGB(212, 212, 212)));
	m_clrBorders[3].SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("BorderBottom"), RGB(212, 212, 212)));
	m_clrScrollTriangleColor.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ScrollTriangleColor"), RGB(119, 119, 119)));
	m_clrButtonHighlight.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonHighlight"), RGB(205, 230, 247)));
	m_clrButtonHighlightText.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonHighlightText"), RGB(119, 119, 119)));
	m_clrButtonHighlightBorder.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonHighlightBorder"), RGB(146, 194, 224)));
	m_clrButtonPressed.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonPressed"), RGB(177, 214, 240)));
	m_clrButtonPressedText.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonPressedText"), RGB(119, 119, 119)));
	m_clrButtonPressedBorder.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonPressedBorder"), RGB(146, 194, 224)));
	m_clrButtonChecked.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonChecked"), RGB(177, 214, 240)));
	m_clrButtonCheckedText.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonCheckedText"), RGB(119, 119, 119)));
	m_clrButtonCheckedBorder.SetStandardValue(
		XTPIniColor(_T("Calendar.CaptionBar"), _T("ButtonCheckedBorder"), RGB(146, 194, 224)));

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

void CXTPCalendarCaptionBarOffice2013Theme::PartRefresh(CXTPSwitchViewButtonPart* pBarPart)
{
	pBarPart->m_clrTextColor.SetStandardValue(COLORREF_NULL);
}

void CXTPCalendarCaptionBarOffice2013Theme::PartRefresh(CXTPRadioButtonPart* pBarPart)
{
	pBarPart->m_clrTextColor.SetStandardValue(COLORREF_NULL);
}

void CXTPCalendarCaptionBarOffice2013Theme::PartRefresh(CXTPCheckboxButtonPart* pBarPart)
{
	pBarPart->m_clrTextColor.SetStandardValue(COLORREF_NULL);
}

void CXTPCalendarCaptionBarOffice2013Theme::DrawBack(CDC* pDC, CRect rcRect, int nState)
{
	if ((nState & xtpBtnStateChecked) != 0)
	{
		if ((nState & xtpBtnStateMouseOver) != 0)
		{
			pDC->FillSolidRect(rcRect, m_clrButtonChecked);
			pDC->Draw3dRect(rcRect, m_clrButtonCheckedBorder, m_clrButtonCheckedBorder);
		}
		else
		{
			pDC->FillSolidRect(rcRect, m_clrButtonPressed);
			pDC->Draw3dRect(rcRect, m_clrButtonPressedBorder, m_clrButtonPressedBorder);
		}
	}
	else if ((nState & xtpBtnStatePressed) != 0)
	{
		pDC->FillSolidRect(rcRect, m_clrButtonPressed);
		pDC->Draw3dRect(rcRect, m_clrButtonPressedBorder, m_clrButtonPressedBorder);
	}
	else if ((nState & xtpBtnStateMouseOver) != 0)
	{
		pDC->FillSolidRect(rcRect, m_clrButtonHighlight);
		pDC->Draw3dRect(rcRect, m_clrButtonHighlightBorder, m_clrButtonHighlightBorder);
	}
}

void CXTPCalendarCaptionBarOffice2013Theme::PartDraw(CXTPSwitchViewButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	DrawBack(pDC, pBarPart->m_rcRect, pBarPart->m_nState);

	// drawText
	CXTPFontDC autoFont(pDC, m_fntTextFont, pBarPart->GetTextColor());

	CRect rcText = pBarPart->m_rcRect;
	rcText.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(pBarPart->GetText(), rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

void CXTPCalendarCaptionBarOffice2013Theme::PartDraw(CXTPScrollDateButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	DrawBack(pDC, pBarPart->m_rcRect, pBarPart->m_nState);

	COLORREF clrScrollTriangle = m_clrScrollTriangleColor;

	if ((pBarPart->m_nState & xtpBtnStateChecked) != 0)
	{
		clrScrollTriangle = m_clrButtonCheckedText;
	}
	else if ((pBarPart->m_nState & xtpBtnStatePressed) != 0)
	{
		clrScrollTriangle = m_clrButtonPressedText;
	}
	else if ((pBarPart->m_nState & xtpBtnStateMouseOver) != 0)
	{
		clrScrollTriangle = m_clrButtonHighlightText;
	}

	PartDrawScrollTriangle(pBarPart, pDC, pBarPart->m_rcRect,
						   (pBarPart->GetPartID() == xtpCalendarCaptionBar_ScrollPrevDate),
						   clrScrollTriangle);
}

void CXTPCalendarCaptionBarOffice2013Theme::PartDraw(CXTPDateLabelPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CXTPFontDC autoFont(pDC, pBarPart->m_fntTextFont, pBarPart->m_clrTextColor);
	pDC->SetBkMode(TRANSPARENT);

	CXTPCalendarView* pCalView = XTP_SAFE_GET2(m_pTheme, GetCalendarCtrl(), GetActiveView(), NULL);

	if (pCalView)
	{
		BOOL bShowMonthDay  = pCalView->GetViewType() != xtpCalendarMonthView;
		COleDateTime dtDay1 = pCalView->GetViewDayDate(0);
		COleDateTime dtDay2 = pCalView->GetViewDayDate(max(0, pCalView->GetViewDayCount() - 1));

		pBarPart->m_strText = pBarPart->_FormatDate(dtDay1, dtDay2, bShowMonthDay);
	}

	if (pBarPart->m_strText.IsEmpty())
		return;

	CRect rcText = pBarPart->m_rcRect;
	rcText.top -= XTP_DPI_Y(6);

	UINT uFlags = DT_VCENTER | DT_SINGLELINE;
	if (m_pTheme && !m_pTheme->IsScrollDateButtons())
	{
		uFlags |= DT_CENTER;
		rcText.right = max(rcText.left, rcText.right - XTP_DPI_X(9));
	}

	pDC->DrawText(pBarPart->GetText(), rcText, uFlags);
}
