// XTPChartPalette.cpp
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
#include "Chart/XTPChartElement.h"
#include "Chart/Appearance/XTPChartPalette.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPChartPalette::CXTPChartPalette(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartPalette::~CXTPChartPalette()
{
}

CXTPChartColor CXTPChartPalette::GetColor(int Index) const
{
	if (m_arrEntries.GetSize() == 0)
		return CXTPChartColor(CXTPChartColor::White);

	return m_arrEntries[Index % m_arrEntries.GetSize()].Color;
}

CXTPChartColor CXTPChartPalette::GetColor2(int Index) const
{
	if (m_arrEntries.GetSize() == 0)
		return CXTPChartColor(CXTPChartColor::Black);

	return m_arrEntries[Index % m_arrEntries.GetSize()].Color2;
}

void CXTPChartPalette::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxItems(pPX->GetSection(_T("Items")));

	if (pPX->IsLoading())
	{
		m_arrEntries.RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pxItems->GetEnumerator(_T("Item")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			CXTPChartPaletteEntry entry;
			PX_Color(&pxItem, _T("Color"), entry.Color);

			CXTPChartColor clr2;
			PX_Color(&pxItem, _T("Color2"), entry.Color2);

			m_arrEntries.Add(entry);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pxItems->GetEnumerator(_T("Item")));
		POSITION pos = pEnumerator->GetPosition(XTPToDWORDChecked(m_arrEntries.GetSize()));

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPChartPaletteEntry& entry = m_arrEntries[i];

			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			PX_Color(&pxItem, _T("Color"), entry.Color);

			PX_Color(&pxItem, _T("Color2"), entry.Color2);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPalette, CXTPChartElement)
	DISP_PROPERTY_PARAM_ID(CXTPChartPalette, "Color", 1, OleGetColor, OleSetColor, VT_COLOR, VTS_I4)
	DISP_PROPERTY_PARAM_ID(CXTPChartPalette, "Color2", 2, OleGetColor2, OleSetColor2, VT_COLOR,
						   VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPChartPalette, "Count", 3, OleGetCount, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPChartPalette, "DeleteAll", 4, OleDeleteAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartPalette, "AddEntry", 5, OleAddEntry, VT_EMPTY, VTS_COLOR VTS_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartPalette, CXTPChartElement)
	INTERFACE_PART(CXTPChartPalette, XTPDIID_ChartPalette, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPalette, XTPDIID_ChartPalette)

OLE_COLOR CXTPChartPalette::OleGetColor(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	return m_arrEntries[nIndex].Color.ToOleColor();
}

OLE_COLOR CXTPChartPalette::OleGetColor2(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	return m_arrEntries[nIndex].Color2.ToOleColor();
}

void CXTPChartPalette::OleSetColor(int nIndex, OLE_COLOR clr)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	m_arrEntries[nIndex].Color = CXTPChartColor::FromOleColor(clr);
}

void CXTPChartPalette::OleSetColor2(int nIndex, OLE_COLOR clr)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	m_arrEntries[nIndex].Color2 = CXTPChartColor::FromOleColor(clr);
}

void CXTPChartPalette::OleDeleteAll()
{
	m_arrEntries.RemoveAll();
}

int CXTPChartPalette::OleGetCount()
{
	return m_arrEntries.GetSize();
}

void CXTPChartPalette::OleAddEntry(OLE_COLOR clr, OLE_COLOR clr2)
{
	CXTPChartPaletteEntry entry;
	entry.Color  = CXTPChartColor::FromOleColor(clr);
	entry.Color2 = CXTPChartColor::FromOleColor(clr2);

	m_arrEntries.Add(entry);
}

#endif
