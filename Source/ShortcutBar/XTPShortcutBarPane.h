// XTPShortcutBarPane.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBARPANE_H__)
#	define __XTPSHORTCUTBARPANE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPShortcutBar;
class CXTPShortcutBarPane;

//===========================================================================
// Summary:
//     CXTPShortcutBarPaneItem is a base class that represents
//     an item for a shortcut bar pane.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarPaneItem
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPaneItem object.
	// Parameters:
	//     lpszCaption - Caption of the item.
	//     pWnd        - Pointer to the associated child window of the item.
	//     nHeight     - Height of the item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the caption.
	// Returns:
	//     The rectangle of the caption.
	//-----------------------------------------------------------------------
	CRect GetCaptionRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the item.
	// Returns
	//     The caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the item.
	// Parameters:
	//     lpszCaption - Caption to be set.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height for the item.
	// Parameters:
	//     nHeight - Height to be set.
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the item.
	// Returns:
	//     The height of the item.
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the caption.
	// Parameters:
	//     bShowCaption - TRUE to show the caption, FALSE to hide the caption.
	//-----------------------------------------------------------------------
	void ShowCaption(BOOL bShowCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the highlighted state of the item.
	// Returns:
	//     TRUE if the item is highlighted, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is expandable.
	// Returns:
	//     TRUE if the item is expandable, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpandable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the expanded state of the item.
	// Returns:
	//     TRUE if the item is expanded, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if the item should be expandable.
	// Parameters:
	//     bExpandable - TRUE to set the item to be expandable, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the expanded state of the item.
	// Parameters:
	//     bExpanded - TRUE to expand the item, FALSE to collapse the item.
	//-----------------------------------------------------------------------
	void SetExpanded(BOOL bExpanded);

protected:
	CString m_strCaption;		  // Caption of the item.
	CRect m_rcCaption;			  // Rectangle of the caption.
	CRect m_rcClient;			  // Bounding rectangle of the client area.
	CWnd* m_pWndClient;			  // Associated child window.
	int m_nHeight;				  // Height of the item.
	BOOL m_bShowCaption;		  // TRUE to show the caption.
	CXTPShortcutBarPane* m_pPane; // Parent pane object.

	BOOL m_bExpandable; // TRUE if the pane is expandable.
	BOOL m_bExpanded;   // TRUE if the pane is expanded.

	friend class CXTPShortcutBarPane;
};

//===========================================================================
// Summary:
//     CXTPShortcutBarPane is a CWnd derived class used
//     as a client area for a shortcut bar control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarPane : public CWnd
{
	DECLARE_DYNAMIC(CXTPShortcutBarPane)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPane object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarPane object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the CXTPShortcutBarPane control.
	// Parameters:
	//     lpszCaption - Caption of the pane.
	//     pParent     - Pointer to the parent CXTPShortcutBar object.
	//-----------------------------------------------------------------------
	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the pane.
	// Returns:
	//     The caption of the pane.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the pane.
	// Parameters:
	//     lpszCaption - Caption to be set.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified item to the pane.
	// Parameters:
	//     lpszCaption - Caption of the item.
	//     pWnd        - Pointer to the associated child window of the item.
	//     nHeight     - Height of the item.
	// Returns:
	//     A pointer to the newly added CXTPShortcutBarPaneItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* AddItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the item at the specified index.
	// Parameters:
	//     nIndex - Zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to the item at the specified index.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the indent placed around the client area of the shortcut bar.
	// Parameters:
	//     left   - Amount of space to be placed between the left client border
	//              and the left border of the shortcut bar client area.
	//     top    - Amount of space to be placed between the top client border
	//              and the top border of the shortcut bar client area.
	//     right  - Amount of space to be placed between the right client border
	//              and the right border of the shortcut bar client area.
	//     bottom - Amount of space to be placed between the bottom client border
	//              and the bottom border of the shortcut bar client area.
	// Remarks:
	//     The indent will be placed around the corresponding
	//     CXTPShortcutBarPaneItem when displayed.
	//-----------------------------------------------------------------------
	void SetIndent(int left, int top, int right, int bottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides the caption.
	// Parameters:
	//     bShowCaption - TRUE to show the caption, FALSE to hide the caption.
	//-----------------------------------------------------------------------
	void ShowCaption(BOOL bShowCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum height for the client area.
	// Parameters:
	//     nMinHeight - Minimum height, in pixels, to be set.
	//-----------------------------------------------------------------------
	void SetMinimumClientHeight(int nMinHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum height of the client area.
	// Returns:
	//     The minimum height, in pixels, of the client area.
	//-----------------------------------------------------------------------
	virtual int GetMinimumClientHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which item, if any, is located at a specified point.
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     A pointer to the CXTPShortcutBarPaneItem object at
	//     the specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* HitTest(CPoint point) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates the layout of the pane.
	//-------------------------------------------------------------------------
	void RecalcLayout();

	CXTPShortcutBar* GetShortcutBar() const;

	CRect GetMinimizeButtonRect() const;
	BOOL IsMinimizeButtonHighlighted() const;
	BOOL IsMinimizeButtonPressed() const;

	virtual COLORREF GetBackgroundColor() const;
	virtual COLORREF GetTextColor() const;

	void SetFlatStyle(BOOL bFlatStyle);
	BOOL IsFlatStyle() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPShortcutBarPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg void OnDraw(CDC* pDC);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnMouseLeave();
	void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CXTPShortcutBar* m_pShortcutBar; // Parent CXTPShortcutBar class.
	CString m_strCaption;			 // Caption of the pane.
	CArray<CXTPShortcutBarPaneItem*, CXTPShortcutBarPaneItem*> m_arrItems; // Array of items.
	CRect m_rcIndent;						 // Indentation/padding around the client pane.
	BOOL m_bShowCaption;					 // TRUE to show caption.
	int m_nMinClientHeight;					 // The minimum height of the client area.
	CXTPShortcutBarPaneItem* m_pHighlighted; // Highlighted shortcut item.
	HCURSOR m_hHandCursor;					 // Hand cursor.

	CRect m_rcMinimizeButton;
	BOOL m_bMinimizeButtonHighlighted;
	BOOL m_bMinimizeButtonPressed;
	BOOL m_bFlatStyle;

	friend class CXTPShortcutBarPaneItem;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CString CXTPShortcutBarPane::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE void CXTPShortcutBarPane::SetIndent(int left, int top, int right, int bottom)
{
	m_rcIndent.SetRect(left, top, right, bottom);
}
AFX_INLINE void CXTPShortcutBarPane::ShowCaption(BOOL bShowCaption)
{
	m_bShowCaption = bShowCaption;
}
AFX_INLINE BOOL CXTPShortcutBarPane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
											UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE CRect CXTPShortcutBarPaneItem::GetCaptionRect() const
{
	return m_rcCaption;
}
AFX_INLINE int CXTPShortcutBarPaneItem::GetHeight() const
{
	return m_nHeight;
}
AFX_INLINE BOOL CXTPShortcutBarPaneItem::IsHighlighted() const
{
	return m_pPane->m_pHighlighted == this;
}
AFX_INLINE CString CXTPShortcutBarPaneItem::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE void CXTPShortcutBarPaneItem::ShowCaption(BOOL bShowCaption)
{
	m_bShowCaption = bShowCaption;
}
AFX_INLINE void CXTPShortcutBarPane::SetMinimumClientHeight(int nMinHeight)
{
	m_nMinClientHeight = nMinHeight;
}
AFX_INLINE int CXTPShortcutBarPane::GetMinimumClientHeight() const
{
	return m_nMinClientHeight;
}
AFX_INLINE BOOL CXTPShortcutBarPaneItem::IsExpandable() const
{
	return m_bExpandable && m_bShowCaption;
}
AFX_INLINE BOOL CXTPShortcutBarPaneItem::IsExpanded() const
{
	return m_bExpanded;
}
AFX_INLINE void CXTPShortcutBarPaneItem::SetExpandable(BOOL bExpandable)
{
	m_bExpandable = bExpandable;
}
AFX_INLINE void CXTPShortcutBarPaneItem::SetExpanded(BOOL bExpanded)
{
	if (m_bExpanded != bExpanded)
	{
		m_bExpanded = bExpanded;
		m_pPane->RecalcLayout();
	}
}
AFX_INLINE CXTPShortcutBar* CXTPShortcutBarPane::GetShortcutBar() const
{
	return m_pShortcutBar;
}
AFX_INLINE CRect CXTPShortcutBarPane::GetMinimizeButtonRect() const
{
	return m_rcMinimizeButton;
}
AFX_INLINE BOOL CXTPShortcutBarPane::IsMinimizeButtonHighlighted() const
{
	return m_bMinimizeButtonHighlighted;
}
AFX_INLINE BOOL CXTPShortcutBarPane::IsMinimizeButtonPressed() const
{
	return m_bMinimizeButtonPressed;
}
AFX_INLINE void CXTPShortcutBarPane::SetFlatStyle(BOOL bFlatStyle)
{
	m_bFlatStyle = bFlatStyle;
	if (m_hWnd)
		Invalidate(FALSE);
}
AFX_INLINE BOOL CXTPShortcutBarPane::IsFlatStyle() const
{
	return m_bFlatStyle;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSHORTCUTBARPANE_H__)
