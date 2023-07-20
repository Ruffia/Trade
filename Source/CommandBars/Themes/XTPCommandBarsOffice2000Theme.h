// XTPCommandBarsOffice2000Theme.h : interface for the CXTPCommandBarsOffice2000Theme class.
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
#if !defined(__XTPCOMMANDBARSOFFICE2000THEME_H__)
#	define __XTPCOMMANDBARSOFFICE2000THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The CXTPCommandBarsOffice2000Theme class is used to enable an
//     Office 2000 style theme for Command Bars.
// See Also:
//     CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsOffice2000Theme : public CXTPPaintManager
{
	DECLARE_DYNAMIC(CXTPCommandBarsOffice2000Theme)
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsOffice2000Theme object.
	//-------------------------------------------------------------------------
	CXTPCommandBarsOffice2000Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Pointer to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

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
										  BOOL bDraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the offset of a pop-up bar.
	// Parameters:
	//     rc       - Control's bounding rectangle.
	//     pControl - Pointer to a CXTPControl object.
	//     bVertical - TRUE if the control is docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the command bar's borders.
	// Parameters:
	//     pBar - Pointer to the CXTPCommandBar object that the borders need to get.
	// Returns:
	//     Borders of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the ListBox.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     pButton   - Pointer to a CXTPControl object.
	//     rc        - Bounding rectangle to draw.
	//     bSelected - TRUE if the control is selected.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
	//     pCommandBars - CommandBars object which metrics need to use.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bSelected,
									 BOOL bDraw, CXTPCommandBars* pCommandBars = 0);

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
	// See Also:
	//     XTPSpecialControl
	//-----------------------------------------------------------------------
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton,
									 CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a tear-off gripper of a pop-up bar.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     rcGripper - Bounding rectangle of the gripper.
	//     bSelected - TRUE if the gripper is selected.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the pop-up bar resize gripper.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     rcGripper - Bounding rectangle of the gripper.
	//     nFlags    - Position of resize gripper
	//-----------------------------------------------------------------------
	virtual void DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the pop-up bar.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPControl object.
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws split button frame.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     pButton  - Pointer to the split button to draw.
	//     rcButton - Bounding rectangle to draw.
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton);

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
	//     nID         - Bar's control ID.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled,
							   BOOL bChecked, BOOL bPopuped, XTPBarType barType,
							   XTPBarPosition barPosition);

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
	//     This method is called to draw the image of the control.
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     pt            - Position to draw.
	//     sz            - Size of the image.
	//     pImage        - Pointer to a CXTPImageManagerIcon object.
	//     bSelected     - TRUE if the control is selected.
	//     bPressed      - TRUE if the control is pushed.
	//     bEnabled      - TRUE if the control is enabled.
	//     bChecked      - TRUE if the control is checked.
	//     bPopuped      - TRUE if the control is pop-up'ed.
	//     bToolBarImage - TRUE if it is a toolbar image.
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage,
						   BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE,
						   BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     pComboBox - ComboBox to draw.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pComboBox, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
	//     pEdit - Edit control to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pEdit, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw the gripper of a dialog bar.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pBar  - Dialog Bar pointer.
	//     bDraw - TRUE to draw the gripper, FALSE to calculate size.
	// Returns:
	//     Size of the gripper to be drawn.
	//-----------------------------------------------------------------------
	virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* pBar, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame of an edit control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - CRect object specifying size of area.
	//     bEnabled  - TRUE if control is enabled.
	//     bSelected - TRUE if control is selected.
	//-----------------------------------------------------------------------
	virtual void DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the button of a combobox control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcBtn     - Button bounding rectangle.
	//     bEnabled  - TRUE if combobox is enabled.
	//     bSelected - TRUE if combobox is selected.
	//     bDropped  - TRUE if combobox is dropped.
	//-----------------------------------------------------------------------
	virtual void DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled, BOOL bSelected,
										   BOOL bDropped);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method draws edit control spin buttons.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pControlEdit - Edit control.
	//-----------------------------------------------------------------------
	virtual void DrawControlEditSpin(CDC* pDC, CXTPControlEdit* pControlEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme()
	{
		return xtpThemeOffice2000;
	}

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void DrawShadedRect(CDC* pDC, CRect& rect);
	virtual CSize GetPopupBarImageSize(CXTPCommandBar* pBar);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_nPopupBarText; // Pop-up bar text index.
};

#	define CXTPDefaultTheme CXTPCommandBarsOffice2000Theme // Old name

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCOMMANDBARSOFFICE2000THEME_H__)
