// XTPTaskPanelToolboxTheme.cpp : implementation of the CXTPTaskPanelToolboxTheme class.
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
#include "TaskPanel/Themes/XTPTaskPanelToolboxTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelToolboxTheme

CXTPTaskPanelToolboxTheme::CXTPTaskPanelToolboxTheme()
{
	m_bBoldCaption		   = FALSE;
	m_bRoundedFrame		   = FALSE;
	m_bOfficeHighlight	 = FALSE;
	m_bInvertDragRect	  = TRUE;
	m_bCaptionScrollButton = TRUE;
	m_eGripper			   = xtpTaskPanelGripperNone;

	m_rcGroupInnerMargins = CRect(0, XTP_DPI_Y(4), 0, XTP_DPI_Y(2));
	m_rcControlMargins	= XTP_DPI(CRect(2, 2, 2, 2));
	m_nGroupSpacing		  = 1;
	m_rcGroupIconPadding  = XTP_DPI(CRect(2, 2, 0, 2));

	m_nCaptionTextFormat = DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS;
}

void CXTPTaskPanelToolboxTheme::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_groupSpecial = m_groupNormal;

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_xtpFontCaption);
	m_nCaptionHeight = max(XTP_DPI_Y(18), dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(5));

	if (!m_bOfficeHighlight)
	{
		COLORREF clr3DFace		 = GetXtremeColor(COLOR_3DFACE);
		COLORREF clr3DHightlight = GetXtremeColor(COLOR_3DHIGHLIGHT);

		int r = GetRValue(clr3DFace), g = GetGValue(clr3DFace), b = GetBValue(clr3DFace);
		int nDelta = max(max(r, g), b);
		if (r < nDelta)
			nDelta = min(min(r, g), b);

		m_clrHighlightHot.SetStandardValue(RGB((r + GetRValue(clr3DHightlight)) / 2,
											   (2 * g - nDelta + GetGValue(clr3DHightlight)) / 2,
											   (2 * b - nDelta + GetBValue(clr3DHightlight)) / 2));
		m_clrHighlightBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW),
											  GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
}

CRect CXTPTaskPanelToolboxTheme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup,
															CRect rc)
{
	COLORREF clrFace = GetGroupColors(pGroup)->clrHead.clrDark;

	CRect rcText(rc);
	pDC->FillSolidRect(rc, clrFace);

	if (pGroup->IsItemHot() && pGroup->IsItemPressed())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
		rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), clrFace);
		rcText.OffsetRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	}
	else
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
	}
	return rcText;
}

int CXTPTaskPanelToolboxTheme::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw)
{
	CXTPFontDC fnt(pDC, GetItemFont(pGroup));

	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

		if (pGroup->IsCaptionVisible())
		{
			CRect rcText = DrawGroupCaptionBackground(pDC, pGroup, rc);

			rcText = DrawCaptionGripper(pDC, pGroup, rcText);

			CSize szIcon				 = pGroup->GetTaskPanel()->GetGroupIconSize();
			CXTPImageManagerIcon* pImage = pGroup->GetImage(szIcon.cx);

			if (rcText.Width() > szIcon.cx && pImage)
			{
				if(pImage->IsVectorIcon())
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

			rcText.DeflateRect(XTP_DPI_X(3), 0);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled()
								  ? GetXtremeColor(COLOR_GRAYTEXT)
								  : pGroupColors->clrHeadTextNormal.operator COLORREF());
			DrawItemCaption(pDC, pGroup, rcText, XTPToUInt(m_nCaptionTextFormat));

			if (pGroup->IsItemDragging())
			{
				pDC->InvertRect(rc);
			}

			if (pGroup->IsItemDragOver()
				&& (m_bInvertDragRect
					|| !(pGroup->IsExpanded() && pGroup->GetItems()->GetFirstVisibleItem() != NULL)))
			{
				DrawDragArrow(pDC, pGroup, rc, TRUE);
			}

			if (pGroup->IsChildItemDragOver() && m_bInvertDragRect)
			{
				pDC->InvertRect(rc);
			}

			if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
			{
				rcText.DeflateRect(-XTP_DPI_X(1), XTP_DPI_Y(2));
				DrawFocusRect(pDC, rcText);
			}
		}
		return 0;
	}
	else
	{
		if (!pGroup->IsCaptionVisible())
			return 0;

		int nIconSize = 0;

		CSize szIcon				 = pGroup->GetTaskPanel()->GetGroupIconSize();
		CXTPImageManagerIcon* pImage = pGroup->GetImage(szIcon.cx);

		if (pImage)
		{
			CRect rcIconPadding(GetItemIconPadding(pGroup));
			if(pImage->IsVectorIcon())
			{
				nIconSize = szIcon.cx + rcIconPadding.top + rcIconPadding.bottom;
			}
			else
			{
				nIconSize = pImage->GetHeight() * szIcon.cx / pImage->GetWidth() + rcIconPadding.top
							+ rcIconPadding.bottom;
			}

		}

		return max(m_nCaptionHeight, nIconSize);
	}
}
