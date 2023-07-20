// XTPPropertyPageNavigator.cpp : implementation file
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

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "TabManager/Includes.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizeDialog.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Edit/XTPEdit.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/Scroll/XTPScrollBar.h"

#ifdef _XTP_INCLUDE_COMMANDBARS
#	include "CommandBars/Resource.h"
#	include "CommandBars/XTPCommandBarsDefines.h"
#	include "CommandBars/XTPPaintManager.h"
#	include "CommandBars/XTPShortcutManager.h"
#	include "CommandBars/XTPCommandBars.h"
#	include "CommandBars/XTPCommandBar.h"
#	include "CommandBars/XTPToolBar.h"
#	include "CommandBars/XTPMenuBar.h"
#	include "CommandBars/XTPControl.h"
#	include "CommandBars/XTPControlEdit.h"
#endif

#include "Controls/Dialog/XTPDialogBase.h"
#include "Controls/Dialog/XTPPropertyPage.h"
#include "Controls/Dialog/XTPPropertySheet.h"
#include "Controls/Dialog/XTPPropertyPageNavigator.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_QUERYUISTATE
#	define WM_QUERYUISTATE 0x0129
#endif

#ifndef WM_CHANGEUISTATE
#	define WM_CHANGEUISTATE 0x0127
#endif

#ifndef UISF_HIDEFOCUS
#	define UISF_HIDEFOCUS 0x1
#endif

#ifndef UIS_CLEAR
#	define UIS_CLEAR 2
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyPageNavigator

CXTPPropertyPageNavigator::CXTPPropertyPageNavigator()
{
	m_pSheet	   = 0;
	m_themeCurrent = xtpControlThemeDefault;
	CPoint pt	  = CXTPDrawHelpers::Dlu2Pix(3, 3);

	m_rcNavigatorMargin.SetRect(pt.x, pt.y, pt.x, pt.y);
	m_rcPageMargin.SetRect(2, 2, 2, 2);

	m_rcButtonMargin.SetRect(pt.x, 0, pt.x, pt.y);
}

CXTPPropertyPageNavigator::~CXTPPropertyPageNavigator()
{
}

void CXTPPropertyPageNavigator::SetPropertySheet(CXTPPropertySheet* pSheet)
{
	m_pSheet = pSheet;
}

void CXTPPropertyPageNavigator::SetTheme(XTPControlTheme theme)
{
	m_themeCurrent = theme;
}

XTPControlTheme CXTPPropertyPageNavigator::GetTheme()
{
	return m_themeCurrent;
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyPageTabNavigator

CXTPPropertyPageTabNavigator::CXTPPropertyPageTabNavigator()
{
	SetColor(xtpTabColorWinNative);
}

BEGIN_MESSAGE_MAP(CXTPPropertyPageTabNavigator, CXTPTabControl)
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()

void CXTPPropertyPageTabNavigator::OnSysColorChange()
{
	CXTPTabControl::OnSysColorChange();

	GetPaintManager()->RefreshMetrics();
	m_brushPage.DeleteObject();
}

void CXTPPropertyPageTabNavigator::OnPageChanged()
{
	DeleteAllItems();

	for (int i = 0; i < m_pSheet->GetPageCount(); i++)
	{
		CXTPPropertyPage* pPage = m_pSheet->GetPage(i);

		CString strCaption = pPage->GetCaption();

		CXTPTabManagerItem* pItem = InsertItem(i, strCaption);
		pItem->SetData((DWORD_PTR)pPage);
		pPage->m_dwData = (DWORD_PTR)pItem;
	}
}

BOOL CXTPPropertyPageTabNavigator::Create()
{
	if (!CXTPTabControl::Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_GROUP | WS_TABSTOP,
								CRect(0, 0, 0, 0), m_pSheet, 1000))
		return FALSE;

	OnPageChanged();

	CFont* pFont = m_pSheet->GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	GetPaintManager()->SetFontIndirect(&lf);

	return TRUE;
}

void CXTPPropertyPageTabNavigator::PageToSize(AFX_SIZEPARENTPARAMS* pLayout, CSize szPage,
											  CRect& rcPage, CSize& szClient)
{
	CRect rcTabControl(0, 0, szPage.cx, szPage.cy);

	CClientDC dc(this);
	GetPaintManager()->RepositionTabControl(this, &dc, rcTabControl);

	AdjustRect(TRUE, &rcTabControl);
	rcTabControl.InflateRect(m_rcPageMargin);

	szClient = CSize(rcTabControl.Width() + m_rcNavigatorMargin.left + m_rcNavigatorMargin.right,
					 rcTabControl.Height() + m_rcNavigatorMargin.top + m_rcNavigatorMargin.bottom);

	rcPage = CRect(CPoint(m_rcNavigatorMargin.left - rcTabControl.left,
						  m_rcNavigatorMargin.top - rcTabControl.top),
				   szPage);

	AfxRepositionWindow(pLayout, m_hWnd, CRect(m_rcNavigatorMargin.TopLeft(), rcTabControl.Size()));

	GetPaintManager()->RepositionTabControl(this, &dc,
											CRect(0, 0, rcTabControl.Width(),
												  rcTabControl.Height()));
}

