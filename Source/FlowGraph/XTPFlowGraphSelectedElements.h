// XTPFlowGraphSelectedElements.h: interface for the CXTPFlowGraphSelectedElements class.
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
#if !defined(__XTPFLOWGRAPHSELECTEDELEMENTS_H__)
#	define __XTPFLOWGRAPHSELECTEDELEMENTS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphControl;
class CXTPFlowGraphPage;

//============================================================================
// Summary:
//     Collection of selected elements in the flow graph.
// Remarks:
//     Selected elements will include Nodes, connections, and connection
//     points. This collection can be used to add/remove elements
//     to/from the selection. Elements can be selected with the mouse or
//     by using methods in code.
//============================================================================
class _XTP_EXT_CLASS CXTPFlowGraphSelectedElements : public CXTPCmdTarget
{
public:
	//------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphSelectedElements object.
	//------------------------------------------------------------------------
	CXTPFlowGraphSelectedElements(CXTPFlowGraphPage* pPage);
	//------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphSelectedElements object, handles cleanup
	//     and deallocation.
	//------------------------------------------------------------------------
	virtual ~CXTPFlowGraphSelectedElements();

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Removes all elements from the current selection.
	//------------------------------------------------------------------------
	void Clear();

	//------------------------------------------------------------------------
	// Summary:
	//     Sets the current selection to a specified element.
	// Parameters:
	//     pElement - The element to set the current selection to; can be any
	//                valid flow graph element (connection, node).
	//------------------------------------------------------------------------
	void SetSelection(CXTPFlowGraphElement* pElement);
	//------------------------------------------------------------------------
	// Summary:
	//     Adds an element to the current selection.
	// Parameters:
	//     pElement - The element to add to the current selection; can be any
	//                valid flow graph element (connection, node).
	//------------------------------------------------------------------------
	void AddSelection(CXTPFlowGraphElement* pElement);

	//------------------------------------------------------------------------
	// Summary:
	//     Removes an element from the current selection.
	// Parameters:
	//     pElement - The element to remove from the current selection; can be
	//                any valid flow graph element (connection, node).
	//------------------------------------------------------------------------
	void Remove(CXTPFlowGraphElement* pElement);
	//------------------------------------------------------------------------
	// Summary:
	//     Removes the element at a specified index from the current selection.
	// Parameters:
	//     nIndex - Index of the element to remove.
	//------------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of elements in the current selection.
	// Returns:
	//     The number of elements in the current selection.
	//------------------------------------------------------------------------
	int GetCount() const;
	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the element at a specified index from the current selection.
	// Parameters:
	//     nIndex - Index of the element to retrieve.
	// Remarks:
	//     Elements are added using the SetSelection method and removed using
	//     the RemoveAt method.
	//------------------------------------------------------------------------
	CXTPFlowGraphElement* GetAt(int nIndex) const;

protected:
	CArray<CXTPFlowGraphElement*, CXTPFlowGraphElement*> m_arrSelectedElements; // Collection of
																				// selected
																				// elements.

	CXTPFlowGraphPage* m_pPage; // Pointer to the page that the selected elements are dispalyed on.
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphSelectedElements)
	DECLARE_ENUM_VARIANT(CXTPFlowGraphSelectedElements)

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg void OleAddSelection(LPDISPATCH lpDisp);
	afx_msg void OleRemove(LPDISPATCH lpDisp);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPFlowGraphSelectedElements::GetCount() const
{
	return (int)m_arrSelectedElements.GetSize();
}
AFX_INLINE CXTPFlowGraphElement* CXTPFlowGraphSelectedElements::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrSelectedElements.GetSize() ? m_arrSelectedElements[nIndex]
																   : NULL;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHSELECTEDELEMENTS_H__)
