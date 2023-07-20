// XTPGridSubListControl.cpp : implementation of the CXTPGridSubListControl class.
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
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridDragDrop.h"
#include "GridControl/XTPGridSubListControl.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPGridSubListControl, CListBox)
/////////////////////////////////////////////////////////////////////////////
// CXTPGridSubListControl

CXTPGridSubListControl::CXTPGridSubListControl()
{
	m_pGridCtrl		 = NULL;
	m_pDragWnd		 = NULL;
	m_bSortAscending = FALSE;
	m_nDropIndex	 = 0;
}

CXTPGridSubListControl::~CXTPGridSubListControl()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridSubListControl, CListBox)
	//{{AFX_MSG_MAP(CXTPGridSubListControl)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPGridSubListControl attributes

BOOL CXTPGridSubListControl::SetGridCtrl(CXTPGridControl* pGridCtrl)
{
	if (pGridCtrl == NULL)
		return FALSE;

	ASSERT_KINDOF(CXTPGridControl, pGridCtrl);

	if (m_pGridCtrl != pGridCtrl)
	{
		ResetContent();
		m_pGridCtrl = pGridCtrl;
	}

	UpdateList();

	return TRUE;
}

CXTPGridControl* CXTPGridSubListControl::GetGridCtrl()
{
	return m_pGridCtrl;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPGridSubListControl operations

void CXTPGridSubListControl::SortAscending(BOOL bSort)
{
	m_bSortAscending = bSort;
	if (GetSafeHwnd())
		ResetContent();
	UpdateList();
}

BOOL CXTPGridSubListControl::UpdateList()
{
	if (m_pGridCtrl == NULL)
		return FALSE;

	if (GetSafeHwnd() == NULL)
		return FALSE;

	CClientDC dc(this);
	CXTPFontDC fnt(&dc, &m_pGridCtrl->GetPaintManager()->m_xtpFontCaption);
	int nHeight = dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(5);

	if (GetItemHeight(0) != nHeight)
		SetItemHeight(0, XTPToUIntChecked(nHeight));

	CXTPGridColumns* pColumns = m_pGridCtrl->GetColumns();
	int nColumnCount		  = pColumns->GetCount();

	BOOL bChanged = FALSE;
	// delete all visible items from the list
	for (int i = GetCount() - 1; i >= 0; i--)
	{
		int nItemIndex			= (int)GetItemData(i);
		CXTPGridColumn* pColumn = pColumns->Find(nItemIndex);
		if (!pColumn || pColumn->IsVisible() || !pColumn->IsShowInFieldChooser())
		{
			DeleteString(XTPToUIntChecked(i));
			bChanged = TRUE;
		}
	}

	// add the rest of invisible items
	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && !pColumn->IsVisible() && pColumn->IsShowInFieldChooser()
			&& (m_pGridCtrl->m_iIconViewColumn == -1
					? TRUE
					: (pColumn->GetItemIndex() != m_pGridCtrl->m_iIconViewColumn ? TRUE : FALSE)))
		{
			int nListItem = FindItemData(pColumn->GetItemIndex());
			if (nListItem < 0)
			{
				int nItem = -1;
				if (m_bSortAscending)
				{
					for (nItem = 0; nItem < GetCount(); nItem++)
					{
						CString str = GetItemCaption(nItem);

						if (str > pColumn->GetCaption())
							break;
					}
					nItem = InsertString(nItem, pColumn->GetCaption());
				}
				else
				{
					nItem = AddString(pColumn->GetCaption());
				}

				if (nItem >= 0)
				{
					SetItemData(nItem, XTPToULongPtr(pColumn->GetItemIndex()));
					bChanged = TRUE;
				}
			}
		}
	}
	EnableWindow(GetCount() > 0);

	if (bChanged)
		Invalidate();

	return TRUE;
}

CXTPGridColumn* CXTPGridSubListControl::GetItemColumn(int nIndex)
{
	if (!m_pGridCtrl || GetSafeHwnd() == 0)
		return NULL;

	int nItemData = (int)GetItemData(nIndex);
	if (nItemData == -1)
		return NULL;

	CXTPGridColumn* pColumn = m_pGridCtrl->GetColumns()->Find(nItemData);

	return pColumn;
}

CString CXTPGridSubListControl::GetItemCaption(int nIndex)
{
	if (!m_pGridCtrl || GetSafeHwnd() == 0)
		return _T("");

	if (GetStyle() & LBS_HASSTRINGS)
	{
		CString str;
		GetText(nIndex, str);
		return str;
	}

	CXTPGridColumn* pColumn = GetItemColumn(nIndex);

	if (pColumn == NULL)
		return _T("");

	return pColumn->GetCaption();
}

int CXTPGridSubListControl::FindItemData(int nItemData)
{
	for (int i = GetCount() - 1; i >= 0; i--)
	{
		int nItemIndex = (int)GetItemData(i);
		if (nItemData == nItemIndex)
			return i;
	}
	return -1;
}

BOOL CXTPGridSubListControl::Include(int /*iSubItem*/)
{
	return TRUE;
}

BOOL CXTPGridSubListControl::Disable(int /*iSubItem*/)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPGridSubListControl message handlers

void CXTPGridSubListControl::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
}

void CXTPGridSubListControl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect rcItem(lpDrawItemStruct->rcItem);

	CXTPGridPaintManager* pPaintManager = m_pGridCtrl ? m_pGridCtrl->GetPaintManager() : NULL;
	COLORREF clrBack = pPaintManager ? pPaintManager->m_clrControlBack.operator COLORREF()
									 : ::GetXtremeColor(COLOR_WINDOW);

	if (GetCount() > 0 && m_pGridCtrl && pPaintManager)
	{
		BOOL bDisable = Disable((int)lpDrawItemStruct->itemData);

		CXTPFontDC fnt(pDC, &pPaintManager->m_xtpFontCaption);

		if (GetExStyle() & WS_EX_STATICEDGE)
		{
			pDC->Draw3dRect(rcItem, pPaintManager->m_clrHighlightText,
							pPaintManager->m_clrControlDark);
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pDC->FillSolidRect(rcItem, pPaintManager->m_clrControlBack);
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		}
		else
		{
			pDC->FillSolidRect(rcItem, pPaintManager->m_clrHeaderControl);
			pDC->Draw3dRect(rcItem, pPaintManager->m_clrBtnFace, GetXtremeColor(COLOR_3DDKSHADOW));
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pDC->Draw3dRect(rcItem, pPaintManager->m_clrControlBack,
							pPaintManager->m_clrControlDark);
			rcItem.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
		}

		pDC->SetBkMode(TRANSPARENT);

		CXTPGridColumn* pColumn = GetItemColumn((int)lpDrawItemStruct->itemID);
		BOOL bSelected			= (0 != (lpDrawItemStruct->itemState & ODS_SELECTED));

		pDC->SetTextColor(pPaintManager->m_clrCaptionText);

		if (bSelected)
		{
			pDC->FillSolidRect(rcItem, GetXtremeColor(COLOR_3DFACE));
			pDC->InvertRect(rcItem);
			pDC->SetTextColor(bDisable ? ::GetXtremeColor(COLOR_BTNFACE)
									   : ::GetXtremeColor(COLOR_3DHIGHLIGHT));
		}

		CXTPMarkupUIElement* pMarkupUIElement = pColumn->GetMarkupUIElement();

		if (pMarkupUIElement)
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement),
									(HFONT)pDC->GetCurrentFont()->GetSafeHandle(),
									pDC->GetTextColor());

			XTPMarkupMeasureElement(pMarkupUIElement, rcItem.Width(), INT_MAX);
			XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), &rcItem);
		}
		else
		{
			if (bDisable && !bSelected)
			{
				pDC->SetTextColor(::GetXtremeColor(COLOR_3DHIGHLIGHT));

				CRect rect = rcItem;
				rect.OffsetRect(XTP_DPI_X(1), XTP_DPI_Y(1));

				pDC->DrawText(pColumn->GetCaption(), rect,
							  DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS
								  | DT_LEFT);

				pDC->SetTextColor(::GetXtremeColor(COLOR_3DSHADOW));
			}

			pDC->DrawText(pColumn->GetCaption(), rcItem,
						  DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS
							  | DT_LEFT);
		}
	}
	else
		pDC->FillSolidRect(rcItem, clrBack);
}

BOOL CXTPGridSubListControl::BeginDrag(int nItem)
{
	if (GetCount() <= 0)
		return FALSE;

	int nSubItem = (int)GetItemData(nItem);

	if (Disable(nSubItem))
	{
		return FALSE;
	}

	GetClientRect(m_rcDragWnd);
	m_rcDragWnd.bottom = m_rcDragWnd.top + GetItemHeight(0);

	CXTPGridHeader* pHeader = m_pGridCtrl->GetGridHeader();
	CXTPGridColumn* pColumn = m_pGridCtrl->GetColumns()->Find(nSubItem);
	pHeader->StartDragging(pColumn);
	pHeader->m_bDragGroupBox = FALSE;

	// pHeader->m_nDragIndex = m_nSubItem;
	m_pDragWnd = new CXTPGridHeaderDragWnd;
	if (m_pDragWnd)
		m_pDragWnd->Create(m_rcDragWnd, pHeader, m_pGridCtrl->GetPaintManager(), pColumn);
	m_pGridCtrl->SetMouseMode(xtpGridMouseDraggingColumn);

	GetWindowRect(m_rcDropTarget1);
	m_rcDropTarget2 = m_pGridCtrl->m_rcHeaderArea;

	if (pColumn->IsGroupable())
		m_rcDropTarget2.UnionRect(&m_rcDropTarget2, &m_pGridCtrl->m_rcGroupByArea);

	m_pGridCtrl->ClientToScreen(m_rcDropTarget2);

	m_nDropIndex = -1;

	return TRUE;
}

