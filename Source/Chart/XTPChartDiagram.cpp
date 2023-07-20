// XTPChartDiagram.cpp
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

#include "Chart/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPTypeId.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/Math/XTPMatrix.h"
#include "Common/Math/XTPArcBall.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartDiagram.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/XTPChartSeriesPointView.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartTitle.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram

IMPLEMENT_DYNAMIC(CXTPChartDiagram, CXTPChartPanel)

CXTPChartDiagram::CXTPChartDiagram()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPChartDiagram::Release()
{
	m_pOwner = NULL;

	for (int i = (int)m_arrSeries.GetSize() - 1; i >= 0; i--)
	{
		m_arrSeries[i]->SetDiagram(NULL);
	}

	CXTPChartPanel::Release();
}

CXTPChartDiagramView* CXTPChartDiagram::CreateView(CXTPChartDeviceContext* pDC,
												   CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pParent);
	return NULL;
}

void CXTPChartDiagram::CalculateSeriesLayout(CXTPChartDeviceContext* pDC,
											 CXTPChartDiagramView* pView)
{
	CXTPChartElementView* pSeriesView = pView->GetSeriesView();

	for (int i = 0; i < pSeriesView->GetCount(); i++)
	{
		CXTPChartSeriesView* pSubView = (CXTPChartSeriesView*)pSeriesView->GetAt(i);
		pSubView->CalculateLayout(pDC);
	}
}

BOOL CXTPChartDiagram::OnSeriesAdded(CXTPChartSeries* pSeries)
{
	m_arrSeries.Add(pSeries);
	return TRUE;
}

void CXTPChartDiagram::OnSeriesRemoved(CXTPChartSeries* pSeries)
{
	for (int i = 0; i < m_arrSeries.GetSize(); i++)
	{
		if (m_arrSeries.GetAt(i) == pSeries)
		{
			m_arrSeries.RemoveAt(i);
			break;
		}
	}

	if (m_arrSeries.GetSize() == 0)
	{
		Remove();
	}
}

void CXTPChartDiagram::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPanel::DoPropExchange(pPX);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2D

IMPLEMENT_DYNAMIC(CXTPChart2dDiagram, CXTPChartDiagram);

CXTPChart2dDiagram::CXTPChart2dDiagram()
{
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dDiagram

IMPLEMENT_DYNAMIC(CXTPChart3dDiagram, CXTPChartDiagram);

const float CXTPChart3dDiagram::m_fChartBoxSize		= 100.f;
const int CXTPChart3dDiagram::m_nPrimitiveSize		= 5;
const float CXTPChart3dDiagram::m_fMinModelDistance = m_fChartBoxSize;
const float CXTPChart3dDiagram::m_fMaxModelDistance = m_fChartBoxSize * 8;

CXTPChart3dDiagram::CXTPChart3dDiagram()
	: m_ptCameraPosition(0, 0, m_fChartBoxSize * 2)
	, m_pArcBall(new CXTPArcBall())
	, m_pMatrixBeforeRotation(NULL)
	, m_pMatrixAfterRotation(new CXTPMatrix<double>(4, 4))
{
	m_pMatrixAfterRotation->SetIdentity();
}

CXTPChart3dDiagram::~CXTPChart3dDiagram()
{
	SAFE_DELETE(m_pArcBall);
	SAFE_DELETE(m_pMatrixBeforeRotation);
	SAFE_DELETE(m_pMatrixAfterRotation);
}

void CXTPChart3dDiagram::SetArcBallBounds(double dWidth, double dHeight)
{
	m_pArcBall->SetBounds(dWidth, dHeight);
}

BOOL CXTPChart3dDiagram::SetStartArcBallRotationPoint(POINT point)
{
	BOOL bPointSet = FALSE;

	if (m_pArcBall->HasBounds())
	{
		SAFE_DELETE(m_pMatrixBeforeRotation);
		m_pMatrixBeforeRotation = m_pMatrixAfterRotation->Clone();

		m_pArcBall->SetBegin(point.x, point.y);

		bPointSet = TRUE;
	}

	return bPointSet;
}

BOOL CXTPChart3dDiagram::RotateArcBallToPoint(POINT point)
{
	BOOL bRotated = FALSE;

	if (m_pArcBall->HasBounds() && NULL != m_pMatrixBeforeRotation)
	{
		m_pArcBall->SetEnd(point.x, point.y);

		double xQuat, yQuat, zQuat, wQuat;
		m_pArcBall->GetRotationQuaternion(xQuat, yQuat, zQuat, wQuat);
		m_pMatrixAfterRotation->Rotate(xQuat, yQuat, zQuat, wQuat);

		CXTPMatrix<double>* pNewMatrix = m_pMatrixAfterRotation->Multiply(*m_pMatrixBeforeRotation);
		SAFE_DELETE(m_pMatrixAfterRotation);
		m_pMatrixAfterRotation = pNewMatrix;

		bRotated = TRUE;
	}

	return bRotated;
}

void CXTPChart3dDiagram::ZoomArcBall(float fDelta)
{
	m_ptCameraPosition.Z += fDelta;

	if (m_ptCameraPosition.Z < m_fMinModelDistance)
	{
		m_ptCameraPosition.Z = m_fMinModelDistance;
	}
	else if (m_fMaxModelDistance < m_ptCameraPosition.Z)
	{
		m_ptCameraPosition.Z = m_fMaxModelDistance;
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartDiagram, CXTPChartPanel)
	DISP_FUNCTION_ID(CXTPChartDiagram, "Titles", 200, OleGetTitles, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartDiagram, CXTPChartPanel)
	INTERFACE_PART(CXTPChartDiagram, XTPDIID_ChartDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartDiagram, XTPDIID_ChartDiagram)

LPDISPATCH CXTPChartDiagram::OleGetTitles()
{
	return XTPGetDispatch(GetTitles());
}

#endif
