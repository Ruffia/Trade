// XTPRibbonCustomizePage.cpp: implementation of the CXTPRibbonCustomizePage class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
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
#include "Controls/Scroll/XTPScrollBar.h"
#include "Controls/Static/XTPMarkupStatic.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Combo/XTPComboBox.h"

#include "Controls/CoreTree/XTPCoreTreeControl.h"
#include "Controls/CoreTree/XTPCoreTreeItem.h"
#include "Controls/CoreTree/XTPCoreTreePaintManager.h"

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
#include "Ribbon/Customize/XTPRibbonCustomizeTreeCtrl.h"
#include "Ribbon/Customize/XTPRibbonRenameDialog.h"
#include "Ribbon/Customize/XTPRibbonCustomizePage.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonTab.h"
#include "Ribbon/XTPRibbonGroups.h"
#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonSystemButton.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_INLINE CXTPControl* ControlFromItem(CXTPCoreTreeItem* pItem)
{
	return DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizePage property page

CXTPRibbonCustomizePage::CXTPRibbonCustomizePage(CXTPCustomizeSheet* pSheet)
	: CXTPPropertyPage(XTP_IDD_RIBBONCUSTOMIZE_RIBBON)
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
}

CXTPRibbonCustomizePage::CXTPRibbonCustomizePage(CXTPCommandBars* pCommandBars)
	: CXTPPropertyPage(XTP_IDD_RIBBONCUSTOMIZE_RIBBON)
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

	m_treeRibbon.RegisterWindowClass(XTPResourceManager()->GetResourceHandle());
}

CXTPRibbonCustomizePage::~CXTPRibbonCustomizePage()
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		delete m_arrCategories[i];
	}

	// Conditional compilation padding issue workaround.
	delete &m_wndTitleIcon;
	delete &m_wndTitle;
}

void CXTPRibbonCustomizePage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPRibbonCustomizePage)
	DDX_Control(pDX, XTP_IDC_RIBBONCOMBO_CATEGORIES, m_cmbCategories);
	DDX_Control(pDX, XTP_IDC_RIBBONCOMBO_TABGROUP, m_cmbTabGroup);

	DDX_Control(pDX, XTP_IDC_RIBBONLIST_QUICKACCESS, m_treeRibbon);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_COMMANDS, m_treeCommands);

	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_REMOVE, m_btnRemove);

	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_NEWTAB, m_btnNewTab);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_NEWGROUP, m_btnNewGroup);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_RENAME, m_btnRename);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_RESET, m_btnReset);

	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_IMPORT, m_btnImport);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_UP, m_btnUp);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_DOWN, m_btnDown);
	//}}AFX_DATA_MAP
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonCustomizePage, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CXTPRibbonCustomizePage)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONCOMBO_CATEGORIES, OnCategoriesSelectionChanged)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_RESET, OnButtonReset)

	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_NEWTAB, OnButtonNewTab)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_NEWGROUP, OnButtonNewGroup)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_RENAME, OnButtonRename)

	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_DOWN, OnButtonDown)
	//}}AFX_MSG_MAP

	ON_NOTIFY(TVN_SELCHANGED, XTP_IDC_RIBBONLIST_COMMANDS, OnCommandsSelChanged)

	ON_NOTIFY(TVN_SELCHANGED, XTP_IDC_RIBBONLIST_QUICKACCESS, OnRibbonSelChanged)

	ON_NOTIFY(XTP_TVN_CHECKCHANGED, XTP_IDC_RIBBONLIST_QUICKACCESS, OnRibbonTreeCheckChanged)

	ON_CBN_DROPDOWN(XTP_IDC_RIBBONBUTTON_IMPORT, OnImportDropDown)

	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPRibbonBar* CXTPRibbonCustomizePage::GetRibbonBar()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, m_pCommandBars->GetMenuBar());
	ASSERT(pRibbonBar);

	return pRibbonBar;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizePage message handlers

