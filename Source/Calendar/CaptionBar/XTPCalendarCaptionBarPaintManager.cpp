// XTPCalendarCaptionBarPaintManager.cpp : implementation file
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPWinThemeWrapper.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarThemePart.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarTheme.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarCaptionBarPaintManager

CXTPCalendarCaptionBarPaintManager::CXTPCalendarCaptionBarPaintManager(
	CXTPCalendarCaptionBarTheme* pTheme /*=NULL*/)
	: m_nPaintTheme(xtpCalendarThemeUnknown)
	, m_pTheme(pTheme)
	, m_pThemeButton(new CXTPWinThemeWrapper())
{
}

CXTPCalendarCaptionBarPaintManager::~CXTPCalendarCaptionBarPaintManager()
{
	SAFE_DELETE(m_pThemeButton);
}

void CXTPCalendarCaptionBarPaintManager::RefreshMetrics()
{
	m_pThemeButton->OpenThemeData(NULL, L"BUTTON");
}

void CXTPCalendarCaptionBarPaintManager::Draw(CDC* pDC)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	CRect rcTopPart = m_pTheme->m_pButtonViewDay->GetRect();

	if (m_pTheme->IsSwitchViewButtons())
	{
		// Fill SwitchViewBar
		rcTopPart.right = m_pTheme->m_rcRect.right;
		pDC->FillSolidRect(rcTopPart, m_clrSwitchViewBarBk);

		rcTopPart.top = rcTopPart.bottom;
		rcTopPart.bottom += XTP_DPI_Y(1);

		pDC->FillSolidRect(rcTopPart, m_clrBorders[1]);
	}

	// Fill ScrollDateBar
	CRect rcBottomPart = m_pTheme->m_rcRect;
	rcBottomPart.top   = rcTopPart.bottom;
	pDC->FillSolidRect(rcBottomPart, m_clrScrollDateBarBk);

	// DrawBorders(pDC, m_pTheme->m_rcRect, m_pTheme->m_rcRect);

	// Draw members
	int nCount = m_pTheme->m_arMembers.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarCaptionBarThemePart* pPart = m_pTheme->m_arMembers.GetAt(i);
		ASSERT(pPart);
		if (pPart)
			pPart->Draw(pDC);
	}
}

void CXTPCalendarCaptionBarPaintManager::DrawBorders(CDC* pDC, const CRect& rcRect,
													 const CRect& rcBorders)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	// Draw borders
	CRect rc;
	if (rcBorders.left)
	{
		rc		 = rcRect;
		rc.right = rc.left + rcBorders.left;
		pDC->FillSolidRect(rc, m_clrBorders[0]);
	}

	if (rcBorders.top)
	{
		rc		  = rcRect;
		rc.bottom = rc.top + rcBorders.top;
		pDC->FillSolidRect(rc, m_clrBorders[1]);
	}

	if (rcBorders.right)
	{
		rc		= rcRect;
		rc.left = rc.right - rcBorders.right;
		pDC->FillSolidRect(rc, m_clrBorders[2]);
	}

	if (rcBorders.bottom)
	{
		rc	 = rcRect;
		rc.top = rc.bottom - rcBorders.bottom;
		pDC->FillSolidRect(rc, m_clrBorders[3]);
	}
}

void CXTPCalendarCaptionBarPaintManager::DrawTriangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2,
													  COLORREF clrColor)
{
	ASSERT(pDC);

	if (!pDC || !pDC->GetSafeHdc())
		return;

	CXTPPenDC pen(*pDC, clrColor);
	CXTPBrushDC brush(*pDC, clrColor);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}
