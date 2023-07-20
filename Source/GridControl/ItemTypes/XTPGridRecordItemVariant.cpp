// XTPGridRecordItemVariant.cpp : implementation of the XTPGridRecordItemVariant class.
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
#include "GridControl/XTPGridRecordItemConstraint.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/ItemTypes/XTPGridRecordItemVariant.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CXTPGridRecordItemVariant::m_nSortLocale = LOCALE_USER_DEFAULT;

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemVariant

IMPLEMENT_SERIAL(CXTPGridRecordItemVariant, CXTPGridRecordItem,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemVariant::CXTPGridRecordItemVariant(const VARIANT& lpValue)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif

	m_oleValue = lpValue;
	m_oleValue.ChangeType((VARTYPE)(m_oleValue.vt & ~VT_BYREF));
}

void CXTPGridRecordItemVariant::SetValue(const COleVariant& var)
{
	m_oleValue = var;
}

void CXTPGridRecordItemVariant::SetValue(const VARIANT& var)
{
	m_oleValue = var;
}

void CXTPGridRecordItemVariant::SetValue(const VARIANT* var)
{
	m_oleValue = var;
}

COleVariant CXTPGridRecordItemVariant::GetValue()
{
	return m_oleValue;
}

CString CXTPGridRecordItemVariant::GetCaption(CXTPGridColumn* pColumn)
{
	if (IsChildOfMerge())
		return GetMergeItem()->GetCaption(pColumn);

	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	COleVariant var(m_oleValue);

	CXTPGridRecordItemEditOptions* pEditOptions = NULL;
	if (m_pEditOptions)
		pEditOptions = m_pEditOptions;
	else if (pColumn)
		pEditOptions = pColumn->GetEditOptions();

	BOOL bConstraintEdit = FALSE;
	if (pEditOptions)
		bConstraintEdit = pEditOptions->m_bConstraintEdit;

	TRY
	{
		if (var.vt == VT_DATE && !bConstraintEdit && m_strFormatString != _T("%s"))
		{
			COleDateTime dt(var);
			return CXTPGridControlLocale::FormatDateTime(dt, m_strFormatString);
		}
		if (var.vt == VT_NULL)
		{
			var.vt   = VT_I4;
			var.lVal = 0;
		}
		else
		{
			if (bConstraintEdit)
				CXTPGridControlLocale::VariantChangeTypeEx(var, VT_I4);
			else
				CXTPGridControlLocale::VariantChangeTypeEx(var, VT_BSTR);
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	if (bConstraintEdit)
	{
		CXTPGridRecordItemConstraint* pConstraint = pEditOptions
														? pEditOptions->FindConstraint(
															  XTPToULongPtrChecked(var.lVal))
														: NULL;
		return pConstraint ? pConstraint->m_strConstraint : CString();
	}

	if (var.vt != VT_BSTR) // this function expected VT_BSTR only?
		return CString();

	CString strVariant(var.bstrVal);

	if (m_strFormatString == _T("%s"))
		return strVariant;

	CString strCaption;
	strCaption.Format(m_strFormatString, (LPCTSTR)strVariant);
	return strCaption;
}

COleVariant CXTPGridRecordItemVariant::GetGroupCaptionValue(CXTPGridColumn* pColumn)
{
	UNREFERENCED_PARAMETER(pColumn);
	return m_oleValue;
}

int CXTPGridRecordItemVariant::CompareGroupCaption(CXTPGridColumn* pColumn,
												   CXTPGridGroupRow* pGroupRow)
{
	ASSERT_VALID(pGroupRow);
	UNREFERENCED_PARAMETER(pColumn);

	if (!m_strGroupCaption.IsEmpty())
		return m_pRecord->GetRecords()->Compare(m_strGroupCaption, pGroupRow->GetCaption());

	return CompareValues(m_oleValue, pGroupRow->GetCaptionValue(),
						 m_pRecord->GetRecords()->IsCaseSensitive());
}

int CXTPGridRecordItemVariant::Compare(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem)
{
	ASSERT_VALID(pItem);
	UNREFERENCED_PARAMETER(pColumn);

	if (GetSortPriority() != -1 || pItem->GetSortPriority() != -1)
		return GetSortPriority() - pItem->GetSortPriority();

	CXTPGridRecordItemVariant* pItemVariant = DYNAMIC_DOWNCAST(CXTPGridRecordItemVariant, pItem);
	if (!pItemVariant)
		return 0;

	return CompareValues(m_oleValue, pItemVariant->m_oleValue,
						 m_pRecord->GetRecords()->IsCaseSensitive());
}

BOOL CXTPGridRecordItemVariant::OnValueChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs,
												LPVARIANT lpNewValue)
{
#ifdef _XTP_ACTIVEX
	XTP_NM_GRIDVALUECHANGING nmParams;
	::ZeroMemory(&nmParams, sizeof(nmParams));

	nmParams.pColumn	= pItemArgs->pColumn;
	nmParams.pItem		= this;
	nmParams.pRow		= pItemArgs->pRow;
	nmParams.bCancel	= FALSE;
	nmParams.lpNewValue = lpNewValue;

	pItemArgs->pControl->SendNotifyMessage(XTP_NM_GRID_VALUECHANGING, (NMHDR*)&nmParams);

	return !nmParams.bCancel;
#else
	UNREFERENCED_PARAMETER(pItemArgs);
	UNREFERENCED_PARAMETER(lpNewValue);
	return TRUE;
#endif
}

BOOL CXTPGridRecordItemVariant::OnEditChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs,
											   CString& rstrNewText)
{
#ifdef _XTP_ACTIVEX
	XTP_NM_GRIDVALUECHANGING nmParams;
	::ZeroMemory(&nmParams, sizeof(nmParams));

	COleVariant olevarText = rstrNewText;
	ASSERT(olevarText.vt == VT_BSTR);

	nmParams.pColumn	= pItemArgs->pColumn;
	nmParams.pItem		= this;
	nmParams.pRow		= pItemArgs->pRow;
	nmParams.bCancel	= FALSE;
	nmParams.lpNewValue = &olevarText;

	pItemArgs->pControl->SendNotifyMessage(XTP_NM_GRID_EDIT_CHANGING, (NMHDR*)&nmParams);

	if (!nmParams.bCancel)
	{
		BOOL bConvert = CXTPGridControlLocale::VariantChangeTypeEx(olevarText, VT_BSTR, FALSE);
		if (bConvert && olevarText.vt == VT_BSTR)
		{
			rstrNewText = olevarText.bstrVal;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return !nmParams.bCancel;
#else
	UNREFERENCED_PARAMETER(pItemArgs);
	UNREFERENCED_PARAMETER(rstrNewText);
	return TRUE;
#endif
}

void CXTPGridRecordItemVariant::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	COleVariant var(szText);

	if (OnValueChanging(pItemArgs, &var))
	{
		TRY
		{
			CXTPGridControlLocale::VariantChangeTypeEx(var, m_oleValue.vt == VT_NULL
																? (VARTYPE)VT_BSTR
																: m_oleValue.vt);
		}
		CATCH_ALL(e)
		{
			return;
		}
		END_CATCH_ALL

		m_oleValue = var;
	}
}

void CXTPGridRecordItemVariant::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPGridRecordItem::DoPropExchange(pPX);

	COleVariant varDefault(_T(""));
	PX_Variant(pPX, _T("Value"), m_oleValue, varDefault);
}

void CXTPGridRecordItemVariant::OnConstraintChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs,
													CXTPGridRecordItemConstraint* pConstraint)
{
	ASSERT_VALID(pConstraint);
	ASSERT(NULL != pItemArgs);

	BOOL bChooseOnly = GetEditOptions(pItemArgs->pColumn)->m_bConstraintEdit;
	if (bChooseOnly)
	{
		long index = (long)pConstraint->m_dwData;
		COleVariant var;
		var.vt   = VT_I4;
		var.lVal = index;

		if (m_oleValue.vt == VT_NULL)
			m_oleValue.vt = VT_I4;

		BOOL bChanged = CXTPGridControlLocale::VariantChangeTypeEx(var, m_oleValue.vt, FALSE);

		if (bChanged && OnValueChanging(pItemArgs, &var))
		{
			m_oleValue = var;
		}
	}
	else
	{
		OnEditChanged(pItemArgs, pConstraint->m_strConstraint);
	}
}

DWORD CXTPGridRecordItemVariant::GetSelectedConstraintData(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	ASSERT(NULL != pItemArgs);

	if (GetEditOptions(pItemArgs->pColumn)->m_bConstraintEdit)
	{
		COleVariant var(m_oleValue);
		TRY
		{
			CXTPGridControlLocale::VariantChangeTypeEx(var, VT_I4);
		}
		CATCH_ALL(e)
		{
			return (DWORD)-1;
		}
		END_CATCH_ALL

		return XTPToDWORD(var.lVal);
	}
	else
	{
		return (DWORD)-1;
	}
}

int CXTPGridRecordItemVariant::CompareValues(const COleVariant& vt1, const COleVariant& vt2,
											 BOOL bCaseSensitive)
{
	ULONG dwFlags = ULONG(bCaseSensitive ? 0 : NORM_IGNORECASE);

	LCID lcidnSortLocale = static_cast<LCID>(m_nSortLocale);
	if (lcidnSortLocale == LOCALE_USER_DEFAULT)
	{
		lcidnSortLocale = CXTPGridControlLocale::GetActiveLCID();
	}

	return VarCmp(&const_cast<COleVariant&>(vt1), &const_cast<COleVariant&>(vt2), lcidnSortLocale,
				  dwFlags)
		   - VARCMP_EQ;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridRecordItemVariant, CXTPGridRecordItem)
	DISP_PROPERTY_ID(CXTPGridRecordItemVariant, "Value", 3, m_oleValue, VT_VARIANT)
END_DISPATCH_MAP()

#endif
