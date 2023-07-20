// XTPGridRecordItemText.cpp : implementation of the CXTPGridRecordItemText class.
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

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/ItemTypes/XTPGridRecordItemText.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemText

IMPLEMENT_SERIAL(CXTPGridRecordItemText, CXTPGridRecordItem,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemText::CXTPGridRecordItemText(LPCTSTR szText)
	: CXTPGridRecordItem()
{
	SetValue(szText);
}

void CXTPGridRecordItemText::SetValue(LPCTSTR szText)
{
	m_strText = szText;
	ParseBBCode(m_strText);
}

CString CXTPGridRecordItemText::GetCaption(CXTPGridColumn* pColumn)
{
	if (IsChildOfMerge())
		return GetMergeItem()->GetCaption(pColumn);

	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	if (m_strFormatString == _T("%s"))
		return m_strText;

	CString strCaption;
	strCaption.Format(m_strFormatString, (LPCTSTR)m_strText);
	return strCaption;
}

void CXTPGridRecordItemText::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	SetValue(szText);

	CXTPGridRecordItem::OnEditChanged(pItemArgs, szText);
}

void CXTPGridRecordItemText::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPGridRecordItem::DoPropExchange(pPX);

	PX_String(pPX, _T("Text"), m_strText, _T(""));
}
