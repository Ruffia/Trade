// XTPCalendarOfficeXPTheme.cpp : implementation of the CXTPCalendarOffice2000Theme class.
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
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarPaintManager.h"

#include "Calendar/Themes/XTPCalendarOffice2000Theme.h"
#include "Calendar/Themes/XTPCalendarOfficeXPTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarOfficeXPTheme

CXTPCalendarOfficeXPTheme::CXTPCalendarOfficeXPTheme()
{
	m_CurrSystemTheme = xtpSystemThemeUnknown;
	m_nPaintTheme	 = xtpCalendarThemeOfficeXP;
}

CXTPCalendarOfficeXPTheme::~CXTPCalendarOfficeXPTheme()
{
}

void CXTPCalendarOfficeXPTheme::PartRefresh(CDayViewWorkCellPart* pViewPart)
{
	CXTPCalendarPaintManager::PartRefresh(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 0));
}

void CXTPCalendarOfficeXPTheme::PartRefresh(CDayViewNonworkCellPart* pViewPart)
{
	ASSERT(pViewPart);
	pViewPart->m_clrBackground.SetStandardValue(RGB(255, 255, 0));
	pViewPart->m_clrBackgroundSlave.SetStandardValue(RGB(128, 128, 128));
	pViewPart->m_clrShadow.SetStandardValue(RGB(128, 128, 128));
	pViewPart->m_clrHour.SetStandardValue(RGB(128, 128, 128));
}
