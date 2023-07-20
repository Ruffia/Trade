// XTPPropertyGridInplaceButton.cpp : implementation of the CXTPPropertyGridInplaceButton class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridInplaceButton.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridView.h"
#include "PropertyGrid/XTPPropertyGridToolBar.h"
#include "PropertyGrid/XTPPropertyGrid.h"
#include "PropertyGrid/XTPPropertyGridIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceButton

CXTPPropertyGridInplaceButton::CXTPPropertyGridInplaceButton(UINT nID)
{
	m_pItem = NULL;
	m_nID   = nID;
	m_pGrid = NULL;
	m_rcButton.SetRectEmpty();
	m_nIndex	  = -1;
	m_bPressed	= FALSE;
	m_nWidth	  = GetSystemMetrics(SM_CXHTHUMB);
	m_nIconIndex  = -1;
	m_bEnabled	= TRUE;
	m_bHyperlink  = FALSE;
	m_bShowAlways = FALSE;
	m_nAlignment  = DT_RIGHT;
	m_bTabStop	= TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPPropertyGridInplaceButton::~CXTPPropertyGridInplaceButton()
{
}

CXTPImageManagerIcon* CXTPPropertyGridInplaceButton::GetImage() const
{
	ASSERT(m_pItem);

	if (!m_pItem)
		return NULL;

	if (m_nIconIndex == -1)
		return NULL;

	return m_pGrid->GetImageManager()->GetImage(XTPToUInt(m_nIconIndex), 0);
}

void CXTPPropertyGridInplaceButton::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	CWindowDC dc(NULL);
	CXTPFontDC font(&dc, m_pGrid->GetPaintManager()->GetItemFont(m_pItem, TRUE));
	m_nWidth = dc.GetTextExtent(m_strCaption).cx + XTP_DPI_X(6);
}

int CXTPPropertyGridInplaceButton::GetIndex() const
{
	return m_nIndex;
}

BOOL CXTPPropertyGridInplaceButton::IsFocused() const
{
	return m_pGrid->m_pFocusedButton == this;
}

BOOL CXTPPropertyGridInplaceButton::IsHot() const
{
	return m_pGrid->m_pHotButton == this && m_bEnabled;
}

BOOL CXTPPropertyGridInplaceButton::IsPressed() const
{
	return m_bPressed;
}

BOOL CXTPPropertyGridInplaceButton::GetEnabled() const
{
	return m_bEnabled;
}

void CXTPPropertyGridInplaceButton::SetEnabled(BOOL bEnabled)
{
	m_bEnabled = bEnabled;
	SAFE_INVALIDATE(m_pGrid);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceButton message handlers

void CXTPPropertyGridInplaceButton::OnDraw(CDC* pDC, CRect rc)
{
	m_rcButton = rc;

	m_pGrid->GetPaintManager()->FillInplaceButton(pDC, this);
}

void CXTPPropertyGridInplaceButton::OnLButtonDown(UINT, CPoint)
{
	if (!m_bEnabled)
		return;

	m_bPressed = TRUE;
	m_pGrid->SetFocus();

	m_pGrid->Invalidate(FALSE);

	m_pGrid->SetCapture();
	BOOL bClick = TRUE;

	while (::GetCapture() == m_pGrid->GetSafeHwnd())
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (::GetCapture() != m_pGrid->GetSafeHwnd())
		{
			DispatchMessage(&msg);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
		{
			bClick = IsHot();
			break;
		}

		if (msg.message == WM_MOUSEMOVE)
		{
			CPoint pt = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));

			CXTPPropertyGridInplaceButton* pHotButton = m_rcButton.PtInRect(pt) ? this : NULL;

			if (m_pGrid->m_pHotButton != pHotButton)
			{
				m_pGrid->m_pHotButton = pHotButton;
				m_pGrid->Invalidate(FALSE);
			}
			continue;
		}

		DispatchMessage(&msg);
	}

	m_bPressed = FALSE;

	ReleaseCapture();
	m_pGrid->Invalidate(FALSE);

	if (bClick)
	{
		m_pItem->OnInplaceButtonDown(this);
	}
}

