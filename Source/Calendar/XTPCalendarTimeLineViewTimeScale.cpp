// XTPCalendarTimeLineViewTimeScale.cpp: implementation of the
// CXTPCalendarTimeLineViewTimeScale class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarTimeLineViewTimeScale.h"
#include "Calendar/XTPCalendarTimeLineView.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarOptions.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPCalendarTimeLineViewTimeScale::CXTPCalendarTimeLineViewTimeScale(CXTPCalendarTimeLineView* pView)
{
	m_rcScale.SetRectEmpty();
	m_rcRect.SetRectEmpty();

	m_pParentView = pView;

	m_pTimeScaleParams = NULL;

	m_dtStartViewDate = COleDateTime::GetCurrentTime();
	SetTimeScaleParams(xtpTSPID_Day);
}

CXTPCalendarTimeLineViewTimeScale::~CXTPCalendarTimeLineViewTimeScale()
{
	CMDTARGET_RELEASE(m_pTimeScaleParams);
}

void CXTPCalendarTimeLineViewTimeScale::SetTimeScaleParams(int eTimeScaleParamsID)
{
	CMDTARGET_RELEASE(m_pTimeScaleParams);

	switch (eTimeScaleParamsID)
	{
		case xtpTSPID_Day:
			m_pTimeScaleParams = new CXTPCalendarTLV_TimeScaleParams_Day(this);
			break;
		case xtpTSPID_Week:
			m_pTimeScaleParams = new CXTPCalendarTLV_TimeScaleParams_Week(this);
			break;
		case xtpTSPID_Month:
			m_pTimeScaleParams = new CXTPCalendarTLV_TimeScaleParams_Month(this);
			break;
		case xtpTSPID_WorkWeek:
			m_pTimeScaleParams = new CXTPCalendarTLV_TimeScaleParams_WorkWeek(this);
			break;
		default: ASSERT(FALSE); m_pTimeScaleParams = new CXTPCalendarTLV_TimeScaleParams_Day(this);
	}
}

void CXTPCalendarTimeLineViewTimeScale::SetTimeScaleParams2(
	CXTPCalendarTLV_TimeScaleParams* pSParams)
{
	if (!pSParams)
	{
		ASSERT(FALSE);
		return;
	}

	ASSERT(pSParams->GetParentTimeScale() == this);
	m_pTimeScaleParams = pSParams;
}

void CXTPCalendarTimeLineViewTimeScale::AdjustLayout(CRect rcParentRect)
{
	if (!m_pTimeScaleParams)
	{
		ASSERT(FALSE);
		return;
	}

	m_rcRect.CopyRect(&rcParentRect);
	m_rcScale.CopyRect(&rcParentRect);

	CClientDC dc(GetView()->GetCalendarControl());

	int nHeight									 = XTP_DPI_Y(68);
	CXTPCalendarTimeLineViewTimeScalePart* pPart = m_pTimeScaleParams->GetPart();
	if (pPart)
		nHeight = pPart->CalcHeigt(&dc);
	m_rcScale.bottom = m_rcScale.top + nHeight;

	m_rcRect.bottom = m_rcScale.bottom;
}

CRect CXTPCalendarTimeLineViewTimeScale::GetClientRect(int nYstart)
{
	CRect rcClient = m_rcRect;

	rcClient.top += nYstart;
	rcClient.bottom += nYstart;

	return rcClient;
}

void CXTPCalendarTimeLineViewTimeScale::Draw(CDC* pDC)
{
	if (!m_pTimeScaleParams)
		return;

	m_pTimeScaleParams->GetPart()->Draw(pDC, m_rcScale, GetView());
}

BOOL CXTPCalendarTimeLineViewTimeScale::HitTestEx(CPoint pt,
												  XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	UNREFERENCED_PARAMETER(pHitTest);
	return m_rcScale.PtInRect(pt);
}

////////////////////////////////////////////////////////////////////////////
// class CXTPCalendarTLV_TimeScaleParams : public CXTPInternalUnknown

