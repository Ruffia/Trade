// XTPProgressCtrlPaintManager.cpp
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

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlPaintManager
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlPaintManager::CXTPProgressCtrlPaintManager()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeDefault;
}

CXTPProgressCtrlPaintManager::~CXTPProgressCtrlPaintManager()
{
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlPaintManager, CXTPCmdTarget)

void CXTPProgressCtrlPaintManager::RefreshMetrics()
{
	m_winTheme.OpenThemeData(NULL, L"PROGRESS");

	m_clrBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrBar.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrBarText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
}

void CXTPProgressCtrlPaintManager::DrawNcBorders(CDC* pDC, CRect rc)
{
	pDC->Draw3dRect(rc, m_clrBorder, m_clrBorder);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, m_clrBack, m_clrBack);
}

void CXTPProgressCtrlPaintManager::DeflateRect(XTPPROGRESSDRAWSTRUCT& ds)
{
	UNUSED_ALWAYS(ds.bSmooth);

	if (UsingWinThemes())
	{
		if (ds.bVertical)
			ds.rcClient.DeflateRect(3, 4);
		else
			ds.rcClient.DeflateRect(4, 3);
	}
	else if (!ds.bFlatStyle)
	{
		//  give 1 pixel around the bar
		ds.rcClient.DeflateRect(1, 1);
	}
}

void CXTPProgressCtrlPaintManager::Draw(XTPPROGRESSDRAWSTRUCT& ds)
{
	ASSERT_VALID(ds.pWndCtrl);

	if (UsingWinThemes())
	{
		m_winTheme.DrawThemeBackground(*ds.pDC, ds.bVertical ? PP_BARVERT : PP_BAR, 0, ds.rcClient,
									   NULL);
	}
	else
	{
		if (!ds.bFlatStyle)
		{
			DrawNcBorders(ds.pDC, ds.rcClient);
			ds.rcClient.DeflateRect(2, 2);
		}

		ds.pDC->FillSolidRect(ds.rcClient, m_clrBack);
	}

	int x, dxSpace, dxBlock, nBlocks;
	int iStart, iEnd;

	DeflateRect(ds);

	CRect rc = ds.rcClient;

	if (ds.bMarquee)
	{
		dxBlock = rc.Height() * 2 / 3;
		dxSpace = (ds.bVista || ds.bSmooth) ? 0 : 2;

		if (UsingWinThemes())
		{
			m_winTheme.GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, &dxBlock);
			m_winTheme.GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, &dxSpace);
		}

		rc.left = ds.nPos + ds.rcClient.left;

		for (int i = 0; i < 5; i++)
		{
			if (rc.left >= ds.rcClient.right)
				rc.left = ds.rcClient.left;

			rc.right = rc.left + dxBlock;

			if (rc.right > ds.rcClient.right)
				rc.right = ds.rcClient.right;

			if (UsingWinThemes())
			{
				m_winTheme.DrawThemeBackground(*ds.pDC, PP_CHUNK, 0, rc, NULL);
			}
			else
			{
				ds.pDC->FillSolidRect(rc, m_clrBar);
			}
			rc.left = rc.right + dxSpace;
		}
	}
	else
	{
		int iLow  = ds.range.iLow;
		int iHigh = ds.range.iHigh;

		if (ds.bVertical)
		{
			iStart  = rc.top;
			iEnd	= rc.bottom;
			dxBlock = (rc.right - rc.left) * 2 / 3;
		}
		else
		{
			iStart  = rc.left;
			iEnd	= rc.right;
			dxBlock = (rc.bottom - rc.top) * 2 / 3;
		}

		x = ::MulDiv(iEnd - iStart, ds.nPos - iLow, iHigh - iLow);

		dxSpace = 2;

		if (UsingWinThemes())
		{
			m_winTheme.GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, &dxBlock);
			m_winTheme.GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, &dxSpace);
		}

		if (dxBlock == 0)
			dxBlock = 1; // avoid div by zero

		if (!UsingWinThemes() && ds.bSmooth)
		{
			dxBlock = 1;
			dxSpace = 0;
		}

		nBlocks = (x + (dxBlock + dxSpace) - 1) / (dxBlock + dxSpace); // round up

		for (int i = 0; i < nBlocks; i++)
		{
			if (ds.bVertical)
			{
				rc.top = rc.bottom - dxBlock;

				// are we past the end?
				if (rc.bottom <= ds.rcClient.top)
					break;

				if (rc.top <= ds.rcClient.top)
					rc.top = ds.rcClient.top + 1;
			}
			else
			{
				rc.right = rc.left + dxBlock;

				// are we past the end?
				if (rc.left >= ds.rcClient.right)
					break;

				if (rc.right >= ds.rcClient.right)
					rc.right = ds.rcClient.right - 1;
			}

			if (UsingWinThemes())
			{
				m_winTheme.DrawThemeBackground(*ds.pDC, ds.bVertical ? PP_CHUNKVERT : PP_CHUNK, 0,
											   rc, NULL);
			}
			else
			{
				ds.pDC->FillSolidRect(&rc, m_clrBar);
			}

			if (ds.bVertical)
			{
				rc.bottom = rc.top - dxSpace;
			}
			else
			{
				rc.left = rc.right + dxSpace;
			}
		}
	}

	DrawText(ds);
}

void CXTPProgressCtrlPaintManager::DrawText(XTPPROGRESSDRAWSTRUCT& ds)
{
	if (!ds.strText.IsEmpty() && !ds.bVertical)
	{
		ASSERT_VALID(ds.pWndCtrl);

		int nLower, nUpper;
		ds.pWndCtrl->GetRange(nLower, nUpper);

		CRect rcLeft, rcRight;
		rcLeft = rcRight = ds.rcClient;

		double dPos  = (double)(ds.pWndCtrl->GetPos() - nLower) / ((double)(nUpper - nLower));
		rcLeft.right = rcLeft.left + (int)((rcLeft.right - rcLeft.left) * dPos);
		rcRight.left = rcLeft.right;

		CXTPFontDC font(ds.pDC, ds.pFont);
		ds.pDC->SetBkMode(TRANSPARENT);

		CRgn rgnLeft;
		rgnLeft.CreateRectRgn(rcLeft.left, rcLeft.top, rcLeft.right, rcLeft.bottom);
		ds.pDC->SelectClipRgn(&rgnLeft);
		ds.pDC->SetTextColor(m_clrBarText);
		ds.pDC->DrawText(ds.strText, ds.rcClient,
						 DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);

		CRgn rgnRight;
		rgnRight.CreateRectRgn(rcRight.left, rcRight.top, rcRight.right, rcRight.bottom);
		ds.pDC->SelectClipRgn(&rgnRight);
		ds.pDC->SetTextColor(m_clrText);
		ds.pDC->DrawText(ds.strText, ds.rcClient,
						 DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
	}
}

BOOL CXTPProgressCtrlPaintManager::UsingWinThemes(BOOL bCheckReady /*=FALSE*/)
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

void CXTPProgressCtrlPaintManager::UseWinThemes(BOOL bUseWinThemes /*=TRUE*/)
{
	m_bUseWinTheme = bUseWinThemes;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeFlat
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeFlat::CXTPProgressCtrlThemeFlat()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeFlat;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeFlat, CXTPProgressCtrlPaintManager)

void CXTPProgressCtrlThemeFlat::RefreshMetrics()
{
	CXTPProgressCtrlPaintManager::RefreshMetrics();

	m_clrBorder.SetStandardValue(::GetSysColor(COLOR_WINDOWFRAME));
}

void CXTPProgressCtrlThemeFlat::DeflateRect(XTPPROGRESSDRAWSTRUCT& ds)
{
	UNUSED_ALWAYS(ds);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeUltraFlat
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeUltraFlat::CXTPProgressCtrlThemeUltraFlat()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeUltraFlat;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeUltraFlat, CXTPProgressCtrlThemeFlat)

void CXTPProgressCtrlThemeUltraFlat::RefreshMetrics()
{
	CXTPProgressCtrlThemeFlat::RefreshMetrics();

	m_clrBorder.SetStandardValue(m_clrBack);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeOffice2000
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeOffice2000::CXTPProgressCtrlThemeOffice2000()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOffice2000;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeOffice2000, CXTPProgressCtrlPaintManager)

void CXTPProgressCtrlThemeOffice2000::DrawNcBorders(CDC* pDC, CRect rc)
{
	pDC->Draw3dRect(rc, m_clrBorder, GetXtremeColor(COLOR_BTNHIGHLIGHT));
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, m_clrBack, m_clrBack);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeOfficeXP
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeOfficeXP::CXTPProgressCtrlThemeOfficeXP()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOfficeXP;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeOfficeXP, CXTPProgressCtrlThemeOffice2000)

void CXTPProgressCtrlThemeOfficeXP::DrawNcBorders(CDC* pDC, CRect rc)
{
	CXTPProgressCtrlPaintManager::DrawNcBorders(pDC, rc);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeOffice2003
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeOffice2003::CXTPProgressCtrlThemeOffice2003()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOffice2003;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeOffice2003, CXTPProgressCtrlThemeOffice2000)

