// XTPDockingPaneColorSet2013.cpp
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
#include "Common/XTPTheme.h"
#include "Common/XTPResourceImage.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPDockingPaneColorSet2013::RefreshMetrics(CString strSection)
{
	if (strSection.IsEmpty())
		return;

	BOOL bActive = strSection.Find(_T("Active")) != -1;

	m_clrBorder		= XTPIniColor(strSection, _T("BorderColor"),
							  bActive ? RGB(43, 87, 154) : RGB(212, 212, 212));
	m_clrBackground = XTPIniColor(strSection, _T("BackgroundColor"),
								  bActive ? RGB(255, 255, 255) : RGB(255, 255, 255));
	m_clrText		= XTPIniColor(strSection, _T("TextColor"),
							  bActive ? RGB(255, 255, 255) : RGB(43, 87, 154));
	m_clrCaption	= XTPIniColor(strSection, _T("CaptionColor"),
								  bActive ? RGB(43, 87, 154) : RGB(255, 255, 255));
	m_clrGripper	= XTPIniColor(strSection, _T("GripperColor"),
								  bActive ? RGB(43, 87, 154) : RGB(255, 255, 255));

	// define caption button hot colors.
	m_csCaptionButtonHot.m_clrOutline	= XTPIniColor(strSection + _T(".CaptionButton.Hot"),
													   _T("Border"),
													   bActive ? RGB(213, 225, 242)
															   : RGB(213, 225, 242));
	m_csCaptionButtonHot.m_clrFill		 = XTPIniColor(strSection + _T(".CaptionButton.Hot"),
												   _T("FillColor"),
												   bActive ? RGB(62, 109, 181) : RGB(62, 109, 181));
	m_csCaptionButtonHot.m_clrBackground = XTPIniColor(strSection + _T(".CaptionButton.Hot"),
													   _T("BackgroundColor"),
													   bActive ? RGB(213, 225, 242)
															   : RGB(213, 225, 242));

	// define caption button normal colors.
	m_csCaptionButtonNormal.m_clrOutline	= XTPIniColor(strSection + _T(".CaptionButton.Normal"),
														  _T("Border"),
														  bActive ? RGB(43, 87, 154)
																  : RGB(255, 255, 255));
	m_csCaptionButtonNormal.m_clrFill		= XTPIniColor(strSection + _T(".CaptionButton.Normal"),
													  _T("FillColor"),
													  bActive ? RGB(255, 255, 255) : RGB(68, 68, 68));
	m_csCaptionButtonNormal.m_clrBackground = XTPIniColor(strSection + _T(".CaptionButton.Normal"),
														  _T("BackgroundColor"),
														  bActive ? RGB(43, 87, 154)
																  : RGB(255, 255, 255));

	// define caption button pressed colors.
	m_csCaptionButtonPressed.m_clrOutline = XTPIniColor(strSection + _T(".CaptionButton.Pressed"),
														_T("Border"),
														bActive ? RGB(163, 189, 227)
																: RGB(163, 189, 227));
	m_csCaptionButtonPressed.m_clrFill	= XTPIniColor(strSection + _T(".CaptionButton.Pressed"),
														_T("FillColor"),
														bActive ? RGB(25, 71, 138) : RGB(25, 71, 138));
	m_csCaptionButtonPressed.m_clrBackground =
		XTPIniColor(strSection + _T(".CaptionButton.Pressed"), _T("BackgroundColor"),
					bActive ? RGB(163, 189, 227) : RGB(163, 189, 227));
}
