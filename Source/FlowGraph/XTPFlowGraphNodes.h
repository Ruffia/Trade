// XTPFlowGraphNodes.h: interface for the CXTPFlowGraphNodes class.
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
#if !defined(__XTPFLOWGRAPHNODES_H__)
#	define __XTPFLOWGRAPHNODES_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphPage;
class CXTPFlowGraphNode;
class CXTPFlowGraphNodeCustom;

//========================================================================
// Summary:
//     Represents a collection of Nodes in a page or group.
// Remarks:
//     Nodes are the main objects that are displayed in the flow graph.
//     In terms of a database, they can be thought of as tables. Nodes contain
//     many connection points, which are like the fields in a table. Finally,
//     there are connections that are used to draw a relationship between the
//     fields of two tables.
//========================================================================
class _XTP_EXT_CLASS CXTPFlowGraphNodes : public CXTPCmdTarget
{
protected:
	//--------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphPage object.
	//--------------------------------------------------------------------
	CXTPFlowGraphNodes(CXTPFlowGraphPage* pPage);
	//--------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNodes object, handles cleanup and
	//     deallocation.
	//--------------------------------------------------------------------
	virtual ~CXTPFlowGraphNodes();

public:
	//--------------------------------------------------------------------
	// Summary:
	//     Removes all Nodes from the page or group.
	//--------------------------------------------------------------------
	void RemoveAll();
	//--------------------------------------------------------------------
	// Summary:
	//     Adds a node to a page or group.
	// Parameters:
	//     pNode - Reference to the Node to add.
	// Returns:
	//     A reference to the Node that was added.
	//--------------------------------------------------------------------
	CXTPFlowGraphNode* AddNode(CXTPFlowGraphNode* pNode);

	// TODO: description
	CXTPFlowGraphNodeCustom* AddCustomNode(CXTPFlowGraphNodeCustom* pNode = NULL);
	CXTPFlowGraphNodeCustom* AddResourceNode(UINT uResShapeID);

	//--------------------------------------------------------------------
	// Summary:
	//     Removes the specified Node.
	// Parameters:
	//     pNode - Reference to the Node to remove.
	//--------------------------------------------------------------------
	void Remove(CXTPFlowGraphNode* pNode);
	//--------------------------------------------------------------------
	// Summary:
	//     Removes the specified Node from the page or group.
	// Parameters:
	//     nIndex - Index of the node to remove.
	//--------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//--------------------------------------------------------------------
	// Summary:
	//     Finds and retrieves the specified Node.
	// Parameters:
	//     nId - ID of the node to find and retrieve.
	// Returns:
	//     If found, then a reference to the Node with the specified ID
	//     will be returned. Otherwise, a NULL value will be returned.
	//--------------------------------------------------------------------
	CXTPFlowGraphNode* FindNode(int nId) const;

	//--------------------------------------------------------------------
	// Summary:
	//     Retrieves the total number of Nodes in the page or group.
	// Returns:
	//     The total number of Nodes in the page or group.
	//--------------------------------------------------------------------
	int GetCount() const;
	//--------------------------------------------------------------------
	// Summary:
	//     Gets the Node at the specified index.
	// Parameters:
	//     nIndex - Index of the specified Node.
	// Returns:
	//     A reference to the specified Node.
	//--------------------------------------------------------------------
	CXTPFlowGraphNode* GetAt(int nIndex) const;

	//--------------------------------------------------------------------
	// Summary:
	//     Moves the specified Node to the front.
	// Parameters:
	//     pNode - Reference to the Node to move to the front.
	//--------------------------------------------------------------------
	void MoveToFront(CXTPFlowGraphNode* pNode);

public:
	//--------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a collection of Nodes
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load a collection of Nodes
	//     data to/from a storage.
	//--------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CXTPFlowGraphPage* m_pPage; // Pointer to the page this collection of nodes is displayed on.

	CArray<CXTPFlowGraphNode*, CXTPFlowGraphNode*> m_arrNodes; // Collection of nodes.

	friend class CXTPFlowGraphPage;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphNodes)
	DECLARE_ENUM_VARIANT(CXTPFlowGraphNodes)

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg LPDISPATCH OleAddNode();
	afx_msg LPDISPATCH OleAddCustomNode();
	afx_msg LPDISPATCH OleAddResourceNode(int nResShapeID);
	afx_msg LPDISPATCH OleFindNode(int nId);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPFlowGraphNodes::GetCount() const
{
	return (int)m_arrNodes.GetSize();
}
AFX_INLINE CXTPFlowGraphNode* CXTPFlowGraphNodes::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrNodes.GetSize() ? m_arrNodes[nIndex] : NULL;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHNODES_H__)
