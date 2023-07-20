// XTPChartTextPainter.cpp
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
#include <float.h>
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPMarkupRender.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartContent.h"

#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartTextDeviceCommand.h"
#include "Chart/Drawing/XTPChartTransformationDeviceCommand.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Chart/Utils/XTPChartTextPainter.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartTextPainterBase

CXTPChartTextPainterBase::CXTPChartTextPainterBase(CXTPChartDeviceContext* pDC,
												   const CXTPChartString& text,
												   CXTPChartTextElement* pTextProvider)
{
	m_strText		= text;
	m_pTextProvider = pTextProvider;

	m_rcRoundedBounds.SetRectEmpty();

	m_pMarkupUIElement = NULL;

	CXTPMarkupContext* pMarkupContext = pTextProvider->GetContent()->GetMarkupContext();

	if (pMarkupContext)
	{
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, text);
	}

	if (m_pMarkupUIElement)
	{
		m_szTextSize = pDC->MeasureMarkupElement(m_pMarkupUIElement, pTextProvider->GetFont());
	}
	else
	{
		m_szTextSize = pDC->MeasureString(&text, pTextProvider->GetFont());
	}

	m_nHeight = (int)ceil(m_szTextSize.Height);
	m_nWidth  = (int)ceil(m_szTextSize.Width);
}

CXTPChartTextPainterBase::~CXTPChartTextPainterBase()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

CRect CXTPChartTextPainterBase::GetRoundedBounds()
{
	if (m_rcRoundedBounds.IsRectNull())
	{
		m_rcBounds = CalculateBounds();
		m_rcRoundedBounds.SetRect((int)m_rcBounds.X, (int)m_rcBounds.Y, (int)m_rcBounds.GetRight(),
								  (int)m_rcBounds.GetBottom());
	}
	return m_rcRoundedBounds;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTextPainter

CXTPChartTextPainter::CXTPChartTextPainter(CXTPChartDeviceContext* pDC, const CXTPChartString& text,
										   CXTPChartTextElement* pTextProvider)
	: CXTPChartTextPainterBase(pDC, text, pTextProvider)
{
	m_ptLocation = CXTPChartPointF(0, 0);
}

void CXTPChartTextPainter::SetLocation(const CXTPChartPointF& location)
{
	m_ptLocation = location;
}

CXTPChartDeviceCommand* CXTPChartTextPainter::CreateDeviceCommand(CXTPChartDeviceContext* pDC,
																  const CXTPChartColor& color)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRectF rect(m_ptLocation.X, m_ptLocation.Y, m_szTextSize.Width, m_szTextSize.Height);

	CXTPChartDeviceCommand* pStateGraphicsCommand =
		CXTPChartDrawingObjectFactory::Create2dSaveStateDeviceCommand();
	CXTPChartDeviceCommand* pTextAntialiasingDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dTextAntialiasingDeviceCommand(
			m_pTextProvider->GetAntialiasing());
	CXTPChartDeviceCommand* pTextContainer = pStateGraphicsCommand->AddChildCommand(
		pTextAntialiasingDeviceCommand);

	if (m_pMarkupUIElement)
	{
		CXTPChartDeviceCommand* pMarkupElementDeviceCommand =
			CXTPChartDrawingObjectFactory::Create2dMarkupElementDeviceCommand(
				m_pMarkupUIElement, m_pTextProvider->GetFont(), color, rect);
		pTextContainer->AddChildCommand(pMarkupElementDeviceCommand);
	}
	else
	{
		CXTPChartDeviceCommand* pBoundedTextDeviceCommand =
			CXTPChartDrawingObjectFactory::Create2dBoundedTextDeviceCommand(
				m_strText, m_pTextProvider->GetFont(), color, rect);
		pTextContainer->AddChildCommand(pBoundedTextDeviceCommand);
	}

	return pStateGraphicsCommand;
}

CXTPChartRectF CXTPChartTextPainter::CalculateBounds()
{
	return CXTPChartRectF(m_ptLocation.X, m_ptLocation.Y, m_szTextSize.Width, m_szTextSize.Height);
}

//////////////////////////////////////////////////////////////////////////
//  CXTPChartRotatedTextPainterBase

CXTPChartRotatedTextPainterBase::CXTPChartRotatedTextPainterBase(
	CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider,
	CPoint ptBase)
	: CXTPChartTextPainterBase(pDC, text, pTextProvider)
{
	m_ptBasePoint = ptBase;
	m_fAngle	  = 0;
}

void CXTPChartRotatedTextPainterBase::SetBasePoint(CPoint pt)
{
	m_ptBasePoint = pt;
}

CXTPChartRectF CXTPChartRotatedTextPainterBase::CalculateBounds()
{
	float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;

	float points[4][2];
	CalculatePoints(CalculateRotation(), GetInitialTextRect(), m_fAngle, points);

	for (int i = 0; i < 4; i++)
	{
		minX = min(minX, points[i][0]);
		maxX = max(maxX, points[i][0]);

		minY = min(minY, points[i][1]);
		maxY = max(maxY, points[i][1]);
	}

	return CXTPChartRectF(minX, minY, maxX - minX, maxY - minY);
}

CRect CXTPChartRotatedTextPainterBase::GetInitialTextRect()
{
	return CRect(CalculateLeftTopPoint(), CSize(m_nWidth, m_nHeight));
}

void CXTPChartRotatedTextPainterBase::CalculatePoints(XTPChartTextRotation rotation, CRect rect,
													  float fDegree, float points[4][2])
{
	double fPI	 = CXTPMathUtils::m_dPI;
	double fRadial = fDegree * fPI / 180.0;

	float width  = static_cast<float>(rect.Width()) / 2.0f;
	float height = static_cast<float>(rect.Height()) / 2.0f;
	float fsin   = (float)sin(fRadial);
	float fcos   = (float)cos(fRadial);

	points[0][0] = static_cast<float>(rect.left) + height * fsin + width * (1 - fcos);
	points[0][1] = static_cast<float>(rect.top) + height * (1 - fcos) - width * fsin;
	points[1][0] = static_cast<float>(rect.right) + height * fsin - width * (1 - fcos);
	points[1][1] = static_cast<float>(rect.top) + height * (1 - fcos) + width * fsin;
	points[2][0] = static_cast<float>(rect.right) - height * fsin - width * (1 - fcos);
	points[2][1] = static_cast<float>(rect.bottom) - height * (1 - fcos) + width * fsin;
	points[3][0] = static_cast<float>(rect.left) - height * fsin + width * (1 - fcos);
	points[3][1] = static_cast<float>(rect.bottom) - height * (1 - fcos) - width * fsin;

	float offset = 0;
	switch (rotation)
	{
		case xtpChartTextLeftTop: offset = width * fsin - height * (1 - fcos); break;
		case xtpChartTextCenterTop: offset = -height * (1 - fcos); break;
		case xtpChartTextRightTop: offset = -width * fsin - height * (1 - fcos); break;
		case xtpChartTextLeftCenter: offset = width * fsin; break;
		case xtpChartTextRightCenter: offset = -width * fsin; break;
		case xtpChartTextLeftBottom: offset = width * fsin + height * (1 - fcos); break;
		case xtpChartTextCenterBottom: offset = height * (1 - fcos); break;
		case xtpChartTextRightBottom: offset = -width * fsin + height * (1 - fcos); break;
		default: return;
	}

	for (int i = 0; i < 4; i++)
	{
		points[i][0] += offset;
		points[i][1] -= offset;
	}
}

