// XTPChart3dDeviceContext.cpp
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
#include <float.h>

#include "GraphicLibrary/OpenGL/GL.h"
#include "GraphicLibrary/OpenGL/GLU.h"

#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPResourceManager.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLTriangleRenderList.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4510)
#pragma warning(disable : 4610)

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dDeviceContext

IMPLEMENT_DYNAMIC(CXTPChart3dDeviceContext, CXTPChartDeviceContext);

CXTPChart3dDeviceContext::CXTPChart3dDeviceContext(
	CXTPChartContainer* pContainer, HDC hDC, CRect rcBounds,
	CXTPChartDrawingObjectFactory* pDrawingObjectFactory)
	: CXTPChartDeviceContext(pContainer, hDC, TRUE, pDrawingObjectFactory)
	, m_rcBounds(rcBounds)
	, m_bRenderingAntialiasing(FALSE)
	, m_bSkippingAntialiasing(FALSE)
	, m_fMinModelDistance(0.f)
	, m_fMaxModelDistance(0.f)
	, m_ptLightPosition(1.f, 1.f, 1.f)
	, m_clrLightAmbientColor(0xff, 0, 0, 0)
	, m_clrLightDiffuseColor(0xff, 0xff, 0xff, 0xff)
	, m_clrLightSpecularColor(0xff, 0xff, 0xff, 0xff)
	, m_clrLightModelAmbientColor(0xff, 0x66, 0x66, 0x66)
	, m_clrMaterialEmissionColor(0xff, 0, 0, 0)
	, m_clrMaterialDiffuseColor(0xff, 0x03, 0x0, 0x03)
	, m_clrMaterialSpecularColor(0xff, 0x80, 0x80, 0x80)
	, m_fMaterialShininess(1.f)
	, m_bAntialiasing(TRUE)
	, m_bInitialPass(FALSE)
	, m_pTriangles(new CXTPOpenGLTriangleRenderList())
{
}

CXTPChart3dDeviceContext::~CXTPChart3dDeviceContext()
{
	SAFE_DELETE(m_pTriangles);
}

void CXTPChart3dDeviceContext::ResetSavedCameraState()
{
	OnUpdateCameraState(CXTPPoint3f(), CXTPPoint3f(), CXTPPoint3f());
}

void CXTPChart3dDeviceContext::OnUpdateCameraState(const CXTPPoint3f& ptPosition,
												   const CXTPPoint3f& ptFocal,
												   const CXTPPoint3f& ptUpVector)
{
	m_cameraState.ptPosition = ptPosition;
	m_cameraState.ptFocal	= ptFocal;
	m_cameraState.ptUpVector = ptUpVector;
}