CXTPCalendarTLV_TimeScaleParams::CXTPCalendarTLV_TimeScaleParams(
	int nTimeScaleParamsID, CXTPCalendarTimeLineViewTimeScale* pTimeScale)
{
	m_nTimeScaleParamsID = nTimeScaleParamsID;
	::ZeroMemory(&m_scaleInterval, sizeof(m_scaleInterval));

	m_nScaleIntervalWidth_px = 0;
	m_pTimeScale			 = pTimeScale;
}

CXTPCalendarTLV_TimeScaleParams::~CXTPCalendarTLV_TimeScaleParams()
{
}

void CXTPCalendarTLV_TimeScaleParams::SetScaleInterval(int eType, int nValue)
{
	ASSERT(eType && nValue > 0);

	m_scaleInterval.m_eType  = eType;
	m_scaleInterval.m_nValue = nValue;
}

COleDateTimeSpan CXTPCalendarTLV_TimeScaleParams::GetScaleInterval() const
{
	COleDateTimeSpan spInterval(0);
	switch (m_scaleInterval.m_eType)
	{
		case xtpDTScale_Sec: spInterval.SetDateTimeSpan(0, 0, 0, m_scaleInterval.m_nValue); break;
		case xtpDTScale_Min: spInterval.SetDateTimeSpan(0, 0, m_scaleInterval.m_nValue, 0); break;
		case xtpDTScale_Hour: spInterval.SetDateTimeSpan(0, m_scaleInterval.m_nValue, 0, 0); break;

		case xtpDTScale_Day: spInterval.SetDateTimeSpan(m_scaleInterval.m_nValue, 0, 0, 0); break;
		case xtpDTScale_WorkWeek:
			spInterval.SetDateTimeSpan(m_scaleInterval.m_nValue * 5, 0, 0, 0);
			break;
		case xtpDTScale_Week:
			spInterval.SetDateTimeSpan(m_scaleInterval.m_nValue * 7, 0, 0, 0);
			break;
		case xtpDTScale_Month:
			spInterval.SetDateTimeSpan(m_scaleInterval.m_nValue * 31, 0, 0, 0);
			break;
		case xtpDTScale_Year:
			spInterval.SetDateTimeSpan(m_scaleInterval.m_nValue * 366, 0, 0, 0);
			break;
		default: ASSERT(FALSE);
	}
	return spInterval;
}

