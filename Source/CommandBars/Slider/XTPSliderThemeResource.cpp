// XTPSliderThemeResource.cpp : implementation of the CXTPSliderResourceTheme class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"

#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"
#include "CommandBars/Themes/XTPCommandBarsResourceTheme.h"

#include "CommandBars/Slider/XTPSliderPaintManager.h"
#include "CommandBars/Slider/XTPSliderThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPSliderResourceTheme

void CXTPSliderResourceTheme::RefreshMetrics()
{
	CXTPSliderPaintManager::RefreshMetrics();

	CXTPResourceImage* pImage = ((CXTPResourceTheme*)m_pPaintManager)->LoadImage(_T("SLIDERUP"));

	if (pImage)
	{
		m_cyHScroll = XTP_DPI_Y(pImage->GetSource(0, 3).Height());
		m_cxHScroll = XTP_DPI_X(pImage->GetSource(0, 3).Width());
	}

	m_cThumb = XTP_DPI_X(11);
}

void CXTPSliderResourceTheme::DrawScrollBar(CDC* pDC, CXTPScrollBase* pGallery)
{
	if (!XTPResourceImages()->IsValid())
		return;

#define GETPARTSTATE4(ht) (nPressedHt == ht ? 2 : nHotHt == ht ? 1 : 0)

	XTP_SCROLLBAR_TRACKINFO* pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO* pSBInfo	= pGallery->GetScrollBarPosInfo();

	ASSERT(!pSBInfo->fSizebox && !pSBInfo->fVert);

	BOOL nPressedHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1)
							   : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0)
					&& pGallery->IsScrollBarEnabled();

	int nBtnTrackSize = pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos  = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	CXTPResourceTheme* pPaintManager = (CXTPResourceTheme*)m_pPaintManager;

	if (!pSBInfo->fVert)
	{
		CRect rcHScroll(pSBInfo->rc);

		CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
		CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);
		CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);
		CRect rcBtnTrack(rcTrack.left + nBtnTrackPos, rcTrack.top,
						 rcTrack.left + nBtnTrackPos + nBtnTrackSize, rcTrack.bottom);

		CXTPResourceImage* pImage = pPaintManager->LoadImage(_T("SLIDERTRACK"));
		ASSERT(pImage);
		if (!pImage)
			return;

		CRect rcTrackDest(CPoint(rcTrack.left,
								 (rcTrack.top + rcTrack.bottom - XTP_DPI_Y(pImage->GetHeight()))
									 / 2),
						  CSize(rcTrack.Width(), XTP_DPI_Y(pImage->GetHeight())));
		pImage->DrawImage(pDC, rcTrackDest, pImage->GetSource());

		pImage = pPaintManager->LoadImage(_T("SLIDERTICK"));

		XTP_SLIDERTICKS* pTicks = pGallery->GetTicks();
		if (!pTicks || (pSBInfo->posMax <= pSBInfo->posMin))
		{
			CRect rcTrackTickDest(
				CPoint((rcTrackDest.left + rcTrackDest.right - XTP_DPI_X(pImage->GetWidth())) / 2,
					   (rcTrackDest.top + rcTrackDest.bottom - XTP_DPI_Y(pImage->GetHeight())) / 2),
				XTP_DPI(pImage->GetExtent()));

			pImage->DrawImage(pDC, rcTrackTickDest, pImage->GetSource(), CRect(0, 0, 0, 0),
							  0xFF00FF);
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
					CRect rcTrackTickDest(CPoint(rcTrackDest.left + (int)dPos
													 - XTP_DPI_X(pImage->GetWidth()) / 2,
												 (rcTrackDest.top + rcTrackDest.bottom
												  - XTP_DPI_Y(pImage->GetHeight()))
													 / 2),
										  XTP_DPI(pImage->GetExtent()));

					pImage->DrawImage(pDC, rcTrackTickDest, pImage->GetSource(), CRect(0, 0, 0, 0),
									  0xFF00FF);
				}
			}
		}

		pImage = pPaintManager->LoadImage(_T("SLIDERUP"));
		pImage->DrawImage(pDC, rcArrowLeft, pImage->GetSource(GETPARTSTATE4(XTP_HTSCROLLUP), 3));

		pImage = pPaintManager->LoadImage(_T("SLIDERDOWN"));
		pImage->DrawImage(pDC, rcArrowRight, pImage->GetSource(GETPARTSTATE4(XTP_HTSCROLLDOWN), 3));

		if (bEnabled)
		{
			pImage = pPaintManager->LoadImage(_T("SLIDERTHUMB"));

			CRect rcImgSrc = pImage->GetSource(nPressedHt == XTP_HTSCROLLTHUMB
												   ? 2
												   : nPressedHt == XTP_HTSCROLLUPPAGE
															 || nPressedHt == XTP_HTSCROLLDOWNPAGE
															 || nHotHt == XTP_HTSCROLLUPPAGE
															 || nHotHt == XTP_HTSCROLLDOWNPAGE
															 || nHotHt == XTP_HTSCROLLTHUMB
														 ? 1
														 : 0,
											   3);

			CRect rcBtnTrackDest(
				CPoint((rcBtnTrack.left + rcBtnTrack.right - XTP_DPI_X(rcImgSrc.Width())) / 2,
					   (rcBtnTrack.top + rcBtnTrack.bottom - XTP_DPI_Y(rcImgSrc.Height())) / 2),
				XTP_DPI(rcImgSrc.Size()));
			pImage->DrawImage(pDC, rcBtnTrackDest, rcImgSrc);
		}
	}
}
