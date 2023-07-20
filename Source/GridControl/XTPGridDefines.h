// XTPGridDefines.h
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
#if !defined(__XTPGRIDDEFINES_H__)
#	define __XTPGRIDDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	ifdef _XTP_ACTIVEX
enum XTPEnumGridCustomDraw
{
	xtpCustomDrawItem			= 0x01,
	xtpCustomMeasureRow			= 0x02,
	xtpCustomBeforeDrawRow		= 0x04,
	xtpCustomDrawPreviewItem	= 0x08,
	xtpCustomMeasurePreviewItem = 0x10,
};
#	endif

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the shell icons.
//-----------------------------------------------------------------------
enum XTPGridShellIcon
{
	xtpGridShellIconLock = 47
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the index of the glyphs.
//-----------------------------------------------------------------------
enum XTPGridGlyph
{
	xtpGridGlyphInvalid			  = -1,
	xtpGridGlyphRowCollapse		  = 0,
	xtpGridGlyphRowExpand		  = 1,
	xtpGridGlyphEnabledUnchecked  = 2,
	xtpGridGlyphEnabledChecked	= 3,
	xtpGridGlyphDisabledUnchecked = 4,
	xtpGridGlyphDisabledChecked   = 5,
	xtpGridGlyphColumnExpand	  = 6,
	xtpGridGlyphColumnRestore	 = 7,
	xtpGridGlyphEnabledTristate   = 8,
	xtpGridGlyphItemLocked		  = 9,
	xtpGridGlyphItemUnlocked	  = 10,
	xtpGridGlyphCount
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the possible checkbox states.
//-----------------------------------------------------------------------
enum XTPGridCheckState
{
	xtpGridCheckStateInvalid	   = -1, // The checkbox state is invalid.
	xtpGridCheckStateUnchecked	 = 0,  // The checkbox is in an unchecked state.
	xtpGridCheckStateChecked	   = 1,  // The checkbox is in a checked state.
	xtpGridCheckStateIndeterminate = 2,  // The checkbox is in an indeterminate state.
	xtpGridCheckStateCount		   = 3   // Number of checkbox states.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines body, header, and footer rows.
// See Also:
//      CXTPGridRow::m_nRowType
//-----------------------------------------------------------------------
enum XTPGridRowType
{
	xtpRowTypeInvalid = -1, // Indicates an invalid row type.
	xtpRowTypeBody	= 0,  // The row is in the body rows collection.
	xtpRowTypeHeader  = 1,  // The row is in the header rows collection.
	xtpRowTypeFooter  = 2   // The row is in the footer rows collection.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of orientations.
// See Also:
//     SetGridStyle
//-----------------------------------------------------------------------
enum XTPGridOrientation
{
	xtpGridOrientationHorizontal = 0,
	xtpGridOrientationVertical   = 1,
	xtpGridOrientationAll		 = 2
};

//===========================================================================
// Summary:
//     Represents predefined grid line styles.
// Example:
//     <code>m_wndGrid.SetGridStyle(FALSE, xtpGridGridLargeDots);</code>
// See Also:
//     CXTPGridControl::SetGridStyle
//
// <KEYWORDS xtpGridGridNoLines, xtpGridGridSmallDots, xtpGridGridLargeDots,
// xtpGridGridDashes, xtpGridGridSolid>
//===========================================================================
enum XTPGridLineStyle
{
	xtpGridLineStyleNone	 = 0, // No line.
	xtpGridLineStyleDotSmall = 1, // Line is drawn with small dots.
	xtpGridLineStyleDotLarge = 2, // Line is drawn with large dots.
	xtpGridLineStyleDash	 = 3, // Line is drawn with dashes.
	xtpGridLineStyleSolid	= 4, // Solid line.
	xtpGridLineStyleCount	= 5, // Number of line styles.

	// Old names
	xtpGridGridNoLines   = xtpGridLineStyleNone,
	xtpGridGridSmallDots = xtpGridLineStyleDotSmall,
	xtpGridGridLargeDots = xtpGridLineStyleDotLarge,
	xtpGridGridDashes	= xtpGridLineStyleDash,
	xtpGridGridSolid	 = xtpGridLineStyleSolid
};

typedef XTPGridLineStyle XTPGridGridStyle;

//===========================================================================
// Summary:
//     Represents predefined freeze column line styles.
//===========================================================================
enum XTPGridFreezeColsDividerStyle
{
	xtpGridFreezeColsDividerThin	   = 0x01, // Thin line style.
	xtpGridFreezeColsDividerBold	   = 0x02, // Bold line style.
	xtpGridFreezeColsDividerHeader	 = 0x04, // Header line style.
	xtpGridFreezeColsDividerShade	  = 0x08, // Shade line style.
	xtpGridFreezeColsDividerShowAlways = 0x10, // Show event if AutoColumnSizing is On.
};

//{{AFX_CODEJOCK_PRIVATE

// deprecated
#	define xtpGridNoLines xtpGridGridNoLines
#	define xtpGridSmallDots xtpGridGridSmallDots
#	define xtpGridLargeDots xtpGridGridLargeDots
#	define xtpGridDashes xtpGridGridDashes
#	define xtpGridSolid xtpGridGridSolid

//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Represents predefined column styles.
// Example:
//     <code>m_wndGrid.GetPaintManager()->m_columnStyle = xtpGridColumnFlat;</code>
// See Also:
//     CXTPGridPaintManager::m_columnStyle, CXTPGridPaintManager
//
// <KEYWORDS xtpGridColumnShaded, xtpGridColumnFlat>
//===========================================================================
enum XTPGridColumnStyle
{
	xtpGridColumnShaded,	 // Columns are gray shaded.
	xtpGridColumnFlat,		 // Flat style for drawing columns.
	xtpGridColumnExplorer,   // Explorer column style.
	xtpGridColumnOffice2003, // Gradient column style.
	xtpGridColumnResource	// Office 2007/2010 column style.
};

//{{AFX_CODEJOCK_PRIVATE

// deprecated
#	define xtpColumnShaded xtpGridColumnShaded
#	define xtpColumnFlat xtpGridColumnFlat
#	define xtpGridColumnOffice2007 xtpGridColumnResource

//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     Represent the style of lines used to draw a hierarchical tree structure.
// Example:
//     <code>m_wndGrid.GetPaintManager()->m_treeStructureStyle =
//     xtpGridTreeStructureSolid;</code>
// See Also:
//     CXTPGridPaintManager::m_treeStructureStyle, CXTPGridPaintManager
//
// <KEYWORDS xtpGridTreeStructureNone, xtpGridTreeStructureSolid>
//-----------------------------------------------------------------------
enum XTPGridTreeStructureStyle
{
	xtpGridTreeStructureNone,  // No lines will be drawn from the parent node to each child node.
	xtpGridTreeStructureSolid, // Lines will be drawn from the parent node to each child node at
							   // the next level.
	xtpGridTreeStructureDots   // Dots will be drawn from the parent node to each child node at the
							   // next level.
};

//-----------------------------------------------------------------------
// Summary:
//     Represent the header/footer rows divider style.
//-----------------------------------------------------------------------
enum XTPGridSectionDividerStyle
{
	xtpGridSectionDividerStyleNone	= 0x00, // No line.
	xtpGridSectionDividerStyleThin	= 0x01, // Thin style.
	xtpGridSectionDividerStyleBold	= 0x02, // Bold style.
	xtpGridSectionDividerStyleShade   = 0x04, // Shaded line style.
	xtpGridSectionDividerStyleOutlook = 0x08, // Outlook line style.

	//{{AFX_CODEJOCK_PRIVATE
	// Deprecated
	xtpGridFixedRowsDividerNone	= xtpGridSectionDividerStyleNone,
	xtpGridFixedRowsDividerThin	= xtpGridSectionDividerStyleThin,
	xtpGridFixedRowsDividerBold	= xtpGridSectionDividerStyleBold,
	xtpGridFixedRowsDividerShade   = xtpGridSectionDividerStyleShade,
	xtpGridFixedRowsDividerOutlook = xtpGridSectionDividerStyleOutlook
	//}}AFX_CODEJOCK_PRIVATE
};

//{{AFX_CODEJOCK_PRIVATE
// Old enum name
typedef XTPGridSectionDividerStyle XTPGridFixedRowsDividerStyle;
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     Section divider position.
//-----------------------------------------------------------------------
enum XTPGridSectionDividerPosition
{
	xtpGridSectionDividerPositionTop	= 0, // Divider above the section.
	xtpGridSectionDividerPositionBottom = 1  // Divider below the section.
};

//-----------------------------------------------------------------------
// Summary:
//     Represent the Grid Draw Sort Triangle logic.
//-----------------------------------------------------------------------
enum XTPGridDrawSortTriangleStyle
{
	xtpGridDrawSortTriangleDefault = 0, // The triangle will be removed when the column size is
										// too small to display the full caption and triangle.
	xtpGridDrawSortTriangleAlways = 1,  // The sort triangle displayed in column header when a
										// column is sorted and the caption will be cut with
										// ellipsis.
	xtpGridDrawSortTriangleNever = 2	// Never draw the triangle.
};

//===========================================================================
// Summary:
//     Enumeration of hyperlink text decorations.
//
// <KEYWORDS xtpGridTextDecorationNone, xtpGridTextDecorationUnderline>
//===========================================================================
enum XTPGridTextDecoration
{
	xtpGridTextDecorationNone	  = 0, // Text has no decoration.
	xtpGridTextDecorationUnderline = 1  // Text is underlined.
};

//-----------------------------------------------------------------------
// Summary:
//     Visual theme enumeration.
// Example:
//     <code> m_wndGridControl.SetTheme(xtpGridThemeOffice2013); </code>
// See Also:
//     CXTPGridControl::SetTheme, CXTPGridControl::GetCurrentTheme
//-----------------------------------------------------------------------
enum XTPGridPaintTheme
{
	xtpGridThemeDefault,							 // Default theme.
	xtpGridThemeOfficeXP,							 // Office XP style theme.
	xtpGridThemeOffice2003,							 // Office 2003 style theme.
	xtpGridThemeOffice2003Luna,						 // Office 2003 style theme with luna colors.
	xtpGridThemeOffice2013,							 // Office 2013 style theme.
	xtpGridThemeOffice2016 = xtpGridThemeOffice2013, // Office 2016 style theme.
	xtpGridThemeVisualStudio2012Light,				 // Visual Studio 2012 Light style theme.
	xtpGridThemeVisualStudio2012Dark,				 // Visual Studio 2012 Dark style theme.
	xtpGridThemeVisualStudio2012Blue,				 // Visual Studio 2012 Blue style theme.
	xtpGridThemeVisualStudio2015,					 // Visual Studio 2015 style theme.
	xtpGridThemeExplorer,							 // Windows Explorer theme.
	xtpGridThemeResource,							 // Resource .DLL style theme.
	xtpGridThemeNativeWindows10,					 // Resource .DLL style theme.
	xtpGridThemeVisualStudio2017,					 // Visual Studio 2017 style theme.
	xtpGridThemeVisualStudio2019,					 // Visual Studio 2019 style theme.
	xtpGridThemeVisualStudio2022,					 // Visual Studio 2022 style theme.
};

//-----------------------------------------------------------------------
// Summary:
//     A NULL-terminated string used by CXTPGridControl which specifies
//     the window class name for the Grid control.
// Example:
// <code>
// BOOL CXTPGridControl::RegisterWindowClass()
// {
//     WNDCLASS wndcls;
//     HINSTANCE hInst = XTPGetInstanceHandle();
//
//     if (!(::GetClassInfo(hInst, XTPGRIDCTRL_CLASSNAME, &wndcls)))
//     {
//         // otherwise we need to register a new class
//         wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
//         wndcls.lpfnWndProc      = ::DefWindowProc;
//         wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
//         wndcls.hInstance        = hInst;
//         wndcls.hIcon            = NULL;
//         wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
//         wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
//         wndcls.lpszMenuName     = NULL;
//         wndcls.lpszClassName    = XTPGRIDCTRL_CLASSNAME;
//
//         if (!AfxRegisterClass(&wndcls))
//         {
//             AfxThrowResourceException();
//             return FALSE;
//         }
//     }
//
//     return TRUE;
// }
// </code>
// See Also:
//     CXTPGridControl::RegisterWindowClass
//-----------------------------------------------------------------------
const TCHAR XTPGRIDCTRL_CLASSNAME[]  = _T("XTPGrid");
const TCHAR XTPTRACKCTRL_CLASSNAME[] = _T("XTPTrackControl");

//===========================================================================
// Summary:
//     Grid control records clipboard format name.
//===========================================================================
static const LPCTSTR XTPGRIDCTRL_CF_RECORDS = _T("XTPGrid_CF_Records");

//-----------------------------------------------------------------------
// Summary:
//     This constant indicates that a COLORREF is not defined.
// Example:
// <code>
// void CXTPGridRecordItem::GetItemMetrics(
//     XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
//     XTP_GRIDRECORDITEM_METRICS* pItemMetrics)
// {
//     if (m_clrBackground != XTP_GRID_COLOR_DEFAULT)
//         pItemMetrics->clrBackground = m_clrBackground;
//
//     if (m_clrText != XTP_GRID_COLOR_DEFAULT)
//         pItemMetrics->clrForeground = m_clrText;
//
//     if (m_pFontCaption != NULL)
//         pItemMetrics->pFont = m_pFontCaption;
//
//     else if (m_bBoldText)
//         pItemMetrics->pFont =
//             &pDrawArgs->pControl->GetPaintManager()->m_fontBoldText;
// }
// </code>
// See Also:
//    CXTPGridRecordItem, CXTPGridRow
//-----------------------------------------------------------------------
const COLORREF XTP_GRID_COLOR_DEFAULT = (COLORREF)-1;

//-----------------------------------------------------------------------
// Summary:
//     This constant indicates that an icon ID is not defined.
// Example:
// <code>
// m_wndGrid.AddColumn(new CXTPGridColumn(COLUMN_PRICE, _T("Price"),
//     80, TRUE, XTP_GRID_NOICON, TRUE, FALSE));
// </code>
// See Also:
//    CXTPGridColumn
//-----------------------------------------------------------------------
const int XTP_GRID_NOICON = (int)-1;

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the column right clicked event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_HEADER_RCLICK notification message is sent to inform the
//     owner window that the user right clicked a column of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_HEADER_RCLICK, XTP_ID_GRID_CONTROL, OnGridColumnRClick)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridColumnRClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*)pNotifyStruct;
//     ASSERT(pItemNotify->pColumn);
//     CPoint ptClick = pItemNotify->pt;
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_HEADER_RCLICK (NM_FIRST - 51)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the row selection changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_SELCHANGED notification message is sent to inform the
//     owner window that the user selected a row of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_SELCHANGED, XTP_ID_GRID_CONTROL, OnGridSelChanged)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridSelChanged(NMHDR* pNMHDR, LRESULT* /*result*/)
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_SELCHANGED (NM_FIRST - 52)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the item checked event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_CHECKED notification message is sent to inform the
//     owner window that the user clicked the check box of an item of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_CHECKED, XTP_ID_GRID_CONTROL, OnGridCheckItem)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridCheckItem(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_CHECKED (NM_FIRST - 53)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the hyperlink clicked event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_HYPERLINK notification message is sent to inform the
//     owner window that the user clicked the hyperlink of an item.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_HYPERLINK, XTP_ID_GRID_CONTROL, OnGridHyperlinkClick)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridHyperlinkClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*) pNotifyStruct;
//
//     if (!pItemNotify->pRow || !pItemNotify->pColumn)
//         return;
//
//     // if click on Hyperlink in Item
//     if (pItemNotify->nHyperlink >= 0)
//     {
//         TRACE(_T("Hyperlink Click : \n row %d \n col %d \n Hyperlink %d.\n"),
//             pItemNotify->pRow->GetIndex(),
//             pItemNotify->pColumn->GetItemIndex(),
//             pItemNotify->nHyperlink);
//     }
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_HYPERLINK (NM_FIRST - 54)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the column order changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_COLUMNORDERCHANGED notification message is sent to inform the
//     owner window that the user changed the order of columns of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_COLUMNORDERCHANGED, XTP_ID_GRID_CONTROL,
//     OnGridColumnOrderChanged)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridColumnOrderChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDCOLUMNORDERCHANGED* pItemNotify = (XTP_NM_GRIDCOLUMNORDERCHANGED*)
//     pNotifyStruct; ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_GROUPORDERCHANGED, XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_COLUMNORDERCHANGED (NM_FIRST - 55)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the sort order changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_SORTORDERCHANGED notification message is sent to inform the
//     owner window that the user clicked the header of the Grid control
//     and changed the sort order.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_SORTORDERCHANGED, XTP_ID_GRID_CONTROL, OnGridSortOrderChanged)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridSortOrderChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     //pNotifyStruct doesn't consist information about column which was clicked.
//     //To get column use XTP_NM_GRID_COLUMNORDERCHANGED notification.
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_SORTORDERCHANGED (NM_FIRST - 56)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message when a column header has been clicked and the sort order
//     has changed, but CXTPGridControl::Populate has NOT yet been called.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_PRESORTORDERCHANGED notification message is sent to inform the
//     owner window that the user clicked the header of the Grid control and that
//     the sort order will change. XTP_NM_GRID_PRESORTORDERCHANGED is sent before the
//     CXTPGridControl::Populate method has been called. As such, the order of rows
//     has not been visibly changed yet. After the CXTPGridControl:Populate method is
//     called, the XTP_NM_GRID_SORTORDERCHANGED notification is sent to inform the
//     owner window that the visible order of rows has actually changed.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_VALUECHANGED, XTP_NM_GRID_SORTORDERCHANGED
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_PRESORTORDERCHANGED (NM_FIRST - 67)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the value changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_VALUECHANGED notification message is sent to inform the
//     owner window that the user changed a value of a grid cell.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// <code>
// BEGIN_MESSAGE_MAP(CPropertiesView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_VALUECHANGED, ID_GRID_CONTROL, OnGridValueChanged)
// END_MESSAGE_MAP()
//
// void CPropertiesView::OnGridValueChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     switch (pItemNotify->pItem->GetItemData())
//     {
//         case ID_PROPERTY_MULTIPLESELECTION:
//             GetTargetGrid()->SetMultipleSelection(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_PREVIEWMODE:
//             GetTargetGrid()->EnablePreviewMode(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             GetTargetGrid()->Populate();
//             break;
//         case ID_PROPERTY_GROUPBOXVISIBLE:
//             GetTargetGrid()->ShowGroupBy(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_FOCUSSUBITEMS:
//             GetTargetGrid()->FocusSubItems(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNREMOVE:
//             GetTargetGrid()->GetGridHeader()->AllowColumnRemove(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNREORDER:
//             GetTargetGrid()->GetGridHeader()->AllowColumnReorder(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNRESIZE:
//             GetTargetGrid()->GetGridHeader()->AllowColumnResize(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_FLATHEADER:
//             GetTargetGrid()->GetPaintManager()->m_columnStyle =
//                 (CRecordPropertyBool::GetValue(pItemNotify)) ? xtpGridColumnFlat :
//                 xtpGridColumnShaded;
//             break;
//         case ID_PROPERTY_HIDESELECTION:
//             GetTargetGrid()->GetPaintManager()->m_bHideSelection =
//                 (CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_TREEINDENT:
//             GetTargetGrid()->GetPaintManager()->m_nTreeIndent =
//                 CRecordPropertyInt::GetValue(pItemNotify);
//             GetTargetGrid()->RedrawControl();
//             break;
//     }
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_VALUECHANGED (NM_FIRST - 57)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the in-place button clicked event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_INPLACEBUTTONDOWN notification message is sent to inform the
//     owner window that the user clicked the in-place button of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Returns:
//     TRUE if this message was processed, FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_ID_GRID_CONTROL, OnGridButtonClick)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridButtonClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDINPLACEBUTTON* pItemNotify = (XTP_NM_GRIDINPLACEBUTTON*) pNotifyStruct;
//     ASSERT(pItemNotify->pButton);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_INPLACEBUTTONDOWN (NM_FIRST - 58)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the row expand changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_ROWEXPANDED notification message is sent to inform the
//     owner window that the user expanded or collapsed a row of the Grid control.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_ROWEXPANDED, XTP_ID_GRID_CONTROL, OnGridRowExpandChanged)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridRowExpandChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_ROWEXPANDED (NM_FIRST - 59)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the left mouse button clicked event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_LBUTTONDOWN notification message is sent to inform the
//     owner window that the user left clicked a row.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_LBUTTONDOWN, XTP_ID_GRID_CONTROL, OnGridLButtonDown)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridLButtonDown(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDRECORDITEM* pItemNotify = (XTP_NM_GRIDRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_LBUTTONDOWN (NM_FIRST - 60)
#	define XTP_NM_GRID_LBUTTONUP (NM_FIRST - 89)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridControl::GetItemMetrics() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_GETITEMMETRICS notification message is sent to allow the
//     owner window to customize item drawing.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_GETITEMMETRICS, XTP_ID_GRID_CONTROL, OnGridGetItemMetrics)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridGetItemMetrics(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDITEMMETRICS* pItemNotify = (XTP_NM_GRIDITEMMETRICS*)pNotifyStruct;
//
//     ASSERT(pItemNotify->pDrawArgs);
//     ASSERT(pItemNotify->pDrawArgs->pControl);
//     ASSERT(pItemNotify->pDrawArgs->pRow);
//
//     //     pItemNotify->pDrawArgs->pColumn   - may be NULL (for a group row)
//     //     pItemNotify->pDrawArgs->pItem     - may be NULL (for a group row)
//
//     ASSERT(pItemNotify->pItemMetrics);
//
//     // TODO: customize members of pItemNotify->pItemMetrics.
// }
// </code>
// See Also:
//     CXTPGridControl, CXTPGridControl::GetItemMetrics()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_GETITEMMETRICS (NM_FIRST - 61)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridControl::OnRequestEdit() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_REQUESTEDIT notification message is sent to notify the
//     owner window that an item has entered edit mode.
//     This message is send when an editable item starts to be edited or when a
//     check box item is checked/unchecked.
// See Also:
//     CXTPGridControl, CXTPGridControl::OnRequestEdit()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_REQUESTEDIT (NM_FIRST - 62)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridControl::OnBeforeCopyToText() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_BEFORE_COPY_TOTEXT notification message is sent to allow the
//     owner window to customize copy/paste operations.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_BEFORE_COPY_TOTEXT, XTP_ID_GRID_CONTROL, OnBeforeCopyToText)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnBeforeCopyToText(NMHDR*  pNotifyStruct, LRESULT* result)
// {
//  ASSERT(pNotifyStruct);
//
//  XTP_NM_GRID_BEFORE_COPYPASTE* pnmCopyPaste = (XTP_NM_GRID_BEFORE_COPYPASTE*)pNotifyStruct;
//
//  if (!pnmCopyPaste || !pnmCopyPaste->ppRecord || !*pnmCopyPaste->ppRecord ||
//      !pnmCopyPaste->parStrings)
//  {
//      ASSERT(FALSE);
//      return;
//  }
//
//  // customize data provided by pnmCopyPaste->parStrings
//  // you can use source record object: (*pnmCopyPaste->ppRecord)->Member()
//
//  // set result to not zero to cancel operation:
//  // *result = (LRESULT)TRUE;
// }
// </code>
// See Also:
//     CXTPGridControl, CXTPGridControl::OnBeforeCopyToText()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_BEFORE_COPY_TOTEXT (NM_FIRST - 63)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridControl::OnBeforePasteFromText() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_BEFORE_PASTE_FROMTEXT notification message is sent to allow the
//     owner window to customize copy/paste operations.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_BEFORE_PASTE_FROMTEXT, XTP_ID_GRID_CONTROL,
//     OnGridBeforePasteFromText)
// END_MESSAGE_MAP()
//
//  void CGridSampleView::OnGridBeforePasteFromText(NMHDR*  pNotifyStruct, LRESULT* result)
//  {
//      ASSERT(pNotifyStruct);
//
//      XTP_NM_GRID_BEFORE_COPYPASTE* pnmCopyPaste =
//      (XTP_NM_GRID_BEFORE_COPYPASTE*)pNotifyStruct;
//
//      if (!pnmCopyPaste || !pnmCopyPaste->ppRecord || !pnmCopyPaste->parStrings) {
//          ASSERT(FALSE);
//          return;
//      }
//
//      CMessageRecord* pRecord = new CMessageRecord();
//      if (!pRecord) {
//          return;
//      }
//
//      *pnmCopyPaste->ppRecord = pRecord;
//
//      CXTPGridColumns* pColumns = GetGridCtrl().GetColumns();
//      ASSERT(pColumns);
//      if (!pColumns) {
//          return;
//      }
//
//      int nDataCount = pnmCopyPaste->parStrings->GetSize();
//
//      const int nColumnCount = pColumns->GetVisibleColumnsCount();
//      for (int nCol = 0; nCol < nColumnCount; nCol++)
//      {
//          CXTPGridColumn* pColumn = pColumns->GetVisibleAt(nCol);
//          CXTPGridRecordItem* pItem = pRecord->GetItem(pColumn);
//          ASSERT(pItem);
//          if (NULL == pItem)
//              continue;
//
//          if (nCol < nDataCount)
//          {
//              CString strItem = pnmCopyPaste->parStrings->GetAt(nCol);
//              pItem->SetCaption(strItem);
//          }
//      }
//      // set result to not zero to cancel operation:
//      // *result = (LRESULT)TRUE;
//  }
// </code>
// See Also:
//     CXTPGridControl, CXTPGridControl::OnBeforePasteFromText()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_BEFORE_PASTE_FROMTEXT (NM_FIRST - 64)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridControl::OnBeforePaste() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_BEFORE_PASTE notification message is sent to allow the
//     owner window to customize copy/paste operations.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_BEFORE_PASTE, XTP_ID_GRID_CONTROL, OnBeforePaste)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnBeforePaste(NMHDR*  pNotifyStruct, LRESULT* result)
// {
//  ASSERT(pNotifyStruct);
//
//  XTP_NM_GRID_BEFORE_COPYPASTE* pnmCopyPaste = (XTP_NM_GRID_BEFORE_COPYPASTE*)pNotifyStruct;
//  ASSERT(pnmCopyPaste && pnmCopyPaste->parStrings == NULL);
//
//  if (!pnmCopyPaste || !pnmCopyPaste->ppRecord || !*pnmCopyPaste->ppRecord) {
//      ASSERT(FALSE);
//      return;
//  }
//
//  // you can create new or change provided record using pnmCopyPaste->ppRecord
//
//  // set result to not zero to cancel operation:
//  // *result = (LRESULT)TRUE;
// }
// </code>
// See Also:
//     CXTPGridControl, CXTPGridControl::OnBeforePaste()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_BEFORE_PASTE (NM_FIRST - 65)

//{{AFX_CODEJOCK_PRIVATE
// Internal Trace operation
#	define XTP_TRACE
//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
#		define XTP_NM_GRID_VALUECHANGING (NM_FIRST - 66)
//}}AFX_CODEJOCK_PRIVATE
#	endif

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridHeader::AdjustColumnsWidth() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_COLUMNWIDTHCHANGED notification message is sent when
//     the width of a column is changed. This can happen when the user changes
//     the width or when the Grid control automatically changes the width.
// See Also:
//     CXTPGridHeader, CXTPGridHeader::AdjustColumnsWidth
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_COLUMNWIDTHCHANGED (NM_FIRST - 68)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the CXTPGridControl::OnPreviewKeyDown() method.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     This notification is sent to the Main window in a WM_NOTIFY message before
//     processing the OnKeyDown event.
// See Also:
//     CXTPGridControl::OnPreviewKeyDown, XTP_NM_GRIDPREVIEWKEYDOWN,
//     CXTPGridControl::OnKeyDown, CWnd::OnKeyDown.
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_PREVIEWKEYDOWN (NM_FIRST - 69)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that the user started dragging records.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDDRAGDROP structure is passed with this message.
// See Also:
//     XTP_NM_GRIDDRAGDROP, XTP_NM_GRID_BEGINDRAG, XTP_NM_GRID_DROP,
//     XTP_NM_GRID_RECORDS_DROPPED, XTP_NM_GRID_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_BEGINDRAG (NM_FIRST - 70)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that the user dropped records to the Grid control.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDDRAGDROP structure is passed with this message.
// See Also:
//     XTP_NM_GRIDDRAGDROP, XTP_NM_GRID_BEGINDRAG,
//     XTP_NM_GRID_RECORDS_DROPPED, XTP_NM_GRID_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_DROP (NM_FIRST - 71)
#	define XTP_NM_GRID_HASVALIDDROPTYPE (NM_FIRST - 85)
#	define XTP_NM_GRID_VALIDDROPTARGET (NM_FIRST - 86)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the in-place editing has been canceled and
//     the value has not been changed event.
// Parameters:
//     id        -  Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDRECORDITEM structure is passed with this message.
// See Also:
//     XTP_NM_GRIDRECORDITEM, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridRecordItem::OnEditCanceled
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_EDIT_CANCELED (NM_FIRST - 72)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that the user changed a selected item in a combobox.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// See Also:
//     CXTPGridControl::OnConstraintSelecting()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_CONSTRAINT_SELECTING (NM_FIRST - 73)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that a tooltip will be drawn.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// See Also:
//     CXTPGridControl::OnGetToolTipInfo()
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_GETTOOLTIPINFO (NM_FIRST - 74)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that records were dropped to the Grid control.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDDRAGDROP structure is passed with this message.
// See Also:
//     XTP_NM_GRIDDRAGDROP, XTP_NM_GRID_BEGINDRAG, XTP_NM_GRID_DROP,
//     XTP_NM_GRID_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_RECORDS_DROPPED (NM_FIRST - 75)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for handling the focus changing event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_FOCUS_CHANGING notification message is sent to inform the
//     owner window that the focused row or column is about to be changed.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_FOCUS_CHANGING, XTP_ID_GRID_CONTROL, OnGridFocusChanging)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridFocusChanging(NMHDR* pNMHDR, LRESULT* /*result*/)
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN,
//     XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_FOCUS_CHANGED
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_FOCUS_CHANGING (NM_FIRST - 76)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that the user completed a drag-and-drop operation.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDDRAGDROP structure is passed with this message.
// See Also:
//     XTP_NM_GRIDDRAGDROP, XTP_NM_GRID_BEGINDRAG, XTP_NM_GRID_DROP,
//     XTP_NM_GRID_RECORDS_DROPPED
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_DRAGDROP_COMPLETED (NM_FIRST - 77)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for starting in-place editing.
//     Sent after XTP_NM_GRID_REQUESTEDIT and when the in-place editor
//     has been created.
// Parameters:
//     id        -  Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDRECORDITEM structure is passed with this message.
// See Also:
//     XTP_NM_GRIDRECORDITEM, XTP_NM_GRID_VALUECHANGED,
//     XTP_NM_GRID_REQUESTEDIT,
//     CXTPGridRecordItem::OnEditCanceled
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_BEGINEDIT (NM_FIRST - 78)

#	ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
#		define XTP_NM_GRID_EDIT_CHANGING (NM_FIRST - 79)
//}}AFX_CODEJOCK_PRIVATE
#	endif

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for changing the selected rows collection.
//     Sent for each row when it is added/removed.
// Parameters:
//     id        -  Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_SELECTION_CHANGING structure is passed with this message.
// See Also:
//     XTP_NM_SELECTION_CHANGING
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_SELCHANGING (NM_FIRST - 80)

//-----------------------------------------------------------------------
// Summary:
//     Indicates that the user is dragging records over a control.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     An XTP_NM_GRIDDRAGDROP structure is passed with this message.
// See Also:
//     XTP_NM_GRIDDRAGDROP, XTP_NM_GRID_BEGINDRAG, XTP_NM_GRID_DROP,
//     XTP_NM_GRID_RECORDS_DROPPED, XTP_NM_GRID_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_DRAGOVER (NM_FIRST - 81)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the column group order changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_GROUPORDERCHANGED notification message is sent to inform the
//     owner window that the user changed the columns group order of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_GROUPORDERCHANGED, XTP_ID_GRID_CONTROL, OnGridGroupOrderChanged)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridGroupOrderChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDCOLUMNORDERCHANGED* pItemNotify = (XTP_NM_GRIDCOLUMNORDERCHANGED*)
//     pNotifyStruct; ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRID_CHECKED, XTP_NM_GRID_HEADER_RCLICK,
//     XTP_NM_GRID_HYPERLINK, XTP_NM_GRID_INPLACEBUTTONDOWN, XTP_NM_GRID_SELCHANGED,
//     XTP_NM_GRID_COLUMNORDERCHANGED, XTP_NM_GRID_SORTORDERCHANGED, XTP_NM_GRID_VALUECHANGED,
//     CXTPGridControl, CXTPGridHeader
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_GROUPORDERCHANGED (NM_FIRST - 82)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the item button click event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_ITEMBUTTONCLICK notification message is sent to inform the
//     owner window that the user clicked an item button of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Returns:
//     TRUE if this message was processed, FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_ITEMBUTTONCLICK, XTP_ID_GRID_CONTROL, OnGridItemButtonClick)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridItemButtonClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDITEMCONTROL* pItemNotify = (XTP_NM_GRIDITEMCONTROL*) pNotifyStruct;
//     ASSERT(pItemNotify->pItemControl);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_GRIDITEMCONTROL
//     CXTPGridRecordItemControl, CXTPGridRecordItemButton
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_ITEMBUTTONCLICK (NM_FIRST - 83)

//-----------------------------------------------------------------------
// Summary:
//     Defines the message for the header column click on PlusMinus icon event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_GRID_PLUSMINUSCLICK notification message is sent to inform the
//     owner window that the user clicked a header column PlusMinus icon
//     of the Grid control.
//     The owner window of the Grid control receives this notification through
//     the WM_COMMAND message.
// Returns:
//     TRUE if this message was processed, FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_PLUSMINUSCLICK, XTP_ID_GRID_CONTROL, OnGridPlusMinusClick)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridPlusMinusClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDITEMCONTROL* pItemNotify = (XTP_NM_GRIDITEMCONTROL*) pNotifyStruct;
//     ASSERT(pItemNotify->pItemControl);
//     ASSERT(pItemNotify->pColumn);
//     CPoint ptClick = pItemNotify->pt;
//
//     // TODO: Handle command.
// }
// </code>
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_PLUSMINUSCLICK (NM_FIRST - 84)

//-----------------------------------------------------------------------
// Summary:
//     This message is sent by the Grid control when the state of an item
//     or range of items has changed. It is sent in the form of a WM_NOTIFY message.
// Parameters:
//     lpStateChange - Long pointer to an XTP_NM_GRIDSTATECHANGED structure that
//                     contains data about the item or items that have changed.
// Remarks:
//     The XTP_NM_GRID_STATECHANGED notification message is sent to inform the
//     owner window that the state of an item or range of items has changed.
// Returns:
//     The application receiving this message must return zero.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CGridSampleView, CXTPGridView)
//     ON_NOTIFY(XTP_NM_GRID_STATECHANGED, XTP_ID_GRID_CONTROL, OnGridStateChanged)
// END_MESSAGE_MAP()
//
// void CGridSampleView::OnGridStateChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_GRIDSTATECHANGED* pItemNotify = (XTP_NM_GRIDSTATECHANGED*)pNotifyStruct;
//     int nBegin = nm.nBegin; // first item in the range
//     int nEnd = nm.nEnd; // last item in the range
//
//     // TODO: Handle command.
// }
// </code>
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_STATECHANGED (NM_FIRST - 88)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_GRID_HSCROLL message is sent when the horizontal
//     scrolling position has changed.
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_HSCROLL (NM_FIRST - 90)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_GRID_VSCROLL message is sent when the vertical
//     scrolling position has changed.
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_VSCROLL (NM_FIRST - 91)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_GRID_MOUSEMOVE message is sent when the
//     mouse is moved over the control.
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_MOUSEMOVE (NM_FIRST - 92)

#	define XTP_NM_GRID_GIVEFEEDBACK (NM_FIRST - 93)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_GRID_ROWHEIGHTCHANGED notification message is sent
//     when the height of a row has changed. This can happen when the user
//     changes the height.
//-----------------------------------------------------------------------
#	define XTP_NM_GRID_ROWHEIGHTCHANGED (NM_FIRST - 94)

//{{AFX_CODEJOCK_PRIVATE
#	define XTP_NM_GRID_ROWHEIHGTCHANGED XTP_NM_GRID_ROWHEIGHTCHANGED
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     Enables deprecated Grid control methods.
//-----------------------------------------------------------------------
#	define XTP_GRID_DEPRECATED() (1)

class CXTPGridRow;

//-----------------------------------------------------------------------
// Summary:
//     Defines a function pointer for comparing events.
// Remarks:
//     This function pointer is used in the SortEx method.
// See Also:
//     Sort, SortEx, CXTPGridControl::SetRowsCompareFunc
//-----------------------------------------------------------------------
typedef int(AFX_CDECL* XTPGridRowsCompareFunc)(const CXTPGridRow** pRow1,
											   const CXTPGridRow** pRow2);

#	define XTP_DECLARE_PROPERTY(property, type)                                                   \
		afx_msg type OleGet##property();                                                           \
		afx_msg void OleSet##property(type);

#	define XTP_DISP_PROPERTY_EX_ID(theClass, szExternalName, dispid, vtPropType)                  \
		DISP_PROPERTY_EX_ID(theClass, #szExternalName, dispid, OleGet##szExternalName,             \
							OleSet##szExternalName, vtPropType)

#	define XTP_IMPLEMENT_PROPERTY(theClass, theProperty, theMember, theType)                      \
		theType theClass::OleGet##theProperty()                                                    \
		{                                                                                          \
			return theMember;                                                                      \
		}                                                                                          \
		void theClass::OleSet##theProperty(theType set)                                            \
		{                                                                                          \
			theMember = set;                                                                       \
		}

#	define XTP_IMPLEMENT_PROPERTY_COLOR(theClass, theProperty, theMember)                         \
		COLORREF theClass::OleGet##theProperty()                                                   \
		{                                                                                          \
			return theMember;                                                                      \
		}                                                                                          \
		void theClass::OleSet##theProperty(COLORREF oleColor)                                      \
		{                                                                                          \
			theMember = AxTranslateColor(oleColor);                                                \
		}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDDEFINES_H__)
