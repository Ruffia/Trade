// XTPGridPaintManager.h: interface for the CXTPGridPaintManager class.
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
#if !defined(__XTPGRIDPAINTMANAGER_H__)
#	define __XTPGRIDPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarkupUIElement;
class CXTPImageManager;
class CXTPWinThemeWrapper;

struct XTP_GRIDRECORDITEM_DRAWARGS;
struct XTP_GRIDRECORDITEM_METRICS;

class CXTPGridControl;
class CXTPGridRow;
class CXTPGridHeader;
class CXTPGridColumn;
class CXTPGridColumns;
class CXTPGridGroupRow;
class CXTPGridInplaceButton;
class CXTPGridHyperlink;
class CXTPGridRecordItem;
class CXTPGridRecordItemControl;
class CXTPGridHyperlinkStyle;
class CXTPGridBorder;
class CXTPTrackBlock;
class CXTPComboBox;

//===========================================================================
// Summary:
//     Utility class that handles most drawing activities. It stores all of
//     the settings required by a control to perform drawing operations
//     (i.e. fonts, colors, and styles for all other classes of a control).
//     It also implements all functions for drawing typical graphical
//     primitives and functions that directly work with the device context.
// Remarks:
//     Can be overridden to provide another look and feel for the control. Thus,
//     you have an easy way to change the "skin" of your control. Just provide
//     your own implementation of CXTPGridPaintManager and there is no need to
//     touch the functionality for other classes of the control.
//
//     You create a CXTPGridPaintManager object by calling its constructor.
//     Furthermore, you can call getter and setter functions to change every
//     setting as you wish.
// See Also:
//     CXTPGridControl overview, CXTPGridRow, CXTPGridGroupRow,
//     CXTPGridColumn, CXTPGridHeader
//===========================================================================
class _XTP_EXT_CLASS CXTPGridPaintManager : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPGridPaintManager);

	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPGridControl;
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default CXTPGridPaintManager constructor, handles properties initialization.
	// See Also:
	//     RefreshMetrics
	//-----------------------------------------------------------------------
	CXTPGridPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default CXTPGridPaintManager destructor, handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated Grid control for the paint manager.
	// Parameters:
	//     pControl - Pointer to the Grid control to set.
	//-----------------------------------------------------------------------
	virtual void SetGridControl(CXTPGridControl* pControl);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridControl)
	virtual void SetReportControl(CXTPGridControl* pControl);
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the default images used by the Grid control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initializes all drawings defaults (fonts, colors, etc.). Most
	//     of them are system defaults.
	// See Also:
	//     CXTPGridPaintManager
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the default row height for print mode
	//     (i.e. the height of the default rectangle where the row's items will be drawn).
	// Parameters:
	//     pDC         - Pointer to the used device context.
	//     pRow        - Pointer to a grid row.
	//     nTotalWidth - Width of the row.
	// Returns:
	//     The default row height for print mode.
	// Example:
	//     <code>int nRowHeight = GetRowHeight(pDC, pRow)</code>
	//-----------------------------------------------------------------------
	virtual int GetRowHeight(CDC* pDC, CXTPGridRow* pRow);
	virtual int GetRowHeight(
		CDC* pDC, CXTPGridRow* pRow,
		int nTotalWidth); // <combine CXTPGridPaintManager::GetRowHeight@CDC*@CXTPGridRow*>
	int GetRowHeightEx(
		CDC* pDC, CXTPGridRow* pRow, int nTotalWidth,
		const BOOL firstLevel = TRUE); // <combine
									   // CXTPGridPaintManager::GetRowHeight@CDC*@CXTPGridRow*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the height of a record item for a given width.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     nWidth    - Width of the rectangle where the item will be drawn.
	// Returns:
	//     The height of the record item for the given width.
	//-----------------------------------------------------------------------
	int GetItemHeight(XTP_GRIDRECORDITEM_DRAWARGS& drawArgs, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Searches the topmost or bottommost row depending on the height of
	//     the current row. The height of rows depends on merged cells which
	//     can contain big text or other content.
	// Parameters:
	//     pColumns     - Pointer to a CXTPGridColumns object.
	//     pRow         - Pointer to a CXTPGridRow object.
	//     bUpward      - TRUE to set the search direction upward from the current row,
	//                    FALSE to set the search direction downward.
	//     nAdjRowIndex - Reference to an integer value to receive
	//                    the index of the adjacent row.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL FindRowAdjacentToMergedEx(const CXTPGridColumns* pColumns, const CXTPGridRow* pRow,
								   int& nAdjRowIndex, BOOL bUpward);

	//-----------------------------------------------------------------------
	// Summary:
	//     Wrapper for the FindRowAdjacentToMergedEx method. Searches the
	//     topmost or bottommost row depending on the height of the current row.
	// Parameters:
	//     pColumns  - Pointer to a CXTPGridColumns object.
	//     pRow      - Pointer to a CXTPGridRow object.
	//     bUpward   - TRUE to set the search direction upward from the current row,
	//                 FALSE to set the search direction downward.
	// Returns:
	//     The index of the adjacent row.
	//-----------------------------------------------------------------------
	int FindRowAdjacentToMerged(const CXTPGridColumns* pColumns, const CXTPGridRow* pRow,
								BOOL bUpward);

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates row height according to the height of merged items.
	// Parameters:
	//     pColumns    - Pointer to a CXTPGridColumns object.
	//     pDC         - Pointer to the used device context.
	//     pRow        - Pointer to a CXTPGridRow object.
	//     nRowHeight  - Height of pRow without merge cells.
	//     nTotalWidth - Width of the row for print mode.
	// Returns:
	//     The height of the default rectangle where the row's items will be drawn.
	//-----------------------------------------------------------------------
	int RecalcTotalHightOfMergedRows(CXTPGridColumns* pColumns, CDC* pDC, CXTPGridRow* pRow,
									 int nRowHeight, int nTotalWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the height of a preview item for print mode.
	// Parameters:
	//     pDC     - Pointer to the used device context.
	//     pRow    - Pointer to a CXTPGridRow object.
	//     nWidth  - Width of the preview item.
	//     nHeight - Height of the preview item.
	// Returns:
	//     The height of the default rectangle where the preview item will be drawn.
	// Example:
	//     <code>int nPreviewItemHeight = GetPreviewItemHeight(pDC, pRow, nWidth, nHeight)</code>
	//-----------------------------------------------------------------------
	virtual int GetPreviewItemHeight(CDC* pDC, CXTPGridRow* pRow, int nWidth, int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the column header height.
	// Returns:
	//     The current height of the rectangle where the column's headers are drawn.
	//-----------------------------------------------------------------------
	virtual int GetHeaderHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the optimal header height.
	// Parameters:
	//     pControl    - Pointer to a Grid control.
	//     pDC         - Pointer to the used device context.
	//     nTotalWidth - Total header width; used only for printing.
	// Returns:
	//     The optimal header height.
	//-----------------------------------------------------------------------
	virtual int GetHeaderHeight(CXTPGridControl* pControl, CDC* pDC, int nTotalWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the column header height.
	// Parameters:
	//     nHeight -  Header height to be set.
	//-----------------------------------------------------------------------
	virtual void SetHeaderHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the optimal footer height.
	// Parameters:
	//     pControl    - Pointer to a Grid control.
	//     pDC         - Pointer to the used device context.
	//     nTotalWidth - Total footer width; used only for printing.
	// Returns:
	//     The optimal footer height.
	//-----------------------------------------------------------------------
	virtual int GetFooterHeight(CXTPGridControl* pControl, CDC* pDC, int nTotalWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the parent Grid control's background color.
	// Parameters:
	//     pControl - Pointer to the parent Grid control object.
	// Returns:
	//     The color used to fill the parent Grid control's client rectangle.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlBackColor(CXTPGridControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified row with its background color.
	// Parameters:
	//     pDC     - Pointer to the used device context.
	//     pRow    - Pointer to the row.
	//     rcRow   - Row area rectangle coordinates.
	// Example:
	// <code>
	// // paint row background
	// pPaintManager->FillRow(pDC, this, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillRow(CDC* pDC, CXTPGridRow* pRow, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a grid for the control.
	// Parameters:
	//     pDC       - Pointer to the used device context.
	//     bVertical - Grid orientation; TRUE if vertical, FALSE if horizontal.
	//     rcGrid    - Grid area rectangle coordinates.
	// Example:
	// <code>
	// // draw vertical grid in Row
	// CRect rcGridItem(rcRowRect);
	// pPaintManager->DrawGrid(pDC, TRUE, rcGridItem);
	//
	// // draw horizontal grid in Row
	// CPaintDC pDC(this);
	// CRect rcGridItem(rcRowRect);
	// pPaintManager->DrawGrid(pDC, FALSE, rcGridItem);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawGrid(CDC* pDC, XTPGridOrientation orientation, CRect rcGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a freeze column divider.
	// Parameters:
	//     pDC       - Pointer to the used device context.
	//     rc        - Reference to rectangle coordinates.
	//     pControl  - Pointer to a Grid control.
	//     pRow      - Pointer to a row.
	//-----------------------------------------------------------------------
	virtual void DrawFreezeColsDivider(CDC* pDC, const CRect& rc, CXTPGridControl* pControl,
									   CXTPGridRow* pRow = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a vertical timeline marker.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     xPos     - The x- coordinate where the vertical timeline marker will be drawn.
	//     pControl - Pointer to a Grid control.
	//     color    - Color of the marker.
	//-----------------------------------------------------------------------
	virtual void DrawVerticalTimeLineMarker(CDC* pDC, int xPos, CXTPGridControl* pControl,
											COLORREF color = RGB(0, 255, 0));

	// Return current Print Page Width after Preview mode was call and set value
	int GetPrintPageWidth();

	virtual CRect GetGroupRowBmpSize(CRect rcRow, int nBitmapOffset);

	virtual CRect GetGroupRowTextSize(CRect rcBitmap, CRect rcRow, int nBitmapWidth,
									  int nNoIconWidth, int nTextOffset);

	virtual void DrawGroupRowBack(CDC* pDC, CRect rcRow, XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a group row.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pRow     - Pointer to the Group Row.
	//     rcRow    - Group row area rectangle coordinates.
	//     pMetrics - Pointer to an XTP_GRIDRECORDITEM_METRICS structure.
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
	//     Fills the font and color members of the metrics structure
	//     for a specified group row.
	// Parameters:
	//     pRow        - Pointer to the group row.
	//     pMetrics    - Pointer to the metrics structure to be filled.
	//     bPrinting   - TRUE if printing mode is enabled.
	//-----------------------------------------------------------------------
	virtual void FillGroupRowMetrics(CXTPGridGroupRow* pRow, XTP_GRIDRECORDITEM_METRICS* pMetrics,
									 BOOL bPrinting);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills an indent area by its background.
	// Parameters:
	//     pDC   - Pointer to the used device context.
	//     rcRow - Row area rectangle coordinates.
	// Example:
	// <code>
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CRect rcRow(rcClient);
	// // Get default row height
	// int nRowHeight = m_pControl->GetRowHeight(FALSE)
	// rcRow.bottom = rcRow.top + nRowHeight;
	// pPaintManager->FillIndent(pDC, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillIndent(CDC* pDC, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills an item shade by its background if necessary.
	// Parameters:
	//     pDC    - Pointer to the used device context.
	//     rcItem - Item area rectangle coordinates.
	// Example:
	// <code>
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CRect rcItem(m_rcRow);
	// pPaintManager->FillItemShade(pDC, rcItem);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillItemShade(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a column header control area by its background.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     rcHeader - Header area rectangle coordinates.
	// Example:
	// <code>
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CRect rcHeader(rcClient);
	// int nHeaderHeight = m_pControl->GetHeaderHeight();
	// rcHeader.bottom = rcHeader.top + nHeaderHeight;
	// pPaintManager->FillHeaderControl(pDC, rcHeader);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillHeaderControl(CDC* pDC, CRect rcHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a column footer control area by its background.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     rcFooter - Footer area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void FillFooter(CDC* pDC, CRect rcFooter);

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
	//     Draws a column header with all related attributes (sort order, icon, etc.).
	// Parameters:
	//     pDC           - Pointer to the used device context.
	//     pColumn       - Pointer to column header area rectangle coordinates.
	//     pHeader       - Pointer to the grid header.
	//     rcColumn      - Column area rectangle coordinates.
	//     bDrawExternal - TRUE to draw the column above the header;
	//                     the default value for this parameter is FALSE.
	//     nShadowWidth  - Size in pixels for the bottom shadow;
	//                     the default value for this parameter is 0.
	// Example:
	// <code>
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CXTPGridColumn* pColumn = CXTPGridColumn* pColumn = m_pColumns->GetAt(1);
	// CRect rcHeader(rcClient);
	// int nHeaderHeight = m_pControl->GetHeaderHeight();
	// rcHeader.bottom = rcHeader.top + nHeaderHeight;
	// GetPaintManager()->DrawColumn(pDC, pColumn, this, rcHeader);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawColumn(CDC* pDC, CXTPGridColumn* pColumn, CXTPGridHeader* pHeader,
							CRect rcColumn, BOOL bDrawExternal = FALSE, int nShadowWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the column footer divider.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pColumn  - Pointer to column header area rectangle coordinates.
	//     rcColumn - Column area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawColumnFooterDivider(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the column footer with all related attributes (e.g. footer text).
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pColumn  - Pointer to column header area rectangle coordinates.
	//     pHeader  - Pointer to the grid header.
	//     rcColumn - Column area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawColumnFooter(CDC* pDC, CXTPGridColumn* pColumn, CXTPGridHeader* pHeader,
								  CRect rcColumn);

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
	//     Draws the Group By box.
	// Parameters:
	//     pDC       - Pointer to the used device context.
	//     rcGroupBy - Reference to Group By box area rectangle coordinates.
	// Example:
	// <code>
	// CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// pPaintManager->FillGroupByControl(pDC, rcGroupBy);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillGroupByControl(CDC* pDC, CRect& rcGroupBy);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a pointer to the additional image list with glyphs.
	// Remarks:
	//     The additional image list is usually used for bitmaps of collapsed
	//     icons, etc. You call this member function to get a pointer to the
	//     additional image list and manipulate its contents as you wish.
	// Returns:
	//     A pointer to the additional imahe list with glyphs.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetGlyphs() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a horizontal line.
	// Parameters:
	//     pDC  - Pointer to the used device context.
	//     xPos - Horizontal coordinate of the beginning of the line.
	//     yPos - Vertical coordinate of the beginning of the line.
	//     cx   - Length of the line.
	//     clr  - Color of the line.
	// Remarks:
	//     This member function is called anywhere in the control where a
	//     simple horizontal line must be drawn.
	//-----------------------------------------------------------------------
	void DrawHorizontalLine(CDC* pDC, int xPos, int yPos, int cx, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a vertical line.
	// Parameters:
	//     pDC  - Pointer to the used device context.
	//     xPos - Horizontal coordinate of the beginning of the line.
	//     yPos - Vertical coordinate of the beginning of the line.
	//     cx   - Length of the line.
	//     clr  - Color of the line.
	// Remarks:
	//     This member function is called anywhere in the control where a
	//     simple vertical line must be drawn.
	//-----------------------------------------------------------------------
	void DrawVerticalLine(CDC* pDC, int xPos, int yPos, int cy, COLORREF clr);

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

	virtual void DrawColumnInverted(CDC* pDC, CRect rcColumn, BOOL bColumnPressed,
									BOOL bDraggingFromHeader, int nShadowWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a column connector for the Group By box.
	// Parameters:
	//     pDC    - Pointer to the used device context.
	//     ptFrom - Source point of the connector.
	//     ptTo   - Target point of the connector.
	// Remarks:
	//     You use this function to draw a line that represents a
	//     chain of columns in the Group By box. It connects columns
	//     in the Group By box together.
	// Example:
	//     <code>DrawConnector(pDC, CPoint(rcItem.right - 5, rcItem.bottom), CPoint(x, rcItem.bottom
	//     + 4));</code>
	//-----------------------------------------------------------------------
	void DrawConnector(CDC* pDC, CPoint ptFrom, CPoint ptTo);

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a bitmap.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pControl - Pointer to the Grid control where the bitmap will be drawn.
	//     rcColumn - Bitmap area rectangle coordinates.
	//     iImage   - Image index.
	// Remarks
	//     You use this function to draw a bitmap in the column rectangle.
	//     If the provided rectangle is too small, the bitmap will not be drawn.
	//     Generally, the column rectangle must be big enough to contain
	//     text (if presented) and a bitmap.
	// Returns:
	//     The width of the drawn bitmap.
	// Example:
	// <code>
	// // draws bitmap with id ID_READ
	// pPaintManager->DrawBitmap(pDC, pControl, rcColumn, ID_READ);
	// </code>
	//-----------------------------------------------------------------------
	virtual int DrawBitmap(CDC* pDC, CXTPGridControl* pControl, CRect rcColumn, int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an icon for a column.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pColumn  - Pointer to a column object.
	//     rcColumn - Column rectangle.
	//     rcIcon   - Image bounding rectangle.
	//     iIcon    - Image index.
	// Returns:
	//      The width of the drawn bitmap.
	//-----------------------------------------------------------------------
	virtual int DrawColumnIcon(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn, CRect rcIcon,
							   int iIcon);

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
	//     Draws the shell icon.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     rcColumn - Bitmap area rectangle coordinates.
	//     iImage   - Image index.
	// Returns:
	//     The size of the drawn bitmap.
	//-----------------------------------------------------------------------
	virtual CSize DrawShellIcon(CDC* pDC, CRect rcColumn, int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables preview mode for the control.
	// Parameters:
	//     bIsPreviewMode - TRUE to enable preview mode,
	//                      FALSE to disable preview mode.
	// Remarks:
	//     Preview mode has two states: enabled and disabled. When preview is
	//     enabled, the control tries to show additional bands with preview text.
	// Example:
	// <code>
	// // enable preview mode
	// EnablePreviewMode(TRUE);
	//
	// // disable preview mode
	// EnablePreviewMode(FALSE);
	// </code>
	//-----------------------------------------------------------------------
	void EnablePreviewMode(BOOL bIsPreviewMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if preview mode is enabled/disabled for the control.
	// Returns:
	//     TRUE if preview mode is enabled, FALSE if preview mode is disabled.
	//-----------------------------------------------------------------------
	BOOL IsPreviewMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default text font used for control items.
	// Returns:
	//     The default text font used for control items.
	//-----------------------------------------------------------------------
	CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the maximum number of lines of text for a preview item.
	// Parameters:
	//     nMaxLines - New maximum number of lines of text for a preview item.
	// Remarks:
	//     You use this member function to bound the height of a textPreview item
	//     by setting the maximum rows of text for it.
	// Returns:
	//     The previous maximum number of lines of text for a preview item.
	// Example:
	//     <code>int nOldMaxLines = SetMaxPreviewLines(4);</code>
	//-----------------------------------------------------------------------
	int SetMaxPreviewLines(int nMaxLines);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum number of lines of text for a preview item.
	// Returns:
	//     The maximum number of lines of text for a preview item.
	//-----------------------------------------------------------------------
	int GetMaxPreviewLines();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the number of lines required to draw a specified preview
	//     text string, taking into account the maximum number of lines of text
	//     for a preview item.
	// Parameters:
	//     pDC     - Pointer to the used device context.
	//     rcText  - Reference to text area rectangle coordinates.
	//     strText - Reference to the preview text string.
	// Remarks:
	//     This function calculates the number of lines required to draw the
	//     specified preview text string based on the length of the preview text
	//     string, the preview text font, and the width of the provided rectangle.
	//     If the calculated number of lines is greater than the maximum number
	//     of lines of text for a preview item, then the maximum number of lines
	//     of text for a preview item will be returned instead.
	// Returns:
	//     If the calculated number of lines required to draw the preview text
	//     string is greater than the maximum number of lines of text for a preview
	//     item, then the maximum number of lines of text for a preview item will
	//     be returned. Otherwise, the calculated number of lines will be returned.
	//-----------------------------------------------------------------------
	int GetPreviewLinesCount(CDC* pDC, CRect& rcText, const CString& strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visibility of specified grid lines.
	// Parameters:
	//     bVertical - Determines the grid lines to be tested;
	//                 TRUE to test vertical grid lines,
	//                 FALSE to test horizontal grid lines.
	// Returns:
	//     TRUE if the tested grid lines are visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsGridVisible(BOOL bVertical) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the visibility of specified grid lines.
	// Parameters:
	//     orientation - Determines the grid lines to be tested;
	//                   must be one of the values defined by
	//                   the XTPGridOrientation enumeration.
	// Returns:
	//     TRUE if the tested grid lines are visible, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsGridVisible(XTPGridOrientation orientation) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the header style.
	// Parameters:
	//     columnStyle - Header style to be set; must be one of the values
	//                   defined by the XTPGridColumnStyle enumeration.
	// See Also:
	//     GetColumnStyle, XTPGridColumnStyle
	//-----------------------------------------------------------------------
	void SetColumnStyle(XTPGridColumnStyle columnStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the header style.
	// Returns:
	//     The header style; one of the values defined by the
	//     XTPGridColumnStyle enumeration.
	// See Also:
	//     SetColumnStyle, XTPGridColumnStyle
	//-----------------------------------------------------------------------
	XTPGridColumnStyle GetColumnStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the header's custom theme base color for Office2007 column style.
	// Parameters:
	//      clrBaseColor - Custom theme base color for Office2007 column style.
	//                     Set this value to -1 to use the standard theme.
	// See Also:
	//     GetColumnOffice2007CustomThemeBaseColor
	//-----------------------------------------------------------------------
	void SetColumnOffice2007CustomThemeBaseColor(COLORREF clrBaseColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the header's custom theme base color for Office2007 column style.
	// Returns:
	//     The custom theme base color for Office2007 column style.
	//     A value of -1 means that the standard theme is used.
	// See Also:
	//     SetColumnOffice2007CustomThemeBaseColor
	//-----------------------------------------------------------------------
	COLORREF GetColumnOffice2007CustomThemeBaseColor() const;

	//--------------------------------------------------------------------------
	// Summary:
	//     Creates and sets the default text font.
	// Parameters:
	//     lf - Reference to a LOGFONT structure.
	// See Also:
	//     LOGFONT, SetCaptionFont
	//-----------------------------------------------------------------------
	void SetTextFont(LOGFONT& lf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates and sets the default caption font.
	// Parameters:
	//     lf - Reference to a LOGFONT structure.
	// See Also:
	//     LOGFONT, SetTextFont
	//-----------------------------------------------------------------------
	void SetCaptionFont(LOGFONT& lf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the preview item indent.
	// Parameters:
	//     nLeft   - Left indent to be set.
	//     nTop    - Top indent to be set.
	//     nRight  - Right indent to be set.
	//     nBottom - Bottom indent to be set.
	//-----------------------------------------------------------------------
	void SetPreviewIndent(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the in-place button of the Grid control.
	// Parameters:
	//     pDC     - Pointer to the device context.
	//     pButton - Pointer to the in-place button to draw.
	//-----------------------------------------------------------------------
	virtual void DrawInplaceButton(CDC* pDC, CXTPGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the combo button of the Grid control.
	// Parameters:
	//     pDC - Pointer to the device context.
	//     pRc - Pointer to the rectangle to draw.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawInplaceComboButton(CDC* pDC, CRect* pRc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the expand button of the Grid control.
	// Parameters:
	//     pDC - Pointer to the device context.
	//     pRc - Pointer to the rectangle to draw.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawInplaceExpandButton(CDC* pDC, CRect* pRc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the spin button of the Grid control.
	// Parameters:
	//     pDC - Pointer to the device context.
	//     pRc - Pointer to the rectangle to draw.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawInplaceSpinButton(CDC* pDC, CRect* pRc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws tree elements of the Grid control.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     pMetrics  - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                 structure containing item metrics.
	//     rcItem    - Item area rectangle coordinates.
	//     sizeGlyph - Item glyph rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawTreeStructure(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_GRIDRECORDITEM_METRICS* pMetrics, CRect rcItem,
								   CSize sizeGlyph);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an item bitmap. Override this method to draw a custom bitmap.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcItem    - Reference to item area rectangle coordinates.
	//     nImage    - Image index.
	//-----------------------------------------------------------------------
	virtual void DrawItemBitmap(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem, int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an item caption. Override this method to draw a custom caption.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     pMetrics  - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                 structure containing item metrics.
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								 XTP_GRIDRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a row number. Override this method to draw differently.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     rcNumber  - Number area rectangle coordinates.
	//     pRow      - Pointer to the grid row.
	//-----------------------------------------------------------------------
	virtual void DrawRowNumber(CDC* pDC, CRect rcNumber, CXTPGridRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws an item control. Override this method to draw a custom control.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	//     pControl  - Pointer to the control to draw.
	//     rcItem    - Reference to item area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawItemControl(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								 CXTPGridRecordItemControl* pControl, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the paint manager supports fixed or variable row height.
	// Returns:
	//     TRUE if the paint manager supports fixed row height,
	//     FALSE if the paint manager supports variable row height.
	// See Also:
	//     GetRowHeight
	//-----------------------------------------------------------------------
	BOOL IsFixedRowHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the paint manager supports fixed or variable row height.
	// Parameters:
	//     bFixedRowHeight - TRUE to set fixed row height,
	//                       FALSE to set variable row height.
	// See Also:
	//     GetRowHeight
	//-----------------------------------------------------------------------
	void SetFixedRowHeight(BOOL bFixedRowHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the paint manager supports column width WYSIWYG mode.
	// Returns:
	//     TRUE if the paint manager supports column width WYSIWYG mode,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsColumnWidthWYSIWYG() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the paint manager supports last column width WYSIWYG mode.
	// Returns:
	//     TRUE if the paint manager supports last column width WYSIWYG mode,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLastColumnWidthWYSIWYG() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the paint manager shows WYSIWYG page-end markers.
	// Returns:
	//     TRUE if the paint manager shows WYSIWYG page-end markers,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsShowWYSIWYGMarkers() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the paint manager shows WYSIWYG page-end markers.
	// Parameters:
	//     bSet - TRUE to show WYSIWYG page-end markers, FALSE otherwise.
	//-----------------------------------------------------------------------
	void ShowWYSIWYGMarkers(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the paint manager supports column width WYSIWYG mode.
	// Parameters:
	//     bColumnWidthWYSIWYG - TRUE to set column width WYSIWYG mode,
	//                           FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetColumnWidthWYSIWYG(BOOL bColumnWidthWYSIWYG);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the paint manager supports last column width WYSIWYG mode.
	// Parameters:
	//     bColumnWidthWYSIWYG - TRUE to set last column width WYSIWYG mode,
	//                           FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetLastColumnWidthWYSIWYG(BOOL bColumnWidthWYSIWYG);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the paint manager supports fixed or variable
	//     in-place button height.
	// Returns:
	//     TRUE if the paint manager supports fixed in-place button height,
	//     FALSE if the paint manager supports variable in-place button height.
	// See Also:
	//     SetInplaceButtonHeight
	//-----------------------------------------------------------------------
	BOOL IsFixedInplaceButtonHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the paint manager supports fixed or variable
	//     in-place button height.
	// Parameters:
	//     bFixedInplaceButtonHeight - TRUE to set fixed in-place button height,
	//                                 FALSE to set variable in-place button height.
	// See Also:
	//     IsFixedInplaceButtonHeight
	//-----------------------------------------------------------------------
	void SetInplaceButtonHeight(BOOL bFixedInplaceButtonHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the style of the freeze columns divider line.
	// Returns:
	//     The style of the freeze columns divider line, one of the values
	//     defined by the XTPGridFreezeColsDividerStyle enumeration.
	// See Also:
	//      XTPGridFreezeColsDividerStyle
	//-----------------------------------------------------------------------
	int GetFreezeColsDividerStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the style for the freeze columns divider line.
	// Parameters:
	//     nStyle - Style for the freeze columns divider line; must be one of the
	//              values defined by the XTPGridFreezeColsDividerStyle enumeration.
	// See Also:
	//      XTPGridFreezeColsDividerStyle
	//-----------------------------------------------------------------------
	void SetFreezeColsDividerStyle(int nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method to perform custom activities for the item
	//     before processing it.
	// Parameters:
	//     pDrawArgs - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                 structure containing drawing arguments.
	// Returns:
	//     TRUE if the item requires further processing.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawAction(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the "No items" text message in the Group By area when there
	//     are no items inside it. Override this method for custom drawing.
	// Parameters:
	//     pDC     - Pointer to the used device context.
	//     rcItem  - Text area rectangle coordinates.
	// See Also:
	//     CXTPGridPaintManager::m_strNoGroupBy
	//-----------------------------------------------------------------------
	virtual void DrawNoGroupByText(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the grid fills empty space after body rows.
	// Remarks:
	//     By default, the grid does not fill empty space after body rows.
	// Returns:
	//     TRUE if the grid fills empty space after body rows, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDrawGridForEmptySpace() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the grid fills empty space after body rows.
	// Parameters:
	//     bDrawGrid - TRUE to set the grid to fill empty space after body rows,
	//                 FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetDrawGridForEmptySpace(BOOL bDrawGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the group row collapse-expand icon is drawn in print mode.
	// Returns:
	//     TRUE if the group row collapse-expand icon is drawn in print mode,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPrintGroupRowIcon() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets if the group row collapse-expand icon is drawn in print mode.
	// Parameters:
	//     bDraw - TRUE to set the group row collapse-expand icon to be
	//             drawn in print mode, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetPrintGroupRowIcon(BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if columns use expand/collapse icons (+/-) or directional arrows.
	// Returns:
	//     TRUE if columns use expand/collapse icons (+/-),
	//     FALSE if columns use directional arrows.
	//-----------------------------------------------------------------------
	BOOL IsColumnPlusMinusSign() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies if columns use expand/collapse icons (+/-) or directional arrows.
	// Parameters:
	//     bDraw - TRUE to set columns to use expand/collapse icons (+/-),
	//             FALSE to set columns to use directional arrows.
	//-----------------------------------------------------------------------
	void SetColumnPlusMinusSign(BOOL bDraw);

	CXTPGridBorder* GetDefaultItemBorder() const;

	CXTPGridBorder* GetItemBorder(const CXTPGridRecordItem* pItem) const;

	virtual int DrawTrackBlock(CDC* pDC, CRect rc, BOOL bSelected, CXTPTrackBlock* pTrackBlock)
	{
		ASSERT(FALSE); // not handled.
		UNREFERENCED_PARAMETER(pDC);
		UNREFERENCED_PARAMETER(rc);
		UNREFERENCED_PARAMETER(bSelected);
		UNREFERENCED_PARAMETER(pTrackBlock);
		return 0;
	};

#	if XTP_GRID_DEPRECATED()

	//--------------------------------------------------------------------------
	// Summary:
	//     Gets the grid style.
	// Parameters:
	//     bvertical - Determines the grid style to get;
	//                 TRUE to get the vertical grid style,
	//                 FALSE to get the horizontal grid style.
	// Returns:
	//     The grid style; one of the values defined by the XTPGridLineStyle enumeration.
	//-----------------------------------------------------------------------
	XTPGridLineStyle GetGridStyle(BOOL bVertical) const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridStyle)
	XTPGridLineStyle GetReportStyle(BOOL bVertical) const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grid style.
	// Parameters:
	//     bVertical - Determines the grid style to set;
	//                 TRUE to set the vertical grid style,
	//                 FALSE to set the horizontal grid style.
	//     gridStyle - Grid style to be set; can be
	//                 any of the values listed in the Remarks section.
	// Remarks:
	//     Grid style can be one of the following:
	//          * <b>xtpGridGridNoLines</b>: Empty line.
	//          * <b>xtpGridGridSmallDots</b>: Line drawn by small dots.
	//          * <b>xtpGridGridLargeDots</b>: Line drawn by large dots.
	//          * <b>xtpGridGridDashes</b>: Line drawn by dashes.
	//          * <b>xtpGridGridSolid</b>: Draws solid line.
	// See Also:
	//     XTPGridGridStyle
	//-----------------------------------------------------------------------
	void SetGridStyle(BOOL bVertical, XTPGridLineStyle lineStyle);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridStyle)
	void SetReportStyle(BOOL bVertical, XTPGridLineStyle lineStyle);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the grid line color.
	// Returns:
	//     The grid line color.
	//-----------------------------------------------------------------------
	COLORREF GetGridColor() const;

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(GetGridColor)
	COLORREF GetReportColor() const;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the grid line color.
	// Parameters:
	//     clrGridLine - Grid line color to be set.
	// Returns:
	//     The previous grid line color.
	// Example:
	// <code>
	// // Set new color, save old one
	// COLORREF clrOld;
	// clrOld = SetGridColor(RGB(0, 255, 0));
	// </code>
	//-----------------------------------------------------------------------
	COLORREF SetGridColor(COLORREF clrGridLine);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(SetGridColor)
	COLORREF SetReportColor(COLORREF clrGridLine);
	//}}AFX_CODEJOCK_PRIVATE

#	endif

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a line of text with respect to the drawing rectangle.
	// Parameters:
	//     pDrawArgs         - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                         structure containing drawing arguments.
	//     pMetrics          - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                         structure containing item metrics.
	//     strText           - Reference to a text string.
	//     rcItem            - Item area rectangle coordinates.
	//     nFlag             - Flag can be the following values:
	//                         * DT_END_ELLIPSIS - truncate text by ellipses.
	//                         * DT_WORDBREAK    - truncate text by word end.
	//     nCharCounter      - Reference to a printed char counter;
	//                         accepts value before and returns value after drawing.
	//     nHyperlinkCounter - Reference to printed hyperlinks counter;
	//                         accepts value before and returns value after drawing.
	// Remarks:
	//     This member function is used internally by DrawText() and draws one line of text.
	//     It processes plain/hyperlink text for one line. For multi-line text, it processes
	//     the last line and then truncates the text either by ellipses or by word.
	//-----------------------------------------------------------------------
	virtual void DrawTextLine(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
							  XTP_GRIDRECORDITEM_METRICS* pMetrics, const CString& strText,
							  CRect rcItem, int nFlag, int& nCharCounter, int& nHyperlinkCounter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draw a simple string into rectangle
	// Parameters:
	//     pnCurrDrawPos - Pointer to the current drawing position
	//                     (is updated after each call to this function).
	//     pDC           - Point to used device context.
	//     strDraw       - Reference to a text string.
	//     rcDraw        - Text area rectangle coordinates.
	//     nFormat       - Flag can allow next values:
	//                     * DT_END_ELLIPSIS - truncate text by ellipses.
	//                     * DT_WORDBREAK    - truncate text by word end.
	// Remarks:
	//     This member function is used internally by DrawTextLine to draw the
	//     plain chunk of a text string.
	// Returns:
	//     The position in the string where drawing was stopped.
	//-----------------------------------------------------------------------
	virtual int DrawString(int* pnCurrDrawPos, CDC* pDC, const CString& strDraw, CRect rcDraw,
						   UINT nFormat);

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

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Generates a custom grid pattern if necessary.
	//     Otherwise, uses the initially created grid pattern.
	// Parameters:
	//     pDC          - Pointer to the device context.
	//     pBrush       - Pointer to a CBrush object.
	//     rc           - Rectangle area.
	//     pPattern     - Pointer to a short-integer array containing the
	//                    initial bitmap bit values. If this parameter is
	//                    NULL, then the new bitmap is left uninitialized.
	//     clr          - Pattern color.
	//     bUpdateBrush - TRUE to recreate pBrush from pPattern, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void DrawGridPattern(CDC* pDC, CBrush* pBrush, CRect rc, const void* pPattern,
								 COLORREF clr, BOOL bUpdateBrush);
	virtual void DrawGridPattern(
		CDC* pDC, CXTPBrush* pBrush, CRect rc, const void* pPattern, COLORREF clr,
		BOOL bUpdateBrush); // <combine
							// CXTPGridPaintManager::DrawGridPattern@CDC*@CBrush*@CRect@const
							// void*@COLORREF@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Generates a custom grid pattern.
	//     This method only uses pPattern initially.
	// Parameters:
	//     pDC      - Pointer to the device context.
	//     pBrush   - Pointer to a CBrush object.
	//     rc       - Rectangle area.
	//     pPattern - Pointer to a short-integer array containing the
	//                initial bitmap bit values. If this parameter is
	//                NULL, then the new bitmap is left uninitialized.
	//     clr      - Pattern color.
	//-----------------------------------------------------------------------
	virtual void DrawGridPattern(CDC* pDC, CBrush* pBrush, CRect rc, const void* pPattern,
								 COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the in-place button frame.
	// Parameters:
	//     pDC     - Pointer to the device context.
	//     pButton - Pointer to the in-place button.
	//-----------------------------------------------------------------------
	virtual void DrawInplaceButtonFrame(CDC* pDC, CXTPGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the in-place button icon.
	// Parameters:
	//     pDC     - Pointer to the device context.
	//     pButton - Pointer to the in-place button.
	// Returns:
	//     TRUE if the icon was drawn successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawInplaceButtonIcon(CDC* pDC, CXTPGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the optimal height for a specified column.
	// Parameters:
	//     pDC          - Pointer to the device context.
	//     pColumn      - Pointer to a CXTPGridColumn object.
	//     nTotalWidth  - Total header width (used only for printing).
	// Returns:
	//     The optimal height for the specified column.
	//-----------------------------------------------------------------------
	virtual int CalcColumnHeight(CDC* pDC, CXTPGridColumn* pColumn, int nTotalWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws multi-line text.
	// Parameters:
	//     pDrawArgs  - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                  structure containing drawing arguments.
	//     pMetrics   - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                  structure containing item metrics.
	//     strText    - Reference to a text string.
	//     rcItem     - Item area rectangle coordinates.
	//     bWordBreak - TRUE to break lines by words, FALSE to only
	//                  allow new line characters to force new lines.
	// Remarks:
	//     This method keeps hyperlinks active.
	//-----------------------------------------------------------------------
	virtual void DrawMultiLineText(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_GRIDRECORDITEM_METRICS* pMetrics, const CString& strText,
								   CRect rcItem, BOOL bWordBreak = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws single-line text.
	// Parameters:
	//     pDrawArgs    - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                    structure containing drawing arguments.
	//     pMetrics     - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                    structure containing item metrics.
	//     strText      - Reference to a text string.
	//     rcItem       - Item area rectangle coordinates.
	//     nStartPos    - Start position in strText.
	//     nEndPos      - End position in strText.
	//     nActualWidth - Width of the text to be drawn.
	// Remarks:
	//     This method keeps hyperlinks active.
	//-----------------------------------------------------------------------
	virtual void DrawSingleLineText(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
									XTP_GRIDRECORDITEM_METRICS* pMetrics, const CString& strText,
									CRect rcItem, int nStartPos, int nEndPos, int nActualWidth);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the height required for a rectangle bounding multi-line text
	//     to draw the multi-line text when the width of the rectangle is fixed.
	// Parameters:
	//     pDC        - Pointer to the device context.
	//     strText    - Reference to a text string.
	//     nMaxWidth  - Maximum width of the rectangle bounding the text.
	//     bWordBreak - TRUE to break lines by words, FALSE to only allow
	//                  new line characters to increase the row height.
	// Returns:
	//     The height required for the rectangle to draw the multi-line text.
	//-----------------------------------------------------------------------
	virtual int CalculateRowHeight(CDC* pDC, const CString& strText, int nMaxWidth,
								   BOOL bWordBreak = TRUE);

	//{{AFX_CODEJOCK_PRIVATE

	virtual CRect CalculateMaxTextRect(CDC* pDC, const CString& strText, const CRect& rcMaxRect,
									   BOOL bWordBreak = TRUE, BOOL bLimitSize = TRUE,
									   UINT uiFlags = 0);

	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a hyperlink string using hyperlink text settings (i.e. removes blanks).
	// Parameters:
	//     pnCurrDrawPos - Pointer to the current drawing position
	//                     (is updated after each call to this function).
	//     pDrawArgs     - Pointer to an XTP_GRIDRECORDITEM_DRAWARGS
	//                     structure containing drawing arguments.
	//     pMetrics      - Pointer to an XTP_GRIDRECORDITEM_METRICS
	//                     structure containing item metrics.
	//     pHyperlink    - Pointer to a CXTPGridHyperlink object.
	//     strText       - Text string.
	//     rcLink        - Link hot spot area rectangle coordinates.
	//     nFlag         - Flag can allow next values:
	//                     * DT_END_ELLIPSIS - truncate text by ellipses.
	//                     * DT_WORDBREAK    - truncate text by word end.
	//     bTrim         - TRUE to trim the hyperlink, FALSE otherwise.
	// Remarks:
	//     This member function is used internally by DrawTextLine to draw the
	//     hyperlink chunk of a text string and update the hyperlink's hot spot.
	//     If the hyperlink begins/ends with blanks, then the blanks are removed.
	// Returns:
	//     The position in the string where drawing was stopped.
	//-----------------------------------------------------------------------
	virtual int DrawLink(int* pnCurrDrawPos, XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
						 XTP_GRIDRECORDITEM_METRICS* pMetrics, CXTPGridHyperlink* pHyperlink,
						 CString strText, CRect rcLink, int nFlag, BOOL bTrim = FALSE);

	virtual void DrawWindowFrame(CDC* pDC, CRect rcWindow);

public:
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

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the section divider.
	// Parameters:
	//     dividerStyle - Section divider style; must be one of the values defined
	//                    by the XTPGridFixedRowsDividerStyle enumeration.
	// Returns:
	//      The height of the section divider.
	// See Also:
	//      XTPGridFixedRowsDividerStyle
	//-----------------------------------------------------------------------
	int GetSectionDividerHeight(XTPGridFixedRowsDividerStyle dividerStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a drop marker will be used when dragging and dropping.
	// Returns:
	//     TRUE if a drop marker will be used when dragging and dropping,
	//     FALSE if no markers will be used.
	//-----------------------------------------------------------------------
	BOOL IsUseDropMarker() const;

	// private:
	//{{AFX_CODEJOCK_PRIVATE

	COLORREF MixColor(COLORREF clrLight, COLORREF clrDark, double dFactor);

	void Line(CDC* pDC, int x, int y, int cx, int cy, CPen* pPen);

	virtual void DrawTreeStructureLine(CDC* pDC, int x, int y, int cx, int cy, COLORREF clr);

	virtual void ReplaceInHyperLinks(CXTPGridRecordItem* pItem, CString& rstrText, TCHAR chReplace);

	//}}AFX_CODEJOCK_PRIVATE

public:
	CXTPFont m_xtpFontText;		// Grid items default text.
	CXTPFont m_xtpFontBoldText; // Grid items default bold text.
	CXTPFont m_xtpFontCaption;  // Column header caption font.
	CXTPFont m_xtpFontPreview;  // Column header caption font.

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontText, m_xtpFontText, GetTextFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontBoldText, m_xtpFontBoldText,
										  GetBoldTextFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontCaption, m_xtpFontCaption,
										  GetCaptionFontHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fontPreview, m_xtpFontPreview,
										  GetPreviewFontHandle);

	CXTPGridHyperlinkStyle* m_pHyperlinkStyle;		// Style for links.
	CXTPGridHyperlinkStyle* m_pHyperlinkHoverStyle; // Style for hovered links.

	CXTPPaintManagerColor m_clrHighlight;		   // Background color of the highlighted row.
	CXTPPaintManagerColor m_clrBoldText;		   // Grid items bold text color.
	CXTPPaintManagerColor m_clrBoldTextHighlight;  // Grid items bold text highlight color.
	CXTPPaintManagerColor m_clrWindowText;		   // Grid items default text color.
	CXTPPaintManagerColor m_clrHighlightText;	  // Text color of the highlighted text.
	CXTPPaintManagerColor m_clrHeaderControl;	  // Background color of the grid header.
	CXTPPaintManagerColor m_clrCaptionText;		   // Column header text color.
	CXTPPaintManagerColor m_clrCaptionTextPressed; // Column header text color when pressed.
	CXTPPaintManagerColor m_clrControlBack;		   // Background color of the control grid area.
	CXTPPaintManagerColor m_clrGroupRowText;	   // Row text color.
	CXTPPaintManagerColor m_clrGroupShadeBack; // Group row background color when indentation shade
											   // is enabled.
	CXTPPaintManagerColor m_clrGroupShadeText; // Group row foreground color when indentation shade
											   // is enabled.
	CXTPPaintManagerColor m_clrGroupShadeBorder; // Color for group border's shade.
	CXTPPaintManagerColor m_clrGroupBoxBack;	 // Color of group box.
	CXTPPaintManagerColor m_clrControlDark; // Dark background color of the Grid control (used on
											// Group By area).
	CXTPPaintManagerColor m_clrControlLightLight; // Light background color used for drawing shades
												  // (used on column header area).
	CXTPPaintManagerColor m_clrHotDivider; // Color of the column hot divider window (two arrows).
	CXTPPaintManagerColor m_clrIndentControl;   // Color of the tree indentation area.
	CXTPPaintManagerColor m_clrItemShade;		// Color of the shade on sorted by column items.
	CXTPPaintManagerColor m_clrBtnFace;			// Standard button face color.
	CXTPPaintManagerColor m_clrBtnText;			// Standard button text color.
	CXTPPaintManagerColor m_clrPreviewText;		// Preview text color.
	CXTPPaintManagerColor m_clrSelectedRow;		// Selected row background color.
	CXTPPaintManagerColor m_clrSelectedRowText; // Selected row text color.
	CXTPPaintManagerColor m_clrBtnFacePressed;  // Standard button face color when pressed.
	CXTPPaintManagerColor m_clrConnector;		// Header column connector line color.
	CXTPPaintManagerColor m_clrDisableBitmapLightest; // The lightest color of bitmap which covers
													  // disabled grid.
	CXTPPaintManagerColor m_clrDisableBitmapDarkest;  // The darkest color of bitmap which covers
													  // disabled grid.

#	if XTP_GRID_DEPRECATED()

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the style of the header divider line
	//     (i.e. the divider line between the header and body rows).
	// Returns:
	//     The style of the header divider line; one of the values defined
	//     by the XTPGridFixedRowsDividerStyle enumeration.
	// See Also:
	//     XTPGridFixedRowsDividerStyle
	//-----------------------------------------------------------------------
	XTPGridFixedRowsDividerStyle GetHeaderRowsDividerStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the style for the header divider line.
	//     (i.e. the divider line between the header and body rows).
	// Parameters:
	//     nStyle - Style to be set; must be one of the values defined
	//              by the XTPGridFixedRowsDividerStyle enumeration.
	// See Also:
	//     XTPGridFixedRowsDividerStyle
	//-----------------------------------------------------------------------
	void SetHeaderRowsDividerStyle(XTPGridFixedRowsDividerStyle style);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the style of the footer divider line.
	//     (i.e. the divider line between the footer and body rows).
	// Returns:
	//     The style of the footer divider line; one of the values defined
	//     by the XTPGridFixedRowsDividerStyle enumeration.
	// See Also:
	//     XTPGridFixedRowsDividerStyle
	//-----------------------------------------------------------------------
	XTPGridFixedRowsDividerStyle GetFooterRowsDividerStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the style for the footer divider line.
	//     (i.e. the divider line between the footer and body rows).
	// Parameters:
	//     nStyle - Style to be set; must be one of the values defined
	//              by the XTPGridFixedRowsDividerStyle enumeration.
	// See Also:
	//     XTPGridFixedRowsDividerStyle
	//-----------------------------------------------------------------------
	void SetFooterRowsDividerStyle(XTPGridFixedRowsDividerStyle style);

	__declspec(property(get = GetHeaderRowsDividerStyle,
						put = SetHeaderRowsDividerStyle)) XTPGridFixedRowsDividerStyle
		m_HeaderRowsDividerStyle; // Style of a divider line between the header and body rows.

	__declspec(property(get = GetFooterRowsDividerStyle,
						put = SetFooterRowsDividerStyle)) XTPGridFixedRowsDividerStyle
		m_FooterRowsDividerStyle; // Style of a divider line between the footer and body rows.

#	endif // XTP_GRID_DEPRECATED()

#	if XTP_GRID_DEPRECATED()

	CXTPPaintManagerColor GetHeaderRowsDividerColor() const;
	void SetHeaderRowsDividerColor(CXTPPaintManagerColor clrDivider);

	CXTPPaintManagerColor GetFooterRowsDividerColor() const;
	void SetFooterRowsDividerColor(CXTPPaintManagerColor clrDivider);

	__declspec(property(get = GetHeaderRowsDividerColor, put = SetHeaderRowsDividerColor))
		CXTPPaintManagerColor m_clrHeaderRowsDivider;

	__declspec(property(get = GetFooterRowsDividerColor, put = SetFooterRowsDividerColor))
		CXTPPaintManagerColor m_clrFooterRowsDivider;

	__declspec(property(get = GetGridColor, put = SetGridColor)) COLORREF m_clrGridLine;
#	endif

	CXTPPaintManagerColorGradient m_grcGradientColumn;	// Color of column used with
														  // xtpGridColumnOffice2003 style.
	CXTPPaintManagerColorGradient m_grcGradientColumnHot; // HotTracking Color of column used with
														  // xtpGridColumnOffice2003 style.
	CXTPPaintManagerColorGradient m_grcGradientColumnPushed; // Pushed Color of column used with
															 // xtpGridColumnOffice2003 style.
	CXTPPaintManagerColor m_clrGradientColumnShadow;		 // Bottom shadow of column used with
															 // xtpGridColumnOffice2003 style.
	CXTPPaintManagerColor m_clrGradientColumnSeparator;		 // Separator of column used with
															 // xtpGridColumnOffice2003 style.
	CXTPPaintManagerColor m_crlNoGroupByText; // A color for "No items" text in GroupBy area (see
											  // m_strNoGroupBy member).
	CXTPPaintManagerColor m_clrFreezeColsDivider; // Freeze Columns Divider color.
	COLORREF m_clrColumnOffice2007CustomTheme;	// Base color for Office2007 custom theme.

	COLORREF m_clrFormulaSum;	// Color for Formula Sum fields.
	COLORREF m_clrFormulaSubSum; // Color for Formula SubSum fields.

	COLORREF m_clrAlternativeBackground; // Color for Alternative Row Background.
	BOOL m_bUseAlternativeBackground; // TRUE to use  Alternative Row Background, FALSE otherwise.
	BOOL m_bAllowMergeCells;		  // TRUE to Allow Merge Cells, FALSE otherwise.

	int m_nTextIndentForGroupRowsWithNoIcon; // Specifies Text indent if a group row icon is not
											 // used.

	int m_nTreeIndent;				  // Tree indentation.
	int m_nTreeStructureStyle;		  // Tree structure style.
	int m_nTreeTextIndentRowParent;   // Text indent, in pixels, for rows with children.
	int m_nTreeTextIndentRowChildren; // Text indent, in pixels, for rows without children.
	int m_nTreeTextIndentGroupRow;	// Text indent, in pixels, for group rows.

	BOOL m_bGroupRowTextBold; // TRUE to draw group rows with bold text.

	BOOL m_bShadeGroupHeadings;  // TRUE to show the indentation shade, FALSE otherwise.
	BOOL m_bShadeSortColumn;	 // TRUE to show the items shade in the sorted by column, FALSE
								 // otherwise.
	BOOL m_bHotTracking;		 // TRUE to allow column hot tracking.
	BOOL m_bInvertColumnOnClick; // TRUE to invert column on click.

	CString m_strNoItems;	// A string which contains customized "No Items" text for displaying in
							 // view when there are no visible rows.
	CString m_strSortBy;	 // A string which contains customized "Sort By" text for displaying in
							 // tooltip.
	CString m_strIncreasing; // A string which contains customized "Increasing" text for displaying
							 // in tooltip.
	CString m_strDecreasing; // A string which contains customized "Decreasing" text for displaying
							 // in tooltip.
	CString m_strNoGroupBy;  // A string which contains customized "No items" text message for
							 // displaying in Group By area when there are no items inside it.
	CString m_strNoFieldsAvailable; // A string which contains customized "No Fields Available" text
									// message for displaying in Field Chooser.
	CString m_strExpand;   // A string which contains customized "Expand" text for displaying in
						   // tooltip.
	CString m_strCollapse; // A string which contains customized "Collapse" text for displaying in
						   // tooltip.
	BOOL m_bHideSelection; // TRUE when hiding Grid control's selection by drawing selected items
						   // as others, FALSE otherwise.
	BOOL m_bPrintSortTriangle; // TRUE to show the sort triangle in printout.
	//{{AFX_CODEJOCK_PRIVATE
	__declspec(property(get = GetDrawSortTriangleAlways,
						put = SetDrawSortTriangleAlways)) BOOL m_bDrawSortTriangleAlways;
	_XTP_DEPRECATED_IN_FAVOR(m_bPrintSortTriangle)
	BOOL GetDrawSortTriangleAlways() const;

	_XTP_DEPRECATED_IN_FAVOR(m_bPrintSortTriangle)
	void SetDrawSortTriangleAlways(BOOL bValue);
	//}}AFX_CODEJOCK_PRIVATE

	XTPGridDrawSortTriangleStyle m_DrawSortTriangleStyle;
	// xtpGridDrawSortTriangleAlways - the sort triangle is displayed in the column header when a
	// column is sorted and the caption will cut will ellipsis; xtpGridDrawSortTriangleDefault -
	// the sort triangle will be removed when the column size is too small to display the full
	// caption and triangle; xtpGridDrawSortTriangleNever - the sort triangle is never used.
	BOOL m_bUseColumnTextAlignment; // TRUE to draw caption text accordingly to its alignment.

	BOOL m_bRevertAlignment;	  // TRUE to revert column alignments.
	BOOL m_bUseEditTextAlignment; //  TRUE to automatically apply edit alignment using column
								  //  alignment.

	BOOL m_bShowLockIcon; // TRUE to show the lock icon, FALSE to hide.
	BOOL m_bUseShellIcon; // TRUE to use the shell icon, FALSE otherwise.
	int m_iRecOrRowStart; // Start position in numbering (0 or 1 or something user want).
	BOOL m_bRecOrRowNum;  // TRUE to display record number if support mode is used,
						  // FALSE to display row number.

	BOOL m_bMakeUpper; // TRUE to display column header and group row text in all caps.

	XTPGridTreeStructureStyle m_treeStructureStyle; // Tree structure style.

	CRect m_rcPreviewIndent;				   // Preview indentation.
	int m_nFixedColumnsIndent;				   // Fixed Column total width.
	BOOL m_bMoveScrollbarOnFixedColumnsIndent; // Move Scrollbar right follow FixedColumnsIndent.

	BOOL m_bThemedInplaceButtons; // Draw in-place buttons using current theme settings.
	BOOL m_bBaseTreeIndent;		  // TRUE to add Extra Tree indentation to cover initial TreeColumn.

	CUIntArray m_arStartCol; // Start column in each virtual page.

	int m_PrintPageWidth;			   // Width of print page (run-time)
	BOOL m_bPrintVirtualPageNumber;	// TRUE to print horizontal page number.
	BOOL m_bPrintPageRectangle;		   // TRUE to draw rectangle around page.
	int m_RecordNumberWidth;		   // Column width for row/record numbers.
	BOOL m_bForceDynamicMarkupForCell; // TRUE if use Markup for Grid Record Item.
	BOOL m_bForceShowDropMarker;	   // TRUE if Force Show Drop Marker.
								 // used to support external (OLE) drag and drop handler and take
								 // care of drop marker line drawing and auto-scrolling.
	BOOL m_bShowNonActiveInPlaceButton; // TRUE if use Show Non-Active In-Place Button (Combo,
										// Expand, Spin).
	BOOL m_bPrintWatermark;				// TRUE to print watermark.
	BOOL m_bUseGroupRowSelectionState;  // TRUE if use GroupSelectionState Glyph.
	BOOL m_bForceShowTooltip;			// TRUE if use show tooltip for any cell width case.
	BOOL m_bCleanTooltip;				// TRUE if use show tooltip without markup.
	BOOL m_bGrayIfDisable;				// TRUE is disable draw in gray (standard behavior).
	int m_nRowHeightDefault;			// The default row height.
	BOOL m_bCompactText;				// TRUE to ignore Icon for Narrow column to show more text.

	BOOL m_bUseDropMarker; // FALSE to not draw drop marked when dragging/dropping, TRUE by default.
	BOOL m_bSortTriangleOffsetFromRight; // Offset of sort triangle.
	int m_nTreeColumnLineLength;   // This is the extra line length to apply to tree column lines if
								   // there is no expand button.
	int m_nBaseParentRecordIndent; // This is initial number of pixels a parent node will be
								   // indented when in a group.

protected:
	int m_nRowHeight;			// Grid row default height.
	int m_nHeaderHeight;		// Column header height.
	int m_nFooterHeight;		// Column footer height.
	int m_nGroupGridLineHeight; // The height of the group grid line.

	CXTPImageManager* m_pGlyphs; // An additional image list for the Grid control.
	BOOL m_bIsPreviewMode;		 // TRUE to show the item preview, FALSE to hide.
	int m_nResizingWidth;		 // The width of the column resizing marker area.
	int m_nMaxPreviewLines;		 // Restrict maximum lines for preview text.
	CSize m_szGlyph;			 // Glyph size.

	CXTPBrush m_xtpBrushTreeStructure; // Brush to draw tree nodes.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushTreeStructure, m_xtpBrushTreeStructure,
										  GetTreeStructureBrushHandle);

	CXTPBrush m_xtpBrushGroupGridLineStyle; // Brush to draw group row grid lines.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushGroupGridLineStyle,
										  m_xtpBrushGroupGridLineStyle,
										  GetGroupGridLineStyleBrushHandle);

	int m_nFreezeColsDividerStyle; // A set of flags from XTPGridFreezeColsDividerStyle.

	BOOL m_bFixedRowHeight;			// TRUE if fixed rows used.
	BOOL m_bColumnWidthWYSIWYG;		// TRUE if columns width WYSIWYG mode used.
	BOOL m_bShowWYSIWYGMarkers;		// TRUE if in non-print mode header show red page-end markers.
	BOOL m_bLastColumnWidthWYSIWYG; // TRUE if last column width WYSIWYG mode used.
	BOOL m_bDrawGridForEmptySpace;  // TRUE if grid fills the empty space (if any) after the body
									// rows.
	BOOL m_bPrintGroupRowIcon;		// TRUE if printout draw collapse-expand icon for group row.
	BOOL m_bColumnPlusMinusSign;	// TRUE if expand/collapse icons are used, FALSE if directional
									// arrows are used.

	BOOL m_bFixedInplaceButtonHeight; // TRUE if fixed in-place button height is used.

	XTPGridGridStyle m_nGroupGridLineStyle; // Current style for group row grid lines.
	XTPGridColumnStyle m_columnStyle;		// Column drawing style.

	CXTPGridBorder* m_pBorder;

	CXTPWinThemeWrapper* m_themeHeader; // Header theme wrapper.
	CXTPWinThemeWrapper* m_themeButton; // In-place button theme wrapper.
	CXTPWinThemeWrapper* m_themeCombo;  // Combo box in-place button theme wrapper.
	CXTPWinThemeWrapper* m_themeSpin;   // Spin in-place button theme wrapper.

#	ifdef _XTP_INCLUDE_CONTROLS
	void SetControlsTheme(int eTheme); // XTPControlTheme.

	CXTPComboBox* m_pCombo;
	BOOL m_bUseThemedControls;
#	else
	CXTPExcludedMemberPadding<CXTPComboBox*> m_pCombo;
	CXTPExcludedMemberPadding<BOOL> m_bUseThemedControls;
#	endif

	CXTPGridControl* m_pControl; // Pointer to the Grid control object.

	CXTPMarkupUIElement* m_pMarkupUIElement_NoGroupBy; // The no group by markup UI element.
	CString m_strMarkupUIElement_NoGroupBy;			   // The no group by markup UI element string.

	void DrawPattern(CDC* pDC, XTPGridLineStyle lineStyle, XTPGridOrientation orientation,
					 CRect rcLine);
	void CreateLineStyleBrushes();

	CXTPBrush m_xtpBrushDotsSmallH;
	CXTPBrush m_xtpBrushDotsLargeH;
	CXTPBrush m_xtpBrushDashH;
	CXTPBrush m_xtpBrushDotsSmallV;
	CXTPBrush m_xtpBrushDotsLargeV;
	CXTPBrush m_xtpBrushDashV;
	CXTPBrush m_xtpBrushVeriticalGrid;  // Brush to draw vertical grid lines.
	CXTPBrush m_xtpBrushHorizontalGrid; // Brush to draw horizontal grid lines.

	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushDotsSmallH, m_xtpBrushDotsSmallH,
										  GetDotsSmallHBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushDotsLargeH, m_xtpBrushDotsLargeH,
										  GetDotsLargeHBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushDashH, m_xtpBrushDashH,
										  GetDashHBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushDotsSmallV, m_xtpBrushDotsSmallV,
										  GetDotsSmallVBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushDotsLargeV, m_xtpBrushDotsLargeV,
										  GetDotsLargeVBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushDashV, m_xtpBrushDashV,
										  GetDashVBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushVeriticalGrid, m_xtpBrushVeriticalGrid,
										  GetVeriticalGridBrushHandle);
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CBrush, m_brushHorizontalGrid, m_xtpBrushHorizontalGrid,
										  GetHorizontalGridBrushHandle);

	XTPGridGridStyle m_nGridLineStylePreviousV; // Previous style for vertical grid
												// lines. Used for update brush.
	XTPGridGridStyle m_nGridLineStylePreviousH; // Previous style for horizontal grid
												// lines. Used for update brush.

#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridPaintManager);

	afx_msg int OleGetVerticalGridStyle();
	afx_msg void OleSetVerticalGridStyle(int nGridStyle);
	afx_msg int OleGetHorizontalGridStyle();
	afx_msg void OleSetHorizontalGridStyle(int nGridStyle);
	afx_msg void OleRefreshMetrics();
	afx_msg void OleSetColumnStyle();

	afx_msg int OleGetHeaderRowsDividerStyle();
	afx_msg void OleSetHeaderRowsDividerStyle(int nDividerStyle);
	afx_msg int OleGetFooterRowsDividerStyle();
	afx_msg void OleSetFooterRowsDividerStyle(int nDividerStyle);

	afx_msg LPFONTDISP OleGetTextFont();
	afx_msg void OleSetTextFont(LPFONTDISP pFontDisp);
	afx_msg LPDISPATCH OleGetGlyphs();
	afx_msg LPFONTDISP OleGetCaptionFont();
	afx_msg void OleSetCaptionFont(LPFONTDISP pFontDisp);
	afx_msg LPFONTDISP OleGetPreviewTextFont();
	afx_msg void OleSetPreviewTextFont(LPFONTDISP pFontDisp);

	afx_msg LPDISPATCH OleGetHyperlinkStyle();
	afx_msg LPDISPATCH OleGetHyperlinkHoverStyle();

	afx_msg int OleGetHeaderHeight();
	afx_msg void OleSetHeaderHeight(int nHeaderHeight);

	XTP_DECLARE_PROPERTY(BackColor, COLORREF);
	XTP_DECLARE_PROPERTY(ForeColor, COLORREF);
	XTP_DECLARE_PROPERTY(GridLineColor, COLORREF);
	XTP_DECLARE_PROPERTY(ShadeSortColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupIndentColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupShadeBackColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupShadeForeColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupShadeBorderColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupForeColor, COLORREF);
	XTP_DECLARE_PROPERTY(CaptionBackColor, COLORREF);
	XTP_DECLARE_PROPERTY(CaptionForeColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupBoxBackColor, COLORREF);
	XTP_DECLARE_PROPERTY(GroupByNoItemsTextColor, COLORREF);
	XTP_DECLARE_PROPERTY(HighlightBackColor, COLORREF);
	XTP_DECLARE_PROPERTY(HighlightForeColor, COLORREF);
	XTP_DECLARE_PROPERTY(BoldTextHighlight, COLORREF);
	XTP_DECLARE_PROPERTY(BoldText, COLORREF);
	XTP_DECLARE_PROPERTY(PreviewTextColor, COLORREF);
	XTP_DECLARE_PROPERTY(SelectedRowBackColor, COLORREF);
	XTP_DECLARE_PROPERTY(SelectedRowForeColor, COLORREF);

	XTP_DECLARE_PROPERTY(CaptionBackGradientColorLight, COLORREF);
	XTP_DECLARE_PROPERTY(CaptionBackGradientColorDark, COLORREF);
	XTP_DECLARE_PROPERTY(ColumnHotGradientColorLight, COLORREF);
	XTP_DECLARE_PROPERTY(ColumnHotGradientColorDark, COLORREF);
	XTP_DECLARE_PROPERTY(ColumnPushedGradientColorLight, COLORREF);
	XTP_DECLARE_PROPERTY(ColumnPushedGradientColorDark, COLORREF);
	XTP_DECLARE_PROPERTY(ColumnShadowGradient, COLORREF);

	XTP_DECLARE_PROPERTY(FreezeColsDividerColor, COLORREF);
	XTP_DECLARE_PROPERTY(HotDividerColor, COLORREF);
	XTP_DECLARE_PROPERTY(HeaderRowsDividerColor, COLORREF);
	XTP_DECLARE_PROPERTY(FooterRowsDividerColor, COLORREF);
	XTP_DECLARE_PROPERTY(ColumnOffice2007CustomThemeBaseColor, COLORREF);
	//  DECLARE_PROPERTY_UN(HyperLinkColor, COLORREF);
	XTP_DECLARE_PROPERTY(FormulaSumColor, COLORREF);
	XTP_DECLARE_PROPERTY(FormulaSubSumColor, COLORREF);
	XTP_DECLARE_PROPERTY(AlternativeBackgroundColor, COLORREF);

	XTP_DECLARE_PROPERTY(TextIndentForGroupRowsWithNoIcon, int);
	XTP_DECLARE_PROPERTY(SortTriangleOffsetFromRight, BOOL);
	XTP_DECLARE_PROPERTY(TreeColumnLineLength, int);
	XTP_DECLARE_PROPERTY(BaseParentRecordIndent, int);

	XTP_DECLARE_PROPERTY(GroupGridLineHeight, int);

//}}AFX_CODEJOCK_PRIVATE
#	endif
};

AFX_INLINE BOOL CXTPGridPaintManager::IsUseDropMarker() const
{
	return m_bUseDropMarker;
}

AFX_INLINE int CXTPGridPaintManager::GetHeaderHeight()
{
	return m_nHeaderHeight;
}

AFX_INLINE void CXTPGridPaintManager::EnablePreviewMode(BOOL bIsPreviewMode)
{
	m_bIsPreviewMode = bIsPreviewMode;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsPreviewMode() const
{
	return m_bIsPreviewMode;
}

AFX_INLINE CFont* CXTPGridPaintManager::GetTextFont()
{
	return &m_xtpFontText;
}

AFX_INLINE int CXTPGridPaintManager::SetMaxPreviewLines(int nMaxLines)
{
	int nOldMaxLines   = m_nMaxPreviewLines;
	m_nMaxPreviewLines = nMaxLines;
	return nOldMaxLines;
}

AFX_INLINE int CXTPGridPaintManager::GetMaxPreviewLines()
{
	return m_nMaxPreviewLines;
}

AFX_INLINE CXTPImageManager* CXTPGridPaintManager::GetGlyphs() const
{
	return m_pGlyphs;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsFixedRowHeight() const
{
	return m_bFixedRowHeight;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsLastColumnWidthWYSIWYG() const
{
	return m_bLastColumnWidthWYSIWYG;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsColumnWidthWYSIWYG() const
{
	return m_bColumnWidthWYSIWYG;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsShowWYSIWYGMarkers() const
{
	return m_bShowWYSIWYGMarkers;
}

AFX_INLINE void CXTPGridPaintManager::ShowWYSIWYGMarkers(BOOL bSet)
{
	m_bShowWYSIWYGMarkers = bSet;
}

AFX_INLINE void CXTPGridPaintManager::SetColumnWidthWYSIWYG(BOOL bColumnWidthWYSIWYG)
{
	m_bColumnWidthWYSIWYG = bColumnWidthWYSIWYG;
}

AFX_INLINE void CXTPGridPaintManager::SetLastColumnWidthWYSIWYG(BOOL bColumnWidthWYSIWYG)
{
	m_bLastColumnWidthWYSIWYG = bColumnWidthWYSIWYG;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsFixedInplaceButtonHeight() const
{
	return m_bFixedInplaceButtonHeight;
}

AFX_INLINE void CXTPGridPaintManager::SetInplaceButtonHeight(BOOL bFixedInplaceButtonHeight)
{
	m_bFixedInplaceButtonHeight = bFixedInplaceButtonHeight;
}

AFX_INLINE int CXTPGridPaintManager::GetFreezeColsDividerStyle() const
{
	return m_nFreezeColsDividerStyle;
}

AFX_INLINE void CXTPGridPaintManager::SetFreezeColsDividerStyle(int nStyle)
{
	m_nFreezeColsDividerStyle = nStyle;
}

AFX_INLINE XTPGridColumnStyle CXTPGridPaintManager::GetColumnStyle() const
{
	return m_columnStyle;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsColumnPlusMinusSign() const
{
	return m_bColumnPlusMinusSign;
}

AFX_INLINE void CXTPGridPaintManager::SetColumnPlusMinusSign(BOOL bDraw)
{
	m_bColumnPlusMinusSign = bDraw;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsPrintGroupRowIcon() const
{
	return m_bPrintGroupRowIcon;
}

AFX_INLINE void CXTPGridPaintManager::SetPrintGroupRowIcon(BOOL bDraw)
{
	m_bPrintGroupRowIcon = bDraw;
}

AFX_INLINE BOOL CXTPGridPaintManager::IsDrawGridForEmptySpace() const
{
	return m_bDrawGridForEmptySpace;
}

AFX_INLINE void CXTPGridPaintManager::SetDrawGridForEmptySpace(BOOL bDrawGrid)
{
	m_bDrawGridForEmptySpace = bDrawGrid;
}

AFX_INLINE void CXTPGridPaintManager::SetColumnOffice2007CustomThemeBaseColor(COLORREF clrBaseColor)
{
	m_clrColumnOffice2007CustomTheme = clrBaseColor;
	RefreshMetrics();
}

AFX_INLINE COLORREF CXTPGridPaintManager::GetColumnOffice2007CustomThemeBaseColor() const
{
	return m_clrColumnOffice2007CustomTheme;
}

AFX_INLINE void CXTPGridPaintManager::SetGridControl(CXTPGridControl* pControl)
{
	m_pControl = pControl;
}

AFX_INLINE void CXTPGridPaintManager::SetReportControl(CXTPGridControl* pControl)
{
	SetGridControl(pControl);
}

AFX_INLINE BOOL CXTPGridPaintManager::GetDrawSortTriangleAlways() const
{
	return m_bPrintSortTriangle;
}

AFX_INLINE void CXTPGridPaintManager::SetDrawSortTriangleAlways(BOOL bValue)
{
	m_bPrintSortTriangle = bValue;
}

AFX_INLINE void CXTPGridPaintManager::SetReportStyle(BOOL bVertical, XTPGridLineStyle lineStyle)
{
	SetGridStyle(bVertical, lineStyle);
}

AFX_INLINE COLORREF CXTPGridPaintManager::SetReportColor(COLORREF clrGridLine)
{
	return SetGridColor(clrGridLine);
}

AFX_INLINE XTPGridLineStyle CXTPGridPaintManager::GetReportStyle(BOOL bVertical) const
{
	return GetGridStyle(bVertical);
}

AFX_INLINE COLORREF CXTPGridPaintManager::GetReportColor() const
{
	return GetGridColor();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDPAINTMANAGER_H__)
