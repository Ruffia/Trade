// XTPGridRecordItemConstraint.cpp : implementation of the CXTPGridRecordItemConstraint class.
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

#include "GridControl/XTPGridRecordItemConstraint.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemConstraint

IMPLEMENT_DYNAMIC(CXTPGridRecordItemConstraint, CXTPCmdTarget);

CXTPGridRecordItemConstraint::CXTPGridRecordItemConstraint()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_dwData = 0;
	m_nIndex = 0;
}

int CXTPGridRecordItemConstraint::GetIndex() const
{
	return m_nIndex;
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemConstraints

CXTPGridRecordItemConstraints::CXTPGridRecordItemConstraints()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridRecordItemConstraints::~CXTPGridRecordItemConstraints()
{
	RemoveAll();
}

int CXTPGridRecordItemConstraints::GetCount() const
{
	return (int)m_arrConstraints.GetSize();
}

CXTPGridRecordItemConstraint* CXTPGridRecordItemConstraints::GetAt(int nIndex) const
{
	return m_arrConstraints.GetAt(nIndex);
}

void CXTPGridRecordItemConstraints::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrConstraints[i] != NULL)
			m_arrConstraints[i]->InternalRelease();
	}
	m_arrConstraints.RemoveAll();
}

#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemConstraint

BEGIN_DISPATCH_MAP(CXTPGridRecordItemConstraint, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPGridRecordItemConstraint, "Caption", 1, m_strConstraint, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridRecordItemConstraint, "Data", 2, m_dwData, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemConstraint, "Index", 3, GetIndex, SetNotSupported, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItemConstraint, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecordItemConstraint, XTPDIID_IGridRecordItemConstraint, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItemConstraint, XTPDIID_IGridRecordItemConstraint)

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemConstraints

BEGIN_DISPATCH_MAP(CXTPGridRecordItemConstraints, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridRecordItemConstraints, "Count", dispidCount, OleGetItemCount, VT_I4,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecordItemConstraints, "Constraint", DISPID_VALUE, OleGetItem,
					 VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemConstraints, "_NewEnum", DISPID_NEWENUM, OleNewEnum,
					 VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecordItemConstraints, "Add", dispidAdd, OleAdd, VT_EMPTY,
					 VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemConstraints, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItemConstraints, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecordItemConstraints, XTPDIID_IGridRecordItemConstraints, Dispatch)
	// INTERFACE_PART(CXTPGridRecordItemConstraints, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItemConstraints, XTPDIID_IGridRecordItemConstraints)

int CXTPGridRecordItemConstraints::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPGridRecordItemConstraints::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANT(CXTPGridRecordItemConstraints)

void CXTPGridRecordItemConstraints::OleAdd(LPCTSTR lpszCaption, long dwData)
{
	CXTPGridRecordItemConstraint* pConstraint = new CXTPGridRecordItemConstraint();
	pConstraint->m_strConstraint			  = lpszCaption;
	pConstraint->m_dwData					  = dwData;
	pConstraint->m_nIndex					  = GetCount();

	m_arrConstraints.Add(pConstraint);
}

#endif
