// XTPChart3dPyramidSeriesLabelsViewDeviceCommand.cpp
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
#include "Common/XTPDrawHelpers.h"

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
#include "Chart/Drawing/XTPChart3dDeviceContext.h"
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
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesPointView.h"
#include "Chart/Styles/Pyramid/3D/XTPChart3dPyramidSeriesLabel.h"

#define XTP_CHART_PYRAMID3D_OUTER_CONNECTOR_LENGTH 10
#define XTP_CHART_PYRAMID3D_CONNECTOR_ENDPOINT_VERTICAL_OFFSET 5
#define XTP_CHART_PYRAMID3D_OUTER_CONNECTOR_THICKNESS 1

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesLabelDeviceCommand

IMPLEMENT_DYNAMIC(CXTPChart3dPyramidSeriesLabelDeviceCommand, CXTPChart3dDeviceCommand);

CXTPChart3dPyramidSeriesLabelDeviceCommand::CXTPChart3dPyramidSeriesLabelDeviceCommand(
	CXTPChart3dPyramidSeriesLabelView* pSeriesLabelView)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDisabled)
	, m_pSeriesLabelView(pSeriesLabelView)
	, m_bFailed(FALSE)
	, m_dConnectorLength(XTP_CHART_PYRAMID3D_OUTER_CONNECTOR_LENGTH)
	, m_dConnectorEndpointVerticalOffset(XTP_CHART_PYRAMID3D_CONNECTOR_ENDPOINT_VERTICAL_OFFSET)
{
	ASSERT(NULL != m_pSeriesLabelView);

	m_pPointView = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesPointView,
									m_pSeriesLabelView->GetPointView());
	ASSERT(NULL != m_pPointView);
}

void CXTPChart3dPyramidSeriesLabelDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	if (!IsCommitted() && !IsFailed())
	{
		const CXTPPoint3d* ptStart = m_pSeriesLabelView->GetConnectorStartPoint();
		const CXTPPoint3d* ptEnd   = m_pSeriesLabelView->GetConnectorEndPoint();
		if (NULL != ptStart && NULL != ptEnd)
		{
			CXTPChartDeviceCommand* pCommand	  = NULL;
			XTPChartPyramidLabelPosition position = m_pSeriesLabelView->GetPosition();

			CXTPChartSeriesLabel* pLabel = m_pSeriesLabelView->GetLabel();
			ASSERT(NULL != pLabel);
			if (pLabel->GetShowLines() && m_pSeriesLabelView->IsConnectorVisible()
				&& xtpChartPyramidLabelCenter != position)
			{
				pCommand = pDC->GetDrawingObjectFactory()->CreateSolidLineDeviceCommand(
					*ptStart, *ptEnd, m_pSeriesLabelView->GetPointView()->GetColor(),
					XTP_CHART_PYRAMID3D_OUTER_CONNECTOR_THICKNESS);
				AddChildCommand(pCommand);
			}

			CXTPChartString strText(pLabel->GetPointLabel(m_pPointView->GetPoint()));
			strText.TrimLeft();
			strText.TrimRight();

			CXTPChartColor clrBackColor = pLabel->GetActualBackColor();
			AddChildCommand(
				pLabel->GetFillStyle()->CreateDeviceCommand(m_rcLabel, clrBackColor, clrBackColor));

			CXTPChartTextPainter painter(pDC, strText, pLabel);
			CXTPChartSizeF szLabel = painter.GetSize();
			painter.SetLocation(
				CXTPChartPointF(static_cast<int>(m_rcLabel.left), static_cast<int>(m_rcLabel.top)));
			pCommand = painter.CreateDeviceCommand(pDC, m_pSeriesLabelView->GetActualTextColor());
			AddChildCommand(pCommand);

			int nThickness = pLabel->GetBorder()->GetThickness();
			if (0 < nThickness)
			{
				pCommand = pLabel->GetBorder()->CreateDeviceCommand(
					m_rcLabel, m_pSeriesLabelView->GetActualBorderColor());
				AddChildCommand(pCommand);
			}
		}

		Commit();
	}
}

void CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeLabel(CXTPChart3dDeviceContext* pDC)
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

	XTPChartPyramidLabelPosition position = m_pSeriesLabelView->GetPosition();
	switch (position)
	{
		case xtpChartPyramidLabelLeft:
		case xtpChartPyramidLabelRight:
			ComputeLabel_Outside(szLabel, *ptAnchorOrigin, *ptAnchor);
			break;
		case xtpChartPyramidLabelCenter: ComputeLabel_Center(szLabel, *ptAnchor); break;
		default: ASSERT(!"Unexpected position"); break;
	}
}

void CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeLabel_Outside(const CSize& szLabel,
																	  const CPoint& ptAnchorOrigin,
																	  const CPoint& ptAnchor)
{
	ASSERT(xtpChartPyramidLabelLeft == m_pSeriesLabelView->GetPosition()
		   || xtpChartPyramidLabelRight == m_pSeriesLabelView->GetPosition());

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

void CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeLabel_Center(const CSize& szLabel,
																	 const CPoint& ptAnchor)
{
	ASSERT(xtpChartPyramidLabelCenter == m_pSeriesLabelView->GetPosition());

	m_rcLabel.SetRect(ptAnchor.x - szLabel.cx / 2, ptAnchor.y - szLabel.cy / 2,
					  ptAnchor.x + szLabel.cx / 2, ptAnchor.y + szLabel.cy / 2);
}

BOOL CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeConnectorVector(
	CXTPChart3dDeviceContext* pDC)
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

		CXTPPoint3d ptEnd = ComputeConnectorEndPoint();
		m_pSeriesLabelView->SetConnectorEndPoint(ptEnd);

		CPoint ptAnchor;
		if (pDC->Project(ptEnd, ptAnchor))
		{
			m_pSeriesLabelView->SetLabelAnchorPoint(ptAnchor);
		}
	}

	return bSuccess;
}

BOOL CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeConnectorStartPoint(
	CXTPChart3dDeviceContext* pDC, CXTPPoint3d& ptStart, CPoint& ptProjectedStart) const
{
	ASSERT_VALID(pDC);
	ASSERT(!IsFailed());

	ptStart = m_pPointView->GetOuterCenterConnectorPoint();
	return pDC->Project(ptStart, ptProjectedStart);
}

CXTPPoint3d CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeConnectorEndPoint() const
{
	ASSERT(!IsFailed());

	CXTPPoint3d point;
	XTPChartPyramidLabelPosition position = m_pSeriesLabelView->GetPosition();
	switch (position)
	{
		case xtpChartPyramidLabelLeft:
		case xtpChartPyramidLabelRight:
			point = ComputeConnectorVectorEndPoint(m_pPointView->GetOuterCenterConnectorPoint());
			break;
		case xtpChartPyramidLabelCenter:
			point = m_pPointView->GetOuterCenterConnectorPoint();
			break;
		default: ASSERT(!"Unexpected position"); break;
	}

	return point;
}

CXTPPoint3d CXTPChart3dPyramidSeriesLabelDeviceCommand::ComputeConnectorVectorEndPoint(
	const CXTPPoint3d& point) const
{
	CXTPPoint3d ptEnd = point;
	double dLength	= sqrt(point.X * point.X + point.Y * point.Y);
	ptEnd.X /= dLength;
	ptEnd.Y /= dLength;

	dLength += m_dConnectorLength;
	ptEnd.X *= dLength;
	ptEnd.Y *= dLength;
	ptEnd.Z -= m_dConnectorEndpointVerticalOffset;

	return ptEnd;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPChart3dPyramidSeriesLabelsViewDeviceCommand

CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::CXTPChart3dPyramidSeriesLabelsViewDeviceCommand(
	CXTPChart3dPyramidSeriesLabelsView* pSeriesLabelsView)
	: CXTPChart3dDeviceCommand(xtpChart3dAntialiasingPolicyDisabled)
{
	UNREFERENCED_PARAMETER(pSeriesLabelsView);
}

void CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext, pDC);
	ASSERT(NULL != p3DDC);

	INT_PTR mResolveLabelOverlapsAttemptsLeft = m_arrChildren.GetSize() * m_arrChildren.GetSize();

	do
	{
		ComputeLabelConnectorVectors(p3DDC);
		ComputeLabels(p3DDC);
	} while (!ResolveLabelOverlaps(p3DDC) && 0 < --mResolveLabelOverlapsAttemptsLeft);
}

CXTPChart3dPyramidSeriesLabel* CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::GetLabel() const
{
	CXTPChart3dPyramidSeriesLabel* pLabel = NULL;

	for (int i = 0; i < m_arrChildren.GetSize() && NULL == pLabel; i++)
	{
		CXTPChart3dPyramidSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand)
		{
			pLabel = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesLabel,
									  pLabelCommand->GetSeriesLabelView()->GetLabel());
		}
	}

	return pLabel;
}

void CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::ComputeLabelConnectorVectors(
	CXTPChart3dDeviceContext* pDC)
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPyramidSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
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
		}
	}
}

void CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::ComputeLabels(CXTPChart3dDeviceContext* pDC)
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPyramidSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
			m_arrChildren[i]);
		if (NULL != pLabelCommand && !pLabelCommand->IsFailed())
		{
			pLabelCommand->ComputeLabel(pDC);
		}
	}
}

BOOL CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::ResolveLabelOverlaps(
	CXTPChart3dDeviceContext* pDC)
{
	BOOL bOverlapsResolved = TRUE;

	CXTPChart3dPyramidSeriesLabel* pLabel = GetLabel();
	if (NULL != pLabel)
	{
		XTPChartPyramidLabelPosition position = pLabel->GetPosition();
		switch (position)
		{
			case xtpChartPyramidLabelLeft:
			case xtpChartPyramidLabelRight:
				bOverlapsResolved = ResolveLabelOverlaps_ChangeConnectorLength(pDC);
				break;
			case xtpChartPyramidLabelCenter:
				// cannot be resolved
				break;
		}
	}

	return bOverlapsResolved;
}

BOOL CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::ResolveLabelOverlaps_ChangeConnectorLength(
	CXTPChart3dDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	BOOL bOverlapsResolved = TRUE;

	int nLabelCounter = 0;
	CList<CXTPChart3dPyramidSeriesLabelDeviceCommand*, CXTPChart3dPyramidSeriesLabelDeviceCommand*>
		rcLabels;
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChart3dPyramidSeriesLabelDeviceCommand* pLabelCommand = FindLabelDeviceCommand(
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
				CXTPChart3dPyramidSeriesLabelDeviceCommand* pOtherLabelCommand = rcLabels.GetNext(
					pos);
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
					double dLength = pLabelCommand->GetConnectorLength();
					dLength += dLength * 0.35;
					pLabelCommand->SetConnectorLength(dLength);

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

CXTPChart3dPyramidSeriesLabelDeviceCommand*
	CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::FindLabelDeviceCommand(
		CXTPChartDeviceCommand* pCommand) const
{
	ASSERT_VALID(pCommand);

	CXTPChart3dPyramidSeriesLabelDeviceCommand* pLabelCommand = NULL;
	if (NULL != pCommand)
	{
		pLabelCommand = DYNAMIC_DOWNCAST(CXTPChart3dPyramidSeriesLabelDeviceCommand, pCommand);
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

void CXTPChart3dPyramidSeriesLabelsViewDeviceCommand::OnAfterExecute(
	CXTPChartDeviceCommand* pCommand, CXTPChartDeviceContext* pDC)
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
		default: ASSERT(!"Unexpected command callback data state"); break;
	}
}
