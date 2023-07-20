// XTPGridTip.h: interface for the CXTPGridTip class.
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
#if !defined(__XTPGRIDTIP_H__)
#	define __XTPGRIDTIP_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     The CXTPGridTip class encapsulates the functionality of
//     a "tip control", a small pop-up window that displays a
//     single line of text describing the hidden text when hovering an
//     item on the grid area.
// Remarks:
//     CXTPGridTip provides the functionality to control the
//     the tip text, the size of the tip window itself, and the text
//     font of the tip.
//
//     This class is used by the Grid control internally.
//
// See Also:
//     CXTPGridControl, CXTPGridRow::ShowToolTip
//===========================================================================
class _XTP_EXT_CLASS CXTPGridTip : public CWnd
{
	friend class CXTPGridRow;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridTip object.
	// Remarks:
	//     You construct a CXTPGridTip object in two steps.
	//     First, call the CXTPGridTip constructor and then call Create.
	//     This initializes the window and attaches it to the parent window.
	// Example:
	// <code>
	// // Declare a local CXTPGridTip object.
	// CXTPGridTip wndTip;
	//
	// // Declare a dynamic CXTPGridTip object.
	// CXTPGridTip* pTipWnd = new CXTPGridTip;
	// </code>
	//
	// See Also:
	//     CWnd, Create, Activate, CXTPGridRow::ShowToolTip
	//-----------------------------------------------------------------------
	CXTPGridTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridTip object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a grid tip window.
	// Parameters:
	//     pParentWnd - Parent Grid control window.
	// Remarks:
	//     You construct a CXTPGridTip object in two steps.
	//     First, call the CXTPGridTip constructor and then call Create.
	//     This initializes the window and attaches it to the parent window.
	// Returns:
	//     TRUE if the grid tip window was created successfully, otherwise FALSE.
	// See Also:
	//     CXTPGridTip::CXTPGridTip, Activate
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the tooltip text contains the new line character.
	// Returns:
	//     TRUE if the tooltip text contains the new line character, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsMultilineForce() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text.
	// Returns:
	//     The tooltip text.
	// See Also:
	//     SetTooltipText
	//-----------------------------------------------------------------------
	CString GetTooltipText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip text.
	// Parameters:
	//     str - New tooltip text.
	// Remarks:
	//     This method should be called before activating the tooltip window.
	// See Also:
	//     GetTooltipText
	//-----------------------------------------------------------------------
	void SetTooltipText(LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the font of the tooltip text.
	// Parameters:
	//     pFont - Pointer to the font to be set.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the coordinates of the tooltip hover rectangle.
	// Parameters:
	//     rc - CRect object containing the tooltip hover rectangle in
	//          parent window coordinates.
	// Remarks:
	//     This method should be called before activating the tooltip window.
	// See Also:
	//     GetHoverRect
	//-----------------------------------------------------------------------
	void SetHoverRect(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the coordinates of the tooltip hover rectangle.
	// Returns:
	//     A CRect object containing the tooltip hover rectangle in
	//     parent window coordinates.
	// See Also:
	//     SetHoverRect
	//-----------------------------------------------------------------------
	CRect GetHoverRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip rectangle.
	// Parameter:
	//     rc - CRect object containing the tooltip rectangle.
	//-----------------------------------------------------------------------
	void SetTooltipRect(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip rectangle.
	// Returns:
	//     A CRect object containing the tooltip rectangle.
	//-----------------------------------------------------------------------
	CRect GetTooltipRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to activate/deactivate the tooltip control.
	// Parameters:
	//     bActive    - TRUE to activate the tooltip control, FALSE to deactivate.
	//     bMultiline - TRUE if the tooltip is multiline, FALSE otherwise.
	// Remarks:
	//     When a tooltip control is active, tooltip information appears when
	//     the cursor is on a tool that is registered with the control.
	//     When a tooltip control is inactive, tooltip information does not appear when
	//     the cursor is on a tool that is registered with the control.
	// See Also:
	//     Create, CXTPGridTip::CXTPGridTip, SetTooltipText
	//-----------------------------------------------------------------------
	void Activate(BOOL bActive, BOOL bMultiline);

protected:
	CRect m_rcHover;			 // Hover window coordinates.
	CRect m_rcTooltip;			 // Tooltip window coordinates.
	CWnd* m_pParentWnd;			 // Pointer to the parent grid window.
	CString m_strTooltipText;	// Tooltip text to display.
	CXTPGridRecordItem* m_pItem; // Item which tooltip is visible.
	int m_nRowIndex;			 // Index of item's row.

	CXTPFont m_xtpFontToolTip; // Font for displaying tooltip text.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_fntToolTip, m_xtpFontToolTip,
										  GetToolTipFontHandle);

	BOOL m_bMultiline;				 // Multiline tooltip flag.
	CXTPGridControl* m_pGridControl; // Parent Grid Control.

protected:
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTPGridTip)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnPaint();
	//}}AFX_MSG

	friend class CXTPGridControl;
	//}}AFX_CODEJOCK_PRIVATE

private:
	void RecalcTooltipRect(CRect& rc);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
				CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()
};

AFX_INLINE CString CXTPGridTip::GetTooltipText() const
{
	return m_strTooltipText;
}
AFX_INLINE void CXTPGridTip::SetTooltipText(LPCTSTR str)
{
	m_strTooltipText = str;
}
AFX_INLINE void CXTPGridTip::SetHoverRect(CRect rc)
{
	m_rcHover = rc;
	MoveWindow(rc);
}
AFX_INLINE void CXTPGridTip::SetTooltipRect(CRect rc)
{
	m_rcTooltip = rc;
	MoveWindow(rc);
}
AFX_INLINE CRect CXTPGridTip::GetHoverRect() const
{
	return m_rcHover;
}
AFX_INLINE CRect CXTPGridTip::GetTooltipRect() const
{
	return m_rcTooltip;
}
AFX_INLINE BOOL CXTPGridTip::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
									const RECT& rect, CWnd* pParentWnd, UINT nID,
									CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPGRIDTIP_H__)
