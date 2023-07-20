// XTPCalendarPaintManager.cpp : implementation file
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"
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
#include "Calendar/XTPCalendarWeekViewDay.h"
#include "Calendar/XTPCalendarWeekView.h"
#include "Calendar/XTPCalendarMonthViewEvent.h"
#include "Calendar/XTPCalendarMonthView.h"
#include "Calendar/XTPCalendarDayViewEvent.h"
#include "Calendar/XTPCalendarDayViewDay.h"
#include "Calendar/XTPCalendarDayView.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarTimeLineViewGroup.h"
#include "Calendar/XTPCalendarTimeLineViewTimeScale.h"
#include "Calendar/XTPCalendarTimeLineView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarShadowDrawer.h"
#include "Calendar/XTPCalendarData.h"

static const double dXTPMaxAlpha2003 = 75.;

static const LPCTSTR cszGlyphsFontName = _T("MS Outlook");

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarPaintManager

CXTPCalendarPaintManager::CXTPCalendarPaintManager()
{
	m_pSelFont			 = NULL;
	m_pGlyphsFont		 = NULL;
	m_szClockRect		 = CSize(XTP_DPI_X(15), XTP_DPI_Y(15));
	m_szExpandSign		 = CSize(0, 0);
	m_pControl			 = NULL;
	m_bEnableTheme		 = TRUE;
	m_dwAskItemTextFlags = 0L;
	m_CurrSystemTheme	= xtpSystemThemeUnknown;
	m_nPaintTheme		 = xtpCalendarThemeOffice2000;

	// Initialize calendar part objects.
	m_pControlPart				  = (CControlPart*)AddViewPart(new CControlPart());
	m_pDayViewTimeScaleHeaderPart = (CDayViewTimeScaleHeaderPart*)AddViewPart(
		new CDayViewTimeScaleHeaderPart(m_pControlPart));
	m_pDayViewTimeScaleCellPart = (CDayViewTimeScaleCellPart*)AddViewPart(
		new CDayViewTimeScaleCellPart(m_pDayViewTimeScaleHeaderPart));
	m_pDayViewCellPart	 = (CDayViewCellPart*)AddViewPart(new CDayViewCellPart(m_pControlPart));
	m_pDayViewWorkCellPart = (CDayViewWorkCellPart*)AddViewPart(
		new CDayViewWorkCellPart(m_pDayViewCellPart));
	m_pDayViewNonworkCellPart = (CDayViewNonworkCellPart*)AddViewPart(
		new CDayViewNonworkCellPart(m_pDayViewCellPart));
	m_pDayViewHeaderPart = (CDayViewHeaderPart*)AddViewPart(new CDayViewHeaderPart(m_pControlPart));
	m_pDayViewGroupHeaderPart = (CDayViewGroupHeaderPart*)AddViewPart(
		new CDayViewGroupHeaderPart(m_pControlPart));
	m_pDayViewAllDayEventsPart = (CDayViewAllDayEventsPart*)AddViewPart(
		new CDayViewAllDayEventsPart(m_pControlPart));
	m_pDayViewAllDayEventScrollIconsPart = (CDayViewAllDayEventScrollIconsPart*)AddViewPart(
		new CDayViewAllDayEventScrollIconsPart(m_pControlPart));
	m_pDayViewEventPart  = (CDayViewEventPart*)AddViewPart(new CDayViewEventPart(m_pControlPart));
	m_pWeekViewPart		 = (CWeekViewPart*)AddViewPart(new CWeekViewPart(m_pControlPart));
	m_pWeekViewEventPart = (CWeekViewEventPart*)AddViewPart(new CWeekViewEventPart(m_pControlPart));
	m_pMonthViewGridPart = (CMonthViewGridPart*)AddViewPart(new CMonthViewGridPart(m_pControlPart));
	m_pMonthViewHeaderPart = (CMonthViewHeaderPart*)AddViewPart(
		new CMonthViewHeaderPart(m_pControlPart));
	m_pMonthViewEventPart = (CMonthViewEventPart*)AddViewPart(
		new CMonthViewEventPart(m_pControlPart));
	m_pTimeLineViewPart = (CTimeLineViewPart*)AddViewPart(new CTimeLineViewPart(m_pControlPart));
	m_pTimeLineViewTimeScalePart_Day = (CTimeLineViewTimeScalePart_Day*)AddViewPart(
		new CTimeLineViewTimeScalePart_Day(m_pControlPart));
	m_pTimeLineViewTimeScalePart_Week = (CTimeLineViewTimeScalePart_Week*)AddViewPart(
		new CTimeLineViewTimeScalePart_Week(m_pControlPart));
	m_pTimeLineViewTimeScalePart_WorkWeek = (CTimeLineViewTimeScalePart_WorkWeek*)AddViewPart(
		new CTimeLineViewTimeScalePart_WorkWeek(m_pControlPart));
	m_pTimeLineViewTimeScalePart_Month = (CTimeLineViewTimeScalePart_Month*)AddViewPart(
		new CTimeLineViewTimeScalePart_Month(m_pControlPart));
}

CXTPCalendarPaintManager::~CXTPCalendarPaintManager()
{
	while (!m_lstViewParts.IsEmpty())
	{
		CXTPCalendarViewPart* pPart = m_lstViewParts.RemoveHead();
		SAFE_DELETE(pPart);
	}

	SAFE_DELETE(m_pGlyphsFont);
}

void CXTPCalendarPaintManager::DrawHeader(CDC* pDC, CRect& rcHeader, BOOL bIsSelected,
										  BOOL bIsCurrent)
{
	DrawHeader(pDC, rcHeader, bIsSelected, bIsCurrent, m_grclrToday, m_clrUnderLineHdr);
}

void CXTPCalendarPaintManager::DrawHeader(CDC* pDC, CRect& rcHeader, BOOL bIsSelected,
										  BOOL bIsCurrent,
										  const CXTPPaintManagerColorGradient& grclr,
										  COLORREF clrUnderLineHdr)
{
	ASSERT(pDC);

	if (pDC->GetSafeHdc())
	{
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
							   XTP_DPI_X(1), clrUnderLineHdr);
		}

		if (bIsSelected)
		{
			pDC->FillSolidRect(rcHeader, m_clrHighlight);
		}
	}
}

void CXTPCalendarPaintManager::DrawShadow(CDC* pDC, const CRect& rcRect)
{
	ASSERT(pDC);

	if (pDC->GetSafeHdc() && !pDC->IsPrinting())
	{
		CXTPCalendarShadowDrawer oShadow(pDC, dXTPMaxAlpha2003);

		CRect rcVShadow(rcRect);
		rcVShadow.left = rcVShadow.right;
		rcVShadow.right += XTP_DPI_X(5);
		rcVShadow.top += XTP_DPI_Y(5);
		oShadow.DrawShadowRectPart(rcVShadow, CXTPCalendarShadowDrawer::xtpShadowLR);

		CRect rcHShadow(rcRect);
		rcHShadow.left += XTP_DPI_X(7);
		rcHShadow.top = rcHShadow.bottom + XTP_DPI_Y(1);
		rcHShadow.bottom += XTP_DPI_X(6);
		oShadow.DrawShadowRectPart(rcHShadow, CXTPCalendarShadowDrawer::xtpShadowTB);

		CRect rcTRShadow(rcVShadow);
		rcTRShadow.bottom = rcTRShadow.top;
		rcTRShadow.top -= XTP_DPI_Y(5);
		oShadow.DrawShadowRectPart(rcTRShadow, CXTPCalendarShadowDrawer::xtpShadowBR);

		CRect rcBRShadow(rcVShadow);
		rcBRShadow.top = rcBRShadow.bottom;
		rcBRShadow.bottom += XTP_DPI_Y(5);
		oShadow.DrawShadowRectPart(rcBRShadow, CXTPCalendarShadowDrawer::xtpShadowTR);

		CRect rcBLShadow(rcRect);
		rcBLShadow.right = rcBLShadow.left + XTP_DPI_X(7);
		rcBLShadow.top   = rcBLShadow.bottom + XTP_DPI_Y(1);
		rcBLShadow.bottom += XTP_DPI_Y(6);
		oShadow.DrawShadowRectPart(rcBLShadow, CXTPCalendarShadowDrawer::xtpShadowBL);
	}
}

BOOL CXTPCalendarPaintManager::GetIconLogFont(LOGFONT* plf)
{
	return CXTPDrawHelpers::GetIconLogFont(plf);
}

void CXTPCalendarPaintManager::UpdateEventLabelsDefaultColors()
{
	COLORREF clrLabels[] = {
		RGB(255, 255, 255), // XTP_IDS_CALENDAR_EVENT_LABEL_NONE
		RGB(255, 148, 132), // XTP_IDS_CALENDAR_EVENT_LABEL_IMPORTANT
		RGB(132, 156, 231), // XTP_IDS_CALENDAR_EVENT_LABEL_BUSINESS
		RGB(165, 222, 99),  // XTP_IDS_CALENDAR_EVENT_LABEL_PERSONAL
		RGB(231, 231, 214), // XTP_IDS_CALENDAR_EVENT_LABEL_VACATION
		RGB(255, 181, 115), // XTP_IDS_CALENDAR_EVENT_LABEL_MUSTATTEND
		RGB(132, 239, 247), // XTP_IDS_CALENDAR_EVENT_LABEL_TRAVELREQUIRED
		RGB(214, 206, 132), // XTP_IDS_CALENDAR_EVENT_LABEL_NEEDSPREPARATION
		RGB(198, 165, 247), // XTP_IDS_CALENDAR_EVENT_LABEL_BIRTHDAY
		RGB(165, 206, 198), // XTP_IDS_CALENDAR_EVENT_LABEL_ANNIVERSARY
		RGB(255, 231, 115)  // XTP_IDS_CALENDAR_EVENT_LABEL_PHONECALL
	};

	XTP_SAFE_CALL2(m_pControl, GetDataProvider(),
				   UpdateEventLabels(clrLabels, _countof(clrLabels)));
}

void CXTPCalendarPaintManager::UpdateEventCategoriesDefaultColors()
{
	COLORREF clrBorder[] = {
		RGB(154, 149, 25), // XTP_IDS_CLR_YELLOW
		RGB(55, 126, 45),  // XTP_IDS_CLR_GREEN
		RGB(167, 29, 35),  // XTP_IDS_CLR_RED
		RGB(77, 49, 141),  // XTP_IDS_CLR_PURPLE
		RGB(173, 77, 13),  // XTP_IDS_CLR_ORANGE
		RGB(40, 81, 142)   // XTP_IDS_CLR_BLUE
	};

	COLORREF clrBkBase[] = {
		RGB(255, 251, 155), // XTP_IDS_CLR_YELLOW
		RGB(96, 194, 82),   // XTP_IDS_CLR_GREEN
		RGB(220, 56, 65),   // XTP_IDS_CLR_RED
		RGB(184, 164, 224), // XTP_IDS_CLR_PURPLE
		RGB(247, 187, 137), // XTP_IDS_CLR_ORANGE
		RGB(159, 183, 232)  // XTP_IDS_CLR_BLUE
	};

	XTP_SAFE_CALL2(m_pControl, GetDataProvider(),
				   UpdateEventCategories(clrBkBase, clrBorder, _countof(clrBkBase)));
}

void CXTPCalendarPaintManager::UpdateGlyphs()
{
	CSize szSexpandSign = GetExpandSignSize();
	m_ilExpandSigns.DeleteImageList();
	VERIFY(
		m_ilExpandSigns.Create(szSexpandSign.cx, szSexpandSign.cy, ILC_COLOR24 | ILC_MASK, 0, 1));

	m_ilGlyphs.DeleteImageList();

	CBitmap bmpSignUp, bmpSignDown, bmpGlyphs;
	const int glypsCount = 5;

	int nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_CALENDAR_EXPANDSIGNUP_100,
												   XTP_IDB_CALENDAR_EXPANDSIGNUP_125,
												   XTP_IDB_CALENDAR_EXPANDSIGNUP_150,
												   XTP_IDB_CALENDAR_EXPANDSIGNUP_200);
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadBitmap(&bmpSignUp, XTPToUInt(nBmpID)));

	nBmpID = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_CALENDAR_EXPANDSIGNDOWN_100,
											   XTP_IDB_CALENDAR_EXPANDSIGNDOWN_125,
											   XTP_IDB_CALENDAR_EXPANDSIGNDOWN_150,
											   XTP_IDB_CALENDAR_EXPANDSIGNDOWN_200);
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadBitmap(&bmpSignDown, XTPToUInt(nBmpID)));

	nBmpID		   = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_CALENDAR_EVENT_GLYPHS_100,
											   XTP_IDB_CALENDAR_EVENT_GLYPHS_125,
											   XTP_IDB_CALENDAR_EVENT_GLYPHS_150,
											   XTP_IDB_CALENDAR_EVENT_GLYPHS_200);
	CSize szGlyphs = GetBitmapSize(XTPToUInt(nBmpID));

	BOOL bRes = m_ilGlyphs.Create(szGlyphs.cx / glypsCount, szGlyphs.cy, ILC_COLOR24 | ILC_MASK, 0,
								  1);
	nBmpID	= XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_CALENDAR_EVENT_GLYPHS_100,
												  XTP_IDB_CALENDAR_EVENT_GLYPHS_125,
												  XTP_IDB_CALENDAR_EVENT_GLYPHS_150,
												  XTP_IDB_CALENDAR_EVENT_GLYPHS_200);
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadBitmap(&bmpGlyphs, XTPToUInt(nBmpID)));

	VERIFY(bRes);
	COLORREF clrMask = RGB(255, 0, 255);

	m_ilExpandSigns.Add(&bmpSignUp, clrMask);
	m_ilExpandSigns.Add(&bmpSignDown, clrMask);
	m_ilGlyphs.Add(&bmpGlyphs, clrMask);
}

void CXTPCalendarPaintManager::RefreshPartMetrics()
{
	POSITION pos = m_lstViewParts.GetHeadPosition();
	while (pos)
	{
		CXTPCalendarViewPart* pPart = m_lstViewParts.GetNext(pos);
		ASSERT(pPart);
		if (pPart)
		{
			pPart->RefreshMetrics();
		}
	}
}

void CXTPCalendarPaintManager::CreateGlyphsFont()
{
	SAFE_DELETE(m_pGlyphsFont);

	if (XTPDrawHelpers()->FontExists(cszGlyphsFontName))
	{
		m_pGlyphsFont = new CFont();
		if (m_pGlyphsFont)
		{
			LOGFONT lfGlyph;
			::ZeroMemory(&lfGlyph, sizeof(lfGlyph));

			STRCPY_S(lfGlyph.lfFaceName, LF_FACESIZE, cszGlyphsFontName);
			lfGlyph.lfCharSet = SYMBOL_CHARSET;
			lfGlyph.lfHeight  = XTP_DPI_Y(-11);
			lfGlyph.lfWeight  = FW_NORMAL;

			if (!m_pGlyphsFont->CreateFontIndirect(&lfGlyph))
			{
				SAFE_DELETE(m_pGlyphsFont);
			}
		}
	}

	if (!m_pGlyphsFont)
	{
		TRACE(_T("WARNING! Calendar Paint Manager: could not create 'MS Outlook' font, standard ")
			  _T("bitmaps will be used.\n"));
	}
}

void CXTPCalendarPaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	m_clrButtonFace.SetStandardValue(GetSysColor(COLOR_BTNFACE));
	m_clrButtonFaceText.SetStandardValue(GetSysColor(COLOR_BTNTEXT));
	m_clrWindow.SetStandardValue(GetSysColor(COLOR_WINDOW));
	m_clrWindowText.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
	m_clr3DShadow.SetStandardValue(GetSysColor(COLOR_3DSHADOW));
	m_clrHighlight.SetStandardValue(GetSysColor(COLOR_HIGHLIGHT));
	m_clrBorders[0].SetStandardValue(RGB(130, 130, 130));
	m_clrBorders[1].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[2].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[3].SetStandardValue(m_clrBorders[0]);
	m_grclrToday.SetStandardValue(m_clrButtonFace, m_clrButtonFace);
	m_clrUnderLineHdr = m_clrHighlight;

	// time line view colors.
	m_clrTLVEventBar.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrTLVEventBarLine.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));
	m_clrTLVEventBorderLine.SetStandardValue(GetXtremeColor(COLOR_INACTIVEBORDER));
	m_clrTLVTimeScaleBackground.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrTLVTimeScaleBorder.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW));
	m_clrTLVTimeScaleHighlight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrTLVTimeScaleText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_clrTLVText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_clrTLVBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrTLVSelectedText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrTLVSelectedBack.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrTLVWeekendBack.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	RefreshPartMetrics();

	UpdateGlyphs();
	CreateGlyphsFont();
	InitBusyStatusDefaultColors();
	UpdateEventLabelsDefaultColors();
	UpdateEventCategoriesDefaultColors();
}

////////////////////////////////////////////////////////////////////////////

CXTPCalendarViewPart* CXTPCalendarPaintManager::AddViewPart(CXTPCalendarViewPart* pPart)
{
	ASSERT(pPart);
	if (pPart)
	{
		m_lstViewParts.AddTail(pPart);
		pPart->m_pPaintManager = this;
	}

	return pPart;
}

void CXTPCalendarPaintManager::DrawLine(CDC* pDC, int x, int y, int cx, int cy)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return;
	}

	pDC->MoveTo(x, y);
	pDC->LineTo(x + cx, y + cy);
}

void CXTPCalendarPaintManager::DrawHorizontalLine(CDC* pDC, int x, int y, int cx)
{
	DrawLine(pDC, x, y, cx, 0);
}

void CXTPCalendarPaintManager::DrawVerticalLine(CDC* pDC, int x, int y, int cy)
{
	DrawLine(pDC, x, y, 0, cy);
}

int CXTPCalendarPaintManager::DrawBitmap(CImageList* pImageList, CDC* pDC, CRect rcView, int iIcon,
										 UINT uFlags)
{
	ASSERT(pImageList && pDC);
	if (!pImageList || !pDC)
	{
		return 0;
	}

	int nImageBound = rcView.left;

	if (rcView.left >= rcView.right - XTP_DPI_X(2))
		return nImageBound;

	IMAGEINFO imgInf;

	if (!pImageList->GetImageInfo(iIcon, &imgInf))
		return nImageBound;

	CSize szImage(CRect(imgInf.rcImage).Size());
	CSize szView(rcView.Size());
	CPoint ptIcon(rcView.TopLeft());

	szImage.cx = min(szImage.cx, szView.cx);

	if (uFlags & DT_VCENTER)
	{
		ptIcon.y = rcView.top + (szView.cy - szImage.cy) / 2 + XTP_DPI_Y(1);
	}

	IMAGELISTDRAWINDIRECT_S(pImageList, pDC, iIcon, ptIcon, szImage);

	return szImage.cx;
}

int CXTPCalendarPaintManager::DrawBitmap(UINT nIDResource, CDC* pDC, CRect rcBitmap)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return 0;
	}

	CBitmap bmpIcon;

	if (!XTPResourceManager()->LoadBitmap(&bmpIcon, nIDResource))
		return 0;

	BITMAP bmpInfo;
	bmpIcon.GetBitmap(&bmpInfo);

	CImageList ilBitmap;
	VERIFY(ilBitmap.Create(bmpInfo.bmWidth, bmpInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 1));
	ilBitmap.Add(&bmpIcon, RGB(255, 0, 255));

	CRect rcBitmap2  = rcBitmap;
	rcBitmap2.bottom = rcBitmap2.top + bmpInfo.bmHeight;

	DrawBitmap(&ilBitmap, pDC, rcBitmap2, 0);

	return bmpInfo.bmWidth;
}

CSize CXTPCalendarPaintManager::GetBitmapSize(UINT nIDResource)
{
	CBitmap bmpIcon;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmpIcon, nIDResource));

	BITMAP bmpInfo;
	bmpIcon.GetBitmap(&bmpInfo);
	CSize szIcon(bmpInfo.bmWidth, bmpInfo.bmHeight);
	return szIcon;
}

const CSize CXTPCalendarPaintManager::GetExpandSignSize()
{
	// if sign size is not initialized, initialize it
	if (!(m_szExpandSign.cx | m_szExpandSign.cy))
	{
		int idResource = XTPDpiHelper()->SelectDpiSpecific(XTP_IDB_CALENDAR_EXPANDSIGNDOWN_100,
														   XTP_IDB_CALENDAR_EXPANDSIGNDOWN_125,
														   XTP_IDB_CALENDAR_EXPANDSIGNDOWN_150,
														   XTP_IDB_CALENDAR_EXPANDSIGNDOWN_200);
		m_szExpandSign = GetBitmapSize(XTPToUInt(idResource));
	}
	return m_szExpandSign;
}

int CXTPCalendarPaintManager::DrawClock(CDC* pDC, COleDateTime dtClockTime, CRect rcView,
										COLORREF clrBackground, XTPCalendarClockAlignFlags cafAlign)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return 0;
	}

	const CPoint arrHours[] = { CPoint(0, XTP_DPI_Y(-6)),
								CPoint(XTP_DPI_X(3), XTP_DPI_Y(-5)),
								CPoint(XTP_DPI_X(5), XTP_DPI_Y(-3)),
								CPoint(XTP_DPI_X(6), 0),
								CPoint(XTP_DPI_X(5), XTP_DPI_Y(3)),
								CPoint(XTP_DPI_X(3), XTP_DPI_Y(5)),
								CPoint(0, XTP_DPI_Y(6)),
								CPoint(XTP_DPI_X(-3), XTP_DPI_Y(5)),
								CPoint(XTP_DPI_X(-5), XTP_DPI_Y(3)),
								CPoint(XTP_DPI_X(-6), 0),
								CPoint(XTP_DPI_X(-5), XTP_DPI_Y(-3)),
								CPoint(XTP_DPI_X(-3), XTP_DPI_Y(-5)),
								CPoint(0, XTP_DPI_Y(-6)) };

	const CPoint arrHalfHours[] =
		{ CPoint(XTP_DPI_X(1), XTP_DPI_Y(-5)),  CPoint(XTP_DPI_X(3), XTP_DPI_Y(-3)),
		  CPoint(XTP_DPI_X(4), XTP_DPI_Y(-1)),  CPoint(XTP_DPI_X(5), XTP_DPI_Y(1)),
		  CPoint(XTP_DPI_X(3), XTP_DPI_Y(3)),   CPoint(XTP_DPI_X(1), XTP_DPI_Y(5)),
		  CPoint(XTP_DPI_X(-1), XTP_DPI_Y(5)),  CPoint(XTP_DPI_X(-3), XTP_DPI_Y(3)),
		  CPoint(XTP_DPI_X(-5), XTP_DPI_Y(1)),  CPoint(XTP_DPI_X(-5), XTP_DPI_Y(-1)),
		  CPoint(XTP_DPI_X(-3), XTP_DPI_Y(-3)), CPoint(XTP_DPI_X(-1), XTP_DPI_Y(-5)),
		  CPoint(XTP_DPI_X(1), XTP_DPI_Y(-5)) };

	if (rcView.Width() < m_szClockRect.cx || rcView.Height() < m_szClockRect.cy)
		return 0;

	CPoint ptClock(rcView.left, rcView.top);

	if (cafAlign == xtpCalendarClockAlignLeft)
		ptClock.Offset(XTP_DPI_X(1), (rcView.Height() - m_szClockRect.cy) / 2 + XTP_DPI_Y(1));
	else if (cafAlign == xtpCalendarClockAlignRight)
		ptClock.Offset(rcView.Width() - m_szClockRect.cx - XTP_DPI_X(1),
					   (rcView.Height() - m_szClockRect.cy) / 2 + XTP_DPI_Y(1));
	else if (cafAlign == xtpCalendarClockAlignCenter)
		ptClock.Offset((rcView.Width() - m_szClockRect.cx) / 2,
					   (rcView.Height() - m_szClockRect.cy) / 2 + XTP_DPI_Y(1));

	CRect rcClock(0, 0, 0, 0);
	rcClock.SetRect(ptClock.x, ptClock.y, ptClock.x + m_szClockRect.cx,
					ptClock.y + m_szClockRect.cy);

	int nHour = dtClockTime.GetHour();
	int nMin  = dtClockTime.GetMinute() / 5;

	COLORREF clrFill;
	COLORREF clrItems;
	if (nHour >= 12)
	{
		clrFill  = RGB(0, 0, 0);
		clrItems = clrBackground;
	}
	else
	{
		clrFill  = clrBackground;
		clrItems = RGB(0, 0, 0);
	}

	nHour = nHour > 12 ? nHour - 12 : nHour;

	CXTPBrushDC brushClock(pDC->m_hDC, clrFill);
	CPoint ptCenter(rcClock.CenterPoint());
	pDC->Ellipse(rcClock);
	CXTPPenDC penItems(pDC->m_hDC, clrItems);
	int x = 0;
	int y = 0;

	for (int i = 0; i <= 12; i++)
	{
		pDC->SetPixel(ptCenter.x + arrHours[i].x, ptCenter.y + arrHours[i].y, clrItems);

		if (nHour == i)
		{
			if (nMin >= 6)
			{
				x = arrHalfHours[i].x;
				y = arrHalfHours[i].y;
			}
			else
			{
				x = arrHours[i].x;
				y = arrHours[i].y;
				x = x > 0 ? x - XTP_DPI_X(1) : x == 0 ? 0 : x + XTP_DPI_X(1);
				y = y > 0 ? y - XTP_DPI_Y(1) : y == 0 ? 0 : y + XTP_DPI_Y(1);
			}
			pDC->MoveTo(ptCenter);
			pDC->LineTo(ptCenter.x + x, ptCenter.y + y);
		}
		if (nMin == i)
		{
			pDC->MoveTo(ptCenter);
			pDC->LineTo(ptCenter.x + arrHours[i].x, ptCenter.y + arrHours[i].y);
		}
	}

	return m_szClockRect.cx;
}

void CXTPCalendarPaintManager::SetControl(CXTPCalendarControl* pControl)
{
	m_pControl = pControl;
}

int CXTPCalendarPaintManager::DrawIconChar(CDC* pDC, CRect rc, TCHAR ch)
{
	if (!pDC || !m_pGlyphsFont || !m_pGlyphsFont->m_hObject)
	{
		ASSERT(FALSE);
		return 0;
	}

	CFont* pPrevFnt = pDC->SelectObject(m_pGlyphsFont);
	pDC->SetTextColor(GetControlPart()->GetTextColor());
	TCHAR cszCh[2] = { ch, 0 };

	UINT uFlags = DT_VCENTER | DT_LEFT; // DT_CENTER;

	// CSize szChar = pDC->GetTextExtent(szCh, 1);
	CRect rcNeed = rc;
	pDC->DrawText(cszCh, 1, rcNeed, uFlags | DT_CALCRECT);
	rc.bottom = rc.top + rcNeed.Height();

	pDC->DrawText(cszCh, 1, rc, uFlags);

	pDC->SelectObject(pPrevFnt);

	// return szChar.cx;
	return rcNeed.Width();
}

int CXTPCalendarPaintManager::DrawIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent)
{
	if (!pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return 0;
	}

	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();

	BOOL bUseFont = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(), GetCalendarOptions(),
								  bUseOutlookFontGlyphs, FALSE);

	if (m_pGlyphsFont == NULL)
	{
		bUseFont = FALSE;
	}

	int nWidth = 0;

	CRect rcReminder(pViewEvent->GetReminderGlyphRect());
	if (pEvent->IsReminder() && !rcReminder.IsRectEmpty())
	{
		if (bUseFont)
		{
			nWidth += DrawIconChar(pDC, rcReminder, 0x41);
		}
		else
		{
			nWidth += DrawBitmap(&m_ilGlyphs, pDC, rcReminder, 0, DT_VCENTER);
		}
	}

	int nRecurrenceState = pEvent->GetRecurrenceState();
	CRect rcRecurrence(pViewEvent->GetRecurrenceGlyphRect());
	if (!rcRecurrence.IsRectEmpty())
	{
		if ((nRecurrenceState == xtpCalendarRecurrenceMaster)
			|| (nRecurrenceState == xtpCalendarRecurrenceOccurrence))
		{
			if (bUseFont)
			{
				nWidth += DrawIconChar(pDC, rcRecurrence, 0x43);
			}
			else
			{
				nWidth += DrawBitmap(&m_ilGlyphs, pDC, rcRecurrence, 1, DT_VCENTER);
			}
		}
		else if (nRecurrenceState == xtpCalendarRecurrenceException)
		{
			if (bUseFont)
			{
				nWidth += DrawIconChar(pDC, rcRecurrence, 0x44);
			}
			else
			{
				nWidth += DrawBitmap(&m_ilGlyphs, pDC, rcRecurrence, 2, DT_VCENTER);
			}
		}
	}

	BOOL bMeeting   = pEvent->IsMeeting();
	CRect rcMeeting = pViewEvent->GetMeetingGlyphRect();
	if (bMeeting && !rcMeeting.IsRectEmpty())
	{
		if (bUseFont)
		{
			nWidth += DrawIconChar(pDC, rcMeeting, 0x45);
		}
		else
		{
			nWidth += DrawBitmap(&m_ilGlyphs, pDC, rcMeeting, 3, DT_VCENTER);
		}
	}

	CRect rcPrivate(pViewEvent->GetPrivateGlyphRect());
	if (pEvent->IsPrivate() && !rcPrivate.IsRectEmpty())
	{
		if (bUseFont)
		{
			nWidth += DrawIconChar(pDC, rcPrivate, 0x42);
		}
		else
		{
			nWidth += DrawBitmap(&m_ilGlyphs, pDC, rcPrivate, 4, DT_VCENTER);
		}
	}

	return nWidth;
}

void CXTPCalendarPaintManager::InitBusyStatusDefaultColors()
{
	m_xtpBrushElsewhere.DeleteObject();
	m_bmpElsewherePattern.DeleteObject();

	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadBitmap(&m_bmpElsewherePattern,
										 XTP_IDB_CALENDAR_ELSEWHERE_PATTERN));
	VERIFY(m_xtpBrushElsewhere.CreatePatternBrush(&m_bmpElsewherePattern));

	m_xtpBrushTentative.DeleteObject();
	m_bmpTentativePattern.DeleteObject();

	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadBitmap(&m_bmpTentativePattern,
										 XTP_IDB_CALENDAR_TENTATIVE_PATTERN));
	VERIFY(m_xtpBrushTentative.CreatePatternBrush(&m_bmpTentativePattern));

	CXTPCalendarViewPartBrushValue* pBVal_Free =
		_GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusFree, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_Elsewhere =
		_GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusElsewhere, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_Tentative =
		_GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusTentative, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_Busy =
		_GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusBusy, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_OutOO =
		_GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusOutOfOffice, TRUE);

	if (pBVal_Free)
	{
		pBVal_Free->SetStandardValue(new CBrush(RGB(255, 255, 255)), TRUE);
	}
	if (pBVal_Elsewhere)
	{
		pBVal_Elsewhere->SetStandardValue(&m_xtpBrushElsewhere, FALSE);
	}
	if (pBVal_Tentative)
	{
		pBVal_Tentative->SetStandardValue(&m_xtpBrushTentative, FALSE);
	}
	if (pBVal_Busy)
	{
		pBVal_Busy->SetStandardValue(new CBrush(RGB(0, 0, 255)), TRUE);
	}
	if (pBVal_OutOO)
	{
		pBVal_OutOO->SetStandardValue(new CBrush(RGB(132, 0, 132)), TRUE);
	}
}

void CXTPCalendarPaintManager::SetBusyStatusBrush(int eBusyStatus, CBrush* pBrush,
												  BOOL bAutodeleteBrush)
{
	if (eBusyStatus < 0 || !pBrush)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarViewPartBrushValue* pBVal = _GetAtGrow_BusyStatusBrushValue(eBusyStatus, TRUE);
	ASSERT(pBVal);
	if (pBVal)
	{
		pBVal->SetCustomValue(pBrush, bAutodeleteBrush);
	}
}

CBrush* CXTPCalendarPaintManager::GetBusyStatusBrush(int nBusyStatus)
{
	CXTPCalendarViewPartBrushValue* pBVal = _GetAtGrow_BusyStatusBrushValue(nBusyStatus, TRUE);
	ASSERT(pBVal);
	if (pBVal)
	{
		CBrush* pBrush = pBVal->GetValue();
		return pBrush;
	}

	return NULL;
}

CXTPCalendarViewPartBrushValue*
	CXTPCalendarPaintManager::_GetAtGrow_BusyStatusBrushValue(int nBusyStatus, BOOL bGrow)
{
	if (nBusyStatus < 0)
	{
		// ASSERT(FALSE);
		nBusyStatus = 0;
	}

	int nCount = (int)m_arBusyStatusBrushes.GetSize();

	if (nBusyStatus >= nCount || m_arBusyStatusBrushes[nBusyStatus] == NULL)
	{
		if (!bGrow)
		{
			return NULL;
		}
		m_arBusyStatusBrushes.SetAtGrow(nBusyStatus, new CXTPCalendarViewPartBrushValue);
	}
	CXTPCalendarViewPartBrushValue* pBVal = m_arBusyStatusBrushes[nBusyStatus];
	return pBVal;
}

void CXTPCalendarPaintManager::DrawBusyStatus(CDC* pDC, CRect& rcRect, int eBusyStatus)
{
	CBrush* pBrush = GetBusyStatusBrush(eBusyStatus);
	if (!pBrush || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	pDC->FillRect(&rcRect, pBrush);
}

void CXTPCalendarPaintManager::DrawBusyStatus(CDC* pDC, CRgn& rgnBusy, int eBusyStatus)
{
	CBrush* pBrush = GetBusyStatusBrush(eBusyStatus);
	if (!pBrush || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	CBrush brBorder(m_clrWindowText);

	VERIFY(pDC->FillRgn(&rgnBusy, pBrush));
	VERIFY(pDC->FrameRgn(&rgnBusy, &brBorder, XTP_DPI_X(1), XTP_DPI_Y(1)));

	brBorder.DeleteObject();
}

void CXTPCalendarPaintManager::DrawBorders(CDC* pDC, const CRect& rcRect, const CRect& rcBorders)
{
	ASSERT(pDC);

	if (pDC->GetSafeHdc())
	{
		// Draw borders
		CRect rc;
		if (rcBorders.left)
		{
			rc		 = rcRect;
			rc.right = rc.left + rcBorders.left;
			pDC->FillSolidRect(rc, m_clrBorders[0]);
		}

		if (rcBorders.top)
		{
			rc		  = rcRect;
			rc.bottom = rc.top + rcBorders.top;
			pDC->FillSolidRect(rc, m_clrBorders[1]);
		}

		if (rcBorders.right)
		{
			rc		= rcRect;
			rc.left = rc.right - rcBorders.right;
			pDC->FillSolidRect(rc, m_clrBorders[2]);
		}

		if (rcBorders.bottom)
		{
			rc	 = rcRect;
			rc.top = rc.bottom - rcBorders.bottom;
			pDC->FillSolidRect(rc, m_clrBorders[3]);
		}
	}
}

void CXTPCalendarPaintManager::DrawExpandSign(CDC* pDC, BOOL bExpandUp, const CRect& rcTSHours)
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

		pDC->FillSolidRect(rcSign, ::GetSysColor(COLOR_INFOBK));
		pDC->Draw3dRect(rcSign, ::GetSysColor(COLOR_INFOTEXT), ::GetSysColor(COLOR_INFOTEXT));
		DrawBitmap(&m_ilExpandSigns, pDC, rcSign, iIcon);
	}
}

CXTPCalendarViewEventSubjectEditor*
	CXTPCalendarPaintManager::StartEditSubject(CXTPCalendarViewEvent* pViewEvent, CRect& rcEditor)
{
	UNREFERENCED_PARAMETER(pViewEvent);
	UNREFERENCED_PARAMETER(rcEditor);

	CXTPCalendarViewEventSubjectEditor* pEditor = new CXTPCalendarViewEventSubjectEditor(
		m_pControl);
	return pEditor;
}

void CXTPCalendarPaintManager::SetEditFont(CXTPCalendarViewEvent* pViewEvent,
										   CXTPCalendarViewEventSubjectEditor* pEditor)
{
	if (!pViewEvent)
	{
		ASSERT(FALSE);
		return;
	}

	CFont* pFont = pViewEvent->GetSubjectEditorFont();
	if (pFont)
	{
		pEditor->SetFont(pFont);
	}
}

void CXTPCalendarPaintManager::GetDateFormat(CString& strShort, CString& strLong)
{
	UNREFERENCED_PARAMETER(strShort);
	UNREFERENCED_PARAMETER(strLong);
}

//-- CALENDAR PART REFRESH METHODS

void CXTPCalendarPaintManager::PartRefresh(CControlPart* pViewPart)
{
	ASSERT(pViewPart);

	pViewPart->m_clrTextColor.SetStandardValue(m_clrButtonFaceText);
	pViewPart->m_clrBackground.SetStandardValue(m_clrButtonFace);

	LOGFONT lfIcon;
	VERIFY(GetIconLogFont(&lfIcon));
	pViewPart->m_fntText.SetStandardValue(&lfIcon);
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewEventPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 255));
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewTimeScaleCellPart* pViewPart)
{
	ASSERT(pViewPart);

	pViewPart->m_nHourWidth = pViewPart->m_nMinWidth = 0;
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	STRCPY_S(lfIcon.lfFaceName, LF_FACESIZE, _T("Tahoma"));
	pViewPart->m_fntTimeText.SetStandardValue(&lfIcon);

	// apply colors
	pViewPart->m_clrBorder = m_clr3DShadow;
	pViewPart->m_clrBackground.SetStandardValue(::GetSysColor(COLOR_3DFACE));
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewTimeScaleHeaderPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBorder = m_clr3DShadow;
	pViewPart->m_clrBackground.SetStandardValue(::GetSysColor(COLOR_3DFACE));
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewCellPart* pViewPart)
{
	ASSERT(pViewPart);
	UNREFERENCED_PARAMETER(pViewPart);
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewAllDayEventsPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(m_clr3DShadow);
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewHeaderPart* pViewPart)
{
	ASSERT(pViewPart);
	UNREFERENCED_PARAMETER(pViewPart);
}

void CXTPCalendarPaintManager::PartRefresh(CWeekViewPart* pViewPart)
{
	ASSERT(pViewPart);

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	pViewPart->m_fntText.SetStandardValue(&lfIcon);

	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 255));
	pViewPart->m_clrTextColor.SetStandardValue(m_clrButtonFaceText);
	pViewPart->m_clrTextHighLightColor = RGB(255, 255, 255);
	pViewPart->m_clrTextNormalColor	= m_clrButtonFaceText;
	pViewPart->m_clrHeaderBottomLine   = m_clr3DShadow;
}

void CXTPCalendarPaintManager::PartRefresh(CMonthViewHeaderPart* pViewPart)
{
	ASSERT(pViewPart);
	UNREFERENCED_PARAMETER(pViewPart);
}

void CXTPCalendarPaintManager::PartRefresh(CMonthViewEventPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrTextHighLightColor		= RGB(255, 255, 255);
	pViewPart->m_clrMultiDayEventFrameColor = RGB(0, 0, 0);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 213));
	pViewPart->m_grclrClassicSelDay.SetStandardValue(pViewPart->m_clrBackground,
													 RGB(217, 214, 202));
	pViewPart->m_clrClassicUnderline = RGB(10, 36, 106);
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewWorkCellPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 255));
	pViewPart->m_clrBackgroundSlave.SetStandardValue(RGB(255, 255, 255));
	pViewPart->m_clrShadow.SetStandardValue(RGB(128, 128, 128));
	pViewPart->m_clrHour.SetStandardValue(RGB(128, 128, 128));
}

void CXTPCalendarPaintManager::PartRefresh(CDayViewNonworkCellPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(212, 208, 200));
	pViewPart->m_clrBackgroundSlave.SetStandardValue(RGB(212, 208, 200));
	pViewPart->m_clrShadow.SetStandardValue(RGB(128, 128, 128));
	pViewPart->m_clrHour.SetStandardValue(RGB(128, 128, 128));
}

void CXTPCalendarPaintManager::PartRefresh(CMonthViewGridPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 255));
	pViewPart->m_clrBackground2.SetStandardValue(RGB(215, 211, 203));
}

void CXTPCalendarPaintManager::PartRefresh(CTimeLineViewPart* pViewPart)
{
	ASSERT(pViewPart);

	LPCTSTR pszBmpName = XTPDpiHelper()->SelectDpiSpecific(_T("XTP_IDB_CALENDAR_TLV_EVENT_100"),
														   _T("XTP_IDB_CALENDAR_TLV_EVENT_125"),
														   _T("XTP_IDB_CALENDAR_TLV_EVENT_150"),
														   _T("XTP_IDB_CALENDAR_TLV_EVENT_200"));

	HMODULE hModule = XTPResourceManager()->GetResourceHandle();
	HRSRC hRc		= NULL;

	if (hModule)
		hRc = ::FindResource(hModule, pszBmpName, _T("PNG"));

	if (!hRc)
		hModule = AfxFindResourceHandle(pszBmpName, _T("PNG"));

	if (hModule)
		VERIFY(pViewPart->m_pimgEvent->LoadFile(hModule, pszBmpName));

	pViewPart->m_clrEventBar.SetStandardValue(::GetSysColor(COLOR_ACTIVECAPTION));
	pViewPart->m_clrEventBarLine.SetStandardValue(RGB(255, 0, 0));
	pViewPart->m_clrEventBorderLine.SetStandardValue(::GetSysColor(COLOR_INACTIVEBORDER));

	pViewPart->m_clrSelectedText.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	pViewPart->m_clrSelectedBackground.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHT));

	pViewPart->m_clrBackground.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	pViewPart->m_clrText.SetStandardValue(::GetSysColor(COLOR_WINDOWTEXT));

	pViewPart->m_clrTimeScaleBackground.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	pViewPart->m_clrTimeScaleHightlite.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHT));
	pViewPart->m_clrTimeScaleText.SetStandardValue(::GetSysColor(COLOR_WINDOWTEXT));

	pViewPart->m_clrWeekendBackground.SetStandardValue(::GetSysColor(COLOR_BTNFACE));
}

//-- CALENDAR PART DRAWING METHODS

COLORREF CXTPCalendarPaintManager::GetEventBackColor(CXTPCalendarEvent* pEvent, COLORREF crDefault)
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
			if (pLabel)
			{
				return pLabel->m_clrColor;
			}
		}
	}

	return crDefault;
}

void CXTPCalendarPaintManager::PartDraw(CDayViewEventPart* pViewPart, CDC* pDC,
										CXTPCalendarDayViewEvent* pViewEvent)
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

		// fill event background
		CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
		COLORREF crBackground	 = GetEventBackColor(pEvent, pViewPart->GetBackgroundColor());

		BOOL bSelected = pViewEvent->IsSelected();
		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();

		BOOL bMultiDay		  = (nMDEFlags & xtpCalendarMultiDayFMLmask) != 0;
		DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(),
											  GetCalendarOptions(), dwAdditionalOptions, 0);

		BOOL bOutOfBorders = FALSE;

		// Draw borders
		CRect rcBorder(rcEvent);
		rcBorder.left += XTP_DPI_X(1);

		pDC->FillSolidRect(rcBorder, crBackground);

		pDC->FillSolidRect(rcBorder.left, rcBorder.top, rcBorder.Width(), XTP_DPI_Y(1),
						   m_clrWindowText);
		pDC->FillSolidRect(rcBorder.left - XTP_DPI_X(1),
						   rcBorder.bottom - XTP_DPI_Y(bMultiDay ? 1 : 0),
						   rcBorder.Width() + XTP_DPI_X(1), XTP_DPI_Y(1), m_clrWindowText);

		if (!bMultiDay)
		{
			pDC->FillSolidRect(rcBorder.right, rcBorder.top, XTP_DPI_X(1),
							   rcBorder.Height() + XTP_DPI_Y(1), m_clrWindowText);
		}

		if (bSelected && bMultiDay)
		{
			pDC->FillSolidRect(rcEvent.left, rcEvent.top + XTP_DPI_Y(1), rcEvent.Width(),
							   rcEvent.Height() - XTP_DPI_Y(2), m_clrButtonFace);
			pDC->FillSolidRect(rcEvent.left, rcEvent.top - XTP_DPI_Y(1),
							   rcEvent.Width() + XTP_DPI_X(1), XTP_DPI_Y(1), m_clrWindowText);
			pDC->FillSolidRect(rcEvent.left, rcEvent.bottom, rcEvent.Width() + XTP_DPI_X(1),
							   XTP_DPI_Y(1), m_clrWindowText);
		}

		// Draw markup if enabled
		BOOL bMarkupDrawn = FALSE;
		if (m_pControl->IsMarkupEnabled())
		{
			CXTPMarkupContext* pMarkupContext = m_pControl->GetMarkupContext();
			ASSERT(pMarkupContext);

			CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();
			if (NULL != pUIElement && NULL != pMarkupContext)
			{
				CRect rcMarkup = rcEvent;
				rcMarkup.DeflateRect(XTP_DPI_X(7), XTP_DPI_Y(7), XTP_DPI_X(7), XTP_DPI_Y(7));
				XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcMarkup);
				bMarkupDrawn = TRUE;
			}
		}

		// Draw non-markup contents
		if (!bMarkupDrawn)
		{
			BOOL bShowStartTime = 0 != (dwAdditionalOpt & xtpCalendarOptDayViewShowStartTimeAlways);
			BOOL bShowEndTime   = 0 != (dwAdditionalOpt & xtpCalendarOptDayViewShowEndTimeAlways);

			CString strLoc  = pViewEvent->GetItemTextLocation();
			CString strText = pViewEvent->GetItemTextSubject();
			CString strBody = pViewEvent->GetItemTextBody();

			if (strLoc.GetLength() > 0)
			{
				if (dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak)
					strText += _T("\n(") + strLoc + _T(")");
				else
					strText += _T(" (") + strLoc + _T(")");
			}

			if (strBody.GetLength() > 0)
			{
				strText += _T("\n") + strBody;
			}

			CString strTime;

			if (!bMultiDay)
			{
				COleDateTime dtStart = pEvent->GetStartTime();
				COleDateTime dtEnd   = pEvent->GetEndTime();

				DWORD dwTimeFlags = 0;
				if (bShowEndTime)
					dwTimeFlags = xtpCalendarItemText_EventStartTimeText;
				if (!CXTPCalendarUtils::IsEqual(dtStart, dtEnd) && bShowEndTime)
					dwTimeFlags = dwTimeFlags | xtpCalendarItemText_EventEndTimeText;

				strTime = pViewEvent->GetItemText(dwTimeFlags,
												  pViewEvent->IsHideEventTime() ? _T("") : NULL);
			}

			if (nMDEFlags & xtpCalendarMultiDayFirst)
			{
				CRect rcClockRect = pViewEvent->GetFirstClockRect();
				pDC->FillSolidRect(rcEvent.left, rcEvent.top, XTP_DPI_X(1), rcEvent.Height(),
								   m_clrWindowText);

				rcEvent.left++;
				if (rcClockRect.Width() > 0)
				{
					DrawClock(pDC, pEvent->GetStartTime(), rcClockRect, crBackground,
							  xtpCalendarClockAlignLeft);
				}
			}
			if (nMDEFlags & xtpCalendarMultiDayLast)
			{
				pDC->FillSolidRect(rcEvent.right, rcEvent.top, XTP_DPI_X(1), rcEvent.Height(),
								   m_clrWindowText);

				rcEvent.right -= XTP_DPI_X(2);
				if (pViewEvent->GetLastClockRect().Width() > 0)
				{
					DrawClock(pDC, pEvent->GetEndTime(), pViewEvent->GetLastClockRect(),
							  crBackground, xtpCalendarClockAlignLeft);
				}
			}

			UINT uAlign			 = 0;
			UINT uAlignWordBreak = 0;

			CRect rcText(pViewEvent->GetTextRect());
			CRect rcTime(rcText);
			BOOL bTimeOutOfBorders = FALSE;

			//  draw busy border
			if (!bMultiDay)
			{
				rcText.top += min(XTP_DPI_Y(2), rcText.Height());
				rcTime.top = rcText.top;

				int nBusyStatus = pEvent->GetBusyStatus();

				// fill busy area
				CRect rcBusyArea(rcEvent);
				//  draw border
				rcBusyArea.right = rcBusyArea.left + XTP_DPI_X(7);
				// rcText.left = rcBusyArea.right + XTP_DPI_X(3);

				pDC->FillSolidRect(rcBusyArea, m_clrWindowText);
				//  draw background
				rcBusyArea.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), 0);
				pDC->FillSolidRect(rcBusyArea, m_clrWindow);

				if (pViewEvent->GetTimeframeRect().top == LONG_MAX
					|| pViewEvent->GetTimeframeRect().bottom == LONG_MAX)
				{
					POINT arBusyPoints[] = { { rcEvent.left, rcEvent.top },
											 { rcEvent.left + XTP_DPI_X(7), rcEvent.top },
											 { rcEvent.left + XTP_DPI_X(7),
											   rcEvent.top + XTP_DPI_Y(7) },
											 { rcEvent.left, rcEvent.top } };
					const int cBPCount   = sizeof(arBusyPoints) / sizeof(arBusyPoints[0]);
					CRgn rgnBusy;
					BOOL bRgn = rgnBusy.CreatePolygonRgn(arBusyPoints, cBPCount, ALTERNATE);
					ASSERT(bRgn);

					if (bRgn)
						DrawBusyStatus(pDC, rgnBusy, nBusyStatus);

					rgnBusy.DeleteObject();
				}
				else
				{
					// draw busy border
					rcBusyArea.top	= pViewEvent->GetTimeframeRect().top;
					rcBusyArea.bottom = pViewEvent->GetTimeframeRect().bottom;

					// draw busy color
					pDC->FillSolidRect(rcBusyArea, m_clrWindowText);
					rcBusyArea.DeflateRect(0, XTP_DPI_Y(1), 0, XTP_DPI_Y(1));
					DrawBusyStatus(pDC, rcBusyArea, nBusyStatus);
				}

				// draw shadow
				if (!bSelected)
					DrawShadow(pDC, pViewEvent->GetEventRect());

				// draw border
				if (bSelected)
				{
					CBrush brushBorder(m_clrWindowText);

					CRect rcBorder2(pViewEvent->GetEventRect());
					rcBorder2.right += XTP_DPI_X(1);

					CRect rcBorderTop(rcBorder2);
					rcBorderTop.bottom = rcBorderTop.top + XTP_DPI_Y(1);
					rcBorderTop.top -= XTP_DPI_Y(5);

					DrawBusyStatus(pDC, rcBorderTop, nBusyStatus);
					pDC->FrameRect(&rcBorderTop, &brushBorder);

					CRect rcBorderBottom(rcBorder2);
					rcBorderBottom.top = rcBorderBottom.bottom;
					rcBorderBottom.bottom += XTP_DPI_Y(6);

					DrawBusyStatus(pDC, rcBorderBottom, nBusyStatus);
					pDC->FrameRect(&rcBorderBottom, &brushBorder);

					brushBorder.DeleteObject();
				}
				rcText.right -= XTP_DPI_X(4);

				uAlignWordBreak = XTPToUInt(dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak
												? DT_WORD_ELLIPSIS
												: DT_WORDBREAK);
				uAlign |= DT_LEFT | uAlignWordBreak | DT_EDITCONTROL;

				if (!strTime.IsEmpty())
				{
					CString strTest = strTime;
					if (dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak)
						strTest += _T(" W...");

					CSize szTime = pViewPart->GetTextExtent(pDC, strTest);
					if (szTime.cx <= rcText.Width())
					{
						if (bShowStartTime || bShowEndTime)
							strText = strTime + _T(" ") + strText;
						strTime.Empty();
					}
					else if (bShowStartTime || bShowEndTime)
					{
						bTimeOutOfBorders = TRUE;
						rcTime.bottom	 = rcTime.top + szTime.cy;
						rcText.top		  = rcTime.bottom;
					}
					else
					{
						strTime.Empty();
					}
				}
			}
			else
			{
				rcText.left += XTP_DPI_X(3);
				CSize szText = pViewPart->GetTextExtent(pDC, strText);
				if (pViewEvent->IsMultyDayTextAlign() || szText.cx + XTP_DPI_X(2) > rcText.Width())
				{
					uAlign |= DT_LEFT;
				}
				else
				{
					uAlign |= DT_CENTER;
				}
				uAlign |= DT_SINGLELINE;
			}

			DrawIcons(pDC, pViewEvent);

			// draw event time, if need
			if (!strTime.IsEmpty())
				pViewPart->DrawText(pDC, strTime, rcTime, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

			// draw event text
			pViewPart->DrawText(pDC, strText, rcText, uAlign | DT_VCENTER);

			// update OutOfBorders flag
			int nX_need = 0, nY_need = 0;
			if (uAlign & DT_SINGLELINE)
			{
				CSize szText = pViewPart->GetTextExtent(pDC, strText);
				nX_need		 = szText.cx;
			}
			else
			{
				CRect rcNeeded(0, 0, rcText.Width(), 0);
				pViewPart->DrawText(pDC, strText, rcNeeded, DT_CALCRECT | uAlignWordBreak);
				nY_need = rcNeeded.Height();
			}

			bOutOfBorders = nY_need > rcText.Height() || nX_need > rcText.Width()
							|| bTimeOutOfBorders;
		}

		pViewEvent->SetTextOutOfBorders(bOutOfBorders);
	}
}

