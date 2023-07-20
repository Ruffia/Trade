// XTPTabColorSetOffice2007.cpp
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetDefault.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2003.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2007.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPTabColorSetOffice2007::RefreshMetrics()
{
	CXTPTabColorSetOffice2003::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_csStateButton.clrFace.SetStandardValue(RGB(255, 192, 111));
	m_csStateButton.clrBorder.SetStandardValue(RGB(0, 0, 128));
	m_clrNavigateButtonBorder.SetStandardValue(RGB(0, 0, 128));

	if (pImages->IsValid())
	{
		m_clrTextHighlight.SetStandardValue(0);

		m_clrFrameBorder.SetStandardValue(
			pImages->GetImageColor(_T("TabManager"), _T("FrameBorder"), RGB(59, 97, 156)));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
		m_csPropertyPage.clrDarkShadow.SetStandardValue(
			pImages->GetImageColor(_T("TabManager"), _T("DarkShadow"), RGB(0, 53, 154)));
		m_csPropertyPage2003.clrDarkShadow.SetStandardValue(m_csPropertyPage.clrDarkShadow);

		m_csPropertyPage.clrShadow.SetStandardValue(
			pImages->GetImageColor(_T("TabManager"), _T("LightShadow"), RGB(117, 166, 241)));
		m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(m_csPropertyPage.clrShadow);
		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

		m_clrHeaderFace.SetStandardValue(
			pImages->GetImageColor(_T("Ribbon"), _T("RibbonFace"), RGB(196, 218, 250)));

		m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);

		m_clrButtonNormal.SetStandardValue(
			pImages->GetImageColor(_T("Window"), _T("ButtonFace"), RGB(162, 192, 246)));
		m_clrButtonSelected.SetStandardValue(
			pImages->GetImageColor(_T("Window"), _T("HighlightChecked"), RGB(255, 194, 115)));
		m_clrButtonHighlighted.SetStandardValue(
			pImages->GetImageColor(_T("Window"), _T("HighlightSelected"), RGB(255, 240, 201)));
		m_clrNavigateButtonBorder.SetStandardValue(
			pImages->GetImageColor(_T("Window"), _T("WindowFrame")));

		m_clrTextHighlight.SetStandardValue(
			pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText"), RGB(21, 66, 139)));
		m_clrTextNormal.SetStandardValue(
			pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText"), RGB(21, 66, 139)));
		m_clrTextSelected.SetStandardValue(
			pImages->GetImageColor(_T("Ribbon"), _T("TabSelectedText"), RGB(21, 66, 139)));
	}
	else
	{
		m_clrTextHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
		m_clrFrameBorder.SetStandardValue(RGB(59, 97, 156));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
		m_csPropertyPage.clrDarkShadow.SetStandardValue(RGB(0, 53, 154));
		m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(0, 53, 154));
		m_csPropertyPage.clrShadow.SetStandardValue(RGB(117, 166, 241));
		m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(117, 166, 241));
		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
		m_clrHeaderFace.SetStandardValue(RGB(196, 218, 250), RGB(252, 253, 254));
		m_clrButtonNormal.SetStandardValue(RGB(162, 192, 246));
		m_clrButtonSelected.SetStandardValue(RGB(255, 194, 115));
		m_clrButtonHighlighted.SetStandardValue(RGB(255, 240, 201));
		m_clrAutoHideFace.SetStandardValue(RGB(158, 190, 245), RGB(196, 218, 250));

		m_clrTextHighlight.SetStandardValue(RGB(21, 66, 139));
		m_clrTextNormal.SetStandardValue(RGB(21, 66, 139));
		m_clrTextSelected.SetStandardValue(RGB(21, 66, 139));
	}
	m_clrButtonText.SetStandardValue(m_clrTextNormal);

	m_bLunaTheme = TRUE;
}

void CXTPTabColorSetOffice2007::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton,
												   CRect& rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	if (!pImages->IsValid())
	{
		CXTPTabColorSetDefault::FillNavigateButton(pDC, pButton, rc);
		return;
	}

	if (pButton->GetItem() == NULL)
		pDC->SetTextColor(m_clrButtonText);

	if (pButton->IsEnabled() && (pButton->IsPressed() || pButton->IsHighlighted()))
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONS22"));
		ASSERT(pImage);
		if (!pImage)
			return;

		int nState = pButton->IsPressed() ? 1 : 0;

		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(8, 8, 8, 8), COLORREF_NULL);

		pDC->SetTextColor(0);
	}

	pButton->DrawEntry(pDC, rc, m_clrActiveFilesGlyph, pButton->IsEnabled());
}
