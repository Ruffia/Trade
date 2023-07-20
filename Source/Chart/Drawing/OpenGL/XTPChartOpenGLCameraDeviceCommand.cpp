// XTPChartOpenGLCameraDeviceCommand.cpp
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
#include "GraphicLibrary/OpenGL/GLU.h"
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

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
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLCameraDeviceCommand.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLCameraDeviceCommand

CXTPChartOpenGLCameraDeviceCommand::CXTPChartOpenGLCameraDeviceCommand(
	const CXTPPoint3f& ptPosition, const CXTPPoint3f& ptFocal, const CXTPPoint3f& ptUpVector)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
	, m_ptPosition(ptPosition)
	, m_ptFocal(ptFocal)
	, m_ptUpVector(ptUpVector)
{
}

void CXTPChartOpenGLCameraDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	gluLookAt(m_ptPosition.X, m_ptPosition.Y, m_ptPosition.Z, m_ptFocal.X, m_ptFocal.Y, m_ptFocal.Z,
			  m_ptUpVector.X, m_ptUpVector.Y, m_ptUpVector.Z);

	CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext, pDC);
	if (NULL != p3DDC)
	{
		p3DDC->OnUpdateCameraState(m_ptPosition, m_ptFocal, m_ptUpVector);
	}
}
