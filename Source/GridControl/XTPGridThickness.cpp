// XTPGridThickness.cpp: interface for the CXTPGridThickness class.
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

#include "Common/XTPFramework.h"
#include "GridControl/XTPGridThickness.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPGridThickness::CXTPGridThickness()
	: m_nLeft(0)
	, m_nTop(0)
	, m_nRight(0)
	, m_nBottom(0)
{
	// Nothing
}

CXTPGridThickness::CXTPGridThickness(const CXTPGridThickness& thickness)
	: m_nLeft(thickness.m_nLeft)
	, m_nTop(thickness.m_nTop)
	, m_nRight(thickness.m_nRight)
	, m_nBottom(thickness.m_nBottom)
{
	// Nothing
}

CXTPGridThickness::CXTPGridThickness(const int nThickness)
	: m_nLeft(nThickness)
	, m_nTop(nThickness)
	, m_nRight(nThickness)
	, m_nBottom(nThickness)
{
	// Nothing
}

CXTPGridThickness::CXTPGridThickness(const int nLeft, const int nTop, const int nRight,
									 const int nBottom)
	: m_nLeft(nLeft)
	, m_nTop(nTop)
	, m_nRight(nRight)
	, m_nBottom(nBottom)
{
	// Nothing
}

void CXTPGridThickness::Set(const CXTPGridThickness& thickness)
{
	m_nLeft   = thickness.m_nLeft;
	m_nTop	= thickness.m_nTop;
	m_nRight  = thickness.m_nRight;
	m_nBottom = thickness.m_nBottom;
}

void CXTPGridThickness::Set(const int nThickness)
{
	m_nLeft   = nThickness;
	m_nTop	= nThickness;
	m_nRight  = nThickness;
	m_nBottom = nThickness;
}

void CXTPGridThickness::Set(const int nLeft, const int nTop, const int nRight, const int nBottom)
{
	m_nLeft   = nLeft;
	m_nTop	= nTop;
	m_nRight  = nRight;
	m_nBottom = nBottom;
}

int CXTPGridThickness::GetLeft() const
{
	return m_nLeft;
}

void CXTPGridThickness::SetLeft(const int nThickness)
{
	m_nLeft = nThickness;
}

int CXTPGridThickness::GetTop() const
{
	return m_nTop;
}

void CXTPGridThickness::SetTop(const int nThickness)
{
	m_nTop = nThickness;
}

int CXTPGridThickness::GetRight() const
{
	return m_nRight;
}

void CXTPGridThickness::SetRight(const int nThickness)
{
	m_nRight = nThickness;
}

int CXTPGridThickness::GetBottom() const
{
	return m_nBottom;
}

void CXTPGridThickness::SetBottom(const int nThickness)
{
	m_nBottom = nThickness;
}

CSize CXTPGridThickness::GetSize() const
{
	return CSize(m_nLeft + m_nRight, m_nTop + m_nBottom);
}

CRect AFX_CDECL CXTPGridThickness::DeflateRect(CRect rcDeflate, const CXTPGridThickness* pThickness)
{
	if (NULL != pThickness)
	{
		rcDeflate.left += pThickness->GetLeft();
		rcDeflate.top += pThickness->GetTop();
		rcDeflate.right -= pThickness->GetRight();
		rcDeflate.bottom -= pThickness->GetBottom();
	}

	return rcDeflate;
}

CXTPGridThickness& CXTPGridThickness::operator=(const CXTPGridThickness& thickness)
{
	Set(thickness);
	return *this;
}

CXTPGridThickness& CXTPGridThickness::operator=(const int nThickness)
{
	Set(nThickness);
	return *this;
}
