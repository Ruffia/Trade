// XTPTabColorSetVisualStudio2010.cpp
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
#include "Common/XTPImageManager.h"
#include "Common/XTPVC80Helpers.h"

#include "TabManager/Includes.h"
#include "TabManager/XTPTabPaintManager.inl"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// void CXTPTabColorSetVisualStudio2010

CXTPTabColorSetVisualStudio2010::CXTPTabColorSetVisualStudio2010(BOOL bPaneColor /* = FALSE */)
{
	m_bGradientButton = FALSE;
	m_bPaneColor	  = bPaneColor;

	const DWORD _cb[] = {
		0x293956, 0x2d3f5c, 0x2d3f5c, 0x2d3f5c, 0x35496a, 0x2d3f5c, 0x2d3f5c, 0x2d3f5c,
		0x2d3f5c, 0x2d3f5c, 0x293956, 0x2d3f5c, 0x2d3f5c, 0x2d3f5c, 0x35496a, 0x2d3f5c,
	};

	CBitmap bmp;
	LPBYTE lpBits;
	bmp.Attach(CXTPImageManager::Create32BPPDIBSection(0, 4, 4, &lpBits));

	if (lpBits)
	{
		MEMCPY_S(lpBits, _cb, 4 * 4 * sizeof(DWORD));

		m_xtpBrushHeader.CreatePatternBrush(&bmp);
	}
}

void CXTPTabColorSetVisualStudio2010::FillHeader(CDC* pDC, CRect rc,
												 CXTPTabManager* /*pTabManager*/)
{
	if (m_xtpBrushHeader.GetSafeHandle() == 0)
		pDC->FillSolidRect(rc, m_clrHeaderFace);
	else
		FillRect(pDC->GetSafeHdc(), rc, m_xtpBrushHeader);
}

void CXTPTabColorSetVisualStudio2010::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_bLunaTheme	  = XTPSystemVersion()->IsWinVistaOrGreater();
	m_bGradientButton = TRUE;

	m_clrTextSelected.SetStandardValue(0);
	m_clrTextHighlight.SetStandardValue(RGB(255, 255, 255));
	m_clrTextNormal.SetStandardValue(RGB(255, 255, 255));
	m_clrTextSelectedHighlighted.SetStandardValue(RGB(206, 212, 221));
	m_clrTextDisabled.SetStandardValue(RGB(206, 212, 221));

	m_clrButtonNormal.SetStandardValue(RGB(74, 93, 128));
	m_clrButtonSelected.SetStandardValue(RGB(255, 232, 166));
	m_clrButtonHighlighted.SetStandardValue(RGB(95, 107, 117));
	m_clrButtonPressed.SetStandardValue(RGB(255, 232, 166));
	m_clrButtonSelectedHighlighted = RGB(74, 93, 128);

	m_clrHeaderFace.SetStandardValue(RGB(45, 63, 92));

	m_clrAutoHideFace.SetStandardValue(RGB(45, 63, 92));

	m_clrNavigateButtonBorder = RGB(229, 195, 101);

	m_clrFrameBorder.SetStandardValue(RGB(47, 65, 95));
	m_clrFrameHighlightBorder.SetStandardValue(RGB(155, 167, 183));

	m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(47, 65, 95));
	m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(41, 58, 86));
	m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(41, 58, 86));
	m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(155, 167, 183));

	if (!m_bLunaTheme)
	{
		m_clrButtonHighlighted.SetStandardValue(RGB(74, 93, 115));
		m_clrButtonNormal.SetStandardValue(RGB(57, 81, 115));
		m_clrButtonSelected.SetStandardValue(RGB(255, 243, 206));
	}

	if (m_bPaneColor)
	{
		m_clrButtonSelected.SetStandardValue(RGB(255, 255, 255));
	}
}

COLORREF CXTPTabColorSetVisualStudio2010::FillPropertyButton(CDC* pDC, CRect rcItem,
															 CXTPTabManagerItem* pItem)
{
	COLORREF clrItem = GetItemColor(pItem);

	if (pItem->IsSelected())
	{
		if (pItem->GetTabManager())
		{
			if (!pItem->GetTabManager()->IsActive())
			{
				if (pItem->IsHighlighted())
				{
					GradientFill(pDC, rcItem, RGB(109, 117, 118), m_clrButtonHighlighted,
								 pItem->GetTabManager()->GetPosition());
					clrItem = m_clrButtonHighlighted;
				}
				return clrItem;
			}
		}
		if (m_bLunaTheme)
		{
			pDC->FillSolidRect(rcItem, m_clrButtonSelected);

			if (!m_bPaneColor)
			{
				CRect rTop = rcItem;

				int nHalf = IsHorizontalTabPosition(pItem->GetTabManager()->GetPosition())
								? rcItem.Height() / 2
								: rcItem.Width() / 2;
				CXTPTabPaintManagerTheme::DeflateRectEx(rTop, CRect(0, 0, 0, nHalf),
														pItem->GetTabManager()->GetPosition());

				GradientFill(pDC, rTop, RGB(255, 252, 242), RGB(255, 243, 207),
							 pItem->GetTabManager()->GetPosition());
			}
		}
		else
		{
			pDC->FillSolidRect(rcItem, m_clrButtonSelected);
		}

		clrItem = m_clrButtonHighlighted;
	}
	else if (pItem->IsHighlighted())
	{
		GradientFill(pDC, rcItem, RGB(109, 117, 118), m_clrButtonHighlighted,
					 pItem->GetTabManager()->GetPosition());
		clrItem = m_clrButtonHighlighted;
	}
	else
	{
		pDC->FillSolidRect(rcItem, clrItem);
	}

	return clrItem;
}

void CXTPTabColorSetVisualStudio2010::FillNavigateButton(CDC* pDC,
														 CXTPTabManagerNavigateButton* pButton,
														 CRect& rc)
{
	COLORREF clrColor = pButton->GetItem()
							? (pButton->GetItem()->IsSelected()
								   ? RGB(117, 99, 61)
								   : pButton->GetItem()->IsHighlighted() ? RGB(206, 212, 221)
																		 : RGB(41, 56, 82))
							: RGB(206, 212, 221);

	if (pButton->IsEnabled())
	{
		if ((pButton->IsHighlighted() && pButton->IsPressed()))
		{
			pDC->FillSolidRect(rc, RGB(255, 232, 166));
			pDC->Draw3dRect(rc, m_clrNavigateButtonBorder, m_clrNavigateButtonBorder);
			clrColor = RGB(0, 0, 0);
		}
		else if (pButton->IsPressed() || pButton->IsHighlighted())
		{
			pDC->FillSolidRect(rc, RGB(255, 252, 244));
			pDC->Draw3dRect(rc, m_clrNavigateButtonBorder, m_clrNavigateButtonBorder);
			clrColor = RGB(0, 0, 0);
		}
		else
		{
		}
	}
	pDC->SetTextColor(clrColor);

	pButton->DrawEntry(pDC, rc, m_clrActiveFilesGlyph, pButton->IsEnabled());
}

void CXTPTabColorSetVisualStudio2010::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	BOOL isActive = TRUE;
	if (pItem->GetTabManager())
	{
		isActive = pItem->GetTabManager()->IsActive();
	}
	COLORREF color;
	if (!pItem->IsEnabled())
		color = m_clrTextDisabled;
	else if (pItem->IsSelected())
	{
		if (isActive)
			color = m_clrTextSelected;
		else
			color = m_clrTextSelectedHighlighted;
	}
	else if (pItem->IsHighlighted())
		color = m_clrTextHighlight;
	else
		color = m_clrTextNormal;

	pDC->SetTextColor(color);
}
