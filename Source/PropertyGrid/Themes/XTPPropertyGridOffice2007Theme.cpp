// XTPPropertyGridOffice2007Theme.cpp : implementation of the CXTPPropertyGridOffice2007Theme class.
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridInplaceButton.h"
#include "PropertyGrid/XTPPropertyGridItem.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridOffice2007Theme

CXTPPropertyGridOffice2007Theme::CXTPPropertyGridOffice2007Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridOffice2003Theme(pGrid)
{
}

void CXTPPropertyGridOffice2007Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_bLunaTheme = FALSE;

	// m_pMetrics->m_clrBack.SetCustomValue(RGB(255, 255, 255));
	// m_pMetrics->m_clrFore.SetCustomValue(RGB(0, 0, 0));

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("PUSHBUTTON"));
	if (pImage)
	{
		m_clrFace.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("Background")));
		m_clrShadow.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("Shadow")));
		m_pMetrics->m_clrHelpBack.SetStandardValue(
			pImages->GetImageColor(_T("PropertyGrid"), _T("HelpBack")));
		m_pMetrics->m_clrHelpFore.SetStandardValue(
			pImages->GetImageColor(_T("PropertyGrid"), _T("HelpFore")));
		m_pMetrics->m_clrLine.SetStandardValue(
			pImages->GetImageColor(_T("PropertyGrid"), _T("GridLine")));
		m_pMetrics->m_clrCategoryFore.SetStandardValue(
			pImages->GetImageColor(_T("PropertyGrid"), _T("CategoryText")));
		m_clrHighlightText.SetStandardValue(
			pImages->GetImageColor(_T("PropertyGrid"), _T("HighlightText")));
		m_clrHighlight.SetStandardValue(
			pImages->GetImageColor(_T("PropertyGrid"), _T("Highlight")));
	}
	else
	{
		m_clrFace.SetStandardValue(RGB(173, 209, 255));
		m_clrShadow.SetStandardValue(RGB(101, 147, 207));
		m_pMetrics->m_clrHelpBack.SetStandardValue(RGB(214, 232, 255));
		m_pMetrics->m_clrHelpFore.SetStandardValue(RGB(0, 0, 0));
		m_pMetrics->m_clrLine.SetStandardValue(RGB(214, 232, 255));
		m_pMetrics->m_clrCategoryFore.SetStandardValue(RGB(21, 66, 139));
		m_clrHighlightText.SetStandardValue(0);
		m_clrHighlight.SetStandardValue(RGB(179, 200, 232));
	}
	m_pMetrics->m_clrVerbFace.SetStandardValue(RGB(0, 102, 204));
}

void CXTPPropertyGridOffice2007Theme::DrawRadioMark(CDC* pDC, CRect rc, BOOL bEnabled,
													BOOL bChecked)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONRADIOBUTTON"));
	if (!pImage)
	{
		CXTPPropertyGridOffice2003Theme::DrawCheckMark(pDC, rc, bEnabled, bChecked);
		return;
	}

	int nState = !bEnabled ? 3 : 0;
	if (bChecked)
		nState += 4;
	CRect rcSrc(pImage->GetSource(nState, 8));

	CRect rcSample(CPoint(rc.left, rc.CenterPoint().y - rcSrc.Height() / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcSample, rcSrc, CRect(0, 0, 0, 0));
}

void CXTPPropertyGridOffice2007Theme::DrawCheckMark(CDC* pDC, CRect rc, BOOL bEnabled,
													BOOL bChecked)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONCHECKBOX"));
	if (!pImage)
	{
		CXTPPropertyGridOffice2003Theme::DrawCheckMark(pDC, rc, bEnabled, bChecked);
		return;
	}

	int nState = !bEnabled ? 3 : 0;
	if (bChecked)
		nState += 4;
	CRect rcSrc(pImage->GetSource(nState, 12));

	CRect rcSample(CPoint(rc.left, rc.CenterPoint().y - rcSrc.Height() / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcSample, rcSrc, CRect(0, 0, 0, 0));
}

void CXTPPropertyGridOffice2007Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem,
													   CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	if (m_buttonsStyle == xtpPropertyGridButtonsThemed)
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPResourceImage* pImage = pImages->LoadFile(_T("GRIDEXPANDBUTTON"));

		if (!pImage)
		{
			CXTPPropertyGridOffice2003Theme::DrawExpandButton(dc, pItem, rcCaption);
		}
		else
		{
			CRect rcSign(CPoint(XTP_DPI_X(XTP_PGI_EXPAND_BORDER) / 2 - XTP_DPI_X(8),
								rcCaption.CenterPoint().y - XTP_DPI_Y(8)),
						 XTP_DPI(CSize(16, 16)));

			if (pItem->GetIndent() > 0)
				rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1))
									  * XTP_DPI_X(XTP_PGI_EXPAND_BORDER),
								  0);

			CRect rcSrc(pImage->GetSource(pItem->IsExpanded() ? 0 : 2, 4));

			pImage->DrawImage(&dc, rcSign, rcSrc);
		}
	}
	else
	{
		CXTPPropertyGridOffice2003Theme::DrawExpandButton(dc, pItem, rcCaption);
	}
}

void CXTPPropertyGridOffice2007Theme::FillInplaceButton(CDC* pDC,
														CXTPPropertyGridInplaceButton* pButton)
{
	CRect rect(pButton->GetRect());
	CPoint ptDots(rect.CenterPoint().x, rect.CenterPoint().y + XTP_DPI_Y(3));
	COLORREF clrText = GetXtremeColor(UINT(pButton->GetEnabled() ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

	if (pButton->IsHyperlink())
	{
		clrText = !pButton->GetEnabled() ? GetXtremeColor(COLOR_GRAYTEXT)
										 : m_pMetrics->m_clrVerbFace.operator COLORREF();
	}
	else
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPResourceImage* pImage = pImages->LoadFile(_T("PUSHBUTTON"));
		if (pImage)
		{
			int nState = pButton->IsPressed() && pButton->IsHot() ? 2 : pButton->IsHot() ? 1 : 0;
			pImage->DrawImage(pDC, rect, pImage->GetSource(nState, 5), CRect(3, 3, 3, 3), 0xFF00FF);
		}
		else
		{
			pDC->FillSolidRect(rect, pButton->IsPressed() && pButton->IsHot()
										 ? RGB(255, 171, 63)
										 : pButton->IsHot() ? RGB(255, 231, 162)
															: m_clrFace.operator DWORD());

			COLORREF clrBorder = GetXtremeColor(
				pButton->IsPressed() && pButton->IsHot()
					? RGB(251, 140, 60)
					: pButton->IsHot() ? RGB(255, 189, 105) : m_clrShadow.operator DWORD());
			pDC->Draw3dRect(rect, clrBorder, clrBorder);
		}
	}

	if (pButton->GetID() == XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		CRect rcArrow = rect;
		rcArrow.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow, clrText);
	}

	BOOL bCustom = FALSE;

	if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		CXTPImageManagerIcon* pIcon = pButton->GetImage();
		if (pIcon)
		{
			CSize sz;
			if (pIcon->IsVectorIcon())
				sz = CSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
			else
				sz = CSize(pIcon->GetWidth(), pIcon->GetHeight());

			sz.cx = min(sz.cx, rect.Width());
			sz.cy = min(sz.cy, rect.Height());

			CPoint pt((rect.left + rect.right - sz.cx) / 2, (rect.top + rect.bottom - sz.cy) / 2);
			pIcon->Draw(pDC, pt, sz);
			bCustom = TRUE;
		}
		else if (!pButton->GetCaption().IsEmpty())
		{
			CXTPFontDC font(pDC, pButton->IsHyperlink() ? &m_pMetrics->m_xtpFontUnderline
														: GetItemFont(pButton->GetItem(), TRUE));
			pDC->SetTextColor(clrText);
			pDC->DrawText(pButton->GetCaption(), rect,
						  DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
			bCustom = TRUE;
		}
	}

	if (!bCustom && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)
	{
		CRect rcEllipsis = rect;
		rcEllipsis.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEllipsis, rcEllipsis, clrText);
	}

	if (pButton->IsFocused())
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		rect.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2));
		pDC->DrawFocusRect(rect);
	}
}

XTPScrollBarTheme CXTPPropertyGridOffice2007Theme::GetScrollBarTheme() const
{
	return xtpScrollBarThemeResource;
}
