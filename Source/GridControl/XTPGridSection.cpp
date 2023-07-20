// XTPGridSection.cpp : implementation of the CXTPGridSection class.
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

#include "StdAfx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridSelectedRows.h"
#include "GridControl/XTPGridSection.h"
#include "GridControl/XTPGridSections.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridBorder.h"
#include "GridControl/Behavior/XTPGridBehavior.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_GRID_DEBUG() (0)
#define XTP_GRID_PERFORMANCE() (0)

CXTPGridSection::CXTPGridSection(CXTPGridControl* pControl, XTPGridRowType rowType)
	: m_pMarkupContext(NULL)
	, m_pBorder(NULL)

	, m_pControl(pControl)

	, m_pRecords(NULL)
	, m_pRows(NULL)
	, m_pTree(NULL)

	, m_nPopulatedRecordsCount(0)
	, m_rcSection(0, 0, 0, 0)

	, m_dividerPosition(xtpGridSectionDividerPositionBottom)
	, m_dividerStyle(xtpGridFixedRowsDividerNone)
	, m_heightMode(xtpGridSectionHeightModeAuto)
	, m_nHeight(0)

	, m_bAllowAccess(TRUE)
	, m_bAllowEdit(FALSE)
	, m_bAllowGroup(FALSE)
	, m_bAllowSort(FALSE)
	, m_bAllowFilter(FALSE)
	, m_bSelectionEnable(TRUE)
	, m_bVisible(FALSE)

	, m_nScrollIndexV(0)
	, m_nScrollOffsetV(0)

	, m_rowType(rowType)
{
	m_pBorder = new CXTPGridBorder();

	m_pRecords = new CXTPHeapObjectT<CXTPGridRecords, CXTPGridAllocatorDefault>;
	m_pRows	= new CXTPHeapObjectT<CXTPGridRows, CXTPGridAllocatorDefault>;
	m_pTree	= new CXTPHeapObjectT<CXTPGridRows, CXTPGridAllocatorDefault>;

	m_pRecords->m_pControl = pControl;

	m_clrDivider.SetStandardValue(COLOR_BTNFACE);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridSection::~CXTPGridSection()
{
	m_pRecords->SetMarkupContext(NULL);

	SAFE_DELETE(m_pBorder);

	CMDTARGET_RELEASE(m_pRecords);
	CMDTARGET_RELEASE(m_pRows);
	CMDTARGET_RELEASE(m_pTree);
}

void CXTPGridSection::SetMarkupContext(CXTPMarkupContext* pMarkupContext)
{
	m_pMarkupContext = pMarkupContext;
	GetRecords()->SetMarkupContext(pMarkupContext);
}

BOOL CXTPGridSection::IsVirtualMode() const
{
	return m_pRecords ? m_pRecords->IsVirtualMode() : FALSE;
}

CXTPGridRecord* CXTPGridSection::AddRecord(CXTPGridRecord* pRecord)
{
	ASSERT(NULL != pRecord);

	if (NULL != pRecord)
	{
		m_pRecords->Add(pRecord);
	}

	return pRecord;
}

void CXTPGridSection::ResetContent()
{
	if (NULL != m_pRecords)
	{
		m_pRecords->RemoveAll();
	}

	if (NULL != m_pRows)
	{
		m_pRows->Clear();
	}

	if (NULL != m_pTree)
	{
		m_pTree->Clear();
	}
}

void CXTPGridSection::Populate(BOOL bUpdate)
{
	if (bUpdate)
	{
		m_pControl->EditItem(NULL);
		m_pControl->BeginUpdate();
	}

	// Save current focused row
	CXTPGridRow* pFocusedRow	   = m_pRows->GetFocusedRow();
	CXTPGridRecord* pFocusedRecord = NULL;

	if (NULL != pFocusedRow)
	{
		pFocusedRecord = pFocusedRow->GetRecord();
	}

	m_pRows->Clear();
	m_pTree->Clear();

	if (IsVirtualMode())
	{
		CXTPGridRow* pRow = m_pControl->CreateRow(); // new CXTPGridRow_Batch();
		pRow->InitRow(m_pControl, this, GetRecords()->m_pVirtualRecord);
		pRow->SetVisible(TRUE);

		m_pRows->SetVirtualMode(pRow, GetRecords()->GetCount());
		m_nPopulatedRecordsCount = GetRecords()->GetCount();
	}
	else
	{
		BuildTree(m_pTree, NULL, m_pRecords);
		m_nPopulatedRecordsCount = m_pTree->GetCount();

		SortRows(m_pTree);
		int nGroupRowsCount = 0;

		CXTPGridColumns* pColumns = m_pControl->m_pColumns;

		if (IsAllowGroup() && pColumns->GetGroupsOrder()->GetCount() > 0)
		{
			CXTPGridRows* pGroupTree = new CXTPHeapObjectT<CXTPGridRows, CXTPGridAllocatorDefault>;

			int nReserve = m_pRecords->GetCount() / (pColumns->GetGroupsOrder()->GetCount() + 1);
			nReserve	 = max(nReserve, 300);
			pGroupTree->ReserveSize(nReserve);

			CXTPGridGroupRow* pLastGroup = NULL;

			for (int nPlainRow = 0; nPlainRow < m_pTree->GetCount(); nPlainRow++)
			{
				CXTPGridRow* pRow			  = m_pTree->GetAt(nPlainRow);
				CXTPGridGroupRow* pGroupToAdd = NULL;

				for (int nColumn = 0; nColumn < pColumns->GetGroupsOrder()->GetCount(); nColumn++)
				{
					CXTPGridColumn* pColumn   = pColumns->GetGroupsOrder()->GetAt(nColumn);
					CXTPGridRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);
					CString strGroup = pItem ? pItem->GetGroupCaption(pColumn) : CString(_T(""));
					COleVariant vtGroupCaptionValue = pItem ? pItem->GetGroupCaptionValue(pColumn)
															: COleVariant();

					if (pLastGroup
						&& GetRecords()->Compare(pLastGroup->GetCaption(), strGroup) == 0)
					{
						pGroupToAdd = pLastGroup;

						if (pGroupToAdd->HasChildren())
							pLastGroup = (CXTPGridGroupRow*)pGroupToAdd->GetChilds()->GetAt(
								pGroupToAdd->GetChilds()->GetCount() - 1);
					}
					else
					{
						CXTPGridGroupRow* pGroup =
							m_pControl->CreateGroupRow(); // new CXTPGridGroupRow_Batch();
						nGroupRowsCount++;

						pGroup->InitRow(m_pControl, this, NULL);
						pGroup->SetCaption(strGroup, vtGroupCaptionValue);

						if (pGroupToAdd)
							pGroupToAdd->AddChild(pGroup);
						else
							pGroupTree->Add(pGroup);

						pGroupToAdd = pGroup;
						pLastGroup  = NULL;
					}
				}
				if (pGroupToAdd)
					pGroupToAdd->AddChild(pRow);
				pRow->InternalAddRef();
				pLastGroup = (CXTPGridGroupRow*)pGroupTree->GetAt(pGroupTree->GetCount() - 1);
			}

			m_pTree->InternalRelease();
			m_pTree = pGroupTree;
		}

		GetRows()->ReserveSize(m_pRecords->GetCount() + nGroupRowsCount + 10);

		for (int nGroupRow = 0; nGroupRow < m_pTree->GetCount(); nGroupRow++)
		{
			CXTPGridRow* pRow = m_pTree->GetAt(nGroupRow);
			InsertRow(GetRows()->GetCount(), pRow);
			pRow->m_nChildIndex = nGroupRow;
			ASSERT(pRow->m_pParentRows == m_pTree);
		}

		for (int nRow = 0; nRow < m_pRows->GetCount(); nRow++)
		{
			CXTPGridRow* pRow = m_pRows->GetAt(nRow);

			// Update indexes
			if (NULL != pRow)
			{
				pRow->SetIndex(nRow);
				pRow->m_pSection = this;
				pRow->SetVisible(TRUE);
			}

			// Restore focus
			if (pFocusedRecord && pRow->GetRecord() == pFocusedRecord)
			{
				m_pRows->m_nFocusedRow = pRow->GetIndex();
				if (IsSelectionEnabled() && m_pControl->IsInitialSelectionEnabled())
				{
					// m_pSelectedRows->Select(pRow);
					CXTPGridRow* pFirst = pRow;
					if (pFirst)
					{
						if (!m_pControl->IsSkipGroupsFocusEnabled() || !pFirst->IsGroupRow())
						{
							m_pControl->m_pSelectedRows->Select(pFirst);
						}
						else
						{
							while (pFirst->IsGroupRow())
							{
								if (pFirst->HasChildren())
								{
									pFirst = pFirst->GetChilds()->GetAt(0);
								}
							}
							m_pControl->m_pSelectedRows->Select(pFirst);
						}
					}
				}
			}
		}

	} // VirtualMode()

	if (bUpdate)
	{
		m_pControl->AdjustIndentation();
		m_pControl->AdjustLayout();
		m_pControl->AdjustScrollBars();
		m_pControl->UpdateSubList();
		m_pControl->EndUpdate();
	}
}

int CXTPGridSection::InsertRow(int nIndex, CXTPGridRow* pRow)
{
	m_pRows->InsertAt(nIndex, pRow);
	pRow->InternalAddRef();
	pRow->m_pSection = this;
	pRow->SetVisible(TRUE);

	int nRowsInserted = 1;

	if (pRow->IsExpanded() && pRow->HasChildren())
	{
		nRowsInserted += _DoExpand(nIndex, pRow, FALSE);
	}

	return nRowsInserted;
}

int CXTPGridSection::_DoExpand(int nIndex, CXTPGridRow* pRow)
{
	return _DoExpand(nIndex, pRow, TRUE);
}

int CXTPGridSection::_DoExpand(int nIndex, CXTPGridRow* pRow, BOOL bUpdateSelection)
{
	int nCount		= 0;
	int nIndexStart = nIndex;

	if (pRow->HasChildren())
	{
		for (int i = 0; i < pRow->GetChilds()->GetCount(); i++)
		{
			CXTPGridRow* pRowChild = pRow->GetChilds()->GetAt(i);
			if (pRowChild)
			{
				pRowChild->m_nRowLevel   = pRow->m_nRowLevel + 1;
				pRowChild->m_nGroupLevel = pRow->m_nGroupLevel + (pRow->IsGroupRow() ? 1 : 0);

				nIndex += InsertRow(nIndex + 1, pRowChild);
			}
		}

		nCount = nIndex - nIndexStart;
	}

	if (nCount > 0 && bUpdateSelection)
	{
		GetSelectedRows()->_OnExpanded(nIndexStart, nCount);
	}

	if (m_pRows->m_nFocusedRow > nIndex)
	{
		m_pRows->m_nFocusedRow += nCount;
	}

	return nCount;
}

int CXTPGridSection::_DoCollapse(CXTPGridRow* pRow)
{
	int nIndex = pRow->GetIndex() + 1;
	if (nIndex == 0) // pRow->GetIndex() == -1
		return 0;	// pRow already collapsed

	int nCount = 0;

	while (nIndex < m_pRows->GetCount())
	{
		CXTPGridRow* pRowChild = m_pRows->GetAt(nIndex);

		if (pRowChild)
		{
			if (!pRowChild->HasParent(pRow))
				break;

			pRowChild->SetVisible(FALSE);
			pRowChild->m_nIndex = -1;
			m_pRows->RemoveAt(nIndex);
			nCount++;
		}
	}

	if (nCount > 0)
	{
		GetSelectedRows()->_OnCollapsed(pRow->GetIndex(), nCount);
	}

	if (nCount > 0)
	{
		if (m_pRows->m_nFocusedRow >= nIndex)
		{
			m_pRows->m_nFocusedRow = max(nIndex - 1, m_pRows->m_nFocusedRow - nCount);
		}
	}

	return nCount;
}

