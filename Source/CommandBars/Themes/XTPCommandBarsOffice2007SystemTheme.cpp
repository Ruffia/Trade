// XTPCommandBarsOffice2007SystemTheme.cpp : implementation of the
// CXTPCommandBarsOffice2007SystemTheme class.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMarkupRender.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

// Frame
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeOffice2007System.h"

// CommandBars
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2007SystemTheme.h"

#ifdef _XTP_INCLUDE_RIBBON
// Ribbon
#	include "Ribbon/XTPRibbonPaintManager.h"
#	include "Ribbon/XTPRibbonMarkupTheme.h"
#	include "Ribbon/Themes/XTPRibbonThemeOffice2007System.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif

#endif

//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOffice2007SystemTheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsOffice2007SystemTheme, CXTPCommandBarsOfficeXPTheme)

CXTPCommandBarsOffice2007SystemTheme::CXTPCommandBarsOffice2007SystemTheme()
{
	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);

	// Frame paint manager
	CXTPFrameThemeOffice2007System* pFramePaintManager = new CXTPFrameThemeOffice2007System(this);
	SAFE_DELETE(m_pFramePaintManager);
	m_pFramePaintManager = pFramePaintManager;

	// Ribbon paint manager
	CXTPRibbonThemeOffice2007System* pRibbonPaintManager = new CXTPRibbonThemeOffice2007System(
		this);
	SAFE_DELETE(m_pRibbonPaintManager);
	m_pRibbonPaintManager = pRibbonPaintManager;
}

CXTPCommandBarsOffice2007SystemTheme::~CXTPCommandBarsOffice2007SystemTheme()
{
	XTPMarkupReleaseContext(m_pMarkupContext);
}

CXTPFramePaintManager* CXTPCommandBarsOffice2007SystemTheme::GetFramePaintManager()
{
	return m_pFramePaintManager;
}

CXTPRibbonPaintManager* CXTPCommandBarsOffice2007SystemTheme::GetRibbonPaintManager()
{
	return m_pRibbonPaintManager;
}

void CXTPCommandBarsOffice2007SystemTheme::RefreshMetrics()
{
	CXTPCommandBarsOfficeXPTheme::RefreshMetrics();
}
