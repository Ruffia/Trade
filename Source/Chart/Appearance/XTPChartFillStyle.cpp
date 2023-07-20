// XTPChartFillStyle.cpp
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
#include "Chart/Drawing/XTPChartCircleDeviceCommand.h"
#include "Chart/Drawing/XTPChartRectangleDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR lpszGradientAngle[] = {
	_T("TopToBottom"),			// xtpChartGradientAngle180
	_T("BottomToTop"),			// xtpChartGradientAngle0
	_T("LeftToRight"),			// xtpChartGradientAngle90
	_T("RightToLeft"),			// xtpChartGradientAngle270
	_T("TopLeftToBottomRight"), // xtpChartGradientAngle135
	_T("BottomRightToTopLeft"), // xtpChartGradientAngle315
	_T("TopRightToBottomLeft"), // xtpChartGradientAngle225
	_T("BottomLeftToTopRight"), // xtpChartGradientAngle45
};

CXTPChartFillStyle::CXTPChartFillStyle(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

	m_nFillMode = xtpChartFillSolid;

	m_nHatchStyle = xtpChartHatchStyleSolidDiamond;

	m_nGradientAngle = xtpChartGradientAngle90;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartDeviceCommand* CXTPChartFillStyle::CreateDeviceCommand(const CXTPChartRectF& bounds,
																const CXTPChartColor& color,
																const CXTPChartColor& color2)
{
	if (m_nFillMode == xtpChartFillSolid)
	{
		return CXTPChartDrawingObjectFactory::Create2dSolidRectangleDeviceCommand(bounds, color);
	}

	if (m_nFillMode == xtpChartFillHatch)
	{
		return CXTPChartDrawingObjectFactory::Create2dHatchRectangleDeviceCommand(bounds, color,
																				  color2,
																				  m_nHatchStyle);
	}

	if (m_nFillMode == xtpChartFillGradient)
	{
		switch (m_nGradientAngle)
		{
			case xtpChartGradientAngle180:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color, color2, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle0:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color2, color, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle90:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color, color2, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle270:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color2, color, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle135:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color, color2, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle315:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color2, color, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle225:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color2, color, xtpChartLinearGradientModeBackwardDiagonal);

			case xtpChartGradientAngle45:
				return CXTPChartDrawingObjectFactory::Create2dGradientRectangleDeviceCommand(
					bounds, color, color2, xtpChartLinearGradientModeBackwardDiagonal);
		}
	}

	return NULL;
}

CXTPChartDeviceCommand* CXTPChartFillStyle::CreateCircleDeviceCommand(const CXTPChartPointF& center,
																	  double radius,
																	  const CXTPChartColor& color,
																	  const CXTPChartColor& color2)
{
	if (m_nFillMode == xtpChartFillSolid)
	{
		return CXTPChartDrawingObjectFactory::Create2dSolidCircleDeviceCommand(center, radius,
																			   color);
	}

	if (m_nFillMode == xtpChartFillHatch)
	{
		return CXTPChartDrawingObjectFactory::Create2dHatchCircleDeviceCommand(center, radius,
																			   color, color2,
																			   m_nHatchStyle);
	}

	if (m_nFillMode == xtpChartFillGradient)
	{
		switch (m_nGradientAngle)
		{
			case xtpChartGradientAngle180:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color, color2, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle0:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color2, color, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle90:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color, color2, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle270:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color2, color, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle135:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color, color2, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle315:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color2, color, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle225:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color2, color, xtpChartLinearGradientModeBackwardDiagonal);

			case xtpChartGradientAngle45:
				return CXTPChartDrawingObjectFactory::Create2dGradientCircleDeviceCommand(
					center, radius, color, color2, xtpChartLinearGradientModeBackwardDiagonal);
		}
	}

	return NULL;
}

CXTPChartDeviceCommand* CXTPChartFillStyle::CreateSplineDeviceCommand(
	const CXTPChartPoints& arrPoints, const CXTPChartColor& color, const CXTPChartColor& /*color2*/,
	BOOL bTwoSides)
{
	return CXTPChartDrawingObjectFactory::Create2dSolidSplinePolygonDeviceCommand(arrPoints, color,
																				  bTwoSides);
}

CXTPChartDeviceCommand* CXTPChartFillStyle::CreateDeviceCommand(const CXTPChartPoints& arrPoints,
																const CXTPChartColor& color,
																const CXTPChartColor& color2)
{
	if (m_nFillMode == xtpChartFillSolid)
	{
		return CXTPChartDrawingObjectFactory::Create2dSolidPolygonDeviceCommand(arrPoints, color);
	}

	if (m_nFillMode == xtpChartFillHatch)
	{
		return CXTPChartDrawingObjectFactory::Create2dHatchPolygonDeviceCommand(arrPoints, color,
																				color2,
																				m_nHatchStyle);
	}

	if (m_nFillMode == xtpChartFillGradient)
	{
		switch (m_nGradientAngle)
		{
			case xtpChartGradientAngle180:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color, color2, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle0:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color2, color, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle90:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color, color2, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle270:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color2, color, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle135:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color, color2, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle315:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color2, color, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle225:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color2, color, xtpChartLinearGradientModeBackwardDiagonal);

			case xtpChartGradientAngle45:
				return CXTPChartDrawingObjectFactory::Create2dGradientPolygonDeviceCommand(
					arrPoints, color, color2, xtpChartLinearGradientModeBackwardDiagonal);
		}
	}
	return NULL;
}

void CXTPChartFillStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		CString strFillMode, strGradientAngle;

		PX_String(pPX, _T("FillMode"), strFillMode);

		if (strFillMode.CompareNoCase(_T("Gradient")) == 0)
		{
			m_nFillMode = xtpChartFillGradient;
			PX_String(pPX, _T("GradientAngle"), strGradientAngle);

			for (int i = 0; i < _countof(lpszGradientAngle); i++)
			{
				if (strGradientAngle.CompareNoCase(lpszGradientAngle[i]) == 0)
				{
					switch (i)
					{
						case 0: m_nGradientAngle = xtpChartGradientAngle180; break; // TopToBottom
						case 1: m_nGradientAngle = xtpChartGradientAngle0; break;   // BottomToTop
						case 2: m_nGradientAngle = xtpChartGradientAngle90; break;  // LeftToRight
						case 3: m_nGradientAngle = xtpChartGradientAngle270; break; // RightToLeft
						case 4:
							m_nGradientAngle = xtpChartGradientAngle135;
							break; // TopLeftToBottomRight
						case 5:
							m_nGradientAngle = xtpChartGradientAngle315;
							break; // BottomRightToTopLeft
						case 6:
							m_nGradientAngle = xtpChartGradientAngle225;
							break; // TopRightToBottomLeft
						case 7:
							m_nGradientAngle = xtpChartGradientAngle45;
							break;											// BottomLeftToTopRight
						default: m_nGradientAngle = xtpChartGradientAngle0; // BottomToTop
					}

					break;
				}
			}
		}
		else if (strFillMode.CompareNoCase(_T("Empty")) == 0)
		{
			m_nFillMode = xtpChartFillEmpty;
		}
		else if (strFillMode.CompareNoCase(_T("Hatch")) == 0)
		{
			m_nFillMode = xtpChartFillHatch;

			PX_Enum(pPX, _T("HatchStyle"), m_nHatchStyle, xtpChartHatchStyleSolidDiamond);
		}
		else
		{
			m_nFillMode = xtpChartFillSolid;
		}
	}
	else
	{
		CString strFillMode = m_nFillMode == xtpChartFillEmpty
								  ? _T("Empty")
								  : m_nFillMode == xtpChartFillGradient
										? _T("Gradient")
										: m_nFillMode == xtpChartFillHatch ? _T("Hatch") : _T("");

		PX_String(pPX, _T("FillMode"), strFillMode, _T(""));

		if (m_nFillMode == xtpChartFillGradient)
		{
			int nGradientAngleNameIndex = -1;
			switch (m_nGradientAngle)
			{
				case xtpChartGradientAngle180:
					nGradientAngleNameIndex = 0; // TopToBottom
					break;

				case xtpChartGradientAngle0:
					nGradientAngleNameIndex = 1; // BottomToTop
					break;

				case xtpChartGradientAngle90:
					nGradientAngleNameIndex = 2; // LeftToRight
					break;

				case xtpChartGradientAngle270:
					nGradientAngleNameIndex = 3; // RightToLeft
					break;

				case xtpChartGradientAngle135:
					nGradientAngleNameIndex = 4; // TopLeftToBottomRight
					break;

				case xtpChartGradientAngle315:
					nGradientAngleNameIndex = 5; // BottomRightToTopLeft
					break;

				case xtpChartGradientAngle225:
					nGradientAngleNameIndex = 6; // TopRightToBottomLeft
					break;

				case xtpChartGradientAngle45:
					nGradientAngleNameIndex = 7; // BottomLeftToTopRight
					break;
			}

			if (-1 < nGradientAngleNameIndex)
			{
				CString strGradientAngle = lpszGradientAngle[nGradientAngleNameIndex];
				PX_String(pPX, _T("GradientAngle"), strGradientAngle);
			}
		}

		if (m_nFillMode == xtpChartFillHatch)
		{
			PX_Enum(pPX, _T("HatchStyle"), m_nHatchStyle, xtpChartHatchStyleHorizontal);
		}
	}
}

void CXTPChartFillStyle::SetFillMode(XTPChartFillMode nFillMode)
{
	m_nFillMode = nFillMode;
	m_pOwner->OnChartChanged();
}

void CXTPChartFillStyle::SetGradientAngle(XTPChartGradientAngle nAngle)
{
	m_nGradientAngle = nAngle;
	m_pOwner->OnChartChanged();
}

void CXTPChartFillStyle::SetHatchStyle(XTPChartHatchStyle nStyle)
{
	m_nHatchStyle = nStyle;
	m_pOwner->OnChartChanged();
}

CXTPChartFillStyle* CXTPChartFillStyle::CreateRotatedStyle()
{
	if (m_nFillMode != xtpChartFillGradient)
	{
		CXTPChartFillStyle* pStyle = this;
		pStyle->InternalAddRef();
		return pStyle;
	}

	CXTPChartFillStyle* pStyle = new CXTPChartFillStyle(m_pOwner);
	pStyle->m_nFillMode		   = xtpChartFillGradient;

	switch (m_nGradientAngle)
	{
		case xtpChartGradientAngle0: pStyle->m_nGradientAngle = xtpChartGradientAngle90; break;
		case xtpChartGradientAngle45: pStyle->m_nGradientAngle = xtpChartGradientAngle135; break;
		case xtpChartGradientAngle90: pStyle->m_nGradientAngle = xtpChartGradientAngle180; break;
		case xtpChartGradientAngle135: pStyle->m_nGradientAngle = xtpChartGradientAngle225; break;
		case xtpChartGradientAngle180: pStyle->m_nGradientAngle = xtpChartGradientAngle270; break;
		case xtpChartGradientAngle225: pStyle->m_nGradientAngle = xtpChartGradientAngle315; break;
		case xtpChartGradientAngle270: pStyle->m_nGradientAngle = xtpChartGradientAngle0; break;
		case xtpChartGradientAngle315: pStyle->m_nGradientAngle = xtpChartGradientAngle45; break;
	}

	return pStyle;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartFillStyle, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartFillStyle, "FillMode", 1, OleGetFillMode, SetFillMode, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartFillStyle, "HatchStyle", 2, OleGetHatchStyle, SetHatchStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartFillStyle, "GradientAngle", 3, OleGetGradientAngle,
						SetGradientAngle, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartFillStyle, CXTPCmdTarget)
	INTERFACE_PART(CXTPChartFillStyle, XTPDIID_ChartFillStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFillStyle, XTPDIID_ChartFillStyle)

int CXTPChartFillStyle::OleGetFillMode()
{
	return GetFillMode();
}

int CXTPChartFillStyle::OleGetHatchStyle()
{
	return GetHatchStyle();
}

int CXTPChartFillStyle::OleGetGradientAngle()
{
	return GetGradientAngle();
}

#endif
