// XTPRibbonBackstageButtonThemeVisualStudio2022.h: interface for the
// CXTPRibbonBackstageButtonThemeVisualStudio2022 class.
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
#if !defined(__XTPRIBBONBACKSTAGEBUTTONTHEMEVISUALSTUDIO2022T_H__)
#	define __XTPRIBBONBACKSTAGEBUTTONTHEMEVISUALSTUDIO2022T_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPRibbonBackstageButtonThemeVisualStudio2022
	: public CXTPButtonThemeVisualStudio2022
{
public:
	CXTPRibbonBackstageButtonThemeVisualStudio2022();

public:
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

protected:
	friend class CBackstageButtonCtrl;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPRIBBONBACKSTAGEBUTTONTHEMEVISUALSTUDIO2022T_H__)
