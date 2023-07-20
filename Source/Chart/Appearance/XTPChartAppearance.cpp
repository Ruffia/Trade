// XTPChartAppearance.cpp
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartContent.h"

#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Appearance/XTPChartPalette.h"
#include "Chart/Appearance/XTPChartFillStyle.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartElementAppearance

CXTPChartElementAppearance::CXTPChartElementAppearance(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;
}

CXTPChartElementAppearance::~CXTPChartElementAppearance()
{
}

void CXTPChartElementAppearance::ApplyToDeviceContext(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	// Do nothing by default.
	UNREFERENCED_PARAMETER(pDC);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisAppearance

CXTPChartAxisAppearance::CXTPChartAxisAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	InterlacedFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartAxisAppearance::~CXTPChartAxisAppearance()
{
	SAFE_RELEASE(InterlacedFillStyle);
}

void CXTPChartAxisAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("Color"), Color);
	PX_Color(pPX, _T("InterlacedColor"), InterlacedColor);
	PX_Color(pPX, _T("InterlacedColor2"), InterlacedColor2);

	CXTPPropExchangeSection pxInterlacedFillStyle(pPX->GetSection(_T("InterlacedFillStyle")));
	InterlacedFillStyle->DoPropExchange(&pxInterlacedFillStyle);

	CXTPPropExchangeSection pxGridLines(pPX->GetSection(_T("GridLines")));
	PX_Color(&pxGridLines, _T("Color"), GridLinesColor);
	PX_Color(&pxGridLines, _T("MinorColor"), GridLinesMinorColor);

	CXTPPropExchangeSection pxAxisLabel(pPX->GetSection(_T("AxisLabel")));
	PX_Color(&pxAxisLabel, _T("TextColor"), AxisLabelTextColor);

	CXTPPropExchangeSection pxAxisTitle(pPX->GetSection(_T("AxisTitle")));
	PX_Color(&pxAxisTitle, _T("TextColor"), AxisTitleTextColor);

	CXTPPropExchangeSection pxAxisConstantLine(pPX->GetSection(_T("ConstantLine")));
	PX_Color(&pxAxisConstantLine, _T("Color"), ConstantLineColor);
	PX_Color(&pxAxisConstantLine, _T("TextColor"), ConstantLineTextColor);

	CXTPPropExchangeSection pxAxisStrip(pPX->GetSection(_T("Strip")));
	PX_Color(&pxAxisStrip, _T("Color"), StripColor);
	PX_Color(&pxAxisStrip, _T("Color2"), StripColor2);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisAppearance, CXTPChartElementAppearance)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "Color", 1, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "InterlacedColor", 2, OleGetInterlacedColor,
						OleSetInterlacedColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "InterlacedColor2", 3, OleGetInterlacedColor2,
						OleSetInterlacedColor2, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "InterlacedFillStyle", 4,
						OleGetInterlacedFillStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "GridLinesColor", 5, OleGetGridLinesColor,
						OleSetGridLinesColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "GridLinesMinorColor", 6,
						OleGetGridLinesMinorColor, OleSetGridLinesMinorColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "AxisLabelTextColor", 7, OleGetAxisLabelTextColor,
						OleSetAxisLabelTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "AxisTitleTextColor", 8, OleGetAxisTitleTextColor,
						OleSetAxisTitleTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "ConstantLineColor", 9, OleGetConstantLineColor,
						OleSetConstantLineColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "ConstantLineTextColor", 10,
						OleGetConstantLineTextColor, OleSetConstantLineTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "StripColor", 11, OleGetStripColor,
						OleSetStripColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisAppearance, "StripColor2", 12, OleGetStripColor2,
						OleSetStripColor2, VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAxisAppearance, CXTPChartElementAppearance)
	INTERFACE_PART(CXTPChartAxisAppearance, XTPDIID_ChartAxisAppearance, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisAppearance, XTPDIID_ChartAxisAppearance)

