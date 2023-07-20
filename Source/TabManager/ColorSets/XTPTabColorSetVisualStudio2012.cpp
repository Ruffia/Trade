// XTPTabColorSetVisualStudio2012.cpp
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
#include "Common/XTPResourceImage.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabColorSetVisualStudio2012::CXTPTabColorSetVisualStudio2012()
	: CXTPTabPaintManagerColorSet()
{
}

void CXTPTabColorSetVisualStudio2012::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonNormal						= RGB(238, 238, 242);
	m_clrButtonPressed						= RGB(14, 97, 152);
	m_clrButtonHighlighted					= RGB(28, 151, 234);
	m_clrButtonHighlightedClose				= RGB(82, 176, 239);
	m_clrButtonSelected						= RGB(0, 122, 204);
	m_clrButtonSelectedPressed				= RGB(14, 97, 152);
	m_clrButtonSelectedHighlighted			= RGB(28, 151, 234);
	m_clrButtonBorderHot					= COLORREF_NULL;
	m_clrTextNormal							= RGB(30, 30, 30);
	m_clrTextPressed						= RGB(255, 255, 255);
	m_clrTextHighlight						= RGB(255, 255, 255);
	m_clrTextHighlightClose					= RGB(255, 255, 255);
	m_clrTextSelected						= RGB(255, 255, 255);
	m_clrTextSelectedPressed				= RGB(255, 255, 255);
	m_clrTextSelectedHighlighted			= RGB(0, 0, 0);
	m_clrActiveFilesGlyph					= RGB(113, 113, 113);
	m_clrActiveFilesGlyphPressed			= RGB(255, 255, 255);
	m_clrActiveFilesGlyphHighlighted		= RGB(0, 122, 204);
	m_clrActiveFilesBack					= RGB(238, 238, 242);
	m_clrActiveFilesBackPressed				= RGB(0, 122, 204);
	m_clrActiveFilesBackHighlighted			= RGB(201, 222, 245);
	m_clrFrameBorder						= RGB(238, 238, 242);
	m_clrButtonBorder						= RGB(238, 238, 242);
	m_clrAutoHideFace						= RGB(238, 238, 242);
	m_clrHeaderFace							= RGB(238, 238, 242);
	m_csStateButton.clrFace.clrLight		= RGB(0, 122, 204);
	m_csStateButton.clrFace.clrDark			= RGB(0, 122, 204);
	m_csStateButton.clrBorder.clrDark		= RGB(238, 238, 242);
	m_csStateButton.clrBorder.clrLight		= RGB(238, 238, 242);
	m_clrNonActiveButtonSelected			= RGB(204, 206, 219);
	m_clrNonActiveButtonSelectedPressed		= RGB(183, 185, 197);
	m_clrNonActiveButtonSelectedHighlighted = RGB(230, 231, 237);
	m_clrNonActiveTextSelected				= RGB(109, 109, 112);
	m_clrNonActiveTextSelectedPressed		= RGB(45, 45, 45);
	m_clrNonActiveTextSelectedHighlighted   = RGB(113, 113, 113);

	m_clrTabClientBack   = XTPIniColor(_T("TabClient"), _T("Back"), RGB(238, 238, 242));
	m_clrTabClientBorder = XTPIniColor(_T("TabClient"), _T("Border"), RGB(238, 238, 242));
	m_clrPaneFace		 = XTPIniColor(_T("TabManager"), _T("PaneFace"), m_clrTabClientBack);
}

COLORREF CXTPTabColorSetVisualStudio2012::GetItemColor(CXTPTabManagerItem* pItem)
{
	COLORREF clrItem;

	if (pItem->IsSelected())
	{
		clrItem = m_clrButtonSelected;
		if (pItem->GetTabManager())
		{
			if (!pItem->GetTabManager()->IsActive())
				clrItem = m_clrNonActiveButtonSelected;
		}
	}

	else if (pItem->IsPressed())
	{
		clrItem = m_clrButtonPressed;
	}
	else if (pItem->IsHighlighted())
	{
		clrItem = m_clrButtonHighlighted;
	}
	else
	{
		clrItem = m_clrButtonNormal;
	}

	return clrItem;
}

