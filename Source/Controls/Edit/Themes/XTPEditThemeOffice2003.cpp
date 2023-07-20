// XTPEditThemeOffice2003.cpp: implementation of the CXTPEditThemeOffice2003 class.
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

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2000.h"
#include "Controls/Edit/Themes/XTPEditThemeOfficeXP.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditThemeOffice2003::CXTPEditThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
}

CXTPEditThemeOffice2003::~CXTPEditThemeOffice2003()
{
}

void CXTPEditThemeOffice2003::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditThemeOfficeXP::RefreshMetrics(pEdit);

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrBorder.SetStandardValue(RGB(127, 157, 185));
			m_clrBorderHilite.SetStandardValue(RGB(0, 0, 128));
			break;

		case xtpSystemThemeOlive:
			m_clrBorder.SetStandardValue(RGB(164, 185, 127));
			m_clrBorderHilite.SetStandardValue(RGB(63, 93, 56));
			break;

		case xtpSystemThemeSilver:
			m_clrBorder.SetStandardValue(RGB(165, 172, 178));
			m_clrBorderHilite.SetStandardValue(RGB(75, 75, 11));
			break;
	}
}
