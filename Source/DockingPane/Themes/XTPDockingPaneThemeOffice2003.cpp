// XTPDockingPaneThemeOffice2003.cpp : implementation of the CXTPDockingPaneOffice2003Theme class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/XTPDockingPaneMiniWnd.h"
#include "DockingPane/XTPDockingPaneSidePanel.h"
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2003Theme

CXTPDockingPaneOffice2003Theme::CXTPDockingPaneOffice2003Theme()
{
	m_bDrawGripper		  = TRUE;
	m_bRoundedCaption	 = TRUE;
	m_nCaptionFontGap	 = XTP_DPI_X(7);
	m_bUseBoldCaptionFont = TRUE;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_pPanelPaintManager->SetColor(xtpTabColorOffice2003);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_pTabPaintManager->SetColor(xtpTabColorOffice2003);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice2003;
}

CXTPDockingPaneOffice2003Theme::~CXTPDockingPaneOffice2003Theme()
{
}

void CXTPDockingPaneOffice2003Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics();

	m_clrNormalCaption.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
																	  GetXtremeColor(COLOR_WINDOW),
																	  0xcd),
										GetXtremeColor(COLOR_3DFACE));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_bLunaTheme = FALSE;

	if (XTPColorManager()->IsLowResolution())
	{
		m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
		m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	}

	XTPCurrentSystemTheme systemTheme = GetCurrentSystemTheme();

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
		{
			m_clrNormalCaption.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226), 0.75f);
			m_clrSplitter.SetStandardValue(RGB(216, 231, 252));
			m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			INT nElements[]		 = { XPCOLOR_TOOLBAR_GRIPPER,
								 XPCOLOR_SEPARATOR,
								 XPCOLOR_DISABLED,
								 XPCOLOR_MENUBAR_FACE,
								 XPCOLOR_MENUBAR_BORDER,
								 XPCOLOR_HIGHLIGHT,
								 XPCOLOR_HIGHLIGHT_BORDER,
								 XPCOLOR_HIGHLIGHT_PUSHED_BORDER,
								 XPCOLOR_HIGHLIGHT_CHECKED_BORDER,
								 XPCOLOR_HIGHLIGHT_PUSHED,
								 XPCOLOR_HIGHLIGHT_CHECKED,
								 XPCOLOR_TOOLBAR_FACE,
								 XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(39, 65, 118),
									 RGB(106, 140, 203),
									 RGB(109, 150, 208),
									 RGB(246, 246, 246),
									 RGB(0, 45, 150),
									 RGB(255, 238, 194),
									 RGB(0, 0, 128),
									 RGB(0, 0, 128),
									 RGB(0, 0, 128),
									 RGB(254, 128, 62),
									 RGB(255, 192, 111),
									 RGB(158, 190, 245),
									 0 };
			SetColors(sizeof(nElements) / sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

		case xtpSystemThemeOlive:
		{
			m_clrNormalCaption.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145), 0.3f);
			m_clrSplitter.SetStandardValue(RGB(226, 231, 191));
			m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			INT nElements[]		 = { XPCOLOR_TOOLBAR_GRIPPER,
								 XPCOLOR_SEPARATOR,
								 XPCOLOR_DISABLED,
								 XPCOLOR_MENUBAR_FACE,
								 XPCOLOR_MENUBAR_BORDER,
								 XPCOLOR_HIGHLIGHT,
								 XPCOLOR_HIGHLIGHT_BORDER,
								 XPCOLOR_HIGHLIGHT_CHECKED_BORDER,
								 XPCOLOR_HIGHLIGHT_PUSHED_BORDER,
								 XPCOLOR_HIGHLIGHT_PUSHED,
								 XPCOLOR_HIGHLIGHT_CHECKED,
								 XPCOLOR_TOOLBAR_FACE,
								 XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(81, 94, 51),
									 RGB(96, 128, 88),
									 RGB(159, 174, 122),
									 RGB(244, 244, 238),
									 RGB(117, 141, 94),
									 RGB(255, 238, 194),
									 RGB(63, 93, 56),
									 RGB(63, 93, 56),
									 RGB(63, 93, 56),
									 RGB(254, 128, 62),
									 RGB(255, 192, 111),
									 RGB(217, 217, 167),
									 0 };
			SetColors(sizeof(nElements) / sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

		case xtpSystemThemeSilver:
		{
			m_clrNormalCaption.SetStandardValue(RGB(249, 249, 255), RGB(156, 155, 183), 0.75f);
			m_clrSplitter.SetStandardValue(RGB(223, 223, 234));
			m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			INT nElements[]		 = { XPCOLOR_TOOLBAR_GRIPPER,
								 XPCOLOR_SEPARATOR,
								 XPCOLOR_DISABLED,
								 XPCOLOR_MENUBAR_FACE,
								 XPCOLOR_MENUBAR_BORDER,
								 XPCOLOR_HIGHLIGHT,
								 XPCOLOR_HIGHLIGHT_BORDER,
								 XPCOLOR_HIGHLIGHT_CHECKED_BORDER,
								 XPCOLOR_HIGHLIGHT_PUSHED_BORDER,
								 XPCOLOR_HIGHLIGHT_PUSHED,
								 XPCOLOR_HIGHLIGHT_CHECKED,
								 XPCOLOR_TOOLBAR_FACE,
								 XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(84, 84, 117),
									 RGB(110, 109, 143),
									 RGB(168, 167, 190),
									 RGB(253, 250, 255),
									 RGB(124, 124, 148),
									 RGB(255, 238, 194),
									 RGB(75, 75, 111),
									 RGB(75, 75, 111),
									 RGB(75, 75, 111),
									 RGB(254, 128, 62),
									 RGB(255, 192, 111),
									 RGB(215, 215, 229),
									 0 };
			SetColors(sizeof(nElements) / sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;
	}

	if (m_bLunaTheme)
	{
		m_clrActiveCaption.SetStandardValue(RGB(255, 213, 140), RGB(255, 166, 76));
		m_clrSplitterGradient.SetStandardValue(XTPColorManager()->grcDockBar);
	}
}

CRect CXTPDockingPaneOffice2003Theme::GetCaptionGripperRect(
	const CXTPDockingPaneTabbedContainer* /*pPane*/)
{
	return CRect(0, 0, XTP_DPI_X(10), m_nTitleHeight);
}

CRect CXTPDockingPaneOffice2003Theme::GetPaneCaptionRect(const CXTPDockingPaneSidePanel* pPane)
{
	CXTPClientRect rect(pPane);
	rect.DeflateRect(1, 1);

	int nTitleFont					  = GetCaptionHeight() + XTP_DPI_Y(2);
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft)
		rect.right = rect.left + nTitleFont;
	else if (direction == xtpPaneDockRight)
		rect.left = rect.right - nTitleFont;
	else if (direction == xtpPaneDockTop)
		rect.bottom = rect.top + nTitleFont;
	else if (direction == xtpPaneDockBottom)
		rect.top = rect.bottom - nTitleFont;

	return rect;
}

void CXTPDockingPaneOffice2003Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane,
													 CRect rcCaption, const CString& strTitle,
													 BOOL bActive, BOOL bVertical)
{
	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, XTP_DPI_X(4), 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, XTP_DPI_X(16), 0,
					   bVertical);

	if (!bVertical && m_bDrawGripper && (rcCaption.right > rcCaption.left + XTP_DPI_X(7)))
	{
		CSize szGripper(XTP_DPI_X(2), XTP_DPI_Y(2));
		CSize szGripper05(szGripper.cx / 2, szGripper.cy / 2);
		CSize szGripper2(szGripper.cx * 2, szGripper.cy * 2);
		int x1 = rcCaption.left + szGripper2.cx + szGripper.cx;
		int x2 = x1 + szGripper05.cx;
		for (int y = rcCaption.top + XTP_DPI_Y(5); y < rcCaption.bottom - XTP_DPI_Y(5);
			 y += szGripper2.cy)
		{
			pDC->FillSolidRect(x2, y + szGripper05.cy, szGripper.cx, szGripper.cy,
							   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(x1, y, szGripper.cx, szGripper.cy,
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
		rcCaption.left += XTP_DPI_X(8);
	}

	if (bVertical && m_bDrawGripper && (rcCaption.bottom > rcCaption.top + XTP_DPI_Y(7)))
	{
		CSize szGripper(XTP_DPI_X(2), XTP_DPI_Y(2));
		CSize szGripper05(szGripper.cx / 2, szGripper.cy / 2);
		CSize szGripper2(szGripper.cx * 2, szGripper.cy * 2);
		int y1 = rcCaption.top + szGripper2.cy + szGripper.cy;
		int y2 = y1 + szGripper05.cy;
		for (int x = rcCaption.left + szGripper2.cy; x < rcCaption.right - XTP_DPI_X(5);
			 x += szGripper2.cx)
		{
			pDC->FillSolidRect(x + szGripper05.cx, y2, szGripper.cx, szGripper.cy,
							   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(x, y1, szGripper.cx, szGripper.cy,
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
		rcCaption.top += XTP_DPI_Y(8);
	}

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText
											  : m_clrDisabledCaptionText.operator COLORREF());

	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(6), XTP_DPI_Y(1), XTP_DPI_X(2), 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneOffice2003Theme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane,
													   CRect rc)
{
	COLORREF clrFrame = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);

	pDC->FillSolidRect(rc, clrFrame);
	pDC->Draw3dRect(rc, clrFrame, clrFrame);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, clrFrame, clrFrame);
	rc.DeflateRect(1, 1);

	XTPDrawHelpers()->ExcludeCorners(pDC, rc);
	pDC->Draw3dRect(rc, m_clrNormalCaption.clrDark, m_clrNormalCaption.clrDark);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, pPane->IsCaptionVertical());
	if (NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(&dcCache, rcCaption,
									   bActive ? m_clrActiveCaption : m_clrNormalCaption, FALSE);
	}
	else
	{
		dcCache.FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	dcCache.SetPixel(rcCaption.left, rcCaption.bottom, m_clrNormalCaption.clrDark);
	dcCache.SetPixel(rcCaption.right - XTP_DPI_X(1), rcCaption.bottom, m_clrNormalCaption.clrDark);

	rcCaption.DeflateRect(XTP_DPI_X(1), 0);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneOffice2003Theme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane,
												   CRect rc)
{
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	pDC->FillSolidRect(rc, bActive ? m_clrActiveCaption : m_clrNormalCaption);
	pDC->SetBkMode(TRANSPARENT);

	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));
	rc.DeflateRect(1, 1);

	CRect rcCaption					  = rc;
	int nTitleFont					  = GetCaptionHeight() + XTP_DPI_Y(2);
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft)
		rcCaption.right = rcCaption.left + nTitleFont;
	else if (direction == xtpPaneDockRight)
		rcCaption.left = rcCaption.right - nTitleFont;
	else if (direction == xtpPaneDockTop)
		rcCaption.bottom = rcCaption.top + nTitleFont;
	else if (direction == xtpPaneDockBottom)
		rcCaption.top = rcCaption.bottom - nTitleFont;

	BOOL bVertical = direction == xtpPaneDockLeft || direction == xtpPaneDockRight;

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcCaption,
									   bActive ? m_clrActiveCaption : m_clrNormalCaption,
									   bVertical);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

void CXTPDockingPaneOffice2003Theme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane,
												 CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - XTP_DPI_X(3), 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));
	}

	CRect rcFill(rcCaption);

	if (m_bRoundedCaption)
	{
		XTPDeflateRectDirection(rcFill, XTP_DPI_X(1), 0, 0, 0, bVertical);
		pDC->FillSolidRect(rcCaption, m_clrSplitter);

		XTPDrawHelpers()->ExcludeCorners(pDC, rcFill);
	}

	BOOL bActive		= m_bHighlightActiveCaption && pPane->IsActive();
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcFill,
									   bActive ? m_clrActiveCaption : m_clrNormalCaption,
									   bVertical);
	}
	else
	{
		pDC->FillRect(rcFill, CBrush::FromHandle(hCustomBrush));
	}

	if (!m_bRoundedCaption)
	{
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(),
						   GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1,
						   GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.right - 1, rcCaption.top, 1, rcCaption.Height(),
						   GetXtremeColor(XPCOLOR_FRAME));
	}

	CString strTitle = pPane->GetTitle();
	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
}
