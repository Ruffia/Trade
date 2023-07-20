// XTPTaskPanelVisualStudio2005Theme.cpp : implementation of the CXTPTaskPanelVisualStudio2005Theme
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarContainer.h"

#include "TaskPanel/XTPTaskPanelDefines.h"
#include "TaskPanel/XTPTaskPanel.h"
#include "TaskPanel/XTPTaskPanelItem.h"
#include "TaskPanel/XTPTaskPanelGroup.h"
#include "TaskPanel/XTPTaskPanelPaintManager.h"

#include "TaskPanel/Themes/XTPTaskPanelPaintManagerPlain.h"
#include "TaskPanel/Themes/XTPTaskPanelToolboxTheme.h"
#include "TaskPanel/Themes/XTPTaskPanelVisualStudio2005Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelVisualStudio2005Theme

CXTPTaskPanelVisualStudio2005Theme::CXTPTaskPanelVisualStudio2005Theme()
{
	m_bOfficeHighlight	 = TRUE;
	m_bBoldCaption		   = TRUE;
	m_bInvertDragRect	  = FALSE;
	m_bCaptionScrollButton = FALSE;
}

void CXTPTaskPanelVisualStudio2005Theme::RefreshMetrics()
{
	CXTPTaskPanelToolboxTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
		case xtpSystemThemeBlue:
			m_groupNormal.clrHead.SetStandardValue(RGB(222, 220, 203), RGB(196, 193, 176));
			m_clrBackground.SetStandardValue(RGB(243, 241, 230), RGB(228, 225, 208));
			break;

		case xtpSystemThemeOlive:
			m_groupNormal.clrHead.SetStandardValue(RGB(223, 228, 209), RGB(222, 227, 206));
			m_clrBackground.SetStandardValue(RGB(243, 241, 230), RGB(228, 225, 208));
			m_clrHighlightHot.SetStandardValue(RGB(182, 198, 141));
			m_clrHighlightPressed.SetStandardValue(RGB(147, 160, 112));
			m_clrHighlightSelected.SetStandardValue(RGB(194, 207, 158));
			break;

		case xtpSystemThemeSilver:
			m_groupNormal.clrHead.SetStandardValue(RGB(233, 232, 237), RGB(223, 224, 227));
			m_clrBackground.SetStandardValue(RGB(234, 233, 238), RGB(215, 213, 218));
			m_clrHighlightHot.SetStandardValue(RGB(255, 238, 194));
			m_clrHighlightPressed.SetStandardValue(RGB(254, 128, 62));
			m_clrHighlightSelected.SetStandardValue(RGB(255, 192, 111));
			m_clrHighlightBorder.SetStandardValue(RGB(75, 75, 111));
			break;

		default:
			m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW),
												   GetXtremeColor(COLOR_3DFACE));
	}

	m_groupNormal.clrClient.SetStandardValue(m_clrBackground);

	m_groupSpecial = m_groupNormal;
}

CRect CXTPTaskPanelVisualStudio2005Theme::DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup,
														   CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);
	COLORREF clrFace						= pGroupColors->clrHead.clrLight;

	const int n1pxX = XTP_DPI_X(1);
	const int n1pxY = XTP_DPI_Y(1);
	const int n9pxX = XTP_DPI_X(9);
	const int n9pxY = XTP_DPI_Y(9);

	CPoint pt(rc.left + XTP_DPI_X(6), rc.CenterPoint().y - XTP_DPI_Y(6));
	CRect rcButton(pt.x, pt.y, pt.x + n9pxX, pt.y + n9pxY);

	COLORREF clrButton = clrFace;
	COLORREF clrFrame  = GetXtremeColor(COLOR_3DSHADOW);

	pDC->FillSolidRect(rcButton, clrButton);
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top + n1pxY, n9pxX - n1pxX * 2,
					   n9pxY - n1pxY * 2, GetXtremeColor(COLOR_3DFACE));

	// borders
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top, n9pxX - n1pxX * 2, n1pxY, clrFrame);
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.bottom - n1pxY, n9pxX - n1pxX * 2, n1pxY,
					   clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + n1pxY, n1pxX, n9pxY - n1pxY * 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - n1pxX, rcButton.top + n1pxY, n1pxX, n9pxY - n1pxY * 2,
					   clrFrame);

	// white blink
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top + n1pxY, n9pxX - n1pxX * 2, XTP_DPI_Y(3),
					   0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + n1pxX, rcButton.top + XTP_DPI_Y(4), n9pxX - XTP_DPI_X(4),
					   n1pxY * 2, 0xFFFFFF);

	// plus/minus
	pDC->FillSolidRect(rcButton.left + n1pxX * 2, rcButton.top + XTP_DPI_Y(4), n9pxX - n1pxX * 4,
					   n1pxY, 0);
	if (!pGroup->IsExpanded())
	{
		pDC->FillSolidRect(rcButton.left + XTP_DPI_X(4), rcButton.top + n1pxY * 2, n1pxX,
						   n9pxY - n1pxY * 4, 0);
	}

	CRect rcText(rc);
	rcText.DeflateRect(XTP_DPI_X(19), 0, 0, XTP_DPI_Y(2));

	return rcText;
}

CRect CXTPTaskPanelVisualStudio2005Theme::DrawGroupCaptionBackground(CDC* pDC,
																	 CXTPTaskPanelGroup* pGroup,
																	 CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);
	COLORREF clrFace						= pGroupColors->clrHead.clrLight;

	if (pGroup->IsItemFocused())
	{
		if (!pGroup->IsItemHot() && !pGroup->IsItemPressed())
			Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightSelected);
		else if (pGroup->IsItemHot() && !pGroup->IsItemPressed())
			Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pGroup->IsItemHot() && pGroup->IsItemPressed())
			Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pGroup->IsItemHot() || pGroup->IsItemPressed())
			Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightHot);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC,
									   CRect(rc.left, rc.top, rc.right, rc.bottom - XTP_DPI_Y(2)),
									   pGroupColors->clrHead, FALSE);

		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1), clrFace);
	}

	return DrawExpandButton(pDC, pGroup, rc);
}

void CXTPTaskPanelVisualStudio2005Theme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, TRUE);
}

void CXTPTaskPanelVisualStudio2005Theme::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup,
															 CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrClient, TRUE);
}

void CXTPTaskPanelVisualStudio2005Theme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup,
															 CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);
}
