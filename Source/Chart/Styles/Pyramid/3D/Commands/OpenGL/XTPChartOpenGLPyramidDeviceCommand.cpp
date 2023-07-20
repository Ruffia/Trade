// XTPChartOpenGLPyramidDeviceCommand.cpp
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
#include <float.h>
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

#include "GraphicLibrary/OpenGL/XTPOpenGLListPool.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLTriangleRenderList.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartSeriesLabel.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLDeviceContext.h"

#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesStyleBase.h"
#include "Chart/Styles/Pyramid/Base/XTPChartPyramidSeriesLabelBase.h"
#include "Chart/Styles/Pyramid/3D/Commands/XTPChart3dPyramidDeviceCommandEvents.h"
#include "Chart/Styles/Pyramid/3D/Commands/OpenGL/XTPChartOpenGLPyramidDeviceCommand.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesStyle.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLPyramidDeviceCommand

CXTPChartOpenGLPyramidDeviceCommand::CXTPChartOpenGLPyramidDeviceCommand(
	const CXTPPoint3d& center, double dWidth, double dHeight, double dFrom, double dTo,
	const CXTPChartColor& color, BOOL bSmoothEdges)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyEnabled)
	, m_bPrepared(FALSE)
	, m_color(color)
	, m_pStyle(NULL)
	, m_listId(0)
	, m_dWidth(dWidth)
	, m_dHeight(dHeight)
	, m_dFrom(dFrom)
	, m_dTo(dTo)
	, m_ptCenter(center)
	, m_bSmoothEdges(bSmoothEdges)
{
	Set3D();
}

CXTPChartOpenGLPyramidDeviceCommand::~CXTPChartOpenGLPyramidDeviceCommand()
{
	if (0 < m_listId)
		XTPOpenGLListPool()->ReclaimList(m_listId);
}

CXTPChartElement* CXTPChartOpenGLPyramidDeviceCommand::HitTest(CPoint point,
															   CXTPChartElement* pParent) const
{
	UNREFERENCED_PARAMETER(point);
	UNREFERENCED_PARAMETER(pParent);

	// Pyramid hit testing is not currently possible.
	return NULL;
}

void CXTPChartOpenGLPyramidDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	if (!m_bPrepared)
		Prepare(pDC);

	ASSERT(m_bPrepared);

	CXTPChartOpenGLDeviceContext* pGLDC = DYNAMIC_DOWNCAST(CXTPChartOpenGLDeviceContext, pDC);
	ASSERT(NULL != pGLDC);

	glPushMatrix();

	if (NULL != m_pStyle)
		pGLDC->Rotate(m_pStyle->Get3dRotation());

	if (pGLDC->IsInitialPass())
	{
		pGLDC->GetTriangleRenderList().SetCurrentMatrices();

		float fAlpha = CXTPChartOpenGLDeviceContext::ClampTransparency(m_pStyle->GetTransparency());
		pGLDC->GetTriangleRenderList().SetCurrentColor(m_color.ClampR(), m_color.ClampG(),
													   m_color.ClampB(), fAlpha);

		DrawPyramidSlice(*pGLDC);
	}

	ComputeConnectorPoints(*pGLDC);
	NotifyDynamic(&CXTPChart3dPyramidDeviceCommandEvents::OnSetConnectorPoints,
				  m_ptCenterConnectorPoint, m_ptOuterConnectorPoint);

	CRect rcBounds;
	if (pGLDC->Project(m_3dBox, rcBounds))
		Notify(&CXTPChartDeviceCommandEvents::OnUpdateBounds, rcBounds);

	glPopMatrix();
}

void CXTPChartOpenGLPyramidDeviceCommand::ApplySeriesStyle(CXTPChartSeriesStyle* pStyle)
{
	ASSERT_VALID(pStyle);
	m_pStyle = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesStyle, pStyle);
	ASSERT(NULL != m_pStyle);
}

void CXTPChartOpenGLPyramidDeviceCommand::PrepareOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	ASSERT(!m_bPrepared);
	m_bPrepared = TRUE;
}

void CXTPChartOpenGLPyramidDeviceCommand::DrawTriangle(CXTPChartOpenGLDeviceContext& dc,
													   const CXTPPoint3d& a, const CXTPPoint3d& va,
													   const CXTPPoint3d& b, const CXTPPoint3d& vb,
													   const CXTPPoint3d& c, const CXTPPoint3d& vc)
{
	dc.GetTriangleRenderList().Add(a, va, b, vb, c, vc);

	m_3dBox.Include(a);
	m_3dBox.Include(b);
	m_3dBox.Include(c);

	CXTPPoint3d ptCenter;
	ptCenter.X = (a.X + b.X + c.X) / 3;
	ptCenter.Y = (a.Y + b.Y + c.Y) / 3;
	ptCenter.Z = (a.Z + b.Z + c.Z) / 3;
	m_3dBox.Include(ptCenter, TRUE);
}

void CXTPChartOpenGLPyramidDeviceCommand::DrawPyramidSlice(CXTPChartOpenGLDeviceContext& dc)
{
	int nEdgeCount = m_pStyle->GetBaseEdgeCount();
	ASSERT(0 == nEdgeCount || 3 <= nEdgeCount);

	BOOL bSmoothEdges = m_bSmoothEdges || 0 == nEdgeCount;

	if (0 == nEdgeCount)
	{
		// Make round pyramid
		nEdgeCount = static_cast<int>((m_dWidth * CXTPMathUtils::m_dPI * 2)
									  / CXTPChart3dDiagram::m_nPrimitiveSize);
	}

	m_arMidFacePoints.SetSize(nEdgeCount);

	double dAngleStep	= (CXTPMathUtils::m_dPI * 2) / nEdgeCount;
	double dTopRadius	= m_dWidth * m_dFrom;
	double dBottomRadius = m_dWidth * m_dTo;
	double dTopZ		 = m_dHeight * m_dFrom - (m_dHeight / 3) * 2;
	double dBottomZ		 = m_dHeight * m_dTo - (m_dHeight / 3) * 2;
	double dRadDiff		 = (dBottomRadius - dTopRadius) / 2;
	double dFaceLength   = sqrt(m_dHeight * m_dHeight + dRadDiff * dRadDiff);
	double dFaceZNormal  = dRadDiff / dFaceLength;

	CXTPPoint3d ptCenterTop(0, 0, dTopZ);
	CXTPPoint3d ptCenterBottom(0, 0, dBottomZ);

	double dAngle			 = 0;
	CXTPPoint3d ptCurrTop	= ptCenterTop, ptPrevTop;
	CXTPPoint3d ptCurrBottom = ptCenterBottom, ptPrevBottom;
	CXTPPoint3d vCurr, vPrev, vCenterTop(0., 0., -1.), vCenterBottom(0., 0., 1.);
	vCurr.Z = -dFaceZNormal;
	for (int i = 0; i <= nEdgeCount; ++i)
	{
		vCurr.X = cos(dAngle);
		vCurr.Y = sin(dAngle);
		dAngle += dAngleStep;

		ptCurrTop.X	= vCurr.X * dTopRadius;
		ptCurrTop.Y	= vCurr.Y * dTopRadius;
		ptCurrBottom.X = vCurr.X * dBottomRadius;
		ptCurrBottom.Y = vCurr.Y * dBottomRadius;

		if (0 == i)
		{
			ptPrevTop	= ptCurrTop;
			ptPrevBottom = ptCurrBottom;
			vPrev		 = vCurr;
			continue;
		}

		m_arMidFacePoints[i - 1].SetPoint(
			(ptCurrTop.X + ptCurrBottom.X + ptPrevTop.X + ptPrevBottom.X) / 4,
			(ptCurrTop.Y + ptCurrBottom.Y + ptPrevTop.Y + ptPrevBottom.Y) / 4,
			(ptCurrTop.Z + ptCurrBottom.Z + ptPrevTop.Z + ptPrevBottom.Z) / 4);

		if (bSmoothEdges)
		{
			// Top face
			DrawTriangle(dc, ptCenterTop, vCenterTop, ptPrevTop, vCenterTop, ptCurrTop, vCenterTop);
			// Bottom face
			DrawTriangle(dc, ptCenterBottom, vCenterBottom, ptCurrBottom, vCenterBottom,
						 ptPrevBottom, vCenterBottom);

			// Side face
			DrawTriangle(dc, ptCurrBottom, vCurr, ptCurrTop, vCurr, ptPrevBottom, vPrev);
			DrawTriangle(dc, ptCurrTop, vCurr, ptPrevTop, vPrev, ptPrevBottom, vPrev);
		}
		else
		{
			// Top face
			CXTPPoint3d vFace = CXTPOpenGLUtils::ComputeTriangleNormal(ptCenterTop, ptPrevTop,
																	   ptCurrTop);
			DrawTriangle(dc, ptCenterTop, vFace, ptPrevTop, vFace, ptCurrTop, vFace);
			// Bottom face
			vFace = CXTPOpenGLUtils::ComputeTriangleNormal(ptCenterBottom, ptCurrBottom,
														   ptPrevBottom);
			DrawTriangle(dc, ptCenterBottom, vFace, ptCurrBottom, vFace, ptPrevBottom, vFace);

			// Side face
			vFace = CXTPOpenGLUtils::ComputeTriangleNormal(ptCurrBottom, ptCurrTop, ptPrevBottom);
			DrawTriangle(dc, ptCurrBottom, vFace, ptCurrTop, vFace, ptPrevBottom, vFace);
			vFace = CXTPOpenGLUtils::ComputeTriangleNormal(ptCurrTop, ptPrevTop, ptPrevBottom);
			DrawTriangle(dc, ptCurrTop, vFace, ptPrevTop, vFace, ptPrevBottom, vFace);
		}

		ptPrevTop	= ptCurrTop;
		ptPrevBottom = ptCurrBottom;
		vPrev		 = vCurr;
	}
}

void CXTPChartOpenGLPyramidDeviceCommand::ComputeConnectorPoints(CXTPChartOpenGLDeviceContext& dc)
{
	double dMidValue  = (m_dTo + m_dFrom) / 2;
	double dMidRadius = m_dWidth * dMidValue;
	double dMidZ	  = m_dHeight * dMidValue - (m_dHeight / 3) * 2;

	m_ptCenterConnectorPoint.SetPoint(0, 0, dMidZ);
	m_ptOuterConnectorPoint.SetPoint(dMidRadius, 0, dMidZ);

	ASSERT(NULL != m_pStyle);

	XTPChartPyramidLabelPosition labelPosition =
		((CXTPChartPyramidSeriesLabelBase*)m_pStyle->GetLabel())->GetPosition();
	CPoint ptCenterConnectorPoint2d;

	if (dc.Project(m_ptCenterConnectorPoint, ptCenterConnectorPoint2d))
	{
		INT_PTR nBestIndex = INT_MIN;
		int nBestDist	  = (xtpChartPyramidLabelCenter == labelPosition ? INT_MAX : INT_MIN);
		double dBestDist   = DBL_MAX;
		for (INT_PTR i = 0; i < m_arMidFacePoints.GetSize(); ++i)
		{
			CPoint pt;
			double dMidFacePointZ = 0;
			if (dc.Project(m_arMidFacePoints[i], pt, &dMidFacePointZ))
			{
				if (xtpChartPyramidLabelCenter == labelPosition)
				{
					if (dMidFacePointZ < dBestDist)
					{
						dBestDist  = dMidFacePointZ;
						nBestIndex = i;
					}
				}
				else
				{
					int nDist = (xtpChartPyramidLabelLeft == labelPosition
									 ? (ptCenterConnectorPoint2d.x - pt.x)
									 : (pt.x - ptCenterConnectorPoint2d.x));
					if (nBestDist < nDist)
					{
						nBestDist  = nDist;
						nBestIndex = i;
					}
				}
			}
		}

		if (INT_MIN != nBestIndex)
		{
			m_ptOuterConnectorPoint = m_arMidFacePoints[nBestIndex];
		}
	}
}
