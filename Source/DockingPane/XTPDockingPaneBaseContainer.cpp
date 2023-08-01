// XTPDockingPaneBaseContainer.cpp : implementation of the CXTPDockingPaneBaseContainer class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"

#include "TabManager/Includes.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneBaseContainer.h"
#include "DockingPane/XTPDockingPaneManager.h"
#include "DockingPane/XTPDockingPane.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneBaseContainer

CXTPDockingPaneBaseContainer::CXTPDockingPaneBaseContainer(XTPDockingPaneType type,
														   CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(type, pLayout)
{
}

CXTPDockingPaneBaseContainer::~CXTPDockingPaneBaseContainer()
{
}

POSITION CXTPDockingPaneBaseContainer::ContainPane(CXTPDockingPaneBase* pPane) const
{
	if (pPane == (CXTPDockingPaneBase*)this)
		return (POSITION)TRUE;

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		POSITION posContain = pos;

		if (m_lstPanes.GetNext(pos)->ContainPane(pPane))
			return posContain;
	}
	return NULL;
}

void CXTPDockingPaneBaseContainer::FindPane(XTPDockingPaneType type,
											CXTPDockingPaneBaseList* pList) const
{
	if (GetType() == type)
	{
		if (IsEmpty())
		{
			return;
		}

		pList->AddTail((CXTPDockingPaneBase*)this);
	}

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);
		pPane->FindPane(type, pList);
	}
}

BOOL CXTPDockingPaneBaseContainer::IsEmpty() const
{
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);

		if (!pPane->IsEmpty())
		{
			return FALSE;
		}
	}
	return TRUE;
}

POSITION CXTPDockingPaneBaseContainer::GetHeadPosition() const
{
	return m_lstPanes.GetHeadPosition();
}

CXTPDockingPaneBase* CXTPDockingPaneBaseContainer::GetNext(POSITION& pos) const
{
	return m_lstPanes.GetNext(pos);
}

CXTPDockingPaneBase* CXTPDockingPaneBaseContainer::GetFirstPane() const
{
	if (m_lstPanes.IsEmpty())
		return NULL;

	return m_lstPanes.GetHead();
}

CXTPDockingPaneBase* CXTPDockingPaneBaseContainer::GetLastPane() const
{
	if (m_lstPanes.IsEmpty())
		return NULL;

	return m_lstPanes.GetTail();
}

BOOL CXTPDockingPaneBaseContainer::OnAction(XTPDockingPaneAction action)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();
	CXTPDockingPaneBaseList lstChilds;

	FindPane(xtpPaneTypeDockingPane, &lstChilds);
	POSITION pos = lstChilds.GetHeadPosition();

	while (pos)
	{
		CXTPDockingPane* pPane = static_cast<CXTPDockingPane*>(lstChilds.GetNext(pos));

		if (pManager->NotifyAction(action, pPane, this))
			return TRUE;
	}

	return FALSE;
}