// XTPDockingPaneThemeWinExplorer.cpp : implementation of the CXTPDockingPaneWinExplorerTheme class.
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
#include "Common/XTPWinThemeWrapper.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPanePaintManager.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeWinExplorer.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneWinExplorerTheme

CXTPDockingPaneWinExplorerTheme::CXTPDockingPaneWinExplorerTheme()
{
	m_bUseBoldCaptionFont = FALSE;
	m_rcCaptionMargin.SetRect(0, 0, 0, 0);
	m_nCaptionFontGap = XTP_DPI_X(9);

	m_nCaptionButtonStyle = xtpPaneCaptionButtonThemedToolBar;
}

void CXTPDockingPaneWinExplorerTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane,
													  CRect rcCaption, const CString& strTitle,
													  BOOL bActive, BOOL bVertical)
{
	if (m_themeRebar->IsAppThemeActive())
	{
		CWnd* pWnd = CWnd::FromHandle(pPane->GetPaneHwnd());

		ASSERT(pWnd);
		if (!pWnd)
			return;

		if (pPane->GetType() == xtpPaneTypeMiniWnd)
		{
			CRect rcClient;
			pWnd->GetWindowRect(rcClient);
			rcClient.OffsetRect(-rcClient.TopLeft());

			HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
			if (NULL == hCustomBrush)
			{
				m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, &rcCaption);
			}
			else
			{
				pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
			}
		}
		else
		{
			CWnd* pParent = pPane->GetDockingSite();

			CRect rc, rcParent, rcClient, rcClipRect;
			pWnd->GetClientRect(&rcClient);
			rcClipRect = rcClient;

			if (pParent)
			{
				pParent->GetWindowRect(rcParent);
				pWnd->GetWindowRect(rc);

				rcClient.TopLeft() += rcParent.TopLeft() - rc.TopLeft();
				rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();
			}

			HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
			if (NULL == hCustomBrush)
			{
				m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, &rcCaption);
			}
			else
			{
				pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
			}
		}
	}

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, XTP_DPI_X(2), 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, XTP_DPI_X(16),
					   XTP_DPI_X(2), bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText
											  : m_clrDisabledCaptionText.operator COLORREF());
	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(4), XTP_DPI_Y(1), 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneWinExplorerTheme::RefreshMetrics()
{
	CXTPDockingPaneGripperedTheme::RefreshMetrics();

	if (m_themeRebar->IsAppThemed())
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
		m_pPanelPaintManager->m_bHotTracking = TRUE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pTabPaintManager->SetColor(xtpTabColorWinNative);
		m_pTabPaintManager->m_bHotTracking = TRUE;
	}
	else
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);
		m_pPanelPaintManager->m_bHotTracking = FALSE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio2003);
		m_pTabPaintManager->m_bHotTracking = FALSE;
	}
}
