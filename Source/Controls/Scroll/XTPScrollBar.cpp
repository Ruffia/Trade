// XTPScrollBar.cpp : implementation of the CXTPScrollBar class.
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

#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Scroll/XTPScrollBar.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPScrollBar, CXTPScrollBarCtrl)

CXTPScrollBar::CXTPScrollBar()
	: m_nTheme(xtpControlThemeDefault)
	, m_bUseVisualStyle(TRUE)
{
	SetTheme(m_nTheme);
}

CXTPScrollBar::~CXTPScrollBar()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPScrollBar, CXTPScrollBarCtrl)
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPScrollBar::SetCommandBars(void*)
{
	TRACE(_T("WARNING: CXTPScrollBar renamed to CXTPCommandBarScrollBarCtrl\n"));
}

void CXTPScrollBar::SetTheme(XTPControlTheme nTheme)
{
	switch (nTheme)
	{
		case xtpControlThemeDefault:
			CXTPScrollBarCtrl::SetTheme(m_bUseVisualStyle ? xtpScrollBarThemeWindowsDefault
														  : xtpScrollBarThemeWindowsClassic);
			break;

		case xtpControlThemeFlat: CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeFlat); break;

		case xtpControlThemeUltraFlat:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeUltraFlat);
			break;

		case xtpControlThemeOffice2000:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeOffice2000);
			break;

		case xtpControlThemeOfficeXP: CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeOfficeXP); break;

		case xtpControlThemeOffice2003:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeOffice2003);
			break;

		case xtpControlThemeNativeWinXP:
		case xtpControlThemeVisualStudio2005:
		case xtpControlThemeVisualStudio2008:
		case xtpControlThemeVisualStudio2010:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeWindowsDefault);
			break;

		case xtpControlThemeResource: CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeResource); break;

		case xtpControlThemeOffice2013:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeOffice2013);
			break;

		case xtpControlThemeVisualStudio2012:
		case xtpControlThemeVisualStudio2012Light:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2012Light);
			break;

		case xtpControlThemeVisualStudio2012Dark:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2012Dark);
			break;

		case xtpControlThemeVisualStudio2015:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2015);
			break;

		case xtpControlThemeVisualStudio2017:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2017);
			break;

		case xtpControlThemeVisualStudio2019:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2019);
			break;

		case xtpControlThemeVisualStudio2022:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2022);
			break;

		case xtpControlThemeNativeWindows10:
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeNativeWindows10);
			break;

		default: CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeWindowsClassic); break;
	}

	RedrawScrollBar();
}

LRESULT CXTPScrollBar::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

void CXTPScrollBar::SetUseVisualStyle(BOOL bUseVisualStyle)
{
	m_bUseVisualStyle = bUseVisualStyle;
	SetTheme(m_nTheme);
}
