// XTPButtonThemeOffice2013.cpp: implementation of the CXTPButtonThemeOffice2013 class.
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPImageManager.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Button/Themes/XTPButtonThemeUltraFlat.h"
#include "Controls/Button/Themes/XTPButtonThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef _XTP_ACTIVEX_BUTTON
#	ifdef _DEBUG
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#		define new DEBUG_NEW
#	endif
#endif

#ifndef WM_QUERYUISTATE
#	define WM_QUERYUISTATE 0x0129
#endif

#ifndef UISF_HIDEACCEL
#	define UISF_HIDEACCEL 0x2
#	define UISF_HIDEFOCUS 0x1
#endif

CXTPButtonThemeOffice2013::CXTPButtonThemeOffice2013()
{
	m_nTheme		   = xtpControlThemeOffice2013;
	m_bFlatGlyphs	  = TRUE;
	m_nBorderWidth	 = 1;
	m_bHiglightButtons = TRUE;
}

void CXTPButtonThemeOffice2013::RefreshMetrics(CXTPButton* pButton)
{
	UNREFERENCED_PARAMETER(pButton);

	m_themeButton->OpenThemeData(NULL, m_bToolbarStyle ? L"TOOLBAR" : L"BUTTON");

	// Defaults are set to OFFICE2013WORD_INI colors.

	m_crBack.SetStandardValue(XTPIniColor(_T("Controls.Button"), _T("Back"), RGB(253, 253, 253)));
	m_crBackHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BackHilite"), RGB(213, 225, 242)));
	m_crBackPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BackPushed"), RGB(163, 189, 227)));
	m_crBackDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BackDisabled"), RGB(253, 253, 253)));
	m_crBorder.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("Border"), RGB(171, 171, 171)));
	m_crBorderHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderHilite"), RGB(163, 189, 227)));
	m_crBorderPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderPushed"), RGB(43, 87, 154)));
	m_crBorderDefault.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderDefault"), RGB(43, 87, 154)));
	m_crBorderDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("BorderDisabled"), RGB(225, 225, 225)));
	m_crText.SetStandardValue(XTPIniColor(_T("Controls.Button"), _T("Text"), RGB(68, 68, 68)));
	m_crTextCheck.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextCheck"), RGB(68, 68, 68)));
	m_crTextHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextHilite"), RGB(68, 68, 68)));
	m_crTextPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextPushed"), RGB(68, 68, 68)));
	m_crTextDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("TextDisabled"), RGB(152, 152, 152)));
	m_crMark.SetStandardValue(XTPIniColor(_T("Controls.Button"), _T("Mark"), RGB(139, 139, 139)));
	m_crMarkHilite.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("MarkHilite"), RGB(67, 75, 80)));
	m_crMarkPushed.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("MarkPushed"), RGB(15, 20, 24)));
	m_crMarkDisabled.SetStandardValue(
		XTPIniColor(_T("Controls.Button"), _T("MarkDisabled"), RGB(198, 198, 198)));
}

void CXTPButtonThemeOffice2013::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bChecked  = pButton->GetChecked();
	BOOL bDefault  = pButton->GetButtonStyle() == BS_DEFPUSHBUTTON;

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	COLORREF clrBack;
	COLORREF clrBorder;

	if (!bEnabled)
	{
		clrBack   = m_crBackDisabled;
		clrBorder = m_crBorderDisabled;
	}
	else if (bSelected && bPressed)
	{
		clrBack   = m_crBackPushed;
		clrBorder = m_crBorderPushed;
	}
	else if (bSelected || bPressed && m_bHiglightButtons)
	{
		if (bDefault)
		{
			clrBack   = XTPColorManager()->LightColor(m_crBackPushed, m_crBackHilite, 50);
			clrBorder = XTPColorManager()->LightColor(m_crBorderPushed, m_crBorderHilite, 50);
		}
		else
		{
			clrBack   = m_crBackHilite;
			clrBorder = m_crBorderHilite;
		}
	}
	else if (bChecked)
	{
		clrBack   = m_crBackHilite;
		clrBorder = m_crBorderHilite;
	}
	else
	{
		clrBack = m_crBack;

		if (bDefault)
		{
			clrBorder = m_crBorderHilite;
		}
		else
		{
			clrBorder = m_crBorder;
		}
	}

	CRect rc(pButton->GetButtonRect());
	pDC->FillSolidRect(rc, clrBack);
	pDC->Draw3dRect(rc, clrBorder, clrBorder);
}

