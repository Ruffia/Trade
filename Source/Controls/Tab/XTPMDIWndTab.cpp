// XTPMDIWndTab.cpp : implementation file
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Tab/XTPTabBase.h"
#include "Controls/Tab/XTPTabCtrlButtons.h"
#include "Controls/Tab/XTPMDIWndTab.h"
#include "Controls/Tab/XTPTabBaseTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMDIClientWnd

CXTPMDIClientWnd::CXTPMDIClientWnd(CXTPMDIWndTab* pMDIWndTab)
{
	ASSERT(pMDIWndTab);
	m_pMDIWndTab = pMDIWndTab;
	m_iBorderGap = pMDIWndTab->m_iBorderGap + ::GetSystemMetrics(SM_CXSIZEFRAME);
}

CXTPMDIClientWnd::~CXTPMDIClientWnd()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPMDIClientWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPMDIClientWnd)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MDICREATE, OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY, OnMDIDestroy)
	ON_MESSAGE(WM_MDIACTIVATE, OnMDIActivate)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPMDIClientWnd message handlers

LRESULT CXTPMDIClientWnd::OnMDICreate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	m_pMDIWndTab->OnMDICreate((HWND)lResult);
	return lResult;
}

LRESULT CXTPMDIClientWnd::OnMDIDestroy(WPARAM wParam, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	m_pMDIWndTab->OnMDIDestroy((HWND)wParam);
	return lResult;
}

LRESULT CXTPMDIClientWnd::OnMDIActivate(WPARAM wParam, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	m_pMDIWndTab->OnMDIActivate((HWND)wParam);
	return lResult;
}

void CXTPMDIClientWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (::IsWindow(m_pMDIWndTab->m_hWnd) && m_pMDIWndTab->IsWindowVisible())
	{
		m_pMDIWndTab->MoveWindow(lpClientRect);

		DWORD dwStyle = m_pMDIWndTab->GetStyle();

		CRect rcTab;
		m_pMDIWndTab->GetItemRect(m_pMDIWndTab->GetCurSel(), &rcTab);

		int cy = rcTab.Height() * m_pMDIWndTab->GetRowCount();
		int cx = rcTab.Width() * m_pMDIWndTab->GetRowCount();

		// vertical tabs
		if (dwStyle & TCS_VERTICAL)
		{
			// Right
			if (dwStyle & TCS_RIGHT)
			{
				lpClientRect->top += m_iBorderGap;
				lpClientRect->left += m_iBorderGap;
				lpClientRect->right -= cx + m_iBorderGap + XTP_DPI_X(2);
				lpClientRect->bottom -= m_iBorderGap;
			}

			// Left
			else
			{
				lpClientRect->top += m_iBorderGap;
				lpClientRect->left += cx + m_iBorderGap + XTP_DPI_X(2);
				lpClientRect->right -= m_iBorderGap;
				lpClientRect->bottom -= m_iBorderGap;
			}
		}

		// horizontal tabs
		else
		{
			// Bottom
			if (dwStyle & TCS_BOTTOM)
			{
				lpClientRect->top += m_iBorderGap;
				lpClientRect->left += m_iBorderGap;
				lpClientRect->right -= m_iBorderGap;
				lpClientRect->bottom -= cy + m_iBorderGap + XTP_DPI_Y(2);
			}

			// Top
			else
			{
				lpClientRect->top += cy + m_iBorderGap + XTP_DPI_Y(2);
				lpClientRect->left += m_iBorderGap;
				lpClientRect->right -= m_iBorderGap;
				lpClientRect->bottom -= m_iBorderGap;
			}
		}
	}

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMDIWndTab

CXTPMDIWndTab::CXTPMDIWndTab()
{
	m_bNoIcons		  = FALSE;
	m_iBorderGap	  = XTP_DPI_X(5);
	m_nPos			  = 0;
	m_nDefCmd		  = 0;
	m_popupMenuID	 = 0;
	m_hActiveChild	= NULL;
	m_pMDIClientWnd   = NULL;
	m_pMDIFrameWnd	= NULL;
	m_bXPBorder		  = false;
	m_dwInitSignature = 1;

	ImplAttach(this);
}

CXTPMDIWndTab::~CXTPMDIWndTab()
{
	SAFE_DELETE(m_pMDIClientWnd);

	while (!m_arMDIChildern.IsEmpty())
	{
		MDICHILD* pMDIChild = m_arMDIChildern.RemoveHead();
		SAFE_DELETE(pMDIChild);
	}
}

IMPLEMENT_DYNAMIC(CXTPMDIWndTab, CTabCtrl)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPMDIWndTab, CTabCtrl)
	//{{AFX_MSG_MAP(CXTPMDIWndTab)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_COMMAND(XTP_IDC_TAB_CLOSE, OnTabClose)
	ON_TABCTRL_REFLECT()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPMDIWndTab message handlers

BOOL CXTPMDIWndTab::Init()
{
	if (!CXTPMDIWndTabBase::Init())
		return FALSE;

	ShowNavButtons(xtpNavBtnShowAll);

	int cx = ::GetSystemMetrics(SM_CXSMICON);
	int cy = ::GetSystemMetrics(SM_CYSMICON);

	if (!m_imageList.Create(cx, cy, ILC_COLOR16 | ILC_MASK, 0, 1))
	{
		TRACE0("Unable to create CXTPMDIWndTab image list.\n");
		return FALSE;
	}

	SetImageList(&m_imageList);
	InitializeFont();

	return TRUE;
}

BOOL CXTPMDIWndTab::Install(CMDIFrameWnd* pMDIFrameWnd,
							DWORD dwStyle /*= TCS_BOTTOM | TCS_HOTTRACK*/,
							BOOL bNoIcons /*= FALSE*/)
{
	ASSERT(pMDIFrameWnd);					   // must be valid.
	ASSERT_KINDOF(CMDIFrameWnd, pMDIFrameWnd); // must be MDI frame window.

	m_bNoIcons	 = bNoIcons;
	m_pMDIFrameWnd = pMDIFrameWnd;
	m_iBorderGap   = XTP_DPI_X(m_pTheme->GetThemeStyle() != xtpControlThemeOffice2000 ? 1 : 5);

	dwStyle |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN;

	if (!Create(dwStyle, CRect(0, 0, 0, 0), m_pMDIFrameWnd, AFX_IDC_TAB_CONTROL))
	{
		TRACE0("Unable to create CXTPMDIWndTab control.\n");
		return FALSE;
	}

	m_pMDIClientWnd = new CXTPMDIClientWnd(this);
	ASSERT(m_pMDIClientWnd != NULL);

	if (!m_pMDIClientWnd->SubclassWindow(m_pMDIFrameWnd->m_hWndMDIClient))
		return FALSE;

	// Retrieve the current active MDI child window.
	CMDIChildWnd* pActiveChild = (CMDIChildWnd*)m_pMDIFrameWnd->MDIGetActive();
	if (pActiveChild != NULL)
	{
		// Get the first top-level window in the list.
		CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pActiveChild->GetWindow(GW_HWNDLAST);
		while (pChildFrame != NULL)
		{
			// Insert the tab and get the next frame on the window manager's list
			InsertTabWnd(pChildFrame, FALSE);
			pChildFrame = (CMDIChildWnd*)pChildFrame->GetWindow(GW_HWNDPREV);
		}
	}

	// Force parent frame to re-adjust layout.
	m_pMDIFrameWnd->RecalcLayout();

	return TRUE;
}

BOOL CXTPMDIWndTab::UnInstall()
{
	// Remove all of the items from the child array.
	while (!m_arMDIChildern.IsEmpty())
	{
		MDICHILD* pMDIChild = m_arMDIChildern.RemoveHead();
		SAFE_DELETE(pMDIChild);
	}

	CXTPTabCtrlButtons* pNavBtns = GetButtons();
	if (pNavBtns && pNavBtns->GetSafeHwnd())
	{
		pNavBtns->UnSubclassTabButtons();
	}

	if (::IsWindow(m_hWnd) && m_pMDIFrameWnd)
	{
		// Unsubclass the MDI client and free the dynamic memory.
		m_pMDIClientWnd->UnsubclassWindow();
		SAFE_DELETE(m_pMDIClientWnd);

		// Force parent frame to re-adjust layout.
		m_pMDIFrameWnd->RecalcLayout();

		// Destroy this window.
		return DestroyWindow();
	}

	return FALSE;
}

void CXTPMDIWndTab::InsertTabWnd(CMDIChildWnd* pChildWnd, BOOL bRecalcLayout /*= TRUE*/)
{
	ASSERT(pChildWnd); // must be valid.

	// Try to locate the icon for the MDI window by calling WM_GETICON
	// first, this will give us the correct icon if the user has called
	// SetIcon(...) for the child frame.
	HICON hIcon = (HICON)::SendMessage(pChildWnd->m_hWnd, WM_GETICON, FALSE, 0);

	// If the icon returned is NULL, then try using GCLP_HICONSM to get the
	// document icon for the child frame
	if (hIcon == NULL)
	{
		hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(pChildWnd->m_hWnd, GCLP_HICONSM);
	}

	// If no icon was found, then use the default windows logo icon.
	if (hIcon == NULL)
	{
		hIcon = AfxGetApp()->LoadStandardIcon(IDI_WINLOGO);
	}
	m_imageList.Add(hIcon);

	// Get the window text for the frame.
	CString strWindowText = GetChildWndText(pChildWnd->GetSafeHwnd());

	int iItem = GetItemCount();

	MDICHILD* pMDIChild = new MDICHILD;
	ASSERT(pMDIChild != NULL);

	pMDIChild->iItem   = iItem;
	pMDIChild->hWnd	= pChildWnd->GetSafeHwnd();
	pMDIChild->strItem = strWindowText;

	m_arMDIChildern.AddTail(pMDIChild);

	if (m_pTheme->GetThemeStyle() != xtpControlThemeOffice2000)
	{
		OnAddPadding(strWindowText);
	}

	// Insert the new item into the tab control.
	TC_ITEM tci;
	tci.mask	   = TCIF_IMAGE | TCIF_PARAM | TCIF_TEXT;
	tci.pszText	= (TCHAR*)(LPCTSTR)strWindowText;
	tci.cchTextMax = strWindowText.GetLength();
	tci.iImage	 = m_imageList.GetImageCount() - 1;
	tci.lParam	 = (LPARAM)pMDIChild;

	if (m_bNoIcons)
	{
		tci.iImage = -1;
	}

	InsertItem(iItem, &tci);

	if (m_pMDIFrameWnd->MDIGetActive() == pChildWnd)
	{
		SetCurSel(iItem);
	}

	// defer request to complete initialization
	RecalcLayout(bRecalcLayout);
}

void CXTPMDIWndTab::RecalcLayout(BOOL bDelayRecalc)
{
	// refresh labels
	RefreshTabLabels();

	// Recalc frame layout.
	if (bDelayRecalc)
	{
		m_pMDIFrameWnd->DelayRecalcLayout();
	}
	else
	{
		m_pMDIFrameWnd->RecalcLayout();
	}
}

void CXTPMDIWndTab::RefreshIndexes()
{
	int iItem;
	for (iItem = 0; iItem < GetItemCount(); ++iItem)
	{
		TC_ITEM tci;
		tci.mask = TCIF_PARAM;

		if (GetItem(iItem, &tci))
		{
			MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
			ASSERT(pMDIChild != NULL);

			if (pMDIChild)
				pMDIChild->iItem = iItem;
		}
	}
}

void CXTPMDIWndTab::OnMDICreate(HWND hWnd)
{
	CMDIChildWnd* pActiveChild = (CMDIChildWnd*)CWnd::FromHandle(hWnd);
	if (pActiveChild != NULL)
	{
		InsertTabWnd(pActiveChild);
	}

	RefreshIndexes();
}

void CXTPMDIWndTab::OnMDIDestroy(HWND hWnd)
{
	// Loop through all of the tabs to find the view that has closed.
	int iItem;
	for (iItem = 0; iItem < GetItemCount(); ++iItem)
	{
		// Get the item data for this tab.
		TC_ITEM tci;
		tci.mask = TCIF_PARAM;

		if (GetItem(iItem, &tci))
		{
			MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
			ASSERT(pMDIChild != NULL);

			// If it is the closed window remove it and exit loop.
			if (pMDIChild && (hWnd == pMDIChild->hWnd))
			{
				POSITION pos = m_arMDIChildern.Find(pMDIChild);
				if (pos)
				{
					m_arMDIChildern.RemoveAt(pos);
					SAFE_DELETE(pMDIChild);
				}

				DeleteItem(iItem);
				break;
			}
		}
	}

	RefreshIndexes();
	RecalcLayout(TRUE);
}

void CXTPMDIWndTab::OnMDIActivate(HWND hWnd)
{
	// Loop through all of the tabs to find the view that has activated.
	int iItem;
	for (iItem = 0; iItem < GetItemCount(); ++iItem)
	{
		// Get the item data for this tab.
		TC_ITEM tci;
		tci.mask = TCIF_PARAM;

		if (GetItem(iItem, &tci))
		{
			MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
			ASSERT(pMDIChild != NULL);

			// If it is the activated window select it and exit loop.
			if (pMDIChild && (hWnd == pMDIChild->hWnd))
			{
				SetCurSel(iItem);
				break;
			}
		}
	}
}

bool CXTPMDIWndTab::SetTabIcon(int iIndex, HICON& hIcon)
{
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	if (GetItem(iIndex, &tci))
	{
		MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
		ASSERT(pMDIChild != NULL);
		if (!pMDIChild)
			return false;

		return SetTabIcon(pMDIChild->hWnd, hIcon);
	}

	return false;
}

bool CXTPMDIWndTab::SetTabIcon(HWND hChildWnd, HICON& hIcon)
{
	if (!hIcon || !::IsWindow(hChildWnd))
	{
		return false;
	}

	m_mapTabIcons.SetAt(hChildWnd, hIcon);
	return true;
}

bool CXTPMDIWndTab::SetTabLabel(int iIndex, CString& strLabel)
{
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	if (GetItem(iIndex, &tci))
	{
		MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
		ASSERT(pMDIChild != NULL);
		if (!pMDIChild)
			return false;

		return SetTabLabel(pMDIChild->hWnd, strLabel);
	}

	return false;
}

bool CXTPMDIWndTab::SetTabLabel(HWND hChildWnd, CString& strLabel)
{
	if (strLabel.IsEmpty() || !::IsWindow(hChildWnd))
	{
		return false;
	}

	m_mapTabLabels.SetAt(hChildWnd, strLabel);
	return true;
}

CString CXTPMDIWndTab::GetChildWndText(HWND hWnd) const
{
	// Check to see if the user has defined a label for the tab first.
	CString strTitle;
	if (m_mapTabLabels.Lookup(hWnd, strTitle))
	{
		return strTitle;
	}

	// Get a pointer to the frame window.
	CMDIChildWnd* pChildFrame = (CMDIChildWnd*)CWnd::FromHandle(hWnd);
	if (!::IsWindow(pChildFrame->GetSafeHwnd()))
	{
		return _T("");
	}

	// Get the window text for the frame and use it for the tab label.
	pChildFrame->GetWindowText(strTitle);

	// If the string is empty the document's title.
	if (strTitle.IsEmpty())
	{
		CWnd* pChildWnd = pChildFrame->GetWindow(GW_CHILD);
		while (pChildWnd)
		{
			if (pChildWnd->IsKindOf(RUNTIME_CLASS(CView)))
			{
				// Get a pointer to the view's document.
				CDocument* pDoc = ((CView*)pChildWnd)->GetDocument();
				if (pDoc == NULL)
				{
					return _T("");
				}

				// Set the return string value
				strTitle = pDoc->GetTitle();

				// Reset the frames title
				pChildFrame->SetWindowText(strTitle);

				// Return the document title.
				return strTitle;
			}
			pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
		}
	}

	// Return the MDI frame window's title.
	return strTitle;
}

BOOL CXTPMDIWndTab::RefreshActiveSel()
{
	// Get the active MDI window, if NULL return FALSE.
	HWND hWndActiveChild = m_pMDIFrameWnd->MDIGetActive()->GetSafeHwnd();
	if (hWndActiveChild == NULL)
	{
		m_hActiveChild = NULL;
		return FALSE;
	}

	// Loop through all of the tabs to find the view that has activated.
	bool bSelectionChanged = false;
	if (POSITION pos = m_arMDIChildern.GetHeadPosition())
	{
		do
		{
			MDICHILD* pMDIChild = m_arMDIChildern.GetNext(pos);
			ASSERT(pMDIChild != NULL);

			// We have found the handle to the active MDI window,
			// lets see if the tab is selected.
			if (pMDIChild && (pMDIChild->hWnd == hWndActiveChild))
			{
				// If the active child pointer does not match, then
				// make sure that the tab is selected then save the pointer.
				bSelectionChanged = hWndActiveChild != m_hActiveChild;
				if (bSelectionChanged)
				{
					SetCurSel(pMDIChild->iItem);
					m_hActiveChild = hWndActiveChild;
				}
			}
		} while (pos != NULL && !bSelectionChanged);
	}
	return bSelectionChanged;
}

void CXTPMDIWndTab::RefreshTabLabels()
{
	// Loop through all of the tabs to find the view that has activated.
	POSITION pos = m_arMDIChildern.GetHeadPosition();
	while (pos != NULL)
	{
		bool bSetItem = false;

		MDICHILD* pMDIChild = m_arMDIChildern.GetNext(pos);
		ASSERT(pMDIChild != NULL);
		if (!pMDIChild)
			continue;

		// Get the window text for the child frame and the tab item.
		CString strWindowText = GetChildWndText(pMDIChild->hWnd);

		TCHAR szText[_MAX_PATH];
		szText[0] = 0;

		TC_ITEM tci;
		tci.mask	   = TCIF_TEXT | TCIF_IMAGE;
		tci.cchTextMax = _countof(szText);
		tci.pszText	= szText;

		// Get the item data for the tab.
		if (GetItem(pMDIChild->iItem, &tci))
		{
			CString strTabLabel = szText;

			if (m_pTheme->GetThemeStyle() != xtpControlThemeOffice2000)
			{
				OnAddPadding(strWindowText);
			}

			// If they do not match then update the tab item label.
			if (strTabLabel != strWindowText)
			{
				STRCPY_S(szText, _MAX_PATH, strWindowText);
				tci.cchTextMax = strWindowText.GetLength();

				bSetItem = true;
			}

			if (!m_bNoIcons)
			{
				// Check to see if the user set the icon for the tab label.
				HICON hIcon = 0;
				if (!m_mapTabIcons.Lookup(pMDIChild->hWnd, hIcon))
				{
					// The user may have set the icon for the child by calling WM_SETICON.  If
					// WM_GETICON returns an icon we need to make sure the tab icon has been
					// updated.

					hIcon = (HICON)::SendMessage(pMDIChild->hWnd, WM_GETICON, FALSE, 0);

					// Insert the icon into map to avoid failed Lookup().
					m_mapTabIcons.SetAt(pMDIChild->hWnd, hIcon);
				}

				// Replace the image in image list ONLY when Lookup() failed.
				if ((hIcon != NULL) && (tci.iImage != -1))
				{
					bSetItem = true;
					m_imageList.Replace(tci.iImage, hIcon);
				}
			}

			if (bSetItem)
			{
				VERIFY(SetItem(pMDIChild->iItem, &tci));
			}
		}
	}
}

void CXTPMDIWndTab::OnIdleUpdateCmdUI()
{
	if (m_pMDIFrameWnd)
	{
		RefreshActiveSel();
		RefreshTabLabels();
	}
}

void CXTPMDIWndTab::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	RECT& rc = lpncsp->rgrc[0];

	if (rc.bottom - rc.top > m_iBorderGap * 2)
	{
		rc.top += m_iBorderGap;
		rc.bottom -= m_iBorderGap;
	}
	else
	{
		rc.top = rc.bottom;
	}

	if (rc.right - rc.left > m_iBorderGap * 2)
	{
		rc.left += m_iBorderGap;
		rc.right -= m_iBorderGap;
	}
	else
	{
		rc.left = rc.right;
	}

	CTabCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CXTPMDIWndTab::OnNcPaint()
{
	CWindowDC dc(this);

	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

	if (rectWindow.Width() > rectClient.Width())
	{
		dc.DrawEdge(rectWindow, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);

		int nBorder = (rectWindow.Width() - rectClient.Width()) / 2;

		if (nBorder > 0)
		{
			dc.FillSolidRect(rectWindow.left, rectWindow.top, rectWindow.Width(), nBorder,
							 GetSysColor(COLOR_3DFACE));
			dc.FillSolidRect(rectWindow.left, rectWindow.top, nBorder, rectWindow.Height(),
							 GetSysColor(COLOR_3DFACE));

			dc.FillSolidRect(rectWindow.left, rectWindow.bottom - nBorder, rectWindow.Width(),
							 nBorder, GetSysColor(COLOR_3DFACE));
			dc.FillSolidRect(rectWindow.right - nBorder, rectWindow.top, nBorder,
							 rectWindow.Height(), GetSysColor(COLOR_3DFACE));
		}
	}
}

void CXTPMDIWndTab::OnSelchange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Get the item data for the currently selected tab.
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	if (GetItem(GetCurSel(), &tci))
	{
		MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
		ASSERT(pMDIChild != NULL);
		if (!pMDIChild)
			return;

		// Get the pointer to the currently selected tab window.
		CWnd* pActiveWnd = CWnd::FromHandle(pMDIChild->hWnd);
		ASSERT(pActiveWnd);

		// Activate the new tab window.
		m_pMDIFrameWnd->MDIActivate(pActiveWnd);
	}

	*pResult = 0;
}

void CXTPMDIWndTab::OnDestroy()
{
	CTabCtrl::OnDestroy();

	// Delete the image list.
	m_imageList.DeleteImageList();

	// Set the active and parent windows to NULL.
	m_hActiveChild = NULL;
	m_pMDIFrameWnd = NULL;
}

int CXTPMDIWndTab::TabFromPoint(CPoint point) const
{
	TCHITTESTINFO tch;
	memset(&tch, 0, sizeof(TCHITTESTINFO));

	if (point == CPoint(0, 0))
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		tch.pt = pt;
	}
	else
	{
		tch.pt = point;
	}

	return HitTest(&tch);
}

CMDIChildWnd* CXTPMDIWndTab::GetFrameWnd(int iIndex) const
{
	if (iIndex == -1)
		iIndex = m_iHitTest;

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	if (GetItem(iIndex, &tci))
	{
		MDICHILD* pMDIChild = (MDICHILD*)tci.lParam;
		ASSERT(pMDIChild != NULL);
		if (!pMDIChild)
			return NULL;

		return DYNAMIC_DOWNCAST(CMDIChildWnd, CWnd::FromHandle(pMDIChild->hWnd));
	}

	return NULL;
}

CMDIChildWnd* CXTPMDIWndTab::GetFrameWnd(CPoint point) const
{
	return GetFrameWnd(TabFromPoint(point));
}

void CXTPMDIWndTab::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	// Get the tab index based upon the cursor position.
	m_iHitTest = TabFromPoint(point);
	if (m_iHitTest != -1)
	{
		if (m_popupMenuID != 0)
		{
			CPoint pt = point;
			ClientToScreen(&pt);

			CMenu popupMenu;
			VERIFY(popupMenu.LoadMenu(m_popupMenuID));

			CMenu* pPopup = popupMenu.GetSubMenu(m_nPos);
			ASSERT(pPopup != NULL);
			if (!pPopup)
				return;

			if (m_nDefCmd != 0)
			{
				::SetMenuDefaultItem(pPopup->m_hMenu, m_nDefCmd, FALSE);
			}

			CWnd* pWndPopupOwner = GetOwner();
			ASSERT(pWndPopupOwner);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pWndPopupOwner);
		}
	}
}

void CXTPMDIWndTab::InitializeFont()
{
	HGDIOBJ hFont = XTPAuxData().xtpFont.GetSafeHandle();

	// if the tabs are vertical, use the vertical menu font.
	if ((GetStyle() & TCS_VERTICAL) == TCS_VERTICAL)
		hFont = XTPAuxData().xtpFontVert.GetSafeHandle();

	if (hFont != NULL)
	{
		if (XTPSystemVersion()->IsWin95())
			PostMessage(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		else
			SendMessage(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
	}
}

void CXTPMDIWndTab::OnTabClose()
{
	if (m_pMDIFrameWnd)
	{
		CMDIChildWnd* pActiveChild = (CMDIChildWnd*)m_pMDIFrameWnd->MDIGetActive();
		if (pActiveChild != NULL)
		{
			pActiveChild->SendMessage(WM_CLOSE);
		}
	}
}
