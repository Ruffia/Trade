// XTPGridThemeOffice2003Luna.h: interface for the CXTPGridPaintManager class.
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
#if !defined(__XTPGridThemeOffice2003Luna_H__)
#	define __XTPGridThemeOffice2003Luna_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridThemeOffice2003Luna is a CXTPGridPaintManager derived
//     class that represents an Office 2003 Luna style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeOffice2003Luna : public CXTPGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeOffice2003Luna();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Grid control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	//     Draws the column footer divider.
	// Parameters:
	//     pDC      - Pointer to the used device context.
	//     pColumn  - Pointer to column header area rectangle coordinates.
	//     rcColumn - Column area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawColumnFooterDivider(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn);

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
#endif //#if !defined(__XTPGridThemeOffice2003Luna_H__)