COleDateTime CXTPCalendarTLV_TimeScaleParams::CalcScaleIntervalBegin(COleDateTime dtStart)
{
	if (!m_scaleInterval.m_nValue)
	{
		ASSERT(FALSE);
		return dtStart;
	}

	COleDateTime dtBegin = dtStart;

	int nFirstDayOfWeekIndex = 0;
	COleDateTimeSpan spAdd;

	LONGLONG nVal = 0;
	LONGLONG nDiv = 0;
	// int nMod = 0;
	LONGLONG nNewVal = 0;

	LONGLONG nllDays_total = (int)(DATE)dtStart;

	switch (m_scaleInterval.m_eType)
	{
		case xtpDTScale_Sec:
			nVal	= dtStart.GetSecond() + nllDays_total * 24 * 60 * 60;
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = nDiv * m_scaleInterval.m_nValue;

			dtBegin = (DATE)(nNewVal / (24 * 60 * 60));
			dtBegin = CXTPCalendarUtils::UpdateTime(dtBegin, (int)(nNewVal % (24 * 60 * 60)),
													(int)(nNewVal % (60 * 60)),
													(int)(nNewVal % 60));

			/*nVal = dtStart.GetSecond();
			nDiv = nVal / m_scaleInterval.m_nValue;
			//nMod = nVal % m_scaleInterval.m_nValue;
			nNewVal = (nDiv + (nMod ? 1 : 0)) * m_scaleInterval.m_nValue;

			spAdd.SetDateTimeSpan(0, 0, 0, nNewVal);
			dtBegin = CXTPCalendarUtils::UpdateTime(dtBegin, dtStart.GetHour(), dtStart.GetMinute(),
			0); dtBegin += spAdd;*/

			break;
		case xtpDTScale_Min:
			nVal = dtStart.GetMinute() /*+ (dtStart.GetSecond() ? 1 : 0)*/;
			nDiv = nVal / m_scaleInterval.m_nValue;
			// nMod = nVal % m_scaleInterval.m_nValue;
			// nNewVal = (nDiv +(nMod?1:0)) * m_scaleInterval.m_nValue;
			nNewVal = nDiv * m_scaleInterval.m_nValue;

			spAdd.SetDateTimeSpan(0, 0, (int)nNewVal, 0);
			dtBegin = CXTPCalendarUtils::UpdateTime(dtStart, dtStart.GetHour(), 0, 0);
			dtBegin += spAdd;

			break;

		case xtpDTScale_Hour:
			nVal	= dtStart.GetHour();
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = (nDiv)*m_scaleInterval.m_nValue;

			spAdd.SetDateTimeSpan(0, (int)nNewVal, 0, 0);
			dtBegin = CXTPCalendarUtils::ResetTime(dtBegin);
			dtBegin += spAdd;

			break;

		case xtpDTScale_Day:
			nVal	= dtStart.GetDay();
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = (nDiv)*m_scaleInterval.m_nValue;

			// spAdd.SetDateTimeSpan((int)max(0, nNewVal - 1), 0, 0, 0);
			// dtBegin = CXTPCalendarUtils::ResetTime(dtBegin);
			// CXTPCalendarUtils::UpdateMonthDay(dtBegin, 1);
			// dtBegin += spAdd;

			dtBegin = CXTPCalendarUtils::ResetTime(dtBegin);
			CXTPCalendarUtils::UpdateMonthDay(dtBegin, (int)nNewVal);

			break;
		case xtpDTScale_Week:
			// update first day of week
			nFirstDayOfWeekIndex = XTP_SAFE_GET3(m_pTimeScale, GetView(), GetCalendarControl(),
												 GetFirstDayOfWeek(), 2);
			dtBegin = CXTPCalendarUtils::ShiftDateToWeekBegin(dtStart, nFirstDayOfWeekIndex);

			nVal	= (int)(DATE)dtStart;
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = (nDiv)*m_scaleInterval.m_nValue;

			dtBegin = (DATE)nNewVal;

			break;

		case xtpDTScale_WorkWeek:
			// update first day of week
			nFirstDayOfWeekIndex = XTP_SAFE_GET3(m_pTimeScale, GetView(), GetCalendarControl(),
												 GetFirstDayOfWeek(), 2);
			dtBegin = CXTPCalendarUtils::ShiftDateToWeekBegin(dtStart, nFirstDayOfWeekIndex);

			nVal	= (int)(DATE)dtStart;
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = (nDiv)*m_scaleInterval.m_nValue;

			dtBegin = (DATE)nNewVal;

			break;
		case xtpDTScale_Month:
			dtBegin = CXTPCalendarUtils::ResetTime(dtStart);

			nVal	= dtStart.GetMonth() + (dtStart.GetYear() - 1900) * 12;
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = (nDiv)*m_scaleInterval.m_nValue;
			{
				int nNewYear  = (int)(nNewVal / 12 + 1900);
				int nNewMonth = (int)(nNewVal % 12);

				dtBegin.SetDate(nNewYear, nNewMonth, 1);
			}

			break;
		case xtpDTScale_Year:
			dtBegin = CXTPCalendarUtils::ResetTime(dtStart);

			nVal	= dtStart.GetYear();
			nDiv	= nVal / m_scaleInterval.m_nValue;
			nNewVal = (nDiv)*m_scaleInterval.m_nValue;

			dtBegin.SetDate((int)nNewVal, 1, 1);

			break;
		default: ASSERT(FALSE);
	}

	return dtBegin;
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarTLV_TimeScaleParams_Day::CXTPCalendarTLV_TimeScaleParams_Day(
	CXTPCalendarTimeLineViewTimeScale* pTimeScale)
	: CXTPCalendarTLV_TimeScaleParams(xtpTSPID_Day, pTimeScale)
{
	SetScaleInterval(xtpDTScale_Min, 30);
	// SetScaleInterval(xtpDTScale_Hour, 1);

	m_nScaleIntervalWidth_px = XTP_DPI_X(31);
}

CXTPCalendarTLV_TimeScaleParams_Day::~CXTPCalendarTLV_TimeScaleParams_Day()
{
}

// CString CXTPCalendarTLV_TimeScaleParams_Day::FormatLowerScaleLabel(COleDateTime dtDateTime)
//{
//  return _T("");
//}

// void CXTPCalendarTLV_TimeScaleParams_Day::DrawLowerScale(CDC* pDC, DATE dtStart, const CRect&
// rcRect)
//{
//
//}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarTLV_TimeScaleParams_Week::CXTPCalendarTLV_TimeScaleParams_Week(
	CXTPCalendarTimeLineViewTimeScale* pTimeScale)
	: CXTPCalendarTLV_TimeScaleParams(xtpTSPID_Week, pTimeScale)
{
	SetScaleInterval(xtpDTScale_Day, 1);

	m_nScaleIntervalWidth_px = XTP_DPI_X(86);
}

