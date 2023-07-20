// XTPRibbonGroups.h: interface for the CXTPRibbonGroups class.
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
#if !defined(__XTPRIBBONGROUPS_H__)
#	define __XTPRIBBONGROUPS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonGroup;
class CXTPRibbonTab;

//===========================================================================
// Summary:
//     Represents a collection of CXTPRibbonGroup objects.
// Remarks:
//     A CXTPRibbonTab uses the CXTPRibbonGroups object for its groups
//     collection to store all of the RibbonGroup objects that have been
//     added to the RibbonTab. A CXTPRibbonGroup is used to hold items
//     such as buttons, pop-ups, edit controls, etc.
//
//     Groups are added to the groups collection using the Add
//     and InsertAt methods.
// See Also:
//     Add, InsertAt
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonGroups : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonGroups object.
	//-----------------------------------------------------------------------
	CXTPRibbonGroups();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonGroups object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonGroups();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Copies all groups from a CXTPRibbonGroups object.
	// Parameters:
	//     pGroups - Pointer to a CXTPRibbonGroups object to copy from.
	//-----------------------------------------------------------------------
	void Copy(CXTPRibbonGroups* pGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all groups.
	// See Also:
	//     InsertAt, Remove, GetCount, GetAt, Add
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified group.
	// Parameters:
	//     nIndex - Index of the group to be removed.
	//     pGroup - Pointer to the group to be removed.
	// See Also:
	//     InsertAt, Add, GetCount, GetAt, RemoveAll
	//-----------------------------------------------------------------------
	void Remove(int nIndex);
	void Remove(CXTPRibbonGroup* pGroup); // <combine CXTPRibbonGroups::Remove@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a CXTPRibbonGroup to a CXTPRibbonTab at a specific location.
	// Parameters:
	//     nIndex      - Position within the tab's groups collection to
	//                   insert the new CXTPRibbonGroup.
	//     lpszCaption - Text to display in the group title bar.
	//                   This is the group's caption.
	//     nId         - Identifier of the new group.
	//     pGroup      - New group to insert.
	// Remarks:
	//     InsertAt adds a group at a specific location within the tab's
	//     groups collection. Groups will be displayed in the order they
	//     are added. This means the group with an index of zero (0) will
	//     be displayed first to the far left, then to the immediate right
	//     will be the group with an index of one (1), and so on.
	// Returns:
	//     A pointer to the CXTPRibbonGroup object added to the CXTPRibbonTab
	//     groups collection.
	// See Also:
	//     Add, Remove, GetCount, GetAt, RemoveAll
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* InsertAt(int nIndex, LPCTSTR lpszCaption, int nId = -1);
	CXTPRibbonGroup*
		InsertAt(int nIndex,
				 CXTPRibbonGroup* pGroup); // <combine CXTPRibbonGroups::InsertAt@int@LPCTSTR@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a CXTPRibbonGroup to a CXTPRibbonTab.
	// Parameters:
	//     lpszCaption - Text to display in the group title bar.
	//                   This is the group's caption.
	//     nId         - Identifier of new group.
	// Remarks:
	//     Tabs in the ribbon bar contain one or more CXTPRibbonGroup objects.
	//     Buttons, pop-ups, etc. can then be added to the groups.
	// Returns:
	//     A pointer to the CXTPRibbonGroup object added to the CXTPRibbonTab
	//     groups collection.
	// See Also:
	//     InsertAt, Remove, GetCount, GetAt, RemoveAll
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* Add(LPCTSTR lpszCaption, int nId = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the total number of CXTPRibbonGroup objects that have
	//     been added to the CXTPRibbonTab.
	// Returns:
	//     The total number of CXTPRibbonGroup objects that have been added
	//     to the CXTPRibbonTab.
	// See Also:
	//     InsertAt, Remove, Add, GetAt, RemoveAll
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonGroup object that has been added to the CXTPRibbonTab
	//     at a specified index.
	// Parameters:
	//     nIndex - Index of the group to retrieve within the
	//              groups collection.
	// Remarks:
	//     GetAt retrieves the CXTPRibbonGroup from the m_arrGroups groups collection
	//     that contains all of the CXTPRibbonGroup objects that have been added
	//     to the CXTPRibbonTab.
	// Returns:
	//     The CXTPRibbonGroup at the specified index.
	// See Also:
	//     InsertAt, Remove, GetCount, Add, RemoveAll
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines which ribbon group, if any,
	//     is at a specified position.
	// Parameters:
	//     point - Point to test.
	// Returns:
	//     The CXTPRibbonGroup at the specified position, if any,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* HitTest(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonBar object that the CXTPRibbonGroup belongs to.
	// Returns:
	//     A pointer to the CXTPRibbonBar object that the CXTPRibbonGroup
	//     belongs to. This is the ribbon bar that the group is displayed in.
	// See Also:
	//     CXTPRibbonBar
	//-----------------------------------------------------------------------
	CXTPRibbonBar* GetRibbonBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CXTPRibbonTab object that the CXTPRibbonGroup belongs to.
	// Returns:
	//     A pointer to the CXTPRibbonTab object that the CXTPRibbonGroup
	//     belongs to. This is the tab that the group is displayed in.
	// See Also:
	//     CXTPRibbonTab
	//-----------------------------------------------------------------------
	CXTPRibbonTab* GetParentTab() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called internally to reposition groups and controls.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     nLength  - Length of the parent bar.
	//     dwMode   - Mode of the parent bar.
	//     rcBorder - Border to be set.
	// See Also:
	//     CXTPRibbonBar::RepositionGroups
	//-----------------------------------------------------------------------
	void CalcDynamicSize(CDC* pDC, int nLength, DWORD dwMode, const CRect& rcBorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from/to an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the group scroll position.
	// Returns:
	//     The group scroll position.
	//-----------------------------------------------------------------------
	int GetScrollPos() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	int _CalcSmartLayoutToolBar(int* pWidth);
	void _ReduceSmartLayoutToolBar(CDC* pDC, int* pWidth, int nWidth);
	//}}AFX_CODEJOCK_PRIVATE

private:
	void RefreshIndexes();

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	afx_msg LPDISPATCH OleInsertGroup(int nIndex, LPCTSTR lpszTitle, int nId);
	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg LPDISPATCH OleAddGroup(LPCTSTR lpszTitle, int nId);
	afx_msg LPDISPATCH OleGetRibbonTab();
	afx_msg void OleRemoveAt(int nIndex);

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonGroups);
	DECLARE_INTERFACE_MAP()
	DECLARE_ENUM_VARIANT(CXTPRibbonGroups);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CArray<CXTPRibbonGroup*, CXTPRibbonGroup*> m_arrGroups; // Collection of groups.
	CXTPRibbonTab* m_pParentTab; // A pointer to the CXTPRibbonTab object the CXTPRibbonGroup
								 // belongs to. This is the tab that the group is displayed in.
	int m_nGroupsScrollPos;		 // Group scroll position.
	friend class CXTPRibbonTab;
	friend class CXTPRibbonBar;
};

AFX_INLINE int CXTPRibbonGroups::GetCount() const
{
	return (int)m_arrGroups.GetSize();
}
AFX_INLINE CXTPRibbonGroup* CXTPRibbonGroups::GetAt(int nIndex) const
{
	ASSERT(nIndex < m_arrGroups.GetSize());
	return (nIndex >= 0 && nIndex < m_arrGroups.GetSize()) ? m_arrGroups.GetAt(nIndex) : NULL;
}
AFX_INLINE CXTPRibbonTab* CXTPRibbonGroups::GetParentTab() const
{
	return m_pParentTab;
}
AFX_INLINE int CXTPRibbonGroups::GetScrollPos() const
{
	return m_nGroupsScrollPos;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONGROUPS_H__)
