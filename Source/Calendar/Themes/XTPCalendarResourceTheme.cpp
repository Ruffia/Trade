// XTPCalendarResourceTheme.cpp : implementation of the CXTPCalendarPaintManager class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/Themes/XTPCalendarResourceTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarResourceTheme

CXTPCalendarResourceTheme::CXTPCalendarResourceTheme()
{
	m_CurrSystemTheme = xtpSystemThemeUnknown;
	m_nPaintTheme	 = xtpCalendarThemeResource;
}

CXTPCalendarResourceTheme::~CXTPCalendarResourceTheme()
{
}

COLORREF CXTPCalendarResourceTheme::GetColor2(LPCTSTR pcszColorName, COLORREF clrDefault)
{
	COLORREF clrColor = COLORREF_NULL;

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages && pImages->IsValid())
	{
		clrColor = pImages->GetImageColor(_T("CalendarControl"), pcszColorName);
	}

	if (clrColor == COLORREF_NULL)
	{
		clrColor = clrDefault;
	}

	return clrColor;
}

void CXTPCalendarResourceTheme::RefreshMetrics()
{
	CXTPCalendarPaintManager::RefreshMetrics();

	m_clrBorders[0].SetStandardValue(GetColor2(_T("CaptionBarBorders"), RGB(101, 147, 201)));
	m_clrBorders[1].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[2].SetStandardValue(m_clrBorders[0]);
	m_clrBorders[3].SetStandardValue(m_clrBorders[0]);
}
