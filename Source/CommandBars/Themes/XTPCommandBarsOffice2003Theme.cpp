// XTPCommandBarsOffice2003Theme.cpp : implementation of the CXTPCommandBarsOffice2003Theme class.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPWinThemeWrapper.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPDockBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPDialogBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"

#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"

// Message bar
#include "CommandBars/MessageBar/XTPMessageBarPaintManager.h"
#include "CommandBars/MessageBar/XTPMessageBarThemeOffice2003.h"

// Status bar
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOffice2003.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOffice2003Theme

IMPLEMENT_DYNAMIC(CXTPCommandBarsOffice2003Theme, CXTPCommandBarsOfficeXPTheme)

CXTPCommandBarsOffice2003Theme::CXTPCommandBarsOffice2003Theme()
{
	// Message bar paint manager
	SAFE_DELETE(m_pMessageBarPaintManager);
	m_pMessageBarPaintManager = new CXTPMessageBarThemeOffice2003(this);

	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeOffice2003(this);

	m_iconsInfo.bUseFadedIcons   = FALSE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	m_grcLunaChecked.SetStandardValue(RGB(255, 213, 140), RGB(255, 173, 85));
	m_grcLunaPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
	m_grcLunaSelected.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));

	m_bThemedStatusBar = TRUE;
	m_bPlainSeparators = FALSE;

	m_bFlatGripper = FALSE;

	m_nToolbarSeparatorMargin = XTP_DPI_X(4);
}

CXTPCommandBarsOffice2003Theme::~CXTPCommandBarsOffice2003Theme()
{
}

void CXTPCommandBarsOffice2003Theme::RefreshMetrics()
{
	CXTPCommandBarsOfficeXPTheme::RefreshMetrics();

	HWND hWnd = AfxGetMainWnd() ? AfxGetMainWnd()->GetSafeHwnd() : 0;
	m_themeStatusBar->OpenThemeData(hWnd, L"STATUS");

	m_clrMenuExpandedGripper.SetStandardValue(
		XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW),
									  GetXtremeColor(XPCOLOR_MENUBAR_EXPANDED), 550),
		GetXtremeColor(XPCOLOR_MENUBAR_EXPANDED));

	m_clrCommandBar.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
																   GetXtremeColor(COLOR_WINDOW),
																   0xcd),
									 GetXtremeColor(COLOR_3DFACE));
	m_clrDockBar.SetStandardValue(GetXtremeColor(COLOR_3DFACE),
								  XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
																GetXtremeColor(COLOR_WINDOW),
																0xcd));

	m_clrTearOffGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrToolbarShadow.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrFloatingGripper.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrPopupControl.SetStandardValue(GetXtremeColor(XPCOLOR_MENUBAR_FACE),
									   XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW),
																	 GetXtremeColor(COLOR_3DFACE),
																	 550));

	m_clrToolbarExpand.SetStandardValue(
		XTPColorManager()->LightColor(GetXtremeColor(XPCOLOR_3DFACE),
									  GetXtremeColor(COLOR_3DSHADOW), 51),
		GetXtremeColor(COLOR_3DSHADOW));

	m_bLunaTheme = FALSE;

	m_clrMenuExpand.SetStandardValue(GetXtremeColor(XPCOLOR_MENUBAR_FACE),
									 XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW),
																   GetXtremeColor(COLOR_3DFACE),
																   550));

	m_arrColor[XPCOLOR_SPLITTER_FACE].SetStandardValue(XTPColorManager()->grcDockBar.clrDark);

	if (XTPColorManager()->IsLowResolution())
	{
		m_clrCommandBar.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrDockBar.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrPopupControl.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrToolbarExpand.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	}

	switch (GetCurrentSystemTheme())
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
		{
			m_clrDockBar.SetStandardValue(RGB(158, 190, 245), RGB(196, 218, 250));
			m_clrToolbarShadow.SetStandardValue(RGB(59, 97, 156));
			m_clrCommandBar.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226), 0.75f);

			m_clrFloatingGripper.SetStandardValue(RGB(42, 102, 201));
			m_clrPopupControl.SetStandardValue(RGB(227, 239, 255), RGB(147, 181, 231));
			m_clrToolbarExpand.SetStandardValue(RGB(117, 166, 241), RGB(0, 53, 145), 0.75f);
			m_clrMenuExpandedGripper.SetStandardValue(RGB(203, 221, 246), RGB(121, 161, 220));

			m_clrMenuExpand.SetStandardValue(0xFEEFE3, 0xE2A981);

			m_pShadowManager->SetShadowColor(RGB(0x16, 0x1E, 0x36));
			m_clrTearOffGripper.SetStandardValue(RGB(169, 199, 240));

			m_clrDisabledIcon.SetStandardValue(RGB(233, 236, 242), RGB(97, 122, 172));

			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(203, 225, 252));
			m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(216, 231, 252));
			m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(158, 190, 245));
			m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(144, 153, 174));
			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(189, 211, 247));

			static const INT nElements[]	  = { XPCOLOR_TOOLBAR_GRIPPER,
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
			static const COLORREF clrValues[] = { RGB(39, 65, 118),
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
												  RGB(196, 219, 249),
												  0 };
			SetColors(sizeof(nElements) / sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

		case xtpSystemThemeOlive:
		{
			m_clrDockBar.SetStandardValue(RGB(217, 217, 167), RGB(242, 241, 228));
			m_clrCommandBar.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145), 0.3f);

			m_clrToolbarShadow.SetStandardValue(RGB(96, 128, 88));
			m_clrFloatingGripper.SetStandardValue(RGB(116, 134, 94));
			m_clrPopupControl.SetStandardValue(RGB(236, 240, 213), RGB(194, 206, 159));
			m_clrToolbarExpand.SetStandardValue(RGB(176, 194, 140), RGB(96, 119, 107), 0.3f);

			m_clrMenuExpandedGripper.SetStandardValue(RGB(230, 230, 209), RGB(164, 180, 120), 0.3f);

			m_clrMenuExpand.SetStandardValue(0xD5F0EC, 0x93C6B8);

			m_pShadowManager->SetShadowColor(RGB(0x1F, 0x22, 0x19));

			m_clrDisabledIcon.SetStandardValue(RGB(243, 244, 240), RGB(131, 144, 113));
			m_clrTearOffGripper.SetStandardValue(RGB(197, 212, 159));

			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(216, 227, 182));
			m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(226, 231, 191));
			m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(171, 192, 138));
			m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(151, 160, 123));
			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(222, 227, 189));

			static const INT nElements[]	  = { XPCOLOR_TOOLBAR_GRIPPER,
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
			static const COLORREF clrValues[] = { RGB(81, 94, 51),
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
												  RGB(209, 222, 173),
												  0 };
			SetColors(sizeof(nElements) / sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

		case xtpSystemThemeSilver:
		{
			m_clrDockBar.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));
			m_clrToolbarShadow.SetStandardValue(RGB(124, 124, 148));
			m_clrCommandBar.SetStandardValue(RGB(249, 249, 255), RGB(156, 155, 183), 0.75f);

			m_clrFloatingGripper.SetStandardValue(RGB(122, 121, 153));
			m_clrPopupControl.SetStandardValue(RGB(233, 231, 241), RGB(186, 185, 205));
			m_clrToolbarExpand.SetStandardValue(RGB(179, 178, 200), RGB(118, 116, 146), 0.75f);
			m_clrMenuExpandedGripper.SetStandardValue(RGB(215, 215, 226), RGB(133, 131, 162));

			m_clrMenuExpand.SetStandardValue(0xF1E8E8, 0xC5AEB0);

			m_pShadowManager->SetShadowColor(RGB(0x24, 0x20, 0x29));

			m_clrDisabledIcon.SetStandardValue(RGB(247, 245, 249), RGB(122, 121, 153));
			m_clrTearOffGripper.SetStandardValue(RGB(192, 192, 211));

			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(214, 211, 231));
			m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(223, 223, 234));
			m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(177, 176, 195));
			m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(155, 154, 179));
			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(214, 215, 231));

			static const INT nElements[]	  = { XPCOLOR_TOOLBAR_GRIPPER,
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
			static const COLORREF clrValues[] = { RGB(84, 84, 117),
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
												  RGB(219, 218, 228),
												  0 };
			SetColors(sizeof(nElements) / sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

		case xtpSystemThemeUnknown:
		case xtpSystemThemeAuto:
		case xtpSystemThemeDefault:
		default: break;
	}

	if (m_bLunaTheme)
	{
		m_arrColor[XPCOLOR_HIGHLIGHT_DISABLED_BORDER].SetStandardValue(RGB(141, 141, 141));
		m_arrColor[XPCOLOR_TOOLBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));
		m_arrColor[XPCOLOR_MENUBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));
		m_arrColor[XPCOLOR_FLOATBAR_BORDER].SetStandardValue(m_clrFloatingGripper);
	}

	m_clrMenuGripper.SetStandardValue(m_clrCommandBar);

	CreateGradientCircle();
}

void CXTPCommandBarsOffice2003Theme::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	if (pBar->GetPosition() == xtpBarTop && pBar->GetCommandBars()->IsDwmEnabled())
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), 0);
		return;
	}

	if (((CWnd*)pBar)->GetStyle() & CBRS_RIGHT)
		pDC->FillSolidRect(CXTPClientRect((CWnd*)pBar), m_clrDockBar.clrDark);
	else
	{
		CXTPClientRect rc((CWnd*)pBar);
		CRect rcFill(rc.left, rc.top, max(rc.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2),
					 rc.bottom);

		XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrDockBar, TRUE, rc);
	}
}

void CXTPCommandBarsOffice2003Theme::ExcludeCorners(CDC* pDC, CRect rc)
{
	pDC->ExcludeClipRect(rc.left, rc.top, rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1));
	pDC->ExcludeClipRect(rc.right - XTP_DPI_X(1), rc.top, rc.right, rc.top + XTP_DPI_Y(1));
	pDC->ExcludeClipRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.left + XTP_DPI_X(1), rc.bottom);
	pDC->ExcludeClipRect(rc.right - XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(1), rc.right, rc.bottom);
}

void CXTPCommandBarsOffice2003Theme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy,
														 BOOL bExpanded)
{
	XTPDrawHelpers()->GradientFill(pDC, CRect(CPoint(x, y), CSize(cx, cy)),
								   bExpanded ? m_clrMenuExpandedGripper : m_clrMenuGripper, TRUE);
}

BOOL CXTPCommandBarsOffice2003Theme::HasFloatingBarGradientEntry(CXTPCommandBar* pBar)
{
	return !IsFlatToolBar(pBar) && ((pBar->GetFlags() & xtpFlagAlignAny) != 0);
}

void CXTPCommandBarsOffice2003Theme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CXTPClientRect rc((CWnd*)pBar);

	if (pBar->GetPosition() == xtpBarPopup)
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);

		FillIntersectRect(pDC, (CXTPPopupBar*)pBar, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
	}
	else if (IsFlatToolBar(pBar))
	{
		CRect rcFill(rc.left, rc.top, max(rc.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2),
					 rc.bottom);
		if (IsDockingPosition(pBar->GetPosition()))
		{
			CWnd* pParent = pBar->GetParent();
			if (pParent)
			{
				CRect rcClient;
				pParent->GetWindowRect(rcClient);
				pBar->ScreenToClient(rcClient);
				rcFill = CRect(rcClient.left, rc.top,
							   rcClient.left
								   + max(rcClient.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2),
							   rc.bottom);
			}
		}
		XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrDockBar, TRUE, rc);

		if (pBar->GetPosition() == xtpBarFloating)
		{
			COLORREF clr = GetXtremeColor(XPCOLOR_FLOATBAR_BORDER);
			pDC->Draw3dRect(rc, clr, clr);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, clr, clr);

			pDC->SetPixel(XTP_DPI_X(2), XTP_DPI_Y(2), clr);
			pDC->SetPixel(rc.right - XTP_DPI_X(2), XTP_DPI_Y(2), clr);
			pDC->SetPixel(XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clr);
			pDC->SetPixel(rc.right - XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clr);
		}
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			CXTPWindowRect rcFill((CWnd*)pWnd);
			pBar->ScreenToClient(rcFill);
			rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
			XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrDockBar, TRUE, rc);

			pDC->ExcludeClipRect(rc.left, rc.top, rc.left + XTP_DPI_X(3), rc.bottom);
			ExcludeCorners(pDC, CRect(rc.left + XTP_DPI_X(3), rc.top + XTP_DPI_Y(1), rc.right,
									  rc.bottom - XTP_DPI_Y(1)));
		}

		CRect rcFill = rc;
		rcFill.top += XTP_DPI_Y(1);

		int nFirst = pBar->GetControls()->GetNext(-1, +1, FALSE, TRUE);
		if (nFirst != -1)
		{
			CRect rcRow   = pBar->GetControls()->GetAt(nFirst)->GetRowRect();
			rcFill.bottom = rcRow.bottom + XTP_DPI_Y(1);
		}
		XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrCommandBar, FALSE);
		pDC->FillSolidRect(rcFill.left, rcFill.bottom, rcFill.Width(),
						   rc.Height() - rcFill.Height() - XTP_DPI_Y(2), m_clrCommandBar.clrDark);

		pDC->FillSolidRect(rc.left + XTP_DPI_X(5), rc.bottom - XTP_DPI_Y(1),
						   rc.Width() - XTP_DPI_X(7), XTP_DPI_Y(1), m_clrToolbarShadow);

		pDC->SelectClipRgn(0);
	}
	else if (pBar->GetPosition() == xtpBarLeft || pBar->GetPosition() == xtpBarRight)
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			if (pWnd->GetStyle() & CBRS_RIGHT)
				pDC->FillSolidRect(rc, m_clrDockBar.clrDark);
			else
			{
				CXTPWindowRect rcFill((CWnd*)pWnd);
				pBar->ScreenToClient(rcFill);
				rcFill.right = rcFill.left
							   + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
				XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrDockBar, TRUE, rc);
			}

			pDC->ExcludeClipRect(rc.left, rc.top, rc.right, rc.top + XTP_DPI_Y(3));
			ExcludeCorners(pDC, CRect(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(3),
									  rc.right - XTP_DPI_X(1), rc.bottom));
		}

		CRect rcFill = rc;
		rcFill.left += XTP_DPI_X(1);

		int nFirst = pBar->GetControls()->GetNext(-1, +1, FALSE, TRUE);
		if (nFirst != -1)
		{
			CRect rcRow  = pBar->GetControls()->GetAt(nFirst)->GetRowRect();
			rcFill.right = rcRow.right + XTP_DPI_X(1);
		}
		XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrCommandBar, TRUE);
		pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top + XTP_DPI_Y(5), XTP_DPI_X(1),
						   rc.Height() - XTP_DPI_Y(7), m_clrToolbarShadow);

		pDC->SelectClipRgn(0);
	}
	else if (pBar->GetPosition() == xtpBarFloating && HasFloatingBarGradientEntry(pBar))
	{
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

		COLORREF clrBorder = GetXtremeColor(XPCOLOR_FLOATBAR_BORDER);
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, clrBorder, clrBorder);

		CXTPFontDC font(pDC, GetSmCaptionFont());
		CSize sz = pDC->GetTextExtent(_T(" "), 1);

		CRect rcFill(rc.left + XTP_DPI_X(2), rc.top + XTP_DPI_Y(2) + max(XTP_DPI_Y(15), sz.cy),
					 rc.right - XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2));

		for (int i = 0; i < pBar->GetControlCount(); i++)
		{
			CXTPControl* pControl = pBar->GetControl(i);

			if (pControl->GetWrap() && pControl->IsVisible()
				&& pControl->GetRect().top > rcFill.top)
			{
				CRect rcRow(rcFill.left, rcFill.top, rcFill.right, pControl->GetRowRect().top);
				XTPDrawHelpers()->GradientFill(pDC, rcRow, m_clrCommandBar, FALSE);

				rcFill.top = rcRow.bottom;
			}
		}

		XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrCommandBar, FALSE);

		pDC->SetPixel(XTP_DPI_X(2), XTP_DPI_Y(2), clrBorder);
		pDC->SetPixel(rc.right - XTP_DPI_X(2), XTP_DPI_Y(2), clrBorder);
		pDC->SetPixel(XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clrBorder);
		pDC->SetPixel(rc.right - XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2), clrBorder);
	}
	else
	{
		CXTPCommandBarsOfficeXPTheme::FillCommandBarEntry(pDC, pBar);
	}
}

CSize CXTPCommandBarsOffice2003Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
															  CXTPControl* pControl, BOOL bDraw)
{
	BOOL bSkipWrapSeparators = pBar->GetPosition() == xtpBarFloating
							   && pBar->GetType() != xtpBarTypePopup;

	if (!bDraw && bSkipWrapSeparators)
	{
		return CSize(XTP_DPI_X(6), 0);
	}

	if (!bDraw)
	{
		return CXTPCommandBarsOfficeXPTheme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRow		= pControl->GetRowRect();

	if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (!pControl->GetWrap())
		{
			Line(pDC, rcRow.left + m_nToolbarSeparatorMargin, rcControl.top - XTP_DPI_Y(4),
				 rcRow.right - m_nToolbarSeparatorMargin, rcControl.top - XTP_DPI_Y(4),
				 XPCOLOR_SEPARATOR);
			if (!m_bPlainSeparators)
			{
				Line(pDC, rcRow.left + m_nToolbarSeparatorMargin + XTP_DPI_X(1),
					 rcControl.top - XTP_DPI_Y(3),
					 rcRow.right - m_nToolbarSeparatorMargin + XTP_DPI_X(1),
					 rcControl.top - XTP_DPI_Y(3), COLOR_3DHIGHLIGHT);
			}
		}
		else
		{
			Line(pDC, rcRow.right + XTP_DPI_X(2), rcRow.top + m_nToolbarSeparatorMargin,
				 rcRow.right + XTP_DPI_X(2), rcRow.bottom, XPCOLOR_SEPARATOR);
			Line(pDC, rcRow.right + XTP_DPI_X(2) + 1,
				 rcRow.top + m_nToolbarSeparatorMargin + XTP_DPI_Y(1),
				 rcRow.right + XTP_DPI_X(2) + 1, rcRow.bottom, COLOR_3DHIGHLIGHT);
		}
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom
			 || bSkipWrapSeparators)
	{
		if (!pControl->GetWrap())
		{
			Line(pDC, rcControl.left - XTP_DPI_X(4), rcRow.top + m_nToolbarSeparatorMargin,
				 rcControl.left - XTP_DPI_X(4), rcRow.bottom - m_nToolbarSeparatorMargin,
				 XPCOLOR_SEPARATOR);
			if (!m_bPlainSeparators)
			{
				Line(pDC, rcControl.left - XTP_DPI_X(4) + 1,
					 rcRow.top + m_nToolbarSeparatorMargin + XTP_DPI_Y(1),
					 rcControl.left - XTP_DPI_X(4) + 1,
					 rcRow.bottom - m_nToolbarSeparatorMargin + XTP_DPI_Y(1), COLOR_3DHIGHLIGHT);
			}
		}
		else if (!bSkipWrapSeparators)
		{
			Line(pDC, rcRow.left + m_nToolbarSeparatorMargin, rcRow.top - XTP_DPI_Y(4), rcRow.right,
				 rcRow.top - XTP_DPI_Y(4), XPCOLOR_SEPARATOR);
			Line(pDC, rcRow.left + m_nToolbarSeparatorMargin + XTP_DPI_X(1),
				 rcRow.top - XTP_DPI_Y(4) + 1, rcRow.right, rcRow.top - XTP_DPI_Y(4) + 1,
				 COLOR_3DHIGHLIGHT);
		}
	}
	else
		return CXTPCommandBarsOfficeXPTheme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);

	return 0;
}

CSize CXTPCommandBarsOffice2003Theme::DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* pBar,
														   BOOL bDraw)
{
	CSize sz(XTP_DPI_X(8), max(XTP_DPI_Y(25), m_nTextHeight + XTP_DPI_Y(6)));

	if (pDC && bDraw)
	{
		CRect rcGripper;
		pBar->GetClientRect(&rcGripper);

		rcGripper.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
		rcGripper.bottom = rcGripper.top + sz.cy;

		if (pBar->IsActive())
		{
			if (m_bLunaTheme)
				XTPDrawHelpers()->GradientFill(pDC, rcGripper, m_grcLunaChecked, FALSE);
			else
				pDC->FillSolidRect(rcGripper, GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
		}
		else
		{
			XTPDrawHelpers()->GradientFill(pDC, rcGripper, m_clrCommandBar, FALSE);
		}

		for (int y = rcGripper.top + XTP_DPI_Y(4); y < rcGripper.bottom - XTP_DPI_Y(5);
			 y += XTP_DPI_Y(4))
		{
			pDC->FillSolidRect(CRect(XTP_DPI_X(4 + 2), y + XTP_DPI_Y(1), XTP_DPI_X(6 + 2),
									 y + XTP_DPI_Y(3)),
							   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(CRect(XTP_DPI_X(3 + 2), y, XTP_DPI_X(5 + 2), y + XTP_DPI_Y(2)),
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}

	return sz;
}

CSize CXTPCommandBarsOffice2003Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar,
															BOOL bDraw)
{
	if (bDraw)
		ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

	if (pBar->IsDialogBar())
		return DrawDialogBarGripper(pDC, (CXTPDialogBar*)pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarPopup && pBar->GetType() == xtpBarTypePopup)
	{
		return CXTPCommandBarsOfficeXPTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
	else if (pBar->GetPosition() == xtpBarFloating)
	{
		return CXTPCommandBarsOfficeXPTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
	else if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (!pDC || !bDraw)
			return CSize(0, XTP_DPI_Y(8) + XTP_DPI_Y(1));

		CSize szGripper(XTP_DPI_X(2), XTP_DPI_Y(2));
		CSize szGripper05(szGripper.cx / 2, szGripper.cy / 2);
		CSize szGripper2(szGripper.cx * 2, szGripper.cy * 2);
		int y1 = rc.top + szGripper2.cy + szGripper.cy;
		int y2 = y1 + szGripper05.cy;
		for (int x = rc.left + szGripper2.cy; x < rc.Width() - XTP_DPI_X(5); x += szGripper2.cx)
		{
			if (!m_bFlatGripper)
				pDC->FillSolidRect(x + szGripper05.cx, y2, szGripper.cx, szGripper.cy,
								   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(x, y1, szGripper.cx, szGripper.cy,
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (!pDC || !bDraw)
			return CSize(XTP_DPI_X(6), 0);

		CSize szGripper(XTP_DPI_X(2), XTP_DPI_Y(2));
		CSize szGripper05(szGripper.cx / 2, szGripper.cy / 2);
		CSize szGripper2(szGripper.cx * 2, szGripper.cy * 2);
		int x1 = rc.left + szGripper2.cx + szGripper.cx;
		int x2 = x1 + szGripper05.cx;
		for (int y = rc.top + XTP_DPI_Y(5); y < rc.Height() - XTP_DPI_Y(7); y += szGripper2.cy)
		{
			if (!m_bFlatGripper)
				pDC->FillSolidRect(x2, y + szGripper05.cy, szGripper.cx, szGripper.cy,
								   GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(x1, y, szGripper.cx, szGripper.cy,
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}

	return 0;
}

void CXTPCommandBarsOffice2003Theme::RectangleEx(CDC* pDC, CRect rc, int nColorBorder, BOOL bHoriz,
												 CXTPPaintManagerColorGradient& color)
{
	GradientFill(pDC, rc, color.clrLight, color.clrDark, bHoriz);
	Draw3dRect(pDC, rc, nColorBorder, nColorBorder);
}

void CXTPCommandBarsOffice2003Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
												   BOOL bPressed, BOOL bEnabled, BOOL bChecked,
												   BOOL bPopuped, XTPBarType barType,
												   XTPBarPosition barPosition)
{
	if (bPopuped)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_clrPopupControl, FALSE);
		Draw3dRect(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_BORDER);
	}
	else if (m_bLunaTheme && barType != xtpBarTypePopup
			 && (IsDockingPosition(barPosition) || (barPosition == xtpBarFloating)))
	{
		BOOL bHoriz = IsVerticalPosition(barPosition);
		if (!bEnabled)
		{
			if (IsKeyboardSelected(bSelected))
				RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, bHoriz,
							bChecked ? m_grcLunaPushed : m_grcLunaSelected);
			else if (bChecked)
				RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_DISABLED_BORDER, bHoriz, m_grcLunaChecked);
			return;
		}
		else if (bChecked == 2 && !bSelected && !bPressed)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_3DFACE);
		}
		else if (bChecked && !bSelected && !bPressed)
			RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, bHoriz, m_grcLunaChecked);
		else if (bChecked && bSelected && !bPressed)
			RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, bHoriz, m_grcLunaPushed);
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed))
			RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, bHoriz, m_grcLunaPushed);
		else if (bSelected || bPressed)
			RectangleEx(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, bHoriz, m_grcLunaSelected);
	}
	else
	{
		CXTPCommandBarsOfficeXPTheme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled,
													bChecked, bPopuped, barType, barPosition);
	}
}

void CXTPCommandBarsOffice2003Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected,
												   BOOL bPressed, BOOL bEnabled, BOOL bChecked,
												   BOOL bPopuped, XTPBarType barType,
												   XTPBarPosition barPosition, int nID)
{
	CXTPCommandBarsOfficeXPTheme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked,
												bPopuped, barType, barPosition, nID);
}

BOOL CXTPCommandBarsOffice2003Theme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	return CXTPCommandBarsOfficeXPTheme::DrawRectangle(pDC, pButton);
}

void CXTPCommandBarsOffice2003Theme::DrawExpandSymbols(CDC* pDC, BOOL bVertical, CRect rc,
													   BOOL bHiddenExists, COLORREF clr)
{
	rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));

	if (!bVertical)
	{
		CRect rcArrow = rc;
		rcArrow.top   = rc.bottom - rcArrow.Width();
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEjectDown, rcArrow, clr);

		if (bHiddenExists)
		{
			rcArrow		   = rc;
			rcArrow.bottom = rc.top + rcArrow.Width();
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolRewindRight, rcArrow, clr);
		}
	}
	else
	{
		CRect rcArrow = rc;
		rcArrow.left  = rc.right - rcArrow.Height();
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEjectRight, rcArrow, clr);

		if (bHiddenExists)
		{
			rcArrow		  = rc;
			rcArrow.right = rc.left + rcArrow.Height();
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolRewindDown, rcArrow, clr);
		}
	}
}

CSize CXTPCommandBarsOffice2003Theme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType,
														 CXTPControl* pButton, CXTPCommandBar* pBar,
														 BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!IsVerticalPosition(pBar->GetPosition()))
				pButton->SetRect(CRect(szBar.cx - XTP_DPI_X(11) - XTP_DPI_X(1), XTP_DPI_Y(1),
									   szBar.cx - 0, szBar.cy - 0));
			else
				pButton->SetRect(
					CRect(0, szBar.cy - XTP_DPI_Y(11) - XTP_DPI_Y(1), szBar.cx - 0, szBar.cy - 0));
		}
		else
		{
			CRect rc = pButton->GetRect();
			if (!IsVerticalPosition(pBar->GetPosition()))
			{
				pDC->ExcludeClipRect(rc.right - XTP_DPI_X(1), rc.top, rc.right,
									 rc.top + XTP_DPI_Y(1));
				pDC->ExcludeClipRect(rc.right - XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(1), rc.right,
									 rc.bottom);
				pDC->ExcludeClipRect(rc.left, rc.top + XTP_DPI_Y(1), rc.left + XTP_DPI_X(1),
									 rc.bottom - XTP_DPI_Y(1));
			}
			else
			{
				pDC->ExcludeClipRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.left + XTP_DPI_X(1),
									 rc.bottom);
				pDC->ExcludeClipRect(rc.right - XTP_DPI_X(1), rc.bottom - XTP_DPI_Y(1), rc.right,
									 rc.bottom);
				pDC->ExcludeClipRect(rc.left + XTP_DPI_X(1), rc.top, rc.right - XTP_DPI_X(1),
									 rc.top + XTP_DPI_Y(1));
			}

			if (pButton->GetPopuped())
			{
				if (m_bLunaTheme)
					GradientFill(pDC, rc, m_grcLunaPushed.clrLight, m_grcLunaPushed.clrDark,
								 IsVerticalPosition(pBar->GetPosition()));
				else
					pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
			}
			else if (pButton->GetSelected())
			{
				if (m_bLunaTheme)
					GradientFill(pDC, rc, m_grcLunaSelected.clrLight, m_grcLunaSelected.clrDark,
								 IsVerticalPosition(pBar->GetPosition()));
				else
					pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT));
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, m_clrToolbarExpand,
											   IsVerticalPosition(pBar->GetPosition()));
			}

			COLORREF clrText = GetXtremeColor(
				UINT(pButton->GetPopuped()
						 ? XPCOLOR_TOOLBAR_TEXT
						 : pButton->GetSelected() ? XPCOLOR_HIGHLIGHT_TEXT : XPCOLOR_TOOLBAR_TEXT));
			BOOL bHiddenExists = *(BOOL*)lpParam;

			DrawExpandSymbols(pDC, IsVerticalPosition(pBar->GetPosition()),
							  CRect(CPoint(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1)),
									rc.Size()),
							  bHiddenExists, GetXtremeColor(COLOR_BTNHIGHLIGHT));
			DrawExpandSymbols(pDC, IsVerticalPosition(pBar->GetPosition()), rc, bHiddenExists,
							  clrText);

			pDC->SelectClipRgn(NULL);
		}
		return 0;
	}
	if (controlType == xtpButtonExpandMenu)
	{
		if (bDraw)
		{
			CXTPPaintManager::DrawControlEntry(pDC, pButton);
			CRect rcButton  = pButton->GetRect();
			CPoint ptCenter = rcButton.CenterPoint();

			IMAGEINFO imgInfo;
			m_ilGradientCircle.GetImageInfo(0, &imgInfo);
			CSize szImg(imgInfo.rcImage.right - imgInfo.rcImage.left,
						imgInfo.rcImage.bottom - imgInfo.rcImage.top);
			CRect rcImg(CPoint(ptCenter.x - szImg.cx / 2, ptCenter.y - szImg.cy / 2), szImg);
			m_ilGradientCircle.Draw(pDC, 0, rcImg.TopLeft(), ILD_NORMAL);

			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolShiftDown, rcImg, 0);
		}

		return CSize(0, XTP_DPI_Y(18));
	}

	return CXTPCommandBarsOfficeXPTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw,
															lpParam);
}

void CXTPCommandBarsOffice2003Theme::CreateGradientCircle()
{
	const BYTE clrMask[16 * 16] =
		{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xEA, 0xC6, 0xB4, 0xB4, 0xB4, 0xC6, 0xFC, 0xFF, 0xFF, 0xFF,
		  0xFF, 0xFF, 0xFF, 0xFF, 0xEA, 0xEA, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x7E, 0xC6, 0xFF,
		  0xFF, 0xFF, 0xFF, 0xFF, 0xEA, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x6C, 0x5A,
		  0x90, 0xFF, 0xFF, 0xFF, 0xEA, 0xFC, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E, 0x6C,
		  0x5A, 0x36, 0x7E, 0xFF, 0xEA, 0xFC, 0xFC, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90, 0x7E,
		  0x6C, 0x5A, 0x36, 0x24, 0xEA, 0xEA, 0xFC, 0xFC, 0xFC, 0xFC, 0xEA, 0xEA, 0xC6, 0xB4, 0x90,
		  0x7E, 0x6C, 0x48, 0x36, 0x24, 0x5A, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xC6, 0xB4, 0x90,
		  0x90, 0x7E, 0x6C, 0x48, 0x24, 0x12, 0x24, 0xC6, 0xEA, 0xEA, 0xEA, 0xC6, 0xC6, 0xC6, 0xB4,
		  0x90, 0x7E, 0x7E, 0x48, 0x36, 0x24, 0x12, 0x12, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xB4, 0xB4,
		  0x90, 0x7E, 0x7E, 0x6C, 0x48, 0x36, 0x12, 0x12, 0x00, 0xC6, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4,
		  0x90, 0x7E, 0x7E, 0x6C, 0x5A, 0x36, 0x24, 0x12, 0x00, 0x00, 0xC6, 0x90, 0x90, 0x90, 0x90,
		  0x90, 0x7E, 0x7E, 0x6C, 0x6C, 0x48, 0x36, 0x24, 0x12, 0x00, 0x36, 0xFC, 0x90, 0x7E, 0x7E,
		  0x7E, 0x7E, 0x6C, 0x6C, 0x5A, 0x5A, 0x36, 0x24, 0x12, 0x00, 0x00, 0xB4, 0xFF, 0xC6, 0x6C,
		  0x6C, 0x6C, 0x6C, 0x5A, 0x5A, 0x5A, 0x48, 0x36, 0x24, 0x12, 0x00, 0x48, 0xFF, 0xFF, 0xFF,
		  0xB4, 0x6C, 0x5A, 0x5A, 0x5A, 0x48, 0x48, 0x36, 0x24, 0x12, 0x00, 0x36, 0xFF, 0xFF, 0xFF,
		  0xFF, 0xFF, 0xC6, 0x5A, 0x48, 0x48, 0x48, 0x36, 0x24, 0x24, 0x12, 0x6C, 0xFF, 0xFF, 0xFF,
		  0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xB4, 0x6C, 0x48, 0x36, 0x5A, 0x7E, 0xEA, 0xFF, 0xFF, 0xFF,
		  0xFF };

	m_ilGradientCircle.DeleteImageList();

	COLORREF clrLight = m_clrMenuExpand.clrLight;
	COLORREF clrDark  = m_clrMenuExpand.clrDark;

	CWindowDC dc(CWnd::GetDesktopWindow());

	CDC dcCircle;
	dcCircle.CreateCompatibleDC(&dc);
	CBitmap bmp;
	VERIFY(bmp.CreateCompatibleBitmap(&dc, 16, 16));
	CBitmap* pOldBitmap = (CBitmap*)dcCircle.SelectObject(&bmp);

	for (int i = 0; i < 16 * 16; i++)
	{
		int n = clrMask[i];

		int nRed   = GetRValue(clrDark) - n * (GetRValue(clrDark) - GetRValue(clrLight)) / 0xFF;
		int nGreen = GetGValue(clrDark) - n * (GetGValue(clrDark) - GetGValue(clrLight)) / 0xFF;
		int nBlue  = GetBValue(clrDark) - n * (GetBValue(clrDark) - GetBValue(clrLight)) / 0xFF;
		dcCircle.SetPixel(i / 16, i % 16, n == 0xFF ? RGB(0, 0xFF, 0) : RGB(nRed, nGreen, nBlue));
	}

	dcCircle.SelectObject(pOldBitmap);

	m_ilGradientCircle.Create(16, 16, ILC_COLOR24 | ILC_MASK, 1, 0);
	m_ilGradientCircle.Add(&bmp, RGB(0, 0xFF, 0));

	ASSERT(m_ilGradientCircle.GetImageCount() == 1);
}

#define RGB_TO_RGBQUAD(r, g, b) (RGB(b, g, r))

HBITMAP CXTPCommandBarsOffice2003Theme::GetModernBitmap(UINT nIDResource)
{
	static const struct
	{
		DWORD rgbqFrom;
		DWORD rgbqTo;
	} modernColorMap[] = { { RGB_TO_RGBQUAD(0, 128, 0), RGB_TO_RGBQUAD(0, 151, 94) },
						   { RGB_TO_RGBQUAD(255, 0, 0), RGB_TO_RGBQUAD(255, 87, 42) },
						   { RGB_TO_RGBQUAD(128, 0, 0), RGB_TO_RGBQUAD(213, 79, 74) },
						   { RGB_TO_RGBQUAD(0, 0, 255), RGB_TO_RGBQUAD(58, 123, 252) },
						   { RGB_TO_RGBQUAD(0, 0, 128), RGB_TO_RGBQUAD(1, 81, 205) },
						   { RGB_TO_RGBQUAD(128, 128, 128), RGB_TO_RGBQUAD(149, 147, 145) },
						   { RGB_TO_RGBQUAD(128, 128, 0), RGB_TO_RGBQUAD(228, 167, 8) },
						   { RGB_TO_RGBQUAD(192, 192, 192), RGB_TO_RGBQUAD(205, 198, 187) },
						   { RGB_TO_RGBQUAD(255, 255, 0), RGB_TO_RGBQUAD(255, 218, 33) } };

	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);
	ASSERT(lpszResourceName != NULL);
	if (!lpszResourceName)
		return NULL;

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	if (!hInst)
		return NULL;

	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_BITMAP);
	if (hRsrc == NULL)
		return NULL;

	HGLOBAL hglb = LoadResource(hInst, hRsrc);
	if (hglb == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
	if (lpBitmap == NULL)
		return NULL;

	// make copy of BITMAPINFOHEADER so we can modify the color table
	const int nColorTableSize		= 16;
	UINT nSize						= lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)::malloc(nSize);
	if (lpBitmapInfo == NULL)
		return NULL;

	MEMCPY_S(lpBitmapInfo, lpBitmap, nSize);

	// color table is in RGBQUAD DIB format
	DWORD* pColorTable = (DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);

	COLORREF clrMask = XTPImageManager()->GetMaskColor();

	for (int iColor = 0; iColor < nColorTableSize; iColor++)
	{
		// look for matching RGBQUAD color in original
		for (int i = 0; i < _countof(modernColorMap); i++)
		{
			if (pColorTable[iColor] == modernColorMap[i].rgbqFrom && clrMask != pColorTable[iColor])
			{
				pColorTable[iColor] = modernColorMap[i].rgbqTo;
				break;
			}
		}
	}

	int nWidth	= (int)lpBitmapInfo->biWidth;
	int nHeight   = (int)lpBitmapInfo->biHeight;
	HDC hDCScreen = ::GetDC(NULL);
	HBITMAP hbm   = ::CreateCompatibleBitmap(hDCScreen, nWidth, nHeight);

	if (hbm != NULL)
	{
		HDC hDCGlyphs  = ::CreateCompatibleDC(hDCScreen);
		HBITMAP hbmOld = (HBITMAP)::SelectObject(hDCGlyphs, hbm);

		LPBYTE lpBits;
		lpBits = (LPBYTE)(lpBitmap + 1);
		lpBits += ((size_t)1 << (lpBitmapInfo->biBitCount)) * sizeof(RGBQUAD);

		StretchDIBits(hDCGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight, lpBits,
					  (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		SelectObject(hDCGlyphs, hbmOld);
		::DeleteDC(hDCGlyphs);
	}
	::ReleaseDC(NULL, hDCScreen);

	// free copy of bitmap info struct and resource itself
	::free(lpBitmapInfo);
	::FreeResource(hglb);

	return hbm;
}

BOOL CXTPCommandBarsOffice2003Theme::LoadModernToolbarIcons(UINT nIDResource)
{
	HBITMAP hbm = GetModernBitmap(nIDResource);
	if (!hbm)
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hbm);

	return XTPImageManager()->SetIcons(nIDResource, bmp);
}

BOOL CXTPCommandBarsOffice2003Theme::LoadModernToolbarIcons(UINT nIDResource, UINT* pCommands,
															int nCount, CSize szIcon)
{
	HBITMAP hbm = GetModernBitmap(nIDResource);
	if (!hbm)
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hbm);

	return XTPImageManager()->SetIcons(bmp, pCommands, nCount, szIcon);
}

CRect CXTPCommandBarsOffice2003Theme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
		return CRect(XTP_DPI_X(4), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
	else
		return CXTPCommandBarsOfficeXPTheme::GetCommandBarBorders(pBar);
}

CSize CXTPCommandBarsOffice2003Theme::DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected,
														 BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc(XTP_DPI_X(1), XTP_DPI_Y(1), rcGripper.right + XTP_DPI_X(1),
				 rcGripper.Height() + XTP_DPI_Y(1));
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		rc.DeflateRect(1, 1);
		int nLeft  = rc.CenterPoint().x - min(XTP_DPI_X(32), rcGripper.Width() / 2 - XTP_DPI_X(10));
		int nRight = rc.CenterPoint().x + min(XTP_DPI_X(32), rcGripper.Width() / 2 - XTP_DPI_X(10));

		if (bSelected)
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
		else
			pDC->FillSolidRect(rc, m_clrTearOffGripper);

		for (int x = nLeft; x < nRight; x += XTP_DPI_X(4))
		{
			pDC->FillSolidRect(CRect(x + XTP_DPI_X(1), XTP_DPI_Y(4 + 1), x + XTP_DPI_X(3),
									 XTP_DPI_Y(6 + 1)),
							   GetXtremeColor(COLOR_BTNHIGHLIGHT));
			pDC->FillSolidRect(CRect(x, XTP_DPI_Y(3 + 1), x + XTP_DPI_X(2), XTP_DPI_Y(5 + 1)),
							   GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}

	return CSize(0, XTP_DPI_Y(9));
}
