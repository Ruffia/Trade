// XTPComboBoxThemeVisualStudio2010.cpp: implementation of the CXTPComboBoxThemeVisualStudio20102010
// class.
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Combo/XTPComboBoxTheme.h"

#include "Controls/Combo/Themes/XTPComboBoxThemeOffice2003.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeVisualStudio.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeVisualStudio2010.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeVisualStudio2010::CXTPComboBoxThemeVisualStudio2010()
{
	m_bHighlight = TRUE;
}

CXTPComboBoxThemeVisualStudio2010::~CXTPComboBoxThemeVisualStudio2010()
{
}

void CXTPComboBoxThemeVisualStudio2010::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	m_clrBorderNormal.SetStandardValue(RGB(133, 145, 162));
	m_clrBorderHot.SetStandardValue(RGB(229, 195, 101));
	m_clrBorderDisabled.SetStandardValue(RGB(164, 173, 186));
	m_clrButtonNormal.SetStandardValue(RGB(241, 243, 248));
	m_grcButton = RGB(255, 255, 255);
	m_grcButtonHilite.SetStandardValue(RGB(255, 252, 242), RGB(255, 236, 181));
	m_grcButtonPushed = RGB(255, 232, 166);
	m_clrButtonArrowNormal.SetStandardValue(RGB(27, 41, 62));
	m_clrButtonArrowPressed.SetStandardValue(RGB(27, 41, 62));
}
