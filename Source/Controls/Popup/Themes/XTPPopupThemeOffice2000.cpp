// XTPPopupThemeOffice2000.cpp: implementation of the CXTPPopupThemeOffice2000 class.
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

#include "Controls/Defines.h"
#include "Controls/Popup/XTPPopupControl.h"
#include "Controls/Popup/XTPPopupPaintManager.h"
#include "Controls/Popup/Themes/XTPPopupThemeOffice2000.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOffice2000

CXTPPopupThemeOffice2000::CXTPPopupThemeOffice2000()
{
}

CXTPPopupThemeOffice2000::~CXTPPopupThemeOffice2000()
{
}

void CXTPPopupThemeOffice2000::RefreshMetrics()
{
	CXTPPopupPaintManager::RefreshMetrics();

	m_clrFrame.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPPopupThemeOffice2000::DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient)
{
	if (pControl->GetBackgroundBitmap() > 0)
	{
		CXTPPopupPaintManager::DrawBackground(pDC, pControl, rcClient);
		return;
	}

	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, FALSE);
	pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	rcClient.DeflateRect(1, 1);

	// rcClient.DeflateRect(2, 2);
	CRect rcBackground(rcClient.left, rcClient.top, rcClient.right, rcClient.top + XTP_DPI_Y(22));
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0, 0, 128), RGB(24, 180, 192), TRUE);

	rcClient.DeflateRect(1, 1);
	rcBackground = CRect(rcClient.left, rcBackground.top + XTP_DPI_Y(23), rcClient.right,
						 rcClient.bottom);
	pDC->Draw3dRect(rcBackground, m_clrFrame.clrDark, m_clrFrame.clrLight);

	rcBackground.DeflateRect(1, 1);
	pDC->Draw3dRect(rcBackground, m_clrFrame.clrLight, m_clrFrame.clrDark);

	rcBackground.DeflateRect(1, 1);
	rcBackground.right = rcBackground.left + XTP_DPI_X(30);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0, 0, 128), RGB(0, 0, 128), FALSE);
}
