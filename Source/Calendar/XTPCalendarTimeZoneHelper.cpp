// XTPCalendarTimeZoneHelper.cpp: implementation of Time Zone Helper classes.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarTimeZoneHelper.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarTimeZone, CXTPCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarTimeZones, CXTPCmdTarget)

#ifdef _XTP_ACTIVEX

IMPLEMENT_DYNAMIC(CXTPCalendarSystemTime, CXTPCmdTarget)

// class CXTPCalendarSystemTime : public CXTPCmdTarget, public SYSTEMTIME
CXTPCalendarSystemTime::CXTPCalendarSystemTime(const SYSTEMTIME* pTime)
{
	if (pTime)
	{
		*((SYSTEMTIME*)this) = *pTime;
	}
	else
	{
		::ZeroMemory((void*)((SYSTEMTIME*)this), sizeof(SYSTEMTIME));
	}

	EnableAutomation();
	EnableTypeLib();
}

CXTPCalendarSystemTime::~CXTPCalendarSystemTime()
{
}

BEGIN_DISPATCH_MAP(CXTPCalendarSystemTime, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wYear", 1, wYear, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wMonth", 2, wMonth, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wDayOfWeek", 3, wDayOfWeek, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wDay", 4, wDay, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wHour", 5, wHour, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wMinute", 6, wMinute, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wSecond", 7, wSecond, VT_I2)
	DISP_PROPERTY_ID(CXTPCalendarSystemTime, "wMilliseconds", 8, wMilliseconds, VT_I2)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarSystemTime, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarSystemTime, XTPDIID_CalendarSystemTime, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarSystemTime, XTPDIID_CalendarSystemTime)

#endif

CXTPCalendarTimeZone::CXTPCalendarTimeZone(const TIME_ZONE_INFORMATION* pTZInfo)
{
	if (pTZInfo)
	{
		*((TIME_ZONE_INFORMATION*)this) = *pTZInfo;
	}
	else
	{
		::ZeroMemory((void*)((TIME_ZONE_INFORMATION*)this), sizeof(TIME_ZONE_INFORMATION));
	}

	m_dwIndex = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarTimeZone::CXTPCalendarTimeZone(const CXTPCalendarTimeZone* pTZInfoEx)
{
	*this = *pTZInfoEx;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarTimeZone::~CXTPCalendarTimeZone()
{
}

const CXTPCalendarTimeZone& CXTPCalendarTimeZone::operator=(const CXTPCalendarTimeZone& rTZInfo)
{
	*((TIME_ZONE_INFORMATION*)this) = *((TIME_ZONE_INFORMATION*)&rTZInfo);

	m_strDisplayString = rTZInfo.GetDisplayString();
	m_dwIndex		   = rTZInfo.GetIndex();

	return *this;
}

CString CXTPCalendarTimeZone::GetDisplayString() const
{
	return m_strDisplayString;
}

DWORD CXTPCalendarTimeZone::GetIndex() const
{
	return m_dwIndex;
}

BOOL CXTPCalendarTimeZone::IsEqual(const TIME_ZONE_INFORMATION* pTZI2) const
{
	if (!pTZI2)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	const int cnChSize_name = _countof(StandardName);

	if (_wcsnicmp(StandardName, pTZI2->StandardName, cnChSize_name) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

CXTPCalendarTimeZonePtr CXTPCalendarTimeZone::GetTimeZoneInfo(const TIME_ZONE_INFORMATION* pTZIdata)
{
	if (!pTZIdata)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarTimeZones arXTP_TZI;

	if (!arXTP_TZI.InitFromRegistry())
	{
		return NULL;
	}

	CXTPCalendarTimeZone* pTZI2Info = arXTP_TZI.Find(pTZIdata);
	if (pTZI2Info)
	{
		CXTPCalendarTimeZone* pTZI2;
		pTZI2 = new CXTPCalendarTimeZone(pTZI2Info);
		// to update "biases" and names values
		if (pTZI2)
		{
			*((TIME_ZONE_INFORMATION*)pTZI2) = *pTZIdata;
		}
		CXTPCalendarTimeZonePtr ptrTZI2(pTZI2, FALSE);
		return ptrTZI2;
	}

	return NULL;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarTimeZone, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPCalendarTimeZone, "Bias", 1, Bias, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeZone, "StandardName", 2, OleGetStandardName,
						OleSetStandardName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeZone, "StandardDate", 3, OleGetStandardDate,
						OleSetStandardDate, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPCalendarTimeZone, "StandardBias", 4, StandardBias, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeZone, "DaylightName", 5, OleGetDaylightName,
						OleSetDaylightName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPCalendarTimeZone, "DaylightDate", 6, OleGetDaylightDate,
						OleSetDaylightDate, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPCalendarTimeZone, "DaylightBias", 7, DaylightBias, VT_I4)

	DISP_FUNCTION_ID(CXTPCalendarTimeZone, "DisplayString", 100, OleGetDisplayString, VT_BSTR,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarTimeZone, "IsEqual", 101, OleIsEqual, VT_BOOL, VTS_DISPATCH)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarTimeZone, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarTimeZone, XTPDIID_CalendarTimeZone, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarTimeZone, XTPDIID_CalendarTimeZone)

BSTR CXTPCalendarTimeZone::OleGetStandardName()
{
	CString strName = StandardName;
	return strName.AllocSysString();
}

void CXTPCalendarTimeZone::OleSetStandardName(LPCTSTR lpszStandardName)
{
	MBSTOWCS_S(StandardName, lpszStandardName, 32);
}

BSTR CXTPCalendarTimeZone::OleGetDaylightName()
{
	CString strName = DaylightName;
	return strName.AllocSysString();
}

void CXTPCalendarTimeZone::OleSetDaylightName(LPCTSTR lpszDaylightName)
{
	MBSTOWCS_S(DaylightName, lpszDaylightName, 32);
}

LPDISPATCH CXTPCalendarTimeZone::OleGetStandardDate()
{
	CXTPCalendarSystemTime* pStandardDate = new CXTPCalendarSystemTime(&StandardDate);
	if (!pStandardDate)
	{
		AfxThrowOleException(E_OUTOFMEMORY);
	}
	return pStandardDate->GetIDispatch(FALSE);
}

void CXTPCalendarTimeZone::OleSetStandardDate(LPDISPATCH pDispStandardDate)
{
	CXTPCalendarSystemTime* pStandardDate = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPCalendarSystemTime, CXTPCalendarSystemTime::FromIDispatchSafe(pDispStandardDate));
	if (!pStandardDate)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	if (pStandardDate)
	{
		StandardDate = *((SYSTEMTIME*)pStandardDate);
	}
}

LPDISPATCH CXTPCalendarTimeZone::OleGetDaylightDate()
{
	CXTPCalendarSystemTime* pDaylightDate = new CXTPCalendarSystemTime(&DaylightDate);
	if (!pDaylightDate)
	{
		AfxThrowOleException(E_OUTOFMEMORY);
	}
	return pDaylightDate->GetIDispatch(FALSE);
}

void CXTPCalendarTimeZone::OleSetDaylightDate(LPDISPATCH pDispDaylightDate)
{
	CXTPCalendarSystemTime* pDaylightDate = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPCalendarSystemTime, CXTPCalendarSystemTime::FromIDispatchSafe(pDispDaylightDate));
	if (!pDaylightDate)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	if (pDaylightDate)
	{
		DaylightDate = *((SYSTEMTIME*)pDaylightDate);
	}
}

BSTR CXTPCalendarTimeZone::OleGetDisplayString()
{
	return m_strDisplayString.AllocSysString();
}

BOOL CXTPCalendarTimeZone::OleIsEqual(LPDISPATCH pDispTZI2)
{
	CXTPCalendarTimeZone* pTZI2;
	pTZI2 = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPCalendarTimeZone,
										 CXTPCalendarTimeZone::FromIDispatchSafe(pDispTZI2));
	if (!pTZI2)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	if (IsEqual(pTZI2))
	{
		return TRUE;
	}

	return FALSE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// class CXTPCalendarTimeZones : public CXTPCmdTarget
CXTPCalendarTimeZones::CXTPCalendarTimeZones()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarTimeZones::~CXTPCalendarTimeZones()
{
}

BOOL AFX_CDECL CXTPCalendarTimeZones::GetRegBSTR(HKEY hKey, LPCWSTR pcszValueNameW,
												 BSTR& rbstrValue)
{
	if (rbstrValue)
	{
		::SysFreeString(rbstrValue);
		rbstrValue = NULL;
	}

	//------------------------------------------------------------
	if (CXTPCalendarUtils::IsWin9x())
	{
		CString strValueName(pcszValueNameW);
		CString strValue;
		if (GetRegStr(hKey, strValueName, strValue))
		{
			rbstrValue = strValue.AllocSysString();
		}
		return rbstrValue != NULL;
	}

	//------------------------------------------------------------
	const DWORD cdwDataSize = 4 * 1024;
	BYTE arData[cdwDataSize];
	::ZeroMemory(arData, sizeof(arData));
	DWORD dwType = 0, dwDataSize = sizeof(arData) - 2;

	LONG lRes = ::RegQueryValueExW(hKey, pcszValueNameW, NULL, &dwType, arData, &dwDataSize);

	if (lRes != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (dwType == REG_SZ || dwType == REG_EXPAND_SZ || dwType == REG_MULTI_SZ)
	{
		ASSERT(dwType != REG_MULTI_SZ); // WARMING!

		rbstrValue = ::SysAllocString((LPCWSTR)arData);
		return rbstrValue != NULL;
	}

	return FALSE;
}

BOOL AFX_CDECL CXTPCalendarTimeZones::GetRegStr(HKEY hKey, LPCTSTR pcszValueName,
												CString& rstrValue)
{
	rstrValue.Empty();

	const DWORD cdwDataSize = 4 * 1024;
	BYTE arData[cdwDataSize];
	::ZeroMemory(arData, sizeof(arData));
	DWORD dwType = 0, dwDataSize = sizeof(arData) - 2;

	LONG lRes = ::RegQueryValueEx(hKey, pcszValueName, NULL, &dwType, arData, &dwDataSize);

	if (lRes != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (dwType == REG_SZ || dwType == REG_EXPAND_SZ || dwType == REG_MULTI_SZ)
	{
		ASSERT(dwType != REG_MULTI_SZ); // WARMING!

		rstrValue = (LPCTSTR)arData;
		return TRUE;
	}

	return FALSE;
}

BOOL AFX_CDECL CXTPCalendarTimeZones::GetRegDWORD(HKEY hKey, LPCTSTR pcszValueName, DWORD& rdwValue)
{
	rdwValue = 0;

	const DWORD cdwDataSize = 16;
	BYTE arData[cdwDataSize];
	::ZeroMemory(arData, sizeof(arData));
	DWORD dwType = 0, dwDataSize = sizeof(arData);

	LONG lRes = ::RegQueryValueEx(hKey, pcszValueName, NULL, &dwType, arData, &dwDataSize);

	if (lRes != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (dwType == REG_DWORD || dwType == REG_DWORD_BIG_ENDIAN || dwType == REG_BINARY)
	{
		rdwValue = *((DWORD*)arData);
		return TRUE;
	}

	return FALSE;
}

BOOL AFX_CDECL CXTPCalendarTimeZones::GetRegTZI(
	HKEY hKey, LPCTSTR pcszValueName, CXTPCalendarTimeZones::REGISTRY_TIMEZONE_INFORMATION& rRegTZI)
{
	::ZeroMemory(&rRegTZI, sizeof(rRegTZI));

	const DWORD cdwDataSize = sizeof(rRegTZI) * 2;
	BYTE arData[cdwDataSize];
	::ZeroMemory(arData, sizeof(arData));
	DWORD dwType = 0, dwDataSize = sizeof(arData);

	LONG lRes = ::RegQueryValueEx(hKey, pcszValueName, NULL, &dwType, arData, &dwDataSize);

	if (lRes != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (dwType == REG_BINARY && dwDataSize == sizeof(rRegTZI))
	{
		rRegTZI = *((REGISTRY_TIMEZONE_INFORMATION*)arData);
		return TRUE;
	}

	ASSERT(FALSE);
	return FALSE;
}

BOOL CXTPCalendarTimeZones::InitFromRegistry()
{
	m_arTZInfo.RemoveAll();

	CString strTZIRootKey = XTP_CALENDAR_TIMEZONESKEY_NT;

	HKEY hkTZIRoot = NULL;
	BOOL bUseIndex = !XTPSystemVersion()->IsWinXPOrGreater();

	// Try NT information first
	LONG lRes = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTZIRootKey, 0, KEY_READ, &hkTZIRoot);
	if (lRes != ERROR_SUCCESS)
	{
		// try Win9x information
		strTZIRootKey = XTP_CALENDAR_TIMEZONESKEY_9X;
		bUseIndex	 = FALSE;

		lRes = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTZIRootKey, 0, KEY_READ, &hkTZIRoot);
		if (lRes != ERROR_SUCCESS)
		{
			TRACE(_T("Cannot open Time Zones information registry key. err = %d \n"), lRes);
			return FALSE;
		}
	}

	const DWORD cdwKeyBufSize = 1024;
	TCHAR szTZIKey[cdwKeyBufSize + 1];

	BSTR bstrStandardName = NULL;
	BSTR bstrDaylightName = NULL;

	lRes = ERROR_SUCCESS;
	for (DWORD dwEnumKey = 0; lRes == ERROR_SUCCESS; dwEnumKey++)
	{
		::ZeroMemory(szTZIKey, sizeof(szTZIKey));

		lRes = ::RegEnumKey(hkTZIRoot, dwEnumKey, szTZIKey, cdwKeyBufSize);

		if (lRes != ERROR_SUCCESS)
		{
			ASSERT(lRes == ERROR_NO_MORE_ITEMS);
			break;
		}

		//===================================================================
		CString strTZIKey = strTZIRootKey + _T("\\") + szTZIKey;

		HKEY hkTZIdata = NULL;
		LONG lRes2	 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strTZIKey, 0, KEY_READ, &hkTZIdata);
		if (lRes2 != ERROR_SUCCESS)
		{
			TRACE(_T("Cannot open registry key 'HKEY_LOCAL_MACHINE\\%s'. err = %d \n"),
				  (LPCTSTR)strTZIKey, lRes2);
			continue;
		}

		CString strDisplayStr, strStandardName, strDaylightName;
		DWORD dwIndex;
		REGISTRY_TIMEZONE_INFORMATION regTZI;

		if (!GetRegStr(hkTZIdata, XTP_CALENDAR_TZIRegValName_DisplayStr, strDisplayStr))
		{
			TRACE(_T("Cannot get value from registry: 'HKEY_LOCAL_MACHINE\\%s\\%s'\n"),
				  (LPCTSTR)strTZIKey, XTP_CALENDAR_TZIRegValName_DisplayStr);
			continue;
		}
		if (!GetRegBSTR(hkTZIdata, XTP_CALENDAR_TZIRegValName_StandardNameW, bstrStandardName))
		{
			TRACE(_T("Cannot get value from registry: 'HKEY_LOCAL_MACHINE\\%s\\%s'\n"),
				  (LPCTSTR)strTZIKey, XTP_CALENDAR_TZIRegValName_StandardName);
			continue;
		}
		if (!GetRegBSTR(hkTZIdata, XTP_CALENDAR_TZIRegValName_DaylightNameW, bstrDaylightName))
		{
			TRACE(_T("Cannot get value from registry: 'HKEY_LOCAL_MACHINE\\%s\\%s'\n"),
				  (LPCTSTR)strTZIKey, XTP_CALENDAR_TZIRegValName_DaylightName);
			continue;
		}

		if (!GetRegTZI(hkTZIdata, XTP_CALENDAR_TZIRegValName_DATA, regTZI))
		{
			TRACE(_T("Cannot get value from registry: 'HKEY_LOCAL_MACHINE\\%s\\%s'\n"),
				  (LPCTSTR)strTZIKey, XTP_CALENDAR_TZIRegValName_DATA);
			continue;
		}

		if (!GetRegDWORD(hkTZIdata, XTP_CALENDAR_TZIRegValName_Index, dwIndex))
		{
			if (bUseIndex)
			{
				TRACE(_T("Cannot get value from registry: 'HKEY_LOCAL_MACHINE\\%s\\%s'\n"),
					  (LPCTSTR)strTZIKey, XTP_CALENDAR_TZIRegValName_Index);
			}
			dwIndex = 0;
		}

		//=================================================
		CXTPCalendarTimeZone* pXTP_TZI;
		pXTP_TZI = new CXTPCalendarTimeZone();
		if (!pXTP_TZI)
		{
			AfxThrowOleException(E_OUTOFMEMORY);
		}

		pXTP_TZI->Bias = regTZI.Bias;

		WCSNCPY_S(pXTP_TZI->StandardName, 32, bstrStandardName, 32);
		pXTP_TZI->StandardName[_countof(pXTP_TZI->StandardName) - 1] = L'\0';

		pXTP_TZI->StandardDate = regTZI.StandardDate;
		pXTP_TZI->StandardBias = regTZI.StandardBias;

		WCSNCPY_S(pXTP_TZI->DaylightName, 32, bstrDaylightName, 32);
		pXTP_TZI->DaylightName[_countof(pXTP_TZI->DaylightName) - 1] = L'\0';

		pXTP_TZI->DaylightDate = regTZI.DaylightDate;
		pXTP_TZI->DaylightBias = regTZI.DaylightBias;

		pXTP_TZI->m_strDisplayString = strDisplayStr;
		pXTP_TZI->m_dwIndex			 = dwIndex;

		m_arTZInfo.Add(pXTP_TZI, FALSE);

		RegCloseKey(hkTZIdata);
	}

	RegCloseKey(hkTZIRoot);

	//-----------------------------------------------
	if (bstrStandardName)
	{
		::SysFreeString(bstrStandardName);
	}
	if (bstrDaylightName)
	{
		::SysFreeString(bstrDaylightName);
	}

	//-----------------------------------------------
	int nCount = m_arTZInfo.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		for (int j = i + 1; j < nCount; j++)
		{
			CXTPCalendarTimeZonePtr ptrElem1, ptrElem2;
			ptrElem1 = m_arTZInfo.GetAt(i, TRUE);
			ptrElem2 = m_arTZInfo.GetAt(j, TRUE);

			if (CompareTZI(ptrElem1, ptrElem2, bUseIndex) > 0)
			{
				m_arTZInfo.SetAt(i, ptrElem2.Detach());
				m_arTZInfo.SetAt(j, ptrElem1.Detach());
			}
		}
	}

	return TRUE;
}

int CXTPCalendarTimeZones::GetDigit(TCHAR ch) const
{
	int nD = ch - _T('0');
	return (nD >= 0 && nD <= 9) ? nD : -1;
}

void CXTPCalendarTimeZones::ParseDisplayStr(LPCTSTR str, int& rnOffset, CString& rstrPlace) const
{
	// (GMT + 02:00) Athens, ...
	// (GMT-02:00) place, ...
	//----------------------------------------------
	//  (GMT - 02 : 00) place, ...
	// 0 1      2    3  4 5
	//----------------------------------------------

	rnOffset = 0;
	rstrPlace.Empty();

	int nSign = 0, nHr = 0, nMin = 0;
	int nState = 0;

	for (const TCHAR* pCh = (LPCTSTR)str; pCh && *pCh != 0; pCh = _tcsinc(pCh))
	{
		if (nState == 0 && *pCh == _T('('))
		{
			nState = 1;
		}
		else if (nState == 1)
		{
			if (*pCh == _T('+'))
			{
				nSign  = 1;
				nState = 2;
			}
			else if (*pCh == _T('-'))
			{
				nSign  = -1;
				nState = 2;
			}
		}
		else if (nState == 2)
		{
			if (GetDigit(*pCh) >= 0)
			{
				nHr *= 10;
				nHr += GetDigit(*pCh);
			}
			else if (*pCh == _T(':'))
			{
				nState = 3;
			}
		}
		else if (nState == 3)
		{
			if (GetDigit(*pCh) >= 0)
			{
				nMin *= 10;
				nMin += GetDigit(*pCh);
			}
			else if (*pCh == _T(')'))
			{
				nState = 4;
			}
		}
		else if (nState == 4)
		{
			if (*pCh != _T(' '))
			{
				rstrPlace = pCh;
				break;
			}
		}
	}
	//---------------------------------------
	rnOffset = nSign * (nHr * 60 + nMin);
}

int CXTPCalendarTimeZones::CompareTZI(const CXTPCalendarTimeZone* pTZI1,
									  const CXTPCalendarTimeZone* pTZI2, BOOL bUseIndex) const
{
	if (bUseIndex)
	{
		return XTPCompare(pTZI1->m_dwIndex, pTZI2->m_dwIndex);
	}

	int nOffset1, nOffset2;
	CString strPlace1, strPlace2;

	ParseDisplayStr(pTZI1->GetDisplayString(), nOffset1, strPlace1);
	ParseDisplayStr(pTZI2->GetDisplayString(), nOffset2, strPlace2);

	int nCmpRes = XTPCompare(nOffset1, nOffset2);
	if (nCmpRes)
	{
		return nCmpRes;
	}

	return XTPCompare(strPlace1, strPlace2);
}

int CXTPCalendarTimeZones::GetCount() const
{
	return m_arTZInfo.GetCount();
}

CXTPCalendarTimeZone* CXTPCalendarTimeZones::GetAt(long nIndex) const
{
	if (nIndex < 0 || nIndex >= m_arTZInfo.GetCount())
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarTimeZone* pTZI = m_arTZInfo.GetAt(nIndex, FALSE);
	return pTZI;
}

CXTPCalendarTimeZone* CXTPCalendarTimeZones::Find(const TIME_ZONE_INFORMATION* pTZI2) const
{
	if (!pTZI2)
	{
		ASSERT(FALSE);
		return NULL;
	}

	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarTimeZone* pTZI = GetAt(i);
		if (!pTZI)
		{
			ASSERT(FALSE);
			continue;
		}
		if (pTZI->IsEqual(pTZI2))
		{
			return pTZI;
		}
	}
	return NULL;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarTimeZones, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarTimeZones, "NewEnum", DISPID_NEWENUM, OleNewEnum, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarTimeZones, "Count", 100, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarTimeZones, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarTimeZones, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarTimeZones, XTPDIID_CalendarTimeZones, Dispatch)
	// INTERFACE_PART(CXTPCalendarTimeZones, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarTimeZones, XTPDIID_CalendarTimeZones)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarTimeZones)

int CXTPCalendarTimeZones::OleGetItemCount()
{
	return m_arTZInfo.GetCount();
}

LPDISPATCH CXTPCalendarTimeZones::OleGetItem(long nIndex)
{
	if (nIndex < 0 || nIndex >= m_arTZInfo.GetCount())
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	CXTPCalendarTimeZone* pTZI = m_arTZInfo.GetAt(nIndex);
	if (pTZI)
	{
		return pTZI->GetIDispatch(TRUE);
	}
	return NULL;
}

#endif
