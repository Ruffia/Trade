// XTPRibbonBackstageButtonThemeVisualStudio2017.h: interface for the
// CXTPRibbonBackstageButtonThemeVisualStudio2017 class.
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
#if !defined(__XTPRIBBONBACKSTAGEBUTTONTHEMEVISUALSTUDIO2017T_H__)
#	define __XTPRIBBONBACKSTAGEBUTTONTHEMEVISUALSTUDIO2017T_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPRibbonBackstageButtonThemeVisualStudio2017
	: public CXTPButtonThemeVisualStudio2017
{
public:
	CXTPRibbonBackstageButtonThemeVisualStudio2017();

public:
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

protected:
	friend class CBackstageButtonCtrl;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONBACKSTAGEBUTTONTHEMEVISUALSTUDIO2017T_H__)
