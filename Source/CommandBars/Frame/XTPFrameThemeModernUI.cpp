// XTPFrameThemeModernUI.cpp
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

#include "GraphicLibrary/unzip/unzip.h"
#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResource.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPTheme.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"
#include "Common/Base/Types/XTPSize.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsModernUITheme.h"

#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/XTPRibbonMarkupFrameTheme.h"
#endif
#include "CommandBars/Frame/XTPFrameThemeModernUI.h"

#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupString.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupFrameworkElement.h"
#include "Markup/Controls/XTPMarkupDecorator.h"
#include "Markup/Controls/XTPMarkupBorder.h"
#include "Markup/Controls/XTPMarkupPanel.h"
#include "Markup/Controls/XTPMarkupCanvas.h"
#include "Markup/Shapes/XTPMarkupShape.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------

void CXTPModernUIFrameBorder::SetBorderColor(COLORREF clrBorder, COLORREF clrBackground)
{
	if (NULL != m_pMarkupUIElement)
	{
		CXTPMarkupBorder* pBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder,
													   m_pMarkupUIElement->FindName(L"Border"));
		ASSERT(NULL != pBorder); // Element not found in markup

		if (NULL != pBorder)
		{
			pBorder->SetValue(CXTPMarkupBorder::m_pBorderBrushProperty,
							  new CXTPMarkupSolidColorBrush(clrBorder));
			pBorder->SetValue(CXTPMarkupBorder::m_pBackgroundProperty,
							  new CXTPMarkupSolidColorBrush(clrBackground));
		}
	}
}

//---------------------------------------------------------------------------

void CXTPModernUIFrameCaptionButton::SetColors(CXTPColorSetCaptionButton* pColorSet)
{
	SetBackgroundColor(pColorSet->m_clrBackground);
	SetOutlineColor(pColorSet->m_clrOutline);
	SetStrokeColor(pColorSet->m_clrFill);
	SetFillColor(pColorSet->m_clrFill);
}

void CXTPModernUIFrameCaptionButton::SetBackgroundColor(COLORREF clrBackground)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupBorder* pBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder,
												   m_pMarkupUIElement->FindName(L"Border"));

	// Element not found in markup
	ASSERT(pBorder != NULL);
	if (pBorder == NULL)
		return;

	if (clrBackground != COLORREF_NULL)
	{
		pBorder->SetValue(CXTPMarkupBorder::m_pBackgroundProperty,
						  new CXTPMarkupSolidColorBrush(clrBackground));
	}
	else
	{
		pBorder->SetValue(CXTPMarkupBorder::m_pBackgroundProperty, NULL);
	}
}

void CXTPModernUIFrameCaptionButton::SetOutlineColor(COLORREF clrOutline)
{
	ASSERT(m_pMarkupUIElement != NULL);
	CXTPMarkupCanvas* pCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas,
												   m_pMarkupUIElement->FindName(L"Canvas"));
	ASSERT(NULL != pCanvas); // Element not found in markup

	if (NULL != pCanvas)
	{
		for (int nChild = 0; nChild < pCanvas->GetLogicalChildrenCount(); nChild++)
		{
			CXTPMarkupShape* pShape = MARKUP_DYNAMICCAST(CXTPMarkupShape,
														 pCanvas->GetLogicalChild(nChild));

			if (NULL != pShape)
			{
				CXTPMarkupString* pName = pShape->GetName();

				if ((NULL != pName) && (0 == _wcsicmp(*pName, L"Outline")))
				{
					pShape->SetValue(CXTPMarkupShape::m_pStrokeProperty,
									 new CXTPMarkupSolidColorBrush(clrOutline));
				}
			}
		}
	}
}

