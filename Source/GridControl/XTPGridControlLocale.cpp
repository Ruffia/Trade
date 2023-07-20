// XTPGridControlLocale.cpp : implementation of the CXTPGridControlLocale class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
BOOL CXTPGridControlLocale::s_bUseResourceFileLocale = FALSE;

//===========================================================================
BOOL CXTPGridControlLocale::IsUseResourceFileLocale()
{
	return s_bUseResourceFileLocale;
}

void CXTPGridControlLocale::SetUseResourceFileLocale(BOOL bUseResourceFileLocale)
{
	s_bUseResourceFileLocale = bUseResourceFileLocale;
}

LCID CXTPGridControlLocale::GetActiveLCID()
{
	LCID lcidCurr = LOCALE_USER_DEFAULT;

	if (s_bUseResourceFileLocale)
		lcidCurr = MAKELCID(XTPResourceManager()->GetResourcesLangID(), SORT_DEFAULT);

	return lcidCurr;
}

BOOL AFX_CDECL CXTPGridControlLocale::VariantChangeTypeEx(VARIANT& rVarValue, VARTYPE vartype,
														  BOOL bThrowError)
{
	if (vartype != rVarValue.vt)
	{
		// if (rVarValue.vt == VT_BSTR)
		//{
		//  vartype = VT_BSTR;
		//  return TRUE;
		//}

		LCID lcID  = GetActiveLCID();
		HRESULT hr = ::VariantChangeTypeEx(&rVarValue, &rVarValue, lcID, 0, vartype);

		if (bThrowError && FAILED(hr))
		{
			if (hr == E_OUTOFMEMORY)
				AfxThrowMemoryException();
			else
				AfxThrowOleException(hr);
		}

		return SUCCEEDED(hr);
	}
	return TRUE;
}

CString AFX_CDECL CXTPGridControlLocale::FormatDateTime(const COleDateTime& dt,
														LPCTSTR lpcszFormatString)
{
	return _FormatDateTime(dt, lpcszFormatString, GetActiveLCID());
}

CString CXTPGridControlLocale::_FormatDateTime(const COleDateTime& dt, LPCTSTR lpcszFormatString,
											   LCID lcLocaleID)
{
	if (dt.GetStatus() != COleDateTime::valid)
	{
		ASSERT(dt.GetStatus() == COleDateTime::null);
		return _T("");
	}

	CString strDT = lpcszFormatString;

	SYSTEMTIME sysTime;
	ZeroMemory(&sysTime, sizeof(sysTime));

	if (!dt.GetAsSystemTime(sysTime))
	{
		ASSERT(FALSE);
		return _T("");
	}

	// Mask (%%) percent sign placeholder
	strDT.Replace(_T("%%"), _T("\x01"));

	_ProcessMappedSpecs(strDT, &sysTime, lcLocaleID);
	_ProcessDateTimeSpecs(strDT, &sysTime, lcLocaleID);

	// All locale dependent specifiers already processed
	_ProcessOtherSpecs(strDT, dt);

	// Unmask (%%) percent sign placeholder and replace it with %.
	strDT.Replace(_T("\x01"), _T("%"));

	return strDT;
}

void CXTPGridControlLocale::_ProcessMappedSpecs(CString& rstrFormat, const SYSTEMTIME* pST,
												LCID lcLocaleID)
{
	_InitMappedSpecs();

	const int cnBufferSize = 96;
	TCHAR szBuffer[cnBufferSize];

	int nCount = (int)s_arMappedSpecs.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		const XTP_TIMESPEC& specI = s_arMappedSpecs.ElementAt(i);
		if (rstrFormat.Find(specI.pcszSpec, 0) < 0)
			continue;

		::ZeroMemory(szBuffer, sizeof(szBuffer));

		int nResult;
		if (specI.bTime)
		{
			nResult = ::GetTimeFormat(lcLocaleID, 0, pST, specI.pcszFormat, szBuffer, cnBufferSize);
		}
		else
		{
			nResult = ::GetDateFormat(lcLocaleID, 0, pST, specI.pcszFormat, szBuffer, cnBufferSize);
		}
		ASSERT(nResult);
		if (nResult)
		{
			rstrFormat.Replace(specI.pcszSpec, szBuffer);
		}
	}
}

void CXTPGridControlLocale::_ProcessDateTimeSpecs(CString& rstrFormat, const SYSTEMTIME* pST,
												  LCID lcLocaleID)
{
	//  %c  Date and time representation appropriate for locale
	//  %#c Long date and time representation

	//  %x Date representation for current locale
	//  %#x Long date representation for current locale

	//  %X Time representation for current locale

	rstrFormat.Replace(_T("%c"), _T("%x %X"));
	rstrFormat.Replace(_T("%#c"), _T("%#x %X"));

	__ProcessDate_x(rstrFormat, pST, lcLocaleID);
	__ProcessTime_X(rstrFormat, pST, lcLocaleID);
}

