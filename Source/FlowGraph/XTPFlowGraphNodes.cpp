// XTPFlowGraphNodes.cpp : implementation of the CXTPFlowGraphNodes class.
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

#include "Common/XTPFramework.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphNode.h"
#include "FlowGraph/XTPFlowGraphNodeCustom.h"
#include "FlowGraph/XTPFlowGraphNodes.h"
#include "FlowGraph/XTPFlowGraphConnections.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphUndoManager.h"
#include "FlowGraph/XTPFlowGraphSelectedElements.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFlowGraphNodes::CXTPFlowGraphNodes(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphNodes::~CXTPFlowGraphNodes()
{
	RemoveAll();
}

void CXTPFlowGraphNodes::RemoveAll()
{
	if (m_arrNodes.GetSize() == 0)
		return;

	CXTPFlowGraphUndoManager* pUndoManager = m_pPage->GetControl()->GetUndoManager();

	pUndoManager->StartGroup();

	m_pPage->GetSelection()->Clear();
	m_pPage->GetConnections()->RemoveAll();

	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		CXTPFlowGraphNode* pNode = m_arrNodes[i];

		pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteNodeCommand(pNode));

		pNode->OnRemoved();
		pNode->m_pPage = NULL;
		pNode->InternalRelease();
	}
	m_arrNodes.RemoveAll();

	pUndoManager->EndGroup();

	m_pPage->OnGraphChanged();
}

CXTPFlowGraphNode* CXTPFlowGraphNodes::AddNode(CXTPFlowGraphNode* pNode)
{
	m_arrNodes.Add(pNode);
	pNode->m_pPage = m_pPage;

	pNode->SetDefaultMarkup();

	m_pPage->GetControl()->GetUndoManager()->AddUndoCommand(
		new CXTPFlowGraphUndoAddNodeCommand(pNode));

	m_pPage->OnGraphChanged();

	return pNode;
}

CXTPFlowGraphNodeCustom*
	CXTPFlowGraphNodes::AddCustomNode(CXTPFlowGraphNodeCustom* pNode /*= NULL */)
{
	if (!pNode)
		pNode = new CXTPFlowGraphNodeCustom();

	return static_cast<CXTPFlowGraphNodeCustom*>(AddNode(pNode));
}

CXTPFlowGraphNodeCustom* CXTPFlowGraphNodes::AddResourceNode(UINT uResShapeID)
{
	if (m_pPage)
		return m_pPage->GetControl()->AddCustomNodeFromToolkitResource(uResShapeID);

	return NULL;
}

void CXTPFlowGraphNodes::MoveToFront(CXTPFlowGraphNode* pNode)
{
	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i] == pNode)
		{
			m_arrNodes.RemoveAt(i);
			m_arrNodes.Add(pNode);

			m_pPage->OnGraphChanged();
			return;
		}
	}
}

void CXTPFlowGraphNodes::Remove(CXTPFlowGraphNode* pNode)
{
	if (pNode->GetPage() != m_pPage)
		return;

	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i] == pNode)
		{
			m_pPage->GetSelection()->Remove(pNode); // should delete from selection first

			RemoveAt(i);

			return;
		}
	}
}

CXTPFlowGraphNode* CXTPFlowGraphNodes::FindNode(int nId) const
{
	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i]->GetID() == nId)
			return m_arrNodes[i];
	}
	return NULL;
}

void CXTPFlowGraphNodes::RemoveAt(int nIndex)
{
	CXTPFlowGraphNode* pNode = GetAt(nIndex);
	if (!pNode)
		return;

	CXTPFlowGraphUndoManager* pUndoManager = m_pPage->GetControl()->GetUndoManager();

	pUndoManager->StartGroup();

	m_arrNodes.RemoveAt(nIndex);
	pNode->OnRemoved();

	pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteNodeCommand(pNode));
	pUndoManager->EndGroup();

	pNode->m_pPage = NULL;
	pNode->InternalRelease();

	m_pPage->OnGraphChanged();
}

void CXTPFlowGraphNodes::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Node")));

	if (pPX->IsStoring())
	{
		int nCount   = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphNode* pNode = GetAt(i);
			ASSERT(pNode);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pNode, RUNTIME_CLASS(CXTPFlowGraphNode));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPFlowGraphNode* pNode = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));

			if (!sec->ExchangeObjectInstance((CObject*&)pNode, RUNTIME_CLASS(CXTPFlowGraphNode)))
				AfxThrowArchiveException(CArchiveException::badClass);

			pNode->m_pPage = m_pPage;

			pNode->SetDefaultMarkup();

			pNode->DoPropExchange(&sec);

			m_arrNodes.Add(pNode);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphNodes, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "Node", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "AddNode", 2, OleAddNode, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "FindNode", 5, OleFindNode, VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "AddCustomNode", 10, OleAddCustomNode, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "AddResourceNode", 11, OleAddResourceNode, VT_DISPATCH,
					 VTS_I4)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphNodes, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphNodes, XTPDIID_FlowGraphNodes, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNodes, XTPDIID_FlowGraphNodes)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphNodes)

LPDISPATCH CXTPFlowGraphNodes::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphNodes::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphNodes::OleAddNode()
{
	return XTPGetDispatch(AddNode(new CXTPFlowGraphNode()));
}

LPDISPATCH CXTPFlowGraphNodes::OleAddCustomNode()
{
	return XTPGetDispatch(AddCustomNode(new CXTPFlowGraphNodeCustom()));
};

LPDISPATCH CXTPFlowGraphNodes::OleAddResourceNode(int nResShapeID)
{
	return XTPGetDispatch(AddResourceNode(nResShapeID));
};

LPDISPATCH CXTPFlowGraphNodes::OleFindNode(int nId)
{
	return XTPGetDispatch(FindNode(nId));
}

#endif
