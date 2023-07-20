// XTPDockingPaneThemeDefault.cpp : implementation of the CXTPDockingPaneDefaultTheme class.
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

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPaneSidePanel.h"
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneDefaultTheme

CXTPDockingPaneDefaultTheme::CXTPDockingPaneDefaultTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);

	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
}

void CXTPDockingPaneDefaultTheme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));

	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));
}

void CXTPDockingPaneDefaultTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane,
													CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), 0);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	dcCache.FillSolidRect(rcCaption, GetXtremeColor(COLOR_3DFACE));

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(2), XTP_DPI_X(1), XTP_DPI_Y(2));

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneDefaultTheme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	pDC->SetBkMode(TRANSPARENT);

	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));

	CRect rcCaption = GetPaneCaptionRect(pPane);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	CString strTitle = pPane->GetTitle();

	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive,
					pPane->GetDirection() == xtpPaneDockLeft
						|| pPane->GetDirection() == xtpPaneDockRight);
}

void CXTPDockingPaneDefaultTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane,
												  CRect rcCaption, const CString& strTitle,
												  BOOL bActive, BOOL bVertical)
{
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL == hCustomBrush)
	{
		pDC->FillSolidRect(rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	XTPDeflateRectDirection(rcCaption, 0, 0, XTP_DPI_X(2), 0, bVertical);
	int nButtonSize = (bVertical ? rcCaption.Width() : rcCaption.Height()) - XTP_DPI_X(2) * 2;
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT),
					   nButtonSize /*XTP_DPI_X(14)*/, 0, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane)
						  ? (bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText)
						  : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(4), XTP_DPI_Y(1), 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneDefaultTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
											  CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, XTP_DPI_Y(2), 0,
							XTPGetRectDirectionHeight(rc, bVertical) - m_nTitleHeight
								- XTP_DPI_Y(1),
							bVertical);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	if (bVertical)
	{
		pDC->FillSolidRect(rc.left, rc.top, rcCaption.right + XTP_DPI_X(2), rcCaption.Height(),
						   GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + XTP_DPI_Y(2),
						   GetXtremeColor(COLOR_3DFACE));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}
