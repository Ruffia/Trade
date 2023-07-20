// XTPTabColorSetOffice2013.cpp
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
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2012.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabColorSetOffice2013::CXTPTabColorSetOffice2013()
	: CXTPTabColorSetVisualStudio2012()
{
}

void CXTPTabColorSetOffice2013::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	// Default colors found in OFFICE2013WORD_INI.

	m_clrButtonNormal	  = XTPIniColor(_T("TabManager"), _T("Button"), RGB(255, 255, 255));
	m_clrButtonPressed	 = XTPIniColor(_T("TabManager"), _T("ButtonPressed"), RGB(62, 109, 181));
	m_clrButtonHighlighted = XTPIniColor(_T("TabManager"), _T("ButtonHighlighted"),
										 RGB(213, 225, 242));
	m_clrButtonHighlightedClose = XTPIniColor(_T("TabManager"), _T("ButtonHighlightedClose"),
											  RGB(163, 189, 227));
	m_clrButtonSelected = XTPIniColor(_T("TabManager"), _T("ButtonSelected"), RGB(43, 87, 154));
	m_clrButtonSelectedPressed	 = XTPIniColor(_T("TabManager"), _T("ButtonSelectedPressed"),
											 RGB(43, 87, 154));
	m_clrButtonSelectedHighlighted = XTPIniColor(_T("TabManager"), _T("ButtonSelectedHighlighted"),
												 RGB(43, 87, 154));
	m_clrButtonBorderHot = XTPIniColor(_T("TabManager"), _T("ButtonBorderHot"), COLORREF_NULL);
	m_clrTextNormal		 = XTPIniColor(_T("TabManager"), _T("Text"), RGB(119, 119, 119));
	m_clrTextPressed	 = XTPIniColor(_T("TabManager"), _T("TextPressed"), RGB(62, 109, 181));
	m_clrTextHighlight   = XTPIniColor(_T("TabManager"), _T("TextHighlighted"), RGB(213, 225, 242));
	m_clrTextHighlightClose  = XTPIniColor(_T("TabManager"), _T("TextHighlightedClose"),
										   RGB(25, 71, 138));
	m_clrTextSelected		 = XTPIniColor(_T("TabManager"), _T("TextSelected"), RGB(43, 87, 154));
	m_clrTextSelectedPressed = XTPIniColor(_T("TabManager"), _T("TextSelectedPressed"),
										   RGB(43, 87, 154));
	m_clrTextSelectedHighlighted	 = XTPIniColor(_T("TabManager"), _T("TextSelectedHighlighted"),
											   RGB(43, 87, 154));
	m_clrActiveFilesGlyph			 = XTPIniColor(_T("TabManager"), _T("ActiveFilesGlyph"),
										   RGB(119, 119, 119));
	m_clrActiveFilesGlyphPressed	 = XTPIniColor(_T("TabManager"), _T("ActiveFilesGlyphPressed"),
											   RGB(25, 71, 138));
	m_clrActiveFilesGlyphHighlighted = XTPIniColor(_T("TabManager"),
												   _T("ActiveFilesGlyphHighlighted"),
												   RGB(62, 109, 181));
	m_clrActiveFilesBack = XTPIniColor(_T("TabManager"), _T("ActiveFilesBack"), RGB(255, 255, 255));
	m_clrActiveFilesBackPressed		= XTPIniColor(_T("TabManager"), _T("ActiveFilesBackPressed"),
											  RGB(163, 189, 227));
	m_clrActiveFilesBackHighlighted = XTPIniColor(_T("TabManager"),
												  _T("ActiveFilesBackHighlighted"),
												  RGB(213, 225, 242));
	m_clrFrameBorder  = XTPIniColor(_T("TabManager"), _T("FrameBorder"), RGB(255, 255, 255));
	m_clrAutoHideFace = XTPIniColor(_T("TabManager"), _T("AutoHideFace"), RGB(255, 255, 255));
	m_clrHeaderFace   = XTPIniColor(_T("TabManager"), _T("HeaderFace"), RGB(230, 230, 230));
	m_clrButtonBorder = m_clrFrameBorder;
	m_csStateButton.clrFace.clrLight   = m_clrButtonSelected;
	m_csStateButton.clrFace.clrDark	= m_clrButtonSelected;
	m_csStateButton.clrBorder.clrDark  = m_clrFrameBorder;
	m_csStateButton.clrBorder.clrLight = m_clrFrameBorder;

	m_clrTabClientBack   = XTPIniColor(_T("TabClient"), _T("Back"), RGB(255, 255, 255));
	m_clrTabClientBorder = XTPIniColor(_T("TabClient"), _T("Border"), RGB(171, 171, 171));
	m_clrPaneFace		 = XTPIniColor(_T("TabManager"), _T("PaneFace"), m_clrTabClientBack);

	m_clrNonActiveButtonNormal		= XTPIniColor(_T("TabManager"), _T("NonActiveButton"),
											  RGB(255, 255, 255));
	m_clrNonActiveButtonHighlighted = XTPIniColor(_T("TabManager"),
												  _T("NonActiveButtonHighlighted"),
												  RGB(200, 200, 200));
	m_clrNonActiveButtonSelected	= XTPIniColor(_T("TabManager"), _T("NonActiveButtonSelected"),
												  RGB(150, 150, 150));
	m_clrNonActiveButtonSelectedPressed		= XTPIniColor(_T("TabManager"),
													  _T("NonActiveButtonSelectedPressed"),
													  RGB(150, 150, 150));
	m_clrNonActiveButtonSelectedHighlighted = XTPIniColor(_T("TabManager"),
														  _T("NonActiveButtonSelectedHighlighted"),
														  RGB(150, 150, 150));
	m_clrNonActiveText = XTPIniColor(_T("TabManager"), _T("NonActiveText"), RGB(119, 119, 119));
	m_clrNonActiveTextHighlighted = XTPIniColor(_T("TabManager"), _T("NonActiveTextHighlighted"),
												RGB(200, 200, 200));
	m_clrNonActiveTextSelected	= XTPIniColor(_T("TabManager"), _T("NonActiveTextSelected"),
												RGB(150, 150, 150));
	m_clrNonActiveTextSelectedPressed	 = XTPIniColor(_T("TabManager"),
													_T("NonActiveTextSelectedPressed"),
													RGB(150, 150, 150));
	m_clrNonActiveTextSelectedHighlighted = XTPIniColor(_T("TabManager"),
														_T("NonActiveTextSelectedHighlighted"),
														RGB(150, 150, 150));

	m_clrButtonSeparator = XTPIniColor(_T("TabManager"), _T("ButtonSeparator"), RGB(153, 153, 153));
	m_clrNonActiveButtonSeparator = XTPIniColor(_T("TabManager"), _T("NonActiveButtonSeparator"),
												RGB(153, 153, 153));
}

void CXTPTabColorSetOffice2013::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	COLORREF clrText;

	if (pItem->IsEnabled())
	{
		if (pItem->GetTabManager()->IsActive())
		{
			if (pItem->IsSelected() && pItem->IsHighlighted())
				clrText = m_clrTextSelectedHighlighted;
			else if (pItem->IsSelected() && pItem->IsPressed())
				clrText = m_clrTextSelectedPressed;
			else if (pItem->IsSelected())
				clrText = m_clrTextSelected;
			else if (pItem->IsHighlighted())
				clrText = m_clrTextHighlight;
			else if (pItem->IsPressed())
				clrText = m_clrTextPressed;
			else
				clrText = m_clrTextNormal;
		}
		else
		{
			if (pItem->IsSelected() && pItem->IsHighlighted())
				clrText = m_clrNonActiveTextSelectedHighlighted;
			else if (pItem->IsSelected() && pItem->IsPressed())
				clrText = m_clrNonActiveTextSelectedPressed;
			else if (pItem->IsSelected())
				clrText = m_clrNonActiveTextSelected;
			else if (pItem->IsHighlighted())
				clrText = m_clrNonActiveTextHighlighted;
			else if (pItem->IsPressed())
				clrText = m_clrNonActiveTextPressed;
			else
				clrText = m_clrNonActiveText;
		}
	}
	else
	{
		clrText = m_clrTextDisabled;
	}

	pDC->SetTextColor(clrText);
}

