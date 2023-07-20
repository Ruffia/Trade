// XTPDockingPaneThemeOffice2013.cpp : implementation of the CXTPDockingPaneOffice2013Theme class.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPTheme.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneManager.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPaneSplitterContainer.h"
#include "DockingPane/XTPDockingPaneSidePanel.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet2013.h"
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2012.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2013.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2013.h"
#include "DockingPane/TabThemes/XTPDockingPanePanelTheme2012.h"
#include "DockingPane/TabThemes/XTPDockingPanePanelTheme2013.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2013.h"
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2013Theme

CXTPDockingPaneOffice2013Theme::CXTPDockingPaneOffice2013Theme()
{
	m_bUseBoldCaptionFont			  = FALSE;
	m_bAutoHidePanelHighlightSelected = FALSE;

	// Tab paint manager
	m_pTabPaintManager->SetAppearanceSet(new CXTPDockingPaneTabTheme2013());
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSet2013());
	m_pTabPaintManager->m_clientFrame  = xtpTabFrameNone;
	m_pTabPaintManager->m_bShowIcons   = FALSE;
	m_pTabPaintManager->m_bHotTracking = TRUE;
	m_pTabPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(2), 0, XTP_DPI_Y(2));

	// Panel paint manager
	m_pPanelPaintManager->SetAppearanceSet(new CXTPDockingPanePanelTheme2013());
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPanePanelColorSet2013());
	m_pPanelPaintManager->SetLayout(xtpTabLayoutAutoSize);
	m_pPanelPaintManager->m_bRotateImageOnVerticalDraw = TRUE;
	m_pPanelPaintManager->m_rcClientMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcButtonMargin.SetRect(XTP_DPI_X(6), 0, 0, 0);
	m_pPanelPaintManager->m_bShowIcons   = FALSE;
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	// Pane color sets
	SetPaneColorSets(new CXTPDockingPaneColorSet2013, new CXTPDockingPaneColorSet2013);
}

CXTPDockingPaneOffice2013Theme::~CXTPDockingPaneOffice2013Theme()
{
}

void CXTPDockingPaneOffice2013Theme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();

	XTP_SAFE_CALL1(GetColorSet(TRUE), RefreshMetrics(_T("DockingPane.Active")));
	XTP_SAFE_CALL1(GetColorSet(FALSE), RefreshMetrics(_T("DockingPane.Inactive")));

	LPCTSTR lpszSection = _T("DockingPane");
	m_strFontFace		= XTPIniString(lpszSection, _T("FontFace"), _T("Segoe UI"));
	m_nFontSize			= XTP_DPI_Y(XTPIniInt(lpszSection, _T("FontSize"), 16));
	m_rcCaptionMargin   = XTP_DPI(XTPIniRect(lpszSection, _T("CaptionMargin"), CRect(5, 5, 5, 0)));
	m_clrSplitter		= XTPIniColor(lpszSection, _T("Splitter"), RGB(212, 212, 212));
	m_clrBackground		= XTPIniColor(lpszSection, _T("Background"), RGB(255, 255, 255));

	if (CXTPDrawHelpers::FontExists(m_strFontFace))
	{
		LOGFONT lf;

		if (CXTPDrawHelpers::GetDefaultLogFont(&lf))
		{
			if (XTPDrawHelpers()->FontExists(m_strFontFace))
				STRCPY_S(lf.lfFaceName, LF_FACESIZE, m_strFontFace);

			if (m_nFontSize > 0)
				m_nFontSize = -m_nFontSize;

			lf.lfHeight = m_nFontSize;
			lf.lfWidth  = 0;
			lf.lfWeight = FW_NORMAL;

			SetCaptionFontIndirect(&lf);
		}
	}
}

void CXTPDockingPaneOffice2013Theme::DrawSplitter(CDC* pDC, CXTPDockingPaneSplitterWnd* pSplitter)
{
	ASSERT(pDC);
	ASSERT(pSplitter);

	CXTPClientRect rc(pSplitter);
	pDC->FillSolidRect(rc, m_clrBackground);

	if (pSplitter->IsHorizontal())
	{
		rc.DeflateRect(0, XTP_DPI_Y(1));
	}
	else
	{
		rc.DeflateRect(XTP_DPI_X(1), 0);
	}

	pDC->FillSolidRect(rc, m_clrSplitter);
}

void CXTPDockingPaneOffice2013Theme::DrawTitleBar(CDC* pDC, CXTPDockingPaneBase* pPane,
												  CRect rcCaption, BOOL bVertical)
{
	BOOL bActive = pPane->IsActive() && m_bHighlightActiveCaption;

	CXTPDockingPaneColorSet* pColorSet = GetColorSet(bActive);
	ASSERT(pColorSet);

	if (!pColorSet)
		return;

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, pPane->IsCaptionVertical());
	if (NULL == hCustomBrush)
	{
		pDC->FillSolidRect(rcCaption, pColorSet->m_clrCaption);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	if (bVertical)
	{
		rcCaption.top += m_rcCaptionMargin.left;
		rcCaption.bottom -= m_rcCaptionMargin.right;
	}
	else
	{
		rcCaption.left += m_rcCaptionMargin.left;
		rcCaption.right -= m_rcCaptionMargin.right;
	}

	pDC->SetTextColor(pColorSet->m_clrText);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, pColorSet->m_clrText,
					   XTP_DPI_X(16), 0, bVertical);
	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, pPane->GetTitle(), rcCaption, bVertical);
}

