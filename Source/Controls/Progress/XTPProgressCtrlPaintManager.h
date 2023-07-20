// XTPProgressCtrlPaintManager.h
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
#if !defined(__XTPPROGRESSCTRLPAINTMANAGER_H__)
#	define __XTPPROGRESSCTRLPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     Struct used by the progress bar during paint operations.
//===========================================================================
struct XTPPROGRESSDRAWSTRUCT
{
	CDC* pDC;				 // Pointer to a valid device context.
	CFont* pFont;			 // Pointer to the progress bar's CFont object.
	CRect rcClient;			 // Client area of the progress bar.
	CString strText;		 // Window text to be displayed on the progress bar.
	CProgressCtrl* pWndCtrl; // Points to the progress control.
	PBRANGE range;			 // High and low limits of a progress bar control.
	int nPos;				 // Position of the progress bar.
	BOOL bVertical;			 // TRUE if the progress bar is vertical, otherwise FALSE.
	BOOL bVista;			 // TRUE if the Windows verison is greater than or equal to Vista.
	BOOL bSmooth;			 // TRUE if the progress bar is drawn smooth.
	BOOL bMarquee;			 // TRUE if this is a Marquee style progress bar.
	BOOL bFlatStyle;		 // TRUE if the progress bar is to be drawn flat (deprecated).
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlPaintManager class is derived from CXTPCmdTarget and
//     is used by the progress bar to perform all default drawing routines.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlPaintManager : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPProgressCtrlPaintManager)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlPaintManager();

	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager destructor.
	// Remarks:
	//     Handles cleanup and deallocation.
	// -----------------------------------------------------------------
	virtual ~CXTPProgressCtrlPaintManager();

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
	//     This member function sets the text color for the progress bar.
	// Parameters:
	//     clrNew  - A COLORREF value that specifies the new text color.
	// Returns:
	//     The COLORREF value used for text.
	// -----------------------------------------------------------------
	COLORREF SetTextColor(COLORREF clrNew);

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

	// -----------------------------------------------------------------
	// Summary:
	//     This member function gets the text color for the progress bar.
	// Returns:
	//     The COLORREF value used for the text.
	// -----------------------------------------------------------------
	COLORREF GetTextColor();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the non-client borders.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rc);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the client area of the control.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void Draw(XTPPROGRESSDRAWSTRUCT& ds);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the progress text for the control.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void DrawText(XTPPROGRESSDRAWSTRUCT& ds) /* = 0*/;

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

	BOOL m_bUseWinTheme;				// TRUE when using Windows themes.
	XTPControlTheme m_nTheme;			// Theme identifier.
	CXTPWinThemeWrapper m_winTheme;		// Wrapper used when drawing Windows themes.
	CXTPPaintManagerColor m_clrBorder;  // An RGB value representing the border color.
	CXTPPaintManagerColor m_clrBar;		// An RGB value representing the bar color.
	CXTPPaintManagerColor m_clrBack;	// An RGB value representing the back color.
	CXTPPaintManagerColor m_clrText;	// An RGB value representing the text color.
	CXTPPaintManagerColor m_clrBarText; // An RGB value representing the hilite text color.
};

//---------------------------------------------------------------------------
// Inline functions.
//---------------------------------------------------------------------------

AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::SetBorderColor(COLORREF clrNew)
{
	m_clrBorder.SetCustomValue(clrNew);
	return m_clrBorder;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::SetBarColor(COLORREF clrNew)
{
	m_clrBar.SetCustomValue(clrNew);
	return m_clrBar;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::SetBackColor(COLORREF clrNew)
{
	m_clrBack.SetCustomValue(clrNew);
	return m_clrBack;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::SetTextColor(COLORREF clrNew)
{
	m_clrText.SetCustomValue(clrNew);
	return m_clrText;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::GetBorderColor()
{
	return m_clrBorder;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::GetBarColor()
{
	return m_clrBar;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::GetBackColor()
{
	return m_clrBack;
}
AFX_INLINE COLORREF CXTPProgressCtrlPaintManager::GetTextColor()
{
	return m_clrText;
}

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeFlat class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw a flat style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeFlat : public CXTPProgressCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeFlat)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeFlat();

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
	virtual void DeflateRect(XTPPROGRESSDRAWSTRUCT& ds);
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeUltraFlat class is derived from CXTPProgressCtrlThemeFlat
//     and is used by the progress bar to draw a flat style progress bar with
//     no borders.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeUltraFlat : public CXTPProgressCtrlThemeFlat
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeUltraFlat)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeUltraFlat();

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
//     The CXTPProgressCtrlThemeFlat class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw an Office 2000 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeOffice2000 : public CXTPProgressCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeOffice2000)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeOffice2000();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the non-client borders.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rc);
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeXP class is derived from CXTPProgressCtrlThemeFlat
//     and is used by the progress bar to draw an Office XP style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeOfficeXP : public CXTPProgressCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeOfficeXP)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeOfficeXP();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the non-client borders.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rc);
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeFlat class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw an Office 2003 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeOffice2003 : public CXTPProgressCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeOffice2003)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeOffice2003();

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
	//     rc  - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rc);
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeFlat class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw an progress bar using a resource DLL.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeResource : public CXTPProgressCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeResource)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeResource();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the client area of the control.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void Draw(XTPPROGRESSDRAWSTRUCT& ds);
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeFlat class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw an Office 2013 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeOffice2013 : public CXTPProgressCtrlThemeOffice2000
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeOffice2013)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeOffice2013();

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
	//     rc  - Size of the non-client border area.
	// -----------------------------------------------------------------
	virtual void DrawNcBorders(CDC* pDC, CRect rc);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called by the progress bar to draw
	//     the client area of the control.
	// Parameters:
	//     ds - Struct used by the progress bar during paint operations.
	// See Also:
	//     XTPPROGRESSDRAWSTRUCT
	// -----------------------------------------------------------------
	virtual void Draw(XTPPROGRESSDRAWSTRUCT& ds);
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeFlat class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw a Visual Studio 2015 style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeVisualStudio2015 : public CXTPProgressCtrlThemeOffice2013
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2015)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeVisualStudio2015();

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
//     The CXTPProgressCtrlThemeNativeWinXP class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw a native Windows style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeNativeWinXP : public CXTPProgressCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeNativeWinXP)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeNativeWinXP();

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
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeNativeWindows class is derived from CXTPProgressCtrlPaintManager
//     and is used by the progress bar to draw a native Windows style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeNativeWindows : public CXTPProgressCtrlPaintManager
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeNativeWindows)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeNativeWindows();

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
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeVisualStudio2017 class is derived from
//     CXTPProgressCtrlThemeOffice2013 and is used by the progress bar to draw a Visual Studio 2017
//     style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeVisualStudio2017 : public CXTPProgressCtrlThemeOffice2013
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2017)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeVisualStudio2017();
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeVisualStudio2019 class is derived from
//     CXTPProgressCtrlThemeOffice2013 and is used by the progress bar to draw a Visual Studio 2019
//     style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeVisualStudio2019 : public CXTPProgressCtrlThemeOffice2013
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2019)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeVisualStudio2019();
};

//===========================================================================
// Summary:
//     The CXTPProgressCtrlThemeVisualStudio2022 class is derived from
//     CXTPProgressCtrlThemeOffice2013 and is used by the progress bar to draw a Visual Studio 2022
//     style progress bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrlThemeVisualStudio2022 : public CXTPProgressCtrlThemeOffice2013
{
	DECLARE_DYNAMIC(CXTPProgressCtrlThemeVisualStudio2022)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also:
	//     RefreshMetrics()
	// -----------------------------------------------------------------
	CXTPProgressCtrlThemeVisualStudio2022();
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPROGRESSCTRLPAINTMANAGER_H__)
