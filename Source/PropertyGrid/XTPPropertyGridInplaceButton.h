// XTPPropertyGridInplaceButton.h interface for the CXTPPropertyGridInplaceButton class.
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
#if !defined(__XTPPROPERTYGRIDINPLACEBUTTON_H__)
#	define __XTPPROPERTYGRIDINPLACEBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// class forwards

class CXTPPropertyGridView;
class CXTPPropertyGridItem;
class CXTPImageManagerIcon;

const UINT XTP_ID_PROPERTYGRID_EXPANDBUTTON =
	100; //<ALIAS CXTPPropertyGridInplaceButton::CXTPPropertyGridInplaceButton@UINT>
const UINT XTP_ID_PROPERTYGRID_COMBOBUTTON =
	101; //<ALIAS CXTPPropertyGridInplaceButton::CXTPPropertyGridInplaceButton@UINT>

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceButton is a CXTPCmdTarget derived class.
//     It represents an in-place button in the property grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceButton : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceButton object.
	// Parameters:
	//     nID - Identifier of the button.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButton(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceButton object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridInplaceButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width of the button.
	// Returns:
	//     The width of the button.
	// See Also:
	//     SetWidth
	//-----------------------------------------------------------------------
	int GetWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the width for the button.
	// Parameters:
	//     nWidth - Width to be set.
	// See Also:
	//     GetWidth
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption text of the button.
	// Returns:
	//     The caption text of the button.
	// See Also:
	//     SetCaption
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption text for the button.
	// Parameters:
	//     lpszCaption - Caption text to be set.
	// See Also:
	//     GetCaption
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the image index of the button.
	// Returns:
	//     The image index of the button.
	// See Also:
	//     SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text for the item.
	// Parameters:
	//     lpstrTooltip - Tooltip text to be set.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpstrTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the item.
	// Remarks:
	//     The tooltip text is displayed when the mouse cursor is over the item.
	// Returns:
	//     The tooltip text of the item.
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon index for the button.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// Remarks:
	//     Icons should be added to the image manager of the property grid
	//     prior to using this method.
	// See Also:
	//     GetImage, GetIconIndex, CXTPPropertyGrid::GetImageManager
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the icon of the button.
	// Returns:
	//     A pointer to the icon of the button.
	// See Also:
	//     SetIconIndex, CXTPPropertyGrid::GetImageManager
	//-----------------------------------------------------------------------
	virtual CXTPImageManagerIcon* GetImage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent property grid item.
	// Returns:
	//     A pointer to the parent property grid item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the button.
	// Remarks:
	//     The default identifier for the expand button is XTP_ID_PROPERTYGRID_EXPANDBUTTON.
	//     The default identifier for the combo button is XTP_ID_PROPERTYGRID_COMBOBUTTON.
	// Returns:
	//     The indentifier of the button.
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the button.
	// Returns:
	//     The bounding rectangle of the button.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button has focus.
	// Returns:
	//     TRUE if the button has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is hot (i.e. currently highlighted).
	// Returns:
	//     TRUE if the button is hot, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHot() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is pressed.
	// Returns:
	//     TRUE if the button is pressed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of this button within the collection of
	//     in-place buttons of the parent item.
	// Returns:
	//     The index of this button within the collection of
	//     in-place buttons of the parent item.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the button. Override this method to draw a custom button.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Bounding rectangle of the button.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the
	//     left mouse button on the button.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses a key
	//     while the button has focus.
	// Parameters:
	//     nChar - Specifies the virtual-key code of the given key.
	//-----------------------------------------------------------------------
	virtual void OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent property grid view.
	// Returns:
	//     A pointer to the parent property grid view.
	//-----------------------------------------------------------------------
	CXTPPropertyGridView* GetGrid() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is enabled/disabled.
	// Returns:
	//     TRUE if the button is enabled, FALSE if the button is disabled.
	//-----------------------------------------------------------------------
	BOOL GetEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the button.
	// Parameters:
	//     bEnabled - TRUE to enable the button, FALSE to disable the button.
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button acts as a hyperlink.
	// Returns:
	//     TRUE if the button acts as a hyperlink, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHyperlink() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the button should act as a hyperlink.
	// Parameters:
	//     bHyperlink - TRUE to set the button to act as a hyperlink,
	//                  FALSE to prevent the button from acting as a hyperlink.
	//-----------------------------------------------------------------------
	void SetHyperlink(BOOL bHyperlink = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the button is always shown.
	// Returns:
	//     TRUE if the button is always shown,
	//     FALSE if the button is only shown when the item is selected.
	// See Also:
	//     SetShowAlways
	//-----------------------------------------------------------------------
	BOOL GetShowAlways() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the button should always be shown.
	// Parameters:
	//     bShowAlways - TRUE to always show the button,
	//                   FALSE to only show the button when the item is selected.
	// See Also:
	//     GetShowAlways
	//-----------------------------------------------------------------------
	void SetShowAlways(BOOL bShowAlways);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the alignment of the button.
	// Returns:
	//     The alignment of the button.
	//-----------------------------------------------------------------------
	int GetAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the alignment for the button.
	// Parameters:
	//     nAlignment - Alignment to be set (DT_LEFT or DT_RIGHT).
	//-----------------------------------------------------------------------
	void SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the button is a tab stop
	//     (i.e. if the user can use the TAB key to move focus to the button).
	// Returns:
	//     TRUE if the button is a tab stop, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetTabStop() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE

	BOOL m_bPressed; // TRUE if the button is pressed.
	int m_nIndex;

	CXTPPropertyGridView* m_pGrid;
	CXTPPropertyGridItem* m_pItem;
	UINT m_nID;
	CRect m_rcButton;
	int m_nWidth;
	CString m_strCaption;
	int m_nIconIndex;
	BOOL m_bEnabled;
	CString m_strTooltip;
	BOOL m_bHyperlink;
	BOOL m_bShowAlways;
	BOOL m_bTabStop;
	int m_nAlignment;
	//}}AFX_CODEJOCK_PRIVATE

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridInplaceButton);
	LPDISPATCH OleGetItem();
	BSTR OleGetCaption();

