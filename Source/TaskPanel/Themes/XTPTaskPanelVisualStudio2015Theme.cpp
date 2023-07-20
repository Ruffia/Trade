// XTPTaskPanelVisualStudio2015Theme.cpp : implementation of the CXTPTaskPanelVisualStudio2015Theme
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"

#include "TaskPanel/XTPTaskPanelDefines.h"
#include "TaskPanel/XTPTaskPanelItem.h"
#include "TaskPanel/XTPTaskPanelPaintManager.h"

#include "TaskPanel/Themes/XTPTaskPanelPaintManagerPlain.h"
#include "TaskPanel/Themes/XTPTaskPanelToolboxTheme.h"
#include "TaskPanel/Themes/XTPTaskPanelVisualStudio2005Theme.h"
#include "TaskPanel/Themes/XTPTaskPanelVisualStudio2012Theme.h"
#include "TaskPanel/Themes/XTPTaskPanelVisualStudio2015Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelVisualStudio2015Theme

CXTPTaskPanelVisualStudio2015Theme::CXTPTaskPanelVisualStudio2015Theme()
{
}

CXTPTaskPanelVisualStudio2015Theme::~CXTPTaskPanelVisualStudio2015Theme()
{
}

void CXTPTaskPanelVisualStudio2015Theme::RefreshMetrics()
{
	CXTPTaskPanelVisualStudio2005Theme::RefreshMetrics();

	m_rcItemInnerMargins = XTP_DPI(
		XTPIniRect(_T("TaskPanel"), _T("ItemInnerMargin"), CRect(23, 2, 2, 2)));
	m_rcItemOuterMargins = XTP_DPI(
		XTPIniRect(_T("TaskPanel"), _T("ItemOuterMargin"), CRect(0, 1, 0, 1)));
	m_rcGroupOuterMargins = XTP_DPI(
		XTPIniRect(_T("TaskPanel"), _T("GroupOuterMargin"), CRect(0, 0, 0, 5)));

	m_clrBackground.SetStandardValue(XTPIniColor(_T("TaskPanel"), _T("Back"), RGB(243, 243, 243)));
	m_clrHighlightSelected.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("HighlightSelected"), RGB(225, 225, 225)));
	m_clrHighlightHot.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("HighlightHot"), RGB(213, 225, 242)));
	m_clrSelectedWithoutFocus.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("SelectedWithoutFocus"), RGB(225, 225, 225)));
	m_clrText.SetStandardValue(XTPIniColor(_T("TaskPanel"), _T("Text"), RGB(68, 68, 68)));
	m_clrSelectedText.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("SelectedText"), RGB(38, 38, 38)));
	m_clrSelectedTextWithoutFocus.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("SelectedTextWithoutFocus"), RGB(68, 68, 68)));
	m_clrTriangle.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("Triangle"), RGB(131, 134, 136)));
	m_clrTriangleHot.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("TriangleHot"), RGB(62, 109, 181)));
	m_clrTriangleFocused.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("TriangleFocused"), RGB(62, 109, 181)));

	// set colors for groups
	m_groupNormal.clrHead.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("GroupNormalHead"), RGB(243, 243, 243)));
	m_groupNormal.clrClient.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("GroupNormalClient"), RGB(243, 243, 243)));
	m_groupSpecial = m_groupNormal;

	// selected, pressed and border
	m_clrHighlightBorder.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("HighlightBorder"), RGB(225, 225, 225)));
	m_clrHighlightPressed.SetStandardValue(
		XTPIniColor(_T("TaskPanel"), _T("HighlightPressed"), RGB(225, 225, 225)));
}