void CXTPPropertyPageTabNavigator::SizeToPage(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient,
											  CRect& rcPage)
{
	CRect rcTabControl(0, 0, szClient.cx, szClient.cy);
	rcTabControl.DeflateRect(m_rcNavigatorMargin);

	rcPage = rcTabControl;
	AdjustRect(FALSE, &rcPage);
	rcPage.DeflateRect(m_rcPageMargin);

	AfxRepositionWindow(pLayout, m_hWnd, CRect(m_rcNavigatorMargin.TopLeft(), rcTabControl.Size()));

	m_brushPage.DeleteObject();
}

#define COLOR_WINXP (0xFF000001)

HBRUSH CXTPPropertyPageTabNavigator::GetPageBrush(CDC* pDC, CXTPPropertyPage* pPage)
{
	COLORREF clrClient = COLORREF_NULL;

	if (GetPaintManager()->GetColor() == xtpTabColorWinNative)
		clrClient = COLOR_WINXP;
	else
		clrClient = GetPaintManager()->GetColorSet()->GetClientColor(this);

	if (m_clrBrush != clrClient)
	{
		m_brushPage.DeleteObject();
		m_clrBrush = clrClient;
	}

	if (m_brushPage.GetSafeHandle())
		return m_brushPage;

	if (m_clrBrush == GetXtremeColor(COLOR_3DFACE))
		return 0;

	if (m_clrBrush != COLOR_WINXP)
	{
		m_brushPage.CreateSolidBrush(m_clrBrush);
		return m_brushPage;
	}

	if (m_brushPage.GetSafeHandle() == 0)
	{
		CXTPClientRect rc(pPage);
		if (rc.IsRectEmpty())
			return 0;

		CXTPWinThemeWrapper wrapper;
		wrapper.OpenThemeData(NULL, L"TAB");

		if (!wrapper.IsAppThemeActive())
			return 0;

		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
		{
			CRect rectContent(0, 0, 0, 0);
			if (FAILED(wrapper.GetThemeBackgroundContentRect(pDC->GetSafeHdc(), TABP_PANE, 0, rc,
															 rectContent)))
			{
				rectContent = rc;
				rectContent.DeflateRect(2, 2, 4, 4);
			}

			CRect rcPane(-rectContent.left, -rectContent.top,
						 rc.right + (rc.right - rectContent.right),
						 rc.bottom + (rc.bottom - rectContent.bottom));

			CXTPCompatibleDC dc(pDC, bmp);
			wrapper.DrawThemeBackground(dc, TABP_PANE, 0, rcPane, 0);
		}

		m_brushPage.CreatePatternBrush(&bmp);
	}

	return m_brushPage;
}

void CXTPPropertyPageTabNavigator::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	CXTPPropertyPage* pPage = (CXTPPropertyPage*)pItem->GetData();
	if (!pPage || (m_pSelected == pItem))
	{
		CXTPTabManager::SetSelectedItem(pItem);
		return;
	}

	m_pSheet->SetActivePage(pPage);
}

void CXTPPropertyPageTabNavigator::OnPageSelected(CXTPPropertyPage* pPage)
{
	CXTPTabManagerItem* pItem = (CXTPTabManagerItem*)pPage->m_dwData;

	CXTPTabManager::SetSelectedItem(pItem);
}

