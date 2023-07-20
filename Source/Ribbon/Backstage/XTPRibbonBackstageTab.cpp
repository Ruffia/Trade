// XTPRibbonBackstageTab.cpp : implementation file
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
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPCasting.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Controls/Util/XTPControlTheme.h"

#include "Ribbon/Backstage/XTPRibbonBackstageTab.h"
#ifdef _XTP_COMMANDBARS_ACTIVEX
#	include "Common/XTPColorManager.h"
#	include "../../../Workspace/ActiveX/CommandBars/BackstageButtonCtl.h"
#else
#	include "Controls/Button/XTPButton.h"
#	include "Ribbon/Backstage/XTPRibbonBackstageButton.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_XTP_CONTROL(CXTPRibbonBackstageTab, CXTPControl)

CXTPRibbonBackstageTab::CXTPRibbonBackstageTab(HWND hwndControl)
{
	m_szControlPaneMinSize = CSize(0, 0);

	m_hwndControl = hwndControl;
	m_arrKeyboardTips.RemoveAll();
	m_arrKeyboardTipsIds.RemoveAll();
}

CWnd* CXTPRibbonBackstageTab::GetControlPane() const
{
	return CWnd::FromHandle(m_hwndControl);
}

void CXTPRibbonBackstageTab::CheckKeyboardTips()
{
	m_arrKeyboardTips.RemoveAll();
	m_arrKeyboardTipsIds.RemoveAll();

	CMap<TCHAR, TCHAR, int, int> mapUsedKeys;
	HWND hWnd = ::GetWindow(m_hwndControl, GW_CHILD);
	int digit = 0;
	while (hWnd != NULL)
	{
		// Check if the control is a Backstage Button
		CWnd* pWnd = CWnd::FromHandle(hWnd);
		if (!pWnd)
		{
			hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
			continue;
		}
#ifdef _XTP_COMMANDBARS_ACTIVEX
		CBackstageButtonCtrl* pBackstageButton = DYNAMIC_DOWNCAST(CBackstageButtonCtrl, pWnd);
#else
		CXTPRibbonBackstageButton* pBackstageButton = DYNAMIC_DOWNCAST(CXTPRibbonBackstageButton,
																	   pWnd);
#endif
		if (pBackstageButton == NULL)
		{
			hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
			continue;
		}
		// Set a digit as a keyboard tip for the current button
		CString s;
		s.Format(_T("%d"), digit);
		digit++;
		mapUsedKeys[s.GetAt(0)] = 1;
		m_arrKeyboardTips.Add(s.GetAt(0));
		m_arrKeyboardTipsIds.Add(hWnd);
		WPARAM wp = XTPToWPARAM(s.GetAt(0));
		::SendMessage(hWnd, WM_XTP_SETBAKSTAGEBUTTONKEYTIP, wp, 0L);

		// Next control of the pane
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
	mapUsedKeys.RemoveAll();
}

void CXTPRibbonBackstageTab::ShowKeyboardTips(BOOL bShow)
{
	for (int i = 0; i < m_arrKeyboardTipsIds.GetSize(); i++)
	{
		HWND hWnd = m_arrKeyboardTipsIds[i];

		WPARAM wp = XTPToWPARAM(bShow);
		::SendMessage(hWnd, WM_XTP_SETBAKSTAGEBUTTONKEYTIPDRAWFLAG, wp, 0L);
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
	}
}

TCHAR CXTPRibbonBackstageTab::GetKeyboardTipChar(int index) const
{
	if ((index >= 0) && (index < m_arrKeyboardTips.GetSize()))
		return m_arrKeyboardTips[index];
	return 0;
}

void CXTPRibbonBackstageTab::ClickControlWithKeyboardTip(int index)
{
	if ((index < 0) && (index >= m_arrKeyboardTipsIds.GetSize()))
		return;
	HWND hWnd = m_arrKeyboardTipsIds[index];
	if (hWnd != NULL)
		::PostMessage(hWnd, BM_CLICK, 0, 0);
}

int CXTPRibbonBackstageTab::GetKeyboardTipsCount() const
{
	return XTPToIntChecked(m_arrKeyboardTips.GetSize());
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonBackstageTab, CXTPControl)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPRibbonBackstageTab, CXTPControl)
	INTERFACE_PART(CXTPRibbonBackstageTab, XTPDIID_RibbonBackstageTab, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonBackstageTab, XTPDIID_RibbonBackstageTab)

#endif // _XTP_COMMANDBARS_ACTIVEX
