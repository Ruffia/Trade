// XTPFramePaintManager.cpp : implementation of the CXTPFramePaintManager class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/XTPMenuBar.h"

#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonPopups.h"
#	include "Ribbon/XTPRibbonBar.h"
#endif

#ifndef OIC_WINLOGO
#	define OIC_WINLOGO 32517
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPFramePaintManager

IMPLEMENT_DYNAMIC(CXTPFramePaintManager, CObject)

CXTPFramePaintManager::CXTPFramePaintManager(CXTPPaintManager* pPaintManager)
	: m_bFrameStatusBar(FALSE)
	, m_bRoundedCornersAlways(FALSE)
	, m_nFrameCaptionHeight(::GetSystemMetrics(SM_CYCAPTION))
	, m_pPaintManager(pPaintManager)
	, m_bCaptionCenterAligned(FALSE)
	, m_hIcon(NULL)
	, m_szIcon(0, 0)
	, m_rcIconMargin(0, 0, 0, 0)
	, m_rcCaptionButtonMargin(0, 0, 0, 0)
	, m_szCaptionButtonSize(::GetSystemMetrics(SM_CYSIZE), ::GetSystemMetrics(SM_CYSIZE))
	, m_szMDIClildWindowCaptionButtonSize(::GetSystemMetrics(SM_CYSIZE),
										  ::GetSystemMetrics(SM_CYSIZE))
{
	ASSERT(NULL != pPaintManager);
}

CXTPFramePaintManager::~CXTPFramePaintManager()
{
}

void CXTPFramePaintManager::RefreshMetrics()
{
	m_rcIconMargin = XTP_DPI(
		XTPIniRect(_T("CommandBars.Frame"), _T("IconMargin"), XTP_UNDPI(m_rcIconMargin)));
	m_rcCaptionButtonMargin = XTP_DPI(XTPIniRect(_T("CommandBars.Frame"), _T("CaptionButtonMargin"),
												 XTP_UNDPI(m_rcCaptionButtonMargin)));
	m_szCaptionButtonSize   = XTP_DPI(XTPIniSize(_T("CommandBars.Frame"), _T("CaptionButtonSize"),
												 XTP_UNDPI(m_szCaptionButtonSize)));
	m_nFrameCaptionHeight   = XTP_DPI_Y(XTPIniInt(_T("CommandBars.Frame"), _T("CaptionHeight"),
												  XTP_UNDPI_Y(m_nFrameCaptionHeight)));
	m_bCaptionCenterAligned = XTPIniInt(_T("CommandBars.Frame"), _T("CaptionCenterAligned"),
										m_bCaptionCenterAligned);
}

void CXTPFramePaintManager::RecalcLayout(CXTPCommandBarsFrameHook* pFrameHook)
{
	UNUSED_ALWAYS(pFrameHook);
}

CRect CXTPFramePaintManager::CalcFrameCaption(const CXTPCommandBarsFrameHook* pFrameHook) const
{
	CWnd* pSite = pFrameHook->GetSite();

	CXTPWindowRect rcWindow(pSite);
	CXTPClientRect rcClient(pSite);
	pSite->ClientToScreen(rcClient);

	rcClient.OffsetRect(-rcWindow.TopLeft());
	rcWindow.OffsetRect(-rcWindow.TopLeft());

	CRect rcBorder = CalcFrameBorder(pFrameHook);

	CRect rcCaption(rcWindow.left, rcWindow.top, rcWindow.right, rcClient.top);
	rcCaption.DeflateRect(rcBorder.left, rcBorder.top, rcBorder.right, 0);

	return rcCaption;
}

CRect CXTPFramePaintManager::CalcFrameBorder(const CXTPCommandBarsFrameHook* pFrameHook) const
{
	DWORD dwStyle   = pFrameHook->GetSiteStyle();
	DWORD dwExStyle = pFrameHook->GetSiteExStyle();

	// dwStyle &= ~WS_DLGFRAME;

	// CXTPClientRect rcClient(pFrameHook->GetSite());
	// CRect rcWindow(rcClient);

	CXTPWindowRect rcWindow(pFrameHook->GetSite());

	LONG nLeft = rcWindow.left;
	::AdjustWindowRectEx(rcWindow, dwStyle, FALSE, dwExStyle);

	CRect rcBorder(nLeft - rcWindow.left, nLeft - rcWindow.left, nLeft - rcWindow.left,
				   nLeft - rcWindow.left);

	return rcBorder;
}

CXTPResourceImages* CXTPFramePaintManager::GetImages() const
{
	return XTPResourceImages();
}

CXTPResourceImage* CXTPFramePaintManager::LoadImage(LPCTSTR lpszFileName)
{
	return GetImages()->LoadFile(lpszFileName);
}

