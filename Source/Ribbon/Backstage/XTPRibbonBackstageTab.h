// XTPRibbonBackstageTab.h: interface for the CXTPRibbonBackstageTab class.
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
#if !defined(__XTPRIBBONBACKSTAGETAB_H__)
#	define __XTPRIBBONBACKSTAGETAB_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPRibbonBackstageTab : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPRibbonBackstageTab)

public:
	CWnd* GetControlPane() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Checks that keyboards tips for buttons on the pane activated by this tab do
	//      not conflict with each other. It also constructs an array of IDs for button
	//      windows on the pane and array of keyboard tips corresponding to those buttons.
	//-----------------------------------------------------------------------
	void CheckKeyboardTips();

	//-----------------------------------------------------------------------
	// Summary:
	//      Retrieves the size of the arrays created by the CheckKeyboardTips method.
	// Returns:
	//      The size of the arrays.
	// See also:
	//      CheckKeyboardTips
	//-----------------------------------------------------------------------
	int GetKeyboardTipsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the character used as the keyboard tip for the button
	//     with the specified index in the m_arrKeyboardTipsIds array.
	// Parameters:
	//     index - Index in the m_arrKeyboardTipsIds array.
	// Returns:
	//     The character used as the keyboard tip for the button.
	// See also:
	//     CheckKeyboardTips
	//-----------------------------------------------------------------------
	TCHAR GetKeyboardTipChar(int index) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Simulates clicking on the button with the specified index in
	//     the m_arrKeyboardTipsIds array.
	// Parameters:
	//     index - Index in the m_arrKeyboardTipsIds array.
	// See also:
	//     CheckKeyboardTips
	//-----------------------------------------------------------------------
	void ClickControlWithKeyboardTip(int index);

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides keyboard tips on the pane corresponding to this tab.
	// Parameters:
	//     bShow - TRUE to show keyboard tips, FALSE to hide.
	//-----------------------------------------------------------------------
	void ShowKeyboardTips(BOOL bShow);

	void SetControlPaneMinSize(CSize sz);

protected:
	CXTPRibbonBackstageTab(HWND hwndControl = 0);

protected:
#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonBackstageTab);

//}}AFX_CODEJOCK_PRIVATE
#	endif
protected:
	HWND m_hwndControl;
	CSize m_szControlPaneMinSize;
	CArray<TCHAR, TCHAR> m_arrKeyboardTips;  // Array of keyboard tips for buttons on the pane.
	CArray<HWND, HWND> m_arrKeyboardTipsIds; // Array of IDs for buttons on the pane.

	friend class CXTPRibbonBackstageView;
};

AFX_INLINE void CXTPRibbonBackstageTab::SetControlPaneMinSize(CSize sz)
{
	m_szControlPaneMinSize = sz;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONBACKSTAGETAB_H__)
