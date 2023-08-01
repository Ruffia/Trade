// XTPFrameWnd.h : interface for the CXTPFrameWnd and CXTPMDIFrameWnd classes.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPFRAMEWND_H__)
#	define __XTPFRAMEWND_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPShortcutManager;
class CXTPTabClientWnd;
class CXTPTearOffFrame;
class CXTPDockingPaneManager;

//===========================================================================
// Summary:
//     CXTPCommandBarsSiteBase is a standalone class.
//     It represents a site for a Command Bars object.
//===========================================================================
template<class TBase>
class CXTPCommandBarsSiteBase : public TBase
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsSiteBase object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsSiteBase()
	{
		m_pCommandBars = 0;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsSiteBase object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPCommandBarsSiteBase()
	{
		CMDTARGET_RELEASE(m_pCommandBars);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates command bars.
	// Parameters:
	//     pCommandBarsClass - Custom runtime class of CommandBars. It can be used if you
	//                         want to override some methods of the CXTPCommandBars class.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL InitCommandBars(CRuntimeClass* pCommandBarsClass = RUNTIME_CLASS(CXTPCommandBars))
	{
		CMDTARGET_RELEASE(m_pCommandBars);

		ASSERT(pCommandBarsClass->IsDerivedFrom(RUNTIME_CLASS(CXTPCommandBars)));
		m_pCommandBars = (CXTPCommandBars*)pCommandBarsClass->CreateObject();
		ASSERT(m_pCommandBars);
		if (!m_pCommandBars)
			return FALSE;
		m_pCommandBars->SetSite(this);
		m_pCommandBars->EnableDocking();
		return TRUE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will re-dock a toolbar specified by 'pBarToDock'
	//     to the right of a newly docked toolbar specified by 'pBarOnLeft'.
	// Parameters:
	//     pBarToDock - A CXTPToolBar pointer to the toolbar to be docked.
	//     pBarOnLeft - A CXTPToolBar pointer to the already docked toolbar.
	//-----------------------------------------------------------------------
	void DockRightOf(CXTPToolBar* pBarToDock, CXTPToolBar* pBarOnLeft)
	{
		CWnd::RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_PANE_FIRST);

		CXTPWindowRect rcBar(pBarOnLeft);

		if (IsVerticalPosition(pBarOnLeft->GetPosition()))
			rcBar.OffsetRect(0, rcBar.Height());
		else
			rcBar.OffsetRect(rcBar.Width(), 0);

		GetCommandBars()->DockCommandBar(pBarToDock, rcBar, pBarOnLeft->GetDockBar());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the
	//     registry or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a null-terminated string that specifies
	//                       the name of a section in the initialization file
	//                       or a key in the Windows registry where state
	//                       information is stored.
	//-----------------------------------------------------------------------
	virtual void SaveCommandBars(LPCTSTR lpszProfileName)
	{
		if (m_pCommandBars)
		{
			m_pCommandBars->SaveOptions(lpszProfileName);
			m_pCommandBars->SaveBarState(lpszProfileName);
			m_pCommandBars->GetShortcutManager()->SaveShortcuts(lpszProfileName);
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the
	//     registry or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a null-terminated string that specifies
	//                       the name of a section in the initialization file
	//                       or a key in the Windows registry where state
	//                       information is stored.
	//     bSilent         - TRUE to disable user notifications when command bars
	//                       are restored to their original state.
	//-----------------------------------------------------------------------
	virtual void LoadCommandBars(LPCTSTR lpszProfileName, BOOL bSilent = FALSE)
	{
		if (m_pCommandBars)
		{
			m_pCommandBars->LoadOptions(lpszProfileName);
			m_pCommandBars->LoadBarState(lpszProfileName, bSilent);
			m_pCommandBars->GetShortcutManager()->LoadShortcuts(lpszProfileName);
		}
	}

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the CommandBars object.
	// Returns:
	//     A pointer to the CommandBars object.
	//----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars() const
	{
		return m_pCommandBars;
	}

	//{{AFX_CODEJOCK_PRIVATE
	//----------------------------------------------------------------------
	// Summary: Deprecated.
	//----------------------------------------------------------------------
	CXTPShortcutManager* XTPShortcutManager()
	{
		return ((m_pCommandBars == NULL) ? NULL : m_pCommandBars->GetShortcutManager());
	}
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
			&& (pMsg->wParam != VK_RETURN && pMsg->wParam != VK_TAB && pMsg->wParam != VK_ESCAPE))
		{
			CWnd* pWnd = CWnd::GetFocus();
			if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CXTPCommandBarEditCtrl)))
				return FALSE;
		}

		if (TBase::PreTranslateMessage(pMsg))
			return TRUE;

		if (m_pCommandBars && m_pCommandBars->PreTranslateFrameMessage(pMsg))
			return TRUE;

		return FALSE;
	}

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		BOOL bBase		  = FALSE; // Message handled by base class
		BOOL bCommandBars = FALSE; // Message handled by CommandBars

		// The WM_SYSCOLORCHANGE message must be handled in the
		//  base class first to update the AUX_DATA brushes.
		if (WM_SYSCOLORCHANGE == message)
		{
			bBase = TBase::OnWndMsg(message, wParam, lParam, pResult);
		}

		if (NULL != m_pCommandBars)
		{
			bCommandBars = m_pCommandBars->OnFrameWndMsg(message, wParam, lParam, pResult);
		}

		if (!bBase && !bCommandBars)
		{
			bBase = TBase::OnWndMsg(message, wParam, lParam, pResult);
		}

		return (bBase || bCommandBars);
	}

	//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPCommandBars* m_pCommandBars;
};

