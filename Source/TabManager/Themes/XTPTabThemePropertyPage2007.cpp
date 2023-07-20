// XTPTabThemePropertyPage2007.cpp
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPImageManager.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerTheme.h"
#include "TabManager/Themes/XTPTabThemePropertyPage2007.h"
#include "TabManager/ColorSets/XTPTabColorSetDefault.h"
#include "TabManager/ColorSets/XTPTabColorSetWinNative.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPage2007

void CAppearanceSetPropertyPage2007::RefreshMetrics()
{
	CXTPTabPaintManagerTheme::RefreshMetrics();
}

CRect CAppearanceSetPropertyPage2007::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC,
													 CRect rcControl)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	pDC->FillSolidRect(rcHeader, GetColorSet()->m_clrHeaderFace.clrLight);

	CRect rcClient = GetClientRect(rcControl, pTabManager);
	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("TABCONTROLPAGE"));
		if (pImage == NULL)
		{
			pImage = pImages->LoadFile(_T("RIBBONGROUPS"));
		}
		if (pImage)
			pImage->DrawImage(pDC, rcClient, pImage->GetSource(0, 1), CRect(8, 8, 8, 8));
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(),
							 GetColorSet()->m_csPropertyPage.clrShadow,
							 GetColorSet()->m_csPropertyPage.clrShadow);
	}

	return rcHeader;
}

void CAppearanceSetPropertyPage2007::DrawButtonBackground(CDC* pDC, CXTPTabManagerItem* pItem,
														  CRect rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("RIBBONTAB"));

	if (pImage)
	{
		int nState = 0;
		if (pItem->IsSelected() && pItem->IsFocused())
		{
			nState = 5;
		}
		else if (pItem->IsSelected() && pItem->IsHighlighted())
		{
			nState = 4;
		}
		else if (pItem->IsSelected())
		{
			nState = 3;
		}
		else if (pItem->IsPressed())
		{
			nState = 2;
		}
		else if (pItem->IsHighlighted())
		{
			nState = 1;
		}

		if (pImage)
			pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 6), CRect(5, 5, 5, 5), 0xFF00FF);
	}
	else
		GetColorSet()->FillPropertyButton(pDC, rc, pItem);
}

void CAppearanceSetPropertyPage2007::DrawRotatedButton(CDC* pDC, CRect rcItem,
													   CXTPTabManagerItem* pItem, BOOL bSwap,
													   LPFNDRAWROTATEDBITS pfnRotatedProcBack,
													   LPFNDRAWROTATEDBITS pfnRotatedProc)
{
	const int cx = rcItem.Width();
	const int cy = rcItem.Height();

	UINT* pSrcBits = NULL;
	HBITMAP hbmSrc = CXTPImageManager::Create32BPPDIBSection(NULL, bSwap ? cy : cx, bSwap ? cx : cy,
															 (LPBYTE*)&pSrcBits);
	if (!pSrcBits)
		return;

	UINT* pDestBits = NULL;
	HBITMAP hbmDest = CXTPImageManager::Create32BPPDIBSection(NULL, cx, cy, (LPBYTE*)&pDestBits);
	if (!pDestBits)
		return;

	CRect rcDraw(0, 0, bSwap ? cy : cx, bSwap ? cx : cy);

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	HGDIOBJ hbmpOld = 0;

	if (pfnRotatedProcBack != NULL)
	{
		hbmpOld = ::SelectObject(dc, hbmDest);

		dc.BitBlt(0, 0, cx, cy, pDC, rcItem.left, rcItem.top, SRCCOPY);

		(*pfnRotatedProcBack)(rcDraw.Width(), rcDraw.Height(), pDestBits, pSrcBits);

		::SelectObject(dc, hbmpOld);

		hbmpOld = ::SelectObject(dc, hbmSrc);
	}
	else
	{
		hbmpOld = ::SelectObject(dc, hbmSrc);

		dc.FillSolidRect(rcDraw, GetColorSet()->m_clrHeaderFace.clrLight);
	}

	DrawButtonBackground(&dc, pItem, rcDraw);

	(*pfnRotatedProc)(cx, cy, pSrcBits, pDestBits);

	pDC->DrawState(rcItem.TopLeft(), rcItem.Size(), hbmDest, DST_BITMAP);

	::SelectObject(dc, hbmpOld);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
}

void CAppearanceSetPropertyPage2007::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	if (pItem->IsSelected() || pItem->IsHighlighted())
	{
		switch (pItem->GetTabManager()->GetPosition())
		{
			case xtpTabPositionTop: DrawButtonBackground(pDC, pItem, rc); break;

			case xtpTabPositionBottom:
				DrawRotatedButton(pDC, rc, pItem, FALSE, NULL,
								  &CColorSetWinNative::DrawRotatedBitsBottom);
				break;

			case xtpTabPositionLeft:
				DrawRotatedButton(pDC, rc, pItem, TRUE, NULL,
								  &CColorSetWinNative::DrawRotatedBitsLeft);
				break;

			case xtpTabPositionRight:
				DrawRotatedButton(pDC, rc, pItem, TRUE, NULL,
								  &CColorSetWinNative::DrawRotatedBitsRight);
				break;
		}
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rc, TRUE);
}
