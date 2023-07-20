// XTPDockingPaneTabbedContainer.h : interface for the CXTPDockingPaneTabbedContainer class.
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
#if !defined(__XTPDOCKINGPANETABBEDCONTAINER_H__)
#	define __XTPDOCKINGPANETABBEDCONTAINER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockingPaneAutoHidePanel;
class CXTPDockingPaneCaptionButton;

//===========================================================================
// Summary:
//     CXTPDockingPaneTabbedContainer is a multiple inheritance class derived from
//     CWnd and CXTPDockingPaneBase. It represents a tabbed container for Docking
//     Panes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneTabbedContainer
	: public CWnd
	, public CXTPDockingPaneBaseContainer
	, public CXTPTabManager
	, public CXTPAccessible
{
	DECLARE_DYNAMIC(CXTPDockingPaneTabbedContainer)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneTabbedContainer object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pLayout - Pointer to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneTabbedContainer object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneTabbedContainer();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to activate a child docking pane.
	// Parameters:
	//     pPane - Pane to be activated.
	//     bSetFocus - TRUE to set focus to the child docking pane.
	//     bDelayRedraw - TRUE to redraw the pane after a small delay.
	//-----------------------------------------------------------------------
	virtual void SelectPane(CXTPDockingPane* pPane, BOOL bSetFocus = TRUE,
							BOOL bDelayRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if a child pane is active.
	// Returns:
	//     TRUE if a child pane is active.
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the selected child pane.
	// Returns:
	//     The selected child pane.
	//-----------------------------------------------------------------------
	CXTPDockingPane* GetSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the title's close button.
	// Returns:
	//     A reference to the title's close button.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton* GetCloseButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the title's pin button.
	// Returns:
	//     A reference to the title's pin button.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton* GetPinButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the title is visible.
	// Returns:
	//     TRUE if the title is visible.
	//-----------------------------------------------------------------------
	BOOL IsTitleVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the caption is drawn vertically.
	// Returns:
	//     TRUE if the caption is drawn vertically.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionVertical() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tab bar is visible.
	// Returns:
	//     TRUE if the tab bar is visible.
	//-----------------------------------------------------------------------
	virtual BOOL IsTabsVisible() const;

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
	//     Call this member to determine if the tabbed container is
	//     auto-hidden. If one pane in a group of panes is auto-hidden,
	//     then the entire group will be auto-hidden.
	// Returns:
	//     TRUE if the tabbed container is auto-hidden,
	//     FALSE if the tab group is visible.
	//-----------------------------------------------------------------------
	BOOL IsHidden() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called by the framework when the left mouse
	//     button is clicked while the mouse pointer is positioned over
	//     a pane caption. This will give the pane focus and begin the
	//     pane dragging process (if the left button is held down).
	// Parameters:
	//     point - Cursor location of the pane's caption that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnCaptionLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to check if a docking pane can be attached to
	//     a tabbed container.
	// Parameters:
	//     rcClient - Client rectangle of the container.
	//     pt       - Point to attach.
	// Returns:
	//     TRUE if the pane can be attached as a new tab to the tabbed container.
	//-----------------------------------------------------------------------
	virtual BOOL CanAttach(CRect& rcClient, CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to fill the pMinMaxInfo structure with the
	//     minimum and maximum width and height of the container.
	//     This will look at the MinMaxInfo for each pane in the
	//     container and set the appropriate minimum and maximum width
	//     and height for the container.
	// Parameters:
	//     pMinMaxInfo - Pointer to a MINMAXINFO structure.
	//-----------------------------------------------------------------------
	void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pane by its index.
	// Parameters:
	//     nIndex - Index of the pane to retrieve.
	// Returns:
	//     A pointer to a CXTPDockingPane object.
	//-----------------------------------------------------------------------
	CXTPDockingPane* GetItemPane(int nIndex) const;

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
	//     Call this member to get the text in the caption for the tabbed container
	//     (group of panes).
	// Returns:
	//     The text in the caption for the tabbed container (group of panes).
	//-----------------------------------------------------------------------
	CString GetTitle() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the caption button is visible.
	// Parameters:
	//     pButton - Caption button pointer.
	// Returns:
	//     TRUE if the caption button is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called by the framework when a caption button
	//     is clicked.
	// Parameters:
	//     pButton - Caption button that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called by the framework when a navigate button
	//     is clicked.
	// Parameters:
	//     pButton - Navigate button that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneTabbedContainer object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pPane   - Child pane.
	//     pFrame  - Parent Frame.
	//     pLayout - Pointer to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	void Init(CXTPDockingPane* pPane, CWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies a CXTPDockingPaneTabbedContainer object
	// Parameters:
	//     pClone - Pointer to a CXTPDockingPaneTabbedContainer object.
	//     pMap - Pointer to a CXTPPaneToPaneMap object.
	//     dwIgnoredOptions - Options that must be skipped.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPDockingPaneBase* pClone, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to recalculate the positions of tab groups/items.
	//-----------------------------------------------------------------------
	virtual void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to redraw the tabbed control (i.e. when
	//     an icon has changed).
	// Parameters:
	//     lpRect - The rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

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
	//     This member function is called in a derived class to set the
	//     tab paint manager.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	virtual void SetPaintManager(CXTPTabPaintManager* pPaintManager);

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
	//     This member is called to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when tabs are changed.
	//-----------------------------------------------------------------------
	virtual void OnTabsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to create a pane.
	//-----------------------------------------------------------------------
	virtual void CreateContainer();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void OnSizeParent(CWnd* pParent, CRect rect, LPVOID lParam);
	virtual void OnFocusChanged();
	virtual void SetDockingSite(CWnd* pFrame);
	virtual void RemovePane(CXTPDockingPaneBase* pPane);
	virtual void OnParentContainerChanged(CXTPDockingPaneBase* pContainer);
	void ShowTitle(BOOL bShow);
	void Show(BOOL bSetFocus);
	void ShowPane(CXTPDockingPane* pPane, BOOL bSetFocus);
	void _Swap(CXTPDockingPane* p1, CXTPDockingPane* p2);
	void _InsertPane(CXTPDockingPane* pPane, BOOL bSetFocus = TRUE);
	int HitTest(CPoint point) const;
	void AdjustMinMaxInfoClientRect(LPMINMAXINFO pMinMaxInfo, BOOL bCaptionOnly = FALSE) const;

	void InvalidatePane(BOOL bSelectionChanged);
	void _RestoreFocus();
	void EnsureSelectedTabVisible();
	BOOL OnCaptionButtonDown(CXTPDockingPaneCaptionButton* pButton);
	CXTPDockingPaneCaptionButton* HitTestCaptionButton(CPoint point) const;
	void DeletePane();
	void ClosePane(CXTPDockingPane* pPane);

	BOOL IsPaneRestored() const;
	BOOL IsAllowMaximize() const;
	BOOL IsPaneMaximized() const;
	BOOL IsPaneMinimized() const;
	void Restore();
	void Maximize();
	void NormalizeDockingSize();
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPDockingPaneTabbedContainer)
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	LRESULT OnHelpHitTest(WPARAM, LPARAM);
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPDockingPaneTabbedContainer)
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility support.
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual HRESULT GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
	virtual HRESULT AccessibleDoDefaultAction(VARIANT varChild);
	virtual CCmdTarget* GetAccessible();

	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
private:
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPDockingPaneTabbedContainer);
	DECLARE_ENUM_VARIANT(CXTPDockingPaneTabbedContainer);
	afx_msg LPDISPATCH OleGetDispatch(BOOL /*bAddRef*/);
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetContainer();
	afx_msg int OleGetType();
	afx_msg LPDISPATCH OleGetPane(int nIndex);
	afx_msg HWND OleGetHwnd();
	afx_msg BOOL OleIsEmpty();
	int OleGetPosition();
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	BOOL m_bActive;					  // TRUE if the tabbed container has an active caption.
	BOOL m_bTitleVisible;			  // TRUE if the title is visible.
	int m_nLockReposition;			  // InvalidatePane method currently executed.
	BOOL m_bDelayRedraw;			  // TRUE to redraw the pane after a small delay.
	CXTPDockingPane* m_pSelectedPane; // Pointer to the selected pane.
	CXTPDockingPane* m_pTrackingPane; // Pointer to the dragging pane.
	CArray<CRect, CRect&> m_lstRects; // Array of previous tabs positions.
	COleDropTarget* m_pDropTarget;	// Pointer to Ole Drop Target.
	BOOL m_bMaximized;				  // TRUE if the pane is currently maximized.
	BOOL m_bEnsureSelectedTab;		  // TRUE to check if the selected tab is visible after a delay.

private:
	class CContainerDropTarget;

private:
	using CXTPTabManager::OnNavigateButtonClick;

	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneContext;
	friend class CXTPDockingPaneMiniWnd;
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneCaptionButton;
	friend class CContainerDropTarget;
	friend class CXTPDockingPaneSidePanel;
};

AFX_INLINE BOOL CXTPDockingPaneTabbedContainer::IsActive() const
{
	return m_bActive;
}
AFX_INLINE CXTPDockingPane* CXTPDockingPaneTabbedContainer::GetSelected() const
{
	return m_pSelectedPane;
}
AFX_INLINE HWND CXTPDockingPaneTabbedContainer::GetPaneHwnd() const
{
	return CWnd::GetSafeHwnd();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPDOCKINGPANETABBEDCONTAINER_H__)