int CXTPGridSection::GetPageRowCount(CDC* pDC) const
{
	int nCount = 0;
	m_pRows->ClearMergeHeight();
	int nHeight = m_rcSection.Height();

	for (int nRow = m_nScrollIndexV; nRow < m_pRows->GetCount(); nRow++)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(nRow);

		int nRowHeight = pRow->GetHeight(pDC, m_rcSection.Width());

		if (nHeight < nRowHeight)
		{
			break;
		}
		else
		{
			nHeight -= nRowHeight;
			nCount++;
		}
	}

	return nCount;
}

CXTPGridBorder* CXTPGridSection::GetBorder() const
{
	return m_pBorder;
}

int CXTPGridSection::GetTotalHeight(CDC* pDC) const
{
	CSize szBorder = GetBorder()->GetSize();

	return szBorder.cy + GetRowsHeight(pDC);
}

int CXTPGridSection::GetRowsHeight(CDC* pDC) const
{
#if XTP_GRID_PERFORMANCE()
	DWORD dwStart = ::GetTickCount();
#endif

	int nRowsHeight = 0;
	m_pRows->ClearMergeHeight();
	for (int i = 0; i < m_pRows->GetCount(); ++i)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(i);

		nRowsHeight += pRow->GetHeight(pDC, m_rcSection.Width());
	}

#if XTP_GRID_PERFORMANCE()
	DWORD dwEnd = ::GetTickCount();
	TRACE(_T("GetRowsHeight: %d\n"), dwEnd - dwStart);
#endif

	return nRowsHeight;
}

BOOL CXTPGridSection::GetRowAt(CDC* pDC, int nWidth, int nOffset, int& nIndex, int& nTop)
{
#if XTP_GRID_PERFORMANCE()
	DWORD dwStart = ::GetTickCount();
#endif

	int nHeight = 0;
	nTop		= 0;
	m_pRows->ClearMergeHeight();

	for (nIndex = 0; nIndex < m_pRows->GetCount(); ++nIndex)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(nIndex);

		if (pRow->GetRect().Width() == nWidth)
		{
			nHeight = pRow->GetRect().Height();
		}
		else
		{
			nHeight = pRow->GetHeight(pDC, nWidth);
		}

		if (nOffset >= nTop && nOffset <= nTop + nHeight)
		{
#if XTP_GRID_PERFORMANCE()
			DWORD dwEnd = ::GetTickCount();
			TRACE(_T("GetRowAt: %d\n"), dwEnd - dwStart);
#endif
			return TRUE;
		}

		nTop += nHeight;
	}

	return FALSE;
}

void CXTPGridSection::Draw(CDC* pDC)
{
#if XTP_GRID_PERFORMANCE()
	DWORD dwStart = ::GetTickCount();
#endif

	CRect rcSection(m_rcSection);

	// Draw border
	if (NULL != m_pBorder)
	{
		m_pBorder->SetBorderColor(m_pControl->GetPaintManager()->GetGridColor());
		m_pBorder->Draw(pDC, rcSection);

		// Deduct margin, border and padding from section rectangle
		rcSection = CXTPGridBorder::DeflateRect(rcSection, m_pBorder);
	}

	int nIndex  = m_nScrollIndexV;
	int nOffset = 0;

	if (m_pControl->m_scrollModeV == xtpGridScrollModeSmooth
		&& m_pControl->m_pSectionScroll == this)
	{
		int nTop;

		if (GetRowAt(pDC, rcSection.Width(), m_nScrollOffsetV, nIndex, nTop))
		{
			m_nScrollIndexV = nIndex;
			nOffset			= m_nScrollOffsetV - nTop;
		}
	}

	m_pControl->DrawRows(pDC, rcSection, rcSection.top - nOffset, m_pRows, nIndex, 0,
						 m_pControl->GetColumns()->GetCount());

	if (xtpGridSectionDividerStyleNone != m_dividerStyle)
	{
		m_pControl->GetPaintManager()->DrawSectionDivider(pDC, m_rcDivider, m_dividerPosition,
														  m_dividerStyle, m_clrDivider);
	}

#if XTP_GRID_PERFORMANCE()
	DWORD dwEnd = ::GetTickCount();
	TRACE(_T("CXTPGridSection::Draw: %d\n"), dwEnd - dwStart);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Internal methods
/////////////////////////////////////////////////////////////////////////////

void CXTPGridSection::BuildTree(CXTPGridRows* pTree, CXTPGridRow* pParentRow,
								CXTPGridRecords* pRecords)
{
	ASSERT(pTree->GetCount() == 0);

	pTree->ReserveSize(pRecords->GetCount());

	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		CXTPGridRecord* pRecord = pRecords->GetAt(i);

		if (pRecord == NULL)
			continue;

		BOOL bFilter = FALSE;

		if (IsAllowFilter())
		{
			bFilter = m_pControl->ApplyFilter(pRecord, m_pControl->GetFilterText(),
											  m_pControl->IsPreviewMode());
		}

		// add record if all conditions are met
		if (pRecord->IsLocked() || (pRecord->IsVisible() && !bFilter))
		{
			CXTPGridRow* pRow = m_pControl->CreateRow(); // new CXTPGridRow_Batch();
			pRow->InitRow(m_pControl, this, pRecord);

			pRow->m_pParentRow = pParentRow;
			pTree->Add(pRow);

			if (pRecord->HasChildren())
			{
				BuildTree(pRow->GetChilds(), pRow, pRecord->GetChilds());

				if (pRow->GetChilds() && m_pControl->IsSortRecordChilds())
				{
					SortRows(pRow->GetChilds());

					pRow->GetChilds()->RefreshChildIndices(FALSE);
				}
			}
		}
	}
}