void CXTPTabColorSetVisualStudio2012::FillNavigateButton(CDC* pDC,
														 CXTPTabManagerNavigateButton* pButton,
														 CRect& rc)
{
	BOOL bEnabled   = pButton->IsEnabled();
	BOOL bHilighted = pButton->IsHighlighted();
	BOOL bPressed   = pButton->IsPressed();

	if (pButton->IsOwnerNull())
	{
		if (bPressed || bHilighted)
		{
			pDC->FillSolidRect(rc, bPressed ? m_clrActiveFilesBackPressed
											: m_clrActiveFilesBackHighlighted);
			pButton->DrawEntry(pDC, rc,
							   bPressed ? m_clrActiveFilesGlyphPressed
										: m_clrActiveFilesGlyphHighlighted,
							   bEnabled);

			if (m_clrButtonBorderHot != COLORREF_NULL)
			{
				pDC->Draw3dRect(rc, m_clrButtonBorderHot, m_clrButtonBorderHot);
			}
		}
		else
		{
			pDC->FillSolidRect(rc, m_clrActiveFilesBack);
			pButton->DrawEntry(pDC, rc, m_clrActiveFilesGlyph, bEnabled);
		}
	}
	else
	{
		BOOL bTabSelected	= pButton->IsOwnerSelected();
		BOOL bTabHilighted   = pButton->IsOwnerHighlighted();
		BOOL bTabGroupActive = pButton->IsOwnerActive();

		if (bTabSelected)
		{
			if (bPressed || bHilighted)
			{
				if (bTabGroupActive)
				{
					pDC->FillSolidRect(rc, bPressed ? m_clrButtonSelectedPressed
													: m_clrButtonSelectedHighlighted);
					pButton->DrawEntry(pDC, rc,
									   bPressed ? m_clrTextSelectedPressed
												: m_clrTextSelectedHighlighted,
									   bEnabled);
				}
				else
				{
					pDC->FillSolidRect(rc, bPressed ? m_clrNonActiveButtonSelectedPressed
													: m_clrNonActiveButtonSelectedHighlighted);
					pButton->DrawEntry(pDC, rc,
									   bPressed ? m_clrNonActiveTextSelectedPressed
												: m_clrNonActiveTextSelectedHighlighted,
									   bEnabled);
				}

				if (m_clrButtonBorderHot != COLORREF_NULL)
				{
					pDC->Draw3dRect(rc, m_clrButtonBorderHot, m_clrButtonBorderHot);
				}
			}
			else
			{
				if (bTabGroupActive)
				{
					pDC->FillSolidRect(rc, m_clrButtonSelected);
					pButton->DrawEntry(pDC, rc, m_clrTextSelected, bEnabled);
				}
				else
				{
					pDC->FillSolidRect(rc, m_clrNonActiveButtonSelected);
					pButton->DrawEntry(pDC, rc, m_clrNonActiveTextSelected, bEnabled);
				}
			}
		}
		else
		{
			if (bPressed || bHilighted)
			{
				pDC->FillSolidRect(rc, bPressed ? m_clrButtonPressed : m_clrButtonHighlightedClose);
				pButton->DrawEntry(pDC, rc, bPressed ? m_clrTextPressed : m_clrTextHighlightClose,
								   bEnabled);

				if (m_clrButtonBorderHot != COLORREF_NULL)
				{
					pDC->Draw3dRect(rc, m_clrButtonBorderHot, m_clrButtonBorderHot);
				}
			}
			else
			{
				pDC->FillSolidRect(rc, bTabHilighted ? m_clrButtonHighlighted : m_clrButtonNormal);
				pButton->DrawEntry(pDC, rc, bTabHilighted ? m_clrTextHighlight : m_clrButtonNormal,
								   bEnabled);
			}
		}
	}
}

COLORREF CXTPTabColorSetVisualStudio2012::GetClientColor(CXTPTabManager* pTabManager)
{
	UNREFERENCED_PARAMETER(pTabManager);
	return m_clrPaneFace;
}
