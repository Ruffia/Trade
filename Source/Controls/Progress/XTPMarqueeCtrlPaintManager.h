// XTPMarqueeCtrlPaintManager.h
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
#if !defined(__XTPMARQUEECTRLPAINTMANAGER_H__)
#	define __XTPMARQUEECTRLPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPMarqueeCtrl;

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlPaintManager class is derived from CCmdTarget and
//     is used by the progress bar to perform all default drawing routines.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlPaintManager : public CCmdTarget
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlPaintManager)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlPaintManager();

	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager destructor.
	// Remarks:
	//     Handles cleanup and deallocation.
	// -----------------------------------------------------------------
	virtual ~CXTPMarqueeCtrlPaintManager();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the non-client borders.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcClient - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawBackground(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CRect& rcClient);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the client area of the control.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DrawMarquee(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CRect& rcClient);

	// -----------------------------------------------------------------
	// Summary:
	//     Used by the progress bar to determine if Windows themes should
	//     be used to draw the control.
	// Parameters:
	//     bCheckReady - TRUE to perform an additional check IsAppThemeReady().
	// Returns:
	//     TRUE if the progress bar is drawn using default Windows themes.
	// -----------------------------------------------------------------
	BOOL UsingWinThemes(BOOL bCheckReady = FALSE);

	// -----------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the control using the current
	//     Windows theme. This will override any themes currently set.
	// Parameters:
	//     bUseWinThemes - TRUE to enable Windows themes.
	// -----------------------------------------------------------------
	void UseWinThemes(BOOL bUseWinThemes = TRUE);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function sets the border color for the progress bar.
	// Parameters:
	//     clrNew  - A COLORREF value that specifies the new border color.
	// Returns:
	//     The COLORREF value used for the border.
	// -----------------------------------------------------------------
	COLORREF SetBorderColor(COLORREF clrNew);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function sets the bar color for the progress bar.
	// Parameters:
	//     clrNew  - A COLORREF value that specifies the new bar color.
	// Returns:
	//     The COLORREF value used for the bar.
	// -----------------------------------------------------------------
	COLORREF SetBarColor(COLORREF clrNew);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function sets the background color for the progress bar.
	// Parameters:
	//     clrNew  - A COLORREF value that specifies the new background color.
	// Returns:
	//     The COLORREF value used for the background.
	// -----------------------------------------------------------------
	COLORREF SetBackColor(COLORREF clrNew);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function gets the border color for the progress bar.
	// Returns:
	//     The COLORREF value used for the border.
	// -----------------------------------------------------------------
	COLORREF GetBorderColor();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function gets the bar color for the progress bar.
	// Returns:
	//     The COLORREF value used for the bar.
	// -----------------------------------------------------------------
	COLORREF GetBarColor();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function gets the background color for the progress bar.
	// Returns:
	//     The COLORREF value used for the background.
	// -----------------------------------------------------------------
	COLORREF GetBackColor();

protected:
	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to adjust
	//     the bar drawing size; can be overridden in derived classes.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DeflateRect(CDC* pDC, CRect& rcClient);

	virtual void DrawChunk(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CXTPCompatibleDC& dcChunk, int i,
						   int& x, int& y, int cx, int cy);

	BOOL m_bUseWinTheme;				 // TRUE when using Windows themes.
	XTPControlTheme m_nTheme;			 // Theme identifier.
	CXTPWinThemeWrapper m_winTheme;		 // Wrapper used when drawing Windows themes.
	CXTPPaintManagerColor m_clr3DHilite; // An RGB value representing the border color.
	CXTPPaintManagerColor m_clr3DShadow; // An RGB value representing the border color.
	CXTPPaintManagerColor m_clrBar;		 // An RGB value representing the bar color.
	CXTPPaintManagerColor m_clrBack;	 // An RGB value representing the back color.
};

//---------------------------------------------------------------------------
// Inline functions.
//---------------------------------------------------------------------------

AFX_INLINE COLORREF CXTPMarqueeCtrlPaintManager::SetBorderColor(COLORREF clrNew)
{
	m_clr3DHilite.SetCustomValue(clrNew);
	m_clr3DShadow.SetCustomValue(clrNew);
	return m_clr3DShadow;
}
AFX_INLINE COLORREF CXTPMarqueeCtrlPaintManager::SetBarColor(COLORREF clrNew)
{
	m_clrBar.SetCustomValue(clrNew);
	return m_clrBar;
}
AFX_INLINE COLORREF CXTPMarqueeCtrlPaintManager::SetBackColor(COLORREF clrNew)
{
	m_clrBack.SetCustomValue(clrNew);
	return m_clrBack;
}
AFX_INLINE COLORREF CXTPMarqueeCtrlPaintManager::GetBorderColor()
{
	return m_clr3DShadow;
}
AFX_INLINE COLORREF CXTPMarqueeCtrlPaintManager::GetBarColor()
{
	return m_clrBar;
}
AFX_INLINE COLORREF CXTPMarqueeCtrlPaintManager::GetBackColor()
{
	return m_clrBack;
}

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeFlat class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw a flat style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeFlat : public CXTPMarqueeCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeFlat)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeFlat();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to adjust
	//     the bar drawing size; can be overridden in derived classes.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DeflateRect(CDC* pDC, CRect& rcClient);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the client area of the control.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DrawMarquee(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CRect& rcClient);
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeUltraFlat class is derived from CXTPMarqueeCtrlThemeFlat
//     and is used by the progress bar to draw a flat style progress bar with
//     no borders.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeUltraFlat : public CXTPMarqueeCtrlThemeFlat
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeUltraFlat)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeUltraFlat();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeFlat class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw an Office 2000 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeOffice2000 : public CXTPMarqueeCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeOffice2000)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeOffice2000();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeXP class is derived from CXTPMarqueeCtrlThemeFlat
//     and is used by the progress bar to draw an Office XP style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeOfficeXP : public CXTPMarqueeCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeOfficeXP)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeOfficeXP();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeFlat class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw an Office 2003 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeOffice2003 : public CXTPMarqueeCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeOffice2003)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeOffice2003();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeFlat class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw an progress bar using a resource DLL.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeResource : public CXTPMarqueeCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeResource)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeResource();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the non-client borders.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcClient - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawBackground(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CRect& rcClient);

	virtual void DrawChunk(CXTPMarqueeCtrl* pWndCtrl, CDC* pDC, CXTPCompatibleDC& dcChunk, int i,
						   int& x, int& y, int cx, int cy);
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeFlat class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw an Office 2013 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeOffice2013 : public CXTPMarqueeCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeOffice2013)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeOffice2013();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.).
	// -----------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeFlat class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw a Visual Studio 2015 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeVisualStudio2015 : public CXTPMarqueeCtrlThemeOffice2013
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeVisualStudio2015)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeVisualStudio2015();
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeNativeWinXP class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw a native Windows style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeNativeWinXP : public CXTPMarqueeCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeNativeWinXP)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeNativeWinXP();

protected:
	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to adjust
	//     the bar drawing size; can be overridden in derived classes.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DeflateRect(XTPPROGRESSDRAWSTRUCT& ds);

	using CXTPMarqueeCtrlPaintManager::DeflateRect;
};

//===========================================================================
// Summary:
//     The CXTPMarqueeCtrlThemeNativeWindows class is derived from CXTPMarqueeCtrlPaintManager
//     and is used by the progress bar to draw a native Windows style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarqueeCtrlThemeNativeWindows : public CXTPMarqueeCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrlThemeNativeWindows)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPMarqueeCtrlThemeNativeWindows();

protected:
	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to adjust
	//     the bar drawing size; can be overridden in derived classes.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DeflateRect(CDC* pDC, CRect& rcClient);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPMARQUEECTRLPAINTMANAGER_H__)