//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	friend class CXTPPropertyGridItem;
	friend class CXTPPropertyGridItems;
	friend class CXTPPropertyGridInplaceButtons;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceButtons is a CXTPCmdTarget derived class.
//     It represents a collection of in-place buttons for a specified grid item.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceButtons : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceButtons object
	// Parameters:
	//     pItem - Pointer to the parent CXTPPropertyGridItem object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButtons(CXTPPropertyGridItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceButtons object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPPropertyGridInplaceButtons();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a standard combo button to the collection.
	//-----------------------------------------------------------------------
	void AddComboButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a standard expand button to the collection.
	//-----------------------------------------------------------------------
	void AddExpandButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a custom button to the collection.
	// Parameters:
	//     pButton - Pointer to the button to add.
	// Returns:
	//     A pointer to the newly added CXTPPropertyGridInplaceButton object.
	// Example:
	//     <code>pItem->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(1));</code>
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButton* AddButton(CXTPPropertyGridInplaceButton* pButton);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of buttons in the collection.
	// Returns:
	//     The number of buttons in the collection.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the button at a specified index within the collection.
	// Parameters:
	//     nIndex - Index of the button to retrieve.
	// Returns:
	//     A pointer to the CXTPPropertyGridInplaceButton object at the specified index
	//     if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButton* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all buttons from the collection.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified button from the collection.
	// Parameters:
	//     pButton - Pointer to the button to remove.
	//-----------------------------------------------------------------------
	void Remove(CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the button with a specified identifier from the collection.
	// Parameters:
	//     nID - Identifier of the button to remove.
	//-----------------------------------------------------------------------
	void Remove(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the button at a specified index within the collection.
	// Parameters:
	//     nIndex - Index of the button to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(long nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which button, if any, is at a specified position.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the button at the specified position, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButton* HitTest(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the button with a specified identifier within the collection.
	// Parameters:
	//     nID - Identifier of the button to retrieve.
	// Returns:
	//     A pointer to the CXTPPropertyGridInplaceButton object with the
	//     specified identifier if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceButton* Find(UINT nID) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a button is removed.
	// Parameters:
	//     pButton - Pointer to the button being removed.
	// Remarks:
	//     This method removes the possible reference from the property grid
	//     to the button in this collection.
	//-----------------------------------------------------------------------
	virtual void OnButtonRemoved(CXTPPropertyGridInplaceButton* pButton);

private:
	void UpdateIndexes();

private:
	CArray<CXTPPropertyGridInplaceButton*, CXTPPropertyGridInplaceButton*> m_arrButtons;
	CXTPPropertyGridItem* m_pItem;

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridInplaceButtons);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleItem(long nIndex);
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg void OleClear();
	afx_msg void OleRemove(long nIndex);
	afx_msg LPDISPATCH OleAdd(UINT ID);
	afx_msg LPDISPATCH OleFindButton(long nId);

	DECLARE_ENUM_VARIANT(CXTPPropertyGridInplaceButtons)

//}}AFX_CODEJOCK_PRIVATE
#	endif
	friend class CXTPPropertyGridItem;
};

AFX_INLINE CXTPPropertyGridItem* CXTPPropertyGridInplaceButton::GetItem() const
{
	return m_pItem;
}
AFX_INLINE UINT CXTPPropertyGridInplaceButton::GetID() const
{
	return m_nID;
}
AFX_INLINE CRect CXTPPropertyGridInplaceButton::GetRect() const
{
	return m_rcButton;
}
AFX_INLINE CString CXTPPropertyGridInplaceButton::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE int CXTPPropertyGridInplaceButton::GetIconIndex() const
{
	return m_nIconIndex;
}
AFX_INLINE void CXTPPropertyGridInplaceButton::SetIconIndex(int nIconIndex)
{
	m_nIconIndex = nIconIndex;
}
AFX_INLINE CXTPPropertyGridView* CXTPPropertyGridInplaceButton::GetGrid() const
{
	return m_pGrid;
}
AFX_INLINE void CXTPPropertyGridInplaceButton::SetTooltip(LPCTSTR lpstrTooltip)
{
	m_strTooltip = lpstrTooltip;
}
AFX_INLINE CString CXTPPropertyGridInplaceButton::GetTooltip() const
{
	return m_strTooltip;
}

AFX_INLINE int CXTPPropertyGridInplaceButtons::GetCount() const
{
	return (int)m_arrButtons.GetSize();
}
AFX_INLINE CXTPPropertyGridInplaceButton* CXTPPropertyGridInplaceButtons::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetCount() ? m_arrButtons.GetAt(nIndex) : NULL;
}
AFX_INLINE void CXTPPropertyGridInplaceButton::SetHyperlink(BOOL bHyperlink)
{
	m_bHyperlink = bHyperlink;
}
AFX_INLINE BOOL CXTPPropertyGridInplaceButton::IsHyperlink() const
{
	return m_bHyperlink;
}
AFX_INLINE BOOL CXTPPropertyGridInplaceButton::GetShowAlways() const
{
	return m_bShowAlways;
}
AFX_INLINE void CXTPPropertyGridInplaceButton::SetShowAlways(BOOL bShowAlways)
{
	m_bShowAlways = bShowAlways;
}
AFX_INLINE int CXTPPropertyGridInplaceButton::GetAlignment() const
{
	return m_nAlignment;
}
AFX_INLINE void CXTPPropertyGridInplaceButton::SetAlignment(int nAlignment)
{
	m_nAlignment = nAlignment;
}
AFX_INLINE BOOL CXTPPropertyGridInplaceButton::GetTabStop() const
{
	return m_bTabStop;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPPROPERTYGRIDINPLACEBUTTON_H__)
