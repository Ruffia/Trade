// XTPRibbonCustomizeQuickAccessPage.cpp: implementation of the CXTPRibbonCustomizeQuickAccessPage
// class.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

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
#include "Controls/Static/XTPMarkupStatic.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlPopup.h"

#include "Controls/Dialog/XTPDialogBase.h"
#include "Controls/Dialog/XTPPropertySheet.h"
#include "Controls/Dialog/XTPPropertyPage.h"
#include "CommandBars/Customize/XTPCustomizePageCommands.h"
#include "CommandBars/Customize/XTPCustomizeSheet.h"

#include "Ribbon/Resource.h"
#include "Ribbon/XTPRibbonQuickAccessControls.h"
#include "Ribbon/Customize/XTPRibbonCustomizeQuickAccessPage.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizeQuickAccessPage property page

CXTPRibbonCustomizeQuickAccessPage::CXTPRibbonCustomizeQuickAccessPage(CXTPCustomizeSheet* pSheet,
																	   UINT nIDTemplate)
	: CXTPPropertyPage(nIDTemplate == (UINT)-1 ? XTP_IDD_RIBBONCUSTOMIZE_QUICKACCESS : nIDTemplate)
// Conditional compilation padding issue workaround.
#ifdef _XTP_INCLUDE_MARKUP
	, m_wndTitleIcon(*(new CXTPMarkupStatic))
	, m_wndTitle(*(new CXTPMarkupStatic))
#else
	, m_wndTitleIcon(*(new CStatic))
	, m_wndTitle(*(new CStatic))
#endif
{
	m_pSheet	   = pSheet;
	m_pCommandBars = pSheet->GetCommandBars();

	m_bShowQuickAccessBelow = FALSE;
}

CXTPRibbonCustomizeQuickAccessPage::CXTPRibbonCustomizeQuickAccessPage(CXTPCommandBars* pCommandBars)
	: CXTPPropertyPage(XTP_IDD_RIBBONCUSTOMIZE_QUICKACCESS_LARGE)
// Conditional compilation padding issue workaround.
#ifdef _XTP_INCLUDE_MARKUP
	, m_wndTitleIcon(*(new CXTPMarkupStatic))
	, m_wndTitle(*(new CXTPMarkupStatic))
#else
	, m_wndTitleIcon(*(new CStatic))
	, m_wndTitle(*(new CStatic))
#endif
{
	m_pSheet	   = NULL;
	m_pCommandBars = pCommandBars;

	m_bShowQuickAccessBelow = FALSE;
}

CXTPRibbonCustomizeQuickAccessPage::~CXTPRibbonCustomizeQuickAccessPage()
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		delete m_arrCategories[i];
	}

	// Conditional compilation padding issue workaround.
	delete &m_wndTitleIcon;
	delete &m_wndTitle;
}

void CXTPRibbonCustomizeQuickAccessPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPRibbonCustomizeQuickAccessPage)
	DDX_Control(pDX, XTP_IDC_RIBBONCOMBO_CATEGORIES, m_lstCategories);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_QUICKACCESS, m_lstQuickAccess);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_COMMANDS, m_lstCommands);
	DDX_Check(pDX, XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, m_bShowQuickAccessBelow);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_RESET, m_btnReset);
	DDX_Control(pDX, XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, m_btnShowQuickAccessBelow);
	//}}AFX_DATA_MAP
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonCustomizeQuickAccessPage, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CXTPRibbonCustomizeQuickAccessPage)
	ON_BN_CLICKED(XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, OnCheckShowQuickAccessBelow)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONCOMBO_CATEGORIES, OnCategoriesSelectionChanged)
	ON_LBN_DBLCLK(XTP_IDC_RIBBONLIST_QUICKACCESS, OnDblclkListQuickAccess)
	ON_LBN_DBLCLK(XTP_IDC_RIBBONLIST_COMMANDS, OnDblclkListCommands)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_RESET, OnButtonReset)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONLIST_COMMANDS, OnCommandsSelectionChanged)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONLIST_QUICKACCESS, OnQuickAccessSelectionChanged)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPRibbonBar* CXTPRibbonCustomizeQuickAccessPage::GetRibbonBar()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, m_pCommandBars->GetMenuBar());
	ASSERT(pRibbonBar);

	return pRibbonBar;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizeQuickAccessPage message handlers

BOOL CXTPRibbonCustomizeQuickAccessPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	ModifyStyle(0, WS_CLIPCHILDREN);
	GetDlgItem(XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW)->ModifyStyle(0, BS_MULTILINE);

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		int nIndex = m_lstCategories.AddString(m_arrCategories[i]->strCategory);
		m_lstCategories.SetItemData(nIndex, XTPToULongPtr(i));

		if (i == 0)
			m_lstCategories.SetCurSel(0);
	}

	CSize sz = m_pCommandBars->GetPaintManager()->DrawListBoxControl(NULL, NULL, CRect(0, 0, 0, 0),
																	 FALSE, FALSE, m_pCommandBars);
	m_lstCommands.SetItemHeight(-1, XTPToUIntChecked(sz.cy));
	m_lstQuickAccess.SetItemHeight(-1, XTPToUIntChecked(sz.cy));

	m_lstCommands.m_pCommandBars	= m_pCommandBars;
	m_lstQuickAccess.m_pCommandBars = m_pCommandBars;

	OnCategoriesSelectionChanged();
	RefreshQuickAccessList();

	m_bShowQuickAccessBelow = GetRibbonBar()->IsQuickAccessBelowRibbon();
	UpdateData(FALSE);

	m_lstCommands.ModifyStyle(LBS_HASSTRINGS, 0);
	ASSERT(0 == (m_lstCommands.GetStyle() & LBS_HASSTRINGS));

	m_lstQuickAccess.ModifyStyle(LBS_HASSTRINGS, 0);
	ASSERT(0 == (m_lstQuickAccess.GetStyle() & LBS_HASSTRINGS));

	if (m_pCommandBars->IsLayoutRTL())
	{
		m_lstCommands.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_lstQuickAccess.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_lstCategories.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}

	if (GetDlgItem(XTP_IDC_RIBBONPAGE_TITLE) != NULL)
	{
		m_wndTitleIcon.SubclassDlgItem(XTP_IDC_RIBBONPAGE_TITLE_ICON, this);
		m_wndTitle.SubclassDlgItem(XTP_IDC_RIBBONPAGE_TITLE, this);

		COLORREF clrText = GetXtremeColor(COLOR_WINDOWTEXT);

		if (CXTPPropertyPage::m_pSheet)
		{
			if (CXTPPropertyPage::m_pSheet->GetTheme() != xtpControlThemeDefault)
				clrText = XTPIniColor(_T("Controls.Edit"), _T("Text"),
									  GetXtremeColor(COLOR_WINDOWTEXT));
		}

		CString foregroundProp;
		foregroundProp.Format(_T("Foreground = '#%02X%02X%02X' "), GetRValue(clrText),
							  GetGValue(clrText), GetBValue(clrText));

		CString icon;
		icon += _T("<StackPanel Orientation='Horizontal'>");
		icon += _T("<Viewbox Stretch='Uniform'>");
		icon += _T("<Canvas Width='32' Height='32' Canvas.Left='0' Canvas.Top='0'>");
		icon += _T("<Canvas>");
		icon += _T("<Rectangle Canvas.Left='1.5' Canvas.Top='11.5' Width='29' Height='13' ")
				_T("Fill='#FFFFFF' Stroke='#383838' StrokeThickness='1' StrokeMiterLimit='5'/>");
		icon += _T("<Path Data='M24.5,11.5H7.5V1.5h17V11.5zM24.5,20.5H7.5v10h17V20.5z' ")
				_T("Fill='#FFFFFF' Stroke='#383838' StrokeThickness='1' StrokeMiterLimit='5'/>");
		icon += _T("<Path Data='M10,23.5h12M10,25.5h12M10,27.5h12' Stroke='#787979' ")
				_T("StrokeThickness='1' StrokeMiterLimit='5'/>");
		icon += _T("<Rectangle Canvas.Left='4.5' Canvas.Top='14.5' Width='2' Height='2' ")
				_T("Fill='#238BCB' Stroke='#228BCB' StrokeThickness='1' ")
				_T("StrokeStartLineCap='Round' StrokeEndLineCap='Round' ")
				_T("StrokeMiterLimit='5'/>");
		icon += _T("</Canvas>");
		icon += _T("<Canvas>");
		icon += _T("<Polygon Points='29.5,18.5 29.5,23.5 20.6,23.5 20.6,18.5 18.5,18.5 ")
				_T("18.5,29.281 20.764,31.5 21.5,31.5 21.5,26.6 28.5,26.6 28.5,31.5 31.5,31.5 ")
				_T("31.5,18.5' Fill='#DFBCD9' StrokeLineJoin='Miter'/>");
		icon += _T("<Path ")
				_T("Data='M32,32H20.56L18,29.491V18h14V32zM20.968,31H31V19H19v10.071L20.968,31z' ")
				_T("Fill='#A0519F'/>");
		icon += _T("<Rectangle Canvas.Left='21.5' Canvas.Top='26.5' Width='7' Height='5' ")
				_T("Fill='#FFFFFF' Stroke='#A0519F' StrokeThickness='1' StrokeMiterLimit='1'/>");
		icon += _T("<Line X1='24.5' Y1='30' X2='24.5' Y2='28' Fill='#FFFFFF' Stroke='#A0519F' ")
				_T("StrokeThickness='1' StrokeMiterLimit='1'/>");
		icon += _T("<Rectangle Canvas.Left='20.5' Canvas.Top='18.5' Width='9' Height='5' ")
				_T("Fill='#FFFFFF' Stroke='#A0519F' StrokeThickness='1' StrokeMiterLimit='1'/>");
		icon += _T("</Canvas>");
		icon += _T("</Canvas>");
		icon += _T("</Viewbox>");
		icon += _T("</StackPanel>");

		m_wndTitleIcon.SetMarkupTextEx(icon);
		m_wndTitleIcon.SetIgnoreRTL(TRUE);

		CString xaml;
		xaml += _T("<StackPanel Orientation='Horizontal'>");
		xaml += _T("<TextBlock Padding='3, 0, 0, 0' FontSize='12' FontQuality='ClearType' ");
		xaml += foregroundProp;
		xaml += _T("VerticalAlignment='Center'>%s</TextBlock>");
		xaml += _T("</StackPanel>");

		m_wndTitle.SetMarkupTextEx(xaml);
	}

	SetResize(XTP_IDC_RIBBONCOMBO_CATEGORIES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(XTP_IDC_RIBBONLIST_COMMANDS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONLIST_QUICKACCESS, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_BOTTOMRIGHT);

	SetResize(XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, XTP_ANCHOR_BOTTOMLEFT,
			  XTP_ANCHOR_BOTTOMLEFT);
	SetResize(XTP_IDC_RIBBONBUTTON_RESET, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);

	SetResize(XTP_IDC_RIBBONBUTTON_ADD, CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));
	SetResize(XTP_IDC_RIBBONBUTTON_REMOVE, CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));

#ifdef _XTP_INCLUDE_CONTROLS

	if (CXTPPropertyPage::m_pSheet)
	{
		XTPControlTheme controlTheme = CXTPPropertyPage::m_pSheet->GetTheme();

		m_btnAdd.SetTheme(controlTheme);
		m_btnRemove.SetTheme(controlTheme);
		m_btnReset.SetTheme(controlTheme);
		m_lstCategories.SetTheme(controlTheme);
		m_btnShowQuickAccessBelow.SetTheme(controlTheme);

		m_clrText = XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"),
								GetXtremeColor(COLOR_WINDOWTEXT));
	}

#endif

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPRibbonCustomizeQuickAccessPage::OnCheckShowQuickAccessBelow()
{
	UpdateData();

	GetRibbonBar()->ShowQuickAccessBelowRibbon(m_bShowQuickAccessBelow);
}

void CXTPRibbonCustomizeQuickAccessPage::RefreshQuickAccessList()
{
	m_lstQuickAccess.SetRedraw(FALSE);
	int nTopIndex = m_lstQuickAccess.GetTopIndex();
	m_lstQuickAccess.ResetContent();

	CXTPControls* pControls = GetRibbonBar()->GetQuickAccessControls();

	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		if (!pControl->IsVisible())
			continue;
		int nPos = XTPToInt(m_lstQuickAccess.SendMessage(LB_INSERTSTRING,
														 XTPToWPARAM(m_lstQuickAccess.GetCount()),
														 XTPToLPARAM(pControl)));

		m_lstQuickAccess.SetItemDataPtr(nPos, pControl);
	}

	m_lstQuickAccess.SetTopIndex(nTopIndex);
	m_lstQuickAccess.SetRedraw(TRUE);

	OnQuickAccessSelectionChanged();
	OnCommandsSelectionChanged();
}

void CXTPRibbonCustomizeQuickAccessPage::OnCategoriesSelectionChanged()
{
	m_lstCommands.ResetContent();

	int nIndex = m_lstCategories.GetCurSel();

	if (nIndex == LB_ERR)
		return;

	XTP_COMMANDBARS_CATEGORYINFO* pInfo = GetCategoryInfo((int)m_lstCategories.GetItemData(nIndex));

	if (pInfo == NULL)
		return;

	for (int i = 0; i < pInfo->pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pInfo->pControls->GetAt(i);
		int nPos			  = XTPToInt(m_lstCommands.SendMessage(LB_INSERTSTRING,
													   XTPToWPARAM(m_lstCommands.GetCount()),
													   XTPToLPARAM(pControl)));
		m_lstCommands.SetItemDataPtr(nPos, pControl);
	}

	OnCommandsSelectionChanged();
}

XTP_COMMANDBARS_CATEGORYINFO*
	CXTPRibbonCustomizeQuickAccessPage::FindCategory(LPCTSTR strCategory) const
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		if (m_arrCategories[i]->strCategory.Compare(strCategory) == 0)
			return m_arrCategories[i];
	}
	return NULL;
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizeQuickAccessPage::GetCategoryInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrCategories.GetSize())
		return m_arrCategories[nIndex];
	return NULL;
}

BOOL CXTPRibbonCustomizeQuickAccessPage::AddCategory(LPCTSTR strCategory, CMenu* pMenu,
													 BOOL bListSubItems)
{
	CXTPControls* pCategoryControls = InsertCategory(strCategory);

	int nCount = pMenu->GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		if (pMenu->GetMenuItemID(i) > 0)
		{
			CXTPControlPopup* pControl = DYNAMIC_DOWNCAST(CXTPControlPopup,
														  pCategoryControls->AddMenuItem(pMenu, i));

			if (pControl && bListSubItems)
			{
				CXTPControls* pControls = pControl->GetCommandBar()->GetControls();
				for (int j = 0; j < pControls->GetCount(); j++)
				{
					pCategoryControls->AddClone(pControls->GetAt(j));
				}
			}
		}
	}

	return TRUE;
}

BOOL CXTPRibbonCustomizeQuickAccessPage::AddCategories(UINT nIDResource, BOOL bListSubItems)
{
	CMenu menu;
	if (!XTPResourceManager()->LoadMenu(&menu, nIDResource))
		return FALSE;

	int nCount = menu.GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		CString strCategory;
		if (menu.GetMenuString(XTPToUIntChecked(i), strCategory, MF_BYPOSITION) > 0)
		{
			CMenu* pMenu = menu.GetSubMenu(i);
			if (pMenu)
			{
				CXTPPaintManager::StripMnemonics(strCategory);

				if (!AddCategory(strCategory, pMenu, bListSubItems))
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CXTPRibbonCustomizeQuickAccessPage::AddCategories(CXTPControls* pControls)
{
	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		CString strCategory   = pControl->GetCategory();

		if (!strCategory.IsEmpty())
		{
			CXTPControls* pCategoryControls = InsertCategory(strCategory);
			pCategoryControls->AddClone(pControl);
		}
	}
	return TRUE;
}

CXTPControls* CXTPRibbonCustomizeQuickAccessPage::InsertCategory(LPCTSTR strCategory, int nIndex)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	if (!pInfo)
	{
		pInfo = new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pCommandBars);
		m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);
	}
	return pInfo->pControls;
}

CXTPControls* CXTPRibbonCustomizeQuickAccessPage::GetControls(LPCTSTR strCategory)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	return pInfo ? pInfo->pControls : NULL;
}

void CXTPRibbonCustomizeQuickAccessPage::OnDblclkListQuickAccess()
{
	OnButtonRemove();
}

void CXTPRibbonCustomizeQuickAccessPage::OnDblclkListCommands()
{
	OnButtonAdd();
}

void CXTPRibbonCustomizeQuickAccessPage::OnButtonAdd()
{
	if (!GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->IsWindowEnabled())
		return;

	if (m_lstCommands.GetCurSel() == LB_ERR)
		return;

	CXTPControl* pControl = (CXTPControl*)m_lstCommands.GetItemDataPtr(m_lstCommands.GetCurSel());
	if (!pControl)
		return;

	GetRibbonBar()->GetQuickAccessControls()->AddClone(pControl);
	GetRibbonBar()->OnRecalcLayout();

	RefreshQuickAccessList();
	m_lstQuickAccess.SetCurSel(m_lstQuickAccess.GetCount() - 1);

	OnQuickAccessSelectionChanged();
}

void CXTPRibbonCustomizeQuickAccessPage::OnButtonRemove()
{
	if (m_lstQuickAccess.GetCurSel() == LB_ERR)
		return;

	CXTPControl* pControl = (CXTPControl*)m_lstQuickAccess.GetItemDataPtr(
		m_lstQuickAccess.GetCurSel());
	if (!pControl)
		return;

	GetRibbonBar()->GetQuickAccessControls()->Remove(pControl);
	GetRibbonBar()->OnRecalcLayout();

	RefreshQuickAccessList();
}

void CXTPRibbonCustomizeQuickAccessPage::OnButtonReset()
{
	CString strPrompt;
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&strPrompt, XTP_IDS_RIBBONCUSTOMIZE_CONFIRMRESET));

	if (m_pSheet)
	{
		if (m_pSheet->MessageBox(strPrompt, MB_ICONWARNING | MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if (AfxMessageBox(strPrompt, MB_ICONWARNING | MB_YESNO) != IDYES)
			return;
	}

	GetRibbonBar()->GetQuickAccessControls()->Reset();
	RefreshQuickAccessList();
}

void CXTPRibbonCustomizeQuickAccessPage::OnCommandsSelectionChanged()
{
	BOOL bEnabled = m_lstCommands.GetCurSel() != LB_ERR;

	if (bEnabled)
	{
		CXTPControl* pControl = (CXTPControl*)m_lstCommands.GetItemDataPtr(
			m_lstCommands.GetCurSel());
		bEnabled = GetRibbonBar()->IsAllowQuickAccessControl(pControl);
	}

	GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->EnableWindow(bEnabled);
}

void CXTPRibbonCustomizeQuickAccessPage::OnQuickAccessSelectionChanged()
{
	GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(m_lstQuickAccess.GetCurSel() != LB_ERR);
}

void CXTPRibbonCustomizeQuickAccessPage::UpdatePage(void)
{
	if (NULL != GetSafeHwnd())
		RefreshQuickAccessList();
}

HBRUSH CXTPRibbonCustomizeQuickAccessPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = CXTPPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

#ifdef _XTP_INCLUDE_CONTROLS
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (CXTPPropertyPage::m_pSheet->GetTheme() != xtpControlThemeDefault)
			::SetTextColor(pDC->GetSafeHdc(), m_clrText);
	}
#endif

	return hBrush;
}
