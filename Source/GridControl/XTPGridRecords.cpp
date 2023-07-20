// XTPGridRecords.cpp : implementation of the CXTPGridRecords class.
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
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridRecordItemRange.h"

#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPGridRecords, CXTPCmdTarget)

void CXTPGridRecords::_Init()
{
	m_pOwnerRecord = NULL;
	m_pControl	 = NULL;

	m_pVirtualRecord	   = NULL;
	m_nVirtualRecordsCount = 0;
	m_bArray			   = FALSE;

	m_bCaseSensitive = TRUE;
	m_pMarkupContext = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPGridRecords::_swapIndexes(int& A, int& B)
{
	int C = B;
	B	 = A;
	A	 = C;
}

void CXTPGridRecords::_clampIndexes(int& nOrig, const int& nMin, const int& nMax)
{
	nOrig = max(nOrig, nMin);
	nOrig = min(nOrig, nMax);
}

CXTPGridRecords::CXTPGridRecords(BOOL bArray /*= FALSE*/)
{
	_Init();
	m_bArray = bArray;
}

CXTPGridRecords::CXTPGridRecords(CXTPGridRecord* pOwnerRecord)
{
	_Init();
	m_pOwnerRecord = pOwnerRecord;
	m_pControl	 = pOwnerRecord->m_pControl;
}

void CXTPGridRecords::SetVirtualMode(CXTPGridRecord* pVirtualRecord, int nCount)
{
	// release old virtual record
	if (m_pVirtualRecord)
		m_pVirtualRecord->InternalRelease();

	// reset virtual mode
	if (!pVirtualRecord || nCount <= 0)
	{
		if (pVirtualRecord)
			pVirtualRecord->InternalRelease();

		m_pVirtualRecord	   = NULL;
		m_nVirtualRecordsCount = 0;
	}
	else // set new virtual record
	{
		m_pVirtualRecord	   = pVirtualRecord;
		m_nVirtualRecordsCount = nCount;
		if (m_pVirtualRecord)
			m_pVirtualRecord->m_pRecords = this;
	}
}

CXTPGridRecords::~CXTPGridRecords()
{
	RemoveAll();

	if (m_pVirtualRecord)
		m_pVirtualRecord->InternalRelease();
}

void CXTPGridRecords::RemoveAll()
{
	if (!m_bArray)
	{
		// array cleanup
		for (int nRecord = (int)m_arrRecords.GetSize() - 1; nRecord >= 0; nRecord--)
		{
			CXTPGridRecord* pRecord = m_arrRecords.GetAt(nRecord);
			if (pRecord)
				pRecord->InternalRelease();
		}
	}
	m_arrRecords.RemoveAll();
}

void CXTPGridRecords::UpdateIndexes(int nStart /*= 0*/)
{
	for (int i = nStart; i < GetCount(); i++)
	{
		if (GetAt(i))
			GetAt(i)->m_nIndex = i;
	}
}

CXTPGridRecord* CXTPGridRecords::Add(CXTPGridRecord* pRecord)
{
	int nIndex = (int)m_arrRecords.Add(pRecord);

	if (m_bArray)
	{
		ASSERT(pRecord->m_pRecords);
	}
	else
	{
		pRecord->m_nIndex   = nIndex;
		pRecord->m_pRecords = this;
		pRecord->SetGridControl(m_pControl);
	}

	return pRecord;
}

void CXTPGridRecords::RemoveAt(int nIndex)
{
	if (m_bArray)
	{
		m_arrRecords.RemoveAt(nIndex);
	}
	else
	{
		if (nIndex < (int)m_arrRecords.GetSize())
		{
			if (m_arrRecords[nIndex] != NULL)
				m_arrRecords[nIndex]->InternalRelease();
			m_arrRecords.RemoveAt(nIndex);

			UpdateIndexes(nIndex);
		}
	}
}

int CXTPGridRecords::RemoveRecord(CXTPGridRecord* pRecord)
{
	ASSERT(!m_bArray);

	for (int i = 0; i < (int)m_arrRecords.GetSize(); i++)
	{
		CXTPGridRecord* pRec = m_arrRecords.GetAt(i);
		if (pRec == pRecord)
		{
			pRecord->InternalRelease();
			m_arrRecords.RemoveAt(i);

			UpdateIndexes(i);

			return i;
		}
	}

	return -1;
}

void CXTPGridRecords::InsertAt(int nIndex, CXTPGridRecord* pRecord)
{
	ASSERT(!m_bArray);

	m_arrRecords.InsertAt(nIndex, pRecord);
	pRecord->m_pRecords = this;
	pRecord->SetGridControl(m_pControl);
	UpdateIndexes(nIndex);
}

int CXTPGridRecords::GetCount() const
{
	if (m_pVirtualRecord != NULL)
		return m_nVirtualRecordsCount;

	return (int)m_arrRecords.GetSize();
}

CXTPGridRecord* CXTPGridRecords::GetAt(int nIndex) const
{
	if (m_pVirtualRecord)
	{
		m_pVirtualRecord->m_nIndex = nIndex;
		return m_pVirtualRecord;
	}

	return nIndex >= 0 && nIndex < GetCount() ? m_arrRecords.GetAt(nIndex) : NULL;
}

void CXTPGridRecords::DoPropExchange(CXTPPropExchange* pPX)
{
	pPX->ExchangeSchemaSafe();

	_DoPropExchange(pPX);
}

void CXTPGridRecords::_DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Record")));

	if (pPX->IsStoring())
	{
		int nCount   = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPGridRecord* pRecord = GetAt(i);
			ASSERT(pRecord);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pRecord, RUNTIME_CLASS(CXTPGridRecord));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPGridRecord* pRecord = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pRecord, RUNTIME_CLASS(CXTPGridRecord));

			if (!pRecord)
				AfxThrowArchiveException(CArchiveException::badClass);

			Add(pRecord);
		}
	}
}