void CXTPButtonThemeOffice2013::DrawGroupBox(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawGroupBox(pDC, pButton);
		return;
	}

	BOOL bEnabled = pButton->IsWindowEnabled();

	COLORREF clrBorder = bEnabled ? m_crBorder : m_crBorderDisabled;

	CRect rcText;
	CalcRect(pDC, pButton, &rcText, CBR_GROUPTEXT);

	if (pButton->GetTextAlignment() & BS_RIGHT)
	{
		rcText.OffsetRect(+m_cxEdge, 0);
	}
	else
	{
		rcText.OffsetRect(-m_cxEdge, 0);
	}

	pDC->ExcludeClipRect(&rcText);

	CRect rcFrame;
	CalcRect(pDC, pButton, &rcFrame, CBR_GROUPFRAME);

	if (pButton->GetBorderStyle() == xtpGroupBoxSingleLine)
	{
		pDC->FillSolidRect(rcFrame.left, rcFrame.top, rcFrame.Width(), 1, clrBorder);
	}
	else
	{
		pDC->Draw3dRect(&rcFrame, clrBorder, clrBorder);
	}

	pDC->SelectClipRgn(NULL);
}

void CXTPButtonThemeOffice2013::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bChecked  = pButton->GetChecked();

	COLORREF clrBack;
	COLORREF clrCheck;
	COLORREF clrBorder;

	if (!bEnabled)
	{
		clrBack   = m_crBackDisabled;
		clrCheck  = m_crMarkDisabled;
		clrBorder = m_crBorderDisabled;
	}
	else if (bSelected && bPressed)
	{
		clrBack   = m_crBackPushed;
		clrCheck  = m_crMarkPushed;
		clrBorder = m_crBorderPushed;
	}
	else if (bSelected || bPressed && m_bHiglightButtons)
	{
		clrBack   = m_crBackHilite;
		clrCheck  = m_crMarkHilite;
		clrBorder = m_crBorderHilite;
	}
	else
	{
		clrBack   = m_crBack;
		clrCheck  = m_crMark;
		clrBorder = m_crBorder;
	}

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_CHECKBOX);

	pDC->FillSolidRect(rc, clrBack);
	pDC->Draw3dRect(rc, clrBorder, clrBorder);

	switch (bChecked)
	{
		case BST_CHECKED:
		{
			XTPPrimitiveDrawer()->DrawSymbol(pDC,
											 CXTPDrawHelpers::IsContextRTL(pDC)
												 ? xtpPrimitiveSymbolCheckMarkRTL
												 : xtpPrimitiveSymbolCheckMark,
											 rc, clrCheck);
		}
		break;

		case BST_INDETERMINATE:
		{
			rc.DeflateRect(3, 3);
			pDC->FillSolidRect(rc, clrCheck);
		}
		break;
	}
}

void CXTPButtonThemeOffice2013::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bChecked  = pButton->GetChecked();

	COLORREF clrBack;
	COLORREF clrRadio;
	COLORREF clrBorder;

	if (!bEnabled)
	{
		clrBack   = m_crBackDisabled;
		clrRadio  = m_crMarkDisabled;
		clrBorder = m_crBorderDisabled;
	}
	else if (bSelected && bPressed)
	{
		clrBack   = m_crBackPushed;
		clrRadio  = m_crMarkPushed;
		clrBorder = m_crBorderPushed;
	}
	else if (bSelected || bPressed && m_bHiglightButtons)
	{
		clrBack   = m_crBackHilite;
		clrRadio  = m_crMarkHilite;
		clrBorder = m_crBorderHilite;
	}
	else
	{
		clrBack   = m_crBack;
		clrRadio  = m_crMark;
		clrBorder = m_crBorder;
	}

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_RADIOBOX);

	AlphaEllipse(pDC, rc, clrBorder, clrBack);

	if (bChecked)
	{
		AlphaEllipse(pDC,
					 CRect(rc.left + XTP_DPI_X(4), rc.top + XTP_DPI_Y(4), rc.left + XTP_DPI_X(9),
						   rc.top + XTP_DPI_Y(9)),
					 clrRadio, clrRadio);
	}
}

COLORREF CXTPButtonThemeOffice2013::GetTextColor(CXTPButton* pButton)
{
	BOOL bEnabled = pButton->IsWindowEnabled();
	if (!bEnabled)
	{
		return m_crTextDisabled;
	}

	COLORREF btnColor = pButton->GetButtonTextColor();
	if (btnColor != COLORREF_NULL)
		return btnColor;

	BYTE bStyle = pButton->GetButtonStyle();
	if (bStyle == BS_RADIOBUTTON || bStyle == BS_AUTORADIOBUTTON || bStyle == BS_CHECKBOX
		|| bStyle == BS_AUTOCHECKBOX || bStyle == BS_3STATE || bStyle == BS_AUTO3STATE)
	{
		return m_crTextCheck;
	}

	BOOL bPressed  = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	if (bPressed)
	{
		return m_crTextPushed;
	}
	else if (bSelected || bPressed && m_bHiglightButtons)
	{
		return m_crTextHilite;
	}

	return m_crText;
}

void CXTPButtonThemeOffice2013::DrawPushButtonDropDown(CDC* pDC, CXTPButton* pButton)
{
	int nGap = pButton->GetBorderGap();

	CRect rcItem(pButton->GetButtonRect());
	rcItem.DeflateRect(nGap, nGap);

	BOOL bIsEmpty  = pButton->GetButtonText().IsEmpty() && pButton->GetImageSize() == CSize(0, 0);
	BOOL bEnabled  = pButton->IsWindowEnabled();
	BOOL bSelected = pButton->IsHighlighted();

	if (!bIsEmpty)
	{
		rcItem.left = rcItem.right - __min(rcItem.Height(), XTP_DPI_X(16));
	}

	CRect rcArrow = rcItem;
	rcArrow.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));

	switch (pButton->GetPushButtonStyle())
	{
		case xtpButtonDropDown:
		{
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow,
											 GetTextColor(pButton));
			break;
		}

		case xtpButtonDropDownRight:
		{
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayRight, rcArrow,
											 GetTextColor(pButton));
			break;
		}

		case xtpButtonSplitDropDown:
		{
			rcArrow.OffsetRect(XTP_DPI_X(2), 0);
			XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow,
											 GetTextColor(pButton));

			BOOL bPressed = pButton->IsPushed();
			COLORREF clrBorder;

			if (!bEnabled)
			{
				clrBorder = m_crBorderDisabled;
			}
			else if (bSelected && bPressed)
			{
				clrBorder = m_crBorderPushed;
			}
			else if (bSelected || bPressed && m_bHiglightButtons)
			{
				clrBorder = m_crBorderHilite;
			}
			else
			{
				clrBorder = m_crBorder;
			}

			pDC->FillSolidRect(rcItem.left, rcItem.top, XTP_DPI_X(1), rcItem.Height(), clrBorder);

			break;
		}
	}
}

void CXTPButtonThemeOffice2013::DrawPushButtonText(CDC* pDC, CXTPButton* pButton)
{
	CString strText = pButton->GetButtonText();

	// if the string is empty just return.
	if (strText.IsEmpty())
		return;

	CRect rcText;
	UINT dsFlags;
	CalcRects(pDC, pButton, &rcText, &dsFlags, NULL);

	COLORREF clrText = GetTextColor(pButton);
	pDC->SetTextColor(clrText);

	if (pButton->GetMarkupUIElement())
	{
		XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), NULL, clrText);

		XTPMarkupRenderElement(pButton->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);
	}
	else
	{
#ifndef _XTP_ACTIVEX
		if (pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEACCEL)
		{
			dsFlags |= DT_HIDEPREFIX;
		}
#endif
		pDC->DrawText(strText, rcText, dsFlags);
	}
}

void CXTPButtonThemeOffice2013::DrawPushButtonIcon(CDC* pDC, CXTPButton* pButton)
{
	ASSERT_VALID(pButton);

	CSize sz = pButton->GetImageSize();
	if (sz == CSize(0, 0))
		return;

	CRect rcImage;
	CalcRects(pDC, pButton, NULL, NULL, &rcImage);

	CPoint pt = rcImage.TopLeft();

	BOOL bSelected = pButton->GetChecked() || pButton->IsPushed();

	if (m_bToolbarStyle)
	{
		CXTPImageManagerIcon* pIcon = pButton->GetIcon();
		if (pIcon && pIcon->HasNormalIcon())
		{
			if (!pButton->IsWindowEnabled())
			{
				if (pIcon->IsRasterIcon())
				{
					pDC->DrawState(pt, pIcon->GetExtent(), pIcon->GetIcon(),
								   DSS_NORMAL | DSS_DISABLED, (HBRUSH)0);
				}
				else
				{
					pIcon->Draw(pDC, pt, xtpImageDisabled);
				}
			}
			else if (pButton->IsHighlighted() || bSelected)
			{
				if (pIcon->IsRasterIcon())
				{
					pDC->DrawState(pt, pIcon->GetExtent(),
								   pButton->IsPushed() ? pIcon->GetPressedIcon()
													   : pIcon->GetHotIcon(),
								   DSS_NORMAL, (HBRUSH)0);
				}
				else
				{
					pIcon->Draw(pDC, pt, pButton->IsPushed() ? xtpImagePressed : xtpImageHot);
				}
			}
			else
			{
				if (pIcon->IsRasterIcon())
				{
					pDC->DrawState(pt, 0, pIcon->GetIcon(), DSS_NORMAL, (HBRUSH)0);
				}
				else
				{
					pIcon->Draw(pDC, pt, xtpImageNormal);
				}
			}
		}
		else
		{
			pButton->DrawImage(pDC, CRect(pt, sz));
		}
	}
	else
	{
		pButton->DrawImage(pDC, CRect(pt, sz));
	}
}

// This inclusion is necessary for ActiveX build
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
