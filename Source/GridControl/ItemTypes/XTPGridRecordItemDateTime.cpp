// XTPGridRecordItemDateTime.cpp : implementation of the CXTPGridRecordItemDateTime class.
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/ItemTypes/XTPGridRecordItemDateTime.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridGroupRow.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemDateTime

IMPLEMENT_SERIAL(CXTPGridRecordItemDateTime, CXTPGridRecordItem,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemDateTime::CXTPGridRecordItemDateTime(COleDateTime odtValue)
	: CXTPGridRecordItem()
	, m_odtValue(odtValue)
{
	// to avoid new string data allocation for each record
	static const CString cstrDateFormatDefault(_T("%a %b/%d/%Y %I:%M %p"));
	m_strFormatString = cstrDateFormatDefault;
}

CString CXTPGridRecordItemDateTime::GetCaption(CXTPGridColumn* pColumn)
{
	if (IsChildOfMerge())
		return GetMergeItem()->GetCaption(pColumn);

	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	return CXTPGridControlLocale::FormatDateTime(m_odtValue, m_strFormatString);
}

int CXTPGridRecordItemDateTime::Compare(CXTPGridColumn*, CXTPGridRecordItem* pItem)
{
	CXTPGridRecordItemDateTime* pItemDateTime = DYNAMIC_DOWNCAST(CXTPGridRecordItemDateTime, pItem);
	if (!pItemDateTime)
		return 0;

	return CompareValues(m_odtValue, pItemDateTime->m_odtValue);
}

COleVariant CXTPGridRecordItemDateTime::GetGroupCaptionValue(CXTPGridColumn* pColumn)
{
	UNREFERENCED_PARAMETER(pColumn);
	return m_odtValue;
}

int CXTPGridRecordItemDateTime::CompareGroupCaption(CXTPGridColumn* pColumn,
													CXTPGridGroupRow* pGroupRow)
{
	ASSERT_VALID(pGroupRow);
	UNREFERENCED_PARAMETER(pColumn);

	if (!m_strGroupCaption.IsEmpty())
		return m_pRecord->GetRecords()->Compare(m_strGroupCaption, pGroupRow->GetCaption());

	COleVariant vtGroupCaptionValue = pGroupRow->GetCaptionValue();
	if (!SUCCEEDED(::VariantChangeType(&vtGroupCaptionValue, &vtGroupCaptionValue, 0, VT_DATE)))
		return 0;

	return CompareValues(m_odtValue, vtGroupCaptionValue.date);
}

void CXTPGridRecordItemDateTime::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	UNREFERENCED_PARAMETER(pItemArgs);
	m_odtValue.ParseDateTime(szText);
}

void CXTPGridRecordItemDateTime::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPGridRecordItem::DoPropExchange(pPX);

	PX_DateTime(pPX, _T("Value"), m_odtValue);
}

int CXTPGridRecordItemDateTime::CompareValues(const COleDateTime& dt1, const COleDateTime& dt2)
{
	if (dt1 == dt2)
		return 0;

	if (dt1.GetStatus() != COleDateTime::valid || dt2.GetStatus() != COleDateTime::valid)
		return int(dt1.m_dt - dt2.m_dt);

	if (dt1 > dt2)
		return 1;

	return -1;
}
