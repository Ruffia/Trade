// XTPDockingPaneLayout.h : interface for the CXTPDockingPaneLayout class.
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
#if !defined(__XTPDOCKINGPANELAYOUT_H__)
#	define __XTPDOCKINGPANELAYOUT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockingPaneBase;
class CXTPDockingPaneSplitterContainer;
class CXTPDockingPaneBase;
class CXTPDockingPaneAutoHidePanel;
class CXTPDockingPaneManager;
class CXTPPropExchange;
class CXTPDockingPaneTabbedContainer;
class CXTPDockingPaneSplitterWnd;
class CXTPDockingPaneClientContainer;

namespace XTPXML
{
struct IXMLDOMNode;
};

//===========================================================================
// Summary:
//     CXTPDockingPaneLayout is a standalone class. It is used to manipulate
//     a pane's layouts.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneLayout : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPDockingPaneLayout);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneLayout object.
	// Parameters:
	//     pManager - Pointer to a CXTPDockingPaneManager object.
	// Returns:
	//     A CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneLayout(CXTPDockingPaneManager* pManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneLayout object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneLayout();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to copy one layout to another.
	// Parameters:
	//     pLayout - Source Layout.
	//-----------------------------------------------------------------------
	void Copy(const CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save the layout to the registry.
	// Parameters:
	//     lpszSection - Registry section.
	//-----------------------------------------------------------------------
	void Save(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the layout from the registry.
	// Parameters:
	//     lpszSection - Registry section.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Load(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a list of layout's panes.
	// Returns:
	//     A list of the layout's panes.
	// See Also:
	//     CXTPDockingPaneInfoList
	//-----------------------------------------------------------------------
	CXTPDockingPaneInfoList& GetPaneList() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	//-----------------------------------------------------------------------
	void Serialize(CArchive& ar);

#	ifndef _XTP_EXCLUDE_XML

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save the layout to the file.
	// Parameters:
	//     lpszFileName - The full path and file name.
	//     lpszSection  - Registry section.
	//-----------------------------------------------------------------------
	void SaveToFile(LPCTSTR lpszFileName, LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the layout from the file.
	// Parameters:
	//     lpszFileName - The full path and file name.
	//     lpszSection  - Registry section.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(LPCTSTR lpszFileName, LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save the layout to the XML node.
	// Parameters:
	//     xmlNode     - XML node to save.
	//     lpszSection - Registry section.
	//-----------------------------------------------------------------------
	void SaveToNode(XTPXML::IXMLDOMNode* pNode, LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the layout from the XML node.
	// Parameters:
	//     xmlNode      - XML node to load.
	//     lpszSection  - Registry section.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL LoadFromNode(XTPXML::IXMLDOMNode* pNode, LPCTSTR lpszSection);

#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the layout is a user created layout.
	// Returns:
	//     TRUE if the layout is a user created layout.
	//-----------------------------------------------------------------------
	BOOL IsUserLayout() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the layout is valid.
	// Returns:
	//     TRUE if the layout is valid.
	//-----------------------------------------------------------------------
	BOOL IsValid() const;

public:
	void DestroyPane(CXTPDockingPane* pPane);

	CXTPDockingPane* FindPane(int nID) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DoPropExchange(CXTPPropExchange* pPX);

private:
	void Push(CXTPDockingPaneBase* pPane)
	{
		m_lstStack.AddTail(pPane);
	}
	void HidePane(CXTPDockingPaneBase* pPane);
	void OnSizeParent(CWnd* pParent, CRect rect, LPVOID lParam);
	void Free();
	void _FreeEmptyPanes();
	XTPDockingPaneDirection _GetPaneDirection(const CXTPDockingPaneBase* pPane) const;

	void _Save(CXTPPropExchange* pPX);
	BOOL _Load(CXTPPropExchange* pPX);
	BOOL _FindTabbedPaneToHide(CXTPDockingPaneAutoHidePanel* pPanel, CXTPDockingPaneBase* pPane);
	void _AddPanesTo(CXTPDockingPaneTabbedContainer* pContainer, CXTPDockingPaneBaseList& lst,
					 DWORD dwIgnoredOptions);

	void MoveToTail(CXTPDockingPaneBase* pBase);

private:
	CXTPDockingPaneSplitterContainer* m_pTopContainer;
	CXTPDockingPaneBase* m_pClient;

	CXTPDockingPaneInfoList m_lstPanes;
	CXTPDockingPaneBaseList m_lstStack;

	XTP_DOCKINGPANE_INFO* FindPaneInfo(CXTPDockingPane* pPane);
	void RepositionMargins(CRect& rect, const CRect& lpszClientMargins);

	CXTPDockingPaneAutoHidePanel* m_wndPanels[4];
	CXTPDockingPaneSplitterWnd* m_wndMargins[4];

	BOOL m_bUserLayout;
	CXTPDockingPaneManager* m_pManager;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPDockingPaneLayout);
	DECLARE_ENUM_VARIANT(CXTPDockingPaneLayout);

public:
	void OleDoPropExchange(LPDISPATCH lpPropExchage);
	long OleGetItemCount();
	LPDISPATCH OleGetItem(int nIndex);
	LPDISPATCH OleGetPane(int nIndex);
	long CXTPDockingPaneLayout::OleGetContainersCount();
	LPDISPATCH CXTPDockingPaneLayout::OleGetContainer(int nIndex);
	long OleGetPaneCount();
	LPDISPATCH OleGetClientContainer();
	LPDISPATCH OleGetTopContainer();

	static CXTPDockingPaneLayout* AFX_CDECL FromDispatch(LPDISPATCH pDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/

private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneBase;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CDockingPaneSite;
	friend class CDockingPaneCtrl;
	friend class CXTPDockingPaneSidePanel;
};

AFX_INLINE BOOL CXTPDockingPaneLayout::IsUserLayout() const
{
	return m_bUserLayout;
}
AFX_INLINE CXTPDockingPaneInfoList& CXTPDockingPaneLayout::GetPaneList() const
{
	return (CXTPDockingPaneInfoList&)m_lstPanes;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPDOCKINGPANELAYOUT_H__)
