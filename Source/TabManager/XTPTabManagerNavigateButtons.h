// XTPTabManagerNavigateButtons.h
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
#if !defined(__XTPTABMANAGERNAVIGATEBUTTONS_H__)
#	define __XTPTABMANAGERNAVIGATEBUTTONS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabManagerNavigateButtons is a class used to represent
//     a collection of tab navigation buttons.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManagerNavigateButtons
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerNavigateButtons object.
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerNavigateButtons object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabManagerNavigateButtons();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tab navigation button at a specified index in the collection.
	// Parameters:
	//     nIndex - Zero-based index of the tab navigation button to retrieve.
	// Returns:
	//     A pointer to the tab naviation navigation button at the specified index
	//     in the collection if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tab navigation button at a specified index in the collection.
	// Parameters:
	//     nIndex - Zero-based index of the tab navigation button to retrieve.
	// Returns:
	//     A pointer to the tab naviation navigation button at the specified index
	//     in the collection if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* operator[](int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified tab navigation button to the collection.
	// Parameters:
	//     pButton - Pointer to the tab navigation button to be added.
	// Returns:
	//     The zero-based index of the newly added tab navigation button.
	//-----------------------------------------------------------------------
	int Add(CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a specified tab navigation button to the collection.
	// Parameters:
	//     nIndex  - Zero-based index to insert the tab navigation button at.
	//     pButton - Pointer to the tab navigation button to be inserted.
	//-----------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the tab navigation button at a specified index from the collection.
	// Parameters:
	//     nIndex - Index of the tab navigation button to be removed.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all tab navigation buttons from the collection.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of tab navigation buttons in the collection.
	// Returns:
	//     The number of tab navigation buttons in the collection.
	//-----------------------------------------------------------------------
	int GetSize() const;

protected:
	CArray<CXTPTabManagerNavigateButton*, CXTPTabManagerNavigateButton*> m_arrButtons; // Collection
																					   // of tab
																					   // navigation
																					   // buttons.
};

AFX_INLINE CXTPTabManagerNavigateButton* CXTPTabManagerNavigateButtons::GetAt(int nIndex) const
{
	return m_arrButtons.GetAt(nIndex);
}

AFX_INLINE int CXTPTabManagerNavigateButtons::Add(CXTPTabManagerNavigateButton* pButton)
{
	return (int)m_arrButtons.Add(pButton);
}

AFX_INLINE int CXTPTabManagerNavigateButtons::GetSize() const
{
	return (int)m_arrButtons.GetSize();
}

AFX_INLINE void CXTPTabManagerNavigateButtons::InsertAt(int nIndex,
														CXTPTabManagerNavigateButton* pButton)
{
	m_arrButtons.InsertAt(nIndex, pButton);
}

AFX_INLINE CXTPTabManagerNavigateButton* CXTPTabManagerNavigateButtons::operator[](int nIndex) const
{
	return m_arrButtons.GetAt(nIndex);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABMANAGERNAVIGATEBUTTONS_H__)
