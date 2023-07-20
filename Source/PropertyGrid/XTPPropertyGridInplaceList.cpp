// XTPPropertyGridInplaceList.cpp : implementation of the CXTPPropertyGridInplaceList class.
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
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridInplaceList.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CXTPPropertyGridInplaceList::m_bShowShadow		= FALSE;
BOOL CXTPPropertyGridInplaceList::m_bTrackSelection = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceList

CXTPPropertyGridInplaceList::CXTPPropertyGridInplaceList()
	: m_pItem(0)
{
}

CXTPPropertyGridInplaceList::~CXTPPropertyGridInplaceList()
{
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceList, CListBox)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceList, CListBox)
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceList)
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_GETDLGCODE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPPropertyGridInplaceList::OnNcPaint()
{
	Default();

	CWindowDC dc(this);

	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	dc.Draw3dRect(rc, GetSysColor(COLOR_WINDOWFRAME), GetSysColor(COLOR_WINDOWFRAME));
}

UINT CXTPPropertyGridInplaceList::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPPropertyGridInplaceList::MeasureItem(LPMEASUREITEMSTRUCT)
{
}

void CXTPPropertyGridInplaceList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc(lpDrawItemStruct->rcItem);
	CXTPPropertyGridItemConstraint* pConstraint = (CXTPPropertyGridItemConstraint*)GetItemDataPtr(
		XTPToInt(lpDrawItemStruct->itemID));

	BOOL bSelected = (0 != (lpDrawItemStruct->itemState & ODS_SELECTED));

	if (m_pItem)
	{
		m_pItem->OnDrawItemConstraint(pDC, pConstraint, rc, bSelected);
	}
}

void CXTPPropertyGridInplaceList::Create(CXTPPropertyGridItem* pItem, CRect rect)
{
	ASSERT(pItem && pItem->GetGrid());
	if (!pItem)
		return;

	CRect rcValue(rect);
	rcValue.left = pItem->GetGrid()->GetDividerPos() + 1;

	CWnd* pParent = (CWnd*)pItem->GetGrid();
	m_pItem		  = pItem;

	DestroyWindow();

	if (!m_hWnd)
	{
		CListBox::CreateEx(WS_EX_TOOLWINDOW | (pParent->GetExStyle() & WS_EX_LAYOUTRTL),
						   _T("LISTBOX"), _T(""),
						   LBS_NOTIFY | WS_CHILD | WS_VSCROLL | WS_BORDER | LBS_OWNERDRAWFIXED
							   | LBS_HASSTRINGS,
						   CRect(0, 0, 0, 0), pParent, 0);
		SetOwner(pParent);

		if (m_bShowShadow && XTPSystemVersion()->IsWinXPOrGreater())
			SetClassLongPtr(m_hWnd, GCL_STYLE,
							XTPToLongPtr(GetClassLongPtr(m_hWnd, GCL_STYLE)) | 0x00020000);
	}
	SetFont(pParent->GetFont());

	ResetContent();

	CXTPPropertyGridItemConstraints* pList = pItem->GetConstraints();

	int dx = rect.right - rcValue.left;

	CWindowDC dc(pParent);
	CXTPFontDC font(&dc, pParent->GetFont());
	int nHeight = dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(3);

	for (int i = 0; i < pList->GetCount(); i++)
	{
		CXTPPropertyGridItemConstraint* pConstraint = pList->GetConstraintAt(i);

		CString str = pConstraint->m_strConstraint;
		int nIndex  = AddString(str);
		SetItemDataPtr(nIndex, pConstraint);

		CSize sz = pItem->OnMergeItemConstraint(&dc, pConstraint);

		dx		= max(dx, sz.cx);
		nHeight = max(nHeight, sz.cy);

		if (pItem->GetValue() == str)
			SetCurSel(nIndex);
	}

	SetItemHeight(0, XTPToUIntChecked(nHeight));

	rect.top = rect.bottom;
	rect.bottom += nHeight * __min(pItem->GetDropDownItemCount(), GetCount()) + XTP_DPI_Y(2);
	rect.left = rect.right - __min(dx, rect.Width() - XTP_DPI_X(XTP_PGI_EXPAND_BORDER));

	pParent->ClientToScreen(&rect);

	CRect rcWork = XTPMultiMonitor()->GetWorkArea(rect);
	if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
	{
		rect.OffsetRect(0, -rect.Height() - rcValue.Height() - 1);
	}
	if (rect.left < rcWork.left)
		rect.OffsetRect(rcWork.left - rect.left, 0);
	if (rect.right > rcWork.right)
		rect.OffsetRect(rcWork.right - rect.right, 0);

	SetFocus();

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(pParent, pParent, TRUE)
#endif

	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, 0);
	ModifyStyle(WS_CHILD, WS_POPUP);
	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)pParent->m_hWnd);

	SetWindowPos(&CWnd::wndTopMost, rect.left, rect.top, rect.Width(), rect.Height(),
				 SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

	CXTPMouseMonitor::SetupHook(this);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceList message handlers

void CXTPPropertyGridInplaceList::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	CXTPMouseMonitor::SetupHook(NULL);

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(GetOwner(), GetOwner(), FALSE)
#endif

	Cancel();
	DestroyWindow();
}

int CXTPPropertyGridInplaceList::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/,
												 UINT /*message*/)
{
	return MA_NOACTIVATE;
}

void CXTPPropertyGridInplaceList::PostNcDestroy()
{
	CXTPMouseMonitor::SetupHook(NULL);

	CListBox::PostNcDestroy();
}

void CXTPPropertyGridInplaceList::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove(nFlags, point);

	if (m_bTrackSelection)
	{
		BOOL bOutsize = FALSE;
		int nItem	 = XTPToIntChecked(ItemFromPoint(point, bOutsize));
		if (nItem != -1 && !bOutsize && nItem != GetCurSel())
		{
			SetCurSel(nItem);
		}
	}
}

void CXTPPropertyGridInplaceList::OnLButtonUp(UINT, CPoint point)
{
	CXTPClientRect rc(this);

	if (rc.PtInRect(point))
		Apply();
	else
		Cancel();
}

void CXTPPropertyGridInplaceList::Cancel()
{
	if (m_pItem)
	{
		m_pItem->OnCancelEdit();
		m_pItem = NULL;
	}

	if (m_hWnd && ::GetFocus() == m_hWnd)
	{
		GetOwner()->SetFocus();
	}
}

void CXTPPropertyGridInplaceList::Apply()
{
	int nIndex = GetCurSel();
	if (nIndex != LB_ERR && m_pItem)
	{
		CXTPPropertyGridItemConstraint* pConstraint = (CXTPPropertyGridItemConstraint*)
			GetItemDataPtr(nIndex);
		if (pConstraint && !pConstraint->m_bEnabled)
			return;

		CXTPPropertyGridItem* pItem = m_pItem;
		m_pItem						= NULL;

		CXTPPropertyGridItemConstraints* pList = pItem->GetConstraints();
		pList->SetCurrent(nIndex);

		CString str;
		GetText(nIndex, str);

		if (pItem->OnAfterEdit(str) && str != pItem->GetValue())
			pItem->OnValueChanged(str);
	}

	if (m_hWnd && ::GetFocus() == m_hWnd)
	{
		GetOwner()->SetFocus();
	}
}

void CXTPPropertyGridInplaceList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
		Cancel();
	else if (nChar == VK_RETURN || nChar == VK_F4)
		Apply();
	else
		CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceList::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		Apply();
		return;
	}

	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceList::DestroyItem()
{
	// reset variables to defaults.
	m_pItem = NULL;

	CXTPMouseMonitor::SetupHook(NULL);

	// destroy the window.
	DestroyWindow();
}

