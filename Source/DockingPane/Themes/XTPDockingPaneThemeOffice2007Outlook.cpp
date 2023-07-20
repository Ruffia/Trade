// XTPDockingPaneThemeOffice2007Outlook.cpp : implementation of the
// CXTPDockingPaneOffice2007OutlookTheme class.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPanePaintManager.h"
#include "DockingPane/XTPDockingPaneTabbedContainer.h"
#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2003.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2007.h"
#include "DockingPane/Themes/XTPDockingPaneThemeOffice2007Outlook.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007OutlookTheme

CXTPDockingPaneOffice2007OutlookTheme::CXTPDockingPaneOffice2007OutlookTheme()
{
	m_nCaptionFontGap = XTP_DPI_X(3);
}

void CXTPDockingPaneOffice2007OutlookTheme::DrawCaption(CDC* pDC,
														CXTPDockingPaneTabbedContainer* pPane,
														CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - XTP_DPI_X(3), 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - XTP_DPI_Y(3));
	}

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcCaption,
									   bActive ? m_clrActiveCaption : m_clrNormalCaption,
									   bVertical);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	if (bVertical)
	{
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1,
						   GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(),
						   GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.bottom - 1, rcCaption.Width(), 1,
						   GetXtremeColor(XPCOLOR_FRAME));

		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, rcCaption.Width() - 1, 1,
						   GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, 1, rcCaption.Height() - 2,
						   GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
	else
	{
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1,
						   GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(),
						   GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.right - 1, rcCaption.top, 1, rcCaption.Height(),
						   GetXtremeColor(XPCOLOR_FRAME));

		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, rcCaption.Width() - 2, 1,
						   GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, 1, rcCaption.Height() - 1,
						   GetXtremeColor(COLOR_3DHIGHLIGHT));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

void CXTPDockingPaneOffice2007OutlookTheme::RefreshMetrics()
{
	CXTPDockingPaneOffice2007Theme::RefreshMetrics();

	if (m_bUseStandardFont)
	{
		LOGFONT lf;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));

		CString strOfficeFont = _T("Segoe UI");

		BOOL bOfficeFont = XTPDrawHelpers()->FontExists(m_strOfficeFont);

		if (bOfficeFont)
		{
			STRCPY_S(lf.lfFaceName, LF_FACESIZE, strOfficeFont);
			if (XTPSystemVersion()->IsClearTypeTextQualitySupported())
				lf.lfQuality = 5;
		}
		else
		{
			STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		}

		lf.lfWeight = FW_BOLD;
		lf.lfHeight = XTP_DPI_Y(20);

		SetCaptionFontIndirect(&lf);
	}
}
