// XTPGridNavigator.h: interface for the CXTPGridNavigator class.
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
#if !defined(__XTPGRIDNAVIGATOR_H__)
#	define __XTPGRIDNAVIGATOR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridControl;
class CXTPGridRow;
class CXTPGridHeader;
class CXTPGridColumn;

//===========================================================================
// Summary:
//     Utility class, handles Grid control item navigation.
// See Also:
//     CXTPGridControl overview
//===========================================================================
class _XTP_EXT_CLASS CXTPGridNavigator : public CXTPCmdTarget
{
	friend class CXTPGridControl;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator constructor, handles properties initialization.
	// Parameters:
	//     pGridControl - Pointer to a CXTPGridControl object
	// See Also:
	//     RefreshMetrics
	//-----------------------------------------------------------------------
	CXTPGridNavigator(CXTPGridControl* pGridControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator destructor, handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridNavigator();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to begin editing the currently focused item.
	//-----------------------------------------------------------------------
	virtual void BeginEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down by one.
	// Parameters:
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row down by one.
	// See Also:
	//     MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveDown(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down by one.
	// Parameters:
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row down by one.
	// See Also:
	//     MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveDownStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up by one.
	// Parameters:
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row up by one.
	// See Also:
	//     MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveUp(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up by one.
	// Parameters:
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row up by one.
	// See Also:
	//     MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveUpStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down by one page.
	// Parameters:
	//     bShiftKey   - TRUE to select rows up to the new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row down by one page
	//     (and count the visible rows in a view).
	// See Also:
	//     MoveDown, MoveUp, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MovePageDown(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up by one page.
	// Parameters:
	//     bShiftKey   - TRUE to select rows up to the new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row up by one page
	//     (and count the visible rows in a view).
	// See Also:
	//     MoveDown, MoveUp, MovePageDown, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MovePageUp(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the beginning of the grid view.
	// Parameters:
	//     bShiftKey   - TRUE to select rows up to the new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row to the top of the rows list.
	// See Also:
	//     MoveDown, MoveUp, MovePageDown, MovePageUp, MoveLast
	//-----------------------------------------------------------------------
	void MoveFirstRow(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the end of the grid view.
	// Parameters:
	//     bShiftKey   - TRUE to select rows up to the new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row to the bottom of the rows list.
	// See Also:
	//     MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst
	//-----------------------------------------------------------------------
	void MoveLastRow(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to a specified row index.
	// Parameters:
	//     nRowIndex   - Row index to move the focused row to.
	//     bShiftKey   - TRUE to select rows up to the new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused row to the specified row index.
	// See Also:
	//     MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst
	//-----------------------------------------------------------------------
	void MoveToRow(int nRowIndex, BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column to the beginning of the currently selected row.
	// See Also:
	//     MoveLastColumn
	//-----------------------------------------------------------------------
	void MoveFirstColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column to the end of the currently selected row.
	// See Also:
	//     MoveFirstColumn
	//-----------------------------------------------------------------------
	void MoveLastColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column left by one.
	// Parameters:
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused column left by one.
	// See Also:
	//     MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveLeft(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column right by one.
	// Parameters:
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Moves the currently focused column right by one.
	// See Also:
	//     MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveRight(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column to a specified column index.
	// Parameters:
	//     nColumnIndex         - Column index to move the focused column to.
	//     bClearIfNonFocusable - Used in the event that the column specified by
	//                            nColumnIndex is not focusable. TRUE to clear
	//                            the column focus (resulting in no column having
	//                            focus), FALSE to prevent the focused column
	//                            from being changed.
	// Remarks:
	//     Moves the currently focused column to the specified column index.
	// See Also:
	//     MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveToColumn(int nColumnIndex, BOOL bClearIfNonFocusable = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current focus (selected row) to header rows.
	// Parameters:
	//     bCurrentFocusInHeadersRows - TRUE to set the current focus to header rows,
	//                                  FALSE otherwise.
	// See Also:
	//     GetCurrentFocusInHeadersRows
	//-----------------------------------------------------------------------
	void SetCurrentFocusInHeadersRows(BOOL bCurrentFocusInHeadersRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current focus (selected row) to footer rows.
	// Parameters:
	//     bCurrentFocusInFootersRows - TRUE to set the current focus to footer rows,
	//                                  FALSE otherwise.
	// See Also:
	//     GetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	void SetCurrentFocusInFootersRows(BOOL bCurrentFocusInFootersRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current focus (selected row) belongs to header rows.
	// Returns:
	//     TRUE if the current focus (selected row) belongs to header rows,
	//     otherwise FALSE.
	// See Also:
	//     SetCurrentFocusInHeadersRows
	//-----------------------------------------------------------------------
	BOOL GetCurrentFocusInHeadersRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current focus (selected row) belongs to footer rows.
	// Returns:
	//     TRUE if the current focus (selected row) belongs to footer rows,
	//     otherwise FALSE.
	// See Also:
	//     SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	BOOL GetCurrentFocusInFootersRows();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column left/right.
	// Parameters:
	//     bBack       - TRUE to move left, FALSE to move right.
	//     bShiftKey   - TRUE to select a new focused row.
	//     bControlKey - TRUE to ignore the current rows selection.
	// See Also:
	//     MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveLeftRight(BOOL bBack, BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the first visible row of a target section:
	//     body, header, or footer.
	// Parameters:
	//     RowType - Target section for the focused row; must be one of the values
	//               defined by the XTPGridRowType enumeration.
	// Remarks:
	//     Moves the focused row to the first visible target row.
	// See Also:
	//     SetCurrentFocusInHeadersRows, SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	void MoveFirstVisibleRow(XTPGridRowType RowType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the last visible row of a target section:
	//     body, header, or footer.
	// Parameters:
	//     RowType - Target section for the focused row; must be one of the values
	//               defined by the XTPGridRowType enumeration.
	// Remarks:
	//     Moves the focused row to the last visible target row.
	// See Also:
	//     SetCurrentFocusInHeadersRows, SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	void MoveLastVisibleRow(XTPGridRowType RowType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks for a dead end row by moving down/up.
	// Parameters:
	//     bMoveDown - TRUE to move down, FALSE to move up.
	// Returns:
	//     The index of the dead end row.
	// See Also:
	//     SetCurrentFocusInHeadersRows, SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	int CheckDeadEnd(BOOL bMoveDown);

protected:
	CXTPGridControl* m_pGridControl; // Associated Grid control.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridNavigator);

#		define DECLARE_OLE_MOVE(Direction)                                                        \
			void OleMove##Direction(const VARIANT& oleSelectBlock,                                 \
									const VARIANT& oleIgnoreSelection);

	DECLARE_OLE_MOVE(Up)
	DECLARE_OLE_MOVE(Down)
	DECLARE_OLE_MOVE(PageUp)
	DECLARE_OLE_MOVE(PageDown)
	DECLARE_OLE_MOVE(FirstRow)
	DECLARE_OLE_MOVE(LastRow)
	DECLARE_OLE_MOVE(Left)
	DECLARE_OLE_MOVE(Right)

	void OleMoveToColumn(int nColumnIndex, const VARIANT& oleClearIfNonFocusable);
	void OleMoveToRow(int nRowIndex, const VARIANT& oleSelectBlock,
					  const VARIANT& oleIgnoreSelection);

	void OleMoveFirstVisibleRow();
	void OleMoveLastVisibleRow();

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     Utility class, handles Grid control icon navigation.
// See Also:
//     CXTPGridControl overview
//===========================================================================
class _XTP_EXT_CLASS CXTPGridIconNavigator : public CXTPGridNavigator
{
	friend class CXTPGridControl;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator constructor, handles properties initialization.
	// Parameters:
	//     pGridControl - Pointer to a CXTPGridControl object.
	// See Also:
	//     RefreshMetrics
	//-----------------------------------------------------------------------
	CXTPGridIconNavigator(CXTPGridControl* pGridControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator destructor, handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridIconNavigator();

public:
	virtual void BeginEdit();

	virtual void MoveDown(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);
	virtual void MoveUp(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	virtual void MoveDownStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);
	virtual void MoveUpStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	virtual void MovePageDown(BOOL bShiftKey, BOOL bControlKey);
	virtual void MovePageUp(BOOL bShiftKey, BOOL bControlKey);

	virtual void MoveLeftRight(BOOL bBack, BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDNAVIGATOR_H__)
