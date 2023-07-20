// XTPGridColumns.cpp : implementation of the CXTPGridColumns class.
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
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPGridColumns

CXTPGridColumns::CXTPGridColumns(CXTPGridControl* pControl)
	: m_pControl(pControl)
{
	m_pGroupsOrder = new CXTPGridColumnOrder(this);
	m_pSortOrder   = new CXTPGridColumnOrder(this);

	m_pTreeColumn = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridColumns::~CXTPGridColumns()
{
	Clear();

	if (m_pGroupsOrder)
		m_pGroupsOrder->InternalRelease();
	if (m_pSortOrder)
		m_pSortOrder->InternalRelease();
}

int CXTPGridColumns::GetAtOffset(int nOffset, BOOL bFrozen) const
{
	int nIndex  = 0;
	int nLeft   = 0;
	int nColumn = 0;

	if (!bFrozen)
	{
		m_pControl->GetFreezeColumnsCount(nColumn);
		nColumn = (nColumn == -1) ? 0 : nColumn + 1; // next column's index after last frozen column
	}

	for (; nColumn < GetCount(); nColumn++)
	{
		const CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn->IsVisible())
		{
			if (nOffset >= nLeft && nOffset < nLeft + pColumn->GetWidth())
			{
				return nIndex;
			}

			++nIndex;
			nLeft += pColumn->GetWidth();
		}
	}

	return -1;
}

int CXTPGridColumns::GetOffset(int nIndex) const
{
	int nOffset = 0;
	int nColumn = 0;

	m_pControl->GetFreezeColumnsCount(nColumn);
	nColumn = (nColumn == -1) ? 0 : nColumn + 1; // next column's index after last frozen column

	for (; nColumn < GetCount() && nIndex > 0; nColumn++)
	{
		const CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn->IsVisible())
		{
			nOffset += pColumn->GetWidth();
			nIndex -= 1;
		}
	}

	return nOffset;
}

void CXTPGridColumns::Clear()
{
	// array cleanup
	for (int nColumn = GetCount() - 1; nColumn >= 0; nColumn--)
	{
		CXTPGridColumn* pColumn = m_arrColumns.GetAt(nColumn);
		if (pColumn)
			pColumn->InternalRelease();
	}
	m_arrColumns.RemoveAll();

	m_pSortOrder->Clear();
	m_pGroupsOrder->Clear();

	// clear variables which could be references to those values
	if (m_pControl && (m_pControl->GetColumns() == this))
		m_pControl->SetFocusedColumn(NULL);
}

int CXTPGridColumnOrder::GetCount() const
{
	return (int)m_arrColumns.GetSize();
}

void CXTPGridColumns::Add(CXTPGridColumn* pColumn)
{
	pColumn->m_pColumns = this;
	pColumn->ResetMarkupUIElement();
	m_arrColumns.Add(pColumn);

	GetGridHeader()->OnColumnsChanged(xtpGridColumnOrderChanged | xtpGridColumnAdded, pColumn);
}

CXTPGridHeader* CXTPGridColumns::GetGridHeader() const
{
	return m_pControl->GetGridHeader();
}

void CXTPGridColumns::Remove(CXTPGridColumn* pColumn)
{
	m_pGroupsOrder->Remove(pColumn);
	m_pSortOrder->Remove(pColumn);

	int nIndex = IndexOf(pColumn);
	if (nIndex != -1)
	{
		m_arrColumns.RemoveAt(nIndex);
		GetGridHeader()->OnColumnsChanged(xtpGridColumnOrderChanged | xtpGridColumnRemoved,
										  pColumn);
		pColumn->InternalRelease();
	}
}

int CXTPGridColumns::IndexOf(const CXTPGridColumn* pColumn) const
{
	// array cleanup
	for (int nColumn = GetCount() - 1; nColumn >= 0; nColumn--)
	{
		if (m_arrColumns.GetAt(nColumn) == pColumn)
			return nColumn;
	}
	return -1;
}

void CXTPGridColumns::ResetSortOrder()
{
	m_pSortOrder->Clear();
}

