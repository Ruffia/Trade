// XTPCalendarOffice2013Theme.cpp : implementation of the CXTPCalendarOffice2000Theme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPMarkupRender.h"

#include "Calendar/Resource.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarOptions.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarEventLabel.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarWeekViewEvent.h"
#include "Calendar/XTPCalendarWeekView.h"
#include "Calendar/XTPCalendarWeekViewDay.h"
#include "Calendar/XTPCalendarMonthViewEvent.h"
#include "Calendar/XTPCalendarMonthView.h"
#include "Calendar/XTPCalendarMonthViewDay.h"
#include "Calendar/XTPCalendarDayViewEvent.h"
#include "Calendar/XTPCalendarDayViewDay.h"
#include "Calendar/XTPCalendarDayView.h"
#include "Calendar/XTPCalendarDayViewTimeScale.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/Themes/XTPCalendarOffice2000Theme.h"
#include "Calendar/Themes/XTPCalendarOffice2013Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarOffice2013Theme

CXTPCalendarOffice2013Theme::CXTPCalendarOffice2013Theme()
{
	m_nBusyWidth	  = XTP_DPI_X(7);
	m_nPadding		  = 4;
	m_CurrSystemTheme = xtpSystemThemeUnknown;
	m_nPaintTheme	 = xtpCalendarThemeOffice2013;

	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_xtpFontSel.CreateFontIndirect(&lf);
	m_pSelFont = &m_xtpFontSel;
}

CXTPCalendarOffice2013Theme::~CXTPCalendarOffice2013Theme()
{
}

void CXTPCalendarOffice2013Theme::RefreshMetrics()
{
	CXTPCalendarPaintManager::RefreshMetrics();

	m_clrHeaderBack.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("HeaderBack"), RGB(225, 242, 250)));
	m_clrHeaderText.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("HeaderText"), RGB(68, 68, 68)));
	m_clrHeaderBackSelected.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("HeaderBackSelected"), RGB(153, 200, 233)));
	m_clrHeaderTextSelected.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("HeaderTextSelected"), RGB(255, 255, 255)));
	m_clrHighlight.SetStandardValue(XTPIniColor(_T("Calendar"), _T("Highlight"), RGB(0, 114, 198)));
	m_clrBorders[0].SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("BorderLeft"), RGB(171, 171, 171)));
	m_clrBorders[1].SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("BorderTop"), RGB(171, 171, 171)));
	m_clrBorders[2].SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("BorderRight"), RGB(171, 171, 171)));
	m_clrBorders[3].SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("BorderBottom"), RGB(171, 171, 171)));
	m_clrEventSelBorder.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("EventSelBorder"), RGB(38, 38, 38)));
	m_clrBusyStatusOutOfOfficeBack.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("BusyStatusOutOfOfficeBack"), RGB(129, 61, 95)));
	m_clrMultiDayArrow.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("MultiDayArrow"), RGB(114, 114, 114)));
	m_clrHourBack.SetStandardValue(XTPIniColor(_T("Calendar"), _T("HourBack"), RGB(255, 255, 255)));
	m_clrHourText.SetStandardValue(XTPIniColor(_T("Calendar"), _T("HourText"), RGB(42, 141, 212)));
	m_clrHourNowText.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("HourNowText"), RGB(42, 141, 212)));
	m_clrHourNowBack.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("HourNowBack"), RGB(255, 255, 255)));
	m_clrNowLine.SetStandardValue(XTPIniColor(_T("Calendar"), _T("NowLine"), RGB(42, 141, 212)));
	m_clrNowLineShadow.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("NowLineShadow"), RGB(177, 214, 240)));
	m_clrCellBack.SetStandardValue(XTPIniColor(_T("Calendar"), _T("CellBack"), RGB(255, 255, 255)));
	m_clrCellText.SetStandardValue(XTPIniColor(_T("Calendar"), _T("CellText"), RGB(38, 38, 38)));
	m_clrNonWorkCellBack.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("NonWorkCellBack"), RGB(240, 240, 240)));
	m_clrGridLine.SetStandardValue(XTPIniColor(_T("Calendar"), _T("GridLine"), RGB(225, 225, 225)));
	m_clrMonthGridLine.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("MonthGridLine"), RGB(225, 225, 225)));
	m_clrMultiDayEventFrameColor.SetStandardValue(
		XTPIniColor(_T("Calendar"), _T("MultiDayEventFrameColor"), RGB(205, 230, 247)));
	m_clrTLVEventBar.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("EventBar"), RGB(42, 141, 212)));
	m_clrTLVEventBarLine.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("EventBarLine"), RGB(42, 141, 212)));
	m_clrTLVEventBorderLine.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("EventBorderLine"), RGB(197, 197, 197)));
	m_clrTLVTimeScaleBackground.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("TimeScaleBack"), RGB(225, 242, 250)));
	m_clrTLVTimeScaleBorder.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("TimeScaleBorder"), RGB(255, 255, 255)));
	m_clrTLVTimeScaleHighlight.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("TimeScaleHighlight"), RGB(255, 255, 255)));
	m_clrTLVTimeScaleText.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("TimeScaleText"), RGB(38, 38, 38)));
	m_clrTLVText.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("Text"), RGB(38, 38, 38)));
	m_clrTLVBack.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("Back"), RGB(255, 255, 255)));
	m_clrTLVWeekendBack.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("WeekendBack"), RGB(219, 219, 219)));
	m_clrTLVSelectedText.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("TextSelected"), RGB(255, 255, 255)));
	m_clrTLVSelectedBack.SetStandardValue(
		XTPIniColor(_T("Calendar.TimeLine"), _T("BackSelected"), RGB(153, 200, 233)));
	m_clrEventLabelNone.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("None"), RGB(205, 230, 247)));
	m_clrEventLabelImportant.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Important"), RGB(217, 136, 137)));
	m_clrEventLabelBusiness.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Business"), RGB(241, 157, 90)));
	m_clrEventLabelPersonal.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Personal"), RGB(235, 202, 103)));
	m_clrEventLabelVacation.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Vacation"), RGB(248, 242, 100)));
	m_clrEventLabelMustattend.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Mustattend"), RGB(124, 206, 110)));
	m_clrEventLabelTravelrequired.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Travelrequired"), RGB(117, 202, 177)));
	m_clrEventLabelNeedspreparation.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Needspreparation"), RGB(171, 187, 141)));
	m_clrEventLabelBirthday.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Birthday"), RGB(116, 153, 225)));
	m_clrEventLabelAnniversary.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Anniversary"), RGB(147, 123, 209)));
	m_clrEventLabelPhonecall.SetStandardValue(
		XTPIniColor(_T("Calendar.EventLabel"), _T("Phonecall"), RGB(205, 141, 170)));

	m_strThemeName = XTPIniString(_T("Theme"), _T("Name"), _T("OFFICE2013"));

	RefreshPartMetrics();
}

void CXTPCalendarOffice2013Theme::FormatLabel(CString& strLabel)
{
	strLabel.MakeUpper();
}

void CXTPCalendarOffice2013Theme::UpdateGlyphs()
{
	CXTPCalendarOffice2000Theme::UpdateGlyphs();

	CBitmap bmpGlyphs;
	BOOL bLoadGlyphs = FALSE;

	LPCTSTR lpResourceName = XTPDpiHelper()->SelectDpiSpecific(_T("CALENDAREVENTGLYPHS_100"),
															   _T("CALENDAREVENTGLYPHS_125"),
															   _T("CALENDAREVENTGLYPHS_150"),
															   _T("CALENDAREVENTGLYPHS_200"));
	bLoadGlyphs = XTPResourceImages()->LoadBitmap(&bmpGlyphs, m_strThemeName + lpResourceName);

	if (bLoadGlyphs)
	{
		BITMAP bmpInfo		 = { 0 };
		const int glypsCount = 5;

		COLORREF clrMask = XTPIniColor(_T("Images"), _T("MaskColor"), RGB(255, 0, 255));
		m_ilGlyphs.DeleteImageList();
		VERIFY(bmpGlyphs.GetBitmap(&bmpInfo));
		VERIFY(m_ilGlyphs.Create(bmpInfo.bmWidth / glypsCount, bmpInfo.bmHeight,
								 ILC_COLOR24 | ILC_MASK, 0, 1));
		m_ilGlyphs.Add(&bmpGlyphs, clrMask);
	}
}

void CXTPCalendarOffice2013Theme::DrawHeader(CDC* pDC, CRect& rcHeader, BOOL bIsSelected,
											 BOOL bIsCurrent,
											 const CXTPPaintManagerColorGradient& grclr,
											 COLORREF clrUnderLineHdr)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	if (!bIsSelected)
	{
		pDC->FillSolidRect(rcHeader, m_clrButtonFace);
	}

	if (bIsCurrent)
	{
		if (grclr.clrDark != grclr.clrLight)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rcHeader, grclr, FALSE);
		}
		else
		{
			pDC->FillSolidRect(rcHeader, grclr.clrDark);
		}

		pDC->FillSolidRect(rcHeader.left, rcHeader.bottom - XTP_DPI_Y(1), rcHeader.Width(),
						   XTP_DPI_Y(1), clrUnderLineHdr);
	}
}

void CXTPCalendarOffice2013Theme::DrawExpandSign(CDC* pDC, BOOL bExpandUp, const CRect& rcTSHours)
{
	ASSERT(pDC);

	if (pDC->GetSafeHdc())
	{
		// draw expand signs
		int iIcon;
		CSize szSign(GetExpandSignSize());
		CRect rcSign(rcTSHours);

		if (bExpandUp)
		{
			iIcon		  = idxExpandSignUp;
			rcSign.left   = rcTSHours.right - szSign.cx;
			rcSign.bottom = rcSign.top + szSign.cy;
		}
		else
		{
			iIcon		= idxExpandSignDown;
			rcSign.left = rcTSHours.right - szSign.cx;
			rcSign.top  = rcSign.bottom - szSign.cy;
		}

		pDC->FillSolidRect(rcSign, m_clrEventLabelNone);
		DrawBitmap(&m_ilExpandSigns, pDC, rcSign, iIcon);
	}
}

void CXTPCalendarOffice2013Theme::UpdateEventLabelsDefaultColors()
{
	COLORREF clrLabels[] = {
		m_clrEventLabelNone,			 // XTP_IDS_CALENDAR_EVENT_LABEL_NONE
		m_clrEventLabelImportant,		 // XTP_IDS_CALENDAR_EVENT_LABEL_IMPORTANT
		m_clrEventLabelBusiness,		 // XTP_IDS_CALENDAR_EVENT_LABEL_BUSINESS
		m_clrEventLabelPersonal,		 // XTP_IDS_CALENDAR_EVENT_LABEL_PERSONAL
		m_clrEventLabelVacation,		 // XTP_IDS_CALENDAR_EVENT_LABEL_VACATION
		m_clrEventLabelMustattend,		 // XTP_IDS_CALENDAR_EVENT_LABEL_MUSTATTEND
		m_clrEventLabelTravelrequired,   // XTP_IDS_CALENDAR_EVENT_LABEL_TRAVELREQUIRED
		m_clrEventLabelNeedspreparation, // XTP_IDS_CALENDAR_EVENT_LABEL_NEEDSPREPARATION
		m_clrEventLabelBirthday,		 // XTP_IDS_CALENDAR_EVENT_LABEL_BIRTHDAY
		m_clrEventLabelAnniversary,		 // XTP_IDS_CALENDAR_EVENT_LABEL_ANNIVERSARY
		m_clrEventLabelPhonecall		 // XTP_IDS_CALENDAR_EVENT_LABEL_PHONECALL
	};

	XTP_SAFE_CALL2(m_pControl, GetDataProvider(),
				   UpdateEventLabels(clrLabels, _countof(clrLabels)));
}

void CXTPCalendarOffice2013Theme::UpdateEventCategoriesDefaultColors()
{
	COLORREF clrBkBase[] = {
		RGB(248, 242, 100), // XTP_IDS_CLR_YELLOW
		RGB(124, 206, 110), // XTP_IDS_CLR_GREEN
		RGB(217, 136, 137), // XTP_IDS_CLR_RED
		RGB(147, 123, 209), // XTP_IDS_CLR_PURPLE
		RGB(241, 157, 90),  // XTP_IDS_CLR_ORANGE
		RGB(115, 153, 225)  // XTP_IDS_CLR_BLUE
	};

	COLORREF clrBorder[] = {
		RGB(160, 152, 29), // XTP_IDS_CLR_YELLOW
		RGB(57, 125, 42),  // XTP_IDS_CLR_GREEN
		RGB(167, 29, 35),  // XTP_IDS_CLR_RED
		RGB(75, 48, 141),  // XTP_IDS_CLR_PURPLE
		RGB(177, 79, 12),  // XTP_IDS_CLR_ORANGE
		RGB(40, 81, 142)   // XTP_IDS_CLR_BLUE
	};

	XTP_SAFE_CALL2(m_pControl, GetDataProvider(),
				   UpdateEventCategories(clrBkBase, clrBorder, _countof(clrBkBase)));
}

