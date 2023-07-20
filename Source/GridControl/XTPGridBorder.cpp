// XTPGridBorder.cpp : implementation of the CXTPGridBorder class.
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

#include "StdAfx.h"

#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridThickness.h"
#include "GridControl/XTPGridBorder.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPGridBorderEdge::CXTPGridBorderEdge()
	: m_lineStyle(xtpGridLineStyleNone)
{
	// Nothing
}

XTPGridLineStyle CXTPGridBorderEdge::GetLineStyle() const
{
	return m_lineStyle;
}

void CXTPGridBorderEdge::SetLineStyle(XTPGridLineStyle lineStyle)
{
	m_lineStyle = lineStyle;
}

CXTPGridBorder::CXTPGridBorder()
	: m_crBackground(XTP_GRID_COLOR_DEFAULT)
	, m_crBorder(RGB(0, 0, 0))

	, m_pMargin(NULL)
	, m_pBorderThickness(NULL)
	, m_pPadding(NULL)
{
	m_pMargin		   = new CXTPGridThickness();
	m_pBorderThickness = new CXTPGridThickness();
	m_pPadding		   = new CXTPGridThickness();

	// Instantiate edges
	m_pEdgeLeft   = new CXTPGridBorderEdge();
	m_pEdgeTop	= new CXTPGridBorderEdge();
	m_pEdgeRight  = new CXTPGridBorderEdge();
	m_pEdgeBottom = new CXTPGridBorderEdge();
}

CXTPGridBorder::~CXTPGridBorder()
{
	SAFE_DELETE(m_pMargin);
	SAFE_DELETE(m_pBorderThickness);
	SAFE_DELETE(m_pPadding);

	// Delete edges
	SAFE_DELETE(m_pEdgeLeft);
	SAFE_DELETE(m_pEdgeTop);
	SAFE_DELETE(m_pEdgeRight);
	SAFE_DELETE(m_pEdgeBottom);
}

void CXTPGridBorder::Draw(CDC* pDC, CRect rcBorder)
{
	DrawH(pDC, rcBorder);
	DrawV(pDC, rcBorder);
}

void CXTPGridBorder::DrawH(CDC* pDC, CRect rcBorder)
{
	if (rcBorder.bottom < rcBorder.top)
	{
		return;
	}

	rcBorder = CXTPGridThickness::DeflateRect(rcBorder, GetMargin());

	CRect rcTop(rcBorder.left, rcBorder.top, rcBorder.right,
				rcBorder.top + m_pBorderThickness->GetTop());
	CRect rcBottom(rcBorder.left, rcBorder.bottom - m_pBorderThickness->GetBottom(), rcBorder.right,
				   rcBorder.bottom);

	CBrush brush(m_crBorder);
	pDC->FillRect(rcTop, &brush);
	pDC->FillRect(rcBottom, &brush);
	brush.DeleteObject();
}

void CXTPGridBorder::DrawV(CDC* pDC, CRect rcBorder)
{
	if (rcBorder.bottom < rcBorder.top)
	{
		return;
	}

	rcBorder = CXTPGridThickness::DeflateRect(rcBorder, GetMargin());

	CRect rcLeft(rcBorder.left, rcBorder.top, rcBorder.left + m_pBorderThickness->GetLeft(),
				 rcBorder.bottom);
	CRect rcRight(rcBorder.right - m_pBorderThickness->GetRight(), rcBorder.top, rcBorder.right,
				  rcBorder.bottom);

	CBrush brush(m_crBorder);
	pDC->FillRect(rcLeft, &brush);
	pDC->FillRect(rcRight, &brush);
	brush.DeleteObject();
}

CXTPGridBorderEdge* CXTPGridBorder::GetEdge(const XTPGridBorderEdge edge) const
{
	CXTPGridBorderEdge* pEdge = NULL;

	switch (edge)
	{
		case xtpGridBorderEdgeLeft: pEdge = m_pEdgeLeft; break;
		case xtpGridBorderEdgeTop: pEdge = m_pEdgeTop; break;
		case xtpGridBorderEdgeRight: pEdge = m_pEdgeRight; break;
		case xtpGridBorderEdgeBottom: pEdge = m_pEdgeBottom; break;
	}

	return pEdge;
}

COLORREF CXTPGridBorder::GetBorderColor() const
{
	return m_crBorder;
}

void CXTPGridBorder::SetBorderColor(COLORREF crBorder)
{
	m_crBorder = crBorder;
}

CSize CXTPGridBorder::GetSize() const
{
	return GetMargin()->GetSize() + GetBorderThickness()->GetSize() + GetPadding()->GetSize();
}

CXTPGridThickness* CXTPGridBorder::GetMargin() const
{
	return m_pMargin;
}

CXTPGridThickness* CXTPGridBorder::GetBorderThickness() const
{
	return m_pBorderThickness;
}

CXTPGridThickness* CXTPGridBorder::GetPadding() const
{
	return m_pPadding;
}

CRect AFX_CDECL CXTPGridBorder::DeflateRect(CRect rcDeflate, const CXTPGridBorder* pBorder)
{
	rcDeflate = CXTPGridThickness::DeflateRect(rcDeflate, pBorder->GetMargin());
	rcDeflate = CXTPGridThickness::DeflateRect(rcDeflate, pBorder->GetBorderThickness());
	rcDeflate = CXTPGridThickness::DeflateRect(rcDeflate, pBorder->GetPadding());
	return rcDeflate;
}