void CXTPGridControlLocale::__ProcessDate_x(CString& rstrFormat, const SYSTEMTIME* pST,
											LCID lcLocaleID)
{
	const int cnBufferSize = 96;
	TCHAR szBuffer[cnBufferSize];

	//  %x Date representation for current locale
	//  %#x Long date representation for current locale

	const TCHAR* arSpec2[2] = { _T("%x"), _T("%#x") };

	for (int i = 0; i < 2; i++)
	{
		if (rstrFormat.Find(arSpec2[i], 0) >= 0)
		{
			::ZeroMemory(szBuffer, sizeof(szBuffer));

			DWORD dwFlags = DWORD((i == 0) ? DATE_SHORTDATE : DATE_LONGDATE);

			int nRes = ::GetDateFormat(lcLocaleID, dwFlags, pST, NULL, szBuffer, cnBufferSize);
			ASSERT(nRes);
			UNUSED(nRes);

			rstrFormat.Replace(arSpec2[i], szBuffer);
		}
	}
}

void CXTPGridControlLocale::__ProcessTime_X(CString& rstrFormat, const SYSTEMTIME* pST,
											LCID lcLocaleID)
{
	if (rstrFormat.Find(_T("%X"), 0) >= 0)
	{
		const int cnBufferSize = 96;
		TCHAR szBuffer[cnBufferSize];

		::ZeroMemory(szBuffer, sizeof(szBuffer));
		int nRes = ::GetTimeFormat(lcLocaleID, 0, pST, NULL, szBuffer, cnBufferSize);
		ASSERT(nRes);
		UNUSED(nRes);

		rstrFormat.Replace(_T("%X"), szBuffer);
	}
}

void CXTPGridControlLocale::_ProcessOtherSpecs(CString& rstrFormat, const COleDateTime& dt)
{
	//  %j Day of year as decimal number (001 - 366)
	//  %w Weekday as decimal number (0 - 6; Sunday is 0)
	//  %U Week of year as decimal number, with Sunday as first day of week (00 - 53)
	//  %W Week of year as decimal number, with Monday as first day of week (00 - 53)
	//  %z, %Z  Either the time-zone name or time zone abbreviation, depending on registry settings;
	//  no characters if time zone is unknown

	static LPCTSTR arszSpecs[] = { _T("%j"),  _T("%#j"), _T("%w"),  _T("%#w"), _T("%U"),
								   _T("%#U"), _T("%W"),  _T("%#W"), _T("%z"),  _T("%Z") };

	int nCount = _countof(arszSpecs);
	for (int i = 0; i < nCount; i++)
	{
		if (rstrFormat.Find(arszSpecs[i], 0) < 0)
			continue;

		CString str = dt.Format(arszSpecs[i]);

		rstrFormat.Replace(arszSpecs[i], str);
	}
}

void CXTPGridControlLocale::_InitMappedSpecs()
{
	if (s_arMappedSpecs.GetSize())
		return;

	// date
	_AddsMappedSpec(_T("%a"), _T("ddd"), FALSE);
	_AddsMappedSpec(_T("%A"), _T("dddd"), FALSE);

	_AddsMappedSpec(_T("%b"), _T("MMM"), FALSE);
	_AddsMappedSpec(_T("%B"), _T("MMMM"), FALSE);

	_AddsMappedSpec(_T("%d"), _T("dd"), FALSE);
	_AddsMappedSpec(_T("%#d"), _T("d"), FALSE);

	_AddsMappedSpec(_T("%m"), _T("MM"), FALSE);
	_AddsMappedSpec(_T("%#m"), _T("M"), FALSE);

	_AddsMappedSpec(_T("%y"), _T("yy"), FALSE);
	_AddsMappedSpec(_T("%#y"), _T("y"), FALSE);
	_AddsMappedSpec(_T("%Y"), _T("yyyy"), FALSE);

	// time
	_AddsMappedSpec(_T("%H"), _T("HH"), TRUE);
	_AddsMappedSpec(_T("%#H"), _T("H"), TRUE);

	_AddsMappedSpec(_T("%I"), _T("hh"), TRUE);
	_AddsMappedSpec(_T("%#I"), _T("h"), TRUE);

	_AddsMappedSpec(_T("%M"), _T("mm"), TRUE);
	_AddsMappedSpec(_T("%#M"), _T("m"), TRUE);

	_AddsMappedSpec(_T("%S"), _T("ss"), TRUE);
	_AddsMappedSpec(_T("%#S"), _T("s"), TRUE);

	_AddsMappedSpec(_T("%p"), _T("tt"), TRUE);
}

void CXTPGridControlLocale::_AddsMappedSpec(LPCTSTR pcszSpec, LPCTSTR pcszFormat, BOOL bTime)
{
	XTP_TIMESPEC tmpSpec = { pcszSpec, pcszFormat, bTime };
	s_arMappedSpecs.Add(tmpSpec);
}