void CXTPGridColumns::SetSortColumn(CXTPGridColumn* pColumn, BOOL bIncreasing)
{
	ResetSortOrder();
	m_pSortOrder->Add(pColumn, bIncreasing);
}

int CXTPGridColumns::ChangeColumnOrder(int nNewIndex, int nItemIndex)
{
	if (nNewIndex < 0 || nItemIndex < 0)
		return -1;

	CXTPGridColumn* pColumn = GetAt(nItemIndex);

	if (pColumn)
	{
		if (nNewIndex == nItemIndex)
			return nNewIndex;

		if (nNewIndex > nItemIndex)
			nNewIndex--;

		m_arrColumns.RemoveAt(nItemIndex);
		m_arrColumns.InsertAt(nNewIndex, pColumn);
	}

	return nNewIndex;
}

int CXTPGridColumns::GetVisibleColumnsCount() const
{
	int nCount = 0;

	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		const CXTPGridColumn* pColumn = GetAt(nColumn);

		if (NULL != pColumn && pColumn->IsVisible())
		{
			nCount++;
		}
	}

	return nCount;
}

void CXTPGridColumns::GetVisibleColumns(CXTPGridColumns& arrColumns) const
{
	int nCount = GetCount();
	for (int nColumn = 0; nColumn < nCount; nColumn++)
	{
		CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			arrColumns.m_arrColumns.Add(pColumn);
			pColumn->InternalAddRef();
		}
	}
}

CXTPGridColumn* CXTPGridColumns::Find(int nItemIndex) const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn->GetItemIndex() == nItemIndex)
			return pColumn;
	}
	return NULL;
}

CXTPGridColumn* CXTPGridColumns::Find(const CString& strInternalName) const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn->GetInternalName() == strInternalName)
			return pColumn;
	}
	return NULL;
}

void CXTPGridColumns::InsertSortColumn(CXTPGridColumn* pColumn)
{
	if (m_pSortOrder->IndexOf(pColumn) == -1)
		m_pSortOrder->Add(pColumn);
}

CXTPGridColumn* CXTPGridColumns::GetVisibleAt(int nIndex) const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPGridColumn* pColumn = GetAt(nColumn);
		if (!pColumn->IsVisible())
			continue;

		if (nIndex == 0)
			return pColumn;

		nIndex--;
	}
	return NULL;
}

CXTPGridColumn* CXTPGridColumns::GetFirstVisibleColumn() const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn->IsVisible())
			return pColumn;
	}
	return NULL;
}

CXTPGridColumn* CXTPGridColumns::GetLastVisibleColumn() const
{
	for (int nColumn = GetCount() - 1; nColumn >= 0; nColumn--)
	{
		CXTPGridColumn* pColumn = GetAt(nColumn);
		if (pColumn->IsVisible())
			return pColumn;
	}
	return NULL;
}

void CXTPGridColumns::DoPropExchange(CXTPPropExchange* pPX)
{
	int nItemIndex;
	CString strInternalName;

	if (pPX->IsStoring())
	{
		int nCount = GetCount();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Column")));
		POSITION pos = pEnumerator->GetPosition(XTPToDWORDChecked(nCount), FALSE);

		for (int nColumn = 0; nColumn < nCount; nColumn++)
		{
			CXTPGridColumn* pColumn = GetAt(nColumn);
			CXTPPropExchangeSection secColumn(pEnumerator->GetNext(pos));

			nItemIndex		= pColumn->GetItemIndex();
			strInternalName = pColumn->GetInternalName();

			PX_Int(&secColumn, _T("ItemIndex"), nItemIndex);
			PX_String(&secColumn, _T("InternalName"), strInternalName);

			pColumn->DoPropExchange(&secColumn);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Column")));
		POSITION pos = pEnumerator->GetPosition(0, FALSE);

		CXTPGridColumn tmpColumn(0, _T(""), 0);
		int i = 0;
		while (pos)
		{
			CXTPPropExchangeSection secColumn(pEnumerator->GetNext(pos));

			CXTPGridColumn* pColumn = NULL;
			PX_Int(&secColumn, _T("ItemIndex"), nItemIndex, -1);

			if (pPX->GetSchema() > _XTP_SCHEMA_1100)
			{
				PX_String(&secColumn, _T("InternalName"), strInternalName);

				if (!strInternalName.IsEmpty())
				{
					pColumn = Find(strInternalName);

					if (!pColumn
						&& pPX->GetSchema() < _XTP_SCHEMA_1500) // before 15.0 release internal name
																// was equal to caption
					{
						for (int nColumn = 0; nColumn < GetCount(); nColumn++)
						{
							CXTPGridColumn* p = GetAt(nColumn);
							if (p->GetCaption() == strInternalName)
							{
								pColumn = p;
								break;
							}
						}
					}

					// column data is exists but column is not in the collection.
					if (!pColumn)
					{
						// just read data to skeep (to be safe for array serialization)
						tmpColumn.DoPropExchange(&secColumn);
						continue;
					}
				}
			}

			if (!pColumn)
				pColumn = Find(nItemIndex);

			if (!pColumn)
				AfxThrowArchiveException(CArchiveException::badIndex);

			pColumn->DoPropExchange(&secColumn);
			ChangeColumnOrder(i, IndexOf(pColumn));
			i++;
		}
	}

	CXTPPropExchangeSection secGroupsOrder(pPX->GetSection(_T("GroupsOrder")));
	m_pGroupsOrder->DoPropExchange(&secGroupsOrder);

	CXTPPropExchangeSection secSortOrder(pPX->GetSection(_T("SortOrder")));
	m_pSortOrder->DoPropExchange(&secSortOrder);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPGridColumnOrder

CXTPGridColumnOrder::CXTPGridColumnOrder(CXTPGridColumns* pColumns)
	: m_pColumns(pColumns)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridColumn* CXTPGridColumnOrder::GetAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
		return m_arrColumns.GetAt(nIndex);
	else
		return NULL;
}

int CXTPGridColumnOrder::InsertAt(int nIndex, CXTPGridColumn* pColumn)
{
	if (nIndex < 0)
		return -1;

	if (nIndex >= GetCount())
		nIndex = GetCount();

	int nPrevIndex = IndexOf(pColumn);

	if (nPrevIndex != -1)
	{
		if (nPrevIndex == nIndex)
			return nIndex;

		if (nIndex > nPrevIndex)
			nIndex--;

		if (nIndex == nPrevIndex)
			return nIndex;

		// change order
		m_arrColumns.RemoveAt(nPrevIndex);
	}
	m_arrColumns.InsertAt(nIndex, pColumn);
	return nIndex;
}

int CXTPGridColumnOrder::Add(CXTPGridColumn* pColumn, BOOL bSortIncreasing)
{
	pColumn->m_bSortIncreasing = bSortIncreasing;
	return (int)m_arrColumns.Add(pColumn);
}

int CXTPGridColumnOrder::Add(CXTPGridColumn* pColumn)
{
	return (int)m_arrColumns.Add(pColumn);
}

void CXTPGridColumnOrder::Clear()
{
	m_arrColumns.RemoveAll();
}

int CXTPGridColumnOrder::IndexOf(const CXTPGridColumn* pColumn)
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (GetAt(i) == pColumn)
			return i;
	}
	return -1;
}

void CXTPGridColumnOrder::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
		m_arrColumns.RemoveAt(nIndex);
}

void CXTPGridColumnOrder::Remove(CXTPGridColumn* pColumn)
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (GetAt(i) == pColumn)
		{
			m_arrColumns.RemoveAt(i);
			break;
		}
	}
}