void CXTPGridSection::SortTree(CXTPGridRows* pTree)
{
	ASSERT(NULL != pTree);

	if (NULL == pTree)
	{
		return;
	}

	SortRows(pTree);

	for (int nRow = 0; nRow < pTree->GetCount(); nRow++)
	{
		CXTPGridRow* pRow = pTree->GetAt(nRow);
		ASSERT(NULL != pRow);

		if (NULL != pRow)
		{
			CXTPGridRecord* pRecord = pRow->GetRecord();

			BOOL bRecordHasChildren  = NULL != pRecord ? pRecord->HasChildren() : FALSE;
			BOOL bRecordSortChildren = m_pControl->m_bSortRecordChilds;

			if (pRow->HasChildren()
				&& (bRecordHasChildren && bRecordSortChildren || !bRecordHasChildren))
			{
				SortTree(pRow->GetChilds());
			}
		}
	}

	pTree->RefreshChildIndices(FALSE);
}

void CXTPGridSection::RefreshIndexes()
{
	RefreshIndexes(0);
}

void CXTPGridSection::OnDeletedRefreshIndexes()
{
	int nRowCount = m_pRows->GetCount();

	for (int nIndex = 0; nIndex < nRowCount; nIndex++)
	{
		CXTPGridRow* pRow = NULL;

		if (m_pRows->m_pVirtualRow)
		{
			pRow		   = m_pRows->m_pVirtualRow;
			pRow->m_nIndex = nIndex;
		}
		else
		{
			// Direct access to increase performance
			pRow = m_pRows->m_arrRows.m_pData[nIndex];
		}

		if (NULL != pRow)
		{
			BOOL bSelected = GetSelectedRows()->Contains(pRow);
			if (bSelected)
			{
				GetSelectedRows()->Remove(pRow);
			}

			pRow->m_nIndex = nIndex;
			ASSERT(pRow->IsVisible());

			if (bSelected)
			{
				GetSelectedRows()->Add(pRow);
			}
		}
	}
}

void CXTPGridSection::RefreshIndexes(int nIndexStart)
{
	int nRowCount = m_pRows->GetCount();
	nIndexStart   = nIndexStart < 0 ? 0 : nIndexStart;

	for (int nIndex = nIndexStart; nIndex < nRowCount; nIndex++)
	{
		CXTPGridRow* pRow = NULL;

		if (m_pRows->m_pVirtualRow)
		{
			pRow		   = m_pRows->m_pVirtualRow;
			pRow->m_nIndex = nIndex;
		}
		else
		{
			// Direct access to increase performance
			pRow = m_pRows->m_arrRows.m_pData[nIndex];
		}

		if (NULL != pRow)
		{
			pRow->m_nIndex = nIndex;
			ASSERT(pRow->IsVisible());
		}
	}
}

void CXTPGridSection::ReSortRows()
{
	if (IsVirtualMode())
	{
		Populate(TRUE);
		return;
	}

	// Are there any selected rows?
	BOOL bRowsSelected = m_pControl->m_bInitialSelectionEnable;
	if (GetSelectedRows() != NULL)
	{
		if (GetSelectedRows()->GetCount() > 0)
			bRowsSelected = TRUE;
	}

	CXTPGridRecord* pFocusedRecord = NULL;
	if (NULL != GetFocusedRow())
	{
		pFocusedRecord = GetFocusedRow()->GetRecord();
	}

	SortTree(m_pTree);
	m_pRows->Clear();

	for (int nGroupRow = 0; nGroupRow < m_pTree->GetCount(); nGroupRow++)
	{
		CXTPGridRow* pRow = m_pTree->GetAt(nGroupRow);
		InsertRow(m_pRows->GetCount(), pRow);
		pRow->m_nChildIndex = nGroupRow;
		ASSERT(pRow->m_pParentRows == m_pTree);
	}

	m_pRows->m_nFocusedRow = -1;

	// Update indexes on virtual rows
	int nRowCount = m_pRows->GetCount();

	for (int nRow = 0; nRow < nRowCount; nRow++)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(nRow);
		if (NULL != pRow)
		{
			pRow->SetIndex(nRow);
			pRow->SetVisible(TRUE);
			CXTPGridRecord* pRecord = pRow->GetRecord();
			if (NULL != pRecord)
			{
				if (pFocusedRecord && pRecord == pFocusedRecord)
				{
					m_pRows->m_nFocusedRow = pRow->GetIndex();
					if (IsSelectionEnabled() && bRowsSelected)
					{
						GetSelectedRows()->Select(pRow);
					}
				}
				BOOL b1 = pRow->IsSelected();
				BOOL b2 = pRecord->IsSelected();
				if (b1 != b2)
					pRow->SetSelected(b2);
			}
		}
	}
}

void CXTPGridSection::SortRows(CXTPGridRows* pRows)
{
	ASSERT(pRows);

	CXTPGridColumns* pColumns				= m_pControl->m_pColumns;
	XTPGridRowsCompareFunc pRowsCompareFunc = m_pControl->m_pRowsCompareFunc;

	if (pRows == NULL || 0 == pRows->GetCount() || !IsAllowSort())
		return;

	if (0 == pColumns->GetSortOrder()->GetCount() && 0 == pColumns->GetGroupsOrder()->GetCount())
	{
		return;
	}

	CXTPGridRow* pRow;
	CXTPGridRecord* pRecord;

	if (NULL != pRowsCompareFunc)
	{
		pRows->SortEx(pRowsCompareFunc);
	}
	else
	{
		pRows->Sort();
	}

	m_UaSorted.RemoveAll();
	for (int nRow = 0; nRow < pRows->GetCount(); nRow++)
	{
		pRow = pRows->GetAt(nRow);
		if (pRow)
		{
			pRecord = pRow->GetRecord();
			if (NULL != pRecord)
			{
				m_UaSorted.Add(XTPToUIntChecked(pRecord->GetIndex()));
			}
		}
	}
}

BOOL CXTPGridSection::EnsureVisible(CDC* pDC, CXTPGridRow* pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);
	ASSERT(pRow->IsVisible());			// Row must be visible
	ASSERT(this == pRow->GetSection()); // Row must be in this section

	if (NULL == pDC || NULL == pRow || !pRow->IsVisible() || this != pRow->GetSection())
	{
		return FALSE;
	}

	BOOL bResult = FALSE;

	if (m_pControl->IsIconView())
	{
		bResult = EnsureVisibleIcon(pDC, pRow);
	}
	else
	{
		switch (m_pControl->GetScrollMode(xtpGridOrientationVertical))
		{
			case xtpGridScrollModeSmooth: bResult = EnsureVisibleSmooth(pDC, pRow); break;

			case xtpGridScrollModeBlockCount:
			case xtpGridScrollModeBlockSize: bResult = EnsureVisibleBlock(pDC, pRow); break;
		}
	}

	return bResult;
}

int CXTPGridSection::GetRowOffset(CDC* pDC, int nWidth, int nIndex)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);

	if (NULL == pDC)
	{
		return -1;
	}

	int nOffset = 0;
	GetRows()->ClearMergeHeight();

	for (int nRow = 0; nRow < nIndex; nRow++)
	{
		CXTPGridRow* pRow = GetRows()->GetAt(nRow);

		nOffset += pRow->GetHeight(pDC, nWidth);
	}

	return nOffset;
}

BOOL CXTPGridSection::EnsureVisibleSmooth(CDC* pDC, CXTPGridRow* pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	if (NULL == pDC || NULL == pRow)
	{
		return FALSE;
	}

	int nOffset = GetRowOffset(pDC, GetRect().Width(), pRow->GetIndex());

	// Scroll up
	if (nOffset < m_nScrollOffsetV)
	{
		m_pControl->SetScrollOffsetV(nOffset);
	}

	// Scroll down
	int nRowHeight = pRow->GetHeight(pDC, GetRect().Width());
	nOffset += nRowHeight;

	if (nOffset > m_nScrollOffsetV + GetRect().Height())
	{
		m_pControl->SetScrollOffsetV(nOffset - GetRect().Height());
	}

	return TRUE;
}

int CXTPGridSection::EnsureVisibleCalc(CDC* pDC, CXTPGridRow* pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	if (NULL == pDC || NULL == pRow)
		return -1;

	int nIndexRow = pRow->GetIndex();
	int nIndexTop = m_nScrollIndexV;
	int nReturn   = -1;

	if (nIndexRow <= nIndexTop)
	{
		// Scroll up
		nReturn = nIndexRow;
	}
	else if (nIndexRow > nIndexTop)
	{
		// Scroll down
		LONG top   = m_rcSection.top;
		int nCount = GetRows()->GetCount();

		int nIndex = nIndexTop;
		GetRows()->ClearMergeHeight();

		// Figure out if row is fully visible
		while ((top < m_rcSection.bottom) && (nIndex <= nIndexRow) && (nIndex < nCount))
		{
			CXTPGridRow* pCurrentRow = GetRows()->GetAt(nIndex);
			ASSERT(NULL != pCurrentRow);
			if (pCurrentRow == NULL)
				continue;

			int nRowHeight = pCurrentRow->GetHeight(pDC, m_rcSection.Width());

			nIndex++; // Move to next row

			top += nRowHeight;
		}

		// Row not fully visible
		if (top >= m_rcSection.bottom)
		{
			int nHeight = m_rcSection.Height();

			for (nIndex = nIndexRow; nIndex >= 0; nIndex--)
			{
				CXTPGridRow* pCurrentRow = m_pRows->GetAt(nIndex);
				ASSERT(NULL != pCurrentRow);
				if (pCurrentRow == NULL)
					continue;

				int nRowHeight = pCurrentRow->GetHeight(pDC, m_rcSection.Width());

				if (nHeight - nRowHeight < 0)
					break;

				nHeight -= nRowHeight;
			}

			nReturn = nIndex + 1;
		}
	}

	return nReturn;
}

BOOL CXTPGridSection::EnsureVisibleBlock(CDC* pDC, CXTPGridRow* pRow)
{
	int nIndex = EnsureVisibleCalc(pDC, pRow);

	if (-1 == nIndex)
		return FALSE;

	SetScrollIndexV(nIndex);

	return TRUE;
}

BOOL CXTPGridSection::EnsureVisibleIcon(CDC* pDC, CXTPGridRow* pRow)
{
	int nIndex = EnsureVisibleCalc(pDC, pRow);

	if (-1 == nIndex)
		return FALSE;

	int nIndexRow = pRow->GetIndex();

	if (nIndex != nIndexRow)
		nIndex += m_pControl->GetRowsPerLine();

	m_pControl->SetTopRow(nIndex);

	return TRUE;
}

int CXTPGridSection::GetScrollOffsetV() const
{
	return m_nScrollOffsetV;
}

void CXTPGridSection::SetScrollOffsetV(int nScrollOffsetV)
{
	m_nScrollOffsetV = nScrollOffsetV;
	m_nScrollIndexV  = -1; // Recalculate
}

XTPGridRowType CXTPGridSection::GetRowType() const
{
	return m_rowType;
}

CXTPGridSelectedRows* CXTPGridSection::GetSelectedRows() const
{
	return m_pControl->GetSelectedRows();
}

CXTPGridRow* CXTPGridSection::GetFocusedRow() const
{
	return GetRows()->GetFocusedRow();
}

