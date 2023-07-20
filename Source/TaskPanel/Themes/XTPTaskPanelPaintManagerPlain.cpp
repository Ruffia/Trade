// XTPTaskPanelPaintManagerPlain.cpp : implementation of the CXTPTaskPanelPaintManagerPlain class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarContainer.h"

#include "TaskPanel/XTPTaskPanelDefines.h"
#include "TaskPanel/XTPTaskPanel.h"
#include "TaskPanel/XTPTaskPanelItem.h"
#include "TaskPanel/XTPTaskPanelGroup.h"
#include "TaskPanel/XTPTaskPanelItems.h"
#include "TaskPanel/XTPTaskPanelPaintManager.h"

#include "TaskPanel/Themes/XTPTaskPanelPaintManagerPlain.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelPaintManagerPlain

CXTPTaskPanelPaintManagerPlain::CXTPTaskPanelPaintManagerPlain()
{
	m_bRoundedFrame  = TRUE;
	m_bOfficeCaption = FALSE;

	m_nCaptionTextFormat = DT_WORDBREAK | DT_LEFT | DT_NOPREFIX;
	m_rcGroupIconPadding.SetRect(0, 0, XTP_DPI_X(3), 0);
	m_eGripper = xtpTaskPanelGripperTriangle;
}

void CXTPTaskPanelPaintManagerPlain::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_BACKGROUND),
									 GetXtremeColor(COLOR_BACKGROUND));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

	m_groupSpecial = m_groupNormal;
}

void CXTPTaskPanelPaintManagerPlain::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup,
														 CRect rc)
{
	pDC->FillSolidRect(rc, GetGroupColors(pGroup)->clrClient);
}

void CXTPTaskPanelPaintManagerPlain::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup,
														 CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);
}

void CXTPTaskPanelPaintManagerPlain::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);

	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, FALSE);

	CXTPTaskPanelItems* pItems = pTaskPanel->GetGroups();
	if (pItems->GetFirstVisibleItem() == NULL)
		return;

	if (m_bRoundedFrame)
	{
		CRect rcFace(((CXTPTaskPanelGroup*)pItems->GetFirstVisibleItem())->GetCaptionRect());
		rcFace.bottom = ((CXTPTaskPanelGroup*)pItems->GetLastVisibleItem())->GetClientRect().bottom;

		rcFace.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(3), XTP_DPI_X(1), XTP_DPI_Y(8));
		CXTPPenDC pen(*pDC, m_groupNormal.clrClientBorder);
		CXTPBrushDC brush(*pDC, m_groupNormal.clrClient);

		pDC->RoundRect(rcFace.left, rcFace.top, rcFace.right, rcFace.bottom, XTP_DPI_X(10),
					   XTP_DPI_Y(10));
	}
}

int CXTPTaskPanelPaintManagerPlain::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup,
													 BOOL bDraw)
{
	CXTPFontDC fnt(pDC, GetItemFont(pGroup));

	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS& groupColors = m_groupNormal;

		if (pGroup->IsCaptionVisible())
		{
			// CRect rcText(rc);
			CRect rcText = DrawCaptionGripper(pDC, pGroup, rc);

			if (!m_bOfficeCaption)
				rcText.DeflateRect(XTP_DPI_X(13), XTP_DPI_Y(5), XTP_DPI_X(13), 0);

			CSize szIcon				 = pGroup->GetTaskPanel()->GetGroupIconSize();
			CXTPImageManagerIcon* pImage = pGroup->GetImage(szIcon.cx);

			if (rcText.Width() > szIcon.cx && pImage)
			{
				if (pImage->IsVectorIcon())
				{
					szIcon.cy = szIcon.cx;
				}
				else
				{
					szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
				}

				CRect rcIconPadding(GetItemIconPadding(pGroup));
				CPoint ptIcon(rcText.left + rcIconPadding.left,
							  min(rcText.CenterPoint().y + szIcon.cy / 2, rcText.bottom) - szIcon.cy
								  - rcIconPadding.bottom + rcIconPadding.top);

				DrawItemImage(pDC, pGroup, ptIcon, pImage, szIcon);

				rcText.left += szIcon.cx + rcIconPadding.right + rcIconPadding.left;
			}

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled()
								  ? GetXtremeColor(COLOR_GRAYTEXT)
								  : groupColors.clrClientText.operator COLORREF());

			DrawItemCaption(pDC, pGroup, rcText, XTPToUInt(m_nCaptionTextFormat));

			if (m_bOfficeCaption)
			{
				pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1,
								   groupColors.clrHead.clrLight);
			}

			if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
			{
				CSize szText = pGroup->GetCaptionTextSize(pDC, rcText,
														  XTPToUInt(m_nCaptionTextFormat));
				DrawFocusRect(pDC, CRect(rcText.TopLeft(), szText));
			}
		}
		return 0;
	}
	else
	{
		if (!pGroup->IsCaptionVisible())
			return 0;

		CRect rcText(pGroup->GetTargetCaptionRect());
		rcText.right -= GetCaptionGripperWidth(pGroup, rcText);

		if (!m_bOfficeCaption)
			rcText.DeflateRect(XTP_DPI_X(13), 0, XTP_DPI_Y(13), 0);

		rcText.top = rcText.bottom = 0;

		CSize szIcon				 = pGroup->GetTaskPanel()->GetGroupIconSize();
		CXTPImageManagerIcon* pImage = pGroup->GetImage(szIcon.cx);

		if (pImage)
		{
			CRect rcIconPadding(GetItemIconPadding(pGroup));
			rcText.left += szIcon.cx + rcIconPadding.right + rcIconPadding.left;
		}

		CSize szText = pGroup->GetCaptionTextSize(pDC, rcText, XTPToUInt(m_nCaptionTextFormat));
		return szText.cy + XTP_DPI_Y(5);
	}
}
