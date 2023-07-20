// XTPFlowGraphElement.cpp : implementation of the CXTPFlowGraphElement class.
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

#include "Common/XTPFramework.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphSelectedElements.h"
#include "FlowGraph/XTPFlowGraphIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPFlowGraphElement, CXTPCmdTarget)

CXTPFlowGraphElement::CXTPFlowGraphElement()
{
	m_bSelected = FALSE;
	m_bDirty	= TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphElement::~CXTPFlowGraphElement()
{
}

BOOL CXTPFlowGraphElement::IsSelected() const
{
	return m_bSelected;
}

void CXTPFlowGraphElement::OnGraphChanged()
{
	m_bDirty = TRUE;

	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
		pControl->OnGraphChanged();
}

void CXTPFlowGraphElement::OnRemoved()
{
	if (IsSelected())
	{
		GetPage()->GetSelection()->Remove(this);
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphElement, CXTPCmdTarget)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPFlowGraphElement, CXTPCmdTarget)
	INTERFACE_PART(CXTPFlowGraphElement, XTPDIID_FlowGraphElement, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphElement, XTPDIID_FlowGraphElement)

#endif
