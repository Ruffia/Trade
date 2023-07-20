// XTPTaskPanelListViewOfficeXPTheme.cpp : implementation of the CXTPTaskPanelListViewOfficeXPTheme
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

#include "TaskPanel/XTPTaskPanelDefines.h"
#include "TaskPanel/XTPTaskPanelItem.h"
#include "TaskPanel/XTPTaskPanelPaintManager.h"

#include "TaskPanel/Themes/XTPTaskPanelPaintManagerPlain.h"
#include "TaskPanel/Themes/XTPTaskPanelToolboxTheme.h"
#include "TaskPanel/Themes/XTPTaskPanelListViewTheme.h"
#include "TaskPanel/Themes/XTPTaskPanelListViewOfficeXPTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelListViewOfficeXPTheme

CXTPTaskPanelListViewOfficeXPTheme::CXTPTaskPanelListViewOfficeXPTheme()
{
	m_bOfficeHighlight		= TRUE;
	m_bEmbossedDisabledText = FALSE;
}

void CXTPTaskPanelListViewOfficeXPTheme::RefreshMetrics()
{
	CXTPTaskPanelListViewTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_nCaptionHeight = max(XTP_DPI_Y(20), m_nCaptionHeight);
	m_groupSpecial   = m_groupNormal;
}
