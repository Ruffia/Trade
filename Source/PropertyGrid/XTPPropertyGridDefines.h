// XTPPropertyGridDefines.h
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
#if !defined(__XTPPROPERTYGRIDDEFINES_H__)
#	define __XTPPROPERTYGRIDDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

// Added for Grid theme constants backward compatibility
#	include "GridControl/XTPGridDefines.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//-----------------------------------------------------------------------
// Summary:
//     XTPPropertyGridSortOrder is an enumeration used by CXTPPropertyGrid to
//     determine the grouping style. This will specify how the items in the grid
//     are displayed (i.e. alphabetical, not sorted, and grouped by category).
// See Also:
//     CXTPPropertyGrid::SetPropertySort, CXTPPropertyGrid::GetPropertySort
// Example:
//     <code>XTPPropertyGridSortOrder sort = m_wndPropertyGrid.GetPropertySort();</code>
//-----------------------------------------------------------------------
enum XTPPropertyGridSortOrder
{
	xtpPropertyGridSortCategorized,  // Group items by category.
	xtpPropertyGridSortAlphabetical, // Sort items alphabetically.
	xtpPropertyGridSortNoSort,		 // Disable sorting and grouping; all items are displayed in
									 // the order that they are added to the grid.

};

//{{AFX_CODEJOCK_PRIVATE
_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridSortCategorized)
const XTPPropertyGridSortOrder xtpGridSortCategorized = xtpPropertyGridSortCategorized;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridSortAlphabetical)
const XTPPropertyGridSortOrder xtpGridSortAlphabetical = xtpPropertyGridSortAlphabetical;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridSortNoSort)
const XTPPropertyGridSortOrder xtpGridSortNoSort = xtpPropertyGridSortNoSort;
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     Visual Theme enumeration.
// Example:
//     <code> m_wndPropertyGrid.SetTheme(xtpPropertyGridThemeVisualStudio2005); </code>
// See Also:
//     CXTPPropertyGrid::SetTheme, CXTPPropertyGrid::GetCurrentTheme
//-----------------------------------------------------------------------
enum XTPPropertyGridPaintTheme
{
	xtpPropertyGridThemeDefault,									 // Default theme.
	xtpPropertyGridThemeOfficeXP,									 // Office XP style theme.
	xtpPropertyGridThemeOffice2003,									 // Office 2003 style theme.
	xtpPropertyGridThemeOffice2013,									 // Office 2013 style theme.
	xtpPropertyGridThemeOffice2016 = xtpPropertyGridThemeOffice2013, // Office 2016 style theme.
	xtpPropertyGridThemeVisualStudio2012Light, // Visual Studio 2012 Light style theme.
	xtpPropertyGridThemeVisualStudio2012Dark,  // Visual Studio 2012 Dark style theme.
	xtpPropertyGridThemeVisualStudio2015,	  // Visual Studio 2015 Light style theme.
	xtpPropertyGridThemeResource,			   // Office 2007 / 2010 style theme.
	xtpPropertyGridThemeNativeWindows10,	   // Native Windows 10 style theme.
	xtpPropertyGridThemeVisualStudio2017,	  // Visual Studio 2017 style theme.
	xtpPropertyGridThemeVisualStudio2019,	  // Visual Studio 2019 style theme.
	xtpPropertyGridThemeVisualStudio2022,	  // Visual Studio 2022 style theme.

	xtpPropertyGridThemeNativeWinXP = 100, // WinXP style theme.
	xtpPropertyGridThemeCool,			   // Cool theme.
	xtpPropertyGridThemeSimple,			   // Visual Basic style theme.
	xtpPropertyGridThemeDelphi,			   // Delphi style theme.
	xtpPropertyGridThemeVisualStudio2005,  // Visual Studio 2005 style theme.
	xtpPropertyGridThemeVisualStudio2010,  // Visual Studio 2010 style theme.

	//{{AFX_CODEJOCK_PRIVATE
	xtpPropertyGridThemeWhidbey	= xtpPropertyGridThemeVisualStudio2005,
	xtpPropertyGridThemeOffice2007 = xtpPropertyGridThemeResource,
	//}}AFX_CODEJOCK_PRIVATE
};

