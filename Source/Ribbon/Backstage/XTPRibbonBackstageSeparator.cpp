// XTPRibbonBackstageSeparator.cpp : implementation file
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
#include "Common/XTPMarkupRender.h"

#include "Controls/Util/XTPControlTheme.h"

#include "Ribbon/Backstage/XTPRibbonBackstageSeparator.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorPaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeResource.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeOffice2013.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeVisualStudio2015.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeVisualStudio2017.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeVisualStudio2019.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeVisualStudio2022.h"
#include "Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparator

IMPLEMENT_DYNAMIC(CXTPRibbonBackstageSeparator, CXTPRibbonBackstageSeparatorBase);

CXTPRibbonBackstageSeparator::CXTPRibbonBackstageSeparator()
	: m_pTheme(NULL)
	, m_bVerticalStyle(FALSE)
	, m_pMarkupContext(NULL)
	, m_pMarkupUIElement(NULL)
{
	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);

	SetTheme(xtpControlThemeResource);
}

CXTPRibbonBackstageSeparator::~CXTPRibbonBackstageSeparator()
{
	XTPMarkupReleaseContext(m_pMarkupContext);
	XTPMarkupReleaseElement(m_pMarkupUIElement);

	SAFE_DELETE(m_pTheme);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonBackstageSeparator, CXTPRibbonBackstageSeparatorBase)
	//{{AFX_MSG_MAP(CXTPRibbonBackstageSeparator)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPRibbonBackstageSeparator::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparator message handlers

void CXTPRibbonBackstageSeparator::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPBufferDC dc(dcPaint);

	OnDraw(&dc);
}

BOOL CXTPRibbonBackstageSeparator::SetTheme(XTPControlTheme theme)
{
	SAFE_DELETE(m_pTheme);

	switch (theme)
	{
		case xtpControlThemeResource:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeResource();
			break;
		case xtpControlThemeOffice2013:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeOffice2013();
			break;
		case xtpControlThemeVisualStudio2015:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeVisualStudio2015();
			break;
		case xtpControlThemeVisualStudio2017:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeVisualStudio2017();
			break;
		case xtpControlThemeVisualStudio2019:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeVisualStudio2019();
			break;
		case xtpControlThemeVisualStudio2022:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeVisualStudio2022();
			break;
		case xtpControlThemeNativeWindows10:
			m_pTheme = new CXTPRibbonBackstageSeparatorThemeNativeWindows10();
			break;
		default: break;
	}

	return TRUE;
}

void CXTPRibbonBackstageSeparator::OnDraw(CDC* pDC)
{
	CXTPClientRect rc(this);

	HBRUSH hBrush = NULL;

#ifndef _XTP_COMMANDBARS_ACTIVEX
	hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC, WPARAM(pDC->GetSafeHdc()),
											  LPARAM(GetSafeHwnd()));
#endif

	if (hBrush)
	{
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);
	}
	else
	{
		COLORREF clrBack(m_pTheme == NULL ? RGB(255, 255, 255) : m_pTheme->GetColorBack());
		pDC->FillSolidRect(rc, clrBack);
	}

	if (m_pMarkupUIElement)
	{
		XTPMarkupRenderElement(m_pMarkupUIElement, pDC->GetSafeHdc(), rc);
		return;
	}

	if (NULL != m_pTheme)
	{
		m_pTheme->DrawSeparator(pDC, this);
	}
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
