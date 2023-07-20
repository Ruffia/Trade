// XTPPropertyGridInplaceEdit.h interface for the CXTPPropertyGridInplaceEdit class.
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
#if !defined(__XTPPROPERTYGRIDINPLACEEDIT_H__)
#	define __XTPPROPERTYGRIDINPLACEEDIT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// class forwards.

class CXTPPropertyGridView;
class CXTPPropertyGridItem;

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceEdit is a CEdit derived class.
//     It is an internal class used by the property grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceEdit : public CXTPMaskEditT<CEdit>
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceEdit)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceEdit object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceEdit object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the value for the edit text.
	// Parameters:
	//     strValue - Value to be set.
	//-----------------------------------------------------------------------
	virtual void SetValue(LPCTSTR strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Hides the in-place edit control.
	//-----------------------------------------------------------------------
	virtual void HideWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the in-place edit control.
	// Parameters:
	//     pItem - Pointer to a CXTPPropertyGridItem object.
	//     rect  - Specifies the size and position of the item.
	//-----------------------------------------------------------------------
	virtual void Create(CXTPPropertyGridItem* pItem, CRect rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the parent item is destroyed.
	//-----------------------------------------------------------------------
	virtual void DestroyItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current item.
	// Returns:
	//     A pointer to the current item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects the next constraint value in the specified direction
	//     within the edit control.
	// Parameters:
	//     nDirection - Direction for the constraint search.
	//     bCycle     - TRUE to enable starting from the opposite end
	//                  of the constraint list in case either
	//                  the beginning or end of the constraint list
	//                  is reached in the specified direction.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SelectConstraint(int nDirection, BOOL bCycle);

protected:
	CString m_strValue; // Previous item value.

	CXTPBrush m_xtpBrushBack; // Background brush.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brBack, m_xtpBrushBack, GetBackBrushHandle);

	COLORREF m_clrBack;			   // Background color.
	CXTPPropertyGridView* m_pGrid; // Parent grid view.
	CXTPPropertyGridItem* m_pItem; // Current item.
	BOOL m_bDelayCreate;		   // TRUE to create Edit after activate.
	BOOL m_bCreated;			   // TRUE if the Create method was executed.
	BOOL m_bEditMode;			   // TRUE after edit first receive focus.
	BOOL m_bCancel;				   // User cancel edit with Escape.
	BOOL m_bCheckAutoComplete;	 // Check auto-complete.
	BOOL m_bIgnoreEditChanged;	 // TRUE to ignore edit changed notification.

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPPropertyGridInplaceEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGridInplaceEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	DECLARE_MESSAGE_MAP()
	friend class CXTPPropertyGridItem;
	friend class CXTPPropertyGrid;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceControl is a CWnd derived class that represents
//     the base class for all items with in-place controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceControl : public CWnd
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceControl)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceControl object.
	// Parameters:
	//     pItem - Pointer to the parent CXTPPropertyGridItem object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControl(CXTPPropertyGridItem* pItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item creates its in-place control.
	// Parameters:
	//     rcValue - Reference to the bounding rectangle of the control.
	// See Also:
	//     OnDestroyWindow
	//-----------------------------------------------------------------------
	virtual void OnCreateWindow(CRect& rcValue) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item destroys its in-place control.
	// See Also:
	//     OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnDestroyWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item adjusts the rectangle of its value part.
	// Parameters:
	//     rcValue - Reference to the bounding rectangle of the value part.
	// See Also:
	//     OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnAdjustValueRect(CRect& rcValue) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the parent item is changed.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user rolls the mouse wheel while
	//     in an edit control.
	// Parameters:
	//     pEdit  - Pointer to the currently selected edit control.
	//     zDelta - Mouse wheel delta.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditMouseWheel(CEdit* pEdit, short zDelta);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	virtual void OnFinalRelease();

	//{{AFX_MSG(CXTPPropertyGridInplaceControl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPPropertyGridItem* m_pItem; // Parent item pointer
	int m_nWidth;				   // Width of in-place item
};

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceSlider is a CXTPPropertyGridInplaceControl derived class.
//     It represents the in-place slider of an item.
//     Call the CXTPPropertyGridItem::AddSliderControl() method to add a slider
//     to a property grid item.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceSlider : public CXTPPropertyGridInplaceControl
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceSlider)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceSlider object.
	// Parameters:
	//     pItem - Pointer to the parent CXTPPropertyGridItem object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceSlider(CXTPPropertyGridItem* pItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item creates its in-place control.
	// Parameters:
	//     rcValue - Reference to the bounding rectangle of the control.
	// See Also:
	//     OnDestroyWindow
	//-----------------------------------------------------------------------
	virtual void OnCreateWindow(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item adjusts the rectangle of its value part.
	// Parameters:
	//     rcValue - Reference to the bounding rectangle of the value part.
	// See Also:
	//     OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnAdjustValueRect(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the parent item is changed.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the slider control.
	// Returns:
	//     A pointer to the slider control.
	//-----------------------------------------------------------------------
	CSliderCtrl* GetSliderCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the lower range for the slider control.
	// Parameters:
	//     nMin - Lower range to be set.
	// See Also:
	//     SetMax
	//-----------------------------------------------------------------------
	void SetMin(int nMin);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the upper range for the slider control.
	// Parameters:
	//     nMax - Upper range to be set.
	// See Also:
	//     SetMin
	//-----------------------------------------------------------------------
	void SetMax(int nMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the lower range of the slider control.
	// Returns:
	//     The lower range of the slider control.
	//-----------------------------------------------------------------------
	int GetMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the upper range of the slider control.
	// Returns:
	//     The upper range of the slider control.
	//-----------------------------------------------------------------------
	int GetMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user rolls the mouse wheel while
	//     in an edit control.
	// Parameters:
	//     pEdit  - Pointer to the currently selected edit control.
	//     zDelta - Mouse wheel delta.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditMouseWheel(CEdit* pEdit, short zDelta);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the slider is moved by the user.
	// Parameters:
	//     nSBCode - Slider code that indicates the user's scrolling request.
	//     nPos    - Current slider position.
	//-----------------------------------------------------------------------
	virtual void OnScroll(UINT nSBCode, UINT nPos);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	int CalcualteEditWidth();

	//{{AFX_MSG(CXTPPropertyGridInplaceSlider)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridInplaceSlider);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	int m_nMin;   // Lower range.
	int m_nMax;   // Upper range.
	int m_nValue; // Value of the slider.

private:
	COLORREF m_clrBack;
	CXTPBrush m_xtpBrushBack;
};

AFX_INLINE CSliderCtrl* CXTPPropertyGridInplaceSlider::GetSliderCtrl() const
{
	return (CSliderCtrl*)this;
}
AFX_INLINE void CXTPPropertyGridInplaceSlider::SetMin(int nMin)
{
	m_nMin = nMin;
}
AFX_INLINE void CXTPPropertyGridInplaceSlider::SetMax(int nMax)
{
	m_nMax = nMax;
}
AFX_INLINE int CXTPPropertyGridInplaceSlider::GetMin() const
{
	return m_nMin;
}
AFX_INLINE int CXTPPropertyGridInplaceSlider::GetMax() const
{
	return m_nMax;
}

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceSpinButton is a CXTPPropertyGridInplaceControl derived class.
//     It represents the in-place spin button of an item.
//     Call the CXTPPropertyGridItem::AddSpinButton() method to add a spin button
//     to a property grid item.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceSpinButton : public CXTPPropertyGridInplaceControl
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceSpinButton)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceSpinButton object.
	// Parameters:
	//     pItem - Pointer to the parent CXTPPropertyGridItem object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceSpinButton(CXTPPropertyGridItem* pItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item creates its in-place control.
	// Parameters:
	//     rcValue - Reference to the bounding rectangle of the control.
	// See Also:
	//     OnDestroyWindow
	//-----------------------------------------------------------------------
	virtual void OnCreateWindow(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item adjusts the rectangle of its value part.
	// Parameters:
	//     rcValue - Reference to the bounding rectangle of the value part.
	// See Also:
	//     OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnAdjustValueRect(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the value of the parent item is changed.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the spin button control.
	// Returns:
	//     A pointer to the spin button control.
	//-----------------------------------------------------------------------
	CSpinButtonCtrl* GetSpinButtonCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the lower range for the spin button control.
	// Parameters:
	//     nMin - Lower range to be set.
	// See Also:
	//     SetMax
	//-----------------------------------------------------------------------
	void SetMin(int nMin);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the upper range for the spin button control.
	// Parameters:
	//     nMax - Upper range to be set.
	// See Also:
	//     SetMin
	//-----------------------------------------------------------------------
	void SetMax(int nMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the lower range of the spin button control.
	// Returns:
	//     The lower range of the spin button control.
	//-----------------------------------------------------------------------
	int GetMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the upper range of the spin button control.
	// Returns:
	//     The upper range of the spin button control.
	//-----------------------------------------------------------------------
	int GetMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user rolls the mouse wheel while
	//     in an edit control.
	// Parameters:
	//     pEdit  - Pointer to the currently selected edit control.
	//     zDelta - Mouse wheel delta.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditMouseWheel(CEdit* pEdit, short zDelta);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPPropertyGridInplaceSpinButton)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridInplaceSpinButton);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	int m_nMin; // Lower range.
	int m_nMax; // Upper range.
};

AFX_INLINE CSpinButtonCtrl* CXTPPropertyGridInplaceSpinButton::GetSpinButtonCtrl() const
{
	return (CSpinButtonCtrl*)this;
}
AFX_INLINE void CXTPPropertyGridInplaceSpinButton::SetMin(int nMin)
{
	m_nMin = nMin;
}
AFX_INLINE void CXTPPropertyGridInplaceSpinButton::SetMax(int nMax)
{
	m_nMax = nMax;
}
AFX_INLINE int CXTPPropertyGridInplaceSpinButton::GetMin() const
{
	return m_nMin;
}
AFX_INLINE int CXTPPropertyGridInplaceSpinButton::GetMax() const
{
	return m_nMax;
}

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceControls is a standalone class that represents
//     a collection of CXTPPropertyGridInplaceControl classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceControls
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceControls object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControls();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceControls object, handles
	//     cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPPropertyGridInplaceControls();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all in-place controls from the collection.
	// See Also:
	//     Add
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of controls in the collection.
	// Returns:
	//     The number of controls in the collection.
	// See Also:
	//     GetAt
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds an in-place control to the collection.
	// Parameters:
	//     pWindow - Pointer to the in-place control to add.
	// See Also:
	//     GetAt
	//-----------------------------------------------------------------------
	void Add(CXTPPropertyGridInplaceControl* pWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the in-place control at a specified index in the collection.
	// Parameters:
	//     nIndex - Index of the control to retrieve.
	// Returns:
	//     A pointer to the in-place control at the specified index in the collection.
	// See Also:
	//     GetCount, Add
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControl* GetAt(int nIndex);

protected:
	CArray<CXTPPropertyGridInplaceControl*, CXTPPropertyGridInplaceControl*>
		m_arrControls; // Array of controls.
};

AFX_INLINE CXTPPropertyGridItem* CXTPPropertyGridInplaceEdit::GetItem()
{
	return m_pItem;
}
AFX_INLINE BOOL CXTPPropertyGridInplaceEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
													DWORD dwStyle, const RECT& rect,
													CWnd* pParentWnd, UINT nID,
													CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPPropertyGridInplaceEdit::Create(DWORD dwStyle, const RECT& rect,
													CWnd* pParentWnd, UINT nID)
{
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE BOOL CXTPPropertyGridInplaceControl::OnEditMouseWheel(CEdit* /*pEdit*/, short /*zDelta*/)
{
	return FALSE;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROPERTYGRIDINPLACEEDIT_H__)
