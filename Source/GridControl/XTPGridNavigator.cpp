// XTPGridNavigator.cpp : implementation of the CXTPGridNavigator class.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridNavigator.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridSection.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridInplaceControls.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CXTPGridNavigator

CXTPGridNavigator::CXTPGridNavigator(CXTPGridControl* pGridControl)
	: m_pGridControl(pGridControl)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridNavigator::~CXTPGridNavigator()
{
}

void CXTPGridNavigator::MoveDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pNextRow	= NULL;
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pGridControl->GetHeaderRows()->HasFocus())
	{
		pNextRow = m_pGridControl->GetHeaderRows()->GetNext(
			pFocusedRow, m_pGridControl->IsSkipGroupsFocusEnabled());
		// from the last header row jump to the first visible body row
		if (pFocusedRow == pNextRow)
		{
			MoveFirstVisibleRow(xtpRowTypeBody);
		}
		else
		{
			m_pGridControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
		}
	}
	else if (m_pGridControl->GetFooterRows()->HasFocus())
	{
		pNextRow = m_pGridControl->GetFooterRows()->GetNext(
			pFocusedRow, m_pGridControl->IsSkipGroupsFocusEnabled());
		if (pNextRow != pFocusedRow)
			m_pGridControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
	}
	else
	{
		// body rows
		pNextRow = m_pGridControl->GetRows()->GetNext(pFocusedRow,
													  m_pGridControl->IsSkipGroupsFocusEnabled());
		if (pNextRow)
		{
			// from the last body row jump to the first footer row
			if (m_pGridControl->GetRows()->m_nFocusedRow == pNextRow->GetIndex())
			{
				if (m_pGridControl->IsFooterRowsVisible()
					&& m_pGridControl->IsFooterRowsAllowAccess())
				{
					MoveFirstVisibleRow(xtpRowTypeFooter);
				}
			}
			else
			{
				m_pGridControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
			}
		}
	}
	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pPrevRow	= NULL;
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pGridControl->GetHeaderRows()->HasFocus())
	{
		pPrevRow = m_pGridControl->GetHeaderRows()->GetPrev(
			pFocusedRow, m_pGridControl->IsSkipGroupsFocusEnabled());
		if (pPrevRow != pFocusedRow)
			m_pGridControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
	}
	else if (m_pGridControl->GetFooterRows()->HasFocus())
	{
		pPrevRow = m_pGridControl->GetFooterRows()->GetPrev(
			pFocusedRow, m_pGridControl->IsSkipGroupsFocusEnabled());

		// from the first footer row jump to the last visible body row
		if (pFocusedRow == pPrevRow)
			MoveLastVisibleRow(xtpRowTypeBody);
		else
			m_pGridControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
	}
	else
	{
		// body rows
		pPrevRow = m_pGridControl->GetRows()->GetPrev(pFocusedRow,
													  m_pGridControl->IsSkipGroupsFocusEnabled());

		if (pPrevRow)
		{
			if (m_pGridControl->GetRows()->m_nFocusedRow == pPrevRow->GetIndex())
			{
				if (m_pGridControl->IsHeaderRowsVisible()
					&& m_pGridControl->IsHeaderRowsAllowAccess())
				{
					CXTPGridRow* pParRow = pFocusedRow->GetParentRow();
					while (pParRow)
					{
						m_pGridControl->EnsureVisible(pParRow);
						pParRow = pParRow->GetParentRow();
					}
					MoveLastVisibleRow(xtpRowTypeHeader);
				}
			}
			else
			{
				// if (!pPrevRow->IsSelected())// && !bControlKey && !bShiftKey)
				//  m_pGridControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
				m_pGridControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
			}
		}
	}
	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveDownStep(BOOL bShiftKey, BOOL bControlKey)
{
	UNREFERENCED_PARAMETER(bShiftKey);
	UNREFERENCED_PARAMETER(bControlKey);
}

void CXTPGridNavigator::MoveUpStep(BOOL bShiftKey, BOOL bControlKey)
{
	UNREFERENCED_PARAMETER(bShiftKey);
	UNREFERENCED_PARAMETER(bControlKey);
}

