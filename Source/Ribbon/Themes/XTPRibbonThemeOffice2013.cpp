// XTPRibbonThemeOffice2013.cpp : implementation file
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPResource.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "TabManager/Includes.h"

#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2013Theme.h"

#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonTab.h"
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonSystemButton.h"
#include "Ribbon/XTPRibbonMarkupTheme.h"
#include "Ribbon/XTPRibbonControlTab.h"
#include "Ribbon/XTPRibbonTabColorSet.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#include "Ribbon/Themes/XTPRibbonThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeStaticExtension

CXTPRibbonOffice2013Theme::CThemeStaticExtension::CThemeStaticExtension()
	: CXTPMarkupStaticExtension(L"Theme")
{
	CXTPMarkupStaticExtension* pColors = new CXTPMarkupStaticExtension(L"Colors");

	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"TabSelectedHighlightedDarkBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("TabSelectedHighlightedDarkBrush"),
					RGB(171, 171, 171))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"TabSelectedHighlightedLightBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("TabSelectedHighlightedLightBrush"),
					RGB(243, 243, 243))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"TabSelectedFocusedDarkBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("TabSelectedFocusedDarkBrush"),
															   RGB(171, 171, 171))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"TabSelectedFocusedLightBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("TabSelectedFocusedLightBrush"),
															   RGB(243, 243, 243))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"TabSelectedDarkBrush", XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
											 _T("TabSelectedDarkBrush"), RGB(171, 171, 171))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"TabSelectedLightBrush", XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
											  _T("TabSelectedLightBrush"), RGB(243, 243, 243))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"TabNormalBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("TabNormalBrush"), RGB(255, 0, 0))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"GroupRectBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("GroupRectBrush"), RGB(171, 171, 171))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"GroupPopupToolbarBrush", XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
											   _T("GroupPopupToolbarBrush"), RGB(171, 171, 171))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"GroupPopupSelectedBorderBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("GroupPopupSelectedBorderBrush"),
															   RGB(171, 171, 171))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"GroupPopupSelectedBackBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("GroupPopupSelectedBackBrush"),
															   RGB(243, 243, 243))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"GroupPopupPopupedBorderBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("GroupPopupPopupedBorderBrush"),
															   RGB(171, 171, 171))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"GroupPopupPopupedBackBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("GroupPopupPopupedBackBrush"),
															   RGB(243, 243, 243))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"GroupPopupNormalBrush", XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
											  _T("GroupPopupNormalBrush"), RGB(171, 171, 171))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"GroupNormalBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("GroupNormalBrush"), RGB(171, 171, 171))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"GroupHighlightedBrush", XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
											  _T("GroupHighlightedBrush"), RGB(171, 171, 171))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameSystemMenuSelectedBackBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("FrameSystemMenuSelectedBackBrush"),
					RGB(42, 138, 212))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameSystemMenuSelectedTextBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("FrameSystemMenuSelectedTextBrush"),
					RGB(255, 255, 255))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameSystemMenuPopupedBackBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("FrameSystemMenuPopupedBackBrush"),
					RGB(42, 141, 212))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameSystemMenuPopupedTextBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Xaml"), _T("FrameSystemMenuPopupedTextBrush"),
					RGB(255, 255, 255))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"FrameSystemMenuNormalBackBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("FrameSystemMenuNormalBackBrush"),
															   RGB(51, 51, 51))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"FrameSystemMenuNormalTextBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("FrameSystemMenuNormalTextBrush"),
															   RGB(255, 255, 255))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"FrameContextHeaderBackBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("FrameContextHeaderBackBrush"),
															   RGB(230, 243, 230))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"FrameContextHeaderBorderBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("FrameContextHeaderBorderBrush"),
															   RGB(73, 163, 73))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"FrameContextHeaderTextBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("FrameContextHeaderTextBrush"),
															   RGB(73, 163, 73))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameCaptionInactiveBackBrush",
		XTPIniColor(_T("CommandBars.Frame"), _T("InactiveCaptionColor"), RGB(255, 255, 255))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameCaptionActiveBackBrush",
		XTPIniColor(_T("CommandBars.Frame"), _T("ActiveCaptionColor"), RGB(255, 255, 255))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"FrameCaptionBackstageLeftBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
															   _T("ButtonBackNormal"),
															   RGB(255, 255, 255))));
	pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(
		L"FrameCaptionBackstageRightBrush",
		XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageBackground"), RGB(255, 255, 255))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"SystemPopupBarEntryBorderBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("SystemPopupBarEntryBorderBrush"),
															   RGB(255, 255, 255))));
	pColors->Extend(
		new CXTPMarkupStaticExtensionBrushProperty(L"SystemPopupBarEntryBackBrush",
												   XTPIniColor(_T("CommandBars.Ribbon.Xaml"),
															   _T("SystemPopupBarEntryBackBrush"),
															   RGB(255, 255, 255))));

	Extend(pColors);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonOffice2013Theme

IMPLEMENT_DYNAMIC(CXTPRibbonOffice2013Theme, CXTPRibbonMarkupTheme)

CXTPRibbonOffice2013Theme::CXTPRibbonOffice2013Theme(CXTPPaintManager* pPaintManager)
	: CXTPRibbonMarkupTheme(pPaintManager)
	, m_nBackImage(XTPNone)
{
	if (m_pMarkupContext)
	{
		CThemeStaticExtension* pThemeStaticExtension = new CThemeStaticExtension();

		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.TabSelectedHighlightedDarkBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.TabSelectedHighlightedLightBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.TabSelectedFocusedDarkBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.TabSelectedFocusedLightBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.TabSelectedDarkBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.TabSelectedLightBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.TabNormalBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupRectBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupPopupToolbarBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.GroupPopupSelectedBorderBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupPopupSelectedBackBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.GroupPopupPopupedBorderBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupPopupPopupedBackBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupPopupNormalBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupNormalBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.GroupHighlightedBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameSystemMenuSelectedBackBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameSystemMenuSelectedTextBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameSystemMenuPopupedBackBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameSystemMenuPopupedTextBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameSystemMenuNormalBackBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameSystemMenuNormalTextBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.FrameContextHeaderBackBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameContextHeaderBorderBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.FrameContextHeaderTextBrush"));
		ASSERT(NULL
			   != pThemeStaticExtension->FindExtension(L"Colors.FrameCaptionInactiveBackBrush"));
		ASSERT(NULL != pThemeStaticExtension->FindExtension(L"Colors.FrameCaptionActiveBackBrush"));

		CXTPMarkupStaticExtension* pExtensionRoot = m_pMarkupContext->GetExtensionRoot();
		if (!pExtensionRoot)
		{
			ASSERT(FALSE);
			return;
		}

		pExtensionRoot->Extend(pThemeStaticExtension);
	}
}

CXTPRibbonOffice2013Theme::~CXTPRibbonOffice2013Theme()
{
}

