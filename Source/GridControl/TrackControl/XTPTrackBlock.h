// XTPTrackBlock.h: interface for the CXTPTrackBlock class.
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
#if !defined(__XTPTRACKBLOCK_H__)
#	define __XTPTRACKBLOCK_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTrackControlItem;

//===========================================================================
// Summary:
//     This class represents a track control block. Track blocks are a visual
//     representation of units of time in a track. They can have both a
//     minimum and maximum time period.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackBlock : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPTrackBlock);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTrackBlock object.
	//-----------------------------------------------------------------------
	CXTPTrackBlock();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the block.
	// Parameters:
	//     pDC       - Pointer to the device context.
	//     rc        - Bounds of the drawing area.
	//     bSelected - TRUE if the block is selected, FALSE otherwise.
	// Returns:
	//     A value of 1 if the drawing was successful, otherwise a value of 0.
	// Remarks:
	//     This is a virtual function so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual int Draw(CDC* pDC, CRect rc, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is a key.
	// Returns:
	//     TRUE if the item is a key, otherwise FALSE.
	// Remarks:
	//     This is a virtual function so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual BOOL IsKey() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the block is resizable.
	// Returns:
	//     TRUE if the block is resizable, otherwise FALSE.
	// Remarks:
	//     This is a virtual function so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual BOOL IsResizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the position for the block on the track.
	// Parameters:
	//     nPosition - Position for the block on the track.
	//-----------------------------------------------------------------------
	void SetPosition(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the block on the track.
	// Returns:
	//     The position of the block on the track.
	//-----------------------------------------------------------------------
	int GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the length for the block on the track.
	// Parameters:
	//     nLength - Length for the block on the track.
	//-----------------------------------------------------------------------
	void SetLength(int nLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length of the block on the track.
	// Returns:
	//     The length of the block on the track.
	//-----------------------------------------------------------------------
	int GetLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the color for the block.
	// Parameters:
	//     clr - COLORREF value specifying the color for the block.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the block.
	// Returns:
	//     A CRect object containing the bounding rectangle of the block.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the locked state of the block
	//     (i.e. a locked block cannot be moved or resized).
	// Returns:
	//     TRUE if the block is locked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the locked state for the block
	//     (i.e. a locked block cannot be moved or resized).
	// Parameters:
	//     bLocked - TRUE to lock the block;
	//               the default value for this parameter is TRUE.
	//-----------------------------------------------------------------------
	void SetLocked(BOOL bLocked = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the height percentage for the block compared to its track.
	// Parameters:
	//     dHeightPercent - Double value denoting the height percentage.
	// Remarks:
	//     Blocks can be set to different heights and different vertical
	//     alignments within a track. This is used as a visual aid to make
	//     different blocks in the track visually different from each other.
	//-----------------------------------------------------------------------
	void SetHeightPercent(double dHeightPercent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height percentage of the block compared to its track.
	// Returns:
	//     A double value denoting the height percentage.
	//-----------------------------------------------------------------------
	double GetHeightPercent() const;

	void SetHeightFixed(int nHeightPercent);
	int GetHeightFixed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the vertical alignment for the block in its track.
	// Parameters:
	//     nAlignment - Double value denoting the vertical alignment.
	// Remarks:
	//     Vertical alignment is used for keys and blocks which do not share
	//     the same height as their track. Vertical alignment aligns blocks
	//     in relation to the center of the track.
	//-----------------------------------------------------------------------
	void SetVerticalAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the vertical alignment of the block in its track.
	// Returns:
	//     A double value denoting the vertical alignment.
	// Remarks:
	//     Vertical alignment is used for keys and blocks which do not share
	//     the same height as their track. Vertical alignment aligns blocks
	//     in relation to the center of the track.
	//-----------------------------------------------------------------------
	int GetVerticalAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the track control to which the block is associated with.
	// Returns:
	//     A pointer to the CXTPTrackControl object to which the block
	//     is associated with.
	// Remarks:
	//     Tracks are what hold blocks and keys.
	//-----------------------------------------------------------------------
	CXTPTrackControlItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the block in its track.
	// Returns:
	//     The index of the block in its track.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected state for the block.
	// Parameters:
	//     bSelect - TRUE to select the block, FALSE to unselect;
	//               the default value for this parameter is TRUE.
	//-----------------------------------------------------------------------
	void Select(BOOL bSelect = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the selected state of the block.
	// Returns:
	//     TRUE if the block is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the block.
	// Returns:
	//     The caption of the block.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the block.
	// Parameters:
	//     lpszCaption - New caption for the block.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text of the block.
	// Returns:
	//     The tooltip text of the block.
	//-----------------------------------------------------------------------
	virtual CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text for the block.
	// Parameters:
	//     lpszTooltip - New tooltip text for the block.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the description text for the block.
	// Parameters:
	//     lpszDescriptionText - New description text for the block.
	//-----------------------------------------------------------------------
	void SetDescriptionText(LPCTSTR lpszDescriptionText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the description text of the block.
	// Returns:
	//     The description text of the block.
	//-----------------------------------------------------------------------
	CString GetDescriptionText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the minimum length for the block.
	// Parameters:
	//     nMinLength - Minimum length for the block.
	//-----------------------------------------------------------------------
	void SetMinLength(int nMinLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum length of the block.
	// Returns:
	//     The minimum length of the block.
	//-----------------------------------------------------------------------
	int GetMinLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the maximum length for the block.
	// Parameters:
	//     nMaxLength - Maximum length for the block.
	//-----------------------------------------------------------------------
	void SetMaxLength(int nMaxLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum length of the block.
	// Returns:
	//     The maximum length of the block.
	//-----------------------------------------------------------------------
	int GetMaxLength() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load the properties related
	//     to the block.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load property data
	//     to/from storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the block.
	//-----------------------------------------------------------------------
	void Remove();

public:
	CRect m_rcBlock;			   // Drawing bounding rectangle of block.
	CXTPTrackControlItem* m_pItem; // Pointer to the parent track of the block.
	int m_nPosition;			   // Position that resolve overlaps with items on same track.
	int m_nLength;				   // Length of the block.
	BOOL m_bLocked;				   // TRUE if the block is not movable/resizable.
	COLORREF m_clrBlock;		   // Color of the block.
	CString m_strCaption;		   // Caption text.
	double m_dHeightPercent;	   // Height percentage of the block compared to its track.
	int m_nHeightFixed;
	int m_nVerticalAlignment; // Vertical alignment of blocks with m_dHeightPercent < 1.

protected:
	int m_nMRUPosition;		  // Last position that was set by the user.
	CString m_strToolTip;	 // Tooltip text.
	CString m_strDescription; // Description text.
	int m_nIndex;			  // Index of the block in its parent track.
	int m_nLastDragTime;	  // Last drag moment of user (used as priority for flexible draging).
	int m_nMinLength;		  // Minimum resize length.
	int m_nMaxLength;		  // Maximum resize length.
	BOOL m_bDragging;		  // Block is currently dragging.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackBlock);

	int OleGetPosition();
	void OleSetPosition(int nPosition);
	int OleGetLength();
	void OleSetLength(int nLength);
	BOOL OleGetLocked();
	void OleSetLocked(BOOL bLocked);
	COLORREF OleGetColor();
	void OleSetColor(OLE_COLOR clr);
	BSTR OleGetTooltip();
	void OleSetTooltip(LPCTSTR lpszTooltip);
	BSTR OleGetDescriptionText();
	void OleSetDescriptionText(LPCTSTR lpszDescription);
	double OleGetHeightPercent();
	void OleSetHeightPercent(double dHeightPercent);
	int OleGetHeightFixed();
	int OleGetVerticalAlignment();
	void OleSetHeightFixed(int dHeightFixed);
	void OleSetVerticalAlignment(int nVerticalAlignment);
	int OleGetMinLength();
	void OleSetMinLength(int nMinLength);
	int OleGetMaxLength();
	void OleSetMaxLength(int nMaxLength);
	LPDISPATCH OleGetItem();
	int OleGetIndex();
	BOOL OleGetIsKey();
	BSTR OleGetCaption();
	void OleSetCaption(LPCTSTR lpszCaption);
//}}AFX_CODEJOCK_PRIVATE
#	endif
private:
	friend class CXTPTrackControlItem;
	friend class CXTPTrackControl;
};

//===========================================================================
// Summary:
//     This class represents a track control key. Keys are used simply as
//     "notes" or reference points in the track. They can be added anywhere
//     in the track to make note of some event at a given moment in time.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackKey : public CXTPTrackBlock
{
	DECLARE_SERIAL(CXTPTrackKey);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTrackKey object.
	//-----------------------------------------------------------------------
	CXTPTrackKey();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the key.
	// Parameters:
	//     pDC       - Pointer to the device context.
	//     rc        - Bounds of the drawing area.
	//     bSelected - TRUE if the block is selected, FALSE otherwise.
	// Returns:
	//     A value of 1 if the drawing was successful, otherwise a value of 0.
	// Remarks:
	//     This is a virtual function so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual int Draw(CDC* pDC, CRect rc, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is a key.
	// Returns:
	//     TRUE if the item is a key, otherwise FALSE.
	// Remarks:
	//     This is a virtual function so derived classes can provide their
	//     specific implementations.
	//-----------------------------------------------------------------------
	virtual BOOL IsKey() const;
};

//===========================================================================
// Summary:
//     This class represents a track control marker. Markers are used for block
//     alignment, blocks can be "snapped" to the left or right of these markers.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackMarker : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPTrackMarker);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTrackMarker object.
	//-----------------------------------------------------------------------
	CXTPTrackMarker();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the position of the marker.
	// Parameters:
	//     nPosition - Position of the marker on the time line.
	//-----------------------------------------------------------------------
	void SetPosition(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the marker.
	// Returns:
	//     The position of the marker on the time line.
	//-----------------------------------------------------------------------
	int GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption for the marker.
	// Parameters:
	//     lpszCaption - New caption for the marker.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the marker.
	// Returns:
	//     The caption of the marker.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the marker.
	// Returns:
	//     A CRect object containing the bounding rectangle of the marker.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the marker.
	//-----------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the track control to which the marker is associated with.
	// Returns:
	//     A pointer to the CXTPTrackControl object to which the marker
	//     is associated with.
	//-----------------------------------------------------------------------
	CXTPTrackControl* GetControl() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load the properties related
	//     to the marker.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load property data
	//     to/from storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CString m_strCaption;		  // Caption of the marker
	int m_nPosition;			  // Position of the marker.
	CRect m_rcMarker;			  // Drawing position of the marker.
	CXTPTrackControl* m_pControl; // Parent control.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackMarker);
//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	friend class CXTPTrackPaintManager;
	friend class CXTPTrackMarkers;
};

//===========================================================================
// Summary:
//     This class abstracts the track control markers. Markers are used for block
//     alignment, blocks can be "snapped" to the left or right of these markers.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackMarkers : public CXTPCmdTarget
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTrackMarkers object.
	// Parameters:
	//     pControl - Pointer to parent Track Control object
	//-----------------------------------------------------------------------
	CXTPTrackMarkers(CXTPTrackControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the CXTPTrackMarkers object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPTrackMarkers();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates and adds a marker to the marker collection.
	// Parameters:
	//     nPosition   - Position of the marker.
	//     lpszCaption - Caption of the marker.
	// Returns:
	//     A pointer to the newly created CXTPTrackMarker object.
	//-----------------------------------------------------------------------
	CXTPTrackMarker* Add(int nPosition, LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a marker to the marker collection.
	// Parameters:
	//     pMarker - Pointer to the marker to add.
	// Returns:
	//     A pointer to the added CXTPTrackMarker object.
	//-----------------------------------------------------------------------
	CXTPTrackMarker* Add(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the marker at a specified index in the marker collection.
	// Parameters:
	//     nIndex - Zero-based index of the marker to retrieve.
	// Returns:
	//     A pointer to the CXTPTrackMarker at the specified index if successful,
	//     NULL if the specified index is not valid in the current context.
	//-----------------------------------------------------------------------
	CXTPTrackMarker* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the marker at a specified index from the marker collection.
	// Parameters:
	//     nIndex - Zero-based index of the marker to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified marker from the marker collection.
	// Parameters:
	//     pMarker - Pointer to the marker to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all markers from the marker collection.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of a marker in the marker collection.
	// Parameters:
	//     pMarker - Pointer to the marker whose index must be retrieved.
	// Returns:
	//     The zero-based index of the marker in the marker collection.
	//-----------------------------------------------------------------------
	int IndexOf(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of markers in the marker collection.
	// Returns:
	//     The number of markers in the marker collection.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the index of the marker, if any,
	//     at a specified point.
	// Parameters:
	//     pt - Point to test.
	// Returns:
	//     The zero-based index of the marker at the specified point, if any,
	//     otherwise a value of -1.
	//-----------------------------------------------------------------------
	int HitTest(CPoint pt) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load the properties related
	//     to the marker collection.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load property data
	//     to/from storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CXTPTrackControl* m_pControl;							 // Parent control.
	CArray<CXTPTrackMarker*, CXTPTrackMarker*> m_arrMarkers; // Array of markers.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackMarkers);
	DECLARE_ENUM_VARIANT(CXTPTrackMarkers)

	LPDISPATCH OleAdd(int Position, LPCTSTR lpszCaption);
	LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();
//}}AFX_CODEJOCK_PRIVATE
#	endif

private:
	friend class CXTPTrackControl;
};

//===========================================================================
// Summary:
//     This class abstracts the selected blocks of the track control. This class
//     helps to perform a common task to the selected blocks (e.g. delete).
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackSelectedBlocks : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackSelectedBlocks object.
	//-----------------------------------------------------------------------
	CXTPTrackSelectedBlocks();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of blocks in selected blocks.
	// Returns:
	//     The number of blocks in selected blocks.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a block to selected blocks.
	// Parameters:
	//     pBlock   - Pointer to the block to add.
	//-----------------------------------------------------------------------
	void Add(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a block is selected.
	// Parameters:
	//     pBlock - Pointer to the block to check.
	// Returns:
	//     TRUE if the block is selected, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected(const CXTPTrackBlock* pBlock) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the block at a specified index in selected blocks.
	// Parameters:
	//     nIndex - Zero-based index of the block to retrieve.
	// Returns:
	//     A pointer to the CXTPTrackBlock at the specified index.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all blocks from selected blocks.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the block at a specified index from selected blocks.
	// Parameters:
	//     nIndex - Zero-based index of the block to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified block from selected blocks.
	// Parameters:
	//     pBlock - Pointer to the block to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPTrackBlock* pBlock);

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackSelectedBlocks);
	DECLARE_ENUM_VARIANT(CXTPTrackSelectedBlocks)

	LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();
//}}AFX_CODEJOCK_PRIVATE
#	endif
protected:
	CArray<CXTPTrackBlock*, CXTPTrackBlock*> m_arrBlocks;
};

AFX_INLINE BOOL CXTPTrackBlock::IsKey() const
{
	return FALSE;
}
AFX_INLINE void CXTPTrackBlock::SetColor(COLORREF clr)
{
	m_clrBlock = clr;
}
AFX_INLINE CRect CXTPTrackBlock::GetRect() const
{
	return m_rcBlock;
}
AFX_INLINE int CXTPTrackBlock::GetPosition() const
{
	return m_nPosition;
}
AFX_INLINE int CXTPTrackBlock::GetLength() const
{
	return m_nLength;
}
AFX_INLINE CString CXTPTrackBlock::GetTooltip() const
{
	return m_strToolTip;
}
AFX_INLINE void CXTPTrackBlock::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strToolTip = lpszTooltip;
}
AFX_INLINE CString CXTPTrackBlock::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE void CXTPTrackBlock::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
}
AFX_INLINE void CXTPTrackBlock::SetDescriptionText(LPCTSTR lpszDescriptionText)
{
	m_strDescription = lpszDescriptionText;
}
AFX_INLINE CString CXTPTrackBlock::GetDescriptionText() const
{
	return m_strDescription;
}
AFX_INLINE void CXTPTrackBlock::SetLocked(BOOL bLocked)
{
	m_bLocked = bLocked;
}
AFX_INLINE void CXTPTrackBlock::SetHeightPercent(double dHeightPercent)
{
	m_dHeightPercent = dHeightPercent;
}
AFX_INLINE double CXTPTrackBlock::GetHeightPercent() const
{
	return m_dHeightPercent;
}
AFX_INLINE void CXTPTrackBlock::SetHeightFixed(int nHeightFixed)
{
	m_nHeightFixed = nHeightFixed;
}
AFX_INLINE int CXTPTrackBlock::GetHeightFixed() const
{
	return m_nHeightFixed;
}
AFX_INLINE void CXTPTrackBlock::SetVerticalAlignment(int nAlignment)
{
	m_nVerticalAlignment = nAlignment;
}
AFX_INLINE int CXTPTrackBlock::GetVerticalAlignment() const
{
	return m_nVerticalAlignment;
}
AFX_INLINE CXTPTrackControlItem* CXTPTrackBlock::GetItem() const
{
	return m_pItem;
}
AFX_INLINE int CXTPTrackBlock::GetIndex() const
{
	return m_nIndex;
}
AFX_INLINE BOOL CXTPTrackBlock::IsResizable() const
{
	return !IsKey();
}
AFX_INLINE void CXTPTrackBlock::SetMinLength(int nMinLength)
{
	m_nMinLength = nMinLength;
}
AFX_INLINE int CXTPTrackBlock::GetMinLength() const
{
	return m_nMinLength;
}
AFX_INLINE void CXTPTrackBlock::SetMaxLength(int nMaxLength)
{
	m_nMaxLength = nMaxLength;
}
AFX_INLINE int CXTPTrackBlock::GetMaxLength() const
{
	return m_nMaxLength;
}
AFX_INLINE BOOL CXTPTrackKey::IsKey() const
{
	return TRUE;
}
AFX_INLINE int CXTPTrackMarker::GetPosition() const
{
	return m_nPosition;
}
AFX_INLINE void CXTPTrackMarker::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
}
AFX_INLINE CString CXTPTrackMarker::GetCaption() const
{
	return m_strCaption;
}
AFX_INLINE CRect CXTPTrackMarker::GetRect() const
{
	return m_rcMarker;
}
AFX_INLINE CXTPTrackControl* CXTPTrackMarker::GetControl() const
{
	return m_pControl;
}

typedef CArray<CXTPTrackBlock*, CXTPTrackBlock*> CXTPTrackBlockArray;

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPTRACKBLOCK_H__)
