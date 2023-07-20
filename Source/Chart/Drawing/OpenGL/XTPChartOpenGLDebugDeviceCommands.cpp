// XTPChartOpenGLDebugDeviceCommands.cpp
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

#ifdef _DEBUG

#	include "Common/XTPTypeId.h"
#	include "Common/Math/XTPMathUtils.h"
#	include "Common/XTPFramework.h"
#	include "Common/XTPSynchro.h"
#	include "Common/XTPSystemHelpers.h"
#	include "Common/XTPApplication.h"
#	include "Common/XTPSingleton.h"
#	include "Common/Base/Types/XTPPoint2.h"
#	include "Common/Base/Types/XTPPoint3.h"
#	include "Common/Base/Types/XTPSize.h"
#	include "Common/Base/Types/XTPRect.h"
#	include "Common/XTPTypeId.h"

#	include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#	include "GraphicLibrary/OpenGL/GL.h"
#	include "GraphicLibrary/OpenGL/GLU.h"
#	include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#	include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"

#	include "Chart/Types/XTPChartTypes.h"
#	include "Chart/XTPChartDefines.h"
#	include "Chart/XTPChartElement.h"

#	include "Chart/XTPChartObjectFactory.h"
#	include "Chart/Drawing/XTPChartDrawingObjectFactory.h"
#	include "Chart/Drawing/XTPChartDeviceCommand.h"
#	include "Chart/Drawing/OpenGL/XTPChartOpenGLDebugDeviceCommands.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLDebugAxesDeviceCommand

CXTPChartOpenGLDebugAxesDeviceCommand::CXTPChartOpenGLDebugAxesDeviceCommand(const CXTPPoint3d& pt,
																			 double dSize)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
	, m_pt(pt)
	, m_dSize(dSize)
{
}

void CXTPChartOpenGLDebugAxesDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glPushMatrix();
	glTranslated(m_pt.X, m_pt.Y, m_pt.Z);
	CXTPOpenGLUtils::DebugAxes(m_dSize);
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLDebugBoxDeviceCommand

CXTPChartOpenGLDebugBoxDeviceCommand::CXTPChartOpenGLDebugBoxDeviceCommand(
	const CXTPChart3dBox& box, BOOL bSolid /*= FALSE*/)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
	, m_box(box)
	, m_bSolid(bSolid)
{
}

void CXTPChartOpenGLDebugBoxDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	CXTPOpenGLUtils::DebugBox(m_box.GetMinPoint(), m_box.GetMaxPoint(), m_bSolid);
}

#endif /*_DEBUG*/