void CXTPRibbonOffice2013Theme::RefreshMetrics()
{
	CXTPRibbonMarkupTheme::RefreshMetrics();

	m_nCaptionHeight = XTP_DPI_Y(XTPIniInt(_T("CommandBars.Frame"), _T("CaptionHeight"), 28));

	m_clrRibbonFace   = XTPIniColor(_T("CommandBars.Ribbon"), _T("RibbonFace"), RGB(243, 243, 243));
	m_clrRibbonBorder = XTPIniColor(_T("CommandBars.Ribbon"), _T("RibbonBorder"),
									RGB(171, 171, 171));
	m_clrRibbonInactiveFace		  = XTPIniColor(_T("CommandBars.Ribbon"), _T("RibbonInactiveFace"),
											RGB(222, 222, 222));
	m_clrRibbonInactiveFaceState  = XTPIniColor(_T("CommandBars.Ribbon"),
												_T("RibbonInactiveFaceState"), RGB(222, 222, 222));
	m_clrMinimizedFrameEdgeShadow = XTPIniColor(_T("CommandBars.Ribbon"),
												_T("MinimizedFrameEdgeShadow"), RGB(243, 243, 243));
	m_clrMinimizedFrameEdgeHighLight = XTPIniColor(_T("CommandBars.Ribbon"),
												   _T("MinimizedFrameEdgeHighLight"),
												   RGB(243, 243, 243));
	m_clrControlGroupOption   = XTPIniColor(_T("CommandBars.Ribbon"), _T("ControlGroupOption"),
											RGB(119, 119, 119));
	m_clrGroupCaption		  = XTPIniColor(_T("CommandBars.Ribbon"), _T("GroupCaption"),
									RGB(119, 119, 119));
	m_clrTabBackInactiveState = XTPIniColor(_T("CommandBars.Ribbon"), _T("TabBackInactiveState"),
											RGB(255, 255, 255));
	m_clrQuickAccessBelow	 = XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"),
										RGB(38, 38, 38));
	m_clrQuickAccess = XTPIniColor(_T("CommandBars.Frame"), _T("ActiveFontColor"), RGB(38, 38, 38));
	m_clrQuickAccessDisabled	  = XTPIniColor(_T("CommandBars.Frame"), _T("InactiveFontColor"),
											RGB(38, 38, 38));
	m_clrQuickAccessButtonHotBack = XTPIniColor(_T("CommandBars.Frame"),
												_T("CaptionButtonHotBackgroundColor"),
												RGB(255, 255, 255));
	m_clrQuickAccessButtonPressedBack		   = XTPIniColor(_T("CommandBars.Frame"),
													 _T("CaptionButtonPressedBackgroundColor"),
													 RGB(255, 255, 255));
	m_clrTabBackackstageLeft				   = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonBackNormal"), RGB(255, 255, 255));
	m_clrTabBackackstageRight				   = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("PageBackground"), RGB(255, 255, 255));
	m_clrBackstageSysButtonBack2013Normal	  = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
														 _T("ButtonBackNormal"), RGB(255, 255, 255));
	m_clrBackstageSysButtonBack2013Highlighted = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
															 _T("ButtonBackHighlighted"),
															 RGB(255, 255, 255));
	m_strHighlightedPrefix					   = XTPIniString(_T("CommandBars.Ribbon.Backstage"),
										  _T("ButtonImagePrefixHighlighted"), _T(""));
	m_strNormalPrefix						   = XTPIniString(_T("CommandBars.Ribbon.Backstage"),
									  _T("ButtonImagePrefixNormal"), _T(""));
	m_clrRibbonTabDelimeter = XTPIniColor(_T("CommandBars.Ribbon"), _T("RibbonTabDelimeter"),
										  RGB(225, 225, 225));

	CXTPTabPaintManagerColorSet* pColorSet = m_pTabPaintManager->GetColorSet();
	if (pColorSet)
	{
		pColorSet->m_clrButtonNormal = XTPIniColor(_T("CommandBars.Ribbon"), _T("TabBackInactive"),
												   RGB(255, 255, 255));
		pColorSet->m_clrButtonHighlighted = XTPIniColor(_T("CommandBars.Ribbon"),
														_T("TabBackHighlighted"),
														RGB(255, 255, 255));
		pColorSet->m_clrTextNormal	= XTPIniColor(_T("CommandBars.Ribbon"), _T("TabTextInactive"),
													RGB(38, 38, 38));
		pColorSet->m_clrTextHighlight = XTPIniColor(_T("CommandBars.Ribbon"),
													_T("TabTextHighlighted"), RGB(0, 114, 198));
		pColorSet->m_clrTextSelected  = XTPIniColor(_T("CommandBars.Ribbon"), _T("TabTextSelected"),
													RGB(0, 114, 198));
	}

	m_clrHeader[0].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorNoneBorder"),
										 RGB(243, 243, 243));
	m_clrHeader[0].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorNoneBack"),
										 RGB(243, 243, 243));
	m_clrHeader[0].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorNoneText"),
										 RGB(102, 102, 102));
	m_clrHeader[1].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorBlueBorder"),
										 RGB(43, 87, 154));
	m_clrHeader[1].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorBlueBack"),
										 RGB(213, 225, 242));
	m_clrHeader[1].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorBlueText"),
										 RGB(62, 109, 181));
	m_clrHeader[2].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"),
										 _T("ColorYellowBorder"), RGB(242, 203, 29));
	m_clrHeader[2].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorYellowBack"),
										 RGB(255, 251, 230));
	m_clrHeader[2].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorYellowText"),
										 RGB(152, 119, 21));
	m_clrHeader[3].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"),
										 _T("ColorGreenBorder"), RGB(73, 163, 73));
	m_clrHeader[3].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorGreenBack"),
										 RGB(230, 243, 230));
	m_clrHeader[3].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorGreenText"),
										 RGB(73, 138, 73));
	m_clrHeader[4].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorRedBorder"),
										 RGB(226, 101, 102));
	m_clrHeader[4].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorRedBack"),
										 RGB(248, 212, 212));
	m_clrHeader[4].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorRedText"),
										 RGB(202, 90, 92));
	m_clrHeader[5].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"),
										 _T("ColorPurpleBorder"), RGB(201, 89, 156));
	m_clrHeader[5].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorPurpleBack"),
										 RGB(252, 240, 247));
	m_clrHeader[5].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorPurpleText"),
										 RGB(161, 64, 122));
	m_clrHeader[6].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorCyanBorder"),
										 RGB(153, 200, 233));
	m_clrHeader[6].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorCyanBack"),
										 RGB(205, 230, 247));
	m_clrHeader[6].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorCyanText"),
										 RGB(1, 89, 138));
	m_clrHeader[7].clrEdge = XTPIniColor(_T("CommandBars.Ribbon.TabContext"),
										 _T("ColorOrangeBorder"), RGB(255, 157, 0));
	m_clrHeader[7].clrBack = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorOrangeBack"),
										 RGB(255, 248, 237));
	m_clrHeader[7].clrText = XTPIniColor(_T("CommandBars.Ribbon.TabContext"), _T("ColorOrangeText"),
										 RGB(207, 92, 10));

	// If System button is popped up the text color of inactive (normal) button is used as the color
	// of text for selected button Let's find out will such text be visible. If it's not we'll set
	// the appropriate flag.
	CXTPRibbonTabColorSet* pRibbonTabCS = (CXTPRibbonTabColorSet*)pColorSet;
	if (NULL != pRibbonTabCS)
	{
		COLORREF c1 = pColorSet->m_clrTextNormal, c2 = m_clrHeader[0].clrBack;
		int curColor, maxColor, minColor;
		maxColor = GetRValue(c1);
		minColor = GetRValue(c1);
		curColor = GetGValue(c1);
		if (maxColor < curColor)
			maxColor = curColor;
		if (minColor > curColor)
			minColor = curColor;
		curColor = GetBValue(c1);
		if (maxColor < curColor)
			maxColor = curColor;
		if (minColor > curColor)
			minColor = curColor;
		int textLight = (maxColor + minColor) / 2;
		maxColor	  = GetRValue(c2);
		minColor	  = GetRValue(c2);
		curColor	  = GetGValue(c2);
		if (maxColor < curColor)
			maxColor = curColor;
		if (minColor > curColor)
			minColor = curColor;
		curColor = GetBValue(c2);
		if (maxColor < curColor)
			maxColor = curColor;
		if (minColor > curColor)
			minColor = curColor;
		int backLight = (maxColor + minColor) / 2;
		int res;
		if (backLight > textLight)
			res = backLight - textLight;
		else
			res = textLight - backLight;
		if (res < 84)
			// the text will not be visible most likely
			pRibbonTabCS->m_bIsInactiveSelectedTextVisible = FALSE;
	}

	LoadParts();
}

void CXTPRibbonOffice2013Theme::LoadParts(BOOL bReload /*= FALSE*/)
{
	if (bReload)
		m_bLoaded = FALSE;

	if (m_bLoaded)
		return;

	HMODULE hModule = XTPResourceImages()->GetHandle();

	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	m_strThemeName = XTPIniString(_T("Theme"), _T("Name"), _T("OFFICE2013"));

	CXTPResource resource;
	resource.LoadResource(hModule, _T("XAML_STYLE"), _T("STYLE"));

	if (NULL != resource.m_hResource)
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		if (hZip)
		{
			STATIC_DOWNCAST(CXTPRibbonMarkupTheme, this)->LoadParts(hZip);
			CloseZip(hZip);
		}
	}
}

int CXTPRibbonOffice2013Theme::GetRibbonCaptionHeight(const CXTPRibbonBar* pRibbonBar) const
{
	if (pRibbonBar)
	{
		CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();
		if (pFrameHook && pFrameHook->IsDwmEnabled())
		{
			return XTPSystemMetrics()->m_nCaptionY;
		}
	}

	return m_nCaptionHeight;
}

void CXTPRibbonOffice2013Theme::DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem)
{
	// must be valid.
	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	// must be valid.
	CXTPRibbonBar* pRibbon = pItem->GetRibbonBar();
	ASSERT(pRibbon != NULL);
	if (!pRibbon)
		return;

	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();
	XTPTabPosition tabPosition		  = pItem->GetTabManager()->GetPosition();

	CRect r(pItem->GetRect());
	CXTPTabPaintManagerTheme::InflateRectEx(r, CRect(-1, 1, -1, -1), tabPosition);
	BOOL bSelected = pItem->IsSelected();
	if (pRibbon->IsRibbonMinimized() && bSelected)
	{
		CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pItem->GetTabManager();
		if (pControlTab)
		{
			if (!pControlTab->GetCommandBar() || !pControlTab->GetCommandBar()->IsTrackingMode())
				bSelected = FALSE;
		}
	}

	if (bSelected)
	{
		r.DeflateRect(0, 0, 1, 0);
		pDC->FillSolidRect(r, m_clrHeader[xtpRibbonTabContextColorNone].clrBack);

		XTPDrawHelpers()->DrawLine(pDC, r.left, r.top, r.left, r.bottom, m_clrRibbonBorder);
		XTPDrawHelpers()->DrawLine(pDC, r.right, r.top, r.right, r.bottom, m_clrRibbonBorder);
		XTPDrawHelpers()->DrawLine(pDC, r.left, r.top, r.right, r.top, m_clrRibbonBorder);
	}
	else
	{
		r.DeflateRect(0, 0, 0, XTP_DPI_Y(1));

		if (pRibbon->IsActive())
		{
			if (pItem->IsHighlighted())
			{
				pDC->FillSolidRect(r, XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
													m_clrButtonHighlighted, tabColor));
			}
			else
			{
				pDC->FillSolidRect(r, XTP_SAFE_GET2(m_pTabPaintManager, GetColorSet(),
													m_clrButtonNormal, tabColor));
			}
		}
		else
		{
			pDC->FillSolidRect(r, m_clrTabBackInactiveState);
		}
	}

	for (int i = 0; i < XTP_SAFE_GET2(pItem, GetTabManager(), GetItemCount(), 0); i++)
	{
		if (XTP_SAFE_GET3(pItem, GetTabManager(), GetItem(i), IsItemShrinked(), FALSE))
		{
			CRect rc(r);
			rc.left  = bSelected ? rc.right + 1 : rc.right;
			rc.right = rc.left + XTP_DPI_X(1);
			rc.top += XTP_DPI_Y(3);
			rc.bottom -= XTP_DPI_Y(3);

			pDC->FillSolidRect(rc, m_clrRibbonTabDelimeter);
			break;
		}
	}
}

void CXTPRibbonOffice2013Theme::DrawRibbonFrameContextHeaders(
	CDC* pDC, CXTPRibbonBar* pRibbonBar, CXTPRibbonTabContextHeaders* pContextHeaders)
{
	int nCount = pContextHeaders->GetCount();

	CXTPWinThemeWrapper wrapper(FALSE);
	wrapper.OpenThemeData(NULL, L"WINDOW");

	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pContextHeaders->GetHeader(i);
		XTPRibbonTabContextColor color		= pHeader->m_color;

		CString strCaption(pHeader->m_strCaption);
		if (pRibbonBar->GetCommandBars()->IsAllCaps())
		{
			strCaption.MakeUpper();
		}

		CRect rcHeader(pHeader->m_rcRect);
		if (LAYOUT_RTL == pDC->GetLayout())
		{
			// GDI+ has issues with RTL renderring, so this is to emulate RTL.
			CPoint ptHeaderTopLeft = rcHeader.TopLeft();
			pRibbonBar->ClientToScreen(&ptHeaderTopLeft);

			CXTPClientRect rcClient(pRibbonBar);
			CPoint ptWindowTopLeft = rcClient.TopLeft();
			CPoint ptWindowTopRight(rcClient.right, ptWindowTopLeft.y);
			pRibbonBar->ClientToScreen(&ptWindowTopLeft);
			pRibbonBar->ClientToScreen(&ptWindowTopRight);
			ASSERT(ptWindowTopRight.x <= ptWindowTopLeft.x);
			ASSERT(ptHeaderTopLeft.x < ptWindowTopLeft.x && ptWindowTopRight.x < ptHeaderTopLeft.x);

			int nWidth	= rcHeader.Width();
			rcHeader.left = rcClient.Width() - (ptWindowTopLeft.x - ptHeaderTopLeft.x) - nWidth
							- XTP_DPI_X(1);
			rcHeader.right = rcHeader.left + nWidth;
		}

		rcHeader.DeflateRect(XTP_DPI_X(1), 0, XTP_DPI_X(1), 0);

		CRect rcBorder(rcHeader.left, rcHeader.top, rcHeader.right, rcHeader.top + XTP_DPI_Y(4));
		CRect rcBackground(rcHeader.left, rcBorder.bottom, rcHeader.right, rcHeader.bottom);
		CRect rcText(rcBackground);
		CSize szText = pDC->GetTextExtent(strCaption);

		int nMaxTextWidth = rcText.Width() - XTP_DPI_X(10);
		if (nMaxTextWidth < szText.cx)
		{
			szText.cx = nMaxTextWidth;
		}

		rcText.left += (rcText.Width() - szText.cx) / 2;

		// Draw header using GDI+
		rcText.top += (rcBackground.Height() - szText.cy) / 2;

		Gdiplus::Graphics graphics(pDC->GetSafeHdc());
		graphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
		graphics.SetCompositingQuality(Gdiplus::CompositingQualityDefault);

		// Draw border
		DWORD clrEdge = m_clrHeader[color].clrEdge;
		Gdiplus::SolidBrush brBorder(
			Gdiplus::Color(0xfe, // Not 0xff because GDI+ the applies color blending.
						   GetRValue(clrEdge), GetGValue(clrEdge), GetBValue(clrEdge)));
		Gdiplus::RectF rcfBorder(static_cast<Gdiplus::REAL>(rcBorder.left),
								 static_cast<Gdiplus::REAL>(rcBorder.top),
								 static_cast<Gdiplus::REAL>(rcBorder.Width()),
								 static_cast<Gdiplus::REAL>(rcBorder.Height()));
		graphics.FillRectangle(&brBorder, rcfBorder);

		// Draw background
		DWORD clrBackground = m_clrHeader[color].clrBack;
		Gdiplus::SolidBrush brBackground(
			Gdiplus::Color(0xfe, // Not 0xff because GDI+ the applies color blending.
						   GetRValue(clrBackground), GetGValue(clrBackground),
						   GetBValue(clrBackground)));
		Gdiplus::RectF rcfBackground(static_cast<Gdiplus::REAL>(rcBackground.left),
									 static_cast<Gdiplus::REAL>(rcBackground.top),
									 static_cast<Gdiplus::REAL>(rcBackground.Width()),
									 static_cast<Gdiplus::REAL>(rcBackground.Height()));
		graphics.FillRectangle(&brBackground, rcfBackground);

		// Draw text
		Gdiplus::Font font(pDC->GetSafeHdc(), *m_pTabPaintManager->GetFont());
		Gdiplus::RectF rcfText(static_cast<Gdiplus::REAL>(rcText.left),
							   static_cast<Gdiplus::REAL>(rcText.top),
							   static_cast<Gdiplus::REAL>(rcText.Width()),
							   static_cast<Gdiplus::REAL>(rcText.Height()));

		DWORD clrText = m_clrHeader[color].clrText;
		Gdiplus::SolidBrush brText(
			Gdiplus::Color(0xfe, // Not 0xff because GDI+ the applies color blending.
						   GetRValue(clrText), GetGValue(clrText), GetBValue(clrText)));

		Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap, ::GetUserDefaultLangID());
		format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		graphics.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
#ifdef _UNICODE
		graphics.DrawString(strCaption, strCaption.GetLength(), &font, rcfText, &format, &brText);
#else
		WCHAR* pUnicodeCaption = new WCHAR[XTPToSizeT(strCaption.GetLength() + 1)];
		int cchCaption		   = MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, strCaption,
											 strCaption.GetLength(), pUnicodeCaption,
											 XTPToInt((strCaption.GetLength() + 1)
													  * sizeof(WCHAR)));
		if (0 < cchCaption)
		{
			graphics.DrawString(pUnicodeCaption, cchCaption, &font, rcfText, &format, &brText);
		}
		delete[] pUnicodeCaption;
#endif
	}

	wrapper.CloseThemeData();
}

void CXTPRibbonOffice2013Theme::FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pRibbonBar);

	CXTPClientRect rcClient(pRibbonBar);

	if (pRibbonBar->IsFrameThemeEnabled())
	{
		CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();

		if (pFrameHook->IsDwmEnabled())
		{
			CRect rcBorder = GetFramePaintManager()->CalcFrameBorder(pFrameHook);
			rcClient.top += rcBorder.top;
		}
	}

	rcClient.top += pRibbonBar->GetCaptionHeight();

	CRect rcTabs(rcClient.left, rcClient.top, rcClient.right,
				 rcClient.top + m_nTabsHeight + XTP_DPI_Y(1));
	if (pRibbonBar->IsCaptionBarInBackstageMode())
	{
		CRect rc(rcTabs);
		rc.bottom = XTP_DPI_Y(pRibbonBar->GetBackstageSysButtonBottom());
		rc.right  = pRibbonBar->GetBackstageModeMenuWidth();
		pDC->FillSolidRect(rc, m_clrTabBackackstageLeft);

		rc.left  = pRibbonBar->GetBackstageModeMenuWidth();
		rc.right = rcTabs.right;
		pDC->FillSolidRect(rc, m_clrTabBackackstageRight);
	}
	else
	{
		if (pRibbonBar->IsTabsVisible())
		{
			pDC->FillSolidRect(rcTabs, pRibbonBar->IsActive() ? m_clrRibbonInactiveFace
															  : m_clrRibbonInactiveFaceState);
		}
		else
		{
			rcTabs.bottom -= m_nTabsHeight + XTP_DPI_Y(1);
		}
	}

	CRect rcGroups(rcClient.left, rcTabs.bottom, rcClient.right, rcClient.bottom);
	if (!pRibbonBar->IsCaptionBarInBackstageMode())
	{
		pDC->FillSolidRect(rcGroups, m_clrRibbonFace);
	}

	if (pRibbonBar->IsQuickAccessBelowRibbon())
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
#if 0
		pDC->Draw3dRect(rcQuickAccess, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));
#endif
	}

	if (!pRibbonBar->IsGroupsVisible() && !pRibbonBar->IsQuickAccessBelowRibbon())
	{
		m_pPaintManager->HorizontalLine(pDC, rcClient.left, rcClient.bottom - XTP_DPI_Y(2),
										rcClient.right, m_clrMinimizedFrameEdgeShadow);
		m_pPaintManager->HorizontalLine(pDC, rcClient.left, rcClient.bottom - XTP_DPI_Y(1),
										rcClient.right, m_clrMinimizedFrameEdgeHighLight);
	}
}

void CXTPRibbonOffice2013Theme::SetBackImage(int nImage)
{
	m_nBackImage = nImage;
}

CString CXTPRibbonOffice2013Theme::GetBackImageName()
{
	// If no image defined, return NULL.
	if (m_nBackImage == XTPNone)
		return _T("");

	// Check INI to determine resource DLL.
	CString strIniFile = XTPResourceImages()->m_strIniFileName;
	if (strIniFile.IsEmpty())
		return _T("");

	strIniFile.MakeUpper();

	CString strImage;
	strImage.Empty();

	// Office2013.dll loaded.
	if (m_strThemeName == _T("OFFICE2013"))
	{
		strImage = m_strThemeName;

		if (strIniFile.Find(_T("DARK_INI")) != -1)
		{
			strImage += _T("DARKGRAY");
		}
		else if (strIniFile.Find(_T("LIGHT_INI")) != -1)
		{
			strImage += _T("LIGHTGRAY");
		}
		else
		{
			strImage += _T("WHITE");
		}
	}

	// Office2016.dll loaded.
	if (m_strThemeName == _T("OFFICE2016"))
	{
		strImage = m_strThemeName;

		if (strIniFile.Find(_T("BLACK_INI")) != -1)
		{
			strImage += _T("BLACK");
		}
		else if (strIniFile.Find(_T("COLORFUL_INI")) != -1)
		{
			strImage += _T("COLORFUL");
		}
		else if (strIniFile.Find(_T("DARKGRAY_INI")) != -1)
		{
			strImage += _T("DARKGRAY");
		}
		else
		{
			strImage += _T("WHITE");
		}
	}

	// Check DPI to determine image size.
	int nDPI = XTPDpiHelper()->GetDPIX();

	if (nDPI <= 96)
	{
		strImage += _T("SMALL");
	}
	else if (nDPI <= 200)
	{
		strImage += _T("MEDIUM");
	}
	else
	{
		strImage += _T("LARGE");
	}

	// Check ID and append image name.
	switch (m_nBackImage)
	{
		case XTPCalligraphy: strImage += _T("CALLIGRAPHY"); break;
		case XTPCirclesAndStripes: strImage += _T("CIRCLESANDSTRIPES"); break;
		case XTPCircuit: strImage += _T("CIRCUIT"); break;
		case XTPClouds: strImage += _T("CLOUDS"); break;
		case XTPDoodleCircles: strImage += _T("DOODLECIRCLES"); break;
		case XTPDoodleDiamonds: strImage += _T("DOODLEDIAMONDS"); break;
		case XTPGeometry: strImage += _T("GEOMETRY"); break;
		case XTPLunchBox: strImage += _T("LUNCHBOX"); break;
		case XTPSchoolSupplies: strImage += _T("SCHOOLSUPPLIES"); break;
		case XTPSpring: strImage += _T("SPRING"); break;
		case XTPStars: strImage += _T("STARS"); break;
		case XTPStraws: strImage += _T("STRAWS"); break;
		case XTPTreeRings: strImage += _T("TREERINGS"); break;
		case XTPUnderWater: strImage += _T("UNDERWATER"); break;
	}

	return strImage;
}

void CXTPRibbonOffice2013Theme::DrawBackImage(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	CXTPClientRect rcClient(pRibbonBar);

	CXTPResourceImages* pImages = XTPResourceImages();
	CXTPResourceImage* pImage   = pImages->LoadFile(GetBackImageName());

	if (pImage)
	{
		if (pRibbonBar->IsFrameThemeEnabled())
		{
			CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();

			if (pFrameHook->IsDwmEnabled())
			{
				CRect rcBorder = GetFramePaintManager()->CalcFrameBorder(pFrameHook);
				rcClient.top += rcBorder.top;
			}

			rcClient.bottom = rcClient.top + GetRibbonCaptionHeight(pRibbonBar);
		}

		if (pRibbonBar->IsTabsVisible())
		{
			rcClient.bottom += m_nTabsHeight + XTP_DPI_Y(1);
		}

		// draw background.
		CRect rcSource = pImage->GetSource();
		float fCoef	= static_cast<float>(rcClient.Height())
					  / static_cast<float>(rcSource.Height()); // for proportional image streching
		CRect rcImage(rcClient.right
						  - static_cast<int>(static_cast<float>(rcSource.Width()) * fCoef),
					  rcClient.top, rcClient.right, rcClient.bottom);

		pImage->DrawImage(pDC, rcImage, rcSource);
	}
}
void CXTPRibbonOffice2013Theme::DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar,
														  BOOL bActive)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pRibbonBar);

	if (!pRibbonBar->IsDwmEnabled() && pRibbonBar->IsCaptionBarInBackstageMode())
	{
		CRect rcCaption = pRibbonBar->GetCaptionRect();
		CRect rc(rcCaption);
		rc.right = pRibbonBar->GetBackstageModeMenuWidth();
		RenderMarkup(pDC, rc, xtpRibbonFrameCaptionBackstageLeft);

		rc.left  = pRibbonBar->GetBackstageModeMenuWidth();
		rc.right = rcCaption.right;
		RenderMarkup(pDC, rc, xtpRibbonFrameCaptionBackstageRight);
	}
	else
	{
		CXTPRibbonMarkupTheme::DrawRibbonFrameCaptionBar(pDC, pRibbonBar, bActive);
	}
}

