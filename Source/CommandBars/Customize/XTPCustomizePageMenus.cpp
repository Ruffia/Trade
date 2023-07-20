// XTPCustomizePageMenus.cpp : implementation of the CXTPCustomizeMenusPage class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "TabManager/Includes.h"

#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizeDialog.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Edit/XTPEdit.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/Combo/XTPComboBox.h"
#include "Controls/Scroll/XTPScrollBar.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPCommandBarList.h"
#include "CommandBars/XTPCommandBarsContextMenus.h"

#include "Controls/Dialog/XTPDialogBase.h"
#include "Controls/Dialog/XTPPropertyPage.h"
#include "Controls/Dialog/XTPPropertySheet.h"
#include "Controls/Dialog/XTPPropertyPageNavigator.h"
#include "CommandBars/Customize/XTPCustomizePageOptions.h"
#include "CommandBars/Customize/XTPCustomizePageMenus.h"
#include "CommandBars/Customize/XTPCustomizeSheet.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CXTPFloatingPopupBar : public CXTPToolBar
{
	DECLARE_XTP_COMMANDBAR(CXTPFloatingPopupBar)
public:
	CXTPFloatingPopupBar()
	{
		m_barType = xtpBarTypePopup;
		m_dwFlags = xtpFlagFloating;
		SetWidth(0);
	}
};

IMPLEMENT_XTP_COMMANDBAR(CXTPFloatingPopupBar, CXTPToolBar)

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeMenusPage property page

CXTPCustomizeMenusPage::CXTPCustomizeMenusPage(CXTPCustomizeSheet* pSheet)
	: CXTPPropertyPage(CXTPCustomizeMenusPage::IDD)
	, m_pSheet(pSheet)
{
	//{{AFX_DATA_INIT(CXTPCustomizeMenusPage)
	//}}AFX_DATA_INIT
	m_pContextMenu  = NULL;
	m_ptContextMenu = CPoint(0, 0);
}

CXTPCustomizeMenusPage::~CXTPCustomizeMenusPage()
{
}

void CXTPCustomizeMenusPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPCustomizeMenusPage)
	DDX_Control(pDX, XTP_IDC_STATIC_ICON, m_wndIcon);
	DDX_Control(pDX, XTP_IDC_COMBO_CONTEXTMENUS, m_cmbContextMenus);
	DDX_Control(pDX, XTP_IDC_COMBO_APPMENUS, m_cmbAppMenus);
	DDX_Control(pDX, XTP_IDC_STATIC_APPLICATIONMENUS, m_wndApplicationMenus);
	DDX_Control(pDX, XTP_IDC_STATIC_CONTEXTMENUS, m_wndContextMenus);
	DDX_Control(pDX, XTP_IDC_BUTTON_RESETAPP, m_btnResetApp);
	DDX_Control(pDX, XTP_IDC_BUTTON_RESETCONTEXT, m_btnResetContent);
	//}}AFX_DATA_MAP
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCustomizeMenusPage, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CXTPCustomizeMenusPage)
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_APPMENUS, OnSelectionChangedAppmenus)
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_CONTEXTMENUS, OnSelectionChangedContextmenus)
	ON_WM_DESTROY()
	ON_BN_CLICKED(XTP_IDC_BUTTON_RESETCONTEXT, OnButtonResetContextMenu)
	ON_BN_CLICKED(XTP_IDC_BUTTON_RESETAPP, OnButtonResetAppMenu)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeMenusPage message handlers

BOOL CXTPCustomizeMenusPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();

	CString strDefaultMenu;
	GetDlgItemText(XTP_IDC_STATIC_DESCRIPTION, strDefaultMenu);

	if (_tcschr(strDefaultMenu, _T('\n')) != NULL)
	{
		AfxExtractSubString(m_strDefaultMenuDesc, strDefaultMenu, 1);
		AfxExtractSubString(m_strDefaultMenu, strDefaultMenu, 0);
	}
	else
	{
		m_strDefaultMenu = m_strDefaultMenuDesc = strDefaultMenu;
	}

	int nIndex = m_cmbAppMenus.AddString(m_strDefaultMenu);
	m_cmbAppMenus.SetItemData(nIndex, 0);
	int nCurSel = 0;

	if (pCommandBars->GetMenuBar()
		&& ((pCommandBars->GetMenuBar()->GetFlags() & xtpFlagIgnoreSetMenuMessage) == 0))
	{
		CXTPMenuBar* pMenuBar			  = pCommandBars->GetMenuBar();
		CXTPMenuBarMDIMenus* pMDIControls = pMenuBar->GetMDIMenus();

		POSITION pos = pMDIControls->GetStartPosition();
		CXTPMenuBarMDIMenuInfo* pInfo;
		while (pos)
		{
			pMDIControls->GetNextMenu(pos, pInfo);
			if (pInfo->m_nIDResource == pMenuBar->GetDefaultMenuResource())
				continue;

			if (!pInfo->m_strTitle.IsEmpty())
			{
				nIndex = m_cmbAppMenus.AddString(pInfo->m_strTitle);
				m_cmbAppMenus.SetItemData(nIndex, (DWORD_PTR)pInfo);

				if (pInfo->m_nIDResource == pMenuBar->GetCurrentMenuResource())
				{
					nCurSel = nIndex;
				}
			}
		}
	}

	m_cmbAppMenus.SetCurSel(nCurSel);
	OnSelectionChangedAppmenus();

	nIndex = m_cmbContextMenus.AddString(_T(""));
	m_cmbContextMenus.SetItemData(nIndex, 0);

	for (int i = 0; i < pCommandBars->GetContextMenus()->GetCount(); i++)
	{
		CXTPCommandBar* menu = pCommandBars->GetContextMenus()->GetAt(i);
		if (menu == NULL)
			continue;
		if (!menu->IsCustomizeDialogPresent())
			continue;
		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, menu);
		if (pPopupBar == NULL)
			continue;

		nIndex = m_cmbContextMenus.AddString(pPopupBar->GetTitle());
		m_cmbContextMenus.SetItemData(nIndex, (DWORD_PTR)pPopupBar);
	}
	m_cmbContextMenus.SetCurSel(0);
	OnSelectionChangedContextmenus();

	SetResize(XTP_IDC_STATIC_APPLICATIONMENUS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(XTP_IDC_STATIC_CONTEXTMENUS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

#ifdef _XTP_INCLUDE_CONTROLS
	XTPControlTheme controlTheme = m_pSheet->GetTheme();

	m_cmbContextMenus.SetTheme(controlTheme);
	m_cmbAppMenus.SetTheme(controlTheme);
	m_btnResetApp.SetTheme(controlTheme);
	m_btnResetContent.SetTheme(controlTheme);

	m_clrText = XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"),
							GetXtremeColor(COLOR_WINDOWTEXT));

	if (controlTheme == xtpControlThemeResource || controlTheme == xtpControlThemeOffice2013
		|| controlTheme == xtpControlThemeOffice2016
		|| controlTheme == xtpControlThemeVisualStudio2015
		|| controlTheme == xtpControlThemeVisualStudio2017
		|| controlTheme == xtpControlThemeVisualStudio2019
		|| controlTheme == xtpControlThemeVisualStudio2022
		|| controlTheme == xtpControlThemeNativeWindows10)
	{
		m_wndApplicationMenus.EnableTheme(controlTheme != xtpControlThemeDefault ? TRUE : FALSE);
		m_wndApplicationMenus.SetTextColor(m_clrText);
		m_wndApplicationMenus.SetBkColor(m_pSheet->GetNavigator()->GetPageColor());

		m_wndContextMenus.EnableTheme(controlTheme != xtpControlThemeDefault ? TRUE : FALSE);
		m_wndContextMenus.SetTextColor(m_clrText);
		m_wndContextMenus.SetBkColor(m_pSheet->GetNavigator()->GetPageColor());
	}

#endif

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPCustomizeMenusPage::OnSelectionChangedAppmenus()
{
	int nSel = m_cmbAppMenus.GetCurSel();

	if (nSel == CB_ERR)
		return;

	CXTPMenuBar* pMenuBar = m_pSheet->GetCommandBars()->GetMenuBar();
	UINT nIDResource	  = pMenuBar->GetDefaultMenuResource();

	CXTPMenuBarMDIMenuInfo* pInfo = (CXTPMenuBarMDIMenuInfo*)m_cmbAppMenus.GetItemData(nSel);

	if (pInfo)
	{
		SetDlgItemText(XTP_IDC_STATIC_DESCRIPTION, pInfo->m_strDescription);
		nIDResource = pInfo->m_nIDResource;
	}
	else
	{
		SetDlgItemText(XTP_IDC_STATIC_DESCRIPTION, m_strDefaultMenuDesc);
	}

	HICON hIcon = NULL;

#ifndef _XTP_COMMANDBARS_ACTIVEX
	if (pInfo)
	{
		hIcon = pInfo->m_hIcon;
	}
	else
#endif
	{
		HWND hWnd = m_pSheet->GetCommandBars()->GetSite()->GetSafeHwnd();
		hIcon	 = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
		if (!hIcon)
			hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(hWnd, GCLP_HICON);
	}

	if (hIcon)
	{
		m_wndIcon.SetIcon(hIcon);
	}

	if (m_cmbAppMenus.GetCount() > 1)
	{
		pMenuBar->SwitchMDIMenu(nIDResource);
	}
}

void CXTPCustomizeMenusPage::CommitContextMenu()
{
	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	if (m_pContextMenu)
	{
		CXTPPopupBar* pPopupBar = (CXTPPopupBar*)m_pContextMenu->GetCommandBarData();

		CXTPControls* pControls = m_pContextMenu->GetControls()->Duplicate();
		pPopupBar->SetControls(pControls);

		m_ptContextMenu = CXTPWindowRect(m_pContextMenu).TopLeft();

		pCommandBars->Remove(m_pContextMenu);
		m_pContextMenu = NULL;
	}
}

void CXTPCustomizeMenusPage::OnSelectionChangedContextmenus()
{
	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	CommitContextMenu();

	int nSel = m_cmbContextMenus.GetCurSel();

	if (nSel < 1)
	{
		GetDlgItem(XTP_IDC_BUTTON_RESETCONTEXT)->EnableWindow(FALSE);
		return;
	}

	GetDlgItem(XTP_IDC_BUTTON_RESETCONTEXT)->EnableWindow(TRUE);
	CXTPPopupBar* pPopupBar = (CXTPPopupBar*)m_cmbContextMenus.GetItemData(nSel);

	CXTPToolBar* pContextMenu = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop,
																RUNTIME_CLASS(
																	CXTPFloatingPopupBar));
	if (!pContextMenu)
	{
		TRACE0("Failed to create toolbar\n");
		return;
	}

	CXTPControls* pControls = pPopupBar->GetControls()->Duplicate();
	pContextMenu->SetControls(pControls);

	pContextMenu->SetBarID(pPopupBar->GetBarID());
	pContextMenu->SetTitle(pPopupBar->GetTitle());
	pContextMenu->SetCustomizeDialogPresent(FALSE);

	pContextMenu->SetPosition(xtpBarFloating);
	pContextMenu->EnableDocking(xtpFlagFloating);

	pContextMenu->OnRecalcLayout();

	CXTPWindowRect rcContext(pContextMenu);
	CXTPWindowRect rcParent(GetParent());

	CPoint pt = m_ptContextMenu;
	if (pt == CPoint(0, 0))
		pt = CPoint(rcParent.left - rcContext.Width(), rcParent.top);

	pContextMenu->MoveWindow(pt.x, pt.y, rcContext.Width(), rcContext.Height());

	m_pContextMenu = pContextMenu;

	pContextMenu->SetCommandBarData((DWORD_PTR)pPopupBar);
	pContextMenu->OnUpdateCmdUI();
}

void CXTPCustomizeMenusPage::OnDestroy()
{
	CommitContextMenu();

	if (m_cmbAppMenus.GetCount() > 1)
	{
		CXTPMenuBar* pMenuBar = m_pSheet->GetCommandBars()->GetMenuBar();
		if (pMenuBar)
			pMenuBar->SwitchActiveMenu();
	}

	CXTPPropertyPage::OnDestroy();
}

void CXTPCustomizeMenusPage::OnButtonResetContextMenu()
{
	if (m_pContextMenu)
	{
		m_pContextMenu->Reset(TRUE);
	}
}

void CXTPCustomizeMenusPage::OnButtonResetAppMenu()
{
	CXTPToolBar* pMenuBar = m_pSheet->GetCommandBars()->GetMenuBar();
	pMenuBar->Reset(TRUE);
}

HBRUSH CXTPCustomizeMenusPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = CXTPPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

#ifdef _XTP_INCLUDE_CONTROLS
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (m_pSheet->GetTheme() != xtpControlThemeDefault)
			::SetTextColor(pDC->GetSafeHdc(), m_clrText);
	}
#endif

	return hBrush;
}
