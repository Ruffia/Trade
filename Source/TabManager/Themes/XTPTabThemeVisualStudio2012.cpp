// XTPTabThemeVisualStudio2012.cpp
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
#include "TabManager/Themes/XTPTabThemeVisualStudio2012.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabThemeVisualStudio2012::CXTPTabThemeVisualStudio2012()
	: CXTPTabThemeFlat()
{
}

void CXTPTabThemeVisualStudio2012::RefreshMetrics()
{
	CXTPTabThemeFlat::RefreshMetrics();
}

void CXTPTabThemeVisualStudio2012::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());

	CXTPTabPaintManagerColorSet* pColorSet = GetColorSet();

	pColorSet->FillPropertyButton(pDC, rcItem, pItem);
	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}

void CXTPTabThemeVisualStudio2012::DrawWorkspacePart(CDC* pDC, LPRECT lpRect,
													 XTPTabWorkspacePart workspacePart)
{
	CRect rc(lpRect);

	switch (workspacePart)
	{
		case xtpTabWorkspacePartBorder:
		{
			// fill non client border.
			pDC->Draw3dRect(rc.left, rc.top, rc.Width(), rc.Height(),
							GetColorSet()->m_clrTabClientBack, GetColorSet()->m_clrTabClientBack);
			rc.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
			pDC->Draw3dRect(rc.left, rc.top, rc.Width(), rc.Height(),
							GetColorSet()->m_clrTabClientBack, GetColorSet()->m_clrTabClientBack);
			rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

			// draw single line border on the bottom.
			CXTPPenDC pen(*pDC, PS_SOLID, 1, GetColorSet()->m_clrTabClientBorder);
			pDC->MoveTo(rc.left, rc.bottom - XTP_DPI_Y(1));
			pDC->LineTo(rc.right, rc.bottom - XTP_DPI_Y(1));
		}
		break;

		case xtpTabWorkspacePartWidth:

			if (!m_pPaintManager->m_bStaticFrame)
			{
				lpRect->top -= XTP_DPI_Y(2);
				lpRect->left -= XTP_DPI_X(2);
				lpRect->right += XTP_DPI_X(2);
				lpRect->bottom += XTP_DPI_Y(2);
			}
			else
			{
				lpRect->top -= XTP_DPI_Y(1);
			}

			break;

		case xtpTabWorkspacePartVSplitter:
			pDC->FillSolidRect(rc, GetColorSet()->m_clrHeaderFace.clrLight);
			pDC->FillSolidRect(rc.right - XTP_DPI_X(1), rc.top, XTP_DPI_X(1), rc.bottom - rc.top,
							   GetColorSet()->m_clrFrameBorder);
			break;

		case xtpTabWorkspacePartHSplitter:
			pDC->FillSolidRect(rc, GetColorSet()->m_clrHeaderFace.clrLight);
			pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.right - rc.left, XTP_DPI_Y(1),
							   GetColorSet()->m_clrFrameBorder);
			break;

		case xtpTabWorkspacePartBack:
			pDC->FillSolidRect(rc, GetColorSet()->m_clrTabClientBack);
			break;
	}
}
