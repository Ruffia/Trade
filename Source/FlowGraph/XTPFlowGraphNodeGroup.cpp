// XTPFlowGraphNodeGroup.cpp : implementation of the XTPFlowGraphNodeGroup class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphNode.h"
#include "FlowGraph/XTPFlowGraphNodeGroup.h"
#include "FlowGraph/XTPFlowGraphNodeGroups.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)

CXTPFlowGraphNodeGroup::CXTPFlowGraphNodeGroup()
{
	m_pGroups = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphNodeGroup::~CXTPFlowGraphNodeGroup()
{
	RemoveAll();
}

void CXTPFlowGraphNodeGroup::RemoveAll()
{
	for (int i = 0; i < m_arrNodes.GetSize(); i++)
		m_arrNodes[i]->m_pGroup = NULL;

	m_arrNodes.RemoveAll();
}

void CXTPFlowGraphNodeGroup::Remove()
{
	if (!m_pGroups)
		return;

	m_pGroups->Remove(this);
}

void CXTPFlowGraphNodeGroup::AddNode(CXTPFlowGraphNode* pNode)
{
	if (!pNode)
		return;

	if (pNode->GetGroup() == this)
		return;

	if (pNode->GetGroup() != NULL)
	{
		pNode->GetGroup()->RemoveNode(pNode);
	}

	m_arrNodes.Add(pNode);
	pNode->m_pGroup = this;

	OnGraphChanged();
}

void CXTPFlowGraphNodeGroup::RemoveNode(CXTPFlowGraphNode* pNode)
{
	if (!pNode || pNode->GetGroup() != this)
		return;

	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i] == pNode)
		{
			m_arrNodes.RemoveAt(i);
			pNode->m_pGroup = NULL;

			OnGraphChanged();
			return;
		}
	}
}

CXTPFlowGraphControl* CXTPFlowGraphNodeGroup::GetControl() const
{
	if (m_pGroups == NULL)
		return NULL;

	return m_pGroups->GetPage()->GetControl();
}

CXTPFlowGraphPage* CXTPFlowGraphNodeGroup::GetPage() const
{
	if (m_pGroups == NULL)
		return NULL;

	return m_pGroups->GetPage();
}

void CXTPFlowGraphNodeGroup::SetCaption(LPCTSTR lpszCaption)
{
	INT_PTR nNodeCount = m_arrNodes.GetSize();
	for (INT_PTR nNode = 0; nNode < nNodeCount; ++nNode)
	{
		CXTPFlowGraphNode* pNode = m_arrNodes.GetAt(nNode);
		pNode->SetCaption(lpszCaption);
	}
}

CString CXTPFlowGraphNodeGroup::GetCaption() const
{
	CString strGroupCaption;
	INT_PTR nNodeCount = m_arrNodes.GetSize();
	for (INT_PTR nNode = 0; nNode < nNodeCount; ++nNode)
	{
		CXTPFlowGraphNode* pNode = m_arrNodes.GetAt(nNode);
		CString strNodeCaption   = pNode->GetCaption();
		if (0 == nNode)
		{
			strGroupCaption = strNodeCaption;
		}
		else if (strNodeCaption != strGroupCaption)
		{
			// Not all captions are the same in the group,
			// so the group has no common node caption.
			strGroupCaption.Empty();
			break;
		}
	}

	return strGroupCaption;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "Node", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeGroup, "Caption", DISPID_CAPTION, OleGetCaption,
						OleSetCaption, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeGroup, "Page", 11, OleGetPage, SetNotSupported,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "AddNode", 2, OleAddNode, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "RemoveNode", 3, OleRemoveNode, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)
	INTERFACE_PART(CXTPFlowGraphNodeGroup, XTPDIID_FlowGraphNodeGroup, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNodeGroup, XTPDIID_FlowGraphNodeGroup)

void CXTPFlowGraphNodeGroup::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(lpszCaption);
}

BSTR CXTPFlowGraphNodeGroup::OleGetCaption()
{
	return GetCaption().AllocSysString();
}

LPDISPATCH CXTPFlowGraphNodeGroup::OleGetPage()
{
	return XTPGetDispatch(GetPage());
}

LPDISPATCH CXTPFlowGraphNodeGroup::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphNodeGroup)

void CXTPFlowGraphNodeGroup::OleAddNode(LPDISPATCH Node)
{
	CXTPFlowGraphNode* pNode =
		XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPFlowGraphNode, CXTPFlowGraphNode::FromIDispatchSafe(Node));
	if (NULL == pNode)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	AddNode(pNode);
}

void CXTPFlowGraphNodeGroup::OleRemoveNode(LPDISPATCH Node)
{
	CXTPFlowGraphNode* pNode =
		XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPFlowGraphNode, CXTPFlowGraphNode::FromIDispatchSafe(Node));
	if (NULL == pNode)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	RemoveNode(pNode);
}

int CXTPFlowGraphNodeGroup::OleGetItemCount()
{
	return GetCount();
}

#endif