//{{AFX_CODEJOCK_PRIVATE
_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeNativeWinXP)
const XTPPropertyGridPaintTheme xtpGridThemeNativeWinXP = xtpPropertyGridThemeNativeWinXP;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeCool)
const XTPPropertyGridPaintTheme xtpGridThemeCool = xtpPropertyGridThemeCool;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeSimple)
const XTPPropertyGridPaintTheme xtpGridThemeSimple = xtpPropertyGridThemeSimple;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeDelphi)
const XTPPropertyGridPaintTheme xtpGridThemeDelphi = xtpPropertyGridThemeDelphi;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeVisualStudio2005)
const XTPPropertyGridPaintTheme xtpGridThemeVisualStudio2005 = xtpPropertyGridThemeVisualStudio2005;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeVisualStudio2010)
const XTPPropertyGridPaintTheme xtpGridThemeVisualStudio2010 = xtpPropertyGridThemeVisualStudio2010;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeVisualStudio2005)
const XTPPropertyGridPaintTheme xtpGridThemeWhidbey = xtpPropertyGridThemeVisualStudio2005;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridThemeResource)
const XTPPropertyGridPaintTheme xtpGridThemeOffice2007 = xtpPropertyGridThemeResource;
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     Border Style enumeration.
// Example:
//     <code>m_wndPropertyGrid.SetBorderStyle(xtpPropertyGridBorderClientEdge);</code>
// See Also:
//     CXTPPropertyGrid::SetBorderStyle
//-----------------------------------------------------------------------
enum XTPPropertyGridBorderStyle
{
	xtpPropertyGridBorderNone,		 // Without borders.
	xtpPropertyGridBorderFlat,		 // Black flat borders.
	xtpPropertyGridBorderStaticEdge, // Static edge.
	xtpPropertyGridBorderClientEdge, // Client edge.
};

//{{AFX_CODEJOCK_PRIVATE
_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridBorderNone)
const XTPPropertyGridBorderStyle xtpGridBorderNone = xtpPropertyGridBorderNone;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridBorderFlat)
const XTPPropertyGridBorderStyle xtpGridBorderFlat = xtpPropertyGridBorderFlat;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridBorderStaticEdge)
const XTPPropertyGridBorderStyle xtpGridBorderStaticEdge = xtpPropertyGridBorderStaticEdge;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridBorderClientEdge)
const XTPPropertyGridBorderStyle xtpGridBorderClientEdge = xtpPropertyGridBorderClientEdge;
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     In-place Button Style enumeration.
// Example:
//     pPaintManager->m_buttonsStyle = xtpPropertyGridButtonsOfficeXP;
// See Also:
//     CXTPPropertyGrid::SetTheme, CXTPPropertyGrid::GetCurrentTheme
//-----------------------------------------------------------------------
enum XTPPropertyGridInplaceButtonsStyle
{
	xtpPropertyGridButtonsDefault,  // Default button style.
	xtpPropertyGridButtonsThemed,   // Themed button style.
	xtpPropertyGridButtonsOfficeXP, // Office XP button style.
};

//{{AFX_CODEJOCK_PRIVATE
_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridButtonsDefault)
const XTPPropertyGridInplaceButtonsStyle xtpGridButtonsDefault = xtpPropertyGridButtonsDefault;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridButtonsThemed)
const XTPPropertyGridInplaceButtonsStyle xtpGridButtonsThemed = xtpPropertyGridButtonsThemed;

_XTP_DEPRECATED_IN_FAVOR(xtpPropertyGridButtonsOfficeXP)
const XTPPropertyGridInplaceButtonsStyle xtpGridButtonsOfficeXP = xtpPropertyGridButtonsOfficeXP;
//}}AFX_CODEJOCK_PRIVATE

class CXTPPropertyGridItem;

