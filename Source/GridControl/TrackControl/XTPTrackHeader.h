// XTPTrackHeader.h: interface for the CXTPTrackHeader class.
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
#if !defined(__XTPTRACKEADER_H__)
#	define __XTPTRACKEADER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     This class represents a track control header.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackHeader : public CXTPGridHeader
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPTrackHeader object.
	// Parameters:
	//     pControl - Pointer to the Grid control.
	//     pColumns - Pointer to the grid columns.
	//-----------------------------------------------------------------------
	CXTPTrackHeader(CXTPGridControl* pControl, CXTPGridColumns* pColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackHeader default destructor.
	//-----------------------------------------------------------------------
	~CXTPTrackHeader();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the block.
	// Parameters:
	//     pDC         - Pointer to the device context.
	//     rcHeader    - Header rectangle.
	//     nLeftOffset - Left offset.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC, CRect rcHeader, int nLeftOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the left mouse button is clicked on the header.
	// Parameters:
	//     ptClick - Point where the mouse event occurred.
	//-----------------------------------------------------------------------
	void OnLButtonDown(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the mouse is moved over the header.
	// Parameters:
	//     nFlags - State of mouse buttons, while the mouse is moved.
	//     point  - Point where the mouse event occurred.
	//-----------------------------------------------------------------------
	void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the scrollbar is about to be resized/dragged.
	// Parameters:
	//     bResize - TRUE if the scrollbar is about to be resized,
	//               FALSE if the scrollbar is about to be dragged.
	//-----------------------------------------------------------------------
	void StartDragScrollBar(BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the slider is about to be resized/dragged.
	// Parameters:
	//     bResize - TRUE if the slider is about to be resized,
	//               FALSE if the slider is about to be dragged.
	//-----------------------------------------------------------------------
	void StartDragSlider(BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the marker is about to be dragged.
	// Parameters:
	//     nMarkerIndex - Zero-based index of the marker which is about
	//                    to be dragged.
	//-----------------------------------------------------------------------
	void StartDragMarker(int nMarkerIndex);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to resize/move the slider.
	// Parameters:
	//     pt      - New position for the slider.
	//     bResize - TRUE if the slider is resizing,
	//               FALSE if the slider is moving.
	//-----------------------------------------------------------------------
	void OnMoveSlider(CPoint pt, BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move the marker.
	// Parameters:
	//     pt           - New position for the marker.
	//     nMarkerIndex - Zero-based index of the marker which is moving.
	//-----------------------------------------------------------------------
	void OnMoveMarker(CPoint pt, int nMarkerIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to resize/move the scrollbar.
	// Parameters:
	//     pt      - New position for the scrollbar.
	//     bResize - TRUE if the scrollbar is resizing,
	//               FALSE if the scrollbar is moving.
	//-----------------------------------------------------------------------
	void OnMoveScrollBar(CPoint pt, BOOL bResize);

protected:
	CPoint m_ptStartDrag;  // The drag start point.
	int m_nOldWorkAreaMin; // The old work area minimum value.
	int m_nOldWorkAreaMax; // The old work area maximum value.
	int m_nOldViewPortMin; // The old view port minimum value.
	int m_nOldViewPortMax; // The old view port maximum value.

	int m_nOldMarkerPos; // The old marker position.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPTRACKEADER_H__)
