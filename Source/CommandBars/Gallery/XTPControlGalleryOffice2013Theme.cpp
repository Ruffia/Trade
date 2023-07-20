// XTPControlGalleryOffice2013Theme.cpp : implementation of the CXTPControlGalleryOffice2013Theme
// class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/Gallery/XTPControlGalleryOffice2013Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPControlGalleryOffice2013Theme::CXTPControlGalleryOffice2013Theme(CXTPPaintManager* pPaintManager)
	: CXTPControlGalleryPaintManager(pPaintManager)
{
}

void CXTPControlGalleryOffice2013Theme::RefreshMetrics()
{
	CXTPControlGalleryPaintManager::RefreshMetrics();

	m_clrBack			= XTPIniColor(_T("CommandBars.Gallery"), _T("Back"), RGB(255, 255, 255));
	m_clrBorder			= XTPIniColor(_T("CommandBars.Gallery"), _T("Border"), RGB(171, 175, 178));
	m_clrScrollTriangle = XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollTriangle"),
									  RGB(119, 119, 119));
	m_clrScrollTriangleHot = XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollTriangleHighlight"),
										 RGB(68, 68, 68));
	m_clrScrollTrianglePressed  = XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollTrianglePushed"),
											  RGB(0, 0, 0));
	m_clrScrollTriangleDisabled = XTPIniColor(_T("CommandBars.Gallery"),
											  _T("ScrollTriangleDisabled"), RGB(198, 198, 198));
	m_clrScrollBorderNormal		= XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollBorder"),
										  RGB(171, 171, 171));
	m_clrScrollBorderHot	  = XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollBorderHighlight"),
										RGB(163, 189, 227));
	m_clrScrollBorderDisabled = XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollBorderDisabled"),
											RGB(225, 225, 225));
	m_clrScrollFillHot		  = XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollBackHighlight"),
										RGB(213, 225, 242));
	m_clrScrollFillPressed	= XTPIniColor(_T("CommandBars.Gallery"), _T("ScrollBackPushed"),
											RGB(163, 189, 227));
}

void CXTPControlGalleryOffice2013Theme::DrawScrollButton(CDC* pDC, const CRect& rc, int type,
														 CXTPControlGallery* pGallery)
{
	XTP_SCROLLBAR_TRACKINFO* pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO* pSBInfo	= pGallery->GetScrollBarPosInfo();

	BOOL bControlEnabled = pGallery->GetEnabled();
	BOOL nPressedHt		 = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt			 = pSBTrack ? -1 : pSBInfo->ht;

	BOOL bEnabled = bControlEnabled;

	if (type != XTP_HTSCROLLPOPUP)
	{
		bEnabled = (bEnabled && pGallery->IsScrollButtonEnabled(type));
	}

	int nState = GETPARTSTATE3(type, bEnabled);

	BOOL bPartEnabled  = IS_PART_ENABLED(nState);
	BOOL bPartPressed  = IS_PART_PRESSED(nState);
	BOOL bPartSelected = IS_PART_SELECTED(nState);

	if (bPartSelected)
	{
		pDC->FillSolidRect(&rc, m_clrScrollFillHot);
	}
	else if (bPartPressed)
	{
		pDC->FillSolidRect(&rc, m_clrScrollFillPressed);
	}

	COLORREF clrBorder = (bPartSelected || bPartPressed) ? m_clrScrollBorderHot
														 : m_clrScrollBorderNormal;
	pDC->Draw3dRect(&rc, clrBorder, clrBorder);

	int n2x = XTP_DPI_X(2);
	int n3x = XTP_DPI_X(3);
	int n4x = XTP_DPI_X(4);

	int n1y = XTP_DPI_Y(1);
	int n2y = XTP_DPI_Y(2);

	if (!bPartEnabled) // draw disabled line
	{
		CXTPPenDC penDC(*pDC, m_clrScrollBorderDisabled);
		pDC->MoveTo(rc.left, rc.top + n1y);
		pDC->LineTo(rc.left, rc.bottom - n1y);
	}

	COLORREF clrTriangle = bPartSelected
							   ? m_clrScrollTriangleHot
							   : bPartPressed ? m_clrScrollTrianglePressed
											  : bPartEnabled ? m_clrScrollTriangle
															 : m_clrScrollTriangleDisabled;
	CPoint pt = rc.CenterPoint();

	switch (type)
	{
		case XTP_HTSCROLLUP:
		{
			XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x, pt.y - n1y),
									   CPoint(pt.x - n2x, pt.y + n1y),
									   CPoint(pt.x + n2x, pt.y + n1y), clrTriangle);
			break;
		}

		case XTP_HTSCROLLPOPUP:
		{
			XTPDrawHelpers()->SolidRectangle(pDC,
											 CRect(CPoint(pt.x - n3x, pt.y - n2y),
												   CPoint(pt.x + n4x, pt.y - n2y + n1y)),
											 clrTriangle, clrTriangle);
			pt.y += n2y;
		}

		case XTP_HTSCROLLDOWN:
		{
			XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x, pt.y + n1y),
									   CPoint(pt.x - n2x, pt.y - n1y),
									   CPoint(pt.x + n2x, pt.y - n1y), clrTriangle);
			break;
		}
	}

	if (bPartSelected || bPartPressed)
	{
		pDC->ExcludeClipRect(&rc);
	}
}

void CXTPControlGalleryOffice2013Theme::DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery)
{
	XTP_SCROLLBAR_POSINFO* pSBInfo = pGallery->GetScrollBarPosInfo();

	CRect rcScroll = pSBInfo->rc;
	pDC->FillSolidRect(rcScroll, m_clrBack);

	CRect rcScrollUp(rcScroll.left, rcScroll.top - 1, rcScroll.right + 1,
					 rcScroll.top + m_cyPopupUp);
	CRect rcScrollDown(rcScroll.left, rcScrollUp.bottom - 1, rcScroll.right + 1,
					   rcScrollUp.bottom + m_cyPopupDown + 1);
	CRect rcScrollPopup(rcScroll.left, rcScrollDown.bottom - 1, rcScroll.right + 1,
						rcScroll.bottom + 1);

	DrawScrollButton(pDC, rcScrollUp, XTP_HTSCROLLUP, pGallery);
	DrawScrollButton(pDC, rcScrollDown, XTP_HTSCROLLDOWN, pGallery);
	DrawScrollButton(pDC, rcScrollPopup, XTP_HTSCROLLPOPUP, pGallery);
}

void CXTPControlGalleryOffice2013Theme::FillControl(CDC* pDC, CXTPControlGallery* pGallery,
													CRect rcControl)
{
	if (!pGallery->IsTransparent())
	{
		pDC->FillSolidRect(rcControl, m_clrBack);
	}

	if (pGallery->IsShowBorders())
	{
		pDC->Draw3dRect(rcControl, m_clrBorder, m_clrBorder);
	}

	if (pGallery->HasBottomSeparator())
	{
		pDC->FillSolidRect(rcControl.left, rcControl.bottom - 2, rcControl.Width(), 1,
						   m_pPaintManager->GetXtremeColor(XPCOLOR_SEPARATOR));
	}
}
