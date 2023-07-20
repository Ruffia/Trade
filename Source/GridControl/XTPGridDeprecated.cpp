// XTPGridDeprecated.cpp
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

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
#include "Common/PropExchange/XTPPropExchangeRegistry.h"
#include "Common/PropExchange/XTPPropExchangeIniFile.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"

#if XTP_GRID_DEPRECATED()

#	include "GridControl/Behavior/XTPGridBehavior.h"
#	include "GridControl/XTPGridRows.h"
#	include "GridControl/XTPGridSelectedRows.h"

#	include "GridControl/XTPGridRecordItem.h"
#	include "GridControl/XTPGridRecord.h"
#	include "GridControl/XTPGridRecords.h"
#	include "GridControl/XTPGridHeader.h"
#	include "GridControl/XTPGridColumn.h"
#	include "GridControl/XTPGridColumns.h"
#	include "GridControl/XTPGridRow.h"
#	include "GridControl/XTPGridControl.h"
#	include "GridControl/XTPGridPaintManager.h"
#	include "GridControl/XTPGridNavigator.h"
#	include "GridControl/XTPGridSubListControl.h"
#	include "GridControl/XTPGridGroupRow.h"
#	include "GridControl/XTPGridInplaceControls.h"
#	include "GridControl/XTPGridRecordItemControls.h"
#	include "GridControl/XTPGridHyperlink.h"
#	include "GridControl/XTPGridRecordItemRange.h"
#	include "GridControl/XTPGridSections.h"
#	include "GridControl/XTPGridSection.h"
#	include "GridControl/XTPGridBorder.h"
#	include "GridControl/XTPGridThickness.h"

#	include "GridControl/XTPGridADO.h"
#	include "GridControl/XTPGridDataManager.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif

/////////////////////////////////////////////////////////////////////////////
// SelectionEnable
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::SelectionEnable(BOOL bEnable)
{
	m_pSectionBody->SelectionEnable(bEnable);

	if (!bEnable)
	{
		m_bInitialSelectionEnable = FALSE;
		m_bBlockSelection		  = FALSE;
		m_bMultiSelectionMode	 = FALSE;
		m_pSelectedRows->Clear();
	}
}

BOOL CXTPGridControl::IsSelectionEnabled() const
{
	return m_pSectionBody->IsSelectionEnabled();
}

void CXTPGridControl::HeaderRowsEnableSelection(BOOL bEnable)
{
	m_pSectionHeader->SelectionEnable(bEnable);
}

BOOL CXTPGridControl::IsHeaderRowsSelectionEnabled() const
{
	return m_pSectionHeader->IsSelectionEnabled();
}

void CXTPGridControl::FooterRowsEnableSelection(BOOL bEnable)
{
	m_pSectionFooter->SelectionEnable(bEnable);
}

BOOL CXTPGridControl::IsFooterRowsSelectionEnabled() const
{
	return m_pSectionFooter->IsSelectionEnabled();
}

/////////////////////////////////////////////////////////////////////////////
// Visible
/////////////////////////////////////////////////////////////////////////////

BOOL CXTPGridControl::IsHeaderRowsVisible() const
{
	return m_pSectionHeader->IsVisible();
}

BOOL CXTPGridControl::IsFooterRowsVisible() const
{
	return m_pSectionFooter->IsVisible();
}

void CXTPGridControl::ShowHeaderRows(BOOL bShow)
{
	m_pSectionHeader->SetVisible(bShow);

	if (!bShow)
		GetNavigator()->SetCurrentFocusInHeadersRows(FALSE);

	AdjustLayout();
	AdjustScrollBars();
}

void CXTPGridControl::ShowFooterRows(BOOL bShow)
{
	m_pSectionFooter->SetVisible(bShow);

	if (!bShow)
		GetNavigator()->SetCurrentFocusInFootersRows(FALSE);

	AdjustLayout();
	AdjustScrollBars();
}

/////////////////////////////////////////////////////////////////////////////
// AllowAccess
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::HeaderRowsAllowAccess(BOOL bAllowAccess)
{
	m_pSectionHeader->AllowAccess(bAllowAccess);
}

BOOL CXTPGridControl::IsHeaderRowsAllowAccess() const
{
	return m_pSectionHeader->IsAllowAccess();
}

void CXTPGridControl::FooterRowsAllowAccess(BOOL bAllowAccess)
{
	m_pSectionFooter->AllowAccess(bAllowAccess);
}

BOOL CXTPGridControl::IsFooterRowsAllowAccess() const
{
	return m_pSectionFooter->IsAllowAccess();
}

/////////////////////////////////////////////////////////////////////////////
// AllowEdit
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::AllowEdit(BOOL bAllowEdit)
{
	m_pSectionBody->AllowEdit(bAllowEdit);
}

BOOL CXTPGridControl::IsAllowEdit() const
{
	return m_pSectionBody->IsAllowEdit();
}

void CXTPGridControl::HeaderRowsAllowEdit(BOOL bAllowEdit)
{
	m_pSectionHeader->AllowEdit(bAllowEdit);
}

BOOL CXTPGridControl::IsHeaderRowsAllowEdit() const
{
	return m_pSectionHeader->IsAllowEdit();
}

void CXTPGridControl::FooterRowsAllowEdit(BOOL bAllowEdit)
{
	m_pSectionFooter->AllowEdit(bAllowEdit);
}

BOOL CXTPGridControl::IsFooterRowsAllowEdit() const
{
	return m_pSectionFooter->IsAllowEdit();
}

/////////////////////////////////////////////////////////////////////////////
// Allow Group
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::AllowGroup(BOOL bAllowGroup)
{
	m_pSectionBody->AllowGroup(bAllowGroup);
}

BOOL CXTPGridControl::IsAllowGroup() const
{
	return m_pSectionBody->IsAllowGroup();
}

void CXTPGridControl::HeaderRowsAllowGroup(BOOL bAllowGroup)
{
	m_pSectionHeader->AllowGroup(bAllowGroup);
}

BOOL CXTPGridControl::IsHeaderRowsAllowGroup() const
{
	return m_pSectionHeader->IsAllowGroup();
}

void CXTPGridControl::FooterRowsAllowGroup(BOOL bAllowGroup)
{
	m_pSectionFooter->AllowGroup(bAllowGroup);
}

BOOL CXTPGridControl::IsFooterRowsAllowGroup() const
{
	return m_pSectionFooter->IsAllowGroup();
}

/////////////////////////////////////////////////////////////////////////////
// Allow Sort
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::AllowSort(BOOL bAllowSort)
{
	m_pSectionBody->AllowSort(bAllowSort);
}

BOOL CXTPGridControl::IsAllowSort() const
{
	return m_pSectionBody->IsAllowSort();
}

void CXTPGridControl::HeaderRowsAllowSort(BOOL bAllowSort)
{
	m_pSectionHeader->AllowSort(bAllowSort);
}

BOOL CXTPGridControl::IsHeaderRowsAllowSort() const
{
	return m_pSectionHeader->IsAllowSort();
}

void CXTPGridControl::FooterRowsAllowSort(BOOL bAllowSort)
{
	m_pSectionFooter->AllowSort(bAllowSort);
}

