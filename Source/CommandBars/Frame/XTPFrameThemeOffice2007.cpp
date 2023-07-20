// XTPFrameThemeOffice2007.cpp : implementation of the CXTPRibbonOffice2007FrameTheme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

// Frame
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/Frame/XTPFrameThemeOffice2007.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPane.h"

#ifndef OIC_WINLOGO
#	define OIC_WINLOGO 32517
#endif

#ifndef LAYOUT_BITMAPORIENTATIONPRESERVED
#	define LAYOUT_BITMAPORIENTATIONPRESERVED 0x00000008
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPRibbonOffice2007FrameTheme::CXTPRibbonOffice2007FrameTheme(CXTPPaintManager* pPaintManager)
	: CXTPFramePaintManager(pPaintManager)
{
	m_bCaptionCenterAligned = TRUE;
}

CXTPRibbonOffice2007FrameTheme::~CXTPRibbonOffice2007FrameTheme()
{
}

void CXTPRibbonOffice2007FrameTheme::RefreshMetrics()
{
	CXTPFramePaintManager::RefreshMetrics();

	if (!GetImages()->IsValid()) // Resources not loaded
		return;

	m_nFrameCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	m_nFrameCaptionHeight = max(m_nFrameCaptionHeight, XTP_DPI_Y(17));

	// Frame border
	m_clrFrameBorderActive0   = GetImages()->GetImageColor(_T("Window"), _T("BorderActive0"));
	m_clrFrameBorderActive1   = GetImages()->GetImageColor(_T("Window"), _T("BorderActive1"));
	m_clrFrameBorderActive2   = GetImages()->GetImageColor(_T("Window"), _T("BorderActive2"));
	m_clrFrameBorderActive3   = GetImages()->GetImageColor(_T("Window"), _T("BorderActive3"));
	m_clrFrameBorderInactive0 = GetImages()->GetImageColor(_T("Window"), _T("BorderInactive0"));
	m_clrFrameBorderInactive1 = GetImages()->GetImageColor(_T("Window"), _T("BorderInactive1"));
	m_clrFrameBorderInactive2 = GetImages()->GetImageColor(_T("Window"), _T("BorderInactive2"));
	m_clrFrameBorderInactive3 = GetImages()->GetImageColor(_T("Window"), _T("BorderInactive3"));

	m_clrFrameCaptionTextActiveTitle = GetImages()->GetImageColor(_T("Window"),
																  _T("CaptionTextActiveTitle"));
	m_clrFrameCaptionTextInactive	= GetImages()->GetImageColor(_T("Window"),
																  _T("CaptionTextInActive"));
	m_clrFrameCaptionTextActive = GetImages()->GetImageColor(_T("Window"), _T("CaptionTextActive"));

	m_bFlatFrame = GetImages()->GetImageInt(_T("Window"), _T("FlatFrame"), 0);

	CXTPNonClientMetrics ncm;
	ncm.lfSmCaptionFont.lfWeight = FW_NORMAL;

	if (m_pPaintManager->m_bClearTypeTextQuality
		&& XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		ncm.lfCaptionFont.lfQuality = 5;
	}

	ncm.lfCaptionFont.lfWeight = FW_NORMAL;
	if (m_pPaintManager->m_bUseOfficeFont
		&& CXTPDrawHelpers::FontExists(m_pPaintManager->m_strOfficeFont))
		STRCPY_S(ncm.lfCaptionFont.lfFaceName, LF_FACESIZE, m_pPaintManager->m_strOfficeFont);

	if (ncm.lfCaptionFont.lfHeight < 0)
		ncm.lfCaptionFont.lfHeight = min(-XTP_DPI_Y(11), ncm.lfCaptionFont.lfHeight);

	ncm.lfCaptionFont.lfCharSet = XTPResourceManager()->GetFontCharset();

	m_fntFrameCaption.SetStandardFont(&ncm.lfCaptionFont);
}

