// XTPFrameThemeNativeWindows10.cpp
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
#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPMathUtils.h"
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
#include "Common/XTPTheme.h"
#include "Common/XTPResource.h"
#include "Common/XTPResourceManager.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/FrameShadow/XTPFrameShadow.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/XTPMenuBar.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonPopups.h"
#	include "Ribbon/XTPRibbonBar.h"
#	include "Ribbon/XTPRibbonPaintManager.h"
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#	include "Ribbon/XTPRibbonMarkupTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeOffice2013.h"
#include "CommandBars/Frame/XTPFrameThemeNativeWindows10.h"

#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2013Theme.h"
#include "CommandBars/Themes/XTPCommandBarsNativeWindows10Theme.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"
#include "Markup/Extensions/XTPMarkupSystemColorsStaticExtension.h"
#include "Markup/Extensions/XTPMarkupAmbientColorsStaticExtension.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFrameThemeNativeWindows10::CXTPFrameThemeNativeWindows10(CXTPPaintManager* pPaintManager)
	: CXTPFrameThemeOffice2013(pPaintManager)
{
	m_pButtonRestoreActive   = new CXTPModernUIFrameCaptionButton();
	m_pButtonRestoreInactive = new CXTPModernUIFrameCaptionButton();
	m_pButtonRestorePressed  = new CXTPModernUIFrameCaptionButton();
	m_pButtonRestoreHot		 = new CXTPModernUIFrameCaptionButton();

	XTPGetApplication()->Subscribe(this);
}

CXTPFrameThemeNativeWindows10::~CXTPFrameThemeNativeWindows10()
{
	XTPGetApplication()->Unsubscribe(this);

	SAFE_DELETE(m_pButtonRestoreActive);
	SAFE_DELETE(m_pButtonRestoreInactive);
	SAFE_DELETE(m_pButtonRestorePressed);
	SAFE_DELETE(m_pButtonRestoreHot);
}

void CXTPFrameThemeNativeWindows10::RefreshMetrics()
{
	CXTPFrameThemeOffice2013::RefreshMetrics();

	m_clrShadowColorActive   = XTPIniColor(_T("CommandBars.Frame"), _T("ActiveShadowColor"),
										   RGB(0, 122, 204));
	m_clrShadowColorInactive = XTPIniColor(_T("CommandBars.Frame"), _T("InactiveShadowColor"),
										   RGB(48, 48, 48));

	UpdateAccentColors();
}

CXTPFrameShadow* CXTPFrameThemeNativeWindows10::CreateShadow(CWnd* pFrame)
{
	ASSERT(!XTPFrameShadowManager()->IsShadowEnabled(pFrame));

	CXTPFrameShadow* pShadow = XTPFrameShadowManager()->EnableShadow(pFrame, m_clrShadowColorActive,
																	 XTPToUInt(XTP_DPI_Y(5)),
																	 XTPToUInt(XTP_DPI_Y(3)), 0,
																	 0x80);

	return pShadow;
}

void CXTPFrameThemeNativeWindows10::OnShadowedFrameActiveStateChanged(BOOL bActive, CWnd* pFrame,
																	  CXTPFrameShadow* pShadow)
{
	ASSERT(pShadow);
	UNUSED_ALWAYS(pFrame);

	pShadow->SetColor(bActive ? m_clrShadowColorActive : m_clrShadowColorInactive);
}

void CXTPFrameThemeNativeWindows10::SetStateColor(COLORREF clrState)
{
	if (clrState == COLORREF_NULL)
	{
		m_clrFrameBorderActive.SetDefaultValue();
	}
	else
	{
		m_clrFrameBorderActive = clrState;
	}
}

COLORREF CXTPFrameThemeNativeWindows10::GetStateColor() const
{
	return m_clrFrameBorderActive.IsDefaultValue() ? COLORREF_NULL
												   : m_clrFrameBorderActive.operator COLORREF();
}

void CXTPFrameThemeNativeWindows10::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
														   BOOL bSelected, BOOL bPressed,
														   BOOL bActive)
{
	DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive, NULL);
}

