// XTPCalendarCaptionBarResourceTheme.cpp : implementation of the
// CXTPCalendarCaptionBarOffice2013Theme class.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"

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
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarResourceTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarResourceTheme

CXTPCalendarCaptionBarResourceTheme::CXTPCalendarCaptionBarResourceTheme(
	CXTPCalendarCaptionBarTheme* pTheme /*=NULL*/)
	: CXTPCalendarCaptionBarOffice2013Theme(pTheme)
{
	m_nPaintTheme = xtpCalendarThemeResource;
}

CXTPCalendarCaptionBarResourceTheme::~CXTPCalendarCaptionBarResourceTheme()
{
}

void CXTPCalendarCaptionBarResourceTheme::RefreshMetrics()
{
	CXTPCalendarCaptionBarPaintManager::RefreshMetrics();

	ASSERT(m_pTheme);
	if (m_pTheme && XTPResourceImages()->IsValid())
	{
		m_pTheme->m_nScrollDateButtonsMargin = XTP_DPI_X(20);

		m_clrSwitchViewBarBk.SetStandardValue(
			GetColor2(_T("CaptionBarSwitchViewBarBk"), RGB(173, 209, 255)));
		m_clrScrollDateBarBk.SetStandardValue(
			GetColor2(_T("CaptionBarDateBarBk"), RGB(227, 239, 255)));
		m_clrBorders[0].SetStandardValue(GetColor2(_T("CaptionBarBorders"), RGB(101, 147, 201)));
		m_clrBorders[1].SetStandardValue(m_clrBorders[0]);
		m_clrBorders[2].SetStandardValue(m_clrBorders[0]);
		m_clrBorders[3].SetStandardValue(m_clrBorders[0]);
		m_clrTextColor.SetStandardValue(GetColor2(_T("CaptionBarTextColor"), RGB(0, 0, 0)));

		if (XTPResourceImages()->GetImageInt(_T("CalendarControl"), _T("FlatStyle"), 0))
		{
			m_pTheme->m_nScrollDateButtonsMargin = XTP_DPI_X(4);
		}

		LOGFONT lfBaseFont;
		::ZeroMemory(&lfBaseFont, sizeof(lfBaseFont));

		// Try to set default Office 2007 font, otherwise icon font used.
		if (XTPDrawHelpers()->FontExists(XTP_CALENDAR_OFFICE2007_FONT_NAME))
		{
			STRCPY_S(lfBaseFont.lfFaceName, LF_FACESIZE, XTP_CALENDAR_OFFICE2007_FONT_NAME);
			lfBaseFont.lfCharSet = XTPResourceManager()->GetFontCharset();
			lfBaseFont.lfHeight  = -11;
			lfBaseFont.lfWeight  = FW_NORMAL;

			if (XTPSystemVersion()->IsClearTypeTextQualitySupported())
				lfBaseFont.lfQuality = 5;
		}
		else
		{
			VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfBaseFont));
		}

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
}

COLORREF CXTPCalendarCaptionBarResourceTheme::GetColor2(LPCTSTR pcszColorName, COLORREF clrDefault)
{
	COLORREF clrColor = COLORREF_NULL;

	if (XTPResourceImages()->IsValid())
	{
		clrColor = XTPResourceImages()->GetImageColor(_T("CalendarControl"), pcszColorName);
	}

	if (clrColor == COLORREF_NULL)
	{
		clrColor = clrDefault;
	}

	return clrColor;
}

CXTPResourceImage* CXTPCalendarCaptionBarResourceTheme::GetResourceImage(LPCTSTR lpszImageFile)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	if (!pImages || !pImages->IsValid())
	{
		return NULL;
	}

	return pImages->LoadFile(lpszImageFile);
}

void CXTPCalendarCaptionBarResourceTheme::PartAdjustLayout(CXTPExpandButtonPart* pBarPart, CDC* pDC,
														   const CRect& rcRect)
{
	ASSERT(pDC);
	ASSERT(pBarPart);

	if (!pDC || !pBarPart)
		return;

	pBarPart->m_rcRect = rcRect;

	if (pBarPart->m_rcRect.IsRectEmpty())
		return;

	CXTPResourceImage* pImage = GetResourceImage(_T("CALENDARCAPTIONBAREXPANDBUTTON"));
	if (pImage)
	{
		CRect rcImage			= pImage->GetSource(0, 6);
		pBarPart->m_rcRect.left = pBarPart->m_rcRect.right - rcImage.Width();
		pBarPart->m_rcRect.top  = pBarPart->m_rcRect.top + pBarPart->m_rcRect.Height() / 2
								 - rcImage.Height() / 2;
		pBarPart->m_rcRect.bottom = pBarPart->m_rcRect.top + rcImage.Height();
	}
	else
	{
		CXTPCalendarCaptionBarOffice2000Theme::PartAdjustLayout(pBarPart, pDC, rcRect);
	}
}

void CXTPCalendarCaptionBarResourceTheme::PartDraw(CXTPSwitchViewButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	ASSERT(pBarPart);

	if (!pDC || !pDC->GetSafeHdc() || !pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	CXTPResourceImage* pImage = GetResourceImage(_T("CALENDARCAPTIONBARSWITCHVIEWBUTTON"));
	if (pImage)
	{
		int nImagePart = (pBarPart->m_nState & xtpBtnStateChecked) ? 3 : 0;

		if (pBarPart->m_nState & xtpBtnStatePressed)
		{
			nImagePart += 2;
		}
		else if (pBarPart->m_nState & xtpBtnStateMouseOver)
		{
			nImagePart += 1;
		}

		pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 6), CRect(0, 0, 0, 0));

		rcRect.left = rcRect.right - XTP_DPI_X(1);
		pDC->FillSolidRect(&rcRect, m_clrBorders[0]);

		// drawText
		CXTPFontDC autoFont(pDC, m_fntTextFont, pBarPart->GetTextColor());

		CRect rcText = pBarPart->m_rcRect;
		rcText.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(pBarPart->GetText(), rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	else
	{
		CXTPCalendarCaptionBarOffice2000Theme::PartDraw(pBarPart, pDC);
	}
}

void CXTPCalendarCaptionBarResourceTheme::PartDraw(CXTPScrollDateButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	ASSERT(pBarPart);

	if (!pDC || !pDC->GetSafeHdc() || !pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	CXTPResourceImage* pImage = GetResourceImage(pBarPart->_GetImageName());
	if (pImage)
	{
		int nImagePart = (pBarPart->m_nState & xtpBtnStatePressed)
							 ? 2
							 : (pBarPart->m_nState & xtpBtnStateMouseOver) ? 1 : 0;

		if (pImage->IsAlphaImage())
			pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 4));
		else
			pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 4), CRect(0, 0, 0, 0),
							  RGB(255, 0, 255));

		if (pBarPart->m_nState & xtpBtnStateFocused)
		{
			if (pImage->IsAlphaImage())
				pImage->DrawImage(pDC, pBarPart->m_rcRect, pImage->GetSource(3, 4),
								  CRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1)));
			else
				pImage->DrawImage(pDC, pBarPart->m_rcRect, pImage->GetSource(3, 4),
								  CRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1)),
								  RGB(255, 0, 255));
		}
	}
	else
	{
		CXTPCalendarCaptionBarOffice2000Theme::PartDraw(pBarPart, pDC);
	}
}

void CXTPCalendarCaptionBarResourceTheme::PartDraw(CXTPDateLabelPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	ASSERT(pBarPart);

	if (!pDC || !pDC->GetSafeHdc() || !pBarPart || !pBarPart->IsVisible())
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

	pDC->DrawText(pBarPart->GetText(), rcText, uFlags);
}

void CXTPCalendarCaptionBarResourceTheme::PartDraw(CXTPExpandButtonPart* pBarPart, CDC* pDC)
{
	ASSERT(pDC);
	ASSERT(pBarPart);

	if (!pDC || !pDC->GetSafeHdc() || !pBarPart || !pBarPart->IsVisible())
		return;

	CRect rcRect = pBarPart->m_rcRect;

	CXTPResourceImage* pImage = GetResourceImage(_T("CALENDARCAPTIONBAREXPANDBUTTON"));
	if (pImage)
	{
		int nImagePart = (pBarPart->m_nState & xtpBtnStateChecked) ? 3 : 0;
		if (pBarPart->m_nState & xtpBtnStatePressed)
			nImagePart += 2;
		else if (pBarPart->m_nState & xtpBtnStateMouseOver)
			nImagePart += 1;

		pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 6), CRect(0, 0, 0, 0),
						  RGB(255, 0, 255));
		// pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 6));
	}
	else
	{
		CXTPCalendarCaptionBarOffice2000Theme::PartDraw(pBarPart, pDC);
	}
}