HRGN CXTPRibbonOffice2007FrameTheme::CalcFrameRegion(CXTPCommandBarsFrameHook* pFrameHook,
													 CSize sz) const
{
	if (pFrameHook->GetSite()->GetStyle() & WS_MAXIMIZE)
	{
		return NULL;
	}

	int cx = sz.cx, cy = sz.cy;

	RECT rgnTopFrame[] = { { XTP_DPI_X(4), 0, cx - XTP_DPI_X(4), XTP_DPI_Y(1) },
						   { XTP_DPI_X(2), XTP_DPI_Y(1), cx - XTP_DPI_X(2), XTP_DPI_Y(2) },
						   { XTP_DPI_X(1), XTP_DPI_Y(2), cx - XTP_DPI_X(1), XTP_DPI_Y(3) },
						   { XTP_DPI_X(1), XTP_DPI_Y(3), cx - XTP_DPI_X(1), XTP_DPI_Y(4) },
						   { 0, XTP_DPI_Y(4), cx, cy - XTP_DPI_Y(4) } };

	RECT rgnRibbonBottomFrame[] =
		{ { XTP_DPI_X(1), cy - XTP_DPI_Y(4), cx - XTP_DPI_X(1), cy - XTP_DPI_Y(2) },
		  { XTP_DPI_X(2), cy - XTP_DPI_Y(2), cx - XTP_DPI_X(2), cy - XTP_DPI_Y(1) },
		  { XTP_DPI_X(4), cy - XTP_DPI_Y(1), cx - XTP_DPI_X(4), cy - 0 } };

	RECT rgnSimpleBottomFrame[] = { { 0, cy - XTP_DPI_Y(4), cx, cy } };

	BOOL bHasStatusBar = (m_bRoundedCornersAlways || pFrameHook->IsFrameHasStatusBar());

	SIZE_T nSizeTopRect	= sizeof(rgnTopFrame);
	SIZE_T nSizeBottomRect = bHasStatusBar ? sizeof(rgnRibbonBottomFrame)
										   : sizeof(rgnSimpleBottomFrame);
	SIZE_T nSizeData = sizeof(RGNDATAHEADER) + nSizeTopRect + nSizeBottomRect;

	RGNDATA* pRgnData = (RGNDATA*)malloc(nSizeData);
	if (!pRgnData)
		return NULL;

	memcpy(&pRgnData->Buffer, (void*)&rgnTopFrame, nSizeTopRect);
	memcpy(&pRgnData->Buffer + nSizeTopRect,
		   bHasStatusBar ? (void*)&rgnRibbonBottomFrame : (void*)&rgnSimpleBottomFrame,
		   nSizeBottomRect);

	pRgnData->rdh.dwSize   = sizeof(RGNDATAHEADER);
	pRgnData->rdh.iType	= RDH_RECTANGLES;
	pRgnData->rdh.nCount   = XTPToDWORD((nSizeTopRect + nSizeBottomRect) / sizeof(RECT));
	pRgnData->rdh.nRgnSize = 0;
	pRgnData->rdh.rcBound  = CRect(0, 0, cx, cy);

	HRGN hRgn = ::ExtCreateRegion(NULL, XTPToDWORD(nSizeData), pRgnData);

	free(pRgnData);

	return hRgn;
}

