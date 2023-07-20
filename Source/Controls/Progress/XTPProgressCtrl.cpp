// XTPProgressCtrl.cpp : implementation of the CXTPProgressCtrl class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Progress/XTPProgressCtrlPaintManager.h"
#include "Controls/Progress/XTPProgressCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_TIMER 101

IMPLEMENT_DYNAMIC(CXTPProgressCtrl, CProgressCtrl)

/////////////////////////////////////////////////////////////////////////////
// CXTPScrollBar
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrl::CXTPProgressCtrl()
{
	m_pPaintManager	= NULL;
	m_bPreSubclassInit = TRUE;
	m_nMarqueePos	  = 0;
	m_nMarqueeDelay	= 0;
	m_bFlatStyle	   = FALSE;
}

CXTPProgressCtrl::~CXTPProgressCtrl()
{
	CMDTARGET_RELEASE(m_pPaintManager);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPProgressCtrl, CProgressCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()

	ON_MESSAGE(PBM_SETMARQUEE, OnStartMarquee)
	ON_MESSAGE(PBM_STEPIT, OnStepIt)
	ON_MESSAGE(PBM_SETPOS, OnSetPos)
	ON_WM_TIMER()
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPProgressCtrl::Init()
{
	SetTheme(xtpControlThemeDefault);
	RefreshMetrics();

	ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_CLIENTEDGE, 0);
	SetWindowPos(NULL, 0, 0, 0, 0,
				 SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowText(_T(""));

	m_nMarqueePos = 0;

	SendMessage(WM_NCPAINT);
	Invalidate(FALSE);
}

void CXTPProgressCtrl::PreSubclassWindow()
{
	CProgressCtrl::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPProgressCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CProgressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPProgressCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CProgressCtrl::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPProgressCtrl::RefreshMetrics()
{
	ASSERT(m_pPaintManager);

	if (m_pPaintManager)
	{
		m_pPaintManager->RefreshMetrics();

		if (::IsWindow(m_hWnd))
		{
			Invalidate(FALSE);
		}
	}
}

void CXTPProgressCtrl::DrawNcBorders(CDC* pDC, CRect rc)
{
	if (!m_bFlatStyle)
	{
		ASSERT(m_pPaintManager);

		if (m_pPaintManager && !m_pPaintManager->UsingWinThemes())
		{
			m_pPaintManager->DrawNcBorders(pDC, rc);
		}
	}
}

void CXTPProgressCtrl::OnSysColorChange()
{
	CProgressCtrl::OnSysColorChange();
	RefreshMetrics();
}

void CXTPProgressCtrl::DoPaint(CDC* pDC)
{
	XTPPROGRESSDRAWSTRUCT ds;

	GetClientRect(ds.rcClient);
	GetWindowText(ds.strText);
	SendMessage(PBM_GETRANGE, (WPARAM)FALSE, (LPARAM)&ds.range);

	ds.bSmooth   = (GetStyle() & PBS_SMOOTH) != 0;
	ds.bMarquee  = (GetStyle() & PBS_MARQUEE) != 0;
	ds.bVertical = (GetStyle() & PBS_VERTICAL) != 0;

	ds.pDC		  = pDC;
	ds.pFont	  = CWnd::GetFont();
	ds.nPos		  = ds.bMarquee ? m_nMarqueePos : (int)SendMessage(PBM_GETPOS);
	ds.bVista	 = XTPSystemVersion()->IsWinVistaOrGreater();
	ds.bFlatStyle = m_bFlatStyle;
	ds.pWndCtrl   = this;

	ASSERT(m_pPaintManager);

	if (m_pPaintManager)
	{
		m_pPaintManager->Draw(ds);
	}
}

BOOL CXTPProgressCtrl::HasText()
{
	CString strText;
	GetWindowText(strText);
	return (strText.GetLength() > 0);
}

BOOL CXTPProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	if (m_pPaintManager && m_pPaintManager->UsingWinThemes(TRUE) && !HasText())
	{
		return (BOOL)Default();
	}
	return TRUE;
}

void CXTPProgressCtrl::OnPaint()
{
	if (m_pPaintManager && m_pPaintManager->UsingWinThemes(TRUE) && !HasText())
	{
		Default();
	}
	else
	{
		CPaintDC dcPaint(this);
		CXTPBufferDC dc(dcPaint);
		DoPaint(&dc);
	}
}

LRESULT CXTPProgressCtrl::OnStartMarquee(WPARAM wParam, LPARAM lParam)
{
	m_nMarqueePos   = 0;
	m_nMarqueeDelay = 0;

	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	if (m_pPaintManager && m_pPaintManager->UsingWinThemes(TRUE))
		return Default();

	BOOL bMarquee = wParam && (lParam > 0);
	m_nMarqueePos = 0;

	KillTimer(TID_TIMER);

	if (bMarquee)
	{
		m_nMarqueeDelay = static_cast<int>(lParam);
		SetTimer(TID_TIMER, XTPToUInt(m_nMarqueeDelay), NULL);
	}

	return 1;
}

void CXTPProgressCtrl::DoStep(int nStep)
{
	CXTPClientRect rcClient(this);

	BOOL bVista = XTPSystemVersion()->IsWinVistaOrGreater();
	int dxBlock = (rcClient.Height() - XTP_DPI_X(2)) * 2 / 3;
	int nSpace  = bVista ? 0 : XTP_DPI_X(2);

	m_nMarqueePos += (dxBlock + nSpace) * nStep;

	if (m_nMarqueePos > rcClient.right)
		m_nMarqueePos = 0;

	Invalidate(FALSE);
}

void CXTPProgressCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_TIMER && (GetStyle() & PBS_MARQUEE))
	{
		DoStep(m_nMarqueeDelay > 50 ? 1 : 2);
		return;
	}

	CWnd::OnTimer(nIDEvent);
}