//===========================================================================
// Summary:
//     CXTPFrameWndBase is a standalone class.
//     It represents the parent class for the CXTPMDIFrameWnd and CXTPFrameWnd classes.
//===========================================================================
template<class TBase>
class CXTPFrameWndBase : public CXTPCommandBarsSiteBase<TBase>
{
	typedef CXTPCommandBarsSiteBase<TBase> Parent;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFrameWndBase object.
	//-----------------------------------------------------------------------
	CXTPFrameWndBase()
	{
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFrameWndBase object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPFrameWndBase()
	{
	}

	//{{AFX_CODEJOCK_PRIVATE
public:
	BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
				   CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL)
	{
		BOOL bResult = CXTPCommandBarsSiteBase<TBase>::LoadFrame(nIDResource, dwDefaultStyle,
																 pParentWnd, pContext);
		if (!bResult)
			return FALSE;

		if (CXTPCommandBarsSiteBase<TBase>::GetCommandBars()
			&& CXTPCommandBarsSiteBase<TBase>::GetCommandBars()
				   ->GetShortcutManager()
				   ->GetOriginalAccelerator())
		{
			CFrameWnd::m_hAccelTable = NULL;
		}

		return TRUE;
	}

	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
	{
		if (CXTPCommandBarsSiteBase<TBase>::GetCommandBars())
		{
			CXTPCommandBarsSiteBase<TBase>::GetCommandBars()->OnSetPreviewMode(bPreview);
		}

		CXTPCommandBarsSiteBase<TBase>::OnSetPreviewMode(bPreview, pState);
	}
	//}}AFX_CODEJOCK_PRIVATE
};

//-----------------------------------------------------------------------
// Summary:
//     A message that is sent to each MDI child window as a result of
//     a call to CXTPMDIFrameWnd::UpdateMDIChildrenTheme.
//-----------------------------------------------------------------------
extern const UINT _XTP_EXTRN XTP_WM_UPDATE_MDI_CHILD_THEME;

//===========================================================================
// Summary:
//     CXTPMDIFrameWnd is a CXTPFrameWndBase derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPMDIFrameWnd : public CXTPFrameWndBase<CMDIFrameWnd>
{
public:
	DECLARE_DYNCREATE(CXTPMDIFrameWnd)

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMDIFrameWnd object.
	//-----------------------------------------------------------------------
	CXTPMDIFrameWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a theme update notification to all MDI children.
	//-----------------------------------------------------------------------
	void UpdateMDIChildrenTheme();

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPMDIFrameWndEx is a CXTPMDIFrameWnd derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPMDIFrameWndEx : public CXTPMDIFrameWnd
{
public:
	DECLARE_DYNAMIC(CXTPMDIFrameWndEx)

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMDIFrameWndEx object.
	//-----------------------------------------------------------------------
	CXTPMDIFrameWndEx();

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles CXTPMDIFrameWndEx object destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPMDIFrameWndEx();

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the associated tab client pointer.
	// Returns:
	//      The associated tab client pointer.
	//-----------------------------------------------------------------------
	virtual CXTPTabClientWnd* GetTabClientWnd() = 0;

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the associated docking pane manager pointer.
	// Returns:
	//      The associated docking pane manager pointer.
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneManager* GetDockingPaneManager() = 0;
#		endif
#	endif

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

#	ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#	endif

protected:
	//{{AFX_MSG(CXTPMDIFrameWndEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg LRESULT OnTearoffActivate(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	CXTPTearOffFrame* m_pPrevActiveFrame;
	CXTPTearOffFrame* m_pLastActiveFrame;

public:
	UINT m_nIDResource;

protected:
	LRESULT UpdateMDIMenu(CWnd* pWnd);

	friend class CXTPTearOffFrame;
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPFrameWnd is a CXTPFrameWndBase derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPFrameWnd : public CXTPFrameWndBase<CFrameWnd>
{
public:
	DECLARE_DYNCREATE(CXTPFrameWnd)

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFrameWnd object.
	//-----------------------------------------------------------------------
	CXTPFrameWnd();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPFRAMEWND_H__)