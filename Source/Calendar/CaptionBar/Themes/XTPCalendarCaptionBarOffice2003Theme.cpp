// XTPCalendarCaptionBarOffice2003Theme.cpp : implementation of the
// CXTPCalendarCaptionBarOfficeXPTheme class.
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

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarOffice2003Theme

CXTPCalendarCaptionBarOffice2003Theme::CXTPCalendarCaptionBarOffice2003Theme(
	CXTPCalendarCaptionBarTheme* pTheme /*=NULL*/)
	: CXTPCalendarCaptionBarOfficeXPTheme(pTheme)
{
	m_nPaintTheme = xtpCalendarThemeOffice2003;
}

CXTPCalendarCaptionBarOffice2003Theme::~CXTPCalendarCaptionBarOffice2003Theme()
{
}

void CXTPCalendarCaptionBarOffice2003Theme::PartDraw(CXTPSwitchViewButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	if (m_pThemeButton->IsAppThemeActive())
	{
		int nState = (pBarPart->m_nState & xtpBtnStatePressed)
						 ? PBS_PRESSED
						 : (pBarPart->m_nState & xtpBtnStateMouseOver) ? PBS_HOT : PBS_NORMAL;

		if (pBarPart->m_nState & xtpBtnStateChecked)
			nState = PBS_PRESSED;

		m_pThemeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rcRect, NULL);
	}
	else
	{
		UINT uFlags = UINT(DFCS_BUTTONPUSH
						   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
						   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));
		::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);
	}

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

void CXTPCalendarCaptionBarOffice2003Theme::PartDraw(CXTPScrollDateButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pBarPart);
	if (!pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	if (m_pThemeButton->IsAppThemeActive())
	{
		int nState = (pBarPart->m_nState & xtpBtnStatePressed)
						 ? PBS_PRESSED
						 : (pBarPart->m_nState & xtpBtnStateMouseOver) ? PBS_HOT : PBS_NORMAL;

		m_pThemeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rcRect, NULL);
	}
	else
	{
		UINT uFlags = UINT(DFCS_BUTTONPUSH
						   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
						   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

		::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);
	}

	BOOL bLeftDirection = pBarPart->GetPartID() == xtpCalendarCaptionBar_ScrollPrevDate;
	PartDrawScrollTriangle(pBarPart, pDC, rcRect, bLeftDirection, m_clrScrollTriangleColor);
}

void CXTPCalendarCaptionBarOffice2003Theme::PartDraw(CXTPRadioButtonPart* pBarPart, CDC* pDC)
{
	if (!pBarPart->IsVisible())
		return;

	CSize szRadio(XTP_DPI_X(13), XTP_DPI_Y(13));
	CRect rcRadio = pBarPart->m_rcRect;

	BOOL bThemed = m_pTheme && m_pThemeButton->IsAppThemeActive();

	if (bThemed)
		m_pThemeButton->GetThemePartSize(NULL, BP_RADIOBUTTON, 1, &rcRadio, TS_DRAW, &szRadio);

	rcRadio.right = rcRadio.left + szRadio.cx;

	if (bThemed)
	{
		int md		   = rcRadio.CenterPoint().y;
		int ht		   = (int)(rcRadio.Height() / (2 * m_pTheme->m_dScale));
		rcRadio.top	= md - ht;
		rcRadio.bottom = md + ht;

		m_pThemeButton->GetThemePartSize(NULL, BP_RADIOBUTTON, 1, &rcRadio, TS_DRAW, &szRadio);

		int nState = (pBarPart->m_nState & xtpBtnStatePressed)
						 ? RBS_UNCHECKEDPRESSED
						 : (pBarPart->m_nState & xtpBtnStateMouseOver) ? RBS_UNCHECKEDHOT
																	   : RBS_UNCHECKEDNORMAL;

		if (pBarPart->m_nState & xtpBtnStateChecked)
			nState += 4;

		m_pThemeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_RADIOBUTTON, nState, rcRadio,
											NULL);
	}
	else
	{
		UINT uFlags = UINT(DFCS_BUTTONRADIO | DFCS_FLAT
						   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
						   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

		::DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, uFlags);
	}

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

void CXTPCalendarCaptionBarOffice2003Theme::PartDraw(CXTPCheckboxButtonPart* pBarPart, CDC* pDC)
{
	if (!pBarPart->IsVisible())
		return;

	CSize szRadio(XTP_DPI_X(13), XTP_DPI_Y(13));
	CRect rcRadio = pBarPart->m_rcRect;

	BOOL bThemed = m_pTheme && m_pThemeButton->IsAppThemeActive();

	if (bThemed)
		m_pThemeButton->GetThemePartSize(NULL, BP_CHECKBOX, 1, &rcRadio, TS_DRAW, &szRadio);

	rcRadio.right = rcRadio.left + szRadio.cx;

	if (bThemed)
	{
		int md		   = rcRadio.CenterPoint().y;
		int ht		   = (int)(rcRadio.Height() / (2 * m_pTheme->m_dScale));
		rcRadio.top	= md - ht;
		rcRadio.bottom = md + ht;

		m_pThemeButton->GetThemePartSize(NULL, BP_CHECKBOX, 1, &rcRadio, TS_DRAW, &szRadio);

		int nState = (pBarPart->m_nState & xtpBtnStatePressed)
						 ? RBS_UNCHECKEDPRESSED
						 : (pBarPart->m_nState & xtpBtnStateMouseOver) ? RBS_UNCHECKEDHOT
																	   : RBS_UNCHECKEDNORMAL;

		if (pBarPart->m_nState & xtpBtnStateChecked)
			nState += 4;

		m_pThemeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_CHECKBOX, nState, rcRadio, NULL);
	}
	else
	{
		UINT uFlags = UINT(DFCS_BUTTONCHECK | DFCS_FLAT
						   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
						   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

		::DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, uFlags);
	}

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

void CXTPCalendarCaptionBarOffice2003Theme::PartDraw(CXTPExpandButtonPart* pBarPart, CDC* pDC)
{
	if (!pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	if (m_pThemeButton->IsAppThemeActive())
	{
		int nState = (pBarPart->m_nState & xtpBtnStatePressed)
						 ? PBS_PRESSED
						 : (pBarPart->m_nState & xtpBtnStateMouseOver) ? PBS_HOT : PBS_NORMAL;

		m_pThemeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rcRect, NULL);
	}
	else
	{
		UINT uFlags = UINT(DFCS_BUTTONPUSH
						   | ((pBarPart->m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0)
						   | ((pBarPart->m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0));

		::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);
	}

	BOOL bDownDirection = (pBarPart->m_nState & xtpBtnStateChecked) == 0;
	PartDrawExpandTriangle(pBarPart, pDC, rcRect, bDownDirection, RGB(0, 0, 0));
}
