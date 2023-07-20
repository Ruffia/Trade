// XTPRibbonBackstageView.cpp : implementation file
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

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollable.h"
#include "Common/XTPCasting.h"
#include "Common/XTPMaskEditT.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#	include "Controls/Edit/XTPEdit.h"
#endif

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"
#include "CommandBars/XTPKeyboardManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/Resource.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonSystemButton.h"
#include "Ribbon/Backstage/XTPRibbonBackstageView.h"
#include "Ribbon/Backstage/XTPRibbonBackstageCommand.h"
#include "Ribbon/Backstage/XTPRibbonBackstageTab.h"
#include "Ribbon/Backstage/XTPRibbonBackstageMenuSeparator.h"

#include "Ribbon/Backstage/XTPRibbonBackstagePaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeResource.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeOffice2013.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeVisualStudio2015.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeVisualStudio2017.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeVisualStudio2019.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeVisualStudio2022.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonBackstageView, CXTPCommandBar);

const int gNewBacksategeHeaderHeight = 64;
const int gNewBacksategeSysButtonTop = 16;

CXTPRibbonBackstageView::CXTPRibbonBackstageView()
	: m_clrText(::GetSysColor(COLOR_WINDOWTEXT))
	, m_clrBack(::GetSysColor(COLOR_WINDOW))
	, m_pPaintManager(NULL)
{
#define CLEARTYPE_QUALITY 5

	m_nMenuWidth			   = 0;
	m_nKeyboardTipsLevel	   = xtpNoTips;
	m_chSecondLevelCurrentChar = 0;
	m_bRestoreQuickAccess	  = FALSE;
	m_backStageStyle		   = xtpRibbonBackstageOffice2016Style;
	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);

	lf.lfQuality = CLEARTYPE_QUALITY;

	m_xtpFontCommand.CreateFontIndirect(&lf);

	HDC hDC			= ::GetDC(NULL);
	UINT uPointSize = XTPToUInt(MulDiv(-lf.lfHeight, 72, ::GetDeviceCaps(hDC, LOGPIXELSY)));
	lf.lfHeight		= -MulDiv(XTPToIntChecked(++uPointSize), ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
	::ReleaseDC(NULL, hDC);

	m_xtpFontTab.CreateFontIndirect(&lf);

	m_pHighlightedControl = NULL;
	m_pFocusedControl	 = NULL;
	m_pActiveTab		  = NULL;

	m_pSystemButton		  = NULL;
	m_bIsScrollBarVisible = FALSE;
	m_listControlsToBeVisible.RemoveAll();

	SetTheme(xtpThemeResource);
}

CXTPRibbonBackstageView::~CXTPRibbonBackstageView()
{
	SAFE_DELETE(m_pPaintManager);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonBackstageView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPRibbonBackstageView* AFX_CDECL
	CXTPRibbonBackstageView::CreateBackstageView(CXTPCommandBars* pCommandBars)
{
	ASSERT_VALID(pCommandBars);

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pCommandBars->GetMenuBar());
	ASSERT(NULL != pRibbonBar);

	CXTPRibbonBackstageView* pView = new CXTPRibbonBackstageView();
	pView->SetCommandBars(pCommandBars);

	pRibbonBar->SetBackstageSysButtonTop(gNewBacksategeSysButtonTop);
	pRibbonBar->SetBackstageSysButtonBottom(gNewBacksategeHeaderHeight);

	return pView;
}

CXTPRibbonBackstageCommand* CXTPRibbonBackstageView::AddCommand(UINT nID)
{
	CXTPRibbonBackstageCommand* pControl = new CXTPRibbonBackstageCommand();
	GetControls()->Add(pControl, XTPToInt(nID));
	return pControl;
}

CXTPRibbonBackstageTab* CXTPRibbonBackstageView::AddTab(CWnd* pWnd, UINT nID)
{
	CXTPRibbonBackstageTab* pTab = new CXTPRibbonBackstageTab(pWnd->GetSafeHwnd());
	GetControls()->Add(pTab, XTPToInt(nID));
	return pTab;
}

CXTPRibbonBackstageMenuSeparator* CXTPRibbonBackstageView::AddMenuSeparator(UINT nID /*= UINT_MAX*/)
{
	CXTPRibbonBackstageMenuSeparator* pSpace = new CXTPRibbonBackstageMenuSeparator();
	GetControls()->Add(pSpace, UINT_MAX == nID ? -1 : XTPToInt(nID));
	return pSpace;
}

int CXTPRibbonBackstageView::IndexOf(CXTPControl* pControl) const
{
	if (NULL == pControl)
		return -1;

	for (int i = 0; i < GetControlCount(); i++)
	{
		if (GetControl(i) == pControl)
		{
			return i;
		}
	}

	return -1;
}

CWnd* CXTPRibbonBackstageView::GetActiveControlPane() const
{
	CXTPRibbonBackstageTab* pControlCustom = DYNAMIC_DOWNCAST(CXTPRibbonBackstageTab, m_pActiveTab);

	if (pControlCustom)
	{
		CWnd* pWnd = pControlCustom->GetControlPane();
		if (pWnd && !IsWindow(pWnd->m_hWnd))
			return NULL;

		return pWnd;
	}
	return NULL;
}

CXTPRibbonBackstageTab* CXTPRibbonBackstageView::GetActiveTab() const
{
	return m_pActiveTab;
}

void CXTPRibbonBackstageView::SetActiveTab(CXTPRibbonBackstageTab* pTab)
{
	CWnd* pOldContent = GetActiveControlPane();

	if (pOldContent && pOldContent->IsKindOf(RUNTIME_CLASS(CPropertyPage)))
	{
		if (((CPropertyPage*)pOldContent)->OnKillActive() == FALSE)
			return;
	}

	m_pActiveTab = pTab;

	CWnd* pNewContent = GetActiveControlPane();

	if (pNewContent)
	{
		pNewContent->ModifyStyle(WS_POPUP, WS_CHILD);
		pNewContent->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
		pNewContent->SetParent(this);
	}

	if (pOldContent && pOldContent != pNewContent)
	{
		pOldContent->ShowWindow(SW_HIDE);
		pOldContent->SetParent(GetSite());
	}

	if (m_pActiveTab)
	{
		m_pActiveTab->NotifySite(XTP_BS_TABCHANGED);
	}

	if (pNewContent && pNewContent->IsKindOf(RUNTIME_CLASS(CPropertyPage)))
	{
		((CPropertyPage*)pNewContent)->OnSetActive();
	}

	RecalcLayout();
}

CXTPControl* CXTPRibbonBackstageView::GetFocusedControl() const
{
	return m_pFocusedControl;
}

void CXTPRibbonBackstageView::SetFocusedControl(CXTPControl* pFocusedControl)
{
	if (m_pFocusedControl != pFocusedControl)
	{
		m_pFocusedControl = pFocusedControl;

		if (pFocusedControl && IsMenuSeparator(pFocusedControl) && pFocusedControl->GetEnabled())
		{
			// do nothing
		}
		else if (pFocusedControl && !IsCommand(pFocusedControl) && pFocusedControl->GetEnabled())
		{
			SetActiveTab((CXTPRibbonBackstageTab*)pFocusedControl);
		}

		Invalidate(FALSE);
	}
}

CXTPControl* CXTPRibbonBackstageView::GetHighlightedControl() const
{
	return m_pHighlightedControl;
}

BOOL CXTPRibbonBackstageView::IsActiveTab(CXTPControl* pControl) const
{
	return m_pActiveTab == pControl;
}

BOOL CXTPRibbonBackstageView::IsFocusedControl(CXTPControl* pControl) const
{
	return m_pFocusedControl == pControl;
}

BOOL CXTPRibbonBackstageView::IsHighlightedControl(CXTPControl* pControl) const
{
	return m_pHighlightedControl == pControl;
}

void CXTPRibbonBackstageView::SetTheme(XTPPaintTheme theme)
{
	SAFE_DELETE(m_pPaintManager);
	switch (theme)
	{
		case xtpThemeOffice2013:
			m_pPaintManager = new CXTPRibbonBackstageViewThemeOffice2013();
			break;
		case xtpThemeVisualStudio2015:
			m_pPaintManager = new CXTPRibbonBackstageViewThemeVisualStudio2015();
			break;
		case xtpThemeVisualStudio2017:
			m_pPaintManager = new CXTPRibbonBackstageViewThemeVisualStudio2017();
			break;
		case xtpThemeVisualStudio2019:
			m_pPaintManager = new CXTPRibbonBackstageViewThemeVisualStudio2019();
			break;
		case xtpThemeVisualStudio2022:
			m_pPaintManager = new CXTPRibbonBackstageViewThemeVisualStudio2022();
			break;
		case xtpThemeNativeWindows10:
			m_pPaintManager = new CXTPRibbonBackstageViewThemeNativeWindows10();
			{
				CXTPRibbonBackstageViewThemeNativeWindows10* pTheme =
					static_cast<CXTPRibbonBackstageViewThemeNativeWindows10*>(m_pPaintManager);

				pTheme->SetButtonBackNormalColor(XTPGetAccentColor(xtpPaintManagerAccentColor));
				pTheme->SetButtonBackHighlightedColor(
					XTPGetAccentColor(xtpPaintManagerPressedBackAccentColor));
				pTheme->SetButtonBackSelectedColor(
					XTPGetAccentColor(xtpPaintManagerHighlightBackAccentColor));

				pTheme->SetButtonTextNormalColor(
					XTPGetAccentColor(xtpPaintManagerNormalTextAccentColor));
				pTheme->SetButtonTextHighlightedColor(
					XTPGetAccentColor(xtpPaintManagerHighlightTextAccentColor));
				pTheme->SetButtonTextSelectedColor(
					XTPGetAccentColor(xtpPaintManagerPressedTextAccentColor));
			}
			break;
		default:
			m_pPaintManager  = new CXTPRibbonBackstageViewThemeResource();
			m_backStageStyle = xtpRibbonBackstageOffice2010Style;
			break;
	}

	int nCommandHeight = m_pPaintManager->GetMenuItemCommandHeight()
						 + m_pPaintManager->GetMenuItemCommandGapY();
	int nPageHeight = m_pPaintManager->GetMenuItemPageHeight()
					  + m_pPaintManager->GetMenuItemPageGapY();
	m_ScrolLine = max(nCommandHeight, nPageHeight);

	m_themeCurrent = theme;

	m_clrText = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageText"),
							::GetSysColor(COLOR_WINDOWTEXT));
	m_clrBack = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("PageBackground"),
							::GetSysColor(COLOR_WINDOW));
}

void CXTPRibbonBackstageView::OnExecute(CXTPControl* pControl)
{
	SetTrackingMode(FALSE);

	if (pControl)
	{
		pControl->OnExecute();
	}
}

void CXTPRibbonBackstageView::OnCancel()
{
	SetTrackingMode(FALSE);
}

CXTPControl* CXTPRibbonBackstageView::HitTest(CPoint point) const
{
	return GetControls()->HitTest(point);
}

BOOL CXTPRibbonBackstageView::IsCommand(CXTPControl* pControl) const
{
	return pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageCommand));
}

BOOL CXTPRibbonBackstageView::IsMenuSeparator(CXTPControl* pControl) const
{
	return pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageMenuSeparator));
}

int CXTPRibbonBackstageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPCommandBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndScrollBar.Create(WS_CHILD | WS_VISIBLE | SBS_VERT, CRect(10, 10, 50, 50), this, 100);
	m_wndScrollBar.SetScrollBarStyle(xtpScrollStyleOffice2007Dark);
	return 0;
}

int CXTPRibbonBackstageView::OnHookMessage(HWND /*hWnd*/, UINT nMessage, WPARAM& wParam,
										   LPARAM& /*lParam*/, LRESULT& /*lResult*/)
{
	if (nMessage == WM_WINDOWPOSCHANGED)
	{
		RecalcLayout();
	}

	if (nMessage == WM_CLOSE)
	{
		OnCancel();
		return FALSE;
	}

	if (nMessage == WM_ACTIVATEAPP)
	{
		SetFocus();
	}

	if (nMessage == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		if (GetSite()->IsWindowEnabled())
		{
			if (m_nKeyboardTipsLevel >= xtpFirstLevel)
			{
				if (m_nKeyboardTipsLevel == xtpTipsOnTabPane)
				{
					CXTPRibbonBackstageTab* pTab = GetActiveTab();
					if (pTab)
						pTab->ShowKeyboardTips(FALSE);
				}
				m_nKeyboardTipsLevel = xtpNoTips;
			}
			OnCancel();
			return TRUE;
		}
	}

	if (nMessage == WM_KEYDOWN || nMessage == WM_KEYUP)
	{
		return FALSE_EXIT;
	}

	if (nMessage == WM_MOUSEWHEEL)
	{
		return FALSE_EXIT;
	}

	return FALSE;
}

void CXTPRibbonBackstageView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_ESCAPE) // ESC is pressed
	{
		if (m_nKeyboardTipsLevel == xtpTipsOnTabPane)
		{
			CXTPRibbonBackstageTab* pTab = GetActiveTab();
			if (pTab)
				pTab->ShowKeyboardTips(FALSE);
		}

		m_nKeyboardTipsLevel = xtpNoTips;
		OnCancel();
		return;
	}

	if (ProcessKeyboardInput(WM_KEYDOWN, FALSE, nChar, 1))
		return;

	if (nChar == VK_SPACE || nChar == VK_RETURN)
	{
		if (m_pFocusedControl && IsCommand(m_pFocusedControl) && m_pFocusedControl->GetEnabled())
		{
			OnExecute(m_pFocusedControl);
		}
	}
	else if (nChar == VK_DOWN)
	{
		if (GetControlCount() == 0)
			return;

		int nIndex = IndexOf(m_pFocusedControl);
		if (nIndex == -1)
			nIndex = IndexOf(m_pActiveTab);

		nIndex = GetControls()->GetNext(nIndex, +1, TRUE);

		SetFocusedControl(GetControl(nIndex));
	}
	else if (nChar == VK_UP)
	{
		if (GetControlCount() == 0)
			return;

		int nIndex = IndexOf(m_pFocusedControl);
		if (nIndex == -1)
			nIndex = IndexOf(m_pActiveTab);

		nIndex = GetControls()->GetNext(nIndex, -1, TRUE);

		SetFocusedControl(GetControl(nIndex));
	}
	else if (nChar == VK_TAB)
	{
		CWnd* pWnd = GetActiveControlPane();
		if (pWnd)
		{
			pWnd->SetFocus();

			HWND hWnd = ::GetNextDlgTabItem(pWnd->GetSafeHwnd(), NULL, FALSE);

			if (hWnd)
			{
				pWnd->SendMessage(WM_NEXTDLGCTL, (WPARAM)hWnd, 1);
			}
		}
	}
}

UINT CXTPRibbonBackstageView::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPRibbonBackstageView::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPRibbonBackstageView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	CXTPControl* pHighlightedControl = HitTest(point);
	if (pHighlightedControl && !pHighlightedControl->GetEnabled())
		pHighlightedControl = NULL;

	if (pHighlightedControl != m_pHighlightedControl)
	{
		m_pHighlightedControl = pHighlightedControl;

		if (m_pHighlightedControl)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd };
			_TrackMouseEvent(&tme);
		}
		Invalidate(FALSE);
	}
}

void CXTPRibbonBackstageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus();

	CXTPControl* pFocusedControl = HitTest(point);

	if (pFocusedControl && !pFocusedControl->GetEnabled())
		pFocusedControl = NULL;

	SetFocusedControl(pFocusedControl);
}

void CXTPRibbonBackstageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	CXTPControl* pFocusedControl = HitTest(point);

	if (pFocusedControl && IsMenuSeparator(pFocusedControl))
	{
		return;
	}

	if (pFocusedControl && IsCommand(pFocusedControl) && pFocusedControl == m_pFocusedControl
		&& pFocusedControl->GetEnabled())
	{
		OnExecute(pFocusedControl);
	}
}

void CXTPRibbonBackstageView::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);

	m_pPaintManager->DrawBackground(&dc, this);

	for (int i = 0; i < GetControlCount(); i++)
	{
		dc.SetTextColor(RGB(0, 0, 0));
		CXTPControl* pControl = GetControl(i);
		m_pPaintManager->DrawControl(&dc, this, pControl);
		if ((m_nKeyboardTipsLevel > xtpNoTips) && (!IsMenuSeparator(pControl))
			&& (m_nKeyboardTipsLevel < xtpTipsOnTabPane))
		{
			if ((m_nKeyboardTipsLevel == xtpSecondLevel)
				&& (pControl->GetKeyboardTip().GetAt(0) != m_chSecondLevelCurrentChar))
				continue;
			m_pPaintManager->DrawKeyboardTip(&dc, m_pCommandBars->GetPaintManager(), pControl);
		}
	}
}

void CXTPRibbonBackstageView::RecalcLayout()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetCommandBars()->GetMenuBar());
	ASSERT(pRibbonBar);

	int nCaptionHeight = 0;
	bool bOldLook	  = (m_backStageStyle < xtpRibbonBackstageOffice2013Style)
					|| ((m_themeCurrent != xtpThemeOffice2013)
						&& (m_themeCurrent != xtpThemeVisualStudio2015)
						&& (m_themeCurrent != xtpThemeVisualStudio2017)
						&& (m_themeCurrent != xtpThemeVisualStudio2019)
						&& (m_themeCurrent != xtpThemeVisualStudio2022)
						&& (m_themeCurrent != xtpThemeNativeWindows10));

	if (pRibbonBar->IsWindowVisible())
	{
		if (bOldLook)
			nCaptionHeight = pRibbonBar->GetCaptionHeight() + pRibbonBar->GetTabsHeight()
							 + pRibbonBar->GetRibbonTopBorder() + XTP_DPI_Y(1);
		else
			nCaptionHeight = XTP_DPI_Y(gNewBacksategeHeaderHeight);
	}

	CWnd* pParentWnd = GetCommandBars()->GetSite();

	CRect rc;
	pParentWnd->GetClientRect(rc);

	rc.top += nCaptionHeight;

	if (rc.bottom < rc.top)
		rc.bottom = rc.top;

	int top = XTP_DPI_Y(7);

	int nCommandHeight = m_pPaintManager->GetMenuItemCommandHeight();
	int nPageHeight	= m_pPaintManager->GetMenuItemPageHeight();
	int nCommandGap	= m_pPaintManager->GetMenuItemCommandGapY();
	int nPageGap	   = m_pPaintManager->GetMenuItemPageGapY();

	int i, count = GetControlCount();
	int last = count - 1;

	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	CClientDC dc(this);
	CXTPFontDC font(&dc, &m_xtpFontCommand);

	m_nMenuWidth = XTP_DPI_X(132);

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl->IsVisible())
			continue;

		font.SetFont(IsCommand(pControl) ? &m_xtpFontCommand : &m_xtpFontTab);

		int width;
		if (!bOldLook)
			width = dc.GetTextExtent(pControl->GetCaption()).cx + 26
					+ m_pPaintManager->GetMenuItemCommandGapX() + XTP_DPI_Y(6);
		else
			width = dc.GetTextExtent(pControl->GetCaption()).cx
					+ m_pPaintManager->GetMenuItemCommandGapX()
					+ m_pPaintManager->GetMenuItemTextGap()
					+ m_pPaintManager->GetMenuItemCommandGapX() + XTP_DPI_Y(6);

		int nIconId					= pControl->GetIconId();
		CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(
			-1 == nIconId ? UINT_MAX : XTPToUInt(nIconId));
		if (pIcon)
		{
			width += XTP_DPI_X(pIcon->GetWidth()) + m_pPaintManager->GetMenuImageMargin();
		}

		m_nMenuWidth = max(m_nMenuWidth, width);
	}

	if (!bOldLook)
		pRibbonBar->SetBackstageModeMenuWidth(m_nMenuWidth);

	if (m_pSystemButton)
	{
		if (!bOldLook)
		{
			CRect rcButton(0, XTP_DPI_Y(gNewBacksategeSysButtonTop), m_nMenuWidth,
						   XTP_DPI_Y(gNewBacksategeHeaderHeight));
			m_pSystemButton->SetRect(rcButton);
		}
	}

	top = XTP_DPI_Y(7);

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl->IsVisible())
			continue;

		if (IsCommand(pControl))
		{
			pControl->SetRect(CRect(m_pPaintManager->GetMenuItemCommandGapX(), top - m_nScrollPos,
									m_nMenuWidth - m_pPaintManager->GetMenuItemCommandGapX(),
									top + nCommandHeight - m_nScrollPos));
			top += nCommandHeight;
			if (i != last)
				top += nCommandGap;
		}
		else if (IsMenuSeparator(pControl))
		{
			if (m_pPaintManager->GetMenuSeparatorHeight() < 1)
				continue;

			pControl->SetRect(
				CRect(0, top - m_nScrollPos, m_nMenuWidth,
					  top + m_pPaintManager->GetMenuSeparatorHeight() - m_nScrollPos));
			top += m_pPaintManager->GetMenuSeparatorHeight();
			if (i != last)
				top += nPageGap;
		}
		else
		{
			int h = nPageHeight;
			pControl->SetRect(CRect(0, top - m_nScrollPos, m_nMenuWidth, top + h - m_nScrollPos));
			top += nPageHeight;
			if (i != last)
				top += nPageGap;
		}
	}

	int nWidth = GetSystemMetrics(SM_CXVSCROLL);
	if (top > rc.Height())
	{
		if (m_nScrollPos != 0)
		{
			int dy = m_nPageSize - rc.Height();
			m_nScrollPos += dy;
			if (m_nScrollPos < 0)
				m_nScrollPos = 0;
			int nMax = top - rc.Height();
			if (m_nScrollPos > nMax)
				m_nScrollPos = nMax;
		}
		m_nPageSize			  = rc.Height();
		m_nMax				  = top;
		m_bIsScrollBarVisible = TRUE;
		m_wndScrollBar.ShowScrollBar(TRUE);
		m_wndScrollBar.SetWindowPos(0, rc.Width() - nWidth, 0, nWidth, rc.Height(),
									SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		m_nScrollPos = 0;
		m_wndScrollBar.ShowScrollBar(FALSE);
		m_bIsScrollBarVisible = FALSE;
	}

	CWnd* pWnd = GetActiveControlPane();

	if (pWnd)
	{
		CSize szControlPaneMinSize = m_pActiveTab->m_szControlPaneMinSize;

		int tabTop = XTP_DPI_Y(2);
		CRect rcContent(m_nMenuWidth, tabTop, rc.right, rc.bottom - nCaptionHeight);
		rcContent.right  = max(rcContent.right, rcContent.left + szControlPaneMinSize.cx);
		rcContent.bottom = max(rcContent.bottom, rcContent.top + szControlPaneMinSize.cy);
		if (m_bIsScrollBarVisible)
			rcContent.right -= nWidth;
		int dy = rcContent.top - m_nScrollPos;
		if (dy > 0)
			dy = 0;
		else
			dy = -dy;
		pWnd->SetWindowPos(&wndBottom, rcContent.left, rcContent.top - m_nScrollPos,
						   rcContent.Width(), rcContent.Height() + dy,
						   SWP_SHOWWINDOW | SWP_NOACTIVATE);
		m_rcOldPaneRect.top	= rcContent.top - m_nScrollPos;
		m_rcOldPaneRect.bottom = rcContent.bottom - m_nScrollPos;
		m_rcOldPaneRect.right  = rcContent.right;
		m_rcOldPaneRect.left   = rcContent.left;
	}
	else
		m_rcOldPaneRect = CRect(0, 0, 0, 0);
	if (m_bIsScrollBarVisible)
		UpdateScrollInfo();
	if (!bOldLook)
		pRibbonBar->Redraw();

	Invalidate(FALSE);
}

BOOL CXTPRibbonBackstageView::Popup(CXTPControlPopup* pControlPopup, BOOL /*bSelectFirst = FALSE*/)
{
	CXTPRibbonControlSystemButton* pSystemButton = DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton,
																	pControlPopup);
	ASSERT(pSystemButton);
	if (!pSystemButton)
		return FALSE;

	m_nKeyboardTipsLevel	  = xtpNoTips;
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pSystemButton->GetParent());
	ASSERT(NULL != pRibbonBar);

	if ((m_backStageStyle > xtpRibbonBackstageOffice2010Style)
		&& ((m_themeCurrent == xtpThemeOffice2013) || (m_themeCurrent == xtpThemeVisualStudio2015)
			|| (m_themeCurrent == xtpThemeVisualStudio2017)
			|| (m_themeCurrent == xtpThemeVisualStudio2019)
			|| (m_themeCurrent == xtpThemeVisualStudio2022)
			|| (m_themeCurrent == xtpThemeNativeWindows10)))
	{
		m_rcOldSysButtonRect = pSystemButton->GetRect();
		pRibbonBar->SetBackstageSysButtonTop(gNewBacksategeSysButtonTop);
		pRibbonBar->SetBackstageSysButtonBottom(gNewBacksategeHeaderHeight);
		pRibbonBar->SetCaptionBarBackstageMode(TRUE);

		m_bRestoreQuickAccess = pRibbonBar->IsQuickAccessVisible();
		if (m_bRestoreQuickAccess)
		{
			pRibbonBar->ShowQuickAccess(FALSE);
		}

		m_listControlsToBeVisible.RemoveAll();

		for (int i = 0; i < pRibbonBar->GetControlCount(); i++)
		{
			CXTPControl* pControl = pRibbonBar->GetControl(i);
			if ((pControl->GetFlags() & xtpFlagRibbonTabBackground) && pControl->IsVisible())
			{
				pControl->SetVisible(FALSE);
				m_listControlsToBeVisible.AddTail(i);
			}
		}
	}

	m_pSystemButton = pSystemButton;

	GetCommandBars()->ClosePopups();

	m_bTracking = TRUE;
	pSystemButton->OnSetPopup(TRUE);

	m_pFocusedControl	 = NULL;
	m_pHighlightedControl = NULL;

	ShowBackstageView();

	OnIdleUpdateCmdUI(TRUE, 0L);

	m_bTracking = FALSE;
	SetTrackingMode(TRUE);

	return TRUE;
}

void CXTPRibbonBackstageView::OnDestroy()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetCommandBars()->GetMenuBar());
	ASSERT(NULL != pRibbonBar);

	m_nKeyboardTipsLevel = xtpNoTips;
	pRibbonBar->SetCaptionBarBackstageMode(FALSE);
	m_bTracking = FALSE;

	CWnd* pParentWnd = GetCommandBars()->GetSite();

	XTPHookManager()->RemoveHook(pParentWnd->GetSafeHwnd(), this);

	for (int i = 0; i < m_arrChildWindows.GetSize(); i++)
	{
		HWND hWndChild = m_arrChildWindows[i];
		::ShowWindow(hWndChild, SW_SHOWNOACTIVATE);
	}

	SetActiveTab(NULL);

	if (m_pSystemButton)
	{
		m_pSystemButton->OnSetPopup(FALSE);
		if (m_backStageStyle > xtpRibbonBackstageOffice2010Style)
		{
			m_pSystemButton->SetRect(m_rcOldSysButtonRect);
		}

		if (m_pSystemButton->GetParent())
		{
			m_pSystemButton->GetParent()->Redraw(0, FALSE);
		}

		GetCommandBars()->RecalcFrameLayout();
	}

	GetCommandBars()->GetKeyboardManager()->UnhookKeyboard(this);

	m_pSystemButton = NULL;

	CWnd::OnDestroy();
	if ((m_backStageStyle > xtpRibbonBackstageOffice2010Style)
		&& ((m_themeCurrent == xtpThemeOffice2013) || (m_themeCurrent == xtpThemeVisualStudio2015)
			|| (m_themeCurrent == xtpThemeVisualStudio2017)
			|| (m_themeCurrent == xtpThemeVisualStudio2019)
			|| (m_themeCurrent == xtpThemeVisualStudio2022)
			|| (m_themeCurrent == xtpThemeNativeWindows10)))
	{
		if (m_bRestoreQuickAccess)
		{
			pRibbonBar->ShowQuickAccess(TRUE);
		}

		pRibbonBar->SetBackstageModeMenuWidth(0);

		if (!m_listControlsToBeVisible.IsEmpty())
		{
			POSITION pos = m_listControlsToBeVisible.GetHeadPosition();
			for (int i = 0; i < m_listControlsToBeVisible.GetCount(); i++)
			{
				int index			  = m_listControlsToBeVisible.GetNext(pos);
				CXTPControl* pControl = pRibbonBar->GetControl(index);
				pControl->SetVisible(TRUE);
			}

			m_listControlsToBeVisible.RemoveAll();
		}

		pRibbonBar->OnIdleUpdateCmdUI(0, 0);
	}
}

BOOL CXTPRibbonBackstageView::SetTrackingMode(int bMode, BOOL /*bSelectFirst = TRUE*/,
											  BOOL /*bKeyboard = FALSE*/)
{
	if (m_bTracking == bMode)
		return FALSE;

	m_bTracking = bMode;

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetCommandBars()->GetMenuBar());
	ASSERT(pRibbonBar);

	for (int i = 0; i < pRibbonBar->GetControlCount(); i++)
	{
		pRibbonBar->GetControl(i)->OnEnabledChanged();
	}

	if (!m_hWnd)
		return FALSE;

	CWnd* pWnd					  = GetSite();
	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (!bMode)
	{
		DestroyWindow();

		if (pWnd && ::IsWindow(pWnd->m_hWnd))
			pWnd->SendMessage(WM_XTP_UNINITCOMMANDSPOPUP, 0, (LPARAM)this);
	}
	else
	{
	}

	if (pCommandBars)
	{
		pCommandBars->OnTrackingModeChanged(this, bMode);
	}
	return TRUE;
}

void CXTPRibbonBackstageView::ShowBackstageView()
{
	CWnd* pParentWnd = GetCommandBars()->GetSite();

	CWnd::Create(AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 0,
				 WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), pParentWnd, 0);

	XTPHookManager()->SetHook(pParentWnd->GetSafeHwnd(), this);

	GetCommandBars()->GetKeyboardManager()->HookKeyboard(this);

	m_wndScrollBar.SetCommandBars(GetCommandBars());
	m_nScrollPos		  = 0;
	m_bIsScrollBarVisible = FALSE;

	RecalcLayout();

	m_arrChildWindows.RemoveAll();

	for (HWND hWndChild = ::GetWindow(pParentWnd->m_hWnd, GW_CHILD); hWndChild != NULL;
		 hWndChild		= ::GetWindow(hWndChild, GW_HWNDNEXT))
	{
		if (::GetDlgCtrlID(hWndChild) == AFX_IDW_DOCKBAR_TOP || hWndChild == m_hWnd)
			continue;

		DWORD dwStyle = XTPToDWORD(::GetWindowLong(hWndChild, GWL_STYLE));
		if ((dwStyle & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE)
		{
			::ShowWindow(hWndChild, SW_HIDE);
			m_arrChildWindows.Add(hWndChild);
		}
	}

	::SetFocus(m_hWnd);

	for (int i = 0; i < GetControlCount(); i++)
	{
		if (GetControl(i)->IsItemDefault())
		{
			SetFocusedControl(GetControl(i));
			break;
		}
	}
	CheckKeyboardTips();
	CheckTabsKeyboardTips();
}

BOOL CXTPRibbonBackstageView::IsDpiIconsScalingEnabled() const
{
	return CXTPPaintManager::IsDpiBitmapScalingEnabled(m_pCommandBars);
}

static BOOL AFXAPI _XTPCompareClassName(HWND hWnd, LPCTSTR lpszClassName)
{
	ASSERT(NULL != lpszClassName);

	BOOL bEqual = FALSE;
	ASSERT(::IsWindow(hWnd));
	TCHAR szTemp[XTP_MAX_CLASSNAME + 1];
	if (0 != ::GetClassName(hWnd, szTemp, _countof(szTemp)))
	{
		bEqual = (0 == _tcsicmp(szTemp, lpszClassName));
	}

	return bEqual;
}

BOOL CXTPRibbonBackstageView::PreTranslateMessage(MSG* pMsg)
{
	// for modeless processing (or modal)
	ASSERT(m_hWnd != NULL);

	if ((pMsg->message == WM_SYSKEYUP) || (pMsg->message == WM_SYSKEYDOWN))
	{
		BOOL altPressed = TRUE;
		UINT mess		= WM_KEYDOWN;
		UINT key		= 0;
		if (pMsg->wParam != VK_MENU)
			key = XTPToUInt(pMsg->wParam);
		int rep = LOWORD(pMsg->lParam);
		if (pMsg->message == WM_SYSKEYUP)
			mess = WM_KEYUP;
		else
			rep++;
		if (ProcessKeyboardInput(mess, altPressed, key, rep))
			return TRUE;
	}
	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// fix around for VK_ESCAPE in a multiline Edit that is on a Dialog
	// that doesn't have a cancel or the cancel is disabled.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL)
		&& (::GetWindowLong(pMsg->hwnd, GWL_STYLE) & ES_MULTILINE)
		&& _XTPCompareClassName(pMsg->hwnd, _T("Edit")))
	{
		HWND hItem = ::GetDlgItem(m_hWnd, IDCANCEL);
		if (hItem == NULL || ::IsWindowEnabled(hItem))
		{
			SendMessage(WM_COMMAND, IDCANCEL, 0);
			return TRUE;
		}
	}
	// filter both messages to dialog and from children
	return PreTranslateInput(pMsg);
}

void CXTPRibbonBackstageView::SetBackstageStyle(
	XTPRibbonBackstageStyle style /* = xtpRibbonBackstageOffice2016Style*/)
{
	if ((m_themeCurrent != xtpThemeOffice2013) && (m_themeCurrent != xtpThemeVisualStudio2015)
		&& (m_themeCurrent != xtpThemeVisualStudio2017)
		&& (m_themeCurrent != xtpThemeVisualStudio2019)
		&& (m_themeCurrent != xtpThemeVisualStudio2022)
		&& (m_themeCurrent != xtpThemeNativeWindows10))
		m_backStageStyle = xtpRibbonBackstageOffice2010Style;
	else
		m_backStageStyle = style;
}

void CXTPRibbonBackstageView::CheckKeyboardTips()
{
	CMap<UINT, UINT, int, int> mapUsedKeys;

	int i;
	// First pass
	// Check if key tip is set. If not try to find the tip in the control caption
	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		CString strTips		  = pControl->GetKeyboardTip();
		if (strTips.IsEmpty())
		{
			CString strCaption = pControl->GetCaption();
			if (strCaption.IsEmpty())
				continue;
			int pos = CXTPShortcutManager::FindAccelPos(strCaption);
			if ((pos < 0) || ((pos + 1) >= strCaption.GetLength()))
				continue;
			// The tip is not set but it is in the caption
			strTips += strCaption.GetAt(pos + 1);
			pControl->SetKeyboardTip(strTips);
		}
		strTips.MakeUpper();
		UINT key = CXTPShortcutManager::AccelKeyToVirtualKey(strTips.GetAt(0));
		int val;
		// Check if the tip is used by other controls
		if (mapUsedKeys.Lookup(key, val))
			val++;
		else
			val = 0;
		mapUsedKeys[key] = val;
	}

	// Second pass
	// Resolving conflicts between keyboard tips in controls
	CStringArray arrDoubleKeys;
	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		CString strTips		  = pControl->GetKeyboardTip();
		CString strCaption	= pControl->GetCaption();
		if (strTips.IsEmpty())
		{
			// No tip was set
			BOOL found = FALSE;
			for (int j = 0; j < strCaption.GetLength(); j++)
			{
				CString s;
				// Pass the "and" symbol
				if ((j + 1) < strCaption.GetLength())
				{
					if ((strCaption.GetAt(j) == _TCHAR('&'))
						&& (strCaption.GetAt(j + 1) == _TCHAR('&')))
					{
						j++;
						continue;
					}
				}
				// Pass spaces
				if (_istspace(XTP_CHARTOCRT(strCaption.GetAt(j))))
					continue;
				// Found letter will be the keyboard tip
				s += strCaption.GetAt(j);
				s.MakeUpper();
				// Check if the tip is used already
				UINT key = CXTPShortcutManager::AccelKeyToVirtualKey(s.GetAt(0));
				int val;
				if (!mapUsedKeys.Lookup(key, val))
				{
					mapUsedKeys[key] = 0;
					pControl->SetKeyboardTip(s);
					found = TRUE;
					break;
				}
			}
			if (!found)
			{
				// Suitable keyboard tip was not found. Set a digit as keyboard tip
				for (int j = 1; j < 10; j++)
				{
					CString s;
					s.Format(_T("%d"), j);
					UINT key = CXTPShortcutManager::AccelKeyToVirtualKey(s.GetAt(0));
					int val;
					if (!mapUsedKeys.Lookup(key, val))
					{
						mapUsedKeys[key] = val;
						pControl->SetKeyboardTip(s);
						found = TRUE;
						break;
					}
				}
			}
			continue;
		}

		// Final checking of using defined keyboard tip
		strTips.MakeUpper();
		UINT key = CXTPShortcutManager::AccelKeyToVirtualKey(strTips.GetAt(0));
		int val;
		if (!mapUsedKeys.Lookup(key, val))
			val = 0;
		if (val == 0)
		{
			// Tip is not used by other controls
			if (strTips.GetLength() > 1)
				strTips = strTips.Left(1);
			pControl->SetKeyboardTip(strTips);
			continue;
		}
		// Tip is used some other control
		if (strTips.GetLength() == 1)
		{
			// Tip has only one letter. Try to construct a tip with two letters
			for (int j = 0; j < strCaption.GetLength(); j++)
			{
				CString s(strTips);
				if ((j + 1) < strCaption.GetLength())
				{
					if ((strCaption.GetAt(j) == _TCHAR('&'))
						&& (strCaption.GetAt(j + 1) == _TCHAR('&')))
					{
						j++;
						continue;
					}
				}
				if (_istspace(XTP_CHARTOCRT(strCaption.GetAt(j))))
					continue;
				s += strCaption.GetAt(j);
				s.MakeUpper();
				BOOL found = FALSE;
				for (int k = 0; k < arrDoubleKeys.GetSize(); k++)
				{
					if (arrDoubleKeys[k].CompareNoCase(s) == 0)
					{
						found = TRUE;
						break;
					}
				}
				if (!found)
				{
					pControl->SetKeyboardTip(s);
					arrDoubleKeys.Add(s);
					break;
				}
			}
			continue;
		}
		// Cut tip to 2 letters. This is the maximum size for keyboard tips
		if (strTips.GetLength() > 2)
			strTips = strTips.Left(2);

		// Check if "two letters" is used already?
		BOOL found = FALSE;
		for (int k = 0; k < arrDoubleKeys.GetSize(); k++)
		{
			if (arrDoubleKeys[k].CompareNoCase(strTips) == 0)
			{
				found = TRUE;
				break;
			}
		}
		if (!found)
		{
			// it's not used. The problem is solved
			pControl->SetKeyboardTip(strTips);
			continue;
		}
		// Find a new second letter for 2 letters key
		strTips = strTips.Left(1);
		for (int j = 0; j < strCaption.GetLength(); j++)
		{
			CString s(strTips);
			if ((j + 1) < strCaption.GetLength())
			{
				if ((strCaption.GetAt(j) == _TCHAR('&'))
					&& (strCaption.GetAt(j + 1) == _TCHAR('&')))
				{
					j++;
					continue;
				}
			}
			if (_istspace(XTP_CHARTOCRT(strCaption.GetAt(j))))
				continue;
			s += strCaption.GetAt(j);
			s.MakeUpper();

			found = FALSE;
			for (int k = 0; k < arrDoubleKeys.GetSize(); k++)
			{
				if (arrDoubleKeys[k].CompareNoCase(s) == 0)
				{
					found = TRUE;
					break;
				}
			}
			if (!found)
			{
				// Appropriate letter is found
				pControl->SetKeyboardTip(s);
				arrDoubleKeys.Add(s);
				break;
			}
		}
	}
	arrDoubleKeys.RemoveAll();
	mapUsedKeys.RemoveAll();
}

XTPRibbonBackstageKeyboardTipResult
	CXTPRibbonBackstageView::TranslateKeyboardTip(UINT key, int& nControlIndex)
{
	if (m_nKeyboardTipsLevel == xtpTipsOnTabPane)
	{
		// The current level shows keyboard tips for buttons on the active pane
		CXTPRibbonBackstageTab* pTab = GetActiveTab();
		int count					 = pTab->GetKeyboardTipsCount();
		// find the button which corresponds to the pressed key
		for (int i = 0; i < count; i++)
		{
			TCHAR ch = pTab->GetKeyboardTipChar(i);
			if (!CXTPShortcutManager::CompareAccelKey(ch, key))
				continue;
			// Button is found. Execute the appropriate command
			pTab->ShowKeyboardTips(FALSE);
			pTab->ClickControlWithKeyboardTip(i);
			return xtpHideTips; // The end of the keyboard tip life cycle
		}
		return xtpNoActionForThisLevel; // No button found
	}
	if ((m_nKeyboardTipsLevel < xtpUserHoldAltKey) || (m_nKeyboardTipsLevel > xtpSecondLevel))
	{
		// It's out of the keyboard "life cycle"
		m_chSecondLevelCurrentChar = 0;
		return xtpNoActionForThisLevel;
	}
	// Tips for backstage controls are currently shown
	for (int i = 0; i < GetControlCount(); i++)
	{
		nControlIndex		  = i;
		CXTPControl* pControl = GetControl(i);
		CString strTip		  = pControl->GetKeyboardTip();
		if (m_nKeyboardTipsLevel == xtpSecondLevel)
		{
			// "two letters" keyboard tips are shown
			if ((strTip.GetLength() == 1) || (strTip.GetAt(0) != m_chSecondLevelCurrentChar))
				continue; // the first character does not correspond currently shown tips
			if (!CXTPShortcutManager::CompareAccelKey(strTip.GetAt(1), key))
				continue; // the second character does not correspond currently shown tips
			// Found needed control. End the life cycle
			m_chSecondLevelCurrentChar = 0;
			if (pControl->GetEnabled())
			{
				if (IsCommand(pControl))
					return xtpExecuteCommand;
				else
					return xtpSelectTab;
			}
			return xtpHideTips;
		}
		if (!CXTPShortcutManager::CompareAccelKey(strTip.GetAt(0), key))
			continue; // This is a first level and the control tip does not correspond pressed key
		if (strTip.GetLength() == 2)
		{
			// The found control has two letters: go to the second level
			m_chSecondLevelCurrentChar = strTip.GetAt(0);
			return xtpShowSecondLevel;
		}
		// It's needed control. End keyboard tips life cycle
		if (pControl->GetEnabled())
		{
			if (IsCommand(pControl))
				return xtpExecuteCommand;
			else
				return xtpSelectTab;
		}
		return xtpHideTips;
	}
	return xtpNoActionForThisLevel;
}

void CXTPRibbonBackstageView::CheckTabsKeyboardTips()
{
	for (int i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (IsCommand(pControl) || IsMenuSeparator(pControl))
			continue;
		CXTPRibbonBackstageTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonBackstageTab, GetControl(i));
		if (pTab)
			pTab->CheckKeyboardTips();
	}
}

COLORREF CXTPRibbonBackstageView::GetKeyboardTipsBackColor()
{
	return m_pCommandBars->GetPaintManager()->GetXtremeColor(COLOR_INFOBK);
}

CFont* CXTPRibbonBackstageView::GetKeyboardTipsFont()
{
	return &m_pCommandBars->GetPaintManager()->m_fontToolTip;
}

BOOL CXTPRibbonBackstageView::ProcessKeyboardInput(UINT keyMessage, BOOL altPressed, UINT key,
												   int repeat)
{
	if (altPressed && (0 == key))
	{
		if ((WM_KEYDOWN == keyMessage) && (repeat > 1))
		{
			// Alt key is pressed but was not released
			if (m_nKeyboardTipsLevel == xtpNoTips)
			{
				m_nKeyboardTipsLevel = xtpUserHoldAltKey;
				Invalidate(FALSE);
			}
			return TRUE;
		}
		if (WM_KEYUP == keyMessage)
		{
			// Alt key was released
			if (m_nKeyboardTipsLevel > xtpUserHoldAltKey)
			{
				if (repeat > 1)
					return TRUE;
				if (m_nKeyboardTipsLevel == xtpTipsOnTabPane)
				{
					CXTPRibbonBackstageTab* pTab = GetActiveTab();
					if (pTab)
						pTab->ShowKeyboardTips(FALSE);
				}
				m_nKeyboardTipsLevel = xtpNoTips;
			}
			else
				m_nKeyboardTipsLevel = xtpFirstLevel;
			Invalidate(FALSE);
			return TRUE;
		}
	}
	if (m_nKeyboardTipsLevel >= xtpUserHoldAltKey)
	{
		if (repeat > 1)
			return TRUE;
		// A key was pressed after activating keyboard tips
		int index;
		XTPRibbonBackstageKeyboardTipResult translateResult = TranslateKeyboardTip(key, index);
		switch (translateResult)
		{
			case xtpNoActionForThisLevel:
				if (xtpTipsOnTabPane == m_nKeyboardTipsLevel)
				{
					CXTPRibbonBackstageTab* pTab = GetActiveTab();
					if (pTab)
						pTab->ShowKeyboardTips(FALSE);
				}
				m_nKeyboardTipsLevel = xtpNoTips;
				return FALSE;
			case xtpExecuteCommand:
				m_nKeyboardTipsLevel = xtpNoTips;
				OnExecute(GetControl(index));
				break;
			case xtpHideTips:
				m_nKeyboardTipsLevel = xtpNoTips;
				Invalidate(FALSE);
				break;
			case xtpShowSecondLevel:
				m_nKeyboardTipsLevel = xtpSecondLevel;
				Invalidate(FALSE);
				break;
			case xtpSelectTab:
				m_nKeyboardTipsLevel		 = xtpTipsOnTabPane;
				CXTPRibbonBackstageTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonBackstageTab,
																GetControl(index));
				if (pTab)
				{
					SetActiveTab(pTab);
					if (pTab->GetKeyboardTipsCount() > 0)
						pTab->ShowKeyboardTips(TRUE);
				}
				else
				{
					m_nKeyboardTipsLevel = xtpNoTips;
				}

				Invalidate(FALSE);
				break;
		}
		return TRUE;
	}
	return FALSE;
}

