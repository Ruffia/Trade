// XTPChartDeviceCommand.cpp
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
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPTypeId.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesStyle.h"

#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChart3dDeviceContext.h"

#include "Chart/XTPChartObjectFactory.h"
#include "Chart/Drawing/XTPChartDrawingObjectFactory.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPChartDeviceCommandEvents

CXTPChartDeviceCommandEvents::CXTPChartDeviceCommandEvents()
{
}

void CXTPChartDeviceCommandEvents::OnBeforeExecute(CXTPChartDeviceCommand* pCommand,
												   CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pCommand);
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartDeviceCommandEvents::OnAfterExecute(CXTPChartDeviceCommand* pCommand,
												  CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pCommand);
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartDeviceCommandEvents::OnUpdateBounds(CXTPChartDeviceCommand* pCommand, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pCommand);
	UNREFERENCED_PARAMETER(rcBounds);
}

void CXTPChartDeviceCommandEvents::OnUnsubscribe(CXTPChartDeviceCommand* pCommand)
{
	ASSERT_VALID(pCommand);
	pCommand->Unsubscribe(this);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartDeviceCommand

IMPLEMENT_DYNAMIC(CXTPChartDeviceCommand, CXTPChartObject);

CXTPChartDeviceCommand::CXTPChartDeviceCommand()
	: m_bCommitted(FALSE)
	, m_nPassNumber(0)
	, m_bEnabled(TRUE)
	, m_pCurrentDC(NULL)
	, m_pBoundView(NULL)
{
}

CXTPChartDeviceCommand::~CXTPChartDeviceCommand()
{
	Notify(&CXTPChartDeviceCommandEvents::OnUnsubscribe);

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		if (NULL != m_arrChildren[i])
		{
			delete m_arrChildren[i];
		}
	}

	m_arrChildren.RemoveAll();

	SAFE_RELEASE(m_pBoundView);
}

int CXTPChartDeviceCommand::GetChildCommandCount() const
{
	return static_cast<int>(m_arrChildren.GetSize());
}

CXTPChartDeviceCommand* CXTPChartDeviceCommand::GetChildCommand(int nIndex) const
{
	ASSERT(0 <= nIndex && nIndex < GetChildCommandCount());

	CXTPChartDeviceCommand* pCommand = NULL;

	if (0 <= nIndex && nIndex < GetChildCommandCount())
	{
		pCommand = m_arrChildren[nIndex];
	}

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartDeviceCommand::AddChildCommand(CXTPChartDeviceCommand* pCommand)
{
	ASSERT("Immutable once committed" && !IsCommitted());

	if (!IsCommitted())
	{
		if (NULL != pCommand)
		{
			m_arrChildren.Add(pCommand);
		}
	}

	return pCommand;
}

void CXTPChartDeviceCommand::Execute(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	m_pCurrentDC = pDC;

	BOOL bEnabled = IsEnabled();

	if (bEnabled && pDC->Is3D() == Is3D())
	{
		BeforeExecute(pDC);
		Notify(&CXTPChartDeviceCommandEvents::OnBeforeExecute, pDC);
		ExecuteOverride(pDC);
	}

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		pDC->Execute(m_arrChildren[i]);
	}

	if (bEnabled && pDC->Is3D() == Is3D())
	{
		Notify(&CXTPChartDeviceCommandEvents::OnAfterExecute, pDC);
		AfterExecute(pDC);
	}

	m_pCurrentDC = NULL;

	++m_nPassNumber;
}

void CXTPChartDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);
	UNREFERENCED_PARAMETER(pDC);
}

CXTPChartElement* CXTPChartDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	for (int i = (int)m_arrChildren.GetUpperBound(); i >= 0; i--)
	{
		CXTPChartElement* pElement = m_arrChildren[i]->HitTest(point, pParent);
		if (NULL != pElement)
		{
			return pElement;
		}
	}

	return NULL;
}

void CXTPChartDeviceCommand::ApplySeriesStyle(CXTPChartSeriesStyle* pStyle)
{
	ASSERT_VALID(pStyle);
	UNREFERENCED_PARAMETER(pStyle);
}

