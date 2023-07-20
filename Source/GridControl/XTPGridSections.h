// XTPGridSections.h: interface for the CXTPGridSections class.
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
#if !defined(__XTPGRIDSECTIONS_H__)
#	define __XTPGRIDSECTIONS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridControl;
class CXTPGridSection;
class CXTPMarkupContext;

enum XTPGridSection
{
	xtpGridSectionHeader = 0, // Index of header section.
	xtpGridSectionBody   = 1, // Index of body section.
	xtpGridSectionFooter = 2, // Index of footer section.
	xtpGridSectionCount  = 3  // Number of default sections.
};

//-----------------------------------------------------------------------
// Summary:
//     Collection of sections.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPGridSections : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the collection and default sections.
	//-----------------------------------------------------------------------
	CXTPGridSections(CXTPGridControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructs the collection and sections.
	//-----------------------------------------------------------------------
	~CXTPGridSections();

public:
	int Add(CXTPGridSection* pSection)
	{
		return (int)m_arrSections.Add(pSection);
	}

	CXTPGridSection* GetAt(int nIndex) const
	{
		CXTPGridSection* pSection = NULL;

		if (GetSize() > nIndex) // Valid index
		{
			pSection = m_arrSections.GetAt(nIndex);
		}

		return pSection;
	}

	int GetSize() const
	{
		return int(m_arrSections.GetSize());
	}

	void RemoveAll()
	{
		m_arrSections.RemoveAll();
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a Grid control row is at least partially visible.
	// Parameters:
	//     pRow - Pointer to the CXTPGridRow object whose
	//            visibility must be checked.
	// Remarks:
	//     Ensures that a grid row item is at least partially visible.
	//     The list view control is scrolled if necessary.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual BOOL EnsureVisible(CDC* pDC, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the currently focused row in the Grid control's view.
	// Returns:
	//     A pointer to the currently focused row in the
	//     Grid control's view if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridRow* GetFocusedRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the current rectangle for the sections.
	// Returns:
	//     A CRect containing the current rectangle for the sections.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which section, if any, is at a specified point.
	// Parameters:
	//     pt - Point to test.
	// Returns:
	//     A pointer to the section at the specified point, if any,
	//     otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPGridSection* HitTest(CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the sections to the device context.
	// Parameters:
	//     pDC - Pointer to the used device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts section areas depending on the current control size.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, CRect rcSections);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the markup context.
	// Parameters:
	//     pMarkupContext - Pointer to the markup context to be set.
	//-----------------------------------------------------------------------
	void SetMarkupContext(CXTPMarkupContext* pMarkupContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Re-sorts the rows for all sections.
	//-----------------------------------------------------------------------
	virtual void ReSortRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates the indices of all rows.
	// Parameters:
	//     bAdjustLayout - TRUE to adjust the layout.
	//     bReverseOrder - TRUE to update row indices in reverse order
	//                     starting from the last now.
	//-----------------------------------------------------------------------
	virtual void RefreshIndexes(BOOL bAdjustLayout);

	virtual void ResetContent();

protected:
	CXTPGridSection* FindSection(CXTPGridRow* pRow) const;

protected:
	CArray<CXTPGridSection*, CXTPGridSection*> m_arrSections;

	CRect m_rcSections;			 // Sections rectangle.
	CXTPGridControl* m_pControl; // Parent control.

private:
#	ifdef _XTP_ACTIVEX
	DECLARE_OLETYPELIB_EX(CXTPGridSections);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	int OleGetCount() const
	{
		return GetSize();
	}

	LPDISPATCH OleGetSection(int nIndex) const;
#	endif // _XTP_ACTIVEX
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDSECTIONS_H__)
