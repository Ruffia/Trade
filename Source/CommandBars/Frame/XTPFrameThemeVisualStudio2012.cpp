// XTPFrameThemeVisualStudio2012.cpp
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPTheme.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/FrameShadow/XTPFrameShadow.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

// Frame
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF VS2012ActiveShadowColor   = RGB(0, 122, 204);
const COLORREF VS2012InactiveShadowColor = RGB(0x30, 0x30, 0x30);

/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012

CXTPFrameThemeVisualStudio2012::CXTPFrameThemeVisualStudio2012(CXTPPaintManager* pPaintManager)
	: CXTPFrameThemeModernUI(pPaintManager)
{
	m_rcIconMargin.SetRect(XTP_DPI_X(12), 0, XTP_DPI_X(12), 0);
}

CXTPFrameThemeVisualStudio2012::~CXTPFrameThemeVisualStudio2012()
{
}

void CXTPFrameThemeVisualStudio2012::RefreshMetrics(CXTPResourceImages* /*pImages*/)
{
	m_clrFrameCaptionActive   = XTPIniColor(_T("Frame.Active"), _T("CaptionColor"),
											RGB(239, 239, 242));
	m_clrFrameCaptionInactive = XTPIniColor(_T("Frame.Inactive"), _T("CaptionColor"),
											RGB(239, 239, 242));
}

void CXTPFrameThemeVisualStudio2012::RefreshMetrics()
{
	CXTPFrameThemeModernUI::RefreshMetrics();
}

LPCTSTR CXTPFrameThemeVisualStudio2012::GetStyleResourceName() const
{
	return _T("XTP_IDR_VS2012_STYLE");
}

CSize CXTPFrameThemeVisualStudio2012::GetCaptionButtonSize(UINT nId) const
{
	UNUSED_ALWAYS(nId);
	return XTP_DPI(CSize(34, 26));
}

CSize CXTPFrameThemeVisualStudio2012::GetCaptionButtonSize(UINT nId, HWND hwnd) const
{
	UNUSED_ALWAYS(nId);
	UNUSED_ALWAYS(hwnd);
	return XTP_DPI(CSize(34, 26));
}

CRect CXTPFrameThemeVisualStudio2012::CalcFrameBorder(
	const CXTPCommandBarsFrameHook* pFrameHook) const
{
	ASSERT(NULL != pFrameHook);

	CRect rc = CXTPFrameThemeModernUI::CalcFrameBorder(pFrameHook);
	if (pFrameHook->IsMDIMaximized())
	{
		rc.top -= XTP_DPI_Y(4);
	}

	return rc;
}

CXTPFrameShadow* CXTPFrameThemeVisualStudio2012::CreateShadow(CWnd* pFrame)
{
	ASSERT(!XTPFrameShadowManager()->IsShadowEnabled(pFrame));

	CXTPFrameShadow* pShadow = XTPFrameShadowManager()->EnableShadow(
		pFrame, VS2012ActiveShadowColor, XTPToUInt(XTP_DPI_Y(5)), XTPToUInt(XTP_DPI_Y(3)), 0, 0x80);
	return pShadow;
}

void CXTPFrameThemeVisualStudio2012::OnShadowedFrameActiveStateChanged(BOOL bActive, CWnd* pFrame,
																	   CXTPFrameShadow* pShadow)
{
	ASSERT(pShadow);
	UNUSED_ALWAYS(pFrame);

	pShadow->SetColor(bActive ? VS2012ActiveShadowColor : VS2012InactiveShadowColor);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012Light

CXTPFrameThemeVisualStudio2012Light::CXTPFrameThemeVisualStudio2012Light(
	CXTPPaintManager* pPaintManager)
	: CXTPFrameThemeVisualStudio2012(pPaintManager)
{
	m_clrFrameCaptionActive   = RGB(239, 239, 242);
	m_clrFrameCaptionInactive = RGB(239, 239, 242);
}

CXTPFrameThemeVisualStudio2012Light::~CXTPFrameThemeVisualStudio2012Light()
{
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012Dark

CXTPFrameThemeVisualStudio2012Dark::CXTPFrameThemeVisualStudio2012Dark(
	CXTPPaintManager* pPaintManager)
	: CXTPFrameThemeVisualStudio2012(pPaintManager)
{
	m_clrFrameCaptionActive   = RGB(45, 45, 48);
	m_clrFrameCaptionInactive = RGB(45, 45, 48);
}

CXTPFrameThemeVisualStudio2012Dark::~CXTPFrameThemeVisualStudio2012Dark()
{
}
