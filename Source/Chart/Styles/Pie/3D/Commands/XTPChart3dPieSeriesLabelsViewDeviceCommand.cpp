// XTPChart3dPieSeriesLabelsViewDeviceCommand.cpp
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPXMLHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
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
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPDrawHelpers.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/Types/XTPChartPie.h"
#include "Chart/Types/3D/XTPChart3dPie.h"

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
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"
#include "Chart/Drawing/XTPChartLineDeviceCommand.h"
#include "Chart/Drawing/XTPChartPolygonDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Chart/Styles/Pie/Base/XTPChartPieDiagramBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesLabelBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesStyleBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesPointViewBase.h"
#include "Chart/Styles/Pie/Base/XTPChartPieSeriesViewBase.h"

#include "Chart/Styles/Pie/3D/Commands/XTPChart3dPieDeviceCommandEvents.h"
#include "Chart/Styles/Pie/3D/Commands/XTPChart3dPieSeriesLabelsViewDeviceCommand.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieSeriesPointView.h"
#include "Chart/Styles/Pie/3D/XTPChart3dPieSeriesLabel.h"

#define XTP_CHART_PIE3D_OUTER_CONNECTOR_LENGTH_FACTOR 0.15
#define XTP_CHART_PIE3D_OUTER_CONNECTOR_THICKNESS 1
#define XTP_CHART_PIE3D_BENT_CONNECTOR_ARM_LENGTH XTP_DPI_X(10)

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPieSeriesLabelDeviceCommand

IMPLEMENT_DYNAMIC(CXTPChart3dPieSeriesLabelDeviceCommand, CXTPChart3dDeviceCommand);

CXTPChart3dPieSeriesLabelDeviceCommand::CXTPChart3dPieSeriesLabelDeviceCommand(
	CXTPChart3dPieSeriesLabelView* pSeriesLabelView)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDisabled)
	, m_pSeriesLabelView(pSeriesLabelView)
	, m_bFailed(FALSE)
	, m_dConnectorLengthFactor(XTP_CHART_PIE3D_OUTER_CONNECTOR_LENGTH_FACTOR)
	, m_dConnectorEndpointVerticalOffset(0.)
{
	ASSERT(NULL != m_pSeriesLabelView);

	m_pPointView = DYNAMIC_DOWNCAST(CXTPChart3dPieSeriesPointView,
									m_pSeriesLabelView->GetPointView());
	ASSERT(NULL != m_pPointView);
}

void CXTPChart3dPieSeriesLabelDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	if (!IsCommitted() && !IsFailed())
	{
		const CXTPPoint3d* ptStart = m_pSeriesLabelView->GetConnectorStartPoint();
		const CXTPPoint3d* ptEnd   = m_pSeriesLabelView->GetConnectorEndPoint();
		if (NULL != ptStart && NULL != ptEnd)
		{
			CXTPChartSeriesLabel* pLabel = m_pSeriesLabelView->GetLabel();
			ASSERT(NULL != pLabel);

			XTPChartPieLabelPosition position = m_pSeriesLabelView->GetPosition();
			CXTPChartColor clrBorder		  = m_pSeriesLabelView->GetActualBorderColor();
			CXTPChartColor clrConnector		  = m_pSeriesLabelView->GetActualConnectorColor();
			CXTPChartColor clrText			  = m_pSeriesLabelView->GetActualTextColor();
			CXTPChartColor clrBackColor		  = pLabel->GetActualBackColor();
			CXTPChartDeviceCommand* pCommand  = NULL;

			if (pLabel->GetShowLines() && m_pSeriesLabelView->IsConnectorVisible()
				&& xtpChartPieLabelInside != position && xtpChartPieLabelRadial != position)
			{
				if (IsBent())
				{
					const CXTPPoint3d* ptBend = m_pSeriesLabelView->GetConnectorBendPoint();
					if (NULL != ptBend)
					{
						pCommand = pDC->GetDrawingObjectFactory()->CreateSolidLineDeviceCommand(
							*ptStart, *ptBend, clrConnector,
							XTP_CHART_PIE3D_OUTER_CONNECTOR_THICKNESS);
						AddChildCommand(pCommand);

						pCommand = pDC->GetDrawingObjectFactory()->CreateSolidLineDeviceCommand(
							*ptBend, *ptEnd, clrConnector,
							XTP_CHART_PIE3D_OUTER_CONNECTOR_THICKNESS);
						AddChildCommand(pCommand);
					}
				}
				else
				{
					pCommand = pDC->GetDrawingObjectFactory()->CreateSolidLineDeviceCommand(
						*ptStart, *ptEnd, clrConnector, XTP_CHART_PIE3D_OUTER_CONNECTOR_THICKNESS);
					AddChildCommand(pCommand);
				}
			}

			CXTPChartString strText(pLabel->GetPointLabel(m_pPointView->GetPoint()));
			strText.TrimLeft();
			strText.TrimRight();
			AddChildCommand(
				pLabel->GetFillStyle()->CreateDeviceCommand(m_rcLabel, clrBackColor, clrBackColor));

			CXTPChartTextPainter painter(pDC, strText, pLabel);
			CXTPChartSizeF szLabel = painter.GetSize();
			painter.SetLocation(
				CXTPChartPointF(static_cast<int>(m_rcLabel.left), static_cast<int>(m_rcLabel.top)));
			pCommand = painter.CreateDeviceCommand(pDC, clrText);
			AddChildCommand(pCommand);

			int nThickness = pLabel->GetBorder()->GetThickness();
			if (0 < nThickness)
			{
				pCommand = pLabel->GetBorder()->CreateDeviceCommand(m_rcLabel, clrBorder);
				AddChildCommand(pCommand);
			}
		}

		Commit();
	}
}

BOOL CXTPChart3dPieSeriesLabelDeviceCommand::IsBent() const
{
	XTPChartPieLabelPosition position = m_pSeriesLabelView->GetPosition();
	return xtpChartPieLabelTwoColumns == position;
}

void CXTPChart3dPieSeriesLabelDeviceCommand::ComputeLabel(CXTPChart3dDeviceContext* pDC)
{
	ASSERT(!IsFailed());

	CXTPChartSeriesLabel* pLabel = m_pSeriesLabelView->GetLabel();
	ASSERT(NULL != pLabel);

	CXTPChartString strText(pLabel->GetPointLabel(m_pPointView->GetPoint()));
	strText.TrimLeft();
	strText.TrimRight();

	CXTPChartTextPainter painter(pDC, strText, pLabel);

	CXTPChartSizeF szLabelF = painter.GetSize();
	CSize szLabel(static_cast<int>(szLabelF.Width), static_cast<int>(szLabelF.Height));

	const CPoint* ptAnchor = m_pSeriesLabelView->GetLabelAnchorPoint();
	ASSERT(NULL != ptAnchor);

	const CPoint* ptAnchorOrigin = m_pSeriesLabelView->GetLabelAnchorOrigin();
	ASSERT(NULL != ptAnchorOrigin);

	XTPChartPieLabelPosition position = m_pSeriesLabelView->GetPosition();
	switch (position)
	{
		case xtpChartPieLabelTwoColumns: ComputeLabel_TwoColumn(szLabel, *ptAnchor); break;
		case xtpChartPieLabelOutside:
			ComputeLabel_Outside(szLabel, *ptAnchorOrigin, *ptAnchor);
			break;
		case xtpChartPieLabelInside:
		case xtpChartPieLabelRadial: ComputeLabel_InsideAndRadial(szLabel, *ptAnchor); break;
		default: ASSERT(!"Unexpected position"); break;
	}
}

BOOL CXTPChart3dPieSeriesLabelDeviceCommand::SetColumnAnchorVerticalPosition(
	CXTPChart3dDeviceContext* pDC, int y)
{
	ASSERT_VALID(pDC);
	ASSERT(xtpChartPieLabelTwoColumns == m_pSeriesLabelView->GetPosition());

	BOOL bSuccess = FALSE;

	const CPoint* ptAnchor = m_pSeriesLabelView->GetLabelAnchorPoint();
	ASSERT(NULL != ptAnchor);

	const CXTPPoint3d* ptBend = m_pSeriesLabelView->GetConnectorBendPoint();
	ASSERT(NULL != ptBend);

	CPoint ptNewAnchor(ptAnchor->x, y);
	int xOffset = m_pSeriesLabelView->IsLeftDirected() ? XTP_CHART_PIE3D_BENT_CONNECTOR_ARM_LENGTH
													   : -XTP_CHART_PIE3D_BENT_CONNECTOR_ARM_LENGTH;
	CPoint ptNewBend(ptNewAnchor.x + xOffset, y);

	CPoint ptProjectedBend;
	double dBendZ = 0;
	if (pDC->Project(*ptBend, ptProjectedBend, &dBendZ))
	{
		double dWinZ = 0, dTestedZ = 0;
		if (pDC->DepthTest(ptProjectedBend, dTestedZ))
		{
			// If there is something in Z-buffer before bend point
			// then use far plane for projecting connector end.
			dWinZ = dTestedZ <= dBendZ ? 1 : 0;
		}

		CXTPPoint3d ptProjectedPoint;
		if (pDC->Unproject(ptNewBend, ptProjectedPoint, dBendZ))
		{
			m_pSeriesLabelView->SetConnectorBendPoint(ptProjectedPoint);

			if (pDC->Unproject(ptNewAnchor, ptProjectedPoint, dWinZ))
			{
				m_pSeriesLabelView->SetConnectorEndPoint(ptProjectedPoint);
				m_pSeriesLabelView->SetLabelAnchorPoint(ptNewAnchor);

				ComputeLabel_TwoColumn(m_rcLabel.Size(), ptNewAnchor);

				bSuccess = TRUE;
			}
		}
	}

	return bSuccess;
}

void CXTPChart3dPieSeriesLabelDeviceCommand::ComputeLabel_TwoColumn(const CSize& szLabel,
																	const CPoint& ptAnchor)
{
	ASSERT(xtpChartPieLabelTwoColumns == m_pSeriesLabelView->GetPosition());

	if (m_pSeriesLabelView->IsLeftDirected())
	{
		m_rcLabel.SetRect(ptAnchor.x - szLabel.cx, ptAnchor.y - szLabel.cy / 2, ptAnchor.x,
						  ptAnchor.y + szLabel.cy / 2);
	}
	else
	{
		m_rcLabel.SetRect(ptAnchor.x, ptAnchor.y - szLabel.cy / 2, ptAnchor.x + szLabel.cx,
						  ptAnchor.y + szLabel.cy / 2);
	}
}

void CXTPChart3dPieSeriesLabelDeviceCommand::ComputeLabel_Outside(const CSize& szLabel,
																  const CPoint& ptAnchorOrigin,
																  const CPoint& ptAnchor)
{
	ASSERT(xtpChartPieLabelOutside == m_pSeriesLabelView->GetPosition());

	m_rcLabel.SetRect(0, 0, szLabel.cx, szLabel.cy);

	if (ptAnchorOrigin.x <= ptAnchor.x && ptAnchorOrigin.y <= ptAnchor.y)
	{
		m_rcLabel.OffsetRect(ptAnchor);
	}
	else if (ptAnchor.x <= ptAnchorOrigin.x && ptAnchorOrigin.y <= ptAnchor.y)
	{
		m_rcLabel.OffsetRect(ptAnchor.x - szLabel.cx, ptAnchor.y);
	}
	else if (ptAnchorOrigin.x <= ptAnchor.x && ptAnchor.y <= ptAnchorOrigin.y)
	{
		m_rcLabel.OffsetRect(ptAnchor.x, ptAnchor.y - szLabel.cy);
	}
	else
	{
		ASSERT(ptAnchor.x <= ptAnchorOrigin.x && ptAnchor.y <= ptAnchorOrigin.y);
		m_rcLabel.OffsetRect(ptAnchor.x - szLabel.cx, ptAnchor.y - szLabel.cy);
	}
}

void CXTPChart3dPieSeriesLabelDeviceCommand::ComputeLabel_InsideAndRadial(const CSize& szLabel,
																		  const CPoint& ptAnchor)
{
	ASSERT(xtpChartPieLabelInside == m_pSeriesLabelView->GetPosition()
		   || xtpChartPieLabelRadial == m_pSeriesLabelView->GetPosition());

	m_rcLabel.SetRect(ptAnchor.x - szLabel.cx / 2, ptAnchor.y - szLabel.cy / 2,
					  ptAnchor.x + szLabel.cx / 2, ptAnchor.y + szLabel.cy / 2);
}

BOOL CXTPChart3dPieSeriesLabelDeviceCommand::ComputeConnectorVector(CXTPChart3dDeviceContext* pDC)
{
	ASSERT_VALID(pDC);
	ASSERT(!IsFailed());

	CXTPPoint3d ptStart;
	CPoint ptProjectedStart;
	BOOL bSuccess = ComputeConnectorStartPoint(pDC, ptStart, ptProjectedStart);
	if (bSuccess)
	{
		m_pSeriesLabelView->SetConnectorStartPoint(ptStart);
		m_pSeriesLabelView->SetLabelAnchorOrigin(ptProjectedStart);

		XTPChartPieLabelPosition position = m_pSeriesLabelView->GetPosition();
		switch (position)
		{
			case xtpChartPieLabelTwoColumns:
				m_pSeriesLabelView->SetConnectorBendPoint(ComputeConnectorBendPoint());
				break;
			case xtpChartPieLabelOutside:
			case xtpChartPieLabelInside:
			case xtpChartPieLabelRadial:
			{
				CXTPPoint3d ptEnd = ComputeConnectorEndPoint();
				m_pSeriesLabelView->SetConnectorEndPoint(ptEnd);

				CPoint ptAnchor;
				if (pDC->Project(ptEnd, ptAnchor))
				{
					m_pSeriesLabelView->SetLabelAnchorPoint(ptAnchor);
				}

				break;
			}
			default:
				ASSERT(!"Unexpected position");
				bSuccess = FALSE;
				break;
		}
	}

	return bSuccess;
}

BOOL CXTPChart3dPieSeriesLabelDeviceCommand::ComputeConnectorBend(CXTPChart3dDeviceContext* pDC,
																  CRect rcBoundaries)
{
	ASSERT_VALID(pDC);
	ASSERT(IsBent());
	ASSERT(!IsFailed());

	BOOL bSuccess = FALSE;

	const CXTPPoint3d* ptBend = m_pSeriesLabelView->GetConnectorBendPoint();
	if (NULL != ptBend)
	{
		CPoint ptProjectedBend;
		double dBendZ = 0;
		if (pDC->Project(*ptBend, ptProjectedBend, &dBendZ))
		{
			CPoint ptCenter = rcBoundaries.CenterPoint();
			CPoint ptEnd;
			if (ptProjectedBend.x <= ptCenter.x)
			{
				ptEnd.x = rcBoundaries.left - XTP_CHART_PIE3D_BENT_CONNECTOR_ARM_LENGTH;
				m_pSeriesLabelView->SetLeftDirected();
			}
			else
			{
				ptEnd.x = rcBoundaries.right + XTP_CHART_PIE3D_BENT_CONNECTOR_ARM_LENGTH;
				m_pSeriesLabelView->SetRightDirected();
			}

			ptEnd.y = ptProjectedBend.y;

			double dWinZ = 0, dTestedZ = 0;
			if (pDC->DepthTest(ptProjectedBend, dTestedZ))
			{
				// If there is something in Z-buffer before bend point
				// then use far plane for projecting connector end.
				dWinZ = dTestedZ <= dBendZ ? 1 : 0;
			}

			CXTPPoint3d ptProjectedEnd;
			if (pDC->Unproject(ptEnd, ptProjectedEnd, dWinZ))
			{
				m_pSeriesLabelView->SetConnectorEndPoint(ptProjectedEnd);
				m_pSeriesLabelView->SetLabelAnchorPoint(ptEnd);

				bSuccess = TRUE;
			}
		}
	}

	return bSuccess;
}

BOOL CXTPChart3dPieSeriesLabelDeviceCommand::ComputeConnectorStartPoint(
	CXTPChart3dDeviceContext* pDC, CXTPPoint3d& ptStart, CPoint& ptProjectedStart) const
{
	ASSERT_VALID(pDC);
	ASSERT(!IsFailed());

	BOOL bSuccess = FALSE;

	XTPChartPieLabelPosition position = m_pSeriesLabelView->GetPosition();
	switch (position)
	{
		case xtpChartPieLabelOutside:
		case xtpChartPieLabelTwoColumns:
			ptStart  = m_pPointView->GetOuterCenterConnectorPoint();
			bSuccess = TRUE;
			break;
		case xtpChartPieLabelInside:
		case xtpChartPieLabelRadial:
			ptStart  = m_pPointView->GetCenterShapeConnectorPoint();
			bSuccess = TRUE;
			break;
		default: ASSERT(!"Unexpected position"); break;
	}

	if (bSuccess)
	{
		bSuccess = pDC->Project(ptStart, ptProjectedStart);
	}

	return bSuccess;
}

CXTPPoint3d CXTPChart3dPieSeriesLabelDeviceCommand::ComputeConnectorBendPoint() const
{
	ASSERT(IsBent());
	ASSERT(!IsFailed());

	CXTPPoint3d point = ComputeConnectorVectorEndPoint(
		m_pPointView->GetOuterCenterConnectorPoint());
	return point;
}

CXTPPoint3d CXTPChart3dPieSeriesLabelDeviceCommand::ComputeConnectorEndPoint() const
{
	ASSERT(!IsFailed());

	CXTPPoint3d point;
	XTPChartPieLabelPosition position = m_pSeriesLabelView->GetPosition();
	switch (position)
	{
		case xtpChartPieLabelTwoColumns: ASSERT(!"Cannot be computed now"); break;
		case xtpChartPieLabelOutside:
			point = ComputeConnectorVectorEndPoint(m_pPointView->GetOuterCenterConnectorPoint());
			break;
		case xtpChartPieLabelInside:
		case xtpChartPieLabelRadial: point = m_pPointView->GetCenterShapeConnectorPoint(); break;
		default: ASSERT(!"Unexpected position"); break;
	}

	return point;
}

CXTPPoint3d CXTPChart3dPieSeriesLabelDeviceCommand::ComputeConnectorVectorEndPoint(
	const CXTPPoint3d& point) const
{
	CXTPPoint3d ptEnd = point;
	double dLength	= sqrt(point.X * point.X + point.Y * point.Y + point.Z * point.Z);
	ptEnd.X /= dLength;
	ptEnd.Y /= dLength;
	ptEnd.Z /= dLength;

	dLength += dLength * m_dConnectorLengthFactor;
	ptEnd.X *= dLength;
	ptEnd.Y *= dLength;
	ptEnd.Z *= dLength;
	ptEnd.Z += m_dConnectorEndpointVerticalOffset;

	return ptEnd;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPieSeriesLabelsViewDeviceCommand

CXTPChart3dPieSeriesLabelsViewDeviceCommand::CXTPChart3dPieSeriesLabelsViewDeviceCommand(
	CXTPChart3dPieSeriesLabelsView* pSeriesLabelsView)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDisabled)
{
	UNREFERENCED_PARAMETER(pSeriesLabelsView);
}

void CXTPChart3dPieSeriesLabelsViewDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext, pDC);
	ASSERT(NULL != p3DDC);

	INT_PTR mResolveLabelOverlapsAttemptsLeft = m_arrChildren.GetSize() * m_arrChildren.GetSize();
	CRect rcBoundaries;

	do
	{
		BOOL bNeedComputeBends = FALSE;
		ComputeLabelConnectorVectors(p3DDC, bNeedComputeBends);

		if (bNeedComputeBends)
		{
			if (ComputeBendBoundaries(p3DDC, rcBoundaries))
			{
				ComputeLabelConnectorBends(p3DDC, rcBoundaries);
			}
		}

		ComputeLabels(p3DDC);
	} while (!ResolveLabelOverlaps(p3DDC, rcBoundaries) && 0 < --mResolveLabelOverlapsAttemptsLeft);
}

CXTPChart3dPieSeriesLabel* CXTPChart3dPieSeriesLabelsViewDeviceCommand::GetLabel() const
{
	CXTPChart3dPieSeriesLabel* pLabel = NULL;

	for (int i = 0; i < m_arrChildren.GetSize() && NULL == pLabel; i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand)
		{
			pLabel = DYNAMIC_DOWNCAST(CXTPChart3dPieSeriesLabel,
									  pLabelCommand->GetSeriesLabelView()->GetLabel());
		}
	}

	return pLabel;
}

void CXTPChart3dPieSeriesLabelsViewDeviceCommand::ComputeLabelConnectorVectors(
	CXTPChart3dDeviceContext* pDC, BOOL& bNeedComputeBends)
{
	bNeedComputeBends = FALSE;

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && !pLabelCommand->IsFailed())
		{
			CXTPChartDeviceCommand* pTranspositionCommand =
				pLabelCommand->GetSeriesLabelView()->CreateTranspositionDeviceCommand(pDC);
			ASSERT(NULL != pTranspositionCommand);
			if (NULL != pTranspositionCommand)
			{
				m_CommandCallbackData.SwitchTo(CommandCallbackData::ComputeVector);
				m_CommandCallbackData.pDC			= pDC;
				m_CommandCallbackData.pLabelCommand = pLabelCommand;
				pTranspositionCommand->Subscribe(this);
				pTranspositionCommand->Execute(pDC);
				pTranspositionCommand->Unsubscribe(this);
				if (!m_CommandCallbackData.bSuccess)
					pLabelCommand->SetFailed();
				m_CommandCallbackData.SwitchTo(CommandCallbackData::None);

				delete pTranspositionCommand;
			}

			if (pLabelCommand->IsBent())
			{
				bNeedComputeBends = TRUE;
			}
		}
	}
}

BOOL CXTPChart3dPieSeriesLabelsViewDeviceCommand::ComputeBendBoundaries(
	CXTPChart3dDeviceContext* pDC, CRect& rcBoundaries)
{
	ASSERT_VALID(pDC);

	BOOL bComputed = FALSE;

	rcBoundaries.left   = INT_MAX;
	rcBoundaries.top	= INT_MAX;
	rcBoundaries.right  = INT_MIN;
	rcBoundaries.bottom = INT_MIN;

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && pLabelCommand->IsBent() && !pLabelCommand->IsFailed())
		{
			CXTPChart3dPieSeriesLabelView* pLabelView = pLabelCommand->GetSeriesLabelView();
			ASSERT(NULL != pLabelView);

			const CXTPPoint3d* ptBend = pLabelView->GetConnectorBendPoint();
			if (NULL != ptBend)
			{
				// Expand boundaries to projected connector boundaries
				CXTPChartDeviceCommand* pTranspositionCommand =
					pLabelView->CreateTranspositionDeviceCommand(pDC);
				ASSERT(NULL != pTranspositionCommand);
				if (NULL != pTranspositionCommand)
				{
					m_CommandCallbackData.SwitchTo(CommandCallbackData::ComputeBend);
					m_CommandCallbackData.pDC			= pDC;
					m_CommandCallbackData.ptBend		= ptBend;
					m_CommandCallbackData.prcBoundaries = &rcBoundaries;
					pTranspositionCommand->Subscribe(this);
					pTranspositionCommand->Execute(pDC);
					pTranspositionCommand->Unsubscribe(this);
					if (!m_CommandCallbackData.bSuccess)
						pLabelCommand->SetFailed();
					m_CommandCallbackData.SwitchTo(CommandCallbackData::None);

					delete pTranspositionCommand;
				}
			}

			// Expand boundaries to projected point boundaries
			CXTPChartSeriesPointView* pPointView = pLabelView->GetPointView();
			if (NULL != pPointView)
			{
				CRect rcPointBounds = pPointView->GetBounds();
				if (rcPointBounds.left < rcBoundaries.left)
					rcBoundaries.left = rcPointBounds.left;
				if (rcBoundaries.right < rcPointBounds.right)
					rcBoundaries.right = rcPointBounds.right;
				if (rcPointBounds.top < rcBoundaries.top)
					rcBoundaries.top = rcPointBounds.top;
				if (rcBoundaries.bottom < rcPointBounds.bottom)
					rcBoundaries.bottom = rcPointBounds.bottom;
			}
		}
	}

	if (rcBoundaries.left < rcBoundaries.right && rcBoundaries.top < rcBoundaries.bottom)
	{
		// Make boundaries square
		int nWidth  = rcBoundaries.Width();
		int nHeight = rcBoundaries.Height();

		if (nWidth < nHeight)
		{
			int nMid		   = (rcBoundaries.right + rcBoundaries.left) / 2;
			rcBoundaries.left  = nMid - nHeight / 2;
			rcBoundaries.right = nMid + nHeight / 2;
		}
		else if (nHeight < nWidth)
		{
			int nMid			= (rcBoundaries.bottom + rcBoundaries.top) / 2;
			rcBoundaries.top	= nMid - nWidth / 2;
			rcBoundaries.bottom = nMid + nWidth / 2;
		}

		bComputed = TRUE;
	}

	return bComputed;
}

void CXTPChart3dPieSeriesLabelsViewDeviceCommand::ComputeLabelConnectorBends(
	CXTPChart3dDeviceContext* pDC, CRect rcBoundaries)
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && pLabelCommand->IsBent() && !pLabelCommand->IsFailed())
		{
			CXTPChartDeviceCommand* pTranspositionCommand =
				pLabelCommand->GetSeriesLabelView()->CreateTranspositionDeviceCommand(pDC);
			ASSERT(NULL != pTranspositionCommand);
			if (NULL != pTranspositionCommand)
			{
				m_CommandCallbackData.SwitchTo(CommandCallbackData::ComputeBendConnector);
				m_CommandCallbackData.pDC			= pDC;
				m_CommandCallbackData.prcBoundaries = &rcBoundaries;
				m_CommandCallbackData.pLabelCommand = pLabelCommand;
				pTranspositionCommand->Subscribe(this);
				pTranspositionCommand->Execute(pDC);
				pTranspositionCommand->Unsubscribe(this);
				if (!m_CommandCallbackData.bSuccess)
					pLabelCommand->SetFailed();
				m_CommandCallbackData.SwitchTo(CommandCallbackData::None);

				delete pTranspositionCommand;
			}
		}
	}
}

void CXTPChart3dPieSeriesLabelsViewDeviceCommand::ComputeLabels(CXTPChart3dDeviceContext* pDC)
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && !pLabelCommand->IsFailed())
		{
			pLabelCommand->ComputeLabel(pDC);
		}
	}
}

BOOL CXTPChart3dPieSeriesLabelsViewDeviceCommand::ResolveLabelOverlaps(
	CXTPChart3dDeviceContext* pDC, const CRect& rcBoundaries)
{
	BOOL bOverlapsResolved = TRUE;

	CXTPChart3dPieSeriesLabel* pLabel = GetLabel();
	if (NULL != pLabel)
	{
		XTPChartPieLabelPosition position = pLabel->GetPosition();
		switch (position)
		{
			case xtpChartPieLabelTwoColumns:
				bOverlapsResolved =
					(ResolveLabelOverlaps_ChangePositionInColumn(pDC, TRUE, rcBoundaries)
					 && ResolveLabelOverlaps_ChangePositionInColumn(pDC, FALSE, rcBoundaries));
				break;
			case xtpChartPieLabelOutside:
				bOverlapsResolved = ResolveLabelOverlaps_ChangeConnectorLength(pDC);
				break;
			case xtpChartPieLabelInside:
			case xtpChartPieLabelRadial:
				// cannot be resolved
				break;
		}
	}

	return bOverlapsResolved;
}

namespace
{
struct LabelInfo
{
	CXTPChart3dPieSeriesLabelDeviceCommand* pCommand;
	CRect rcLabel;
	CPoint ptAnchor;
	float fDistributionFactor;

	static int AFX_CDECL Compare(const void* left, const void* right)
	{
		ASSERT(NULL != left && NULL != right);

		int nResult = 0;

		const LabelInfo* pLeft  = reinterpret_cast<const LabelInfo*>(left);
		const LabelInfo* pRight = reinterpret_cast<const LabelInfo*>(right);
		if (pLeft->fDistributionFactor < pRight->fDistributionFactor)
			nResult = -1;
		if (pRight->fDistributionFactor < pLeft->fDistributionFactor)
			nResult = 1;

		return nResult;
	}
};
} // namespace

BOOL CXTPChart3dPieSeriesLabelsViewDeviceCommand::ResolveLabelOverlaps_ChangePositionInColumn(
	CXTPChart3dDeviceContext* pDC, BOOL bLeftDirected, const CRect& rcBoundaries)
{
	BOOL bOverlapsResolved = TRUE;

	// Collect labels
	int nMinY				= INT_MAX;
	int nMaxY				= INT_MIN;
	int nColumnHeightNeeded = 0;
	int i;
	CArray<LabelInfo, const LabelInfo&> labels;
	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && !pLabelCommand->IsFailed()
			&& pLabelCommand->GetSeriesLabelView()->IsLeftDirected() == bLeftDirected)
		{
			LabelInfo labelInfo;
			labelInfo.pCommand = pLabelCommand;
			labelInfo.rcLabel  = pLabelCommand->GetLabelRect();
			ASSERT(NULL != pLabelCommand->GetSeriesLabelView()->GetLabelAnchorPoint());
			labelInfo.ptAnchor = *pLabelCommand->GetSeriesLabelView()->GetLabelAnchorPoint();
			labels.SetAtGrow(labels.GetSize(), labelInfo);

			if (labelInfo.ptAnchor.y < nMinY)
				nMinY = labelInfo.ptAnchor.y;
			if (nMaxY < labelInfo.ptAnchor.y)
				nMaxY = labelInfo.ptAnchor.y;

			nColumnHeightNeeded += labelInfo.rcLabel.Height();
		}
	}

	int nLabelCount = static_cast<int>(labels.GetSize());
	if (0 < nLabelCount)
	{
		nColumnHeightNeeded = __max(rcBoundaries.Height(), nColumnHeightNeeded);
		int nColumnStartY   = rcBoundaries.CenterPoint().y - nColumnHeightNeeded / 2;

		// Compute distribution factors
		for (i = 0; i < nLabelCount; ++i)
		{
			labels[i].fDistributionFactor = static_cast<float>(labels[i].ptAnchor.y - nMinY)
											/ static_cast<float>(nMaxY - nMinY);
		}

		// Sort according distribution factors
		qsort(labels.GetData(), XTPToSizeTChecked(nLabelCount), sizeof(LabelInfo),
			  LabelInfo::Compare);

		// Compute vertical distribution
		int nVerticalStep			 = nColumnHeightNeeded / nLabelCount;
		int nCurrentVerticalPosition = nColumnStartY;
		for (i = 0; i < nLabelCount; ++i)
		{
			LabelInfo& labelInfo = labels[i];
			int nNewYPos		 = nCurrentVerticalPosition + labelInfo.rcLabel.Height() / 2;
			nCurrentVerticalPosition += nVerticalStep;

			CXTPChartDeviceCommand* pTranspositionCommand =
				labelInfo.pCommand->GetSeriesLabelView()->CreateTranspositionDeviceCommand(pDC);
			ASSERT(NULL != pTranspositionCommand);
			if (NULL != pTranspositionCommand)
			{
				m_CommandCallbackData.SwitchTo(CommandCallbackData::ColumnAnchorVericalPosition);
				m_CommandCallbackData.pDC			= pDC;
				m_CommandCallbackData.pLabelCommand = labelInfo.pCommand;
				m_CommandCallbackData.nNewYPos		= nNewYPos;
				pTranspositionCommand->Subscribe(this);
				pTranspositionCommand->Execute(pDC);
				pTranspositionCommand->Unsubscribe(this);
				if (!m_CommandCallbackData.bSuccess)
				{
					labelInfo.pCommand->SetFailed();
					bOverlapsResolved = FALSE;
				}
				m_CommandCallbackData.SwitchTo(CommandCallbackData::None);

				delete pTranspositionCommand;
			}
		}
	}

	return bOverlapsResolved;
}

