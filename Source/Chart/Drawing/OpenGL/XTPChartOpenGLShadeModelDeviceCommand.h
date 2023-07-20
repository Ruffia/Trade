// XTPChartOpenGLShadeModelDeviceCommand.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTOPENGLPOLYGONDEVICECOMMAND_H__)
#	define __XTPCHARTOPENGLPOLYGONDEVICECOMMAND_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class _XTP_EXT_CLASS CXTPChartOpenGLShadeModelDeviceCommand
	: public CXTPChartPolygonAntialiasingDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartOpenGLShadeModelDeviceCommand object.
	// Parameters:
	//      bAntiAlias - TRUE if antialiasing is enabled, FALSE if
	//                   antialiasing is disabled.
	//-----------------------------------------------------------------------
	CXTPChartOpenGLShadeModelDeviceCommand(BOOL bAntiAlias = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles object destruction.
	//-----------------------------------------------------------------------
	~CXTPChartOpenGLShadeModelDeviceCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called before a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart native device context.
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called after a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart native device context.
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);

private:
	GLint m_nOldShadeModel;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTOPENGLPOLYGONDEVICECOMMAND_H__)
//}}AFX_CODEJOCK_PRIVATE
