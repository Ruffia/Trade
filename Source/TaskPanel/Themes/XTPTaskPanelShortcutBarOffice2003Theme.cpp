// XTPTaskPanelShortcutBarOffice2003Theme.cpp : implementation of the
// CXTPTaskPanelShortcutBarOffice2003Theme class.
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
#include "TaskPanel/Themes/XTPTaskPanelListViewTheme.h"
#include "TaskPanel/Themes/XTPTaskPanelListViewOfficeXPTheme.h"
#include "TaskPanel/Themes/XTPTaskPanelListViewOffice2003Theme.h"
#include "TaskPanel/Themes/XTPTaskPanelShortcutBarOffice2003Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelShortcutBarOffice2003Theme

CXTPTaskPanelShortcutBarOffice2003Theme::CXTPTaskPanelShortcutBarOffice2003Theme()
{
	m_nCaptionTextFormat = DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS;
}

void CXTPTaskPanelShortcutBarOffice2003Theme::RefreshMetrics()
{
	CXTPTaskPanelListViewOffice2003Theme::RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_xtpFontCaption);
	m_nCaptionHeight = max(XTP_DPI_Y(22), dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(3));

	m_groupNormal.clrClient.SetStandardValue(XTPColorManager()->grcCaption);
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_groupNormal.clrHead.SetStandardValue(XTPColorManager()->grcToolBar);
	m_clrBackground.SetStandardValue(XTPColorManager()->grcCaption);

	m_groupSpecial = m_groupNormal;
}

CRect CXTPTaskPanelShortcutBarOffice2003Theme::DrawGroupCaptionBackground(
	CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (pGroup->IsItemHot() || pGroup->IsItemPressed())
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, pGroup->IsItemPressed() ? m_grcPushed : m_grcHot,
									   FALSE);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrHead, FALSE);
	}

	if (pGroup->GetIndex() != 0)
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));

	return rc;
}

void CXTPTaskPanelShortcutBarOffice2003Theme::FillGroupClientFace(CDC* pDC,
																  CXTPTaskPanelGroup* pGroup,
																  CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrClient, FALSE);
}

void CXTPTaskPanelShortcutBarOffice2003Theme::DrawGroupClientFace(CDC* pDC,
																  CXTPTaskPanelGroup* pGroup,
																  CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);

	pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
}

void CXTPTaskPanelShortcutBarOffice2003Theme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	rc.bottom = max(rc.bottom, pTaskPanel->GetClientHeight());

	XTPDrawHelpers()->GradientFill(pDC, rc, m_clrBackground, FALSE);
}
