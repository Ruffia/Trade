// XTPSyntaxEditToolTipCtrl.h: interface for the CXTPSyntaxEditToolTipCtrl class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITTOOLTIPCTRL_H__)
#	define __XTPSYNTAXEDITTOOLTIPCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditCtrl;
// Internal class

//===========================================================================
// Summary:
//     The CXTPSyntaxEditToolTipCtrl class encapsulates the functionality of
//     a "tip control," a small pop-up window that displays a
//     single line of text describing the hidden text when hovering over
//     an item on the report area.
//
//     CXTPSyntaxEditToolTipCtrl provides the functionality to control the
//     the tip text, the size of the tip window itself, and
//     the text font of the tip.
//
//     This class is used by the Grid control internally.
// See Also:
//     CXTPSyntaxEditCtrl
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditToolTipCtrl : public CWnd
{
	// Construction
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSyntaxEditToolTipCtrl object.
	// Remarks:
	//     You construct a object object in two steps. First, call the constructor
	//     object and then call Create, which initializes the window and attaches
	//     it to the parent window.
	// Example:
	//     // Declare a local CXTPSyntaxEditToolTipCtrl object.
	//     CXTPSyntaxEditToolTipCtrl wndTip;
	//
	//     // Declare a dynamic CXTPSyntaxEditToolTipCtrl object.
	//     CXTPSyntaxEditToolTipCtrl* pTipWnd = new CXTPSyntaxEditToolTipCtrl;
	// See Also:
	//     Create, Activate
	//-----------------------------------------------------------------------
	CXTPSyntaxEditToolTipCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditToolTipCtrl object, handles cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditToolTipCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a smart edit tip window.
	// Parameters:
	//     pParentWnd - [in] Pointer to the parent Grid control window.
	// Returns:
	//     TRUE if the tip window was created successfully, otherwise FALSE.
	// See Also:
	//     CXTPSyntaxEditCtrl::ShowCollapsedToolTip
	//-----------------------------------------------------------------------
	virtual BOOL Create(CXTPSyntaxEditCtrl* pParentWnd);

	using CWnd::Create;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the display delay, in milliseconds, for the tooltip.
	// Parameters:
	//     nDelay - [in] Display delay, in milliseconds, to be set.
	// See Also:
	//     int GetDelay();
	//-----------------------------------------------------------------------
	void SetDelay(int nDelay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the display delay, in milliseconds, of the tooltip.
	// Returns:
	//     The display delay, in milliseconds, of the tooltip.
	// See also:
	//     void SetDelay(int nDelay);
	//-----------------------------------------------------------------------
	int GetDelay() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Hides the tooltip window.
	//-----------------------------------------------------------------------
	void Hide();

	//-----------------------------------------------------------------------
	// Summary:
	//     Activates the tooltip window.
	// Parameters:
	//     strText - [in] Reference to the text to be shown
	//                    within the tooltip window.
	//-----------------------------------------------------------------------
	void Activate(const CString& strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the coordinates of the tooltip hover rectangle.
	// Parameters:
	//     rc - [in] New tooltip hover rectangle in parent window coordinates.
	// Remarks:
	//     This method should be called before activating the tooltip window.
	// See also:
	//     GetHoverRect()
	//-----------------------------------------------------------------------
	void SetHoverRect(CRect rc);

	//{{AFX_VIRTUAL(CXTPSyntaxEditToolTipCtrl)
	//}}AFX_VIRTUAL

protected:
	int m_nDelayTime;				  // Delay, in milliseconds, before showing the tooltip.
	CXTPSyntaxEditCtrl* m_pParentWnd; // Pointer to the parent report window.
	CRect m_rcHover;				  // Hover window coordinates.
	CString m_strToolTipText;		  // Tooltip text.

	//{{AFX_MSG(CXTPSyntaxEditToolTipCtrl)

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void ReCalcToolTipRect();
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPSyntaxEditToolTipCtrl::SetDelay(int nDelay)
{
	m_nDelayTime = nDelay;
}
AFX_INLINE int CXTPSyntaxEditToolTipCtrl::GetDelay() const
{
	return m_nDelayTime;
}
AFX_INLINE void CXTPSyntaxEditToolTipCtrl::SetHoverRect(CRect rc)
{
	m_rcHover = rc;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITTOOLTIPCTRL_H__)
