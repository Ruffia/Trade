// XTPTrackControlItem.h: interface for the CXTPTrackControlItem class.
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
#if !defined(__XTPTRACKCONTROLITEM_H__)
#	define __XTPTRACKCONTROLITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTrackControl;
class CXTPTrackBlock;
typedef CArray<CXTPTrackBlock*, CXTPTrackBlock*> CXTPTrackBlockArray;

//===========================================================================
// Summary:
//     This class represents a container cell for a set of blocks and keys
//     and inherits basic functionality from the CXTPGridRecordItem class.
// See Also:
//     CXTPGridRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackControlItem : public CXTPGridRecordItem
{
	DECLARE_SERIAL(CXTPTrackControlItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPTrackControlItem item.
	//-----------------------------------------------------------------------
	CXTPTrackControlItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackControlItem default destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackControlItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the text caption of the current item.
	// Parameters:
	//     pColumn - Pointer to the corresponding column of the current item.
	// Remarks:
	//     Overwrites parents' member function.
	// Returns:
	//     The text caption of the current item.
	//-----------------------------------------------------------------------
	CString GetCaption(CXTPGridColumn* pColumn);

	using CXTPGridRecordItem::GetCaption;

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	// Remarks:
	//     This function simply prepares and calls the necessary drawing
	//     functions from PaintManager to draw a grid item. As such,
	//     you must provide your own implementation of PaintManager in
	//     order to change the appearance of your grid item.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	// Example:
	// <code>
	// // fill structure
	// XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
	// drawArgs.pDC = pDC;
	// drawArgs.pControl = m_pControl;
	// drawArgs.pRow = this;
	// // call function
	// Draw(&drawArgs);
	// </code>
	//
	// See Also:
	//     XTP_GRIDRECORDITEM_DRAWARGS
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which block, if any, is at a specified point.
	// Parameters:
	//     ptPoint - Point to be tested.
	// Returns:
	//     A pointer to the CXTPTrackBlock object at the
	//     specified point, if any, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* HitTest(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a block.
	// Parameters:
	//     pBlock - Pointer to the block to be removed.
	// Returns:
	//     TRUE if the removal is successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Remove(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a block.
	// Parameters:
	//     nIndex - Zero-based index of the block to be removed.
	// Returns:
	//     TRUE if the removal is successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all blocks.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a block to the track.
	// Parameters:
	//     pBlock - Pointer to track block to be added.
	// Returns:
	//     A pointer to the newly added block.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* Add(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the track control associated with this item.
	// Returns:
	//     A pointer to the track control associated with this item.
	//-----------------------------------------------------------------------
	CXTPTrackControl* GetTrackControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified block.
	// Parameters:
	//     nIndex - Index of the block to retrieve.
	// Returns:
	//     A pointer to a CXTPTrackBlock object.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* GetBlock(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the number of blocks in the track control.
	// Returns:
	//     The number of blocks in the track control.
	//-----------------------------------------------------------------------
	int GetBlockCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the locked state of the track control.
	// Returns:
	//     TRUE if the track control is locked, otherwise FALSE.
	// Remarks:
	//     If a track is locked, then it cannot be moved and none of its
	//     blocks or keys can be moved. When locked, blocks and keys
	//     from other tracks cannot be dragged into this track.
	//-----------------------------------------------------------------------
	BOOL IsLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the locked state of the track control.
	// Parameters:
	//     bLocked - TRUE to lock the track control, FALSE to unlock.
	// Remarks:
	//     If a track is locked, then it cannot be moved and none of its
	//     blocks or keys can be moved. When locked, blocks and keys
	//     from other tracks cannot be dragged into this track.
	//-----------------------------------------------------------------------
	void SetLocked(BOOL bLocked = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Returns:
	//     TRUE if the implementation handles the mouse click event and no
	//     further handling is required, otherwise FALSE.
	// Remarks:
	//     Usually this function is called by GridRow's OnLButtonDown function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//  virtual BOOL OnRButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Returns:
	//     TRUE if the implementation handles the mouse click event and no
	//     further handling is required, otherwise FALSE.
	// Remarks:
	//     Usually this function is called by GridRow's OnLButtonUp function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Remarks:
	//     Usually this function is called by GridRow's OnClick function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes double mouse clicks.
	// Parameters:
	//     pClickArgs - Pointer to an XTP_GRIDRECORDITEM_CLICKARGS
	//                  structure containing click arguments.
	// Remarks:
	//     Usually this function is called by GridRow's OnDblClick function.
	// See Also:
	//     XTP_GRIDRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnDblClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     point    - Specifies the x- and y- coordinates of the cursor.
	//     nFlags   - Additional flags.
	//     pControl - Pointer to the Grid control.
	// Remarks:
	//     Usually this function is called by GridRow's OnMouseMove function.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point, CXTPGridControl* pControl = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a grid record item using
	//     the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to perform a hit test on the track control
	//     at a specified point and fill a TOOLINFO structure
	//     with the contextual display string.
	// Parameters:
	//     point - Point to test.
	//     pTI   - Pointer to a TOOLINFO structure.
	// Returns:
	//     An INT_PTR value pointing to the track control item where the
	//     hit test was performed.
	//-----------------------------------------------------------------------
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to recalculate the size of the track items
	//     when the layout is changed.
	//-----------------------------------------------------------------------
	void RecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the block position when a block is resized/moved.
	// Parameters:
	//     pBlock  - Pointer to the track block that is being resized/moved.
	//     bResize - TRUE if the block is being resized,
	//               FALSE if the block is being moved.
	//-----------------------------------------------------------------------
	void AdjustBlockPosition(CXTPTrackBlock* pBlock, BOOL bResize = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the track control.
	//-----------------------------------------------------------------------
	void RedrawControl();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Highlights the work area.
	// Parameters:
	//     pDC    - Pointer to the device context.
	//     rcItem - Work area rectangle.
	//-----------------------------------------------------------------------
	virtual void HighlightWorkArea(CDC* pDC, CRect rcItem);

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	LPDISPATCH OleGetBlock(int nIndex);
	int OleGetBlockCount();
	LPDISPATCH OleAddBlock(BOOL bKey, int nPosition, int nLength);
	void OleRemoveBlock(int nIndex);

	BOOL OleGetLocked();
	void OleSetLocked(BOOL bLocked);

	DECLARE_OLETYPELIB_EX(CXTPTrackControlItem);
//}}AFX_CODEJOCK_PRIVATE
#	endif

public:
	BOOL m_bLocked; // TRUE if the whole track is locked.

protected:
	CXTPTrackBlockArray m_arrBlocks; // Blocks array.

protected:
	struct DOCK_INFO;
	class CDockInfoArray;

	friend class CXTPTrackControl;
};

AFX_INLINE CXTPTrackBlock* CXTPTrackControlItem::GetBlock(int nIndex) const
{
	return m_arrBlocks[nIndex];
}

AFX_INLINE int CXTPTrackControlItem::GetBlockCount() const
{
	return (int)m_arrBlocks.GetSize();
}

AFX_INLINE BOOL CXTPTrackControlItem::IsLocked() const
{
	return m_bLocked;
}

AFX_INLINE void CXTPTrackControlItem::SetLocked(BOOL bLocked)
{
	m_bLocked = bLocked;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPTRACKCONTROLITEM_H__)
