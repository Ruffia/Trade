// XTPSyntaxEditSelection.h: interface for the CXTPSyntaxEditSelection class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITSELECTION_H__)
#	define __XTPSYNTAXEDITSELECTION_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_ACTIVEX
class CXTPSyntaxEditSelPosEx_imp;
class CXTPSyntaxEditSelPosExNormal_imp;
#	endif
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class is used to represent syntax edit text selection and allows
//     for selection properties to be get and set in two measurements:
//     string and display. Also the selection may be 'Normal' if its start is
//     less then its end or 'Reverse' if its end is less than its start.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditSelection : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditCtrl;
	friend class CXTPSyntaxEditSelection;

	DECLARE_DYNAMIC(CXTPSyntaxEditSelection)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Copy constructor.
	// Parameters:
	//     rSrc - [in] Reference to a source object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSelection(const CXTPSyntaxEditSelection& rSrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selection start position
	//     where the column is a zero-based char index in the string.
	// Returns:
	//     A reference to an XTP_EDIT_LINECOL object containing the selection start.
	// See Also:
	//     GetEnd_str, GetStart_disp, GetEnd_disp, GetNormalStart_str,
	//     GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//     SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//     Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetStart_str() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selection end position
	//     where the column is a zero-based char index in the string.
	// Returns:
	//     A reference to an XTP_EDIT_LINECOL object containing the selection end.
	// See Also:
	//     GetStart_str, GetStart_disp, GetEnd_disp, GetNormalStart_str,
	//     GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//     SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//     Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetEnd_str() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selection start position
	//     where the column is a one-based column index on the screen.
	// Returns:
	//     A reference to an XTP_EDIT_LINECOL object containing the selection start.
	// See Also:
	//     GetStart_str, GetEnd_str, GetEnd_disp, GetNormalStart_str,
	//     GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//     SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//     Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetStart_disp() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selection end position
	//     where the column is a one-based column index on the screen.
	// Returns:
	//     A reference to an XTP_EDIT_LINECOL object containing the selection end.
	// See Also:
	//     GetStart_str, GetEnd_str, GetStart_disp, GetNormalStart_str,
	//     GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//     SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//     Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetEnd_disp() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the normalized selection start.
	// Returns:
	//     A reference to an XTP_EDIT_LINECOL object which contains
	//     the normalized selection start.
	// See Also:
	//     GetStart_str, GetEnd_str, GetStart_disp, GetEnd_disp, GetNormalEnd_str,
	//     GetNormalEnd_disp, SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//     Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL GetNormalStart_str() const;
	XTP_EDIT_LINECOL
	GetNormalStart_disp() const; // <combine CXTPSyntaxEditSelection::GetNormalStart_str@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the normalized selection end.
	// Returns:
	//     A reference to an XTP_EDIT_LINECOL object which contains
	//     the normalized selection end.
	// See Also:
	//     GetStart_str, GetEnd_str, GetStart_disp, GetEnd_disp, GetNormalStart_str,
	//     GetNormalStart_disp, SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//     Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL GetNormalEnd_str() const;
	XTP_EDIT_LINECOL GetNormalEnd_disp() const; // <combine
												// CXTPSyntaxEditSelection::GetNormalEnd_str@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selection start position.
	// Parameters:
	//     nTextRow - Text row number.
	//     nStrPos  - Zero-based char index in the string.
	//     nDispCol - One-based column index on the screen.
	// See Also:
	//     SetEnd_str, SetEnd_disp, GetStart_str, GetEnd_str, GetStart_disp,
	//     GetEnd_disp, GetNormalStart_str, GetNormalStart_disp, GetNormalEnd_str,
	//     GetNormalEnd_disp, Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	void SetStart_str(int nTextRow, int nStrPos);
	void SetStart_disp(int nTextRow,
					   int nDispCol); // <combine CXTPSyntaxEditSelection::SetStart_str@int@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selection end position.
	// Parameters:
	//     nTextRow - Text row number.
	//     nStrPos  - Zero-based char index in the string.
	//     nDispCol - One-based column index on the screen.
	// See Also:
	//     SetStart_str, SetStart_disp, GetStart_str, GetEnd_str, GetStart_disp,
	//     GetNormalStart_str, GetNormalStart_disp, GetNormalEnd_str,
	//     GetNormalEnd_disp, Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	void SetEnd_str(int nTextRow, int nStrPos);
	void SetEnd_disp(int nTextRow,
					 int nDispCol); // <combine CXTPSyntaxEditSelection::SetEnd_str@int@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selection start and end positions.
	//     The selection is empty in this case.
	// Parameters:
	//     nTextRow - Text row number.
	//     nStrPos  - Zero-based char index in the string.
	//     nDispCol - One-based column index on the screen.
	// See Also:
	//     SetStart_str, SetStart_disp, GetStart_str, GetEnd_str, GetStart_disp,
	//     GetEnd_disp, GetNormalStart_str, GetNormalStart_disp, GetNormalEnd_str,
	//     GetNormalEnd_disp
	//-----------------------------------------------------------------------
	void Reset_str(int nTextRow, int nStrPos);
	void Reset_disp(int nTextRow,
					int nDispCol); // <combine CXTPSyntaxEditSelection::Reset_str@int@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection mode is 'Word'
	//     (enabled by pressing and holding Ctrl before selecting).
	// Returns:
	//     TRUE if the selection mode is 'Word', otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsWordSelectionMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection mode is 'Block'
	//     (enabled by pressing and holding Alt before selecting).
	// Returns:
	//     TRUE if the selection mode is 'Block', otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsbBlockSelectionMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection exists (i.e. start != end).
	// Returns:
	//     TRUE if the selection exists, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelExist() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection is 'Normal' (i.e. start \< end).
	// Returns:
	//     TRUE if the selection is 'Normal', otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelNormal() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified position is inside the selection.
	// Parameters:
	//     nTextRow - Text row number.
	//     nStrPos  - Zero-based char index in the string.
	// Returns:
	//     TRUE if the specified position is inside the selection, otherwise FALSE.
	// See Also:
	//     IsIntersectSel_str, IsIntersectSel_disp
	//-----------------------------------------------------------------------
	BOOL IsInSel_str(int nTextRow, int nStrPos) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified position is inside the selection.
	// Parameters:
	//     nTextRow - Text row number.
	//     nDispCol - One-based column index on the screen.
	// Returns:
	//     TRUE if the specified position is inside the selection, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsInSel_disp(int nTextRow, int nDispCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified text range intersects the selection.
	// Parameters:
	//     nTextRow  - Text row number.
	//     nStrPos1  - Text range start position (zero-based char index in the string).
	//     nStrPos2  - Text range end position (zero-based char index in the string).
	// Returns:
	//     TRUE if the specified text range intersects the selection, otherwise FALSE.
	// See Also:
	//     IsIntersectSel_str, IsIntersectSel_disp
	//-----------------------------------------------------------------------
	BOOL IsIntersectSel_str(int nTextRow, int nStrPos1, int nStrPos2) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified text range intersects the selection.
	// Parameters:
	//     nTextRow  - Text row number.
	//     nDispCol1 - Text range start position (one-based column index on the screen).
	//     nDispCol2 - Text range end position (one-based column index on the screen).
	// Returns:
	//     TRUE if the specified text range intersects the selection, otherwise FALSE.
	// See Also:
	//     IsIntersectSel_str, IsIntersectSel_disp
	//-----------------------------------------------------------------------
	BOOL IsIntersectSel_disp(int nTextRow, int nDispCol1, int nDispCol2) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Equality operator.
	//     Compares the current object to a specified object.
	// Parameters:
	//     rSrc - Reference to a CXTPSyntaxEditSelection object to compare to.
	// Returns:
	//     TRUE if the current object and the specified object
	//     are equal, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL operator==(const CXTPSyntaxEditSelection& rSrc) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Non-Equality operator.
	//     Compares the current object to a specified object.
	// Parameters:
	//     rSrc - Reference to a CXTPSyntaxEditSelection object to compare to.
	// Returns:
	//     TRUE if the current object and the specified object
	//     are not equal, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL operator!=(const CXTPSyntaxEditSelection& rSrc) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Copy operator.
	//     Initializes members from a specified object.
	// Parameters:
	//     rSrc - Reference to a CXTPSyntaxEditSelection object to copy from.
	// Returns:
	//     A reference to the current object.
	//-----------------------------------------------------------------------
	const CXTPSyntaxEditSelection& operator=(const CXTPSyntaxEditSelection& rSrc);

	//{{AFX_CODEJOCK_PRIVATE
	int GetSelStartForRow_str(int nTextRow, int nDispLine);
	int GetSelEndForRow_str(int nTextRow, int nDispLine, BOOL* pbInfinitSelEnd);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL _IsInSel(BOOL bStr, int nTextRow, int nColX) const;
	BOOL _IsIntersectSel(BOOL bStr, int nTextRow, int nCol1, int nCol2) const;
	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL bSelectingRunning;			 // Stores the 'SelectingRunning' mode.
	BOOL bWordSelectionMode;		 // TRUE if the selection mode is 'Word', otherwise FALSE.
	BOOL bBlockSelectionMode;		 // TRUE if the selection mode is 'Block', otherwise FALSE.
	int nSelStartTextRowFromLeftBar; // Stores the start selection line for full line selecting
									 // (from the left bar).