CXTPCalendarViewEventSubjectEditor* CXTPCalendarOffice2013Theme::StartEditSubject(
	CXTPCalendarViewEvent* pViewEvent, CRect& rcEditor)
{
	if (!pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
	if (!pEvent)
	{
		ASSERT(FALSE);
		return NULL;
	}

	COLORREF clrBack = GetEventBackColor(pEvent);

	CXTPCalendarViewEventSubjectEditor* pEditor =
		new CXTPCalendarViewEventSubjectEditorColored(m_pControl, pViewEvent, clrBack);
	if (pEditor)
	{
		CRect rcEvent(pViewEvent->GetEventRect());
		if (rcEvent.IsRectEmpty())
		{
			return pEditor;
		}

		rcEditor.CopyRect(rcEvent);
		rcEditor.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

		if (pViewEvent->IsKindOf(RUNTIME_CLASS(CXTPCalendarMonthViewEvent))
			|| pViewEvent->IsMultidayEvent() || pEvent->IsAllDayEvent())
		{
			rcEditor.DeflateRect(XTP_DPI_X(7), 0, XTP_DPI_X(2), 0);
		}
		else
		{
			rcEditor.DeflateRect(XTP_DPI_X(10), XTP_DPI_Y(1), 0, 0);
		}
	}

	return pEditor;
}

void CXTPCalendarOffice2013Theme::SetEditFont(CXTPCalendarViewEvent* pViewEvent,
											  CXTPCalendarViewEventSubjectEditor* pEditor)
{
	if (!pViewEvent || !pViewEvent->GetEvent() || !pEditor)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
	if (!pEvent)
	{
		ASSERT(FALSE);
		return;
	}

	CFont* pFont = pViewEvent->GetSubjectEditorFont();
	if (pFont && pFont->GetSafeHandle())
	{
		if (pViewEvent->IsKindOf(RUNTIME_CLASS(CXTPCalendarDayViewEvent))
			|| pViewEvent->IsMultidayEvent() || pEvent->IsAllDayEvent())
		{
			if (m_xtpFontBold.GetSafeHandle())
			{
				m_xtpFontBold.DeleteObject();
			}

			CFont fntBold;
			CreateBoldFont(pFont, &fntBold);

			LOGFONT lfBold;
			fntBold.GetLogFont(&lfBold);
			m_xtpFontBold.CreateFontIndirect(&lfBold);

			pEditor->SetFont(&m_xtpFontBold);

			fntBold.DeleteObject();
		}
		else
		{
			pEditor->SetFont(pFont);
		}
	}
}

void CXTPCalendarOffice2013Theme::GetDateFormat(CString& strShort, CString& strLong)
{
	UNREFERENCED_PARAMETER(strShort);
	strLong = _T("MMM d");
}

void CXTPCalendarOffice2013Theme::CreateBoldFont(CFont* pFont, CFont* pFontBold)
{
	if (!pFontBold || !pFont || !pFont->GetSafeHandle())
	{
		ASSERT(FALSE);
		return;
	}

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	pFontBold->CreateFontIndirect(&lf);
}

void CXTPCalendarOffice2013Theme::CreateHeaderFont(CDC* pDC, CFont* pFont, CFont* pFontNew,
												   BOOL bBoldFont)
{
	if (!pDC || !pDC->GetSafeHdc() || !pFontNew || !pFont || !pFont->GetSafeHandle())
	{
		ASSERT(FALSE);
		return;
	}

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfWeight = bBoldFont ? FW_BOLD : FW_NORMAL;
	int nSize   = ::MulDiv(-lf.lfHeight, 72, ::GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY));
	lf.lfHeight = -::MulDiv(nSize + 1, ::GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 72);
	pFontNew->CreateFontIndirect(&lf);
}

void CXTPCalendarOffice2013Theme::PartRefresh(CControlPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		CXTPCalendarPaintManager::PartRefresh(pViewPart);
		pViewPart->m_clrTextColor = m_clrCellText;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewEventPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		CXTPCalendarPaintManager::PartRefresh(pViewPart);
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewTimeScaleCellPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		CXTPCalendarPaintManager::PartRefresh(pViewPart);
		pViewPart->m_clrBackground.SetStandardValue(m_clrCellBack);
		pViewPart->m_clrBorder = m_clrGridLine;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewTimeScaleHeaderPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrBackground.SetStandardValue(m_clrCellBack);
		pViewPart->m_clrBorder = m_clrGridLine;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewAllDayEventsPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrBackground.SetStandardValue(m_clrCellBack);
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewHeaderPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrTextColor  = m_clrHeaderText;
		pViewPart->m_clrBackground = m_clrHeaderBack;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CWeekViewPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		LOGFONT lfIcon;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
		pViewPart->m_fntText.SetStandardValue(&lfIcon);

		pViewPart->m_clrBackground.SetStandardValue(m_clrCellBack);
		pViewPart->m_clrTextNormalColor = m_clrCellText;
		pViewPart->m_clrTextColor.SetStandardValue(m_clrCellText);
		pViewPart->m_clrTextHighLightColor = m_clrCellText;
		pViewPart->m_clrHeaderBottomLine   = m_clrGridLine;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CMonthViewHeaderPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrTextColor  = m_clrHeaderText;
		pViewPart->m_clrBackground = m_clrHeaderBack;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CMonthViewEventPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		CXTPCalendarPaintManager::PartRefresh(pViewPart);
		pViewPart->m_clrMultiDayEventFrameColor = m_clrMultiDayEventFrameColor;
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewWorkCellPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrBackground.SetStandardValue(m_clrCellBack);
		pViewPart->m_clrShadow.SetStandardValue(m_clrGridLine);
		pViewPart->m_clrHour.SetStandardValue(m_clrGridLine);
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CDayViewNonworkCellPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrBackground.SetStandardValue(m_clrNonWorkCellBack);
		pViewPart->m_clrShadow.SetStandardValue(m_clrGridLine);
		pViewPart->m_clrHour.SetStandardValue(m_clrGridLine);
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CMonthViewGridPart* pViewPart)
{
	ASSERT(pViewPart);

	if (pViewPart)
	{
		pViewPart->m_clrBackground.SetStandardValue(m_clrCellBack);
		pViewPart->m_clrBackground2.SetStandardValue(m_clrHighlight);
	}
}

void CXTPCalendarOffice2013Theme::PartRefresh(CTimeLineViewPart* pViewPart)
{
	ASSERT(pViewPart);

	SAFE_DELETE(pViewPart->m_pimgEvent);

	LPCTSTR lpResourceName = XTPDpiHelper()->SelectDpiSpecific(
		_T("TIMELINEVIEWAPPOINTMENTICON_100"), _T("TIMELINEVIEWAPPOINTMENTICON_125"),
		_T("TIMELINEVIEWAPPOINTMENTICON_150"), _T("TIMELINEVIEWAPPOINTMENTICON_200"));
	pViewPart->m_pimgEvent = XTPResourceImages()->LoadImage(m_strThemeName + lpResourceName);

	pViewPart->m_clrEventBar.SetStandardValue(m_clrTLVEventBar);
	pViewPart->m_clrEventBarLine.SetStandardValue(m_clrTLVEventBarLine);
	pViewPart->m_clrEventBorderLine.SetStandardValue(m_clrTLVEventBorderLine);
	pViewPart->m_clrSelectedText.SetStandardValue(m_clrTLVSelectedText);
	pViewPart->m_clrSelectedBackground.SetStandardValue(m_clrTLVSelectedBack);
	pViewPart->m_clrWeekendBackground.SetStandardValue(m_clrTLVWeekendBack);
	pViewPart->m_clrBackground.SetStandardValue(m_clrTLVBack);
	pViewPart->m_clrText.SetStandardValue(m_clrTLVText);
	pViewPart->m_clrTimeScaleBackground.SetStandardValue(m_clrTLVTimeScaleBackground);
	pViewPart->m_clrTimeScaleHightlite.SetStandardValue(m_clrTLVTimeScaleHighlight);
	pViewPart->m_clrTimeScaleText.SetStandardValue(m_clrTLVTimeScaleText);
}

void CXTPCalendarOffice2013Theme::PartDrawEventSelectedBorder(CDC* pDC, CRect rcBorder, int l,
															  int t, int r, int b)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	COLORREF clrFore = m_clrEventSelBorder;
	COLORREF clrBack = RGB(255, 255, 255) /*m_clrCellBack*/;

	// draw the selected rect.
	CRect rc(rcBorder);
	for (int i = 0; i <= XTP_DPI_X(1); i++)
	{
		pDC->Draw3dRect(rc, clrFore, clrFore);
		rc.InflateRect(1, 1, 1, 1);
	}

	if ((l != 0) || (t != 0) || (r != 0) || (b != 0))
	{
		int x, y;

		// draw left gripper
		if (l != 0)
		{
			x = rcBorder.left - XTP_DPI_X(2);
			y = rcBorder.bottom - (rcBorder.Height() / 2) - XTP_DPI_Y(3);

			CXTPDrawHelpers::DrawColorRect(pDC, CRect(x, y, x + XTP_DPI_X(5), y + XTP_DPI_Y(5)),
										   clrBack, clrFore);
		}

		// draw top gripper
		if (t != 0)
		{
			x = rcBorder.right - (rcBorder.Width() / 2) - XTP_DPI_X(3);
			y = rcBorder.top - XTP_DPI_Y(3);

			CXTPDrawHelpers::DrawColorRect(pDC, CRect(x, y, x + XTP_DPI_X(5), y + XTP_DPI_Y(5)),
										   clrBack, clrFore);
		}

		// draw right gripper
		if (r != 0)
		{
			x = rcBorder.right - XTP_DPI_X(3);
			y = rcBorder.bottom - (rcBorder.Height() / 2) - XTP_DPI_Y(3);

			CXTPDrawHelpers::DrawColorRect(pDC, CRect(x, y, x + XTP_DPI_X(5), y + XTP_DPI_Y(5)),
										   clrBack, clrFore);
		}

		// draw bottom gripper
		if (b != 0)
		{
			x = rcBorder.right - (rcBorder.Width() / 2) - XTP_DPI_X(3);
			y = rcBorder.bottom - XTP_DPI_Y(2);

			CXTPDrawHelpers::DrawColorRect(pDC, CRect(x, y, x + XTP_DPI_X(5), y + XTP_DPI_Y(5)),
										   clrBack, clrFore);
		}
	}
}

COLORREF CXTPCalendarOffice2013Theme::GetEventBackColor(CXTPCalendarEvent* pEvent)
{
	if (pEvent)
	{
		CXTPCalendarEventCategoryIDs* pCategoryIDs = pEvent->GetCategories();
		if (pCategoryIDs && pCategoryIDs->GetSize() > 0)
		{
			CXTPCalendarEventCategories* pCategories = XTP_SAFE_GET2(pEvent, GetDataProvider(),
																	 GetEventCategories(), NULL);
			if (pCategories)
			{
				for (int i = 0; i < pCategoryIDs->GetSize(); ++i)
				{
					CXTPCalendarEventCategory* pCategory = pCategories->Find(
						pCategoryIDs->GetAt(i));
					if (pCategory)
					{
						return pCategory->GetBackgroundColor();
					}
				}
			}
		}
		else
		{
			CXTPCalendarEventLabel* pLabel = pEvent->GetLabel();
			if (pLabel && pLabel->m_nLabelID != XTP_CALENDAR_NONE_LABEL_ID)
			{
				return pLabel->m_clrColor;
			}
		}
	}

	return m_clrEventLabelNone;
}

