// XTPStatusBarPaneSwitch.h
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
#if !defined(__XTPSTATUSBARPANESWITCH_H__)
#	define __XTPSTATUSBARPANESWITCH_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPStatusBarSwitchPane is a CXTPStatusBarPane derived class.
//     CXTPStatusBarSwitchPane works with CXTStatusBar and allows
//     you to create switches in your status bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarSwitchPane : public CXTPStatusBarPane
{
	DECLARE_DYNAMIC(CXTPStatusBarSwitchPane)
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Holds status bar pane switch properties.
	//-------------------------------------------------------------------------
	struct SWITCH
	{
		CString strToolTip; // Tooltip of the switch.
		UINT nID;			// Id of the switch.
		int nWidth;			// Width of the switch.
		BOOL bChecked;		// TRUE to "check" the switch.
		BOOL bHighlighted;  // TRUE if the switch is highlighted.
		BOOL bPressed;		// TRUE if the switch is pressed.
		BOOL bEnabled;		// TRUE if enabled
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarSwitchPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarSwitchPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarSwitchPane object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	~CXTPStatusBarSwitchPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set switches.
	// Parameters:
	//     lpIDArray - Pointer to an array with identifiers.
	//     nIDCount - Count of elements in the array.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetSwitches(const UINT* lpIDArray, int nIDCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a switch to the Switch Pane.
	// Parameters:
	//     nID - ID of the switch.
	//     lpszToolTip - Tooltip of the switch.
	// Returns:
	//     Reference to a SWITCH object.
	//-----------------------------------------------------------------------
	SWITCH* AddSwitch(UINT nID);
	SWITCH* AddSwitch(UINT nID,
					  LPCTSTR lpszToolTip); // <combine CXTPStatusBarSwitchPane::AddSwitch@UINT>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a switch by its index/identifier.
	//     Returns a switch by its position.
	// Parameters:
	//     nIndex - Index of the switch to retrieve.
	//     nID    - Identifier of the switch to retrieve.
	// Returns:
	//     Pointer to the SWITCH.
	//-----------------------------------------------------------------------
	SWITCH* GetSwitch(int nIndex) const;
	SWITCH* FindSwitch(UINT nID) const; // <combine CXTPStatusBarSwitchPane::GetSwitch@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the total number of switches in the switch pane.
	// Returns:
	//     Total number of switches in the switch pane.
	//-----------------------------------------------------------------------
	int GetSwitchCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all switches from the switch pane.
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a switch from the switch pane.
	// Parameters:
	//     nID - ID of the switch to remove.
	//-----------------------------------------------------------------------
	void RemoveSwitch(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the switch pane is enabled.
	// Parameters:
	//     nID - ID of the switch to enable\disable.
	//     bEnabled - TRUE to enable the switch pane, FALSE to disable the swith pane.
	//-----------------------------------------------------------------------
	void EnableSwitch(UINT nID, BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the ID of the switch that is currently checked.
	// Parameters:
	//     nID - ID of the switch to be checked.
	// Remarks:
	//     Checked is used to get/set the currently selected/checked
	//     switch in the switch pane.
	//-----------------------------------------------------------------------
	void SetChecked(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the width of the switch pane.
	// Returns:
	//     Width of the switch pane.
	//-----------------------------------------------------------------------
	virtual int GetWidth();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine which pane, if any, is at the
	//     specified screen position.
	// Parameters:
	//     point - Point to test.
	//     rcItem - Retrieves the bounding rectangle of the item.
	// Returns:
	//     A SWITCH object containing the data of the pane at the specified point.
	//-----------------------------------------------------------------------
	SWITCH* HitTest(CPoint point, CRect& rcItem) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process left mouse button events.
	// Parameters:
	//     point - A CPoint that specifies the x- and y- coordinates of
	//             the cursor. These coordinates are always relative to
	//             the upper-left corner of the window.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a pane switch item. Override it to
	//     draw a custom switch.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Item rectangle.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether
	//     a point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor. These
	//             coordinates are always relative to the upper-left
	//             corner of the window.
	//     pTI - A pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, then the window control ID is returned.
	//     If the tooltip control was not found, then a value of -1 is returned.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPStatusBarSwitchPane);

	void OleAddSwitch(UINT nID, LPCTSTR lpszToolTip);
	void OleRemoveSwitch(UINT nID);
	int OleGetChecked();
	BSTR OleGetSwitchTooltip(int nID);
	void OleSetSwitchTooltip(int nID, LPCTSTR lpszTooltip);
//}}AFX_CODEJOCK_PRIVATE
#	endif

protected:
	CArray<SWITCH*, SWITCH*> m_arrSwitches; // Collection of switces in the switch pane.
	SWITCH* m_pHighlighted;					// TRUE if the switch is currently highlighted.
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSTATUSBARPANESWITCH_H__)
