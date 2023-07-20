// XTPControlGalleryPaintManager.cpp : implementation file.
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/Gallery/XTPControlGalleryItem.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryPaintManager

IMPLEMENT_DYNAMIC(CXTPControlGalleryPaintManager, CObject)

CXTPControlGalleryPaintManager::CXTPControlGalleryPaintManager(CXTPPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	ASSERT(NULL != pPaintManager);

	m_cyPopupUp = m_cyPopupDown = XTP_DPI_Y(19);
	m_cxPopup					= XTP_DPI_X(16);
}

void CXTPControlGalleryPaintManager::DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel,
											   CRect rcItem)
{
	CXTPPaintManager* pPaintManager = m_pPaintManager;
	pDC->FillSolidRect(rcItem, pPaintManager->GetXtremeColor(XPCOLOR_LABEL));

	pDC->FillSolidRect(rcItem.left, rcItem.bottom - XTP_DPI_Y(1), rcItem.Width(), XTP_DPI_Y(1),
					   pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW));

	CXTPFontDC fnt(pDC, pPaintManager->GetRegularBoldFont());

	CRect rcText(rcItem);
	rcText.DeflateRect(XTP_DPI_X(10), 0);
	pDC->SetTextColor(pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_TEXT));
	pDC->DrawText(pLabel->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

void CXTPControlGalleryPaintManager::FillControl(CDC* pDC, CXTPControlGallery* pGallery,
												 CRect rcControl)
{
	if (!pGallery->IsTransparent())
	{
		pDC->FillSolidRect(rcControl, m_pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_FACE));
	}

	if (pGallery->IsShowBorders())
	{
		pDC->Draw3dRect(rcControl, m_clrBorder, m_clrBorder);
	}

	if (pGallery->HasBottomSeparator())
	{
		pDC->FillSolidRect(rcControl.left, rcControl.bottom - XTP_DPI_Y(2), rcControl.Width(),
						   XTP_DPI_Y(1), m_pPaintManager->GetXtremeColor(XPCOLOR_SEPARATOR));
	}
}

void CXTPControlGalleryPaintManager::RefreshMetrics()
{
	m_cyPopupUp = m_cyPopupDown = XTP_DPI_Y(19);
	m_cxPopup					= XTP_DPI_X(16);

	m_clrBorder = m_pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW);
}

void CXTPControlGalleryPaintManager::DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery)
{
	XTP_SCROLLBAR_TRACKINFO* pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO* pSBInfo	= pGallery->GetScrollBarPosInfo();

	CRect rcScroll = pSBInfo->rc;
	pDC->FillSolidRect(rcScroll, m_pPaintManager->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

	CRect rcScrollUp(rcScroll.left, rcScroll.top, rcScroll.right, rcScroll.top + m_cyPopupUp);
	CRect rcScrollDown(rcScroll.left, rcScrollUp.bottom, rcScroll.right,
					   rcScrollUp.bottom + m_cyPopupDown);
	CRect rcScrollPopup(rcScroll.left, rcScrollDown.bottom, rcScroll.right, rcScroll.bottom);

	BOOL bControlEnabled = pGallery->GetEnabled();
	BOOL nPressedHt		 = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt			 = pSBTrack ? -1 : pSBInfo->ht;

	int nState = GETPARTSTATE3(XTP_HTSCROLLUP,
							   (bControlEnabled
								&& pGallery->IsScrollButtonEnabled(XTP_HTSCROLLUP)));
	m_pPaintManager->DrawRectangle(pDC, rcScrollUp,
								   IS_PART_SELECTED(nState) || IS_PART_PRESSED(nState),
								   IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE,
								   xtpBarTypeNormal, xtpBarPopup);
	COLORREF clr = GetItemTextColor(IS_PART_ENABLED(nState), IS_PART_SELECTED(nState),
									IS_PART_PRESSED(nState), FALSE, xtpBarTypeNormal);
	CPoint pt	= rcScrollUp.CenterPoint();

	CRect rcArrow = rcScrollUp;
	rcArrow.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayUp, rcArrow, clr);

	nState = GETPARTSTATE3(XTP_HTSCROLLDOWN,
						   (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLDOWN)));
	m_pPaintManager->DrawRectangle(pDC, rcScrollDown,
								   IS_PART_SELECTED(nState) || IS_PART_PRESSED(nState),
								   IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE,
								   xtpBarTypeNormal, xtpBarPopup);
	clr = GetItemTextColor(IS_PART_ENABLED(nState), IS_PART_SELECTED(nState),
						   IS_PART_PRESSED(nState), FALSE, xtpBarTypeNormal);

	rcArrow = rcScrollDown;
	rcArrow.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow, clr);

	nState = GETPARTSTATE3(XTP_HTSCROLLPOPUP, bControlEnabled);
	m_pPaintManager->DrawRectangle(pDC, rcScrollPopup,
								   IS_PART_SELECTED(nState) || IS_PART_PRESSED(nState),
								   IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE,
								   xtpBarTypeNormal, xtpBarPopup);
	clr = GetItemTextColor(IS_PART_ENABLED(nState), IS_PART_SELECTED(nState),
						   IS_PART_PRESSED(nState), FALSE, xtpBarTypeNormal);
	pt  = rcScrollPopup.CenterPoint();

	rcScrollPopup.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3), XTP_DPI_X(3), XTP_DPI_Y(3));
	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolEjectDown, rcScrollPopup, clr);
}

COLORREF CXTPControlGalleryPaintManager::GetItemTextColor(BOOL bEnabled, BOOL bSelected,
														  BOOL bPressed, BOOL bChecked,
														  XTPBarType barType)
{
	return m_pPaintManager->GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, FALSE,
												  barType, xtpBarPopup);
}

void CXTPControlGalleryPaintManager::DrawItemBack(CDC* pDC, CRect rcItem, BOOL bEnabled,
												  BOOL bSelected, BOOL bPressed, BOOL bChecked,
												  XTPBarType barType)
{
	m_pPaintManager->DrawGalleryItemBack(pDC, rcItem, bEnabled, bSelected, bPressed, bChecked,
										 barType);
}

void CXTPControlGalleryPaintManager::DrawItem(CDC* pDC, CRect rcItem, BOOL bEnabled, BOOL bSelected,
											  BOOL bPressed, BOOL bChecked,
											  CXTPControlGalleryItem* pGalleryItem,
											  CXTPControlGallery* pGallery)
{
	ASSERT(pGallery);
	ASSERT(pGalleryItem);

	CXTPMarkupUIElement* pMarkupUIElement = pGalleryItem->GetMarkupUIElement();
	if (pMarkupUIElement)
	{
		BOOL bListStyle = pGalleryItem->GetSize().cx == 0;

		DrawItemBack(pDC, rcItem, bEnabled, bSelected, bPressed, bChecked,
					 bListStyle ? xtpBarTypePopup : xtpBarTypeNormal);
		COLORREF clrTextColor = GetItemTextColor(bEnabled, bSelected, bListStyle ? FALSE : bPressed,
												 bChecked,
												 bListStyle ? xtpBarTypePopup : xtpBarTypeNormal);

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement),
								(HFONT)m_pPaintManager->GetRegularFont()->GetSafeHandle(),
								clrTextColor);

		CRect rcDest(rcItem);
		rcDest.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));

		XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), rcDest);

		return;
	}

	CString strCaption			 = pGalleryItem->GetCaption();
	CString strDescription		 = pGalleryItem->GetDescription();
	CXTPImageManagerIcon* pImage = pGalleryItem->GetImage();

	CSize szVectorIcon = CSize(pGalleryItem->GetImageWidth(), pGalleryItem->GetImageWidth());

	if (NULL != pImage)
	{
		BOOL bDrawImageAndText = (!strCaption.IsEmpty()
								  && (pGallery->GetStyle() == xtpButtonIconAndCaption
									  || pGallery->GetStyle() == xtpButtonCaptionAndDescription
									  || pGallery->GetStyle() == xtpButtonIconAndCaptionBelow));

		CSize szImage = (pImage->IsVectorIcon()
							 ? rcItem.Size()
							 : CXTPPaintManager::AdjustDpiIconSize(pImage,
																   CSize(pImage->GetWidth(),
																		 pImage->GetHeight()),
																   pGallery, rcItem.Size()));

		if (bDrawImageAndText && pGallery->GetStyle() == xtpButtonIconAndCaptionBelow)
		{
			DrawItemBack(pDC, rcItem, bEnabled, bSelected, bPressed, bChecked, xtpBarTypeNormal);

			CPoint pt((rcItem.left + rcItem.right - szImage.cx) / 2, rcItem.top + XTP_DPI_Y(2));

			pImage->Draw(pDC, pt, bEnabled ? xtpImageNormal : xtpImageDisabled, szImage);

			COLORREF clrTextColor = GetItemTextColor(bEnabled, bSelected, FALSE, FALSE,
													 xtpBarTypePopup);

			pDC->SetTextColor(clrTextColor);

			CRect rcText(rcItem);
			rcText.top += szImage.cy + XTP_DPI_Y(4);

			CXTPFontDC font(pDC, m_pPaintManager->GetRegularFont());
			pDC->DrawText(strCaption, rcText, DT_WORDBREAK | DT_CENTER | DT_NOPREFIX);
		}
		else if (bDrawImageAndText)
		{
			int n2x = XTP_DPI_X(2);
			int n2y = XTP_DPI_Y(2);

			DrawItemBack(pDC, rcItem, bEnabled, bSelected, FALSE, FALSE, xtpBarTypePopup);

			int nImageBoxWidth = max(rcItem.Height() - n2x * 2, szImage.cx);

			if (bChecked)
			{
				CRect rcCheckBox(rcItem.left + n2x, rcItem.top + n2y,
								 rcItem.left + n2x + nImageBoxWidth, rcItem.bottom - n2y);
				DrawItemBack(pDC, rcCheckBox, bEnabled, FALSE, FALSE, TRUE, xtpBarTypePopup);
			}

			CPoint pt(rcItem.left + n2x + (nImageBoxWidth - szImage.cx) / 2,
					  (rcItem.top + rcItem.bottom - szImage.cy) / 2);

			pImage->Draw(pDC, pt, bEnabled ? xtpImageNormal : xtpImageDisabled, szImage);

			COLORREF clrTextColor = GetItemTextColor(bEnabled, bSelected, FALSE, FALSE,
													 xtpBarTypePopup);

			pDC->SetTextColor(clrTextColor);

			CRect rcText(rcItem);
			rcText.left += n2x + nImageBoxWidth + szImage.cx / 3;

			if (pGallery->GetStyle() == xtpButtonCaptionAndDescription)
			{
				CXTPFontDC font(pDC, m_pPaintManager->GetRegularBoldFont());
				pDC->DrawText(strCaption, rcText,
							  DT_END_ELLIPSIS | DT_SINGLELINE | DT_TOP | DT_NOPREFIX);

				rcText.top += pDC->GetTextExtent(_T("XXX"), 3).cy;
				font.SetFont(m_pPaintManager->GetRegularFont());
				pDC->DrawText(strDescription, rcText, DT_WORDBREAK | DT_TOP | DT_NOPREFIX);
			}
			else
			{
				CXTPFontDC font(pDC, m_pPaintManager->GetRegularFont());
				pDC->DrawText(strCaption, rcText,
							  DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			}
		}
		else
		{
			DrawItemBack(pDC, rcItem, bEnabled, bSelected, bPressed, bChecked, xtpBarTypeNormal);

			CPoint pt((rcItem.left + rcItem.right - szImage.cx) / 2,
					  (rcItem.top + rcItem.bottom - szImage.cy) / 2);

			pImage->Draw(pDC, pt, bEnabled ? xtpImageNormal : xtpImageDisabled, szImage);
		}
	}
	else
	{
		DrawItemBack(pDC, rcItem, bEnabled, bSelected, FALSE, bChecked, xtpBarTypePopup);
		COLORREF clrTextColor = GetItemTextColor(bEnabled, bSelected, FALSE, bChecked,
												 xtpBarTypePopup);

		pDC->SetTextColor(clrTextColor);
		CXTPFontDC font(pDC, m_pPaintManager->GetRegularFont());

		CRect rcText(rcItem);
		rcText.left += XTP_DPI_X(3);

		pDC->DrawText(strCaption, rcText,
					  DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}
}