COLORREF CXTPCalendarOffice2013Theme::GetEventEdgeColor(CXTPCalendarEvent* pEvent)
{
	if (pEvent)
	{
		CXTPCalendarEventCategoryIDs* pCategoryIDs = pEvent->GetCategories();
		if (pCategoryIDs && pCategoryIDs->GetSize() > 0)
		{
			CXTPCalendarEventCategories* pCategories = XTP_SAFE_GET2(pEvent, GetDataProvider(),
																	 GetEventCategories(), NULL);
			if (pCategories)
			{
				for (int i = 0; i < pCategoryIDs->GetSize(); ++i)
				{
					CXTPCalendarEventCategory* pCategory = pCategories->Find(
						pCategoryIDs->GetAt(i));
					if (pCategory)
					{
						return pCategory->GetBorderColor();
					}
				}
			}
		}
		else
		{
			CXTPCalendarEventLabel* pLabel = pEvent->GetLabel();
			if (pLabel && pLabel->m_nLabelID != XTP_CALENDAR_NONE_LABEL_ID)
			{
				return CXTPDrawHelpers::DarkenColorLum(pLabel->m_clrColor, 0.60);
			}
		}
	}

	return CXTPDrawHelpers::DarkenColorLum(m_clrEventLabelNone, 0.60);
}

void CXTPCalendarOffice2013Theme::DrawCategories(CDC* pDC, CRect& rcEvent,
												 CXTPCalendarEvent* pEvent)
{
	if (!pEvent)
		return;

	CXTPCalendarEventCategoryIDs* pCategoryIDs = pEvent->GetCategories();
	if (!pCategoryIDs || pCategoryIDs->GetSize() <= 1)
		return;

	CXTPCalendarEventCategories* pCategories = XTP_SAFE_GET2(pEvent, GetDataProvider(),
															 GetEventCategories(), NULL);
	if (!pCategories)
		return;

	CRect rBlock(rcEvent);
	rBlock.right -= XTP_DPI_X(1);
	rBlock.bottom -= XTP_DPI_Y(2);
	rBlock.left = rBlock.right - XTP_DPI_X(6);
	rBlock.top  = rBlock.bottom - XTP_DPI_Y(12);

	for (int i = 1; i < pCategoryIDs->GetSize(); ++i)
	{
		CXTPCalendarEventCategory* pCategory = pCategories->Find(pCategoryIDs->GetAt(i));
		if (pCategory)
		{
			CXTPDrawHelpers::DrawColorRect(pDC, rBlock, pCategory->GetBackgroundColor(),
										   pCategory->GetBorderColor());

			rBlock.OffsetRect(-XTP_DPI_X(6), 0);
		}
	}

	CXTPCalendarEventCategory* pCategory = pCategories->Find(pCategoryIDs->GetAt(0));
	if (pCategory)
	{
		rBlock.left = rBlock.right - XTP_DPI_X(2);
		pDC->FillSolidRect(&rBlock, pCategory->GetBackgroundColor());
	}
}

void CXTPCalendarOffice2013Theme::PartDrawEventBusy(CXTPCalendarViewEvent* pViewEvent,
													CXTPCalendarViewPart* pViewPart, CDC* pDC,
													CRect& rcEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent && pViewEvent->GetEvent())
	{
		CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();

		COLORREF clrBack = GetEventBackColor(pEvent);
		COLORREF clrEdge = GetEventEdgeColor(pEvent);

		int nBusyStatus = pEvent->GetBusyStatus();

		// fill event background
		pDC->FillSolidRect(&rcEvent, clrBack);

		// calc busy area rect.
		CRect rcBusyArea(rcEvent);
		rcBusyArea.right = rcBusyArea.left + m_nBusyWidth;

		switch (nBusyStatus)
		{
			case xtpCalendarBusyStatusFree:
			{
				CXTPDrawHelpers::DrawColorRect(pDC, rcBusyArea, m_clrCellBack, clrEdge);
				break;
			}
			case xtpCalendarBusyStatusElsewhere:
			{
				WORD bitsBrush[] = { 0x77, 0xff, 0xdd, 0xff, 0x77, 0xff, 0xdd, 0xff };
				CXTPDrawHelpers::DrawBrushRect(pDC, rcBusyArea, m_clrCellBack, clrEdge, bitsBrush);
				break;
			}
			case xtpCalendarBusyStatusTentative:
			{
				WORD bitsBrush[] = { 0xe3, 0xc7, 0x8f, 0x1f, 0x3e, 0x7c, 0xf8, 0xf1 };
				CXTPDrawHelpers::DrawBrushRect(pDC, rcBusyArea, m_clrCellBack, clrEdge, bitsBrush);
				break;
			}
			case xtpCalendarBusyStatusBusy:
			{
				pDC->FillSolidRect(rcBusyArea, clrEdge);
				break;
			}
			case xtpCalendarBusyStatusOutOfOffice:
			{
				pDC->FillSolidRect(rcBusyArea, m_clrBusyStatusOutOfOfficeBack);
				break;
			}
		}
	}
}

CSize CXTPCalendarOffice2013Theme::GetTextSize(CDC* pDC, CString strText, CFont* pFont)
{
	CSize szResult(0, 0);

	ASSERT(pDC);
	ASSERT(pFont);

	if (!pDC || !pDC->GetSafeHdc() || !pFont || !pFont->GetSafeHandle())
	{
		return szResult;
	}

	CXTPFontDC fontDC(pDC, pFont);
	szResult = pDC->GetTextExtent(strText);

	return szResult;
}

void CXTPCalendarOffice2013Theme::DrawMultiDayArrowL(CDC* pDC, int nLeft, CRect& rcText,
													 COleDateTime& dtDate, CFont& font)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	CSize szArrow(XTP_DPI_X(7), XTP_DPI_Y(6));

	CXTPPenDC penDC(pDC->GetSafeHdc(), m_clrMultiDayArrow);
	CBrush brush(m_clrMultiDayArrow);

	CRect rcArrow;
	rcArrow.top	= rcText.top + (rcText.Height() - szArrow.cy) / 2;
	rcArrow.bottom = rcArrow.top + szArrow.cy;
	rcArrow.left   = rcText.left;
	rcArrow.right  = rcArrow.left + szArrow.cx;

	CString strMonthName = CXTPCalendarUtils::GetLocaleString(
		XTPToUIntChecked(LOCALE_SABBREVMONTHNAME1 + (dtDate.GetMonth() - 1)), 100);

	CString strFrom;
	strFrom.LoadString(XTP_IDS_CALENDAR_FROM);
	strFrom += _T(" ");
	strFrom += strMonthName + dtDate.Format(_T(" %d"));

	CSize szFrom(GetTextSize(pDC, strFrom, &font));

	CRect rcFrom(rcText);
	rcFrom.left  = rcArrow.right + XTP_DPI_X(8);
	rcFrom.right = rcFrom.left + szFrom.cx;

	if (nLeft > rcFrom.right + XTP_DPI_X(5))
	{
		CPoint pt1(rcArrow.TopLeft());
		CPoint pt2(rcArrow.BottomRight());

		int h = szArrow.cy / 3;

		CRect r(pt1.x, pt1.y + h, pt2.x, pt1.y + h * 2);
		pDC->FillRect(r, &brush);

		int x1 = XTP_DPI_X(2), x2 = XTP_DPI_X(5);
		for (int i = pt1.y; i < pt1.y + h; i++)
		{
			pDC->MoveTo(pt1.x + x1, i);
			pDC->LineTo(pt1.x + x2, i);
			x1--;
			x2--;
		}

		x1 = XTP_DPI_X(2), x2 = XTP_DPI_X(5);
		for (int j = pt1.y + h * 3 - 1; j > pt1.y + h * 2 - 1; j--)
		{
			pDC->MoveTo(pt1.x + x1, j);
			pDC->LineTo(pt1.x + x2, j);
			x1--;
			x2--;
		}

		CXTPFontDC fontDC(pDC, &font);
		pDC->DrawText(strFrom, &rcFrom, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
}

void CXTPCalendarOffice2013Theme::DrawMultiDayArrowR(CDC* pDC, int nRight, CRect& rcText,
													 COleDateTime& dtDate, CFont& font)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	CSize szArrow(XTP_DPI_X(7), XTP_DPI_Y(6));

	CXTPPenDC penDC(pDC->GetSafeHdc(), m_clrMultiDayArrow);
	CBrush brush(m_clrMultiDayArrow);

	CRect rcArrow;
	rcArrow.top	= rcText.top + (rcText.Height() - szArrow.cy) / 2;
	rcArrow.bottom = rcArrow.top + szArrow.cy;
	rcArrow.right  = rcText.right;
	rcArrow.left   = rcArrow.right - szArrow.cx;

	CString strMonthName = CXTPCalendarUtils::GetLocaleString(
		XTPToUIntChecked(LOCALE_SABBREVMONTHNAME1 + (dtDate.GetMonth() - 1)), 100);

	CString strTo;
	strTo.LoadString(XTP_IDS_CALENDAR_TO);
	strTo += _T(" ");
	strTo += strMonthName + dtDate.Format(_T(" %d"));

	CSize szTo(GetTextSize(pDC, strTo, &font));

	CRect rcTo(rcText);
	rcTo.left  = rcArrow.left - XTP_DPI_X(8) - szTo.cx;
	rcTo.right = rcTo.left + szTo.cx;

	if (nRight < rcTo.left - XTP_DPI_X(5))
	{
		CPoint pt1(rcArrow.TopLeft());
		CPoint pt2(rcArrow.BottomRight());

		int h = szArrow.cy / 3;

		CRect r(pt1.x, pt1.y + h, pt2.x, pt1.y + h * 2);
		pDC->FillRect(r, &brush);

		int x1 = XTP_DPI_X(2), x2 = XTP_DPI_X(5);
		for (int i = pt1.y; i < pt1.y + h; i++)
		{
			pDC->MoveTo(pt1.x + x1, i);
			pDC->LineTo(pt1.x + x2, i);
			x1++;
			x2++;
		}

		x1 = XTP_DPI_X(2), x2 = XTP_DPI_X(5);
		for (int j = pt1.y + h * 3 - 1; j > pt1.y + h * 2 - 1; j--)
		{
			pDC->MoveTo(pt1.x + x1, j);
			pDC->LineTo(pt1.x + x2, j);
			x1++;
			x2++;
		}

		CXTPFontDC fontDC(pDC, &font);
		pDC->DrawText(strTo, &rcTo, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
}

void CXTPCalendarOffice2013Theme::DrawMultiDayEvent(CDayViewEventPart* pViewPart, CDC* pDC,
													CXTPCalendarDayViewEvent* pViewEvent,
													CRect& rcText)
{
	BOOL bOutOfBorders = FALSE;

	if (!pDC || !pViewPart || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bNoVisibleMDEvent = pViewEvent->IsMultidayEvent() && !pViewEvent->IsVisible();
	int nMDEFlags		   = pViewEvent->GetMultiDayEventFlags();
	if ((nMDEFlags & xtpCalendarMultiDaySlave) || bNoVisibleMDEvent)
	{
		return; // was drawn in the Master (or out of events rect)
	}

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	// draw select border
	if (pViewEvent->IsSelected())
	{
		int r = ((nMDEFlags & xtpCalendarMultiDayLast) == 0) ? 0 : 1;
		int l = ((nMDEFlags & xtpCalendarMultiDayFirst) == 0) ? 0 : 1;
		PartDrawEventSelectedBorder(pDC, rcEvent, XTP_DPI_X(l), 0, XTP_DPI_X(r), 0);
	}

	CString strSub = pViewEvent->GetItemTextSubject();
	CString strLoc = pViewEvent->GetItemTextLocation();

	if (strSub.GetLength() > 0 && strLoc.GetLength() > 0)
		strSub += _T(";");

	CFont& font = pViewPart->GetTextFont();

	CFont fontBold;
	CreateBoldFont(&font, &fontBold);

	// get initial subject and location text size.
	CSize szSub(GetTextSize(pDC, strSub, &fontBold));
	CSize szLoc(GetTextSize(pDC, strLoc, &font));

	// calculate starting points and sizes.
	int cx = szSub.cx + XTP_DPI_X(5) + szLoc.cx;
	int cy = max(szSub.cy, szLoc.cy);
	int x  = rcText.left + (rcText.Width() - cx) / 2;
	int y  = rcText.top + (rcText.Height() - cy) / 2;

	CRect rcSub(rcText);
	CRect rcLoc(rcText);
	CRect rcTime(rcText);

	// calculate subject text rect.
	rcSub.left   = (cx < rcText.Width()) ? x : rcText.left;
	rcSub.right  = rcSub.left + szSub.cx;
	rcSub.top	= y;
	rcSub.bottom = rcSub.top + szSub.cy;

	if (rcSub.right > rcText.right)
	{
		rcSub.right   = rcText.right;
		bOutOfBorders = TRUE;
	}

	// calculate location text rect.
	rcLoc.left   = rcSub.right + XTP_DPI_X(5);
	rcLoc.right  = rcLoc.left + szLoc.cx;
	rcLoc.top	= y;
	rcLoc.bottom = rcLoc.top + szLoc.cy;

	if (rcLoc.right > rcText.right)
	{
		rcLoc.right   = rcText.right;
		bOutOfBorders = TRUE;
	}

	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
	COLORREF clrBack		  = GetEventBackColor(pEvent);

	COleDateTime dtStart = pEvent->GetStartTime();
	COleDateTime dtEnd   = pEvent->GetEndTime();

	if ((nMDEFlags & xtpCalendarMultiDayFirst) == 0)
	{
		DrawMultiDayArrowL(pDC, rcSub.left, rcText, dtStart, font);
	}
	if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
	{
		DrawMultiDayArrowR(pDC, rcLoc.right, rcText, dtEnd, font);
	}

	DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(), GetCalendarOptions(),
										  dwAdditionalOptions, 0);
	BOOL bShowStartTime   = (dwAdditionalOpt & xtpCalendarOptDayViewShowStartTimeAlways) ? TRUE
																					   : FALSE;
	BOOL bShowEndTime = (dwAdditionalOpt & xtpCalendarOptDayViewShowEndTimeAlways) ? TRUE : FALSE;

	// draw time text.
	if (!pViewEvent->IsHideEventTime() && (bShowStartTime || bShowEndTime))
	{
		CString strStartTime = pViewEvent->GetItemText(xtpCalendarItemText_EventStartTimeText);
		CSize szStartTime(GetTextSize(pDC, strStartTime, &font));

		CString strEndTime = pViewEvent->GetItemText(xtpCalendarItemText_EventEndTimeText);
		CSize szEndTime(GetTextSize(pDC, strEndTime, &font));

		CRect rcTest(rcEvent);
		rcTest.left  = rcText.left + (rcText.Width() - (szSub.cx + szLoc.cx + XTP_DPI_X(5))) / 2;
		rcTest.right = rcTest.left + (szSub.cx + szLoc.cx + XTP_DPI_X(5));

		BOOL bTimeAsClocks		  = FALSE;
		CXTPCalendarOptions* pOpt = m_pControl->GetCalendarOptions();
		if (pOpt)
		{
			bShowEndTime  = pOpt->bWeekView_ShowEndDate;
			bTimeAsClocks = pOpt->bWeekView_ShowTimeAsClocks;
		}

		if (nMDEFlags & xtpCalendarMultiDayFirst)
		{
			if (bShowStartTime && bTimeAsClocks)
			{
				CRect rcClockRect = pViewEvent->GetFirstClockRect();
				rcClockRect.OffsetRect(m_nBusyWidth, 0); // offset for busy area.
				if (rcClockRect.Width() > 0)
				{
					DrawClock(pDC, dtStart, rcClockRect, clrBack, xtpCalendarClockAlignLeft);
				}
				rcEvent.InflateRect(XTP_DPI_X(1), 0, 0, 0);

				rcText.left = rcClockRect.right + XTP_DPI_X(m_nPadding);
			}
			else if (bShowStartTime && !strStartTime.IsEmpty())
			{
				CRect rcTimeText;
				rcTimeText.left   = rcText.left;
				rcTimeText.right  = rcTimeText.left + szStartTime.cx;
				rcTimeText.top	= rcEvent.top + (rcEvent.Height() - szStartTime.cy) / 2;
				rcTimeText.bottom = rcTimeText.top + szStartTime.cy;

				if (rcTimeText.right < rcTest.left)
				{
					CXTPFontDC fontDC(pDC, &font);
					pDC->DrawText(strStartTime, &rcTimeText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				}
				else
				{
					bOutOfBorders = TRUE;
				}
			}
		}

		if (nMDEFlags & xtpCalendarMultiDayLast && !CXTPCalendarUtils::IsEqual(dtStart, dtEnd))
		{
			if (bShowEndTime && bTimeAsClocks)
			{
				CRect rcClockRect = pViewEvent->GetLastClockRect();
				rcClockRect.OffsetRect(-XTP_DPI_X(m_nPadding), 0);

				if (rcClockRect.Width() > 0)
				{
					DrawClock(pDC, dtEnd, rcClockRect, clrBack, xtpCalendarClockAlignLeft);
				}
				rcEvent.DeflateRect(0, XTP_DPI_Y(2), 0, 0);
			}
			else if (bShowEndTime && !strStartTime.IsEmpty())
			{
				CRect rcTimeText;
				rcTimeText.left   = rcText.right - szEndTime.cx;
				rcTimeText.right  = rcTimeText.left + szEndTime.cx;
				rcTimeText.top	= rcEvent.top + (rcEvent.Height() - szEndTime.cy) / 2;
				rcTimeText.bottom = rcTimeText.top + szEndTime.cy;

				if (rcTimeText.left > rcTest.right)
				{
					CXTPFontDC fontDC(pDC, &font);
					pDC->DrawText(strStartTime, &rcTimeText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
				}
				else
				{
					bOutOfBorders = TRUE;
				}
			}
		}
	}

	// draw subject text.
	CXTPFontDC fontDC(pDC, &fontBold);
	if (rcSub.right < rcText.right)
	{
		pDC->DrawText(strSub, &rcSub, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
	else
	{
		if (rcSub.left < rcText.right)
		{
			pDC->DrawText(strSub, &rcSub, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
		}
		bOutOfBorders = TRUE;
	}

	// draw location text.
	fontDC.SetFont(&font);
	if (rcLoc.right < rcText.right)
	{
		pDC->DrawText(strLoc, &rcLoc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
	else
	{
		if (rcLoc.left < rcText.right)
		{
			pDC->DrawText(strLoc, &rcLoc, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
		}
		bOutOfBorders = TRUE;
	}

	pViewEvent->SetTextOutOfBorders(bOutOfBorders);

	fontBold.DeleteObject();
}

void CXTPCalendarOffice2013Theme::DrawSingleDayEvent(CDayViewEventPart* pViewPart, CDC* pDC,
													 CXTPCalendarDayViewEvent* pViewEvent,
													 CRect& rcText)
{
	BOOL bOutOfBorders = FALSE;

	if (!pDC || !pViewPart || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
		return;

	// draw select border
	if (pViewEvent->IsSelected())
		PartDrawEventSelectedBorder(pDC, rcEvent, 0, XTP_DPI_Y(1), 0, XTP_DPI_Y(1));

	// Draw markup if enabled
	BOOL bMarkupDrawn = FALSE;
	if (m_pControl->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = m_pControl->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();
		if (NULL != pUIElement && NULL != pMarkupContext)
		{
			CRect rcMarkup(rcEvent);
			rcMarkup.DeflateRect(XTP_DPI_X(7), XTP_DPI_Y(7), XTP_DPI_X(7), XTP_DPI_Y(7));
			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcMarkup);
			bMarkupDrawn = TRUE;
		}
	}

	if (!bMarkupDrawn)
	{
		CString strSub  = pViewEvent->GetItemTextSubject();
		CString strLoc  = pViewEvent->GetItemTextLocation();
		CString strBody = pViewEvent->GetItemTextBody();

		CFont& font = pViewPart->GetTextFont();

		CFont fontBold;
		CreateBoldFont(&font, &fontBold);

		CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
		COleDateTime dtStart	  = pEvent->GetStartTime();
		COleDateTime dtEnd		  = pEvent->GetEndTime();

		CRect rcSub(rcText);
		CRect rcLoc(rcText);
		CRect rcTime(rcText);
		CRect rcBody(rcText);

		BOOL bDrawSubject = TRUE;
		BOOL bDrawLoc	 = TRUE;
		BOOL bDrawBody	= TRUE;

		POINT padding = { XTP_DPI_X(m_nPadding), XTP_DPI_Y(m_nPadding) };

		int nTextHeight = (pDC->GetTextExtent(_T(" "))).cy;

		rcSub.top = rcLoc.top = rcTime.top = rcBody.top = rcEvent.top + XTP_DPI_Y(2);

		DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(),
											  GetCalendarOptions(), dwAdditionalOptions, 0);
		BOOL bShowStartTime   = (dwAdditionalOpt & xtpCalendarOptDayViewShowStartTimeAlways) ? TRUE
																						   : FALSE;
		BOOL bShowEndTime = (dwAdditionalOpt & xtpCalendarOptDayViewShowEndTimeAlways) ? TRUE
																					   : FALSE;
		BOOL bNoWordBreak = (dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak) ? TRUE : FALSE;
		DWORD dwFormat	= XTPToDWORD(
			   DT_LEFT | DT_EDITCONTROL | DT_END_ELLIPSIS
			   | (bNoWordBreak ? (DT_WORD_ELLIPSIS | DT_SINGLELINE) : DT_WORDBREAK));

		CXTPCalendarOptions* pOpt = m_pControl->GetCalendarOptions();
		if (pOpt)
			bShowEndTime = pOpt->bWeekView_ShowEndDate;

		CXTPFontDC fontDC(pDC, &font);

		// draw time text.
		if (!pViewEvent->IsHideEventTime() && (bShowStartTime || bShowEndTime))
		{
			BOOL bTimeAsClocks			 = FALSE;
			CXTPCalendarOptions* pOption = m_pControl->GetCalendarOptions();
			if (pOption)
			{
				bShowEndTime  = pOption->bWeekView_ShowEndDate;
				bTimeAsClocks = pOption->bWeekView_ShowTimeAsClocks;
			}

			if (bTimeAsClocks)
			{
				COLORREF clrBack = GetEventBackColor(pEvent);

				if (bShowStartTime)
				{
					CRect rcClock(rcTime);
					rcClock.right  = rcClock.left + XTP_DPI_X(15);
					rcClock.bottom = rcClock.top + XTP_DPI_Y(15);

					if (rcClock.Width() > 0)
						DrawClock(pDC, dtStart, rcClock, clrBack, xtpCalendarClockAlignLeft);

					rcSub.top = rcClock.bottom + padding.y;
				}

				if (!CXTPCalendarUtils::IsEqual(dtStart, dtEnd) && bShowEndTime)
				{
					CRect rcClock(rcTime);
					rcClock.right  = rcClock.left + XTP_DPI_X(15);
					rcClock.bottom = rcClock.top + XTP_DPI_Y(15);

					if (bShowStartTime)
						rcClock.OffsetRect(XTP_DPI_X(15), 0);

					if (rcClock.Width() > 0)
						DrawClock(pDC, dtEnd, rcClock, clrBack, xtpCalendarClockAlignLeft);

					if (!bShowStartTime)
						rcSub.top = rcClock.bottom + padding.y;
				}
			}
			else
			{
				DWORD dwTimeFlags = 0;
				if (bShowStartTime)
					dwTimeFlags = xtpCalendarItemText_EventStartTimeText;

				if (!CXTPCalendarUtils::IsEqual(dtStart, dtEnd) && bShowEndTime)
					dwTimeFlags |= xtpCalendarItemText_EventEndTimeText;

				CString strTime = pViewEvent->GetItemText(dwTimeFlags);

				if (!strTime.IsEmpty())
				{
					if (pDC->DrawText(strTime, &rcTime, DT_CALCRECT | dwFormat))
					{
						if ((rcEvent.right - padding.x) <= rcTime.right)
						{
							rcTime.right  = rcEvent.right - padding.x;
							bOutOfBorders = TRUE;
						}

						if ((rcEvent.bottom - padding.y) <= rcTime.bottom)
						{
							rcTime.bottom = rcEvent.bottom - padding.y;
							bOutOfBorders = TRUE;
							bDrawSubject  = FALSE;
							bDrawLoc	  = FALSE;
							bDrawBody	 = FALSE;
						}

						pViewPart->DrawText(pDC, strTime, rcTime, dwFormat | DT_VCENTER);

						if (bDrawSubject)
						{
							rcSub.top = rcTime.bottom;
						}
						else
						{
							// Make subject on the same line with time
							rcSub.top	= rcTime.top;
							rcSub.bottom = rcTime.bottom;
							rcSub.left   = rcTime.right;
							rcSub.right  = rcText.right;
							bDrawSubject = TRUE;

							strSub = CString(_T(" ")) + strSub;
						}
					}
					else
					{
						rcTime.bottom = rcTime.top;
						rcTime.right  = rcTime.left;
					}
				}
			}
		}

		if (bDrawSubject)
		{
			fontDC.SetFont(&fontBold);

			if (pDC->DrawText(strSub, &rcSub, DT_CALCRECT | dwFormat))
			{
				if ((rcEvent.right - padding.x) <= rcSub.right)
				{
					rcSub.right   = rcEvent.right - padding.x;
					bOutOfBorders = TRUE;
				}

				if ((rcEvent.bottom - padding.y) <= rcSub.bottom
					|| (rcEvent.bottom - padding.y - rcSub.bottom) < nTextHeight)
				{
					rcSub.bottom  = rcEvent.bottom - padding.y;
					bOutOfBorders = TRUE;
					bDrawLoc	  = FALSE;
					bDrawBody	 = FALSE;
				}

				// draw text.
				pDC->DrawText(strSub, &rcSub, dwFormat | DT_VCENTER);
			}
			else
			{
				rcSub.bottom = rcSub.top;
				rcSub.right  = rcSub.left;
			}
		}

		// draw location text.
		if (bDrawLoc)
		{
			// adjust location text rect.
			rcLoc.top = rcSub.bottom;

			fontDC.SetFont(&font);

			// calc location text size.
			if (pDC->DrawText(strLoc, &rcLoc, DT_CALCRECT | dwFormat))
			{
				if ((rcEvent.right - padding.x) <= rcLoc.right)
				{
					rcLoc.right   = rcEvent.right - padding.x;
					bOutOfBorders = TRUE;
				}

				if ((rcEvent.bottom - padding.y) <= rcLoc.bottom
					|| (rcEvent.bottom - padding.y - rcLoc.bottom) < nTextHeight)
				{
					rcLoc.bottom  = rcEvent.bottom - padding.y;
					bOutOfBorders = TRUE;
					bDrawBody	 = FALSE;
				}

				// draw location text.
				pDC->DrawText(strLoc, &rcLoc, dwFormat | DT_VCENTER);
			}
			else
			{
				rcLoc.bottom = rcLoc.top;
				rcLoc.right  = rcLoc.left;
			}
		}

		if (bDrawBody)
		{
			// adjust location text rect.
			rcBody.top = rcLoc.bottom;

			// calc body text size.
			if (pDC->DrawText(strBody, &rcBody, DT_CALCRECT | dwFormat))
			{
				if ((rcEvent.right - padding.x) <= rcBody.right)
				{
					rcBody.right  = rcEvent.right - padding.x;
					bOutOfBorders = TRUE;
				}

				if ((rcEvent.bottom - padding.y) <= rcBody.bottom
					|| (rcEvent.bottom - padding.y - rcBody.bottom) < nTextHeight)
				{
					rcBody.bottom = rcEvent.bottom - padding.y;
					bOutOfBorders = TRUE;
				}

				// draw body text.
				pDC->DrawText(strBody, &rcBody, dwFormat | DT_VCENTER);
			}
			else
			{
				rcBody.bottom = rcBody.top;
				rcBody.right  = rcBody.left;
			}
		}

		fontDC.DisableHandleLeakDetection();
		fontBold.DeleteObject();
	}

	pViewEvent->SetTextOutOfBorders(bOutOfBorders);
}

void CXTPCalendarOffice2013Theme::PartDraw(CDayViewEventPart* pViewPart, CDC* pDC,
										   CXTPCalendarDayViewEvent* pViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart || pViewEvent && pViewEvent->GetEvent())
	{
		CRect rcEvent(pViewEvent->GetEventRect());
		if (rcEvent.IsRectEmpty())
		{
			return;
		}

		int nMDEFlags	= pViewEvent->GetMultiDayEventFlags();
		BOOL bMultiDay   = (nMDEFlags & xtpCalendarMultiDayFMLmask) ? TRUE : FALSE;
		COLORREF clrText = pViewPart->GetTextColor();
		COLORREF clrOld  = pDC->SetTextColor(clrText);

		// fill background and draw busy status rect.
		PartDrawEventBusy(pViewEvent, pViewPart, pDC, rcEvent);

		// define working area for text display.
		CRect rcText = pViewEvent->GetTextRect();
		rcText.right = rcEvent.right - XTP_DPI_X(m_nPadding);
		rcText.left  = rcEvent.left + m_nBusyWidth + XTP_DPI_X(m_nPadding);

		CRect rcIcons(rcText);
		rcIcons.top += XTP_DPI_Y(2);

		int nIcons = pViewEvent->CalcIconsRect(rcIcons);
		if (nIcons)
		{
			rcText.left += nIcons + XTP_DPI_X(m_nPadding);
		}

		DrawIcons(pDC, pViewEvent);

		if (bMultiDay)
		{
			DrawMultiDayEvent(pViewPart, pDC, pViewEvent, rcText);
		}
		else
		{
			DrawSingleDayEvent(pViewPart, pDC, pViewEvent, rcText);
		}

		pDC->SetTextColor(clrOld);
		pViewPart->m_clrTextColor.SetDefaultValue();

		DrawCategories(pDC, rcEvent, pViewEvent->GetEvent());
	}
}

void CXTPCalendarOffice2013Theme::PartDrawHourCell(CDayViewTimeScaleCellPart* pViewPart, CDC* pDC,
												   CRect rc, CString strText, BOOL bFillBackground,
												   BOOL bCurrentTime)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		if (bFillBackground)
			pDC->FillSolidRect(rc, m_clrHourBack);

		if (bCurrentTime)
		{
			CRect rcBack = rc;
			rcBack.DeflateRect(XTP_DPI_X(5), 0, 0, 0);
			pDC->FillSolidRect(rcBack, m_clrHourNowBack);
		}

		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.Height(),
						   pViewPart->m_clrBorder);

		LOGFONT lf;
		pViewPart->m_fntTimeText->GetLogFont(&lf);

		COLORREF clrTextColor = m_clrHourText;

		if (bCurrentTime)
		{
			lf.lfWeight  = FW_BOLD;
			clrTextColor = m_clrHourNowText;
		}

		pViewPart->SetTextColor(clrTextColor);

		CXTPFont fntTimeText;
		fntTimeText.CreateFontIndirect(&lf);

		// Draw hours text
		CXTPFontDC fontBig(pDC, &fntTimeText, clrTextColor);
		pDC->DrawText(strText, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		rc.DeflateRect(XTP_DPI_X(5), 0, 0, 0);
		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrBorder);
	}
}

void CXTPCalendarOffice2013Theme::PartDrawBigHourCell(CDayViewTimeScaleCellPart* pViewPart,
													  CDC* pDC, CRect rc, CString strHour,
													  CString strMin, int nRowPerHour,
													  BOOL bFillBackground, BOOL bCurrentTime)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		// Fill background
		if (bFillBackground)
			pDC->FillSolidRect(rc, m_clrHourBack);

		if (bCurrentTime)
		{
			CRect rcBack = rc;
			rcBack.DeflateRect(XTP_DPI_X(5), 0, 0, 0);
			pDC->FillSolidRect(rcBack, m_clrHourNowBack);
		}

		// Calculate coordinates
		CRect rcMinutesText(rc);
		rcMinutesText.bottom = rcMinutesText.top + rc.Height() / nRowPerHour;
		rcMinutesText.left += pViewPart->m_nHourWidth + XTP_DPI_X(10);

		CRect rcHoursText(rc);
		rcHoursText.right = rcMinutesText.left;
		rcHoursText.DeflateRect(0, XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(7));

		COLORREF clrTextColor = m_clrHourText;

		UINT dtHourTextFormat = DT_TOP | DT_RIGHT | DT_SINGLELINE | DT_NOCLIP;

		LOGFONT lf;
		pViewPart->m_fntTimeText->GetLogFont(&lf);

		if (bCurrentTime)
		{
			lf.lfWeight  = FW_BOLD;
			clrTextColor = m_clrHourNowText;
		}

		pViewPart->SetTextColor(clrTextColor);

		CXTPFont fntTimeText;
		fntTimeText.CreateFontIndirect(&lf);

		dtHourTextFormat = DT_TOP | DT_CENTER | DT_SINGLELINE | DT_NOCLIP;
		rcHoursText		 = rc;
		rcHoursText.DeflateRect(0, XTP_DPI_Y(2), XTP_DPI_X(10), 0);

		rcMinutesText.left += XTP_DPI_X(2);
		rcMinutesText.top -= XTP_DPI_Y(6);

		// Draw hours text
		CXTPFontDC fontBig(pDC, &fntTimeText, clrTextColor);

		pDC->DrawText(strHour, rcHoursText, dtHourTextFormat);

		if (strMin.CompareNoCase(_T("am")) == 0 || strMin.CompareNoCase(_T("pm")) == 0)
		{
			// draw minutes text
			pViewPart->DrawText(pDC, strMin, rcMinutesText,
								DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
		}

		// draw small lines
		if (nRowPerHour > 2)
		{
			BOOL bShowMinutes = XTP_SAFE_GET3(this, m_pControl, GetCalendarOptions(),
											  bDayView_TimeScaleShowMinutes, FALSE);
			CString strMinEx;

			for (int i = 1; i < nRowPerHour; i++)
			{
				int y = rc.top + i * rc.Height() / nRowPerHour;
				pDC->FillSolidRect(rcMinutesText.left, y - XTP_DPI_Y(1), rcMinutesText.Width(),
								   XTP_DPI_Y(1), pViewPart->m_clrBorder);

				if (bShowMinutes)
				{
					int nMinEx = 60 / nRowPerHour * i;
					strMinEx.Format(_T("%02d"), nMinEx);

					CRect rcMinEx  = rcMinutesText;
					rcMinEx.top	= y;
					rcMinEx.bottom = y + rc.Height();

					pViewPart->DrawText(pDC, strMinEx, rcMinEx,
										DT_TOP | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
				}
			}
		}

		// draw line below
		CRect rcLine = rc;
		CRect rcClient;
		pViewPart->GetCalendarControl()->GetClientRect(rcClient);

		rcLine.DeflateRect(rcLine.right == rcClient.right ? 0 : XTP_DPI_X(5), 0, 0, 0);

		pDC->FillSolidRect(rcLine.left, rcLine.bottom - XTP_DPI_Y(1), rcLine.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrBorder);

		if (rcLine.right == rcClient.right)
		{
			// draw left vertical line
			rcLine		 = rc;
			rcLine.right = rcLine.left + XTP_DPI_X(1);
			pDC->FillSolidRect(rcLine, pViewPart->m_clrBorder);
		}
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CDayViewTimeScaleHeaderPart* pViewPart, CDC* pDC,
										   CXTPCalendarDayView* /*pView*/, CRect rc,
										   CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		pDC->FillSolidRect(rc, m_clrHourBack);

		rc.DeflateRect(XTP_DPI_X(1), 0, 0, 0);
		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrBorder);

		rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(2));
		pViewPart->DrawText(pDC, strText, rc, DT_BOTTOM | DT_LEFT | DT_SINGLELINE);
	}
}

void CXTPCalendarOffice2013Theme::PartDrawNowLine(CDayViewTimeScaleHeaderPart* pViewPart, CDC* pDC,
												  CXTPCalendarDayView* pView, CRect rc, int y,
												  BOOL bDrawBk)
{
	UNREFERENCED_PARAMETER(bDrawBk);

	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);

	if (pDC->GetSafeHdc() && pViewPart && pView)
	{
		CRect rcClient;
		pViewPart->GetCalendarControl()->GetClientRect(rcClient);

		CRect rcNowLine  = rc;
		rcNowLine.top	= y;
		rcNowLine.bottom = y + XTP_DPI_Y(3);

		if (rcClient.left == rc.left)
			rcNowLine.right = rcClient.right;
		if (rcClient.right == rc.right)
			rcNowLine.left = rcClient.left;

		rcNowLine.left += XTP_DPI_X(5);
		rcNowLine.right -= XTP_DPI_X(1);

		pDC->FillSolidRect(&rcNowLine, m_clrNowLineShadow);
		rcNowLine.DeflateRect(0, XTP_DPI_Y(1), 0, 0);
		pDC->FillSolidRect(&rcNowLine, m_clrNowLine);
		rcNowLine.DeflateRect(0, XTP_DPI_Y(1), 0, 0);
		pDC->FillSolidRect(&rcNowLine, m_clrNowLineShadow);
	}
}

void CXTPCalendarOffice2013Theme::PartDrawNowLine(CDayViewTimeScaleHeaderPart* pViewPart, CDC* pDC,
												  CXTPCalendarDayView* pView,
												  CXTPCalendarDayViewTimeScale* pScale, CRect rc,
												  int y, BOOL bDrawBk)
{
	UNREFERENCED_PARAMETER(bDrawBk);

	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);
	ASSERT(pScale);

	if (pDC->GetSafeHdc() && pViewPart && pView)
	{
		CRect rcClient;
		pViewPart->GetCalendarControl()->GetClientRect(rcClient);

		CRect rcNowLine  = rc;
		rcNowLine.top	= y;
		rcNowLine.bottom = y + XTP_DPI_Y(3);

		if (pScale == pView->GetTimeScale(2)) // m_pTimeScaleHeaderAdditionalLeft
		{
			rcNowLine.left += XTP_DPI_X(5);
		}
		if (pScale == pView->GetTimeScale(1)) // m_pTimeScaleHeaderLeft
		{
			if (!pView->IsAdditionalScaleLeftVisible())
				rcNowLine.left += XTP_DPI_X(5);

			rcNowLine.right = rcClient.right;

			if (pView->IsScaleRightVisible())
				rcNowLine.right -= pScale->GetWidth();
			if (pView->IsAdditionalScaleRightVisible())
				rcNowLine.right -= pScale->GetWidth();
			if (!pView->IsScaleRightVisible() && !pView->IsAdditionalScaleRightVisible())
				rcNowLine.right -= XTP_DPI_X(1);
		}
		if (pScale == pView->GetTimeScale(3)) // m_pTimeScaleHeaderRight
		{
			if (!pView->IsAdditionalScaleRightVisible())
				rcNowLine.right -= XTP_DPI_X(1);

			rcNowLine.left = rcClient.left;

			if (pView->IsScaleLeftVisible())
				rcNowLine.left += pScale->GetWidth();
			if (pView->IsAdditionalScaleLeftVisible())
				rcNowLine.left += pScale->GetWidth();
			if (!pView->IsScaleLeftVisible() && !pView->IsAdditionalScaleLeftVisible())
				rcNowLine.left += XTP_DPI_X(5);
		}
		if (pScale == pView->GetTimeScale(4)) // m_pTimeScaleHeaderAdditionalRight
		{
			rcNowLine.right -= XTP_DPI_X(1);
		}

		pDC->FillSolidRect(&rcNowLine, m_clrNowLineShadow);
		rcNowLine.DeflateRect(0, XTP_DPI_Y(1), 0, 0);
		pDC->FillSolidRect(&rcNowLine, m_clrNowLine);
		rcNowLine.DeflateRect(0, XTP_DPI_Y(1), 0, 0);
		pDC->FillSolidRect(&rcNowLine, m_clrNowLineShadow);
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CDayViewCellPart* pViewPart, CDC* pDC,
										   CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
										   const XTP_CALENDAR_DAYVIEWCELL_PARAMS& cellParams)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewGroup);

	if (pDC->GetSafeHdc() && pViewPart && pViewGroup)
	{
		int iLeftMargin	= 1;
		COLORREF clrBorder = pViewPart->m_clrHour;

		// TODO: remove vertical gray cell line in the left

		COleDateTime dtTime = cellParams.dtBeginTime;

		CRect rcBusyStatus(rc);
		rcBusyStatus.right = rcBusyStatus.left + (iLeftMargin - XTP_DPI_X(1));

		pDC->FillSolidRect(&rcBusyStatus, m_clrWindow);
		int nDayBusyStatus = pViewGroup->GetBusyStatus(dtTime);
		if (nDayBusyStatus != xtpCalendarBusyStatusUnknown)
			DrawBusyStatus(pDC, rcBusyStatus, nDayBusyStatus);

		if (XTP_SAFE_GET2(pViewGroup, GetViewDay(), GetDayIndex(), 0) != 0
			|| pViewGroup->GetGroupIndex() != 0)
			pDC->FillSolidRect(rc.left, rc.top, XTP_DPI_X(1), rc.Height(), clrBorder);

		int nDayDeviderX = 0; // pViewGroup->GetGroupIndex() ? -2 : 0;

		pDC->FillSolidRect(rc.left + (iLeftMargin - XTP_DPI_X(1)) + nDayDeviderX, rc.top,
						   XTP_DPI_X(1), rc.Height(), clrBorder);

		rc.left += iLeftMargin + nDayDeviderX;
		pDC->FillSolidRect(rc, cellParams.clrBackgroundColor);

		if (dtTime.GetMinute() == 0 && dtTime.GetHour() != 0)
			pDC->FillSolidRect(rc.left, rc.top - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   pViewPart->m_clrHour);

		COleDateTime begin, end;
		pViewGroup->GetViewDay()->GetView()->GetSelection(&begin, &end);

		// this check doesn't look fine and pretty, but this is how Outlook2013 deals with selection
		// it means that we shouldn't draw dotted line for a the last selected cell (in selection)
		if (dtTime >= begin && dtTime <= end
			&& (dtTime + pViewGroup->GetViewDay()->GetView()->GetCellDuration() >= end))
		{
		}
		else
		{
			COLORREF clrDottedLineColor = cellParams.bSelected ? m_clrCellBack
															   : pViewPart->m_clrShadow;

			CPen outerPen;
			outerPen.CreatePen(PS_DOT, 1, clrDottedLineColor);
			CPen* pOldPen = pDC->SelectObject(&outerPen);

			pDC->MoveTo(rc.left, rc.bottom - XTP_DPI_Y(1));
			pDC->LineTo(rc.right, rc.bottom - XTP_DPI_Y(1));

			pDC->SelectObject(pOldPen);

			outerPen.DeleteObject();
		}
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CDayViewAllDayEventsPart* pViewPart, CDC* pDC,
										   CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
										   BOOL bSelected)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewGroup);

	if (pDC->GetSafeHdc() && pViewPart && pViewGroup && pViewGroup->GetViewDay())
	{
		pDC->FillSolidRect(rc, bSelected ? m_clrHighlight.operator COLORREF()
										 : pViewPart->GetBackgroundColor());

		COLORREF clrBorder = m_clrGridLine;
		CXTPPenDC pen(*pDC, clrBorder);

		// draw left border.
		pDC->MoveTo(rc.left, rc.top);
		pDC->LineTo(rc.left, rc.top + rc.Height());

		// draw bottom border.
		pDC->MoveTo(rc.left, rc.top + rc.Height() - XTP_DPI_Y(1));
		pDC->LineTo(rc.left + rc.Width(), rc.top + rc.Height() - XTP_DPI_Y(1));
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CDayViewHeaderPart* pViewPart, CDC* pDC,
										   CXTPCalendarDayViewDay* pViewDay, CRect rc,
										   CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewDay);

	if (pDC->GetSafeHdc() && pViewPart && pViewDay && pViewDay->GetView())
	{
		COLORREF clrRestore = pDC->GetTextColor();

		BOOL bIsToday = XTP_SAFE_GET1(pViewPart->GetCalendarControl(),
									  IsToday(pViewDay->GetDayDate()), FALSE);
		if (bIsToday)
		{
			pDC->FillSolidRect(rc, m_clrHeaderBackSelected);
			pDC->SetTextColor(m_clrHeaderTextSelected);
		}
		else
		{
			pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());
			pDC->SetTextColor(pViewPart->GetTextColor());
		}

		COleDateTime dtDate = pViewDay->GetDayDate();
		CString strDayName  = CXTPCalendarUtils::GetLocaleString(
			 XTPToUIntChecked(LOCALE_SDAYNAME1 + ((dtDate.GetDayOfWeek() - 2 + 7) % 7)), 100);
		strText = dtDate.Format(_T("%d ")) + strDayName;
		strText.MakeUpper();

		rc.DeflateRect(XTP_DPI_X(10), 0, 0, 0);

		CFont font;
		CreateHeaderFont(pDC, &pViewPart->GetTextFont(), &font, bIsToday);
		{
			CXTPFontDC fontDC(pDC, &font);
			pDC->DrawText(strText, rc,
						  DT_VCENTER | DT_LEFT | DT_NOPREFIX | DT_SINGLELINE | DT_END_ELLIPSIS);
			pDC->SetTextColor(clrRestore);
		}
		font.DeleteObject();
	}
}

