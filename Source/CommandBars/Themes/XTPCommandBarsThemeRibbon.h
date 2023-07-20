// XTPCommandBarsThemeRibbon.h: interface for the CXTPRibbonTheme class.
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
#if !defined(__XTPRIBBONTHEME_H__)
#	define __XTPRIBBONTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The CXTPRibbonTheme class is used to enable a Ribbon style theme for Command Bars.
// See Also:
//     CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonTheme : public CXTPResourceTheme
{
	DECLARE_DYNAMIC(CXTPRibbonTheme)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonTheme object.
	//-----------------------------------------------------------------------
	CXTPRibbonTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonTheme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	virtual void DrawControlEntry(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the offset of a pop-up bar.
	// Parameters:
	//     rc        - Control's bounding rectangle.
	//     pControl  - Pointer to a CXTPControl object.
	//     bVertical - TRUE if the control is docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame for the SplitButton
	//     control.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPControl object.
	//     rc      - Bounding rectangle to draw.
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a pop-up bar's right gripper.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     xPos      - Specifies the logical x-coordinate of the upper-left corner of
	//                 the rectangle.
	//     yPos      - Specifies the logical y-coordinate of the upper-left corner of
	//                 the destination rectangle.
	//     cx        - Specifies the width of the rectangle.
	//     cy        - Specifies the height of the rectangle.
	//     bExpanded - TRUE if expanded.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarGripper(CDC* pDC, int xPos, int yPos, int cx, int cy,
									 BOOL bExpanded = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context.
	//     pBar - Pointer to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Rectangle to draw.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is pop-up'ed.
	//     barType     - Parent's bar type.
	//     barPosition - Parent's bar position.
	//     nID         - Bar's control ID.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled,
							   BOOL bChecked, BOOL bPopuped, XTPBarType barType,
							   XTPBarPosition barPosition);
	virtual void
		DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled,
					  BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition,
					  int nID); // <combine
								// CXTPRibbonTheme::DrawRectangle@CDC*@CRect@BOOL@BOOL@BOOL@BOOL
								// @BOOL@XTPBarType@XTPBarPosition>

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context.
	//     pBar - Pointer to a CXTPControl object to draw.
	// Returns:
	//     TRUE if the rectange was drawn, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawRectangle(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a command bar's separator.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pBar     - Pointer to a CXTPCommandBar object.
	//     pControl - Pointer to a CXTPControl object.
	//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
	// Returns:
	//     Size of the separator.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl,
										  BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar's gripper.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pBar  - Pointer to a CXTPCommandBar object.
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a status bar.
	// Parameters:
	//     pDC  - Pointer to a valid device context.
	//     pBar - Pointer to a CXTPStatusBar object.
	//-----------------------------------------------------------------------
	virtual void FillStatusBar(CDC* pDC, CXTPStatusBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame of a single cell of the status bar.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - CRect object specifying the size of the area.
	//     pPane - The status bar pane to draw.
	//     bGripperPane - TRUE if the pane is the last cell of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane,
										 BOOL bGripperPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pane separator of status bar panes.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - CRect object specifying the size of the area.
	//     pPane - The status bar pane to draw the separator by.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneSeparator(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar switch pane.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcItem - CRect object specifying the size of the area.
	//     pPane - The status bar switch pane to draw.
	//-----------------------------------------------------------------------
	virtual int DrawStatusBarSwitchPane(CDC* pDC, CRect rcItem, CXTPStatusBarSwitchPane* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves a button's text color.
	// Parameters:
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is pop-up'ed.
	//     barType     - Parent's bar type.
	//     barPosition - Parent's bar position.
	//-----------------------------------------------------------------------
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled,
										   BOOL bChecked, BOOL bPopuped, XTPBarType barType,
										   XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns a control text color to draw.
	// Parameters:
	//     pButton - Pointer to a CXTPControl object to draw.
	// Returns:
	//     Button text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlTextColor(CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
	//     pControlEdit - Edit control to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pComboBox - ComboBox to draw.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pComboBox, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns the edit control back color to draw.
	// Parameters:
	//     pControl - Points to a CXTPControl object to get the back color from.
	// Returns:
	//     Edit control back color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlEditBackColor(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme()
	{
		return xtpThemeRibbon;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the bounding region for a pop-up bar.
	// Parameters:
	//     pCommandBar - Pointer to the pop-up bar in which the bounding region must be set.
	//-----------------------------------------------------------------------
	virtual void SetCommandBarRegion(CXTPCommandBar* pCommandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a menu split button dropdown rect.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pButton - Split button to draw.
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonPopup(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to a draw menu pop-up glyph.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pButton - Pop-up button to draw the glyph.
	//-----------------------------------------------------------------------
	virtual void DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a menu label background.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - Bounding rectangle of the button.
	//-----------------------------------------------------------------------
	virtual void FillPopupLabelEntry(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set a rounded rectangle region for a window.
	// Parameters:
	//     pWnd - Window to set region to.
	//-----------------------------------------------------------------------
	void SetRoundRectRegion(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a ribbon keyboard tip.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pWnd - Keyboard tip window.
	//     bSetRegion - TRUE to set region.
	//-----------------------------------------------------------------------
	virtual void DrawKeyboardTip(CDC* pDC, CXTPCommandBarKeyboardTip* pWnd, BOOL bSetRegion);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified controls.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     controlType - Special control enumerator.
	//     pButton     - Pointer to a CXTPControl object to draw.
	//     pBar        - Parent CXTPCommandBar object.
	//     bDraw       - TRUE to draw; FALSE to retrieve the size of the
	//                   control.
	//     lpParam     - Specified parameter.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton,
									 CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the resize gripper for the pop-up window.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcGripper - CRect object specifying the size of the area.
	//     nFlags - Options for gripper.
	//-----------------------------------------------------------------------
	void DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a check mark.
	// Parameters:
	//     pDC -      Pointer to a valid device context
	//     rcCheck -  Specifies the rectangle in logical units.
	//     clr -      Color to fill.
	//     bEnabled - Specifies whether the checkmark will be drawn enabled or disabled.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarCheckMark(CDC* pDC, CRect rcCheck, BOOL bEnabled, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a radio button.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     rcCheck    - Specifies the rectangle in logical units.
	//     clr        - Color to fill.
	//     bEnabled   - TRUE to enable, FALSE to disable the button.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarRadioMark(CDC* pDC, CRect rcCheck, BOOL bEnabled, COLORREF clr);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the current ribbon paint manager.
	// Parameters:
	//     pRibbonPaintManager - Pointer to a valid ribbon paint manager.
	//-----------------------------------------------------------------------
	void SetRibbonPaintManager(CXTPRibbonPaintManager* pRibbonPaintManager);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if images are available.
	// Returns:
	//     TRUE if images exist in the application.
	//-----------------------------------------------------------------------
	BOOL IsImagesAvailable();

	//{{AFX_CODEJOCK_PRIVATE
	virtual void DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt, BOOL bSelected,
								   BOOL bPopuped, BOOL bEnabled, BOOL bVert);
	virtual CSize DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected,
										  BOOL bPressed, BOOL bChecked, BOOL bEnabled);
	virtual CSize DrawControlCheckBoxMark(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bDraw);
	virtual CSize DrawControlRadioButtonMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected,
											 BOOL bPressed, BOOL bChecked, BOOL bEnabled);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	friend class CXTPRibbonBar;

protected:
	COLORREF m_clrControlEditNormal;		 // Edit control color
	COLORREF m_clrControlEditSelected;		 // Edit control selected color
	COLORREF m_clrControlEditDisabledBorder; // Edit disabled border
	COLORREF m_clrRibbonText;				 // Ribbon text color
	COLORREF m_clrRibbonGrayText;			 // Gray text of the ribbon.
	COLORREF m_clrMenuPopupSeparator;		 // Pop-up separator color.
	COLORREF m_clrMenuPopupGripperShadow;	// Pop-up gripper shadow color.
	BOOL m_bOffice2010Style;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONTHEME_H__)