// Draw caption when docked
void CXTPDockingPaneOffice2013Theme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
												 CRect rc)
{
	CRect rcCaption(rc);
	BOOL bVertical = pPane->IsCaptionVertical();
	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - m_nTitleHeight, 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - m_nTitleHeight);
	}

	DrawTitleBar(pDC, pPane, rcCaption, bVertical);
}

void CXTPDockingPaneOffice2013Theme::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
											  CRect rc)
{
	CXTPDockingPaneColorSet* pColorSet = GetColorSet(pPane->IsActive());
	ASSERT(pColorSet);

	if (!pColorSet)
		return;

	pDC->FillSolidRect(rc, pColorSet->m_clrBackground);
	pDC->SetBkMode(TRANSPARENT);

	CRect rcCaption(rc);

	if (pPane->IsTabsVisible())
	{
		AdjustCaptionRect(pPane, rc);
		m_pTabPaintManager->DrawTabControl(pPane, pDC, rc);
	}

	if (pPane->IsTitleVisible())
	{
		DrawCaption(pDC, pPane, rcCaption);
	}
}

void CXTPDockingPaneOffice2013Theme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane,
												   CRect rc)
{
	CXTPDockingPaneColorSet* pColorSet = GetColorSet(pPane->IsActive());
	ASSERT(pColorSet);

	if (!pColorSet)
		return;

	// Border
	CRect rcBorder(rc);
	pDC->Draw3dRect(rcBorder, pColorSet->m_clrBorder, pColorSet->m_clrBorder);

	// Background
	CRect rcBackground(rc);
	rcBackground.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rcBackground, pColorSet->m_clrBackground);

	XTPDockingPaneDirection direction = pPane->GetDirection();
	BOOL bVertical = (xtpPaneDockLeft == direction || xtpPaneDockRight == direction);

	CRect rcCaption = GetPaneCaptionRect(pPane);
	DrawTitleBar(pDC, pPane, rcCaption, bVertical);
}

void CXTPDockingPaneOffice2013Theme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane,
													   CRect rc)
{
	CXTPDockingPaneColorSet* pColorSet = GetColorSet(pPane->IsActive());
	ASSERT(pColorSet);

	if (!pColorSet)
		return;

	// Border
	CRect rcBorder(rc);
	pDC->Draw3dRect(rcBorder, pColorSet->m_clrBorder, pColorSet->m_clrBorder);

	// Background
	CRect rcBackground(rc);
	rcBackground.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rcBackground, pColorSet->m_clrBackground);

	CRect rcCaption(rcBackground);
	rcCaption.bottom = rcCaption.top + m_nTitleHeight;
	DrawTitleBar(pDC, pPane, rcCaption, FALSE);
}

void CXTPDockingPaneOffice2013Theme::DrawCaptionButton(CDC* pDC,
													   CXTPDockingPaneCaptionButton* pButton,
													   COLORREF clrButton)
{
	UNUSED_ALWAYS(clrButton);

	CXTPDockingPaneColorSet* pColorSet = GetColorSet(
		XTP_SAFE_GET2(pButton, GetPane(), IsActive() && m_bHighlightActiveCaption, FALSE));
	ASSERT(pColorSet);

	if (!pColorSet)
		return;

	CXTPColorSetCaptionButton& colorSetButton = pColorSet->GetButtonColorSet(pButton);

	BOOL bActive   = XTP_SAFE_GET2(pButton, GetPane(), IsActive() && m_bHighlightActiveCaption,
								   FALSE);
	BOOL bVertical = XTP_SAFE_GET2(pButton, GetPane(), IsCaptionVertical(), FALSE);

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (hCustomBrush)
	{
		pDC->FillRect(pButton->GetRect(), CBrush::FromHandle(hCustomBrush));
	}
	else
	{
		pDC->FillSolidRect(pButton->GetRect(), colorSetButton.m_clrBackground);
	}

	DrawCaptionButtonEntry(pDC, pButton, colorSetButton.m_clrFill, CSize(0, 0));
}

void CXTPDockingPaneOffice2013Theme::DrawAutoHidePanel(CDC* pDC, CRect rc,
													   XTPDockingPaneDirection direction)
{
	pDC->FillSolidRect(rc, m_clrBackground);

	switch (direction)
	{
		case xtpPaneDockLeft:
			pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.Height(),
							   m_clrSplitter);
			break;

		case xtpPaneDockTop:
			pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   m_clrSplitter);
			break;

		case xtpPaneDockRight:
			pDC->FillSolidRect(rc.left, rc.top, XTP_DPI_X(1), rc.Height(), m_clrSplitter);
			break;

		case xtpPaneDockBottom:
			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_Y(1), m_clrSplitter);
			break;
	}
}
