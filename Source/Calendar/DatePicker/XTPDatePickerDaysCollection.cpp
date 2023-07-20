// XTPDatePickerDaysCollection.cpp: implementation of the CXTPDatePickerDaysCollection class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/DatePicker/XTPDatePickerDaysCollection.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"

#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPDatePickerDaysCollection

CXTPDatePickerDaysCollection::CXTPDatePickerDaysCollection(CXTPDatePickerControl* pControl)
	: m_pControl(pControl)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPDatePickerDaysCollection::~CXTPDatePickerDaysCollection()
{
}

void CXTPDatePickerDaysCollection::_InsertBlock(int nIndexInsert, long nDateBegin, long nDateEnd)
{
	SELECTED_BLOCK block;
	block.nDateBegin = nDateBegin;
	block.nDateEnd   = nDateEnd;
	m_arrSelectedBlocks.InsertAt(nIndexInsert, block);
}

void CXTPDatePickerDaysCollection::AddBlock(long nBegin, long nEnd)
{
	int i	  = 0;
	int nCount = (int)m_arrSelectedBlocks.GetSize();

	if (nCount > 0)
	{
		long& nDateEnd = m_arrSelectedBlocks[nCount - 1].nDateEnd;

		if (nDateEnd == nBegin)
		{
			nDateEnd = nEnd + 1;
			m_pControl->_RedrawControl(FALSE);
			return;
		}

		if (nDateEnd < nBegin)
		{
			i = nCount;
		}
	}

	for (; i < nCount; i++)
	{
		long& nDateBegin = m_arrSelectedBlocks[i].nDateBegin;
		long& nDateEnd   = m_arrSelectedBlocks[i].nDateEnd;

		if ((nDateBegin <= nBegin) && (nDateEnd > nEnd))
		{
			return;
		}

		if (nDateEnd == nBegin)
		{
			nDateEnd = nEnd + 1;
			if (i + 1 < nCount && m_arrSelectedBlocks[i + 1].nDateBegin == nDateEnd)
			{
				nDateEnd = m_arrSelectedBlocks[i + 1].nDateEnd;
				m_arrSelectedBlocks.RemoveAt(i + 1);
			}

			m_pControl->_RedrawControl(FALSE);
			return;
		}
		if (nDateBegin == nEnd + 1)
		{
			nDateBegin = nBegin;
			m_pControl->_RedrawControl(FALSE);
			return;
		}

		if (nDateBegin > nEnd)
			break;
	}

	_InsertBlock(i, nBegin, nEnd + 1);
	m_pControl->_RedrawControl(FALSE);
}

void CXTPDatePickerDaysCollection::Clear()
{
	m_arrSelectedBlocks.RemoveAll();
}

void CXTPDatePickerDaysCollection::Add(const COleDateTime& dtDay)
{
	AddBlock((long)dtDay, (long)dtDay);
}

void CXTPDatePickerDaysCollection::Remove(const COleDateTime& dtDay)
{
	long nDate = (long)dtDay;

	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		long nDateBegin = m_arrSelectedBlocks[i].nDateBegin;
		long nDateEnd   = m_arrSelectedBlocks[i].nDateEnd;

		if ((nDateBegin <= nDate) && (nDateEnd > nDate))
		{
			if (nDateBegin != nDate)
			{
				_InsertBlock(i, nDateBegin, nDate);
				i++;
			}

			if (nDateEnd - 1 != nDate)
			{
				_InsertBlock(i + 1, nDate + 1, nDateEnd);
			}

			m_arrSelectedBlocks.RemoveAt(i);
			return;
		}
	}
}

void CXTPDatePickerDaysCollection::Select(const COleDateTime& dtDay)
{
	long nDate = (long)dtDay;

	if (m_arrSelectedBlocks.GetSize() == 1
		&& m_arrSelectedBlocks[0].nDateBegin == m_arrSelectedBlocks[0].nDateEnd - 1
		&& m_arrSelectedBlocks[0].nDateBegin == nDate)
	{
	}
	else
	{
		Clear();
		Add(dtDay);
	}
}

void CXTPDatePickerDaysCollection::SelectRange(const COleDateTime& dtDayBegin,
											   const COleDateTime& dtDayEnd)
{
	COleDateTime dtDayBeginCopy;
	dtDayBeginCopy.SetDate(dtDayBegin.GetYear(), dtDayBegin.GetMonth(), dtDayBegin.GetDay());
	COleDateTime dtDayEndCopy;
	dtDayEndCopy.SetDate(dtDayEnd.GetYear(), dtDayEnd.GetMonth(), dtDayEnd.GetDay());

	// From MSDN: The COleDateTime class handles dates from 1 January 100 - 31 December 9999.
	if (dtDayBeginCopy.GetYear() < 100)
		dtDayBeginCopy.SetDate(100, dtDayBeginCopy.GetMonth(), dtDayBeginCopy.GetDay());

	if (dtDayEndCopy.GetYear() < 100)
		dtDayEndCopy.SetDate(100, dtDayEndCopy.GetMonth(), dtDayEndCopy.GetDay());

	if (dtDayBeginCopy.GetYear() >= 10000)
		dtDayBeginCopy.SetDate(9999, dtDayBeginCopy.GetMonth(), dtDayBeginCopy.GetDay());

	if (dtDayEndCopy.GetYear() >= 10000)
		dtDayEndCopy.SetDate(9999, dtDayEndCopy.GetMonth(), dtDayEndCopy.GetDay());

	long nBeginDate = (long)dtDayBeginCopy;
	long nEndDate   = (long)dtDayEndCopy;

	if (nBeginDate > nEndDate)
	{
		long nBlockBegin = nBeginDate;
		nBeginDate		 = nEndDate;
		nEndDate		 = nBlockBegin;
	}

	if (m_arrSelectedBlocks.GetSize() == 1 && m_arrSelectedBlocks[0].nDateBegin == nBeginDate
		&& m_arrSelectedBlocks[0].nDateEnd == nEndDate + 1)
	{
		return;
	}

	Clear();

	_InsertBlock(0, nBeginDate, nEndDate + 1);
}

