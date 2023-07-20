// XTPFrameThemeOffice2007System.cpp : implementation of the CXTPFrameThemeOffice2007System class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResource.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeOffice2007System.h"

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

//////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeOffice2007System

CXTPFrameThemeOffice2007System::CXTPFrameThemeOffice2007System(CXTPPaintManager* pPaintManager)
	: CXTPRibbonMarkupFrameTheme(pPaintManager)
{
	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);

	m_pCaptionActive			  = new CXTPMarkupThemePart();
	m_pCaptionInactive			  = new CXTPMarkupThemePart();
	m_pBorderLeftActive			  = new CXTPMarkupThemePart();
	m_pBorderLeftInactive		  = new CXTPMarkupThemePart();
	m_pBorderLeftCaptionActive	= new CXTPMarkupThemePart();
	m_pBorderLeftCaptionInactive  = new CXTPMarkupThemePart();
	m_pBorderTopActive			  = new CXTPMarkupThemePart();
	m_pBorderTopInactive		  = new CXTPMarkupThemePart();
	m_pBorderRightActive		  = new CXTPMarkupThemePart();
	m_pBorderRightInactive		  = new CXTPMarkupThemePart();
	m_pBorderRightCaptionActive   = new CXTPMarkupThemePart();
	m_pBorderRightCaptionInactive = new CXTPMarkupThemePart();
	m_pBorderBottomActive		  = new CXTPMarkupThemePart();
	m_pBorderBottomInactive		  = new CXTPMarkupThemePart();

	m_pCaptionButtonSelected = new CXTPMarkupThemePart();
	m_pCaptionButtonPressed  = new CXTPMarkupThemePart();

	m_pGlyphMinimize9  = new CXTPMarkupThemePart();
	m_pGlyphMinimize11 = new CXTPMarkupThemePart();
	m_pGlyphMaximize9  = new CXTPMarkupThemePart();
	m_pGlyphMaximize11 = new CXTPMarkupThemePart();
	m_pGlyphRestore9   = new CXTPMarkupThemePart();
	m_pGlyphRestore11  = new CXTPMarkupThemePart();
	m_pGlyphClose9	 = new CXTPMarkupThemePart();
	m_pGlyphClose11	= new CXTPMarkupThemePart();

	LoadParts();
}

CXTPFrameThemeOffice2007System::~CXTPFrameThemeOffice2007System()
{
	SAFE_DELETE(m_pCaptionActive);
	SAFE_DELETE(m_pCaptionInactive);
	SAFE_DELETE(m_pBorderLeftActive);
	SAFE_DELETE(m_pBorderLeftInactive);
	SAFE_DELETE(m_pBorderLeftCaptionActive);
	SAFE_DELETE(m_pBorderLeftCaptionInactive);
	SAFE_DELETE(m_pBorderTopActive);
	SAFE_DELETE(m_pBorderTopInactive);
	SAFE_DELETE(m_pBorderRightActive);
	SAFE_DELETE(m_pBorderRightInactive);
	SAFE_DELETE(m_pBorderRightCaptionActive);
	SAFE_DELETE(m_pBorderRightCaptionInactive);
	SAFE_DELETE(m_pBorderBottomActive);
	SAFE_DELETE(m_pBorderBottomInactive);

	SAFE_DELETE(m_pCaptionButtonSelected);
	SAFE_DELETE(m_pCaptionButtonPressed);

	SAFE_DELETE(m_pGlyphMinimize9);
	SAFE_DELETE(m_pGlyphMinimize11);
	SAFE_DELETE(m_pGlyphMaximize9);
	SAFE_DELETE(m_pGlyphMaximize11);
	SAFE_DELETE(m_pGlyphRestore9);
	SAFE_DELETE(m_pGlyphRestore11);
	SAFE_DELETE(m_pGlyphClose9);
	SAFE_DELETE(m_pGlyphClose11);

	XTPMarkupReleaseContext(m_pMarkupContext);
}

