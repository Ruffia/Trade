// XTPRibbonControlTab.h: interface for the CXTPRibbonControlTab class.
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
#if !defined(__XTPRIBBONCONTROLTAB_H__)
#	define __XTPRIBBONCONTROLTAB_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPRibbonGroups;
class CXTPRibbonBar;
class CXTPRibbonGroup;
class CXTPRibbonTab;

//---------------------------------------------------------------------------
// Summary:
//     Structure used as a parameter for TCN_SELCHANGING and TCN_SELCHANGE
//     messages when a Ribbon tab is changed.
//---------------------------------------------------------------------------
struct NMXTPTABCHANGE : public NMXTPCONTROL
{
	CXTPRibbonTab* pTab; // Ribbon tab to be selected.
};

//===========================================================================
// Summary:
//     CXTPRibbonControlTab is a CXTPTabManager derived class.
//     It represents tabs of the Ribbon Bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlTab
	: public CXTPControlPopup
	, public CXTPTabManager
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlTab)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlTab object.
	//-----------------------------------------------------------------------
	CXTPRibbonControlTab();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonControlTab object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonControlTab();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the tab paint manager.
	// Remarks:
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager (i.e. tab colors,
	//     styles, etc.). This member must be overridden in derived classes.
	// Returns:
	//     A pointer to the CXTPTabPaintManager that contains the
	//     visual elements of the tabs.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the parent CXTPRibbonBar object.
	// Returns:
	//     A pointer to the parent CXTPRibbonBar object.
	//-----------------------------------------------------------------------
	CXTPRibbonBar* GetRibbonBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find a tab with a specified identifier.
	// Parameters:
	//     nId - Identifier of the tab to be found.
	// Returns:
	//     A pointer to the CXTPRibbonTab object with the specified identifier.
	// See Also:
	//     FindGroup
	//-----------------------------------------------------------------------
	CXTPRibbonTab* FindTab(int nId) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the tab at a specified position.
	// Parameters:
	//     nIndex - Index of the tab to retrieve.
	// Returns:
	//     A pointer to the CXTPRibbonTab object at the specified position.
	// See Also:
	//     GetTabCount
	//-----------------------------------------------------------------------
	CXTPRibbonTab* GetTab(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select an item in the ribbon bar.
	// Parameters:
	//     pItem - Pointer to the CXTPTabManagerItem object to be selected.
	//-----------------------------------------------------------------------
	void SetSelectedItem(CXTPTabManagerItem* pItem);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update the position of the tab manager.
	//-----------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     The implementation should return TRUE if
	//     the header has focus or FALSE otherwise.
	// Returns:
	//     TRUE if the header has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HeaderHasFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set focus to the control.
	// Parameters:
	//     bFocused - TRUE to set focus to the control.
	//-----------------------------------------------------------------------
	virtual void SetFocused(BOOL bFocused);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the control has focus.
	// Returns:
	//     TRUE if the control has focus, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user actives the control using
	//     its underline.
	//-----------------------------------------------------------------------
	void OnUnderlineActivate();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to check if the control can accept focus.
	// Returns:
	//     TRUE if the control can accept focus, otherwise FALSE.
	// See Also:
	//     SetFocused
	//-----------------------------------------------------------------------
	virtual BOOL IsFocusable() const;

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
	//     This method is called when a non-system key is pressed.
	// Parameters:
	//     nChar  - Specifies the virtual key code of the given key.
	//     lParam - Specifies additional message-dependent information.
	// Returns:
	//     TRUE if the key was handled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item (tab button) is clicked.
	// Parameters:
	//     pItem - Item that was clicked.
	//-----------------------------------------------------------------------
	void OnItemClick(CXTPTabManagerItem* pItem);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the icon of the ribbon tab needs to be drawn.
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Specifies the location of the image.
	//     pItem  - CXTPTabManagerItem object to draw icon on.
	//     bDraw  - TRUE if the icon needs to be drawn (e.g. the icon size changed),
	//              FALSE if the icon does not need to be drawn or redrawn.
	//     szIcon - Size of the tab icon.
	// Remarks:
	//     This member is overridden by its descendants.
	//     This member must be overridden in derived classes.
	// Returns:
	//     TRUE if the icon was successfully drawn, FALSE if the icon was not drawn.
	//-----------------------------------------------------------------------
	BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates redrawing of the ribbon bar control.
	// Remarks:
	//     Call this member function if you want to initialize redrawing
	//     of the control. The control will be redrawn taking into account
	//     its latest state.
	// Parameters:
	//     lpRect   - The rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in the bounding rectangle.
	//-----------------------------------------------------------------------
	void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks to see if the mouse is locked.
	// Remarks:
	//     The mouse is locked when a CXTPCommandBarsPopup is currently visible.
	// Returns:
	//     TRUE if the mouse is locked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsMouseLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/load a properties collection
	//     using the specified data object.
	// Parameters:
	//     pPX - Source/destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store/load properties collection
	//     data to/form storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl   - Pointer to a source CXTPControl object.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tracking state.
	// Returns:
	//     TRUE if the ribbon is in tracking mode.
	//-----------------------------------------------------------------------
	BOOL IsPopupBarTracking() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));
	void ShowPopupBar(BOOL bKeyboard);
	BOOL OnSetPopup(BOOL bPopup);
	void SetEnabled(BOOL bEnabled);
	CString GetItemTooltip(const CXTPTabManagerItem* pItem) const;
	virtual void AdjustExcludeRect(CRect& rc, BOOL bVertical);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
									   VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual HRESULT GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
	virtual HRESULT AccessibleDoDefaultAction(VARIANT varChild);
	virtual HRESULT AccessibleSelect(long flagsSelect, VARIANT varChild);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bFocused; // TRUE if groups is focused.

	friend class CXTPRibbonBar;
};

AFX_INLINE CXTPRibbonBar* CXTPRibbonControlTab::GetRibbonBar() const
{
	return (CXTPRibbonBar*)m_pParent;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONCONTROLTAB_H__)
