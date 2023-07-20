// XTPRibbonThemeOffice2007System.cpp : implementation file
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
#include "Common/XTPResource.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/Frame/XTPFrameHook.h"

#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonMarkupTheme.h"

#include "Ribbon/Themes/XTPRibbonThemeOffice2007System.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPRibbonThemeOffice2007System::CXTPRibbonThemeOffice2007System(CXTPPaintManager* pPaintManager)
	: CXTPRibbonMarkupTheme(pPaintManager)
{
	LoadParts();

	if (XTPSystemVersion()->IsWinVistaOrGreater())
	{
		m_pPaintManager->m_bUseOfficeFont = TRUE;
		m_pPaintManager->m_strOfficeFont  = _T("Segoe UI");
		m_pPaintManager->SetFontHeight(XTP_DPI_Y(12));
	}
}

CXTPRibbonThemeOffice2007System::~CXTPRibbonThemeOffice2007System()
{
}

void CXTPRibbonThemeOffice2007System::RefreshMetrics()
{
	CXTPRibbonMarkupTheme::RefreshMetrics();

	m_pPaintManager->GetIconsInfo()->bUseFadedIcons	= FALSE;
	m_pPaintManager->GetIconsInfo()->bIconsWithShadow  = FALSE;
	m_pPaintManager->GetIconsInfo()->bUseDisabledIcons = TRUE;
}

void CXTPRibbonThemeOffice2007System::DrawRibbonQuickAccessControls(CDC* pDC,
																	CXTPRibbonBar* pRibbonBar)
{
	if (!pRibbonBar->IsQuickAccessBelowRibbon() && pRibbonBar->IsQuickAccessVisible()
		&& ((CXTPControls*)pRibbonBar->GetQuickAccessControls())->GetVisibleCount() > 0)
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
		int nFrameBorder	= -GetSystemMetrics(SM_CYBORDER);
		if (pRibbonBar->IsDwmEnabled())
		{
			nFrameBorder += pRibbonBar->GetFrameHook()->CalcFrameBorder().top;
		}

		CRect rcQuickAccessArea(rcQuickAccess.left - XTP_DPI_X(15),
								rcQuickAccess.top + nFrameBorder / 2, rcQuickAccess.right,
								rcQuickAccess.bottom - XTP_DPI_Y(1));
		CString strMarkupText;
		CSize sz = rcQuickAccessArea.Size();

		strMarkupText.Format(_T("<Path Data='M0 0 S %i %i %i %i L %i %i S %i %i %i %i l 0 0 S  %i ")
							 _T("%i %i %i Z' Stroke='#22FFFFFF' StrokeThickness='2' ")
							 _T("Fill='#44FFFFFF' ></Path>"),
							 sz.cy / 2, 0, (sz.cy / 4) * 3, sz.cy, sz.cx - sz.cy / 2, sz.cy, sz.cx,
							 sz.cy, sz.cx, sz.cy / 2, sz.cx, 0, sz.cx - sz.cy / 2, 0);

		RenderMarkup(pDC, rcQuickAccessArea, strMarkupText);
	}
}

void CXTPRibbonThemeOffice2007System::DrawRibbonQuickAccessButton(CDC* pDC,
																  CXTPControlPopup* pControl)
{
	ASSERT(pControl);

	m_pPaintManager->DrawRectangle(pDC, pControl->GetRect(), pControl->GetSelected(),
								   pControl->GetPressed(), pControl->GetEnabled(), FALSE,
								   pControl->GetPopuped(), xtpBarTypeNormal, xtpBarTop);

	CRect rc = pControl->GetRect();
	rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	rc.OffsetRect(0, XTP_DPI_Y(1));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEjectDown, rc,
									 GetSysColor(COLOR_3DHIGHLIGHT));

	rc.OffsetRect(0, XTP_DPI_Y(-1));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEjectDown, rc,
									 GetSysColor(COLOR_BTNTEXT));
}

BOOL CXTPRibbonThemeOffice2007System::LoadResource()
{
	LoadParts();
	return TRUE;
}

void CXTPRibbonThemeOffice2007System::LoadParts(BOOL bReload /*= FALSE*/)
{
	if (bReload)
		m_bLoaded = FALSE;

	if (m_bLoaded)
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
			STATIC_DOWNCAST(CXTPRibbonMarkupTheme, this)->LoadParts(hZip);

		CloseZip(hZip);
	}
}

int CXTPRibbonThemeOffice2007System::GetRibbonCaptionHeight(const CXTPRibbonBar* pRibbonBar) const
{
	int nRibbonCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION);

	if (!pRibbonBar->IsQuickAccessVisible() || pRibbonBar->IsQuickAccessBelowRibbon())
	{
		nRibbonCaptionHeight -= XTP_DPI_Y(4);
	}

	return nRibbonCaptionHeight;
}
