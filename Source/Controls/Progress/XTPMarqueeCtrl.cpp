// XTPMarqueeCtrl.cpp : implementation file
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Progress/XTPProgressCtrlPaintManager.h"
#include "Controls/Progress/XTPMarqueeCtrlPaintManager.h"
#include "Controls/Progress/XTPMarqueeCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef PBS_MARQUEE
#	define PBS_MARQUEE 0x08
#endif

#define PERCENT2BYTE(i) (BYTE)((i * 255) / 100)

CXTPMarqueeCtrl::CXTPMarqueeCtrl()
	: m_nGap(0)
	, m_nPos(0)
	, m_cxChunk(XTP_DPI_X(12))
	, m_pPaintManager(new CXTPMarqueeCtrlPaintManager())
{
	m_chAlpha[0] = PERCENT2BYTE(25);
	m_chAlpha[1] = PERCENT2BYTE(50);
	m_chAlpha[2] = PERCENT2BYTE(75);
	m_chAlpha[3] = PERCENT2BYTE(100);
	m_chAlpha[4] = PERCENT2BYTE(100);
}

CXTPMarqueeCtrl::~CXTPMarqueeCtrl()
{
	CMDTARGET_RELEASE(m_pPaintManager);
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrl, CProgressCtrl)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPMarqueeCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CXTPMarqueeCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

// CXTPMarqueeCtrl auto-initialization
//---------------------------------------------------------------------------

BOOL CXTPMarqueeCtrl::OnInitControl()
{
	if (::IsWindow(m_hWnd))
	{
		// initialize the progress bar.
		SetMarquee();
		return TRUE;
	}

	return FALSE;
}

void CXTPMarqueeCtrl::PreSubclassWindow()
{
	CProgressCtrl::PreSubclassWindow();

	if (m_bIsSubclassed)
	{
		// Initialize the control.
		OnInitControl();
	}
}

int CXTPMarqueeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CProgressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	OnInitControl();

	return 0;
}

BOOL CXTPMarqueeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CProgressCtrl::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically InitControl() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bIsSubclassed = FALSE;

	return TRUE;
}

void CXTPMarqueeCtrl::RefreshMetrics()
{
	m_pPaintManager->RefreshMetrics();
}

// CXTPMarqueeCtrl drawing.
//---------------------------------------------------------------------------

void CXTPMarqueeCtrl::OnPaint()
{
	if (IsMarquee())
	{
		CPaintDC dc(this);
		CXTPBufferDC dcMem(dc);
		CXTPClientRect rcClient(this);

		m_pPaintManager->DrawBackground(this, &dcMem, rcClient);
		m_pPaintManager->DrawMarquee(this, &dcMem, rcClient);
	}
	else
	{
		Default();
	}
}

BOOL CXTPMarqueeCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (IsMarquee())
	{
		return TRUE;
	}
	else
	{
		return CProgressCtrl::OnEraseBkgnd(pDC);
	}
}

// CXTPMarqueeCtrl message handling.
//---------------------------------------------------------------------------

BOOL CXTPMarqueeCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
		case WM_SETTINGCHANGE:
		case WM_SYSCOLORCHANGE:
		{
			RefreshMetrics();
			RedrawWindow();
		}
		break;
	}

	return CProgressCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPMarqueeCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (IsMarquee())
	{
		if (nIDEvent == 1)
		{
			CXTPClientRect rcClient(this);

			// calculate current position.
			if (IsVertical())
			{
				int cy = m_cxChunk;
				m_nPos += cy;

				if (m_nPos >= (rcClient.bottom - cy))
					m_nPos = -(((cy * 5) - 2) + cy);
			}
			else
			{
				int cx = m_cxChunk;
				m_nPos += cx;

				if (m_nPos >= (rcClient.right - cx))
					m_nPos = -(((cx * 5) - 2) + cx);
			}

			RedrawWindow();
		}
	}
	else
	{
		CProgressCtrl::OnTimer(nIDEvent);
	}
}

BOOL CXTPMarqueeCtrl::UseWinThemes()
{
	return m_pPaintManager->UsingWinThemes();
}

BOOL CXTPMarqueeCtrl::IsSmooth()
{
	return (GetStyle() & PBS_SMOOTH) != 0;
}

BOOL CXTPMarqueeCtrl::IsVertical()
{
	return (GetStyle() & PBS_VERTICAL) != 0;
}

BOOL CXTPMarqueeCtrl::IsMarquee()
{
	return (GetStyle() & PBS_MARQUEE) != 0;
}

void CXTPMarqueeCtrl::SetColors(COLORREF crBack, COLORREF crChunk)
{
	m_pPaintManager->SetBackColor(crBack);
	m_pPaintManager->SetBarColor(crChunk);
}

void CXTPMarqueeCtrl::SetMarquee(BOOL bMarqueeMode /*=TRUE*/, int nInterval /*=50*/,
								 int nGap /*=2*/, int cx /*=12*/, int* piTrans /*=NULL*/)
{
	if (bMarqueeMode)
	{
		// set transparency.
		if (piTrans)
		{
			for (int i = 0; i < _countof(m_chAlpha); ++i)
			{
				m_chAlpha[i] = PERCENT2BYTE(piTrans[i]);
			}
		}

		// set the progress chunk size.
		m_cxChunk = cx;

		// set chunk gap and create bitmaps.
		m_nGap = nGap;
		RefreshMetrics();

		// set the marquee style and start timer.
		ModifyStyle(0, PBS_MARQUEE);
		SetTimer(1, XTPToUInt(nInterval), NULL);
	}
	else
	{
		// stop the timer and remove marquee style.
		KillTimer(1);
		ModifyStyle(PBS_MARQUEE, 0);
	}

	RedrawWindow();
}

void CXTPMarqueeCtrl::SetTheme(XTPControlTheme nTheme)
{
	switch (nTheme)
	{
		case xtpControlThemeFlat: SetTheme(new CXTPMarqueeCtrlThemeFlat); break;

		case xtpControlThemeUltraFlat: SetTheme(new CXTPMarqueeCtrlThemeUltraFlat); break;

		case xtpControlThemeOffice2000: SetTheme(new CXTPMarqueeCtrlThemeOffice2000); break;

		case xtpControlThemeOfficeXP: SetTheme(new CXTPMarqueeCtrlThemeOfficeXP); break;

		case xtpControlThemeOffice2003: SetTheme(new CXTPMarqueeCtrlThemeOffice2003); break;

		case xtpControlThemeResource: SetTheme(new CXTPMarqueeCtrlThemeResource); break;

		case xtpControlThemeOffice2013: SetTheme(new CXTPMarqueeCtrlThemeOffice2013); break;

		case xtpControlThemeVisualStudio2015:
			SetTheme(new CXTPMarqueeCtrlThemeVisualStudio2015);
			break;

		case xtpControlThemeNativeWinXP: SetTheme(new CXTPMarqueeCtrlThemeNativeWinXP); break;

		case xtpControlThemeNativeWindows10: SetTheme(new CXTPMarqueeCtrlThemeNativeWindows); break;

		default: SetTheme(NULL); break;
	}
}

void CXTPMarqueeCtrl::SetTheme(CXTPMarqueeCtrlPaintManager* pPaintManager)
{
	CMDTARGET_RELEASE(m_pPaintManager);

	if (pPaintManager == NULL)
	{
		m_pPaintManager = new CXTPMarqueeCtrlPaintManager();
	}
	else
	{
		m_pPaintManager = pPaintManager;
	}

	RefreshMetrics();
}

LRESULT CXTPMarqueeCtrl::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}
