// XTPCalendarRecurrencePattern.cpp: implementation of the CXTPCalendarRecurrencePattern class.
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
#include "Common/XTPXMLHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPSystemHelpers.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarEvents.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/XTPCalendarRecurrencePattern.h"
#include "Calendar/XTPCalendarCustomProperties.h"
#include "Calendar/XTPCalendarIIDs.h"

static LPCTSTR cszRPatt_PatternID = _T("PatternID");
static LPCTSTR cszRPatt_StartTime = _T("StartTime");

static LPCTSTR cszRPatt_DurationMinutes		= _T("DurationMinutes");
static LPCTSTR cszRPatt_RecurrenceType		= _T("RecurrenceType");
static LPCTSTR cszRPatt_RecurrenceOpt_Data1 = _T("RecurrenceOpt_Data1");
static LPCTSTR cszRPatt_RecurrenceOpt_Data2 = _T("RecurrenceOpt_Data2");
static LPCTSTR cszRPatt_RecurrenceOpt_Data3 = _T("RecurrenceOpt_Data3");

static LPCTSTR cszRPatt_PatternStartDate	= _T("PatternStartDate");
static LPCTSTR cszRPatt_UseEndMethod		= _T("UseEndMethod");
static LPCTSTR cszRPatt_PatternEndDate		= _T("PatternEndDate");
static LPCTSTR cszRPatt_EndAfterOccurrences = _T("EndAfterOccurrences");

static LPCTSTR cszRPatt_MasterEventID = _T("MasterEventID");

//===========================================================================
static const LPCTSTR cszOccRMD_Count = _T("*xtp*OccRMD_Count");

static const LPCTSTR cszOccRMD_StartTime = _T("*xtp*OccRMD_Start");
static const LPCTSTR cszOccRMD_EndTime   = _T("*xtp*OccRMD_End");

static const LPCTSTR cszOccRMD_IsReminder				= _T("*xtp*OccRMD_IsReminder");
static const LPCTSTR cszOccRMD_NextReminderTime_Snoozed = _T("*xtp*OccRMD_NextReminderTime_")
														  _T("Snoozed");
//===========================================================================

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////
#define XTP_RECPATT_PROP_SET_DATA_VER 1
////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXTPCalendarRecurrencePattern, CXTPCmdTarget)

/////////////////////////////////////////////////////////////////////////////
CXTPCalendarRecurrencePattern::CXTPCalendarRecurrencePattern(CXTPCalendarData* pDataProvider)
{
	ASSERT(pDataProvider);

	m_ptrDataProvider = pDataProvider;
	CMDTARGET_ADDREF(m_ptrDataProvider);

	m_dwPatternID	 = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;
	m_dwMasterEventID = XTP_CALENDAR_UNKNOWN_EVENT_ID;

	m_dtStartTime	  = (DATE)0; //.SetStatus(COleDateTime::null);
	m_nDurationMinutes = 0;

	m_Options.m_nRecurrenceType = xtpCalendarRecurrenceUnknown;

	m_dtPatternStartDate = (DATE)0; //.SetStatus(COleDateTime::null);

	m_pCustomProperties = new CXTPCalendarCustomProperties;

	m_parExceptionEvents = new CXTPCalendarEvents();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarRecurrencePattern::~CXTPCalendarRecurrencePattern()
{
	CMDTARGET_RELEASE(m_ptrDataProvider);
	CMDTARGET_RELEASE(m_pCustomProperties);
	CMDTARGET_RELEASE(m_parExceptionEvents);
}

COleDateTime
	CXTPCalendarRecurrencePattern::GetStartOccurrenceTime(COleDateTime dtOccurrenceStartDate) const
{
	COleDateTime dtStartOcc = GetStartTime();
	dtStartOcc				= CXTPCalendarUtils::UpdateDate(dtStartOcc, dtOccurrenceStartDate);

	return dtStartOcc;
}

COleDateTime
	CXTPCalendarRecurrencePattern::GetEndOccurrenceTime(COleDateTime dtOccurrenceStartDate) const
{
	COleDateTimeSpan spDuration = GetDuration();
	COleDateTime dtEndOcc		= GetStartOccurrenceTime(dtOccurrenceStartDate);

	dtEndOcc += spDuration;

	return dtEndOcc;
}

void CXTPCalendarRecurrencePattern::_GetOccurrenceStartEnd(const COleDateTime& dtOccStartDate,
														   COleDateTime& rdtOccStart,
														   COleDateTime& rdtOccEnd) const
{
	rdtOccStart = GetStartTime();
	rdtOccStart = CXTPCalendarUtils::UpdateDate(rdtOccStart, dtOccStartDate);

	COleDateTimeSpan spDuration = GetDuration();
	rdtOccEnd					= rdtOccStart + spDuration;
}

void CXTPCalendarRecurrencePattern::SetPatternStartDate(COleDateTime dtStart)
{
	m_dtPatternStartDate = CXTPCalendarUtils::ResetTime(dtStart);

	if (m_dtPatternStartDate.GetStatus() == COleDateTime::valid
		&& m_PatternEnd.m_dtPatternEndDate.GetStatus() == COleDateTime::valid
		&& m_dtPatternStartDate > m_PatternEnd.m_dtPatternEndDate)
	{
		m_PatternEnd.m_dtPatternEndDate = m_dtPatternStartDate;
	}
}

void CXTPCalendarRecurrencePattern::SetPatternEndDate(COleDateTime dtEnd)
{
	m_PatternEnd.m_dtPatternEndDate = CXTPCalendarUtils::ResetTime(dtEnd);
	m_PatternEnd.m_nUseEnd			= xtpCalendarPatternEndDate;

	if (m_dtPatternStartDate.GetStatus() == COleDateTime::valid
		&& m_PatternEnd.m_dtPatternEndDate.GetStatus() == COleDateTime::valid
		&& m_PatternEnd.m_dtPatternEndDate < m_dtPatternStartDate)
	{
		m_dtPatternStartDate = m_PatternEnd.m_dtPatternEndDate;
	}
}

BOOL CXTPCalendarRecurrencePattern::GetOccurrences(CXTPCalendarEvents* pEventsArray,
												   COleDateTime dtStart, COleDateTime dtEnd,
												   CXTPCalendarEvent* pMasterEvent)
{
	if (!pMasterEvent || !pEventsArray)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	COleDateTime dtStartOcc, dtEndOcc;
	_GetOccurrenceStartEnd(dtStart, dtStartOcc, dtEndOcc);

	int nOccDays = CXTPCalendarUtils::GetEventPeriodDays(dtStartOcc, dtEndOcc);
	ASSERT(nOccDays > 0);
	nOccDays = max(1, nOccDays);

	COleDateTime dtStartCheckOcc = dtStart;

	if (dtStart == dtEnd)
	{
		//- check for previous multiday occurrences ---
		if (nOccDays > 1)
		{
			COleDateTimeSpan spCheckMDOcc(nOccDays - 1, 0, 0, 0);
			dtStartCheckOcc = dtStart - spCheckMDOcc;
		}
	}
	CXTPCalendarDatesArray arDates;
	GetOccurrencesDates(arDates, dtStartCheckOcc, dtEnd);

	COleDateTime dtI;
	int nCount = (int)arDates.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		dtI = arDates[i];

		_GetOccurrenceStartEnd(dtI, dtStartOcc, dtEndOcc);

		ASSERT(
			!(CXTPCalendarUtils::ResetTime(dtEndOcc) < CXTPCalendarUtils::ResetTime(dtStart)
			  || CXTPCalendarUtils::ResetTime(dtStartOcc) > CXTPCalendarUtils::ResetTime(dtEnd)));

		CXTPCalendarEventPtr ptrOccurrenceEvent(FindException(dtStartOcc, dtEndOcc), TRUE);

		if (ptrOccurrenceEvent)
		{
			continue;
		}
		else
		{
			ptrOccurrenceEvent = pMasterEvent->CloneForOccurrence(dtStartOcc, dtEndOcc);

			if (!!ptrOccurrenceEvent
				&& ptrOccurrenceEvent->GetStartTime() < pMasterEvent->GetLastModificationTime())
			{
				ptrOccurrenceEvent->SetReminder(FALSE);
			}
		}

		if (ptrOccurrenceEvent)
		{
			m_arOccReminders.UpdateOccIfNeed(ptrOccurrenceEvent, this);

			pEventsArray->Add(ptrOccurrenceEvent);
		}
	}

	FindExceptions(pEventsArray, dtStart, dtEnd);

	return TRUE;
}

void CXTPCalendarRecurrencePattern::GetOccurrencesDates(CXTPCalendarDatesArray& rArDates,
														COleDateTime dtStart,
														COleDateTime dtEnd) const
{
	COleDateTime dtStart0 = CXTPCalendarUtils::ResetTime(dtStart);
	COleDateTime dtEnd0   = CXTPCalendarUtils::SetTime_235959(dtEnd);

	int nMaxOccCount = INT_MAX;
	int nOccCount	= 0;
	COleDateTime dtI;
	COleDateTime dtE1(9999, 1, 1, 0, 0, 0);
	COleDateTime dtS1 = max(dtStart0, m_dtPatternStartDate);

	CString s;

	switch (m_PatternEnd.m_nUseEnd)
	{
		case xtpCalendarPatternEndDate: dtE1 = min(m_PatternEnd.m_dtPatternEndDate, dtEnd0); break;
		case xtpCalendarPatternEndAfterOccurrences:
			nMaxOccCount = m_PatternEnd.m_nEndAfterOccurrences;
			dtE1		 = dtEnd0;
			break;
		case xtpCalendarPatternEndNoDate: dtE1 = dtEnd0; break;
		default: ASSERT(FALSE); return;
	}

	switch (m_Options.m_nRecurrenceType)
	{
		case xtpCalendarRecurrenceDaily:
		{
			//------------------------------------------------------
			// 1. Every N days:
			//      m_nInterval = N,
			//      m_nDayOfWeekMask = xtpCalendarDayAllWeek
			//------------------------------------------------------
			// 2. Every week day (Mo-Fr):
			//      m_nInterval = 1,
			//      m_nDayOfWeekMask = xtpCalendarDayMo_Fr
			//------------------------------------------------------

			int nDays = m_Options.m_Daily.nIntervalDays;

			if (m_Options.m_Daily.bEveryWeekDayOnly)
			{
				ASSERT(nDays == 1);
				nDays = 1;
			}

			COleDateTimeSpan spStep(nDays, 0, 0, 0);

			if (m_dtPatternStartDate < dtStart0)
			{
				//--- shift start date --------------
				COleDateTimeSpan spDiff = dtStart0 - m_dtPatternStartDate;
				int nDiff_Days			= (int)CXTPCalendarUtils::GetTotalDays(spDiff);
				ASSERT(nDiff_Days >= 1);

				int nJump_Days = (nDiff_Days / nDays) * nDays;
				spDiff.SetDateTimeSpan(nJump_Days, 0, 0, 0);

				dtS1 = m_dtPatternStartDate + spDiff;
				if (dtS1 < dtStart0)
				{
					dtS1 += spStep;
				}

				//--- calculate occurrences ----------
				if (nMaxOccCount != INT_MAX)
				{
					nOccCount = nDiff_Days / nDays;

					if (m_Options.m_Daily.bEveryWeekDayOnly)
					{
						int nWDstart = m_dtPatternStartDate.GetDayOfWeek();

						int nBasis1_days = (nDiff_Days - (7 - nWDstart));
						int nSatDays	 = nBasis1_days / 7 + ((nBasis1_days % 7) ? 1 : 0);

						int nBasis6_days = (nDiff_Days - (6 - nWDstart));
						int nSunDays	 = nBasis6_days / 7 + ((nBasis6_days % 7) ? 1 : 0);

						nOccCount -= nSatDays + nSunDays;
					}
				}
			}
			else
			{
				dtS1 = m_dtPatternStartDate;
			}

			for (dtI = dtS1; dtI <= dtE1 && nOccCount < nMaxOccCount; dtI += spStep)
			{
				if (m_Options.m_Daily.bEveryWeekDayOnly)
				{
					int nWD = dtI.GetDayOfWeek(); // 1-Sunday, 2 - Monday, ...
					if (nWD == 1 || nWD == 7)
					{
						continue;
					}
				}
				ASSERT(dtI >= dtStart0 && dtI <= dtE1);
				rArDates.Add(dtI);
				nOccCount++;
			}

			break;
		}

		case xtpCalendarRecurrenceWeekly:
		{
			//------------------------------------------------------
			// 1. Every N weeks:
			//      m_nInterval = N,
			//      m_nDayOfWeekMask = <*MASK*>
			//------------------------------------------------------

			int nWeeksStep = m_Options.m_Weekly.nIntervalWeeks;
			if (nWeeksStep <= 0)
			{
				ASSERT(FALSE);
				nWeeksStep = 1;
			}
			COleDateTimeSpan spStep(1, 0, 0, 0);
			COleDateTimeSpan spStepWeeks((nWeeksStep)*7, 0, 0, 0);

			int nFirstDayOfWeekIndex = 2; // Always Monday (as in MS Outlook)

			COleDateTimeSpan spWeek(7, 0, 0, 0);
			COleDateTime dtPatSW = CXTPCalendarUtils::ShiftDateToWeekBegin(m_dtPatternStartDate,
																		   nFirstDayOfWeekIndex);
			dtPatSW				 = CXTPCalendarUtils::ResetTime(dtPatSW);

			COleDateTime dtOccFrame1 = dtPatSW;
			COleDateTime dtOccFrame2 = dtOccFrame1 + spWeek;

			if (m_dtPatternStartDate < dtStart0)
			{
				int nPatDW = m_dtPatternStartDate.GetDayOfWeek();
				int nMask  = CXTPCalendarUtils::MakeDaysOfWeekMask_Mo_(nPatDW);

				COleDateTime dtStart0SW =
					CXTPCalendarUtils::ShiftDateToWeekBegin(dtStart0, nFirstDayOfWeekIndex);

				COleDateTimeSpan spDiff = dtStart0SW - dtPatSW;

				int nDiff_Weeks = (int)spDiff.GetTotalDays() / 7;
				ASSERT((int)spDiff.GetTotalDays() % 7 == 0);

				int nJump_Weeks = (nDiff_Weeks / nWeeksStep) * nWeeksStep;
				spDiff.SetDateTimeSpan(nJump_Weeks * 7, 0, 0, 0);

				dtOccFrame1 = dtPatSW + spDiff;
				dtOccFrame2 = dtOccFrame1 + spWeek;

				int nDW = CXTPCalendarUtils::GetDayOfWeekCount(m_Options.m_Weekly.nDayOfWeekMask);
				if (nMask)
				{
					nOccCount = nDW;
					nOccCount *= nJump_Weeks / nWeeksStep;
				}

				int nSkippedMask = CXTPCalendarUtils::MakeDaysOfWeekMask_Mo_(
					m_dtPatternStartDate.GetDayOfWeek());
				nOccCount -= CXTPCalendarUtils::GetDayOfWeekCount(
					nSkippedMask & m_Options.m_Weekly.nDayOfWeekMask);

				int nAddedMask = CXTPCalendarUtils::MakeDaysOfWeekMask_Mo_(dtS1.GetDayOfWeek());
				nOccCount += CXTPCalendarUtils::GetDayOfWeekCount(
					nAddedMask & m_Options.m_Weekly.nDayOfWeekMask);
			}

			for (dtI = dtS1; dtI <= dtE1 && nOccCount < nMaxOccCount; dtI += spStep)
			{
				if (nWeeksStep > 1 && !(dtOccFrame1 <= dtI && dtI < dtOccFrame2))
				{
					dtOccFrame1 += spStepWeeks;
					dtOccFrame2 = dtOccFrame1 + spWeek;

					dtI = dtOccFrame1 - spStep;
					continue;
				}

				int nWD		= dtI.GetDayOfWeek(); // 1-Sunday, 2 - Monday, ...
				int nWDMask = CXTPCalendarUtils::GetDayOfWeekMask(nWD);

				if (m_Options.m_Weekly.nDayOfWeekMask & nWDMask)
				{
					ASSERT(dtI >= dtStart0 && dtI <= dtE1);

					rArDates.Add(dtI);
					nOccCount++;
				}
			}

			break;
		}

		case xtpCalendarRecurrenceMonthly:
		{
			//------------------------------------------------------
			// 1. Day N of every M month(s):
			//      m_nDayOfMonth = N,
			//      m_nInterval = M
			//------------------------------------------------------

			int nMInterval = m_Options.m_Monthly.nIntervalMonths;
			if (nMInterval <= 0)
			{
				ASSERT(FALSE);
				nMInterval = 1;
			}

			COleDateTime dtOccFrame1 = CXTPCalendarUtils::ResetTime(m_dtPatternStartDate);
			CXTPCalendarUtils::UpdateMonthDay(dtOccFrame1, 1);

			if (m_dtPatternStartDate < dtStart0)
			{
				int nPS_Year		= m_dtPatternStartDate.GetYear();
				int nPS_Month		= m_dtPatternStartDate.GetMonth();
				int nPS_GlobalMonth = nPS_Year * 12 + nPS_Month;

				int nS0_Year		= dtStart0.GetYear();
				int nS0_Month		= dtStart0.GetMonth();
				int nS0_GlobalMonth = nS0_Year * 12 + nS0_Month;

				int nMonthDiff  = nS0_GlobalMonth - nPS_GlobalMonth;
				int nJump_Month = (nMonthDiff / nMInterval) * nMInterval;

				CXTPCalendarUtils::ShiftDate_Month(dtOccFrame1, nJump_Month, 1);

				nOccCount = nJump_Month / nMInterval;

				if (m_dtPatternStartDate.GetDay() > m_Options.m_Monthly.nDayOfMonth)
					nOccCount--;

				if (dtS1.GetDay() > m_Options.m_Monthly.nDayOfMonth)
					nOccCount++;
			}

			COleDateTime dtOcc(dtOccFrame1);
			VERIFY(CXTPCalendarUtils::UpdateMonthDay(dtOcc, m_Options.m_Monthly.nDayOfMonth));
			if (dtOcc >= dtS1)
			{
				dtS1 = dtOcc;
			}
			else
			{
				dtS1 = dtOcc;
				CXTPCalendarUtils::ShiftDate_Month(dtS1, m_Options.m_Monthly.nIntervalMonths,
												   m_Options.m_Monthly.nDayOfMonth);
			}

			for (dtI = dtS1; dtI <= dtE1 && nOccCount < nMaxOccCount;)
			{
				ASSERT(dtI >= dtStart0 && dtI <= dtE1);
				rArDates.Add(dtI);
				nOccCount++;

				CXTPCalendarUtils::ShiftDate_Month(dtI, m_Options.m_Monthly.nIntervalMonths,
												   m_Options.m_Monthly.nDayOfMonth);
			}

			break;
		}

		case xtpCalendarRecurrenceMonthNth:
		{
			//------------------------------------------------------
			//~ The I:{First, second, ..., Last>
			//~         W:{Day, WeekDay, WeekEndDay, Su, Mo, ...}
			//~             of every M month(s):
			//------------------------------------------------------

			int nMInterval = m_Options.m_MonthNth.nIntervalMonths;
			if (nMInterval <= 0)
			{
				ASSERT(FALSE);
				nMInterval = 1;
			}

			COleDateTime dtOccFrame1 = CXTPCalendarUtils::ResetTime(m_dtPatternStartDate);
			CXTPCalendarUtils::UpdateMonthDay(dtOccFrame1, 1);

			if (m_dtPatternStartDate < dtStart0)
			{
				int nPS_Year		= m_dtPatternStartDate.GetYear();
				int nPS_Month		= m_dtPatternStartDate.GetMonth();
				int nPS_GlobalMonth = nPS_Year * 12 + nPS_Month;

				int nS0_Year		= dtStart0.GetYear();
				int nS0_Month		= dtStart0.GetMonth();
				int nS0_GlobalMonth = nS0_Year * 12 + nS0_Month;

				int nMonthDiff  = nS0_GlobalMonth - nPS_GlobalMonth;
				int nJump_Month = (nMonthDiff / nMInterval) * nMInterval;

				CXTPCalendarUtils::ShiftDate_Month(dtOccFrame1, nJump_Month, 1);

				nOccCount = nJump_Month / nMInterval;

				COleDateTime dtTestMD(m_dtPatternStartDate);
				CXTPCalendarUtils::CalcDayOfMonth(dtTestMD, m_Options.m_MonthNth.nWhichDay,
												  m_Options.m_MonthNth.nWhichDayMask);

				if (m_dtPatternStartDate.GetDay() > dtTestMD.GetDay())
				{
					nOccCount--;
				}
				dtTestMD = dtS1;
				CXTPCalendarUtils::CalcDayOfMonth(dtTestMD, m_Options.m_MonthNth.nWhichDay,
												  m_Options.m_MonthNth.nWhichDayMask);

				if (dtS1.GetDay() > dtTestMD.GetDay())
				{
					nOccCount++;
				}
			}
			COleDateTime dtOcc(dtOccFrame1);

			BOOL bRes = CXTPCalendarUtils::CalcDayOfMonth(dtOcc, m_Options.m_MonthNth.nWhichDay,
														  m_Options.m_MonthNth.nWhichDayMask);
			if (!bRes)
			{
				return;
			}

			if (dtOcc >= dtS1)
			{
				dtS1 = dtOcc;
			}
			else
			{
				dtS1 = dtOcc;
				CXTPCalendarUtils::ShiftDate_Month(dtS1, nMInterval);
				bRes = CXTPCalendarUtils::CalcDayOfMonth(dtS1, m_Options.m_MonthNth.nWhichDay,
														 m_Options.m_MonthNth.nWhichDayMask);
				if (!bRes)
				{
					return;
				}
			}

			for (dtI = dtS1; dtI <= dtE1 && nOccCount < nMaxOccCount;)
			{
				ASSERT(dtI >= dtStart0 && dtI <= dtE1);
				rArDates.Add(dtI);
				nOccCount++;

				CXTPCalendarUtils::ShiftDate_Month(dtI, nMInterval);
				bRes = CXTPCalendarUtils::CalcDayOfMonth(dtI, m_Options.m_MonthNth.nWhichDay,
														 m_Options.m_MonthNth.nWhichDayMask);

				if (!bRes)
				{
					ASSERT(FALSE);
					break;
				}
			}

			break;
		}

		case xtpCalendarRecurrenceYearly:
		{
			//------------------------------------------------------
			// Every M:{month} D:{Day}
			//      m_nMonthOfYear = M
			//      m_nDayOfMonth = D
			//------------------------------------------------------

			COleDateTime dtOcc(dtS1.GetYear(), m_Options.m_Yearly.nMonthOfYear, 1, 0, 0, 0);
			CXTPCalendarUtils::UpdateMonthDay(dtOcc, m_Options.m_Yearly.nDayOfMonth);

			if (dtOcc >= dtS1)
			{
				dtS1 = dtOcc;
			}
			else
			{
				dtS1 = dtOcc;
				CXTPCalendarUtils::ShiftDate_Year(dtS1, 1, m_Options.m_Yearly.nDayOfMonth);
			}

			for (dtI = dtS1; dtI <= dtE1 && nOccCount < nMaxOccCount;
				 CXTPCalendarUtils::ShiftDate_Year(dtI, 1, m_Options.m_Yearly.nDayOfMonth))
			{
				ASSERT(dtI >= dtStart0 && dtI <= dtE1);
				rArDates.Add(dtI);
				nOccCount++;
			}

			break;
		}

		case xtpCalendarRecurrenceYearNth:
		{
			//------------------------------------------------------
			//~ The I:{First, second, ..., Last>
			//~         W:{Day, WeekDay, WeekEndDay, Su, Mo, ...}
			//~             of M:{month}
			//------------------------------------------------------
			COleDateTime dtOcc(dtS1.GetYear(), m_Options.m_YearNth.nMonthOfYear, 1, 0, 0, 0);

			BOOL bRes = CXTPCalendarUtils::CalcDayOfMonth(dtOcc, m_Options.m_YearNth.nWhichDay,
														  m_Options.m_YearNth.nWhichDayMask);
			if (!bRes)
			{
				return;
			}

			if (dtOcc >= dtS1)
			{
				dtS1 = dtOcc;
			}
			else
			{
				dtS1 = dtOcc;
				CXTPCalendarUtils::ShiftDate_Year(dtS1, 1);
				bRes = CXTPCalendarUtils::CalcDayOfMonth(dtS1, m_Options.m_YearNth.nWhichDay,
														 m_Options.m_YearNth.nWhichDayMask);
				if (!bRes)
				{
					return;
				}
			}

			for (dtI = dtS1; dtI <= dtE1 && nOccCount < nMaxOccCount;)
			{
				ASSERT(dtI >= dtStart0 && dtI <= dtE1);
				rArDates.Add(dtI);
				nOccCount++;

				CXTPCalendarUtils::ShiftDate_Year(dtI, 1);
				bRes = CXTPCalendarUtils::CalcDayOfMonth(dtI, m_Options.m_YearNth.nWhichDay,
														 m_Options.m_YearNth.nWhichDayMask);
				if (!bRes)
				{
					ASSERT(FALSE);
					break;
				}
			}

			break;
		}

		case xtpCalendarRecurrenceUnknown:
		default: ASSERT(!"Unexpected"); break;
	}
}

void CXTPCalendarRecurrencePattern::SetRecurrenceOptions(
	const XTP_CALENDAR_RECURRENCE_OPTIONS& Options)
{
	m_Options = Options;
	if (m_Options.m_nRecurrenceType == xtpCalendarRecurrenceDaily
		&& m_Options.m_Daily.bEveryWeekDayOnly)
	{
		m_Options.m_Daily.nIntervalDays = 1;
	}
	// Prevent wrong user entry (e.g. infinite loop for nIntervalDays = 0 case)
	if (m_Options.m_nRecurrenceType == xtpCalendarRecurrenceDaily)
	{
		m_Options.m_Daily.nIntervalDays = min(max(m_Options.m_Daily.nIntervalDays, 1), 999);
	}
	if (m_Options.m_nRecurrenceType == xtpCalendarRecurrenceWeekly)
	{
		m_Options.m_Weekly.nIntervalWeeks = min(max(m_Options.m_Weekly.nIntervalWeeks, 1), 999);
	}
	if (m_Options.m_nRecurrenceType == xtpCalendarRecurrenceMonthly)
	{
		m_Options.m_Monthly.nIntervalMonths = min(max(m_Options.m_Monthly.nIntervalMonths, 1), 999);
		m_Options.m_Monthly.nDayOfMonth		= min(max(m_Options.m_Monthly.nDayOfMonth, 1), 31);
	}
	if (m_Options.m_nRecurrenceType == xtpCalendarRecurrenceYearly)
	{
		m_Options.m_Yearly.nDayOfMonth = min(max(m_Options.m_Yearly.nDayOfMonth, 1), 31);
	}
}

void CXTPCalendarRecurrencePattern::SetPatternEnd(const XTP_CALENDAR_PATTERN_END& patternEnd)
{
	m_PatternEnd = patternEnd;

	if (m_PatternEnd.m_nUseEnd == xtpCalendarPatternEndDate)
	{
		m_PatternEnd.m_dtPatternEndDate = CXTPCalendarUtils::ResetTime(
			m_PatternEnd.m_dtPatternEndDate);

		if (m_dtPatternStartDate.GetStatus() == COleDateTime::valid
			&& m_PatternEnd.m_dtPatternEndDate.GetStatus() == COleDateTime::valid
			&& m_PatternEnd.m_dtPatternEndDate < m_dtPatternStartDate)
		{
			m_dtPatternStartDate = m_PatternEnd.m_dtPatternEndDate;
		}
	}
}

CXTPCalendarRecurrencePatternPtr CXTPCalendarRecurrencePattern::ClonePattern()
{
	if (!m_ptrDataProvider)
	{
		ASSERT(FALSE);
		return NULL;
	}

	DWORD dwPatternID = GetPatternID();
	DWORD dwMasterID  = GetMasterEventID();

	CXTPCalendarRecurrencePatternPtr ptrNew = m_ptrDataProvider->CreateNewRecurrencePattern();
	if (!ptrNew)
	{
		ASSERT(FALSE);
		return NULL;
	}
	ptrNew->SetPatternID(dwPatternID);
	ptrNew->SetMasterEventID(dwMasterID);

	if (!ptrNew->Update(this))
	{
		ASSERT(FALSE);
		return NULL;
	}

	return ptrNew;
}

BOOL CXTPCalendarRecurrencePattern::Update(CXTPCalendarRecurrencePattern* pNewData)
{
	if (pNewData == this)
	{
		return TRUE;
	}

	COleDateTime dtTmp = pNewData->GetStartTime();
	SetStartTime(dtTmp);

	int nTmp = pNewData->GetDurationMinutes();
	SetDurationMinutes(nTmp);

	const XTP_CALENDAR_RECURRENCE_OPTIONS& rOpt = pNewData->GetRecurrenceOptions();
	SetRecurrenceOptions(rOpt);

	dtTmp = pNewData->GetPatternStartDate();
	SetPatternStartDate(dtTmp);

	const XTP_CALENDAR_PATTERN_END& rPatternEnd = pNewData->GetPatternEnd();
	SetPatternEnd(rPatternEnd);

	RemoveAllExceptions();

	CXTPCalendarEventsPtr ptrExceptions = pNewData->GetExceptions();

	int nCount = XTP_SAFE_GET1(ptrExceptions, GetCount(), 0);
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pException = ptrExceptions->GetAt(i, FALSE);
		if (pException)
		{
			if (!SetException(pException))
			{
				ASSERT(FALSE);
			}
		}
	}

	ASSERT(pNewData->GetCustomProperties());
	pNewData->GetCustomProperties()->CopyTo(m_pCustomProperties);

	m_arOccReminders.RemoveAll();
	m_arOccReminders.Append(pNewData->m_arOccReminders);

	return TRUE;
}

BOOL CXTPCalendarRecurrencePattern::SetException(CXTPCalendarEvent* pException,
												 CXTPCalendarEvent* pMasterEventToUpdate)
{
	if (!pException)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DWORD dwExcID			= pException->GetEventID();
	CXTPCalendarEvent* pExc = m_parExceptionEvents->FindEvent(dwExcID);

	if (!pExc)
	{
		COleDateTime dtStartOcc = pException->GetRException_StartTimeOrig();
		COleDateTime dtEndOcc   = pException->GetRException_EndTimeOrig();

		pExc = FindException(dtStartOcc, dtEndOcc);
	}

	BOOL bRes = TRUE;
	if (pExc)
	{
		pExc->SetEventID(pException->GetEventID());
		bRes = pExc->Update(pException);
		ASSERT(bRes);
	}
	else
	{
		CXTPCalendarEventPtr ptrExc = pException->CloneEvent();
		m_parExceptionEvents->Add(ptrExc);
		pExc = ptrExc;
	}
	pExc->SetRecurrencePatternID(m_dwPatternID);

	if (pMasterEventToUpdate)
	{
		pMasterEventToUpdate->UpdateStartEndDatesFromPatternExceptions();
	}

	return bRes;
}

