// XTPMenuBarMDIMenus.cpp : implementation of the CXTPMenuBarMDIMenus class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

CXTPMenuBarMDIMenus::CXTPMenuBarMDIMenus(CXTPMenuBar* pMenuBar)
	: m_pMenuBar(pMenuBar)
{
#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPMenuBarMDIMenus::~CXTPMenuBarMDIMenus()
{
	RemoveAll();
}

void CXTPMenuBarMDIMenus::RemoveAll()
{
	POSITION pos = m_mapMenus.GetStartPosition();
	while (pos)
	{
		UINT nIDCommand;
		CXTPMenuBarMDIMenuInfo* pInfo;
		m_mapMenus.GetNextAssoc(pos, nIDCommand, pInfo);
		pInfo->InternalRelease();
	}
	m_mapMenus.RemoveAll();
}

void CXTPMenuBarMDIMenus::Remove(UINT nIDResource)
{
	CXTPMenuBarMDIMenuInfo* pInfo = Lookup(nIDResource);
	if (pInfo)
	{
		m_mapMenus.RemoveKey(nIDResource);
		pInfo->InternalRelease();
	}
}

void CXTPMenuBarMDIMenus::SetAt(UINT nIDResource, CXTPMenuBarMDIMenuInfo* pInfo)
{
	ASSERT(pInfo);
	ASSERT(!Lookup(nIDResource));

	m_mapMenus.SetAt(nIDResource, pInfo);
}

CXTPMenuBarMDIMenuInfo* CXTPMenuBarMDIMenus::Lookup(UINT nIDResource) const
{
	CXTPMenuBarMDIMenuInfo* pInfo;
	if (!m_mapMenus.Lookup(nIDResource, pInfo))
		return NULL;

	return pInfo;
}

int CXTPMenuBarMDIMenus::GetCount() const
{
	return (int)m_mapMenus.GetCount();
}

POSITION CXTPMenuBarMDIMenus::GetStartPosition() const
{
	return m_mapMenus.GetStartPosition();
}

void CXTPMenuBarMDIMenus::GetNextMenu(POSITION& pos, CXTPMenuBarMDIMenuInfo*& pInfo) const
{
	UINT nIDResource = 0;
	m_mapMenus.GetNextAssoc(pos, nIDResource, pInfo);
}
