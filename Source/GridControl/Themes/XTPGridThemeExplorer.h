// XTPGridThemeExplorer.h: interface for the CXTPGridPaintManager class.
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
#if !defined(__XTPGridThemeExplorer_H__)
#	define __XTPGridThemeExplorer_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridThemeExplorer is a CXTPGridPaintManager derived class
//     that represents an Explorer style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeExplorer : public CXTPGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeExplorer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a triangle that shows the sort order of the column.
	// Parameters:
	//     pDC          - Pointer to the used device context.
	//     rcTriangle   - Triangle area rectangle coordinates.
	//     bToDown      - TRUE to turn the top of the triangle downward,
	//                    FALSE to turn the top of the triangle upward.
	//     nShadowWidth - Size, in pixels, of the shadow to draw under
	//                    the triangle image, or 0 to disable the shadow;
	//                    the default value for this parameter is 0.
	// Remarks:
	//     This function is used to draw a triangle image on the header of
	//     the column that is sorted. The triangle represents the direction
	//     of the sort: ascending or descending. The size of the triangle
	//     depends on the rectangle area provided.
	// Example:
	// <code>
	// // draw ascendant triangle
	// CRect rcTriangle;
	// rcTriangle.CopyRect(rcHeader);
	// rcTriangle.DeflateRect(40, 5, 5, 5);
	// DrawTriangle(pDC, rcTriangle, TRUE);
	//
	// // draw descendant triangle
	// CRect rcTriangle;
	// rcTriangle.CopyRect(rcHeader);
	// rcTriangle.DeflateRect(40, 5, 5, 5);
	// DrawTriangle(pDC, rcTriangle, FALSE);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth = 0);

	using CXTPGridPaintManager::DrawTriangle;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the standard glyphs of the Grid control.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     rcColumn - Bitmap area rectangle coordinates.
	//     iImage   - Image index.
	// Returns:
	//     The size of the drawn bitmap.
	//-----------------------------------------------------------------------
	virtual CSize DrawGlyph(CDC* pDC, CRect rcColumn, int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if hot tracking is enabled for the current column style.
	// Returns:
	//     TRUE if hot tracking is enabled, otherwise FALSE.
	// Remarks:
	//     Hot tracking can only be used for the xtpGridColumnOffice2003
	//     and xtpGridColumnExplorer styles.
	// See Also:
	//     m_columnStyle, m_bHotTracking
	//-----------------------------------------------------------------------
	virtual BOOL IsColumHotTrackingEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background of a column using
	//     the current column style.
	// Parameters:
	//     pDC                 - Pointer to the used device context.
	//     pColumn             - Pointer to the column whose background must be drawn.
	//     rcColumn            - Bounding rectangle of the column.
	//     bColumnPressed      - TRUE if the column is pressed by the user.
	//     bDraggingFromHeader - TRUE if the column is being dragged from the header area.
	//     pHeader             - Pointer to the header that the column belongs to.
	//-----------------------------------------------------------------------
	virtual void DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn,
									  BOOL& bColumnPressed, BOOL& bDraggingFromHeader,
									  CXTPGridHeader* pHeader = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the divider between grid sections
	//     (e.g. dividing header record rows and body rows).
	// Parameters:
	//     pDC             - Pointer to the used device context.
	//     rc              - Reference to rectangle coordinates.
	//     dividerPosition - Position of the section divider; must be one of the values
	//                       defined by the XTPGridSectionDividerPosition enumeration.
	//     dividerStyle    - Style of the section divider; must be one of the values
	//                       defined by the XTPGridSectionDividerStyle enumeration.
	//     dividerColor    - Color of the section divider.
	//-----------------------------------------------------------------------
	void DrawSectionDivider(CDC* pDC, const CRect& rc,
							XTPGridSectionDividerPosition dividerPosition,
							XTPGridSectionDividerStyle dividerStyle,
							CXTPPaintManagerColor dividerColor);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGridThemeExplorer_H__)