void CXTPGridNavigator::MovePageDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = CheckDeadEnd(TRUE);
	if (nCurrentRowIndex == -1)
		return;

	CXTPGridRow* pCurRow = m_pGridControl->GetRows()->GetAt(nCurrentRowIndex);
	if (pCurRow && pCurRow->IsGroupRow() && m_pGridControl->IsSkipGroupsFocusEnabled())
		pCurRow = m_pGridControl->GetRows()->GetPrev(pCurRow,
													 m_pGridControl->IsSkipGroupsFocusEnabled());
	if (pCurRow)
		m_pGridControl->SetFocusedRow(pCurRow, bShiftKey, bControlKey);

	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MovePageUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = CheckDeadEnd(FALSE);
	if (nCurrentRowIndex == -1)
		return;

	CXTPGridRow* pCurRow = m_pGridControl->GetRows()->GetAt(nCurrentRowIndex);
	if (pCurRow && pCurRow->IsGroupRow() && m_pGridControl->IsSkipGroupsFocusEnabled())
		pCurRow = m_pGridControl->GetRows()->GetNext(pCurRow,
													 m_pGridControl->IsSkipGroupsFocusEnabled());
	if (pCurRow)
		m_pGridControl->SetFocusedRow(pCurRow, bShiftKey, bControlKey);

	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveFirstRow(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pFirst = m_pGridControl->GetRows()->GetAt(0);
	if (m_pGridControl->IsVirtualMode())
		m_pGridControl->SetFocusedRow(pFirst, bShiftKey, bControlKey);
	else if (pFirst != m_pGridControl->GetFocusedRow())
		m_pGridControl->SetFocusedRow(pFirst, bShiftKey, bControlKey);

	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveLastRow(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridRow* pLast = m_pGridControl->GetRows()->GetAt(m_pGridControl->GetRows()->GetCount()
														  - 1);
	if (m_pGridControl->IsVirtualMode())
		m_pGridControl->SetFocusedRow(pLast, bShiftKey, bControlKey);
	else if (pLast != m_pGridControl->GetFocusedRow())
		m_pGridControl->SetFocusedRow(pLast, bShiftKey, bControlKey);

	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveToRow(int nRowIndex, BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = max(0, nRowIndex);
	nCurrentRowIndex	 = min(nCurrentRowIndex, m_pGridControl->GetRows()->GetCount() - 1);
	if (nCurrentRowIndex < 0)
		return;

	m_pGridControl->SetFocusedRow(m_pGridControl->GetRows()->GetAt(nCurrentRowIndex), bShiftKey,
								  bControlKey);
}

void CXTPGridNavigator::BeginEdit()
{
	if (!m_pGridControl)
		return;

	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (!m_pGridControl->IsVirtualMode())
	{
		m_pGridControl->AdjustScrollBars();
		m_pGridControl->RedrawControl();
		m_pGridControl->UpdateWindow();
	}
	if (m_pGridControl->m_pFocusedColumn && pFocusedRow && pFocusedRow->GetRecord())
	{
		XTP_GRIDRECORDITEM_ARGS itemArgs(m_pGridControl, pFocusedRow,
										 m_pGridControl->m_pFocusedColumn);

		if (itemArgs.pItem && itemArgs.pItem->IsAllowEdit(&itemArgs))
		{
			if (!m_pGridControl->IsVirtualMode())
				m_pGridControl->EnsureVisible(pFocusedRow);

			m_pGridControl->EditItem(&itemArgs);

			if (m_pGridControl->GetInplaceEdit()->GetSafeHwnd()
				&& m_pGridControl->GetInplaceEdit()->GetItem() == itemArgs.pItem)
			{
				CXTPGridRecordItemEditOptions* pEditOptions = itemArgs.pItem->GetEditOptions(
					itemArgs.pColumn);
				if (pEditOptions && pEditOptions->m_bSelectTextOnEdit)
				{
					m_pGridControl->GetInplaceEdit()->SetSel(0, -1);
				}
				else
				{
					CString str;
					m_pGridControl->GetInplaceEdit()->GetWindowText(str);
					m_pGridControl->GetInplaceEdit()->SetSel(str.GetLength(), str.GetLength());
				}
			}
		}
	}
}

void CXTPGridNavigator::MoveLeftRight(BOOL bBack, BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pGridControl)
		return;

	if (m_pGridControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPGridControl::CUpdateContext updateContext(m_pGridControl);

	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPGridColumn* pFocusedColumn = m_pGridControl->GetNextFocusableColumn(
		pFocusedRow,
		m_pGridControl->m_pFocusedColumn ? m_pGridControl->m_pFocusedColumn->GetIndex() : -1,
		bBack ? -1 : 1);

	if (pFocusedColumn)
	{
		m_pGridControl->SetFocusedColumn(pFocusedColumn);
	}
	else
	{
		CXTPGridRows* pRows;
		int nFocusedRow = m_pGridControl->GetFocusedRow()
							  ? m_pGridControl->GetFocusedRow()->GetIndex()
							  : -1;
		switch (pFocusedRow->GetType())
		{
			case xtpRowTypeHeader: pRows = m_pGridControl->GetHeaderRows(); break;
			case xtpRowTypeFooter: pRows = m_pGridControl->GetFooterRows(); break;
			default: pRows = m_pGridControl->GetRows(); break;
		}
		CXTPGridRow* pRow = bBack ? pRows->GetPrev(pFocusedRow, FALSE)
								  : pRows->GetNext(pFocusedRow, FALSE);
		if (pRow && pRow->GetIndex() != nFocusedRow)
		{
			m_pGridControl->SetFocusedRow(pRow, bShiftKey, bControlKey);
			m_pGridControl->SetFocusedColumn(m_pGridControl->GetNextFocusableColumn(
				m_pGridControl->GetFocusedRow(),
				bBack ? m_pGridControl->m_pColumns->GetCount() : -1, bBack ? -1 : +1));
			if (pRow->IsGroupRow() && m_pGridControl->IsSkipGroupsFocusEnabled())
				pRow->SetSelected(FALSE);
		}
	}
	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveLeft(BOOL bShiftKey, BOOL bControlKey)
{
	MoveLeftRight(TRUE, bShiftKey, bControlKey);
}

void CXTPGridNavigator::MoveRight(BOOL bShiftKey, BOOL bControlKey)
{
	MoveLeftRight(FALSE, bShiftKey, bControlKey);
}

void CXTPGridNavigator::MoveFirstColumn()
{
	if (!m_pGridControl)
		return;

	CXTPGridControl::CUpdateContext updateContext(m_pGridControl);
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPGridColumn* pFocusedColumn = m_pGridControl->GetNextFocusableColumn(pFocusedRow, -1, +1);
	if (pFocusedColumn)
		m_pGridControl->SetFocusedColumn(pFocusedColumn);
}

void CXTPGridNavigator::MoveLastColumn()
{
	if (!m_pGridControl)
		return;

	CXTPGridControl::CUpdateContext updateContext(m_pGridControl);
	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPGridColumn* pFocusedColumn = m_pGridControl->GetNextFocusableColumn(
		pFocusedRow, m_pGridControl->GetColumns()->GetCount(), -1);
	if (pFocusedColumn)
	{
		m_pGridControl->SetFocusedColumn(pFocusedColumn);
	}
}

void CXTPGridNavigator::MoveToColumn(int nColumnIndex, BOOL bClearIfNonFocusable)
{
	if (!m_pGridControl)
		return;

	nColumnIndex = max(0, nColumnIndex);
	nColumnIndex = min(nColumnIndex, m_pGridControl->GetColumns()->GetCount() - 1);
	if (nColumnIndex < 0)
		return;

	CXTPGridControl::CUpdateContext updateContext(m_pGridControl);

	CXTPGridColumn* pColumn = m_pGridControl->GetColumns()->GetAt(nColumnIndex);
	if (!pColumn)
		return;

	CXTPGridRow* pFocusedRow = m_pGridControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (pFocusedRow->GetRecord())
	{
		CXTPGridRecordItem* pItem = pFocusedRow->GetRecord()->GetItem(pColumn);

		if (!pItem || !pItem->IsFocusable())
		{
			if (bClearIfNonFocusable)
				pColumn = NULL;
			else
				return;
		}

		m_pGridControl->SetFocusedColumn(pColumn);
	}
}

void CXTPGridNavigator::SetCurrentFocusInHeadersRows(BOOL bCurrentFocusInHeadersRows)
{
	if (!m_pGridControl->m_pSectionHeader->IsVisible()
		|| !m_pGridControl->m_pSectionHeader->IsAllowAccess())
	{
		bCurrentFocusInHeadersRows = FALSE;
	}

	if (bCurrentFocusInHeadersRows)
	{
		MoveFirstVisibleRow(xtpRowTypeHeader);
	}
	else if (!m_pGridControl->GetFooterRows()->HasFocus()
			 && m_pGridControl->m_pSectionHeader->IsAllowAccess())
	{
		MoveFirstVisibleRow(xtpRowTypeBody); // neither header nor footer is active
	}
}

void CXTPGridNavigator::SetCurrentFocusInFootersRows(BOOL bCurrentFocusInFootersRows)
{
	if (!m_pGridControl->m_pSectionFooter->IsVisible()
		|| !m_pGridControl->m_pSectionFooter->IsAllowAccess())
	{
		bCurrentFocusInFootersRows = FALSE;
	}

	if (bCurrentFocusInFootersRows)
	{
		MoveFirstVisibleRow(xtpRowTypeFooter);
	}
	else if (!m_pGridControl->GetHeaderRows()->HasFocus()
			 && m_pGridControl->m_pSectionFooter->IsAllowAccess())
	{
		MoveFirstVisibleRow(xtpRowTypeBody); // neither header nor footer is active
	}
}

BOOL CXTPGridNavigator::GetCurrentFocusInHeadersRows()
{
	return m_pGridControl->GetHeaderRows()->HasFocus();
}

BOOL CXTPGridNavigator::GetCurrentFocusInFootersRows()
{
	return m_pGridControl->GetFooterRows()->HasFocus();
}

void CXTPGridNavigator::MoveFirstVisibleRow(XTPGridRowType TargetType)
{
	CXTPGridRows* pRows = NULL;

	switch (TargetType)
	{
		case xtpRowTypeBody:
		{
			pRows				  = m_pGridControl->GetRows();
			CXTPGridRow* pTopRow  = pRows->GetAt(m_pGridControl->GetTopRowIndex());
			CXTPGridRow* pNextRow = NULL;
			if (pTopRow && pTopRow->IsGroupRow() && m_pGridControl->IsSkipGroupsFocusEnabled())
				pNextRow = pRows->GetNext(pTopRow, m_pGridControl->IsSkipGroupsFocusEnabled());
			if (pNextRow && pNextRow != pTopRow && pTopRow->IsExpanded())
				m_pGridControl->SetFocusedRow(pNextRow);
			else
				m_pGridControl->SetFocusedRow(pTopRow);
		}
		break;

		case xtpRowTypeHeader:
		{
			pRows = m_pGridControl->GetHeaderRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPGridRow* pFirst = pRows->GetAt(0);
				if (pFirst != m_pGridControl->GetFocusedRow())
					m_pGridControl->SetFocusedRow(FALSE, pFirst, FALSE, FALSE);
			}
		}
		break;

		case xtpRowTypeFooter:
		{
			pRows = m_pGridControl->GetFooterRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPGridRow* pFirst = pRows->GetAt(0);
				if (pFirst != m_pGridControl->GetFocusedRow())
				{
					m_pGridControl->SetFocusedRow(FALSE, pFirst, FALSE, FALSE);
				}
			}
		}
		break;
	}
	m_pGridControl->UnselectGroupRows();
}

void CXTPGridNavigator::MoveLastVisibleRow(XTPGridRowType TargetType)
{
	CXTPGridRows* pRows = NULL;

	switch (TargetType)
	{
		case xtpRowTypeBody:
		{
			pRows	 = m_pGridControl->GetRows();
			int nRows = m_pGridControl->GetGridAreaRows(m_pGridControl->GetTopRowIndex(), TRUE);

			if (nRows > -1 && pRows->GetCount() > 0)
			{
				int nIdx = min(m_pGridControl->GetTopRowIndex() + nRows, pRows->GetCount() - 1);

				m_pGridControl->SetFocusedRow(pRows->GetAt(nIdx));
			}
		}
		break;

		case xtpRowTypeHeader:
		{
			pRows = m_pGridControl->GetHeaderRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPGridRow* pLast = pRows->GetAt(pRows->GetCount() - 1);
				if (pLast != m_pGridControl->GetFocusedRow())
					m_pGridControl->SetFocusedRow(pLast);
			}
		}
		break;

		case xtpRowTypeFooter:
		{
			pRows = m_pGridControl->GetFooterRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPGridRow* pLast = pRows->GetAt(pRows->GetCount() - 1);
				if (pLast != m_pGridControl->GetFocusedRow())
					m_pGridControl->SetFocusedRow(pLast);
			}
		}
		break;
	}
	m_pGridControl->UnselectGroupRows();
}

int CXTPGridNavigator::CheckDeadEnd(BOOL bMoveDown)
{
	int nCurrentRowIndex = m_pGridControl->GetRows()->m_nFocusedRow != -1
							   ? m_pGridControl->GetRows()->m_nFocusedRow
							   : 0;
	int nDelta = m_pGridControl->GetGridAreaRows(nCurrentRowIndex, bMoveDown);
	if (nDelta == 0)
	{
		m_pGridControl->UnselectGroupRows();
		return -1;
	}
	if (bMoveDown)
		return min(m_pGridControl->GetRows()->GetCount() - 1, nCurrentRowIndex + nDelta);
	else
		return max(0, nCurrentRowIndex - nDelta);
}

//////////////////////////////////////////////////////////////////////////

#ifdef _XTP_ACTIVEX

#	define IMPLEMENT_OLE_MOVE(Direction)                                                          \
		void CXTPGridNavigator::OleMove##Direction(const VARIANT& oleSelectBlock,                  \
												   const VARIANT& oleIgnoreSelection)              \
		{                                                                                          \
			BOOL bShiftKey   = VariantToBool(&oleSelectBlock);                                     \
			BOOL bControlKey = VariantToBool(&oleIgnoreSelection);                                 \
			Move##Direction(bShiftKey, bControlKey);                                               \
		}

