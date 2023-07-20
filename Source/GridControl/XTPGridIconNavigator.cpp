// XTPGridIconNavigator.cpp : implementation of the CXTPGridIconNavigator class.
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
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridNavigator.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridColumns.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridIconNavigator

CXTPGridIconNavigator::CXTPGridIconNavigator(CXTPGridControl* pGridControl)
	: CXTPGridNavigator(pGridControl)
{
}

CXTPGridIconNavigator::~CXTPGridIconNavigator()
{
}

void CXTPGridIconNavigator::MoveDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pNextRow	= NULL;
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	int iPrevRowIndex = -1;
	int iFocusRow	 = pFocusedRow->GetIndex();
	int nRpL		  = m_pGridControl->GetRowsPerLine();
	int iRowOffset	= iFocusRow % nRpL;
	// body rows
	pNextRow = m_pGridControl->GetRows()->GetNext(pFocusedRow,
												  m_pGridControl->IsSkipGroupsFocusEnabled());

	if (pNextRow)
	{
		while (pNextRow && pNextRow->GetIndex() != iPrevRowIndex
			   && iRowOffset != pNextRow->GetIndex() % nRpL)
		{
			iPrevRowIndex = pNextRow->GetIndex();
			pNextRow	  = m_pGridControl->GetRows()->GetNext(
				 pNextRow, m_pGridControl->IsSkipGroupsFocusEnabled());
		}

		if (pNextRow && pNextRow->GetIndex() != iPrevRowIndex)
		{
			// from the last body row jump to the first header row
			if (iFocusRow >= pNextRow->GetIndex())
			{
				if (m_pGridControl->IsFooterRowsVisible()
					&& m_pGridControl->IsFooterRowsAllowAccess())
					MoveFirstVisibleRow(xtpRowTypeFooter);
			}
			else
			{
				m_pGridControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
			}
		}
	}
}

void CXTPGridIconNavigator::MoveUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pPrevRow	= NULL;
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	int iPrevRowIndex = -1;
	int iFocusRow	 = pFocusedRow->GetIndex();
	int nRpL		  = m_pGridControl->GetRowsPerLine();
	int iRowOffset	= iFocusRow % nRpL;
	// body rows
	pPrevRow = m_pGridControl->GetRows()->GetPrev(pFocusedRow,
												  m_pGridControl->IsSkipGroupsFocusEnabled());
	if (pPrevRow)
	{
		while (pPrevRow && pPrevRow->GetIndex() != iPrevRowIndex
			   && iRowOffset != pPrevRow->GetIndex() % nRpL)
		{
			iPrevRowIndex = pPrevRow->GetIndex();
			pPrevRow	  = m_pGridControl->GetRows()->GetPrev(
				 pPrevRow, m_pGridControl->IsSkipGroupsFocusEnabled());
		}
		if (pPrevRow && pPrevRow->GetIndex() != iPrevRowIndex)
		{
			// from the first body row jump to the last header row
			if (iFocusRow <= pPrevRow->GetIndex())
			{
				if (m_pGridControl->IsHeaderRowsVisible()
					&& m_pGridControl->IsHeaderRowsAllowAccess())
					MoveLastVisibleRow(xtpRowTypeHeader);
			}
			else
			{
				m_pGridControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
			}
		}
	}
}

void CXTPGridIconNavigator::MoveDownStep(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pNextRow	= NULL;
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pGridControl->IsIconView())
	{
		int iPrevRowIndex = -1;
		int iFocusRow	 = pFocusedRow->GetIndex();
		// body rows
		pNextRow = m_pGridControl->GetRows()->GetNext(pFocusedRow,
													  m_pGridControl->IsSkipGroupsFocusEnabled());
		if (pNextRow)
		{
			if (pNextRow && pNextRow->GetIndex() != iPrevRowIndex)
			{
				// from the last body row jump to the first header row
				if (iFocusRow >= pNextRow->GetIndex())
				{
					if (m_pGridControl->IsFooterRowsVisible()
						&& m_pGridControl->IsFooterRowsAllowAccess())
						MoveFirstVisibleRow(xtpRowTypeFooter);
				}
				else
				{
					m_pGridControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
				}
			}
		}
	}
}

void CXTPGridIconNavigator::MoveUpStep(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pPrevRow	= NULL;
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pGridControl->IsIconView())
	{
		int iPrevRowIndex = -1;
		int iFocusRow	 = pFocusedRow->GetIndex();
		// body rows
		pPrevRow = m_pGridControl->GetRows()->GetPrev(pFocusedRow,
													  m_pGridControl->IsSkipGroupsFocusEnabled());
		if (pPrevRow)
		{
			if (pPrevRow && pPrevRow->GetIndex() != iPrevRowIndex)
			{
				// from the first body row jump to the last header row
				if (iFocusRow <= pPrevRow->GetIndex())
				{
					if (m_pGridControl->IsHeaderRowsVisible()
						&& m_pGridControl->IsHeaderRowsAllowAccess())
						MoveLastVisibleRow(xtpRowTypeHeader);
				}
				else
				{
					m_pGridControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
				}
			}
		}
	}
}

void CXTPGridIconNavigator::MovePageDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = m_pGridControl->GetFocusedRow()
							   ? m_pGridControl->GetFocusedRow()->GetIndex()
							   : 0;
	int iNextRowIndex = nCurrentRowIndex;
	while (iNextRowIndex % m_pGridControl->GetRowsPerLine())
	{
		iNextRowIndex--;
	}
	iNextRowIndex = min(m_pGridControl->GetRows()->GetCount() - 1,
						nCurrentRowIndex + m_pGridControl->GetGridAreaRows(iNextRowIndex, TRUE));

	int nRpL = m_pGridControl->GetRowsPerLine();
	// Now go backwards until we get the same equivalency class.
	while (nCurrentRowIndex < iNextRowIndex && (nCurrentRowIndex % nRpL) != (iNextRowIndex % nRpL))
	{
		iNextRowIndex--;
	}
	if (nCurrentRowIndex == iNextRowIndex)
	{
		MoveLastRow(bShiftKey, bControlKey);
	}
	else
	{
		m_pGridControl->SetFocusedRow(m_pGridControl->GetRows()->GetAt(iNextRowIndex), bShiftKey,
									  bControlKey);
	}
}

void CXTPGridIconNavigator::MovePageUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = m_pGridControl->GetFocusedRow()
							   ? m_pGridControl->GetFocusedRow()->GetIndex()
							   : 0;
	int iNextRowIndex = nCurrentRowIndex;
	while (iNextRowIndex % m_pGridControl->GetRowsPerLine())
	{
		iNextRowIndex--;
	}
	iNextRowIndex = nCurrentRowIndex - m_pGridControl->GetGridAreaRows(iNextRowIndex, FALSE);

	int nRpL = m_pGridControl->GetRowsPerLine();
	// Now go forward until we get the same equivalency class.
	while (nCurrentRowIndex > iNextRowIndex && (nCurrentRowIndex % nRpL) != (iNextRowIndex % nRpL))
	{
		iNextRowIndex++;
	}

	if (nCurrentRowIndex == iNextRowIndex)
	{
		MoveFirstRow(bShiftKey, bControlKey);
	}
	else
	{
		m_pGridControl->SetFocusedRow(m_pGridControl->GetRows()->GetAt(iNextRowIndex), bShiftKey,
									  bControlKey);
	}
}

void CXTPGridIconNavigator::BeginEdit()
{
	if (!m_pGridControl)
		return;

	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPGridColumn* pIconColumn = m_pGridControl->GetColumns()->Find(
		m_pGridControl->m_iIconViewColumn);

	if (pIconColumn && pFocusedRow && pFocusedRow->GetRecord())
	{
		XTP_GRIDRECORDITEM_ARGS itemArgs(m_pGridControl, pFocusedRow, pIconColumn);

		if (itemArgs.pItem && itemArgs.pItem->IsAllowEdit(&itemArgs))
			m_pGridControl->EditItem(&itemArgs);
	}
}

void CXTPGridIconNavigator::MoveLeftRight(BOOL bBack, BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	if (bBack)
		MoveUpStep(bShiftKey, bControlKey);
	else
		MoveDownStep(bShiftKey, bControlKey);
}
