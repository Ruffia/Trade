// XTPControlPopup.h : interface for the CXTPControlPopup class.
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
#if !defined(__XTPCONTROLPOPUP_H__)
#	define __XTPCONTROLPOPUP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCommandBar;
class CXTPControls;

//===========================================================================
// Summary:
//     CXTPControlPopup is a CXTPControl derived class.
//     It represents a pop-upable control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlPopup : public CXTPControlButton
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlPopup object.
	//-----------------------------------------------------------------------
	CXTPControlPopup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlPopup object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPControlPopup();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPControlPopup object with the specified type.
	// Parameters:
	//     controlType - XTPControlType type of pop-up control. Can be any of the
	//                   values listed in the Remarks section.
	// Remarks:
	//     Pop-up type can be one of the following:
	//         * <b>xtpControlPopup</b>: Pop-up type.
	//         * <b>xtpControlButtonPopup</b>: Button pop-up.
	//         * <b>xtpControlSplitButtonPopup</b>: Split button pop-up.
	// Returns:
	//     A pointer to a CXTPControlPopup object.
	//-----------------------------------------------------------------------
	static CXTPControlPopup* AFX_CDECL CreateControlPopup(XTPControlType controlType);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the child command bar.
	// Parameters:
	//     pMenu - The menu to be loaded as a child.
	//     pBar - The child command bar.
	//-----------------------------------------------------------------------
	void SetCommandBar(CMenu* pMenu);
	void SetCommandBar(CXTPCommandBar* pBar); //<combine CXTPControlPopup::SetCommandBar@CMenu*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the child command bar.
	// Returns:
	//     A pointer to the child command bar.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetCommandBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's pop-up state.
	// Returns:
	//     TRUE if the control is a pop-up, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetPopuped() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's caption.
	// Returns:
	//     The caption of the control.
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates rectangle to set the position where the child pop-up bar becomes visible.
	// Parameters:
	//     rc        - CRect object specifying the size of the area.
	//     bVertical - TRUE if the control is located vertically in its parent.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Pointer to a source CXTPControl object.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user activates a control using its underline.
	//----------------------------------------------------------------------
	virtual void OnUnderlineActivate();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to check if pControlPopup contains in
	//     the control recursively.
	// Parameters:
	//     pControlPopup - Pointer to a CXTPControlPopup object to check.
	// Returns:
	//     TRUE if contains; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL ContainsPopup(CXTPControlPopup* pControlPopup);

	//{{AFX_CODEJOCK_PRIVATE
	_XTP_DEPRECATED_IN_FAVOR(ContainsPopup)
	BOOL IsContainPopup(CXTPControlPopup* pControlPopup)
	{
		return ContainsPopup(pControlPopup);
	}
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to expand the parent pop-up bar.
	//-----------------------------------------------------------------------
	void ExpandCommandBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnSetSelected(int bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called after the mouse hovers over the control.
	//-----------------------------------------------------------------------
	void OnMouseHover();

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	//----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to pop-up the control.
	// Parameters:
	//     bPopup - TRUE to set pop-up.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnSetPopup(BOOL bPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the left mouse
	//     button.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user double-clicks the left mouse
	//     button.
	// Parameters:
	//     point - Specifies the x- and y- coordinates of the cursor.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user moves the mouse over the
	//     control.
	// Parameters:
	//     pDataObject - Pointer to a CXTPControl object.
	//     point - Mouse position.
	//     dropEffect - DROPEFFECT enumerator.
	//-----------------------------------------------------------------------
	void OnCustomizeDragOver(CXTPControl* pDataObject, CPoint point, DROPEFFECT& dropEffect);

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to compare controls.
	// Parameters:
	//     pOther - The control to compare with.
	// Returns:
	//     TRUE if the controls are identical, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Compare(CXTPControl* pOther);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to update the shadow of the control.
	//-----------------------------------------------------------------------
	virtual void UpdateShadow();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called from LoadCommandBars to restore
	//     controls and its child command bars.
	// Parameters:
	//     pCommandBarList - Command bars collection that contains all the
	//                       command bars to be restored.
	//-----------------------------------------------------------------------
	virtual void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList);

protected:
	BOOL m_bPopup;				   // TRUE if pop-up'ed.
	CXTPCommandBar* m_pCommandBar; // Child pop-up bar.
	DWORD m_dwCommandBarID;		   // Child pop-up ID.
	BOOL m_bShowShadow;			   // TRUE to show shadow.

public:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlPopup);

	afx_msg LPDISPATCH OleGetCommandBar();
	afx_msg void OleSetCommandBar(LPDISPATCH lpCommandBar);
	enum
	{
		dispidCommandBar = 50L,
		dispidStyle		 = 51L,
	};

//}}AFX_CODEJOCK_PRIVATE
#	endif
	DECLARE_XTP_CONTROL(CXTPControlPopup)
	friend class CXTPControls;
	friend class CXTPPopupBar;
	friend class CXTPCommandBar;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPCommandBar* CXTPControlPopup::GetCommandBar() const
{
	return m_pCommandBar;
}
AFX_INLINE BOOL CXTPControlPopup::GetPopuped() const
{
	return m_bPopup;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCONTROLPOPUP_H__)
