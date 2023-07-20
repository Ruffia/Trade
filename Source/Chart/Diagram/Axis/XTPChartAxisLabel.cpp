// XTPChartAxisLabel.cpp
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
#include "Chart/Utils/XTPChartNumberFormat.h"

#include "Chart/Diagram/Axis/XTPChartAxisLabel.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPChartAxisLabel, CXTPChartTextElement)

CXTPChartAxisLabel::CXTPChartAxisLabel(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

	m_clrTextColor = CXTPChartColor::Empty;
	m_bVisible	 = TRUE;

	m_bAntialiasing = FALSE;

	m_pFont = CXTPChartFont::GetTahoma8();

	m_nAngle = 0;

	m_pFormat = new CXTPChartNumberFormat(this);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisLabel::~CXTPChartAxisLabel()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pFormat);
}

void CXTPChartAxisLabel::SetFont(CXTPChartFont* pFont)
{
	SAFE_RELEASE(m_pFont);
	m_pFont = pFont;
	OnChartChanged();
}

void CXTPChartAxisLabel::SetTextColor(const CXTPChartColor& clrTextColor)
{
	m_clrTextColor = clrTextColor;
	OnChartChanged();
}

CXTPChartColor CXTPChartAxisLabel::GetTextColor() const
{
	return m_clrTextColor;
}

CXTPChartColor CXTPChartAxisLabel::GetActualTextColor() const
{
	if (!m_clrTextColor.IsEmpty())
		return m_clrTextColor;

	return GetAxis()->GetAppearance()->AxisLabelTextColor;
}

void CXTPChartAxisLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("Antialiasing"), m_bAntialiasing, FALSE);
	PX_Int(pPX, _T("Angle"), m_nAngle, 0);
	PX_Color(pPX, _T("TextColor"), m_clrTextColor);
	PX_Font(pPX, _T("Font"), m_pFont);

	CXTPPropExchangeSection pxFormat(pPX->GetSection(_T("Format")));
	m_pFormat->DoPropExchange(&pxFormat);
}

void CXTPChartAxisLabel::SetFormat(LPCTSTR lpszFormat)
{
	m_pFormat->SetPattern(lpszFormat);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisLabel, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisLabel, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisLabel, "Angle", 2, GetAngle, SetAngle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisLabel, "Antialiasing", 3, GetAntialiasing, SetAntialiasing,
						VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisLabel, "Font", DISPID_FONT, OleGetFont, SetNotSupported,
						VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxisLabel, "TextColor", 4, OleGetTextColor, OleSetTextColor,
						VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisLabel, "Format", 5, OleGetFormat, SetNotSupported, VT_DISPATCH)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisLabel, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisLabel, XTPDIID_ChartAxisLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisLabel, XTPDIID_ChartAxisLabel)

LPDISPATCH CXTPChartAxisLabel::OleGetFont()
{
	return XTPGetDispatch(m_pFont);
}

OLE_COLOR CXTPChartAxisLabel::OleGetTextColor()
{
	return m_clrTextColor.ToOleColor();
}

void CXTPChartAxisLabel::OleSetTextColor(OLE_COLOR clr)
{
	SetTextColor(CXTPChartColor::FromOleColor(clr));
}

LPDISPATCH CXTPChartAxisLabel::OleGetFormat()
{
	return XTPGetDispatch(m_pFormat);
}

#endif
