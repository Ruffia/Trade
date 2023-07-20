// XTPCommandBarsVisualStudio6Theme.cpp : implementation of the CXTPVisualStudio6Theme class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPSystemHelpers.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"

#include "CommandBars/Themes/XTPCommandBarsOffice2000Theme.h"
#include "CommandBars/Themes/XTPCommandBarsVisualStudio6Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPVisualStudio6Theme

CSize CXTPVisualStudio6Theme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	if (pBar->IsDialogBar() || pBar->GetPosition() == xtpBarFloating)
		return CXTPCommandBarsOffice2000Theme::DrawCommandBarGripper(pDC, pBar, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarRight || pBar->GetPosition() == xtpBarLeft)
	{
		if (pDC && bDraw)
		{
			Draw3dRect(pDC,
					   CRect(XTP_DPI_X(3), XTP_DPI_Y(3), rc.right - XTP_DPI_X(3), XTP_DPI_Y(6)),
					   COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			Draw3dRect(pDC,
					   CRect(XTP_DPI_X(3), XTP_DPI_Y(7), rc.right - XTP_DPI_X(3), XTP_DPI_Y(10)),
					   COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		}
		return CSize(0, XTP_DPI_Y(10));
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		if (pDC && bDraw)
		{
			Draw3dRect(pDC,
					   CRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(6), rc.bottom - XTP_DPI_Y(3)),
					   COLOR_BTNHILIGHT, COLOR_3DSHADOW);
			Draw3dRect(pDC,
					   CRect(XTP_DPI_X(7), XTP_DPI_Y(3), XTP_DPI_X(10), rc.bottom - XTP_DPI_Y(3)),
					   COLOR_BTNHILIGHT, COLOR_3DSHADOW);
		}
		return CSize(XTP_DPI_X(10), 0);
	}
	return 0;
}