BOOL CXTPChart3dPieSeriesLabelsViewDeviceCommand::ResolveLabelOverlaps_ChangeConnectorLength(
	CXTPChart3dDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	BOOL bOverlapsResolved = TRUE;

	int nLabelCounter = 0;
	CList<CXTPChart3dPieSeriesLabelDeviceCommand*, CXTPChart3dPieSeriesLabelDeviceCommand*> rcLabels;
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && !pLabelCommand->IsFailed())
		{
			++nLabelCounter;
			CRect rcLabel = pLabelCommand->GetLabelRect();

			BOOL bOverlapped	   = FALSE;
			double dVerticalOffset = 0;
			POSITION pos		   = rcLabels.GetHeadPosition();
			while (NULL != pos)
			{
				CXTPChart3dPieSeriesLabelDeviceCommand* pOtherLabelCommand = rcLabels.GetNext(pos);
				ASSERT(NULL != pOtherLabelCommand);

				CRect rcIntersection;
				if (rcIntersection.IntersectRect(rcLabel, pOtherLabelCommand->GetLabelRect()))
				{
					bOverlapsResolved = FALSE;
					if (!bOverlapped)
					{
						bOverlapped = TRUE;

						// Offset label either up or down
						dVerticalOffset = (0 == (nLabelCounter % 2) ? rcLabel.Height()
																	: -rcLabel.Height());
					}

					// Increase connector length
					double dFactor = pLabelCommand->GetConnectorLengthFactor();
					dFactor += dFactor * 0.35;
					pLabelCommand->SetConnectorLengthFactor(dFactor);

					// Offset connector vertically
					double yOffset = pLabelCommand->GetConnectorEndpointVerticalOffset();
					pLabelCommand->SetConnectorEndpointVerticalOffset(yOffset + dVerticalOffset);
				}
			}

			if (!bOverlapped)
			{
				rcLabels.AddTail(pLabelCommand);
			}
		}
	}

	return bOverlapsResolved;
}

CXTPChart3dPieSeriesLabelDeviceCommand*
	CXTPChart3dPieSeriesLabelsViewDeviceCommand::FindLabelDeviceCommand(
		CXTPChartDeviceCommand* pCommand) const
{
	ASSERT_VALID(pCommand);

	CXTPChart3dPieSeriesLabelDeviceCommand* pLabelCommand = NULL;
	if (NULL != pCommand)
	{
		pLabelCommand = DYNAMIC_DOWNCAST(CXTPChart3dPieSeriesLabelDeviceCommand, pCommand);
		if (NULL == pLabelCommand)
		{
			for (int i = 0; i < pCommand->GetChildCommandCount() && NULL == pLabelCommand; i++)
			{
				pLabelCommand = FindLabelDeviceCommand(pCommand->GetChildCommand(i));
			}
		}
	}

	return pLabelCommand;
}

void CXTPChart3dPieSeriesLabelsViewDeviceCommand::OnAfterExecute(CXTPChartDeviceCommand* pCommand,
																 CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pCommand);
	UNREFERENCED_PARAMETER(pDC);

	switch (m_CommandCallbackData.GetState())
	{
		case CommandCallbackData::ComputeVector:
		{
			ASSERT(NULL != m_CommandCallbackData.pLabelCommand);

			m_CommandCallbackData.bSuccess =
				m_CommandCallbackData.pLabelCommand->ComputeConnectorVector(
					m_CommandCallbackData.pDC);
			break;
		}
		case CommandCallbackData::ComputeBend:
		{
			ASSERT(NULL != m_CommandCallbackData.ptBend);
			ASSERT(NULL != m_CommandCallbackData.prcBoundaries);
			ASSERT(NULL != m_CommandCallbackData.pDC);

			CPoint ptProjectedBend;
			if (m_CommandCallbackData.pDC->Project(*m_CommandCallbackData.ptBend, ptProjectedBend))
			{
				if (ptProjectedBend.x < m_CommandCallbackData.prcBoundaries->left)
					m_CommandCallbackData.prcBoundaries->left = ptProjectedBend.x;
				if (m_CommandCallbackData.prcBoundaries->right < ptProjectedBend.x)
					m_CommandCallbackData.prcBoundaries->right = ptProjectedBend.x;
				if (ptProjectedBend.y < m_CommandCallbackData.prcBoundaries->top)
					m_CommandCallbackData.prcBoundaries->top = ptProjectedBend.y;
				if (m_CommandCallbackData.prcBoundaries->bottom < ptProjectedBend.y)
					m_CommandCallbackData.prcBoundaries->bottom = ptProjectedBend.y;

				m_CommandCallbackData.bSuccess = TRUE;
			}

			break;
		}
		case CommandCallbackData::ComputeBendConnector:
		{
			ASSERT(NULL != m_CommandCallbackData.pLabelCommand);
			ASSERT(NULL != m_CommandCallbackData.prcBoundaries);
			ASSERT(NULL != m_CommandCallbackData.pDC);

			m_CommandCallbackData.bSuccess =
				m_CommandCallbackData.pLabelCommand->ComputeConnectorBend(
					m_CommandCallbackData.pDC, m_CommandCallbackData.prcBoundaries);
			break;
		}
		case CommandCallbackData::ColumnAnchorVericalPosition:
		{
			ASSERT(NULL != m_CommandCallbackData.pLabelCommand);
			ASSERT(NULL != m_CommandCallbackData.pDC);

			m_CommandCallbackData.bSuccess =
				m_CommandCallbackData.pLabelCommand->SetColumnAnchorVerticalPosition(
					m_CommandCallbackData.pDC, m_CommandCallbackData.nNewYPos);
			break;
		}
		default: ASSERT(!"Unexpected command callback data state"); break;
	}
}
