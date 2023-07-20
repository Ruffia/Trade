// XTPTrackPaintManager.cpp : implementation of the CXTPTrackPaintManager class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMarkupRender.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRecordItem.h"

#include "GridControl/TrackControl/XTPTrackPaintManager.h"

#include "GridControl/TrackControl/XTPTrackControlItem.h"
#include "GridControl/TrackControl/XTPTrackControl.h"
#include "GridControl/TrackControl/XTPTrackBlock.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTrackPaintManager::CXTPTrackPaintManager()
{
	RefreshMetrics();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif

	m_nTimeLineStep		= 1;
	m_bTimeLineStepAuto = TRUE;

#ifdef _XTP_ACTIVEX
	m_bCustomTimeLineLabel = FALSE;
#endif
}

CXTPTrackPaintManager::~CXTPTrackPaintManager()
{
}

void CXTPTrackPaintManager::RefreshMetrics()
{
	CXTPGridPaintManager::RefreshMetrics();

	m_clrWorkArea = RGB(245, 245, 245);

	m_clrTrackHeader   = GetSysColor(COLOR_3DSHADOW);
	m_clrTrackTimeArea = GetSysColor(COLOR_3DFACE);

	m_clrTimeHeaderDarkDark   = GetSysColor(COLOR_3DDKSHADOW);
	m_clrTimeHeaderDark		  = GetSysColor(COLOR_3DSHADOW);
	m_clrTimeSliderBackground = GetSysColor(COLOR_3DSHADOW);

	m_clrTimeHeaderDivider = GetSysColor(COLOR_3DDKSHADOW);

	m_clrScrollBarLight = GetSysColor(COLOR_WINDOW);
	m_clrScrollBarDark  = GetSysColor(COLOR_3DFACE);

	m_clrMarker = GetSysColor(COLOR_3DFACE);

	m_clrSelectedArea = GetSysColor(COLOR_HIGHLIGHT);

	m_clrTimeLineTick			 = RGB(35, 35, 35);
	m_clrMarkerBorder			 = RGB(43, 90, 127);
	m_clrMarkerBack				 = RGB(153, 192, 231);
	m_clrTrackMarkerBorder		 = RGB(0, 0, 0);
	m_clrTrackMarkerText		 = RGB(0, 0, 0);
	m_clrTrackMarkerTextBack	 = RGB(230, 230, 230);
	m_clrTrackMarkerTextBorder   = RGB(123, 123, 123);
	m_clrTimeLinePosition		 = RGB(205, 0, 0);
	m_clrTimeHeaderBorder		 = RGB(35, 35, 35);
	m_clrScrollBarBorderDark	 = RGB(0, 0, 0);
	m_clrScrollBarBorderLight	= RGB(155, 155, 155);
	m_clrWorkAreaScrollBarLight  = RGB(153, 192, 231);
	m_clrWorkAreaScrollBarDark   = RGB(27, 102, 157);
	m_clrWorkAreaScrollBarBorder = RGB(43, 90, 127);
}

void CXTPTrackPaintManager::SetColumnStyle(XTPGridColumnStyle columnStyle)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	if (::IsWindow(pTrackControl->GetSafeHwnd()))
	{
		switch (columnStyle)
		{
			case xtpGridColumnShaded: pTrackControl->SetTheme(xtpGridThemeOffice2003, FALSE); break;
			case xtpGridColumnFlat: pTrackControl->SetTheme(xtpGridThemeOfficeXP, FALSE); break;
			case xtpGridColumnExplorer: pTrackControl->SetTheme(xtpGridThemeExplorer, FALSE); break;
			case xtpGridColumnOffice2003:
				pTrackControl->SetTheme(xtpGridThemeOffice2003Luna, FALSE);
				break;
			case xtpGridColumnResource: pTrackControl->SetTheme(xtpGridThemeResource, FALSE); break;
		}
	}
}

#ifdef _XTP_ACTIVEX
COleControl* GetParentOleControl(CWnd* pWnd);
#endif

CString CXTPTrackPaintManager::FormatTime(int nPosition)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CString strCaption;
	strCaption.Format(pTrackControl->m_strTimeFormat, nPosition);

	return strCaption;
}

CString CXTPTrackPaintManager::FormatTimeLineTick(int nPosition, COLORREF& clrTick,
												  int& nTickLength)
{
	CString strCaption = FormatTime(nPosition);

#ifdef _XTP_ACTIVEX
	if (m_bCustomTimeLineLabel)
	{
		BSTR bstrValue = strCaption.AllocSysString();

		COleControl* pControl = GetParentOleControl(m_pControl);
		ASSERT(pControl);

		pControl->FireEvent(1014, EVENT_PARAM(VTS_I4 VTS_PBSTR VTS_PCOLOR VTS_PI4), nPosition,
							&bstrValue, &clrTick, &nTickLength);

		strCaption = bstrValue;
		SysFreeString(bstrValue);
	}
#else
	UNREFERENCED_PARAMETER(clrTick);
	UNREFERENCED_PARAMETER(nTickLength);
#endif

	return strCaption;
}

void CXTPTrackPaintManager::DrawTimeLine(CDC* pDC)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea   = pTrackControl->GetElementRect(xtpGridElementRectHeaderArea);

	CXTPGridColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	int nLeftOffset				 = pTrackColumn->GetRect().left;
	int nRightOffset			 = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset,
					   rcTimeLineArea.top + XTP_DPI_Y(10));
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.top);

	rcTimeArea.DeflateRect(XTP_DPI_X(7), 0);

	pDC->FillSolidRect(rcTimeArea, m_clrTrackTimeArea);

	int nTimeScaleMin = pTrackControl->GetTimeLineMin();
	int nTimeScaleMax = pTrackControl->GetTimeLineMax();

	int delta = pTrackControl->PositionToTrack(100) - pTrackControl->PositionToTrack(0);

	int dx = 1;

	if (m_bTimeLineStepAuto)
	{
		if (delta < 30)
			dx = 1000;
		else if (delta < 150)
			dx = 100;
		else if (delta < 300)
			dx = 20;
		else if (delta < 500)
			dx = 10;
		else if (delta < 2000)
			dx = 5;
		else
			dx = 1;

		m_nTimeLineStep = dx;
	}
	else
	{
		dx = m_nTimeLineStep;
	}

	CXTPFontDC dcFont(pDC, pTrackControl->GetPaintManager()->GetTextFont());
	pDC->SetTextColor(m_clrTrackMarkerText);

	int nFirstPos = nTimeScaleMin / dx * dx;
	if (nFirstPos < nTimeScaleMin)
		nFirstPos += dx;

	for (int pos = nFirstPos; pos <= nTimeScaleMax; pos += dx)
	{
		int x = pTrackControl->PositionToTrack(pos);

		if (x < nLeftOffset - XTP_DPI_X(20) || x > nRightOffset + XTP_DPI_X(20))
			continue;

		int nTickLength = 8;

		CString strCaption = FormatTimeLineTick(pos, m_clrTimeLineTick, nTickLength);

		pDC->FillSolidRect(x, rcTimeArea.bottom - min(XTP_DPI_Y(4), nTickLength), XTP_DPI_X(1),
						   nTickLength, m_clrTimeLineTick);

		int nCaptionWidth = pDC->GetTextExtent(strCaption).cx;
		pDC->DrawText(strCaption,
					  CRect(x - nCaptionWidth / 2, rcTimeArea.top,
							x - nCaptionWidth / 2 + nCaptionWidth, rcTimeArea.bottom),
					  DT_VCENTER | DT_SINGLELINE);
	}
}

void CXTPTrackPaintManager::DrawMarkers(CDC* pDC)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea   = pTrackControl->GetElementRect(xtpGridElementRectHeaderArea);

	CXTPGridColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	int nLeftOffset				 = pTrackColumn->GetRect().left;
	int nRightOffset			 = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset,
					   rcTimeLineArea.top + XTP_DPI_Y(10));
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset,
					   rcHeaderArea.bottom - XTP_DPI_Y(3));

	int nPos = pTrackControl->PositionToTrack(pTrackControl->GetTimeLinePosition());

	CXTPPenDC pen(*pDC, m_clrMarkerBorder);
	CXTPBrushDC brush(*pDC, m_clrMarkerBack);

	if (pTrackControl->m_bShowTimeLinePosition)
	{
		int X = nPos;
		// int Y = rcColumnArea.top + 7;
		int Y		 = rcColumnArea.top - XTP_DPI_Y(6);
		CPoint pts[] = { CPoint(X - XTP_DPI_X(5), Y), CPoint(X, Y + XTP_DPI_Y(5)),
						 CPoint(X + XTP_DPI_X(5), Y), CPoint(X + XTP_DPI_X(5), Y - XTP_DPI_Y(10)),
						 CPoint(X - XTP_DPI_X(5), Y - XTP_DPI_Y(10)) };

		pDC->Polygon(pts, 5);
	}

	pen.Color(m_clrTrackMarkerBorder);
	brush.Color(m_clrMarker);

	CXTPFontDC dcFont(pDC, pTrackControl->GetPaintManager()->GetTextFont());
	pDC->SetTextColor(m_clrTrackMarkerText);

	for (int i = 0; i < pTrackControl->GetMarkers()->GetCount(); i++)
	{
		CXTPTrackMarker* pMarker = pTrackControl->GetMarkers()->GetAt(i);

		int nTrackedPos = pTrackControl->PositionToTrack(pMarker->GetPosition());

		CString strCaption = pMarker->GetCaption();
		if (!strCaption.IsEmpty())
		{
			int dx = pDC->GetTextExtent(strCaption).cx;

			pDC->FillSolidRect(CRect(nTrackedPos, rcColumnArea.top + XTP_DPI_Y(2),
									 nTrackedPos + XTP_DPI_X(12) + dx,
									 rcColumnArea.bottom - XTP_DPI_Y(2)),
							   m_clrTrackMarkerTextBack);
			pDC->Draw3dRect(CRect(nTrackedPos, rcColumnArea.top + XTP_DPI_Y(2),
								  nTrackedPos + XTP_DPI_X(12) + dx,
								  rcColumnArea.bottom - XTP_DPI_Y(2)),
							m_clrTrackMarkerTextBorder, m_clrTrackMarkerTextBorder);

			pDC->DrawText(strCaption,
						  CRect(nTrackedPos + XTP_DPI_X(8), rcColumnArea.top,
								nTrackedPos + XTP_DPI_X(8) + dx, rcColumnArea.bottom),
						  DT_VCENTER | DT_SINGLELINE);
		}

		int X = nTrackedPos;
		int Y = rcHeaderArea.bottom - XTP_DPI_Y(10);

		CPoint pts[] = { CPoint(X - XTP_DPI_X(5), Y), CPoint(X, Y - XTP_DPI_Y(5)),
						 CPoint(X + XTP_DPI_X(5), Y), CPoint(X + XTP_DPI_X(5), Y + XTP_DPI_Y(5)),
						 CPoint(X - XTP_DPI_X(5), Y + XTP_DPI_Y(5)) };
		pDC->Polygon(pts, 5);

		pMarker->m_rcMarker = CRect(nTrackedPos - XTP_DPI_X(5), rcHeaderArea.bottom - XTP_DPI_Y(20),
									nTrackedPos + XTP_DPI_X(5), rcHeaderArea.bottom - XTP_DPI_Y(5));
	}

	if (pTrackControl->m_bShowTimeLinePosition)
	{
		pDC->FillSolidRect(CRect(nPos, rcColumnArea.top, nPos + XTP_DPI_X(1), rcHeaderArea.bottom),
						   m_clrTimeLinePosition);
	}
}

