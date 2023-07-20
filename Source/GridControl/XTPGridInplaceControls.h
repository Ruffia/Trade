// XTPGridInplaceControls.h
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
#if !defined(__XTPGRIDINPLACECONTROLS_H__)
#	define __XTPGRIDINPLACECONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRecord;
class CXTPGridControl;
class CXTPGridRecordItem;
class CXTPGridRecordItemConstraints;
class CXTPGridRecordItemConstraint;

//===========================================================================
// Summary:
//     CXTPGridInplaceControl is the base class for all in-place controls of
//     the Grid control item.
// Remarks:
//     There are three built-in, in-place controls for the Grid control item:
//     * CXTPGridInplaceEdit: Represents the edit control to allow users
//                              to change a cell of the Grid control.
//     * CXTPGridInplaceButton: Represents a single button of the item.
//     * CXTPGridInplaceList: Represents the list box to allow users
//                              to select the constraints of the grid item.
// See Also:
//     CXTPGridInplaceEdit, CXTPGridInplaceButton, CXTPGridInplaceList
//===========================================================================
class _XTP_EXT_CLASS CXTPGridInplaceControl : public XTP_GRIDRECORDITEM_ARGS
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridInplaceControl object.
	//-----------------------------------------------------------------------
	CXTPGridInplaceControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridInplaceControl object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridInplaceControl();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internally to fill the item arguments
	//     of an in-place control.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//-------------------------------------------------------------------------
	virtual void SetItemArgs(XTP_GRIDRECORDITEM_ARGS* pItemArgs);
};