void CXTPRibbonOffice2013Theme::DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar,
													   BOOL bActive)
{
	CRect rcCaption = pRibbonBar->GetCaptionRect();

	if (!pRibbonBar->IsDwmEnabled())
	{
		if (pRibbonBar->IsCaptionBarInBackstageMode())
		{
			CRect rc(rcCaption);
			rc.left  = pRibbonBar->GetBackstageModeMenuWidth();
			rc.right = rcCaption.right;
			RenderMarkup(pDC, rc, xtpRibbonFrameCaptionBackstageRight);
		}
		else
		{
			if (bActive)
			{
				RenderMarkup(pDC, rcCaption, xtpRibbonFrameCaptionActive);
			}
			else
			{
				RenderMarkup(pDC, rcCaption, xtpRibbonFrameCaptionInactive);
			}
		}
	}

	DrawBackImage(pDC, pRibbonBar);
	if (!pRibbonBar->IsDwmEnabled() && pRibbonBar->IsCaptionBarInBackstageMode())
	{
		CRect rc(rcCaption);
		rc.right = pRibbonBar->GetBackstageModeMenuWidth();
		RenderMarkup(pDC, rc, xtpRibbonFrameCaptionBackstageLeft);
	}

	// Frame icon
	CXTPRibbonControlSystemButton* pSystemButton = pRibbonBar->GetSystemButton();

	if ((NULL == pSystemButton) || (xtpButtonCaption == pSystemButton->GetStyle()))
	{
		if (!pRibbonBar->IsCaptionBarInBackstageMode())
		{
			m_pPaintManager->GetFramePaintManager()->DrawFrameIcon(pDC, pRibbonBar->GetFrameHook(),
																   rcCaption);
		}
	}

	DrawRibbonQuickAccessControls(pDC, pRibbonBar);

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if ((pContextHeaders->GetCount() != 0) && (!pRibbonBar->IsCaptionBarInBackstageMode()))
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}

	CRect rcCaptionText = pRibbonBar->GetCaptionTextRect();

	CXTPFontDC font(pDC, &m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption);

	CWnd* pSite = pRibbonBar->GetSite();

	CString strWindowText;
	pSite->GetWindowText(strWindowText);

	if (pSite->GetStyle() & WS_MAXIMIZE)
	{
		rcCaptionText.top += pRibbonBar->GetFrameHook()->CalcFrameBorder().top / 2;
	}

	if (!pRibbonBar->IsDwmEnabled())
	{
		m_pPaintManager->GetFramePaintManager()->DrawCaptionText(pDC, rcCaptionText, pSite, bActive,
																 pRibbonBar);
	}
	else
	{
		DrawDwmCaptionText(pDC, rcCaptionText, strWindowText, pSite, bActive);
	}
}

void CXTPRibbonOffice2013Theme::DrawQuickAccessControl(CDC* pDC, CXTPRibbonBar* pRibbon,
													   CXTPControl* pControl)
{
	UNREFERENCED_PARAMETER(pRibbon);
	ASSERT_VALID(pControl);
	pControl->Draw(pDC);
}

void CXTPRibbonOffice2013Theme::DrawRibbonFrameSystemButton(CDC* pDC,
															CXTPRibbonControlSystemButton* pControl,
															CRect rc)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pControl);

	LOGFONT lf;
	m_fontGroupCaption.GetLogFont(&lf);
	int fontSize = abs(lf.lfHeight);

	m_pSystemMenuNormal->SetFontFamily(lf.lfFaceName);
	m_pSystemMenuNormal->SetFontSize(fontSize);
	m_pSystemMenuSelected->SetFontFamily(lf.lfFaceName);
	m_pSystemMenuSelected->SetFontSize(fontSize);
	m_pSystemMenuPopuped->SetFontFamily(lf.lfFaceName);
	m_pSystemMenuPopuped->SetFontSize(fontSize);

	CXTPRibbonMarkupTheme::DrawRibbonFrameSystemButton(pDC, pControl, rc);
}
