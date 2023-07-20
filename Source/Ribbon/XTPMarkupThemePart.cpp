// XTPMarkupThemePart.cpp
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
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"

#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonMarkupTheme.h"

#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupString.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupFrameworkElement.h"
#include "Markup/Controls/XTPMarkupTextBlock.h"
#include "Markup/XTPMarkupDrawingContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPRibbonMarkupThemePartGroup::SetCaption(LPCTSTR pszCaption)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pTextProperty,
							   CXTPMarkupString::CreateValue(pszCaption));
	}
}

void CXTPRibbonMarkupThemePartGroup::SetTextColor(COLORREF clrText)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pForegroundProperty,
							   new CXTPMarkupSolidColorBrush(clrText));
	}
}

void CXTPRibbonMarkupThemePartGroup::SetFontSize(int nFontSize)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pFontSizeProperty,
							   new CXTPMarkupInt(nFontSize));
	}
}

void CXTPRibbonMarkupThemePartGroup::SetFontFamily(LPCTSTR pszFontFamily)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pFontFamilyProperty,
							   CXTPMarkupString::CreateValue(pszFontFamily));
	}
}

/////////////////////////////////////////////////////////////////////////////

void CXTPRibbonMarkupThemePartSystemMenu::SetCaption(LPCTSTR pszCaption)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"Caption"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pTextProperty,
							   CXTPMarkupString::CreateValue(pszCaption));
	}
}

void CXTPRibbonMarkupThemePartSystemMenu::SetFontSize(int nFontSize)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"Caption"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pFontSizeProperty,
							   new CXTPMarkupInt(nFontSize));
	}
}

void CXTPRibbonMarkupThemePartSystemMenu::SetFontFamily(LPCTSTR pszFontFamily)
{
	ASSERT(NULL != m_pMarkupUIElement);
	CXTPMarkupTextBlock* pCaptionText =
		MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"Caption"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pFontFamilyProperty,
							   CXTPMarkupString::CreateValue(pszFontFamily));
	}
}
