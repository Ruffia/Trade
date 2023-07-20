// XTPGridRecordItemRange.h: interface for the CXTPGridRecordItemRange class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPGRIDRECORDITEMRANGE_H__)
#	define __XTPGRIDRECORDITEMRANGE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRecordItem;

class _XTP_EXT_CLASS CXTPGridRecordItemId
{
public:
	CXTPGridRecordItemId();
	CXTPGridRecordItemId(int nColumn, int nRecord);

	void Set(int nColumn, int nRecord);

	int GetColumn() const;

	int GetRecord() const;

	bool operator==(const CXTPGridRecordItemId& otherId) const;

public:
	int m_nColumn; // Column index.
	int m_nRecord; // Record index.
};

class _XTP_EXT_CLASS CXTPGridRecordMergeItemId : public CXTPGridRecordItemId
{
public:
	CXTPGridRecordMergeItemId();
	CXTPGridRecordMergeItemId(int nColumnPos, int nColumn, int nRecord);

public:
	int m_nColumnPos; // Column position.
};

class _XTP_EXT_CLASS CXTPGridRecordItemRange
{
public:
	CXTPGridRecordItemRange();

	CXTPGridRecordItemRange(int nColumnFrom, int nColumnTo, int nRecordFrom, int nRecordTo);

	void SetTopLeft(const CXTPGridRecordItemId topLeft);
	void SetBottomRight(const CXTPGridRecordItemId bottomRight);

	void ExpandTo(int nColumn, int nRecord);

	void ExpandTo(const CXTPGridRecordItemId expand);

	void Invalidate();

	bool IsValid() const;

	bool IsInRange(const int nColumn, const int nRecord) const;

	bool IsNeighbour(const int nColumn, const int nRecord) const;

	bool IsConnected(const int nColumn, const int nRecord) const;

public:
	int m_nColumnFrom;
	int m_nColumnTo;
	int m_nRecordFrom;
	int m_nRecordTo;
};

class _XTP_EXT_CLASS CXTPGridRecordMergeItem
{
public:
	CXTPGridRecordMergeItem();
	CXTPGridRecordMergeItem(const CXTPGridRecordMergeItem& other);

	CXTPGridRecordMergeItem& operator=(const CXTPGridRecordMergeItem& other);

	void AddItem(CXTPGridRecordMergeItemId itemId, CRect rcItem);

	bool FindRange(CXTPGridRecordItemRange& range, CRect& rcItem);

	struct Item
	{
		CXTPGridRecordMergeItemId m_itemId;
		CRect m_rcItem;
	};

public:
	void CopyFrom(const CXTPGridRecordMergeItem& other);

	CList<Item, Item> m_items;
};

class _XTP_EXT_CLASS CXTPGridRecordMergeItems
	: public CMap<CXTPGridRecordItem*, CXTPGridRecordItem*, CXTPGridRecordMergeItem,
				  CXTPGridRecordMergeItem&>
{
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORDITEMRANGE_H__)
