// ThemeDef.h : Definitions for CThemeDlg class.
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

#if !defined(__THEMEDEF_H__)
#	define __THEMEDEF_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

//---------------------------------------------------------------------------
// Summary:
//		The WM_XTP_THEMECHANGED registered message sent to parent window of
//      the active theme dialog.
// Parameters:
//      lParam - THEME_COLOR*
//---------------------------------------------------------------------------
const UINT WM_XTP_THEMECHANGED = ::RegisterWindowMessage(_T("WM_XTP_THEMECHANGED"));

#	define ON_WM_XTP_THEMECHANGED()                                                               \
		{ 0xC000,                                                                                  \
		  0,                                                                                       \
		  0,                                                                                       \
		  0,                                                                                       \
		  reinterpret_cast<UINT_PTR>(&WM_XTP_THEMECHANGED),                                        \
		  (AFX_PMSG)(AFX_PMSGW)(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) & OnThemeChanged },

//---------------------------------------------------------------------------
// Use the following flags to exclude themes from the theme dialog.
// See CThemeDlg::ExcludeTheme(DWORD dwExclude).
//---------------------------------------------------------------------------
#	define XTP_EXCLUDE_THEME_OFFICE2000 0x00000001
#	define XTP_EXCLUDE_THEME_OFFICEXP 0x00000002
#	define XTP_EXCLUDE_THEME_OFFICE2003 0x00000004
#	define XTP_EXCLUDE_THEME_OFFICE2007 0x00000008
#	define XTP_EXCLUDE_THEME_OFFICE2010 0x00000010
#	define XTP_EXCLUDE_THEME_OFFICE2013 0x00000020
#	define XTP_EXCLUDE_THEME_OFFICE2016 0x00000040
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO6 0x00000080
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2005 0x00000100
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2008 0x00000200
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2010 0x00000400
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2012 0x00000800
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2015 0x00001000
#	define XTP_EXCLUDE_THEME_NATIVEWINDOWS 0x00002000
#	define XTP_EXCLUDE_THEME_CUSTOM 0x00004000
#	define XTP_EXCLUDE_THEME_NATIVEWINDOWS10 0x00008000
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2017 0x00010000
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2019 0x00020000
#	define XTP_EXCLUDE_THEME_VISUALSTUDIO2022 0x00040000

const int xtpThemeDlgUndef		  = -1;
const int xtpThemeColorUndef	  = -1;
const int xtpThemeAccentUndef	 = -1;
const int xtpThemeRibbonBackUndef = -1;

/////////////////////////////////////////////////////////////////////////////
// THEME_COLOR struct

struct THEME_COLOR
{
	// Enumeration that identifies which theme was selected.
	int nTheme;

	// Enumeration that identifies which color if
	// any has been selected.  This will correspond with the nTheme
	// value that has been sent.  For example if xtpThemeDlgOffice2016
	// theme was selected then the XTPThemeColor2016 enumeration is used.
	// If the value is -1, then no color was defined.
	int nColor;

	// Enumeration that identifies which style if
	// any has been selected.  For example if xtpThemeDlgOffice2016
	// theme was selected then the XTPThemeAccent2016 enumeration is used.
	// If the value is -1, then no color was defined.
	int nAccent;

	// Enumeration that identifies which background
	// if any has been selected.  This parameter is only valid for
	// later versions of Office theme starting with 2013.  See
	// XTPThemeRibbonBack for a complete list.
	int nBack;

	THEME_COLOR()
		: nTheme(xtpThemeDlgUndef)
		, nColor(xtpThemeColorUndef)
		, nAccent(xtpThemeAccentUndef)
		, nBack(xtpThemeRibbonBackUndef)
	{
	}

	THEME_COLOR(int nTheme, int nColor = -1, int nAccent = -1, int nBack = -1)
		: nTheme(nTheme)
		, nColor(nColor)
		, nAccent(nAccent)
		, nBack(nBack)
	{
	}

	THEME_COLOR(const THEME_COLOR& tc)
	{
		*this = tc;
	}

	THEME_COLOR& operator=(const THEME_COLOR& tc)
	{
		nTheme  = tc.nTheme;
		nBack   = tc.nBack;
		nColor  = tc.nColor;
		nAccent = tc.nAccent;
		return *this;
	}

