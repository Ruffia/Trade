// XTPChartLineStyle.cpp
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
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPTypeId.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartLineStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPChartLineStyle::CXTPChartLineStyle(CXTPChartElement* pOwner)
{
	m_pOwner	 = pOwner;
	m_nDashStyle = xtpChartDashStyleSolid;
	m_nThickness = 1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPChartLineStyle::SetDashStyle(XTPChartDashStyle nDashStyle)
{
	m_nDashStyle = nDashStyle;
	if (m_pOwner)
		m_pOwner->OnChartChanged();
}

void CXTPChartLineStyle::SetThickness(int nThickness)
{
	m_nThickness = nThickness;
	if (m_pOwner)
		m_pOwner->OnChartChanged();
}

CXTPChartDeviceCommand* CXTPChartLineStyle::CreateDeviceCommand(const CXTPChartPointF& point1,
																const CXTPChartPointF& point2,
																const CXTPChartColor& color)
{
	if (m_nDashStyle == xtpChartDashStyleEmpty)
		return NULL;

	if (m_nDashStyle == xtpChartDashStyleSolid)
		return CXTPChartDrawingObjectFactory::Create2dSolidLineDeviceCommand(point1, point2, color,
																			 m_nThickness);

	return CXTPChartDrawingObjectFactory::Create2dDashedLineDeviceCommand(point1, point2, color,
																		  m_nThickness,
																		  m_nDashStyle,
																		  m_arrDashArray);
}

CXTPChartDeviceCommand* CXTPChartLineStyle::CreateDeviceCommand(const CXTPChartPoints& points,
																const CXTPChartColor& color)
{
	if (m_nDashStyle == xtpChartDashStyleEmpty)
		return NULL;

	if (m_nDashStyle == xtpChartDashStyleSolid)
		return CXTPChartDrawingObjectFactory::Create2dSolidPolylineDeviceCommand(points, color,
																				 m_nThickness);

	return CXTPChartDrawingObjectFactory::Create2dDashedPolylineDeviceCommand(points, color,
																			  m_nThickness,
																			  m_nDashStyle,
																			  m_arrDashArray);
}

CXTPChartDeviceCommand* CXTPChartLineStyle::CreateSplineDeviceCommand(const CXTPChartPoints& points,
																	  const CXTPChartColor& color)
{
	if (m_nDashStyle == xtpChartDashStyleEmpty)
		return NULL;

	if (m_nDashStyle == xtpChartDashStyleSolid)
		return CXTPChartDrawingObjectFactory::Create2dSolidSplineDeviceCommand(points, color,
																			   m_nThickness);

	return CXTPChartDrawingObjectFactory::Create2dDashedSplineDeviceCommand(points, color,
																			m_nThickness,
																			m_nDashStyle,
																			m_arrDashArray);
}

void CXTPChartLineStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Enum(pPX, _T("DashStyle"), m_nDashStyle, xtpChartDashStyleSolid);

	if (pPX->IsStoring())
	{
		int nCount = (int)m_arrDashArray.GetSize();

		PX_Int(pPX, _T("DashArray"), nCount, 0);

		for (int i = 0; i < nCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Dash%d"), i);

			PX_Float(pPX, strSection, m_arrDashArray[i]);
		}
	}
	else
	{
		int nCount;
		PX_Int(pPX, _T("DashArray"), nCount, 0);

		m_arrDashArray.SetSize(nCount);

		for (int i = 0; i < nCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Dash%d"), i);

			PX_Float(pPX, strSection, m_arrDashArray[i]);
		}
	}
}

void CXTPChartLineStyle::SetDashArray(const CXTPChartLineDashArray& dashArray)
{
	m_arrDashArray.Copy(dashArray);
	m_nDashStyle = xtpChartDashStyleCustom;

	m_pOwner->OnChartChanged();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartLineStyle, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartLineStyle, "Thickness", 1, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartLineStyle, "DashStyle", 2, GetDashStyle, SetDashStyle, VT_I4)
	DISP_FUNCTION_ID(CXTPChartLineStyle, "SetDashArray", 3, OleSetDashArray, VT_EMPTY, VTS_VARIANT)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartLineStyle, CXTPCmdTarget)
	INTERFACE_PART(CXTPChartLineStyle, XTPDIID_ChartLineStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLineStyle, XTPDIID_ChartLineStyle)

void CXTPChartLineStyle::OleSetDashArray(const VARIANT& var)
{
	if ((var.vt & VT_ARRAY) != 0)
	{
		int nType = var.vt & (~VT_ARRAY);

		if (nType != VT_VARIANT && nType != VT_I4 && nType != VT_I2 && nType != VT_R4
			&& nType != VT_R8)
			return;

		COleSafeArray saSrc(var);

		if (saSrc.GetDim() != 1)
			return;

		int nCount = saSrc.GetOneDimSize();

		long nLBound = 0;
		saSrc.GetLBound(1, &nLBound);

		if (nCount < 1)
			return;

		CXTPChartLineDashArray array;
		array.SetSize(nCount);

		for (long nIndex = 0; nIndex < nCount; nIndex++)
		{
			long nElement = nIndex + nLBound;

			if (nType == VT_VARIANT)
			{
				VARIANT varData;
				saSrc.GetElement(&nElement, &varData);

				COleVariant vtData(varData);
				vtData.ChangeType(VT_R4);

				array[nIndex] = vtData.fltVal;
			}
			else if (nType == VT_I2 || nType == VT_I4)
			{
				int nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				array[nIndex] = nItem;
			}
			else if (nType == VT_R4)
			{
				float nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				array[nIndex] = nItem;
			}
			else if (nType == VT_R8)
			{
				double nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				array[nIndex] = nItem;
			}
		}

		SetDashArray(array);
	}
}

#endif