void CXTPFrameThemeNativeWindows10::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
														   BOOL bSelected, BOOL bPressed,
														   BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	CXTPModernUIFrameCaptionButton* pButton = NULL;

	switch (nId)
	{
		case SC_MINIMIZE: pButton = m_pButtonMinimize; break;
		case SC_MAXIMIZE: pButton = m_pButtonMaximize; break;
		case SC_RESTORE:
			if (bPressed)
				pButton = m_pButtonRestorePressed;
			else if (bSelected)
				pButton = m_pButtonRestoreHot;
			else if (bActive)
				pButton = m_pButtonRestoreActive;
			else if (!bActive)
				pButton = m_pButtonRestoreInactive;
			else
				ASSERT(0);
			break;
		case SC_CLOSE: pButton = m_pButtonClose; break;
	}

	CXTPColorSetCaptionButton* pColorSet = NULL;

	if (bPressed)
	{
		if (NULL != pRibbonBar && pRibbonBar->IsCaptionBarInBackstageMode())
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonPressed
										  : &m_csFrameCaptionButtonPressedBSMode;
		}
		else
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonPressed
										  : &m_csFrameCaptionButtonPressed;
		}
	}
	else if (bSelected)
	{
		if (NULL != pRibbonBar && pRibbonBar->IsCaptionBarInBackstageMode())
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonHot
										  : &m_csFrameCaptionButtonHotBSMode;
		}
		else
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonHot : &m_csFrameCaptionButtonHot;
		}
	}
	else if (bActive)
	{
		if (NULL != pRibbonBar && pRibbonBar->IsCaptionBarInBackstageMode())
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonActiveBSMode
										  : &m_csFrameCaptionButtonActiveBSMode;
		}
		else
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonActive
										  : &m_csFrameCaptionButtonActive;
		}
	}
	else if (!bActive)
	{
		if (NULL != pRibbonBar && pRibbonBar->IsCaptionBarInBackstageMode())
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonInactiveBSMode
										  : &m_csFrameCaptionButtonInactiveBSMode;
		}
		else
		{
			pColorSet = (nId == SC_CLOSE) ? &m_csFrameCloseButtonInactive
										  : &m_csFrameCaptionButtonInactive;
		}
	}

	if (NULL != pColorSet)
	{
		ASSERT(NULL != pButton);
		pButton->SetColors(pColorSet);
		pButton->RenderMarkup(pDC, rc);
	}
}