void CXTPCalendarOffice2013Theme::PartDrawDayHeader(CWeekViewPart* pViewPart, CDC* pDC, CRect rcDay,
													int nHeaderHeight, CString strHeader,
													BOOL bIsCurrent, BOOL bIsSelected)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		pDC->SetBkMode(TRANSPARENT);

		CRect rcHeader(rcDay);
		rcHeader.DeflateRect(0, 0, 0, rcHeader.Height() - (nHeaderHeight - XTP_DPI_Y(1)));

		COLORREF clrOld = pDC->GetTextColor();

		if (bIsSelected)
		{
			pDC->SetTextColor(m_clrHeaderTextSelected);
			pDC->FillSolidRect(rcHeader, m_clrHighlight);
		}
		else if (bIsCurrent)
		{
			pDC->SetTextColor(m_clrHeaderTextSelected);
			pDC->FillSolidRect(rcHeader, m_clrHeaderBackSelected);
		}
		else
		{
			pDC->SetTextColor(m_clrHeaderText);
			pDC->FillSolidRect(rcHeader, m_clrHeaderBack);
		}

		CFont& font = pViewPart->GetTextFont();

		CFont fontBold;
		CreateBoldFont(&font, &fontBold);
		{
			CXTPFontDC fontDC(pDC, (bIsSelected || bIsCurrent) ? &fontBold : &font);

			CRect rcText(rcHeader);
			rcText.DeflateRect(XTP_DPI_X(10), 0, XTP_DPI_X(10), 0);

			strHeader.MakeUpper();
			pDC->DrawText(strHeader, rcText, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
			pDC->SetTextColor(clrOld);
		}
		fontBold.DeleteObject();
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CWeekViewPart* pViewPart, CDC* pDC,
										   CXTPCalendarWeekView* pWeekView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pWeekView);

	if (pDC->GetSafeHdc() && pViewPart && pWeekView)
	{
		CRect rcView(pWeekView->GetViewRect());

		pDC->FillSolidRect(rcView, pViewPart->m_clrBackground);

		// draw grid
		int nDayWidth = pWeekView->GetDayWidth();

		COLORREF clrGridColor = pViewPart->m_clrHeaderBottomLine;

		CPen outerPen;
		outerPen.CreatePen(PS_SOLID, 1, clrGridColor);
		CPen* pOldPen = pDC->SelectObject(&outerPen);

		DrawVerticalLine(pDC, rcView.left + nDayWidth - XTP_DPI_X(1), rcView.top, rcView.bottom);

		pDC->SelectObject(pOldPen);
		outerPen.DeleteObject();

		// draw headers
		BOOL bIsSelected	   = FALSE;
		BOOL bIsCurrent		   = FALSE;
		COleDateTime dtCurrent = CXTPCalendarUtils::ResetTime(COleDateTime::GetCurrentTime());

		COleDateTime dtDay;

		int nDayHeaderHeight = pWeekView->GetDayHeaderHeight();
		for (int nDay = 0; nDay < 7; nDay++)
		{
			CXTPCalendarWeekViewDay* pWDay = pWeekView->GetViewDay(nDay);
			ASSERT(pWDay);
			if (!pWDay)
			{
				continue;
			}

			dtDay		= pWDay->GetDayDate();
			bIsSelected = pWDay->IsSelected();
			bIsCurrent  = CXTPCalendarUtils::IsEqual(dtDay, dtCurrent);

			CRect rcDay(pWDay->GetDayRect());
			rcDay.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);

			if (bIsSelected || bIsCurrent)
			{
				COLORREF clrBorder = bIsSelected ? m_clrHighlight : m_clrHeaderBackSelected;
				CRect rcBorder(rcDay);

				rcBorder.DeflateRect(0, 0, 0, XTP_DPI_Y(1));

				for (int i = 0; i < XTP_DPI_X(3); i++)
				{
					pDC->Draw3dRect(rcBorder, clrBorder, clrBorder);
					rcBorder.DeflateRect(1, 0, 1, 1);
				}
			}
			CString strDayName = CXTPCalendarUtils::GetLocaleString(
				XTPToUIntChecked(LOCALE_SDAYNAME1 + ((dtDay.GetDayOfWeek() - 2 + 7) % 7)), 100);
			CString strMonthName = CXTPCalendarUtils::GetLocaleString(
				XTPToUIntChecked(LOCALE_SMONTHNAME1 + (dtDay.GetMonth() - 1)), 100);

			CString strDate = strDayName + _T(", ") + strMonthName + dtDay.Format(_T(" %d"));
			PartDrawDayHeader(pViewPart, pDC, rcDay, nDayHeaderHeight, strDate, bIsCurrent,
							  bIsSelected);
		}
	}
}

void CXTPCalendarOffice2013Theme::PartDrawEventBack(CXTPCalendarViewEvent* pViewEvent,
													CXTPCalendarViewPart* pViewPart, CDC* pDC,
													CRect& rcEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent)
	{
		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();
		BOOL bSelected = pViewEvent->IsSelected();

		// draw busy status rect and fill event background.
		PartDrawEventBusy(pViewEvent, pViewPart, pDC, rcEvent);

		// draw select border
		if (bSelected)
		{
			int r = ((nMDEFlags & xtpCalendarMultiDayLast) == 0) ? 0 : 1;
			int l = ((nMDEFlags & xtpCalendarMultiDayFirst) == 0) ? 0 : 1;
			PartDrawEventSelectedBorder(pDC, rcEvent, XTP_DPI_X(l), 0, XTP_DPI_X(r), 0);
		}
	}
}

void CXTPCalendarOffice2013Theme::PartDrawEventTime(CXTPCalendarViewEvent* pViewEvent,
													CXTPCalendarViewPart* pViewPart, CDC* pDC,
													CRect& rcEvent, CRect& rcText,
													BOOL bTimeAsClock)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent && pViewEvent->GetEvent())
	{
		CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
		COLORREF clrBack		  = GetEventBackColor(pEvent);

		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();
		BOOL bMultiDay = (nMDEFlags & xtpCalendarMultiDayFMLmask) ? TRUE : FALSE;

		DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(),
											  GetCalendarOptions(), dwAdditionalOptions, 0);

		BOOL bShowEndTime			  = FALSE;
		CXTPCalendarView* pActiveView = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(),
													  GetActiveView(), FALSE);
		if (pActiveView)
		{
			if (pActiveView
				== XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), GetWeekView(), FALSE))
				bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(),
											 WeekView_IsShowEndDate(), TRUE);

			if (pActiveView
				== XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), GetMonthView(), FALSE))
				bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(),
											 MonthView_IsShowEndDate(), TRUE);
		}

		BOOL bDrawStartTime = 0 != (dwAdditionalOpt & xtpCalendarOptMonthViewShowStartTimeAlways);
		BOOL bDrawEndTime   = 0 != (dwAdditionalOpt & xtpCalendarOptMonthViewShowEndTimeAlways);

		COleDateTime dtStart(pEvent->GetStartTime());
		COleDateTime dtEnd(pEvent->GetEndTime());

		CFont& font  = pViewPart->GetTextFont();
		CFont* pFont = &font;

		CFont fontBold;
		CreateBoldFont(&font, &fontBold);

		CRect rcSubLoc(rcText);

		if (bMultiDay)
		{
			if (ShowMultiDayTime(pViewEvent, pViewPart))
			{
				pFont		   = &fontBold;
				bDrawEndTime   = ((nMDEFlags & xtpCalendarMultiDayLast) != 0);
				bDrawStartTime = ((nMDEFlags & xtpCalendarMultiDayFirst) != 0);

				CString strSub = pViewEvent->GetItemTextSubject();
				CString strLoc = pViewEvent->GetItemTextLocation();

				CSize szSub(GetTextSize(pDC, strSub, &fontBold));
				CSize szLoc(GetTextSize(pDC, strLoc, &font));

				int cx		   = szSub.cx + XTP_DPI_X(5) + szLoc.cx;
				rcSubLoc.left  = rcText.left + (rcText.Width() - cx) / 2;
				rcSubLoc.right = rcSubLoc.left + cx;
			}
			else
			{
				return;
			}
		}

		CXTPFontDC fontDC(pDC, pFont);

		if (bDrawStartTime || (bDrawEndTime && bShowEndTime))
		{
			if (bTimeAsClock)
			{
				CSize szClock(XTP_DPI_X(15), XTP_DPI_Y(15));

				// calc start clock rect.
				CRect rcStart;
				rcStart.top	= rcEvent.top;
				rcStart.bottom = rcStart.top + szClock.cy;
				rcStart.left   = rcText.left;
				rcStart.right  = rcStart.left + szClock.cx;

				// calc end clock rect.
				CRect rcEnd;
				rcEnd.top	= rcEvent.top;
				rcEnd.bottom = rcEnd.top + szClock.cy;
				rcEnd.left   = rcStart.right;
				rcEnd.right  = rcEnd.left + szClock.cx;

				if (bDrawStartTime)
				{
					if (rcStart.right > rcEvent.right - XTP_DPI_X(m_nPadding))
					{
						rcStart.right = rcEvent.right - XTP_DPI_X(m_nPadding);
						bDrawEndTime  = FALSE;
					}

					if (rcStart.right > rcStart.left)
					{
						if (bMultiDay)
						{
							if (rcStart.right + XTP_DPI_X(m_nPadding) > rcSubLoc.left)
								return;
						}

						DrawClock(pDC, dtStart, rcStart, clrBack, xtpCalendarClockAlignCenter);

						if (!bMultiDay)
						{
							rcText.left = rcStart.right + XTP_DPI_X(m_nPadding);
						}
					}
				}
				else
				{
					rcEnd.CopyRect(rcStart);
				}

				if (bDrawEndTime && bShowEndTime && bDrawStartTime
					&& !CXTPCalendarUtils::IsEqual(dtStart, dtEnd))
				{
					if (rcEnd.right > rcEvent.right - XTP_DPI_X(m_nPadding))
					{
						rcEnd.right = rcEvent.right - XTP_DPI_X(m_nPadding);
					}

					if (rcEnd.right > rcEnd.left)
					{
						if (bMultiDay)
						{
							if (rcEnd.right + XTP_DPI_X(m_nPadding) > rcSubLoc.left)
								return;
						}

						DrawClock(pDC, dtEnd, rcEnd, clrBack, xtpCalendarClockAlignCenter);

						if (!bMultiDay)
						{
							rcText.left = rcEnd.right + XTP_DPI_X(m_nPadding);
						}
					}
				}
			}
			else
			{
				DWORD dwTimeFlags = 0L;

				if (bDrawStartTime)
				{
					dwTimeFlags = xtpCalendarItemText_EventStartTimeText;
				}

				if (bDrawEndTime && bShowEndTime && bDrawStartTime
					&& !CXTPCalendarUtils::IsEqual(dtStart, dtEnd))
				{
					dwTimeFlags |= xtpCalendarItemText_EventEndTimeText;
				}

				CString strTime(pViewEvent->GetItemText(dwTimeFlags));

				CSize szTime(GetTextSize(pDC, strTime, pFont));

				CRect rcTime;
				rcTime.top	= rcEvent.top + (rcEvent.Height() - szTime.cy) / 2;
				rcTime.bottom = rcTime.top + szTime.cy;
				rcTime.left   = rcText.left;
				rcTime.right  = rcTime.left + szTime.cx;

				if (rcTime.right > rcEvent.right - XTP_DPI_X(m_nPadding))
				{
					rcTime.right = rcEvent.right - XTP_DPI_X(m_nPadding);
				}

				if (rcTime.right > rcTime.left)
				{
					if (bMultiDay)
					{
						if (rcTime.right + XTP_DPI_X(m_nPadding) > rcSubLoc.left)
							return;
					}

					pDC->DrawText(strTime, &rcTime,
								  DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

					if (!bMultiDay)
					{
						rcText.left = rcTime.right + XTP_DPI_X(m_nPadding);
					}
				}
			}
		}
	}
}

