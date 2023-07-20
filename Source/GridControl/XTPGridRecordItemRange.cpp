// XTPGridRecordItemRange.cpp
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

#include "Common/XTPFramework.h"

#include "GridControl/XTPGridRecordItemRange.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPGridRecordItemId::CXTPGridRecordItemId()
	: m_nColumn(-1)
	, m_nRecord(-1)
{
	// Nothing
}

CXTPGridRecordItemId::CXTPGridRecordItemId(int nColumn, int nRecord)
	: m_nColumn(nColumn)
	, m_nRecord(nRecord)
{
	// Nothing
}

void CXTPGridRecordItemId::Set(int nColumn, int nRecord)
{
	m_nColumn = nColumn;
	m_nRecord = nRecord;
}

int CXTPGridRecordItemId::GetColumn() const
{
	return m_nColumn;
}

int CXTPGridRecordItemId::GetRecord() const
{
	return m_nRecord;
}

bool CXTPGridRecordItemId::operator==(const CXTPGridRecordItemId& otherId) const
{
	return (this->m_nColumn == otherId.m_nColumn) && (this->m_nRecord == otherId.m_nRecord);
}

CXTPGridRecordMergeItemId::CXTPGridRecordMergeItemId()
	: CXTPGridRecordItemId()
	, m_nColumnPos(-1)
{
}

CXTPGridRecordMergeItemId::CXTPGridRecordMergeItemId(int nColumnPos, int nColumn, int nRecord)
	: CXTPGridRecordItemId(nColumn, nRecord)
	, m_nColumnPos(nColumnPos)
{
}

/////////////////////////////////////////////////////////////////////////////

CXTPGridRecordItemRange::CXTPGridRecordItemRange()
	: m_nColumnFrom(-1)
	, m_nColumnTo(-1)
	, m_nRecordFrom(-1)
	, m_nRecordTo(-1)
{
	// Nothing
}

CXTPGridRecordItemRange::CXTPGridRecordItemRange(int nColumnFrom, int nColumnTo, int nRecordFrom,
												 int nRecordTo)
	: m_nColumnFrom(nColumnFrom)
	, m_nColumnTo(nColumnTo)
	, m_nRecordFrom(nRecordFrom)
	, m_nRecordTo(nRecordTo)
{
	// Nothing
}

void CXTPGridRecordItemRange::Invalidate()
{
	m_nColumnFrom = -1;
	m_nColumnTo   = -1;
	m_nRecordFrom = -1;
	m_nRecordTo   = -1;
}

bool CXTPGridRecordItemRange::IsValid() const
{
	return (m_nColumnFrom >= 0) && (m_nColumnTo >= 0) && (m_nRecordFrom >= 0) && (m_nRecordTo >= 0);
}

bool CXTPGridRecordItemRange::IsInRange(const int nColumn, const int nRecord) const
{
	return (nColumn >= m_nColumnFrom) && (nColumn <= m_nColumnTo) && (nRecord >= m_nRecordFrom)
		   && (nRecord <= m_nRecordTo);
}

bool CXTPGridRecordItemRange::IsNeighbour(const int nColumn, const int nRecord) const
{
	bool bNeighbour = false;

	if (IsValid())
	{
		if (abs(m_nColumnFrom - nColumn) <= 1 && abs(m_nColumnTo - nColumn) <= 1
			&& abs(m_nRecordFrom - nRecord) <= 1 && abs(m_nRecordTo - nRecord) <= 1)
		{
			bNeighbour = true;
		}
	}

	return bNeighbour;
}

bool CXTPGridRecordItemRange::IsConnected(const int nColumn, const int nRecord) const
{
	bool bConnected = false;

	/*
		Surrounding items marked as '+' are considered connected,
		items marked as '-' not.

		[-][+][+][-]
		[+][ ][ ][+]
		[+][ ][ ][+]
		[-][+][+][-]
	*/

	if (IsValid())
	{
		if (nColumn >= m_nColumnFrom && nColumn <= m_nColumnTo)
		{
			if (nRecord == m_nRecordFrom - 1 || nRecord == m_nRecordTo + 1)
			{
				bConnected = true;
			}
		}

		if (nRecord >= m_nRecordFrom && nRecord <= m_nRecordTo)
		{
			if (nColumn == m_nColumnFrom - 1 || nColumn == m_nColumnTo + 1)
			{
				bConnected = true;
			}
		}
	}

	return bConnected;
}

