// XTPTrackControl.h: interface for the CXTPTrackControl class.
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
#if !defined(__XTPTRACKCONTROL_H__)
#	define __XTPTRACKCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTrackBlock;
class CXTPTrackMarker;
class CXTPTrackSelectedBlocks;
class CXTPTrackMarkers;

//---------------------------------------------------------------------------
// Summary:
//     This notification is sent to the parent, when the track slider is
//     changed.
//---------------------------------------------------------------------------
#	define XTP_NM_TRACK_SLIDERCHANGED (NM_FIRST - 200)

//---------------------------------------------------------------------------
// Summary:
//     This notification is sent to the parent, when the track time line is
//     changed.
//---------------------------------------------------------------------------
#	define XTP_NM_TRACK_TIMELINECHANGED (NM_FIRST - 201)

//---------------------------------------------------------------------------
// Summary:
//     This notification is sent to the parent, when the track marker is
//     changed.
//---------------------------------------------------------------------------
#	define XTP_NM_TRACK_MARKERCHANGED (NM_FIRST - 202)

//---------------------------------------------------------------------------
// Summary:
//     This notification is sent to the parent, when the track block position is
//     changed.
//---------------------------------------------------------------------------
#	define XTP_NM_TRACK_BLOCKCHANGED (NM_FIRST - 203)

//---------------------------------------------------------------------------
// Summary:
//     This notification is sent to the parent, when the selected blocks position is
//     changed.
//---------------------------------------------------------------------------
#	define XTP_NM_TRACK_SELECTEDBLOCKSCHANGED (NM_FIRST - 204)

//---------------------------------------------------------------------------
// Summary:
//     This structure contains notification details.
//---------------------------------------------------------------------------
struct XTP_NM_TRACKCONTROL
{
	NMHDR hdr;				  // The NMHDR instance.
	CXTPTrackBlock* pBlock;   // The track block.
	CXTPTrackMarker* pMarker; // The track marker.
};

//===========================================================================
// Summary:
//     Enumeration of operational mouse cursor modes.
// Remarks:
//     TrackControl has several mouse cursor states that are handled by the
//     control. This enumeration helps to clearly identify each of these.
// See Also:
//     CXTPTrackControl::SetMouseCursorMode,
//     CXTPTrackControl::GetMouseCursorMode,
//     CXTPGridControl::XTPGridMouseMode
//
// <KEYWORDS xtpTrackCursorNothing, xtpTrackCursorDrag, xtpTrackCursorResize>
//===========================================================================
enum XTPTrackMouseCursorMode
{
	xtpTrackCursorNothing, // User is just watching.
	xtpTrackCursorDrag,	// User is dragging something.
	xtpTrackCursorResize   // User is resizing something.
};

class CXTPTrackPaintManager;
class CXTPTrackUndoManager;

//===========================================================================
// Summary:
//     This class represents a track control. The track control is an
//     extension to the Grid control. It can do basically everything
//     the Grid control can do with the addition of a track control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackControl : public CXTPGridControl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTrackControl object.
	//-----------------------------------------------------------------------
	CXTPTrackControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTrackControl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPTrackControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the time line position.
	// Parameters:
	//     nTimeLinePos - Time line position.
	//-----------------------------------------------------------------------
	void SetTimeLinePosition(int nTimeLinePos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the time line position.
	// Returns:
	//     The time line position.
	//-----------------------------------------------------------------------
	int GetTimeLinePosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum time line position.
	// Returns:
	//     The minimum time line position.
	//-----------------------------------------------------------------------
	int GetTimeLineMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum time line position.
	// Returns:
	//     The maximum time line position.
	//-----------------------------------------------------------------------
	int GetTimeLineMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bounds of the work area.
	// Parameters:
	//     nWorkAreaMin - Start value of the work area rectangle.
	//     nWorkAreaMax - End value of the work area rectangle.
	//-----------------------------------------------------------------------
	void SetWorkArea(int nWorkAreaMin, int nWorkAreaMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the view port of the work area.
	// Parameters:
	//     nViewPortMin - Start value of the view port rectangle.
	//     nViewPortMax - End value of the view port rectangle.
	//-----------------------------------------------------------------------
	void SetViewPort(int nViewPortMin, int nViewPortMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the time line range.
	// Parameters:
	//     nTimeScaleMin - Start value of the view port rectangle.
	//     nTimeScaleMax - End value of the view port rectangle.
	//-----------------------------------------------------------------------
	void SetTimeLineRange(int nTimeScaleMin, int nTimeScaleMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum value of the work area range.
	// Returns:
	//     The minimum value of the work area range.
	//-----------------------------------------------------------------------
	int GetWorkAreaMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum value of the work area range.
	// Returns:
	//     The maximum value of the work area range.
	//-----------------------------------------------------------------------
	int GetWorkAreaMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the minimum value of the view port.
	// Returns:
	//     The minimum value of the view port.
	//-----------------------------------------------------------------------
	int GetViewPortMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum value of the view port.
	// Returns:
	//     The maximum value of the view port.
	//-----------------------------------------------------------------------
	int GetViewPortMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the track paint manager associated with this control.
	// Returns:
	//     A pointer to the track paint manager associated with this control.
	//-----------------------------------------------------------------------
	CXTPTrackPaintManager* GetTrackPaintManager() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load the properties related
	//     to the track control.
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
	//     Gets the selected blocks in the track control.
	// Returns:
	//     A pointer to the CXTPTrackSelectedBlocks object.
	//-----------------------------------------------------------------------
	CXTPTrackSelectedBlocks* GetSelectedBlocks() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Populates the track control with the records.
	//-----------------------------------------------------------------------
	void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws all rows in the provided device context.
	// Parameters:
	//     pDC         - Provided device context to draw rows image with.
	//     rcClient    - Reference to a rectangle to draw rows image into.
	//     pRows       - Rows collection to draw.
	//     nTopRow     - Index of the topmost visible row of the rows collection.
	//     nColumnFrom - Index of the first column to draw from.
	//     nColumnTo   - Index of the last column to draw to.
	//     pnHeight    - Pointer to an integer to receive the total height
	//                   of the drawn rows; can be NULL.
	// Returns:
	//     The index of the last printed row.
	//-----------------------------------------------------------------------
	int DrawRows(CDC* pDC, CRect& rcClient, int y, CXTPGridRows* pRows, int nTopRow,
				 int nColumnFrom, int nColumnTo, int* pnHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the track offset.
	// Returns:
	//     The track offset.
	//-----------------------------------------------------------------------
	int GetTrackOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the layout of the track control when there is a
	//     change in the metrics of the control.
	//-----------------------------------------------------------------------
	void AdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the track column.
	// Returns:
	//     A pointer to the track column.
	//-----------------------------------------------------------------------
	CXTPGridColumn* GetTrackColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a coordinate point on the track control to a track point.
	// Parameters:
	//     nPosition - Coordinate point to convert.
	// Returns:
	//     The converted track point.
	//-----------------------------------------------------------------------
	int PositionToTrack(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a track point to a coordinate point on the track control.
	// Parameters:
	//     nTrack - Track point to convert.
	// Returns:
	//     The converted coordinate point on the track control.
	//-----------------------------------------------------------------------
	int TrackToPosition(int nTrack) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the index of the track column.
	// Returns:
	//     The zero-based index of the track column.
	//-----------------------------------------------------------------------
	int GetTrackColumnIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the snap margin.
	// Returns:
	//     The snap margin.
	// Remarks:
	//     Snap margin specifies the margin that a block must be dragged into
	//     before it will snap to either another block or a marker.
	//-----------------------------------------------------------------------
	int GetSnapMargin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the time line area.
	// Returns:
	//     A CRect object containing the bounds of the time line area.
	//-----------------------------------------------------------------------
	CRect GetTimelineArea() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to ensure that a specified block is visible.
	// Parameters:
	//     pBlock - Pointer to the CXTPTrackBlock whose visibility must be ensured.
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the track markers object associated with the track control.
	// Returns:
	//     A pointer to the track markers object associated with the track control.
	// Remarks:
	//     CXTPTrackMarkers is a collection of all of the track markers in the track
	//     control. This collection has methods to add/delete/access track
	//     markers. Track markers are used for block alignment. Blocks can be
	//     "snapped" to the left or right of these markers.
	//-----------------------------------------------------------------------
	CXTPTrackMarkers* GetMarkers() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the undo manager associated with the track control.
	// Returns:
	//     A pointer to the track undo manager object associated with the track control.
	// Remarks:
	//     The track undo manager allows for certain operation to be undone
	//     and redone in the track control. Supported operations are
	//     add/remove/move markers and add/remove/move/resize blocks. If
	//     any of these operations are performed, then they can easily
	//     be undone/redone using the methods of the CXTPTrackUndoManager.
	//-----------------------------------------------------------------------
	CXTPTrackUndoManager* GetUndoManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which grid part, if any, is at a specified point.
	// Parameters:
	//     pt    - Point to be tested.
	//     pInfo - Pointer to a CXTPGridHitTestInfo object that will
	//             receive returned information.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, CXTPGridHitTestInfo* pInfo) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sends messages to the parent window of the track control.
	// Parameters:
	//     nMessage - Message to be sent.
	//     pBlock   - Related block.
	//     pMarker  - Related marker.
	// Remarks:
	//     This function is useful to send notification to the parent window
	//     when some event happens in the track control.
	//-----------------------------------------------------------------------
	LRESULT SendMessageToParent(UINT nMessage, CXTPTrackBlock* pBlock = NULL,
								CXTPTrackMarker* pMarker = NULL) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function when you are about to start dragging a block.
	// Parameters:
	//     bResize - TRUE to resize the block, FALSE to drag.
	//-----------------------------------------------------------------------
	void StartDragBlocks(BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a snap position for a block at a specified position on the
	//     track. The snap position depends on if the block should snap to
	//     either another block or a marker.
	// Parameters:
	//     nPosition - Cursor position.
	// Return:
	//     Snap position for the block.
	//-----------------------------------------------------------------------
	int SnapPosition(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function when you are about to start drag selection.
	//-----------------------------------------------------------------------
	void StartDragSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the track column width.
	//-----------------------------------------------------------------------
	void AdjustTrackColumnWidth();

	void SetPaintManager(CXTPTrackPaintManager* pPaintManager);
	void SetTheme(XTPGridPaintTheme paintTheme, BOOL bEnableMetrics /*=FALSE*/);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the mouse cursor mode.
	// Parameters:
	//     enMode - Mouse cursor mode; must be one of the values defined by
	//              the XTPTrackMouseCursorMode enumeration.
	//-----------------------------------------------------------------------
	void SetMouseCursorMode(XTPTrackMouseCursorMode enMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the mouse cursor mode.
	// Returns:
	//     The mouse cursor mode; one of the values defined by
	//     the XTPTrackMouseCursorMode enumeration.
	//-----------------------------------------------------------------------
	XTPTrackMouseCursorMode GetMouseCursorMode() const;

	CPoint m_ptStartDrag; // The point in which dragging starts.

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void OnMoveBlock(CPoint pt, BOOL bResize);
	void OnMoveSelection(CPoint pt);
	void CreateDragBlocks();
	void ReleaseDragBlocks();
	void OnDraw(CDC* pDC);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_nTimeLineMin; // Minimum value for the time line.
	int m_nTimeLineMax; // Maximum value for the time line.

	double m_nViewPortMin; // Minimum value for the view port.
	double m_nViewPortMax; // Maximum value for the view port.

	int m_nOldTrackWidth;			 // The old track width.
	double m_nOldTrackViewPortRange; // The old track view port range.

	int m_nWorkAreaMin; // The minimum value for the work area.
	int m_nWorkAreaMax; // The maximum value for the work area.

	int m_nTimeLinePosition; // The trim line position.
	int m_nTrackColumnIndex; // The track column index.

	CRect m_rcSelectedArea; // The selected area.

	CXTPTrackMarkers* m_pMarkers; // The marker collection object.

	CRect m_rcTimelineArea; // The area of the time line.

	//-----------------------------------------------------------------------
	// Summary:
	//     This structure abstracts the state of block being dragged.
	//-----------------------------------------------------------------------
	struct DRAGBLOCK
	{
		int nOldPos;			// The old position.
		int nOldLength;			// The old length.
		CRect rcOrigin;			// The original rectangle.
		CXTPTrackBlock* pBlock; // Pointer to the block.
	};
	CArray<DRAGBLOCK, DRAGBLOCK&> m_arrDragBlocks; // The collection of blocks which are being
												   // dragged.

	CXTPTrackSelectedBlocks* m_pSelectedBlocks; // The pointer to the selected blocks object.
	CXTPTrackUndoManager* m_pUndoManager;		// The pointer to the undo manager object.

	XTPTrackMouseCursorMode m_mouseCursorMode; // Current mouse operation mode.

public:
	HCURSOR m_hMoveCursor;   // The cursor while moving the blocks.
	HCURSOR m_hResizeCursor; // The cursor while resizing the blocks.

	int m_nSnapMargin;	// The snap margin, specifies the margin that a block must be dragged into
						  // before it will snap to another block or snap to a marker.
	BOOL m_bSnapToBlocks; // TRUE if the current block is about to be snapped to the blocks, FALSE
						  // otherwise.
	BOOL m_bSnapToMarkers; // TRUE if the current block is is about to be snapped to the markers,
						   // FALSE otherwise.

	BOOL m_bScaleOnResize; // Specifies whether to scale the track control view area when resizing
						   // the track control.

	BOOL m_bFlexibleDrag; // Enables or disables flexible dragging of blocks in the track control.

	BOOL m_bAllowBlockRemove; // Specifies whether the user can move blocks from one track to
							  // another.
	BOOL m_bAllowBlockScale;  // Specifies whether the user can resize a block.
	BOOL m_bAllowBlockMove;   // Specifies whether the user can move a block.

	BOOL m_bShowWorkArea; // TRUE to show the work area and FALSE to hide it.
	BOOL m_bShowTimeLinePosition;

	CString m_strTimeFormat; // The string which specifies the time format.

	friend class CXTPTrackControlItem;
};

AFX_INLINE int CXTPTrackControl::GetTimeLinePosition() const
{
	return m_nTimeLinePosition;
}

AFX_INLINE void CXTPTrackControl::SetWorkArea(int nWorkAreaMin, int nWorkAreaMax)
{
	m_nWorkAreaMin = nWorkAreaMin;
	m_nWorkAreaMax = nWorkAreaMax;
	RedrawControl();
}

AFX_INLINE void CXTPTrackControl::SetTimeLineRange(int nTimeScaleMin, int nTimeScaleMax)
{
	m_nTimeLineMin = nTimeScaleMin;
	m_nTimeLineMax = nTimeScaleMax;
	RedrawControl();
}

AFX_INLINE int CXTPTrackControl::GetWorkAreaMin() const
{
	return m_nWorkAreaMin;
}

AFX_INLINE int CXTPTrackControl::GetWorkAreaMax() const
{
	return m_nWorkAreaMax;
}

AFX_INLINE int CXTPTrackControl::GetViewPortMin() const
{
	return (int)m_nViewPortMin;
}

AFX_INLINE int CXTPTrackControl::GetViewPortMax() const
{
	return (int)m_nViewPortMax;
}

AFX_INLINE int CXTPTrackControl::GetTimeLineMin() const
{
	return m_nTimeLineMin;
}

AFX_INLINE int CXTPTrackControl::GetTimeLineMax() const
{
	return m_nTimeLineMax;
}

AFX_INLINE int CXTPTrackControl::GetSnapMargin() const
{
	return m_nSnapMargin;
}

AFX_INLINE CRect CXTPTrackControl::GetTimelineArea() const
{
	return m_rcTimelineArea;
}

AFX_INLINE CXTPTrackPaintManager* CXTPTrackControl::GetTrackPaintManager() const
{
	return (CXTPTrackPaintManager*)GetPaintManager();
}

AFX_INLINE CXTPTrackSelectedBlocks* CXTPTrackControl::GetSelectedBlocks() const
{
	return m_pSelectedBlocks;
}

AFX_INLINE CXTPTrackMarkers* CXTPTrackControl::GetMarkers() const
{
	return m_pMarkers;
}

AFX_INLINE CXTPTrackUndoManager* CXTPTrackControl::GetUndoManager() const
{
	return m_pUndoManager;
}

AFX_INLINE void CXTPTrackControl::SetMouseCursorMode(XTPTrackMouseCursorMode enMode)
{
	m_mouseCursorMode = enMode;
}

AFX_INLINE XTPTrackMouseCursorMode CXTPTrackControl::GetMouseCursorMode() const
{
	return m_mouseCursorMode;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPTRACKCONTROL_H__)
