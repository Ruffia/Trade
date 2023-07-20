// XTPPropertyGridOfficeXPTheme.cpp : implementation of the CXTPPropertyGridOfficeXPTheme class.
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
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridOfficeXPTheme

CXTPPropertyGridOfficeXPTheme::CXTPPropertyGridOfficeXPTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpPropertyGridButtonsOfficeXP;
}

void CXTPPropertyGridOfficeXPTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
}

XTPScrollBarTheme CXTPPropertyGridOfficeXPTheme::GetScrollBarTheme() const
{
	return xtpScrollBarThemeOfficeXP;
}