void CXTPRibbonOffice2007FrameTheme::DrawFrame(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook)
{
	CWnd* pSite  = pFrameHook->GetSite();
	BOOL bActive = pFrameHook->IsFrameActive();

	CXTPClientRect rcBorders(pSite);
	pSite->ClientToScreen(rcBorders);

	CXTPWindowRect rc(pSite);

	int nRightBorder  = rcBorders.left - rc.left;
	int nLeftBorder   = rcBorders.left - rc.left;
	int nTopBorder	= CalcFrameBorder(pFrameHook).top;
	int nBottomBorder = rc.bottom - rcBorders.bottom;

	rc.OffsetRect(-rc.TopLeft());
	CRect rcFrame(rc);

	int nCaptionHeight = pFrameHook->GetCaptionHeight();
	rcFrame.top += nCaptionHeight;

	int nStatusHeight  = 0;
	BOOL bHasStatusBar = pFrameHook->IsFrameHasStatusBar(&nStatusHeight);

	int nBordersHeight = bHasStatusBar ? rcFrame.Height() - nStatusHeight - 1 : rcFrame.Height();

	if (nLeftBorder > 0)
		pDC->FillSolidRect(rc.left + 0, rcFrame.top, 1, rcFrame.Height(),
						   bActive ? m_clrFrameBorderActive0 : m_clrFrameBorderInactive0);
	if (nLeftBorder > 1)
		pDC->FillSolidRect(rc.left + 1, rcFrame.top, 1, nBordersHeight,
						   bActive ? m_clrFrameBorderActive1 : m_clrFrameBorderInactive1);

	if (nRightBorder > 0)
		pDC->FillSolidRect(rc.right - 1, rcFrame.top, 1, rcFrame.Height(),
						   bActive ? m_clrFrameBorderActive0 : m_clrFrameBorderInactive0);
	if (nRightBorder > 1)
		pDC->FillSolidRect(rc.right - 2, rcFrame.top, 1, nBordersHeight,
						   bActive ? m_clrFrameBorderActive1 : m_clrFrameBorderInactive1);

	if (m_bFlatFrame)
	{
		if (nLeftBorder > 2)
			pDC->FillSolidRect(rc.left + 2, rcFrame.top, nLeftBorder - 3, nBordersHeight,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);
		if (nLeftBorder > 2)
			pDC->FillSolidRect(rc.left + nLeftBorder - 1, rcFrame.top, 1, nBordersHeight,
							   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);

		if (nRightBorder > 2)
			pDC->FillSolidRect(rc.right - nRightBorder + 1, rcFrame.top, nRightBorder - 3,
							   nBordersHeight,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);
		if (nRightBorder > 2)
			pDC->FillSolidRect(rc.right - nRightBorder, rcFrame.top, 1, nBordersHeight,
							   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
	}
	else
	{
		if (nLeftBorder > 2)
			pDC->FillSolidRect(rc.left + 2, rcFrame.top, 1, nBordersHeight,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);
		if (nLeftBorder > 3)
			pDC->FillSolidRect(rc.left + 3, rcFrame.top, nLeftBorder - 3, nBordersHeight,
							   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);

		if (nRightBorder > 2)
			pDC->FillSolidRect(rc.right - 3, rcFrame.top, 1, nBordersHeight,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);
		if (nRightBorder > 3)
			pDC->FillSolidRect(rc.right - nRightBorder, rcFrame.top, nRightBorder - 3,
							   nBordersHeight,
							   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
	}

	pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1,
					   bActive ? pSite->GetStyle() & WS_MAXIMIZE ? m_clrFrameBorderActive3
																 : m_clrFrameBorderActive0
							   : m_clrFrameBorderInactive0);

	if (nBottomBorder > 1)
	{
		if (m_bFlatFrame)
		{
			pDC->FillSolidRect(rc.left + nLeftBorder, rc.bottom - nBottomBorder,
							   rc.Width() - nLeftBorder - nRightBorder, 1,
							   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
			pDC->FillSolidRect(rc.left + 1, rc.bottom - nBottomBorder + 1, rc.Width() - 2,
							   nBottomBorder - 2,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);
		}
		else
		{
			pDC->FillSolidRect(rc.left + 1, rc.bottom - nBottomBorder, rc.Width() - 2,
							   nBottomBorder - 1,
							   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CRect rcCaption(rc.left, rc.top, rc.right, rc.top + nCaptionHeight);

	if (pFrameHook->IsCaptionVisible())
	{
		CXTPBufferDC dc(*pDC, rcCaption);
		CRect rcTopLeft, rcTopRight, rcTopCenter, rcSrcTopLeft, rcSrcTopRight;
		rcTopLeft.SetRectEmpty();
		rcTopRight.SetRectEmpty();
		rcTopCenter.SetRectEmpty();
		rcSrcTopLeft.SetRectEmpty();
		rcSrcTopRight.SetRectEmpty();

		CXTPResourceImage* pImage = LoadImage(_T("FRAMETOPLEFT"));
		if (pImage)
		{
			rcSrcTopLeft = pImage->GetSource(bActive ? 0 : 1, 2);

			rcTopLeft.SetRect(rc.left, rc.top, rc.left + rcSrcTopLeft.Width(), rcCaption.bottom);
			pImage->DrawImage(&dc, rcTopLeft, rcSrcTopLeft, CRect(0, 5, 0, 3));
		}
		//

		pImage = LoadImage(_T("FRAMETOPRIGHT"));
		if (pImage)
		{
			rcSrcTopRight = pImage->GetSource(bActive ? 0 : 1, 2);

			rcTopRight.SetRect(rc.right - rcSrcTopRight.Width(), rc.top, rc.right,
							   rcCaption.bottom);
			pImage->DrawImage(&dc, rcTopRight, rcSrcTopRight, CRect(0, 5, 0, 3));
		}

		pImage = LoadImage(_T("FRAMETOPCENTER"));
		if (pImage)
		{
			rcTopCenter.SetRect(rc.left + rcTopLeft.Width(), rc.top,
								rc.right - rcSrcTopRight.Width(), rcCaption.bottom);
			pImage->DrawImage(&dc, rcTopCenter, pImage->GetSource(bActive ? 0 : 1, 2),
							  CRect(0, 5, 0, 3));
		}

		CRect rcCaptionText(rcCaption);
		rcCaptionText.left = XTP_DPI_X(7);
		rcCaptionText.DeflateRect(0, nTopBorder, 0, XTP_DPI_Y(3));

		DrawFrameIcon(&dc, pFrameHook, rcCaptionText);

		DrawFrameCaptionButtons(&dc, pFrameHook, rcCaptionText);

		rcCaptionText.left += nLeftBorder;
		rcCaptionText.right -= nRightBorder;

		DrawCaptionText(&dc, rcCaptionText, pSite, bActive, pFrameHook->m_pRibbonBar);

		if (m_bFlatFrame)
		{
			dc.FillSolidRect(rcCaption.left + nLeftBorder, rcCaption.bottom - 1,
							 rcCaption.Width() - nLeftBorder - nRightBorder, 1,
							 bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
		}
	}
	else
	{
		CRect rcSrc;
		rcSrc.SetRectEmpty();

		CXTPResourceImage* pImage = LoadImage(_T("FRAMETOPLEFT"));
		if (pImage)
		{
			rcSrc		= pImage->GetSource(bActive ? 0 : 1, 2);
			rcSrc.right = XTP_UNDPI_X(nLeftBorder);

			CRect rcTopLeft(rc.left, rcCaption.top, rc.left + nLeftBorder, rcCaption.bottom);
			pImage->DrawImage(pDC, rcTopLeft, rcSrc, CRect(0, 5, 0, 3));
		}

		pImage = LoadImage(_T("FRAMETOPRIGHT"));
		if (pImage)
		{
			rcSrc	  = pImage->GetSource(bActive ? 0 : 1, 2);
			rcSrc.left = rcSrc.right - XTP_UNDPI_X(nRightBorder);

			CRect rcTopRight(rc.right - nRightBorder, rcCaption.top, rc.right, rcCaption.bottom);
			pImage->DrawImage(pDC, rcTopRight, rcSrc, CRect(0, 5, 0, 3));
		}

		pImage = LoadImage(_T("FRAMETOPCENTER"));
		if (pImage)
		{
			rcSrc		 = pImage->GetSource(bActive ? 0 : 1, 2);
			rcSrc.bottom = rcSrc.top + XTP_UNDPI_Y(nTopBorder);

			CRect rcTopCenter(rc.left + nLeftBorder, rc.top, rc.right - nRightBorder,
							  rc.top + nTopBorder);
			pImage->DrawImage(pDC, rcTopCenter, rcSrc, CRect(0, 0, 0, 0));
		}

		pFrameHook->DrawRibbonFramePart(pDC);
	}

	if (bHasStatusBar)
	{
		if (!m_bFlatFrame)
		{
			// Determine if left and right borders should be dark.
			BOOL bLeftIsDark  = FALSE;
			BOOL bRightIsDark = FALSE;
			if (pFrameHook->GetFramePaintManager()->m_bFrameStatusBar)
			{
				CWnd* pStatusBarWnd = pFrameHook->GetSite()->GetDlgItem(AFX_IDW_STATUS_BAR);
				if (pStatusBarWnd && pStatusBarWnd->IsKindOf(RUNTIME_CLASS(CXTPStatusBar)))
				{
					CXTPStatusBar* pStatusBar = static_cast<CXTPStatusBar*>(pStatusBarWnd);
					int nPaneCount			  = pStatusBar->GetPaneCount();
					if (0 < nPaneCount)
					{
						int nIndex;

						// Check if the leftmost visible pane is dark.
						for (nIndex = 0; nIndex < nPaneCount; ++nIndex)
						{
							CXTPStatusBarPane* pPane = pStatusBar->GetPane(nIndex);
							ASSERT(NULL != pPane);
							if (pPane->IsVisible())
							{
								bLeftIsDark = pPane->IsDark();
								break;
							}
						}

						// Check if the rightmost visible pane is dark.
						for (nIndex = nPaneCount - 1; 0 <= nIndex; --nIndex)
						{
							CXTPStatusBarPane* pPane = pStatusBar->GetPane(nIndex);
							ASSERT(NULL != pPane);
							if (pPane->IsVisible())
							{
								bRightIsDark = pPane->IsDark();
								break;
							}
						}
					}
				}
			}

			// Draw left border.
			CRect rcSrc;
			CXTPResourceImage* pImage = LoadImage(bLeftIsDark ? _T("STATUSBARDARK")
															  : _T("STATUSBARLIGHT"));
			if (pImage)
			{
				rcSrc.SetRect(0, 0, XTP_UNDPI_X(nLeftBorder) - 1, pImage->GetHeight());
				CRect rcLight(rc.left + XTP_DPI_X(1), rc.bottom - nStatusHeight - nBottomBorder,
							  rc.left + nLeftBorder, rc.bottom - nBottomBorder);
				pImage->DrawImage(pDC, rcLight, rcSrc, CRect(0, 0, 0, 0));
			}

			// Draw right border.
			pImage = LoadImage(bRightIsDark ? _T("STATUSBARDARK") : _T("STATUSBARLIGHT"));
			if (pImage)
			{
				rcSrc.SetRect(0, 0, XTP_UNDPI_X(nRightBorder) - 1, pImage->GetHeight());
				CRect rcDark(rc.right - nRightBorder, rc.bottom - nStatusHeight - nBottomBorder,
							 rc.right - XTP_DPI_X(1), rc.bottom - nBottomBorder);
				pImage->DrawImage(pDC, rcDark, rcSrc, CRect(0, 0, 0, 0));
			}
		}
		else
		{
			CRect rcLight(rc.left + XTP_DPI_X(1), rc.bottom - nStatusHeight - nBottomBorder,
						  rc.left + nLeftBorder, rc.bottom - nBottomBorder + XTP_DPI_Y(1));
			pDC->FillSolidRect(rcLight,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);

			CRect rcDark(rc.right - nRightBorder, rc.bottom - nStatusHeight - nBottomBorder,
						 rc.right - XTP_DPI_X(1), rc.bottom - nBottomBorder + XTP_DPI_Y(1));
			pDC->FillSolidRect(rcDark,
							   bActive ? m_clrFrameBorderActive2 : m_clrFrameBorderInactive2);

			if (nBottomBorder > XTP_DPI_Y(1))
			{
				pDC->FillSolidRect(rc.left + nLeftBorder - XTP_DPI_X(1),
								   rc.bottom - nStatusHeight - nBottomBorder, XTP_DPI_X(1),
								   nStatusHeight,
								   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
				pDC->FillSolidRect(rc.right - nRightBorder,
								   rc.bottom - nStatusHeight - nBottomBorder, XTP_DPI_X(1),
								   nStatusHeight,
								   bActive ? m_clrFrameBorderActive3 : m_clrFrameBorderInactive3);
			}
		}
	}

	if (bHasStatusBar || m_bRoundedCornersAlways)
	{
		CXTPResourceImage* pImage;
		CRect rcSrc;

		if (nLeftBorder > XTP_DPI_X(3))
		{
			pImage = LoadImage(_T("FRAMEBOTTOMLEFT"));
			if (pImage)
			{
				rcSrc = pImage->GetSource(bActive ? 0 : 1, 2);
				CRect rcBottomLeft(rc.left, rc.bottom - XTP_DPI_Y(rcSrc.Height()),
								   rc.left + XTP_DPI_X(rcSrc.Width()), rc.bottom);
				pImage->DrawImage(pDC, rcBottomLeft, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
			}
		}

		if (nRightBorder > XTP_DPI_X(3))
		{
			pImage = LoadImage(_T("FRAMEBOTTOMRIGHT"));
			if (pImage)
			{
				rcSrc = pImage->GetSource(bActive ? 0 : 1, 2);
				CRect rcBottomRight(rc.right - XTP_DPI_X(rcSrc.Width()),
									rc.bottom - XTP_DPI_Y(rcSrc.Height()), rc.right, rc.bottom);
				pImage->DrawImage(pDC, rcBottomRight, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
			}
		}
	}
}

void CXTPRibbonOffice2007FrameTheme::DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite,
													 BOOL bActive)
{
	CXTPFramePaintManager::DrawCaptionText(pDC, rcCaptionText, pSite, bActive);
}

void CXTPRibbonOffice2007FrameTheme::DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite,
													 BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	CXTPFramePaintManager::DrawCaptionText(pDC, rcCaptionText, pSite, bActive, pRibbonBar);
}

void CXTPRibbonOffice2007FrameTheme::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
															BOOL bSelected, BOOL bPressed,
															BOOL bActive)
{
	DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive, NULL);
}

void CXTPRibbonOffice2007FrameTheme::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
															BOOL bSelected, BOOL bPressed,
															BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	UNREFERENCED_PARAMETER(pRibbonBar);

	int nState = !bActive ? 3 : bPressed && bSelected ? 2 : bSelected || bPressed ? 1 : 0;

	int nGlyphSize = 23;
	if (rc.Width() < XTP_DPI_X(27))
		nGlyphSize = 17;
	if (rc.Width() < XTP_DPI_X(15))
		nGlyphSize = 13;

	LPCTSTR lpszButton = NULL;

	switch (nId)
	{
		case SC_CLOSE: lpszButton = _T("CLOSE"); break;
		case SC_MINIMIZE: lpszButton = _T("MINIMIZE"); break;
		case SC_MAXIMIZE: lpszButton = _T("MAXIMIZE"); break;
		case SC_RESTORE: lpszButton = _T("RESTORE"); break;
	}

	if (bSelected || bPressed)
	{
		CString strImage;
		strImage.Format(_T("FRAMECAPTION%sBUTTON"), lpszButton);
		CXTPResourceImage* pImage = LoadImage(strImage);

		if (!pImage)
			pImage = LoadImage(_T("FRAMECAPTIONBUTTON"));
		pImage->DrawImage(pDC, rc, pImage->GetSource(bPressed && bSelected ? 1 : 0, 2),
						  CRect(3, 3, 3, 3), 0xFF00FF);
	}

	CString strImage;
	strImage.Format(_T("FRAMECAPTION%s%i"), lpszButton, nGlyphSize);
	CXTPResourceImage* pImage = LoadImage(strImage);

	CRect rcSrc(pImage->GetSource(nState, 5));

	CRect rcGlyph(CPoint((rc.right + rc.left - XTP_DPI_X(rcSrc.Width())) / 2,
						 (rc.top + rc.bottom - XTP_DPI_Y(rcSrc.Height())) / 2),
				  XTP_DPI(rcSrc.Size()));

	pImage->DrawImage(pDC, rcGlyph, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
}

CRect CXTPRibbonOffice2007FrameTheme::CalcFrameBorder(
	const CXTPCommandBarsFrameHook* pFrameHook) const
{
	DWORD dwStyle = pFrameHook->GetSiteStyle();

	if ((WS_MAXIMIZE & dwStyle) || pFrameHook->IsDwmEnabled())
	{
		return CXTPFramePaintManager::CalcFrameBorder(pFrameHook);
	}
	else
	{
		return XTP_DPI(CRect(4, 4, 4, 4));
	}
}
