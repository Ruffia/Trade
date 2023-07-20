// XTPEditThemeVisualStudio2012.cpp: implementation of the CXTPEditThemeVisualStudio2012 class.
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
#include "Controls/Edit/Themes/XTPEditThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditThemeVisualStudio2012::CXTPEditThemeVisualStudio2012(XTPControlTheme nTheme)
{
	m_nTheme = nTheme;
}

CXTPEditThemeVisualStudio2012::~CXTPEditThemeVisualStudio2012()
{
}

void CXTPEditThemeVisualStudio2012::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditTheme::RefreshMetrics(pEdit);

	switch (m_nTheme)
	{
		case xtpControlThemeVisualStudio2012:
		{
			m_clrText.SetStandardValue(RGB(0, 0, 0));
			m_clrTextReadOnly.SetStandardValue(RGB(164, 173, 186));
			m_clrBack.SetStandardValue(RGB(255, 255, 255));
			m_clrBackNonClient.SetStandardValue(RGB(232, 232, 236));
			m_clrBorder.SetStandardValue(RGB(133, 145, 162));
			m_clrBorderHilite.SetStandardValue(RGB(229, 195, 101));
			m_clrBorderDisabled.SetStandardValue(RGB(164, 173, 186));
		}
		break;

		case xtpControlThemeVisualStudio2012Light:
		{
			m_clrText.SetStandardValue(RGB(30, 30, 30));
			m_clrTextReadOnly.SetStandardValue(RGB(204, 206, 219));
			m_clrBack.SetStandardValue(RGB(246, 246, 246));
			m_clrBackNonClient.SetStandardValue(RGB(239, 239, 242));
			m_clrBorder.SetStandardValue(RGB(204, 206, 219));
			m_clrBorderHilite.SetStandardValue(RGB(0, 122, 204));
			m_clrBorderDisabled.SetStandardValue(RGB(204, 206, 219));
		}
		break;

		case xtpControlThemeVisualStudio2012Dark:
		{
			m_clrText.SetStandardValue(RGB(241, 241, 241));
			m_clrTextReadOnly.SetStandardValue(RGB(178, 178, 178));
			m_clrBack.SetStandardValue(RGB(51, 51, 55));
			m_clrBackNonClient.SetStandardValue(RGB(62, 62, 66));
			m_clrBorder.SetStandardValue(RGB(67, 67, 70));
			m_clrBorderHilite.SetStandardValue(RGB(67, 67, 70));
			m_clrBorderDisabled.SetStandardValue(RGB(67, 67, 70));
		}
		break;
	}

	m_xtpBrushBack.DeleteObject();
	m_xtpBrushBack.CreateSolidBrush(m_clrBack);
}
