// XTPListBoxThemeOfficeDefault.cpp : implementation of the CXTPListBoxThemeDefault class.
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
#include "Common/XTPCasting.h"

#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeDefault.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPListBoxThemeDefault::DrawItem(CDC* pDC, CXTPListBox* pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem = lpDIS->rcItem;

	if ((lpDIS->itemID != (UINT)-1) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		COLORREF clrWindow	 = GetBackColor();
		COLORREF clrWindowText = pListBox->IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOWTEXT)
															 : GetXtremeColor(COLOR_GRAYTEXT);
		BOOL bSelected = ((lpDIS->itemState & ODS_SELECTED) != 0);
		CRect rcText(rcItem);
		rcText.DeflateRect(pListBox->m_nTextPadding, 0);

		if (bSelected)
		{
			clrWindow = pListBox->IsWindowEnabled() ? GetXtremeColor(COLOR_HIGHLIGHT)
													: GetXtremeColor(COLOR_GRAYTEXT);
			clrWindowText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		}

		/*if (pListBox->m_nStyle == xtpListBoxOfficeXP && bSelected && pListBox->IsWindowEnabled())
		{
			clrWindowText = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
			clrWindow = GetXtremeColor(XPCOLOR_HIGHLIGHT);
			pDC->Draw3dRect(rcItem, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
		GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER)); rcItem.DeflateRect(1, 1);
		}

		if (m_nStyle == xtpListBoxOffice2007)
		{
			CXTPResourceImages* pImages = XTPResourceImages();

			BOOL bHasFocus = ::GetFocus() == m_hWnd;
			BOOL bHighlighted = (int)lpDIS->itemID == m_nHotItem;

			if ((bSelected || bHighlighted) && IsWindowEnabled())
			{
				CXTPResourceImage* pImage = pImages->LoadFile(_T("LISTBOX"));
				if (pImage)
				{
					pImage->DrawImage(pDC, rcItem, pImage->GetSource(bSelected && bHighlighted ? 2 :
						bHasFocus && bSelected ? 1 : !bHasFocus && bSelected ? 3 : 0, 4), CRect(4,
		4, 4, 4), COLORREF_NULL); clrWindow = COLORREF_NULL;
				}
			}

			pDC->SetBkMode(TRANSPARENT);
			clrWindowText = pImages->GetImageColor(_T("LISTBOX"), _T("NormalText"));
		}*/

		// set the text and text background colors, then repaint the item.
		pDC->SetBkColor(clrWindow);
		pDC->SetTextColor(clrWindowText);
		if (clrWindow != COLORREF_NULL)
			pDC->FillSolidRect(&rcItem, clrWindow);

		CString strText;
		pListBox->GetText(XTPToInt(lpDIS->itemID), strText);

		pDC->DrawText(strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
	}

	if ((lpDIS->itemAction & ODA_FOCUS))
		pDC->DrawFocusRect(&lpDIS->rcItem);
}

void CXTPListBoxThemeDefault::RefreshMetrics()
{
}
