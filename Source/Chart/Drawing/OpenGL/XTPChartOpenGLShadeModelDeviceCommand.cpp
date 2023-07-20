// XTPChartOpenGLPolygonDeviceCommand.cpp
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
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLShadeModelDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLShadeModelDeviceCommand

CXTPChartOpenGLShadeModelDeviceCommand::CXTPChartOpenGLShadeModelDeviceCommand(BOOL bAntiAlias)
	: CXTPChartPolygonAntialiasingDeviceCommand(bAntiAlias)
	, m_nOldShadeModel(0)
{
}

CXTPChartOpenGLShadeModelDeviceCommand::~CXTPChartOpenGLShadeModelDeviceCommand()
{
}

void CXTPChartOpenGLShadeModelDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glGetIntegerv(GL_SHADE_MODEL, &m_nOldShadeModel);
	glShadeModel(static_cast<GLenum>(m_bAntiAlias ? GL_SMOOTH : GL_FLAT));
}

void CXTPChartOpenGLShadeModelDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	glShadeModel(static_cast<GLenum>(m_nOldShadeModel));
}
