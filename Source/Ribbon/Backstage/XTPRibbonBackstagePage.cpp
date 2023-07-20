// XTPRibbonBackstagePage.cpp : implementation file
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPVC80Helpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizePropertyPage.h"

#include "Ribbon/Backstage/XTPRibbonBackstagePage.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePage

IMPLEMENT_DYNAMIC(CXTPRibbonBackstagePage, CXTPResizePropertyPage)

CXTPRibbonBackstagePage::CXTPRibbonBackstagePage(UINT nID)
	: CXTPResizePropertyPage(nID)
	, m_nTheme(xtpControlThemeDefault)
	, m_clrBack(::GetSysColor(COLOR_WINDOW))
	, m_clrText(::GetSysColor(COLOR_WINDOWTEXT))
	, m_bRightToLeft(FALSE)
{
	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);

	if (XTPDrawHelpers()->FontExists(_T("Segoe UI"))) // Primary font
	{
		lf.lfHeight = XTP_DPI_Y(30);
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Segoe UI"));
	}
	else if (XTPDrawHelpers()->FontExists(_T("Tahoma"))) // Fall-back font for XP
	{
		lf.lfHeight = XTP_DPI_Y(30);
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));
	}
	else
	{
		lf.lfHeight = XTP_DPI_Y(25);
	}
	m_xtpFontTitle.CreateFontIndirect(&lf);

	lf.lfHeight = XTP_DPI_Y(18);
	lf.lfWeight = FW_BOLD;

	m_xtpFontCaption.CreateFontIndirect(&lf);
}

CXTPRibbonBackstagePage::~CXTPRibbonBackstagePage()
{
	if (m_xtpBrushBack.GetSafeHandle())
		m_xtpBrushBack.DeleteObject();
}

XTPControlTheme CXTPRibbonBackstagePage::GetTheme() const
{
	return m_nTheme;
}

void CXTPRibbonBackstagePage::SetTheme(const XTPControlTheme nTheme)
{
	m_nTheme = nTheme;

	// get the background color from the selected theme if available.
	m_clrText = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageText"),
							::GetSysColor(COLOR_WINDOWTEXT));
	m_clrBack = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageBackground"),
							::GetSysColor(COLOR_WINDOW));

	// destroy the previous brush.
	if (m_xtpBrushBack.GetSafeHandle())
		m_xtpBrushBack.DeleteObject();

	// create a new brush.
	m_xtpBrushBack.CreateSolidBrush(m_clrBack);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonBackstagePage, CXTPResizePropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

HBRUSH CXTPRibbonBackstagePage::OnCtlColor(CDC* pDC, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBack);

	return (HBRUSH)m_xtpBrushBack;
}

void CXTPRibbonBackstagePage::PreSubclassWindow()
{
	ModifyStyleEx(XTPToDWORD(IsRightToLeft() ? 0 : WS_EX_LAYOUTRTL),
				  XTPToDWORD(!IsRightToLeft() ? 0 : WS_EX_LAYOUTRTL));

	CXTPResizePropertyPage::PreSubclassWindow();
}
