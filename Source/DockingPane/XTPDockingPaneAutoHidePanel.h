// XTPDockingPaneAutoHidePanel.h : interface for the CXTPDockingPaneAutoHidePanel class.
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
#if !defined(__XTPDOCKINGPANEAUTOHIDEPANEL_H__)
#	define __XTPDOCKINGPANEAUTOHIDEPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockingPaneLayout;
class CXTPDockingPaneAutoHidePanel;

//===========================================================================
// Summary:
//     CXTPDockingPaneAutoHideWnd is a CMiniFrameWnd derived class. It is
//     used internally by CXTPDockingPaneAutoHidePanel as a sliding
//     auto-hide window for the hidden pane.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneAutoHideWnd : public CMiniFrameWnd
{
	DECLARE_DYNAMIC(CXTPDockingPaneAutoHideWnd)

private:
	CXTPDockingPaneAutoHideWnd(CXTPDockingPaneAutoHidePanel* pPanel,
							   CXTPDockingPaneTabbedContainer* pPane);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the child container of a sliding auto-hide window.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer* GetPane() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPDockingPaneAutoHideWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPDockingPaneAutoHideWnd)
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	void CloseWindow();
	CXTPDockingPaneManager* GetDockingPaneManager() const;

	void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo, BOOL bIncludeSplitter = TRUE) const;

	void RecalcLayout(BOOL bNotify = TRUE);
	void DoSlideStep();
	void Reposition(CRect rc, CRect rcClient);
	BOOL GetAvailableRect(CRect& rcAvail, CRect& rc) const;

public:
	static double m_dAnimationDelay;	  // Animation delay.
	static int m_nAnimationInterval;	  // Delay in ms between animation steps.
	static int m_nAnimationDuration;	  // Duration of sliding animation in ms.
	static DWORD m_nMouseHoverDelay;	  // Duration of mouse hover before expanding.
	static UINT m_nInactiveCollapseDelay; // Delay before collapsing inactive.

private:
	CXTPDockingPaneTabbedContainer* m_pPane;
	CXTPDockingPaneAutoHidePanel* m_pPanel;
	XTPDockingPaneDirection m_direction;

	BOOL m_bHoriz;
	int m_nSlideStep;
	int m_nStepsCount;
	BOOL m_bSlideOut;
	CRect m_rcWindow;
	int m_nDeactivationCount;
	BOOL m_bTracking;

private:
	friend class CXTPDockingPaneAutoHidePanel;
};

//===========================================================================
// Summary:
//     CXTPDockingPaneAutoHidePanel is a multiple inheritance class derived from
//     CWnd and CXTPDockingPaneBase. It is used internally as an auto-hide container
//     for CXTPDockingPaneBase derived classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneAutoHidePanel
	: public CWnd
	, public CXTPDockingPaneBaseContainer
	, public CXTPTabManager
{
private:
	class CAutoHidePanelTabManager;
	class CAutoHidePanelTabManagersArray;
	class CPanelDropTarget;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneAutoHidePanel object. Protected constructor.
	//     Internally constructed only.
	// Parameters:
	//     pLayout - Pointer to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneAutoHidePanel(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneAutoHidePanel object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	~CXTPDockingPaneAutoHidePanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the position of the
	//     auto-hide panel.
	// Returns:
	//     The panel's direction.
	//-----------------------------------------------------------------------
	XTPDockingPaneDirection GetDirection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the window handle for a window.
	// Returns:
	//     The window handle for a window if successful. NULL if the HWND is not
	//     attached to a window or if it is used with a NULL HWND pointer.
	//-----------------------------------------------------------------------
	virtual HWND GetPaneHwnd() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines which header item, if any, is
	//     at a specified cursor position.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     The index of the item at the position specified by 'point',
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPDockingPane* HitTest(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tab for a corresponding pane.
	// Parameters:
	//     pPane - Pane to get the tab for.
	// Returns:
	//     Pointer to the CXTPTabManagerItem object corresponding with pPane.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetPaneTab(CXTPDockingPane* pPane) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to access the visual elements of the docking
	//     pane tabs (i.e. tab colors, styles, etc.).
	// Returns:
	//     Pointer to the CXTPTabPaintManager that contains the visual elements
	//     of the docking pane tabs.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the icon of the tab in the tabbed
	//     container needs to be drawn (i.e. on mouse over).
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Specifies the location of the image.
	//     pItem  - CXTPTabManagerItem object to draw icon on.
	//     bDraw  - TRUE if the icon needs to be drawn (i.e. the icon size changed),
	//              FALSE if the icon does not need to be drawn or redrawn.
	//     szIcon - Size of the tab icon.
	// Returns:
	//     TRUE if the icon was drawn successfully, FALSE if the icon was not drawn.
	//-----------------------------------------------------------------------
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw,
						  CSize& szIcon) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the position of the pane's tab in the auto-hide panel.
	// Returns:
	//     Flag from the XTPTabPosition enumeration that specifies the position.
	//-----------------------------------------------------------------------
	XTPTabPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the length of the items in the auto-hide panel.
	// Returns:
	//     Length of the items in the auto hide panel.
	//-----------------------------------------------------------------------
	virtual int GetItemsLength() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the position of the pane.
	// Parameters:
	//     pParent - Pointer to a CWnd object.
	//     rect    - Rectangle of the pane to be set.
	//     lParam  - Pointer to an AFX_SIZEPARENTPARAMS structure.
	// ----------------------------------------------------------------------
	virtual void OnSizeParentEx(CWnd* pParent, CRect& rect, LPVOID lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a pane from the collection.
	// Parameters:
	//     pPane - Pointer to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void RemovePane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a child pane collection is changed.
	// Parameters:
	//     pContainer - Pointer to the CXTPDockingPaneBase object that was changed.
	//-----------------------------------------------------------------------
	virtual void OnChildContainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert a pane into a collection.
	// Parameters:
	//     pPane - Pointer to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void _InsertPane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to close the active window from a panel.
	// Parameters:
	//     bDelayed - FALSE to delay the closing.
	// See Also:
	//     CloseActiveWindows
	//-----------------------------------------------------------------------
	void CloseActiveWindow(BOOL bDelayed = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to close active windows from all panels.
	// See Also:
	//     CloseActiveWindow
	//-------------------------------------------------------------------------
	void CloseActiveWindows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show (activate) a pane by its identifier.
	// Parameters:
	//     pPane     - Pointer to a CXTPDockingPane object.
	//     bSetFocus - TRUE to set focus to the child docking pane.
	//-----------------------------------------------------------------------
	void ShowPane(CXTPDockingPane* pPane, BOOL bSetFocus = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to refresh the manager array.
	// Parameters:
	//     pDC      - Pointer to a CDC object.
	//     rcClient - Size of the client area.
	//-----------------------------------------------------------------------
	void RefreshManagersArray(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the height of the panel.
	// Returns:
	//     The height of the panel.
	//-----------------------------------------------------------------------
	int GetPanelHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an auto-hide panel.
	// Parameters:
	//     pParent - Parent frame site.
	//-----------------------------------------------------------------------
	virtual void Create(CWnd* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a pane must be redrawn.
	// Parameters:
	//     bSelectionChanged - TRUE if the selected pane was changed.
	//-----------------------------------------------------------------------
	virtual void InvalidatePane(BOOL bSelectionChanged);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	virtual void RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/);
	virtual void Reposition();

	void DeletePane();
	void OnFinalRelease();

	//{{AFX_MSG(CXTPDockingPaneAutoHidePanel)
	afx_msg void OnPaint();
	afx_msg void OnDraw(CDC* pDC);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT /*nFlags*/, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM, LPARAM lParam);
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
private:
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPDockingPaneAutoHidePanel);
	DECLARE_ENUM_VARIANT(CXTPDockingPaneAutoHidePanel);
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	LPDISPATCH OleGetDispatch(BOOL /*bAddRef*/);
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetContainer();
	afx_msg int OleGetType();
	afx_msg LPDISPATCH OleGetPane(int nIndex);
	afx_msg HWND OleGetHwnd();
	afx_msg BOOL OleIsEmpty();
	afx_msg int OleGetPosition();
//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	void Copy(CXTPDockingPaneBase* pClone, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

public:
	static BOOL m_bCloseActiveWindow; // TRUE to allow the auto-hide window to be closed.

protected:
	CXTPDockingPaneAutoHideWnd* m_pActiveWnd; // Active window.
	XTPDockingPaneDirection m_direction;	  // Location of hidden panel.

private:
	CAutoHidePanelTabManagersArray* m_pTabManagers;
	CPanelDropTarget* m_pDropTarget;
	CRect m_rcFrameRect;
	int m_nItemsLength;

private:
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneTabbedContainer;
	friend class CXTPDockingPaneManager;
	friend class CPanelDropTarget;
};

AFX_INLINE BOOL CXTPDockingPaneAutoHidePanel::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
													 DWORD dwStyle, const RECT& rect,
													 CWnd* pParentWnd, UINT nID,
													 CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE XTPDockingPaneDirection CXTPDockingPaneAutoHidePanel::GetDirection() const
{
	return m_direction;
}
AFX_INLINE HWND CXTPDockingPaneAutoHidePanel::GetPaneHwnd() const
{
	return CWnd::GetSafeHwnd();
}
AFX_INLINE CXTPDockingPaneTabbedContainer* CXTPDockingPaneAutoHideWnd::GetPane() const
{
	return m_pPane;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPDOCKINGPANEAUTOHIDEPANEL_H__)