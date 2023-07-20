// XTPShortcutBarNativeWinXPTheme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBARNATIVEWINXPTHEME_H__)
#	define __XTPSHORTCUTBARNATIVEWINXPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPShortcutBarNativeWinXPTheme is a CXTPShortcutBarOffice2003Theme
//     derived class that represents a Native Windows XP shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarNativeWinXPTheme : public CXTPShortcutBarOffice2003Theme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarNativeWinXPTheme object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarNativeWinXPTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarNativeWinXPTheme object,
	//     handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPShortcutBarNativeWinXPTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the visual metrics of the shortcut bar paint manager.
	// Remarks:
	//     This method refreshes all of the visual metrics of the shortcut bar
	//     (pane caption color, pane font, pane caption height, etc.).
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the client area of the shortcut bar control.
	// Parameters:
	//     pDC          - Pointer to the client device context.
	//     pShortcutBar - Pointer to the shortcut bar control.
	//-----------------------------------------------------------------------
	void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills a specified item for the shortcut bar control.
	// Parameters:
	//     pDC   - Pointer to the client device context.
	//     pItem - Pointer to the CXTPShortcutBarItem object to be filled.
	//-----------------------------------------------------------------------
	void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

protected:
	CXTPWinThemeWrapper* m_themeToolbar;
	CXTPWinThemeWrapper* m_themeRebar;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //  !defined(__XTPSHORTCUTBARNATIVEWINXPTHEME_H__)