IMPLEMENT_OLE_MOVE(Up)
IMPLEMENT_OLE_MOVE(Down)
IMPLEMENT_OLE_MOVE(PageUp)
IMPLEMENT_OLE_MOVE(PageDown)
IMPLEMENT_OLE_MOVE(FirstRow)
IMPLEMENT_OLE_MOVE(LastRow)
IMPLEMENT_OLE_MOVE(Left)
IMPLEMENT_OLE_MOVE(Right)

BEGIN_DISPATCH_MAP(CXTPGridNavigator, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveUp", 1, OleMoveUp, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveDown", 2, OleMoveDown, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MovePageUp", 3, OleMovePageUp, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MovePageDown", 4, OleMovePageDown, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveFirstRow", 5, OleMoveFirstRow, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveLastRow", 6, OleMoveLastRow, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveLeft", 7, OleMoveLeft, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveRight", 8, OleMoveRight, VT_EMPTY,
					 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "BeginEdit", 9, BeginEdit, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveFirstColumn", 10, MoveFirstColumn, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveLastColumn", 11, MoveLastColumn, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveToRow", 12, OleMoveToRow, VT_EMPTY,
					 VTS_I4 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveToColumn", 13, OleMoveToColumn, VT_EMPTY,
					 VTS_I4 VTS_VARIANT)

	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveFirstVisibleRow", 14, OleMoveFirstVisibleRow, VT_EMPTY,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridNavigator, "MoveLastVisibleRow", 15, OleMoveLastVisibleRow, VT_EMPTY,
					 VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPGridColumn, "CurrentFocusInHeadersRows", 16,
						GetCurrentFocusInHeadersRows, SetCurrentFocusInHeadersRows, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "CurrentFocusInFootersRows", 17,
						GetCurrentFocusInFootersRows, SetCurrentFocusInFootersRows, VT_BOOL)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridNavigator, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridNavigator, XTPDIID_IGridNavigator, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridNavigator, XTPDIID_IGridNavigator)

void CXTPGridNavigator::OleMoveToRow(int nRowIndex, const VARIANT& oleSelectBlock,
									 const VARIANT& oleIgnoreSelection)
{
	BOOL bShiftKey   = VariantToBool(&oleSelectBlock);
	BOOL bControlKey = VariantToBool(&oleIgnoreSelection);

	MoveToRow(nRowIndex, bShiftKey, bControlKey);
}

void CXTPGridNavigator::OleMoveToColumn(int nColumnIndex, const VARIANT& oleClearIfNonFocusable)
{
	BOOL bClearIfNonFocusable = VariantToBool(&oleClearIfNonFocusable);

	MoveToColumn(nColumnIndex, bClearIfNonFocusable);
}

void CXTPGridNavigator::OleMoveFirstVisibleRow()
{
	if (m_pGridControl->GetHeaderRows()->HasFocus()
		&& m_pGridControl->m_pSectionHeader->IsVisible())
	{
		MoveFirstVisibleRow(xtpRowTypeHeader);
	}
	else
	{
		MoveFirstVisibleRow(xtpRowTypeBody);
	}
}

void CXTPGridNavigator::OleMoveLastVisibleRow()
{
	if (m_pGridControl->GetFooterRows()->HasFocus()
		&& m_pGridControl->m_pSectionFooter->IsVisible())
	{
		MoveLastVisibleRow(xtpRowTypeFooter);
	}
	else
	{
		MoveLastVisibleRow(xtpRowTypeBody);
	}
}

#endif
