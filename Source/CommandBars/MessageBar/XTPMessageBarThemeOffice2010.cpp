// XTPMessageBarThemeOffice2010.cpp
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

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "CommandBars/MessageBar/XTPMessageBar.h"
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2010.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPMessageBarThemeOffice2010::CXTPMessageBarThemeOffice2010(CXTPPaintManager* pPaintManager)
	: CXTPMessageBarPaintManager(pPaintManager)
{
}

void CXTPMessageBarThemeOffice2010::RefreshMetrics()
{
	m_clrText.SetStandardValue(::GetSysColor(COLOR_BTNTEXT));
	m_clrBack.SetStandardValue(::GetSysColor(COLOR_INFOBK));
	m_clrBorder.SetStandardValue(::GetSysColor(COLOR_3DSHADOW));
}

void CXTPMessageBarThemeOffice2010::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, m_clrBack);
	pDC->Draw3dRect(rcClient, m_clrBorder, m_clrBorder);
}

void CXTPMessageBarThemeOffice2010::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	CRect rc(pButton->m_rcButton);

	if (pButton->m_nID == SC_CLOSE)
	{
		CPoint pt = rc.CenterPoint();

		CXTPPenDC pen(*pDC, m_clrText);

		pDC->MoveTo(pt.x - 4, pt.y - 3);
		pDC->LineTo(pt.x + 3, pt.y + 4);
		pDC->MoveTo(pt.x - 4, pt.y - 4);
		pDC->LineTo(pt.x + 4, pt.y + 4);
		pDC->MoveTo(pt.x - 3, pt.y - 4);
		pDC->LineTo(pt.x + 4, pt.y + 3);

		pDC->MoveTo(pt.x - 4, pt.y + 2);
		pDC->LineTo(pt.x + 3, pt.y - 5);
		pDC->MoveTo(pt.x - 4, pt.y + 3);
		pDC->LineTo(pt.x + 4, pt.y - 5);
		pDC->MoveTo(pt.x - 3, pt.y + 3);
		pDC->LineTo(pt.x + 4, pt.y - 4);
	}
	else
	{
		if (pButton->m_bPressed)
		{
			pDC->FillSolidRect(rc, m_clrBack);
			m_pPaintManager->DrawRectangle(pDC, rc, pButton->m_bHot, pButton->m_bPressed, TRUE,
										   FALSE, FALSE, xtpBarTypeNormal, xtpBarTop);
		}
		else if (pButton->m_bHot)
		{
			pDC->FillSolidRect(rc, m_clrBack);
			m_pPaintManager->DrawRectangle(pDC, rc, pButton->m_bHot, FALSE, TRUE, TRUE, FALSE,
										   xtpBarTypeNormal, xtpBarTop);
		}
		else
		{
			pDC->FillSolidRect(rc, m_clrBack);
			pDC->Draw3dRect(rc, m_clrBorder, m_clrBorder);
		}
	}
}

CRect CXTPMessageBarThemeOffice2010::GetCloseButtonRect(int x, int y, int cy)
{
	UNREFERENCED_PARAMETER(y);

	CSize sz(8, 8);

	CRect rc;
	rc.top	= (cy - sz.cy) / 2;
	rc.bottom = rc.top + sz.cy;
	rc.right  = x - 11;
	rc.left   = rc.right - sz.cx;

	return rc;
}
