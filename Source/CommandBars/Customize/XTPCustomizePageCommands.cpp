// XTPCustomizePageCommands.cpp : implementation of the CXTPCustomizeCommandsPage class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
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
#include "Controls/Scroll/XTPScrollBar.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPMouseManager.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"

#include "Controls/Dialog/XTPDialogBase.h"
#include "Controls/Dialog/XTPPropertyPage.h"
#include "Controls/Dialog/XTPPropertySheet.h"
#include "CommandBars/Customize/XTPCustomizeTools.h"
#include "CommandBars/Customize/XTPCustomizeSheet.h"
#include "CommandBars/Customize/XTPCustomizePageCommands.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

XTP_COMMANDBARS_CATEGORYINFO::XTP_COMMANDBARS_CATEGORYINFO(LPCTSTR _strCategory,
														   CXTPCommandBars* pCommandBars)
{
	strCategory = _strCategory;
	pControls   = new CXTPControls;
	pControls->SetCommandBars(pCommandBars);
}
XTP_COMMANDBARS_CATEGORYINFO::~XTP_COMMANDBARS_CATEGORYINFO()
{
	pControls->InternalRelease();
}

void CXTPCustomizeCommandsListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc(lpDrawItemStruct->rcItem);
	CXTPControl* pControl = (CXTPControl*)lpDrawItemStruct->itemData;
	if (pControl)
	{
		BOOL bSelected				  = (0 != (lpDrawItemStruct->itemState & ODS_SELECTED));
		CXTPCommandBars* pCommandBars = m_pCommandBars;
		pCommandBars->GetPaintManager()->DrawListBoxControl(pDC, pControl, rc, bSelected, TRUE,
															pCommandBars);
	}
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"

#ifdef _XTP_INCLUDE_CONTROLS
BEGIN_MESSAGE_MAP(CXTPCustomizeCommandsListBox, CXTPListBox)
#else
BEGIN_MESSAGE_MAP(CXTPCustomizeCommandsListBox, CListBox)
#endif
	//{{AFX_MSG_MAP(CXTPCustomizeCommandsPage)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPCustomizeCommandsListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPCustomizeCommandsListBox::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CXTPBufferDC memDC(dc);

	m_pCommandBars->GetPaintManager()->DrawListBoxControl(&memDC, NULL, rc, FALSE, TRUE,
														  m_pCommandBars);

#pragma warning(push)
// warning C4097: typedef-name '...' used as synonym for class-name '...'
#pragma warning(disable : 4097)
	BaseType::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
#pragma warning(pop)
}

LRESULT CXTPCustomizeCommandsListBox::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	CXTPClientRect rc(this);

	m_pCommandBars->GetPaintManager()->DrawListBoxControl(pDC, NULL, rc, FALSE, TRUE,
														  m_pCommandBars);
#pragma warning(push)
// warning C4097: typedef-name '...' used as synonym for class-name '...'
#pragma warning(disable : 4097)
	return BaseType::DefWindowProc(WM_PRINTCLIENT, wParam, lParam);
#pragma warning(pop)
}

void CXTPCustomizeCommandsListBox::OnSetFocus(CWnd* pOldWnd)
{
#pragma warning(push)
// warning C4097: typedef-name '...' used as synonym for class-name '...'
#pragma warning(disable : 4097)
	BaseType::OnSetFocus(pOldWnd);
#pragma warning(pop)

	if ((GetKeyState(VK_TAB) < 0 || GetKeyState(VK_MENU) < 0) && GetCurSel() == -1)
		SetCurSel(0);
}

void CXTPCustomizeCommandsListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = m_pCommandBars;
	ASSERT(pCommandBars);
	if (!pCommandBars)
		return;

	pCommandBars->SetDragControl(NULL);

	BOOL bOutside;
	int nIndex			  = (int)ItemFromPoint(point, bOutside);
	CXTPControl* pControl = NULL;
	if (!bOutside && m_bDragable)
	{
		if (SetCurSel(nIndex) != LB_ERR)
		{
			// Already selected, start drag operation
			pControl = (CXTPControl*)GetItemData(nIndex);

			// since we are eating the message, if the selection has changed, notify the owner.
			CWnd* pWndOwner = GetOwner();
			if (pWndOwner)
			{
				SetFocus();

				pWndOwner->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE),
									   (LPARAM)m_hWnd);
			}

			// Special case for New Menu
			if (pControl && pControl->GetCommandBar()
				&& pControl->GetCommandBar()->GetControlCount() == 0)
			{
				pControl = pControl->Clone(TRUE);

				CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
				pDropSource->DoDragDrop(pControl, TRUE);

				pControl->InternalRelease();
				return;
			}

			if (pControl)
			{
				CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
				pDropSource->DoDragDrop(pControl, TRUE);

				return;
			}
		}
	}

#pragma warning(push)
// warning C4097: typedef-name '...' used as synonym for class-name '...'
#pragma warning(disable : 4097)

	// process as a click first
	BaseType::OnLButtonDown(nFlags, point);

#pragma warning(pop)
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeCommandsPage property page

IMPLEMENT_DYNCREATE(CXTPCustomizeCommandsPage, CXTPPropertyPage)

CXTPCustomizeCommandsPage::CXTPCustomizeCommandsPage(CXTPCustomizeSheet* pSheet)
	: CXTPPropertyPage(XTP_IDD_PAGE_COMMANDS)
	, m_pSheet(pSheet)
{
	//{{AFX_DATA_INIT(CXTPCustomizeCommandsPage)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CXTPCustomizeCommandsPage::~CXTPCustomizeCommandsPage()
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		delete m_arrCategories[i];
	}
}

void CXTPCustomizeCommandsPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPCustomizeCommandsPage)
	DDX_Control(pDX, XTP_IDC_LIST_COMMANDS, m_lstCommands);
#ifdef _XTP_INCLUDE_CONTROLS
	DDX_Control(pDX, XTP_IDC_LIST_CATEGORIES_EX, m_lstCategories);
#else
	DDX_Control(pDX, XTP_IDC_LIST_CATEGORIES, m_lstCategories);
#endif
	//}}AFX_DATA_MAP
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPCustomizeCommandsPage::FindCategory(LPCTSTR strCategory) const
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		if (m_arrCategories[i]->strCategory.Compare(strCategory) == 0)
			return m_arrCategories[i];
	}
	return NULL;
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPCustomizeCommandsPage::GetCategoryInfo(int nIndex) const
{
	if (nIndex >= 0 && nIndex < m_arrCategories.GetSize())
		return m_arrCategories[nIndex];
	return NULL;
}

BOOL CXTPCustomizeCommandsPage::AddCategory(LPCTSTR strCategory, CMenu* pMenu, BOOL bListSubItems)
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

BOOL CXTPCustomizeCommandsPage::_LoadToolBar(CXTPCommandBar* pCommandBar, UINT nIDResource)
{
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);

	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		WORD* items()
		{
			return (WORD*)(this + 1);
		}
	};

	ASSERT(lpszResourceName != NULL);
	if (!lpszResourceName)
		return FALSE;

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	if (!hInst)
		return FALSE;

	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (!hRsrc)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (!hGlobal)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (!pData)
		return FALSE;

	ASSERT(pData->wVersion == 1);
	ASSERT(pData->wWidth == 16);

	int i;
	UINT* pItems = new UINT[pData->wItemCount];

	for (i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];

	BOOL bResult = pCommandBar->SetButtons(pItems, pData->wItemCount);

	delete[] pItems;

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bResult;
}