int CXTPGridSection::GetIndex() const
{
	int nIndex = -1;

	const CXTPGridSections* pSections = m_pControl->GetSections();

	for (int nSection = 0; nSection < pSections->GetSize(); nSection++)
	{
		if (this == pSections->GetAt(nSection))
		{
			nIndex = nSection;
		}
	}

	return nIndex;
}

void CXTPGridSection::AddRecordEx(CXTPGridRecord* pRecord, CXTPGridRecord* pParentRecord,
								  int nRowChildIndex, int nRecordChildIndex)
{
	ASSERT(pRecord);
	if (!pRecord)
		return;

	// add record
	BOOL bAddRecord					= TRUE;
	CXTPGridRecords* pParentRecords = pParentRecord ? pParentRecord->HasChildren()
														  ? pParentRecord->GetChilds()
														  : NULL
													: GetRecords();
	if (pParentRecords)
	{
		for (int nChild = 0; nChild < pParentRecords->GetCount(); nChild++)
		{
			if (pRecord == pParentRecords->GetAt(nChild))
			{
				bAddRecord = FALSE;
				break;
			}
		}
	}
	if (bAddRecord)
	{
		CXTPGridRecords* pRecords = pParentRecord ? pParentRecord->GetChilds() : GetRecords();

		if (nRecordChildIndex >= 0 && nRecordChildIndex < pRecords->GetCount())
			pRecords->InsertAt(nRecordChildIndex, pRecord);
		else
			pRecords->Add(pRecord);
	}

	if (!pRecord->IsVisible())
		return;

	// find parent record rows
	CXTPGridRows* pParentRows = m_pTree;
	CXTPGridRow* pParentRow   = NULL;
	if (pParentRecord)
	{
		pParentRow = m_pTree->FindInTree(pParentRecord);
		if (pParentRow)
			pParentRows = pParentRow->GetChilds();
	}
	BOOL bRoot = pParentRows == m_pTree;

	// create new row
	CXTPGridRow* pNewRow = m_pControl->CreateRow();
	pNewRow->InitRow(m_pControl, this, pRecord);
	pNewRow->m_pParentRow = NULL;

	// add row to pParentRows
	BOOL bInsertAfter		   = FALSE;
	CXTPGridRow* pPlainTreeRow = NULL;
	CXTPGridRow* pInsertRowPos = NULL;
	int nNextSiblingIndex	  = m_pRows->GetCount();
	if (bRoot)
	{
		pInsertRowPos	 = pParentRows->FindInsertionPos(pNewRow, bInsertAfter);
		CXTPGridRow* pRow = pInsertRowPos;
		while (pRow)
		{
			if (pRow->GetNextSiblingRow())
			{
				nNextSiblingIndex = pRow->GetNextSiblingRow()->GetIndex();
				break;
			}
			else
				pRow = pRow->GetParentRow();
		}

		CXTPGridColumns* pColumns = m_pControl->GetColumns();

		if (pColumns->GetGroupsOrder()->GetCount() > 0)
		{
			CXTPGridGroupRow* pGroupToAdd = pInsertRowPos
												? (CXTPGridGroupRow*)pInsertRowPos->GetParentRow()
												: NULL;
			if (!pInsertRowPos || pInsertRowPos->IsGroupRow())
			{
				for (int nColumn = pInsertRowPos ? pInsertRowPos->GetGroupLevel() : 0;
					 nColumn < pColumns->GetGroupsOrder()->GetCount(); nColumn++)
				{
					CXTPGridColumn* pColumn   = pColumns->GetGroupsOrder()->GetAt(nColumn);
					CXTPGridRecordItem* pItem = pRecord->GetItem(pColumn);
					CString strGroup		  = pItem ? pItem->GetGroupCaption(pColumn) : CString();
					COleVariant vtGroupCaptionValue = pItem ? pItem->GetGroupCaptionValue(pColumn)
															: COleVariant();

					CXTPGridGroupRow* pGroup = m_pControl->CreateGroupRow();
					pGroup->InitRow(m_pControl, this, NULL);
					pGroup->SetCaption(strGroup, vtGroupCaptionValue);
					pGroup->m_nGroupLevel = pGroupToAdd ? pGroupToAdd->m_nRowLevel + 1 : 0;
					pGroup->m_nRowLevel   = nColumn;
					if (pGroupToAdd)
					{
						if (!pPlainTreeRow)
						{
							pPlainTreeRow = pGroup;
							pGroupToAdd->GetChilds()->InsertAt(!pInsertRowPos
																   ? 0
																   : (pInsertRowPos->m_nChildIndex
																	  + (bInsertAfter ? 1 : 0)),
															   pGroup);
						}
						else
							pGroupToAdd->AddChild(pGroup);
						pGroup->m_pParentRows = pGroupToAdd->GetChilds();
						pGroup->m_pParentRow  = pGroupToAdd;
						pGroup->SetVisible(pGroupToAdd->IsExpanded());
					}
					else
					{
						pPlainTreeRow = pGroup;
						pParentRows->InsertAt(pInsertRowPos ? pInsertRowPos->m_nChildIndex
															: pParentRows->GetCount(),
											  pGroup);
						pGroup->m_pParentRows = pParentRows;
						pGroup->m_pParentRow  = NULL;
						pGroup->SetVisible(TRUE);
					}
					pGroupToAdd = pGroup;
				}
				// insert row
				if (pGroupToAdd)
				{
					pGroupToAdd->AddChild(pNewRow);
					pNewRow->m_pParentRows = pGroupToAdd->GetChilds();
					//              pNewRow->m_pParentRow = pGroupToAdd;
					pNewRow->SetVisible(pGroupToAdd->IsExpanded());
				}
			}
			else
			{
				// insert row
				if (pGroupToAdd)
				{
					pGroupToAdd->GetChilds()->InsertAt(!pInsertRowPos
														   ? 0
														   : (pInsertRowPos->m_nChildIndex
															  + (bInsertAfter ? 1 : 0)),
													   pNewRow);
					pNewRow->m_pParentRows = pGroupToAdd->GetChilds();
					pNewRow->m_pParentRow  = pGroupToAdd;
					pNewRow->SetVisible(pGroupToAdd->IsExpanded());
				}
			}
		}
		else
		{
			pParentRows->InsertAt(pInsertRowPos ? pInsertRowPos->m_nChildIndex
												: pParentRows->GetCount(),
								  pNewRow);
			pNewRow->m_pParentRows = pParentRows;
			pNewRow->m_pParentRow  = NULL;
			pNewRow->SetVisible(TRUE);
		}
	}
	else
	{
		if (nRowChildIndex >= 0 && nRowChildIndex < pParentRows->GetCount())
			pParentRows->InsertAt(nRowChildIndex, pNewRow);
		else
			pParentRows->Add(pNewRow);
		pNewRow->m_pParentRows = pParentRows;
		pNewRow->m_pParentRow  = pParentRow;
		pNewRow->SetVisible(TRUE);
		pNewRow->m_nRowLevel   = pParentRow->m_nRowLevel + 1;
		pNewRow->m_nGroupLevel = pParentRow->m_nGroupLevel;
		pNewRow->m_nChildIndex = nRowChildIndex >= 0 ? nRowChildIndex : pParentRows->GetCount() - 1;

		CXTPGridRow* pRow = pNewRow;
		while (pRow)
		{
			if (pRow->GetNextSiblingRow())
			{
				nNextSiblingIndex = pRow->GetNextSiblingRow()->GetIndex();
				break;
			}
			else
				pRow = pRow->GetParentRow();
		}
	}
	// refresh child indices
	if (pInsertRowPos && pInsertRowPos->GetParentRows())
		pInsertRowPos->GetParentRows()->RefreshChildIndices();
	else
		pParentRows->RefreshChildIndices();

	// add row to m_pRows
	if (bRoot)
	{
		if (pInsertRowPos)
		{
			CXTPGridRow* pRow = pInsertRowPos->GetParentRow();
			BOOL bIsExpanded  = TRUE;
			while (pRow && bIsExpanded)
			{
				bIsExpanded = bIsExpanded && pRow->IsExpanded();
				pRow		= pRow->GetParentRow();
			}

			if (bIsExpanded)
			{
				if (bInsertAfter)
					InsertRow(nNextSiblingIndex, pPlainTreeRow ? pPlainTreeRow : pNewRow);
				else
					InsertRow(pInsertRowPos->GetIndex(), pPlainTreeRow ? pPlainTreeRow : pNewRow);
			}
		}
		else
		{
			InsertRow(m_pRows->GetCount(), pPlainTreeRow ? pPlainTreeRow : pNewRow);
		}
	}
	else
	{
		CXTPGridRow* pRow = pNewRow->GetParentRow();
		BOOL bIsExpanded  = TRUE;
		while (pRow && bIsExpanded)
		{
			bIsExpanded = bIsExpanded && pRow->IsExpanded();
			pRow		= pRow->GetParentRow();
		}

		if (bIsExpanded)
			InsertRow(nNextSiblingIndex, pNewRow);
	}

	RefreshIndexes();

	// add children
	if (pRecord && pRecord->HasChildren())
	{
		for (int nChild = 0; nChild < pRecord->GetChilds()->GetCount(); nChild++)
		{
			AddRecordEx(pRecord->GetChilds()->GetAt(nChild), pRecord);
		}
	}
}

void CXTPGridSection::UpdateRecord(CXTPGridRecord* pRecord, BOOL bUpdateChildren)
{
	// Get parent record
	CXTPGridRecord* pParentRecord = NULL;
	if (pRecord->GetRecords())
	{
		pParentRecord = pRecord->GetRecords()->GetOwnerRecord();
	}

	// Update record
	if (NULL == pParentRecord || bUpdateChildren)
	{
		// Internal AddRef
		pRecord->TreeAddRef();

		// Get record row child index
		CXTPGridRow* pRow = m_pTree->FindInTree(pRecord);

		if (NULL != pRow)
		{
			int nRowChildIndex	= pRow->m_nChildIndex;
			int nRecordChildIndex = pRecord->GetIndex();

			// Remove record
			RemoveRecordEx(pRecord, FALSE);

			// Add record
			pRecord->InternalAddRef(); // AddRecordEx expects that pRecord has +1 ref count.
			AddRecordEx(pRecord, pParentRecord, nRowChildIndex, nRecordChildIndex);

			// Internal release
			pRecord->TreeRelease();
		}
	}

	m_pControl->RedrawControl();
}

BOOL CXTPGridSection::RemoveRecordEx(CXTPGridRecord* pRecord, BOOL bAdjustLayout,
									 BOOL bRemoveFromParent)
{
	CXTPGridRecords* pRecords = GetRecords();

	ASSERT(pRecord && pRecords);

	if (!pRecord || !pRecords)
		return FALSE;

	if (pRecord->HasChildren())
	{
		for (int i = pRecord->GetChilds()->GetCount() - 1; i >= 0; i--)
		{
			RemoveRecordEx(pRecord->GetChilds()->GetAt(i), bAdjustLayout, FALSE);
		}
	}

	BOOL bResult = FALSE;

	//--------------------------------------------------------
	CXTPGridRow* pRow0 = m_pTree->FindInTree(pRecord);

	if (NULL != pRow0)
	{
		CXTPGridRow* pRow = pRow0;
		pRow0			  = NULL;

		do
		{
			CXTPGridRow* pRow_parent = pRow->GetParentRow();

			ASSERT(pRow->GetParentRows());

			// 1. Remove from selected rows
			if (GetSelectedRows()->Contains(pRow))
			{
				GetSelectedRows()->Remove(pRow);
			}

			// 2. remove from Rows Tree
			if (pRow->GetParentRows())
			{
				VERIFY(pRow->GetParentRows()->RemoveRow(pRow) >= 0);
			}

			// 3. remove from Display Rows array
			m_pRows->RemoveRow(pRow);

			pRow = pRow_parent;

		} while (pRow && pRow->IsGroupRow() && pRow->GetChilds()->GetCount() == 0);

		// refresh child indices
		if (pRow && pRow->HasChildren())
			pRow->GetChilds()->RefreshChildIndices();
		else
			m_pTree->RefreshChildIndices();

		bResult = TRUE;
	}

	//-------------------------------------------------------
	// pRecord->Delete(); // the code below is more safe when record already removed!

	ASSERT(pRecord->GetRecords());

	if (bRemoveFromParent && pRecord->GetRecords())
	{
		BOOL bRecordRem = pRecord->GetRecords()->RemoveRecord(pRecord) >= 0;
		pRecord			= NULL;

		bResult |= bRecordRem;
	}

	if (m_pRows->m_nFocusedRow >= 0 && m_pRows->m_nFocusedRow >= m_pRows->GetCount())
		m_pRows->m_nFocusedRow = m_pRows->GetCount() - 1;

	//-------------------------------------------------------
	if (bResult)
	{
		// RefreshIndexes();
		OnDeletedRefreshIndexes();

		if (bAdjustLayout)
		{
			m_pControl->AdjustScrollBars();
		}
	}

	return bResult;
}