COLORREF CXTPTabColorSetOffice2013::GetItemColor(CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pItem);
	ASSERT(pItem->GetTabManager());

	COLORREF clrItem;

	if (pItem->GetTabManager()->IsActive())
	{
		if (pItem->IsSelected() && pItem->IsHighlighted())
			clrItem = m_clrButtonSelectedHighlighted;
		else if (pItem->IsSelected() && pItem->IsPressed())
			clrItem = m_clrButtonSelectedPressed;
		else if (pItem->IsSelected())
			clrItem = m_clrButtonSelected;
		else if (pItem->IsHighlighted())
			clrItem = m_clrButtonHighlighted;
		else if (pItem->IsPressed())
			clrItem = m_clrButtonPressed;
		else
			clrItem = m_clrButtonNormal;
	}
	else
	{
		if (pItem->IsSelected() && pItem->IsHighlighted())
			clrItem = m_clrNonActiveButtonSelectedHighlighted;
		else if (pItem->IsSelected() && pItem->IsPressed())
			clrItem = m_clrNonActiveButtonSelectedPressed;
		else if (pItem->IsSelected())
			clrItem = m_clrNonActiveButtonSelected;
		else if (pItem->IsHighlighted())
			clrItem = m_clrNonActiveButtonHighlighted;
		else if (pItem->IsPressed())
			clrItem = m_clrNonActiveButtonPressed;
		else
			clrItem = m_clrNonActiveButtonNormal;
	}

	return clrItem;
}

COLORREF CXTPTabColorSetOffice2013::GetClientColor(CXTPTabManager* pTabManager)
{
	UNREFERENCED_PARAMETER(pTabManager);
	return m_clrPaneFace;
}

COLORREF CXTPTabColorSetOffice2013::FillPropertyButton(CDC* pDC, CRect rcItem,
													   CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pItem);

	COLORREF clr = GetItemColor(pItem);

	CRect partTop(rcItem);
	CRect partLeft(rcItem);
	CRect partRight(rcItem);

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
			partTop.bottom = partTop.top + XTP_DPI_Y(2);
			partLeft.right = partTop.left + XTP_DPI_X(1);
			partRight.left = partTop.right - XTP_DPI_X(1);

			partLeft.bottom -= pItem->IsSelected() ? 0 : XTP_DPI_Y(3);
			partRight.bottom -= pItem->IsSelected() ? 0 : XTP_DPI_Y(3);
			break;

		case xtpTabPositionBottom:
			partTop.top		= partTop.bottom - XTP_DPI_Y(2);
			partLeft.left   = partTop.right - XTP_DPI_X(1);
			partRight.right = partTop.left + XTP_DPI_X(1);

			partLeft.top += pItem->IsSelected() ? 0 : XTP_DPI_Y(3);
			partRight.top += pItem->IsSelected() ? 0 : XTP_DPI_Y(3);
			break;

		case xtpTabPositionLeft:
			partTop.right   = partTop.left + XTP_DPI_X(2);
			partLeft.bottom = partTop.top + XTP_DPI_Y(1);
			partRight.top   = partTop.bottom - XTP_DPI_Y(1);

			partLeft.right -= pItem->IsSelected() ? 0 : XTP_DPI_X(3);
			partRight.right -= pItem->IsSelected() ? 0 : XTP_DPI_X(3);
			break;

		case xtpTabPositionRight:
			partTop.left	= partTop.right - XTP_DPI_X(2);
			partRight.top   = partTop.bottom - XTP_DPI_Y(1);
			partLeft.bottom = partTop.top + XTP_DPI_Y(1);

			partLeft.left += pItem->IsSelected() ? 0 : XTP_DPI_X(3);
			partRight.left += pItem->IsSelected() ? 0 : XTP_DPI_X(3);
			break;
	}

	if (pItem->IsSelected())
	{
		pDC->FillSolidRect(rcItem, m_clrButtonNormal);
		pDC->FillSolidRect(partLeft, m_clrButtonSeparator);
		pDC->FillSolidRect(partRight, m_clrButtonSeparator);
		pDC->FillSolidRect(partTop, clr);
	}
	else
	{
		int selIndex = 0;
		for (int i = 0; i < pItem->GetTabManager()->GetItemCount(); i++)
		{
			if (pItem->GetTabManager()->GetItem(i)->IsSelected())
			{
				selIndex = pItem->GetTabManager()->GetItem(i)->GetIndex();
				break;
			}
		}

		if (pItem->GetIndex() < selIndex)
		{
			CRect r = (pItem->GetTabManager()->GetPosition() == xtpTabPositionBottom) ? partRight
																					  : partLeft;
			pDC->FillSolidRect(r, m_clrButtonSeparator);
		}
		if (pItem->GetIndex() > selIndex)
		{
			CRect r = (pItem->GetTabManager()->GetPosition() == xtpTabPositionBottom) ? partLeft
																					  : partRight;
			pDC->FillSolidRect(r, m_clrButtonSeparator);
		}
	}

	return clr;
}

void CXTPTabColorSetOffice2013::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton,
												   CRect& rc)
{
	ASSERT_VALID(pDC);
	ASSERT(pButton);

	BOOL bBtnHilighted = pButton->IsHighlighted();
	BOOL bBtnPressed   = pButton->IsPressed();

	if (pButton->IsOwnerNull())
	{
		if (bBtnPressed || bBtnHilighted)
		{
			FillNavigateButtonEx(pDC, pButton, rc,
								 bBtnPressed ? m_clrActiveFilesBackPressed
											 : m_clrActiveFilesBackHighlighted,
								 bBtnPressed ? m_clrActiveFilesGlyphPressed
											 : m_clrActiveFilesGlyphHighlighted);
		}
		else
		{
			FillNavigateButtonEx(pDC, pButton, rc, m_clrActiveFilesBack, m_clrActiveFilesGlyph);
		}
	}
	else
	{
		BOOL bTabGroupActive = pButton->IsOwnerActive();
		BOOL bTabSelected	= pButton->IsOwnerSelected();
		BOOL bTabHilighted   = pButton->IsOwnerHighlighted();

		if (bTabGroupActive)
		{
			if (bTabSelected && bTabHilighted)
			{
				if (bBtnPressed && bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrTextSelectedHighlighted,
										 m_clrButtonNormal);
				else if (bBtnPressed)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal,
										 m_clrTextSelectedHighlighted);
				else if (bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal,
										 m_clrTextSelectedHighlighted);
				else
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal,
										 m_clrTextSelectedHighlighted);
			}
			else if (bTabSelected)
			{
				if (bBtnPressed && bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextSelected);
				else if (bBtnPressed)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextSelected);
				else if (bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextSelected);
				else
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextSelected);
			}
			else if (bTabHilighted)
			{
				if (bBtnPressed && bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrTextHighlight, m_clrButtonNormal);
				else if (bBtnPressed)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextHighlight);
				else if (bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextHighlight);
				else
					FillNavigateButtonEx(pDC, pButton, rc, m_clrButtonNormal, m_clrTextHighlight);
			}
			else
			{
				//
			}
		}
		else
		{
			if (bTabSelected && bTabHilighted)
			{
				if (bBtnPressed && bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveTextSelectedHighlighted,
										 m_clrNonActiveButtonNormal);
				else if (bBtnPressed)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelectedHighlighted);
				else if (bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelectedHighlighted);
				else
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelectedHighlighted);
			}
			else if (bTabSelected)
			{
				if (bBtnPressed && bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelected);
				else if (bBtnPressed)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelected);
				else if (bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelected);
				else
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextSelected);
			}
			else if (bTabHilighted)
			{
				if (bBtnPressed && bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveTextHighlighted,
										 m_clrNonActiveButtonNormal);
				else if (bBtnPressed)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextHighlighted);
				else if (bBtnHilighted)
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextHighlighted);
				else
					FillNavigateButtonEx(pDC, pButton, rc, m_clrNonActiveButtonNormal,
										 m_clrNonActiveTextHighlighted);
			}
			else
			{
				//
			}
		}
	}
}

void CXTPTabColorSetOffice2013::FillNavigateButtonEx(CDC* pDC,
													 CXTPTabManagerNavigateButton* pButton,
													 CRect& rc, CXTPPaintManagerColor clrBack,
													 CXTPPaintManagerColor clrText)
{
	pDC->FillSolidRect(rc, clrBack);
	pButton->DrawEntry(pDC, rc, clrText, pButton->IsEnabled());
}
