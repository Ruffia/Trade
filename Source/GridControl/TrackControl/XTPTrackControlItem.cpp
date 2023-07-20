// XTPGridRecordTrack.cpp : implementation of the CXTPTrackControlItem class.
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridPaintManager.h"

#include "GridControl/TrackControl/XTPTrackBlock.h"
#include "GridControl/TrackControl/XTPTrackControlItem.h"
#include "GridControl/TrackControl/XTPTrackControl.h"
#include "GridControl/TrackControl/XTPTrackPaintManager.h"
#include "GridControl/TrackControl/XTPTrackUndoManager.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTrackControlItem

IMPLEMENT_SERIAL(CXTPTrackControlItem, CXTPGridRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPTrackControlItem::CXTPTrackControlItem()
	: CXTPGridRecordItem()
{
	m_bLocked = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPTrackControlItem::~CXTPTrackControlItem()
{
	for (int n = 0; n < m_arrBlocks.GetSize(); n++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks[n];
		if (pBlock)
		{
			pBlock->InternalRelease();
		}
	}
	m_arrBlocks.RemoveAll();
}

CString CXTPTrackControlItem::GetCaption(CXTPGridColumn* pColumn)
{
	if (IsChildOfMerge())
		return GetMergeItem()->GetCaption(pColumn);

	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	return _T("");
}

void CXTPTrackControlItem::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPGridRecordItem::DoPropExchange(pPX);

	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Block")));

	if (pPX->IsStoring())
	{
		int nCount   = (int)GetBlockCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPTrackBlock* pBlock = GetBlock(i);
			ASSERT(pBlock);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pBlock, RUNTIME_CLASS(CXTPTrackBlock));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPTrackBlock* pBlock = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pBlock, RUNTIME_CLASS(CXTPTrackBlock));

			if (!pBlock)
				AfxThrowArchiveException(CArchiveException::badClass);

			Add(pBlock);
		}
		RecalcLayout();
	}
}

CXTPTrackControl* CXTPTrackControlItem::GetTrackControl() const
{
	return (CXTPTrackControl*)m_pControl;
}

void CXTPTrackControlItem::HighlightWorkArea(CDC* pDC, CRect rcItem)
{
	CXTPTrackControl* pControl = ((CXTPTrackControl*)m_pControl);

	if (pControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pControl->PositionToTrack(pControl->GetWorkAreaMin());
		int nWorkAreaMax = pControl->PositionToTrack(pControl->GetWorkAreaMax());

		CRect rcWorkArea(nWorkAreaMin, rcItem.top, nWorkAreaMax, rcItem.bottom);

		pDC->FillSolidRect(rcWorkArea, pControl->GetTrackPaintManager()->m_clrWorkArea);
	}

	for (int i = 0; i < pControl->GetMarkers()->GetCount(); i++)
	{
		CXTPTrackMarker* pMarker = pControl->GetMarkers()->GetAt(i);
		int nPos				 = pControl->PositionToTrack(pMarker->GetPosition());

		pDC->FillSolidRect(CRect(nPos, rcItem.top, nPos + XTP_DPI_Y(1),
								 rcItem.bottom + XTP_DPI_Y(10)),
						   pControl->GetTrackPaintManager()->GetGridColor());
	}
}

int CXTPTrackControlItem::Draw(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return 0;
	else
		m_pControl = pDrawArgs->pControl;

	CDC* pDC	  = pDrawArgs->pDC;
	CRect& rcItem = pDrawArgs->rcItem;

	CXTPTrackControl* pControl = ((CXTPTrackControl*)m_pControl);

	CRgn rgn;
	rgn.CreateRectRgn(rcItem.left, rcItem.top - XTP_DPI_Y(1), rcItem.right, rcItem.bottom);

	if (!pDC->IsPrinting())
		pDC->SelectClipRgn(&rgn);

	XTP_GRIDRECORDITEM_METRICS* pMetrics = new XTP_GRIDRECORDITEM_METRICS();
	pDrawArgs->pRow->GetItemMetrics(pDrawArgs, pMetrics);

	CFont* pOldFont = (CFont*)pDC->SelectObject(pMetrics->pFont);

	HighlightWorkArea(pDC, rcItem);

	CRect rect = rcItem;

	rect.top++;
	rect.bottom--;

	int j;
	for (j = 0; j < (int)m_arrBlocks.GetSize(); j++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks.GetAt(j);
		if (pBlock && !pBlock->IsKey())
		{
			BOOL bSelected(FALSE);
			for (int iG = 0; iG < (int)pControl->GetSelectedBlocks()->GetCount(); iG++)
			{
				CXTPTrackBlock* pSelBlock = (CXTPTrackBlock*)pControl->GetSelectedBlocks()->GetAt(
					iG);
				if (pSelBlock == pBlock)
				{
					bSelected = TRUE;
					break;
				}
			}

			pBlock->Draw(pDC, rect, bSelected);
		}
	}

	for (j = 0; j < (int)m_arrBlocks.GetSize(); j++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks.GetAt(j);
		if (pBlock && pBlock->IsKey())
		{
			BOOL bSelected(FALSE);
			for (int iG = 0; iG < (int)pControl->GetSelectedBlocks()->GetCount(); iG++)
			{
				CXTPTrackBlock* pSelBlock = (CXTPTrackBlock*)pControl->GetSelectedBlocks()->GetAt(
					iG);
				if (pSelBlock == pBlock)
				{
					bSelected = TRUE;
					break;
				}
			}

			pBlock->Draw(pDC, rect, bSelected);
		}
	}

	pDC->SelectObject(pOldFont);

	pMetrics->InternalRelease();

	if (!pDC->IsPrinting())
		pDC->SelectClipRgn(NULL);

	rgn.DeleteObject();

	return 1;
}

struct CXTPTrackControlItem::DOCK_INFO
{
	DOCK_INFO(CXTPTrackBlock* p = 0, CRect rc = 0, int n = 0, int nPriority = 0)
	{
		pBar	 = p;
		rcMRUPos = rcBar = rc;
		nIndex			 = n;
		nMinWidth		 = 0;
		nTotlaMinWidth   = 0;
		this->nPriority  = nPriority;
	}

	CXTPTrackBlock* pBar;
	CRect rcBar;
	CRect rcMRUPos;
	int nIndex;
	int nMinWidth;
	int nTotlaMinWidth;
	int nPriority;
};

class CXTPTrackControlItem::CDockInfoArray : public CArray<DOCK_INFO, DOCK_INFO&>
{
public:
	void Sort()
	{
		qsort(GetData(), XTPToSizeTChecked(GetSize()), sizeof(DOCK_INFO), CompareFunc);
	}
	static int AFX_CDECL CompareFunc(const void* elem1, const void* elem2)
	{
		return ((DOCK_INFO*)elem1)->rcBar.left < ((DOCK_INFO*)elem2)->rcBar.left ? -1 : 1;
	}
	void _swap(LONG& A, LONG& B)
	{
		int C = B;
		B	 = A;
		A	 = C;
	}
	void InvertRects()
	{
		for (int nPos = 0; nPos < GetSize(); nPos++)
		{
			CRect& rect = ElementAt(nPos).rcBar;
			_swap(rect.left, rect.top);
			_swap(rect.right, rect.bottom);
		}
	}
};

void CXTPTrackControlItem::RecalcLayout()
{
	CDockInfoArray arrInfo;
	int nPos;

	// Step 1. Getting maximum available size;

	for (nPos = 0; nPos < m_arrBlocks.GetSize(); nPos++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks[nPos];

		pBlock->m_nPosition = pBlock->m_nMRUPosition;
		if (pBlock->IsKey())
			continue;

		CSize sizeBar(pBlock->m_nLength, 0);
		CPoint p(pBlock->m_nMRUPosition, 0);

		DOCK_INFO dockInfo(pBlock, CRect(p, sizeBar), nPos,
						   pBlock->m_nLastDragTime ? pBlock->m_nLastDragTime : nPos);
		arrInfo.Add(dockInfo);
	}
	arrInfo.Sort();

	// Step 2. if Total length is more than available, fill empty area.
	int nPriority = -1;
	int nLen	  = 0;
	BOOL bMove	= TRUE;

	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++)
	{
		CRect& rect = arrInfo[nPos].rcBar;

		bMove = (rect.left < nLen && (nPriority > arrInfo[nPos].nPriority))
				&& !arrInfo[nPos].pBar->IsLocked();
		if (bMove)
			rect.OffsetRect(nLen - rect.left, 0);
		nLen = rect.right;

		nPriority = bMove ? max(nPriority, arrInfo[nPos].nPriority) : arrInfo[nPos].nPriority;
	}

	nLen	  = GetTrackControl() ? GetTrackControl()->GetTimeLineMax() : 320000;
	nPriority = -1;
	bMove	 = TRUE;

	for (nPos = (int)arrInfo.GetSize() - 1; nPos >= 0; nPos--)
	{
		CRect& rect = arrInfo[nPos].rcBar;
		bMove		= (rect.right - nLen >= 0 && (nPriority > arrInfo[nPos].nPriority || bMove))
				&& !arrInfo[nPos].pBar->IsLocked();
		if (bMove)
			rect.OffsetRect(nLen - rect.right, 0);

		nLen = rect.left;

		if (arrInfo[nPos].pBar->IsLocked())
			nPriority = INT_MAX;
		else
			nPriority = bMove ? max(nPriority, arrInfo[nPos].nPriority) : arrInfo[nPos].nPriority;
	}

	nLen = 0;
	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++)
	{
		CRect& rect = arrInfo[nPos].rcBar;
		if (rect.left < nLen)
			rect.OffsetRect(nLen - rect.left, 0);

		nLen = rect.left + rect.Width();
	}

	// Step 4. Move it.

	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++)
	{
		CXTPTrackBlock* pBar = arrInfo[nPos].pBar;
		CRect& rect			 = arrInfo[nPos].rcBar;

		pBar->m_nPosition = rect.left;
	}
}

void CXTPTrackControlItem::AdjustBlockPosition(CXTPTrackBlock* pNewBlock, BOOL bResize)
{
	CXTPTrackControlItem* pItem = this;
	ASSERT(pNewBlock->GetItem() == this);
	int nPos;

	if (pNewBlock->IsKey())
		return;

	if (bResize)
	{
		int nLeft  = pNewBlock->m_nMRUPosition;
		int nRight = nLeft + pNewBlock->GetLength();

		for (nPos = 0; nPos < pItem->GetBlockCount(); nPos++)
		{
			CXTPTrackBlock* pBlock = pItem->GetBlock(nPos);

			// pBlock->m_nPosition = pBlock->m_nMRUPosition;
			if (pBlock->IsKey() || pBlock == pNewBlock)
				continue;

			if (pBlock->m_nMRUPosition + pBlock->m_nLength <= nLeft)
				continue;
			if (pBlock->m_nMRUPosition >= nRight)
				continue;

			if (bResize == 2 && pNewBlock->m_nMRUPosition < pBlock->m_nMRUPosition)
				nRight = pBlock->m_nMRUPosition;
			if (bResize == 1
				&& pNewBlock->m_nMRUPosition + pNewBlock->m_nLength
					   > pBlock->m_nMRUPosition + pBlock->m_nLength)
				nLeft = pBlock->m_nMRUPosition + pBlock->m_nLength;
		}

		pNewBlock->SetLength(nRight - nLeft);
		pNewBlock->SetPosition(nLeft);
	}
	else
	{
		int nPosition = pNewBlock->m_nMRUPosition;
		int nLength   = pNewBlock->GetLength();

		CDockInfoArray arrInfo;

		for (nPos = 0; nPos < pItem->GetBlockCount(); nPos++)
		{
			CXTPTrackBlock* pBlock = pItem->GetBlock(nPos);

			pBlock->m_nPosition = pBlock->m_nMRUPosition;
			if (pBlock->IsKey() || pBlock == pNewBlock)
				continue;

			CSize sizeBar(pBlock->m_nLength, 0);
			CPoint p(pBlock->m_nPosition, 0);

			DOCK_INFO dockInfo(pBlock, CRect(p, sizeBar), nPos,
							   pBlock->m_nLastDragTime ? pBlock->m_nLastDragTime : nPos);
			arrInfo.Add(dockInfo);
		}
		arrInfo.Sort();

		int nRightPosition = nPosition;

		for (nPos = 0; nPos < arrInfo.GetSize(); nPos++)
		{
			CRect rc = arrInfo[nPos].rcBar;
			if (rc.right <= nRightPosition)
				continue;

			if (rc.left >= nRightPosition + nLength)
				break;

			nRightPosition = rc.right;
		}

		int nLeftPosition = nPosition;
		for (nPos = (int)arrInfo.GetSize() - 1; nPos >= 0; nPos--)
		{
			CRect rc = arrInfo[nPos].rcBar;
			if (rc.left >= nLeftPosition + nLength)
				continue;

			if (rc.right <= nLeftPosition)
				break;

			nLeftPosition = rc.left - nLength;
		}

		if (nLeftPosition < 0)
			nLeftPosition = nRightPosition;

		if (abs(nRightPosition - nPosition) < abs(nLeftPosition - nPosition))
			pNewBlock->SetPosition(nRightPosition);
		else
			pNewBlock->SetPosition(nLeftPosition);
	}
}