void CXTPGridRecords::MoveRecord(int nIndex, CXTPGridRecord* pRecord, BOOL bUpdateIndexes)
{
	if (nIndex < 0)
		nIndex = 0;

	if (nIndex > GetCount())
		nIndex = GetCount();

	if (pRecord)
	{
		int nRecordIndex = pRecord->GetIndex();

		if (nRecordIndex == nIndex)
			return;

		if (GetAt(nRecordIndex) == pRecord)
		{
			if (nRecordIndex < nIndex)
				nIndex--;

			if (nRecordIndex == nIndex)
				return;

			m_arrRecords.RemoveAt(nRecordIndex);
			m_arrRecords.InsertAt(nIndex, pRecord);

			if (bUpdateIndexes)
				UpdateIndexes();
		}
	}
}

void CXTPGridRecords::Move(int nIndex, CXTPGridRecords* pRecords)
{
	if (pRecords->m_bArray)
	{
		if (nIndex < 0)
			nIndex = 0;

		int N = GetCount();

		if (nIndex > N)
			nIndex = N;

		int nRecordsCount = (int)pRecords->GetCount(), i;
		for (i = 0; i < nRecordsCount; i++)
		{
			CXTPGridRecord* pRecord = pRecords->GetAt(i);
			if (pRecord)
			{
				int nRecordIndex = pRecord->GetIndex();

				if (pRecord->m_pRecords != this)
					continue;

				if (GetAt(nRecordIndex) == pRecord)
				{
					m_arrRecords.RemoveAt(nRecordIndex);

					if (nRecordIndex < nIndex)
						nIndex--;

					for (int j = i + 1; j < nRecordsCount; j++)
					{
						pRecord = pRecords->GetAt(j);
						if (pRecord->m_pRecords != this)
							continue;

						if (pRecord->GetIndex() > nRecordIndex)
							pRecord->m_nIndex--;
					}
				}
				else
				{
					for (int k = 0; k < m_arrRecords.GetSize(); k++)
					{
						CXTPGridRecord* paRr = m_arrRecords.GetAt(k);
						if (pRecord == paRr)
							m_arrRecords.RemoveAt(k);
					}
				}
			}
		}

		for (i = 0; i < nRecordsCount; i++)
		{
			CXTPGridRecord* pRecord = pRecords->GetAt(i);
			if (pRecord)
			{
				if (pRecord->m_pRecords != this)
					continue;

				m_arrRecords.InsertAt(nIndex, pRecord);
				nIndex++;
			}
		}

		UpdateIndexes();
	}
}

int CXTPGridRecords::Compare(const CString& str1, const CString& str2) const
{
	if (!IsCaseSensitive())
		return str1.CompareNoCase(str2);

	return str1.Compare(str2);
}

void CXTPGridRecords::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy)
{
	int nSize = GetCount();
	if (!m_bArray && nNewSize < nSize)
	{
		for (int i = (int)nNewSize; i < nSize; i++)
		{
			CXTPGridRecord* pRecord = GetAt(i);
			if (pRecord)
				pRecord->InternalRelease();
		}
	}
	m_arrRecords.SetSize(nNewSize, nGrowBy);
}

