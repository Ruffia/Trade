// XTPComboBoxThemeNativeWindows10.cpp: implementation of the CXTPComboBoxThemeNativeWindows10
// class.
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

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Combo/XTPComboBox.h"
#include "Controls/Combo/XTPComboBoxTheme.h"

#include "Controls/Combo/Themes/XTPComboBoxThemeOffice2003.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeVisualStudio.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeOffice2013.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeNativeWindows10::CXTPComboBoxThemeNativeWindows10()
	: CXTPComboBoxThemeOffice2013()
{
}

CXTPComboBoxThemeNativeWindows10::~CXTPComboBoxThemeNativeWindows10()
{
}

void CXTPComboBoxThemeNativeWindows10::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	m_clrBackground.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Back"), RGB(246, 246, 246)));
	m_clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("BackHilite"), RGB(51, 153, 255)));
	m_clrBackDisabled.SetStandardValue(m_clrBackground);
	m_clrText.SetStandardValue(XTPIniColor(_T("Controls.ComboBox"), _T("Text"), RGB(30, 30, 30)));
	m_clrTextHighlight.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("TextHilite"), RGB(255, 255, 255)));
	m_clrTextDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("TextDisabled"), RGB(162, 164, 165)));
	m_clrBorderNormal.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Border"), RGB(204, 206, 219)));
	m_clrBorderHot.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("BorderHilite"), RGB(0, 122, 204)));
	m_clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("BorderDisabled"), RGB(204, 206, 219)));
	m_clrButtonNormal.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Button"), RGB(255, 255, 255)));
	m_clrButtonHot.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ButtonHilite"), RGB(201, 222, 245)));
	m_clrButtonPressed.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ButtonPushed"), RGB(0, 122, 204)));
	m_clrButtonDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ButtonDisabled"), RGB(238, 238, 242)));
	m_clrButtonArrowNormal.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("Arrow"), RGB(113, 113, 113)));
	m_clrButtonArrowHot.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ArrowHilite"), RGB(30, 30, 30)));
	m_clrButtonArrowPressed.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ArrowPushed"), RGB(255, 255, 255)));
	m_clrButtonArrowDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.ComboBox"), _T("ArrowDisabled"), RGB(204, 206, 219)));

	m_grcButton		  = m_clrButtonNormal;
	m_grcButtonHilite = m_clrButtonHot;
	m_grcButtonPushed = m_clrButtonPressed;

	CXTPComboBoxTheme::UpdateBackgroundBrush();
}

void CXTPComboBoxThemeNativeWindows10::DrawButton(CDC* pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	BOOL bEnabled  = pComboBox->IsWindowEnabled();
	BOOL bDropped  = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();

	COLORREF clrBack = !bEnabled ? m_clrButtonDisabled
								 : bDropped ? m_clrButtonPressed
											: bSelected ? m_clrButtonHot : m_clrButtonNormal;

	COLORREF clrBorder = !bEnabled ? m_clrBorderDisabled
								   : bDropped ? m_clrBorderHot
											  : bSelected ? m_clrBorderHot : m_clrBackground;

	pDC->FillSolidRect(&rcBtn, clrBack);

	if (bSelected || bDropped || !bEnabled)
	{
		rcBtn.right = rcBtn.left + 1;
		pDC->FillSolidRect(&rcBtn, clrBorder);
	}
}
