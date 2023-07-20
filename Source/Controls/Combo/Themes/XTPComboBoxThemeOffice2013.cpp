// XTPComboBoxThemeOffice2013.cpp: implementation of the CXTPComboBoxThemeOffice2013 class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Combo/XTPComboBox.h"
#include "Controls/Combo/XTPComboBoxTheme.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeOffice2003.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeVisualStudio.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeOffice2013::CXTPComboBoxThemeOffice2013()
{
	m_bHighlight = FALSE;
}

CXTPComboBoxThemeOffice2013::~CXTPComboBoxThemeOffice2013()
{
}

void CXTPComboBoxThemeOffice2013::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	m_clrBackground.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Back"), RGB(255, 255, 255)));
	m_clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("BackHilite"), RGB(136, 195, 255)));
	m_clrBackDisabled.SetStandardValue(m_clrBackground);
	m_clrText.SetStandardValue(XTPIniColor(_T("Controls.ComboBox"), _T("Text"), RGB(68, 68, 68)));
	m_clrTextHighlight.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("TextHilite"), RGB(0, 0, 0)));
	m_clrTextDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("TextDisabled"), RGB(152, 152, 152)));
	m_clrBorderNormal.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Border"), RGB(171, 171, 171)));
	m_clrBorderHot.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("BorderHilite"), RGB(163, 189, 227)));
	m_clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("BorderDisabled"), RGB(225, 225, 225)));
	m_clrButtonNormal.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Button"), RGB(255, 255, 255)));
	m_clrButtonHot.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ButtonHilite"), RGB(213, 225, 242)));
	m_clrButtonPressed.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ButtonPushed"), RGB(163, 189, 227)));
	m_clrButtonDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ButtonDisabled"), RGB(253, 253, 253)));
	m_clrButtonArrowNormal.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Arrow"), RGB(119, 119, 119)));
	m_clrButtonArrowHot.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ArrowHilite"), RGB(68, 68, 68)));
	m_clrButtonArrowPressed.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ArrowPushed"), RGB(68, 68, 68)));
	m_clrButtonArrowDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ArrowDisabled"), RGB(198, 198, 198)));

	m_grcButton		  = m_clrButtonNormal;
	m_grcButtonHilite = m_clrButtonHot;
	m_grcButtonPushed = m_clrButtonPressed;

	CXTPComboBoxTheme::UpdateBackgroundBrush();
}

void CXTPComboBoxThemeOffice2013::DrawButton(CDC* pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	BOOL bEnabled  = pComboBox->IsWindowEnabled();
	BOOL bDropped  = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();

	COLORREF clrBack = bEnabled ? (bDropped ? m_clrButtonPressed
											: (bSelected ? m_clrButtonHot : m_clrButtonNormal))
								: m_clrButtonDisabled;
	pDC->FillSolidRect(&rcBtn, clrBack);

	if (bSelected || bDropped)
	{
		rcBtn.right = rcBtn.left + XTP_DPI_X(1);
		pDC->FillSolidRect(&rcBtn, m_clrBorderHot);
	}
}

void CXTPComboBoxThemeOffice2013::DrawButtonTriangle(CDC* pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	BOOL bDropped  = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled  = pComboBox->IsWindowEnabled();

	CRect rcArrow = rcBtn;
	rcArrow.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
	rcArrow.OffsetRect(XTP_DPI_X(1), 0);

	COLORREF clrButtonArrow = bEnabled ? (bDropped ? m_clrButtonArrowPressed
												   : (bSelected ? m_clrButtonArrowHot
																: m_clrButtonArrowNormal))
									   : m_clrButtonArrowDisabled;
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow, clrButtonArrow);
}
