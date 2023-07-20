// XTPTabColorSetWinNative.h
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
#if !defined(__XTPTABCOLORSETWINNATIVE_H__)
#	define __XTPTABCOLORSETWINNATIVE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CColorSetWinNative implements a Windows XP tab color set.
// Remarks:
//     To use the Windows XP color set, SetColor is used to apply
//     the xtpTabColorWinNative XTPTabColorStyle.
//
//     The following appearances (XTPTabAppearanceStyle) support CColorSetWinNative:
//     * <b>xtpTabAppearancePropertyPage</b>: Office 2000 appearance.
//     * <b>xtpTabAppearancePropertyPageSelected</b>: Office 2000 selected appearance.
//     * <b>xtpTabAppearanceStateButtons</b>: State Button appearance.
// See Also:
//     XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance,
//     GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//===========================================================================
class _XTP_EXT_CLASS CColorSetWinNative : public CXTPTabColorSetDefault
{
public:
	CColorSetWinNative();
	virtual ~CColorSetWinNative();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the tabs.
	//     Override this method to change the colors of the color members.
	// Remarks:
	//     All of the color members are refreshed when this method is called.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Pointer to the tab button to fill.
	// Returns:
	//     XTP_TABMANAGER_COLOR_NONE
	// Remarks:
	//     This method is used to fill all tab buttons except when
	//     XTPTabAppearanceStyle is set to xtpTabAppearanceStateButtons.
	//     xtpTabAppearanceStateButtons requires the FillStateButton member.
	//
	//     If IsAppThemed is FALSE, then
	//     CXTPTabColorSetDefault::FillPropertyButton is used.
	// See Also:
	//     FillStateButton
	//-----------------------------------------------------------------------
	virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the tab client area of a specified tab manager.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Bounding rectangle of the tab client area.
	//     pTabManager - Pointer to the tab manager with the tab client area to fill.
	// Remarks:
	//     The bounding rectangle of the tab client area can be retrieved by using
	//     CXTPTabPaintManagerTheme::GetClientRect or CXTPTabManager::GetClientRect.
	// Returns:
	//     If IsAppThemed is TRUE, then a value of 0 is returned.
	//
	//     If IsAppThemed is FALSE and a tab is selected, then the color from
	//     CXTPTabManager::GetItemColor for the currently selected tab is returned.
	//
	//     If IsAppThemed is FALSE and a tab is not selected, then
	//     CXTPTabPaintManagerColorSet::m_clrButtonSelected is returned.
	//-----------------------------------------------------------------------
	virtual COLORREF FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a clipping region for the specified tab client area.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Bounding rectangle of the tab client area. This will
	//                   be the clipping region for the device context.
	//     pTabManager - Pointer to the tab manager.
	//-----------------------------------------------------------------------
	virtual void SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab navigation button.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Pointer to the tab navigation button to fill.
	//     rc      - Reference to the bounding rectangle of the tab navigation button.
	// Remarks:
	//     This method is used to fill the tab navigation buttons in the header of
	//     the tab client and is overridden by XTPTabColorStyle and
	//     CXTPTabPaintManagerColorSet to perform actions such as painting
	//     the highlighted, pressed, and normal versions of tab navigation buttons.
	//
	//     If IsAppThemed is FALSE, then
	//     CXTPTabColorSetDefault::FillNavigationButton is used.
	// See Also:
	//     CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified tab button when XTPTabAppearanceStyle
	//     is xtpTabAppearanceStateButtons.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Pointer to the tab button to fill.
	// Remarks:
	//     This method is only used to fill buttons when XTPTabAppearanceStyle
	//     is xtpTabAppearanceStateButtons. Otherwise, FillPropertyButton is used.
	//
	//     If IsAppThemed is FALSE and/or the tab button is not selected, then
	//     CXTPTabColorSetDefault::FillStateButton is used.
	// See Also:
	//     FillPropertyButton
	//-----------------------------------------------------------------------
	virtual void FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Windows XP themes are enabled and if
	//     a Windows XP theme is currently used.
	// Returns:
	//     TRUE if Windows XP themes are enabled and if
	//     a Windows XP theme is currently used, otherwise FALSE.
	// Remarks:
	//     IsAppThemed will return FALSE if running an operating system before
	//     Windows XP (i.e. 98, 2000, NT) or if Windows XP is used
	//     with Standard/Classic themes.
	//
	//     This method is called internally to determine how to colorize
	//     tab buttons. If this method returns TRUE, then Windows XP colorization
	//     is used to colorize tab buttons. If this method returns FALSE, then
	//     CXTPTabPaintManager::CXTPTabColorSetDefault is used to colorize tab buttons.
	// See Also:
	//     CXTPTabPaintManager::IsAppThemed
	//-----------------------------------------------------------------------
	virtual BOOL IsAppThemed() const;

private:
	typedef void(AFX_CDECL* LPFNDRAWROTATEDBITS)(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);

	//{{AFX_CODEJOCK_PRIVATE
public:
	void DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap,
						   LPFNDRAWROTATEDBITS pfnRotatedProc);
	static void AFX_CDECL DrawRotatedBitsBottom(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	static void AFX_CDECL DrawRotatedBitsRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	static void AFX_CDECL DrawRotatedBitsLeft(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	static void AFX_CDECL DrawRotatedBitsInvertRight(int cx, int cy, UINT* pSrcBits,
													 UINT* pDestBits);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPWinThemeWrapper* m_themeTabButton; // Internally used helper to draw property page buttons
										   // using Windows XP color set.
	CXTPWinThemeWrapper* m_themeToolbar;   // Internally used helper to draw state buttons using
										   // Windows XP color set.
};

//{{AFX_CODEJOCK_PRIVATE
// obsolete, for backward compatibility only.
class _XTP_EXT_CLASS CColorSetWinXP : public CColorSetWinNative
{
};
//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCOLORSETWINNATIVE_H__)
