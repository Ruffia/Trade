// XTPFlowGraphEditItem.cpp : implementation of the CXTPFlowGraphEditItem class.
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
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "FlowGraph/XTPFlowGraphElement.h"
#include "FlowGraph/XTPFlowGraphPage.h"
#include "FlowGraph/XTPFlowGraphNode.h"
#include "FlowGraph/XTPFlowGraphPaintManager.h"
#include "FlowGraph/XTPFlowGraphControl.h"
#include "FlowGraph/XTPFlowGraphEditItem.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphEditItem

IMPLEMENT_DYNAMIC(CXTPFlowGraphEditItem, CEdit)

CXTPFlowGraphEditItem::CXTPFlowGraphEditItem()
{
	m_bEscapeKey = FALSE;
	m_pElement   = NULL;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPFlowGraphEditItem, CEdit)
	//{{AFX_MSG_MAP(CXTPFlowGraphEditItem)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

UINT CXTPFlowGraphEditItem::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPFlowGraphEditItem::OnKillFocus(CWnd* /*pNewWnd*/)
{
	PostMessage(WM_CLOSE);

	if (!m_pElement)
		return;

	if (!m_bEscapeKey)
	{
		CString str;
		GetWindowText(str);

		m_pElement->GetControl()->OnEndLabelEdit(m_pElement, str);
	}
	else
	{
		m_pElement->GetControl()->OnEndLabelEdit(m_pElement, m_pElement->GetCaption());
	}
	m_pElement = NULL;
}

void CXTPFlowGraphEditItem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		m_bEscapeKey = (nChar == VK_ESCAPE);
		OnKillFocus(NULL);

		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPFlowGraphEditItem::PostNcDestroy()
{
	delete this;
}

BOOL CXTPFlowGraphEditItem::Create(CXTPFlowGraphElement* pElement)
{
	m_pElement					   = pElement;
	CXTPFlowGraphControl* pControl = pElement->GetControl();

	LOGFONT lf  = pControl->GetPaintManager()->m_lfText;
	lf.lfHeight = LONG(lf.lfHeight * pControl->GetZoomLevel());

	m_xtpFont.CreateFontIndirect(&lf);

	UINT nFormat = UINT(pElement->IsKindOf(RUNTIME_CLASS(CXTPFlowGraphNode)) ? DT_CENTER : DT_LEFT);

	CRect rc = pElement->GetControl()->GetPaintManager()->GetCaptionRect(pElement);
	rc		 = pElement->GetPage()->PageToScreen(rc);

	CString strText = pElement->GetCaption();
	UINT nCenter	= UINT(nFormat & DT_CENTER ? ES_CENTER : 0);

	if (!CEdit::Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | nCenter, rc, pControl, 0))
		return FALSE;

	SetFont(&m_xtpFont);
	SetMargins(0, 0);
	SetWindowText(strText);
	SetFocus();
	SetSel(0, -1);

	return TRUE;
}
