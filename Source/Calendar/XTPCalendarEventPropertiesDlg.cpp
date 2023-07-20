// XTPCalendarEventPropertiesDlg.cpp: implementation of the CXTPCalendarEventPropertiesDlg.
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Calendar/Resource.h"

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
#include "Common/XTPColorManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarRecurrencePattern.h"

#include "Calendar/XTPCalendarResource.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewDay.h"
#include "Calendar/XTPCalendarEventLabel.h"
#include "Calendar/XTPCalendarTheme.h"

#include "Calendar/XTPCalendarOccurSeriesChooseDlg.h"
#include "Calendar/XTPCalendarEventPropertiesDlg.h"
#include "Calendar/XTPCalendarEventRecurrenceDlg.h"
#include "Calendar/XTPCalendarPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarComboBoxBase

CXTPCalendarComboBoxBase::CXTPCalendarComboBoxBase()
{
	m_pEvent = NULL;
	m_nTheme = xtpCalendarThemeUnknown;
}

CXTPCalendarComboBoxBase::~CXTPCalendarComboBoxBase()
{
}

BOOL CXTPCalendarComboBoxBase::Init(CXTPCalendarEvent* pEvent, XTPCalendarTheme nTheme)
{
	m_pEvent = pEvent;
	m_nTheme = nTheme;

	CRect r;
	GetClientRect(&r);
	m_sizeColor.cx = r.Height() - XTP_DPI_X(3) * 2;
	m_sizeColor.cy = r.Height() - XTP_DPI_Y(3) * 2;

	if (!m_pEvent)
		return FALSE;

	if (::IsWindow(m_hWnd))
		ResetContent();

	return TRUE;
}

void CXTPCalendarComboBoxBase::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_COMBOBOX);
	lpMIS->itemHeight = XTPToUIntChecked(m_sizeColor.cy + XTP_DPI_Y(6));
	lpMIS->itemWidth  = 0;
}

CRect CXTPCalendarComboBoxBase::CalcColorRect(CRect rItem)
{
	CRect rColor(rItem);
	rColor.top	= rItem.top + (rItem.Height() - m_sizeColor.cy) / 2;
	rColor.bottom = rColor.top + m_sizeColor.cy;
	rColor.left   = rItem.left + XTP_DPI_X(4);
	rColor.right  = rColor.left + m_sizeColor.cx;
	return rColor;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventCategoryComboBox

CXTPCalendarEventCategoryComboBox::CXTPCalendarEventCategoryComboBox()
{
	m_nLastKey	= 0;
	m_pCategories = NULL;
}

CXTPCalendarEventCategoryComboBox::~CXTPCalendarEventCategoryComboBox()
{
	CMDTARGET_RELEASE(m_pCategories);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarEventCategoryComboBox, CXTPCalendarComboBoxBase)
	//{{AFX_MSG_MAP(CXTPCalendarEventCategoryComboBox)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelChange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPCalendarEventCategoryComboBox::Init(CXTPCalendarEvent* pEvent, XTPCalendarTheme nTheme)
{
	if (!CXTPCalendarComboBoxBase::Init(pEvent, nTheme))
		return FALSE;

	CXTPCalendarEventCategories* pCategories = XTP_SAFE_GET2(pEvent, GetDataProvider(),
															 GetEventCategories(), NULL);
	if (!pCategories)
	{
		ASSERT(0);
		return FALSE;
	}

	CXTPCalendarEventCategoryIDs* pChecked = XTP_SAFE_GET1(pEvent, GetCategories(), NULL);
	if (!pChecked)
	{
		ASSERT(0);
		return FALSE;
	}

	m_pCategories = pCategories;
	CMDTARGET_ADDREF(m_pCategories);

	int nCount = m_pCategories->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEventCategory* pCat = m_pCategories->GetAt(i);
		ASSERT(pCat);
		if (pCat)
		{
			int nIdx = AddString(pCat->GetName());
			SetItemDataPtr(nIdx, (void*)pCat);
		}
	}

	m_arChecked.RemoveAll();
	m_arChecked.Copy(*pChecked);

	return TRUE;
}

void CXTPCalendarEventCategoryComboBox::GetChecked(CXTPCalendarEventCategoryIDs* pChecked,
												   BOOL* pbChanged)
{
	if (!pChecked)
		return;

	if (pbChanged)
	{
		int nCount = m_arChecked.GetSize();
		if (nCount == pChecked->GetSize())
		{
			for (int i = 0; i < nCount; i++)
			{
				if (m_arChecked[i] != pChecked->GetAt(i))
				{
					*pbChanged |= TRUE;
					break;
				}
			}
		}
		else
		{
			*pbChanged |= TRUE;
		}
	}

	pChecked->RemoveAll();
	pChecked->Copy(m_arChecked);
}

void CXTPCalendarEventCategoryComboBox::DrawCollapsed(CDC* pDC, CRect rcItem, BOOL bFocused,
													  BOOL bDisabled)
{
	if (!m_pCategories)
		return;

	// fill the background.
	pDC->FillSolidRect(&rcItem,
					   ::GetSysColor(bDisabled ? COLOR_3DFACE
											   : bFocused ? COLOR_HIGHLIGHT : COLOR_WINDOW));

	// determine the size of the color rectangle.
	CRect rColor(CalcColorRect(rcItem));

	pDC->SetBkColor(
		::GetSysColor(bDisabled ? COLOR_3DFACE : bFocused ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	pDC->SetTextColor(::GetSysColor(bDisabled ? COLOR_GRAYTEXT
											  : bFocused ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

	int nSize = m_arChecked.GetSize();

	for (int i = 0; i < nSize; i++)
	{
		CXTPCalendarEventCategory* pCategory = m_pCategories->Find(m_arChecked.GetAt(i));

		if (!pCategory)
			continue;

		if (bFocused)
		{
			CRect rColor2 = rColor;
			rColor2.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

			if (m_nTheme == xtpCalendarThemeResource)
			{
				CXTPCalendarTheme::DrawRoundRect(pDC, rColor2, ::GetSysColor(COLOR_WINDOW), TRUE,
												 CBrush::FromHandle(
													 (HBRUSH)::GetStockObject(LTGRAY_BRUSH)));
			}
			else
			{
				pDC->Draw3dRect(rColor2, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_WINDOW));
			}
		}

		if (m_nTheme == xtpCalendarThemeResource)
		{
			CXTPCalendarTheme::DrawRoundRect(pDC, rColor, pCategory->GetBorderColor(), FALSE,
											 pCategory->GetBackgroundColor());
		}
		else
		{
			CXTPDrawHelpers::DrawColorRect(pDC, rColor, pCategory->GetBackgroundColor(),
										   pCategory->GetBorderColor());
		}

		if (nSize == 1)
		{
			// draw text.
			CRect rText(rcItem);
			rText.left = rColor.right + XTP_DPI_X(6);

			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(pCategory->GetName(), rText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			rColor.OffsetRect(m_sizeColor.cx + XTP_DPI_X(3), 0);
		}
	}
}

void CXTPCalendarEventCategoryComboBox::DrawExpanded(CDC* pDC, CRect rcItem, BOOL bSelected,
													 UINT uItemID)
{
	if (uItemID == (UINT)-1)
		return;

	// fill the background.
	pDC->FillSolidRect(&rcItem, ::GetSysColor(bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW));

	// determine the size of the color rectangle.
	CRect rColor(CalcColorRect(rcItem));

	COLORREF crBackColor = ::GetSysColor(bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW);
	pDC->SetBkColor(crBackColor);

	COLORREF crTextColor = ::GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT);
	pDC->SetTextColor(crTextColor);

	CXTPCalendarEventCategory* pCategory = (CXTPCalendarEventCategory*)GetItemDataPtr(
		XTPToInt(uItemID));

	if (!pCategory)
		return;

	if (bSelected)
	{
		pDC->DrawFocusRect(&rcItem);
	}

	CRect rCheck(rColor);
	rColor.OffsetRect(m_sizeColor.cx + XTP_DPI_X(3), 0);

	BOOL bChecked = (m_arChecked.Find(pCategory->GetID()) >= 0);

	if (bChecked)
	{
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolCheckMark, rCheck,
										 ::GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT
																 : COLOR_WINDOWTEXT));
	}

	pDC->Draw3dRect(rCheck, ::GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_3DSHADOW),
					::GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_3DSHADOW));

	CString strName = pCategory->GetName();

	if (m_nTheme == xtpCalendarThemeResource)
	{
		if (bSelected)
		{
			CRect rBorder(rColor);
			rBorder.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

			CXTPCalendarTheme::DrawRoundRect(pDC, rBorder, ::GetSysColor(COLOR_HIGHLIGHTTEXT),
											 FALSE,
											 CBrush::FromHandle(
												 (HBRUSH)GetStockObject(LTGRAY_BRUSH)));
		}

		CXTPCalendarTheme::DrawRoundRect(pDC, rColor, pCategory->GetBorderColor(), FALSE,
										 pCategory->GetBackgroundColor());
	}
	else
	{
		if (bSelected)
		{
			CRect rBorder(rColor);
			rBorder.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

			pDC->Draw3dRect(rBorder, ::GetSysColor(COLOR_HIGHLIGHTTEXT),
							::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}

		CXTPDrawHelpers::DrawColorRect(pDC, rColor, pCategory->GetBackgroundColor(),
									   pCategory->GetBorderColor());
	}

	// determine the size of the text display.
	CRect rText(rcItem);
	rText.left = rColor.right + XTP_DPI_X(6);

	// draw text.
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strName, rText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

void CXTPCalendarEventCategoryComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ((lpDIS->itemState & ODS_COMBOBOXEDIT) == ODS_COMBOBOXEDIT)
	{
		DrawCollapsed(CDC::FromHandle(lpDIS->hDC), lpDIS->rcItem, (GetFocus() == this),
					  ((lpDIS->itemState & ODS_DISABLED) == ODS_DISABLED));
	}
	else
	{
		DrawExpanded(CDC::FromHandle(lpDIS->hDC), lpDIS->rcItem,
					 ((lpDIS->itemState & ODS_FOCUS) == ODS_FOCUS), lpDIS->itemID);
	}
}

int CXTPCalendarEventCategoryComboBox::GetLBCurSel() const
{
	if (GetDroppedState() == TRUE)
	{
		HWND hWnd = ::FindWindow(_T("ComboLBox"), NULL);
		if (::IsWindow(hWnd))
		{
			return (int)::SendMessage(hWnd, LB_GETCURSEL, 0, 0);
		}
	}
	return LB_ERR;
}

void CXTPCalendarEventCategoryComboBox::RedrawLB(BOOL bUpdateNow)
{
	if (GetDroppedState() == TRUE)
	{
		HWND hWnd = ::FindWindow(_T("ComboLBox"), NULL);
		if (::IsWindow(hWnd))
		{
			if (bUpdateNow)
				::UpdateWindow(hWnd);
			else
				::InvalidateRect(hWnd, NULL, FALSE);
		}
	}
}

void CXTPCalendarEventCategoryComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE && GetDroppedState())
	{
		_OnSelChange(GetLBCurSel());

		Invalidate(FALSE);
		RedrawLB();

		m_nLastKey = nChar;

		return;
	}

	if (nChar == VK_RETURN && m_nLastKey == VK_SPACE)
	{
		ShowDropDown(FALSE);
		Invalidate(FALSE);

		return;
	}

	m_nLastKey = nChar;

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CXTPCalendarEventCategoryComboBox::OnDropDown()
{
	SetCurSel(CB_ERR);
	m_nLastKey = 0;
}

void CXTPCalendarEventCategoryComboBox::OnSelChange()
{
	if (GetDroppedState())
		_OnSelChange(GetCurSel());
}

void CXTPCalendarEventCategoryComboBox::_OnSelChange(int nItemIndex)
{
	if (!m_pCategories || nItemIndex == CB_ERR || GetDroppedState() == FALSE)
		return;

	CXTPCalendarEventCategory* pCat = (CXTPCalendarEventCategory*)GetItemDataPtr(nItemIndex);
	ASSERT(pCat);
	if (!pCat)
		return;

	int nFIdx = m_arChecked.Find(pCat->GetID());

	if (nFIdx < 0)
		m_arChecked.InsertAt(0, pCat->GetID());
	else
		m_arChecked.RemoveAt(nFIdx);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarBusyStatusComboBox

BOOL CXTPCalendarBusyStatusComboBox::Init(CXTPCalendarEvent* pEvent, XTPCalendarTheme nTheme)
{
	if (!CXTPCalendarComboBoxBase::Init(pEvent, nTheme))
		return FALSE;

	SetItemData(AddString(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_TIME_STATUS_FREE)),
				(DWORD_PTR)XTP_IDS_CALENDAR_TIME_STATUS_FREE);
	SetItemData(AddString(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_TIME_STATUS_ELSEWHERE)),
				(DWORD_PTR)XTP_IDS_CALENDAR_TIME_STATUS_ELSEWHERE);
	SetItemData(AddString(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_TIME_STATUS_TENTATIVE)),
				(DWORD_PTR)XTP_IDS_CALENDAR_TIME_STATUS_TENTATIVE);
	SetItemData(AddString(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_TIME_STATUS_BUSY)),
				(DWORD_PTR)XTP_IDS_CALENDAR_TIME_STATUS_BUSY);
	SetItemData(AddString(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_TIME_STATUS_OUTOFOFFICE)),
				(DWORD_PTR)XTP_IDS_CALENDAR_TIME_STATUS_OUTOFOFFICE);

	return TRUE;
}

void CXTPCalendarBusyStatusComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC	   = CDC::FromHandle(lpDIS->hDC);
	UINT itemState = lpDIS->itemState;
	UINT itemID	= lpDIS->itemID;
	CRect rcItem   = lpDIS->rcItem;

	if (itemID == (UINT)-1)
	{
		return;
	}

	BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED);
	BOOL bFocus	= ((itemState & ODS_FOCUS) == ODS_FOCUS);

	// draw background.
	if (bDisabled)
	{
		pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
		pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_3DFACE));
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_WINDOW));
		}

		// draw focus rectangle.
		if (bFocus)
		{
			pDC->DrawFocusRect(&rcItem);
		}
	}

	// determine the size of the color rectangle.
	CRect rColor(rcItem);
	rColor.top	= rcItem.top + (rcItem.Height() - m_sizeColor.cy) / 2;
	rColor.bottom = rColor.top + m_sizeColor.cy;
	rColor.left   = rcItem.left + XTP_DPI_X(5);
	rColor.right  = rColor.left + m_sizeColor.cx;

	BOOL bDarkEdge = (m_nTheme < xtpCalendarThemeOffice2013);

	COLORREF crLight	= RGB(147, 123, 209);
	COLORREF crDark		= RGB(129, 61, 95);
	COLORREF crDarkEdge = ::GetSysColor(COLOR_3DDKSHADOW);

	if (bDisabled)
	{
		crLight	= ::GetSysColor(COLOR_GRAYTEXT);
		crDark	 = ::GetSysColor(COLOR_GRAYTEXT);
		crDarkEdge = ::GetSysColor(COLOR_GRAYTEXT);

		pDC->SetTextColor(crDark);
	}

	// draw color rectangle.
	switch (itemID)
	{
		case xtpCalendarBusyStatusFree:
		{
			CXTPDrawHelpers::DrawColorRect(pDC, rColor, RGB(255, 255, 255),
										   bDarkEdge ? crDarkEdge : crLight);
			break;
		}

		case xtpCalendarBusyStatusElsewhere:
		{
			WORD bitsBrush[] = { 0x77, 0xff, 0xdd, 0xff, 0x77, 0xff, 0xdd, 0xff };
			CXTPDrawHelpers::DrawBrushRect(pDC, rColor, RGB(255, 255, 255),
										   bDarkEdge ? crDarkEdge : crLight, bitsBrush);
			break;
		}

		case xtpCalendarBusyStatusTentative:
		{
			WORD bitsBrush[] = { 0x77, 0xee, 0xdd, 0xbb, 0x77, 0xee, 0xdd, 0xbb };
			CXTPDrawHelpers::DrawBrushRect(pDC, rColor, RGB(255, 255, 255),
										   bDarkEdge ? crDarkEdge : crLight, bitsBrush);
			break;
		}

		case xtpCalendarBusyStatusBusy:
		{
			CXTPDrawHelpers::DrawColorRect(pDC, rColor, crLight, bDarkEdge ? crDarkEdge : crLight);
			break;
		}

		case xtpCalendarBusyStatusOutOfOffice:
		{
			CXTPDrawHelpers::DrawColorRect(pDC, rColor, crDark, bDarkEdge ? crDarkEdge : crDark);
			break;
		}
	}

	CString strItem = CXTPCalendarUtils::LoadString((UINT)lpDIS->itemData);

	// determine the size of the text display.
	CRect rText(rcItem);
	rText.left = rColor.right + XTP_DPI_X(6);

	// draw text.
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strItem, rText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventLabelComboBox

BEGIN_MESSAGE_MAP(CXTPCalendarEventLabelComboBox, CComboBox)
	//{{AFX_MSG_MAP(CXTPCalendarEventLabelComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPCalendarEventLabelComboBox::Init(CXTPCalendarEvent* pEvent, XTPCalendarTheme nTheme)
{
	if (!CXTPCalendarComboBoxBase::Init(pEvent, nTheme))
		return FALSE;

	CXTPCalendarEventLabels* pLabels = XTP_SAFE_GET2(pEvent, GetDataProvider(), GetLabelList(),
													 NULL);
	if (!pLabels)
		return FALSE;

	for (int i = 0; i < pLabels->GetCount(); i++)
	{
		CXTPCalendarEventLabel* pLabel = pLabels->GetAt(i);
		if (pLabel)
		{
			SetItemData(AddString(pLabel->m_strName), (DWORD_PTR)pLabel);
		}
	}

	return TRUE;
}

int CXTPCalendarEventLabelComboBox::GetLabelID() const
{
	return GetLabelID(GetCurSel());
}

int CXTPCalendarEventLabelComboBox::GetLabelID(int nID) const
{
	if (nID != CB_ERR)
	{
		CXTPCalendarEventLabel* pLabel = (CXTPCalendarEventLabel*)GetItemData(nID);
		if (pLabel)
		{
			return pLabel->m_nLabelID;
		}
	}

	return CB_ERR;
}

void CXTPCalendarEventLabelComboBox::SetLabelID(int nID)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (nID == GetLabelID(i))
		{
			SetCurSel(i);
			return;
		}
	}

	SetCurSel(CB_ERR);
}

void CXTPCalendarEventLabelComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC		 = CDC::FromHandle(lpDIS->hDC);
	UINT itemState   = lpDIS->itemState;
	UINT itemID		 = lpDIS->itemID;
	CRect rcItem	 = lpDIS->rcItem;
	DWORD_PTR dwData = lpDIS->itemData;

	if (itemID == (UINT)-1)
	{
		return;
	}

	BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED);
	BOOL bFocus	= ((itemState & ODS_FOCUS) == ODS_FOCUS);

	// draw background.
	if (bDisabled)
	{
		pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
		pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_3DFACE));
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, ::GetSysColor(COLOR_WINDOW));
		}

		// draw focus rectangle.
		if (bFocus)
		{
			pDC->DrawFocusRect(&rcItem);
		}
	}

	// determine the size of the color rectangle.
	CRect rColor(rcItem);
	rColor.top	= rcItem.top + (rcItem.Height() - m_sizeColor.cy) / 2;
	rColor.bottom = rColor.top + m_sizeColor.cy;
	rColor.left   = rcItem.left + XTP_DPI_X(5);
	rColor.right  = rColor.left + m_sizeColor.cx;

	// draw color rectangle.
	CXTPCalendarEventLabel* pLabel = (CXTPCalendarEventLabel*)dwData;
	if (pLabel)
	{
		BOOL bDarkEdge = (m_nTheme < xtpCalendarThemeOffice2013);

		COLORREF crBack = pLabel->m_clrColor;
		COLORREF crEdge = bDarkEdge ? ::GetSysColor(COLOR_3DDKSHADOW)
									: CXTPDrawHelpers::DarkenColorLum(pLabel->m_clrColor, 0.5);

		if (bDisabled)
		{
			crBack = ::GetSysColor(COLOR_3DFACE);
			crEdge = ::GetSysColor(COLOR_GRAYTEXT);

			pDC->SetTextColor(crEdge);
		}

		CXTPDrawHelpers::DrawColorRect(pDC, rColor, crBack, crEdge);

		// determine the size of the text display.
		CRect rText(rcItem);
		rText.left = rColor.right + XTP_DPI_X(6);

		// draw text.
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(pLabel->m_strName, rText,
					  DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CXTPCalendarEventPropertiesDlg, CDialog)

CXTPCalendarEventPropertiesDlg::CXTPCalendarEventPropertiesDlg(CXTPCalendarControl* pControl,
															   BOOL bAddEvent, CWnd* pParent,
															   CXTPCalendarEvent* pEditingEvent,
															   UINT nIDTemplate,
															   COleDateTime dtInitial)
{
	InitModalIndirect(XTPResourceManager()->LoadDialogTemplate(nIDTemplate), pParent);

	m_nAllDayEventSet = -1;

	m_bAddEvent = bAddEvent;

	m_bAllDayEvent = FALSE;
	m_bPrivate	 = FALSE;
	m_bMeeting	 = FALSE;
	m_bReminder	= FALSE;
	m_nBusyStatus  = xtpCalendarBusyStatusUnknown;
	m_bOccurrence  = TRUE;

	m_dNewEventID = XTP_CALENDAR_UNKNOWN_EVENT_ID;

	CXTPCalendarData* pData = XTP_SAFE_GET1(pControl, GetDataProvider(), NULL);
	if (!pControl || !pData)
	{
		ASSERT(FALSE);
		return;
	}

	m_ptrControl = pControl;
	m_nTheme	 = XTP_SAFE_GET2(m_ptrControl, GetPaintManager(), GetPaintTheme(),
							 xtpCalendarThemeUnknown);

	m_ptrEditingEvent.SetPtr(pEditingEvent, TRUE);
	if (!pEditingEvent)
	{
		m_ptrEditingEvent = pData->CreateNewEvent();
		if (m_ptrEditingEvent)
		{
			CXTPCalendarView* pView = m_ptrControl->GetActiveView();
			ASSERT(pView);

			if (pView)
			{
				COleDateTime dtSelBegin, dtSelEnd;
				BOOL bAllDayEvent = FALSE;
				int nGroupIndex   = 0;

				if (!pView->GetSelection(&dtSelBegin, &dtSelEnd, &bAllDayEvent, &nGroupIndex))
				{
					dtSelBegin = COleDateTime::GetCurrentTime();

					if (fabs(dtSelBegin - dtInitial) > COleDateTimeSpan(0, 0, 0, 30))
						dtSelBegin = dtInitial;

					dtSelEnd	= dtSelBegin + COleDateTimeSpan(0, 1, 0, 0);
					nGroupIndex = 0;
				}

				UINT uSchID = XTP_SAFE_GET3(pView, GetViewDay_(0), GetViewGroup_(nGroupIndex),
											GetScheduleID(), XTP_CALENDAR_UNKNOWN_SCHEDULE_ID);

				m_ptrEditingEvent->SetStartTime(min(dtSelBegin, dtSelEnd));
				m_ptrEditingEvent->SetEndTime(max(dtSelBegin, dtSelEnd));
				m_ptrEditingEvent->SetScheduleID(uSchID);

				m_nAllDayEventSet = bAllDayEvent ? 1 : 0;

				if (!bAllDayEvent)
					m_ptrEditingEvent->SetBusyStatus(xtpCalendarBusyStatusBusy);
			}
		}
	}

	if (m_ptrEditingEvent)
	{
		m_dtStartDate = m_ptrEditingEvent->GetStartTime();
		m_dtStartTime = m_dtStartDate;

		m_dtEndDate = m_ptrEditingEvent->GetEndTime();
		m_dtEndTime = m_dtEndDate;

		if (m_ptrEditingEvent->IsAllDayEvent() && CXTPCalendarUtils::IsZeroTime(m_dtEndDate))
		{
			AddEndDate_ResetTime(m_dtStartDate, m_dtEndDate, -1);
		}
	}

	RepairCheckedEvent();
}

void CXTPCalendarEventPropertiesDlg::RepairCheckedEvent()
{
	CXTPCalendarEventCategories* pCategories = XTP_SAFE_GET2(m_ptrEditingEvent, GetDataProvider(),
															 GetEventCategories(), NULL);
	if (!pCategories)
	{
		ASSERT(0);
		return;
	}

	CXTPCalendarEventCategoryIDs* pChecked = XTP_SAFE_GET1(m_ptrEditingEvent, GetCategories(),
														   NULL);
	if (!pChecked)
	{
		ASSERT(0);
		return;
	}

	for (int i = pChecked->GetSize() - 1; i >= 0; i--)
	{
		if (!pCategories->Find(pChecked->GetAt(i)))
		{
			pChecked->RemoveAt(i);
		}
	}
}

#if _MSC_VER < 1200
AFX_INLINE void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, COleDateTime& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		CXTPDateTimeHelper::GetDateTimeCtrlTime(hWndCtrl, value);
	else
		CXTPDateTimeHelper::SetDateTimeCtrlTime(hWndCtrl, value);
}
#endif

void CXTPCalendarEventPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, XTP_IDC_COMBO_SCHEDULEID, m_ctrlScheduleIDCB);

	if (m_ptrControl != NULL && m_ptrControl->m_bMultipleSchedulesMode)
	{
		if (m_ctrlScheduleSet.m_hWnd)
			DDX_Control(pDX, XTP_IDC_LABEL + 1, m_ctrlScheduleSet);
	}

	DDX_Control(pDX, XTP_IDC_SCHEDULEID_TEXT, m_ctrlScheduleLabel);
	DDX_Control(pDX, XTP_IDC_COMBO_EVENT_REMINDER, m_wndReminderCombo);
	DDX_Control(pDX, XTP_IDC_COMBO_EVENT_BUSYSTATE, m_wndBusyStatusCombo);
	DDX_Control(pDX, XTP_IDC_COMBO_EVENT_LABEL, m_wndLabelCombo);
	DDX_Control(pDX, XTP_IDC_COMBO_EVENT_CATEGORY, m_wndCategoryCombo);

	DDX_Text(pDX, XTP_IDC_EDIT_SUBJECT, m_strSubject);
	DDX_Text(pDX, XTP_IDC_EDIT_LOCATION, m_strLocation);
	DDX_Text(pDX, XTP_IDC_EDIT_BODY, m_strBody);
	DDX_DateTimeCtrl(pDX, XTP_IDC_DTP_START_DATE, m_dtStartDate);
	DDX_DateTimeCtrl(pDX, XTP_IDC_DTP_START_TIME, m_dtStartTime);
	DDX_DateTimeCtrl(pDX, XTP_IDC_DTP_END_DATE, m_dtEndDate);
	DDX_DateTimeCtrl(pDX, XTP_IDC_DTP_END_TIME, m_dtEndTime);

	DDX_CBIndex(pDX, XTP_IDC_COMBO_EVENT_BUSYSTATE, m_nBusyStatus);

	DDX_Check(pDX, XTP_IDC_CHECK_ALLDAY_EVENT, m_bAllDayEvent);
	DDX_Check(pDX, XTP_IDC_CHECK_PRIVATE, m_bPrivate);
	DDX_Check(pDX, XTP_IDC_CHECK_MEETING, m_bMeeting);
	DDX_Check(pDX, XTP_IDC_CHECK_REMINDER, m_bReminder);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCalendarEventPropertiesDlg, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(XTP_IDC_CHECK_REMINDER, OnCheckReminder)
	ON_BN_CLICKED(XTP_IDC_BUTTON_RECURRECNCE, OnBnClickedButtonRecurrecnce)
	ON_BN_CLICKED(XTP_IDC_CHECK_ALLDAY_EVENT, OnBnClickedCheckAlldayEvent)
	ON_NOTIFY(DTN_DATETIMECHANGE, XTP_IDC_DTP_START_DATE, OnStartDateChanged)
	ON_NOTIFY(DTN_DATETIMECHANGE, XTP_IDC_DTP_START_TIME, OnStartTimeChanged)
	ON_NOTIFY(DTN_DATETIMECHANGE, XTP_IDC_DTP_END_DATE, OnEndDateChanged)
	ON_NOTIFY(DTN_DATETIMECHANGE, XTP_IDC_DTP_END_TIME, OnEndTimeChanged)
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_SCHEDULEID, OnScheduleIdChanged)
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_EVENT_CATEGORY, OnSelChangeEventCategory)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventPropertiesDlg message handlers

void CXTPCalendarEventPropertiesDlg::OnStartDateChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	*pResult					 = 0;

	if (COleDateTime(pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay, 0, 0, 0)
		== COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(),
						0, 0, 0))
		return;

	UpdateData();

	if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(), 0,
					 0, 0)
		> COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(), m_dtEndDate.GetDay(), 0, 0,
					   0))
	{
		m_dtEndDate = m_dtStartDate;
		m_dtStartTime.SetDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(),
								  m_dtStartDate.GetDay(), m_dtStartTime.GetHour(),
								  m_dtStartTime.GetMinute(), m_dtStartTime.GetSecond());
		m_dtEndTime.SetDateTime(m_dtEndTime.GetYear(), m_dtEndTime.GetMonth(), m_dtEndTime.GetDay(),
								m_dtEndTime.GetHour(), m_dtEndTime.GetMinute(),
								m_dtEndTime.GetSecond());

		if (m_dtStartTime > m_dtEndTime)
			m_dtEndTime = m_dtStartTime;
	}
	else if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(),
						  0, 0, 0)
				 == COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(),
								 m_dtEndDate.GetDay(), 0, 0, 0)
			 && m_dtStartTime > m_dtEndTime)
		m_dtEndTime = m_dtStartTime;

	UpdateData(FALSE);
}

