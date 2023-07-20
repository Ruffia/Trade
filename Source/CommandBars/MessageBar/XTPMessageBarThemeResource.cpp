// XTPMessageBarThemeResource.cpp
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

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"
#include "CommandBars/Themes/XTPCommandBarsResourceTheme.h"

#include "CommandBars/MessageBar/XTPMessageBar.h"
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPMessageBarThemeResource::CXTPMessageBarThemeResource(CXTPResourceTheme* pTheme)
	: CXTPMessageBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPMessageBarThemeResource::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, m_pTheme->m_clrMessageBarFace);

	CRect rcMessage = pBar->GetMessageRect();
	pDC->Draw3dRect(rcMessage, m_pTheme->m_clrMessageBarFrame, m_pTheme->m_clrMessageBarFrame);

	rcMessage.DeflateRect(1, 1);
	XTPDrawHelpers()->GradientFill(pDC, rcMessage, m_pTheme->m_clrMessageBar, FALSE);
}

void CXTPMessageBarThemeResource::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	BOOL bCloseButton = (pButton->m_nID == SC_CLOSE);
	CRect rc(pButton->m_rcButton);

	if (pButton->m_bPressed && bCloseButton && XTPResourceImages()->IsValid())
	{
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED_BORDER),
						GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED_BORDER));
	}
	else if (pButton->m_bHot && XTPResourceImages()->IsValid())
	{
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT));
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
						GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	}
	else if (!bCloseButton)
	{
		pDC->FillSolidRect(rc, 0xFFFFFF);
		pDC->Draw3dRect(rc, m_pTheme->m_clrMessageBarFrame, m_pTheme->m_clrMessageBarFrame);
	}

	if (bCloseButton)
	{
		if (!XTPResourceImages()->IsValid())
			return;

		CXTPResourceImage* pImage = m_pTheme->LoadImage(_T("FRAMECAPTIONCLOSE17"));

		CRect rcSrc(pImage->GetSource(0, 5));

		CRect rcGlyph(CPoint((rc.right + rc.left - XTP_DPI_X(rcSrc.Width())) / 2,
							 (rc.top + rc.bottom - XTP_DPI_Y(rcSrc.Height())) / 2),
					  XTP_DPI(rcSrc.Size()));

		pImage->DrawImage(pDC, rcGlyph, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
	}
}

CSize CXTPMessageBarThemeResource::GetDefaultButtonSize() const
{
	CSize size = m_pTheme->m_szMessageBarDefaultButtonSize;
	if (!(0 < size.cx && 0 < size.cy))
	{
		size = CXTPMessageBarPaintManager::GetDefaultButtonSize();
	}

	return size;
}

int CXTPMessageBarThemeResource::GetHeight() const
{
	int nHeight = m_pTheme->m_nMessageBarHeight;
	if (!(0 < nHeight))
	{
		nHeight = CXTPMessageBarPaintManager::GetHeight();
	}

	return nHeight;
}
