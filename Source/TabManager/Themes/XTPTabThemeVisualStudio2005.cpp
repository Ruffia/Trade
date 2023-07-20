// XTPTabThemeVisualStudio2005.cpp
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

#include "TabManager/Includes.h"
#include "TabManager/Themes/XTPTabThemePropertyPage2003.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2005.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CAppearanceSetVisualStudio2005

CRect CAppearanceSetVisualStudio2005::GetButtonDrawRect(const CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	return pItem->GetTabManager()->IsHorizontalPosition()
			   ? CRect(rc.left - rc.Height() / 2, rc.top, rc.right + rc.Height() / 2, rc.bottom)
			   : CRect(rc.left, rc.top - rc.Width() / 2, rc.right, rc.bottom + rc.Width() / 2);
}

void CAppearanceSetVisualStudio2005::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	COLORREF clrBorder		  = GetColorSet()->m_csPropertyPage2003.clrDarkShadow,
			 clrLeftHighlight = GetColorSet()->m_csPropertyPage2003.clrBorderLeftHighlight,
			 clrTopHighlight  = GetColorSet()->m_csPropertyPage2003.clrBorderTopHighlight;
	if (pItem->IsSelected())
	{
		clrBorder		 = GetColorSet()->m_csPropertyPage2003.clrFrameBorder;
		clrLeftHighlight = RGB(255, 255, 255);
		clrTopHighlight  = RGB(255, 255, 255);
	}

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionBottom:
		{
			int nHeight2 = rc.Height() / 2;

			POINT ptsBorder[] = { { rc.right + nHeight2 - 4, rc.top },
								  { -nHeight2 + 3, nHeight2 * 2 - 6 },
								  { -4, 4 },
								  { -2, 1 },
								  { -rc.Width() + 14, 0 },
								  { -2, -1 },
								  { -3, -3 },
								  { -nHeight2 + 3 - 1, -(nHeight2 * 2 - 6 + 2) } };
			FillButton(pDC, pItem, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]),
					   pItem->GetTabManager()->GetPosition());
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsRightBorder[] = { { rc.right + nHeight2 - 4 - 1, rc.top },
									   { -nHeight2 + 3, nHeight2 * 2 - 6 },
									   { -3, 3 },
									   { -2, 1 } };
			DrawPolyLine(pDC, clrLeftHighlight, ptsRightBorder,
						 sizeof(ptsRightBorder) / sizeof(ptsRightBorder[0]));

			POINT ptsLeftBorder[] = { { rc.right - 4 + 3 - 4 - 2 - rc.Width() + 14,
										rc.top + nHeight2 * 2 - 6 + 4 },
									  { -2, -1 },
									  { -2, -2 },
									  { -nHeight2 + 3 - 1, -(nHeight2 * 2 - 6 + 2) } };
			DrawPolyLine(pDC, clrTopHighlight, ptsLeftBorder,
						 sizeof(ptsLeftBorder) / sizeof(ptsLeftBorder[0]));
		}
		break;

		case xtpTabPositionTop:
		{
			int nHeight2 = rc.Height() / 2;

			POINT ptsBorder[] = { { rc.right + nHeight2 - 4, rc.bottom - 1 },
								  { -nHeight2 + 3, -nHeight2 * 2 + 6 },
								  { -4, -4 },
								  { -3, -1 },
								  { -rc.Width() + 16, 0 },
								  { -3, 1 },
								  { -3, 3 },
								  { -nHeight2 + 3 - 1, (nHeight2 * 2 - 6 + 2) } };

			FillButton(pDC, pItem, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]),
					   pItem->GetTabManager()->GetPosition());
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsRightBorder[] = { { rc.right + nHeight2 - 4 - 1, rc.bottom - 1 },
									   { -nHeight2 + 3, -nHeight2 * 2 + 6 },
									   { -3, -3 },
									   { -3, -1 } };
			DrawPolyLine(pDC, clrLeftHighlight, ptsRightBorder,
						 sizeof(ptsRightBorder) / sizeof(ptsRightBorder[0]));

			POINT ptsLeftBorder[] = { { rc.right - 4 + 3 - 4 - 2,
										rc.bottom - nHeight2 * 2 + 6 - 1 - 4 },
									  { -rc.Width() + 16 - 1, 0 },
									  { -3, 1 },
									  { -2, 2 },
									  { -nHeight2 + 3 - 1, (nHeight2 * 2 - 6 + 2) } };
			DrawPolyLine(pDC, clrTopHighlight, ptsLeftBorder,
						 sizeof(ptsLeftBorder) / sizeof(ptsLeftBorder[0]));
		}
		break;

		case xtpTabPositionRight:
		{
			int nHeight2 = rc.Width() / 2;

			POINT ptsBorder[] = { { rc.left, rc.bottom + nHeight2 - 4 },
								  { nHeight2 * 2 - 6 + 1, -nHeight2 + 3 },
								  { 4, -4 },
								  { +1, -3 },
								  { 0, -rc.Height() + 16 },
								  { -1, -3 },
								  { -3, -3 },
								  { -nHeight2 * 2 + 6 - 2, (-nHeight2 + 3 - 1) } };

			FillButton(pDC, pItem, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]),
					   pItem->GetTabManager()->GetPosition());
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsRightBorder[] = { { rc.left, rc.bottom + nHeight2 - 5 },
									   { nHeight2 * 2 - 6 + 1, -nHeight2 + 3 },
									   { 3, -3 },
									   { +1, -3 },
									   { 0, -rc.Height() + 14 } };
			DrawPolyLine(pDC, clrLeftHighlight, ptsRightBorder,
						 sizeof(ptsRightBorder) / sizeof(ptsRightBorder[0]));

			POINT ptsLeftBorder[] = { { rc.left + nHeight2 * 2 - 6 + 2 + 4 - 2,
										rc.bottom - 4 - 4 - rc.Height() + 16 - 3 },
									  { -2, -2 },
									  { -nHeight2 * 2 + 6 - 2, (-nHeight2 + 3 - 1) } };
			DrawPolyLine(pDC, clrTopHighlight, ptsLeftBorder,
						 sizeof(ptsLeftBorder) / sizeof(ptsLeftBorder[0]));
		}
		break;

		case xtpTabPositionLeft:
		{
			int nHeight2 = rc.Width() / 2;

			POINT ptsBorder[] = { { rc.right - 1, rc.bottom + nHeight2 - 4 },
								  { -(nHeight2 * 2 - 6), -nHeight2 + 3 },
								  { -4, -4 },
								  { -1, -3 },
								  { 0, -rc.Height() + 16 },
								  { 1, -3 },
								  { 3, -3 },
								  { nHeight2 * 2 - 6 + 2, (-nHeight2 + 3 - 1) } };

			FillButton(pDC, pItem, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]),
					   pItem->GetTabManager()->GetPosition());
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsRightBorder[] = { { rc.right - 1, rc.bottom + nHeight2 - 5 },
									   { -(nHeight2 * 2 - 6), -nHeight2 + 3 },
									   { -3, -3 },
									   { -1, -3 } };
			DrawPolyLine(pDC, clrLeftHighlight, ptsRightBorder,
						 sizeof(ptsRightBorder) / sizeof(ptsRightBorder[0]));

			POINT ptsLeftBorder[] = { { rc.right - 1 - (nHeight2 * 2 - 6) - 4 - 1 + 1,
										rc.bottom + nHeight2 - 4 - nHeight2 + 3 - 4 - 3 },
									  { 0, -rc.Height() + 16 },
									  { 1, -3 },
									  { 2, -2 },
									  { nHeight2 * 2 - 6 + 2, (-nHeight2 + 3 - 1) } };
			DrawPolyLine(pDC, clrTopHighlight, ptsLeftBorder,
						 sizeof(ptsLeftBorder) / sizeof(ptsLeftBorder[0]));
		}
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