void CXTPCalendarEventPropertiesDlg::OnEndDateChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	*pResult					 = 0;

	if (COleDateTime(pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay, 0, 0, 0)
		== COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(), m_dtEndDate.GetDay(), 0, 0,
						0))
		return;

	UpdateData();

	if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(), 0,
					 0, 0)
		> COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(), m_dtEndDate.GetDay(), 0, 0,
					   0))
	{
		m_dtStartDate = m_dtEndDate;
		m_dtStartTime.SetDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(),
								  m_dtStartDate.GetDay(), m_dtStartTime.GetHour(),
								  m_dtStartTime.GetMinute(), m_dtStartTime.GetSecond());
		m_dtEndTime.SetDateTime(m_dtEndTime.GetYear(), m_dtEndTime.GetMonth(), m_dtEndTime.GetDay(),
								m_dtEndTime.GetHour(), m_dtEndTime.GetMinute(),
								m_dtEndTime.GetSecond());

		if (m_dtStartTime > m_dtEndTime)
			m_dtStartTime = m_dtEndTime;
	}
	else if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(),
						  0, 0, 0)
				 == COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(),
								 m_dtEndDate.GetDay(), 0, 0, 0)
			 && m_dtStartTime > m_dtEndTime)
		m_dtStartTime = m_dtEndTime;

	UpdateData(FALSE);
}

void CXTPCalendarEventPropertiesDlg::OnStartTimeChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	*pResult					 = 0;

	if (COleDateTime(pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay,
					 pDTChange->st.wHour, pDTChange->st.wMinute, pDTChange->st.wSecond)
		== COleDateTime(m_dtStartTime.GetYear(), m_dtStartTime.GetMonth(), m_dtStartTime.GetDay(),
						m_dtStartTime.GetHour(), m_dtStartTime.GetMinute(),
						m_dtStartTime.GetSecond()))
		return;

	UpdateData();

	if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(), 0,
					 0, 0)
		> COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(), m_dtEndDate.GetDay(), 0, 0,
					   0))
	{
		m_dtEndDate = m_dtStartDate;
		// m_dtStartTime.SetDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(),
		// m_dtStartDate.GetDay(),
		//  m_dtStartTime.GetHour(), m_dtStartTime.GetMinute(), m_dtStartTime.GetSecond());
		// m_dtEndTime.SetDateTime(m_dtEndTime.GetYear(), m_dtEndTime.GetMonth(),
		// m_dtEndTime.GetDay(),
		//  m_dtEndTime.GetHour(), m_dtEndTime.GetMinute(), m_dtEndTime.GetSecond());

		if (m_dtStartTime > m_dtEndTime)
			m_dtEndTime = m_dtStartTime;
	}
	else if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(),
						  0, 0, 0)
				 == COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(),
								 m_dtEndDate.GetDay(), 0, 0, 0)
			 && m_dtStartTime > m_dtEndTime)
		m_dtEndTime = m_dtStartTime;

	UpdateData(FALSE);
}

void CXTPCalendarEventPropertiesDlg::OnEndTimeChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	*pResult					 = 0;

	if (COleDateTime(pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay,
					 pDTChange->st.wHour, pDTChange->st.wMinute, pDTChange->st.wSecond)
		== COleDateTime(m_dtEndTime.GetYear(), m_dtEndTime.GetMonth(), m_dtEndTime.GetDay(),
						m_dtEndTime.GetHour(), m_dtEndTime.GetMinute(), m_dtEndTime.GetSecond()))
		return;

	UpdateData();

	if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(), 0,
					 0, 0)
		> COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(), m_dtEndDate.GetDay(), 0, 0,
					   0))
	{
		m_dtStartDate = m_dtEndDate;
		// m_dtStartTime.SetDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(),
		// m_dtStartDate.GetDay(),
		//  m_dtStartTime.GetHour(), m_dtStartTime.GetMinute(), m_dtStartTime.GetSecond());
		// m_dtEndTime.SetDateTime(m_dtEndTime.GetYear(), m_dtEndTime.GetMonth(),
		// m_dtEndTime.GetDay(),
		//  m_dtEndTime.GetHour(), m_dtEndTime.GetMinute(), m_dtEndTime.GetSecond());

		if (m_dtStartTime > m_dtEndTime)
			m_dtStartTime = m_dtEndTime;
	}
	else if (COleDateTime(m_dtStartDate.GetYear(), m_dtStartDate.GetMonth(), m_dtStartDate.GetDay(),
						  0, 0, 0)
				 == COleDateTime(m_dtEndDate.GetYear(), m_dtEndDate.GetMonth(),
								 m_dtEndDate.GetDay(), 0, 0, 0)
			 && m_dtStartTime > m_dtEndTime)
		m_dtStartTime = m_dtEndTime;

	UpdateData(FALSE);
}

void CXTPCalendarEventPropertiesDlg::OnOK()
{
	UpdateData();

	//////////////////////////////////////////////////////////////////////////

	BOOL bCustomValue	= FALSE;
	int nReminderMinutes = 0;

	if (m_ptrControl->m_mapCustomReminderValues.GetCount())
	{
		int iCurItem = m_wndReminderCombo.GetCurSel();

		if (iCurItem != CB_ERR)
		{
			int iSelectedMinutes = (int)m_wndReminderCombo.GetItemData(iCurItem);

			if (iSelectedMinutes) // custom value
			{
				nReminderMinutes = iSelectedMinutes;

				bCustomValue = TRUE;
			}
		}
	}

	if (!bCustomValue)
	{
		CString strReminder;
		m_wndReminderCombo.GetWindowText(strReminder);
		nReminderMinutes = CXTPCalendarUtils::ParceTimeDuration_min(strReminder);

		if (nReminderMinutes == INT_MIN)
		{
			m_wndReminderCombo.SetFocus();
			XTPResourceManager()->ShowMessageBox(XTP_IDS_CALENDAR_WRONG_VALUE);
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	BOOL bChanged = FALSE;

	if (m_ptrEditingEvent->GetRecurrenceState() == xtpCalendarRecurrenceOccurrence)
	{
		VERIFY(m_ptrEditingEvent->MakeAsRException());
	}

	if (m_strSubject.Compare(m_ptrEditingEvent->GetSubject()))
	{
		m_ptrEditingEvent->SetSubject(m_strSubject);
		bChanged = TRUE;
	}
	if (m_strLocation.Compare(m_ptrEditingEvent->GetLocation()))
	{
		m_ptrEditingEvent->SetLocation(m_strLocation);
		bChanged = TRUE;
	}
	if (m_strBody.Compare(m_ptrEditingEvent->GetBody()))
	{
		m_ptrEditingEvent->SetBody(m_strBody);
		bChanged = TRUE;
	}

	if (m_bOccurrence)
	{
		COleDateTime dtStartTime(CXTPCalendarUtils::UpdateTime(m_dtStartDate, m_dtStartTime));
		COleDateTime dtEndTime(CXTPCalendarUtils::UpdateTime(m_dtEndDate, m_dtEndTime));

		if (m_bAllDayEvent && CXTPCalendarUtils::IsZeroTime(m_dtEndDate))
		{
			AddEndDate_ResetTime(m_dtStartDate, dtEndTime, 1);
		}

		if (dtStartTime != m_ptrEditingEvent->GetStartTime())
		{
			m_ptrEditingEvent->SetStartTime(dtStartTime);
			bChanged = TRUE;
		}
		if (dtEndTime != m_ptrEditingEvent->GetEndTime())
		{
			m_ptrEditingEvent->SetEndTime(dtEndTime);
			bChanged = TRUE;
		}

		if (m_bAllDayEvent != m_ptrEditingEvent->IsAllDayEvent())
		{
			m_ptrEditingEvent->SetAllDayEvent(m_bAllDayEvent);
			bChanged = TRUE;
		}
	}
	else
	{
		// m_ptrEditingEvent->SetAllDayEvent(FALSE);
		m_ptrEditingEvent->SetAllDayEvent(m_bAllDayEvent);
	}

	if (m_nBusyStatus != m_ptrEditingEvent->GetBusyStatus())
	{
		m_ptrEditingEvent->SetBusyStatus(m_nBusyStatus);
		bChanged = TRUE;
	}

	m_wndCategoryCombo.GetChecked(m_ptrEditingEvent->GetCategories(), &bChanged);

	if (m_wndLabelCombo.GetLabelID() != m_ptrEditingEvent->GetLabelID())
	{
		m_ptrEditingEvent->SetLabelID(m_wndLabelCombo.GetLabelID());
		bChanged = TRUE;
	}

	if (m_bPrivate != m_ptrEditingEvent->IsPrivate())
	{
		m_ptrEditingEvent->SetPrivate(m_bPrivate);
		bChanged = TRUE;
	}

	if (m_bMeeting != m_ptrEditingEvent->IsMeeting())
	{
		m_ptrEditingEvent->SetMeeting(m_bMeeting);
		bChanged = TRUE;
	}

	if (m_bReminder != m_ptrEditingEvent->IsReminder())
	{
		m_ptrEditingEvent->SetReminder(m_bReminder);
		bChanged = TRUE;
	}

	if (m_bReminder)
	{
		if (nReminderMinutes != m_ptrEditingEvent->GetReminderMinutesBeforeStart())
		{
			m_ptrEditingEvent->SetReminderMinutesBeforeStart(nReminderMinutes);
			bChanged = TRUE;
		}
	}

	if (m_ptrControl != NULL && m_ptrControl->m_bMultipleSchedulesMode)
	{
		if (m_ctrlScheduleSet.m_hWnd)
		{
			CString sSet, sGet, sOne;
			int nCnt = m_ctrlScheduleIDCB.GetCount();
			for (int nT = 0; nT < nCnt; nT++)
			{
				if (m_ctrlScheduleSet.GetCheck(nT) > 0)
				{
					sOne.Format(_T("|%d|"), (int)m_ctrlScheduleIDCB.GetItemData(nT));
					sSet += sOne;
				}
			}
			sSet.Replace(_T("||"), _T("|"));
			sGet = m_ptrEditingEvent->GetScheduleSet();

			if (sGet != sSet)
			{
				m_ptrEditingEvent->SetScheduleSet(sSet);
				bChanged = TRUE;
			}
		}
	}
	CXTPCalendarData* pData = m_ptrEditingEvent->GetDataProvider();
	if (pData)
	{
		int nSelSch = m_ctrlScheduleIDCB.GetCurSel();
		if (nSelSch != CB_ERR)
		{
			UINT uSchID = (UINT)m_ctrlScheduleIDCB.GetItemData(nSelSch);
			if (uSchID != m_ptrEditingEvent->GetScheduleID())
			{
				m_ptrEditingEvent->SetScheduleID(uSchID);
				bChanged = TRUE;
			}
		}

		if (m_ptrEditingEvent->GetSubject().IsEmpty() && m_ptrEditingEvent->GetLocation().IsEmpty()
			&& m_ptrEditingEvent->GetBody().IsEmpty())
		{
			EndDialog(0);
		}
		else if (!bChanged && !m_bRecurrenceUpdate)
		{
			EndDialog(0);
		}
		else
		{
			// Update event in data storage
			CXTPCalendarControl::CUpdateContext updateContext(m_ptrControl,
															  xtpCalendarUpdateLayout
																  | xtpCalendarUpdateRedraw);

			if (m_bAddEvent)
				VERIFY(pData->AddEvent(m_ptrEditingEvent));
			else
				VERIFY(pData->ChangeEvent(m_ptrEditingEvent));

			m_ptrControl->Populate();
		}
	}

	m_dNewEventID = m_ptrEditingEvent->GetEventID();

	CDialog::OnOK();

	// AfxGetMainWnd()->EnableWindow(TRUE);
}

void CXTPCalendarEventPropertiesDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CXTPCalendarEventPropertiesDlg::OnScheduleIdChanged()
{
	if (m_ptrControl != NULL && m_ptrControl->m_bMultipleSchedulesMode
		&& !m_ptrControl->m_bMultipleSchedulesNoSelfLinks)
	{
		int nSelSch = m_ctrlScheduleIDCB.GetCurSel();
		if (nSelSch != CB_ERR)
		{
			if (m_ctrlScheduleSet.m_hWnd && m_ctrlScheduleSet.GetCount() > nSelSch)
				m_ctrlScheduleSet.SetCheck(nSelSch, TRUE);
		}
	}
}

void CXTPCalendarEventPropertiesDlg::OnSelChangeEventCategory()
{
	m_wndLabelCombo.EnableWindow(m_wndCategoryCombo.GetCategoryArray().GetSize() == 0);
}

void CXTPCalendarEventPropertiesDlg::UpdateControlsFromEvent()
{
	ASSERT(!!m_ptrEditingEvent);

	m_strSubject  = m_ptrEditingEvent->GetSubject();
	m_strLocation = m_ptrEditingEvent->GetLocation();
	m_strBody	 = m_ptrEditingEvent->GetBody();

	if (m_ptrEditingEvent->GetDataProvider())
		m_wndCategoryCombo.Init(m_ptrEditingEvent, m_nTheme);

	if (m_ptrEditingEvent->GetDataProvider())
		m_wndLabelCombo.Init(m_ptrEditingEvent, m_nTheme);

	m_wndLabelCombo.SetLabelID(m_ptrEditingEvent->GetLabelID());

	if (m_nAllDayEventSet < 0)
	{
		m_bAllDayEvent = m_ptrEditingEvent->IsAllDayEvent();
	}
	else
	{
		m_bAllDayEvent = m_nAllDayEventSet != 0;

		if (m_bAllDayEvent && CXTPCalendarUtils::IsZeroTime(m_dtEndDate))
			AddEndDate_ResetTime(m_dtStartDate, m_dtEndDate, -1);
	}

	m_bPrivate	= m_ptrEditingEvent->IsPrivate();
	m_bMeeting	= m_ptrEditingEvent->IsMeeting();
	m_nBusyStatus = m_ptrEditingEvent->GetBusyStatus();

	m_bReminder = m_ptrEditingEvent->IsReminder();

	int nMinutes		= m_ptrEditingEvent->GetReminderMinutesBeforeStart();
	CString strReminder = CXTPCalendarUtils::FormatTimeDuration(nMinutes, FALSE);

	m_wndReminderCombo.SetWindowText(strReminder);

	CXTPCalendarData* pData = m_ptrEditingEvent->GetDataProvider();

	CXTPCalendarSchedules* pSchedules = pData->GetSchedules();
	ASSERT(pSchedules);

	BOOL bSchEnabled = pSchedules && pSchedules->GetCount();

	m_ctrlScheduleIDCB.EnableWindow(bSchEnabled);
	m_ctrlScheduleLabel.EnableWindow(bSchEnabled);

	BOOL bSuccess(FALSE);
	if (pSchedules != NULL && bSchEnabled)
	{
		m_ctrlScheduleIDCB.ResetContent();
		int nIdx(-1);

		if (pData->m_bOwnershipMode)
		{
			nIdx = m_ctrlScheduleIDCB.AddString(_T("-"));
			m_ctrlScheduleIDCB.SetItemData(nIdx, (DWORD_PTR)XTP_CALENDAR_UNKNOWN_SCHEDULE_ID);
		}
		CString sName;
		int nCount = pSchedules->GetCount();
		int nOwnerIndex(0);
		DWORD_PTR sChId(0);

		for (int nSchNr = 0; nSchNr < nCount; nSchNr++)
		{
			CXTPCalendarSchedule* pSch = pSchedules->GetAt(nSchNr);
			ASSERT(pSch);
			if (pSch)
			{
				sName = pSch->GetName();
				if (pData->m_bOwnershipMode && !m_ptrControl->m_OwnerUser.IsEmpty())
				{
					if (m_ptrControl->m_OwnerUser == sName)
					{
						sChId = (DWORD_PTR)pSch->GetID();
						nIdx  = m_ctrlScheduleIDCB.AddString(sName);
						m_ctrlScheduleIDCB.SetItemData(nIdx, sChId);
						nOwnerIndex = nIdx;
					}
					else if (sName == _T("Public"))
					{
						sChId = (DWORD_PTR)pSch->GetID();
						nIdx  = m_ctrlScheduleIDCB.AddString(sName);
						m_ctrlScheduleIDCB.SetItemData(nIdx, sChId);
					}
#ifdef _BUSINESS_MODEL_
					else if (pSch->GetSecType() == 0)
					{
						sChId = (DWORD_PTR)pSch->GetID();
						nIdx  = m_ctrlScheduleIDCB.AddString(sName);
						m_ctrlScheduleIDCB.SetItemData(nIdx, sChId);
					}
#endif

					m_ctrlScheduleIDCB.EnableWindow(FALSE);
					m_ctrlScheduleLabel.EnableWindow(FALSE);
				}
				else
				{
					sChId = (DWORD_PTR)pSch->GetID();
					BOOL bNotFound(TRUE);
					for (int iPrev = 0; iPrev < nSchNr; iPrev++)
					{
						if (sChId == m_ctrlScheduleIDCB.GetItemData(iPrev))
						{
							bNotFound = FALSE;
							break;
						}
					}
					if (bNotFound) // to prevent duplicates
					{
						nIdx = m_ctrlScheduleIDCB.AddString(sName);
						m_ctrlScheduleIDCB.SetItemData(nIdx, sChId);
						bSuccess = TRUE;
					}
				}
			}
		}

		UINT uSchID = m_ptrEditingEvent->GetScheduleID();
		nCount		= m_ctrlScheduleIDCB.GetCount();

		if (m_bAddEvent && nCount)
		{
			m_ctrlScheduleIDCB.SetCurSel(nOwnerIndex);
			bSuccess = TRUE;
		}

		for (int nSchIDCS = 0; nSchIDCS < nCount; nSchIDCS++)
		{
			if (m_ctrlScheduleIDCB.GetItemData(nSchIDCS) == uSchID)
			{
				m_ctrlScheduleIDCB.SetCurSel(nSchIDCS);
				bSuccess = TRUE;
				break;
			}
		}
	}

	UpdateData(FALSE);

	if (pData->m_bOwnershipMode && !bSuccess)
		EndDialog(0);
}

void CXTPCalendarEventPropertiesDlg::UpdateControlsState()
{
	if (GetDlgItem(XTP_IDC_DTP_START_DATE))
		GetDlgItem(XTP_IDC_DTP_START_DATE)->ShowWindow(m_bOccurrence ? SW_SHOW : SW_HIDE);
	if (GetDlgItem(XTP_IDC_DTP_START_TIME))
		GetDlgItem(XTP_IDC_DTP_START_TIME)
			->ShowWindow(m_bOccurrence && !m_bAllDayEvent ? SW_SHOW : SW_HIDE);
	if (GetDlgItem(XTP_IDC_DTP_END_DATE))
		GetDlgItem(XTP_IDC_DTP_END_DATE)->ShowWindow(m_bOccurrence ? SW_SHOW : SW_HIDE);
	if (GetDlgItem(XTP_IDC_DTP_END_TIME))
		GetDlgItem(XTP_IDC_DTP_END_TIME)
			->ShowWindow(m_bOccurrence && !m_bAllDayEvent ? SW_SHOW : SW_HIDE);
	if (GetDlgItem(XTP_IDC_START_TEXT))
		GetDlgItem(XTP_IDC_START_TEXT)->ShowWindow(m_bOccurrence ? SW_SHOW : SW_HIDE);
	if (GetDlgItem(XTP_IDC_END_TEXT))
		GetDlgItem(XTP_IDC_END_TEXT)->ShowWindow(m_bOccurrence ? SW_SHOW : SW_HIDE);

	m_wndReminderCombo.EnableWindow(m_bReminder);

	m_wndLabelCombo.EnableWindow(m_wndCategoryCombo.GetCategoryArray().GetSize() == 0);
}

void CXTPCalendarEventPropertiesDlg::InitReminderComboBox()
{
	m_wndReminderCombo.ResetContent();

	if (!m_ptrControl->m_bIgnoreDefaultReminderValues)
	{
		m_wndReminderCombo.SetItemData(m_wndReminderCombo.AddString(
										   LoadString(XTP_IDS_CALENDAR_MINUTES_0)),
									   NULL);
		m_wndReminderCombo.SetItemData(m_wndReminderCombo.AddString(
										   LoadString(XTP_IDS_CALENDAR_MINUTES_1)),
									   NULL);
		m_wndReminderCombo.SetItemData(m_wndReminderCombo.AddString(
										   LoadString(XTP_IDS_CALENDAR_MINUTES_5)),
									   NULL);
		m_wndReminderCombo.SetItemData(m_wndReminderCombo.AddString(
										   LoadString(XTP_IDS_CALENDAR_MINUTES_10)),
									   NULL);
		m_wndReminderCombo.SetItemData(m_wndReminderCombo.AddString(
										   LoadString(XTP_IDS_CALENDAR_MINUTES_15)),
									   NULL);
	}

	// add all custom intervals
	POSITION pos = m_ptrControl->m_mapCustomReminderValues.GetStartPosition();
	int nMinutes;
	CString sName;
	while (pos != NULL)
	{
		m_ptrControl->m_mapCustomReminderValues.GetNextAssoc(pos, nMinutes, sName);

		m_wndReminderCombo.SetItemData(m_wndReminderCombo.AddString(sName), XTPToUIntPtr(nMinutes));
	}
}

BOOL CXTPCalendarEventPropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bRecurrenceUpdate = FALSE;

	InitReminderComboBox();

	m_wndBusyStatusCombo.Init(m_ptrEditingEvent, m_nTheme);

	UpdateControlsFromEvent();
	UpdateControlsState();

	if (m_ptrControl != NULL && m_ptrControl->m_bMultipleSchedulesMode)
	{
		CRect rcLab, rcCmb, rcRmd, rcBody;
		GetDlgItem(XTP_IDC_SCHEDULEID_TEXT)->GetWindowRect(&rcLab);

		CString s_SCHEDULEID_TEXT;
		GetDlgItem(XTP_IDC_SCHEDULEID_TEXT)->GetWindowText(s_SCHEDULEID_TEXT);
		s_SCHEDULEID_TEXT.Replace(_T(" ID"), _T(""));
		GetDlgItem(XTP_IDC_SCHEDULEID_TEXT)->SetWindowText(s_SCHEDULEID_TEXT);

		ScreenToClient(&rcLab);
		rcLab.right -= 40;
		rcLab.left -= 30;

		m_wndReminderCombo.GetWindowRect(&rcRmd);
		ScreenToClient(&rcRmd);

		m_ctrlScheduleIDCB.GetWindowRect(&rcCmb);
		ScreenToClient(&rcCmb);

		GetDlgItem(XTP_IDC_EDIT_BODY)->GetWindowRect(&rcBody);
		ScreenToClient(&rcBody);
		rcBody.right = rcCmb.right;
		GetDlgItem(XTP_IDC_EDIT_BODY)->MoveWindow(&rcBody);

		rcCmb.top	= rcBody.top;
		rcCmb.bottom = rcBody.bottom;

		GetDlgItem(XTP_IDC_CHECK_PRIVATE)->GetWindowRect(&rcBody);
		ScreenToClient(&rcBody);
		rcCmb.left = rcBody.left;

		GetDlgItem(XTP_IDC_BUTTON_RECURRECNCE)->GetWindowRect(&rcBody);
		ScreenToClient(&rcBody);

		rcCmb.right = rcBody.right;

		int nCnt = m_ctrlScheduleIDCB.GetCount();
		if (nCnt > 1)
		{
			m_ctrlScheduleSet.Create(LBS_EXTENDEDSEL | LBS_OWNERDRAWFIXED | LBS_STANDARD
										 | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL
										 | WS_TABSTOP | WS_VISIBLE,
									 rcCmb, this, XTP_IDC_LABEL + 1); // 118

			m_ctrlScheduleSet.SetFont(m_ctrlScheduleIDCB.GetFont());

			CString sSet = m_ptrEditingEvent->GetScheduleSet();
			CString rString, sOne, sAll;
			nCnt	 = m_ctrlScheduleIDCB.GetCount();
			int iSel = m_ctrlScheduleIDCB.GetCurSel();
			for (int nT = 0; nT < nCnt; nT++)
			{
				m_ctrlScheduleIDCB.GetLBText(nT, rString);
				sOne.Format(_T("|%d|"), (int)m_ctrlScheduleIDCB.GetItemData(nT));
				if (sAll.IsEmpty() || sAll.Find(sOne) == -1) // to prevent duplicates
				{
					sAll += sOne;
					m_ctrlScheduleSet.InsertString(nT, rString);
					if (!sSet.IsEmpty() && sSet.Find(sOne) > -1)
						m_ctrlScheduleSet.SetCheck(nT, TRUE);

					if (iSel == nT && !m_ptrControl->m_bMultipleSchedulesNoSelfLinks)
						m_ctrlScheduleSet.SetCheck(nT, TRUE);
				}
			}
		}
	}

	return TRUE;
}

void CXTPCalendarEventPropertiesDlg::OnBnClickedButtonRecurrecnce()
{
	UpdateData();

	BOOL bWasNotRecur	 = FALSE;
	BOOL bUpdateFromEvent = FALSE;
	m_bRecurrenceUpdate   = TRUE;

	CXTPCalendarEventPtr ptrMasterEvent = m_ptrEditingEvent;
	int nRState							= ptrMasterEvent->GetRecurrenceState();

	if (nRState != xtpCalendarRecurrenceMaster)
	{
		ptrMasterEvent = m_ptrEditingEvent->CloneEvent();

		if (nRState == xtpCalendarRecurrenceNotRecurring)
		{
			VERIFY(ptrMasterEvent->MakeEventAsRecurrence());
			bWasNotRecur = TRUE;

			CXTPCalendarRecurrencePatternPtr ptrPattern;
			ptrPattern = ptrMasterEvent->GetRecurrencePattern();

			COleDateTime dtStartDateTime(
				CXTPCalendarUtils::UpdateTime(m_dtStartDate, m_dtStartTime));
			COleDateTime dtEndDateTime(CXTPCalendarUtils::UpdateTime(m_dtEndDate, m_dtEndTime));

			COleDateTimeSpan spDuration = dtEndDateTime - dtStartDateTime;

			ptrPattern->SetStartTime(CXTPCalendarUtils::ResetDate(m_dtStartTime));
			ptrPattern->SetDurationMinutes((int)spDuration.GetTotalMinutes());

			ptrPattern->SetPatternStartDate(CXTPCalendarUtils::ResetTime(m_dtStartDate));

			ptrMasterEvent->UpdateRecurrence(ptrPattern);
		}
		else
		{
			bUpdateFromEvent = TRUE;

			ASSERT(nRState == xtpCalendarRecurrenceOccurrence
				   || nRState == xtpCalendarRecurrenceException);

			CXTPCalendarRecurrencePatternPtr ptrPattern;
			ptrPattern	 = m_ptrEditingEvent->GetRecurrencePattern();
			ptrMasterEvent = ptrPattern->GetMasterEvent();
		}
	}

	CXTPCalendarEventRecurrenceDlg dlg(ptrMasterEvent, this);
	dlg.m_bDisableRemove = bWasNotRecur;
	dlg.nWorkWeekMask	= m_ptrControl->GetWorkWeekMask();

	int nRes = (int)dlg.DoModal();
	if (nRes == IDOK || nRes == XTP_CALENDAR_DLGRESULT_REMOVE_RECURRENCE)
	{
		m_ptrEditingEvent = ptrMasterEvent;

		m_bOccurrence = m_ptrEditingEvent->GetRecurrenceState()
						== xtpCalendarRecurrenceNotRecurring;

		if (bUpdateFromEvent)
		{
			UpdateData();
			UpdateControlsFromEvent();
		}

		UpdateControlsState();
	}
}

int CXTPCalendarEventPropertiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	int nType = m_ptrEditingEvent->GetRecurrenceState();

	if (nType == xtpCalendarRecurrenceOccurrence || nType == xtpCalendarRecurrenceException)
	{
		// CXTPCalendarOccurSeriesChooseDlg dlg(0, XTP_IDS_CALENDAR_OCURR_SERIES_OPEN);
		CXTPCalendarOccurSeriesChooseDlg dlg(this, XTP_IDS_CALENDAR_OCURR_SERIES_OPEN);
		dlg.SetEvent(m_ptrEditingEvent);
		dlg.m_bOccur = m_bOccurrence;
		// this assignment select default picklist item in dlg!
		if (dlg.DoModal() != IDOK)
			return -1;

		m_bOccurrence = dlg.m_bOccur;
		if (!m_bOccurrence)
		{
			CXTPCalendarRecurrencePatternPtr ptrPatternRO;
			ptrPatternRO					 = m_ptrEditingEvent->GetRecurrencePattern();
			CXTPCalendarEventPtr ptrMasterRO = ptrPatternRO->GetMasterEvent();

			m_ptrEditingEvent = ptrMasterRO->CloneEvent();
		}
	}
	else
		m_bOccurrence = TRUE;

	return 0;
}

void CXTPCalendarEventPropertiesDlg::OnBnClickedCheckAlldayEvent()
{
	UpdateData();
	UpdateControlsState();

	if (m_bAllDayEvent && CXTPCalendarUtils::IsZeroTime(m_dtEndTime))
	{
		AddEndDate_ResetTime(m_dtStartDate, m_dtEndDate, -1);
		UpdateData(FALSE);
	}
	else if (!m_bAllDayEvent && CXTPCalendarUtils::IsZeroTime(m_dtEndTime))
	{
		AddEndDate_ResetTime(m_dtStartDate, m_dtEndDate, 1);
		UpdateData(FALSE);
	}
}

void CXTPCalendarEventPropertiesDlg::OnCheckReminder()
{
	UpdateData();
	UpdateControlsState();
}

void CXTPCalendarEventPropertiesDlg::AddEndDate_ResetTime(COleDateTime dtStartDate,
														  COleDateTime& rdtEndDate, int nDays)
{
	COleDateTimeSpan spDiff = CXTPCalendarUtils::ResetTime(rdtEndDate)
							  - CXTPCalendarUtils::ResetTime(dtStartDate);
	if (nDays > 0 || nDays < 0 && (DWORD)(double)spDiff >= (DWORD)(-1 * nDays))
	{
		rdtEndDate = CXTPCalendarUtils::ResetTime(rdtEndDate) + COleDateTimeSpan(nDays, 0, 0, 0);
	}
}

DWORD CXTPCalendarEventPropertiesDlg::GetLastEventID()
{
	return m_dNewEventID;
}

CString CXTPCalendarEventPropertiesDlg::LoadString(UINT nIDResource) const
{
	return CXTPCalendarUtils::LoadString(nIDResource);
}