BOOL CXTPGridSection::RemoveRowEx(CXTPGridRow* pRow, BOOL bAdjustLayout)
{
	ASSERT(NULL != pRow);
	if (NULL == pRow)
	{
		return FALSE;
	}

	if (!pRow->IsGroupRow())
	{
		ASSERT(pRow->GetRecord());
		return RemoveRecordEx(pRow->GetRecord(), bAdjustLayout);
	}
	else
	{
		int nCount = pRow->GetChilds()->GetCount();

		for (int i = nCount - 1; i >= 0; i--)
		{
			CXTPGridRow* pRowI = pRow->GetChilds()->GetAt(i);
			RemoveRowEx(pRowI, bAdjustLayout);
		}
		return TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Scroll index
/////////////////////////////////////////////////////////////////////////////

int CXTPGridSection::GetScrollIndexV()
{
	if (-1 == m_nScrollIndexV)
	{
		switch (GetScrollModeV())
		{
			case xtpGridScrollModeSmooth:
			{
				int nIndex = 0;
				int nTop   = 0;

				CRect rcSection(m_rcSection);

				// Draw border
				if (NULL != m_pBorder)
				{
					// Deduct margin, border and padding from section rectangle
					rcSection = CXTPGridBorder::DeflateRect(rcSection, m_pBorder);
				}

				CClientDC dc(m_pControl);
				if (GetRowAt(&dc, rcSection.Width(), m_nScrollOffsetV, nIndex, nTop))
				{
					m_nScrollIndexV = nIndex;
				}
			}
			break;

			default: m_nScrollIndexV = 0; break;
		}
	}

	return m_nScrollIndexV;
}

void CXTPGridSection::SetScrollIndexV(int nScrollIndexV)
{
	m_nScrollIndexV = nScrollIndexV;

	if (xtpGridScrollModeSmooth == GetScrollModeV())
	{
		CClientDC dc(m_pControl);
		m_nScrollOffsetV = GetRowOffset(&dc, GetRect().Width(), nScrollIndexV);
	}

	// Update scrollbar position
	if (this == m_pControl->m_pSectionScroll)
	{
		m_pControl->UpdateScrollBarV();
	}
}

void CXTPGridSection::CollapseAll()
{
	BOOL bNotify = m_pControl->GetBehavior()->Notifications->Row->bRowExpanded;

	CXTPGridRows* pRows = GetRows();

	for (int nRow = pRows->GetCount() - 1; nRow >= 0; nRow--)
	{
		CXTPGridRow* pRow = pRows->GetAt(nRow);
		ASSERT(NULL != pRow);

		if (NULL != pRow)
		{
			pRow->_SetExpanded(FALSE, FALSE, bNotify);
		}
	}

	if (!bNotify)
	{
		RefreshIndexes(0); // Refresh all indexes after expand
	}
}

void CXTPGridSection::ExpandAll(BOOL bRecursive)
{
	BOOL bNotify = m_pControl->GetBehavior()->Notifications->Row->bRowExpanded;

	CXTPGridRows* pRows = GetRows();

	for (int nRow = pRows->GetCount() - 1; nRow >= 0; nRow--)
	{
		CXTPGridRow* pRow = pRows->GetAt(nRow);
		ASSERT(NULL != pRow);

		if (NULL != pRow)
		{
			pRow->_SetExpanded(TRUE, bRecursive, bNotify);
		}
	}

	if (!bNotify)
	{
		RefreshIndexes(0); // Refresh all indexes after expand
	}
}

#ifdef _XTP_ACTIVEX

IMPLEMENT_OLETYPELIB_EX(CXTPGridSection, XTPDIID_IGridSection)

BEGIN_DISPATCH_MAP(CXTPGridSection, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPGridSection, "Index", 1, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridSection, "AllowAccess", 10, IsAllowAccess, AllowAccess, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridSection, "AllowEdit", 11, IsAllowEdit, AllowEdit, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridSection, "AllowFilter", 12, IsAllowFilter, AllowFilter, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridSection, "AllowGroup", 13, IsAllowGroup, AllowGroup, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridSection, "AllowSort", 14, IsAllowSort, AllowSort, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridSection, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridSection, XTPDIID_IGridSection, Dispatch)
END_INTERFACE_MAP()

#endif
