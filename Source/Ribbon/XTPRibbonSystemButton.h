// XTPRibbonSystemButton.h: interface for the CXTPRibbonControlSystemButton class.
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
#if !defined(__XTPRIBBONSYSTEMBUTTON_H__)
#	define __XTPRIBBONSYSTEMBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemButton is a CXTPControlPopup derived class used
//     as a system button for a Ribbon Bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemButton : public CXTPControlPopup
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemButton)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemButton object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemButton();

public:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine if this system button is a
	//     normal button or the Office 2007 style system button.
	// Returns:
	//     TRUE if this system button is a normal button,
	//     FALSE if this system button is an Office 2007 style system button.
	//----------------------------------------------------------------------
	BOOL IsSimpleButton() const;

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     The size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the caption of the control.
	// Returns:
	//     The caption of the control.
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user double-clicks the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(CPoint point);

	// ----------------------------------------------------------------------
	// Summary:
	//     Updates rectangle to set the position where a child pop-up bar
	//     becomes visible.
	// Parameters:
	//     rc        - CRect object specifying size of area.
	//     bVertical - TRUE if vertical, FALSE otherwise.
	// ----------------------------------------------------------------------
	void AdjustExcludeRect(CRect& rc, BOOL bVertical);

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonControlSystemButton);

	//}}AFX_CODEJOCK_PRIVATE
#	endif
protected:
	BOOL m_bCloseOnDblClick;

	friend class CXTPRibbonBackstageView;
	friend class CXTPRibbonBar;
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarButton is a CXTPControlButton derived
//     class used in a system pop-up bar for bottom option buttons.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarButton : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarButton)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarButton object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarButton();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     The size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the control needs to draw its background.
	// Returns:
	//     TRUE if the paint manager must skip filling the background of the control.
	//-----------------------------------------------------------------------
	BOOL IsTransparent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the command bar button.
	// Returns:
	//     The width and height values of the command bar button.
	//-----------------------------------------------------------------------
	virtual CSize GetButtonSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of the toolbar icon.
	// Returns:
	//     The size of the toolbar icon.
	// See Also:
	//     GetButtonSize
	//-----------------------------------------------------------------------
	virtual CSize GetIconSize() const;

private:
	BOOL IsSystemPopupButton() const;
};

//===========================================================================
// Summary:
//     CXTPRibbonSystemPopupBar is a CXTPPopupBar derived class used
//     as a system pop-up bar for a Ribbon Bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonSystemPopupBar : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonSystemPopupBar)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonSystemPopupBar object.
	//-----------------------------------------------------------------------
	CXTPRibbonSystemPopupBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the border margins of the command bar.
	// Returns:
	//     A CRect object containing the border margins of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetBorders();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the background of the command bar.
	// Parameters:
	//     pDC - Pointer to the device context to draw.
	// See Also:
	//     DrawCommandBar
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either
	//                horizontal or vertical depending on dwMode.
	//      dwMode  - See CControlBar::CalcDynamicLayout for the list of
	//                supported flags.
	// Returns:
	//     The size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPRibbonSystemPopupBarPage is a CXTPPopupBar derived class used
//     as a system pop-up bar page of a Ribbon Bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonSystemPopupBarPage : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonSystemPopupBarPage)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonSystemPopupBarPage object.
	//-----------------------------------------------------------------------
	CXTPRibbonSystemPopupBarPage();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either
	//                horizontal or vertical depending on dwMode.
	//      dwMode  - See CControlBar::CalcDynamicLayout for the list of
	//                supported flags.
	// Returns:
	//     The size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates rectangle to set the position where the pop-up bar becomes visible.
	// Parameters:
	//     rc        - CRect object specifying the size of the area.
	//     bVertical - TRUE if vertical, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, BOOL bVertical);
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarListCaption is a CXTPControl derived class
//     used in a Ribbon system pop-up to draw captions of enumerable controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarListCaption : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListCaption)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarListCaption object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarListCaption();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     The size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarListItem is a CXTPControlButton derived class
//     used in a Ribbon system pop-up to draw buttons of enumerable controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarListItem : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarListItem object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarListItem();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     The size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

