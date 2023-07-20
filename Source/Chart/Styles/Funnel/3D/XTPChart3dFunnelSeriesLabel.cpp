// XTPChart3dFunnelSeriesLabel.cpp
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

#include "Chart/Styles/Funnel/Base/XTPChartFunnelDiagramBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesLabelBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesStyleBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesPointViewBase.h"
#include "Chart/Styles/Funnel/Base/XTPChartFunnelSeriesViewBase.h"

#include "Chart/Styles/Funnel/3D/Commands/XTPChart3dFunnelDeviceCommandEvents.h"
#include "Chart/Styles/Funnel/3D/Commands/XTPChart3dFunnelSeriesLabelsViewDeviceCommand.h"

#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesLabel.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesStyle.h"
#include "Chart/Styles/Funnel/3D/XTPChart3dFunnelSeriesPointView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dFunnelSeriesLabel

IMPLEMENT_SERIAL(CXTPChart3dFunnelSeriesLabel, CXTPChartFunnelSeriesLabelBase,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChart3dFunnelSeriesLabel::CXTPChart3dFunnelSeriesLabel()
{
	Set3D();
}

CXTPChartElementView* CXTPChart3dFunnelSeriesLabel::CreateView(CXTPChartDeviceContext* pDC,
															   CXTPChartSeriesPointView* pPointView,
															   CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChart3dFunnelSeriesLabelView(this, pPointView, pParentView);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dFunnelSeriesLabelView

IMPLEMENT_DYNAMIC(CXTPChart3dFunnelSeriesLabelView, CXTPChartFunnelSeriesLabelViewBase);

CXTPChart3dFunnelSeriesLabelView::CXTPChart3dFunnelSeriesLabelView(
	CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView,
	CXTPChartElementView* pParentView)
	: CXTPChartFunnelSeriesLabelViewBase(pLabel, pPointView, pParentView)
	, m_ptStart(NULL)
	, m_ptBend(NULL)
	, m_ptEnd(NULL)
	, m_ptLabelAnchor(INT_MIN, INT_MIN)
	, m_ptLabelAnchorOrigin(INT_MIN, INT_MIN)
	, m_bLeftDirected(FALSE)
	, m_bConnectorVisible(TRUE)
{
	m_p3dPointView = DYNAMIC_DOWNCAST(CXTPChart3dFunnelSeriesPointView, pPointView);
	ASSERT(NULL != m_p3dPointView);
}

CXTPChart3dFunnelSeriesLabelView::~CXTPChart3dFunnelSeriesLabelView()
{
	SAFE_DELETE(m_ptStart);
	SAFE_DELETE(m_ptBend);
	SAFE_DELETE(m_ptEnd);
}

CXTPChartDeviceCommand*
	CXTPChart3dFunnelSeriesLabelView::CreateTranspositionDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChart3dFunnelSeriesStyle* pStyle =
		DYNAMIC_DOWNCAST(CXTPChart3dFunnelSeriesStyle, m_p3dPointView->GetSeriesView()->GetStyle());
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
	CXTPChart3dFunnelSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = CreateTranspositionDeviceCommand(pDC);
	ASSERT(NULL != pCommand);
	if (NULL != pCommand)
	{
		pCommand->AddChildCommand(new CXTPChart3dFunnelSeriesLabelDeviceCommand(this));
	}

	return pCommand;
}

CXTPChartDeviceCommand*
	CXTPChart3dFunnelSeriesLabelView::CreateConnectorDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	// No special command for connector
	return NULL;
}

void CXTPChart3dFunnelSeriesLabelView::SetConnectorStartPoint(const CXTPPoint3d& point)
{
	SAFE_DELETE(m_ptStart);
	m_ptStart = new CXTPPoint3d(point);
}

void CXTPChart3dFunnelSeriesLabelView::SetConnectorBendPoint(const CXTPPoint3d& point)
{
	SAFE_DELETE(m_ptBend);
	m_ptBend = new CXTPPoint3d(point);
}

void CXTPChart3dFunnelSeriesLabelView::SetConnectorEndPoint(const CXTPPoint3d& point)
{
	SAFE_DELETE(m_ptEnd);
	m_ptEnd = new CXTPPoint3d(point);
}

const CPoint* CXTPChart3dFunnelSeriesLabelView::GetLabelAnchorPoint() const
{
	return INT_MIN != m_ptLabelAnchor.x || INT_MIN != m_ptLabelAnchor.y ? &m_ptLabelAnchor : NULL;
}

const CPoint* CXTPChart3dFunnelSeriesLabelView::GetLabelAnchorOrigin() const
{
	return INT_MIN != m_ptLabelAnchorOrigin.x || INT_MIN != m_ptLabelAnchorOrigin.y
			   ? &m_ptLabelAnchorOrigin
			   : NULL;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dFunnelSeriesLabelsView

IMPLEMENT_DYNAMIC(CXTPChart3dFunnelSeriesLabelsView, CXTPChartElementView);

CXTPChart3dFunnelSeriesLabelsView::CXTPChart3dFunnelSeriesLabelsView(
	CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
}

CXTPChartDeviceCommand*
	CXTPChart3dFunnelSeriesLabelsView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pAggCommand = new CXTPChart3dFunnelSeriesLabelsViewDeviceCommand(this);

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartFunnelSeriesLabelViewBase* pChildView =
			static_cast<CXTPChartFunnelSeriesLabelViewBase*>(m_arrChildren[i]);
		pAggCommand->AddChildCommand(pChildView->CreateDeviceCommand(pDC));
	}

	return pAggCommand;
}

void CXTPChart3dFunnelSeriesLabelsView::ResolveOverlapping()
{
	// do nothing
}

CXTPChart3dFunnelSeriesStyle* CXTPChart3dFunnelSeriesLabelsView::GetStyle() const
{
	CXTPChart3dFunnelSeriesStyle* pStyle = NULL;

	for (int i = 0; i < m_arrChildren.GetSize() && NULL != pStyle; i++)
	{
		CXTPChartFunnelSeriesLabelViewBase* pChildView =
			static_cast<CXTPChartFunnelSeriesLabelViewBase*>(m_arrChildren[i]);
		pStyle = DYNAMIC_DOWNCAST(CXTPChart3dFunnelSeriesStyle,
								  pChildView->GetPointView()->GetSeriesView()->GetStyle());
		ASSERT(NULL != pStyle);
	}

	return pStyle;
}
