// XTPChartTextDeviceCommand.cpp
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
#include "Chart/Drawing/XTPChartTextDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartTextDeviceCommand

CXTPChartTextDeviceCommand::CXTPChartTextDeviceCommand(const CXTPChartString& strText,
													   CXTPChartFont* pFont,
													   const CXTPChartColor& color)
{
	m_strText = strText;
	m_pFont   = pFont;
	m_color   = color;
}

void CXTPChartTextDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ExecuteInternal(pDC);
}

void CXTPChartTextDeviceCommand::ExecuteInternal(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedTextDeviceCommand

CXTPChartBoundedTextDeviceCommand::CXTPChartBoundedTextDeviceCommand(
	const CXTPChartString& strText, CXTPChartFont* pFont, const CXTPChartColor& color,
	const CXTPChartRectF& rectangle)
	: CXTPChartTextDeviceCommand(strText, pFont, color)
{
	m_rect = rectangle;
}

CXTPChartElement* CXTPChartBoundedTextDeviceCommand::HitTest(CPoint point,
															 CXTPChartElement* pParent) const
{
	if (m_rect.Contains((float)point.x, (float)point.y))
		return pParent;

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTextAntialiasingDeviceCommand

CXTPChartTextAntialiasingDeviceCommand::CXTPChartTextAntialiasingDeviceCommand(
	BOOL bAntialias /* = TRUE */)
{
	m_bAntiAlias			= bAntialias;
	m_nOldTextRenderingHint = 0;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartMarkupElementDeviceCommand

CXTPChartMarkupElementDeviceCommand::CXTPChartMarkupElementDeviceCommand(
	CXTPMarkupUIElement* pMarkupUIElement, CXTPChartFont* pFont, const CXTPChartColor& color,
	const CXTPChartRectF& rectangle)
{
	m_pMarkupUIElement = pMarkupUIElement;

	m_pMarkupUIElement->AddRef();

	m_pFont = pFont;
	m_color = color;
	m_rect  = rectangle;
}

CXTPChartMarkupElementDeviceCommand::~CXTPChartMarkupElementDeviceCommand()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

CXTPChartElement* CXTPChartMarkupElementDeviceCommand::HitTest(CPoint point,
															   CXTPChartElement* pParent) const
{
	if (m_rect.Contains((float)point.x, (float)point.y))
		return pParent;

	return NULL;
}