CXTPChartDeviceCommand* CXTPChartRotatedTextPainterBase::CreateDeviceCommand(
	CXTPChartDeviceContext* pDC, const CXTPChartColor& color)
{
	UNREFERENCED_PARAMETER(pDC);

	CRect rect = GetInitialTextRect();
	CPoint offset;

	CPoint center(rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);

	switch (CalculateRotation())
	{
		case xtpChartTextLeftTop: offset = CPoint(rect.left, rect.top); break;
		case xtpChartTextCenterTop: offset = CPoint(center.x, rect.top); break;
		case xtpChartTextRightTop: offset = CPoint(rect.right, rect.top); break;
		case xtpChartTextLeftCenter: offset = CPoint(rect.left, center.y); break;
		case xtpChartTextCenterCenter: offset = CPoint(center.x, center.y); break;
		case xtpChartTextRightCenter: offset = CPoint(rect.right, center.y); break;
		case xtpChartTextLeftBottom: offset = CPoint(rect.left, rect.bottom); break;
		case xtpChartTextCenterBottom: offset = CPoint(center.x, rect.bottom); break;
		case xtpChartTextRightBottom: offset = CPoint(rect.right, rect.bottom); break;
		default: offset = CPoint(0, 0); break;
	}

	rect.OffsetRect(-offset.x, -offset.y);

	CXTPChartDeviceCommand* pStateGraphicsCommand =
		CXTPChartDrawingObjectFactory::Create2dSaveStateDeviceCommand();

	CXTPChartDeviceCommand* pCommand = pStateGraphicsCommand;

	CXTPChartDeviceCommand* pTranslateDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dTranslateDeviceCommand((float)offset.x,
																	  (float)offset.y);
	pCommand = pCommand->AddChildCommand(pTranslateDeviceCommand);

	CXTPChartDeviceCommand* pRotateDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dRotateDeviceCommand(m_fAngle);
	pCommand = pCommand->AddChildCommand(pRotateDeviceCommand);

	CXTPChartDeviceCommand* pTextAntialiasingDeviceCommand =
		CXTPChartDrawingObjectFactory::Create2dTextAntialiasingDeviceCommand(
			m_pTextProvider->GetAntialiasing());
	CXTPChartDeviceCommand* pTextContainer = pCommand->AddChildCommand(
		pTextAntialiasingDeviceCommand);

	if (m_pMarkupUIElement)
	{
		CXTPChartDeviceCommand* pMarkupElementDeviceCommand =
			CXTPChartDrawingObjectFactory::Create2dMarkupElementDeviceCommand(
				m_pMarkupUIElement, m_pTextProvider->GetFont(), color, CXTPChartRectF(rect));
		pTextContainer->AddChildCommand(pMarkupElementDeviceCommand);
	}
	else
	{
		CXTPChartDeviceCommand* pBoundedTextDeviceCommand =
			CXTPChartDrawingObjectFactory::Create2dBoundedTextDeviceCommand(
				m_strText, m_pTextProvider->GetFont(), color, CXTPChartRectF(rect));
		pTextContainer->AddChildCommand(pBoundedTextDeviceCommand);
	}

	return pStateGraphicsCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRotatedTextPainterNearLine

CXTPChartRotatedTextPainterNearLine::CXTPChartRotatedTextPainterNearLine(
	CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider,
	CPoint ptBase, XTPChartNearTextPosition position, float fAngle)
	: CXTPChartRotatedTextPainterBase(pDC, text, pTextProvider, ptBase)
{
	m_nNearPosition = position;
	m_fAngle		= fAngle;
}

CPoint CXTPChartRotatedTextPainterNearLine::CalculateLeftTopPoint()
{
	double fPI	 = acos(-1.0);
	double fRadial = m_fAngle * fPI / 180.0;
	int halfHeight = m_nHeight / 2;

	switch (m_nNearPosition)
	{
		case xtpChartTextNearBottom:
			if (m_fAngle == 0 || m_fAngle == 180)
				return CPoint(m_ptBasePoint.x - m_nWidth / 2, m_ptBasePoint.y);

			return m_fAngle < 180
					   ? CPoint(m_ptBasePoint.x,
								m_ptBasePoint.y - (int)(m_nHeight / 2 * sin(fRadial)))
					   : CPoint(m_ptBasePoint.x - m_nWidth,
								m_ptBasePoint.y - (int)(m_nHeight / 2 * sin(fRadial - fPI)));

		case xtpChartTextNearLeft:
			if (m_fAngle == 90 || m_fAngle == 270)
				return CPoint(m_ptBasePoint.x - m_nWidth / 2 - halfHeight,
							  m_ptBasePoint.y - halfHeight);
			return (m_fAngle < 90 || m_fAngle > 270)
					   ? CPoint(m_ptBasePoint.x - (int)(m_nWidth + fabs(halfHeight * sin(fRadial))),
								m_ptBasePoint.y - halfHeight)
					   : CPoint(m_ptBasePoint.x - (int)(fabs(halfHeight * cos(fRadial - fPI / 2))),
								m_ptBasePoint.y - halfHeight);

		case xtpChartTextNearRight:
			if (m_fAngle == 90 || m_fAngle == 270)
				return CPoint(m_ptBasePoint.x - m_nWidth / 2 + halfHeight,
							  m_ptBasePoint.y - halfHeight);
			return (m_fAngle < 90 || m_fAngle > 270)
					   ? CPoint(m_ptBasePoint.x + (int)(fabs(halfHeight * sin(fRadial))),
								m_ptBasePoint.y - halfHeight)
					   : CPoint(m_ptBasePoint.x - m_nWidth
									+ (int)(fabs(halfHeight * cos(fRadial - fPI / 2))),
								m_ptBasePoint.y - halfHeight);

		case xtpChartTextNearTop:
			if (m_fAngle == 0 || m_fAngle == 180)
				return CPoint(m_ptBasePoint.x - m_nWidth / 2, m_ptBasePoint.y - m_nHeight);
			return m_fAngle < 180
					   ? CPoint(m_ptBasePoint.x - m_nWidth,
								m_ptBasePoint.y
									- (int)(halfHeight + fabs(halfHeight * cos(fRadial))))
					   : CPoint(m_ptBasePoint.x,
								m_ptBasePoint.y
									- (int)(halfHeight + fabs(halfHeight * cos(fRadial - fPI))));
	}

	return CPoint(0, 0);
}

XTPChartTextRotation CXTPChartRotatedTextPainterNearLine::CalculateRotation()
{
	switch (m_nNearPosition)
	{
		case xtpChartTextNearBottom:
			if (m_fAngle == 0 || m_fAngle == 180)
				return xtpChartTextCenterCenter;
			return m_fAngle < 180 ? xtpChartTextLeftCenter : xtpChartTextRightCenter;

		case xtpChartTextNearLeft:
			if (m_fAngle == 90 || m_fAngle == 270)
				return xtpChartTextCenterCenter;
			return (m_fAngle < 90 || m_fAngle > 270) ? xtpChartTextRightCenter
													 : xtpChartTextLeftCenter;

		case xtpChartTextNearRight:
			if (m_fAngle == 90 || m_fAngle == 270)
				return xtpChartTextCenterCenter;
			return (m_fAngle < 90 || m_fAngle > 270) ? xtpChartTextLeftCenter
													 : xtpChartTextRightCenter;

		case xtpChartTextNearTop:
			if (m_fAngle == 0 || m_fAngle == 180)
				return xtpChartTextCenterCenter;
			return m_fAngle < 180 ? xtpChartTextRightCenter : xtpChartTextLeftCenter;

		default: return xtpChartTextCenterCenter;
	}
}