void CXTPCalendarPaintManager::PartDrawHourCell(CDayViewTimeScaleCellPart* pViewPart, CDC* pDC,
												CRect rc, CString strText, BOOL bFillBackground,
												BOOL bCurrentTime)
{
	UNREFERENCED_PARAMETER(bCurrentTime);

	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		if (bFillBackground)
			pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());

		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.Height(),
						   pViewPart->m_clrBorder);

		COLORREF clrOldTextColor = pViewPart->GetTextColor();

		rc.DeflateRect(0, 0, XTP_DPI_X(4), 0);
		pViewPart->DrawText(pDC, strText, rc, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);

		rc.DeflateRect(XTP_DPI_X(5), 0, 0, 0);
		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_X(1),
						   pViewPart->m_clrBorder);

		// restore text color
		pViewPart->SetTextColor(clrOldTextColor);
	}
}

void CXTPCalendarPaintManager::PartDrawBigHourCell(CDayViewTimeScaleCellPart* pViewPart, CDC* pDC,
												   CRect rc, CString strHour, CString strMin,
												   int nRowPerHour, BOOL bFillBackground,
												   BOOL bCurrentTime)
{
	UNREFERENCED_PARAMETER(bCurrentTime);

	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		// Fill background
		if (bFillBackground)
			pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());

		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.Height(),
						   pViewPart->m_clrBorder);

		// Calculate coordinates
		CRect rcMinutesText(rc);
		rcMinutesText.bottom = rcMinutesText.top + rc.Height() / nRowPerHour;
		rcMinutesText.left += pViewPart->m_nHourWidth + XTP_DPI_X(10);

		CRect rcHoursText(rc);
		rcHoursText.right = rcMinutesText.left;
		rcHoursText.DeflateRect(0, XTP_DPI_Y(4), XTP_DPI_X(4), XTP_DPI_Y(7));

		COLORREF clrOldTextColor = pViewPart->GetTextColor();
		COLORREF clrTextColor	= pViewPart->GetTextColor();

		UINT dtHourTextFormat = DT_TOP | DT_RIGHT | DT_SINGLELINE | DT_NOCLIP;

		// Draw hours text
		CXTPFontDC fontBig(pDC, pViewPart->m_fntTimeText, clrTextColor);

		pDC->DrawText(strHour, rcHoursText, dtHourTextFormat);

		if (strMin.GetLength())
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
								   XTP_DPI_X(1), pViewPart->m_clrBorder);

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

		// restore text color
		pViewPart->SetTextColor(clrOldTextColor);

		// draw line below
		CRect rcLine = rc;
		rcLine.DeflateRect(XTP_DPI_X(5), 0, XTP_DPI_X(4), 0);

		pDC->FillSolidRect(rcLine.left, rcLine.bottom - XTP_DPI_Y(1), rcLine.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrBorder);
	}
}

void CXTPCalendarPaintManager::PartDraw(CDayViewTimeScaleHeaderPart* pViewPart, CDC* pDC,
										CXTPCalendarDayView* /*pView*/, CRect rc, CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());
		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.Height(),
						   pViewPart->m_clrBorder);

		rc.DeflateRect(XTP_DPI_X(1), 0, XTP_DPI_X(4), 0);
		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_X(1),
						   pViewPart->m_clrBorder);

		rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(2));
		pViewPart->DrawText(pDC, strText, rc, DT_BOTTOM | DT_LEFT | DT_SINGLELINE);
	}
}

void CXTPCalendarPaintManager::PartDrawNowLine(CDayViewTimeScaleHeaderPart* pViewPart, CDC* pDC,
											   CXTPCalendarDayView* pView, CRect rc, int y,
											   BOOL bDrawBk)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);

	if (pDC->GetSafeHdc() && pViewPart && pView)
	{
		int nHeight = CXTPCalendarUtils::GetTotalMinutes(pView->GetScaleInterval()) >= 60 ? 5 : 10;

		CRect rcNowLine  = rc;
		rcNowLine.top	= y - nHeight;
		rcNowLine.bottom = y + XTP_DPI_Y(1);
		rcNowLine.left += XTP_DPI_X(5);
		rcNowLine.right -= XTP_DPI_X(1);

		BOOL bSysTheme = GetCurrentSystemTheme() != xtpSystemThemeUnknown;

		if (bDrawBk)
		{
			if (bSysTheme)
				XTPDrawHelpers()->GradientFill(pDC, &rcNowLine, m_grclrToday, FALSE);
		}
		else
		{
			rcNowLine.top = rcNowLine.bottom - XTP_DPI_X(1);
			pDC->FillSolidRect(&rcNowLine, m_clrUnderLineHdr);
		}
	}
}

void CXTPCalendarPaintManager::PartDrawNowLine(CDayViewTimeScaleHeaderPart* pViewPart, CDC* pDC,
											   CXTPCalendarDayView* pView,
											   CXTPCalendarDayViewTimeScale* pScale, CRect rc,
											   int y, BOOL bDrawBk)
{
	UNREFERENCED_PARAMETER(pScale);

	PartDrawNowLine(pViewPart, pDC, pView, rc, y, bDrawBk);
}

void CXTPCalendarPaintManager::PartDraw(CDayViewCellPart* pViewPart, CDC* pDC,
										CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
										const XTP_CALENDAR_DAYVIEWCELL_PARAMS& cellParams)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewGroup);

	if (pDC->GetSafeHdc() && pViewPart && pViewGroup)
	{
		int iLeftMargin	= XTP_DPI_X(7);
		COLORREF clrBorder = m_clrWindowText;

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

		int nDayDeviderX = 0; // pViewGroup->GetGroupIndex() ? XTP_DPI_X(-2) : 0;

		pDC->FillSolidRect(rc.left + (iLeftMargin - XTP_DPI_X(1)) + nDayDeviderX, rc.top,
						   XTP_DPI_X(1), rc.Height(), clrBorder);

		rc.left += iLeftMargin + nDayDeviderX;

		if (cellParams.bSelected)
		{
			pDC->FillSolidRect(rc, cellParams.clrBackgroundColor);
		}
		else
		{
			WORD arBits[2] = { 0xAA, 0x55 };

			// Use the bit pattern to create a bitmap.
			CBitmap bm;
			bm.CreateBitmap(8, 2, 1, 1, arBits);

			CBrush brush;
			brush.CreatePatternBrush(&bm);

			COLORREF clrTxt0 = pDC->SetTextColor(cellParams.clrBackgroundColor);
			COLORREF clrBk0  = pDC->SetBkColor(pViewPart->m_clrBackgroundSlave);

			pDC->FillRect(rc, &brush);

			pDC->SetTextColor(clrTxt0);
			pDC->SetBkColor(clrBk0);

			brush.DeleteObject();
		}

		if (dtTime.GetMinute() == 0 && dtTime.GetHour() != 0)
			pDC->FillSolidRect(rc.left, rc.top - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   pViewPart->m_clrHour);

		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrShadow);
	}
}

void CXTPCalendarPaintManager::PartDraw(CDayViewAllDayEventsPart* pViewPart, CDC* pDC,
										CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
										BOOL bSelected)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewGroup);

	if (pDC->GetSafeHdc() && pViewPart && pViewGroup && pViewGroup->GetViewDay())
	{
		COLORREF clrBorder = RGB(0, 0, 0);

		if (bSelected)
		{
			pDC->FillSolidRect(rc, m_clrWindow);
			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_Y(1), m_clr3DShadow);
			pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   m_clr3DShadow);
		}
		else
		{
			pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());
		}

		if (pViewGroup->GetViewDay()->GetDayIndex() || pViewGroup->GetGroupIndex())
			pDC->FillSolidRect(rc.left, rc.top + XTP_DPI_Y(1), XTP_DPI_X(1),
							   rc.Height() - XTP_DPI_Y(2), clrBorder);
	}
}

void CXTPCalendarPaintManager::PartDraw(CDayViewHeaderPart* pViewPart, CDC* pDC,
										CXTPCalendarDayViewDay* pViewDay, CRect rc, CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewDay);

	if (pDC->GetSafeHdc() && pViewPart && pViewDay && pViewDay->GetView())
	{
		BOOL bIsToday = XTP_SAFE_GET1(pViewPart->GetCalendarControl(),
									  IsToday(pViewDay->GetDayDate()), FALSE);

		pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());

		CRect rcHeader(rc);

		DrawHeader(pDC, rcHeader, FALSE, bIsToday);

		BOOL bFirstDay = pViewDay->GetDayIndex() == 0;
		BOOL bLastDay  = pViewDay->GetDayIndex() == pViewDay->GetView()->GetViewDayCount() - 1;

		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_X(1),
						   pViewPart->m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.left, rc.top + XTP_DPI_Y(bFirstDay ? 0 : 2), XTP_DPI_X(1),
						   rc.Height() - XTP_DPI_Y(bFirstDay ? 0 : 3),
						   pViewPart->m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top + XTP_DPI_Y(bLastDay ? 0 : 2),
						   XTP_DPI_X(1), rc.Height() - XTP_DPI_Y(bLastDay ? 0 : 3), m_clr3DShadow);

		rc.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);

		if (pViewDay->GetCalendarControl()->m_bFullDateMode)
		{
			CString sFmt = pViewDay->GetView()->GetDayHeaderFormat();
			if (!sFmt.IsEmpty())
			{
				if (sFmt.Find(_T("MM")) > -1)
				{
					sFmt = _T("%a, %b %d");

					pViewDay->GetView()->SetDayHeaderFormat(sFmt);
					strText = pViewDay->GetDayDate().Format(sFmt);
				}
			}
		}

		pViewPart->DrawLine_CenterLR(pDC, strText, rc, DT_VCENTER);
	}
}

void CXTPCalendarPaintManager::PartDraw(CDayViewAllDayEventScrollIconsPart* pViewPart, CDC* pDC,
										CXTPCalendarDayViewGroup* pViewGroup, BOOL bDrawUp,
										BOOL bDrawDown, const CRect& rcIconUp,
										const CRect& rcIconDown, BOOL bHighlightUp,
										BOOL bHighlightDown)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart && pViewGroup)
	{
		COLORREF clrIconColor		  = RGB(0, 0, 0);
		COLORREF clrIconColorInactive = XTPDrawHelpers()->LightenColor(130, RGB(0, 0, 0));

		if (bDrawUp)
		{
			CPoint pt1(rcIconUp.left, rcIconUp.bottom),
				pt2(rcIconUp.left + rcIconUp.Width() / 2, rcIconUp.top),
				pt3(rcIconUp.right, rcIconUp.bottom);
			XTPDrawHelpers()->Triangle(pDC, pt1, pt2, pt3,
									   bHighlightUp ? clrIconColor : clrIconColorInactive);
		}

		if (bDrawDown)
		{
			CPoint pt1(rcIconDown.left, rcIconDown.top),
				pt2(rcIconDown.left + rcIconDown.Width() / 2, rcIconDown.bottom),
				pt3(rcIconDown.right, rcIconDown.top);
			XTPDrawHelpers()->Triangle(pDC, pt1, pt2, pt3,
									   bHighlightDown ? clrIconColor : clrIconColorInactive);
		}
	}
}

void CXTPCalendarPaintManager::PartDraw(CDayViewGroupHeaderPart* pViewPart, CDC* pDC,
										CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
										CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewGroup);

	if (pDC->GetSafeHdc() && pViewPart && pViewGroup && pViewGroup->GetViewDay())
	{
		pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());

		CRect rcHeader(rc);

		DrawHeader(pDC, rcHeader, FALSE, FALSE);

		CXTPCalendarDayViewDay* pViewDay = pViewGroup->GetViewDay();

		BOOL bFirstGroup = pViewDay->GetDayIndex() == 0 && pViewGroup->GetGroupIndex() == 0;
		BOOL bLastGroup  = pViewDay->GetDayIndex() == pViewDay->GetView()->GetViewDayCount() - 1
						  && pViewGroup->GetGroupIndex() == pViewDay->GetViewGroupsCount() - 1;
		;

		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_Y(1),
						   pViewPart->m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.left, rc.top + XTP_DPI_Y(bFirstGroup ? 0 : 2), XTP_DPI_X(1),
						   rc.Height() - XTP_DPI_Y(bFirstGroup ? 0 : 3),
						   pViewPart->m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top + XTP_DPI_Y(bLastGroup ? 0 : 2),
						   XTP_DPI_X(1), rc.Height() - XTP_DPI_Y(bLastGroup ? 0 : 3),
						   m_clr3DShadow);

		rc.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);

		// Colors for schedules
		if (pViewDay->GetCalendarControl()->m_bMultiColorScheduleMode)
		{
			int ind		 = pViewGroup->GetGroupIndex();
			COLORREF clr = pDC->GetTextColor();
			if (ind == 1)
				pViewPart->SetTextColor(RGB(255, 0, 0));
			else if (ind == 2)
				pViewPart->SetTextColor(RGB(0, 0, 255));
			else if (ind == 3)
				pViewPart->SetTextColor(RGB(0, 255, 255));
			else
				pViewPart->SetTextColor(RGB((GetRValue(clr) + 64 * ind) % 255,
											(GetGValue(clr) + 64 * ind) % 255,
											(GetBValue(clr) + 64 * ind) % 255));
		}

		pViewPart->DrawLine_CenterLR(pDC, strText, rc, DT_VCENTER);
	}
}

void CXTPCalendarPaintManager::PartDrawDayHeader(CWeekViewPart* pViewPart, CDC* pDC, CRect rcDay,
												 int nHeaderHeight, CString strHeader,
												 BOOL bIsCurrent, BOOL bIsSelected)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		bIsCurrent = FALSE;

		CRect rcHeader(rcDay);
		rcHeader.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(1),
							 rcHeader.Height() - nHeaderHeight - XTP_DPI_Y(1));
		CRect rcText(rcHeader);
		rcText.DeflateRect(XTP_DPI_X(1), 0, XTP_DPI_X(3), XTP_DPI_Y(1));

		CPen outerPen;
		outerPen.CreatePen(PS_SOLID, 1, pViewPart->m_clrHeaderBottomLine);
		CPen* pOldPen = pDC->SelectObject(&outerPen);

		// draw header top line
		DrawHorizontalLine(pDC, rcDay.left + XTP_DPI_X(1), rcDay.top, rcDay.Width() + XTP_DPI_X(1));

		pDC->SelectObject(pOldPen);
		outerPen.DeleteObject();

		// draw header contents
		CRect rcHeader2(rcHeader);
		rcHeader2.bottom -= XTP_DPI_Y(1);

		if (bIsCurrent && bIsSelected)
		{
			DrawHeader(pDC, rcHeader2, FALSE, bIsCurrent);
			CRect rcSel  = CRect(rcHeader2);
			CSize szText = pViewPart->GetTextExtent(pDC, strHeader);
			rcSel.left   = rcSel.right - (szText.cx + XTP_DPI_X(7));
			rcSel.bottom--;
			DrawHeader(pDC, rcSel, bIsSelected, FALSE);
		}
		else
		{
			DrawHeader(pDC, rcHeader2, bIsSelected, bIsCurrent);
		}

		// draw header bottom line
		if (!bIsCurrent)
		{
			CRect rcBottomLine(rcHeader);
			rcBottomLine.top = --rcBottomLine.bottom - XTP_DPI_Y(1);
			if (!bIsSelected)
				rcBottomLine.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);
			pDC->FillSolidRect(rcBottomLine, pViewPart->m_clrHeaderBottomLine);
		}

		// draw header text
		pDC->SetBkMode(TRANSPARENT);
		pViewPart->SetTextColor(bIsSelected ? pViewPart->m_clrTextHighLightColor
											: pViewPart->m_clrTextNormalColor);
		pViewPart->DrawText(pDC, strHeader, rcText, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
	}
}

void CXTPCalendarPaintManager::PartDraw(CWeekViewPart* pViewPart, CDC* pDC,
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
			CString strDate = pWDay->GetCaption();

			dtDay		= pWDay->GetDayDate();
			bIsSelected = pWDay->IsSelected();
			bIsCurrent  = CXTPCalendarUtils::IsEqual(dtDay, dtCurrent);

			//-------------------------------------------------------------

			pViewPart->DrawDayHeader(pDC, pWDay->GetDayRect(), nDayHeaderHeight, strDate,
									 bIsCurrent, bIsSelected);
		}
	}
}

void CXTPCalendarPaintManager::PartDrawBorder(CWeekViewEventPart* pViewPart, CDC* pDC, CRect rcView,
											  CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pWeekViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pWeekViewEvent && pWeekViewEvent->GetEvent())
	{
		int nBorderWidth = pWeekViewEvent->IsSelected() ? XTP_DPI_X(2) : XTP_DPI_X(1);

		int nMDEFlags = pWeekViewEvent->GetMultiDayEventFlags();
		BOOL bAllDay  = pWeekViewEvent->GetEvent()->IsAllDayEvent();

		if (pWeekViewEvent->IsSelected()
			&& (nMDEFlags & xtpCalendarMultiDayFirst || nMDEFlags & xtpCalendarMultiDayLast
				|| nMDEFlags & xtpCalendarMultiDayMiddle))
		{
			rcView.top--;
			rcView.bottom++;
		}

		if (bAllDay || nMDEFlags & xtpCalendarMultiDayFirst || nMDEFlags & xtpCalendarMultiDayLast
			|| nMDEFlags & xtpCalendarMultiDayMiddle)
		{
			pDC->FillSolidRect(rcView.left, rcView.top, rcView.Width(), nBorderWidth,
							   pViewPart->m_clrTextNormalColor);
			pDC->FillSolidRect(rcView.left, rcView.bottom - nBorderWidth, rcView.Width(),
							   nBorderWidth, pViewPart->m_clrTextNormalColor);
		}
		if (nMDEFlags & xtpCalendarMultiDayFirst)
		{
			pDC->FillSolidRect(rcView.left, rcView.top, XTP_DPI_X(1), rcView.Height(),
							   pViewPart->m_clrTextNormalColor);
		}
		if (nMDEFlags & xtpCalendarMultiDayLast)
		{
			pDC->FillSolidRect(rcView.right, rcView.top, XTP_DPI_X(1), rcView.Height(),
							   pViewPart->m_clrTextNormalColor);
		}
	}
}

int CXTPCalendarPaintManager::PartDrawTimes(CWeekViewEventPart* pViewPart, CDC* pDC,
											CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pWeekViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pWeekViewEvent && pWeekViewEvent->GetEvent())
	{
		DWORD dwAdditionalOpt = XTP_SAFE_GET3(pWeekViewEvent, GetCalendarControl(),
											  GetCalendarOptions(), dwAdditionalOptions, 0);
		BOOL bShowStartTime   = 0 != (dwAdditionalOpt & xtpCalendarOptWeekViewShowStartTimeAlways);
		BOOL bShowEndTime	 = 0 != (dwAdditionalOpt & xtpCalendarOptWeekViewShowEndTimeAlways);

		COLORREF clrBackground = pWeekViewEvent->IsSelected() ? m_clrHighlight
															  : pViewPart->m_clrBackground;
		COLORREF clrLabel = GetEventBackColor(pWeekViewEvent->GetEvent(), clrBackground);
		clrBackground	 = clrLabel;

		CRect rcStart(pWeekViewEvent->GetStartTimeRect());
		CRect rcEnd(pWeekViewEvent->GetEndTimeRect());
		COleDateTime dtStart(pWeekViewEvent->GetEvent()->GetStartTime());

		if (pWeekViewEvent->IsTimeAsClock())
		{
			if (bShowStartTime)
				DrawClock(pDC, dtStart, rcStart, clrBackground, xtpCalendarClockAlignCenter);
		}
		else
		{
			CString strStartTime = pWeekViewEvent->GetItemTextStartTime();
			if (bShowStartTime)
				pViewPart->DrawLineEx(pDC, strStartTime, &rcStart, DT_RIGHT | DT_VCENTER,
									  DT_LEFT | DT_VCENTER);
		}

		if (pWeekViewEvent->IsShowEndTime())
		{
			COleDateTime dtEnd(pWeekViewEvent->GetEvent()->GetEndTime());
			if (pWeekViewEvent->IsTimeAsClock())
			{
				if (bShowEndTime)
					DrawClock(pDC, dtEnd, rcEnd, clrBackground, xtpCalendarClockAlignCenter);
			}
			else
			{
				CString strEndTime = pWeekViewEvent->GetItemTextEndTime();
				if (bShowEndTime)
					pViewPart->DrawLineEx(pDC, strEndTime, &rcEnd, DT_RIGHT | DT_VCENTER,
										  DT_LEFT | DT_VCENTER);
			}
		}
	}

	return 0;
}

void CXTPCalendarPaintManager::PartDrawSubj(CWeekViewEventPart* pViewPart, CDC* pDC,
											CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pWeekViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pWeekViewEvent && pWeekViewEvent->GetEvent())
	{
		CRect rcText(pWeekViewEvent->GetTextRect());
		CString strLoc		 = pWeekViewEvent->GetItemTextLocation();
		CString strEventText = pWeekViewEvent->GetItemTextSubject();
		if (strLoc.GetLength() > 0)
		{
			strEventText += _T(" (") + strLoc + _T(")");
		}

		int nDayEventFlags = pWeekViewEvent->GetMultiDayEventFlags();
		UINT nAlign		   = DT_CENTER;
		if (pWeekViewEvent->IsMultyDayTextAlign()
			|| pViewPart->GetTextExtent(pDC, strEventText).cx >= rcText.Width())
			nAlign = DT_LEFT;

		if (pWeekViewEvent->GetEvent()->IsAllDayEvent() || nDayEventFlags & xtpCalendarMultiDayFirst
			|| nDayEventFlags & xtpCalendarMultiDayMiddle
			|| nDayEventFlags & xtpCalendarMultiDayLast)
		{
			pViewPart->DrawText(pDC, strEventText, &rcText, nAlign | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			pViewPart->DrawText(pDC, strEventText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void CXTPCalendarPaintManager::PartDraw(CWeekViewEventPart* pViewPart, CDC* pDC,
										CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pWeekViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pWeekViewEvent && pWeekViewEvent->GetEvent())
	{
		CRect rcView(pWeekViewEvent->GetViewEventRect());
		CRect rcViewMax(rcView);
		CString strEventText;

		BOOL bSelected	 = pWeekViewEvent->IsSelected();
		int nDayEventFlags = pWeekViewEvent->GetMultiDayEventFlags();
		BOOL bMday		   = (nDayEventFlags & xtpCalendarMultiDayFMLmask) != 0;
		BOOL bAllDay	   = pWeekViewEvent->GetEvent()->IsAllDayEvent();

		CXTPCalendarEventLabel* ptrLabel = pWeekViewEvent->GetEvent()->GetLabel();
		BOOL bLabel = (ptrLabel && ptrLabel->m_nLabelID != XTP_CALENDAR_NONE_LABEL_ID);

		COLORREF clrBackground = pViewPart->m_clrBackground;
		pViewPart->SetTextColor(pViewPart->m_clrTextNormalColor);
		if (bMday || bAllDay)
			clrBackground = RGB(255, 255, 255);
		if (bLabel && ptrLabel)
			clrBackground = GetEventBackColor(pWeekViewEvent->GetEvent(), clrBackground);
		if (bSelected)
		{
			rcViewMax.CopyRect(pWeekViewEvent->GetViewEventRectMax());
			clrBackground = m_clrHighlight;
			pViewPart->SetTextColor(pViewPart->m_clrTextHighLightColor);
		}

		if (bMday || bAllDay)
			pDC->FillSolidRect(rcView, clrBackground);
		else
			pDC->FillSolidRect(rcViewMax, clrBackground);

		// Draw border for long event
		pViewPart->DrawBorder(pDC, rcView, pWeekViewEvent);

		// Draw Clock for Long Events
		if (nDayEventFlags & xtpCalendarMultiDayFirst)
		{
			DrawClock(pDC, pWeekViewEvent->GetEvent()->GetStartTime(),
					  pWeekViewEvent->GetFirstClockRect(), clrBackground,
					  xtpCalendarClockAlignLeft);
		}
		else if (nDayEventFlags & xtpCalendarMultiDayLast)
		{
			DrawClock(pDC, pWeekViewEvent->GetEvent()->GetEndTime(),
					  pWeekViewEvent->GetLastClockRect(), clrBackground,
					  xtpCalendarClockAlignRight);
		}
		// 3 Draw time start/end and icons
		clrBackground = bSelected ? m_clrHighlight : pViewPart->m_clrBackground;
		if (!bMday && !bAllDay)
			pViewPart->DrawTimes(pDC, pWeekViewEvent);

		DrawIcons(pDC, pWeekViewEvent);

		// 5 Draw Text
		pViewPart->DrawSubj(pDC, pWeekViewEvent);
	}
}

void CXTPCalendarPaintManager::PartDrawGrid(CMonthViewGridPart* pViewPart, CDC* pDC,
											CXTPCalendarMonthView* pMonthView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pMonthView);

	if (pDC->GetSafeHdc() && pViewPart && pMonthView && pMonthView->GetGrid()
		&& pMonthView->GetCalendarControl())
	{
		COLORREF clrGridLine = RGB(0, 0, 0);
		BOOL bCompressWeD	= pMonthView->GetCalendarControl()->MonthView_IsCompressWeekendDays();
		int nWeeksCount		 = pMonthView->GetGrid()->GetWeeksCount();

		COleDateTime dtNow		= CXTPCalendarUtils::GetCurrentTime();
		int nCurrentMonth		= dtNow.GetMonth();
		int nWhiteMonthColorIdx = nCurrentMonth % 2;

		for (int nWeek = 0; nWeek < nWeeksCount; nWeek++)
		{
			for (int nDayIdx = 0; nDayIdx < 7; nDayIdx++)
			{
				COleDateTime dtDay = pMonthView->GetGrid()->GetDayDate(nWeek, nDayIdx);
				CRect rcB		   = pMonthView->GetGrid()->GetDayRect(nWeek, nDayIdx, TRUE);

				// Draw Grid lines
				if (nWeek > 0)
				{
					pDC->FillSolidRect(rcB.left, rcB.top, rcB.Width(), XTP_DPI_X(1), clrGridLine);
				}
				if (nWeek < nWeeksCount - 1)
				{
					pDC->FillSolidRect(rcB.left, rcB.bottom, rcB.Width(), XTP_DPI_X(1),
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

				int nMonth			   = dtDay.GetMonth();
				COLORREF clrBackground = nMonth % 2 == nWhiteMonthColorIdx
											 ? pViewPart->m_clrBackground
											 : pViewPart->m_clrBackground2;
				if (nDayIdx == 0)
				{
					rcDay.left--;
				}

				// Show Discrete Selection in non Office 2007 mode

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

void CXTPCalendarPaintManager::PartDraw(CMonthViewHeaderPart* pViewPart, CDC* pDC,
										CXTPCalendarMonthView* pMonthView, CRect rc, int nCollIndex,
										CString strText)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pMonthView);

	if (pDC->GetSafeHdc() && pViewPart && pMonthView && pMonthView->GetGrid())
	{
		pDC->FillSolidRect(rc, pViewPart->GetBackgroundColor());

		BOOL bFirstDay = nCollIndex == 0;
		BOOL bLastDay  = nCollIndex == pMonthView->GetGrid()->GetColsCount() - 1;

		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_X(1),
						   pViewPart->m_clrTopLeftBorder);

		pDC->FillSolidRect(rc.left, rc.top + XTP_DPI_Y(bFirstDay ? 0 : 2), XTP_DPI_X(1),
						   rc.Height() - XTP_DPI_Y(bFirstDay ? 1 : 3),
						   pViewPart->m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top + XTP_DPI_Y(bLastDay ? 0 : 2),
						   XTP_DPI_X(1), rc.Height() - XTP_DPI_Y(bLastDay ? 1 : 3), m_clr3DShadow);

		pDC->FillSolidRect(rc.left, rc.bottom, rc.Width(), XTP_DPI_X(1), m_clr3DShadow);

		rc.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);

		pViewPart->DrawLine_CenterLR(pDC, strText, rc, DT_VCENTER);
	}
}

void CXTPCalendarPaintManager::PartDrawDayDate(CMonthViewEventPart* pViewPart, CDC* pDC, CRect rc,
											   BOOL bToday, BOOL bSelected, BOOL bFirstMonthDay,
											   CString strText)
{
	UNREFERENCED_PARAMETER(bToday);
	UNREFERENCED_PARAMETER(bFirstMonthDay);

	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

		if (bSelected)
		{
			DrawHeader(pDC, rc, bSelected, FALSE, pViewPart->m_grclrClassicSelDay,
					   pViewPart->m_clrClassicUnderline);
		}

		UINT dtTextFormat = DT_VCENTER | DT_SINGLELINE;

		dtTextFormat |= DT_RIGHT;
		rc.DeflateRect(0, 0, XTP_DPI_X(5), 0);

		COLORREF clrOldTextColor = pDC->GetTextColor();

		if (bSelected)
		{
			pDC->SetTextColor(pViewPart->m_clrTextHighLightColor);
		}

		CXTPFontDC fnt(pDC, &pViewPart->GetTextFont());
		pDC->DrawText(strText, rc, dtTextFormat);
		pViewPart->m_clrTextColor.SetDefaultValue();
		pDC->SetTextColor(clrOldTextColor);
	}
}

void CXTPCalendarPaintManager::PartDrawEvent(CMonthViewEventPart* pViewPart, CDC* pDC,
											 CXTPCalendarMonthViewEvent* pViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent && pViewEvent->GetEvent())
	{
		// Start - End options
		DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(),
											  GetCalendarOptions(), dwAdditionalOptions, 0);
		BOOL bShowEndTime	 = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(),
										  MonthView_IsShowEndDate(), TRUE);
		BOOL bHideTimes		= XTP_SAFE_GET2(m_pControl, GetCalendarOptions(), bMonthView_HideTimes,
										FALSE);
		BOOL bDrawStartTime = (dwAdditionalOpt & xtpCalendarOptMonthViewShowStartTimeAlways)
							  && !bHideTimes;
		BOOL bDrawEndTime = (dwAdditionalOpt & xtpCalendarOptMonthViewShowEndTimeAlways)
							&& !bHideTimes;

		CRect rcEvent				= pViewEvent->GetEventRect();
		CRect rcText				= pViewEvent->GetTextRect();
		CXTPCalendarEvent* prtEvent = pViewEvent->GetEvent();

		BOOL bSelected = pViewEvent->IsSelected();
		int nMDEFlags  = pViewEvent->GetMultiDayEventFlags();

		CRect rcClockFirst(0, 0, 0, 0);
		CRect rcClockLast(0, 0, 0, 0);
		COleDateTime dtClockFirst;
		COleDateTime dtClockLast;
		COLORREF clrBackground;
		COLORREF clrForeground = pViewPart->GetTextColor();
		if (nMDEFlags & xtpCalendarMultiDayFirst)
		{
			rcClockFirst.CopyRect(pViewEvent->GetFirstClockRect());
			dtClockFirst = pViewEvent->GetEvent()->GetStartTime();
		}
		if (nMDEFlags & xtpCalendarMultiDayLast)
		{
			rcClockLast.CopyRect(pViewEvent->GetLastClockRect());
			dtClockLast = pViewEvent->GetEvent()->GetEndTime();
		}

		if (bSelected)
		{
			if (nMDEFlags & xtpCalendarMultiDayFMLmask)
			{
				rcEvent.top--;
				rcEvent.bottom++;
			}
			else
			{
				rcEvent.left  = pViewEvent->m_rcEventMax.left + XTP_DPI_X(2);
				rcEvent.right = pViewEvent->m_rcEventMax.right - XTP_DPI_X(2);
			}
			clrForeground = pViewPart->m_clrTextHighLightColor;
			clrBackground = m_clrHighlight;
			pDC->FillSolidRect(&rcEvent, m_clrHighlight);
			clrBackground = m_clrHighlight;
		}
		else
		{
			CXTPCalendarEventLabel* ptrLabel = pViewEvent->GetEvent()->GetLabel();
			COLORREF clrLabel = GetEventBackColor(pViewEvent->GetEvent(), RGB(255, 255, 255));

			BOOL bNoneLabel = ptrLabel ? (ptrLabel->m_nLabelID == XTP_CALENDAR_NONE_LABEL_ID)
									   : TRUE;
			clrBackground = clrLabel;

			if (nMDEFlags & xtpCalendarMultiDayFMLmask || !bNoneLabel)
			{
				pDC->FillSolidRect(&rcEvent, clrLabel);
			}
		}

		UINT uAlign = DT_LEFT;

		CString strLoc  = pViewEvent->GetItemTextLocation();
		CString strText = pViewEvent->GetItemTextSubject();
		if (strLoc.GetLength() > 0)
			// strText += _T(" (") + strLoc + _T(")");
			strText += _T(";"); // to have same separator in different themes

		if (nMDEFlags & xtpCalendarMultiDayFMLmask)
		{
			int nBY = XTP_DPI_X(bSelected ? 2 : 1);
			pDC->FillSolidRect(rcEvent.left, rcEvent.top, rcEvent.Width(), nBY,
							   pViewPart->m_clrMultiDayEventFrameColor);
			pDC->FillSolidRect(rcEvent.left, rcEvent.bottom - nBY, rcEvent.Width(), nBY,
							   pViewPart->m_clrMultiDayEventFrameColor);

			if (nMDEFlags & xtpCalendarMultiDayFirst)
			{
				pDC->FillSolidRect(rcEvent.left, rcEvent.top, XTP_DPI_X(1), rcEvent.Height(),
								   pViewPart->m_clrMultiDayEventFrameColor);
			}
			if (nMDEFlags & xtpCalendarMultiDayLast)
			{
				pDC->FillSolidRect(rcEvent.right, rcEvent.top, XTP_DPI_X(1), rcEvent.Height(),
								   pViewPart->m_clrMultiDayEventFrameColor);
			}

			// CSize szSpace = GetTextExtent(pDC, _T(" "));

			rcText.left += XTP_DPI_X(1); // min(1, szSpace.cx/2);
			rcText.right -= XTP_DPI_X(1);
			CSize szText = pViewPart->GetTextExtent(pDC, strText);

			if (pViewEvent->IsMultyDayTextAlign() || szText.cx > rcText.Width())
				uAlign = DT_LEFT;
			else
				uAlign = DT_CENTER;
		}

		// draw clocks for multyday events
		if ((nMDEFlags & xtpCalendarMultiDayFirst) || (nMDEFlags & xtpCalendarMultiDayLast))
		{
			if (rcClockFirst.Width() > 0)
				DrawClock(pDC, dtClockFirst, rcClockFirst, clrBackground,
						  xtpCalendarClockAlignLeft);

			if (rcClockLast.Width() > 0)
				DrawClock(pDC, dtClockLast, rcClockLast, clrBackground, xtpCalendarClockAlignLeft);
		}

		// CRect rcText = pViewEvent->GetTextRect();
		if (nMDEFlags & xtpCalendarMultiDayNoMultiDay)
		{
			// IsTimeAsClock()
			COleDateTime dtStart(prtEvent->GetStartTime());
			COleDateTime dtEnd(prtEvent->GetEndTime());

			CString strStartTime = pViewEvent->GetItemTextStartTime();
			CString strEndTime   = pViewEvent->GetItemTextEndTime();

			CRect rcStart(pViewEvent->GetStartTimeRect());
			CRect rcEnd(pViewEvent->GetEndTimeRect());

			if (pViewEvent->IsTimeAsClock())
			{
				if (bDrawStartTime)
					DrawClock(pDC, dtStart, rcStart, clrBackground, xtpCalendarClockAlignCenter);
				else
					rcEnd.left -= rcStart.Width();
				if (bDrawEndTime && bShowEndTime)
					DrawClock(pDC, dtEnd, rcEnd, clrBackground, xtpCalendarClockAlignCenter);
				else
					rcText.left -= rcEnd.Width();
			}
			else
			{
				if (bDrawStartTime)
					pViewPart->DrawLineEx(pDC, strStartTime, rcStart, DT_RIGHT | DT_VCENTER,
										  DT_LEFT | DT_VCENTER);
				else
					rcEnd.left -= rcStart.Width();
				if (bDrawEndTime && bShowEndTime)
					pViewPart->DrawLineEx(pDC, strEndTime, rcEnd, DT_RIGHT | DT_VCENTER,
										  DT_LEFT | DT_VCENTER);
				else
					rcText.left -= rcEnd.Width();
			}
		}

		rcText.left += pViewEvent->CalcIconsRect(rcText);

		COLORREF clrOldTextColor = pDC->SetTextColor(clrForeground);
		pViewPart->DrawText(pDC, strText, rcText,
							uAlign | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		pDC->SetTextColor(clrOldTextColor);

		DrawIcons(pDC, pViewEvent);

		pViewPart->m_clrTextColor.SetDefaultValue();
	}
}

void CXTPCalendarPaintManager::PartDraw(CTimeLineViewTimeScalePart_Day* pViewPart, CDC* pDC,
										const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);

	if (pDC->GetSafeHdc() && pViewPart && pView && pView->GetTimeScale())
	{
		pDC->SetBkMode(TRANSPARENT);

		CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
		pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

		CRect rcBorder = rcRect;
		rcBorder.top   = rcBorder.bottom - XTP_DPI_Y(1);
		pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

		CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
		if (!pTSParams)
		{
			ASSERT(FALSE);
			return;
		}

		COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
		int nSInterval_px			 = pTSParams->GetScaleIntervalWidth_px();

		COleDateTime dtStart = pTimeScale->GetStartViewDate();
		COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

		double dFirstStroke_Shift = (double)(dtBegin - dtStart);
		double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval /
																	 // nSInterval_px);

		int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

		CXTPFontDC autoFont(pDC, pViewPart->m_fntScaleHeaderText, pViewPart->GetTextColor());

		pViewPart->DrawHeader(pDC, rcRect, pView, xtpDTScale_Day);

		CRect rcHeader  = rcRect;
		rcHeader.bottom = rcHeader.top + pViewPart->m_nHeaderHeight;

		pViewPart->TBase::Draw(pDC, rcRect, pView);

		CString strLDFormat = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE);

		COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dtStart);
		COleDateTimeSpan spDay(1, 0, 0, 0);
		for (int nP = 0; nP < 100; nP++, dtDay += spDay)
		{
			double dDay_tshift = double(dtDay - dtStart);
			int nDay_xshift	= int(dDay_tshift / dTimePerPixel);

			if (nDay_xshift > rcHeader.Width())
				break;

			double dNextDay_tshift = double(dtDay + spDay - dtStart);
			int nNextDay_xshift	= int(dNextDay_tshift / dTimePerPixel);

			CRect rcDayLabel = rcHeader;
			rcDayLabel.left += 5;
			rcDayLabel.right = rcHeader.left + nNextDay_xshift - XTP_DPI_X(15);

			if (nDay_xshift > 0)
				rcDayLabel.left += nDay_xshift;

			CString strLabel = CXTPCalendarUtils::GetDateFormat(dtDay, strLDFormat);
			CSize szLabel	= pDC->GetTextExtent(strLabel);

			UINT nFormat = DT_VCENTER | DT_SINGLELINE;

			CPoint ptTR(rcDayLabel.left + szLabel.cx + XTP_DPI_X(4),
						rcDayLabel.bottom - XTP_DPI_Y(9));

			if (szLabel.cx >= rcDayLabel.Width())
			{
				nFormat |= DT_RIGHT;
				rcDayLabel.left = rcHeader.left;
				ptTR.x			= rcDayLabel.right;
			}

			pViewPart->DrawText(pDC, strLabel, &rcDayLabel, nFormat, pView->m_clrTimeScaleText);

			POINT arTRPts[] = { { ptTR.x, ptTR.y },
								{ ptTR.x + XTP_DPI_X(9), ptTR.y },
								{ ptTR.x + XTP_DPI_X(4), ptTR.y + XTP_DPI_Y(5) } };
			CRgn rgnTR;
			BOOL bRgn = rgnTR.CreatePolygonRgn(arTRPts, _countof(arTRPts), ALTERNATE);
			ASSERT(bRgn);

			if (bRgn)
			{
				CBrush brTR(pView->m_clrTimeScaleText);
				VERIFY(pDC->FillRgn(&rgnTR, &brTR));
				brTR.DeleteObject();
				rgnTR.DeleteObject();
			}
		}
		autoFont.SetFont(&pViewPart->GetTextFont());

		CString strHour;
		int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + 1;
		for (int i = 0; i < nCount; i++)
		{
			COleDateTimeSpan spFirstStroke((double)spSInterval * i);
			COleDateTime dtStroke = dtBegin + spFirstStroke;
			BOOL bBold			  = dtStroke.GetMinute() == 0;
			CRect rcStroke;

			rcStroke.left  = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
			rcStroke.right = rcStroke.left + XTP_DPI_X(bBold ? 2 : 1);

			rcStroke.bottom = rcRect.bottom;
			rcStroke.top	= rcStroke.bottom - XTP_DPI_Y(bBold ? 10 : 8);

			pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

			if (bBold)
			{
				CRect rcLabel = rcRect;

				if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(),
								  bTimeLineFocused, FALSE))
				{
					rcLabel.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(1);
					rcLabel.left  = rcStroke.left + XTP_DPI_X(2);
					rcLabel.right = rcLabel.left + nSInterval_px - XTP_DPI_X(3);
					rcLabel.bottom -= XTP_DPI_Y(2);
				}
				else
				{
					rcLabel.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(3);
					rcLabel.left   = rcStroke.left + XTP_DPI_X(5);
					rcLabel.right  = rcLabel.left + nSInterval_px - XTP_DPI_X(8);
					rcLabel.bottom = rcStroke.top + XTP_DPI_Y(6);
				}

				strHour.Format(_T("%d"), dtStroke.GetHour());

				COleDateTime dtNow = COleDateTime::GetCurrentTime();

				BOOL bSelected = dtStroke.GetYear() == dtNow.GetYear()
								 && dtStroke.GetMonth() == dtNow.GetMonth()
								 && dtStroke.GetDay() == dtNow.GetDay()
								 && dtStroke.GetHour() == dtNow.GetHour();

				CXTPDCSaveState dcState(pDC);

				if (bSelected)
				{
					pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);
					pViewPart->DrawText(pDC, strHour, &rcLabel,
										DT_CENTER | DT_SINGLELINE | DT_VCENTER,
										pView->m_clrSelectedText, m_pSelFont);
				}
				else
				{
					pViewPart->DrawText(pDC, strHour, &rcLabel,
										DT_CENTER | DT_SINGLELINE | DT_VCENTER,
										pView->m_clrTimeScaleText);
				}
			}
		}
	}
}

void CXTPCalendarPaintManager::PartDraw(CTimeLineViewTimeScalePart_Week* pViewPart, CDC* pDC,
										const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);

	if (pDC->GetSafeHdc() && pViewPart && pView && pView->GetTimeScale())
	{
		CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
		pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

		pViewPart->DrawHeader(pDC, rcRect, pView, xtpDTScale_Month);

		CRect rcBorder = rcRect;
		rcBorder.top   = rcBorder.bottom - XTP_DPI_Y(1);
		pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

		CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
		if (!pTSParams)
		{
			ASSERT(FALSE);
			return;
		}

		COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
		int nSInterval_px			 = pTSParams->GetScaleIntervalWidth_px();

		COleDateTime dtStart = pTimeScale->GetStartViewDate();
		COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

		// COleDateTimeSpan spFirstStroke_Shift = dtBegin - dtStart;
		double dFirstStroke_Shift = (double)(dtBegin - dtStart);
		double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval /
																	 // nSInterval_px);

		int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

		CString strLabel;
		int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + XTP_DPI_X(1);
		for (int i = 0; i < nCount; i++)
		{
			COleDateTimeSpan spFirstStroke((double)spSInterval * i);
			COleDateTime dtStroke = dtBegin + spFirstStroke;

			CRect rcStroke;
			rcStroke.left  = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
			rcStroke.right = rcStroke.left + XTP_DPI_X(1);

			rcStroke.bottom = rcRect.bottom;
			rcStroke.top	= rcStroke.bottom - XTP_DPI_Y(8);

			pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

			CRect rcBack = rcRect;
			if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(), bTimeLineFocused,
							  FALSE))
			{
				rcBack.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(1);
				rcBack.left  = rcStroke.left + XTP_DPI_X(2);
				rcBack.right = rcBack.left + nSInterval_px - XTP_DPI_X(3);
				rcBack.bottom -= XTP_DPI_Y(2);
			}
			else
			{
				rcBack.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(3);
				rcBack.left   = rcStroke.left + XTP_DPI_X(4);
				rcBack.right  = rcBack.left + nSInterval_px - XTP_DPI_X(7);
				rcBack.bottom = rcStroke.top + XTP_DPI_Y(4);
			}

			int nWD = dtStroke.GetDayOfWeek() - 1;
			if (nWD == 0)
				nWD = 7;

			CString strWD = CXTPCalendarUtils::GetLocaleString(
				XTPToUInt(CXTPCalendarUtils::GetDayOfWeekLocaleStringId(nWD)));
			strLabel.Format(_T("%s %d"), (LPCTSTR)strWD, dtStroke.GetDay());

			pDC->SetBkColor(pView->m_clrTimeScaleBackground);

			if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(), bTimeLineFocused,
							  FALSE))
			{
				int nWorkWeekMask = XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(),
												  nWorkWeekMask, 0);
				BOOL bWorkDay	 = FALSE;
				switch (dtStroke.GetDayOfWeek())
				{
					case 1: bWorkDay = (nWorkWeekMask & xtpCalendarDaySunday) != 0; break;
					case 2: bWorkDay = (nWorkWeekMask & xtpCalendarDayMonday) != 0; break;
					case 3: bWorkDay = (nWorkWeekMask & xtpCalendarDayTuesday) != 0; break;
					case 4: bWorkDay = (nWorkWeekMask & xtpCalendarDayWednesday) != 0; break;
					case 5: bWorkDay = (nWorkWeekMask & xtpCalendarDayThursday) != 0; break;
					case 6: bWorkDay = (nWorkWeekMask & xtpCalendarDayFriday) != 0; break;
					case 7: bWorkDay = (nWorkWeekMask & xtpCalendarDaySaturday) != 0; break;
					default: break;
				}
				if (!bWorkDay)
					pDC->FillSolidRect(&rcBack, pView->m_clrWeekendBackground);
			}

			COleDateTime dtNow = COleDateTime::GetCurrentTime();

			BOOL bSelected = dtStroke.GetYear() == dtNow.GetYear()
							 && dtStroke.GetMonth() == dtNow.GetMonth()
							 && dtStroke.GetDay() == dtNow.GetDay();

			CRect rcLabel(rcBack);
			rcLabel.DeflateRect(XTP_DPI_X(6), XTP_DPI_Y(4));

			FormatLabel(strLabel);

			if (bSelected)
			{
				pDC->FillSolidRect(&rcBack, pView->m_clrSelectedBackground);
				pViewPart->DrawText(pDC, strLabel, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_VCENTER,
									pView->m_clrSelectedText, m_pSelFont);
			}
			else
			{
				pViewPart->DrawText(pDC, strLabel, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_VCENTER,
									pView->m_clrTimeScaleText);
			}
		}
	}
}

void CXTPCalendarPaintManager::FormatLabel(CString& strLabel)
{
	UNREFERENCED_PARAMETER(strLabel);
}

void CXTPCalendarPaintManager::PartDraw(CTimeLineViewTimeScalePart_Month* pViewPart, CDC* pDC,
										const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);

	if (pDC->GetSafeHdc() && pViewPart && pView && pView->GetTimeScale())
	{
		CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
		pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

		pViewPart->DrawHeader(pDC, rcRect, pView, xtpDTScale_Month);

		CRect rcBorder = rcRect;
		rcBorder.top   = rcBorder.bottom - XTP_DPI_Y(1);
		pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

		CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
		if (!pTSParams)
		{
			ASSERT(FALSE);
			return;
		}

		COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
		int nSInterval_px			 = pTSParams->GetScaleIntervalWidth_px();

		COleDateTime dtStart = pTimeScale->GetStartViewDate();
		COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

		// COleDateTimeSpan spFirstStroke_Shift = dtBegin - dtStart;
		double dFirstStroke_Shift = (double)(dtBegin - dtStart);
		double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval /
																	 // nSInterval_px);

		int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

		CString strLabel;
		int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + XTP_DPI_X(1);
		for (int i = 0; i < nCount; i++)
		{
			COleDateTimeSpan spFirstStroke((double)spSInterval * i);
			COleDateTime dtStroke = dtBegin + spFirstStroke;
			BOOL bBold			  = dtStroke.GetDay() == 1;
			CRect rcStroke;

			rcStroke.left  = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
			rcStroke.right = rcStroke.left + XTP_DPI_X(bBold ? 2 : 1);

			rcStroke.bottom = rcRect.bottom;
			rcStroke.top	= rcStroke.bottom - XTP_DPI_Y(bBold ? 10 : 8);

			pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

			CRect rcLabel = rcRect;
			if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(), bTimeLineFocused,
							  FALSE))
			{
				rcLabel.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(1);
				rcLabel.left  = rcStroke.left + XTP_DPI_X(2);
				rcLabel.right = rcLabel.left + nSInterval_px - XTP_DPI_X(3);
				rcLabel.bottom -= XTP_DPI_Y(2);
			}
			else
			{
				rcLabel.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(3);
				rcLabel.left   = rcStroke.left + XTP_DPI_X(4);
				rcLabel.right  = rcLabel.left + nSInterval_px - XTP_DPI_X(7);
				rcLabel.bottom = rcStroke.top + XTP_DPI_Y(4);
			}

			int nWD = dtStroke.GetDayOfWeek() - 1;
			if (nWD == 0)
				nWD = 7;
			CString strWD = CXTPCalendarUtils::GetLocaleString(
				XTPToUInt(CXTPCalendarUtils::GetDayOfWeekLocaleStringId(nWD)));

			//  Microsoft BUG: Unless the user has manually tweaked the "iFirstDayOfWeek" registry
			//  key, GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, ...) always returns
			//  zero, which means Sunday, even though the locale on the system indicates
			//  otherwise.
			// Ask the system for the first day of the week
			// TCHAR sz[2];
			//::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, sz, 2);
			//::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, sz, 2);
			// CString strWD =
			// CXTPCalendarUtils::GetLocaleString(CXTPCalendarUtils::GetDayOfWeekLocaleStringId(nWD));
			TCHAR szWD1[8] = { 0 };
			STRNCPY_S(szWD1, 4, strWD, 1);
			strLabel.Format(_T("%s%d"), szWD1, dtStroke.GetDay());

			pDC->SetBkColor(pView->m_clrTimeScaleBackground);

			if (nWD % 7 == 0)
				pDC->FillSolidRect(&rcStroke, pView->m_clrEventBar);

			if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(), bTimeLineFocused,
							  FALSE))
			{
				int nWorkWeekMask = XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(),
												  nWorkWeekMask, 0);
				BOOL bWorkDay	 = FALSE;
				switch (dtStroke.GetDayOfWeek())
				{
					case 1: bWorkDay = (nWorkWeekMask & xtpCalendarDaySunday) != 0; break;
					case 2: bWorkDay = (nWorkWeekMask & xtpCalendarDayMonday) != 0; break;
					case 3: bWorkDay = (nWorkWeekMask & xtpCalendarDayTuesday) != 0; break;
					case 4: bWorkDay = (nWorkWeekMask & xtpCalendarDayWednesday) != 0; break;
					case 5: bWorkDay = (nWorkWeekMask & xtpCalendarDayThursday) != 0; break;
					case 6: bWorkDay = (nWorkWeekMask & xtpCalendarDayFriday) != 0; break;
					case 7: bWorkDay = (nWorkWeekMask & xtpCalendarDaySaturday) != 0; break;
					default: break;
				}
				if (!bWorkDay)
					pDC->FillSolidRect(&rcLabel, pView->m_clrWeekendBackground);
			}

			COleDateTime dtNow = COleDateTime::GetCurrentTime();

			BOOL bSelected = dtStroke.GetYear() == dtNow.GetYear()
							 && dtStroke.GetMonth() == dtNow.GetMonth()
							 && dtStroke.GetDay() == dtNow.GetDay();

			if (bSelected)
			{
				pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);
				pViewPart->DrawText(pDC, strLabel, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_VCENTER,
									pView->m_clrSelectedText, m_pSelFont);
			}
			else
			{
				pViewPart->DrawText(pDC, strLabel, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_VCENTER,
									pView->m_clrTimeScaleText);
			}
		}
	}
}

void CXTPCalendarPaintManager::PartDraw(CTimeLineViewTimeScalePart_WorkWeek* pViewPart, CDC* pDC,
										const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pView);

	if (pDC->GetSafeHdc() && pViewPart && pView && pView->GetTimeScale())
	{
		CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
		pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

		pViewPart->DrawHeader(pDC, rcRect, pView, xtpDTScale_Month);

		CRect rcBorder = rcRect;
		rcBorder.top   = rcBorder.bottom - XTP_DPI_Y(1);
		pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

		CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
		if (!pTSParams)
		{
			ASSERT(FALSE);
			return;
		}

		COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
		int nSInterval_px			 = pTSParams->GetScaleIntervalWidth_px();

		COleDateTime dtStart = pTimeScale->GetStartViewDate();
		COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

		// COleDateTimeSpan spFirstStroke_Shift = dtBegin - dtStart;
		double dFirstStroke_Shift = (double)(dtBegin - dtStart);
		double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval /
																	 // nSInterval_px);

		int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

		CString strLabel;
		int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + XTP_DPI_X(1);
		for (int i = 0; i < nCount; i++)
		{
			COleDateTimeSpan spFirstStroke((double)spSInterval * i);
			COleDateTime dtStroke = dtBegin + spFirstStroke;

			CRect rcStroke;
			rcStroke.left  = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
			rcStroke.right = rcStroke.left + XTP_DPI_X(1);

			rcStroke.bottom = rcRect.bottom;
			rcStroke.top	= rcStroke.bottom - XTP_DPI_Y(8);

			pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

			CRect rcLabel = rcRect;
			if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(), bTimeLineFocused,
							  FALSE))
			{
				rcLabel.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(1);
				rcLabel.left  = rcStroke.left + XTP_DPI_X(2);
				rcLabel.right = rcLabel.left + nSInterval_px - XTP_DPI_X(3);
				rcLabel.bottom -= XTP_DPI_Y(2);
			}
			else
			{
				rcLabel.top += pViewPart->m_nHeaderHeight + XTP_DPI_Y(7);
				rcLabel.left   = rcStroke.left + 1;
				rcLabel.right  = rcLabel.left + nSInterval_px;
				rcLabel.bottom = rcStroke.top - XTP_DPI_Y(1);
			}

			int nWD = dtStroke.GetDayOfWeek() - 1;
			if (nWD == 0)
				nWD = 7;
			CString strWD = CXTPCalendarUtils::GetLocaleString(
				XTPToUInt(CXTPCalendarUtils::GetDayOfWeekLocaleStringId(nWD)));
			strLabel.Format(_T("%s %d"), (LPCTSTR)strWD, dtStroke.GetDay());

			pDC->SetBkColor(pView->m_clrTimeScaleBackground);

			COleDateTime dtNow = COleDateTime::GetCurrentTime();
			if (dtStroke.GetYear() == dtNow.GetYear() && dtStroke.GetMonth() == dtNow.GetMonth()
				&& dtStroke.GetDay() == dtNow.GetDay())
				pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);

			if (XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(), bTimeLineFocused,
							  FALSE))
			{
				int nWorkWeekMask = XTP_SAFE_GET3(pView, GetCalendarControl(), GetCalendarOptions(),
												  nWorkWeekMask, 0);
				BOOL bWorkDay	 = FALSE;
				switch (dtStroke.GetDayOfWeek())
				{
					case 1: bWorkDay = (nWorkWeekMask & xtpCalendarDaySunday) != 0; break;
					case 2: bWorkDay = (nWorkWeekMask & xtpCalendarDayMonday) != 0; break;
					case 3: bWorkDay = (nWorkWeekMask & xtpCalendarDayTuesday) != 0; break;
					case 4: bWorkDay = (nWorkWeekMask & xtpCalendarDayWednesday) != 0; break;
					case 5: bWorkDay = (nWorkWeekMask & xtpCalendarDayThursday) != 0; break;
					case 6: bWorkDay = (nWorkWeekMask & xtpCalendarDayFriday) != 0; break;
					case 7: bWorkDay = (nWorkWeekMask & xtpCalendarDaySaturday) != 0; break;
					default: break;
				}
				if (!bWorkDay)
					pDC->FillSolidRect(&rcLabel, pView->m_clrWeekendBackground);
			}

			pViewPart->DrawText(pDC, strLabel, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_VCENTER,
								pView->m_clrTimeScaleText);
		}
	}
}

void CXTPCalendarPaintManager::PartDrawGroup(CTimeLineViewPart* pViewPart, CDC* pDC,
											 const CRect& rcRect,
											 CXTPCalendarTimeLineViewGroup* pGroup)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pGroup);

	if (pDC->GetSafeHdc() && pViewPart && pGroup)
	{
		// TODO: draw group header if need.

		int nCount = pGroup->GetViewEventsCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarTimeLineViewEvent* pViewEv = pGroup->GetViewEvent(i);
			pViewPart->DrawEvent(pDC, rcRect, pViewEv);
		}
	}
}

void CXTPCalendarPaintManager::PartDrawEvent(CTimeLineViewPart* pViewPart, CDC* pDC,
											 const CRect& rcEvents,
											 CXTPCalendarTimeLineViewEvent* pViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent)
	{
		CRect rcEvent = pViewEvent->GetEventRect();

		if (!pDC->IsPrinting())
		{
			int t = rcEvents.top;
			if (rcEvent.top < t)
				return; // to hide not fully visible event
			rcEvent.top -= t - XTP_DPI_Y(70);
			rcEvent.bottom -= t - XTP_DPI_Y(70);
		}

		rcEvent.bottom -= XTP_DPI_Y(2);
		rcEvent.right -= XTP_DPI_X(2);

		pViewEvent->SetLastVisibleRect(rcEvent);

		if (pViewEvent->IsSelected())
			pDC->FillSolidRect(rcEvent, pViewPart->m_clrSelectedBackground);

		CXTPCalendarOptions* pOptions = pViewEvent->GetCalendarControl()->GetCalendarOptions();

		// Draw border
		if (pOptions->bTimeLineCompact && pOptions->bTimeLineFocused)
		{
			pDC->FillSolidRect(rcEvent, pViewPart->m_clrBackground);
			pDC->Draw3dRect(rcEvent, pViewPart->m_clrEventBorderLine,
							pViewPart->m_clrEventBorderLine);
		}

		int indent = 0;

		// Draw line
		{
			CRect rcLine = rcEvent;
			rcLine.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
			rcLine.bottom = rcLine.top + XTP_DPI_Y(pOptions->bTimeLineFocused ? 5 : 7);
			rcLine.right  = rcLine.left + pViewEvent->GetEventDurationInPixels() - XTP_DPI_X(1) * 2;
			pDC->FillSolidRect(rcLine, pViewEvent->GetView()->m_clrEventBar);
			rcLine.bottom = rcLine.top + XTP_DPI_Y(1);
			pDC->FillSolidRect(rcLine, pViewEvent->GetView()->m_clrEventBarLine);

			indent += XTP_DPI_X(1);
		}

		// Draw icon
		if (pViewPart->m_pimgEvent)
		{
			CSize szIcon = pViewPart->m_pimgEvent->GetExtent();

			CRect rcIcon = rcEvent;
			rcIcon.left += indent + XTP_DPI_X(2);
			rcIcon.bottom -= XTP_DPI_Y(5);
			rcIcon.top   = rcIcon.bottom - szIcon.cy;
			rcIcon.right = rcIcon.left + szIcon.cx;

			pViewPart->m_pimgEvent->DrawImage(pDC, rcIcon, pViewPart->m_pimgEvent->GetSource());

			indent += XTP_DPI_X(2) + rcIcon.Width();
		}

		// Draw text
		COLORREF clrText = pViewEvent->GetView()->m_clrText;
		if (pViewEvent->IsSelected())
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			pDC->DrawFocusRect(rcEvent);
			clrText = pViewPart->m_clrSelectedText;
		}

		pDC->SetBkMode(TRANSPARENT);

		CXTPFontDC autoFont(pDC, &pViewPart->GetTextFont(), clrText);

		CRect rcText = rcEvent;
		rcText.left += indent + XTP_DPI_X(2);
		rcText.right -= XTP_DPI_X(2);
		rcText.bottom -= XTP_DPI_Y(3);

		CString sText = pViewEvent->GetEvent()->GetSubject();

		// extend text draw size in NonCompact mode
		if (!pOptions->bTimeLineCompact)
		{
			CRect rcCl;
			pViewEvent->GetCalendarControl()->GetClientRect(&rcCl);
			if (rcText.left < rcCl.left && rcText.right > rcCl.right)
			{
				rcText.left  = rcCl.left;
				rcText.right = rcCl.right;
			}
			else if (rcText.left <= rcCl.left && rcText.right > rcCl.left)
			{
				rcText.left = rcCl.left; // keep right side
			}
			else if (rcText.left > rcCl.left)
			{
				rcText.right = rcCl.right; // unrestricted length if start visible
			}
		}
		pDC->SetTextColor(pViewEvent->GetView()->m_clrText);
		pDC->DrawText(sText, rcText, DT_SINGLELINE | DT_BOTTOM | DT_NOCLIP | DT_END_ELLIPSIS);
	}
}

//-- CALENDAR PART GET METHODS

COLORREF CXTPCalendarPaintManager::PartGetBackColor(CXTPCalendarViewPart* pViewPart)
{
	while (pViewPart != NULL)
	{
		if (!pViewPart->m_clrBackground.IsDefaultValue())
			return pViewPart->m_clrBackground;

		pViewPart = pViewPart->m_pParentPart;
		ASSERT(pViewPart);
	};

	return 0;
}

COLORREF CXTPCalendarPaintManager::PartGetTextColor(CXTPCalendarViewPart* pViewPart)
{
	while (pViewPart != NULL)
	{
		if (!pViewPart->m_clrTextColor.IsDefaultValue())
			return pViewPart->m_clrTextColor;

		pViewPart = pViewPart->m_pParentPart;
		ASSERT(pViewPart);
	};

	return 0;
}

CFont& CXTPCalendarPaintManager::PartGetTextFont(CXTPCalendarViewPart* pViewPart,
												 CXTPCalendarViewPartFontValue& fntText)
{
	while (pViewPart != NULL)
	{
		if (!pViewPart->m_fntText.IsDefaultValue())
			return pViewPart->m_fntText;

		pViewPart = pViewPart->m_pParentPart;
		ASSERT(pViewPart);
	};

	return fntText;
}

void CXTPCalendarPaintManager::PartGetParams(CDayViewCellPart* pViewPart,
											 CXTPCalendarDayViewGroup* pViewGroup,
											 XTP_CALENDAR_DAYVIEWCELL_PARAMS* rCellParams)
{
	if (pViewGroup)
	{
		rCellParams->clrBackgroundColor = rCellParams->bSelected
											  ? m_clrHighlight.operator COLORREF()
											  : pViewPart->GetBackgroundColor();
	}
}

//-- CALENDAR PART ADJUST METHODS

void CXTPCalendarPaintManager::PartCalcWidth(CDayViewTimeScaleCellPart* pViewPart, CDC* pDC,
											 const CString& strHour, const CString& strMin,
											 int nHourHeight, int& nWidth)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		CSize szMin(0, 0), szHour(0, 0);
		{
			CXTPFontDC fnt(pDC, &pViewPart->GetTextFont());
			szMin = pDC->GetTextExtent(strMin);
		}

		{
			CRect rcDefaultCell(0, 0, XTP_DPI_X(100), nHourHeight);
			pViewPart->AdjustTimeFont(pDC, rcDefaultCell);

			CXTPFontDC fnt(pDC, pViewPart->m_fntTimeText);
			szHour = pDC->GetTextExtent(strHour);

			// check also widest hours number
			CSize szHour2 = pDC->GetTextExtent(_T("88"), 2);
			if (szHour2.cx > szHour.cx)
				szHour = szHour2;
		}

		pViewPart->m_nHourWidth = szHour.cx;
		pViewPart->m_nMinWidth  = szMin.cx;
		nWidth					= max(XTP_DPI_X(42), szHour.cx + szMin.cx + XTP_DPI_X(17));
	}
}

void CXTPCalendarPaintManager::PartAdjustTimeFont(CDayViewTimeScaleCellPart* pViewPart, CDC* pDC,
												  CRect rcCell)
{
	ASSERT(pDC);
	ASSERT(pViewPart);

	if (pDC->GetSafeHdc() && pViewPart)
	{
		rcCell.DeflateRect(XTP_DPI_X(5), XTP_DPI_Y(6), XTP_DPI_X(1), XTP_DPI_Y(7));

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

CSize CXTPCalendarPaintManager::PartCalcEventSize(CTimeLineViewPart* pViewPart, CDC* pDC,
												  CXTPCalendarTimeLineViewEvent* pViewEvent)
{
	ASSERT(pDC);
	ASSERT(pViewPart);
	ASSERT(pViewEvent);

	if (pDC->GetSafeHdc() && pViewPart && pViewEvent)
	{
		CXTPCalendarTimeLineView* pView = pViewEvent->GetView();
		if (!pView)
			return CSize(0, 0);

		CXTPResourceImage* pImage = pViewPart->m_pimgEvent;
		if (!pImage)
			return CSize(0, 0);

		CSize szEvent = pImage->GetExtent();

		CSize szText = pViewPart->GetTextExtent(pDC, pViewEvent->GetItemTextSubject());

		int iMaxPixels = pView->m_nTimelineMaxPixelsForEvent;

		int nScaleID = pView->GetTimeScaleID();
		if (nScaleID == 1)
			iMaxPixels = pView->m_nTimelineMaxPixelsForEvent_ScaleDay;
		else if (nScaleID == 2)
			iMaxPixels = pView->m_nTimelineMaxPixelsForEvent_ScaleWeek;

		szEvent.cx = szEvent.cx + min(szText.cx, iMaxPixels) + XTP_DPI_X(10);
		szEvent.cy = max(szEvent.cy, szText.cy) + XTP_DPI_Y(10);

		return szEvent;
	}

	return CSize(0, 0);
}

/*- CALENDAR PART RENDERING -*/

void CXTPCalendarPaintManager::CControlPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewEventPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewEventPart::OnDraw(CDC* pDC,
														 CXTPCalendarDayViewEvent* pViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pViewEvent);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::CalcWidth(CDC* pDC,
																	const CString& strHour,
																	const CString& strMin,
																	int nHourHeight, int& nWidth)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartCalcWidth(this, pDC, strHour, strMin, nHourHeight, nWidth);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::AdjustTimeFont(CDC* pDC, CRect rcCell)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartAdjustTimeFont(this, pDC, rcCell);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::DrawHourCell(
	CDC* pDC, CRect rc, CString strText, BOOL bFillBackground, BOOL bCurrentTime /*= FALSE*/)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawHourCell(this, pDC, rc, strText, bFillBackground, bCurrentTime);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::DrawBigHourCell(
	CDC* pDC, CRect rc, CString strHour, CString strMin, int nRowPerHour, BOOL bFillBackground,
	BOOL bCurrentTime /*= FALSE*/)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawBigHourCell(this, pDC, rc, strHour, strMin, nRowPerHour,
											 bFillBackground, bCurrentTime);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::OnDraw(CDC* pDC,
																   CXTPCalendarDayView* pView,
																   CRect rc, CString strText)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pView, rc, strText);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::DrawNowLine(CDC* pDC,
																		CXTPCalendarDayView* pView,
																		CRect rc, int y,
																		BOOL bDrawBk)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawNowLine(this, pDC, pView, rc, y, bDrawBk);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::DrawNowLine(
	CDC* pDC, CXTPCalendarDayView* pView, CXTPCalendarDayViewTimeScale* pScale, CRect rc, int y,
	BOOL bDrawBk)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawNowLine(this, pDC, pView, pScale, rc, y, bDrawBk);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::DrawExpandSigns(
	CDC* pDC, CXTPCalendarDayView* pView, const CRect& rcTSHours)
{
	ASSERT(pView);
	ASSERT(m_pPaintManager);

	if (pView && m_pPaintManager)
	{
		if (pView->IsExpandUp())
		{
			m_pPaintManager->DrawExpandSign(pDC, TRUE, rcTSHours);
		}
		else if (pView->IsExpandDown())
		{
			m_pPaintManager->DrawExpandSign(pDC, FALSE, rcTSHours);
		}
	}
}

void CXTPCalendarPaintManager::CDayViewCellPart::GetParams(
	CXTPCalendarDayViewGroup* pViewGroup, XTP_CALENDAR_DAYVIEWCELL_PARAMS* rCellParams)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartGetParams(this, pViewGroup, rCellParams);
	}
}

void CXTPCalendarPaintManager::CDayViewCellPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewCellPart::OnDraw(
	CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
	const XTP_CALENDAR_DAYVIEWCELL_PARAMS& cellParams)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pViewGroup, rc, cellParams);
	}
}

void CXTPCalendarPaintManager::CDayViewAllDayEventsPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewAllDayEventsPart::OnDraw(
	CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup, CRect rc, BOOL bSelected)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pViewGroup, rc, bSelected);
	}
}

void CXTPCalendarPaintManager::CDayViewHeaderPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewHeaderPart::OnDraw(CDC* pDC,
														  CXTPCalendarDayViewDay* pViewDay,
														  CRect rc, CString strText)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pViewDay, rc, strText);
	}
}

void CXTPCalendarPaintManager::CDayViewAllDayEventScrollIconsPart::OnDraw(
	CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup, BOOL bDrawUp, BOOL bDrawDown,
	const CRect& rcIconUp, const CRect& rcIconDown, BOOL bHighlightUp, BOOL bHighlightDown)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pViewGroup, bDrawUp, bDrawDown, rcIconUp, rcIconDown,
								  bHighlightUp, bHighlightDown);
	}
}

void CXTPCalendarPaintManager::CDayViewGroupHeaderPart::OnDraw(CDC* pDC,
															   CXTPCalendarDayViewGroup* pViewGroup,
															   CRect rc, CString strText)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pViewGroup, rc, strText);
	}
}

void CXTPCalendarPaintManager::CWeekViewPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CWeekViewPart::DrawDayHeader(CDC* pDC, CRect rcDay,
															int nHeaderHeight, CString strHeader,
															BOOL bIsCurrent, BOOL bIsSelected)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawDayHeader(this, pDC, rcDay, nHeaderHeight, strHeader, bIsCurrent,
										   bIsSelected);
	}
}

void CXTPCalendarPaintManager::CWeekViewPart::OnDraw(CDC* pDC, CXTPCalendarWeekView* pWeekView)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pWeekView);
	}
}

void CXTPCalendarPaintManager::CWeekViewEventPart::DrawBorder(
	CDC* pDC, CRect rcView, CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawBorder(this, pDC, rcView, pWeekViewEvent);
	}
}

int CXTPCalendarPaintManager::CWeekViewEventPart::DrawTimes(
	CDC* pDC, CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		return m_pPaintManager->PartDrawTimes(this, pDC, pWeekViewEvent);
	}
	return 0;
}

void CXTPCalendarPaintManager::CWeekViewEventPart::DrawSubj(
	CDC* pDC, CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawSubj(this, pDC, pWeekViewEvent);
	}
}

void CXTPCalendarPaintManager::CWeekViewEventPart::OnDraw(CDC* pDC,
														  CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pWeekViewEvent);
	}
}

void CXTPCalendarPaintManager::CMonthViewGridPart::OnDrawGrid(CDC* pDC,
															  CXTPCalendarMonthView* pMonthView)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawGrid(this, pDC, pMonthView);
	}
}

void CXTPCalendarPaintManager::CMonthViewHeaderPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CMonthViewHeaderPart::OnDraw(CDC* pDC,
															CXTPCalendarMonthView* pMonthView,
															CRect rc, int nCollIndex,
															CString strText)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, pMonthView, rc, nCollIndex, strText);
	}
}

void CXTPCalendarPaintManager::CMonthViewEventPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CMonthViewEventPart::OnDrawDayDate(CDC* pDC, CRect rc, BOOL bToday,
																  BOOL bSelected,
																  BOOL bFirstMonthDay,
																  CString strText)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawDayDate(this, pDC, rc, bToday, bSelected, bFirstMonthDay, strText);
	}
}

void CXTPCalendarPaintManager::CMonthViewEventPart::OnDrawEvent(
	CDC* pDC, CXTPCalendarMonthViewEvent* pViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawEvent(this, pDC, pViewEvent);
	}
}

void CXTPCalendarPaintManager::CDayViewWorkCellPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CDayViewNonworkCellPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CMonthViewGridPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Day::Draw(CDC* pDC, const CRect& rcRect,
																	CXTPCalendarTimeLineView* pView)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, rcRect, pView);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Week::Draw(
	CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, rcRect, pView);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Month::Draw(
	CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, rcRect, pView);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_WorkWeek::Draw(
	CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDraw(this, pDC, rcRect, pView);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewPart::DrawGroup(CDC* pDC, const CRect& rcRect,
															CXTPCalendarTimeLineViewGroup* pGroup)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawGroup(this, pDC, rcRect, pGroup);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewPart::DrawEvent(
	CDC* pDC, const CRect& rcEvents, CXTPCalendarTimeLineViewEvent* pViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartDrawEvent(this, pDC, rcEvents, pViewEvent);
	}
}

CSize CXTPCalendarPaintManager::CTimeLineViewPart::CalcEventSize(
	CDC* pDC, CXTPCalendarTimeLineViewEvent* pViewEvent)
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		return m_pPaintManager->PartCalcEventSize(this, pDC, pViewEvent);
	}
	return CSize(0);
}

void CXTPCalendarPaintManager::CTimeLineViewPart::RefreshMetrics()
{
	ASSERT(m_pPaintManager);
	if (m_pPaintManager)
	{
		m_pPaintManager->PartRefresh(this);
	}
}

CXTPCalendarPaintManager::CTimeLineViewPart::~CTimeLineViewPart()
{
	CMDTARGET_RELEASE(m_pimgEvent);
}

void CXTPCalendarPaintManager::CTimeLineViewPart::_Init()
{
	CXTPResourceImages* pImages = XTPResourceImages();
	m_pimgEvent					= new CXTPResourceImage(pImages);
}

//////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

CXTPCalendarFlagsSet_imp* CXTPCalendarPaintManager::AxGetAskItemTextFlags()
{
	CXTPCalendarFlagsSet_imp* pFlagsWrapper = new CXTPCalendarFlagsSet_imp();
	if (pFlagsWrapper)
	{
		pFlagsWrapper->SetDataRef(&m_dwAskItemTextFlags, this);
	}
	return pFlagsWrapper;
}

#endif
