// XTPPopupThemeOfficeXP.cpp: implementation of the CXTPPopupThemeOfficeXP class.
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
#include "Controls/Popup/Themes/XTPPopupThemeOfficeXP.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOfficeXP

CXTPPopupThemeOfficeXP::CXTPPopupThemeOfficeXP()
{
}

CXTPPopupThemeOfficeXP::~CXTPPopupThemeOfficeXP()
{
}

void CXTPPopupThemeOfficeXP::RefreshMetrics()
{
	CXTPPopupThemeOffice2000::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrButtonSelected = GetXtremeColor(XPCOLOR_HIGHLIGHT);
	m_clrButtonPressed  = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
	m_clrButtonSelectedBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrButtonPressedBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrFrame.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPPopupThemeOfficeXP::DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient)
{
	if (pControl->GetBackgroundBitmap() > 0)
	{
		CXTPPopupPaintManager::DrawBackground(pDC, pControl, rcClient);
		return;
	}

	XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, FALSE);
	pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	rcClient.DeflateRect(1, 1);

	CRect rcBackground(rcClient.left, rcClient.top, rcClient.right, rcClient.top + XTP_DPI_Y(22));
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0, 0, 128), RGB(24, 180, 192), TRUE);

	rcBackground = CRect(rcClient.left, rcBackground.top + XTP_DPI_Y(22), rcClient.right,
						 rcClient.bottom);
	rcBackground.DeflateRect(1, 1);
	rcBackground.right = rcBackground.left + XTP_DPI_X(30);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(0, 0, 128), RGB(0, 0, 128), FALSE);
}
