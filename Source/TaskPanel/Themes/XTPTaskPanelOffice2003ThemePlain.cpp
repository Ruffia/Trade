// XTPTaskPanelOffice2003ThemePlain.cpp : implementation of the CXTPTaskPanelOffice2003ThemePlain
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
#include "TaskPanel/Themes/XTPTaskPanelOfficeXPThemePlain.h"
#include "TaskPanel/Themes/XTPTaskPanelOffice2003ThemePlain.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelOffice2003ThemePlain

CXTPTaskPanelOffice2003ThemePlain::CXTPTaskPanelOffice2003ThemePlain()
{
	m_bOfficeHighlight = XTP_TASKPANEL_HIGHLIGHT_OFFICE2003;
	m_rcControlMargins = XTP_DPI(CRect(8, 8, 0, 8));
	m_nGroupSpacing	= XTP_DPI_X(15);
	m_eGripper		   = xtpTaskPanelGripperNone;
}

void CXTPTaskPanelOffice2003ThemePlain::RefreshMetrics()
{
	CXTPTaskPanelOfficeXPThemePlain::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	COLORREF clrBackground;
	COLORREF clr3DShadow;

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			clrBackground = RGB(221, 236, 254);
			clr3DShadow   = RGB(123, 164, 224);
			break;

		case xtpSystemThemeOlive:
			clrBackground = RGB(243, 242, 231);
			clr3DShadow   = RGB(188, 187, 177);
			break;

		case xtpSystemThemeSilver:
			clrBackground = RGB(238, 238, 244);
			clr3DShadow   = RGB(161, 160, 187);
			break;

		default:
			clrBackground = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
														  GetXtremeColor(COLOR_WINDOW), 50);
			clr3DShadow   = GetXtremeColor(COLOR_3DSHADOW);
	}

	m_clrBackground.SetStandardValue(clrBackground, clrBackground);

	m_groupNormal.clrClient.SetStandardValue(clrBackground);
	m_groupNormal.clrHead.SetStandardValue(clr3DShadow, clr3DShadow);
	m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 0, 0xFF));
	m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 0, 0xFF));

	m_groupSpecial = m_groupNormal;
}
