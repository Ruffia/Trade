// XTPTabPaintManagerTheme.h
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
#if !defined(__XTPTABPAINTMANAGERTHEME_H__)
#	define __XTPTABPAINTMANAGERTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTabPaintManagerColorSet;
class CXTPTabManager;
class CXTPTabManagerItem;
class CXTPTabPaintManager;

//===========================================================================
// Summary:
//     CXTPTabPaintManagerTheme is a class that represents the appearance of all
//     components of the tab manager (e.g. tab button, tab caption font,
//     button size, tab margins, etc.).
// Remarks:
//     CXTPTabPaintManagerTheme is the base class for all appearances.
//     To create a custom appearance, a new class can be derived
//     either from this class or any of the appearances.
//
//     The appearance should be updated in RefreshMetric's members
//     for each CXTPTabPaintManagerTheme object.
// See Also:
//     XTPTabAppearanceStyle, SetAppearance, SetAppearanceSet, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet,
//     CXTPTabPaintManagerColorSet
//===========================================================================
class _XTP_EXT_CLASS CXTPTabPaintManagerTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManagerTheme object.
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManagerTheme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabPaintManagerTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the appearance metrics.
	//     Override this member to change the appearance metrics.
	// Remarks:
	//     This method is used to refresh the appearance metrics
	//     such as font, button height, margin size, etc.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the color set.
	// Returns:
	//     A pointer to the color set.
	// See Also:
	//     XTPTabColorStyle, CXTPTabPaintManager::GetColor,
	//     CXTPTabPaintManager::SetColorSet, CXTPTabPaintManager::SetColor
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* GetColorSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab header area.
	// Parameters:
	//     rcControl   - Bounding rectangle of the tab header.
	//                   See CXTPTabManager::GetControlRect.
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The bounding rectangle of the tab header area.
	// See Also:
	//     GetClientRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the clip box of the tab header area.
	// Parameters:
	//     pDC         - Pointer to a CDC object.
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The clip box of the tab header area.
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderClipBox(CDC* pDC, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the tab header area.
	// Parameters:
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The height of the tab header area.
	// See Also:
	//     GetHeaderRect, GetClientRect, GetHeaderMargin, GetClientMargin,
	//     CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	int GetHeaderHeight(CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle of the tab client area.
	// Parameters:
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	//     pTabManager - Pointer to a CXTPTabManager object.
	// Returns:
	//     The bounding rectangle of the tab client area.
	// See Also:
	//     GetHeaderRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	virtual CRect GetClientRect(CRect rcControl, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the top, left, right, and bottom margins of the tab header.
	// Returns:
	//     The top, left, right, and bottom margins of the tab header.
	// Remarks:
	//     The margins of the tab header are the space placed around
	//     the tab buttons inside the tab header.
	//
	//     The margins of the tab header are not placed around each
	//     tab button. Instead, they are placed around all tab buttons.
	// Example:
	// <code>
	// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
	// // place a margin of 5 pixels around the tab buttons in the tab header.
	// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5, 5, 5, 5);
	//
	// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
	// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab
	// // client area).
	// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4, 4, 4, 4);
	//
	// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
	// // place a margin of 2 pixels around the text and icon in the tab buttons.
	// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2, 2, 2, 2);
	//
	// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
	// // place a margin of 10 pixels around the tab client area.
	// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10, 10, 10, 10);
	// </code>
	// See Also:
	//     m_rcHeaderMargin, GetHeaderRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderMargin();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the top, left, right, and bottom margins of the tab client area.
	// Returns:
	//     The top, left, right, and bottom margins of the tab client area.
	// Remarks:
	//     The margins of the tab client area are the space placed around
	//     the tab client area inside the tab control.
	// Example:
	// <code>
	// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
	// // place a margin of 5 pixels around the tab buttons in the tab header.
	// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5, 5, 5, 5);
	//
	// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
	// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab
	// // client area).
	// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4, 4, 4, 4);
	//
	// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
	// // place a margin of 2 pixels around the text and icon in the tab buttons.
	// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2, 2, 2, 2);
	//
	// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
	// // place a margin of 10 pixels around the tab client area.
	// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10, 10, 10, 10);
	// </code>
	// See Also:
	//     CXTPTabPaintManager::m_rcClientMargin, GetClientRect
	//-----------------------------------------------------------------------
	virtual CRect GetClientMargin();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the total length of a specified tab button
	//     including the left and right button margins.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab button with the length to be retrieved.
	// Returns:
	//     The total length of the specified tab button including
	//     the left and right button margins.
	// Remarks:
	//     This method calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     with bDraw = FALSE to get the total length of the specified tab button
	//     including the left and right button margins.
	// See Also:
	//     CXTPTabPaintManager::m_rcButtonMargin, GetClientMargin, GetHeaderMargin,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText,
	//     CXTPTabManager::GetItemMetrics, GetButtonHeight
	//-----------------------------------------------------------------------
	virtual int GetButtonLength(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the total height of the tab buttons within a specified tab manager
	//     including the top and bottom margins and the icon.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to retrieve
	//                   the total height of the tab buttons within.
	// Returns:
	//     The height of the tab buttons within the specified tab manager.
	// Remarks:
	//     This method adds the size of the top and bottom margins and
	//     the size of the icon to the size of
	//     CXTPTabPaintManager.CXTPTabPaintManagerTheme.m_nButtonHeight
	//     which is the height of
	//     the tab buttons without the top or bottom margins or the icon.
	// See Also:
	//     CXTPTabPaintManager.CXTPTabPaintManagerTheme.m_nButtonHeight,
	//     CXTPTabPaintManager::m_rcButtonMargin, GetClientMargin,
	//     GetHeaderMargin, CXTPTabPaintManager::DrawSingleButtonIconAndText,
	//     CXTPTabManager::GetItemMetrics, GetButtonLength
	//-----------------------------------------------------------------------
	virtual int GetButtonHeight(const CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the bounding rectangle of a specified drawing item.
	// Parameters:
	//     pItem - Pointer to the drawing item with
	//             the bounding rectangle to retrieve.
	// Returns:
	//     The bounding rectangle of the specified drawing item.
	// See Also:
	//     CXTPTabManagerItem::GetRect
	//-----------------------------------------------------------------------
	virtual CRect GetButtonDrawRect(const CXTPTabManagerItem* pItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     This method is called during various Draw and Paint functions.
	//
	//     Unless overridden, this method will just call
	//     CXTPTabPaintManager::DrawTabControlEx.
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx.
	//-----------------------------------------------------------------------
	virtual void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills both the tab header and the tab client area and then
	//     draws the tab client frame.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to fill.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	// Returns:
	//     The bounding rectangle of the tab header.
	// Remarks:
	//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client space
	//     is filled using CXTPTabPaintManagerColorSet::m_clrControlFace.
	//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
	//     docking pane panel auto-hide tabs because there is no client area to draw.
	// See Also:
	//     GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame,
	//     CXTPTabPaintManager::m_clientFrame
	//-----------------------------------------------------------------------
	virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the tab buttons of the tab control to their correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager that the tab buttons are on.
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Bounding rectangle of the tab client area.
	// Remarks:
	//     Both CXTPTabPaintManager::AdjustClientRect and
	//     CXTPTabPaintManager::RepositionTabControl are called when the
	//     window is resized. When CXTPTabPaintManager::DrawTabControl
	//     is called, the changes will be used when drawing the tab control.
	//
	//     This method is called during CXTPTabPaintManager::RepositionTabControl.
	//
	//     Unless it is overridden, this method will only call
	//     CXTPTabPaintManager::RepositionTabControlEx.
	//-----------------------------------------------------------------------
	virtual void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts the tab client rectangle.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to adjust the tab client rectangle.
	//     rcClient    - [out] Reference to a CRect object to receive
	//                   the new tab client rectangle with margins included.
	// Remarks:
	//     Both CXTPTabPaintManager::AdjustClientRect and
	//     CXTPTabPaintManager::RepositionTabControl are called when the
	//     window is resized. When CXTPTabPaintManager::DrawTabControl
	//     is called, the changes will be used when drawing the tab control.
	//
	//     This method will adjust the tab client rectangle based on
	//     the currently set XTPTabClientFrame.
	//
	//     CXTPTabPaintManager::AdjustClientRect calls this method if
	//     there is one or more tabs in the tab manager.
	// See Also:
	//     CXTPTabPaintManager::AdjustClientRect
	//-----------------------------------------------------------------------
	virtual void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a single tab button in the tab client header area.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the tab button to draw.
	// Remarks:
	//     This method is only used to draw the tab button. This method does not
	//     draw the button's icon or text.
	//
	//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     after the button is drawn to draw the button's icon and text.
	// See Also:
	//     CXTPTabPaintManager::DrawTabControlEx,
	//     CXTPTabPaintManager::DrawSingleButtonIconAndText
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a specified TabMDIClient TabWorkspace part.
	// Parameters:
	//     pDC            - Pointer to a valid device context.
	//     lpRect         - Size of the splitter.
	//     workspacePart  - TabWorkspace part to be drawn; one of the values
	//                      defined by the XTPTabWorkspacePart enumeration.
	// Remarks:
	//     This method is used when drawing the TabWorkspace of
	//     MDI tabs when tab grouping is enabled.
	//
	//     If workspacePart is XTPTabWorkspacePartHSplitter, then lpRect refers to
	//     the size of the splitter placed between horizontal tab groups.
	//
	//     If workspacePart is XTPTabWorkspacePartVSplitter, then lpRect refers to
	//     the size of the splitter placed between vertical tab groups.
	//
	//     If workspacePart is XTPTabWorkspacePartWidth, then this is
	//     the size of the splitter.
	// See Also:
	//     XTPTabWorkspacePart
	//-----------------------------------------------------------------------
	virtual void DrawWorkspacePart(CDC* pDC, LPRECT lpRect, XTPTabWorkspacePart workspacePart);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the supported color sets for this theme.
	// Returns:
	//     The supported color sets for this theme.
	// See Also:
	//     GetDefaultColorSet
	//-----------------------------------------------------------------------
	virtual int GetSupportedColorSets()
	{
		return xtpTabColorAll;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the default value of the color set for this theme.
	// Returns:
	//     The default value of the color set for this theme.
	// See Also:
	//     GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet()
	{
		return xtpTabColorDefault;
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a tab button within a specified polygon.
	// Parameters:
	//     pDC         - Pointer to the device context to draw.
	//     pItem       - Pointer to the tab item to draw.
	//     pts         - Pointer to an array of points that determine the target polygon.
	//     nCount      - Number of points in the array of points of the target polygon.
	//     tabPosition - Tab position; must be one of the values
	//                   defined by the XTPTabPosition enumeration.
	//     bAbsolute   - Flag that specifies if points are in absolute coordinates.
	// Returns:
	//     The color of the tab that was filled
	//-----------------------------------------------------------------------
	COLORREF FillButton(CDC* pDC, CXTPTabManagerItem* pItem, LPPOINT pts, int nCount,
						XTPTabPosition tabPosition, BOOL bAbsolute = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the text of a specified item.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pManager - Pointer to a valid CXTPTabManager object.
	//     str      - Reference to the text to render.
	//     lpRect   - Size of the text to render.
	//     nFormat  - Drawing format flags.
	//-----------------------------------------------------------------------
	virtual void DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str, LPRECT lpRect,
						  UINT nFormat);

	//{{AFX_CODEJOCK_PRIVATE
	static void AFX_CDECL DrawPolyLine(CDC* pDC, COLORREF clr, LPPOINT pts, int nCount);
	static void AFX_CDECL DrawSingleLineBorder(CDC* pDC, CRect rc, XTPTabPosition position,
											   COLORREF clrTopLeft, COLORREF clrBottomRight);
	static void AFX_CDECL InflateRectEx(CRect& rc, CRect rcInflate, XTPTabPosition position);
	static void AFX_CDECL DeflateRectEx(CRect& rc, CRect rcDeflate, XTPTabPosition position);
	//}}AFX_CODEJOCK_PRIVATE

public:
	CRect m_rcHeaderMargin; // CRect struct containing the left, top, right, and bottom
							// margins of the tab header. This is the space placed around
							// the tab buttons inside the tab header.
	int m_nRowMargin;		// Margin between rows.

protected:
	int m_nButtonHeight;				  // Current tab button height (without margins).
	CXTPTabPaintManager* m_pPaintManager; // Self paint manager pointer.
	BOOL m_bButtonsReverseZOrder;		  // Draw buttons in right to left order.

	friend class CXTPTabPaintManager;
};

#	define CXTPTabPaintManagerAppearanceSet CXTPTabPaintManagerTheme // Old class name.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABPAINTMANAGERTHEME_H__)
