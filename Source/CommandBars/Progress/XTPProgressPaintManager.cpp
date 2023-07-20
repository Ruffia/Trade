// XTPProgressPaintManager.cpp : implementation of the CXTPProgressPaintManager class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/Controls/Progress/XTPProgressBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlProgress.h"
#include "CommandBars/Progress/XTPProgressPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPProgressPaintManager

IMPLEMENT_DYNAMIC(CXTPProgressPaintManager, CObject);

CXTPProgressPaintManager::CXTPProgressPaintManager(CXTPPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
	, m_cyProgress(XTP_DPI_Y(16))
{
	ASSERT(NULL != pPaintManager);
}

CXTPProgressPaintManager::~CXTPProgressPaintManager()
{
}

void CXTPProgressPaintManager::RefreshMetrics()
{
}

void CXTPProgressPaintManager::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	CRect rc(pProgressBar->GetProgressRect());

	BOOL bThemed   = FALSE;
	BOOL bVertical = FALSE;

	int nLower, nUpper, nPos;
	pProgressBar->GetRange(nLower, nUpper);
	nPos = pProgressBar->GetPos();

	int nWidth, dxBlock;

	CRect rcClient(rc);

	if (!bThemed)
		rcClient.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));
	else if (bVertical)
		rcClient.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(4));
	else
		rcClient.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(3));

	if (bVertical)
	{
		nWidth  = rcClient.bottom - rcClient.top;
		dxBlock = (rcClient.right - rcClient.left) * 2 / 3;
	}
	else
	{
		nWidth  = rcClient.right - rcClient.left;
		dxBlock = (rcClient.bottom - rcClient.top) * 2 / 3;
	}

	int x = MulDiv(nWidth, nPos - nLower, nUpper - nLower);

	if (!bThemed)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_BTNFACE));
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));

		if (rcClient.Width() < XTP_DPI_X(2))
			return;

		pDC->FillSolidRect(rcClient.left, rcClient.top, x, rcClient.Height(),
						   GetXtremeColor(COLOR_HIGHLIGHT));
	}
}