	BOOL operator!=(const THEME_COLOR& tc) const
	{
		return !(*this == tc);
	}

	BOOL operator==(const THEME_COLOR& tc) const
	{
		return (nTheme == tc.nTheme && nColor == tc.nColor && nAccent == tc.nAccent
				&& nBack == tc.nBack && nAccent == tc.nAccent);
	}
};

//---------------------------------------------------------------------------
// Codejock Tookit Pro theme definitions.
//---------------------------------------------------------------------------
enum XTPThemeDlg
{
	xtpThemeDlgOffice2000,
	xtpThemeDlgOfficeXP,
	xtpThemeDlgOffice2003,
	xtpThemeDlgOffice2007,
	xtpThemeDlgOffice2010,
	xtpThemeDlgOffice2013,
	xtpThemeDlgOffice2016,
	xtpThemeDlgVisualStudio6,
	xtpThemeDlgVisualStudio2005,
	xtpThemeDlgVisualStudio2008,
	xtpThemeDlgVisualStudio2010,
	xtpThemeDlgVisualStudio2012,
	xtpThemeDlgVisualStudio2015,
	xtpThemeDlgVisualStudio2017,
	xtpThemeDlgVisualStudio2019,
	xtpThemeDlgVisualStudio2022,
	xtpThemeDlgNativeWindows,
	xtpThemeDlgNativeWindows10,
	xtpThemeDlgCustom,
};
//---------------------------
// 2007 Theme Colors
//---------------------------
enum XTPThemeColor2007
{
	xtpThemeColor2007Blue,
	xtpThemeColor2007Silver,
	xtpThemeColor2007Black,
	xtpThemeColor2007Aqua,
	xtpThemeColor2007System,
};
//---------------------------
// 2007 Theme Accent
//---------------------------
enum XTPThemeAccent2007
{
	xtpThemeAccent2007Default,
	xtpThemeAccent2007System,
	xtpThemeAccent2007Ribbon,
};
//---------------------------
// 2010 Theme Colors
//---------------------------
enum XTPThemeColor2010
{
	xtpThemeColor2010Blue,
	xtpThemeColor2010Silver,
	xtpThemeColor2010Black,
};
//---------------------------
// 2012 Theme Colors
//---------------------------
enum XTPThemeColor2012
{
	xtpThemeColor2012Light,
	xtpThemeColor2012Dark,
};
//---------------------------
// 2013 Theme Colors
//---------------------------
enum XTPThemeColor2013
{
	xtpThemeColor2013White,
	xtpThemeColor2013GrayLight,
	xtpThemeColor2013GrayDark,
};
//---------------------------
// 2015 Theme Colors
//---------------------------
enum XTPThemeColor2015
{
	xtpThemeColor2015Blue,
	xtpThemeColor2015Dark,
	xtpThemeColor2015Light,
};
//---------------------------
// 2016 Theme Colors
//---------------------------
enum XTPThemeColor2016
{
	xtpThemeColor2016Black,
	xtpThemeColor2016White,
	xtpThemeColor2016Colorful,
	xtpThemeColor2016GrayDark,
};
//---------------------------
// 2013, 2016 Theme Accent
//---------------------------
enum XTPThemeAccentOffice
{
	xtpThemeAccentAccess,
	xtpThemeAccentExcel,
	xtpThemeAccentOneNote,
	xtpThemeAccentOutlook,
	xtpThemeAccentPowerPoint,
	xtpThemeAccentPublisher,
	xtpThemeAccentWord,
};
//---------------------------
// 2013, 2016 Ribbon Background
//---------------------------
enum XTPThemeRibbonBack
{
	xtpThemeRibbonBackNone,
	xtpThemeRibbonBackCalligraphy,
	xtpThemeRibbonBackCirclesStripes,
	xtpThemeRibbonBackCircuit,
	xtpThemeRibbonBackClouds,
	xtpThemeRibbonBackDoodleCircles,
	xtpThemeRibbonBackDoodleDiamonds,
	xtpThemeRibbonBackGeometry,
	xtpThemeRibbonBackLunchBox,
	xtpThemeRibbonBackSchoolSupplies,
	xtpThemeRibbonBackSpring,
	xtpThemeRibbonBackStars,
	xtpThemeRibbonBackStraws,
	xtpThemeRibbonBackTreeRings,
	xtpThemeRibbonBackUnderWater,
};
//---------------------------
// Windows 10 Ribbon Colors
//---------------------------
enum XTPThemeColorWindows10
{
	xtpThemeColorWindows10Light,
	xtpThemeColorWindows10Dark,
};

//---------------------------
// 2017 Theme Colors
//---------------------------
enum XTPThemeColor2017
{
	xtpThemeColor2017Blue,
	xtpThemeColor2017BlueExtra,
	xtpThemeColor2017Dark,
	xtpThemeColor2017Light,
};

//---------------------------
// 2019 Theme Colors
//---------------------------
enum XTPThemeColor2019
{
	xtpThemeColor2019Blue,
	xtpThemeColor2019BlueExtra,
	xtpThemeColor2019Dark,
	xtpThemeColor2019Light,
};

//---------------------------
// 2022 Theme Colors
//---------------------------
enum XTPThemeColor2022
{
	xtpThemeColor2022Blue,
	xtpThemeColor2022BlueExtra,
	xtpThemeColor2022Dark,
	xtpThemeColor2022Light,
};

static LPCTSTR g_szThemes[] = {
	_T("Office 2000"),
	_T("Office XP"),
	_T("Office 2003"),
	_T("Office 2007"),
	_T("Office 2010"),
	_T("Office 2013"),
	_T("Office 2016"),
	_T("Visual Studio 6"),
	_T("Visual Studio 2005"),
	_T("Visual Studio 2008"),
	_T("Visual Studio 2010"),
	_T("Visual Studio 2012"),
	_T("Visual Studio 2015"),
	_T("Visual Studio 2017"),
	_T("Visual Studio 2019"),
	_T("Visual Studio 2022"),
	_T("Native Windows"),
	_T("Native Windows 10"),
	_T("Custom"),
};

static LPCTSTR g_szColor2007[] = {
	_T("Blue"), _T("Silver"), _T("Black"), _T("Aqua"), _T("System"),
};

static LPCTSTR g_szAccent2007[] = {
	_T("Default"),
	_T("System"),
	_T("Ribbon"),
};

static LPCTSTR g_szColor2010[] = {
	_T("Blue"),
	_T("Silver"),
	_T("Black"),
};

static LPCTSTR g_szColor2012[] = {
	_T("Light"),
	_T("Dark"),
};

static LPCTSTR g_szColor2013[] = {
	_T("White"),
	_T("Light Gray"),
	_T("Dark Gray"),
};

static LPCTSTR g_szColor2015[] = {
	_T("Blue"),
	_T("Dark"),
	_T("Light"),
};

static LPCTSTR g_szColor2017[] = {
	_T("Blue"),
	_T("Blue (Extra Contrast)"),
	_T("Dark"),
	_T("Light"),
};

static LPCTSTR g_szColor2019[] = {
	_T("Blue"),
	_T("Blue (Extra Contrast)"),
	_T("Dark"),
	_T("Light"),
};

static LPCTSTR g_szColor2022[] = {
	_T("Blue"),
	_T("Blue (Extra Contrast)"),
	_T("Dark"),
	_T("Light"),
};

static LPCTSTR g_szColor2016[] = {
	_T("Black"),
	_T("White"),
	_T("Colorful"),
	_T("Dark Gray"),
};

static LPCTSTR g_szAccent2016[] = {
	_T("Access"),	 _T("Excel"),	 _T("OneNote"), _T("Outlook"),
	_T("PowerPoint"), _T("Publisher"), _T("Word"),
};

static LPCTSTR g_szBack[] = {
	_T("None"),		_T("Calligraphy"),	 _T("Circles Stripes"), _T("Circuit"),
	_T("Clouds"),   _T("Doodle Circles"),  _T("Doodle Diamonds"), _T("Geometry"),
	_T("LunchBox"), _T("School Supplies"), _T("Spring"),		  _T("Stars"),
	_T("Straws"),   _T("Tree Rings"),	  _T("Under Water"),
};

static LPCTSTR g_szColorWindows10[] = {
	_T("Light"),
	_T("Dark"),
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__THEMEDEF_H__)
