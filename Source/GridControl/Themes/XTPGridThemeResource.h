// XTPGridThemeResource.h: interface for the CXTPGridPaintManager class.
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
#if !defined(__XTPGridThemeResource_H__)
#	define __XTPGridThemeResource_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridThemeResource is a CXTPGridPaintManager derived
//     class that represents a Resource style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeResource : public CXTPGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeResource();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Grid control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a collapsed bitmap.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pRow     - Pointer to the row.
	//     rcBitmap - Reference to bitmap area rectangle coordinates.
	// Returns:
	//     The size of the drawn bitmap.
	//-----------------------------------------------------------------------
	CSize DrawCollapsedBitmap(CDC* pDC, const CXTPGridRow* pRow, CRect& rcBitmap);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method used to draw a "No items" text message in the Group By area
	//     when there are no items inside it. Override it for custom drawing.
	// Parameters:
	//     pDC     - Pointer to the used device context.
	//     rcItem  - Reference to text area rectangle coordinates.
	// See Also:
	//     CXTPGridPaintManager::m_strNoGroupBy
	//-----------------------------------------------------------------------
	virtual void DrawNoGroupByText(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the column footer control area by its background.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     rcFooter - Footer area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void FillFooter(CDC* pDC, CRect rcFooter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a triangle that shows the sort order of the column.
	// Parameters:
	//     pDC            - Pointer to the used device context.
	//     rcTriangle     - Triangle area rectangle coordinates.
	//     bToDown        - TRUE to turn the top of the triangle downward,
	//                      FALSE to turn the top of the triangle upward.
	//     nShadowWidth   - Size, in pixels, of the shadow to draw under
	//                      the triangle image, or 0 to disable the shadow;
	//                      the default value for this parameter is 0.
	//     bColumnPressed - TRUE if the column is pressed by the user.
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
	virtual void DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth,
							  BOOL bColumnPressed);

	using CXTPGridPaintManager::DrawTriangle;

protected:
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

	virtual void DrawPlainColumnBackground(CDC* /*pDC*/, CRect /*rcColumn*/);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the column footer divider.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pColumn  - Column header area rectangle coordinates.
	//     rcColumn - Column area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawColumnFooterDivider(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws text for the column header.
	// Parameters:
	//     pDC              - Pointer to the used device context.
	//     rcText           - Size and position of the text to render.
	//     strCaption       - NULL-terminated string representing the caption text.
	//     nHeaderAlignment - DT_ text alignment flags.
	//     bIsHeader        - TRUE if the column is to be rendered for the header.
	//     bColumnPressed   - TRUE if the column is pressed by the user.
	//-----------------------------------------------------------------------
	virtual void DrawColumnText(CDC* pDC, CRect rcText, CString strCaption, int nHeaderAlignment,
								BOOL bIsHeader, BOOL bColumnPressed);

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
#endif //#if !defined(__XTPGridThemeResource_H__)
