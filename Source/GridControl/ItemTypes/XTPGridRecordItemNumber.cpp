// XTPGridRecordItemNumber.cpp : implementation of the CXTPGridRecordItemNumber class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/ItemTypes/XTPGridRecordItemNumber.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemNumber

IMPLEMENT_SERIAL(CXTPGridRecordItemNumber, CXTPGridRecordItem,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemNumber::CXTPGridRecordItemNumber(double dValue)
	: CXTPGridRecordItem()
	, m_dValue(dValue)
{
	// To avoid new string data allocation for each record
	static const CString cstrNumberFormatDefault(_T("%0.f"));
	m_strFormatString = cstrNumberFormatDefault;
}

CXTPGridRecordItemNumber::CXTPGridRecordItemNumber(double dValue, LPCTSTR strFormat)
	: CXTPGridRecordItem()
	, m_dValue(dValue)
{
	m_strFormatString = strFormat;
}

CString CXTPGridRecordItemNumber::GetCaption(CXTPGridColumn* pColumn)
{
	if (IsChildOfMerge())
		return GetMergeItem()->GetCaption(pColumn);

	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	CString strCaption;
	strCaption.Format(m_strFormatString, m_dValue);
	return strCaption;
}

int CXTPGridRecordItemNumber::Compare(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem)
{
	ASSERT_VALID(pItem);
	UNREFERENCED_PARAMETER(pColumn);

	CXTPGridRecordItemNumber* pItemNumber = DYNAMIC_DOWNCAST(CXTPGridRecordItemNumber, pItem);
	if (!pItemNumber)
		return 0;

	return CompareValues(m_dValue, pItemNumber->m_dValue);
}

COleVariant CXTPGridRecordItemNumber::GetGroupCaptionValue(CXTPGridColumn* pColumn)
{
	UNREFERENCED_PARAMETER(pColumn);
	return m_dValue;
}

int CXTPGridRecordItemNumber::CompareGroupCaption(CXTPGridColumn* pColumn,
												  CXTPGridGroupRow* pGroupRow)
{
	ASSERT_VALID(pGroupRow);
	UNREFERENCED_PARAMETER(pColumn);

	if (!m_strGroupCaption.IsEmpty())
		return m_pRecord->GetRecords()->Compare(m_strGroupCaption, pGroupRow->GetCaption());

	COleVariant vtGroupCaptionValue = pGroupRow->GetCaptionValue();
	if (!SUCCEEDED(::VariantChangeType(&vtGroupCaptionValue, &vtGroupCaptionValue, 0, VT_R8)))
		return 0;

	return CompareValues(m_dValue, vtGroupCaptionValue.dblVal);
}

void CXTPGridRecordItemNumber::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	UNREFERENCED_PARAMETER(pItemArgs);
	SetValue(StringToDouble(szText));
}

void CXTPGridRecordItemNumber::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPGridRecordItem::DoPropExchange(pPX);

	PX_Double(pPX, _T("Value"), m_dValue);
}

int CXTPGridRecordItemNumber::CompareValues(double v1, double v2)
{
	if (v1 == v2)
		return 0;
	else if (v1 > v2)
		return 1;
	else
		return -1;
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemPercentNumber

IMPLEMENT_SERIAL(CXTPGridRecordItemPercentNumber, CXTPGridRecordItemNumber,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItemPercentNumber::CXTPGridRecordItemPercentNumber(double dValue, COLORREF clr,
																 BOOL bPercentCompleteDisplay)
	: CXTPGridRecordItemNumber(dValue)
{
	m_strFormatString		  = _T("%2.0f%%");
	m_clr					  = clr;
	m_bPercentCompleteDisplay = bPercentCompleteDisplay;
}

void CXTPGridRecordItemPercentNumber::OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
													XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	ASSERT(NULL != pDrawArgs);
	ASSERT(NULL != pMetrics);
	ASSERT(pDrawArgs->pItem == this);

	CString sTxt = pMetrics->strText;

	if (sTxt.Find('%') > -1)
	{
		sTxt.Replace(_T("%"), _T(""));
		int iTxt = _ttoi(sTxt);

		if (m_bPercentCompleteDisplay)
		{
			iTxt = max(0, iTxt);
			iTxt = min(100, iTxt);
			pMetrics->strText.Format(_T("%d"), iTxt);

			CDC* pDC = pDrawArgs->pDC;
			if (pDC)
			{
				CRect rc = pDrawArgs->rcItem;
				rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
				int W = rc.Width();

				if (pMetrics->nColumnAlignment == xtpColumnTextLeft)
					rc.right = rc.left + W * iTxt / 100;
				else if (pMetrics->nColumnAlignment == xtpColumnTextRight)
					rc.left = rc.right - W * iTxt / 100;
				else if (pMetrics->nColumnAlignment == xtpColumnTextCenter)
				{
					rc.left += W * (100 - iTxt) / 200;
					rc.right -= W * (100 - iTxt) / 200;
				}

				if (pDrawArgs->pControl && pDrawArgs->pControl->GetPaintManager()
					&& pDrawArgs->pControl->GetPaintManager()->m_bShowNonActiveInPlaceButton)
					rc.right -= rc.Height();

				pDC->FillSolidRect(rc, m_clr);
			}
		}
	}
	if (m_pMarkupUIElement)
	{
		CRect rcItem = pDrawArgs->rcItem;
		rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0);

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement),
								(HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);

		XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width(), INT_MAX);
		XTPMarkupRenderElement(m_pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
	}
	else
	{
		pDrawArgs->pControl->GetPaintManager()->DrawItemCaption(pDrawArgs, pMetrics);
	}
}
