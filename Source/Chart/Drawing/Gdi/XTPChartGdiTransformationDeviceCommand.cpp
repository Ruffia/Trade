// XTPChartGdiTransformationDeviceCommand.cpp
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
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/Gdi/XTPChartGdiDeviceContext.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/Gdi/XTPChartGdiTransformationDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiRotateDeviceCommand

CXTPChartGdiRotateDeviceCommand::CXTPChartGdiRotateDeviceCommand(float fAngle)
	: CXTPChartRotateDeviceCommand(fAngle)
{
}

CXTPChartGdiRotateDeviceCommand::CXTPChartGdiRotateDeviceCommand(float fAngle,
																 const CXTPPoint3d& rotateVector)
	: CXTPChartRotateDeviceCommand(fAngle, rotateVector)
{
}

void CXTPChartGdiRotateDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipRotateWorldTransform(pGdiDC->GetGraphics(), m_fAngle, MatrixOrderPrepend);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiSaveStateDeviceCommand

CXTPChartGdiSaveStateDeviceCommand::CXTPChartGdiSaveStateDeviceCommand()
{
	m_nState = 0;
}

void CXTPChartGdiSaveStateDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSaveGraphics(pGdiDC->GetGraphics(), &m_nState);
}

void CXTPChartGdiSaveStateDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipRestoreGraphics(pGdiDC->GetGraphics(), m_nState);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiTranslateDeviceCommand

CXTPChartGdiTranslateDeviceCommand::CXTPChartGdiTranslateDeviceCommand(double dx, double dy)
	: CXTPChartTranslateDeviceCommand(dx, dy)
{
}

void CXTPChartGdiTranslateDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipTranslateWorldTransform(pGdiDC->GetGraphics(), (float)m_dx, (float)m_dy,
								MatrixOrderPrepend);
}

CXTPChartElement* CXTPChartGdiTranslateDeviceCommand::HitTest(CPoint point,
															  CXTPChartElement* pParent) const
{
	return CXTPChartDeviceCommand::HitTest(CPoint(int(point.x - m_dx), int(point.y - m_dy)),
										   pParent);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiClipDeviceCommand

CXTPChartGdiClipDeviceCommand::CXTPChartGdiClipDeviceCommand(CRect rcClip)
	: CXTPChartClipDeviceCommand(rcClip)
{
}

void CXTPChartGdiClipDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipGetClipBoundsI(pGdiDC->GetGraphics(), (GpRect*)&m_rcState);
	GdipSetClipRectI(pGdiDC->GetGraphics(), m_rcClip.left, m_rcClip.top, m_rcClip.Width(),
					 m_rcClip.Height(), CombineModeIntersect);
}

void CXTPChartGdiClipDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSetClipRectI(pGdiDC->GetGraphics(), m_rcState.left, m_rcState.top, m_rcState.Width(),
					 m_rcState.Width(), CombineModeReplace);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGdiClipRegionDeviceCommand

CXTPChartGdiClipRegionDeviceCommand::CXTPChartGdiClipRegionDeviceCommand(
	const CXTPChartPoints& points)
{
	GpPath* pGpPath = NULL;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipAddPathPolygon(pGpPath, (GpPointF*)points.GetData(), (int)points.GetSize());

	m_pGpClip = NULL;
	GdipCreateRegionPath(pGpPath, &m_pGpClip);

	m_pGpState = NULL;
	GdipCreateRegion(&m_pGpState);

	GdipDeletePath(pGpPath);
}

CXTPChartGdiClipRegionDeviceCommand::~CXTPChartGdiClipRegionDeviceCommand()
{
	GdipDeleteRegion(m_pGpClip);
	GdipDeleteRegion(m_pGpState);
}

void CXTPChartGdiClipRegionDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipGetClip(pGdiDC->GetGraphics(), m_pGpState);
	GdipSetClipRegion(pGdiDC->GetGraphics(), m_pGpClip, CombineModeExclude);
}

void CXTPChartGdiClipRegionDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_KINDOF(CXTPChartGdiDeviceContext, pDC);
	CXTPChartGdiDeviceContext* pGdiDC = static_cast<CXTPChartGdiDeviceContext*>(pDC);

	GdipSetClipRegion(pGdiDC->GetGraphics(), m_pGpState, CombineModeReplace);
}
