// XTPControlGallery.h
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
#if !defined(__XTPCONTROLGALLERY_H__)
#	define __XTPCONTROLGALLERY_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPControlGalleryItems;
class CXTPControlGallery;
class CXTPControlGalleryPaintManager;
class CXTPImageManagerIcon;
class CXTPMarkupUIElement;
class CXTPMarkupContext;

const UINT XTP_GN_PREVIEWSTART = 0x1010;  // Gallery Notification - Occurs when the gallery preview
										  // is started.
const UINT XTP_GN_PREVIEWCANCEL = 0x1011; // Gallery Notification - Occurs when the gallery preview
										  // is canceled.
const UINT XTP_GN_PREVIEWAPPLY = 0x1012;  // Gallery Notification - Occurs when the gallery preview
										  // is applied.
const UINT XTP_GN_PREVIEWCHANGE = 0x1013; // Gallery Notification - Occurs when the gallery preview
										  // is changed.
const UINT XTP_GN_STARTDRAG = 0x1014; // Gallery Notification - Occurs when an item is dragged in
									  // the gallery.
const UINT XTP_GN_SELCHANGE = 0x1015; // Gallery Notification - Occurs when the gallery selection
									  // has changed.

class CXTPControlGalleryItem;

//===========================================================================
// Summary:
//     CXTPControlGallery is a CXTPControlPopup derived class.
//     It represents a gallery control toolbar.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGallery
	: public CXTPControlPopup
	, public CXTPScrollBase
{
	DECLARE_XTP_CONTROL(CXTPControlGallery)

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Structure contains item position description.
	//-------------------------------------------------------------------------
	struct GALLERYITEM_POSITION
	{
		RECT rcItem;				   // Bounding rectangle of the item.
		CXTPControlGalleryItem* pItem; // Item pointer.
		BOOL bBeginRow;				   // TRUE if the item starts a new row.
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGallery object.
	//-----------------------------------------------------------------------
	CXTPControlGallery();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlGallery object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPControlGallery();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set items for the gallery.
	// Parameters:
	//     pItems - Pointer to items to be set.
	//-----------------------------------------------------------------------
	void SetItems(CXTPControlGalleryItems* pItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set margins around items.
	// Parameters:
	//     nLeft   - Left margin to be set.
	//     nTop    - Top margin to be set.
	//     nRight  - nRight margin to be set.
	//     nBottom - Bottom margin to be set.
	//-----------------------------------------------------------------------
	void SetItemsMargin(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the size of the control.
	// Parameters:
	//     szControl - New control size to be set.
	//-----------------------------------------------------------------------
	void SetControlSize(CSize szControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to the set size of the control depending on the
	//     number of rows, columns, current item size and current DPI.
	// Parameters:
	//     nRownColumns     - Number of columns.
	//     nRows            - Number of rows.
	//     szItem           - Default item size.
	//-----------------------------------------------------------------------
	void SetControlSize(int nColumns, int nRows, CSize szItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the count of items in the gallery.
	// Returns:
	//     Count of items in the gallery.
	// See Also:
	//     SetItems, GetItem
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve an item by its index.
	// Parameters:
	//     nIndex - Index of the item to be retrieved.
	// Returns:
	//     If successful, item at the specified index. Otherwise, NULL.
	// See Also:
	//     SetItems, GetItemCount
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get items associated with the gallery.
	// See Also:
	//     SetItems
	//-----------------------------------------------------------------------
	virtual CXTPControlGalleryItems* GetItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the selected item in the gallery.
	// Parameters:
	//     nSelected - Item to set as selected.
	// See Also:
	//     GetSelectedItem
	//-----------------------------------------------------------------------
	void SetSelectedItem(int nSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the selected item in the gallery.
	// Returns:
	//     The selected item in the gallery.
	// See Also:
	//     SetSelectedItem
	//-----------------------------------------------------------------------
	int GetSelectedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the checked item identifier for the gallery.
	// Parameters:
	//     nId - Item ID.
	// See Also:
	//     GetCheckedItem
	//-----------------------------------------------------------------------
	void SetCheckedItem(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if an item is checked.
	// Parameters:
	//     pItem - Item to test.
	// Returns:
	//     TRUE if the specified item is checked, otherwise FALSE.
	// See Also:
	//     SetCheckedItem
	//-----------------------------------------------------------------------
	virtual BOOL IsItemChecked(CXTPControlGalleryItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the checked item identifier for the gallery.
	// Returns:
	//     Checked item's identifier.
	// See Also:
	//     SetCheckedItem
	//-----------------------------------------------------------------------
	int GetCheckedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if any item in the gallery is selected.
	// Returns:
	//     TRUE if any item is selected, otherwise FALSE.
	// See Also:
	//     GetSelectedItem
	//-----------------------------------------------------------------------
	BOOL IsItemSelected() const;

public:
	static BOOL m_bAutoScaleDpi; // TRUE by default. Enables scaling of ribbon gallery items
								 // (both inline and displayed in pop-up).

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide borders around the gallery.
	// Parameters:
	//     bShowBorders - TRUE to show borders around the gallery.
	// See Also:
	//     IsShowBorders
	//-----------------------------------------------------------------------
	void ShowBorders(BOOL bShowBorders);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if borders are visible around the gallery.
	// Returns:
	//     TRUE if borders are visible, otherwise FALSE.
	// See Also:
	//     ShowBorders
	//-----------------------------------------------------------------------
	BOOL IsShowBorders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide labels for the gallery.
	// Parameters:
	//     bShowLabels - TRUE to show labels for the gallery.
	//-----------------------------------------------------------------------
	void ShowLabels(BOOL bShowLabels);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide the scrollbar for the gallery.
	// Parameters:
	//     bShowScrollBar - TRUE to show the scrollbar for the gallery.
	// See Also:
	//     IsScrollBarVisible
	//-----------------------------------------------------------------------
	void ShowScrollBar(BOOL bShowScrollBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the scrollbar for the gallery is visible.
	// Returns:
	//     TRUE if the scrollbar is visible, otherwise FALSE.
	// See Also:
	//     ShowScrollBar
	//-----------------------------------------------------------------------
	BOOL IsScrollBarVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to automatically show/hide the scrollbar for the gallery.
	// Parameters:
	//     bShowScrollBarAuto - TRUE to show the scrollbar automatically.
	//-----------------------------------------------------------------------
	void SetScrollBarAuto(BOOL bScrollBarAuto);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the scrollbar for the gallery is automatically shown/hidden.
	// Returns:
	//     TRUE if the scrollbar is automatically shown.
	//-----------------------------------------------------------------------
	BOOL IsScrollBarAuto() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the border margins for the gallery.
	// Returns:
	//     A CRect object containing the border margins for the gallery.
	//-----------------------------------------------------------------------
	virtual CRect GetBorders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the bounding rectangle of the item.
	// Parameters:
	//     nIndex - Item index.
	// Returns:
	//     Bounding rectangle of the item.
	//-----------------------------------------------------------------------
	CRect GetItemDrawRect(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     point - Specifies the point to be tested.
	//     lpRect - Retrieves rectangle where item is located. Can be NULL.
	// Returns:
	//     An index of CXTPControlGalleryItem that occupies the specified
	//     point or NULL if no control occupies the point.
	//-----------------------------------------------------------------------
	int HitTestItem(CPoint point, LPRECT lpRect = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the bounding rectangle of the items.
	// Returns:
	//     Bounding rectangle of the items.
	//-----------------------------------------------------------------------
	CRect GetItemsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw items.
	// Parameters:
	//     lpRect - Rectangle to redraw.
	//     bAnimate - TRUE to animate changes.
	//-----------------------------------------------------------------------
	void InvalidateItems(LPCRECT lpRect = NULL, BOOL bAnimate = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CXTPControlGalleryPaintManager pointer to draw gallery.
	// Returns:
	//     Pointer to a CXTPControlGalleryPaintManager object.
	//-----------------------------------------------------------------------
	CXTPControlGalleryPaintManager* GetGalleryPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CXTPScrollBarPaintManager pointer to draw ScrollBar.
	// Returns:
	//     Pointer to a CXTPScrollBarPaintManager object.
	//-----------------------------------------------------------------------
	CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to ensure that a specified item is visible in the gallery.
	//     This will scroll the gallery until the specified item is visible.
	// Parameters:
	//     nIndex - Index of the item whose visibility must be ensured.
	//-----------------------------------------------------------------------
	void EnsureVisible(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the scrollbar position for the gallery.
	// Parameters:
	//     nScrollPos - New scrollbar position to be set.
	// See Also:
	//     GetScrollPos
	//-----------------------------------------------------------------------
	void SetScrollPos(int nScrollPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the scrollbar position for the gallery.
	// Returns:
	//     The scrollbar position.
	// See Also:
	//     SetScrollPos
	//-----------------------------------------------------------------------
	int GetScrollPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to scroll gallery to the specified top item.
	// Parameters:
	//     nIndex - Item to be scrolled to.
	// Returns:
	//     If successful, a value of 0 will be returned.
	//     If an error occurs, a value of -1 will be returned.
	// See Also:
	//     SetScrollPos
	//-----------------------------------------------------------------------
	int SetTopIndex(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow the user to resize the gallery.
	// Parameters:
	//     bResizeWidth - Allow change width.
	//     bResizeHeight - Allow change height.
	// See Also:
	//     IsResizable
	//-----------------------------------------------------------------------
	void SetResizable(BOOL bResizeWidth = TRUE, BOOL bResizeHeight = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the gallery can be resized by the user.
	// Returns:
	//     TRUE if the gallery can be resized by the user, otherwise FALSE.
	// See Also:
	//     SetResizable
	//-----------------------------------------------------------------------
	BOOL IsResizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the selection is changed by the user.
	//-----------------------------------------------------------------------
	virtual void OnSelectedItemChanged();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check if the gallery has a bottom separator.
	// Returns:
	//     TRUE if the gallery has a bottom separator, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasBottomSeparator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the next focusable item.
	// Parameters:
	//     nIndex - Index to start.
	//     nDirection - Direction to find (+1 or -1).
	// Returns:
	//     Index of the next focusable item.
	//-----------------------------------------------------------------------
	long GetNext(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the next focusable item in the same row.
	// Parameters:
	//     nIndex - Index to start.
	//     nDirection - Direction to find (+1 or -1).
	// Returns:
	//     Index of the next focusable item in the same row.
	//-----------------------------------------------------------------------
	long GetNextInRow(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the next focusable item in the same column.
	// Parameters:
	//     nIndex - Index to start.
	//     nDirection - Direction to find (+1 or -1).
	// Returns:
	//     Index of the next focusable item in the same column.
	//-----------------------------------------------------------------------
	long GetNextInColumn(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the next focusable item in the same page.
	// Parameters:
	//     nIndex - Index to start.
	//     nDirection - Direction to find (+1 or -1).
	// Returns:
	//     Index of the next focusable item in the same page.
	//-----------------------------------------------------------------------
	long GetNextInPage(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to search for an item.
	// Parameters:
	//     nStartAfter - Index to begin search.
	//     lpszItem - Item caption to find.
	//     bExact - TRUE to find whole caption or only part.
	// Returns:
	//     If successful, the index of the search item will be returned.
	//     If unsuccessful, a value of -1 will be returned.
	//-----------------------------------------------------------------------
	int FindItem(int nStartAfter, LPCTSTR lpszItem, BOOL bExact) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the caption of an item.
	// Parameters:
	//     nIndex - Index of the item.
	// Returns:
	//     Caption of the item.
	//-----------------------------------------------------------------------
	CString GetItemCaption(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the scrollbar button is enabled.
	// Parameters:
	//     ht - HitTest of scrollbar button.
	// Returns:
	//     TRUE if the scrollbar button is enabled.
	//-----------------------------------------------------------------------
	BOOL IsScrollButtonEnabled(int ht);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the scrollbar is enabled.
	// Returns:
	//     TRUE if the scrollbar is enabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsScrollBarEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the gallery is shown as a button.
	// Returns:
	//     TRUE if the gallery is shown as a button.
	//-----------------------------------------------------------------------
	BOOL IsShownAsButton() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(IsShownAsButton)
	BOOL IsShowAsButton() const
	{
		return IsShownAsButton();
	}
	//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to hide the selection.
	//-------------------------------------------------------------------------
	void HideSelection();

	void SetTransparent(BOOL bTransparent = TRUE);
	BOOL IsTransparent() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to set the position of all items.
	//-------------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called to draw all items.
	// Parameters:
	//      pDC - Pointer to a device context to draw.
	//-----------------------------------------------------------------------
	void DrawItems(CDC* pDC);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Pointer to a source CXTPControl object.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command
	//     bar size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar.
	//-----------------------------------------------------------------------
	void OnCalcDynamicSize(DWORD dwMode);

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual void SetRect(CRect rcControl);
	virtual void OnItemsChanged();
	BOOL OnHookMouseWheel(UINT nFlags, short zDelta, CPoint pt);

protected:
	virtual CSize GetSize(CDC* pDC);
	virtual void Draw(CDC* pDC);

	virtual void OnMouseMove(CPoint point);

	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));
	virtual void OnLButtonUp(CPoint point);
	virtual BOOL OnSetSelected(int bSelected);
	void DoScroll(int cmd, int pos);
	void GetScrollInfo(SCROLLINFO* pSI);
	void PerformMouseMove(CPoint point);
	void PerformMouseDown(CPoint point);
	void PerformMouseUp(CPoint point);
	virtual void FillControl(CDC* pDC, CRect rcControl);
	virtual void SetAction(CXTPControlAction* pAction);
	virtual int GetCustomizeMinWidth() const;
	virtual int GetCustomizeMinHeight() const;
	virtual BOOL IsCustomizeResizeAllow() const;

protected:
	void DrawScrollBar(CDC* pDC);
	CRect GetScrollBarRect();
	void CalcScrollBarInfo(LPRECT lprc, XTP_SCROLLBAR_POSINFO* pSBInfo, SCROLLINFO* pSI);

	BOOL IsFocused() const;
	BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);
	virtual void OnScrollChanged();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	void OnExecute();
	BOOL OnSetPopup(BOOL bPopup);
	void OnMouseHover();

protected:
	CWnd* GetParentWindow() const;
	virtual void AdjustExcludeRect(CRect& rc, BOOL /*bVertical*/);
	virtual void RedrawScrollBar();
	void DoAnimate();
	void StartAnimation(int nScrollPos);
	void RepositionCore();

	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual HRESULT GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
	virtual HRESULT AccessibleDoDefaultAction(VARIANT varChild);
	virtual HRESULT AccessibleSelect(long flagsSelect, VARIANT varChild);
	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlGallery);

	void OleSetItems(LPDISPATCH lpDispatch);
	LPDISPATCH OleGetItems();

	DECLARE_PROPERTY(SelectedItem, LPDISPATCH);
	DECLARE_PROPERTY(CheckedItem, int);

//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	int m_nChecked;			 // Checked ID.
	int m_nSelected;		 // Index of selected item.
	BOOL m_bHideSelection;   // TRUE to hide selection.
	BOOL m_bPressed;		 // TRUE if item is pressed.
	CPoint m_ptPressed;		 // Position of pressed item.
	int m_nScrollPos;		 // Scroll position.
	int m_nScrollPosTarget;  // Scroll position.
	double m_dAnimationStep; // Number of steps to complete animation.

	BOOL m_bKeyboardSelected; // TRUE if item was selected using keyboard.

	int m_nTotalHeight; // Total height of items.

	BOOL m_bShowLabels; // TRUE to show labels.

	BOOL m_bShowScrollBar; // TRUE to show scrollbars.
	BOOL m_bShowBorders;   // TRUE to show borders.
	CRect m_rcMargin;	  // Margins of gallery items.
	BOOL m_bPreview;	   // TRUE if control is in preview mode.
	int m_nResizable;	  // TRUE if the pop-up galley is resizable, see XTPGalleryResizable.

	BOOL m_bScrollBarAuto;
	BOOL m_bTransparent;

	CArray<GALLERYITEM_POSITION, GALLERYITEM_POSITION&> m_arrRects; // bounding rectangles

	friend class CXTPControlGalleryItems;
};

#	define GETPARTSTATE3(ht, bEnabled)                                                            \
		(!bEnabled ? 4 : nPressedHt == ht ? 3 : nHotHt == ht ? 2 : 0)
#	define IS_PART_SELECTED(nState) (nState == 2)
#	define IS_PART_PRESSED(nState) (nState == 3)
#	define IS_PART_ENABLED(nState) (nState != 4)

//===========================================================================
// Summary:
//     CXTPControlGalleryPaintManager is a standalone class used to draw
//     a CXTPControlGallery object.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryPaintManager : public CObject
{
	DECLARE_DYNAMIC(CXTPControlGalleryPaintManager);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryPaintManager object.
	// Parameters:
	//     pPaintManager - PaintManager of command bars.
	//-----------------------------------------------------------------------
	CXTPControlGalleryPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Handles object destruction
	//-----------------------------------------------------------------------
	virtual ~CXTPControlGalleryPaintManager()
	{
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a label control.
	// Parameters:
	//     pDC - Pointer to a device context.
	//     pLabel - Label item.
	//     rc - Bounding rectangle of the label.
	//-----------------------------------------------------------------------
	virtual void DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background of the gallery.
	// Parameters:
	//      pDC - Pointer to a device context.
	//      rc - Bounding rectangle of gallery object.
	//      pGallery - CXTPControlGallery object to draw.
	//-----------------------------------------------------------------------
	virtual void FillControl(CDC* pDC, CXTPControlGallery* pGallery, CRect rc);

	virtual void DrawItem(CDC* pDC, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed,
						  BOOL bChecked, CXTPControlGalleryItem* pGalleryItem,
						  CXTPControlGallery* pGallery);

	virtual void DrawItemBack(CDC* pDC, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed,
							  BOOL bChecked, XTPBarType barType);

	virtual COLORREF GetItemTextColor(BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked,
									  XTPBarType barType);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the scrollbar for an in-place gallery.
	// Parameters:
	//     pDC - Pointer to device context.
	//     pGallery - CXTPControlGallery object to draw.
	// See Also:
	//     DrawScrollBar
	//-----------------------------------------------------------------------
	virtual void DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery);

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates gallery metrics.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	int m_cyPopupUp;   // Height of up arrow of in-place scrollbar.
	int m_cyPopupDown; // Height of down arrow of in-place scrollbar.
	int m_cxPopup;	 // Width of arrows of in-place scrollbar.

	COLORREF m_clrBorder;

	CXTPPaintManager* m_pPaintManager;
};

AFX_INLINE BOOL CXTPControlGallery::IsShowBorders() const
{
	return m_bShowBorders;
}
AFX_INLINE void CXTPControlGallery::ShowLabels(BOOL bShowLabels)
{
	m_bShowLabels = bShowLabels;
}
AFX_INLINE void CXTPControlGallery::ShowScrollBar(BOOL bShowScrollBar)
{
	m_bShowScrollBar = bShowScrollBar;
}
AFX_INLINE void CXTPControlGallery::ShowBorders(BOOL bShowBorders)
{
	m_bShowBorders = bShowBorders;
}
AFX_INLINE BOOL CXTPControlGallery::IsItemSelected() const
{
	return !m_bHideSelection && GetSelectedItem() != -1;
}
AFX_INLINE BOOL CXTPControlGallery::IsResizable() const
{
	return !IsShownAsButton() && GetItems() ? m_nResizable : FALSE;
}
AFX_INLINE BOOL CXTPControlGallery::IsCustomizeResizeAllow() const
{
	return IsResizable();
}
AFX_INLINE void CXTPControlGallery::SetScrollBarAuto(BOOL bScrollBarAuto)
{
	m_bScrollBarAuto = bScrollBarAuto;
}
AFX_INLINE BOOL CXTPControlGallery::IsScrollBarVisible() const
{
	return m_bShowScrollBar;
}
AFX_INLINE BOOL CXTPControlGallery::IsScrollBarAuto() const
{
	return m_bScrollBarAuto;
}
AFX_INLINE void CXTPControlGallery::SetTransparent(BOOL bTransparent)
{
	m_bTransparent = bTransparent;
}
AFX_INLINE BOOL CXTPControlGallery::IsTransparent() const
{
	return m_bTransparent;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPCONTROLGALLERY_H__)
