// XTPDockingPaneThemeNativeWindows10.cpp : implementation of the
// CXTPDockingPaneNativeWindows10Theme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPTheme.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneSplitterContainer.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSetNativeWindows10.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2012.h"
#include "DockingPane/Themes/XTPDockingPaneThemeNativeWindows10.h"

#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2012.h"
#include "DockingPane/TabThemes/XTPDockingPaneTabThemeNativeWindows10.h"
#include "DockingPane/TabThemes/XTPDockingPanePanelTheme2012.h"
#include "DockingPane/TabThemes/XTPDockingPanePanelThemeNativeWindows10.h"

#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSetNativeWindows10.h"
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSetNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneNativeWindows10Theme::CXTPDockingPaneNativeWindows10Theme()
{
	// Tab paint manager
	m_pTabPaintManager->SetAppearanceSet(new CXTPDockingPaneTabThemeNativeWindows10());
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetNativeWindows10());

	// Panel paint manager
	m_pPanelPaintManager->SetAppearanceSet(new CXTPDockingPanePanelThemeNativeWindows10());
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPanePanelColorSetNativeWindows10());

	// Pane color sets
	SetPaneColorSets(new CXTPDockingPaneColorSetNativeWindows10,
					 new CXTPDockingPaneColorSetNativeWindows10);
}

CXTPDockingPaneNativeWindows10Theme::~CXTPDockingPaneNativeWindows10Theme()
{
}

void CXTPDockingPaneNativeWindows10Theme::RefreshMetrics()
{
	CXTPDockingPaneDefaultTheme::RefreshMetrics();

	XTP_SAFE_CALL1(GetColorSet(TRUE), RefreshMetrics(_T("DockingPane.Active")));
	XTP_SAFE_CALL1(GetColorSet(FALSE), RefreshMetrics(_T("DockingPane.Inactive")));

	CString strSection = _T("DockingPane");

	m_strFontFace	 = XTPIniString(strSection, _T("FontFace"), _T("Segoe UI"));
	m_nFontSize		  = XTP_DPI_Y(XTPIniInt(strSection, _T("FontSize"), 10));
	m_rcCaptionMargin = XTP_DPI(XTPIniRect(strSection, _T("CaptionMargin"), CRect(5, 0, 5, 0)));
	m_clrSplitter	 = XTPIniColor(strSection, _T("Splitter"), RGB(239, 239, 242));
	m_clrBackground   = XTPIniColor(strSection, _T("Background"), RGB(239, 239, 242));
	m_clrSplitterGradient.clrLight = XTPIniColor(strSection, _T("Splitter"), RGB(239, 239, 242));
	m_clrSplitterGradient.clrDark = XTPIniColor(strSection, _T("SplitterDark"), RGB(239, 239, 242));
	m_clrCaptionBorder			  = XTPIniColor(strSection + _T(".Tabs"), _T("ButtonBorder"),
										RGB(204, 206, 219));
}

void CXTPDockingPaneNativeWindows10Theme::DrawSplitter(CDC* pDC,
													   CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc(pSplitter);
	pDC->FillSolidRect(rc, m_clrSplitter);

	if (pSplitter->IsHorizontal())
	{
		pDC->FillSolidRect(rc, m_clrSplitterGradient.clrDark);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrSplitterGradient.clrLight);
	}
}

void CXTPDockingPaneNativeWindows10Theme::DrawTitleBar(CDC* pDC, CXTPDockingPaneBase* pPane,
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
		if (bVertical || pPane->GetType() == xtpPaneTypeMiniWnd)
		{
			pDC->FillSolidRect(rcCaption, pColorSet->m_clrCaption);
		}
		else
		{
			pDC->FillSolidRect(rcCaption, pColorSet->m_clrCaption);

			COLORREF clrBorder = bActive ? pColorSet->m_clrCaption.operator COLORREF()
										 : m_clrCaptionBorder;
			COLORREF pClrBorder[4] = { clrBorder, clrBorder, clrBorder,
									   pColorSet->m_clrCaption.operator COLORREF() };
			CXTPDrawHelpers::DrawBorder(pDC, rcCaption, pClrBorder);
		}
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

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);

	pDC->SetTextColor(pColorSet->m_clrText);
	int nWidth = DrawCaptionText(pDC, pPane->GetTitle(), rcCaption, bVertical, TRUE);

	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, pColorSet->m_clrText,
					   XTP_DPI_X(16), 0, bVertical);

	if (NULL == hCustomBrush)
	{
		CRect rcGripper(rcCaption);

		if (bVertical)
		{
			rcGripper.top += nWidth + XTP_DPI_Y(5);
			rcGripper.left += ((rcCaption.Width() - XTP_DPI_X(5)) / 2);
		}
		else
		{
			rcGripper.top += ((rcCaption.Height() - XTP_DPI_Y(5)) / 2);
			rcGripper.left += nWidth + XTP_DPI_X(5);
		}

		DrawGripper(pDC, rcGripper, pColorSet->m_clrGripper, bVertical);
	}
}

void CXTPDockingPaneNativeWindows10Theme::DrawAutoHidePanel(CDC* pDC, CRect rc,
															XTPDockingPaneDirection direction)
{
	pDC->FillSolidRect(rc, m_clrBackground);

	switch (direction)
	{
		case xtpPaneDockLeft:
			pDC->FillSolidRect(rc.right - XTP_DPI_X(4), rc.top, XTP_DPI_X(4), rc.Height(),
							   m_clrSplitter);
			pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.Height(),
							   m_clrCaptionBorder);
			break;

		case xtpPaneDockTop:
			pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(4), rc.Width(), XTP_DPI_Y(4),
							   m_clrSplitter);
			pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
							   m_clrCaptionBorder);
			break;

		case xtpPaneDockRight:
			pDC->FillSolidRect(rc.left, rc.top, XTP_DPI_X(4), rc.Height(), m_clrSplitter);
			pDC->FillSolidRect(rc.left, rc.top, XTP_DPI_X(1), rc.Height(), m_clrCaptionBorder);
			break;

		case xtpPaneDockBottom:
			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_Y(4), m_clrSplitter);
			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_Y(1), m_clrCaptionBorder);
			break;
	}
}