void CXTPModernUIFrameCaptionButton::SetStrokeColor(COLORREF clrStroke)
{
	ASSERT(m_pMarkupUIElement != NULL);
	CXTPMarkupCanvas* pCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas,
												   m_pMarkupUIElement->FindName(L"Canvas"));
	ASSERT(NULL != pCanvas); // Element not found in markup

	if (NULL != pCanvas)
	{
		for (int nChild = 0; nChild < pCanvas->GetLogicalChildrenCount(); nChild++)
		{
			CXTPMarkupShape* pShape = MARKUP_DYNAMICCAST(CXTPMarkupShape,
														 pCanvas->GetLogicalChild(nChild));

			if (NULL != pShape)
			{
				CXTPMarkupString* pName = pShape->GetName();

				if ((NULL != pName) && (0 == _wcsicmp(*pName, L"Stroke")))
				{
					pShape->SetValue(CXTPMarkupShape::m_pStrokeProperty,
									 new CXTPMarkupSolidColorBrush(clrStroke));
				}
			}
		}
	}
}

void CXTPModernUIFrameCaptionButton::SetFillColor(COLORREF clrFill)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupCanvas* pCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas,
												   m_pMarkupUIElement->FindName(L"Canvas"));
	ASSERT(NULL != pCanvas); // Element not found in markup

	if (NULL != pCanvas)
	{
		for (int nChild = 0; nChild < pCanvas->GetLogicalChildrenCount(); nChild++)
		{
			CXTPMarkupShape* pShape = MARKUP_DYNAMICCAST(CXTPMarkupShape,
														 pCanvas->GetLogicalChild(nChild));

			if (NULL != pShape)
			{
				CXTPMarkupString* pName = pShape->GetName();

				if ((NULL != pName) && (0 == _wcsicmp(*pName, L"Fill")))
				{
					pShape->SetValue(CXTPMarkupShape::m_pStrokeProperty,
									 new CXTPMarkupSolidColorBrush(clrFill));
					pShape->SetValue(CXTPMarkupShape::m_pFillProperty,
									 new CXTPMarkupSolidColorBrush(clrFill));
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CXTPFrameThemeModernUI, CXTPRibbonMarkupFrameTheme);

CXTPFrameThemeModernUI::CXTPFrameThemeModernUI(CXTPPaintManager* pPaintManager)
	: CXTPRibbonMarkupFrameTheme(pPaintManager)
{
	// Frame border
	m_pBorderLeft   = new CXTPModernUIFrameBorder();
	m_pBorderTop	= new CXTPModernUIFrameBorder();
	m_pBorderRight  = new CXTPModernUIFrameBorder();
	m_pBorderBottom = new CXTPModernUIFrameBorder();

	// Frame caption buttons
	m_pButtonMinimize = new CXTPModernUIFrameCaptionButton();
	m_pButtonMaximize = new CXTPModernUIFrameCaptionButton();
	m_pButtonRestore  = new CXTPModernUIFrameCaptionButton();
	m_pButtonClose	= new CXTPModernUIFrameCaptionButton();
}

CXTPFrameThemeModernUI::~CXTPFrameThemeModernUI()
{
	// Frame border
	SAFE_DELETE(m_pBorderLeft);
	SAFE_DELETE(m_pBorderTop);
	SAFE_DELETE(m_pBorderRight);
	SAFE_DELETE(m_pBorderBottom);

	// Frame caption buttons
	SAFE_DELETE(m_pButtonMinimize);
	SAFE_DELETE(m_pButtonMaximize);
	SAFE_DELETE(m_pButtonRestore);
	SAFE_DELETE(m_pButtonClose);
}

void CXTPFrameThemeModernUI::LoadParts(HZIP hZip, CXTPMarkupContext* pMarkupContext)
{
	if (m_bMarkupPartsLoaded)
		return;

	m_bMarkupPartsLoaded = TRUE;

	// Frame border
	BOOL bMarkupLoaded = m_pBorderLeft->LoadPart(hZip, _T("Frame/BorderLeft.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderTop->LoadPart(hZip, _T("Frame/BorderTop.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderRight->LoadPart(hZip, _T("Frame/BorderRight.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pBorderBottom->LoadPart(hZip, _T("Frame/BorderBottom.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	// Frame caption buttons
	bMarkupLoaded = m_pButtonMinimize->LoadPart(hZip, _T("Frame/ButtonMinimize.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonMaximize->LoadPart(hZip, _T("Frame/ButtonMaximize.xaml"),
												pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonRestore->LoadPart(hZip, _T("Frame/ButtonRestore.xaml"),
											   pMarkupContext);
	ASSERT(bMarkupLoaded);

	bMarkupLoaded = m_pButtonClose->LoadPart(hZip, _T("Frame/ButtonClose.xaml"), pMarkupContext);
	ASSERT(bMarkupLoaded);

	UNREFERENCED_PARAMETER(bMarkupLoaded);
}

void CXTPFrameThemeModernUI::LoadParts(BOOL bReload /*= FALSE*/)
{
	if (bReload)
		m_bMarkupPartsLoaded = FALSE;

	if (m_bMarkupPartsLoaded)
		return;

	HMODULE hModule = XTPResourceImages()->GetHandle();

	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	CXTPResource resource;
	resource.LoadResource(hModule, GetStyleResourceName(), _T("STYLE"));

	if (NULL != resource.m_hResource)
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		LoadParts(hZip, ((CXTPCommandBarsModernUITheme*)m_pPaintManager)->GetMarkupContext());
		CloseZip(hZip);
	}
}

HRGN CXTPFrameThemeModernUI::CalcFrameRegion(CXTPCommandBarsFrameHook* pFrameHook, CSize sz) const
{
	UNREFERENCED_PARAMETER(pFrameHook);

	HRGN hRgn = ::CreateRectRgn(0, 0, sz.cx, sz.cy);
	ASSERT(NULL != hRgn); // Region creation failed

	return hRgn;
}

void CXTPFrameThemeModernUI::DrawFrame(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook)
{
	CXTPRibbonMarkupFrameTheme::DrawFrame(pDC, pFrameHook);
}

void CXTPFrameThemeModernUI::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected,
													BOOL bPressed, BOOL bActive)
{
	DrawFrameCaptionButton(pDC, rc, nId, bSelected, bPressed, bActive, NULL);
}

void CXTPFrameThemeModernUI::DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected,
													BOOL bPressed, BOOL bActive,
													CXTPRibbonBar* pRibbonBar)
{
	UNREFERENCED_PARAMETER(pRibbonBar);

	CXTPModernUIFrameCaptionButton* pButton = NULL;

	switch (nId)
	{
		case SC_MINIMIZE: pButton = m_pButtonMinimize; break;
		case SC_MAXIMIZE: pButton = m_pButtonMaximize; break;
		case SC_RESTORE: pButton = m_pButtonRestore; break;
		case SC_CLOSE: pButton = m_pButtonClose; break;
	}

	CXTPColorSetCaptionButton* pColorSet = NULL;

	if (bPressed)
	{
		pColorSet = &m_csFrameCaptionButtonPressed;
	}
	else if (bSelected)
	{
		pColorSet = &m_csFrameCaptionButtonHot;
	}
	else if (bActive)
	{
		pColorSet = &m_csFrameCaptionButtonActive;
	}
	else if (!bActive)
	{
		pColorSet = &m_csFrameCaptionButtonInactive;
	}

	if (pColorSet)
	{
		ASSERT(NULL != pButton);
		pButton->SetColors(pColorSet);
		pButton->RenderMarkup(pDC, rc);
	}
}

CXTPMarkupThemePart* CXTPFrameThemeModernUI::GetFrameBorder(int nId, BOOL bActive)
{
	CXTPModernUIFrameBorder* pBorder = NULL;

	switch (nId)
	{
		case xtpBorderPartTop: pBorder = m_pBorderTop; break;
		case xtpBorderPartLeft:
		case xtpBorderPartLeftCaption: pBorder = m_pBorderLeft; break;
		case xtpBorderPartRight:
		case xtpBorderPartRightCaption: pBorder = m_pBorderRight; break;
		case xtpBorderPartBottom: pBorder = m_pBorderBottom; break;
	}

	if (NULL != pBorder)
	{
		if (bActive)
		{
			pBorder->SetBorderColor(m_clrFrameBorderActive, m_clrFrameCaptionActive);
		}
		else
		{
			pBorder->SetBorderColor(m_clrFrameBorderInactive, m_clrFrameCaptionInactive);
		}
	}

	return pBorder;
}

void CXTPFrameThemeModernUI::DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite,
											 BOOL bActive)
{
	CXTPFramePaintManager::DrawCaptionText(pDC, rcCaptionText, pSite, bActive);
}

void CXTPFrameThemeModernUI::DrawCaptionText(CDC* pDC, CRect rcCaptionText, CWnd* pSite,
											 BOOL bActive, CXTPRibbonBar* pRibbonBar)
{
	CXTPFramePaintManager::DrawCaptionText(pDC, rcCaptionText, pSite, bActive, pRibbonBar);
}

CRect CXTPFrameThemeModernUI::CalcFrameBorder(const CXTPCommandBarsFrameHook* pFrameHook) const
{
	DWORD dwStyle = pFrameHook->GetSiteStyle();

	if ((WS_MAXIMIZE & dwStyle) || pFrameHook->IsDwmEnabled())
	{
		return CXTPFramePaintManager::CalcFrameBorder(pFrameHook);
	}
	else if (0 != (pFrameHook->GetSiteExStyle() & WS_EX_MDICHILD))
	{
		CSize szFrame(::GetSystemMetrics(SM_CXFRAME), ::GetSystemMetrics(SM_CYFRAME));
		return (0 != (pFrameHook->GetSiteStyle() & WS_CAPTION)
					? CRect(szFrame.cx, XTP_DPI_Y(1), szFrame.cx, szFrame.cy)
					: CRect(szFrame.cx, szFrame.cy, szFrame.cx, szFrame.cy));
	}
	else
	{
		return CRect(1, 1, 1, 1);
	}
}

void CXTPFrameThemeModernUI::RefreshMetrics()
{
	CXTPRibbonMarkupFrameTheme::RefreshMetrics();

	LoadParts();

	CXTPResourceImages* pImages = XTPResourceImages();

	m_nFrameCaptionHeight = XTP_DPI_Y(pImages->GetImageInt(_T("Frame"), _T("CaptionHeight")));

	m_clrFrameBorderInactive.SetStandardValue(
		pImages->GetImageColor(_T("Frame.Inactive"), _T("BorderColor")));
	m_clrFrameBorderActive.SetStandardValue(
		pImages->GetImageColor(_T("Frame.Active"), _T("BorderColor")));

	m_clrFrameCaptionTextInactive = pImages->GetImageColor(_T("Frame.Inactive"), _T("FontColor"));
	m_clrFrameCaptionTextActive   = pImages->GetImageColor(_T("Frame.Active"), _T("FontColor"));
	m_clrFrameCaptionTextActiveTitle = m_clrFrameCaptionTextActive;

	m_csFrameCaptionButtonInactive.RefreshMetrics(_T("Frame.CaptionButton.Inactive"));
	m_csFrameCaptionButtonActive.RefreshMetrics(_T("Frame.CaptionButton.Active"));
	m_csFrameCaptionButtonHot.RefreshMetrics(_T("Frame.CaptionButton.Hot"));
	m_csFrameCaptionButtonPressed.RefreshMetrics(_T("Frame.CaptionButton.Pressed"));

	CXTPNonClientMetrics ncm;
	ncm.lfCaptionFont.lfWeight = FW_NORMAL;
	// Read FontFace and FontSize entries under [Frame] from style .ini files
	ncm.lfCaptionFont.lfHeight = -XTP_DPI_Y(pImages->GetImageInt(_T("Frame"), _T("FontSize")));
	CString strFace			   = pImages->GetImageString(_T("Frame"), _T("FontFace"));
	TCSNCCPY_S(ncm.lfCaptionFont.lfFaceName, LF_FACESIZE, strFace, LF_FACESIZE);

	m_fntFrameCaption.SetStandardFont(&ncm.lfCaptionFont);
}