int CXTPPropertyGridInplaceButton::GetWidth()
{
	BOOL bItemText = FALSE;
	if (m_pGrid->GetPropertyGrid()->IsHighlightChangedButtonItems()
		&& m_pGrid->GetPropertyGrid()->IsHighlightChangedItems() && GetItem()->IsValueChanged()
		&& !GetCaption().IsEmpty() && GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON && !GetImage())
		bItemText = TRUE;

	if ((m_nWidth <= 0 && !m_strCaption.IsEmpty()) || bItemText)
	{
		CWindowDC dc(NULL);
		CXTPFontDC font(&dc, m_pGrid->GetPaintManager()->GetItemFont(m_pItem, TRUE));
		return dc.GetTextExtent(m_strCaption).cx + XTP_DPI_X(6);
	}

	return m_nWidth;
}

void CXTPPropertyGridInplaceButton::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

void CXTPPropertyGridInplaceButton::OnKeyDown(UINT nChar)
{
	if (m_pItem && m_bEnabled
		&& (nChar == VK_SPACE || nChar == VK_DOWN || nChar == VK_RETURN || nChar == VK_F4))
	{
		m_pItem->OnInplaceButtonDown(this);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceButtons

CXTPPropertyGridInplaceButtons::CXTPPropertyGridInplaceButtons(CXTPPropertyGridItem* pItem)
{
	m_pItem = pItem;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPPropertyGridInplaceButtons::~CXTPPropertyGridInplaceButtons()
{
	RemoveAll();
}

void CXTPPropertyGridInplaceButtons::OnButtonRemoved(CXTPPropertyGridInplaceButton* pButton)
{
	CXTPPropertyGridView* pGrid = m_pItem ? m_pItem->GetGrid() : NULL;
	if (!pGrid)
		return;

	if (pGrid->m_pFocusedButton == pButton)
	{
		pGrid->m_pFocusedButton = NULL;
	}

	if (pGrid->m_pHotButton == pButton)
	{
		pGrid->m_pHotButton = NULL;
	}
}

void CXTPPropertyGridInplaceButtons::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPPropertyGridInplaceButton* pButton = m_arrButtons[i];

		OnButtonRemoved(pButton);
		pButton->InternalRelease();
	}
	m_arrButtons.RemoveAll();
}

void CXTPPropertyGridInplaceButtons::UpdateIndexes()
{
	for (int i = 0; i < GetCount(); i++)
	{
		m_arrButtons[i]->m_nIndex = i;
	}
}

void CXTPPropertyGridInplaceButtons::Remove(CXTPPropertyGridInplaceButton* pButton)
{
	if (!pButton)
		return;

	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrButtons[i] == pButton)
		{
			RemoveAt(i);

			OnButtonRemoved(pButton);
			return;
		}
	}
}

void CXTPPropertyGridInplaceButtons::Remove(UINT nID)
{
	Remove(Find(nID));
}

void CXTPPropertyGridInplaceButtons::RemoveAt(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPPropertyGridInplaceButton* pButton = m_arrButtons[nIndex];
		m_arrButtons.RemoveAt(nIndex);

		OnButtonRemoved(pButton);
		pButton->InternalRelease();

		UpdateIndexes();
	}
}

CXTPPropertyGridInplaceButton*
	CXTPPropertyGridInplaceButtons::AddButton(CXTPPropertyGridInplaceButton* pButton)
{
	pButton->m_pItem = m_pItem;
	pButton->m_pGrid = m_pItem->GetGrid();
	m_arrButtons.Add(pButton);
	UpdateIndexes();

	return pButton;
}

void CXTPPropertyGridInplaceButtons::AddComboButton()
{
	if (Find(XTP_ID_PROPERTYGRID_COMBOBUTTON) == NULL)
	{
		AddButton(new CXTPPropertyGridInplaceButton(XTP_ID_PROPERTYGRID_COMBOBUTTON));
	}
}