int CXTPTrackPaintManager::DrawTrackBlock(CDC* pDC, CRect rc, BOOL bSelected,
										  CXTPTrackBlock* pTrackBlock)
{
	if (!pTrackBlock)
		return 0;

	BOOL bLocked = pTrackBlock->IsLocked();

	int nHeight;

	if (pTrackBlock->m_nHeightFixed != 0)
		nHeight = pTrackBlock->m_nHeightFixed;
	else
		nHeight = int((rc.Height()) * pTrackBlock->m_dHeightPercent);

	if (nHeight != rc.Height())
	{
		if (pTrackBlock->m_nVerticalAlignment == DT_TOP)
		{
			rc.bottom = rc.top + nHeight;
		}
		else if (pTrackBlock->m_nVerticalAlignment == DT_BOTTOM)
		{
			rc.top = rc.bottom - nHeight;
		}
		else
		{
			rc.top	= (rc.top + rc.bottom - nHeight) / 2;
			rc.bottom = rc.top + nHeight;
		}
	}

	if (pTrackBlock->m_bLocked)
		bLocked = TRUE;

	CXTPTrackControl* pTrackControl = pTrackBlock->m_pItem->GetTrackControl();

	rc.left  = pTrackControl->PositionToTrack(pTrackBlock->m_nPosition);
	rc.right = pTrackControl->PositionToTrack(pTrackBlock->m_nPosition + pTrackBlock->m_nLength);

	COLORREF clrBackground = pTrackBlock->m_clrBlock;
	DWORD dwHSLBackground  = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL			   = GetBValue(dwHSLBackground);
	DWORD dwLight		   = (dwL + 240) / 2;

	COLORREF clrLight = CXTPDrawHelpers::HSLtoRGB(
		RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwLight));
	DWORD dwDark	 = XTPToDWORD(MulDiv(XTPToIntChecked(dwL), 2, 3));
	COLORREF clrDark = CXTPDrawHelpers::HSLtoRGB(
		RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));

	if (bSelected && !bLocked)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, clrLight, clrDark, FALSE);
	}
	else
	{
		pDC->FillSolidRect(rc, pTrackBlock->m_clrBlock);
	}

	if (bLocked)
	{
		pDC->Draw3dRect(rc, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DDKSHADOW));
	}
	else if (bSelected)
	{
		pDC->Draw3dRect(rc, clrLight, clrLight);
	}
	else
	{
		pDC->Draw3dRect(rc, clrLight, clrDark);
	}

	if (!pTrackBlock->m_strCaption.IsEmpty())
	{
		pDC->SetTextColor(bLocked ? GetSysColor(COLOR_GRAYTEXT) : GetSysColor(COLOR_WINDOWTEXT));

		CRect rcText(rc);
		rcText.DeflateRect(XTP_DPI_X(4), 0);

		XTPMarkupDrawText(pTrackBlock->m_pItem->GetTrackControl()->GetMarkupContext(),
						  pDC->GetSafeHdc(), pTrackBlock->m_strCaption, rcText,
						  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}

	pTrackBlock->m_rcBlock = rc;

	return 1;
}

void CXTPTrackPaintManager::DrawTrackHeader(CDC* pDC)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CXTPGridColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	if (!pTrackColumn || !pTrackColumn->IsVisible())
		return;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea   = pTrackControl->GetElementRect(xtpGridElementRectHeaderArea);

	pDC->FillSolidRect(rcTimeLineArea, m_clrTrackHeader);

	int nLeftOffset  = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset,
					   rcTimeLineArea.top + XTP_DPI_Y(10));
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset,
					   rcHeaderArea.bottom - XTP_DPI_Y(3));

	pDC->FillSolidRect(nLeftOffset, rcTimeLineArea.top, nRightOffset - nLeftOffset,
					   rcTimeLineArea.Height() + rcHeaderArea.Height(), m_clrTimeHeaderDarkDark);
	pDC->FillSolidRect(nLeftOffset - XTP_DPI_X(1), rcTimeLineArea.top, XTP_DPI_X(1),
					   rcTimeLineArea.Height() + rcHeaderArea.Height(), m_clrTimeHeaderDivider);

	pDC->FillSolidRect(nLeftOffset + XTP_DPI_X(7), rcColumnArea.top,
					   nRightOffset - nLeftOffset - XTP_DPI_X(14), rcColumnArea.Height(),
					   m_clrTimeHeaderDark);
	pDC->FillSolidRect(nLeftOffset, rcColumnArea.bottom - XTP_DPI_Y(1), nRightOffset - nLeftOffset,
					   XTP_DPI_Y(1), m_clrTimeHeaderBorder);

	pDC->FillSolidRect(nLeftOffset + XTP_DPI_X(7), rcSliderArea.top,
					   nRightOffset - nLeftOffset - XTP_DPI_X(14), rcSliderArea.Height(),
					   m_clrTimeSliderBackground);

	pDC->FillSolidRect(nLeftOffset, rcSliderArea.bottom, XTP_DPI_X(7),
					   rcColumnArea.top - rcSliderArea.bottom, m_clrTimeHeaderDark);
	pDC->FillSolidRect(nRightOffset - XTP_DPI_X(7), rcSliderArea.bottom, XTP_DPI_X(7),
					   rcColumnArea.top - rcSliderArea.bottom, m_clrTimeHeaderDark);

	CRect rcClipArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcHeaderArea.bottom);
	pDC->IntersectClipRect(rcClipArea);

	// TimeLine
	DrawTimeLine(pDC);

	// Work ScrollBar

	if (pTrackControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMin());
		int nWorkAreaMax = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMax());

		CRect rcWorkAreaScrollBar(nWorkAreaMin, rcColumnArea.top, nWorkAreaMax,
								  rcColumnArea.bottom);

		XTPDrawHelpers()->GradientFill(pDC, rcWorkAreaScrollBar, m_clrScrollBarLight,
									   m_clrScrollBarDark, FALSE);
		pDC->Draw3dRect(rcWorkAreaScrollBar.left - XTP_DPI_X(1), rcWorkAreaScrollBar.top,
						rcWorkAreaScrollBar.Width() + XTP_DPI_X(2), rcWorkAreaScrollBar.Height(),
						m_clrScrollBarBorderDark, m_clrScrollBarBorderLight);

		XTPDrawHelpers()->GradientFill(pDC,
									   CRect(rcWorkAreaScrollBar.left - XTP_DPI_X(7),
											 rcWorkAreaScrollBar.top, rcWorkAreaScrollBar.left + 0,
											 rcWorkAreaScrollBar.bottom),
									   m_clrWorkAreaScrollBarLight, m_clrWorkAreaScrollBarDark,
									   FALSE);

		pDC->Draw3dRect(CRect(rcWorkAreaScrollBar.left - XTP_DPI_X(7), rcWorkAreaScrollBar.top,
							  rcWorkAreaScrollBar.left + 0, rcWorkAreaScrollBar.bottom),
						m_clrWorkAreaScrollBarBorder, m_clrWorkAreaScrollBarBorder);

		XTPDrawHelpers()->GradientFill(pDC,
									   CRect(rcWorkAreaScrollBar.right - 0, rcWorkAreaScrollBar.top,
											 rcWorkAreaScrollBar.right + XTP_DPI_X(7),
											 rcWorkAreaScrollBar.bottom),
									   m_clrWorkAreaScrollBarLight, m_clrWorkAreaScrollBarDark,
									   FALSE);

		pDC->Draw3dRect(CRect(rcWorkAreaScrollBar.right - 0, rcWorkAreaScrollBar.top,
							  rcWorkAreaScrollBar.right + XTP_DPI_X(7), rcWorkAreaScrollBar.bottom),
						m_clrWorkAreaScrollBarBorder, m_clrWorkAreaScrollBarBorder);
	}

	// Zoom Slider

	rcSliderArea.DeflateRect(XTP_DPI_X(7), 0);

	int nZoomAreaMin = rcSliderArea.left
					   + MulDiv(pTrackControl->GetViewPortMin() - pTrackControl->GetTimeLineMin(),
								rcSliderArea.Width(),
								(pTrackControl->GetTimeLineMax()
								 - pTrackControl->GetTimeLineMin()));
	int nZoomAreaMax = rcSliderArea.left
					   + MulDiv(pTrackControl->GetViewPortMax() - pTrackControl->GetTimeLineMin(),
								rcSliderArea.Width(),
								(pTrackControl->GetTimeLineMax()
								 - pTrackControl->GetTimeLineMin()));

	CRect rcZoomSlider(nZoomAreaMin, rcSliderArea.top, nZoomAreaMax, rcSliderArea.bottom);

	XTPDrawHelpers()->GradientFill(pDC, rcZoomSlider, m_clrScrollBarLight, m_clrScrollBarDark,
								   FALSE);
	pDC->Draw3dRect(rcZoomSlider.left - XTP_DPI_X(1), rcZoomSlider.top,
					rcZoomSlider.Width() + XTP_DPI_X(2), rcZoomSlider.Height(),
					m_clrScrollBarBorderDark, m_clrScrollBarBorderDark);

	XTPDrawHelpers()->GradientFill(pDC,
								   CRect(rcZoomSlider.left - XTP_DPI_X(7), rcZoomSlider.top,
										 rcZoomSlider.left + 0, rcZoomSlider.bottom),
								   m_clrWorkAreaScrollBarLight, m_clrWorkAreaScrollBarDark, FALSE);

	pDC->Draw3dRect(CRect(rcZoomSlider.left - XTP_DPI_X(7), rcZoomSlider.top, rcZoomSlider.left + 0,
						  rcZoomSlider.bottom),
					m_clrWorkAreaScrollBarBorder, m_clrWorkAreaScrollBarBorder);

	XTPDrawHelpers()->GradientFill(pDC,
								   CRect(rcZoomSlider.right - 0, rcZoomSlider.top,
										 rcZoomSlider.right + XTP_DPI_X(7), rcZoomSlider.bottom),
								   m_clrWorkAreaScrollBarLight, m_clrWorkAreaScrollBarDark, FALSE);

	pDC->Draw3dRect(CRect(rcZoomSlider.right - 0, rcZoomSlider.top,
						  rcZoomSlider.right + XTP_DPI_X(7), rcZoomSlider.bottom),
					m_clrWorkAreaScrollBarBorder, m_clrWorkAreaScrollBarBorder);

	if (pTrackControl->m_bShowTimeLinePosition)
	{
		int nZoomAreaTimeLinePos =
			rcSliderArea.left
			+ MulDiv(pTrackControl->GetTimeLinePosition() - pTrackControl->GetTimeLineMin(),
					 rcSliderArea.Width(),
					 (pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin()));
		pDC->FillSolidRect(CRect(nZoomAreaTimeLinePos, rcZoomSlider.top, nZoomAreaTimeLinePos + 1,
								 rcZoomSlider.bottom),
						   m_clrTimeLinePosition);
	}

	DrawMarkers(pDC);

	pDC->SelectClipRgn(0);
}

#ifdef _XTP_ACTIVEX

#	define DISP_PROPERTY_UN_ID(theClass, szExternalName, dispid, vtPropType)                      \
		DISP_PROPERTY_EX_ID(theClass, #szExternalName, dispid, OleGet##szExternalName,             \
							OleSet##szExternalName, vtPropType)

BEGIN_DISPATCH_MAP(CXTPTrackPaintManager, CXTPGridPaintManager)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, WorkArea, 1000, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TrackHeader, 1001, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TrackTimeArea, 1002, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeHeaderDarkDark, 1003, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeHeaderDark, 1004, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeSliderBackground, 1005, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeHeaderDivider, 1006, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, ScrollBarLight, 1007, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, ScrollBarDark, 1008, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, Marker, 1009, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, SelectedArea, 1010, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPTrackPaintManager, "TimeLineStep", 1112, OleGetTimeLineStep,
						OleSetTimeLineStep, VT_I4)
	DISP_PROPERTY_ID(CXTPTrackPaintManager, "TimeLineStepAuto", 1113, m_bTimeLineStepAuto, VT_BOOL)
	DISP_PROPERTY_ID(CXTPTrackPaintManager, "CustomTimeLineLabel", 1114, m_bCustomTimeLineLabel,
					 VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPTrackPaintManager, "ColumnStyle", 7, m_columnStyle,
							OleSetColumnStyle, VT_I4)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPTrackPaintManager, CXTPGridPaintManager)
	INTERFACE_PART(CXTPTrackPaintManager, XTPDIID_ITrackPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackPaintManager, XTPDIID_ITrackPaintManager)

#	define IMPLEMENT_PROPERTY_CLR(theClass, theProperty, theMember)                               \
		COLORREF theClass::OleGet##theProperty()                                                   \
		{                                                                                          \
			return theMember;                                                                      \
		}                                                                                          \
		void theClass::OleSet##theProperty(COLORREF oleColor)                                      \
		{                                                                                          \
			theMember = AxTranslateColor(oleColor);                                                \
		}

IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, WorkArea, m_clrWorkArea)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TrackHeader, m_clrTrackHeader)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TrackTimeArea, m_clrTrackTimeArea)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeHeaderDarkDark, m_clrTimeHeaderDarkDark)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeHeaderDark, m_clrTimeHeaderDark)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeSliderBackground, m_clrTimeSliderBackground)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeHeaderDivider, m_clrTimeHeaderDivider)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, ScrollBarLight, m_clrScrollBarLight)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, ScrollBarDark, m_clrScrollBarDark)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, Marker, m_clrMarker)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, SelectedArea, m_clrSelectedArea)

void CXTPTrackPaintManager::OleSetTimeLineStep(int nStep)
{
	m_nTimeLineStep = nStep;
	if (m_pControl)
		m_pControl->RedrawControl();
}

int CXTPTrackPaintManager::OleGetTimeLineStep()
{
	return m_nTimeLineStep;
}

void CXTPTrackPaintManager::OleSetColumnStyle()
{
	SetColumnStyle(m_columnStyle);

	RefreshMetrics();
}

#endif
