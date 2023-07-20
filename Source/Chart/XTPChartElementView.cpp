// XTPChartElementView.cpp
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

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartElementView

IMPLEMENT_DYNAMIC(CXTPChartElementView, CXTPChartObject);

CXTPChartElementView::CXTPChartElementView(CXTPChartElementView* pParentView)
{
	Initialize(pParentView, TRUE);
}

CXTPChartElementView::CXTPChartElementView(CXTPChartElementView* pParentView, BOOL bAddToParent)
{
	Initialize(pParentView, bAddToParent);
}

CXTPChartElementView::CXTPChartElementView(CXTPChartContainer* pContainer)
{
	m_pParentView = NULL;
	m_pContainer  = pContainer;
	m_rcBounds.SetRectEmpty();
}

CXTPChartElementView::~CXTPChartElementView()
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		if (NULL != m_arrChildren[i])
		{
			delete m_arrChildren[i];
		}
	}

	m_arrChildren.RemoveAll();
}

void CXTPChartElementView::Initialize(CXTPChartElementView* pParentView, BOOL bAddToParent)
{
	m_pParentView = pParentView;
	ASSERT(NULL != pParentView);

	if (bAddToParent && NULL != pParentView)
	{
		pParentView->m_arrChildren.Add(this);
	}

	m_pContainer = m_pParentView->m_pContainer;
	m_rcBounds.SetRectEmpty();
}

CXTPChartElementView* CXTPChartElementView::AddChildView(CXTPChartElementView* pChild)
{
	if (NULL != pChild)
	{
		m_arrChildren.Add(pChild);
		pChild->m_pParentView = this;
	}

	return pChild;
}

CXTPChartDeviceCommand* CXTPChartElementView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		if (NULL != m_arrChildren[i])
		{
			pCommand->AddChildCommand(m_arrChildren[i]->CreateDeviceCommand(pDC));
		}
	}

	return pCommand;
}

void CXTPChartElementView::Release()
{
	delete this;
}

CRect CXTPChartElementView::GetBounds(BOOL bIncludingChildren) const
{
	CRect rcBounds = GetBounds();

	if (bIncludingChildren)
	{
		if (rcBounds.IsRectEmpty())
		{
			// Current boundaries are unknown, adjust to children boundaries only
			rcBounds = CRect(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
		}

		for (int i = 0; i < m_arrChildren.GetSize(); i++)
		{
			CRect rcChildBounds = m_arrChildren[i]->GetBounds(TRUE);

			if (rcChildBounds.left < rcBounds.left)
				rcBounds.left = rcChildBounds.left;
			if (rcBounds.right < rcChildBounds.right)
				rcBounds.right = rcChildBounds.right;

			if (rcChildBounds.top < rcBounds.top)
				rcBounds.top = rcChildBounds.top;
			if (rcBounds.bottom < rcChildBounds.bottom)
				rcBounds.bottom = rcChildBounds.bottom;
		}
	}

	return rcBounds;
}

void CXTPChartElementView::OnMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{
}

void CXTPChartElementView::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
}

void CXTPChartElementView::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
}

BOOL CXTPChartElementView::OnSetCursor(CPoint /*point*/)
{
	return FALSE;
}
