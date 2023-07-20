// XTPGridFilterEditControl.cpp : implementation of the CXTPGridFilterEditControl class.
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

#include "GridControl/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridFilterEditControl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPGridFilterEditControl

CXTPGridFilterEditControl::CXTPGridFilterEditControl()
{
	m_pGridCtrl = NULL;
	m_strHint   = XTPResourceManager()->LoadString(XTP_IDS_GRID_FILERT_TEXT_HINT);
}

CXTPGridFilterEditControl::~CXTPGridFilterEditControl()
{
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridFilterEditControl, CEdit)
	//{{AFX_MSG_MAP(CXTPGridFilterEditControl)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPGridFilterEditControl attributes

BOOL CXTPGridFilterEditControl::SetGridCtrl(CXTPGridControl* pGridCtrl)
{
	if (pGridCtrl == NULL)
		return FALSE;

	ASSERT_KINDOF(CXTPGridControl, pGridCtrl);
	m_pGridCtrl = pGridCtrl;

	return TRUE;
}

CXTPGridControl* CXTPGridFilterEditControl::GetGridCtrl()
{
	return m_pGridCtrl;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPGridFilterEditControl operations

/////////////////////////////////////////////////////////////////////////////
// CXTPGridFilterEditControl message handlers

void CXTPGridFilterEditControl::SetHint(LPCTSTR lpszHint)
{
	m_strHint = lpszHint;
}

CString CXTPGridFilterEditControl::GetHint() const
{
	return m_strHint;
}

// Handles the EN_CHANGE message
BOOL CXTPGridFilterEditControl::OnChange()
{
	if (!m_pGridCtrl)
		return FALSE;

	// Get text
	CString strNewFilter;
	GetWindowText(strNewFilter);

	// Apply new filter
	if (m_pGridCtrl->GetFilterText() != strNewFilter)
	{
		m_pGridCtrl->SetFilterText(strNewFilter);

		// refresh control
		m_pGridCtrl->Populate();
	}

	// Returning FALSE allows the parent window to also handle the EN_CHANGE message
	return FALSE;
}

void CXTPGridFilterEditControl::OnPaint()
{
	if (GetWindowTextLength() == 0 && ::GetFocus() != m_hWnd)
	{
		CPaintDC dc(this); // device context for painting

		CXTPFontDC autoFont(&dc, GetFont(), GetXtremeColor(COLOR_GRAYTEXT));

		// show hint text
		CString strText = GetHint();

		CRect rc;
		GetClientRect(&rc);
		dc.FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));

		CRect rcText;
		GetRect(&rcText);
		dc.DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_EDITCONTROL);
	}
	else
	{
		Default();
	}
}
