// XTPCommandBarsColorSetOffice2013.cpp : implementation of the CXTPCommandBarsColorSetOffice2013
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateButton

IMPLEMENT_DYNAMIC(CXTPColorSetStateButton, CXTPCmdTarget)

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateMenu - Default colors found in OFFICE2013WORD_INI

IMPLEMENT_DYNAMIC(CXTPColorSetStateMenu, CXTPColorSetStateButton)

void CXTPColorSetStateMenu::RefreshColors()
{
	// load text state colors from INI file.
	clrText.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonText"), RGB(68, 68, 68)));
	clrTextHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonTextHighlight"), RGB(68, 68, 68)));
	clrTextPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonTextPushed"), RGB(68, 68, 68)));
	clrTextChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonTextChecked"), RGB(68, 68, 68)));
	clrTextCheckedHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonTextCheckedHighlight"), RGB(68, 68, 68)));
	clrTextDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonTextDisabled"), RGB(177, 177, 177)));

	// load background state colors from INI file.
	clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBack"), RGB(255, 255, 255)));
	clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBackHighlight"), RGB(213, 225, 242)));
	clrBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBackPushed"), RGB(163, 189, 227)));
	clrBackChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBackChecked"), RGB(194, 213, 242)));
	clrBackCheckedHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBackCheckedHighlight"), RGB(194, 213, 242)));
	clrBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBackDisabled"), RGB(238, 238, 238)));

	// load border state colors from INI file.
	clrBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBorder"), RGB(255, 255, 255)));
	clrBorderHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBorderHighlight"), RGB(213, 225, 242)));
	clrBorderPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBorderPushed"), RGB(163, 189, 227)));
	clrBorderChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBorderChecked"), RGB(194, 213, 242)));
	clrBorderCheckedHighlight.SetStandardValue(XTPIniColor(_T("CommandBars.Menu"),
														   _T("ButtonBorderCheckedHighlight"),
														   RGB(194, 213, 242)));
	clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("ButtonBorderDisabled"), RGB(192, 193, 194)));

	// additional colors.
	clrMenuBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("Back"), RGB(255, 255, 255)));
	clrMenuBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("Border"), RGB(212, 212, 212)));
	clrGripper.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("Gripper"), RGB(255, 255, 255)));

	// load separator colors from INI file.
	clrSeparator.clrDark.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("SeparatorDark"), RGB(225, 225, 225)));
	clrSeparator.clrLight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Menu"), _T("SeparatorLight"), RGB(255, 255, 255)));
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateToolbar - Default colors found in OFFICE2013WORD_INI

IMPLEMENT_DYNAMIC(CXTPColorSetStateToolbar, CXTPColorSetStateButton)

void CXTPColorSetStateToolbar::RefreshColors()
{
	// load text state colors from INI file.
	clrText.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"), RGB(68, 68, 68)));
	clrTextHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonTextHighlight"), RGB(68, 68, 68)));
	clrTextPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonTextPushed"), RGB(68, 68, 68)));
	clrTextChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonTextChecked"), RGB(68, 68, 68)));
	clrTextCheckedHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonTextCheckedHighlight"), RGB(68, 68, 68)));
	clrTextDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonTextDisabled"), RGB(177, 177, 177)));

	// load background state colors from INI file.
	clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBack"), RGB(255, 255, 255)));
	clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBackHighlight"), RGB(213, 225, 242)));
	clrBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBackPushed"), RGB(163, 189, 227)));
	clrBackChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBackChecked"), RGB(194, 213, 242)));
	clrBackCheckedHighlight.SetStandardValue(XTPIniColor(_T("CommandBars.Toolbar"),
														 _T("ButtonBackCheckedHighlight"),
														 RGB(213, 225, 242)));
	clrBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBackDisabled"), RGB(238, 238, 238)));

	// load border state colors from INI file.
	clrBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBorder"), RGB(255, 255, 255)));
	clrBorderHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBorderHighlight"), RGB(213, 225, 242)));
	clrBorderPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBorderPushed"), RGB(163, 189, 227)));
	clrBorderChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBorderChecked"), RGB(194, 213, 242)));
	clrBorderCheckedHighlight.SetStandardValue(XTPIniColor(_T("CommandBars.Toolbar"),
														   _T("ButtonBorderCheckedHighlight"),
														   RGB(163, 189, 227)));
	clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonBorderDisabled"), RGB(192, 193, 194)));

	// additional colors.
	clrToolbarBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("Back"), RGB(255, 255, 255)));
	clrToolbarBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("Border"), RGB(225, 225, 225)));
	clrEditBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("BorderEdit"), RGB(166, 172, 181)));
	clrGripper.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("Gripper"), RGB(166, 172, 181)));
	clrDisabledIcon.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("DisabledIcon"), RGB(212, 212, 212)));

	// load separator colors from INI file.
	clrSeparator.clrDark.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("SeparatorDark"), RGB(225, 225, 225)));
	clrSeparator.clrLight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Toolbar"), _T("SeparatorLight"), RGB(255, 255, 255)));
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateRibbon - Default colors found in OFFICE2013WORD_INI

IMPLEMENT_DYNAMIC(CXTPColorSetStateRibbon, CXTPColorSetStateButton)

void CXTPColorSetStateRibbon::RefreshColors()
{
	// load text state colors from INI file.
	clrText.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonText"), RGB(68, 68, 68)));
	clrTextHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonTextHighlight"), RGB(68, 68, 68)));
	clrTextPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonTextPushed"), RGB(68, 68, 68)));
	clrTextChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonTextChecked"), RGB(68, 68, 68)));
	clrTextCheckedHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonTextCheckedHighlight"), RGB(68, 68, 68)));
	clrTextDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonTextDisabled"), RGB(177, 177, 177)));

	// load background state colors from INI file.
	clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBack"), RGB(255, 255, 255)));
	clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBackHighlight"), RGB(213, 225, 242)));
	clrBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBackPushed"), RGB(163, 189, 227)));
	clrBackChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBackChecked"), RGB(194, 213, 242)));
	clrBackCheckedHighlight.SetStandardValue(XTPIniColor(_T("CommandBars.Ribbon"),
														 _T("ButtonBackCheckedHighlight"),
														 RGB(213, 225, 242)));
	clrBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBackDisabled"), RGB(238, 238, 238)));

	// load border state colors from INI file.
	clrBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBorder"), RGB(255, 255, 255)));
	clrBorderHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBorderHighlight"), RGB(213, 225, 242)));
	clrBorderPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBorderPushed"), RGB(163, 189, 227)));
	clrBorderChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBorderChecked"), RGB(194, 213, 242)));
	clrBorderCheckedHighlight.SetStandardValue(XTPIniColor(_T("CommandBars.Ribbon"),
														   _T("ButtonBorderCheckedHighlight"),
														   RGB(163, 189, 227)));
	clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("ButtonBorderDisabled"), RGB(192, 193, 194)));

	// tab colors.
	clrTabTextInactive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("TabTextInactive"), RGB(68, 68, 68)));
	clrTabBackInactive.SetStandardValue(
		XTPIniColor(_T("CommandBars.Ribbon"), _T("TabBackInactive"), RGB(255, 255, 255)));
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateCheckMark - Default colors found in OFFICE2013WORD_INI

void CXTPColorSetStateCheckMark::RefreshColors()
{
	// load foreground state colors from INI file.
	clrCheck.SetStandardValue(XTPIniColor(_T("CommandBars"), _T("CheckMark"), RGB(68, 68, 68)));
	clrCheckHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkHighlight"), RGB(68, 68, 68)));
	clrCheckPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkPushed"), RGB(68, 68, 68)));
	clrCheckDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkDisabled"), RGB(177, 177, 177)));

	// load background state colors from INI file.
	clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkBack"), RGB(255, 255, 255)));
	clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkBackHighlight"), RGB(213, 225, 242)));
	clrBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkBackPushed"), RGB(163, 189, 227)));
	clrBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("CheckMarkBackDisabled"), RGB(238, 238, 238)));
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateComboBox - Default colors found in OFFICE2013WORD_INI

void CXTPColorSetStateComboBox::RefreshColors()
{
	// load background state colors from INI file.
	clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBack"), RGB(255, 255, 255)));
	clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBackHighlight"), RGB(255, 255, 255)));
	clrBackDropped.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBackDropped"), RGB(255, 255, 255)));
	clrBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBackDisabled"), RGB(255, 255, 255)));

	// load border state colors from INI file.
	clrBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBorder"), RGB(171, 171, 171)));
	clrBorderHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBorderHighlight"), RGB(163, 189, 227)));
	clrBorderDropped.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBorderDropped"), RGB(163, 189, 227)));
	clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("ComboBoxBorderDisabled"), RGB(225, 225, 225)));

	// load background state colors from INI file.
	clrDropDownBack.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBack"), RGB(255, 255, 255)));
	clrDropDownBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBackHighlight"), RGB(213, 225, 242)));
	clrDropDownBackDropped.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBackDropped"), RGB(163, 189, 227)));
	clrDropDownBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBackDisabled"), RGB(253, 253, 253)));

	// load border state colors from INI file.
	clrDropDownBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBorder"), RGB(255, 255, 255)));
	clrDropDownBorderHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBorderHighlight"), RGB(163, 189, 227)));
	clrDropDownBorderDropped.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBorderDropped"), RGB(163, 189, 227)));
	clrDropDownBorderDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownBorderDisabled"), RGB(255, 255, 255)));

	// load marker state colors from INI file.
	clrDropDownMarker.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownMarker"), RGB(119, 119, 119)));
	clrDropDownMarkerHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownMarkerHighlight"), RGB(68, 68, 68)));
	clrDropDownMarkerDropped.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownMarkerDropped"), RGB(68, 68, 68)));
	clrDropDownMarkerDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars"), _T("DropDownMarkerDisabled"), RGB(198, 198, 198)));
}

//////////////////////////////////////////////////////////////////////
// CXTPColorSetStateCheckBox - Default colors found in OFFICE2013WORD_INI

void CXTPColorSetStateCheckBox::RefreshColors()
{
	// load check mark state colors from INI file.
	clrMark.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("Mark"), RGB(139, 139, 139)));
	clrMarkPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("MarkPushed"), RGB(15, 20, 24)));
	clrMarkHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("MarkHighlight"), RGB(67, 75, 80)));
	clrMarkDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("MarkDisabled"), RGB(198, 198, 198)));

	// load background state colors from INI file.
	clrBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("Back"), RGB(253, 253, 253)));
	clrBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("BackPushed"), RGB(163, 189, 227)));
	clrBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("BackHighlight"), RGB(213, 225, 242)));
	clrBackDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("BackDisabled"), RGB(253, 253, 253)));

	// load border state colors from INI file.
	clrBorder.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("Border"), RGB(171, 171, 171)));
	clrBorderPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("BorderPushed"), RGB(163, 189, 227)));
	clrBorderHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("BorderHighlight"), RGB(163, 189, 227)));
	clrBorderDisabled.SetStandardValue(
		XTPIniColor(_T("CommandBars.CheckBox"), _T("BorderDisabled"), RGB(225, 225, 225)));
}
