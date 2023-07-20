// XTPGridRecordItemConstraint.h: interface for the CXTPGridRecordItemConstraint class.
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
#if !defined(__XTPGRIDRECORDITEMCONSTRAINT_H__)
#	define __XTPGRIDRECORDITEMCONSTRAINT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridRecordItemConstraint is a CXTPCmdTarget derived class.
//     It represents a single item constraint.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemConstraint : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPGridRecordItemConstraint);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemConstraint object.
	//-------------------------------------------------------------------------
	CXTPGridRecordItemConstraint();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieve the index of this constraint within the collection of constraints.
	// Returns:
	//     The index of this constraint within the collection of constraints.
	//-----------------------------------------------------------------------
	int GetIndex() const;

public:
	CString m_strConstraint; // Caption text of this constraint. This is the
							 // text displayed for this constraint.
	DWORD_PTR m_dwData;		 // The 32-bit value associated with the item.

protected:
	int m_nIndex; // Index of the constraint.

private:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItemConstraint);
//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPGridRecordItemConstraints;
	friend class CXTPGridRecordItemEditOptions;
};

//===========================================================================
// Summary:
//     CXTPGridRecordItemConstraints is a CXTPCmdTarget derived class.
//     It represents the item constraints collection.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridRecordItemConstraints : public CXTPCmdTarget
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridRecordItemConstraints object.
	//-------------------------------------------------------------------------
	CXTPGridRecordItemConstraints();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemConstraints object,
	//     handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPGridRecordItemConstraints();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the total number of constraints in the list.
	// Returns:
	//     The total number of constraints added to the
	//     GridRecordItem and/or GridColumn.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all constraints from the list of constraints.
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the constraint at a specified index from the collection
	//     of constraints.
	// Parameters:
	//     nIndex - Index of the constraint to retrieve.
	// Returns:
	//     A pointer to the constraint at the specified index in the collection
	//     of constraints.
	//-----------------------------------------------------------------------
	CXTPGridRecordItemConstraint* GetAt(int nIndex) const;

protected:
	CArray<CXTPGridRecordItemConstraint*, CXTPGridRecordItemConstraint*>
		m_arrConstraints; // Collection of constraints

private:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridRecordItemConstraints);

	int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg void OleAdd(LPCTSTR lpszCaption, long dwData);
	DECLARE_ENUM_VARIANT(CXTPGridRecordItemConstraints)

	enum
	{
		dispidCount = 1L,
		dispidAdd   = 2L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPGridRecordItemEditOptions;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDRECORDITEMCONSTRAINT_H__)
