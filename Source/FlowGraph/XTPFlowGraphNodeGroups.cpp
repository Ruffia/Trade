// XTPFlowGraphNodeGroups.cpp : implementation of the CXTPFlowGraphNodeGroups class.
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

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphNodeGroup.h"
#include "FlowGraph/XTPFlowGraphNodeGroups.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFlowGraphNodeGroups::CXTPFlowGraphNodeGroups(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphNodeGroups::~CXTPFlowGraphNodeGroups()
{
	RemoveAll();
}

void CXTPFlowGraphNodeGroups::RemoveAll()
{
	if (m_arrGroups.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrGroups.GetSize(); i++)
	{
		m_arrGroups[i]->RemoveAll();
		m_arrGroups[i]->InternalRelease();
	}
	m_arrGroups.RemoveAll();
}

void CXTPFlowGraphNodeGroups::Remove(CXTPFlowGraphNodeGroup* pGroup)
{
	for (int i = 0; i < m_arrGroups.GetSize(); i++)
	{
		if (m_arrGroups[i] == pGroup)
		{
			pGroup->RemoveAll();
			pGroup->InternalRelease();
			m_arrGroups.RemoveAt(i);
			return;
		}
	}
}

CXTPFlowGraphNodeGroup* CXTPFlowGraphNodeGroups::AddGroup()
{
	CXTPFlowGraphNodeGroup* pGroup = new CXTPFlowGraphNodeGroup();
	pGroup->m_pGroups			   = this;

	m_arrGroups.Add(pGroup);

	return pGroup;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphNodeGroups, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroups, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroups, "Group", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroups, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroups, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroups, "AddGroup", 2, OleAddGroup, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroups, "RemoveGroup", 4, OleRemoveGroup, VT_EMPTY,
					 VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphNodeGroups, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphNodeGroups, XTPDIID_FlowGraphNodeGroups, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNodeGroups, XTPDIID_FlowGraphNodeGroups)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphNodeGroups)

LPDISPATCH CXTPFlowGraphNodeGroups::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphNodeGroups::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphNodeGroups::OleAddGroup()
{
	return XTPGetDispatch(AddGroup());
}

void CXTPFlowGraphNodeGroups::OleRemoveGroup(LPDISPATCH Group)
{
	CXTPFlowGraphNodeGroup* pGroup = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPFlowGraphNodeGroup, CXTPFlowGraphNodeGroup::FromIDispatchSafe(Group));
	if (NULL == pGroup)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	Remove(pGroup);
}

#endif
