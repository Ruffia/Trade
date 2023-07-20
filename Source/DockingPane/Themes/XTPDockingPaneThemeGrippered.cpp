// XTPDockingPaneThemeGrippered.cpp : implementation of the CXTPDockingPaneGripperedTheme class.
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
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneGripperedTheme

CXTPDockingPaneGripperedTheme::CXTPDockingPaneGripperedTheme()
{
	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
}

void CXTPDockingPaneGripperedTheme::RefreshMetrics()
{
	CXTPDockingPaneDefaultTheme::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
}

void CXTPDockingPaneGripperedTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane,
													CRect rcCaption, const CString& strTitle,
													BOOL bActive, BOOL bVertical)
{
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, XTP_DPI_X(2), 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, XTP_DPI_X(14),
					   XTP_DPI_X(2), bVertical);

	int nOffset = 4;
	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(4), XTP_DPI_Y(1), XTP_DPI_X(2), 0, bVertical);

	if (!strTitle.IsEmpty())
	{
		pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText
												  : m_clrDisabledCaptionText.operator COLORREF());

		DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
		int nWidth = DrawCaptionText(pDC, strTitle, rcCaption, bVertical, TRUE);

		nOffset = nWidth + XTP_DPI_X(4) + (bVertical ? rcCaption.top : rcCaption.left);
	}

	if (NULL == hCustomBrush)
	{
		if (bVertical)
		{
			if (rcCaption.bottom > nOffset)
			{
				int nCenter = rcCaption.CenterPoint().x;
				pDC->Draw3dRect(nCenter - XTP_DPI_X(3), nOffset, XTP_DPI_X(3),
								rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT),
								GetXtremeColor(COLOR_3DSHADOW));
				pDC->Draw3dRect(nCenter + XTP_DPI_X(1), nOffset, XTP_DPI_X(3),
								rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT),
								GetXtremeColor(COLOR_3DSHADOW));
			}
		}
		else
		{
			if (rcCaption.right > nOffset)
			{
				int nCenter = rcCaption.CenterPoint().y;
				pDC->Draw3dRect(nOffset, nCenter - XTP_DPI_Y(3), rcCaption.right - nOffset,
								XTP_DPI_Y(3), GetXtremeColor(COLOR_BTNHILIGHT),
								GetXtremeColor(COLOR_3DSHADOW));
				pDC->Draw3dRect(nOffset, nCenter + XTP_DPI_Y(1), rcCaption.right - nOffset,
								XTP_DPI_Y(3), GetXtremeColor(COLOR_BTNHILIGHT),
								GetXtremeColor(COLOR_3DSHADOW));
			}
		}
	}
}

void CXTPDockingPaneGripperedTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane,
													  CRect rc)
{
	CXTPDockingPaneDefaultTheme::DrawFloatingFrame(pDC, pPane, rc);
}

void CXTPDockingPaneGripperedTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
												CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, m_rcCaptionMargin.top, 0,
							XTPGetRectDirectionHeight(rc, bVertical) - nTitleHeight - XTP_DPI_Y(1),
							bVertical);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	if (bVertical)
	{
		pDC->FillSolidRect(rc.left, rc.top, rcCaption.right + XTP_DPI_X(2), rc.Height(),
						   GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + XTP_DPI_Y(2),
						   GetXtremeColor(COLOR_3DFACE));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}
