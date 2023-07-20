// XTPGridRecord.cpp : implementation of the CXTPGridRecord class.
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/ItemTypes/XTPGridRecordItemPreview.h"
#include "GridControl/ItemTypes/XTPGridRecordItemVariant.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPGridRecord, CXTPCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecord::CXTPGridRecord()
	: m_pPreviewItem(NULL)
	, m_pControl(NULL)
	, m_pRecords(NULL)
	, m_pChildren(NULL)
	, m_bExpanded(FALSE)
	, m_bEditable(TRUE)
	, m_bVisible(TRUE)
	, m_bLocked(FALSE)
	, m_bSelected(FALSE)
{
	m_nIndex = -1;

	m_vtBookmark.vt = VT_EMPTY;
	//<<TC>>
	m_nFreeHeight = 0;
//<<TC>>
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridRecord::~CXTPGridRecord()
{
	RemoveAll();

	if (m_pChildren)
		m_pChildren->InternalRelease();
}

void CXTPGridRecord::RemoveAll()
{
	for (int nItem = GetItemCount() - 1; nItem >= 0; nItem--)
	{
		CXTPGridRecordItem* pItem = m_arrItems.GetAt(nItem);
		if (pItem)
			pItem->InternalRelease();
	}
	m_arrItems.RemoveAll();

	CMDTARGET_RELEASE(m_pPreviewItem);
}

void CXTPGridRecord::Delete()
{
	ASSERT(m_pRecords);
	if (m_pRecords)
		m_pRecords->RemoveAt(m_nIndex);
}

BOOL CXTPGridRecord::HasChildren() const
{
	return m_pChildren && (m_pChildren->GetCount() > 0);
}

CXTPGridRecords* CXTPGridRecord::GetChilds()
{
	if (m_pChildren == NULL)
	{
		m_pChildren = new CXTPGridRecords(this);
	}

	if (GetRecords())
	{
		BOOL bCase = GetRecords()->IsCaseSensitive();
		m_pChildren->SetCaseSensitive(bCase);
	}
	return m_pChildren;
}

CXTPGridRecordItem* CXTPGridRecord::GetItem(CXTPGridColumn* pColumn) const
{
	if (this == NULL)
		return NULL;

	return GetItem(pColumn->GetItemIndex());
}

int CXTPGridRecord::IndexOf(const CXTPGridRecordItem* pItem) const
{
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		if (GetItem(nItem) == pItem)
			return nItem;
	}
	return -1;
}

CXTPGridRecordItem* CXTPGridRecord::AddItem(CXTPGridRecordItem* pItem)
{
	m_arrItems.Add(pItem);
	pItem->m_pControl = this->m_pControl;
	pItem->SetRecord(this);
	return pItem;
}

CXTPGridRecordItemPreview* CXTPGridRecord::GetItemPreview() const
{
	return m_pPreviewItem;
}

void CXTPGridRecord::SetEditable(BOOL bEditable)
{
	m_bEditable = bEditable;
}

void CXTPGridRecord::SetPreviewItem(CXTPGridRecordItemPreview* pItemPreview)
{
	if (m_pPreviewItem)
		m_pPreviewItem->InternalRelease();
	m_pPreviewItem = pItemPreview;

	m_pPreviewItem->m_pRecord = this;
}

BOOL CXTPGridRecord::IsFiltered() const
{
	return FALSE;
}

int CXTPGridRecord::GetIndex() const
{
	return m_nIndex;
}

void CXTPGridRecord::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Locked"), m_bLocked);
	PX_Bool(pPX, _T("Editable"), m_bEditable);

	BOOL bPreview = m_pPreviewItem != NULL;
	PX_Bool(pPX, _T("Preview"), bPreview);

	int nCount = GetItemCount();

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("Item")));

	if (pPX->IsStoring())
	{
		POSITION posItem = pEnumItems->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPGridRecordItem* pItem = GetItem(i);
			ASSERT(pItem);
			if (!pItem)
				AfxThrowArchiveException(CArchiveException::badClass);

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Object(&secItem, pItem, RUNTIME_CLASS(CXTPGridRecordItem));
		}
	}
	else
	{
		RemoveAll();

		POSITION posItem = pEnumItems->GetPosition();

		while (posItem)
		{
			CXTPGridRecordItem* pItem = NULL;

			CXTPPropExchangeSection sec(pEnumItems->GetNext(posItem));
			PX_Object(&sec, pItem, RUNTIME_CLASS(CXTPGridRecordItem));

			if (!pItem)
				AfxThrowArchiveException(CArchiveException::badClass);

			AddItem(pItem);
		}
	}

	//------------------------------------------------------------
	if (bPreview)
	{
		CXTPPropExchangeSection secPreviewItem(pPX->GetSection(_T("PreviewItem")));

		if (pPX->IsLoading())
		{
			CMDTARGET_RELEASE(m_pPreviewItem);
		}

		PX_Object(&secPreviewItem, m_pPreviewItem, RUNTIME_CLASS(CXTPGridRecordItemPreview));

		if (m_pPreviewItem && pPX->IsLoading())
		{
			m_pPreviewItem->m_pRecord = this;
		}
	}

	//------------------------------------------------------------
	if (pPX->GetSchema() > _XTP_SCHEMA_1041)
	{
		BOOL bHasChildren = HasChildren();
		PX_Bool(pPX, _T("HasChildren"), bHasChildren, FALSE);

		if (bHasChildren)
		{
			CXTPPropExchangeSection secChildren(pPX->GetSection(_T("Children")));
			GetChilds()->_DoPropExchange(&secChildren);
		}
		else if (m_pChildren)
		{
			m_pChildren->RemoveAll();
		}
	}
}

void CXTPGridRecord::TreeAddRef()
{
	InternalAddRef();
	if (HasChildren())
	{
		for (int nChild = 0; nChild < GetChilds()->GetCount(); nChild++)
		{
			GetChilds()->GetAt(nChild)->TreeAddRef();
		}
	}
}

void CXTPGridRecord::TreeRelease()
{
	if (HasChildren())
	{
		for (int nChild = 0; nChild < GetChilds()->GetCount(); nChild++)
		{
			GetChilds()->GetAt(nChild)->TreeRelease();
		}
	}
	InternalRelease();
}

CXTPMarkupContext* CXTPGridRecord::GetMarkupContext() const
{
	CXTPGridRecords* pRecords = m_pRecords;

	while (pRecords != NULL)
	{
		CXTPMarkupContext* pMarkupContext = pRecords->GetMarkupContext();
		if (pMarkupContext)
			return pMarkupContext;

		if (pRecords->m_pOwnerRecord)
			pRecords = pRecords->m_pOwnerRecord->GetRecords();
		else
			return NULL;
	}

	return NULL;
}

CXTPGridRecord* CXTPGridRecord::GetParentRecord() const
{
	if (m_pRecords && m_pRecords->GetOwnerRecord())
		return m_pRecords->GetOwnerRecord();

	return NULL;
}

void CXTPGridRecord::SetGridControl(CXTPGridControl* pGridControl)
{
	m_pControl = pGridControl;
	for (int nItem = 0; nItem < GetItemCount(); ++nItem)
	{
		CXTPGridRecordItem* pItem = m_arrItems.GetAt(nItem);
		if (pItem)
		{
			pItem->m_pControl = m_pControl;
			pItem->ResetMarkupUIElement();
		}
	}
}

void CXTPGridRecord::SetExpanded(BOOL bExpanded)
{
	m_bExpanded = bExpanded;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridRecord, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridRecord, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecord, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecord, "AddItem", dispidAdd, OleAdd, VT_DISPATCH, VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridRecord, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecord, "Childs", 6, OleGetChilds, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPGridRecord, "PreviewText", 7, OleGetPreviewText, OleSetPreviewText,
						VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridRecord, "Expanded", 8, m_bExpanded, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecord, "Tag", 9, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPGridRecord, "Index", 10, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPGridRecord, "Visible", 11, m_bVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridRecord, "PreviewItem", 12, OleGetPreviewItem, SetNotSupported,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridRecord, "ParentRecord", 13, OleGetParentRecord, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecord, "Records", 14, OleGetRecords, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPGridRecord, "ItemCount", 15, OleGetItemCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPGridRecord, "Bookmark", 16, m_vtBookmark, VT_VARIANT)

	DISP_FUNCTION_ID(CXTPGridRecord, "AddItemEx", 17, OleAddEx, VT_EMPTY, VTS_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPGridRecord, "FreeHeight", 18, OleGetFreeHeight, OleSetFreeHeight, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecord, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecord, XTPDIID_IGridRecord, Dispatch)
	// INTERFACE_PART(CXTPGridRecord, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecord, XTPDIID_IGridRecord)

CXTPGridRecord* AFX_CDECL CXTPGridRecord::FromDispatch(LPDISPATCH pDisp)
{
	CXTPGridRecord* pInst = NULL;
	if (NULL != pDisp)
	{
		pInst = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPGridRecord,
											 CXTPGridRecord::FromIDispatchSafe(pDisp));
		if (NULL == pInst)
		{
			AfxThrowOleException(E_INVALIDARG);
		}
	}

	return pInst;
}

LPDISPATCH CXTPGridRecord::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetItemCount())
	{
		return GetItem(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANT(CXTPGridRecord)

int CXTPGridRecord::OleGetItemCount()
{
	return GetItemCount();
}

LPDISPATCH CXTPGridRecord::OleAdd(const VARIANT& lpValue)
{
	if (lpValue.vt == (VT_VARIANT | VT_BYREF))
	{
		return OleAdd(*lpValue.pvarVal);
	}

	CXTPGridRecordItemVariant* pItem = new CXTPGridRecordItemVariant(lpValue);

	AddItem(pItem);

	return pItem->GetIDispatch(TRUE);
}

void CXTPGridRecord::OleAddEx(LPDISPATCH lpDisp)
{
	CXTPGridRecordItem* pItem = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPGridRecordItem,
															 CXTPGridRecordItem::FromIDispatchSafe(
																 lpDisp));
	if (pItem)
	{
		AddItem(pItem);
		pItem->InternalAddRef();
	}
}

LPDISPATCH CXTPGridRecord::OleGetChilds()
{
	return GetChilds()->GetIDispatch(TRUE);
}

BSTR CXTPGridRecord::OleGetPreviewText()
{
	CString strPreviewText;

	if (m_pPreviewItem)
	{
		strPreviewText = m_pPreviewItem->GetPreviewText();
	}

	return strPreviewText.AllocSysString();
}

void CXTPGridRecord::OleSetPreviewText(LPCTSTR lpszText)
{
	if (lpszText)
	{
		if (!m_pPreviewItem)
		{
			SetPreviewItem(new CXTPGridRecordItemPreview());
		}

		m_pPreviewItem->SetPreviewText(lpszText);
	}
	else if (m_pPreviewItem)
	{
		m_pPreviewItem->InternalRelease();
		m_pPreviewItem = NULL;
	}
}

LPDISPATCH CXTPGridRecord::OleGetPreviewItem()
{
	return m_pPreviewItem ? m_pPreviewItem->GetIDispatch(TRUE) : FALSE;
}

int CXTPGridRecord::OleGetFreeHeight()
{
	return m_nFreeHeight;
}

void CXTPGridRecord::OleSetFreeHeight(int nFreeHeight)
{
	m_nFreeHeight = nFreeHeight;
}

LPDISPATCH CXTPGridRecord::OleGetParentRecord()
{
	if (m_pRecords && m_pRecords->GetOwnerRecord())
		return m_pRecords->GetOwnerRecord()->GetIDispatch(TRUE);

	return NULL;
}

LPDISPATCH CXTPGridRecord::OleGetRecords()
{
	if (m_pRecords)
		return m_pRecords->GetIDispatch(TRUE);

	return NULL;
}

#endif
