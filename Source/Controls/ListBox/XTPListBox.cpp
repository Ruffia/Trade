// XTPListBox.cpp : implementation of the CXTPListBox class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/ListBox/XTPListBox.h"

#include "Controls/ListBox/Themes/XTPListBoxThemeOfficeXP.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeOffice2013.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeVisualStudio2015.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeVisualStudio2017.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeVisualStudio2019.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeVisualStudio2022.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeNativeWindows10.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeResource.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeDefault.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPListBoxTheme
/////////////////////////////////////////////////////////////////////////////

void CXTPListBoxTheme::DrawItemText(CDC* pDC, CRect rcText, CXTPListBox* pListBox,
									LPDRAWITEMSTRUCT lpDIS)
{
	CString strText;
	pListBox->GetText(static_cast<int>(lpDIS->itemID), strText);

	pDC->DrawText(strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
}

COLORREF CXTPListBoxTheme::GetBackColor()
{
	return GetXtremeColor(COLOR_WINDOW);
}

BOOL CXTPListBoxTheme::DrawNcBorders(CDC* pDC, CRect rc)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPListBox
/////////////////////////////////////////////////////////////////////////////

CXTPListBox::CXTPListBox()
	: m_bPreSubclassInit(TRUE)
	, m_pTheme(NULL)
	, m_theme(xtpControlThemeDefault)
{
	m_nItemHeight  = 0;
	m_nTextPadding = 2;
	m_nHotItem	 = -1;

	SetTheme(xtpControlThemeDefault);
}

CXTPListBox::~CXTPListBox()
{
	SAFE_DELETE(m_pTheme);
}

IMPLEMENT_DYNAMIC(CXTPListBox, CListBox)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPListBox, CListBox)
	//{{AFX_MSG_MAP(CXTPListBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPListBox::Initialize(bool bAutoFont /*= true*/)
{
	// set the font for the list box.
	if (bAutoFont)
	{
		SetFont(&XTPAuxData().xtpFont);
	}
}

void CXTPListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (NULL != m_pTheme)
		m_pTheme->DrawItem(pDC, this, lpDIS);
}

void CXTPListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight = (m_nItemHeight > 0 ? XTPToUIntChecked(m_nItemHeight)
										   : static_cast<UINT>(::GetSystemMetrics(SM_CYMENU) - 2));
}

BOOL CXTPListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

COLORREF CXTPListBox::GetBackColor()
{
	return m_pTheme ? m_pTheme->GetBackColor() : GetXtremeColor(COLOR_WINDOW);
}

void CXTPListBox::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// Paint to a memory device context to reduce screen flicker.
	CXTPBufferDC memDC(dc, r);
	memDC.FillSolidRect(r, GetBackColor());

	CPoint ptOrg = memDC.GetWindowOrg();

	// Let the window do its default painting   ...
	CListBox::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
	memDC.SetWindowOrg(ptOrg);
}

void CXTPListBox::OnNcPaint()
{
	CListBox::OnNcPaint();

	if (m_pTheme)
	{
		CWindowDC dc(this);

		CXTPWindowRect rc(this);
		rc.OffsetRect(-rc.TopLeft());
		m_pTheme->DrawNcBorders(&dc, rc);
	}
}

LRESULT CXTPListBox::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	CListBox::DefWindowProc(WM_ERASEBKGND, wParam, 0);
	return CListBox::DefWindowProc(WM_PRINTCLIENT, wParam, lParam);
}

void CXTPListBox::Init()
{
	// make sure the style is set to LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS.
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if ((lStyle & LBS_OWNERDRAWVARIABLE) == 0)
		lStyle |= LBS_OWNERDRAWVARIABLE;

	if ((lStyle & LBS_HASSTRINGS) == 0)
		lStyle |= LBS_HASSTRINGS;

	::SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
}

void CXTPListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();

	if (m_bPreSubclassInit)
		Init();
}

BOOL CXTPListBox::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CListBox::PreCreateWindow(cs))
		return FALSE;

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

int CXTPListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}

void CXTPListBox::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove(nFlags, point);

	BOOL bOutside = FALSE;
	int nHotItem  = 0;
	if (TryItemFromPoint(point, bOutside, nHotItem))
	{
		if (bOutside)
			nHotItem = -1;

		if (nHotItem != m_nHotItem)
		{
			m_nHotItem = nHotItem;
			Invalidate(FALSE);

			if (m_nHotItem != -1)
			{
				TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT };
				_TrackMouseEvent(&tme);
			}
		}
	}
}

void CXTPListBox::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);
	Invalidate(FALSE);
}

void CXTPListBox::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	Invalidate(FALSE);
}

LRESULT CXTPListBox::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

void CXTPListBox::SetTheme(XTPControlTheme theme)
{
	SAFE_DELETE(m_pTheme);

	switch (theme)
	{
		case xtpControlThemeOfficeXP: m_pTheme = new CXTPListBoxThemeOfficeXP(); break;
		case xtpControlThemeOffice2013: m_pTheme = new CXTPListBoxThemeOffice2013(); break;
		case xtpControlThemeVisualStudio2015:
			m_pTheme = new CXTPListBoxThemeVisualStudio2015();
			break;
		case xtpControlThemeVisualStudio2017:
			m_pTheme = new CXTPListBoxThemeVisualStudio2017();
			break;
		case xtpControlThemeVisualStudio2019:
			m_pTheme = new CXTPListBoxThemeVisualStudio2019();
			break;
		case xtpControlThemeVisualStudio2022:
			m_pTheme = new CXTPListBoxThemeVisualStudio2022();
			break;
		case xtpControlThemeNativeWindows10:
			m_pTheme = new CXTPListBoxThemeNativeWindows10();
			break;
		case xtpControlThemeResource: m_pTheme = new CXTPListBoxThemeResource(); break;
		default: m_pTheme = new CXTPListBoxThemeDefault(); break;
	}

	if (m_hWnd)
	{
		RedrawWindow();
	}
}

void CXTPListBox::SetListStyle(XTPListBoxStyle style)
{
	switch (style)
	{
		case xtpListBoxOfficeXP: SetTheme(xtpControlThemeOfficeXP); break;
		case xtpListBoxOffice2007: SetTheme(xtpControlThemeResource); break;
		case xtpListBoxOffice2013: SetTheme(xtpControlThemeOffice2013); break;
		case xtpListBoxVisualStudio2015: SetTheme(xtpControlThemeVisualStudio2015); break;
		case xtpListBoxVisualStudio2017: SetTheme(xtpControlThemeVisualStudio2017); break;
		case xtpListBoxVisualStudio2019: SetTheme(xtpControlThemeVisualStudio2019); break;
		case xtpListBoxVisualStudio2022: SetTheme(xtpControlThemeVisualStudio2022); break;
		case xtpListBoxNativeWindows10: SetTheme(xtpControlThemeNativeWindows10); break;
	}
}

XTPListBoxStyle CXTPListBox::GetListStyle() const
{
	switch (m_theme)
	{
		case xtpControlThemeOfficeXP: return xtpListBoxOfficeXP;
		case xtpControlThemeResource: return xtpListBoxOffice2007;
		case xtpControlThemeOffice2013: return xtpListBoxOffice2013;
		case xtpControlThemeVisualStudio2015: return xtpListBoxVisualStudio2015;
		case xtpControlThemeVisualStudio2017: return xtpListBoxVisualStudio2017;
		case xtpControlThemeVisualStudio2019: return xtpListBoxVisualStudio2019;
		case xtpControlThemeVisualStudio2022: return xtpListBoxVisualStudio2022;
		case xtpControlThemeNativeWindows10: return xtpListBoxNativeWindows10;
		default: return xtpListBoxStandard;
	}
}

BOOL CXTPListBox::HasFocus() const
{
	return (::GetFocus() == GetSafeHwnd());
}

BOOL CXTPListBox::TryItemFromPoint(CPoint pt, BOOL& bOutside, int& nIndex) const
{
	int nCount = GetCount();

	if (nCount >= 0)
	{
		nIndex = static_cast<int>(ItemFromPoint(pt, bOutside));

		if (0 <= nIndex && nIndex < nCount)
		{
			return TRUE;
		}
	}

	nIndex = -1;
	return FALSE;
}
