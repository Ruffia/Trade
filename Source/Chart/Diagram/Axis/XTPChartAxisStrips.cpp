// XTPChartAxisStrips.cpp
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"

#include "Chart/Diagram/Axis/XTPChartAxisStrips.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CXTPChartAxisStrip, CXTPChartElement)

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisStrip

CXTPChartAxisStrip::CXTPChartAxisStrip()
{
	m_bLegendVisible = TRUE;
	m_bVisible		 = TRUE;

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillSolid);

	m_dAxisMinValue = 0;
	m_dAxisMaxValue = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisStrip::~CXTPChartAxisStrip()
{
	SAFE_RELEASE(m_pFillStyle);
}

void CXTPChartAxisStrip::SetName(const CXTPChartString& lpszTitle)
{
	m_strName = lpszTitle;
	OnChartChanged();
}

void CXTPChartAxisStrip::SetColor(const CXTPChartColor& clrColor)
{
	m_clrColor = clrColor;
	OnChartChanged();
}

void CXTPChartAxisStrip::SetColor2(const CXTPChartColor& clrColor)
{
	m_clrColor2 = clrColor;
	OnChartChanged();
}

CXTPChartString CXTPChartAxisStrip::GetLegendName() const
{
	if (!m_strLegendName.IsEmpty())
		return m_strLegendName;

	return m_strName;
}

CXTPChartColor CXTPChartAxisStrip::GetActualColor() const
{
	if (!m_clrColor.IsEmpty())
		return m_clrColor;

	return GetAppearance()->GetDiagram2DAppearance()->GetAxisAppearance()->StripColor;
}

CXTPChartColor CXTPChartAxisStrip::GetActualColor2() const
{
	if (!m_clrColor2.IsEmpty())
		return m_clrColor2;

	return GetAppearance()->GetDiagram2DAppearance()->GetAxisAppearance()->StripColor2;
}

CXTPChartDeviceCommand* CXTPChartAxisStrip::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC,
																	  CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);

	rcBounds.DeflateRect(1, 1);

	return m_pFillStyle->CreateDeviceCommand(rcBounds, GetActualColor(), GetActualColor2());
}

void CXTPChartAxisStrip::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("LegendVisible"), m_bLegendVisible, TRUE);

	PX_String(pPX, _T("Name"), m_strName, _T(""));
	PX_String(pPX, _T("LegendName"), m_strLegendName, _T(""));

	PX_String(pPX, _T("AxisMinValue"), m_strAxisMinValue, _T(""));
	PX_String(pPX, _T("AxisMaxValue"), m_strAxisMaxValue, _T(""));

	PX_Double(pPX, _T("AxisMinValueInternal"), m_dAxisMinValue, 0);
	PX_Double(pPX, _T("AxisMaxValueInternal"), m_dAxisMaxValue, 0);

	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Color(pPX, _T("Color2"), m_clrColor2);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisStrips

CXTPChartAxisStrips::CXTPChartAxisStrips(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisStrips::~CXTPChartAxisStrips()
{
}

CXTPChartAxisStrip* CXTPChartAxisStrips::Add(CXTPChartAxisStrip* pStrip)
{
	InsertAt(GetCount(), pStrip);

	return pStrip;
}

void CXTPChartAxisStrips::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Strip")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			CXTPChartAxisStrip* pElement = new CXTPChartAxisStrip();
			Add(pElement);

			pElement->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Strip")));
		POSITION pos = pEnumerator->GetPosition(XTPToDWORDChecked(m_arrElements.GetSize()));

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisStrip, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "Name", DISPID_VALUE, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "AxisMinValue", 1, OleGetAxisMinValue,
						OleSetAxisMinValue, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "AxisMaxValue", 2, OleGetAxisMaxValue,
						OleSetAxisMaxValue, VT_VARIANT)

	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "LegendVisible", 3, OleGetLegendVisible,
						OleSetLegendVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "Visible", 4, OleGetVisible, OleSetVisible, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "FillStyle", 6, OleGetFillStyle, SetNotSupported,
						VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "Color", 7, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisStrip, "Color2", 8, OleGetColor2, OleSetColor2, VT_COLOR)

	DISP_PROPERTY_ID(CXTPChartAxisStrip, "LegendText", 10, m_strLegendName, VT_BSTR)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisStrip, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisStrip, XTPDIID_ChartAxisStrip, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisStrip, XTPDIID_ChartAxisStrip)

BOOL CXTPChartAxisStrip::OleGetLegendVisible()
{
	return m_bLegendVisible;
}

void CXTPChartAxisStrip::OleSetLegendVisible(BOOL bValue)
{
	SetLegendVisible(bValue);
}

BOOL CXTPChartAxisStrip::OleGetVisible()
{
	return m_bVisible;
}

void CXTPChartAxisStrip::OleSetVisible(BOOL bValue)
{
	SetVisible(bValue);
}

LPDISPATCH CXTPChartAxisStrip::OleGetFillStyle()
{
	return XTPGetDispatch(m_pFillStyle);
}

BSTR CXTPChartAxisStrip::OleGetText()
{
	return m_strName.AllocSysString();
}

void CXTPChartAxisStrip::OleSetText(LPCTSTR lpszText)
{
	SetName(lpszText);
}

VARIANT CXTPChartAxisStrip::OleGetAxisMinValue()
{
	VARIANT vt;
	if (!m_strAxisMinValue.IsEmpty())
	{
		vt.vt	  = VT_BSTR;
		vt.bstrVal = m_strAxisMinValue.AllocSysString();
	}
	else
	{
		vt.vt	 = VT_R8;
		vt.dblVal = m_dAxisMinValue;
	}
	return vt;
}

void CXTPChartAxisStrip::OleSetAxisMinValue(const VARIANT& var)
{
	if (IsStringVariant(&var))
		SetAxisMinValue(VariantToString(&var));
	else
		SetAxisMinValue(VariantToDoubleEx(&var));
}

VARIANT CXTPChartAxisStrip::OleGetAxisMaxValue()
{
	VARIANT vt;
	if (!m_strAxisMaxValue.IsEmpty())
	{
		vt.vt	  = VT_BSTR;
		vt.bstrVal = m_strAxisMaxValue.AllocSysString();
	}
	else
	{
		vt.vt	 = VT_R8;
		vt.dblVal = m_dAxisMaxValue;
	}
	return vt;
}

void CXTPChartAxisStrip::OleSetAxisMaxValue(const VARIANT& var)
{
	if (IsStringVariant(&var))
		SetAxisMaxValue(VariantToString(&var));
	else
		SetAxisMaxValue(VariantToDoubleEx(&var));
}

OLE_COLOR CXTPChartAxisStrip::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

OLE_COLOR CXTPChartAxisStrip::OleGetColor2()
{
	return m_clrColor2.ToOleColor();
}

void CXTPChartAxisStrip::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

void CXTPChartAxisStrip::OleSetColor2(OLE_COLOR clr)
{
	SetColor2(CXTPChartColor::FromOleColor(clr));
}

//////////////////////////////////////////////////////////////////////////
//

BEGIN_DISPATCH_MAP(CXTPChartAxisStrips, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartAxisStrips, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartAxisStrips, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPChartAxisStrips, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPChartAxisStrips, "Add", 2, OleAdd, VT_DISPATCH, VTS_WBSTR)
	DISP_FUNCTION_ID(CXTPChartAxisStrips, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisStrips, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisStrips, XTPDIID_ChartAxisStrips, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisStrips, XTPDIID_ChartAxisStrips)

long CXTPChartAxisStrips::OleGetItemCount()
{
	return (long)GetCount();
}

#	pragma warning(push)
#	pragma warning(disable : 4702)

LPDISPATCH CXTPChartAxisStrips::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartAxisStrip* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#	pragma warning(pop)

LPDISPATCH CXTPChartAxisStrips::OleAdd(LPCOLESTR lpszStrip)
{
	CXTPChartAxisStrip* pStrip = Add(new CXTPChartAxisStrip());

	pStrip->SetName(lpszStrip);

	return XTPGetDispatch(pStrip);
}

IMPLEMENT_ENUM_VARIANT(CXTPChartAxisStrips)

#endif