BOOL CXTPCustomizeCommandsPage::AddToolbarCategory(LPCTSTR strCategory, UINT nIDResource)
{
	CXTPControls* pCategoryControls = InsertCategory(strCategory);
	ASSERT(pCategoryControls);
	if (!pCategoryControls)
		return FALSE;

	CXTPCommandBar* pCommandBar = (CXTPCommandBar*)CXTPCommandBar::CreateObject();
	pCommandBar->SetCommandBars(m_pSheet->GetCommandBars());

	if (!_LoadToolBar(pCommandBar, nIDResource))
	{
		pCommandBar->InternalRelease();
		return FALSE;
	}

	for (int i = 0; i < pCommandBar->GetControlCount(); i++)
	{
		pCategoryControls->AddClone(pCommandBar->GetControl(i));
	}
	pCommandBar->InternalRelease();

	return TRUE;
}

BOOL CXTPCustomizeCommandsPage::AddCategories(UINT nIDResource, BOOL bListSubItems)
{
	CMenu menu;
	if (!XTPResourceManager()->LoadMenu(&menu, nIDResource))
		return FALSE;

	int nCount = menu.GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		CString strCategory;
		if (XTPResourceManager()->GetMenuLocaleString(&menu, XTPToUInt(i), strCategory,
													  MF_BYPOSITION)
			> 0)
		{
			CMenu* pMenu = menu.GetSubMenu(i);
			if (pMenu)
			{
				CXTPPaintManager::StripMnemonics(strCategory);

				int iPos = strCategory.Find(_T('\t'));
				if (iPos > 0)
				{
					strCategory.ReleaseBuffer(iPos);
				}

				if (!AddCategory(strCategory, pMenu, bListSubItems))
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CXTPCustomizeCommandsPage::AddCategories(CXTPControls* pControls)
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

BOOL CXTPCustomizeCommandsPage::InsertBuiltInMenus(UINT nIDResourceMenu, UINT nIDResourceCategory,
												   int nIndex)
{
	CMenu menu;
	if (!XTPResourceManager()->LoadMenu(&menu, nIDResourceMenu))
		return FALSE;

	CString strCategory;
	XTPResourceManager()->LoadString(&strCategory, nIDResourceCategory);

	ASSERT(FindCategory(strCategory) == NULL);

	XTP_COMMANDBARS_CATEGORYINFO* pInfo =
		new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pSheet->GetCommandBars());
	m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);

	int nCount = menu.GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		pInfo->pControls->AddMenuItem(&menu, i);
	}
	return TRUE;
}

void CXTPCustomizeCommandsPage::InsertNewMenuCategory(UINT nIDResource, int nIndex)
{
	CString strCategory;
	XTPResourceManager()->LoadString(&strCategory, nIDResource);

	ASSERT(FindCategory(strCategory) == NULL);

	XTP_COMMANDBARS_CATEGORYINFO* pInfo =
		new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pSheet->GetCommandBars());
	m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);

	CXTPControl* pControl = pInfo->pControls->Add(xtpControlPopup, 0);
	pControl->SetCaption(strCategory);
}

CXTPControls* CXTPCustomizeCommandsPage::InsertCategory(LPCTSTR strCategory, int nIndex)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	if (!pInfo)
	{
		pInfo = new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pSheet->GetCommandBars());
		m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);
	}
	return pInfo->pControls;
}

CXTPControls* CXTPCustomizeCommandsPage::GetControls(LPCTSTR strCategory)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	return pInfo ? pInfo->pControls : NULL;
}

void CXTPCustomizeCommandsPage::InsertAllCommandsCategory(UINT nIDResource, int nIndex,
														  BOOL bSortCommands)
{
	CString strCategory;
	XTPResourceManager()->LoadString(&strCategory, nIDResource);

	ASSERT(FindCategory(strCategory) == NULL);

	XTP_COMMANDBARS_CATEGORYINFO* pInfo =
		new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pSheet->GetCommandBars());

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		CXTPControls* pControls = m_arrCategories[i]->pControls;
		for (int j = 0; j < pControls->GetCount(); j++)
		{
			pInfo->pControls->AddClone(pControls->GetAt(j));
		}
	}

	if (bSortCommands)
		pInfo->pControls->Sort();

	m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCustomizeCommandsPage, CXTPPropertyPage)
//{{AFX_MSG_MAP(CXTPCustomizeCommandsPage)
#ifdef _XTP_INCLUDE_CONTROLS
	ON_LBN_SELCHANGE(XTP_IDC_LIST_CATEGORIES_EX, OnCategoriesSelectionChanged)
#else
	ON_LBN_SELCHANGE(XTP_IDC_LIST_CATEGORIES, OnCategoriesSelectionChanged)
#endif
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeCommandsPage message handlers

BOOL CXTPCustomizeCommandsPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		int nIndex = m_lstCategories.AddString(m_arrCategories[i]->strCategory);
		m_lstCategories.SetItemData(nIndex, XTPToDWORD(i));

		if (i == 0)
			m_lstCategories.SetCurSel(0);
	}

	m_lstCategories.SetItemHeight(0, XTPToUInt(::GetSystemMetrics(SM_CYMENU)));

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	CSize sz = pCommandBars->GetPaintManager()->DrawListBoxControl(NULL, NULL, CRect(0, 0, 0, 0),
																   FALSE, FALSE, pCommandBars);
	m_lstCommands.SetItemHeight(-1, XTPToUIntChecked(sz.cy));

	m_lstCommands.m_pCommandBars = m_pSheet->GetCommandBars();
	m_lstCommands.m_bDragable	= TRUE;

	if (pCommandBars->IsLayoutRTL())
	{
		m_lstCommands.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_lstCategories.ModifyStyleEx(0, WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR | WS_EX_RTLREADING);
	}

	m_lstCommands.ModifyStyle(LBS_OWNERDRAWVARIABLE, 0);

	pCommandBars->GetMouseManager()->AddTrustedWindow(m_hWnd);

	OnCategoriesSelectionChanged();

#ifdef _XTP_INCLUDE_CONTROLS
	SetResize(XTP_IDC_LIST_CATEGORIES_EX, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
#else
	SetResize(XTP_IDC_LIST_CATEGORIES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
#endif
	SetResize(XTP_IDC_LIST_COMMANDS, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_BOTTOMRIGHT);

#ifdef _XTP_INCLUDE_CONTROLS
	XTPControlTheme controlTheme = m_pSheet->GetTheme();

	m_lstCategories.SetTheme(controlTheme);
	m_lstCommands.SetTheme(controlTheme);

	m_clrText = XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"),
							GetXtremeColor(COLOR_WINDOWTEXT));

	XTP_SAFE_CALL1(GetDlgItem(XTP_IDC_LIST_CATEGORIES), ShowWindow(SW_HIDE));
#else
	XTP_SAFE_CALL1(GetDlgItem(XTP_IDC_LIST_CATEGORIES_EX), ShowWindow(SW_HIDE));
#endif

	return FALSE;
}

void CXTPCustomizeCommandsPage::OnDestroy()
{
	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();

	pCommandBars->GetMouseManager()->RemoveTrustedWindow(m_hWnd);

	CXTPPropertyPage::OnDestroy();
}

void CXTPCustomizeCommandsPage::OnCategoriesSelectionChanged()
{
	m_lstCommands.ResetContent();

	int nIndex = m_lstCategories.GetCurSel();

	if (nIndex == LB_ERR)
		return;

	XTP_COMMANDBARS_CATEGORYINFO* pInfo = GetCategoryInfo(
		XTPToIntChecked(m_lstCategories.GetItemData(nIndex)));

	if (pInfo == NULL)
		return;

	for (int i = 0; i < pInfo->pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pInfo->pControls->GetAt(i);
		if (!pControl->IsVisible(xtpHideWrap))
			continue;

		int nItemIndex = m_lstCommands.InsertString(m_lstCommands.GetCount(), _T(""));
		if (0 <= nItemIndex)
			m_lstCommands.SetItemDataPtr(nItemIndex, pControl);
	}
}

HBRUSH CXTPCustomizeCommandsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