BOOL CXTPCalendarOffice2013Theme::ShowMultiDayTime(CXTPCalendarViewEvent* pViewEvent,
												   CXTPCalendarViewPart* pViewPart)
{
	ASSERT(pViewEvent);
	UNREFERENCED_PARAMETER(pViewPart);

	if (!pViewEvent || !pViewEvent->GetEvent())
	{
		return FALSE;
	}

	BOOL bHideTimes = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(), GetCalendarOptions(),
									bMonthView_HideTimes, FALSE);
	if (XTP_SAFE_GET3(pViewEvent, GetCalendarControl(), GetActiveView(), GetViewType(), -1)
			== xtpCalendarMonthView
		&& bHideTimes)
	{
		return FALSE;
	}

	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();
	if (nMDEFlags & xtpCalendarMultiDayFirst || nMDEFlags & xtpCalendarMultiDayLast)
	{
		BOOL fAllTimesZero = CXTPCalendarUtils::IsZeroTime(pViewEvent->GetEvent()->GetStartTime())
							 && CXTPCalendarUtils::IsZeroTime(pViewEvent->GetEvent()->GetEndTime());
		if (!pViewEvent->GetEvent()->IsAllDayEvent() && !fAllTimesZero)
		{
			return (nMDEFlags & xtpCalendarMultiDayFirst) || (nMDEFlags & xtpCalendarMultiDayLast);
		}
	}

	return FALSE;
}