protected:
	BOOL m_bAlignShortcut; // Alignment of the shortcut in the list.
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarPinableListItem is a
//     CXTPRibbonControlSystemPopupBarListItem derived class used in a
//     Ribbon system pop-up to draw recent file list items.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarPinableListItem
	: public CXTPRibbonControlSystemPopupBarListItem
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarPinableListItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarListItem object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarPinableListItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonControlSystemPopupBarPinableListItem object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPRibbonControlSystemPopupBarPinableListItem();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the MRU item becomes selected.
	// Parameters:
	//     bSelected - TRUE if the MRU item becomes selected.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnSetSelected(int bSelected);

protected:
	CXTPImageManager* m_pIcons; // Icons used for the MRU item.
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemRecentFileList is a
//     CXTPRibbonControlSystemPopupBarListCaption derived class used in a
//     Ribbon system pop-up to draw recent file list controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemRecentFileList
	: public CXTPRibbonControlSystemPopupBarListCaption
{
	class CControlFileItem;
	class CControlPinableFileItem;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemRecentFileList object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemRecentFileList();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command bar
	//     size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar. See Remarks section for a list of
	//              values.
	// Remarks:
	//     The following predefined flags are used to determine the height and
	//     width of the dynamic command bar. Use the bitwise-OR (|) operator to
	//     combine the flags.<p/>
	//
	//     * <b>LM_STRETCH</b>: Indicates whether the command bar should be
	//                          stretched to the size of the frame. Set if the bar is
	//                          not a docking bar (not available for docking). Not set
	//                          when the bar is docked or floating (available for
	//                          docking). If set, LM_STRETCH returns dimensions based
	//                          on LM_HORZ state. LM_STRETCH works similarly to
	//                          the bStretch parameter used in CalcFixedLayout;
	//                          see that member function for more information about
	//                          the relationship between stretching and orientation.
	//     * <b>LM_HORZ</b>: Indicates that the bar is horizontally or
	//                       vertically oriented. Set if the bar is horizontally
	//                       oriented, and if it is vertically oriented, it is not
	//                       set. LM_HORZ works similarly to the the bHorz
	//                       parameter used in CalcFixedLayout; see that member
	//                       function for more information about the relationship
	//                       between stretching and orientation.
	//     * <b>LM_MRUWIDTH</b>: Most Recently Used Dynamic Width. Uses the
	//                           remembered most recently used width.
	//     * <b>LM_HORZDOCK</b>: Horizontal Docked Dimensions. Returns the
	//                           dynamic size with the largest width.
	//     * <b>LM_VERTDOCK</b>: Vertical Docked Dimensions. Returns the dynamic
	//                           size with the largest height.
	//     * <b>LM_COMMIT</b>: Resets LM_MRUWIDTH to the current width of the
	//                         floating command bar.
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//----------------------------------------------------------------------
	// Summary:
	//     The implementation may implement a custom drag over handling logic in this method.
	// Parameters:
	//     pCommandBar - Pointer to a CXTPCommandBar object.
	//     point       - Mouse position.
	//     dropEffect  - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if custom implementation handles drag over, otherwise FALSE.
	//----------------------------------------------------------------------
	virtual BOOL DoCustomDragOver(CXTPCommandBar* pCommandBar, CPoint point,
								  DROPEFFECT& dropEffect);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the recent file list collection.
	// Returns:
	//     A pointer to the recent file list collection.
	//-----------------------------------------------------------------------
	virtual CRecentFileList* GetRecentFileList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves ID_FILE_MRU_FILE1.
	// Returns:
	//     ID_FILE_MRU_FILE1.
	//-----------------------------------------------------------------------
	virtual int GetFirstMruID();

private:
	CString ConstructCaption(const CString& lpszTitle, int nIndex);

private:
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemRecentFileList)
};

#	define CXTPPinableRecentFileList CXTPRecentFileList

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONSYSTEMBUTTON_H__)
