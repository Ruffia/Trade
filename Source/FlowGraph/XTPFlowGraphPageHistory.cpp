// XTPFlowGraphPageHistory.cpp : implementation of the CXTPFlowGraphPageHistory class.
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
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPSystemHelpers.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphNode.h"
#include "FlowGraph/XTPFlowGraphPageHistory.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPFlowGraphPageHistory::CXTPFlowGraphPageHistory(CXTPFlowGraphControl* pControl)
{
	m_pControl = pControl;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphPageHistory::~CXTPFlowGraphPageHistory()
{
}

BOOL CXTPFlowGraphPageHistory::CanGoBackward() const
{
	return m_arrPageUndo.GetSize() > 0;
}

BOOL CXTPFlowGraphPageHistory::CanGoForward() const
{
	return m_arrPageRedo.GetSize() > 0;
}

void CXTPFlowGraphPageHistory::AnimatePageTransition(CXTPFlowGraphNode* pNodeFrom,
													 CXTPFlowGraphNode* pNodeTo)
{
	ASSERT(pNodeFrom && pNodeTo && pNodeFrom->GetPage() != pNodeTo->GetPage());

	if (!pNodeFrom || !pNodeTo)
		return;

	CXTPFlowGraphPageHistoryItem hi = { pNodeFrom, pNodeTo };

	m_arrPageUndo.Add(hi);
	m_arrPageRedo.RemoveAll();

	m_pControl->AnimatePageTransition(pNodeFrom, pNodeTo);
};

void CXTPFlowGraphPageHistory::GoBackward()
{
	if (m_arrPageUndo.GetSize() == 0)
		return;

	CXTPFlowGraphPageHistoryItem hi = m_arrPageUndo[m_arrPageUndo.GetSize() - 1];
	m_arrPageUndo.RemoveAt(m_arrPageUndo.GetSize() - 1);

	m_arrPageRedo.Add(hi);

	m_pControl->AnimatePageTransition(hi.pTo, hi.pFrom);
}

void CXTPFlowGraphPageHistory::GoForward()
{
	if (m_arrPageRedo.GetSize() == 0)
		return;

	CXTPFlowGraphPageHistoryItem hi = m_arrPageRedo[m_arrPageRedo.GetSize() - 1];
	m_arrPageRedo.RemoveAt(m_arrPageRedo.GetSize() - 1);

	m_arrPageUndo.Add(hi);

	m_pControl->AnimatePageTransition(hi.pFrom, hi.pTo);
}

void CXTPFlowGraphPageHistory::Clear()
{
	m_arrPageRedo.RemoveAll();
	m_arrPageUndo.RemoveAll();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphPageHistory, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "GoBackward", 1000, GoBackward, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "GoForward", 1001, GoForward, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphPageHistory, "CanGoBackward", 1002, CanGoBackward,
						SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphPageHistory, "CanGoForward", 1003, CanGoForward,
						SetNotSupported, VT_BOOL)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "Clear", 1004, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "AnimatePageTransition", 1015,
					 OleAnimatePageTransition, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphPageHistory, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphPageHistory, XTPDIID_FlowGraphPageHistory, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphPageHistory, XTPDIID_FlowGraphPageHistory)

void CXTPFlowGraphPageHistory::OleAnimatePageTransition(LPDISPATCH lpDispFrom, LPDISPATCH lpDispTo)
{
	CXTPFlowGraphNode* pNodeFrom = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPFlowGraphNode, CXTPFlowGraphNode::FromIDispatchSafe(lpDispFrom));
	CXTPFlowGraphNode* pNodeTo = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPFlowGraphNode,
															  CXTPFlowGraphNode::FromIDispatchSafe(
																  lpDispTo));

	if (NULL == pNodeFrom || NULL == pNodeTo)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	AnimatePageTransition(pNodeFrom, pNodeTo);
}

#endif
