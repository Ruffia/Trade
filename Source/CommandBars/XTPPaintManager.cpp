// XTPPaintManager.cpp : implementation of the CXTPPaintManager class.
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "GraphicLibrary/unzip/unzip.h"
#include "CommandBars/XTPCommandBarsDefines.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "CommandBars/Resource.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h"
#include "Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/FrameShadow/XTPFrameShadow.h"

#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBarKeyboardTip.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#ifdef _XTP_INCLUDE_MARKUP
#	include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"
using namespace Gdiplus;
using namespace Gdiplus::DllExports;
#	include "Markup/DeviceContext/XTPGdiPlus.h"
#endif

// Progress
#include "CommandBars/Progress/XTPProgressPaintManager.h"

// Slider
#include "CommandBars/Slider/XTPSliderPaintManager.h"
#include "CommandBars/Slider/XTPSliderThemeOffice2007System.h"

// Message bar
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2010.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"

// Frame
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeOffice2007System.h"

// Office
#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2000Theme.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2007SystemTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2013Theme.h"
#include "CommandBars/Themes/XTPCommandBarsResourceTheme.h"

// Windows
#include "CommandBars/Themes/XTPCommandBarsNativeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsNativeWindows10Theme.h"

// Visual Studio
#include "CommandBars/Themes/XTPCommandBarsVisualStudio6Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2005Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2008Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2010Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2012Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2015Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2017Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2019Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio2022Theme.h"

// Color sets

#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonPaintManager.h"
#	include "CommandBars/Themes/XTPCommandBarsThemeRibbon.h"
#	include "Ribbon/XTPRibbonMarkupTheme.h"
#	include "Ribbon/Themes/XTPRibbonThemeOffice2007System.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////
// CXTPPaintManagerFont

BOOL CXTPPaintManagerFont::SetCustomFont(const LOGFONT* lpLogFont)
{
	m_bStandardFont = (lpLogFont == NULL);
	if (lpLogFont)
	{
		DeleteObject();
		return CreateFontIndirect(lpLogFont);
	}
	return TRUE;
}

void CXTPPaintManagerFont::EnableCustomFont(BOOL bEnable /*= TRUE*/)
{
	m_bStandardFont = !bEnable;
}

BOOL CXTPPaintManagerFont::SetStandardFont(const LOGFONT* lpLogFont)
{
	if (lpLogFont && (m_bStandardFont || !GetSafeHandle()))
	{
		DeleteObject();
		return CreateFontIndirect(lpLogFont);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager::CLogFont

CXTPPaintManager::CLogFont::CLogFont()
{
	::ZeroMemory(this, sizeof(LOGFONT));
}

///////////////////////////////////////////////////////////////////////////////
// CXTPPaintManager::CGlobalPaintManagerPtr

class CXTPPaintManager::CGlobalPaintManagerPtr
	: public CXTPSingleton<CXTPPaintManager::CGlobalPaintManagerPtr
#ifdef _XTP_INCLUDE_MARKUP
						   ,
						   CXTPSingletonDependencies<CXTPSingleton<CXTPGdiPlus> >
#endif
						   >
{
	friend class CXTPSingleton<CXTPPaintManager::CGlobalPaintManagerPtr
#ifdef _XTP_INCLUDE_MARKUP
							   ,
							   CXTPSingletonDependencies<CXTPSingleton<CXTPGdiPlus> >
#endif
							   >;

	CXTPPaintManager* m_pPaintManager;

private:
	CGlobalPaintManagerPtr()
		: m_pPaintManager(NULL)
	{
	}

	~CGlobalPaintManagerPtr()
	{
		if (NULL != m_pPaintManager)
		{
			CXTPPaintManager::ReleaseInstance();
		}
	}

public:
	CXTPPaintManager* GetCurrent()
	{
		return m_pPaintManager;
	}

	void SetCurrent(CXTPPaintManager* pPaintManager)
	{
		m_pPaintManager = pPaintManager;

#pragma warning(push)
#pragma warning(disable : 4996)
		// Retain backward compatibility
		CXTPPaintManager::s_pInstance = pPaintManager;
#pragma warning(pop)
	}

	ULONG AddRef()
	{
		return (NULL != m_pPaintManager ? m_pPaintManager->InternalAddRef() : 0);
	}

	ULONG Release()
	{
		ULONG cRefs = 0;
		if (NULL != m_pPaintManager)
		{
			cRefs = m_pPaintManager->InternalRelease();
			if (0 == cRefs)
			{
				m_pPaintManager = NULL;
			}
		}

		return cRefs;
	}
};

///////////////////////////////////////////////////////////////////////////////
// CXTPPaintManager

CXTPPaintManager* AFX_CDECL XTPPaintManager()
{
	CXTPPaintManager* pPaintManager = CXTPPaintManager::GetGlobalPaintManager();
	if (NULL == pPaintManager)
	{
		pPaintManager = CXTPPaintManager::GetInstanceAddRef();
	}

	return pPaintManager;
}

CXTPPaintManager* CXTPPaintManager::s_pInstance = NULL;

IMPLEMENT_DYNAMIC(CXTPPaintManager, CXTPCmdTarget)

CXTPPaintManager* AFX_CDECL CXTPPaintManager::GetGlobalPaintManager()
{
	return CXTPPaintManager::CGlobalPaintManagerPtr::Instance().GetCurrent();
}

void AFX_CDECL CXTPPaintManager::SetGlobalPaintManager(CXTPPaintManager* pPaintManager)
{
	CXTPPaintManager::CGlobalPaintManagerPtr::Instance().SetCurrent(pPaintManager);
}

CXTPPaintManager* AFX_CDECL CXTPPaintManager::GetInstanceAddRef()
{
	if (NULL == GetGlobalPaintManager())
	{
		SetTheme(xtpThemeOffice2000);
	}
	else
	{
		CGlobalPaintManagerPtr::Instance().AddRef();
	}

	return GetGlobalPaintManager();
}

void AFX_CDECL CXTPPaintManager::ReleaseInstance()
{
	CGlobalPaintManagerPtr::Instance().Release();
}

CXTPPaintManager::CXTPPaintManager()
{
	m_bFlatToolBar = FALSE;
	m_bFlatMenuBar = FALSE;

	m_nTextHeight			= 0;
	m_bUseOfficeFont		= FALSE;
	m_strOfficeFont			= _T("Tahoma");
	m_bUseStandardFont		= TRUE;
	m_bShowShadow			= TRUE;
	m_bEmbossedDisabledText = TRUE;
	m_nStatusBarMinHeight   = XTP_DPI_Y(20);

	m_bThemedStatusBar = FALSE;
	m_bThemedCheckBox  = FALSE;

	m_iconsInfo.bUseFadedIcons			  = FALSE;
	m_iconsInfo.bIconsWithShadow		  = FALSE;
	m_iconsInfo.bUseDisabledIcons		  = FALSE;
	m_iconsInfo.bOfficeStyleDisabledIcons = FALSE;

	m_nAnimationSteps		= 10;
	m_nAnimationTime		= 100;
	m_bThickCheckMark		= FALSE;
	m_bClearTypeTextQuality = FALSE;

	m_systemTheme		= xtpSystemThemeDefault;
	m_bShowKeyboardCues = -1;

	m_nSplitButtonDropDownWidth  = CalcSplitButtonDropDownWidth(XTP_COMMANDBAR_MIN_CONTROL_HEIGHT);
	m_nSplitButtonDropDownHeight = CalcSplitButtonDropDownHeight(XTP_COMMANDBAR_MIN_FONT_HEIGHT);
	m_nPopupBarTextPadding		 = XTP_DPI_X(4);
	m_nSplitButtonPopupWidth	 = XTP_DPI_X(20);

	m_bSelectImageInPopupBar = FALSE;
	m_bEnableAnimation		 = FALSE;

	m_bOffice2007Padding = FALSE;

	m_bOffsetPopupLabelText = TRUE;

	m_nEditHeight	  = 0;
	m_nFontHeight	  = 0;
	m_bAutoResizeIcons = FALSE;

	m_bWrapCaptionBelowText = TRUE;

	m_rcStatusBarBorder.SetRect(0, 0, 0, 0);

	m_themeStatusBar = new CXTPWinThemeWrapper();
	m_themeButton	= new CXTPWinThemeWrapper();

	XTPImageManager()->RefreshAll();

	m_pGalleryPaintManager	= new CXTPControlGalleryPaintManager(this);
	m_pSliderPaintManager	 = new CXTPSliderThemeOffice2007System(this);
	m_pProgressPaintManager   = new CXTPProgressPaintManager(this);
	m_pScrollBarPaintManager  = new CXTPScrollBarThemeWindowsUx();
	m_pStatusBarPaintManager  = new CXTPStatusBarPaintManager(this);
	m_pMessageBarPaintManager = new CXTPMessageBarPaintManager(this);
	m_pFramePaintManager	  = NULL;
	m_pRibbonPaintManager	 = NULL;

#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_pfnSetLayeredWindowAttributes = NULL;
	// init layered function  (for Win98 compatible)
	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		m_pfnSetLayeredWindowAttributes = (PVOID)::GetProcAddress(hLib,
																  "SetLayeredWindowAttributes");
	}

	m_pShadowManager = new CXTPShadowManager();
}

CXTPPaintManager::~CXTPPaintManager()
{
	SAFE_DELETE(m_pGalleryPaintManager);
	SAFE_DELETE(m_pSliderPaintManager);
	SAFE_DELETE(m_pProgressPaintManager);
	SAFE_DELETE(m_pFramePaintManager);
	SAFE_DELETE(m_pScrollBarPaintManager);
	SAFE_DELETE(m_pStatusBarPaintManager);
	SAFE_DELETE(m_pMessageBarPaintManager);
	SAFE_DELETE(m_pStatusBarPaintManager);

	CMDTARGET_RELEASE(m_pShadowManager);

	if (m_pRibbonPaintManager)
		((CCmdTarget*)m_pRibbonPaintManager)->InternalRelease();

	SAFE_DELETE(m_themeStatusBar);
	SAFE_DELETE(m_themeButton);

	Notify(&IXTPPaintManagerEvents::OnPaintManagerDestroyed);
}

XTPCurrentSystemTheme CXTPPaintManager::GetCurrentSystemTheme()
{
	if (m_systemTheme == xtpSystemThemeDefault)
		return XTPColorManager()->IsLunaColorsDisabled()
				   ? xtpSystemThemeUnknown
				   : XTPColorManager()->GetCurrentSystemTheme();

	if (m_systemTheme == xtpSystemThemeAuto)
		return XTPColorManager()->GetWinThemeWrapperTheme();

	return m_systemTheme;
}
void CXTPPaintManager::SetLunaTheme(XTPCurrentSystemTheme systemTheme)
{
	m_systemTheme = systemTheme;
	RefreshMetrics();
}

COLORREF CXTPPaintManager::GetXtremeColor(UINT nIndex)
{
	if (nIndex > XPCOLOR_LAST)
		return nIndex;
	return m_arrColor[nIndex];
}

void CXTPPaintManager::SetColors(int cElements, CONST INT* lpaElements,
								 CONST COLORREF* lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrColor[lpaElements[i]].SetStandardValue(lpaRgbValues[i]);
}

int CXTPPaintManager::GetShadowOptions() const
{
	return m_pShadowManager->GetShadowOptions();
}

XTP_COMMANDBARS_ICONSINFO* CXTPPaintManager::GetIconsInfo()
{
	return &m_iconsInfo;
}

void CXTPPaintManager::UpdateColors()
{
	RefreshXtremeColors(FALSE);

	CXTPColorManager* pColorManager = XTPColorManager();

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrColor[i].SetStandardValue(pColorManager->GetColor(i));
}

void CXTPPaintManager::RefreshMetrics()
{
	UpdateFonts();
	UpdateColors();

	m_pShadowManager->SetShadowColor(0);

	// Message bar
	m_rcMessageBarMargin.SetRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));
	m_clrMessageBarText = GetXtremeColor(COLOR_INFOTEXT);

	m_clrFloatingGripper.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrFloatingGripperText.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT));

	m_clrDisabledIcon.SetStandardValue(GetXtremeColor(COLOR_3DFACE),
									   GetXtremeColor(COLOR_3DSHADOW));

	m_themeStatusBar->CloseThemeData();
	m_themeButton->CloseThemeData();

	if (m_bThemedCheckBox)
	{
		m_themeButton->OpenThemeData(NULL, L"BUTTON");
	}

	if (m_pScrollBarPaintManager)
		m_pScrollBarPaintManager->RefreshMetrics();
	if (m_pGalleryPaintManager)
		m_pGalleryPaintManager->RefreshMetrics();
	if (m_pSliderPaintManager)
		m_pSliderPaintManager->RefreshMetrics();
	if (m_pProgressPaintManager)
		m_pProgressPaintManager->RefreshMetrics();
	if (m_pFramePaintManager)
		m_pFramePaintManager->RefreshMetrics();
	if (m_pStatusBarPaintManager)
		m_pStatusBarPaintManager->RefreshMetrics();
	if (m_pMessageBarPaintManager)
		m_pMessageBarPaintManager->RefreshMetrics();

	if (m_bThemedStatusBar && m_pStatusBarPaintManager)
	{
		m_themeStatusBar->OpenThemeData(NULL, L"STATUS");

		if (m_themeStatusBar->IsAppThemeActive())
		{
			COLORREF clrText = 0;
			if (SUCCEEDED(m_themeStatusBar->GetThemeColor(SP_PANE, 0, TMT_TEXTCOLOR, &clrText)))
			{
				m_pStatusBarPaintManager->m_clrText.SetStandardValue(clrText);
			}
		}
	}

#ifdef _XTP_INCLUDE_RIBBON
	if (m_pRibbonPaintManager)
		m_pRibbonPaintManager->RefreshMetrics();
#endif
}

CXTPFramePaintManager* CXTPPaintManager::GetFramePaintManager()
{
	if (!m_pFramePaintManager)
	{
		m_pFramePaintManager = new CXTPFrameThemeOffice2007System(this);
		m_pFramePaintManager->RefreshMetrics();
	}
	return m_pFramePaintManager;
}

CXTPStatusBarPaintManager* CXTPPaintManager::GetStatusBarPaintManager()
{
	return m_pStatusBarPaintManager;
}

CXTPMessageBarPaintManager* CXTPPaintManager::GetMessageBarPaintManager()
{
	return m_pMessageBarPaintManager;
}

CXTPRibbonPaintManager* CXTPPaintManager::GetRibbonPaintManager()
{
#ifdef _XTP_INCLUDE_RIBBON
	if (!m_pRibbonPaintManager)
	{
		m_pRibbonPaintManager = new CXTPRibbonThemeOffice2007System(this);
		m_pRibbonPaintManager->RefreshMetrics();
	}
#endif
	return m_pRibbonPaintManager;
}

int CXTPPaintManager::GetControlHeight() const
{
	return max(XTP_COMMANDBAR_MIN_CONTROL_HEIGHT, m_nEditHeight);
}

CSize CXTPPaintManager::GetDefaultCommandBarSize(CXTPCommandBar* pBar)
{
	CClientDC dc(pBar);
	CXTPFontDC font(&dc, GetCommandBarFont(pBar));
	CSize szSize = DrawCommandBarGripper(&dc, pBar, FALSE);
	return szSize;
}

CXTPPaintManager* CXTPPaintManager::CreateTheme(XTPPaintTheme paintTheme)
{
#if defined(_XTP_COMMANDBARS_ACTIVEX) && defined(_XTP_INCLUDE_RIBBON)
	if (xtpThemeRibbon == paintTheme && XTPResourceImages()->m_bLoadedNothing)
	{
		paintTheme							  = xtpThemeOfficeXP;
		XTPResourceImages()->m_bLoadedNothing = FALSE;
	}
#endif

	CXTPCommandBarsFrameHook::m_bAllowDwm = CXTPWinDwmWrapper().IsCompositionEnabled();
	CXTPPaintManager* pInstance			  = NULL;

	switch (paintTheme)
	{
		case xtpThemeOfficeXP: pInstance = new CXTPCommandBarsOfficeXPTheme(); break;
		case xtpThemeOffice2003: pInstance = new CXTPCommandBarsOffice2003Theme(); break;
		case xtpThemeOffice2007System:
			pInstance = new CXTPCommandBarsOffice2007SystemTheme();
			break;
		case xtpThemeOffice2013:
			pInstance							  = new CXTPCommandBarsOffice2013Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeNativeWinXP: pInstance = new CXTPCommandBarsNativeXPTheme(); break;
		case xtpThemeVisualStudio6: pInstance = new CXTPVisualStudio6Theme(); break;
		case xtpThemeVisualStudio2005: pInstance = new CXTPVisualStudio2005Theme(); break;
		case xtpThemeVisualStudio2008: pInstance = new CXTPVisualStudio2008Theme(); break;
		case xtpThemeVisualStudio2010: pInstance = new CXTPVisualStudio2010Theme(); break;
		case xtpThemeVisualStudio2012:
			pInstance							  = new CXTPCommandBarsVisualStudio2012Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeVisualStudio2012Light:
			pInstance = new CXTPCommandBarsVisualStudio2012Theme(xtpThemeVisualStudio2012Light);
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeVisualStudio2012Dark:
			pInstance = new CXTPCommandBarsVisualStudio2012Theme(xtpThemeVisualStudio2012Dark);
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeVisualStudio2015:
			pInstance							  = new CXTPCommandBarsVisualStudio2015Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeVisualStudio2017:
			pInstance							  = new CXTPCommandBarsVisualStudio2017Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeVisualStudio2019:
			pInstance							  = new CXTPCommandBarsVisualStudio2019Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeVisualStudio2022:
			pInstance							  = new CXTPCommandBarsVisualStudio2022Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeNativeWindows10:
			pInstance							  = new CXTPCommandBarsNativeWindows10Theme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
		case xtpThemeResource:
			pInstance							  = new CXTPResourceTheme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
#ifdef _XTP_INCLUDE_RIBBON
		case xtpThemeRibbon:
			pInstance							  = new CXTPRibbonTheme();
			CXTPCommandBarsFrameHook::m_bAllowDwm = FALSE;
			break;
#endif
		case xtpThemeOffice2000:
		case xtpThemeNone:
		case xtpThemeCustom:
		default: pInstance = new CXTPCommandBarsOffice2000Theme(); break;
	}

	ASSERT(NULL != pInstance);
	pInstance->m_themeCurrent = paintTheme;
	return pInstance;
}

void CXTPPaintManager::SetTheme(XTPPaintTheme paintTheme)
{
	SetCustomTheme(CreateTheme(paintTheme));

	CXTPPaintManager* pPaintManager = GetGlobalPaintManager();
	ASSERT_VALID(pPaintManager);
	pPaintManager->m_themeCurrent = paintTheme;

	if (XTPResourceImages()->m_strIniFileName.Find(_T("OFFICE2010")) != -1)
	{
		SAFE_DELETE(pPaintManager->m_pMessageBarPaintManager);
		pPaintManager->m_pMessageBarPaintManager = new CXTPMessageBarThemeOffice2010(
			XTPPaintManager());
		pPaintManager->m_pMessageBarPaintManager->RefreshMetrics();
	}
}

void CXTPPaintManager::SetCustomTheme(CXTPPaintManager* pTheme)
{
	ASSERT_VALID(pTheme);

	CXTPPaintManager* pOldTheme = GetGlobalPaintManager();
	SetGlobalPaintManager(pTheme);

	XTPColorManager()->ResetCustomColors();
	pTheme->m_themeCurrent = xtpThemeCustom;
	pTheme->RefreshMetrics();
	RefreshXtremeColors(FALSE);

	if (pOldTheme != NULL)
	{
		pTheme->InternalAddRef();
		DWORD nNewRefCount = pTheme->InternalRelease();

		pOldTheme->InternalAddRef();
		DWORD nOldRefCount = pOldTheme->InternalRelease();

		if (nNewRefCount < nOldRefCount)
		{
			while (nOldRefCount != pTheme->InternalAddRef())
				;
		}
		else if (nOldRefCount < nNewRefCount)
		{
			while (nOldRefCount != pTheme->InternalRelease())
				;
		}

		while (0 < pOldTheme->InternalRelease())
			;
	}
}

void CXTPPaintManager::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr)
{
	CXTPPenDC pen(*pDC, clr);
	CXTPBrushDC brush(*pDC, clr);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}

void CXTPPaintManager::Rectangle(CDC* pDC, LPCRECT rc, COLORREF clrPen, COLORREF clrBrush)
{
	if (clrBrush != COLORREF_NULL)
		pDC->FillSolidRect(rc, clrBrush);

	if (clrPen != clrBrush && clrPen != COLORREF_NULL)
		pDC->Draw3dRect(rc, clrPen, clrPen);
}

void CXTPPaintManager::Rectangle(CDC* pDC, LPCRECT rc, int nPen, const int nBrush)
{
	Rectangle(pDC, rc, (nPen == -1 ? COLORREF_NULL : GetXtremeColor(XTPToUIntChecked(nPen))),
			  (nBrush == -1 ? COLORREF_NULL : GetXtremeColor(XTPToUIntChecked(nBrush))));
}

void CXTPPaintManager::Draw3dRect(CDC* pDC, LPCRECT rc, int nTopLeft, int nBottomRight)
{
	pDC->Draw3dRect(rc, GetXtremeColor(XTPToUIntChecked(nTopLeft)),
					GetXtremeColor(XTPToUIntChecked(nBottomRight)));
}

void CXTPPaintManager::Draw3dRect(CDC* pDC, LPCRECT rc, int frameThickness, COLORREF clrPen,
								  COLORREF clrBrush)
{
	if (clrBrush != COLORREF_NULL)
	{
		CRect r(rc);
		for (int i = 0; i < frameThickness; i++)
		{
			pDC->Draw3dRect(r, clrBrush, clrBrush);
			r.DeflateRect(1, 1, 1, 1);
		}
	}

	if (clrPen != clrBrush && clrPen != COLORREF_NULL)
		pDC->Draw3dRect(rc, clrPen, clrPen);
}

void CXTPPaintManager::_DrawCheckMark(CDC* pDC, CRect rc, COLORREF clr, BOOL bLayoutRTL /*= FALSE*/)
{
	XTPPrimitiveDrawer()->DrawSymbol(pDC,
									 bLayoutRTL ? xtpPrimitiveSymbolCheckMarkRTL
												: xtpPrimitiveSymbolCheckMark,
									 rc, clr);
}

AFX_INLINE COLORREF Mix(CDC* pDC, int x, int y, COLORREF clrBorder, COLORREF clrFace, double a)
{
	COLORREF clr = clrFace;

	if (a < 0)
		a = -a;
	else
	{
		clr = pDC->GetPixel(x, y);
	}
	int r = int(GetRValue(clrBorder) + a * double(GetRValue(clr) - GetRValue(clrBorder)));
	int g = int(GetGValue(clrBorder) + a * double(GetGValue(clr) - GetGValue(clrBorder)));
	int b = int(GetBValue(clrBorder) + a * double(GetBValue(clr) - GetBValue(clrBorder)));

	return RGB(r, g, b);
}

void CXTPPaintManager::AlphaEllipse(CDC* pDC, CRect rc, COLORREF clrBorder, COLORREF clrFace)
{
	if (rc.Width() != rc.Height())
		return;
	double x0	 = double(rc.left + rc.right - 1) / 2;
	double y0	 = double(rc.top + rc.bottom - 1) / 2;
	double radius = double(rc.right - rc.left - 1) / 2 - 0.25;

	for (int x = rc.left; x < rc.right; x++)
		for (int y = rc.top; y < rc.bottom; y++)
		{
			double distance = sqrt(pow(x0 - x, 2) + pow(y0 - y, 2));

			if (distance >= radius - 1 && distance <= radius + 1)
			{
				pDC->SetPixel(x, y, Mix(pDC, x, y, clrBorder, clrFace, distance - radius));
			}
			else if (distance < radius - 1)
			{
				pDC->SetPixel(x, y, clrFace);
			}
		}
}

void CXTPPaintManager::DrawPopupBarRadioMark(CDC* pDC, CRect rcCheck, BOOL /*bEnabled*/,
											 COLORREF clr)
{
	CPoint pt(rcCheck.CenterPoint());
	CRect rcRadio(pt.x - XTP_DPI_X(3), pt.y - XTP_DPI_Y(3), pt.x + XTP_DPI_X(3),
				  pt.y + XTP_DPI_Y(3));

	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolRadioMark, rcCheck, clr);
}

void CXTPPaintManager::DrawPopupBarCheckMark(CDC* pDC, CRect rc, BOOL /*bEnabled*/, COLORREF clr)
{
	BOOL bLayoutRTL = CXTPDrawHelpers::IsContextRTL(pDC);

	_DrawCheckMark(pDC, rc, clr, bLayoutRTL);
	if (m_bThickCheckMark)
	{
		rc.OffsetRect(0, XTP_DPI_Y(1));
		_DrawCheckMark(pDC, rc, clr, bLayoutRTL);
	}
}

void CXTPPaintManager::DrawComboExpandMark(CDC* pDC, CRect rc, COLORREF clr)
{
	rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rc, clr);
}

void CXTPPaintManager::FillCompositeRect(CDC* pDC, LPCRECT lpRect, COLORREF clrPen)
{
	if (!lpRect)
		return;

	CSize sz(lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
	CBitmap bmp;
	LPDWORD lpBits;
	bmp.Attach(
		CXTPImageManager::Create32BPPDIBSection(pDC->GetSafeHdc(), sz.cx, sz.cy, (LPBYTE*)&lpBits));
	if (!lpBits)
		return;

	CXTPCompatibleDC dc(pDC, &bmp);

	DWORD clr = 0xFF000000 + RGB(GetBValue(clrPen), GetGValue(clrPen), GetRValue(clrPen));

	for (int i = sz.cx * sz.cy - 1; i >= 0; i--)
	{
		*lpBits++ = clr;
	}

	pDC->BitBlt(lpRect->left, lpRect->top, sz.cx, sz.cy, &dc, 0, 0, SRCCOPY);
}

void CXTPPaintManager::HorizontalLine(CDC* pDC, int x0, int y, int x1, COLORREF clrPen)
{
	// do not scale the height
	pDC->FillSolidRect(x0, y, x1 - x0, 1, clrPen);
}

void CXTPPaintManager::VerticalLine(CDC* pDC, int x, int y0, int y1, COLORREF clrPen)
{
	// do not scale the width
	pDC->FillSolidRect(x, y0, 1, y1 - y0, clrPen);
}

void CXTPPaintManager::Line(CDC* pDC, CPoint p0, CPoint p1)
{
	pDC->MoveTo(p0);
	pDC->LineTo(p1);
}

void CXTPPaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF clrPen)
{
	if (x0 == x1)
	{
		VerticalLine(pDC, x0, y0, y1, clrPen);
	}
	else if (y0 == y1)
	{
		HorizontalLine(pDC, x0, y0, x1, clrPen);
	}
	else
	{
		CXTPPenDC pen(*pDC, clrPen);
		pDC->MoveTo(x0, y0);
		pDC->LineTo(x1, y1);
	}
}

void CXTPPaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, const int nPen)
{
	Line(pDC, x0, y0, x1, y1, GetXtremeColor(XTPToUIntChecked(nPen)));
}

void CXTPPaintManager::Pixel(CDC* pDC, int x, int y, int nPen)
{
	pDC->SetPixel(x, y, GetXtremeColor(XTPToUIntChecked(nPen)));
}

void CXTPPaintManager::StripMnemonics(CString& strClear)
{
	CXTPDrawHelpers::StripMnemonics(strClear);
}

void CXTPPaintManager::SplitString(const CString& str, CString& strFirstRow, CString& strSecondRow)
{
	strFirstRow = str;
	strSecondRow.Empty();

	int nIndex = str.Find(_T(' '));
	if (nIndex < 1)
		return;

	int nCenter = str.GetLength() / 2;
	for (int i = 0; i < nCenter - 1; i++)
	{
		ASSERT(nCenter - i >= 0 && nCenter + i + 1 < str.GetLength());

		if (str[nCenter - i] == _T(' '))
		{
			strFirstRow  = str.Left(nCenter - i);
			strSecondRow = str.Mid(nCenter - i + 1);
			return;
		}

		if (str[nCenter + i + 1] == _T(' '))
		{
			strFirstRow  = str.Left(nCenter + i + 1);
			strSecondRow = str.Mid(nCenter + i + 2);
			return;
		}
	}
}

#define XTP_DTT_COMPOSITED (1UL << 13) // Draws text with antialiased alpha (needs a DIB section)
#define XTP_DTT_TEXTCOLOR (1UL << 0)

void CXTPPaintManager::DrawTextApi(CDC* pDC, const CString& str, LPRECT lpRect, UINT format,
								   BOOL bComposited)
{
	if (!bComposited)
	{
		pDC->DrawText(str, lpRect, format);
	}
	else
	{
		CXTPWinThemeWrapper wrapper(FALSE);
		wrapper.OpenThemeData(NULL, L"GLOBALS");

		if (!wrapper.IsAppThemed())
		{
			pDC->DrawText(str, lpRect, format);
			return;
		}

		CRect rcBuffer(lpRect);

		HDC hDC			= 0;
		HPAINTBUFFER pb = wrapper.BeginBufferedPaint(pDC->GetSafeHdc(), rcBuffer,
													 XTP_BPBF_TOPDOWNDIB, 0, &hDC);

		if (hDC != NULL)
		{
			::BitBlt(hDC, rcBuffer.left, rcBuffer.top, rcBuffer.Width(), rcBuffer.Height(),
					 pDC->GetSafeHdc(), rcBuffer.left, rcBuffer.top, SRCCOPY);

			XTP_UX_DTTOPTS op;
			op.dwSize  = sizeof(op);
			op.dwFlags = XTP_DTT_COMPOSITED | XTP_DTT_TEXTCOLOR;
			op.crText  = pDC->GetTextColor();

			CXTPFontDC font(CDC::FromHandle(hDC), pDC->GetCurrentFont());

			wrapper.DrawThemeTextEx(hDC, 0, 0, XTP_CT2CW(str), -1, format, rcBuffer, &op);
		}

		wrapper.EndBufferedPaint(pb, TRUE);
	}
}

void CXTPPaintManager::DrawControlText3(CDC* pDC, CXTPControl* pButton, const CString& str,
										const CString& strClear, CRect rcText, BOOL bVert,
										BOOL bCentered)
{
	BOOL bEmbossed		   = !pButton->GetEnabled() && m_bEmbossedDisabledText;
	BOOL bShowKeyboardCues = pButton->GetParent()->IsKeyboardCuesVisible();

	if (m_bShowKeyboardCues > -1)
		bShowKeyboardCues = m_bShowKeyboardCues;

	UINT nFormat = UINT(DT_SINGLELINE | (bCentered ? DT_CENTER : DT_LEFT));

	if (!bVert)
	{
		if (bEmbossed)
		{
			COLORREF clrText = pDC->GetTextColor();
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(strClear, rcText + XTP_DPI(CPoint(1, 1)),
						  nFormat | DT_VCENTER | DT_NOPREFIX);
			pDC->SetTextColor(clrText);
		}

		if (bShowKeyboardCues)
		{
			DrawTextApi(pDC, str, &rcText, nFormat | DT_VCENTER, pButton->HasDwmCompositedRect());
		}
		else
		{
			DrawTextApi(pDC, strClear, &rcText, nFormat | DT_VCENTER | DT_NOPREFIX,
						pButton->HasDwmCompositedRect());
		}
	}
	else
	{
		if (bEmbossed)
		{
			COLORREF clrText = pDC->GetTextColor();
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(strClear, rcText + XTP_DPI(CPoint(1, 1)),
						  nFormat | DT_NOPREFIX | DT_NOCLIP);
			pDC->SetTextColor(clrText);
		}

		pDC->DrawText(strClear, &rcText, nFormat | DT_NOPREFIX | DT_NOCLIP);
	}
}

void CXTPPaintManager::DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt, BOOL bSelected,
										 BOOL bPopuped, BOOL bEnabled, BOOL bVert)
{
	COLORREF clr = GetRectangleTextColor(bSelected, FALSE, bEnabled, FALSE, bPopuped,
										 xtpBarTypeNormal, xtpBarTop);

	XTPPrimitiveSymbol symb = GetControlSymbol(pControl->GetParent()->GetPosition(), bVert);
	int size				= XTP_COMMANDBAR_TRIANGLE_SIZE / 2;

	XTPPrimitiveDrawer()->DrawSymbol(pDC, symb,
									 CRect(pt.x - size, pt.y - size, pt.x + size, pt.y + size),
									 clr);
}

XTPPrimitiveSymbol CXTPPaintManager::GetControlSymbol(XTPBarPosition barPosition, BOOL bVert) const
{
	XTPPrimitiveSymbol symb(xtpPrimitiveSymbolInvalid);

	switch (barPosition)
	{
		case xtpBarTop: symb = xtpPrimitiveSymbolPlayDown; break;
		case xtpBarBottom: symb = xtpPrimitiveSymbolPlayUp; break;
		case xtpBarLeft: symb = xtpPrimitiveSymbolPlayRight; break;
		case xtpBarRight: symb = xtpPrimitiveSymbolPlayLeft; break;
		case xtpBarFloating:
		case xtpBarPopup:
		case xtpBarListBox:
		case xtpBarNone:
		default: symb = bVert ? xtpPrimitiveSymbolPlayLeft : xtpPrimitiveSymbolPlayDown; break;
	};

	return symb;
}

CSize CXTPPaintManager::DrawControlText2(CDC* pDC, CXTPControl* pButton, CRect rcText, BOOL bDraw,
										 BOOL bVert, BOOL bTriangled)
{
	CString str = pButton->GetCaption();
	CString strClear(str);
	StripMnemonics(strClear);

	CString strFirstRow, strSecondRow;
	if (m_bWrapCaptionBelowText)
		SplitString(str, strFirstRow, strSecondRow);
	else
		strFirstRow = str;

	CString strFirstRowClear(strFirstRow), strSecondRowClear(strSecondRow);
	StripMnemonics(strFirstRowClear);
	StripMnemonics(strSecondRowClear);

	if (!bDraw)
	{
		CSize szFirstRow  = pDC->GetTextExtent(strClear.IsEmpty() ? CString(_T(" "))
																  : strFirstRowClear);
		CSize szSecondRow = pDC->GetTextExtent(strClear.IsEmpty() ? CString(_T(" "))
																  : strSecondRowClear);
		if (bTriangled)
		{
			szSecondRow.cx += XTP_COMMANDBAR_TRIANGLE_SIZE;
		}
		CSize sz(max(szFirstRow.cx, szSecondRow.cx), !bVert ? szFirstRow.cy * 2 : XTP_DPI_Y(24));
		return bVert ? CSize(sz.cy, sz.cx) : sz;
	}

	if (!bVert)
	{
		// Draw first row
		CRect rcFirstRowText(rcText.left, rcText.top, rcText.right, rcText.CenterPoint().y + 1);

		// Added 2px offset on first row for all buttons (with one or two text rows) to fix issue
		// under high DPI.
		rcFirstRowText.OffsetRect(0, XTP_COMMANDBAR_BUTTON_TEXT_OFFSET);

		DrawControlText3(pDC, pButton, strFirstRow, strFirstRowClear, rcFirstRowText, bVert, TRUE);

		// Draw second row
		CRect rcSecondRowText(rcText.left, rcText.CenterPoint().y, rcText.right, rcText.bottom);
		int nSecondRowWidth = pDC->GetTextExtent(strSecondRowClear).cx
							  + (bTriangled ? XTP_COMMANDBAR_TRIANGLE_SIZE : 0);
		rcSecondRowText.left += (rcText.Width() - nSecondRowWidth) / 2;
		if (!strSecondRow.IsEmpty())
		{
			DrawControlText3(pDC, pButton, strSecondRow, strSecondRowClear, rcSecondRowText, bVert,
							 FALSE);
		}

		// Draw triangle
		if (bTriangled)
		{
			CPoint pt = CPoint(strSecondRow.IsEmpty() ? rcText.CenterPoint().x - XTP_DPI_X(1)
													  : rcSecondRowText.left + nSecondRowWidth
															- XTP_COMMANDBAR_TRIANGLE_SIZE / 2,
							   rcSecondRowText.CenterPoint().y);
			if (strSecondRow.IsEmpty())
			{
				pt.y += XTP_COMMANDBAR_TRIANGLE_SIZE / 2;
			}
			DrawDropDownGlyph(pDC, pButton, pt, pButton->GetSelected(), pButton->GetPopuped(),
							  pButton->GetEnabled(), bVert);
		}
	}
	else
	{
		CRect rcSecondRowText(rcText.left + XTP_DPI_X(1), rcText.top,
							  rcText.CenterPoint().x + XTP_DPI_X(1), rcText.bottom);
		CRect rcFirstRowText(rcText.CenterPoint().x, rcText.top, rcText.right - XTP_DPI_X(1),
							 rcText.bottom);

		CSize szText		= pDC->GetTextExtent(strFirstRowClear);
		rcFirstRowText.left = rcFirstRowText.right
							  - (rcFirstRowText.Width() - szText.cy + XTP_DPI_X(1)) / 2;
		rcFirstRowText.top = rcFirstRowText.top
							 + (rcFirstRowText.Height() - szText.cx + XTP_DPI_Y(1)) / 2;
		DrawControlText3(pDC, pButton, strFirstRow, strFirstRowClear, rcFirstRowText, bVert, FALSE);

		szText = pDC->GetTextExtent(strSecondRowClear);
		szText.cx += (bTriangled ? XTP_COMMANDBAR_TRIANGLE_SIZE : 0);
		CRect rcSecondRow(rcSecondRowText);
		rcSecondRowText.left = rcSecondRowText.right
							   - (rcSecondRowText.Width() - szText.cy + XTP_DPI_Y(1)) / 2;
		rcSecondRowText.top = rcSecondRowText.top
							  + (rcSecondRowText.Height() - szText.cx + XTP_DPI_X(1)) / 2;
		DrawControlText3(pDC, pButton, strSecondRow, strSecondRowClear, rcSecondRowText, bVert,
						 FALSE);

		if (bTriangled)
		{
			CPoint pt = CPoint(rcSecondRow.CenterPoint().x - XTP_DPI_X(2),
							   rcSecondRowText.top + szText.cx - XTP_COMMANDBAR_TRIANGLE_SIZE / 2);
			DrawDropDownGlyph(pDC, pButton, pt, pButton->GetSelected(), pButton->GetPopuped(),
							  pButton->GetEnabled(), bVert);
		}
	}

	return 0;
}

CSize CXTPPaintManager::DrawControlText(CDC* pDC, CXTPControl* pButton, CRect rcText, BOOL bDraw,
										BOOL bVert, BOOL bCentered, BOOL bTriangled)
{
	CString str = pButton->GetCaption();

	CString strClear(str);
	StripMnemonics(strClear);

	if (!bDraw)
	{
		CSize sz = pDC->GetTextExtent(strClear.IsEmpty() ? CString(_T(" ")) : strClear);
		if (bTriangled)
			sz.cx += m_bOffice2007Padding ? XTP_COMMANDBAR_TRIANGLE_SIZE
										  : XTP_COMMANDBAR_TRIANGLE_SIZE + XTP_DPI_X(2);
		return bVert ? CSize(sz.cy, sz.cx) : sz;
	}

	if (!bVert)
	{
		int nWidth = pDC->GetTextExtent(strClear).cx + (bTriangled ? XTP_DPI_X(6) : 0);

		if (bCentered)
		{
			rcText.left += (rcText.Width() - nWidth) / 2;
		}

		DrawControlText3(pDC, pButton, str, strClear, rcText, bVert, FALSE);

		if (bTriangled)
		{
			CPoint pt = CPoint(rcText.left + nWidth, rcText.CenterPoint().y);
			DrawDropDownGlyph(pDC, pButton, pt, pButton->GetSelected(), pButton->GetPopuped(),
							  pButton->GetEnabled(), bVert);
		}
	}
	else
	{
		CRect rc(rcText);
		CSize szText = pDC->GetTextExtent(strClear);
		szText.cx += (bTriangled ? XTP_DPI_X(5) : 0);
		rcText.left = rcText.right - (rcText.Width() - szText.cy + XTP_DPI_Y(1)) / 2;
		if (bCentered)
		{
			rcText.top = rcText.top + (rcText.Height() - szText.cx + XTP_DPI_X(1)) / 2;
		}

		DrawControlText3(pDC, pButton, str, strClear, rcText, bVert, FALSE);

		if (bTriangled)
		{
			CPoint pt = CPoint(rc.CenterPoint().x, rcText.top + szText.cx - XTP_DPI_X(1));
			DrawDropDownGlyph(pDC, pButton, pt, pButton->GetSelected(), pButton->GetPopuped(),
							  pButton->GetEnabled(), bVert);
		}
	}
	return 0;
}

void CXTPPaintManager::UseOfficeFont(bool bUseOfficeFont)
{
	m_bUseOfficeFont = bUseOfficeFont;
	RefreshMetrics();
}

BOOL CXTPPaintManager::FontExists(LPCTSTR strFaceName)
{
	return CXTPDrawHelpers::FontExists(strFaceName);
}

void CXTPPaintManager::SetCommandBarsFontIndirect(LOGFONT* pLogFont,
												  BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	if (m_bClearTypeTextQuality && XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		pLogFont->lfQuality = CLEARTYPE_QUALITY;
	}

	m_xtpFontRegular.DeleteObject();
	m_xtpFontRegularBold.DeleteObject();
	m_xtpFontVert.DeleteObject();
	m_xtpFontVertBold.DeleteObject();
	m_fontIconBold.DeleteObject();

	// see if the Tahoma font was found.
	BOOL bTahomaExists = FontExists(m_strOfficeFont);

	// see if the system font is set to the Tahoma font, if so set the office font flag to true.
	bool bTahomaLF = m_strOfficeFont.CompareNoCase(pLogFont->lfFaceName) == 0;
	if (bTahomaLF)
	{
		m_bUseOfficeFont = true;
	}

	// set the office font if applicable.
	// BOOL bUseSystemFont = (pLogFont->lfCharSet > SYMBOL_CHARSET);
	if (bUseStandardFont)
	{
		if (m_bUseOfficeFont && bTahomaExists)
			STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, m_strOfficeFont);
	}

	m_xtpFontRegular.CreateFontIndirect(pLogFont);

	int nWeight = pLogFont->lfWeight;

	pLogFont->lfWeight = FW_BOLD;
	m_xtpFontRegularBold.CreateFontIndirect(pLogFont);

	pLogFont->lfOrientation = 900;
	pLogFont->lfEscapement  = 2700;
	pLogFont->lfWeight		= nWeight;
	STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE,
			 CXTPDrawHelpers::GetVerticalFontName(m_bUseOfficeFont));

	m_xtpFontVert.CreateFontIndirect(pLogFont);

	pLogFont->lfWeight = FW_BOLD;
	m_xtpFontVertBold.CreateFontIndirect(pLogFont);

	CWindowDC dc(NULL);
	CXTPFontDC font(&dc, &m_xtpFontRegular);
	m_nTextHeight = dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(6);
}

void CXTPPaintManager::SetFontHeight(int nFontHeight)
{
	m_nFontHeight = nFontHeight;
	RefreshMetrics();

	if (m_pFramePaintManager)
	{
		if (m_nFontHeight)
		{
			LOGFONT lf;
			m_pFramePaintManager->m_fntFrameCaption.GetLogFont(&lf);
			lf.lfHeight = -m_nFontHeight;
			m_pFramePaintManager->m_fntFrameCaption.SetCustomFont(&lf);
		}
		else
		{
			m_pFramePaintManager->m_fntFrameCaption.SetCustomFont(LPLOGFONT(NULL));
		}
	}
}

int CXTPPaintManager::GetTabsHeight() const
{
	int nTextHeight = GetTextHeight();
	return nTextHeight + XTP_DPI_Y(4);
}

void CXTPPaintManager::UpdateFonts()
{
	CXTPNonClientMetrics ncm;
	BYTE lfCharSet	 = XTPResourceManager()->GetFontCharset();
	BOOL bUseClearType = m_bClearTypeTextQuality
						 && XTPSystemVersion()->IsClearTypeTextQualitySupported();

	if (m_bUseStandardFont)
	{
		CWindowDC dc(NULL);
		CLogFont lf;
		lf.lfHeight = ncm.lfMenuFont.lfHeight < 0
						  ? min(-XTP_COMMANDBAR_MIN_FONT_HEIGHT, ncm.lfMenuFont.lfHeight)
						  : ncm.lfMenuFont.lfHeight;
		lf.lfWeight  = ncm.lfMenuFont.lfWeight;
		lf.lfItalic  = ncm.lfMenuFont.lfItalic;
		lf.lfCharSet = lfCharSet;
		lf.lfQuality = ncm.lfMenuFont.lfQuality;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, ncm.lfMenuFont.lfFaceName);
		if (bUseClearType)
		{
			lf.lfQuality = CLEARTYPE_QUALITY;
		}

		if (0 < m_nFontHeight)
		{
			lf.lfHeight = -m_nFontHeight;
		}

		m_nTextHeight = CalcTextHeight(lf.lfHeight, XTP_COMMANDBAR_TEXT_HEIGHT_FACTOR,
									   XTP_DEFAULT_DISPLAY_DPI);
		m_nEditHeight = CalcCtrlHeight(lf.lfHeight, XTP_COMMANDBAR_CONTROL_HEIGHT_FACTOR,
									   XTP_DEFAULT_DISPLAY_DPI);
		m_nSplitButtonDropDownWidth  = CalcSplitButtonDropDownWidth(m_nTextHeight);
		m_nSplitButtonDropDownHeight = CalcSplitButtonDropDownHeight(lf.lfHeight);

		SetCommandBarsFontIndirect(&lf, TRUE);
	}

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	if (bUseClearType)
	{
		lfIcon.lfQuality			  = CLEARTYPE_QUALITY;
		ncm.lfSmCaptionFont.lfQuality = CLEARTYPE_QUALITY;
		ncm.lfStatusFont.lfQuality	= CLEARTYPE_QUALITY;
	}

	lfIcon.lfHeight  = m_nFontHeight ? -m_nFontHeight : lfIcon.lfHeight;
	lfIcon.lfCharSet = lfCharSet;
	VERIFY(m_fontIcon.SetStandardFont(&lfIcon));

	lfIcon.lfWeight = FW_BOLD;
	VERIFY(m_fontIconBold.SetStandardFont(&lfIcon));

	ncm.lfSmCaptionFont.lfCharSet = lfCharSet;
	m_fontSmCaption.SetStandardFont(&ncm.lfSmCaptionFont);

	ncm.lfStatusFont.lfCharSet = lfCharSet;
	m_fontToolTip.SetStandardFont(&ncm.lfStatusFont);
}

void CXTPPaintManager::DrawGalleryItemBack(CDC* pDC, CRect rcItem, BOOL bEnabled, BOOL bSelected,
										   BOOL bPressed, BOOL bChecked, XTPBarType barType)
{
	DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, barType,
				  xtpBarPopup);
}

BOOL CXTPPaintManager::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pButton);
	return FALSE;
}

void CXTPPaintManager::DrawControlEntry(CDC* pDC, CXTPControl* pButton)
{
#ifdef _XTP_INCLUDE_RIBBON
	if (pButton->GetRibbonGroup() && m_pRibbonPaintManager)
	{
		if (m_pRibbonPaintManager->DrawRibbonGroupControlEntry(pDC, pButton))
			return;
	}
#endif
	if (!DrawRectangle(pDC, pButton))
	{
		DrawRectangle(pDC, pButton->GetRect(), pButton->GetSelected(), pButton->GetPressed(),
					  pButton->GetEnabled(), pButton->GetChecked(), pButton->GetPopuped(),
					  pButton->GetParent()->GetType(), pButton->GetParent()->GetPosition());
	}
}

COLORREF CXTPPaintManager::GetControlTextColor(CXTPControl* pButton)
{
	BOOL bEnabled = pButton->GetType() == xtpControlSplitButtonPopup
						? (pButton->GetStyle() != xtpButtonIconAndCaptionBelow
							   ? IsControlCommandEnabled(pButton->GetEnabled())
							   : IsControlDropDownEnabled(pButton->GetEnabled()))
						: pButton->GetEnabled();

	BOOL bCheckBoxStyle = (pButton->GetType() == xtpControlCheckBox
						   || pButton->GetType() == xtpControlRadioButton)
						  && (pButton->GetParent()->GetType() != xtpBarTypePopup);

	if (bCheckBoxStyle)
		return GetRectangleTextColor(FALSE, FALSE, bEnabled, FALSE, FALSE,
									 pButton->GetParent()->GetType(),
									 pButton->GetParent()->GetPosition());

	return GetRectangleTextColor(pButton->GetSelected(), pButton->GetPressed(), bEnabled,
								 pButton->GetChecked(), pButton->GetPopuped(),
								 pButton->GetParent()->GetType(),
								 pButton->GetParent()->GetPosition());
}

void CXTPPaintManager::DrawCommandBarGroupSeparator(CDC* pDC, CXTPCommandBar* pBar,
													CXTPControl* pLeftControl,
													CXTPControl* pRightControl,
													BOOL bDraw /*= TRUE*/)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pBar);
	UNREFERENCED_PARAMETER(pLeftControl);
	UNREFERENCED_PARAMETER(pRightControl);
	UNREFERENCED_PARAMETER(bDraw);
}

int CXTPPaintManager::GetPopupBarGripperWidth(CXTPCommandBar* pBar)
{
	ASSERT_VALID(pBar);

	if (pBar->GetType() != xtpBarTypePopup)
		return 0;

	int nDoubleGripper = pBar->IsPopupBar() && ((CXTPPopupBar*)pBar)->IsDoubleGripper() ? 2 : 1;
	CSize szImage(GetPopupBarImageSize(pBar));

	return szImage.cx * nDoubleGripper + XTP_DPI_X(1);
}

COLORREF CXTPPaintManager::GetControlEditBackColor(CXTPControl* pControl)
{
	ASSERT_VALID(pControl);
	return GetXtremeColor(UINT(pControl->GetEnabled() ? COLOR_WINDOW : COLOR_3DFACE));
}

CSize CXTPPaintManager::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit, BOOL bDraw)
{
	if (bDraw)
	{
		ASSERT_VALID(pControlEdit);

		if (pControlEdit->GetParent()->GetType() == xtpBarTypePopup)
		{
			DrawControlPopupParent(pDC, pControlEdit, TRUE);
		}
		else
		{
			CRect rc = pControlEdit->GetRect();
			CRect rcLabelText(rc);
			rcLabelText.left += XTP_DPI_X(2);
			BOOL bEnabled = pControlEdit->GetEnabled();

			if (pControlEdit->IsImageVisible())
			{
				CSize szIcon = pControlEdit->GetIconSize();

				CXTPImageManagerIcon* pImage = pControlEdit->GetImage(szIcon.cx);
				szIcon = AdjustDpiIconSize(pImage, szIcon, pControlEdit, rc.Size());

				CPoint pt = CPoint(rc.left + XTP_DPI_X(3), rc.CenterPoint().y - szIcon.cy / 2);
				DrawImage(pDC, pt, szIcon, pImage, FALSE, FALSE, bEnabled, FALSE, FALSE);

				rcLabelText.left = rc.left + szIcon.cx + XTP_DPI_X(3) * 2;
			}

			if (pControlEdit->IsCaptionVisible())
			{
				CXTPFontDC fontLabel(pDC, GetRegularFont());

				pDC->SetTextColor(GetRectangleTextColor(FALSE, FALSE, bEnabled, FALSE, FALSE,
														pControlEdit->GetParent()->GetType(),
														pControlEdit->GetParent()->GetPosition()));
				DrawControlText(pDC, pControlEdit, rcLabelText, TRUE, FALSE, FALSE, FALSE);
			}
		}
		return 0;
	}

	BOOL bPopupBar = pControlEdit->GetParent()->GetType() == xtpBarTypePopup;
	CString strCaption(pControlEdit->GetCaption());

	StripMnemonics(strCaption);

	CXTPFontDC font(pDC, GetIconFont());
	int nHeight = m_nEditHeight > 0 ? m_nEditHeight
									: pDC->GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(7);

	font.SetFont(GetRegularFont());

	BOOL bCaptionVisible = pControlEdit->IsCaptionVisible();

	if (bCaptionVisible && m_nEditHeight == 0)
		nHeight = max(nHeight, pDC->GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(7));

	int nCaptionWidth = pDC->GetTextExtent(strCaption).cx + XTP_DPI_X(5);

	if (bPopupBar)
	{
		int nGripperWidth = GetPopupBarGripperWidth(pControlEdit->GetParent());
		pControlEdit->SetLabelWidth(nCaptionWidth + nGripperWidth + m_nPopupBarTextPadding);

		return CSize(pControlEdit->GetWidth() + nGripperWidth, nHeight);
	}

	BOOL bImageVisible = pControlEdit->IsImageVisible();
	CSize szIcon	   = CSize(0);
	if (bImageVisible)
	{
		szIcon			  = pControlEdit->GetIconSize();
		CSize szAvailable = CSize(XTP_DPI_X(static_cast<int>(szIcon.cx)) + XTP_DPI_X(2) * 2,
								  nHeight);
		CXTPImageManagerIcon* pImage = pControlEdit->GetImage(szIcon.cx);
		szIcon						 = AdjustDpiIconSize(pImage, szIcon, pControlEdit, szAvailable);
	}

	int nLabelWidth = (bImageVisible ? szIcon.cx + XTP_DPI_X(3) * 2 : 0)
					  + (bCaptionVisible ? nCaptionWidth : 0);

	if (bImageVisible)
		nHeight = max(nHeight, pControlEdit->GetButtonSize().cy);

	pControlEdit->SetLabelWidth(nLabelWidth);

	return CSize(pControlEdit->GetWidth(), nHeight);
}

CSize CXTPPaintManager::DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pControlCombo,
											BOOL bDraw)
{
	if (bDraw)
	{
		if (pControlCombo->GetParent()->GetType() == xtpBarTypePopup)
		{
			DrawControlPopupParent(pDC, pControlCombo, TRUE);
		}
		else
		{
			CRect rc = pControlCombo->GetRect();
			CRect rcLabelText(rc);
			rcLabelText.left += XTP_DPI_X(2);
			BOOL bEnabled = pControlCombo->GetEnabled();

			if (pControlCombo->IsImageVisible())
			{
				CSize szIcon = pControlCombo->GetIconSize();

				CXTPImageManagerIcon* pImage = pControlCombo->GetImage(szIcon.cx);
				szIcon = AdjustDpiIconSize(pImage, szIcon, pControlCombo, rc.Size());

				CPoint pt = CPoint(rc.left + XTP_DPI_X(3), rc.CenterPoint().y - szIcon.cy / 2);
				DrawImage(pDC, pt, szIcon, pImage, FALSE, FALSE, bEnabled, FALSE, FALSE);

				rcLabelText.left = rc.left + szIcon.cx + XTP_DPI_X(3) * 2;
			}

			if (pControlCombo->IsCaptionVisible())
			{
				CXTPFontDC fontLabel(pDC, GetRegularFont());

				pDC->SetTextColor(GetRectangleTextColor(FALSE, FALSE, bEnabled, FALSE, FALSE,
														pControlCombo->GetParent()->GetType(),
														pControlCombo->GetParent()->GetPosition()));
				DrawControlText(pDC, pControlCombo, rcLabelText, TRUE, FALSE, FALSE, FALSE);
			}
		}
		return 0;
	}

	BOOL bPopupBar = pControlCombo->GetParent()->GetType() == xtpBarTypePopup;
	CString strCaption(pControlCombo->GetCaption());

	StripMnemonics(strCaption);

	CXTPFontDC font(pDC, GetIconFont());
	int nHeight = m_nEditHeight > 0
					  ? m_nEditHeight
					  : max(XTP_DPI_Y(20), pDC->GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(7));

	pControlCombo->SetThumbWidth(
		max(XTP_DPI_X(15), MulDiv(nHeight, XTP_DPI_X(9), XTP_DPI_X(14)) + XTP_DPI_X(1)));

	font.SetFont(GetRegularFont());

	BOOL bCaptionVisible = pControlCombo->IsCaptionVisible();

	if (bCaptionVisible && m_nEditHeight == 0)
		nHeight = max(nHeight, pDC->GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(7));

	int nCaptionWidth = pDC->GetTextExtent(strCaption).cx + XTP_DPI_X(5);

	if (bPopupBar)
	{
		int nGripperWidth = GetPopupBarGripperWidth(pControlCombo->GetParent());
		pControlCombo->SetLabelWidth(nCaptionWidth + nGripperWidth + m_nPopupBarTextPadding);

		return CSize(pControlCombo->GetWidth() + nGripperWidth, nHeight);
	}

	BOOL bImageVisible = pControlCombo->IsImageVisible();
	CSize szIcon	   = CSize(0);
	if (bImageVisible)
	{
		szIcon			  = pControlCombo->GetIconSize();
		CSize szAvailable = CSize(XTP_DPI_X(static_cast<int>(szIcon.cx)) + XTP_DPI_X(2) * 2,
								  nHeight);
		CXTPImageManagerIcon* pImage = pControlCombo->GetImage(szIcon.cx);
		szIcon = AdjustDpiIconSize(pImage, szIcon, pControlCombo, szAvailable);
	}

	int nLabelWidth = (bImageVisible ? szIcon.cx + XTP_DPI_X(3) * 2 : 0)
					  + (bCaptionVisible ? nCaptionWidth : 0);

	if (bImageVisible)
		nHeight = max(nHeight, pControlCombo->GetParent()->GetButtonSize().cy);

	pControlCombo->SetLabelWidth(nLabelWidth);

	return CSize(pControlCombo->GetWidth(), nHeight);
}

CSize CXTPPaintManager::DrawControl(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	if (pButton->GetType() == xtpControlEdit)
	{
		return DrawControlEdit(pDC, (CXTPControlEdit*)pButton, bDraw);
	}

	if (pButton->GetType() == xtpControlComboBox)
	{
		return DrawControlComboBox(pDC, (CXTPControlComboBox*)pButton, bDraw);
	}

	if (pButton->GetParent()->GetType() == xtpBarTypePopup)
	{
		return DrawControlPopupParent(pDC, pButton, bDraw);
	}

	return DrawControlToolBarParent(pDC, pButton, bDraw);
}

void CXTPPaintManager::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect((CWnd*)pBar), GetXtremeColor(COLOR_3DFACE));
}

XTPBarTextOrientation CXTPPaintManager::GetControlTextOrientation(CXTPControl* pControl)
{
	XTPBarTextOrientation nTextOrientation = pControl->GetParent()->GetTextOrientation();
	if (nTextOrientation != xtpBarTextAuto)
		return nTextOrientation;

	return (pControl->GetParent()->GetPosition() == xtpBarLeft
			|| pControl->GetParent()->GetPosition() == xtpBarRight)
			   ? xtpBarTextVertical
			   : xtpBarTextHorizontal;
}

CFont* CXTPPaintManager::GetControlFont(CXTPControl* pControl)
{
	return GetControlTextOrientation(pControl) == xtpBarTextVertical
			   ? (pControl->IsItemDefault() ? &m_xtpFontVertBold : &m_xtpFontVert)
			   : (pControl->IsItemDefault() ? &m_xtpFontRegularBold : &m_xtpFontRegular);
}

CFont* CXTPPaintManager::GetCommandBarFont(CXTPCommandBar* pCommandBar)
{
	XTPBarTextOrientation nTextOrientation = pCommandBar->GetTextOrientation();
	if (nTextOrientation == xtpBarTextAuto)
	{
		nTextOrientation = pCommandBar->GetPosition() == xtpBarLeft
								   || pCommandBar->GetPosition() == xtpBarRight
							   ? xtpBarTextVertical
							   : xtpBarTextHorizontal;
	}

	return nTextOrientation == xtpBarTextVertical ? &m_xtpFontVert : &m_xtpFontRegular;
}

void CXTPPaintManager::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, CPoint pt0, CPoint pt1,
									   CPoint pt2, COLORREF clr)
{
	ASSERT(pDC);
	UNREFERENCED_PARAMETER(pButton);

	Triangle(pDC, pt0, pt1, pt2, clr);
}

void CXTPPaintManager::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, COLORREF clr, BOOL bVertical)
{
	ASSERT(pDC);
	ASSERT(pButton);

	CRect rc = pButton->GetRect();
	if (bVertical)
	{
		rc.top += rc.Height() / 2;
	}
	else
	{
		rc.left += rc.Width() / 2;
	}

	rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
	XTPPrimitiveDrawer()->DrawSymbol(pDC,
									 bVertical ? xtpPrimitiveSymbolPlayDown
											   : xtpPrimitiveSymbolPlayRight,
									 rc, clr);
}

void CXTPPaintManager::DrawHiddenGlyph(CDC* pDC, CXTPControl* pButton, COLORREF clr, BOOL bVertical)
{
	ASSERT(pDC);
	ASSERT(pButton);

	CRect rc = pButton->GetRect();

	if (bVertical)
	{
		rc.bottom -= rc.Height() / 2;
	}
	else
	{
		rc.right -= rc.Width() / 2;
	}

	if (bVertical)
	{
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolShiftDown, rc, clr);
	}
	else
	{
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolShiftRight, rc, clr);
	}
}

CSize CXTPPaintManager::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType,
										   CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw,
										   LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!IsVerticalPosition(pBar->GetPosition()))
				pButton->SetRect(CRect(szBar.cx - XTP_DPI_X(11) - XTP_DPI_X(2), XTP_DPI_Y(2),
									   szBar.cx - XTP_DPI_X(2), szBar.cy - XTP_DPI_Y(2)));
			else
				pButton->SetRect(CRect(XTP_DPI_X(2), szBar.cy - XTP_DPI_Y(11) - XTP_DPI_Y(2),
									   szBar.cx - XTP_DPI_X(2), szBar.cy - XTP_DPI_Y(2)));
		}
		else
		{
			DrawControlEntry(pDC, pButton);
			COLORREF clrText = GetControlTextColor(pButton);

			BOOL bHiddenExists = *(BOOL*)lpParam;
			BOOL bVertical	 = !IsVerticalPosition(pBar->GetPosition());
			DrawExpandGlyph(pDC, pButton, clrText, bVertical);
			if (bHiddenExists)
			{
				DrawHiddenGlyph(pDC, pButton, clrText, bVertical);
			}
		}
	}
	else if (controlType == xtpButtonExpandFloating)
	{
		DrawControlEntry(pDC, pButton);
		CRect rcButton   = pButton->GetRect();
		COLORREF clrText = GetControlTextColor(pButton);

		if (!pButton->GetSelected() && !pButton->GetPopuped())
			clrText = m_clrFloatingGripperText;

		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcButton, clrText);
	}
	else if (controlType == xtpButtonHideFloating)
	{
		DrawControlEntry(pDC, pButton);

		COLORREF crClose = (!pButton->GetPressed() && !pButton->GetSelected()
								? m_clrFloatingGripperText.operator COLORREF()
								: GetControlTextColor(pButton));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolClose, pButton->GetRect(), crClose);
	}
	else if (controlType == xtpButtonExpandMenu)
	{
		if (bDraw)
		{
			DrawControlEntry(pDC, pButton);
			CRect rcButton  = pButton->GetRect();
			CPoint ptCenter = rcButton.CenterPoint();

			CRect rcArrow = rcButton;
			rcArrow.left  = (rcButton.right - rcButton.left) / 2 - rcArrow.Height();
			rcArrow.right = (rcButton.right - rcButton.left) / 2 + rcArrow.Height();
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolShiftDown, rcArrow, 0);
		}

		return CSize(0, XTP_DPI_Y(12));
	}
	return 0;
}

CSize CXTPPaintManager::DrawControlText(CDC* pDC, CXTPControl* pButton, CRect rcText, BOOL bDraw,
										BOOL bTriangled, CSize szImage, BOOL bDrawImage)
{
	BOOL bVert = GetControlTextOrientation(pButton) == xtpBarTextVertical;

	if (!bVert)
	{
		if (bDrawImage)
		{
			rcText.left += m_bOffice2007Padding ? szImage.cx : szImage.cx - XTP_DPI_X(1);

			CSize sz = DrawControlText(pDC, pButton, &rcText, bDraw, bVert, FALSE, bTriangled);

			return CSize(XTP_DPI_X(m_bOffice2007Padding ? 4 : 3) + sz.cx + szImage.cx,
						 max(sz.cy + XTP_DPI_Y(6), szImage.cy));
		}
		else
		{
			CSize sz = DrawControlText(pDC, pButton, &rcText, bDraw, bVert, TRUE, bTriangled);
			return CSize(XTP_DPI_X(8) + sz.cx, max(sz.cy + XTP_DPI_Y(6), szImage.cy));
		}
	}
	else
	{
		if (bDrawImage)
		{
			rcText.top += szImage.cy;

			CSize sz = DrawControlText(pDC, pButton, &rcText, bDraw, bVert, FALSE, bTriangled);

			return CSize(max(sz.cx + XTP_DPI_X(6), szImage.cx), XTP_DPI_Y(3) + sz.cy + szImage.cx);
		}
		else
		{
			CSize sz = DrawControlText(pDC, pButton, &rcText, bDraw, bVert, TRUE, bTriangled);
			return CSize(max(sz.cx + XTP_DPI_X(6), szImage.cx), XTP_DPI_Y(8) + sz.cy);
		}
	}
}

CRect CXTPPaintManager::GetSplitButtonCommandRect(CXTPControl* pButton)
{
	CRect rc(pButton->GetRect());
	CXTPCommandBar* pParent = pButton->GetParent();

	if (pParent->GetType() != xtpBarTypePopup
		&& pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		int nSplitDropDownHeight = GetSplitDropDownHeight();

		BOOL bVert = GetControlTextOrientation(pButton) == xtpBarTextVertical;

		return bVert ? CRect(rc.left + nSplitDropDownHeight, rc.top, rc.right, rc.bottom)
					 : CRect(rc.left, rc.top, rc.right, rc.bottom - nSplitDropDownHeight);
	}

	int nSize = pParent->GetType() == xtpBarTypePopup ? m_nSplitButtonPopupWidth
													  : m_nSplitButtonDropDownWidth + 1;
	return CRect(rc.left, rc.top, rc.right - nSize, rc.bottom);
}
CSize CXTPPaintManager::DrawControlCheckBoxMark(CDC* pDC, CXTPControl* pButton, CRect rc,
												BOOL bDraw)
{
	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked();
	CRect rcButton(rc);
	if (!bDraw)
		rcButton = CXTPEmptyRect();
	return DrawControlCheckBoxMark(pDC, rcButton, bDraw, bSelected, bPressed, bChecked, bEnabled);
}

CSize CXTPPaintManager::DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected,
												BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (bDraw)
	{
		if (m_bThemedCheckBox && m_themeButton->IsAppThemeActive())
		{
			int nState = 0;
			if (!bEnabled)
				nState = 3;
			else if (bSelected && bPressed)
				nState = 2;
			else if (bSelected)
				nState = 1;

			if (bChecked == 1)
				nState += 4;

			if (bChecked == 2)
				nState += 8;

			m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_CHECKBOX, nState + 1, rc, 0);
		}
		else
		{
			pDC->DrawFrameControl(rc, UINT(DFC_BUTTON),
								  UINT(DFCS_BUTTONCHECK
									   | (!bEnabled || bChecked == 2 ? DFCS_INACTIVE : 0)
									   | (bChecked ? DFCS_CHECKED : 0)
									   | (bPressed ? DFCS_PUSHED : 0)));
		}
	}

	return XTP_DPI(CSize(13, 13));
}

CSize CXTPPaintManager::DrawControlRadioButtonMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected,
												   BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (bDraw)
	{
		if (m_bThemedCheckBox && m_themeButton->IsAppThemeActive())
		{
			int nState = 0;
			if (!bEnabled)
				nState = 3;
			else if (bSelected && bPressed)
				nState = 2;
			else if (bSelected)
				nState = 1;

			if (bChecked == 1)
				nState += 4;

			if (bChecked == 2)
				nState += 8;

			m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_RADIOBUTTON, nState + 1, rc,
											   0);
		}
		else
		{
			pDC->DrawFrameControl(rc, UINT(DFC_BUTTON),
								  UINT(DFCS_BUTTONRADIO
									   | (!bEnabled || bChecked == 2 ? DFCS_INACTIVE : 0)
									   | (bChecked ? DFCS_CHECKED : 0)
									   | (bPressed ? DFCS_PUSHED : 0)));
		}
	}

	return XTP_DPI(CSize(13, 13));
}

CSize CXTPPaintManager::GetControlSize(CXTPControl* pControl, CSize sz, BOOL bVert)
{
	if (bVert)
	{
		if (pControl->GetWidth() > 0)
			sz.cy = pControl->GetWidth();

		if (pControl->GetHeight() > 0)
			sz.cx = pControl->GetHeight();
	}
	else
	{
		if (pControl->GetWidth() > 0)
			sz.cx = pControl->GetWidth();

		if (pControl->GetHeight() > 0)
			sz.cy = pControl->GetHeight();
	}

	return sz;
}

CSize CXTPPaintManager::GetPopupBarImageSize(CXTPCommandBar* pBar)
{
	CSize szIcon = pBar->GetIconSize();
	CSize szImage(max(szIcon.cx + XTP_DPI_X(4), m_nTextHeight + XTP_DPI_X(4)),
				  max(szIcon.cy + XTP_DPI_Y(6), m_nTextHeight));
	return szImage;
}

void CXTPPaintManager::DrawPopupBarText(CDC* pDC, const CString& strText, CRect& rcText,
										UINT nFlags, BOOL bDraw, BOOL bSelected, BOOL bEnabled)
{
	if (bDraw && !bEnabled && !bSelected && m_bEmbossedDisabledText)
	{
		COLORREF clrText = pDC->GetTextColor();
		pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->DrawText(strText, rcText + XTP_DPI(CPoint(1, 1)), DT_SINGLELINE | DT_VCENTER | nFlags);
		pDC->SetTextColor(clrText);
	}

	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : nFlags));
}

CSize CXTPPaintManager::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pButton);

	CXTPCommandBar* pParent = pButton->GetParent();

	CRect rcText(0, 0, 0, 0), rcShortcut(0, 0, 0, 0);
	CRect rcButton				  = pButton->GetRect();
	XTPControlType controlType	= pButton->GetType();
	XTPButtonStyle buttonStyle	= pButton->GetStyle();
	CString strCaption			  = pButton->GetCaption();
	CString strCustomShortcutText = pButton->GetCustomShortcutText();

	BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup)
					  || (buttonStyle != xtpButtonCaption);

	CSize szImage(GetPopupBarImageSize(pParent));

	int nGripperWidth = GetPopupBarGripperWidth(pParent) - XTP_DPI_X(1);

	BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled();
	if (controlType == xtpControlSplitButtonPopup && bEnabled == TRUE_SPLITDROPDOWN)
		bEnabled = FALSE;

	if (controlType == xtpControlLabel)
	{
		CSize szIcon				 = GetIconSize(pButton);
		CXTPImageManagerIcon* pImage = pButton->GetIconId() != 0 && bDrawImage
										   ? pButton->GetImage(szIcon.cx)
										   : NULL;

		if (!m_bOffsetPopupLabelText && !pImage)
			nGripperWidth = 0;

		if (!bDrawImage)
			szImage.cy = m_nTextHeight;
	}

	if (bDraw)
	{
		rcText.SetRect(rcButton.left + nGripperWidth + m_nPopupBarTextPadding, rcButton.top,
					   rcButton.right, rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + nGripperWidth + XTP_DPI_X(8), rcButton.top,
						   rcButton.right - XTP_DPI_X(19), rcButton.bottom);

		if (controlType == xtpControlButton && pButton->GetCustomShortcutText().GetLength() != 0)
			rcText.right -= pDC->GetTextExtent(pButton->GetCustomShortcutText()).cx + XTP_DPI_X(19);
		else if (controlType == xtpControlButton && (pButton->GetShortcutText().GetLength() != 0))
			rcText.right -= pDC->GetTextExtent(pButton->GetShortcutText()).cx + XTP_DPI_X(19);
		else if (controlType == xtpControlSplitButtonPopup)
			rcText.right -= pParent->GetPaintManager()->m_nSplitButtonPopupWidth;
		else if (controlType == xtpControlPopup || controlType == xtpControlButtonPopup)
			rcText.right -= szImage.cx;
	}

	BOOL bShowKeyboardCues = pParent->IsKeyboardCuesVisible();

	if (!bShowKeyboardCues)
		StripMnemonics(strCaption);

	if (buttonStyle == xtpButtonCaptionAndDescription && !pButton->GetDescription().IsEmpty())
	{
		{
			CXTPFontDC font(pDC, GetRegularBoldFont());
			if (bDraw)
			{
				rcShortcut.bottom = rcText.bottom = rcText.top + pDC->GetTextExtent(_T(" "), 1).cy
													+ XTP_DPI_Y(10);
			}
			DrawPopupBarText(pDC, strCaption, rcText,
							 UINT(DT_END_ELLIPSIS | (!bShowKeyboardCues ? DT_NOPREFIX : 0)), bDraw,
							 bSelected, bEnabled);

			if (controlType == xtpControlButton
				&& pButton->GetCustomShortcutText().GetLength() != 0)
				DrawPopupBarText(pDC, pButton->GetCustomShortcutText(), rcShortcut, DT_RIGHT, bDraw,
								 bSelected, bEnabled);
			else if (controlType == xtpControlButton && pButton->GetShortcutText().GetLength() != 0)
				DrawPopupBarText(pDC, pButton->GetShortcutText(), rcShortcut, DT_RIGHT, bDraw,
								 bSelected, bEnabled);
		}

		if (bDraw)
		{
			CRect rcDescription(rcText.left, rcText.bottom - XTP_DPI_Y(2),
								rcButton.right - XTP_DPI_X(10), rcButton.bottom);
			pDC->DrawText(pButton->GetDescription(), rcDescription,
						  UINT(DT_WORDBREAK | DT_TOP | DT_NOPREFIX));
		}
		else
		{
			CSize szDescription = pDC->GetTextExtent(pButton->GetDescription());
			CRect rcDescription(0, 0, (szDescription.cx + XTP_DPI_X(20)) / 2, 0);
			pDC->DrawText(_T("1\n2"), rcDescription, UINT(DT_CALCRECT | DT_WORDBREAK));

			int nWidth = nGripperWidth + m_nPopupBarTextPadding + rcText.Width()
						 + rcShortcut.Width() + XTP_DPI_X(32);

			return GetControlSize(pButton,
								  CSize(max(nWidth, nGripperWidth + m_nPopupBarTextPadding
														+ szDescription.cx / 2 + XTP_DPI_X(32)),
										max(szImage.cy, rcDescription.Height() + rcText.Height()
															+ XTP_DPI_Y(19))),
								  FALSE);
		}
		return 0;
	}

	DrawPopupBarText(pDC, strCaption, rcText,
					 UINT(DT_END_ELLIPSIS | (!bShowKeyboardCues ? DT_NOPREFIX : 0)), bDraw,
					 bSelected, bEnabled);

	if ((controlType == xtpControlButton || controlType == xtpControlCheckBox)
		&& pButton->GetCustomShortcutText().GetLength() != 0)
		DrawPopupBarText(pDC, pButton->GetCustomShortcutText(), rcShortcut, DT_RIGHT, bDraw,
						 bSelected, bEnabled);
	else if ((controlType == xtpControlButton || controlType == xtpControlCheckBox)
			 && pButton->GetShortcutText().GetLength() != 0)
		DrawPopupBarText(pDC, pButton->GetShortcutText(), rcShortcut, DT_RIGHT, bDraw, bSelected,
						 bEnabled);

	return GetControlSize(pButton,
						  CSize(nGripperWidth + m_nPopupBarTextPadding + rcText.Width()
									+ rcShortcut.Width() + XTP_DPI_X(32),
								szImage.cy),
						  FALSE);
}

CSize CXTPPaintManager::GetIconSize(CXTPControl* pControl) const
{
	XTPButtonStyle buttonStyle = pControl->GetStyle();

	CSize szIcon = buttonStyle == xtpButtonIconAndCaptionBelow
						   && pControl->GetParent()->GetType() != xtpBarTypePopup
					   ? pControl->GetParent()->GetLargeIconSize(m_bAutoResizeIcons)
					   : pControl->GetIconSize();

	return szIcon;
}

CSize CXTPPaintManager::GetStretchIconSize(CXTPImageManagerIcon* pIcon, CSize szIcon)
{
	if (!pIcon)
		return szIcon;

	if (pIcon->GetWidth() == 0 || pIcon->GetHeight() == 0)
		return szIcon;

	int nHeight = XTP_DPI_Y(pIcon->GetHeight()) * szIcon.cx / XTP_DPI_X(pIcon->GetWidth());

	if (nHeight <= szIcon.cy + 1)
		return CSize(szIcon.cx, nHeight);

	int nWidth = szIcon.cy * XTP_DPI_X(pIcon->GetWidth()) / XTP_DPI_Y(pIcon->GetHeight());

	return CSize(nWidth, szIcon.cy);
}

BOOL CXTPPaintManager::IsDpiBitmapScalingEnabled(CXTPControl* pControl)
{
	if (NULL == pControl)
		return XTPDpiHelper()->IsDpiBitmapScalingEnabled();

	return IsDpiBitmapScalingEnabled(pControl->GetCommandBar());
}

BOOL CXTPPaintManager::IsDpiBitmapScalingEnabled(CXTPCommandBar* pCommandBar)
{
	if (NULL == pCommandBar)
		return XTPDpiHelper()->IsDpiBitmapScalingEnabled();

	return IsDpiBitmapScalingEnabled(pCommandBar->GetCommandBars());
}

BOOL CXTPPaintManager::IsDpiBitmapScalingEnabled(CXTPCommandBars* pCommandBars)
{
	if (NULL == pCommandBars)
		return XTPDpiHelper()->IsDpiBitmapScalingEnabled();

	if (NULL == pCommandBars->GetCommandBarsOptions())
		return XTPDpiHelper()->IsDpiBitmapScalingEnabled();

	return pCommandBars->GetCommandBarsOptions()->IsDpiBitmapScalingEnabled();
}

CSize CXTPPaintManager::AdjustDpiIconSize(CXTPImageManagerIcon* pIcon, CSize szIcon,
										  CXTPControl* pControl, CSize szAvailable)
{
	if (NULL == pIcon)
		return szIcon;

	if (pIcon->IsVectorIcon())
		return (0 == szIcon.cx || 0 == szIcon.cy ? szAvailable : szIcon);

	BOOL bScaleDpi			= IsDpiBitmapScalingEnabled(pControl);
	BOOL bIsEditBuddyButton = XTP_SAFE_GET1((DYNAMIC_DOWNCAST(CXTPControlEdit, pControl)),
											IsBuddyButtonVisible(), FALSE);
	CSize szMargin			= bIsEditBuddyButton ? CSize(0, 0) : XTP_DPI(CSize(2, 2));

	return AdjustDpiIconSize(pIcon, szIcon, szAvailable, bScaleDpi, szMargin);
}

CSize CXTPPaintManager::AdjustDpiIconSize(CXTPImageManagerIcon* pIcon, CSize szIcon,
										  CXTPCommandBar* pCommandBar, CSize szAvailable,
										  CSize szMargin /*= CSize(0, 0)*/)
{
	return AdjustDpiIconSize(pIcon, szIcon, szAvailable, IsDpiBitmapScalingEnabled(pCommandBar),
							 szMargin);
}

CSize CXTPPaintManager::AdjustDpiIconSize(CXTPImageManagerIcon* pIcon, CSize szIcon,
										  CXTPCommandBars* pCommandBars, CSize szAvailable,
										  CSize szMargin /*= CSize(0, 0)*/)
{
	return AdjustDpiIconSize(pIcon, szIcon, szAvailable, IsDpiBitmapScalingEnabled(pCommandBars),
							 szMargin);
}

CSize CXTPPaintManager::AdjustDpiIconSize(CXTPImageManagerIcon* pIcon, CSize szIcon,
										  CSize szAvailable, BOOL bScaleDpi,
										  CSize szMargin /*= CSize(0, 0)*/)
{
	ASSERT_VALID(pIcon);

	if (NULL == pIcon)
		return szIcon;

	if (pIcon->IsVectorIcon())
		return (0 == szIcon.cx || 0 == szIcon.cy ? szAvailable : szIcon);

	if (bScaleDpi)
	{
		szIcon = XTP_DPI(szIcon);

		szAvailable.cx -= szMargin.cx * 2;
		szAvailable.cy -= szMargin.cy * 2;

		szIcon.cx = min(szAvailable.cx, szIcon.cx);
		szIcon.cy = min(szAvailable.cy, szIcon.cy);
		szIcon	= GetStretchIconSize(pIcon, szIcon);
	}
	else
	{
		// save original size of icon
		szIcon.cx = pIcon->GetWidth();
		szIcon.cy = pIcon->GetHeight();

		// reduce icon size if it is bigger than control
		if (szIcon.cx + szMargin.cx * 2 >= szAvailable.cx
			|| szIcon.cy + szMargin.cy * 2 >= szAvailable.cy)
		{
			szAvailable.cx -= szMargin.cx * 2;
			szAvailable.cy -= szMargin.cy * 2;

			szIcon = GetStretchIconSize(pIcon, szAvailable);
		}
	}

	return szIcon;
}

int AFX_CDECL CXTPPaintManager::CalcEntityHeight(
	int nFontHeight, int nFactor, int nMinHeight /*= XTP_COMMANDBAR_MIN_CONTROL_HEIGHT*/,
	int nCustomDPI /*= 0*/)
{
	ASSERT(0 < nFactor);

	int nLogHeight;
	if (0 == nCustomDPI)
	{
		CWindowDC dc(NULL);
		nLogHeight = dc.GetDeviceCaps(LOGPIXELSY);
	}
	else
	{
		nLogHeight = nCustomDPI;
	}

	if (nFontHeight < 0)
	{
		nFontHeight = -nFontHeight;
	}

	int nHeight = max(nMinHeight, nFontHeight * nFactor / nLogHeight);
	return nHeight;
}

int AFX_CDECL CXTPPaintManager::CalcTextHeight(int nFontHeight,
											   int nFactor /*= XTP_COMMANDBAR_TEXT_HEIGHT_FACTOR*/,
											   int nCustomDPI /*= 0*/)
{
	return CalcEntityHeight(nFontHeight, nFactor, XTP_COMMANDBAR_MIN_FONT_HEIGHT, nCustomDPI);
}

int AFX_CDECL CXTPPaintManager::CalcCtrlHeight(
	int nFontHeight, int nFactor /*= XTP_COMMANDBAR_CONTROL_HEIGHT_FACTOR*/, int nCustomDPI /*= 0*/)
{
	return CalcEntityHeight(nFontHeight, nFactor, XTP_COMMANDBAR_MIN_CONTROL_HEIGHT, nCustomDPI);
}

int AFX_CDECL CXTPPaintManager::CalcSplitButtonDropDownWidth(int nTextHeight)
{
	nTextHeight = abs(nTextHeight);
	return XTP_DPI_Y(max(XTP_COMMANDBAR_SPLITBUTTON_DROPDOWN_WIDTH, nTextHeight / 2));
}

int AFX_CDECL CXTPPaintManager::CalcSplitButtonDropDownHeight(int nFontHeight)
{
	const int TextMargin = XTP_DPI_Y(2);
	nFontHeight			 = abs(nFontHeight);
	return nFontHeight * 2 + TextMargin * 2;
}

CSize CXTPPaintManager::DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	CXTPFontDC font(pDC, GetControlFont(pButton));

	CRect rcButton			   = bDraw ? pButton->GetRect() : CXTPEmptyRect();
	XTPControlType controlType = pButton->GetType();
	XTPButtonStyle buttonStyle = pButton->GetStyle();

	BOOL bVert = GetControlTextOrientation(pButton) == xtpBarTextVertical;

	CSize szIcon   = GetIconSize(pButton);
	CSize szButton = pButton->GetButtonSize();

	if (buttonStyle == xtpButtonIconAndCaptionBelow)
	{
		szButton = CSize(max(XTP_DPI_X(42), GetControlHeight() * 2 - XTP_DPI_X(4)),
						 m_nEditHeight > 0 ? m_nEditHeight * 3
										   : GetSplitDropDownHeight() + szIcon.cy + XTP_DPI_Y(7));
	}
	else if (pButton->GetWidth() > 0 || pButton->GetHeight() > 0) // individual custom size
	{
		szButton = GetControlSize(pButton, szButton, bVert);
	}
	else if (pButton->GetParent()
			 && pButton->GetParent()->m_szButtons != CSize(0)) // common custom size
	{
		szButton = pButton->GetParent()->m_szButtons;
	}

	BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup)
					  || (buttonStyle == xtpButtonIcon) || (buttonStyle == xtpButtonIconAndCaption)
					  || (buttonStyle == xtpButtonIconAndCaptionBelow);

	CXTPImageManagerIcon* pImage = (bDrawImage && pButton->GetIconId() != 0)
									   ? pButton->GetImage(szIcon.cx)
									   : NULL;
	bDrawImage = bDrawImage && (pImage != NULL);

	szIcon = AdjustDpiIconSize(pImage, szIcon, pButton, szButton);

	BOOL bDrawText = pButton->IsCaptionVisible();

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		 bPopuped = pButton->GetPopuped();

	if (controlType == xtpControlSplitButtonPopup && bEnabled == TRUE_SPLITDROPDOWN)
		bEnabled = FALSE;

	if (controlType == xtpControlCheckBox || controlType == xtpControlRadioButton)
	{
		CSize szCheckBox = controlType == xtpControlRadioButton
							   ? DrawControlRadioButtonMark(pDC, rcButton, FALSE, bSelected,
															bPressed, bChecked, bEnabled)
							   : DrawControlCheckBoxMark(pDC, pButton, rcButton, FALSE);

		if (bDraw)
		{
			pDC->SetTextColor(GetControlTextColor(pButton));
			CRect rcCheck;
			if (!bVert)
			{
				rcCheck = CRect(CPoint(rcButton.left + XTP_DPI_X(3),
									   (rcButton.bottom + rcButton.top - szCheckBox.cy) / 2),
								szCheckBox);
				rcButton.left += szCheckBox.cx + XTP_DPI_X(5);
			}
			else
			{
				rcCheck = CRect(CPoint(rcButton.CenterPoint().x - szCheckBox.cx / 2,
									   rcButton.top + XTP_DPI_Y(3)),
								szCheckBox);
				rcButton.top += szCheckBox.cy + XTP_DPI_Y(5);
			}

			if (controlType == xtpControlRadioButton)
				DrawControlRadioButtonMark(pDC, rcCheck, TRUE, bSelected, bPressed, bChecked,
										   bEnabled);
			else
				DrawControlCheckBoxMark(pDC, pButton, rcCheck, TRUE);

			DrawControlText(pDC, pButton, rcButton, bDraw, FALSE, szButton, FALSE);

			if (IsKeyboardSelected(bSelected))
			{
				pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
				pDC->SetBkColor(GetXtremeColor(COLOR_BTNFACE));
				pDC->DrawFocusRect(rcButton);
			}

			return GetControlSize(pButton, szCheckBox, bVert);
		}
		else
		{
			CSize sz = DrawControlText(pDC, pButton, rcButton, bDraw, FALSE, szButton, FALSE);
			if (bVert)
				sz.cy += szCheckBox.cy + XTP_DPI_Y(5);
			else
				sz.cx += szCheckBox.cx + XTP_DPI_X(5);
			return GetControlSize(pButton, sz, bVert);
		}
	}

	if (bDraw)
	{
		if (controlType != xtpControlSplitButtonPopup)
		{
			if (!pButton->IsTransparent())
			{
				DrawControlEntry(pDC, pButton);
			}
		}
		else
		{
			BOOL bDrawSplitButtonFrame = TRUE;

#ifdef _XTP_INCLUDE_RIBBON
			if (pButton->GetRibbonGroup() && m_pRibbonPaintManager)
			{
				if (m_pRibbonPaintManager->DrawRibbonGroupControlEntry(pDC, pButton))
					bDrawSplitButtonFrame = FALSE;
			}
#endif
			if (bDrawSplitButtonFrame)
				DrawSplitButtonFrame(pDC, pButton, rcButton);
		}
		pDC->SetTextColor(GetControlTextColor(pButton));
	}

	if (buttonStyle == xtpButtonIconAndCaptionBelow)
	{
		BOOL bTriangled = pButton->GetCommandBar() != NULL;

		int nSplitDropDownHeight = GetSplitDropDownHeight();

		if (bVert)
			szButton = CSize(szButton.cy, szButton.cx);

		if (bDraw)
		{
			if (bDrawImage)
			{
				CPoint pt = bVert ? CPoint(rcButton.left + XTP_DPI_X(3) + nSplitDropDownHeight,
										   rcButton.CenterPoint().y - szIcon.cy / 2)
								  : CPoint(rcButton.CenterPoint().x - szIcon.cx / 2,
										   rcButton.top + XTP_DPI_Y(2));

				BOOL bRTL = XTPDrawHelpers()->IsContextRTL(pDC);
				if (bRTL && m_bEnableAnimation)
				{
					if (pImage->IsVectorIcon())
					{
						XTPDrawHelpers()->SetContextRTL(pDC, !bRTL);

						CXTPClientRect rcParent(pButton->GetParent());
						pt.x = rcParent.Width() - (pt.x + szIcon.cx);
					}
				}

				DrawImage(pDC, pt, szIcon, pImage, bSelected, bPressed, bEnabled, bChecked,
						  bPopuped);

				XTPDrawHelpers()->SetContextRTL(pDC, XTPToDWORD(bRTL));
			}

			CRect rcText = rcButton;
			if (bVert)
			{
				rcText.right = rcText.left + nSplitDropDownHeight;
			}
			else
			{
				rcText.bottom -= XTP_DPI_Y(1);
			}

			CSize szText = DrawControlText2(pDC, pButton, CXTPEmptyRect(), FALSE, bVert,
											bTriangled);
			if (!bVert)
			{
				rcText.top = rcText.bottom - szText.cy;
			}
			else
			{
				// TODO: Test case needed to make sure nothing is broken
			}

			DrawControlText2(pDC, pButton, rcText, TRUE, bVert, bTriangled);
		}
		else
		{
			CSize szText = DrawControlText2(pDC, pButton, CXTPEmptyRect(), FALSE, bVert,
											bTriangled);

			if (bVert)
			{
				szButton.cy = max(szButton.cy, szText.cy + XTP_DPI_Y(6));
			}
			else
			{
				szButton.cx = max(szButton.cx, szText.cx + XTP_DPI_X(5));
			}
		}
		return GetControlSize(pButton, szButton, bVert);
	}

	if (pButton->GetParent()->IsTextBelowIcons() && bDrawImage)
	{
		CXTPFontDC fontHorizontal(pDC, pButton->IsItemDefault() ? &m_xtpFontRegularBold
																: &m_xtpFontRegular);
		BOOL bTriangled = controlType == xtpControlPopup;

		if (bDraw)
		{
			if (controlType == xtpControlSplitButtonPopup)
				rcButton.right -= m_nSplitButtonDropDownWidth;

			CPoint pt = CPoint(rcButton.CenterPoint().x - szIcon.cx / 2,
							   rcButton.top + XTP_DPI_Y(4));

			DrawImage(pDC, pt, szIcon, pImage, bSelected, bPressed, bEnabled, bChecked, bPopuped);

			CRect rcText = rcButton;
			rcText.top += szIcon.cy + XTP_DPI_Y(4);

			DrawControlText(pDC, pButton, rcText, TRUE, FALSE, TRUE, bTriangled);
		}
		else
		{
			CSize szText = DrawControlText(pDC, pButton, CXTPEmptyRect(), FALSE, FALSE, TRUE,
										   bTriangled);

			szButton.cy += szText.cy;
			szButton.cx = max(szButton.cx, szText.cx + XTP_DPI_X(10));

			if (pButton->GetType() == xtpControlSplitButtonPopup)
				szButton.cx += XTP_DPI_X(11);

			szButton.cx = max(szButton.cx, szButton.cy);
		}
		return GetControlSize(pButton, szButton, bVert);
	}

	if (bDraw && pImage)
	{
		CPoint pt = !bVert ? CPoint(rcButton.left + XTP_DPI_X(3),
									rcButton.CenterPoint().y - szIcon.cy / 2)
						   : CPoint(rcButton.CenterPoint().x - szIcon.cx / 2,
									rcButton.top + XTP_DPI_Y(3));

		if (controlType == xtpControlSplitButtonPopup)
			pt.x = rcButton.left + XTP_DPI_X(3);
		if (controlType == xtpControlPopup && bDrawImage && !bDrawText)
			pt.x = rcButton.left + XTP_DPI_X(3);
		if ((controlType == xtpControlButtonPopup || controlType == xtpControlButton) && bDrawImage
			&& !bDrawText)
		{
			if (!bVert)
				pt.x = rcButton.CenterPoint().x - szIcon.cx / 2;
			else
				pt.y = rcButton.CenterPoint().y - szIcon.cy / 2;
		}

		pImage->m_bDrawComposited = pButton->HasDwmCompositedRect();

		BOOL bRTL = XTPDrawHelpers()->IsContextRTL(pDC);
		if (bRTL && m_bEnableAnimation)
		{
			if (pImage->IsVectorIcon())
			{
				XTPDrawHelpers()->SetContextRTL(pDC, !bRTL);

				CXTPClientRect rcParent(pButton->GetParent());
				pt.x = rcParent.Width() - (pt.x + szIcon.cx);
			}
		}

		DrawImage(pDC, pt, szIcon, pImage, bSelected, bPressed, bEnabled, bChecked, bPopuped);

		XTPDrawHelpers()->SetContextRTL(pDC, XTPToDWORD(bRTL));

		pImage->m_bDrawComposited = FALSE;
	}

	if (bDrawImage)
	{
		if (!bVert)
			szIcon.cx += XTP_DPI_X(3) * 2;
		else
			szIcon.cy += XTP_DPI_Y(3) * 2;
	}

	switch (controlType)
	{
		case xtpControlPopup:
		{
			if (!bDrawImage)
			{
				CSize sz = DrawControlText(pDC, pButton, rcButton, bDraw, bVert, TRUE,
										   !bDraw
											   || pButton->GetParent()->GetType()
													  != xtpBarTypeMenuBar);

				if (pButton->GetID() == XTP_ID_CUSTOMIZE_ADDORREMOVE)
				{
					sz = CSize(sz.cx + XTP_DPI_X(14), sz.cy + XTP_DPI_Y(10));
				}
				else
				{
					sz = CSize(sz.cx + XTP_DPI_X(6), sz.cy + XTP_DPI_Y(6));
				}

				return GetControlSize(pButton, sz, bVert);
			}
			else
			{
				if (!bDrawText)
				{
					CPoint pt = CPoint(rcButton.right - XTP_DPI_X(7), rcButton.CenterPoint().y);
					DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
					return GetControlSize(pButton, CSize(szButton.cy + XTP_DPI_X(7), szButton.cy),
										  bVert);
				}
				return GetControlSize(pButton,
									  DrawControlText(pDC, pButton, rcButton, bDraw,
													  pButton->GetParent()->GetType()
														  != xtpBarTypeMenuBar,
													  szIcon, bDrawImage),
									  bVert);
			}
		}
		case xtpControlSplitButtonPopup:
		{
			if (!bDrawImage)
				szButton = CSize(XTP_COMMANDBAR_MIN_CONTROL_HEIGHT,
								 XTP_COMMANDBAR_MIN_CONTROL_HEIGHT);

			if (!bDrawText)
				return GetControlSize(pButton,
									  CSize(szButton.cx + m_nSplitButtonDropDownWidth, szButton.cy),
									  bVert);

			rcButton.right -= m_nSplitButtonDropDownWidth;

			CSize sz = DrawControlText(pDC, pButton, rcButton, bDraw, FALSE, szIcon, bDrawImage);
			return GetControlSize(pButton, CSize(sz.cx + m_nSplitButtonDropDownWidth, sz.cy),
								  bVert);
		}
		case xtpControlButtonPopup:
		case xtpControlButton:
		case xtpControlLabel:
		case xtpControlCheckBox:
		case xtpControlGallery:
		{
			if (!bDrawText)
				return GetControlSize(pButton, szButton, bVert);

			if (!bDrawImage)
				szButton = CSize(XTP_COMMANDBAR_MIN_CONTROL_HEIGHT, szButton.cy);

			return GetControlSize(pButton,
								  DrawControlText(pDC, pButton, rcButton, bDraw, FALSE,
												  bDrawImage ? szIcon : szButton, bDrawImage),
								  bVert);
		}

		case xtpControlError:
		case xtpControlComboBox:
		case xtpControlEdit:
		case xtpControlCustom:
		case xtpControlRadioButton:
			// do nothing
			break;
	}

	return 0;
}

CRect CXTPPaintManager::GetCommandBarBorders(CXTPCommandBar* /*pBar*/)
{
	return XTP_DPI(CRect(2, 2, 2, 2));
}

void CXTPPaintManager::GradientFill(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo,
									BOOL bHorz, LPCRECT lpRectClip)
{
	XTPDrawHelpers()->GradientFill(pDC, lpRect, crFrom, crTo, bHorz, lpRectClip);
}

void CXTPPaintManager::AnimateExpanding(CXTPCommandBar* pCommandBar, CDC* pDestDC, CDC* pSrcDC,
										BOOL bExpandDown)
{
	ASSERT_VALID(pCommandBar);

	CXTPClientRect rc(pCommandBar);
	int nWidth  = rc.Width();
	int nHeight = rc.Height();

	int nSteps		   = m_nAnimationSteps;
	int nAnimationTime = m_nAnimationTime;

	CXTPControls* pControls = pCommandBar->GetControls();

	for (int i = 0; i < nSteps; ++i)
	{
		double dPercent = double(i + 1) / nSteps;

		int nTopDest	   = 0;
		int nTopSrc		   = 0;
		BOOL bPrevExpanded = FALSE;

		if (bExpandDown)
		{
			for (int j = 0; j < pControls->GetCount(); ++j)
			{
				CXTPControl* pControl = pControls->GetAt(j);
				if (!pControl->IsVisible())
					continue;

				BOOL bExpanded = pControl->GetExpanded();

				if (bExpanded != bPrevExpanded)
				{
					int nTopSrcCurrent = pControl->GetRect().top;
					int nHeightDstr	= int((bPrevExpanded ? dPercent : 1)
											 * (nTopSrcCurrent - nTopSrc));

					pDestDC->StretchBlt(0, nTopDest, nWidth, nHeightDstr, pSrcDC, 0, nTopSrc,
										nWidth, nTopSrcCurrent - nTopSrc, SRCCOPY);

					nTopDest = nTopDest + nHeightDstr;
					nTopSrc  = nTopSrcCurrent;
				}

				bPrevExpanded = bExpanded;
			}

			int nTopSrcCurrent = nHeight;
			int nHeightDstr	= int((bPrevExpanded ? dPercent : 1) * (nTopSrcCurrent - nTopSrc));

			pDestDC->StretchBlt(0, nTopDest, nWidth, nHeightDstr, pSrcDC, 0, nTopSrc, nWidth,
								nTopSrcCurrent - nTopSrc, SRCCOPY);
		}
		else
		{
			for (int j = pControls->GetCount() - 1; j >= 0; j--)
			{
				CXTPControl* pControl = pControls->GetAt(j);
				if (!pControl->IsVisible())
					continue;

				BOOL bExpanded = pControl->GetExpanded();

				if (bExpanded != bPrevExpanded)
				{
					int nTopSrcCurrent = nHeight - pControl->GetRect().bottom;
					int nHeightDstr	= int((bPrevExpanded ? dPercent : 1)
											 * (nTopSrcCurrent - nTopSrc));

					pDestDC->StretchBlt(0, nHeight - nTopDest - nHeightDstr, nWidth, nHeightDstr,
										pSrcDC, 0, nHeight - nTopSrcCurrent, nWidth,
										nTopSrcCurrent - nTopSrc, SRCCOPY);

					nTopDest = nTopDest + nHeightDstr;
					nTopSrc  = nTopSrcCurrent;
				}

				bPrevExpanded = bExpanded;
			}

			int nTopSrcCurrent = nHeight;
			int nHeightDstr	= int((bPrevExpanded ? dPercent : 1) * (nTopSrcCurrent - nTopSrc));

			pDestDC->StretchBlt(0, nHeight - nTopDest - nHeightDstr, nWidth, nHeightDstr, pSrcDC, 0,
								nHeight - nTopSrcCurrent, nWidth, nTopSrcCurrent - nTopSrc,
								SRCCOPY);
		}

		Sleep(XTPToDWORDChecked(nAnimationTime / nSteps));
	}
}

void CXTPPaintManager::AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc,
								   BYTE byAlpha)
{
	ASSERT(NULL != pDest);
	ASSERT(NULL != pSrcBack);

	const BYTE byDiff = (BYTE)(255 - byAlpha);

	for (int i = 0; i < cx * cy; i++)
	{
		pDest[0] = (BYTE)((pSrcBack[0] * byDiff + pSrc[0] * byAlpha) >> 8);
		pDest[1] = (BYTE)((pSrcBack[1] * byDiff + pSrc[1] * byAlpha) >> 8);
		pDest[2] = (BYTE)((pSrcBack[2] * byDiff + pSrc[2] * byAlpha) >> 8);

		pDest += 4;
		pSrcBack += 4;
		pSrc += 4;
	}
}

void CXTPPaintManager::Animate(CDC* pDestDC, CDC* pSrcDC, CRect rc, XTPAnimationType animationType)
{
	int nSteps		   = m_nAnimationSteps;
	int nAnimationTime = m_nAnimationTime;

	switch (animationType)
	{
		case xtpAnimateSlide:
		{
			for (int i = 0; i < nSteps; ++i)
			{
				int nHeight = rc.Height() * (i + 1) / nSteps;

				pDestDC->BitBlt(0, 0, rc.Width(), nHeight, pSrcDC, 0, rc.Height() - nHeight,
								SRCCOPY);

				Sleep(XTPToDWORDChecked(nAnimationTime / nSteps));
			}
		}
		break;

		case xtpAnimateUnfold:
		{
			for (int i = 0; i < nSteps; ++i)
			{
				CRect rcDest(rc.right - rc.Width() * (i + 1) / nSteps,
							 rc.bottom - rc.Height() * (i + 1) / nSteps, rc.right, rc.bottom);

				pDestDC->BitBlt(0, 0, rcDest.Width(), rcDest.Height(), pSrcDC, rcDest.left,
								rcDest.top, SRCCOPY);

				Sleep(XTPToDWORDChecked(nAnimationTime / nSteps));
			}
		}
		break;

		case xtpAnimateFade:
		{
			const int cx = rc.Width();
			const int cy = rc.Height();

			BITMAPINFOHEADER BMI;
			// Fill in the header info.
			ZeroMemory(&BMI, sizeof(BMI));
			BMI.biSize		  = sizeof(BITMAPINFOHEADER);
			BMI.biWidth		  = cx;
			BMI.biHeight	  = cy;
			BMI.biPlanes	  = 1;
			BMI.biBitCount	= 32;
			BMI.biCompression = BI_RGB; // No compression

			BYTE* pSrcBits = NULL;
			HBITMAP hbmSrc = CreateDIBSection(NULL, (BITMAPINFO*)&BMI, DIB_RGB_COLORS,
											  (void**)&pSrcBits, 0, 0l);

			BYTE* pSrcBackBits = NULL;
			HBITMAP hbmSrcBack = CreateDIBSection(NULL, (BITMAPINFO*)&BMI, DIB_RGB_COLORS,
												  (void**)&pSrcBackBits, 0, 0l);

			BYTE* pDestBits = NULL;
			HBITMAP hbmDest = CreateDIBSection(NULL, (BITMAPINFO*)&BMI, DIB_RGB_COLORS,
											   (void**)&pDestBits, 0, 0l);

			// Copy our source and destination bitmaps onto our DIBSections,
			// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection

			CDC dc;
			dc.CreateCompatibleDC(NULL);
			HBITMAP hbmpOld = (HBITMAP)::SelectObject(dc, hbmSrc);
			::BitBlt(dc, 0, 0, cx, cy, pSrcDC->GetSafeHdc(), 0, 0, SRCCOPY);

			::SelectObject(dc, hbmSrcBack);
			::BitBlt(dc, 0, 0, cx, cy, pDestDC->GetSafeHdc(), 0, 0, SRCCOPY);

			DWORD dwTimePer = XTPToDWORDChecked(nAnimationTime / nSteps);

			::SelectObject(dc, hbmDest);
			for (int i = 1; i < nSteps; ++i)
			{
				DWORD dwTime = GetTickCount();
				AlphaBlendU(pDestBits, pSrcBackBits, cx, cy, pSrcBits, (BYTE)(255 * i / nSteps));
				pDestDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
				dwTime = GetTickCount() - dwTime;
				if (dwTime < dwTimePer)
				{
					Sleep(dwTimePer - dwTime);
				}
			}
			::SelectObject(dc, hbmpOld);
			DeleteObject(hbmSrc);
			DeleteObject(hbmSrcBack);
			DeleteObject(hbmDest);
		}

		break;

		case xtpAnimateWindowsDefault:
		case xtpAnimateRandom:
		case xtpAnimateNone:
		default: break;
	}
}

void CXTPPaintManager::UpdateRTLCaption(CString& /*strCaption*/)
{
}

CSize CXTPPaintManager::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	if (pBar->GetPosition() == xtpBarFloating)
	{
		CRect rc;
		pBar->GetClientRect(&rc);

		ASSERT(pDC);
		if (!pDC)
			return CSize(0, 0);
		CXTPFontDC font(pDC, GetSmCaptionFont());
		CSize sz = pDC->GetTextExtent(_T(" "), 1);

		rc.SetRect(XTP_DPI_X(3), XTP_DPI_Y(3), rc.right - XTP_DPI_X(3),
				   XTP_DPI_Y(3) + max(XTP_DPI_Y(15), sz.cy));
		if (pDC && bDraw)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->FillSolidRect(rc, m_clrFloatingGripper);
			pDC->SetTextColor(m_clrFloatingGripperText);
			pDC->DrawText(pBar->GetTitle(),
						  CRect(XTP_DPI_X(5), XTP_DPI_Y(3), rc.right - 2 * rc.Size().cy, rc.bottom),
						  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
		}
		return rc.Size();
	}
	return 0;
}

void CXTPPaintManager::SetCommandBarRegion(CXTPCommandBar* /*pCommandBar*/)
{
}

BOOL CXTPPaintManager::IsFlatToolBar(CXTPCommandBar* pCommandBar)
{
	if (pCommandBar->GetFlags() & xtpFlagNoBorders)
	{
		return TRUE;
	}

	return (pCommandBar->GetType() == xtpBarTypeMenuBar) ? m_bFlatMenuBar : m_bFlatToolBar;
}

void CXTPPaintManager::DrawPopupBarGripper(CDC* /*pDC*/, int /*xPos*/, int /*yPos*/, int /*cx*/,
										   int /*cy*/, BOOL /*bExpanded = FALSE*/)
{
}
void CXTPPaintManager::FillWorkspace(CDC* pDC, CRect rc, CRect /*rcExclude*/)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_APPWORKSPACE));
}

void AFX_CDECL CXTPPaintManager::FillCompositeAlpha(CDC* pDC, CRect rc)
{
	CSize sz(rc.Width(), rc.Height());
	CBitmap bmp;
	LPDWORD lpBits;
	bmp.Attach(
		CXTPImageManager::Create32BPPDIBSection(pDC->GetSafeHdc(), sz.cx, sz.cy, (LPBYTE*)&lpBits));

	CXTPCompatibleDC dc(pDC, &bmp);

	CRect rcDestOrig(0, 0, sz.cx, sz.cy);

	dc.BitBlt(0, 0, sz.cx, sz.cy, pDC, rc.left, rc.top, SRCCOPY);

	for (int i = 0; i < sz.cx * sz.cy; i++)
	{
		lpBits[0] |= 0xFF000000;
		lpBits++;
	}

	pDC->BitBlt(rc.left, rc.top, sz.cx, sz.cy, &dc, 0, 0, SRCCOPY);
}

void CXTPPaintManager::DrawKeyboardTip(CDC* pDC, CXTPCommandBarKeyboardTip* pWnd, BOOL bSetRegion)
{
	if (!bSetRegion)
	{
		CXTPClientRect rc(pWnd);
		CXTPFontDC font(pDC, &m_fontToolTip);

		pDC->FillSolidRect(rc, GetXtremeColor(UINT(COLOR_INFOBK)));
		pDC->Draw3dRect(rc, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(UINT(pWnd->m_bEnabled ? COLOR_INFOTEXT : COLOR_GRAYTEXT)));
		pDC->DrawText(pWnd->m_strTip, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

CSize CXTPPaintManager::GetAutoIconSize(BOOL bLarge) const
{
	if (m_bAutoResizeIcons)
	{
		int nHeight = GetControlHeight() - XTP_DPI_Y(4);
		if (bLarge)
			nHeight = nHeight * 2;

		nHeight = max(XTP_DPI_Y(2), nHeight / 8);

		return CSize(nHeight * 8, nHeight * 8);
	}

	return bLarge ? XTPSystemMetrics()->GetIconSize() : XTPSystemMetrics()->GetSmallIconSize();
}

void CXTPPaintManager::DrawControlMDIButton(CDC* pDC, CXTPControlButton* pButton)
{
	CXTPFont font;
	font.CreatePointFont(80, _T("Marlett"));

	CFont* pFont = pDC->SelectObject(&font);

	COLORREF oldColor = GetTextColor(pDC->GetSafeHdc());
	pDC->SetTextColor(GetControlTextColor(pButton));

	CString strText(pButton->GetParameter());
	DrawTextApi(pDC, strText, pButton->GetRect(),
				DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX,
				pButton->HasDwmCompositedRect());

	SetTextColor(pDC->GetSafeHdc(), oldColor);

	pDC->SelectObject(pFont);
}

void CXTPPaintManager::SetImageHandle(HMODULE hResource, LPCTSTR lpszIniFileName)
{
	UNUSED_ALWAYS(hResource);
	UNUSED_ALWAYS(lpszIniFileName);
}

BOOL XTPReBarPaintManager_FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar,
											  CXTPWinThemeWrapper& theme)
{
	ASSERT_VALID(pBar);

	if (pBar->GetPosition() == xtpBarFloating || pBar->GetPosition() == xtpBarPopup)
		return FALSE;

	CRect rcClient;
	if (theme.IsAppThemed())
	{
		pBar->GetParent()->GetWindowRect(&rcClient);
		pBar->ScreenToClient(&rcClient);

		theme.DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, 0);
	}
	else
	{
		pBar->GetClientRect(&rcClient);
		pDC->FillSolidRect(rcClient, GetSysColor(COLOR_3DFACE));
	}

	return TRUE;
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPPaintManager, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPPaintManager, "EnableAnimation", 1, m_bEnableAnimation, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "FlatToolBar", 2, m_bFlatToolBar, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "FlatMenuBar", 3, m_bFlatMenuBar, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPPaintManager, "ClearTypeTextQuality", 4, m_bClearTypeTextQuality,
							OleRefreshMetrics, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "ShowShadow", 5, m_bShowShadow, VT_BOOL)
	DISP_FUNCTION_ID(CXTPPaintManager, "RefreshMetrics", 6, OleRefreshMetrics, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPaintManager, "FillWorkspace", 7, OleFillWorkspace, VT_EMPTY,
					 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPPaintManager, "DrawRectangle", 8, OleDrawRectangle, VT_EMPTY,
					 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL
						 VTS_BOOL VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPPaintManager, "GetRectangleTextColor", 9, OleGetRectangleTextColor,
					 VT_COLOR, VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_I4 VTS_I4)

	DISP_PROPERTY_ID(CXTPPaintManager, "ThemedStatusBar", 10, m_bThemedStatusBar, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "ThickCheckMark", 11, m_bThickCheckMark, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "SelectImageInPopupBar", 12, m_bSelectImageInPopupBar,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "ThemedCheckBox", 13, m_bThemedCheckBox, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPaintManager, "AutoResizeIcons", 14, m_bAutoResizeIcons, VT_BOOL)

	DISP_FUNCTION_ID(CXTPPaintManager, "LoadFrameIconFromResource", 16, OleLoadFrameIcon, VT_EMPTY,
					 MODULE_HANDLE_TYPE VTS_BSTR VTS_I4 VTS_I4)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPaintManager, XTPDIID_IPaintManager)

BEGIN_INTERFACE_MAP(CXTPPaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPPaintManager, XTPDIID_IPaintManager, Dispatch)
END_INTERFACE_MAP()

void CXTPPaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPPaintManager::OleLoadFrameIcon(MODULE_HANDLE Module, LPCTSTR pszFileName, long Width,
										long Height)
{
	CXTPFramePaintManager* pFramePaintManager = GetFramePaintManager();
	if (!pFramePaintManager)
		return;

	HICON hIcon = NULL;

	hIcon = (HICON)::LoadImage((HMODULE)Module, pszFileName, IMAGE_ICON, Width, Height,
							   LR_LOADFROMFILE);

	if (hIcon)
	{
		CSize szIcon;
		szIcon.cx = Width;
		szIcon.cy = Height;

		pFramePaintManager->SetIcon(hIcon, szIcon);
	}
}

void CXTPPaintManager::OleFillWorkspace(OLE_HANDLE hDC, int x, int y, int cx, int cy)
{
	FillWorkspace(CDC::FromHandle(reinterpret_cast<HDC>((LONG_PTR)hDC)),
				  CRect(x, y, x + cx, y + cy), CRect(0, 0, 0, 0));
}

void CXTPPaintManager::OleDrawRectangle(OLE_HANDLE hDC, int x, int y, int cx, int cy,
										BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked,
										BOOL bPopuped, XTPBarType barType,
										XTPBarPosition barPosition)
{
	DrawRectangle(CDC::FromHandle(reinterpret_cast<HDC>((LONG_PTR)hDC)),
				  CRect(x, y, x + cx, y + cy), bSelected, bPressed, bEnabled, bChecked, bPopuped,
				  barType, barPosition);
}

COLORREF CXTPPaintManager::OleGetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled,
													BOOL bChecked, BOOL bPopuped,
													XTPBarType barType, XTPBarPosition barPosition)
{
	return GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, bPopuped, barType,
								 barPosition);
}

#endif
