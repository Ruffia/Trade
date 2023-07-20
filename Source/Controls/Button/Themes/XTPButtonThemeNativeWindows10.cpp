// XTPButtonThemeNativeWindows10.cpp: implementation of the CXTPButtonThemeNativeWindows10 class.
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

#include "StdAfx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"
#include "Controls/Button/Themes/XTPButtonThemeOffice2013.h"
#include "Controls/Button/Themes/XTPButtonThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

CXTPButtonThemeNativeWindows10::CXTPButtonThemeNativeWindows10()
{
	m_nTheme = xtpControlThemeNativeWindows10;
}

void CXTPButtonThemeNativeWindows10::RefreshMetrics(CXTPButton* pButton)
{
	UNREFERENCED_PARAMETER(pButton);

	m_themeButton->OpenThemeData(NULL, m_bToolbarStyle ? L"TOOLBAR" : L"BUTTON");

	// Defaults are set to NativeWindows10LIGHT_INI colors.

	m_crBack.SetStandardValue(XTPIniColor(_T("Controls.Button"), _T("Back"), RGB(238, 238, 242)));
	m_crBackHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BackHilite"), RGB(201, 222, 245)));
	m_crBackPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BackPushed"), RGB(0, 122, 204)));
	m_crBackDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BackDisabled"), RGB(238, 238, 242)));
	m_crBorder.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("Border"), RGB(204, 206, 219)));
	m_crBorderHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderHilite"), RGB(0, 122, 204)));
	m_crBorderPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderPushed"), RGB(0, 122, 204)));
	m_crBorderDefault.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderDefault"), RGB(0, 122, 204)));
	m_crBorderDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderDisabled"), RGB(204, 206, 219)));
	m_crText.SetStandardValue(XTPIniColor(_T("Controls.Button"), _T("Text"), RGB(30, 30, 30)));
	m_crTextCheck.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextCheck"), RGB(30, 30, 30)));
	m_crTextHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextHilite"), RGB(30, 30, 30)));
	m_crTextPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextPushed"), RGB(255, 255, 255)));
	m_crTextDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextDisabled"), RGB(162, 164, 165)));
	m_crMark.SetStandardValue(XTPIniColor(_T("Controls.Button"), _T("Mark"), RGB(113, 113, 113)));
	m_crMarkHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("MarkHilite"), RGB(30, 30, 30)));
	m_crMarkPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("MarkPushed"), RGB(255, 255, 255)));
	m_crMarkDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("MarkDisabled"), RGB(204, 206, 219)));
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
