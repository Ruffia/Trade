// XTPTabPaintManagerColorSet.cpp: implementation of the CXTPTabPaintManagerColorSet class.
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

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManager.inl"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTabPaintManagerColorSet

CXTPTabPaintManagerColorSet::CXTPTabPaintManagerColorSet()
	: m_pPaintManager(NULL)
{
}

CXTPTabPaintManagerColorSet::~CXTPTabPaintManagerColorSet()
{
}

BOOL CXTPTabPaintManagerColorSet::IsAppThemed() const
{
	return FALSE;
}

void CXTPTabPaintManagerColorSet::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	m_clrFrameBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrControlFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrPaneFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrBorderHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNHILIGHT));

	m_clrTextNormal.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrTextSelectedHighlighted.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrTextSelected.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrTextHighlight.SetStandardValue(RGB(0, 0, 128));
	m_clrTextDisabled.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	m_csStateButton.clrFace.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_csStateButton.clrBorder.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

	m_csPropertyPage2003.clrDarkShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csPropertyPage2003.clrFrameBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	m_csPropertyPage.clrHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNHILIGHT));
	m_csPropertyPage.clrShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csPropertyPage.clrDarkShadow.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW));

	m_csExcelSelected.clrRightInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrLeftInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrTopInnerBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csExcelSelected.clrLeftOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelSelected.clrRightOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelSelected.clrTopOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal = m_csExcelSelected;
	m_csExcelNormal.clrLeftOuterBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_csExcelNormal.clrRightOuterBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonNormal.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonSelectedHighlighted.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonBorder.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT));

	m_clrTabClientBack.SetStandardValue(GetXtremeColor(COLOR_APPWORKSPACE));
	m_clrTabClientBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceExcel2003)
	{
		m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	}

	m_clrAutoHideFace = m_clrHeaderFace;
}

void CXTPTabPaintManagerColorSet::GradientFill(CDC* pDC, CRect rc, COLORREF clrLight,
											   COLORREF clrDark, XTPTabPosition position)
{
	ASSERT_VALID(pDC);

	if (clrLight == XTP_TABMANAGER_COLOR_SHADED)
	{
		WORD bits[] = { 0x0055, 0x00AA, 0x0055, 0x00AA, 0x0055, 0x00AA, 0x0055, 0x00AA };

		CBitmap bitmap;
		bitmap.CreateBitmap(8, 8, 1, 1, &bits);

		CBrush brush;
		brush.CreatePatternBrush(&bitmap);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));

		pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);

		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	else
	{
		BOOL bHoriz = IsHorizontalTabPosition(position);
		if (!IsTopLeftTabPosition(position))
			SwapColors(clrLight, clrDark);
		if (m_pPaintManager->m_bInvertGradient)
			SwapColors(clrLight, clrDark);

		XTPDrawHelpers()->GradientFill(pDC, rc, clrLight, clrDark, !bHoriz);
	}
}

void CXTPTabPaintManagerColorSet::FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	COLORREF clrLight = m_clrHeaderFace.clrLight, clrDark = m_clrHeaderFace.clrDark;

	if (clrLight == clrDark && clrLight == GetXtremeColor(COLOR_3DFACE))
	{
		if (pTabManager->DrawParentBackground(pDC, rc))
			return;
	}

	GradientFill(pDC, rc, clrLight, clrDark, pTabManager->GetPosition());
}

COLORREF CXTPTabPaintManagerColorSet::FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	COLORREF clrClient = GetClientColor(pTabManager);
	pDC->FillSolidRect(rc, clrClient);
	return clrClient;
}

COLORREF CXTPTabPaintManagerColorSet::GetClientColor(CXTPTabManager* pTabManager)
{
	ASSERT(NULL != pTabManager);

	CXTPTabManagerItem* pItem = pTabManager->GetSelectedItem();
	COLORREF clrClient		  = (NULL != pItem ? GetItemColor(pItem)
										   : m_clrButtonSelected.operator COLORREF());

	return clrClient;
}

void CXTPTabPaintManagerColorSet::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	pDC->SetTextColor(!pItem->IsEnabled()
						  ? m_clrTextDisabled
						  : !pItem->GetTabManager()->IsActive()
								? m_clrTextSelectedHighlighted
								: pItem->IsSelected() ? m_clrTextSelected
													  : pItem->IsHighlighted() ? m_clrTextHighlight
																			   : m_clrTextNormal);
}

COLORREF CXTPTabPaintManagerColorSet::GetItemColor(CXTPTabManagerItem* pItem)
{
	COLORREF clrItem;

	if (m_pPaintManager->m_bOneNoteColors)
	{
		clrItem = pItem->GetColor();
	}
	else
	{
		if (pItem->IsSelected())
		{
			clrItem = m_clrButtonSelected;
			if (pItem->GetTabManager())
			{
				if (!pItem->GetTabManager()->IsActive())
					clrItem = m_clrButtonSelectedHighlighted;
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
	}

	return clrItem;
}

COLORREF CXTPTabPaintManagerColorSet::FillPropertyButton(CDC* pDC, CRect rcItem,
														 CXTPTabManagerItem* pItem)
{
	COLORREF clr = GetItemColor(pItem);

	pDC->FillSolidRect(rcItem, clr);

	return clr;
}

void CXTPTabPaintManagerColorSet::FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (!pItem->IsSelected())
	{
		if (pItem->GetTabManager()->IsHorizontalPosition())
			pDC->Draw3dRect(rcItem.right, rcItem.top + 2, 2, rcItem.Height() - 4, m_clrFrameBorder,
							m_clrButtonBorder);
		else
			pDC->Draw3dRect(rcItem.left + 2, rcItem.bottom, rcItem.Width() - 4, 2, m_clrFrameBorder,
							m_clrButtonBorder);
	}
	else
	{
		if (m_pPaintManager->m_bOneNoteColors)
		{
			pDC->FillSolidRect(rcItem, GetItemColor(pItem));
		}
		else
		{
			GradientFill(pDC, rcItem, m_csStateButton.clrFace.clrLight,
						 m_csStateButton.clrFace.clrDark, pItem->GetTabManager()->GetPosition());
		}

		pDC->Draw3dRect(rcItem, m_csStateButton.clrBorder.clrLight,
						m_csStateButton.clrBorder.clrDark);
	}
}
