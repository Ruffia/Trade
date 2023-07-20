// XTPDialogBase.cpp : implementation of the CXTPDialogBaseImpl class.
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

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "TabManager/Includes.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Edit/XTPEdit.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizeDialog.h"
#include "Controls/Scroll/XTPScrollBar.h"

#ifdef _XTP_INCLUDE_COMMANDBARS
#	include "CommandBars/Resource.h"
#	include "CommandBars/XTPCommandBarsDefines.h"
#	include "CommandBars/XTPPaintManager.h"
#	include "CommandBars/XTPShortcutManager.h"
#	include "CommandBars/XTPCommandBars.h"
#	include "CommandBars/XTPCommandBar.h"
#	include "CommandBars/XTPToolBar.h"
#	include "CommandBars/XTPMenuBar.h"
#	include "CommandBars/XTPControl.h"
#	include "CommandBars/XTPControlEdit.h"
#endif

#include "Controls/Dialog/XTPDialogBase.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPDialogBaseImpl::~CXTPDialogBaseImpl()
{
#ifdef _XTP_INCLUDE_COMMANDBARS
	CMDTARGET_RELEASE(m_pCommandBars);
#endif
}

#ifdef _XTP_INCLUDE_COMMANDBARS

BOOL CXTPDialogBaseImpl::InitCommandBars(CRuntimeClass* pCommandBarsClass /*= NULL*/)
{
	if (NULL == pCommandBarsClass)
		pCommandBarsClass = RUNTIME_CLASS(CXTPCommandBars);

	CMDTARGET_RELEASE(m_pCommandBars);

	ASSERT(NULL != pCommandBarsClass);
	ASSERT(pCommandBarsClass->IsDerivedFrom(RUNTIME_CLASS(CXTPCommandBars)));

	m_pCommandBars = (CXTPCommandBars*)pCommandBarsClass->CreateObject();
	ASSERT(NULL != m_pCommandBars);

	m_pCommandBars->SetSite(GetCommandBarsSite());
	m_pCommandBars->EnableDocking();

	return TRUE;
}

void CXTPDialogBaseImpl::DockRightOf(CXTPToolBar* pBarToDock, CXTPToolBar* pBarOnLeft)
{
	ASSERT(NULL != pBarOnLeft);

	GetCommandBarsSite()->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,
										 AFX_IDW_PANE_FIRST);

	CXTPWindowRect rcBar(pBarOnLeft);

	if (IsVerticalPosition(pBarOnLeft->GetPosition()))
		rcBar.OffsetRect(0, rcBar.Height());
	else
		rcBar.OffsetRect(rcBar.Width(), 0);

	GetCommandBars()->DockCommandBar(pBarToDock, rcBar, pBarOnLeft->GetDockBar());
}

void CXTPDialogBaseImpl::SaveCommandBars(LPCTSTR lpszProfileName)
{
	if (NULL != m_pCommandBars)
	{
		m_pCommandBars->SaveOptions(lpszProfileName);
		m_pCommandBars->SaveBarState(lpszProfileName);
		m_pCommandBars->GetShortcutManager()->SaveShortcuts(lpszProfileName);
	}
}

void CXTPDialogBaseImpl::LoadCommandBars(LPCTSTR lpszProfileName, BOOL bSilent /*= FALSE*/)
{
	if (NULL != m_pCommandBars)
	{
		m_pCommandBars->LoadOptions(lpszProfileName);
		m_pCommandBars->LoadBarState(lpszProfileName, bSilent);
		m_pCommandBars->GetShortcutManager()->LoadShortcuts(lpszProfileName);
	}
}

BOOL CXTPDialogBaseImpl::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(NULL != pMsg);

	if ((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		&& (pMsg->wParam != VK_RETURN && pMsg->wParam != VK_TAB && pMsg->wParam != VK_ESCAPE))
	{
		CWnd* pWnd = CWnd::GetFocus();
		if (NULL != pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CXTPCommandBarEditCtrl)))
			return FALSE;
	}

	if (NULL != m_pCommandBars && m_pCommandBars->PreTranslateFrameMessage(pMsg))
		return TRUE;

	return FALSE;
}

BOOL CXTPDialogBaseImpl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (NULL != m_pCommandBars && m_pCommandBars->OnFrameWndMsg(message, wParam, lParam, pResult))
		return TRUE;

	return FALSE;
}

#endif

#ifdef _XTP_INCLUDE_COMMANDBARS

void CXTPDialogEx::SetMenuBar(CXTPMenuBar* pMenuBar)
{
	UNREFERENCED_PARAMETER(pMenuBar);

	XTP_ERROR_MSG("WARNING: CXTPDialogEx::SetMenuBar(...) has been deprecated, use\n"
				  "CXTPCommandBars::SetMenu(...) instead, for example:\n\n"
				  "VERIFY(InitCommandBars());\n\n"
				  "CXTPCommandBars* pCommandBars = GetCommandBars();\n"
				  "pCommandBars->SetMenu(_T(\"Menu Bar\"), IDR_MENU);");
}

#	if (_MSC_VER > 1200)
void CXTPDHtmlDialogEx::SetMenuBar(CXTPMenuBar* pMenuBar)
{
	UNREFERENCED_PARAMETER(pMenuBar);

	XTP_ERROR_MSG("WARNING: CXTPDHtmlDialogEx::SetMenuBar(...) has been deprecated, use\n"
				  "CXTPCommandBars::SetMenu(...) instead, for example:\n\n"
				  "VERIFY(InitCommandBars());\n\n"
				  "CXTPCommandBars* pCommandBars = GetCommandBars();\n"
				  "pCommandBars->SetMenu(_T(\"Menu Bar\"), IDR_MENU);");
}
#	endif

#endif
