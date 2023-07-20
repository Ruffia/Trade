// XTPGridThemeOffice2003.h: interface for the CXTPGridPaintManager class.
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
#if !defined(__XTPGridThemeOffice2003_H__)
#	define __XTPGridThemeOffice2003_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPGridThemeOffice2003 is a CXTPGridPaintManager derived
//     class that represents an Office 2003 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridThemeOffice2003 : public CXTPGridPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPGridThemeOffice2003();

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
#endif //#if !defined(__XTPGridThemeOffice2003_H__)
