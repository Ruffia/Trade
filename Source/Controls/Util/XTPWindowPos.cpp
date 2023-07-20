// XTPWindowPos.cpp : implementation of the CXTPWindowPos class.
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
#include "Common/XTPVC80Helpers.h" // Visual Studio 2005 helper functions
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Controls/Util/XTPWindowPos.h"
#include "Controls/Util/XTPRegistryManager.h"

const TCHAR XTP_REG_SETTINGS[] = _T("Settings");
const TCHAR XTP_REG_WNDPOS[]   = _T("Window Position");

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPWindowPos
/////////////////////////////////////////////////////////////////////////////
CXTPWindowPos::CXTPWindowPos()
{
	ASSERT(sizeof(CXTPWindowPos) == sizeof(WINDOWPLACEMENT));

	memset((LPWINDOWPLACEMENT)this, 0, sizeof(WINDOWPLACEMENT));

	length  = sizeof(WINDOWPLACEMENT);
	showCmd = SW_SHOW;
}

BOOL CXTPWindowPos::SaveWindowPos(CWnd* pWnd, LPCTSTR lpszWndPos /*=NULL*/,
								  LPCTSTR lpszSection /*=NULL*/)
{
	ASSERT(pWnd); // must be valid.
	if (!pWnd)
		return FALSE;

	if (::IsWindow(pWnd->m_hWnd))
	{
		if (pWnd->GetWindowPlacement(this))
		{
			LPWINDOWPLACEMENT wndPlacement = static_cast<LPWINDOWPLACEMENT>(this);
			wndPlacement->ptMinPosition	= XTP_UNDPI_POINT(wndPlacement->ptMinPosition);
			wndPlacement->ptMaxPosition	= XTP_UNDPI_POINT(wndPlacement->ptMaxPosition);
			wndPlacement->rcNormalPosition = XTP_UNDPI_RECT(wndPlacement->rcNormalPosition);

			CString strSection;
			if (lpszSection)
				strSection = lpszSection;
			else
				strSection = XTP_REG_SETTINGS;

			CString strEntry;
			if (lpszWndPos)
				strEntry = lpszWndPos;
			else
				strEntry = XTP_REG_WNDPOS;

			// Make sure we don't pop up
			// minimized the next time
			if (showCmd != SW_SHOWMAXIMIZED)
			{
				showCmd = SW_SHOWNORMAL;
			}

			CXTPRegistryManager regManager;
			regManager.WriteProfileBinary(strSection, strEntry,
										  reinterpret_cast<LPBYTE>((LPWINDOWPLACEMENT)this),
										  sizeof(WINDOWPLACEMENT));

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPWindowPos::LoadWindowPos(CWnd* pWnd, LPCTSTR lpszWndPos /*=NULL*/,
								  LPCTSTR lpszSection /*=NULL*/)
{
	ASSERT(pWnd); // must be valid.

	if (::IsWindow(pWnd->m_hWnd))
	{
		if (LoadWindowPos(lpszWndPos, lpszSection))
		{
			LPWINDOWPLACEMENT wndPlacement = static_cast<LPWINDOWPLACEMENT>(this);
			wndPlacement->ptMinPosition	= XTP_DPI_POINT(wndPlacement->ptMinPosition);
			wndPlacement->ptMaxPosition	= XTP_DPI_POINT(wndPlacement->ptMaxPosition);
			wndPlacement->rcNormalPosition = XTP_DPI_RECT(wndPlacement->rcNormalPosition);

			pWnd->SetWindowPlacement(this);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPWindowPos::LoadWindowPos(LPCTSTR lpszWndPos /*=NULL*/, LPCTSTR lpszSection /*=NULL*/)
{
	UINT nSize	 = 0;
	LPBYTE pbtData = 0;

	CString strSection;
	if (lpszSection)
		strSection = lpszSection;
	else
		strSection = XTP_REG_SETTINGS;

	CString strEntry;
	if (lpszWndPos)
		strEntry = lpszWndPos;
	else
		strEntry = XTP_REG_WNDPOS;

	CXTPRegistryManager regManager;
	if (!regManager.GetProfileBinary(strSection, strEntry, &pbtData, &nSize))
	{
		return FALSE;
	}

	BOOL bResult = FALSE;

	if (nSize == sizeof(WINDOWPLACEMENT))
	{
		MEMCPY_S((void*)&*((LPWINDOWPLACEMENT)this), pbtData, (DWORD)sizeof(WINDOWPLACEMENT));
		bResult = TRUE;
	}
	delete[] pbtData;

	return bResult;
}