void CXTPChartDeviceCommand::Prepare(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);

	m_pCurrentDC = pDC;

	if (IsEnabled() && pDC->Is3D() == Is3D())
	{
		PrepareOverride(pDC);
	}

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		if (NULL != m_arrChildren[i])
		{
			m_arrChildren[i]->Prepare(pDC);
		}
	}

	m_pCurrentDC = NULL;
}

void CXTPChartDeviceCommand::PrepareOverride(CXTPChartDeviceContext* pDC)
{
	ASSERT_VALID(pDC);
	UNREFERENCED_PARAMETER(pDC);
}

BOOL CXTPChartDeviceCommand::IsEnabled() const
{
	return m_bEnabled;
}

void CXTPChartDeviceCommand::BindView(CXTPChartElementView* pView)
{
	SAFE_RELEASE(m_pBoundView);
	m_pBoundView = pView;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChart3dDeviceCommand

IMPLEMENT_DYNAMIC(CXTPChart3dDeviceCommand, CXTPChartDeviceCommand);

CXTPChart3dDeviceCommand::CXTPChart3dDeviceCommand(XTPChart3dAntialiasingPolicy nAntialiasingPolicy)
	: m_nAntialiasingPolicy(nAntialiasingPolicy)
{
	Set3D();
}

BOOL CXTPChart3dDeviceCommand::IsEnabled() const
{
	BOOL bEnabled = CXTPChartDeviceCommand::IsEnabled();
	if (bEnabled && xtpChart3dAntialiasingPolicyDefault != m_nAntialiasingPolicy)
	{
		CXTPChartDeviceContext* pCurrentDC = GetCurrentDC();
		if (NULL != pCurrentDC)
		{
			CXTPChart3dDeviceContext* p3DDC = DYNAMIC_DOWNCAST(CXTPChart3dDeviceContext,
															   pCurrentDC);
			if (NULL != p3DDC)
			{
				if (!p3DDC->IsSkippingAntialiasing())
				{
					if (p3DDC->IsRenderingAntialiasing())
					{
						bEnabled = (xtpChart3dAntialiasingPolicyEnabled == m_nAntialiasingPolicy);
					}
					else
					{
						bEnabled = (xtpChart3dAntialiasingPolicyEnabled != m_nAntialiasingPolicy);
					}
				}
				else
				{
					bEnabled = TRUE;
				}
			}
		}
	}

	return bEnabled;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartHitTestElementCommand

CXTPChartHitTestElementCommand::CXTPChartHitTestElementCommand(CXTPChartElement* pElement)
{
	m_pElement = pElement;
	m_rcBounds.SetRectEmpty();
}

CXTPChartHitTestElementCommand::CXTPChartHitTestElementCommand(CXTPChartElement* pElement,
															   const CRect& rcBounds)
{
	m_pElement = pElement;
	m_rcBounds = rcBounds;
}

CXTPChartHitTestElementCommand::CXTPChartHitTestElementCommand(CXTPChartElement* pElement,
															   const CXTPChartRectF& rcBounds)
{
	m_pElement = pElement;

	m_rcBounds.left   = (int)rcBounds.GetLeft();
	m_rcBounds.right  = (int)rcBounds.GetRight();
	m_rcBounds.top	= (int)rcBounds.GetTop();
	m_rcBounds.bottom = (int)rcBounds.GetBottom();
}

CXTPChartElement* CXTPChartHitTestElementCommand::HitTest(CPoint point,
														  CXTPChartElement* /*pParent*/) const
{
	if (!m_rcBounds.IsRectEmpty())
	{
		if (!m_rcBounds.PtInRect(point))
		{
			return NULL;
		}
	}

	for (int i = (int)m_arrChildren.GetSize() - 1; i >= 0; i--)
	{
		CXTPChartElement* pElement = m_arrChildren[i]->HitTest(point, m_pElement);
		if (NULL != pElement)
		{
			return pElement;
		}
	}

	return NULL;
}