BOOL CXTPRibbonCustomizePage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	ModifyStyle(0, WS_CLIPCHILDREN);

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		int nIndex = m_cmbCategories.AddString(m_arrCategories[i]->strCategory);
		m_cmbCategories.SetItemData(nIndex, XTPToULongPtr(i));

		if (i == 0)
			m_cmbCategories.SetCurSel(0);
	}

	m_treeCommands.m_pCommandBars = m_pCommandBars;
	m_treeCommands.m_bItemsTree   = TRUE;

	m_treeCommands.GetPaintManager()->m_bShowButtons = FALSE;

	m_treeRibbon.m_pCommandBars = m_pCommandBars;

	m_treeCommands.SetImageManager(m_pCommandBars->GetImageManager());
	CMDTARGET_ADDREF(m_pCommandBars->GetImageManager());

	m_treeRibbon.SetImageManager(m_pCommandBars->GetImageManager());
	CMDTARGET_ADDREF(m_pCommandBars->GetImageManager());

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

	OnCategoriesSelectionChanged();
	RefreshRibbonList();

	UpdateData(FALSE);

	if (m_pCommandBars->IsLayoutRTL())
	{
		m_treeCommands.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_treeRibbon.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_cmbCategories.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_cmbTabGroup.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}

	SetResize(XTP_IDC_RIBBONCOMBO_CATEGORIES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(XTP_IDC_RIBBONLIST_COMMANDS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONLIST_QUICKACCESS, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_BOTTOMRIGHT);

	SetResize(XTP_IDC_RIBBONBUTTON_RESET, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONBUTTON_NEWTAB, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONBUTTON_NEWGROUP, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONBUTTON_RENAME, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);

	SetResize(XTP_IDC_RIBBONBUTTON_ADD, CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));
	SetResize(XTP_IDC_RIBBONBUTTON_REMOVE, CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));

	SetResize(XTP_IDC_RIBBONBUTTON_IMPORT, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONSTATIC_CUSTOMIZATIONS, XTP_ANCHOR_BOTTOMCENTER,
			  XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONSTATIC_CUSTOMIZE, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(XTP_IDC_RIBBONCOMBO_TABGROUP, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);

	SetResize(XTP_IDC_RIBBONBUTTON_UP, CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 0.5));
	SetResize(XTP_IDC_RIBBONBUTTON_DOWN, CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 0.5));

	// GetDlgItem(XTP_IDC_RIBBONBUTTON_UP)->ShowWindow(SW_HIDE);
	// GetDlgItem(XTP_IDC_RIBBONBUTTON_DOWN)->ShowWindow(SW_HIDE);

	GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(FALSE);

	m_cmbTabGroup.AddString(XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_MAINTABS));
	m_cmbTabGroup.SetCurSel(0);

	m_btnImport.SetPushButtonStyle(xtpButtonDropDown);

	CString xamlUp, xamlDown;
	VERIFY(XTPResourceManager()->LoadXAML(xamlUp, XTP_XAML_ICON_UP));
	VERIFY(XTPResourceManager()->LoadXAML(xamlDown, XTP_XAML_ICON_DOWN));

	m_btnUp.EnableMarkup();
	m_btnUp.SetWindowText(xamlUp);
	m_btnUp.ModifyStyleEx(WS_EX_LAYOUTRTL, 0);

	m_btnDown.EnableMarkup();
	m_btnDown.SetWindowText(xamlDown);
	m_btnDown.ModifyStyleEx(WS_EX_LAYOUTRTL, 0);

#ifdef _XTP_INCLUDE_CONTROLS

	if (CXTPPropertyPage::m_pSheet)
	{
		XTPControlTheme controlTheme = CXTPPropertyPage::m_pSheet->GetTheme();

		m_btnAdd.SetTheme(controlTheme);
		m_btnRemove.SetTheme(controlTheme);

		m_btnNewTab.SetTheme(controlTheme);
		m_btnNewGroup.SetTheme(controlTheme);
		m_btnRename.SetTheme(controlTheme);
		m_btnReset.SetTheme(controlTheme);

		m_btnImport.SetTheme(controlTheme);

		m_btnUp.SetTheme(controlTheme);
		m_btnDown.SetTheme(controlTheme);

		m_cmbCategories.SetTheme(controlTheme);
		m_cmbTabGroup.SetTheme(controlTheme);

		m_treeRibbon.EnableTheme(controlTheme != xtpControlThemeDefault ? TRUE : FALSE);
		m_treeCommands.EnableTheme(controlTheme != xtpControlThemeDefault ? TRUE : FALSE);

		m_clrText = XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"),
								GetXtremeColor(COLOR_WINDOWTEXT));
	}

#endif

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPRibbonCustomizePage::RefreshRibbonList()
{
	m_treeRibbon.SetLockRedraw(TRUE);

	m_treeRibbon.ResetContent();

	CXTPCoreTreeItem* pLabelItem = m_treeRibbon.GetItems()->AddItem(
		XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_MAINTABS));
	pLabelItem->SetLabel(TRUE);

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();

	// remove capitalization if set
	BOOL bAllCaps = pRibbonBar->GetCommandBars()->IsAllCaps();
	pRibbonBar->GetCommandBars()->SetAllCaps(FALSE);

	for (int i = 0; i < pRibbonBar->GetTabCount(); i++)
	{
		CXTPRibbonTab* pTab = pRibbonBar->GetTab(i);

		CString strCaption(pTab->GetCustomizeCaption());
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItemTab = m_treeRibbon.GetItems()->AddItem(strCaption);
		pItemTab->ShowCheckbox(TRUE);
		pItemTab->SetChecked(pTab->IsVisible());
		pItemTab->SetItemData((DWORD_PTR)pTab);

		for (int g = 0; g < pTab->GetGroups()->GetCount(); g++)
		{
			CXTPRibbonGroup* pGroup = pTab->GetGroups()->GetAt(g);

			CString strGroupCaption = pGroup->GetCaption();
			XTPDrawHelpers()->StripMnemonics(strGroupCaption);

			CXTPCoreTreeItem* pItemGroup = pItemTab->GetChildren()->AddItem(strGroupCaption);
			pItemGroup->SetItemData((DWORD_PTR)pGroup);

			for (int c = 0; c < pGroup->GetCount(); c++)
			{
				CXTPControl* pControl = pGroup->GetAt(c);

				CString strControlCaption = pControl->GetCaption();
				XTPDrawHelpers()->StripMnemonics(strControlCaption);

				CXTPCoreTreeItem* pItemControl = pItemGroup->GetChildren()->AddItem(
					strControlCaption);
				pItemControl->SetItemData((DWORD_PTR)pControl);
				pItemControl->SetIconId(pControl->GetIconId());
			}
		}
	}

	// restore capitalization
	pRibbonBar->GetCommandBars()->SetAllCaps(bAllCaps);

	m_treeRibbon.SetLockRedraw(FALSE);

	OnRibbonSelectionChanged();
	OnCommandsSelectionChanged();
}

void CXTPRibbonCustomizePage::OnCategoriesSelectionChanged()
{
	m_treeCommands.GetItems()->RemoveAll();

	int nIndex = m_cmbCategories.GetCurSel();

	if (nIndex == LB_ERR)
		return;

	XTP_COMMANDBARS_CATEGORYINFO* pInfo = GetCategoryInfo((int)m_cmbCategories.GetItemData(nIndex));

	if (pInfo == NULL)
		return;

	m_treeCommands.SetLockRedraw(TRUE);

	for (int i = 0; i < pInfo->pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pInfo->pControls->GetAt(i);

		CString strCaption(pControl->GetCaption());
		CXTPPaintManager::StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItem = m_treeCommands.GetItems()->AddItem(strCaption);
		pItem->SetItemData((DWORD_PTR)pControl);
		pItem->SetIconId(pControl->GetIconId());
	}

	m_treeCommands.SetLockRedraw(FALSE);

	OnCommandsSelectionChanged();
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizePage::FindCategory(LPCTSTR strCategory) const
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		if (m_arrCategories[i]->strCategory.Compare(strCategory) == 0)
			return m_arrCategories[i];
	}
	return NULL;
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizePage::GetCategoryInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrCategories.GetSize())
		return m_arrCategories[nIndex];
	return NULL;
}

BOOL CXTPRibbonCustomizePage::AddCategory(LPCTSTR strCategory, CMenu* pMenu, BOOL bListSubItems)
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

BOOL CXTPRibbonCustomizePage::AddCategories(UINT nIDResource, BOOL bListSubItems)
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

BOOL CXTPRibbonCustomizePage::AddCategories(CXTPControls* pControls)
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

CXTPControls* CXTPRibbonCustomizePage::InsertCategory(LPCTSTR strCategory, int nIndex)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	if (!pInfo)
	{
		pInfo = new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pCommandBars);
		m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);
	}
	return pInfo->pControls;
}

CXTPControls* CXTPRibbonCustomizePage::GetControls(LPCTSTR strCategory)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	return pInfo ? pInfo->pControls : NULL;
}

void CXTPRibbonCustomizePage::OnButtonAdd()
{
	CXTPCoreTreeItem* pItem = m_treeCommands.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControlToAdd = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	if (!pControlToAdd)
		return;

	pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	if (pControl)
	{
		CXTPControl* pControlNew = pControl->GetRibbonGroup()->AddClone(
			pControlToAdd, pControl->GetRibbonGroup()->IndexOf(pControl) + 1);

		CString strCaption(pControlNew->GetCaption());
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItemControl =
			pItem->GetParentItems()->AddItemAfter(new CXTPCoreTreeItem(), pItem);
		pItemControl->SetCaption(strCaption);
		pItemControl->SetItemData((DWORD_PTR)pControlNew);
		pItemControl->SetIconId(pControlNew->GetIconId());

		m_treeRibbon.SetFocusedItem(pItemControl);

		m_pCommandBars->UpdateCommandBars();

		return;
	}

	CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());
	if (pGroup)
	{
		CXTPControl* pControlNew = pGroup->AddClone(pControlToAdd, -1);

		CString strCaption(pControlNew->GetCaption());
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItemControl = pItem->GetChildren()->AddItemAfter(new CXTPCoreTreeItem());
		pItemControl->SetCaption(strCaption);
		pItemControl->SetItemData((DWORD_PTR)pControlNew);
		pItemControl->SetIconId(pControlNew->GetIconId());

		m_treeRibbon.SetFocusedItem(pItemControl);

		m_pCommandBars->UpdateCommandBars();

		return;
	}
}

void CXTPRibbonCustomizePage::OnButtonRemove()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	if (pControl)
	{
		pControl->GetRibbonGroup()->Remove(pControl);

		pItem->Remove();

		m_pCommandBars->UpdateCommandBars();

		return;
	}

	CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());

	if (pGroup)
	{
		pGroup->GetGroups()->Remove(pGroup);

		pItem->Remove();

		m_pCommandBars->UpdateCommandBars();

		return;
	}

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (pTab)
	{
		pTab->Remove();

		pItem->Remove();

		m_pCommandBars->UpdateCommandBars();
	}
}

void CXTPRibbonCustomizePage::OnCommandsSelectionChanged()
{
	GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->EnableWindow(m_treeCommands.GetFocusedItem() != NULL);
}

void CXTPRibbonCustomizePage::OnRibbonSelectionChanged()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControl   = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());
	CXTPRibbonTab* pTab		= DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (pControl || pGroup || pTab)
	{
		GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(FALSE);
	}

	if (pTab)
	{
		if (pTab->GetIndex() == 0)
		{
			m_btnUp.EnableWindow(FALSE);
		}
		else
		{
			m_btnUp.EnableWindow(TRUE);
		}

		if (pTab->GetIndex() == pTab->GetTabManager()->GetItemCount() - 1)
		{
			m_btnDown.EnableWindow(FALSE);
		}
		else
		{
			m_btnDown.EnableWindow(TRUE);
		}
	}
	else
	{
		m_btnUp.EnableWindow(FALSE);
		m_btnDown.EnableWindow(FALSE);
	}
}

void CXTPRibbonCustomizePage::OnRibbonTreeCheckChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());
	if (pTab)
	{
		pTab->SetVisible(pItem->IsChecked());
	}
}

void CXTPRibbonCustomizePage::OnRibbonSelChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	OnRibbonSelectionChanged();
}

void CXTPRibbonCustomizePage::OnCommandsSelChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	OnCommandsSelectionChanged();
}

void CXTPRibbonCustomizePage::OnButtonNewTab()
{
	CXTPCoreTreeItem* pItemBefore = m_treeRibbon.GetFocusedItem();
	if (!pItemBefore)
		return;

	while (pItemBefore->GetParentItem() != NULL)
		pItemBefore = pItemBefore->GetParentItem();

	CXTPRibbonTab* pTabBefore = DYNAMIC_DOWNCAST(CXTPRibbonTab,
												 (CCmdTarget*)pItemBefore->GetItemData());
	if (!pTabBefore)
		return;

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();

	CXTPRibbonTab* pTab = pRibbonBar->InsertTab(pTabBefore->GetIndex() + 1,
												XTPResourceManager()->LoadString(
													XTP_IDS_RIBBONCUSTOMIZE_NEWTAB));

	CXTPCoreTreeItem* pItemTab = m_treeRibbon.GetItems()->AddItemAfter(new CXTPCoreTreeItem(),
																	   pItemBefore);

	pItemTab->SetCaption(pTab->GetCaption());
	pItemTab->ShowCheckbox(TRUE);
	pItemTab->SetChecked(pTab->IsVisible());
	pItemTab->SetItemData((DWORD_PTR)pTab);

	CXTPRibbonGroup* pGroup = pTab->AddGroup(
		XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_NEWGROUP));

	CXTPCoreTreeItem* pItemGroup = pItemTab->GetChildren()->AddItem(pGroup->GetCaption());
	pItemGroup->SetItemData((DWORD_PTR)pGroup);

	m_treeRibbon.SetFocusedItem(pItemGroup);

	m_treeRibbon.UpdateCommandBars();
}

void CXTPRibbonCustomizePage::OnButtonNewGroup()
{
	CXTPCoreTreeItem* pItemTab = m_treeRibbon.GetFocusedItem();
	if (!pItemTab)
		return;

	while (pItemTab->GetParentItem() != NULL)
		pItemTab = pItemTab->GetParentItem();

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItemTab->GetItemData());
	if (!pTab)
		return;

	CXTPRibbonGroup* pGroup = pTab->AddGroup(
		XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_NEWGROUP));

	CXTPCoreTreeItem* pItemGroup = pItemTab->GetChildren()->AddItem(pGroup->GetCaption());
	pItemGroup->SetItemData((DWORD_PTR)pGroup);

	m_treeRibbon.SetFocusedItem(pItemGroup);

	m_treeRibbon.UpdateCommandBars();
}

void CXTPRibbonCustomizePage::OnButtonRename()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	if (pItem)
	{
		CXTPControl* pControl = ControlFromItem(pItem);

		if (pControl)
		{
			CXTPRibbonRenameDialog dlg;
			dlg.SetName(pControl->GetCaption());

			if (dlg.DoModal() == IDOK)
			{
				CString strName = dlg.GetName();
				pControl->SetCaption(strName);

				XTPDrawHelpers()->StripMnemonics(strName);
				pItem->SetCaption(strName);

				m_treeRibbon.UpdateCommandBars();
			}

			return;
		}
	}

	if (pItem)
	{
		CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

		if (pTab)
		{
			CXTPRibbonRenameDialog dlg;
			dlg.SetName(pTab->GetCaption());

			if (dlg.DoModal() == IDOK)
			{
				CString strName = dlg.GetName();
				pTab->SetCaption(strName);

				XTPDrawHelpers()->StripMnemonics(strName);
				pItem->SetCaption(strName);

				m_treeRibbon.UpdateCommandBars();
			}

			return;
		}
	}

	if (pItem)
	{
		CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup,
												   (CCmdTarget*)pItem->GetItemData());

		if (pGroup)
		{
			CXTPRibbonRenameDialog dlg;
			dlg.SetName(pGroup->GetCaption());

			if (dlg.DoModal() == IDOK)
			{
				CString strName = dlg.GetName();
				pGroup->SetCaption(strName);

				XTPDrawHelpers()->StripMnemonics(strName);
				pItem->SetCaption(strName);

				m_treeRibbon.UpdateCommandBars();
			}

			return;
		}
	}
}

void CXTPRibbonCustomizePage::OnButtonReset()
{
	CString strPrompt = XTPResourceManager()->LoadString(
		XTP_IDS_RIBBONCUSTOMIZE_CONFIRMRIBBONRESET);

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

	m_pCommandBars->GetSite()->SendMessage(WM_XTP_CUSTOMIZATION_RIBBONRESET);

	RefreshRibbonList();

	CXTPPropertySheet* pSheet = DYNAMIC_DOWNCAST(CXTPPropertySheet, GetParent());
	ASSERT(NULL != pSheet);
	pSheet->UpdatePages();
}

void CXTPRibbonCustomizePage::OnImportDropDown()
{
	CMenu menu;
	XTPResourceManager()->LoadMenu(&menu, XTP_IDM_RIBBONCUSTOMIZE_IMPORT);

	CRect rc;
	GetDlgItem(XTP_IDC_RIBBONBUTTON_IMPORT)->GetWindowRect(&rc);

	UINT nCommand = XTPToUInt(TrackPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), TPM_RETURNCMD,
											 rc.left, rc.bottom, 0, m_hWnd, rc));

	if (nCommand == XTP_ID_RIBBONCUSTOMIZE_IMPORT)
	{
		CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");
		CFileDialog fd(TRUE, _T("xml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
		if (fd.DoModal() == IDOK)
		{
			CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
			if (!px.LoadFromFile(fd.GetPathName()))
				return;

			CXTPRibbonBar* pRibbonBar		= GetRibbonBar();
			CXTPCommandBar* pSystemPopupBar = pRibbonBar->GetSystemButton()
												  ? pRibbonBar->GetSystemButton()->GetCommandBar()
												  : NULL;
			CMDTARGET_ADDREF(pSystemPopupBar);

			XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
			param.bSaveOriginalControls = FALSE;
			param.bSerializeControls	= TRUE;
			param.bSerializeOptions		= FALSE;
			param.bSerializeLayout		= FALSE;
			param.bSerializeActions		= TRUE;

			m_pCommandBars->DoPropExchange(&px, &param);

			m_pCommandBars->UpdateCommandBars();

			if (pSystemPopupBar)
			{
				GetRibbonBar()->GetSystemButton()->SetCommandBar(pSystemPopupBar);
				CMDTARGET_RELEASE(pSystemPopupBar);
			}

			RefreshRibbonList();
		}
	}
	else if (nCommand == XTP_ID_RIBBONCUSTOMIZE_EXPORT)
	{
		CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");
		CFileDialog fd(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
		if (fd.DoModal() == IDOK)
		{
			CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));

			XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
			param.bSaveOriginalControls = FALSE;
			param.bSerializeControls	= TRUE;
			param.bSerializeOptions		= FALSE;
			param.bSerializeLayout		= FALSE;
			param.bSerializeActions		= TRUE;

			m_pCommandBars->DoPropExchange(&px, &param);

			px.SaveToFile(fd.GetPathName());
		}
	}
}

CXTPCoreTreeItem* CXTPRibbonCustomizePage::FindItem(CCmdTarget* pItemData,
													CXTPCoreTreeItems* pItems /* = NULL*/)
{
	if (pItems == NULL)
		pItems = m_treeRibbon.GetItems();

	for (CXTPCoreTreeItem* pItem = pItems->GetFirstItem(); pItem != NULL;
		 pItem					 = pItem->GetNextSiblingItem())
	{
		if (pItem->GetItemData() == (DWORD_PTR)pItemData)
			return pItem;

		if (pItem->HasChildren())
		{
			CXTPCoreTreeItem* p = FindItem(pItemData, pItem->GetChildren());
			if (p)
				return p;
		}
	}
	return NULL;
}

void CXTPRibbonCustomizePage::OnButtonUp()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (!pTab)
		return;

	pTab->Move(pTab->GetIndex() - 1);

	m_pCommandBars->UpdateCommandBars();

	RefreshRibbonList();

	pItem = FindItem(pTab, m_treeRibbon.GetItems());
	if (pItem)
		m_treeRibbon.SetFocusedItem(pItem);
}

void CXTPRibbonCustomizePage::OnButtonDown()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (!pTab)
		return;

	pTab->Move(pTab->GetIndex() + 1);

	m_pCommandBars->UpdateCommandBars();

	RefreshRibbonList();

	pItem = FindItem(pTab, m_treeRibbon.GetItems());
	if (pItem)
		m_treeRibbon.SetFocusedItem(pItem);
}

HBRUSH CXTPRibbonCustomizePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