HICON CXTPFramePaintManager::GetFrameSmallIcon(CWnd* pFrame)
{
	if (NULL == pFrame)
		return NULL;

	DWORD dwStyle   = pFrame->GetStyle();
	DWORD dwExStyle = pFrame->GetExStyle();

	if (dwExStyle & WS_EX_TOOLWINDOW)
		return NULL;

	if ((dwStyle & WS_SYSMENU) == 0)
		return NULL;

	HICON hIcon = (HICON)(DWORD_PTR)::SendMessage(pFrame->m_hWnd, WM_GETICON, ICON_SMALL, 0);
	if (hIcon)
		return hIcon;

	hIcon = (HICON)(DWORD_PTR)::GetClassLongPtr(pFrame->m_hWnd, GCLP_HICONSM);
	if (hIcon)
		return hIcon;

	if (((dwStyle & (WS_BORDER | WS_DLGFRAME)) != WS_DLGFRAME)
		&& ((dwExStyle & WS_EX_DLGMODALFRAME) == 0))
	{
		ULONG_PTR dwResult;

		if (::SendMessageTimeout(pFrame->GetSafeHwnd(), WM_QUERYDRAGICON, 0, 0, SMTO_NORMAL, 100,
								 &dwResult))
		{
			hIcon = (HICON)dwResult;
		}

		if (hIcon == NULL)
		{
			hIcon = AfxGetApp()->LoadOEMIcon(OIC_WINLOGO);
		}
	}

	return hIcon;
}

HICON CXTPFramePaintManager::GetFrameLargeIcon(CWnd* pFrame)
{
	if (!pFrame)
		return NULL;

	HICON hIcon = (HICON)(DWORD_PTR)::SendMessage(pFrame->m_hWnd, WM_GETICON, ICON_BIG, 0);
	if (hIcon)
		return hIcon;

	hIcon = (HICON)(DWORD_PTR)::GetClassLongPtr(pFrame->m_hWnd, GCLP_HICON);
	if (hIcon)
		return hIcon;

	return hIcon;
}

void CXTPFramePaintManager::DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite,
											BOOL bActive)
{
	DrawCaptionText(pDC, rcCaptionText, pSite, bActive, NULL);
}

void CXTPFramePaintManager::DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite,
											BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	const UINT nFormatNoCenter = DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE
								 | DT_NOPREFIX;
	const UINT nFormat = (m_bCaptionCenterAligned ? DT_CENTER : 0) | nFormatNoCenter;

	CString strWindowText;
	CXTPDrawHelpers::GetWindowCaption(pSite->GetSafeHwnd(), strWindowText);

	pDC->SetBkMode(TRANSPARENT);
	CXTPFontDC font(pDC, &m_fntFrameCaption);

	if (NULL != pRibbonBar && pRibbonBar->IsCaptionBarInBackstageMode())
	{
		pDC->SetTextColor(!bActive ? m_clrFrameCaptionTextInactiveBSMode
								   : m_clrFrameCaptionTextActiveBSMode);
		if (rcCaptionText.left < pRibbonBar->GetBackstageModeMenuWidth())
		{
			rcCaptionText.left = pRibbonBar->GetBackstageModeMenuWidth() + XTP_DPI_X(5);
			rcCaptionText.right += (pRibbonBar->GetBackstageModeMenuWidth() + XTP_DPI_X(5));
			CRect rc;
			pSite->GetClientRect(&rc);
			int limit = rc.right - (m_szCaptionButtonSize.cx * 3) - XTP_DPI_X(5);
			if (rcCaptionText.right > limit)
			{
				rcCaptionText.right = limit;
			}
		}
	}
	else
	{
		pDC->SetTextColor(!bActive ? m_clrFrameCaptionTextInactive : m_clrFrameCaptionTextActive);
	}

	if (pSite->GetStyle() & FWS_PREFIXTITLE)
	{
#if _MSC_VER >= 1200
		CFrameWnd* pFrame = pSite->IsFrameWnd() ? (CFrameWnd*)pSite : NULL;
		CString strTitle  = pFrame ? pFrame->GetTitle() : CString();

		if (!strTitle.IsEmpty() && strWindowText.Right(strTitle.GetLength()) == strTitle
			&& ((pSite->GetExStyle() & WS_EX_LAYOUTRTL) == 0))
		{
			strWindowText.Delete(strWindowText.GetLength() - strTitle.GetLength(),
								 strTitle.GetLength());
			pDC->DrawText(strWindowText, rcCaptionText, nFormatNoCenter);

			int nExtent = pDC->GetTextExtent(strWindowText).cx;
			rcCaptionText.left += nExtent + XTP_DPI_X(2);

			if (rcCaptionText.left < rcCaptionText.right - XTP_DPI_X(5))
			{
				if (bActive)
				{
					if (NULL != pRibbonBar && pRibbonBar->IsCaptionBarInBackstageMode())
					{
						pDC->SetTextColor(m_clrFrameCaptionTextActiveTitleBSMode);
					}
					else
					{
						pDC->SetTextColor(m_clrFrameCaptionTextActiveTitle);
					}
				}

				pDC->DrawText(strTitle, rcCaptionText, nFormatNoCenter);
			}
		}
		else
#endif
		{
			pDC->DrawText(strWindowText, rcCaptionText, nFormat);
		}
	}
	else
	{
		pDC->DrawText(strWindowText, rcCaptionText, nFormat);
	}
}

