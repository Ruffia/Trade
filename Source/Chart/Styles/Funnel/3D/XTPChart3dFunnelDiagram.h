// XTPChart3dFunnelDiagram.h
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
#if !defined(__XTPCHART3DFUNNELDIAGRAM_H__)
#	define __XTPCHART3DFUNNELDIAGRAM_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartDeviceContext;
class CXTPChartElementView;
class CXTPChartDiagramView;

//===========================================================================
// Summary:
//     This class abstracts the funnel diagram. This class is a kind of
//     CXTPChartDiagram.
//===========================================================================
class _XTP_EXT_CLASS CXTPChart3dFunnelDiagram : public CXTPChart3dFunnelDiagramBase
{
	DECLARE_SERIAL(CXTPChart3dFunnelDiagram);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChart3dFunnelDiagram object.
	//-----------------------------------------------------------------------
	CXTPChart3dFunnelDiagram();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the funnel diagram.
	// Parameters:
	//     pDC           - A pointer to the chart device context.
	//     pParent       - A pointer to the parent view.
	// Returns:
	//     Returns a pointer to the associated diagram view object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC,
											 CXTPChartElementView* pParent);

#	ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChart3dFunnelDiagram);
	DECLARE_OLECREATE_EX(CXTPChart3dFunnelDiagram)
	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHART3DFUNNELDIAGRAM_H__)