void CXTPPropertyGridInplaceButtons::AddExpandButton()
{
	if (Find(XTP_ID_PROPERTYGRID_EXPANDBUTTON) == NULL)
	{
		AddButton(new CXTPPropertyGridInplaceButton(XTP_ID_PROPERTYGRID_EXPANDBUTTON));
	}
}

CXTPPropertyGridInplaceButton* CXTPPropertyGridInplaceButtons::Find(UINT nID) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrButtons[i]->GetID() == nID)
			return m_arrButtons[i];
	}
	return NULL;
}

CXTPPropertyGridInplaceButton* CXTPPropertyGridInplaceButtons::HitTest(CPoint point) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPPropertyGridInplaceButton* pButton = m_arrButtons[i];

		if (pButton->GetRect().PtInRect(point) && m_pItem->IsInplaceButtonVisible(pButton))
			return pButton;
	}
	return NULL;
}

#ifdef _XTP_ACTIVEX
///////////////////////////////////////////////////////////////////////////////////
//  Acteve X Methods
//

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceButton

BEGIN_DISPATCH_MAP(CXTPPropertyGridInplaceButton, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridInplaceButton, "Caption", 1, OleGetCaption, SetCaption,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridInplaceButton, "Index", 3, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "ID", DISPID_VALUE, m_nID, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridInplaceButton, "Item", 2, OleGetItem, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "IconIndex", 5, m_nIconIndex, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "Width", 4, m_nWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridInplaceButton, "Enabled", 6, GetEnabled, SetEnabled,
						VT_BOOL)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "Tooltip", 20, m_strTooltip, VT_BSTR)

	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "Hyperlink", 21, m_bHyperlink, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "ShowAlways", 22, m_bShowAlways, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "Alignment", 23, m_nAlignment, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceButton, "TabStop", 24, m_bTabStop, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridInplaceButton, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridInplaceButton, XTPDIID_IPropertyGridInplaceButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridInplaceButton, XTPDIID_IPropertyGridInplaceButton)

LPDISPATCH CXTPPropertyGridInplaceButton::OleGetItem()
{
	return XTPGetDispatch(m_pItem);
}
BSTR CXTPPropertyGridInplaceButton::OleGetCaption()
{
	return GetCaption().AllocSysString();
}

//////////////////////////////////////////////////////////////////////////
//

BEGIN_DISPATCH_MAP(CXTPPropertyGridInplaceButtons, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "Add", 2, OleAdd, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "Clear", 3, OleClear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "Remove", 4, OleRemove, VT_EMPTY, VTS_I4)

	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "Item", DISPID_VALUE, OleItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "_NewEnum", DISPID_NEWENUM, OleNewEnum,
					 VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridInplaceButtons, "FindButton", 5, OleFindButton, VT_DISPATCH,
					 VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridInplaceButtons, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridInplaceButtons, XTPDIID_IPropertyGridInplaceButtons, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridInplaceButtons, XTPDIID_IPropertyGridInplaceButtons)
IMPLEMENT_ENUM_VARIANT(CXTPPropertyGridInplaceButtons)

long CXTPPropertyGridInplaceButtons::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPPropertyGridInplaceButtons::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return XTPGetDispatch(m_arrButtons[nIndex]);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPPropertyGridInplaceButtons::OleItem(long nIndex)
{
	return OleGetItem(nIndex - 1);
}

void CXTPPropertyGridInplaceButtons::OleClear()
{
	RemoveAll();
}

LPDISPATCH CXTPPropertyGridInplaceButtons::OleAdd(UINT Id)
{
	return XTPGetDispatch(AddButton(new CXTPPropertyGridInplaceButton(Id)));
}

LPDISPATCH CXTPPropertyGridInplaceButtons::OleFindButton(long nId)
{
	return XTPGetDispatch(Find(nId));
}

void CXTPPropertyGridInplaceButtons::OleRemove(long nIndex)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		RemoveAt(nIndex - 1);
	}
	else
	{
		AfxThrowOleException(E_INVALIDARG);
	}
}

#endif