void CXTPGridRecordItemRange::ExpandTo(int nColumn, int nRecord)
{
	if (IsValid())
	{
		m_nColumnFrom = min(m_nColumnFrom, nColumn);
		m_nColumnTo   = max(m_nColumnTo, nColumn);
		m_nRecordFrom = min(m_nRecordFrom, nRecord);
		m_nRecordTo   = max(m_nRecordTo, nRecord);
	}
}

void CXTPGridRecordItemRange::ExpandTo(const CXTPGridRecordItemId expand)
{
	ExpandTo(expand.m_nColumn, expand.m_nRecord);
}

/////////////////////////////////////////////////////////////////////////////

CXTPGridRecordMergeItem::CXTPGridRecordMergeItem()
{
	// Nothing
}

CXTPGridRecordMergeItem::CXTPGridRecordMergeItem(const CXTPGridRecordMergeItem& other)
{
	CopyFrom(other);
}

CXTPGridRecordMergeItem& CXTPGridRecordMergeItem::operator=(const CXTPGridRecordMergeItem& other)
{
	if (this != &other)
	{
		m_items.RemoveAll();
		CopyFrom(other);
	}

	return *this;
}

void CXTPGridRecordMergeItem::AddItem(CXTPGridRecordMergeItemId itemId, CRect rcItem)
{
	Item item;
	item.m_itemId = itemId;
	item.m_rcItem = rcItem;

	m_items.AddTail(item);
}

bool CXTPGridRecordMergeItem::FindRange(CXTPGridRecordItemRange& range, CRect& rcItem)
{
	CXTPGridRecordItemRange internal;

	range.Invalidate();

	Item item;

	POSITION pos = m_items.GetHeadPosition();

	while (pos)
	{
		POSITION posOld = pos;
		item			= m_items.GetNext(pos);

		if (!internal.IsValid())
		{
			m_items.RemoveAt(posOld);
			rcItem = item.m_rcItem;

			internal.m_nColumnFrom = item.m_itemId.m_nColumnPos;
			internal.m_nColumnTo   = item.m_itemId.m_nColumnPos;
			internal.m_nRecordFrom = item.m_itemId.m_nRecord;
			internal.m_nRecordTo   = item.m_itemId.m_nRecord;

			range.m_nColumnFrom = item.m_itemId.m_nColumn;
			range.m_nColumnTo   = item.m_itemId.m_nColumn;
			range.m_nRecordFrom = item.m_itemId.m_nRecord;
			range.m_nRecordTo   = item.m_itemId.m_nRecord;
		}
		else
		{
			if (internal.IsInRange(item.m_itemId.m_nColumnPos, item.m_itemId.m_nRecord)
				|| internal.IsConnected(item.m_itemId.m_nColumnPos, item.m_itemId.m_nRecord))
			{
				m_items.RemoveAt(posOld);

				internal.ExpandTo(item.m_itemId.m_nColumnPos, item.m_itemId.m_nRecord);
				range.ExpandTo(item.m_itemId);

				rcItem.left   = min(rcItem.left, item.m_rcItem.left);
				rcItem.right  = max(rcItem.right, item.m_rcItem.right);
				rcItem.top	= min(rcItem.top, item.m_rcItem.top);
				rcItem.bottom = max(rcItem.bottom, item.m_rcItem.bottom);
			}
		}
	}

	return range.IsValid();
}

void CXTPGridRecordMergeItem::CopyFrom(const CXTPGridRecordMergeItem& other)
{
	POSITION pos = other.m_items.GetHeadPosition();

	while (pos)
	{
		m_items.AddTail(other.m_items.GetAt(pos));
		other.m_items.GetNext(pos);
	}
}
