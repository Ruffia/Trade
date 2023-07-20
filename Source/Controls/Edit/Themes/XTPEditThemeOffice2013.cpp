// XTPEditThemeOffice2013.cpp: implementation of the CXTPEditThemeOffice2013 class.
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPResourceImage.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2000.h"
#include "Controls/Edit/Themes/XTPEditThemeOfficeXP.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditThemeOffice2013::CXTPEditThemeOffice2013()
{
	m_nTheme = xtpControlThemeOffice2013;
}

CXTPEditThemeOffice2013::~CXTPEditThemeOffice2013()
{
}

void CXTPEditThemeOffice2013::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditTheme::RefreshMetrics(pEdit);

	// Defaults are set to OFFICE2013WORD_INI.

	m_clrBack.SetStandardValue(XTPIniColor(_T("Controls.Edit"), _T("Back"), RGB(255, 255, 255)));
	m_clrBackNonClient.SetStandardValue(
		XTPIniColor(_T("Controls.Edit"), _T("BackNonClient"), RGB(246, 246, 246)));
	m_clrText.SetStandardValue(XTPIniColor(_T("Controls.Edit"), _T("Text"), RGB(68, 68, 68)));
	m_clrTextReadOnly.SetStandardValue(
		XTPIniColor(_T("Controls.Edit"), _T("TextReadOnly"), RGB(68, 68, 68)));
	m_clrBorder.SetStandardValue(
		XTPIniColor(_T("Controls.Edit"), _T("Border"), RGB(171, 171, 171)));
	m_clrBorderHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Edit"), _T("BorderHilite"), RGB(163, 189, 227)));
	m_clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Edit"), _T("BorderDisabled"), RGB(225, 225, 225)));

	m_xtpBrushBack.DeleteObject();
	m_xtpBrushBack.CreateSolidBrush(m_clrBack);
}