CScrollBar* CXTPRibbonBackstageView::GetScrollBarCtrl(int nBar)
{
	if (nBar == SB_VERT)
		return &m_wndScrollBar;
	return NULL;
}

CScrollBar* CXTPRibbonBackstageView::GetScrollBarCtrl(int nBar) const
{
	return const_cast<CXTPRibbonBackstageView*>(this)->GetScrollBarCtrl(nBar);
}

void CXTPRibbonBackstageView::UpdateScrollInfo()
{
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_ALL;
	si.nPos   = m_nScrollPos;

	si.nPage = XTPToUIntChecked(m_nPageSize);
	si.nMax  = m_nMax;

	m_wndScrollBar.SetScrollInfo(&si);
}
void CXTPRibbonBackstageView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
	int y = m_nScrollPos;
	switch (nSBCode)
	{
		case SB_TOP: y = 0; break;
		case SB_BOTTOM: y = m_nMax - m_nPageSize; break;
		case SB_LINEUP: y -= m_ScrolLine; break;
		case SB_LINEDOWN: y += m_ScrolLine; break;
		case SB_PAGEUP: y -= m_nPageSize; break;
		case SB_PAGEDOWN: y += XTPToIntChecked(m_nPageSize); break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION: y = XTPToIntChecked(nPos); break;
	}
	SetScrollPos(y);
}

void CXTPRibbonBackstageView::SetScrollPos(int nScrollPos)
{
	CRect r;
	GetClientRect(&r);
	r.right -= GetSystemMetrics(SM_CXVSCROLL);
	r.bottom = m_nMax;
	int nMax = m_nMax - m_nPageSize;
	if (nScrollPos > nMax)
		nScrollPos = nMax;

	if (nScrollPos < 0)
		nScrollPos = 0;

	if (m_nScrollPos == nScrollPos)
		return;

	int dy		 = m_nScrollPos - nScrollPos;
	m_nScrollPos = nScrollPos;
	RepositionControls(dy);
	UpdateScrollInfo();
	Invalidate(TRUE);
}

void CXTPRibbonBackstageView::RepositionControls(int dy)
{
	for (int i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		CRect r				  = pControl->GetRect();
		r.top += dy;
		r.bottom += dy;
		pControl->SetRect(r);
	}
	CWnd* pWnd = GetActiveControlPane();

	if (pWnd)
	{
		m_rcOldPaneRect.top += dy;
		m_rcOldPaneRect.bottom += dy;
		if (m_rcOldPaneRect.top < 0)
			dy = -m_rcOldPaneRect.top;
		else
			dy = 0;
		pWnd->SetWindowPos(0, m_rcOldPaneRect.left, m_rcOldPaneRect.top, m_rcOldPaneRect.Width(),
						   m_rcOldPaneRect.Height() + dy,
						   SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

BOOL CXTPRibbonBackstageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_bIsScrollBarVisible)
	{
		SetScrollPos(m_nScrollPos + (zDelta < 0 ? m_ScrolLine : -m_ScrolLine));
		return TRUE;
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonBackstageView, CXTPCommandBar)
	DISP_PROPERTY_EX_ID(CXTPRibbonBackstageView, "Style", 1102, OleGetStyle, OleSetStyle, VT_I4)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddCommand", 1000, OleAddCommand, VT_DISPATCH,
					 VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddTab", 1001, OleAddTab, VT_DISPATCH,
					 VTS_I4 VTS_BSTR VTS_HANDLE)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddTab", 1002, OleClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "SetTheme", 1003, OleSetTheme, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddMenuSeparator", 1004, OleAddMenuSeparator,
					 VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "GetForeColor", 1100, OleGetColorForeground, VT_COLOR,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "GetBackColor", 1101, OleGetColorBackground, VT_COLOR,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPRibbonBackstageView, CXTPCommandBar)
	INTERFACE_PART(CXTPRibbonBackstageView, XTPDIID_RibbonBackstageView, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonBackstageView, XTPDIID_RibbonBackstageView)

LPDISPATCH CXTPRibbonBackstageView::OleAddCommand(long nId, LPCTSTR lpszCaption)
{
	CXTPControl* pControl = AddCommand(nId);
	pControl->SetCaption(lpszCaption);

	return XTPGetDispatch(pControl);
}

LPDISPATCH CXTPRibbonBackstageView::OleAddMenuSeparator()
{
	CXTPControl* pControl = AddMenuSeparator();
	return XTPGetDispatch(pControl);
}

void CXTPRibbonBackstageView::OleSetTheme(long nTheme)
{
	SetTheme((XTPPaintTheme)nTheme);
}

COLORREF CXTPRibbonBackstageView::OleGetColorForeground() const
{
	return m_clrText;
}

COLORREF CXTPRibbonBackstageView::OleGetColorBackground() const
{
	return m_clrBack;
}

LPDISPATCH CXTPRibbonBackstageView::OleAddTab(long nId, LPCTSTR lpszCaption, OLE_HANDLE hWnd)
{
	CXTPRibbonBackstageTab* pControl = (CXTPRibbonBackstageTab*)GetControls()->Add(
		new CXTPRibbonBackstageTab(reinterpret_cast<HWND>((LONG_PTR)hWnd)), nId);
	pControl->SetCaption(lpszCaption);

	return XTPGetDispatch(pControl);
}

void CXTPRibbonBackstageView::OleClose()
{
	OnCancel();
}

int CXTPRibbonBackstageView::OleGetStyle()
{
	return m_backStageStyle;
}

void CXTPRibbonBackstageView::OleSetStyle(int NewValue)
{
	SetBackstageStyle((XTPRibbonBackstageStyle)NewValue);
}

#endif // _XTP_COMMANDBARS_ACTIVEX