BOOL CXTPCalendarRecurrencePattern::RemoveException(CXTPCalendarEvent* pException,
													CXTPCalendarEvent* pMasterEventToUpdate)
{
	if (!pException)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nRState = pException->GetRecurrenceState();
	ASSERT(nRState == xtpCalendarRecurrenceOccurrence || nRState == xtpCalendarRecurrenceException);

	COleDateTime dtStartOcc, dtEndOcc;

	if (nRState == xtpCalendarRecurrenceOccurrence)
	{
		dtStartOcc = pException->GetStartTime();
		dtEndOcc   = pException->GetEndTime();
	}
	else
	{
		if (nRState == xtpCalendarRecurrenceException)
		{
			dtStartOcc = pException->GetRException_StartTimeOrig();
			dtEndOcc   = pException->GetRException_EndTimeOrig();
		}
		else
		{
			return FALSE;
		}
	}

	int nCount   = m_parExceptionEvents->GetCount();
	int nFExcIdx = FindExceptionIndex(dtStartOcc, dtEndOcc);

	if (nFExcIdx >= 0 && nFExcIdx < nCount)
	{
		m_parExceptionEvents->RemoveAt(nFExcIdx);

		if (pMasterEventToUpdate)
		{
			pMasterEventToUpdate->UpdateStartEndDatesFromPatternExceptions();
		}

		return TRUE;
	}

	return FALSE;
}

void CXTPCalendarRecurrencePattern::RemoveAllExceptions()
{
	m_parExceptionEvents->RemoveAll();

	m_arOccReminders.RemoveAll();
}

CXTPCalendarEvent* CXTPCalendarRecurrencePattern::FindException(COleDateTime dtStartOrig,
																COleDateTime dtEndOrig) const
{
	int nCount   = m_parExceptionEvents->GetCount();
	int nFExcIdx = FindExceptionIndex(dtStartOrig, dtEndOrig);

	if (nFExcIdx >= 0 && nFExcIdx < nCount)
	{
		CXTPCalendarEvent* pEvent = m_parExceptionEvents->GetAt(nFExcIdx, FALSE);
		return pEvent;
	}
	return NULL;
}

int CXTPCalendarRecurrencePattern::FindExceptionIndex(COleDateTime dtStartOrig,
													  COleDateTime dtEndOrig) const
{
	COleDateTime dtTmpS, dtTmpE;

	int nCount = m_parExceptionEvents->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = m_parExceptionEvents->GetAt(i, FALSE);

		ASSERT(pEvent->GetRecurrenceState() == xtpCalendarRecurrenceException);

		dtTmpS = pEvent->GetRException_StartTimeOrig();
		dtTmpE = pEvent->GetRException_EndTimeOrig();

		if (CXTPCalendarUtils::IsEqual(dtTmpS, dtStartOrig)
			&& CXTPCalendarUtils::IsEqual(dtTmpE, dtEndOrig))
		{
			return i;
		}
	}
	return -1;
}

BOOL CXTPCalendarRecurrencePattern::FindExceptions(CXTPCalendarEvents* pEvents,
												   COleDateTime dtStart, COleDateTime dtEnd) const
{
	dtStart = CXTPCalendarUtils::ResetTime(dtStart);
	dtEnd   = CXTPCalendarUtils::ResetTime(dtEnd);

	COleDateTime dtTmpS, dtTmpE;

	int nCount = m_parExceptionEvents->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = m_parExceptionEvents->GetAt(i, FALSE);

		ASSERT(pEvent->GetRecurrenceState() == xtpCalendarRecurrenceException);

		dtTmpS = CXTPCalendarUtils::ResetTime(pEvent->GetStartTime());
		dtTmpE = CXTPCalendarUtils::ResetTime(pEvent->GetEndTime());

		if (dtStart <= dtTmpS && dtTmpS <= dtEnd || dtStart <= dtTmpE && dtTmpE <= dtEnd
			|| dtTmpS <= dtStart && dtStart <= dtTmpE || dtTmpS <= dtEnd && dtEnd <= dtTmpE)
		{
			if (!pEvent->IsRExceptionDeleted())
			{
				pEvents->Add(pEvent);
			}
		}
	}
	return TRUE;
}

CXTPCalendarEventsPtr CXTPCalendarRecurrencePattern::GetExceptions()
{
	CXTPCalendarEventsPtr ptrExceptions = new CXTPCalendarEvents();
	if (!ptrExceptions)
	{
		return NULL;
	}

	int nCount = m_parExceptionEvents->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = m_parExceptionEvents->GetAt(i, FALSE);
		ASSERT(pEvent);

		CXTPCalendarEventPtr ptrEventCopy;
		if (pEvent)
			ptrEventCopy = pEvent->CloneEvent();

		if (ptrEventCopy)
			ptrExceptions->Add(ptrEventCopy);
	}
	return ptrExceptions;
}

CXTPCalendarEventPtr CXTPCalendarRecurrencePattern::GetMasterEvent() const
{
	if (!m_ptrDataProvider)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarEventPtr ptrMEvent = m_ptrDataProvider->GetEvent(m_dwMasterEventID);
	ASSERT(!!ptrMEvent);
	return ptrMEvent;
}

void CXTPCalendarRecurrencePattern::SetPatternID(DWORD dwID)
{
	m_dwPatternID = dwID;

	int nCount = m_parExceptionEvents->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = m_parExceptionEvents->GetAt(i, FALSE);
		ASSERT(pEvent);
		if (pEvent)
		{
			pEvent->SetRecurrencePatternID(m_dwPatternID);
		}
	}
}

void CXTPCalendarRecurrencePattern::DoPropExchange(CXTPPropExchange* pPX)
{
	long nVersion = XTP_RECPATT_PROP_SET_DATA_VER;
	PX_Long(pPX, _T("Version"), nVersion, XTP_RECPATT_PROP_SET_DATA_VER);

	PX_DWord(pPX, cszRPatt_PatternID, m_dwPatternID, XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);
	PX_DateTime(pPX, cszRPatt_StartTime, m_dtStartTime);
	PX_Int(pPX, cszRPatt_DurationMinutes, m_nDurationMinutes, 0);

	PX_Enum(pPX, cszRPatt_RecurrenceType, m_Options.m_nRecurrenceType,
			xtpCalendarRecurrenceUnknown);

	PX_Int(pPX, cszRPatt_RecurrenceOpt_Data1, m_Options.m_YearNth.nWhichDay, 0);
	PX_Int(pPX, cszRPatt_RecurrenceOpt_Data2, m_Options.m_YearNth.nWhichDayMask, 0);
	PX_Int(pPX, cszRPatt_RecurrenceOpt_Data3, m_Options.m_YearNth.nMonthOfYear, 0);

	PX_DateTime(pPX, cszRPatt_PatternStartDate, m_dtPatternStartDate);

	PX_Enum(pPX, cszRPatt_UseEndMethod, m_PatternEnd.m_nUseEnd, xtpCalendarPatternEndNoDate);

	if (m_PatternEnd.m_nUseEnd == xtpCalendarPatternEndDate)
	{
		PX_DateTime(pPX, cszRPatt_PatternEndDate, m_PatternEnd.m_dtPatternEndDate);
	}
	else if (m_PatternEnd.m_nUseEnd == xtpCalendarPatternEndAfterOccurrences)
	{
		PX_Int(pPX, cszRPatt_EndAfterOccurrences, m_PatternEnd.m_nEndAfterOccurrences, 1);
	}

	PX_DWord(pPX, cszRPatt_MasterEventID, m_dwMasterEventID, XTP_CALENDAR_UNKNOWN_EVENT_ID);

	//------------------------------------------------------------------------
	if (pPX->IsStoring())
	{
		VERIFY(m_arOccReminders.Save(m_pCustomProperties, this));
	}

	//-------------------------------------------
	m_pCustomProperties->DoPropExchange(pPX);

	//-------------------------------------------
	if (pPX->IsLoading())
	{
		VERIFY(m_arOccReminders.Load(m_pCustomProperties));
	}

	m_arOccReminders.ClearProperties(m_pCustomProperties);
}

