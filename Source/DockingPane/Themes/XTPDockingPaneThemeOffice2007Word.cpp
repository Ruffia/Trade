// XTPDockingPaneThemeOffice2007Word.cpp : implementation of the CXTPDockingPaneOffice2007WordTheme
// class.
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

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2007Word.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007WordTheme

CXTPDockingPaneOffice2007WordTheme::CXTPDockingPaneOffice2007WordTheme()
{
	m_bRoundedCaption = FALSE;
	m_dwSplitterStyle = xtpPaneSplitterFlat;
	m_bDrawGripper	= FALSE;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageAccess2007);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageAccess2007);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_nCaptionFontGap = XTP_DPI_X(2);
	m_nSplitterSize   = 0;
	m_nSplitterIndent = XTP_DPI_X(2);
}

void CXTPDockingPaneOffice2007WordTheme::DrawCaption(CDC* pDC,
													 CXTPDockingPaneTabbedContainer* pPane,
													 CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - XTP_DPI_X(3), 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));
	}

	CRect rcFill(rcCaption);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcFill,
									   bActive ? m_clrActiveCaption : m_clrNormalCaption,
									   bVertical);
	}
	else
	{
		pDC->FillRect(rcFill, CBrush::FromHandle(hCustomBrush));
	}

	CString strTitle = pPane->GetTitle();
	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
}

void CXTPDockingPaneOffice2007WordTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane,
														 CRect rcCaption, const CString& strTitle,
														 BOOL bActive, BOOL bVertical)
{
	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, XTP_DPI_X(16), 0,
					   bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText
											  : m_clrDisabledCaptionText.operator COLORREF());

	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

BOOL CXTPDockingPaneOffice2007WordTheme::PaneHasBorders(CXTPDockingPaneTabbedContainer* pPane) const
{
	return pPane->IsTitleVisible();
}

void CXTPDockingPaneOffice2007WordTheme::AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane,
														  CRect& rect, BOOL bApply)
{
	if (PaneHasBorders(pPane))
	{
		rect.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
	}
	CXTPDockingPaneOffice2003Theme::AdjustClientRect(pPane, rect, bApply);
}

void CXTPDockingPaneOffice2007WordTheme::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
												  CRect rc)
{
	if (m_bOffice2010Style)
	{
		pDC->FillSolidRect(rc, m_arrColor[XPCOLOR_3DFACE]);
		pDC->Draw3dRect(rc.left, rc.top, rc.Width(), rc.Height(),
						m_arrColor[XPCOLOR_TOOLBAR_GRIPPER], m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
		rc.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
	}
	else
	{
		if (PaneHasBorders(pPane))
		{
			pDC->FillSolidRect(rc, m_arrColor[XPCOLOR_FRAME]);

			pDC->FillSolidRect(rc.left + XTP_DPI_X(3), rc.top + XTP_DPI_Y(2),
							   rc.Width() - XTP_DPI_X(6), XTP_DPI_Y(1),
							   m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
			pDC->FillSolidRect(rc.left + XTP_DPI_X(2), rc.top + XTP_DPI_Y(3), XTP_DPI_X(1),
							   rc.Height() - XTP_DPI_Y(5), m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
			pDC->FillSolidRect(rc.right - XTP_DPI_X(3), rc.top + XTP_DPI_Y(3), XTP_DPI_X(1),
							   rc.Height() - XTP_DPI_Y(5), m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
			pDC->FillSolidRect(rc.left + XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(3),
							   rc.Width() - XTP_DPI_X(4), XTP_DPI_Y(1),
							   m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);

			rc.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
		}
	}

	CXTPDockingPaneOffice2003Theme::DrawPane(pDC, pPane, rc);
}

void CXTPDockingPaneOffice2007WordTheme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();
	ASSERT_VALID(pImages);

	m_bOffice2010Style = pImages->GetImageInt(_T("DockingPane"), _T("Office2010Style"), FALSE);

	m_arrColor[XPCOLOR_FRAME]	 = pImages->GetImageColor(_T("DockingPane"), _T("WordSplitter"));
	m_arrColor[XPCOLOR_3DFACE]	= pImages->GetImageColor(_T("DockingPane"),
														   _T("WordPaneBackground"));
	m_arrColor[COLOR_3DHIGHLIGHT] = pImages->GetImageColor(_T("Window"), _T("EdgeHighLightColor"));

	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER] = pImages->GetImageColor(_T("DockingPane"),
																 _T("WordPaneBorder"));
	m_arrColor[XPCOLOR_MENUBAR_BORDER]  = pImages->GetImageColor(_T("DockingPane"),
																 _T("WordSplitter"));

	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER] = pImages->GetImageColor(_T("Window"),
																  _T("HighlightSelectedBorder"));
	m_arrColor[XPCOLOR_HIGHLIGHT] = pImages->GetImageColor(_T("Window"), _T("HighlightSelected"));
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] =
		pImages->GetImageColor(_T("Window"), _T("HighlightPressedBorder"));
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED] = pImages->GetImageColor(_T("Window"),
																  _T("HighlightPressed"));
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]   = 0;
	m_arrColor[XPCOLOR_PUSHED_TEXT]		 = 0;

	m_bLunaTheme = FALSE;

	m_clrSplitterGradient.SetStandardValue(
		pImages->GetImageColor(_T("DockingPane"), _T("WordSplitter")));
	m_clrSplitter.SetStandardValue(m_clrSplitterGradient);

	m_clrNormalCaption.SetStandardValue(
		pImages->GetImageColor(_T("DockingPane"), _T("WordNormalCaption")));
	m_clrNormalCaptionText.SetStandardValue(
		pImages->GetImageColor(_T("DockingPane"), _T("WordCaptionText")));

	m_clrActiveCaption.SetStandardValue(
		pImages->GetImageColor(_T("DockingPane"), _T("WordActiveCaption")));
	m_clrActiveCaptionText.SetStandardValue(
		pImages->GetImageColor(_T("DockingPane"), _T("WordCaptionText")));
	m_clrSplitterGripper.SetStandardValue(m_clrNormalCaptionText);

	if (m_bOffice2010Style)
	{
		m_nSplitterSize   = XTP_DPI_X(4);
		m_nSplitterIndent = 0;
	}
	else
	{
		m_nSplitterSize   = 0;
		m_nSplitterIndent = XTP_DPI_X(2);
	}

	if (m_bOffice2010Style)
	{
		m_arrColor[XPCOLOR_MENUBAR_BORDER] = m_arrColor[XPCOLOR_TOOLBAR_GRIPPER];
	}
}
