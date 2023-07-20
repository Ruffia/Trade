// XTPTaskPanelItems.cpp : implementation of the CXTPTaskPanelItems class.
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPGdiObjects.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarContainer.h"

#include "TaskPanel/XTPTaskPanelDefines.h"
#include "TaskPanel/XTPTaskPanelItems.h"
#include "TaskPanel/XTPTaskPanelItem.h"
#include "TaskPanel/XTPTaskPanelGroup.h"
#include "TaskPanel/XTPTaskPanelGroupItem.h"
#include "TaskPanel/XTPTaskPanel.h"
#include "TaskPanel/XTPTaskPanelIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelItems

IMPLEMENT_DYNCREATE(CXTPTaskPanelItems, CXTPCmdTarget)

CXTPTaskPanelItems::CXTPTaskPanelItems(CXTPTaskPanel* pPanel)
{
	m_pPanel = pPanel;
	m_pOwner = NULL;

	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif
}

CXTPTaskPanelItems::~CXTPTaskPanelItems()
{
	Clear(FALSE);
}

CXTPTaskPanelItem* CXTPTaskPanelItems::Add(CXTPTaskPanelItem* pItem, UINT nID)
{
	pItem->SetID(nID);

	m_arrItems.Add(pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;
	pItem->OnInserted();

	m_pPanel->Reposition();

	return pItem;
}

CXTPTaskPanelItem* CXTPTaskPanelItems::Add(CXTPTaskPanelItem* pItem, UINT nID, LPCTSTR lpCaption,
										   LPCTSTR lpTooltip /*= NULL*/)
{
	pItem->SetID(nID, lpCaption, lpTooltip);

	m_arrItems.Add(pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;
	pItem->OnInserted();

	m_pPanel->Reposition();

	return pItem;
}

CXTPTaskPanelItem* CXTPTaskPanelItems::InsertAt(int nIndex, CXTPTaskPanelItem* pItem)
{
	if (nIndex < 0 || nIndex > GetCount())
		nIndex = GetCount();

	m_arrItems.InsertAt(nIndex, pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;
	pItem->OnInserted();

	m_pPanel->Reposition();

	return pItem;
}

CXTPTaskPanelItem* CXTPTaskPanelItems::Find(UINT nID) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->GetID() == nID)
			return m_arrItems[i];
	}
	return NULL;
}

int CXTPTaskPanelItems::GetIndex(const CXTPTaskPanelItem* pItem) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i] == pItem)
			return i;
	}
	return -1;
}

void CXTPTaskPanelItems::Move(CXTPTaskPanelItem* pItem, int nIndex)
{
	ASSERT(pItem->m_pItems == this);

	int nItemIndex = GetIndex(pItem);

	if ((nItemIndex == -1) || nItemIndex == nIndex)
		return;

	m_arrItems.RemoveAt(nItemIndex);

	m_arrItems.InsertAt(nIndex, pItem);

	m_pPanel->Reposition(FALSE);
}

void CXTPTaskPanelItems::Clear(BOOL bReposition)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPTaskPanelItem* pItem = m_arrItems[i];

		SAFE_CALLPTR(m_pPanel, OnItemRemoved(pItem));
		pItem->OnRemoved();
		pItem->InternalRelease();
	}

	m_arrItems.RemoveAll();

	if (bReposition && m_pPanel)
	{
		m_pPanel->Reposition();
	}
}

void CXTPTaskPanelItems::Remove(CXTPTaskPanelItem* pItem)
{
	int nIndex = GetIndex(pItem);

	if (nIndex != -1)
	{
		RemoveAt(nIndex);
	}
}
void CXTPTaskPanelItems::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrItems.GetSize())
		return;

	CXTPTaskPanelItem* pItem = m_arrItems[nIndex];
	m_pPanel->OnItemRemoved(pItem);
	pItem->OnRemoved();

	pItem->InternalRelease();
	m_arrItems.RemoveAt(nIndex);

	m_pPanel->Reposition();
}

CXTPTaskPanelItem* CXTPTaskPanelItems::GetFirstVisibleItem() const
{
	return GetAt(GetNextVisibleIndex(-1, +1));
}

CXTPTaskPanelItem* CXTPTaskPanelItems::GetLastVisibleItem() const
{
	return GetAt(GetNextVisibleIndex(GetCount(), -1));
}

int CXTPTaskPanelItems::GetNextVisibleIndex(int nIndex, int nDirection) const
{
	CXTPTaskPanelItem* pItem = NULL;

	do
	{
		nIndex += nDirection;

		pItem = GetAt(nIndex);
		if (!pItem)
			return -1;

		if (pItem->IsVisible())
			return nIndex;
	} while (pItem);

	return -1;
}

void CXTPTaskPanelItems::DoPropExchange(CXTPPropExchange* pPX)
{
	m_pPanel->SetLockRedraw(TRUE);

	try
	{
		pPX->m_dwData = (DWORD_PTR)m_pPanel;

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Item")));
		POSITION pos = pEnumerator->GetPosition(XTPToDWORDChecked(GetCount()));

		if (pPX->IsStoring())
		{
			for (int i = 0; i < GetCount(); i++)
			{
				CXTPTaskPanelItem* pItem = GetAt(i);

				CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
				PX_Object(&sec, pItem, GetDefaultItemClass());
			}
		}
		else
		{
			Clear(FALSE);

			while (pos)
			{
				CXTPTaskPanelItem* pItem = NULL;

				CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
				PX_Object(&sec, pItem, GetDefaultItemClass());

				if (!pItem)
					AfxThrowArchiveException(CArchiveException::badClass);

				pItem->m_pItems = this;
				pItem->OnInserted();

				m_arrItems.Add(pItem);
			}
		}
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItems::Serialize. OLE exception: %x\r\n"), pEx->m_sc);
		pEx->Delete();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItems::Serialize. Archive exception\r\n"));
		pEx->Delete();
	}

	m_pPanel->SetLockRedraw(FALSE);
}

CRuntimeClass* CXTPTaskPanelItems::GetDefaultItemClass() const
{
	return RUNTIME_CLASS(CXTPTaskPanelItem);
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroups

CXTPTaskPanelGroups::CXTPTaskPanelGroups(CXTPTaskPanel* pPanel)
	: CXTPTaskPanelItems(pPanel)
{
	EnableAutomation();
}

CRuntimeClass* CXTPTaskPanelGroups::GetDefaultItemClass() const
{
	return RUNTIME_CLASS(CXTPTaskPanelGroup);
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroupItems

CXTPTaskPanelGroupItems::CXTPTaskPanelGroupItems(CXTPTaskPanel* pPanel)
	: CXTPTaskPanelItems(pPanel)
{
	EnableAutomation();
}

CRuntimeClass* CXTPTaskPanelGroupItems::GetDefaultItemClass() const
{
	return RUNTIME_CLASS(CXTPTaskPanelGroupItem);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTaskPanelItems, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Remove", dispidRemove, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Clear", dispidClear, OleClear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Find", dispidFind, OleFind, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Move", 8, OleMove, VT_EMPTY, VTS_DISPATCH VTS_I4)

	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Item", DISPID_VALUE, OleItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPTaskPanelItems, CXTPCmdTarget)
	INTERFACE_PART(CXTPTaskPanelItems, XTPDIID_ITaskPanelItems, Dispatch)
	// INTERFACE_PART(CXTPTaskPanelItems, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelItems, XTPDIID_ITaskPanelItems)

BEGIN_DISPATCH_MAP(CXTPTaskPanelGroups, CXTPTaskPanelItems)
	DISP_FUNCTION_ID(CXTPTaskPanelGroups, "Add", dispidAdd, OleAdd, VT_DISPATCH, VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_DISPATCH_MAP(CXTPTaskPanelGroupItems, CXTPTaskPanelItems)
	DISP_FUNCTION_ID(CXTPTaskPanelGroupItems, "Add", dispidAdd, OleAdd, VT_DISPATCH,
					 VTS_I4 VTS_BSTR VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPTaskPanelGroupItems, "Insert", 6, OleInsert, VT_DISPATCH,
					 VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_VARIANT)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPTaskPanelGroups, CXTPTaskPanelItems)
	INTERFACE_PART(CXTPTaskPanelGroups, XTPDIID_ITaskPanelGroups, Dispatch)
	// INTERFACE_PART(CXTPTaskPanelGroups, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelGroups, XTPDIID_ITaskPanelGroups)

BEGIN_INTERFACE_MAP(CXTPTaskPanelGroupItems, CXTPTaskPanelItems)
	INTERFACE_PART(CXTPTaskPanelGroupItems, XTPDIID_ITaskPanelGroupItems, Dispatch)
	// INTERFACE_PART(CXTPTaskPanelGroupItems, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelGroupItems, XTPDIID_ITaskPanelGroupItems)

LPDISPATCH CXTPTaskPanelGroups::OleAdd(long nID, LPCTSTR lpCaption)
{
	CXTPTaskPanelItem* pGroup = Add(new CXTPTaskPanelGroup(m_pPanel), nID);
	pGroup->SetCaption(lpCaption);

	return pGroup->GetIDispatch(TRUE);
}

LPDISPATCH CXTPTaskPanelGroupItems::OleAdd(long nID, LPCTSTR lpCaption, long nType,
										   const VARIANT& varImageIndex)
{
	CXTPTaskPanel::CRepositionContext context(m_pPanel);

	CXTPTaskPanelItem* pItem = Add(new CXTPTaskPanelGroupItem(), nID);
	pItem->SetCaption(lpCaption);
	pItem->SetType((XTPTaskPanelItemType)nType);
	pItem->SetIconIndex(VariantToLong(&varImageIndex, -1));

	return pItem->GetIDispatch(TRUE);
}

LPDISPATCH CXTPTaskPanelGroupItems::OleInsert(long nIndex, long nID, LPCTSTR lpCaption, long nType,
											  const VARIANT& varImageIndex)
{
	CXTPTaskPanel::CRepositionContext context(m_pPanel);

	CXTPTaskPanelItem* pItem = InsertAt(nIndex - 1, new CXTPTaskPanelGroupItem());
	pItem->SetID(nID);
	pItem->SetCaption(lpCaption);
	pItem->SetType((XTPTaskPanelItemType)nType);
	pItem->SetIconIndex(VariantToLong(&varImageIndex, -1));

	return pItem->GetIDispatch(TRUE);
}

void CXTPTaskPanelItems::OleMove(LPDISPATCH lpItem, long nIndex)
{
	Move(CXTPTaskPanelItem::FromDispatch(lpItem), nIndex - 1);
}

LPDISPATCH CXTPTaskPanelItems::OleFind(long nID)
{
	CXTPTaskPanelItem* pItem = Find(nID);
	return pItem ? pItem->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPTaskPanelItems::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPTaskPanelItem* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPTaskPanelItems::OleItem(long nIndex)
{
	return OleGetItem(nIndex - 1);
}

void CXTPTaskPanelItems::OleClear()
{
	Clear();
}
void CXTPTaskPanelItems::OleRemove(long nIndex)
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

long CXTPTaskPanelItems::OleGetItemCount()
{
	return GetCount();
}

IMPLEMENT_ENUM_VARIANT(CXTPTaskPanelItems)

#endif