void CXTPProgressCtrlThemeOffice2003::RefreshMetrics()
{
	CXTPProgressCtrlPaintManager::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
	switch (systemTheme)
	{
		case xtpSystemThemeOlive: m_clrBorder.SetStandardValue(RGB(164, 185, 127)); break;

		case xtpSystemThemeSilver: m_clrBorder.SetStandardValue(RGB(165, 172, 178)); break;

		default: m_clrBorder.SetStandardValue(RGB(127, 157, 185)); break;
	}
}

void CXTPProgressCtrlThemeOffice2003::DrawNcBorders(CDC* pDC, CRect rc)
{
	CXTPProgressCtrlPaintManager::DrawNcBorders(pDC, rc);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeResource
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeResource::CXTPProgressCtrlThemeResource()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeResource;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeResource, CXTPProgressCtrlThemeOffice2000)

void CXTPProgressCtrlThemeResource::Draw(XTPPROGRESSDRAWSTRUCT& ds)
{
	CXTPResourceImage* pImageTrack = XTPLoadImage(_T("PROGRESSTRACK"));
	CXTPResourceImage* pImageChunk = XTPLoadImage(_T("PROGRESSCHUNK"));

	if (!pImageTrack || !pImageChunk)
	{
		ASSERT(FALSE);
		CXTPProgressCtrlThemeOffice2000::Draw(ds);
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

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeOffice2013
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeOffice2013::CXTPProgressCtrlThemeOffice2013()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeOffice2013;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeOffice2013, CXTPProgressCtrlThemeOffice2000)

void CXTPProgressCtrlThemeOffice2013::DrawNcBorders(CDC* pDC, CRect rc)
{
	CXTPProgressCtrlPaintManager::DrawNcBorders(pDC, rc);
}

void CXTPProgressCtrlThemeOffice2013::RefreshMetrics()
{
	m_clrBorder.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Border"), RGB(188, 188, 188)));
	m_clrBar.SetStandardValue(XTPIniColor(_T("Controls.ProgressBar"), _T("Bar"), RGB(6, 176, 37)));
	m_clrBack.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Back"), RGB(230, 230, 230)));
	m_clrText.SetStandardValue(::GetSysColor(COLOR_BTNTEXT));
	m_clrBarText.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Text"), RGB(68, 68, 68)));
}

void CXTPProgressCtrlThemeOffice2013::Draw(XTPPROGRESSDRAWSTRUCT& ds)
{
	CXTPProgressCtrlThemeOffice2000::Draw(ds);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeVisualStudio2015
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeVisualStudio2015::CXTPProgressCtrlThemeVisualStudio2015()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeVisualStudio2015;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2015, CXTPProgressCtrlThemeOffice2013)

void CXTPProgressCtrlThemeVisualStudio2015::RefreshMetrics()
{
	CXTPProgressCtrlThemeOffice2013::RefreshMetrics();

	m_clrText.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Text"), RGB(68, 68, 68)));
	m_clrBarText.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHTTEXT));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeNativeWinXP
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeNativeWinXP::CXTPProgressCtrlThemeNativeWinXP()
{
	m_bUseWinTheme = TRUE;
	m_nTheme	   = xtpControlThemeNativeWinXP;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeNativeWinXP, CXTPProgressCtrlPaintManager)

void CXTPProgressCtrlThemeNativeWinXP::DeflateRect(XTPPROGRESSDRAWSTRUCT& ds)
{
	if (ds.bVertical)
		ds.rcClient.DeflateRect(3, 4);
	else
		ds.rcClient.DeflateRect(4, 3);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeNativeWindows
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeNativeWindows::CXTPProgressCtrlThemeNativeWindows()
{
	m_bUseWinTheme = TRUE;
	m_nTheme	   = xtpControlThemeNativeWindows10;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeNativeWindows, CXTPProgressCtrlPaintManager)

void CXTPProgressCtrlThemeNativeWindows::DeflateRect(XTPPROGRESSDRAWSTRUCT& ds)
{
	UNUSED_ALWAYS(ds);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeVisualStudio2017
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeVisualStudio2017::CXTPProgressCtrlThemeVisualStudio2017()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeVisualStudio2017;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2017, CXTPProgressCtrlThemeOffice2013)

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeVisualStudio2019
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeVisualStudio2019::CXTPProgressCtrlThemeVisualStudio2019()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeVisualStudio2019;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2019, CXTPProgressCtrlThemeOffice2013)

/////////////////////////////////////////////////////////////////////////////
// CXTPProgressCtrlThemeVisualStudio2022
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrlThemeVisualStudio2022::CXTPProgressCtrlThemeVisualStudio2022()
{
	m_bUseWinTheme = FALSE;
	m_nTheme	   = xtpControlThemeVisualStudio2022;
}

IMPLEMENT_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2022, CXTPProgressCtrlThemeOffice2013)
