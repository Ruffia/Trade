// XTPMessageBarPaintManager.cpp
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

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPMessageBarPaintManager, CObject);

CXTPMessageBarPaintManager::CXTPMessageBarPaintManager(CXTPPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	ASSERT(NULL != pPaintManager);
}

void CXTPMessageBarPaintManager::RefreshMetrics()
{
	m_clrText	 = GetXtremeColor(COLOR_BTNTEXT);
	m_clrBack	 = GetXtremeColor(COLOR_3DFACE);
	m_clrBorder   = GetXtremeColor(COLOR_3DSHADOW);
	m_clrInfoBack = GetXtremeColor(COLOR_INFOBK);
}

void CXTPMessageBarPaintManager::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, m_clrBack);

	CRect rcMessage = pBar->GetMessageRect();
	pDC->Draw3dRect(rcMessage, m_clrBorder, m_clrBorder);

	rcMessage.DeflateRect(1, 1);
	pDC->FillSolidRect(rcMessage, m_clrInfoBack);
}

void CXTPMessageBarPaintManager::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	ASSERT_VALID(pDC);
	ASSERT(NULL != pButton);

	BOOL bCloseButton = (pButton->m_nID == SC_CLOSE);
	CRect rc(pButton->m_rcButton);

	if (pButton->m_bPressed)
	{
		pDC->FillSolidRect(rc, m_clrBack);
		m_pPaintManager->DrawRectangle(pDC, rc, pButton->m_bHot, pButton->m_bPressed, TRUE, FALSE,
									   FALSE, xtpBarTypeNormal, xtpBarTop);
	}
	else if (pButton->m_bHot)
	{
		pDC->FillSolidRect(rc, m_clrBack);
		m_pPaintManager->DrawRectangle(pDC, rc, pButton->m_bHot, FALSE, TRUE, FALSE, FALSE,
									   xtpBarTypeNormal, xtpBarTop);
	}
	else if (!bCloseButton)
	{
		pDC->FillSolidRect(rc, m_clrBack);
		pDC->Draw3dRect(rc, m_clrBorder, m_clrBorder);
	}

	if (bCloseButton)
	{
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolClose, rc);
	}
}

CRect CXTPMessageBarPaintManager::GetCloseButtonRect(int x, int /*y*/, int cy)
{
	CSize sz(XTP_DPI_X(18), XTP_DPI_Y(18));
	return CRect(CPoint(x - sz.cx, (cy - sz.cy) / 2), sz);
}

CSize CXTPMessageBarPaintManager::GetDefaultButtonSize() const
{
	return CSize(XTP_DPI_X(89), GetHeight() - XTP_DPI_Y(8));
}

int CXTPMessageBarPaintManager::GetHeight() const
{
	return ::GetSystemMetrics(SM_CYCAPTION) + XTP_DPI_Y(8);
}

CRect CXTPMessageBarPaintManager::GetPadding() const
{
	return CRect(XTP_DPI_X(5), 0, XTP_DPI_X(5), 0);
}
