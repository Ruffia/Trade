// XTPChartGdiDeviceContext.cpp
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

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPIntel80Helpers.h"
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
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4510)
#pragma warning(disable : 4610)

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiDeviceContext

IMPLEMENT_DYNAMIC(CXTPChartGdiDeviceContext, CXTPChartDeviceContext);

void CXTPChartGdiDeviceContext::Register(BOOL bReserved)
{
	UNREFERENCED_PARAMETER(bReserved);
}

CXTPChartGdiDeviceContext::CXTPChartGdiDeviceContext(CXTPChartContainer* pContainer, HDC hDC,
													 CXTPChartDeviceContext* p3dDC /*= NULL*/)
	: CXTPChartDeviceContext(pContainer, hDC, FALSE,
							 XTPChartObjectFactory()->Get2dDrawingObjectFactory())
	, m_p3dDC(p3dDC)
{
	GdipCreateFromHDC(m_hDC, &m_pGpGraphics);

	// Adjust world scalling to output DPI.
	GdipSetPageUnit(m_pGpGraphics, UnitDocument);
	REAL dpix;
	REAL dpiy;
	GdipGetDpiX(m_pGpGraphics, &dpix);
	GdipGetDpiY(m_pGpGraphics, &dpiy);
	GdipScaleWorldTransform(m_pGpGraphics, 300.f / dpix, 300.f / dpiy, MatrixOrderAppend);

	// Additional device configuration.
	GdipSetSmoothingMode(m_pGpGraphics, SmoothingModeHighSpeed);
}

CXTPChartGdiDeviceContext::CXTPChartGdiDeviceContext(CXTPChartContainer* pContainer,
													 Graphics* pGraphics, HDC hDC)
	: CXTPChartDeviceContext(pContainer, hDC, FALSE,
							 XTPChartObjectFactory()->Get2dDrawingObjectFactory())
{
	_XTP_USES_PROTECTED_ACCESS_ARGS(CXTPChartGdiDeviceContext, Graphics, (HDC hDC), (hDC),
									GpGraphics, nativeGraphics);

	m_pGpGraphics = _XTP_PROTECTED_ACCESS(Graphics, pGraphics, nativeGraphics);

	GdipSetPageUnit(m_pGpGraphics, UnitPixel);
	GdipSetSmoothingMode(m_pGpGraphics, SmoothingModeHighSpeed);
}

CXTPChartGdiDeviceContext::~CXTPChartGdiDeviceContext()
{
	if (m_pGpGraphics)
	{
		GdipDeleteGraphics(m_pGpGraphics);
	}
}

void CXTPChartGdiDeviceContext::Execute(CXTPChartDeviceCommand* pCommand)
{
	ASSERT_VALID(pCommand);

	if (!pCommand->Is3D())
	{
		CXTPChartDeviceContext::Execute(pCommand);
	}
	else
	{
		if (NULL != m_p3dDC)
		{
			m_p3dDC->Execute(pCommand);
		}
	}
}

CXTPChartSizeF CXTPChartGdiDeviceContext::MeasureString(const CXTPChartString* pText,
														CXTPChartFont* pFont)
{
	GpFont* pGpFont = NULL;
	GdipCreateFontFromLogfont(m_hDC, &pFont->m_lf, &pGpFont);

	RectF boundingBox;
	RectF layoutRect(0, 0, 0.0f, 0.0f);

	GdipMeasureString(m_pGpGraphics, XTP_CT2CW(*pText), -1, pGpFont, &layoutRect, 0, &boundingBox,
					  0, 0);

	CXTPChartSizeF sz;
	boundingBox.GetSize((SizeF*)&sz);

	GdipDeleteFont(pGpFont);

	return sz;
}

CXTPChartSizeF CXTPChartGdiDeviceContext::MeasureMarkupElement(
	CXTPMarkupUIElement* pMarkupUIElement, CXTPChartFont* pFont)
{
	ASSERT_VALID(pFont);

	CXTPChartSizeF size(0, 0);
	if (NULL != pMarkupUIElement)
	{
		pMarkupUIElement->GetMarkupContext()->SetDefaultFont(&pFont->m_lf);

		CXTPMarkupDrawingContext dc(pMarkupUIElement->GetMarkupContext(), GetGraphics());
		pMarkupUIElement->Measure(&dc, CSize(INT_MAX, INT_MAX));

		CSize sz	= pMarkupUIElement->GetDesiredSize();
		size.Width  = static_cast<float>(sz.cx);
		size.Height = static_cast<float>(sz.cy);
	}

	return size;
}
