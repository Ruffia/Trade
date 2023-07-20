// XTPDockingPaneThemeVisualStudio6.cpp : implementation of the CXTPDockingPaneVisualStudio6Theme
// class.
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

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPanePaintManager.h"

#include "DockingPane/Themes/XTPDockingPaneThemeDefault.h"
#include "DockingPane/Themes/XTPDockingPaneThemeGrippered.h"
#include "DockingPane/Themes/XTPDockingPaneThemeVisualStudio6.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio6Theme

CXTPDockingPaneVisualStudio6Theme::CXTPDockingPaneVisualStudio6Theme()
{
	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
}

void CXTPDockingPaneVisualStudio6Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane,
														CRect rcCaption, const CString& strTitle,
														BOOL bActive, BOOL bVertical)
{
	UNREFERENCED_PARAMETER(strTitle);
	UNREFERENCED_PARAMETER(bActive);

	XTPDeflateRectDirection(rcCaption, 0, 0, XTP_DPI_X(2), 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, m_clrNormalCaptionText,
					   XTP_DPI_X(14), XTP_DPI_Y(2), bVertical);

	int nOffsetX = XTP_DPI_X(4);
	int nOffsetY = XTP_DPI_Y(4);
	XTPDeflateRectDirection(rcCaption, XTP_DPI_X(4), XTP_DPI_Y(1), XTP_DPI_X(2), 0, bVertical);

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if (NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	if (bVertical)
	{
		if (rcCaption.bottom > nOffsetX)
		{
			int nCenter = rcCaption.CenterPoint().x;
			pDC->Draw3dRect(nCenter - XTP_DPI_X(3), nOffsetY, XTP_DPI_X(3),
							rcCaption.bottom - nOffsetY, GetXtremeColor(COLOR_BTNHILIGHT),
							GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nCenter + XTP_DPI_X(1), nOffsetY, XTP_DPI_X(3),
							rcCaption.bottom - nOffsetY, GetXtremeColor(COLOR_BTNHILIGHT),
							GetXtremeColor(COLOR_3DSHADOW));
		}
	}
	else
	{
		if (rcCaption.right > nOffsetX)
		{
			int nCenter = rcCaption.CenterPoint().y;
			pDC->Draw3dRect(nOffsetX, nCenter - XTP_DPI_Y(3), rcCaption.right - nOffsetX,
							XTP_DPI_Y(3), GetXtremeColor(COLOR_BTNHILIGHT),
							GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nOffsetX, nCenter + XTP_DPI_Y(1), rcCaption.right - nOffsetX,
							XTP_DPI_Y(3), GetXtremeColor(COLOR_BTNHILIGHT),
							GetXtremeColor(COLOR_3DSHADOW));
		}
	}
}
