// XTPTabColorSetVisualStudio2012Dark.cpp
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
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2012Dark.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabColorSetVisualStudio2012Dark::CXTPTabColorSetVisualStudio2012Dark()
	: CXTPTabColorSetVisualStudio2012()
{
}

void CXTPTabColorSetVisualStudio2012Dark::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonNormal						= RGB(45, 45, 48);
	m_clrButtonPressed						= RGB(14, 97, 152);
	m_clrButtonHighlighted					= RGB(28, 151, 234);
	m_clrButtonHighlightedClose				= RGB(82, 176, 239);
	m_clrButtonSelected						= RGB(0, 122, 204);
	m_clrButtonSelectedPressed				= RGB(14, 97, 152);
	m_clrButtonSelectedHighlighted			= RGB(82, 176, 239);
	m_clrButtonBorderHot					= COLORREF_NULL;
	m_clrTextNormal							= RGB(241, 241, 241);
	m_clrTextPressed						= RGB(255, 255, 255);
	m_clrTextHighlight						= RGB(255, 255, 255);
	m_clrTextHighlightClose					= RGB(255, 255, 255);
	m_clrTextSelected						= RGB(255, 255, 255);
	m_clrTextSelectedPressed				= RGB(255, 255, 255);
	m_clrTextSelectedHighlighted			= RGB(255, 255, 255);
	m_clrActiveFilesGlyph					= RGB(241, 241, 241);
	m_clrActiveFilesGlyphPressed			= RGB(255, 255, 255);
	m_clrActiveFilesGlyphHighlighted		= RGB(0, 122, 204);
	m_clrActiveFilesBack					= RGB(45, 45, 48);
	m_clrActiveFilesBackPressed				= RGB(0, 122, 204);
	m_clrActiveFilesBackHighlighted			= RGB(62, 62, 64);
	m_clrFrameBorder						= RGB(45, 45, 48);
	m_clrButtonBorder						= RGB(45, 45, 48);
	m_clrAutoHideFace						= RGB(45, 45, 48);
	m_clrHeaderFace							= RGB(45, 45, 48);
	m_csStateButton.clrFace.clrLight		= m_clrButtonSelected;
	m_csStateButton.clrFace.clrDark			= m_clrButtonSelected;
	m_csStateButton.clrBorder.clrDark		= m_clrFrameBorder;
	m_csStateButton.clrBorder.clrLight		= m_clrFrameBorder;
	m_clrNonActiveButtonSelected			= RGB(63, 63, 70);
	m_clrNonActiveButtonSelectedPressed		= RGB(27, 27, 28);
	m_clrNonActiveButtonSelectedHighlighted = RGB(85, 85, 85);
	m_clrNonActiveTextSelected				= RGB(109, 109, 112);
	m_clrNonActiveTextSelectedPressed		= RGB(241, 241, 241);
	m_clrNonActiveTextSelectedHighlighted   = RGB(241, 241, 241);

	m_clrTabClientBack   = XTPIniColor(_T("TabClient"), _T("Back"), RGB(45, 45, 48));
	m_clrTabClientBorder = XTPIniColor(_T("TabClient"), _T("Border"), RGB(45, 45, 48));
	m_clrPaneFace		 = XTPIniColor(_T("TabManager"), _T("PaneFace"), m_clrTabClientBack);
}