void CXTPFrameThemeNativeWindows10::LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext)
{
	if (m_bMarkupPartsLoaded)
		return;

	if (pMarkupContext)
	{
		CXTPMarkupStaticExtension* pRoot = pMarkupContext->GetExtensionRoot();
		if (pRoot)
		{
			CXTPMarkupStaticExtension* pFrame  = new CXTPMarkupStaticExtension(L"Frame");
			CXTPMarkupStaticExtension* pColors = new CXTPMarkupStaticExtension(L"Colors");
			CXTPMarkupStaticExtension* pTheme  = new CXTPMarkupStaticExtension(L"Theme");

			pFrame->Extend(new CXTPMarkupPaintManagerBaseThemeBrushStaticExtensionProperty(
				L"ButtonRestoreActiveBrush", xtpPaintManagerAccentColor));

			pFrame->Extend(
				new CXTPMarkupStaticExtensionBrushProperty(L"ButtonRestoreInactiveBrush",
														   XTPIniColor(_T("CommandBars.Frame"),
																	   _T("InactiveCaptionColor"),
																	   RGB(255, 255, 255))));

			pFrame->Extend(new CXTPMarkupStaticExtensionBrushProperty(
				L"ButtonRestoreHotBrush",
				XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonHotBackgroundColor"),
							RGB(213, 225, 242))));

			pFrame->Extend(new CXTPMarkupStaticExtensionBrushProperty(
				L"ButtonRestorePressedBrush",
				XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonPressedBackgroundColor"),
							RGB(163, 189, 227))));

			pColors->Extend(pFrame);
			pTheme->Extend(pColors);
			pRoot->Extend(pTheme, TRUE);
		}
	}

	m_bMarkupPartsLoaded = TRUE;

	// Frame border
	BOOL bMarkupLoaded = m_pBorderLeft->LoadPart(hZip, _T("Frame/BorderLeft.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderTop->LoadPart(hZip, _T("Frame/BorderTop.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderRight->LoadPart(hZip, _T("Frame/BorderRight.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderBottom->LoadPart(hZip, _T("Frame/BorderBottom.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	// Frame caption buttons
	bMarkupLoaded = m_pButtonMinimize->LoadPart(hZip, _T("Frame/ButtonMinimize.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonMaximize->LoadPart(hZip, _T("Frame/ButtonMaximize.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonRestoreActive->LoadPart(hZip, _T("Frame/ButtonRestoreActive.xaml"),
													 pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonRestoreInactive->LoadPart(hZip, _T("Frame/ButtonRestoreInactive.xaml"),
													   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonRestoreHot->LoadPart(hZip, _T("Frame/ButtonRestoreHot.xaml"),
												  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonRestorePressed->LoadPart(hZip, _T("Frame/ButtonRestorePressed.xaml"),
													  pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonClose->LoadPart(hZip, _T("Frame/ButtonClose.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	UNREFERENCED_PARAMETER(bMarkupLoaded);
}

void CXTPFrameThemeNativeWindows10::UpdateAccentColors()
{
	COLORREF crAccent			= XTPGetAccentColor(xtpPaintManagerAccentColor);
	COLORREF crNormalTextAccent = XTPGetAccentColor(xtpPaintManagerNormalTextAccentColor);

	// Frame back color
	m_clrFrameBorderActive.SetStandardValue(crAccent);
	m_clrFrameCaptionActive.SetStandardValue(crAccent);

	// Frame caption text
	m_clrFrameCaptionTextActive.SetStandardValue(crNormalTextAccent);
	m_clrFrameCaptionTextActiveBSMode.SetStandardValue(crNormalTextAccent);

	// Minimize/maximize/restore button text
	m_csFrameCaptionButtonActive.m_clrBackground.SetStandardValue(crAccent);
	m_csFrameCaptionButtonActive.m_clrFill.SetStandardValue(crNormalTextAccent);

	// Close button text
	m_csFrameCloseButtonActive.m_clrBackground.SetStandardValue(crAccent);
	m_csFrameCloseButtonActive.m_clrFill.SetStandardValue(crNormalTextAccent);
}

void CXTPFrameThemeNativeWindows10::LoadParts(BOOL bReload /*= FALSE*/)
{
	if (bReload)
		m_bMarkupPartsLoaded = FALSE;

	if (m_bMarkupPartsLoaded)
		return;

	HMODULE hModule = XTPResourceImages()->GetHandle();
	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	CXTPResource resource;
	resource.LoadResource(hModule, GetStyleResourceName(), _T("STYLE"));

	if (NULL != resource.m_hResource)
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		LoadParts(hZip, STATIC_DOWNCAST(CXTPCommandBarsNativeWindows10Theme, m_pPaintManager)
							->GetMarkupContext());
		CloseZip(hZip);
	}
}

void CXTPFrameThemeNativeWindows10::OnAmbientPropertyChanged(
	CXTPApplication* pApplication, XTPApplicationAmbientProperty nProperty)
{
	UNREFERENCED_PARAMETER(pApplication);

	ASSERT(NULL != pApplication);

	if (nProperty != xtpApplicationAccentColor)
		return;

#ifdef _XTP_INCLUDE_RIBBON
	CXTPRibbonPaintManager* pRibbonPaintManager =
		STATIC_DOWNCAST(CXTPCommandBarsNativeWindows10Theme, m_pPaintManager)
			->GetRibbonPaintManager();
	STATIC_DOWNCAST(CXTPRibbonMarkupTheme, pRibbonPaintManager)->LoadParts(TRUE);
#endif

	LoadParts(TRUE);

	m_pPaintManager->RefreshMetrics();
}
