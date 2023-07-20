// XTPCalendarEventColorListBox.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "Calendar/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Popup/XTPColorSelectorCtrl.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPColorPicker.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarEventLabel.h"
#include "Calendar/XTPCalendarEventColorListBox.h"
#include "Calendar/XTPCalendarEventColorNewDlg.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorEdit

CXTPCalendarEventColorEdit::CXTPCalendarEventColorEdit(int nIndex)
	: m_nIndex(nIndex)
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarEventColorEdit, CEdit)
	//{{AFX_MSG_MAP(CXTPCalendarEventColorEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorEdit message handlers

BOOL CXTPCalendarEventColorEdit::PreTranslateMessage(MSG* pMsg)
{
	CXTPCalendarEventColorListBox* pOwnerWnd = DYNAMIC_DOWNCAST(CXTPCalendarEventColorListBox,
																GetOwner());
	if (pOwnerWnd)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN: pOwnerWnd->EndEdit(); return TRUE;

			case VK_ESCAPE: pOwnerWnd->EndEdit(FALSE); return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CXTPCalendarEventColorEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CXTPCalendarEventColorListBox* pOwnerWnd = (CXTPCalendarEventColorListBox*)GetOwner();
	if (pOwnerWnd)
	{
		pOwnerWnd->EndEdit();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorListBox

CXTPCalendarEventColorListBox::CXTPCalendarEventColorListBox()
	: m_bModified(FALSE)
	, m_bCategories(FALSE)
	, m_pData(NULL)
	, m_pEvent(NULL)
	, m_pWndEdit(NULL)
{
	m_sizeColor.cx = ::GetSystemMetrics(SM_CXHTHUMB) - ::GetSystemMetrics(SM_CXEDGE);
	m_sizeColor.cy = ::GetSystemMetrics(SM_CYVTHUMB) - ::GetSystemMetrics(SM_CYEDGE);
}

CXTPCalendarEventColorListBox::~CXTPCalendarEventColorListBox()
{
	SAFE_DELETE(m_pWndEdit);
}

IMPLEMENT_DYNAMIC(CXTPCalendarEventColorListBox, CListBox)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarEventColorListBox, CListBox)
	//{{AFX_MSG_MAP(CXTPCalendarEventColorListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPCalendarEventColorListBox::Init(CXTPCalendarData* pData, CXTPCalendarEvent* pEvent,
										 BOOL bCategories)
{
	m_pData = pData;
	ASSERT(m_pData);

	if (!m_pData)
		return;

	m_pEvent = pEvent; // can be NULL.

	m_bCategories = bCategories;

	if (!::IsWindow(m_hWnd))
		return;

	ResetContent();

	int nIndex = 0;

	if (m_bCategories)
	{
		CXTPCalendarEventCategories* pCategories = pData->GetEventCategories();
		ASSERT(pCategories);

		if (!pCategories)
			return;

		for (int i = 0; i < pCategories->GetCount(); i++)
		{
			CXTPCalendarEventCategory* pCategory = pCategories->GetAt(i);
			if (pCategory)
			{
				SetItemData(AddString(pCategory->GetName()),
							(DWORD_PTR) new CXTPCalendarEventCategory(pCategory));
			}
		}

		CXTPCalendarEventCategoryIDs* pCategoryIDs = XTP_SAFE_GET1(m_pEvent, GetCategories(), NULL);
		if (pCategoryIDs)
		{
			m_arChecked.RemoveAll();
			m_arChecked.Copy(*pCategoryIDs);
		}
	}
	else
	{
		CXTPCalendarEventLabels* pLabels = pData->GetLabelList();
		ASSERT(pLabels);

		if (!pLabels)
			return;

		for (int i = 0; i < pLabels->GetCount(); i++)
		{
			CXTPCalendarEventLabel* pLabel = pLabels->GetAt(i);
			if (pLabel)
			{
				SetItemData(AddString(pLabel->m_strName),
							(DWORD_PTR) new CXTPCalendarEventLabel(pLabel));

				if (pEvent->GetLabelID() == pLabel->m_nLabelID)
				{
					nIndex = i;
				}
			}
		}
	}

	SetCurSel(nIndex);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorListBox message handlers

void CXTPCalendarEventColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_LISTBOX);
	lpMIS->itemHeight = XTPToUIntChecked(m_sizeColor.cy + XTP_DPI_Y(6));
	lpMIS->itemWidth  = 0;
}

CRect CXTPCalendarEventColorListBox::CalcColorRect(CRect rItem)
{
	CRect rColor(rItem);
	rColor.top	= rItem.top + (rItem.Height() - m_sizeColor.cy) / 2;
	rColor.bottom = rColor.top + m_sizeColor.cy;
	rColor.left   = rItem.left + XTP_DPI_X(4);
	rColor.right  = rColor.left + m_sizeColor.cx;
	return rColor;
}

void CXTPCalendarEventColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC		 = CDC::FromHandle(lpDIS->hDC);
	UINT itemState   = lpDIS->itemState;
	UINT itemID		 = lpDIS->itemID;
	CRect rcItem	 = lpDIS->rcItem;
	DWORD_PTR dwData = lpDIS->itemData;

	if (itemID == (UINT)-1 || !m_pData)
	{
		return;
	}

	BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED) && (m_pWndEdit == NULL);
	BOOL bFocus	= ((itemState & ODS_FOCUS) == ODS_FOCUS) && (m_pWndEdit == NULL);

	// fill the background.
	pDC->FillSolidRect(&rcItem, ::GetSysColor(bDisabled ? COLOR_3DFACE : COLOR_WINDOW));

	// determine the size of the color rectangle.
	CRect rColor(CalcColorRect(rcItem));

	pDC->SetBkColor(
		::GetSysColor(bDisabled ? COLOR_3DFACE : bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	pDC->SetTextColor(::GetSysColor(
		bDisabled ? COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

	COLORREF clrBack;
	COLORREF clrEdge;

	CString strName;
	CRect rFocus(rcItem);

	if (m_bCategories)
	{
		CXTPCalendarEventCategory* pCategory = (CXTPCalendarEventCategory*)dwData;

		if (!pCategory)
			return;

		if (m_pEvent)
		{
			CRect rCheck(rColor);
			rColor.OffsetRect(m_sizeColor.cx + XTP_DPI_X(3), 0);

			BOOL bChecked = (m_arChecked.Find(pCategory->GetID()) >= 0);

			if (bChecked)
			{
				XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolCheckMark, rCheck,
												 ::GetSysColor(bDisabled ? COLOR_GRAYTEXT
																		 : COLOR_WINDOWTEXT));
			}

			clrEdge = ::GetSysColor(COLOR_3DSHADOW);
			pDC->Draw3dRect(rCheck, clrEdge, clrEdge);
		}

		strName = pCategory->GetName();
		clrBack = pCategory->GetBkBaseColor();
		clrEdge = pCategory->GetBorderColor();

		rFocus.left  = rColor.right + XTP_DPI_X(2);
		rFocus.right = rFocus.left + pDC->GetTextExtent(strName).cx + +XTP_DPI_X(10);
	}
	else
	{
		CXTPCalendarEventLabel* pLabel = (CXTPCalendarEventLabel*)dwData;

		if (!pLabel)
			return;

		strName = pLabel->m_strName;
		clrBack = pLabel->m_clrColor;
		clrEdge = CXTPDrawHelpers::DarkenColorLum(pLabel->m_clrColor, 0.5);
	}

	if (bSelected && !bDisabled)
	{
		// draw highlight rectangle.
		pDC->FillSolidRect(&rFocus, ::GetSysColor(COLOR_HIGHLIGHT));

		// draw focus rectangle.
		if (bFocus)
		{
			pDC->DrawFocusRect(&rFocus);
		}
	}

	// draw color rectangle.
	CXTPDrawHelpers::DrawColorRect(pDC, rColor,
								   bDisabled ? CXTPDrawHelpers::GreyScaleColor(clrBack) : clrBack,
								   bDisabled ? CXTPDrawHelpers::GreyScaleColor(clrEdge) : clrEdge);

	// determine the size of the text display.
	CRect rText(rcItem);
	rText.left = rColor.right + XTP_DPI_X(6);

	// draw text.
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strName, rText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CXTPCalendarEventColorListBox::ToggleCheck(int nItem)
{
	if (m_pEvent)
	{
		CXTPCalendarEventCategory* pCategory = (CXTPCalendarEventCategory*)GetItemData(nItem);

		if (pCategory)
		{
			int nIndex = m_arChecked.Find(pCategory->GetID());

			if (nIndex < 0)
			{
				m_arChecked.InsertAt(0, pCategory->GetID());
			}
			else
			{
				m_arChecked.RemoveAt(nIndex);
			}
		}
	}
}

void CXTPCalendarEventColorListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bCategories)
	{
		BOOL bOutside;
		int nItem = XTPToIntChecked(ItemFromPoint(point, bOutside));

		if (!bOutside)
		{
			CRect rItem;
			GetItemRect(nItem, &rItem);

			CRect rCheck = CalcColorRect(rItem);
			if (rCheck.PtInRect(point))
			{
				ToggleCheck(nItem);
				InvalidateRect(&rCheck);

				SAFE_DELETE(m_pWndEdit);
				return;
			}
		}
	}

	if (m_pWndEdit)
	{
		SAFE_DELETE(m_pWndEdit);
		CListBox::OnLButtonDown(nFlags, point);
	}
	else
	{
		int nCurSel = GetCurSel();

		CListBox::OnLButtonDown(nFlags, point);

		int nNewSel = GetCurSel();

		if (nCurSel == nNewSel)
		{
			BeginEdit();
		}
	}
}

void CXTPCalendarEventColorListBox::BeginEdit()
{
	int nIndex = GetCurSel();

	if (nIndex == LB_ERR)
		return;

	if (::IsWindow(m_hWnd))
	{
		CString strName;

		if (m_bCategories)
		{
			CXTPCalendarEventCategory* pCategory = GetCategory(nIndex);

			if (!pCategory)
				return;

			strName = pCategory->GetName();
		}
		else
		{
			CXTPCalendarEventLabel* pLabel = GetLabel(nIndex);

			if (!pLabel)
				return;

			strName = pLabel->m_strName;
		}

		CRect rEdit;
		if (GetItemRect(nIndex, &rEdit) == LB_ERR)
			return;

		rEdit.left += m_sizeColor.cx + XTP_DPI_X(10);

		if (m_bCategories)
		{
			rEdit.left += m_sizeColor.cx + XTP_DPI_X(3);
		}

		SAFE_DELETE(m_pWndEdit);

		m_pWndEdit = new CXTPCalendarEventColorEdit(nIndex);

		if (m_pWndEdit->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, rEdit,
							   this, 0xff))
		{
			m_pWndEdit->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
			m_pWndEdit->SetFont(GetFont());
			m_pWndEdit->SetWindowText(strName);
			m_pWndEdit->SetSel(0, -1, TRUE);
			m_pWndEdit->SetFocus();
		}
	}
}

void CXTPCalendarEventColorListBox::EndEdit(BOOL bUpdate /*=TRUE*/)
{
	if (::IsWindow(m_pWndEdit->GetSafeHwnd()))
	{
		int nIndex = m_pWndEdit->GetIndex();

		if (bUpdate && nIndex != LB_ERR)
		{
			CString strEdit;
			m_pWndEdit->GetWindowText(strEdit);

			if (m_bCategories)
			{
				CXTPCalendarEventCategory* pCategory = GetCategory(nIndex);

				if (pCategory && pCategory->GetName() != strEdit)
				{
					pCategory->SetName(strEdit);
					SetModified(TRUE);
				}
			}
			else
			{
				CXTPCalendarEventLabel* pLabel = GetLabel(nIndex);

				if (pLabel && pLabel->m_strName != strEdit)
				{
					pLabel->m_strName = strEdit;
					SetModified(TRUE);
				}
			}
		}
	}

	SAFE_DELETE(m_pWndEdit);
}

void CXTPCalendarEventColorListBox::DeleteItem(int nItem /*=LB_ERR*/)
{
	if (!::IsWindow(m_hWnd))
		return;

	if (nItem == LB_ERR)
		nItem = GetCurSel();

	if (nItem != LB_ERR)
	{
		DeleteString(XTPToUIntChecked(nItem));
		SetModified(TRUE);

		if (GetCount() > 0)
		{
			SetCurSel((nItem > 0) ? nItem - 1 : 0);
		}
		else
		{
			SetCurSel(LB_ERR);
		}

		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	}
}

int CXTPCalendarEventColorListBox::AddItem()
{
	if (!::IsWindow(m_hWnd))
		return LB_ERR;

	CXTPCalendarEventColorNewDlg dlg(m_bCategories, this);

	if (dlg.DoModal() == IDOK)
	{
		int nItem = AddString(dlg.m_strName);

		if (nItem == LB_ERR)
			return LB_ERR;

		if (m_bCategories)
		{
			SetItemData(nItem,
						XTPToUIntPtr(new CXTPCalendarEventCategory(
							XTPToUIntChecked(nItem + 1), dlg.m_strName,
							CXTPDrawHelpers::DarkenColorLum(dlg.m_crColor, 0.5), dlg.m_crColor)));

			ToggleCheck(nItem);
			SetModified(TRUE);
		}
		else
		{
			SetItemData(nItem, XTPToUIntPtr(new CXTPCalendarEventLabel(nItem, dlg.m_crColor,
																	   dlg.m_strName)));

			SetModified(TRUE);
		}

		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

		return nItem;
	}

	return LB_ERR;
}

void CXTPCalendarEventColorListBox::SetItemColor(COLORREF crItem, int nItem /*=LB_ERR*/)
{
	if (!::IsWindow(m_hWnd))
		return;

	if (nItem == LB_ERR)
		nItem = GetCurSel();

	if (nItem != LB_ERR)
	{
		if (m_bCategories)
		{
			CXTPCalendarEventCategory* pCategory = GetCategory(nItem);

			if (!pCategory)
				return;

			pCategory->SetBkBaseColor(crItem);
			SetModified(TRUE);
		}
		else
		{
			CXTPCalendarEventLabel* pLabel = GetLabel(nItem);

			if (!pLabel)
				return;

			pLabel->m_clrColor = crItem;
			SetModified(TRUE);
		}

		CRect rItem;
		GetItemRect(nItem, &rItem);
		InvalidateRect(&rItem);
	}
}

void CXTPCalendarEventColorListBox::DeleteItem(LPDELETEITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_LISTBOX);

	if (m_bCategories)
	{
		CXTPCalendarEventCategory* pCategory = (CXTPCalendarEventCategory*)lpDIS->itemData;
		SAFE_DELETE(pCategory);
	}
	else
	{
		CXTPCalendarEventLabel* pLabel = (CXTPCalendarEventLabel*)lpDIS->itemData;
		SAFE_DELETE(pLabel);
	}

	CListBox::DeleteItem(lpDIS);
}

CXTPCalendarEventLabel* CXTPCalendarEventColorListBox::GetLabel(int nIndex)
{
	return m_bCategories ? NULL : (CXTPCalendarEventLabel*)GetItemData(nIndex);
}

CXTPCalendarEventCategory* CXTPCalendarEventColorListBox::GetCategory(int nIndex)
{
	return m_bCategories ? (CXTPCalendarEventCategory*)GetItemData(nIndex) : NULL;
}

void CXTPCalendarEventColorListBox::UpdateCategories(CXTPCalendarData* pData)
{
	if (!pData)
		return;

	if (IsModified())
	{
		// remove all of the categories from the data provider.
		pData->ClearEventCategoryList();

		// add new categories based on user settings.
		for (int i = 0; i < GetCount(); i++)
		{
			CXTPCalendarEventCategory* pCategory = GetCategory(i);
			if (pCategory)
			{
				pData->AddEventCategory(pCategory->GetID(), pCategory->GetName(),
										pCategory->GetBorderColor(), pCategory->GetBkBaseColor());
			}
		}
	}

	// update the event with the selected categories.
	CXTPCalendarEventCategoryIDs* pCategoryIDs = XTP_SAFE_GET1(m_pEvent, GetCategories(), NULL);
	if (pCategoryIDs)
	{
		pCategoryIDs->RemoveAll();
		pCategoryIDs->Copy(m_arChecked);
	}
}

void CXTPCalendarEventColorListBox::UpdateLabels(CXTPCalendarData* pData)
{
	if (!pData)
		return;

	int nIndex = GetCurSel();

	if (IsModified())
	{
		// remove all of the labels from the data provider.
		pData->ClearEventLabelList();
	}

	// add new labels based on user settings.
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPCalendarEventLabel* pLabel = GetLabel(i);
		if (pLabel)
		{
			if (IsModified())
			{
				pData->AddEventLabel(pLabel->m_nLabelID, pLabel->m_strName, pLabel->m_clrColor);
			}

			if (i == nIndex)
			{
				// update the event with selected label.
				m_pEvent->SetLabelID(pLabel->m_nLabelID);
			}
		}
	}
}

HBRUSH CXTPCalendarEventColorListBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(nCtlColor);

	return (IsWindowEnabled() ? NULL : ::GetSysColorBrush(COLOR_3DFACE));
}
