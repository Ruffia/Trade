// XTPChartGdiTextDeviceCommand.cpp
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceManager.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/XTPTypeId.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"
#include "Chart/Drawing/XTPChartTextDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiTextDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiBoundedTextDeviceCommand

CXTPChartGdiBoundedTextDeviceCommand::CXTPChartGdiBoundedTextDeviceCommand(
	const CXTPChartString& strText, CXTPChartFont* pFont, const CXTPChartColor& color,
	const CXTPChartRectF& rectangle)
	: CXTPChartBoundedTextDeviceCommand(strText, pFont, color, rectangle)
{
}

void CXTPChartGdiBoundedTextDeviceCommand::ExecuteInternal(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GpFont* pGpFont = NULL;
	if (m_pFont)
	{
		GdipCreateFontFromLogfont(pDC->GetHDC(), &m_pFont->m_lf, &pGpFont);
	}
	else
	{
		LOGFONT lf  = { 0 };
		lf.lfHeight = -12;
		lf.lfWeight = FW_NORMAL;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));

		GdipCreateFontFromLogfont(pDC->GetHDC(), &lf, &pGpFont);
	}

	GpStringFormat* pGpStringFormat = NULL;
	GdipCreateStringFormat(0, 0, &pGpStringFormat);

	GdipSetStringFormatLineAlign(pGpStringFormat, StringAlignmentCenter);
	GdipSetStringFormatAlign(pGpStringFormat, StringAlignmentCenter);

	GdipDrawString(pGdiDC->GetGraphics(), XTP_CT2CW(m_strText), -1, pGpFont, (RectF*)&m_rect,
				   pGpStringFormat, pGpBrush);

	GdipDeleteFont(pGpFont);
	GdipDeleteBrush(pGpBrush);
	GdipDeleteStringFormat(pGpStringFormat);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiTextAntialiasingDeviceCommand

CXTPChartGdiTextAntialiasingDeviceCommand::CXTPChartGdiTextAntialiasingDeviceCommand(
	BOOL bAntialias /* = TRUE */)
	: CXTPChartTextAntialiasingDeviceCommand(bAntialias)
{
}

void CXTPChartGdiTextAntialiasingDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipGetTextRenderingHint(pGdiDC->GetGraphics(), (TextRenderingHint*)&m_nOldTextRenderingHint);
	GdipSetTextRenderingHint(pGdiDC->GetGraphics(), m_bAntiAlias ? TextRenderingHintAntiAlias
																 : TextRenderingHintSystemDefault);
}

void CXTPChartGdiTextAntialiasingDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSetTextRenderingHint(pGdiDC->GetGraphics(), (TextRenderingHint)m_nOldTextRenderingHint);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiMarkupElementDeviceCommand

CXTPChartGdiMarkupElementDeviceCommand::CXTPChartGdiMarkupElementDeviceCommand(
	CXTPMarkupUIElement* pMarkupUIElement, CXTPChartFont* pFont, const CXTPChartColor& color,
	const CXTPChartRectF& rectangle)
	: CXTPChartMarkupElementDeviceCommand(pMarkupUIElement, pFont, color, rectangle)
{
}

void CXTPChartGdiMarkupElementDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	if (!m_pMarkupUIElement)
		return;

	CRect rc((int)floor(m_rect.GetLeft()), (int)floor(m_rect.GetTop()),
			 (int)ceil(m_rect.GetRight()), (int)ceil(m_rect.GetBottom()));

	m_pMarkupUIElement->GetMarkupContext()->SetDefaultFont(&m_pFont->m_lf);
	m_pMarkupUIElement->GetMarkupContext()->SetDefaultTextColor(
		RGB(m_color.GetR(), m_color.GetG(), m_color.GetB()));

	CXTPMarkupDrawingContext dc(m_pMarkupUIElement->GetMarkupContext(), pGdiDC->GetGraphics());
	m_pMarkupUIElement->Arrange(rc);
	m_pMarkupUIElement->Render(&dc);
}

CXTPChartElement* CXTPChartGdiMarkupElementDeviceCommand::HitTest(CPoint point,
																  CXTPChartElement* pParent) const
{
	if (m_rect.Contains((float)point.x, (float)point.y))
		return pParent;

	return NULL;
}
