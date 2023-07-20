// XTPRibbonBackstageViewThemeVisualStudio2019.cpp : implementation file
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/XTPHookManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"
#include "CommandBars/XTPCommandBar.h"

#include "Ribbon/Backstage/XTPRibbonBackstageView.h"
#include "Ribbon/Backstage/XTPRibbonBackstagePaintManager.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeOffice2013.h"
#include "Ribbon/Backstage/XTPRibbonBackstageViewThemeVisualStudio2019.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPRibbonBackstageViewThemeVisualStudio2019::CXTPRibbonBackstageViewThemeVisualStudio2019()
{
	m_clrButtonBackNormal = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("ButtonBackNormal"),
										RGB(255, 243, 156));
	m_clrButtonBackSelected	= XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonBackSelected"), RGB(253, 244, 191));
	m_clrButtonBackHighlighted = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonBackHighlighted"), RGB(253, 244, 191));
	m_clrButtonTextNormal = XTPIniColor(_T("CommandBars.Ribbon.Backstage"), _T("ButtonTextNormal"),
										RGB(115, 97, 57));
	m_clrButtonTextSelected	= XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonTextSelected"), RGB(115, 97, 57));
	m_clrButtonTextHighlighted = XTPIniColor(_T("CommandBars.Ribbon.Backstage"),
											 _T("ButtonTextHighlighted"), RGB(115, 97, 57));
}

CXTPRibbonBackstageViewThemeVisualStudio2019::~CXTPRibbonBackstageViewThemeVisualStudio2019()
{
}