private:
	CXTPSyntaxEditCtrl* m_pOwnerCtrl;

	XTP_EDIT_LINECOL selStart_disp;
	XTP_EDIT_LINECOL selEnd_disp;

	XTP_EDIT_LINECOL selStart_str;
	XTP_EDIT_LINECOL selEnd_str;

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditSelection)

	LPDISPATCH OleGetStart();
	LPDISPATCH OleGetEnd();
	LPDISPATCH OleGetNormalStart();
	LPDISPATCH OleGetNormalEnd();

	BSTR OleGetText();
	void OleSetText(LPCTSTR pcszSelText);
	void OleReset();

	CXTPSyntaxEditSelPosEx_imp* m_pAxStart;
	CXTPSyntaxEditSelPosEx_imp* m_pAxEnd;

	CXTPSyntaxEditSelPosExNormal_imp* m_pAxNormalStart;
	CXTPSyntaxEditSelPosExNormal_imp* m_pAxNormalEnd;
//}}AFX_CODEJOCK_PRIVATE
#	endif
};

#	ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPSyntaxEditTextPosition_imp : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPSyntaxEditTextPosition_imp)
public:
	CXTPSyntaxEditTextPosition_imp();

	virtual long OleGetRow()	= 0;
	virtual long OleGetCol()	= 0;
	virtual long OleGetStrPos() = 0;

	virtual void Ole_SetPos(long nRow, long nCol)		= 0;
	virtual void Ole_SetPosStr(long nRow, long nStrPos) = 0;

	virtual void OleSetRow(long nRow)
	{
		Ole_SetPos(nRow, OleGetCol());
	};

	virtual void OleSetCol(long nCol)
	{
		Ole_SetPos(OleGetRow(), nCol);
	};

	virtual void OleSetStrPos(long nStrPos)
	{
		Ole_SetPosStr(OleGetRow(), nStrPos);
	};

