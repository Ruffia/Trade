// XTPFrameThemeVisualStudio2017.cpp
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

#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"
#include "CommandBars/Frame/XTPFrameThemeOffice2013.h"
#include "CommandBars/Frame/XTPFrameThemeVisualStudio2017.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFrameThemeVisualStudio2017::CXTPFrameThemeVisualStudio2017(CXTPPaintManager* pPaintManager)
	: CXTPFrameThemeOffice2013(pPaintManager)
	, m_bAutomaticStateColor(TRUE)
{
	XTPGetApplication()->Subscribe(this);
}

CXTPFrameThemeVisualStudio2017::~CXTPFrameThemeVisualStudio2017()
{
	XTPGetApplication()->Unsubscribe(this);
}

void CXTPFrameThemeVisualStudio2017::RefreshMetrics()
{
	CXTPFrameThemeOffice2013::RefreshMetrics();

	m_clrShadowColorActive   = XTPIniColor(_T("CommandBars.Frame"), _T("ActiveShadowColor"),
										   RGB(0, 122, 204));
	m_clrShadowColorInactive = XTPIniColor(_T("CommandBars.Frame"), _T("InactiveShadowColor"),
										   RGB(48, 48, 48));

	UpdateStateColor();
}

CRect CXTPFrameThemeVisualStudio2017::CalcFrameBorder(
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

CXTPFrameShadow* CXTPFrameThemeVisualStudio2017::CreateShadow(CWnd* pFrame)
{
	ASSERT(!XTPFrameShadowManager()->IsShadowEnabled(pFrame));

	CXTPFrameShadow* pShadow = XTPFrameShadowManager()->EnableShadow(pFrame, m_clrShadowColorActive,
																	 XTPToUInt(XTP_DPI_Y(5)),
																	 XTPToUInt(XTP_DPI_Y(3)), 0,
																	 0x80);

	return pShadow;
}

void CXTPFrameThemeVisualStudio2017::OnShadowedFrameActiveStateChanged(BOOL bActive, CWnd* pFrame,
																	   CXTPFrameShadow* pShadow)
{
	ASSERT(pShadow);
	UNUSED_ALWAYS(pFrame);

	pShadow->SetColor(bActive ? m_clrShadowColorActive : m_clrShadowColorInactive);
}

void CXTPFrameThemeVisualStudio2017::SetStateColor(COLORREF clrState)
{
	if (clrState == COLORREF_NULL)
	{
		m_clrFrameBorderActive.SetDefaultValue();
		m_bAutomaticStateColor = TRUE;
	}
	else
	{
		m_clrFrameBorderActive = clrState;
		m_bAutomaticStateColor = FALSE;
	}
}

COLORREF CXTPFrameThemeVisualStudio2017::GetStateColor() const
{
	return m_clrFrameBorderActive.IsDefaultValue() ? COLORREF_NULL
												   : m_clrFrameBorderActive.operator COLORREF();
}

void CXTPFrameThemeVisualStudio2017::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
															BOOL bSelected, BOOL bPressed,
															BOOL bActive)
{
	DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive, NULL);
}

void CXTPFrameThemeVisualStudio2017::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
															BOOL bSelected, BOOL bPressed,
															BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	UNREFERENCED_PARAMETER(pRibbonBar);

	CXTPFrameThemeOffice2013::DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive,
													 NULL);

	CXTPColorSetCaptionButton* pColorSet = NULL;

	if (bPressed)
	{
		pColorSet = &m_csFrameCaptionButtonPressed;
	}
	else if (bSelected)
	{
		pColorSet = &m_csFrameCaptionButtonHot;
	}

	if (pColorSet)
	{
		CXTPPenDC pen(*pDC, pColorSet->m_clrOutline);

		// draw left border.
		pDC->MoveTo(rc.left, rc.top);
		pDC->LineTo(rc.left, rc.bottom - 1);

		// draw bottom border.
		pDC->MoveTo(rc.left, rc.bottom - 1);
		pDC->LineTo(rc.right, rc.bottom - 1);

		if (nId == SC_MINIMIZE || nId == SC_MAXIMIZE || nId == SC_RESTORE)
		{
			// draw right border.
			pDC->MoveTo(rc.right - 1, rc.top);
			pDC->LineTo(rc.right - 1, rc.bottom - 1);
		}
	}
}

void CXTPFrameThemeVisualStudio2017::OnAmbientPropertyChanged(
	CXTPApplication* pApplication, XTPApplicationAmbientProperty nProperty)
{
	UNREFERENCED_PARAMETER(pApplication);

	if (xtpApplicationStateColor != nProperty)
		return;

	UpdateStateColor();
}

void CXTPFrameThemeVisualStudio2017::UpdateStateColor()
{
	if (!m_bAutomaticStateColor)
		return;

	COleVariant color = XTPGetApplication()->GetAmbientProperty(xtpApplicationStateColor);
	color.ChangeType(VT_I4);
	SetStateColor(static_cast<COLORREF>(color.lVal));

	m_bAutomaticStateColor = TRUE;
}