BOOL CXTPTrackControlItem::OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)pClickArgs->pControl;
	CPoint point			   = pClickArgs->ptClient;
	CPoint ptFrom(point);
	pControl->ClientToScreen(&point);
	CRect rc = pClickArgs->rcItem;
	pControl->ClientToScreen(&rc);

	// pControl->SetFocusedRow(pClickArgs->pRow, FALSE, FALSE);

	CRect rcControl = pControl->GetGridRectangle();
	m_pControl->ClientToScreen(&rcControl);

	CXTPTrackBlock* pBlock = HitTest(ptFrom);
	if (pBlock && !pBlock->IsLocked())
	{
		BOOL bShiftKey   = (GetKeyState(VK_SHIFT) < 0);
		BOOL bControlKey = (GetKeyState(VK_CONTROL) < 0);

		BOOL bChanged = FALSE;
		BOOL bSelected(FALSE);
		for (int iG = 0; iG < (int)pControl->GetSelectedBlocks()->GetCount(); iG++)
		{
			CXTPTrackBlock* pSelBlock = (CXTPTrackBlock*)pControl->GetSelectedBlocks()->GetAt(iG);
			if (pSelBlock == pBlock)
			{
				if (bControlKey)
				{
					pControl->GetSelectedBlocks()->RemoveAt(iG);
					bChanged = TRUE;
				}

				bSelected = TRUE;
				break;
			}
		}

		if (!bSelected)
		{
			if (!bShiftKey && !bControlKey)
			{
				pControl->GetSelectedBlocks()->RemoveAll();
			}

			pControl->GetSelectedBlocks()->Add(pBlock);
			bChanged = TRUE;
		}

		if (bChanged)
		{
			pControl->SendMessageToParent(XTP_NM_TRACK_SELECTEDBLOCKSCHANGED);
		}

		pControl->RedrawControl();

		if (pControl->m_bAllowBlockScale && pBlock->IsResizable()
			&& pBlock->GetRect().left >= ptFrom.x - XTP_DPI_X(4))
		{
			pControl->StartDragBlocks(1);
		}
		else if (pControl->m_bAllowBlockScale && pBlock->IsResizable()
				 && pBlock->GetRect().right <= ptFrom.x + XTP_DPI_X(4))
		{
			pControl->StartDragBlocks(2);
		}
		else if (pControl->m_bAllowBlockMove)
		{
			pControl->StartDragBlocks(0);
		}
	}
	else
	{
		pControl->GetSelectedBlocks()->RemoveAll();
		pControl->RedrawControl();

		pControl->StartDragSelection();
	}
	return TRUE;
}