void CXTPFrameThemeOffice2007System::LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext)
{
	if (m_bMarkupPartsLoaded)
		return;

	m_bMarkupPartsLoaded = TRUE;

	// Caption bar
	BOOL bMarkupLoaded = m_pCaptionActive->LoadPart(hZip, _T("RibbonFrameCaptionActive.xaml"),
													pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pCaptionInactive->LoadPart(hZip, _T("RibbonFrameCaptionInactive.xaml"),
												 pMarkupContext);
	ASSERT(bMarkupLoaded);

	// Frame border
	bMarkupLoaded = m_pBorderLeftActive->LoadPart(hZip, _T("Frame/BorderLeftActive.xaml"),
												  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderLeftInactive->LoadPart(hZip, _T("Frame/BorderLeftInactive.xaml"),
													pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderLeftCaptionActive->LoadPart(hZip,
														 _T("Frame/BorderLeftCaptionActive.xaml"),
														 pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderLeftCaptionInactive->LoadPart(
		hZip, _T("Frame/BorderLeftCaptionInactive.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderTopActive->LoadPart(hZip, _T("Frame/BorderTopActive.xaml"),
												 pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderTopInactive->LoadPart(hZip, _T("Frame/BorderTopInactive.xaml"),
												   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderRightActive->LoadPart(hZip, _T("Frame/BorderRightActive.xaml"),
												   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderRightInactive->LoadPart(hZip, _T("Frame/BorderRightInactive.xaml"),
													 pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderRightCaptionActive->LoadPart(hZip,
														  _T("Frame/BorderRightCaptionActive.xaml"),
														  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderRightCaptionInactive->LoadPart(
		hZip, _T("Frame/BorderRightCaptionInactive.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderBottomActive->LoadPart(hZip, _T("Frame/BorderBottomActive.xaml"),
													pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderBottomInactive->LoadPart(hZip, _T("Frame/BorderBottomInactive.xaml"),
													  pMarkupContext);
	ASSERT(bMarkupLoaded);

	// Frame caption
	bMarkupLoaded = m_pCaptionButtonSelected->LoadPart(hZip, _T("Frame/CaptionButtonSelected.xaml"),
													   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pCaptionButtonPressed->LoadPart(hZip, _T("Frame/CaptionButtonPressed.xaml"),
													  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphMinimize9->LoadPart(hZip, _T("Frame/GlyphMinimize9.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphMinimize11->LoadPart(hZip, _T("Frame/GlyphMinimize11.xaml"),
												 pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphMaximize9->LoadPart(hZip, _T("Frame/GlyphMaximize9.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphMaximize11->LoadPart(hZip, _T("Frame/GlyphMaximize11.xaml"),
												 pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphRestore9->LoadPart(hZip, _T("Frame/GlyphRestore9.xaml"),
											   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphRestore11->LoadPart(hZip, _T("Frame/GlyphRestore11.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphClose9->LoadPart(hZip, _T("Frame/GlyphClose9.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pGlyphClose11->LoadPart(hZip, _T("Frame/GlyphClose11.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	UNREFERENCED_PARAMETER(bMarkupLoaded);
}

BOOL CXTPFrameThemeOffice2007System::LoadResource()
{
	LoadParts();
	return TRUE;
}

void CXTPFrameThemeOffice2007System::LoadParts(BOOL bReload /*= FALSE*/)
{
	if (bReload)
		m_bMarkupPartsLoaded = FALSE;

	if (m_bMarkupPartsLoaded)
		return;

	static LPCTSTR lpszName = _T("XTP_IDR_OFFICE2007SYSTEM_STYLE");
	static LPCTSTR lpszType = _T("STYLE");

	HINSTANCE hResource = AfxFindResourceHandle(lpszName, lpszType);

	CXTPResource resource;
	resource.LoadResource(hResource, lpszName, lpszType);

	if (NULL != resource.m_hResource)
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		ASSERT(NULL != hZip); // Invalid file

		if (NULL != hZip)
			LoadParts(hZip, m_pMarkupContext);

		CloseZip(hZip);
	}
}

HRGN CXTPFrameThemeOffice2007System::CalcFrameRegion(CXTPCommandBarsFrameHook* pFrameHook,
													 CSize sz) const
{
	ASSERT(NULL != pFrameHook);

	if (pFrameHook->GetSite()->GetStyle() & WS_MAXIMIZE)
	{
		return NULL;
	}

	int cx = sz.cx, cy = sz.cy;

	RECT rgnTopFrame[] = { { 4, 0, cx - 4, 1 },
						   { 2, 1, cx - 2, 2 },
						   { 1, 2, cx - 1, 3 },
						   { 1, 3, cx - 1, 4 },
						   { 0, 4, cx, cy - 4 } };

	RECT rgnRibbonBottomFrame[] = { { 1, cy - 4, cx - 1, cy - 2 },
									{ 2, cy - 2, cx - 2, cy - 1 },
									{ 4, cy - 1, cx - 4, cy - 0 } };

	RECT rgnSimpleBottomFrame[] = { { 0, cy - 4, cx, cy } };

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

CRect CXTPFrameThemeOffice2007System::CalcFrameBorder(
	const CXTPCommandBarsFrameHook* pFrameHook) const
{
	ASSERT(NULL != pFrameHook);

	DWORD dwStyle = pFrameHook->GetSiteStyle();

	if ((WS_MAXIMIZE & dwStyle) || pFrameHook->IsDwmEnabled())
	{
		return CXTPFramePaintManager::CalcFrameBorder(pFrameHook);
	}
	else
	{
		return CRect(4, 4, 4, 4); // Custom frame theme
	}
}

void CXTPFrameThemeOffice2007System::RefreshMetrics()
{
	CXTPRibbonMarkupFrameTheme::RefreshMetrics();

	m_bFlatFrame = FALSE;

	const int nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	const int nBorderHeight  = GetSystemMetrics(SM_CYBORDER);
	m_nFrameCaptionHeight	= (nCaptionHeight - nBorderHeight);

	// Update frame caption font.
	if (NULL != m_fntFrameCaption.m_hObject)
	{
		m_fntFrameCaption.DeleteObject();
	}

	HFONT hRegularFont = (HFONT)m_pPaintManager->GetRegularFont()->m_hObject;
	if (NULL != hRegularFont)
	{
		LOGFONT lfRegularFont;
		if (0 < GetObject(hRegularFont, sizeof(LOGFONT), &lfRegularFont))
		{
			m_fntFrameCaption.CreateFontIndirect(&lfRegularFont);
		}
	}
}

CXTPMarkupThemePart* CXTPFrameThemeOffice2007System::GetFrameBorder(int nId, BOOL bActive)
{
	CXTPMarkupThemePart* pBorder = NULL;

	switch (nId)
	{
		case xtpBorderPartTop: pBorder = bActive ? m_pBorderTopActive : m_pBorderTopInactive; break;
		case xtpBorderPartLeft:
			pBorder = bActive ? m_pBorderLeftActive : m_pBorderLeftInactive;
			break;
		case xtpBorderPartLeftCaption:
			pBorder = bActive ? m_pBorderLeftCaptionActive : m_pBorderLeftCaptionInactive;
			break;
		case xtpBorderPartRightCaption:
			pBorder = bActive ? m_pBorderRightCaptionActive : m_pBorderRightCaptionInactive;
			break;
		case xtpBorderPartRight:
			pBorder = bActive ? m_pBorderRightActive : m_pBorderRightInactive;
			break;
		case xtpBorderPartBottom:
			pBorder = bActive ? m_pBorderBottomActive : m_pBorderBottomInactive;
			break;
	}

	ASSERT(NULL != pBorder);

	return pBorder;
}

void CXTPFrameThemeOffice2007System::DrawCaptionBar(CDC* pDC, CRect rcCaptionBar, CWnd* pSite,
													BOOL bActive)
{
	if (bActive)
	{
		ASSERT(NULL != m_pCaptionActive);
		m_pCaptionActive->RenderMarkup(pDC, rcCaptionBar);
	}
	else
	{
		ASSERT(NULL != m_pCaptionInactive);
		m_pCaptionInactive->RenderMarkup(pDC, rcCaptionBar);
	}

	UNREFERENCED_PARAMETER(pSite);
}

void CXTPFrameThemeOffice2007System::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
															BOOL bSelected, BOOL bPressed,
															BOOL bActive)
{
	DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive, NULL);
}

void CXTPFrameThemeOffice2007System::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
															BOOL bSelected, BOOL bPressed,
															BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	UNUSED_ALWAYS(bActive);
	UNUSED_ALWAYS(pRibbonBar);

	if (bPressed)
	{
		ASSERT(NULL != m_pCaptionButtonPressed);
		m_pCaptionButtonPressed->RenderMarkup(pDC, rc);
	}
	else if (bSelected)
	{
		ASSERT(NULL != m_pCaptionButtonSelected);
		m_pCaptionButtonSelected->RenderMarkup(pDC, rc);
	}

	int nGlyphSize = rc.Width() >= 21 ? 11 : 9;

	CXTPMarkupThemePart* pGlyph = NULL;

	switch (nId)
	{
		case SC_MINIMIZE:
			pGlyph = (nGlyphSize == 11 ? m_pGlyphMinimize11 : m_pGlyphMinimize9);
			break;
		case SC_MAXIMIZE:
			pGlyph = (nGlyphSize == 11 ? m_pGlyphMaximize11 : m_pGlyphMaximize9);
			break;
		case SC_RESTORE: pGlyph = (nGlyphSize == 11 ? m_pGlyphRestore11 : m_pGlyphRestore9); break;
		case SC_CLOSE: pGlyph = (nGlyphSize == 11 ? m_pGlyphClose11 : m_pGlyphClose9); break;
	}

	if (NULL != pGlyph)
	{
		CSize szGlpyh = pGlyph->Measure(rc.Width(), rc.Height());

		CPoint ptCenter((rc.left + rc.right - szGlpyh.cx) / 2,
						(rc.top + rc.bottom - szGlpyh.cy) / 2);

		CRect rcGlyph(ptCenter, szGlpyh);
		pGlyph->RenderMarkup(pDC, rcGlyph);
	}
}