void CXTPCalendarOffice2013Theme::PartDrawEventText(CXTPCalendarViewEvent* pViewEvent,
													CXTPCalendarViewPart* pViewPart, CDC* pDC,
													CRect& rcEvent, CRect& rcText,
													int nViewDayIndex)
{
	UNREFERENCED_PARAMETER(rcEvent);

	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent && pViewEvent->GetEvent())
	{
		CString strSub = pViewEvent->GetItemTextSubject();
		CString strLoc = pViewEvent->GetItemTextLocation();

		CFont& font = pViewPart->GetTextFont();

		CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
		COleDateTime dtStart(pEvent->GetStartTime());
		COleDateTime dtEnd(pEvent->GetEndTime());

		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();
		BOOL bMultiDay = (nMDEFlags & xtpCalendarMultiDayFMLmask) ? TRUE : FALSE;

		CRect rcSub(rcText);
		CRect rcLoc(rcText);
		CXTPFontDC fontDC(pDC, &font);

		if (!bMultiDay)
		{
			CString strEvent;

			if (strSub.GetLength() > 0)
				strEvent.Format(_T("%s; %s"), strSub.operator LPCTSTR(), strLoc.operator LPCTSTR());
			else
				strEvent.Format(_T("%s"), strLoc.operator LPCTSTR());

			pDC->DrawText(strEvent, &rcText,
						  DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
		else
		{
			CFont fontBold;
			CreateBoldFont(&font, &fontBold);

			if (strSub.GetLength() > 0)
				strSub += _T(";");

			// get initial subject and location text size.
			CSize szSub(GetTextSize(pDC, strSub, &fontBold));
			CSize szLoc(GetTextSize(pDC, strLoc, &font));

			// calculate starting points and sizes.
			int cx = szSub.cx + XTP_DPI_X(5) + szLoc.cx;
			int cy = min(szSub.cy != 0 ? szSub.cy : szLoc.cy, szLoc.cy != 0 ? szLoc.cy : szSub.cy);
			int x  = rcText.left + (rcText.Width() - cx) / 2;
			int y  = rcText.top + (rcText.Height() - cy) / 2;

			// calculate subject text rect.
			rcSub.left   = (cx < rcText.Width()) ? x : rcText.left;
			rcSub.right  = rcSub.left + szSub.cx;
			rcSub.top	= y;
			rcSub.bottom = rcSub.top + szSub.cy;

			if (rcSub.right > rcText.right)
			{
				rcSub.right = rcText.right;
			}

			// calculate location text rect.
			rcLoc.left   = rcSub.right + XTP_DPI_X(5);
			rcLoc.right  = rcLoc.left + szLoc.cx;
			rcLoc.top	= y;
			rcLoc.bottom = rcLoc.top + szLoc.cy;

			if (rcLoc.right > rcText.right)
			{
				rcLoc.right = rcText.right;
			}

			// draw subject text.
			fontDC.SetFont(&fontBold);
			if (rcSub.right < rcText.right)
			{
				pDC->DrawText(strSub, &rcSub, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}
			else if (rcSub.left < rcText.right)
			{
				pDC->DrawText(strSub, &rcSub,
							  DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
			}

			// draw location text.
			fontDC.SetFont(&font);
			if (rcLoc.right < rcText.right)
			{
				pDC->DrawText(strLoc, &rcLoc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}
			else if (rcLoc.left < rcText.right)
			{
				pDC->DrawText(strLoc, &rcLoc,
							  DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
			}

			if ((nMDEFlags & xtpCalendarMultiDayFirst) == 0)
			{
				if (nViewDayIndex == 0)
				{
					DrawMultiDayArrowL(pDC, rcSub.left, rcText, dtStart, font);
				}
			}

			if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
			{
				if (nViewDayIndex == 6)
				{
					DrawMultiDayArrowR(pDC, rcLoc.right, rcText, dtEnd, font);
				}
			}

			fontBold.DeleteObject();
		}
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CWeekViewEventPart* pViewPart, CDC* pDC,
										   CXTPCalendarWeekViewEvent* pViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent && pViewEvent->GetEvent())
	{
		CRect rcEvent(pViewEvent->GetEventRect());
		if (rcEvent.IsRectEmpty())
		{
			return;
		}

		CRect rcText = pViewEvent->GetTextRect();

		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();
		BOOL bMultiDay = (nMDEFlags & xtpCalendarMultiDayFMLmask) ? TRUE : FALSE;

		rcEvent.DeflateRect(XTP_DPI_X(m_nPadding) + XTP_DPI_X(1), 0,
							XTP_DPI_X(m_nPadding) + XTP_DPI_X(1), 0);
		rcText.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);

		if (bMultiDay)
		{
			if ((nMDEFlags & xtpCalendarMultiDayFirst) == 0)
			{
				rcEvent.left -= XTP_DPI_X(m_nPadding);
			}
			if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
			{
				rcEvent.right += XTP_DPI_X(m_nPadding);
			}
		}

		// draw event background.
		PartDrawEventBack(pViewEvent, pViewPart, pDC, rcEvent);

		// define working area for text display.
		rcText.right = rcEvent.right - XTP_DPI_X(m_nPadding);
		rcText.left  = rcEvent.left + m_nBusyWidth + XTP_DPI_X(m_nPadding);

		COLORREF clrOld = pDC->SetTextColor(pViewPart->GetTextColor());

		// draw time
		PartDrawEventTime(pViewEvent, pViewPart, pDC, rcEvent, rcText, pViewEvent->IsTimeAsClock());

		CRect rcIcons(rcText);
		rcIcons.DeflateRect(0, XTP_DPI_Y(2));

		int cxIcons = pViewEvent->CalcIconsRect(rcIcons);
		if (cxIcons)
		{
			rcText.left += cxIcons + XTP_DPI_X(m_nPadding);
			DrawIcons(pDC, pViewEvent);
		}

		CXTPCalendarWeekViewDay* pMVDay = XTP_SAFE_GET2(pViewEvent, GetViewGroup(), GetViewDay(),
														NULL);
		int nViewDayIndex				= pMVDay ? pMVDay->GetDayIndex() : -1;

		PartDrawEventText(pViewEvent, pViewPart, pDC, rcEvent, rcText, nViewDayIndex);

		pDC->SetTextColor(clrOld);
		pViewPart->m_clrTextColor.SetDefaultValue();

		DrawCategories(pDC, rcEvent, pViewEvent->GetEvent());
	}
}

void CXTPCalendarOffice2013Theme::PartDrawGrid(CMonthViewGridPart* pViewPart, CDC* pDC,
											   CXTPCalendarMonthView* pMonthView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pMonthView);

	if (pDC->GetSafeHdc() && pViewPart && pMonthView && pMonthView->GetGrid()
		&& pMonthView->GetCalendarControl())
	{
		COLORREF clrGridLine = m_clrMonthGridLine;
		BOOL bCompressWeD	= pMonthView->GetCalendarControl()->MonthView_IsCompressWeekendDays();
		int nWeeksCount		 = pMonthView->GetGrid()->GetWeeksCount();

		COleDateTime dtNow = CXTPCalendarUtils::GetCurrentTime();

		for (int nWeek = 0; nWeek < nWeeksCount; nWeek++)
		{
			for (int nDayIdx = 0; nDayIdx < 7; nDayIdx++)
			{
				COleDateTime dtDay = pMonthView->GetGrid()->GetDayDate(nWeek, nDayIdx);
				CRect rcB		   = pMonthView->GetGrid()->GetDayRect(nWeek, nDayIdx, TRUE);

				// Draw Grid lines
				if (nWeek > 0)
				{
					pDC->FillSolidRect(rcB.left, rcB.top, rcB.Width(), XTP_DPI_Y(1), clrGridLine);
				}
				if (nWeek < nWeeksCount - 1)
				{
					pDC->FillSolidRect(rcB.left, rcB.bottom, rcB.Width(), XTP_DPI_Y(1),
									   clrGridLine);
				}
				int nWDay			   = dtDay.GetDayOfWeek();
				BOOL bCompressedCol_Su = bCompressWeD && nDayIdx == 1 && nWDay == 1;
				if (nDayIdx > 0 && !bCompressedCol_Su)
				{
					pDC->FillSolidRect(rcB.left, rcB.top, XTP_DPI_X(1), rcB.Height(), clrGridLine);
				}

				// Fill Grid cells background
				CRect rcDay = pMonthView->GetGrid()->GetDayRect(nWeek, nDayIdx, FALSE);

				COLORREF clrBackground = pViewPart->m_clrBackground;
				if (nDayIdx == 0)
				{
					rcDay.left--;
				}

				UINT uDay			   = (UINT)dtDay;
				BOOL bDiscreteSelected = pMonthView->IsInDiscreteSelection(uDay);

				if (bDiscreteSelected)
				{
					if (clrBackground == pViewPart->m_clrBackground)
						clrBackground = pViewPart->m_clrBackground2;
					else
						clrBackground = pViewPart->m_clrBackground;
				}
				else
				{
					// Show continue selection in non Office 2007 mode
					COleDateTime dBeg, dEnd;
					pMonthView->GetSelection(&dBeg, &dEnd);
					// if (dtDay >= dBeg && dtDay <= dEnd)
					if (dtDay >= dBeg && dtDay < dEnd)
					{
						if (clrBackground == pViewPart->m_clrBackground)
							clrBackground = pViewPart->m_clrBackground2;
						else
							clrBackground = pViewPart->m_clrBackground;
					}
				}
				pDC->FillSolidRect(&rcDay, clrBackground);
			}
		}
	}
}

void CXTPCalendarOffice2013Theme::PartDraw(CMonthViewHeaderPart* pViewPart, CDC* pDC,
										   CXTPCalendarMonthView* pMonthView, CRect rc,
										   int nCollIndex, CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pMonthView);

	if (pDC->GetSafeHdc() && pViewPart && pMonthView && pMonthView->GetGrid())
	{
		COleDateTime dtCol(pMonthView->GetViewDayDate(nCollIndex));
		int nCol = dtCol.GetDayOfWeek();

		COleDateTime dtNow(COleDateTime::GetCurrentTime());
		int nNow = dtNow.GetDayOfWeek();

		int nCount = pMonthView->GetGrid()->GetColsCount();
		if (nCount == 6 && nNow == 1)
		{
			nNow = 7;
		}

		rc.InflateRect(0, 0, 0, XTP_DPI_Y(1));

		COLORREF clrOldTextColor = pDC->GetTextColor();

		// if (strCol.Find(strDay, 0) != -1)
		if (nNow == nCol)
		{
			pDC->SetTextColor(m_clrHeaderTextSelected);
			pDC->FillSolidRect(rc, m_clrHeaderBackSelected);
		}
		else
		{
			pDC->SetTextColor(pViewPart->GetTextColor());
			pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());
		}

		rc.DeflateRect(XTP_DPI_X(10), 0, 0, XTP_DPI_Y(1));

		strText.MakeUpper();

		CFont font;
		CreateHeaderFont(pDC, &pViewPart->GetTextFont(), &font, (nNow == nCol));
		{
			CXTPFontDC fontDC(pDC, &font);
			pDC->DrawText(strText, rc, DT_VCENTER | DT_LEFT | DT_NOPREFIX | DT_SINGLELINE);

			pViewPart->m_clrTextColor.SetDefaultValue();
			pDC->SetTextColor(clrOldTextColor);
		}
		font.DeleteObject();
	}
}

void CXTPCalendarOffice2013Theme::PartDrawEvent(CMonthViewEventPart* pViewPart, CDC* pDC,
												CXTPCalendarMonthViewEvent* pViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent && pViewEvent->GetEvent())
	{
		CRect rcEvent(pViewEvent->GetEventRect());
		if (rcEvent.IsRectEmpty())
		{
			return;
		}

		CRect rcText = pViewEvent->GetTextRect();

		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();
		BOOL bMultiDay = (nMDEFlags & xtpCalendarMultiDayFMLmask) ? TRUE : FALSE;

		CXTPCalendarMonthViewDay* pMVDay = XTP_SAFE_GET2(pViewEvent, GetViewGroup(), GetViewDay(),
														 NULL);
		int nViewDayIndex				 = pMVDay ? pMVDay->GetDayIndex() : -1;

		if (bMultiDay)
		{
			rcEvent.left -= XTP_DPI_X(1);
			rcText.left -= XTP_DPI_X(1);
		}
		else
		{
			rcEvent.left -= XTP_DPI_X(3);
			rcText.left -= XTP_DPI_X(3);
		}

		// draw event background.
		PartDrawEventBack(pViewEvent, pViewPart, pDC, rcEvent);

		// define working area for text display.
		rcText.right = rcEvent.right - XTP_DPI_X(m_nPadding);
		rcText.left  = rcEvent.left + m_nBusyWidth + XTP_DPI_X(m_nPadding);

		COLORREF clrOld = pDC->SetTextColor(pViewPart->GetTextColor());

		// draw time
		PartDrawEventTime(pViewEvent, pViewPart, pDC, rcEvent, rcText, pViewEvent->IsTimeAsClock());

		int cxIcons = pViewEvent->CalcIconsRect(rcText);
		if (cxIcons)
		{
			rcText.left += cxIcons + XTP_DPI_X(m_nPadding);
			DrawIcons(pDC, pViewEvent);
		}

		PartDrawEventText(pViewEvent, pViewPart, pDC, rcEvent, rcText, nViewDayIndex);

		pDC->SetTextColor(clrOld);
		pViewPart->m_clrTextColor.SetDefaultValue();

		DrawCategories(pDC, rcEvent, pViewEvent->GetEvent());
	}
}

void CXTPCalendarOffice2013Theme::PartDrawDayDate(CMonthViewEventPart* pViewPart, CDC* pDC,
												  CRect rc, BOOL bToday, BOOL bSelected,
												  BOOL bFirstMonthDay, CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		CRect rcTop  = rc;
		rcTop.bottom = rcTop.top + XTP_DPI_Y(3);

		rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

		if (bSelected)
		{
			DrawHeader(pDC, rc, bSelected, FALSE, pViewPart->m_grclrClassicSelDay,
					   pViewPart->m_clrClassicUnderline);
		}

		UINT dtTextFormat = DT_VCENTER | DT_SINGLELINE;

		dtTextFormat |= DT_LEFT;
		rc.DeflateRect(XTP_DPI_X(5), 0, 0, 0);

		COLORREF clrOldTextColor = pDC->GetTextColor();

		if (bSelected)
		{
			pDC->SetTextColor(pViewPart->m_clrTextHighLightColor);
		}

		if (bToday)
		{
			if (!bSelected)
			{
				pDC->SetTextColor(m_clrHeaderBackSelected);
			}

			pDC->FillSolidRect(rcTop, m_clrHeaderBackSelected);
		}

		CFont font;
		CreateHeaderFont(pDC, &pViewPart->GetTextFont(), &font, (bFirstMonthDay || bToday));
		{
			CXTPFontDC fontDC(pDC, &font);
			pDC->DrawText(strText, rc, dtTextFormat);

			pViewPart->m_clrTextColor.SetDefaultValue();

			pDC->SetTextColor(clrOldTextColor);
		}
		font.DeleteObject();
	}
}

void CXTPCalendarOffice2013Theme::PartAdjustTimeFont(CDayViewTimeScaleCellPart* pViewPart, CDC* pDC,
													 CRect rcCell)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		rcCell.DeflateRect(XTP_DPI_X(5), XTP_DPI_Y(6), XTP_DPI_X(1), XTP_DPI_Y(7));

		if (rcCell.Height() > XTP_DPI_Y(10))
			rcCell.DeflateRect(0, rcCell.Height() / 2 - XTP_DPI_Y(2), 0,
							   0); // this will make the font smaller as required in Outlook 2013

		if (rcCell.Height() > XTP_DPI_Y(26))
			rcCell.DeflateRect(0, 0, 0, rcCell.Height() - XTP_DPI_Y(26)); // set maximum font height

		if (rcCell.Height() < XTP_DPI_Y(14))
			rcCell.InflateRect(0, 0, 0, XTP_DPI_Y(15) - rcCell.Height()); // set minimum font height

		LOGFONT lfIcon;
		pViewPart->m_fntTimeText->GetLogFont(&lfIcon);
		CSize szText;
		{
			CXTPFontDC fontBig(pDC, pViewPart->m_fntTimeText);
			szText = pDC->GetTextExtent(_T("88"), 2);
		}

		int nParam1 = rcCell.Width() * szText.cy;
		int nParam2 = rcCell.Height() * szText.cx;
		if (nParam1 > nParam2)
			lfIcon.lfHeight = ::MulDiv(lfIcon.lfHeight, rcCell.Height(), szText.cy);
		else
			lfIcon.lfHeight = ::MulDiv(lfIcon.lfHeight, rcCell.Width(), szText.cx);

		pViewPart->m_fntTimeText.SetStandardValue(&lfIcon);
	}
}
