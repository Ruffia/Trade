// XTPStatusBarPaintManager.cpp
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
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPWinThemeWrapper.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPane.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPaneSwitch.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPStatusBarPaintManager, CObject)

CXTPStatusBarPaintManager::CXTPStatusBarPaintManager(CXTPPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	ASSERT(NULL != pPaintManager);
}

void CXTPStatusBarPaintManager::RefreshMetrics()
{
	m_clrText.SetStandardValue(
		XTPIniColor(_T("StatusBar"), _T("TextColor"), ::GetSysColor(COLOR_BTNTEXT)));
	m_clrTextGray.SetStandardValue(
		XTPIniColor(_T("StatusBar"), _T("TextGrayColor"), ::GetSysColor(COLOR_GRAYTEXT)));
	m_clrBackground.SetStandardValue(
		XTPIniColor(_T("StatusBar"), _T("BackgroundColor"), ::GetSysColor(COLOR_BTNFACE)));
	m_clrGripperLite.SetStandardValue(
		XTPIniColor(_T("StatusBar"), _T("GripperLite"), ::GetSysColor(COLOR_BTNHIGHLIGHT)));
	m_clrGripperDark.SetStandardValue(
		XTPIniColor(_T("StatusBar"), _T("GripperDark"), ::GetSysColor(COLOR_BTNSHADOW)));
}

