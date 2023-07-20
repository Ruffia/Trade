// XTPDockingPaneBase.h : interface for the CXTPDockingPaneBase class.
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
#if !defined(__XTPDOCKINGPANEBASE_H__)
#	define __XTPDOCKINGPANEBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockingPaneBase;
class CXTPDockingPane;
class CXTPDockingPaneTabbedContainer;
class CXTPDockingPaneLayout;
class CXTPDockingPaneManager;
class CXTPDockingPanePaintManager;
class CXTPDockingPaneCaptionButton;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPDockingPaneCaptionButtons is a CArray derived class used to manipulate
//     Caption Buttons.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneCaptionButtons
	: public CArray<CXTPDockingPaneCaptionButton*, CXTPDockingPaneCaptionButton*>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the mouse pointer is positioned
	//     over one of the buttons in the pane caption.
	// Parameters:
	//     pt - A CPoint object containing the coordinates of the mouse pointer.
	// Returns:
	//     TRUE if the mouse pointer is positioned over a caption button,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL CheckForMouseOver(CPoint pt);
};

//===========================================================================
// Summary:
//     CXTPDockingPaneBase is a standalone class. It represents the parent
//     class for a docking pane and its containers.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneBase
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor. You cannot create this class.
	// Parameters:
	//     type    - Docking Pane type. Can be any of the values listed in the
	//               Remarks section.
	//     pLayout - Pointer to a CXTPDockingPaneLayout object.
	// Remarks:
	//     Docking Pane type can be one of the following:
	//     * <b>xtpPaneTypeDockingPane</b>: Indicates the pane's style is a docking pane.
	//     * <b>xtpPaneTypeTabbedContainer</b>: Indicates the pane's style is a tabbed
	//                                          container for a pane.
	//     * <b>xtpPaneTypeSplitterContainer</b>: Indicates the pane's style is a splitter
	//                                            container.
	//     * <b>xtpPaneTypeMiniWnd</b>: Indicates the pane's style is a floating window
	//                                  container.
	//     * <b>xtpPaneTypeClient</b>: Indicates the pane's style is a container for
	//                                 client area.
	//     * <b>xtpPaneTypeAutoHidePanel</b>: Indicates the pane's style is an auto-hide
	//                                        panel container.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneBase object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneBase();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the frame is active.
	// Returns:
	//     TRUE if the frame is active.
	//-----------------------------------------------------------------------
	virtual BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the caption is drawn vertically.
	// Returns:
	//     TRUE if the caption is drawn vertically.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionVertical() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the text in the caption for the tabbed container
	//     (group of panes).
	// Returns:
	//     The text in the caption for the tabbed container (group of panes).
	//-----------------------------------------------------------------------
	virtual CString GetTitle() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the type of the pane.
	// Returns:
	//     The type of the pane. It can be one of the following:
	//     * <b>xtpPaneTypeDockingPane</b>: Indicates the pane's style is a docking pane.
	//     * <b>xtpPaneTypeTabbedContainer</b>: Indicates the pane's style is a tabbed
	//                                          container for a pane.
	//     * <b>xtpPaneTypeSplitterContainer</b>: Indicates the pane's style is a splitter
	//                                            container.
	//     * <b>xtpPaneTypeMiniWnd</b>: Indicates the pane's style is a floating window
	//                                  container.
	//     * <b>xtpPaneTypeClient</b>: Indicates the pane's style is a container for
	//                                 client area.
	//     * <b>xtpPaneTypeAutoHidePanel</b>: Indicates the pane's style is an auto-hide
	//                                        panel container.
	//-----------------------------------------------------------------------
	XTPDockingPaneType GetType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     The GetPaneWindowRect function retrieves the dimensions of the
	//     bounding rectangle of the pane.
	// Returns:
	//     The visible rectangle of the pane.
	//-----------------------------------------------------------------------
	CRect GetPaneWindowRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the parent container of
	//     the pane.
	// Returns:
	//     The parent container of the pane.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase* GetContainer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the pane manager.
	// Returns:
	//     A pointer to a CXTPDockingPaneManager object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneManager* GetDockingPaneManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the paint manager.
	// Returns:
	//     A pointer to a CXTPDockingPanePaintManager object.
	//-----------------------------------------------------------------------
	CXTPDockingPanePaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified caption button.
	// Parameters:
	//     nID - Identifier of the button.
	// Returns:
	//     The CXTPDockingPaneCaptionButton object if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton* FindCaptionButton(int nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a reference to the caption's close/pop-up button.
	// Returns:
	//     A reference to a CXTPDockingPaneCaptionButtons object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButtons* GetCaptionButtons() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the pane.
	//-----------------------------------------------------------------------
	virtual void RedrawPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if one pane is contained in the
	//     collection of another.
	// Parameters:
	//     pPane - Pointer to a CXTPDockingPaneBase object.
	// Returns:
	//     POSITION of the pane in the collection if successful,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	virtual POSITION ContainPane(CXTPDockingPaneBase* pPane) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a collection of the panes with a
	//     specified type.
	// Parameters:
	//     type  - Type of the panes to find.
	//     pList - Pointer to a CXTPDockingPaneBaseList object that will
	//             contain found panes.
	// See Also:
	//     CXTPDockingPaneBaseList
	//-----------------------------------------------------------------------
	virtual void FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList) const;

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
	virtual void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if there are no children panes.
	// Returns:
	//     TRUE if there are no children panes, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsEmpty() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the parent window for the pane.
	// Returns:
	//     A pointer to a parent window if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* GetDockingSite() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the pane is hidden.
	// Returns:
	//     TRUE if the pane is hidden.
	//-----------------------------------------------------------------------
	virtual BOOL IsHidden() const;

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
	//     This member is called when a pane must be redrawn.
	// Parameters:
	//     bSelectionChanged - TRUE if the selected pane was changed.
	//-----------------------------------------------------------------------
	virtual void InvalidatePane(BOOL bSelectionChanged);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the position of the pane.
	// Parameters:
	//     pParent - Pointer to a CWnd object.
	//     rect    - Rectangle of the pane to be set.
	//     lParam  - Pointer to an AFX_SIZEPARENTPARAMS structure.
	//-----------------------------------------------------------------------
	virtual void OnSizeParent(CWnd* pParent, CRect rect, LPVOID lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update the state of the pane.
	//-----------------------------------------------------------------------
	virtual void OnFocusChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the parent frame of the pane.
	// Parameters:
	//     pFrame - Pointer to a CWnd object.
	//-----------------------------------------------------------------------
	virtual void SetDockingSite(CWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the parent container of the pane.
	// Parameters:
	//     pContainer - Pointer to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void SetParentContainer(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a child pane collection is changed.
	// Parameters:
	//     pContainer - Pointer to the CXTPDockingPaneBase object that was changed.
	//-----------------------------------------------------------------------
	virtual void OnChildContainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the parent pane collection is changed.
	// Parameters:
	//     pContainer - Pointer to the CXTPDockingPaneBase object that was changed.
	//-----------------------------------------------------------------------
	virtual void OnParentContainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to clone the pane.
	// Parameters:
	//     pLayout - Pointer to a CXTPDockingPaneLayout object.
	//     pMap - Pointer to a CXTPPaneToPaneMap object.
	//     dwIgnoredOptions - Options that must be skipped.
	// Returns:
	//     A pointer to a CXTPDockingPaneBase object.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneBase* Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap,
									   DWORD dwIgnoredOptions = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to copy a pane.
	// Parameters:
	//     pClone - Pointer to a CXTPDockingPaneBase object.
	//     pMap - Pointer to a CXTPPaneToPaneMap object.
	//     dwIgnoredOptions - Options that must be skipped.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPDockingPaneBase* pClone, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a pane from the collection.
	// Parameters:
	//     pPane - Pointer to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void RemovePane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the dimensions of the caption button
	//     are changed.
	// Parameters:
	//     pButton - Caption button pointer.
	//-----------------------------------------------------------------------
	virtual void OnCaptionButtonRectChanged(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a caption button is visible.
	// Parameters:
	//     pButton - Caption button pointer.
	// Returns:
	//     TRUE if the caption button is visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to destroy a pane.
	//-----------------------------------------------------------------------
	virtual void DeletePane() = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to create a pane.
	//-----------------------------------------------------------------------
	virtual void CreateContainer();

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual LPDISPATCH OleGetDispatch(BOOL bAddRef) = 0;

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CSize m_szDocking;				  // Size of the pane.
	CXTPDockingPaneLayout* m_pLayout; // Parent layout.

	CXTPDockingPaneBase* m_pParentContainer;		  // Parent container.
	CWnd* m_pDockingSite;							  // Parent frame.
	XTPDockingPaneType m_type;						  // Type of the pane.
	CRect m_rcWindow;								  // Position of the pane.
	CXTPDockingPaneCaptionButtons* m_pCaptionButtons; // Collection of caption buttons.

private:
	int m_nLength;
	int m_nIndex;

private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneSplitterContainer;
	friend class CXTPDockingPaneTabbedContainer;
	friend class CXTPDockingPaneMiniWnd;
	friend class CXTPDockingPaneSplitterWnd;
	friend class CXTPDockingPaneContext;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneCaptionButton;
	friend class CXTPDockingPaneSidePanel;
};

//{{AFX_CODEJOCK_PRIVATE
class _XTP_EXT_CLASS CXTPDockingPaneClientContainer
	: public CXTPCmdTarget
	, public CXTPDockingPaneBase
{
public:
	CXTPDockingPaneClientContainer(CXTPDockingPaneLayout* pLayout);

protected:
	virtual void DeletePane();

#	ifdef _XTP_ACTIVEX
private:
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPDockingPaneClientContainer);
	DECLARE_INTERFACE_MAP()

	LPDISPATCH OleGetDispatch(BOOL /*bAddRef*/);
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetContainer();
	afx_msg int OleGetType();
	afx_msg LPDISPATCH OleGetPane(int nIndex);
	afx_msg HWND OleGetHwnd();
	afx_msg BOOL OleIsEmpty();
	afx_msg int OleGetPosition();
#	endif
};
//}}AFX_CODEJOCK_PRIVATE

AFX_INLINE BOOL CXTPDockingPaneBase::IsActive() const
{
	return FALSE;
}

AFX_INLINE BOOL CXTPDockingPaneBase::IsCaptionVertical() const
{
	return FALSE;
}

AFX_INLINE CString CXTPDockingPaneBase::GetTitle() const
{
	return CString(_T(""));
}

AFX_INLINE XTPDockingPaneType CXTPDockingPaneBase::GetType() const
{
	return m_type;
}

AFX_INLINE CXTPDockingPaneBase* CXTPDockingPaneBase::GetContainer() const
{
	return m_pParentContainer;
}

AFX_INLINE void CXTPDockingPaneBase::SetDockingSite(CWnd* pFrame)
{
	m_pDockingSite = pFrame;
}

AFX_INLINE void CXTPDockingPaneBase::SetParentContainer(CXTPDockingPaneBase* pContainer)
{
	m_pParentContainer = pContainer;
}

AFX_INLINE void CXTPDockingPaneBase::OnCaptionButtonRectChanged(CXTPDockingPaneCaptionButton*)
{
}

AFX_INLINE BOOL
	CXTPDockingPaneBase::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* /*pButton*/)
{
	return TRUE;
}

AFX_INLINE CWnd* CXTPDockingPaneBase::GetDockingSite() const
{
	return m_pDockingSite;
}

AFX_INLINE HWND CXTPDockingPaneBase::GetPaneHwnd() const
{
	return 0;
}

AFX_INLINE CXTPDockingPaneCaptionButtons* CXTPDockingPaneBase::GetCaptionButtons() const
{
	return m_pCaptionButtons;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPDOCKINGPANEBASE_H__)
