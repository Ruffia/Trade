// XTPCommandBarsModernUITheme.cpp : implementation of the CXTPCommandBarsModernUITheme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMarkupRender.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsModernUITheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsModernUITheme, CXTPCommandBarsOfficeXPTheme)

CXTPCommandBarsModernUITheme::CXTPCommandBarsModernUITheme()
	: m_pMarkupContext(NULL)
	, m_pImages(NULL)
{
	m_pImages		 = XTPResourceImages();
	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);
}

CXTPCommandBarsModernUITheme::~CXTPCommandBarsModernUITheme()
{
	XTPMarkupReleaseContext(m_pMarkupContext);
}

void CXTPCommandBarsModernUITheme::RefreshMetrics()
{
	CXTPCommandBarsOfficeXPTheme::RefreshMetrics();

	m_pShadowManager->SetShadowOptions(0);

	m_iconsInfo.bUseFadedIcons			  = FALSE;
	m_iconsInfo.bUseDisabledIcons		  = TRUE;
	m_iconsInfo.bIconsWithShadow		  = FALSE;
	m_iconsInfo.bOfficeStyleDisabledIcons = FALSE;

	// Message bar
	m_rcMessageBarMargin = CRect(0, 0, 0, 0);
}
