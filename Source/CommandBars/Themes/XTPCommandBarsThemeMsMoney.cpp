// XTPCommandBarsThemeMsMoney.cpp: implementation of the CXTPCommandBarsThemeMsMoney class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2000Theme.h"
#include "CommandBars/Themes/XTPCommandBarsThemeMsMoney.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPCommandBarsThemeMsMoney::CXTPCommandBarsThemeMsMoney(COLORREF clrGradientLight,
														 COLORREF clrGradientDark,
														 COLORREF clrLineLight,
														 COLORREF clrLineDark)
{
	m_clrGradientLight = clrGradientLight;
	m_clrGradientDark  = clrGradientDark;
	m_clrLineLight	 = clrLineLight;
	m_clrLineDark	  = clrLineDark;
}

CXTPCommandBarsThemeMsMoney::~CXTPCommandBarsThemeMsMoney()
{
}

void CXTPCommandBarsThemeMsMoney::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CXTPClientRect rc(pBar);

	if (pBar->GetType() == xtpBarTypeMenuBar)
	{
		pDC->FillSolidRect(rc, RGB(91, 91, 91));
	}
	else if (pBar->GetType() == xtpBarTypeNormal)
	{
		GradientFill(pDC, rc, m_clrGradientLight, m_clrGradientDark, FALSE);

		CXTPPenDC pen(*pDC, 0);

		for (int i = rc.top + 1; i < rc.bottom; i += 3)
		{
			pDC->FillSolidRect(rc.left, i, rc.Width(), 1,
							   XTPDrawHelpers()->BlendColors(
								   m_clrLineLight, m_clrLineDark,
								   static_cast<float>(1.0
													  - (static_cast<float>(i - rc.top))
															/ static_cast<float>(rc.Height()))));
		}
	}
	else if (pBar->GetType() == xtpBarTypePopup)
	{
		pDC->FillSolidRect(rc, RGB(91, 91, 91));
		pDC->Draw3dRect(rc, RGB(68, 68, 68), RGB(68, 68, 68));
	}
}

CSize CXTPCommandBarsThemeMsMoney::DrawControlPopupParent(CDC* pDC, CXTPControl* pButton,
														  BOOL bDraw)
{
	CRect rcButton			   = pButton->GetRect();
	XTPControlType controlType = pButton->GetType();
	CRect rcText(0, 0, 0, 0), rcShortcut(0, 0, 0, 0);
	BOOL bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(),
		 bChecked = pButton->GetChecked();

	ASSERT(controlType == xtpControlButton || controlType == xtpControlPopup); // implemented only.

	if (bDraw)
	{
		CRect rcSelection(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom);

		COLORREF clrText = !bEnabled ? RGB(145, 145, 145)
									 : bSelected ? RGB(255, 223, 127) : RGB(255, 254, 249);
		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);
		if (bSelected)
			pDC->FillSolidRect(rcSelection, RGB(80, 80, 80));

		if (bChecked)
		{
			CRect rcCheck(CPoint(rcButton.left - XTP_DPI_X(2),
								 rcButton.top + rcButton.Height() / 2 - XTP_DPI_Y(10)),
						  XTP_DPI(CSize(20, 20)));
			DrawCheckMark(pDC, rcCheck, clrText);
		}

		if (controlType == xtpControlPopup)
		{
			CSize sz(XTP_DPI_X(6), XTP_DPI_Y(6));
			CPoint ptCenter = rcButton.CenterPoint();
			CRect rc(ptCenter.x - sz.cx, ptCenter.y - sz.cy, ptCenter.x + sz.cx,
					 ptCenter.y + sz.cy);
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayRight, rc, clrText);
		}

		rcText.SetRect(rcButton.left + XTP_DPI_X(16), rcButton.top, rcButton.right,
					   rcButton.bottom);
		rcShortcut.SetRect(rcButton.left + XTP_DPI_X(16) + XTP_DPI_X(4), rcButton.top,
						   rcButton.right - XTP_DPI_X(19), rcButton.bottom);
	}

	pDC->DrawText(pButton->GetCaption(), &rcText,
				  UINT(DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_END_ELLIPSIS)));

	if (controlType == xtpControlButton && pButton->GetShortcutText().GetLength() != 0)
		pDC->DrawText(pButton->GetShortcutText(), &rcShortcut,
					  UINT(DT_SINGLELINE | DT_VCENTER | (!bDraw ? DT_CALCRECT : DT_RIGHT)));

	return CSize(XTP_DPI_X(16) + XTP_DPI_X(12) + XTP_DPI_X(32) + rcText.Width()
					 + rcShortcut.Width(),
				 max(XTP_DPI_Y(17), m_nTextHeight - XTP_DPI_Y(4)));
}

CSize CXTPCommandBarsThemeMsMoney::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar,
														   CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		if (pBar->GetType() == xtpBarTypePopup)
			return XTP_DPI(CSize(3, 3));

		return XTP_DPI(CSize(6, 6));
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		CXTPPenDC pen(*pDC, RGB(112, 112, 112));
		Line(pDC, CPoint(XTP_DPI_X(16), rcControl.top - XTP_DPI_Y(1)),
			 CPoint(rc.right - XTP_DPI_X(14), rcControl.top - XTP_DPI_Y(1)));
	}
	return 0;
}

CSize CXTPCommandBarsThemeMsMoney::DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton,
															BOOL bDraw)
{
	CRect rcButton			   = bDraw ? pButton->GetRect() : CXTPEmptyRect();
	XTPControlType controlType = pButton->GetType();

	CSize szIcon   = pButton->GetParent()->GetIconSize();
	CSize szButton = CSize(szIcon.cx + XTP_DPI_X(7), szIcon.cy + XTP_DPI_Y(7));

	BOOL bVert = pButton->GetParent()->GetPosition() == xtpBarRight
				 || pButton->GetParent()->GetPosition() == xtpBarLeft;

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(),
		 bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		 bPopuped = pButton->GetPopuped();

	if (bDraw)
	{
		pDC->SetTextColor(!bEnabled
							  ? RGB(177, 177, 177)
							  : bSelected || bPressed ? RGB(255, 223, 127) : RGB(255, 254, 249));
		pDC->SetBkMode(TRANSPARENT);
	}

	switch (controlType)
	{
		case xtpControlPopup:
		{
			if (bDraw && (bSelected || bPopuped)
				&& pButton->GetParent()->GetPosition() != xtpBarPopup)
			{
				pDC->FillSolidRect(rcButton, RGB(80, 80, 80));
			}

			CSize sz = DrawControlText(pDC, pButton, rcButton, bDraw, bVert, TRUE, FALSE);
			return !bVert ? CSize(sz.cx + XTP_DPI_X(16), sz.cy + XTP_DPI_Y(8))
						  : CSize(sz.cx + XTP_DPI_X(8), sz.cy + XTP_DPI_Y(16));
		}
		case xtpControlSplitButtonPopup:
		{
			ASSERT(FALSE); // not implemented.
			return 0;
		}

		case xtpControlButtonPopup:
		case xtpControlButton:
		{
			CXTPFontDC font(pDC, &m_xtpFontRegular);

			if (bDraw)
			{
				CRect rcText = rcButton;
				rcText.top   = rcButton.top + szIcon.cy;

				DrawControlText(pDC, pButton, rcText, TRUE, FALSE, TRUE,
								controlType == xtpControlButtonPopup);

				if (pButton->GetIconId() != 0)
				{
					CPoint pt = CPoint(rcButton.CenterPoint().x - szIcon.cx / 2,
									   rcButton.top + XTP_DPI_Y(3));
					CXTPImageManagerIcon* pImage =
						XTPImageManager()->GetImage(XTPToUInt(pButton->GetIconId()), szIcon.cx);
					DrawImage(pDC, pt, CSize(szIcon.cx, 0), pImage, bSelected, bPressed, bEnabled,
							  bChecked, FALSE);
				}
			}
			else
			{
				CSize szText = DrawControlText(pDC, pButton, CXTPEmptyRect(), FALSE, FALSE, TRUE,
											   controlType == xtpControlButtonPopup);
				szButton.cy  = XTP_DPI_Y(10) + szIcon.cy + szText.cy;
				szButton.cx  = max(szButton.cx, szText.cx + XTP_DPI_X(10));
			}

			return szButton;
		}

		case xtpControlError:
		case xtpControlComboBox:
		case xtpControlEdit:
		case xtpControlCustom:
		case xtpControlLabel:
		case xtpControlCheckBox:
		case xtpControlGallery:
		case xtpControlRadioButton:
		default: break;
	}

	return 0;
}

void CXTPCommandBarsThemeMsMoney::AdjustExcludeRect(CRect& rc, CXTPControl* pControl,
													BOOL bVertical)
{
	CXTPCommandBar* pParent = pControl->GetParent();
	ASSERT(pParent);

	if (pControl->GetType() == xtpControlComboBox)
		return;

	if (pParent->GetType() != xtpBarTypePopup)
	{
		if (bVertical)
			rc.InflateRect(0, 0);
		else
			rc.InflateRect(0, 0);
	}
	else
		rc.DeflateRect(0, 0);
}

CRect CXTPCommandBarsThemeMsMoney::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetType() == xtpBarTypePopup)
		return XTP_DPI(CRect(1, 1, 1, 1));
	if (pBar->GetType() == xtpBarTypeNormal)
		return XTP_DPI(CRect(0, 5, 0, 5));
	return CXTPEmptyRect();
}

CSize CXTPCommandBarsThemeMsMoney::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	if (pBar->GetPosition() == xtpBarFloating)
	{
		CXTPClientRect rc(pBar);
		ASSERT(pDC);
		CXTPFontDC font(pDC, GetSmCaptionFont());
		CSize sz = pDC->GetTextExtent(_T(" "));

		rc.SetRect(XTP_DPI_X(3), XTP_DPI_Y(3), rc.right - XTP_DPI_X(3),
				   XTP_DPI_Y(3) + max(XTP_DPI_Y(15), sz.cy));
		if (pDC && bDraw)
		{
			pDC->FillSolidRect(rc, pBar->GetType() == xtpBarTypeMenuBar ? RGB(80, 80, 80)
																		: m_clrGradientDark);
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pBar->GetTitle(),
						  CRect(XTP_DPI_X(5), XTP_DPI_Y(3), rc.right - 2 * rc.Size().cy, rc.bottom),
						  DT_SINGLELINE | DT_VCENTER);
		}

		return rc.Size();
	}

	return 0;
}

void CXTPCommandBarsThemeMsMoney::DrawImage(CDC* pDC, CPoint pt, CSize /*sz*/,
											CXTPImageManagerIcon* pImage, BOOL bSelected,
											BOOL bPressed, BOOL bEnabled, BOOL bChecked,
											BOOL bPopuped, BOOL)
{
	if (!bEnabled)
	{
		pImage->Draw(pDC, pt, xtpImageDisabled);
	}
	else if (bPopuped || bChecked)
	{
		pImage->Draw(pDC, pt, (bChecked && (bSelected || bPressed) ? xtpImageHot : xtpImageNormal));
	}
	else
	{
		pImage->Draw(pDC, pt, (bSelected || bPressed ? xtpImageHot : xtpImageNormal));
	}
}

CSize CXTPCommandBarsThemeMsMoney::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType,
													  CXTPControl* pButton, CXTPCommandBar* pBar,
													  BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!IsVerticalPosition(pBar->GetPosition()))
				pButton->SetRect(CRect(szBar.cx - XTP_DPI_X(11) + XTP_DPI_X(2), XTP_DPI_Y(2),
									   szBar.cx - XTP_DPI_X(2), szBar.cy - XTP_DPI_Y(2)));
			else
				pButton->SetRect(CRect(XTP_DPI_X(2), szBar.cy - XTP_DPI_Y(11) + XTP_DPI_Y(2),
									   szBar.cx - XTP_DPI_X(2), szBar.cy - XTP_DPI_Y(2)));
		}
		else
		{
			COLORREF clrText = pButton->GetSelected() || pButton->GetPressed() ? RGB(255, 223, 127)
																			   : RGB(255, 254, 249);
			CXTPPenDC pen(*pDC, clrText);
			BOOL bHiddenExists = *(BOOL*)lpParam;
			CRect rcButton	 = pButton->GetRect();
			if (!IsVerticalPosition(pBar->GetPosition()))
			{
				CRect rc(rcButton.left, rcButton.bottom - rcButton.Width(), rcButton.right,
						 rcButton.bottom);
				XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rc, clrText);

				if (bHiddenExists)
				{
					rc.SetRect(rcButton.right - rcButton.Height(), rcButton.top, rcButton.right,
							   rcButton.bottom);
					XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolShiftRight, rc,
													 clrText);
				}
			}
			else
			{
				CRect rc(rcButton.left, rcButton.top, rcButton.left + rcButton.Height(),
						 rcButton.bottom);
				XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rc, clrText);

				if (bHiddenExists)
				{
					rc.SetRect(rcButton.right - rcButton.Height(), rcButton.top, rcButton.right,
							   rcButton.bottom);
					XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolShiftDown, rc, clrText);
				}
			}
		}
	}
	else
		return CXTPDefaultTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw,
													lpParam);

	return 0;
}
