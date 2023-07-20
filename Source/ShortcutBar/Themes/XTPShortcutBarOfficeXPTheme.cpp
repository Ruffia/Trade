// XTPShortcutBarOfficeXPTheme.cpp : implementation of the CXTPShortcutBarOfficeXPTheme class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"

#include "ShortcutBar/XTPShortcutBarPaintManager.h"
#include "ShortcutBar/XTPShortcutBar.h"
#include "ShortcutBar/Themes/XTPShortcutBarOfficeXPTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOfficeXPTheme

void CXTPShortcutBarOfficeXPTheme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);

	pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
}

void CXTPShortcutBarOfficeXPTheme::DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPWindowRect rc(pShortcutBar);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
}

void CXTPShortcutBarOfficeXPTheme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc					  = pItem->GetItemRect();

	if (pItem->IsSelected()
		&& (pShortcutBar->GetHotItem() != pItem
			|| (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED);
		pDC->SetTextColor(GetXtremeColor(XPCOLOR_CHECKED_TEXT));
	}
	else if (pItem->IsSelected()
			 || (pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL)
			 || pShortcutBar->GetPressedItem() == pItem)
	{
		Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
		pDC->SetTextColor(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	}
}

int CXTPShortcutBarOfficeXPTheme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();

		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW));

		Line(pDC, rc.CenterPoint().x - XTP_DPI_X(16), rc.top + XTP_DPI_Y(3),
			 rc.CenterPoint().x + XTP_DPI_X(16), rc.top + XTP_DPI_Y(3), XPCOLOR_TOOLBAR_GRIPPER);
		Line(pDC, rc.CenterPoint().x - XTP_DPI_X(16), rc.top + XTP_DPI_Y(5),
			 rc.CenterPoint().x + XTP_DPI_X(16), rc.top + XTP_DPI_Y(5), XPCOLOR_TOOLBAR_GRIPPER);
	}

	return XTP_DPI_Y(8);
}

void CXTPShortcutBarOfficeXPTheme::DrawImage(CDC* pDC, CPoint pt, CSize sz,
											 CXTPImageManagerIcon* pImage, BOOL bSelected,
											 BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (!bEnabled)
	{
		pImage->Draw(pDC, pt, xtpImageDisabled, sz);
	}
	else if (bChecked)
	{
		pImage->Draw(pDC, pt, bChecked && (bSelected || bPressed) ? xtpImageHot : xtpImageChecked,
					 sz);
	}
	else if (!bSelected && !bPressed)
	{
		if (pImage->IsRasterIcon())
		{
			pImage->Draw(pDC, pt, pImage->GetFadedIcon(), sz);
		}
		else
		{
			pImage->Draw(pDC, pt, xtpImageNormal, sz);
		}
	}
	else if ((bSelected && !bPressed) || (!bSelected && bPressed))
	{
		if (pImage->IsRasterIcon())
		{
			pImage->Draw(pDC, CPoint(pt.x + 1, pt.y + 1), pImage->GetShadowIcon(), sz);
		}

		pImage->Draw(pDC, CPoint(pt.x - 1, pt.y - 1), xtpImageHot, sz);
	}
	else if (bSelected && bPressed)
	{
		pImage->Draw(pDC, pt, xtpImageHot, sz);
	}
}
