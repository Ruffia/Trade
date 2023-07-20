// XTPChartOpenGLPieDeviceCommand.cpp
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

#include "GraphicLibrary/OpenGL/XTPOpenGLListPool.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLUtils.h"
#include "GraphicLibrary/OpenGL/XTPOpenGLTriangleRenderList.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"
#include "Chart/XTPChartPanel.h"
#include "Chart/XTPChartDiagram.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartPieDeviceCommand.h"
#include "Chart/Drawing/OpenGL/XTPChartOpenGLDeviceContext.h"

#include "Chart/Types/XTPChartPie.h"

#include "Chart/Styles/Pie/3D/Commands/XTPChart3dPieDeviceCommandEvents.h"
#include "Chart/Styles/Pie/3D/Commands/OpenGL/XTPChartOpenGLPieDeviceCommand.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieSeriesStyle.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartOpenGLPieDeviceCommand

CXTPChartOpenGLPieDeviceCommand::CXTPChartOpenGLPieDeviceCommand(
	const CXTPPoint3d& center, double radius, double dStartAngle, double dSweepAngle, double depth,
	double holePercent, const CXTPChartRectF& gradientBounds, const CXTPChartColor& color,
	const CXTPChartColor& color2, BOOL bStartGap /*= FALSE*/, BOOL bEndGap /*= FALSE*/)
	: CXTPChart3dPieDeviceCommand(xtpChart3dAntialiasingPolicyEnabled, center, radius, radius,
								  dStartAngle, dSweepAngle, depth, holePercent)
	, m_rcGradientBounds(gradientBounds)
	, m_color(color)
	, m_color2(color2)
	, m_pStyle(NULL)
	, m_listId(0)
	, m_bStartGap(bStartGap)
	, m_bEndGap(bEndGap)
	, m_bPrepared(FALSE)
{
	Set3D();
}

CXTPChartOpenGLPieDeviceCommand::~CXTPChartOpenGLPieDeviceCommand()
{
	if (0 < m_listId)
		XTPOpenGLListPool()->ReclaimList(m_listId);
}

CXTPChartElement* CXTPChartOpenGLPieDeviceCommand::HitTest(CPoint point,
														   CXTPChartElement* pParent) const
{
	UNREFERENCED_PARAMETER(point);
	UNREFERENCED_PARAMETER(pParent);

	// Pie hit testing is not currently possible.
	return NULL;
}

struct CXTPChartOpenGLPieDeviceCommand::BarRingInfo
{
	CXTPPoint3d ptTopOuter, ptTopInner, ptBottomOuter, ptBottomInner;
	CXTPPoint3d vOuterNormal, vInnerNormal;
	double dAngle;

	void Compute(double dAngleRad, double dOuterRadius, double dInnerRadius, double dDepth)
	{
		dAngle		   = dAngleRad;
		vOuterNormal.X = cos(dAngle);
		vOuterNormal.Z = sin(dAngle);

		vInnerNormal.X = -vOuterNormal.X;
		vInnerNormal.Z = -vOuterNormal.Z;

		ptTopOuter.X = dOuterRadius * vOuterNormal.X;
		ptTopOuter.Y = dDepth;
		ptTopOuter.Z = dOuterRadius * vOuterNormal.Z;

		ptTopInner.X = dInnerRadius * vOuterNormal.X;
		ptTopInner.Y = dDepth;
		ptTopInner.Z = dInnerRadius * vOuterNormal.Z;

		ptBottomOuter.X = ptTopOuter.X;
		ptBottomOuter.Y = -dDepth;
		ptBottomOuter.Z = ptTopOuter.Z;

		ptBottomInner.X = ptTopInner.X;
		ptBottomInner.Y = -dDepth;
		ptBottomInner.Z = ptTopInner.Z;
	}
};

struct CXTPChartOpenGLPieDeviceCommand::TorusRingInfo
{
	static const int nVerticeCount;
	CArray<CXTPPoint3d, const CXTPPoint3d&> arVertices;
	CArray<CXTPPoint3d, const CXTPPoint3d&> arNormals;
	CXTPPoint3d ptCenter;

	TorusRingInfo()
	{
		arVertices.SetSize(nVerticeCount);
		arNormals.SetSize(nVerticeCount);
	}

	void Compute(double dAngle, double dTorusRadius, double dRingRadius)
	{
		static const double dStep = (CXTPMathUtils::m_dPI * 2) / nVerticeCount;
		double dRingAngle		  = 0;
		for (int i = 0; i < nVerticeCount; ++i, dRingAngle += dStep)
		{
			CXTPPoint3d& vNormal  = arNormals[i];
			CXTPPoint3d& ptVertex = arVertices[i];

			struct
			{
				double dCos, dSin;
			} yaw = { cos(dAngle), sin(dAngle) }, pitch = { cos(dRingAngle), sin(dRingAngle) };
			vNormal.X = pitch.dCos * yaw.dCos;
			vNormal.Y = pitch.dSin;
			vNormal.Z = pitch.dCos * yaw.dSin;

			ptVertex.X = (pitch.dCos * dRingRadius + dTorusRadius) * yaw.dCos;
			ptVertex.Y = pitch.dSin * dRingRadius;
			ptVertex.Z = (pitch.dCos * dRingRadius + dTorusRadius) * yaw.dSin;
		}

		ptCenter.X = cos(dAngle) * dTorusRadius;
		ptCenter.Y = 0;
		ptCenter.Z = sin(dAngle) * dTorusRadius;
	}
};

const int CXTPChartOpenGLPieDeviceCommand::TorusRingInfo::nVerticeCount =
	(360 / CXTPChart3dDiagram::m_nPrimitiveSize);

void CXTPChartOpenGLPieDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	if (!m_bPrepared)
		Prepare(pDC);

	ASSERT(m_bPrepared);

	CXTPChartOpenGLDeviceContext* pGLDC = DYNAMIC_DOWNCAST(CXTPChartOpenGLDeviceContext, pDC);
	ASSERT(NULL != pGLDC);

	glPushMatrix();

	if (NULL != m_pStyle)
		pGLDC->Rotate(m_pStyle->Get3dRotation());

	if (1 <= fabs(m_dSweepAngle))
	{
		double dFromAngle = fabs(m_dStartAngle),
			   dToAngle   = fabs(m_dStartAngle) + fabs(m_dSweepAngle);

		if (pGLDC->IsInitialPass())
		{
			CXTPPoint3d translation(m_ptCenter.X, 0, -m_ptCenter.Y);
			glTranslated(translation.X, translation.Y, translation.Z);

			pGLDC->GetTriangleRenderList().SetCurrentMatrices();

			float fAlpha = CXTPChartOpenGLDeviceContext::ClampTransparency(
				m_pStyle->GetTransparency());
			pGLDC->GetTriangleRenderList().SetCurrentColor(m_color.ClampR(), m_color.ClampG(),
														   m_color.ClampB(), fAlpha);

			glShadeModel(GLenum(m_pStyle->Is3dEdgeSmoothingEnabled() ? GL_SMOOTH : GL_FLAT));

			if (m_pStyle->IsTorus())
				DrawTorusPie(*pGLDC, dFromAngle, dToAngle);
			else
				DrawBarPie(*pGLDC, dFromAngle, dToAngle);
		}

		ComputeConnectorPoints(dFromAngle, dToAngle);
		NotifyDynamic(&CXTPChart3dPieDeviceCommandEvents::OnSetConnectorPoints,
					  m_ptCenterConnectorPoint, m_ptOuterConnectorPoint);
	}

	CRect rcBounds;
	if (pGLDC->Project(m_3dBox, rcBounds))
		Notify(&CXTPChartDeviceCommandEvents::OnUpdateBounds, rcBounds);

	glPopMatrix();
}

void CXTPChartOpenGLPieDeviceCommand::ApplySeriesStyle(CXTPChartSeriesStyle* pStyle)
{
	ASSERT_VALID(pStyle);
	m_pStyle = DYNAMIC_DOWNCAST(CXTPChart3dPieSeriesStyle, pStyle);
	ASSERT(NULL != m_pStyle);
}

void CXTPChartOpenGLPieDeviceCommand::PrepareOverride(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	ASSERT(!m_bPrepared);
	m_bPrepared = TRUE;
}

CXTPPoint3d CXTPChartOpenGLPieDeviceCommand::GetBarPieSliceCenter(int n, ...) const
{
	ASSERT(3 <= n); // at least 3 vertices required

	va_list vl;
	va_start(vl, n);

	CXTPPoint3d ptCenter(0, 0, 0);
	for (int i = 0; i < n; ++i)
	{
		const CXTPPoint3d* pv = va_arg(vl, const CXTPPoint3d*);
		ASSERT(NULL != pv);
		ptCenter.X += pv->X;
		ptCenter.Y += pv->Y;
		ptCenter.Z += pv->Z;
	}

	ptCenter.X /= n;
	ptCenter.Y /= n;
	ptCenter.Z /= n;

	va_end(vl);

	return ptCenter;
}

void CXTPChartOpenGLPieDeviceCommand::DrawBarPie(CXTPChartOpenGLDeviceContext& dc,
												 double dFromAngle, double dToAngle)
{
	double dAngle = dFromAngle;
	CXTPPoint3d vFaceNormal;

	BarRingInfo ringFrom, ringTo;
	BarRingInfo* pRingFrom = &ringFrom;
	BarRingInfo* pRingTo   = &ringTo;

	// Render a closed ring for each sub-arc.
	bool bKeepDrawing = true, bFirstIteration = true;
	while (true)
	{
		if (bFirstIteration)
		{
			// Compute and draw the first side
			pRingFrom->Compute(CXTPMathUtils::Degree2Radian(dAngle), m_dRadius, m_dInnerRadius,
							   m_dCorrectedDepth);
			DrawBarPie_OpenShape(dc, *pRingFrom);
			bFirstIteration = false;
		}

		// Compute subsequent ring
		dAngle += CXTPChart3dDiagram::m_nPrimitiveSize;
		if (dToAngle < dAngle)
		{
			dAngle = dToAngle;

			// Last ring is computed, no more rings left
			bKeepDrawing = false;
		}

		pRingTo->Compute(CXTPMathUtils::Degree2Radian(dAngle), m_dRadius, m_dInnerRadius,
						 m_dCorrectedDepth);
		DrawBarPie_Shape(dc, *pRingFrom, *pRingTo);

		if (bKeepDrawing)
		{
			// Swap ring infos for the next iteration
			BarRingInfo* pSideFromOld = pRingFrom;
			pRingFrom				  = pRingTo;
			pRingTo					  = pSideFromOld;
		}
		else
		{
			if (m_bEndGap)
			{
				// Second ring face
				DrawBarPie_CloseShape(dc, *pRingTo);
			}

			break;
		}
	}
}

void CXTPChartOpenGLPieDeviceCommand::DrawBarPie_OpenShape(CXTPChartOpenGLDeviceContext& dc,
														   const BarRingInfo& ring)
{
	CXTPPoint3d vFaceNormal = CXTPOpenGLUtils::ComputeTriangleNormal(ring.ptTopOuter,
																	 ring.ptTopInner,
																	 ring.ptBottomInner);

	dc.GetTriangleRenderList().Add(ring.ptTopOuter, vFaceNormal, ring.ptTopInner, vFaceNormal,
								   ring.ptBottomInner, vFaceNormal);
	dc.GetTriangleRenderList().Add(ring.ptBottomInner, vFaceNormal, ring.ptBottomOuter, vFaceNormal,
								   ring.ptTopOuter, vFaceNormal);

	m_3dBox.Include(ring.ptTopOuter);
	m_3dBox.Include(ring.ptTopInner);
	m_3dBox.Include(ring.ptBottomInner);
	m_3dBox.Include(ring.ptBottomInner);
	m_3dBox.Include(ring.ptBottomOuter);
	m_3dBox.Include(ring.ptTopOuter);

	CXTPPoint3d ptFaceCenter = GetBarPieSliceCenter(4, &ring.ptTopOuter, &ring.ptTopInner,
													&ring.ptBottomInner, &ring.ptBottomOuter);
	m_3dBox.Include(ptFaceCenter, TRUE);
}

void CXTPChartOpenGLPieDeviceCommand::DrawBarPie_Shape(CXTPChartOpenGLDeviceContext& dc,
													   const BarRingInfo& ringFrom,
													   const BarRingInfo& ringTo)
{
	static const CXTPPoint3d vTop(0, 1, 0), vBottom(0, -1, 0);

	// Outer face
	dc.GetTriangleRenderList().Add(ringTo.ptTopOuter, ringTo.vOuterNormal, ringFrom.ptTopOuter,
								   ringFrom.vOuterNormal, ringFrom.ptBottomOuter,
								   ringFrom.vOuterNormal);
	dc.GetTriangleRenderList().Add(ringFrom.ptBottomOuter, ringFrom.vOuterNormal,
								   ringTo.ptBottomOuter, ringTo.vOuterNormal, ringTo.ptTopOuter,
								   ringTo.vOuterNormal);
	m_3dBox.Include(ringTo.ptTopOuter);
	m_3dBox.Include(ringFrom.ptTopOuter);
	m_3dBox.Include(ringFrom.ptBottomOuter);
	m_3dBox.Include(ringFrom.ptBottomOuter);
	m_3dBox.Include(ringTo.ptBottomOuter);
	m_3dBox.Include(ringTo.ptTopOuter);

	// Inner face
	dc.GetTriangleRenderList().Add(ringFrom.ptTopInner, ringFrom.vInnerNormal, ringTo.ptTopInner,
								   ringTo.vInnerNormal, ringTo.ptBottomInner, ringTo.vInnerNormal);
	dc.GetTriangleRenderList().Add(ringTo.ptBottomInner, ringTo.vInnerNormal,
								   ringFrom.ptBottomInner, ringFrom.vInnerNormal,
								   ringFrom.ptTopInner, ringFrom.vInnerNormal);

	m_3dBox.Include(ringFrom.ptTopInner);
	m_3dBox.Include(ringTo.ptTopInner);
	m_3dBox.Include(ringTo.ptBottomInner);
	m_3dBox.Include(ringTo.ptBottomInner);
	m_3dBox.Include(ringFrom.ptBottomInner);
	m_3dBox.Include(ringFrom.ptTopInner);

	// Top face
	dc.GetTriangleRenderList().Add(ringTo.ptTopInner, vTop, ringFrom.ptTopInner, vTop,
								   ringFrom.ptTopOuter, vTop);
	dc.GetTriangleRenderList().Add(ringFrom.ptTopOuter, vTop, ringTo.ptTopOuter, vTop,
								   ringTo.ptTopInner, vTop);
	m_3dBox.Include(ringTo.ptTopInner);
	m_3dBox.Include(ringFrom.ptTopInner);
	m_3dBox.Include(ringFrom.ptTopOuter);
	m_3dBox.Include(ringFrom.ptTopOuter);
	m_3dBox.Include(ringTo.ptTopOuter);
	m_3dBox.Include(ringTo.ptTopInner);

	// Bottom face
	dc.GetTriangleRenderList().Add(ringTo.ptBottomInner, vBottom, ringFrom.ptBottomOuter, vBottom,
								   ringFrom.ptBottomInner, vBottom);
	dc.GetTriangleRenderList().Add(ringFrom.ptBottomOuter, vBottom, ringTo.ptBottomInner, vBottom,
								   ringTo.ptBottomOuter, vBottom);
	m_3dBox.Include(ringTo.ptBottomInner);
	m_3dBox.Include(ringFrom.ptBottomInner);
	m_3dBox.Include(ringFrom.ptBottomOuter);
	m_3dBox.Include(ringFrom.ptBottomOuter);
	m_3dBox.Include(ringTo.ptBottomOuter);
	m_3dBox.Include(ringTo.ptBottomInner);

	CXTPPoint3d ptFaceCenter = GetBarPieSliceCenter(4, &ringTo.ptTopInner, &ringTo.ptTopOuter,
													&ringTo.ptBottomOuter, &ringTo.ptBottomInner);
	m_3dBox.Include(ptFaceCenter, TRUE);
}

void CXTPChartOpenGLPieDeviceCommand::DrawBarPie_CloseShape(CXTPChartOpenGLDeviceContext& dc,
															const BarRingInfo& ring)
{
	CXTPPoint3d vFaceNormal = CXTPOpenGLUtils::ComputeTriangleNormal(ring.ptTopOuter,
																	 ring.ptTopInner,
																	 ring.ptBottomInner);

	dc.GetTriangleRenderList().Add(ring.ptTopInner, vFaceNormal, ring.ptTopOuter, vFaceNormal,
								   ring.ptBottomOuter, vFaceNormal);
	dc.GetTriangleRenderList().Add(ring.ptBottomOuter, vFaceNormal, ring.ptBottomInner, vFaceNormal,
								   ring.ptTopInner, vFaceNormal);
	m_3dBox.Include(ring.ptTopInner);
	m_3dBox.Include(ring.ptTopOuter);
	m_3dBox.Include(ring.ptBottomOuter);
	m_3dBox.Include(ring.ptBottomOuter);
	m_3dBox.Include(ring.ptBottomInner);
	m_3dBox.Include(ring.ptTopInner);

	CXTPPoint3d ptFaceCenter = GetBarPieSliceCenter(4, &ring.ptTopInner, &ring.ptTopOuter,
													&ring.ptBottomOuter, &ring.ptBottomInner);
	m_3dBox.Include(ptFaceCenter, TRUE);
}

void CXTPChartOpenGLPieDeviceCommand::DrawTorusPie(CXTPChartOpenGLDeviceContext& dc,
												   double dFromAngle, double dToAngle)
{
	double dAngle = dFromAngle;

	CXTPPoint3d vFaceNormal;
	TorusRingInfo ringFrom, ringTo;
	TorusRingInfo* pRingFrom = &ringFrom;
	TorusRingInfo* pRingTo   = &ringTo;

	// Render a closed ring for each sub-arc.

	double dTorusRadius		= m_dRadius - m_dDepth / 2;
	double dTorusRingRadius = m_dDepth / 2;
	bool bKeepDrawing = true, bFirstIteration = true;
	while (true)
	{
		if (bFirstIteration)
		{
			// Compute the first side
			pRingFrom->Compute(CXTPMathUtils::Degree2Radian(dAngle), dTorusRadius,
							   dTorusRingRadius);
			DrawTorusPie_OpenShape(dc, *pRingFrom);

			bFirstIteration = false;
		}

		// Compute subsequent side
		dAngle += CXTPChart3dDiagram::m_nPrimitiveSize;
		if (dToAngle < dAngle)
		{
			dAngle = dToAngle;

			// Last side is computed, no more sides left
			bKeepDrawing = false;
		}

		pRingTo->Compute(CXTPMathUtils::Degree2Radian(dAngle), dTorusRadius, dTorusRingRadius);

		DrawTorusPie_Shape(dc, *pRingFrom, *pRingTo);

		if (bKeepDrawing)
		{
			// Swap side infos for the next iteration
			TorusRingInfo* pSideFromOld = pRingFrom;
			pRingFrom					= pRingTo;
			pRingTo						= pSideFromOld;
		}
		else
		{
			// Second side face
			DrawTorusPie_CloseShape(dc, *pRingTo);
			break;
		}
	}
}

void CXTPChartOpenGLPieDeviceCommand::DrawTorusPie_OpenShape(CXTPChartOpenGLDeviceContext& dc,
															 const TorusRingInfo& ring)
{
	CXTPPoint3d vFaceNormal = CXTPOpenGLUtils::ComputeTriangleNormal(ring.ptCenter,
																	 ring.arVertices[0],
																	 ring.arVertices[1]);

	m_3dBox.Include(ring.ptCenter, TRUE);
	for (int i = 1; i < TorusRingInfo::nVerticeCount; ++i)
	{
		dc.GetTriangleRenderList().Add(ring.ptCenter, vFaceNormal, ring.arVertices[i - 1],
									   vFaceNormal, ring.arVertices[i], vFaceNormal);
		m_3dBox.Include(ring.arVertices[i]);
	}

	dc.GetTriangleRenderList().Add(ring.ptCenter, vFaceNormal,
								   ring.arVertices[TorusRingInfo::nVerticeCount - 1], vFaceNormal,
								   ring.arVertices[0], vFaceNormal);
	m_3dBox.Include(ring.arVertices[0]);
}

void CXTPChartOpenGLPieDeviceCommand::DrawTorusPie_Shape(CXTPChartOpenGLDeviceContext& dc,
														 const TorusRingInfo& ringFrom,
														 const TorusRingInfo& ringTo)
{
	m_3dBox.Include(ringTo.ptCenter, TRUE);

	for (int i = 1; i <= TorusRingInfo::nVerticeCount; ++i)
	{
		int nFrom = (i < TorusRingInfo::nVerticeCount ? i : 0);
		int nTo   = i - 1;

		dc.GetTriangleRenderList().Add(ringTo.arVertices[nTo], ringTo.arNormals[nTo],
									   ringFrom.arVertices[nFrom], ringFrom.arNormals[nFrom],
									   ringFrom.arVertices[nTo], ringFrom.arNormals[nTo]);
		dc.GetTriangleRenderList().Add(ringFrom.arVertices[nFrom], ringFrom.arNormals[nFrom],
									   ringTo.arVertices[nTo], ringTo.arNormals[nTo],
									   ringTo.arVertices[nFrom], ringTo.arNormals[nFrom]);

		m_3dBox.Include(ringFrom.arVertices[nFrom]);
		m_3dBox.Include(ringFrom.arVertices[nTo]);
		m_3dBox.Include(ringTo.arVertices[nTo]);
		m_3dBox.Include(ringTo.arVertices[nTo]);
		m_3dBox.Include(ringTo.arVertices[nFrom]);
		m_3dBox.Include(ringFrom.arVertices[nFrom]);
	}
}

void CXTPChartOpenGLPieDeviceCommand::DrawTorusPie_CloseShape(CXTPChartOpenGLDeviceContext& dc,
															  const TorusRingInfo& ring)
{
	CXTPPoint3d vFaceNormal = CXTPOpenGLUtils::ComputeTriangleNormal(ring.ptCenter,
																	 ring.arVertices[1],
																	 ring.arVertices[0]);

	m_3dBox.Include(ring.ptCenter, TRUE);
	for (int i = TorusRingInfo::nVerticeCount; 1 < i;)
	{
		--i;
		dc.GetTriangleRenderList().Add(ring.ptCenter, vFaceNormal, ring.arVertices[i], vFaceNormal,
									   ring.arVertices[i - 1], vFaceNormal);
		m_3dBox.Include(ring.arVertices[i]);
	}

	dc.GetTriangleRenderList().Add(ring.ptCenter, vFaceNormal, ring.arVertices[0], vFaceNormal,
								   ring.arVertices[TorusRingInfo::nVerticeCount - 1], vFaceNormal);
	m_3dBox.Include(ring.arVertices[TorusRingInfo::nVerticeCount - 1]);
}

void CXTPChartOpenGLPieDeviceCommand::ComputeConnectorPoints(double dFromAngle, double dToAngle)
{
	ASSERT(NULL != m_pStyle);

	double dCenterRadius = m_pStyle->IsTorus()
							   ? m_dRadius - m_dDepth / 2
							   : (0 != m_dInnerRadius ? (m_dRadius + m_dInnerRadius) / 2
													  : m_dRadius - m_dRadius * 0.33);
	double dCenterAngle = (dFromAngle + dToAngle) / 2;

	CXTPPoint3d v(cos(CXTPMathUtils::Degree2Radian(dCenterAngle)), 0,
				  sin(CXTPMathUtils::Degree2Radian(dCenterAngle)));
	m_ptCenterConnectorPoint.SetPoint(v.X * dCenterRadius, 0, v.Z * dCenterRadius);

	m_ptOuterConnectorPoint.SetPoint(v.X * m_dRadius, 0, v.Z * m_dRadius);
}
