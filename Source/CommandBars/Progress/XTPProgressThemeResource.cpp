// XTPProgressThemeResource.cpp : implementation of the CXTPProgressResourceTheme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/Controls/Progress/XTPProgressBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"
#include "CommandBars/Themes/XTPCommandBarsResourceTheme.h"

#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlProgress.h"
#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/Progress/XTPProgressThemeResource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPProgressResourceTheme

CXTPProgressResourceTheme::CXTPProgressResourceTheme(CXTPPaintManager* pPaintManager)
	: CXTPProgressPaintManager(pPaintManager)
{
	m_cyProgress = XTP_DPI_Y(12);
}

void CXTPProgressResourceTheme::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	if (!XTPResourceImages()->IsValid())
		return;

	CXTPResourceTheme* pPaintManager = (CXTPResourceTheme*)m_pPaintManager;

	CXTPResourceImage* pImage = pPaintManager->LoadImage(_T("PROGRESSTRACK"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rc = pProgressBar->GetProgressRect();

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(2, 2, 2, 2), 0xFF00FF);

	int nLower, nUpper, nPos;
	pProgressBar->GetRange(nLower, nUpper);
	nPos = pProgressBar->GetPos();

	rc.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));

	int nWidth = rc.Width();

	int x = MulDiv(nWidth, nPos - nLower, nUpper - nLower);

	pImage = pPaintManager->LoadImage(_T("PROGRESSCHUNK"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(pImage->GetSource());
	rcSrc.right -= 4;

	CRect rcDest(rc.left, rc.top, rc.left + x, rc.bottom);

	if (rcDest.Width() < XTP_DPI_X(rcSrc.Width()))
		rcSrc.left = rcSrc.right - XTP_UNDPI_X(rcDest.Width());

	pImage->DrawImage(pDC, rcDest, rcSrc, CRect(2, 2, 2, 2), 0xFF00FF);

	if (rc.left + x < rc.right - XTP_DPI_X(1))
	{
		int nShadow = min(XTP_DPI_X(4), rc.right - rc.left - x);
		rcSrc = CRect(rcSrc.right, rcSrc.top, rcSrc.right + XTP_UNDPI_X(nShadow), rcSrc.bottom);

		rcDest = CRect(rcDest.right, rcDest.top, rcDest.right + nShadow, rcDest.bottom);

		pImage->DrawImage(pDC, rcDest, rcSrc, CRect(0, 2, 0, 2), 0xFF00FF);
		return;
	}
}
