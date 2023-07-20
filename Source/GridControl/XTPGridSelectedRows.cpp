// XTPGridSelectedRows.cpp : implementation of the CXTPGridSelectedRows class.
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
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridSelectedRows.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridSelectedRows

CXTPGridSelectedRows::CXTPGridSelectedRows(CXTPGridControl* pControl)
	: m_pControl(pControl)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_nRowBlockBegin = -1;
	m_nPosSelected   = 0;
	SetChanged(FALSE);
	m_nRowType		 = xtpRowTypeBody;
	m_bNotifyOnClear = TRUE;
}

BOOL CXTPGridSelectedRows::Clear()
{
	return Clear(TRUE);
}

BOOL CXTPGridSelectedRows::Clear(BOOL bNotifyOnClear)
{
	if (m_arrSelectedBlocks.GetSize() > 0)
	{
		if (bNotifyOnClear && _NotifySelChanging(xtpGridSelectionClear))
		{
			return FALSE; // Handled / Cancel
		}

		m_arrSelectedBlocks.RemoveAll();
		SetChanged(TRUE);
		m_nRowBlockBegin = -1;
		m_nRowType		 = xtpRowTypeBody;
		m_pControl->RedrawControl();

		return TRUE;
	}

	return FALSE;
}

void CXTPGridSelectedRows::_InsertBlock(int nIndexInsert, int nIndexBegin, int nIndexEnd)
{
	SwapIfNeed(nIndexBegin, nIndexEnd);

	SELECTED_BLOCK block;
	block.nIndexBegin = nIndexBegin;
	block.nIndexEnd   = nIndexEnd;
	m_arrSelectedBlocks.InsertAt(nIndexInsert, block);

	SetChanged(TRUE);
}

void CXTPGridSelectedRows::DeselectBlock(int nIndexB, int nIndexE, BOOL bCollapse)
{
	SwapIfNeed(nIndexB, nIndexE);
	nIndexE += 1; // internal numbering

	int nCount = nIndexE - nIndexB;
	int i	  = (int)m_arrSelectedBlocks.GetSize() - 1;
	for (; i >= 0; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin >= nIndexE)
		{
			if (bCollapse) // block [i] is below collapsed rows, shift it upward
			{
				nIndexBegin -= nCount;
				nIndexEnd -= nCount;
				SetChanged(TRUE);
			}
			else
				continue; // deselection don't impact on selection below
		}
		else if (nIndexEnd <= nIndexB) // deselection don't impact on selection above
		{
			break;
		}
		else if (nIndexBegin >= nIndexB)
		{
			if (nIndexEnd <= nIndexE) // [nIndexB:nIndexE] fully cover [i]
			{
				m_arrSelectedBlocks.RemoveAt(i);
			}
			else // cut begin of block [i]
			{
				nIndexBegin = bCollapse ? nIndexB : nIndexE;
				nIndexEnd -= bCollapse ? nCount : 0;
			}
			SetChanged(TRUE);
		}
		else // if (nIndexBegin < nIndexB)
		{
			if (nIndexEnd <= nIndexE) // cut end of block [i]
			{
				nIndexEnd = nIndexB;
			}
			else // [i] fully cover [nIndexB:nIndexE]
			{
				if (bCollapse)
				{
					nIndexEnd -= nCount;
				}
				else
				{
					_InsertBlock(i + 1, nIndexE, nIndexEnd);
					m_arrSelectedBlocks[i].nIndexEnd = nIndexB;
				}
			}
			SetChanged(TRUE);
			break;
		}
	}

	// join adjacent blocks
	int i2 = i + 1;
	if (bCollapse && i >= 0 && i2 < m_arrSelectedBlocks.GetSize())
	{
		int& nIndexEnd1   = m_arrSelectedBlocks[i].nIndexEnd;
		int& nIndexBegin2 = m_arrSelectedBlocks[i2].nIndexBegin;
		int& nIndexEnd2   = m_arrSelectedBlocks[i2].nIndexEnd;

		if (nIndexEnd1 == nIndexBegin2)
		{
			nIndexEnd1 = nIndexEnd2;
			m_arrSelectedBlocks.RemoveAt(i2);
			SetChanged(TRUE);
		}
	}
}

void CXTPGridSelectedRows::_OnCollapsed(int nIndexB, int nCount)
{
	ASSERT(nCount > 0);
	nIndexB += 1;					// don't touch parent of collapsed rows
	int nIndexE = nIndexB + nCount; // end of collapsed block [nIndexB:nIndexE]

	int i = (int)m_arrSelectedBlocks.GetSize() - 1;
	for (; i >= 0; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin >= nIndexE) // block [i] is below collapsed rows, shift it upward
		{
			nIndexBegin -= nCount;
			nIndexEnd -= nCount;
			SetChanged(TRUE);
		}
		else if (nIndexEnd <= nIndexB) // collapsed rows don't impact on selection above
		{
			break;
		}
		else if (nIndexBegin >= nIndexB)
		{
			if (nIndexEnd <= nIndexE) // [nIndexB:nIndexE] fully cover [i]
			{
				m_arrSelectedBlocks.RemoveAt(i);
			}
			else // cut begin of block [i]
			{
				nIndexBegin = nIndexB;
				nIndexEnd -= nCount;
			}
			SetChanged(TRUE);
		}
		else // if (nIndexBegin < nIndexB)
		{
			if (nIndexEnd <= nIndexE)
				nIndexEnd = nIndexB; // cut end of block [i]
			else
				nIndexEnd -= nCount; // [i] fully cover [nIndexB:nIndexE], delete [nIndexB:nIndexE]

			SetChanged(TRUE);
			break;
		}
	}

	// join adjacent blocks
	int i2 = i + 1;
	if (i >= 0 && i2 < m_arrSelectedBlocks.GetSize())
	{
		int& nIndexEnd1   = m_arrSelectedBlocks[i].nIndexEnd;
		int& nIndexBegin2 = m_arrSelectedBlocks[i2].nIndexBegin;
		int& nIndexEnd2   = m_arrSelectedBlocks[i2].nIndexEnd;

		if (nIndexEnd1 == nIndexBegin2)
		{
			nIndexEnd1 = nIndexEnd2;
			m_arrSelectedBlocks.RemoveAt(i2);
			SetChanged(TRUE);
		}
	}
}

void CXTPGridSelectedRows::_OnExpanded(int nIndex, int nCount)
{
	ASSERT(nCount > 0);

	for (int i = (int)m_arrSelectedBlocks.GetSize() - 1; i >= 0; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin <= nIndex && nIndexEnd > nIndex + 1)
		{
			_InsertBlock(i + 1, nIndex + 1 + nCount, nIndexEnd + nCount);
			m_arrSelectedBlocks[i].nIndexEnd = nIndex + 1;
			SetChanged(TRUE);
		}
		else if (nIndexBegin > nIndex)
		{
			nIndexBegin += nCount;
			nIndexEnd += nCount;
			SetChanged(TRUE);
		}
		else if (nIndexEnd < nIndex)
			break;
	}
}

void CXTPGridSelectedRows::AddBlock(int ib, int ie, BOOL bDeselectIfSelected)
{
	SwapIfNeed(ib, ie);
	ie += 1; // internal numbering

	int i	  = 0;
	int nCount = (int)m_arrSelectedBlocks.GetSize();

	if (nCount > 0)
	{
		int& nIndexEnd = m_arrSelectedBlocks[nCount - 1].nIndexEnd;

		if (nIndexEnd == ib)
		{
			nIndexEnd = ie;
			SetChanged(TRUE);
			return;
		}

		if (nIndexEnd < ib)
		{
			i = nCount; // ignore for()
		}
	}

	for (; i < nCount; i++)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if ((nIndexBegin <= ib) && (nIndexEnd >= ie)) // block [ib:ie] is selected already
		{
			if (bDeselectIfSelected)
				DeselectBlock(ib, ie - 1, FALSE);

			return;
		}
		else if (nIndexBegin > ie)
		{
			break; // insert block [ib:ie] before block [i]
		}
		else if (nIndexEnd >= ib) // block [ib:ie] intersects block [i] or adjacent to it.
		{
			nIndexBegin = min(nIndexBegin, ib); // stretch block [i]

			// check is there other blocks between ib and ie. They should be removed.
			for (int k = i + 1; k < m_arrSelectedBlocks.GetSize(); k++)
			{
				int kb = m_arrSelectedBlocks[k].nIndexBegin;
				int ke = m_arrSelectedBlocks[k].nIndexEnd;

				if (kb <= ie) // block [k] intersect block [i] or adjacent to it.
				{
					m_arrSelectedBlocks.RemoveAt(k);
					k--;
					if (ke >= ie) // block [i] ends in block [k]
					{
						ie = ke;
						break;
					}
				}
				else
					break;
			}

			nIndexEnd = max(nIndexEnd, ie); // stretch block [i]
			SetChanged(TRUE);
			return;
		}
	}

	_InsertBlock(i, ib, ie);
}

BOOL CXTPGridSelectedRows::Add(CXTPGridRow* pRow)
{
	BOOL bAdd = FALSE;

	if (NULL != pRow)
	{
		BOOL bCancel = _NotifySelChanging(xtpGridSelectionAdd, pRow);

		if (!bCancel)
		{
			int nIndex = pRow->GetIndex();

			if (nIndex != -1)
			{
				AddBlock(nIndex, nIndex);
				bAdd			 = TRUE;
				m_nRowBlockBegin = -1;
			}
		}
	}

	return bAdd;
}

void CXTPGridSelectedRows::Remove(CXTPGridRow* pRow)
{
	if (NULL == pRow)
	{
		return;
	}

	if (_NotifySelChanging(xtpGridSelectionRemove, pRow))
	{
		return; // Handled / Cancel
	}

	int nIndex = pRow->GetIndex();

	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		int nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if ((nIndexBegin <= nIndex) && (nIndexEnd > nIndex))
		{
			if (nIndexBegin != nIndex)
			{
				_InsertBlock(i, nIndexBegin, nIndex);
				i++;
			}

			if (nIndexEnd - 1 != nIndex)
			{
				_InsertBlock(i + 1, nIndex + 1, nIndexEnd);
			}

			m_arrSelectedBlocks.RemoveAt(i);
			m_pControl->RedrawControl();
			m_nRowBlockBegin = -1;
			SetChanged(TRUE);
			return;
		}
	}
}

BOOL CXTPGridSelectedRows::Select(CXTPGridRow* pRow)
{
	BOOL bSelect = FALSE;

	if (NULL != pRow)
	{
		// If new selected row belongs to the other type - clear previous selection.
		if (m_nRowType != pRow->GetType())
		{
			Clear(GetNotifyOnClear());
			SetChanged(TRUE);
		}

		if (GetCount() == 1 && Contains(pRow))
		{
		}
		else
		{
			BOOL bClear = Clear(GetNotifyOnClear());
			BOOL bAdd   = Add(pRow);

			bSelect = bClear && bAdd;

			if (bClear || bAdd /* || Always */)
			{
				SetChanged(TRUE);
			}
			m_nRowType = pRow->GetType();
		}
	}

	return bSelect;
}

void CXTPGridSelectedRows::SelectBlock(int nBlockBegin, int nEnd, BOOL bControlKey)
{
	CXTPGridRows* pRows;
	switch (m_nRowType)
	{
		case xtpRowTypeHeader: pRows = m_pControl->GetHeaderRows(); break;
		case xtpRowTypeFooter: pRows = m_pControl->GetFooterRows(); break;
		default: pRows = m_pControl->GetRows(); break;
	}
	int nRowsCount(0);
	if (pRows)
		nRowsCount = pRows->GetCount();
	BOOL bGo = (nBlockBegin >= 0 && nBlockBegin < nRowsCount && nEnd >= 0 && nEnd < nRowsCount);
	if (!bGo)
	{
		Clear(GetNotifyOnClear());
		return;
	}

	BOOL bSkipGroupFocus = bControlKey ? FALSE : m_pControl->IsSkipGroupsFocusEnabled();
	BOOL bHandleGroups   = bSkipGroupFocus
						 && m_pControl->GetColumns()->GetGroupsOrder()->GetCount() != 0;

	if (bControlKey == FALSE) // clear selection but save settings
	{
		XTPGridRowType nRowType = m_nRowType;
		int nRowBlockBegin		= m_nRowBlockBegin;

		Clear(GetNotifyOnClear());

		m_nRowType		 = nRowType;
		m_nRowBlockBegin = nRowBlockBegin;
	}

	if (m_nRowBlockBegin != -1 && !bControlKey)
		nBlockBegin = m_nRowBlockBegin; // restore begin position

	int nBegin = nBlockBegin;
	SwapIfNeed(nBegin, nEnd);

	if (m_nRowBlockBegin == -1)
		m_nRowBlockBegin = nBlockBegin; // save begin position

	if (bHandleGroups)
	{
		int nRowBlockBegin = m_nRowBlockBegin; // save m_nRowBlockBegin because it is (-1) after
											   // Add(pRow)
		for (int i = nBegin; i <= nEnd; i++)
		{
			CXTPGridRow* pRow = pRows->GetAt(i);
			if (pRow
				&& (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded() || (i == nBegin)
					|| (i == nEnd)))
			{
				Add(pRow);
			}
		}
		m_nRowBlockBegin = nRowBlockBegin;
	}
	else
	{
		AddBlock(nBegin, nEnd);
	}

	// notify owner the selection state has changed.
	_NotifyStateChanged(nBegin, nEnd);

	if (m_pControl->m_bSelectionExcludeGroupRows)
		m_pControl->UnselectGroupRows();

	// to show only selected children under group rows
	// flag - m_bSelectionExcludeGroupRows default = TRUE - this is 12.1 way to show selection
	// TRACE(_T("return from SelectBlock with Count=%d\n"), GetCount());
}

BOOL CXTPGridSelectedRows::Contains(const CXTPGridRow* pRow)
{
	if (!pRow)
		return FALSE;

	if (m_nRowType != pRow->GetType())
		return FALSE;

	int nIndex = pRow->GetIndex();

	int nCount = (int)m_arrSelectedBlocks.GetSize();
	if (nCount == 0)
		return FALSE;

	const SELECTED_BLOCK& blockFirst = m_arrSelectedBlocks[0];
	const SELECTED_BLOCK& blockLast  = m_arrSelectedBlocks[nCount - 1];

	if (blockFirst.nIndexBegin > nIndex)
		return FALSE;

	if (blockLast.nIndexEnd <= nIndex)
		return FALSE;

	for (int i = 0; i < nCount; i++)
	{
		const SELECTED_BLOCK& block = m_arrSelectedBlocks[i];

		if ((block.nIndexBegin <= nIndex) && (block.nIndexEnd > nIndex))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CXTPGridSelectedRows::Invert(CXTPGridRow* pRow)
{
	if (pRow->GetType() != m_nRowType)
		return;

	if (Contains(pRow))
		Remove(pRow);
	else
		Add(pRow);

	m_nRowBlockBegin = -1;
	SetChanged(TRUE);
}

int CXTPGridSelectedRows::GetCount()
{
	int nCount = 0;
	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
		nCount += m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;

	return nCount;
}

POSITION CXTPGridSelectedRows::GetFirstSelectedRowPosition()
{
	if (m_arrSelectedBlocks.GetSize() == 0)
		return NULL;

	m_nPosSelected = 0;

	//  int nLast = (int) m_arrSelectedBlocks.GetSize() - 1;
	POSITION pos = (POSITION)(DWORD_PTR)(m_arrSelectedBlocks[m_nPosSelected].nIndexBegin + 1);
	// TRACE(_T("m_nPosSelected=%d Blocks=%d GetFirstSelectedRowPosition=%d\n"), m_nPosSelected,
	// nLast, (int) pos);
	return pos;
}

CXTPGridRow* CXTPGridSelectedRows::GetNextSelectedRow(POSITION& pos)
{
	ASSERT(m_nPosSelected < m_arrSelectedBlocks.GetSize());

	int nIndex = (int)(DWORD_PTR)pos - 1;
	CXTPGridRow* pRow;
	switch (m_nRowType)
	{
		case xtpRowTypeHeader: pRow = m_pControl->GetHeaderRows()->GetAt(nIndex); break;
		case xtpRowTypeFooter: pRow = m_pControl->GetFooterRows()->GetAt(nIndex); break;
		default: pRow = m_pControl->GetRows()->GetAt(nIndex); break;
	}

	int nLast = (int)m_arrSelectedBlocks.GetSize() - 1;
	if (nIndex < m_arrSelectedBlocks[m_nPosSelected].nIndexEnd - 1)
		pos++;
	else
	{
		if (m_nPosSelected >= nLast)
		{
			// TRACE(_T("m_nPosSelected=%d Blocks=%d GetNextSelectedRow=%d nIndex=%d\n"),
			// m_nPosSelected, nLast, (int) pos, nIndex);
			pos = NULL;
		}
		else
			pos = (POSITION)(DWORD_PTR)(m_arrSelectedBlocks[++m_nPosSelected].nIndexBegin + 1);
	}

	return pRow;
}

CXTPGridRow* CXTPGridSelectedRows::GetAt(int nIndex)
{
	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		int nCount = m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;
		if (nCount <= nIndex)
		{
			nIndex -= nCount;
			continue;
		}

		CXTPGridRow* pRow;
		switch (m_nRowType)
		{
			case xtpRowTypeHeader:
				pRow = m_pControl->GetHeaderRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin
														  + nIndex);
				break;
			case xtpRowTypeFooter:
				pRow = m_pControl->GetFooterRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin
														  + nIndex);
				break;
			default:
				pRow = m_pControl->GetRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin + nIndex);
				break;
		}
		return pRow;
	}

	return NULL;
}

void CXTPGridSelectedRows::_NotifyStateChanged(int nBegin, int nEnd)
{
	if (m_pControl)
	{
		XTP_NM_GRIDSTATECHANGED nm;
		::ZeroMemory(&nm, sizeof(nm));

		nm.nBegin = nBegin;
		nm.nEnd   = nEnd;

		m_pControl->SendNotifyMessage(XTP_NM_GRID_STATECHANGED, &nm.hdr);
	}
}

BOOL CXTPGridSelectedRows::_NotifySelChanging(XTPGridSelectionChangeType nType, CXTPGridRow* pRow)
{
	BOOL bResult = FALSE;

	if (NULL != m_pControl)
	{
		XTP_NM_SELECTION_CHANGING nm;
		::ZeroMemory(&nm, sizeof(nm));

		nm.pRow  = pRow;
		nm.nType = nType;

		LRESULT lResult = m_pControl->SendNotifyMessage(XTP_NM_GRID_SELCHANGING, &nm.hdr);

		bResult = (0 != lResult);
	}

	return bResult;
}

BOOL CXTPGridSelectedRows::SwapIfNeed(int& indexB, int& indexE)
{
	if (indexB > indexE)
	{
		int indexT = indexB;
		indexB	 = indexE;
		indexE	 = indexT;
		return TRUE;
	}
	return FALSE;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridSelectedRows, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridSelectedRows, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridSelectedRows, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridSelectedRows, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridSelectedRows, "DeleteAll", 5, OleDeleteAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridSelectedRows, "Row", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridSelectedRows, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridSelectedRows, XTPDIID_IGridSelectedRows, Dispatch)
	// INTERFACE_PART(CXTPGridSelectedRows, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridSelectedRows, XTPDIID_IGridSelectedRows)

void CXTPGridSelectedRows::OleAdd(LPDISPATCH lpRow)
{
	CXTPGridRow* pRow = CXTPGridRow::FromDispatch(lpRow);

	Add(pRow);
}

void CXTPGridSelectedRows::OleDeleteAll()
{
	Clear(GetNotifyOnClear());
}

int CXTPGridSelectedRows::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPGridSelectedRows::OleGetItem(long nIndex)
{
	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		int nCount = m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;
		if (nCount <= nIndex)
		{
			nIndex -= nCount;
			continue;
		}

		CXTPGridRow* pRow;
		switch (m_nRowType)
		{
			case xtpRowTypeHeader:
				pRow = m_pControl->GetHeaderRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin
														  + nIndex);
				break;
			case xtpRowTypeFooter:
				pRow = m_pControl->GetFooterRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin
														  + nIndex);
				break;
			default:
				pRow = m_pControl->GetRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin + nIndex);
				break;
		}
		return pRow ? pRow->GetIDispatch(TRUE) : NULL;
	}

	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANTLIST(CXTPGridSelectedRows)

BOOL CXTPGridSelectedRows::OleGetNextItem(/* [in, out] */ POSITION& rPos,
										  /* [out] */ VARIANT* pVar)
{
	ASSERT(pVar);
	if (!rPos || !pVar)
	{
		return FALSE;
	}

	CXTPGridRow* pRow = GetNextSelectedRow(rPos);
	if (pRow)
	{
		pVar->vt	   = VT_DISPATCH;
		pVar->pdispVal = pRow->GetIDispatch(TRUE);
		return TRUE;
	}

	return FALSE;
}

POSITION CXTPGridSelectedRows::OleGetFirstItemPosition()
{
	return GetFirstSelectedRowPosition();
}

#endif // _XTP_ACTIVEX