LRESULT CXTPProgressCtrl::OnStepIt(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	if (m_pPaintManager && m_pPaintManager->UsingWinThemes(TRUE))
		return Default();

	DoStep(1);
	return 1;
}

LRESULT CXTPProgressCtrl::OnSetPos(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	Invalidate(FALSE);
	return lResult;
}

void CXTPProgressCtrl::SetUseVisualStyle(BOOL bUseVisualStyle /* = TRUE*/)
{
	if (m_pPaintManager)
	{
		m_pPaintManager->UseWinThemes(bUseVisualStyle);
		RefreshMetrics();
	}
}

void CXTPProgressCtrl::SetTheme(XTPControlTheme nTheme)
{
	switch (nTheme)
	{
		case xtpControlThemeFlat: SetTheme(new CXTPProgressCtrlThemeFlat); break;

		case xtpControlThemeUltraFlat: SetTheme(new CXTPProgressCtrlThemeUltraFlat); break;

		case xtpControlThemeOffice2000: SetTheme(new CXTPProgressCtrlThemeOffice2000); break;

		case xtpControlThemeOfficeXP: SetTheme(new CXTPProgressCtrlThemeOfficeXP); break;

		case xtpControlThemeOffice2003: SetTheme(new CXTPProgressCtrlThemeOffice2003); break;

		case xtpControlThemeResource: SetTheme(new CXTPProgressCtrlThemeResource); break;

		case xtpControlThemeOffice2013: SetTheme(new CXTPProgressCtrlThemeOffice2013); break;

		case xtpControlThemeVisualStudio2015:
			SetTheme(new CXTPProgressCtrlThemeVisualStudio2015);
			break;

		case xtpControlThemeNativeWinXP: SetTheme(new CXTPProgressCtrlThemeNativeWinXP); break;

		case xtpControlThemeNativeWindows10:
			SetTheme(new CXTPProgressCtrlThemeNativeWindows);
			break;

		case xtpControlThemeVisualStudio2017:
			SetTheme(new CXTPProgressCtrlThemeVisualStudio2017);
			break;

		case xtpControlThemeVisualStudio2019:
			SetTheme(new CXTPProgressCtrlThemeVisualStudio2019);
			break;

		case xtpControlThemeVisualStudio2022:
			SetTheme(new CXTPProgressCtrlThemeVisualStudio2022);
			break;

		default: SetTheme(NULL); break;
	}
}

void CXTPProgressCtrl::SetTheme(CXTPProgressCtrlPaintManager* pPaintManager)
{
	CMDTARGET_RELEASE(m_pPaintManager);

	if (pPaintManager == NULL)
	{
		m_pPaintManager = new CXTPProgressCtrlPaintManager();
	}
	else
	{
		m_pPaintManager = pPaintManager;
	}

	RefreshMetrics();
}

LRESULT CXTPProgressCtrl::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

COLORREF CXTPProgressCtrl::SetBorderColor(COLORREF clrNew)
{
	ASSERT(m_pPaintManager);

	if (m_pPaintManager)
	{
		COLORREF clr = m_pPaintManager->SetBorderColor(clrNew);
		Invalidate(FALSE);
		return clr;
	}

	return COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::SetBarColor(COLORREF clrNew)
{
	ASSERT(m_pPaintManager);

	if (m_pPaintManager)
	{
		COLORREF clr = m_pPaintManager->SetBarColor(clrNew);
		Invalidate(FALSE);
		return clr;
	}

	return COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::SetBkColor(COLORREF clrNew)
{
	ASSERT(m_pPaintManager);

	if (m_pPaintManager)
	{
		COLORREF clr = m_pPaintManager->SetBackColor(clrNew);
		Invalidate(FALSE);
		return clr;
	}

	return COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::SetTextColor(COLORREF clrNew)
{
	ASSERT(m_pPaintManager);

	if (m_pPaintManager)
	{
		COLORREF clr = m_pPaintManager->SetTextColor(clrNew);
		Invalidate(FALSE);
		return clr;
	}

	return COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::GetBorderColor()
{
	ASSERT(m_pPaintManager);
	return m_pPaintManager ? m_pPaintManager->GetBorderColor() : COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::GetBarColor()
{
	ASSERT(m_pPaintManager);
	return m_pPaintManager ? m_pPaintManager->GetBarColor() : COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::GetBackColor()
{
	ASSERT(m_pPaintManager);
	return m_pPaintManager ? m_pPaintManager->GetBackColor() : COLORREF_NULL;
}

COLORREF CXTPProgressCtrl::GetTextColor()
{
	ASSERT(m_pPaintManager);
	return m_pPaintManager ? m_pPaintManager->GetTextColor() : COLORREF_NULL;
}

BOOL CXTPProgressCtrl::GetUseVisualStyle() const
{
	ASSERT(m_pPaintManager);
	return (m_pPaintManager && m_pPaintManager->UsingWinThemes());
}
