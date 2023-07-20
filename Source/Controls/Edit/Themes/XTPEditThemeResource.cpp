// XTPEditThemeResource.cpp: implementation of the CXTPEditThemeResource class.
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/Themes/XTPEditThemeDefault.h"
#include "Controls/Edit/Themes/XTPEditThemeOffice2000.h"
#include "Controls/Edit/Themes/XTPEditThemeOfficeXP.h"
#include "Controls/Edit/Themes/XTPEditThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPEditThemeResource::CXTPEditThemeResource()
{
	m_nTheme = xtpControlThemeResource;
}

CXTPEditThemeResource::~CXTPEditThemeResource()
{
}

void CXTPEditThemeResource::RefreshMetrics(CXTPEdit* pEdit)
{
	CXTPEditThemeOfficeXP::RefreshMetrics(pEdit);

	m_clrBorder.SetStandardValue(
		XTPIniColor(_T("Toolbar"), _T("ControlEditBorder"), ::GetSysColor(COLOR_3DSHADOW)));
	m_clrBorderHilite.SetStandardValue(
		XTPIniColor(_T("Toolbar"), _T("ControlEditBorder"), ::GetSysColor(COLOR_3DSHADOW)));
}

BOOL CXTPEditThemeResource::AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignemnt)
{
	if (!CXTPEditThemeOfficeXP::AdjustBuddyRect(pBuddy, rcUpDown, nAlignemnt))
	{
		if (nAlignemnt == UDS_ALIGNRIGHT)
		{
			rcUpDown.DeflateRect(0, XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		}
		else
		{
			rcUpDown.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), 0, XTP_DPI_X(2));
		}

		return TRUE;
	}

	return FALSE;
}

void CXTPEditThemeResource::DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignemnt,
											 CXTPEdit* pEdit)
{
	ASSERT_VALID(pDC);

	if (!CXTPEditThemeDefault::DrawBuddyBorders(pDC, rc, nAlignemnt, pEdit))
	{
		if (DYNAMIC_DOWNCAST(CSpinButtonCtrl, pBuddy) == NULL)
		{
			CBrush* pBrush = GetClientBrush(pDC, pEdit);

			pDC->FillRect(rc, pBrush);

			if (nAlignemnt == UDS_ALIGNRIGHT)
			{
				rc.left -= XTP_DPI_X(2);
			}
			else
			{
				rc.right += XTP_DPI_X(2);
			}

			pDC->Draw3dRect(rc, m_clrBorder.clrLight, m_clrBorder.clrDark);
		}
	}
}
