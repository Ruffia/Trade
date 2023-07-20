// XTPDockingPaneThemeVisualStudio2012.cpp : implementation of the
// CXTPDockingPaneVisualStudio2012Theme class.
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
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPaneSplitterContainer.h"
#include "DockingPane/XTPDockingPaneSidePanel.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet2012.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2012.h"

#include "DockingPane/TabThemes/XTPDockingPaneTabTheme2012.h"
#include "DockingPane/TabThemes/XTPDockingPanePanelTheme2012.h"
#include "DockingPane/TabColorSets/XTPDockingPaneTabColorSet2012.h"
#include "DockingPane/TabColorSets/XTPDockingPanePanelColorSet2012.h"
#include "DockingPane/XTPDockingPaneManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2012Theme

CXTPDockingPaneVisualStudio2012Theme::CXTPDockingPaneVisualStudio2012Theme()
	: m_pcsActive(NULL)
	, m_pcsInactive(NULL)
{
	m_bUseBoldCaptionFont			  = FALSE;
	m_bAutoHidePanelHighlightSelected = FALSE;

	// Tab paint manager
	m_pTabPaintManager->SetAppearanceSet(new CXTPDockingPaneTabTheme2012());
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSet2012());
	m_pTabPaintManager->m_clientFrame  = xtpTabFrameNone;
	m_pTabPaintManager->m_bShowIcons   = FALSE;
	m_pTabPaintManager->m_bHotTracking = TRUE;
	m_pTabPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(1), 0, XTP_DPI_Y(1));

	// Panel paint manager
	m_pPanelPaintManager->SetAppearanceSet(new CXTPDockingPanePanelTheme2012());
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPanePanelColorSet2012());
	m_pPanelPaintManager->SetLayout(xtpTabLayoutAutoSize);
	m_pPanelPaintManager->m_bRotateImageOnVerticalDraw = TRUE;
	m_pPanelPaintManager->m_rcClientMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcButtonMargin.SetRect(XTP_DPI_X(6), 0, 0, 0);
	m_pPanelPaintManager->m_bShowIcons   = FALSE;
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	// Pane color sets
	SetPaneColorSets(new CXTPDockingPaneColorSet2012, new CXTPDockingPaneColorSet2012);
}

CXTPDockingPaneVisualStudio2012Theme::~CXTPDockingPaneVisualStudio2012Theme()
{
	DeletePaneColorSets();
}

void CXTPDockingPaneVisualStudio2012Theme::SetPaneColorSets(CXTPDockingPaneColorSet* pcsActive,
															CXTPDockingPaneColorSet* pcsInactive)
{
	DeletePaneColorSets();
	m_pcsActive   = pcsActive;
	m_pcsInactive = pcsInactive;
}

void CXTPDockingPaneVisualStudio2012Theme::DeletePaneColorSets()
{
	SAFE_DELETE(m_pcsActive);
	SAFE_DELETE(m_pcsInactive);
}

void CXTPDockingPaneVisualStudio2012Theme::RefreshMetrics()
{
	CXTPDockingPaneDefaultTheme::RefreshMetrics();

	XTP_SAFE_CALL1(GetColorSet(TRUE), RefreshMetrics(_T("DockingPane.Active")));
	XTP_SAFE_CALL1(GetColorSet(FALSE), RefreshMetrics(_T("DockingPane.Inactive")));

	CString strSection = _T("DockingPane");
	m_strFontFace	  = XTPIniString(strSection, _T("FontFace"), _T("Segoe UI"));
	m_nFontSize		   = XTP_DPI_Y(XTPIniInt(strSection, _T("FontSize"), 10));
	m_rcCaptionMargin  = XTP_DPI(XTPIniRect(strSection, _T("CaptionMargin"), CRect(5, 0, 5, 0)));
	m_clrSplitter	  = XTPIniColor(strSection, _T("Splitter"), RGB(239, 239, 242));
	m_clrBackground	= XTPIniColor(strSection, _T("Background"), RGB(239, 239, 242));
	m_clrSplitterGradient.clrLight = XTPIniColor(strSection, _T("Splitter"), RGB(239, 239, 242));
	m_clrSplitterGradient.clrDark = XTPIniColor(strSection, _T("SplitterDark"), RGB(204, 206, 219));
}

void CXTPDockingPaneVisualStudio2012Theme::DrawSplitter(CDC* pDC,
														CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc(pSplitter);
	pDC->FillSolidRect(rc, m_clrSplitter);

	CXTPDockingPaneManager* pPaneManager = pSplitter->GetDockingPaneManager();
	if (pPaneManager)
	{
		CXTPDockingPaneBase* pPaneBase = pSplitter->GetPaneBase();
		if (pPaneBase)
		{
			XTPDockingPaneDirection direction = pPaneManager->GetPaneDirection(pPaneBase);

			CRect rc1(rc);
			CRect rc2(rc);

			BOOL bIsHoriz = pSplitter->IsHorizontal();
			if (bIsHoriz)
			{
				rc.DeflateRect(XTP_DPI_X(2), 0, XTP_DPI_X(2), 0);
				pDC->FillSolidRect(rc, m_clrSplitterGradient.clrDark);
			}
			else
			{
				rc1.bottom = rc1.top + rc1.Height() / 2;
				rc2.top	= rc1.bottom;

				if (direction == xtpPaneDockLeft)
				{
					rc1.left = rc1.right - XTP_DPI_X(2);
					rc2.left = rc2.right - XTP_DPI_X(2);
				}
				else
				{
					rc1.right = rc1.left + XTP_DPI_X(2);
					rc2.right = rc2.left + XTP_DPI_X(2);
				}

				XTPDrawHelpers()->GradientFill(pDC, &rc1, m_clrSplitterGradient.clrLight,
											   m_clrSplitterGradient.clrDark, bIsHoriz);

				XTPDrawHelpers()->GradientFill(pDC, &rc2, m_clrSplitterGradient.clrDark,
											   m_clrSplitterGradient.clrLight, bIsHoriz);
			}
		}
	}
}

CRect CXTPDockingPaneVisualStudio2012Theme::GetPaneCaptionRect(const CXTPDockingPaneSidePanel* pPane)
{
	return CXTPDockingPaneDefaultTheme::GetPaneCaptionRect(pPane);
}

void CXTPDockingPaneVisualStudio2012Theme::AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane,
															CRect& rc, BOOL bApply)
{
	CXTPDockingPaneDefaultTheme::AdjustClientRect(pPane, rc, bApply);
}

void CXTPDockingPaneVisualStudio2012Theme::AdjustCaptionRect(
	const CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	CXTPDockingPaneDefaultTheme::AdjustCaptionRect(pPane, rc);
}

void CXTPDockingPaneVisualStudio2012Theme::DrawGripper(CDC* pDC, CRect rcGripper, COLORREF clr,
													   BOOL bVertical)
{
	if (bVertical)
	{
		DrawDottedLine(pDC, rcGripper.left, rcGripper.top, rcGripper.bottom, clr, TRUE);
		DrawDottedLine(pDC, rcGripper.left + XTP_DPI_X(2), rcGripper.top + XTP_DPI_Y(2),
					   rcGripper.bottom, clr, TRUE);
		DrawDottedLine(pDC, rcGripper.left + XTP_DPI_X(4), rcGripper.top, rcGripper.bottom, clr,
					   TRUE);
	}
	else
	{
		DrawDottedLine(pDC, rcGripper.left, rcGripper.top, rcGripper.right, clr, FALSE);
		DrawDottedLine(pDC, rcGripper.left + XTP_DPI_X(2), rcGripper.top + XTP_DPI_Y(2),
					   rcGripper.right, clr, FALSE);
		DrawDottedLine(pDC, rcGripper.left, rcGripper.top + XTP_DPI_Y(4), rcGripper.right, clr,
					   FALSE);
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawDottedLine(CDC* pDC, int x, int y, int nSize,
														  COLORREF clr, BOOL bVertical)
{
	if (bVertical)
	{
		while (y < nSize)
		{
			pDC->SetPixel(x, y, clr);
			y += XTP_DPI_Y(4);
		}
	}
	else
	{
		while (x < nSize)
		{
			pDC->SetPixel(x, y, clr);
			x += XTP_DPI_X(4);
		}
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawTitleBar(CDC* pDC, CXTPDockingPaneBase* pPane,
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
			pDC->FillSolidRect(rcCaption, GetColorSet(FALSE)->m_clrCaption);
			rcCaption.DeflateRect(XTP_DPI_X(2), 0);
			pDC->FillSolidRect(rcCaption, pColorSet->m_clrCaption);
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

// Draw caption when docked
void CXTPDockingPaneVisualStudio2012Theme::DrawCaption(CDC* pDC,
													   CXTPDockingPaneTabbedContainer* pPane,
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

void CXTPDockingPaneVisualStudio2012Theme::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
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

void CXTPDockingPaneVisualStudio2012Theme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane,
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

void CXTPDockingPaneVisualStudio2012Theme::DrawFloatingFrame(CDC* pDC,
															 CXTPDockingPaneMiniWnd* pPane,
															 CRect rc)
{
	CXTPDockingPaneColorSet* pColorSet = GetColorSet(pPane->IsActive());
	ASSERT(pColorSet);

	if (!pColorSet)
		return;

	CString strTitle;
	strTitle = pPane->GetTitle();

	if (pPane->IsCaptionVertical())
	{
		pDC->FillSolidRect(rc, RGB(255, 0, 0));
	}
	else
	{
		// Border
		CRect rcBorder(rc);
		pDC->Draw3dRect(rcBorder, pColorSet->m_clrBorder, pColorSet->m_clrBorder);

		// Background
		CRect rcBackground(rc);
		rcBackground.DeflateRect(1, 1, 1, 1);
		pDC->FillSolidRect(rcBackground, pColorSet->m_clrBackground);

		// Caption
		CRect rcCaption(rcBackground);
		rcCaption.bottom = rcCaption.top + m_nTitleHeight;

		DrawTitleBar(pDC, pPane, rcCaption, FALSE);
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawCaptionButton(CDC* pDC,
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

	if (pButton->IsSelected() || pButton->IsPressed())
	{
		pDC->Draw3dRect(pButton->GetRect(), colorSetButton.m_clrOutline,
						colorSetButton.m_clrOutline);
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawAutoHidePanel(CDC* pDC, CRect rc,
															 XTPDockingPaneDirection direction)
{
	pDC->FillSolidRect(rc, m_clrBackground);

	switch (direction)
	{
		case xtpPaneDockLeft:
			pDC->FillSolidRect(rc.right - XTP_DPI_X(4), rc.top, XTP_DPI_X(4), rc.Height(),
							   m_clrSplitter);
			break;

		case xtpPaneDockTop:
			pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(4), rc.Width(), XTP_DPI_Y(4),
							   m_clrSplitter);
			break;

		case xtpPaneDockRight:
			pDC->FillSolidRect(rc.left, rc.top, XTP_DPI_X(4), rc.Height(), m_clrSplitter);
			break;

		case xtpPaneDockBottom:
			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), XTP_DPI_Y(4), m_clrSplitter);
			break;
	}
}
