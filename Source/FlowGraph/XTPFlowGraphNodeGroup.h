// XTPFlowGraphNodeGroup.h: interface for the CXTPFlowGraphNodeGroup class.
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
#if !defined(__XTPFLOWGRAPHNODEGROUP_H__)
#	define __XTPFLOWGRAPHNODEGROUP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphPage;
class CXTPFlowGraphNodeGroups;
class CXTPFlowGraphNode;

//========================================================================
// Summary:
//     Represents a group of Nodes.
// Remarks:
//     A group is just a "box" that is used to visually group Nodes
//     together. By default, the group will only be shown when a Node in
//     the group is selected, but you can choose to always show the
//     groups with ShowNodeGroupsAlways.
//========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphNodeGroup : public CXTPFlowGraphElement
{
	DECLARE_DYNAMIC(CXTPFlowGraphNodeGroup)
public:
	//--------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphNodeGroup object.
	//--------------------------------------------------------------------
	CXTPFlowGraphNodeGroup();
	//--------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNodeGroup object, handles cleanup and
	//     deallocation.
	//--------------------------------------------------------------------
	virtual ~CXTPFlowGraphNodeGroup();

public:
	//--------------------------------------------------------------------
	// Summary:
	//     Adds a Node to the Node group.
	// Parameters:
	//     pNode - Reference to the Node to add to the group.
	//--------------------------------------------------------------------
	void AddNode(CXTPFlowGraphNode* pNode);
	//--------------------------------------------------------------------
	// Summary:
	//     Removes a Node from the Node group.
	// Parameters:
	//     pNode - Reference to the Node to remove from the group.
	//--------------------------------------------------------------------
	void RemoveNode(CXTPFlowGraphNode* pNode);

	//--------------------------------------------------------------------
	// Summary:
	//     Removes all nodes from this group.
	//--------------------------------------------------------------------
	void RemoveAll();
	//--------------------------------------------------------------------
	// Summary:
	//     Removes this group from the flow graph's collection of groups.
	//--------------------------------------------------------------------
	void Remove();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets a reference to the parent FlowGraph control.
	// Returns:
	//     A reference to the parent FlowGraph control.
	//--------------------------------------------------------------------
	CXTPFlowGraphControl* GetControl() const;
	//--------------------------------------------------------------------
	// Summary:
	//     Gets a reference to the page that this group belongs to.
	// Returns:
	//     A reference to the page that this group belongs to.
	//--------------------------------------------------------------------
	CXTPFlowGraphPage* GetPage() const;

	//--------------------------------------------------------------------
	// Summary:
	//     Get the total number of Nodes that are in the group.
	// Returns:
	//     The total number of Nodes that are in the group.
	//--------------------------------------------------------------------
	int GetCount() const;
	//--------------------------------------------------------------------
	// Summary:
	//     Retrieves the Node at the specified index from the group's collection
	//     of Nodes.
	// Parameters:
	//     nIndex - Index of the specified Node to retrieve.
	// Returns:
	//     A reference to the specified Node.
	//--------------------------------------------------------------------
	CXTPFlowGraphNode* GetAt(int nIndex) const;

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle for the group.
	// Returns:
	//     The bounding rectangle for the group.
	//--------------------------------------------------------------------
	CRect GetWorkRect() const;

	//--------------------------------------------------------------------
	// Summary:
	//     Sets the common caption for each Node in the group.
	// Parameters:
	//     lpszCaption - New common caption for each Node in the group.
	//--------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);
	//--------------------------------------------------------------------
	// Summary:
	//     Gets the common caption for each Node in the group. If any Nodes in the group
	//     have a different caption, then the return value will be an empty string.
	// Returns:
	//     If all Nodes in the group share the same caption, then the common caption
	//     for each Node in the group will be returned. If any Nodes in the group
	//     have a different caption, then an empty string will be returned.
	//--------------------------------------------------------------------
	CString GetCaption() const;

protected:
	CArray<CXTPFlowGraphNode*, CXTPFlowGraphNode*> m_arrNodes; // Collection of nodes that are in
															   // this group.
	CXTPFlowGraphNodeGroups* m_pGroups; // Pointer to the collection of groups that this group
										// belongs to.
	CRect m_rcWorkRect;					// Rectangle displayed for this group.

	friend class CXTPFlowGraphNodeGroups;
	friend class CXTPFlowGraphPage;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphNodeGroup)

	afx_msg void OleSetCaption(LPCTSTR lpszCaption);
	afx_msg BSTR OleGetCaption();

	afx_msg LPDISPATCH OleGetPage();
	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);

	afx_msg void OleAddNode(LPDISPATCH Node);
	afx_msg void OleRemoveNode(LPDISPATCH Node);
	DECLARE_ENUM_VARIANT(CXTPFlowGraphNodeGroup)

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPFlowGraphNodeGroup::GetCount() const
{
	return (int)m_arrNodes.GetSize();
}
AFX_INLINE CXTPFlowGraphNode* CXTPFlowGraphNodeGroup::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrNodes.GetSize() ? m_arrNodes[nIndex] : NULL;
}
AFX_INLINE CRect CXTPFlowGraphNodeGroup::GetWorkRect() const
{
	return m_rcWorkRect;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHNODEGROUP_H__)
