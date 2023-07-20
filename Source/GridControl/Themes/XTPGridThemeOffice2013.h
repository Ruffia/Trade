// XTPGridThemeOffice2013.h: interface for the CXTPGridPaintManager class.
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
#if !defined(__XTPGridThemeOffice2013_H__)
#	define __XTPGridThemeOffice2013_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridThemeOffice2013 is a CXTPGridPaintManager derived
//     class that represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeOffice2013 : public CXTPGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeOffice2013();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the Grid control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Grid control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a triangle that shows the sort order of the column.
	// Parameters:
	//     pDC                 - Pointer to the used device context.
	//     rcTriangle          - Triangle area rectangle coordinates.
	//     bToDown             - TRUE to turn the top of the triangle downward,
	//                           FALSE to turn the top of the triangle upward.
	//     bDraggingFromHeader - TRUE if the column is being dragged from the header area.
	//     nShadowWidth        - Size, in pixels, of the shadow to draw under
	//                           the triangle image, or 0 to disable the shadow;
	//                           the default value for this parameter is 0.
	//     bColumnPressed      - TRUE if the column is pressed by the user.
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
	virtual void DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, BOOL bDraggingFromHeader,
							  int nShadowWidth, BOOL bColumnPressed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a collapsed bitmap.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pRow     - Pointer to a grid row.
	//     rcBitmap - Reference to bitmap area rectangle coordinates.
	// Returns:
	//     The size of the drawn bitmap.
	//-----------------------------------------------------------------------
	virtual CSize DrawCollapsedBitmap(CDC* pDC, const CXTPGridRow* pRow, CRect& rcBitmap);

	virtual void DrawColumnInverted(CDC* pDC, CRect rcColumn, BOOL bColumnPressed,
									BOOL bDraggingFromHeader, int nShadowWidth);

	virtual void DrawWindowFrame(CDC* pDC, CRect rcWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the font and color members of the metrics structure
	//     for a specified group row.
	// Parameters:
	//     pRow        - Pointer to the group row.
	//     pMetrics    - Pointer to the metrics structure to be filled.
	//     bPrinting   - TRUE if printing mode is enabled.
	//-----------------------------------------------------------------------
	virtual void FillGroupRowMetrics(CXTPGridGroupRow* pRow, XTP_GRIDRECORDITEM_METRICS* pMetrics,
									 BOOL bPrinting);

	virtual void DrawGroupRowBack(CDC* pDC, CRect rcRow, XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a group row.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pRow     - Pointer to the group row.
	//     rcRow    - Group row area rectangle coordinates.
	//     pMetrics - Pointer to a metrics structure.
	// Example:
	// <code>
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// pDC->SetBkMode(TRANSPARENT);
	// pPaintManager->DrawGroupRow(pDC, this, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawGroupRow(CDC* pDC, CXTPGridGroupRow* pRow, CRect rcRow,
							  XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws focused rows.
	// Parameters:
	//     pDC   - Pointer to the used device context.
	//     rcRow - Area rectangle coordinates.
	// Example:
	// <code>
	// CRect rcClient(GetParent()->GetClientRect());
	// CXTPGridHeader* pHeader = m_pColumns->GetGridHeader();
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// GetPaintManager()->DrawResizingRect(pDC, pHeader, rcClient);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawFocusedRow(CDC* pDC, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the default row height for print mode
	//     (i.e. the height of the default rectangle where the row's items will be drawn).
	// Parameters:
	//     pDC  - Pointer to the used device context.
	//     pRow - Pointer to a grid row.
	// Returns:
	//     The default row height for print mode.
	// Example:
	//     <code>int nRowHeight = GetRowHeight(pDC, pRow)</code>
	//-----------------------------------------------------------------------
	virtual int GetRowHeight(CDC* pDC, CXTPGridRow* pRow);

	using CXTPGridPaintManager::GetRowHeight;

	virtual CRect GetGroupRowBmpSize(CRect rcRow, int nBitmapOffset);

	virtual CRect GetGroupRowTextSize(CRect rcBitmap, CRect rcRow, int nBitmapWidth,
									  int nNoIconWidth, int nTextOffset);

	CXTPPaintManagerColor m_clrGroupText;
	CXTPPaintManagerColor m_clrGroupTextHilite;
	CXTPPaintManagerColor m_clrGroupBack;
	CXTPPaintManagerColor m_clrGroupBackHilite;
	CXTPPaintManagerColor m_clrGlyph;		   // Header glyph color.
	CXTPPaintManagerColor m_clrGlyphHighlight; // Header highlighted glyph color.
	CXTPPaintManagerColor m_clrGlyphExpand;	// Expand glyph color.
	CXTPPaintManagerColor m_clrGlyphBorder;	// Expand glyph border color.
	CXTPPaintManagerColor m_clrGlyphContract;  // Contract glyph color.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGridThemeOffice2013_H__)