BOOL CXTPGridControl::IsFooterRowsAllowSort() const
{
	return m_pSectionFooter->IsAllowSort();
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

int CXTPGridControl::GetTopRowIndex() const
{
	return m_pSectionScroll->GetScrollIndexV();
}

/////////////////////////////////////////////////////////////////////////////
// GetRows / GetRecords
/////////////////////////////////////////////////////////////////////////////

CXTPGridRecords* CXTPGridControl::GetRecords() const
{
	return m_pSectionBody->GetRecords();
}

CXTPGridRows* CXTPGridControl::GetRows() const
{
	return m_pSectionBody->GetRows();
}

CXTPGridRecords* CXTPGridControl::GetHeaderRecords() const
{
	return m_pSectionHeader->GetRecords();
}

CXTPGridRecords* CXTPGridControl::GetFooterRecords() const
{
	return m_pSectionFooter->GetRecords();
}

CXTPGridRows* CXTPGridControl::GetHeaderRows() const
{
	return m_pSectionHeader->GetRows();
}

CXTPGridRows* CXTPGridControl::GetFooterRows() const
{
	return m_pSectionFooter->GetRows();
}

/////////////////////////////////////////////////////////////////////////////
// Areas
/////////////////////////////////////////////////////////////////////////////

CRect CXTPGridControl::GetGridRectangle() const
{
	return m_pSectionBody->m_rcSection;
}

CRect CXTPGridControl::GetHeaderRowsRect() const
{
	return m_pSectionHeader->m_rcSection;
}

CRect CXTPGridControl::GetFooterRowsRect() const
{
	return m_pSectionFooter->m_rcSection;
}

/////////////////////////////////////////////////////////////////////////////
// Populate
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::PopulateHeaderRows()
{
	m_pSectionHeader->Populate(TRUE);
}

void CXTPGridControl::PopulateFooterRows()
{
	m_pSectionFooter->Populate(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Pin footer
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::PinFooterRows(BOOL bPin)
{
	if (bPin)
	{
		m_pSectionBody->SetHeightMode(xtpGridSectionHeightModeAutoShrink);
		m_pSectionFooter->SetHeightMode(xtpGridSectionHeightModeAutoExpand);
		// m_pSectionFooter->GetBorder()->GetBorderThickness()->SetTop(0);
	}
	else
	{
		m_pSectionBody->SetHeightMode(xtpGridSectionHeightModeExpand);
		m_pSectionFooter->SetHeightMode(xtpGridSectionHeightModeAuto);
		// m_pSectionFooter->GetBorder()->GetBorderThickness()->SetTop(1);
	}

	m_bPinFooterRows = bPin;
	AdjustLayout();
	AdjustScrollBars();
}

////////////////////////////////////////////////////////////////////////////
// Divider style
/////////////////////////////////////////////////////////////////////////////

XTPGridFixedRowsDividerStyle CXTPGridPaintManager::GetHeaderRowsDividerStyle() const
{
	if (m_pControl)
	{
		return m_pControl->m_pSectionHeader->GetDividerStyle();
	}
	else
	{
		return xtpGridFixedRowsDividerNone;
	}
}

void CXTPGridPaintManager::SetHeaderRowsDividerStyle(XTPGridFixedRowsDividerStyle style)
{
	if (m_pControl)
	{
		m_pControl->m_pSectionHeader->SetDividerStyle(style);
	}
}

XTPGridFixedRowsDividerStyle CXTPGridPaintManager::GetFooterRowsDividerStyle() const
{
	if (m_pControl)
	{
		return m_pControl->m_pSectionFooter->GetDividerStyle();
	}
	else
	{
		return xtpGridFixedRowsDividerNone;
	}
}

void CXTPGridPaintManager::SetFooterRowsDividerStyle(XTPGridFixedRowsDividerStyle style)
{
	if (m_pControl)
	{
		m_pControl->m_pSectionFooter->SetDividerStyle(style);
	}
}

#	if defined(_XTP_ACTIVEX)

int CXTPGridPaintManager::OleGetHeaderRowsDividerStyle()
{
	return GetHeaderRowsDividerStyle();
}

void CXTPGridPaintManager::OleSetHeaderRowsDividerStyle(int style)
{
	SetHeaderRowsDividerStyle(XTPGridFixedRowsDividerStyle(style));
}

int CXTPGridPaintManager::OleGetFooterRowsDividerStyle()
{
	return GetFooterRowsDividerStyle();
}

void CXTPGridPaintManager::OleSetFooterRowsDividerStyle(int style)
{
	SetFooterRowsDividerStyle(XTPGridFixedRowsDividerStyle(style));
}

#	endif // defined(_XTP_ACTIVEX)

/////////////////////////////////////////////////////////////////////////////
// Divider color
/////////////////////////////////////////////////////////////////////////////

CXTPPaintManagerColor CXTPGridPaintManager::GetHeaderRowsDividerColor() const
{
	if (m_pControl)
	{
		return m_pControl->m_pSectionHeader->GetDividerColor();
	}
	else
	{
		return CXTPPaintManagerColor(COLOR_BTNFACE);
	}
}

void CXTPGridPaintManager::SetHeaderRowsDividerColor(CXTPPaintManagerColor clrDivider)
{
	if (m_pControl)
	{
		m_pControl->m_pSectionHeader->SetDividerColor(clrDivider);
	}
}

CXTPPaintManagerColor CXTPGridPaintManager::GetFooterRowsDividerColor() const
{
	if (m_pControl)
	{
		return m_pControl->m_pSectionFooter->GetDividerColor();
	}
	else
	{
		return CXTPPaintManagerColor(COLOR_BTNFACE);
	}
}

void CXTPGridPaintManager::SetFooterRowsDividerColor(CXTPPaintManagerColor clrDivider)
{
	if (m_pControl)
	{
		m_pControl->m_pSectionFooter->SetDividerColor(clrDivider);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::SetFullColumnScrolling(BOOL bSet)
{
	if (bSet)
	{
		m_scrollModeH = xtpGridScrollModeBlock;
	}
	else
	{
		m_scrollModeH = xtpGridScrollModeSmooth;
	}

	if (m_hWnd)
	{
		CXTPGridControl::OnHScroll(SB_TOP, 0, NULL);

		RedrawControl();
		UpdateWindow();

		AdjustScrollBars();
	}
}

void CXTPGridControl::GetColumnTotalSize(int& nTotalCount, int& nTotalWidth) const
{
	nTotalCount = 0;
	nTotalWidth = 0;
	int nColumn = 0;

	GetFreezeColumnsCount(nColumn);
	nColumn = (nColumn == -1) ? 0 : nColumn + 1; // next column's index after last frozen column

	for (; nColumn < GetColumns()->GetCount(); nColumn++)
	{
		const CXTPGridColumn* pColumn = GetColumns()->GetAt(nColumn);

		if (pColumn->IsVisible())
		{
			nTotalCount += 1;
			nTotalWidth += pColumn->GetWidth();
		}
	}
}

void CXTPGridControl::GetColumnOutOfViewSize(int& nOutOfViewCount, int& nOutOfViewWidth) const
{
	int nLeft				   = 0;
	int nRight				   = 0;
	int nColumn				   = 0;
	int nLastFreezeColumnIndex = -1;

	GetFreezeColumnsCount(nLastFreezeColumnIndex);

	if (nLastFreezeColumnIndex > -1)
	{
		for (; nColumn <= nLastFreezeColumnIndex; nColumn++)
		{
			const CXTPGridColumn* pColumn = GetColumns()->GetAt(nColumn);
			if (pColumn->IsVisible())
				nLeft += pColumn->GetWidth();
		}
	}

	nOutOfViewCount = 0;
	nOutOfViewWidth = 0;

	for (; nColumn < GetColumns()->GetCount(); nColumn++)
	{
		const CXTPGridColumn* pColumn = GetColumns()->GetAt(nColumn);

		if (pColumn->IsVisible())
		{
			nRight = nLeft + pColumn->GetWidth();

			if (nRight > GetGridRectangle().right)
			{
				nOutOfViewCount += 1;
				nOutOfViewWidth += pColumn->GetWidth() - max(0, GetGridRectangle().right - nLeft);
			}

			nLeft = nRight;
		}
	}
}

void CXTPGridControl::GetColumnScrollSize(int nOutOfViewWidth, int& nScrollCount,
										  int& nScrollWidth) const
{
	nScrollCount = 0;
	nScrollWidth = 0;

	if (nOutOfViewWidth <= nScrollWidth)
		return;

	int nColumn = 0;
	GetFreezeColumnsCount(nColumn);
	nColumn = (nColumn == -1) ? 0 : nColumn + 1; // next column index after last frozen column

	for (; nColumn < GetColumns()->GetCount(); nColumn++)
	{
		const CXTPGridColumn* pColumn = GetColumns()->GetAt(nColumn);
		if (pColumn->IsVisible())
		{
			nScrollCount += 1;
			nScrollWidth += pColumn->GetWidth();

			if (nOutOfViewWidth <= nScrollWidth)
				break;
		}
	}
}

int CXTPGridControl::InsertRow(int nIndex, CXTPGridRow* pRow)
{
	return m_pSectionBody->InsertRow(nIndex, pRow);
}

CXTPGridRecord* CXTPGridControl::AddRecord(CXTPGridRecord* pRecord)
{
	return m_pSectionBody->AddRecord(pRecord);
}

void CXTPGridControl::AddRecordEx(CXTPGridRecord* pRecord, CXTPGridRecord* pParentRecord,
								  int nRowChildIndex, int nRecordChildIndex)
{
	m_pSectionBody->AddRecordEx(pRecord, pParentRecord, nRowChildIndex, nRecordChildIndex);
}

void CXTPGridControl::UpdateRecord(CXTPGridRecord* pRecord, BOOL bUpdateChildren)
{
	m_pSectionBody->UpdateRecord(pRecord, bUpdateChildren);
}

BOOL CXTPGridControl::RemoveRecordEx(CXTPGridRecord* pRecord, BOOL bAdjustLayout,
									 BOOL bRemoveFromParent)
{
	return m_pSectionBody->RemoveRecordEx(pRecord, bAdjustLayout, bRemoveFromParent);
}

BOOL CXTPGridControl::RemoveRowEx(CXTPGridRow* pRow, BOOL bAdjustLayout)
{
	return m_pSectionBody->RemoveRowEx(pRow, bAdjustLayout);
}

void CXTPGridControl::ReSortRows()
{
	m_pSections->ReSortRows();
}

#endif // XTP_GRID_DEPRECATED()
