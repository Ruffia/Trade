// XTPTaskPanelVisualStudio2012Theme.h : implementation of the CXTPTaskPanelVisualStudio2012Theme
// class.
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
#ifndef XTPTaskPanelVisualStudio2012Theme_h__
#define XTPTaskPanelVisualStudio2012Theme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#	pragma once
#endif // _MSC_VER > 1000

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelVisualStudio2012Theme represents a VS.NET 2012 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelVisualStudio2012Theme : public CXTPTaskPanelVisualStudio2005Theme
{
protected:
	void Triangle(CDC* pDC, POINT* pPoints, COLORREF clr, BOOL bFilled);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelVisualStudio2012Theme object.
	// Parameters:
	//     bLight - TRUE to draw light theme, FALSE otherwise.
	//-----------------------------------------------------------------------
	CXTPTaskPanelVisualStudio2012Theme(BOOL bLight = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelVisualStudio2012Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelVisualStudio2012Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group caption background of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the group caption background of.
	//     rc     - Bounding rectangle of the group caption.
	// Returns:
	//     The bounding rectangle of the text within the group caption.
	//-----------------------------------------------------------------------
	CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the client face of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to draw the client face of.
	//     rc     - Rectangle of the group to draw the client face of.
	//-----------------------------------------------------------------------
	void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a rectangle around a group item when it is
	//     selected, pressed or highlighted.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to the group item.
	//     rc    - Rectangle to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client face of a specified group.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Pointer to the group to fill the client face of.
	//     rc     - Rectangle of the group to fill the client face of.
	// See Also:
	//     DrawGroupClientFace
	//-----------------------------------------------------------------------
	void FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the background of a specified task panel.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pTaskPanel - Pointer to the task panel to fill.
	//-----------------------------------------------------------------------
	void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the text caption of a specified item.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pItem   - Pointer to the item to draw the text caption of.
	//     rcText  - Rectangle of the item to draw the text caption of.
	//     nFormat - Specifies the method of formatting the text.
	// Remarks:
	//     DrawGroupCaption uses this method to draw the group caption with
	//     nFormat = DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX
	//
	//     DrawGroupItemImageWithText and DrawGroupItemImageWithTextBelow both use
	//     this method to draw the group item caption.
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the task panel.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
	virtual CXTPPaintManagerColor GetTriangleColorAndState(CXTPTaskPanelGroup* pGroup,
														   BOOL& bFilled);

	virtual CRect DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

protected:
	CXTPPaintManagerColor m_clrSelectedWithoutFocus; // Color of the selected item when
													 // the Task Panel does not have focus.

	CXTPPaintManagerColor m_clrText;					 // Normal text color.
	CXTPPaintManagerColor m_clrSelectedText;			 // Text color of the selected item.
	CXTPPaintManagerColor m_clrSelectedTextWithoutFocus; // Text color of the selected item when
														 // the Task Panel does not have focus.

	CXTPPaintManagerColor m_clrTriangle;
	CXTPPaintManagerColor m_clrTriangleFocused;
	CXTPPaintManagerColor m_clrTriangleHot;

	BOOL m_bLight;
};

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // XTPTaskPanelVisualStudio2012Theme_h__
