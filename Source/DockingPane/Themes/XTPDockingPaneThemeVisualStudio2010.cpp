// XTPDockingPaneThemeVisualStudio2010.cpp : implementation of the
// CXTPDockingPaneVisualStudio2010Theme class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/Includes.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2010.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2008.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2010.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2010Theme

CXTPDockingPaneVisualStudio2010Theme::CXTPDockingPaneVisualStudio2010Theme()
{
	m_nCaptionFontGap				  = XTP_DPI_X(6);
	m_nCaptionButtonSize			  = XTP_DPI_X(15);
	m_nCaptionButtonStyle			  = xtpPaneCaptionButtonOffice2003;
	m_bAutoHidePanelHighlightSelected = FALSE;

	CXTPTabThemeVisualStudio2010* pTabTheme = new CXTPTabThemeVisualStudio2010();
	pTabTheme->m_rcHeaderMargin.left		= 0;
	pTabTheme->m_rcHeaderMargin.right		= 0;

	m_pTabPaintManager->SetAppearanceSet(pTabTheme);
	m_pTabPaintManager->SetColorSet(new CXTPTabColorSetVisualStudio2010(TRUE));
	m_pTabPaintManager->m_clientFrame		 = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_rcClientMargin.top = XTP_DPI_X(1);
	m_pTabPaintManager->m_bHotTracking		 = TRUE;

	CXTPTabThemeVisualStudio2010* pPanelTheme = new CXTPTabThemeVisualStudio2010();
	pPanelTheme->m_bDrawNormalTab			  = TRUE;
	pPanelTheme->m_rcHeaderMargin.left		  = 0;

	m_pPanelPaintManager->SetAppearanceSet(pPanelTheme);
	m_pPanelPaintManager->SetColor(xtpTabColorVisualStudio2010);
	m_pPanelPaintManager->m_bHotTracking = TRUE;
}

void CXTPDockingPaneVisualStudio2010Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Theme::RefreshMetrics();

	m_bLunaTheme = XTPSystemVersion()->IsWinVistaOrGreater();

	COLORREF clrBackground = m_pTabPaintManager->GetColorSet()->m_clrAutoHideFace;

	m_clrNormalCaption.SetStandardValue(RGB(77, 96, 130), RGB(61, 82, 119));
	m_clrActiveCaption.SetStandardValue(RGB(255, 232, 166));
	m_clrCaptionBorder = clrBackground;
	m_clrSplitter.SetStandardValue(clrBackground);
	m_clrSplitterGradient.SetStandardValue(clrBackground);

	m_clrNormalCaptionText.SetStandardValue(RGB(255, 255, 255));
	m_clrActiveCaptionText.SetStandardValue(0);

	if (!m_bLunaTheme)
	{
		m_clrNormalCaption.SetStandardValue(RGB(74, 97, 132));
		m_clrActiveCaption.SetStandardValue(RGB(255, 243, 206));
	}

	m_arrColor[XPCOLOR_3DFACE]					= clrBackground;
	m_arrColor[XPCOLOR_HIGHLIGHT]				= RGB(255, 255, 247);
	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER]		= RGB(231, 195, 99);
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]		= RGB(255, 235, 165);
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] = RGB(231, 195, 99);
	m_arrColor[XPCOLOR_PUSHED_TEXT]				= RGB(0, 0, 0);
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]			= RGB(0, 0, 0);
}

void CXTPDockingPaneVisualStudio2010Theme::DrawFloatingFrame(CDC* pDC,
															 CXTPDockingPaneMiniWnd* pPane,
															 CRect rc)
{
	pDC->FillSolidRect(rc, m_clrCaptionBorder);
	rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	HBRUSH hCustomBrush = GetCustomTitleBrush(pPane->IsActive(), pPane->IsCaptionVertical());
	if (NULL == hCustomBrush)
	{
		dcCache.FillSolidRect(rcCaption, m_clrCaptionBorder);
	}
	else
	{
		dcCache.FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(2), XTP_DPI_X(1), 0);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

COLORREF CXTPDockingPaneVisualStudio2010Theme::FillCaptionPart(CDC* pDC,
															   CXTPDockingPaneBase* /*pPane*/,
															   CRect rcCaption, BOOL bActive,
															   BOOL bVertical)
{
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL == hCustomBrush)
	{
		CXTPPaintManagerColorGradient& clr = bActive ? m_clrActiveCaption : m_clrNormalCaption;
		XTPDrawHelpers()->GradientFill(pDC, rcCaption, clr, bVertical);

		if (bActive)
		{
			CRect rTop(rcCaption);
			if (!bVertical)
				rTop.bottom = rTop.top + rTop.Height() / 2;
			else
				rTop.right = rTop.left + rTop.Width() / 2;

			XTPDrawHelpers()->GradientFill(pDC, rTop, RGB(255, 252, 242), RGB(255, 243, 207),
										   bVertical);
		}

		if (!bVertical)
		{
			pDC->SetPixel(rcCaption.left, rcCaption.top,
						  CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)60 / 255));
			pDC->SetPixel(rcCaption.left + 1, rcCaption.top,
						  CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
			pDC->SetPixel(rcCaption.left, rcCaption.top + 1,
						  CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));

			pDC->SetPixel(rcCaption.right - 1, rcCaption.top,
						  CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)60 / 255));
			pDC->SetPixel(rcCaption.right - 2, rcCaption.top,
						  CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
			pDC->SetPixel(rcCaption.right - 1, rcCaption.top + 1,
						  CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
		}
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	return bActive ? RGB(115, 97, 57) : RGB(206, 215, 222);
}
