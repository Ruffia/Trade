// XTPCommandBarsNativeWindows10Theme.h : interface for the CXTPCommandBarsNativeWindows10Theme
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
#if !defined(__XTPCOMMANDBARSNATIVEWINDOWS10THEME_H__)
#	define __XTPCOMMANDBARSNATIVEWINDOWS10THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The CXTPCommandBarsNativeWindows10Theme class is used to enable a
//     Native Windows 10 style theme for Command Bars.
// See Also:
//     CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsNativeWindows10Theme : public CXTPCommandBarsOffice2013Theme
{
	DECLARE_DYNAMIC(CXTPCommandBarsNativeWindows10Theme)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsNativeWindows10Theme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsNativeWindows10Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsNativeWindows10Theme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarsNativeWindows10Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get/set the application state color for the
	//     frame border and status bar.
	// Parameters:
	//      clrState - COLORREF value representing the application state color. Use
	//                 COLORREF_NULL to reset back to default color.
	//-----------------------------------------------------------------------
	virtual void SetStateColor(COLORREF clrState = COLORREF_NULL);
	virtual COLORREF GetStateColor()
		const; // <combine CXTPCommandBarsNativeWindows10Theme::SetStateColor@COLORREF>

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame for the SplitButton control.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to a CXTPControl object.
	//     rc      - Bounding rectangle to draw.
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face.
	// Parameters:
	//     pDC         - Pointer to a valid device context
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
	virtual void DrawRectangle(
		CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked,
		BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition,
		int nID); // <combine
				  // CXTPCommandBarsNativeWindows10Theme::DrawRectangle@CDC*@CRect@BOOL@BOOL@BOOL@BOOL@BOOL@XTPBarType@XTPBarPosition>

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
	//     This method returns the control text color to draw.
	// Parameters:
	//     pButton - Pointer to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlTextColor(CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background for a gallery menu item.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Rectangle to draw.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     barType     - Parent's bar type.
	//-----------------------------------------------------------------------
	virtual void DrawGalleryItemBack(CDC* pDC, CRect rcItem, BOOL bEnabled, BOOL bSelected,
									 BOOL bPressed, BOOL bChecked, XTPBarType barType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Helper class called by DrawRectangle to render a checkmark
	//     background and border.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     rc         - Rectangle to draw.
	//     clrFore    - RGB color value used to draw border.
	//     clrBack    - RGB color value used to draw background.
	//-----------------------------------------------------------------------
	virtual void DrawCheckRect(CDC* pDC, CRect rc, COLORREF clrFore, COLORREF clrBack);

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
	//     This method is called to draw the control.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     bDraw  - TRUE to draw; FALSE to retrieve the size of the control.
	//     pComboBox - CXTPControlComboBox pointer need to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pComboBox, BOOL bDraw);
};

//---------------------------------------------------------------------------
// Summary:
//     Call XTPSetNativeWindows10StateColor to set the border and status bar colors for this
//     theme to indicate state changes. This is similar to when Visual Studio 2015
//     opens a document or runs the debugger causing both the border and status
//     bar colors to be updated.
// Parameters:
//     pCommandBars - Pointer to a valid CXTPCommandBars object.
//     clrState     - RGB state color value to set. Use default COLORREF_NULL
//                    to reset colors to their default state.
// Returns:
//     TRUE if the colors were successfully updated, otherwise FALSE.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL XTPSetNativeWindows10StateColor(CXTPCommandBars* pCommandBars,
															  COLORREF clrState = COLORREF_NULL);

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPCOMMANDBARSNATIVEWINDOWS10THEME_H__)