void CXTPGridRecords::SetAt(INT_PTR nIndex, CXTPGridRecord* pRecord)
{
	ASSERT(pRecord);
	ASSERT(nIndex >= 0 && nIndex < GetCount());

	if (!pRecord || nIndex < 0 || nIndex >= GetCount())
		return;

	if (!m_bArray)
	{
		CXTPGridRecord* pRecord_prev = GetAt((int)nIndex);
		if (pRecord_prev)
			pRecord_prev->InternalRelease();

		pRecord->m_nIndex   = (int)nIndex;
		pRecord->m_pRecords = this;
	}

	m_arrRecords.SetAt(nIndex, pRecord);
}

CXTPGridRecord* CXTPGridRecords::FindRecordByBookmark(VARIANT vtBookmark, BOOL bSearchInChildren)
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPGridRecord* pRecord = GetAt(i);
		if (!pRecord)
			continue;
		if (pRecord->GetBookmark().dblVal == vtBookmark.dblVal)
			return pRecord;
		if (pRecord->HasChildren() && pRecord->GetChilds() && bSearchInChildren)
		{
			pRecord = pRecord->GetChilds()->FindRecordByBookmark(vtBookmark, bSearchInChildren);
			if (pRecord)
				return pRecord;
		}
	}

	return NULL;
}

CXTPGridRecordItem* CXTPGridRecords::FindRecordItem(int nStartIndex, int nEndIndex,
													int nStartColumn, int nEndColumn,
													int nRecordIndex, int nItem, LPCTSTR pcszText,
													int nFlags)
{
	if (GetCount() < 1 || GetAt(0) == NULL)
		return NULL;

	// validate parameters
	BOOL bExactPhrase = (nFlags & xtpGridTextSearchExactPhrase) > 0;
	BOOL bMatchCase   = (nFlags & xtpGridTextSearchMatchCase) > 0;
	BOOL bBackward	= (nFlags & xtpGridTextSearchBackward) > 0;
	BOOL bExactStart  = (nFlags & xtpGridTextSearchExactStart) > 0;

	CString sFind(pcszText);
	if (!bMatchCase)
		sFind.MakeUpper();

	CXTPGridRecord* pRec = GetAt(0);
	if (pRec->m_pControl == NULL)
		return NULL;

	int nLastColumn = pRec->m_pControl->GetColumns()->GetCount() - 1;
	int nStep		= bBackward ? -1 : 1;

	_clampIndexes(nStartIndex, 0, GetCount() - 1);
	_clampIndexes(nEndIndex, 0, GetCount() - 1);

	if (nEndIndex < nStartIndex)
		_swapIndexes(nStartIndex, nEndIndex);

	_clampIndexes(nStartColumn, 0, nLastColumn);
	_clampIndexes(nEndColumn, 0, nLastColumn);

	if (nEndColumn < nStartColumn)
		_swapIndexes(nStartColumn, nEndColumn);

	_clampIndexes(nRecordIndex, nStartIndex, nEndIndex);
	_clampIndexes(nItem, nStartColumn, nEndColumn);

	if (bBackward)
	{
		_swapIndexes(nStartIndex, nEndIndex);
		_swapIndexes(nStartColumn, nEndColumn);
	}

	// searching
	for (int i = nRecordIndex; bBackward ? (i >= nEndIndex) : (i <= nEndIndex); i += nStep)
	{
		CXTPGridRecord* pTryRecord = GetAt(i);
		if (pTryRecord == NULL)
			continue;

		for (int j = nStartColumn; bBackward ? (j >= nEndColumn) : (j <= nEndColumn); j += nStep)
		{
			if (i == nRecordIndex && (bBackward ? (j > nItem) : (j < nItem)))
				continue;

			CXTPGridRecordItem* pTryItem = pTryRecord->GetItem(j);
			if (pTryItem == NULL)
				continue;

			CString strCaption = pTryItem->GetCaption(NULL);
			if (strCaption.IsEmpty())
				continue;

			if (!bMatchCase)
				strCaption.MakeUpper();

			if (bExactPhrase) // includes case bExactStart
			{
				if (strCaption == sFind)
					return pTryItem;
				else
					continue;
			}

			int k = strCaption.Find(sFind);

			if (bExactStart)
			{
				if (k == 0)
					return pTryItem;
				else
					continue;
			}

			if (k > -1) // !bExactPhrase && !bExactStart
				return pTryItem;
		}
	}

	return NULL;
}

void CXTPGridRecords::MergeItems(const CXTPGridRecordItemRange& range)
{
	CXTPGridRecord* pMergeRecord   = GetAt(range.m_nRecordFrom);
	CXTPGridRecordItem* pMergeItem = pMergeRecord->GetItem(range.m_nColumnFrom);

	for (int iRecord = range.m_nRecordFrom; iRecord <= range.m_nRecordTo; iRecord++)
	{
		CXTPGridRecord* pRecord = GetAt(iRecord);
		ASSERT(pRecord);

		if (pRecord)
		{
			for (int iColumn = range.m_nColumnFrom; iColumn <= range.m_nColumnTo; iColumn++)
			{
				CXTPGridRecordItem* pItem = pRecord->GetItem(iColumn);
				ASSERT(pItem);

				pItem->Merge(pMergeItem);
			}
		}
	}
}

void CXTPGridRecords::CleanOfRedundant()
{
	if (this->GetCount() < 2)
		return;

	int i			 = 0;
	int* arrLevels   = new int[XTPToUIntChecked(this->GetCount())];
	int nRecordLevel = -1; // -1 means record is ignored
	for (i = 0; i < this->GetCount(); i++)
	{
		nRecordLevel			= -1;
		CXTPGridRecord* pRecord = this->GetAt(i);
		if (pRecord)
		{
			nRecordLevel			   = 0;
			CXTPGridRecord* pParentRec = pRecord->GetParentRecord();
			while (pParentRec)
			{
				pParentRec = pParentRec->GetParentRecord();
				nRecordLevel++;
			}
		}
		arrLevels[i] = nRecordLevel;
	}

	int nMaxLevel = arrLevels[0];
	int nMinLevel = arrLevels[0];
	for (i = 0; i < this->GetCount(); i++)
	{
		if (arrLevels[i] > nMaxLevel)
			nMaxLevel = arrLevels[i];
		else if (arrLevels[i] < nMinLevel)
			nMinLevel = arrLevels[i];
	}

	if (nMaxLevel == nMinLevel) // dropped records are from the same level
	{
		delete[] arrLevels;
		return;
	}

	int nDroppedLevelsCount = nMaxLevel + 1;
	int* arrDroppedLevels   = new int[XTPToUIntChecked(nDroppedLevelsCount)];
	for (i = 0; i < nDroppedLevelsCount; i++)
		arrDroppedLevels[i] = 0;

	for (i = 0; i < this->GetCount(); i++) // count records on different levels
		if (arrLevels[i] > -1)
			arrDroppedLevels[arrLevels[i]] += 1;

	// check: Do records from one level have parents on another level ?
	int nLevelA					= -1;   // level to searching parents.
	int nLevelB					= -1;   // currently checked level. Started from the deepest level.
	CXTPGridRecord* pRecordA	= NULL; // record from level nLevelA
	CXTPGridRecord* pRecordB	= NULL; // record from level nLevelB
	CXTPGridRecord* pParentRecB = NULL; // parent of pRecordB
	BOOL bMarkRecordAsRedundant = FALSE;
	for (nLevelB = nDroppedLevelsCount - 1; nLevelB >= 1; nLevelB--)
	{
		if (arrDroppedLevels[nLevelB] == 0)
			continue;

		for (nLevelA = nLevelB - 1; nLevelA >= 0; nLevelA--)
		{
			if (arrDroppedLevels[nLevelA] == 0)
				continue;

			for (i = 0; i < this->GetCount(); i++)
			{
				if (arrLevels[i] != nLevelB) // takes records only from level nLevelB
					continue;

				pRecordB = this->GetAt(i);
				if (pRecordB == NULL)
					continue;
				bMarkRecordAsRedundant = FALSE;

				// takes record's parent form level nLevelA
				pParentRecB = pRecordB;
				int diff	= nLevelB - nLevelA;
				for (; diff > 0 && pParentRecB; diff--)
					pParentRecB = pParentRecB->GetParentRecord();

				if (pParentRecB == NULL || diff != 0) // pRecordB don't has parent on level nLevelA
					continue;
				// parents comparison
				for (int j = 0; j < this->GetCount(); j++)
				{
					if (arrLevels[j] != nLevelA) // takes records only from level nLevelA
						continue;

					pRecordA = this->GetAt(j);
					if (pRecordA == pParentRecB)
					{
						bMarkRecordAsRedundant = TRUE;
						break; // from parents comparison
					}
				}

				if (bMarkRecordAsRedundant)
					arrLevels[i] = -1;
			}
		}
	}

	for (i = this->GetCount() - 1; i > 0; i--) // delete redundant records
	{
		if (arrLevels[i] != -1)
			continue;

		pRecordA = this->GetAt(i);
		if (pRecordA)
			this->RemoveAt(i);
	}

	delete[] arrLevels;
	delete[] arrDroppedLevels;
}

void CXTPGridRecords::PrepareToDropping(CXTPGridRecord* pTargetRecord,
										CXTPGridRecords* pTargetRecords, BOOL bChangeParent)
{
	if (pTargetRecords == NULL && pTargetRecord == NULL)
		return;

	// change parent of dropped records to allow moving by pTargetRecords->Move(nInsert,
	// pDropRecords)
	BOOL bOmit = FALSE;
	for (int i = 0; i < this->GetCount(); i++)
	{
		bOmit					= FALSE;
		CXTPGridRecord* pRecord = this->GetAt(i);
		if (pRecord && pRecord->m_pRecords != pTargetRecords)
		{
			if (pTargetRecord) // solve recursion between pRecord and pTargetRecord
			{
				CXTPGridRecord* pParentRec = pTargetRecord;
				do
				{
					if (pParentRec == pRecord)
					{
						this->RemoveAt(i);
						--i;
						bOmit = TRUE;
						break; // from do-while
					}
					pParentRec = pParentRec->GetParentRecord();
				} while (pParentRec);

				if (bOmit)
					continue; // in for()
			}

			if (bChangeParent && pTargetRecords)
			{
				if (pRecord->m_pRecords) // remove from previous parent
				{
					pRecord->InternalAddRef();
					pRecord->m_pRecords->RemoveRecord(pRecord);
				}
				pTargetRecords->Add(pRecord);
			}
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridRecords, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridRecords, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecords, "Record", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecords, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecords, "Add", dispidAdd, OleAdd, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecords, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecords, "RemoveAt", 6, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecords, "Insert", 7, OleInsert, VT_DISPATCH, VTS_I4)
	DISP_PROPERTY_ID(CXTPGridRecords, "CaseSensitive", 8, m_bCaseSensitive, VT_BOOL)
	DISP_FUNCTION_ID(CXTPGridRecords, "InsertAt", 9, OleInsertAt, VT_EMPTY, VTS_I4 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridRecords, "FindRecordItem", 10, OleFindRecordItem, VT_DISPATCH,
					 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecords, "MergeItems", 11, OleMergeItems, VTS_NONE,
					 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecords, "DoPropExchange", 41, OleDoPropExchange, VT_EMPTY,
					 VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecords, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecords, XTPDIID_IGridRecords, Dispatch)
	// INTERFACE_PART(CXTPGridRecords, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecords, XTPDIID_IGridRecords)
IMPLEMENT_ENUM_VARIANT(CXTPGridRecords)

int CXTPGridRecords::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPGridRecords::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPGridRecords::OleAdd()
{
	CXTPGridRecord* pRecord = new CXTPGridRecord();
	Add(pRecord);

	return pRecord->GetIDispatch(TRUE);
}

LPDISPATCH CXTPGridRecords::OleInsert(int nIndex)
{
	CXTPGridRecord* pRecord = new CXTPGridRecord();
	InsertAt(nIndex, pRecord);

	return pRecord->GetIDispatch(TRUE);
}

void CXTPGridRecords::OleInsertAt(int nIndex, LPDISPATCH pdispRecord)
{
	CXTPGridRecord* pRecord = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPGridRecord,
														   CXTPGridRecord::FromIDispatchSafe(
															   pdispRecord));
	if (!pRecord)
		AfxThrowOleException(E_INVALIDARG);

	if (nIndex < 0 || nIndex > GetCount())
		AfxThrowOleException(DISP_E_BADINDEX);

	CMDTARGET_ADDREF(pRecord);
	InsertAt(nIndex, pRecord);
}

LPDISPATCH CXTPGridRecords::OleFindRecordItem(int nStartRecord, int nEndRecord, int nStartColumn,
											  int nEndColumn, int nRecord, int nItem,
											  LPCTSTR pcszText, int nFlags)
{
	CXTPGridRecordItem* pItem = FindRecordItem(nStartRecord, nEndRecord, nStartColumn, nEndColumn,
											   nRecord, nItem, pcszText, nFlags);
	if (pItem)
		return pItem->GetIDispatch(TRUE);
	return NULL;
}

void CXTPGridRecords::OleMergeItems(int nRecordFrom, int nRecordTo, int nColumnFrom, int nColumnTo)
{
	MergeItems(CXTPGridRecordItemRange(nColumnFrom, nColumnTo, nRecordFrom, nRecordTo));
}

void CXTPGridRecords::OleDoPropExchange(LPDISPATCH lpPropExchage)
{
	CXTPPropExchangeSection px(PropExchangeFromControl(lpPropExchage));

	if ((CXTPPropExchange*)&px == NULL)
		return;

	DoPropExchange(&px);
}

#endif
