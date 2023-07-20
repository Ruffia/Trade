// XTPMarqueeCtrlPaintManager.cpp
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
#include "Common/XTPWinThemeWrapper.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Progress/XTPProgressCtrlPaintManager.h"
#include "Controls/Progress/XTPMarqueeCtrlPaintManager.h"
#include "Controls/Progress/XTPMarqueeCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlPaintManager
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlPaintManager::CXTPMarqueeCtrlPaintManager()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeDefault;
}

CXTPMarqueeCtrlPaintManager::~CXTPMarqueeCtrlPaintManager()
{
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlPaintManager, CCmdTarget)

void CXTPMarqueeCtrlPaintManager::RefreshMetrics()
{
	m_winTheme.OpenThemeData(NULL, L"PROGRESS");
	m_clr3DHilite.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clr3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrBar.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
}

void CXTPMarqueeCtrlPaintManager::DrawBackground(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC,
												 CRect& rcClient)
{
	if (UsingWinThemes())
	{
		m_winTheme.DrawThemeBackground(pDC->GetSafeHdc(),
									   pWndCtrl->IsVertical() ? PP_BARVERT : PP_BAR, PBS_NORMAL,
									   &rcClient, NULL);
	}
	else
	{
		pDC->Draw3dRect(rcClient, m_clr3DShadow, m_clr3DHilite);
		rcClient.DeflateRect(1, 1);
		pDC->FillSolidRect(&rcClient, m_clrBack);
	}
}

void CXTPMarqueeCtrlPaintManager::DrawChunk(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC,
											CXTPCompatibleDC& dcChunk, int i, int& x, int& y,
											int cx, int cy)
{
	dcChunk.BitBlt(0, 0, cx, cy, pDC, x, y, SRCCOPY);

	// draw the chunk using the memory dc.
	CXTPEmptyRect rcChunk;

	if (pWndCtrl->IsVertical())
	{
		rcChunk.right  = cx;
		rcChunk.bottom = pWndCtrl->m_cxChunk - (pWndCtrl->IsSmooth() ? 0 : pWndCtrl->m_nGap);
	}
	else
	{
		rcChunk.right  = pWndCtrl->m_cxChunk - (pWndCtrl->IsSmooth() ? 0 : pWndCtrl->m_nGap);
		rcChunk.bottom = cy;
	}

	if (UsingWinThemes())
	{
		m_winTheme.DrawThemeBackground(dcChunk, pWndCtrl->IsVertical() ? PP_CHUNKVERT : PP_CHUNK,
									   PBS_NORMAL, &rcChunk, NULL);
	}
	else
	{
		dcChunk.FillSolidRect(rcChunk, m_clrBar);
	}

	if (XTPDrawHelpers()->m_pfnAlphaBlend)
	{
		BLENDFUNCTION bf;
		bf.BlendOp			   = AC_SRC_OVER;
		bf.BlendFlags		   = 0;
		bf.SourceConstantAlpha = pWndCtrl->m_chAlpha[i];
		bf.AlphaFormat		   = 0;

		XTPDrawHelpers()->m_pfnAlphaBlend(pDC->GetSafeHdc(), x, y, cx, cy, dcChunk, 0, 0, cx, cy,
										  bf);
	}
	else
	{
		::BitBlt(pDC->GetSafeHdc(), x, y, cx, cy, dcChunk, 0, 0, SRCCOPY);
	}

	if (pWndCtrl->IsVertical())
	{
		y += cy;
	}
	else
	{
		x += cx;
	}
}

void CXTPMarqueeCtrlPaintManager::DrawMarquee(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CRect& rcClient)
{
	ASSERT_VALID(pWndCtrl);

	DeflateRect(pDC, rcClient);

	// calculate marquee size.
	int x = pWndCtrl->IsVertical() ? rcClient.left : pWndCtrl->m_nPos;
	int y = pWndCtrl->IsVertical() ? pWndCtrl->m_nPos : rcClient.top;

	// determine the dimensions for the bitmap.
	int cx = pWndCtrl->IsVertical() ? rcClient.Width() : pWndCtrl->m_cxChunk;
	int cy = pWndCtrl->IsVertical() ? pWndCtrl->m_cxChunk : rcClient.Height();

	// create a memory device context, and select the bitmap into it.
	CBitmap bmpChunk;
	bmpChunk.CreateCompatibleBitmap(pDC, cx, cy);

	CXTPCompatibleDC dcChunk(pDC, &bmpChunk);

	for (int i = 0; i < _countof(pWndCtrl->m_chAlpha); ++i)
	{
		DrawChunk(pWndCtrl, pDC, dcChunk, i, x, y, cx, cy);
	}
}

void CXTPMarqueeCtrlPaintManager::DeflateRect(CDC* pDC, CRect& rcClient)
{
	int nEdge = (UsingWinThemes() ? 3 : 2);
	rcClient.DeflateRect(XTP_DPI_X(nEdge), XTP_DPI_Y(nEdge));
	pDC->IntersectClipRect(rcClient);
}

BOOL CXTPMarqueeCtrlPaintManager::UsingWinThemes(BOOL bCheckReady /*=FALSE*/)
{
	if (bCheckReady)
	{
		return (m_bUseWinTheme && m_winTheme.IsAppThemed() && m_winTheme.IsAppThemeReady());
	}
	else
	{
		return (m_bUseWinTheme && m_winTheme.IsAppThemed());
	}
}

void CXTPMarqueeCtrlPaintManager::UseWinThemes(BOOL bUseWinThemes /*=TRUE*/)
{
	m_bUseWinTheme = bUseWinThemes;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeFlat
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeFlat::CXTPMarqueeCtrlThemeFlat()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeFlat;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeFlat, CXTPMarqueeCtrlPaintManager)

void CXTPMarqueeCtrlThemeFlat::RefreshMetrics()
{
	CXTPMarqueeCtrlPaintManager::RefreshMetrics();
	m_clr3DHilite.SetStandardValue(::GetSysColor(COLOR_WINDOWFRAME));
	m_clr3DShadow.SetStandardValue(::GetSysColor(COLOR_WINDOWFRAME));
}

void CXTPMarqueeCtrlThemeFlat::DeflateRect(CDC* pDC, CRect& rcClient)
{
	rcClient.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	pDC->IntersectClipRect(rcClient);
}

void CXTPMarqueeCtrlThemeFlat::DrawMarquee(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CRect& rcClient)
{
	CXTPMarqueeCtrlPaintManager::DrawMarquee(pWndCtrl, pDC, rcClient);
	pDC->Draw3dRect(rcClient, m_clrBack, m_clrBack);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeUltraFlat
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeUltraFlat::CXTPMarqueeCtrlThemeUltraFlat()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeUltraFlat;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeUltraFlat, CXTPMarqueeCtrlThemeFlat)

void CXTPMarqueeCtrlThemeUltraFlat::RefreshMetrics()
{
	CXTPMarqueeCtrlThemeFlat::RefreshMetrics();
	m_clr3DShadow.SetStandardValue(m_clrBack);
	m_clr3DHilite.SetStandardValue(m_clrBack);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeOffice2000
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeOffice2000::CXTPMarqueeCtrlThemeOffice2000()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOffice2000;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeOffice2000, CXTPMarqueeCtrlPaintManager)

void CXTPMarqueeCtrlThemeOffice2000::RefreshMetrics()
{
	CXTPMarqueeCtrlPaintManager::RefreshMetrics();
	m_clr3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clr3DHilite.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeOfficeXP
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeOfficeXP::CXTPMarqueeCtrlThemeOfficeXP()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOfficeXP;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeOfficeXP, CXTPMarqueeCtrlThemeOffice2000)

void CXTPMarqueeCtrlThemeOfficeXP::RefreshMetrics()
{
	CXTPMarqueeCtrlPaintManager::RefreshMetrics();
	m_clr3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clr3DHilite.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeOffice2003
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeOffice2003::CXTPMarqueeCtrlThemeOffice2003()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOffice2003;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeOffice2003, CXTPMarqueeCtrlThemeOffice2000)

void CXTPMarqueeCtrlThemeOffice2003::RefreshMetrics()
{
	CXTPMarqueeCtrlPaintManager::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
	switch (systemTheme)
	{
		case xtpSystemThemeOlive:
			m_clr3DHilite.SetStandardValue(RGB(164, 185, 127));
			m_clr3DShadow.SetStandardValue(RGB(164, 185, 127));
			break;

		case xtpSystemThemeSilver:
			m_clr3DHilite.SetStandardValue(RGB(165, 172, 178));
			m_clr3DShadow.SetStandardValue(RGB(165, 172, 178));
			break;

		default:
			m_clr3DHilite.SetStandardValue(RGB(127, 157, 185));
			m_clr3DShadow.SetStandardValue(RGB(127, 157, 185));
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeResource
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeResource::CXTPMarqueeCtrlThemeResource()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeResource;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeResource, CXTPMarqueeCtrlThemeOffice2000)

void CXTPMarqueeCtrlThemeResource::DrawBackground(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC,
												  CRect& rcClient)
{
	CXTPResourceImage* pImageTrack = XTPLoadImage(_T("PROGRESSTRACK"));
	if (pImageTrack)
	{
		pDC->FillSolidRect(rcClient, m_clrBack);
		pImageTrack->DrawImage(pDC, rcClient, pImageTrack->GetSource(), CRect(2, 2, 2, 2),
							   0xff00ff);
	}
	else
	{
		CXTPMarqueeCtrlThemeOffice2000::DrawBackground(pWndCtrl, pDC, rcClient);
	}
}

void CXTPMarqueeCtrlThemeResource::DrawChunk(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC,
											 CXTPCompatibleDC& dcChunk, int i, int& x, int& y,
											 int cx, int cy)
{
	dcChunk.BitBlt(0, 0, cx, cy, pDC, x, y, SRCCOPY);

	// draw the chunk using the memory dc.
	CXTPEmptyRect rcChunk;

	if (pWndCtrl->IsVertical())
	{
		rcChunk.right  = cx;
		rcChunk.bottom = pWndCtrl->m_cxChunk - (pWndCtrl->IsSmooth() ? 0 : pWndCtrl->m_nGap);
	}
	else
	{
		rcChunk.right  = pWndCtrl->m_cxChunk - (pWndCtrl->IsSmooth() ? 0 : pWndCtrl->m_nGap);
		rcChunk.bottom = cy;
	}

	if (UsingWinThemes())
	{
		m_winTheme.DrawThemeBackground(dcChunk, pWndCtrl->IsVertical() ? PP_CHUNKVERT : PP_CHUNK,
									   PBS_NORMAL, &rcChunk, NULL);
	}
	else
	{
		CXTPResourceImage* pImageChunk = XTPLoadImage(_T("PROGRESSCHUNK"));
		if (pImageChunk)
		{
			CRect rcSource = pImageChunk->GetSource();
			rcSource.right = rcChunk.right;

			pImageChunk->DrawImage(&dcChunk, rcChunk, rcSource, CRect(2, 2, 2, 2), 0xff00ff);
		}
		else
		{
			dcChunk.FillSolidRect(rcChunk, m_clrBar);
		}
	}

	if (XTPDrawHelpers()->m_pfnAlphaBlend)
	{
		BLENDFUNCTION bf;
		bf.BlendOp			   = AC_SRC_OVER;
		bf.BlendFlags		   = 0;
		bf.SourceConstantAlpha = pWndCtrl->m_chAlpha[i];
		bf.AlphaFormat		   = 0;

		XTPDrawHelpers()->m_pfnAlphaBlend(pDC->GetSafeHdc(), x, y, cx, cy, dcChunk, 0, 0, cx, cy,
										  bf);
	}
	else
	{
		::BitBlt(pDC->GetSafeHdc(), x, y, cx, cy, dcChunk, 0, 0, SRCCOPY);
	}

	if (pWndCtrl->IsVertical())
	{
		y += cy;
	}
	else
	{
		x += cx;
	}
}

/*
void CXTPMarqueeCtrlThemeResource::Draw(XTPPROGRESSDRAWSTRUCT& ds)
{
	CXTPResourceImage* pImageTrack = XTPLoadImage(_T("PROGRESSTRACK"));
	CXTPResourceImage* pImageChunk = XTPLoadImage(_T("PROGRESSCHUNK"));

	if (!pImageTrack || !pImageChunk)
	{
		ASSERT(FALSE);
		CXTPMarqueeCtrlThemeOffice2000::Draw(ds);
		return;
	}

	ds.pDC->FillSolidRect(ds.rcClient, m_clrBack);
	pImageTrack->DrawImage(ds.pDC, ds.rcClient, pImageTrack->GetSource(), CRect(2, 2, 2, 2),
						   0xff00ff);

	ds.rcClient.DeflateRect(2, 2);

	int x = ::MulDiv(ds.rcClient.Width(), ds.nPos - ds.range.iLow, ds.range.iHigh - ds.range.iLow);

	CRect rcSrc(pImageChunk->GetSource());
	rcSrc.right -= 4;

	CRect rcDest(ds.rcClient.left, ds.rcClient.top, ds.rcClient.left + x, ds.rcClient.bottom);

	if (rcDest.Width() < rcSrc.Width())
		rcSrc.left = rcSrc.right - rcDest.Width();

	pImageChunk->DrawImage(ds.pDC, rcDest, rcSrc, CRect(2, 2, 2, 2), 0xff00ff);

	if (ds.rcClient.left + x < ds.rcClient.right - 1)
	{
		int nShadow = min(4, ds.rcClient.right - ds.rcClient.left - x);
		rcSrc		= CRect(rcSrc.right, rcSrc.top, rcSrc.right + nShadow, rcSrc.bottom);

		rcDest = CRect(rcDest.right, rcDest.top, rcDest.right + nShadow, rcDest.bottom);

		pImageChunk->DrawImage(ds.pDC, rcDest, rcSrc, CRect(0, 2, 0, 2), 0xFF00FF);
	}

	DrawText(ds);
}
*/
/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeOffice2013
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeOffice2013::CXTPMarqueeCtrlThemeOffice2013()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOffice2013;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeOffice2013, CXTPMarqueeCtrlThemeOffice2000)

void CXTPMarqueeCtrlThemeOffice2013::RefreshMetrics()
{
	CXTPMarqueeCtrlPaintManager::RefreshMetrics();

	m_clr3DHilite.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Border"), RGB(188, 188, 188)));
	m_clr3DShadow.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Border"), RGB(188, 188, 188)));
	m_clrBar.SetStandardValue(XTPIniColor(_T("Controls.ProgressBar"), _T("Bar"), RGB(6, 176, 37)));
	m_clrBack.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Back"), RGB(230, 230, 230)));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeVisualStudio2015
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeVisualStudio2015::CXTPMarqueeCtrlThemeVisualStudio2015()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeVisualStudio2015;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeVisualStudio2015, CXTPMarqueeCtrlThemeOffice2013)

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeNativeWinXP
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeNativeWinXP::CXTPMarqueeCtrlThemeNativeWinXP()
{
	m_bUseWinTheme = TRUE;
	m_nTheme	   = xtpControlThemeNativeWinXP;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeNativeWinXP, CXTPMarqueeCtrlPaintManager)

void CXTPMarqueeCtrlThemeNativeWinXP::DeflateRect(XTPPROGRESSDRAWSTRUCT& ds)
{
	if (ds.bVertical)
		ds.rcClient.DeflateRect(3, 4);
	else
		ds.rcClient.DeflateRect(4, 3);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarqueeCtrlThemeNativeWindows
/////////////////////////////////////////////////////////////////////////////

CXTPMarqueeCtrlThemeNativeWindows::CXTPMarqueeCtrlThemeNativeWindows()
{
	m_bUseWinTheme = TRUE;
	m_nTheme	   = xtpControlThemeNativeWindows10;
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrlThemeNativeWindows, CXTPMarqueeCtrlPaintManager)

void CXTPMarqueeCtrlThemeNativeWindows::DeflateRect(CDC* pDC, CRect& rcClient)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcClient);
}
