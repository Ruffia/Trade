// XTPTaskPanelGroupItem.h interface for the CXTPTaskPanelGroupItem class.
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
#if !defined(__XTPTASKPANELGROUPITEM_H__)
#	define __XTPTASKPANELGROUPITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelGroupItem is used in a TaskPanel control
//     to represent a single item of a group.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroupItem : public CXTPTaskPanelItem
{
	DECLARE_SERIAL(CXTPTaskPanelGroupItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelGroupItem object
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelGroupItem object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelGroupItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the group item must be repositioned.
	// Parameters:
	//     rc - Rectangle of the group item.
	//-----------------------------------------------------------------------
	virtual CRect OnReposition(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the group item.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Rectangle of the group item to draw.
	//-----------------------------------------------------------------------
	virtual void OnDrawItem(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the margins of the group item.
	// Returns:
	//     A reference to the margins of the group item.
	//-----------------------------------------------------------------------
	CRect& GetMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the group item.
	// Returns:
	//     The bounding rectangle of the group item.
	//-----------------------------------------------------------------------
	CRect GetItemRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bounding rectangle for the group item.
	// Parameters:
	//     lpRect  - Bounding rectangle to be set.
	//-----------------------------------------------------------------------
	void SetItemRect(LPCRECT lpRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the parent group of the item.
	// Returns:
	//     A pointer to the parent group of the item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* GetItemGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables bold font for the group item.
	// Parameters:
	//     bBold - TRUE to enable bold font, FALSE to disable bold font.
	//-----------------------------------------------------------------------
	void SetBold(BOOL bBold = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if bold font is enabled/disabled for the group item.
	// Returns:
	//     TRUE if bold font is enabled, FALSE if bold font is disabled.
	//-----------------------------------------------------------------------
	BOOL IsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the window handle for the group item.
	// Parameters:
	//     hWnd - Window handle to be set.
	//-----------------------------------------------------------------------
	void SetControlHandle(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the window handle of the group item.
	// Returns:
	//     The window handle of the group item.
	//-----------------------------------------------------------------------
	HWND GetControlHandle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an animation step is executed.
	// Parameters:
	//     nStep - Animation step that was executed.
	//-----------------------------------------------------------------------
	void OnAnimate(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size of the group item.
	// Parameters:
	//     szItem      - Group item size to be set.
	//     nAutoHeight - TRUE to enable auto-height for the group item,
	//                   FALSE to disable auto-height for the group item.
	// Remarks:
	//     If auto-height is enabled, then the height of the group item will be
	//     stretched to fill the entire height of the group.
	//     If auto-height is disabled, then the group item will use a defined height.
	// See Also:
	//     GetSize
	//-----------------------------------------------------------------------
	void SetSize(CSize szItem, BOOL bAutoHeight = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the backcolor of the group item.
	// Returns:
	//     The backcolor of the group item.
	//-----------------------------------------------------------------------
	COLORREF GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text color of the group item.
	// Returns:
	//     The text color of the group item.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the text color for the group item.
	// Parameters:
	//     clrText - Text color to be set.
	// ----------------------------------------------------------------------
	void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle of the "tooltip zone".
	// Returns:
	//     The rectangle of the "tooltip zone".
	// Remarks:
	//     The "tooltip zone" is the area where the tooltip will be displayed
	//     when the mouse is positioned over the group item.
	// See Also:
	//     CXTPTaskPanelGroup::GetHitTestRect
	//-----------------------------------------------------------------------
	CRect GetHitTestRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected state of the group item.
	// Remarks:
	//     Multiple group items can be selected at a given time.
	//     However, only one group item can have focus at a given time.
	// Returns:
	//     TRUE if the group item is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected state of the group item.
	// Parameters:
	//     bSelected - TRUE to set the group item to selected, FALSE otherwise.
	// Remarks:
	//     Multiple items can have focus.
	//     See CXTPTaskPanel::SetSelectItemOnFocus for details.
	// See Also:
	//     CXTPTaskPanel::SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	void SetItemSelected(BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads/writes this object from/to an archive.
	// Parameters:
	//     pPX - Pointer to a CXTPPropExchange object to serialize to/from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the drag text for the group item.
	// Parameters:
	//     lpszDragText - Drag text to be set.
	//----------------------------------------------------------------------
	void SetDragText(LPCTSTR lpszDragText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the drag text of the group item.
	// Returns:
	//     The drag text of the group item.
	//----------------------------------------------------------------------
	virtual CString GetDragText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Caches the COleDataSource object containing the group item being dragged.
	// Parameters:
	//     srcItem        - Reference to the COleDataSource object to cache.
	//     bCacheTextData - TRUE to cache the text data of the object, FALSE otherwise.
	// Returns:
	//     TRUE if successful, FALSE if CacheGlobalData returns NULL.
	// Remarks:
	//     This method places the item being dragged onto the clipboard.
	// See Also:
	//     CreateFromOleData, CacheGlobalData, CopyToClipboard,
	//     PasteFromClipboard, PrepareDrag
	//-----------------------------------------------------------------------
	BOOL PrepareDrag(COleDataSource& srcItem, BOOL bCacheTextData = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the size of the group item.
	// Returns:
	//     The size of the group item.
	// See Also:
	//     SetSize
	//-----------------------------------------------------------------------
	CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if auto-height is enabled/disabled for the group item.
	// Returns:
	//     TRUE if auto-height is enabled, FALSE if auto-height is disabled.
	// Remarks:
	//     If auto-height is enabled, then the height of the group item will be
	//     stretched to fill the entire height of the group.
	//     If auto-height is disabled, then the group item will use a defined height.
	//-----------------------------------------------------------------------
	BOOL IsAutoHeight() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility Support

	virtual HRESULT GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
	virtual HRESULT AccessibleDoDefaultAction(VARIANT varChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual HRESULT AccessibleSelect(long flagsSelect, VARIANT varChild);
	//}}AFX_CODEJOCK_PRIVATE

private:
	void GetPreviewBitmap(CWnd* pWnd, CBitmap& bmp);

protected:
	CRect m_rcMargins; // Item's margins.
	CRect m_rcItem;	// Rectangle of the item.

	BOOL m_bBold;	 // TRUE to use bold font.
	BOOL m_bSelected; // TRUE if the item is currently selected.

	HWND m_hWnd; // Only used for items of type xtpTaskItemTypeControl; this is
				 // the control's child window.
	CSize m_szItem; // Size of the item. If the size is set to 0, then the size will
					// be calculated by the Task Panel.
	CBitmap m_bmpPreview;  // Internally used. This holds a screen shot of the attached Windows
						   // control when the item type is xtpTaskItemTypeControl. This screen shot
						   // is used during group animation. During animation, the Windows control
						   // is hidden and the screen shot of the control is used in place of the
						   // actual control during the animation process.
	CSize m_szPreview;	 // Last preview bitmap size.
	CString m_strDragText; // Drag text of the item.
	BOOL m_bAutoHeight;	// TRUE if the item stretched inside its group.
	COLORREF m_clrText;	// Text color.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	afx_msg void OleSetMargins(long nLeft, long nTop, long nRight, long nBottom);
	afx_msg void OleSetControl(LPDISPATCH lpDispatch);

	afx_msg LPDISPATCH OleGetControl()
	{
		return NULL;
	}
	afx_msg HWND OleGetHandle();
	afx_msg void OleSetHandle(HWND);
	afx_msg BSTR OlegGetDragText();
	afx_msg void OleSetSize(long cx, long cy, BOOL bAutHeight);
	afx_msg void OleRedrawPanel();
	afx_msg void OleGetRect(long* nLeft, long* nTop, long* nRight, long* nBottom);

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTaskPanelGroupItem)

	enum
	{
		dispidBold		 = 10L,
		dispidSetMargins = 11L,
		dispidControl	= 12L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif

	friend class CXTPTaskPanelGroup;
};

AFX_INLINE CRect& CXTPTaskPanelGroupItem::GetMargins()
{
	return m_rcMargins;
}
AFX_INLINE CRect CXTPTaskPanelGroupItem::GetItemRect() const
{
	return m_rcItem;
}
AFX_INLINE void CXTPTaskPanelGroupItem::SetItemRect(LPCRECT lpRect)
{
	::CopyRect(&m_rcItem, lpRect);
}
AFX_INLINE CSize CXTPTaskPanelGroupItem::GetSize() const
{
	return m_szItem;
}
AFX_INLINE BOOL CXTPTaskPanelGroupItem::IsAutoHeight() const
{
	return m_bAutoHeight;
}
AFX_INLINE COLORREF CXTPTaskPanelGroupItem::GetTextColor() const
{
	return m_clrText;
}
AFX_INLINE void CXTPTaskPanelGroupItem::SetTextColor(COLORREF clr)
{
	m_clrText = clr;
	RedrawPanel();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTASKPANELGROUPITEM_H__)
