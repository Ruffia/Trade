// XTPFlowGraphConnections.cpp : implementation of the CXTPFlowGraphConnections class.
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

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphConnection.h"
#include "FlowGraph/XTPFlowGraphConnections.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphUndoManager.h"
#include "FlowGraph/XTPFlowGraphMessages.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFlowGraphConnections::CXTPFlowGraphConnections(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphConnections::~CXTPFlowGraphConnections()
{
	RemoveAll();
}

void CXTPFlowGraphConnections::RemoveAll()
{
	if (m_arrConnections.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrConnections.GetSize(); i++)
	{
		CXTPFlowGraphConnection* pConnection = m_arrConnections[i];

		pConnection->OnRemoved();

		pConnection->m_pPage			= NULL;
		pConnection->m_nConnectionIndex = -1;

		pConnection->InternalRelease();
	}
	m_arrConnections.RemoveAll();

	m_pPage->OnGraphChanged();
}

CXTPFlowGraphConnection*
	CXTPFlowGraphConnections::AddConnection(CXTPFlowGraphConnection* pConnection)
{
	ASSERT(pConnection->m_nConnectionIndex == -1);
	if (pConnection->m_nConnectionIndex != -1)
		return pConnection;

	CXTPFlowGraphControl* pControl = m_pPage->GetControl();
	if (!pControl)
		return pConnection;

	XTP_NM_FLOWGRAPH_CONNECTIONCHANGED cc;
	cc.pConnection = pConnection;
	cc.nAction	 = 0;

	if (pControl->SendNotifyMessage(XTP_FGN_CONNECTIONCHANGED, &cc.hdr) == -1)
		return pConnection;

	pConnection->m_nConnectionIndex = (int)m_arrConnections.Add(pConnection);
	pConnection->m_pPage			= m_pPage;

	pControl->GetUndoManager()->AddUndoCommand(
		new CXTPFlowGraphUndoAddConnectionCommand(pConnection));

	m_pPage->OnGraphChanged();

	return pConnection;
}

void CXTPFlowGraphConnections::Remove(CXTPFlowGraphConnection* pConnection)
{
	if (pConnection->m_pPage != m_pPage)
		return;

	ASSERT(pConnection->m_nConnectionIndex != -1);
	ASSERT(m_arrConnections[pConnection->m_nConnectionIndex] == pConnection);

	RemoveAt(pConnection->m_nConnectionIndex);
}

void CXTPFlowGraphConnections::RemoveAt(int nIndex)
{
	CXTPFlowGraphConnection* pConnection = GetAt(nIndex);
	if (!pConnection)
		return;

	CXTPFlowGraphControl* pControl = m_pPage->GetControl();
	if (!pControl)
		return;

	XTP_NM_FLOWGRAPH_CONNECTIONCHANGED cc;
	cc.pConnection = pConnection;
	cc.nAction	 = 1;

	if (pControl->SendNotifyMessage(XTP_FGN_CONNECTIONCHANGED, &cc.hdr) == -1)
		return;

	pControl->GetUndoManager()->AddUndoCommand(
		new CXTPFlowGraphUndoDeleteConnectionCommand(pConnection));

	int nCount = (int)m_arrConnections.GetSize();

	if (nIndex != nCount - 1)
	{
		m_arrConnections[nIndex]					 = m_arrConnections[nCount - 1];
		m_arrConnections[nIndex]->m_nConnectionIndex = nIndex;
	}

	m_arrConnections.SetSize(nCount - 1);

	pConnection->OnRemoved();

	pConnection->m_nConnectionIndex = -1;
	pConnection->m_pPage			= NULL;
	pConnection->InternalRelease();

	m_pPage->OnGraphChanged();
}

void CXTPFlowGraphConnections::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Connection")));

	if (pPX->IsStoring())
	{
		int nCount   = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphConnection* pConnection = GetAt(i);
			ASSERT(pConnection);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pConnection, RUNTIME_CLASS(CXTPFlowGraphConnection));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPFlowGraphConnection* pConnection = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pConnection, RUNTIME_CLASS(CXTPFlowGraphConnection));

			if (!pConnection)
				AfxThrowArchiveException(CArchiveException::badClass);

			pConnection->m_pPage			= m_pPage;
			pConnection->m_nConnectionIndex = (int)m_arrConnections.Add(pConnection);

			pConnection->RestoreConnection();
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphConnections, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphConnections, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphConnections, "Connection", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphConnections, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphConnections, "AddConnection", 2, OleAddConnection, VT_DISPATCH,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphConnections, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphConnections, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphConnections, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphConnections, XTPDIID_FlowGraphConnections, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphConnections, XTPDIID_FlowGraphConnections)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphConnections)

LPDISPATCH CXTPFlowGraphConnections::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphConnections::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphConnections::OleAddConnection()
{
	return XTPGetDispatch(AddConnection(new CXTPFlowGraphConnection()));
}

#endif