BOOL CXTPPropertyGridInplaceList::PreTranslateMessage(MSG* pMsg)
{
	return (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceMultilineEdit

CXTPPropertyGridInplaceMultilineEdit::CXTPPropertyGridInplaceMultilineEdit()
	: m_pItem(0)
{
}

CXTPPropertyGridInplaceMultilineEdit::~CXTPPropertyGridInplaceMultilineEdit()
{
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceMultilineEdit, CEdit)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceMultilineEdit, CEdit)
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceMultilineEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPPropertyGridInplaceMultilineEdit::Create(CXTPPropertyGridItem* pItem, CRect rect)
{
	ASSERT(pItem && pItem->GetGrid());
	if (!pItem)
		return;

	CRect rcValue(rect);
	rcValue.left = pItem->GetGrid()->GetDividerPos() + 1;

	CWnd* pParent = (CWnd*)pItem->GetGrid();
	m_pItem		  = pItem;

	if (!m_hWnd)
	{
		CEdit::CreateEx(WS_EX_TOOLWINDOW, _T("EDIT"), m_pItem->GetValue(),
						ES_MULTILINE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL,
						CRect(0, 0, 0, 0), pParent, 0);
		SetOwner(pParent);
	}
	SetFont(pParent->GetFont());

	int dx = rect.right - rcValue.left;

	CWindowDC dc(pParent);
	CXTPFontDC font(&dc, pParent->GetFont());
	int nHeight = dc.GetTextExtent(_T(" "), 1).cy;

	rect.top = rect.bottom;
	rect.bottom += nHeight * pItem->GetDropDownItemCount() + XTP_DPI_Y(4);
	rect.left = rect.right - __min(dx, rect.Width() - XTP_DPI_X(XTP_PGI_EXPAND_BORDER));

	pParent->ClientToScreen(&rect);

	CRect rcWork = XTPMultiMonitor()->GetWorkArea(rect);
	if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
	{
		rect.OffsetRect(0, -rect.Height() - rcValue.Height() - 1);
	}
	if (rect.left < rcWork.left)
		rect.OffsetRect(rcWork.left - rect.left, 0);
	if (rect.right > rcWork.right)
		rect.OffsetRect(rcWork.right - rect.right, 0);

	SetFocus();

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(pParent, pParent, TRUE)
#endif

	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, 0);
	ModifyStyle(WS_CHILD, WS_POPUP);
	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)pParent->m_hWnd);

	SetWindowPos(&CWnd::wndTopMost, rect.left, rect.top, rect.Width(), rect.Height(),
				 SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

	CXTPMouseMonitor::SetupHook(this);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceMultilineEdit message handlers

void CXTPPropertyGridInplaceMultilineEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CXTPMouseMonitor::SetupHook(NULL);

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(GetOwner(), GetOwner(), FALSE)
#endif

	Apply();
	DestroyWindow();
}

int CXTPPropertyGridInplaceMultilineEdit::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/,
														  UINT /*message*/)
{
	return MA_NOACTIVATE;
}

void CXTPPropertyGridInplaceMultilineEdit::PostNcDestroy()
{
	CXTPMouseMonitor::SetupHook(NULL);

	delete this;
}

void CXTPPropertyGridInplaceMultilineEdit::Cancel()
{
	if (m_pItem)
	{
		m_pItem->OnCancelEdit();
		m_pItem = NULL;
	}

	if (m_hWnd && ::GetFocus() == m_hWnd)
	{
		GetOwner()->SetFocus();
	}
}

void CXTPPropertyGridInplaceMultilineEdit::Apply()
{
	if (m_pItem)
	{
		CXTPPropertyGridItem* pItem = m_pItem;
		m_pItem						= NULL;

		CString str;
		GetWindowText(str);

		if (pItem->OnAfterEdit(str) && str != pItem->GetValue())
			pItem->OnValueChanged(str);
	}

	if (m_hWnd && ::GetFocus() == m_hWnd)
	{
		GetOwner()->SetFocus();
	}
}

void CXTPPropertyGridInplaceMultilineEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
		Cancel();
	else if (nChar == VK_F4)
		Apply();
	else if (nChar == VK_RETURN && (GetKeyState(VK_CONTROL) >= 0))
		Apply();
	else
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceMultilineEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		Apply();
		return;
	}

	CEdit::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

UINT CXTPPropertyGridInplaceMultilineEdit::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

BOOL CXTPPropertyGridInplaceMultilineEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		Cancel();
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}
