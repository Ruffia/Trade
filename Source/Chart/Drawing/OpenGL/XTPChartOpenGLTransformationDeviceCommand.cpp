// XTPChartOpenGLTransformationDeviceCommand.cpp
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

#include "GraphicLibrary/OpenGL/GL.h"

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
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMatrix.h"

#include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLTransformationDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLSaveMatrixDeviceCommand

CXTPChartOpenGLSaveMatrixDeviceCommand::CXTPChartOpenGLSaveMatrixDeviceCommand()
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
{
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLLoadIdentityDeviceCommand

CXTPChartOpenGLLoadIdentityDeviceCommand::CXTPChartOpenGLLoadIdentityDeviceCommand()
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
{
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLSaveMatrixDeviceCommand

void CXTPChartOpenGLSaveMatrixDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glPushMatrix();
}

void CXTPChartOpenGLSaveMatrixDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLLoadIdentityDeviceCommand

void CXTPChartOpenGLLoadIdentityDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLMultiplyMatrixDeviceCommand

CXTPChartOpenGLMultiplyMatrixDeviceCommand::CXTPChartOpenGLMultiplyMatrixDeviceCommand(
	const CXTPMatrix<double>& matrix, BOOL bPushMatrix)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDefault)
	, m_pMatrix(matrix.Clone())
	, m_bPushMatrix(bPushMatrix)
{
	ASSERT(4 == m_pMatrix->GetWidth() && 4 == m_pMatrix->GetHeight());
}

CXTPChartOpenGLMultiplyMatrixDeviceCommand::~CXTPChartOpenGLMultiplyMatrixDeviceCommand()
{
	SAFE_DELETE(m_pMatrix);
}

void CXTPChartOpenGLMultiplyMatrixDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	if (m_bPushMatrix)
	{
		glPushMatrix();
	}
}

void CXTPChartOpenGLMultiplyMatrixDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glMultMatrixd(m_pMatrix->GetData());
}

void CXTPChartOpenGLMultiplyMatrixDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	if (m_bPushMatrix)
	{
		glPopMatrix();
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLRotateDeviceCommand

CXTPChartOpenGLRotateDeviceCommand::CXTPChartOpenGLRotateDeviceCommand(
	const CXTPChart3dRotation& rotation)
	: CXTPChart3dRotateDeviceCommand(rotation)
{
}

void CXTPChartOpenGLRotateDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPOpenGLUtils::Rotate(m_rotation.m_dYaw, m_rotation.m_dPitch, m_rotation.m_dRoll);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLTranslateDeviceCommand

CXTPChartOpenGLTranslateDeviceCommand::CXTPChartOpenGLTranslateDeviceCommand(double dx, double dy,
																			 double dz)
	: CXTPChart3dTranslateDeviceCommand(dx, dy, dz)
{
}

void CXTPChartOpenGLTranslateDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glTranslated(m_dx, m_dy, m_dz);
}
