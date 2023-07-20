// XTPProgressThemeOffice2013.cpp : implementation of the CXTPProgressThemeOffice2013 class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/Controls/Progress/XTPProgressBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlProgress.h"
#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/Progress/XTPProgressThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPProgressThemeOffice2013

CXTPProgressThemeOffice2013::CXTPProgressThemeOffice2013(CXTPPaintManager* pPaintManager)
	: CXTPProgressPaintManager(pPaintManager)
{
}

CXTPProgressThemeOffice2013::~CXTPProgressThemeOffice2013()
{
}

void CXTPProgressThemeOffice2013::RefreshMetrics()
{
	m_clrBorder.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Border"), RGB(188, 188, 188)));
	m_clrBar.SetStandardValue(XTPIniColor(_T("Controls.ProgressBar"), _T("Bar"), RGB(6, 176, 37)));
	m_clrBack.SetStandardValue(
		XTPIniColor(_T("Controls.ProgressBar"), _T("Back"), RGB(230, 230, 230)));
}

void CXTPProgressThemeOffice2013::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	CRect rc(pProgressBar->GetProgressRect());

	int nLower, nUpper;
	pProgressBar->GetRange(nLower, nUpper);
	int nPos = pProgressBar->GetPos();

	CRect rcClient(rc);
	rcClient.DeflateRect(2, 2);

	int nWidth = rcClient.right - rcClient.left;
	int x	  = MulDiv(nWidth, nPos - nLower, nUpper - nLower);

	pDC->FillSolidRect(rc, m_clrBack);
	pDC->Draw3dRect(rc, m_clrBorder, m_clrBorder);

	if (rcClient.Width() > 1)
	{
		pDC->FillSolidRect(rcClient.left, rcClient.top, x, rcClient.Height(), m_clrBar);
	}
}