void CXTPPropertyPageTabNavigator::SetTheme(XTPControlTheme theme)
{
	CXTPPropertyPageNavigator::SetTheme(theme);

	XTPTabAppearanceStyle tabAppearance = xtpTabAppearancePropertyPage;

	switch (theme)
	{
		case xtpControlThemeResource: tabAppearance = xtpTabAppearancePropertyPage2007; break;
		case xtpControlThemeOffice2013: tabAppearance = xtpTabThemeOffice2013; break;
		case xtpControlThemeVisualStudio2015: tabAppearance = xtpTabThemeVisualStudio2015; break;
		case xtpControlThemeVisualStudio2017: tabAppearance = xtpTabThemeVisualStudio2017; break;
		case xtpControlThemeVisualStudio2019: tabAppearance = xtpTabThemeVisualStudio2019; break;
		case xtpControlThemeVisualStudio2022: tabAppearance = xtpTabThemeVisualStudio2022; break;
		case xtpControlThemeNativeWindows10: tabAppearance = xtpTabAppearanceNativeWindows10; break;
		default: break;
	}

	GetPaintManager()->SetAppearance(tabAppearance);
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyPageControlNavigator

CXTPPropertyPageControlNavigator::CXTPPropertyPageControlNavigator()
{
	m_szControl = XTP_DPI(CSize(150, 50));
	m_clrBrush  = COLORREF_NULL;
}

void CXTPPropertyPageControlNavigator::PageToSize(AFX_SIZEPARENTPARAMS* pLayout, CSize szPage,
												  CRect& rcPage, CSize& szClient)
{
	CSize szList(m_szControl);
	szList.cy = max(szList.cy, szPage.cy);

	szClient = CSize(szList.cx + 2 * m_rcNavigatorMargin.left + m_rcNavigatorMargin.right
						 + szPage.cx,
					 szList.cy + m_rcNavigatorMargin.top + m_rcNavigatorMargin.bottom);

	rcPage = CRect(CPoint(szList.cx + 2 * m_rcNavigatorMargin.left, m_rcNavigatorMargin.top),
				   szPage);

	AfxRepositionWindow(pLayout, GetSafeHwnd(), CRect(m_rcNavigatorMargin.TopLeft(), szList));
}

void CXTPPropertyPageControlNavigator::SizeToPage(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient,
												  CRect& rcPage)
{
	CSize szList(m_szControl.cx,
				 szClient.cy - m_rcNavigatorMargin.top - m_rcNavigatorMargin.bottom);

	CSize szPage(szClient.cx - szList.cx - 2 * m_rcNavigatorMargin.left - m_rcNavigatorMargin.right,
				 szClient.cy - m_rcNavigatorMargin.top - m_rcNavigatorMargin.bottom);

	rcPage = CRect(CPoint(szList.cx + 2 * m_rcNavigatorMargin.left, m_rcNavigatorMargin.top),
				   szPage);

	AfxRepositionWindow(pLayout, GetSafeHwnd(), CRect(m_rcNavigatorMargin.TopLeft(), szList));
}

HBRUSH CXTPPropertyPageControlNavigator::GetPageBrush(CDC* /*pDC*/, CXTPPropertyPage* /*pPage*/)
{
	if (m_pSheet->GetPageBorderStyle() != xtpPageBorderFrame)
		return NULL;

	COLORREF clrClient = GetXtremeColor(COLOR_WINDOW);

	if (m_clrBrush != clrClient)
	{
		m_brushPage.DeleteObject();
		m_clrBrush = clrClient;
	}

	if (!m_brushPage.GetSafeHandle())
		m_brushPage.CreateSolidBrush(clrClient);

	return m_brushPage;
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyPageListNavigator

CXTPPropertyPageListNavigator::CXTPPropertyPageListNavigator()
{
	m_nItemHeight  = 0;
	m_nItemMargin  = XTP_DPI_X(12);
	m_nTextPadding = XTP_DPI_X(12);
}

BEGIN_MESSAGE_MAP(CXTPPropertyPageListNavigator, CXTPListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelChanged)
END_MESSAGE_MAP()

void CXTPPropertyPageListNavigator::OnPageChanged()
{
	ResetContent();

	for (int i = 0; i < m_pSheet->GetPageCount(); i++)
	{
		CXTPPropertyPage* pPage = m_pSheet->GetPage(i);

		CString strCaption = pPage->GetCaption();

		int nItem = AddString(strCaption);
		SetItemData(nItem, (DWORD_PTR)pPage);
		pPage->m_dwData = (DWORD_PTR)nItem;
	}
}

void CXTPPropertyPageListNavigator::SetPropertySheet(CXTPPropertySheet* pSheet)
{
	CXTPPropertyPageNavigator::SetPropertySheet(pSheet);
	m_pSheet->SetPageBorderStyle(xtpPageBorderFrame);
}

BOOL CXTPPropertyPageListNavigator::Create()
{
	CFont* pFont = m_pSheet->GetFont();
	{
		CWindowDC dc(NULL);
		CXTPFontDC font(&dc, pFont);
		m_nItemHeight = dc.GetTextExtent(_T("X"), 1).cy + m_nItemMargin;
	}

	if (!CListBox::CreateEx(WS_EX_CLIENTEDGE, _T("LISTBOX"), 0,
							LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | WS_VISIBLE | WS_VSCROLL
								| WS_CHILD | WS_GROUP | WS_TABSTOP | LBS_NOTIFY | LBS_HASSTRINGS,
							CRect(0, 0, 0, 0), m_pSheet, 1000))
		return FALSE;

	SetFont(pFont);

	OnPageChanged();

	return TRUE;
}

void CXTPPropertyPageListNavigator::OnPageSelected(CXTPPropertyPage* pPage)
{
	int nItem = (int)pPage->m_dwData;
	CListBox::SetCurSel(nItem);
}

void CXTPPropertyPageListNavigator::OnSelChanged()
{
	int nCurSel = GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	CXTPPropertyPage* pPage = (CXTPPropertyPage*)GetItemData(nCurSel);
	if (!pPage)
	{
		return;
	}

	if (!m_pSheet->SetActivePage(pPage))
	{
		SetCurSel(m_pSheet->GetActiveIndex());
	}
}

void CXTPPropertyPageListNavigator::SetTheme(XTPControlTheme theme)
{
	CXTPPropertyPageNavigator::SetTheme(theme);

	CXTPListBox::SetTheme(theme);
}

HBRUSH CXTPPropertyPageListNavigator::GetPageBrush(CDC* /*pDC*/, CXTPPropertyPage* /*pPage*/)
{
	COLORREF clrClient = GetXtremeColor(COLOR_WINDOW);

	if (m_themeCurrent != xtpControlThemeDefault)
	{
		clrClient = XTPIniColor(_T("Controls.ListBox"), _T("ItemBack"),
								GetXtremeColor(COLOR_WINDOW));
	}

	if (m_clrBrush != clrClient)
	{
		m_brushPage.DeleteObject();
		m_clrBrush = clrClient;
	}

	if (!m_brushPage.GetSafeHandle())
		m_brushPage.CreateSolidBrush(clrClient);

	return m_brushPage;
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyPageTreeNavigator

CXTPPropertyPageTreeNavigator::CXTPPropertyPageTreeNavigator(DWORD dwStyle)
	: m_dwStyle(dwStyle)
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPropertyPageTreeNavigator, CWnd)
	ON_NOTIFY_REFLECT(TVN_SELCHANGINGA, OnSelChanging)
	ON_NOTIFY_REFLECT(TVN_SELCHANGINGW, OnSelChanging)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPPropertyPageTreeNavigator::CreateTree()
{
	for (int i = 0; i < m_pSheet->GetPageCount(); i++)
	{
		CXTPPropertyPage* pPage = m_pSheet->GetPage(i);

		CString strCaption = pPage->GetCaption();

		HTREEITEM hItem = InsertItem(strCaption);
		SetItemData(hItem, (DWORD_PTR)pPage);
		pPage->m_dwData = (DWORD_PTR)hItem;
	}

	return TRUE;
}

void CXTPPropertyPageTreeNavigator::OnPageChanged()
{
	DeleteAllItems();
	VERIFY(CreateTree());
}

void CXTPPropertyPageTreeNavigator::SetPropertySheet(CXTPPropertySheet* pSheet)
{
	CXTPPropertyPageNavigator::SetPropertySheet(pSheet);
	m_pSheet->SetPageBorderStyle(xtpPageBorderFrame);
}

BOOL CXTPPropertyPageTreeNavigator::Create()
{
	CFont* pFont = m_pSheet->GetFont();

	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, 0,
						WS_VISIBLE | WS_VSCROLL | WS_CHILD | WS_GROUP | WS_TABSTOP | m_dwStyle,
						CRect(0, 0, 0, 0), m_pSheet, 1000))
		return FALSE;

	SetFont(pFont);

	if (!CreateTree())
		return FALSE;

	return TRUE;
}

void CXTPPropertyPageTreeNavigator::OnPageSelected(CXTPPropertyPage* pPage)
{
	HTREEITEM hItem = (HTREEITEM)pPage->m_dwData;
	CTreeCtrl::EnsureVisible(hItem);
	CTreeCtrl::SelectItem(hItem);
}

void CXTPPropertyPageTreeNavigator::OnSelChanging(NMHDR* pNMHeader, LRESULT* pResult)
{
	HTREEITEM hItem = (pNMHeader->code == TVN_SELCHANGINGA)
						  ? ((LPNMTREEVIEWA)pNMHeader)->itemNew.hItem
						  : ((LPNMTREEVIEWW)pNMHeader)->itemNew.hItem;

	if (!hItem)
		return;

	CXTPPropertyPage* pPage = (CXTPPropertyPage*)GetItemData(hItem);
	if (!pPage)
	{
		hItem = GetChildItem(hItem);
		if (!hItem)
			return;

		pPage = (CXTPPropertyPage*)GetItemData(hItem);
		if (!pPage)
			return;
	}

	if (!m_pSheet->SetActivePage(pPage))
	{
		*pResult = TRUE;
	}
}