protected:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditTextPosition_imp)
};

//////////////////////////////////////////////////////////////////////////
class _XTP_EXT_CLASS CXTPSyntaxEditSelPosEx_imp : public CXTPSyntaxEditTextPosition_imp
{
public:
	CXTPSyntaxEditSelPosEx_imp(CXTPSyntaxEditSelection* pOwner, BOOL bUseStart);

	virtual long OleGetRow();
	virtual long OleGetCol();
	virtual long OleGetStrPos();

	virtual void Ole_SetPos(long nRow, long nCol);
	virtual void Ole_SetPosStr(long nRow, long nStrPos);

	void Detach()
	{
		m_pOwner = NULL;
	}

protected:
	CXTPSyntaxEditSelection* m_pOwner;
	BOOL m_bUseStart;
};

//////////////////////////////////////////////////////////////////////////
class _XTP_EXT_CLASS CXTPSyntaxEditSelPosExNormal_imp : public CXTPSyntaxEditSelPosEx_imp
{
public:
	CXTPSyntaxEditSelPosExNormal_imp(CXTPSyntaxEditSelection* pOwner, BOOL bUseStart);

	virtual long OleGetRow();
	virtual long OleGetCol();
	virtual long OleGetStrPos();

	virtual void Ole_SetPos(long nRow, long nCol);
	virtual void Ole_SetPosStr(long nRow, long nStrPos);
};
//}}AFX_CODEJOCK_PRIVATE
#	endif

//////////////////////////////////////////////////////////////////////////
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetStart_str() const
{
	return selStart_str;
}
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetEnd_str() const
{
	return selEnd_str;
}
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetStart_disp() const
{
	return selStart_disp;
}
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetEnd_disp() const
{
	return selEnd_disp;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsWordSelectionMode() const
{
	return bWordSelectionMode;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsbBlockSelectionMode() const
{
	return bBlockSelectionMode;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalStart_str() const
{
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::min2(selStart_str, selEnd_str);
	return selStart_str <= selEnd_str ? selStart_str : selEnd_str;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalEnd_str() const
{
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::max2(selStart_str, selEnd_str);
	return selEnd_str <= selStart_str ? selStart_str : selEnd_str;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalStart_disp() const
{
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::min2(selStart_disp, selEnd_disp);
	return selStart_disp <= selEnd_disp ? selStart_disp : selEnd_disp;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalEnd_disp() const
{
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::max2(selStart_disp, selEnd_disp);
	return selEnd_disp <= selStart_disp ? selStart_disp : selEnd_disp;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsSelNormal() const
{
	return selStart_disp <= selEnd_disp;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsInSel_str(int nTextRow, int nStrPos) const
{
	ASSERT(nStrPos >= 0);
	return _IsInSel(TRUE, nTextRow, nStrPos);
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsInSel_disp(int nTextRow, int nDispCol) const
{
	return _IsInSel(FALSE, nTextRow, nDispCol);
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsIntersectSel_str(int nTextRow, int nStrPos1,
															int nStrPos2) const
{
	return _IsIntersectSel(TRUE, nTextRow, nStrPos1, nStrPos2);
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsIntersectSel_disp(int nTextRow, int nDispCol1,
															 int nDispCol2) const
{
	return _IsIntersectSel(FALSE, nTextRow, nDispCol1, nDispCol2);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITSELECTION_H__)
