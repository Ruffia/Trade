// XTPStatusBarThemeOffice2013.cpp
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
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOffice2013.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPane.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPaneSwitch.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeOffice2013::CXTPStatusBarThemeOffice2013(CXTPPaintManager* pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeOffice2013::RefreshMetrics()
{
	m_clrText.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("TextColor"), RGB(255, 255, 255)));
	m_clrTextGray.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("TextGrayColor"), RGB(43, 87, 154)));
	m_clrBackground.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("BackgroundColor"), RGB(43, 87, 154)));
	m_clrButtonBack.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonBack"), RGB(43, 87, 154)));
	m_clrButtonBackChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonBackChecked"), RGB(25, 71, 138)));
	m_clrButtonBackPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonBackPushed"), RGB(25, 71, 138)));
	m_clrButtonBackHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonBackHighlight"), RGB(62, 109, 181)));
	m_clrButtonTextChecked.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonTextChecked"), RGB(68, 68, 68)));
	m_clrButtonTextPushed.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonTextPushed"), RGB(68, 68, 68)));
	m_clrButtonTextHighlight.SetStandardValue(
		XTPIniColor(_T("CommandBars.StatusBar"), _T("ButtonTextHighlight"), RGB(68, 68, 68)));
	m_clrGripperLite.SetStandardValue(m_clrBackground);
	m_clrGripperDark.SetStandardValue(m_clrBackground);
}

void CXTPStatusBarThemeOffice2013::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	if ((NULL != pDC) && (NULL != pBar))
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), m_clrBackground);
	}
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	// No gripper
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rcClient);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarPaneBorder(CDC* pDC, CRect rc,
														   CXTPStatusBarPane* pPane,
														   BOOL bGripperPane)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	UNUSED_ALWAYS(pPane);
	UNUSED_ALWAYS(bGripperPane);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarPaneSeparator(CDC* pDC, CRect rc,
															  CXTPStatusBarPane* pPane)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	UNUSED_ALWAYS(pPane);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarPaneEntry(CDC* pDC, CRect rcItem,
														  CXTPStatusBarPane* pPane)
{
	CXTPStatusBarPaintManager::DrawStatusBarPaneEntry(pDC, rcItem, pPane);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarButtonFace(CDC* pDC, CRect rcItem,
														   CXTPStatusBarPane* pPane)
{
	if (pPane->IsPressed())
		pDC->FillSolidRect(rcItem, m_clrButtonBackPushed);
	else if (pPane->IsHighlighted())
		pDC->FillSolidRect(rcItem, m_clrButtonBackHighlight);
	else if (pPane->IsChecked())
		pDC->FillSolidRect(rcItem, m_clrButtonBackChecked);
	else
		pDC->FillSolidRect(rcItem, m_clrButtonBack);
}

int CXTPStatusBarThemeOffice2013::DrawStatusBarSwitchPane(CDC* pDC, CRect rcItem,
														  CXTPStatusBarSwitchPane* pPane)
{
	ASSERT_VALID(pPane);

	int nWidth = 0;

	if (NULL == pDC)
	{
		for (int i = 0; i < pPane->GetSwitchCount(); i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);
			pSwitch->nWidth							 = XTP_DPI_X(38);

			nWidth += pSwitch->nWidth;
		}
	}
	else
	{
		for (int i = 0; i < pPane->GetSwitchCount(); i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);

			rcItem.right  = rcItem.left + pSwitch->nWidth;
			BOOL bEnabled = pPane->IsEnabled() && pSwitch->bEnabled;

			if (pSwitch->bPressed)
				pDC->FillSolidRect(rcItem, m_clrButtonBackPushed);
			else if (pSwitch->bHighlighted)
				pDC->FillSolidRect(rcItem, m_clrButtonBackHighlight);
			else if (pSwitch->bChecked)
				pDC->FillSolidRect(rcItem, m_clrButtonBackChecked);
			else
				pDC->FillSolidRect(rcItem, m_clrButtonBack);

			CXTPImageManagerIcon* pIcon =
				pPane->GetStatusBar()->GetImageManager()->GetImage(pSwitch->nID, ICON_SMALL);
			if (pIcon)
			{
				CSize sz = CXTPPaintManager::AdjustDpiIconSize(
					pIcon, pIcon->GetExtent(),
					XTP_SAFE_GET2(pPane, GetStatusBar(), GetCommandBars(), NULL), rcItem.Size());

				CPoint pt((rcItem.left + rcItem.right - sz.cx) / 2,
						  (rcItem.top + rcItem.bottom - sz.cy) / 2);
				pIcon->Draw(pDC, pt,
							(!bEnabled ? xtpImageDisabled
									   : pSwitch->bChecked ? xtpImageChecked : xtpImageNormal),
							sz);
			}

			rcItem.left = rcItem.right;
		}
	}

	return nWidth;
}

COLORREF CXTPStatusBarThemeOffice2013::GetTextColor(CXTPStatusBarPane* pPane)
{
	if (pPane->IsPressed())
	{
		return m_clrButtonTextPushed;
	}
	else if (pPane->IsChecked())
	{
		return m_clrButtonTextChecked;
	}
	else if (pPane->IsHighlighted())
	{
		return m_clrButtonTextHighlight;
	}

	return m_clrText;
}
