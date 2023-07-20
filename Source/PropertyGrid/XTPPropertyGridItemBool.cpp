// CXTPPropertyGridItemBool.cpp : implementation of the XTPPropertyGridItemBool class.
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

#include "PropertyGrid/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridItemBool.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridView.h"
#include "PropertyGrid/XTPPropertyGridIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemBool

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemBool, CXTPPropertyGridItem)

CXTPPropertyGridItemBool::CXTPPropertyGridItemBool(LPCTSTR strCaption, BOOL bValue, BOOL* pBindBool)
	: CXTPPropertyGridItem(strCaption)
	, m_strTrueText(_T("True"))
	, m_strFalseText(_T("False"))
{
	m_pBindBool = pBindBool;
	_Init(bValue);
}

CXTPPropertyGridItemBool::CXTPPropertyGridItemBool(UINT nID, BOOL bValue, BOOL* pBindBool)
	: CXTPPropertyGridItem(nID)
	, m_strTrueText(_T("True"))
	, m_strFalseText(_T("False"))
{
	m_pBindBool = pBindBool;
	_Init(bValue);
}

CXTPPropertyGridItemBool::~CXTPPropertyGridItemBool()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemBool::_Init(BOOL bValue)
{
	m_nFlags = xtpPropertyGridItemHasComboButton | xtpPropertyGridItemHasEdit;

	CString strTrueFalse;
	if (XTPResourceManager()->LoadString(&strTrueFalse, XTP_IDS_PROPERTYGRID_TRUEFALSE)
		&& (strTrueFalse.Find(_T('\n')) != -1))
	{
		AfxExtractSubString(m_strTrueText, strTrueFalse, 0);
		AfxExtractSubString(m_strFalseText, strTrueFalse, 1);
	}
	SetBool(bValue);

	m_pConstraints->AddConstraint(m_strTrueText);
	m_pConstraints->AddConstraint(m_strFalseText);

	SetConstraintEdit(TRUE);
	m_strDefaultValue = m_strValue;

	m_bCheckBoxStyle = FALSE;
}

void CXTPPropertyGridItemBool::SetValue(CString strValue)
{
	SetBool(strValue.CompareNoCase(m_strTrueText) == 0);
}

void CXTPPropertyGridItemBool::SetBool(BOOL bValue)
{
	m_bValue = bValue;

	if (m_pBindBool)
	{
		*m_pBindBool = bValue;
	}

	CXTPPropertyGridItem::SetValue(bValue ? m_strTrueText : m_strFalseText);
}

void CXTPPropertyGridItemBool::BindToBool(BOOL* pBindBool)
{
	m_pBindBool = pBindBool;
	if (m_pBindBool)
	{
		*m_pBindBool = m_bValue;
	}
}

void CXTPPropertyGridItemBool::OnBeforeInsert()
{
	if (m_pBindBool && *m_pBindBool != m_bValue)
	{
		SetBool(*m_pBindBool);
	}
}

BOOL CXTPPropertyGridItemBool::SetValueText(CString& strValueText, LPCTSTR strNewText)
{
	// see if the value exists.
	int iIndex = m_pConstraints->FindConstraint(strValueText);
	if (iIndex != -1)
	{
		// if this is the current value change it as well.
		if (GetValue() == strValueText)
		{
			CXTPPropertyGridItem::SetValue(strNewText);
		}

		// update the value.
		strValueText											 = strNewText;
		m_pConstraints->GetConstraintAt(iIndex)->m_strConstraint = strValueText;
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPPropertyGridItemBool::SetTrueFalseText(LPCTSTR strTrueText, LPCTSTR strFalseText)
{
	// update the "True" value text
	if (!SetValueText(m_strTrueText, strTrueText))
		return FALSE;

	// update the "False" value text
	if (!SetValueText(m_strFalseText, strFalseText))
		return FALSE;

	return TRUE;
}

void CXTPPropertyGridItemBool::SetCheckBoxStyle(BOOL bCheckBoxStyle /*= TRUE*/)
{
	m_bCheckBoxStyle = bCheckBoxStyle;
	SetFlags(UINT(
		m_bCheckBoxStyle ? 0 : xtpPropertyGridItemHasComboButton | xtpPropertyGridItemHasEdit));
}

BOOL CXTPPropertyGridItemBool::IsCheckBoxStyle() const
{
	return m_bCheckBoxStyle;
}

CString CXTPPropertyGridItemBool::GetViewValue()
{
	return m_bCheckBoxStyle ? CString() : CXTPPropertyGridItem::GetViewValue();
}

BOOL CXTPPropertyGridItemBool::OnKeyDown(UINT nChar)
{
	if (m_bCheckBoxStyle && IsAllowEdit() && (nChar == VK_SPACE))
	{
		CRect rc = GetValueRect();
		OnLButtonDblClk(0, rc.CenterPoint());

		return TRUE;
	}
	return FALSE;
}

void CXTPPropertyGridItemBool::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_bCheckBoxStyle && PtInValueRect(point) && IsAllowEdit())
	{
		OnSelect();
		SelectNextConstraint();
	}
	else
	{
		CXTPPropertyGridItem::OnLButtonDblClk(nFlags, point);
	}
}

BOOL CXTPPropertyGridItemBool::PtInCheckBoxRect(CPoint point)
{
	CRect rc = GetValueRect();
	rc.right = rc.left + XTP_DPI_X(15);

	return rc.PtInRect(point);
}

BOOL CXTPPropertyGridItemBool::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!CXTPPropertyGridItem::OnLButtonDown(nFlags, point))
		return FALSE;

	if (m_bCheckBoxStyle)
	{
		if (PtInCheckBoxRect(point) && IsAllowEdit())
		{
			OnLButtonDblClk(nFlags, point);
		}
	}

	return TRUE;
}

BOOL CXTPPropertyGridItemBool::OnDrawItemValue(CDC& dc, CRect /*rcValue*/)
{
	if (!m_bCheckBoxStyle)
		return FALSE;

	CXTPPropertyGridPaintManager* pPaintManager = m_pGrid->GetPaintManager();
	pPaintManager->DrawCheckBox(&dc, this);

	return TRUE;
}

#ifdef _XTP_ACTIVEX

void CXTPPropertyGridItemBool::OleSetValue(const VARIANT* varValue)
{
	SetBool(VariantToBool(varValue));
}

const VARIANT CXTPPropertyGridItemBool::OleGetValue()
{
	return CComVariant(m_bValue ? true : false);
}

void CXTPPropertyGridItemBool::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_BOOL, m_bValue);
	}
}

#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemEnum

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemEnum, CXTPPropertyGridItem)

CXTPPropertyGridItemEnum::CXTPPropertyGridItemEnum(LPCTSTR strCaption, int nValue, int* pBindEnum)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindEnum = pBindEnum;
	_Init(nValue);
}

CXTPPropertyGridItemEnum::CXTPPropertyGridItemEnum(UINT nID, int nValue, int* pBindEnum)
	: CXTPPropertyGridItem(nID)
{
	m_pBindEnum = pBindEnum;
	_Init(nValue);
}

CXTPPropertyGridItemEnum::~CXTPPropertyGridItemEnum()
{
}

void CXTPPropertyGridItemEnum::_Init(int nValue)
{
	SetEnum(nValue);
	m_nFlags = xtpPropertyGridItemHasComboButton | xtpPropertyGridItemHasEdit;

	SetConstraintEdit(TRUE);
	m_strDefaultValue = m_strValue;
}

void CXTPPropertyGridItemEnum::SetValue(CString strValue)
{
	int nIndex = m_pConstraints->FindConstraint(strValue);
	ASSERT(nIndex >= 0);
	if (nIndex >= 0)
	{
		SetEnum(m_pConstraints->GetConstraintAt(nIndex));
	}
}

void CXTPPropertyGridItemEnum::SetEnum(int nValue)
{
	m_nValue = nValue;

	if (m_pBindEnum)
	{
		*m_pBindEnum = nValue;
	}

	int nIndex = m_pConstraints->FindConstraint(static_cast<DWORD_PTR>(nValue));
	CXTPPropertyGridItem::SetValue(m_pConstraints->GetAt(nIndex));
}

void CXTPPropertyGridItemEnum::SetEnum(CXTPPropertyGridItemConstraint* pContraint)
{
	m_nValue = static_cast<int>(pContraint->m_dwData);

	if (m_pBindEnum)
	{
		*m_pBindEnum = m_nValue;
	}

	CXTPPropertyGridItem::SetValue(pContraint->m_strConstraint);
}

void CXTPPropertyGridItemEnum::BindToEnum(int* pBindEnum)
{
	m_pBindEnum = pBindEnum;
	if (m_pBindEnum)
	{
		*m_pBindEnum = m_nValue;
	}
}

void CXTPPropertyGridItemEnum::OnBeforeInsert()
{
	if (m_pBindEnum && *m_pBindEnum != m_nValue)
	{
		SetEnum(*m_pBindEnum);
	}
}

void CXTPPropertyGridItemEnum::OnConstraintsChanged()
{
	if (m_strValue.IsEmpty())
	{
		int nIndex = m_pConstraints->FindConstraint(static_cast<DWORD_PTR>(m_nValue));
		if (nIndex != -1)
			m_strDefaultValue = m_strValue = m_pConstraints->GetAt(nIndex);
	}
}

#ifdef _XTP_ACTIVEX

void CXTPPropertyGridItemEnum::OleSetValue(const VARIANT* varValue)
{
	SetEnum(VariantToLong(varValue));
}

const VARIANT CXTPPropertyGridItemEnum::OleGetValue()
{
	return CComVariant(m_nValue);
}

void CXTPPropertyGridItemEnum::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_I4, m_nValue);
	}
}

#endif

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPPropertyGridItemBool, CXTPPropertyGridItem)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemBool, "CheckBoxStyle", 50, IsCheckBoxStyle,
						SetCheckBoxStyle, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridItemBool, CXTPPropertyGridItem)
	INTERFACE_PART(CXTPPropertyGridItemBool, XTPDIID_IPropertyGridItemBool, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemBool, XTPDIID_IPropertyGridItemBool)

#endif
