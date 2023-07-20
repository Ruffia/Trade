// XTPFlowGraphPages.h: interface for the CXTPFlowGraphPages class.
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
#if !defined(__XTPFLOWGRAPHPAGES_H__)
#	define __XTPFLOWGRAPHPAGES_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPFlowGraphControl;
class CXTPFlowGraphPage;

//============================================================================
// Summary:
//     Represents a collection of pages in the flow graph.
// Remarks:
//     A page is what contains all the elements in the flow graph. The
//     flow graph can contain many pages, each of which can contain
//     Nodes, Node groups, and connections. A page is the first thing that
//     you must add to the flow graph and then you can add elements
//     to the pages.
//============================================================================
class _XTP_EXT_CLASS CXTPFlowGraphPages : public CXTPCmdTarget
{
protected:
	//------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphPages object.
	//------------------------------------------------------------------------
	CXTPFlowGraphPages(CXTPFlowGraphControl* pControl);
	//------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphPages object, handles cleanup and
	//     deallocation.
	//------------------------------------------------------------------------
	virtual ~CXTPFlowGraphPages();

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Adds a page to the flow graph.
	// Returns:
	//     A reference to the newly added FlowGraphPage.
	//------------------------------------------------------------------------
	CXTPFlowGraphPage* AddPage(CXTPFlowGraphPage* pPage);

	//------------------------------------------------------------------------
	// Summary:
	//     Removes all pages from the flow graph.
	//------------------------------------------------------------------------
	void RemoveAll();

	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the total number of pages in the flow graph.
	// Returns:
	//     The total number of pages in the flow graph.
	//------------------------------------------------------------------------
	int GetCount() const;
	//------------------------------------------------------------------------
	// Summary:
	//     Gets a page at the specified index from the FlowGraph's collection
	//     of pages.
	// Parameters:
	//     nIndex - Index of the specified page.
	// Returns:
	//     A reference to the specified FlowGraphPage.
	//------------------------------------------------------------------------
	CXTPFlowGraphPage* GetAt(int nIndex) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Removes a page from the FlowGraph.
	// Parameters:
	//     pPage - Reference to the page to remove.
	//------------------------------------------------------------------------
	void Remove(CXTPFlowGraphPage* pPage);
	//------------------------------------------------------------------------
	// Summary:
	//     Removes the specified page from the FlowGraph.
	// Parameters:
	//     nIndex - Index of the page to remove.
	//------------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//------------------------------------------------------------------------
	// Summary:
	//     Finds and retrieves the specified page.
	// Parameters:
	//     nId - ID of the page to find and retrieve.
	// Returns:
	//     A reference to the page with the specified ID if found, otherwise NULL.
	//------------------------------------------------------------------------
	CXTPFlowGraphPage* FindPage(int nId) const;

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a collection of pages
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load a collection of pages
	//     data to/from storage.
	//------------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CArray<CXTPFlowGraphPage*, CXTPFlowGraphPage*> m_arrPages; // Collection of pages in the flow
															   // graph control.
	CXTPFlowGraphControl* m_pControl; // Pointer to the parent flow graph control.

	friend class CXTPFlowGraphControl;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphPages)
	DECLARE_ENUM_VARIANT(CXTPFlowGraphPages)

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg LPDISPATCH OleAddPage();
	afx_msg LPDISPATCH OleFindPage(int nId);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE int CXTPFlowGraphPages::GetCount() const
{
	return (int)m_arrPages.GetSize();
}
AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphPages::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrPages.GetSize() ? m_arrPages[nIndex] : NULL;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPFLOWGRAPHPAGES_H__)
