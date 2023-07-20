// TabClientDropTarget.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/TabClient/TabClientDropTarget.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabClientWnd::CTabClientDropTarget::CTabClientDropTarget()
{
	m_ptDragLastPoint = CPoint(-1, -1);
	m_dwDragLastTick  = 0;
}

void CXTPTabClientWnd::CTabClientDropTarget::OnDragLeave(CWnd* /*pWnd*/)
{
	m_dwDragLastTick  = 0;
	m_ptDragLastPoint = CPoint(-1, -1);
}

DROPEFFECT CXTPTabClientWnd::CTabClientDropTarget::OnDragOver(CWnd* /*pWnd*/,
															  COleDataObject* /*pDataObject*/,
															  DWORD /*dwKeyState*/, CPoint point)
{
	ASSERT(m_pTabClientWnd);

	if (!m_pTabClientWnd || !m_pTabClientWnd->GetPaintManager()->m_bSelectOnDragOver)
		return DROPEFFECT_NONE;

	if (m_dwDragLastTick != (DWORD)-1
		&& m_pTabClientWnd->GetPaintManager()->m_bSelectOnDragOver == 2)
	{
		DWORD dwTick = GetTickCount();

		if (point != m_ptDragLastPoint)
		{
			m_dwDragLastTick  = dwTick;
			m_ptDragLastPoint = point;
		}

		if (dwTick - m_dwDragLastTick > CXTPTabPaintManager::m_nSelectOnDragOverDelay)
		{
			m_dwDragLastTick = (DWORD)-1;
		}
	}
	else
	{
		CXTPTabManagerItem* pItem = m_pTabClientWnd->HitTest(point);

		if (pItem)
		{
			if (!pItem->IsSelected())
				m_pTabClientWnd->MDIActivate(CWnd::FromHandle(pItem->GetHandle()));
		}
		else
		{
			m_dwDragLastTick = 0;
		}
	}

	return DROPEFFECT_NONE;
}
