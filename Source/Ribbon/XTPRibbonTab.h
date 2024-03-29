// XTPRibbonTab.h: interface for the CXTPRibbonTab class.
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
#if !defined(__XTPRIBBONTAB_H__)
#	define __XTPRIBBONTAB_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonGroups;
class CXTPRibbonBar;
class CXTPRibbonGroup;
class CXTPRibbonTab;
class CXTPCommandBar;
class CXTPPropExchange;

//-----------------------------------------------------------------------
// Summary:
//     Ribbon tab context color
// Example:
// <code>
// pTab = pRibbonBar->AddTab(ID_TAB_CHARTFORMAT);
// pTab->SetContextTab(xtpRibbonTabContextColorGreen, ID_TAB_CONTEXTCHART);
// </code>
// See Also:
//     CXTPRibbonTab, CXTPRibbonTab::SetContextColor
//-----------------------------------------------------------------------
enum XTPRibbonTabContextColor
{
	xtpRibbonTabContextColorNone,   // Default color.
	xtpRibbonTabContextColorBlue,   // Blue context color.
	xtpRibbonTabContextColorYellow, // Yellow context color.
	xtpRibbonTabContextColorGreen,  // Green context color.
	xtpRibbonTabContextColorRed,	// Red context color.
	xtpRibbonTabContextColorPurple, // Purple context color.
	xtpRibbonTabContextColorCyan,   // Cyan context color.
	xtpRibbonTabContextColorOrange  // Orange context color.
};

//{{AFX_CODEJOCK_PRIVATE

// Context headers internal classes

class _XTP_EXT_CLASS CXTPRibbonTabContextHeader
{
public:
	CXTPRibbonTabContextHeader(CXTPRibbonTab* pFirstTab);

public:
	CXTPRibbonTab* m_pFirstTab;
	CXTPRibbonTab* m_pLastTab;
	CString m_strCaption;
	XTPRibbonTabContextColor m_color;
	CRect m_rcRect;
};

class _XTP_EXT_CLASS CXTPRibbonTabContextHeaders
{
public:
	CXTPRibbonTabContextHeaders();
	~CXTPRibbonTabContextHeaders();

public:
	void RemoveAll();
	int GetCount() const;
	void Add(CXTPRibbonTabContextHeader* pHeader);
	CXTPRibbonTabContextHeader* GetHeader(int nIndex) const;
	CXTPRibbonTabContextHeader* HitTest(CPoint pt) const;

protected:
	CArray<CXTPRibbonTabContextHeader*, CXTPRibbonTabContextHeader*> m_arrHeaders;
};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Represents a tab in the Ribbon Bar.
// Remarks:
//     Tabs in the ribbon bar are used to display different groups of items.
//     The CXTPRibbonBar.InsertTab method adds a new CXTPRibbonTab object to
//     the tabs collection of the ribbon bar.
//
//     Tabs in the ribbon bar contain one or more CXTPRibbonGroup objects.
//     Buttons, pop-ups, etc. can then be added to the groups.
//
//     The CXTPRibbonBar::GetTabCount property can be used to determine the
//     total number of tabs that have been added.
// See Also:
//     CXTPRibbonBar.InsertTab, CXTPRibbonBar::GetTabCount, CXTPRibbonBar::GetTab
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonTab : public CXTPTabManagerItem
{
	DECLARE_SERIAL(CXTPRibbonTab)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonTab object.
	//-----------------------------------------------------------------------
	CXTPRibbonTab();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonTab object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonTab();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the collection of CXTPRibbonGroup objects that have been added
	//     to the CXTPRibbonTab.
	// Remarks:
	//     A CXTPRibbonGroup is used to hold buttons, pop-ups, edit controls,
	//     combo box controls, etc. The group organizes the different types of
	//     controls in a CXTPRibbonTab so it is easy to see what functionality
	//     the controls should perform. Groups are added to the group collection
	//     using the AddGroup and InsertGroup methods. Groups will be displayed
	//     in the order they are added. This means the group with an index of
	//     zero (0) will be displayed first to the far left, then to the immediate
	//     right will be the group with an index of one (1), and so on.
	// Returns:
	//     A pointer to the collection of CXTPRibbonGroup objects that have
	//     been added to the CXTPRibbonTab.
	// See Also:
	//     CXTPRibbonGroups::GetAt, CXTPRibbonGroups::InsertAt, CXTPRibbonGroups::Add,
	//     CXTPRibbonGroups.GetCount
	//-----------------------------------------------------------------------
	CXTPRibbonGroups* GetGroups() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonBar object that the CXTPRibbonTab belongs to.
	// Returns:
	//     A pointer to the CXTPRibbonBar object that the CXTPRibbonTab
	//     belongs to. This is the RibbonBar that the tab is displayed in.
	// See Also:
	//     CXTPRibbonBar
	//-----------------------------------------------------------------------
	CXTPRibbonBar* GetRibbonBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent command bar.
	// Returns:
	//     A pointer to the parent CXTPCommandBar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetParent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text caption of the tab.
	// Returns:
	//     The text caption of the tab. This is the text displayed in
	//     the tab button.
	// See Also:
	//     SetCaption
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

	//----------------------------------------------------------------------
	// Summary:
	//     Copies the tab.
	// Parameters:
	//     pRibbonTab - Pointer to a source CXTPRibbonTab object.
	//----------------------------------------------------------------------
	virtual void Copy(CXTPRibbonTab* pRibbonTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a CXTPRibbonGroup to a CXTPRibbonTab.
	// Parameters:
	//     lpszCaption - Text to display in the group's title bar.
	//                   This is the group's caption.
	//     nID         - Identifier of the group.
	// Returns:
	//     A pointer to the CXTPRibbonGroup object added to the CXTPRibbonTab's
	//     groups collection (CXTPRibbonTab::GetGroups).
	// See Also:
	//     CXTPRibbonGroup::SetCaption, CXTPRibbonGroup::SetCaption, CXTPRibbonTab,
	//     CXTPRibbonTab::GetGroups
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* AddGroup(LPCTSTR lpszCaption);
	CXTPRibbonGroup* AddGroup(int nID); // <combine CXTPRibbonTab::AddGroup@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a group by its identifier.
	// Parameters:
	//     nId - Identifier of the group to be retrieved.
	// Returns:
	//     A pointer to the CXTPRibbonGroup object with the specified identifier.
	// See Also:
	//     FindTab
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* FindGroup(int nId) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab color.
	// Returns:
	//     The tab color; one of the values defined by the
	//     XTPRibbonTabContextColor enumeration.
	// See Also:
	//     XTPRibbonTabContextColor
	//-----------------------------------------------------------------------
	XTPRibbonTabContextColor GetContextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab color.
	// Parameters:
	//     color - New tab color; must be one of the values defined by the
	//                            XTPRibbonTabContextColor enumeration.
	// See Also:
	//     XTPRibbonTabContextColor
	//-----------------------------------------------------------------------
	void SetContextColor(XTPRibbonTabContextColor color);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the context tab header text.
	// Returns:
	//     The context tab header text.
	// See Also:
	//     SetContextCaption, SetContextColor
	//-----------------------------------------------------------------------
	CString GetContextCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the context tab header text.
	// Parameters:
	//     lpszContextCaption - New text to be set.
	// See Also:
	//     GetContextCaption
	//-----------------------------------------------------------------------
	void SetContextCaption(LPCTSTR lpszContextCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the customize dialog specific tab text.
	// Returns:
	//     The customize dialog specific tab text.
	// See Also:
	//     GetCaption, SetCaption
	//-----------------------------------------------------------------------
	CString GetCustomizeCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the customize dialog specific tab text.
	// Parameters:
	//     lpszCustomizeCaption - New text to be set.
	// See Also:
	//     GetCaption, SetCaption
	//-----------------------------------------------------------------------
	void SetCustomizeCaption(LPCTSTR lpszCustomizeCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the customize dialog specific tab text.
	// Parameters:
	//     nCustomizeCaption - Resource ID of the new text to be set.
	// See Also:
	//     GetCaption, SetCaption
	//-----------------------------------------------------------------------
	void SetCustomizeCaption(UINT nCustomizeCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the context tab color and text.
	// Parameters:
	//     color - New color to be set.
	//     nContextID - Resource ID of the new text to be set.
	//     lpszContextCaption - New text to be set.
	// See Also:
	//     GetContextCaption, GetContextColor
	//-----------------------------------------------------------------------
	void SetContextTab(XTPRibbonTabContextColor color, int nContextID);
	void SetContextTab(XTPRibbonTabContextColor color,
					   LPCTSTR lpszContextCaption =
						   NULL); // <combine
								  // CXTPRibbonTab::SetContextTab@XTPRibbonTabContextColor@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab's ID.
	// Remarks:
	//     The ID is used to identify the tab.
	// Returns:
	//     The tab's ID.
	// See Also:
	//     SetID
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab's ID.
	// Parameters:
	//     nId - New ID to be set.
	// See Also:
	//     GetID
	//-----------------------------------------------------------------------
	void SetID(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonTabContextHeader associated with the tab.
	// Returns:
	//     A pointer to the CXTPRibbonTabContextHeader associated with the tab.
	//-----------------------------------------------------------------------
	CXTPRibbonTabContextHeader* GetContextHeader() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show/hide the tab.
	// Parameters:
	//     bVisible - TRUE to show the tab, FALSE to hide the tab.
	// See Also:
	//     IsVisible
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from/to an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the keyboard tip for the control.
	// Parameters:
	//     lpszKeyboardTip - New keyboard tip to be set.
	//-----------------------------------------------------------------------
	void SetKeyboardTip(LPCTSTR lpszKeyboardTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the keyboard tip of the control.
	// Returns:
	//     The keyboard tip of the control.
	//-----------------------------------------------------------------------
	CString GetKeyboardTip() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called before removing a tab from the ribbon tabs collection.
	//-------------------------------------------------------------------------
	void OnRemoved();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to route and dispatch command messages
	//     and to handle the update of command user-interface objects.
	// Parameters:
	//     nID          - Command ID.
	//     nCode        - Command notification code.
	//     pExtra       - Used according to the value of nCode.
	//     pHandlerInfo - If not NULL, then OnCmdMsg fills in the pTarget
	//                    and pmf members of the pHandlerInfo structure
	//                    instead of dispatching the command. Typically,
	//                    this parameter should be NULL.
	// Returns:
	//     Nonzero if the message is handled, otherwise 0.
	//-----------------------------------------------------------------------
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	LPDISPATCH OleGetRibbonBar();
	LPDISPATCH OleGetGroups();

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonTab);
	DECLARE_INTERFACE_MAP()
	void OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CXTPRibbonGroups* m_pGroups; // Collection of CXTPRibbonGroup objects that have been added to
								 // the CXTPRibbonTab.
	CXTPRibbonBar* m_pRibbonBar; // The CXTPRibbonBar object the CXTPRibbonTab belongs to.

	CXTPCommandBar* m_pParent; // This is the ribbon bar that the tab is displayed in.
	int m_nId;				   // Identifier of the tab.
	XTPRibbonTabContextColor m_nContextColor; // Context color.
	CString m_strContextCaption;			  // Context caption.
	CString m_strCustomizeCaption; // Caption used by Customize dialog. If empty, a default caption
								   // is displayed.
	CXTPRibbonTabContextHeader* m_pContextHeader; // Context header.
	CString m_strKeyboardTip;					  // Keyboard tip.

	friend class CXTPRibbonBar;
	friend class CXTPRibbonTabContextHeader;
	friend class CXTPRibbonTabPopupToolBar;
	friend class CXTPRibbonControlTab;
};

AFX_INLINE void CXTPRibbonTab::SetKeyboardTip(LPCTSTR lpszKeyboardTip)
{
	m_strKeyboardTip = lpszKeyboardTip;
}
AFX_INLINE CString CXTPRibbonTab::GetKeyboardTip() const
{
	return m_strKeyboardTip;
}

AFX_INLINE CXTPRibbonGroups* CXTPRibbonTab::GetGroups() const
{
	return m_pGroups;
}
AFX_INLINE void CXTPRibbonTab::SetContextTab(XTPRibbonTabContextColor color,
											 LPCTSTR lpszContextCaption)
{
	m_nContextColor		= color;
	m_strContextCaption = lpszContextCaption;
}
AFX_INLINE int CXTPRibbonTab::GetID() const
{
	return m_nId;
}
AFX_INLINE void CXTPRibbonTab::SetID(int nID)
{
	m_nId = nID;
}
AFX_INLINE CString CXTPRibbonTab::GetContextCaption() const
{
	return m_strContextCaption;
}
AFX_INLINE CString CXTPRibbonTab::GetCustomizeCaption() const
{
	return m_strCustomizeCaption.IsEmpty() ? GetCaption() : m_strCustomizeCaption;
}
AFX_INLINE void CXTPRibbonTab::SetCustomizeCaption(LPCTSTR lpszCustomizeCaption)
{
	m_strCustomizeCaption = lpszCustomizeCaption;
}
AFX_INLINE void CXTPRibbonTab::SetCustomizeCaption(UINT nCustomizeCaption)
{
	VERIFY(m_strCustomizeCaption.LoadString(nCustomizeCaption));
}
AFX_INLINE XTPRibbonTabContextColor CXTPRibbonTab::GetContextColor() const
{
	return m_nContextColor;
}
AFX_INLINE void CXTPRibbonTab::SetContextColor(XTPRibbonTabContextColor color)
{
	m_nContextColor = color;
}
AFX_INLINE void CXTPRibbonTab::SetContextCaption(LPCTSTR lpszContextCaption)
{
	m_strContextCaption = lpszContextCaption;
}
AFX_INLINE CXTPRibbonTabContextHeader* CXTPRibbonTab::GetContextHeader() const
{
	return m_pContextHeader;
}
AFX_INLINE CXTPCommandBar* CXTPRibbonTab::GetParent() const
{
	return m_pParent;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONTAB_H__)
