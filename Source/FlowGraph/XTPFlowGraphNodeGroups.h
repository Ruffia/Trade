// XTPFlowGraphNodeGroups.h: interface for the CXTPFlowGraphNodeGroups class.
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
#if !defined(__XTPFLOWGRAPHNODEGROUPS_H__)
#	define __XTPFLOWGRAPHNODEGROUPS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphPage;
class CXTPFlowGraphNode;

//========================================================================
// Summary:
//     Represents a collection of groups in a page.
// Remarks:
//     A group is just a "box" that is used to visually group Nodes
//     together. By default, the group will only be shown when a Node in
//     the group is selected, but you can choose to always show the
//     groups with ShowNodeGroupsAlways.
//========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphNodeGroups : public CXTPCmdTarget
{
protected:
	//--------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphNodeGroups object.
	//--------------------------------------------------------------------
	CXTPFlowGraphNodeGroups(CXTPFlowGraphPage* pPage);
	//--------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNodeGroups object, handles cleanup and
	//     deallocation.
	//--------------------------------------------------------------------
	virtual ~CXTPFlowGraphNodeGroups();

public:
	//--------------------------------------------------------------------
	// Summary:
	//     Removes all groups from the collection.
	//--------------------------------------------------------------------
	void RemoveAll();
	//--------------------------------------------------------------------
	// Summary:
	//     Adds a group to the page's collection of groups.
	// Returns:
	//     A reference to the group added.
	//--------------------------------------------------------------------
	CXTPFlowGraphNodeGroup* AddGroup();

	//--------------------------------------------------------------------
	// Summary:
	//     Retrieves the total number of groups in the collection.
	// Returns:
	//     The total number of groups in the collection.
	//--------------------------------------------------------------------
	int GetCount() const;
	//--------------------------------------------------------------------
	// Summary:
	//     Gets the group at a specified index in the collection.
	// Parameters:
	//     nIndex - Index of the group in the collection.
	// Returns:
	//     A reference to the group at the specified index in the collection.
	//--------------------------------------------------------------------
	CXTPFlowGraphNodeGroup* GetAt(int nIndex) const;

	//--------------------------------------------------------------------
	// Summary:
	//     Removes a specific group from the collection.
	// Parameters:
	//     pGroup - Reference to the group to remove.
	//--------------------------------------------------------------------
	void Remove(CXTPFlowGraphNodeGroup* pGroup);

	//--------------------------------------------------------------------
	// Summary:
	//     Retrieves a reference to the flow graph page that this collection
	//     of groups are displayed on.
	// Returns:
	//     A reference to the flow graph page that this collection of groups
	//     are displayed on.
	//--------------------------------------------------------------------
	CXTPFlowGraphPage* GetPage() const;

protected:
	CXTPFlowGraphPage* m_pPage; // Pointer to the page that this collection of groups is displayed
								// on.

	CArray<CXTPFlowGraphNodeGroup*, CXTPFlowGraphNodeGroup*> m_arrGroups; // Collection of groups.

	friend class CXTPFlowGraphPage;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphNodeGroups)
	DECLARE_ENUM_VARIANT(CXTPFlowGraphNodeGroups)

	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg int OleGetItemCount();

	afx_msg LPDISPATCH OleAddGroup();
	afx_msg void OleRemoveGroup(LPDISPATCH Group);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphNodeGroups::GetPage() const
{
	return m_pPage;
}
AFX_INLINE int CXTPFlowGraphNodeGroups::GetCount() const
{
	return (int)m_arrGroups.GetSize();
}
AFX_INLINE CXTPFlowGraphNodeGroup* CXTPFlowGraphNodeGroups::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrGroups.GetSize() ? m_arrGroups[nIndex] : NULL;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHNODEGROUPS_H__)
