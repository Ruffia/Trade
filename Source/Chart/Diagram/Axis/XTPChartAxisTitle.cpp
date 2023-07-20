// XTPChartAxisTitle.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Diagram/Axis/XTPChartAxisTitle.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPChartAxisTitle, CXTPChartTextElement)

CXTPChartAxisTitle::CXTPChartAxisTitle(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

	m_pFont			= CXTPChartFont::GetTahoma12();
	m_clrTextColor  = CXTPChartColor::Empty;
	m_bAntialiasing = TRUE;
	m_nAlignment	= xtpChartAlignCenter;
	m_bVisible		= FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisTitle::~CXTPChartAxisTitle()
{
	SAFE_RELEASE(m_pFont);
}

void CXTPChartAxisTitle::SetText(LPCTSTR lpszTitle)
{
	m_strText = lpszTitle;
	OnChartChanged();
}

void CXTPChartAxisTitle::SetFont(CXTPChartFont* pFont)
{
	SAFE_RELEASE(m_pFont);
	m_pFont = pFont;
	OnChartChanged();
}

void CXTPChartAxisTitle::SetTextColor(const CXTPChartColor& clrTextColor)
{
	m_clrTextColor = clrTextColor;
	OnChartChanged();
}

void CXTPChartAxisTitle::SetAlignment(XTPChartStringAlignment nAlignment)
{
	m_nAlignment = nAlignment;
	OnChartChanged();
}

void CXTPChartAxisTitle::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}

CXTPChartColor CXTPChartAxisTitle::GetTextColor() const
{
	return m_clrTextColor;
}

CXTPChartColor CXTPChartAxisTitle::GetActualTextColor() const
{
	if (!m_clrTextColor.IsEmpty())
		return m_clrTextColor;

	return GetAxis()->GetAppearance()->AxisTitleTextColor;
}

void CXTPChartAxisTitle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, FALSE);
	PX_String(pPX, _T("Text"), m_strText);
	PX_Color(pPX, _T("TextColor"), m_clrTextColor);
	PX_Font(pPX, _T("Font"), m_pFont);
	PX_Bool(pPX, _T("Antialiasing"), m_bAntialiasing, TRUE);
	PX_Enum(pPX, _T("Alignment"), m_nAlignment, xtpChartAlignCenter);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisTitle, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTitle, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTitle, "Text", DISPID_VALUE, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTitle, "Antialiasing", 2, GetAntialiasing, SetAntialiasing,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTitle, "Alignment", 3, GetAlignment, SetAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTitle, "Font", DISPID_FONT, OleGetFont, SetNotSupported,
						VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisTitle, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisTitle, XTPDIID_ChartAxisTitle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisTitle, XTPDIID_ChartAxisTitle)

BSTR CXTPChartAxisTitle::OleGetText()
{
	return m_strText.AllocSysString();
}

void CXTPChartAxisTitle::OleSetText(LPCTSTR lpszText)
{
	SetText(lpszText);
}

LPDISPATCH CXTPChartAxisTitle::OleGetFont()
{
	return XTPGetDispatch(m_pFont);
}

#endif