BOOL CXTPDatePickerDaysCollection::Contains(const COleDateTime& dtDay) const
{
	long nDay = (long)dtDay;

	int nCount = (int)m_arrSelectedBlocks.GetSize();
	if (nCount == 0)
		return FALSE;

	if (m_arrSelectedBlocks[0].nDateBegin > nDay)
		return FALSE;

	if (m_arrSelectedBlocks[nCount - 1].nDateEnd <= nDay)
		return FALSE;

	for (int i = 0; i < nCount; i++)
	{
		if ((m_arrSelectedBlocks[i].nDateBegin <= nDay) && (m_arrSelectedBlocks[i].nDateEnd > nDay))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CXTPDatePickerDaysCollection::GetMinMaxRange(COleDateTime& refMinRange,
												  COleDateTime& refMaxRange) const
{
	int nSelCount = (int)m_arrSelectedBlocks.GetSize();
	if (nSelCount == 0)
		return FALSE;

	refMinRange = (DATE)(double)m_arrSelectedBlocks[0].nDateBegin;
	refMaxRange = (DATE)(double)m_arrSelectedBlocks[nSelCount - 1].nDateEnd - 1;

	return TRUE;
}

int CXTPDatePickerDaysCollection::GetSelectedDaysCount() const
{
	int nCount = 0;
	for (int i = 0; i < (int)m_arrSelectedBlocks.GetSize(); i++)
	{
		nCount += m_arrSelectedBlocks[i].nDateEnd - m_arrSelectedBlocks[i].nDateBegin;
	}
	return nCount;
}

#ifdef _XTP_ACTIVEX

class _XTP_EXT_CLASS CXTPDatePickerSelectionBlock : public CXTPCmdTarget
{
public:
	CXTPDatePickerSelectionBlock(long nBeginDate, long nEndDate)
	{
		m_nBeginDate = nBeginDate;
		m_nEndDate   = nEndDate;

		EnableAutomation();
		EnableTypeLib();
	}
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPDatePickerSelectionBlock);

	DATE OleGetDateBegin();
	DATE OleGetDateEnd();

protected:
	long m_nBeginDate;
	long m_nEndDate;
};

BEGIN_DISPATCH_MAP(CXTPDatePickerSelectionBlock, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPDatePickerSelectionBlock, "DateBegin", 1, OleGetDateBegin,
						SetNotSupported, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPDatePickerSelectionBlock, "DateEnd", 2, OleGetDateEnd, SetNotSupported,
						VT_DATE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPDatePickerSelectionBlock, CXTPCmdTarget)
	INTERFACE_PART(CXTPDatePickerSelectionBlock, XTPDIID_DatePickerSelectionBlock, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDatePickerSelectionBlock, XTPDIID_DatePickerSelectionBlock)

DATE CXTPDatePickerSelectionBlock::OleGetDateBegin()
{
	return m_nBeginDate;
}

DATE CXTPDatePickerSelectionBlock::OleGetDateEnd()
{
	return m_nEndDate;
}

BEGIN_DISPATCH_MAP(CXTPDatePickerDaysCollection, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPDatePickerSelectionBlock, "AllowNoncontinuousSelection", 1,
						OleGetAllowNoncontinuousSelection, OleSetAllowNoncontinuousSelection,
						VT_BOOL)
	DISP_FUNCTION_ID(CXTPDatePickerDaysCollection, "BlocksCount", 103, OleGetItemCount, VT_I4,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPDatePickerDaysCollection, "Blocks", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPDatePickerDaysCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum,
					 VT_UNKNOWN, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPDatePickerDaysCollection, CXTPCmdTarget)
	INTERFACE_PART(CXTPDatePickerDaysCollection, XTPDIID_DatePickerSelection, Dispatch)
	// INTERFACE_PART(CXTPDatePickerDaysCollection, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDatePickerDaysCollection, XTPDIID_DatePickerSelection)
IMPLEMENT_ENUM_VARIANT(CXTPDatePickerDaysCollection)

int CXTPDatePickerDaysCollection::OleGetItemCount()
{
	return GetSelectedBlocksCount();
}

LPDISPATCH CXTPDatePickerDaysCollection::OleGetItem(long nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_arrSelectedBlocks.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	SELECTED_BLOCK block				 = GetSelectedBlock(nIndex);
	CXTPDatePickerSelectionBlock* pBlock = new CXTPDatePickerSelectionBlock(block.nDateBegin,
																			block.nDateEnd - 1);
	return pBlock->GetIDispatch(FALSE);
}

BOOL CXTPDatePickerDaysCollection::OleGetAllowNoncontinuousSelection()
{
	return m_pControl->IsAllowNoncontinuousSelection();
}

void CXTPDatePickerDaysCollection::OleSetAllowNoncontinuousSelection(BOOL bSet)
{
	m_pControl->AllowNoncontinuousSelection(bSet);
}

#endif
