// XTPTearOffFrame.h : interface for the CXTPTearOffFrame class.
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
#if !defined(__XTPTEAROFFFRAMEWND_H__)
#	define __XTPTEAROFFFRAMEWND_H__
//{{AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE
class CXTPMultiDocTemplate : public CMultiDocTemplate
{
	friend class CXTPTearOffFrame;
	friend class CXTPTabClientWnd;
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	friend class CXTPDockingPaneManager;
#		endif
#	endif

	CXTPMultiDocTemplate();
};

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_TEAROFF_ACTIVATE message is sent to the main window to
//     inform about last active TearOff	frame.
// Example:
//     Here is an example of how an application would process the WM_XTP_TEAROFF_ACTIVATE
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
//     //{{AFX_MSG_MAP(CChildFrame)
//     ON_MESSAGE(WM_XTP_TEAROFF_ACTIVATE, OnTearoffActivate)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTearoffActivate(WPARAM wParam, LPARAM lParam)
// {
//     CXTPTearOffFrame* pFrame = (CXTPTearOffFrame*)wParam;
//
//     m_pPrevActiveFrame = m_pLastActiveFrame;
//     m_pLastActiveFrame = pFrame;
//
//     return 0;
// }
// </code>
//-----------------------------------------------------------------------
const UINT WM_XTP_TEAROFF_ACTIVATE = (WM_XTP_COMMANDBARS_BASE + 33);

//{{AFX_CODEJOCK_PRIVATE

//=======================================================================
// Summary:
//      The class represents a floating torn off frame.
//=======================================================================
class CXTPTearOffFrame
	: public CXTPMDIFrameWnd
	, CXTPCommandBarsFrameHook
{
	DECLARE_DYNCREATE(CXTPTearOffFrame);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Construct frame object.
	//-----------------------------------------------------------------------
	CXTPTearOffFrame();

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles resources cleanup.
	//-----------------------------------------------------------------------
	virtual ~CXTPTearOffFrame();

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles frame window destruction.
	// Parameters:
	//      TRUE if the window is destroyed successfully, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL DestroyWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//      Initializes frame object.
	// Parameters:
	//      pTabClient - A pointer to the associated tab client object.
	// 	    pTabItem - A pointer to the initial tab item object.
	//-----------------------------------------------------------------------
	void Init(CXTPTabClientWnd* pTabClient, CXTPTabManagerItem* pTabItem);

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates an associated tab client object and and all related frame states.
	// Parameters:
	//      pTabClient - A pointer to the new associated tab client object.
	//-----------------------------------------------------------------------
	void Update(CXTPTabClientWnd* pTabClient);

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains an associated tab client object pointer.
	// Returns:
	//      An associated tab client object pointer.
	//-----------------------------------------------------------------------
	CXTPTabClientWnd* GetTabClient() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the frame is being dragged.
	// Returns:
	//      TRUE if the frame is being dragged or FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsFrameBeingDragged() const;

	virtual HMENU GetWindowMenuPopup(HMENU hMenuBar);

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnterSizeMove();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnExitSizeMove();
	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnTabbarNewItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetActiveWorkspace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bIsFrameDragging;
	HCURSOR m_hCursor;
	HCURSOR m_hCursorNew;
	CXTPTabClientWnd m_wndTabClient;
	CRect m_rcPrev;
	CXTPTabClientContext* m_pTabClientContext;

	CRect m_rcDragRect;
	CXTPTabClientWnd* pTargetTabPrev;
	void UpdateDragRect();

	void ChangeMDIChildFrame(CXTPTabClientWnd* pTarget, int nOldWorkspace, int nSticker);
	CXTPTabClientWnd::CWorkspace*
		ChangeItemWorkspace(CXTPTabClientWnd* pTarget, int nOldWorkspace, int nSticker,
							CXTPTabClientWnd::CWorkspace* pExistingWorkspace = NULL);

	friend class CXTPMDIFrameWndEx;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTEAROFFFRAMEWND_H__)
