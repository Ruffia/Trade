// XTPDatePickerItemDay.cpp: implementation of the CXTPDatePickerItemDay class.
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

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/DatePicker/XTPDatePickerItemDay.h"
#include "Calendar/DatePicker/XTPDatePickerControl.h"
#include "Calendar/DatePicker/XTPDatePickerPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPDatePickerItemDay

CXTPDatePickerItemDay::CXTPDatePickerItemDay(CXTPDatePickerControl* pControl,
											 CXTPDatePickerItemMonth* pMonth, COleDateTime dtDay)
	: m_pControl(pControl)
	, m_pMonth(pMonth)
	, m_dtDay(dtDay)
{
	m_rcDay.SetRectEmpty();
	m_bVisible = FALSE;
}

CXTPDatePickerItemDay::~CXTPDatePickerItemDay()
{
	// destructor
}

void CXTPDatePickerItemDay::Draw(CDC* pDC)
{
	m_bVisible = FALSE;

	if (m_rcDay.IsRectEmpty())
		return;

	// get current paint manager
	CXTPDatePickerPaintManager* pPaintManager = m_pControl->GetPaintManager();

	m_bVisible = pPaintManager->DrawDay(pDC, this);
}

BOOL CXTPDatePickerItemDay::IsToday(COleDateTime& dt) const
{
	BOOL bReturn = FALSE;
	COleDateTime dtToday;
	if (m_pControl->GetToday(dtToday))
	{
		if (dt.GetMonth() == dtToday.GetMonth() && dt.GetYear() == dtToday.GetYear()
			&& dt.GetDay() == dtToday.GetDay())
			bReturn = TRUE;
	}

	return bReturn;
}
