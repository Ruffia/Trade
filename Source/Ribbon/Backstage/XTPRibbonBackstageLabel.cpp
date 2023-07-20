// XTPRibbonBackstageLabel.cpp : implementation file
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
#include "Common/XTPResourceImage.h"

#include "Controls/Util/XTPControlTheme.h"

#include "Ribbon/Backstage/XTPRibbonBackstageLabel.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageLabel

CXTPRibbonBackstageLabel::CXTPRibbonBackstageLabel()
{
	RefreshMetrics();
}

CXTPRibbonBackstageLabel::~CXTPRibbonBackstageLabel()
{
}

BEGIN_MESSAGE_MAP(CXTPRibbonBackstageLabel, CStatic)
	//{{AFX_MSG_MAP(CXTPRibbonBackstageLabel)
	ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageLabel message handlers

void CXTPRibbonBackstageLabel::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint);

	dc.FillSolidRect(rc, m_clrBack);

	CXTPFontDC font(&dc, GetFont());

	CString strText;
	GetWindowText(strText);

	dc.SetTextColor(m_clrText);

	dc.DrawText(strText, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
}

void CXTPRibbonBackstageLabel::SetTheme(XTPControlTheme /*nTheme*/)
{
	RefreshMetrics();
}

void CXTPRibbonBackstageLabel::RefreshMetrics()
{
	m_clrText = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageText"), RGB(94, 94, 94));
	m_clrBack = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageBackground"),
							RGB(255, 255, 255));
}
