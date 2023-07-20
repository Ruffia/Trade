// XTPFrameThemeOffice2013.cpp
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResource.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPTheme.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "TabManager/Includes.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/XTPMenuBar.h"

#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonPopups.h"
#	include "Ribbon/XTPRibbonBar.h"
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif

#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFrameThemeOffice2013::CXTPFrameThemeOffice2013(CXTPPaintManager* pPaintManager)
	: CXTPFrameThemeModernUI(pPaintManager)
{
	CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
}

CXTPFrameThemeOffice2013::~CXTPFrameThemeOffice2013()
{
}

void CXTPFrameThemeOffice2013::RefreshMetrics()
{
	CXTPRibbonMarkupFrameTheme::RefreshMetrics();

	LoadParts();

	// Default values found in OFFICE2013WORD_INI.

	m_bFlatFrame		  = XTPIniInt(_T("CommandBars.Frame"), _T("FlatFrame"), FALSE);
	m_nFrameCaptionHeight = XTP_DPI_Y(XTPIniInt(_T("CommandBars.Frame"), _T("CaptionHeight"), 26));
	m_szMDIClildWindowCaptionButtonSize = XTP_DPI(
		XTPIniSize(_T("CommandBars.Frame"), _T("MDIChildWindowCaptionButtonSize"),
				   XTP_UNDPI(m_szMDIClildWindowCaptionButtonSize)));

	m_clrFrameBorderActive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("ActiveBorderColor"), RGB(43, 87, 154)));
	m_clrFrameBorderInactive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("InactiveBorderColor"), RGB(131, 131, 131)));
	m_clrFrameCaptionTextActive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("ActiveFontColor"), RGB(68, 68, 68)));
	m_clrFrameCaptionTextInactive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("InactiveFontColor"), RGB(177, 177, 177)));
	m_clrFrameCaptionTextActiveTitle.SetStandardValue(m_clrFrameCaptionTextActive);
	m_clrFrameCaptionBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionBorder"), RGB(255, 255, 255)));
	m_clrFrameCaptionActive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("ActiveCaptionColor"), RGB(255, 255, 255)));
	m_clrFrameCaptionInactive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("InactiveCaptionColor"), RGB(255, 255, 255)));
	m_clrFrameCaptionTextActiveBSMode.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionActiveText"),
					RGB(68, 68, 68)));
	m_clrFrameCaptionTextInactiveBSMode.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionInactiveText"),
					RGB(177, 177, 177)));
	m_clrFrameCaptionTextActiveTitleBSMode.SetStandardValue(m_clrFrameCaptionTextActiveBSMode);

	// minimize / maximize / restore caption buttons.
	m_csFrameCaptionButtonActive.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonActiveBorder"), RGB(255, 255, 255)));
	m_csFrameCaptionButtonActive.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonActiveFillColor"),
					RGB(119, 119, 119)));
	m_csFrameCaptionButtonActive.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonActiveBackgroundColor"),
					(COLORREF)-1)); // transparent
	m_csFrameCaptionButtonInactive.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonInactiveBorder"),
					RGB(255, 255, 255)));
	m_csFrameCaptionButtonInactive.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonInactiveFillColor"),
					RGB(198, 198, 198)));
	m_csFrameCaptionButtonInactive.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonInactiveBackgroundColor"),
					(COLORREF)-1)); // transparent
	m_csFrameCaptionButtonHot.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonHotBorder"), RGB(213, 225, 242)));
	m_csFrameCaptionButtonHot.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonHotFillColor"), RGB(62, 109, 181)));
	m_csFrameCaptionButtonHot.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonHotBackgroundColor"),
					RGB(213, 225, 242)));
	m_csFrameCaptionButtonPressed.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonPressedBorder"), RGB(163, 189, 227)));
	m_csFrameCaptionButtonPressed.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonPressedFillColor"),
					RGB(25, 71, 138)));
	m_csFrameCaptionButtonPressed.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CaptionButtonPressedBackgroundColor"),
					RGB(163, 189, 227)));

	// close button.
	m_csFrameCloseButtonActive.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonActiveBorder"), RGB(255, 255, 255)));
	m_csFrameCloseButtonActive.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonActiveFillColor"), RGB(119, 119, 119)));
	m_csFrameCloseButtonActive.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonActiveBackgroundColor"),
					(COLORREF)-1)); // transparent
	m_csFrameCloseButtonInactive.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonInactiveBorder"), RGB(255, 255, 255)));
	m_csFrameCloseButtonInactive.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonInactiveFillColor"),
					RGB(198, 198, 198)));
	m_csFrameCloseButtonInactive.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonInactiveBackgroundColor"),
					(COLORREF)-1)); // transparent
	m_csFrameCloseButtonHot.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonHotBorder"), RGB(213, 225, 242)));
	m_csFrameCloseButtonHot.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonHotFillColor"), RGB(62, 109, 181)));
	m_csFrameCloseButtonHot.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonHotBackgroundColor"),
					RGB(213, 225, 242)));
	m_csFrameCloseButtonPressed.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonPressedBorder"), RGB(163, 189, 227)));
	m_csFrameCloseButtonPressed.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonPressedFillColor"), RGB(25, 71, 138)));
	m_csFrameCloseButtonPressed.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Frame"), _T("CloseButtonPressedBackgroundColor"),
					RGB(163, 189, 227)));

	m_csFrameCloseButtonActiveBSMode.m_clrOutline.SetStandardValue(COLORREF_NULL);
	m_csFrameCloseButtonActiveBSMode.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionActiveText"),
					RGB(0, 0, 0)));
	m_csFrameCloseButtonActiveBSMode.m_clrBackground.SetStandardValue(COLORREF_NULL);

	m_csFrameCloseButtonInactiveBSMode.m_clrOutline.SetStandardValue(COLORREF_NULL);
	m_csFrameCloseButtonInactiveBSMode.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonInactiveFill"),
					RGB(0, 0, 0)));
	m_csFrameCloseButtonInactiveBSMode.m_clrBackground.SetStandardValue(COLORREF_NULL);

	m_csFrameCaptionButtonActiveBSMode.m_clrOutline.SetStandardValue(COLORREF_NULL);
	m_csFrameCaptionButtonActiveBSMode.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionActiveText"),
					RGB(0, 0, 0)));
	m_csFrameCaptionButtonActiveBSMode.m_clrBackground.SetStandardValue(COLORREF_NULL);

	m_csFrameCaptionButtonInactiveBSMode.m_clrOutline.SetStandardValue(COLORREF_NULL);
	m_csFrameCaptionButtonInactiveBSMode.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonInactiveFill"),
					RGB(0, 0, 0)));
	m_csFrameCaptionButtonInactiveBSMode.m_clrBackground.SetStandardValue(COLORREF_NULL);

	m_csFrameCaptionButtonHotBSMode.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonHotBack"),
					RGB(0, 0, 0)));
	m_csFrameCaptionButtonHotBSMode.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonHotFill"),
					RGB(0, 0, 0)));
	m_csFrameCaptionButtonHotBSMode.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonHotBack"),
					RGB(0, 0, 0)));

	m_csFrameCaptionButtonPressedBSMode.m_clrOutline.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonPressedBack"),
					RGB(0, 0, 0)));
	m_csFrameCaptionButtonPressedBSMode.m_clrFill.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonPressedFill"),
					RGB(0, 0, 0)));
	m_csFrameCaptionButtonPressedBSMode.m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("FrameCaptionButtonPressedBack"),
					RGB(0, 0, 0)));

	CXTPNonClientMetrics ncm;
	ncm.lfCaptionFont.lfWeight = FW_NORMAL;
	ncm.lfCaptionFont.lfHeight = -XTP_DPI_Y(XTPIniInt(_T("CommandBars.Frame"), _T("FontSize"), 12));
	TCSNCCPY_S(ncm.lfCaptionFont.lfFaceName, LF_FACESIZE,
			   XTPIniString(_T("CommandBars.Frame"), _T("FontFace"), _T("Segoe UI")), LF_FACESIZE);
	m_fntFrameCaption.SetStandardFont(&ncm.lfCaptionFont);
}

void CXTPFrameThemeOffice2013::LoadParts(BOOL bReload /*= FALSE*/)
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
		STATIC_DOWNCAST(CXTPFrameThemeModernUI, this)
			->LoadParts(hZip, STATIC_DOWNCAST(CXTPCommandBarsModernUITheme, m_pPaintManager)
								  ->GetMarkupContext());
		CloseZip(hZip);
	}
}

void CXTPFrameThemeOffice2013::DrawCaptionBar(CDC* pDC, CRect rcCaptionBar, CWnd* pSite,
											  BOOL bActive)
{
	UNREFERENCED_PARAMETER(pSite);

	pDC->FillSolidRect(rcCaptionBar, bActive ? m_clrFrameCaptionActive : m_clrFrameCaptionInactive);

	CRect rcBorder(rcCaptionBar);
	rcBorder.top = rcBorder.bottom - XTP_DPI_Y(1);
	pDC->FillSolidRect(rcBorder, m_clrFrameCaptionBorder);
}

void CXTPFrameThemeOffice2013::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected,
													  BOOL bPressed, BOOL bActive)
{
	DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive, NULL);
}

void CXTPFrameThemeOffice2013::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected,
													  BOOL bPressed, BOOL bActive,
													  CXTPRibbonBar* pRibbonBar)
{
	CXTPModernUIFrameCaptionButton* pButton = NULL;

	switch (nId)
	{
		case SC_MINIMIZE: pButton = m_pButtonMinimize; break;
		case SC_MAXIMIZE: pButton = m_pButtonMaximize; break;
		case SC_RESTORE: pButton = m_pButtonRestore; break;
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

LPCTSTR CXTPFrameThemeOffice2013::GetStyleResourceName() const
{
	return _T("XAML_STYLE");
}

CRect CXTPFrameThemeOffice2013::CalcFrameBorder(const CXTPCommandBarsFrameHook* pFrameHook) const
{
	ASSERT(NULL != pFrameHook);

	CRect rc = CXTPFrameThemeModernUI::CalcFrameBorder(pFrameHook);

	if (pFrameHook->IsMDIMaximized())
	{
		rc.top -= m_nFrameCaptionHeight - GetSystemMetrics(SM_CYCAPTION);
		return rc;
	}

	return rc;
}

CXTPFrameShadow* CXTPFrameThemeOffice2013::CreateShadow(CWnd* pFrame)
{
	ASSERT(!XTPFrameShadowManager()->IsShadowEnabled(pFrame));

	CXTPFrameShadow* pShadow = XTPFrameShadowManager()->EnableShadow(pFrame, 0,
																	 XTPToUInt(XTP_DPI_Y(5)),
																	 XTPToUInt(XTP_DPI_Y(3)), 0,
																	 0x80);
	return pShadow;
}
