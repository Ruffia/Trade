// XTPCalendarCaptionBarOffice2000Theme.cpp : implementation of the
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"

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

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarOffice2000Theme

CXTPCalendarCaptionBarOffice2000Theme::CXTPCalendarCaptionBarOffice2000Theme(
	CXTPCalendarCaptionBarTheme* pTheme /*=NULL*/)
	: CXTPCalendarCaptionBarPaintManager(pTheme)
{
	m_nPaintTheme = xtpCalendarThemeOffice2000;
}

CXTPCalendarCaptionBarOffice2000Theme::~CXTPCalendarCaptionBarOffice2000Theme()
{
}

void CXTPCalendarCaptionBarOffice2000Theme::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager::RefreshMetrics();

	m_pTheme->m_nScrollDateButtonsMargin = XTP_DPI_X(20);

	m_clrSwitchViewBarBk.SetStandardValue(GetSysColor(COLOR_BTNFACE));
	m_clrScrollDateBarBk.SetStandardValue(RGB(146, 146, 146));
	m_clrBorders[0].SetStandardValue(RGB(130, 130, 130));
	m_clrBorders[1].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[2].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[3].SetStandardValue(m_clrBorders[0]);
	m_clrTextColor.SetStandardValue(RGB(255, 255, 255));
	m_clrScrollTriangleColor.SetStandardValue(RGB(0, 0, 0));

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

