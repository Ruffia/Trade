// XTPEditTheme.cpp
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/XTPEdit.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditTheme::CXTPEditTheme()
	: m_bFlatStyle(FALSE)
	, m_bUseWinTheme(FALSE)
	, m_nTheme(xtpControlThemeCustom)
{
}

CXTPEditTheme::~CXTPEditTheme()
{
}

void CXTPEditTheme::RefreshMetrics(CXTPEdit* pEdit)
{
	UNREFERENCED_PARAMETER(pEdit);

	m_winTheme.OpenThemeData(NULL, L"EDIT");
}

void CXTPEditTheme::FillRect(CDC* pDC, int x, int y, int cx, int cy, CBrush* pBrush)
{
	ASSERT_VALID(pDC);
	pDC->FillRect(CRect(x, y, x + cx, y + cy), pBrush);
}

void CXTPEditTheme::DrawFrame(CDC* pDC, CRect rc, int nSize, CBrush* pBrush)
{
	FillRect(pDC, rc.left, rc.top, rc.right - rc.left - nSize, nSize, pBrush);
	FillRect(pDC, rc.left, rc.top, nSize, rc.bottom - rc.top - nSize, pBrush);
	FillRect(pDC, rc.right - nSize, rc.top, nSize, rc.bottom - rc.top, pBrush);
	FillRect(pDC, rc.left, rc.bottom - nSize, rc.right - rc.left, nSize, pBrush);
}

CBrush* CXTPEditTheme::GetClientBrush(CDC* pDC, CXTPEdit* pEdit)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pEdit);

	if (!::IsWindow(pEdit->GetSafeHwnd()))
	{
		return NULL;
	}

	BOOL bEnabled = pEdit->IsWindowEnabled();

	CWnd* pParentWnd = pEdit->GetParent();

	if (!pParentWnd)
	{
		return NULL;
	}

	HBRUSH hBrush = (HBRUSH)pParentWnd->SendMessage(UINT(!bEnabled ? WM_CTLCOLORSTATIC
																   : WM_CTLCOLOREDIT),
													(WPARAM)pDC->GetSafeHdc(),
													(LPARAM)pEdit->GetSafeHwnd());

	if (hBrush)
	{
		return CBrush::FromHandle(hBrush);
	}

	return CBrush::FromHandle(::GetSysColorBrush(!bEnabled ? COLOR_3DFACE : COLOR_WINDOW));
}

BOOL CXTPEditTheme::AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignemnt)
{
	UNREFERENCED_PARAMETER(pBuddy);

	if (m_bUseWinTheme && m_winTheme.IsAppThemed())
	{
		if (nAlignemnt == UDS_ALIGNRIGHT)
		{
			rcUpDown.DeflateRect(0, 1, 1, 1);
		}
		else
		{
			rcUpDown.DeflateRect(1, 1, 0, 1);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPEditTheme::RedrawFocusedFrame() const
{
	return FALSE;
}
