// XTPSliderThemeOffice2007System.cpp : implementation of the CXTPSliderThemeOffice2007System class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Slider/XTPSliderPaintManager.h"
#include "CommandBars/Slider/XTPSliderThemeOffice2007System.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPSliderThemeOffice2007System::CXTPSliderThemeOffice2007System(CXTPPaintManager* pPaintManager)
	: CXTPSliderPaintManager(pPaintManager)
{
}

void CXTPSliderThemeOffice2007System::RefreshMetrics()
{
	CXTPSliderPaintManager::RefreshMetrics();
}

void CXTPSliderThemeOffice2007System::DrawScrollBar(CDC* pDC, CXTPScrollBase* pScrollBar)
{
	XTP_SCROLLBAR_TRACKINFO* pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO* pSBInfo	= pScrollBar->GetScrollBarPosInfo();

	ASSERT(!pSBInfo->fSizebox && !pSBInfo->fVert);

	int nPressedHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1)
							  : -1;
	int nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
		return;

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0)
					&& pScrollBar->IsScrollBarEnabled();

	int nBtnTrackSize = pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos  = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	COLORREF clrFace   = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);
	COLORREF clrLight  = GetXtremeColor(COLOR_3DHIGHLIGHT);

	if (!pSBInfo->fVert)
	{
		CRect rcHScroll(pSBInfo->rc);

		CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
		CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);
		CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);
		CRect rcBtnTrack(rcTrack.left + nBtnTrackPos, rcTrack.top,
						 rcTrack.left + nBtnTrackPos + nBtnTrackSize, rcTrack.bottom);

		CRect rcTrackDest(CPoint(rcTrack.left, (rcTrack.top + rcTrack.bottom - XTP_DPI_Y(2)) / 2),
						  CSize(rcTrack.Width(), XTP_DPI_Y(2)));

		pDC->FillSolidRect(rcTrackDest.left, rcTrackDest.top, rcTrackDest.Width(), XTP_DPI_Y(1),
						   clrShadow);
		pDC->FillSolidRect(rcTrackDest.left, rcTrackDest.top + XTP_DPI_Y(1), rcTrackDest.Width(),
						   XTP_DPI_Y(1), clrLight);

		XTP_SLIDERTICKS* pTicks = pScrollBar->GetTicks();
		if (!pTicks || (pSBInfo->posMax <= pSBInfo->posMin))
		{
			CRect rcTrackTickDest(CPoint((rcTrackDest.left + rcTrackDest.right - 2) / 2,
										 (rcTrackDest.top + rcTrackDest.bottom - 8) / 2),
								  CSize(2, 8));

			pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top, XTP_DPI_X(1),
							   XTP_DPI_Y(4), clrShadow);
			pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top + XTP_DPI_Y(5),
							   XTP_DPI_X(1), XTP_DPI_Y(2), clrShadow);
			pDC->FillSolidRect(rcTrackTickDest.left + XTP_DPI_X(1),
							   rcTrackTickDest.top + XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(7),
							   clrLight);
		}
		else
		{
			rcTrackDest.DeflateRect(XTP_DPI_X(6), 0);

			for (int i = 0; i < pTicks->nCount; i++)
			{
				double dTick = pTicks->pTicks[i];
				double dPos  = (dTick - pSBInfo->posMin) * rcTrackDest.Width()
							  / (pSBInfo->posMax - pSBInfo->posMin);

				if (dPos >= 0 && dPos <= rcTrackDest.Width())
				{
					CRect rcTrackTickDest(
						CPoint(rcTrackDest.left + (int)dPos - XTP_DPI_X(1),
							   (rcTrackDest.top + rcTrackDest.bottom - XTP_DPI_Y(8)) / 2),
						XTP_DPI(CSize(2, 8)));

					pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top, XTP_DPI_X(1),
									   XTP_DPI_Y(4), clrShadow);
					pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top + XTP_DPI_Y(5),
									   XTP_DPI_X(1), XTP_DPI_Y(2), clrShadow);
					pDC->FillSolidRect(rcTrackTickDest.left + XTP_DPI_X(1),
									   rcTrackTickDest.top + XTP_DPI_Y(1), XTP_DPI_X(1),
									   XTP_DPI_Y(7), clrLight);
				}
			}
		}
		m_pPaintManager->AlphaEllipse(pDC, rcArrowLeft, clrShadow,
									  nHotHt == XTP_HTSCROLLUP ? clrLight : clrFace);
		if (nPressedHt != XTP_HTSCROLLUP)
		{
			rcArrowLeft.DeflateRect(1, 1);
			m_pPaintManager->AlphaEllipse(pDC, rcArrowLeft, clrLight,
										  nHotHt == XTP_HTSCROLLUP ? clrLight : clrFace);
		}
		CPoint ptCenter = rcArrowLeft.CenterPoint();
		pDC->FillSolidRect(ptCenter.x - XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(1), XTP_DPI_X(8),
						   XTP_DPI_Y(2), clrShadow);

		m_pPaintManager->AlphaEllipse(pDC, rcArrowRight, clrShadow,
									  nHotHt == XTP_HTSCROLLDOWN ? clrLight : clrFace);
		if (nPressedHt != XTP_HTSCROLLDOWN)
		{
			rcArrowRight.DeflateRect(1, 1);
			m_pPaintManager->AlphaEllipse(pDC, rcArrowRight, clrLight,
										  nHotHt == XTP_HTSCROLLDOWN ? clrLight : clrFace);
		}
		ptCenter = rcArrowRight.CenterPoint();
		pDC->FillSolidRect(ptCenter.x - XTP_DPI_X(4), ptCenter.y - XTP_DPI_Y(1), XTP_DPI_X(8),
						   XTP_DPI_Y(2), clrShadow);
		pDC->FillSolidRect(ptCenter.x - XTP_DPI_X(1), ptCenter.y - XTP_DPI_Y(4), XTP_DPI_X(2),
						   XTP_DPI_Y(8), clrShadow);

		if (bEnabled)
		{
			CRect rc(CPoint((rcBtnTrack.left + rcBtnTrack.right - XTP_DPI_X(8)) / 2,
							(rcBtnTrack.top + rcBtnTrack.bottom - XTP_DPI_Y(13)) / 2),
					 XTP_DPI(CSize(8, 13)));

			POINT pts[] = { { rc.left, rc.top },
							{ rc.right, rc.top },
							{ rc.right, rc.bottom - XTP_DPI_Y(4) },
							{ rc.left + XTP_DPI_X(4), rc.bottom },
							{ rc.left, rc.bottom - XTP_DPI_Y(4) },
							{ rc.left, rc.top } };

			CXTPPenDC pen(*pDC, nHotHt == XTP_HTSCROLLTHUMB ? clrLight : clrFace);
			CXTPBrushDC brush(*pDC, nHotHt == XTP_HTSCROLLTHUMB ? clrLight : clrFace);

			pDC->Polygon(pts, sizeof(pts) / sizeof(POINT));

			pen.Color(clrShadow);
			pDC->Polyline(pts, sizeof(pts) / sizeof(POINT));

			if (nPressedHt != XTP_HTSCROLLTHUMB)
			{
				POINT pts2[] = { { rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1) },
								 { rc.right - XTP_DPI_X(1), rc.top + XTP_DPI_Y(1) },
								 { rc.right - XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(4) },
								 { rc.left + XTP_DPI_X(4), rc.bottom - XTP_DPI_Y(1) },
								 { rc.left + XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(4) },
								 { rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1) } };

				pen.Color(clrLight);
				pDC->Polyline(pts2, sizeof(pts2) / sizeof(POINT));
			}
		}
	}
}