void CXTPCalendarCaptionBarOffice2000Theme::PartRefresh(CXTPSwitchViewButtonPart* pBarPart)
{
	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	pBarPart->m_clrTextColor.SetStandardValue(RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarOffice2000Theme::PartRefresh(CXTPRadioButtonPart* pBarPart)
{
	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	pBarPart->m_clrTextColor.SetStandardValue(RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarOffice2000Theme::PartRefresh(CXTPCheckboxButtonPart* pBarPart)
{
	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	pBarPart->m_clrTextColor.SetStandardValue(RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarOffice2000Theme::PartRefresh(CXTPDateLabelPart* pBarPart)
{
	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	if (!m_pTheme)
		return;

	pBarPart->m_clrTextColor.SetStandardValue(m_clrTextColor);

	LOGFONT lfBaseFont;
	::ZeroMemory(&lfBaseFont, sizeof(lfBaseFont));

	m_fntTextFont->GetLogFont(&lfBaseFont);

	if (m_pTheme->IsOneLine())
	{
		if (XTPSystemVersion()->IsWinVistaOrGreater())
			lfBaseFont.lfHeight = -XTP_DPI_Y(14);
		else
			lfBaseFont.lfHeight = -XTP_DPI_Y(18);
	}
	else
		lfBaseFont.lfHeight = -XTP_DPI_Y(20);

	pBarPart->m_fntTextFont.SetStandardValue(&lfBaseFont);
}

void CXTPCalendarCaptionBarOffice2000Theme::PartAdjustLayout(CXTPSwitchViewButtonPart* pBarPart,
															 CDC* pDC, const CRect& rcRect)
{
	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	pBarPart->m_rcRect = rcRect;

	if (pBarPart->m_rcRect.IsRectEmpty())
		return;

	if (pDC && pDC->GetSafeHdc())
	{
		CXTPFontDC autoFont(pDC, m_fntTextFont);

		CSize szText = pDC->GetTextExtent(pBarPart->GetText());

		if (szText.cx + XTP_DPI_X(8) > pBarPart->m_rcRect.Width())
			pBarPart->m_rcRect.right = pBarPart->m_rcRect.left + szText.cx + XTP_DPI_X(8);
	}
}

void CXTPCalendarCaptionBarOffice2000Theme::PartAdjustLayout(CXTPScrollDateButtonPart* pBarPart,
															 CDC* pDC, const CRect& rcRect)
{
	UNREFERENCED_PARAMETER(pDC);

	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	pBarPart->m_rcRect = rcRect;

	if (pBarPart->m_rcRect.IsRectEmpty())
		return;

	CSize szButton(XTP_DPI_X(20), XTP_DPI_Y(20));

	pBarPart->m_rcRect.right = pBarPart->m_rcRect.left + szButton.cx;

	int C					  = (pBarPart->m_rcRect.top + pBarPart->m_rcRect.bottom) / 2;
	int H					  = szButton.cy / 2;
	pBarPart->m_rcRect.top	= C - H;
	pBarPart->m_rcRect.bottom = C + H;

	// pBarPart->m_rcRect.bottom = pBarPart->m_rcRect.top + szButton.cy;
}

void CXTPCalendarCaptionBarOffice2000Theme::PartAdjustLayout(CXTPRadioButtonPart* pBarPart,
															 CDC* pDC, const CRect& rcRect)
{
	ASSERT(pBarPart);
	if (!pBarPart)
		return;

	pBarPart->m_rcRect = rcRect;

	if (pBarPart->m_rcRect.IsRectEmpty())
		return;

	CSize szRadio(XTP_DPI_X(13), XTP_DPI_Y(13));
	CRect rcRadio = pBarPart->m_rcRect;

	if (m_pTheme && m_pThemeButton->IsAppThemeActive())
		m_pThemeButton->GetThemePartSize(NULL, BP_RADIOBUTTON, 1, &rcRadio, TS_DRAW, &szRadio);

	if (pDC && pDC->GetSafeHdc())
	{
		CXTPFontDC autoFont(pDC, m_fntTextFont);
		CSize szText = pDC->GetTextExtent(pBarPart->GetText());

		if (szRadio.cx + XTP_DPI_X(9) + szText.cx + XTP_DPI_X(8) > pBarPart->m_rcRect.Width())
			pBarPart->m_rcRect.right = pBarPart->m_rcRect.left + szRadio.cx + XTP_DPI_X(9)
									   + szText.cx + XTP_DPI_X(8);

		szRadio.cy = max(szRadio.cy, szText.cy);
	}
}

void CXTPCalendarCaptionBarOffice2000Theme::PartAdjustLayout(CXTPCheckboxButtonPart* pBarPart,
															 CDC* pDC, const CRect& rcRect)
{
	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	pBarPart->m_rcRect = rcRect;

	if (pBarPart->m_rcRect.IsRectEmpty())
		return;

	CSize szRadio(XTP_DPI_X(13), XTP_DPI_Y(13));
	CRect rcRadio = pBarPart->m_rcRect;

	if (m_pTheme && m_pThemeButton->IsAppThemeActive())
		m_pThemeButton->GetThemePartSize(NULL, BP_CHECKBOX, 1, &rcRadio, TS_DRAW, &szRadio);

	if (pDC && pDC->GetSafeHdc())
	{
		CXTPFontDC autoFont(pDC, m_fntTextFont);
		CSize szText = pDC->GetTextExtent(pBarPart->GetText());

		if (szRadio.cx + XTP_DPI_X(9) + szText.cx + XTP_DPI_X(8) > pBarPart->m_rcRect.Width())
			pBarPart->m_rcRect.right = pBarPart->m_rcRect.left + szRadio.cx + XTP_DPI_X(9)
									   + szText.cx + XTP_DPI_X(8);

		szRadio.cy = max(szRadio.cy, szText.cy);
	}
}

void CXTPCalendarCaptionBarOffice2000Theme::PartAdjustLayout(CXTPExpandButtonPart* pBarPart,
															 CDC* pDC, const CRect& rcRect)
{
	UNREFERENCED_PARAMETER(pDC);

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	pBarPart->m_rcRect = rcRect;

	if (pBarPart->m_rcRect.IsRectEmpty())
		return;

	CSize szButton(18, 18);

	pBarPart->m_rcRect.left = pBarPart->m_rcRect.right - szButton.cx;
	pBarPart->m_rcRect.top  = pBarPart->m_rcRect.top + pBarPart->m_rcRect.Height() / 2
							 - szButton.cy / 2;
	pBarPart->m_rcRect.bottom = pBarPart->m_rcRect.top + szButton.cy;
}

void CXTPCalendarCaptionBarOffice2000Theme::PartDraw(CXTPSwitchViewButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	UINT uFlags = UINT(DFCS_BUTTONPUSH
					   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
					   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));
	::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);

	// drawText
	CXTPFont fntTmp;
	CXTPFontDC autoFont(pDC, m_fntTextFont, pBarPart->GetTextColor());

	if (pBarPart->m_nState & xtpBtnStateChecked)
	{
		LOGFONT lf;
		if (m_fntTextFont->GetLogFont(&lf))
		{
			lf.lfWeight = FW_BOLD;

			if (fntTmp.CreateFontIndirect(&lf))
				autoFont.SetFont(&fntTmp);
		}
	}

	CRect rcText = pBarPart->m_rcRect;
	rcText.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(pBarPart->GetText(), rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

void CXTPCalendarCaptionBarOffice2000Theme::PartDraw(CXTPScrollDateButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	UINT uFlags = UINT(DFCS_BUTTONPUSH
					   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
					   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

	::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);

	BOOL bLeftDirection = pBarPart->GetPartID() == xtpCalendarCaptionBar_ScrollPrevDate;
	PartDrawScrollTriangle(pBarPart, pDC, rcRect, bLeftDirection, m_clrScrollTriangleColor);
}

void CXTPCalendarCaptionBarOffice2000Theme::PartDrawScrollTriangle(
	CXTPScrollDateButtonPart* pBarPart, CDC* pDC, CRect rcRect, BOOL bLeftDirection,
	COLORREF clrColor)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	rcRect.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	XTPPrimitiveDrawer()->DrawSymbol(pDC,
									 bLeftDirection ? xtpPrimitiveSymbolPlayLeft
													: xtpPrimitiveSymbolPlayRight,
									 rcRect, clrColor);
}

void CXTPCalendarCaptionBarOffice2000Theme::PartDraw(CXTPRadioButtonPart* pBarPart, CDC* pDC)
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

	UINT uFlags = UINT(DFCS_BUTTONRADIO
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

void CXTPCalendarCaptionBarOffice2000Theme::PartDraw(CXTPCheckboxButtonPart* pBarPart, CDC* pDC)
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

	UINT uFlags = UINT(DFCS_BUTTONCHECK
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

void CXTPCalendarCaptionBarOffice2000Theme::PartDraw(CXTPDateLabelPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	COLORREF clrText = m_pTheme->IsOneLine() ? GetSysColor(COLOR_BTNTEXT)
											 : pBarPart->m_clrTextColor.operator DWORD();
	CXTPFontDC autoFont(pDC, pBarPart->m_fntTextFont, clrText);
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

void CXTPCalendarCaptionBarOffice2000Theme::PartDraw(CXTPExpandButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	UINT uFlags = UINT(DFCS_BUTTONPUSH
					   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
					   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

	::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);

	BOOL bDownDirection = (pBarPart->m_nState & xtpBtnStateChecked) == 0;
	PartDrawExpandTriangle(pBarPart, pDC, rcRect, bDownDirection, RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarOffice2000Theme::PartDrawExpandTriangle(CXTPExpandButtonPart* pBarPart,
																   CDC* pDC, CRect rcRect,
																   BOOL bDownDirection,
																   COLORREF clrColor)
{
	ASSERT(pDC);
	if (!pDC || !pDC->GetSafeHdc())
		return;

	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CPoint ptCenter(rcRect.CenterPoint());
	if (bDownDirection)
	{
		ptCenter.y += XTP_DPI_X(2);
		DrawTriangle(pDC, ptCenter, CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(4)),
					 CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(4)), clrColor);
	}
	else
	{
		ptCenter.y -= XTP_DPI_X(2);
		DrawTriangle(pDC, ptCenter, CPoint(ptCenter.x + XTP_DPI_X(4), ptCenter.y + XTP_DPI_Y(4)),
					 CPoint(ptCenter.x - XTP_DPI_X(4), ptCenter.y + XTP_DPI_Y(4)), clrColor);
	}
}
