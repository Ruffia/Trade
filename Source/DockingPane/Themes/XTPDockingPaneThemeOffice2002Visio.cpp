// XTPDockingPaneThemeOffice2002Visio.cpp : implementation of the
// CXTPDockingPaneOffice2002VisioTheme class.
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
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2002Visio.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2002VisioTheme

CXTPDockingPaneOffice2002VisioTheme::CXTPDockingPaneOffice2002VisioTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);

	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisio);
	m_pTabPaintManager->SetPosition(xtpTabPositionTop);

	m_nCaptionFontGap	 = XTP_DPI_X(2);
	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice2003;
}

void CXTPDockingPaneOffice2002VisioTheme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();
}

void CXTPDockingPaneOffice2002VisioTheme::AdjustCaptionRect(
	const CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	CXTPDockingPaneVisualStudio2003Theme::AdjustCaptionRect(pPane, rc);

	BOOL bVertical = pPane->IsCaptionVertical();

	XTPDeflateRectDirection(rc, XTP_DPI_X(3), 0, XTP_DPI_X(3), XTP_DPI_Y(3), bVertical);

	if (!pPane->IsTitleVisible())
	{
		rc.DeflateRect(0, XTP_DPI_Y(3), 0, 0);
	}
	if (!pPane->IsTabsVisible() && m_pTabPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rc.DeflateRect(1, 1);
	}
}

void CXTPDockingPaneOffice2002VisioTheme::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
												   CRect rc)
{
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	pDC->FillSolidRect(rc,
					   GetXtremeColor(UINT(bActive ? XPCOLOR_HIGHLIGHT : XPCOLOR_TOOLBAR_FACE)));
	pDC->SetBkMode(TRANSPARENT);

	CRect rcClient(rc);

	if (pPane->IsTitleVisible())
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_clrSplitter);
		DrawCaption(pDC, pPane, rc);
		rcClient.top++;
	}

	AdjustCaptionRect(pPane, rc);

	if (pPane->IsTabsVisible())
	{
		m_pTabPaintManager->DrawTabControl(pPane, pDC, rc);
	}
	else if (m_pTabPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		pDC->Draw3dRect(rc.left - XTP_DPI_X(1), rc.top - XTP_DPI_Y(1), rc.Width() + XTP_DPI_X(2),
						rc.Height() + XTP_DPI_Y(2),
						m_pTabPaintManager->GetColorSet()->m_clrFrameBorder,
						m_pTabPaintManager->GetColorSet()->m_clrFrameBorder);
	}

	Pixel(pDC, rcClient.left, rcClient.top, XPCOLOR_3DFACE);
	Pixel(pDC, rcClient.right - XTP_DPI_X(1), rcClient.top, XPCOLOR_3DFACE);
	Pixel(pDC, rcClient.left, rcClient.bottom - XTP_DPI_Y(1), XPCOLOR_3DFACE);
	Pixel(pDC, rcClient.right - XTP_DPI_X(1), rcClient.bottom - XTP_DPI_Y(1), XPCOLOR_3DFACE);
}

void CXTPDockingPaneOffice2002VisioTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane,
															CRect rc)
{
	BOOL bActive	  = m_bHighlightActiveCaption && pPane->IsActive();
	COLORREF clrFrame = GetXtremeColor(UINT(bActive ? XPCOLOR_HIGHLIGHT : XPCOLOR_TOOLBAR_FACE));

	pDC->FillSolidRect(rc, clrFrame);
	pDC->Draw3dRect(rc, clrFrame, 0);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, pPane->IsCaptionVertical());
	if (NULL == hCustomBrush)
	{
		pDC->FillSolidRect(rcCaption, clrFrame);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	rcCaption.right -= XTP_DPI_X(2);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT),
					   XTP_DPI_X(16), 0, FALSE);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	int nRight = rcCaption.right;

	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);
	int nLeft		 = XTP_DPI_X(4);
	if (!strTitle.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rcText(rcCaption.left + XTP_DPI_X(4), rcCaption.top + XTP_DPI_Y(1),
					 nRight - XTP_DPI_X(2), rcCaption.bottom);
		pDC->SetTextColor(clrText);
		CXTPFontDC font(pDC, &m_xtpFontTitle);
		pDC->DrawText(strTitle, rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX);

		CRect rcCalc(0, 0, 0, 0);
		pDC->DrawText(strTitle, rcCalc, DT_CALCRECT | DT_SINGLELINE | DT_NOPREFIX);

		nLeft = rcCalc.Width() + rcCaption.left + XTP_DPI_X(14);
	}
	nRight -= XTP_DPI_X(4);

	if (NULL == hCustomBrush)
	{
		if (nRight - nLeft > 0)
		{
			int nCenter = rcCaption.CenterPoint().y;
			Line(pDC, nLeft, nCenter - XTP_DPI_Y(3), nRight, nCenter - XTP_DPI_Y(3),
				 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nLeft, nCenter - XTP_DPI_Y(1), nRight, nCenter - XTP_DPI_Y(1),
				 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nLeft, nCenter + XTP_DPI_Y(1), nRight, nCenter + XTP_DPI_Y(1),
				 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nLeft, nCenter + XTP_DPI_Y(3), nRight, nCenter + XTP_DPI_Y(3),
				 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
		}
	}
}

void CXTPDockingPaneOffice2002VisioTheme::DrawCaption(CDC* pDC,
													  CXTPDockingPaneTabbedContainer* pPane,
													  CRect rc)
{
	BOOL bVertical   = pPane->IsCaptionVertical();
	BOOL bActive	 = m_bHighlightActiveCaption && pPane->IsActive();
	int nTitleHeight = m_nTitleHeight;
	CString strTitle = pPane->GetTitle();

	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, XTP_DPI_Y(2), XTP_DPI_X(1),
							XTPGetRectDirectionHeight(rc, bVertical) - nTitleHeight - XTP_DPI_Y(3),
							bVertical);

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, 0, XTP_DPI_X(16), 0, bVertical);

	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);

	int nOffset = XTP_DPI_X(4);
	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(4), XTP_DPI_Y(1), XTP_DPI_X(2), 0, bVertical);

	if (!strTitle.IsEmpty())
	{
		pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText
												  : m_clrDisabledCaptionText.operator COLORREF());
		int nWidth = DrawCaptionText(pDC, strTitle, rcCaption, bVertical, TRUE);

		nOffset = nWidth + XTP_DPI_X(10) + (bVertical ? rcCaption.top : rcCaption.left);
	}

	if (NULL == hCustomBrush)
	{
		if (bVertical)
		{
			if (rcCaption.bottom - nOffset > 0)
			{
				int nCenter = rcCaption.CenterPoint().x;
				Line(pDC, nCenter - XTP_DPI_X(3), nOffset, nCenter - XTP_DPI_X(3), rcCaption.bottom,
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
				Line(pDC, nCenter - XTP_DPI_X(1), nOffset, nCenter - XTP_DPI_X(1), rcCaption.bottom,
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
				Line(pDC, nCenter + XTP_DPI_X(1), nOffset, nCenter + XTP_DPI_X(1), rcCaption.bottom,
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
				Line(pDC, nCenter + XTP_DPI_X(3), nOffset, nCenter + XTP_DPI_X(3), rcCaption.bottom,
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			}
		}
		else
		{
			if (rcCaption.right - nOffset > 0)
			{
				int nCenter = rcCaption.CenterPoint().y;
				Line(pDC, nOffset, nCenter - XTP_DPI_Y(3), rcCaption.right, nCenter - XTP_DPI_Y(3),
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
				Line(pDC, nOffset, nCenter - XTP_DPI_Y(1), rcCaption.right, nCenter - XTP_DPI_Y(1),
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
				Line(pDC, nOffset, nCenter + XTP_DPI_Y(1), rcCaption.right, nCenter + XTP_DPI_Y(1),
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
				Line(pDC, nOffset, nCenter + XTP_DPI_Y(3), rcCaption.right, nCenter + XTP_DPI_Y(3),
					 !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			}
		}
	}
}