OLE_COLOR CXTPChartAxisAppearance::OleGetColor()
{
	return Color.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetColor(OLE_COLOR clr)
{
	Color = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetInterlacedColor()
{
	return InterlacedColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetInterlacedColor(OLE_COLOR clr)
{
	InterlacedColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetInterlacedColor2()
{
	return InterlacedColor2.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetInterlacedColor2(OLE_COLOR clr)
{
	InterlacedColor2 = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

LPDISPATCH CXTPChartAxisAppearance::OleGetInterlacedFillStyle()
{
	return XTPGetDispatch(InterlacedFillStyle);
}

OLE_COLOR CXTPChartAxisAppearance::OleGetGridLinesColor()
{
	return GridLinesColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetGridLinesColor(OLE_COLOR clr)
{
	GridLinesColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetGridLinesMinorColor()
{
	return GridLinesMinorColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetGridLinesMinorColor(OLE_COLOR clr)
{
	GridLinesMinorColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetAxisLabelTextColor()
{
	return AxisLabelTextColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetAxisLabelTextColor(OLE_COLOR clr)
{
	AxisLabelTextColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetAxisTitleTextColor()
{
	return AxisTitleTextColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetAxisTitleTextColor(OLE_COLOR clr)
{
	AxisTitleTextColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetConstantLineColor()
{
	return ConstantLineColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetConstantLineColor(OLE_COLOR clr)
{
	ConstantLineColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetConstantLineTextColor()
{
	return ConstantLineTextColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetConstantLineTextColor(OLE_COLOR clr)
{
	ConstantLineTextColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetStripColor()
{
	return StripColor.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetStripColor(OLE_COLOR clr)
{
	StripColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartAxisAppearance::OleGetStripColor2()
{
	return StripColor2.ToOleColor();
}

void CXTPChartAxisAppearance::OleSetStripColor2(OLE_COLOR clr)
{
	StripColor2 = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DAppearance

CXTPChartDiagram2DAppearance::CXTPChartDiagram2DAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pAxisAppearance = new CXTPChartAxisAppearance(this);

	BackgroundFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartDiagram2DAppearance::~CXTPChartDiagram2DAppearance()
{
	SAFE_RELEASE(m_pAxisAppearance);
	SAFE_RELEASE(BackgroundFillStyle);
}

void CXTPChartDiagram2DAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("BackgroundColor2"), BackgroundColor2);
	PX_Color(pPX, _T("BorderColor"), BorderColor);

	CXTPPropExchangeSection pxBackgroundFillStyle(pPX->GetSection(_T("BackgroundFillStyle")));
	BackgroundFillStyle->DoPropExchange(&pxBackgroundFillStyle);

	CXTPPropExchangeSection pxAxis(pPX->GetSection(_T("Axis")));
	m_pAxisAppearance->DoPropExchange(&pxAxis);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartDiagram2DAppearance, CXTPChartElementAppearance)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DAppearance, "BackgroundColor", 1, OleGetBackgroundColor,
						OleSetBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DAppearance, "BackgroundColor2", 2, OleGetBackgroundColor2,
						OleSetBackgroundColor2, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DAppearance, "BorderColor", 3, OleGetBorderColor,
						OleSetBorderColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DAppearance, "BackgroundFillStyle", 4,
						OleGetBackgroundFillStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DAppearance, "AxisAppearance", 5, OleGetAxisAppearance,
						SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartDiagram2DAppearance, CXTPChartElementAppearance)
	INTERFACE_PART(CXTPChartDiagram2DAppearance, XTPDIID_ChartDiagram2DAppearance, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartDiagram2DAppearance, XTPDIID_ChartDiagram2DAppearance)

OLE_COLOR CXTPChartDiagram2DAppearance::OleGetBackgroundColor()
{
	return BackgroundColor.ToOleColor();
}

void CXTPChartDiagram2DAppearance::OleSetBackgroundColor(OLE_COLOR clr)
{
	BackgroundColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram2DAppearance::OleGetBackgroundColor2()
{
	return BackgroundColor2.ToOleColor();
}

void CXTPChartDiagram2DAppearance::OleSetBackgroundColor2(OLE_COLOR clr)
{
	BackgroundColor2 = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram2DAppearance::OleGetBorderColor()
{
	return BorderColor.ToOleColor();
}

void CXTPChartDiagram2DAppearance::OleSetBorderColor(OLE_COLOR clr)
{
	BorderColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

LPDISPATCH CXTPChartDiagram2DAppearance::OleGetBackgroundFillStyle()
{
	return XTPGetDispatch(BackgroundFillStyle);
}

LPDISPATCH CXTPChartDiagram2DAppearance::OleGetAxisAppearance()
{
	return XTPGetDispatch(m_pAxisAppearance);
}

#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram3DAppearance

const CXTPPoint3f CXTPChartDiagram3DAppearance::DefaultLightPosition(1.f, 1.f, 1.f);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultLightAmbientColor(0xff, 0, 0, 0);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultLightDiffuseColor(0xff, 0xff, 0xff, 0xff);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultLightSpecularColor(0xff, 0xff, 0xff,
																			 0xff);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultLightModelAmbientColor(0xff, 0x66, 0x66,
																				 0x66);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultMaterialEmissionColor(0xff, 0, 0, 0);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultMaterialDiffuseColor(0xff, 0x03, 0x0,
																			   0x03);
const CXTPChartColor CXTPChartDiagram3DAppearance::DefaultMaterialSpecularColor(0xff, 0x80, 0x80,
																				0x80);
const float CXTPChartDiagram3DAppearance::DefaultMaterialShininess(1.f);
const BOOL CXTPChartDiagram3DAppearance::DefaultAntialiasing = TRUE;

CXTPChartDiagram3DAppearance::CXTPChartDiagram3DAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
	, LightPosition(DefaultLightPosition)
	, LightAmbientColor(DefaultLightAmbientColor)
	, LightDiffuseColor(DefaultLightDiffuseColor)
	, LightSpecularColor(DefaultLightSpecularColor)
	, LightModelAmbientColor(DefaultLightModelAmbientColor)
	, MaterialEmissionColor(DefaultMaterialEmissionColor)
	, MaterialDiffuseColor(DefaultMaterialDiffuseColor)
	, MaterialSpecularColor(DefaultMaterialSpecularColor)
	, MaterialShininess(DefaultMaterialShininess)
	, Antialiasing(DefaultAntialiasing)
{
}

CXTPChartDiagram3DAppearance::~CXTPChartDiagram3DAppearance()
{
}

void CXTPChartDiagram3DAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Point(pPX, _T("LightPosition"), LightPosition, DefaultLightPosition);
	PX_Color(pPX, _T("LightAmbientColor"), LightAmbientColor, DefaultLightAmbientColor);
	PX_Color(pPX, _T("LightDiffuseColor"), LightDiffuseColor, DefaultLightDiffuseColor);
	PX_Color(pPX, _T("LightSpecularColor"), LightSpecularColor, DefaultLightSpecularColor);
	PX_Color(pPX, _T("LightModelAmbientColor"), LightModelAmbientColor,
			 DefaultLightModelAmbientColor);
	PX_Color(pPX, _T("MaterialEmissionColor"), MaterialEmissionColor, DefaultMaterialEmissionColor);
	PX_Color(pPX, _T("MaterialDiffuseColor"), MaterialDiffuseColor, DefaultMaterialDiffuseColor);
	PX_Color(pPX, _T("MaterialSpecularColor"), MaterialSpecularColor, DefaultMaterialSpecularColor);
	PX_Float(pPX, _T("MaterialShininess"), MaterialShininess, DefaultMaterialShininess);
	PX_Bool(pPX, _T("Antialiasing"), Antialiasing, DefaultAntialiasing);
}

void CXTPChartDiagram3DAppearance::ApplyToDeviceContext(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext, pDC);
	ASSERT("Unsupported device context provided" && NULL != p3DDC);

	if (NULL != p3DDC)
	{
		p3DDC->SetLightPosition(LightPosition);
		p3DDC->SetLightAmbientColor(LightAmbientColor);
		p3DDC->SetLightDiffuseColor(LightDiffuseColor);
		p3DDC->SetLightSpecularColor(LightSpecularColor);
		p3DDC->SetLightModelAmbientColor(LightModelAmbientColor);
		p3DDC->SetMaterialEmissionColor(MaterialEmissionColor);
		p3DDC->SetMaterialDiffuseColor(MaterialDiffuseColor);
		p3DDC->SetMaterialSpecularColor(MaterialSpecularColor);
		p3DDC->SetMaterialShininess(MaterialShininess);
		p3DDC->SetAntialiasing(Antialiasing);
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartDiagram3DAppearance, CXTPChartElementAppearance)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightPositionX", 1, OleGetLightPositionX,
						OleSetLightPositionX, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightPositionY", 2, OleGetLightPositionY,
						OleSetLightPositionY, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightPositionZ", 3, OleGetLightPositionZ,
						OleSetLightPositionZ, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightAmbientColor", 4,
						OleGetLightAmbientColor, OleSetLightAmbientColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightDiffuseColor", 5,
						OleGetLightDiffuseColor, OleSetLightDiffuseColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightSpecularColor", 6,
						OleGetLightSpecularColor, OleSetLightSpecularColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "LightModelAmbientColor", 7,
						OleGetLightModelAmbientColor, OleSetLightModelAmbientColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "MaterialEmissionColor", 8,
						OleGetMaterialEmissionColor, OleSetMaterialEmissionColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "MaterialDiffuseColor", 9,
						OleGetMaterialDiffuseColor, OleSetMaterialDiffuseColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "MaterialSpecularColor", 10,
						OleGetMaterialSpecularColor, OleSetMaterialSpecularColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "MaterialShininess", 11,
						OleGetMaterialShininess, OleSetMaterialShininess, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram3DAppearance, "Antialiasing", 12, OleGetAntialiasing,
						OleSetAntialiasing, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartDiagram3DAppearance, CXTPChartElementAppearance)
	INTERFACE_PART(CXTPChartDiagram3DAppearance, XTPDIID_ChartDiagram3DAppearance, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartDiagram3DAppearance, XTPDIID_ChartDiagram3DAppearance)

double CXTPChartDiagram3DAppearance::OleGetLightPositionX()
{
	return LightPosition.X;
}

void CXTPChartDiagram3DAppearance::OleSetLightPositionX(double dValue)
{
	LightPosition.X = static_cast<float>(dValue);
	GetContent()->OnChartChanged();
}

double CXTPChartDiagram3DAppearance::OleGetLightPositionY()
{
	return LightPosition.Y;
}

void CXTPChartDiagram3DAppearance::OleSetLightPositionY(double dValue)
{
	LightPosition.Y = static_cast<float>(dValue);
	GetContent()->OnChartChanged();
}

double CXTPChartDiagram3DAppearance::OleGetLightPositionZ()
{
	return LightPosition.Z;
}

void CXTPChartDiagram3DAppearance::OleSetLightPositionZ(double dValue)
{
	LightPosition.Z = static_cast<float>(dValue);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetLightAmbientColor()
{
	return LightAmbientColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetLightAmbientColor(OLE_COLOR clr)
{
	LightAmbientColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetLightDiffuseColor()
{
	return LightAmbientColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetLightDiffuseColor(OLE_COLOR clr)
{
	LightAmbientColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetLightSpecularColor()
{
	return LightSpecularColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetLightSpecularColor(OLE_COLOR clr)
{
	LightSpecularColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetLightModelAmbientColor()
{
	return LightModelAmbientColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetLightModelAmbientColor(OLE_COLOR clr)
{
	LightModelAmbientColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetMaterialEmissionColor()
{
	return MaterialEmissionColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetMaterialEmissionColor(OLE_COLOR clr)
{
	MaterialEmissionColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetMaterialDiffuseColor()
{
	return MaterialDiffuseColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetMaterialDiffuseColor(OLE_COLOR clr)
{
	MaterialDiffuseColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

OLE_COLOR CXTPChartDiagram3DAppearance::OleGetMaterialSpecularColor()
{
	return MaterialSpecularColor.ToOleColor();
}

void CXTPChartDiagram3DAppearance::OleSetMaterialSpecularColor(OLE_COLOR clr)
{
	MaterialSpecularColor = CXTPChartColor::FromOleColor(clr);
	GetContent()->OnChartChanged();
}

double CXTPChartDiagram3DAppearance::OleGetMaterialShininess()
{
	return MaterialShininess;
}

void CXTPChartDiagram3DAppearance::OleSetMaterialShininess(double dValue)
{
	MaterialShininess = static_cast<float>(dValue);
	GetContent()->OnChartChanged();
}

BOOL CXTPChartDiagram3DAppearance::OleGetAntialiasing()
{
	return Antialiasing;
}

void CXTPChartDiagram3DAppearance::OleSetAntialiasing(BOOL bValue)
{
	Antialiasing = bValue;
	GetContent()->OnChartChanged();
}

#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesLabelAppearance

CXTPChartSeriesLabelAppearance::CXTPChartSeriesLabelAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
}

CXTPChartSeriesLabelAppearance::~CXTPChartSeriesLabelAppearance()
{
}
void CXTPChartSeriesLabelAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("TextColor"), TextColor);
	PX_Color(pPX, _T("BorderColor"), BorderColor);
	PX_Color(pPX, _T("ConnectorColor"), ConnectorColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesStyleAppearance

CXTPChartSeriesStyleAppearance::CXTPChartSeriesStyleAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pLabelAppearance = new CXTPChartSeriesLabelAppearance(this);
}

CXTPChartSeriesStyleAppearance::~CXTPChartSeriesStyleAppearance()
{
	SAFE_RELEASE(m_pLabelAppearance);
}

void CXTPChartSeriesStyleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxTitle(pPX->GetSection(_T("Label")));
	m_pLabelAppearance->DoPropExchange(&pxTitle);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartFinanceStyleAppearance

CXTPChartFinanceStyleAppearance::CXTPChartFinanceStyleAppearance(CXTPChartElement* pOwner)
	: CXTPChartSeriesStyleAppearance(pOwner)
{
}

CXTPChartFinanceStyleAppearance::~CXTPChartFinanceStyleAppearance()
{
}

void CXTPChartFinanceStyleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyleAppearance::DoPropExchange(pPX);

	PX_Color(pPX, _T("UpColor"), UpColor);
	PX_Color(pPX, _T("DownColor"), DownColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieStyleAppearance

CXTPChartPieStyleAppearance::CXTPChartPieStyleAppearance(CXTPChartElement* pOwner)
	: CXTPChartSeriesStyleAppearance(pOwner)
{
}

CXTPChartPieStyleAppearance::~CXTPChartPieStyleAppearance()
{
}

void CXTPChartPieStyleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyleAppearance::DoPropExchange(pPX);

	PX_Color(pPX, _T("BorderColor"), BorderColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTitleAppearance

CXTPChartTitleAppearance::CXTPChartTitleAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
}

void CXTPChartTitleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("TextColor"), TextColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartLegendAppearance

CXTPChartLegendAppearance::CXTPChartLegendAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
}

void CXTPChartLegendAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("TextColor"), TextColor);
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("BorderColor"), BorderColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartContentAppearance

CXTPChartContentAppearance::CXTPChartContentAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pTitleAppearance  = new CXTPChartTitleAppearance(this);
	m_pLegendAppearance = new CXTPChartLegendAppearance(this);
}

CXTPChartContentAppearance::~CXTPChartContentAppearance()
{
	SAFE_RELEASE(m_pLegendAppearance);
	SAFE_RELEASE(m_pTitleAppearance);
}

void CXTPChartContentAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("BorderColor"), BorderColor);

	CXTPPropExchangeSection pxTitle(pPX->GetSection(_T("ChartTitle")));
	m_pTitleAppearance->DoPropExchange(&pxTitle);

	CXTPPropExchangeSection pxLegend(pPX->GetSection(_T("Legend")));
	m_pLegendAppearance->DoPropExchange(&pxLegend);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartAppearance

CXTPChartAppearance::CXTPChartAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pDiagram2DAppearance = new CXTPChartDiagram2DAppearance(this);
	m_pDiagram3DAppearance = new CXTPChartDiagram3DAppearance(this);
	m_pContentAppearance   = new CXTPChartContentAppearance(this);

	m_pSeriesStyleAppearance  = new CXTPChartSeriesStyleAppearance(this);
	m_pFinanceStyleAppearance = new CXTPChartFinanceStyleAppearance(this);
	m_pPieStyleAppearance	 = new CXTPChartPieStyleAppearance(this);

	m_pPalette = new CXTPChartPalette(this);

	// load default colors.
	VERIFY(LoadAppearance(_T("CHART_APPEARANCE_NATURE")));
	VERIFY(LoadPalette(_T("CHART_PALETTE_NATURE")));

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAppearance::~CXTPChartAppearance()
{
	SAFE_RELEASE(m_pDiagram2DAppearance);
	SAFE_RELEASE(m_pDiagram3DAppearance);
	SAFE_RELEASE(m_pContentAppearance);

	SAFE_RELEASE(m_pSeriesStyleAppearance);
	SAFE_RELEASE(m_pFinanceStyleAppearance);
	SAFE_RELEASE(m_pPieStyleAppearance);

	SAFE_RELEASE(m_pPalette);
}

void CXTPChartAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxContent(pPX->GetSection(_T("Content")));
	m_pContentAppearance->DoPropExchange(&pxContent);

	CXTPPropExchangeSection pxDiagram2D(pPX->GetSection(_T("Diagram2D")));
	m_pDiagram2DAppearance->DoPropExchange(&pxDiagram2D);

	CXTPPropExchangeSection pxDiagram3D(pPX->GetSection(_T("Diagram3D")));
	m_pDiagram3DAppearance->DoPropExchange(&pxDiagram3D);

	CXTPPropExchangeSection pxSeriesStyleAppearance(pPX->GetSection(_T("SeriesStyle")));
	m_pSeriesStyleAppearance->DoPropExchange(&pxSeriesStyleAppearance);

	CXTPPropExchangeSection pxFinanceSeriesAppearance(pPX->GetSection(_T("FinanceSeriesStyle")));
	m_pFinanceStyleAppearance->DoPropExchange(&pxFinanceSeriesAppearance);

	CXTPPropExchangeSection pxPieSeriesAppearance(pPX->GetSection(_T("PieSeriesStyle")));
	m_pPieStyleAppearance->DoPropExchange(&pxPieSeriesAppearance);
}

BOOL CXTPChartAppearance::LoadAppearance(LPCTSTR lpszAppearance)
{
	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("Appearance"));

	HMODULE hInstance = NULL;
	HRSRC hResource   = XTPResourceManager()->FindResource(hInstance, lpszAppearance, RT_HTML);

	if (!hResource)
		return FALSE;

	if (!px.LoadFromResource(hInstance, lpszAppearance, RT_HTML))
		return FALSE;

	if (!px.OnBeforeExchange())
		return FALSE;

	px.SetCompactMode(TRUE);

	DoPropExchange(&px);

	OnChartChanged();

	return TRUE;
}

BOOL CXTPChartAppearance::LoadPalette(LPCTSTR lpszPallete)
{
	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("Palette"));

	HMODULE hInstance = NULL;
	HRSRC hResource   = XTPResourceManager()->FindResource(hInstance, lpszPallete, RT_HTML);

	if (!hResource)
		return FALSE;

	if (!px.LoadFromResource(hInstance, lpszPallete, RT_HTML))
		return FALSE;

	if (!px.OnBeforeExchange())
		return FALSE;

	px.SetCompactMode(TRUE);

	m_pPalette->DoPropExchange(&px);

	OnChartChanged();

	return TRUE;
}

CXTPChartAppearance* CXTPChartAppearance::GetAppearance(const CXTPChartElement* pElement)
{
	return pElement->GetContent()->GetAppearance();
}

CXTPChartColor CXTPChartAppearance::GetLightColor(const CXTPChartColor& clr)
{
	COLORREF clrBackground = RGB(clr.GetR(), clr.GetG(), clr.GetB());

	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL			  = GetBValue(dwHSLBackground);
	DWORD dwLight		  = (dwL + 240) / 2;

	CXTPChartColor res;
	res.SetFromCOLORREF(CXTPDrawHelpers::HSLtoRGB(
		RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwLight)));

	return res;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAppearance, CXTPChartElementAppearance)
	DISP_FUNCTION_ID(CXTPChartAppearance, "SetAppearance", 1, OleSetAppearance, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartAppearance, "SetPalette", 2, OleSetPalette, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartAppearance, "LoadAppearance", 3, OleLoadAppearance, VT_EMPTY,
					 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPChartAppearance, "LoadPalette", 4, OleLoadPalette, VT_EMPTY, VTS_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAppearance, "Palette", 5, OleGetPalette, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAppearance, "Diagram2DAppearance", 6, OleGetDiagram2DAppearance,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAppearance, "Diagram3DAppearance", 7, OleGetDiagram3DAppearance,
						SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartAppearance, CXTPChartElementAppearance)
	INTERFACE_PART(CXTPChartAppearance, XTPDIID_ChartAppearance, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAppearance, XTPDIID_ChartAppearance)

void CXTPChartAppearance::OleSetAppearance(LPCTSTR lpszName)
{
	CString str(lpszName);
	str.MakeUpper();
	str.Replace(_T(" "), _T(""));

	LoadAppearance(_T("CHART_APPEARANCE_") + str);
}

void CXTPChartAppearance::OleSetPalette(LPCTSTR lpszName)
{
	CString str(lpszName);
	str.MakeUpper();
	str.Replace(_T(" "), _T(""));

	LoadPalette(_T("CHART_PALETTE_") + str);
}

void CXTPChartAppearance::OleLoadAppearance(LPDISPATCH lpPropExchage)
{
	CXTPPropExchangeSection px(PropExchangeFromControl(lpPropExchage));

	if ((CXTPPropExchange*)&px == NULL)
		return;

	if (px->OnBeforeExchange())
	{
		if (px->IsKindOf(RUNTIME_CLASS(CXTPPropExchangeXMLNode)))
			((CXTPPropExchangeXMLNode*)&px)->SetCompactMode(TRUE);

		DoPropExchange(&px);

		OnChartChanged();
	}
}

void CXTPChartAppearance::OleLoadPalette(LPDISPATCH lpPropExchage)
{
	CXTPPropExchangeSection px(PropExchangeFromControl(lpPropExchage));

	if ((CXTPPropExchange*)&px == NULL)
		return;

	if (px->OnBeforeExchange())
	{
		if (px->IsKindOf(RUNTIME_CLASS(CXTPPropExchangeXMLNode)))
			((CXTPPropExchangeXMLNode*)&px)->SetCompactMode(TRUE);

		m_pPalette->DoPropExchange(&px);

		OnChartChanged();
	}
}

LPDISPATCH CXTPChartAppearance::OleGetPalette()
{
	return XTPGetDispatch(GetPalette());
}

LPDISPATCH CXTPChartAppearance::OleGetDiagram2DAppearance()
{
	return XTPGetDispatch(GetDiagram2DAppearance());
}

LPDISPATCH CXTPChartAppearance::OleGetDiagram3DAppearance()
{
	return XTPGetDispatch(GetDiagram3DAppearance());
}

#endif
