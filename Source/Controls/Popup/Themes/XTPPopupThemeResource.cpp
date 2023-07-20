// XTPPopupThemeResource.cpp: implementation of the CXTPPopupThemeResource class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"

#include "Controls/Popup/XTPPopupPaintManager.h"
#include "Controls/Popup/Themes/XTPPopupThemeOffice2000.h"
#include "Controls/Popup/Themes/XTPPopupThemeOfficeXP.h"
#include "Controls/Popup/Themes/XTPPopupThemeOffice2003.h"
#include "Controls/Popup/Themes/XTPPopupThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeResource

CXTPPopupThemeResource::CXTPPopupThemeResource()
{
}

void CXTPPopupThemeResource::RefreshMetrics()
{
	CXTPPopupThemeOffice2003::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	m_clrBackground.SetStandardValue(pImages->GetImageColor(_T("PopupControl"),
															_T("BackgroundLight")),
									 pImages->GetImageColor(_T("PopupControl"),
															_T("BackgroundDark")));
	m_clrGripper.SetStandardValue(pImages->GetImageColor(_T("PopupControl"), _T("CaptionLight")),
								  pImages->GetImageColor(_T("PopupControl"), _T("CaptionDark")));
	m_clrFrame.SetStandardValue(pImages->GetImageColor(_T("PopupControl"), _T("FrameBorder")));

	m_clrText			= pImages->GetImageColor(_T("PopupControl"), _T("NormalText"));
	m_clrButtonSelected = pImages->GetImageColor(_T("PopupControl"), _T("ButtonSelected"));
	m_clrButtonPressed  = pImages->GetImageColor(_T("PopupControl"), _T("ButtonPressed"));
	m_clrButtonPressedBorder.SetStandardValue(
		pImages->GetImageColor(_T("PopupControl"), _T("ButtonBorder")));
	m_clrButtonSelectedBorder.SetStandardValue(
		pImages->GetImageColor(_T("PopupControl"), _T("ButtonBorder")));
}
