// XTPTabColorSetNativeWindows10.cpp
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
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2012.h"
#include "TabManager/ColorSets/XTPTabColorSetNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabColorSetNativeWindows10::CXTPTabColorSetNativeWindows10()
	: CXTPTabColorSetVisualStudio2012()
{
}

void CXTPTabColorSetNativeWindows10::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonNormal	  = XTPIniColor(_T("TabManager"), _T("Button"), RGB(238, 238, 242));
	m_clrButtonPressed	 = XTPIniColor(_T("TabManager"), _T("ButtonPressed"), RGB(14, 97, 152));
	m_clrButtonHighlighted = XTPIniColor(_T("TabManager"), _T("ButtonHighlighted"),
										 RGB(28, 151, 234));
	m_clrButtonHighlightedClose = XTPIniColor(_T("TabManager"), _T("ButtonHighlightedClose"),
											  RGB(82, 176, 239));
	m_clrButtonSelected = XTPIniColor(_T("TabManager"), _T("ButtonSelected"), RGB(0, 122, 204));
	m_clrButtonSelectedPressed	 = XTPIniColor(_T("TabManager"), _T("ButtonSelectedPressed"),
											 RGB(14, 97, 152));
	m_clrButtonSelectedHighlighted = XTPIniColor(_T("TabManager"), _T("ButtonSelectedHighlighted"),
												 RGB(28, 151, 234));
	m_clrButtonBorderHot = XTPIniColor(_T("TabManager"), _T("ButtonBorderHot"), COLORREF_NULL);
	m_clrTextNormal		 = XTPIniColor(_T("TabManager"), _T("Text"), RGB(30, 30, 30));
	m_clrTextPressed	 = XTPIniColor(_T("TabManager"), _T("TextPressed"), RGB(255, 255, 255));
	m_clrTextHighlight   = XTPIniColor(_T("TabManager"), _T("TextHighlighted"), RGB(255, 255, 255));
	m_clrTextHighlightClose = XTPIniColor(_T("TabManager"), _T("TextHighlightedClose"),
										  RGB(255, 255, 255));
	m_clrTextSelected		= XTPIniColor(_T("TabManager"), _T("TextSelected"), RGB(255, 255, 255));
	m_clrTextSelectedPressed		 = XTPIniColor(_T("TabManager"), _T("TextSelectedPressed"),
										   RGB(255, 255, 255));
	m_clrTextSelectedHighlighted	 = XTPIniColor(_T("TabManager"), _T("TextSelectedHighlighted"),
											   RGB(0, 0, 0));
	m_clrActiveFilesGlyph			 = XTPIniColor(_T("TabManager"), _T("ActiveFilesGlyph"),
										   RGB(113, 113, 113));
	m_clrActiveFilesGlyphPressed	 = XTPIniColor(_T("TabManager"), _T("ActiveFilesGlyphPressed"),
											   RGB(255, 255, 255));
	m_clrActiveFilesGlyphHighlighted = XTPIniColor(_T("TabManager"),
												   _T("ActiveFilesGlyphHighlighted"),
												   RGB(0, 122, 204));
	m_clrActiveFilesBack = XTPIniColor(_T("TabManager"), _T("ActiveFilesBack"), RGB(238, 238, 242));
	m_clrActiveFilesBackPressed		= XTPIniColor(_T("TabManager"), _T("ActiveFilesBackPressed"),
											  RGB(0, 122, 204));
	m_clrActiveFilesBackHighlighted = XTPIniColor(_T("TabManager"),
												  _T("ActiveFilesBackHighlighted"),
												  RGB(255, 255, 255));
	m_clrFrameBorder  = XTPIniColor(_T("TabManager"), _T("FrameBorder"), RGB(238, 238, 242));
	m_clrButtonBorder = XTPIniColor(_T("TabManager"), _T("FrameBorder"), RGB(238, 238, 242));
	m_clrAutoHideFace = XTPIniColor(_T("TabManager"), _T("AutoHideFace"), RGB(238, 238, 242));
	m_clrHeaderFace   = XTPIniColor(_T("TabManager"), _T("HeaderFace"), RGB(238, 238, 242));
	m_csStateButton.clrFace.clrLight   = m_clrButtonSelected;
	m_csStateButton.clrFace.clrDark	= m_clrButtonSelected;
	m_csStateButton.clrBorder.clrDark  = m_clrFrameBorder;
	m_csStateButton.clrBorder.clrLight = m_clrFrameBorder;
	m_clrNonActiveButtonSelected = XTPIniColor(_T("TabManager"), _T("NonActiveButtonSelected"),
											   RGB(204, 206, 219));
	m_clrNonActiveButtonSelectedPressed		= XTPIniColor(_T("TabManager"),
													  _T("NonActiveButtonSelectedPressed"),
													  RGB(183, 185, 197));
	m_clrNonActiveButtonSelectedHighlighted = XTPIniColor(_T("TabManager"),
														  _T("NonActiveButtonSelectedHighlighted"),
														  RGB(230, 231, 237));
	m_clrNonActiveTextSelected		  = XTPIniColor(_T("TabManager"), _T("NonActiveTextSelected"),
												RGB(109, 109, 112));
	m_clrNonActiveTextSelectedPressed = XTPIniColor(_T("TabManager"),
													_T("NonActiveTextSelectedPressed"),
													RGB(45, 45, 45));
	m_clrNonActiveTextSelectedHighlighted = XTPIniColor(_T("TabManager"),
														_T("NonActiveTextSelectedHighlighted"),
														RGB(113, 113, 113));

	m_clrTabClientBack   = XTPIniColor(_T("TabClient"), _T("Back"), m_clrTabClientBack);
	m_clrTabClientBorder = XTPIniColor(_T("TabClient"), _T("Border"), m_clrTabClientBorder);
}
