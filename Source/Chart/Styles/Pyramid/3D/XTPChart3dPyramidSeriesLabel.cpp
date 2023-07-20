// XTPChart3dPyramidSeriesLabel.cpp
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

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/PropExchange/XTPPropExchangeXMLNode.h"
#include "Common/PropExchange/XTPPropExchangeRegistry.h"
#include "Common/PropExchange/XTPPropExchangeIniFile.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"

#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartSeriesPointView.h"

#include "Chart/Appearance/XTPChartFillStyle.h"
#include "Chart/Appearance/XTPChartBorder.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidDiagramBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesPointViewBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesViewBase.h"

#include "Chart/Styles/Pyramid/3D/Commands/XTPChart3dPyramidDeviceCommandEvents.h"
#include "Chart/Styles/Pyramid/3D/Commands/XTPChart3dPyramidSeriesLabelsViewDeviceCommand.h"

#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesLabel.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesStyle.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesPointView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesLabel

IMPLEMENT_SERIAL(CXTPChart3dPyramidSeriesLabel, CXTPChartPyramidSeriesLabelBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChart3dPyramidSeriesLabel::CXTPChart3dPyramidSeriesLabel()
{
	Set3D();
}

CXTPChartElementView* CXTPChart3dPyramidSeriesLabel::CreateView(CXTPChartDeviceContext* pDC,
																CXTPChartSeriesPointView* pPointView,
																CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChart3dPyramidSeriesLabelView(this, pPointView, pParentView);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesLabelView

IMPLEMENT_DYNAMIC(CXTPChart3dPyramidSeriesLabelView, CXTPChartPyramidSeriesLabelViewBase);

CXTPChart3dPyramidSeriesLabelView::CXTPChart3dPyramidSeriesLabelView(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartPyramidSeriesLabelViewBase(pLabel, pPointView, pParentView)
	, m_ptStart(NULL)
	, m_ptBend(NULL)
	, m_ptEnd(NULL)
	, m_ptLabelAnchor(INT_MIN, INT_MIN)
	, m_ptLabelAnchorOrigin(INT_MIN, INT_MIN)
	, m_bLeftDirected(FALSE)
	, m_bConnectorVisible(TRUE)
{
	m_p3dPointView = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesPointView, pPointView);
	ASSERT(NULL != m_p3dPointView);
}

CXTPChart3dPyramidSeriesLabelView::~CXTPChart3dPyramidSeriesLabelView()
{
	SAFE_DELETE(m_ptStart);
	SAFE_DELETE(m_ptBend);
	SAFE_DELETE(m_ptEnd);
}

CXTPChartDeviceCommand*
	CXTPChart3dPyramidSeriesLabelView::CreateTranspositionDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChart3dPyramidSeriesStyle* pStyle = DYNAMIC_DOWNCAST(
		CXTPChart3dPyramidSeriesStyle, m_p3dPointView->GetSeriesView()->GetStyle());
	ASSERT(NULL != pStyle);

	CXTPChartDeviceCommand* pCommand =
		CXTPChartDrawingObjectFactory::Create3dSaveMatrixDeviceCommand();

	// Rotate if rotated
	const CXTPChart3dRotation& rotation = pStyle->Get3dRotation();
	if (rotation.IsRotated())
	{
		pCommand->AddChildCommand(
			CXTPChartDrawingObjectFactory::Create3dRotateDeviceCommand(rotation));
	}

	return pCommand;
}

CXTPChartDeviceCommand*
	CXTPChart3dPyramidSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = CreateTranspositionDeviceCommand(pDC);
	ASSERT(NULL != pCommand);
	if (NULL != pCommand)
	{
		pCommand->AddChildCommand(new CXTPChart3dPyramidSeriesLabelDeviceCommand(this));
	}

	return pCommand;
}

CXTPChartDeviceCommand*
	CXTPChart3dPyramidSeriesLabelView::CreateConnectorDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	// No special command for connector
	return NULL;
}

void CXTPChart3dPyramidSeriesLabelView::SetConnectorStartPoint(const CXTPPoint3d& point)
{
	SAFE_DELETE(m_ptStart);
	m_ptStart = new CXTPPoint3d(point);
}

void CXTPChart3dPyramidSeriesLabelView::SetConnectorBendPoint(const CXTPPoint3d& point)
{
	SAFE_DELETE(m_ptBend);
	m_ptBend = new CXTPPoint3d(point);
}

void CXTPChart3dPyramidSeriesLabelView::SetConnectorEndPoint(const CXTPPoint3d& point)
{
	SAFE_DELETE(m_ptEnd);
	m_ptEnd = new CXTPPoint3d(point);
}

const CPoint* CXTPChart3dPyramidSeriesLabelView::GetLabelAnchorPoint() const
{
	return INT_MIN != m_ptLabelAnchor.x || INT_MIN != m_ptLabelAnchor.y ? &m_ptLabelAnchor : NULL;
}

const CPoint* CXTPChart3dPyramidSeriesLabelView::GetLabelAnchorOrigin() const
{
	return INT_MIN != m_ptLabelAnchorOrigin.x || INT_MIN != m_ptLabelAnchorOrigin.y
			   ? &m_ptLabelAnchorOrigin
			   : NULL;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesLabelsView

IMPLEMENT_DYNAMIC(CXTPChart3dPyramidSeriesLabelsView, CXTPChartElementView);

CXTPChart3dPyramidSeriesLabelsView::CXTPChart3dPyramidSeriesLabelsView(
	CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
}

CXTPChartDeviceCommand*
	CXTPChart3dPyramidSeriesLabelsView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pAggCommand = new CXTPChart3dPyramidSeriesLabelsViewDeviceCommand(this);

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPyramidSeriesLabelViewBase* pChildView =
			static_cast<CXTPChartPyramidSeriesLabelViewBase*>(m_arrChildren[i]);
		pAggCommand->AddChildCommand(pChildView->CreateDeviceCommand(pDC));
	}

	return pAggCommand;
}

void CXTPChart3dPyramidSeriesLabelsView::ResolveOverlapping()
{
	// do nothing
}

CXTPChart3dPyramidSeriesStyle* CXTPChart3dPyramidSeriesLabelsView::GetStyle() const
{
	CXTPChart3dPyramidSeriesStyle* pStyle = NULL;

	for (int i = 0; i < m_arrChildren.GetSize() && NULL != pStyle; i++)
	{
		CXTPChartPyramidSeriesLabelViewBase* pChildView =
			static_cast<CXTPChartPyramidSeriesLabelViewBase*>(m_arrChildren[i]);
		pStyle = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesStyle,
								  pChildView->GetPointView()->GetSeriesView()->GetStyle());
		ASSERT(NULL != pStyle);
	}

	return pStyle;
}
