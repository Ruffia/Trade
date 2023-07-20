// XTPGridRecordItemEditOptions.cpp : implementation of the CXTPGridRecordItemEditOptions class.
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
#include "Common/XTPCustomHeap.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridRecordItemConstraint.h"
#include "GridControl/XTPGridInplaceControls.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemEditOptions

CXTPGridRecordItemEditOptions::CXTPGridRecordItemEditOptions()
{
	m_bAllowEdit		= TRUE;
	m_bConstraintEdit   = FALSE;
	m_pConstraints		= new CXTPGridRecordItemConstraints();
	m_bSelectTextOnEdit = FALSE;
	m_bScrollTextOnEdit = TRUE;
	m_bExpandOnSelect   = FALSE;
	m_dwEditStyle		= ES_AUTOHSCROLL;
	m_nMaxLength		= 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridRecordItemEditOptions::~CXTPGridRecordItemEditOptions()
{
	RemoveButtons();

	CMDTARGET_RELEASE(m_pConstraints);
}

void CXTPGridRecordItemEditOptions::RemoveButtons()
{
	for (int j = 0; j < arrInplaceButtons.GetSize(); j++)
		if (arrInplaceButtons[j] != NULL)
			arrInplaceButtons[j]->InternalRelease();

	arrInplaceButtons.RemoveAll();
}

CXTPGridInplaceButton* CXTPGridRecordItemEditOptions::AddComboButton(BOOL bInside)
{
	CXTPGridInplaceButton* pButton = new CXTPGridInplaceButton(XTP_ID_GRID_COMBOBUTTON);
	if (pButton)
	{
		arrInplaceButtons.Add(pButton);
		pButton->SetInsideCellButton(bInside);
	}
	return pButton;
}

CXTPGridInplaceButton* CXTPGridRecordItemEditOptions::AddExpandButton(BOOL bInside)
{
	CXTPGridInplaceButton* pButton = new CXTPGridInplaceButton(XTP_ID_GRID_EXPANDBUTTON);
	if (pButton)
	{
		arrInplaceButtons.Add(pButton);
		pButton->SetInsideCellButton(bInside);
	}
	return pButton;
}

CXTPGridInplaceButton* CXTPGridRecordItemEditOptions::AddSpinButton(BOOL bInside)
{
	CXTPGridInplaceButton* pButton = new CXTPGridInplaceButton(XTP_ID_GRID_SPINBUTTON);
	if (pButton)
	{
		arrInplaceButtons.Add(pButton);
		pButton->SetInsideCellButton(bInside);
	}
	return pButton;
}

CXTPGridInplaceButton* CXTPGridRecordItemEditOptions::GetButton(int nIndex)
{
	if (nIndex >= arrInplaceButtons.GetSize())
		return NULL;
	return arrInplaceButtons.GetAt(nIndex);
}

CXTPGridRecordItemConstraint* CXTPGridRecordItemEditOptions::FindConstraint(DWORD_PTR dwData)
{
	for (int i = 0; i < m_pConstraints->GetCount(); i++)
	{
		CXTPGridRecordItemConstraint* pConstraint = m_pConstraints->GetAt(i);
		if (pConstraint->m_dwData == dwData)
			return pConstraint;
	}
	return NULL;
}

CXTPGridRecordItemConstraint* CXTPGridRecordItemEditOptions::FindConstraint(LPCTSTR lpszConstraint)
{
	for (int i = 0; i < m_pConstraints->GetCount(); i++)
	{
		CXTPGridRecordItemConstraint* pConstraint = m_pConstraints->GetAt(i);
		if (pConstraint->m_strConstraint == lpszConstraint)
			return pConstraint;
	}
	return NULL;
}

CXTPGridRecordItemConstraint* CXTPGridRecordItemEditOptions::AddConstraint(LPCTSTR lpszConstraint,
																		   DWORD_PTR dwData /*= 0*/)
{
	CXTPGridRecordItemConstraint* pConstraint = new CXTPGridRecordItemConstraint();

	pConstraint->m_strConstraint = lpszConstraint;
	pConstraint->m_dwData		 = dwData;
	pConstraint->m_nIndex		 = (int)m_pConstraints->m_arrConstraints.Add(pConstraint);

	return pConstraint;
}

#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemEditOptions

BEGIN_DISPATCH_MAP(CXTPGridRecordItemEditOptions, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "Constraints", 1, OleGetConstraints,
					 VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_ID(CXTPGridRecordItemEditOptions, "ConstraintEdit", 2, m_bConstraintEdit, VT_BOOL)
	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "AddComboButton", 3, OleAddComboButton,
					 VT_DISPATCH, VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "AddExpandButton", 4, OleAddExpandButton,
					 VT_DISPATCH, VTS_VARIANT)
	DISP_PROPERTY_ID(CXTPGridRecordItemEditOptions, "AllowEdit", 5, m_bAllowEdit, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItemEditOptions, "SelectTextOnEdit", 6, m_bSelectTextOnEdit,
					 VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItemEditOptions, "EditControlStyle", 7, m_dwEditStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPGridRecordItemEditOptions, "MaxLength", 8, m_nMaxLength, VT_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "RemoveButtons", 9, RemoveButtons, VT_EMPTY,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "GetInplaceButton", 10, OleGetInplaceButton,
					 VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "InplaceButtonsCount", 11,
					 OleInplaceButtonsCount, VT_I4, VTS_NONE)

	DISP_FUNCTION_ID(CXTPGridRecordItemEditOptions, "AddSpinButton", 12, OleAddSpinButton,
					 VT_DISPATCH, VTS_VARIANT)

	DISP_PROPERTY_ID(CXTPGridRecordItemEditOptions, "ExpandOnSelect", 13, m_bExpandOnSelect,
					 VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItemEditOptions, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecordItemEditOptions, XTPDIID_IGridRecordItemEditOptions, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItemEditOptions, XTPDIID_IGridRecordItemEditOptions)

LPDISPATCH CXTPGridRecordItemEditOptions::OleGetConstraints()
{
	return m_pConstraints->GetIDispatch(TRUE);
}

LPDISPATCH CXTPGridRecordItemEditOptions::OleGetInplaceButton(long nIndex)
{
	INT_PTR nCount = arrInplaceButtons.GetSize();
	if (nIndex < nCount && nCount >= 0)
	{
		return arrInplaceButtons[nIndex] ? arrInplaceButtons[nIndex]->GetIDispatch(TRUE) : NULL;
	}
	return NULL;
}

long CXTPGridRecordItemEditOptions::OleInplaceButtonsCount()
{
	return static_cast<long>(arrInplaceButtons.GetSize());
}

LPDISPATCH CXTPGridRecordItemEditOptions::OleAddComboButton(const VARIANT& bInside)
{
	BOOL bIn = VariantToBool(&bInside);
	return XTPGetDispatch(AddComboButton(bIn));
}

LPDISPATCH CXTPGridRecordItemEditOptions::OleAddExpandButton(const VARIANT& bInside)
{
	BOOL bIn = VariantToBool(&bInside);
	return XTPGetDispatch(AddExpandButton(bIn));
}

LPDISPATCH CXTPGridRecordItemEditOptions::OleAddSpinButton(const VARIANT& bInside)
{
	BOOL bIn = VariantToBool(&bInside);
	return XTPGetDispatch(AddSpinButton(bIn));
}

#endif // _XTP_ACTIVEX