void CXTPStatusBarPaintManager::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	if (m_pPaintManager->m_bThemedStatusBar
		&& m_pPaintManager->m_themeStatusBar->IsAppThemeActive())
	{
		m_pPaintManager->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0,
															   CXTPClientRect((CWnd*)pBar), 0);
	}
	else
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_pPaintManager->m_bThemedStatusBar
		&& m_pPaintManager->m_themeStatusBar->IsAppThemeActive())
	{
		CRect rcGripper(rcClient.right - XTP_DPI_X(18), rcClient.top, rcClient.right,
						rcClient.bottom);
		m_pPaintManager->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), SP_GRIPPER, 0,
															   &rcGripper, 0);
	}
	else
	{
		CXTPPenDC penBorder(*pDC, m_clrGripperDark);
		CSize szGripperStep(XTP_DPI_X(4), XTP_DPI_Y(4));
		CSize szGripperMargin(XTP_DPI_X(1), XTP_DPI_Y(1));
		int i;
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(rcClient.Width() - 3 - i * szGripperStep.cx,
						rcClient.Height() - szGripperMargin.cy);
			pDC->LineTo(rcClient.Width() - szGripperMargin.cx,
						rcClient.Height() - 3 - i * szGripperStep.cy);

			pDC->MoveTo(rcClient.Width() - 4 - i * szGripperStep.cx,
						rcClient.Height() - szGripperMargin.cy);
			pDC->LineTo(rcClient.Width() - szGripperMargin.cx,
						rcClient.Height() - 4 - i * szGripperStep.cy);
		}

		CXTPPenDC penBorderWhite(*pDC, m_clrGripperLite);
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(rcClient.Width() - 5 - i * szGripperStep.cx,
						rcClient.Height() - szGripperMargin.cy);
			pDC->LineTo(rcClient.Width() - szGripperMargin.cx,
						rcClient.Height() - 5 - i * szGripperStep.cy);
		}
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarButtonFace(CDC* pDC, CRect rcItem,
														CXTPStatusBarPane* pPane)
{
	if (pPane->IsHighlighted() || pPane->IsPressed() || pPane->IsChecked())
	{
		m_pPaintManager->DrawRectangle(pDC, rcItem, pPane->IsHighlighted(), pPane->IsPressed(),
									   TRUE, pPane->IsChecked(), FALSE, xtpBarTypeNormal,
									   xtpBarBottom);
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarPaneBorder(CDC* pDC, CRect rc,
														CXTPStatusBarPane* pPane, BOOL bGripperPane)
{
	if (m_pPaintManager->m_bThemedStatusBar
		&& m_pPaintManager->m_themeStatusBar->IsAppThemeActive())
	{
		m_pPaintManager->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(),
															   bGripperPane ? SP_GRIPPERPANE
																			: SP_PANE,
															   0, rc, 0);
	}
	else
	{
		if (!pPane->IsButton())
			m_pPaintManager->Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarPaneSeparator(CDC* pDC, CRect rc,
														   CXTPStatusBarPane* /*pPane*/)
{
	CRect rcSeparator(rc.left - XTP_DPI_X(2), rc.top, rc.left, rc.bottom);
	m_pPaintManager->Draw3dRect(pDC, rcSeparator, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
}

COLORREF CXTPStatusBarPaintManager::GetTextColor(CXTPStatusBarPane* pPane)
{
	return pPane->IsButton() && (pPane->IsHighlighted() || pPane->IsPressed())
			   ? m_pPaintManager->GetRectangleTextColor(pPane->IsHighlighted(), pPane->IsPressed(),
														pPane->IsEnabled(), FALSE, FALSE,
														xtpBarTypeNormal, xtpBarBottom)
			   : m_clrText.operator COLORREF();
}

void CXTPStatusBarPaintManager::DrawStatusBarPaneEntry(CDC* pDC, CRect rcItem,
													   CXTPStatusBarPane* pPane)
{
	BOOL bEnabled  = pPane->IsEnabled();
	BOOL bDrawText = bEnabled || pPane->GetStatusBar()->GetDrawDisabledText();

	if (pPane->GetBackgroundColor() != (COLORREF)-1)
	{
		CRect rcBackground(rcItem);

		if (pPane->HasBorders())
			rcBackground.DeflateRect(1, 1);

		pDC->FillSolidRect(rcBackground, pPane->GetBackgroundColor());
	}
	if (pPane->IsButton())
	{
		DrawStatusBarButtonFace(pDC, rcItem, pPane);
	}

	CXTPFontDC font(pDC, pPane->GetTextFont());
	rcItem.DeflateRect(pPane->GetPadding());

	CString str = pPane->GetText();

	CXTPMarkupUIElement* pUIElement = pPane->GetMarkupUIElement();
	CXTPImageManagerIcon* pIcon		= pPane->GetImage();

	COLORREF clrNormalText = GetTextColor(pPane);

	COLORREF clrText = !bEnabled ? pPane->GetTextGrayColor() != (COLORREF)-1
									   ? pPane->GetTextGrayColor()
									   : m_clrTextGray.operator COLORREF()
								 : pPane->GetTextColor() != (COLORREF)-1 ? pPane->GetTextColor()
																		 : clrNormalText;

	pDC->SetTextColor(clrText);
	XTPMarkupSetDefaultFont(pPane->GetStatusBar()->GetMarkupContext(),
							(HFONT)pPane->GetTextFont()->GetSafeHandle(), clrText);

	if (pPane->GetTextAlignment() != DT_LEFT)
	{
		int nWidth = 0;

		if (NULL != pUIElement)
		{
			nWidth += XTPMarkupMeasureElement(pUIElement).cx; // Markup
		}
		else
		{
			nWidth += pDC->GetTextExtent(str).cx; // Text
		}

		nWidth += pIcon ? XTP_DPI_X(pIcon->GetWidth()) + (nWidth > 0 ? XTP_DPI_X(2) : 0) : 0;

		if (rcItem.Width() > nWidth)
		{
			if (pPane->GetTextAlignment() == DT_RIGHT)
			{
				rcItem.left = rcItem.right - nWidth;
			}
			if (pPane->GetTextAlignment() == DT_CENTER)
			{
				rcItem.left = (rcItem.left + rcItem.right - nWidth) / 2;
			}
		}
	}

	if (pIcon)
	{
		CSize szIcon = (pIcon->IsVectorIcon()
							? rcItem.Size()
							: CXTPPaintManager::AdjustDpiIconSize(
								  pIcon, pIcon->GetExtent(),
								  XTP_SAFE_GET2(pPane, GetStatusBar(), GetCommandBars(), NULL),
								  rcItem.Size()));

		if (rcItem.right >= rcItem.left + szIcon.cx)
			pIcon->Draw(pDC, CPoint(rcItem.left, (rcItem.top + rcItem.bottom - szIcon.cy) / 2),
						bEnabled ? xtpImageNormal : xtpImageDisabled, szIcon);

		rcItem.left += szIcon.cx + XTP_DPI_X(2);
	}

	if (bDrawText)
	{
		if (pUIElement)
		{
			XTPMarkupMeasureElement(pUIElement, rcItem.Width(), INT_MAX);
			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcItem);
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(str, rcItem, DT_SINGLELINE | DT_VCENTER | DT_EXPANDTABS | DT_NOPREFIX);
		}
	}
}

int CXTPStatusBarPaintManager::DrawStatusBarSwitchPane(CDC* pDC, CRect rcItem,
													   CXTPStatusBarSwitchPane* pPane)
{
	ASSERT_VALID(pPane);

	int nWidth = 0;
	int nCount = pPane->GetSwitchCount();

	if (pDC)
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);

			rcItem.right  = rcItem.left + pSwitch->nWidth;
			BOOL bEnabled = pPane->IsEnabled() && pSwitch->bEnabled;

			m_pPaintManager->DrawRectangle(pDC, rcItem, pSwitch->bHighlighted, pSwitch->bPressed,
										   bEnabled, pSwitch->bChecked, FALSE, xtpBarTypeNormal,
										   xtpBarTop);

			CXTPImageManagerIcon* pIcon =
				pPane->GetStatusBar()->GetImageManager()->GetImage(pSwitch->nID, ICON_SMALL);
			if (pIcon)
			{
				CSize szSize = (pIcon->IsVectorIcon() ? rcItem.Size()
													  : CXTPPaintManager::AdjustDpiIconSize(
															pIcon, pIcon->GetExtent(),
															XTP_SAFE_GET2(pPane, GetStatusBar(),
																		  GetCommandBars(), NULL),
															rcItem.Size()));
				CPoint pt((rcItem.left + rcItem.right - szSize.cx) / 2,
						  (rcItem.top + rcItem.bottom - szSize.cy) / 2);
				pIcon->Draw(pDC, pt,
							(!bEnabled ? xtpImageDisabled
									   : pSwitch->bChecked ? xtpImageChecked : xtpImageNormal),
							szSize);
			}

			rcItem.left = rcItem.right;
		}
	}
	else
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);
			pSwitch->nWidth							 = XTP_DPI_X(18);

			nWidth += pSwitch->nWidth;
		}
	}

	return nWidth;
}
