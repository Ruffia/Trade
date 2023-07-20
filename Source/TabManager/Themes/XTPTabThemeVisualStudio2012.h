// XTPTabThemeVisualStudio2012.h
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
#if !defined(__XTPTABTHEMEVISUALSTUDIO2012_H__)
#	define __XTPTABTHEMEVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabThemeVisualStudio2012 implements a Visual Studio 2012 tab appearance.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabThemeVisualStudio2012 : public CXTPTabThemeFlat
{
public:
	CXTPTabThemeVisualStudio2012();

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
	//     Gets the default value of the color set for this theme.
	// Returns:
	//     The default value of the color set for this theme.
	// See Also:
	//     GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet()
	{
		return xtpTabColorVisualStudio2012;
	}
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABTHEMEVISUALSTUDIO2012_H__)