void CXTPFramePaintManager::DrawCaptionBar(CDC* pDC, CRect rcCaptionBar, CWnd* pSite, BOOL bActive)
{
	pDC->FillSolidRect(rcCaptionBar, bActive ? m_clrFrameCaptionActive : m_clrFrameCaptionInactive);

	UNREFERENCED_PARAMETER(pSite);
}

CSize GetIconSize(HICON hIcon)
{
	CSize szIcon(0, 0);

	ICONINFO iconinfo;

	if (::GetIconInfo(hIcon, &iconinfo))
	{
		BITMAP bitmap;

		if (iconinfo.hbmColor) //  Icon has color bitmap
		{
			if (::GetObject(iconinfo.hbmColor, sizeof(bitmap), &bitmap))
			{
				szIcon.cx = bitmap.bmWidth;
				szIcon.cy = bitmap.bmHeight;
				::DeleteObject(iconinfo.hbmColor);
			}
		}
		else
		{
			if (::GetObject(iconinfo.hbmMask, sizeof(bitmap), &bitmap))
			{
				szIcon.cx = bitmap.bmWidth;
				szIcon.cy = bitmap.bmHeight;
				::DeleteObject(iconinfo.hbmMask);
			}
		}
	}

	return szIcon;
}

void CXTPFramePaintManager::SetIcon(HICON hIcon, CSize szIcon)
{
	m_hIcon  = hIcon;
	m_szIcon = szIcon;
}

void CXTPFramePaintManager::DrawFrameIcon(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook,
										  CRect& rcCaption)
{
	CWnd* pSite = pFrameHook->GetSite();

	HICON hIcon = NULL;
	CSize szIcon(0, 0);

	if (NULL != m_hIcon)
	{
		hIcon = m_hIcon;

		if (m_szIcon != CSize(0, 0))
		{
			szIcon = m_szIcon;
		}
		else
		{
			szIcon = GetIconSize(m_hIcon);
		}
	}
	else
	{
		hIcon	 = GetFrameSmallIcon(pSite);
		szIcon.cx = XTPSystemMetrics()->m_nSmallIconX;
		szIcon.cy = XTPSystemMetrics()->m_nSmallIconY;
	}

	if (NULL != hIcon)
	{
		CPoint ptIcon;
		ptIcon.x = rcCaption.left + m_rcIconMargin.left;
		ptIcon.y = rcCaption.top + ((rcCaption.Height() - szIcon.cy) / 2); // Vertically centered

		DWORD dwLayout = XTPDrawHelpers()->GetLayout(pDC);
		if (LAYOUT_RTL & dwLayout)
		{
			XTPDrawHelpers()->SetContextRTL(pDC, TRUE);
		}

		::DrawIconEx(pDC->m_hDC, ptIcon.x, ptIcon.y, hIcon, szIcon.cx, szIcon.cy, 0, NULL,
					 DI_NORMAL);

		rcCaption.left += (m_rcIconMargin.left + szIcon.cx + m_rcIconMargin.right);
	}
}

void CXTPFramePaintManager::DrawFrameCaptionButtons(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook,
													CRect& rcCaptionText)
{
	BOOL bActive = pFrameHook->IsFrameActive();

	CXTPControls* pCaptionButtons = pFrameHook->GetCaptionButtons();

	for (int i = 0; i < pCaptionButtons->GetCount(); i++)
	{
		CXTPControl* pControl = pCaptionButtons->GetAt(i);

		if (pControl->IsVisible())
		{
			DrawFrameCaptionButton(pDC, pControl->GetRect(), pControl->GetID(),
								   pControl->GetSelected(), pControl->GetPressed(),
								   pControl->GetEnabled() && bActive, pFrameHook->m_pRibbonBar);
			rcCaptionText.right = min(rcCaptionText.right, pControl->GetRect().left);
		}
	}
}

CXTPFrameShadow* CXTPFramePaintManager::CreateShadow(CWnd* pFrame)
{
	ASSERT(!XTPFrameShadowManager()->IsShadowEnabled(pFrame));
	UNUSED_ALWAYS(pFrame);
	return NULL;
}

void CXTPFramePaintManager::OnShadowedFrameActiveStateChanged(BOOL bActive, CWnd* pFrame,
															  CXTPFrameShadow* pShadow)
{
	UNUSED_ALWAYS(bActive);
	UNUSED_ALWAYS(pFrame);
	UNUSED_ALWAYS(pShadow);
}

CSize CXTPFramePaintManager::GetCaptionButtonSize(UINT nId, HWND hwnd) const
{
	UNUSED_ALWAYS(nId);

	if (GetWindowLong(hwnd, GWL_STYLE) & WS_CHILDWINDOW)
	{
		return m_szMDIClildWindowCaptionButtonSize;
	}

	return m_szCaptionButtonSize;
}