UINT CXTPGridSubListControl::Dragging(CPoint pt)
{
	CPoint point = pt;
	point.Offset(-(m_rcDragWnd.Width() >> 1), -(m_rcDragWnd.Height() >> 1));

	if (m_pDragWnd != NULL)
	{
		m_pDragWnd->SetWindowPos(&wndTop, point.x, point.y, 0, 0,
								 SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}

	if (m_pGridCtrl)
	{
		CPoint pointCtrl(pt);
		m_pGridCtrl->ScreenToClient(&pointCtrl);
		m_pGridCtrl->OnMouseMove(0, pointCtrl);
	}

	if (m_rcDropTarget1.PtInRect(pt))
		return DL_MOVECURSOR;

	if (m_rcDropTarget2.PtInRect(pt))
		return DL_MOVECURSOR;

	return DL_STOPCURSOR;
}

void CXTPGridSubListControl::CancelDrag()
{
	if (m_pDragWnd != NULL)
	{
		m_pDragWnd->DestroyWindow();
		m_pDragWnd = NULL;
	}

	CXTPGridHeader* pHeader = m_pGridCtrl->GetGridHeader();
	pHeader->SetHotDivider(-1);

	m_pGridCtrl->SetMouseMode(xtpGridMouseNothing);
}

void CXTPGridSubListControl::Dropped(CPoint pt)
{
	if (m_pDragWnd != NULL)
	{
		m_pDragWnd->DestroyWindow();
		m_pDragWnd = NULL;
	}

	if (m_pGridCtrl)
	{
		m_pGridCtrl->ScreenToClient(&pt);
		m_pGridCtrl->OnLButtonUp(0, pt);
	}
}

void CXTPGridSubListControl::OnPaint()
{
	CPaintDC dcPaint(this);
	CRect rc;
	GetClientRect(rc);

	CXTPBufferDC dc(dcPaint, rc);

	CXTPGridPaintManager* pPaintManager = m_pGridCtrl ? m_pGridCtrl->GetPaintManager() : NULL;
	COLORREF clrBack = pPaintManager ? pPaintManager->m_clrControlBack.operator COLORREF()
									 : GetXtremeColor(COLOR_WINDOW);

	dc.FillSolidRect(rc, IsWindowEnabled() ? clrBack : GetXtremeColor(COLOR_BTNFACE));

	if (GetCount() > 0 || !pPaintManager)
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
	else
	{
		dc.SetTextColor(GetXtremeColor(COLOR_3DSHADOW));
		dc.SetBkMode(TRANSPARENT);
		CXTPFontDC fnt(&dc, &pPaintManager->m_xtpFontCaption);

		dc.DrawText(pPaintManager->m_strNoFieldsAvailable, rc,
					DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX);
	}
}

LRESULT CXTPGridSubListControl::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	CListBox::DefWindowProc(WM_ERASEBKGND, wParam, 0);
	return CListBox::DefWindowProc(WM_PRINTCLIENT, wParam, lParam);
}

BOOL CXTPGridSubListControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPGridSubListControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	BOOL bOutside = TRUE;
	int nItem	 = XTPToIntChecked(ItemFromPoint(point, bOutside));
	if (nItem == LB_ERR)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	SetCurSel(nItem);
	ParentNotify_SelChangd();

	BOOL bAccept	= FALSE;
	BOOL bStartDrag = FALSE;

	HCURSOR hCursorNo	= AfxGetApp()->LoadStandardCursor(IDC_NO),
			hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	SetCapture();

	while (CWnd::GetCapture() == this)
	{
		MSG msg;
		if (!GetMessage(&msg, NULL, 0, 0))
			break;

		if (msg.message == WM_MOUSEMOVE)
		{
			CPoint pt(msg.lParam);

			if (point != pt)
			{
				if (!bStartDrag && abs(point.x - pt.x) + abs(point.y - pt.y) < XTP_DPI_X(3))
					continue;

				point = pt;

				if (!bStartDrag)
				{
					BeginDrag(nItem);
					bStartDrag = TRUE;
				}

				if (bStartDrag)
				{
					ClientToScreen(&pt);
					UINT nCursor = Dragging(pt);
					::SetCursor(nCursor == DL_STOPCURSOR ? hCursorNo : hCursorArrow);
				}
			}
		}
		else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
			break;
		else if (msg.message == WM_LBUTTONUP)
		{
			bAccept = TRUE;
			break;
		}
		else
			::DispatchMessage(&msg);
	}

	if (GetCapture() == this)
	{
		ReleaseCapture();
	}

	if (bStartDrag)
	{
		if (bAccept)
		{
			ClientToScreen(&point);
			Dropped(point);

			ParentNotify_SelChangd();
		}
		else
		{
			CancelDrag();
		}
	}
}

void CXTPGridSubListControl::ParentNotify_SelChangd()
{
	if (GetParent() && (GetStyle() & LBS_NOTIFY))
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE),
								 (LPARAM)m_hWnd);
}
