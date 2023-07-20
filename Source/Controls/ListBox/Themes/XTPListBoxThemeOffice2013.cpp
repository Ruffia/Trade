// XTPListBoxThemeOffice2013.cpp : implementation of the CXTPListBoxThemeOffice2013 class.
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
#include "Common/XTPResourceImage.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/ListBox/Themes/XTPListBoxThemeOffice2013.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPListBoxThemeOffice2013::CXTPListBoxThemeOffice2013()
{
	RefreshMetrics();
}

void CXTPListBoxThemeOffice2013::DrawItem(CDC* pDC, CXTPListBox* pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	if ((lpDIS->itemID != (UINT)-1) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		CRect rcItem(lpDIS->rcItem);

		BOOL bSelected  = ((lpDIS->itemState & ODS_SELECTED) == ODS_SELECTED);
		BOOL bHighlight = (int)lpDIS->itemID == pListBox->m_nHotItem;

		COLORREF clrText;
		COLORREF clrBack;
		COLORREF clrBorder;

		if (bSelected)
		{
			clrText   = m_clrItemTextSelected;
			clrBack   = m_clrItemBackSelected;
			clrBorder = m_clrItemBorderSelected;
		}
		else if (bHighlight)
		{
			clrText   = m_clrItemTextHighlight;
			clrBack   = m_clrItemBackHighlight;
			clrBorder = m_clrItemBorderHighlight;
		}
		else
		{
			clrText   = m_clrItemText;
			clrBack   = m_clrItemBack;
			clrBorder = m_clrItemBorder;
		}

		pDC->SetBkColor(clrBack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);

		pDC->FillSolidRect(rcItem, clrBack);
		pDC->Draw3dRect(rcItem, clrBorder, clrBorder);

		CRect rcText(rcItem);
		rcText.DeflateRect(pListBox->m_nTextPadding, 0);
		DrawItemText(pDC, rcText, pListBox, lpDIS);
	}
}

void CXTPListBoxThemeOffice2013::RefreshMetrics()
{
	// Defaults are set to OFFICE2013WORD_INI colors.

	m_clrBack			   = XTPIniColor(_T("Controls.ListBox"), _T("Back"), RGB(255, 255, 255));
	m_clrBorder			   = XTPIniColor(_T("Controls.ListBox"), _T("Border"), RGB(171, 171, 171));
	m_clrItemText		   = XTPIniColor(_T("Controls.ListBox"), _T("ItemText"), RGB(68, 68, 68));
	m_clrItemTextSelected  = XTPIniColor(_T("Controls.ListBox"), _T("ItemTextSelected"),
										 RGB(255, 255, 255));
	m_clrItemTextHighlight = XTPIniColor(_T("Controls.ListBox"), _T("ItemTextHighlight"),
										 RGB(68, 68, 68));
	m_clrItemBack		  = XTPIniColor(_T("Controls.ListBox"), _T("ItemBack"), RGB(255, 255, 255));
	m_clrItemBackSelected = XTPIniColor(_T("Controls.ListBox"), _T("ItemBackSelected"),
										RGB(43, 87, 154));
	m_clrItemBackHighlight = XTPIniColor(_T("Controls.ListBox"), _T("ItemBackHighlight"),
										 RGB(213, 225, 242));
	m_clrItemBorder = XTPIniColor(_T("Controls.ListBox"), _T("ItemBorder"), RGB(255, 255, 255));
	m_clrItemBorderSelected  = XTPIniColor(_T("Controls.ListBox"), _T("ItemBorderSelected"),
										   RGB(43, 87, 154));
	m_clrItemBorderHighlight = XTPIniColor(_T("Controls.ListBox"), _T("ItemBorderHighlight"),
										   RGB(163, 189, 227));
}

COLORREF CXTPListBoxThemeOffice2013::GetBackColor()
{
	return m_clrItemBack;
}

BOOL CXTPListBoxThemeOffice2013::DrawNcBorders(CDC* pDC, CRect rc)
{
	if (pDC->GetSafeHdc())
	{
		pDC->Draw3dRect(rc, m_clrBorder, m_clrBorder);
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, m_clrBack, m_clrBack);
		return TRUE;
	}

	return FALSE;
}