CXTPCalendarTLV_TimeScaleParams_Week::~CXTPCalendarTLV_TimeScaleParams_Week()
{
}

// CString CXTPCalendarTLV_TimeScaleParams_Week::FormatLowerScaleLabel(COleDateTime dtDateTime)
//{
//  return _T("");
//}

// void CXTPCalendarTLV_TimeScaleParams_Week::DrawLowerScale(CDC* pDC, DATE dtStart, const CRect&
// rcRect)
//{
//
//}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarTLV_TimeScaleParams_WorkWeek::CXTPCalendarTLV_TimeScaleParams_WorkWeek(
	CXTPCalendarTimeLineViewTimeScale* pTimeScale)
	: CXTPCalendarTLV_TimeScaleParams(xtpTSPID_WorkWeek, pTimeScale)
{
	SetScaleInterval(xtpDTScale_Day, 1);

	m_nScaleIntervalWidth_px = XTP_DPI_X(86);
}

CXTPCalendarTLV_TimeScaleParams_WorkWeek::~CXTPCalendarTLV_TimeScaleParams_WorkWeek()
{
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarTLV_TimeScaleParams_Month::CXTPCalendarTLV_TimeScaleParams_Month(
	CXTPCalendarTimeLineViewTimeScale* pTimeScale)
	: CXTPCalendarTLV_TimeScaleParams(xtpTSPID_Month, pTimeScale)
{
	SetScaleInterval(xtpDTScale_Day, 1);

	m_nScaleIntervalWidth_px = XTP_DPI_X(30);
}

CXTPCalendarTLV_TimeScaleParams_Month::~CXTPCalendarTLV_TimeScaleParams_Month()
{
}

// CString CXTPCalendarTLV_TimeScaleParams_Month::FormatLowerScaleLabel(COleDateTime dtDateTime)
//{
//  return _T("");
//}

// void CXTPCalendarTLV_TimeScaleParams_Month::DrawLowerScale(CDC* pDC, DATE dtStart, const CRect&
// rcRect)
//{
//
//}

CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart() const
{
	return GetParentTimeScale()->GetView()->GetPaintManager()->GetTimeLineViewTimeScalePart_Day();
}

CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart() const
{
	return GetParentTimeScale()->GetView()->GetPaintManager()->GetTimeLineViewTimeScalePart_Week();
}

CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_WorkWeek::GetPart() const
{
	return GetParentTimeScale()
		->GetView()
		->GetPaintManager()
		->GetTimeLineViewTimeScalePart_WorkWeek();
}

CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart() const
{
	return GetParentTimeScale()->GetView()->GetPaintManager()->GetTimeLineViewTimeScalePart_Month();
}

COleDateTimeSpan CXTPCalendarTLV_TimeScaleParams::GetTimePerPixel() const
{
	ASSERT(m_nScaleIntervalWidth_px > 0);
	COleDateTimeSpan spSInterval	= GetScaleInterval();
	COleDateTimeSpan spTimePerPixel = (DATE)((double)GetScaleInterval()
											 / max(m_nScaleIntervalWidth_px, 1));
	return spTimePerPixel;
}

CXTPCalendarTimeLineViewTimeScaleGrid::CXTPCalendarTimeLineViewTimeScaleGrid(
	CXTPCalendarTimeLineView* pView)
{
	m_pParentView = pView;
	m_rcRect.SetRectEmpty();
}

CXTPCalendarTimeLineViewTimeScaleGrid::~CXTPCalendarTimeLineViewTimeScaleGrid()
{
	//
}

void CXTPCalendarTimeLineViewTimeScaleGrid::AdjustLayout(CRect rcParentRect)
{
	m_rcRect = rcParentRect;
}

CRect CXTPCalendarTimeLineViewTimeScaleGrid::GetRect() const
{
	return m_rcRect;
}

void CXTPCalendarTimeLineViewTimeScaleGrid::Draw(CDC* pDC)
{
	if (!m_pParentView)
		return;

	CXTPCalendarOptions* options = m_pParentView->GetCalendarControl()->GetCalendarOptions();
	if (!options)
		return;

	if (!options->bTimeLineFocused)
		return;

	CXTPCalendarTimeLineViewTimeScale* pTimeScale = m_pParentView->GetTimeScale();
	if (!pTimeScale)
		return;

	CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
	if (!pTSParams)
		return;

	COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
	int nSInterval_px			 = pTSParams->GetScaleIntervalWidth_px();

	COleDateTime dtStart = pTimeScale->GetStartViewDate();
	COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

	double dFirstStroke_Shift = (double)(dtBegin - dtStart);
	double dTimePerPixel	  = (double)pTSParams->GetTimePerPixel();
	int nFirstStroke_Xshift   = (int)(dFirstStroke_Shift / dTimePerPixel);

	int nCount = (m_rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + XTP_DPI_X(1);
	for (int i = 0; i < nCount; i++)
	{
		COleDateTimeSpan spFirstStroke((double)spSInterval * i);
		COleDateTime dtStroke = dtBegin + spFirstStroke;

		CRect rcStroke(m_rcRect);
		rcStroke.left  = m_rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
		rcStroke.right = rcStroke.left + XTP_DPI_X(1);
		pDC->FillSolidRect(&rcStroke, m_pParentView->m_clrTimeScaleBorder);

		BOOL bWorkDay = FALSE;
		switch (dtStroke.GetDayOfWeek())
		{
			case 1: bWorkDay = (options->nWorkWeekMask & xtpCalendarDaySunday) != 0; break;
			case 2: bWorkDay = (options->nWorkWeekMask & xtpCalendarDayMonday) != 0; break;
			case 3: bWorkDay = (options->nWorkWeekMask & xtpCalendarDayTuesday) != 0; break;
			case 4: bWorkDay = (options->nWorkWeekMask & xtpCalendarDayWednesday) != 0; break;
			case 5: bWorkDay = (options->nWorkWeekMask & xtpCalendarDayThursday) != 0; break;
			case 6: bWorkDay = (options->nWorkWeekMask & xtpCalendarDayFriday) != 0; break;
			case 7: bWorkDay = (options->nWorkWeekMask & xtpCalendarDaySaturday) != 0; break;
			default: break;
		}

		rcStroke.left = rcStroke.right;
		rcStroke.right += nSInterval_px;
		if (!bWorkDay)
		{
			rcStroke.DeflateRect(XTP_DPI_X(1), 0, XTP_DPI_X(2), XTP_DPI_Y(1));
			pDC->FillSolidRect(&rcStroke, m_pParentView->m_clrWeekendBackground);
		}

		// Current day
		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		if (dtStroke.GetYear() == dtNow.GetYear() && dtStroke.GetMonth() == dtNow.GetMonth()
			&& dtStroke.GetDay() == dtNow.GetDay())
		{
			rcStroke.DeflateRect(XTP_DPI_X(1), 0, XTP_DPI_X(2), XTP_DPI_Y(1));
			pDC->FillSolidRect(&rcStroke, m_pParentView->m_clrSelectedBackground);
		}
	}
}