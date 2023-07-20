// XTPMessageBarThemeOffice2013.cpp
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
#include "Common/XTPResourceImage.h"

#include "CommandBars/MessageBar/XTPMessageBar.h"
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPMessageBarThemeOffice2013::CXTPMessageBarThemeOffice2013(CXTPPaintManager* pPaintManager)
	: CXTPMessageBarPaintManager(pPaintManager)
	, m_rcBorders(0, 0, 0, 1)
{
}

void CXTPMessageBarThemeOffice2013::RefreshMetrics()
{
	// Default colors found in OFFICE2013WORD_INI.

	m_clrText.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("Text"), RGB(119, 119, 119)));
	m_clrTextHilite.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("TextHilite"), RGB(68, 68, 68)));
	m_clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("Back"), RGB(252, 247, 182)));
	m_clrBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("Border"), RGB(215, 216, 137)));
	m_clrButtonBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("ButtonBack"), RGB(253, 253, 253)));
	m_clrButtonBackHilite.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("ButtonBackHilite"), RGB(240, 240, 240)));
	m_clrButtonBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("ButtonBackPushed"), RGB(198, 198, 198)));
	m_clrButtonBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("ButtonBorder"), RGB(171, 171, 171)));
	m_clrButtonBorderHilite.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("ButtonBorderHilite"), RGB(198, 198, 198)));
	m_clrButtonBorderPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.MessageBar"), _T("ButtonBorderPushed"), RGB(171, 171, 171)));
}

void CXTPMessageBarThemeOffice2013::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, m_clrBack);

	if (m_rcBorders.left)
	{
		CRect rcBorder(rcClient);
		rcBorder.right = rcBorder.left + 1;
		pDC->FillSolidRect(rcBorder, m_clrBorder);
	}
	if (m_rcBorders.top)
	{
		CRect rcBorder(rcClient);
		rcBorder.bottom = rcBorder.top + 1;
		pDC->FillSolidRect(rcBorder, m_clrBorder);
	}
	if (m_rcBorders.right)
	{
		CRect rcBorder(rcClient);
		rcBorder.left = rcBorder.right - 1;
		pDC->FillSolidRect(rcBorder, m_clrBorder);
	}
	if (m_rcBorders.bottom)
	{
		CRect rcBorder(rcClient);
		rcBorder.top = rcBorder.bottom - 1;
		pDC->FillSolidRect(rcBorder, m_clrBorder);
	}
}

void CXTPMessageBarThemeOffice2013::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	CRect rc(pButton->m_rcButton);

	if (pButton->m_nID == SC_CLOSE)
	{
		CPoint pt = rc.CenterPoint();

		COLORREF clr;
		if (pButton->m_bPressed)
		{
			clr = m_clrTextHilite;
		}
		else if (pButton->m_bHot)
		{
			clr = m_clrTextHilite;
		}
		else
		{
			clr = m_clrText;
		}

		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolClose, rc, clr);
	}
	else
	{
		if (pButton->m_bPressed)
		{
			pDC->FillSolidRect(rc, m_clrButtonBackPushed);
			pDC->Draw3dRect(rc, m_clrButtonBorderPushed, m_clrButtonBorderPushed);
		}
		else if (pButton->m_bHot)
		{
			pDC->FillSolidRect(rc, m_clrButtonBackHilite);
			pDC->Draw3dRect(rc, m_clrButtonBorderHilite, m_clrButtonBorderHilite);
		}
		else
		{
			pDC->FillSolidRect(rc, m_clrButtonBack);
			pDC->Draw3dRect(rc, m_clrButtonBorder, m_clrButtonBorder);
		}
	}
}

CRect CXTPMessageBarThemeOffice2013::GetCloseButtonRect(int x, int y, int cy)
{
	UNREFERENCED_PARAMETER(y);

	CXTPEmptyRect rc;

	CSize sz(XTP_DPI_X(8), XTP_DPI_Y(8));

	rc.top	= (cy - sz.cy) / 2;
	rc.bottom = rc.top + sz.cy;
	rc.right  = x - XTP_DPI_X(11);
	rc.left   = rc.right - sz.cx;

	return rc;
}