BOOL CXTPTrackControlItem::OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	UNREFERENCED_PARAMETER(pClickArgs);

	return TRUE;
}

void CXTPTrackControlItem::OnClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	if (!pClickArgs || !pClickArgs->pControl)
		return;

	if (OnLButtonUp(pClickArgs))
		return;

	InternalAddRef();
	CMDTARGET_ADDREF(pClickArgs->pColumn);
	CMDTARGET_ADDREF(pClickArgs->pControl);

	CXTPGridRow* pRow = pClickArgs->pRow;
	CMDTARGET_ADDREF(pRow);

	pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_CLICK,
											  &pClickArgs->ptClient);

	CMDTARGET_RELEASE(pRow);
	CMDTARGET_RELEASE(pClickArgs->pColumn);
	CMDTARGET_RELEASE(pClickArgs->pControl);
	InternalRelease();
}

void CXTPTrackControlItem::OnDblClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	if (!pClickArgs || !pClickArgs->pControl)
		return;

	if (OnLButtonDown(pClickArgs))
		return;

	CXTPGridControl* pControl = pClickArgs->pControl;

	pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_DBLCLK,
								  &pClickArgs->ptClient, -1);
}

CXTPTrackBlock* CXTPTrackControlItem::HitTest(CPoint ptPoint)
{
	CXTPTrackBlock* pClip = NULL;

	for (int j = 0; j < (int)m_arrBlocks.GetSize(); j++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks.GetAt(j);

		if (pBlock->GetRect().PtInRect(ptPoint))
		{
			if (pBlock->IsKey())
				return pBlock;
			pClip = pBlock;
		}
	}
	return pClip;
}

void CXTPTrackControlItem::OnMouseMove(UINT nFlags, CPoint point, CXTPGridControl* pControl)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPGridRow* pRow = pControl->HitTest(point);
	if (pRow)
	{
		CXTPTrackBlock* pBlock = HitTest(point);
		if (pBlock && !pBlock->IsLocked())
		{
			if (((CXTPTrackControl*)pControl)->m_bAllowBlockScale && pBlock->IsResizable()
				&& (pBlock->GetRect().left >= point.x - XTP_DPI_X(4)
					|| pBlock->GetRect().right <= point.x + XTP_DPI_X(4)))
			{
				((CXTPTrackControl*)pControl)->SetMouseCursorMode(xtpTrackCursorResize);
				::SetCursor(((CXTPTrackControl*)pControl)->m_hResizeCursor);
			}
			else if (((CXTPTrackControl*)pControl)->m_bAllowBlockMove)
			{
				((CXTPTrackControl*)pControl)->SetMouseCursorMode(xtpTrackCursorDrag);
				::SetCursor(((CXTPTrackControl*)pControl)->m_hMoveCursor);
			}
		}
	}
}

void CXTPTrackControlItem::RemoveAll()
{
	CXTPTrackUndoManager* pUndoManager = m_pControl ? GetTrackControl()->GetUndoManager() : NULL;

	if (pUndoManager)
		pUndoManager->StartGroup();

	int N = (int)m_arrBlocks.GetSize();

	for (int j = 0; j < N; j++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks.GetAt(j);

		if (pUndoManager)
			pUndoManager->AddUndoCommand(new CXTPTrackUndoDeleteBlockCommand(pBlock));

		pBlock->InternalRelease();
	}

	if (pUndoManager)
		pUndoManager->EndGroup();

	m_arrBlocks.RemoveAll();

	RedrawControl();
}

BOOL CXTPTrackControlItem::Remove(CXTPTrackBlock* pPassedBlock)
{
	ASSERT(pPassedBlock->m_pItem == this);

	int j, N = (int)m_arrBlocks.GetSize();
	for (j = 0; j < N; j++)
	{
		CXTPTrackBlock* pBlock = m_arrBlocks.GetAt(j);
		if (pBlock == pPassedBlock)
		{
			return RemoveAt(j);
		}
	}

	return FALSE;
}

BOOL CXTPTrackControlItem::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrBlocks.GetSize())
		return FALSE;

	CXTPTrackBlock* pBlock = m_arrBlocks[nIndex];

	if (m_pControl)
	{
		GetTrackControl()->GetUndoManager()->AddUndoCommand(
			new CXTPTrackUndoDeleteBlockCommand(pBlock));
	}

	m_arrBlocks.RemoveAt(nIndex);
	pBlock->InternalRelease();

	for (int j = nIndex; j < m_arrBlocks.GetSize(); j++)
	{
		m_arrBlocks[j]->m_nIndex = j;
	}

	RecalcLayout();
	RedrawControl();

	return TRUE;
}

CXTPTrackBlock* CXTPTrackControlItem::Add(CXTPTrackBlock* pBlock)
{
	pBlock->m_nIndex = (int)m_arrBlocks.Add(pBlock);
	pBlock->m_pItem  = this;

	if (m_pControl)
	{
		GetTrackControl()->GetUndoManager()->AddUndoCommand(
			new CXTPTrackUndoAddBlockCommand(pBlock));
	}

	RecalcLayout();

	RedrawControl();

	return pBlock;
}

INT_PTR CXTPTrackControlItem::OnToolHitTest(CPoint point, TOOLINFO* pTI)
{
	CXTPTrackBlock* pBlock = HitTest(point);

	if (pBlock != NULL)
	{
		CString strTip = pBlock->GetTooltip();
		CXTPToolTipContext::FillInToolInfo(pTI, m_pControl->m_hWnd, pBlock->GetRect(),
										   (INT_PTR)pBlock, strTip);

		return (INT_PTR)pBlock;
	}

	return (INT_PTR)this;
}

void CXTPTrackControlItem::RedrawControl()
{
	if (m_pControl)
		m_pControl->RedrawControl();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTrackControlItem, CXTPGridRecordItem)
	DISP_PROPERTY_EX_ID(CXTPTrackControlItem, "Block", 1000, OleGetBlock, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPTrackControlItem, "BlockCount", 1001, OleGetBlockCount, SetNotSupported,
						VT_I4)
	DISP_FUNCTION_ID(CXTPTrackControlItem, "AddBlock", 1002, OleAddBlock, VT_DISPATCH,
					 VTS_BOOL VTS_I4 VTS_I4)

	DISP_PROPERTY_EX_ID(CXTPTrackControlItem, "Locked", 1003, OleGetLocked, OleSetLocked, VT_BOOL)

	DISP_FUNCTION_ID(CXTPTrackControlItem, "RemoveBlock", 1004, OleRemoveBlock, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPTrackControlItem, "RemoveAllBlocks", 1005, RemoveAll, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPTrackControlItem, CXTPGridRecordItem)
	INTERFACE_PART(CXTPTrackControlItem, XTPDIID_TrackControlItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackControlItem, XTPDIID_TrackControlItem)

LPDISPATCH CXTPTrackControlItem::OleGetBlock(int nIndex)
{
	return XTPGetDispatch(GetBlock(nIndex));
}

int CXTPTrackControlItem::OleGetBlockCount()
{
	return GetBlockCount();
}

LPDISPATCH CXTPTrackControlItem::OleAddBlock(BOOL bKey, int nPosition, int nLength)
{
	CXTPTrackBlock* pBlock = bKey ? new CXTPTrackKey() : new CXTPTrackBlock();
	pBlock->SetPosition(nPosition);
	pBlock->SetLength(nLength);

	Add(pBlock);

	return XTPGetDispatch(pBlock);
}

BOOL CXTPTrackControlItem::OleGetLocked()
{
	return m_bLocked;
}

void CXTPTrackControlItem::OleSetLocked(BOOL bLocked)
{
	if (bLocked != m_bLocked)
	{
		m_bLocked = bLocked;
		if (m_pControl)
			m_pControl->RedrawControl();
	}
}

void CXTPTrackControlItem::OleRemoveBlock(int nIndex)
{
	RemoveAt(nIndex);
}

#endif