void CXTPCalendarRecurrencePattern::SetOccReminder(CXTPCalendarEvent* pOccEvent, int nbIsReminder,
												   DATE dtNextReminderTime_Snoozed)
{
	ASSERT(pOccEvent);
	if (!pOccEvent)
	{
		return;
	}

	int nIndex = m_arOccReminders.Find(pOccEvent);
	if (nIndex < 0)
	{
		CXTPCalendarReminderForOccurrence tmpData;
		tmpData.m_dtOccurrenceStartTime = pOccEvent->GetStartTime();
		tmpData.m_dtOccurrenceEndTime   = pOccEvent->GetEndTime();

		nIndex = (int)m_arOccReminders.Add(tmpData);
	}

	CXTPCalendarReminderForOccurrence* pOccRmdData;
	pOccRmdData = &m_arOccReminders[nIndex];

	if (nbIsReminder != xtpCalendarRmdPrm_DontChange)
	{
		pOccRmdData->m_nbIsReminder = nbIsReminder;
	}

	if (static_cast<int>(dtNextReminderTime_Snoozed) != xtpCalendarRmdPrm_DontChange)
	{
		pOccRmdData->m_dtNextReminderTime_Snoozed = dtNextReminderTime_Snoozed;
	}

	m_arOccReminders.UpdateOccIfNeedEx(pOccEvent, pOccRmdData, this);

	if (pOccRmdData->m_nbIsReminder == xtpCalendarRmdPrm_Default
		&& static_cast<int>(pOccRmdData->m_dtNextReminderTime_Snoozed) == xtpCalendarRmdPrm_Default)
	{
		m_arOccReminders.RemoveAt(nIndex);
	}
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarReminderForOccurrence::CXTPCalendarReminderForOccurrence()
{
	m_dtOccurrenceStartTime = 0;
	m_dtOccurrenceEndTime   = 0;

	m_nbIsReminder				 = xtpCalendarRmdPrm_Default;
	m_dtNextReminderTime_Snoozed = (DATE)xtpCalendarRmdPrm_Default;
}

CXTPCalendarReminderForOccurrence::~CXTPCalendarReminderForOccurrence()
{
}

const CXTPCalendarReminderForOccurrence& CXTPCalendarReminderForOccurrence::
	operator=(const CXTPCalendarReminderForOccurrence& rSrc)
{
	m_dtOccurrenceStartTime = rSrc.m_dtOccurrenceStartTime;
	m_dtOccurrenceEndTime   = rSrc.m_dtOccurrenceEndTime;

	m_nbIsReminder				 = rSrc.m_nbIsReminder;
	m_dtNextReminderTime_Snoozed = rSrc.m_dtNextReminderTime_Snoozed;

	return *this;
}

//===========================================================================
CXTPCalendarReminderForOccurrenceArray::CXTPCalendarReminderForOccurrenceArray()
{
}

CXTPCalendarReminderForOccurrenceArray::~CXTPCalendarReminderForOccurrenceArray()
{
}

int CXTPCalendarReminderForOccurrenceArray::Find(CXTPCalendarEvent* pOccEvent) const
{
	ASSERT(pOccEvent);
	if (!pOccEvent)
	{
		return -1;
	}
	ASSERT(pOccEvent->GetRecurrenceState() == xtpCalendarRecurrenceOccurrence);

	DATE dtOcctartTime = pOccEvent->GetStartTime();
	DATE dtOccEndTime  = pOccEvent->GetEndTime();

	int nFIndex = Find(dtOcctartTime, dtOccEndTime);
	return nFIndex;
}

int CXTPCalendarReminderForOccurrenceArray::Find(DATE dtOccStartTime, DATE dtOccEndTime) const
{
	int nCount = (int)GetSize();
	for (int i = 0; i < nCount; i++)
	{
		const CXTPCalendarReminderForOccurrence& rData = m_pData[i];

		if (CXTPCalendarUtils::IsEqual(dtOccStartTime, rData.m_dtOccurrenceStartTime)
			&& CXTPCalendarUtils::IsEqual(dtOccEndTime, rData.m_dtOccurrenceEndTime))
		{
			return i;
		}
	}
	return -1;
}

int CXTPCalendarReminderForOccurrenceArray::UpdateOccIfNeed(CXTPCalendarEvent* pOccEvent,
															CXTPCalendarRecurrencePattern* pPattern)
{
	if (!pOccEvent || !pPattern)
	{
		ASSERT(FALSE);
		return 0;
	}

	int nFIndex = Find(pOccEvent);
	if (nFIndex < 0)
	{
		return 0;
	}

	CXTPCalendarReminderForOccurrence* pOccRmdData;
	pOccRmdData = &ElementAt(nFIndex);

	int nRes = UpdateOccIfNeedEx(pOccEvent, pOccRmdData, pPattern);
	return nRes;
}

int AFX_CDECL CXTPCalendarReminderForOccurrenceArray::UpdateOccIfNeedEx(
	CXTPCalendarEvent* pOccEvent, const CXTPCalendarReminderForOccurrence* pOccRmdData,
	CXTPCalendarRecurrencePattern* pPattern)
{
	if (!pOccEvent || !pOccRmdData || !pPattern)
	{
		ASSERT(FALSE);
		return 0;
	}

	// IsReminder
	if (pOccRmdData->m_nbIsReminder == xtpCalendarRmdPrm_Default)
	{
		CXTPCalendarEventPtr ptrMasterEvent = pPattern->GetMasterEvent();
		if (!ptrMasterEvent)
		{
			ASSERT(FALSE);
			return 0;
		}
		pOccEvent->SetReminder(ptrMasterEvent->IsReminder());
	}
	else if (pOccRmdData->m_nbIsReminder != xtpCalendarRmdPrm_DontChange)
	{
		pOccEvent->SetReminder(pOccRmdData->m_nbIsReminder != 0);
	}

	// MinutesBeforeStart_Snoozed
	if (static_cast<int>(pOccRmdData->m_dtNextReminderTime_Snoozed) == xtpCalendarRmdPrm_Default)
	{
		pOccEvent->GetCustomProperties()->RemoveProperty(cszEventCustProp_NextReminderTime_Snoozed);
	}
	else if (static_cast<int>(pOccRmdData->m_dtNextReminderTime_Snoozed)
			 != xtpCalendarRmdPrm_DontChange)
	{
		VERIFY(pOccEvent->GetCustomProperties()->SetProperty(
			cszEventCustProp_NextReminderTime_Snoozed, pOccRmdData->m_dtNextReminderTime_Snoozed));
	}
	return 1;
}

BOOL CXTPCalendarReminderForOccurrenceArray::Load(CXTPCalendarCustomProperties* pProps)
{
	ASSERT(pProps);
	if (!pProps)
	{
		return FALSE;
	}
	RemoveAll();

	COleVariant varCount;
	BOOL bExists = pProps->GetProperty(cszOccRMD_Count, varCount);
	if (!bExists)
	{
		return TRUE;
	}
	COleVariant varValue;
	CString strPropNameI;

	int nCount = (int)(long)_variant_t(varCount);
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarReminderForOccurrence rmdData;

		_PropName(cszOccRMD_StartTime, i, strPropNameI);
		if (pProps->GetProperty(strPropNameI, varValue))
		{
			rmdData.m_dtOccurrenceStartTime = (DATE)_variant_t(varValue);
		}

		_PropName(cszOccRMD_EndTime, i, strPropNameI);
		if (pProps->GetProperty(strPropNameI, varValue))
		{
			rmdData.m_dtOccurrenceEndTime = (DATE)_variant_t(varValue);
		}

		_PropName(cszOccRMD_IsReminder, i, strPropNameI);
		if (pProps->GetProperty(strPropNameI, varValue))
		{
			rmdData.m_nbIsReminder = (long)_variant_t(varValue);
		}

		_PropName(cszOccRMD_NextReminderTime_Snoozed, i, strPropNameI);
		if (pProps->GetProperty(strPropNameI, varValue))
		{
			rmdData.m_dtNextReminderTime_Snoozed = (DATE)_variant_t(varValue);
		}

		Add(rmdData);
	}
	return TRUE;
}

BOOL CXTPCalendarReminderForOccurrenceArray::Save(CXTPCalendarCustomProperties* pProps,
												  CXTPCalendarRecurrencePattern* pPattern)
{
	ASSERT(pProps);
	if (!pProps)
	{
		return FALSE;
	}
	ClearProperties(pProps);

	CString strPropNameI;
	int nCount = (int)GetSize();
	int i;

	//===========================================================================
	if (pPattern)
	{
		for (i = nCount - 1; i >= 0; i--)
		{
			const CXTPCalendarReminderForOccurrence& rData = ElementAt(i);

			int nFExcIdx = pPattern->FindExceptionIndex(rData.m_dtOccurrenceStartTime,
														rData.m_dtOccurrenceEndTime);
			if (nFExcIdx >= 0)
			{
				RemoveAt(i);
			}
		}
	}

	//===========================================================================
	nCount = (int)GetSize();
	for (i = 0; i < nCount; i++)
	{
		const CXTPCalendarReminderForOccurrence& rData = ElementAt(i);

		_PropName(cszOccRMD_StartTime, i, strPropNameI);
		VERIFY(pProps->SetProperty(strPropNameI, COleDateTime(rData.m_dtOccurrenceStartTime)));

		_PropName(cszOccRMD_EndTime, i, strPropNameI);
		VERIFY(pProps->SetProperty(strPropNameI, COleDateTime(rData.m_dtOccurrenceEndTime)));

		_PropName(cszOccRMD_IsReminder, i, strPropNameI);
		VERIFY(pProps->SetProperty(strPropNameI, (long)rData.m_nbIsReminder));

		if (static_cast<int>(rData.m_dtNextReminderTime_Snoozed) != xtpCalendarRmdPrm_Default)
		{
			ASSERT(rData.m_dtNextReminderTime_Snoozed >= xtpCalendarDateTime_min
				   && rData.m_dtNextReminderTime_Snoozed <= xtpCalendarDateTime_max);
			_PropName(cszOccRMD_NextReminderTime_Snoozed, i, strPropNameI);
			VERIFY(pProps->SetProperty(strPropNameI,
									   COleDateTime(rData.m_dtNextReminderTime_Snoozed)));
		}
	}

	BOOL bRes = pProps->SetProperty(cszOccRMD_Count, (long)nCount);
	ASSERT(bRes);

	return bRes;
}

void AFX_CDECL
	CXTPCalendarReminderForOccurrenceArray::ClearProperties(CXTPCalendarCustomProperties* pProps)
{
	ASSERT(pProps);
	if (!pProps)
	{
		return;
	}
	COleVariant varCount;
	BOOL bExists = pProps->GetProperty(cszOccRMD_Count, varCount);
	if (!bExists)
	{
		return;
	}
	CString strPropNameI;

	int nCount = (int)(long)_variant_t(varCount);
	for (int i = 0; i < nCount; i++)
	{
		_PropName(cszOccRMD_StartTime, i, strPropNameI);
		pProps->RemoveProperty(strPropNameI);

		_PropName(cszOccRMD_EndTime, i, strPropNameI);
		pProps->RemoveProperty(strPropNameI);

		_PropName(cszOccRMD_IsReminder, i, strPropNameI);
		pProps->RemoveProperty(strPropNameI);

		_PropName(cszOccRMD_NextReminderTime_Snoozed, i, strPropNameI);
		pProps->RemoveProperty(strPropNameI);
	}

	pProps->RemoveProperty(cszOccRMD_Count);
}
void AFX_CDECL CXTPCalendarReminderForOccurrenceArray::_PropName(LPCTSTR pcszProp, int nIndex,
																 CString& rstrPropNameI)
{
	TCHAR szIndex[64];
	ITOT_S(nIndex, szIndex, _countof(szIndex), 10);

	rstrPropNameI = pcszProp;
	rstrPropNameI += _T("[");
	rstrPropNameI += szIndex;
	rstrPropNameI += _T("]");
}

COleDateTimeSpan CXTPCalendarRecurrencePattern::GetDuration() const
{
	return CXTPCalendarUtils::Minutes2Span(m_nDurationMinutes);
}

#ifdef _XTP_ACTIVEX

#	define DECLARE_OPTION(szExternalName, vtPropType)                                             \
		vtPropType OleGet##szExternalName();                                                       \
		void OleSet##szExternalName(vtPropType);

#	define IMPLEMENT_OPTION(szExternalName, vtPropType, code)                                     \
		vtPropType CXTPCalendarRecurrencePatternOptions::OleGet##szExternalName()                  \
		{                                                                                          \
			return (vtPropType)Options.##code;                                                     \
		}                                                                                          \
		void CXTPCalendarRecurrencePatternOptions::OleSet##szExternalName(vtPropType type)         \
		{                                                                                          \
			Options.##code = (vtPropType)type;                                                     \
		}

#	define IMPLEMENT_OPTION_EX(szExternalName, vtPropType, code, memberType)                      \
		vtPropType CXTPCalendarRecurrencePatternOptions::OleGet##szExternalName()                  \
		{                                                                                          \
			return (memberType)Options.##code;                                                     \
		}                                                                                          \
		void CXTPCalendarRecurrencePatternOptions::OleSet##szExternalName(vtPropType type)         \
		{                                                                                          \
			Options.##code = (memberType)type;                                                     \
		}

#	define DISP_OPTION(szExternalName, dispid, vtPropType)                                        \
		DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePatternOptions, #szExternalName, dispid,         \
							OleGet##szExternalName, OleSet##szExternalName, vtPropType)

class _XTP_EXT_CLASS CXTPCalendarRecurrencePatternOptions : public CXTPCmdTarget
{
public:
	CXTPCalendarRecurrencePatternOptions(XTP_CALENDAR_RECURRENCE_OPTIONS& pOptions)
		: Options(pOptions)
	{
		EnableAutomation();
		EnableTypeLib();
	}
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OPTION(DailyEveryWeekDayOnly, BOOL)
	DECLARE_OPTION(DailyIntervalDays, long)
	DECLARE_OPTION(WeeklyIntervalWeeks, long)
	DECLARE_OPTION(WeeklyDayOfWeekMask, long)
	DECLARE_OPTION(MonthlyIntervalMonths, long)
	DECLARE_OPTION(MonthlyDayOfMonth, long)
	DECLARE_OPTION(MonthNthIntervalMonths, long)
	DECLARE_OPTION(MonthNthWhichDay, long)
	DECLARE_OPTION(MonthNthWhichDayMask, long)
	DECLARE_OPTION(YearlyMonthOfYear, long)
	DECLARE_OPTION(YearlyDayOfMonth, long)
	DECLARE_OPTION(YearNthWhichDay, long)
	DECLARE_OPTION(YearNthWhichDayMask, long)
	DECLARE_OPTION(YearNthMonthOfYear, long)

	DECLARE_OPTION(RecurrenceType, XTPCalendarEventRecurrenceType)

	DECLARE_OPTION(Data1, long)
	DECLARE_OPTION(Data2, long)
	DECLARE_OPTION(Data3, long)
	DECLARE_OPTION(Data4, long)

	DECLARE_OLETYPELIB_EX(CXTPCalendarRecurrencePatternOptions);

	XTP_CALENDAR_RECURRENCE_OPTIONS& Options;
};

BEGIN_DISPATCH_MAP(CXTPCalendarRecurrencePatternOptions, CXTPCmdTarget)
	DISP_OPTION(DailyEveryWeekDayOnly, 1, VT_BOOL)
	DISP_OPTION(DailyIntervalDays, 2, VT_I4)
	DISP_OPTION(WeeklyIntervalWeeks, 3, VT_I4)
	DISP_OPTION(WeeklyDayOfWeekMask, 4, VT_I4)
	DISP_OPTION(MonthlyIntervalMonths, 5, VT_I4)
	DISP_OPTION(MonthlyDayOfMonth, 6, VT_I4)
	DISP_OPTION(MonthNthIntervalMonths, 7, VT_I4)
	DISP_OPTION(MonthNthWhichDay, 8, VT_I4)
	DISP_OPTION(MonthNthWhichDayMask, 9, VT_I4)
	DISP_OPTION(YearlyMonthOfYear, 10, VT_I4)
	DISP_OPTION(YearlyDayOfMonth, 11, VT_I4)
	DISP_OPTION(YearNthWhichDay, 12, VT_I4)
	DISP_OPTION(YearNthWhichDayMask, 13, VT_I4)
	DISP_OPTION(YearNthMonthOfYear, 14, VT_I4)

	DISP_OPTION(RecurrenceType, 15, VT_I4)

	DISP_OPTION(Data1, 21, VT_I4)
	DISP_OPTION(Data2, 22, VT_I4)
	DISP_OPTION(Data3, 23, VT_I4)
	DISP_OPTION(Data4, 24, VT_I4)

END_DISPATCH_MAP()

IMPLEMENT_OPTION(DailyEveryWeekDayOnly, BOOL, m_Daily.bEveryWeekDayOnly)
IMPLEMENT_OPTION(DailyIntervalDays, long, m_Daily.nIntervalDays)
IMPLEMENT_OPTION(WeeklyIntervalWeeks, long, m_Weekly.nIntervalWeeks)
IMPLEMENT_OPTION(WeeklyDayOfWeekMask, long, m_Weekly.nDayOfWeekMask)
IMPLEMENT_OPTION(MonthlyIntervalMonths, long, m_Monthly.nIntervalMonths)
IMPLEMENT_OPTION(MonthlyDayOfMonth, long, m_Monthly.nDayOfMonth)
IMPLEMENT_OPTION(MonthNthIntervalMonths, long, m_MonthNth.nIntervalMonths)
IMPLEMENT_OPTION(MonthNthWhichDay, long, m_MonthNth.nWhichDay)
IMPLEMENT_OPTION(MonthNthWhichDayMask, long, m_MonthNth.nWhichDayMask)
IMPLEMENT_OPTION(YearlyMonthOfYear, long, m_Yearly.nMonthOfYear)
IMPLEMENT_OPTION(YearlyDayOfMonth, long, m_Yearly.nDayOfMonth)
IMPLEMENT_OPTION(YearNthWhichDay, long, m_YearNth.nWhichDay)
IMPLEMENT_OPTION(YearNthWhichDayMask, long, m_YearNth.nWhichDayMask)
IMPLEMENT_OPTION(YearNthMonthOfYear, long, m_YearNth.nMonthOfYear)

IMPLEMENT_OPTION(RecurrenceType, XTPCalendarEventRecurrenceType, m_nRecurrenceType)

IMPLEMENT_OPTION_EX(Data1, long, m_nRecurrenceType, XTPCalendarEventRecurrenceType);
IMPLEMENT_OPTION(Data2, long, m_YearNth.nWhichDay);
IMPLEMENT_OPTION(Data3, long, m_YearNth.nWhichDayMask);
IMPLEMENT_OPTION(Data4, long, m_YearNth.nMonthOfYear);

BEGIN_INTERFACE_MAP(CXTPCalendarRecurrencePatternOptions, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarRecurrencePatternOptions, XTPDIID_CalendarRecurrencePatternOptions,
				   Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarRecurrencePatternOptions,
						XTPDIID_CalendarRecurrencePatternOptions)

BEGIN_DISPATCH_MAP(CXTPCalendarRecurrencePattern, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePattern, "StartTime", 1, OleGetStartTime,
						OleSetStartTime, VT_DATE)
	DISP_PROPERTY_ID(CXTPCalendarRecurrencePattern, "DurationMinutes", 2, m_nDurationMinutes, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePattern, "StartDate", 3, OleGetStartDate,
						OleSetStartDate, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePattern, "EndDate", 4, OleGetEndDate, OleSetEndDate,
						VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePattern, "EndAfterOccurrences", 5,
						OleGetEndAfterOccurrences, OleSetEndAfterOccurrences, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarRecurrencePattern, "EndMethod", 6, m_PatternEnd.m_nUseEnd, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePattern, "MasterEvent", 8, OleGetMasterEvent,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPCalendarRecurrencePattern, "Options", 9, OleGetOptions, SetNotSupported,
						VT_DISPATCH)

	DISP_FUNCTION_ID(CXTPCalendarRecurrencePattern, "Exceptions", 10, OleGetExceptions, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarRecurrencePattern, "SetException", 11, OleSetException, VT_EMPTY,
					 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarRecurrencePattern, "RemoveException", 12, OleRemoveException,
					 VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarRecurrencePattern, "RemoveAllExceptions", 13,
					 OleRemoveAllExceptions, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarRecurrencePattern, "CustomProperties", 14, OleCustomProperties,
					 VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarRecurrencePattern, "Id", 15, OleGetId, VT_I4, VTS_NONE)

	DISP_PROPERTY_ID(CXTPCalendarRecurrencePattern, "MasterEventId", 16, m_dwMasterEventID, VT_I4)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarRecurrencePattern, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarRecurrencePattern, XTPDIID_CalendarRecurrencePattern, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarRecurrencePattern, XTPDIID_CalendarRecurrencePattern)

DATE CXTPCalendarRecurrencePattern::OleGetStartTime()
{
	return GetStartTime();
}
void CXTPCalendarRecurrencePattern::OleSetStartTime(DATE dt)
{
	SetStartTime(dt);
}
DATE CXTPCalendarRecurrencePattern::OleGetStartDate()
{
	return GetPatternStartDate();
}
void CXTPCalendarRecurrencePattern::OleSetStartDate(DATE dt)
{
	SetPatternStartDate(dt);
}
DATE CXTPCalendarRecurrencePattern::OleGetEndDate()
{
	return GetPatternEndDate();
}
void CXTPCalendarRecurrencePattern::OleSetEndDate(DATE dt)
{
	SetPatternEndDate(dt);
}
long CXTPCalendarRecurrencePattern::OleGetEndAfterOccurrences()
{
	return GetEndAfterOccurrences();
}
void CXTPCalendarRecurrencePattern::OleSetEndAfterOccurrences(int nOccurrences)
{
	SetEndAfterOccurrences(nOccurrences);
}
LPDISPATCH CXTPCalendarRecurrencePattern::OleGetMasterEvent()
{
	CXTPCalendarEventPtr pEvent = GetMasterEvent();
	return pEvent ? pEvent->GetIDispatch(TRUE) : 0;
}
LPDISPATCH CXTPCalendarRecurrencePattern::OleGetOptions()
{
	CXTPCalendarRecurrencePatternOptions* pOptions = new CXTPCalendarRecurrencePatternOptions(
		m_Options);
	return pOptions->GetIDispatch(FALSE);
}

LPDISPATCH CXTPCalendarRecurrencePattern::OleGetExceptions()
{
	CXTPCalendarEventsPtr pEvents = GetExceptions();
	return pEvents ? pEvents->GetIDispatch(TRUE) : 0;
}

LPDISPATCH CXTPCalendarRecurrencePattern::OleCustomProperties()
{
	CXTPCalendarCustomProperties* pProps = GetCustomProperties();
	return pProps ? pProps->GetIDispatch(TRUE) : NULL;
}

void CXTPCalendarRecurrencePattern::OleSetException(LPDISPATCH pExcEventDisp)
{
	CXTPCalendarEvent* pExc = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPCalendarEvent,
														   CXTPCalendarEvent::FromIDispatchSafe(
															   pExcEventDisp));
	if (!pExc)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	SetException(pExc);
}

void CXTPCalendarRecurrencePattern::OleRemoveException(LPDISPATCH pExcEventDisp)
{
	CXTPCalendarEvent* pExc = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPCalendarEvent,
														   CXTPCalendarEvent::FromIDispatchSafe(
															   pExcEventDisp));
	if (!pExc)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	RemoveException(pExc);
}

void CXTPCalendarRecurrencePattern::OleRemoveAllExceptions()
{
	RemoveAllExceptions();
}
long CXTPCalendarRecurrencePattern::OleGetId()
{
	return (long)GetPatternID();
}

#endif