//-----------------------------------------------------------------------
// Summary:
//     The XTPWM_PROPERTYGRID_NOTIFY message is sent to the CXTPPropertyGrid
//     owner window whenever an action occurs within the CXTPPropertyGrid.
//
// <code>XTPWM_PROPERTYGRID_NOTIFY
// nGridAction = (int) wParam;              // Property grid action
// pItem = (CXTPPropertyGridItem*) lParam;  // pointer to a CXTPPropertyGridItem object</code>
//
//     When the user performs an action in the property grid, the XTPWM_PROPERTYGRID_NOTIFY
//     message is sent to the property grid's owner window.
// Parameters:
//     nGridAction - Value of wParam that specifies a value that indicates the user's request.
//     pItem       - Value of lParam that points to a CXTPPropertyGridItem object
//                   that contains information for the specified item.
//                   This pointer should <b>never</b> be NULL.
// Returns:
//     If the application is to process this message, the return value should be TRUE.
//     Otherwise, the return value should be FALSE.
// Remarks:
//     The nGridAction parameter can be one of the following values:
//     * <b>XTP_PGN_SORTORDER_CHANGED</b>: The sort order has changed in the property grid.
//     * <b>XTP_PGN_SELECTION_CHANGED</b>: The selection has changed in the property grid.
//     * <b>XTP_PGN_ITEMVALUE_CHANGED</b>: The value has changed for pItem in the
//                                         property grid.
//     * <b>XTP_PGN_EDIT_CHANGED</b>: The edit value has changed in the property grid.
//     * <b>XTP_PGN_INPLACEBUTTONDOWN</b>: The item's in-place button (combo/expand) is down.
//     * <b>XTP_PGN_ITEMEXPANDCHANGED</b>: The user expanded or collapsed the item.
//     * <b>XTP_PGN_DBLCLICK</b>: The user double-clicked the left mouse button in the
//                                property grid.
//     * <b>XTP_PGN_RCLICK</b>: The user pressed the right mouse button in the property grid.
//     * <b>XTP_PGN_VERB_CLICK</b>: The user clicked a verb in the property grid.
//     * <b>XTP_PGN_REQUESTEDIT</b>: The user is changing the value of the item.
//
// Example:
//     Here is an example of how an application would process the
//     XTPWM_PROPERTYGRID_NOTIFY message:
//
// <code>
// int nGridAction = (int)wParam;
//
//
// switch (nGridAction)
// {
//     case XTP_PGN_SORTORDER_CHANGED:
//           {
//              m_nSort = m_wndXTPPropertyGrid.GetPropertySort();
//              UpdateData(FALSE);
//           }
//         break;
//     case XTP_PGN_ITEMVALUE_CHANGED:
//           {
//              CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
//              TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"),
//                  pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
//           }
//         break;
//     case XTP_PGN_SELECTION_CHANGED:
//           {
//              CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
//              TRACE(_T("Selection Changed. Item = %s\n"), pItem->GetCaption());
//           }
//     case XTP_PGN_INPLACEBUTTONDOWN:
//           {
//              CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
//           }
//         break;
// }
// return FALSE;
// </code>
//-----------------------------------------------------------------------
const UINT XTPWM_PROPERTYGRID_NOTIFY = (WM_USER + 9950 + 1);

const UINT XTP_PGN_SORTORDER_CHANGED = 1;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_SELECTION_CHANGED = 2;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_ITEMVALUE_CHANGED = 3;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_EDIT_CHANGED		 = 4;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_INPLACEBUTTONDOWN = 5;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_DRAWITEM			 = 6;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_ITEMEXPANDCHANGED = 7;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_DBLCLICK			 = 8;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_RCLICK			 = 9;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_VERB_CLICK		 = 10; //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>

const UINT XTP_PGN_REQUESTEDIT = 11; //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_AFTEREDIT   = 12; //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_CANCELEDIT  = 13; //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_USER		   = 14; //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>

//===========================================================================
// Summary:
//     This struct is passed with XTP_PGN_REQUESTEDIT, XTP_PGN_AFTEREDIT, and
//     XTP_PGN_CANCELEDIT notifications to determine the item and its new value.
// See Also:
//     XTP_PGN_REQUESTEDIT
//===========================================================================
struct XTP_PROPERTYGRID_NOTIFYEDIT
{
	BOOL bCancel;				 // TRUE to cancel user edit.
	CString strValue;			 // New caption value to be set.
	CXTPPropertyGridItem* pItem; // Item to be edited.
};

//-----------------------------------------------------------------------
// Summary:
//     Apply this style to the property grid to use the owner draw feature.
// See Also:
//     CXTPPropertyGrid::Create
// Example:
//     The following example illustrates using XTP_PGS_OWNERDRAW:
// <code>
// m_wndPropertyGrid.ModifyStyle(0, XTP_PGS_OWNERDRAW);
// </code>
//-----------------------------------------------------------------------
const UINT XTP_PGS_OWNERDRAW = 0x0010L;

//-----------------------------------------------------------------------
// Summary:
//     A NULL-terminated string used by CXTPPropertyGrid which specifies
//     the window class name for the property grid.
// Example:
// <code>
// BOOL CXTPPropertyGrid::RegisterWindowClass()
// {
//     WNDCLASS wndcls;
//     HINSTANCE hInst = XTPGetInstanceHandle();
//
//     if (!(::GetClassInfo(hInst, XTPPROPERTYGRIDCTRL_CLASSNAME, &wndcls)))
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
//         wndcls.lpszClassName    = XTPPROPERTYGRIDCTRL_CLASSNAME;
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
//     CXTPPropertyGrid::RegisterWindowClass
//-----------------------------------------------------------------------
const TCHAR XTPPROPERTYGRID_CLASSNAME[] = _T("XTPPropertyGrid");

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#define __XTPPROPERTYGRIDDEFINES_H__
