// XTPDockingPaneThemeOffice2003Outlook.cpp : implementation of the
// CXTPDockingPaneOffice2003OutlookTheme class.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2003Outlook.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2003OutlookTheme

CXTPDockingPaneOffice2003OutlookTheme::CXTPDockingPaneOffice2003OutlookTheme()
{
	m_bDrawGripper	= FALSE;
	m_bRoundedCaption = FALSE;
	m_nCaptionFontGap = XTP_DPI_X(3);
}

CXTPDockingPaneOffice2003OutlookTheme::~CXTPDockingPaneOffice2003OutlookTheme()
{
}

void CXTPDockingPaneOffice2003OutlookTheme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	if (m_bUseStandardFont)
	{
		LOGFONT lf;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));

		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = XTP_DPI_Y(20);

		SetCaptionFontIndirect(&lf);
	}

	if (m_bLunaTheme)
	{
		m_clrNormalCaption.SetStandardValue(XTPColorManager()->grcCaption);
		m_clrNormalCaptionText.SetStandardValue(RGB(255, 255, 255));
	}
	else
	{
		m_clrNormalCaption.SetStandardValue(
			XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DSHADOW),
										  GetXtremeColor(COLOR_WINDOW), 0xcd),
			GetXtremeColor(COLOR_3DSHADOW));
	}
}

CRect CXTPDockingPaneOffice2003OutlookTheme::GetCaptionGripperRect(
	const CXTPDockingPaneTabbedContainer* /*pPane*/)
{
	return CRect(0, 0, 0, 0);
}
