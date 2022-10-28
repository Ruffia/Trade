//******************************************************************************//
//
//  Helper file for the sample application illustrating the usage of 
//  the CSQLiteDatabase and CSQLiteRecordset classes, used to access SQLite 
//  databases from MFC applications
//
//  Designed for the Oxetta report builder, the free report design and print
//  library for C/C++ developers
//
//  http://www.oxetta.com for more details
//
//******************************************************************************//

#include "StdAfx.h"
#include "UnicodeTools.h"

char* Utf16ToAnsi(WCHAR *wszString, int *piLength)
{
	if (wszString == NULL)
		return NULL;
	int iLength = WideCharToMultiByte(CP_ACP, 0, wszString, -1, NULL, 0, NULL, NULL);
	char* szString = (char*)malloc(iLength);
	memset(szString, 0, iLength);
	WideCharToMultiByte(CP_ACP, 0, wszString, -1, szString, iLength, NULL, NULL);
	if (NULL != piLength)
		*piLength = iLength;
	return szString;
}

WCHAR* AnsiToUtf16(char *szString, int *piLength)
{
	if (NULL == szString)
		return NULL;
	int iLength = MultiByteToWideChar(CP_ACP, 0, szString, -1, NULL, 0);
	WCHAR* wszString = (WCHAR*)malloc(sizeof(WCHAR) * iLength);
	memset(wszString, 0, sizeof(WCHAR) * iLength);
	MultiByteToWideChar(CP_ACP, 0, szString, -1, wszString, iLength);
	if (NULL != piLength)
		*piLength = sizeof(WCHAR) * iLength;
	return wszString;
}

char* Utf16ToUtf8(WCHAR *wszSource, int *piLength)
{
	if (NULL == wszSource)
		return NULL;
	int iLength = WideCharToMultiByte( CP_UTF8, 0, wszSource, -1, NULL, 0, NULL, NULL);
	char *szDest = (char*)malloc(iLength);
	memset(szDest, 0, iLength);
	WideCharToMultiByte( CP_UTF8, 0, wszSource, -1, szDest, iLength, NULL, NULL);
	if (NULL != piLength)
		*piLength = iLength;
	return szDest;
}

WCHAR* Utf8ToUtf16(char *szSource, int *piLength)
{
	if (NULL == szSource)
		return NULL;
	int iLength = MultiByteToWideChar( CP_UTF8, 0, szSource, -1, NULL, 0 );
	WCHAR *wszDest = (WCHAR*)malloc( sizeof(WCHAR)*iLength);
	memset(wszDest, 0, sizeof(WCHAR)*iLength);
	MultiByteToWideChar( CP_UTF8, 0, szSource, -1, wszDest, iLength);
	if (NULL != piLength)
		*piLength = sizeof(WCHAR)*(iLength);
	return wszDest;
}

char* AnsiToUtf8(char *szSource, int *piLength)
{
	WCHAR* wszResult = AnsiToUtf16(szSource, piLength);
	char *utf8Result = Utf16ToUtf8(wszResult, piLength);
	FREE_AND_NULL(wszResult);
	return utf8Result;
}

char* Utf8ToAnsi(char *szSource, int *piLength)
{
	WCHAR* wszResult = Utf8ToUtf16(szSource, piLength);
	char *utf8Result = Utf16ToUtf8(wszResult, piLength);
	FREE_AND_NULL(wszResult);
	return utf8Result;
}

char* ToUtf8(LPCTSTR szSource, int *piLength)
{
#ifdef _UNICODE
	return Utf16ToUtf8((TCHAR*)szSource, piLength);
#else
	return AnsiToUtf8((char*)szSource, piLength);
#endif
}

CString FromUtf8(char* szSource)
{
	LPCTSTR szResult = NULL;
#ifdef _UNICODE
	szResult = Utf8ToUtf16(szSource);
#else
	szResult = Utf8ToAnsi(szSource);
#endif
	CString sResult = szResult;
	FREE_AND_NULL(szResult);
	return sResult;
}
