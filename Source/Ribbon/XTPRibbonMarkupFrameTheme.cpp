// XTPRibbonMarkupFrameTheme.cpp : implementation of the CXTPRibbonMarkupFrameTheme class.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/FrameShadow/XTPFrameShadow.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"

#include "Ribbon/XTPRibbonMarkupFrameTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPRibbonMarkupFrameTheme, CXTPFramePaintManager);

CXTPRibbonMarkupFrameTheme::CXTPRibbonMarkupFrameTheme(CXTPPaintManager* pPaintManager)
	: CXTPFramePaintManager(pPaintManager)
	, m_bMarkupPartsLoaded(FALSE)
{
}

CXTPRibbonMarkupFrameTheme::~CXTPRibbonMarkupFrameTheme()
{
}

void CXTPRibbonMarkupFrameTheme::DrawFrame(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook)
{
	CString strCompat;

	CXTPResourceImages* pImages = XTPResourceImages();
	if (NULL != pImages)
	{
		strCompat = pImages->GetImageString(_T("Theme"), _T("Compatibility"));
	}

	XTPPaintTheme paintTheme = m_pPaintManager->GetCurrentTheme();
	switch (paintTheme)
	{
		case xtpThemeResource:
		case xtpThemeRibbon: { return;
		}
		case xtpThemeOffice2013:
		{
			if (strCompat != _T("Office2013"))
				return;
			break;
		}
		case xtpThemeVisualStudio2012:
		case xtpThemeVisualStudio2012Light:
		case xtpThemeVisualStudio2012Dark:
		{
			if (strCompat != _T("VisualStudio2012"))
				return;
			break;
		}
		case xtpThemeVisualStudio2015:
		{
			if (strCompat != _T("VisualStudio2015"))
				return;
			break;
		}
		case xtpThemeVisualStudio2017:
		{
			if (strCompat != _T("VisualStudio2017"))
				return;
			break;
		}
		case xtpThemeVisualStudio2019:
		{
			if (strCompat != _T("VisualStudio2019"))
				return;
			break;
		}
		case xtpThemeVisualStudio2022:
		{
			if (strCompat != _T("VisualStudio2022"))
				return;
			break;
		}
		case xtpThemeNativeWindows10:
		{
			if (strCompat != _T("NativeWindows10"))
				return;
			break;
		}
	}

	BOOL bActive = pFrameHook->IsFrameActive();

	if (!pFrameHook->IsCaptionVisible())
	{
		DrawFrameBorder(pDC, pFrameHook);
	}
	else
	{
		CRect rcCaption = CalcFrameCaption(pFrameHook);

		CRgn rgnCaption;
		rgnCaption.CreateRectRgn(rcCaption.left, rcCaption.top, rcCaption.right, rcCaption.bottom);

		pDC->SelectClipRgn(&rgnCaption, RGN_XOR);
		DrawFrameBorder(pDC, pFrameHook);
		pDC->SelectClipRgn(NULL);
		rgnCaption.DeleteObject();

		CXTPBufferDC dc(*pDC, rcCaption);

		if (!pFrameHook->IsDwmEnabled())
			DrawCaptionBar(&dc, rcCaption, pFrameHook->GetSite(), bActive);

		DrawFrameIcon(&dc, pFrameHook, rcCaption);
		DrawFrameCaptionButtons(&dc, pFrameHook, rcCaption);

		rcCaption.left += pFrameHook->CalcFrameBorder().left;
		DrawCaptionText(&dc, rcCaption, pFrameHook->GetSite(), bActive, pFrameHook->m_pRibbonBar);
	}
}

void CXTPRibbonMarkupFrameTheme::DrawFrameBorder(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook)
{
	CWnd* pSite  = pFrameHook->GetSite();
	BOOL bActive = pFrameHook->IsFrameActive();

	if (!bActive && NULL != pFrameHook->GetShadow()
		&& ::GetActiveWindow() == pFrameHook->GetShadow()->m_hWnd)
	{
		bActive = TRUE;
	}

	CXTPWindowRect rcWindow(pSite);
	CXTPClientRect rcClient(pSite);
	pSite->ClientToScreen(rcClient);

	rcClient.OffsetRect(-rcWindow.TopLeft());
	rcWindow.OffsetRect(-rcWindow.TopLeft());

	const int nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	CXTPMarkupThemePart* pPart;

	pPart = GetFrameBorder(xtpBorderPartTop, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderTop(rcWindow.left, rcWindow.top, rcWindow.right, rcClient.top);
		pPart->RenderMarkup(pDC, rcBorderTop);
	}

	pPart = GetFrameBorder(xtpBorderPartLeftCaption, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderLeftCaption(rcWindow.left, rcClient.top, rcClient.left,
								  rcClient.top + nCaptionHeight);
		pPart->RenderMarkup(pDC, rcBorderLeftCaption);
	}

	pPart = GetFrameBorder(xtpBorderPartLeft, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderLeft(rcWindow.left, rcClient.top + nCaptionHeight, rcClient.left,
						   rcClient.bottom);
		pPart->RenderMarkup(pDC, rcBorderLeft);
	}

	pPart = GetFrameBorder(xtpBorderPartRightCaption, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderRightCaption(rcClient.right, rcClient.top, rcWindow.right,
								   rcClient.top + nCaptionHeight);
		pPart->RenderMarkup(pDC, rcBorderRightCaption);
	}

	pPart = GetFrameBorder(xtpBorderPartRight, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderRight(rcClient.right, rcClient.top + nCaptionHeight, rcWindow.right,
							rcClient.bottom);
		pPart->RenderMarkup(pDC, rcBorderRight);
	}

	pPart = GetFrameBorder(xtpBorderPartBottom, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderBottom(rcWindow.left, rcClient.bottom, rcWindow.right, rcWindow.bottom);
		pPart->RenderMarkup(pDC, rcBorderBottom);
	}
}

void CXTPRibbonMarkupFrameTheme::ReloadMarkupParts(HZIP hZip, CXTPMarkupContext* pMarkupContext)
{
	m_bMarkupPartsLoaded = FALSE;
	LoadParts(hZip, pMarkupContext);
}