//===========================================================================
// Summary:
//     CXTPGridInplaceEdit is a CXTPGridInplaceControl derived class that
//     represents an edit control to allow users to change a cell of a Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridInplaceEdit
	: public CXTPMaskEditT<CEdit>
	, public CXTPGridInplaceControl
{
	DECLARE_DYNAMIC(CXTPGridInplaceEdit)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridInplaceEdit object.
	//-----------------------------------------------------------------------
	CXTPGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridInplaceEdit object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridInplaceEdit();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Hides the in-place edit control.
	//-------------------------------------------------------------------------
	void HideWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a child Grid control item.
	// Returns:
	//     A pointer to a CXTPGridRecordItem object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an in-place edit control.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item cell parameters.
	//-----------------------------------------------------------------------
	void Create(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets edit text.
	// Parameters:
	//     lpszString - Edit text to be set.
	//-----------------------------------------------------------------------
	void SetWindowText(LPCTSTR lpszString);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font of the in-place edit control.
	// Parameters:
	//     pFont - New font to be set.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPGridInplaceControl)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridInplaceControl)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnEnKillfocus();
	afx_msg void OnSetFocus(CWnd* pWnd);
	afx_msg void OnEnChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	CString m_strValue;		// The value of item before user change it.
	CString m_strText_prev; // The text value from previous change.

	CXTPFont m_xtpFontEdit; // Font of in-place edit.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntEdit, m_xtpFontEdit, GetEditFontHandle);

	CXTPGridRecordItemConstraint* m_pSelectedConstraint; // Selected constraint.
	COLORREF m_clrText;									 // Text color of in-place edit.
	COLORREF m_clrBack;									 // Background color of in-place edit.
	CXTPBrush m_xtpBrushBack;							 // Background brush of in-place edit.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brBack, m_xtpBrushBack, GetBackBrushHandle);
	BOOL m_bSetWindowText; // Indicates that text is being set with the SetWindowText function.
						   // private:

	friend class CXTPGridRecordItem;
};

AFX_INLINE CXTPGridRecordItem* CXTPGridInplaceEdit::GetItem() const
{
	return pItem;
}

AFX_INLINE BOOL CXTPGridInplaceEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

AFX_INLINE BOOL CXTPGridInplaceEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											UINT nID)
{
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

const UINT XTP_ID_GRID_EXPANDBUTTON = 100; //<ALIAS
										   // CXTPGridInplaceButton::CXTPGridInplaceButton@UINT>
const UINT XTP_ID_GRID_COMBOBUTTON = 101;  //<ALIAS
										   // CXTPGridInplaceButton::CXTPGridInplaceButton@UINT>
const UINT XTP_ID_GRID_SPINBUTTON = 102;   //<ALIAS
										   // CXTPGridInplaceButton::CXTPGridInplaceButton@UINT>

//===========================================================================
// Summary:
//     CXTPGridInplaceButton is a CXTPGridInplaceControl derived class
//     that represents a combo button and expand button of a grid cell.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridInplaceButton
	: public CStatic
	, public CXTPGridInplaceControl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridInplaceButton object.
	// Parameters:
	//     nID - Identifier of the in-place button. Can be any of the
	//           values listed in the Remarks section.
	// Remarks:
	//     Default identifiers are:
	//     * <b>XTP_ID_GRID_EXPANDBUTTON</b>: Indicates that the button
	//                                          acts like an expand button.
	//     * <b>XTP_ID_GRID_COMBOBUTTON</b>:  Indicates that the button
	//                                          acts like a combo button.
	//     * <b>XTP_ID_GRID_SPINBUTTON</b>:   Indicates that the button
	//                                          acts like a spin button.
	//-----------------------------------------------------------------------
	CXTPGridInplaceButton(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an in-place button control.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item cell parameters.
	//     rcButtons - Reference to the bounding rectangle of the button.
	//-----------------------------------------------------------------------
	void Create(XTP_GRIDRECORDITEM_ARGS* pItemArgs, CRect& rcButtons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the button.
	// Returns:
	//     The identifier of the button.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the user pressed the button.
	// Returns:
	//     TRUE if the user pressed the button.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the state of the button.
	// Returns:
	//     The state of the button.
	//-----------------------------------------------------------------------
	int GetState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a child Grid control item.
	// Returns:
	//     A pointer to a CXTPGridRecordItem object.
	//-----------------------------------------------------------------------
	CXTPGridRecordItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width of the button.
	// Returns:
	//     The width of the button.
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is placed inside or
	//     outside (near the right cell side) the cell rect.
	// Returns:
	//     TRUE if the button is placed inside the cell rect, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsInsideCellButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the button should be placed inside or
	//     outside (near the right cell side) the cell rect.
	// Parameters:
	//     bInsideCell - TRUE to place the button inside the cell rect,
	//                   FALSE to place the button outside the cell rect.
	//-----------------------------------------------------------------------
	void SetInsideCellButton(BOOL bInsideCell);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon index for the button.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// See Also:
	//     GetIconIndex
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the icon index of the button.
	// Returns:
	//     The icon index of the button.
	// See Also:
	//     SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets spin button properties.
	// Returns:
	//     The requested spin button property value.
	// See Also:
	//     SetSpinButtonMin, SetSpinButtonMax, SetSpinButtonStep
	//-----------------------------------------------------------------------
	int GetSpinButtonMin() const;
	int GetSpinButtonMax() const;  //<COMBINE CXTPGridInplaceButton::GetSpinButtonMin@const>
	int GetSpinButtonStep() const; //<COMBINE CXTPGridInplaceButton::GetSpinButtonMin@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets spin button properties.
	// Parameters:
	//     nValue - Spin button property value to be set.
	// See Also:
	//     GetSpinButtonMin, GetSpinButtonMax, GetSpinButtonStep
	//-----------------------------------------------------------------------
	void SetSpinButtonMin(int nValue);
	void SetSpinButtonMax(int nValue);  //<COMBINE CXTPGridInplaceButton::SetSpinButtonMin@int>
	void SetSpinButtonStep(int nValue); //<COMBINE CXTPGridInplaceButton::SetSpinButtonMin@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to activate the button.
	//-----------------------------------------------------------------------
	void Activate();

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets the number of items to show in the dropdown list.
	// Parameters:
	//      nNumber - A number of items to show.
	// Returns:
	//      A number of items to show.
	//-----------------------------------------------------------------------
	int GetDropListItemNumber() const;
	void SetDropListItemNumber(int nNumber); // <combine
											 // CXTPGridInplaceButton::GetDropListItemNumber@const>

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPGridInplaceButton)
	virtual void OnFinalRelease();
	//{{AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridInplaceButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	DECLARE_OLETYPELIB_EX(CXTPGridInplaceButton);
	LPDISPATCH OleGetRow();
	LPDISPATCH OleGetColumn();
	LPDISPATCH OleGetItem();

	void OleGetRect(long* pnLeft, long* pnTop, long* pnRight, long* pnBottom);

//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID = 0xffff);

protected:
	int m_nID;			// Identifier of the button.
	int m_nWidth;		// Width of the button.
	int m_nFixedHeight; // Height of the button.
	BOOL m_bInsideCell; // Store button place: inside the cell rect, or outside.
	int m_nIconIndex;   // Icon index.

	BOOL m_bPressed; // TRUE if the button is pressed.
	BOOL m_bOver;	// TRUE if the mouse cursor is over the button.
	int m_nState;	// Button state.

	int m_nSpinMin;  // Spin button only: minimum value.
	int m_nSpinMax;  // Spin button only: maximum value.
	int m_nSpinStep; // Spin button only: step value.

	UINT_PTR m_unSpinTimerCnt; // Spin button only: timer counter.
	UINT_PTR m_unSpinTimerId;  // Spin button only: timer identifier.
	int m_nSpinIncrement;	  // Spin button only: increment value.

private:
	int m_Items2Show; // Picklist desired item's number to show. Use
					  // GetDropListItemNumber/SetDropListItemNumber in order to access its value.

	friend class CXTPGridRecordItem;
};

AFX_INLINE int CXTPGridInplaceButton::GetDropListItemNumber() const
{
	return m_Items2Show;
}

AFX_INLINE void CXTPGridInplaceButton::SetDropListItemNumber(int nNumber)
{
	ASSERT(0 <= nNumber);
	m_Items2Show = nNumber;
}

AFX_INLINE int CXTPGridInplaceButton::GetID() const
{
	return m_nID;
}
AFX_INLINE BOOL CXTPGridInplaceButton::IsPressed() const
{
	return m_bPressed && m_bOver;
}
AFX_INLINE int CXTPGridInplaceButton::GetState() const
{
	return m_bOver ? m_nState : 0;
}
AFX_INLINE BOOL CXTPGridInplaceButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											  UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPGridInplaceButton::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect,
											  CWnd* pParentWnd, UINT nID)
{
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE CXTPGridRecordItem* CXTPGridInplaceButton::GetItem() const
{
	return pItem;
}
AFX_INLINE void CXTPGridInplaceButton::SetIconIndex(int nIconIndex)
{
	m_nIconIndex = nIconIndex;
}
AFX_INLINE int CXTPGridInplaceButton::GetIconIndex() const
{
	return m_nIconIndex;
}

//===========================================================================
// Summary:
//     CXTPGridInplaceList is a CXTPGridInplaceControl derived class
//     that represents a list box with item constraints.
// See Also:
//     CXTPGridRecordItemConstraints
//===========================================================================
class _XTP_EXT_CLASS CXTPGridInplaceList
	: public CListBox
	, public CXTPGridInplaceControl
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridInplaceList object.
	//-------------------------------------------------------------------------
	CXTPGridInplaceList();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an in-place list control.
	// Parameters:
	//     pItemArgs    - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                    structure containing item cell parameters.
	//     pConstraints - Pointer to a CXTPGridRecordItemConstraints
	//                    object containing item constraints.
	//-----------------------------------------------------------------------
	void Create(XTP_GRIDRECORDITEM_ARGS* pItemArgs, CXTPGridRecordItemConstraints* pConstraints);

	//-------------------------------------------------------------------------
	// Summary:
	//     Cancels the user selection.
	//-------------------------------------------------------------------------
	void Cancel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Saves the selected value of the list box.
	//-------------------------------------------------------------------------
	void Apply();

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets the number of items to show in the dropdown list.
	// Parameters:
	//      nNumber - A number of items to show.
	// Returns:
	//      A number of items to show.
	//-----------------------------------------------------------------------
	int GetDropListItemNumber() const;
	void SetDropListItemNumber(int nNumber); // <combine
											 // CXTPGridInplaceList::GetDropListItemNumber@const>

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPGridInplaceList)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPGridInplaceList)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT, CPoint point);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internally to fill the item arguments
	//     of an in-place control.
	// Parameters:
	//     pItemArgs - Pointer to an XTP_GRIDRECORDITEM_ARGS
	//                 structure containing item arguments.
	//-------------------------------------------------------------------------
	virtual void SetItemArgs(XTP_GRIDRECORDITEM_ARGS* pItemArgs);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internally when the selection is changed.
	// Parameters:
	//     nLBIndex - Index of the selected item in the list.
	//-------------------------------------------------------------------------
	virtual void OnSelectionChanged(int nLBIndex);

private:
	CString m_strHotSearchContext;
	DWORD m_dwLastKeyDownTime;
	BOOL m_bApply;
	int m_Items2Show; // Use GetDropListItemNumber/SetDropListItemNumber method to access it.

	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};

AFX_INLINE int CXTPGridInplaceList::GetDropListItemNumber() const
{
	return m_Items2Show;
}

AFX_INLINE void CXTPGridInplaceList::SetDropListItemNumber(int nNumber)
{
	ASSERT(0 <= nNumber);
	m_Items2Show = nNumber;
}

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPGridInplaceList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPGridInplaceList::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											UINT nID)
{
	return CListBox::Create(dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE int CXTPGridInplaceButton::GetWidth() const
{
	return m_nWidth;
}
AFX_INLINE BOOL CXTPGridInplaceButton::IsInsideCellButton() const
{
	return m_bInsideCell;
}
AFX_INLINE void CXTPGridInplaceButton::SetInsideCellButton(BOOL bInsideCell)
{
	m_bInsideCell = bInsideCell;
}
AFX_INLINE int CXTPGridInplaceButton::GetSpinButtonMin() const
{
	return m_nSpinMin;
}
AFX_INLINE int CXTPGridInplaceButton::GetSpinButtonMax() const
{
	return m_nSpinMax;
}
AFX_INLINE int CXTPGridInplaceButton::GetSpinButtonStep() const
{
	return m_nSpinStep;
}
AFX_INLINE void CXTPGridInplaceButton::SetSpinButtonMin(int nValue)
{
	m_nSpinMin = nValue;
}
AFX_INLINE void CXTPGridInplaceButton::SetSpinButtonMax(int nValue)
{
	m_nSpinMax = nValue;
}
AFX_INLINE void CXTPGridInplaceButton::SetSpinButtonStep(int nValue)
{
	m_nSpinStep = nValue;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPGRIDINPLACECONTROLS_H__)
