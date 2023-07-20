// XTPCalendarTip.cpp
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarResource.h"
#include "Calendar/XTPCalendarTheme.h"
#include "Calendar/XTPCalendarThemePrevNextEventButton.h"
#include "Calendar/XTPCalendarThemePrevNextEventButtons.h"
#include "Calendar/XTPCalendarThemeOffice2007.h"
#include "Calendar/XTPCalendarTip.h"
#include "Calendar/XTPCalendarControl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarTip

CXTPCalendarTip::CXTPCalendarTip()
{
	m_pTheme		= NULL;
	m_bAdvancedMode = FALSE;
	m_pParentWnd	= NULL;
	m_pFont			= NULL;
	m_rcHover.SetRect(0, 0, 0, 0);
}

CXTPCalendarTip::~CXTPCalendarTip()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarTip, CWnd)
	//{{AFX_MSG_MAP(CXTPCalendarTip)
	ON_WM_NCHITTEST_EX()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarTip message handlers

LRESULT CXTPCalendarTip::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);

	return HTTRANSPARENT;
}

BOOL CXTPCalendarTip::Create(CWnd* pParentWnd)
{
	ASSERT(pParentWnd);

	// Already created ?
	if (m_hWnd)
		return TRUE;

	m_pParentWnd = pParentWnd;
	return CWnd::CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
						  NULL, WS_POPUP, CXTPEmptyRect(), pParentWnd, NULL);
}

void CXTPCalendarTip::Activate(BOOL bActive, BOOL bAdvanced)
{
	if (bActive)
		m_bAdvancedMode = bAdvanced;

	if (bActive)
	{
		CString strText = m_strTooltipText;
		CWindowDC dc(this);
		CXTPFontDC font(&dc, m_pFont);
		CXTPWindowRect rc(this);
		rc.right = rc.left + dc.GetTextExtent(strText).cx + XTP_DPI_X(6);

		CRect rcWork = XTPMultiMonitor()->GetWorkArea(m_pParentWnd);
		if (rc.right > rcWork.right)
			rc.OffsetRect(rcWork.right - rc.right, 0);

		if (!m_pTheme)
		{
			SetWindowRgn(NULL, FALSE);
		}
		else
		{
			m_pTheme->SetTipWindowRgn(this);
		}

		Invalidate(FALSE);
	}

	ShowWindow(bActive ? SW_SHOWNOACTIVATE : SW_HIDE);
}

BOOL CXTPCalendarTip::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPCalendarTip::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	XTPCalendarTheme eTheme = xtpCalendarThemeUnknown;

	CXTPCalendarControl* pControl = static_cast<CXTPCalendarControl*>(m_pParentWnd);
	if (pControl)
	{
		eTheme = pControl->GetPaintTheme();
	}

	if (eTheme == xtpCalendarThemeOffice2013 || eTheme == xtpCalendarThemeVisualStudio2015
		|| eTheme == xtpCalendarThemeVisualStudio2017 || eTheme == xtpCalendarThemeVisualStudio2019
		|| eTheme == xtpCalendarThemeVisualStudio2022)
	{
		CXTPFontDC font(&dc, m_pFont);

		dc.FillSolidRect(&rc, RGB(0xff, 0xff, 0xff));

		const COLORREF clrBorder = RGB(190, 190, 190);
		dc.Draw3dRect(rc, clrBorder, clrBorder);

		CString str = m_strTooltipText;

		dc.SetBkMode(TRANSPARENT);

		rc.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));

		DWORD dwFlags   = DT_NOPREFIX | DT_EXPANDTABS;
		BOOL bLayoutRTL = GetSafeHwnd()
						  && GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) & WS_EX_LAYOUTRTL;

		if (bLayoutRTL)
		{
			dwFlags |= DT_RTLREADING | DT_RIGHT;
		}

		COLORREF clrTextOld = dc.SetTextColor(RGB(93, 93, 93));

		dc.DrawText(str, rc, dwFlags | DT_WORDBREAK);
		dc.SetTextColor(clrTextOld);
	}
	else if (!m_pTheme)
	{
		COLORREF clrText = GetSysColor(COLOR_INFOTEXT);
		dc.FillSolidRect(rc, GetSysColor(COLOR_INFOBK));
		dc.Draw3dRect(rc, RGB(241, 239, 226), RGB(113, 111, 100));

		dc.SetTextColor(clrText);
		dc.SetBkMode(TRANSPARENT);

		CXTPFontDC font(&dc, m_pFont);
		CRect rcText(rc);
		rcText.left += XTP_DPI_X(3);
		rcText.top += XTP_DPI_Y(3);

		if (m_strTooltipText.GetLength() >= 12 && m_strTooltipText.Mid(11).Find(_T(" ")) > -1)
			dc.DrawText(m_strTooltipText, rcText, DT_WORDBREAK | DT_NOPREFIX);
		else
			dc.DrawText(m_strTooltipText, rcText, DT_NOPREFIX);
	}
	else // Office 2007 theme-similar drawings
	{
		// fill background
		COLORREF clrLight = m_pTheme->GetColor(
			CXTPCalendarThemeOffice2007::xtpCLR_DayViewCellWorkBk);
		COLORREF clrDark = m_pTheme->GetColor(
			CXTPCalendarThemeOffice2007::xtpCLR_DayViewCellNonWorkBk);
		COLORREF clrText = m_bAdvancedMode
							   ? XTPDrawHelpers()->LightenColor(120, m_pTheme->m_clrBaseColor)
							   : RGB(0, 0, 0);
		COLORREF clrBorder = m_pTheme->GetColor(
			CXTPCalendarThemeOffice2007::xtpCLR_DayViewSingleDayEventBorder);

		CXTPPaintManagerColorGradient grclrBk(clrLight, clrDark);

		dc.SetTextColor(clrText);
		dc.SetBkMode(TRANSPARENT);

		m_pTheme->DrawRoundRect(&dc, &rc, clrBorder, FALSE, grclrBk);

		CXTPFontDC font(&dc, m_pTheme->m_fntBaseFont);
		CRect rcText(rc);

		rcText.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));

		if (m_strTooltipText.GetLength() >= 12 && m_strTooltipText.Mid(11).Find(_T(" ")) > -1)
		{
			dc.DrawText(m_strTooltipText, rcText, DT_WORDBREAK | DT_NOPREFIX | DT_VCENTER);
		}
		else
		{
			dc.DrawText(m_strTooltipText, rcText,
						XTPToUInt(DT_NOPREFIX | DT_VCENTER | (m_bAdvancedMode ? DT_CENTER : 0)));
		}
	}
}

CSize CXTPCalendarTip::CalcToolTipRect(BOOL bFull)
{
	CWindowDC dc(NULL);
	CRect rc(0, 0, XTP_DPI_X(146), XTP_DPI_Y(20));

	CXTPFontDC font(&dc, m_pFont);
	if (bFull)
	{
		if (m_strTooltipText.GetLength() >= 12 && m_strTooltipText.Mid(11).Find(_T(" ")) > -1)
		{
			dc.DrawText(m_strTooltipText, rc, DT_CALCRECT | DT_WORDBREAK);
			return CSize(rc.Width() + XTP_DPI_X(15), rc.Height() + XTP_DPI_Y(6));
		}
		else
		{
			dc.DrawText(m_strTooltipText, rc, DT_CALCRECT);
			return CSize(rc.Width() + XTP_DPI_X(15), rc.Height() + XTP_DPI_Y(6));
		}
	}
	else
	{
		dc.DrawText(m_strTooltipText, rc, DT_CALCRECT | DT_WORDBREAK);
		return CSize(XTP_DPI_X(160), rc.Height() + XTP_DPI_Y(6));
	}
}

void CXTPCalendarTip::SetTheme(CXTPCalendarTheme* pTheme)
{
	m_pTheme = pTheme;
}