void CXTPGridColumnOrder::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsStoring())
	{
		int nCount = GetCount();
		PX_Int(pPX, _T("Count"), nCount, 0);

		for (int i = 0; i < nCount; i++)
		{
			CXTPGridColumn* pColumn = GetAt(i);
			if (pColumn)
			{
				int nItemIndex			= pColumn->GetItemIndex();
				CString strInternalName = pColumn->GetInternalName();
				CString strParamName;

				strParamName.Format(_T("Column%i"), i);
				PX_Int(pPX, strParamName, nItemIndex, 0);

				strParamName.Format(_T("InternalName%i"), i);
				PX_String(pPX, strParamName, strInternalName);
			}
		}
	}
	else
	{
		Clear();

		int nCount = 0;
		PX_Int(pPX, _T("Count"), nCount, 0);

		for (int i = 0; i < nCount; i++)
		{
			int nItemIndex = 0;
			CString strParamName;
			strParamName.Format(_T("Column%i"), i);
			PX_Int(pPX, strParamName, nItemIndex, 0);

			CXTPGridColumn* pColumn = NULL;
			if (pPX->GetSchema() > _XTP_SCHEMA_1100)
			{
				strParamName.Format(_T("InternalName%i"), i);
				CString strInternalName;
				PX_String(pPX, strParamName, strInternalName);

				if (!strInternalName.IsEmpty())
					pColumn = m_pColumns->Find(strInternalName);
			}
			if (!pColumn)
				pColumn = m_pColumns->Find(nItemIndex);

			if (pColumn)
				Add(pColumn);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridColumns, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridColumns, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridColumns, "Column", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridColumns, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridColumns, "Add", dispidAdd, OleAdd, VT_DISPATCH,
					 VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CXTPGridColumns, "AddEx", dispidAddEx, OleAddEx, VT_DISPATCH,
					 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CXTPGridColumns, "Find", dispidFindColumn, OleFindColumn, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridColumns, "DeleteAll", 5, Clear, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridColumns, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridColumns, XTPDIID_IGridColumns, Dispatch)
	// INTERFACE_PART(CXTPGridColumns, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridColumns, XTPDIID_IGridColumns)

int CXTPGridColumns::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPGridColumns::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANT(CXTPGridColumns)

LPDISPATCH CXTPGridColumns::OleAdd(long nId, LPCTSTR strCaption, int nWidth, BOOL bAutoSize)
{
	CXTPGridColumn* pColumn = new CXTPGridColumn(nId, strCaption, nWidth, bAutoSize);
	Add(pColumn);

	return pColumn->GetIDispatch(TRUE);
}

LPDISPATCH CXTPGridColumns::OleAddEx(long nId, LPCTSTR strCaption, LPCTSTR strInternalName,
									 int nWidth, BOOL bAutoSize)
{
	CXTPGridColumn* pColumn = new CXTPGridColumn(nId, strCaption, strInternalName, nWidth,
												 bAutoSize);
	Add(pColumn);

	return pColumn->GetIDispatch(TRUE);
}

LPDISPATCH CXTPGridColumns::OleFindColumn(long nId)
{
	CXTPGridColumn* pColumn = Find(nId);
	return pColumn ? pColumn->GetIDispatch(TRUE) : NULL;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CXTPGridColumnOrder, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "Column", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "DeleteAll", 5, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "IndexOf", 6, OleIndexOf, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridColumnOrder, "RemoveAt", 7, RemoveAt, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridColumnOrder, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridColumnOrder, XTPDIID_IGridColumnOrder, Dispatch)
	// INTERFACE_PART(CXTPGridColumnOrder, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridColumnOrder, XTPDIID_IGridColumnOrder)

int CXTPGridColumnOrder::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPGridColumnOrder::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

void CXTPGridColumnOrder::OleAdd(LPDISPATCH lpColumn)
{
	CXTPGridColumn* pColumn = CXTPGridColumn::FromDispatch(lpColumn);

	if (pColumn)
	{
		Add(pColumn);
	}
}

int CXTPGridColumnOrder::OleIndexOf(LPDISPATCH lpColumn)
{
	CXTPGridColumn* pColumn = CXTPGridColumn::FromDispatch(lpColumn);

	if (pColumn)
	{
		return IndexOf(pColumn);
	}
	return -1;
}

IMPLEMENT_ENUM_VARIANT(CXTPGridColumnOrder)

#endif
